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
/*  F i l e               &F: mrp_mrc.c                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  implements the pnio client norm state machine for                        */ 
/*  media redundancy protocol (MRP)                                          */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef MRP_MESSAGE
/*  2008-06-06  mk    fdb settings revised for LinkUp delay.                 */
/*  2008-06-11  mk    CheckUUID added.                                       */
/*  2008-06-30  mk    force link up in state de_idle to adapt different      */
/*                    link up times of phys.                                 */
/*  2008-08-05  mk    add port state set for PRM_Port in state pt if PRM_Port*/
/*                    has link down.                                         */
/*  2009-03-19  ds    ringport handling reworked                             */
/*  2009-07-02  ds    include hierarchy changed,                             */
/*                    performance measurement traces introduced              */
/*  2009-10-15  ds    cleanups for LL services (fwdCtrl)                     */
/*                    trace output for received MRP linkchange frame         */
/*  2010-02-16  ds    MRP based on 1 ms (HW) timer instead of 10 ms base     */
/*                    timer constants cleaned up, timer handling corrected   */
/*  2010-07-19  ds    new feature MRP MIB implemented according to           */
/*                    IEC 62439-2                                            */
/*  2010-10-25  ds    bug fixed: DB element CheckUUID must be initialized in */
/*                    mrc_init                                               */
/*  2012-04-04  ds    introduction of N MRP instances per interface          */
/*  2018-10-19  hh    introduction of N MRP interconnection instances        */
/*                                                                           */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID   6
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
/*                                globals                                    */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                            forward declarations                           */
/*---------------------------------------------------------------------------*/


/*===========================================================================
* FUNCTION : mrp_mrc_indicate_mrp_chg_to_mrp_in
*----------------------------------------------------------------------------
* PURPOSE  : indicate MRC state changes to MRP interconnection (MIM)
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBmrm = pointer to one instance of mrm database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_VOID mrp_mrc_indicate_mrp_chg_to_mrp_in (const MRP_MRC_DB_TYPE *pDBmrc)
{
  LSA_UINT16 mrpInMaxInstance, i;

  MRP_PROGRAM_TRACE_02 (pDBmrc->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mrc_indicate_mrp_chg_to_mrp_in[inst=%d]: currentEvent=%d", pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBmrc->CurrentEvent);

  mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pDBmrc->pChSys);

  if (mrpInMaxInstance)
  {
    MRP_FATAL1 (!LSA_HOST_PTR_ARE_EQUAL (pDBmrc->ppMrpInRingChgCbf, LSA_NULL));

    for (i = 0; i < mrpInMaxInstance; i++)
    {
      if (pDBmrc->ppMrpInRingChgCbf[i] != LSA_NULL)
      {
        (pDBmrc->ppMrpInRingChgCbf[i]) (pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], i);
      }
    }
  }
}

/*===========================================================================
* FUNCTION : mrp_mrc_up_timer_start
*----------------------------------------------------------------------------
* PURPOSE  : start up timer
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : *pDBmrc       = internal data base reference
*            durationTicks = time in ticks
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_mrc_up_timer_start (const MRP_MRC_DB_TYPE *pDBmrc)
{
  mrp_sys_timer_stop(pDBmrc->TimerIdxUp);
  mrp_sys_timer_start(pDBmrc->TimerIdxUp, pDBmrc->LNKupTcnt);
}


/*===========================================================================
* FUNCTION : mrp_mrc_up_timer_stop
*----------------------------------------------------------------------------
* PURPOSE  : stop up timer
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : *pDBmrc=internal data base reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_mrc_up_timer_stop (const MRP_MRC_DB_TYPE *pDBmrc)
{
  mrp_sys_timer_stop(pDBmrc->TimerIdxUp);
}


/*===========================================================================
* FUNCTION : mrp_mrc_down_timer_start
*----------------------------------------------------------------------------
* PURPOSE  : start down timer
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : *pDBmrc       = internal data base reference
*            durationTicks = time in ticks
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_mrc_down_timer_start (const MRP_MRC_DB_TYPE *pDBmrc)
{
  mrp_sys_timer_stop(pDBmrc->TimerIdxDown);
  mrp_sys_timer_start(pDBmrc->TimerIdxDown, pDBmrc->LNKdownTcnt);
}


/*===========================================================================
* FUNCTION : mrp_mrc_down_timer_stop
*----------------------------------------------------------------------------
* PURPOSE  : stop down timer
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : *pDBmrc=internal data base reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_mrc_down_timer_stop (const MRP_MRC_DB_TYPE *pDBmrc)
{
  mrp_sys_timer_stop(pDBmrc->TimerIdxDown);
}


/*===========================================================================
* FUNCTION : mrp_mrc_link_change_req
*----------------------------------------------------------------------------
* PURPOSE  : send mautype change indications
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : *pDBmrc       = internal data base reference
*            instance      = affected MRP instance
*            portID        = edd port id (1..MAX_PORTS)
*            linkState     = MRP_LINK_STATE_UP, MRP_LINK_STATE_DOWN
*            durationTicks = time in ticks
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_mrc_link_change_req (MRP_MRC_DB_TYPE *pDBmrc, MRP_LINK_STATE_TYPE linkState)
{
  MRP_PACKET_STRUCT_LINK_CHG_TYPE *pFrame;
  LSA_UINT16 time;
  MRP_EDD_LOWER_RQB_PTR_TYPE pRQB = LSA_NULL;
  LSA_UINT16 instance = pDBmrc->pInstance[MRP_INSTANCE_INDEX]; 
  LSA_UINT16 portID = pDBmrc->PRM_RPort;
  LSA_UINT32 durationTicks;

  MRP_ASSERT((MRP_LINK_STATE_UP == linkState)||(MRP_LINK_STATE_DOWN == linkState));

  if(MRP_LINK_STATE_DOWN == linkState)
  {
    durationTicks = (pDBmrc->LNKNReturn * pDBmrc->LNKdownTcnt);
  }
  else
  {
    durationTicks = (pDBmrc->LNKNReturn * pDBmrc->LNKupTcnt);
  }

  time = (LSA_UINT16)(durationTicks * MRP_TIMER_TIME_BASE_DEZ);

  if (pDBmrc->pChSys->state != MRP_CH_STATE_CLOSING)
    (void)(mrp_edd_get_snd_rqb (pDBmrc->pChSys, pDBmrc->pInstance, MRP_LL_SND_RQB_ENUM_NSM_WRK_LNK_CHG_BOT, &pRQB, MRP_EDD_GET_SND_RQB_NO_THRESHOLD));
  
  if (!LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL)) 
  {
    /* send parameter */
    pFrame = (MRP_PACKET_STRUCT_LINK_CHG_TYPE*)MRP_EDD_LOWER_GET_NRT_TX_BUFFER(pRQB); /*lint !e826 CM 10/11/2016 cast on purpose */
    pFrame->payload.interval = MRP_HTON16(time);
    pFrame->payload.portRole = MRP_HTON16(MRP_PACKET_VAL_PORT_ROLE_PRIMARY);

    if (portID == pDBmrc->SEC_RPort) 
    {
      pFrame->payload.portRole = MRP_HTON16(MRP_PACKET_VAL_PORT_ROLE_SECONDARY);
    }
    pFrame->payload.sequenceID = MRP_HTON16(pDBmrc->pDBifRef->pSequenceID[pDBmrc->pInstance[MRP_INSTANCE_INDEX]]);
    MRP_MEMCPY((LSA_VOID_PTR_TYPE)pFrame->payload.domainUUID, pDBmrc->DomainUUID, sizeof(pFrame->payload.domainUUID));
    
    if (linkState == MRP_LINK_STATE_UP) 
    {
      pFrame->payload.tlv = MRP_HTON16(MRP_PACKET_VAL_TLV_LNK_UP);
    }
    else 
    {
#ifdef MRP_CFG_PERF_TRACE
      {
        MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pDBmrc->pChSys->pChSysDB->pEdd;
        
        if (time == 80) 
        {
          pDBedd->pInstData[instance].perfCapture.endTime = ERTEC_TIME;
          pDBedd->pInstData[instance].perfCapture.timeDiff = pDBedd->pInstData[instance].perfCapture.endTime - pDBedd->pInstData[instance].perfCapture.startTime;
          pDBedd->pInstData[instance].perfCapture.startTime = pDBedd->pInstData[instance].perfCapture.endTime;
          MRP_PROGRAM_TRACE_03(pDBmrc->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_FATAL, "{@@@ T1CE[inst=%d] @@@}[%d] mrp_mrc_link_change_req (time=%d ns)",
                               instance, pDBedd->pInstData[instance].perfCapture.endTime, (pDBedd->pInstData[instance].perfCapture.timeDiff * 10));
        }
      }
#endif
      pFrame->payload.tlv = MRP_HTON16(MRP_PACKET_VAL_TLV_LNK_DWN);
    }

    MRP_PROGRAM_TRACE_05(pDBmrc->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "send mrp_mrc_link_change_req[inst=%d] port=%u, linkUp=%d, time=%u, sequID=%u", 
                         instance, portID, linkState, time, pDBmrc->pDBifRef->pSequenceID[pDBmrc->pInstance[MRP_INSTANCE_INDEX]]);
    mrp_edd_req_snd(pRQB, portID, MRP_PACKET_SIZE_LINK_CHG, LSA_TRUE);

    pDBmrc->pDBifRef->pSequenceID[pDBmrc->pInstance[MRP_INSTANCE_INDEX]]++;
  }
}


/*===========================================================================
* FUNCTION : mrp_mrc_ac_stat1
*----------------------------------------------------------------------------
* PURPOSE  : activate state one for MRC
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : *pDBmrc = reference to one MRC instance
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : Startup, wait for Link Up on one of the ringports.
*==========================================================================*/
LSA_VOID mrp_mrc_ac_stat1 (MRP_MRC_DB_TYPE *pDBmrc)
{

  switch (pDBmrc->CurrentEvent) 
  {
  /*==========================================================================*/
  case MRP_MRC_EVENT_T_MAU_TYPE_CHG:
    if (pDBmrc->LinkState == MRP_LINK_STATE_UP)
    {
      if (pDBmrc->RPort == pDBmrc->PRM_RPort)
      {
        MRP_PROGRAM_TRACE_02(pDBmrc->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrc_ac_stat1=>de_idle[inst=%d] (MAU_TYPE_CHG PRM_RPort=%u UP)", pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBmrc->PRM_RPort);
      }
      else /* pDBmrc->RPort != pDBmrc->PRM_RPort */
      {
        MRP_PROGRAM_TRACE_03(pDBmrc->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrc_ac_stat1=>de_idle[inst=%d] (MAU_TYPE_CHG SEC_RPort=%u(=>%u) UP)", pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBmrc->SEC_RPort,pDBmrc->PRM_RPort);
        pDBmrc->SEC_RPort = pDBmrc->PRM_RPort;
        pDBmrc->PRM_RPort = pDBmrc->RPort;
      }
      mrp_edd_set_port_state (pDBmrc->pChSys, pDBmrc->pInstance, pDBmrc->PRM_RPort, MRP_PORT_STATE_FORWARD);
#ifdef MRP_CFG_DELAYED_LINK_UP
      mrp_ll_fdb_cfg (pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], &pDBmrc->fwdCtrl, pDBmrc->PRM_RPort, 0);
#endif
      mrp_in_ll_fdb_in_ctrl_cfg (pDBmrc->pChSys, pDBmrc->pInstance, pDBmrc->PRM_RPort, 0);
#ifdef MRP_CFG_PLUGIN_2_HSR
      mrp_ard_fdb_cfg (pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBmrc->PRM_RPort, 0);
#endif
      mrp_in_ll_fdb_in_tst_cfg (pDBmrc->pChSys, pDBmrc->pInstance, pDBmrc->PRM_RPort, 0);

      pDBmrc->pStateCbf = mrp_mrc_de_idle;
    }
    else /* pDBmrc->LinkState == MRP_LINK_STATE_DOWN */
    {
    }
    break;
  /*==========================================================================*/
  case MRP_MRC_EVENT_T_TPLG_CHG:
  case MRP_MRC_EVENT_T_NONE:
  case MRP_MRC_EVENT_T_LINK_CHANGE:
  case MRP_MRC_EVENT_T_UP_TIMER:
  case MRP_MRC_EVENT_T_DOWN_TIMER:
  case MRP_MRC_EVENT_T_IN_TX_REQ:
  case MRP_MRC_EVENT_T_IN_FWD_REQ:
    /* ignore */
    /*    return; */
    break;
    /*==========================================================================*/

  default:
    /* other events are ignored */
    break;
  }
}


/*===========================================================================
* FUNCTION : mrp_mrc_de_idle
*----------------------------------------------------------------------------
* PURPOSE  : data equipment idle
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : *pDBmrc = reference to one MRC instance
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : This state shall be reached if only one ringport (primary) 
*            has a link and is set to FORWARD.
*==========================================================================*/
LSA_VOID mrp_mrc_de_idle (MRP_MRC_DB_TYPE *pDBmrc)
{

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBmrc->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  switch (pDBmrc->CurrentEvent) 
  {
  /*==========================================================================*/
  case MRP_MRC_EVENT_T_MAU_TYPE_CHG:
    if (pDBmrc->RPort == pDBmrc->PRM_RPort)
    {
      if (pDBmrc->LinkState == MRP_LINK_STATE_DOWN)
      {
        MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "de_idle=>ac_stat1[inst=%d] (MAU_TYPE_CHG PRM_RPort=%u DOWN)", pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBmrc->PRM_RPort);
        mrp_edd_set_port_state(pDBmrc->pChSys, pDBmrc->pInstance, pDBmrc->PRM_RPort, MRP_PORT_STATE_BLOCKED);
#ifdef MRP_CFG_DELAYED_LINK_UP
        mrp_ll_fdb_cfg (pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], &pDBmrc->fwdCtrl, 0, 0);
#endif
        mrp_in_ll_fdb_in_ctrl_cfg (pDBmrc->pChSys, pDBmrc->pInstance, 0, 0);
#ifdef MRP_CFG_PLUGIN_2_HSR
        mrp_ard_fdb_cfg (pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], 0, 0);
#endif
        mrp_in_ll_fdb_in_tst_cfg (pDBmrc->pChSys, pDBmrc->pInstance, 0, 0);

        pDBmrc->pStateCbf = mrp_mrc_ac_stat1;
      }
      else /* pDBmrc->LinkState == MRP_LINK_STATE_UP */
      {
        /* ignore */
      }
    }
    else /* pDBmrc->RPort != pDBmrc->PRM_RPort */
    {
      if (pDBmrc->LinkState == MRP_LINK_STATE_DOWN)
      {
        /* ignore */
      }
      else /* pDBmrc->LinkState == MRP_LINK_STATE_UP */
      {
        MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "de_idle=>pt[inst=%d] (MAU_TYPE_CHG SEC_RPort=%u UP)", pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBmrc->SEC_RPort);
        pDBmrc->LNKNReturn = pDBmrc->LNKNRmax;
        mrp_mrc_up_timer_start(pDBmrc);
        mrp_mrc_link_change_req(pDBmrc, MRP_LINK_STATE_UP);
#ifdef MRP_CFG_DELAYED_LINK_UP
        mrp_ll_fdb_cfg(pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], &pDBmrc->fwdCtrl, pDBmrc->PRM_RPort, pDBmrc->SEC_RPort);
#endif
        /* mrp_in_ll_fdb_in_ctrl_cfg (pDBmrc->pChSys, pDBmrc->pInstance, pDBmrc->PRM_RPort, pDBmrc->SEC_RPort); */
#ifdef MRP_CFG_PLUGIN_2_HSR
        mrp_ard_fdb_cfg(pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBmrc->PRM_RPort, pDBmrc->SEC_RPort);
#endif
        /* mrp_in_ll_fdb_in_tst_cfg (pDBmrc->pChSys, pDBmrc->pInstance, pDBmrc->PRM_RPort, pDBmrc->SEC_RPort);  */

        pDBmrc->pStateCbf = mrp_mrc_pt;
      }
    }
    break;
  /*==========================================================================*/
  case MRP_MRC_EVENT_T_TPLG_CHG:
    MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_CHAT, "de_idle[inst=%d]: (TPLG_CHG received at port=%u)", pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBmrc->SEC_RPort);
    (void) (mrp_ll_clear_fdb (pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBmrc->RxTimeTcnt));
#ifdef MRP_CFG_DELAYED_LINK_UP
    if (mrp_edd_force_link_up (pDBmrc->pChSys, pDBmrc->SEC_RPort)) 
    {
      MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "de_idle=>pt_idle[inst=%d] (TPLG_CHG and LinkUp running at port=%u)", pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBmrc->SEC_RPort);
      mrp_ll_fdb_cfg(pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], &pDBmrc->fwdCtrl, pDBmrc->PRM_RPort, pDBmrc->SEC_RPort);
      mrp_edd_set_port_state(pDBmrc->pChSys, pDBmrc->pInstance, pDBmrc->SEC_RPort, MRP_PORT_STATE_FORWARD);
#ifdef MRP_CFG_PLUGIN_2_HSR
      mrp_ard_fdb_cfg(pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBmrc->PRM_RPort, pDBmrc->SEC_RPort);
#endif
      mrp_in_ll_fdb_in_ctrl_cfg (pDBmrc->pChSys, pDBmrc->pInstance, pDBmrc->PRM_RPort, pDBmrc->SEC_RPort);
      mrp_in_ll_fdb_in_tst_cfg (pDBmrc->pChSys, pDBmrc->pInstance, pDBmrc->PRM_RPort, pDBmrc->SEC_RPort);

      pDBmrc->pStateCbf = mrp_mrc_pt_idle;
    }
    else
    {
      MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "de_idle=>de_idle[inst=%d] (TPLG_CHG but NO LinkUp running at port=%u)", pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBmrc->SEC_RPort);
    }
#endif
    /*    return; */
    break;

  /*==========================================================================*/
  case MRP_MRC_EVENT_T_IN_TX_REQ:
    MRP_PROGRAM_TRACE_01 (traceIdx, LSA_TRACE_LEVEL_NOTE, "de_idle=>de_idle[inst=%d] (IN_TX_REQ)", pDBmrc->pInstance[MRP_INSTANCE_INDEX]);
    mrp_in_to_mrp_send_req (LSA_TRUE, (LSA_VOID_PTR_TYPE)pDBmrc, LSA_FALSE);
    break;
  /*==========================================================================*/

  case MRP_MRC_EVENT_T_IN_FWD_REQ:
    {
      if (pDBmrc->pMrcInTxInfo->rxPort != pDBmrc->PRM_RPort && pDBmrc->pMrcInTxInfo->rxPort != pDBmrc->SEC_RPort)
      {
        MRP_PROGRAM_TRACE_01 (traceIdx, LSA_TRACE_LEVEL_NOTE, "de_idle=>de_idle[inst=%d] (IN_FWD_REQ)", pDBmrc->pInstance[MRP_INSTANCE_INDEX]);
        mrp_in_to_mrp_send_req (LSA_TRUE, (LSA_VOID_PTR_TYPE)pDBmrc, LSA_TRUE);
      }
    }
    break;

  case MRP_MRC_EVENT_T_NONE:
  case MRP_MRC_EVENT_T_LINK_CHANGE:
  case MRP_MRC_EVENT_T_UP_TIMER:
  case MRP_MRC_EVENT_T_DOWN_TIMER:
    /* ignore */
    break;
  /*==========================================================================*/
  default:
    /* other events are ignored */
    break;
  }
}


/*===========================================================================
* FUNCTION : mrp_mrc_pt
*----------------------------------------------------------------------------
* PURPOSE  : path through
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : *pDBmrc = reference to one MRC instance
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : Temporary state while signaling link changes.
*==========================================================================*/
LSA_VOID mrp_mrc_pt (MRP_MRC_DB_TYPE *pDBmrc)
{
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBmrc->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */
   
  switch (pDBmrc->CurrentEvent) 
  {
  /*==========================================================================*/
  case MRP_MRC_EVENT_T_UP_TIMER:
    if (0 == pDBmrc->LNKNReturn) 
    {
      MRP_PROGRAM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "pt=>pt_idle[inst=%d] (UP_TIMER)", pDBmrc->pInstance[MRP_INSTANCE_INDEX]);
      pDBmrc->LNKNReturn = pDBmrc->LNKNRmax;
      mrp_edd_set_port_state (pDBmrc->pChSys, pDBmrc->pInstance, pDBmrc->SEC_RPort, MRP_PORT_STATE_FORWARD);

      mrp_in_ll_fdb_in_ctrl_cfg (pDBmrc->pChSys, pDBmrc->pInstance, pDBmrc->PRM_RPort, pDBmrc->SEC_RPort);
      mrp_in_ll_fdb_in_tst_cfg (pDBmrc->pChSys, pDBmrc->pInstance, pDBmrc->PRM_RPort, pDBmrc->SEC_RPort);

      pDBmrc->pStateCbf = mrp_mrc_pt_idle;
      return;
    }
    if (0 < pDBmrc->LNKNReturn) 
    {
      pDBmrc->LNKNReturn--;
      mrp_mrc_up_timer_start(pDBmrc);
      mrp_mrc_link_change_req(pDBmrc, MRP_LINK_STATE_UP);
      return;
    }
    break;
  /*==========================================================================*/
  case MRP_MRC_EVENT_T_MAU_TYPE_CHG:
    if (pDBmrc->LinkState == MRP_LINK_STATE_DOWN)
    {
      if (pDBmrc->RPort == pDBmrc->PRM_RPort)
      {
        MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE, "pt=>de[inst=%d] (MAU_TYPE_CHG PRM_RPort=%u(=>%u) DOWN)", pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBmrc->PRM_RPort, pDBmrc->SEC_RPort);
        pDBmrc->LNKNReturn = pDBmrc->LNKNRmax;
        mrp_mrc_up_timer_stop(pDBmrc);
        pDBmrc->PRM_RPort = pDBmrc->SEC_RPort;
        pDBmrc->SEC_RPort = pDBmrc->RPort;
        mrp_edd_set_port_state(pDBmrc->pChSys, pDBmrc->pInstance, pDBmrc->PRM_RPort, MRP_PORT_STATE_FORWARD);
      }
      else /* pDBmrc->RPort != pDBmrc->PRM_RPort */
      {
        MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE,"pt=>de[inst=%d] (MAU_TYPE_CHG SEC_RPort=%u DOWN)", pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBmrc->SEC_RPort);
        pDBmrc->LNKNReturn = pDBmrc->LNKNRmax;
        mrp_mrc_up_timer_stop(pDBmrc);
      }
      mrp_edd_set_port_state(pDBmrc->pChSys, pDBmrc->pInstance, pDBmrc->SEC_RPort, MRP_PORT_STATE_BLOCKED);
      mrp_mrc_down_timer_start(pDBmrc);
      mrp_mrc_link_change_req(pDBmrc, MRP_LINK_STATE_DOWN);
#ifdef MRP_CFG_DELAYED_LINK_UP
      mrp_ll_fdb_cfg(pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], &pDBmrc->fwdCtrl, pDBmrc->PRM_RPort, 0);
#endif
      mrp_in_ll_fdb_in_ctrl_cfg (pDBmrc->pChSys, pDBmrc->pInstance, pDBmrc->PRM_RPort, 0);
#ifdef MRP_CFG_PLUGIN_2_HSR
      mrp_ard_fdb_cfg(pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBmrc->PRM_RPort, 0);
#endif
      mrp_in_ll_fdb_in_tst_cfg (pDBmrc->pChSys, pDBmrc->pInstance, pDBmrc->PRM_RPort, 0);
      pDBmrc->isRingBreakEvt = LSA_TRUE;

      pDBmrc->pStateCbf = mrp_mrc_de;
    }
    else /* pDBmrc->LinkState == MRP_LINK_STATE_UP */
    {
      /* ignore */
    }
    break;
  /*==========================================================================*/
  case MRP_MRC_EVENT_T_TPLG_CHG:
    MRP_PROGRAM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "pt=>pt_idle[inst=%d] (TPLG_CHG)", pDBmrc->pInstance[MRP_INSTANCE_INDEX]);
    pDBmrc->LNKNReturn = pDBmrc->LNKNRmax;
    mrp_mrc_up_timer_stop (pDBmrc);
    mrp_edd_set_port_state (pDBmrc->pChSys, pDBmrc->pInstance, pDBmrc->SEC_RPort, MRP_PORT_STATE_FORWARD);
    (void) (mrp_ll_clear_fdb (pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBmrc->RxTimeTcnt));

    mrp_in_ll_fdb_in_ctrl_cfg (pDBmrc->pChSys, pDBmrc->pInstance, pDBmrc->PRM_RPort, pDBmrc->SEC_RPort);
    mrp_in_ll_fdb_in_tst_cfg (pDBmrc->pChSys, pDBmrc->pInstance, pDBmrc->PRM_RPort, pDBmrc->SEC_RPort);

    pDBmrc->pStateCbf = mrp_mrc_pt_idle;
    /*    return; */
    break;

  /*==========================================================================*/
  case MRP_MRC_EVENT_T_IN_TX_REQ:
    MRP_PROGRAM_TRACE_01 (traceIdx, LSA_TRACE_LEVEL_NOTE, "pt=>pt[inst=%d] (IN_TX_REQ)", pDBmrc->pInstance[MRP_INSTANCE_INDEX]);
    mrp_in_to_mrp_send_req (LSA_TRUE, (LSA_VOID_PTR_TYPE)pDBmrc, LSA_FALSE);
    break;
  /*==========================================================================*/

  case MRP_MRC_EVENT_T_IN_FWD_REQ:
    {
      if (pDBmrc->pMrcInTxInfo->rxPort != pDBmrc->PRM_RPort && pDBmrc->pMrcInTxInfo->rxPort != pDBmrc->SEC_RPort)
      {
        MRP_PROGRAM_TRACE_01 (traceIdx, LSA_TRACE_LEVEL_NOTE, "pt=>pt[inst=%d] (IN_FWD_REQ)", pDBmrc->pInstance[MRP_INSTANCE_INDEX]);
        mrp_in_to_mrp_send_req (LSA_TRUE, (LSA_VOID_PTR_TYPE)pDBmrc, LSA_TRUE);
      }
    }
    break;

  case MRP_MRC_EVENT_T_LINK_CHANGE:
    {
      if (pDBmrc->LinkState == MRP_LINK_STATE_DOWN)
      {
        pDBmrc->isRingBreakEvt = LSA_TRUE;
      }
    }
    break;

  case MRP_MRC_EVENT_T_DOWN_TIMER:
  case MRP_MRC_EVENT_T_NONE:
  /* ignore */
    break;

  default:

    /* other events are ignored */
    break;
  }
}


/*===========================================================================
* FUNCTION : mrp_mrc_de
*----------------------------------------------------------------------------
* PURPOSE  : state data equipment
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : *pDBmrc = reference to one MRC instance
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : Temporary state while signaling link changes.
*==========================================================================*/
LSA_VOID mrp_mrc_de (MRP_MRC_DB_TYPE *pDBmrc)
{
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBmrc->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  switch (pDBmrc->CurrentEvent) 
  {
  /*==========================================================================*/
  case MRP_MRC_EVENT_T_DOWN_TIMER:
    if (pDBmrc->LNKNReturn == 0) 
    {
      MRP_PROGRAM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "de=>de_idle[inst=%d] (DOWN_TIMER)", pDBmrc->pInstance[MRP_INSTANCE_INDEX]);
      pDBmrc->LNKNReturn = pDBmrc->LNKNRmax;
      pDBmrc->pStateCbf = mrp_mrc_de_idle;
      return;
    }
    if (pDBmrc->LNKNReturn > 0) 
    {
      pDBmrc->LNKNReturn--;
      mrp_mrc_down_timer_start(pDBmrc);
      mrp_mrc_link_change_req(pDBmrc, MRP_LINK_STATE_DOWN);
      return;
    }
    break;
  /*==========================================================================*/
  case MRP_MRC_EVENT_T_MAU_TYPE_CHG:
    if (pDBmrc->RPort == pDBmrc->PRM_RPort)
    {
      if (pDBmrc->LinkState == MRP_LINK_STATE_DOWN)
      {
        MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "de=>mrc_ac_stat1[inst=%d] (MAU_TYPE_CHG PRM_RPort=%u DOWN)", pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBmrc->PRM_RPort);
        pDBmrc->LNKNReturn = pDBmrc->LNKNRmax;
        mrp_edd_set_port_state(pDBmrc->pChSys, pDBmrc->pInstance, pDBmrc->PRM_RPort, MRP_PORT_STATE_BLOCKED);
        mrp_mrc_down_timer_stop(pDBmrc);
#ifdef MRP_CFG_DELAYED_LINK_UP
        mrp_ll_fdb_cfg(pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], &pDBmrc->fwdCtrl, 0, 0);
#endif
        mrp_in_ll_fdb_in_ctrl_cfg (pDBmrc->pChSys, pDBmrc->pInstance, 0, 0);
#ifdef MRP_CFG_PLUGIN_2_HSR
        mrp_ard_fdb_cfg(pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], 0, 0);
#endif
        mrp_in_ll_fdb_in_tst_cfg (pDBmrc->pChSys, pDBmrc->pInstance, 0, 0);

        pDBmrc->pStateCbf = mrp_mrc_ac_stat1;
      }
      else /* pDBmrc->LinkState == MRP_LINK_STATE_UP */
      {
        /* ignore */
      }
    }
    else /* pDBmrc->RPort != pDBmrc->PRM_RPort */
    {
      if (pDBmrc->LinkState == MRP_LINK_STATE_DOWN)
      {
        /* ignore */
      }
      else /* pDBmrc->LinkState == MRP_LINK_STATE_UP */
      {
        MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE,"de=>pt[inst=%d] (MAU_TYPE_CHG SEC_RPort=%u UP)", pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBmrc->SEC_RPort);
        pDBmrc->LNKNReturn = pDBmrc->LNKNRmax;
        /* pDBmrc->SEC_RPort=pDBmrc->RPort; */
        mrp_mrc_down_timer_stop (pDBmrc);
        mrp_mrc_up_timer_start (pDBmrc);
        mrp_mrc_link_change_req (pDBmrc, MRP_LINK_STATE_UP);
#ifdef MRP_CFG_DELAYED_LINK_UP
        mrp_ll_fdb_cfg (pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], &pDBmrc->fwdCtrl, pDBmrc->PRM_RPort, pDBmrc->SEC_RPort);
#endif
        /* mrp_in_ll_fdb_in_ctrl_cfg (pDBmrc->pChSys, pDBmrc->pInstance, pDBmrc->PRM_RPort, pDBmrc->SEC_RPort); */
#ifdef MRP_CFG_PLUGIN_2_HSR
        mrp_ard_fdb_cfg (pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBmrc->PRM_RPort, pDBmrc->SEC_RPort);
#endif
        /* mrp_in_ll_fdb_in_tst_cfg (pDBmrc->pChSys, pDBmrc->pInstance, pDBmrc->PRM_RPort, pDBmrc->SEC_RPort);  */

        pDBmrc->pStateCbf = mrp_mrc_pt;
      }
    }
    break;
  /*==========================================================================*/
  case MRP_MRC_EVENT_T_TPLG_CHG:
    MRP_PROGRAM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "de=>de_idle[inst=%d] (TPLG_CHG)", pDBmrc->pInstance[MRP_INSTANCE_INDEX]);
    pDBmrc->LNKNReturn = pDBmrc->LNKNRmax;
    mrp_mrc_down_timer_stop(pDBmrc);
    (void) (mrp_ll_clear_fdb(pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBmrc->RxTimeTcnt));

    pDBmrc->pStateCbf = mrp_mrc_de_idle;
    /*    return; */
    break;

  /*==========================================================================*/

  case MRP_MRC_EVENT_T_IN_TX_REQ:
    MRP_PROGRAM_TRACE_01 (traceIdx, LSA_TRACE_LEVEL_NOTE, "de=>de[inst=%d] (IN_TX_REQ)", pDBmrc->pInstance[MRP_INSTANCE_INDEX]);
    mrp_in_to_mrp_send_req (LSA_TRUE, (LSA_VOID_PTR_TYPE)pDBmrc, LSA_FALSE);
    break;
  /*==========================================================================*/


  case MRP_MRC_EVENT_T_IN_FWD_REQ:
    {
      if (pDBmrc->pMrcInTxInfo->rxPort != pDBmrc->PRM_RPort && pDBmrc->pMrcInTxInfo->rxPort != pDBmrc->SEC_RPort)
      {
        MRP_PROGRAM_TRACE_01 (traceIdx, LSA_TRACE_LEVEL_NOTE, "de=>de[inst=%d] (IN_FWD_REQ)", pDBmrc->pInstance[MRP_INSTANCE_INDEX]);
        mrp_in_to_mrp_send_req (LSA_TRUE, (LSA_VOID_PTR_TYPE)pDBmrc, LSA_TRUE);
      }
    }
    break;

  case MRP_MRC_EVENT_T_LINK_CHANGE:
  case MRP_MRC_EVENT_T_UP_TIMER:
  case MRP_MRC_EVENT_T_NONE:
    break;

  default:
    /* other events are ignored */
    break;
  }
}


/*===========================================================================
* FUNCTION : mrp_mrc_pt_idle
*----------------------------------------------------------------------------
* PURPOSE  : state path through idle
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : *pDBmrc = reference to one MRC instance
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : This state shall be reached if both ringports have a link 
*            and are set to FORWARD.
*==========================================================================*/
LSA_VOID mrp_mrc_pt_idle (MRP_MRC_DB_TYPE *pDBmrc)
{

  switch (pDBmrc->CurrentEvent) 
  {
  /*==========================================================================*/
  case MRP_MRC_EVENT_T_MAU_TYPE_CHG:
    if (pDBmrc->LinkState == MRP_LINK_STATE_DOWN)
    {
      if (pDBmrc->RPort == pDBmrc->PRM_RPort)
      {
        MRP_PROGRAM_TRACE_03(pDBmrc->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE,"pt_idle=>de[inst=%d] (MAU_TYPE_CHG PRM_RPort=%u(=>%u) DOWN)", pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBmrc->PRM_RPort, pDBmrc->SEC_RPort);
        pDBmrc->PRM_RPort = pDBmrc->SEC_RPort;
        pDBmrc->SEC_RPort = pDBmrc->RPort;
      }
      else /* pDBmrc->RPort != pDBmrc->PRM_RPort */
      {
        MRP_PROGRAM_TRACE_02(pDBmrc->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "pt_idle=>de[inst=%d] (MAU_TYPE_CHG SEC_RPort=%u DOWN)", pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBmrc->SEC_RPort);
      }
      pDBmrc->LNKNReturn = pDBmrc->LNKNRmax;
      mrp_edd_set_port_state(pDBmrc->pChSys, pDBmrc->pInstance, pDBmrc->SEC_RPort, MRP_PORT_STATE_BLOCKED);
      mrp_mrc_down_timer_start(pDBmrc);
      mrp_mrc_link_change_req (pDBmrc, MRP_LINK_STATE_DOWN);
#ifdef MRP_CFG_DELAYED_LINK_UP
      mrp_ll_fdb_cfg (pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], &pDBmrc->fwdCtrl, pDBmrc->PRM_RPort, 0);
#endif
      mrp_in_ll_fdb_in_ctrl_cfg (pDBmrc->pChSys, pDBmrc->pInstance, pDBmrc->PRM_RPort, 0);
#ifdef MRP_CFG_PLUGIN_2_HSR
      mrp_ard_fdb_cfg (pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBmrc->PRM_RPort, 0);
#endif
      mrp_in_ll_fdb_in_tst_cfg (pDBmrc->pChSys, pDBmrc->pInstance, pDBmrc->PRM_RPort, 0);
      pDBmrc->isRingBreakEvt = LSA_TRUE;

      pDBmrc->pStateCbf = mrp_mrc_de;
    }
    else /* pDBmrc->LinkState == MRP_LINK_STATE_UP */
    {
      /* ignore */
    }
    break;
  /*==========================================================================*/
  case MRP_MRC_EVENT_T_TPLG_CHG:
    pDBmrc->isRingBreakEvt = mrp_ll_clear_fdb (pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBmrc->RxTimeTcnt);
/*    return; */
    break;
  /*==========================================================================*/
  case MRP_MRC_EVENT_T_IN_TX_REQ:
    MRP_PROGRAM_TRACE_01 (pDBmrc->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "pt_idle=>pt_idle[inst=%d] (IN_TX_REQ)", pDBmrc->pInstance[MRP_INSTANCE_INDEX]);
    mrp_in_to_mrp_send_req (LSA_TRUE, (LSA_VOID_PTR_TYPE)pDBmrc, LSA_FALSE);
    break;
  /*==========================================================================*/


  case MRP_MRC_EVENT_T_IN_FWD_REQ:
    {
      if (pDBmrc->pMrcInTxInfo->rxPort != pDBmrc->PRM_RPort && pDBmrc->pMrcInTxInfo->rxPort != pDBmrc->SEC_RPort)
      {
        MRP_PROGRAM_TRACE_01 (pDBmrc->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "pt_idle=>pt_idle[inst=%d] (IN_FWD_REQ)", pDBmrc->pInstance[MRP_INSTANCE_INDEX]);
        mrp_in_to_mrp_send_req (LSA_TRUE, (LSA_VOID_PTR_TYPE)pDBmrc, LSA_TRUE);
      }
    }
    break;

  case MRP_MRC_EVENT_T_LINK_CHANGE:
    {
      if (pDBmrc->LinkState == MRP_LINK_STATE_DOWN)
      {
        pDBmrc->isRingBreakEvt = LSA_TRUE;
      }
    }
    break;

  case MRP_MRC_EVENT_T_UP_TIMER:
  case MRP_MRC_EVENT_T_DOWN_TIMER:
  case MRP_MRC_EVENT_T_NONE:
  /* ignore */
    break;
  /*==========================================================================*/
  default:
    /* other events are ignored */
    break;
  }
}


/*===========================================================================
* FUNCTION : mrp_mrc_update
*----------------------------------------------------------------------------
* PURPOSE  : update states of one MRC instance
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : *pDBmrc=internal data base reference
*            event=current event type
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : this function is called to signal state changes.
*==========================================================================*/
LSA_VOID mrp_mrc_update (MRP_MRC_DB_TYPE *pDBmrc, MRP_MRC_EVENT_TYPE event)
{
  pDBmrc->CurrentEvent = event;
  
  /* enter state machine */
  pDBmrc->pStateCbf(pDBmrc);

  mrp_mib_write_domain_state (pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], MRP_MIB_DOMAIN_STATE_UNDEFINED);

  /* only in case that MRP interconnection functionality is supported by the MRP device */
  if (pDBmrc->pDBifRef->mrpInSupportedRoleInst0 != EDD_SUPPORTED_MRP_INTERCONN_ROLE_CAP_NONE)
  {
    /* if mrp_mrc_update () is called with an event triggered from MIM->MRC, indication from MRC->MIM MUST not be called (loop!!!) */
    if (event != MRP_MRC_EVENT_T_IN_FWD_REQ && event != MRP_MRC_EVENT_T_IN_TX_REQ)
    {
      /* if an event was received in MRC, which indicates an MRP ring interruption -> inform the MIM about it */
      if (pDBmrc->isRingBreakEvt)
      {
        /* indicate MRP ring interruption to an upper user (MIM) to enforce an additional InPollReq */
        mrp_mrc_indicate_mrp_chg_to_mrp_in (pDBmrc);

        pDBmrc->isRingBreakEvt = LSA_FALSE;
      }
    }
  }
}


/*===========================================================================
* FUNCTION : mrp_mrc_timer_cbf_up
*----------------------------------------------------------------------------
* PURPOSE  : up timer
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys    = channel reference for edd system channel
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            uid       = user identification==timer id
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_mrc_timer_cbf_up (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, LSA_USER_ID_TYPE uid)
{
  MRP_MRC_DB_TYPE *pDBmrc;
  MRP_MRC_DB_TYPE *pDBmrcBase = (MRP_MRC_DB_TYPE *)pChSys->pChSysDB->pMrc;

  LSA_UNUSED_ARG (uid);

  pDBmrc = &pDBmrcBase[pInstance[MRP_INSTANCE_INDEX]];
  mrp_mrc_update(pDBmrc, MRP_MRC_EVENT_T_UP_TIMER);
}


/*===========================================================================
* FUNCTION : mrp_mrc_timer_cbf_down
*----------------------------------------------------------------------------
* PURPOSE  : down timer
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys=channel reference for edd system channel
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            uid=user identification==timer id
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_mrc_timer_cbf_down (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, LSA_USER_ID_TYPE uid)
{
  MRP_MRC_DB_TYPE *pDBmrc;
  MRP_MRC_DB_TYPE *pDBmrcBase = (MRP_MRC_DB_TYPE *)pChSys->pChSysDB->pMrc;

  LSA_UNUSED_ARG (uid);

  pDBmrc = &pDBmrcBase[pInstance[MRP_INSTANCE_INDEX]];
  mrp_mrc_update(pDBmrc, MRP_MRC_EVENT_T_DOWN_TIMER);
}


/*===========================================================================
* FUNCTION : mrp_mrc_rcv
*----------------------------------------------------------------------------
* PURPOSE  : receive functions for mrp packets
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys=channel reference for edd system channel
*            instance      = affected MRP instance
*            *pBuffer=pointer to nrt buffer
*            userDataOffset=offset to payload
*            len = packet length
*            port = receive port 1..numOfPorts
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_UINT8 mrp_mrc_rcv (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT8 *pBuffer, LSA_UINT16 userDataOffset, LSA_UINT16 len, LSA_UINT16 port)
{
  MRP_MRC_DB_TYPE *pDBmrc;
  MRP_MRC_DB_TYPE *pDBmrcBase = (MRP_MRC_DB_TYPE *)pChSys->pChSysDB->pMrc;

  LSA_UNUSED_ARG(port);
  LSA_UNUSED_ARG(len);

  pDBmrc = &pDBmrcBase[instance];
  
  MRP_PROGRAM_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_mrc_rcv[inst=%d] entered", instance);
 
  if (!MRP_MEMCMP(pBuffer,(LSA_VOID_PTR_TYPE)MRP_PACKET_VAL_DST_ADR_NSM_WRK, 6)) 
  {
    MRP_PACKET_STRUCT_HEADER_TYPE *pFrameHdr = (MRP_PACKET_STRUCT_HEADER_TYPE *)pBuffer; /*lint !e826 CM 10/11/2016 cast on purpose */
    /* userDataOffset always points to Byte 18 of MRP frame ->ver */
    LSA_UINT16 *pVersion = (LSA_UINT16*)(pBuffer + userDataOffset); /*lint !e826 CM 10/11/2016 cast on purpose */
    MRP_PACKET_STRUCT_PAYLOAD_TPLG_CHG_TYPE *pFrame = (MRP_PACKET_STRUCT_PAYLOAD_TPLG_CHG_TYPE*)(pBuffer + userDataOffset + 2); /*lint !e826 CM 10/11/2016 cast on purpose */
    MRP_USR_ENUM_TOPO_CTRL topoFlag;
    LSA_UINT16 interval, version, tlv, RxSequenceID;

    if (mrp_mib_write_vlanid (pChSys, instance, pFrameHdr->vlan.vlan_u32)) /* if valid VLAN ID */
    {
      pDBmrc->Vlan = MRP_NTOH32(pFrameHdr->vlan.vlan_u32);

      if (pDBmrc->CheckUUID == MRP_CHECK_UUID_ON)
        topoFlag = MRP_USR_ENUM_TOPO_CTRL_ADD_CHECK;
      else
        topoFlag = MRP_USR_ENUM_TOPO_CTRL_ADD;

      mrp_oha_multi_usr_write_curr_ringstate_data (pChSys, instance, topoFlag);
      mrp_oha_multi_usr_ringstate_ind_done (pChSys, instance);
    }

    version = MRP_NTOH16(*pVersion);
    tlv = MRP_NTOH16(pFrame->tlv);

    if (MRP_PACKET_VAL_VERSION == version) 
    {
      if (MRP_PACKET_VAL_TLV_TPLG_CHG == tlv) 
      {
        /* MRP_MRC_EVENT_T_TPLG_CHG(RxTimeTcnt) */
        interval = MRP_NTOH16(pFrame->interval);
        RxSequenceID = MRP_NTOH16(pFrame->sequenceID);

        LSA_UNUSED_ARG(RxSequenceID);   /* hhr: to please the compiler in case traces are disabled. */

        pDBmrc->RxTimeTcnt  = (LSA_UINT16)(interval/MRP_TIMER_TIME_BASE_DEZ);
  #ifdef MRP_CFG_PERF_TRACE
        {
          MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

          if (interval == 20) 
          {
            pDBedd->pInstData[instance].perfCapture.endTime = ERTEC_TIME;
            pDBedd->pInstData[instance].perfCapture.timeDiff = pDBedd->pInstData[instance].perfCapture.endTime - pDBedd->pInstData[instance].perfCapture.startTime;
            pDBedd->pInstData[instance].perfCapture.startTime = pDBedd->pInstData[instance].perfCapture.endTime;
            MRP_PROGRAM_TRACE_03(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_FATAL, "{@@@ T3CA[inst=%d] @@@}[%d] mrp_mrc_rcv TPLG_CHG (time=%d ns)",
                                 instance, pDBedd->pInstData[instance].perfCapture.endTime, (pDBedd->pInstData[instance].perfCapture.timeDiff * 10));
          }
        }
  #endif
        MRP_PROGRAM_TRACE_09(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mrc_rcv TPLG_CHG port=%u, sequID=%u, time=%u from=%.2x:%.2x:%.2x:%.2x:%.2x:%.2x",
                    port,
                    RxSequenceID,
                    interval,
                    pFrame->sa[0],
                    pFrame->sa[1],
                    pFrame->sa[2],
                    pFrame->sa[3],
                    pFrame->sa[4],
                    pFrame->sa[5]);
        mrp_mrc_update(pDBmrc, MRP_MRC_EVENT_T_TPLG_CHG);
        return LSA_TRUE;
      }
      else
      {
        if (MRP_PACKET_VAL_TLV_LNK_DWN == tlv || MRP_PACKET_VAL_TLV_LNK_UP == tlv)
        {
          MRP_PACKET_STRUCT_PAYLOAD_LINK_CHG_TYPE *pFrameLnk = (MRP_PACKET_STRUCT_PAYLOAD_LINK_CHG_TYPE*)(pBuffer + userDataOffset + 2); /*lint !e826 CM 10/11/2016 cast on purpose */

          interval = MRP_NTOH16(pFrameLnk->interval);
          RxSequenceID = MRP_NTOH16(pFrameLnk->sequenceID);

          LSA_UNUSED_ARG(RxSequenceID);   /* hhr: to please the compiler in case traces are disabled. */

          if (tlv == MRP_PACKET_VAL_TLV_LNK_UP)
          {
            MRP_PROGRAM_TRACE_04 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mrc_rcv MRP_LNK_CHANGE_UP port=%u, sequID=%u, tlv=0x%x, time=%u", port, RxSequenceID, tlv, interval);
            pDBmrc->LinkState = MRP_LINK_STATE_UP;

          }
          else
          {
            MRP_PROGRAM_TRACE_04 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mrc_rcv MRP_LNK_CHANGE_DOWN port=%u, sequID=%u, tlv=0x%x, time=%u", port, RxSequenceID, tlv, interval);
            pDBmrc->LinkState = MRP_LINK_STATE_DOWN;
          }

          LSA_UNUSED_ARG(interval);   /* hhr: to please the compiler in case traces are disabled. */

          mrp_mrc_update (pDBmrc, MRP_MRC_EVENT_T_LINK_CHANGE);
          return LSA_TRUE;
        }
        else
        {
          return LSA_FALSE;
        }
      }
    }
  }
  return LSA_FALSE;
}


/*===========================================================================
* FUNCTION : mrp_mrc_link_chg_cbf
*----------------------------------------------------------------------------
* PURPOSE  : link change handling
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys=channel reference for edd system channel
*            instance = affected MRP instance
*            port=edd port id (1..MAX_PORTS)
*            linkUp=LSA_TRUE:link up; LSA_FALSE=>link down
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_UINT8 mrp_mrc_link_chg_cbf (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT16 port, MRP_LINK_ADD_INFO addInfo, LSA_UINT8 linkUp)
{
  MRP_MRC_DB_TYPE *pDBmrc;
  MRP_MRC_DB_TYPE *pDBmrcBase = (MRP_MRC_DB_TYPE *)pChSys->pChSysDB->pMrc;

  LSA_UNUSED_ARG (addInfo);

  pDBmrc = &pDBmrcBase[instance];

  if (port == pDBmrc->RPort_1 || port == pDBmrc->RPort_2) 
  {
    pDBmrc->RPort = port;
    if (linkUp) 
    {
      MRP_PROGRAM_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "LINK_UP[inst=%d] at port=%u", instance, port);
      pDBmrc->LinkState = MRP_LINK_STATE_UP;
    }
    else 
    {
      MRP_PROGRAM_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "LINK_DOWN[inst=%d] at port=%u", instance, port);
      pDBmrc->LinkState = MRP_LINK_STATE_DOWN;
    }
    mrp_mrc_update(pDBmrc, MRP_MRC_EVENT_T_MAU_TYPE_CHG);
  }
  return LSA_FALSE;
}


/*===========================================================================
* FUNCTION : mrp_mrc_init_snd_rqb
*----------------------------------------------------------------------------
* PURPOSE  : init send RQB 
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys=channel reference for edd system channel
*            mrpMaxInstance = maximum number of MRP instances to be initialized
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_mrc_init_snd_rqb (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpMaxInstance)
{
  MRP_EDD_LOWER_RQB_PTR_TYPE pRQB;
  MRP_PACKET_STRUCT_LINK_CHG_TYPE *pFrame;
  LSA_UINT16 cnt;
  LSA_UINT16 i, k;
  MRP_IF_DB_TYPE *pDBif = (MRP_IF_DB_TYPE *)pChSys->pChSysDB->pInterfacePara;
  LSA_UINT16  pInstance[2];

  pInstance[MRP_IN_INSTANCE_INDEX]   = MRP_IN_INSTANCE_NONE;

  for (i = 0; i < mrpMaxInstance; i++)
  {
    pInstance[MRP_INSTANCE_INDEX]   = i;

    for (k = 0, cnt = mrp_edd_get_snd_rqb_max (pChSys, pInstance, MRP_LL_SND_RQB_ENUM_NSM_WRK_LNK_CHG_BOT); k < cnt; k++) 
    {
      (void)(mrp_edd_get_snd_rqb (pChSys, pInstance, MRP_LL_SND_RQB_ENUM_NSM_WRK_LNK_CHG_BOT, &pRQB, MRP_EDD_GET_SND_RQB_NO_THRESHOLD));

      MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL));
      /* send parameters */
      pFrame  = (MRP_PACKET_STRUCT_LINK_CHG_TYPE*)MRP_EDD_LOWER_GET_NRT_TX_BUFFER(pRQB); /*lint !e826 CM 10/11/2016 cast on purpose */
      MRP_MEMCPY((LSA_VOID_PTR_TYPE)pFrame, (LSA_VOID_PTR_TYPE)MRP_PACKET_VAL_DEFAULT, sizeof(MRP_PACKET_VAL_DEFAULT) - 1);
      MRP_MEMCPY((LSA_VOID_PTR_TYPE)pFrame->hdr.dst, (LSA_VOID_PTR_TYPE)MRP_PACKET_VAL_DST_ADR_NSM_WRK, sizeof(pFrame->hdr.dst));
      pFrame->payload.tlv = MRP_HTON16(MRP_PACKET_VAL_TLV_TPLG_CHG);
      MRP_MEMCPY((LSA_VOID_PTR_TYPE)pFrame->payload.sa, pDBif->macAddr, sizeof(pFrame->payload.sa));
      pFrame->payload.blocked = MRP_HTON16(0x0001);
      pFrame->payload.tlv_common = MRP_HTON16(MRP_PACKET_VAL_TLV_COMMON);
      MRP_MEMSET((LSA_VOID_PTR_TYPE)pFrame->payload.domainUUID, 0xFF, sizeof(pFrame->payload.domainUUID));

      mrp_edd_set_snd_rqb (pInstance, MRP_LL_SND_RQB_ENUM_NSM_WRK_LNK_CHG_BOT, pRQB);
    }  
  }
}


/*===========================================================================
* FUNCTION : mrp_mrc_init_var
*----------------------------------------------------------------------------
* PURPOSE  : initialize variables of one MRC instance
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : *pDBmrc     = internal data base reference of one MRC instance
*            *pParams=mrc parameter reference
*            mrcByVoting = MRM to MRC fallback because of auto manager voting
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_mrc_init_var (MRP_MRC_DB_TYPE *pDBmrc, MRP_MRC_PARAMS_TYPE *pParams, LSA_UINT8 mrcByVoting)
{
  LSA_UINT16 maxInInstances, i;
  MRP_EDD_DB_TYPE *pDBedd;

  pDBedd = (MRP_EDD_DB_TYPE *)pDBmrc->pChSys->pChSysDB->pEdd;

  MRP_PROGRAM_TRACE_01(pDBmrc->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_mrc_init_var[inst=%d]", pDBmrc->pInstance[MRP_INSTANCE_INDEX]);

  maxInInstances = mrp_in_sys_get_num_of_in_instances (pDBmrc->pChSys);

  /* variables */
  MRP_MEMSET(pDBmrc->DomainUUID, 0xFF, sizeof(pDBmrc->DomainUUID));

  pDBmrc->RPort_1    = pParams->RPort_1;
  pDBmrc->RPort_2    = pParams->RPort_2;
  pDBmrc->PRM_RPort  = pParams->PRM_RPort;    
  pDBmrc->SEC_RPort  = pParams->SEC_RPort;
  pDBmrc->Vlan       = 0;
  pDBmrc->LNKNReturn = 0;   
  pDBmrc->pMrcInTxInfo = pParams->pMrcInTxInfo;

  pDBmrc->fwdCtrl.pTestRqb     = pDBedd->pInstData[pDBmrc->pInstance[MRP_INSTANCE_INDEX]].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_TST];
  pDBmrc->fwdCtrl.pWorkRqb     = pDBedd->pInstData[pDBmrc->pInstance[MRP_INSTANCE_INDEX]].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_WRK];
  pDBmrc->fwdCtrl.pTrfRqb      = 0;
  pDBmrc->fwdCtrl.macAddrPrio = EDD_MULTICAST_FWD_PRIO_UNCHANGED;
  pDBmrc->fwdCtrl.pLLreqType   = g_MrpllReqTbl_mrp_set_fdb;

  /* internals */
  pDBmrc->CurrentEvent = MRP_MRC_EVENT_T_NONE; 
  pDBmrc->pStateCbf     = mrp_mrc_ac_stat1;     

  /* event attributes */
  pDBmrc->RPort      = 0;        
  pDBmrc->LinkState  = MRP_LINK_STATE_DOWN;    
  pDBmrc->RxTimeTcnt = 0;       

  /* parameters */
  pDBmrc->LNKNRmax   = MRP_DEFAULT_VAL_MRC_LNK_NR_MAX;
  pDBmrc->LNKupTcnt  = MRP_DEFAULT_VAL_MRC_LNK_UP_T_MS/MRP_TIMER_TIME_BASE_DEZ;
  pDBmrc->LNKdownTcnt= MRP_DEFAULT_VAL_MRC_LNK_DOWN_T_MS/MRP_TIMER_TIME_BASE_DEZ;
  pDBmrc->CheckUUID  = pParams->CheckUUID;
  /* timer 
  pDBmrc->TimerIdxUp;
  pDBmrc->TimerIdxDown;
  */

  for (i = 0; i < maxInInstances; i++)
  {
    pDBmrc->ppMrpInRingChgCbf[i] = pParams->ppMrpInRingChgCbf[i];
  }

  if (pParams->SetDefaults == MRP_SET_DEFAULTS_OFF)
  {
    /* set new values */
    MRP_MEMCPY(pDBmrc->DomainUUID, pParams->DomainUUID, sizeof(pDBmrc->DomainUUID));
    pDBmrc->LNKdownTcnt = (LSA_UINT16)(pParams->LNKdownT_ms/MRP_TIMER_TIME_BASE_DEZ);
    pDBmrc->LNKupTcnt   = (LSA_UINT16)(pParams->LNKupT_ms/MRP_TIMER_TIME_BASE_DEZ);
    pDBmrc->LNKNRmax    = pParams->LNKNRmax;
  }
  (void)(mrp_mib_write_vlanid (pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], 0));
  mrp_mib_write_domain_error (pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], MRP_MIB_DOMAIN_NO_ERR, LSA_TRUE);
  
  if (!mrcByVoting)
    mrp_mib_write_domain_state (pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], MRP_MIB_DOMAIN_STATE_DISABLED);

  if (!pParams->RPort_1 && !pParams->RPort_2) /* if called during DEINIT of MRC */
  {
    mrp_mib_write_rports(pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBedd->pInstData[pDBmrc->pInstance[MRP_INSTANCE_INDEX]].RPort_1, pDBedd->pInstData[pDBmrc->pInstance[MRP_INSTANCE_INDEX]].RPort_2);
    if (pDBedd->pInstData[pDBmrc->pInstance[MRP_INSTANCE_INDEX]].RPort_1)
      mrp_mib_write_link_state (pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBedd->pInstData[pDBmrc->pInstance[MRP_INSTANCE_INDEX]].RPort_1, pDBedd->pLinkData[pDBedd->pInstData[pDBmrc->pInstance[MRP_INSTANCE_INDEX]].RPort_1 - 1].currentState);
    if (pDBedd->pInstData[pDBmrc->pInstance[MRP_INSTANCE_INDEX]].RPort_2)
      mrp_mib_write_link_state (pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], pDBedd->pInstData[pDBmrc->pInstance[MRP_INSTANCE_INDEX]].RPort_2, pDBedd->pLinkData[pDBedd->pInstData[pDBmrc->pInstance[MRP_INSTANCE_INDEX]].RPort_2 - 1].currentState);
  }
  else /* if called during INIT of MRC */
  {
    mrp_mib_write_rports(pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], pParams->RPort_1, pParams->RPort_2);
    if (!mrcByVoting)
    {
      if (pParams->RPort_1)
        mrp_mib_write_link_state (pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], pParams->RPort_1, pDBedd->pLinkData[pParams->RPort_1 - 1].currentState);
      if (pParams->RPort_2)
        mrp_mib_write_link_state (pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], pParams->RPort_2, pDBedd->pLinkData[pParams->RPort_2 - 1].currentState);
    }
  }
  
  if (!mrcByVoting)
    mrp_oha_multi_usr_write_curr_ringstate_data (pDBmrc->pChSys, pDBmrc->pInstance[MRP_INSTANCE_INDEX], MRP_USR_ENUM_TOPO_CTRL_NONE);
}


/*===========================================================================
* FUNCTION : mrp_mrc_get_params
*----------------------------------------------------------------------------
* PURPOSE  : return mrc params
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys=channel reference for edd system channel
*            instance = affected MRC instance
*            *pParams=mrc parameter reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_mrc_get_params (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_MRC_PARAMS_TYPE *pParams)
{
  MRP_MRC_DB_TYPE *pDBmrcBase = (MRP_MRC_DB_TYPE *)pChSys->pChSysDB->pMrc;
  MRP_MRC_DB_TYPE *pDBmrc;
  
  pDBmrc = &pDBmrcBase[instance];

  pParams->RPort_1         = pDBmrc->RPort_1;
  pParams->RPort_2         = pDBmrc->RPort_2;
  pParams->PRM_RPort       = pDBmrc->PRM_RPort;
  pParams->SEC_RPort       = pDBmrc->SEC_RPort;
  pParams->Mode            = pDBmrc->runState;
  pParams->SetDefaults     = MRP_SET_DEFAULTS_OFF;
  
  MRP_PROGRAM_TRACE_02 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_mrc_get_params[inst=%d]: mode=%d retrieved", instance, pParams->Mode);

  MRP_MEMCPY(pParams->DomainUUID, pDBmrc->DomainUUID, sizeof(pParams->DomainUUID));
  
  pParams->LNKdownT_ms     =(LSA_UINT16)(pDBmrc->LNKdownTcnt * MRP_TIMER_TIME_BASE_DEZ);
  pParams->LNKupT_ms       =(LSA_UINT16)(pDBmrc->LNKupTcnt * MRP_TIMER_TIME_BASE_DEZ);
  pParams->LNKNRmax        = pDBmrc->LNKNRmax;
  pParams->CheckUUID       = pDBmrc->CheckUUID;
}

/*===========================================================================
* FUNCTION : mrp_mrc_get_module_state
*----------------------------------------------------------------------------
* PURPOSE  : get all important module params to reflect module state of MRC
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChUsr       = user channel reference
*            instance     = affected MRP/HSR instance
* OUTPUTS  : *moduleState = module state data of this instance
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_mrc_get_module_state (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance, MRP_REC_MODULE_STATE_TYPE *pModuleState)
{  
  MRP_MRC_PARAMS_TYPE paramsMRCRead;
  MRP_REC_DB_INST_SET_TYPE *pRecInstWork;
  MRP_REC_DB_TYPE *pDBrec = (MRP_REC_DB_TYPE *)pChUsr->pChUsrDB->pRec;

  mrp_mrc_get_params (pDBrec->pChSys, instance, &paramsMRCRead);  
  
  pRecInstWork = &pDBrec->pInstData[instance].work;
  
  pModuleState->topoCtrl    = MRP_USR_ENUM_TOPO_CTRL_NONE; 
  pModuleState->configured  = (pRecInstWork->RecInstanceAdjust.Role == MRP_PROJ_ROLE_CLIENT);
  
  if (pModuleState->configured) 
  {
    if ((pRecInstWork->RecInstanceUuidCheck.Check & MRP_REC_DATA_CHECK_UUID) && (paramsMRCRead.CheckUUID == MRP_CHECK_UUID_OFF))
    { 
      pModuleState->topoCtrl = MRP_USR_ENUM_TOPO_CTRL_ADD_CHECK; 
    }
    if (!(pRecInstWork->RecInstanceUuidCheck.Check & MRP_REC_DATA_CHECK_UUID) && (paramsMRCRead.CheckUUID == MRP_CHECK_UUID_ON))
    { 
      pModuleState->topoCtrl = MRP_USR_ENUM_TOPO_CTRL_ADD; 
    }
  }
  pModuleState->rpChanged = ( !((pRecInstWork->RPort_1 == paramsMRCRead.RPort_1 && pRecInstWork->RPort_2 == paramsMRCRead.RPort_2) ||
                                (pRecInstWork->RPort_1 == paramsMRCRead.RPort_2 && pRecInstWork->RPort_2 == paramsMRCRead.RPort_1))
                            );
  pModuleState->paramChanged = ( pModuleState->rpChanged
                                 || (MRP_MEMCMP(paramsMRCRead.DomainUUID, pRecInstWork->RecInstanceAdjust.DomainUUID, sizeof(paramsMRCRead.DomainUUID)))
                                 || (pRecInstWork->RecInstanceAdjust.client.LNKdownT_ms != paramsMRCRead.LNKdownT_ms)
                                 || (pRecInstWork->RecInstanceAdjust.client.LNKupT_ms != paramsMRCRead.LNKupT_ms)
                                 || (pRecInstWork->RecInstanceAdjust.client.LNKNRmax != paramsMRCRead.LNKNRmax));
  
  pModuleState->running     = (paramsMRCRead.Mode == MRP_OPERATION_ON);

  pModuleState->roleChangeRestricted = LSA_FALSE;

  /*  stop := run && !configured || restart */
  pModuleState->stop  = ((pModuleState->running && !pModuleState->configured) || (pModuleState->running && pModuleState->paramChanged));
  /*  start := !run && configured || restart */
  pModuleState->start = ((!pModuleState->running && pModuleState->configured) || (pModuleState->configured && pModuleState->running && pModuleState->paramChanged));
}


/*===========================================================================
* FUNCTION : mrp_mrc_stop
*----------------------------------------------------------------------------
* PURPOSE  : stop one MRC instance
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys=channel reference for edd system channel
*            instance = (MRP) instance to be started
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_mrc_stop (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance)
{
  MRP_MRC_PARAMS_TYPE params;
  LSA_UINT8 mrpInPossible;
  LSA_UINT16 maxInInstances;
  LSA_UINT16 i;
  MRP_MRC_DB_TYPE *pDBmrc;
  MRP_MRC_DB_TYPE *pDBmrcBase = (MRP_MRC_DB_TYPE *)pChSys->pChSysDB->pMrc;

  pDBmrc         = &pDBmrcBase[instance];
  maxInInstances = mrp_in_sys_get_num_of_in_instances (pChSys);
  mrpInPossible  = LSA_FALSE;

  MRP_PROGRAM_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mrc_stop[inst=%d] entered", instance);
  
  if (!instance &&  maxInInstances)
  {
    mrpInPossible = LSA_TRUE;
    mrp_usr_write_red_start_state (pChSys, instance, MRP_USR_ENUM_STATE_IN, pDBmrc->RPort_1, pDBmrc->RPort_2, LSA_NULL, LSA_FALSE);
  }
  mrp_usr_write_red_start_state (pChSys, instance, MRP_USR_ENUM_STATE_MRC, pDBmrc->RPort_1, pDBmrc->RPort_2, LSA_NULL, LSA_FALSE);
  mrp_ll_red_state_handler (pChSys, instance);


  if (mrpInPossible)
  {
    for (i = 0; i < maxInInstances; i++)
    {
      mrp_in_base_stop (pChSys, i);
    }
  }

  pDBmrc->runState = MRP_OPERATION_OFF;
  mrp_sys_timer_stop(pDBmrc->TimerIdxUp);
  mrp_sys_timer_stop(pDBmrc->TimerIdxDown);

  mrp_mib_write_oper_role (pChSys, instance, MRP_MIB_DOMAIN_ROLE_DISABLE);
  mrp_ll_deinit_modul_mrc(pChSys, instance);

  /* topo ctrl */
  mrp_oha_multi_usr_write_curr_ringstate_data (pChSys, instance, MRP_USR_ENUM_TOPO_CTRL_NONE);
  mrp_oha_multi_usr_ringstate_ind_done (pChSys, instance);

  mrp_edd_remove_link_chg_callback(pChSys, instance, mrp_mrc_link_chg_cbf);
  mrp_edd_remove_rcv_callback(pChSys, instance, mrp_mrc_rcv);

  params.RPort_1 = params.RPort_2 = 0;
  params.PRM_RPort = params.SEC_RPort = 0;
  params.SetDefaults = MRP_SET_DEFAULTS_ON;

  params.ppMrpInRingChgCbf = pDBmrc->ppMrpInRingChgCbf;

  mrp_mrc_init_var(pDBmrc, &params, LSA_FALSE);

  mrp_ll_event_handler(pChSys, instance);

  MRP_PROGRAM_TRACE_01 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mrc_stop[inst=%d] exit", instance);
}


/*===========================================================================
* FUNCTION : mrp_mrc_start
*----------------------------------------------------------------------------
* PURPOSE  : start one MRC instance
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys=channel reference for edd system channel
*            instance = (MRP) instance to be started
*            *pParams=mrc parameter reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_mrc_start (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_MRC_PARAMS_TYPE *pParams)
{
  MRP_USR_ENUM_TOPO_CTRL topoFlag;
  LSA_UINT16 maxInInstances, i;
  LSA_UINT8 mrpInPossible;
  MRP_MRC_DB_TYPE *pDBmrc;
  MRP_MRC_DB_TYPE *pDBmrcBase = (MRP_MRC_DB_TYPE *)pChSys->pChSysDB->pMrc;

  pDBmrc = &pDBmrcBase[instance];

  pDBmrc->runState = MRP_OPERATION_ON;
  mrpInPossible    = LSA_FALSE;
  maxInInstances   = mrp_in_sys_get_num_of_in_instances (pChSys);

  MRP_PROGRAM_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mrc_start[inst=%d] entered", instance);

  mrp_usr_write_red_start_state (pChSys, instance, MRP_USR_ENUM_STATE_MRC, pParams->RPort_1, pParams->RPort_2, LSA_NULL, LSA_TRUE);
  
  if (!instance &&  maxInInstances)
  {
    mrpInPossible = LSA_TRUE;
    mrp_usr_write_red_start_state (pChSys, instance, MRP_USR_ENUM_STATE_IN, pParams->RPort_1, pParams->RPort_2, LSA_NULL, LSA_TRUE);
  }
  mrp_ll_red_state_handler (pChSys, instance);

  pParams->ppMrpInRingChgCbf = pDBmrc->ppMrpInRingChgCbf;

  mrp_mrc_init_var(pDBmrc, pParams, LSA_FALSE);
  mrp_edd_set_rcv_callback(pChSys, instance, mrp_mrc_rcv);
  mrp_edd_set_link_chg_callback(pChSys, instance, mrp_mrc_link_chg_cbf);
  
  mrp_mib_write_oper_role (pChSys, instance, MRP_MIB_DOMAIN_ROLE_CLIENT);
  mrp_ll_init_modul_mrc(pChSys, instance, pParams);

  /* topo ctrl */
  if (pDBmrc->CheckUUID == MRP_CHECK_UUID_ON)
    topoFlag = MRP_USR_ENUM_TOPO_CTRL_ADD_CHECK;
  else
    topoFlag = MRP_USR_ENUM_TOPO_CTRL_ADD;

  mrp_oha_multi_usr_write_curr_ringstate_data (pChSys, instance, topoFlag);
  mrp_ll_event_handler (pChSys, instance);

  if (mrpInPossible)
  {
    for (i = 0; i < maxInInstances; i++)
    {
      mrp_in_base_start (pChSys, i, LSA_FALSE);
    }
  }

  MRP_PROGRAM_TRACE_01 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mrc_start[inst=%d] exit", instance);
}


/*===========================================================================
* FUNCTION : mrp_mrc_init
*----------------------------------------------------------------------------
* PURPOSE  : set nsm to start
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys           = channel reference for edd system channel
*            mrpMaxInstance   = maximum number of MRP instances to be initialized
*            mrpInMaxInstance = maximum number of MRP interconnection instances to be initialized
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_mrc_init (const MRP_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance)
{
  LSA_UINT16 i, k;
  LSA_UINT32 timerIdx = 0;
  MRP_MRC_PARAMS_TYPE params;
  MRP_MRC_DB_TYPE *pDBmrcBase = LSA_NULL;
  MRP_MRC_DB_TYPE *pDBmrc;
  MRP_MIB_DB_TYPE *pDBmibBase = (MRP_MIB_DB_TYPE *)pChSys->pChSysDB->pMrpMIB;

  MRP_PROGRAM_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mrc_init entered, mrpMaxInstance=%d", mrpMaxInstance);
  
  if (mrpMaxInstance)
  {
    MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBmrcBase), (LSA_UINT16)(sizeof(MRP_MRC_DB_TYPE) * mrpMaxInstance));
    MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBmrcBase, LSA_NULL));
    pChSys->pChSysDB->pMrc = pDBmrcBase;

    pDBmibBase = (MRP_MIB_DB_TYPE *)pChSys->pChSysDB->pMrpMIB;

    for (i = 0; i < mrpMaxInstance; i++)
    {
      pDBmrc = &pDBmrcBase[i];

      /* preset zero */
      MRP_MEMSET(pDBmrc, 0, sizeof(MRP_MRC_DB_TYPE));

      pDBmrc->pChSys   = pChSys;
      pDBmrc->pDBifRef = (MRP_IF_DB_TYPE *)pChSys->pChSysDB->pInterfacePara;
      pDBmrc->pDBmib   = &pDBmibBase[i];

      pDBmrc->pInstance[MRP_INSTANCE_INDEX]     = i;
      pDBmrc->pInstance[MRP_IN_INSTANCE_INDEX]  = MRP_IN_INSTANCE_NONE;

      pDBmrc->runState = MRP_OPERATION_OFF;
      pDBmrc->CheckUUID = MRP_CHECK_UUID_OFF;

      params.RPort_1 = params.RPort_2 = 0;
      params.PRM_RPort = params.SEC_RPort = 0;
      params.CheckUUID = MRP_CHECK_UUID_OFF;
      params.SetDefaults = MRP_SET_DEFAULTS_ON;

      if (mrpInMaxInstance)
      {
        /* allocate an array of CBF function pointers and store the pointer to these array of pointers in ppMrpInRingChgCbf */
        MRP_ALLOC_LOCAL_MEM ((MRP_LOCAL_MEM_PTR_TYPE*)&pDBmrc->ppMrpInRingChgCbf, (LSA_UINT16)(sizeof (MRP_IN_MIM_MRP_RING_CHG_FCT_PTR_TYPE) * mrpInMaxInstance));
        MRP_FATAL1 (!LSA_HOST_PTR_ARE_EQUAL (pDBmrc->ppMrpInRingChgCbf, LSA_NULL));

        for (k = 0; k < mrpInMaxInstance; k++)
        {
          pDBmrc->ppMrpInRingChgCbf[k] = LSA_NULL;
        }
      }
      else
        pDBmrc->ppMrpInRingChgCbf = LSA_NULL;

      params.ppMrpInRingChgCbf = pDBmrc->ppMrpInRingChgCbf;

      mrp_mrc_init_var(pDBmrc, &params, LSA_FALSE);  
    }
  
    for (i = 0; i < mrpMaxInstance; i++)
    {
      pDBmrc = &pDBmrcBase[i];
    
      pDBmrc->TimerIdxUp = mrp_sys_timer_get (pChSys, MRP_TIMER_POOL_INSTANCE, i, LSA_TIMER_TYPE_ONE_SHOT, MRP_TIMER_ENUM_MRP_LINKUP_MRC, (MRP_TIMER_CBF_FCT_PTR_TYPE)mrp_mrc_timer_cbf_up);
      timerIdx = pDBmrc->TimerIdxUp % MRP_MAX_NO_OF_TIMER_IDS_PER_INTERFACE;
      MRP_PROGRAM_TRACE_03(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mrc_init[inst=%d]: timer_get called for TimerIdxUp: id=%d, idx=%d",
        i, pDBmrc->TimerIdxUp, timerIdx);
    }

    for (i = 0; i < mrpMaxInstance; i++)
    {
      pDBmrc = &pDBmrcBase[i];
    
      pDBmrc->TimerIdxDown = mrp_sys_timer_get (pChSys, MRP_TIMER_POOL_INSTANCE, i, LSA_TIMER_TYPE_ONE_SHOT, MRP_TIMER_ENUM_MRP_LINKDOWN_MRC, (MRP_TIMER_CBF_FCT_PTR_TYPE)mrp_mrc_timer_cbf_down);
      timerIdx = pDBmrc->TimerIdxDown % MRP_MAX_NO_OF_TIMER_IDS_PER_INTERFACE;
      MRP_PROGRAM_TRACE_03(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mrc_init[inst=%d]: timer_get called for TimerIdxDown: id=%d, idx=%d",
        i, pDBmrc->TimerIdxDown, timerIdx);
    }

    LSA_UNUSED_ARG(timerIdx);   /* hhr: to please the compiler in case traces are disabled. */
  }

}


/*===========================================================================
* FUNCTION : mrp_mrc_deinit
*----------------------------------------------------------------------------
* PURPOSE  : uninitialize mrc
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys=channel reference for edd system channel
*            mrpMaxInstance = maximum number of MRP instances to be initialized
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_mrc_deinit (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance)
{
  LSA_UINT16  rc, i;
  MRP_MRC_DB_TYPE *pDBmrc;
  MRP_MRC_DB_TYPE *pDBmrcBase = (MRP_MRC_DB_TYPE *)pChSys->pChSysDB->pMrc;
  
  
  for (i = 0; i < mrpMaxInstance; i++)
  {
    pDBmrc = &pDBmrcBase[i];

    MRP_PROGRAM_TRACE_03 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_mrc_deinit[inst=%d]: mrpMaxInstance=%d, mrpInMaxInstance=%d", i, mrpMaxInstance, mrpInMaxInstance);

    if (mrpInMaxInstance)
    {
      MRP_FREE_LOCAL_MEM (&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBmrc->ppMrpInRingChgCbf);
    }

    mrp_sys_timer_free(pDBmrc->TimerIdxDown);
    mrp_sys_timer_free(pDBmrc->TimerIdxUp);
  }

  if (mrpMaxInstance)
  {
    MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBmrcBase);
    MRP_FATAL1(rc == LSA_RET_OK);
  }
  pChSys->pChSysDB->pMrc = LSA_NULL;

}
