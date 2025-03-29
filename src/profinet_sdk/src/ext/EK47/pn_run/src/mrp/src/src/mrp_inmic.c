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
/*  F i l e               &F: mrp_inmic.c                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Functionality for MRP interconnection client (MIC)                       */ 
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef MRP_MESSAGE

/*  2017-11-15  ds    initial version of file introduced                     */
/*  2018-10-19  hh    introduction of N MRP interconnection instances        */
/*                                                                           */
/*                                                                           */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID   42
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

const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_in_init_mic[] =
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
* FUNCTION : mrp_in_ll_init_modul_mic
*----------------------------------------------------------------------------
* PURPOSE  : initialize EDD settings of one MIC instance
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
static LSA_VOID mrp_in_ll_init_modul_mic (const MRP_IN_DB_TYPE *pDBin, const MRP_IN_MIC_PARAMS_TYPE *pParams)
{
  MRP_EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE pPSParam;
  MRP_EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE pPSParamTplgChgStart;
  MRP_EDD_UPPER_SWI_FLUSH_FILTERING_DB_PTR_TYPE pFlushFilterParam;
  LSA_UINT16 portIdx;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pDBin->pChSys->pChSysDB->pEdd;

  MRP_LOWER_TRACE_01 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_ll_init_modul_mic[inInst=%d] entered", pDBin->pInstance[MRP_IN_INSTANCE_INDEX]);

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
  /* structure is already initialized by MRP init */
  pPSParamTplgChgStart->PortIDState[portIdx] = EDD_PORT_STATE_BLOCKING;

  mrp_ll_in_burst_req (pDBin->pChSys, pDBin->pInstance, g_MrpllReqTbl_mrp_in_init_mic, LSA_TRUE);
}

/*===========================================================================
* FUNCTION : mrp_in_ll_deinit_modul_mic
*----------------------------------------------------------------------------
* PURPOSE  : deinitialize EDD settings of one MIC instance
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   :  pDBin    = pointer to one MRP interconnection instance
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_VOID mrp_in_ll_deinit_modul_mic (const MRP_IN_DB_TYPE *pDBin)
{
  MRP_EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE pPSParam;
  MRP_EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE pPSParamTplgChgStart;
  MRP_EDD_UPPER_SWI_FLUSH_FILTERING_DB_PTR_TYPE pFlushFilterParam;
  LSA_UINT16 pos, k, numOfPorts;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pDBin->pChSys->pChSysDB->pEdd;

  MRP_LOWER_TRACE_01 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_ll_deinit_modul_mic[inInst=%d] entered", pDBin->pInstance[MRP_IN_INSTANCE_INDEX]);

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
  MRP_LOWER_TRACE_02 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_ll_deinit_modul_mic[inInst=%d] IPort:%d", 
                      pDBin->pInstance[MRP_IN_INSTANCE_INDEX],
                      pDBin->IPort);
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
* FUNCTION : mrp_in_mic_rx_inPoll_valid_at_this_IPort
*----------------------------------------------------------------------------
* PURPOSE  : plausibility check to match RxPortRole of received InPoll frame
*            to MICPosition provisioning info
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin        = internal MRP interconnection data base reference
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_BOOL mrp_in_mic_rx_inPoll_valid_at_this_IPort (const MRP_IN_DB_TYPE *pDBin)
{
  LSA_BOOL validFrame = LSA_TRUE;

  if (pDBin->DBmic.MRPIC_MICPosition == MRP_IN_DEFAULT_VAL_IN_PRM_MIC_POS)
  {
    if (pDBin->rxPara.RxPortRole == MRP_IN_PACKET_VAL_PORT_ROLE_IN)
      validFrame = LSA_FALSE;

  }
  else /* SEC coupled MIC */
  {
    if (pDBin->rxPara.RxPortRole != MRP_IN_PACKET_VAL_PORT_ROLE_IN)
      validFrame = LSA_FALSE;
  }

  return validFrame;
}


/*===========================================================================
* FUNCTION : mrp_in_mic_lnkChg_down_timer_start
*----------------------------------------------------------------------------
* PURPOSE  : start LNK_CHG_DOWN timer
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin        = internal MRP interconnection data base reference
*            durationTicks = time in ticks
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_VOID mrp_in_mic_lnkChg_down_timer_start (const MRP_IN_DB_TYPE *pDBin, LSA_UINT32 durationTicks)
{
  mrp_sys_timer_stop (pDBin->TimerIdInLnkChgDownSend);
  mrp_sys_timer_start (pDBin->TimerIdInLnkChgDownSend, durationTicks);
}


/*===========================================================================
* FUNCTION : mrp_in_mic_lnkChg_down_timer_stop
*----------------------------------------------------------------------------
* PURPOSE  : stop LNK_CHG_DOWN timer
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = internal MRP interconnection data base reference
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_VOID mrp_in_mic_lnkChg_down_timer_stop (const MRP_IN_DB_TYPE *pDBin)
{
  mrp_sys_timer_stop (pDBin->TimerIdInLnkChgDownSend);
}

/*===========================================================================
* FUNCTION : mrp_in_mic_lnkChg_up_timer_start
*----------------------------------------------------------------------------
* PURPOSE  : start LNK_CHG_UP timer
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin        = internal MRP interconnection data base reference
*            durationTicks = time in ticks
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_VOID mrp_in_mic_lnkChg_up_timer_start (const MRP_IN_DB_TYPE *pDBin, LSA_UINT32 durationTicks)
{
  mrp_sys_timer_stop (pDBin->TimerIdInLnkChgUpSend);
  mrp_sys_timer_start (pDBin->TimerIdInLnkChgUpSend, durationTicks);
}


/*===========================================================================
* FUNCTION : mrp_in_mic_lnkChg_up_timer_stop
*----------------------------------------------------------------------------
* PURPOSE  : stop LNK_CHG_UP timer
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = internal MRP interconnection data base reference
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_VOID mrp_in_mic_lnkChg_up_timer_stop (const MRP_IN_DB_TYPE *pDBin)
{
  mrp_sys_timer_stop (pDBin->TimerIdInLnkChgUpSend);
}


/*===========================================================================
* FUNCTION : mrp_in_mic_lnkChg_req_send
*----------------------------------------------------------------------------
* PURPOSE  : send a LNK_CHG frame to notify the MIM about an IPort state change
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin        = internal MRP interconnection data base reference
*            instance      = affected MRP instance
*            linkState     = MRP_LINK_STATE_UP, MRP_LINK_STATE_DOWN
*            durationTicks = time in ticks
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_VOID mrp_in_mic_lnkChg_req_send (MRP_IN_DB_TYPE *pDBin, MRP_LINK_STATE_TYPE linkState, LSA_UINT32 durationTicks)
{
  MRP_IN_PACKET_STRUCT_LNK_CHG_TYPE *pFrame;
  LSA_UINT16 time;
  MRP_IN_TX_REQ_INFO_TYPE *pInLnkChgTxInfo;
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBin->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */
  

  MRP_EDD_LOWER_RQB_PTR_TYPE pRQB = LSA_NULL;

  MRP_ASSERT ((MRP_LINK_STATE_UP == linkState) || (MRP_LINK_STATE_DOWN == linkState));

  if (pDBin->DBmic.MRPIC_MICPosition == MRP_IN_DEFAULT_VAL_IN_PRM_MIC_POS) /* only send InLnkChg frames, if PRM MIC */
  {
    if (pDBin->pChSys->state != MRP_CH_STATE_CLOSING)
    {
      pInLnkChgTxInfo = &pDBin->InLnkChgTxInfo;

      time = (LSA_UINT16)(durationTicks * MRP_TIMER_TIME_BASE_DEZ);

      pInLnkChgTxInfo->inInstance     = pDBin->pInstance[MRP_IN_INSTANCE_INDEX];
      pInLnkChgTxInfo->inId           = pDBin->InID;
      pInLnkChgTxInfo->inFrameType    = MRP_IN_FRAME_TYPE_LINK_CHANGE;
      pInLnkChgTxInfo->inLinkState    = linkState;
      pInLnkChgTxInfo->intervalTimeMs = time;
      pInLnkChgTxInfo->inLinkRole     =   (LSA_UINT16) ((((LSA_UINT16)pDBin->LinkAddInfo.AddCheckEnabled) << MRP_IN_PACKET_VAL_LNK_ADD_CHECK_BIT_OFFSET) 
                                           + (((LSA_UINT16)pDBin->LinkAddInfo.ChangeReason) << MRP_IN_PACKET_VAL_LNK_CHANGE_REASON_BIT_OFFSET));
      pInLnkChgTxInfo->inLinkRole    |= pDBin->DBmic.MRPIC_MICPosition;

      MRP_MEMCPY (pInLnkChgTxInfo->rxSA, pDBin->pDBifRef->macAddr, MRP_PACKET_VAL_MAC_ADDR_LEN);

      if (linkState == MRP_LINK_STATE_UP /*|| mrp_edd_force_link_up (pDBin->pChSys, pDBin->IPort) */)
      {
        (void) (mrp_edd_get_snd_rqb (pDBin->pChSys, pDBin->pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_LNK_CHG_BOT, &pRQB, MRP_EDD_GET_SND_RQB_NO_THRESHOLD));

        if (!LSA_HOST_PTR_ARE_EQUAL (pRQB, LSA_NULL))
        {
          /* send parameter */
          pFrame = (MRP_IN_PACKET_STRUCT_LNK_CHG_TYPE*)MRP_EDD_LOWER_GET_NRT_TX_BUFFER (pRQB); /*lint !e826 CM 10/11/2016 cast on purpose */

          pFrame->payload.tlv        = MRP_HTON16 (MRP_IN_PACKET_VAL_TLV_LNK_UP);

          pFrame->payload.portRole   = MRP_HTON16 (MRP_IN_PACKET_VAL_PORT_ROLE_IN);
          pFrame->payload.inId       = MRP_HTON16 (pInLnkChgTxInfo->inId);
          pFrame->payload.interval   = MRP_HTON16 (pInLnkChgTxInfo->intervalTimeMs);
          pFrame->payload.linkRole   = MRP_HTON16 (pInLnkChgTxInfo->inLinkRole);
          pFrame->payload.sequenceID = MRP_HTON16 (pDBin->pDBifRef->pSequenceID[MRP_DEFAULT_INSTANCE]);

          MRP_MEMCPY (pFrame->payload.sa, pDBin->pDBifRef->macAddr, MRP_PACKET_VAL_MAC_ADDR_LEN);
          MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrame->payload.domainUUID, pDBin->DomainUUID, sizeof (pFrame->payload.domainUUID));


          MRP_PROGRAM_TRACE_07 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d](secLink/coupled=%d/%d): send mrp_in_lnkChg_req at port=%u, linkUp=%d, sequID=%u, time=%u ", 
                                pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->DBmic.MRPIC_MICPosition, pDBin->DBmic.isCoupledMic, pDBin->IPort, linkState, pDBin->pDBifRef->pSequenceID[MRP_DEFAULT_INSTANCE], time);
          mrp_edd_req_snd (pRQB, pDBin->IPort, MRP_IN_PACKET_SIZE_DEFAULT, LSA_TRUE);

          pDBin->pDBifRef->pSequenceID[MRP_DEFAULT_INSTANCE]++;
        }
        else
        {
          MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_ERROR, "MIC[inInst=%d]: mrp_in_mic_lnkChg_req_send: NO RQB to send inLnkChg on IPort=%u", 
                                pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);
        }
      }
      else
      {
        MRP_PROGRAM_TRACE_04 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d](secLink/coupled=%d/%d): mrp_in_mic_lnkChg_req_send: inLnkChg (LNK_DOWN) not sent on IPort=%u", 
                              pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->DBmic.MRPIC_MICPosition, pDBin->DBmic.isCoupledMic, pDBin->IPort);
      }

      /* provide TX info for MRC/MRM */
      pDBin->InTxInfo = pDBin->InLnkChgTxInfo;
      mrp_in_mrp_tx_request (pDBin);
    }
    else
    {
      MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_ERROR, "MIC[inInst=%d]: mrp_in_mic_lnkChg_req_send: inLnkChg NOT sent on IPort=%u because ch_state=CLOSING", 
                            pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);
    }
  }
  else
  {
    MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d]: ignore send mrp_in_lnkChg_req at port=%u, linkUp=%d on SECcoupledMIC", 
                          pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort, linkState);
  }
}



/*===========================================================================
* FUNCTION : mrp_in_mic_oper_state_ip_idle
*----------------------------------------------------------------------------
* PURPOSE  : operational MRP interconnection CBF for MIC called when the MIC
*            is in IP_IDLE state (has received LINK_UP on IPort), sending of
*            IN_LNK_CHG (LNK_DOWN) frames to inform the MIM is completed and
*            the port state of the IPort is changed to FORWARDING.
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of mrp interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mic_oper_state_ip_idle (MRP_IN_DB_TYPE *pDBin)
{
  LSA_BOOL rxFromDifferentIn, differentMac, differentInId;
  MRP_IN_MIC_DB_TYPE *pDBmic;

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBin->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  pDBmic = &pDBin->DBmic;

  MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIC[inInst=%d]: mrp_in_mic_oper_state_ip_idle entered: event=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.CurrentEvent);

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
        /* ignore */
      }
      else /* LinkState == MRP_LINK_STATE_DOWN */
      {
        MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d]: ip_idle=>de, (MAU_TYPE_CHG IPort=%u DOWN)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);

        pDBmic->LNKchgNReturnDown = pDBmic->LNKNRmaxDown;

        mrp_edd_set_port_state (pDBin->pChSys, pDBin->pInstance, pDBin->IPort, MRP_PORT_STATE_BLOCKED);
        mrp_in_mic_lnkChg_req_send (pDBin, MRP_LINK_STATE_DOWN, ((pDBmic->LNKchgNReturnDown + 1) * pDBmic->LNKdownTcnt_10ms));
        mrp_in_mic_lnkChg_down_timer_start (pDBin, pDBmic->LNKdownTcnt_10ms);

        pDBmic->pInOperStateCbf = mrp_in_mic_oper_state_de;
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_TPLG_CHG_TRF:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort == pDBin->IPort)
        {
          pDBmic->hasReceivedMimFrame = LSA_TRUE;
          pDBmic->isCoupledMic        = LSA_TRUE;

          MRP_PROGRAM_TRACE_04 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d](secLink/coupled=%d/%d): ip_idle=>ip_idle, (IN_TC_TRF received at IPort=%u)", 
                                pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmic->MRPIC_MICPosition, pDBmic->isCoupledMic, pDBin->IPort);

          mrp_in_inTC_forward_to_mrp (pDBin);
        }
        else
        {
          /* ignore */
        }
      }
      else
      {
        /* ignore */
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_TPLG_CHG_WRK:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort != pDBin->IPort)
        {
          pDBmic->hasReceivedMimFrame = LSA_TRUE;

          MRP_PROGRAM_TRACE_05 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d](secLink/coupled=%d/%d): ip_idle=>ip_idle, (IN_TC_WRK received at port=%d, IPort=%u)", 
                                pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmic->MRPIC_MICPosition, pDBmic->isCoupledMic, pDBin->rxPara.RxPort, pDBin->IPort);

          if (mrp_inTC_forward_is_needed (pDBin)) /* avoid duplicate forwarding, if InTC is received at both ring ports (MIC/MRA role combination) */
          {
            mrp_in_inTC_forward_to_mrp (pDBin); /* if mrpRole= MRM -> InTC has to be converted to TC, if mrpRole = MRC -> do nothing */

            if (pDBmic->MRPIC_MICPosition == MRP_IN_DEFAULT_VAL_IN_PRM_MIC_POS && !pDBmic->isCoupledMic)
            {
              mrp_in_inTC_forward_to_iport (pDBin);
            }
            else
            {
              /* ignore */
            }
          }
        }
        else
        {
          /* ignore */
        }
      }
      else
      {
        /* ignore */
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
          pDBmic->hasReceivedMimFrame = LSA_TRUE;

          if (pDBmic->MRPIC_MICPosition == MRP_IN_DEFAULT_VAL_IN_PRM_MIC_POS)
          {
            if (pDBin->rxPara.RxPortRole != MRP_IN_PACKET_VAL_PORT_ROLE_IN)
              pDBmic->isCoupledMic = LSA_FALSE;
            else
              pDBmic->isCoupledMic = LSA_TRUE;

            MRP_PROGRAM_TRACE_05 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIC[inInst=%d](secLink/coupled=%d/%d): ip_idle=>ip_idle, (IN_POLL_WRK received at port=%d, IPort=%d)",
                                  pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmic->MRPIC_MICPosition, pDBmic->isCoupledMic, pDBin->rxPara.RxPort, pDBin->IPort);

            mrp_in_mic_lnkChg_req_send (pDBin, MRP_LINK_STATE_UP, 0);
            /* if a PRM MIC has received an InPoll frame at a ring port -> respond to it */

            if (!pDBmic->isCoupledMic)
              mrp_in_inPoll_forward_to_iport (pDBin);
          }
          else
          {
            MRP_PROGRAM_TRACE_05 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIC[inInst=%d](secLink/coupled=%d/%d): ip_idle=>ip_idle, (IN_POLL_WRK received at port=%d, IPort=%d) ingnored",
                                  pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmic->MRPIC_MICPosition, pDBmic->isCoupledMic, pDBin->rxPara.RxPort, pDBin->IPort);
          }
        }
        else
        {
          /* ignore */
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_POLL_TRF:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort == pDBin->IPort)
        {
          if (mrp_in_mic_rx_inPoll_valid_at_this_IPort (pDBin))
          {
            pDBmic->hasReceivedMimFrame = LSA_TRUE;
            pDBmic->isCoupledMic        = LSA_TRUE;

            MRP_PROGRAM_TRACE_04 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIC[inInst=%d](secLink/coupled=%d/%d): ip_idle=>ip_idle, (IN_POLL_TRF received at IPort=%d)",
                                  pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmic->MRPIC_MICPosition, pDBmic->isCoupledMic, pDBin->IPort);

            if (pDBmic->MRPIC_MICPosition == MRP_IN_DEFAULT_VAL_IN_PRM_MIC_POS) /* PRM MIC received an InPoll frame on IPort */
            {
              mrp_in_mic_lnkChg_req_send (pDBin, MRP_LINK_STATE_UP, 0);
            }
            else /* SEC coupled MIC */
            {
              mrp_in_inPoll_forward_to_mrp (pDBin);
            }
          }
          else
          {
            MRP_PROGRAM_TRACE_05 (traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "MIC[inInst=%d](secLink/coupled=%d/%d): ip_idle=>ip_idle, (IN_POLL_TRF (portRole=%d) received at IPort=%d) ignored",
                                  pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmic->MRPIC_MICPosition, pDBmic->isCoupledMic, pDBin->rxPara.RxPortRole, pDBin->IPort);
          }
        }
        else
        {
          /* ignore */
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_LNK_CHG_WRK:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort != pDBin->IPort)
        {
          rxFromDifferentIn = mrp_in_rx_from_different_interconn (pDBin, &differentMac, &differentInId);

          if (rxFromDifferentIn) /* only forward InLnkChg frames sent by another MIC -> filter InLnkChg frames sent by local MIC */
          {
            if ((pDBin->rxPara.RxLnkRole & MRP_IN_PACKET_VAL_BITMASK_BIT0_SET) == MRP_IN_PACKET_VAL_LNK_ROLE_BIT0_PRM) /* MRP_IN_LNK_CHG from PRM or PRM coupled MIC received at MRP ringports */
            {
              MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIC[inInst=%d]: ip_idle=>ip_idle, (IN_LNK_CHG_WRK received at port=%d, IPort=%d)", 
                                    pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);

              if (mrp_inLnkChg_forward_is_needed (pDBin))
              {
                mrp_in_inLnkChg_forward_to_iport (pDBin);
              }
            }
            else
            {
              /* ignore */
            }
          }
          else
          {
            /* ignore */
          }
        }
        else
        {
          /* ignore */
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
          rxFromDifferentIn = mrp_in_rx_from_different_interconn (pDBin, &differentMac, &differentInId); /* InLinkChange received from coupling MIC on PRM link */

          if (rxFromDifferentIn) /* only forward InLnkChg frames sent by another MIC -> filter InLnkChg frames sent by local MIC */
          {
            if (pDBmic->MRPIC_MICPosition == MRP_IN_DEFAULT_VAL_IN_PRM_MIC_POS) /* only relevant, if it's a PRM MIC */
            {
              if ((pDBin->rxPara.RxLnkRole & MRP_IN_PACKET_VAL_BITMASK_BIT0_SET) == MRP_IN_PACKET_VAL_LNK_ROLE_BIT0_PRM)
              {
                MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIC[inInst=%d]: ip_idle=>ip_idle, (IN_LNK_CHG_TRF received at IPort=%d)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);

                if (mrp_inLnkChg_forward_is_needed (pDBin))
                {
                  mrp_in_inLnkChg_forward_to_mrp (pDBin);
                }
              }
              else
              {
                /* ignore */
              }
            }
            else
            {
              /* ignore */
            }
          }
          else
          {
            /* ignore */
          }
        }
        else
        {
          /* ignore */
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_LNK_CHG_DOWN_TIMER:
    case MRP_IN_EVENT_T_LNK_CHG_UP_TIMER:
    case MRP_IN_EVENT_T_MRP_CHG_IND:
    {
      /* ignore */
    }
    break;

    /*==========================================================================*/
    default:
    {
      MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_FATAL, "MIC[inInst=%d]: mrp_in_mic_oper_state_ip_idle: Invalid eventType=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.CurrentEvent);
    }
    break;
  }
}

/*===========================================================================
* FUNCTION : mrp_in_mic_oper_state_de
*----------------------------------------------------------------------------
* PURPOSE  : operational MRP interconnection CBF for MIC called when the MIC
*            is in DE state (has received LINK_DOWN on IPort) and is sending
*            IN_LNK_CHG (LNK_DOWN) frames to inform the MIM
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of mrp interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mic_oper_state_de (MRP_IN_DB_TYPE *pDBin)
{
  MRP_IN_MIC_DB_TYPE *pDBmic;

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBin->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  pDBmic = &pDBin->DBmic;

  MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIC[inInst=%d]: mrp_in_mic_oper_state_de entered: event=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.CurrentEvent);

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
        MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d]: de=>pt, (MAU_TYPE_CHG IPort=%u UP)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);

        mrp_in_mic_lnkChg_down_timer_stop (pDBin);
        pDBmic->LNKchgNReturnUp = pDBmic->LNKNRmaxUp;

        mrp_in_mic_lnkChg_req_send (pDBin, MRP_LINK_STATE_UP, ((pDBmic->LNKchgNReturnUp + 1) * pDBmic->LNKupTcnt_10ms));
        mrp_in_mic_lnkChg_up_timer_start (pDBin, pDBmic->LNKupTcnt_10ms);

        pDBmic->pInOperStateCbf = mrp_in_mic_oper_state_pt;
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
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort == pDBin->IPort)
        {
          MRP_PROGRAM_TRACE_04 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d](secLink/coupled=%d/%d): de=>ac_stat1, (IN_TC_TRF received at IPort=%u DOWN)", 
                                pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmic->MRPIC_MICPosition, pDBmic->isCoupledMic, pDBin->IPort);

          pDBmic->hasReceivedMimFrame = LSA_TRUE;
          mrp_in_mic_lnkChg_down_timer_stop (pDBin);

          pDBmic->LNKchgNReturnDown = pDBmic->LNKNRmaxDown;
          mrp_edd_set_port_state (pDBin->pChSys, pDBin->pInstance, pDBin->IPort, MRP_PORT_STATE_BLOCKED);

          pDBmic->pInOperStateCbf = mrp_in_mic_oper_state_ac_stat1;
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_TPLG_CHG_WRK:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort != pDBin->IPort)
        {
          MRP_PROGRAM_TRACE_05 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d](secLink/coupled=%d/%d): de=>ac_stat1, (IN_TC_WRK received at port=%d, IPort=%u DOWN)", 
                                pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmic->MRPIC_MICPosition, pDBmic->isCoupledMic, pDBin->rxPara.RxPort, pDBin->IPort);

          pDBmic->hasReceivedMimFrame = LSA_TRUE;

          mrp_in_inTC_forward_to_mrp (pDBin); /* if mrpRole= MRM -> InTC has to be converted to TC, if mrpRole = MRC -> do nothing */

          mrp_in_mic_lnkChg_down_timer_stop (pDBin);

          pDBmic->LNKchgNReturnDown = pDBmic->LNKNRmaxDown;
          mrp_edd_set_port_state (pDBin->pChSys, pDBin->pInstance, pDBin->IPort, MRP_PORT_STATE_BLOCKED);

          pDBmic->pInOperStateCbf = mrp_in_mic_oper_state_ac_stat1;
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
          pDBmic->hasReceivedMimFrame = LSA_TRUE;

          if (pDBmic->MRPIC_MICPosition == MRP_IN_DEFAULT_VAL_IN_PRM_MIC_POS)
          {
            if (pDBin->rxPara.RxPortRole != MRP_IN_PACKET_VAL_PORT_ROLE_IN)
              pDBmic->isCoupledMic = LSA_FALSE;
            else
              pDBmic->isCoupledMic = LSA_TRUE;
          }

          MRP_PROGRAM_TRACE_05 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIC[inInst=%d](secLink/coupled=%d/%d): de=>de, (IN_POLL_WRK received at port=%d, IPort=%u DOWN)", 
                                pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmic->MRPIC_MICPosition, pDBmic->isCoupledMic, pDBin->rxPara.RxPort, pDBin->IPort);
         
          mrp_in_mic_lnkChg_req_send (pDBin, MRP_LINK_STATE_DOWN, 0);
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_POLL_TRF:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort == pDBin->IPort)
        {
          if (mrp_in_mic_rx_inPoll_valid_at_this_IPort (pDBin))
          {
            pDBmic->hasReceivedMimFrame = LSA_TRUE;
            pDBmic->isCoupledMic = LSA_TRUE;

            MRP_PROGRAM_TRACE_04 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIC[inInst=%d](secLink/coupled=%d/%d): de=>de, (IN_POLL_TRF received at IPort=%d DOWN)",
                                  pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmic->MRPIC_MICPosition, pDBmic->isCoupledMic, pDBin->IPort);

            mrp_in_mic_lnkChg_req_send (pDBin, MRP_LINK_STATE_DOWN, 0);
          }
          else
          {
            MRP_PROGRAM_TRACE_05 (traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "MIC[inInst=%d](secLink/coupled=%d/%d): de=>de, (IN_POLL_TRF (portRole=%d) received at IPort=%d) ignored",
                                  pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmic->MRPIC_MICPosition, pDBmic->isCoupledMic, pDBin->rxPara.RxPortRole, pDBin->IPort);
          }
        }
        else
        {
          /* ignore */
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_LNK_CHG_WRK:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort != pDBin->IPort)
        {
          /* ignore */
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
          /* ignore */
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_LNK_CHG_DOWN_TIMER:
    {
      if (pDBmic->LNKchgNReturnDown > 0)
      {
        MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIC[inInst=%d]: de=>de, (LNK_CHG_DOWN TIMER expired IPort=%u DOWN, NReturn=%d)", 
                              pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort, pDBmic->LNKchgNReturnDown);

        pDBmic->LNKchgNReturnDown--;

        mrp_in_mic_lnkChg_req_send (pDBin, MRP_LINK_STATE_DOWN, ((pDBmic->LNKchgNReturnDown + 1) * pDBmic->LNKdownTcnt_10ms));
        mrp_in_mic_lnkChg_down_timer_start (pDBin, pDBmic->LNKdownTcnt_10ms);
      }
      else /* pDBmic->LNKchgNReturn == 0 */
      {
        MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d]: de=>ac_stat1, (LNK_CHG_DOWN TIMER expired IPort=%u DOWN)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);

        pDBmic->LNKchgNReturnDown = pDBmic->LNKNRmaxDown;

        pDBmic->pInOperStateCbf = mrp_in_mic_oper_state_ac_stat1;
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_LNK_CHG_UP_TIMER:
    case MRP_IN_EVENT_T_MRP_CHG_IND:
    {
      /* ignore */
    }
    break;


    /*==========================================================================*/
    default:
    {
      MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_FATAL, "MIC[inInst=%d]: mrp_in_mic_oper_state_de: Invalid eventType=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.CurrentEvent);
    }
    break;
  }
}



/*===========================================================================
* FUNCTION : mrp_in_mic_oper_state_pt
*----------------------------------------------------------------------------
* PURPOSE  : operational MRP interconnection CBF for MIC called when the MIC
*            is in PT state (has an MRP interconnection LINKUP) and is
*            sending IN_LNK_CHG (LINK_UP) frames to inform the MIM
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of mrp interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mic_oper_state_pt (MRP_IN_DB_TYPE *pDBin)
{
  LSA_BOOL rxFromDifferentIn, differentMac, differentInId;
  MRP_IN_MIC_DB_TYPE *pDBmic;

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBin->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  pDBmic = &pDBin->DBmic;

  MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIC[inInst=%d]: mrp_in_mic_oper_state_pt entered: event=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.CurrentEvent);

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
        /* ignore */
      }
      else /* LinkState == MRP_LINK_STATE_DOWN */
      {
        MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d]: pt=>de, (MAU_TYPE_CHG IPort=%u DOWN)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);

        mrp_in_mic_lnkChg_up_timer_stop (pDBin);

        pDBmic->LNKchgNReturnDown = pDBmic->LNKNRmaxDown;
        pDBmic->LNKchgNReturnUp   = pDBmic->LNKNRmaxUp;

        mrp_in_mic_lnkChg_req_send (pDBin, MRP_LINK_STATE_DOWN, ((pDBmic->LNKchgNReturnDown + 1) * pDBmic->LNKdownTcnt_10ms));
        mrp_in_mic_lnkChg_down_timer_start (pDBin, pDBmic->LNKdownTcnt_10ms);

        pDBmic->pInOperStateCbf = mrp_in_mic_oper_state_de;
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_TPLG_CHG_TRF:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort == pDBin->IPort)
        {
          pDBmic->hasReceivedMimFrame = LSA_TRUE;
          pDBmic->isCoupledMic        = LSA_TRUE;

          mrp_in_mic_lnkChg_up_timer_stop (pDBin);
          mrp_in_mic_init_proj_lnkChgUp_params (pDBin);

          mrp_edd_set_port_state (pDBin->pChSys, pDBin->pInstance, pDBin->IPort, MRP_PORT_STATE_FORWARD);

          MRP_PROGRAM_TRACE_04 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d](secLink/coupled=%d/%d): pt=>ip_idle, (IN_TC_TRF received at IPort=%u)", 
                                pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmic->MRPIC_MICPosition, pDBmic->isCoupledMic, pDBin->IPort);

          mrp_in_inTC_forward_to_mrp (pDBin);

          pDBmic->pInOperStateCbf = mrp_in_mic_oper_state_ip_idle;
        }
        else
        {
          /* ignore */
        }
      }
      else
      {
        /* ignore */
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_TPLG_CHG_WRK:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort != pDBin->IPort) /* MRP_IN_TC received on MRP ringports */
        {
          pDBmic->hasReceivedMimFrame = LSA_TRUE;

          mrp_in_mic_lnkChg_up_timer_stop (pDBin);
          mrp_in_mic_init_proj_lnkChgUp_params (pDBin);

          mrp_edd_set_port_state (pDBin->pChSys, pDBin->pInstance, pDBin->IPort, MRP_PORT_STATE_FORWARD);

          MRP_PROGRAM_TRACE_05 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d](secLink/coupled=%d/%d): pt=>ip_idle, (IN_TC_WRK received at port=%d, IPort=%d)", 
                                pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmic->MRPIC_MICPosition, pDBmic->isCoupledMic, pDBin->rxPara.RxPort, pDBin->IPort);

          if (mrp_inTC_forward_is_needed (pDBin)) /* avoid duplicate forwarding, if InTC is received at both ring ports (MIC/MRA role combination) */
          {
            mrp_in_inTC_forward_to_mrp (pDBin); /* if mrpRole= MRM -> InTC has to be converted to TC, if mrpRole = MRC -> do nothing */

            if (pDBmic->MRPIC_MICPosition == MRP_IN_DEFAULT_VAL_IN_PRM_MIC_POS && !pDBmic->isCoupledMic)
            {
              mrp_in_inTC_forward_to_iport (pDBin);
            }
            else
            {
              /* ignore */
            }
          }
          pDBmic->pInOperStateCbf = mrp_in_mic_oper_state_ip_idle;
        }
        else
        {
          /* ignore */
        }
      }
      else
      {
        /* ignore */
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
          pDBmic->hasReceivedMimFrame = LSA_TRUE;

          if (pDBmic->MRPIC_MICPosition == MRP_IN_DEFAULT_VAL_IN_PRM_MIC_POS) /* PRM MIC received the InPoll at a ring port */
          {
            if (pDBin->rxPara.RxPortRole != MRP_IN_PACKET_VAL_PORT_ROLE_IN)
              pDBmic->isCoupledMic = LSA_FALSE;
            else
              pDBmic->isCoupledMic = LSA_TRUE;

            mrp_in_mic_lnkChg_req_send (pDBin, MRP_LINK_STATE_UP, 0);
 
            if (!pDBmic->isCoupledMic)
              mrp_in_inPoll_forward_to_iport (pDBin);
            /* The PRM coupled MIC has no need to FWD the POLL to the IPort */

            MRP_PROGRAM_TRACE_05 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d](secLink/coupled=%d/%d): pt=>pt, (IN_POLL_WRK received at port=%d, IPort=%d)",
                                  pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmic->MRPIC_MICPosition, pDBmic->isCoupledMic, pDBin->rxPara.RxPort, pDBin->IPort);
          }
          else /* SEC_COUPLED MIC */
          {
            MRP_PROGRAM_TRACE_05 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIC[inInst=%d](secLink/coupled=%d/%d): pt=>pt, (IN_POLL_WRK received at port=%d, IPort=%d) ignored",
                                  pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmic->MRPIC_MICPosition, pDBmic->isCoupledMic, pDBin->rxPara.RxPort, pDBin->IPort);
            /* if the SEC_COUPLED MIC has received InPoll at a ring port, there is no need to forward it to the IPort */
           }
        }
        else
        {
          /* ignore */
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_POLL_TRF:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort == pDBin->IPort)
        {
          if (mrp_in_mic_rx_inPoll_valid_at_this_IPort (pDBin))
          {
            pDBmic->hasReceivedMimFrame = LSA_TRUE;
            pDBmic->isCoupledMic        = LSA_TRUE;

            MRP_PROGRAM_TRACE_04 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d](secLink/coupled=%d/%d): pt=>pt, (IN_POLL_TRF received at IPort=%d)",
                                  pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmic->MRPIC_MICPosition, pDBmic->isCoupledMic, pDBin->IPort);

            if (pDBmic->MRPIC_MICPosition == MRP_IN_DEFAULT_VAL_IN_PRM_MIC_POS) /* PRM MIC received an InPoll frame on IPort */
            {
              mrp_in_mic_lnkChg_req_send (pDBin, MRP_LINK_STATE_UP, 0);
            }
            else /* SEC coupled MIC */
            {
              mrp_in_inPoll_forward_to_mrp (pDBin);
            }
          }
          else
          {
            MRP_PROGRAM_TRACE_05 (traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "MIC[inInst=%d](secLink/coupled=%d/%d): pt=>pt, (IN_POLL_TRF (portRole=%d) received at IPort=%d) ignored",
                                  pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmic->MRPIC_MICPosition, pDBmic->isCoupledMic, pDBin->rxPara.RxPortRole, pDBin->IPort);
          }
        }
        else
        {
          /* ignore */
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_LNK_CHG_WRK:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort != pDBin->IPort)
        {
          rxFromDifferentIn = mrp_in_rx_from_different_interconn (pDBin, &differentMac, &differentInId);

          if (rxFromDifferentIn) /* only forward InLnkChg frames sent by another MIC -> filter InLnkChg frames sent by local MIC */
          {
            if ((pDBin->rxPara.RxLnkRole & MRP_IN_PACKET_VAL_BITMASK_BIT0_SET) == MRP_IN_PACKET_VAL_LNK_ROLE_BIT0_PRM) /* MRP_IN_LNK_CHG from PRM or PRM coupled MIC received at MRP ringports */
            {
              MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIC[inInst=%d]: pt=>pt, (IN_LNK_CHG_WRK received at port=%d, IPort=%d)", 
                                    pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);
              if (mrp_inLnkChg_forward_is_needed (pDBin))
              {
                mrp_in_inLnkChg_forward_to_iport (pDBin);
              }
            }
            else
            {
              /* ignore */
            }
          }
          else
          {
            /* ignore */
          }
        }
        else
        {
          /* ignore */
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
          rxFromDifferentIn = mrp_in_rx_from_different_interconn (pDBin, &differentMac, &differentInId);

          if (rxFromDifferentIn) /* only forward InLnkChg frames sent by another MIC -> filter InLnkChg frames sent by local MIC */
          {
            if (pDBmic->MRPIC_MICPosition == MRP_IN_DEFAULT_VAL_IN_PRM_MIC_POS)      /* a PRM MIC received the InLnkChg on IPort */
            {
              if ((pDBin->rxPara.RxLnkRole & MRP_IN_PACKET_VAL_BITMASK_BIT0_SET) == MRP_IN_PACKET_VAL_LNK_ROLE_BIT0_PRM) /* MRP_IN_LNK_CHG was sent from a PRM LINK device (PRM coupled MIC) */
              {
                MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIC[inInst=%d]: pt=>pt, (IN_LNK_CHG_TRF received at IPort=%d)", 
                                      pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);

                if (mrp_inLnkChg_forward_is_needed (pDBin))
                {
                  mrp_in_inLnkChg_forward_to_mrp (pDBin);
                }
              }
            }
            else
            {
              /* ignore */
            }
          }
          else
          {
            /* ignore */
          }
        }
        else
        {
          /* ignore */
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_LNK_CHG_DOWN_TIMER:
    case MRP_IN_EVENT_T_MRP_CHG_IND:
    {
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_LNK_CHG_UP_TIMER:
    {
      if (pDBmic->LNKchgNReturnUp > 0)
      {
        MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIC[inInst=%d]: pt=>pt, (LNK_CHG_UP TIMER expired IPort=%u UP, NReturn=%d)", 
                              pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort, pDBmic->LNKchgNReturnUp);

        pDBmic->LNKchgNReturnUp--;

        mrp_in_mic_lnkChg_req_send (pDBin, MRP_LINK_STATE_UP, ((pDBmic->LNKchgNReturnUp + 1) * pDBmic->LNKupTcnt_10ms));
        mrp_in_mic_lnkChg_up_timer_start (pDBin, pDBmic->LNKupTcnt_10ms);
      }
      else /* pDBmic->LNKchgNReturnUp == 0 */
      {
        mrp_in_mic_init_proj_lnkChgUp_params (pDBin);

        if (pDBmic->hasToWait4MimFrame == LSA_FALSE || (pDBmic->hasToWait4MimFrame == LSA_TRUE && pDBmic->hasReceivedMimFrame == LSA_TRUE))
        {

          MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d]: pt=>ip_idle, (LNK_CHG_UP TIMER expired IPort=%u UP)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);

          mrp_edd_set_port_state (pDBin->pChSys, pDBin->pInstance, pDBin->IPort, MRP_PORT_STATE_FORWARD);

          pDBmic->pInOperStateCbf = mrp_in_mic_oper_state_ip_idle;
        }
        else
        {
          MRP_PROGRAM_TRACE_04 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d]: pt=>pt, (LNK_CHG_UP TIMER expired IPort=%u UP, hasToWait4MIM=%d, hasReceivedMIMFrame=%d)",
                                pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort, pDBmic->hasToWait4MimFrame, pDBmic->hasReceivedMimFrame);
        }
      }
    }
    break;

    /*==========================================================================*/
    default:
    {
      MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_FATAL, "MIC[inInst=%d]: mrp_in_mic_oper_state_pt: Invalid eventType=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.CurrentEvent);
    }
    break;
  }
}




/*===========================================================================
* FUNCTION : mrp_in_mic_oper_state_ac_stat1
*----------------------------------------------------------------------------
* PURPOSE  : operational MRP interconnection CBF for MIC called when the MIC
*            is in initial state (without any MRP interconnection link)
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of mrp interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mic_oper_state_ac_stat1 (MRP_IN_DB_TYPE *pDBin)
{
  MRP_IN_MIC_DB_TYPE *pDBmic;

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBin->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  pDBmic = &pDBin->DBmic;

  MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIC[inInst=%d]: mrp_in_mic_oper_state_ac_stat1 entered: event=%d", 
                        pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.CurrentEvent);

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
        MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d]: ac_stat1=>pt, (MAU_TYPE_CHG IPort=%u UP)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);

        pDBmic->LNKchgNReturnUp = pDBmic->LNKNRmaxUp;
        mrp_in_mic_lnkChg_req_send (pDBin, MRP_LINK_STATE_UP, ((pDBmic->LNKchgNReturnUp + 1) * pDBmic->LNKupTcnt_10ms));
        mrp_in_mic_lnkChg_up_timer_start (pDBin, pDBmic->LNKupTcnt_10ms);

        pDBmic->pInOperStateCbf = mrp_in_mic_oper_state_pt;
      }
      else /* LinkState == MRP_LINK_STATE_DOWN */
      {
        MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d]: ac_stat1=>de, (MAU_TYPE_CHG IPort=%u DOWN)", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);

        pDBmic->LNKchgNReturnDown = pDBmic->LNKNRmaxDown;
        mrp_in_mic_lnkChg_req_send (pDBin, MRP_LINK_STATE_DOWN, ((pDBmic->LNKchgNReturnDown + 1) * pDBmic->LNKdownTcnt_10ms));
        mrp_in_mic_lnkChg_down_timer_start (pDBin, pDBmic->LNKdownTcnt_10ms);

        pDBmic->pInOperStateCbf = mrp_in_mic_oper_state_de;
      }
    }
    break;


    /*==========================================================================*/
    case MRP_IN_EVENT_T_TPLG_CHG_WRK:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort != pDBin->IPort) /* MRP_IN_TC received on MRP ringports */
        {
          pDBmic->hasReceivedMimFrame = LSA_TRUE;

          if (mrp_edd_force_link_up (pDBin->pChSys, pDBin->IPort))
          {
            MRP_PROGRAM_TRACE_05 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d](secLink/coupled=%d/%d): ac_stat1=>pt, (IN_TC_WRK received at port=%d, FORCE LinkUp, IPort=%u)",
                                  pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmic->MRPIC_MICPosition, pDBmic->isCoupledMic, pDBin->rxPara.RxPort, pDBin->IPort);
            
            pDBmic->LNKchgNReturnUp = pDBmic->LNKNRmaxUp;
            mrp_in_mic_lnkChg_req_send (pDBin, MRP_LINK_STATE_UP, ((pDBmic->LNKchgNReturnUp + 1) * pDBmic->LNKupTcnt_10ms));
            mrp_in_mic_lnkChg_up_timer_start (pDBin, pDBmic->LNKupTcnt_10ms);

            if (mrp_inTC_forward_is_needed (pDBin)) /* avoid duplicate forwarding, if InTC is received at both ring ports (MIC/MRA role combination) */
            {
              mrp_in_inTC_forward_to_mrp (pDBin); /* if mrpRole= MRM -> InTC has to be converted to TC, if mrpRole = MRC -> do nothing */

              if (pDBmic->MRPIC_MICPosition == MRP_IN_DEFAULT_VAL_IN_PRM_MIC_POS && !pDBmic->isCoupledMic)
              {
                mrp_in_inTC_forward_to_iport (pDBin);
              }
              else
              {
                /* ignore */
              }
            }

            pDBmic->pInOperStateCbf = mrp_in_mic_oper_state_pt;
          }
          else
          {
            MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d]: ac_stat1=>ac_stat1, (IN_TC_WRK received at port=%d, NO LinkUp timer running, IPort=%u)", 
                                  pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBin->IPort);
          }
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_TPLG_CHG_TRF:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort == pDBin->IPort)
        {
          pDBmic->hasReceivedMimFrame = LSA_TRUE;
          pDBmic->isCoupledMic        = LSA_TRUE;

          if (mrp_edd_force_link_up (pDBin->pChSys, pDBin->IPort))
          {
            MRP_PROGRAM_TRACE_05 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d](secLink/coupled=%d/%d): ac_stat1=>pt, (IN_TC_TRF received at port=%d, FORCE LinkUp, IPort=%u)",
                                  pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmic->MRPIC_MICPosition, pDBmic->isCoupledMic, pDBin->rxPara.RxPort, pDBin->IPort);

            pDBmic->LNKchgNReturnUp = pDBmic->LNKNRmaxUp;
            mrp_in_mic_lnkChg_req_send (pDBin, MRP_LINK_STATE_UP, ((pDBmic->LNKchgNReturnUp + 1) * pDBmic->LNKupTcnt_10ms));
            mrp_in_mic_lnkChg_up_timer_start (pDBin, pDBmic->LNKupTcnt_10ms);

            mrp_in_inTC_forward_to_mrp (pDBin);

            pDBmic->pInOperStateCbf = mrp_in_mic_oper_state_pt;
          }
          else
          {
            MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d]: ac_stat1=>ac_stat1, (IN_TC_TRF received IPort=%u, NO LinkUp timer running)", 
                                  pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);
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
        if (pDBin->rxPara.RxPort != pDBin->IPort) /* MRP_IN_TC received on MRP ringports */
        {
          pDBmic->hasReceivedMimFrame = LSA_TRUE;

          if (pDBmic->MRPIC_MICPosition == MRP_IN_DEFAULT_VAL_IN_PRM_MIC_POS)
          {
            if (pDBin->rxPara.RxPortRole != MRP_IN_PACKET_VAL_PORT_ROLE_IN)
              pDBmic->isCoupledMic = LSA_FALSE;
            else
              pDBmic->isCoupledMic = LSA_TRUE;
          }

          if (mrp_edd_force_link_up (pDBin->pChSys, pDBin->IPort))
          {
            MRP_PROGRAM_TRACE_05 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d](secLink/coupled=%d/%d): ac_stat1=>pt, (IN_POLL_WRK received at port=%d, FORCE LinkUp, IPort=%u)",
                                  pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmic->MRPIC_MICPosition, pDBmic->isCoupledMic, pDBin->rxPara.RxPort, pDBin->IPort);

            pDBmic->LNKchgNReturnUp = pDBmic->LNKNRmaxUp;
            mrp_in_mic_lnkChg_req_send (pDBin, MRP_LINK_STATE_UP, ((pDBmic->LNKchgNReturnUp + 1) * pDBmic->LNKupTcnt_10ms));
            mrp_in_mic_lnkChg_up_timer_start (pDBin, pDBmic->LNKupTcnt_10ms);

            pDBmic->pInOperStateCbf = mrp_in_mic_oper_state_pt;
          }
          else
          {
            MRP_PROGRAM_TRACE_05 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIC[inInst=%d](secLink/coupled=%d/%d): ac_stat1=>ac_stat1, (IN_POLL_WRK received at port=%d, IPort=%u)",
                                  pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmic->MRPIC_MICPosition, pDBmic->isCoupledMic, pDBin->rxPara.RxPort, pDBin->IPort);

            mrp_in_mic_lnkChg_req_send (pDBin, MRP_LINK_STATE_DOWN, 0);
          }
        }
      }
    }
    break;

    /*==========================================================================*/
    case MRP_IN_EVENT_T_POLL_TRF:
    {
      if (pDBin->rxPara.RxInId == pDBin->InID)
      {
        if (pDBin->rxPara.RxPort == pDBin->IPort)
        {
          if (mrp_in_mic_rx_inPoll_valid_at_this_IPort (pDBin))
          {
            pDBmic->hasReceivedMimFrame = LSA_TRUE;
            pDBmic->isCoupledMic        = LSA_TRUE;

            if (mrp_edd_force_link_up (pDBin->pChSys, pDBin->IPort))
            {
              MRP_PROGRAM_TRACE_04 (traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d](secLink/coupled=%d/%d): ac_stat1=>pt, (IN_POLL_TRF received at IPort=%u, FORCE LinkUp)",
                                    pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmic->MRPIC_MICPosition, pDBmic->isCoupledMic, pDBin->IPort);

              pDBmic->LNKchgNReturnUp = pDBmic->LNKNRmaxUp;
              mrp_in_mic_lnkChg_req_send (pDBin, MRP_LINK_STATE_UP, ((pDBmic->LNKchgNReturnUp + 1) * pDBmic->LNKupTcnt_10ms));
              mrp_in_mic_lnkChg_up_timer_start (pDBin, pDBmic->LNKupTcnt_10ms);

              if (pDBmic->MRPIC_MICPosition != MRP_IN_DEFAULT_VAL_IN_PRM_MIC_POS) /* SEC coupled MIC */
              {
                mrp_in_inPoll_forward_to_mrp (pDBin);
              }

              pDBmic->pInOperStateCbf = mrp_in_mic_oper_state_pt;
            }
            else
            {
              MRP_PROGRAM_TRACE_04 (traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "MIC[inInst=%d](secLink/coupled=%d/%d): ac_stat1=>ac_stat1, (IN_POLL_TRF received at IPort=%u) ignored",
                                    pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmic->MRPIC_MICPosition, pDBmic->isCoupledMic, pDBin->IPort);
            }
          }
          else
          {
            MRP_PROGRAM_TRACE_05 (traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "MIC[inInst=%d](secLink/coupled=%d/%d): ac_stat1=>ac_stat1, (IN_POLL_TRF (portRole=%d) received at IPort=%d) ignored",
                                  pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmic->MRPIC_MICPosition, pDBmic->isCoupledMic, pDBin->rxPara.RxPortRole, pDBin->IPort);
          }
        }
      }
    }
    break;


    /*==========================================================================*/
    case MRP_IN_EVENT_T_LNK_CHG_WRK:
    case MRP_IN_EVENT_T_LNK_CHG_TRF:
    case MRP_IN_EVENT_T_LNK_CHG_DOWN_TIMER:
    case MRP_IN_EVENT_T_LNK_CHG_UP_TIMER:
    case MRP_IN_EVENT_T_MRP_CHG_IND:
    {
      /* ignore */
    }
    break;


    /*==========================================================================*/
    default:
    {
      MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_FATAL, "MIC[inInst=%d]: mrp_in_mic_oper_state_ac_stat1: Invalid eventType=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.CurrentEvent);
    }
    break;
  }
}



/*===========================================================================
* FUNCTION : mrp_in_mic_proj_state_cbf
*----------------------------------------------------------------------------
* PURPOSE  : MRP interconnection CBF called when no MRP interconnection
*            instance is provisioned (e.g. on MRM/MRA or MRC devices)
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of mrp interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mic_proj_state_cbf (MRP_IN_DB_TYPE *pDBin)
{
  MRP_IN_MIC_DB_TYPE *pDBmic;

  pDBmic = &pDBin->DBmic;

  MRP_PROGRAM_TRACE_02 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d]: mrp_in_mic_proj_state_cbf entered: event=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.CurrentEvent);

  /* route the MIC related event to the corresponding MIC operational state */
  pDBmic->pInOperStateCbf (pDBin);

  mrp_in_ll_in_event_handler (pDBin);
}



/*===========================================================================
* FUNCTION : mrp_in_mic_link_chg_cbf
*----------------------------------------------------------------------------
* PURPOSE  : MIC link change handling
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
static LSA_UINT8 mrp_in_mic_link_chg_cbf (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT16 port, MRP_LINK_ADD_INFO addInfo, LSA_UINT8 linkUp)
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
      MRP_PROGRAM_TRACE_03 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC_LINK_UP[inst=%d][inInst=%d] at IPort=%u", instance, inInstance, port);
      pDBinEv->LinkState = MRP_LINK_STATE_UP;
    }
    else
    {
      MRP_PROGRAM_TRACE_03 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC_LINK_DOWN[inst=%d][inInst=%d] at IPort=%u", instance, inInstance, port);
      pDBinEv->LinkState = MRP_LINK_STATE_DOWN;
    }
    mrp_in_update (pDBinEv, MRP_IN_EVENT_T_MAU_TYPE_CHG);

    return LSA_TRUE;
  }

  return LSA_FALSE;
}

/*===========================================================================
* FUNCTION : mrp_in_mic_get_params
*----------------------------------------------------------------------------
* PURPOSE  : returns mic params
*----------------------------------------------------------------------------
* RETURNS  : -
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference
*            inInstance = affected MRP interconnection instance
* OUTPUTS  : *pParams   = pointer to params struct with current MIC params
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mic_get_params (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 inInstance, MRP_IN_MIC_PARAMS_TYPE *pParams)
{
  LSA_UINT16 mrpInMaxInstance;
  MRP_IN_DB_TYPE *pDBin;
  MRP_IN_DB_TYPE *pDBinBase = (MRP_IN_DB_TYPE *)pChSys->pChSysDB->pMrpIn;

  mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pChSys);

  if (mrpInMaxInstance)
  {
    pDBin = &pDBinBase[inInstance];

    pParams->runState = pDBin->DBmic.runState;

    MRP_PROGRAM_TRACE_02 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIC[inInst=%d]: mrp_in_mic_get_params: mode=%d retrieved", inInstance, pParams->runState);

    pParams->IPort = pDBin->IPort;
    pParams->InID  = pDBin->InID;

    pParams->MRPIC_LNKNRmax         = pDBin->DBmic.MRPIC_LNKNRmax;
    pParams->MRPIC_LNKdownT_ms      = pDBin->DBmic.MRPIC_LNKdownT_ms;
    pParams->MRPIC_LNKupT_ms        = pDBin->DBmic.MRPIC_LNKupT_ms;

    pParams->MRPIC_StartDelay_100ms = pDBin->DBmic.MRPIC_StartDelay_100ms;
    pParams->MRPIC_MICPosition      = pDBin->DBmic.MRPIC_MICPosition;
  }
  else
  {
    MRP_MEMSET (pParams, 0, sizeof (MRP_IN_MIC_PARAMS_TYPE));
    MRP_PROGRAM_TRACE_00 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "MIC: mrp_in_mic_get_params called while mrpInMaxInst=0");
  }
}

/*===========================================================================
* FUNCTION : mrp_in_mic_get_module_state
*----------------------------------------------------------------------------
* PURPOSE  : get all important module params to reflect module state of MIC
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
LSA_VOID mrp_in_mic_get_module_state (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance, LSA_UINT16 inInstance, MRP_REC_MODULE_STATE_TYPE *pModuleState, LSA_BOOL addStop)
{
  MRP_IN_MIC_PARAMS_TYPE paramsMICRead;
  MRP_REC_DB_INST_SET_TYPE *pRecInstWork;
  LSA_UINT16 mrpInMaxInstance;
  MRP_REC_DB_TYPE *pDBrec = (MRP_REC_DB_TYPE *)pChUsr->pChUsrDB->pRec;

  mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pChUsr);

  if (mrpInMaxInstance && !instance)
  {
    mrp_in_mic_get_params (pDBrec->pChSys, inInstance, &paramsMICRead);

    pRecInstWork = &pDBrec->pInstData[instance].work;

    pModuleState->topoCtrl = MRP_USR_ENUM_TOPO_CTRL_NONE;
    pModuleState->configured = (pRecInstWork->pRecIcPortAdjust[inInstance].Role == MRP_IC_PROJ_ROLE_CLIENT);

    if (pModuleState->configured)
    {
      if ((pRecInstWork->pRecIcPortCheck[inInstance].Check & MRP_REC_IC_PORT_DATA_CHECK_DOMAINID) && (paramsMICRead.MRPIC_CheckDomainID == MRP_IC_CHECK_DOMAINID_OFF))
      {
        pModuleState->topoCtrl = MRP_USR_ENUM_TOPO_CTRL_ADD_CHECK;
      }
      if (!(pRecInstWork->pRecIcPortCheck[inInstance].Check & MRP_REC_IC_PORT_DATA_CHECK_DOMAINID) && (paramsMICRead.MRPIC_CheckDomainID == MRP_IC_CHECK_DOMAINID_ON))
      {
        pModuleState->topoCtrl = MRP_USR_ENUM_TOPO_CTRL_ADD;
      }
    }
    pModuleState->rpChanged = pRecInstWork->pRecIcPortAdjust[inInstance].PortID != paramsMICRead.IPort;

    pModuleState->paramChanged = (pModuleState->rpChanged
                                  || (pRecInstWork->pRecIcPortAdjust[inInstance].DomainID             != paramsMICRead.InID)
                                  || (pRecInstWork->pRecIcPortAdjust[inInstance].mic.LNKdownT_ms      != paramsMICRead.MRPIC_LNKdownT_ms)
                                  || (pRecInstWork->pRecIcPortAdjust[inInstance].mic.LNKupT_ms        != paramsMICRead.MRPIC_LNKupT_ms)
                                  || (pRecInstWork->pRecIcPortAdjust[inInstance].mic.LNKNRmax         != paramsMICRead.MRPIC_LNKNRmax)
                                  || (pRecInstWork->pRecIcPortAdjust[inInstance].mic.StartDelay_100ms != paramsMICRead.MRPIC_StartDelay_100ms)
                                  || (pRecInstWork->pRecIcPortAdjust[inInstance].mic.MICPosition      != paramsMICRead.MRPIC_MICPosition)
                                  || addStop);

    pModuleState->running = (paramsMICRead.runState == MRP_IN_OPERATION_ON);
  }
  else
  {
    pModuleState->topoCtrl     = MRP_USR_ENUM_TOPO_CTRL_NONE;
    pModuleState->configured   = LSA_FALSE;
    pModuleState->rpChanged    = LSA_FALSE;
    pModuleState->paramChanged = LSA_FALSE;
    pModuleState->running      = LSA_FALSE;
  }

  pModuleState->roleChangeRestricted = LSA_FALSE;

  /*  stop := run && !configured || restart */
  pModuleState->stop = ((pModuleState->running && !pModuleState->configured) || (pModuleState->running && pModuleState->paramChanged));
  /*  start := !run && configured || restart */
  pModuleState->start = ((!pModuleState->running && pModuleState->configured) || (pModuleState->configured && pModuleState->running && pModuleState->paramChanged));
}


/*===========================================================================
* FUNCTION : mrp_in_mic_init_proj_lnkChgUp_params
*----------------------------------------------------------------------------
* PURPOSE  : initialize MIC InLnkChg params conforming to provisioning
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin   = pointer to one instance of MRP interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mic_init_proj_lnkChgUp_params (MRP_IN_DB_TYPE *pDBin)
{
  MRP_IN_MIC_DB_TYPE *pDBmic;

  pDBmic = &pDBin->DBmic;

  /* initialize the provisioned MIC InLnkChg parameters */
  pDBmic->LNKupTcnt_10ms  = (LSA_UINT16)(pDBmic->MRPIC_LNKupT_ms / MRP_TIMER_TIME_BASE_DEZ);
  pDBmic->LNKNRmaxUp      = pDBmic->MRPIC_LNKNRmax;
  pDBmic->LNKchgNReturnUp = pDBmic->LNKNRmaxUp;

  MRP_PROGRAM_TRACE_03 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d]: mrp_in_mic_init_proj_lnkChgUp_params: LNKupTcnt_10ms=%d, LNKNRmaxUp=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBmic->LNKupTcnt_10ms, pDBmic->LNKNRmaxUp);
}



/*===========================================================================
* FUNCTION : mrp_in_mic_init_var_from_params
*----------------------------------------------------------------------------
* PURPOSE  : initialize MIC variables if default setting shall not be used
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin   = pointer to one instance of MRP interconnection database
*            *pParams = pointer to params for MIC initial setup
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_VOID mrp_in_mic_init_var_from_params (MRP_IN_DB_TYPE *pDBin, MRP_IN_MIC_PARAMS_TYPE *pParams)
{
  MRP_IN_MIC_DB_TYPE *pDBmic;

  pDBmic = &pDBin->DBmic;

  /* set new values */
  MRP_MEMCPY (pDBin->DomainUUID, pParams->DomainUUID, sizeof (pDBin->DomainUUID));

  pDBmic->MRPIC_LNKdownT_ms      = pParams->MRPIC_LNKdownT_ms;
  pDBmic->MRPIC_LNKupT_ms        = pParams->MRPIC_LNKupT_ms;

  pDBmic->MRPIC_LNKNRmax         = pParams->MRPIC_LNKNRmax;

  pDBmic->MRPIC_MICPosition      = pParams->MRPIC_MICPosition;

  if (pDBmic->MRPIC_MICPosition != MRP_IN_DEFAULT_VAL_IN_PRM_MIC_POS) /* if the MIC device is configured to be secondary coupled MIC */
  {
    pDBmic->isCoupledMic = LSA_TRUE;
  }

  pDBmic->MRPIC_CheckDomainID    = pParams->MRPIC_CheckDomainID;
  pDBmic->MRPIC_StartDelay_100ms = pParams->MRPIC_StartDelay_100ms;
  
  pDBmic->LNKdownTcnt_10ms  = (LSA_UINT16)(pParams->MRPIC_LNKdownT_ms / MRP_TIMER_TIME_BASE_DEZ);  
  pDBmic->LNKNRmaxDown      = pDBmic->MRPIC_LNKNRmax;
  pDBmic->LNKchgNReturnDown = pDBmic->LNKNRmaxDown;

  /* calculate the MIC startup parameters */
  if (pDBmic->MRPIC_StartDelay_100ms <= MRP_IN_START_DELAY_LOW_LIMIT)
  {
    mrp_in_mic_init_proj_lnkChgUp_params (pDBin);
    pDBmic->hasToWait4MimFrame = LSA_FALSE;
  }
  else
  {
    if (pDBmic->MRPIC_StartDelay_100ms >= MRP_IN_START_DELAY_MEDIUM_LIMIT_LOW && pDBmic->MRPIC_StartDelay_100ms <= MRP_IN_START_DELAY_MEDIUM_LIMIT_HIGH)
    {
      pDBmic->LNKupTcnt_10ms  = MRP_IN_DEFAULT_VAL_IN_LNK_CHG_DEFAULT_T_100MS / MRP_TIMER_TIME_BASE_DEZ;
      pDBmic->LNKNRmaxUp      = pDBmic->MRPIC_StartDelay_100ms;
      pDBmic->LNKchgNReturnUp = pDBmic->LNKNRmaxUp;

      pDBmic->hasToWait4MimFrame = LSA_FALSE;
    }
    else
    {
      mrp_in_mic_init_proj_lnkChgUp_params (pDBin);
      pDBmic->hasToWait4MimFrame = LSA_TRUE;
    }
  }

  /* takeover MIC roles here, because no default setting is requested, but probably a new role has to be activated */
  pDBin->adminRole = pParams->adminRole;
  pDBin->operRole  = pParams->operRole;
  pDBin->IPort     = pParams->IPort;
  pDBin->InID      = pParams->InID;

  MRP_PROGRAM_TRACE_03 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d]: mrp_in_mic_init_var_from_params: pDBmic written, adminRole=%d, operRole=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->adminRole, pDBin->operRole);
}


/*===========================================================================
* FUNCTION : mrp_in_mic_init_var
*----------------------------------------------------------------------------
* PURPOSE  : initialize variables
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin   = pointer to one MRP interconnection instance database
*            *pParams = pointer to params for MIC initial setup
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mic_init_var (MRP_IN_DB_TYPE *pDBin, MRP_IN_MIC_PARAMS_TYPE *pParams)
{
  MRP_IN_MIC_DB_TYPE *pDBmic;

  MRP_PROGRAM_TRACE_01 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "MIC[inInst=%d]: mrp_in_mic_init_var entered", pDBin->pInstance[MRP_IN_INSTANCE_INDEX]);

  pDBmic = &pDBin->DBmic;

  /* variables */
  MRP_MEMSET (pDBmic, 0, sizeof (pDBin->DBmic));

  pDBin->IPort     = 0;
  pDBin->InID      = MRP_IN_DEFAULT_VAL_IN_ID;

  pDBin->adminRole = pParams->adminRole;
  pDBin->operRole  = pParams->operRole;

  pDBin->rxPara.Vlan        = 0;
  pDBmic->LNKchgNReturnUp   = 0;
  pDBmic->LNKchgNReturnDown = 0;
  pDBmic->isMorphing        = LSA_FALSE;
  pDBmic->pChUsr4NextRole   = LSA_NULL;

  MRP_PROGRAM_TRACE_03 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d]: mrp_in_mic_init_var: pDBmim written, adminRole=%d, operRole=%d, morphing=OFF", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->adminRole, pDBin->operRole);

  /* internals */
  pDBin->rxPara.CurrentEvent = MRP_IN_EVENT_T_NONE;
  pDBmic->CurrentEvent       = MRP_IN_EVENT_T_NONE;
  
  pDBmic->pInOperStateCbf    = mrp_in_mic_oper_state_ac_stat1;

  if (pParams->SoftStart == MRP_SET_SOFT_START_ON)
  {
    MRP_PROGRAM_TRACE_01 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "MIC[inInst=%d]: mrp_in_mic_init_var: SoftStart=ON results to state AC_STAT1", pDBin->pInstance[MRP_IN_INSTANCE_INDEX]);
    pDBmic->pInOperStateCbf  = mrp_in_mic_oper_state_ac_stat1;
  }

  /* event attributes */
  pDBin->rxPara.LinkState  = MRP_LINK_STATE_DOWN;
  pDBin->rxPara.RxPort     = 0;

  MRP_MEMSET (pDBin->rxPara.RxSA, 0, MRP_PACKET_VAL_MAC_ADDR_LEN);
  MRP_MEMSET (pDBin->rxPara.RxPeerPortMacAddr, 0, MRP_PACKET_VAL_MAC_ADDR_LEN);
  MRP_MEMSET (pDBin->rxPara.RxUUID, 0, 16);

  pDBmic->MRPIC_LNKdownT_ms = MRP_IN_DEFAULT_VAL_IN_LNK_CHG_DEFAULT_T_20MS;
  pDBmic->MRPIC_LNKupT_ms   = MRP_IN_DEFAULT_VAL_IN_LNK_CHG_DEFAULT_T_20MS;
  pDBmic->MRPIC_LNKNRmax    = MRP_IN_DEFAULT_VAL_IN_LNK_CHG_NR_MAX;

  pDBmic->LNKdownTcnt_10ms  = MRP_IN_DEFAULT_VAL_IN_LNK_CHG_DEFAULT_T_20MS / MRP_TIMER_TIME_BASE_DEZ;
  pDBmic->LNKupTcnt_10ms    = MRP_IN_DEFAULT_VAL_IN_LNK_CHG_DEFAULT_T_20MS / MRP_TIMER_TIME_BASE_DEZ;

  pDBmic->LNKNRmaxUp        = pDBmic->MRPIC_LNKNRmax;
  pDBmic->LNKchgNReturnUp   = pDBmic->MRPIC_LNKNRmax;

  pDBmic->LNKNRmaxDown      = pDBmic->MRPIC_LNKNRmax;
  pDBmic->LNKchgNReturnDown = pDBmic->MRPIC_LNKNRmax;

  /* content of record PDPortMrpIcDataAdjust: field MRPIC_StartDelay */
  pDBmic->MRPIC_StartDelay_100ms = MRP_IN_DEFAULT_VAL_START_DELAY_ENDLESS;

  /* content of record PDPortMrpIcDataAdjust: field MRPIC_MICPosition */
  pDBmic->MRPIC_MICPosition      = MRP_IN_DEFAULT_VAL_IN_PRM_MIC_POS;

  /* content of record PDPortMrpIcDataCheck */
  pDBmic->MRPIC_CheckDomainID  = MRP_IC_CHECK_DOMAINID_OFF;

  /* state machine flags */
  pDBmic->hasToWait4MimFrame   = LSA_TRUE;
  pDBmic->hasReceivedMimFrame  = LSA_FALSE;
  pDBmic->isCoupledMic         = LSA_FALSE;

  /* parameters */

  if (pParams->SetDefaults == MRP_SET_DEFAULTS_OFF)
  {
    /* set new values */
    mrp_in_mic_init_var_from_params (pDBin, pParams);
  }
}



/*===========================================================================
* FUNCTION : mrp_in_mic_start
*----------------------------------------------------------------------------
* PURPOSE  : start one MIC instance
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys     = channel reference
*            inInstance = MRP interconnection instance to be started
*            *pParams   = pointer to MIC params to be used for start
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mic_start (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 inInstance, MRP_IN_MIC_PARAMS_TYPE *pParams)
{
  MRP_IN_DB_TYPE *pDBin;
  MRP_IN_DB_TYPE *pDBinBase = (MRP_IN_DB_TYPE *)pChSys->pChSysDB->pMrpIn;

  pDBin = &pDBinBase[inInstance];
  MRP_PROGRAM_TRACE_01 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d]: mrp_in_mic_start entered", inInstance);

  mrp_in_write_red_start_state (pDBin, MRP_USR_ENUM_STATE_IN_MIC, pParams->InID, pParams->IPort, LSA_TRUE);
  mrp_ll_red_state_handler (pChSys, pDBin->pInstance[MRP_INSTANCE_INDEX]);

  mrp_in_mic_init_var (pDBin, pParams);

  pDBin->DBmic.runState  = MRP_IN_OPERATION_ON;
  pDBin->operRole        = MRP_IN_OPER_ROLE_MIC;

  pDBin->pInProjStateBaseCbf = mrp_in_mic_proj_state_cbf;

  mrp_edd_set_link_chg_callback (pChSys, MRP_DEFAULT_INSTANCE, mrp_in_mic_link_chg_cbf);
  mrp_in_ll_init_modul_mic (pDBin, pParams);
  mrp_in_ll_in_trf_chg_host (pDBin, LSA_TRUE);

  if (!pDBin->isStartedFromMrm) /* if an MRM instance is active, then HOST_FWD for IN_CTRL is already activated */
  {
    mrp_in_ll_in_ctrl_chg_host (pDBin, LSA_TRUE);
  }

  mrp_in_write_current_event_diag (pDBin, MRP_IN_RED_DIAG_IN_ALL, LSA_FALSE);
  mrp_in_write_current_event_prm_info (pDBin, MRP_IN_OPER_ROLE_MIC, pParams->MRPIC_MICPosition);

  mrp_in_ll_in_event_handler (pDBin);


  MRP_PROGRAM_TRACE_01 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d]: mrp_in_mic_start exit", inInstance);
}


/*===========================================================================
* FUNCTION : mrp_in_mic_stop
*----------------------------------------------------------------------------
* PURPOSE  : stop one MIC instance
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys     = channel reference
*            inInstance = MRP interconnection instance to be stopped
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mic_stop (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 inInstance)
{
  MRP_IN_MIC_PARAMS_TYPE params;
  MRP_IN_DB_TYPE *pDBin;
  LSA_UINT8 peerMacAddr[] = { 0, 0, 0, 0, 0, 0 };
  MRP_IN_DB_TYPE *pDBinBase = (MRP_IN_DB_TYPE *)pChSys->pChSysDB->pMrpIn;

  pDBin = &pDBinBase[inInstance];

  MRP_PROGRAM_TRACE_01 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d]: mrp_in_mic_stop entered", inInstance);
  
  mrp_in_write_red_start_state (pDBin, MRP_USR_ENUM_STATE_IN_MIC, pDBin->InID, pDBin->IPort, LSA_FALSE);
  mrp_ll_red_state_handler (pChSys, pDBin->pInstance[MRP_INSTANCE_INDEX]);

  mrp_in_write_current_event_diag (pDBin, MRP_IN_RED_DIAG_IN_ALL, LSA_FALSE);
  mrp_in_write_current_event_peer_macaddr (pDBin, peerMacAddr);
  mrp_in_write_current_event_prm_info (pDBin, MRP_IN_OPER_ROLE_OFF, MRP_IN_OPER_ROLE_OFF);

  mrp_in_ll_in_event_handler (pDBin);

  mrp_in_mic_lnkChg_down_timer_stop (pDBin);
  mrp_in_mic_lnkChg_up_timer_stop (pDBin);

  pDBin->pInProjStateBaseCbf = mrp_in_proj_state_off_cbf;

  mrp_edd_remove_link_chg_callback (pChSys, MRP_DEFAULT_INSTANCE, mrp_in_mic_link_chg_cbf);

  if (!mrp_in_check_active_instance(pChSys, inInstance)) /* if another MRP_IN is active, then HOST_FWD for IN_TRF has to be kept activated */
  {
    mrp_in_ll_in_trf_chg_host (pDBin, LSA_FALSE);
  }

  if (!pDBin->isStartedFromMrm && !mrp_in_check_active_instance(pChSys, inInstance)) /* if an MRM instance or another MRP_IN is active, then HOST_FWD for IN_CTRL has to be kept activated */
  {
    mrp_in_ll_in_ctrl_chg_host (pDBin, LSA_FALSE);
  }

  mrp_in_ll_deinit_modul_mic (pDBin);

  params.SetDefaults = MRP_SET_DEFAULTS_ON;
  params.SoftStart   = MRP_SET_SOFT_START_OFF;

  params.adminRole   = MRP_IN_ADMIN_ROLE_OFF;
  params.operRole    = MRP_IN_OPER_ROLE_OFF;

  mrp_in_mic_init_var (pDBin, &params);

  pDBin->DBmic.runState = MRP_IN_OPERATION_OFF;

  MRP_PROGRAM_TRACE_01 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "MIC[inInst=%d]: mrp_in_mic_stop exit", inInstance);
}



