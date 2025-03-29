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
/*  F i l e               &F: mrp_sys.c                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  implements the system LSA interface.                                     */
/*                                                                           */
/*  Timer Statemachine:                                                      */
/*  ==================                                                       */
/*  Because of the asynchronous timer handling in mrp_timeout the timer      */
/*  events start/stop/restart cannot be directly mapped to lsa output        */
/*  macros.                                                                  */
/*  This statemachine uses an internal state to handle timer rqbs            */
/*  who are already in the message queue.                                    */
/*                                                                           */
/*  STOPPED       :  timer stopped                                           */
/*  RUNNING       :  timer runs                                              */
/*  STOP_PENDING  :  timer rqb queued - stop called                          */
/*                                                                           */
/*                                                                           */
/*  state handling of cyclic component timer:                                */
/*  ------------                                                             */
/*  | NOT_INIT |                                                             */
/*  ------------                                                             */
/*        | init                                                             */
/*       \/                                                                  */
/*  ------------                                                             */
/*  | STOPPED  | <------------------------------                             */
/*  ------------                               |                             */
/*        | mrp_sys_timer_start                |                             */
/*       \/                                    |                             */
/*  ------------       mrp_sys_timer_request   |                             */
/*  | RUNNING  | ----------------------------->|                             */
/*  ------------                               |                             */
/*        | mrp_sys_timer_stop                 |                             */
/*       \/                                    |                             */
/*  -----------------  mrp_sys_timer_request   |                             */
/*  | STOP_PENDING  | ------------------------>|                             */
/*  -----------------                                                        */
/*                                                                           */
/*  actions triggered at cyclic timer expiry:                                */
/*  -----------------------------------------                                */
/*  mrp_timeout (called from interrupt) -->(RQB (MBX)-->                     */
/*  --> mrp_request --> mrp_sys_timer_request -->                            */
/*  --> mrp_cyclic_timer_cbf --> timer CBF related to each oneshot timer     */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef MRP_MESSAGE
/*  2008-01-07  mk    multi nic awareness for timer.                         */
/*  2009-02-14  mk    stop timer before freeing it.                          */
/*  2009-03-19  ds    calculation of number of timers corrected              */
/*  2009-05-06  ds    some trace levels changed                              */
/*  2009-07-02  ds    include hierarchy changed                              */
/*  2009-10-15  ds    cleanups for LL services (fwdCtrl)                     */
/*  2009-10-22  ds    static timers not supported any longer, both MRP and   */
/*                    HSR use AUTO timers                                    */
/*  2010-02-16  ds    MRP based on 1 ms (HW) timer instead of 10 ms base     */
/*                    timer constants cleaned up, timer handling corrected   */
/*  2010-06-28  ds    MRP timer cleanups: don't dealloc timer structure, if  */
/*                    timer is in PENDING state, additional PENDING traces   */
/*                    function mrp_sys_timer_stop returns a LSA_BOOL, if the */
/*                    timer can be stopped or is in pending state            */
/*  2010-07-19  ds    new feature MRP MIB implemented according to           */
/*                    IEC 62439-2                                            */
/*  2010-12-21  ds    N-IF: channel handling redesigned                      */
/*  2011-01-14  ds    N-IF: rework of burst DB and user req map              */
/*  2011-07-21  ds    timer handling completely redesigned for cyclic        */
/*                    component timer concept                                */
/*  2012-04-04  ds    introduction of N MRP instances per interface          */
/*  2018-10-19  hh    introduction of N MRP interconnection instances        */
/*                                                                           */
/*                                                                           */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID   2
#define MRP_MODULE_ID      LTRC_ACT_MODUL_ID

#include "mrp_int.h"

MRP_FILE_SYSTEM_EXTENSION(MRP_MODULE_ID)

/*---------------------------------------------------------------------------*/
/*                                defines                                    */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                                 types                                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                            forward declarations                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                                globals                                    */
/*---------------------------------------------------------------------------*/

static  LSA_FATAL_ERROR_TYPE mrp_fatal_var; /* for some debuggers that have problems with the stack-trace */


/*===========================================================================
* FUNCTION : mrp_fatal1
*----------------------------------------------------------------------------
* PURPOSE  : notify a fatal-error
*----------------------------------------------------------------------------
* RETURNS  : the function does not return
*----------------------------------------------------------------------------
* INPUTS   : LSA_UINT16    mrp_module_id:     mrp module ID
*            LSA_INT       line:              line number
*            LSA_UINT32    ec_0:              error reason code
  OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/

static LSA_VOID
mrp_fatal1(
  LSA_UINT16 mrp_module_id,
  LSA_INT line,
  LSA_VOID* pFile,
  LSA_UINT16 fileNameLength,
  LSA_UINT32 ec_0
) {
  MRP_UPPER_TRACE_03(0, LSA_TRACE_LEVEL_FATAL
    , "module_id(%u) line(%u) ec_0(0x%x)"
    , mrp_module_id, line, ec_0
    );

  mrp_fatal_var.lsa_component_id = LSA_COMP_ID_MRP;
  mrp_fatal_var.module_id        = mrp_module_id;
  mrp_fatal_var.line             = (LSA_UINT16)line;

  mrp_fatal_var.error_code[0]    = ec_0;
  mrp_fatal_var.error_code[1]    = 0;
  mrp_fatal_var.error_code[2]    = 0;
  mrp_fatal_var.error_code[3]    = 0;

  mrp_fatal_var.error_data_length = fileNameLength;
  mrp_fatal_var.error_data_ptr    = pFile;

  MRP_FATAL_ERROR(sizeof(mrp_fatal_var), &mrp_fatal_var);
  /* not reached */
}


/*===========================================================================
* FUNCTION : mrp_fatal
*----------------------------------------------------------------------------
* PURPOSE  : notify a fatal-error
*----------------------------------------------------------------------------
* RETURNS  : the function does not return
*----------------------------------------------------------------------------
* INPUTS   : LSA_UINT16    mrp_module_id:     mrp module ID
*            LSA_INT       line:              line number
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/

LSA_VOID
mrp_fatal(
  LSA_UINT16 mrp_module_id,
  LSA_INT line,
  LSA_VOID* pFile,
  LSA_UINT16 fileNameLength
) 
{
  mrp_fatal1(mrp_module_id, line, pFile, fileNameLength, 0);
}


/*===========================================================================
* FUNCTION : mrp_init
*----------------------------------------------------------------------------
* PURPOSE  : initialize the module MRP, must be called once at startup
*----------------------------------------------------------------------------
* RETURNS  : LSA_UINT16 resp = status of execution
*----------------------------------------------------------------------------
* INPUTS   : 
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT16 mrp_init (LSA_VOID)
{
  static LSA_UINT8 init = MRP_INIT_NOT_COMPLETED;
  LSA_UINT16 resp;
  LSA_UINT16 i, k;
  const MRP_USR_RQ_MAPPING_TBL_TYPE  tmpUsrReqTbl[]=
  {
     { mrp_rec_prm_prepare               , MRP_OPC_PRM_PREPARE          , MRP_PATH_TYPE_USER } 
    ,{ mrp_rec_prm_write                 , MRP_OPC_PRM_WRITE            , MRP_PATH_TYPE_USER } 
    ,{ mrp_rec_prm_end                   , MRP_OPC_PRM_END              , MRP_PATH_TYPE_USER } 
    ,{ mrp_rec_prm_change_port           , MRP_OPC_PRM_CHANGE_PORT      , MRP_PATH_TYPE_USER } 
    ,{ mrp_edd_init_prm_commit           , MRP_OPC_PRM_COMMIT           , MRP_PATH_TYPE_USER } /* prm_commit is queued now!!! */
    ,{ mrp_rec_prm_read                  , MRP_OPC_PRM_READ             , MRP_PATH_TYPE_USER } 
    ,{ mrp_rec_prm_ind_provide_rqb       , MRP_OPC_PRM_INDICATION       , MRP_PATH_TYPE_USER } 
    ,{ mrp_oha_ringstate_ind_provide_rqb , MRP_OPC_RINGSTATE_INDICATION , MRP_PATH_TYPE_USER } 
    ,{ mrp_oha_get_status                , MRP_OPC_GET_STATUS           , MRP_PATH_TYPE_USER } 
    ,{ mrp_oha_set_status                , MRP_OPC_SET_STATUS           , MRP_PATH_TYPE_USER } 
    ,{ mrp_usr_state_ind_provide_rqb     , MRP_OPC_STATE_INDICATION     , MRP_PATH_TYPE_USER_2} 
    ,{ mrp_usr_red_state_ind_provide_rqb , MRP_OPC_RED_STATE_INDICATION , MRP_PATH_TYPE_USER_2}
    ,{ mrp_in_red_in_tc_request          , MRP_OPC_RED_IN_TC_REQUEST    , MRP_PATH_TYPE_USER_2}
    ,{ mrp_in_red_event_ind_provide_rqb, MRP_OPC_RED_IN_EVENT_INDICATION , MRP_PATH_TYPE_USER_2}
#ifdef MRP_CFG_PLUGIN_2_HSR
    ,{ mrp_usr_ind_hsr_provide           , MRP_OPC_IND_PROVIDE          , MRP_PATH_TYPE_USER_2}
#endif
    ,{ LSA_NULL                          , 0xFF                         , 0xFF} 
  };

  resp = MRP_RET_OK;

  MRP_SYSTEM_TRACE_00(0, LSA_TRACE_LEVEL_CHAT, "IN: mrp_init()");
  if (init == MRP_INIT_NOT_COMPLETED)
  {
    g_Mrp.init = MRP_INIT_NOT_COMPLETED;
    init = MRP_INIT_COMPLETED;
  }

  if (g_Mrp.init == MRP_INIT_NOT_COMPLETED)
  {
    MRP_MEMSET (&g_Mrp, 0, sizeof (MRP_DB_TYPE));

    /* get the max. number of different user requests -> the length of the user request mapping table to be allocated */
    g_Mrp.usrReqCnt = 0;
    while (tmpUsrReqTbl[g_Mrp.usrReqCnt].pUserReqCbf != 0)
    {
      g_Mrp.usrReqCnt++;
    }

    MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&g_Mrp.pUsrReqMap), (LSA_UINT16)(sizeof(MRP_USR_RQ_MAPPING_TBL_TYPE) * g_Mrp.usrReqCnt));

    if (!LSA_HOST_PTR_ARE_EQUAL(g_Mrp.pUsrReqMap, LSA_NULL))
    {
      MRP_MEMSET(g_Mrp.pUsrReqMap, 0, (LSA_UINT16)(sizeof(MRP_USR_RQ_MAPPING_TBL_TYPE) * g_Mrp.usrReqCnt));   
      
      for (i = 0; i < g_Mrp.usrReqCnt; i++)
      {
        g_Mrp.pUsrReqMap[i] = tmpUsrReqTbl[i];
      }

      for (k = 0; k < MRP_MAX_CHANNELS && resp == MRP_RET_OK; k++)
      {
        switch (k % MRP_MAX_CHANNELS_PER_IF)
        {
        case 0:
          g_Mrp.pChSys[k/MRP_MAX_CHANNELS_PER_IF]              = &g_Mrp.channels[k];
          g_Mrp.pChSys[k/MRP_MAX_CHANNELS_PER_IF]->myHandle    = (LSA_HANDLE_TYPE)k;
          g_Mrp.pChSys[k/MRP_MAX_CHANNELS_PER_IF]->state       = MRP_CH_STATE_FREE;
          break;

        case 1:
          g_Mrp.pChPrmUsr[k/MRP_MAX_CHANNELS_PER_IF]           = &g_Mrp.channels[k];
          g_Mrp.pChPrmUsr[k/MRP_MAX_CHANNELS_PER_IF]->myHandle = (LSA_HANDLE_TYPE)k;
          g_Mrp.pChPrmUsr[k/MRP_MAX_CHANNELS_PER_IF]->state    = MRP_CH_STATE_FREE;
          break;

        case 2:
          g_Mrp.pChUsr[k/MRP_MAX_CHANNELS_PER_IF]              = &g_Mrp.channels[k];
          g_Mrp.pChUsr[k/MRP_MAX_CHANNELS_PER_IF]->myHandle    = (LSA_HANDLE_TYPE)k;
          g_Mrp.pChUsr[k/MRP_MAX_CHANNELS_PER_IF]->state       = MRP_CH_STATE_FREE;
          break;

        case 3:
          g_Mrp.pChPrivUsr[k/MRP_MAX_CHANNELS_PER_IF]          = &g_Mrp.channels[k];
          g_Mrp.pChPrivUsr[k/MRP_MAX_CHANNELS_PER_IF]->myHandle= (LSA_HANDLE_TYPE)k;
          g_Mrp.pChPrivUsr[k/MRP_MAX_CHANNELS_PER_IF]->state   = MRP_CH_STATE_FREE;
          break;

        default:
          MRP_SYSTEM_TRACE_01(0, LSA_TRACE_LEVEL_ERROR, "mrp_init (): invalid channel type index=0x%x", (k % MRP_MAX_CHANNELS_PER_IF));
          resp = MRP_RSP_ERR_RESOURCE;
          break;
        }
      }

      if (resp == MRP_RET_OK)
        g_Mrp.init = MRP_INIT_COMPLETED;
    }
    else
      resp = MRP_RSP_ERR_RESOURCE;
  }
  else
  {
    resp = MRP_RSP_ERR_SEQUENCE;
  }

  MRP_SYSTEM_TRACE_01(0, LSA_TRACE_LEVEL_CHAT, "OUT: mrp_init(), resp=0x%x", resp);

  return resp;
}


/*===========================================================================
* FUNCTION : mrp_undo_init
*----------------------------------------------------------------------------
* PURPOSE  : undo initialization of MRP module
*----------------------------------------------------------------------------
* RETURNS  : LSA_UINT16 resp = status of execution
*----------------------------------------------------------------------------
* INPUTS   : 
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT16 mrp_undo_init (LSA_VOID)
{
  LSA_UINT16 resp;
  LSA_UINT16 i;

  resp = MRP_RET_OK;

  MRP_SYSTEM_TRACE_00(0, LSA_TRACE_LEVEL_CHAT, "IN: mrp_undo_init()");
  
  if (g_Mrp.init)
  {
    for (i = 0; i < MRP_MAX_CHANNELS; i++)
    {
      if (g_Mrp.channels[i].state != MRP_CH_STATE_FREE)
      {
        resp = MRP_RSP_ERR_SEQUENCE;
        break;
      }
    }

    if (resp == MRP_RET_OK)
    {
      MRP_FREE_LOCAL_MEM(&resp, (MRP_LOCAL_MEM_PTR_TYPE)g_Mrp.pUsrReqMap);
      MRP_FATAL1(resp == LSA_RET_OK);

      g_Mrp.init = MRP_INIT_NOT_COMPLETED;
    }
  }
  else
  {
    resp = MRP_RSP_ERR_SEQUENCE;
  }

  MRP_SYSTEM_TRACE_01(0, LSA_TRACE_LEVEL_CHAT, "OUT: mrp_undo_init(), resp=0x%x", resp);

  return resp;
}

/*===========================================================================
* FUNCTION : mrp_timeout
*----------------------------------------------------------------------------
* PURPOSE  : do timeout actions
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : timer_id = id of timer (UNUSED!!!)
*            user_id  = unique user specific timer ID
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_timeout (LSA_UINT16 timer_id, LSA_USER_ID_TYPE user_id)
{
  LSA_UINT16 id, idx;
  LSA_UINT16 i;
  MRP_CHANNEL_TYPE_COMMON pChSys;
  MRP_TIMER_DB_TYPE *pDBtimer = LSA_NULL;
  MRP_DB_TYPE *pMrpGlob = LSA_NULL;

  LSA_UNUSED_ARG(timer_id);

  id = user_id.uvar16;
  idx = id % MRP_MAX_NO_OF_TIMER_IDS_PER_INTERFACE;
  
  if (!idx)  /* MRP cyclic base timer */
  {
    pMrpGlob = &g_Mrp;
    for (i = 0; i < MRP_MAX_NO_OF_INTERFACES; i++)
    {
      if (pMrpGlob->pChSys[i]->state == MRP_CH_STATE_READY)
      {
        pChSys = pMrpGlob->pChSys[i];

        if (!LSA_HOST_PTR_ARE_EQUAL (pChSys->pChSysDB, LSA_NULL) && !LSA_HOST_PTR_ARE_EQUAL(pChSys->pChSysDB->pInterfaceTimer, LSA_NULL))
        {
          pDBtimer = (MRP_TIMER_DB_TYPE *)pChSys->pChSysDB->pInterfaceTimer;
          if (!LSA_HOST_PTR_ARE_EQUAL(pDBtimer, LSA_NULL) && !LSA_HOST_PTR_ARE_EQUAL(pDBtimer->pTimerPara, LSA_NULL))
          {
            if (pMrpGlob->cyclicTimer.state == MRP_TIMER_STS_T_RUNNING)
            {
              if (!i)
                pMrpGlob->cyclicTimer.currTicks += (MRP_DEFAULT_VAL_CYCLIC_INTERVAL_MS / MRP_TIMER_TIME_BASE_DEZ);

              if (pDBtimer->pTimerPara[idx].state == MRP_TIMER_STS_T_RUNNING)
              {
                if (!LSA_HOST_PTR_ARE_EQUAL(pDBtimer->pTimerPara[idx].pRQB, LSA_NULL))
                {
                  if (pDBtimer->pTimerPara[idx].tx != pDBtimer->pTimerPara[idx].rx)
                  {
  #if 0 /* mrp_timeout is called from ISR --> traces should be avoided therefore!!! */
                    MRP_SYSTEM_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_WARN, "--> mrp_timeout (cyclic) idx=0 rqb in use, pnioInterfaceID=%i", 
                      ((MRP_IF_DB_TYPE *)pChSys->pChSysDB->pInterfacePara)->pnioInterfaceID);          
  #endif
                  }
                  else
                  {
  #if 0
                    MRP_SYSTEM_TRACE_00(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_CHAT, "--> mrp_timeout (cyclic) idx=0");
  #endif
                    pDBtimer->pTimerPara[idx].tx++;

                    MRP_REQUEST_LOCAL(pDBtimer->pTimerPara[idx].pRQB);            
                  }            
                }
              }
            }        
          } 
        }
      }
    }
  }
  else
  {
    MRP_ASSERT (idx == 0);
  } 
}

/*===========================================================================
* FUNCTION : mrp_sys_get_sys_ch_handle
*----------------------------------------------------------------------------
* PURPOSE  : get channel handle by means of a channel reference
*----------------------------------------------------------------------------
* RETURNS  : channel reference
*----------------------------------------------------------------------------
* INPUTS   : handle = mrp internal channel handle (myHandle)
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_HANDLE_TYPE mrp_sys_get_sys_ch_handle (const MRP_CONST_CHANNEL_TYPE_COMMON pCh)
{
  LSA_UINT16 i;
  LSA_HANDLE_TYPE handle;
  MRP_CHANNEL_TYPE_SYSTEM tmpChPtr;
  MRP_IF_DB_TYPE *pDBif = (MRP_IF_DB_TYPE *)pCh->pChSysDB->pInterfacePara;
  MRP_IF_DB_TYPE *pDBifTmp;

  handle = MRP_INVALID_HANDLE;

  for (i = 0; i < MRP_MAX_NO_OF_INTERFACES; i++)
  {
    tmpChPtr = g_Mrp.pChSys[i];

    if (tmpChPtr->state != MRP_CH_STATE_FREE) 
    {
      pDBifTmp = (MRP_IF_DB_TYPE *)tmpChPtr->pChSysDB->pInterfacePara;
      if (pDBifTmp->pnioInterfaceID == pDBif->pnioInterfaceID)
      {
        handle = tmpChPtr->myHandle;
        break;
      }
    }
  }

  return handle;
}

/*===========================================================================
* FUNCTION : mrp_sys_get_pCh
*----------------------------------------------------------------------------
* PURPOSE  : get channel reference from internal database
*----------------------------------------------------------------------------
* RETURNS  : channel reference
*----------------------------------------------------------------------------
* INPUTS   : handle = mrp internal channel handle (myHandle)
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
MRP_CHANNEL_TYPE_COMMON mrp_sys_get_pCh (const LSA_HANDLE_TYPE handle)
{
  MRP_FATAL1(handle < MRP_MAX_CHANNELS);

  if (handle < MRP_MAX_CHANNELS && g_Mrp.channels[handle].state != MRP_CH_STATE_FREE)
  {
    return &g_Mrp.channels[handle];
  }

  return LSA_NULL;
}

/*===========================================================================
* FUNCTION : mrp_sys_get_edd_sys_channel_cnt
*----------------------------------------------------------------------------
* PURPOSE  : gives number of edd system channels
*----------------------------------------------------------------------------
* RETURNS  : cnt = EDD system channel count
*----------------------------------------------------------------------------
* INPUTS   : 
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT16 mrp_sys_get_edd_sys_channel_cnt (LSA_VOID)
{
  LSA_UINT16 cnt, i;

  for (i = 0, cnt = 0; i < MRP_MAX_NO_OF_INTERFACES; i++) 
  {
    if (g_Mrp.pChSys[i]->state != MRP_CH_STATE_FREE)
    {
      cnt++;
    }
  }

  return cnt;
}

/*===========================================================================
* FUNCTION : mrp_sys_get_mrp_mode
*----------------------------------------------------------------------------
* PURPOSE  : returns flag, if MRA 2.0 is activated
*----------------------------------------------------------------------------
* RETURNS  : LSA_TRUE: MRA 2.0 is activated
*            LSA_FALSE: MRA 2.0 is not activated, behave
*            like an MRA 1.0 MRP device
*----------------------------------------------------------------------------
* INPUTS   : pCh = channel reference for system or user channel
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_sys_get_mrp_mode (const MRP_CONST_CHANNEL_TYPE_COMMON pCh, LSA_UINT16 instance, MRP_MODE_DATA_TYPE *pData)
{
  MRP_MRC_PARAMS_TYPE paramsMRC;
#ifdef MRP_CFG_PLUGIN_0_MRM
  MRP_MRM_PARAMS_TYPE paramsMRM;
#endif
#ifdef MRP_CFG_PLUGIN_2_HSR
  LSA_UINT8 ardRunState;
  LSA_BOOL ardHsrClientByFallback;

  mrp_ard_get_state (pCh, &ardRunState, &ardHsrClientByFallback);
#endif

  pData->ardMode     = MRP_ARD_MODE_OFF;
  pData->mrpMode     = MRP_MODE_OFF;
  pData->mrpOperRole = MRP_OPER_ROLE_OFF;
  pData->rPort1      = 0;
  pData->rPort2      = 0;
  MRP_MEMSET (pData->DomainUUID, 0, sizeof (pData->DomainUUID));


#ifdef MRP_CFG_PLUGIN_2_HSR
  if (MRP_OPERATION_ON == ardRunState)
  {
    if (ardHsrClientByFallback)
    {
      pData->ardMode = MRP_ARD_MODE_HSRC_ON;
    }
    else
    {
      pData->ardMode = MRP_ARD_MODE_MRP_ON;
    }
  }
  else
  {
    pData->ardMode = MRP_ARD_MODE_OFF;
  }
#endif

#ifdef MRP_CFG_PLUGIN_0_MRM
  mrp_mrm_get_params (pCh, instance, &paramsMRM);

  if (MRP_OPERATION_ON == paramsMRM.Mode)
  {
    MRP_MEMCPY (pData->DomainUUID, paramsMRM.DomainUUID, sizeof (pData->DomainUUID));

    pData->mrpMode = MRP_MODE_MRM_ON;
    
    pData->mrpOperRole = paramsMRM.operRole;
    
    pData->rPort1 = paramsMRM.RPort_1;
    pData->rPort2 = paramsMRM.RPort_2;
  }
#endif
  /* mrc params */
  mrp_mrc_get_params (pCh, instance, &paramsMRC);

  if (MRP_OPERATION_ON == paramsMRC.Mode)
  {
    MRP_MEMCPY (pData->DomainUUID, paramsMRC.DomainUUID, sizeof (pData->DomainUUID));

    pData->mrpMode     = MRP_MODE_MRC_ON;

    pData->mrpOperRole = MRP_OPER_ROLE_CLIENT;

    pData->rPort1 = paramsMRC.RPort_1;
    pData->rPort2 = paramsMRC.RPort_2;
  }
  MRP_SYSTEM_TRACE_06 (pCh->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_sys_get_mrp_mode[inst=%d]: ardMode=%d, mrpMode=%d, mrpOperRole=%d, rport1=%d, rport2=%d", instance, pData->ardMode, pData->mrpMode, pData->mrpOperRole, pData->rPort1, pData->rPort2);
}

/*===========================================================================
* FUNCTION : mrp_sys_get_ed2_flag
*----------------------------------------------------------------------------
* PURPOSE  : returns flag, if MRA 2.0 is activated
*----------------------------------------------------------------------------
* RETURNS  : LSA_TRUE: MRA 2.0 is activated
*            LSA_FALSE: MRA 2.0 is not activated, behave
*            like an MRA 1.0 MRP device
*----------------------------------------------------------------------------
* INPUTS   : pCh = channel reference for system or user channel
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT8 mrp_sys_get_ed2_flag (const MRP_CONST_CHANNEL_TYPE_COMMON pCh)
{
  LSA_UINT8 mraEd2Flag;
  MRP_IF_DB_TYPE *pDBif = LSA_NULL;

  mraEd2Flag = 1;

  if (!LSA_HOST_PTR_ARE_EQUAL (pCh, LSA_NULL))
  {
    if (!LSA_HOST_PTR_ARE_EQUAL (pCh->pChSysDB, LSA_NULL))
    {
      pDBif = (MRP_IF_DB_TYPE *)(pCh->pChSysDB->pInterfacePara);

      if (!LSA_HOST_PTR_ARE_EQUAL (pDBif, LSA_NULL))
      {
        if (pDBif->mraEnableLegacyMode)
          mraEd2Flag = 0;
        else
          mraEd2Flag = 1;
      }
    }
  }

  return mraEd2Flag;
}


/*===========================================================================
* FUNCTION : mrp_sys_check_edd_details
*----------------------------------------------------------------------------
* PURPOSE  : check new channel details, if open of new channel might be granted
*----------------------------------------------------------------------------
* RETURNS  : openRsp      = success indicator
*----------------------------------------------------------------------------
* INPUTS   : tmpDetailPtr = ptr to mrp details of new channel 
* OUTPUTS  : *newEntry    = ptr to free entry index
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT16 mrp_sys_check_edd_details (const MRP_CONST_DETAIL_PTR_TYPE tmpDetailPtr, LSA_UINT16 *newEntry)
{
  LSA_UINT16 openRsp, i;

  openRsp = MRP_RSP_OK;
  *newEntry = 0;

  for (i = 0; i < MRP_MAX_NO_OF_INTERFACES; i++) 
  {
    if (g_Mrp.pChSys[i]->state            == MRP_CH_STATE_READY &&
        g_Mrp.pChSys[i]->detailptr->NicId == tmpDetailPtr->NicId)
    {
      break;
    }
  }

  if (tmpDetailPtr->PathType == MRP_PATH_TYPE_EDD)
  {
    if (i < MRP_MAX_NO_OF_INTERFACES)
    {
      MRP_SYSTEM_TRACE_01(0, LSA_TRACE_LEVEL_ERROR, ">> mrp_sys_check_edd_details: EDD channel for nicID=%d already established", tmpDetailPtr->NicId);
      openRsp = MRP_RSP_ERR_SYS_PATH;
    }
    else
    {
      /* get the index of a free entry */
      for (i = 0; i < MRP_MAX_NO_OF_INTERFACES; i++) 
      {
        if (g_Mrp.pChSys[i]->state == MRP_CH_STATE_FREE)
        {
          break;
        }
      }
      if (i < MRP_MAX_NO_OF_INTERFACES)
        *newEntry = i;
      else
        openRsp = MRP_RSP_ERR_RESOURCE;
    }
  }
  else /* if path_type == MRP_PATH_TYPE_USER or MRP_PATH_TYPE_USER_2 */
  {
    if (i < MRP_MAX_NO_OF_INTERFACES)
    {
      *newEntry = i;
    }
    else
    {
      /* no system channel found for this USER channel */
      MRP_SYSTEM_TRACE_01(0, LSA_TRACE_LEVEL_ERROR, ">> mrp_sys_check_edd_details: no EDD channel for nicID=%d established", tmpDetailPtr->NicId);
      openRsp = MRP_RSP_ERR_SEQUENCE;
    }
  }

  return openRsp;
}

/*===========================================================================
* FUNCTION : mrp_sys_get_sys_ch_by_if_id
*----------------------------------------------------------------------------
* PURPOSE  : maps a given interfaceId to the corresponding system channel
*----------------------------------------------------------------------------
* RETURNS  : pChSys = pointer to the corresponding system channel
*----------------------------------------------------------------------------
* INPUTS   : pnioInterfaceID = EDDx interfaceId of the interface
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
MRP_CHANNEL_TYPE_SYSTEM mrp_sys_get_sys_ch_by_if_id (const LSA_UINT32 pnioInterfaceID)
{
  MRP_CHANNEL_TYPE_SYSTEM pChSys, tmpSysChPtr;
  MRP_IF_DB_TYPE *pDBif;
  LSA_UINT16 i;

  pChSys      = LSA_NULL;
  tmpSysChPtr = LSA_NULL;

  for (i = 0; i < MRP_MAX_NO_OF_INTERFACES; i++)
  {
    tmpSysChPtr = g_Mrp.pChSys[i];

    if (tmpSysChPtr->state != MRP_CH_STATE_FREE) 
    {
      pDBif = (MRP_IF_DB_TYPE *)tmpSysChPtr->pChSysDB->pInterfacePara;
      if (pDBif->pnioInterfaceID == pnioInterfaceID)
      {
        pChSys = tmpSysChPtr;
        break;
      }
    }
  }

  return pChSys;
}

/*===========================================================================
* FUNCTION : mrp_sys_map_timer_id_to_if_id
*----------------------------------------------------------------------------
* PURPOSE  : maps the user_id of a timer to the corresponding pnioInterfaceID
*----------------------------------------------------------------------------
* RETURNS  : pnioInterfaceID = EDDx interfaceID - 1
*----------------------------------------------------------------------------
* INPUTS   : userId = userId of the expired timer
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT32 mrp_sys_map_timer_id_to_if_id (const LSA_UINT16 userId)
{
  LSA_UINT32 pnioInterfaceId;

  pnioInterfaceId = userId/MRP_MAX_NO_OF_TIMER_IDS_PER_INTERFACE;

  return pnioInterfaceId;
}

/*===========================================================================
* FUNCTION : mrp_sys_cyclic_get_curr_time
*----------------------------------------------------------------------------
* PURPOSE  : get the current timestamp of the component's cyclic timer
*----------------------------------------------------------------------------
* RETURNS  : currTicks = current time ticks
*----------------------------------------------------------------------------
* INPUTS   : 
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT32 mrp_sys_cyclic_get_curr_time (void)
{
  return g_Mrp.cyclicTimer.currTicks;
}

/*===========================================================================
* FUNCTION : mrp_sys_timer_get_elapsed_ticks
*----------------------------------------------------------------------------
* PURPOSE  : get the number of timeTicks (in ms) already elapsed for one timer
*----------------------------------------------------------------------------
* RETURNS  : LSA_UINT32:  timeTicks elapsed
*----------------------------------------------------------------------------
* INPUTS   : id = timerId of the timer
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT32 mrp_sys_timer_get_elapsed_ticks (const LSA_UINT16 id)
{
  LSA_UINT32 currTicks;
  LSA_UINT32 deltaTicks;
  LSA_UINT16 idx;
  LSA_UINT32 pnioInterfaceID;
  MRP_CHANNEL_TYPE_COMMON pChSys;
  MRP_TIMER_DB_TYPE *pDBtimer;

  idx = id % MRP_MAX_NO_OF_TIMER_IDS_PER_INTERFACE;

  pnioInterfaceID = mrp_sys_map_timer_id_to_if_id (id);
  pChSys          = mrp_sys_get_sys_ch_by_if_id (pnioInterfaceID);
  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pChSys, LSA_NULL));

  pDBtimer = (MRP_TIMER_DB_TYPE *)pChSys->pChSysDB->pInterfaceTimer;
  MRP_FATAL1(pChSys == pDBtimer->pCh);

  MRP_ASSERT(idx < pDBtimer->timerTblSize);

  currTicks = mrp_sys_cyclic_get_curr_time ();

  if (currTicks >= pDBtimer->pTimerPara[idx].startTicks)
  {
    deltaTicks = currTicks - pDBtimer->pTimerPara[idx].startTicks;
  }
  else /* currTicks < pTimerPara->startTicks --> timer overrun*/
  {
    /* deltaTicks = 0xFFFFFFFFUL - (pTimerPara->startTicks - currTicks) + 1 */
    deltaTicks = (0xFFFFFFFFUL - pDBtimer->pTimerPara[idx].startTicks) + currTicks + 1;
  }

  return deltaTicks;
}

/*===========================================================================
* FUNCTION : mrp_sys_oneshot_time_expired
*----------------------------------------------------------------------------
* PURPOSE  : check, if the timeout of a oneshot timer is already reached
*----------------------------------------------------------------------------
* RETURNS  : LSA_TRUE:  timeout reached
*            LSA_FALSE: timeout not reached
*----------------------------------------------------------------------------
* INPUTS   : *pTimerPara = reference of the timer to be checked for timeout
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_UINT8 mrp_sys_oneshot_time_expired (const MRP_TIMER_PARA_DB_TYPE *pTimerPara)
{
  LSA_UINT8 isExpired;
  LSA_UINT32 currTicks;
  LSA_UINT32 deltaTicks;
  
  isExpired = LSA_FALSE;
  currTicks = mrp_sys_cyclic_get_curr_time ();
  
  if (currTicks >= pTimerPara->startTicks)
  {
    deltaTicks = currTicks - pTimerPara->startTicks;
  }
  else /* currTicks < pTimerPara->startTicks --> timer overrun*/
  {
    /* deltaTicks = 0xFFFFFFFFUL - (pTimerPara->startTicks - currTicks) + 1 */
    deltaTicks = (0xFFFFFFFFUL - pTimerPara->startTicks) + currTicks + 1;
  }
  
  if (deltaTicks  >= pTimerPara->durationTicks)
  {
    isExpired = LSA_TRUE;
  }
  
  return isExpired;
}

/*===========================================================================
* FUNCTION : mrp_sys_cyclic_timer_cbf
*----------------------------------------------------------------------------
* PURPOSE  : do cyclic timer timeout actions in "mrp"-task and interface context
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for system or user channel
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            uid      = user identification (here: timer_id is stored)
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_sys_cyclic_timer_cbf (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, LSA_USER_ID_TYPE uid)
{
  LSA_UINT16 id, idx;
  MRP_TIMER_DB_TYPE *pDBtimer;

  id  = uid.uvar16;
  idx = id % MRP_MAX_NO_OF_TIMER_IDS_PER_INTERFACE;

  MRP_ASSERT(pInstance[MRP_INSTANCE_INDEX] == MRP_TIMER_INTERFACE);
  LSA_UNUSED_ARG (*pInstance);

  MRP_ASSERT (idx == 0);
  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pChSys, LSA_NULL));

  pDBtimer        = (MRP_TIMER_DB_TYPE *)pChSys->pChSysDB->pInterfaceTimer;
  MRP_FATAL1(pDBtimer->pCh == pChSys);

  if (pDBtimer->pTimerPara[idx].state == MRP_TIMER_STS_T_RUNNING) 
  {
    /* handle cyclic timeout actions for every timer of one instance */
    MRP_SYSTEM_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_CHAT, "--> mrp_sys_cyclic_timer_cbf: idx=%i timeout processed", idx);
    
    for (idx = 1; idx < pDBtimer->timerTblSize; idx++)
    {
      if (pDBtimer->pTimerPara[idx].state == MRP_TIMER_STS_T_RUNNING)
      {
        if (mrp_sys_oneshot_time_expired (&(pDBtimer->pTimerPara[idx])))
        {
          /* stop timer, which is expired */
          pDBtimer->pTimerPara[idx].state = MRP_TIMER_STS_T_STOPPED;
          /* trigger timer callback */
          pDBtimer->pTimerPara[idx].pTimerCbf(pChSys, pDBtimer->pTimerPara[idx].instance, pDBtimer->pTimerPara[idx].uid);
        } 
      }
    }
  }
  else 
  {
    MRP_SYSTEM_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_WARN, "--> mrp_sys_cyclic_timer_cbf: idx=%i dumped", idx);
    pDBtimer->pTimerPara[idx].state = MRP_TIMER_STS_T_STOPPED;
  }
}


/*===========================================================================
* FUNCTION : mrp_sys_timer_request
*----------------------------------------------------------------------------
* PURPOSE  : do timeout actions in "mrp"-task context
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pCh    = channel reference for system or user channel
*            pRQB   = upper rqb reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_sys_timer_request (const MRP_CONST_CHANNEL_TYPE_COMMON pCh, const MRP_CONST_UPPER_RQB_PTR_TYPE pRQB)
{
  LSA_UINT16 id, idx;
  MRP_TIMER_DB_TYPE *pDBtimer;
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pCh->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  id  = MRP_RQB_GET_USER_ID_UVAR16(pRQB);
  idx = id % MRP_MAX_NO_OF_TIMER_IDS_PER_INTERFACE;

  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pCh, LSA_NULL));

  pDBtimer        = (MRP_TIMER_DB_TYPE *)pCh->pChSysDB->pInterfaceTimer;
  MRP_FATAL1(pDBtimer->pCh == pCh);
  MRP_FATAL1(idx < pDBtimer->timerTblSize);

  /* only the interface representative timer is relevant here (idx = 0)! */
  if (!idx)
  {
    pDBtimer->pTimerPara[idx].rx++;
    
    /* should only happen, if function is interrupted by timer interrupt after rx++ --> tx != rx */
    if (pDBtimer->pTimerPara[idx].rx != pDBtimer->pTimerPara[idx].tx)
    {
      MRP_SYSTEM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_WARN, "--> mrp_sys_timer_request: idx=%i, tx(%d) != rx(%d)", idx, pDBtimer->pTimerPara[idx].tx, pDBtimer->pTimerPara[idx].rx);
    }
    
    switch (pDBtimer->pTimerPara[idx].state)
    {
    case MRP_TIMER_STS_T_RUNNING:
      MRP_SYSTEM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_CHAT, "--> mrp_sys_timer_request idx=%i", idx);

      /* call cyclic timer cbf for one interface (mrp_sys_cyclic_timer_cbf) */
      pDBtimer->pTimerPara[idx].pTimerCbf(pCh, pDBtimer->pTimerPara[idx].instance, pDBtimer->pTimerPara[idx].uid);
      break;
      
    case MRP_TIMER_STS_T_STOP_PENDING:
      MRP_SYSTEM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_NOTE, "--> mrp_sys_timer_request: idx=%i STOP_PENDING-->STOPPED", idx);
      pDBtimer->pTimerPara[idx].state = MRP_TIMER_STS_T_STOPPED;
      break;
      
    case MRP_TIMER_STS_T_STOPPED:
      MRP_SYSTEM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_WARN, "--> mrp_sys_timer_request: idx=%i already stopped", idx);
      break;
      
    case MRP_TIMER_STS_T_NOT_INIT:
      /*fall through*/
    default:
      MRP_SYSTEM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_ERROR, "--> mrp_sys_timer_request: idx=%i unexpected state=%i", idx, pDBtimer->pTimerPara[idx].state);
      break;
    }
  }
  else
  {
    MRP_ASSERT (idx == 0);
  }
}

/*===========================================================================
* FUNCTION : mrp_sys_cylic_timer_alloc
*----------------------------------------------------------------------------
* PURPOSE  : allocate a cyclic component timer
*----------------------------------------------------------------------------
* RETURNS  : id     = unique timer ID
*----------------------------------------------------------------------------
* INPUTS   : 
*            type   = LSA_TIMER_TYPE_CYCLIC
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : only one shot timer used!!!
*==========================================================================*/
LSA_UINT16 mrp_sys_cyclic_timer_alloc (LSA_UINT16 type)
{
  LSA_UINT16  rc, id;
  MRP_BASE_TIMER_PARA_DB_TYPE *pCyclicTimer;

  MRP_ASSERT (type == LSA_TIMER_TYPE_CYCLIC);
  id = 0;
  
  pCyclicTimer               = &g_Mrp.cyclicTimer;  
  pCyclicTimer->timerId      = 0;
  
  MRP_ASSERT (pCyclicTimer->state == MRP_TIMER_STS_T_NOT_INIT);
  
  MRP_ALLOC_TIMER(&rc, &pCyclicTimer->timerId, type, MRP_TIMER_TIME_BASE);
  MRP_FATAL1(rc == LSA_RET_OK);  
  
  pCyclicTimer->currTicks    = 0;
  pCyclicTimer->state        = MRP_TIMER_STS_T_STOPPED;
  pCyclicTimer->type         = LSA_TIMER_TYPE_CYCLIC;
  pCyclicTimer->uid.uvar16   = 0;
  
  MRP_SYSTEM_TRACE_00(0, LSA_TRACE_LEVEL_NOTE, "--> mrp_sys_cyclic_timer_alloc: id = 0");

  return id;
}

/*===========================================================================
* FUNCTION : mrp_sys_timer_get
*----------------------------------------------------------------------------
* PURPOSE  : get a timer and initialize timer data 
*            (idx = 0: cyclic timer, idx > 0: oneshot timer)
*----------------------------------------------------------------------------
* RETURNS  : id     = unique timer ID, calculated by idx and pnioInterfaceID
*----------------------------------------------------------------------------
* INPUTS   : pCh       = channel reference for system or user channel
*            timerPool = get the timer from interface related timer pool or
*                        from MRP instance related timer pool
*            instance  = MRP (HSR) or MRP interconnection instance 
*                        resp. MRP_TIMER_INTERFACE
*            type      = LSA_TIMER_TYPE_CYCLIC, LSA_TIMER_TYPE_ONE_SHOT 
*            purpose   = purpose of the timer (usage)
*            pCbf      = callback function, called when timeout occurs
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : only oneshot timer used!!!
*==========================================================================*/
/*lint -e{550,438}*/
/*numOfPorts is used to get numInterfaceTimer*/
LSA_UINT16 mrp_sys_timer_get (const MRP_CONST_CHANNEL_TYPE_COMMON pCh, const MRP_TIMER_POOL timerPool, LSA_UINT16 instance, LSA_UINT16 type, MRP_ENUM_TIMER_USE_T purpose, MRP_TIMER_CBF_FCT_PTR_TYPE pCbf)
{
  LSA_HANDLE_TYPE handle;
  LSA_UINT16  id, idx, numOfPorts, numInterfaceTimer;

  MRP_IF_DB_TYPE    *pDBif    = (MRP_IF_DB_TYPE *)pCh->pChSysDB->pInterfacePara;
  MRP_TIMER_DB_TYPE *pDBtimer = (MRP_TIMER_DB_TYPE *)pCh->pChSysDB->pInterfaceTimer;

  idx        = 0;
  numOfPorts = mrp_sys_get_num_of_ports(pCh);
  numInterfaceTimer = (LSA_UINT16)MRP_GET_IF_TIMER_NUM(numOfPorts);
  
  switch(timerPool)
  {
  case MRP_TIMER_POOL_INTERFACE:
    if (type == LSA_TIMER_TYPE_CYCLIC)
    {
      idx = 0;
    }
    else
    {
      for (idx = 1; idx < numInterfaceTimer; idx++) 
      {
        if (pDBtimer->pTimerPara[idx].state == MRP_TIMER_STS_T_NOT_INIT)
        {
          break;
        }
      }  
    }  
    MRP_FATAL1(idx < numInterfaceTimer);
    pDBtimer->pTimerPara[idx].instance[MRP_INSTANCE_INDEX] = instance;
    pDBtimer->pTimerPara[idx].instance[MRP_IN_INSTANCE_INDEX] = MRP_IN_INSTANCE_NONE;
    break;
    
  case MRP_TIMER_POOL_INSTANCE:
  case MRP_TIMER_POOL_IN_INSTANCE:
    for (idx = numInterfaceTimer; idx < pDBtimer->timerTblSize; idx++) 
    {
      if (pDBtimer->pTimerPara[idx].state == MRP_TIMER_STS_T_NOT_INIT)
      {
        break;
      }  
    }  
    MRP_FATAL1(idx < pDBtimer->timerTblSize);
    if (timerPool == MRP_TIMER_POOL_INSTANCE)
    {
      pDBtimer->pTimerPara[idx].instance[MRP_INSTANCE_INDEX]  = instance;
      pDBtimer->pTimerPara[idx].instance[MRP_IN_INSTANCE_INDEX]  = MRP_IN_INSTANCE_NONE;

    }
    else /* MRP interconnection timer pool */
    {
      pDBtimer->pTimerPara[idx].instance[MRP_INSTANCE_INDEX]   = MRP_DEFAULT_INSTANCE;
      pDBtimer->pTimerPara[idx].instance[MRP_IN_INSTANCE_INDEX]   = instance;
    }
    break;
    
  default:
    MRP_FATAL;
    break;
  } 
   

  MRP_ASSERT(pDBtimer->pTimerPara[idx].state == MRP_TIMER_STS_T_NOT_INIT);
  
  /* the timer ID MUST be unique: therefore it is calculated by means of idx and pnioInterfaceID */
  id = idx + ((LSA_UINT16)(pDBif->pnioInterfaceID * MRP_MAX_NO_OF_TIMER_IDS_PER_INTERFACE));
  
  MRP_SYSTEM_TRACE_04(pCh->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "--> mrp_sys_timer_get[inst=%d]: id=%d, idx=%d, pool=%d", instance, id, idx, timerPool);
  
  if (type == LSA_TIMER_TYPE_CYCLIC)
  {
    MRP_ALLOC_UPPER_RQB_LOCAL(&(pDBtimer->pTimerPara[idx].pRQB), sizeof(MRP_RQB_TYPE));
    MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pDBtimer->pTimerPara[idx].pRQB, LSA_NULL));

    MRP_RQB_SET_OPCODE(pDBtimer->pTimerPara[idx].pRQB, MRP_OPC_INTERNAL_TIMER);
    
    handle = mrp_sys_get_sys_ch_handle(pCh);
    MRP_RQB_SET_HANDLE(pDBtimer->pTimerPara[idx].pRQB, handle);
    MRP_RQB_SET_USER_ID_UVAR16(pDBtimer->pTimerPara[idx].pRQB, id);
  }
  
  pDBtimer->pTimerPara[idx].state           = MRP_TIMER_STS_T_STOPPED;

  pDBtimer->pTimerPara[idx].durationTicks   = 0xFFFFFFFFUL;
  pDBtimer->pTimerPara[idx].type            = type;
  pDBtimer->pTimerPara[idx].purpose         = purpose;
  pDBtimer->pTimerPara[idx].pTimerCbf       = pCbf;
  pDBtimer->pTimerPara[idx].tx              = 0;
  pDBtimer->pTimerPara[idx].rx              = 0;
  pDBtimer->pTimerPara[idx].uid.uvar16      = id;

  return id;
}

/*===========================================================================
* FUNCTION : mrp_sys_cyclic_timer_free
*----------------------------------------------------------------------------
* PURPOSE  : free a cyclic timer
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : id = unique timer id
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_sys_cyclic_timer_free (LSA_UINT16 id)
{
  LSA_UINT16 rc;
  MRP_BASE_TIMER_PARA_DB_TYPE *pCyclicTimer;

  MRP_ASSERT (id == 0);
  LSA_UNUSED_ARG (id);

  pCyclicTimer             = &g_Mrp.cyclicTimer;
  
  MRP_ASSERT (pCyclicTimer->state == MRP_TIMER_STS_T_STOPPED);
  
  MRP_FREE_TIMER(&rc, pCyclicTimer->timerId);

  MRP_FATAL1(rc == LSA_RET_OK);
  
  pCyclicTimer->state      = MRP_TIMER_STS_T_NOT_INIT;
  pCyclicTimer->timerId    = 0;
  pCyclicTimer->uid.uvar16 = 0;
  pCyclicTimer->type       = 0;

  MRP_SYSTEM_TRACE_00(0, LSA_TRACE_LEVEL_NOTE, "--> mrp_sys_cyclic_timer_free: id=0 complete");
}

  
/*===========================================================================
* FUNCTION : mrp_sys_timer_free
*----------------------------------------------------------------------------
* PURPOSE  : mark a oneshot (idx > 0) or cyclic (idx == 0) timer as free
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : id = unique timer id
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_sys_timer_free (LSA_UINT16 id)
{
  LSA_UINT16 rc, idx;
  LSA_UINT32 pnioInterfaceID;
  MRP_CHANNEL_TYPE_COMMON pChSys;
  MRP_TIMER_DB_TYPE *pDBtimer;

  rc = LSA_RET_OK;
  idx = id % MRP_MAX_NO_OF_TIMER_IDS_PER_INTERFACE;
    
  mrp_sys_timer_stop (id);
  
  pnioInterfaceID = mrp_sys_map_timer_id_to_if_id (id);
  pChSys          = mrp_sys_get_sys_ch_by_if_id (pnioInterfaceID);
  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pChSys, LSA_NULL));

  pDBtimer = (MRP_TIMER_DB_TYPE *)pChSys->pChSysDB->pInterfaceTimer;
  MRP_FATAL1(pChSys == pDBtimer->pCh);
    
  MRP_SYSTEM_TRACE_03(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "--> mrp_sys_timer_free[inst=%d]: id=%d, idx=%d", pDBtimer->pTimerPara[idx].instance[0], id, idx);
  pDBtimer->pTimerPara[idx].state      = MRP_TIMER_STS_T_NOT_INIT;
  pDBtimer->pTimerPara[idx].uid.uvar16 = 0;
  pDBtimer->pTimerPara[idx].type       = 0;
  pDBtimer->pTimerPara[idx].instance[0] = 0;
  pDBtimer->pTimerPara[idx].instance[1] = 0;
  pDBtimer->pTimerPara[idx].pTimerCbf  = 0;

  if (idx == 0)
  {
    MRP_FREE_UPPER_RQB_LOCAL(&rc, pDBtimer->pTimerPara[idx].pRQB);
    MRP_FATAL1(rc == LSA_RET_OK);
    pDBtimer->pTimerPara[idx].pRQB = LSA_NULL;  
  }

  MRP_SYSTEM_TRACE_00(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "--> mrp_sys_timer_free complete");
}


/*===========================================================================
* FUNCTION : mrp_sys_cyclic_timer_start
*----------------------------------------------------------------------------
* PURPOSE  : start cyclic timer
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : id            = unique timer ID
*            durationTicks = timer run time factor(time divided by MRP_TIMER_TIME_BASE)
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_sys_cyclic_timer_start (LSA_UINT16 id, LSA_UINT32 durationTicks) 
{
  MRP_BASE_TIMER_PARA_DB_TYPE *pCyclicTimer;

  MRP_ASSERT (id == 0);
  LSA_UNUSED_ARG (id);

  pCyclicTimer = &g_Mrp.cyclicTimer;

  switch (pCyclicTimer->state) 
  {
    /*==========================================================================*/
  case MRP_TIMER_STS_T_STOPPED:
    {
      LSA_UINT16  rc;
      LSA_UINT32 duration;
      duration = durationTicks * MRP_TIMER_TIME_BASE_DEZ;
      MRP_SYSTEM_TRACE_01(0, LSA_TRACE_LEVEL_NOTE, "--> mrp_sys_cyclic_timer_start id=0, time=%i ms", duration);

      LSA_UNUSED_ARG(duration);   /* hhr: to please the compiler in case traces are disabled. */

      pCyclicTimer->state = MRP_TIMER_STS_T_RUNNING;
      pCyclicTimer->currTicks = 0;

      MRP_START_TIMER(&rc, pCyclicTimer->timerId, pCyclicTimer->uid, (LSA_UINT16)durationTicks);

      if (rc != LSA_RET_OK) 
      {
        MRP_SYSTEM_TRACE_02(0, LSA_TRACE_LEVEL_FATAL, "--> mrp_sys_cyclic_timer_start id=0 rc=0x%x timerId=%i", rc, pCyclicTimer->timerId);
      }
      MRP_FATAL1(rc == LSA_RET_OK);
    }
    break;
    /*==========================================================================*/

  case MRP_TIMER_STS_T_NOT_INIT:
  case MRP_TIMER_STS_T_STOP_PENDING:
  case MRP_TIMER_STS_T_RUNNING:
    /*fall through*/
  default:
    MRP_SYSTEM_TRACE_01(0, LSA_TRACE_LEVEL_FATAL,"--> mrp_sys_cyclic_timer_start id=0 unknown state=%i", pCyclicTimer->state);
    MRP_ASSERT(LSA_FALSE);
    break;
  }
}
  

/*===========================================================================
* FUNCTION : mrp_sys_timer_start
*----------------------------------------------------------------------------
* PURPOSE  : start timer
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : id            = unique timer ID
*            durationTicks = timer run time factor(time divided by MRP_TIMER_TIME_BASE)
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_sys_timer_start (LSA_UINT16 id, LSA_UINT32 durationTicks) 
{
  LSA_UINT32 pnioInterfaceID, duration;
  LSA_UINT16 idx;
  MRP_CHANNEL_TYPE_COMMON pChSys;
  MRP_TIMER_STS_T timerState;
  MRP_TIMER_DB_TYPE *pDBtimer;
  LSA_UINT32 traceIdx;

  idx = id % MRP_MAX_NO_OF_TIMER_IDS_PER_INTERFACE;

  pnioInterfaceID = mrp_sys_map_timer_id_to_if_id (id);
  pChSys          = mrp_sys_get_sys_ch_by_if_id (pnioInterfaceID);
  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pChSys, LSA_NULL));

  traceIdx = pChSys->pChSysDB->traceIdx;   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */
  LSA_UNUSED_ARG(traceIdx);   /* hhr: to please the compiler in case traces are disabled. */

  pDBtimer = (MRP_TIMER_DB_TYPE *)pChSys->pChSysDB->pInterfaceTimer;
  MRP_FATAL1(pChSys == pDBtimer->pCh);

  duration = durationTicks * MRP_TIMER_TIME_BASE_DEZ;
  LSA_UNUSED_ARG(duration);   /* hhr: to please the compiler in case traces are disabled. */

  timerState = mrp_sys_cyclic_timer_get_state (0);
  if (timerState != MRP_TIMER_STS_T_RUNNING)
  {
    MRP_SYSTEM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "--> mrp_sys_timer_start called for id=%d, idx=%d, time=%i ms, while CYCLIC COMP. timer STOPPED!", id, idx, duration);
  }

  switch (pDBtimer->pTimerPara[idx].state) 
  {
  /*==========================================================================*/
  case MRP_TIMER_STS_T_STOPPED:
    {
      if (pDBtimer->pTimerPara[idx].tx == pDBtimer->pTimerPara[idx].rx) 
      {
        MRP_SYSTEM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_CHAT, "--> mrp_sys_timer_start id=%i, idx=%i, time=%i ms", id, idx, duration);
      }
      else 
      {
        MRP_SYSTEM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_WARN, "--> mrp_sys_timer_start id=%i, idx=%i, time=%i ms rqb in use", id, idx, duration);
      }
      pDBtimer->pTimerPara[idx].state          = MRP_TIMER_STS_T_RUNNING; 
      pDBtimer->pTimerPara[idx].durationTicks  = durationTicks;
      pDBtimer->pTimerPara[idx].startTicks     = mrp_sys_cyclic_get_curr_time();
    }
    break;
    
  case MRP_TIMER_STS_T_RUNNING:
    pDBtimer->pTimerPara[idx].durationTicks    = durationTicks;
    MRP_SYSTEM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE, "--> mrp_sys_timer_start: timer already running!! id=%i, idx=%i, time=%i ms", id, idx, duration);
    break;

  /*==========================================================================*/

  case MRP_TIMER_STS_T_NOT_INIT:
  case MRP_TIMER_STS_T_STOP_PENDING:
    /*fall through*/

  default:
    MRP_SYSTEM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_FATAL,"--> mrp_sys_timer_start id=%i, idx=%i, unexpected state=%i", id, idx, pDBtimer->pTimerPara[idx].state);
    MRP_ASSERT(LSA_FALSE);
    break;
  }
}

/*===========================================================================
* FUNCTION : mrp_sys_cyclic_timer_stop
*----------------------------------------------------------------------------
* PURPOSE  : stop timer
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : id = unique timer index
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_sys_cyclic_timer_stop (LSA_UINT16 id) 
{
  LSA_UINT16 rc;
  MRP_BASE_TIMER_PARA_DB_TYPE *pCyclicTimer;

  MRP_ASSERT (id == 0);
  LSA_UNUSED_ARG (id);

  pCyclicTimer = &g_Mrp.cyclicTimer;

  switch (pCyclicTimer->state) 
  {
    /*==========================================================================*/
  case MRP_TIMER_STS_T_STOPPED:
  case MRP_TIMER_STS_T_RUNNING:
    pCyclicTimer->state = MRP_TIMER_STS_T_STOPPED;

    MRP_SYSTEM_TRACE_00(0, LSA_TRACE_LEVEL_NOTE, "--> mrp_sys_cyclic_timer_stop id=0");

    MRP_STOP_TIMER(&rc, pCyclicTimer->timerId);

    if (!(rc == LSA_RET_OK || rc == LSA_RET_OK_TIMER_NOT_RUNNING)) 
    {
      MRP_SYSTEM_TRACE_02(0, LSA_TRACE_LEVEL_FATAL,"--> mrp_sys_cyclic_timer_stop id=0, rc=0x%x, timerId=%i", rc, pCyclicTimer->timerId);
    }
    MRP_FATAL1(rc == LSA_RET_OK || rc == LSA_RET_OK_TIMER_NOT_RUNNING);
    break;

    /*==========================================================================*/

  case MRP_TIMER_STS_T_NOT_INIT:
  case MRP_TIMER_STS_T_STOP_PENDING:
    /*fall through*/

  default:
    MRP_SYSTEM_TRACE_01(0, LSA_TRACE_LEVEL_FATAL,"--> mrp_sys_cyclic_timer_stop id=0, unknown state=%i", pCyclicTimer->state);
    MRP_ASSERT(LSA_FALSE);
    break;
  }
}


/*===========================================================================
* FUNCTION : mrp_sys_timer_stop
*----------------------------------------------------------------------------
* PURPOSE  : stop timer
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : id = unique timer index
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_sys_timer_stop (LSA_UINT16 id) 
{
  LSA_UINT16 idx;
  LSA_UINT32 pnioInterfaceID;
  MRP_CHANNEL_TYPE_COMMON pChSys;
  MRP_TIMER_DB_TYPE *pDBtimer;
  LSA_UINT32 traceIdx;

  idx = id % MRP_MAX_NO_OF_TIMER_IDS_PER_INTERFACE;

  pnioInterfaceID = mrp_sys_map_timer_id_to_if_id (id);
  pChSys          = mrp_sys_get_sys_ch_by_if_id (pnioInterfaceID);
  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pChSys, LSA_NULL));

  traceIdx = pChSys->pChSysDB->traceIdx;   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */
  LSA_UNUSED_ARG(traceIdx);   /* hhr: to please the compiler in case traces are disabled. */

  pDBtimer = (MRP_TIMER_DB_TYPE *)pChSys->pChSysDB->pInterfaceTimer;
  MRP_FATAL1(pChSys == pDBtimer->pCh);

  switch (pDBtimer->pTimerPara[idx].state) 
  {
  /*==========================================================================*/
  case MRP_TIMER_STS_T_STOPPED:
    MRP_SYSTEM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_CHAT, "--> mrp_sys_timer_stop id=%i, idx=%i already stopped", id, idx);
    break;
    
  /*==========================================================================*/
  case MRP_TIMER_STS_T_STOP_PENDING:
    MRP_SYSTEM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_WARN, "--> mrp_sys_timer_stop id=%i, idx=%i still pending", id, idx);
    break;
    
  /*==========================================================================*/
  case MRP_TIMER_STS_T_RUNNING:
    /* rqb in queue ? */
    /* only applicable for idx = 0, because for idx > 0 no rqb is used (tx = rx = 0)!!! */
    if (pDBtimer->pTimerPara[idx].tx == pDBtimer->pTimerPara[idx].rx) 
    {
      pDBtimer->pTimerPara[idx].state = MRP_TIMER_STS_T_STOPPED;
      MRP_SYSTEM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_CHAT, "--> mrp_sys_timer_stop id=%i, idx=%i stopped", id, idx);
    }
    else
    {
      pDBtimer->pTimerPara[idx].state = MRP_TIMER_STS_T_STOP_PENDING;
      MRP_SYSTEM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "--> mrp_sys_timer_stop id=%i, idx=%i: timer already expired, but timer request not processed yet", id, idx);
    }
    break;

  /*==========================================================================*/
  case MRP_TIMER_STS_T_NOT_INIT:
      /*fall through*/

  default:
    MRP_SYSTEM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_FATAL, "--> mrp_sys_timer_stop id=%i, idx=%i unknown state=%i", id, idx, pDBtimer->pTimerPara[idx].state);
    MRP_ASSERT(LSA_FALSE);
    break;
  }
}

/*===========================================================================
* FUNCTION : mrp_sys_cyclic_timer_get_state
*----------------------------------------------------------------------------
* PURPOSE  : get timer state of cyclic component timer
*----------------------------------------------------------------------------
* RETURNS  : state of timer
*----------------------------------------------------------------------------
* INPUTS   : id            = unique timer ID
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
MRP_TIMER_STS_T mrp_sys_cyclic_timer_get_state (LSA_UINT16 id)
{
  MRP_BASE_TIMER_PARA_DB_TYPE *pCyclicTimer;

  MRP_ASSERT (id == 0);
  LSA_UNUSED_ARG (id);

  pCyclicTimer = &g_Mrp.cyclicTimer;

  return pCyclicTimer->state;
}

/*===========================================================================
* FUNCTION : mrp_sys_cyclic_timer_state_check
*----------------------------------------------------------------------------
* PURPOSE  : start or stop cyclic timer (depending on redundancy states)
*----------------------------------------------------------------------------
* RETURNS  : runFlag
*----------------------------------------------------------------------------
* INPUTS   : id = unique timer ID
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT8 mrp_sys_cyclic_timer_state_check (LSA_UINT16 id)
{
  LSA_UINT16 i, k, mrpMaxInstance;
  MRP_CHANNEL_TYPE_USER pChPrmUsr;
  MRP_TIMER_STS_T timerState;
  LSA_UINT8 runFlag, startCyclicTimer = LSA_FALSE;

  for (i = 0; i < MRP_MAX_NO_OF_INTERFACES && !startCyclicTimer; i++) 
  {
    pChPrmUsr = g_Mrp.pChPrmUsr[i];
    if (pChPrmUsr->state == MRP_CH_STATE_READY)
    {
      MRP_REC_DB_TYPE *pDBrec = (MRP_REC_DB_TYPE *)pChPrmUsr->pChUsrDB->pRec;
      
      mrpMaxInstance = mrp_sys_get_num_of_instances (pChPrmUsr);
      for (k = 0; k < mrpMaxInstance; k++)
      {
        if (pDBrec->pInstData[k].activeRedModules)
        {
          startCyclicTimer = LSA_TRUE;
          break;
        }
      }
    }
  }

  timerState = mrp_sys_cyclic_timer_get_state (id);
  if (startCyclicTimer)
  {
    if (timerState != MRP_TIMER_STS_T_RUNNING)
      mrp_sys_cyclic_timer_start (id, (MRP_DEFAULT_VAL_CYCLIC_INTERVAL_MS / MRP_TIMER_TIME_BASE_DEZ));

    runFlag = LSA_TRUE;
  }
  else
  {
    if (timerState != MRP_TIMER_STS_T_STOPPED)
      mrp_sys_cyclic_timer_stop (id);

    runFlag = LSA_FALSE;
  }

  return runFlag;
}

/*===========================================================================
* FUNCTION : mrp_sys_timer_get_state
*----------------------------------------------------------------------------
* PURPOSE  : get timer state
*----------------------------------------------------------------------------
* RETURNS  : state of timer
*----------------------------------------------------------------------------
* INPUTS   : id = unique timer index
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
MRP_TIMER_STS_T mrp_sys_timer_get_state (LSA_UINT16 id) 
{
  LSA_UINT16 idx;
  LSA_UINT32 pnioInterfaceID;
  MRP_CHANNEL_TYPE_COMMON pChSys;
  MRP_TIMER_DB_TYPE *pDBtimer;

  idx = id % MRP_MAX_NO_OF_TIMER_IDS_PER_INTERFACE;

  pnioInterfaceID = mrp_sys_map_timer_id_to_if_id (id);
  pChSys          = mrp_sys_get_sys_ch_by_if_id (pnioInterfaceID);
  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pChSys, LSA_NULL));

  pDBtimer = (MRP_TIMER_DB_TYPE *)pChSys->pChSysDB->pInterfaceTimer;
  MRP_FATAL1(pChSys == pDBtimer->pCh);

  MRP_ASSERT(idx < pDBtimer->timerTblSize);

  return pDBtimer->pTimerPara[idx].state;
}

/*===========================================================================
* FUNCTION : mrp_sys_timer_get_time_diff
*----------------------------------------------------------------------------
* PURPOSE  : get time difference between two timer values
*----------------------------------------------------------------------------
* RETURNS  : time difference in timer units (e.g. ms)
*----------------------------------------------------------------------------
* INPUTS   : startTime (e.g. send time of testframe), 
*            endTime (e.g. recv time of sent testframe)
* OUTPUTS  : time difference
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT32 mrp_sys_timer_get_time_diff (LSA_UINT32 startTime, LSA_UINT32 endTime) 
{
  LSA_UINT32 delay;

  if (endTime >= startTime) 
  {
    delay = endTime - startTime;
  }
  else 
  {
    delay = (0xFFFFFFFFUL - startTime) + endTime + 1;
  }

  return delay;
}

/*===========================================================================
* FUNCTION : mrp_sys_map_port_to_instance
*----------------------------------------------------------------------------
* PURPOSE  : return the MRP/HSR instance associated with one port
*----------------------------------------------------------------------------
* RETURNS  : instance = associated instance to a port
*----------------------------------------------------------------------------
* INPUTS   : pCh  = channel reference for system or user channel
*            port = port to be checked as ringport
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT16 mrp_sys_map_port_to_instance (const MRP_CONST_CHANNEL_TYPE_COMMON pCh, LSA_UINT16 port)
{
  LSA_UINT16 instance, maxInstance, i;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pCh->pChSysDB->pEdd;
  MRP_IF_DB_TYPE *pDBif = LSA_NULL;

  instance = 0;

  if(!LSA_HOST_PTR_ARE_EQUAL(pCh, LSA_NULL))
  {
    if (!LSA_HOST_PTR_ARE_EQUAL(pCh->pChSysDB, LSA_NULL))
    {
      pDBif = (MRP_IF_DB_TYPE *)(pCh->pChSysDB->pInterfacePara);

      if (!LSA_HOST_PTR_ARE_EQUAL(pDBif, LSA_NULL))
      {
        maxInstance = pDBif->mrpMaxInstance;

        for (i = 0; i < maxInstance; i++)
        {
          if (pDBedd->pInstData[i].RPort_1 == port || pDBedd->pInstData[i].RPort_2 == port)
          {
            instance = i;
            break;
          }
        }
      }
    }
  }

  return instance;
}


/*===========================================================================
* FUNCTION : mrp_sys_map_rcv_frame_to_instance
*----------------------------------------------------------------------------
* PURPOSE  : return the MRP/HSR instance associated with one port or received frame
*----------------------------------------------------------------------------
* RETURNS  : instance = associated instance to a port or received frame
*----------------------------------------------------------------------------
* INPUTS   : pCh     = channel reference for system or user channel
*            port    = port to be checked as ringport
*            pBuffer = pointer to received frame
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT16 mrp_sys_map_rcv_frame_to_instance (const MRP_CONST_CHANNEL_TYPE_COMMON pCh, LSA_UINT16 port, LSA_UINT8 *pBuffer)
{
  LSA_UINT16 instance, maxInstance, i, k;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pCh->pChSysDB->pEdd;
  MRP_IF_DB_TYPE *pDBif = LSA_NULL;
  MRP_PACKET_STRUCT_HEADER_TYPE *pFrameHdr;

  pFrameHdr = (MRP_PACKET_STRUCT_HEADER_TYPE *)pBuffer; /*lint !e826 CM 10/11/2016 cast on purpose */
  instance = 0;

  if(!LSA_HOST_PTR_ARE_EQUAL(pCh, LSA_NULL))
  {
    if (!LSA_HOST_PTR_ARE_EQUAL(pCh->pChSysDB, LSA_NULL))
    {
      pDBif = (MRP_IF_DB_TYPE *)(pCh->pChSysDB->pInterfacePara);

      if (!LSA_HOST_PTR_ARE_EQUAL(pDBif, LSA_NULL))
      {
        maxInstance = pDBif->mrpMaxInstance;

        /* frame was received on ringport */
        for (i = 0; i < maxInstance; i++)
        {
          if (pDBedd->pInstData[i].RPort_1 == port || pDBedd->pInstData[i].RPort_2 == port)
          {
            instance = i;
            return instance;
          }
        }
        /* frame was received on non-ringport */
        for (k = 0; k < pDBif->numOfPorts; k++) 
        {
          /* if the sender port mac address is on the same device, map the frame to the MRP-/HSR instance, this port belongs to */
          if (!MRP_MEMCMP(pFrameHdr->src, pDBedd->ppPortMacAddr[k], 6))
          {
            for (i = 0; i < maxInstance; i++)
            {
              if (pDBedd->pInstData[i].RPort_1 == k + 1 || pDBedd->pInstData[i].RPort_2 == k + 1)
              {
                instance = i;
                return instance;
              }
            }
          }
        }
      }
    }
  }

  return instance;
}

/*===========================================================================
* FUNCTION : mrp_sys_get_num_of_ports
*----------------------------------------------------------------------------
* PURPOSE  : return the number of ethernet ports for a given system channel
*----------------------------------------------------------------------------
* RETURNS  : numOfPorts = number of ports for a given system channel
*----------------------------------------------------------------------------
* INPUTS   : pCh = channel reference for system or user channel
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT16 mrp_sys_get_num_of_ports (const MRP_CONST_CHANNEL_TYPE_COMMON pCh)
{
  LSA_UINT16 numOfPorts;
  MRP_IF_DB_TYPE *pDBif = LSA_NULL;

  numOfPorts = 0;

  if(!LSA_HOST_PTR_ARE_EQUAL(pCh, LSA_NULL))
  {
    if (!LSA_HOST_PTR_ARE_EQUAL(pCh->pChSysDB, LSA_NULL))
    {
      pDBif = (MRP_IF_DB_TYPE *)(pCh->pChSysDB->pInterfacePara);

      if (!LSA_HOST_PTR_ARE_EQUAL(pDBif, LSA_NULL))
      {
        numOfPorts = pDBif->numOfPorts;
      }
    }
  }

  return numOfPorts;
}

/*===========================================================================
* FUNCTION : mrp_sys_get_num_of_instances
*----------------------------------------------------------------------------
* PURPOSE  : return the number of MRP instances for a given system channel
*----------------------------------------------------------------------------
* RETURNS  : numOfInstances = number of MRP instances for a given system channel
*----------------------------------------------------------------------------
* INPUTS   : pCh = channel reference for system or user channel
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT16 mrp_sys_get_num_of_instances (const MRP_CONST_CHANNEL_TYPE_COMMON pCh)
{
  LSA_UINT16 numOfInstances;
  MRP_IF_DB_TYPE *pDBif = LSA_NULL;

  numOfInstances = 1;

  if(!LSA_HOST_PTR_ARE_EQUAL(pCh, LSA_NULL))
  {
    if (!LSA_HOST_PTR_ARE_EQUAL(pCh->pChSysDB, LSA_NULL))
    {
      pDBif = (MRP_IF_DB_TYPE *)(pCh->pChSysDB->pInterfacePara);

      if (!LSA_HOST_PTR_ARE_EQUAL(pDBif, LSA_NULL))
      {
        numOfInstances = pDBif->mrpMaxInstance;
      }
    }
  }

  return numOfInstances;
}


/*===========================================================================
* FUNCTION : mrp_sys_get_inport_cnt
*----------------------------------------------------------------------------
* PURPOSE  : get the number of ports indicated as interconnection ports
*----------------------------------------------------------------------------
* RETURNS  : inportCnt = number of MRP interconnection ports
*----------------------------------------------------------------------------
* INPUTS   : pChUsr = user channel reference
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_UINT16 mrp_sys_get_inport_cnt (const MRP_CONST_CHANNEL_TYPE_USER pChUsr)
{
  LSA_UINT16 inPortCnt, i;
  MRP_IF_DB_TYPE *pDBif = (MRP_IF_DB_TYPE *)pChUsr->pChSysDB->pInterfacePara;

  for (i = 0, inPortCnt = 0; i < pDBif->numOfPorts; i++)
  {
    if (pDBif->pPortData[i].inPortType == EDD_SUPPORTS_MRP_INTERCONN_PORT_CONFIG_YES)
      inPortCnt++;
  }

  return inPortCnt;
}



/*===========================================================================
* FUNCTION : mrp_sys_calc_ha_admin_role
*----------------------------------------------------------------------------
* PURPOSE  : calculate the admin role for a proprietary high-availability
*            feature (HSYNC)
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pCh = channel reference for system or user channel
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_sys_calc_ha_admin_role (const MRP_CONST_CHANNEL_TYPE_COMMON pCh)
{
  MRP_IF_DB_TYPE *pDBif = LSA_NULL;

  if (!LSA_HOST_PTR_ARE_EQUAL (pCh, LSA_NULL))
  {
    if (!LSA_HOST_PTR_ARE_EQUAL (pCh->pChSysDB, LSA_NULL))
    {
      pDBif = (MRP_IF_DB_TYPE *)(pCh->pChSysDB->pInterfacePara);

      if (!LSA_HOST_PTR_ARE_EQUAL (pDBif, LSA_NULL))
      {
        if (!pDBif->mrpMaxInstance || pDBif->mrpSupportedRoleInst0 == EDD_MRP_ROLE_NONE)
        {
          pDBif->hsyncAdminRole = MRP_HA_ADMIN_NONE;
        }
        else
        {
          if (pDBif->hsyncFwdRulesSupported == MRP_HSYNC_FWD_RULES_SUPPORTED_OFF)
          {
            pDBif->hsyncAdminRole = MRP_HA_ADMIN_NONE;
          }
          else
          {
            if (pDBif->hsyncApplExists == MRP_HSYNC_APPL_EXISTS_OFF)
            {
              pDBif->hsyncAdminRole = MRP_HA_ADMIN_FORWARDER;
            }
            else
            {
              pDBif->hsyncAdminRole = MRP_HA_ADMIN_APPLICATION;
            }
          }
        }
      }
    }
  }
}

/*===========================================================================
* FUNCTION : mrp_sys_get_ha_admin_role
*----------------------------------------------------------------------------
* PURPOSE  : return the admin role for a proprietary high-availability 
*            feature (HSYNC)
*----------------------------------------------------------------------------
* RETURNS  : hsyncAdminRole = requested admin role, calculated by means of 
*                             GET_PARAMS flags
*----------------------------------------------------------------------------
* INPUTS   : pCh = channel reference for system or user channel
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
MRP_HA_ADMIN_ROLE_ENUM_T mrp_sys_get_ha_admin_role (const MRP_CONST_CHANNEL_TYPE_COMMON pCh)
{
  MRP_IF_DB_TYPE *pDBif = LSA_NULL;
  MRP_HA_ADMIN_ROLE_ENUM_T hsyncAdminRole;

  hsyncAdminRole = MRP_HA_ADMIN_NONE;

  if (!LSA_HOST_PTR_ARE_EQUAL (pCh, LSA_NULL))
  {
    if (!LSA_HOST_PTR_ARE_EQUAL (pCh->pChSysDB, LSA_NULL))
    {
      pDBif = (MRP_IF_DB_TYPE *)(pCh->pChSysDB->pInterfacePara);

      if (!LSA_HOST_PTR_ARE_EQUAL (pDBif, LSA_NULL))
      {
        hsyncAdminRole = pDBif->hsyncAdminRole;
      }
    }
  }

  return hsyncAdminRole;
}


/*===========================================================================
* FUNCTION : mrp_sys_timer_init
*----------------------------------------------------------------------------
* PURPOSE  : initialize timer database
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys         = channel reference for edd system channel
*            mrpMaxInstance = maximum number of MRP instances to be initialized
*            mrpInMaxInstance = maximum number of MRP interconnection instances
*                               to be initialized
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : the number of ports of the interface has to be known!
*==========================================================================*/
LSA_VOID mrp_sys_timer_init (const MRP_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance)
{
  LSA_UINT16 numOfPorts;
  LSA_UINT16 timerTblSize, i;
  MRP_TIMER_DB_TYPE *pDBtimer = LSA_NULL;

  MRP_SYSTEM_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_sys_timer_init entered: mrpMaxInstance=%d, mrpInMaxInstance=%d", mrpMaxInstance, mrpInMaxInstance);
  
  MRP_ASSERT (mrpInMaxInstance <= EDD_CFG_MAX_MRP_IN_INSTANCE_CNT);
  
  numOfPorts = mrp_sys_get_num_of_ports (pChSys);
  
  timerTblSize = (LSA_UINT16)(MRP_GET_TIMER_TBL_SIZE(numOfPorts, mrpMaxInstance, mrpInMaxInstance));

  if (numOfPorts && timerTblSize)
  {
    MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBtimer), sizeof(MRP_TIMER_DB_TYPE));
    if (LSA_HOST_PTR_ARE_EQUAL(pDBtimer, LSA_NULL)) 
    {
      MRP_SYSTEM_TRACE_00(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> mrp_sys_timer_init: ALLOC_LOCAL_MEM for TIMER_DB failed");
    }
    else
    {
      /* preset zero */
      MRP_MEMSET(pDBtimer, 0, sizeof(MRP_TIMER_DB_TYPE));

      MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBtimer->pTimerPara), (LSA_UINT16)(timerTblSize * sizeof(MRP_TIMER_PARA_DB_TYPE)));
      if (LSA_HOST_PTR_ARE_EQUAL(pDBtimer->pTimerPara, LSA_NULL)) 
      {
        MRP_SYSTEM_TRACE_00(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, ">> mrp_sys_timer_init: ALLOC_LOCAL_MEM for TIMER_PARA_DB failed");
      }
      else
      {
        for (i = 0; i < timerTblSize; i++) 
        {
          MRP_MEMSET(&(pDBtimer->pTimerPara[i]), 0, sizeof(MRP_TIMER_PARA_DB_TYPE));
        }
        pDBtimer->timerTblSize = timerTblSize;
        pDBtimer->pCh          = pChSys;

        MRP_SYSTEM_TRACE_03(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, ">> mrp_sys_timer_init: ALLOC_LOCAL_MEM for TIMER_PARA_DB successful, timerTblSize=%d, numOfPorts=%d, numOfInstances=%d", timerTblSize, numOfPorts, mrpMaxInstance);
        
        pChSys->pChSysDB->pInterfaceTimer = pDBtimer;
      }
    }
  }
}


/*===========================================================================
* FUNCTION : mrp_sys_timer_deinit
*----------------------------------------------------------------------------
* PURPOSE  : deallocate timer DB of one interface
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference for edd system channel
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_sys_timer_deinit (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys)
{
  LSA_UINT16  rc;
  MRP_TIMER_DB_TYPE *pDBtimer = (MRP_TIMER_DB_TYPE *)pChSys->pChSysDB->pInterfaceTimer;

  MRP_SYSTEM_TRACE_00(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_sys_timer_deinit entered");
  
  pChSys->pChSysDB->pInterfaceTimer = LSA_NULL;

  MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBtimer->pTimerPara);
  MRP_FATAL1(rc == LSA_RET_OK);

  pDBtimer->pTimerPara    = LSA_NULL;
  pDBtimer->timerTblSize  = 0;
  pDBtimer->pCh           = LSA_NULL;

  MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBtimer);
  MRP_FATAL1(rc == LSA_RET_OK);



}

/*===========================================================================
* FUNCTION : mrp_sys_if_db_init
*----------------------------------------------------------------------------
* PURPOSE  : initialize EDD interface details
*----------------------------------------------------------------------------
* RETURNS  : status
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference for edd system channel
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT16 mrp_sys_if_db_init (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys)
{
  LSA_UINT16 resp;
  MRP_IF_DB_TYPE *pDBif = LSA_NULL;

  resp     = MRP_RSP_OK;

  MRP_SYSTEM_TRACE_00(0, LSA_TRACE_LEVEL_NOTE, "mrp_sys_if_db_init entered");
  
  MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBif), sizeof(MRP_IF_DB_TYPE));
  if (LSA_HOST_PTR_ARE_EQUAL(pDBif, LSA_NULL)) 
  {
    MRP_SYSTEM_TRACE_00(0, LSA_TRACE_LEVEL_ERROR, ">> mrp_sys_if_db_init: ALLOC_LOCAL_MEM for IF_DB failed");
    resp = MRP_RSP_ERR_RESOURCE;
  }
  else
  {
    /* preset zero */
    MRP_MEMSET(pDBif, 0, sizeof(MRP_IF_DB_TYPE));

    pChSys->pChSysDB->pInterfacePara = pDBif;
  }

  return resp;
}


/*===========================================================================
* FUNCTION : mrp_sys_if_db_deinit
*----------------------------------------------------------------------------
* PURPOSE  : deallocate EDD interface details
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference for edd system channel
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_sys_if_db_deinit (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys)
{
  LSA_UINT16 rc;
  MRP_IF_DB_TYPE *pDBif = (MRP_IF_DB_TYPE *)pChSys->pChSysDB->pInterfacePara;

  MRP_SYSTEM_TRACE_00(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_sys_if_db_deinit entered");
  
  pChSys->pChSysDB->pInterfacePara = LSA_NULL;

  if (!(LSA_HOST_PTR_ARE_EQUAL(pDBif->pPortData, LSA_NULL)) )
  {
    MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBif->pPortData);
    MRP_FATAL1(rc == LSA_RET_OK);

    pDBif->pPortData = LSA_NULL;
  }

  if (!(LSA_HOST_PTR_ARE_EQUAL (pDBif->pSequenceID, LSA_NULL)))
  {
    MRP_FREE_LOCAL_MEM (&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBif->pSequenceID);
    MRP_FATAL1 (rc == LSA_RET_OK);

    pDBif->pSequenceID = LSA_NULL;
  }

  MRP_FREE_LOCAL_MEM(&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBif);
  MRP_FATAL1(rc == LSA_RET_OK);
}

/*===========================================================================
* FUNCTION : mrp_sys_ch_db_init
*----------------------------------------------------------------------------
* PURPOSE  : initialize system channel database
*----------------------------------------------------------------------------
* RETURNS  : status
*----------------------------------------------------------------------------
* INPUTS   : pChSys = system channel reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT16 mrp_sys_ch_db_init (const MRP_CHANNEL_TYPE_SYSTEM pChSys)
{
  LSA_UINT16 resp;
  MRP_CH_SYS_DB_TYPE *pDBsys = LSA_NULL;

  MRP_SYSTEM_TRACE_00(0, LSA_TRACE_LEVEL_NOTE, "mrp_sys_ch_db_init");

  MRP_ALLOC_LOCAL_MEM((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBsys), sizeof(MRP_CH_SYS_DB_TYPE));

  if (LSA_HOST_PTR_ARE_EQUAL(pDBsys, LSA_NULL)) 
  {
    MRP_SYSTEM_TRACE_00(0, LSA_TRACE_LEVEL_ERROR, ">> mrp_sys_ch_db_init: ALLOC_LOCAL_MEM for DB_sys(channel) failed");
    resp = MRP_RSP_ERR_RESOURCE;
  }
  else
  {
    resp = MRP_RSP_OK;

    /* preset zero */
    MRP_MEMSET(pDBsys, 0, sizeof(MRP_CH_SYS_DB_TYPE));

    pChSys->pChSysDB = pDBsys;
  }

  return resp;
}


/*===========================================================================
* FUNCTION : mrp_sys_ch_db_deinit
*----------------------------------------------------------------------------
* PURPOSE  : uninitialize system channel database
*----------------------------------------------------------------------------
* RETURNS  : status
*----------------------------------------------------------------------------
* INPUTS   : pChSys = system channel reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_sys_ch_db_deinit(const MRP_CHANNEL_TYPE_SYSTEM pChSys)
{
  LSA_UINT16  rc;
  MRP_CH_SYS_DB_TYPE *pDBsys = (MRP_CH_SYS_DB_TYPE *)pChSys->pChSysDB;

  MRP_SYSTEM_TRACE_00(0, LSA_TRACE_LEVEL_NOTE, "mrp_sys_ch_db_deinit");
  
  pChSys->pChSysDB = LSA_NULL;

  MRP_FREE_LOCAL_MEM(&rc,(MRP_LOCAL_MEM_PTR_TYPE)pDBsys);
  MRP_FATAL1(rc == LSA_RET_OK);

}

/*===========================================================================
* FUNCTION : mrp_sys_init
*----------------------------------------------------------------------------
* PURPOSE  : initialize MRP resp. HSR/standby
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys         = channel reference for edd system channel
*            mrpMaxInstance = maximum number of MRP instances to be initialized
*            mrpInMaxInstance = maximum number of MRP interconnection instances 
*                               to be initialized
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : for init/deinit function channel declaration not "const" 
*==========================================================================*/
LSA_VOID mrp_sys_init (const MRP_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance)
{
  MRP_SYSTEM_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_sys_init entered, mrpMaxInstance=%d, mrpInMaxInstance=%d", mrpMaxInstance, mrpInMaxInstance);
  
  MRP_ASSERT (mrpInMaxInstance <= EDD_CFG_MAX_MRP_IN_INSTANCE_CNT);
  
  mrp_mib_db_init (pChSys, mrpMaxInstance);

  mrp_mrc_init_snd_rqb (pChSys, mrpMaxInstance);
  mrp_mrc_init (pChSys, mrpMaxInstance, mrpInMaxInstance);

#ifdef MRP_CFG_PLUGIN_0_MRM
  mrp_mrm_init_snd_rqb (pChSys, mrpMaxInstance);
  mrp_mrm_init (pChSys, mrpMaxInstance, mrpInMaxInstance);
#endif

  /* MRP_IN: Init MRP interconnection */
  mrp_in_init_snd_rqb (pChSys, mrpInMaxInstance);
  mrp_in_init (pChSys, mrpMaxInstance, mrpInMaxInstance);

#ifdef  MRP_CFG_PLUGIN_2_HSR
  mrp_bs_init (pChSys);
  mrp_rc_init (pChSys);
  mrp_mgr_init (pChSys);
  mrp_stby_init (pChSys);
  mrp_ard_init (pChSys);
#endif /* MRP_CFG_PLUGIN_2_HSR */

}


/*===========================================================================
* FUNCTION : mrp_sys_deinit
*----------------------------------------------------------------------------
* PURPOSE  : deinitialize MRP resp. HSR/standby 
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys         = channel reference for edd system channel
*            mrpMaxInstance   = maximum number of MRP instances to be uninitialized
*            mrpMaxInInstance = maximum number of MRP interconnection instances to be uninitialized
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : for init/deinit function channel declaration not "const" 
*==========================================================================*/
LSA_VOID mrp_sys_deinit (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpMaxInInstance)
{
  MRP_SYSTEM_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_sys_deinit entered, mrpMaxInstance=%d, mrpMaxInInstance=%d", mrpMaxInstance, mrpMaxInInstance);
  
#ifdef MRP_CFG_PLUGIN_0_MRM
  mrp_mrm_deinit (pChSys, mrpMaxInstance, mrpMaxInInstance);
#endif /* MRP_CFG_PLUGIN_0_MRM */

  mrp_mrc_deinit (pChSys, mrpMaxInstance, mrpMaxInInstance);
  mrp_mib_db_deinit (pChSys, mrpMaxInstance);

  /* MRP_IN: Deinit MRP interconnection */
  mrp_in_deinit (pChSys, mrpMaxInstance, mrpMaxInInstance);

#ifdef  MRP_CFG_PLUGIN_2_HSR
  mrp_ard_deinit (pChSys);
  mrp_stby_deinit (pChSys);
  mrp_mgr_deinit (pChSys);
  mrp_rc_deinit (pChSys);
  mrp_bs_deinit (pChSys);
#endif /* MRP_CFG_PLUGIN_2_HSR */
}
