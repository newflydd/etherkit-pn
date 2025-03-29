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
/*  F i l e               &F: mrp_edd.c                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  lower layer functions for edd.                                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef MRP_MESSAGE
/*  2008-01-07  mk    blocking wait removed. wait option in NSM deinit.      */
/*                    set/reset stdby merged to cfgStdby.                    */
/*                    mgr_send_req added. new service                        */
/*                    EDD_NRT_CANCEL_MODE_ALL. check hardware type.          */
/*  2008-01-09  mk    NRT_CANCEL send to close_channel macro.                */
/*  2008-01-11  mk    mrp_edd_get_snd_rqb no more fatal when buffer miss.    */
/*  2008-06-06  mk    added g_MrpllReqTbl_hsr_set_fdb_ard. chk low mem alloc */
/*                    for null ptr.                                          */
/*  2008-06-11  mk    topoCtrl added.                                        */
/*  2008-06-30  mk    new mrp_edd_force_link_up to accelerate link up        */
/*                    hysteresis.                                            */
/*  2008-07-15  mk    traces added for port setting and fdb flush.           */
/*  2008-10-20  mk    edd_wait_time removed.                                 */
/*  2009-07-02  ds    include hierarchy changed, timerSwiBlocking discarded  */
/*                    performance measurement traces introduced              */
/*  2009-10-15  ds    provide only one RQB per port for service              */ 
/*                    EDD_SRV_LINK_STATUS_IND_PROVIDE, as DoCheck=true is    */
/*                    always set now                                         */
/*                    cleanups for LL services                               */
/*  2009-11-24  ds    Don't worry about responses EDD_STS_ERR_TX and         */
/*                    EDD_STS_ERR_TIMEOUT for service EDD_SRV_NRT_SEND,      */
/*                    because at least EDDP uses them, if frame can't be sent*/
/*                    while link is down                                     */
/*  2010-01-27  ds    function mrp_edd_burst_trace optimized                 */
/*  2010-02-16  ds    MRP based on 1 ms (HW) timer instead of 10 ms base     */
/*                    timer constants cleaned up, timer handling corrected   */
/*  2010-05-25  ds    new OHA upper channel related function calls           */
/*  2010-06-28  ds    new function mrp_edd_close_channel, stop EDD related   */
/*                    timers, if close_channel of EDD system channel is      */
/*                    called (AP00977319)                                    */
/*  2010-07-19  ds    new feature MRP MIB implemented according to           */
/*                    IEC 62439-2                                            */
/*  2010-07-22  ds    N-IF: Upper callbacks called within macro, to          */
/*                    be able to operate in different systems                */
/*  2011-01-14  ds    N-IF: rework of burst DB                               */
/*                                                                           */
/*  2011-01-20  ds    function mrp_edd_burst_trace renamed to                */
/*                    mrp_ll_burst_trace and moved to mrp_low.c              */
/*  2011-01-20  ds    functions to handle bursts renamed from mrp_edd_burst..*/
/*                    to mrp_ll_burst.. and moved to mrp_low.c               */
/*  2011-03-23  ds    SSR reworked to get rid of "Non-recoverable ring error"*/
/*                    in event log                                           */
/*  2011-07-20  ds    new function mrp_edd_link_down_ind for processing      */
/*                    of link changes without timer delay                    */
/*  2011-07-21  ds    link down timer (timer_index_down) eliminated          */
/*  2011-06-16  TB    EDD_NRT_SEND_PRIO_ORG -> EDD_NRT_SEND_PRIO_MGMT_HIGH   */
/*  2012-04-18  ds    introduction of N MRP instances per interface          */
/*  2017-11-21  ds    introduction of MRP interconnection                    */
/*  2018-10-19  hh    introduction of N MRP interconnection instances        */

#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID   4
#define MRP_MODULE_ID      LTRC_ACT_MODUL_ID

#include "mrp_int.h"

MRP_FILE_SYSTEM_EXTENSION(MRP_MODULE_ID)

/*---------------------------------------------------------------------------*/
/*             performance measurement settings                              */
/*---------------------------------------------------------------------------*/
#ifdef MRP_CFG_PERF_TRACE

 #ifdef MRP_CFG_PERF_TRACE_EDD
  extern LSA_UINT32 g_perf_measurement_delta, g_perf_measurement_beg, g_perf_measurement_end; /* PERF_MEASUREMENT */
 #endif
#endif

/*---------------------------------------------------------------------------*/
/*                                 defines                                   */
/*---------------------------------------------------------------------------*/
/* ll response */

/*---------------------------------------------------------------------------*/
/*                                 macros                                    */
/*---------------------------------------------------------------------------*/

/* vgl. TFS 324717 pcIOX: MEMORY alloc-free not balanced, size(116)! at device close */
#define MRP_EDD_CHK_LINKCHG_RESPONSE(_rqb) \
{\
  MRP_CHANNEL_TYPE_SYSTEM pSysChannel; \
  pSysChannel = (MRP_CHANNEL_TYPE_SYSTEM)MRP_EDD_LOWER_GET_USERID_PTR(_rqb); \
  if ( (MRP_EDD_LOWER_GET_RESPONSE(_rqb) == EDD_STS_OK_CANCEL) || \
       (MRP_EDD_LOWER_GET_RESPONSE(_rqb) == EDD_STS_OK && pSysChannel->state == MRP_CH_STATE_CLOSING)) \
  {\
    mrp_edd_free_rqb(_rqb);\
    return;\
  }\
  MRP_FATAL1((LSA_UINT8)((MRP_EDD_LOWER_GET_RESPONSE((_rqb)) == EDD_STS_OK)));\
}

/* TFS 1265868 pcIOX Classic - Fatal error in EB200P for MRP and MRPD -> abort request handling, but don't dealloc rqbs here, because they will be deallocated in mrp_deinit_rqb () */
/* applied to services: EDD_SRV_MULTICAST_FWD_CTRL, EDD_SRV_MULTICAST, EDD_SRV_SWITCH_FLUSH_FILTERING_DB, EDD_SRV_SWITCH_SET_PORT_STATE                                             */
#define MRP_EDD_CHK_RESPONSE(_rqb) \
{\
  MRP_CHANNEL_TYPE_SYSTEM pSysChannel; \
  pSysChannel = (MRP_CHANNEL_TYPE_SYSTEM)MRP_EDD_LOWER_GET_USERID_PTR(_rqb); \
  if ( (MRP_EDD_LOWER_GET_RESPONSE(_rqb) == EDD_STS_OK_CANCEL) || \
       (MRP_EDD_LOWER_GET_RESPONSE(_rqb) == EDD_STS_OK && pSysChannel->state == MRP_CH_STATE_CLOSING)) \
  {\
    return;\
  }\
  MRP_FATAL1((LSA_UINT8)((MRP_EDD_LOWER_GET_RESPONSE((_rqb)) == EDD_STS_OK)));\
}

#define MRP_LL_RQB_ENUM_ASSERT(_pid)    \
{ \
  if ((_pid >= MRP_LL_SND_RQB_ENUM_TBL_END) || (_pid & 0x00000001)) \
    MRP_FATAL; \
}

/*---------------------------------------------------------------------------*/
/*                     nrt-api sx table conversion                           */
/*---------------------------------------------------------------------------*/
#define MRP_GET_IN_NRT_SND_FRAME_TABLE_INDEX(_poolID) \
( \
  (_poolID - MRP_LL_SND_RQB_ENUM_IN_TYPES_START_OFFSET) \
)

/* send queue macros */
#define MRP_LL_TX_QUEUE_PREFIX_SIZE   (sizeof(LSA_UINT32) * 2)

#ifdef MRP_CFG_TX_BUFFER_EXTRA_INFO_TO_END
#define MRP_LL_GET_TX_QUEUE(_buf) \
         (MRP_LL_SND_RQB_ENUM_T)(*(LSA_UINT32*)((_buf) + MRP_NRT_TX_BUF_SIZE));

#define MRP_LL_GET_TX_INSTANCE(_buf) \
        (LSA_UINT16)(*(LSA_UINT16*)((_buf) + MRP_NRT_TX_BUF_SIZE + sizeof(LSA_UINT32) ));
#define MRP_LL_GET_TX_ININSTANCE(_buf) \
        (LSA_UINT16)(*(LSA_UINT16*)((_buf) + MRP_NRT_TX_BUF_SIZE + sizeof(LSA_UINT32) + sizeof(LSA_UINT16) ));

 #define MRP_LL_SET_TX_QUEUE(_buf, _pool) \
         /* buffer address must be 32-bit aligned */\
         *(LSA_UINT32*)((_buf) + MRP_NRT_TX_BUF_SIZE) = (_pool); /* pool number */

 #define MRP_LL_SET_TX_INSTANCE(_buf, _instance) \
        /* buffer address must be 32-bit aligned: the MRP instance is in the 2 low-bytes and the MRP inInstance in the 2 high-bytes*/\
        *(LSA_UINT16*)(_buf + MRP_NRT_TX_BUF_SIZE + sizeof(LSA_UINT32) ) = (_instance); /* pool number */
 #define MRP_LL_SET_TX_ININSTANCE(_buf, _inInstance) \
        /* buffer address must be 32-bit aligned: the MRP instance is in the 2 low-bytes and the MRP inInstance in the 2 high-bytes*/\
        *(LSA_UINT16*)(_buf + MRP_NRT_TX_BUF_SIZE + sizeof(LSA_UINT32) + sizeof(LSA_UINT16) ) = (_inInstance); /* pool number */

 #define MRP_LL_SET_PAYLOAD_ADDR(_buf) \
        /* buffer address has to be incremented by number of prefix bytes */\
        (_buf) += 0;

 #define MRP_LL_GET_BASE_ADDR(_payload) \
        /* payload address is equal to buffer base address */\
        (_payload + 0);

 #define MRP_LL_RESET_TX_QUEUE(_payload) \
        /* buffer address must be 32-bit aligned */\
        *(LSA_UINT32*)((_payload) + MRP_NRT_TX_BUF_SIZE) = 0; /* reset pool number */

 #define MRP_LL_RESET_TX_INSTANCE(_payload) \
        /* buffer address must be 32-bit aligned: the MRP instance is in the 2 low-bytes and the MRP inInstance in the 2 high-bytes*/\
        *(LSA_UINT16*)((_payload) + MRP_NRT_TX_BUF_SIZE + sizeof(LSA_UINT32) ) = 0; /* reset instance number*/
 #define MRP_LL_RESET_TX_ININSTANCE(_payload) \
        /* buffer address must be 32-bit aligned: the MRP instance is in the 2 low-bytes and the MRP inInstance in the 2 high-bytes*/\
        *(LSA_UINT16*)((_payload) + MRP_NRT_TX_BUF_SIZE + sizeof(LSA_UINT32) + sizeof(LSA_UINT16) ) = 0; /* reset instance number*/

#else 

 /* get queue and instance info from send buffer */
 #define MRP_LL_GET_TX_QUEUE(_payload) \
        (MRP_LL_SND_RQB_ENUM_T)(*(LSA_UINT32*)((_payload) - MRP_LL_TX_QUEUE_PREFIX_SIZE));

 #define MRP_LL_GET_TX_INSTANCE(_payload) \
        (LSA_UINT16)(*(LSA_UINT16*)(((_payload) - MRP_LL_TX_QUEUE_PREFIX_SIZE) + sizeof(LSA_UINT32)));
 #define MRP_LL_GET_TX_ININSTANCE(_payload) \
        (LSA_UINT16)(*(LSA_UINT16*)(((_payload) - MRP_LL_TX_QUEUE_PREFIX_SIZE) + sizeof(LSA_UINT32) + sizeof(LSA_UINT16)));

 /* place queue and instance info to send buffer */
 #define MRP_LL_SET_TX_QUEUE(_buf, _pool) \
        /* buffer address must be 32-bit aligned */\
        *(LSA_UINT32*)(_buf) = (_pool); /* pool number */
        
 #define MRP_LL_SET_TX_INSTANCE(_buf, _instance) \
        /* buffer address must be 32-bit aligned: the MRP instance is in the 2 low-bytes and the MRP inInstance in the 2 high-bytes*/\
        *(LSA_UINT16*)(_buf + sizeof(LSA_UINT32)) = (_instance); /* MRP instance number */
 #define MRP_LL_SET_TX_ININSTANCE(_buf, _inInstance) \
        /* buffer address must be 32-bit aligned: the MRP instance is in the 2 low-bytes and the MRP inInstance in the 2 high-bytes*/\
        *(LSA_UINT16*)(_buf + sizeof(LSA_UINT32) + sizeof(LSA_UINT16)) = (_inInstance); /* MRP interconnection instance number */

 #define MRP_LL_SET_PAYLOAD_ADDR(_buf) \
        /* buffer address has to be incremented by number of prefix bytes */\
        (_buf) += MRP_LL_TX_QUEUE_PREFIX_SIZE;

 #define MRP_LL_GET_BASE_ADDR(_payload) \
       /* payload address has to be decremented by number of prefix bytes */\
       (_payload - MRP_LL_TX_QUEUE_PREFIX_SIZE);

 /* reset queue and instance info in send buffer */
 #define MRP_LL_RESET_TX_QUEUE(_payload) \
        /* buffer address must be 32-bit aligned */\
        *(LSA_UINT32*)(_payload - MRP_LL_TX_QUEUE_PREFIX_SIZE) = 0; /* reset pool number */

 #define MRP_LL_RESET_TX_INSTANCE(_payload) \
        /* buffer address must be 32-bit aligned: the MRP instance is in the 2 low-bytes and the MRP inInstance in the 2 high-bytes*/\
        *(LSA_UINT16*)((_payload - MRP_LL_TX_QUEUE_PREFIX_SIZE) + sizeof(LSA_UINT32)) = 0; /* reset instance number */
 #define MRP_LL_RESET_TX_ININSTANCE(_payload) \
        /* buffer address must be 32-bit aligned: the MRP instance is in the 2 low-bytes and the MRP inInstance in the 2 high-bytes*/\
        *(LSA_UINT16*)((_payload - MRP_LL_TX_QUEUE_PREFIX_SIZE) + sizeof(LSA_UINT32) + sizeof(LSA_UINT16)) = 0; /* reset interconnection instance number */
#endif

/*---------------------------------------------------------------------------*/
/*                                 types                                     */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                            forward declarations                           */
/*---------------------------------------------------------------------------*/
static LSA_VOID mrp_edd_link_chg_cbf (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT16 port, LSA_UINT8 linkUp);

static LSA_VOID mrp_edd_init (const MRP_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance);
static LSA_VOID mrp_edd_deinit (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance);

/*---------------------------------------------------------------------------*/
/* static variables                                                          */
/*---------------------------------------------------------------------------*/

/* Default maximum number of send RQBs */
static const LSA_UINT16 mrpDefaultSendRqbMax[] =
{
  MRP_LL_SND_RES_NSM_WRK_LNK_CHG       /* 3, MRP_LL_SND_RQB_ENUM_NSM_WRK_LNK_CHG_TOP    */

                                       /* IN_ORIGINATOR (RSTP+) */
  ,MRP_LL_SND_RES_MRP_IN_WRK_TC_ORIG   /* 6, MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_ORIG_TOP */

                                       /* MRP interconnection */
  ,MRP_LL_SND_RES_MRP_IN_WRK   /* 6, MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_TOP,     */
                               /* 8, MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_POLL_TOP,   */
                               /* 8, MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_LNK_CHG_TOP */

  ,MRP_LL_SND_RES_MRP_IN_TRF   /* 3, MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_TC_TOP,     */
                               /* 4, MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_POLL_TOP,   */
                               /* 4, MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_LNK_CHG_TOP */

#ifdef MRP_CFG_PLUGIN_0_MRM
  ,MRP_LL_SND_RES_PLUGIN_MRM   /* 6, MRP_LL_SND_RQB_ENUM_NSM_TST_TOP,           */
                               /* 8, MRP_LL_SND_RQB_ENUM_NSM_TPL_CHG_TOP        */
#endif
#ifdef MRP_CFG_PLUGIN_1_MRM_EXT
  ,MRP_LL_SND_RES_PLUGIN_MRM_EXT /* 6, MRP_LL_SND_RQB_ENUM_NSM_EXT_TOP          */
#endif
#ifdef MRP_CFG_PLUGIN_2_HSR
  ,MRP_LL_SND_RES_PLUGIN_HSR     /* 6, MRP_LL_SND_RQB_ENUM_TPLG_CHG_TOP,        */
                                 /* 6, MRP_LL_SND_RQB_ENUM_TST_TOP,             */
                                 /* 6, MRP_LL_SND_RQB_ENUM_STBY_TOP,            */
                                 /* 6, MRP_LL_SND_RQB_ENUM_TPLG_CHG_STDBY_TOP   */
#endif
  ,MRP_LL_SND_RQB_ENUM_TBL_END   /* end of table */
};



/*===========================================================================
* FUNCTION : mrp_edd_get_nrt_snd_rqb_ptr
*----------------------------------------------------------------------------
* PURPOSE  : checks the poolID value and if it relates to an interconnetion instance
*            it returns the according entry of the InInstance, ohterwise it returns
*            the value of the MRP instance.
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pSendRqbData = (output value) pointer to the correct sendRqbData-set debendend on
*                           the poolID and the correct MRP and MRP interconnection instance
*            pDBedd     = pointer to the Data Base of the EDD
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            poolID     = according send rqb enum type 
* OUTPUTS  : pSendRqbData
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_edd_get_nrt_snd_rqb_ptr(MRP_EDD_SEND_RQB_DATA_SET **pSendRqbData,const MRP_EDD_DB_TYPE *pDBedd, const LSA_UINT16 *pInstance, MRP_LL_SND_RQB_ENUM_T poolID)
{
  /* check if the frame is an MRP or an MRP interconnection frame */
  if((poolID >= MRP_LL_SND_RQB_ENUM_IN_TYPES_START_OFFSET)
      && (poolID < MRP_LL_SND_RQB_ENUM_IN_TYPES_END_OFFSET))
  {
    if(pInstance[MRP_IN_INSTANCE_INDEX] >= EDD_CFG_MAX_MRP_IN_INSTANCE_CNT)
    {
      MRP_FATAL;
    }
    else
    {
      /* get the correct data set of the according MRP interconnection instance */
      *pSendRqbData = &pDBedd->pInstData[pInstance[MRP_INSTANCE_INDEX]].sendRqbInInstData[pInstance[MRP_IN_INSTANCE_INDEX]][poolID - MRP_LL_SND_RQB_ENUM_IN_TYPES_START_OFFSET];
    }
  }
  else
  {
    /* get the data set of the MRP instance */
    *pSendRqbData = &pDBedd->pInstData[pInstance[MRP_INSTANCE_INDEX]].sendRqbInstData[poolID];
  }
}

/*===========================================================================
* FUNCTION : mrp_edd_process_req_lower_done
*----------------------------------------------------------------------------
* PURPOSE  : processes any EDD request responses from lower layer cbf 
*            mrp_edd_request_lower_done
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pRQB   = lower rqb reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_process_req_lower_done (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB)
{
  MRP_CHANNEL_TYPE_SYSTEM pChSys  = MRP_EDD_LOWER_GET_SYS_CHANNEL_FROM_RQB(pRQB);

  switch (MRP_EDD_LOWER_GET_SERVICE(pRQB))
  {
    case EDD_SRV_NRT_RECV:
    {
      MRP_FATAL1 ((MRP_EDD_LOWER_GET_RESPONSE (pRQB) == EDD_STS_OK) || (MRP_EDD_LOWER_GET_RESPONSE (pRQB) == EDD_STS_OK_CANCEL));
      mrp_edd_req_rcv_done (pRQB);
    }
    break;

    case EDD_SRV_NRT_SEND:
    {
      switch (MRP_EDD_LOWER_GET_RESPONSE (pRQB))
      {
        case EDD_STS_OK:
        mrp_edd_req_snd_done (pRQB);
        break;

        case EDD_STS_ERR_TIMEOUT:
        case EDD_STS_ERR_TX:
        MRP_LOWER_TRACE_00 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "<< EDD_SRV_NRT_SEND resp. ERR_TIMEOUT or ERR_TX");
        mrp_edd_req_snd_done (pRQB);
        break;

        case EDD_STS_OK_CANCEL:
        /* RQ 1004165 MRP SOC1 Fatal : assert: rc == WAIT_OBJECT_0  during restart of the pcIOX. */
        /* RQB has to be put back to the corresponding MRP rqb pool first to avoid FATAL error during close channel procedure, when mrp_edd_deinit_rqb() is called */
        MRP_LOWER_TRACE_00 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "<< EDD_SRV_NRT_SEND resp. with EDD_STS_OK_CANCEL");
        mrp_edd_req_snd_done (pRQB);
        break;

        default:
        MRP_LOWER_TRACE_01 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "<< EDD_SRV_NRT_SEND resp=0x%x", MRP_EDD_LOWER_GET_RESPONSE (pRQB));
        mrp_edd_req_snd_done (pRQB);
        break;
      }
    }
    break;

    case EDD_SRV_NRT_CANCEL:
    {
      MRP_FATAL1 (MRP_EDD_LOWER_GET_RESPONSE (pRQB) == EDD_STS_OK);
      mrp_edd_req_nrt_cancel_done (pRQB);
      mrp_edd_req_close_ch_sys (pChSys);
    }
    break;

    case EDD_SRV_LINK_STATUS_IND_PROVIDE:
    {
      MRP_EDD_CHK_LINKCHG_RESPONSE (pRQB);
      mrp_edd_req_linkChg_done (pRQB);
    }
    break;

    case EDD_SRV_GET_PARAMS:
    {
      LSA_UINT16 mrpMaxInstance;
      LSA_UINT16 mrpInMaxInstance;
      
      MRP_FATAL1(MRP_EDD_LOWER_GET_RESPONSE(pRQB) == EDD_STS_OK);
      mrp_edd_req_get_params_done (pRQB);
      mrp_edd_check_if_params (pChSys); 
      
      mrpMaxInstance = mrp_sys_get_num_of_instances (pChSys);
      mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pChSys);
      
      mrp_sys_timer_init (pChSys, mrpMaxInstance, mrpInMaxInstance);
      mrp_edd_init (pChSys, mrpMaxInstance, mrpInMaxInstance);
      mrp_sys_init (pChSys, mrpMaxInstance, mrpInMaxInstance);
      /* retrieve port mac address */
      mrp_edd_req_get_port_params (pChSys, 1);
    }
    break;

    case EDD_SRV_GET_PORT_PARAMS:
    {
      MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
      LSA_UINT16 numOfPorts;

      MRP_FATAL1(MRP_EDD_LOWER_GET_RESPONSE(pRQB) == EDD_STS_OK);
      mrp_edd_req_get_port_params_done (pRQB);

      pDBedd->PortID4MacAddr++;
      numOfPorts = mrp_sys_get_num_of_ports (pChSys);

      if (pDBedd->PortID4MacAddr <= numOfPorts) 
      {
        mrp_edd_req_get_port_params (pChSys, pDBedd->PortID4MacAddr);
      }
      else 
      {
        pDBedd->PortID4MacAddr = 1;
        mrp_edd_check_if_port_params (pChSys);
        mrp_usr_call_upper_cbf (pChSys, pChSys->pRQB);
        pChSys->pRQB = LSA_NULL;
      }
    }
    break;

    default:
    {
      MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

      MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL (pDBedd->llQueue.pPendingLLreq, LSA_NULL));
      MRP_FATAL1(MRP_EDD_LOWER_GET_SERVICE(pRQB) == pDBedd->llQueue.pPendingLLreq->service);

      MRP_LOWER_TRACE_02 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "<< mrp_edd_process_req_lower_done: service=0x%x, cbf=0x%x", MRP_EDD_LOWER_GET_SERVICE (pRQB), pDBedd->llQueue.pPendingLLreq->cbf);

      mrp_ll_burst_do (pChSys);
    }
    break;
  }
}

/*===========================================================================
* FUNCTION : mrp_edd_process_open_lower_done
*----------------------------------------------------------------------------
* PURPOSE  : processes EDD open channel response from lower layer cbf 
*            mrp_edd_request_lower_done
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pRQB   = lower rqb reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_process_open_lower_done (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB)
{
  MRP_CHANNEL_TYPE_SYSTEM pChSys  = MRP_EDD_LOWER_GET_SYS_CHANNEL_FROM_RQB(pRQB);

  MRP_FATAL1(MRP_EDD_LOWER_GET_RESPONSE(pRQB) == EDD_STS_OK);

  mrp_edd_req_open_ch_sys_done (pRQB);
  mrp_edd_req_get_params (pChSys);
}

/*===========================================================================
* FUNCTION : mrp_edd_process_close_lower_done
*----------------------------------------------------------------------------
* PURPOSE  : processes EDD close channel response from lower layer cbf 
*            mrp_edd_request_lower_done
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pRQB   = lower rqb reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_process_close_lower_done (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB)
{
  LSA_UINT16 rc;
  LSA_UINT32 tmpTraceIdx;
  MRP_CHANNEL_TYPE_SYSTEM pChSys  = MRP_EDD_LOWER_GET_SYS_CHANNEL_FROM_RQB(pRQB);
  MRP_IF_DB_TYPE *pDBif = (MRP_IF_DB_TYPE *)pChSys->pChSysDB->pInterfacePara;

  MRP_FATAL1(MRP_EDD_LOWER_GET_RESPONSE(pRQB) == EDD_STS_OK);

  mrp_edd_req_close_ch_sys_done (pRQB);
  mrp_sys_deinit (pChSys, pDBif->mrpMaxInstance, pDBif->mrpInMaxInstance);
  mrp_edd_deinit (pChSys, pDBif->mrpMaxInstance, pDBif->mrpInMaxInstance);

  mrp_sys_timer_deinit (pChSys);
  /* save traceIdx for final TRACE, because IF details will be deallocated here */
  tmpTraceIdx = pChSys->pChSysDB->traceIdx;
  mrp_sys_if_db_deinit(pChSys);

  /* finish upper rqb - no "mrp_usr_call_upper_cbf_proc_next_req" because channel is already deallocated */
  mrp_usr_release_channel_data (pChSys);

  MRP_RQB_SET_RESPONSE(pChSys->pRQB, MRP_RSP_OK);

  mrp_usr_call_upper_cbf (pChSys, pChSys->pRQB);
  pChSys->pRQB = LSA_NULL;

  MRP_RELEASE_PATH_INFO(&rc, pChSys->sysptr, pChSys->detailptr);
  MRP_FATAL1(rc == LSA_RET_OK);

  LSA_UNUSED_ARG(tmpTraceIdx);   /* hhr: to please lint in case traces are disabled. */
  MRP_LOWER_TRACE_00(tmpTraceIdx, LSA_TRACE_LEVEL_NOTE, "<< mrp_close_channel(EDD) successful");
}

/*===========================================================================
* FUNCTION : mrp_edd_request_lower_done
*----------------------------------------------------------------------------
* PURPOSE  : handles lower response for edd
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pRQB = lower rqb reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_request_lower_done (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB)
{

  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL));
  MRP_FATAL1(MRP_EDD_LOWER_GET_USERID_PTR(pRQB));

  switch (MRP_EDD_LOWER_GET_OPCODE(pRQB))
  {
  case EDD_OPC_OPEN_CHANNEL:
    mrp_edd_process_open_lower_done (pRQB);
    break;

  case EDD_OPC_CLOSE_CHANNEL:
    mrp_edd_process_close_lower_done (pRQB);
    break;

  case EDD_OPC_REQUEST:
    mrp_edd_process_req_lower_done (pRQB);
    break;

  default:
    MRP_FATAL;
    break;
  }
}

/*===========================================================================
* FUNCTION : mrp_edd_req_open_ch_sys
*----------------------------------------------------------------------------
* PURPOSE  : open a channel to EDD (system channel)
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_open_ch_sys (const MRP_CHANNEL_TYPE_SYSTEM pChSys)
{
  MRP_EDD_LOWER_RQB_TYPE *p_llRQB;
  MRP_EDD_LOWER_OPEN_CHANNEL_PTR_TYPE pRQBOpen;

  MRP_EDD_ALLOC_RQB(pChSys, p_llRQB, sizeof(MRP_EDD_RQB_OPEN_CHANNEL_TYPE), 0);

  MRP_EDD_LOWER_SET_OPCODE(p_llRQB, EDD_OPC_OPEN_CHANNEL);
  pRQBOpen = MRP_EDD_LOWER_GET_PPARAM_OPEN_CHANNEL(p_llRQB);
  pRQBOpen->SysPath     = pChSys->sys_path;
  pRQBOpen->Cbf         = mrp_edd_request_lower_done;
  pRQBOpen->HandleUpper = pChSys->myHandle;

  MRP_EDD_OPEN_CHANNEL_LOWER(p_llRQB, pChSys->sysptr);
}


/*===========================================================================
* FUNCTION : mrp_edd_req_open_ch_sys_done
*----------------------------------------------------------------------------
* PURPOSE  : finish EDD open channel
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pRQB   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_open_ch_sys_done (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB)
{
  MRP_CHANNEL_TYPE_SYSTEM pChSys  = MRP_EDD_LOWER_GET_SYS_CHANNEL_FROM_RQB(pRQB);
  MRP_EDD_LOWER_OPEN_CHANNEL_PTR_TYPE pLowerOpenChl;
  MRP_UPPER_ARGS_PTR_TYPE pArgs;

  pLowerOpenChl = MRP_EDD_LOWER_GET_PPARAM_OPEN_CHANNEL(pRQB);

  /* get lower handle */
  pChSys->handle_lower_edd = pLowerOpenChl->HandleLower;
  pChSys->state = MRP_CH_STATE_READY;
  /* finish upper rqb */
  pArgs = MRP_RQB_GET_ARGS_PTR(pChSys->pRQB);
  pArgs->channel.open.handle = pChSys->myHandle;
  MRP_RQB_SET_RESPONSE(pChSys->pRQB, MRP_RSP_OK);

  mrp_edd_free_rqb(pRQB);
}

/*===========================================================================
* FUNCTION : mrp_edd_open_channel
*----------------------------------------------------------------------------
* PURPOSE  : open a channel to EDD (system channel)
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference
* OUTPUTS  : resp   = result of checks and memory allocation
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT16 mrp_edd_open_channel (const MRP_CHANNEL_TYPE_SYSTEM pChSys)
{
  LSA_UINT16 resp;

  MRP_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_NOTE, ">> mrp_edd_open_channel[pChSys=0x%x] entered", pChSys);

  resp = mrp_sys_ch_db_init (pChSys);

  if (resp == MRP_RSP_OK)
  {
    resp = mrp_sys_if_db_init (pChSys);

    if (resp == MRP_RSP_OK)
    {
      mrp_edd_req_open_ch_sys(pChSys);
    }
  }
  MRP_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_NOTE, "<< mrp_edd_open_channel[pChSys=0x%x] finished", pChSys);

  return resp;
}

/*===========================================================================
* FUNCTION : mrp_edd_req_close_ch_sys
*----------------------------------------------------------------------------
* PURPOSE  : close a channel to EDD (system channel)
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference for edd system channel
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_close_ch_sys (const MRP_CHANNEL_TYPE_SYSTEM pChSys)
{
  MRP_EDD_LOWER_RQB_PTR_TYPE pRQB;

  MRP_EDD_ALLOC_RQB(pChSys, pRQB, sizeof(LSA_UINT32),0);
  MRP_EDD_LOWER_SET_OPCODE(pRQB, EDD_OPC_CLOSE_CHANNEL);
  MRP_EDD_LOWER_SET_HANDLE(pRQB, pChSys->handle_lower_edd);

  MRP_EDD_CLOSE_CHANNEL_LOWER(pRQB, pChSys->sysptr);
}


/*===========================================================================
* FUNCTION : mrp_edd_req_close_ch_sys_done
*----------------------------------------------------------------------------
* PURPOSE  : finish EDD close channel
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pRQB   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_close_ch_sys_done (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB)
{
  MRP_CHANNEL_TYPE_SYSTEM pChSys  = MRP_EDD_LOWER_GET_SYS_CHANNEL_FROM_RQB(pRQB);

  /* reset lower handle */
  pChSys->handle_lower_edd = 0;
  mrp_edd_free_rqb(pRQB);
}


/*===========================================================================
* FUNCTION : mrp_edd_req_nrt_cancel
*----------------------------------------------------------------------------
* PURPOSE  : cancel NRT request in EDD 
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference for edd system channel
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_nrt_cancel (const MRP_CHANNEL_TYPE_SYSTEM pChSys)
{
  MRP_EDD_LOWER_RQB_PTR_TYPE pRQB;
  MRP_EDD_UPPER_NRT_CANCEL_PTR_TYPE pParamCancel;

  MRP_EDD_ALLOC_RQB(pChSys, pRQB, sizeof(MRP_EDD_RQB_NRT_CANCEL_TYPE), EDD_SRV_NRT_CANCEL);
  pParamCancel = MRP_EDD_LOWER_GET_PPARAM_NRT_CANCEL(pRQB);
  pParamCancel->Mode = EDD_NRT_CANCEL_MODE_ALL;
  pParamCancel->RequestID = 0;

  MRP_EDD_REQUEST_LOWER(pRQB, pChSys->sysptr);
}


/*===========================================================================
* FUNCTION : mrp_edd_req_nrt_cancel_done
*----------------------------------------------------------------------------
* PURPOSE  : finish EDD NRT cancel
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pRQB   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_nrt_cancel_done (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB)
{
  mrp_edd_free_rqb(pRQB);
}


/*===========================================================================
* FUNCTION : mrp_edd_close_channel
*----------------------------------------------------------------------------
* PURPOSE  : initiate closing of EDD system channel 
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference for edd system channel
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_close_channel (const MRP_CHANNEL_TYPE_SYSTEM pChSys)
{
  LSA_UINT16 idx, i, numOfPorts, sysCnt, mrpMaxInstance;
  MRP_TIMER_DB_TYPE *pDBtimer;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  pDBtimer        = (MRP_TIMER_DB_TYPE *)pChSys->pChSysDB->pInterfaceTimer;
  MRP_FATAL1(pDBtimer->pCh == pChSys);
  
  MRP_PROGRAM_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, ">> mrp_edd_close_channel[pChSys=0x%x] entered", pChSys);

  pChSys->state = MRP_CH_STATE_CLOSING;

  /* if the last system channel is closed, stop the cyclic component timer as well */
  sysCnt = mrp_sys_get_edd_sys_channel_cnt();
  if (sysCnt == 1) /* if it's the last system channel, which is being closed */
  {
    mrp_sys_cyclic_timer_stop (g_Mrp.cyclicTimer.uid.uvar16);
  }
  
  /* cancel further lower layer (EDD) requests */
  mrp_edd_req_nrt_cancel(pChSys);

  mrpMaxInstance = mrp_sys_get_num_of_instances(pChSys);
  /* stop any lower layer (EDD) related timer */
  for (i = 0; i < mrpMaxInstance; i++)
  {
    mrp_sys_timer_stop (pDBedd->pInstData[i].TimerIdTplgChg);
  }
  
  mrp_sys_timer_stop (pDBedd->TimerIdCyclic);

  numOfPorts = mrp_sys_get_num_of_ports (pChSys);

  for (i = 0; i < numOfPorts; i++) 
  {
    mrp_sys_timer_stop (pDBedd->pLinkData[i].timer_index_up);
  }

  for (idx = 1; idx < pDBtimer->timerTblSize; idx++)
  {
    if (pDBtimer->pTimerPara[idx].state == MRP_TIMER_STS_T_RUNNING)
    {
      MRP_LOWER_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "--> mrp_edd_close_channel: state of inherited timer[idx=%i] reset", idx);
      /* reset timer state of inherited timer */
      pDBtimer->pTimerPara[idx].state = MRP_TIMER_STS_T_STOPPED;
    }
  }

  MRP_PROGRAM_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "<< mrp_edd_close_channel[pChSys=0x%x] exited", pChSys);


}

/*===========================================================================
* FUNCTION : mrp_edd_write_rport_states_to_mib
*----------------------------------------------------------------------------
* PURPOSE  : save the current ring port states to MRP MIB
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            instance = affected instance
*            rqbNum   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_write_rport_states_to_mib (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_LL_SREQ_RQB_ENUM_T rqbNum)
{
  MRP_EDD_LOWER_RQB_PTR_TYPE pRQB;
  MRP_EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE pPSParam;
  LSA_UINT16 k, numOfPorts, portNum;
  MRP_MIB_ENUM_RPORT_STATE portStateMIB;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  LSA_UNUSED_ARG(rqbNum);

  pRQB = pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_PORT_STATE];
  pPSParam = MRP_EDD_LOWER_GET_PPARAM_SET_PORT_STATE(pRQB);

  numOfPorts = mrp_sys_get_num_of_ports (pChSys);
  for (k = 0; k < numOfPorts; k++) 
  {
    portNum = k + 1;
    portStateMIB = mrp_mib_map_edd_and_link_state_to_mib_state (pChSys, instance, (LSA_UINT16)(k + 1), pPSParam->PortIDState[k], pDBedd->pLinkData[k].currentState);
    LSA_UNUSED_ARG(portNum);   /* hhr: to please lint in case traces are disabled. */
    LSA_UNUSED_ARG(portStateMIB);   /* hhr: to please lint in case traces are disabled. */
    MRP_LOWER_TRACE_03(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_mib_map_edd_and_link_state_to_mib_state[inst=%d] returns: rport=%u portStateMIB=%u", instance, portNum, portStateMIB);
  }
}

/*===========================================================================
* FUNCTION : mrp_edd_check_if_port_params
*----------------------------------------------------------------------------
* PURPOSE  : check interface related edd port params
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference for edd system channel
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_check_if_port_params (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys)
{
  LSA_UINT16 i, rportDefCnt, rportCnt, inPortCnt;
  MRP_IF_DB_TYPE *pDBif = (MRP_IF_DB_TYPE *)pChSys->pChSysDB->pInterfacePara;
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  for (i = 0, rportCnt = 0, rportDefCnt = 0, inPortCnt = 0; i < pDBif->numOfPorts; i++)
  {
    if (pDBif->pPortData[i].rpType == EDD_MRP_RING_PORT)
      rportCnt++;
    if (pDBif->pPortData[i].rpType == EDD_MRP_RING_PORT_DEFAULT)
      rportDefCnt++;
    if (pDBif->pPortData[i].inPortType == EDD_SUPPORTS_MRP_INTERCONN_PORT_CONFIG_YES)
      inPortCnt++;
  }

  /* AP01182311: rportCnt == 1 is a valid configuration, e.g. SOC */
  if (rportDefCnt == 1 || rportCnt > pDBif->numOfPorts || rportDefCnt > pDBif->numOfPorts)
  {
    MRP_LOWER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_FATAL, ">> ambiguous number of ringports: defRportCnt=%i, rportCnt=%i",
      rportDefCnt, rportCnt);
    MRP_FATAL;
  }
  if (pDBif->mrpSupportedRoleInst0 == EDD_MRP_ROLE_NONE && (rportCnt || rportDefCnt))
  {
    MRP_LOWER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_FATAL, ">> rport configuration mismatch: mrpSupportedRoleInst0=%i, defRportCnt=%i, rportCnt=%i",
      pDBif->mrpSupportedRoleInst0, rportDefCnt, rportCnt);
    MRP_FATAL;
  }
  /* RQ 1706269 Fix MRP for Micrel KSZ8842 */
  if (pDBif->mrpMaxInstance && !rportCnt && !rportDefCnt)
  {
    MRP_LOWER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_FATAL, ">> rport configuration mismatch: mrpMaxInstance=%d, but NO port declared as default or possible ringports!", pDBif->mrpMaxInstance);
    MRP_FATAL;
  }
  if (!pDBif->mrpMaxInstance && (rportCnt || rportDefCnt))
  {
    MRP_LOWER_TRACE_02(traceIdx, LSA_TRACE_LEVEL_FATAL, ">> rport configuration mismatch: mrpMaxInstance=0, but ports declared as default or possible ringport, rportCnt/DefCnt=%d/%d!", rportCnt, rportDefCnt);
    MRP_FATAL;
  }
  if (pDBif->mrpInSupportedRoleInst0 != EDD_SUPPORTED_MRP_INTERCONN_ROLE_CAP_NONE && (pDBif->mrpInMaxInstance - pDBif->mrpRstpInstances) > inPortCnt)
  {
    MRP_LOWER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> inPort configuration mismatch: mrpInMaxInstance=%d exceeds the number of possible interconnection ports inPortCnt=%d !", pDBif->mrpInMaxInstance, inPortCnt);
    MRP_FATAL;
  }
  if (!pDBif->mrpInMaxInstance && inPortCnt)
  {
    MRP_LOWER_TRACE_01 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> inPort configuration mismatch: mrpInMaxInstance=0, but at least one port is declared as possible interconnection port, inPortCnt=%d!", inPortCnt);
    MRP_FATAL;
  }
  if (pDBif->mrpInSupportedRoleInst0 != EDD_SUPPORTED_MRP_INTERCONN_ROLE_CAP_NONE && !inPortCnt)
  {
    MRP_LOWER_TRACE_01 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> inPort configuration mismatch: mrpInSupportedRoleInst0=0x%x, but no port has the capability to be an interconnection port!", pDBif->mrpInSupportedRoleInst0);
    MRP_FATAL;
  }
}

/*===========================================================================
* FUNCTION : mrp_edd_check_if_params
*----------------------------------------------------------------------------
* PURPOSE  : check interface related edd params
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference for edd system channel
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_check_if_params (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys)
{
  MRP_IF_DB_TYPE *pDBif = (MRP_IF_DB_TYPE *)pChSys->pChSysDB->pInterfacePara;

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  /* check edd details for mrpDefaultRoleInst0 */
  if (!(pDBif->mrpDefaultRoleInst0 == EDD_MRP_ROLE_NONE || pDBif->mrpDefaultRoleInst0 == EDD_MRP_ROLE_CLIENT
#ifdef MRP_CFG_PLUGIN_0_MRM
      || pDBif->mrpDefaultRoleInst0 == EDD_MRP_ROLE_MANAGER
#endif
#ifdef MRP_CFG_PLUGIN_1_MRM_EXT
      || pDBif->mrpDefaultRoleInst0 == EDD_MRP_ROLE_AUTOMANAGER
#endif
      )
      )
  {
    MRP_LOWER_TRACE_01 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> unknown value for mrpDefaultRoleInst0=0x%x in edd details", pDBif->mrpDefaultRoleInst0);
    MRP_FATAL;
  }

  /* check edd details for mrpSupportedRoleInst0 */
#ifdef MRP_CFG_PLUGIN_0_MRM
  if (pDBif->mrpDefaultRoleInst0 == EDD_MRP_ROLE_MANAGER && !(pDBif->mrpSupportedRoleInst0 & EDD_MRP_ROLE_CAP_MANAGER)
      )
  {
    MRP_LOWER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> insufficient mrpSupportedRoleInst0=0x%x (need=0x%x)", pDBif->mrpSupportedRoleInst0, EDD_MRP_ROLE_CAP_MANAGER);
    MRP_FATAL;

  }
#endif
#ifdef MRP_CFG_PLUGIN_1_MRM_EXT
  if (pDBif->mrpDefaultRoleInst0 == EDD_MRP_ROLE_AUTOMANAGER && !(pDBif->mrpSupportedRoleInst0 & EDD_MRP_ROLE_CAP_AUTOMANAGER)
      )
  {
    MRP_LOWER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> insufficient mrpSupportedRoleInst0=0x%x (need=0x%x)", pDBif->mrpSupportedRoleInst0, EDD_MRP_ROLE_CAP_AUTOMANAGER);
    MRP_FATAL;

  }
#endif

  if (!(pDBif->mrpSupportedRoleInst0 & EDD_MRP_ROLE_CAP_CLIENT) && (pDBif->mrpDefaultRoleInst0 != EDD_MRP_ROLE_NONE)
      )
  {
    MRP_LOWER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> mismatch mrpSupportedRoleInst0=0x%x to mrpDefaultRoleInst0=0x%x", pDBif->mrpSupportedRoleInst0, pDBif->mrpDefaultRoleInst0);
    MRP_FATAL;
  }

#ifdef MRP_CFG_PLUGIN_1_MRM_EXT
  if (!(pDBif->mrpSupportedRoleInst0 & EDD_MRP_ROLE_CAP_CLIENT) &&
    (pDBif->mrpSupportedRoleInst0 & EDD_MRP_ROLE_CAP_AUTOMANAGER)
      )
  {
    MRP_LOWER_TRACE_01 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> mrpSupportedRoleInst0=0x%x mismatch, MRC has to be supported moreover", pDBif->mrpSupportedRoleInst0);
    MRP_FATAL;
  }
#endif

  /* check edd details for mrpSupportedMultiRole */
  if (pDBif->mrpSupportedRoleInst0 < pDBif->mrpSupportedMultiRole)
  {
    MRP_LOWER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> mrpSupportedRoleInst0(=0x%x) has to be greater or equal to mrpSupportedMultiRole(=0x%x)", pDBif->mrpSupportedRoleInst0, pDBif->mrpSupportedMultiRole);
    MRP_FATAL;

  }

#ifdef MRP_CFG_PLUGIN_1_MRM_EXT
  if (!(pDBif->mrpSupportedMultiRole & EDD_MRP_ROLE_CAP_CLIENT) &&
    (pDBif->mrpSupportedMultiRole & EDD_MRP_ROLE_CAP_AUTOMANAGER)
      )
  {
    MRP_LOWER_TRACE_01 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> mrpSupportedMultiRole=0x%x mismatch, MRC has to be supported moreover", pDBif->mrpSupportedMultiRole);
    MRP_FATAL;
  }
#endif

  if (!pDBif->mrpMaxInstance)
  {
    MRP_LOWER_TRACE_01 (traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, ">> MRP not supported for this interface! mrpMaxInstance=%d", pDBif->mrpMaxInstance);
  }

  if (!pDBif->mrpMaxInstance && pDBif->mrpSupportedRoleInst0 == EDD_MRP_ROLE_NONE && pDBif->hsyncApplExists == MRP_HSYNC_APPL_EXISTS_ON)
  {
    MRP_LOWER_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, ">> maxInst=%d, hsyncApplExists=%i and MRP (mrpSupportedRoleInst0=%i)",
                        pDBif->mrpMaxInstance, pDBif->hsyncApplExists, pDBif->mrpSupportedRoleInst0);
  }

  if (!pDBif->mrpMaxInstance && pDBif->mrpSupportedRoleInst0 == EDD_MRP_ROLE_NONE && pDBif->hsyncFwdRulesSupported == MRP_HSYNC_FWD_RULES_SUPPORTED_ON)
  {
    MRP_LOWER_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, ">> maxInst=%d, hsyncFwdRulesSupported=%i and MRP (mrpSupportRoleInst0=%i)",
                        pDBif->mrpMaxInstance, pDBif->hsyncFwdRulesSupported, pDBif->mrpSupportedRoleInst0);
  }

  if (pDBif->mrpMaxInstance && pDBif->hsyncFwdRulesSupported == MRP_HSYNC_FWD_RULES_SUPPORTED_OFF && pDBif->hsyncApplExists == MRP_HSYNC_APPL_EXISTS_ON)
  {
    MRP_LOWER_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> maxInst=%d, hsyncApplExists=%i, but hsyncFwdRulesSupported=%i",
                        pDBif->mrpMaxInstance, pDBif->hsyncApplExists, pDBif->hsyncFwdRulesSupported);
    MRP_FATAL;
  }

  if (pDBif->mrpMaxInstance && pDBif->mrpSupportedRoleInst0 == EDD_MRP_ROLE_NONE && pDBif->hsyncFwdRulesSupported == MRP_HSYNC_FWD_RULES_SUPPORTED_ON)
  {
    MRP_LOWER_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> maxInst=%d, hsyncFwdRulesSupported=%i, but MRP (mrpSupportRoleInst0=%i) is not possible",
                        pDBif->mrpMaxInstance, pDBif->hsyncFwdRulesSupported, pDBif->mrpSupportedRoleInst0);
    MRP_FATAL;
  }

  if (pDBif->mrpMaxInstance && pDBif->mrpSupportedRoleInst0 != EDD_MRP_ROLE_NONE && pDBif->mrpDefaultRoleInst0 == EDD_MRP_ROLE_NONE &&
      pDBif->hsyncFwdRulesSupported == MRP_HSYNC_FWD_RULES_SUPPORTED_ON && pDBif->hsyncApplExists == MRP_HSYNC_APPL_EXISTS_ON)
  {
    MRP_LOWER_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> maxInst=%d, hsyncApplExists=%i, but MRP (mrpDefRoleInst0=%i) is not possible",
                        pDBif->mrpMaxInstance, pDBif->hsyncApplExists, pDBif->mrpDefaultRoleInst0);
    MRP_FATAL;
  }

  if (pDBif->mrpSupportedMultiRole != EDD_MRP_ROLE_NONE && pDBif->hsyncFwdRulesSupported == MRP_HSYNC_FWD_RULES_SUPPORTED_ON)
  {
    MRP_LOWER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, ">> hsyncFwdRulesSupported only possible for single MRP rings: mrpSupportedMultiRole=%i but hsyncFwdRulesSupported=%i",
                        pDBif->mrpSupportedMultiRole, pDBif->hsyncFwdRulesSupported);
  }

  if (!pDBif->mrpMaxInstance && pDBif->hsyncApplExists == MRP_HSYNC_APPL_EXISTS_ON)
  {
    MRP_LOWER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, ">> hsyncApplExists=%i, but MRP (mrpMaxInstance=%i) is not possible",
                        pDBif->hsyncApplExists, pDBif->mrpMaxInstance);
  }

  if (!pDBif->mrpMaxInstance && pDBif->hsyncFwdRulesSupported == MRP_HSYNC_FWD_RULES_SUPPORTED_ON)
  {
    MRP_LOWER_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, ">> hsyncFwdRulesSupported=%i, but MRP (mrpMaxInstance=%i) is not possible",
                        pDBif->hsyncFwdRulesSupported, pDBif->mrpMaxInstance);
  }

  if (pDBif->mrpMaxInstance > EDD_CFG_MAX_MRP_INSTANCE_CNT)
  {
    MRP_LOWER_TRACE_01 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> invalid value for mrpMaxInstance=%d", pDBif->mrpMaxInstance);
    MRP_FATAL;
  }

  if (pDBif->mrpInMaxInstance > EDD_CFG_MAX_MRP_IN_INSTANCE_CNT)
  {
    MRP_LOWER_TRACE_01 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> invalid value for mrpMaxInInstance=%d", pDBif->mrpInMaxInstance);
    MRP_FATAL;
  }

  if (!pDBif->numOfPorts)
  {
    MRP_LOWER_TRACE_00 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> invalid numOfPorts (= 0)");
    MRP_FATAL;
  }

  if (!pDBif->mrpMaxInstance)
  {
    if (pDBif->mrpInMaxInstance)
    {
      MRP_LOWER_TRACE_00 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> mrpMaxInstance =0 while mrpInMaxInstance != 0");
      MRP_FATAL;
    }
    if (pDBif->mrpInFwdRulesSupported != MRP_IN_FWD_RULES_SUPPORTED_OFF)
    {
      MRP_LOWER_TRACE_00 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> mrpMaxInstance = 0 while mrpInFwdRulesSupported != OFF");
      MRP_FATAL;
    }
    if (pDBif->mrpInSupportedRoleInst0 != EDD_SUPPORTED_MRP_INTERCONN_ROLE_CAP_NONE)
    {
      MRP_LOWER_TRACE_00 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> mrpMaxInstance = 0 while mrpInSupportedRoleInst0 != NONE");
      MRP_FATAL;
    }
    if (pDBif->mrpInOriginator != MRP_IN_ORIGINATOR_OFF)
    {
      MRP_LOWER_TRACE_00 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> mrpMaxInstance = 0 while mrpInOriginator != OFF");
      MRP_FATAL;
    }
  }
  else /* mrpMaxInstance > 0*/
  {
    if (!pDBif->mrpInMaxInstance)
    {
      if (pDBif->mrpInFwdRulesSupported != MRP_IN_FWD_RULES_SUPPORTED_OFF)
      {
        MRP_LOWER_TRACE_00 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> mrpInMaxInstance = 0 while mrpInOriginator != OFF");
        MRP_FATAL;
      }
      if (pDBif->mrpInOriginator != MRP_IN_ORIGINATOR_OFF)
      {
        MRP_LOWER_TRACE_00 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> mrpInMaxInstance = 0 while mrpInOriginator != OFF");
        MRP_FATAL;
      }
      if (pDBif->mrpInSupportedRoleInst0 != EDD_SUPPORTED_MRP_INTERCONN_ROLE_CAP_NONE)
      {
        MRP_LOWER_TRACE_00 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> mrpInMaxInstance = 0 while mrpInSupportedRoleInst0 != NONE");
        MRP_FATAL;
      }
    }
    else /* mrpInMaxInstance > 0*/
    {
      if (pDBif->mrpInSupportedRoleInst0 != EDD_SUPPORTED_MRP_INTERCONN_ROLE_CAP_NONE && pDBif->mrpInFwdRulesSupported == MRP_IN_FWD_RULES_SUPPORTED_OFF)
      {
        MRP_LOWER_TRACE_00 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> mrpInSupportedRoleInst0 != NONE but mrpInFwdRulesSupported = OFF");
        MRP_FATAL;
      }
      if (pDBif->mrpInFwdRulesSupported == MRP_IN_FWD_RULES_SUPPORTED_OFF && pDBif->mrpInOriginator != MRP_IN_ORIGINATOR_OFF)
      {
        MRP_LOWER_TRACE_00 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> Invalid parameter combination: mrpInFwdRulesSupported = OFF while mrpInOriginator != OFF");
        MRP_FATAL;
      }
      if (pDBif->mrpInFwdRulesSupported != MRP_IN_FWD_RULES_SUPPORTED_OFF && pDBif->mrpSupportedRoleInst0 == EDD_MRP_ROLE_NONE)
      {
        MRP_LOWER_TRACE_00 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> Invalid parameter combination: mrpInFwdRulesSupported != OFF while mrpSupportedRoleInst0 = OFF");
        MRP_FATAL;
      }
      if (pDBif->mrpInOriginator != MRP_IN_ORIGINATOR_OFF && pDBif->mrpSupportedRoleInst0 == EDD_MRP_ROLE_NONE)
      {
        MRP_LOWER_TRACE_00 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> Invalid parameter combination: mrpInOriginator != OFF while mrpSupportedRoleInst0 = OFF");
        MRP_FATAL;
      }
      if (pDBif->mrpSupportedRoleInst0 == EDD_MRP_ROLE_NONE && pDBif->mrpInSupportedRoleInst0 != EDD_SUPPORTED_MRP_INTERCONN_ROLE_CAP_NONE)
      {
        MRP_LOWER_TRACE_00 (traceIdx, LSA_TRACE_LEVEL_FATAL, ">> Invalid parameter combination: mrpInSupportedRoleInst0 != OFF while mrpSupportedRoleInst0 = OFF");
        MRP_FATAL;
      }
    }
  }
}


/*===========================================================================
* FUNCTION : mrp_edd_req_get_params
*----------------------------------------------------------------------------
* PURPOSE  : get edd parameters
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference for edd system channel
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_get_params (const MRP_CHANNEL_TYPE_SYSTEM pChSys)
{
  MRP_EDD_LOWER_RQB_PTR_TYPE pRQB;

  MRP_EDD_ALLOC_RQB(pChSys, pRQB, sizeof(MRP_EDD_RQB_GET_PARAMS_TYPE), EDD_SRV_GET_PARAMS);
  MRP_EDD_REQUEST_LOWER(pRQB, pChSys->sysptr);
}


/*===========================================================================
* FUNCTION : mrp_edd_req_get_params_done
*----------------------------------------------------------------------------
* PURPOSE  : finish get EDD params
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pRQB   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_get_params_done (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB)
{
  LSA_UINT16 i;
  MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys  = MRP_EDD_LOWER_GET_SYS_CHANNEL_FROM_RQB(pRQB);
  MRP_IF_DB_TYPE *pDBif = (MRP_IF_DB_TYPE *)pChSys->pChSysDB->pInterfacePara;
  MRP_EDD_UPPER_GET_PARAMS_PTR_TYPE pParam;

  pParam = MRP_EDD_LOWER_GET_PPARAM_GET_PARAMS(pRQB);
  pDBif->numOfPorts            = pParam->MaxPortCnt;
  pDBif->pnioInterfaceID       = pParam->InterfaceID - 1; /* pnioInterfaceID = eddxInterfaceID - 1 */
  pDBif->hwType                = pParam->HardwareType;
  pDBif->mrpDefaultRoleInst0   = pParam->MRPDefaultRoleInstance0;
  pDBif->mrpSupportedRoleInst0 = pParam->MRPSupportedRole;
  pDBif->mrpMaxInstance        = pParam->MaxMRP_Instances;
  pDBif->mrpSupportedMultiRole = pParam->MRPSupportedMultipleRole;
  pDBif->mraEnableLegacyMode   = pParam->MRAEnableLegacyMode;  /* 0: MRA 2.0 requested, 1: MRA 1.0 requested */

  pDBif->mrpInMaxInstance        = pParam->MaxMRPInterconn_Instances;
  pDBif->mrpInSupportedRoleInst0 = pParam->SupportedMRPInterconnRole;

  pChSys->pChSysDB->traceIdx     = pParam->TraceIdx;

  if (pParam->FeatureMask & EDD_FEATURE_SUPPORT_MCMAC_SRCPORT_MSK)
    pDBif->eddSrcPortIDSupported = MRP_FWD_SRC_PORT_ID_SUPPORTED_ON;
  else
    pDBif->eddSrcPortIDSupported = MRP_FWD_SRC_PORT_ID_SUPPORTED_OFF;

  if (pParam->FeatureMask & EDD_FEATURE_SUPPORT_ADD_FWD_RULES_MSK)
  {
    pDBif->hsyncFwdRulesSupported = MRP_HSYNC_FWD_RULES_SUPPORTED_ON;
  }
  else
  {
    pDBif->hsyncFwdRulesSupported = MRP_HSYNC_FWD_RULES_SUPPORTED_OFF;
  }

  if (pParam->FeatureMask & EDD_FEATURE_SUPPORT_APPL_EXIST_MSK)
  {
    pDBif->hsyncApplExists = MRP_HSYNC_APPL_EXISTS_ON;
  }
  else
  {
    pDBif->hsyncApplExists = MRP_HSYNC_APPL_EXISTS_OFF;
  }

  if (pParam->FeatureMask & EDD_FEATURE_SUPPORT_MRP_INTERCONN_FWD_RULES_MSK)
  {
    pDBif->mrpInFwdRulesSupported = MRP_IN_FWD_RULES_SUPPORTED_ON;
  }
  else
  {
    pDBif->mrpInFwdRulesSupported = MRP_IN_FWD_RULES_SUPPORTED_OFF;
  }

  if (pParam->FeatureMask & EDD_FEATURE_SUPPORT_MRP_INTERCONN_ORIGINATOR_MSK)
  {
    /* In this case RSTP+ coupling is possible. MRP inInstance 0 is reserved for RSTP+ coupling. */
    pDBif->mrpInOriginator    = MRP_IN_ORIGINATOR_ON;
    /* last instance is reserved for RSTP+ */
    pDBif->mrpRstpInstances   = 1;
  }
  else
  {
    pDBif->mrpInOriginator    = MRP_IN_ORIGINATOR_OFF;
    pDBif->mrpRstpInstances   = 0;
  }

  mrp_sys_calc_ha_admin_role (pChSys);

  MRP_MEMCPY(pDBif->macAddr, pParam->MACAddr.MacAdr, sizeof(pDBif->macAddr));

  MRP_ALLOC_LOCAL_MEM(((MRP_LOCAL_MEM_PTR_TYPE*)&pDBif->pPortData), (LSA_UINT16)(sizeof(MRP_PORT_DATA_TYPE) * pDBif->numOfPorts));
  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBif->pPortData, LSA_NULL));  

  for (i = 0; i < pDBif->numOfPorts; i++) 
  {
    pDBif->pPortData[i].moduleState  = MRP_PRM_PORTMODULE_PLUG_COMMIT;  /* default is plugged             */
    pDBif->pPortData[i].rpType       = EDD_MRP_RING_PORT;          /* default is MRP ringport             */
    pDBif->pPortData[i].inPortType   = EDD_SUPPORTS_MRP_INTERCONN_PORT_CONFIG_YES;            /* default is MRP interconnection port */
    pDBif->pPortData[i].isModularPort= MRP_PORT_IS_NOT_MODULAR;    /* default is compact port             */
  }

  if (pDBif->mrpMaxInstance)
  {
    MRP_ALLOC_LOCAL_MEM (((MRP_LOCAL_MEM_PTR_TYPE*)&pDBif->pSequenceID), (LSA_UINT16)(sizeof (LSA_UINT16) * pDBif->mrpMaxInstance));
    MRP_FATAL1 (!LSA_HOST_PTR_ARE_EQUAL (pDBif->pSequenceID, LSA_NULL));

    for (i = 0; i < pDBif->mrpMaxInstance; i++)
    {
      pDBif->pSequenceID[i] = 0;
    }
  }

  mrp_edd_free_rqb (pRQB);

  MRP_LOWER_TRACE_00(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "<< mrp_open_channel(EDD) successful");
}

/*===========================================================================
* FUNCTION : mrp_edd_req_get_port_params
*----------------------------------------------------------------------------
* PURPOSE  : get edd port parameters
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference for edd system channel
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_get_port_params (const MRP_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 portID)
{
  MRP_EDD_LOWER_RQB_PTR_TYPE pRQB;
  MRP_EDD_UPPER_GET_PORT_PARAMS_PTR_TYPE pParam;

  MRP_EDD_ALLOC_RQB(pChSys, pRQB, sizeof(MRP_EDD_RQB_GET_PORT_PARAMS_TYPE), EDD_SRV_GET_PORT_PARAMS);
  pParam = MRP_EDD_LOWER_GET_PPARAM_GET_PORT_PARAMS(pRQB);
  MRP_MEMSET(pParam, 0, sizeof(MRP_EDD_RQB_GET_PORT_PARAMS_TYPE));
  pParam->PortID = portID;

  MRP_EDD_REQUEST_LOWER(pRQB, pChSys->sysptr);
}


/*===========================================================================
* FUNCTION : mrp_edd_req_get_port_params_done
*----------------------------------------------------------------------------
* PURPOSE  : finish get EDD port params
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pRQB   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_get_port_params_done (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB)
{
  MRP_EDD_UPPER_GET_PORT_PARAMS_PTR_TYPE pParam;
  MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys  = MRP_EDD_LOWER_GET_SYS_CHANNEL_FROM_RQB(pRQB);
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  MRP_IF_DB_TYPE *pDBif = (MRP_IF_DB_TYPE *)pChSys->pChSysDB->pInterfacePara;

  pParam = MRP_EDD_LOWER_GET_PPARAM_GET_PORT_PARAMS(pRQB);
  MRP_FATAL1(pParam->PortID);
  MRP_MEMCPY(pDBedd->ppPortMacAddr[pParam->PortID - 1], pParam->MACAddr.MacAdr, 6);

  MRP_FATAL1(pParam->PortID <= pDBif->numOfPorts);
  pDBif->pPortData[pParam->PortID - 1].rpType     = pParam->MRPRingPort;
  pDBif->pPortData[pParam->PortID - 1].inPortType = pParam->SupportsMRPInterconnPortConfig;

  if (pParam->PortStatus == EDD_PORT_PRESENT)
  {
    pDBif->pPortData[pParam->PortID - 1].moduleState   = MRP_PRM_PORTMODULE_PLUG_COMMIT;
    pDBif->pPortData[pParam->PortID - 1].isModularPort = MRP_PORT_IS_NOT_MODULAR;    /* default: port is a compact port. But, if a port is present, it could be a modular port as well */               
  }
  else
  {
    pDBif->pPortData[pParam->PortID - 1].moduleState   = MRP_PRM_PORTMODULE_PULLED;
    pDBif->pPortData[pParam->PortID - 1].isModularPort = MRP_PORT_IS_MODULAR;                   
  }

  /* According to the User Story the following check needs to be done. */
  if (pDBif->pPortData[pParam->PortID - 1].inPortType == EDD_SUPPORTS_MRP_INTERCONN_PORT_CONFIG_YES 
        && (pDBif->mrpInMaxInstance - pDBif->mrpRstpInstances) > mrp_sys_get_inport_cnt (pChSys))
  {
    MRP_FATAL;
  }

  mrp_edd_free_rqb (pRQB);
}


/*===========================================================================
* FUNCTION : mrp_edd_req_oha_mrp_data_write
*----------------------------------------------------------------------------
* PURPOSE  : wrap's an edd prototype for an user channel for providing topo
*            ctrl data
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            rqbNum   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : used for burst mode!
*            function call must be done at the end
*==========================================================================*/
LSA_VOID mrp_edd_req_oha_data_write (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum)
{
  LSA_UNUSED_ARG(rqbNum);

  MRP_PROGRAM_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_CHAT, ">> mrp_edd_req_oha_data_write[inst=%d]", pInstance[MRP_INSTANCE_INDEX]);

  MRP_ASSERT(!LSA_HOST_PTR_ARE_EQUAL(pChSys, LSA_NULL));

  mrp_oha_multi_usr_ringstate_ind_done(pChSys, pInstance[MRP_INSTANCE_INDEX]);

  mrp_edd_req_oha_data_write_done (pChSys, pInstance[MRP_INSTANCE_INDEX], MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_START);
}


/*===========================================================================
* FUNCTION : mrp_edd_req_oha_data_write_done
*----------------------------------------------------------------------------
* PURPOSE  : 
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            instance = affected instance
*            rqbNum   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_oha_data_write_done (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_LL_SREQ_RQB_ENUM_T rqbNum)
{
  LSA_UNUSED_ARG(rqbNum);
  LSA_UNUSED_ARG(instance);   /* hhr: to please the compiler in case traces are disabled. */

  MRP_PROGRAM_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_CHAT, "<< mrp_edd_req_oha_data_write_done[inst=%d]", instance);
  
  mrp_ll_burst_do(pChSys);
}

/*===========================================================================
* FUNCTION : mrp_edd_req_oha_topo_change_ind
*----------------------------------------------------------------------------
* PURPOSE  : indicate topology ctrl information to OHA
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference for edd system channel
*            instance = affected (MRP) instance
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_oha_topo_change_ind (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT8 checkUUID)
{
  MRP_USR_ENUM_TOPO_CTRL topoFlag;

  if (checkUUID == MRP_CHECK_UUID_ON)
    topoFlag = MRP_USR_ENUM_TOPO_CTRL_ADD_CHECK;
  else
    topoFlag = MRP_USR_ENUM_TOPO_CTRL_ADD;

  mrp_oha_multi_usr_write_curr_ringstate_data (pChSys, instance, topoFlag);

  mrp_ll_burst_req (pChSys, instance, g_MrpllReqTbl_mrp_ind_topo_change, LSA_TRUE);
}

/*===========================================================================
* FUNCTION : mrp_edd_req_snd
*----------------------------------------------------------------------------
* PURPOSE  : send request resource to EDD 
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pRQB     = edd req reference
*            port     = send port MRP_PORT_0, MRP_PORT_1, ..., MRP_PORT_AUTO
*            length   = length of packet
*            macStamp = put port mac address in packet
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_snd (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB, LSA_UINT16 port, LSA_UINT16 length, LSA_UINT8 macStamp)
{
  MRP_EDD_UPPER_NRT_SEND_PTR_TYPE pParamSnd;
  MRP_LL_SND_RQB_ENUM_T poolID;
  LSA_UINT16 pTxInstance[2];
  LSA_UINT16 numOfPorts;
  MRP_CHANNEL_TYPE_SYSTEM pChSys  = MRP_EDD_LOWER_GET_SYS_CHANNEL_FROM_RQB(pRQB);
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  MRP_EDD_SEND_RQB_DATA_SET  *pSendRqpInstData = LSA_NULL;

  pParamSnd = MRP_EDD_LOWER_GET_PPARAM_NRT_SEND(pRQB);
  pParamSnd->Length  = length;
  pParamSnd->PortID  = port;
  numOfPorts = mrp_sys_get_num_of_ports (pChSys);

  if (macStamp && ((0 < port) && (numOfPorts >= port))) 
  {
    /* patch port mac address */
    MRP_MEMCPY(pParamSnd->pBuffer + 6, pDBedd->ppPortMacAddr[port - 1], 6);
  }
  /* queue statistic */
  poolID = MRP_LL_GET_TX_QUEUE(pParamSnd->pBuffer); /*lint !e826 CM 11/11/2016 cast on purpose */
  pTxInstance[MRP_INSTANCE_INDEX] = MRP_LL_GET_TX_INSTANCE(pParamSnd->pBuffer); /*lint !e826 CM 11/11/2016 cast on purpose */
  pTxInstance[MRP_IN_INSTANCE_INDEX] = MRP_LL_GET_TX_ININSTANCE(pParamSnd->pBuffer); /*lint !e826 CM 11/11/2016 cast on purpose */
  
  MRP_LOWER_TRACE_03(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_CHAT, ">> mrp_edd_req_snd[inst=%d/inInst=%d], poolID=%d", pTxInstance[MRP_INSTANCE_INDEX], pTxInstance[MRP_IN_INSTANCE_INDEX], poolID);
  
  MRP_LL_RQB_ENUM_ASSERT((LSA_UINT32)poolID);

  mrp_edd_get_nrt_snd_rqb_ptr(&pSendRqpInstData, pDBedd, pTxInstance, poolID);
  
  pSendRqpInstData->sendRqbCnt--;

  if (!pSendRqpInstData->sendRqbCnt) 
  {
    MRP_LOWER_TRACE_03(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_WARN, ">> mrp_edd_req_snd[inst=%d/inInst=%d]: tx RQB queue %u last RQB", pTxInstance[MRP_INSTANCE_INDEX], pTxInstance[MRP_IN_INSTANCE_INDEX], poolID);
  }
  MRP_EDD_REQUEST_LOWER(pRQB, pDBedd->ll_edd_sysptr);
}


/*===========================================================================
* FUNCTION : mrp_edd_req_snd_done
*----------------------------------------------------------------------------
* PURPOSE  : finish EDD send request
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pRQB   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_snd_done (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB)
{
  MRP_EDD_UPPER_NRT_SEND_PTR_TYPE pParamSnd;
  MRP_LL_SND_RQB_ENUM_T poolID;
  LSA_UINT16 pTxInstance[2];
  MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys  = MRP_EDD_LOWER_GET_SYS_CHANNEL_FROM_RQB(pRQB);
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  MRP_EDD_SEND_RQB_DATA_SET  *pSendRqpInstData = LSA_NULL;

  pParamSnd = MRP_EDD_LOWER_GET_PPARAM_NRT_SEND(pRQB);

  MRP_EDD_LOWER_SET_HANDLE(pRQB, pChSys->handle_lower_edd);
  MRP_EDD_LOWER_SET_RESPONSE(pRQB, EDD_STS_OK);
  
  poolID     = MRP_LL_GET_TX_QUEUE(pParamSnd->pBuffer); /*lint !e826 CM 11/11/2016 cast on purpose */
  pTxInstance[MRP_INSTANCE_INDEX] = MRP_LL_GET_TX_INSTANCE(pParamSnd->pBuffer); /*lint !e826 CM 11/11/2016 cast on purpose */
  pTxInstance[MRP_IN_INSTANCE_INDEX] = MRP_LL_GET_TX_ININSTANCE(pParamSnd->pBuffer); /*lint !e826 CM 11/11/2016 cast on purpose */

  MRP_LOWER_TRACE_03(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_CHAT, "<< mrp_edd_req_snd_done[inst=%d/inInst=%d], poolID=%d", pTxInstance[MRP_INSTANCE_INDEX], pTxInstance[MRP_IN_INSTANCE_INDEX], poolID);
  
  MRP_LL_RQB_ENUM_ASSERT((LSA_UINT32)poolID);

  mrp_edd_get_nrt_snd_rqb_ptr(&pSendRqpInstData, pDBedd, pTxInstance, poolID);
  
  pSendRqpInstData->sendRqbCnt++;

  MRP_RQB_PUSH_TO_BOTTOM(pSendRqpInstData->pDBsendRqb, (pSendRqpInstData + 1)->pDBsendRqb, pRQB); 
}


/*===========================================================================
* FUNCTION : mrp_edd_req_rcv
*----------------------------------------------------------------------------
* PURPOSE  : receive request resource to EDD 
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pRQB   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_rcv (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB)
{
  MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys  = MRP_EDD_LOWER_GET_SYS_CHANNEL_FROM_RQB(pRQB);
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  MRP_LOWER_TRACE_00(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_CHAT, ">> mrp_edd_req_rcv entered");
  
  MRP_EDD_REQUEST_LOWER(pRQB, pDBedd->ll_edd_sysptr);
}


/*===========================================================================
* FUNCTION : mrp_edd_req_rcv_done
*----------------------------------------------------------------------------
* PURPOSE  : finish EDD receive request
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pRQB   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_rcv_done (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB)
{
  LSA_UINT16 i, instance;
  LSA_UINT8 *pBuffer;
  MRP_EDD_UPPER_NRT_RECV_PTR_TYPE pParamNrtRcv;
  MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys  = MRP_EDD_LOWER_GET_SYS_CHANNEL_FROM_RQB(pRQB);
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  pParamNrtRcv = MRP_EDD_LOWER_GET_PPARAM_NRT_RECV(pRQB);
  pBuffer      = pParamNrtRcv->pBuffer;
  instance     = mrp_sys_map_rcv_frame_to_instance (pChSys, pParamNrtRcv->PortID, pBuffer);
  
  MRP_LOWER_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_CHAT, "<< mrp_edd_req_rcv_done[inst=%d] entered, port=%u", instance, pParamNrtRcv->PortID);
  
  if (EDD_STS_OK_CANCEL == MRP_EDD_LOWER_GET_RESPONSE(pRQB) || pChSys->state == MRP_CH_STATE_CLOSING)
  {
    LSA_UINT16 ret_val_ptr = LSA_RET_ERR_PARAM;

    MRP_LOWER_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_CHAT, "<< mrp_edd_req_rcv_done[inst=see trace before], port=see trace before, (resp=%u/chState=%u) ->dealloc", MRP_EDD_LOWER_GET_RESPONSE(pRQB), pChSys->state);
      
    MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pBuffer, LSA_NULL));
    MRP_EDD_FREE_LOWER_RX_MEM(&ret_val_ptr, pBuffer, pDBedd->ll_edd_sysptr);
    MRP_FATAL1(ret_val_ptr == LSA_RET_OK);
    mrp_edd_free_rqb (pRQB);
  }
  else 
  {
    for (i = 0; i < MRP_LL_RCV_CBF_TBL_SIZE; i++)//lint !e835
    {
      if (!(0 == pDBedd->pInstData[instance].pRcvCbfFctTbl[i])) 
      {
        MRP_LOWER_TRACE_BYTE_ARRAY(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_edd_req_rcv_done[inst=see trace before] ", pBuffer, MRP_PACKET_VAL_MAC_ADDR_LEN);
        if (pDBedd->pInstData[instance].pRcvCbfFctTbl[i](pDBedd->pChSys, instance, pBuffer, pParamNrtRcv->UserDataOffset, pParamNrtRcv->IOCount, pParamNrtRcv->PortID)) 
        {
          break;
        }
      }
    }
    MRP_EDD_LOWER_SET_HANDLE(pRQB,pChSys->handle_lower_edd);
    /*  MRP_EDD_LOWER_SET_RESPONSE(pRQB,EDD_STS_OK); */
    mrp_edd_req_rcv (pRQB);
  }
}


/*===========================================================================
* FUNCTION : mrp_edd_req_linkChg
*----------------------------------------------------------------------------
* PURPOSE  : link change indication request to EDD 
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pRQB   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_linkChg (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB)
{
  MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys  = MRP_EDD_LOWER_GET_SYS_CHANNEL_FROM_RQB(pRQB);
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  MRP_LOWER_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_CHAT, ">> mrp_edd_req_linkChg, rqb=%x", pRQB);
  MRP_EDD_REQUEST_LOWER(pRQB, pDBedd->ll_edd_sysptr);
}


/*===========================================================================
* FUNCTION : mrp_edd_req_linkChg_done
*----------------------------------------------------------------------------
* PURPOSE  : finish EDD link change indication request
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pRQB   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : delays link state change with lsa timer
*            
*            change      | current | action
*            ------------+---------*----------------
*            down -> up  |    up   | stop link up
*            up -> down  |    up   | start link down
*            down -> up  |  down   | start link up
*            up -> down  |  down   | stop link down
*==========================================================================*/
LSA_VOID mrp_edd_req_linkChg_done (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB)
{
  MRP_EDD_UPPER_LINK_STATUS_IND_PROVIDE_PTR_TYPE pParamLink;
  LSA_UINT16 i;
  LSA_UINT16 numOfPorts;
  MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys  = MRP_EDD_LOWER_GET_SYS_CHANNEL_FROM_RQB(pRQB);
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  MRP_LINK_TBL_TYPE *pLinkChgData;

  MRP_EDD_CHK_LINKCHG_RESPONSE(pRQB);
  pParamLink = MRP_EDD_LOWER_GET_PPARAM_LINK_STATUS_IND(pRQB);

  MRP_LOWER_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "<< mrp_edd_req_linkChg_done port=%u edd_status=%i", pParamLink->PortID, pParamLink->Status);
  /* map EDD port state to MRP port state */
  i = pParamLink->PortID - 1;
  numOfPorts = mrp_sys_get_num_of_ports (pChSys);

  MRP_FATAL1(i < numOfPorts);
  
  pLinkChgData = &pDBedd->pLinkData[i];
  pLinkChgData->state = (pParamLink->Status != EDD_LINK_DOWN);

  while (pLinkChgData->state != pLinkChgData->preState) 
  {
    pLinkChgData->preState = pLinkChgData->state;
    
    if (pLinkChgData->state) /* if LINK_UP is indicated */
    {
      if (pLinkChgData->currentState) /* current linkState is LINK_UP */
      {
        /* no LINK_DOWN timer necessary, since LINK_CHANGE (DOWN) is processed directly */
      }
      else /* !pLinkChgData->currentState --> current linkState is LINK_DOWN */
      {
        pLinkChgData->addLinkInfo.AddCheckEnabled = pParamLink->AddCheckEnabled;
        pLinkChgData->addLinkInfo.ChangeReason    = pParamLink->ChangeReason;

        if (pLinkChgData->timer_upTimeTcnt)
        {
          mrp_sys_timer_start (pLinkChgData->timer_index_up, pLinkChgData->timer_upTimeTcnt);
        }
        else
        {
          /* if NO link_up hysteresis is necessary, than NO timer should be started! */
          mrp_edd_link_state_ind (pChSys, pParamLink->PortID, LSA_TRUE);
        }
      }
    }
    else /* !pLinkChgData->state --> LINK_DOWN is indicated */
    {
      if (pLinkChgData->currentState) /* current linkState is LINK_UP */
      {
        pLinkChgData->addLinkInfo.AddCheckEnabled = pParamLink->AddCheckEnabled;
        pLinkChgData->addLinkInfo.ChangeReason = pParamLink->ChangeReason;

        mrp_edd_link_state_ind (pChSys, pParamLink->PortID, LSA_FALSE);
      }
      else /* !pLinkChgData->currentState --> current linkState is LINK_DOWN */
      {
        mrp_sys_timer_stop(pLinkChgData->timer_index_up); 
      }
    }
  }
  MRP_EDD_LOWER_SET_HANDLE(pRQB, pChSys->handle_lower_edd);
  MRP_EDD_LOWER_SET_RESPONSE(pRQB, EDD_STS_OK);
  mrp_edd_req_linkChg(pRQB);
}

/*===========================================================================
* FUNCTION : mrp_edd_get_new_mc_mode
*----------------------------------------------------------------------------
* PURPOSE  : calc new multicast mode from requested settings of several
*            MRP instances
*----------------------------------------------------------------------------
* RETURNS  : newMcMode = new multicast mode, which should be set as a result
*                        of the requested settings of any MRP instance
*----------------------------------------------------------------------------
* INPUTS   : pChSys    = channel reference for edd system channel
*            instance  = affected instance
*            rqbNum    = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT8 mrp_edd_get_new_mc_mode (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_LL_SREQ_RQB_ENUM_T rqbNum)
{
  MRP_EDD_UPPER_MULTICAST_PTR_TYPE pFDBEntryShadow;
  LSA_UINT16 mrpMaxInstance, i, k;
  LSA_UINT8 newMcMode;
  MRP_EDD_INST_DATA *pDBeddInst;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  LSA_UNUSED_ARG(instance);
  
  newMcMode = EDD_MULTICAST_DISABLE;
  mrpMaxInstance = mrp_sys_get_num_of_instances(pChSys);

  k = (LSA_UINT16)rqbNum - MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_START;
  
  MRP_ASSERT (k < MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_END);

  for (i = 0; i < mrpMaxInstance; i++)
  {
    pDBeddInst = &pDBedd->pInstData[i];
    pFDBEntryShadow = MRP_EDD_LOWER_GET_PPARAM_MULTICAST(pDBeddInst->pDBshadowRqb[k]);
  
    MRP_ASSERT((EDD_MULTICAST_ENABLE == pFDBEntryShadow->Mode) || (EDD_MULTICAST_DISABLE == pFDBEntryShadow->Mode));

    if (pFDBEntryShadow->Mode == EDD_MULTICAST_ENABLE)
    {
      newMcMode = EDD_MULTICAST_ENABLE;
      break;
    }
  }

  return newMcMode;
}


/*===========================================================================
* FUNCTION : mrp_edd_req_set_multicast
*----------------------------------------------------------------------------
* PURPOSE  : set FDB entries request to EDD 
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            rqbNum   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_set_multicast (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum)
{
  MRP_EDD_UPPER_MULTICAST_PTR_TYPE pFDBEntry;
  LSA_UINT16 i;
  LSA_UINT8 newMcMode;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  i = (LSA_UINT16)rqbNum - MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_START;

  pFDBEntry = MRP_EDD_LOWER_GET_PPARAM_MULTICAST(pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_MULTICAST]);

  newMcMode = mrp_edd_get_new_mc_mode (pChSys, pInstance[MRP_INSTANCE_INDEX], rqbNum);
  MRP_LOWER_TRACE_04(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, ">> mrp_edd_req_set_multicast[inst=%d], rqbNum=%d, currentMcMode=%d, newMCmode=%d (0=ENA)", pInstance[MRP_INSTANCE_INDEX], rqbNum, pDBedd->pCurrentMCsetting[i]->Mode, newMcMode);
  MRP_FATAL1(pDBedd->pCurrentMCsetting[i]->RefCnt < 2);
  
  MRP_MEMCPY(pFDBEntry, pDBedd->pCurrentMCsetting[i], sizeof(MRP_EDD_RQB_MULTICAST_TYPE));
  pFDBEntry->Mode = newMcMode;

  if ((EDD_MULTICAST_ENABLE  == newMcMode && pDBedd->pCurrentMCsetting[i]->RefCnt) ||
      (EDD_MULTICAST_DISABLE == newMcMode && !pDBedd->pCurrentMCsetting[i]->RefCnt)) 
  {
    /* do nothing */
    mrp_ll_burst_do(pChSys);
    return;
  }
  
  /**/
  MRP_ASSERT((EDD_MULTICAST_ENABLE == pFDBEntry->Mode) || (EDD_MULTICAST_DISABLE == pFDBEntry->Mode));
  
  MRP_PROGRAM_TRACE_BYTE_ARRAY(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_edd_req_set_multicast: req sent for newMCmode=[see trace before] and MCaddr=", pFDBEntry->MACAddr.MacAdr, EDD_MAC_ADDR_SIZE);


  MRP_EDD_REQUEST_LOWER(pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_MULTICAST], pDBedd->ll_edd_sysptr);
}


/*===========================================================================
* FUNCTION : mrp_edd_req_set_multicast_done
*----------------------------------------------------------------------------
* PURPOSE  : finish EDD set FDB entries request
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            rqbNum   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_set_multicast_done (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum)
{
  MRP_EDD_LOWER_RQB_PTR_TYPE pRQB;
  LSA_UINT16 i;
  MRP_EDD_UPPER_MULTICAST_PTR_TYPE pFDBEntry;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  LSA_UNUSED_ARG(pInstance);   /* hhr: to please the compiler in case traces are disabled. */

  i = (LSA_UINT16)rqbNum - MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_START;
  pRQB = pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_MULTICAST];
  
  MRP_EDD_CHK_RESPONSE(pRQB);
  MRP_EDD_LOWER_SET_HANDLE(pRQB, pChSys->handle_lower_edd);
  MRP_EDD_LOWER_SET_RESPONSE(pRQB, EDD_STS_OK);
  
  pFDBEntry = MRP_EDD_LOWER_GET_PPARAM_MULTICAST(pRQB);
  /* multicast setting is acknowledged by EDD -> save parameters to current settings */
  pDBedd->pCurrentMCsetting[i]->RefCnt = pFDBEntry->RefCnt;
  pDBedd->pCurrentMCsetting[i]->Mode   = pFDBEntry->Mode;
     
  MRP_LOWER_TRACE_04(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "<< mrp_edd_req_set_multicast_done[inst=%d], rqbNum=%d, newMCmode=%d, RefCnt=%d", pInstance[MRP_INSTANCE_INDEX], rqbNum, pFDBEntry->Mode, pFDBEntry->RefCnt);
  
  mrp_ll_burst_do(pChSys);
}


/*===========================================================================
* FUNCTION : mrp_edd_req_set_multicast_fwd_ctrl
*----------------------------------------------------------------------------
* PURPOSE  : set FDB entries request to EDD 
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            rqbNum   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
/*lint -e{818,830}*/
/*function is used within bursts, i.e. array element has to have a distinguished type*/
LSA_VOID mrp_edd_req_set_multicast_fwd_ctrl (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum)
{
  LSA_UINT16 k;
  MRP_EDD_UPPER_MULTICAST_FWD_CTRL_PTR_TYPE pFDBEntry;
  MRP_EDD_UPPER_MULTICAST_FWD_CTRL_PTR_TYPE pFDBEntryShadow;
  MRP_EDD_INST_DATA *pDBeddInst;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  LSA_UINT16 instance;

  instance  = pInstance[MRP_INSTANCE_INDEX];
  pDBeddInst = &pDBedd->pInstData[instance];

  MRP_LOWER_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, ">> mrp_edd_req_set_multicast_fwd_ctrl[inst=%d]", instance);

  pFDBEntryShadow = MRP_EDD_LOWER_GET_PPARAM_MULTICAST_FWD_CTRL(pDBeddInst->pDBshadowRqb[rqbNum]);
  pFDBEntry = MRP_EDD_LOWER_GET_PPARAM_MULTICAST_FWD_CTRL(pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_MULTICAST_FWD_CTRL]);

  pFDBEntry->MACAddrGroup = pFDBEntryShadow->MACAddrGroup;
  pFDBEntry->MACAddrPrio = pFDBEntryShadow->MACAddrPrio;

  MRP_PROGRAM_TRACE_03(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[inst=%d] MACAddrGroup=0x%x, MACAddrLow=0x%x", instance, pFDBEntry->MACAddrGroup, pFDBEntry->MACAddrLow);

  for (k = 0; k < pFDBEntry->PortIDModeCnt; k++) 
  {
    pFDBEntry->pPortIDModeArray[k].Mode      = pFDBEntryShadow->pPortIDModeArray[k].Mode;
    pFDBEntry->pPortIDModeArray[k].DstPortID = pFDBEntryShadow->pPortIDModeArray[k].DstPortID;
    pFDBEntry->pPortIDModeArray[k].SrcPortID = (LSA_UINT16) EDD_PORT_ID_AUTO;
    
    MRP_PROGRAM_TRACE_04(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[inst=%d] DstPortID=%u, SrcPortID=%u, McastFwdMode=%u", instance, pFDBEntry->pPortIDModeArray[k].DstPortID, pFDBEntry->pPortIDModeArray[k].SrcPortID, pFDBEntry->pPortIDModeArray[k].Mode);
  }
  MRP_EDD_REQUEST_LOWER(pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_MULTICAST_FWD_CTRL], pDBedd->ll_edd_sysptr);
}


/*===========================================================================
* FUNCTION : mrp_edd_req_set_multicast_fwd_ctrl_done
*----------------------------------------------------------------------------
* PURPOSE  : finish EDD set FDB entries request
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            rqbNum   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_set_multicast_fwd_ctrl_done (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum)
{
  MRP_EDD_LOWER_RQB_PTR_TYPE pRQB;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  LSA_UNUSED_ARG(rqbNum);
  LSA_UNUSED_ARG(pInstance);   /* hhr: to please the compiler in case traces are disabled. */

  pRQB = pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_MULTICAST_FWD_CTRL];
  MRP_EDD_CHK_RESPONSE(pRQB);
  MRP_EDD_LOWER_SET_HANDLE(pRQB, pChSys->handle_lower_edd);
  MRP_EDD_LOWER_SET_RESPONSE(pRQB, EDD_STS_OK);
  
  MRP_LOWER_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "<< mrp_edd_req_set_multicast_fwd_ctrl_done[inst=%d]", pInstance[MRP_INSTANCE_INDEX]);
  
  mrp_ll_burst_do(pChSys);
}


/*===========================================================================
* FUNCTION : mrp_edd_req_flush_filtering_db
*----------------------------------------------------------------------------
* PURPOSE  : reset port queues request to EDD 
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            rqbNum   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
/*lint -e{818,830}*/
/*function is used within bursts, i.e. array element has to have a distinguished type*/
LSA_VOID mrp_edd_req_flush_filtering_db (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum)
{
  MRP_EDD_UPPER_SWI_FLUSH_FILTERING_DB_PTR_TYPE pFlushFilterDB;
  MRP_EDD_UPPER_SWI_FLUSH_FILTERING_DB_PTR_TYPE pFlushFilterDBShadow;
  LSA_UINT16 i;
  MRP_EDD_INST_DATA *pDBeddInst;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  LSA_UINT16 instance;

  instance  = pInstance[MRP_INSTANCE_INDEX];
  pDBeddInst = &pDBedd->pInstData[instance];

  MRP_LOWER_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, ">> mrp_edd_req_flush_filtering_db[inst=%d]", instance);

  pFlushFilterDBShadow = MRP_EDD_LOWER_GET_PPARAM_FLUSH_FILTERING_DB(pDBeddInst->pDBshadowRqb[rqbNum]);
  pFlushFilterDB       = MRP_EDD_LOWER_GET_PPARAM_FLUSH_FILTERING_DB(pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_FLUSH_FILTERING_DB]);

  *pFlushFilterDB = *pFlushFilterDBShadow;

  for (i = 0; i < pFlushFilterDB->PortCnt; i++)
  {
    MRP_LOWER_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_edd_req_flush_filtering_db[inst=%d]: send flush filtering db req for portID=%d", instance, pFlushFilterDB->PortID[i]);
  }

#ifdef MRP_CFG_PERF_TRACE
  pDBedd->pInstData[instance].perfCapture.endTime = ERTEC_TIME;
  pDBedd->pInstData[instance].perfCapture.timeDiff = pDBedd->pInstData[instance].perfCapture.endTime - pDBedd->pInstData[instance].perfCapture.startTime;
  pDBedd->pInstData[instance].perfCapture.startTime = pDBedd->pInstData[instance].perfCapture.endTime;
 #ifdef MRP_CFG_PERF_TRACE_EDD
  g_perf_measurement_beg = pDBedd->pInstData[instance].perfCapture.startTime;
 #endif
  MRP_LOWER_TRACE_03(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_FATAL, "{@@@ T4xA[inst=%d](flushStart) @@@}[%d] mrp_edd_req_flush_filtering_db (time=%d ns)",
                     instance, pDBedd->pInstData[instance].perfCapture.endTime, (pDBedd->pInstData[instance].perfCapture.timeDiff * 10));
#endif
  
  MRP_EDD_REQUEST_LOWER(pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_FLUSH_FILTERING_DB], pDBedd->ll_edd_sysptr);
}


/*===========================================================================
* FUNCTION : mrp_edd_req_flush_filtering_db_done
*----------------------------------------------------------------------------
* PURPOSE  : finish EDD reset port queues request
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            rqbNum   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_flush_filtering_db_done (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum)
{
  MRP_EDD_LOWER_RQB_PTR_TYPE pRQB;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  LSA_UINT16 instance;

  instance  = pInstance[MRP_INSTANCE_INDEX];  

  LSA_UNUSED_ARG(rqbNum);
  LSA_UNUSED_ARG(instance);   /* hhr: to please lint in case traces are disabled. */

#ifdef MRP_CFG_PERF_TRACE
 #ifdef MRP_CFG_PERF_TRACE_EDD
  pDBedd->pInstData[instance].perfCapture.startTime = g_perf_measurement_beg;
 #endif
  pDBedd->pInstData[instance].perfCapture.endTime = ERTEC_TIME;
  pDBedd->pInstData[instance].perfCapture.timeDiff = pDBedd->pInstData[instance].perfCapture.endTime - pDBedd->pInstData[instance].perfCapture.startTime;
  pDBedd->pInstData[instance].perfCapture.startTime = pDBedd->pInstData[instance].perfCapture.endTime;
  pDBedd->pInstData[instance].perfCapture.perfFlag = MRP_SET_PERF_FLAG_ON;
  MRP_LOWER_TRACE_03(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_FATAL, "{@@@ T4xA[inst=%d] (flushEnd) @@@}[%d] mrp_edd_req_flush_filtering_db_done (time=%d ns)",
                     instance, pDBedd->pInstData[instance].perfCapture.endTime, (pDBedd->pInstData[instance].perfCapture.timeDiff * 10));
#endif
  MRP_LOWER_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "<< mrp_edd_req_flush_filtering_db_done[inst=%d]", instance);
  
  pRQB = pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_FLUSH_FILTERING_DB];
  MRP_EDD_CHK_RESPONSE(pRQB);
  MRP_EDD_LOWER_SET_HANDLE(pRQB, pChSys->handle_lower_edd);
  MRP_EDD_LOWER_SET_RESPONSE(pRQB, EDD_STS_OK);
  
  mrp_ll_burst_do(pChSys);
}


/*===========================================================================
* FUNCTION : mrp_edd_req_port_state
*----------------------------------------------------------------------------
* PURPOSE  : port state request to EDD 
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            rqbNum   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
/*lint -e{818,830}*/
/*function is used within bursts, i.e. array element has to have a distinguished type*/
LSA_VOID mrp_edd_req_port_state (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum)
{
  MRP_EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE pPSParam;
  MRP_EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE pPSParamShadow;
  LSA_UINT16 k, numOfPorts, portNum;
  MRP_EDD_INST_DATA *pDBeddInst;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  LSA_UINT16 instance;

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  instance  = pInstance[MRP_INSTANCE_INDEX];
  pDBeddInst = &pDBedd->pInstData[instance];

  MRP_LOWER_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, ">> mrp_edd_req_port_state[inst=%d]", instance);
  pPSParamShadow = MRP_EDD_LOWER_GET_PPARAM_SET_PORT_STATE(pDBeddInst->pDBshadowRqb[rqbNum]);
  pPSParam = MRP_EDD_LOWER_GET_PPARAM_SET_PORT_STATE(pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_PORT_STATE]);
  MRP_MEMCPY(pPSParam, pPSParamShadow, sizeof(MRP_EDD_RQB_SWI_SET_PORT_STATE_TYPE));

#ifdef MRP_CFG_PERF_TRACE
  if (pDBedd->pInstData[instance].perfCapture.perfFlag == MRP_SET_PERF_FLAG_ON)
  {
    pDBedd->pInstData[instance].perfCapture.endTime = ERTEC_TIME;
    pDBedd->pInstData[instance].perfCapture.timeDiff = pDBedd->pInstData[instance].perfCapture.endTime - pDBedd->pInstData[instance].perfCapture.startTime;
    pDBedd->pInstData[instance].perfCapture.startTime = pDBedd->pInstData[instance].perfCapture.endTime;
    MRP_LOWER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_FATAL, "{@@@ T2A/T3xE/T4xA[inst=%d] @@@}[%d] mrp_edd_req_port_state (time=%d ns)",
                       instance, pDBedd->pInstData[instance].perfCapture.endTime, (pDBedd->pInstData[instance].perfCapture.timeDiff * 10));
  }
#endif

  numOfPorts = mrp_sys_get_num_of_ports (pChSys);
  for (k = 0; k < numOfPorts; k++) 
  {
    portNum = k + 1;

    LSA_UNUSED_ARG(portNum);   /* hhr: to please lint in case traces are disabled. */

    switch (pPSParam->PortIDState[k])
    {
    case EDD_PORT_STATE_DISABLE:
      MRP_LOWER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> mrp_edd_req_port_state[inst=%d] PortID=%u State=%u(DISABLE)", instance, portNum, pPSParam->PortIDState[k]);
      break;
    case EDD_PORT_STATE_BLOCKING:
      MRP_LOWER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> mrp_edd_req_port_state[inst=%d] PortID=%u State=%u(BLOCKING)", instance, portNum, pPSParam->PortIDState[k]);
      break;
    case EDD_PORT_STATE_FORWARDING:
      MRP_LOWER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> mrp_edd_req_port_state[inst=%d] PortID=%u State=%u(FORWARDING)", instance, portNum, pPSParam->PortIDState[k]);
      break;
    case EDD_PORT_STATE_UNCHANGED:
      MRP_LOWER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> mrp_edd_req_port_state[inst=%d] PortID=%u State=%u(UNCHANGED)", instance, portNum, pPSParam->PortIDState[k]);
     break;
    case EDD_PORT_STATE_FORWARDING_WITHOUT_LEARNING:
      MRP_LOWER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, ">> mrp_edd_req_port_state[inst=%d] PortID=%u State=%u(FWD_WITHOUT_LEARNING)", instance, portNum, pPSParam->PortIDState[k]);
      break;
    default:
      MRP_LOWER_TRACE_03(traceIdx, LSA_TRACE_LEVEL_ERROR, ">> mrp_edd_req_port_state[inst=%d] PortID=%u State=%u(UNKNOWN)", instance, portNum, pPSParam->PortIDState[k]);
      break;
    }
  }
  /* reset 
  for (k=0; k < numOfPorts; k++) 
  {
    pPSParamShadow->PortIDState[k] = EDD_PORT_STATE_UNCHANGED;
  }
  */
  MRP_EDD_REQUEST_LOWER(pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_PORT_STATE], pDBedd->ll_edd_sysptr);
}


/*===========================================================================
* FUNCTION : mrp_edd_req_port_state_done
*----------------------------------------------------------------------------
* PURPOSE  : finish EDD port state request
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            rqbNum   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_port_state_done (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum)
{
  MRP_EDD_LOWER_RQB_PTR_TYPE pRQB;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  LSA_UINT16 instance;

  instance  = pInstance[MRP_INSTANCE_INDEX];
  LSA_UNUSED_ARG(rqbNum);

#ifdef MRP_CFG_PERF_TRACE
  if (pDBedd->pInstData[instance].perfCapture.perfFlag == MRP_SET_PERF_FLAG_ON)
  {
    pDBedd->pInstData[instance].perfCapture.perfFlag = MRP_SET_PERF_FLAG_OFF;
    pDBedd->pInstData[instance].perfCapture.endTime = ERTEC_TIME;
    pDBedd->pInstData[instance].perfCapture.timeDiff = pDBedd->pInstData[instance].perfCapture.endTime - pDBedd->pInstData[instance].perfCapture.startTime;
    pDBedd->pInstData[instance].perfCapture.startTime = pDBedd->pInstData[instance].perfCapture.endTime;
    MRP_LOWER_TRACE_03(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_FATAL, "{@@@ T4xE[inst=%d] @@@}[%d] mrp_edd_req_port_state_done (time=%d ns)",
                       instance, pDBedd->pInstData[instance].perfCapture.endTime, (pDBedd->pInstData[instance].perfCapture.timeDiff * 10));
  }
#endif

  MRP_LOWER_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "<< mrp_edd_req_port_state_done[inst=%d]", instance);
  pRQB = pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_PORT_STATE];

  MRP_EDD_CHK_RESPONSE(pRQB);

  mrp_edd_write_rport_states_to_mib (pChSys, instance, rqbNum);

  MRP_EDD_LOWER_SET_HANDLE(pRQB, pChSys->handle_lower_edd);
  MRP_EDD_LOWER_SET_RESPONSE(pRQB, EDD_STS_OK);
  
  mrp_ll_burst_do(pChSys);
}


/*===========================================================================
* FUNCTION : mrp_edd_req_in_link_retrigger
*----------------------------------------------------------------------------
* PURPOSE  : indicate current link state again during MRP interconnection initialization
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            rqbNum   = edd req reference
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_edd_req_in_link_retrigger (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum)
{
  LSA_UINT16 maxInInstances;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  MRP_IN_DB_TYPE *pDBin;
  MRP_IN_DB_TYPE *pDBinBase;
  LSA_UINT16 instance;

  instance  = pInstance[MRP_INSTANCE_INDEX];
  LSA_UNUSED_ARG (rqbNum);

  maxInInstances = mrp_in_sys_get_num_of_in_instances (pChSys);

  if (maxInInstances)
  {
    pDBinBase = (MRP_IN_DB_TYPE *)pChSys->pChSysDB->pMrpIn;

    pDBin = &pDBinBase[pInstance[MRP_IN_INSTANCE_INDEX]];

    /* if the MRP interconnection instance is configured with a MRP interconnection role, then the inId has to match as well */
    if (pDBin->runState == MRP_IN_OPERATION_ON && pDBin->operRole != MRP_IN_OPER_ROLE_OFF)
    {
      MRP_ASSERT (pDBin->IPort > 0);
      MRP_PROGRAM_TRACE_03 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_edd_req_in_link_retrigger[inst=%d/inInst=%d]: IPort=%d", instance, pInstance[MRP_IN_INSTANCE_INDEX], pDBin->IPort);

      pDBedd->pLinkData[pDBin->IPort - 1].addLinkInfo.AddCheckEnabled = 1;
      mrp_edd_link_chg_cbf (pChSys, instance, pDBin->IPort, pDBedd->pLinkData[pDBin->IPort - 1].currentState);
    }
  }

  mrp_ll_burst_do (pChSys);
}

/*===========================================================================
* FUNCTION : mrp_edd_req_link_retrigger
*----------------------------------------------------------------------------
* PURPOSE  : indicate current link state again during initialization
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            rqbNum   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_link_retrigger (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum)
{
  LSA_UINT16 rport1, rport2;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  LSA_UINT16 instance;

  instance  = pInstance[MRP_INSTANCE_INDEX];
  LSA_UNUSED_ARG(rqbNum);
  
  mrp_edd_get_ring_ports(pChSys, instance, &rport1, &rport2);
  MRP_PROGRAM_TRACE_03(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_edd_req_link_retrigger[inst=%d]: rport1=%d, rport2=%d", instance, rport1, rport2);
  
  mrp_edd_link_chg_cbf(pChSys, instance, rport1, pDBedd->pLinkData[rport1 - 1].currentState);
  mrp_edd_link_chg_cbf(pChSys, instance, rport2, pDBedd->pLinkData[rport2 - 1].currentState);
  
  mrp_ll_burst_do(pChSys);
}


/*===========================================================================
* FUNCTION : mrp_edd_req_finish_upper
*----------------------------------------------------------------------------
* PURPOSE  : call cbf function for upper rqb and give rqb back
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            rqbNum   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_finish_upper (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum)
{
  LSA_OPCODE_TYPE opc;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  LSA_UNUSED_ARG(rqbNum);
  MRP_ASSERT (pInstance[MRP_INSTANCE_INDEX] == 0);

  opc = MRP_RQB_GET_OPCODE(pDBedd->pChUsr_finishUpper->pRQB);

  MRP_PROGRAM_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_edd_req_finish_upper[inst=%d] entered", pInstance[MRP_INSTANCE_INDEX]);
  
  mrp_usr_call_upper_cbf_proc_next_req(pDBedd->pChUsr_finishUpper, pDBedd->pChUsr_finishUpper->pRQB);

  if (opc == MRP_OPC_PRM_COMMIT) 
  {
    mrp_rec_prm_ind_commit_cnf_done(pDBedd->pChUsr_finishUpper);
  }
  mrp_ll_burst_do(pChSys);
}


/*===========================================================================
* FUNCTION : mrp_edd_req_finish_close
*----------------------------------------------------------------------------
* PURPOSE  : call cbf function for finish close channel
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            rqbNum   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_finish_close (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum)
{
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  LSA_UNUSED_ARG(rqbNum);
  LSA_UNUSED_ARG(pInstance);   /* hhr: to please the compiler in case traces are disabled. */

  MRP_PROGRAM_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_edd_req_finish_close[inst=%d] entered", pInstance[MRP_INSTANCE_INDEX]);
  
  mrp_usr_finish_close_channel(pDBedd->pChUsr_finishClose);
  mrp_ll_burst_do(pChSys);
}

/*===========================================================================
* FUNCTION : mrp_edd_req_prm_commit
*----------------------------------------------------------------------------
* PURPOSE  : execute prm_commit
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            rqbNum   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_prm_commit (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum)
{
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  LSA_UNUSED_ARG(rqbNum);
  MRP_ASSERT (pInstance[MRP_INSTANCE_INDEX] == 0);

  MRP_PROGRAM_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_edd_req_prm_commit[inst=%d] entered", pInstance[MRP_INSTANCE_INDEX]);

  mrp_rec_prm_commit (pDBedd->pChUsr_prmCommit, pDBedd->pRQB_prmCommit);

  mrp_ll_burst_do(pChSys);      /* for mrpMaxInstance = 0: function must do nothing! */
}

#ifdef MRP_CFG_PLUGIN_0_MRM
/*===========================================================================
* FUNCTION : mrp_edd_req_tplg_chg_req
*----------------------------------------------------------------------------
* PURPOSE  : queue tplg chg snd requests
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            rqbNum   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : due to synchronisation with lower layer send request must
*            wait for acknowledged edd requests.
*==========================================================================*/
LSA_VOID mrp_edd_req_tplg_chg_req (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum)
{
  LSA_UINT16 instance;
  LSA_UNUSED_ARG(rqbNum);

  instance  = pInstance[MRP_INSTANCE_INDEX];
  MRP_PROGRAM_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_edd_req_tplg_chg_req[inst=%d] entered", instance);
  
  mrp_mrm_topology_change (pChSys, instance);
  
  mrp_ll_burst_do (pChSys);
}
#endif

/*===========================================================================
* FUNCTION : mrp_edd_req_tplg_chg_finished
*----------------------------------------------------------------------------
* PURPOSE  : tplg change is over
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            rqbNum   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_req_tplg_chg_finished (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum)
{
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE*)pChSys->pChSysDB->pEdd;
  LSA_UINT16 instance;

  instance  = pInstance[MRP_INSTANCE_INDEX];
  LSA_UNUSED_ARG(rqbNum);

  MRP_PROGRAM_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_edd_req_tplg_chg_finished[inst=%d] entered", instance);
  
  if (!(0 == pDBedd->pInstData[instance].TplgChgFinishFct)) 
  {
    pDBedd->pInstData[instance].TplgChgFinishFct(pChSys, instance);
  }
  pDBedd->pInstData[instance].TplgChgRunning = LSA_FALSE;
  
  mrp_ll_burst_do (pChSys);
}

/*-- end of requests --------------------------------------------------------*/



/*===========================================================================
* FUNCTION : mrp_edd_alloc_rqb
*----------------------------------------------------------------------------
* PURPOSE  : alloc a lower request-block for edd
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys    = channel reference for edd system channel, it is needed
*                         to get the RQB from the right "pot/queue"!
*            *ppRQB = reference to RQB pointer
*            size   = size of EDD parameter buffer
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : this function allocates memory for RQB and parameter block
*==========================================================================*/
LSA_VOID mrp_edd_alloc_rqb (const MRP_CHANNEL_TYPE_SYSTEM pChSys, MRP_EDD_LOWER_RQB_PTR_TYPE *ppRQB, const LSA_UINT16 size)
{
  LSA_UINT16  rc;
  MRP_EDD_LOWER_USED_DATA_PTR_TYPE pParam;  /* one pointer fits it all */

  MRP_PROGRAM_TRACE_00(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_edd_alloc_rqb entered");
  
  MRP_EDD_ALLOC_LOWER_RQB(ppRQB, sizeof(MRP_EDD_LOWER_RQB_TYPE), pChSys->sysptr);

  if (LSA_HOST_PTR_ARE_EQUAL(*ppRQB, LSA_NULL)) 
  {
    MRP_LOWER_TRACE_00(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "--> !lower_rqb");
    return;
  }
  MRP_EDD_LOWER_SET_RESPONSE(*ppRQB, EDD_STS_OK);
  MRP_EDD_LOWER_SET_OPCODE(*ppRQB, EDD_OPC_REQUEST);
  MRP_EDD_LOWER_SET_HANDLE(*ppRQB, pChSys->handle_lower_edd);
  MRP_EDD_LOWER_SET_USERID_PTR(*ppRQB, pChSys);

  rc = LSA_RET_OK;

  MRP_EDD_ALLOC_LOWER_MEM(((MRP_EDD_LOWER_MEM_PTR_TYPE*)&pParam), size, pChSys->sysptr);
  if (LSA_HOST_PTR_ARE_EQUAL(pParam, LSA_NULL)) 
  {
    MRP_LOWER_TRACE_00(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "--> !lower_pParam");
    MRP_EDD_FREE_LOWER_RQB(&rc, *ppRQB, pChSys->sysptr);
    MRP_FATAL1(rc == LSA_RET_OK);
    *ppRQB = LSA_NULL;
    return;
  }
  MRP_MEMSET(pParam, 0, size);
  MRP_EDD_LOWER_SET_PPARAM(*ppRQB, pParam);
}


/*===========================================================================
* FUNCTION : mrp_edd_free_rqb
*----------------------------------------------------------------------------
* PURPOSE  : free a lower request-block, with its param 
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pRQB   = edd req reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : frees memory of RQB and parameter block
*==========================================================================*/
LSA_VOID mrp_edd_free_rqb (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB)
{
  LSA_UINT16 rc;
  MRP_CHANNEL_TYPE_SYSTEM pChSys  = MRP_EDD_LOWER_GET_SYS_CHANNEL_FROM_RQB(pRQB);

  MRP_PROGRAM_TRACE_00(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_edd_free_rqb entered");
  
  rc = LSA_RET_OK;
  if (!LSA_HOST_PTR_ARE_EQUAL(MRP_EDD_LOWER_GET_PPARAM_USED_DATA_PTR(pRQB), LSA_NULL)) 
  {
    MRP_EDD_FREE_LOWER_MEM (&rc, MRP_EDD_LOWER_GET_PPARAM_USED_DATA_PTR(pRQB), pChSys->sysptr);
    MRP_FATAL1(rc == LSA_RET_OK);
    MRP_EDD_LOWER_SET_PPARAM(pRQB, LSA_NULL);
  }
  MRP_EDD_FREE_LOWER_RQB (&rc, pRQB, pChSys->sysptr);
  MRP_FATAL1(rc == LSA_RET_OK);
}


/*===========================================================================
* FUNCTION : mrp_edd_link_chg_cbf
*----------------------------------------------------------------------------
* PURPOSE  : handle link state changes after linkUp timer expiry or called by
*            mrp_edd_link_state_ind
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference
*            instance = affected MRP/HSR instance
*            port   = edd port id (1..MAX_PORTS)
*            linkUp = link state: LSA_TRUE:link up, LSA_FALSE=link down
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_edd_link_chg_cbf(const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT16 port, LSA_UINT8 linkUp)
{
  LSA_UINT8 psAck;
  LSA_UINT16 i, numOfPorts;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  numOfPorts = mrp_sys_get_num_of_ports (pChSys);
  MRP_ASSERT((port > 0) && (port <= numOfPorts));
  psAck = LSA_FALSE;
  
  for (i = 0; i < MRP_LL_LINK_CHG_CBF_TBL_SIZE; i++) 
  {
    if (!(0 == pDBedd->pInstData[instance].pLinkChgCbfFctTbl[i])) 
    {
      /* call linkChg cbf of different MRP/HSR modules, e.g. mrp_mrm_link_chg_cbf */
      psAck = pDBedd->pInstData[instance].pLinkChgCbfFctTbl[i](pDBedd->pChSys, instance, port, pDBedd->pLinkData[port - 1].addLinkInfo, linkUp);
      if (psAck) 
        break;
    }
  }
}

/*===========================================================================
* FUNCTION : mrp_edd_link_state_ind
*----------------------------------------------------------------------------
* PURPOSE  : handle link state changes,if no timer was started (time = 0)
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference
*            port   = edd port id (1..MAX_PORTS)
*            linkUp = 1: linkUp, 0: linkDown 
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_link_state_ind (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 port, LSA_UINT8 isLinkUp)
{
  LSA_UINT16 numOfPorts, instance;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
#ifdef MRP_CFG_PERF_TRACE
  LSA_UINT32 startTime, endTime, timeDiff;

  timeDiff = 0;

#ifdef MRP_CFG_PERF_TRACE_EDD
  startTime = g_perf_measurement_beg;
#else
  startTime = ERTEC_TIME;
#endif
  endTime   = ERTEC_TIME;
  timeDiff  = endTime - startTime;
#endif /* MRP_CFG_PERF_TRACE */

  numOfPorts = mrp_sys_get_num_of_ports (pChSys);
  instance   = mrp_sys_map_port_to_instance (pChSys, port);

  MRP_ASSERT (port > 0 && port <= numOfPorts);
  pDBedd->pLinkData[port - 1].currentState = isLinkUp;

  if (isLinkUp) 
  {
    MRP_PROGRAM_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_edd_link_state_ind[inst=%d]: linkChg port=%u LINK_UP", instance, port);
  }
  else 
  {
#ifdef MRP_CFG_PERF_TRACE
    pDBedd->pInstData[instance].perfCapture.startTime = startTime;
    pDBedd->pInstData[instance].perfCapture.endTime = endTime;
    pDBedd->pInstData[instance].perfCapture.timeDiff = timeDiff;
    pDBedd->pInstData[instance].perfCapture.startTime = pDBedd->pInstData[instance].perfCapture.endTime;
    MRP_LOWER_TRACE_03(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_FATAL, "{@@@ T1CA[inst=%d] @@@}[%d] mrp_edd_link_state_ind (time=%d ns)",
      instance, pDBedd->pInstData[instance].perfCapture.endTime, (pDBedd->pInstData[instance].perfCapture.timeDiff * 10));

    pDBedd->pInstData[instance].perfCapture.endTime = ERTEC_TIME;
    pDBedd->pInstData[instance].perfCapture.timeDiff = pDBedd->pInstData[instance].perfCapture.endTime - pDBedd->pInstData[instance].perfCapture.startTime;
    pDBedd->pInstData[instance].perfCapture.startTime = pDBedd->pInstData[instance].perfCapture.endTime;
    MRP_LOWER_TRACE_03(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_FATAL, "{@@@ T1CA[inst=%d] (timerStart) @@@}[%d] mrp_edd_link_state_ind (time=%d ns)",
      instance, pDBedd->pInstData[instance].perfCapture.endTime, (pDBedd->pInstData[instance].perfCapture.timeDiff * 10));

    pDBedd->pInstData[instance].perfCapture.endTime = ERTEC_TIME;
    pDBedd->pInstData[instance].perfCapture.timeDiff = pDBedd->pInstData[instance].perfCapture.endTime - pDBedd->pInstData[instance].perfCapture.startTime;
    pDBedd->pInstData[instance].perfCapture.startTime = pDBedd->pInstData[instance].perfCapture.endTime;

    MRP_LOWER_TRACE_03(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_FATAL, "{@@@ T1CA[inst=%d] (timerEnd) @@@}[%d] mrp_edd_link_state_ind LINK_DOWN (time=%d ns)",
      instance, pDBedd->pInstData[instance].perfCapture.endTime, (pDBedd->pInstData[instance].perfCapture.timeDiff * 10));
#endif
    MRP_PROGRAM_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_edd_link_state_ind[inst=%d]: linkChg port=%u LINK_DOWN", instance, port);
  }
  mrp_mib_write_link_state (pChSys, instance, port, isLinkUp);

  mrp_edd_link_chg_cbf(pChSys, instance, port, isLinkUp);  
}

/*===========================================================================
* FUNCTION : mrp_edd_timer_cbf_link
*----------------------------------------------------------------------------
* PURPOSE  : handle link state changes after expiry of linkup timer
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference
*            pIfInstance = pointer to interface instance (0xffff)  
*            uid    = user identification (here: timer_id is stored) 
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_timer_cbf_link (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pIfInstance, LSA_USER_ID_TYPE uid)
{
  LSA_UINT16 port, i, numOfPorts, instance;
  LSA_UINT8 linkUp;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  MRP_ASSERT(*pIfInstance == MRP_TIMER_INTERFACE);
  LSA_UNUSED_ARG (*pIfInstance);

  linkUp = LSA_FALSE;
  numOfPorts = mrp_sys_get_num_of_ports (pChSys);
  
  for(i = 0; i < numOfPorts; i++) 
  {
    if (pDBedd->pLinkData[i].timer_index_up == uid.uvar16) 
    {
      linkUp = LSA_TRUE;
      break;
    }
  }
  MRP_ASSERT(i < numOfPorts);

  pDBedd->pLinkData[i].currentState = linkUp;
  port = i + 1;

  instance   = mrp_sys_map_port_to_instance (pChSys, port);

  if (linkUp) 
  {
    MRP_PROGRAM_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_edd_timer_cbf_link[inst=%d]: linkChg port=%u UP", instance, port);
  }
  else 
  {
    /* should never happen, because there is no LINK_DOWN timer allocated at all!!! */
    MRP_PROGRAM_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_FATAL, "mrp_edd_timer_cbf_link[inst=%d]: linkChg port=%u DOWN", instance, port);
  }
  mrp_mib_write_link_state (pChSys, instance, port, linkUp);

  mrp_edd_link_chg_cbf(pChSys, instance, port, linkUp);  
}


/*===========================================================================
* FUNCTION : mrp_edd_timer_cbf_tplg_chg
*----------------------------------------------------------------------------
* PURPOSE  : cbf for topologyChange countdown timer called, because timer for 
*            clearing FDB expired:
*            queue request for FDB cleanup now
*            timer is started in function mrp_ll_clear_fdb 
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys    = channel reference
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            uid       = user identification (here: timer_id is stored) 
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_edd_timer_cbf_tplg_chg (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, LSA_USER_ID_TYPE uid)
{
  LSA_UNUSED_ARG (uid);
#ifdef MRP_CFG_PERF_TRACE
  {
    MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

    pDBedd->pInstData[pInstance[MRP_INSTANCE_INDEX]].perfCapture.endTime = ERTEC_TIME;
    pDBedd->pInstData[pInstance[MRP_INSTANCE_INDEX]].perfCapture.timeDiff = pDBedd->pInstData[pInstance[MRP_INSTANCE_INDEX]].perfCapture.endTime - pDBedd->pInstData[pInstance[MRP_INSTANCE_INDEX]].perfCapture.startTime;
    pDBedd->pInstData[pInstance[MRP_INSTANCE_INDEX]].perfCapture.startTime = pDBedd->pInstData[pInstance[MRP_INSTANCE_INDEX]].perfCapture.endTime;
    pDBedd->pInstData[pInstance[MRP_INSTANCE_INDEX]].perfCapture.perfFlag = MRP_SET_PERF_FLAG_ON;
    MRP_LOWER_TRACE_03(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_FATAL, "{@@@ T3CA[inst=%d] (timerEnd) @@@}[%d] mrp_edd_timer_cbf_tplg_chg (time=%d ns)",
                       pInstance[MRP_INSTANCE_INDEX], pDBedd->pInstData[pInstance[MRP_INSTANCE_INDEX]].perfCapture.endTime, (pDBedd->pInstData[pInstance[MRP_INSTANCE_INDEX]].perfCapture.timeDiff * 10));
  }
#endif
  mrp_ll_in_burst_req((MRP_CHANNEL_TYPE_SYSTEM) pChSys, pInstance, g_MrpllReqTbl_mrp_flush_fdb, LSA_FALSE);
}


/*===========================================================================
* FUNCTION : mrp_edd_set_rcv_callback
*----------------------------------------------------------------------------
* PURPOSE  : set callback for receive 
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference for edd system channel
*            instance = affected MRP/HSR instance
*            cbf    = callback function pointer
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_set_rcv_callback (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_LL_RCV_FCT_PTR_TYPE cbf)
{
  LSA_INT32 i;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  for (i = 0; i < MRP_LL_RCV_CBF_TBL_SIZE; i++)//lint !e835 
  {
    if (0 == pDBedd->pInstData[instance].pRcvCbfFctTbl[i]) 
    {
      pDBedd->pInstData[instance].pRcvCbfFctTbl[i] = cbf;
      break;
    }
  }
  MRP_ASSERT(i < MRP_LL_RCV_CBF_TBL_SIZE);//lint !e835
}


/*===========================================================================
* FUNCTION : mrp_edd_remove_rcv_callback
*----------------------------------------------------------------------------
* PURPOSE  : remove callback for receive
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference for edd system channel
*            instance = affected MRP/HSR instance
*            cbf    = callback function pointer
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID  mrp_edd_remove_rcv_callback (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_LL_RCV_FCT_PTR_TYPE cbf)
{
  LSA_INT32 i;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  for (i = 0; i < MRP_LL_RCV_CBF_TBL_SIZE; i++)//lint !e835 
  {
    if (pDBedd->pInstData[instance].pRcvCbfFctTbl[i] == cbf) 
    {
      pDBedd->pInstData[instance].pRcvCbfFctTbl[i] = 0;
      break;
    }
  }
  MRP_ASSERT(i < MRP_LL_RCV_CBF_TBL_SIZE);//lint !e835 
}


/*===========================================================================
* FUNCTION : mrp_edd_set_link_chg_callback
*----------------------------------------------------------------------------
* PURPOSE  : set callback for link change
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference for edd system channel
*            instance = affected HSR/MRP instance
*            cbf    = callback function pointer
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_set_link_chg_callback (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_LL_LINK_CHG_FCT_PTR_TYPE cbf)
{
  LSA_INT32 i;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  for (i = 0; i < MRP_LL_LINK_CHG_CBF_TBL_SIZE; i++) 
  {
    if (0 == pDBedd->pInstData[instance].pLinkChgCbfFctTbl[i])
    {
      pDBedd->pInstData[instance].pLinkChgCbfFctTbl[i] = cbf;
      break;
    }
  }
  MRP_ASSERT(i < MRP_LL_LINK_CHG_CBF_TBL_SIZE);
}


/*===========================================================================
* FUNCTION : mrp_edd_remove_link_chg_callback
*----------------------------------------------------------------------------
* PURPOSE  : remove callback for link change
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference for edd system channel
*            instance = affected HSR/MRP instance
*            cbf    = callback function pointer
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_remove_link_chg_callback (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_LL_LINK_CHG_FCT_PTR_TYPE cbf)
{
  LSA_INT32 i;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  for (i = 0; i < MRP_LL_LINK_CHG_CBF_TBL_SIZE; i++) 
  {
    if (pDBedd->pInstData[instance].pLinkChgCbfFctTbl[i] == cbf) 
    {
      pDBedd->pInstData[instance].pLinkChgCbfFctTbl[i] = 0;
      break;
    }
  }
  MRP_ASSERT(i < MRP_LL_LINK_CHG_CBF_TBL_SIZE);
}


/*===========================================================================
* FUNCTION : mrp_edd_set_tplg_chg_callback
*----------------------------------------------------------------------------
* PURPOSE  : set callback for tplg change
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference for edd system channel
*            instance = affected HSR instance
*            cbf    = callback function pointer
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_set_tplg_chg_callback (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_LL_TPLG_CHG_FINISH_FCT_PTR_TYPE cbf)
{
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;


  MRP_ASSERT(0 == pDBedd->pInstData[instance].TplgChgFinishFct);
  pDBedd->pInstData[instance].TplgChgFinishFct = cbf;
}


/*===========================================================================
* FUNCTION : mrp_edd_remove_tplg_chg_callback
*----------------------------------------------------------------------------
* PURPOSE  : remove callback for tplg change
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference for edd system channel
*            instance = affected HSR instance
*            cbf    = callback function pointer
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_remove_tplg_chg_callback (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_LL_TPLG_CHG_FINISH_FCT_PTR_TYPE cbf)
{
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  MRP_ASSERT(pDBedd->pInstData[instance].TplgChgFinishFct == cbf);
  LSA_UNUSED_ARG (cbf);

  pDBedd->pInstData[instance].TplgChgFinishFct = 0;
}


/*===========================================================================
* FUNCTION : mrp_edd_get_ring_ports
*----------------------------------------------------------------------------
* PURPOSE  : get ring ports
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys      = channel reference for edd system channel
*            instance    = affected MRP instance
* OUTPUTS  : port0,port1 = edd port id (1..MAX_PORTS)
*----------------------------------------------------------------------------
* COMMENTS :  
*==========================================================================*/
LSA_VOID mrp_edd_get_ring_ports (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT16 *port0, LSA_UINT16 *port1)
{
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  *port0 = pDBedd->pInstData[instance].RPort_1;
  *port1 = pDBedd->pInstData[instance].RPort_2;
}


/*===========================================================================
* FUNCTION : mrp_edd_set_port_state
*----------------------------------------------------------------------------
* PURPOSE  : set port state immediately to hardware
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference for edd system channel
*            instance = affected MRP instance
*            port   = edd port id (1..MAX_PORTS)
*            state  = MRP_PORT_STATE_FORWARD,MRP_PORT_STATE_BLOCKED
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS :  
*==========================================================================*/
LSA_VOID mrp_edd_set_port_state (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, LSA_UINT16 port, MRP_PORT_STATE_TYPE state)
{  
  MRP_EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE pPSParam;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  MRP_EDD_INST_DATA *pDBeddInst;

  pDBeddInst = &pDBedd->pInstData[pInstance[MRP_INSTANCE_INDEX]];

  MRP_PROGRAM_TRACE_03(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_edd_set_port_state[inst=%d]: port=%d, state=%d", pInstance[MRP_INSTANCE_INDEX], port, state);

  if (port != pDBeddInst->RPort_1 && port != pDBeddInst->RPort_2)
  {
    MRP_PROGRAM_TRACE_04(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "mrp_edd_set_port_state[inst=%d/inInst=%d]: port=%d, state=%d (MRP interconnection port)", pInstance[MRP_INSTANCE_INDEX], pInstance[MRP_IN_INSTANCE_INDEX], port, state);
  }
  pPSParam = MRP_EDD_LOWER_GET_PPARAM_SET_PORT_STATE(pDBeddInst->pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_SET_PS_SHADOW]);

  mrp_ll_set_port_state_param (pChSys, pPSParam, port, state);

  mrp_ll_in_burst_req(pChSys, pInstance, g_MrpllReqTbl_mrp_ps_set, LSA_TRUE);
}


/*===========================================================================
* FUNCTION : mrp_edd_get_snd_rqb_max
*----------------------------------------------------------------------------
* PURPOSE  : get number of send RQB 
*----------------------------------------------------------------------------
* RETURNS  : sendRqbMaxCnt = no of send rqbs which have been reserved for 
*                            this interface
*----------------------------------------------------------------------------
* INPUTS   : pChSys    = channel reference for edd system channel
*            instance  = (MRP) instance requesting the rqb
*            inInstance  = (MRP) interconnection instance requesting the rqb
*            poolID = id of send RQB pool (MRP_LL_SND_RQB_ENUM_TBL_START..
*                                          ..MRP_LL_SND_RQB_ENUM_TBL_END)
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT16 mrp_edd_get_snd_rqb_max (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SND_RQB_ENUM_T poolID)
{
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  MRP_EDD_SEND_RQB_DATA_SET  *pSendRqpInstData = LSA_NULL;

  MRP_LL_RQB_ENUM_ASSERT((LSA_UINT32)poolID);

  if (pInstance[MRP_INSTANCE_INDEX] < mrp_sys_get_num_of_instances(pChSys))
  {
    mrp_edd_get_nrt_snd_rqb_ptr(&pSendRqpInstData, pDBedd, pInstance, poolID);

    return pSendRqpInstData->sendRqbMaxCnt;
  }
  else
    return 0;
}


/*===========================================================================
* FUNCTION : mrp_edd_get_snd_rqb
*----------------------------------------------------------------------------
* PURPOSE  : get send RQB
*----------------------------------------------------------------------------
* RETURNS  : LSA_TRUE = threshold reached, LSA_FALSE = threshold not reached
*----------------------------------------------------------------------------
* INPUTS   : pChSys    = channel reference for edd system channel, it is needed
*                         to get the RQB from the right "pot/queue"!
*            instance  = (MRP) instance requesting the rqb
*            inInstance  = (MRP) interconnection instance requesting the rqb
*            poolID    = id of send RQB pool (MRP_LL_SND_RQB_ENUM_TBL_START..
*                                        ..MRP_LL_SND_RQB_ENUM_TBL_END)
*            threshold = no of lost rqb 
* OUTPUTS  : *ppRQB    = pointer to RQB
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT8 mrp_edd_get_snd_rqb (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SND_RQB_ENUM_T poolID, MRP_EDD_LOWER_RQB_PTR_TYPE *ppRQB, LSA_UINT32 threshold)
{
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  MRP_EDD_SEND_RQB_DATA_SET  *pSendRqpInstData = LSA_NULL;

  MRP_LL_RQB_ENUM_ASSERT((LSA_UINT32)poolID);
  
  mrp_edd_get_nrt_snd_rqb_ptr(&pSendRqpInstData, pDBedd, pInstance, poolID);
  
  MRP_RQB_POP_FROM_TOP(pSendRqpInstData->pDBsendRqb, (pSendRqpInstData + 1)->pDBsendRqb, *ppRQB);

  if (LSA_HOST_PTR_ARE_EQUAL(*ppRQB, LSA_NULL)) 
  {
    pSendRqpInstData->sendRqbMissCnt++;
    MRP_LOWER_TRACE_04(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_WARN, ">> tx RQB[inst=%u/inInst=%u], queue=%u empty for %u times", pInstance[MRP_INSTANCE_INDEX], pInstance[MRP_IN_INSTANCE_INDEX], poolID, pSendRqpInstData->sendRqbMissCnt);
  }
  else 
  {
    pSendRqpInstData->sendRqbMissCnt = 0;
    MRP_FATAL1(pChSys == MRP_EDD_LOWER_GET_SYS_CHANNEL_FROM_RQB(*ppRQB)); /* the input parameter "pChSys" must be the same as stored in the RQB!! */
  }
  /*  "send requests" = "max buffer" - "no of rqbs in buffer" + "lost buffers" */
  if (((LSA_UINT32)((pSendRqpInstData->sendRqbMaxCnt - pSendRqpInstData->sendRqbCnt) + pSendRqpInstData->sendRqbMissCnt)) > threshold) 
  {
    MRP_LOWER_TRACE_06(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_UNEXP, ">> tx RQB[inst=%u/inInst=%u], queue=%u missing tx-done for %u times (threshold=%u, rqbQueueCnt=%u)", pInstance[MRP_INSTANCE_INDEX], pInstance[MRP_IN_INSTANCE_INDEX], poolID, pSendRqpInstData->sendRqbMissCnt, threshold, pSendRqpInstData->sendRqbCnt);
    return LSA_TRUE;
  }
  return LSA_FALSE;
}


/*===========================================================================
* FUNCTION : mrp_edd_set_snd_rqb
*----------------------------------------------------------------------------
* PURPOSE  : push send RQB in queue
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : instance  = (MRP) instance requesting the rqb
*            inInstance  = (MRP) interconnection instance requesting the rqb
*            poolID = id of send RQB pool (MRP_LL_SND_RQB_ENUM_TBL_START..
*                                        ..MRP_LL_SND_RQB_ENUM_TBL_END)
*            *ppRQB = pointer to RQB
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : used only for init
*==========================================================================*/
LSA_VOID mrp_edd_set_snd_rqb (const LSA_UINT16 *pInstance, MRP_LL_SND_RQB_ENUM_T poolID, MRP_EDD_LOWER_RQB_PTR_TYPE pRQB)
{
  MRP_CHANNEL_TYPE_SYSTEM pChSys  = MRP_EDD_LOWER_GET_SYS_CHANNEL_FROM_RQB(pRQB);
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  MRP_EDD_SEND_RQB_DATA_SET *pSendRqpInstData = LSA_NULL;

  MRP_LL_RQB_ENUM_ASSERT((LSA_UINT32)poolID);
  mrp_edd_get_nrt_snd_rqb_ptr(&pSendRqpInstData, pDBedd, pInstance, poolID);

  MRP_RQB_PUSH_TO_BOTTOM(pSendRqpInstData->pDBsendRqb, (pSendRqpInstData + 1)->pDBsendRqb, pRQB);
}


/*===========================================================================
* FUNCTION : mrp_edd_set_link_up_time
*----------------------------------------------------------------------------
* PURPOSE  : set time where link up must be stable 
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference for edd system channel
*            port   = 1..numOfPorts 
*            upTime = value of timer ticks (see mrp_int.h) 
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_set_link_up_time (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 port, LSA_UINT16 upTcnt)
{
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  LSA_UINT16 numOfPorts;

  numOfPorts = mrp_sys_get_num_of_ports(pChSys);

  MRP_ASSERT (port > 0 && port <= numOfPorts);

  pDBedd->pLinkData[port - 1].timer_upTimeTcnt = upTcnt;
  
  MRP_LOWER_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_edd_set_link_up_time: port=%u, timeCnt=%u", port, upTcnt);
}


/*===========================================================================
* FUNCTION : mrp_edd_force_link_up
*----------------------------------------------------------------------------
* PURPOSE  : accelerate link_up if link hysteresis timer is already running
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            port     = 1..numOfPorts
* OUTPUTS  : LSA_TRUE = LinkUp running, LSA_FALSE = not running
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT8 mrp_edd_force_link_up (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 port)
{
  LSA_UINT16 idx, numOfPorts;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
 
  numOfPorts = mrp_sys_get_num_of_ports(pChSys);
  
  MRP_ASSERT (port > 0 && port <= numOfPorts);
  idx = (LSA_UINT16)(port - 1);

  if (mrp_sys_timer_get_state (pDBedd->pLinkData[idx].timer_index_up) == MRP_TIMER_STS_T_RUNNING) 
  {
    mrp_sys_timer_stop(pDBedd->pLinkData[idx].timer_index_up);
    mrp_sys_timer_start (pDBedd->pLinkData[idx].timer_index_up, MRP_VAL_LINK_UP_RING_SHORT_T_CNT);
    
    MRP_PROGRAM_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_edd_force_link_up: force LINK_UP event for port=%u, timeCnt=%u", port, MRP_VAL_LINK_UP_RING_SHORT_T_CNT);
    
    return LSA_TRUE;
  }
  return LSA_FALSE;
}


/*===========================================================================
* FUNCTION : mrp_edd_init_finish_upper
*----------------------------------------------------------------------------
* PURPOSE  : finish upper usr request
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference for edd system channel
*            instance = affected MRP instance
*            pChUsr = user channel reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_init_finish_upper (const MRP_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, const MRP_CHANNEL_TYPE_USER pChUsr)
{
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  
  MRP_ASSERT (instance == 0);

  MRP_PROGRAM_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_edd_init_finish_upper[inst=%d] entered", instance);

  pDBedd->pChUsr_finishUpper = pChUsr;
  mrp_ll_burst_req (pChSys, instance, g_MrpllReqTbl_mrp_finish_upper, LSA_TRUE);
} /*lint !e818 CM 22/11/2016 can not made const, because of request burst */


/*===========================================================================
* FUNCTION : mrp_edd_init_finish_close
*----------------------------------------------------------------------------
* PURPOSE  : store usr channel to be closed
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference for edd system channel
*            instance = affected MRP instance
*            pChUsr = user channel reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_edd_init_finish_close (const MRP_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, const MRP_CHANNEL_TYPE_USER pChUsr)
{
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  MRP_ASSERT(instance == 0);

  MRP_PROGRAM_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_edd_init_finish_close[inst=%d] entered", instance);

  pDBedd->pChUsr_finishClose = pChUsr;
  mrp_ll_burst_req (pChSys, instance, g_MrpllReqTbl_mrp_finish_close, LSA_TRUE);
} /*lint !e818 CM 22/11/2016 can not made const, because of request burst */

/*===========================================================================
* FUNCTION : mrp_edd_init_prm_commit
*----------------------------------------------------------------------------
* PURPOSE  : queue a PRM_COMMIT request
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : 
*            pChUsr   = user channel reference
*            pRQB     = usr request RQB (prm_commit)
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : COMMIT without previous END causes a sequence error (a bug in CM-PD).
*            COMMIT in case of inconsistency causes a consistency error (a bug in CM-PD).
*            
*            NOTE:
*            This function is used as central supervision function for MRP functionality!!
*
*            RQB response:
*            MRP_RSP_OK
*            MRP_RSP_ERR_SEQUENCE
*            MRP_RSP_ERR_PRM_CONSISTENCY
*==========================================================================*/
LSA_UINT16 mrp_edd_init_prm_commit (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB)
{
  MRP_REC_DB_TYPE *pDBrec = (MRP_REC_DB_TYPE *)pChUsr->pChUsrDB->pRec;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pDBrec->pChSys->pChSysDB->pEdd;
  LSA_UINT16 mrpMaxInstance;
  LSA_UINT16 instanceArray[2];

  mrpMaxInstance = mrp_sys_get_num_of_instances(pDBrec->pChSys);

  MRP_PROGRAM_TRACE_01(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_edd_init_prm_commit[inst=%d] entered", 0);

  if (pChUsr->detailptr->PathType == MRP_PATH_TYPE_USER && pChUsr->detailptr->type.user.isPrmInterface == MRP_IS_PRM_INTERFACE_ON)
  {
    if (pChUsr->state == MRP_CH_STATE_CLOSING) 
    {
      /* shutdown */
      pDBrec->PrmEnd = LSA_TRUE;
      pDBrec->PrmState = MRP_REC_ENUM_STATE_PRM_END;
    }

    if (!pDBrec->PrmEnd) 
    {
      MRP_PROGRAM_TRACE_00(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> mrp_edd_init_prm_commit: ERR_SEQUENCE: PRM_COMMIT missing PRM_END");
      MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_SEQUENCE);

      return MRP_RET_ERR;
    }
  }
  else
  {
    MRP_PROGRAM_TRACE_02(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_UNEXP, ">> mrp_edd_init_prm_commit: ERR_PRM_CONSISTENCY: invalid channel for PRM_COMMIT (pathType=%d, isPrmIf=%d)", pChUsr->detailptr->PathType, pChUsr->detailptr->type.user.isPrmInterface);
    MRP_RQB_SET_RESPONSE(pRQB, MRP_RSP_ERR_PRM_CONSISTENCY);

    return MRP_RET_ERR;
  }

  pDBedd->pChUsr_prmCommit = pChUsr;
  pDBedd->pRQB_prmCommit   = pRQB;
  pDBrec->PrmState         = MRP_REC_ENUM_STATE_PRM_COMMIT_START;

  MRP_PROGRAM_TRACE_02(pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_edd_init_prm_commit: prm_commit queued pChUsr=0x%x, pRQB=0x%x", pChUsr, pRQB);

  if (mrpMaxInstance == 0)
  {
    instanceArray[MRP_INSTANCE_INDEX] = MRP_DEFAULT_INSTANCE;
    instanceArray[MRP_IN_INSTANCE_INDEX] = MRP_IN_INSTANCE_DEFAULT;

    mrp_edd_req_prm_commit (pDBrec->pChSys, instanceArray, MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_START);
  }
  else
  {
    mrp_ll_burst_req (pDBrec->pChSys, 0, g_MrpllReqTbl_mrp_prm_commit, LSA_FALSE);      /* for mrpMaxInstance = 0 function does nothing! */
  }

  return MRP_RET_OK;
}


/*===========================================================================
* FUNCTION : mrp_edd_init_rqb
*----------------------------------------------------------------------------
* PURPOSE  : allocate and initialize all needed RQBs
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys           = channel reference for edd system channel
*            mrpMaxInstance   = maximum number of MRP instances to be initialized
*            mrpInMaxInstance = maximum number of MRP interconnection instances 
*                               to be initialized
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_edd_init_rqb (const MRP_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance)
{
  LSA_UINT16 numOfPorts;
  LSA_UINT16 i, k, m, idxInst, idxInInst;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  MRP_EDD_INST_DATA *pDBeddInst;
  MRP_EDD_SEND_RQB_DATA_SET *pDBeddSendRqbInstData;

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  numOfPorts = mrp_sys_get_num_of_ports (pChSys);

  /* init RQBs send */
  {
    LSA_UINT16 txQueue;

    idxInst = MRP_LL_SND_RQB_ENUM_TBL_END;
    idxInInst = MRP_LL_SND_RQB_ENUM_IN_TYPES_TBL_END;

    for (i = 0; i < mrpMaxInstance; i++)
    {
      pDBeddSendRqbInstData  = &pDBedd->pInstData[i].sendRqbInstData[0];

      for (txQueue = MRP_LL_SND_RQB_ENUM_TBL_START; txQueue < MRP_LL_SND_RQB_ENUM_TBL_END; txQueue += 2) 
      {
        MRP_ASSERT((txQueue + 1) < idxInst); /* check out of bound array index*/
        pDBeddSendRqbInstData[txQueue].pDBsendRqb = pDBeddSendRqbInstData[txQueue + 1].pDBsendRqb = LSA_NULL;
        MRP_ASSERT(pDBeddSendRqbInstData[txQueue + 1].sendRqbMaxCnt == txQueue + 1);

        for (k = 0; k < pDBeddSendRqbInstData[txQueue].sendRqbMaxCnt; k++) 
        {
          MRP_EDD_LOWER_RQB_PTR_TYPE pRQB;
          MRP_EDD_UPPER_NRT_SEND_PTR_TYPE pParamSnd;
          LSA_UINT8 *pBuffer;

          MRP_EDD_ALLOC_RQB(pChSys, pRQB, sizeof(MRP_EDD_RQB_NRT_SEND_TYPE), EDD_SRV_NRT_SEND); 
          MRP_EDD_ALLOC_LOWER_TX_MEM((MRP_EDD_LOWER_MEM_PTR_TYPE*)&pBuffer, MRP_NRT_TX_BUF_SIZE + MRP_LL_TX_QUEUE_PREFIX_SIZE, pChSys->sysptr);
          
          MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pBuffer, LSA_NULL));
          MRP_FATAL1(!(((LSA_UINT32)(pBuffer)) & 3)); /* ensure 32 bit alignment */
          
          MRP_PROGRAM_TRACE_05(traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_edd_init_rqb[inst=%d]: alloc send rqb=0x%x, pBuffer=0x%x, txQueue=%d, cnt=%d", i, pRQB, pBuffer, txQueue, k);
          
          MRP_LL_SET_TX_QUEUE(pBuffer, txQueue); /*lint !e826 CM 11/11/2016 cast on purpose */
          MRP_LL_SET_TX_INSTANCE(pBuffer, i); /*lint !e826 CM 11/11/2016 cast on purpose */
          MRP_LL_SET_TX_ININSTANCE(pBuffer, MRP_IN_INSTANCE_NONE); /*lint !e826 CM 11/11/2016 cast on purpose */

          MRP_LL_SET_PAYLOAD_ADDR(pBuffer);
          
          MRP_MEMCPY(pBuffer, (LSA_VOID_PTR_TYPE)MRP_PACKET_VAL_DEFAULT, sizeof(MRP_PACKET_VAL_DEFAULT) - 1);
          pParamSnd = MRP_EDD_LOWER_GET_PPARAM_NRT_SEND(pRQB);
          pParamSnd->pBuffer    = pBuffer;
          pParamSnd->Length     = sizeof(MRP_PACKET_VAL_DEFAULT) - 1;
          pParamSnd->PortID     = EDD_PORT_ID_AUTO;
          pParamSnd->Priority   = EDD_NRT_SEND_PRIO_MGMT_HIGH;
          pParamSnd->TxFrmGroup = EDD_NRT_TX_GRP_MRP;
          
          pDBeddSendRqbInstData[txQueue].sendRqbCnt++;
          pDBeddSendRqbInstData[txQueue].sendRqbMissCnt = 0;
                    
          MRP_RQB_PUSH_TO_BOTTOM(pDBeddSendRqbInstData[txQueue].pDBsendRqb, pDBeddSendRqbInstData[txQueue + 1].pDBsendRqb, pRQB);
        }
      }

      /* create and init snd RQBs for interconnection */
      if(i == MRP_DEFAULT_INSTANCE)
      {
        /* loop for MRP interconnection instances */
        for (m = 0; m < mrpInMaxInstance; m++)
        {
          pDBeddSendRqbInstData  = &pDBedd->pInstData[i].sendRqbInInstData[m][0];

          for (txQueue = 0; txQueue < MRP_LL_SND_RQB_ENUM_IN_TYPES_TBL_END; txQueue += 2) 
          {
            MRP_ASSERT((txQueue + 1) < idxInInst); /* check out of bound array index*/
            pDBeddSendRqbInstData[txQueue].pDBsendRqb = pDBeddSendRqbInstData[txQueue + 1].pDBsendRqb = LSA_NULL;
            MRP_ASSERT(pDBeddSendRqbInstData[txQueue + 1].sendRqbMaxCnt == txQueue + 1 + MRP_LL_SND_RQB_ENUM_IN_TYPES_START_OFFSET);

            for (k = 0; k < pDBeddSendRqbInstData[txQueue].sendRqbMaxCnt; k++) 
            {
              MRP_EDD_LOWER_RQB_PTR_TYPE pRQB;
              MRP_EDD_UPPER_NRT_SEND_PTR_TYPE pParamSnd;
              LSA_UINT8 *pBuffer;

              MRP_EDD_ALLOC_RQB(pChSys, pRQB, sizeof(MRP_EDD_RQB_NRT_SEND_TYPE), EDD_SRV_NRT_SEND); 
              MRP_EDD_ALLOC_LOWER_TX_MEM((MRP_EDD_LOWER_MEM_PTR_TYPE*)&pBuffer, MRP_NRT_TX_BUF_SIZE + MRP_LL_TX_QUEUE_PREFIX_SIZE, pChSys->sysptr);
          
              MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pBuffer, LSA_NULL));
              MRP_FATAL1(!(((LSA_UINT32)(pBuffer)) & 3)); /* ensure 32 bit alignment */
          
              MRP_PROGRAM_TRACE_05(traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_edd_init_rqb[inInst=%d]: alloc send rqb=0x%x, pBuffer=0x%x, txQueue=%d, cnt=%d", m, pRQB, pBuffer, (txQueue + MRP_LL_SND_RQB_ENUM_IN_TYPES_START_OFFSET), k);
          
              MRP_LL_SET_TX_QUEUE(pBuffer, txQueue + MRP_LL_SND_RQB_ENUM_IN_TYPES_START_OFFSET); /*lint !e826 CM 11/11/2016 cast on purpose */
              MRP_LL_SET_TX_INSTANCE(pBuffer, i); /*lint !e826 CM 11/11/2016 cast on purpose */
              MRP_LL_SET_TX_ININSTANCE(pBuffer, m); /*lint !e826 CM 11/11/2016 cast on purpose */

              MRP_LL_SET_PAYLOAD_ADDR(pBuffer);
          
              MRP_MEMCPY(pBuffer, (LSA_VOID_PTR_TYPE)MRP_PACKET_VAL_DEFAULT, sizeof(MRP_PACKET_VAL_DEFAULT) - 1);
              pParamSnd = MRP_EDD_LOWER_GET_PPARAM_NRT_SEND(pRQB);
              pParamSnd->pBuffer    = pBuffer;
              pParamSnd->Length     = sizeof(MRP_PACKET_VAL_DEFAULT) - 1;
              pParamSnd->PortID     = EDD_PORT_ID_AUTO;
              pParamSnd->Priority   = EDD_NRT_SEND_PRIO_MGMT_HIGH;
              pParamSnd->TxFrmGroup = EDD_NRT_TX_GRP_MRP;
          
              pDBeddSendRqbInstData[txQueue].sendRqbCnt++;
              pDBeddSendRqbInstData[txQueue].sendRqbMissCnt = 0;
                    
              MRP_RQB_PUSH_TO_BOTTOM(pDBeddSendRqbInstData[txQueue].pDBsendRqb, pDBeddSendRqbInstData[txQueue + 1].pDBsendRqb, pRQB);
            }
          }
        }
      }
    }
  }
  /* init RQBs recv */
  {
    LSA_UINT16 numOfRcvResources;

    numOfRcvResources = (LSA_UINT16)(MRP_GET_NRT_RCV_RQB_PER_MRP_INST(mrpMaxInstance, mrpInMaxInstance));

    for (k = 0; k < numOfRcvResources; k++)
    {
      MRP_EDD_LOWER_RQB_PTR_TYPE pRQB;
      MRP_EDD_UPPER_NRT_RECV_PTR_TYPE pParamRcv;
      LSA_UINT8 *pBuffer;

      MRP_EDD_ALLOC_RQB(pChSys, pRQB, sizeof(MRP_EDD_RQB_NRT_RECV_TYPE), EDD_SRV_NRT_RECV);
      MRP_EDD_ALLOC_LOWER_RX_MEM((MRP_EDD_LOWER_MEM_PTR_TYPE*)&pBuffer, MRP_NRT_RX_BUF_SIZE, pChSys->sysptr);
      MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pBuffer, LSA_NULL));
      
      pParamRcv = MRP_EDD_LOWER_GET_PPARAM_NRT_RECV(pRQB);
      pParamRcv->pBuffer   = pBuffer;
      pParamRcv->RequestID = k;
      
      MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_edd_init_rqb: alloc recv rqb=0x%x, pBuffer=0x%x, cnt=%d", pRQB, pBuffer, k);
      
      mrp_edd_req_rcv(pRQB);
    }
  }
  /* init RQBs link change */
  {
    LSA_UINT16 port;

    if (mrpMaxInstance)
    {
      for (port = 1; port <= numOfPorts; port++) 
      {
        MRP_EDD_LOWER_RQB_PTR_TYPE pRQB;
        MRP_EDD_UPPER_LINK_STATUS_IND_PROVIDE_PTR_TYPE pParamLink;

        MRP_EDD_ALLOC_RQB(pChSys, pRQB, sizeof(MRP_EDD_RQB_LINK_STATUS_IND_PROVIDE_TYPE), EDD_SRV_LINK_STATUS_IND_PROVIDE);
      
        MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_edd_init_rqb: alloc link change rqb=0x%x, port=%d", pRQB, port);
      
        pParamLink = MRP_EDD_LOWER_GET_PPARAM_LINK_STATUS_IND(pRQB);
        MRP_MEMSET(pParamLink, 0, sizeof(MRP_EDD_RQB_LINK_STATUS_IND_PROVIDE_TYPE));

        pParamLink->PortID  = port;
        pParamLink->PhyStatus = EDD_PHY_STATUS_UNKNOWN;
        pParamLink->MAUType = EDD_MAUTYPE_UNKNOWN;

        mrp_edd_req_linkChg(pRQB);
      }
    }
  }
  /* init RQBs multicast */
  {
    MRP_EDD_UPPER_MULTICAST_PTR_TYPE pMcastParam;

    MRP_EDD_ALLOC_RQB(pChSys, pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_MULTICAST], sizeof(MRP_EDD_RQB_MULTICAST_TYPE), EDD_SRV_MULTICAST);
    pMcastParam = MRP_EDD_LOWER_GET_PPARAM_MULTICAST(pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_MULTICAST]);
    pMcastParam->Mode = EDD_MULTICAST_DISABLE;
    
    MRP_PROGRAM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_edd_init_rqb: alloc multicast rqb=0x%x", pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_MULTICAST]);
    
    for (i = 0; i < mrpMaxInstance; i++)
    {
      pDBeddInst = &pDBedd->pInstData[i];
      for (k = MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_START; k < MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_END; k++) 
      {
        MRP_EDD_ALLOC_RQB(pChSys, pDBeddInst->pDBshadowRqb[k], sizeof(MRP_EDD_RQB_MULTICAST_TYPE), EDD_SRV_MULTICAST);
        pMcastParam = MRP_EDD_LOWER_GET_PPARAM_MULTICAST(pDBeddInst->pDBshadowRqb[k]);
        MRP_MEMSET(pMcastParam, 0, sizeof(MRP_EDD_RQB_MULTICAST_TYPE));
        
        MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_edd_init_rqb[inst=%d]: alloc multicast shadow rqb=0x%x, type=%d", i, pDBeddInst->pDBshadowRqb[k], k);
        pMcastParam->Mode = EDD_MULTICAST_DISABLE;
        
        switch (k) 
        {
        case MRP_LL_SREQ_RQB_ENUM_MULTICAST_WRK: 
          MRP_MEMCPY(pMcastParam->MACAddr.MacAdr, (LSA_VOID_PTR_TYPE)MRP_PACKET_VAL_DST_ADR_NSM_WRK, sizeof(pMcastParam->MACAddr.MacAdr)); 
          break;

        case MRP_LL_SREQ_RQB_ENUM_MULTICAST_TST: 
          MRP_MEMCPY(pMcastParam->MACAddr.MacAdr, (LSA_VOID_PTR_TYPE)MRP_PACKET_VAL_DST_ADR_NSM_TST, sizeof(pMcastParam->MACAddr.MacAdr)); 
          break;
        case MRP_LL_SREQ_RQB_ENUM_MULTICAST_HSYNC:
          MRP_MEMCPY (pMcastParam->MACAddr.MacAdr, (LSA_VOID_PTR_TYPE)MRP_PACKET_VAL_DST_ADR_HA_VENDOR, sizeof (pMcastParam->MACAddr.MacAdr));
          break;

  #ifdef MRP_CFG_PLUGIN_2_HSR
        case MRP_LL_SREQ_RQB_ENUM_MULTICAST_HSR_TPLG_CHG: 
          MRP_MEMCPY(pMcastParam->MACAddr.MacAdr, (LSA_VOID_PTR_TYPE)MRP_PACKET_DST_ADR_TPLG_CHG, sizeof(pMcastParam->MACAddr.MacAdr)); 
          break;

        case MRP_LL_SREQ_RQB_ENUM_MULTICAST_HSR_TST     : 
          MRP_MEMCPY(pMcastParam->MACAddr.MacAdr, (LSA_VOID_PTR_TYPE)MRP_PACKET_DST_ADR_TST, sizeof(pMcastParam->MACAddr.MacAdr)); 
          break;

        case MRP_LL_SREQ_RQB_ENUM_MULTICAST_STBY_PARTNER: 
          MRP_MEMCPY(pMcastParam->MACAddr.MacAdr, (LSA_VOID_PTR_TYPE)MRP_PACKET_DST_ADR_TPLG_CHG_STBY_PARTNER, sizeof(pMcastParam->MACAddr.MacAdr)); 
          break;

        case MRP_LL_SREQ_RQB_ENUM_MULTICAST_STBY_COM    : 
          MRP_MEMCPY(pMcastParam->MACAddr.MacAdr, (LSA_VOID_PTR_TYPE)MRP_PACKET_DST_ADR_STBY_COM, sizeof(pMcastParam->MACAddr.MacAdr)); 
          break;
  #endif
        case MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_TST:
          MRP_MEMCPY (pMcastParam->MACAddr.MacAdr, (LSA_VOID_PTR_TYPE)MRP_IN_PACKET_VAL_DST_ADR_TST, sizeof (pMcastParam->MACAddr.MacAdr));
          break;

        case MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_WRK:
          MRP_MEMCPY (pMcastParam->MACAddr.MacAdr, (LSA_VOID_PTR_TYPE)MRP_IN_PACKET_VAL_DST_ADR_WRK, sizeof (pMcastParam->MACAddr.MacAdr));
          break;

        case MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_TRF:
          MRP_MEMCPY (pMcastParam->MACAddr.MacAdr, (LSA_VOID_PTR_TYPE)MRP_IN_PACKET_VAL_DST_ADR_TRF, sizeof (pMcastParam->MACAddr.MacAdr));
          break;

        default: MRP_ASSERT(0); 
          break;
        }
      }
    }
  }
  /* init RQBs multicast fwd ctrl */
  {
    MRP_EDD_UPPER_MULTICAST_FWD_CTRL_PTR_TYPE pMcastFwdParam;

    MRP_EDD_ALLOC_RQB(pChSys, pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_MULTICAST_FWD_CTRL], sizeof(MRP_EDD_RQB_MULTICAST_FWD_CTRL_TYPE), EDD_SRV_MULTICAST_FWD_CTRL);
    pMcastFwdParam = MRP_EDD_LOWER_GET_PPARAM_MULTICAST_FWD_CTRL(pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_MULTICAST_FWD_CTRL]);
    
    MRP_EDD_ALLOC_LOWER_MEM(((MRP_EDD_LOWER_MEM_PTR_TYPE*)&pMcastFwdParam->pPortIDModeArray), (LSA_UINT16)(sizeof(EDD_RQB_PORTID_MODE_TYPE) * numOfPorts), pChSys->sysptr);
    
    MRP_PROGRAM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_edd_init_rqb: alloc multicast fwd ctrl rqb=0x%x", pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_MULTICAST_FWD_CTRL]);
    
    if (LSA_HOST_PTR_ARE_EQUAL(pMcastFwdParam->pPortIDModeArray, LSA_NULL)) 
    {
      MRP_LOWER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_FATAL, "out of lower memory for MRP_EDD_LOWER_MEM_PTR_TYPE");
    }
    pMcastFwdParam->PortIDModeCnt = numOfPorts;

    for (i = 0; i < mrpMaxInstance; i++)
    {
      pDBeddInst = &pDBedd->pInstData[i];
      for (k = MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_TBL_START; k < MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_TBL_END; k++) 
      {
        MRP_EDD_ALLOC_RQB(pChSys, pDBeddInst->pDBshadowRqb[k], sizeof(MRP_EDD_RQB_MULTICAST_FWD_CTRL_TYPE), EDD_SRV_MULTICAST_FWD_CTRL);

        pMcastFwdParam = MRP_EDD_LOWER_GET_PPARAM_MULTICAST_FWD_CTRL(pDBeddInst->pDBshadowRqb[k]);
        MRP_EDD_ALLOC_LOWER_MEM(((MRP_EDD_LOWER_MEM_PTR_TYPE*)&pMcastFwdParam->pPortIDModeArray), (LSA_UINT16)(sizeof(EDD_RQB_PORTID_MODE_TYPE) * numOfPorts), pChSys->sysptr);

        MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_edd_init_rqb[inst=%d]: alloc multicast fwd ctrl shadow rqb=0x%x, type=%d", i, pDBeddInst->pDBshadowRqb[k], k);
        
        if (LSA_HOST_PTR_ARE_EQUAL(pMcastFwdParam->pPortIDModeArray, LSA_NULL)) 
        {
          MRP_LOWER_TRACE_00(traceIdx, LSA_TRACE_LEVEL_FATAL, "out of lower memory for MRP_EDD_LOWER_MEM_PTR_TYPE");
        }
        pMcastFwdParam->PortIDModeCnt = numOfPorts;

        for (m = 0; m < numOfPorts; m++) 
        {
          pMcastFwdParam->pPortIDModeArray[m].Mode      = EDD_MULTICAST_FWD_DISABLE;
          pMcastFwdParam->pPortIDModeArray[m].DstPortID = (LSA_UINT16)(m + 1);
          pMcastFwdParam->pPortIDModeArray[m].SrcPortID = (LSA_UINT16) EDD_PORT_ID_AUTO;

        }
        switch (k) 
        {
        case MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_TST: 
          pMcastFwdParam->MACAddrGroup = EDD_MAC_ADDR_GROUP_MRP_1; 
          break; /* MRP_PACKET_VAL_DST_ADR_NSM_TST */

        case MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_WRK: 
          pMcastFwdParam->MACAddrGroup = EDD_MAC_ADDR_GROUP_MRP_2; 
          break; /* MRP_PACKET_VAL_DST_ADR_NSM_WRK */
        
        case MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_HSYNC:
          pMcastFwdParam->MACAddrGroup  = EDD_MAC_ADDR_GROUP_HSYNC;
          break; /* MRP_PACKET_VAL_DST_ADR_HA_VENDOR */

  #ifdef MRP_CFG_PLUGIN_2_HSR
        case MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_HSR_TPLG_CHG: 
          pMcastFwdParam->MACAddrGroup = EDD_MAC_ADDR_GROUP_HSR_1; 
          break;

        case MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_HSR_TST     : 
          pMcastFwdParam->MACAddrGroup = EDD_MAC_ADDR_GROUP_HSR_2; 
          break;

        case MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_STBY_PARTNER: 
          pMcastFwdParam->MACAddrGroup = EDD_MAC_ADDR_GROUP_STBY_1; 
          break;

        case MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_STBY_COM    : 
          pMcastFwdParam->MACAddrGroup = EDD_MAC_ADDR_GROUP_STBY_2; 
          break;
  #endif

        case MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_TST:
          pMcastFwdParam->MACAddrGroup = EDD_MAC_ADDR_GROUP_MRP_IN_1;
          break; /* MRP_IN_PACKET_VAL_DST_ADR_TST */

        case MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_WRK:
          pMcastFwdParam->MACAddrGroup = EDD_MAC_ADDR_GROUP_MRP_IN_2;
          break; /* MRP_IN_PACKET_VAL_DST_ADR_WRK */

        case MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_TRF:
          pMcastFwdParam->MACAddrGroup = EDD_MAC_ADDR_GROUP_MRP_IN_3;
          break; /* MRP_IN_PACKET_VAL_DST_ADR_TRF */

        default: 
          MRP_ASSERT(0); 
          break;
        }
      }    
    }
  }
  /* init RQBs flush filtering db */
  {
    MRP_EDD_UPPER_SWI_FLUSH_FILTERING_DB_PTR_TYPE pFlushFilterParam;

    MRP_EDD_ALLOC_RQB(pChSys, pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_FLUSH_FILTERING_DB], sizeof(MRP_EDD_RQB_SWI_FLUSH_FILTERING_DB_TYPE), EDD_SRV_SWITCH_FLUSH_FILTERING_DB);
    MRP_PROGRAM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_edd_init_rqb: alloc flush filtering rqb=0x%x", pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_FLUSH_FILTERING_DB]);
    
    for (i = 0; i < mrpMaxInstance; i++)
    {
      pDBeddInst = &pDBedd->pInstData[i];
      for (k = MRP_LL_SREQ_RQB_ENUM_FLUSH_FILTERING_DB_TBL_START; k < MRP_LL_SREQ_RQB_ENUM_FLUSH_FILTERING_DB_TBL_END; k++) 
      {
        MRP_EDD_ALLOC_RQB(pChSys, pDBeddInst->pDBshadowRqb[k], sizeof(MRP_EDD_RQB_SWI_FLUSH_FILTERING_DB_TYPE), EDD_SRV_SWITCH_FLUSH_FILTERING_DB);

        MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_edd_init_rqb[inst=%d]: alloc flush filtering rqb=0x%x, type=%d", i, pDBeddInst->pDBshadowRqb[k], k);
        
        /* default: flush all ports */ 
        /* n-domains: if more than one MRP instance is active, flush only ringport entries related to this MRP instance */ 
        pFlushFilterParam = MRP_EDD_LOWER_GET_PPARAM_FLUSH_FILTERING_DB(pDBeddInst->pDBshadowRqb[k]);

        for (m = 0; m < numOfPorts; m++) 
        {
          pFlushFilterParam->PortID[m] = m + 1;
        }
        pFlushFilterParam->PortCnt = numOfPorts;
      }     
    }
  }
  /* init RQBs port state */
  {
    MRP_EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE pPSParam;

    MRP_EDD_ALLOC_RQB(pChSys, pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_PORT_STATE], sizeof(MRP_EDD_RQB_SWI_SET_PORT_STATE_TYPE), EDD_SRV_SWITCH_SET_PORT_STATE);
    MRP_PROGRAM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_edd_init_rqb: alloc req port state rqb=0x%x", pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_PORT_STATE]);
    
    for (i = 0; i < mrpMaxInstance; i++)
    {
      pDBeddInst = &pDBedd->pInstData[i];
      for (k = MRP_LL_SREQ_RQB_ENUM_SET_PS_TBL_START; k < MRP_LL_SREQ_RQB_ENUM_SET_PS_TBL_END; k++) 
      {
        MRP_EDD_ALLOC_RQB(pChSys, pDBeddInst->pDBshadowRqb[k], sizeof(MRP_EDD_RQB_SWI_SET_PORT_STATE_TYPE), EDD_SRV_SWITCH_SET_PORT_STATE);

        MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_edd_init_rqb[inst=%d]: alloc req port state rqb=0x%x, type=%d", i, pDBeddInst->pDBshadowRqb[k], k);
        
        pPSParam = MRP_EDD_LOWER_GET_PPARAM_SET_PORT_STATE(pDBeddInst->pDBshadowRqb[k]); 

        for (m = 0; m < numOfPorts; m++) 
        {
          pPSParam->PortIDState[m] = EDD_PORT_STATE_UNCHANGED;
        }
      }    
    }
  }
}


/*===========================================================================
* FUNCTION : mrp_edd_deinit_rqb
*----------------------------------------------------------------------------
* PURPOSE  : free all used RQBs
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference for edd system channel
*            mrpMaxInstance = maximum number of MRP instances to be initialized
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_edd_deinit_rqb (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance)
{
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  LSA_UINT16 i, k, m, idxInst, idxInInst;
  MRP_EDD_SEND_RQB_DATA_SET *pDBeddSendRqbInstData;
  LSA_UINT16  pInstance[2];

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  /* deinit RQBs send */
  {
    LSA_INT32 txQueue;

    idxInst = MRP_LL_SND_RQB_ENUM_TBL_END;
    idxInInst = MRP_LL_SND_RQB_ENUM_IN_TYPES_TBL_END;

    for (i = 0; i < mrpMaxInstance; i++)
    {
      pInstance[MRP_INSTANCE_INDEX]     = i;
      pInstance[MRP_IN_INSTANCE_INDEX]  = MRP_IN_INSTANCE_NONE;
      pDBeddSendRqbInstData  = &pDBedd->pInstData[i].sendRqbInstData[0];

      for (txQueue = MRP_LL_SND_RQB_ENUM_TBL_START; txQueue < MRP_LL_SND_RQB_ENUM_TBL_END; txQueue += 2) 
      {
        MRP_ASSERT((txQueue + 1) < idxInst);  /* check out of bound array index*/
        MRP_ASSERT(pDBeddSendRqbInstData[txQueue + 1].sendRqbMaxCnt == txQueue + 1);

        for (k = 0; k < pDBeddSendRqbInstData[txQueue].sendRqbMaxCnt; k++) 
        {
          MRP_EDD_LOWER_RQB_PTR_TYPE pRQB;
          MRP_EDD_UPPER_NRT_SEND_PTR_TYPE pParamSnd;
          LSA_UINT8 *pBuffer;
          LSA_UINT16 ret_val_ptr = LSA_RET_ERR_PARAM;

          (void)(mrp_edd_get_snd_rqb (pChSys, pInstance, (MRP_LL_SND_RQB_ENUM_T)txQueue, &pRQB, MRP_EDD_GET_SND_RQB_NO_THRESHOLD));

          MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL));

          pParamSnd = MRP_EDD_LOWER_GET_PPARAM_NRT_SEND(pRQB);
          pBuffer = pParamSnd->pBuffer;

          MRP_LL_RESET_TX_QUEUE(pBuffer); /*lint !e826 CM 11/11/2016 cast on purpose */
          MRP_LL_RESET_TX_INSTANCE(pBuffer); /*lint !e826 CM 11/11/2016 cast on purpose */
          MRP_LL_RESET_TX_ININSTANCE(pBuffer); /*lint !e826 CM 11/11/2016 cast on purpose */
          pBuffer = MRP_LL_GET_BASE_ADDR(pBuffer);
          
          MRP_PROGRAM_TRACE_05(traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_edd_deinit_rqb: dealloc send rqb=0x%x, pBuffer=0x%x, inst=%d, txQueue=%d, cnt=%d", pRQB, pBuffer, i, txQueue, k);
          
          MRP_EDD_FREE_LOWER_TX_MEM(&ret_val_ptr, pBuffer, pChSys->sysptr);

          MRP_FATAL1(ret_val_ptr == LSA_RET_OK);
          mrp_edd_free_rqb(pRQB);
        }
      }

      /* free snd RQBs for interconnection */
      if(i == MRP_DEFAULT_INSTANCE)
      {
        /* loop for MRP interconnection instances */
        for (m = 0; m < mrpInMaxInstance; m++)
        {
          pInstance[MRP_IN_INSTANCE_INDEX]  = m;
          pDBeddSendRqbInstData  = &pDBedd->pInstData[i].sendRqbInInstData[m][0];

          for (txQueue = 0; txQueue < MRP_LL_SND_RQB_ENUM_IN_TYPES_TBL_END; txQueue += 2) 
          {
            MRP_ASSERT((txQueue + 1) < idxInInst);  /* check out of bound array index*/
            MRP_ASSERT(pDBeddSendRqbInstData[txQueue + 1].sendRqbMaxCnt == txQueue + 1 + MRP_LL_SND_RQB_ENUM_IN_TYPES_START_OFFSET);

            for (k = 0; k < pDBeddSendRqbInstData[txQueue].sendRqbMaxCnt; k++) 
            {
              MRP_EDD_LOWER_RQB_PTR_TYPE pRQB;
              MRP_EDD_UPPER_NRT_SEND_PTR_TYPE pParamSnd;
              LSA_UINT8 *pBuffer;
              LSA_UINT16 ret_val_ptr = LSA_RET_ERR_PARAM;

              (void)(mrp_edd_get_snd_rqb (pChSys, pInstance, (MRP_LL_SND_RQB_ENUM_T)(txQueue + MRP_LL_SND_RQB_ENUM_IN_TYPES_START_OFFSET), &pRQB, MRP_EDD_GET_SND_RQB_NO_THRESHOLD));

              MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL));

              pParamSnd = MRP_EDD_LOWER_GET_PPARAM_NRT_SEND(pRQB);
              pBuffer = pParamSnd->pBuffer;

              MRP_LL_RESET_TX_QUEUE(pBuffer); /*lint !e826 CM 11/11/2016 cast on purpose */
              MRP_LL_RESET_TX_INSTANCE(pBuffer); /*lint !e826 CM 11/11/2016 cast on purpose */
              MRP_LL_RESET_TX_ININSTANCE(pBuffer); /*lint !e826 CM 11/11/2016 cast on purpose */
              pBuffer = MRP_LL_GET_BASE_ADDR(pBuffer);
          
              MRP_PROGRAM_TRACE_05(traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_edd_deinit_rqb: dealloc send rqb=0x%x, pBuffer=0x%x, inInst=%d, txQueue=%d, cnt=%d", pRQB, pBuffer, m, (txQueue + MRP_LL_SND_RQB_ENUM_IN_TYPES_START_OFFSET), k);
          
              MRP_EDD_FREE_LOWER_TX_MEM(&ret_val_ptr, pBuffer, pChSys->sysptr);

              MRP_FATAL1(ret_val_ptr == LSA_RET_OK);
              mrp_edd_free_rqb(pRQB);
            }
          }
        }
      }

    }
  }
  /* deinit RQBs recv */
  {
    /* free will be done from ll cbf */
  }
  /* deinit RQBs link change */
  {
    /* free will be done from ll cbf */
  }
  /* deinit RQBs multicast */
  {
    MRP_PROGRAM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_edd_deinit_rqb: dealloc multicast rqb=0x%x", pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_MULTICAST]);
    mrp_edd_free_rqb(pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_MULTICAST]);
    
    for (i = 0; i < mrpMaxInstance; i++)
    {
      for (k = MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_START; k < MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_END; k++) 
      {
        MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_edd_deinit_rqb: dealloc multicast shadow rqb=0x%x, inst=%d, type=%d", pDBedd->pInstData[i].pDBshadowRqb[k], i, k);
        mrp_edd_free_rqb(pDBedd->pInstData[i].pDBshadowRqb[k]);
      }
    }
  }
  
  /* deinit RQBs multicast fwd ctrl */
  {
    LSA_UINT16 rc;
    MRP_EDD_UPPER_MULTICAST_FWD_CTRL_PTR_TYPE pMcastFwdParam;

    pMcastFwdParam = MRP_EDD_LOWER_GET_PPARAM_MULTICAST_FWD_CTRL(pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_MULTICAST_FWD_CTRL]);
    rc = LSA_RET_OK;
    
    MRP_PROGRAM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_edd_deinit_rqb: dealloc multicast fwd ctrl rqb=0x%x", pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_MULTICAST_FWD_CTRL]);
    MRP_EDD_FREE_LOWER_MEM(&rc, pMcastFwdParam->pPortIDModeArray, pChSys->sysptr);
    
    MRP_FATAL1(rc == LSA_RET_OK);
    mrp_edd_free_rqb (pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_MULTICAST_FWD_CTRL]);

    for (i = 0; i < mrpMaxInstance; i++)
    {
      for (k = MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_TBL_START; k < MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_TBL_END; k++) 
      {
        MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_edd_deinit_rqb: dealloc multicast fwd ctrl shadow rqb=0x%x, inst=%d, type=%d", pDBedd->pInstData[i].pDBshadowRqb[k], i, k);
        pMcastFwdParam = MRP_EDD_LOWER_GET_PPARAM_MULTICAST_FWD_CTRL(pDBedd->pInstData[i].pDBshadowRqb[k]);
        MRP_EDD_FREE_LOWER_MEM(&rc, pMcastFwdParam->pPortIDModeArray, pChSys->sysptr);
        
        MRP_FATAL1(rc == LSA_RET_OK);
        mrp_edd_free_rqb (pDBedd->pInstData[i].pDBshadowRqb[k]);
      }    
    }
  }
  /* deinit RQBs flush filtering db */
  {
    MRP_PROGRAM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_edd_deinit_rqb: dealloc flush filtering rqb=0x%x", pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_FLUSH_FILTERING_DB]);
    mrp_edd_free_rqb(pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_FLUSH_FILTERING_DB]);
    
    for (i = 0; i < mrpMaxInstance; i++)
    {
      for (k = MRP_LL_SREQ_RQB_ENUM_FLUSH_FILTERING_DB_TBL_START; k < MRP_LL_SREQ_RQB_ENUM_FLUSH_FILTERING_DB_TBL_END; k++) 
      {
        MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_edd_deinit_rqb: dealloc flush filtering rqb=0x%x, inst=%d, type=%d", pDBedd->pInstData[i].pDBshadowRqb[k], i, k);
        mrp_edd_free_rqb(pDBedd->pInstData[i].pDBshadowRqb[k]);
      }
    }
  }
  /* deinit RQBs port state */
  {
    MRP_PROGRAM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_edd_deinit_rqb: dealloc req port state rqb=0x%x", pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_PORT_STATE]);
    mrp_edd_free_rqb(pDBedd->pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_PORT_STATE]);

    for (i = 0; i < mrpMaxInstance; i++)
    {
      for (k = MRP_LL_SREQ_RQB_ENUM_SET_PS_TBL_START; k < MRP_LL_SREQ_RQB_ENUM_SET_PS_TBL_END; k++) 
      {
        MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_edd_deinit_rqb: dealloc req port state rqb=0x%x, inst=%d, type=%d", pDBedd->pInstData[i].pDBshadowRqb[k], i, k);
        mrp_edd_free_rqb(pDBedd->pInstData[i].pDBshadowRqb[k]);
      }
    }
  }
}


/*===========================================================================
* FUNCTION : mrp_edd_init
*----------------------------------------------------------------------------
* PURPOSE  : initialize lower layer structures
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference for edd system channel
*            mrpMaxInstance = maximum number of MRP instances to be initialized
*            mrpInMaxInstance = maximum number of MRP interconnection instances
*            to be initialized
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_edd_init (const MRP_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance)
{
  LSA_UINT16 i, k, m, numOfPorts, newCnt, sumCnt, freeEntries, portNum;
  LSA_UINT16 sysCnt;
  LSA_UINT16 id = 0;
  LSA_UINT32 timerIdx;
  MRP_EDD_INST_DATA *pDBeddInst;
  LSA_UINT16 tmpSendRqbMax[MRP_LL_SND_RQB_ENUM_TBL_END];
  LSA_UINT16 tmpSendInRqbMax[MRP_LL_SND_RQB_ENUM_IN_TYPES_TBL_END];
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  /* prepare REQ_TBL part of burst table initialization */
  const MRP_LL_REQ_TBL_T *pTmpReqTbl[MRP_MAX_NO_OF_REQUEST_BURSTS];
  /* !!! NOTE: adapt "mrp_ll_burst_trace as well, if structure is extended */
  MRP_IF_DB_TYPE *pDBif = (MRP_IF_DB_TYPE *)pChSys->pChSysDB->pInterfacePara;

  MRP_EDD_DB_TYPE *pDBedd = LSA_NULL;
  /* prepare initialization of send RQB max. count for each LL interface within pDBedd */ 

  MRP_PROGRAM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_edd_init entered, mrpMaxInstance=%d", mrpMaxInstance);
  
  for (i = 0; i < MRP_LL_SND_RQB_ENUM_TBL_END; i++)
  {
    /* initialize with defaults*/
    tmpSendRqbMax[i] = mrpDefaultSendRqbMax[i];
  }

  for (i = MRP_LL_SND_RQB_ENUM_IN_TYPES_START_OFFSET; i <= MRP_LL_SND_RQB_ENUM_IN_TYPES_END_OFFSET; i++)
  {
    /* initialize interconnection instances with defaults*/
    tmpSendInRqbMax[MRP_GET_IN_NRT_SND_FRAME_TABLE_INDEX(i)] = mrpDefaultSendRqbMax[i];
  }

  /* there is no need to allocate send RQB resources for interconnection in the MRP data set (sendRqbInstData), because it is done in the MRP interconnection
    data set (sendRqbInInstData) per inInstance. */
  tmpSendRqbMax[MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_ORIG_BOT] = 0;

  tmpSendRqbMax[MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_BOT]      = 0;
  tmpSendRqbMax[MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_POLL_BOT]    = 0;
  tmpSendRqbMax[MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_LNK_CHG_BOT] = 0;

  tmpSendRqbMax[MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_TC_BOT]      = 0;
  tmpSendRqbMax[MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_POLL_BOT]    = 0;
  tmpSendRqbMax[MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_LNK_CHG_BOT] = 0;

  if (pDBif->mrpInOriginator == MRP_IN_ORIGINATOR_OFF)
  {
    /* there is no need to allocate send RQB resources, if IN_ORIGINATOR is not supported for this interface */
    tmpSendInRqbMax[MRP_GET_IN_NRT_SND_FRAME_TABLE_INDEX(MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_ORIG_BOT)]  = 0; /*lint !e778*/
  }
  if (pDBif->mrpInSupportedRoleInst0 == EDD_SUPPORTED_MRP_INTERCONN_ROLE_CAP_NONE)
  {
    /* there is no need to allocate send RQB resources, if no MRP interconnection role is supported for this interface */

    tmpSendInRqbMax[MRP_GET_IN_NRT_SND_FRAME_TABLE_INDEX(MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_BOT)]  = 0;
    tmpSendInRqbMax[MRP_GET_IN_NRT_SND_FRAME_TABLE_INDEX(MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_POLL_BOT)]  = 0;
    tmpSendInRqbMax[MRP_GET_IN_NRT_SND_FRAME_TABLE_INDEX(MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_LNK_CHG_BOT)]  = 0;

    tmpSendInRqbMax[MRP_GET_IN_NRT_SND_FRAME_TABLE_INDEX(MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_TC_BOT)]  = 0;
    tmpSendInRqbMax[MRP_GET_IN_NRT_SND_FRAME_TABLE_INDEX(MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_POLL_BOT)]  = 0;
    tmpSendInRqbMax[MRP_GET_IN_NRT_SND_FRAME_TABLE_INDEX(MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_LNK_CHG_BOT)]  = 0;
  }

  for (i = 0; i < MRP_MAX_NO_OF_REQUEST_BURSTS; i++)
  {
    pTmpReqTbl[i] = LSA_NULL;
  }
  sumCnt = 0;
  freeEntries = MRP_MAX_NO_OF_REQUEST_BURSTS;
  newCnt = mrp_ll_get_mrp_base_req_tbl (pTmpReqTbl, freeEntries);
  sumCnt += newCnt;
  MRP_ASSERT (freeEntries > newCnt);
  freeEntries -= newCnt;

  /* MRP interconnection instances */
  for (i = 0; i < EDD_CFG_MAX_MRP_IN_INSTANCE_CNT; i++)
  {
    newCnt = mrp_ll_get_mrp_in_base_req_tbl(&pTmpReqTbl[sumCnt], freeEntries);
    sumCnt += newCnt;
    MRP_ASSERT(freeEntries > newCnt);
    freeEntries -= newCnt;
  }

#ifdef MRP_CFG_PLUGIN_0_MRM
  newCnt = mrp_ll_get_mrm_req_tbl (&pTmpReqTbl[sumCnt], freeEntries);
  sumCnt += newCnt;
  MRP_ASSERT (freeEntries > newCnt);
  freeEntries -= newCnt;
#else
  LSA_UNUSED_ARG(sumCnt);
#endif
#ifdef MRP_CFG_PLUGIN_1_MRM_EXT
  newCnt = mrp_ll_get_mrm_ext_req_tbl (&pTmpReqTbl[sumCnt], freeEntries);
  sumCnt += newCnt;
  MRP_ASSERT (freeEntries > newCnt);
  freeEntries -= newCnt;
#endif
#ifdef MRP_CFG_PLUGIN_2_HSR
  newCnt = mrp_bs_get_hsr_req_tbl (&pTmpReqTbl[sumCnt], freeEntries);
  sumCnt += newCnt;
  MRP_ASSERT (freeEntries > newCnt);
  freeEntries -= newCnt;
#endif

  MRP_ALLOC_LOCAL_MEM(((MRP_LOCAL_MEM_PTR_TYPE*)&pDBedd), sizeof(MRP_EDD_DB_TYPE));
  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBedd, LSA_NULL));  
  pChSys->pChSysDB->pEdd = pDBedd;

  /* preset zero */
  MRP_MEMSET(pDBedd, 0, sizeof(MRP_EDD_DB_TYPE));

  /* channel attributes */
  pDBedd->pChSys = pChSys;

  /* allocate interface related timer representative of cyclic timer*/
  pDBedd->TimerIdCyclic  = mrp_sys_timer_get (pChSys, MRP_TIMER_POOL_INTERFACE, MRP_TIMER_INTERFACE, LSA_TIMER_TYPE_CYCLIC, MRP_TIMER_ENUM_MRP_EDD_CYCLIC, (MRP_TIMER_CBF_FCT_PTR_TYPE)mrp_sys_cyclic_timer_cbf);
  timerIdx = pDBedd->TimerIdCyclic % MRP_MAX_NO_OF_TIMER_IDS_PER_INTERFACE;
  MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_edd_init[inst=%d]: timer_get called for TimerIdCyclic: id=%d, idx=%d",
    MRP_TIMER_INTERFACE, pDBedd->TimerIdCyclic, timerIdx);

  sysCnt = mrp_sys_get_edd_sys_channel_cnt();
  if (sysCnt == 1) /* if it's the first system channel, which is opened */
  {
    /* allocate the cyclic (global) MRP component timer, if the first system channel is just being opened (mrp_edd_init is called the first time) */
    id = mrp_sys_cyclic_timer_alloc (LSA_TIMER_TYPE_CYCLIC);
    
    /* start the cyclic (global) MRP component timer, if the first system channel is just being opened (mrp_edd_init is called the first time) */
    mrp_sys_cyclic_timer_start (id, (MRP_DEFAULT_VAL_CYCLIC_INTERVAL_MS / MRP_TIMER_TIME_BASE_DEZ));
  }

  /* start (means set state and init timer para) interface related timer representative of cyclic timer*/
  mrp_sys_timer_start (pDBedd->TimerIdCyclic, (MRP_DEFAULT_VAL_CYCLIC_INTERVAL_MS / MRP_TIMER_TIME_BASE_DEZ));

  numOfPorts = mrp_sys_get_num_of_ports (pChSys);

  MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBedd->pLinkData), (LSA_UINT16)(sizeof(MRP_LINK_TBL_TYPE) * numOfPorts));
  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBedd->pLinkData, LSA_NULL));  
  
  for (i = 0; i < numOfPorts; i++) 
  {
    MRP_MEMSET(&(pDBedd->pLinkData[i]), 0, sizeof(MRP_LINK_TBL_TYPE));
    mrp_edd_set_link_up_time (pChSys, (LSA_UINT16)(i + 1), MRP_VAL_LINK_UP_T_DEFAULT_CNT);
    
    pDBedd->pLinkData[i].timer_index_up   = mrp_sys_timer_get (pChSys, MRP_TIMER_POOL_INTERFACE, MRP_TIMER_INTERFACE, LSA_TIMER_TYPE_ONE_SHOT, (MRP_ENUM_TIMER_USE_T)(MRP_TIMER_ENUM_MRP_LINKUP_PORT1 + i), (MRP_TIMER_CBF_FCT_PTR_TYPE)mrp_edd_timer_cbf_link);
    timerIdx = pDBedd->pLinkData[i].timer_index_up % MRP_MAX_NO_OF_TIMER_IDS_PER_INTERFACE;
    portNum = i + 1;
    LSA_UNUSED_ARG(portNum);   /* hhr: to please lint in case traces are disabled. */
    MRP_PROGRAM_TRACE_04(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_edd_init[inst=%d]: timer_get called for timer_index_up[port=%d]: id=%d, idx=%d",
      MRP_TIMER_INTERFACE, portNum, pDBedd->pLinkData[i].timer_index_up, timerIdx); 
  }
  /* snd */
  MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBedd->ppPortMacAddr), (LSA_UINT16)(sizeof(pDBedd->ppPortMacAddr) * numOfPorts));
  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBedd->ppPortMacAddr, LSA_NULL));
  /* preset zero */
  MRP_MEMSET(pDBedd->ppPortMacAddr, 0, (LSA_UINT16)(sizeof(pDBedd->ppPortMacAddr) * numOfPorts));

  for (i = 0; i < numOfPorts; i++) 
  {
    MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBedd->ppPortMacAddr[i]), 6);
    MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBedd->ppPortMacAddr[i], LSA_NULL));
    /* preset zero */
    MRP_MEMSET(pDBedd->ppPortMacAddr[i], 0, 6);
  }
  pDBedd->PortID4MacAddr = 1;

  for (i = 0; i < MRP_MAX_USER_CHANNELS_PER_IF; i++)
  {
    pDBedd->pChUsr[i] = LSA_NULL;
  }
  pDBedd->pChPrivUsr = LSA_NULL;

  MRP_MEMCPY(&pDBedd->ll_edd_sysptr, &pChSys->sysptr, sizeof(pDBedd->ll_edd_sysptr));

  /* get the max. number of bursts -> the length of the burst table to be allocated */
  pDBedd->llBurstMaxCnt = 0;
  while (pTmpReqTbl[pDBedd->llBurstMaxCnt] != LSA_NULL)
  {
    pDBedd->llBurstMaxCnt++;
  }
  for (k = 0; k < (LSA_UINT16)MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_END - MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_START; k++)
  {
    MRP_EDD_UPPER_MULTICAST_PTR_TYPE pMcastParam;
    MRP_LOCAL_MEM_PTR_TYPE pMem = LSA_NULL; 

    MRP_ALLOC_LOCAL_MEM(&pMem, sizeof(MRP_EDD_RQB_MULTICAST_TYPE));
    MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pMem, LSA_NULL));

    pDBedd->pCurrentMCsetting[k] = (MRP_EDD_UPPER_MULTICAST_PTR_TYPE)pMem;
    
    MRP_MEMSET(pDBedd->pCurrentMCsetting[k], 0, sizeof(MRP_EDD_RQB_MULTICAST_TYPE));
    
    pMcastParam       = pDBedd->pCurrentMCsetting[k];
    pMcastParam->Mode = EDD_MULTICAST_FWD_DISABLE;
    
    switch (k) 
    {
    case (LSA_UINT16)MRP_LL_SREQ_RQB_ENUM_MULTICAST_WRK - MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_START: 
      MRP_MEMCPY(pMcastParam->MACAddr.MacAdr, (LSA_VOID_PTR_TYPE)MRP_PACKET_VAL_DST_ADR_NSM_WRK, sizeof(pMcastParam->MACAddr.MacAdr)); 
      break;
    case (LSA_UINT16)MRP_LL_SREQ_RQB_ENUM_MULTICAST_TST - MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_START: 
      MRP_MEMCPY(pMcastParam->MACAddr.MacAdr, (LSA_VOID_PTR_TYPE)MRP_PACKET_VAL_DST_ADR_NSM_TST, sizeof(pMcastParam->MACAddr.MacAdr)); 
      break;
    case (LSA_UINT16)MRP_LL_SREQ_RQB_ENUM_MULTICAST_HSYNC - MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_START:
      MRP_MEMCPY (pMcastParam->MACAddr.MacAdr, (LSA_VOID_PTR_TYPE)MRP_PACKET_VAL_DST_ADR_HA_VENDOR, sizeof (pMcastParam->MACAddr.MacAdr));
      break;

#ifdef MRP_CFG_PLUGIN_2_HSR
    case (LSA_UINT16)MRP_LL_SREQ_RQB_ENUM_MULTICAST_HSR_TPLG_CHG - MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_START:
      MRP_MEMCPY(pMcastParam->MACAddr.MacAdr, (LSA_VOID_PTR_TYPE)MRP_PACKET_DST_ADR_TPLG_CHG, sizeof(pMcastParam->MACAddr.MacAdr)); 
      break;

    case (LSA_UINT16)MRP_LL_SREQ_RQB_ENUM_MULTICAST_HSR_TST - MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_START:
      MRP_MEMCPY(pMcastParam->MACAddr.MacAdr, (LSA_VOID_PTR_TYPE)MRP_PACKET_DST_ADR_TST, sizeof(pMcastParam->MACAddr.MacAdr)); 
      break;

    case (LSA_UINT16)MRP_LL_SREQ_RQB_ENUM_MULTICAST_STBY_PARTNER - MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_START:
      MRP_MEMCPY(pMcastParam->MACAddr.MacAdr, (LSA_VOID_PTR_TYPE)MRP_PACKET_DST_ADR_TPLG_CHG_STBY_PARTNER, sizeof(pMcastParam->MACAddr.MacAdr)); 
      break;

    case (LSA_UINT16)MRP_LL_SREQ_RQB_ENUM_MULTICAST_STBY_COM - MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_START:
      MRP_MEMCPY(pMcastParam->MACAddr.MacAdr, (LSA_VOID_PTR_TYPE)MRP_PACKET_DST_ADR_STBY_COM, sizeof(pMcastParam->MACAddr.MacAdr)); 
      break;
#endif

    case (LSA_UINT16)MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_TST - MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_START:
      MRP_MEMCPY (pMcastParam->MACAddr.MacAdr, (LSA_VOID_PTR_TYPE)MRP_IN_PACKET_VAL_DST_ADR_TST, sizeof (pMcastParam->MACAddr.MacAdr));
      break;

    case (LSA_UINT16)MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_WRK - MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_START:
      MRP_MEMCPY (pMcastParam->MACAddr.MacAdr, (LSA_VOID_PTR_TYPE)MRP_IN_PACKET_VAL_DST_ADR_WRK, sizeof (pMcastParam->MACAddr.MacAdr));
      break;

    case (LSA_UINT16)MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_TRF - MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_START:
      MRP_MEMCPY (pMcastParam->MACAddr.MacAdr, (LSA_VOID_PTR_TYPE)MRP_IN_PACKET_VAL_DST_ADR_TRF, sizeof (pMcastParam->MACAddr.MacAdr));
      break;

    default: 
      MRP_ASSERT(0); 
      break;
    }
  }

  if (mrpMaxInstance)
  {
    MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBedd->pInstData), (LSA_UINT16)(sizeof(MRP_EDD_INST_DATA) * mrpMaxInstance));
    MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBedd->pInstData, LSA_NULL));

    for (i = 0; i < mrpMaxInstance; i++)
    {
      MRP_MEMSET(&(pDBedd->pInstData[i]), 0, sizeof(MRP_EDD_INST_DATA));
    }
  
    MRP_PROGRAM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_edd_init: pInstData allocated for %d instances", mrpMaxInstance);

    for (i = 0; i < mrpMaxInstance; i++)
    {
      pDBeddInst = &pDBedd->pInstData[i];
      MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBeddInst->pDBllBurst), (LSA_UINT16)(sizeof(MRP_LL_BURST_DB_T) * pDBedd->llBurstMaxCnt));
      MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBeddInst->pDBllBurst, LSA_NULL));  
      /* preset zero */
      MRP_MEMSET(pDBeddInst->pDBllBurst, 0, (LSA_UINT16)(sizeof(MRP_LL_BURST_DB_T) * pDBedd->llBurstMaxCnt));

      for (k = 0; k < pDBedd->llBurstMaxCnt; k++)
      {
        pDBeddInst->pDBllBurst[k].pReqTblBegin = pTmpReqTbl[k];
        pDBeddInst->pDBllBurst[k].pNext        = LSA_NULL;    
        pDBeddInst->pDBllBurst[k].queued       = LSA_FALSE;
        pDBeddInst->pDBllBurst[k].reset        = LSA_FALSE;
        pDBeddInst->pDBllBurst[k].pInstance[MRP_INSTANCE_INDEX]      = i;
        pDBeddInst->pDBllBurst[k].pInstance[MRP_IN_INSTANCE_INDEX]   = MRP_IN_INSTANCE_DEFAULT;
      }
      for (m = 0; m < MRP_LL_SND_RQB_ENUM_TBL_END; m++)
      {
        pDBeddInst->sendRqbInstData[m].sendRqbMaxCnt = tmpSendRqbMax[m];
        pDBeddInst->sendRqbInstData[m].sendRqbCnt    = 0;
        pDBeddInst->sendRqbInstData[m].sendRqbMissCnt = 0;
      }

      if(i == MRP_DEFAULT_INSTANCE)
      { /* Initialyze the interconnection sendRqb's */
        /* loop over the interconnection instances */
        for (k = 0; k < mrpInMaxInstance; k++)
        {
          /* loop over the interconnection table */
          for (m = 0; m < MRP_LL_SND_RQB_ENUM_IN_TYPES_TBL_END; m++)
          {
            pDBeddInst->sendRqbInInstData[k][m].sendRqbMaxCnt = tmpSendInRqbMax[m];
            pDBeddInst->sendRqbInInstData[k][m].sendRqbCnt    = 0;
            pDBeddInst->sendRqbInInstData[k][m].sendRqbMissCnt = 0;
          }
        }
      }
    
      pDBeddInst->RPort_1 = 0;
      pDBeddInst->RPort_2 = 0;
      pDBeddInst->TplgChgRunning   = LSA_FALSE;
      pDBeddInst->TplgChgFinishFct = 0;

      /* instance timer */
      pDBeddInst->TimerIdTplgChg = mrp_sys_timer_get (pChSys, MRP_TIMER_POOL_INSTANCE, i, LSA_TIMER_TYPE_ONE_SHOT, MRP_TIMER_ENUM_MRP_TPLG_CHG_EDD, (MRP_TIMER_CBF_FCT_PTR_TYPE)mrp_edd_timer_cbf_tplg_chg);
      timerIdx = pDBeddInst->TimerIdTplgChg % MRP_MAX_NO_OF_TIMER_IDS_PER_INTERFACE;
      MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_edd_init[inst=%d]: timer_get called for TimerIdTplgChg: id=%d, idx=%d",
        i, pDBeddInst->TimerIdTplgChg, timerIdx);
    } 
  }

  LSA_UNUSED_ARG(timerIdx);   /* hhr: to please lint in case traces are disabled. */

  /* edd rqb tbl */
  mrp_edd_init_rqb(pChSys, mrpMaxInstance, mrpInMaxInstance);
  
  /* tplgChg */
  pDBedd->activeMrmInstCnt   = 0;
}


/*===========================================================================
* FUNCTION : mrp_edd_deinit
*----------------------------------------------------------------------------
* PURPOSE  : uninitialize lower layer structures
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys           = channel reference for edd system channel
*            mrpMaxInstance   = maximum number of MRP instances to be uninitialized
*            mrpInMaxInstance = maximum number of MRP interconnection instances
*                               to be uninitialized
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_VOID mrp_edd_deinit (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance)
{
  LSA_UINT16  rc, sysCnt;
  LSA_UINT16 i, numOfPorts;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  MRP_PROGRAM_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_edd_deinit entered, mrpMaxInstance=%d, mrpInMaxInstance=%d", mrpMaxInstance, mrpInMaxInstance);
  
  /* free interface timers */
  mrp_sys_timer_free (pDBedd->TimerIdCyclic);

  pDBedd->activeMrmInstCnt   = 0;

  mrp_edd_deinit_rqb (pChSys, mrpMaxInstance, mrpInMaxInstance);

  pDBedd->pChSys = LSA_NULL;
  pDBedd->PortID4MacAddr = 0;

  numOfPorts = mrp_sys_get_num_of_ports (pChSys);
  for (i = 0; i < numOfPorts; i++) 
  {
    MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBedd->ppPortMacAddr[i]);
    MRP_FATAL1(rc == LSA_RET_OK);
  }

  MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBedd->ppPortMacAddr);
  MRP_FATAL1(rc == LSA_RET_OK);
  pDBedd->ppPortMacAddr = LSA_NULL;

  for (i = 0; i < numOfPorts; i++) 
  {
    /* free interface related linkup timers */
    mrp_sys_timer_free (pDBedd->pLinkData[i].timer_index_up);
  }

  MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBedd->pLinkData);
  MRP_FATAL1(rc == LSA_RET_OK);
  pDBedd->pLinkData = LSA_NULL;

  for (i = 0; i < mrpMaxInstance; i++)
  {
  /* deallocate lower layer burst table */
    MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBedd->pInstData[i].pDBllBurst);
    MRP_FATAL1(rc == LSA_RET_OK);
    
    /* free instance timers */
    mrp_sys_timer_free (pDBedd->pInstData[i].TimerIdTplgChg);
        
    MRP_MEMSET(&(pDBedd->pInstData[i]), 0, sizeof(MRP_EDD_INST_DATA));
  }
  pDBedd->llBurstMaxCnt = 0;
  for (i = 0; i < (LSA_UINT16)MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_END - MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_START; i++)
  {
    MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBedd->pCurrentMCsetting[i]);
    MRP_FATAL1(rc == LSA_RET_OK);  
  }

  if (mrpMaxInstance)
  {
    MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBedd->pInstData);
    MRP_FATAL1(rc == LSA_RET_OK);
  }
  pDBedd->pInstData = LSA_NULL;

  MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBedd);
  MRP_FATAL1(rc == LSA_RET_OK);
  pChSys->pChSysDB->pEdd = LSA_NULL;
  
  sysCnt = mrp_sys_get_edd_sys_channel_cnt();
  if (sysCnt == 1) /* if it's the last system channel, which is being closed */
  {
    mrp_sys_cyclic_timer_free (g_Mrp.cyclicTimer.uid.uvar16);
  }
}
