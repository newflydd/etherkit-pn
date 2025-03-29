/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
/*  This program is protected by German copyright law and international      */
/*  treaties. The use of this software including but not limited to its      */
/*  Source Code is subject to restrictions as agreed in the license          */
/*  agreement between you and Siemens.                                       */
/*  Copying or distribution is not allowed unless expressly permitted        */
/*  according to your license agreement with Siemens.                        */
/*****************************************************************************/
/*                                                                           */
/*  P r o j e c t         &P: PROFINET IO Runtime Software              :P&  */
/*                                                                           */
/*  P a c k a g e         &W: PROFINET IO Runtime Software              :W&  */
/*                                                                           */
/*  C o m p o n e n t     &C: MRP (Media Redundancy Protocol)           :C&  */
/*                                                                           */
/*  F i l e               &F: mrp_usr.c                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  user interface functions                                                 */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef MRP_MESSAGE
/*  2008-01-07  mk    No state indication if diag=0. NRT_CANCEL by           */
/*                    close_channel.                                         */
/*  2008-02-29  mk    mrp_request_synchron added.                            */
/*  2008-04-25  mk    MRP_OPC_STATE_INDICATION signaled no OFF and MRC state */
/*  2008-06-06  mk    detail_ptr released if system channel missing.         */
/*  2008-10-20  mk    MRP_RED_DIAG_ROLE_MORPHING event added.                */
/*  2008-10-28  mk    rename ROLE_MORPHING in MORPHING.                      */
/*  2009-03-19  ds    user state ind. corrected                              */
/*  2009-07-02  ds    include hierarchy changed                              */
/*  2009-10-15  ds    cleanups for function interfaces                       */
/*  2009-10-22  ds    system init changed: initialize MRP base FIRST, then   */
/*                    initialize plugin0, plugin1, plugin2                   */
/*  2010-05-12  ds    channel handling changed: channel OHA-->MRP opened by  */
/*                    OHA now, open/close handling redesigned                */
/*  2010-05-25  ds    new OHA requests: MRP_OPC_GET_STATUS,MRP_OPC_SET_STATUS*/
/*  2010-06-11  ds    common user channel functionality implemented          */
/*  2010-06-28  ds    call mrp_edd_close_channel instead of                  */
/*                    mrp_edd_req_nrt_cancel                                 */
/*  2010-07-19  ds    new feature MRP MIB implemented according to           */
/*                    IEC 62439-2                                            */
/*  2010-07-22  ds    N-IF: Upper callbacks called within macro, to          */
/*                    be able to operate in different systems                */
/*  2010-12-21  ds    N-IF: channel handling redesigned                      */
/*  2011-01-14  ds    N-IF: rework of user req map                           */
/*  2012-04-05  ds    introduction of N MRP instances per interface          */
/*  2018-10-19  hh    introduction of N MRP interconnection instances        */
/*                                                                           */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID  1
#define MRP_MODULE_ID      LTRC_ACT_MODUL_ID

#include "mrp_int.h"

MRP_FILE_SYSTEM_EXTENSION(MRP_MODULE_ID)

/*---------------------------------------------------------------------------*/
/*                                 defines                                   */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                                 types                                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                            forward declarations                           */
/*---------------------------------------------------------------------------*/
static LSA_VOID mrp_usr_request_internal (const MRP_CHANNEL_TYPE_USER pChUsr);
static LSA_UINT16 mrp_usr_get_common_usr_ch_cnt (const LSA_UINT16 nicId);
static LSA_UINT16 mrp_usr_get_usr_ch_cnt (const LSA_UINT16 nicId);
static LSA_VOID mrp_usr_add_prm_reference (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, const MRP_CONST_CHANNEL_TYPE_USER pChPrmUsr);
static LSA_VOID mrp_usr_remove_prm_reference (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, const MRP_CONST_CHANNEL_TYPE_USER pChPrmUsr);
static LSA_UINT16 mrp_usr_check_new_usr_details (const MRP_CONST_DETAIL_PTR_TYPE tmpDetailPtr, const LSA_UINT32 traceIdx, LSA_UINT16 *newEntry);
static LSA_UINT16 mrp_usr_check_new_priv_usr_details (const MRP_CONST_DETAIL_PTR_TYPE tmpDetailPtr, const LSA_UINT32 traceIdx, LSA_UINT16 *newEntry);

static LSA_UINT16 mrp_usr_ch_db_init (const MRP_CHANNEL_TYPE_USER pChUsr);
static LSA_VOID mrp_usr_ch_db_deinit (const MRP_CHANNEL_TYPE_USER pChUsr);

static LSA_VOID mrp_usr_init (const MRP_CHANNEL_TYPE_USER pChUsr, const MRP_CONST_CHANNEL_TYPE_USER pChPrmUsr);
static LSA_VOID mrp_usr_deinit (const MRP_CONST_CHANNEL_TYPE_USER pChUsr);


/*---------------------------------------------------------------------------*/
/*                                globals                                    */
/*---------------------------------------------------------------------------*/
MRP_DB_TYPE             g_Mrp;

/*===========================================================================
* FUNCTION : mrp_usr_get_common_usr_ch_cnt
*----------------------------------------------------------------------------
* PURPOSE  : get the number of common user channels still established for a 
*            network device (PRM user channels + user channels)
*----------------------------------------------------------------------------
* RETURNS  : channel cnt
*----------------------------------------------------------------------------
* INPUTS   : nicId = reference to the network device 
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_UINT16 mrp_usr_get_common_usr_ch_cnt (const LSA_UINT16 nicId)
{
  LSA_UINT16 cnt, i;
  MRP_CHANNEL_TYPE_USER pCh;

  for (i = 0, cnt = 0; i < MRP_MAX_NO_OF_INTERFACES; i++)
  {
    pCh = g_Mrp.pChPrmUsr[i];
    if (pCh->state != MRP_CH_STATE_FREE && 
        pCh->detailptr->NicId == nicId) 
    {
      cnt++;
    }
  }
  for (i = 0; i < MRP_MAX_NO_OF_INTERFACES; i++)
  {
    pCh = g_Mrp.pChUsr[i];
    if (pCh->state != MRP_CH_STATE_FREE && 
        pCh->detailptr->NicId == nicId) 
    {
      cnt++;
    }
  }

  return cnt;
}

/*===========================================================================
* FUNCTION : mrp_usr_get_usr_ch_cnt
*----------------------------------------------------------------------------
* PURPOSE  : get the number of user channels still established for a 
*            network device (PRM user channels + user channels + priv. user channels)
*----------------------------------------------------------------------------
* RETURNS  : channel cnt
*----------------------------------------------------------------------------
* INPUTS   : nicId = reference to the network device 
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_UINT16 mrp_usr_get_usr_ch_cnt (const LSA_UINT16 nicId)
{
  LSA_UINT16 cnt, i;
  MRP_CHANNEL_TYPE_USER pCh;

  for (i = 0, cnt = 0; i < MRP_MAX_NO_OF_INTERFACES; i++)
  {
    pCh = g_Mrp.pChPrmUsr[i];
    if (pCh->state != MRP_CH_STATE_FREE && 
        pCh->detailptr->NicId == nicId) 
    {
      cnt++;
    }
  }
  for (i = 0; i < MRP_MAX_NO_OF_INTERFACES; i++)
  {
    pCh = g_Mrp.pChUsr[i];
    if (pCh->state != MRP_CH_STATE_FREE && 
        pCh->detailptr->NicId == nicId) 
    {
      cnt++;
    }
  }

  for (i = 0; i < MRP_MAX_NO_OF_INTERFACES; i++)
  {
    pCh = g_Mrp.pChPrivUsr[i];
    if (pCh->state != MRP_CH_STATE_FREE && 
        pCh->detailptr->NicId == nicId) 
    {
      cnt++;
    }
  }

  return cnt;
}

/*===========================================================================
* FUNCTION : mrp_usr_get_prm_usr_channel
*----------------------------------------------------------------------------
* PURPOSE  : get prm user channel reference from internal database
*----------------------------------------------------------------------------
* RETURNS  : channel reference
*----------------------------------------------------------------------------
* INPUTS   : tmpDetailPtr = ptr to mrp details of new channel
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
MRP_CHANNEL_TYPE_USER mrp_usr_get_prm_usr_channel (const MRP_CONST_DETAIL_PTR_TYPE tmpDetailPtr)
{
  MRP_CHANNEL_TYPE_COMMON pChPrm;
  LSA_UINT16 i;

  pChPrm = LSA_NULL;

  for (i = 0; i < MRP_MAX_NO_OF_INTERFACES; i++) 
  {
    if ((g_Mrp.pChPrmUsr[i]->state == MRP_CH_STATE_READY || g_Mrp.pChPrmUsr[i]->state == MRP_CH_STATE_RESERVED) &&
         g_Mrp.pChPrmUsr[i]->detailptr->NicId == tmpDetailPtr->NicId)
    {
      pChPrm = g_Mrp.pChPrmUsr[i];
      break;
    }
  }

  return pChPrm;
}

/*===========================================================================
* FUNCTION : mrp_usr_check_new_usr_details
*----------------------------------------------------------------------------
* PURPOSE  : check new channel details, if open of new channel might be granted
*----------------------------------------------------------------------------
* RETURNS  : openRsp      = success indicator
*----------------------------------------------------------------------------
* INPUTS   : tmpDetailPtr = ptr to mrp details of new channel
*            traceIdx     = trace index for indexed trace output
* OUTPUTS  : *newEntry    = ptr to free entry index
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_UINT16 mrp_usr_check_new_usr_details (const MRP_CONST_DETAIL_PTR_TYPE tmpDetailPtr, const LSA_UINT32 traceIdx, LSA_UINT16 *newEntry)
{
  LSA_UINT16 openRsp, i;
  MRP_CHANNEL_TYPE_USER *pChusr;

#if (MRP_CFG_TRACE_MODE == 0)
  LSA_UNUSED_ARG(traceIdx);
#endif
#if (MRP_CFG_TRACE_MODE == 1)
  LSA_UNUSED_ARG(traceIdx);
#endif

  openRsp = MRP_RSP_OK;
  *newEntry = 0;

  if (tmpDetailPtr->type.user.isPrmInterface == MRP_IS_PRM_INTERFACE_ON)
  {
    pChusr = &g_Mrp.pChPrmUsr[0];
  }
  else
  {
    pChusr = &g_Mrp.pChUsr[0];
  }

  for (i = 0; i < MRP_MAX_NO_OF_INTERFACES; i++) 
  {
    if (pChusr[i]->state            == MRP_CH_STATE_READY &&
        pChusr[i]->detailptr->NicId == tmpDetailPtr->NicId)
    {
      break;
    }
  }

  if (i < MRP_MAX_NO_OF_INTERFACES)
  {
    MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> mrp_usr_check_new_usr_details: user channel for nicID=%d already established", tmpDetailPtr->NicId);
    openRsp = MRP_RSP_ERR_SYS_PATH;
  }
  else
  {
    /* get the index of a free entry */
    for (i = 0; i < MRP_MAX_NO_OF_INTERFACES; i++) 
    {
      if (pChusr[i]->state == MRP_CH_STATE_FREE)
      {
        break;
      }
    }
    if (i < MRP_MAX_NO_OF_INTERFACES)
      *newEntry = i;
    else
    {
      MRP_UPPER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> mrp_usr_check_new_usr_details: resource error");
      openRsp = MRP_RSP_ERR_RESOURCE;
    }
  }

  return openRsp;
}

/*===========================================================================
* FUNCTION : mrp_usr_check_new_priv_usr_details
*----------------------------------------------------------------------------
* PURPOSE  : check new channel details, if open of new channel might be granted
*----------------------------------------------------------------------------
* RETURNS  : openRsp      = success indicator
*----------------------------------------------------------------------------
* INPUTS   : tmpDetailPtr = ptr to mrp details of new channel 
*            traceIdx     = trace index for indexed trace output
* OUTPUTS  : *newEntry    = ptr to free entry index
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_UINT16 mrp_usr_check_new_priv_usr_details (const MRP_CONST_DETAIL_PTR_TYPE tmpDetailPtr, const LSA_UINT32 traceIdx, LSA_UINT16 *newEntry)
{
  LSA_UINT16 openRsp, i;
  MRP_CHANNEL_TYPE_USER *pChusr;

#if (MRP_CFG_TRACE_MODE == 0)
  LSA_UNUSED_ARG(traceIdx);
#endif
#if (MRP_CFG_TRACE_MODE == 1)
  LSA_UNUSED_ARG(traceIdx);
#endif
  openRsp = MRP_RSP_OK;
  *newEntry = 0;

  pChusr = &g_Mrp.pChPrivUsr[0];

  /* check, if a priv. user channel is already established for this nicID */
  for (i = 0; i < MRP_MAX_NO_OF_INTERFACES; i++) 
  {
    if (pChusr[i]->state            == MRP_CH_STATE_READY &&
        pChusr[i]->detailptr->NicId == tmpDetailPtr->NicId)
    {
      break;
    }
  }

  if (i < MRP_MAX_NO_OF_INTERFACES)
  {
    MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> mrp_usr_check_new_priv_usr_details: priv. user channel for nicID=%d already established", tmpDetailPtr->NicId);
    openRsp = MRP_RSP_ERR_SYS_PATH;
  }
  else
  {
    /* get the index of a free entry */
    for (i = 0; i < MRP_MAX_NO_OF_INTERFACES; i++) 
    {
      if (pChusr[i]->state == MRP_CH_STATE_FREE)
      {
        break;
      }
    }
    if (i < MRP_MAX_NO_OF_INTERFACES)
    {
      /* there is still no PRIV user channel for this nicID */
      *newEntry = i;

      /* check the sequence: a PRM user and COMMON user channel must already have been established!! */
      for (i = 0; i < MRP_MAX_NO_OF_INTERFACES; i++) 
      {
        if (g_Mrp.pChPrmUsr[i]->state            == MRP_CH_STATE_READY &&
            g_Mrp.pChPrmUsr[i]->detailptr->NicId == tmpDetailPtr->NicId)
        {
          break;
        }
      }
      if (i < MRP_MAX_NO_OF_INTERFACES)
      {
        for (i = 0; i < MRP_MAX_NO_OF_INTERFACES; i++) 
        {
          if (g_Mrp.pChUsr[i]->state            == MRP_CH_STATE_READY &&
              g_Mrp.pChUsr[i]->detailptr->NicId == tmpDetailPtr->NicId)
          {
            break;
          }
        }
        if (i >= MRP_MAX_NO_OF_INTERFACES)
        {
          MRP_UPPER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> mrp_usr_check_new_priv_usr_details: sequence error (NO PRM/Common user chan available)");
          openRsp = MRP_RSP_ERR_SEQUENCE;
        }
      }
    }
    else
    {
      MRP_UPPER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> mrp_usr_check_new_priv_usr_details: resource error");
      openRsp = MRP_RSP_ERR_RESOURCE;
    }
  }

  return openRsp;
}

/*===========================================================================
* FUNCTION : mrp_usr_add_prm_reference
*----------------------------------------------------------------------------
* PURPOSE  : add a reference to prm database to user channel(s) 
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr    = user channel referenced
*            pChPrmUsr = user channel with prm capability
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_usr_add_prm_reference (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, const MRP_CONST_CHANNEL_TYPE_USER pChPrmUsr)
{
  LSA_UINT16 i;
  MRP_CHANNEL_TYPE_COMMON pCh;

  if (pChUsr == LSA_NULL)
  {    
    MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pChPrmUsr, LSA_NULL));

    for (pCh = &g_Mrp.channels[0], i = 0; i < MRP_MAX_CHANNELS; i++, pCh++) 
    {
      if (pCh->state != MRP_CH_STATE_FREE             &&
          pCh->detailptr->PathType != MRP_PATH_TYPE_EDD && 
          pCh->detailptr->NicId == pChPrmUsr->detailptr->NicId) 
      {
        pCh->pChUsrDB->pRec = pChPrmUsr->pChUsrDB->pRec;
      }
    }
  }
  else
  {
    pChUsr->pChUsrDB->pRec = pChPrmUsr->pChUsrDB->pRec;
  }
}

/*===========================================================================
* FUNCTION : mrp_usr_remove_prm_reference
*----------------------------------------------------------------------------
* PURPOSE  : remove a reference to prm database from user channel(s) 
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr    = user channel referenced
*            pChPrmUsr = user channel with prm capability
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_usr_remove_prm_reference (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, const MRP_CONST_CHANNEL_TYPE_USER pChPrmUsr)
{
  LSA_UINT16 i;
  MRP_CHANNEL_TYPE_COMMON pCh;

  if (pChUsr == LSA_NULL)
  {
    MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pChPrmUsr, LSA_NULL));
   
    for (pCh = &g_Mrp.channels[0], i = 0; i < MRP_MAX_CHANNELS; i++, pCh++) 
    {
      if (pCh->state               != MRP_CH_STATE_FREE && 
          pCh->detailptr->PathType != MRP_PATH_TYPE_EDD &&
          (pChPrmUsr != LSA_NULL && pCh->detailptr->NicId == pChPrmUsr->detailptr->NicId))
      {
        pCh->pChUsrDB->pRec = LSA_NULL;
      }
    }
  }
  else
  {
    pChUsr->pChUsrDB->pRec = LSA_NULL;
  }
}

/*===========================================================================
* FUNCTION : mrp_usr_call_upper_cbf
*----------------------------------------------------------------------------
* PURPOSE  : call upper cbf and put RQB back to upper layer, while other requests 
*            might be pending
*            (don't cancel these requests by setting pCh->pRQB = LSA_NULL)
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pCh  = user/system channel reference
*            pRQB = rqb reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : used for indication requests.
*==========================================================================*/
LSA_VOID mrp_usr_call_upper_cbf (const MRP_CONST_CHANNEL_TYPE_COMMON pCh, MRP_UPPER_RQB_PTR_TYPE pRQB)
{
  MRP_RQB_SET_HANDLE (pRQB, pCh->handle_upper);
  MRP_REQUEST_UPPER_DONE (pCh->pCbf, pRQB, pCh->sysptr);
}


/*===========================================================================
* FUNCTION : mrp_usr_call_upper_cbf_proc_next_req
*----------------------------------------------------------------------------
* PURPOSE  : put RQB back to upper layer and finish request (pChUsr->pRQB = LSA_NULL)
*            look for another usr request already queued
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr = user channel reference
*            pRQB   = rqb reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_usr_call_upper_cbf_proc_next_req (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB)
{
  mrp_usr_call_upper_cbf (pChUsr, pRQB);
  pChUsr->pRQB = LSA_NULL;

  mrp_usr_request_internal (pChUsr);
}


/*===========================================================================
* FUNCTION : mrp_usr_state_ind_done
*----------------------------------------------------------------------------
* PURPOSE  : indicate state change
*----------------------------------------------------------------------------
* RETURNS  : LSA_TRUE  = RQB in queue
*            LSA_FALSE = no RQB in queue
*----------------------------------------------------------------------------
* INPUTS   : pChPrivUsr = channel reference for private user channel (USER2)
*            instance   = affected MRP instance 
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT8 mrp_usr_state_ind_done (const MRP_CONST_CHANNEL_TYPE_USER pChPrivUsr, LSA_UINT16 instance)
{
#ifdef MRP_CFG_PLUGIN_0_MRM
  MRP_MRM_PARAMS_TYPE paramsMRM;
  MRP_MRM_STATE_TYPE mrpState;
#endif
  MRP_MRC_PARAMS_TYPE paramsMRC;
  MRP_UPPER_RQB_PTR_TYPE pRQB;
  MRP_UPPER_ARGS_PTR_TYPE pArgs;
  LSA_UINT16 resp = MRP_RSP_OK;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE*)pChPrivUsr->pChSysDB->pEdd;
  MRP_USR_DB_TYPE *pDBusr = (MRP_USR_DB_TYPE *)pChPrivUsr->pChUsrDB->pUsr;
  MRP_USR_INST_DATA *pDBusrInst;

  pDBusrInst = &pDBusr->pInstData[instance];

  pDBusrInst->StateIndCurrent.usrStateInd.mrpDiag = 0;
  pDBusrInst->StateIndCurrent.usrStateInd.ssrAddInfo = 0;
  pDBusrInst->StateIndCurrent.usrStateInd.mrpDiagMultipleOtherMRMs  = MRP_DIAG_MULTIPLE_OTHER_RM_NO;
  MRP_MEMSET(pDBusrInst->StateIndCurrent.usrStateInd.mrpDiagMACAddrOtherMRM, 0, MRP_PACKET_VAL_MAC_ADDR_LEN);

  /* get infos */
#ifdef MRP_CFG_PLUGIN_0_MRM
  mrp_mrm_get_params (pDBedd->pChSys, instance, &paramsMRM);
  if (paramsMRM.Mode == MRP_OPERATION_ON) 
  {
    (void)(mrp_mrm_get_state (pDBedd->pChSys, instance, &mrpState, LSA_FALSE));
    if (!mrpState.mrmVotingLost) 
    {
      if (mrpState.roleMgr)
      {
        pDBusrInst->StateIndCurrent.usrStateInd.mrpDiag |= MRP_RED_DIAG_ROLE_MRM;
        if (paramsMRM.operRole == MRP_OPER_ROLE_AUTO_MGR)
        {
          pDBusrInst->StateIndCurrent.usrStateInd.mrpDiag |= MRP_RED_DIAG_ROLE_MRM_AUTO;        
        }
      }
      else
        pDBusrInst->StateIndCurrent.usrStateInd.mrpDiag |= MRP_RED_DIAG_ROLE_MRC;
      
      if (mrpState.roleMgr && mrpState.singleSideRx) 
      {
        pDBusrInst->StateIndCurrent.usrStateInd.mrpDiag |= MRP_RED_DIAG_MRM_SINGLE_SIDE_RCV;
        pDBusrInst->StateIndCurrent.usrStateInd.ssrAddInfo = paramsMRM.Not_rcv_RPort;
      }
      else
      {
        if (mrpState.roleMgr)
        {
          /* MRM_state is not equal chk_rc */
          if (mrpState.ringOpen)
            pDBusrInst->StateIndCurrent.usrStateInd.mrpDiag |= MRP_RED_DIAG_MRM_RING_OPEN;
          else
          {
            /* MRM_state is chk_rc, but NO MRP testframe was received yet */
            if (!mrpState.morphing && mrpState.noTC)
            {
              pDBusrInst->StateIndCurrent.usrStateInd.mrpDiag |= MRP_RED_DIAG_MRM_RING_OPEN;
              MRP_UPPER_TRACE_01 (pChPrivUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_usr_state_ind_done[inst=%d] special case: MRM state chk_rc, but flag NoTC set", instance);
            }
          }
        }
      }

      if (mrpState.roleMgr && mrpState.multiRM) 
      {
        pDBusrInst->StateIndCurrent.usrStateInd.mrpDiag |= MRP_RED_DIAG_MRM_OTHER;
        pDBusrInst->StateIndCurrent.usrStateInd.mrpDiagMultipleOtherMRMs  = mrpState.multiRMMultipleOtherRM;
        MRP_MEMCPY(pDBusrInst->StateIndCurrent.usrStateInd.mrpDiagMACAddrOtherMRM, mrpState.multiRMSAOtherMRM, MRP_PACKET_VAL_MAC_ADDR_LEN);
      }
      if (mrpState.roleMgr && mrpState.morphing)
      {
        pDBusrInst->StateIndCurrent.usrStateInd.mrpDiag |= MRP_RED_DIAG_MRM_MORPHING;
      }
    }
    else 
    {
      pDBusrInst->StateIndCurrent.usrStateInd.mrpDiag |= MRP_RED_DIAG_ROLE_MRC;
    }
  }
#endif
  mrp_mrc_get_params (pDBedd->pChSys, instance, &paramsMRC);  
  if (paramsMRC.Mode == MRP_OPERATION_ON) 
  {
    pDBusrInst->StateIndCurrent.usrStateInd.mrpDiag |= MRP_RED_DIAG_ROLE_MRC;
  }
#ifdef MRP_CFG_PLUGIN_2_HSR
  {
    MRP_MODE_T mode;
    LSA_UINT16 portA; 
    LSA_UINT16 portB;

    mrp_bs_get_ring_ports_and_mode (pDBedd->pChSys, &portA, &portB, &mode);    
    if (MRP_MODE_T_RC == mode || MRP_MODE_T_RM == mode) 
    {
      pDBusrInst->StateIndCurrent.usrStateInd.mrpDiag |= MRP_RED_DIAG_ROLE_HSR;
    }
  }
#endif
  if (!pDBusrInst->StateIndCurrent.usrStateInd.mrpDiag) 
  {
    pDBusrInst->StateIndCurrent.usrStateInd.mrpDiag = MRP_RED_DIAG_ROLE_OFF;
  }
  if (pChPrivUsr->state == MRP_CH_STATE_CLOSING   ||
      (pDBusrInst->StateIndLastSignaled.usrStateInd.mrpDiag != pDBusrInst->StateIndCurrent.usrStateInd.mrpDiag) ||
      (pDBusrInst->StateIndLastSignaled.usrStateInd.mrpDiagMultipleOtherMRMs != pDBusrInst->StateIndCurrent.usrStateInd.mrpDiagMultipleOtherMRMs)
    ) 
  {
    MRP_RQB_POP_FROM_TOP(pDBusrInst->pStateIndRqbB, pDBusrInst->pStateIndRqbT, pRQB);
    if (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL)) 
    {
      MRP_UPPER_TRACE_02(pChPrivUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "<< MRP_OPC_STATE_INDICATION done[inst=%d]: out of resource, myHandle =%d", instance, pChPrivUsr->myHandle);
      pDBusrInst->StateIndOutOfResource = MRP_IND_RESOURCE_MISSING_ON;

      return LSA_FALSE;
    }

    pArgs = MRP_RQB_GET_ARGS_PTR(pRQB);
    
    pArgs->state.state_ind.usrStateInd.mrpDiag = pDBusrInst->StateIndLastSignaled.usrStateInd.mrpDiag = pDBusrInst->StateIndCurrent.usrStateInd.mrpDiag;
    pArgs->state.state_ind.usrStateInd.ssrAddInfo = pDBusrInst->StateIndLastSignaled.usrStateInd.ssrAddInfo = pDBusrInst->StateIndCurrent.usrStateInd.ssrAddInfo;
    pArgs->state.state_ind.usrStateInd.mrpDiagMultipleOtherMRMs = pDBusrInst->StateIndLastSignaled.usrStateInd.mrpDiagMultipleOtherMRMs = pDBusrInst->StateIndCurrent.usrStateInd.mrpDiagMultipleOtherMRMs;
    MRP_MEMCPY(pDBusrInst->StateIndLastSignaled.usrStateInd.mrpDiagMACAddrOtherMRM, pDBusrInst->StateIndCurrent.usrStateInd.mrpDiagMACAddrOtherMRM, MRP_PACKET_VAL_MAC_ADDR_LEN);
    MRP_MEMCPY(pArgs->state.state_ind.usrStateInd.mrpDiagMACAddrOtherMRM, pDBusrInst->StateIndLastSignaled.usrStateInd.mrpDiagMACAddrOtherMRM, MRP_PACKET_VAL_MAC_ADDR_LEN);

    if (pChPrivUsr->state == MRP_CH_STATE_CLOSING) 
    {
      resp = MRP_RSP_OK_CANCEL;
      MRP_RQB_SET_RESPONSE(pRQB, resp);
    }
    MRP_UPPER_TRACE_05 (pChPrivUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "<< MRP_OPC_STATE_INDICATION done[inst=%d]: cbf called, myHandle =%d, resp=0x%x, mrpDiag=0x%x, ssrAddInfo=%d",
                        instance, pChPrivUsr->myHandle, resp, pArgs->state.state_ind.usrStateInd.mrpDiag, pArgs->state.state_ind.usrStateInd.ssrAddInfo);
    mrp_usr_call_upper_cbf (pChPrivUsr, pRQB);
  }
  return LSA_TRUE;
}

/*===========================================================================
* FUNCTION : mrp_usr_state_ind_provide_rqb
*----------------------------------------------------------------------------
* PURPOSE  : set indication rqbs for signaling 
*----------------------------------------------------------------------------
* RETURNS  : MRP_RET_OK/MRP_RET_ERR
*----------------------------------------------------------------------------
* INPUTS   : pChPrivUsr = channel reference to private user channel (USER2)
*            pRQB       = upper rqb reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : queues indication requests 
*==========================================================================*/
LSA_UINT16 mrp_usr_state_ind_provide_rqb (const MRP_CHANNEL_TYPE_USER pChPrivUsr, MRP_UPPER_RQB_PTR_TYPE pRQB)
{
  LSA_UINT16 instance;
  MRP_UPPER_ARGS_PTR_TYPE pArgs;
  MRP_USR_DB_TYPE *pDBusr = (MRP_USR_DB_TYPE *)pChPrivUsr->pChUsrDB->pUsr;

  pArgs = MRP_RQB_GET_ARGS_PTR(pRQB);

  MRP_ASSERT (pArgs->state.state_ind.usrStateInd.mrpInstance > 0 && pArgs->state.state_ind.usrStateInd.mrpInstance <= EDD_CFG_MAX_MRP_INSTANCE_CNT);
  instance = pArgs->state.state_ind.usrStateInd.mrpInstance - 1;

  if (pChPrivUsr->state != MRP_CH_STATE_CLOSING) 
  {
    if (instance < mrp_sys_get_num_of_instances(pChPrivUsr))
    {
      MRP_UPPER_TRACE_05(pChPrivUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, ">> MRP_OPC_STATE_INDICATION[inst=%d]: rqb provided, pChPrivUsr=0x%x, state=%d, myHandle=%d, outOfRes=%d, resp=MRP_RSP_OK", 
        instance, pChPrivUsr, pChPrivUsr->state, pChPrivUsr->myHandle, pDBusr->pInstData[instance].StateIndOutOfResource);

      MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_OK);
      MRP_RQB_PUSH_TO_BOTTOM(pDBusr->pInstData[instance].pStateIndRqbB, pDBusr->pInstData[instance].pStateIndRqbT, pRQB);
      pChPrivUsr->pRQB = LSA_NULL;

      if (pDBusr->pInstData[instance].StateIndOutOfResource) 
      {
        pDBusr->pInstData[instance].StateIndOutOfResource = MRP_IND_RESOURCE_MISSING_OFF;

        (void)(mrp_usr_state_ind_done (pChPrivUsr, instance));
      }
      return MRP_RET_OK;
    }
    else
    {
      MRP_UPPER_TRACE_04(pChPrivUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> MRP_OPC_STATE_INDICATION[inst=%d]: rqb provided, pChPrivUsr=0x%x, state=%d, myHandle=%d, resp=MRP_RSP_ERR_PRM_DATA",
        instance, pChPrivUsr, pChPrivUsr->state, pChPrivUsr->myHandle);

      MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_PRM_DATA);

      return MRP_RET_ERR;
    }
  }
  else
  {
    MRP_UPPER_TRACE_04(pChPrivUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> MRP_OPC_STATE_INDICATION[inst=%d]: rqb provided, pChPrivUsr=0x%x, state=%d, myHandle=%d, resp=MRP_RSP_ERR_SEQUENCE",
      instance, pChPrivUsr, pChPrivUsr->state, pChPrivUsr->myHandle);

    MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_SEQUENCE);

    return MRP_RET_ERR;
  }
}


/*===========================================================================
* FUNCTION : mrp_usr_write_red_init_state
*----------------------------------------------------------------------------
* PURPOSE  : write the current init state of redundancy modules to usr database
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr       = channel reference to user channel
*            instance     = affected MRP instance 
*            initState    = init module, which has to be set/reset
*            setFlag      = LSA_TRUE := set initState, LSA_FALSE := reset initState
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_usr_write_red_init_state (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance, MRP_USR_ENUM_RED_STATE_TYPE initState, LSA_UINT8 setFlag)
{
  MRP_CHANNEL_TYPE_USER pChPrmUsr;
  MRP_USR_DB_TYPE *pDBusr;
  MRP_USR_INST_DATA *pDBusrInst;
  LSA_UINT8 change = LSA_TRUE;

  pChPrmUsr = mrp_usr_get_prm_usr_channel (pChUsr->detailptr);

  if (!LSA_HOST_PTR_ARE_EQUAL(pChPrmUsr, LSA_NULL))
  {
    /* the init state must only be written, if this function is called during establishment of PRM user channel */
    if (pChUsr->detailptr->PathType == MRP_PATH_TYPE_USER && pChUsr->detailptr->type.user.isPrmInterface == MRP_IS_PRM_INTERFACE_ON)
    {
      pDBusr = (MRP_USR_DB_TYPE *)pChUsr->pChUsrDB->pUsr;
      pDBusrInst = &pDBusr->pInstData[instance];

      MRP_PROGRAM_TRACE_04(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_usr_write_red_init_state[inst=%d]: initState to change=0x%x, setFlag=%d, current initState=0x%x", instance, initState, setFlag, pDBusrInst->RedStateIndCurrent.redStateInd.initState);

      if (setFlag)
      {
        if ( (pDBusrInst->RedStateIndCurrent.redStateInd.initState & initState) == initState)
        {
          change = LSA_FALSE;
        }

        pDBusrInst->RedStateIndCurrent.redStateInd.initState |= initState;
      }
      else
      {
        if ((pDBusrInst->RedStateIndCurrent.redStateInd.initState & initState) == 0)
        {
          change = LSA_FALSE;
        }

        pDBusrInst->RedStateIndCurrent.redStateInd.initState &= ~initState;
      }

      if (change)
      {
        MRP_PROGRAM_TRACE_02(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_usr_write_red_init_state[inst=%d]: new current initState=0x%x", instance, pDBusrInst->RedStateIndCurrent.redStateInd.initState);
      }
    }
  }
  else
  {
    MRP_PROGRAM_TRACE_01(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_usr_write_red_init_state[inst=%d]: pChPrmUsr must not be NULL", instance);
  }
}

/*===========================================================================
* FUNCTION : mrp_usr_red_state_init
*----------------------------------------------------------------------------
* PURPOSE  : initialize the current init state of redundancy modules to usr database
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr       = channel reference to a user channel
*            instance     = affected MRP instance 
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_usr_red_state_init (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance)
{
  LSA_UINT16 i;
  MRP_USR_INST_DATA *pDBusrInst;
  MRP_USR_DB_TYPE *pDBusr = (MRP_USR_DB_TYPE *)pChUsr->pChUsrDB->pUsr;
  MRP_IF_DB_TYPE  *pDBif  = (MRP_IF_DB_TYPE *)pChUsr->pChSysDB->pInterfacePara;

  MRP_PROGRAM_TRACE_03(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_usr_red_state_init entered: pChusr=0x%x, myHandle=%d, instance=%d", pChUsr, pChUsr->myHandle, instance);

  pDBusrInst = &pDBusr->pInstData[instance];

  MRP_MEMSET (&pDBusrInst->RedStateIndCurrent.redStateInd, 0, sizeof (MRP_OPC_RED_STATE_INDICATION_TYPE));

  pDBusrInst->RedStateIndCurrent.redStateInd.mrpInstance = instance + 1;
  pDBusrInst->RedStateIndLastSignaled.redStateInd.mrpInstance = instance + 1;

  if (pChUsr->detailptr->PathType == MRP_PATH_TYPE_USER && pChUsr->detailptr->type.user.isPrmInterface == MRP_IS_PRM_INTERFACE_ON)
  {
    if (!instance)
    {
      if (pDBif->mrpSupportedRoleInst0 & EDD_MRP_ROLE_CAP_CLIENT)
      {
        mrp_usr_write_red_init_state (pChUsr, instance, MRP_USR_ENUM_STATE_MRC, LSA_TRUE);
      }
      if (pDBif->mrpSupportedRoleInst0 & EDD_MRP_ROLE_CAP_MANAGER)
      {
        mrp_usr_write_red_init_state (pChUsr, instance, MRP_USR_ENUM_STATE_MRM, LSA_TRUE);
      }
      if (pDBif->mrpSupportedRoleInst0 & EDD_MRP_ROLE_CAP_AUTOMANAGER)
      {
        mrp_usr_write_red_init_state (pChUsr, instance, MRP_USR_ENUM_STATE_AUTO_MRM, LSA_TRUE);
      }
#ifdef  MRP_CFG_PLUGIN_2_HSR
      mrp_usr_write_red_init_state (pChUsr, instance, MRP_USR_ENUM_STATE_HSR_RC, LSA_TRUE);
      mrp_usr_write_red_init_state (pChUsr, instance, MRP_USR_ENUM_STATE_HSR_MGR, LSA_TRUE);
      mrp_usr_write_red_init_state (pChUsr, instance, MRP_USR_ENUM_STATE_STBY, LSA_TRUE);
      mrp_usr_write_red_init_state (pChUsr, instance, MRP_USR_ENUM_STATE_ARD, LSA_TRUE);
#endif /* MRP_CFG_PLUGIN_2_HSR */

      if (pDBif->mrpInMaxInstance)
      {
        LSA_UINT16  pInstance[2];
        pInstance[MRP_INSTANCE_INDEX]     = instance;

        mrp_usr_write_red_init_state (pChUsr, instance, MRP_USR_ENUM_STATE_IN, LSA_TRUE);

        MRP_FATAL1 (pDBif->mrpInMaxInstance <= EDD_CFG_MAX_MRP_IN_INSTANCE_CNT);

        for (i = 0; i < pDBif->mrpInMaxInstance; i++)
        {
          pInstance[MRP_IN_INSTANCE_INDEX]  = i;

          if (pDBif->mrpInSupportedRoleInst0 & EDD_SUPPORTED_MRP_INTERCONN_ROLE_CAP_CLIENT)
            mrp_in_write_red_init_state (pChUsr, pInstance, MRP_USR_ENUM_STATE_IN_MIC, LSA_TRUE);

          if (pDBif->mrpInSupportedRoleInst0 & EDD_SUPPORTED_MRP_INTERCONN_ROLE_CAP_MANAGER)
            mrp_in_write_red_init_state (pChUsr, pInstance, MRP_USR_ENUM_STATE_IN_MIM, LSA_TRUE);
        }
      }
    }
    else
    {
      if (pDBif->mrpSupportedMultiRole & EDD_MRP_ROLE_CAP_CLIENT)
      {
        mrp_usr_write_red_init_state (pChUsr, instance, MRP_USR_ENUM_STATE_MRC, LSA_TRUE);
      }
      if (pDBif->mrpSupportedMultiRole & EDD_MRP_ROLE_CAP_MANAGER)
      {
        mrp_usr_write_red_init_state (pChUsr, instance, MRP_USR_ENUM_STATE_MRM, LSA_TRUE);
      }
      if (pDBif->mrpSupportedMultiRole & EDD_MRP_ROLE_CAP_AUTOMANAGER)
      {
        mrp_usr_write_red_init_state (pChUsr, instance, MRP_USR_ENUM_STATE_AUTO_MRM, LSA_TRUE);
      }
    }
  }
}

/*===========================================================================
* FUNCTION : mrp_usr_red_state_deinit
*----------------------------------------------------------------------------
* PURPOSE  : uninitialize the current init state of redundancy modules
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr       = channel reference to a user channel
*            instance     = affected MRP instance 
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_usr_red_state_deinit (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance)
{
  MRP_USR_INST_DATA *pDBusrInst;
  MRP_USR_DB_TYPE *pDBusr = (MRP_USR_DB_TYPE *)pChUsr->pChUsrDB->pUsr;

  MRP_PROGRAM_TRACE_03(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_usr_red_state_deinit entered: pChusr=0x%x, myHandle=%d, instance=%d", pChUsr, pChUsr->myHandle, instance);

  pDBusrInst = &pDBusr->pInstData[instance];

  pDBusrInst->RedStateIndCurrent.redStateInd.mrpInstance = instance + 1;
  pDBusrInst->RedStateIndLastSignaled.redStateInd.mrpInstance = instance + 1;

  if (pChUsr->detailptr->PathType == MRP_PATH_TYPE_USER_2) 
  {
    if (!pDBusrInst->RedStateIndCurrent.redStateInd.startState)
    {
      pDBusrInst->RedStateIndCurrent.redStateInd.rport1 = 0;
      pDBusrInst->RedStateIndCurrent.redStateInd.rport2 = 0;

      MRP_MEMSET (&pDBusrInst->RedStateIndCurrent.redStateInd.stdbyPorts[0], 0, sizeof (LSA_UINT16) * MRP_MAX_NO_OF_STDBY_PORTS);

      MRP_MEMSET (&pDBusrInst->RedStateIndCurrent.redStateInd.inState[0], 0, sizeof (MRP_RED_IN_STATE_TYPE) * (LSA_UINT32)EDD_CFG_MAX_MRP_IN_INSTANCE_CNT);
      /* call of mrp_in_write_red_init_state (pChUsr, instance, i, MRP_USR_ENUM_STATE_IN_MIC, LSA_FALSE); obsolete */
      /* call of mrp_in_write_red_init_state (pChUsr, instance, i, MRP_USR_ENUM_STATE_IN_MIM, LSA_FALSE); obsolete */

      if (!instance)
      {
        mrp_usr_write_red_init_state (pChUsr, instance, MRP_USR_ENUM_STATE_IN, LSA_FALSE);
      }

      mrp_usr_write_red_init_state (pChUsr, instance, MRP_USR_ENUM_STATE_MRC, LSA_FALSE);
      mrp_usr_write_red_init_state (pChUsr, instance, MRP_USR_ENUM_STATE_MRM, LSA_FALSE);
      mrp_usr_write_red_init_state (pChUsr, instance, MRP_USR_ENUM_STATE_AUTO_MRM, LSA_FALSE);
#ifdef  MRP_CFG_PLUGIN_2_HSR
      mrp_usr_write_red_init_state (pChUsr, instance, MRP_USR_ENUM_STATE_HSR_RC, LSA_FALSE);
      mrp_usr_write_red_init_state (pChUsr, instance, MRP_USR_ENUM_STATE_HSR_MGR, LSA_FALSE);
      mrp_usr_write_red_init_state (pChUsr, instance, MRP_USR_ENUM_STATE_STBY, LSA_FALSE);
      mrp_usr_write_red_init_state (pChUsr, instance, MRP_USR_ENUM_STATE_ARD, LSA_FALSE);
#endif /* MRP_CFG_PLUGIN_2_HSR */
    }
  }
}

/*===========================================================================
* FUNCTION : mrp_usr_write_red_start_state
*----------------------------------------------------------------------------
* PURPOSE  : write the current start state of redundancy modules to usr database
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys       = channel reference to system channel
*            instance     = affected MRP instance 
*            startState   = start module flag, which has to be set/reset
*            rport1       = first ringport of the started/stopped redundancy module
*            rport2       = second ringport of the started/stopped redundancy module
*            pStdbyPorts  = pointer to array of standby ports
*            setFlag      = LSA_TRUE := set startState, LSA_FALSE := reset startState
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_usr_write_red_start_state (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_USR_ENUM_RED_STATE_TYPE startState, LSA_UINT16 rport1, LSA_UINT16 rport2, const LSA_UINT8 *pStdbyPorts, LSA_UINT8 setFlag)
{
  LSA_UINT16 i;
  MRP_CHANNEL_TYPE_USER pChPrmUsr;
  MRP_USR_DB_TYPE *pDBusr;
  MRP_USR_INST_DATA *pDBusrInst; 
  LSA_UINT8 change = LSA_TRUE;
  MRP_IF_DB_TYPE  *pDBif = (MRP_IF_DB_TYPE *)pChSys->pChSysDB->pInterfacePara;
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  pChPrmUsr = mrp_usr_get_prm_usr_channel (pChSys->detailptr);

  if (!LSA_HOST_PTR_ARE_EQUAL(pChPrmUsr, LSA_NULL))
  {
    /* PRM is only possible, if a PRM user channel was established before -> startState data are linked to PRM user channel */
    /* If the startState is linked to usr2 channel, some of the provisioning action would probably have been completed before. */
    pDBusr = (MRP_USR_DB_TYPE *)pChPrmUsr->pChUsrDB->pUsr;
    pDBusrInst = &pDBusr->pInstData[instance];
    
    MRP_PROGRAM_TRACE_04(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_usr_write_red_start_state[inst=%d]: startState to change=0x%x, setFlag=%d, current startState=0x%x", instance, startState, setFlag, pDBusrInst->RedStateIndCurrent.redStateInd.startState);
    
    if (setFlag)
    {
      if ( (pDBusrInst->RedStateIndCurrent.redStateInd.startState & startState) == startState)
        change = LSA_FALSE;

      /* if MRM becomes operational, MRC is deactivated automatically */
      if (startState & MRP_USR_ENUM_STATE_MRM)
        pDBusrInst->RedStateIndCurrent.redStateInd.startState &= ~MRP_USR_ENUM_STATE_MRC;

#ifdef MRP_CFG_PLUGIN_2_HSR
      /* if HSR client becomes operational, MRC is deactivated automatically */
      if (startState & MRP_USR_ENUM_STATE_HSR_RC)
        pDBusrInst->RedStateIndCurrent.redStateInd.startState &= ~MRP_USR_ENUM_STATE_MRC;

      /* if HSR manager becomes operational, MRC is deactivated automatically */
      if (startState & MRP_USR_ENUM_STATE_HSR_MGR)
        pDBusrInst->RedStateIndCurrent.redStateInd.startState &= ~MRP_USR_ENUM_STATE_MRC;
#endif

      /* if MRC becomes operational, MRM is deactivated automatically */
      if (startState & MRP_USR_ENUM_STATE_MRC)
        pDBusrInst->RedStateIndCurrent.redStateInd.startState &= ~MRP_USR_ENUM_STATE_MRM;

      pDBusrInst->RedStateIndCurrent.redStateInd.startState |= startState;
    }
    else /* setFlag == LSA_FALSE */
    {
      if ((pDBusrInst->RedStateIndCurrent.redStateInd.startState & startState) == 0)
        change = LSA_FALSE;

      pDBusrInst->RedStateIndCurrent.redStateInd.startState &= ~startState;
    }


    /* neither MRP nor MRP interconnection active at this instance */
    if (!pDBusrInst->RedStateIndCurrent.redStateInd.startState)
    {
      pDBusrInst->RedStateIndCurrent.redStateInd.rport1 = 0;
      pDBusrInst->RedStateIndCurrent.redStateInd.rport2 = 0;

      MRP_MEMSET (&pDBusrInst->RedStateIndCurrent.redStateInd.stdbyPorts[0], 0, sizeof (LSA_UINT16) * MRP_MAX_NO_OF_STDBY_PORTS);

      if (pDBif->mrpInMaxInstance)
      {
        MRP_FATAL1 (pDBif->mrpInMaxInstance <= EDD_CFG_MAX_MRP_IN_INSTANCE_CNT);

        for (i = 0; i < pDBif->mrpInMaxInstance; i++)
        {
          /* don't overwrite the initState of the MRP interconnection instance -> it has to be kept! */
          pDBusrInst->RedStateIndCurrent.redStateInd.inState[i].inId       = 0;
          pDBusrInst->RedStateIndCurrent.redStateInd.inState[i].inPortId   = 0;
          pDBusrInst->RedStateIndCurrent.redStateInd.inState[i].startState = 0;
        }
      }
    }
    else /* any other parameters changed ??? */
    {
#ifndef MRP_CFG_PLUGIN_2_HSR
      pDBusrInst->RedStateIndCurrent.redStateInd.rport1 = rport1;
      pDBusrInst->RedStateIndCurrent.redStateInd.rport2 = rport2;

      MRP_ASSERT(pStdbyPorts == LSA_NULL);
      LSA_UNUSED_ARG (pStdbyPorts);
#else
      if (startState & MRP_USR_ENUM_STATE_STBY) /* standby configuration change */
      {
        if (setFlag)
        {
          if (!LSA_HOST_PTR_ARE_EQUAL(pStdbyPorts, LSA_NULL))
          {
            LSA_UINT16 k;

            for (i = 0, k = 0; i < mrp_sys_get_num_of_ports(pChSys); i++)
            {
              if (pStdbyPorts[i] == MRP_OPERATION_ON)
              {
                MRP_ASSERT(k < MRP_MAX_NO_OF_STDBY_PORTS);
                pDBusrInst->RedStateIndCurrent.redStateInd.stdbyPorts[k] = i + 1;
                MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_usr_write_red_start_state[inst=%d]: standbyPort[%d]=%d written", instance, k, i + 1);
                k++;
              }
            }
          }
          else
            MRP_PROGRAM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_usr_write_red_start_state[inst=%d]: pStdbyPorts must not be NULL", instance);
        }
        else
        {
          MRP_MEMSET (&pDBusrInst->RedStateIndCurrent.redStateInd.stdbyPorts[0], 0, sizeof (LSA_UINT16) * MRP_MAX_NO_OF_STDBY_PORTS);
        }
      }
      else /* redundancy configuration change */
      {
        pDBusrInst->RedStateIndCurrent.redStateInd.rport1 = rport1;
        pDBusrInst->RedStateIndCurrent.redStateInd.rport2 = rport2;
      }
#endif
    }

    if (change)
    {
      MRP_PROGRAM_TRACE_04(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_usr_write_red_start_state[inst=%d]: new current startState=0x%x, rport1=%d, rport2=%d", instance, pDBusrInst->RedStateIndCurrent.redStateInd.startState, pDBusrInst->RedStateIndCurrent.redStateInd.rport1, pDBusrInst->RedStateIndCurrent.redStateInd.rport2);
    }
  }
  else
  {
    MRP_PROGRAM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_usr_write_red_start_state[inst=%d]: pChPrmUsr must not be NULL", instance);
  }
}


/*===========================================================================
* FUNCTION : mrp_usr_get_current_redstate
*----------------------------------------------------------------------------
* PURPOSE  : get current redundancy state
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChPrivUsr   = channel reference
*            instance     = affected MRP instance 
*            pMrpRedStateCurrent  = pointer to struct of type MRP_OPC_RED_STATE_INDICATION_TYPE
* OUTPUTS  : *pMrpRedStateCurrent = current redState data
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_usr_get_current_redstate (const MRP_CONST_CHANNEL_TYPE_USER pChPrivUsr, LSA_UINT16 instance, MRP_OPC_RED_STATE_INDICATION_TYPE *pMrpRedStateCurrent)
{
  MRP_CHANNEL_TYPE_USER pChPrmUsr;
  MRP_USR_DB_TYPE *pDBusr;

  pChPrmUsr = mrp_usr_get_prm_usr_channel (pChPrivUsr->detailptr);

  if (!LSA_HOST_PTR_ARE_EQUAL(pChPrmUsr, LSA_NULL))
  {
    /* PRM is only possible, if a PRM user channel was established before -> startState data are linked to PRM user channel */
    /* If the startState is linked to usr2 channel, some of the provisioning action would probably have been completed before. */
    pDBusr = (MRP_USR_DB_TYPE *)pChPrmUsr->pChUsrDB->pUsr;

    *pMrpRedStateCurrent = pDBusr->pInstData[instance].RedStateIndCurrent.redStateInd;
  }
  else
    MRP_PROGRAM_TRACE_01(pChPrivUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_usr_get_current_redstate[inst=%d]: pChPrmUsr must not be NULL", instance);
}

/*===========================================================================
* FUNCTION : mrp_usr_check_redstate_change
*----------------------------------------------------------------------------
* PURPOSE  : compares last indicated redundancy state with current redundancy state 
*----------------------------------------------------------------------------
* RETURNS  : LSA_TRUE or LSA_FALSE (difference in ringState)
*----------------------------------------------------------------------------
* INPUTS   : pChPrivUsr       = channel reference
*            instance         = affected MRP instance 
*            pMrpRedStateCurrent = pointer to struct with current redundancy state variables
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_UINT8 mrp_usr_check_redstate_change (const MRP_CONST_CHANNEL_TYPE_USER pChPrivUsr, LSA_UINT16 instance, const MRP_OPC_RED_STATE_INDICATION_TYPE *pMrpRedStateCurrent)
{
  LSA_UINT8 redStateDiff, rpChanged, stdbyPortChanged, inChanged;
  MRP_OPC_RED_STATE_INDICATION_TYPE *pRedStateIndLastSignaled;
  MRP_USR_DB_TYPE *pDBusr = (MRP_USR_DB_TYPE *)pChPrivUsr->pChUsrDB->pUsr;

  pRedStateIndLastSignaled = &pDBusr->pInstData[instance].RedStateIndLastSignaled.redStateInd;
  redStateDiff = LSA_FALSE;

  rpChanged = ( !((pRedStateIndLastSignaled->rport1 == pMrpRedStateCurrent->rport1 && pRedStateIndLastSignaled->rport2 == pMrpRedStateCurrent->rport2) ||
                  (pRedStateIndLastSignaled->rport1 == pMrpRedStateCurrent->rport2 && pRedStateIndLastSignaled->rport2 == pMrpRedStateCurrent->rport1)));

  stdbyPortChanged = ( !(pRedStateIndLastSignaled->stdbyPorts[0] == pMrpRedStateCurrent->stdbyPorts[0] && pRedStateIndLastSignaled->stdbyPorts[1] == pMrpRedStateCurrent->stdbyPorts[1] &&
                         pRedStateIndLastSignaled->stdbyPorts[2] == pMrpRedStateCurrent->stdbyPorts[2] && pRedStateIndLastSignaled->stdbyPorts[3] == pMrpRedStateCurrent->stdbyPorts[3] &&
                         pRedStateIndLastSignaled->stdbyPorts[4] == pMrpRedStateCurrent->stdbyPorts[4]));

  inChanged = mrp_in_check_redstate_change (pChPrivUsr, instance, pMrpRedStateCurrent);

  if (pRedStateIndLastSignaled->initState  != pMrpRedStateCurrent->initState  ||
      pRedStateIndLastSignaled->startState != pMrpRedStateCurrent->startState ||
      inChanged                                                               ||
      rpChanged                                                               ||
      stdbyPortChanged
    )
  {
    redStateDiff = LSA_TRUE;
  }

  return redStateDiff;
}

/*===========================================================================
* FUNCTION : mrp_usr_red_state_ind_done
*----------------------------------------------------------------------------
* PURPOSE  : indicate state changes of redundancy state machines
*----------------------------------------------------------------------------
* RETURNS  : LSA_TRUE  = RQB in queue
*            LSA_FALSE = no RQB in queue
*----------------------------------------------------------------------------
* INPUTS   : pChPrivUsr = channel reference for private user channel (USER2)
*            instance   = affected MRP instance 
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT8 mrp_usr_red_state_ind_done (const MRP_CONST_CHANNEL_TYPE_USER pChPrivUsr, LSA_UINT16 instance)
{
  MRP_UPPER_RQB_PTR_TYPE pRQB;
  MRP_UPPER_ARGS_PTR_TYPE pArgs;
  LSA_UINT16 resp = MRP_RSP_OK;
  LSA_UINT8 redStateDiff;
  MRP_USR_DB_TYPE *pDBusr = (MRP_USR_DB_TYPE *)pChPrivUsr->pChUsrDB->pUsr;
  MRP_USR_INST_DATA *pDBusrInst;
  MRP_OPC_RED_STATE_INDICATION_TYPE mrpRedStateCurrent;

  pDBusrInst = &pDBusr->pInstData[instance];
        
  MRP_MEMSET(&mrpRedStateCurrent, 0, sizeof(MRP_OPC_RED_STATE_INDICATION_TYPE));

  /* get current redundancy state */
  mrp_usr_get_current_redstate(pChPrivUsr, instance, &mrpRedStateCurrent);
  redStateDiff = mrp_usr_check_redstate_change (pChPrivUsr, instance, &mrpRedStateCurrent);

  if (pChPrivUsr->state == MRP_CH_STATE_CLOSING || redStateDiff) 
  {
    MRP_RQB_POP_FROM_TOP(pDBusrInst->pRedStateIndRqbB, pDBusrInst->pRedStateIndRqbT, pRQB);
    if (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL)) 
    {
      MRP_UPPER_TRACE_02(pChPrivUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "<< MRP_OPC_RED_STATE_INDICATION done[inst=%d]: out of resource, myHandle =%d", instance, pChPrivUsr->myHandle);
      pDBusrInst->RedStateIndOutOfResource = MRP_IND_RESOURCE_MISSING_ON;

      return LSA_FALSE;
    }

    pArgs = MRP_RQB_GET_ARGS_PTR(pRQB);
    pArgs->state.state_ind.redStateInd = pDBusrInst->RedStateIndLastSignaled.redStateInd = pDBusrInst->RedStateIndCurrent.redStateInd = mrpRedStateCurrent;
    if (pChPrivUsr->state == MRP_CH_STATE_CLOSING) 
    {
      resp = MRP_RSP_OK_CANCEL;
      MRP_RQB_SET_RESPONSE(pRQB, resp);
    }
    MRP_UPPER_TRACE_03(pChPrivUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "<< MRP_OPC_RED_STATE_INDICATION done[inst=%d]: cbf called, myHandle =%d, resp=0x%x", instance, pChPrivUsr->myHandle, resp);
    mrp_usr_call_upper_cbf (pChPrivUsr, pRQB);
  }
  return LSA_TRUE;
}


/*===========================================================================
* FUNCTION : mrp_usr_red_state_ind_provide_rqb
*----------------------------------------------------------------------------
* PURPOSE  : set indication rqbs for signaling of redundancy states
*----------------------------------------------------------------------------
* RETURNS  : MRP_RET_OK/MRP_RET_ERR
*----------------------------------------------------------------------------
* INPUTS   : pChPrivUsr = channel reference to private user channel (USER2)
*            pRQB       = upper rqb reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : queues indication requests 
*==========================================================================*/
LSA_UINT16 mrp_usr_red_state_ind_provide_rqb (const MRP_CHANNEL_TYPE_USER pChPrivUsr, MRP_UPPER_RQB_PTR_TYPE pRQB)
{
  LSA_UINT16 instance;
  MRP_UPPER_ARGS_PTR_TYPE pArgs;
  MRP_USR_DB_TYPE *pDBusr = (MRP_USR_DB_TYPE *)pChPrivUsr->pChUsrDB->pUsr;

  pArgs = MRP_RQB_GET_ARGS_PTR(pRQB);

  MRP_ASSERT (pArgs->state.state_ind.redStateInd.mrpInstance > 0 && pArgs->state.state_ind.redStateInd.mrpInstance <= EDD_CFG_MAX_MRP_INSTANCE_CNT);
  instance = pArgs->state.state_ind.redStateInd.mrpInstance - 1;

  if (pChPrivUsr->state != MRP_CH_STATE_CLOSING) 
  {
    if (instance < mrp_sys_get_num_of_instances(pChPrivUsr))
    {
      MRP_UPPER_TRACE_05(pChPrivUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, ">> MRP_OPC_RED_STATE_INDICATION[inst=%d]: rqb provided, pChPrivUsr=0x%x, state=%d, myHandle=%d, outOfRes=%d, resp=MRP_RSP_OK",
        instance, pChPrivUsr, pChPrivUsr->state, pChPrivUsr->myHandle, pDBusr->pInstData[instance].RedStateIndOutOfResource);

      MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_OK);
      MRP_RQB_PUSH_TO_BOTTOM(pDBusr->pInstData[instance].pRedStateIndRqbB, pDBusr->pInstData[instance].pRedStateIndRqbT, pRQB);
      pChPrivUsr->pRQB = LSA_NULL;

      if (pDBusr->pInstData[instance].RedStateIndOutOfResource) 
      {
        pDBusr->pInstData[instance].RedStateIndOutOfResource = MRP_IND_RESOURCE_MISSING_OFF;

        (void)(mrp_usr_red_state_ind_done (pChPrivUsr, instance));
      }
      return MRP_RET_OK;
    }
    else
    {
      MRP_UPPER_TRACE_04(pChPrivUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> MRP_OPC_RED_STATE_INDICATION[inst=%d]: rqb provided, pChPrivUsr=0x%x, state=%d, myHandle=%d, resp=MRP_RSP_ERR_PRM_DATA",
        instance, pChPrivUsr, pChPrivUsr->state, pChPrivUsr->myHandle);

      MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_PRM_DATA);

      return MRP_RET_ERR;
    }
  }
  else
  {
    MRP_UPPER_TRACE_04(pChPrivUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> MRP_OPC_RED_STATE_INDICATION[inst=%d]: rqb provided, pChPrivUsr=0x%x, state=%d, myHandle=%d, resp=MRP_RSP_ERR_SEQUENCE",
      instance, pChPrivUsr, pChPrivUsr->state, pChPrivUsr->myHandle);

    MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_SEQUENCE);

    return MRP_RET_ERR;
  }
}

#ifdef MRP_CFG_PLUGIN_2_HSR
/*===========================================================================
* FUNCTION : mrp_usr_ind_hsr_provide_done
*----------------------------------------------------------------------------
* PURPOSE  : indicate HSR state changes
*----------------------------------------------------------------------------
* RETURNS  : LSA_TRUE  = RQB in queue
*            LSA_FALSE = no RQB in queue
*----------------------------------------------------------------------------
* INPUTS   : pChPrivUsr = channel reference to private user channel (USER2)
*            instance   = affected HSR instance
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT8 mrp_usr_ind_hsr_provide_done (const MRP_CONST_CHANNEL_TYPE_USER pChPrivUsr, LSA_UINT16 instance)
{
  MRP_UPPER_RQB_PTR_TYPE pRQB;
  MRP_UPPER_ARGS_PTR_TYPE pArgs;
  MRP_STBY_PARAM_T StdbyState;

  LSA_UINT16 resp = MRP_RSP_OK;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE*)pChPrivUsr->pChSysDB->pEdd;
  MRP_USR_DB_TYPE *pDBusr = (MRP_USR_DB_TYPE *)pChPrivUsr->pChUsrDB->pUsr;
  
  MRP_RQB_POP_FROM_TOP(pDBusr->pInstData[instance].pHsrIndProvideRqbB, pDBusr->pInstData[instance].pHsrIndProvideRqbT, pRQB);
  if (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL)) 
  {
    MRP_UPPER_TRACE_03(pChPrivUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "<< MRP_OPC_IND_PROVIDE done[inst=%d]: out of resource, pChPrivUsr=0x%x, myHandle=%d", instance, pChPrivUsr, pChPrivUsr->myHandle);
    
    return LSA_FALSE;
  }
  else
  {
    pArgs = MRP_RQB_GET_ARGS_PTR(pRQB);

    (void)(mrp_stby_get_state (pDBedd->pChSys, &pArgs->state.ind.stdbyState, &pArgs->state.ind.stdbyDiag, LSA_FALSE));
    MRP_MEMSET (&StdbyState, 0, sizeof (StdbyState));
    mrp_stby_get_params (pDBedd->pChSys, &StdbyState);
    pArgs->state.ind.stdbyPartnerPort=StdbyState.PartnerPortID;
    pArgs->state.ind.stdbyChangeOver=StdbyState.ChangeOver;
    MRP_MEMCPY((LSA_VOID_PTR_TYPE)(pArgs->state.ind.stdbyPartnerID), (LSA_VOID_PTR_TYPE)(StdbyState.PartnerID), 6);
    (void)(mrp_mgr_get_state_ext (pDBedd->pChSys, &pArgs->state.ind.mgrState, &pArgs->state.ind.mgrDiag, &pArgs->state.ind.ssrAddInfo, pArgs->state.ind.mgrDiagMAC, LSA_FALSE));

    if (pChPrivUsr->state == MRP_CH_STATE_CLOSING) 
    {
      resp = MRP_RSP_OK_CANCEL;
      MRP_RQB_SET_RESPONSE(pRQB, resp);
    }
    MRP_UPPER_TRACE_05(pChPrivUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "<< MRP_OPC_IND_PROVIDE[inst=%d] done: cbf called, pChPrivUsr=0x%x, state=%d, myHandle=%d, resp=0x%x",
      instance, pChPrivUsr, pChPrivUsr->state, pChPrivUsr->myHandle, resp);

    mrp_usr_call_upper_cbf (pChPrivUsr, pRQB);
  }

  return LSA_TRUE;
}


/*===========================================================================
* FUNCTION : mrp_usr_ind_hsr_provide
*----------------------------------------------------------------------------
* PURPOSE  : set indication provide for signaling 
*----------------------------------------------------------------------------
* RETURNS  : MRP_RET_OK
*----------------------------------------------------------------------------
* INPUTS   : pChPrivUsr = channel reference
*            pRQB       = upper rqb reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : queues indication requests 
*==========================================================================*/
LSA_UINT16 mrp_usr_ind_hsr_provide (const MRP_CHANNEL_TYPE_USER pChPrivUsr, MRP_UPPER_RQB_PTR_TYPE pRQB)
{
  LSA_UINT16 instance;
  LSA_UINT16 retval;
  MRP_UPPER_ARGS_PTR_TYPE pArgs;
  MRP_USR_DB_TYPE *pDBusr = (MRP_USR_DB_TYPE *)pChPrivUsr->pChUsrDB->pUsr;

  pArgs = MRP_RQB_GET_ARGS_PTR(pRQB);
  MRP_ASSERT (pArgs->state.ind.hsrInstance == 1);
  instance = pArgs->state.ind.hsrInstance - 1;
  
  if (pChPrivUsr->state != MRP_CH_STATE_CLOSING) 
  {
    if (instance < mrp_sys_get_num_of_instances(pChPrivUsr))
    {
      MRP_UPPER_TRACE_04(pChPrivUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, ">> MRP_OPC_IND_PROVIDE[inst=%d]: rqb provided, pChPrivUsr=0x%x, state=%d, myHandle=%d, resp=MRP_RSP_OK",
        instance, pChPrivUsr, pChPrivUsr->state, pChPrivUsr->myHandle);

      MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_OK);
      MRP_RQB_PUSH_TO_BOTTOM(pDBusr->pInstData[instance].pHsrIndProvideRqbB, pDBusr->pInstData[instance].pHsrIndProvideRqbT, pRQB);
      pChPrivUsr->pRQB = LSA_NULL;

      retval = MRP_RET_OK;
    }
    else
    {
      MRP_UPPER_TRACE_04(pChPrivUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> MRP_OPC_IND_PROVIDE[inst=%d]: rqb provided, pChPrivUsr=0x%x, state=%d, myHandle=%d, resp=MRP_RSP_ERR_PRM_DATA",
        instance, pChPrivUsr, pChPrivUsr->state, pChPrivUsr->myHandle);

      MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_PRM_DATA);

      retval = MRP_RET_ERR;
    }
  }
  else
  {
    MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_SEQUENCE);

    MRP_UPPER_TRACE_04(pChPrivUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> MRP_OPC_IND_PROVIDE[inst=%d]: rqb provided, pChPrivUsr=0x%x, state=%d, myHandle=%d, resp=MRP_RSP_ERR_SEQUENCE",
      instance, pChPrivUsr, pChPrivUsr->state, pChPrivUsr->myHandle);
    retval = MRP_RET_ERR;
  }
  return retval;
}
#endif /* MRP_CFG_PLUGIN_2_HSR */


/*===========================================================================
* FUNCTION : mrp_usr_release_channel_data
*----------------------------------------------------------------------------
* PURPOSE  : release channel data struct
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pCh = channel reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : used to free channel info for user and system channel. Called 
*            when closing edd, usr or usr2 channel.
*==========================================================================*/
LSA_VOID mrp_usr_release_channel_data (const MRP_CHANNEL_TYPE_COMMON pCh)
{

  switch (pCh->detailptr->PathType) 
  {
  case MRP_PATH_TYPE_EDD:
    mrp_sys_ch_db_deinit (pCh);
    break;

  case MRP_PATH_TYPE_USER:
  case MRP_PATH_TYPE_USER_2:
    mrp_usr_ch_db_deinit (pCh);
    break;

  default:
    /* unknown pCh->detailptr->PathType */
    MRP_ASSERT(LSA_FALSE);
    break;
  }

  pCh->state = MRP_CH_STATE_FREE;
}


/*===========================================================================
* FUNCTION : mrp_usr_open_channel
*----------------------------------------------------------------------------
* PURPOSE  : internal processing of usr channel open 
*----------------------------------------------------------------------------
* RETURNS  : resp = result of memory allocation for USR DB
*----------------------------------------------------------------------------
* INPUTS   : pChUsr       = user channel reference
*            pChPrmUser   = first user channel, which is already open (CM-PD channel)
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT16 mrp_usr_open_channel (const MRP_CHANNEL_TYPE_USER pChUsr, const MRP_CONST_CHANNEL_TYPE_USER pChPrmUsr)
{
  LSA_UINT16 resp;
  MRP_UPPER_ARGS_PTR_TYPE pArgs;

  pArgs = MRP_RQB_GET_ARGS_PTR(pChUsr->pRQB);
  resp = mrp_usr_ch_db_init(pChUsr);

  if (resp == MRP_RSP_OK)
  {
    mrp_usr_init (pChUsr, pChPrmUsr);

    pChUsr->state = MRP_CH_STATE_READY;

    pArgs->channel.open.handle = pChUsr->myHandle;
    MRP_RQB_SET_RESPONSE(pChUsr->pRQB, resp);

    mrp_usr_call_upper_cbf (pChUsr, pChUsr->pRQB);
    pChUsr->pRQB = LSA_NULL;

    MRP_UPPER_TRACE_01(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "<< mrp_open_channel(USR or USR2) successful, pathType=%d", pChUsr->detailptr->PathType);
  }

  return resp;
}

/*===========================================================================
* FUNCTION : mrp_open_channel
*----------------------------------------------------------------------------
* PURPOSE  : create a MRP entity, open or reuse an communication-channel 
*            to the lower layer
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pRQB = upper rqb reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_open_channel (MRP_UPPER_RQB_PTR_TYPE pRQB)
{
  LSA_UINT16 pathState, openRsp, freeEntry;
  MRP_HDB_TYPE chTmp;
  MRP_CHANNEL_TYPE_COMMON pChtmp, pCh;
  MRP_CHANNEL_TYPE_SYSTEM pChSys;
  MRP_CHANNEL_TYPE_USER pChPrmUsr;
  MRP_UPPER_ARGS_PTR_TYPE pArgs;
  LSA_UINT8 pathOpen;
  LSA_SYS_PTR_TYPE tmpSysPtr;
  MRP_DETAIL_PTR_TYPE tmpDetailPtr = LSA_NULL;

  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL));

  pChtmp = &chTmp;
  /* init tmp channel struct */
  MRP_MEMSET(pChtmp, 0, sizeof(MRP_HDB_TYPE));

  pArgs = MRP_RQB_GET_ARGS_PTR(pRQB);

  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pArgs, LSA_NULL));
  MRP_FATAL1(MRP_RQB_GET_OPCODE(pRQB) == MRP_OPC_OPEN_CHANNEL);
  MRP_FATAL1(!(0 == pArgs->channel.open.pCbf));

  MRP_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_NOTE, ">> mrp_open_channel, handle_upper=%d", pArgs->channel.open.handle_upper);

  pArgs->channel.open.handle = 0;
  pathOpen = LSA_FALSE;
  /* set bad result */
  openRsp = MRP_RSP_ERR;
  /* set handle_upper as RQB handle*/
  MRP_RQB_SET_HANDLE (pRQB, pArgs->channel.open.handle_upper);

  pChtmp->state            = MRP_CH_STATE_RESERVED; /* later     */
  pChtmp->handle_upper     = pArgs->channel.open.handle_upper;  
  pChtmp->handle_lower_edd = 0;                     /* later     */             
  pChtmp->pCbf             = pArgs->channel.open.pCbf;
  pChtmp->detailptr        = 0;                     /* later     */
  pChtmp->sys_path         = pArgs->channel.open.sys_path;
  pChtmp->pRQB             = pRQB;
  pChtmp->pChSysDB         = LSA_NULL;
  pChtmp->pChUsrDB         = LSA_NULL;

  /* get the path info of the new channel */
  MRP_GET_PATH_INFO(&pathState, &tmpSysPtr, &tmpDetailPtr, pArgs->channel.open.sys_path);

  if (pathState != LSA_RET_OK || LSA_HOST_PTR_ARE_EQUAL(tmpDetailPtr, LSA_NULL)) 
  {
    MRP_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_FATAL, ">> MRP_GET_PATH_INFO failed (no sysptr/detailptr) rsp=0x%x", pathState);
    MRP_FATAL1(pathState != LSA_RET_OK || LSA_HOST_PTR_ARE_EQUAL(tmpDetailPtr, LSA_NULL)); 
    return;
  }
  else
    pathOpen = LSA_TRUE;

  pChSys = pChPrmUsr = pCh = LSA_NULL;

  pChtmp->detailptr = tmpDetailPtr;
  pChtmp->sysptr    = tmpSysPtr;

  /* check, if there are still new channels allowed for the given path_type*/
  switch (tmpDetailPtr->PathType) 
  {
  case MRP_PATH_TYPE_EDD:
    /* search, if there is already one system channel for this nic_id */
    openRsp = mrp_sys_check_edd_details (tmpDetailPtr, &freeEntry);
    if (openRsp == MRP_RSP_OK)
    {
      pCh = g_Mrp.pChSys[freeEntry];
      pChtmp->myHandle = pCh->myHandle;
      *pCh = *pChtmp;
      openRsp = mrp_edd_open_channel (pCh);
    }
    break;

  case MRP_PATH_TYPE_USER:
    openRsp = mrp_sys_check_edd_details (tmpDetailPtr, &freeEntry);
    if (openRsp == MRP_RSP_OK)
    {
      pChSys = g_Mrp.pChSys[freeEntry];
      openRsp = mrp_usr_check_new_usr_details (tmpDetailPtr, pChSys->pChSysDB->traceIdx, &freeEntry);

      if (openRsp == MRP_RSP_OK)
      {
        pChtmp->pChSysDB         = pChSys->pChSysDB;
        pChtmp->handle_lower_edd = pChSys->handle_lower_edd;
        
        if (tmpDetailPtr->type.user.isPrmInterface == MRP_IS_PRM_INTERFACE_ON)
        {
          pCh = g_Mrp.pChPrmUsr[freeEntry];
          pChtmp->myHandle = pCh->myHandle;
          *pCh = *pChtmp;
        }
        else
        {
          pCh = g_Mrp.pChUsr[freeEntry];
          pChtmp->myHandle = pCh->myHandle;
          *pCh = *pChtmp;
          /* get the corresponding prm user channel */
          pChPrmUsr = mrp_usr_get_prm_usr_channel (tmpDetailPtr);
        }
        openRsp = mrp_usr_open_channel (pCh, pChPrmUsr);
      }
    }
    break;

  case MRP_PATH_TYPE_USER_2:
    openRsp = mrp_sys_check_edd_details (tmpDetailPtr, &freeEntry);
    if (openRsp == MRP_RSP_OK)
    {
      pChSys = g_Mrp.pChSys[freeEntry];
      openRsp = mrp_usr_check_new_priv_usr_details (tmpDetailPtr, pChSys->pChSysDB->traceIdx, &freeEntry);

      if (openRsp == MRP_RSP_OK)
      {
        pChtmp->pChSysDB         = pChSys->pChSysDB;
        pChtmp->handle_lower_edd = pChSys->handle_lower_edd;

        pCh = g_Mrp.pChPrivUsr[freeEntry];
        pChtmp->myHandle = pCh->myHandle;
        *pCh = *pChtmp;

        pChPrmUsr = mrp_usr_get_prm_usr_channel (tmpDetailPtr);

        openRsp = mrp_usr_open_channel (pCh, pChPrmUsr);
      }
    }
    break;

  default:
    MRP_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_ERROR, ">> unknown path type=%d", tmpDetailPtr->PathType);
    openRsp = MRP_RSP_ERR_SYS_PATH;
    break;
  }

  if (openRsp != MRP_RSP_OK)
  {
    if (!LSA_HOST_PTR_ARE_EQUAL (pCh, LSA_NULL))
    {
      pCh->state = MRP_CH_STATE_FREE;
    }
    MRP_RQB_SET_RESPONSE(pRQB, openRsp);

    /* mrp_open_channel failed, call CBF of open_channel RQB, because pCh->pRQB not initialized */
    MRP_REQUEST_UPPER_DONE (pArgs->channel.open.pCbf, pRQB, tmpSysPtr);

    if (pathOpen)   /*lint !e774*/
    {
      MRP_RELEASE_PATH_INFO(&pathState, tmpSysPtr, tmpDetailPtr);    
      MRP_FATAL1(pathState == LSA_RET_OK);
    }
  }
}

/*===========================================================================
* FUNCTION : mrp_usr_finish_close_channel
*----------------------------------------------------------------------------
* PURPOSE  : close user channel
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr = user channel reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : freeing and uninitializing component resources for user channels.
*==========================================================================*/
LSA_VOID mrp_usr_finish_close_channel (const MRP_CHANNEL_TYPE_USER pChUsr)
{
  LSA_UINT16 portID;
  LSA_UINT8 isPrmUser;
  LSA_UINT16 pathType;
  LSA_UINT16 i, k, rc, numOfPorts;
  LSA_UINT16 mrpMaxInstance, mrpInMaxInstance;
  LSA_UINT16  pInstance[2];

  pathType = pChUsr->detailptr->PathType;
  isPrmUser = LSA_FALSE;

  mrpMaxInstance = mrp_sys_get_num_of_instances (pChUsr); 

  switch (pathType)
  {
  case MRP_PATH_TYPE_USER_2:
    mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pChUsr);
    for (i = 0; i < mrpMaxInstance; i++)
    {
      pInstance[MRP_INSTANCE_INDEX]     = i;

      while (mrp_usr_state_ind_done (pChUsr, i))
      {
    
      }

      mrp_usr_red_state_deinit (pChUsr, i);
      while (mrp_usr_red_state_ind_done (pChUsr, i))
      {

      }
      for (k = 0; k < mrpInMaxInstance; k++)
      {
        pInstance[MRP_IN_INSTANCE_INDEX]  = k;

        while (mrp_in_red_event_ind_done (pChUsr, pInstance))
        {

        }
      }
    }
#ifdef MRP_CFG_PLUGIN_2_HSR
    /* give HSR indication back */
    for (i = 0; i < 1; i++)
    {
      while (mrp_usr_ind_hsr_provide_done (pChUsr, i))
      {

      }    
    }
#endif
    break;

  case MRP_PATH_TYPE_USER:
    /* give back PRM indications*/
    if (pChUsr->detailptr->type.user.isPrmInterface == MRP_IS_PRM_INTERFACE_ON)
    {
      isPrmUser = LSA_TRUE;
      while (mrp_rec_prm_ind_done (pChUsr))
      {
      }
      numOfPorts = mrp_sys_get_num_of_ports (pChUsr);

      for (portID = 1; portID < numOfPorts + 1; portID++) 
      {
        while (mrp_rec_prm_ind_port_done (pChUsr, portID))
        {
        }
      }
    }
    for (i = 0; i < mrpMaxInstance; i++)
    {
    /* give back ringstate indications */
      while (mrp_oha_ringstate_ind_done (pChUsr, i))
      {
      }    
    }
    break;

  default:
    MRP_FATAL;
    break;
  }

  mrp_usr_deinit (pChUsr);
  mrp_usr_release_channel_data (pChUsr);

  MRP_RQB_SET_RESPONSE(pChUsr->pRQB, MRP_RSP_OK);

  mrp_usr_call_upper_cbf (pChUsr, pChUsr->pRQB);
  pChUsr->pRQB = LSA_NULL;

  MRP_RELEASE_PATH_INFO(&rc, pChUsr->sysptr, pChUsr->detailptr);
  MRP_FATAL1(rc == LSA_RET_OK);

  MRP_UPPER_TRACE_02(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "<< mrp_close_channel(USRx) successful, pathType=%d, isPrmUser= %d", pathType, isPrmUser);
  LSA_UNUSED_ARG(isPrmUser);   /* hhr: to please the compiler in case traces are disabled. */

}

/*===========================================================================
* FUNCTION : mrp_usr_close_channel
*----------------------------------------------------------------------------
* PURPOSE  : internal processing of usr close channel 
*----------------------------------------------------------------------------
* RETURNS  : resp = result of memory allocation for USR DB
*----------------------------------------------------------------------------
* INPUTS   : pChUsr       = user channel reference
*            ch_usr_first = first user channel, which is already open (CM-PD channel)
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT16 mrp_usr_close_channel (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB)
{
  LSA_UINT16 resp;
  MRP_USR_DB_TYPE *pDBusr = (MRP_USR_DB_TYPE *)pChUsr->pChUsrDB->pUsr;

  MRP_ASSERT(!LSA_HOST_PTR_ARE_EQUAL(pDBusr, LSA_NULL));
  if (!LSA_HOST_PTR_ARE_EQUAL(pDBusr->pUsrPendingRqbT, LSA_NULL)) 
  {
    MRP_UPPER_TRACE_00(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> mrp_close_channel: usr request pending");

    resp = MRP_RSP_ERR_SEQUENCE;
  }
  else
  {
    /* close user channel */
    pChUsr->state = MRP_CH_STATE_CLOSING;

    resp = MRP_RSP_OK;

    if (pChUsr->detailptr->PathType == MRP_PATH_TYPE_USER_2) 
    {
      mrp_usr_finish_close_channel (pChUsr);
    }
    else 
    {
      if (pChUsr->detailptr->type.user.isPrmInterface == MRP_IS_PRM_INTERFACE_ON)
        mrp_rec_prm_commit (pChUsr, pRQB);
      else
        mrp_usr_finish_close_channel (pChUsr);
    }
  }

  return resp;
}

/*===========================================================================
* FUNCTION : mrp_close_channel
*----------------------------------------------------------------------------
* PURPOSE  : close a MRP entity or a communication-channel 
*            to the lower layer
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pRQB = upper rqb reference 
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_close_channel (MRP_UPPER_RQB_PTR_TYPE pRQB)
{
  LSA_UINT16 i;
  MRP_CHANNEL_TYPE_COMMON pCh;
  MRP_UPPER_ARGS_PTR_TYPE pArgs;
  LSA_UINT16 closeRsp;
  LSA_UINT32 traceIdx;

  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL));

  closeRsp = MRP_RSP_OK;
  pArgs = MRP_RQB_GET_ARGS_PTR(pRQB);

  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pArgs, LSA_NULL));
  MRP_FATAL1(MRP_RQB_GET_OPCODE(pRQB) == MRP_OPC_CLOSE_CHANNEL);

  pCh = mrp_sys_get_pCh (MRP_RQB_GET_HANDLE(pRQB));
  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pCh, LSA_NULL));
  traceIdx = pCh->pChSysDB->traceIdx;   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */
  LSA_UNUSED_ARG(traceIdx);   /* hhr: to please the compiler in case traces are disabled. */

  MRP_UPPER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, ">> mrp_close_channel: handle=%d", MRP_RQB_GET_HANDLE(pRQB));

  /* request pending and close channel requested ? */
  if (!LSA_HOST_PTR_ARE_EQUAL(pCh->pRQB, LSA_NULL)) 
  {
    MRP_UPPER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> mrp_close_channel: request already pending");
    closeRsp = MRP_RSP_ERR_SEQUENCE;
  }
  else
  {
#ifdef MRP_CFG_PLUGIN_2_HSR
    /* check if HSR/STDBY functions are OFF because HSR will not be deactivated by mrp_close_channel!!! */
    {
      LSA_UINT16 portA;
      LSA_UINT16 portB; 
      MRP_MODE_T mode;
      MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pCh->pChSysDB->pEdd;

      mrp_bs_get_ring_ports_and_mode (pDBedd->pChSys, &portA, &portB, &mode);
      if (mode != MRP_MODE_T_INIT && mode != MRP_MODE_T_MRP) 
      {
        MRP_UPPER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> HSR still running (must be deactivated before mrp_close_channel)");
        closeRsp = MRP_RSP_ERR_SEQUENCE;
      }
      else
      {
        if (mrp_bs_get_stdby_info (pDBedd->pChSys, LSA_NULL)) 
        {
          MRP_UPPER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> STDBY still running (must be deactivated before mrp_close_channel)");
          closeRsp = MRP_RSP_ERR_SEQUENCE;
        }
      }
    }
#endif

    if (closeRsp == MRP_RSP_OK)   /*lint !e774*/
    {
      switch (pCh->detailptr->PathType) 
      {
      case MRP_PATH_TYPE_EDD:
        i = mrp_usr_get_usr_ch_cnt(pCh->detailptr->NicId);

        if (i != 0)
        {
          MRP_UPPER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> mrp_close_channel: user channel pending");
          closeRsp = MRP_RSP_ERR_SEQUENCE;
        }
        if (closeRsp == MRP_RSP_OK)
        {
          pCh->pRQB = pRQB;
          mrp_edd_close_channel (pCh);
        }
        break;

      case MRP_PATH_TYPE_USER:
      case MRP_PATH_TYPE_USER_2:
        pCh->pRQB = pRQB;
        closeRsp = mrp_usr_close_channel (pCh, pRQB);
        break;

      default:
        MRP_UPPER_TRACE_00(0, LSA_TRACE_LEVEL_ERROR, ">> mrp_close_channel: unknown path type");
        closeRsp = MRP_RSP_ERR_PARAM;
        break; 
      }
    }
  }

  if (closeRsp != MRP_RSP_OK)
  {
    MRP_RQB_SET_RESPONSE(pRQB, closeRsp);

    /* close channel finished with error */
    mrp_usr_call_upper_cbf (pCh, pRQB);

    if (pCh->pRQB == pRQB)
      pCh->pRQB = LSA_NULL;
    /* NOTE:  pCh->pRQB = LSA_NULL; Only reset pCh->pRQB, if it was set before!!! */
  }
}


/*===========================================================================
* FUNCTION : mrp_usr_request_internal
*----------------------------------------------------------------------------
* PURPOSE  : handle queued requests
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr = user channel reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_usr_request_internal (const MRP_CHANNEL_TYPE_USER pChUsr)
{
  LSA_UINT16 retVal;
  LSA_UINT16 i;
  MRP_UPPER_RQB_PTR_TYPE  pRQB;
  MRP_USR_DB_TYPE *pDBusr = (MRP_USR_DB_TYPE *)pChUsr->pChUsrDB->pUsr;
  
  while (LSA_HOST_PTR_ARE_EQUAL(pChUsr->pRQB, LSA_NULL)) 
  {
    MRP_RQB_POP_FROM_TOP(pDBusr->pUsrPendingRqbB, pDBusr->pUsrPendingRqbT, pRQB);
    if (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL)) 
    {
      /* end of requests */
      return;
    }
    pChUsr->pRQB = pRQB;
    retVal = MRP_RET_OK;

    switch (pChUsr->detailptr->PathType)
    {
    case MRP_PATH_TYPE_USER_2:
    case MRP_PATH_TYPE_USER:
      for (i = 0; i < g_Mrp.usrReqCnt; i++) 
      {
        if (MRP_RQB_GET_OPCODE(pRQB) == g_Mrp.pUsrReqMap[i].opcode) 
        {
          if ( (pChUsr->detailptr->PathType != g_Mrp.pUsrReqMap[i].pathType) &&
              !(pChUsr->detailptr->PathType == MRP_PATH_TYPE_USER_2 && MRP_RQB_GET_OPCODE(pRQB) == MRP_OPC_PRM_READ)  
             ) 
          {
            MRP_UPPER_TRACE_02(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> mrp_usr_request_internal: not allowed for this pathtype: needed=0x%x req=0x%x", 
                               g_Mrp.pUsrReqMap[i].pathType, pChUsr->detailptr->PathType);

            MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_SYS_PATH);
            retVal = MRP_RET_ERR;
          }
          else 
          {
            retVal = g_Mrp.pUsrReqMap[i].pUserReqCbf(pChUsr, pRQB);
          }
          break;
        }
      }
      /* unknown opcode */
      if (i >= g_Mrp.usrReqCnt)
      {
        MRP_UPPER_TRACE_01(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_FATAL, ">> mrp_usr_request_internal: unknown opcode=0x%x", MRP_RQB_GET_OPCODE(pRQB));
        
        MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_SYS_PATH);
        retVal = MRP_RET_ERR;
      }
      break;

    default:
      /* only requests on user channels allowed */
      MRP_UPPER_TRACE_01(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> mrp_usr_request_internal: no req allowed for pathtype=%d",
                         pChUsr->detailptr->PathType);

      MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_PARAM);
      retVal = MRP_RET_ERR;
      break;
    }

    /* in case of errors, cancel request and call cbf*/
    if (retVal == MRP_RET_ERR && MRP_RQB_GET_RESPONSE(pRQB) != MRP_RSP_OK) 
    {
      if (MRP_RQB_GET_OPCODE(pRQB) != MRP_OPC_PRM_READ_ADMIN_DATA) 
      {
        mrp_usr_call_upper_cbf_proc_next_req (pChUsr, pRQB);
      }
      return;
    }
  }
}

/*===========================================================================
* FUNCTION : mrp_request
*----------------------------------------------------------------------------
* PURPOSE  : handle mrp requests
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pRQB = upper rqb reference 
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_request(MRP_UPPER_RQB_PTR_TYPE pRQB)
{
  MRP_CHANNEL_TYPE_USER pCh;
  MRP_UPPER_ARGS_PTR_TYPE pArgs;
  MRP_USR_DB_TYPE *pDBusr;

  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL));
  pArgs = MRP_RQB_GET_ARGS_PTR(pRQB);
  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pArgs, LSA_NULL));

  pCh = mrp_sys_get_pCh(MRP_RQB_GET_HANDLE(pRQB));
  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pCh, LSA_NULL));

  /* timer requests need special handling  */
  if (MRP_RQB_GET_OPCODE(pRQB) == MRP_OPC_INTERNAL_TIMER) 
  {
    mrp_sys_timer_request((MRP_CONST_CHANNEL_TYPE_COMMON)pCh, pRQB);

    return;
  }

  pDBusr = (MRP_USR_DB_TYPE *)pCh->pChUsrDB->pUsr;

  MRP_UPPER_TRACE_01(pCh->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, ">> mrp_request opcode=%u", MRP_RQB_GET_OPCODE(pRQB));
  MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR);
  MRP_RQB_PUSH_TO_BOTTOM(pDBusr->pUsrPendingRqbB, pDBusr->pUsrPendingRqbT, pRQB);

  mrp_usr_request_internal(pCh);
}

/*===========================================================================
* FUNCTION : mrp_request_synchron
*----------------------------------------------------------------------------
* PURPOSE  : handle synchronous mrp requests
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pRQB = upper rqb reference 
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_request_synchron(MRP_UPPER_RQB_PTR_TYPE  pRQB)
{
  MRP_CHANNEL_TYPE_USER pChUsr;
  MRP_UPPER_ARGS_PTR_TYPE pArgs;

  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL));
  pArgs = MRP_RQB_GET_ARGS_PTR(pRQB);
  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pArgs, LSA_NULL));

  pChUsr = mrp_sys_get_pCh(MRP_RQB_GET_HANDLE(pRQB));
  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pChUsr, LSA_NULL));

  MRP_UPPER_TRACE_01(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, ">> mrp_request_synchron opcode=%u", MRP_RQB_GET_OPCODE(pRQB));
  MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR);

  if (pChUsr->detailptr->PathType == MRP_PATH_TYPE_EDD) 
  {
    /* only requests on user channels allowed */
    MRP_UPPER_TRACE_00(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> mrp_request_synchron: no synchronous req allowed for this pathtype");
    MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_SYS_PATH);
  }
  else 
  {
    switch (MRP_RQB_GET_OPCODE(pRQB)) 
    {
    case MRP_OPC_PRM_READ_ADMIN_DATA:
      (void)(mrp_rec_prm_read(pChUsr, pRQB));
      break;

    default:
      MRP_UPPER_TRACE_00(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_FATAL, ">> mrp_request_synchron unknown opcode");
      break;
    }
  }
}


/*===========================================================================
* FUNCTION : mrp_usr_db_init
*----------------------------------------------------------------------------
* PURPOSE  : initialize user database
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr = user channel reference
*            mrpMaxInstance = maximum number of MRP instances to be initialized
*            mrpInMaxInstance = maximum number of MRP interconnection instances
*                               to be initialized
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_usr_db_init (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance)
{
  LSA_UINT16 i, k;
  MRP_USR_DB_TYPE *pDBusr = LSA_NULL;

  MRP_PROGRAM_TRACE_04(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_usr_db_init entered: pChusr=0x%x, myHandle=%d, mrpMaxInst/mrpInMaxInst=%d/%d",
                       pChUsr, pChUsr->myHandle, mrpMaxInstance, mrpInMaxInstance); 
  
  MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBusr), sizeof(MRP_USR_DB_TYPE));
  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBusr, LSA_NULL));
  pChUsr->pChUsrDB->pUsr = pDBusr;

  /* preset zero */
  MRP_MEMSET(pDBusr, 0, sizeof(MRP_USR_DB_TYPE));

  if (mrpMaxInstance)
  {
    MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBusr->pInstData), (LSA_UINT16)(sizeof(MRP_USR_INST_DATA) * mrpMaxInstance));
    MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBusr->pInstData, LSA_NULL));
  }

  for (i = 0; i < mrpMaxInstance; i++)
  {
    MRP_MEMSET(&(pDBusr->pInstData[i]), 0, sizeof(MRP_USR_INST_DATA));
    
    pDBusr->pInstData[i].StateIndCurrent.usrStateInd.mrpInstance = i + 1;
    pDBusr->pInstData[i].StateIndLastSignaled.usrStateInd.mrpInstance = i + 1;

    mrp_usr_red_state_init (pChUsr, i);

    pDBusr->pInstData[i].StateIndOutOfResource = MRP_IND_RESOURCE_MISSING_ON;
    pDBusr->pInstData[i].RedStateIndOutOfResource = MRP_IND_RESOURCE_MISSING_ON;

    if (mrpInMaxInstance)
    {
      /* MRP interconnection may only be active if only one MRP instance is available */
      MRP_ALLOC_LOCAL_MEM ((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBusr->pInstData[i].pInInstEventData), (LSA_UINT16)(sizeof (MRP_IN_INST_EVENT_DATA) * mrpInMaxInstance));
      MRP_FATAL1 (!LSA_HOST_PTR_ARE_EQUAL (pDBusr->pInstData[i].pInInstEventData, LSA_NULL));

      for (k = 0; k < mrpInMaxInstance; k++)
      {
        MRP_MEMSET (&(pDBusr->pInstData[i].pInInstEventData[k]), 0, sizeof (MRP_IN_INST_EVENT_DATA));

        pDBusr->pInstData[i].pInInstEventData[k].InEventIndCurrent.inStateInd.mrpInstance = i + 1;
        pDBusr->pInstData[i].pInInstEventData[k].InEventIndCurrent.inStateInd.mrpInInstance = k + 1;
        pDBusr->pInstData[i].pInInstEventData[k].InEventIndCurrent.inStateInd.inMICPosition = MRP_IN_RED_EVT_PARA_DONT_CARE;
        pDBusr->pInstData[i].pInInstEventData[k].InEventIndCurrent.inStateInd.inRxPort = MRP_IN_RED_EVT_PARA_DONT_CARE;
        pDBusr->pInstData[i].pInInstEventData[k].InEventIndCurrent.inStateInd.inRxInID = MRP_IN_RED_EVT_PARA_DONT_CARE;
        pDBusr->pInstData[i].pInInstEventData[k].InEventIndCurrent.inStateInd.inPeerOperRole = MRP_IN_RED_EVT_PARA_DONT_CARE;
        pDBusr->pInstData[i].pInInstEventData[k].InEventIndCurrent.inStateInd.inPeerMICPosition = MRP_IN_RED_EVT_PARA_DONT_CARE;

        pDBusr->pInstData[i].pInInstEventData[k].InEventIndLastSignaled.inStateInd.mrpInstance = i + 1;
        pDBusr->pInstData[i].pInInstEventData[k].InEventIndLastSignaled.inStateInd.mrpInInstance = k + 1;

        pDBusr->pInstData[i].pInInstEventData[k].InEventIndOutOfResource = MRP_IND_RESOURCE_MISSING_ON;      
      }
    }
  }
}


/*===========================================================================
* FUNCTION : mrp_usr_db_deinit
*----------------------------------------------------------------------------
* PURPOSE  : uninitialize user database
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr = user channel reference
*            mrpMaxInstance = maximum number of MRP instances to be initialized
*            mrpInMaxInstance = maximum number of MRP interconnection instances
*                               to be initialized
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_usr_db_deinit (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance)
{
  LSA_UINT16 rc, i;
  MRP_USR_DB_TYPE *pDBusr = (MRP_USR_DB_TYPE *)pChUsr->pChUsrDB->pUsr;

  MRP_PROGRAM_TRACE_04(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_usr_db_deinit pChusr=0x%x, myHandle=%d entered, mrpMaxInst/mrpInMaxInst=%d/%d",
                       pChUsr, pChUsr->myHandle, mrpMaxInstance, mrpInMaxInstance); 
  
  if (mrpMaxInstance && mrpInMaxInstance)
  {
    for (i = 0; i < mrpMaxInstance; i++)
    {
      MRP_FREE_LOCAL_MEM (&rc, (MRP_LOCAL_MEM_PTR_TYPE)(pDBusr->pInstData[i].pInInstEventData));
      MRP_FATAL1 (rc == LSA_RET_OK);
    }
  }
  
  if (mrpMaxInstance)
  {
    MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBusr->pInstData);
    MRP_FATAL1(rc == LSA_RET_OK);
  }

  MRP_FREE_LOCAL_MEM(&rc,(MRP_LOCAL_MEM_PTR_TYPE)pDBusr);
  MRP_FATAL1(rc == LSA_RET_OK);
  pChUsr->pChUsrDB->pUsr = LSA_NULL;

}

/*===========================================================================
* FUNCTION : mrp_usr_ch_db_init
*----------------------------------------------------------------------------
* PURPOSE  : initialize user channel database
*----------------------------------------------------------------------------
* RETURNS  : status
*----------------------------------------------------------------------------
* INPUTS   : pChUsr = user channel reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_UINT16 mrp_usr_ch_db_init (const MRP_CHANNEL_TYPE_USER pChUsr)
{
  LSA_UINT16 resp;
  MRP_CH_USR_DB_TYPE *pDB_chUsr = LSA_NULL;
  
  MRP_UPPER_TRACE_02(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_usr_ch_db_init entered: pChUsr=0x%x, myHandle=%d", pChUsr, pChUsr->myHandle);
  
  MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDB_chUsr), sizeof(MRP_CH_USR_DB_TYPE));

  if (LSA_HOST_PTR_ARE_EQUAL(pDB_chUsr, LSA_NULL)) 
  {
    MRP_UPPER_TRACE_02(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_usr_ch_db_init: ALLOC_LOCAL_MEM for DB_usr(channel) pChUsr=0x%x, myHandle=%d failed", pChUsr, pChUsr->myHandle);
    resp = MRP_RSP_ERR_RESOURCE;
  }
  else
  {
    resp = MRP_RSP_OK;
    pChUsr->pChUsrDB = pDB_chUsr;

    /* preset zero */
    MRP_MEMSET(pChUsr->pChUsrDB, 0, sizeof(MRP_CH_USR_DB_TYPE));
  }

  return resp;
}


/*===========================================================================
* FUNCTION : mrp_usr_ch_db_deinit
*----------------------------------------------------------------------------
* PURPOSE  : uninitialize user channel database
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr = user channel reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_usr_ch_db_deinit (const MRP_CHANNEL_TYPE_USER pChUsr)
{
  LSA_UINT16  rc;
  MRP_CH_USR_DB_TYPE *pDB_chUsr = (MRP_CH_USR_DB_TYPE *)pChUsr->pChUsrDB;
  
  MRP_UPPER_TRACE_02(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_usr_ch_db_deinit entered: pChUsr=0x%x, myHandle=%d", pChUsr, pChUsr->myHandle);
  
  MRP_FREE_LOCAL_MEM(&rc,(MRP_LOCAL_MEM_PTR_TYPE)pDB_chUsr);
  MRP_FATAL1(rc == LSA_RET_OK);
  pChUsr->pChUsrDB = LSA_NULL;

}

/*===========================================================================
* FUNCTION : mrp_usr_init
*----------------------------------------------------------------------------
* PURPOSE  : init data structures of a user channel (USR or USR2)
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr = user channel reference
*            pChPrmUser = first user channel, which is already open (CM-PD channel)
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : for init/deinit function channel declaration not "const" 
*==========================================================================*/
static LSA_VOID mrp_usr_init (const MRP_CHANNEL_TYPE_USER pChUsr, const MRP_CONST_CHANNEL_TYPE_USER pChPrmUsr)
{
  LSA_UINT16 i, mrpMaxInstance, mrpInMaxInstance;
  LSA_UINT8 isPrmUser;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChUsr->pChSysDB->pEdd;

  isPrmUser = LSA_FALSE;

  mrpMaxInstance = mrp_sys_get_num_of_instances(pChUsr);
  mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pChUsr);

  MRP_UPPER_TRACE_02(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_usr_init entered: pChUsr=0x%x, myHandle=%d", pChUsr, pChUsr->myHandle);

  mrp_usr_db_init (pChUsr, mrpMaxInstance, mrpInMaxInstance);
  mrp_ll_add_user_reference (pDBedd->pChSys, pChUsr->detailptr->PathType, pChUsr);

  if (pChUsr->detailptr->PathType == MRP_PATH_TYPE_USER_2) 
  {
    MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pChPrmUsr, LSA_NULL));
    mrp_usr_add_prm_reference (pChUsr, pChPrmUsr);

    MRP_UPPER_TRACE_01(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_usr_init: priv. USR2 channel cnt = 1 now, isPrmUser=%d", isPrmUser);
  }
  else
  {
    if (pChPrmUsr == LSA_NULL)
    {
      if (pChUsr->detailptr->type.user.isPrmInterface == MRP_IS_PRM_INTERFACE_ON)
      {
        isPrmUser = LSA_TRUE;
        mrp_rec_init (pChUsr, mrpMaxInstance, mrpInMaxInstance);
        mrp_usr_add_prm_reference (LSA_NULL, pChUsr);
      }
    }
    else
      mrp_usr_add_prm_reference (pChUsr, pChPrmUsr);

    mrp_oha_db_init (pChUsr, mrpMaxInstance);

    i = mrp_usr_get_common_usr_ch_cnt (pChUsr->detailptr->NicId);
    MRP_UPPER_TRACE_02(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_usr_init: common USR channel cnt = %d now, isPrmUser= %d", i, isPrmUser);
    LSA_UNUSED_ARG(i);   /* hhr: to please the compiler in case traces are disabled. */
  }
  LSA_UNUSED_ARG(isPrmUser);   /* hhr: to please the compiler in case traces are disabled. */
}

/*===========================================================================
* FUNCTION : mrp_usr_deinit
*----------------------------------------------------------------------------
* PURPOSE  : deinitialize data structures of a user channel (USR or USR2)  
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr = user channel reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : for init/deinit function channel declaration not "const" 
*==========================================================================*/
static LSA_VOID mrp_usr_deinit (const MRP_CONST_CHANNEL_TYPE_USER pChUsr)
{
  LSA_UINT16 i, mrpMaxInstance, mrpInMaxInstance;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChUsr->pChSysDB->pEdd;

  mrpMaxInstance = mrp_sys_get_num_of_instances(pChUsr);
  mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pChUsr);
  
  MRP_UPPER_TRACE_02(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_usr_deinit entered: pChUsr=0x%x, myHandle=%d", pChUsr, pChUsr->myHandle);

  if (pChUsr->detailptr->PathType == MRP_PATH_TYPE_USER) 
  {
    mrp_oha_db_deinit(pChUsr, mrpMaxInstance);

    if (pChUsr->detailptr->type.user.isPrmInterface == MRP_IS_PRM_INTERFACE_ON)
    {
      mrp_rec_deinit(pChUsr, mrpMaxInstance, mrpInMaxInstance);
      mrp_usr_remove_prm_reference(LSA_NULL, pChUsr);
    }
    else
    {
      mrp_usr_remove_prm_reference(pChUsr, LSA_NULL);
    }

    i = mrp_usr_get_common_usr_ch_cnt (pChUsr->detailptr->NicId);
    MRP_UPPER_TRACE_01(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_usr_deinit: common USR channel cnt = %d, before completing close chan", i);
    LSA_UNUSED_ARG(i);   /* hhr: to please the compiler in case traces are disabled. */
  }
  else
    mrp_usr_remove_prm_reference(pChUsr, LSA_NULL);

  mrp_ll_remove_user_reference (pDBedd->pChSys, pChUsr->detailptr->PathType, pChUsr);

  mrp_usr_db_deinit(pChUsr, mrpMaxInstance, mrpInMaxInstance);
}
