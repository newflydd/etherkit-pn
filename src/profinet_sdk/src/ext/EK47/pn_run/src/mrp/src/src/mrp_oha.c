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
/*  F i l e               &F: mrp_oha.c                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  lower layer functions for a common user channel (e.g. established by oha)*/
/*  processing topo ctrl info or MRP-MIB related data                        */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef MRP_MESSAGE
/*  2009-03-19  ds    trace added                                            */
/*  2009-07-02  ds    include hierarchy changed                              */
/*  2010-05-12  ds    direction of OHA system channel is OHA->MRP now        */
/*                    open/close handling redesigned                         */
/*  2010-05-25  ds    handle new OHA MIB requests                            */
/*  2010-06-11  ds    common user channel functionality implemented          */
/*  2010-07-19  ds    new feature MRP MIB implemented according to           */
/*                    IEC 62439-2                                            */
/*  2010-10-14  ds    write rPort states to MIB: don't write state           */
/*                    CONNECTED_BASE, if well-defined state already set,     */
/*                    vlanId handling corrected in MIB                       */
/*  2012-04-04  ds    introduction of N MRP instances per interface          */
/*                                                                           */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID   5
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
static LSA_UINT8 mrp_oha_check_ringstate_change (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance, MRP_OPC_RINGSTATE_INDICATION_TYPE *pMrpStateCurrent);
static LSA_VOID mrp_oha_get_current_ringstate (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance, MRP_OPC_RINGSTATE_INDICATION_TYPE *pMrpStateCurrent);

static LSA_VOID mrp_oha_init_ringstate_ind (const MRP_OHA_DB_TYPE *pDBoha, MRP_MIB_DB_TYPE *pDBmib);
static LSA_VOID mrp_mib_init_var (MRP_MIB_DB_TYPE *pDBmib);


/*---------------------------------------------------------------------------*/
/*                                globals                                    */
/*---------------------------------------------------------------------------*/


/*===========================================================================*/
/*---------------------------------------------------------------------------*/
/*       START of section for MRP-OHA user channel related functions         */
/*---------------------------------------------------------------------------*/
/*===========================================================================*/

/*===========================================================================
* FUNCTION : mrp_oha_get_status
*----------------------------------------------------------------------------
* PURPOSE  : get mrp MIB parameters
*----------------------------------------------------------------------------
* RETURNS  : MRP_RET_OK/MRP_RET_ERR
*----------------------------------------------------------------------------
* INPUTS   : pChUsr = user channel reference
*            pRQB   = upper rqb reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT16 mrp_oha_get_status (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB)
{
  LSA_UINT16 instance;
  MRP_GET_STATUS_PTR_TYPE pGetStat;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChUsr->pChSysDB->pEdd;
  MRP_UPPER_ARGS_PTR_TYPE pArgs = MRP_RQB_GET_ARGS_PTR(pRQB);
  MRP_MIB_DB_TYPE *pDBmib;
  MRP_MIB_DB_TYPE *pDBmibBase = (MRP_MIB_DB_TYPE *)pChUsr->pChSysDB->pMrpMIB;

  pGetStat = &pArgs->ctrl.get_status;

  MRP_ASSERT (pArgs->ctrl.get_status.mrpInstance > 0 && pArgs->ctrl.get_status.mrpInstance <= EDD_CFG_MAX_MRP_INSTANCE_CNT);
  instance = pArgs->ctrl.get_status.mrpInstance - 1;
  
  MRP_UPPER_TRACE_02(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_oha_get_status[inst=%d] called: pDomainName=0x%x", instance, pGetStat->pDomainName);
  
  if (instance < mrp_sys_get_num_of_instances(pChUsr))
  {
    pDBmib = &pDBmibBase[instance];

    if (pGetStat->pDomainName != LSA_NULL)
    {
      MRP_MEMCPY (pGetStat->pDomainName, pDBmib->domainName, pDBmib->domainNameLength);
      pGetStat->domainNameLength  = pDBmib->domainNameLength;
    }
    else
    {
      pGetStat->domainNameLength  = 0;
    }
    pGetStat->operRole            = pDBmib->domainOperRole;

    pGetStat->rPort1              = pDBmib->domainDataRPort1.mrpPortId;
    pGetStat->rPort2              = pDBmib->domainDataRPort2.mrpPortId;
    pGetStat->rPort1State         = pDBmib->domainDataRPort1.mrpMibState;
    pGetStat->rPort2State         = pDBmib->domainDataRPort2.mrpMibState;

    pGetStat->domainError         = pDBmib->domainError;
    pGetStat->domainState         = pDBmib->domainState;
    pGetStat->domainRingOpenCount = pDBmib->domainRingOpenCount;
  
    mrp_mib_calc_ringopen_time(pDBedd->pChSys, instance);

    pGetStat->domainLastRingOpenChange = pDBmib->domainLastRingOpenChange / MRP_MIB_TIMETICKS_10MS;
    /* the ringOpenChangeTime value has to be delivered in "timeticks", i.e. multiples of 1/100 seconds     */
    /* time is measured in milliseconds internally --> thus the ms-value has to be divided by 10            */
    /* to get multiples of 1/100 seconds                                                                    */

    pGetStat->domainRoundTripDelayMax  = pDBmib->domainRoundTripDelayMax;
    pGetStat->domainRoundTripDelayMin  = pDBmib->domainRoundTripDelayMin;
    pGetStat->domainResetRoundTripDelays = pDBmib->domainResetRoundTripDelays;

    MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_OK);
    mrp_usr_call_upper_cbf_proc_next_req (pChUsr, pRQB);

    return MRP_RET_OK;
  }
  else
  {
    MRP_UPPER_TRACE_04(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> MRP_OPC_GET_STATUS[inst=%d]: request rejected, pChUsr=0x%x, state=%d, myHandle=%d, resp=MRP_RSP_ERR_PRM_DATA",
      instance, pChUsr, pChUsr->state, pChUsr->myHandle);
    
    MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_PRM_DATA);
    return MRP_RET_ERR;
  }
}

/*===========================================================================
* FUNCTION : mrp_oha_set_status
*----------------------------------------------------------------------------
* PURPOSE  : set mrp MIB parameters
*----------------------------------------------------------------------------
* RETURNS  : MRP_RET_OK/MRP_RET_ERR
*----------------------------------------------------------------------------
* INPUTS   : pChUsr = user channel reference
*            pRQB   = upper rqb reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT16 mrp_oha_set_status (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB)
{
  LSA_UINT16 instance;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChUsr->pChSysDB->pEdd;
  MRP_UPPER_ARGS_PTR_TYPE pArgs = MRP_RQB_GET_ARGS_PTR(pRQB);
 
  MRP_ASSERT (pArgs->ctrl.set_status.mrpInstance > 0 && pArgs->ctrl.set_status.mrpInstance <= EDD_CFG_MAX_MRP_INSTANCE_CNT);
  instance = pArgs->ctrl.set_status.mrpInstance - 1;
  
  MRP_UPPER_TRACE_02(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_oha_set_status[inst=%d]: resetRoundTripDelays=%d",
    instance, pArgs->ctrl.set_status.domainResetRoundTripDelays);
  
  if (instance < mrp_sys_get_num_of_instances(pChUsr))
  {
    mrp_mib_reset_round_trip_delays (pDBedd->pChSys, instance, pArgs->ctrl.set_status.domainResetRoundTripDelays);  

    MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_OK);
    mrp_usr_call_upper_cbf_proc_next_req (pChUsr, pRQB);

    return MRP_RET_OK;
  }
  else
  {
    MRP_UPPER_TRACE_04(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> MRP_OPC_SET_STATUS[inst=%d]: request rejected, pChUsr=0x%x, state=%d, myHandle=%d, resp=MRP_RSP_ERR_PRM_DATA",
      instance, pChUsr, pChUsr->state, pChUsr->myHandle);

    MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_PRM_DATA);
    return MRP_RET_ERR;
  }
}

/*===========================================================================
* FUNCTION : mrp_oha_write_current_ringstate_data
*----------------------------------------------------------------------------
* PURPOSE  : save current ringstate 
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr      = channel reference
*            instance    = affected MRP instance
*            topoCtrl    = current topoCtrl mode
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_oha_write_current_ringstate_data (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance, MRP_USR_ENUM_TOPO_CTRL topoCtrl)
{
  MRP_OPC_RINGSTATE_INDICATION_TYPE *pMrpStateCurrent;
  MRP_MIB_DB_TYPE *pDBmib;
  MRP_OHA_DB_TYPE *pDBoha = (MRP_OHA_DB_TYPE *)pChUsr->pChUsrDB->pOha;
  MRP_MIB_DB_TYPE *pDBmibBase = (MRP_MIB_DB_TYPE *)pChUsr->pChSysDB->pMrpMIB;
  
  
  pDBmib = &pDBmibBase[instance];
  
  pMrpStateCurrent = &pDBoha->pInstData[instance].RingstateIndCurrent.ohaStateInd;

  /* LLDP related info */
  pMrpStateCurrent->topoCtrlMode  = topoCtrl;

  if (topoCtrl != MRP_USR_ENUM_TOPO_CTRL_NONE)
  {
    pMrpStateCurrent->mrpStatus = MRP_OPERATION_ON;
  }
  else
  {
    pMrpStateCurrent->mrpStatus = MRP_OPERATION_OFF;
  }

  /* domainUUID is important for MRP MIB and LLDP signaling as well */
  MRP_MEMCPY (pMrpStateCurrent->domainUUID, pDBmib->domainUUID, sizeof(pMrpStateCurrent->domainUUID));

  /* MRP MIB related info */
  pMrpStateCurrent->adminRole                  = pDBmib->domainAdminRole;
  pMrpStateCurrent->operRole                   = pDBmib->domainOperRole;
  pMrpStateCurrent->domainVlanId               = pDBmib->domainVlanId;
  pMrpStateCurrent->domainManagerPrio          = pDBmib->domainManagerPrio;

  pMrpStateCurrent->domainMrmReactOnLinkChange = pDBmib->domainMrmReactOnLinkChange; /* constant */
  pMrpStateCurrent->domainBlocked              = pDBmib->domainBlocked;              /* constant */

  pMrpStateCurrent->rPort1                     = pDBmib->domainDataRPort1.mrpPortId;
  pMrpStateCurrent->rPort2                     = pDBmib->domainDataRPort2.mrpPortId;
}


/*===========================================================================
* FUNCTION : mrp_oha_multi_usr_write_curr_ringstate_data
*----------------------------------------------------------------------------
* PURPOSE  : save current ringstate data to multiple user channels
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys      = channel reference
*            instance    = affected MRP instance
*            topoCtrl    = current topoCtrl mode
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_oha_multi_usr_write_curr_ringstate_data (const MRP_CONST_CHANNEL_TYPE_USER pChSys, LSA_UINT16 instance, MRP_USR_ENUM_TOPO_CTRL topoCtrl)
{
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  LSA_UINT16 i;

  /* topo ctrl */
  for (i = 0; i < MRP_MAX_USER_CHANNELS_PER_IF; i++)
  {
    if (pDBedd->pChUsr[i] != LSA_NULL)
    {
      MRP_UPPER_TRACE_03(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_oha_multi_usr_write_curr_ringstate_data[inst=%d]: pDBedd->pChUsr[i]=0x%x, topoCtrl=%d", instance, pDBedd->pChUsr[i], topoCtrl);
      mrp_oha_write_current_ringstate_data (pDBedd->pChUsr[i], instance, topoCtrl);
    }
  }
}

/*===========================================================================
* FUNCTION : mrp_oha_get_current_ringstate
*----------------------------------------------------------------------------
* PURPOSE  : get current ringstate 
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr       = channel reference
*            instance     = affected MRP instance 
*            &ohaStateInd = pointer to struct of type MRP_OPC_RINGSTATE_INDICATION_TYPE
* OUTPUTS  : *ohaStateInd 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_oha_get_current_ringstate (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance, MRP_OPC_RINGSTATE_INDICATION_TYPE *pMrpStateCurrent)
{
  MRP_OHA_DB_TYPE *pDBoha = (MRP_OHA_DB_TYPE *)pChUsr->pChUsrDB->pOha;
  
  *pMrpStateCurrent = pDBoha->pInstData[instance].RingstateIndCurrent.ohaStateInd;
}

/*===========================================================================
* FUNCTION : mrp_oha_check_ringstate
*----------------------------------------------------------------------------
* PURPOSE  : compares last indicated ringstate with current ringstate 
*----------------------------------------------------------------------------
* RETURNS  : LSA_TRUE or LSA_FALSE (difference in ringState)
*----------------------------------------------------------------------------
* INPUTS   : pChUsr           = channel reference
*            instance         = affected MRP instance 
*            pMrpStateCurrent = pointer to struct with current ringstate variables
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_UINT8 mrp_oha_check_ringstate_change (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance, MRP_OPC_RINGSTATE_INDICATION_TYPE *pMrpStateCurrent)
{
  LSA_UINT8 ringStateDiff;
  MRP_OPC_RINGSTATE_INDICATION_TYPE *pStateIndLastSignaled;
  MRP_OHA_DB_TYPE *pDBoha = (MRP_OHA_DB_TYPE *)pChUsr->pChUsrDB->pOha;
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChUsr->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  pStateIndLastSignaled = &pDBoha->pInstData[instance].RingstateIndLastSignaled.ohaStateInd;
  ringStateDiff = LSA_FALSE;

  MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_oha_check_ringstate_change[inst=%d] entered: pChUsr=0x%x", instance, pChUsr);

  if (pStateIndLastSignaled->rPort1            != pMrpStateCurrent->rPort1            ||
      pStateIndLastSignaled->rPort2            != pMrpStateCurrent->rPort2            ||
      pStateIndLastSignaled->mrpStatus         != pMrpStateCurrent->mrpStatus         ||
      pStateIndLastSignaled->topoCtrlMode      != pMrpStateCurrent->topoCtrlMode      ||
      MRP_MEMCMP(pStateIndLastSignaled->domainUUID, pMrpStateCurrent->domainUUID, sizeof(pStateIndLastSignaled->domainUUID)) ||
      pStateIndLastSignaled->domainManagerPrio != pMrpStateCurrent->domainManagerPrio ||
      pStateIndLastSignaled->domainVlanId      != pMrpStateCurrent->domainVlanId      ||
      pStateIndLastSignaled->adminRole         != pMrpStateCurrent->adminRole         ||
      pStateIndLastSignaled->operRole          != pMrpStateCurrent->operRole
     )
  {
    ringStateDiff = LSA_TRUE;
  }

  if (ringStateDiff)
  {
    if (pStateIndLastSignaled->rPort1            != pMrpStateCurrent->rPort1)
    {
      MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "LastSignaled->rPort1=%d, Current->rPort1=%d", pStateIndLastSignaled->rPort1, pMrpStateCurrent->rPort1);
    }
    if (pStateIndLastSignaled->rPort2            != pMrpStateCurrent->rPort2)
    {
      MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "LastSignaled->rPort2=%d, Current->rPort2=%d", pStateIndLastSignaled->rPort2, pMrpStateCurrent->rPort2);
    }
    if (pStateIndLastSignaled->mrpStatus         != pMrpStateCurrent->mrpStatus)
    {
      MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "LastSignaled->mrpStatus=%d, Current->mrpStatus=%d", pStateIndLastSignaled->mrpStatus, pMrpStateCurrent->mrpStatus);
    }
    if (pStateIndLastSignaled->topoCtrlMode      != pMrpStateCurrent->topoCtrlMode)
    {
      MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "LastSignaled->topoCtrlMode=%d, Current->topoCtrlMode=%d", pStateIndLastSignaled->topoCtrlMode, pMrpStateCurrent->topoCtrlMode);
    }
    if (MRP_MEMCMP(pStateIndLastSignaled->domainUUID, pMrpStateCurrent->domainUUID, sizeof(pStateIndLastSignaled->domainUUID)))
    {
      MRP_UPPER_TRACE_BYTE_ARRAY(traceIdx, LSA_TRACE_LEVEL_NOTE, "LastSignaled->DomainUUID=", pStateIndLastSignaled->domainUUID, 6);
      MRP_UPPER_TRACE_BYTE_ARRAY(traceIdx, LSA_TRACE_LEVEL_NOTE, "Current->DomainUUID=", pMrpStateCurrent->domainUUID, 6);
    }
    if (pStateIndLastSignaled->domainManagerPrio != pMrpStateCurrent->domainManagerPrio)
    {
      MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "LastSignaled->domainManagerPrio=%d, Current->domainManagerPrio=%d", pStateIndLastSignaled->domainManagerPrio, pMrpStateCurrent->domainManagerPrio);
    }
    if (pStateIndLastSignaled->domainVlanId      != pMrpStateCurrent->domainVlanId)
    {
      MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "LastSignaled->domainVlanId=%d, Current->domainVlanId=%d", pStateIndLastSignaled->domainVlanId, pMrpStateCurrent->domainVlanId);
    }
    if (pStateIndLastSignaled->adminRole         != pMrpStateCurrent->adminRole)
    {
      MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "LastSignaled->adminRole=%d, Current->adminRole=%d", pStateIndLastSignaled->adminRole, pMrpStateCurrent->adminRole);
    }
    if (pStateIndLastSignaled->operRole          != pMrpStateCurrent->operRole)
    {
      MRP_UPPER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "LastSignaled->operRole=%d, Current->operRole=%d", pStateIndLastSignaled->operRole, pMrpStateCurrent->operRole);
    }
  }

  return ringStateDiff;
}

/*===========================================================================
* FUNCTION : mrp_oha_ringstate_ind_done
*----------------------------------------------------------------------------
* PURPOSE  : indicate state change
*----------------------------------------------------------------------------
* RETURNS  : LSA_TRUE = RQB in queue
*            LSA_FALSE = no RQB in queue
*----------------------------------------------------------------------------
* INPUTS   : pChUsr   = channel reference
*            instance = affected MRP instance 
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT8 mrp_oha_ringstate_ind_done (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance)
{
  LSA_UINT32 instanceNum;
  MRP_OPC_RINGSTATE_INDICATION_TYPE mrpStateCurrent;
  MRP_UPPER_RQB_PTR_TYPE pRQB = LSA_NULL;
  MRP_UPPER_ARGS_PTR_TYPE pArgs;
  LSA_UINT8 ringStateDiff;
  LSA_UINT16 resp = MRP_RSP_OK;
  MRP_OHA_DB_TYPE *pDBoha = (MRP_OHA_DB_TYPE *)pChUsr->pChUsrDB->pOha;

  /* get current ringstate */
  mrp_oha_get_current_ringstate(pChUsr, instance, &mrpStateCurrent);
  ringStateDiff = mrp_oha_check_ringstate_change (pChUsr, instance, &mrpStateCurrent);

  MRP_UPPER_TRACE_04(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_oha_ringstate_ind_done[inst=%d] entered: pChUsr=0x%x, myHandle=%d, ringStateDiff=%d", instance, pChUsr, pChUsr->myHandle, ringStateDiff);

  if (pChUsr->state == MRP_CH_STATE_CLOSING || ringStateDiff) 
  {
    MRP_RQB_POP_FROM_TOP(pDBoha->pInstData[instance].pRingstateIndRqbB, pDBoha->pInstData[instance].pRingstateIndRqbT, pRQB);
    if (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL)) 
    {
      MRP_UPPER_TRACE_03(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "<< MRP_OPC_RINGSTATE_INDICATION done[inst=%d]: out of resource, pChUsr=0x%x, myHandle=%d", instance, pChUsr, pChUsr->myHandle);
        pDBoha->pInstData[instance].RingstateIndOutOfResource = MRP_IND_RESOURCE_MISSING_ON;

      return LSA_FALSE;
    }
    pArgs = MRP_RQB_GET_ARGS_PTR(pRQB);

    /* copy current ringstate data to rqb */
    pArgs->state.state_ind.ohaStateInd = pDBoha->pInstData[instance].RingstateIndLastSignaled.ohaStateInd = mrpStateCurrent;
    
    if (pChUsr->state == MRP_CH_STATE_CLOSING) 
    {
      resp = MRP_RSP_OK_CANCEL;
      MRP_RQB_SET_RESPONSE(pRQB, resp);
    }
    instanceNum = pArgs->state.state_ind.ohaStateInd.mrpInstance - 1;
    MRP_UPPER_TRACE_05(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "<< MRP_OPC_RINGSTATE_INDICATION done[inst=%d]: cbf called, pChUsr=0x%x, myHandle=%d, state=%d, resp=0x%x",
                       instanceNum, pChUsr, pChUsr->myHandle, pChUsr->state, resp);

    LSA_UNUSED_ARG(instanceNum);   /* hhr: to please the compiler in case traces are disabled. */

    mrp_usr_call_upper_cbf (pChUsr, pRQB);
  }
  return LSA_TRUE;
}

/*===========================================================================
* FUNCTION : mrp_oha_multi_usr_ringstate_ind_done
*----------------------------------------------------------------------------
* PURPOSE  : indicate ringstate change
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference
*            instance = affected MRP instance 
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_oha_multi_usr_ringstate_ind_done (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance)
{
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  LSA_UINT16 i;

  for (i = 0; i < MRP_MAX_USER_CHANNELS_PER_IF; i++)
  {
    if (pDBedd->pChUsr[i] != LSA_NULL)
    {
      (void)(mrp_oha_ringstate_ind_done (pDBedd->pChUsr[i], instance));
    }
  }
}

/*===========================================================================
* FUNCTION : mrp_oha_ringstate_ind_provide_rqb
*----------------------------------------------------------------------------
* PURPOSE  : set indication rqbs for signaling 
*----------------------------------------------------------------------------
* RETURNS  : MRP_RET_OK/MRP_RET_ERR
*----------------------------------------------------------------------------
* INPUTS   : pChUsr = channel reference
*            pRQB   = upper rqb reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : queues indication requests 
*==========================================================================*/
LSA_UINT16 mrp_oha_ringstate_ind_provide_rqb (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB)
{
  LSA_UINT16 instance;
  MRP_UPPER_ARGS_PTR_TYPE pArgs;
  MRP_OHA_DB_TYPE *pDBoha = (MRP_OHA_DB_TYPE *)pChUsr->pChUsrDB->pOha;

  pArgs = MRP_RQB_GET_ARGS_PTR(pRQB);
  
  MRP_ASSERT (pArgs->state.state_ind.ohaStateInd.mrpInstance > 0 && pArgs->state.state_ind.ohaStateInd.mrpInstance <= EDD_CFG_MAX_MRP_INSTANCE_CNT);
  instance = pArgs->state.state_ind.ohaStateInd.mrpInstance - 1;
  
  if (pChUsr->state != MRP_CH_STATE_CLOSING) 
  {
    if (instance < mrp_sys_get_num_of_instances(pChUsr))
    {
      MRP_UPPER_TRACE_04(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, ">> MRP_OPC_RINGSTATE_INDICATION[inst=%d]: rqb provided, pChUsr=0x%x, state=%d, myHandle=%d, resp=MRP_RSP_OK",
        instance, pChUsr, pChUsr->state, pChUsr->myHandle);

      MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_OK);
      MRP_RQB_PUSH_TO_BOTTOM(pDBoha->pInstData[instance].pRingstateIndRqbB, pDBoha->pInstData[instance].pRingstateIndRqbT, pRQB);
      pChUsr->pRQB = LSA_NULL;

      if (pDBoha->pInstData[instance].RingstateIndOutOfResource)
      {
        pDBoha->pInstData[instance].RingstateIndOutOfResource = MRP_IND_RESOURCE_MISSING_OFF;

        (void)(mrp_oha_ringstate_ind_done (pChUsr, instance));
      }
      return MRP_RET_OK;
    }
    else
    {
      MRP_UPPER_TRACE_04(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> MRP_OPC_RINGSTATE_INDICATION[inst=%d]: rqb provided, pChUsr=0x%x, state=%d, myHandle=%d, resp=MRP_RSP_ERR_PRM_DATA",
        instance, pChUsr, pChUsr->state, pChUsr->myHandle);

      MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_PRM_DATA);

      return MRP_RET_ERR;
    }
  }
  else
  {
    MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_SEQUENCE);
    
    MRP_UPPER_TRACE_04(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> MRP_OPC_RINGSTATE_INDICATION[inst=%d]: rqb provided, pChUsr=0x%x, state=%d, myHandle=%d, resp=MRP_RSP_ERR_SEQUENCE",
                       instance, pChUsr, pChUsr->state, pChUsr->myHandle);

    return MRP_RET_ERR;
  }
}

/*===========================================================================
* FUNCTION : mrp_oha_init_ringstate_ind
*----------------------------------------------------------------------------
* PURPOSE  : initialize MIB variables
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : *pDBoha = reference to one instance of user data related 
*            to an OHA-MRP channel
*            *pDBmib = reference to one instance of MRP MIB
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_oha_init_ringstate_ind (const MRP_OHA_DB_TYPE *pDBoha, MRP_MIB_DB_TYPE *pDBmib)
{
  MRP_OPC_RINGSTATE_INDICATION_TYPE *pMrpStateCurrent;
  MRP_OPC_RINGSTATE_INDICATION_TYPE *pStateIndLastSignaled;

  pMrpStateCurrent = &pDBoha->pInstData[pDBmib->instance].RingstateIndCurrent.ohaStateInd;
  pStateIndLastSignaled = &pDBoha->pInstData[pDBmib->instance].RingstateIndLastSignaled.ohaStateInd;

  pMrpStateCurrent->mrpInstance                = pDBmib->instance + 1;
  pMrpStateCurrent->domainBlocked              = pDBmib->domainBlocked;              /* not changeable */
  pMrpStateCurrent->domainMrmReactOnLinkChange = pDBmib->domainMrmReactOnLinkChange; /* not changeable */

  MRP_MEMCPY(pMrpStateCurrent->domainUUID, pDBmib->domainUUID, sizeof(pMrpStateCurrent->domainUUID));
  pMrpStateCurrent->domainVlanId      = pDBmib->domainVlanId;
  pMrpStateCurrent->adminRole         = pDBmib->domainAdminRole;
  pMrpStateCurrent->operRole          = pDBmib->domainOperRole;
  pMrpStateCurrent->domainManagerPrio = pDBmib->domainManagerPrio;
  pMrpStateCurrent->rPort1            = pDBmib->domainDataRPort1.mrpPortId;
  pMrpStateCurrent->rPort2            = pDBmib->domainDataRPort2.mrpPortId;

  pMrpStateCurrent->mrpStatus         = (pDBmib->domainOperRole == MRP_MIB_DOMAIN_ROLE_DISABLE) ? MRP_OPERATION_OFF : MRP_OPERATION_ON;
  pMrpStateCurrent->topoCtrlMode      = MRP_USR_ENUM_TOPO_CTRL_NONE;

  *pStateIndLastSignaled = *pMrpStateCurrent;
  /* force MRP to signal MIB data once after indication RQB was provided by OHA */  
  pStateIndLastSignaled->topoCtrlMode = MRP_USR_ENUM_TOPO_CTRL_MAX; 
}

/*===========================================================================
* FUNCTION : mrp_oha_db_init
*----------------------------------------------------------------------------
* PURPOSE  : initialize lower layer structures
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr         = channel reference to a common usr channel
*            mrpMaxInstance = maximum number of MRP instances to be initialized
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_oha_db_init (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 mrpMaxInstance)
{
  LSA_UINT16 i;
  MRP_OHA_DB_TYPE *pDBoha = LSA_NULL;
  MRP_MIB_DB_TYPE *pDBmibBase = (MRP_MIB_DB_TYPE *)pChUsr->pChSysDB->pMrpMIB;
  MRP_MIB_DB_TYPE *pDBmib;

  MRP_PROGRAM_TRACE_03(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_oha_db_init entered: pChusr=0x%x, myHandle=%d, mrpMaxInstance=%d", pChUsr, pChUsr->myHandle, mrpMaxInstance);
  
  MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBoha), sizeof(MRP_OHA_DB_TYPE));
  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBoha, LSA_NULL));
  pChUsr->pChUsrDB->pOha = pDBoha;
  
  /* preset zero */
  MRP_MEMSET(pDBoha, 0, sizeof(MRP_OHA_DB_TYPE));

  if (mrpMaxInstance)
  {
    MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBoha->pInstData), (LSA_UINT16)(sizeof(MRP_OHA_INST_DATA) * mrpMaxInstance));
    MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBoha->pInstData, LSA_NULL));
  }

  for (i = 0; i < mrpMaxInstance; i++)
  {
    MRP_MEMSET(&(pDBoha->pInstData[i]), 0, sizeof(MRP_OHA_INST_DATA));

    pDBoha->pInstData[i].RingstateIndCurrent.ohaStateInd.mrpInstance = i + 1;
    pDBoha->pInstData[i].RingstateIndLastSignaled.ohaStateInd.mrpInstance = i + 1;

    pDBoha->pInstData[i].RingstateIndOutOfResource = MRP_IND_RESOURCE_MISSING_ON;

    pDBmib = &pDBmibBase[i];
    
    mrp_oha_init_ringstate_ind (pDBoha, pDBmib);    
  }
}


/*===========================================================================
* FUNCTION : mrp_oha_db_deinit
*----------------------------------------------------------------------------
* PURPOSE  : uninitialize lower layer structures
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr         = channel reference to a common usr channel
*            mrpMaxInstance = maximum number of MRP instances to be initialized
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_oha_db_deinit (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 mrpMaxInstance)
{
  LSA_UINT16  rc;
  MRP_OHA_DB_TYPE *pDBoha = (MRP_OHA_DB_TYPE *)pChUsr->pChUsrDB->pOha;

  MRP_PROGRAM_TRACE_03(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_oha_db_deinit entered: pChusr=0x%x, myHandle=%d, mrpMaxInstance=%d", pChUsr, pChUsr->myHandle, mrpMaxInstance);
  
  if (mrpMaxInstance)
  {
    MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBoha->pInstData);
    MRP_FATAL1(rc == LSA_RET_OK);
  }
  
  MRP_FREE_LOCAL_MEM(&rc,(MRP_LOCAL_MEM_PTR_TYPE)pDBoha);
  MRP_FATAL1(rc == LSA_RET_OK);
  pChUsr->pChUsrDB->pOha = LSA_NULL;
 
}

/*===========================================================================*/
/*---------------------------------------------------------------------------*/
/*       END of section for MRP-OHA user channel related functions           */
/*---------------------------------------------------------------------------*/
/*===========================================================================*/




/*===========================================================================*/
/*---------------------------------------------------------------------------*/
/*   START of section for MRP-OHA-MIB system channel related functions       */
/*   (MRP-MIB is related to a MRP instance (domain)                          */
/*---------------------------------------------------------------------------*/
/*===========================================================================*/

/*===========================================================================
* FUNCTION : mrp_mib_map_edd_and_link_state_to_mib_state
*----------------------------------------------------------------------------
* PURPOSE  : map an EDD port state to a MIB port state
*----------------------------------------------------------------------------
* RETURNS  : MRP_MIB_ENUM_RPORT_STATE
*----------------------------------------------------------------------------
* INPUTS   : pChSys       = channel reference to system channel
*            instance     = affected MRP instance 
*            rPort        = ring port, which has a state change
*            eddPortState = current ring port state reported by EDD
*            linkState    = current EDD link state
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
MRP_MIB_ENUM_RPORT_STATE mrp_mib_map_edd_and_link_state_to_mib_state (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT16 rPort, LSA_UINT16 eddPortState, LSA_UINT8 linkState)
{
  MRP_MIB_ENUM_RPORT_STATE mibPortState, preMibPortState;
  MRP_MIB_RPORT_STRUCT *rPortPtr = LSA_NULL;
  MRP_MIB_DB_TYPE *pDBmibBase = (MRP_MIB_DB_TYPE *)pChSys->pChSysDB->pMrpMIB;
  MRP_MIB_DB_TYPE *pDBmib;

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  pDBmib = &pDBmibBase[instance];

  if (rPort == pDBmib->domainDataRPort1.mrpPortId)
  {
    rPortPtr = &pDBmib->domainDataRPort1;
  }
  else
  {
    if (rPort == pDBmib->domainDataRPort2.mrpPortId)
      rPortPtr = &pDBmib->domainDataRPort2;
    else
    {
      MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_mib_map_edd_and_link_state_to_mib_state[inst=%d]: rport=%d not known as rport", instance, rPort);
    }
  }

  mibPortState = MRP_MIB_RPORT_STATE_UNKNOWN;

  if (!LSA_HOST_PTR_ARE_EQUAL(rPortPtr, LSA_NULL))
  {
    rPortPtr->mrpLinkState = linkState;

    switch (eddPortState)
    {
    case EDD_PORT_STATE_DISABLE:
      rPortPtr->mrpEddState = MRP_MIB_RPORT_STATE_DISABLED;
      break;

    case EDD_PORT_STATE_BLOCKING:
      rPortPtr->mrpEddState = MRP_MIB_RPORT_STATE_BLOCKED;
      break;

    case EDD_PORT_STATE_LISTENING:
    case EDD_PORT_STATE_LEARNING:
      MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "mrp_mib_map_edd_and_link_state_to_mib_state[inst=%d]: eddPortState=%d not suitable for rport=%d -> mapped to STATE_BLOCKED", instance, eddPortState, rPort);
      rPortPtr->mrpEddState = MRP_MIB_RPORT_STATE_BLOCKED;
      break;

    case EDD_PORT_STATE_FORWARDING:
    case EDD_PORT_STATE_FORWARDING_WITHOUT_LEARNING:
      rPortPtr->mrpEddState = MRP_MIB_RPORT_STATE_FORWARDING;
      break;

    case EDD_PORT_STATE_UNCHANGED:
      break;

    default: /* no mapping possible */
      MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_mib_map_edd_and_link_state_to_mib_state[inst=%d]: eddPortState=%d UNKNOWN for rport=%d", instance, eddPortState, rPort);
      rPortPtr->mrpEddState = MRP_MIB_RPORT_STATE_UNKNOWN;
      break;
    }

    /* save the previous MIB port state for comparison purposes */
    preMibPortState = rPortPtr->mrpMibState;

    if (linkState)
    {
      mibPortState = (MRP_MIB_ENUM_RPORT_STATE)rPortPtr->mrpEddState;
      rPortPtr->mrpMibState = mibPortState;
    }
    else
    {
      mibPortState = MRP_MIB_RPORT_STATE_NOT_CONNECTED;
      rPortPtr->mrpMibState = mibPortState;
    }
    
    if (preMibPortState != rPortPtr->mrpMibState)
    {
      switch (rPortPtr->mrpMibState)
      {
      case MRP_MIB_RPORT_STATE_MRP_OFF:
        MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE, "MRP-MIB: mapped_rport_state[inst=%d] for rport=%d, rPortState=0x%x(MRP_OFF) written", instance, rPort, rPortPtr->mrpMibState);
        break;
      case MRP_MIB_RPORT_STATE_DISABLED:
        MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE, "MRP-MIB: mapped_rport_state[inst=%d] for rport=%d, rPortState=0x%x(DISABLED) written", instance, rPort, rPortPtr->mrpMibState);
        break;
      case MRP_MIB_RPORT_STATE_BLOCKED:
        MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE, "MRP-MIB: mapped_rport_state[inst=%d] for rport=%d, rPortState=0x%x(BLOCKED) written", instance, rPort, rPortPtr->mrpMibState);
        break;
      case MRP_MIB_RPORT_STATE_FORWARDING:
        MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE, "MRP-MIB: mapped_rport_state[inst=%d] for rport=%d, rPortState=0x%x(FORWARDING) written", instance, rPort, rPortPtr->mrpMibState);
        break;
      case MRP_MIB_RPORT_STATE_NOT_CONNECTED:
        MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE, "MRP-MIB: mapped_rport_state[inst=%d] for rport=%d, rPortState=0x%x(NOT_CONNECTED) written", instance, rPort, rPortPtr->mrpMibState);
        break;
      case MRP_MIB_RPORT_STATE_UNCHANGED:
        MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE, "MRP-MIB: mapped_rport_state[inst=%d] for rport=%d, UNCHANGED rPortState=0x%x", instance, rPort, rPortPtr->mrpMibState);
        break;
      case MRP_MIB_RPORT_STATE_UNKNOWN:
        /*fall through*/
      default:
        MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_ERROR, "MRP-MIB: mapped_rport_state[inst=%d] for rport=%d, rPortState=0x%x(UNKNOWN) written", instance, rPort, rPortPtr->mrpMibState);
        break;
      }
    }
  }

  return mibPortState;
}

/*===========================================================================
* FUNCTION : mrp_mib_write_link_state
*----------------------------------------------------------------------------
* PURPOSE  : write the current MRP ring port state to MRP MIB
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys     = channel reference to system channel
*            instance   = affected MRP instance 
*            rPort      = ring port, which has a state change
*            linkState  = current EDD link state
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_mib_write_link_state (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT16 rPort, LSA_UINT8 linkState)
{
  (void)(mrp_mib_map_edd_and_link_state_to_mib_state (pChSys, instance, rPort, EDD_PORT_STATE_UNCHANGED, linkState));
}


/*===========================================================================
* FUNCTION : mrp_mib_write_rports
*----------------------------------------------------------------------------
* PURPOSE  : write the current MRP ring ports to MIB database
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference to system channel
*            instance = affected MRP instance 
*            rPort1   = first ring port
*            rPort2   = second ring port
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_mib_write_rports (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT16 rPort1, LSA_UINT16 rPort2)
{
  MRP_MIB_DB_TYPE *pDBmibBase = (MRP_MIB_DB_TYPE *)pChSys->pChSysDB->pMrpMIB;
  MRP_MIB_DB_TYPE *pDBmib;
 
  pDBmib = &pDBmibBase[instance];

  if (rPort1 != pDBmib->domainDataRPort1.mrpPortId || rPort2 != pDBmib->domainDataRPort2.mrpPortId)
  {
    pDBmib->domainDataRPort1.mrpPortId = rPort1;
    pDBmib->domainDataRPort2.mrpPortId = rPort2;

    pDBmib->domainDataRPort1.mrpIfIndex = rPort1 + 1;
    pDBmib->domainDataRPort2.mrpIfIndex = rPort2 + 1;

    MRP_PROGRAM_TRACE_05(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mib_write_rports[inst=%d]: rport1=%d/ifIndrport1=%d, rport2=%d/ifIndrport2=%d written",
      instance, rPort1, pDBmib->domainDataRPort1.mrpIfIndex, rPort2, pDBmib->domainDataRPort2.mrpIfIndex);

    if (rPort1 == 0)
    {
      pDBmib->domainDataRPort1.mrpMibState = MRP_MIB_RPORT_STATE_MRP_OFF;
    }
    if (rPort2 == 0)
    {
      pDBmib->domainDataRPort2.mrpMibState = MRP_MIB_RPORT_STATE_MRP_OFF;
    }
  }
}

/*===========================================================================
* FUNCTION : mrp_mib_write_domain_state
*----------------------------------------------------------------------------
* PURPOSE  : write the current MRP domain error to MIB database
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys      = channel reference to system channel
*            instance    = affected MRP instance 
*            domainState = domain state to be written
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_mib_write_domain_state (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_MIB_ENUM_DOMAIN_STATE domainState)
{
  MRP_MIB_DB_TYPE *pDBmibBase = (MRP_MIB_DB_TYPE *)pChSys->pChSysDB->pMrpMIB;
  MRP_MIB_DB_TYPE *pDBmib;

  pDBmib = &pDBmibBase[instance];

  if (domainState != pDBmib->domainState)
  {
    if (domainState == MRP_MIB_DOMAIN_STATE_RINGOPEN ||
        domainState == MRP_MIB_DOMAIN_STATE_DISABLED)
    {
      pDBmib->ringOpenEndTime = MRP_NO_TIME_AVAILABLE;
      /* ringOpenStartTime is set at the moment the MRP ring is opened */
    }
    else
    {
      if (domainState == MRP_MIB_DOMAIN_STATE_RINGCLOSED)
      {
        if (pDBmib->ringOpenStartTime != MRP_NO_TIME_AVAILABLE)
        {
          MRP_GET_MS_CNT_VAL(&pDBmib->ringOpenEndTime);
          mrp_mib_calc_ringopen_time(pChSys, instance);

          pDBmib->ringOpenStartTime = MRP_NO_TIME_AVAILABLE;
        }
      }
    }
    pDBmib->domainState = domainState;
    MRP_PROGRAM_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mib_write_domain_state[inst=%d]: domainState=%d written", instance, pDBmib->domainState);
  }
}

/*===========================================================================
* FUNCTION : mrp_mib_write_domain_error
*----------------------------------------------------------------------------
* PURPOSE  : write the current MRP domain error to MIB database
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys       = channel reference to system channel
*            instance     = affected MRP instance 
*            domainError  = type of domain error
*            setFlag      = LSA_TRUE := set error, LSA_FALSE := reset error
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_mib_write_domain_error (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_MIB_ENUM_DOMAIN_ERROR domainError, LSA_UINT8 setFlag)
{
  LSA_UINT8 change = LSA_TRUE;
  MRP_MIB_DB_TYPE *pDBmibBase = (MRP_MIB_DB_TYPE *)pChSys->pChSysDB->pMrpMIB;
  MRP_MIB_DB_TYPE *pDBmib;

  pDBmib = &pDBmibBase[instance];

  MRP_PROGRAM_TRACE_04(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_mib_write_domain_err[inst=%d]: domainErr to change=0x%x, setFlag=%d, current MIB domainErr=0x%x", instance, domainError, setFlag, pDBmib->domainError);
  
  if (setFlag)
  {
    if ( (pDBmib->domainError & domainError) == domainError)
      change = LSA_FALSE;

    if (domainError == MRP_MIB_DOMAIN_NO_ERR)
    {
        pDBmib->domainError = domainError;
    }
    else
    {
      pDBmib->domainError |= domainError;
      if (pDBmib->domainError && pDBmib->domainError != MRP_MIB_DOMAIN_NO_ERR)
      {
        /* if an error is set, MRP_MIB_DOMAIN_NO_ERR must be reset! */
        pDBmib->domainError &= ~MRP_MIB_DOMAIN_NO_ERR;
      }
    }
  }
  else
  {
    if ((pDBmib->domainError & domainError) == 0)
      change = LSA_FALSE;

    pDBmib->domainError &= ~domainError;
    if (!pDBmib->domainError)
    {
      /* if no other error is present, MRP_MIB_DOMAIN_NO_ERR has to be set! */
      pDBmib->domainError |= MRP_MIB_DOMAIN_NO_ERR;
    }
  }

  if (change)
  {
    MRP_PROGRAM_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_mib_write_domain_err[inst=%d]: new current domainErr=0x%x", instance, pDBmib->domainError);
  }
}

/*===========================================================================
* FUNCTION : mrp_mib_write_domain_name
*----------------------------------------------------------------------------
* PURPOSE  : write the current MRP domain name to MIB database
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys           = channel reference to system channel
*            instance         = affected MRP instance 
*            pDomainName      = pointer to domain name to be written
*            lengthDomainName = length of domain name
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_mib_write_domain_name (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT8 *pDomainName, LSA_UINT8 lengthDomainName)
{
  MRP_MIB_DB_TYPE *pDBmibBase = (MRP_MIB_DB_TYPE *)pChSys->pChSysDB->pMrpMIB;
  MRP_MIB_DB_TYPE *pDBmib;

  pDBmib = &pDBmibBase[instance];
  
  MRP_MEMCPY(pDBmib->domainName, pDomainName, lengthDomainName);
  pDBmib->domainName[lengthDomainName] = '\0';
  pDBmib->domainNameLength = lengthDomainName;

  if (lengthDomainName >= 5)
  {
    MRP_PROGRAM_TRACE_06(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mib_write_domain_name[inst=%d]: domain_name (%c%c%c%c%c...) written",
      instance, pDBmib->domainName[0], pDBmib->domainName[1], pDBmib->domainName[2], pDBmib->domainName[3], pDBmib->domainName[4]);
  }
  else
    MRP_PROGRAM_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mib_write_domain_name[inst=%d]: domain_name written", instance);
   
}

/*===========================================================================
* FUNCTION : mrp_mib_write_vlanid
*----------------------------------------------------------------------------
* PURPOSE  : write the current VLAN ID to MIB database
*----------------------------------------------------------------------------
* RETURNS  : LSA_TRUE  := VLAN ID written (VLAN ID valid)
*            LSA_FALSE := VLAN ID not written (VLAN ID invalid)
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference to system channel
*            instance = affected MRP instance 
*            pVlanId  = pointer to VLAN ID
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT8 mrp_mib_write_vlanid (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, const LSA_UINT32 vlanIn)
{
  LSA_UINT32 vlanId, vlan;
  LSA_UINT8 valid, retVal;
  MRP_MIB_DB_TYPE *pDBmibBase = (MRP_MIB_DB_TYPE *)pChSys->pChSysDB->pMrpMIB;
  MRP_MIB_DB_TYPE *pDBmib;

  pDBmib = &pDBmibBase[instance];

  valid  = LSA_TRUE;
  retVal = LSA_FALSE;

  vlan = MRP_NTOH32(vlanIn);

  vlanId = 0;
  if ((vlan & MRP_PACKET_VAL_VLAN_ID_MRP_MASK) == MRP_PACKET_VAL_VLAN_ID_MRP) /* vlan LT present */
  {
    if ((vlan & MRP_PACKET_VAL_VLAN_ID_TAG_MASK) == MRP_PACKET_VAL_VLAN_ID_TAG_VALID)
    {
      vlanId = vlan & 0x00000FFF;
    }
    else
    {
      valid = LSA_FALSE;
    }
  }

  if (vlanId != pDBmib->domainVlanId)
  {
    pDBmib->domainVlanId = vlanId;

    MRP_PROGRAM_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_mib_write_vlanid[inst=%d]: vlanId=0x%x written", instance, vlanId);

    if (valid)
    {
      mrp_mib_write_domain_error (pChSys, instance, MRP_MIB_DOMAIN_ERR_INVALID_VLANID, LSA_FALSE);
      retVal = LSA_TRUE;
    }
    else
    {
      MRP_PROGRAM_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "mrp_mib_write_vlanid[inst=%d]: invalid vlanId=0x%x", instance, vlanId);
      mrp_mib_write_domain_error (pChSys, instance, MRP_MIB_DOMAIN_ERR_INVALID_VLANID, LSA_TRUE);
    }
  }
  else
  {
    /* vlanID didn't change */
  }

  return retVal;
}

/*===========================================================================
* FUNCTION : mrp_mib_write_domain_uuid
*----------------------------------------------------------------------------
* PURPOSE  : write the current MRP domain UUID to MIB database
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference to system channel
*            instance = affected MRP instance 
*            pDomain  = pointer to domain uuid
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_mib_write_domain_uuid (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT8 *pDomain)
{
  MRP_MIB_DB_TYPE *pDBmibBase = (MRP_MIB_DB_TYPE *)pChSys->pChSysDB->pMrpMIB;
  MRP_MIB_DB_TYPE *pDBmib;

  pDBmib = &pDBmibBase[instance];

  MRP_MEMCPY(pDBmib->domainUUID, pDomain, sizeof(pDBmib->domainUUID));

  MRP_PROGRAM_TRACE_06(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mib_write_domain_uuid[inst=%d]: domain_uuid (%x.%x.%x.%x.%x...) written",
    instance, pDBmib->domainUUID[0], pDBmib->domainUUID[1], pDBmib->domainUUID[2], pDBmib->domainUUID[3], pDBmib->domainUUID[4]);
}

/*===========================================================================
* FUNCTION : mrp_mib_chk_currentResetRoundTripDelayFlag
*----------------------------------------------------------------------------
* PURPOSE  : check, if a resetRoundTripDelays was requested and reset the 
*            current flag
*----------------------------------------------------------------------------
* RETURNS  : LSA_TRUE  := resetRoundTripDelays request currently pending
*            LSA_FALSE := no resetRoundTripDelays request currently pending
*----------------------------------------------------------------------------
* INPUTS   : pChSys     = channel reference
*            instance   = affected MRP instance 
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT8 mrp_mib_chk_currentResetRoundTripDelayFlag (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance)
{
  LSA_UINT8 retVal = LSA_FALSE;
  MRP_MIB_DB_TYPE *pDBmibBase = (MRP_MIB_DB_TYPE *)pChSys->pChSysDB->pMrpMIB;
  MRP_MIB_DB_TYPE *pDBmib;

  pDBmib = &pDBmibBase[instance];

  if (pDBmib->currentResetRoundTripDelayFlag)
  {
    MRP_PROGRAM_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mib_chk_currentResetRTDFlag[inst=%d]: currentRTDFlag=%d",
                         instance, pDBmib->currentResetRoundTripDelayFlag);
    pDBmib->currentResetRoundTripDelayFlag = LSA_FALSE;
    retVal = LSA_TRUE;
  }

  return retVal;
}


/*===========================================================================
* FUNCTION : mrp_mib_reset_round_trip_delays
*----------------------------------------------------------------------------
* PURPOSE  : reset the max and min round trip delay values in mib database
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys     = channel reference
*            instance   = affected MRP instance 
*            resetDelays = round trip delay values have to be reset (1) or not (0)
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_mib_reset_round_trip_delays (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT32 resetDelays)
{
  MRP_MIB_DB_TYPE *pDBmibBase = (MRP_MIB_DB_TYPE *)pChSys->pChSysDB->pMrpMIB;
  MRP_MIB_DB_TYPE *pDBmib;

  pDBmib = &pDBmibBase[instance];

  if (resetDelays)
  {
    pDBmib->domainRoundTripDelayMax = MRP_NO_TIME_AVAILABLE;
    pDBmib->domainRoundTripDelayMin = MRP_NO_TIME_AVAILABLE;
  }
  pDBmib->domainResetRoundTripDelays = resetDelays;
  pDBmib->currentResetRoundTripDelayFlag = resetDelays ? LSA_TRUE : LSA_FALSE;

  MRP_PROGRAM_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mib_reset_round_trip_delays[inst=%d]: resetDelays=%d written", instance, resetDelays);
}

/*===========================================================================
* FUNCTION : mrp_mib_write_round_trip_delays
*----------------------------------------------------------------------------
* PURPOSE  : write the max and min round trip delay values to mib database
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys     = channel reference
*            instance   = affected MRP instance 
*            delayMax   = max round trip delay to be set
*            delayMin   = min round trip delay to be set
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_mib_write_round_trip_delays (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT32 delayMax, LSA_UINT32 delayMin)
{
  MRP_MIB_DB_TYPE *pDBmibBase = (MRP_MIB_DB_TYPE *)pChSys->pChSysDB->pMrpMIB;
  MRP_MIB_DB_TYPE *pDBmib;
  
  pDBmib = &pDBmibBase[instance];

  if (delayMax != pDBmib->domainRoundTripDelayMax)
  {
    pDBmib->domainRoundTripDelayMax = delayMax;
    MRP_PROGRAM_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mib_write_round_trip_delays[inst=%d]: delayMax=%d written", instance, delayMax);
  }
  if (delayMin != pDBmib->domainRoundTripDelayMin)
  {
    pDBmib->domainRoundTripDelayMin = delayMin;
    MRP_PROGRAM_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mib_write_round_trip_delays[inst=%d]: delayMin=%d written", instance, delayMin);
  }
}

/*===========================================================================
* FUNCTION : mrp_mib_write_mrm_prio
*----------------------------------------------------------------------------
* PURPOSE  : write the mrp manager prio to mib database
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference to system channel
*            instance = affected MRP instance 
*            mrmPrio  = MRP manager prio to be set
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/

LSA_VOID mrp_mib_write_mrm_prio (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT16 mrmPrio)
{
  MRP_MIB_DB_TYPE *pDBmibBase = (MRP_MIB_DB_TYPE *)pChSys->pChSysDB->pMrpMIB;
  MRP_MIB_DB_TYPE *pDBmib;

  pDBmib = &pDBmibBase[instance];

  if (mrmPrio != pDBmib->domainManagerPrio)
  {
    pDBmib->domainManagerPrio = mrmPrio;
    MRP_PROGRAM_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mib_write_mrm_prio[inst=%d]: prio=0x%x written", instance, mrmPrio);
  }
}

/*===========================================================================
* FUNCTION : mrp_mib_write_ringopen_cnt
*----------------------------------------------------------------------------
* PURPOSE  : write the MRP MIB counter for ring-state changes to open
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys      = channel reference to system channel
*            instance    = affected MRP instance 
*            ringOpenCnt = ring open counter to be set
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_mib_write_ringopen_cnt (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT16 ringOpenCnt)
{
  MRP_MIB_DB_TYPE *pDBmibBase = (MRP_MIB_DB_TYPE *)pChSys->pChSysDB->pMrpMIB;
  MRP_MIB_DB_TYPE *pDBmib;

  pDBmib = &pDBmibBase[instance];

  pDBmib->domainRingOpenCount = ringOpenCnt;
  MRP_PROGRAM_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mib_write_ringopen_cnt[inst=%d]: cnt=%d written", instance, ringOpenCnt);
}

/*===========================================================================
* FUNCTION : mrp_mib_calc_ringopen_time
*----------------------------------------------------------------------------
* PURPOSE  : calculate time difference, since MRP ring was opened
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys      = channel reference to system channel
*            instance    = affected MRP instance 
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_mib_calc_ringopen_time (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance)
{
  LSA_UINT32 currentTime;
  MRP_MIB_DB_TYPE *pDBmibBase = (MRP_MIB_DB_TYPE *)pChSys->pChSysDB->pMrpMIB;
  MRP_MIB_DB_TYPE *pDBmib;

  pDBmib = &pDBmibBase[instance];
 
  if (pDBmib->ringOpenStartTime != MRP_NO_TIME_AVAILABLE)
  {
    MRP_GET_MS_CNT_VAL(&currentTime);
    pDBmib->domainLastRingOpenChange = mrp_sys_timer_get_time_diff (pDBmib->ringOpenStartTime, currentTime);
    MRP_PROGRAM_TRACE_04(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mib_calc_ringopen_time[inst=%d]: startT=%d, currT=%d -> ringopenT=%d written",
                         instance, pDBmib->ringOpenStartTime, currentTime, pDBmib->domainLastRingOpenChange);
  }
}

/*===========================================================================
* FUNCTION : mrp_mib_write_admin_role
*----------------------------------------------------------------------------
* PURPOSE  : write the mrp admin role to mib database
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys     = channel reference to system channel
*            instance   = affected MRP instance 
*            adminRole  = MRP admin role to be set
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_mib_write_admin_role (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_MIB_ENUM_DOMAIN_ROLE adminRole)
{
  MRP_MIB_DB_TYPE *pDBmibBase = (MRP_MIB_DB_TYPE *)pChSys->pChSysDB->pMrpMIB;
  MRP_MIB_DB_TYPE *pDBmib;

  pDBmib = &pDBmibBase[instance];

  pDBmib->domainAdminRole = adminRole;
  MRP_PROGRAM_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mib_write_admin_role[inst=%d]: adminRole=%d written", instance, adminRole);

  /* special case: (new) device started with default role MRA, afterwards old (Ed.1) provisioning is received */
  /* -> Ed.1 MIB values -> adminRole = operRole = 2 */
  if (pDBmib->domainAdminRole == MRP_MIB_DOMAIN_ROLE_MANAGER && 
      pDBmib->domainOperRole  == MRP_MIB_DOMAIN_ROLE_AUTO_MANAGER_COMP)
    mrp_mib_write_oper_role (pChSys, instance, MRP_MIB_DOMAIN_ROLE_MANAGER);
}

/*===========================================================================
* FUNCTION : mrp_mib_write_oper_role
*----------------------------------------------------------------------------
* PURPOSE  : write the mrp oper role to mib database
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys     = channel reference to system channel
*            instance   = affected MRP instance 
*            adminRole  = MRP admin role to be set
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_mib_write_oper_role (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_MIB_ENUM_DOMAIN_ROLE operRole)
{
  MRP_MIB_DB_TYPE *pDBmibBase = (MRP_MIB_DB_TYPE *)pChSys->pChSysDB->pMrpMIB;
  MRP_MIB_DB_TYPE *pDBmib;

  pDBmib = &pDBmibBase[instance];

  pDBmib->domainOperRole = operRole;
  MRP_PROGRAM_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mib_write_oper_role[inst=%d]: operRole=%d written", instance, operRole);
}

/*===========================================================================
* FUNCTION : mrp_mib_init_mrm_var
*----------------------------------------------------------------------------
* PURPOSE  : initialize mrm related mrp mib database
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys     = channel reference to system channel
*            instance   = affected MRP instance 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_mib_init_mrm_var (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance)
{
  MRP_MIB_DB_TYPE *pDBmibBase = (MRP_MIB_DB_TYPE *)pChSys->pChSysDB->pMrpMIB;
  MRP_MIB_DB_TYPE *pDBmib;

  pDBmib = &pDBmibBase[instance];

  pDBmib->domainLastRingOpenChange = 0;
  pDBmib->ringOpenStartTime = MRP_NO_TIME_AVAILABLE;
  
  mrp_mib_write_ringopen_cnt (pChSys, instance, 0);
  /* reset roundTripDelayMax/roundTripDelayMin values */
  mrp_mib_write_round_trip_delays (pChSys, instance, MRP_NO_TIME_AVAILABLE, MRP_NO_TIME_AVAILABLE);
  (void)(mrp_mib_write_vlanid (pChSys, instance, 0));
  mrp_mib_write_domain_error (pChSys, instance, MRP_MIB_DOMAIN_NO_ERR, LSA_TRUE);
}

/*===========================================================================
* FUNCTION : mrp_mib_init_var
*----------------------------------------------------------------------------
* PURPOSE  : initialize mrp mib database
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : *pDBmib = reference to one MRP MIB instance
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_mib_init_var (MRP_MIB_DB_TYPE *pDBmib)
{
  MRP_MEMSET(pDBmib->domainUUID, 0xFF, sizeof(pDBmib->domainUUID));
  pDBmib->domainName[0]             = 0;
  pDBmib->domainNameLength           = 0;
  
  pDBmib->domainAdminRole            = MRP_MIB_DOMAIN_ROLE_DISABLE;
  pDBmib->domainOperRole             = MRP_MIB_DOMAIN_ROLE_DISABLE;
  
  pDBmib->domainDataRPort1.mrpPortId   = 0;
  pDBmib->domainDataRPort2.mrpPortId   = 0;
  pDBmib->domainDataRPort1.mrpIfIndex  = 0;
  pDBmib->domainDataRPort2.mrpIfIndex  = 0;
  pDBmib->domainDataRPort1.mrpEddState = 0;
  pDBmib->domainDataRPort2.mrpEddState = 0;
  pDBmib->domainDataRPort1.mrpLinkState = 0;
  pDBmib->domainDataRPort2.mrpLinkState = 0;

  pDBmib->domainDataRPort1.mrpMibState = MRP_MIB_RPORT_STATE_UNKNOWN;
  pDBmib->domainDataRPort2.mrpMibState = MRP_MIB_RPORT_STATE_UNKNOWN;

  pDBmib->domainState                = MRP_MIB_DOMAIN_STATE_DISABLED;
  pDBmib->domainError                = MRP_MIB_DOMAIN_NO_ERR;

  pDBmib->domainBlocked              = MRP_MIB_DOMAIN_BLOCKED_ENABLED;                   /* not changeable */
  pDBmib->domainMrmReactOnLinkChange = MRP_MIB_DOMAIN_MRM_REACT_ON_LINK_CHANGE_DISABLED; /* not changeable */
  pDBmib->domainVlanId               = 0;
  pDBmib->domainManagerPrio          = MRP_DEFAULT_VAL_MRM_PRIO;

  pDBmib->domainRingOpenCount        = 0;

  pDBmib->domainLastRingOpenChange   = MRP_NO_TIME_AVAILABLE;
  pDBmib->ringOpenStartTime          = MRP_NO_TIME_AVAILABLE; /* as long as ring was never closed, no start time is available*/
  pDBmib->ringOpenEndTime            = MRP_NO_TIME_AVAILABLE;

  pDBmib->domainResetRoundTripDelays     = 0;
  pDBmib->currentResetRoundTripDelayFlag = LSA_FALSE;
  pDBmib->domainRoundTripDelayMax        = MRP_NO_TIME_AVAILABLE;
  pDBmib->domainRoundTripDelayMin        = MRP_NO_TIME_AVAILABLE;
}



/*===========================================================================
* FUNCTION : mrp_mib_db_init
*----------------------------------------------------------------------------
* PURPOSE  : initialize mrp mib database
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys         = channel reference to system channel
*            mrpMaxInstance = maximum number of MRP instances to be initialized
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_mib_db_init (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpMaxInstance)
{
  LSA_UINT16 i;
  MRP_MIB_DB_TYPE *pDBmibBase = LSA_NULL;
  MRP_MIB_DB_TYPE *pDBmib;

  MRP_PROGRAM_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mib_db_init entered, mrpMaxInstance=%d", mrpMaxInstance);
  
  if (mrpMaxInstance)
  {
    MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBmibBase), (LSA_UINT16)(sizeof(MRP_MIB_DB_TYPE) * mrpMaxInstance));
    MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBmibBase, LSA_NULL));

    pChSys->pChSysDB->pMrpMIB = pDBmibBase;

    for (i = 0; i < mrpMaxInstance; i++)
    {
      pDBmib = &pDBmibBase[i];
      /* preset zero */
      MRP_MEMSET(pDBmib, 0, sizeof(MRP_MIB_DB_TYPE));

      pDBmib->instance = i;
      mrp_mib_init_var (pDBmib);  
    }
  }
}


/*===========================================================================
* FUNCTION : mrp_mib_db_deinit
*----------------------------------------------------------------------------
* PURPOSE  : uninitialize MRP mib database
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys         = channel reference to system channel
*            mrpMaxInstance = maximum number of MRP instances to be initialized
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_mib_db_deinit (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpMaxInstance)
{
  LSA_UINT16  rc;
  MRP_MIB_DB_TYPE *pDBmibBase = (MRP_MIB_DB_TYPE *)pChSys->pChSysDB->pMrpMIB;

  MRP_PROGRAM_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mib_db_deinit entered, mrpMaxInstance=%d", mrpMaxInstance);
  
  if (mrpMaxInstance)
  {
    /* preset zero */
    MRP_MEMSET(pDBmibBase, 0, sizeof(MRP_MIB_DB_TYPE) * mrpMaxInstance);

    MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBmibBase);
    MRP_FATAL1(rc == LSA_RET_OK);
    pChSys->pChSysDB->pMrpMIB = LSA_NULL;
  }

}

/*===========================================================================*/
/*---------------------------------------------------------------------------*/
/*   END of section for MRP-OHA-MIB system channel related functions         */
/*---------------------------------------------------------------------------*/
/*===========================================================================*/
