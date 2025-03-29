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
/*  F i l e               &F: mrp_inmim.c                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Functionality for MRP interconnection manager (MIM)                      */ 
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef MRP_MESSAGE

/*  2017-11-15  ds    initial version of file introduced                     */
/*  2017-12-05  ds    initial MIM implementation completed                   */
/*  2018-10-19  hh    introduction of N MRP interconnection instances        */
/*                                                                           */
/*                                                                           */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID   43
#define MRP_MODULE_ID      LTRC_ACT_MODUL_ID

#include "mrp_int.h"

MRP_FILE_SYSTEM_EXTENSION (MRP_MODULE_ID)



/*---------------------------------------------------------------------------*/
/*                                 defines                                   */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                                 types                                     */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                                globals                                    */
/*---------------------------------------------------------------------------*/
const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_in_init_mim[] =
{
  /* first and last entry must be LSA_NULL (dummy) !!! */
  /* cbf                                     , rqbNum                                  , service  */
  { LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END            , 0x0000 }
  ,{ mrp_edd_req_port_state                  , MRP_LL_SREQ_RQB_ENUM_SET_PS_SHADOW      , EDD_SRV_SWITCH_SET_PORT_STATE }
  ,{ mrp_edd_req_port_state_done             , MRP_LL_SREQ_RQB_ENUM_SET_PS_SHADOW      , EDD_SRV_SWITCH_SET_PORT_STATE }
  ,{ mrp_edd_req_in_link_retrigger           , MRP_LL_SREQ_RQB_ENUM_TBL_END            , 0x0000 }
  ,{ LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END            , 0x0000 }
};

/* deinit: NOT necessary */
/*---------------------------------------------------------------------------*/
/*                            forward declarations                           */
/*---------------------------------------------------------------------------*/




/*===========================================================================
* FUNCTION : mrp_in_ll_init_modul_mim
*----------------------------------------------------------------------------
* PURPOSE  : initialize EDD settings of one MIM instance
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pDBin       = pointer to MRP interconnection database instance
*            pParams     = edd port id (1..MAX_PORTS) for MRP interconnection
*                          port
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_VOID mrp_in_ll_init_modul_mim (const MRP_IN_DB_TYPE *pDBin, const MRP_IN_MIM_PARAMS_TYPE *pParams)
{
  MRP_EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE pPSParam;
  MRP_EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE pPSParamTplgChgStart;
  MRP_EDD_UPPER_SWI_FLUSH_FILTERING_DB_PTR_TYPE pFlushFilterParam;
  LSA_UINT16 portIdx;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pDBin->pChSys->pChSysDB->pEdd;

  MRP_LOWER_TRACE_01 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_ll_init_modul_mim[inInst=%d] entered", pDBin->pInstance[MRP_IN_INSTANCE_INDEX]);

  /* adapt flush FDB entries with MRP interconnection port related to this MRP interconnection instance */
  pFlushFilterParam = MRP_EDD_LOWER_GET_PPARAM_FLUSH_FILTERING_DB (pDBedd->pInstData[pDBin->pInstance[MRP_INSTANCE_INDEX]].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_FLUSH_FILTERING_DB]);
  pFlushFilterParam->PortID[pFlushFilterParam->PortCnt++] = pParams->IPort;


  /* if the port state is set, then the initial IPort settings have to be adapted as well */
  MRP_ASSERT (pParams->IPort > 0);
  portIdx = (LSA_UINT16)(pParams->IPort - 1);

  pPSParam = MRP_EDD_LOWER_GET_PPARAM_SET_PORT_STATE (pDBedd->pInstData[pDBin->pInstance[MRP_INSTANCE_INDEX]].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_SET_PS_SHADOW]);
  pPSParam->PortIDState[portIdx] = EDD_PORT_STATE_BLOCKING;

#ifdef MRP_CFG_DELAYED_LINK_UP
  mrp_edd_set_link_up_time (pDBin->pChSys, pParams->IPort, MRP_IN_VAL_LINK_UP_IPORT_T_CNT);
#endif

  /* if FDB is flushed, then the IPort has to be blocked (in addition to the MRP ringports) */
  pPSParamTplgChgStart = MRP_EDD_LOWER_GET_PPARAM_SET_PORT_STATE (pDBedd->pInstData[pDBin->pInstance[MRP_INSTANCE_INDEX]].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_SET_PS_TPLG_CHG_START]);
  /* structure is already initialized by MRM/MRC init */
  pPSParamTplgChgStart->PortIDState[portIdx] = EDD_PORT_STATE_BLOCKING;

  mrp_ll_in_burst_req (pDBin->pChSys, pDBin->pInstance, g_MrpllReqTbl_mrp_in_init_mim, LSA_TRUE);
}


/*===========================================================================
* FUNCTION : mrp_in_ll_deinit_modul_mim
*----------------------------------------------------------------------------
* PURPOSE  : deinitialize EDD settings of one MIM instance
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   :  pDBin    = pointer to one MRP interconnection instance
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_VOID mrp_in_ll_deinit_modul_mim (const MRP_IN_DB_TYPE *pDBin)
{
  MRP_EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE pPSParam;
  MRP_EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE pPSParamTplgChgStart;
  MRP_EDD_UPPER_SWI_FLUSH_FILTERING_DB_PTR_TYPE pFlushFilterParam;
  LSA_UINT16 pos, k, numOfPorts;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pDBin->pChSys->pChSysDB->pEdd;

  MRP_LOWER_TRACE_01 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_ll_deinit_modul_mim[inInst=%d] entered", pDBin->pInstance[MRP_IN_INSTANCE_INDEX]);


  /* as this MRP interconnection instance is deactivated, adapt RQB settings for FLUSH FDB to MRP instance settings */
  pFlushFilterParam = MRP_EDD_LOWER_GET_PPARAM_FLUSH_FILTERING_DB (pDBedd->pInstData[pDBin->pInstance[MRP_INSTANCE_INDEX]].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_FLUSH_FILTERING_DB]);

  for (k = 0, pos = pFlushFilterParam->PortCnt; k < pFlushFilterParam->PortCnt; k++)
  {
    if (pFlushFilterParam->PortID[k] == pDBin->IPort)
    {
      pos = k;
      pFlushFilterParam->PortCnt--;
      break;
    }
  }
  for (k = pos; k < pFlushFilterParam->PortCnt; k++)
  {
    pFlushFilterParam->PortID[k] = pFlushFilterParam->PortID[k + 1];
  }


  MRP_ASSERT (pDBin->IPort > 0);
  pPSParam = MRP_EDD_LOWER_GET_PPARAM_SET_PORT_STATE (pDBedd->pInstData[pDBin->pInstance[MRP_INSTANCE_INDEX]].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_SET_PS_SHADOW]);
  pPSParam->PortIDState[pDBin->IPort - 1] = EDD_PORT_STATE_UNCHANGED;


  /* if FDB is flushed, then the IPort has to be blocked (in addition to the MRP ringports) */
  pPSParamTplgChgStart = MRP_EDD_LOWER_GET_PPARAM_SET_PORT_STATE (pDBedd->pInstData[pDBin->pInstance[MRP_INSTANCE_INDEX]].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_SET_PS_TPLG_CHG_START]);
  /* structure is already initialized by MRP init */
  pPSParamTplgChgStart->PortIDState[pDBin->IPort - 1] = EDD_PORT_STATE_UNCHANGED;

  /* port of MRP interconnection IPort has to be set to forwarding directly, because afterwards DEINIT of MRP instance might follow which might not change the port state of the IPort */
  numOfPorts = mrp_sys_get_num_of_ports (pDBin->pChSys);
  pPSParam = MRP_EDD_LOWER_GET_PPARAM_SET_PORT_STATE (pDBedd->pInstData[pDBin->pInstance[MRP_INSTANCE_INDEX]].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_SET_PS_DEINIT_MRPI]);
  for (k = 0; k < numOfPorts; k++)
  {
    pPSParam->PortIDState[k] = EDD_PORT_STATE_UNCHANGED;
    if (k == (pDBin->IPort - 1))
    {
      pPSParam->PortIDState[k] = EDD_PORT_STATE_FORWARDING;
    }
  }
  mrp_ll_in_burst_req(pDBin->pChSys, pDBin->pInstance, g_MrpllReqTbl_mrp_ps_deinit_mrpi_set, LSA_TRUE);


#ifdef MRP_CFG_DELAYED_LINK_UP
  /**/
  mrp_edd_set_link_up_time (pDBin->pChSys, pDBin->IPort, MRP_VAL_LINK_UP_T_DEFAULT_CNT);
#endif

}

/*===========================================================================
* FUNCTION : mrp_in_mim_mrp_ring_chg_ind_cbf
*----------------------------------------------------------------------------
* PURPOSE  : indicate MRP ring changes to a MIM
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys     = channel reference for edd system channel
*            instance   = affected MRP instance
*            inInstance = affected MRP interconnection instance
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mim_mrp_ring_chg_ind_cbf (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT16 inInstance)
{
  LSA_UINT32 currTimeTicks, delayTicks;
  MRP_IN_EVENT_DB_TYPE DBinEv, *pDBinEv;
  MRP_IN_DB_TYPE *pDBin;
  MRP_IN_DB_TYPE *pDBinBase = (MRP_IN_DB_TYPE *)pChSys->pChSysDB->pMrpIn;
  LSA_UINT16 mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pChSys);

  MRP_ASSERT (inInstance <= mrpInMaxInstance);
  MRP_ASSERT (instance == MRP_DEFAULT_INSTANCE);

  pDBin = &pDBinBase[inInstance];

  currTimeTicks = mrp_sys_cyclic_get_curr_time ();
  delayTicks = mrp_sys_timer_get_time_diff (pDBin->prevTimeTicks, currTimeTicks);

  if (delayTicks >= MRP_IN_POLL_RETRIGGER_DELAY_MS / MRP_TIMER_TIME_BASE_DEZ)
  {
    pDBinEv = &DBinEv;
    MRP_MEMSET (pDBinEv, 0, sizeof (MRP_IN_EVENT_DB_TYPE));

    pDBinEv->pChSys     = pChSys;
    pDBinEv->evInstance = inInstance;

    mrp_in_update (pDBinEv, MRP_IN_EVENT_T_MRP_CHG_IND);

    pDBin->prevTimeTicks = currTimeTicks;
  }
  else
  {
    MRP_PROGRAM_TRACE_02 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIM[inInst=%d]: mrp_in_mim_mrp_ring_chg_ind_cbf: MRP_IN_EVENT_T_MRP_CHG_IND NOT sent to MIM SM, delayTicks=%d", 
                          inInstance, delayTicks);
  }
}

/*===========================================================================
* FUNCTION : mrp_in_mim_set_mrp_ring_chg_ind_callback
*----------------------------------------------------------------------------
* PURPOSE  : set MRP indication callback function for indication of MRP
*            ring changes
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pDBin    = pointer to one MRP interconnection instance
*            cbf        = callback function pointer
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mim_set_mrp_ring_chg_ind_callback (const MRP_IN_DB_TYPE *pDBin, MRP_IN_MIM_MRP_RING_CHG_FCT_PTR_TYPE cbf)
{
  const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys = pDBin->pChSys;
  const LSA_UINT16 *pInstance = pDBin->pInstance;

  MRP_MODE_DATA_TYPE mrpData;
  MRP_MRC_DB_TYPE *pDBmrc;
  MRP_MRC_DB_TYPE *pDBmrcBase = (MRP_MRC_DB_TYPE *)pChSys->pChSysDB->pMrc;

#ifdef MRP_CFG_PLUGIN_0_MRM
  MRP_MRM_DB_TYPE *pDBmrm;
  MRP_MRM_DB_TYPE *pDBmrmBase = (MRP_MRM_DB_TYPE *)pChSys->pChSysDB->pMrm;

  pDBmrm = &pDBmrmBase[pInstance[MRP_INSTANCE_INDEX]];
#endif
  pDBmrc = &pDBmrcBase[pInstance[MRP_INSTANCE_INDEX]];

  MRP_FATAL1 (cbf != LSA_NULL);

  mrp_sys_get_mrp_mode (pChSys, pInstance[MRP_INSTANCE_INDEX], &mrpData);

  if (mrpData.mrpMode == MRP_MODE_MRM_ON) /* MRM module is active */
  {
    if (mrpData.mrpOperRole == MRP_OPER_ROLE_CLIENT) /* MRA is active in operational role MRC */
    {
      pDBmrc->ppMrpInRingChgCbf[pInstance[MRP_IN_INSTANCE_INDEX]] = cbf;
    }
    else /* MRA/MRM is active in operational role MRA/MRM */
    {
#ifdef MRP_CFG_PLUGIN_0_MRM
      pDBmrm->ppMrpInRingChgCbf[pInstance[MRP_IN_INSTANCE_INDEX]] = cbf;
#endif
    }
  }
  else /* MRC module is active */
  {
    if (mrpData.mrpMode == MRP_MODE_MRC_ON)
    {
      pDBmrc->ppMrpInRingChgCbf[pInstance[MRP_IN_INSTANCE_INDEX]] = cbf;
    }
    else
    {
      MRP_ASSERT (mrpData.mrpMode == MRP_MODE_MRC_ON);
    }
  }
}

/*===========================================================================
* FUNCTION : mrp_in_mim_remove_mrp_ring_chg_ind_callback
*----------------------------------------------------------------------------
* PURPOSE  : remove MRP indication callback function for indication of MRP
*            ring changes
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pDBin    = pointer to one MRP interconnection instance
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mim_remove_mrp_ring_chg_ind_callback (const MRP_IN_DB_TYPE *pDBin)
{
  const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys = pDBin->pChSys;
  const LSA_UINT16 *pInstance = pDBin->pInstance;

  MRP_MODE_DATA_TYPE mrpData;
  MRP_MRC_DB_TYPE *pDBmrc;
  MRP_MRC_DB_TYPE *pDBmrcBase = (MRP_MRC_DB_TYPE *)pChSys->pChSysDB->pMrc;

#ifdef MRP_CFG_PLUGIN_0_MRM
  MRP_MRM_DB_TYPE *pDBmrm;
  MRP_MRM_DB_TYPE *pDBmrmBase = (MRP_MRM_DB_TYPE *)pChSys->pChSysDB->pMrm;

  pDBmrm = &pDBmrmBase[pInstance[MRP_INSTANCE_INDEX]];
#endif
  pDBmrc = &pDBmrcBase[pInstance[MRP_INSTANCE_INDEX]];

  mrp_sys_get_mrp_mode (pChSys, pInstance[MRP_INSTANCE_INDEX], &mrpData);

  if (mrpData.mrpMode == MRP_MODE_MRM_ON) /* MRM module is active */
  {
    if (mrpData.mrpOperRole == MRP_OPER_ROLE_CLIENT) /* MRA is active in operational role MRC */
    {
      pDBmrc->ppMrpInRingChgCbf[pInstance[MRP_IN_INSTANCE_INDEX]] = LSA_NULL;
    }
    else /* MRA/MRM is active in operational role MRA/MRM */
    {
#ifdef MRP_CFG_PLUGIN_0_MRM
      pDBmrm->ppMrpInRingChgCbf[pInstance[MRP_IN_INSTANCE_INDEX]] = LSA_NULL;
#endif
    }
  }
  else /* MRC module is active */
  {
    if (mrpData.mrpMode == MRP_MODE_MRC_ON)
    {
      pDBmrc->ppMrpInRingChgCbf[pInstance[MRP_IN_INSTANCE_INDEX]] = LSA_NULL;
    }
    else
    {
      MRP_ASSERT (mrpData.mrpMode == MRP_MODE_MRC_ON);
    }
  }
}

/*===========================================================================
* FUNCTION : mrp_in_mim_inTC_timer_start
*----------------------------------------------------------------------------
* PURPOSE  : start InTC timer
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin        = internal MRP interconnection data base reference
*            durationTicks = time in ticks
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_VOID mrp_in_mim_inTC_timer_start (const MRP_IN_DB_TYPE *pDBin, LSA_UINT32 durationTicks)
{
  mrp_sys_timer_stop (pDBin->TimerIdInTCsend);
  mrp_sys_timer_start (pDBin->TimerIdInTCsend, durationTicks);
}


/*===========================================================================
* FUNCTION : mrp_in_mim_inTC_timer_stop
*----------------------------------------------------------------------------
* PURPOSE  : stop InTC timer
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = internal MRP interconnection data base reference
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_VOID mrp_in_mim_inTC_timer_stop (const MRP_IN_DB_TYPE *pDBin)
{
  mrp_sys_timer_stop (pDBin->TimerIdInTCsend);
}


/*===========================================================================
* FUNCTION : mrp_in_mim_lnkPoll_timer_start
*----------------------------------------------------------------------------
* PURPOSE  : start LNK_POLL timer
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin        = internal MRP interconnection data base reference
*            durationTicks = time in ticks
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_VOID mrp_in_mim_lnkPoll_timer_start (const MRP_IN_DB_TYPE *pDBin, LSA_UINT32 durationTicks)
{
  mrp_sys_timer_stop (pDBin->TimerIdInPollsend);
  mrp_sys_timer_start (pDBin->TimerIdInPollsend, durationTicks);
}


/*===========================================================================
* FUNCTION : mrp_in_mim_lnkPoll_timer_stop
*----------------------------------------------------------------------------
* PURPOSE  : stop LNK_POLL timer
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = internal MRP interconnection data base reference
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_VOID mrp_in_mim_lnkPoll_timer_stop (const MRP_IN_DB_TYPE *pDBin)
{
  mrp_sys_timer_stop (pDBin->TimerIdInPollsend);
}


/*===========================================================================
* FUNCTION : mrp_in_mim_inTC_req_send
*----------------------------------------------------------------------------
* PURPOSE  : send an InTC frame to request ring reconfiguration from MRM
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin        = internal MRP interconnection data base reference
*            durationTicks = time in ticks
*            inInstance    = MRP IN instance
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_VOID mrp_in_mim_inTC_req_send (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 inInstance)
{
  MRP_IN_PACKET_STRUCT_TPLG_CHG_TYPE *pFrame;
  LSA_UINT32 durationTicks;
  LSA_UINT16 intervalTimeMs;
  MRP_IN_TX_REQ_INFO_TYPE *pInTcTxInfo;
  MRP_IN_DB_TYPE *pDBin;
  MRP_IN_DB_TYPE *pDBinBase = (MRP_IN_DB_TYPE *)pChSys->pChSysDB->pMrpIn;
  MRP_IN_MIM_DB_TYPE *pDBmim;
  MRP_EDD_LOWER_RQB_PTR_TYPE pRQB = LSA_NULL;
  LSA_UINT16 mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pChSys);

  MRP_ASSERT (inInstance <= mrpInMaxInstance);

  pDBin = &pDBinBase[inInstance];
  pDBmim = &pDBin->DBmim;

  durationTicks = (LSA_UINT32)(pDBmim->TOPNReturn * pDBmim->TOPintervalTcnt);
  intervalTimeMs = (LSA_UINT16)(durationTicks * MRP_TIMER_TIME_BASE_DEZ);

  if (pDBmim->TOPNReturn > 0)
  {
    pDBmim->TOPNReturn--;
    mrp_in_mim_inTC_timer_start (pDBin, pDBmim->TOPintervalTcnt);
  }
  else
  {
    pDBmim->TOPNReturn = pDBmim->MRPIC_TOPNRmax;
    /* NOTE: InTC is repeated MRPIC_TOPNRmax times */
    /* first InTC is sent with timestamp SetupInterconnTopologyChangeReq(MRP_IN_TOPNRmax X time) -> number of sent InTC frames per port: MRPIC_TOPNRmax + 1 */
  }

  if (pDBin->pChSys->state != MRP_CH_STATE_CLOSING)
  {
    if (pDBmim->runState == MRP_OPERATION_ON)
    {
      pInTcTxInfo = &pDBin->InTcTxInfo;

      pInTcTxInfo->intervalTimeMs = intervalTimeMs;
      MRP_MEMCPY (pInTcTxInfo->coupledDomainUUID, pDBin->CoupledDomainUUID, sizeof (pInTcTxInfo->coupledDomainUUID)); /* if the coupled UUID changed between two InTC send requests -> use the newer one */

      (void) (mrp_edd_get_snd_rqb (pDBin->pChSys, pDBin->pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_TC_BOT, &pRQB, MRP_EDD_GET_SND_RQB_NO_THRESHOLD));

      if (!LSA_HOST_PTR_ARE_EQUAL (pRQB, LSA_NULL))
      {
        /* send parameter */
        pFrame = (MRP_IN_PACKET_STRUCT_TPLG_CHG_TYPE*)MRP_EDD_LOWER_GET_NRT_TX_BUFFER (pRQB); /*lint !e826 CM 10/11/2016 cast on purpose */

        pFrame->payload.inId       = MRP_HTON16 (pInTcTxInfo->inId);
        pFrame->payload.sequenceID = MRP_HTON16 (pDBin->pDBifRef->pSequenceID[MRP_DEFAULT_INSTANCE]);
        pFrame->payload.interval   = MRP_HTON16 (intervalTimeMs);

        MRP_MEMCPY (pFrame->payload.sa, pDBin->pDBifRef->macAddr, MRP_PACKET_VAL_MAC_ADDR_LEN);

        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrame->payload.domainUUID, pDBin->DomainUUID, sizeof (pFrame->payload.domainUUID));

        /* Append the UUID of the MRP interconnection coupling device to the InTC frame. This is a manufacturer specific option. */
        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)(&pFrame->payload.uSuffix.inTcOption), (LSA_VOID_PTR_TYPE)MRP_IN_PACKET_VAL_OPTION_COUPLED_UUID, sizeof (pFrame->payload.uSuffix.inTcOption));
        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)(pFrame->payload.uSuffix.inTcOption.coupledDomainUUID), pDBin->CoupledDomainUUID, sizeof (pFrame->payload.uSuffix.inTcOption.coupledDomainUUID));

        MRP_PROGRAM_TRACE_03 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: send mrp_inTC_req port=%u, sequID=%u", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort, pDBin->pDBifRef->pSequenceID[MRP_DEFAULT_INSTANCE]);
        mrp_edd_req_snd (pRQB, pDBin->IPort, MRP_IN_PACKET_SIZE_IN_TPLG_CHG, LSA_TRUE);

        pDBin->pDBifRef->pSequenceID[MRP_DEFAULT_INSTANCE]++;
      }
      else
      {
        MRP_PROGRAM_TRACE_02 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "MIM[inInst=%d]: mrp_in_mim_inTC_req_send: NO RQB to send inTC on port=%u", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);
      }

      /* provide TX info for MRC/MRM */
      pDBin->InTxInfo = pDBin->InTcTxInfo;
      mrp_in_mrp_tx_request (pDBin);
    }
    else
    {
      MRP_PROGRAM_TRACE_02 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "MIM[inInst=%d]: mrp_in_mim_inTC_req_send: inTC NOT sent on port=%u because MIM runState NOT ON", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);
    }
  }
  else
  {
    MRP_PROGRAM_TRACE_02 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "MIM[inInst=%d]: mrp_in_mim_inTC_req_send: inTC NOT sent on port=%u because ch_state=CLOSING", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);
  }
}


/*===========================================================================
* FUNCTION : mrp_in_mim_edd_req_in_tplg_chg_req
*----------------------------------------------------------------------------
* PURPOSE  : queue interconnection topology change snd requests
*            originated by MIM
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            inInstance    = MRP interconnection instance
*            rqbNum   = edd req reference
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS : due to synchronization with lower layer send request must
*            wait for acknowledged edd requests.
*==========================================================================*/
LSA_VOID mrp_in_mim_edd_req_in_tplg_chg_req (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum)
{
  LSA_UNUSED_ARG (rqbNum);

  MRP_PROGRAM_TRACE_01 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: mrp_in_mim_edd_req_in_tplg_chg_req entered", pInstance[MRP_IN_INSTANCE_INDEX]);

  mrp_in_mim_inTC_req_send (pChSys, pInstance[MRP_IN_INSTANCE_INDEX]);

  mrp_ll_burst_do (pChSys);
}


/*===========================================================================
* FUNCTION : mrp_in_mim_queue_inTC_req
*----------------------------------------------------------------------------
* PURPOSE  : queue a send request for a InTC frame for MRP interconnection
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of MRP interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mim_queue_inTC_req (MRP_IN_DB_TYPE *pDBin)
{
  MRP_IN_TX_REQ_INFO_TYPE *pInTcTxInfo;

  pInTcTxInfo                   = &pDBin->InTcTxInfo;

  pInTcTxInfo->inInstance       = pDBin->pInstance[MRP_IN_INSTANCE_INDEX];
  pInTcTxInfo->inId             = pDBin->InID;
  pInTcTxInfo->isRstpOriginated = LSA_FALSE;
  pInTcTxInfo->inFrameType      = MRP_IN_FRAME_TYPE_IN_TC;
  pInTcTxInfo->rxPort           = 0;

  MRP_MEMCPY (pInTcTxInfo->rxSA, pDBin->pDBifRef->macAddr, MRP_PACKET_VAL_MAC_ADDR_LEN);
  MRP_MEMCPY (pInTcTxInfo->domainUUID, pDBin->DomainUUID, sizeof (pInTcTxInfo->domainUUID));
  MRP_MEMCPY (pInTcTxInfo->coupledDomainUUID, pDBin->CoupledDomainUUID, sizeof (pInTcTxInfo->coupledDomainUUID));

  MRP_PROGRAM_TRACE_02 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "MIM[inInst=%d]: mrp_in_mim_queue_inTC_req: new InTC request (InID=%d) queued", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->InID);
  
  mrp_ll_in_burst_req (pDBin->pChSys, pDBin->pInstance, g_MrpllReqTbl_mrp_in_mim_tplg_chg_req, LSA_FALSE);

}

/*===========================================================================
* FUNCTION : mrp_in_mim_lnkPoll_req_send
*----------------------------------------------------------------------------
* PURPOSE  : send a LNK_POLL frame to request the MIC's interconnection link state
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin        = internal MRP interconnection data base reference
*            durationTicks = time in ticks
*            txOnIPort     = if true, then send request is for ring ports AND IPort
*                            if false, send request is only for ring ports
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_VOID mrp_in_mim_lnkPoll_req_send (MRP_IN_DB_TYPE *pDBin, LSA_UINT32 durationTicks, LSA_BOOL txOnIPort)
{
  MRP_IN_PACKET_STRUCT_POLL_TYPE *pFrame;
  MRP_IN_TX_REQ_INFO_TYPE *pInLnkPollTxInfo;

  MRP_EDD_LOWER_RQB_PTR_TYPE pRQB = LSA_NULL;

  if (pDBin->pChSys->state != MRP_CH_STATE_CLOSING)
  {
    if (mrp_sys_timer_get_state (pDBin->TimerIdInPollsend) != MRP_TIMER_STS_T_RUNNING)
    {
      pInLnkPollTxInfo = &pDBin->InPollTxInfo;

      pInLnkPollTxInfo->inInstance  = pDBin->pInstance[MRP_IN_INSTANCE_INDEX];
      pInLnkPollTxInfo->inId        = pDBin->InID;
      pInLnkPollTxInfo->inFrameType = MRP_IN_FRAME_TYPE_LINK_STATE_POLL;
      MRP_MEMCPY (pInLnkPollTxInfo->rxSA, pDBin->pDBifRef->macAddr, MRP_PACKET_VAL_MAC_ADDR_LEN);

      if (txOnIPort)
      {
        (void) (mrp_edd_get_snd_rqb (pDBin->pChSys, pDBin->pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_POLL_BOT, &pRQB, MRP_EDD_GET_SND_RQB_NO_THRESHOLD));

        if (!LSA_HOST_PTR_ARE_EQUAL (pRQB, LSA_NULL))
        {
          /* send parameter */
          pFrame = (MRP_IN_PACKET_STRUCT_POLL_TYPE*)MRP_EDD_LOWER_GET_NRT_TX_BUFFER (pRQB); /*lint !e826 CM 10/11/2016 cast on purpose */

          pFrame->payload.portRole = MRP_HTON16 (MRP_IN_PACKET_VAL_PORT_ROLE_IN);
          pFrame->payload.inId = MRP_HTON16 (pInLnkPollTxInfo->inId);
          pFrame->payload.sequenceID = MRP_HTON16 (pDBin->pDBifRef->pSequenceID[MRP_DEFAULT_INSTANCE]);

          MRP_MEMCPY (pFrame->payload.sa, pDBin->pDBifRef->macAddr, MRP_PACKET_VAL_MAC_ADDR_LEN);
          MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrame->payload.domainUUID, pDBin->DomainUUID, sizeof (pFrame->payload.domainUUID));


          MRP_PROGRAM_TRACE_04 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: send mrp_in_mim_lnkPoll_req port=%u, sequID=%u, durationTicks=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort, pDBin->pDBifRef->pSequenceID[MRP_DEFAULT_INSTANCE], durationTicks);
          mrp_edd_req_snd (pRQB, pDBin->IPort, MRP_IN_PACKET_SIZE_DEFAULT, LSA_TRUE);

          pDBin->pDBifRef->pSequenceID[MRP_DEFAULT_INSTANCE]++;
        }
        else
        {
          MRP_PROGRAM_TRACE_02 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "MIM[inInst=%d]: No RQB available! Ignore mrp_in_mim_lnkPoll_req_send IPort=%u", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);
        }
      }

      /* provide TX info for MRC/MRM */
      pDBin->InTxInfo = pDBin->InPollTxInfo;
      mrp_in_mrp_tx_request (pDBin);

      if (durationTicks)
        mrp_in_mim_lnkPoll_timer_start (pDBin, durationTicks);
    }
    else
    {
      MRP_PROGRAM_TRACE_02 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "MIM[inInst=%d]: mrp_in_mim_lnkPoll_req_send: PollReq timer already running for IPort=%d!", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);
    }
  }
  else
  {
    MRP_PROGRAM_TRACE_02 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "MIM[inInst=%d]: chState=CLOSING! Ignore mrp_in_mim_lnkPoll_req_send port=%u", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);

  }
}



/*===========================================================================
* FUNCTION : mrp_in_mim_oper_state_chk_ic
*----------------------------------------------------------------------------
* PURPOSE  : operational MRP interconnection CBF for MIM called when the MIM
*            detects a LINK_UP on it's own IPort, while both PRM and SEC link
*            have signaled a LINK_UP state and SEC IPort is BLOCKED by MIM
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of mrp interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mim_oper_state_chk_ic (MRP_IN_DB_TYPE *pDBin)
{
  LSA_BOOL rxFromDifferentIn, differentMac, differentInId;
  MRP_IN_MIM_DB_TYPE *pDBmim;

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBin->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  pDBmim = &pDBin->DBmim;

  MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: mrp_in_mim_oper_state_chk_ic entered: event=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.CurrentEvent);

  switch (pDBin->rxPara.CurrentEvent)
  {
    /*==========================================================================*/
    case MRP_IN_EVENT_T_NONE:
    {
      MRP_FATAL;
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_MAU_TYPE_CHG:
    {
      if (pDBin->rxPara.LinkState == MRP_LINK_STATE_DOWN)
      {
        MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: chk_ic=>prm_up, (MAU_TYPE_CHG SEC IPort=%u DOWN)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);

        mrp_in_write_current_in_open_diag (pDBin, MRP_IN_RED_LINK_IN_OPEN_SEC, LSA_TRUE);

        pDBmim->pInOperStateCbf = mrp_in_mim_oper_state_prm_up;
      }
      else /* LinkState == MRP_LINK_STATE_UP */
      {
        /* do nothing */
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_TPLG_CHG_TRF:
    {
      /* ignore */
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_TPLG_CHG_WRK:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort != pDBin->IPort)
        {
          rxFromDifferentIn = mrp_in_rx_from_different_interconn (pDBin, &differentMac, &differentInId);

          if (rxFromDifferentIn)
          {
            MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: chk_ic=>chk_ic, (IN_TC_WRK received from different MIM at port=%d, IPort=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);

            mrp_in_inTC_forward_to_mrp (pDBin);
          }
          else
          {
            /* ignore */
          }
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_POLL_WRK:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort != pDBin->IPort)
        {
          rxFromDifferentIn = mrp_in_rx_from_different_interconn (pDBin, &differentMac, &differentInId);

          if (rxFromDifferentIn)
          {
            MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "MIM[inInst=%d]: chk_ic=>chk_ic, (IN_POLL_WRK received from different MIM at port=%d, IPort=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);

            mrp_in_mim_lnkPoll_req_send (pDBin, pDBmim->LNKSTATPollTcntCurrent_10ms, LSA_TRUE); /* duplicate InIDs: respond with Poll_Req */
          }
          else
          {
            /* ignore */
          }
        }
      }

    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_POLL_TRF:
    {
      /* ignore */
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_LNK_CHG_WRK:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort != pDBin->IPort)
        {
          if ((pDBin->rxPara.RxLnkRole & MRP_IN_PACKET_VAL_BITMASK_BIT0_SET) == MRP_IN_PACKET_VAL_LNK_ROLE_BIT0_PRM)
          {
            pDBmim->hasReceivedMicFrame = LSA_TRUE;
            pDBmim->addPoll             = LSA_FALSE;

            mrp_in_mim_lnkPoll_timer_stop (pDBin);
            mrp_in_mim_init_proj_poll_params (pDBin);

            if (pDBin->rxPara.LinkState == MRP_LINK_STATE_DOWN)
            {
              MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: chk_ic=>chk_io, (IN_LNK_CHG_WRK (LNK_DOWN) received at port=%d, IPort=%d)", 
                                    pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);

              mrp_edd_set_port_state (pDBin->pChSys, pDBin->pInstance, pDBin->IPort, MRP_PORT_STATE_FORWARD);

              mrp_in_mim_queue_inTC_req (pDBin);

              mrp_in_write_current_in_open_diag (pDBin, MRP_IN_RED_LINK_IN_OPEN_PRM, LSA_TRUE);

              pDBmim->pInOperStateCbf = mrp_in_mim_oper_state_chk_io;
            }
            else /* LINK_UP received from PRM MIC */
            {
              MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: chk_ic=>chk_ic, (IN_LNK_CHG_WRK (LNK_UP) received at port=%d, IPort=%d)", 
                                    pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);
            }
          }
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_LNK_CHG_TRF:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort == pDBin->IPort)
        {
          if ((pDBin->rxPara.RxLnkRole & MRP_IN_PACKET_VAL_BITMASK_BIT0_SET) == MRP_IN_PACKET_VAL_LNK_ROLE_BIT0_PRM)
          {
            pDBmim->hasReceivedMicFrame = LSA_TRUE;
            pDBmim->addPoll             = LSA_FALSE;

            mrp_in_mim_lnkPoll_timer_stop (pDBin);
            mrp_in_mim_init_proj_poll_params (pDBin);

            if (pDBin->rxPara.LinkState == MRP_LINK_STATE_DOWN)
            {
              MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: chk_ic=>chk_io, (IN_LNK_CHG_TRF (LNK_DOWN) received at port=%d, IPort=%d)", 
                                    pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);

              mrp_edd_set_port_state (pDBin->pChSys, pDBin->pInstance, pDBin->IPort, MRP_PORT_STATE_FORWARD);

              mrp_in_mim_queue_inTC_req (pDBin);

              mrp_in_write_current_in_open_diag (pDBin, MRP_IN_RED_LINK_IN_OPEN_PRM, LSA_TRUE);

              pDBmim->pInOperStateCbf = mrp_in_mim_oper_state_chk_io;
            }
            else /* LINK_UP received from PRM MIC */
            {
              MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: chk_ic=>chk_ic, (IN_LNK_CHG_TRF (LNK_UP) received at port=%d, IPort=%d)", 
                                    pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);
            }
          }
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_LNK_POLL_TIMER:
    {
      if (pDBmim->LNKSTATPollNReturnCurrent > 0)
      {
        MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIM[inInst=%d]: chk_ic=>chk_ic, (LNK_POLL TIMER expired IPort=%u UP, NReturn=%d)", 
                              pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort, pDBmim->LNKSTATPollNReturnCurrent);

        mrp_in_mim_lnkPoll_req_send (pDBin, pDBmim->LNKSTATPollTcntCurrent_10ms, LSA_TRUE);

        pDBmim->LNKSTATPollNReturnCurrent--;
      }
      else /* pDBmim->LNKSTATPollNReturnCurrent == 0 */
      {
        mrp_in_mim_init_proj_poll_params (pDBin);

        if (pDBmim->addPoll)
        {
          pDBmim->addPoll = LSA_FALSE;
          pDBin->rxPara.LinkAddInfo.AddCheckEnabled = EDD_LINK_ADD_CHECK_DISABLED;
          pDBin->rxPara.LinkAddInfo.ChangeReason    = EDD_LINK_CHANGE_REASON_NONE;

          MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: chk_ic=>chk_io, (ADD_POLL_TIMER expired (->LNK_DOWN expected for PRM link), IPort=%d)",
                                pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);

          mrp_edd_set_port_state (pDBin->pChSys, pDBin->pInstance, pDBin->IPort, MRP_PORT_STATE_FORWARD);
          mrp_in_mim_queue_inTC_req (pDBin);

          mrp_in_write_current_in_open_diag (pDBin, MRP_IN_RED_LINK_IN_OPEN_PRM, LSA_TRUE);

          pDBmim->pInOperStateCbf = mrp_in_mim_oper_state_chk_io;
        }
        else
        {
          MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIM[inInst=%d]: chk_ic=>chk_ic, (LNK_POLL TIMER expired IPort=%u UP, NReturn=0)", 
                                pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_MRP_CHG_IND:
    {
      pDBmim->addPoll = LSA_TRUE;

      if (mrp_sys_timer_get_state (pDBin->TimerIdInPollsend) != MRP_TIMER_STS_T_RUNNING) /* only start a new InPoll request, if InPoll request is not already going on!!! */
      {
        MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "MIM[inInst=%d]: chk_ic=>chk_ic, (MRP_CHG_IND received from MRP, trigger addPoll, IPort=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);

        mrp_in_mim_init_add_poll_params (pDBin);

        mrp_in_mim_lnkPoll_req_send (pDBin, pDBmim->LNKSTATPollTcntCurrent_10ms, LSA_TRUE);
      }
    }
    break;

    /*==========================================================================*/
    default:
    {
      MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_FATAL, "MIM[inInst=%d]: mrp_in_mim_oper_state_chk_ic: Invalid eventType=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.CurrentEvent);
    }
    break;
  }
}


/*===========================================================================
* FUNCTION : mrp_in_mim_oper_state_chk_io
*----------------------------------------------------------------------------
* PURPOSE  : operational MRP interconnection CBF for MIM called when the MIM
*            detects a LINK_UP on it's own IPort, while the polling of the status
*            of the PRM link has ended without response from PRM link device
*            (SEC IPort is changed to FORWARDING state)
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of mrp interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mim_oper_state_chk_io (MRP_IN_DB_TYPE *pDBin)
{
  LSA_BOOL rxFromDifferentIn, differentMac, differentInId;
  MRP_IN_MIM_DB_TYPE *pDBmim;

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBin->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  pDBmim = &pDBin->DBmim;

  MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: mrp_in_mim_oper_state_chk_io entered: event=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.CurrentEvent);

  switch (pDBin->rxPara.CurrentEvent)
  {
    /*==========================================================================*/
    case MRP_IN_EVENT_T_NONE:
    {
      MRP_FATAL;
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_MAU_TYPE_CHG:
    {
      if (pDBin->rxPara.LinkState == MRP_LINK_STATE_DOWN)
      {
        MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: chk_io=>ac_stat1, (MAU_TYPE_CHG SEC IPort=%u DOWN)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);

        pDBmim->hasReceivedMicFrame = LSA_FALSE;
        mrp_in_mim_init_start_delay (pDBin);

        mrp_in_mim_lnkPoll_req_send (pDBin, pDBmim->LNKSTATPollTcntCurrent_10ms, LSA_TRUE);

        mrp_edd_set_port_state (pDBin->pChSys, pDBin->pInstance, pDBin->IPort, MRP_PORT_STATE_BLOCKED);

        mrp_in_write_current_in_open_diag (pDBin, MRP_IN_RED_LINK_IN_OPEN_SEC, LSA_TRUE);

        pDBmim->pInOperStateCbf = mrp_in_mim_oper_state_ac_stat1;
      }
      else /* LinkState == MRP_LINK_STATE_UP */
      {
        /* do nothing */
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_TPLG_CHG_TRF:
    {
      /* ignore */
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_TPLG_CHG_WRK:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort != pDBin->IPort)
        {
          rxFromDifferentIn = mrp_in_rx_from_different_interconn (pDBin, &differentMac, &differentInId);

          if (rxFromDifferentIn)
          {
            MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: chk_io=>chk_io, (IN_TC_WRK received from different MIM at port=%d, IPort=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);

            mrp_in_inTC_forward_to_mrp (pDBin);
          }
          else
          {
            /* ignore */
          }
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_POLL_WRK:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort != pDBin->IPort)
        {
          rxFromDifferentIn = mrp_in_rx_from_different_interconn (pDBin, &differentMac, &differentInId);

          if (rxFromDifferentIn)
          {
            MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "MIM[inInst=%d]: chk_io=>chk_io, (IN_POLL_WRK received from different MIM at port=%d, IPort=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);

            mrp_in_mim_lnkPoll_req_send (pDBin, pDBmim->LNKSTATPollTcntCurrent_10ms, LSA_TRUE); /* duplicate InIDs: respond with Poll_Req */
          }
          else
          {
            /* ignore */
          }
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_POLL_TRF:
    {
      /* ignore */
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_LNK_CHG_WRK:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort != pDBin->IPort)
        {
          if ((pDBin->rxPara.RxLnkRole & MRP_IN_PACKET_VAL_BITMASK_BIT0_SET) == MRP_IN_PACKET_VAL_LNK_ROLE_BIT0_PRM)
          {
            pDBmim->hasReceivedMicFrame = LSA_TRUE;

            mrp_in_mim_lnkPoll_timer_stop (pDBin);
            mrp_in_mim_init_proj_poll_params (pDBin);

            if (pDBin->rxPara.LinkState == MRP_LINK_STATE_UP)
            {
              MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: chk_io=>chk_ic, (IN_LNK_CHG_WRK (LNK_UP) received at port=%d, IPort=%d)", 
                                    pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);

              mrp_edd_set_port_state (pDBin->pChSys, pDBin->pInstance, pDBin->IPort, MRP_PORT_STATE_BLOCKED);

              mrp_in_mim_queue_inTC_req (pDBin);

              mrp_in_write_current_in_open_diag (pDBin, MRP_IN_RED_LINK_IN_OPEN_PRM, LSA_FALSE);

              pDBmim->pInOperStateCbf = mrp_in_mim_oper_state_chk_ic;
            }
            else /* LINK_DOWN received from PRM MIC */
            {
              MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: chk_io=>chk_io, (IN_LNK_CHG_WRK (LNK_DOWN) received at port=%d, IPort=%d)", 
                                    pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);
            }
          }
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_LNK_CHG_TRF:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort == pDBin->IPort)
        {
          if ((pDBin->rxPara.RxLnkRole & MRP_IN_PACKET_VAL_BITMASK_BIT0_SET) == MRP_IN_PACKET_VAL_LNK_ROLE_BIT0_PRM)
          {
            pDBmim->hasReceivedMicFrame = LSA_TRUE;

            mrp_in_mim_lnkPoll_timer_stop (pDBin);
            mrp_in_mim_init_proj_poll_params (pDBin);

            if (pDBin->rxPara.LinkState == MRP_LINK_STATE_UP)
            {
              MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: chk_io=>chk_ic, (IN_LNK_CHG_TRF (LNK_UP) received at port=%d, IPort=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);

              mrp_edd_set_port_state (pDBin->pChSys, pDBin->pInstance, pDBin->IPort, MRP_PORT_STATE_BLOCKED);

              mrp_in_mim_queue_inTC_req (pDBin);

              mrp_in_write_current_in_open_diag (pDBin, MRP_IN_RED_LINK_IN_OPEN_PRM, LSA_FALSE);

              pDBmim->pInOperStateCbf = mrp_in_mim_oper_state_chk_ic;
            }
            else /* LINK_DOWN received from PRM MIC */
            {
              MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: chk_io=>chk_io, (IN_LNK_CHG_TRF (LNK_DOWN) received at port=%d, IPort=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);
            }
          }
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_LNK_POLL_TIMER:
    {
      MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIM[inInst=%d]: chk_io=>chk_io, (LNK_POLL TIMER expired IPort=%u UP, NReturn=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort, pDBmim->LNKSTATPollNReturnCurrent);
      
      if (pDBmim->LNKSTATPollNReturnCurrent > 0)
      {
        mrp_in_mim_lnkPoll_req_send (pDBin, pDBmim->LNKSTATPollTcntCurrent_10ms, LSA_TRUE);

        pDBmim->LNKSTATPollNReturnCurrent--;
      }
      else /* pDBmim->LNKSTATPollNReturnCurrent == 0 */
      {
        mrp_in_mim_init_proj_poll_params (pDBin);
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_MRP_CHG_IND:
    {
      /* ignore */
    }
    break;

    /*==========================================================================*/
    default:
    {
      MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_FATAL, "MIM[inInst=%d]: mrp_in_mim_oper_state_chk_io: Invalid eventType=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.CurrentEvent);
    }
    break;
  }
}

/*===========================================================================
* FUNCTION : mrp_in_mim_oper_state_sec_up
*----------------------------------------------------------------------------
* PURPOSE  : operational MRP interconnection CBF for MIM called when the MIM
*            detects a LINK_UP on it's IPort, while polling of the link status
*            of the PRM link is in progress
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of mrp interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mim_oper_state_sec_up (MRP_IN_DB_TYPE *pDBin)
{
  LSA_BOOL rxFromDifferentIn, differentMac, differentInId;
  MRP_IN_MIM_DB_TYPE *pDBmim;

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBin->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  pDBmim = &pDBin->DBmim;

  MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: mrp_in_mim_oper_state_sec_up entered: event=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.CurrentEvent);

  switch (pDBin->rxPara.CurrentEvent)
  {
    /*==========================================================================*/
    case MRP_IN_EVENT_T_NONE:
    {
      MRP_FATAL;
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_MAU_TYPE_CHG:
    {
      if (pDBin->rxPara.LinkState == MRP_LINK_STATE_DOWN)
      {
        MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: sec_up=>ac_stat1, (MAU_TYPE_CHG SEC IPort=%u DOWN)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);

        pDBmim->hasReceivedMicFrame = LSA_FALSE;
        mrp_in_mim_init_start_delay (pDBin);

        mrp_in_mim_lnkPoll_req_send (pDBin, pDBmim->LNKSTATPollTcntCurrent_10ms, LSA_TRUE);

        mrp_in_write_current_in_open_diag (pDBin, MRP_IN_RED_LINK_IN_OPEN_SEC, LSA_TRUE);

        pDBmim->pInOperStateCbf = mrp_in_mim_oper_state_ac_stat1;
      }
      else /* LinkState == MRP_LINK_STATE_UP */
      {
        /* do nothing */
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_TPLG_CHG_TRF:
    {
      /* ignore */
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_TPLG_CHG_WRK:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort != pDBin->IPort)
        {
          rxFromDifferentIn = mrp_in_rx_from_different_interconn (pDBin, &differentMac, &differentInId);

          if (rxFromDifferentIn)
          {
            MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: sec_up=>sec_up, (IN_TC_WRK received from different MIM at port=%d, IPort=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);

            mrp_in_inTC_forward_to_mrp (pDBin);
          }
          else
          {
            /* ignore */
          }
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_POLL_WRK:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort != pDBin->IPort)
        {
          rxFromDifferentIn = mrp_in_rx_from_different_interconn (pDBin, &differentMac, &differentInId);

          if (rxFromDifferentIn)
          {
            MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "MIM[inInst=%d]: sec_up=>sec_up, (IN_POLL_WRK received from different MIM at port=%d, IPort=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);

            mrp_in_mim_lnkPoll_req_send (pDBin, pDBmim->LNKSTATPollTcntCurrent_10ms, LSA_TRUE); /* duplicate InIDs: respond with Poll_Req */
          }
          else
          {
            /* ignore */
          }
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_POLL_TRF:
    {
      /* ignore */
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_LNK_CHG_WRK:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort != pDBin->IPort)
        {
          if ((pDBin->rxPara.RxLnkRole & MRP_IN_PACKET_VAL_BITMASK_BIT0_SET) == MRP_IN_PACKET_VAL_LNK_ROLE_BIT0_PRM)
          {
            pDBmim->hasReceivedMicFrame = LSA_TRUE;

            mrp_in_mim_lnkPoll_timer_stop (pDBin);
            mrp_in_mim_init_proj_poll_params (pDBin);

            if (pDBin->rxPara.LinkState == MRP_LINK_STATE_UP)
            {
              MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIM[inInst=%d]: sec_up=>chk_ic, (IN_LNK_CHG_WRK (LNK_UP) received at port=%d, IPort=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);


              mrp_in_write_current_in_open_diag (pDBin, MRP_IN_RED_LINK_IN_OPEN_PRM, LSA_FALSE);

              pDBmim->pInOperStateCbf = mrp_in_mim_oper_state_chk_ic;
            }
            else /* LINK_DOWN received from PRM MIC */
            {
              MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: sec_up=>chk_io, (IN_LNK_CHG_WRK (LNK_DOWN) received at port=%d, IPort=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);
              
              mrp_edd_set_port_state (pDBin->pChSys, pDBin->pInstance, pDBin->IPort, MRP_PORT_STATE_FORWARD);

              mrp_in_mim_queue_inTC_req (pDBin);

              mrp_in_write_current_in_open_diag (pDBin, MRP_IN_RED_LINK_IN_OPEN_PRM, LSA_TRUE);

              pDBmim->pInOperStateCbf = mrp_in_mim_oper_state_chk_io;

            }
          }
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_LNK_CHG_TRF:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort == pDBin->IPort)
        {
          if ((pDBin->rxPara.RxLnkRole & MRP_IN_PACKET_VAL_BITMASK_BIT0_SET) == MRP_IN_PACKET_VAL_LNK_ROLE_BIT0_PRM)
          {
            pDBmim->hasReceivedMicFrame = LSA_TRUE;

            mrp_in_mim_lnkPoll_timer_stop (pDBin);
            mrp_in_mim_init_proj_poll_params (pDBin);

            if (pDBin->rxPara.LinkState == MRP_LINK_STATE_UP)
            {
              MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIM[inInst=%d]: sec_up=>chk_ic, (IN_LNK_CHG_TRF (LNK_UP) received at port=%d, IPort=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);

              mrp_in_write_current_in_open_diag (pDBin, MRP_IN_RED_LINK_IN_OPEN_PRM, LSA_FALSE);

              pDBmim->pInOperStateCbf = mrp_in_mim_oper_state_chk_ic;
            }
            else /* LINK_DOWN received from PRM MIC */
            {
              MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: sec_up=>chk_io, (IN_LNK_CHG_TRF (LNK_DOWN) received at port=%d, IPort=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);
 
              mrp_edd_set_port_state (pDBin->pChSys, pDBin->pInstance, pDBin->IPort, MRP_PORT_STATE_FORWARD);

              mrp_in_mim_queue_inTC_req (pDBin);

              mrp_in_write_current_in_open_diag (pDBin, MRP_IN_RED_LINK_IN_OPEN_PRM, LSA_TRUE);

              pDBmim->pInOperStateCbf = mrp_in_mim_oper_state_chk_io;
            }
          }
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_LNK_POLL_TIMER:
    {
      if (pDBmim->LNKSTATPollNReturnCurrent > 0)
      {
        MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIM[inInst=%d]: sec_up=>sec_up, (LNK_POLL TIMER expired IPort=%u UP, NReturn=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort, pDBmim->LNKSTATPollNReturnCurrent);

        mrp_in_mim_lnkPoll_req_send (pDBin, pDBmim->LNKSTATPollTcntCurrent_10ms, LSA_TRUE);

        pDBmim->LNKSTATPollNReturnCurrent--;
      }
      else /* pDBmim->LNKSTATPollNReturnCurrent == 0 */
      {
        mrp_in_mim_init_proj_poll_params (pDBin);

        if (pDBmim->hasToWait4MicFrame == LSA_FALSE || (pDBmim->hasToWait4MicFrame == LSA_TRUE && pDBmim->hasReceivedMicFrame == LSA_TRUE))
        {
          MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: sec_up=>chk_io, (LNK_POLL TIMER expired IPort=%u UP)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);

          mrp_edd_set_port_state (pDBin->pChSys, pDBin->pInstance, pDBin->IPort, MRP_PORT_STATE_FORWARD);
          
          mrp_in_mim_queue_inTC_req (pDBin);

          pDBmim->pInOperStateCbf = mrp_in_mim_oper_state_chk_io;
        }
        else
        {
          MRP_PROGRAM_TRACE_04 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIM[inInst=%d]: sec_up=>sec_up, (LNK_POLL TIMER expired IPort=%u UP, hasToWait4MIC=%d, hasReceivedMICFrame=%d)",
                                pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort, pDBmim->hasToWait4MicFrame, pDBmim->hasReceivedMicFrame);
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_MRP_CHG_IND:
    {
      /* ignore */
    }
    break;

    /*==========================================================================*/
    default:
    {
      MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_FATAL, "MIM[inInst=%d]: mrp_in_mim_oper_state_sec_up: Invalid eventType=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.CurrentEvent);
    }
    break;
  }
}

/*===========================================================================
* FUNCTION : mrp_in_mim_oper_state_prm_up
*----------------------------------------------------------------------------
* PURPOSE  : operational MRP interconnection CBF for MIM called when the MIM
*            has received an IN_LNK_CHG (LINK_UP) frame from PRM MIC, without
*            having knowledge about the SEC IPort state
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of mrp interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mim_oper_state_prm_up (MRP_IN_DB_TYPE *pDBin)
{
  LSA_BOOL rxFromDifferentIn, differentMac, differentInId;
  MRP_IN_MIM_DB_TYPE *pDBmim;

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBin->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  pDBmim = &pDBin->DBmim;

  MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: mrp_in_mim_oper_state_prm_up entered: event=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.CurrentEvent);

  switch (pDBin->rxPara.CurrentEvent)
  {
    /*==========================================================================*/
    case MRP_IN_EVENT_T_NONE:
    {
      MRP_FATAL;
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_MAU_TYPE_CHG:
    {
      if (pDBin->rxPara.LinkState == MRP_LINK_STATE_UP)
      {
        MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: prm_up=>chk_ic, (MAU_TYPE_CHG SEC IPort=%u UP)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);

        mrp_in_write_current_in_open_diag (pDBin, MRP_IN_RED_LINK_IN_OPEN_SEC, LSA_FALSE);

        pDBmim->pInOperStateCbf = mrp_in_mim_oper_state_chk_ic;
      }
      else /* LinkState == MRP_LINK_STATE_DOWN */
      {
        /* do nothing */
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_TPLG_CHG_TRF:
    {
      /* ignore */
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_TPLG_CHG_WRK:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort != pDBin->IPort)
        {
          rxFromDifferentIn = mrp_in_rx_from_different_interconn (pDBin, &differentMac, &differentInId);

          if (rxFromDifferentIn)
          {
            MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: prm_up=>prm_up, (IN_TC_WRK received from different MIM at port=%d, IPort=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);
            
            mrp_in_inTC_forward_to_mrp (pDBin);
          }
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_POLL_WRK:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort != pDBin->IPort)
        {
          rxFromDifferentIn = mrp_in_rx_from_different_interconn (pDBin, &differentMac, &differentInId);

          if (rxFromDifferentIn)
          {
            MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "MIM[inInst=%d]: prm_up=>prm_up, (IN_POLL_WRK received from different MIM at port=%d, IPort=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);
            
            mrp_in_mim_lnkPoll_req_send (pDBin, pDBmim->LNKSTATPollTcntCurrent_10ms, LSA_FALSE); /* duplicate InIDs: respond with Poll_Req */
          }
          else
          {
            /* ignore */
          }
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_POLL_TRF:
    {
      /* ignore */
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_LNK_CHG_WRK:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort != pDBin->IPort)
        {
          if ((pDBin->rxPara.RxLnkRole & MRP_IN_PACKET_VAL_BITMASK_BIT0_SET) == MRP_IN_PACKET_VAL_LNK_ROLE_BIT0_PRM)
          {
            pDBmim->hasReceivedMicFrame = LSA_TRUE;
            pDBmim->addPoll             = LSA_FALSE;

            if (pDBin->rxPara.LinkState == MRP_LINK_STATE_UP)
            {
              MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIM[inInst=%d]: prm_up=>prm_up, (IN_LNK_CHG_WRK (LNK_UP) received at port=%d, IPort=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);

              mrp_in_mim_lnkPoll_timer_stop (pDBin);
              mrp_in_mim_init_proj_poll_params (pDBin);
            }
            else /* LINK_DOWN received from PRM MIC */
            {
              MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: prm_up=>ac_stat1, (IN_LNK_CHG_WRK (LNK_DOWN) received at port=%d, IPort=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);

              pDBmim->hasReceivedMicFrame = LSA_FALSE;
              mrp_in_mim_init_start_delay (pDBin);

              mrp_in_mim_lnkPoll_req_send (pDBin, pDBmim->LNKSTATPollTcntCurrent_10ms, LSA_FALSE);

              mrp_in_write_current_in_open_diag (pDBin, MRP_IN_RED_LINK_IN_OPEN_PRM, LSA_TRUE);

              pDBmim->pInOperStateCbf = mrp_in_mim_oper_state_ac_stat1;
            }
          }
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_LNK_CHG_TRF:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort == pDBin->IPort)
        {
          if ((pDBin->rxPara.RxLnkRole & MRP_IN_PACKET_VAL_BITMASK_BIT0_SET) == MRP_IN_PACKET_VAL_LNK_ROLE_BIT0_PRM)
          {
            pDBmim->hasReceivedMicFrame = LSA_TRUE;
            pDBmim->addPoll             = LSA_FALSE;

            if (pDBin->rxPara.LinkState == MRP_LINK_STATE_UP)
            {
              MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIM[inInst=%d]: prm_up=>prm_up, (IN_LNK_CHG_TRF (LNK_UP) received at port=%d, IPort=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);

              mrp_in_mim_lnkPoll_timer_stop (pDBin);
              mrp_in_mim_init_proj_poll_params (pDBin);
            }
            else /* LINK_DOWN received from PRM MIC */
            {
              MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: prm_up=>ac_stat1, (IN_LNK_CHG_TRF (LNK_DOWN) received at port=%d, IPort=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);

              pDBmim->hasReceivedMicFrame = LSA_FALSE;
              mrp_in_mim_init_start_delay (pDBin);

              mrp_in_mim_lnkPoll_req_send (pDBin, pDBmim->LNKSTATPollTcntCurrent_10ms, LSA_FALSE);

              mrp_in_write_current_in_open_diag (pDBin, MRP_IN_RED_LINK_IN_OPEN_PRM, LSA_TRUE);

              pDBmim->pInOperStateCbf = mrp_in_mim_oper_state_ac_stat1;
            }
          }
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_LNK_POLL_TIMER:
    {
      if (pDBmim->LNKSTATPollNReturnCurrent > 0)
      {
        MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIM[inInst=%d]: prm_up=>prm_up, (LNK_POLL TIMER expired IPort=%u UP, NReturn=%d)", 
                              pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort, pDBmim->LNKSTATPollNReturnCurrent);

        mrp_in_mim_lnkPoll_req_send (pDBin, pDBmim->LNKSTATPollTcntCurrent_10ms, LSA_FALSE);

        pDBmim->LNKSTATPollNReturnCurrent--;
      }
      else /* pDBmim->LNKSTATPollNReturnCurrent == 0 */
      {
        if (pDBmim->addPoll)
        {
          pDBmim->addPoll = LSA_FALSE;
          pDBin->rxPara.LinkAddInfo.AddCheckEnabled = EDD_LINK_ADD_CHECK_DISABLED;
          pDBin->rxPara.LinkAddInfo.ChangeReason    = EDD_LINK_CHANGE_REASON_NONE;

          MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: prm_up=>ac_stat1, (ADD_POLL_TIMER expired (->LNK_DOWN expected for PRM link), IPort=%d)", 
                                pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);

          pDBmim->hasReceivedMicFrame = LSA_FALSE;
          mrp_in_mim_init_start_delay (pDBin);

          mrp_in_mim_lnkPoll_req_send (pDBin, pDBmim->LNKSTATPollTcntCurrent_10ms, LSA_FALSE);

          mrp_in_write_current_in_open_diag (pDBin, MRP_IN_RED_LINK_IN_OPEN_PRM, LSA_TRUE);

          pDBmim->pInOperStateCbf = mrp_in_mim_oper_state_ac_stat1;
        }
        else
        {
          mrp_in_mim_init_proj_poll_params (pDBin);
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_MRP_CHG_IND:
    {
      pDBmim->addPoll = LSA_TRUE;

      if (mrp_sys_timer_get_state (pDBin->TimerIdInPollsend) != MRP_TIMER_STS_T_RUNNING) /* only start a new InPoll request, if InPoll request is not already going on!!! */
      {
        MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "MIM[inInst=%d]: prm_up=>prm_up, (MRP_CHG_IND received from MRP, trigger addPoll, IPort=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);

        mrp_in_mim_init_add_poll_params (pDBin);

        mrp_in_mim_lnkPoll_req_send (pDBin, pDBmim->LNKSTATPollTcntCurrent_10ms, LSA_FALSE);
      }
    }
    break;

    /*==========================================================================*/
    default:
    {
      MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_FATAL, "MIM[inInst=%d]: mrp_in_mim_oper_state_prm_up: Invalid eventType=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.CurrentEvent);
    }
    break;
  }
}

/*===========================================================================
* FUNCTION : mrp_in_mim_oper_state_ac_stat1
*----------------------------------------------------------------------------
* PURPOSE  : operational MRP interconnection CBF for MIM called when the MIM
*            is in initial state (without any MRP interconnection link)
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of mrp interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mim_oper_state_ac_stat1 (MRP_IN_DB_TYPE *pDBin)
{
  LSA_BOOL rxFromDifferentIn, differentMac, differentInId;
  MRP_IN_MIM_DB_TYPE *pDBmim;

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBin->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  pDBmim = &pDBin->DBmim;

  MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: mrp_in_mim_oper_state_ac_stat1 entered: event=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.CurrentEvent);

  switch (pDBin->rxPara.CurrentEvent)
  {
    /*==========================================================================*/
    case MRP_IN_EVENT_T_NONE:
    {
      MRP_FATAL;
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_MAU_TYPE_CHG:
    {
      if (pDBin->rxPara.LinkState == MRP_LINK_STATE_UP)
      {
        MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: ac_stat1=>sec_up, (MAU_TYPE_CHG IPort=%u UP)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);

        mrp_in_mim_lnkPoll_req_send (pDBin, pDBmim->LNKSTATPollTcntCurrent_10ms, LSA_TRUE);

        mrp_in_write_current_in_open_diag (pDBin, MRP_IN_RED_LINK_IN_OPEN_SEC, LSA_FALSE);

        pDBmim->pInOperStateCbf = mrp_in_mim_oper_state_sec_up;
      }
      else /* LinkState == MRP_LINK_STATE_DOWN */
      {
        /* do nothing */
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_TPLG_CHG_TRF:
    {
      /* ignore */
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_TPLG_CHG_WRK:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort != pDBin->IPort)
        {
          rxFromDifferentIn = mrp_in_rx_from_different_interconn (pDBin, &differentMac, &differentInId);

          if (rxFromDifferentIn)
          {
            MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: ac_stat1=>ac_stat1, (IN_TC_WRK received from different MIM at port=%d, IPort=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);
            
            mrp_in_inTC_forward_to_mrp (pDBin);
          }
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_POLL_WRK:
    {
      /* ignore */
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_POLL_TRF:
    {
      /* ignore */
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_LNK_CHG_WRK:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort != pDBin->IPort)
        {
          if ((pDBin->rxPara.RxLnkRole & MRP_IN_PACKET_VAL_BITMASK_BIT0_SET) == MRP_IN_PACKET_VAL_LNK_ROLE_BIT0_PRM)
          {
            pDBmim->hasReceivedMicFrame = LSA_TRUE;
            
            mrp_in_mim_lnkPoll_timer_stop (pDBin);
            mrp_in_mim_init_proj_poll_params (pDBin);

            if (pDBin->rxPara.LinkState == MRP_LINK_STATE_UP)
            {
              MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: ac_stat1=>prm_up, (IN_LNK_CHG_WRK (LNK_UP) received at port=%d, IPort=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);
             
              mrp_in_write_current_in_open_diag (pDBin, MRP_IN_RED_LINK_IN_OPEN_PRM, LSA_FALSE);
              
              pDBmim->pInOperStateCbf = mrp_in_mim_oper_state_prm_up;
            }
            else /* LINK_DOWN received from PRM MIC */
            {
              MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIM[inInst=%d]: ac_stat1=>ac_stat1, (IN_LNK_CHG_WRK (LNK_DOWN) received at port=%d, IPort=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);
            }
          }
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_LNK_CHG_TRF:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort == pDBin->IPort)
        {
          if ((pDBin->rxPara.RxLnkRole & MRP_IN_PACKET_VAL_BITMASK_BIT0_SET) == MRP_IN_PACKET_VAL_LNK_ROLE_BIT0_PRM)
          {
            pDBmim->hasReceivedMicFrame = LSA_TRUE;

            mrp_in_mim_lnkPoll_timer_stop (pDBin);
            mrp_in_mim_init_proj_poll_params (pDBin);

            if (pDBin->rxPara.LinkState == MRP_LINK_STATE_UP)
            {
              MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: ac_stat1=>prm_up, (IN_LNK_CHG_TRF (LNK_UP) received at port=%d, IPort=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);

              mrp_in_write_current_in_open_diag (pDBin, MRP_IN_RED_LINK_IN_OPEN_PRM, LSA_FALSE);

              pDBmim->pInOperStateCbf = mrp_in_mim_oper_state_prm_up;
            }
            else /* LINK_DOWN received from PRM MIC */
            {
               MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIM[inInst=%d]: ac_stat1=>ac_stat1, (IN_LNK_CHG_TRF (LNK_DOWN) received at port=%d, IPort=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);
            }
          }
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_LNK_POLL_TIMER:
    {
      if (pDBmim->LNKSTATPollNReturnCurrent > 0)
      {
        MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIM[inInst=%d]: ac_stat1=>ac_stat1, (LNK_POLL TIMER expired IPort=%u UP, NReturn=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort, pDBmim->LNKSTATPollNReturnCurrent);

        mrp_in_mim_lnkPoll_req_send (pDBin, pDBmim->LNKSTATPollTcntCurrent_10ms, LSA_FALSE);

        pDBmim->LNKSTATPollNReturnCurrent--;
      }
      else /* pDBmim->LNKSTATPollNReturnCurrent == 0 */
      {
        mrp_in_mim_init_proj_poll_params (pDBin);
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_MRP_CHG_IND:
    {
      /* ignore */
    }
    break;

    /*==========================================================================*/
    default:
    {
      MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_FATAL, "MIM[inInst=%d]: mrp_in_mim_oper_state_ac_stat1: Invalid eventType=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.CurrentEvent);
    }
    break;
  }
}

/*===========================================================================
* FUNCTION : mrp_in_mim_proj_state_cbf
*----------------------------------------------------------------------------
* PURPOSE  : MRP interconnection CBF called when MRP interconnection
*            instance is provisioned to MIM
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of mrp interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mim_proj_state_cbf (MRP_IN_DB_TYPE *pDBin)
{
  MRP_IN_MIM_DB_TYPE *pDBmim;

  pDBmim = &pDBin->DBmim;

  MRP_PROGRAM_TRACE_02 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: mrp_in_mim_proj_state_cbf entered: event=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.CurrentEvent);

  if (pDBin->rxPara.CurrentEvent == MRP_IN_EVENT_T_IN_TC_TIMER) /* catch the InTC timer event here, because it is independent from operational MIM state */
  {
    mrp_in_mim_inTC_req_send (pDBin->pChSys, pDBin->pInstance[MRP_IN_INSTANCE_INDEX]);
  }
  else
  {
    /* route the MIM related event to the corresponding MIM operational state */
    pDBmim->pInOperStateCbf (pDBin);

    /* diag events are set in the MIM state machine and have to be indicated here */
    mrp_in_ll_in_event_handler (pDBin);
  }
}


/*===========================================================================
* FUNCTION : mrp_in_mim_link_chg_cbf
*----------------------------------------------------------------------------
* PURPOSE  : MIM link change handling
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference
*            instance = affected MRP instance
*            port     = 1..RED_NO_OF_PORTS
*            linkUp   = true=>link up false=>link down
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_UINT8 mrp_in_mim_link_chg_cbf (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT16 port, MRP_LINK_ADD_INFO addInfo, LSA_UINT8 linkUp)
{
  LSA_UINT16 inInstance;
  MRP_IN_EVENT_DB_TYPE DBinEv;
  MRP_IN_EVENT_DB_TYPE *pDBinEv = &DBinEv;

  LSA_UNUSED_ARG(instance);   /* hhr: to please the compiler in case traces are disabled. */

  inInstance = mrp_in_sys_map_iport_to_inInstance (pChSys, port);

  MRP_MEMSET (pDBinEv, 0, sizeof (MRP_IN_EVENT_DB_TYPE));

  if (inInstance != MRP_IN_INSTANCE_NONE)
  {
    pDBinEv->pChSys      = pChSys;
    pDBinEv->evInstance  = inInstance;
    pDBinEv->RxPort      = port;
    pDBinEv->LinkAddInfo = addInfo;

    if (linkUp)
    {
      MRP_PROGRAM_TRACE_03 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIM_LINK_UP[inst=%d][inInst=%d] at IPort=%u", instance, inInstance, port);
      pDBinEv->LinkState = MRP_LINK_STATE_UP;
    }
    else
    {
      MRP_PROGRAM_TRACE_03 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIM_LINK_DOWN[inst=%d][inInst=%d] at IPort=%u", instance, inInstance, port);
      pDBinEv->LinkState = MRP_LINK_STATE_DOWN;
    }
    mrp_in_update (pDBinEv, MRP_IN_EVENT_T_MAU_TYPE_CHG);

    return LSA_TRUE;
  }

  return LSA_FALSE;
}


/*===========================================================================
* FUNCTION : mrp_in_mim_get_state
*----------------------------------------------------------------------------
* PURPOSE  : returns interconnection state
*----------------------------------------------------------------------------
* RETURNS  : MRP_RET_OK
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference
*            inInstance = affected MRP interconnection instance
* OUTPUTS  : *pInOpen = interconnection state (LSA_TRUE = interconnection open, LSA_FALSE = interconnection closed)
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mim_get_state (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 inInstance, LSA_UINT8 *pInOpen)
{
  LSA_UINT16 mrpInMaxInstance;
  MRP_IN_DB_TYPE *pDBin;
  MRP_IN_MIM_DB_TYPE *pDBmim;
  MRP_IN_DB_TYPE *pDBinBase = (MRP_IN_DB_TYPE *)pChSys->pChSysDB->pMrpIn;

  *pInOpen = LSA_TRUE;
  mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pChSys);

  MRP_ASSERT (inInstance <= mrpInMaxInstance);

  if (mrpInMaxInstance)
  {
    pDBin = &pDBinBase[inInstance];
    pDBmim = &pDBin->DBmim;

    if (pDBmim->runState == MRP_IN_OPERATION_ON)
    {
      if (pDBmim->pInOperStateCbf == mrp_in_mim_oper_state_chk_ic)
        *pInOpen = LSA_FALSE;
    }
  }
}



/*===========================================================================
* FUNCTION : mrp_in_mim_get_params
*----------------------------------------------------------------------------
* PURPOSE  : returns mim params
*----------------------------------------------------------------------------
* RETURNS  : -
*----------------------------------------------------------------------------
* INPUTS   : pChSys     = channel reference
*            inInstance = affected MRP interconnection instance
* OUTPUTS  : *pParams   = pointer to params struct with current MIM params
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mim_get_params (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 inInstance, MRP_IN_MIM_PARAMS_TYPE *pParams)
{
  LSA_UINT16 mrpInMaxInstance;
  MRP_IN_DB_TYPE *pDBin;
  MRP_IN_DB_TYPE *pDBinBase = (MRP_IN_DB_TYPE *)pChSys->pChSysDB->pMrpIn;

  pDBin = &pDBinBase[inInstance];

  mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pChSys);

  MRP_ASSERT (inInstance <= mrpInMaxInstance);

  if (mrpInMaxInstance)
  {
    pParams->runState = pDBin->DBmim.runState;

    MRP_PROGRAM_TRACE_02 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIM[inInst=%d]: mrp_in_mim_get_params: mode=%d retrieved", inInstance, pParams->runState);

    pParams->IPort = pDBin->IPort;
    pParams->InID  = pDBin->InID;

    pParams->MRPIC_TOPchgT_10ms         = pDBin->DBmim.MRPIC_TOPchgT_10ms;
    pParams->MRPIC_TOPNRmax             = pDBin->DBmim.MRPIC_TOPNRmax;
    pParams->MRPIC_LinkStatusChangeT_ms = pDBin->DBmim.MRPIC_LinkStatusChangeT_ms;
    pParams->MRPIC_LinkStatusNRmax      = pDBin->DBmim.MRPIC_LinkStatusNRmax;

    pParams->MRPIC_StartDelay_100ms     = pDBin->DBmim.MRPIC_StartDelay_100ms;

    pParams->MRPIC_CheckDomainID        = pDBin->DBmim.MRPIC_CheckDomainID;
  }
  else
  {
    MRP_MEMSET (pParams, 0, sizeof (MRP_IN_MIM_PARAMS_TYPE));
    MRP_PROGRAM_TRACE_00 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "MIM: mrp_in_mim_get_params called while mrpInMaxInst=0");
  }
}


/*===========================================================================
* FUNCTION : mrp_in_mim_get_module_state
*----------------------------------------------------------------------------
* PURPOSE  : get all important module params to reflect module state of MRC
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChUsr       = user channel reference
*            instance     = affected MRP/HSR instance
*            inInstance   = MRP interconnection instance
*            addStop      = additional stop of module required
* OUTPUTS  : *moduleState = module state data of this instance
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mim_get_module_state (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance, LSA_UINT16 inInstance, MRP_REC_MODULE_STATE_TYPE *pModuleState, LSA_BOOL addStop)
{
  LSA_UINT16 mrpInMaxInstance;
  MRP_IN_MIM_PARAMS_TYPE paramsMIMRead;
  MRP_REC_DB_INST_SET_TYPE *pRecInstWork;
  MRP_REC_DB_TYPE *pDBrec = (MRP_REC_DB_TYPE *)pChUsr->pChUsrDB->pRec;

  mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pChUsr);

  if (mrpInMaxInstance && !instance)
  {
    mrp_in_mim_get_params (pDBrec->pChSys, inInstance, &paramsMIMRead);

    pRecInstWork = &pDBrec->pInstData[instance].work;

    pModuleState->topoCtrl = MRP_USR_ENUM_TOPO_CTRL_NONE;
    pModuleState->configured = (pRecInstWork->pRecIcPortAdjust[inInstance].Role == MRP_IC_PROJ_ROLE_MGR);

    if (pModuleState->configured)
    {
      if ((pRecInstWork->pRecIcPortCheck[inInstance].Check & MRP_REC_IC_PORT_DATA_CHECK_DOMAINID) && (paramsMIMRead.MRPIC_CheckDomainID == MRP_IC_CHECK_DOMAINID_OFF))
      {
        pModuleState->topoCtrl = MRP_USR_ENUM_TOPO_CTRL_ADD_CHECK;
      }
      if (!(pRecInstWork->pRecIcPortCheck[inInstance].Check & MRP_REC_IC_PORT_DATA_CHECK_DOMAINID) && (paramsMIMRead.MRPIC_CheckDomainID == MRP_IC_CHECK_DOMAINID_ON))
      {
        pModuleState->topoCtrl = MRP_USR_ENUM_TOPO_CTRL_ADD;
      }
    }
    pModuleState->rpChanged = (pRecInstWork->pRecIcPortAdjust[inInstance].PortID != paramsMIMRead.IPort);

    pModuleState->paramChanged = (pModuleState->rpChanged
                                  || (pRecInstWork->pRecIcPortAdjust[inInstance].DomainID                 != paramsMIMRead.InID)
                                  || (pRecInstWork->pRecIcPortAdjust[inInstance].mim.LinkStatusChangeT_ms != paramsMIMRead.MRPIC_LinkStatusChangeT_ms)
                                  || (pRecInstWork->pRecIcPortAdjust[inInstance].mim.LinkStatusNRmax      != paramsMIMRead.MRPIC_LinkStatusNRmax)
                                  || (pRecInstWork->pRecIcPortAdjust[inInstance].mim.TOPchgT_10ms         != paramsMIMRead.MRPIC_TOPchgT_10ms)
                                  || (pRecInstWork->pRecIcPortAdjust[inInstance].mim.TOPNRmax             != paramsMIMRead.MRPIC_TOPNRmax)
                                  || (pRecInstWork->pRecIcPortAdjust[inInstance].mim.StartDelay_100ms     != paramsMIMRead.MRPIC_StartDelay_100ms)
                                  || addStop);

    pModuleState->running = (paramsMIMRead.runState == MRP_IN_OPERATION_ON);
  }
  else
  {
    pModuleState->topoCtrl     = MRP_USR_ENUM_TOPO_CTRL_NONE;
    pModuleState->configured   = LSA_FALSE;
    pModuleState->rpChanged    = LSA_FALSE;
    pModuleState->paramChanged = LSA_FALSE;
    pModuleState->running      = LSA_FALSE;
  }
  /* NOTE: If morphing for MRP_IN is required, the restriction needs to be set like for MRP:  pModuleState->roleChangeRestricted = (roleMgr && !ringOpen); */
  pModuleState->roleChangeRestricted = LSA_FALSE;

  /*  stop := run && !configured || restart */
  pModuleState->stop = ((pModuleState->running && !pModuleState->configured) || (pModuleState->running && pModuleState->paramChanged));
  /*  start := !run && configured || restart */
  pModuleState->start = ((!pModuleState->running && pModuleState->configured) || (pModuleState->configured && pModuleState->running && pModuleState->paramChanged));
}


/*===========================================================================
* FUNCTION : mrp_in_mim_init_add_poll_params
*----------------------------------------------------------------------------
* PURPOSE  : initialize MIM lnkStatusPoll params for an additional InPoll request
*            to acquire the PRM link state
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin   = pointer to one instance of MRP interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mim_init_add_poll_params (MRP_IN_DB_TYPE *pDBin)
{
  MRP_IN_MIM_DB_TYPE *pDBmim;

  pDBmim = &pDBin->DBmim;

  /* initialize additional MIM poll parameters to force the determination of the PRM link state */
  pDBmim->LNKSTATPollTcntCurrent_10ms = pDBmim->LNKSTATPollChgTcnt_Proj_10ms;

  pDBmim->LNKSTATPollNRmaxCurrent     = MRP_IN_DEFAULT_VAL_IN_ADD_POLL_NR_MAX;
  pDBmim->LNKSTATPollNReturnCurrent   = pDBmim->LNKSTATPollNRmaxCurrent - 1;

  MRP_PROGRAM_TRACE_03 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: mrp_in_mim_init_add_poll_params: LNKSTATPollTcntCurrent_10ms=%d, LNKSTATPollNRmaxCurrent=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmim->LNKSTATPollTcntCurrent_10ms, pDBmim->LNKSTATPollNRmaxCurrent);
}


/*===========================================================================
* FUNCTION : mrp_in_mim_init_slow_poll_params
*----------------------------------------------------------------------------
* PURPOSE  : initialize MIM lnkStatusPoll params for slow poll intervals (100 ms)
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin   = pointer to one instance of MRP interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mim_init_slow_poll_params (MRP_IN_DB_TYPE *pDBin)
{
  MRP_IN_MIM_DB_TYPE *pDBmim;

  pDBmim = &pDBin->DBmim;

  /* initialize the MIM startup poll parameters */
  pDBmim->LNKSTATPollTcntCurrent_10ms = pDBmim->LNKSTATPollChgTcnt_Slow_10ms;

  pDBmim->LNKSTATPollNRmaxCurrent     = pDBmim->MRPIC_StartDelay_100ms;
  pDBmim->LNKSTATPollNReturnCurrent   = pDBmim->LNKSTATPollNRmaxCurrent - 1;

  MRP_PROGRAM_TRACE_03 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: mrp_in_mim_init_slow_poll_params: LNKSTATPollTcntCurrent_10ms=%d, LNKSTATPollNRmaxCurrent=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmim->LNKSTATPollTcntCurrent_10ms, pDBmim->LNKSTATPollNRmaxCurrent);
}


/*===========================================================================
* FUNCTION : mrp_in_mim_init_proj_poll_params
*----------------------------------------------------------------------------
* PURPOSE  : initialize MIM lnkStatusPoll params conforming to provisioning
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin   = pointer to one instance of MRP interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mim_init_proj_poll_params (MRP_IN_DB_TYPE *pDBin)
{
  MRP_IN_MIM_DB_TYPE *pDBmim;

  pDBmim = &pDBin->DBmim;

  /* initialize the normal MIM poll parameters */
  pDBmim->LNKSTATPollTcntCurrent_10ms = pDBmim->LNKSTATPollChgTcnt_Proj_10ms;

  pDBmim->LNKSTATPollNRmaxCurrent     = pDBmim->MRPIC_LinkStatusNRmax;
  pDBmim->LNKSTATPollNReturnCurrent   = pDBmim->LNKSTATPollNRmaxCurrent - 1;

  MRP_PROGRAM_TRACE_03 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: mrp_in_mim_init_proj_poll_params: LNKSTATPollTcntCurrent_10ms=%d, LNKSTATPollNRmaxCurrent=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmim->LNKSTATPollTcntCurrent_10ms, pDBmim->LNKSTATPollNRmaxCurrent);
}


/*===========================================================================
* FUNCTION : mrp_in_mim_init_start_delay
*----------------------------------------------------------------------------
* PURPOSE  : initialize MIM start delay variables
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin   = pointer to one instance of MRP interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mim_init_start_delay (MRP_IN_DB_TYPE *pDBin)
{
  MRP_IN_MIM_DB_TYPE *pDBmim;

  pDBmim = &pDBin->DBmim;

  /* calculate the MIM startup parameters */
  if (pDBmim->MRPIC_StartDelay_100ms <= MRP_IN_START_DELAY_LOW_LIMIT)
  {
    mrp_in_mim_init_proj_poll_params (pDBin);
    pDBmim->hasToWait4MicFrame = LSA_FALSE;
  }
  else
  {
    if (pDBmim->MRPIC_StartDelay_100ms >= MRP_IN_START_DELAY_MEDIUM_LIMIT_LOW && pDBmim->MRPIC_StartDelay_100ms <= MRP_IN_START_DELAY_MEDIUM_LIMIT_HIGH)
    {
      mrp_in_mim_init_slow_poll_params (pDBin);
      pDBmim->hasToWait4MicFrame = LSA_FALSE;
    }
    else
    {
      mrp_in_mim_init_proj_poll_params (pDBin);
      pDBmim->hasToWait4MicFrame = LSA_TRUE;
    }
  }

  MRP_PROGRAM_TRACE_03 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: mrp_in_mim_init_start_delay: MRPIC_StartDelay_100ms=%d, hasToWait4MIC=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmim->MRPIC_StartDelay_100ms, pDBmim->hasToWait4MicFrame);
}


/*===========================================================================
* FUNCTION : mrp_in_mim_init_var_from_params
*----------------------------------------------------------------------------
* PURPOSE  : initialize MIM variables if default setting shall not be used
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin   = pointer to one instance of MRP interconnection database
*            *pParams = pointer to params for MIM initial setup
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_VOID mrp_in_mim_init_var_from_params (MRP_IN_DB_TYPE *pDBin, MRP_IN_MIM_PARAMS_TYPE *pParams)
{
  MRP_IN_MIM_DB_TYPE *pDBmim;

  pDBmim = &pDBin->DBmim;

  /* set new values */
  MRP_MEMCPY (pDBin->DomainUUID, pParams->DomainUUID, sizeof (pDBin->DomainUUID));

  pDBmim->MRPIC_TOPchgT_10ms         = pParams->MRPIC_TOPchgT_10ms;
  pDBmim->MRPIC_TOPNRmax             = pParams->MRPIC_TOPNRmax;
  pDBmim->MRPIC_LinkStatusChangeT_ms = pParams->MRPIC_LinkStatusChangeT_ms;
  pDBmim->MRPIC_LinkStatusNRmax      = pParams->MRPIC_LinkStatusNRmax;
  pDBmim->MRPIC_StartDelay_100ms     = pParams->MRPIC_StartDelay_100ms;

  pDBmim->MRPIC_CheckDomainID        = pParams->MRPIC_CheckDomainID;

  pDBmim->TOPintervalTcnt            = (LSA_UINT16)((pParams->MRPIC_TOPchgT_10ms * MRP_IN_DEFAULT_VAL_IN_TC_CHG_T_RES_MS) / MRP_TIMER_TIME_BASE_DEZ);
  pDBmim->TOPNReturn                 = pParams->MRPIC_TOPNRmax; 
  /* NOTE: InTC is repeated MRPIC_TOPNRmax times */
  /* first InTC is sent with timestamp SetupInterconnTopologyChangeReq(MRP_IN_TOPNRmax X time) -> number of sent InTC frames per port: MRPIC_TOPNRmax + 1 */

  pDBmim->LNKSTATPollChgTcnt_Proj_10ms = (LSA_UINT16)(pParams->MRPIC_LinkStatusChangeT_ms / MRP_TIMER_TIME_BASE_DEZ);

  mrp_in_mim_init_start_delay (pDBin);

  /* takeover MIM roles here, because no default setting is requested, but probably a new role has to be activated */
  pDBin->adminRole = pParams->adminRole;
  pDBin->operRole  = pParams->operRole;
  pDBin->IPort     = pParams->IPort;
  pDBin->InID      = pParams->InID;

  MRP_PROGRAM_TRACE_03 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: mrp_in_mim_init_var_from_params: pDBmim written, adminRole=%d, operRole=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->adminRole, pDBin->operRole);
}


/*===========================================================================
* FUNCTION : mrp_in_mim_init_var
*----------------------------------------------------------------------------
* PURPOSE  : initialize variables
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin   = pointer to one instance MRP interconnection database
*            *pParams = pointer to params for MIM initial setup
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mim_init_var (MRP_IN_DB_TYPE *pDBin, MRP_IN_MIM_PARAMS_TYPE *pParams)
{
  MRP_IN_MIM_DB_TYPE *pDBmim;

  MRP_PROGRAM_TRACE_01 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIM[inInst=%d]: mrp_in_mim_init_var entered", pDBin->pInstance[MRP_IN_INSTANCE_INDEX]);

  pDBmim = &pDBin->DBmim;

  /* variables */
  MRP_MEMSET (pDBmim, 0, sizeof (pDBin->DBmim));

  pDBin->IPort     = 0;
  pDBin->InID      = MRP_IN_DEFAULT_VAL_IN_ID;

  pDBin->adminRole = pParams->adminRole;
  pDBin->operRole  = pParams->operRole;

  pDBin->rxPara.Vlan                = 0;

  pDBmim->LNKSTATPollNReturnCurrent = 0;
  pDBmim->TOPNReturn                = 0;
  pDBmim->addPoll                   = LSA_FALSE;
  pDBmim->isMorphing                = LSA_FALSE;
  pDBmim->pChUsr4NextRole           = LSA_NULL;

  MRP_PROGRAM_TRACE_03 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: mrp_in_mim_init_var: pDBmim written, adminRole=%d, operRole=%d, morphing=OFF", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->adminRole, pDBin->operRole);

  /* internals */
  pDBin->rxPara.CurrentEvent = MRP_IN_EVENT_T_NONE;
  pDBmim->CurrentEvent       = MRP_IN_EVENT_T_NONE;

  pDBmim->pInOperStateCbf = mrp_in_mim_oper_state_ac_stat1;

  if (pParams->SoftStart == MRP_SET_SOFT_START_ON)
  {
    MRP_PROGRAM_TRACE_01 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "MIM[inInst=%d]: mrp_in_mim_init_var: SoftStart=ON results to state AC_STAT1", pDBin->pInstance[MRP_IN_INSTANCE_INDEX]);
    pDBmim->pInOperStateCbf = mrp_in_mim_oper_state_ac_stat1;
  }
  /* event attributes */
  pDBin->rxPara.LinkState    = MRP_LINK_STATE_DOWN;
  pDBin->rxPara.RxPort       = 0;

  MRP_MEMSET (pDBin->rxPara.RxSA, 0, MRP_PACKET_VAL_MAC_ADDR_LEN);
  MRP_MEMSET (pDBin->rxPara.RxPeerPortMacAddr, 0, MRP_PACKET_VAL_MAC_ADDR_LEN);
  MRP_MEMSET (pDBin->rxPara.RxUUID, 0, 16);

  /* parameters */
  /* content of record PDPortMrpIcDataAdjust: field MRPIC_StartDelay */
  pDBmim->MRPIC_TOPchgT_10ms         = MRP_IN_DEFAULT_VAL_IN_TC_CHG_DEFAULT_T_MS / MRP_TIMER_TIME_BASE_DEZ;
  pDBmim->MRPIC_TOPNRmax             = MRP_IN_DEFAULT_VAL_IN_TC_NR_MAX;
  pDBmim->MRPIC_LinkStatusChangeT_ms = MRP_IN_DEFAULT_VAL_IN_POLL_DEFAULT_T_20MS;
  pDBmim->MRPIC_LinkStatusNRmax      = MRP_IN_DEFAULT_VAL_IN_POLL_NR_MAX;
  pDBmim->MRPIC_StartDelay_100ms     = MRP_IN_DEFAULT_VAL_START_DELAY_ENDLESS;
  
  /* content of record PDPortMrpIcDataCheck */
  pDBmim->MRPIC_CheckDomainID     = MRP_IC_CHECK_DOMAINID_OFF;

  pDBmim->TOPNReturn              = pDBmim->MRPIC_TOPNRmax;
  /* NOTE: InTC is repeated MRPIC_TOPNRmax times */
  /* first InTC is sent with timestamp SetupInterconnTopologyChangeReq(MRP_IN_TOPNRmax X time) -> number of sent InTC frames per port: MRPIC_TOPNRmax + 1 */

  pDBmim->TOPintervalTcnt         = (LSA_UINT16)((MRP_IN_DEFAULT_VAL_IN_TC_CHG_T_CNT * MRP_IN_DEFAULT_VAL_IN_TC_CHG_T_RES_MS) / MRP_TIMER_TIME_BASE_DEZ);

  pDBmim->LNKSTATPollChgTcnt_Slow_10ms = MRP_IN_DEFAULT_VAL_IN_POLL_SLOW_T_100MS / MRP_TIMER_TIME_BASE_DEZ;
  pDBmim->LNKSTATPollChgTcnt_Proj_10ms = MRP_IN_DEFAULT_VAL_IN_POLL_DEFAULT_T_20MS / MRP_TIMER_TIME_BASE_DEZ;

  mrp_in_mim_init_proj_poll_params (pDBin);

  pDBmim->hasToWait4MicFrame           = LSA_TRUE;
  pDBmim->hasReceivedMicFrame          = LSA_FALSE;

  if (pParams->SetDefaults == MRP_SET_DEFAULTS_OFF)
  {
    /* set new values */
    mrp_in_mim_init_var_from_params (pDBin, pParams);
  }
}


/*===========================================================================
* FUNCTION : mrp_in_mim_start
*----------------------------------------------------------------------------
* PURPOSE  : start one MIM instance
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys     = channel reference
*            inInstance = MRP interconnection instance to be started
*            *pParams   = pointer to MIM params to be used for start
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mim_start (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 inInstance, MRP_IN_MIM_PARAMS_TYPE *pParams)
{
  MRP_IN_DB_TYPE *pDBin;
  MRP_IN_DB_TYPE *pDBinBase = (MRP_IN_DB_TYPE *)pChSys->pChSysDB->pMrpIn;

  pDBin = &pDBinBase[inInstance];

  MRP_PROGRAM_TRACE_01 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: mrp_in_mim_start entered", inInstance);

  mrp_in_write_red_start_state (pDBin, MRP_USR_ENUM_STATE_IN_MIM, pParams->InID, pParams->IPort, LSA_TRUE);
  mrp_ll_red_state_handler (pChSys, pDBin->pInstance[MRP_INSTANCE_INDEX]);

  mrp_in_mim_init_var (pDBin, pParams);

  pDBin->DBmim.runState = MRP_IN_OPERATION_ON;
  pDBin->operRole       = MRP_IN_OPER_ROLE_MIM;

  pDBin->pInProjStateBaseCbf = mrp_in_mim_proj_state_cbf;

  mrp_edd_set_link_chg_callback (pChSys, MRP_DEFAULT_INSTANCE, mrp_in_mim_link_chg_cbf);
  mrp_in_mim_set_mrp_ring_chg_ind_callback (pDBin, mrp_in_mim_mrp_ring_chg_ind_cbf);

  mrp_in_ll_init_modul_mim (pDBin, pParams);
  mrp_in_ll_in_trf_chg_host (pDBin, LSA_TRUE);

  if (!pDBin->isStartedFromMrm) /* if an MRM instance is active, then HOST_FWD for IN_CTRL is already activated */
  {
    mrp_in_ll_in_ctrl_chg_host (pDBin, LSA_TRUE);
  }

  mrp_in_mim_lnkPoll_req_send (pDBin, pDBin->DBmim.LNKSTATPollTcntCurrent_10ms, LSA_FALSE);

  mrp_in_write_current_event_prm_info (pDBin, MRP_IN_OPER_ROLE_MIM, MRP_IN_RED_EVT_PARA_DONT_CARE);
  mrp_in_write_current_event_diag (pDBin, MRP_IN_RED_DIAG_IN_ALL, LSA_FALSE);

  mrp_in_write_current_event_diag (pDBin, MRP_IN_RED_DIAG_IN_OPEN_PRM_MD, LSA_TRUE);
  mrp_in_write_current_event_diag (pDBin, MRP_IN_RED_DIAG_IN_OPEN_SEC_MD, LSA_TRUE);

  mrp_in_ll_in_event_handler (pDBin);

  MRP_PROGRAM_TRACE_01 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: mrp_in_mim_start exit", inInstance);
}

/*===========================================================================
* FUNCTION : mrp_in_mim_stop
*----------------------------------------------------------------------------
* PURPOSE  : stop one MIM instance
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys     = channel reference
*            inInstance = MRP interconnection instance to be started
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mim_stop (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 inInstance)
{
  MRP_IN_MIM_PARAMS_TYPE params;
  MRP_IN_DB_TYPE *pDBin;
  LSA_UINT8 peerMacAddr[] = { 0, 0, 0, 0, 0, 0 };
  MRP_IN_DB_TYPE *pDBinBase = (MRP_IN_DB_TYPE *)pChSys->pChSysDB->pMrpIn;

  pDBin = &pDBinBase[inInstance];

  MRP_PROGRAM_TRACE_01 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: mrp_in_mim_stop entered", inInstance);

  mrp_in_write_red_start_state (pDBin, MRP_USR_ENUM_STATE_IN_MIM, pDBin->InID, pDBin->IPort, LSA_FALSE);
  mrp_ll_red_state_handler (pChSys, pDBin->pInstance[MRP_INSTANCE_INDEX]);

  mrp_in_write_current_event_diag (pDBin, MRP_IN_RED_DIAG_IN_ALL, LSA_FALSE);
  mrp_in_write_current_event_peer_macaddr (pDBin, peerMacAddr);
  mrp_in_write_current_event_prm_info (pDBin, MRP_IN_OPER_ROLE_OFF, MRP_IN_RED_EVT_PARA_DONT_CARE);

  mrp_in_ll_in_event_handler (pDBin);

  mrp_in_mim_inTC_timer_stop (pDBin);
  mrp_in_mim_lnkPoll_timer_stop (pDBin);

  pDBin->pInProjStateBaseCbf = mrp_in_proj_state_off_cbf;

  mrp_edd_remove_link_chg_callback (pChSys, MRP_DEFAULT_INSTANCE, mrp_in_mim_link_chg_cbf);
  mrp_in_mim_remove_mrp_ring_chg_ind_callback (pDBin);

  if (!mrp_in_check_active_instance(pChSys, inInstance)) /* if another MRP_IN is active, then HOST_FWD for IN_TRF has to be kept activated */
  {
    mrp_in_ll_in_trf_chg_host (pDBin, LSA_FALSE);
  }

  if (!pDBin->isStartedFromMrm && !mrp_in_check_active_instance(pChSys, inInstance)) /* if an MRM instance or another MRP_IN is active, then HOST_FWD for IN_CTRL has to be kept activated */
  {
    mrp_in_ll_in_ctrl_chg_host (pDBin, LSA_FALSE);
  }

  mrp_in_ll_deinit_modul_mim (pDBin);

  params.SetDefaults = MRP_SET_DEFAULTS_ON;
  params.SoftStart   = MRP_SET_SOFT_START_OFF;

  params.adminRole   = MRP_IN_ADMIN_ROLE_OFF;
  params.operRole    = MRP_IN_OPER_ROLE_OFF;

  mrp_in_mim_init_var (pDBin, &params);

  pDBin->DBmim.runState = MRP_IN_OPERATION_OFF;

  MRP_PROGRAM_TRACE_01 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "MIM[inInst=%d]: mrp_in_mim_stop exit", inInstance);
}
