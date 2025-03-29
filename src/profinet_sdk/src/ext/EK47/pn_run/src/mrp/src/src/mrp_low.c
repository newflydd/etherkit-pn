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
/*  F i l e               &F: mrp_low.c                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  lower layer functions.                                                   */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef MRP_MESSAGE
/*  2008-06-06  mk    port params added to mrp_ll_fdb_cfg.                   */ 
/*  2008-06-11  mk    topoCtrl added.                                        */
/*  2009-03-19  ds    trace added                                            */
/*  2009-07-02  ds    include hierarchy changed                              */
/*  2009-10-15  ds    cleanups for LL services (fwdCtrl)                     */
/*  2009-10-20  ds    cleanups for LL services (MC enable/disable)           */
/*  2009-10-22  ds    cleanups for LL services (OHA topo ctrl)               */
/*  2010-02-16  ds    MRP based on 1 ms (HW) timer instead of 10 ms base     */
/*                    timer constants cleaned up, timer handling corrected   */
/*  2010-05-25  ds    new OHA upper channel related function calls           */
/*  2010-06-11  ds    some new LL functions: mrp_ll_remove_user_reference,   */
/*                    mrp_ll_add_user_reference                              */
/*                    make more than one common user channel possible        */
/*  2010-07-19  ds    new feature MRP MIB implemented according to           */
/*                    IEC 62439-2                                            */
/*  2012-04-04  ds    introduction of N MRP instances per interface          */
/*  2013-11-28  mk    RQ827954: erroneous connected ring line detection      */
/*  2018-10-19  hh    introduction of N MRP interconnection instances        */
/*                                                                           */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID   3
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

/*---------------------------------------------------------------------------*/
/*                                globals                                    */
/*---------------------------------------------------------------------------*/
/*=== used to init mrp mode ===*/
const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_init_nsm[] = 
{
   /* first and last entry must be LSA_NULL (dummy) !!! */
   /* cbf                                     , rqbNum                                  , service  */
   { LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END            , 0x0000 }
  ,{ mrp_edd_req_set_multicast               , MRP_LL_SREQ_RQB_ENUM_MULTICAST_TST      , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_done          , MRP_LL_SREQ_RQB_ENUM_MULTICAST_TST      , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl      , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_TST , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl_done , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_TST , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_set_multicast               , MRP_LL_SREQ_RQB_ENUM_MULTICAST_WRK      , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_done          , MRP_LL_SREQ_RQB_ENUM_MULTICAST_WRK      , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl      , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_WRK , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl_done , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_WRK , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_port_state                  , MRP_LL_SREQ_RQB_ENUM_SET_PS_SHADOW      , EDD_SRV_SWITCH_SET_PORT_STATE }
  ,{ mrp_edd_req_port_state_done             , MRP_LL_SREQ_RQB_ENUM_SET_PS_SHADOW      , EDD_SRV_SWITCH_SET_PORT_STATE }
  ,{ mrp_edd_req_link_retrigger              , MRP_LL_SREQ_RQB_ENUM_TBL_END            , 0x0000 }
  ,{ mrp_edd_req_oha_data_write              , MRP_LL_SREQ_RQB_ENUM_TBL_END            , 0x0000 } /* topo ctrl */
  ,{ LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END            , 0x0000 }
};

/*=== used to deinit mrp mode ===*/
const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_deinit_nsm[] = 
{
   { LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END            , 0x0000 }
  ,{ mrp_edd_req_set_multicast               , MRP_LL_SREQ_RQB_ENUM_MULTICAST_TST      , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_done          , MRP_LL_SREQ_RQB_ENUM_MULTICAST_TST      , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl      , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_TST , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl_done , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_TST , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_set_multicast               , MRP_LL_SREQ_RQB_ENUM_MULTICAST_WRK      , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_done          , MRP_LL_SREQ_RQB_ENUM_MULTICAST_WRK      , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl      , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_WRK , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl_done , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_WRK , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_port_state                  , MRP_LL_SREQ_RQB_ENUM_SET_PS_DEINIT      , EDD_SRV_SWITCH_SET_PORT_STATE }
  ,{ mrp_edd_req_port_state_done             , MRP_LL_SREQ_RQB_ENUM_SET_PS_DEINIT      , EDD_SRV_SWITCH_SET_PORT_STATE }
  ,{ mrp_edd_req_oha_data_write              , MRP_LL_SREQ_RQB_ENUM_TBL_END            , 0x0000 } /* topo ctrl */
  ,{ LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END            , 0x0000 }
};

/*=== used to control MRP data in OHA/LLDP frames ===*/
const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_ind_topo_change[] = 
{
   /* cbf                                     , rqbNum                                  , service  */
   { LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END            , 0x0000 }
  ,{ mrp_edd_req_oha_data_write              , MRP_LL_SREQ_RQB_ENUM_TBL_END            , 0x0000 } /* topo ctrl */
  ,{ LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END            , 0x0000 }
};

/*=== controls flush FDB ===*/
const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_flush_fdb[] = 
{
   { LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END                , 0x0000 }
  ,{ mrp_edd_req_port_state                  , MRP_LL_SREQ_RQB_ENUM_SET_PS_TPLG_CHG_START  , EDD_SRV_SWITCH_SET_PORT_STATE  }
  ,{ mrp_edd_req_port_state_done             , MRP_LL_SREQ_RQB_ENUM_SET_PS_TPLG_CHG_START  , EDD_SRV_SWITCH_SET_PORT_STATE  }
  ,{ mrp_edd_req_flush_filtering_db          , MRP_LL_SREQ_RQB_ENUM_FLUSH_FILTERING_DB     , EDD_SRV_SWITCH_FLUSH_FILTERING_DB }
  ,{ mrp_edd_req_flush_filtering_db_done     , MRP_LL_SREQ_RQB_ENUM_FLUSH_FILTERING_DB     , EDD_SRV_SWITCH_FLUSH_FILTERING_DB }
  /*  ,{ mrp_edd_req_blocking_wait             , MRP_LL_SREQ_RQB_ENUM_TBL_END                , 0x0000  } */
  ,{ mrp_edd_req_port_state                  , MRP_LL_SREQ_RQB_ENUM_SET_PS_SHADOW          , EDD_SRV_SWITCH_SET_PORT_STATE }
  ,{ mrp_edd_req_port_state_done             , MRP_LL_SREQ_RQB_ENUM_SET_PS_SHADOW          , EDD_SRV_SWITCH_SET_PORT_STATE }
  ,{ mrp_edd_req_tplg_chg_finished           , MRP_LL_SREQ_RQB_ENUM_TBL_END                , 0x0000 }
  ,{ LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END                , 0x0000 }
};


/*=== used to set port state ===*/
const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_ps_set[] = 
{
   { LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END                , 0x0000 } 
  ,{ mrp_edd_req_port_state                  , MRP_LL_SREQ_RQB_ENUM_SET_PS_SHADOW          , EDD_SRV_SWITCH_SET_PORT_STATE }
  ,{ mrp_edd_req_port_state_done             , MRP_LL_SREQ_RQB_ENUM_SET_PS_SHADOW          , EDD_SRV_SWITCH_SET_PORT_STATE }
  ,{ LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END                , 0x0000 }
};

/*=== used to set port state ===*/
const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_ps_deinit_mrpi_set[] = 
{
   { LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END                , 0x0000 } 
  ,{ mrp_edd_req_port_state                  , MRP_LL_SREQ_RQB_ENUM_SET_PS_DEINIT_MRPI          , EDD_SRV_SWITCH_SET_PORT_STATE }
  ,{ mrp_edd_req_port_state_done             , MRP_LL_SREQ_RQB_ENUM_SET_PS_DEINIT_MRPI          , EDD_SRV_SWITCH_SET_PORT_STATE }
  ,{ LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END                , 0x0000 }
};


/*=== used to finish close usr channel ===*/
const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_finish_upper[] = 
{
   { LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END            , 0x0000 }
  ,{ mrp_edd_req_finish_upper                , MRP_LL_SREQ_RQB_ENUM_TBL_END            , 0x0000 }
  ,{ LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END            , 0x0000 }
};


/*=== used to complete close channel ===*/
const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_finish_close[] = 
{
   { LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END            , 0x0000 }
  ,{ mrp_edd_req_finish_close                , MRP_LL_SREQ_RQB_ENUM_TBL_END            , 0x0000 }
  ,{ LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END            , 0x0000 }
};

/*=== used to process prm_commit ===*/
const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_prm_commit[] = 
{
   { LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END            , 0x0000 }
  ,{ mrp_edd_req_prm_commit                  , MRP_LL_SREQ_RQB_ENUM_TBL_END            , 0x0000 }
  ,{ LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END            , 0x0000 }
};


#ifdef MRP_CFG_DELAYED_LINK_UP
const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_set_fdb[] = 
{
   { LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END                , 0x0000 }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl      , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_TST     , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl_done , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_TST     , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl      , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_WRK     , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl_done , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_WRK     , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END                , 0x0000 }
};
#endif

/* used to initialize HSYNC FDB settings */
const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_init_hsync[] =
{
   { LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END                , 0x0000 }
  ,{ mrp_edd_req_set_multicast               , MRP_LL_SREQ_RQB_ENUM_MULTICAST_HSYNC        , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_done          , MRP_LL_SREQ_RQB_ENUM_MULTICAST_HSYNC        , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl      , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_HSYNC   , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl_done , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_HSYNC   , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END                , 0x0000 }
};

/* used to deinit HSYNC FDB settings */
const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_deinit_hsync[] =
{
   { LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END                , 0x0000 }
  ,{ mrp_edd_req_set_multicast               , MRP_LL_SREQ_RQB_ENUM_MULTICAST_HSYNC        , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_done          , MRP_LL_SREQ_RQB_ENUM_MULTICAST_HSYNC        , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl      , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_HSYNC   , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl_done , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_HSYNC   , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END                , 0x0000 }
};

/* used to change HSYNC forwarding FDB settings */
const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_hsync_set_fdb[] =
{
   { LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END                 , 0x0000 }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl      , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_HSYNC    , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl_done , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_HSYNC    , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END                 , 0x0000 }
};

LSA_UINT16 ReqTbl_mrp_in_size = 0;

const MRP_LL_REQ_TBL_T * const pReqTbl_mrp_in[] =
{
  /* The following burst tables are needed per Interconnection instance!!! */
  g_MrpllReqTbl_mrp_in_mim_tplg_chg_req,
  g_MrpllReqTbl_mrp_in_init_mic,
  g_MrpllReqTbl_mrp_in_init_mim,
  LSA_NULL
};


/*===========================================================================
* FUNCTION : mrp_ll_get_mrp_base_req_tbl
*----------------------------------------------------------------------------
* PURPOSE  : get lower layer request bursts for mrp base (no plugins) 
*----------------------------------------------------------------------------
* RETURNS  : cnt    = number of lower layer request bursts counted
*----------------------------------------------------------------------------
* INPUTS   : ppReqTbl = pointer to array of request burst pointers
*            maxCnt   = max available array entries
* OUTPUTS  : ppReqTbl = updated array of request burst pointers
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT16 mrp_ll_get_mrp_base_req_tbl (const MRP_LL_REQ_TBL_T** ppReqTbl, LSA_UINT16 maxCnt)
{
  LSA_UINT16 cnt, array_size;

  const MRP_LL_REQ_TBL_T *pTmpReqTbl[]=
  { 
    g_MrpllReqTbl_mrp_init_nsm,
    g_MrpllReqTbl_mrp_deinit_nsm,
    g_MrpllReqTbl_mrp_ind_topo_change,
    g_MrpllReqTbl_mrp_flush_fdb,
    g_MrpllReqTbl_mrp_ps_set,
    g_MrpllReqTbl_mrp_ps_deinit_mrpi_set,
    g_MrpllReqTbl_mrp_finish_upper,
    g_MrpllReqTbl_mrp_finish_close,
    g_MrpllReqTbl_mrp_prm_commit,
    g_MrpllReqTbl_mrp_init_hsync,
    g_MrpllReqTbl_mrp_deinit_hsync,
    g_MrpllReqTbl_mrp_hsync_set_fdb,
#ifdef MRP_CFG_DELAYED_LINK_UP
    g_MrpllReqTbl_mrp_set_fdb,
#endif
    /* The following MRP interconnection entries are only needed once (general MRP_IN settings)!! */
    g_MrpllReqTbl_mrp_in_init,    /* only called from MRP init */
    g_MrpllReqTbl_mrp_in_deinit,  /* only called from MRP init */

    g_MrpllReqTbl_mrp_in_tst_set_fdb,
//    g_MrpllReqTbl_mrp_in_tst_chg_host,  /* Not used at the moment. */
    g_MrpllReqTbl_mrp_in_ctrl_set_fdb,  
    g_MrpllReqTbl_mrp_in_ctrl_chg_host,
    g_MrpllReqTbl_mrp_in_trf_set_fdb,   /* Not used at the moment. */
    g_MrpllReqTbl_mrp_in_trf_chg_host,
    g_MrpllReqTbl_mrp_in_rstpp_tplg_chg_req,
    LSA_NULL
  };

  array_size = sizeof(pTmpReqTbl) / sizeof(pTmpReqTbl[0]);

  for (cnt = 0; cnt < array_size && pTmpReqTbl[cnt] != LSA_NULL; cnt++)
  {
    ppReqTbl[cnt] = pTmpReqTbl[cnt];
  }

  MRP_ASSERT(cnt < maxCnt);
  LSA_UNUSED_ARG (maxCnt);

  return cnt;
}

/*===========================================================================
* FUNCTION : mrp_ll_get_mrp_base_req_tbl
*----------------------------------------------------------------------------
* PURPOSE  : get lower layer request bursts for mrp base (no plugins)
*----------------------------------------------------------------------------
* RETURNS  : cnt    = number of lower layer request bursts counted
*----------------------------------------------------------------------------
* INPUTS   : ppReqTbl = pointer to array of request burst pointers
*            maxCnt   = max available array entries
* OUTPUTS  : ppReqTbl = updated array of request burst pointers
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_UINT16 mrp_ll_get_mrp_in_base_req_tbl(const MRP_LL_REQ_TBL_T** ppReqTbl, LSA_UINT16 maxCnt)
{
  LSA_UINT16 cnt, array_size;

  array_size = sizeof(pReqTbl_mrp_in) / sizeof(pReqTbl_mrp_in[0]);
  ReqTbl_mrp_in_size = array_size - 1;

  for (cnt = 0; cnt < array_size && pReqTbl_mrp_in[cnt] != LSA_NULL; cnt++)
  {
    ppReqTbl[cnt] = pReqTbl_mrp_in[cnt];
  }

  MRP_ASSERT(cnt < maxCnt);
  LSA_UNUSED_ARG(maxCnt);

  return cnt;
}

/*===========================================================================
* FUNCTION : mrp_ll_burst_state_trace
*----------------------------------------------------------------------------
* PURPOSE  : print out info about pending EDD requests
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            instance = instance, for which the burst is processed
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_ll_burst_state_trace (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance)
{
  LSA_UINT16 i, k, numOfPorts;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  MRP_PROGRAM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_CHAT, "**[inst=%d], EDD_SRV_MULTICAST: **", instance);
  for (i = MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_START; i < MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_END; i++) 
  {
    MRP_EDD_UPPER_MULTICAST_PTR_TYPE pMcast = MRP_EDD_LOWER_GET_PPARAM_MULTICAST(pDBedd->pInstData[instance].pDBshadowRqb[i]);

#if (MRP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pMcast);
#endif

    MRP_PROGRAM_TRACE_07(traceIdx, LSA_TRACE_LEVEL_CHAT, "MCaddr: %2x:%.2x:%.2x:%.2x:%.2x:%.2x Mode=%u",
      pMcast->MACAddr.MacAdr[0],
      pMcast->MACAddr.MacAdr[1],
      pMcast->MACAddr.MacAdr[2],
      pMcast->MACAddr.MacAdr[3],
      pMcast->MACAddr.MacAdr[4],
      pMcast->MACAddr.MacAdr[5],
      pMcast->Mode);
  }

  MRP_PROGRAM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_CHAT, "**[inst=%d], EDD_SRV_MULTICAST_FWD_CTRL: **", instance);
  for (i = MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_TBL_START; i < MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_TBL_END; i++) 
  {
    MRP_EDD_UPPER_MULTICAST_FWD_CTRL_PTR_TYPE pMcastFwdParam = MRP_EDD_LOWER_GET_PPARAM_MULTICAST_FWD_CTRL(pDBedd->pInstData[instance].pDBshadowRqb[i]);

    switch (i) 
    {
    case MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_TST: 
      MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_CHAT, "TST: MACAddrGroup=0x%x MACAddrLow=0x%x",
        pMcastFwdParam->MACAddrGroup, pMcastFwdParam->MACAddrLow);
      break;

    case MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_WRK: 
      MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_CHAT, "WRK: MACAddrGroup=0x%x MACAddrLow=0x%x",
        pMcastFwdParam->MACAddrGroup, pMcastFwdParam->MACAddrLow);
      break;

    case MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_HSYNC:
      MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_CHAT, "HSYNC: MACAddrGroup=0x%x MACAddrLow=0x%x",
                            pMcastFwdParam->MACAddrGroup, pMcastFwdParam->MACAddrLow);
      break;

#ifdef MRP_CFG_PLUGIN_2_HSR
    case MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_HSR_TPLG_CHG: 
      MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_CHAT, "HSR_TPLG_CHG: MACAddrGroup=0x%x MACAddrLow=0x%x",
        pMcastFwdParam->MACAddrGroup, pMcastFwdParam->MACAddrLow);
      break;

    case MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_HSR_TST     : 
      MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_CHAT, "HSR_TST: MACAddrGroup=0x%x MACAddrLow=0x%x",
        pMcastFwdParam->MACAddrGroup, pMcastFwdParam->MACAddrLow);
      break;

    case MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_STBY_PARTNER: 
      MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_CHAT, "STBY_PARTNER: MACAddrGroup=0x%x MACAddrLow=0x%x",
        pMcastFwdParam->MACAddrGroup, pMcastFwdParam->MACAddrLow);
      break;

    case MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_STBY_COM    : 
      MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_CHAT, "STBY_COM: MACAddrGroup=0x%x MACAddrLow=0x%x",
        pMcastFwdParam->MACAddrGroup, pMcastFwdParam->MACAddrLow);
      break;

#endif

    case MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_TST:
      MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_CHAT, "IN_TST: MACAddrGroup=0x%x MACAddrLow=0x%x",
                            pMcastFwdParam->MACAddrGroup, pMcastFwdParam->MACAddrLow);
      break;

    case MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_WRK:
      MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_CHAT, "IN_WRK: MACAddrGroup=0x%x MACAddrLow=0x%x",
                            pMcastFwdParam->MACAddrGroup, pMcastFwdParam->MACAddrLow);
      break;

    case MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_TRF:
      MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_CHAT, "IN_TRF: MACAddrGroup=0x%x MACAddrLow=0x%x",
                            pMcastFwdParam->MACAddrGroup, pMcastFwdParam->MACAddrLow);
      break;

    default:
      MRP_FATAL;
      break;  
    }

    for (k = 0; k < pMcastFwdParam->PortIDModeCnt; k++) 
    {
      MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_CHAT, "DstPortID=%u, SrcPortID=%u, McastFwdMode=%u",
        pMcastFwdParam->pPortIDModeArray[k].DstPortID, pMcastFwdParam->pPortIDModeArray[k].SrcPortID, pMcastFwdParam->pPortIDModeArray[k].Mode);
    }
  }

  MRP_PROGRAM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_CHAT, "**[inst=%d], EDD_SRV_SWITCH_SET_PORT_STATE: **", instance);
  for (i = MRP_LL_SREQ_RQB_ENUM_SET_PS_TBL_START; i < MRP_LL_SREQ_RQB_ENUM_SET_PS_TBL_END; i++) 
  {
    MRP_EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE pPSParam = MRP_EDD_LOWER_GET_PPARAM_SET_PORT_STATE(pDBedd->pInstData[instance].pDBshadowRqb[i]); 
#if (MRP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pPSParam);
#endif

    switch (i) 
    {
    case MRP_LL_SREQ_RQB_ENUM_SET_PS_TPLG_CHG_START:
      MRP_PROGRAM_TRACE_00(traceIdx, LSA_TRACE_LEVEL_CHAT, "TPLG_CHG_START:");
      break;

    case MRP_LL_SREQ_RQB_ENUM_SET_PS_DEINIT:
      MRP_PROGRAM_TRACE_00(traceIdx, LSA_TRACE_LEVEL_CHAT, "DEINIT:");
      break;

    case MRP_LL_SREQ_RQB_ENUM_SET_PS_SHADOW:
      MRP_PROGRAM_TRACE_00(traceIdx, LSA_TRACE_LEVEL_CHAT, "SHADOW:");
      break;

    case MRP_LL_SREQ_RQB_ENUM_SET_PS_DEINIT_MRPI:
      MRP_PROGRAM_TRACE_00(traceIdx, LSA_TRACE_LEVEL_CHAT, "DEINIT_MRPI:");
      break;

#ifdef MRP_CFG_PLUGIN_2_HSR
    case MRP_LL_SREQ_RQB_ENUM_SET_PS_HSR_TPLG_CHG_START:
      MRP_PROGRAM_TRACE_00(traceIdx, LSA_TRACE_LEVEL_CHAT, "HSR_TPLG_CHG_START:");
      break;

    case MRP_LL_SREQ_RQB_ENUM_SET_PS_HSR:
      MRP_PROGRAM_TRACE_00(traceIdx, LSA_TRACE_LEVEL_CHAT, "HSR:");
      break;

    case MRP_LL_SREQ_RQB_ENUM_SET_PS_HSR_ECORI_DE:
      MRP_PROGRAM_TRACE_00(traceIdx, LSA_TRACE_LEVEL_CHAT, "HSR_ECORI_DE:");
      break;

    case MRP_LL_SREQ_RQB_ENUM_SET_PS_HSR_DEINIT:
      MRP_PROGRAM_TRACE_00(traceIdx, LSA_TRACE_LEVEL_CHAT, "HSR_DEINIT:");
      break;
#endif

    default:
      MRP_FATAL;
      break;  
    }

    numOfPorts = mrp_sys_get_num_of_ports (pChSys);
    for (k = 0; k < numOfPorts; k++) 
    {
      MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_CHAT, "Port=%u State=%u", (LSA_UINT16)(k + 1), pPSParam->PortIDState[k]);
    }
  }
  
  MRP_PROGRAM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_CHAT, "**[inst=%d], EDD_SRV_SWITCH_FLUSH_FILTERING_DB: **", instance);
  for (i = MRP_LL_SREQ_RQB_ENUM_FLUSH_FILTERING_DB_TBL_START; i < MRP_LL_SREQ_RQB_ENUM_FLUSH_FILTERING_DB_TBL_END; i++) 
  {
    MRP_EDD_UPPER_SWI_FLUSH_FILTERING_DB_PTR_TYPE pFlushFilterParam = MRP_EDD_LOWER_GET_PPARAM_FLUSH_FILTERING_DB(pDBedd->pInstData[instance].pDBshadowRqb[i]); 

#if (MRP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pFlushFilterParam);
#endif
    switch (i)
    {
    case MRP_LL_SREQ_RQB_ENUM_FLUSH_FILTERING_DB:
      MRP_PROGRAM_TRACE_00(traceIdx, LSA_TRACE_LEVEL_CHAT, "MRP_FLUSH_FILTERING_DB:");
      break;
      
#ifdef MRP_CFG_PLUGIN_2_HSR
    case MRP_LL_SREQ_RQB_ENUM_FLUSH_FILTERING_DB_HSR:
      MRP_PROGRAM_TRACE_00(traceIdx, LSA_TRACE_LEVEL_CHAT, "HSR_FLUSH_FILTERING_DB:");
      break;  
#endif      
    default:
      MRP_PROGRAM_TRACE_00(traceIdx, LSA_TRACE_LEVEL_ERROR, "Invalid enum for FLUSH_FILTERING_DB");
      break;
    }
    
    MRP_PROGRAM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_CHAT, "number of ports to be flushed portCnt=%d", pFlushFilterParam->PortCnt);

    for (k = 0; k < pFlushFilterParam->PortCnt; k++)
    {
      MRP_PROGRAM_TRACE_01(traceIdx, LSA_TRACE_LEVEL_CHAT, "portID=%d to be flushed", pFlushFilterParam->PortID[k]);
    }
  }  
}

/*===========================================================================
* FUNCTION : mrp_ll_burst_trace
*----------------------------------------------------------------------------
* PURPOSE  : print trace massages for burst function
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            state    = burst state (START/FINISH/RESET...)
*            *pReqTbl = request table reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_ll_burst_trace (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_ENUM_BURST_TRACE_TYPE state, const MRP_LL_REQ_TBL_T *pReqTbl) /*lint -e522 */
{
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */
  LSA_UINT16 mrpInst = pInstance[MRP_INSTANCE_INDEX];
  LSA_UINT16 mrpInInst = pInstance[MRP_IN_INSTANCE_INDEX];

#if (MRP_CFG_TRACE_MODE == 0)
  LSA_UNUSED_ARG(state);
  LSA_UNUSED_ARG(pChSys);
  LSA_UNUSED_ARG(pInstance);    /* hhr: to please the compiler in case traces are disabled. */
  LSA_UNUSED_ARG(mrpInst);      /* hhr: to please the compiler in case traces are disabled. */
  LSA_UNUSED_ARG(mrpInInst);    /* hhr: to please the compiler in case traces are disabled. */
#endif
#if (MRP_CFG_TRACE_MODE == 1)
  LSA_UNUSED_ARG(pChSys);
#endif

  if      (LSA_HOST_PTR_ARE_EQUAL(g_MrpllReqTbl_mrp_init_nsm       ,pReqTbl)) 
  {
    MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[mrp_initNSM]", mrpInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL(g_MrpllReqTbl_mrp_deinit_nsm     ,pReqTbl)) 
  {
    MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[mrp_deinitNSM]", mrpInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL(g_MrpllReqTbl_mrp_ind_topo_change      ,pReqTbl)) 
  {
    MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[mrp_indTopoChg]", mrpInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL(g_MrpllReqTbl_mrp_finish_upper   ,pReqTbl)) 
  {
    MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[mrp_finishUpper]", mrpInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL(g_MrpllReqTbl_mrp_finish_close   ,pReqTbl)) 
  {
    MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[mrp_finishClose]", mrpInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL(g_MrpllReqTbl_mrp_ps_set ,pReqTbl)) 
  {
    MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d/inInst=%d], switch cfg %c[mrp_setPS]", mrpInst, mrpInInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL(g_MrpllReqTbl_mrp_ps_deinit_mrpi_set             ,pReqTbl)) 
  {
    MRP_PROGRAM_TRACE_03(traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d/inInst=%d], switch cfg %c[mrp_setPSdeinitMrpi]", mrpInst, mrpInInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL(g_MrpllReqTbl_mrp_prm_commit         ,pReqTbl)) 
  {
    MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[mrp_prmCommit]", mrpInst, state);
  }
#ifdef MRP_CFG_DELAYED_LINK_UP
  else if (LSA_HOST_PTR_ARE_EQUAL(g_MrpllReqTbl_mrp_set_fdb            ,pReqTbl)) 
  {
    MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[mrp_setFDB]", mrpInst, state);
  }
#endif
  else if (LSA_HOST_PTR_ARE_EQUAL(g_MrpllReqTbl_mrp_flush_fdb,pReqTbl))
  {
    MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[mrp_flushFDB]", mrpInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL (g_MrpllReqTbl_mrp_init_hsync,        pReqTbl))
  {
    MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[mrp_initHsync]", mrpInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL (g_MrpllReqTbl_mrp_deinit_hsync,      pReqTbl))
  {
    MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[mrp_deinitHsync]", mrpInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL (g_MrpllReqTbl_mrp_hsync_set_fdb, pReqTbl))
  {
    MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[mrp_hsyncSetFDB]", mrpInst, state);
  }
#ifdef MRP_CFG_PLUGIN_0_MRM
  else if (LSA_HOST_PTR_ARE_EQUAL(g_MrpllReqTbl_mrm_tplg_chg_req       ,pReqTbl)) 
  {
    MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[mrm_tplgChgReq]", mrpInst, state);
  }
#endif
#ifdef MRP_CFG_PLUGIN_1_MRM_EXT
  else if (LSA_HOST_PTR_ARE_EQUAL(g_MrpllReqTbl_mrm_ext_init_nsm_mode_chg,pReqTbl)) 
  {
    MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[mrm_ext_initModeChg]", mrpInst, state);
  }
#endif
#ifdef MRP_CFG_PLUGIN_2_HSR
  else if (LSA_HOST_PTR_ARE_EQUAL(g_MrpllReqTbl_hsr_init_ard       ,pReqTbl)) 
  {
    MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[hsr_initARD]", mrpInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL(g_MrpllReqTbl_hsr_deinit_ard     ,pReqTbl)) 
  {
    MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[hsr_deinitARD]", mrpInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL(g_MrpllReqTbl_hsr_set_fdb_ard    ,pReqTbl)) 
  {
    MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[hsr_setFdbARD]", mrpInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL(g_MrpllReqTbl_hsr_mode_change_ard,pReqTbl)) 
  {
    MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[hsr_modeChgARD]", mrpInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL(g_MrpllReqTbl_hsr_init_rcrm      ,pReqTbl)) 
  {
    MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[hsr_initRCRM]", mrpInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL(g_MrpllReqTbl_hsr_deinit_rcrm    ,pReqTbl)) 
  {
    MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[hsr_deinitRCRM]", mrpInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL(g_MrpllReqTbl_hsr_init_stby      ,pReqTbl)) 
  {
    MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[hsr_initStby]", mrpInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL(g_MrpllReqTbl_hsr_cfg_stby_fdb   ,pReqTbl)) 
  {
    MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[hsr_cfgStbyFdb]", mrpInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL(g_MrpllReqTbl_hsr_tplg_chg       ,pReqTbl)) 
  {
    MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[hsr_tplgChg]", mrpInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL(g_MrpllReqTbl_hsr_mgr_ps         ,pReqTbl)) 
  {
    MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[hsr_psMgr]", mrpInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL(g_MrpllReqTbl_hsr_mgr_ps_ecori_de    ,pReqTbl)) 
  {
    MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[hsr_psMgrEcori_de]", mrpInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL(g_MrpllReqTbl_hsr_stby_ps            ,pReqTbl)) 
  {
    MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[hsr_psStby]", mrpInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL(g_MrpllReqTbl_hsr_tplg_chg_snd       ,pReqTbl)) 
  {
    MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[hsr_tplgChgSnd]", mrpInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL(g_MrpllReqTbl_hsr_mgr_snd_req        ,pReqTbl)) 
  {
    MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d], switch cfg %c[hsr_mgrSndReq]", mrpInst, state);
  }
#endif
  else if (LSA_HOST_PTR_ARE_EQUAL (g_MrpllReqTbl_mrp_in_init, pReqTbl))
  {
    /* not dependent on the inInstances */
    /* has to be called when MRP instance 0 is started */
    MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d/inInst=%d], switch cfg %c[mrp_inInit], MRP inst has to be 0 (independent of inInst)", mrpInst, mrpInInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL (g_MrpllReqTbl_mrp_in_init_mic, pReqTbl))
  {
    MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d/inInst=%d], switch cfg %c[mrp_inInitMIC]", mrpInst, mrpInInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL (g_MrpllReqTbl_mrp_in_init_mim, pReqTbl))
  {
    MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d/inInst=%d], switch cfg %c[mrp_inInitMIM]", mrpInst, mrpInInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL (g_MrpllReqTbl_mrp_in_deinit, pReqTbl))
  {
    /* not dependent on the inInstances, it is controlled by the MRP instance. */
    /* has to be called when MRP instance 0 is started */
    MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d/inInst=%d], switch cfg %c[mrp_inDeinit], MRP inst has to be 0 (independent of inInst)", mrpInst, mrpInInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL (g_MrpllReqTbl_mrp_in_ctrl_set_fdb, pReqTbl))
  {
    /* not dependent on the inInstances */
    MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d/inInst=%d], switch cfg %c[mrp_inCtrlSetFdb], MRP inst has to be 0 (independend on inInst)", mrpInst, mrpInInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL (g_MrpllReqTbl_mrp_in_ctrl_chg_host, pReqTbl))
  {
    MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d/inInst=%d], switch cfg %c[mrp_inCtrlChgHost]", mrpInst, mrpInInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL (g_MrpllReqTbl_mrp_in_trf_chg_host, pReqTbl))
  {
    /* not dependent on the inInstances */
    MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d/inInst=%d], switch cfg %c[mrp_inTrfChgHost], MRP inst has to be 0 (independent of inInst)", pInstance[MRP_INSTANCE_INDEX], pInstance[MRP_IN_INSTANCE_INDEX], state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL (g_MrpllReqTbl_mrp_in_trf_set_fdb, pReqTbl))
  {
    /* not yet used */
    MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d/inInst=%d], switch cfg %c[mrp_inTrfSetFdb]", mrpInst, mrpInInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL (g_MrpllReqTbl_mrp_in_rstpp_tplg_chg_req, pReqTbl))
  {
    MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d/inInst=%d], switch cfg %c[mrm_inRstppTplgChgReq]", mrpInst, mrpInInst, state);
  }
  else if (LSA_HOST_PTR_ARE_EQUAL (g_MrpllReqTbl_mrp_in_mim_tplg_chg_req, pReqTbl))
  {
    MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d/inInst=%d], switch cfg %c[mrm_inMimTplgChgReq]", mrpInst, mrpInInst, state);
  }
}


/*===========================================================================
* FUNCTION : mrp_ll_burst_do
*----------------------------------------------------------------------------
* PURPOSE  : do lower layer requests to EDD for several instances
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys = channel reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_ll_burst_do (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys)
{
  LSA_UINT16 mrpMaxInstance;
  MRP_LL_BURST_DB_T *pPendingBurst;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  mrpMaxInstance = mrp_sys_get_num_of_instances(pChSys);

  if (mrpMaxInstance)
  {
    MRP_ASSERT(!LSA_HOST_PTR_ARE_EQUAL(pDBedd->llQueue.pPendingLLreq, LSA_NULL));
    pDBedd->llQueue.pPendingLLreq++;
    
    /* burst finished ? */
    if (0 == pDBedd->llQueue.pPendingLLreq->cbf) 
    {
      /* burst was reset? */
      if (!pDBedd->llQueue.pPendingLLburst->reset) 
      {
        pPendingBurst = pDBedd->llQueue.pPendingLLburst;
        mrp_ll_burst_trace(pChSys, pPendingBurst->pInstance, MRP_LL_ENUM_BURST_TRACE_FINISH, pPendingBurst->pReqTblBegin);
        /* get next from queue */
        if (LSA_HOST_PTR_ARE_EQUAL(pPendingBurst->pNext, LSA_NULL)) 
        {
          mrp_ll_burst_state_trace(pChSys, pPendingBurst->pInstance[MRP_INSTANCE_INDEX]);
          pDBedd->llQueue.pPendingLLburst = LSA_NULL;
          return;
        }
        /* set next burst as active and pending burst */
        pDBedd->llQueue.pPendingLLburst = pPendingBurst->pNext;
        pDBedd->llQueue.pPendingLLreq = pPendingBurst->pNext->pReqTblBegin;
        /* reset some entries of previous burst */
        pPendingBurst->pNext->queued = LSA_FALSE;
        pPendingBurst->pNext = LSA_NULL;
      }
      else 
      {
        /* again */
        pDBedd->llQueue.pPendingLLburst->reset = LSA_FALSE;
        pDBedd->llQueue.pPendingLLreq = pDBedd->llQueue.pPendingLLburst->pReqTblBegin;
        mrp_ll_burst_trace(pChSys, pDBedd->llQueue.pPendingLLburst->pInstance, MRP_LL_ENUM_BURST_TRACE_RESET, pDBedd->llQueue.pPendingLLburst->pReqTblBegin);
      }
      pDBedd->llQueue.pPendingLLreq++;
    }
    
    if (pDBedd->llQueue.pPendingLLreq->rqbNum >= MRP_LL_SREQ_RQB_ENUM_TBL_END) 
    {
      pDBedd->llQueue.pPendingLLreq->cbf(pChSys, pDBedd->llQueue.pPendingLLburst->pInstance, MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_START);
    }
    else 
    {
      pDBedd->llQueue.pPendingLLreq->cbf(pChSys, pDBedd->llQueue.pPendingLLburst->pInstance, pDBedd->llQueue.pPendingLLreq->rqbNum);
    }
  }
}


/*===========================================================================
* FUNCTION : mrp_ll_burst_req
*----------------------------------------------------------------------------
* PURPOSE  : do several edd requests according to referenced table
*----------------------------------------------------------------------------
* RETURNS  : 
*            
*----------------------------------------------------------------------------
* INPUTS   : pChSys       = channel reference
*            instance     = instance, for which the burst is processed
*            *pReqTbl     = reference to request sequence
*            resetAllowed = allow reset if this burst is at work
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : functions grouped to edd request sets called "burst". 
*            If a burst is running than new request is queued.
*==========================================================================*/
LSA_VOID mrp_ll_in_burst_req (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, const MRP_LL_REQ_TBL_T *pReqTbl, LSA_UINT8 resetAllowed)
{
  MRP_LL_BURST_DB_T *pPendingBurst, *pNewBurst;
  LSA_UINT16 i, tableIndex, mrpMaxInstance, tempInInstReqTable = LSA_FALSE;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  mrpMaxInstance = mrp_sys_get_num_of_instances(pChSys);
  
  if (mrpMaxInstance)
  {
    /* search for burst in tbl */
    for (i = 0; i < pDBedd->llBurstMaxCnt; i++)
    {
      if (LSA_HOST_PTR_ARE_EQUAL (pReqTbl, pDBedd->pInstData[pInstance[MRP_INSTANCE_INDEX]].pDBllBurst[i].pReqTblBegin))
      {
        /* according burst table was found. */

        if (MRP_IN_INSTANCE_NONE != pInstance[MRP_IN_INSTANCE_INDEX]) /* if an interconnection instance is active the correct IN burst needs to be taken */
        {
          for (tableIndex = 0; pReqTbl_mrp_in[tableIndex] != LSA_NULL; tableIndex++)
          {
            if (pReqTbl == pReqTbl_mrp_in[tableIndex])
            {
              /* Flag for MRP interconnection instance based burst table. */
              tempInInstReqTable = LSA_TRUE;
              /* correct the burst index to the correct MRP interconnection instance. */
              i += (LSA_UINT16)(pInstance[MRP_IN_INSTANCE_INDEX] * ReqTbl_mrp_in_size);
              break;
            }
          }
        }

        break;
      }
    }

    MRP_ASSERT(i < pDBedd->llBurstMaxCnt);

    pNewBurst = &pDBedd->pInstData[pInstance[MRP_INSTANCE_INDEX]].pDBllBurst[i];

    /* set the according inInstance for the burst */
    pNewBurst->pInstance[MRP_IN_INSTANCE_INDEX] = pInstance[MRP_IN_INSTANCE_INDEX];

    /* any burst working ? */
    if (LSA_HOST_PTR_ARE_EQUAL(pDBedd->llQueue.pPendingLLburst, LSA_NULL)) 
    {
      /* NO burst is the queue!! */
      pDBedd->llQueue.pPendingLLburst = pNewBurst;
      /* do request */
      pDBedd->llQueue.pPendingLLreq = pDBedd->llQueue.pPendingLLburst->pReqTblBegin;
      mrp_ll_burst_trace(pChSys, pInstance, MRP_LL_ENUM_BURST_TRACE_START, pDBedd->llQueue.pPendingLLburst->pReqTblBegin);
      mrp_ll_burst_do(pChSys);
      return;
    }
    /* this burst working ? */
    if (LSA_HOST_PTR_ARE_EQUAL(pDBedd->llQueue.pPendingLLburst, pNewBurst)) 
    {
      /* YES the current burst is in already in the LL queue!! */
      if (resetAllowed) 
      {
        pDBedd->llQueue.pPendingLLburst->reset = LSA_TRUE;
      }
      else 
      {
        mrp_ll_burst_trace(pChSys, pInstance, MRP_LL_ENUM_BURST_TRACE_DENIED, pDBedd->llQueue.pPendingLLburst->pReqTblBegin);
      }
      return;
    }
    /* this burst queued ? */
    if (pNewBurst->queued) 
    {
      MRP_PROGRAM_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "[inst=%d/inInst=%d], This burst was already queued!!! ", pInstance[MRP_INSTANCE_INDEX], pInstance[MRP_IN_INSTANCE_INDEX]);
      return;
    }


    mrp_ll_burst_trace(pChSys, pInstance, MRP_LL_ENUM_BURST_TRACE_WORKING, pDBedd->llQueue.pPendingLLburst->pReqTblBegin);
    /* queue request */
    i = 0;
    pPendingBurst = pDBedd->llQueue.pPendingLLburst;
    while (!LSA_HOST_PTR_ARE_EQUAL(pPendingBurst->pNext, LSA_NULL)) 
    { 
      if (LSA_HOST_PTR_ARE_EQUAL(pPendingBurst->pNext->pReqTblBegin, pReqTbl))  /* Burst entry already exists! */
      {
        if (tempInInstReqTable) /* Check if MRP interconnection instance based burst table. */
        {
          /* MRP interconnection instance based burst table. */
          MRP_ASSERT(pPendingBurst->pNext->pInstance[MRP_IN_INSTANCE_INDEX] != pInstance[MRP_IN_INSTANCE_INDEX]); /* Assert if burst table entry already exists. */
        } 
        else
        {
          /* MRP instance based burst table. */
          MRP_ASSERT(pPendingBurst->pNext->pInstance[MRP_INSTANCE_INDEX] != pInstance[MRP_INSTANCE_INDEX]); /* Assert if burst table entry already exists. */
        }
      }
      pPendingBurst = pPendingBurst->pNext;
      i++;
      MRP_ASSERT(i < pDBedd->llBurstMaxCnt);
    }
    mrp_ll_burst_trace(pChSys, pInstance, MRP_LL_ENUM_BURST_TRACE_QUEUED, pNewBurst->pReqTblBegin);
    pNewBurst->queued = LSA_TRUE;
    pPendingBurst->pNext = pNewBurst;  
  }

  return;
}

/*===========================================================================
* FUNCTION : mrp_ll_add_user_reference
*----------------------------------------------------------------------------
* PURPOSE  : add a user channel reference to pDBedd
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            pathType = user, user_2
*            pChUsr   = user channel reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_ll_add_user_reference (const MRP_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 pathType, const MRP_CHANNEL_TYPE_USER pChUsr)
{
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  LSA_UINT16 i;

  switch (pathType)
  {
  case MRP_PATH_TYPE_USER:
    for (i = 0; i < MRP_MAX_USER_CHANNELS_PER_IF; i++)
    {
      if (pDBedd->pChUsr[i] == LSA_NULL)
      {
        pDBedd->pChUsr[i] = pChUsr;
        break;
      }
    }
    if (i >= MRP_MAX_USER_CHANNELS_PER_IF)
    {
      MRP_LOWER_TRACE_00(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_ll_add_user_reference: no free user chan. entry found");
    }
    break;

  case MRP_PATH_TYPE_USER_2:
    MRP_ASSERT( (!LSA_HOST_PTR_ARE_EQUAL(pChUsr, LSA_NULL) &&  LSA_HOST_PTR_ARE_EQUAL(pDBedd->pChPrivUsr, LSA_NULL) ) ||
                ( LSA_HOST_PTR_ARE_EQUAL(pChUsr, LSA_NULL) && !LSA_HOST_PTR_ARE_EQUAL(pDBedd->pChPrivUsr, LSA_NULL) )
              );
    pDBedd->pChPrivUsr = pChUsr;
    break;

  default:
    MRP_FATAL;
    break;
  }
} /*lint !e818 CM 22/11/2016 can not made const, because of request burst */

/*===========================================================================
* FUNCTION : mrp_ll_remove_user_reference
*----------------------------------------------------------------------------
* PURPOSE  : remove the reference of a user channel in pDBedd
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            pathType = user, user_2
*            pChUsr   = user channel reference
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_ll_remove_user_reference (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 pathType, const MRP_CONST_CHANNEL_TYPE_USER pChUsr)
{
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  LSA_UINT16 i;

  switch (pathType)
  {
  case MRP_PATH_TYPE_USER:
    for (i = 0; i < MRP_MAX_USER_CHANNELS_PER_IF; i++)
    {
      if (pDBedd->pChUsr[i] == pChUsr)
      {
        pDBedd->pChUsr[i] = LSA_NULL;
        break;
      }
    }
    if (i >= MRP_MAX_USER_CHANNELS_PER_IF)
    {
      MRP_LOWER_TRACE_00(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_ll_remove_user_reference: user chan. entry NOT found");
    }
    break;

  case MRP_PATH_TYPE_USER_2:
    pDBedd->pChPrivUsr = LSA_NULL;
    break;

  default:
    MRP_FATAL;
    break;
  }
}

/*===========================================================================
* FUNCTION : mrp_ll_event_handler
*----------------------------------------------------------------------------
* PURPOSE  : common event handler, which handles diagnosis and state indications
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            instance = affected MRP/HSR instance
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : signal state changes form MRM/MRC.
*==========================================================================*/
LSA_VOID mrp_ll_event_handler (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance)
{
  LSA_UINT16 i;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
#ifdef MRP_CFG_PLUGIN_2_HSR
  MRP_ARD_DB_TYPE *pDBard = (MRP_ARD_DB_TYPE *)pChSys->pChSysDB->pArd;
#endif

  MRP_ASSERT(!LSA_HOST_PTR_ARE_EQUAL(pDBedd, LSA_NULL));

  for (i = 0; i < MRP_MAX_USER_CHANNELS_PER_IF; i++)
  {
    if (!LSA_HOST_PTR_ARE_EQUAL(pDBedd->pChUsr[i], LSA_NULL))
      mrp_rec_event_req (pDBedd->pChUsr[i], instance);
  }

  if (!LSA_HOST_PTR_ARE_EQUAL(pDBedd->pChPrivUsr, LSA_NULL))
  {
    if (instance)
    {
      (void)(mrp_usr_state_ind_done (pDBedd->pChPrivUsr, instance));
    }
    else
    {
#ifdef MRP_CFG_PLUGIN_2_HSR
      if (!(pDBard->blockAlarm))
#endif
        (void)(mrp_usr_state_ind_done (pDBedd->pChPrivUsr, instance));
    }
  }
}

/*===========================================================================
* FUNCTION : mrp_ll_red_state_handler
*----------------------------------------------------------------------------
* PURPOSE  : signaling the redundancy state of a redundancy instance
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            instance = affected MRP/HSR instance
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : signal state changes from MRM/MRC/HSR.
*==========================================================================*/
LSA_VOID mrp_ll_red_state_handler (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance)
{
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  MRP_ASSERT(!LSA_HOST_PTR_ARE_EQUAL(pDBedd, LSA_NULL));

  if (!LSA_HOST_PTR_ARE_EQUAL(pDBedd->pChPrivUsr, LSA_NULL))
  {
    (void)(mrp_usr_red_state_ind_done (pDBedd->pChPrivUsr, instance));
  }
}

/*===========================================================================
* FUNCTION : mrp_ll_set_port_state_param
*----------------------------------------------------------------------------
* PURPOSE  : set port state param to requested state
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pPSParam  = pointer to internal port state param struct
*            port      = portId
*            portState = portState, which shall be set
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_ll_set_port_state_param (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, MRP_EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE pPSParam, LSA_UINT16 port, MRP_PORT_STATE_TYPE portState)
{
  LSA_UINT16 eddPS;

#if (MRP_CFG_TRACE_MODE == 0)
  LSA_UNUSED_ARG(pChSys);
#endif
#if (MRP_CFG_TRACE_MODE == 1)
  LSA_UNUSED_ARG(pChSys);
#endif

  eddPS = EDD_PORT_STATE_FORWARDING; 

  switch (portState) 
  {
  case MRP_PORT_STATE_FORWARD: 
    MRP_PROGRAM_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_ll_set_port_state_param port=%u, state=FORWARD", port);
    /* eddPS=EDD_PORT_STATE_FORWARDING; */
    break;

  case MRP_PORT_STATE_BLOCKED: 
    MRP_PROGRAM_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_ll_set_port_state_param port=%u, state=BLOCKED", port);
    eddPS = EDD_PORT_STATE_BLOCKING; 
    break;

  case MRP_PORT_STATE_DISABLED: 
    MRP_PROGRAM_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_ll_set_port_state_param port=%u, state=DISABLED", port);
    eddPS = EDD_PORT_STATE_DISABLE;
    break;

  default: 
    MRP_ASSERT(0); 
    break;
  }
  pPSParam->PortIDState[port - 1] = eddPS;
}


/*===========================================================================
* FUNCTION : mrp_ll_mc_mode_set
*----------------------------------------------------------------------------
* PURPOSE  : change multicast mode to new mode
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : mcMode = rqb and mode for test and work multicast rqb
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/

LSA_VOID mrp_ll_mc_mode_set (const MRP_MC_MODE_DATA_TYPE *mcMode)
{
  MRP_EDD_UPPER_MULTICAST_PTR_TYPE pMulticast;

  if (mcMode->pTestRqb)
  {
    pMulticast =  MRP_EDD_LOWER_GET_PPARAM_MULTICAST(mcMode->pTestRqb);
    pMulticast->Mode = mcMode->modeTest;
  }

  if (mcMode->pWorkRqb)
  {
    pMulticast =  MRP_EDD_LOWER_GET_PPARAM_MULTICAST(mcMode->pWorkRqb);
    pMulticast->Mode = mcMode->modeWork;
  }

  if (mcMode->pTrfRqb)
  {
    pMulticast =  MRP_EDD_LOWER_GET_PPARAM_MULTICAST(mcMode->pTrfRqb);
    pMulticast->Mode = mcMode->modeTrf;
  }
}



/*===========================================================================
* FUNCTION : mrp_ll_fwd_ctrl_chg_rports
*----------------------------------------------------------------------------
* PURPOSE  : change fwd ctrl for rports to fwdMode
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            pFwdCtrl = pointer to fwdCtrl data (rqbs,...)
*            rport_1, rport2 = ring ports. If rport == 0 than forward disabled.
*            fwdMode  = EDD_MULTICAST_FWD_DISABLE or EDD_MULTICAST_FWD_ENABLE
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_ll_fwd_ctrl_chg_rports (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const MRP_FWD_CTRL_DATA_TYPE *pFwdCtrl, LSA_UINT16 rport_1, LSA_UINT16 rport_2, LSA_UINT16 fwdMode)
{
  MRP_EDD_UPPER_MULTICAST_FWD_CTRL_PTR_TYPE pMcastFwdCtrl;
  LSA_UINT16 portIdx_1;
  LSA_UINT16 portIdx_2, numOfPorts;
  LSA_INT32 k;

  portIdx_1 = 0xFFFF;
  portIdx_2 = 0xFFFF;

  if (rport_1)
    portIdx_1 = (LSA_UINT16)(rport_1 - 1);

  if (rport_2)
    portIdx_2 = (LSA_UINT16)(rport_2 - 1);

  numOfPorts = mrp_sys_get_num_of_ports (pChSys);

  MRP_LOWER_TRACE_03 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_ll_fwd_ctrl_chg_rports: DISABLE FWD=%d, for rport1=%d, rport2=%d queued", fwdMode, rport_1, rport_2);

  if (pFwdCtrl->pWorkRqb)
  {
    /**/
    pMcastFwdCtrl = MRP_EDD_LOWER_GET_PPARAM_MULTICAST_FWD_CTRL(pFwdCtrl->pWorkRqb);
    MRP_ASSERT(!LSA_HOST_PTR_ARE_EQUAL(pMcastFwdCtrl, LSA_NULL));

    for (k = 0; k < numOfPorts; k++) 
    {
      if (portIdx_1 == k || portIdx_2 == k) 
      {
        pMcastFwdCtrl->pPortIDModeArray[k].Mode = fwdMode;
      }
    }
    pMcastFwdCtrl->MACAddrPrio = pFwdCtrl->macAddrPrio;
  }

  if (pFwdCtrl->pTestRqb)
  {
    /**/
    pMcastFwdCtrl = MRP_EDD_LOWER_GET_PPARAM_MULTICAST_FWD_CTRL(pFwdCtrl->pTestRqb);
    MRP_ASSERT(!LSA_HOST_PTR_ARE_EQUAL(pMcastFwdCtrl, LSA_NULL));

    for (k = 0; k < numOfPorts; k++) 
    {
      if (portIdx_1 == k || portIdx_2 == k) 
      {
        pMcastFwdCtrl->pPortIDModeArray[k].Mode = fwdMode;
      }
    }
    pMcastFwdCtrl->MACAddrPrio = pFwdCtrl->macAddrPrio;
  }

  if (pFwdCtrl->pTrfRqb)
  {
    /* This function ist only used for HSR. So no need to do anything for Transfer (TRF) frames for MRP_Interconnection. */
  }
}


/*===========================================================================
* FUNCTION : mrp_ll_fwd_ctrl_chg_all_ports
*----------------------------------------------------------------------------
* PURPOSE  : change fwd ctrl for any port to fwdMode
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            pFwdCtrl = pointer to fwdCtrl data (rqbs,...)
*            fwdMode  = EDD_MULTICAST_FWD_DISABLE or EDD_MULTICAST_FWD_ENABLE
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_ll_fwd_ctrl_chg_all_ports (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const MRP_FWD_CTRL_DATA_TYPE *pFwdCtrl, LSA_UINT16 fwdMode)
{
  MRP_EDD_UPPER_MULTICAST_FWD_CTRL_PTR_TYPE pMcastFwdCtrl;
  LSA_UINT16 k, numOfPorts;

  MRP_LOWER_TRACE_01 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_ll_fwd_ctrl_chg_all_ports: DISABLE FWD=%d for any port queued", fwdMode);

  numOfPorts = mrp_sys_get_num_of_ports (pChSys);

  if (pFwdCtrl->pTestRqb)
  {
    pMcastFwdCtrl = MRP_EDD_LOWER_GET_PPARAM_MULTICAST_FWD_CTRL (pFwdCtrl->pTestRqb);
    MRP_ASSERT (!LSA_HOST_PTR_ARE_EQUAL (pMcastFwdCtrl, LSA_NULL));

    for (k = 0; k < numOfPorts; k++)
    {
      pMcastFwdCtrl->pPortIDModeArray[k].Mode = fwdMode;
    }
    pMcastFwdCtrl->MACAddrPrio = pFwdCtrl->macAddrPrio;
  }

  if (pFwdCtrl->pWorkRqb)
  {
    pMcastFwdCtrl = MRP_EDD_LOWER_GET_PPARAM_MULTICAST_FWD_CTRL (pFwdCtrl->pWorkRqb);
    MRP_ASSERT (!LSA_HOST_PTR_ARE_EQUAL (pMcastFwdCtrl, LSA_NULL));

    for (k = 0; k < numOfPorts; k++)
    {
      pMcastFwdCtrl->pPortIDModeArray[k].Mode = fwdMode;
    }
    pMcastFwdCtrl->MACAddrPrio = pFwdCtrl->macAddrPrio;
  }

  if (pFwdCtrl->pTrfRqb)
  {
    pMcastFwdCtrl = MRP_EDD_LOWER_GET_PPARAM_MULTICAST_FWD_CTRL (pFwdCtrl->pTrfRqb);
    MRP_ASSERT (!LSA_HOST_PTR_ARE_EQUAL (pMcastFwdCtrl, LSA_NULL));

    for (k = 0; k < numOfPorts; k++)
    {
      pMcastFwdCtrl->pPortIDModeArray[k].Mode = fwdMode;
    }
    pMcastFwdCtrl->MACAddrPrio = pFwdCtrl->macAddrPrio;
  }
}


/*===========================================================================
* FUNCTION : mrp_ll_fwd_ctrl_chg_all_ports_dis_rports_mode_ena
*----------------------------------------------------------------------------
* PURPOSE  : enable fwd ctrl for rports, if mode is set,
*            disable fwd ctrl for any other port
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            pFwdCtrl = pointer to fwdCtrl data (rqbs,...)
*            rport_1, rport2 = ring ports. If rport == 0 than forward disabled.
*            mode     = LSA_TRUE (if RC) or LSA_FALSE (if RM)
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_ll_fwd_ctrl_chg_all_ports_dis_rports_mode_ena (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const MRP_FWD_CTRL_DATA_TYPE *pFwdCtrl, LSA_UINT16 rport_1, LSA_UINT16 rport_2, LSA_UINT8 mode)
{
  MRP_EDD_UPPER_MULTICAST_FWD_CTRL_PTR_TYPE pMcastFwdCtrl;
  LSA_UINT16 portIdx_1;
  LSA_UINT16 portIdx_2;
  LSA_UINT16 k, numOfPorts;

  portIdx_1 = 0xFFFF;
  portIdx_2 = 0xFFFF;

  if (rport_1)
    portIdx_1 = (LSA_UINT16)(rport_1 - 1);

  if (rport_2)
    portIdx_2 = (LSA_UINT16)(rport_2 - 1);

  numOfPorts = mrp_sys_get_num_of_ports (pChSys);

  MRP_LOWER_TRACE_03 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_ll_fwd_ctrl_chg_all_ports_dis_rports_mode_ena: mode=%d, FWD for any port=DIS, (rport1=%d/rport2=%d)=ENA queued", mode, rport_1, rport_2);

  if (pFwdCtrl->pWorkRqb)
  {
    /**/
    pMcastFwdCtrl = MRP_EDD_LOWER_GET_PPARAM_MULTICAST_FWD_CTRL (pFwdCtrl->pWorkRqb);
    MRP_ASSERT (!LSA_HOST_PTR_ARE_EQUAL (pMcastFwdCtrl, LSA_NULL));

    for (k = 0; k < numOfPorts; k++)
    {
      pMcastFwdCtrl->pPortIDModeArray[k].Mode = EDD_MULTICAST_FWD_DISABLE;
      if (mode && (portIdx_1 == k || portIdx_2 == k))
      {
        pMcastFwdCtrl->pPortIDModeArray[k].Mode = EDD_MULTICAST_FWD_ENABLE;
      }
    }
    pMcastFwdCtrl->MACAddrPrio = pFwdCtrl->macAddrPrio;
  }

  if (pFwdCtrl->pTestRqb)
  {
    /**/
    pMcastFwdCtrl = MRP_EDD_LOWER_GET_PPARAM_MULTICAST_FWD_CTRL (pFwdCtrl->pTestRqb);
    MRP_ASSERT (!LSA_HOST_PTR_ARE_EQUAL (pMcastFwdCtrl, LSA_NULL));

    for (k = 0; k < numOfPorts; k++)
    {
      pMcastFwdCtrl->pPortIDModeArray[k].Mode = EDD_MULTICAST_FWD_DISABLE;
      if (mode && (portIdx_1 == k || portIdx_2 == k))
      {
        pMcastFwdCtrl->pPortIDModeArray[k].Mode = EDD_MULTICAST_FWD_ENABLE;
      }
    }
    pMcastFwdCtrl->MACAddrPrio = pFwdCtrl->macAddrPrio;
  }

  if (pFwdCtrl->pTrfRqb)
  {
    /* This function ist only used for HSR. So no need to do anything for Transfer (TRF) frames for MRP_Interconnection. */
  }
}


/*===========================================================================
* FUNCTION : mrp_ll_fwd_ctrl_chg_all_ports_dis_rports_ena
*----------------------------------------------------------------------------
* PURPOSE  : enable fwd ctrl for rports, disable fwd ctrl for any other port
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            pFwdCtrl = pointer to fwdCtrl data (rqbs,...)
*            rport_1, rport2 = ring ports. If rport == 0 than forward disabled.
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_ll_fwd_ctrl_chg_all_ports_dis_rports_ena (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const MRP_FWD_CTRL_DATA_TYPE *pFwdCtrl, LSA_UINT16 rport_1, LSA_UINT16 rport_2)
{
  MRP_EDD_UPPER_MULTICAST_FWD_CTRL_PTR_TYPE pMcastFwdCtrl;
  LSA_UINT16 portIdx_1;
  LSA_UINT16 portIdx_2;
  LSA_UINT16 k, numOfPorts;

  portIdx_1 = 0xFFFF;
  portIdx_2 = 0xFFFF;

  if (rport_1)
    portIdx_1 = (LSA_UINT16)(rport_1 - 1);

  if (rport_2)
    portIdx_2 = (LSA_UINT16)(rport_2 - 1);

  numOfPorts = mrp_sys_get_num_of_ports (pChSys);

  MRP_LOWER_TRACE_02 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_ll_fwd_ctrl_chg_all_ports_dis_rports_ena: FWD for any port=DIS, (rport1=%d/rport2=%d)=ENA queued", rport_1, rport_2);

  if (pFwdCtrl->pWorkRqb)
  {
    pMcastFwdCtrl = MRP_EDD_LOWER_GET_PPARAM_MULTICAST_FWD_CTRL (pFwdCtrl->pWorkRqb);
    MRP_ASSERT (!LSA_HOST_PTR_ARE_EQUAL (pMcastFwdCtrl, LSA_NULL));

    for (k = 0; k < numOfPorts; k++)
    {
      pMcastFwdCtrl->pPortIDModeArray[k].Mode = EDD_MULTICAST_FWD_DISABLE;
      if (portIdx_1 == k || portIdx_2 == k)
      {
        pMcastFwdCtrl->pPortIDModeArray[k].Mode = EDD_MULTICAST_FWD_ENABLE;
      }
    }
    pMcastFwdCtrl->MACAddrPrio = pFwdCtrl->macAddrPrio;
  }

  if (pFwdCtrl->pTestRqb)
  {
    pMcastFwdCtrl = MRP_EDD_LOWER_GET_PPARAM_MULTICAST_FWD_CTRL (pFwdCtrl->pTestRqb);
    MRP_ASSERT (!LSA_HOST_PTR_ARE_EQUAL (pMcastFwdCtrl, LSA_NULL));

    for (k = 0; k < numOfPorts; k++)
    {
      pMcastFwdCtrl->pPortIDModeArray[k].Mode = EDD_MULTICAST_FWD_DISABLE;
      if (portIdx_1 == k || portIdx_2 == k)
      {
        pMcastFwdCtrl->pPortIDModeArray[k].Mode = EDD_MULTICAST_FWD_ENABLE;
      }
    }
    pMcastFwdCtrl->MACAddrPrio = pFwdCtrl->macAddrPrio;
  }

  if (pFwdCtrl->pTrfRqb)
  {
    pMcastFwdCtrl = MRP_EDD_LOWER_GET_PPARAM_MULTICAST_FWD_CTRL (pFwdCtrl->pTrfRqb);
    MRP_ASSERT (!LSA_HOST_PTR_ARE_EQUAL (pMcastFwdCtrl, LSA_NULL));

    for (k = 0; k < numOfPorts; k++)
    {
      pMcastFwdCtrl->pPortIDModeArray[k].Mode = EDD_MULTICAST_FWD_DISABLE;
      if (portIdx_1 == k || portIdx_2 == k)
      {
        pMcastFwdCtrl->pPortIDModeArray[k].Mode = EDD_MULTICAST_FWD_ENABLE;
      }
    }
    pMcastFwdCtrl->MACAddrPrio = pFwdCtrl->macAddrPrio;
  }
}

/*===========================================================================
* FUNCTION : mrp_ll_clear_fdb
*----------------------------------------------------------------------------
* PURPOSE  : start clear fdb
*----------------------------------------------------------------------------
* RETURNS  : isFlushInitiated = timer to flush FDB is started initially
*----------------------------------------------------------------------------
* INPUTS   : pChSys         = channel reference for edd system channel
*            instance       = affected MRP instance
*            durationTicks  = time in ticks or MRP_VAL_STOP_CLR_FDB_T_CNT to stop clearing of FDB
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_UINT8 mrp_ll_clear_fdb (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT32 durationTicks)
{
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  LSA_UINT32 intervalTime_ms;
  LSA_UINT8 isFlushInitiated;

  intervalTime_ms = (LSA_UINT32)(durationTicks * MRP_TIMER_TIME_BASE_DEZ);
  isFlushInitiated = LSA_FALSE;

  LSA_UNUSED_ARG(intervalTime_ms);   /* hhr: to please the compiler in case traces are disabled. */
  LSA_UNUSED_ARG(isFlushInitiated);   /* hhr: to please the compiler in case traces are disabled. */

  MRP_PROGRAM_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_ll_clear_fdb[inst=%d] time=%u ms", instance, intervalTime_ms);

  mrp_sys_timer_stop(pDBedd->pInstData[instance].TimerIdTplgChg);

  if (durationTicks == MRP_VAL_STOP_CLR_FDB_T_CNT) 
  {
    MRP_PROGRAM_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_ll_clear_fdb stopped[inst=%d]", instance);
    pDBedd->pInstData[instance].TplgChgRunning = LSA_FALSE;
  }
  else
  {
    if (!pDBedd->pInstData[instance].TplgChgRunning)
    {
      isFlushInitiated = LSA_TRUE;
      pDBedd->pInstData[instance].TplgChgRunning = LSA_TRUE;
    }

    if (durationTicks)
    {
#ifdef MRP_CFG_PERF_TRACE
      if (intervalTime_ms == 20)
      {
        pDBedd->pInstData[instance].perfCapture.endTime = ERTEC_TIME;
        pDBedd->pInstData[instance].perfCapture.timeDiff = pDBedd->pInstData[instance].perfCapture.endTime - pDBedd->pInstData[instance].perfCapture.startTime;
        pDBedd->pInstData[instance].perfCapture.startTime = pDBedd->pInstData[instance].perfCapture.endTime;

        MRP_PROGRAM_TRACE_03 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_FATAL, "{@@@ T3CA[inst=%d] (timerBegin) @@@}[%d] mrp_ll_clear_fdb (time=%d ns)",
                              instance, pDBedd->pInstData[instance].perfCapture.endTime, (pDBedd->pInstData[instance].perfCapture.timeDiff * 10));
      }
#endif
      mrp_sys_timer_start (pDBedd->pInstData[instance].TimerIdTplgChg, durationTicks);
    }
    else
    {
      mrp_ll_burst_req (pChSys, instance, g_MrpllReqTbl_mrp_flush_fdb, LSA_FALSE);
    }
  }
  return isFlushInitiated;
}

/*===========================================================================
* FUNCTION : mrp_ll_fdb_hsync_cfg
*----------------------------------------------------------------------------
* PURPOSE  : set fdb for high-availability MC forwarding
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            instance = affected MRP instance
*            pFwdCtrl  = pointer to fwdCtrl data (rqbs,...)
*            rport_1, rport2 = ring ports. If rport == 0 than forward disabled.
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_ll_fdb_hsync_cfg (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, const MRP_FWD_CTRL_DATA_TYPE *pFwdCtrl, LSA_UINT16 rport_1, LSA_UINT16 rport_2)
{
  MRP_FWD_CTRL_DATA_TYPE fwdCtrl;
  MRP_HA_ADMIN_ROLE_ENUM_T hsyncAdminRole;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  hsyncAdminRole = mrp_sys_get_ha_admin_role (pChSys);

  if (hsyncAdminRole == MRP_HA_ADMIN_FORWARDER)
  {
    /* enabling of HSYNC FWD is only allowed, if one MRP instance is active OR forwarding for MRP MC is changed simultaneously */
    if (LSA_HOST_PTR_ARE_EQUAL (pFwdCtrl, LSA_NULL) || pFwdCtrl->pLLreqType == g_MrpllReqTbl_mrp_set_fdb)
    {
      MRP_LOWER_TRACE_01 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_ll_fdb_hsync_cfg[inst=%d]: hsyncAdminRole=FORWARDER, HSYNC FWD change requested", instance);
      
      /**/
      fwdCtrl.pLLreqType  = g_MrpllReqTbl_mrp_hsync_set_fdb;
      fwdCtrl.pWorkRqb    = pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_HSYNC];
      fwdCtrl.pTestRqb    = 0;
      fwdCtrl.pTrfRqb     = 0;
      fwdCtrl.macAddrPrio = EDD_MULTICAST_FWD_PRIO_UNCHANGED;

      mrp_ll_fwd_ctrl_chg_all_ports_dis_rports_ena (pChSys, &fwdCtrl, rport_1, rport_2);

      mrp_ll_burst_req (pChSys, instance, g_MrpllReqTbl_mrp_hsync_set_fdb, LSA_TRUE);
    }
  }
}

/*===========================================================================
* FUNCTION : mrp_ll_fdb_cfg
*----------------------------------------------------------------------------
* PURPOSE  : set fdb
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            instance = affected MRP instance
*            pFwdCtrl  = pointer to fwdCtrl data (rqbs,...)
*            rport_1, rport2 = ring ports. If rport == 0 than forward disabled.
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_ll_fdb_cfg (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, const MRP_FWD_CTRL_DATA_TYPE *pFwdCtrl, LSA_UINT16 rport_1, LSA_UINT16 rport_2)
{
  MRP_LOWER_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_ll_fdb_cfg[inst=%d] entered, MRP/HRP FWD change requested", instance);

  mrp_ll_fwd_ctrl_chg_all_ports_dis_rports_ena (pChSys, pFwdCtrl, rport_1, rport_2);
  /**/
  mrp_ll_burst_req(pChSys, instance, pFwdCtrl->pLLreqType, LSA_TRUE);

  if (!instance)
    mrp_ll_fdb_hsync_cfg (pChSys, instance, pFwdCtrl, rport_1, rport_2);
}


/*===========================================================================
* FUNCTION : mrp_ll_init_hsync
*----------------------------------------------------------------------------
* PURPOSE  : initialize EDD settings for HSYNC
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys            = channel reference for edd system channel
*            instance          = affected MRP instance
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_ll_init_hsync (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance)
{
  MRP_FWD_CTRL_DATA_TYPE fwdCtrl;
  MRP_MC_MODE_DATA_TYPE  mcMode;
  MRP_HA_ADMIN_ROLE_ENUM_T hsyncAdminRole;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  hsyncAdminRole = mrp_sys_get_ha_admin_role (pChSys);

  if (hsyncAdminRole != MRP_HA_ADMIN_NONE)
  {
    /*fwd ctrl*/
    fwdCtrl.pLLreqType = g_MrpllReqTbl_mrp_init_hsync;
    fwdCtrl.pWorkRqb   = pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_HSYNC];
    fwdCtrl.pTestRqb   = 0;
    fwdCtrl.pTrfRqb    = 0;
    fwdCtrl.macAddrPrio = EDD_MULTICAST_FWD_PRIO_ORG;

    mrp_ll_fwd_ctrl_chg_all_ports (pChSys, &fwdCtrl, EDD_MULTICAST_FWD_DISABLE);

    mcMode.pWorkRqb = pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MULTICAST_HSYNC];
    mcMode.pTestRqb = 0;
    mcMode.modeTest = 0;
    mcMode.pTrfRqb  = 0;
    mcMode.modeTrf  = 0;

    if (hsyncAdminRole == MRP_HA_ADMIN_FORWARDER)
    {
      mcMode.modeWork = EDD_MULTICAST_DISABLE;
    }
    else /* hsyncAdminRole == MRP_HA_ADMIN_APPLICATION */
    {
      mcMode.modeWork = EDD_MULTICAST_ENABLE;
    }

    mrp_ll_mc_mode_set (&mcMode);

    MRP_LOWER_TRACE_03 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_ll_init_hsync[inst=%d]: hsyncAdminRole=%d, FWD any port=DIS queued, DISABLE hostFwd=%d queued", instance, hsyncAdminRole, mcMode.modeWork);

    mrp_ll_burst_req (pChSys, instance, fwdCtrl.pLLreqType, LSA_TRUE);
  }
  else /* hsyncAdminRole == MRP_HA_ADMIN_NONE */
  {
    /* nothing to do, default flooding */
  }
}

/*===========================================================================
* FUNCTION : mrp_ll_deinit_hsync
*----------------------------------------------------------------------------
* PURPOSE  : deinit EDD settings for HSYNC
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys      = channel reference for edd system channel
*            instance    = affected MRP instance
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_ll_deinit_hsync (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance)
{
  MRP_FWD_CTRL_DATA_TYPE fwdCtrl;
  MRP_MC_MODE_DATA_TYPE  mcMode;
  MRP_HA_ADMIN_ROLE_ENUM_T hsyncAdminRole;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  hsyncAdminRole = mrp_sys_get_ha_admin_role (pChSys);


  if (hsyncAdminRole != MRP_HA_ADMIN_NONE)
  {
    MRP_LOWER_TRACE_02 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_ll_deinit_hsync[inst=%d]: hsyncAdminRole=%d, but MRP=OFF queued, FWD to any port=DIS, DISABLE hostFWD=TRUE", instance, hsyncAdminRole);

    /*fwd ctrl*/
    fwdCtrl.pLLreqType = g_MrpllReqTbl_mrp_deinit_hsync;
    fwdCtrl.pWorkRqb = pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_HSYNC];
    fwdCtrl.pTestRqb = 0;
    fwdCtrl.pTrfRqb  = 0;
    fwdCtrl.macAddrPrio = EDD_MULTICAST_FWD_PRIO_UNCHANGED;

    mrp_ll_fwd_ctrl_chg_all_ports (pChSys, &fwdCtrl, EDD_MULTICAST_FWD_DISABLE);

    /* multicast configuration */
    mcMode.pWorkRqb = pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MULTICAST_HSYNC];
    mcMode.pTestRqb = 0;
    mcMode.pTrfRqb  = 0;

    mcMode.modeTrf  = 0;
    mcMode.modeWork = EDD_MULTICAST_DISABLE;
    mcMode.modeTest = 0;

    mrp_ll_mc_mode_set (&mcMode);

    mrp_ll_burst_req (pChSys, instance, fwdCtrl.pLLreqType, LSA_TRUE);
  }
  else /* hsyncAdminRole == MRP_HA_ADMIN_NONE */
  {
    /* nothing to do, default flooding */
  }
}


/*===========================================================================
* FUNCTION : mrp_ll_init_modul_mrc
*----------------------------------------------------------------------------
* PURPOSE  : initialize EDD settings of one MRC instance
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys      = channel reference for edd system channel
*            instance    = affected MRP instance
*            port0,port1 = edd port id (1..MAX_PORTS) for ring ports
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_ll_init_modul_mrc (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, const MRP_MRC_PARAMS_TYPE *pParams)
{
#ifndef MRP_CFG_DELAYED_LINK_UP
  MRP_EDD_UPPER_MULTICAST_FWD_CTRL_PTR_TYPE pMcastFwdCtrl;
#endif
  MRP_EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE pPSParam;
  MRP_EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE pPSParamTplgChgStart;
  MRP_EDD_UPPER_SWI_FLUSH_FILTERING_DB_PTR_TYPE pFlushFilterParam;
  LSA_UINT16 portIdx_1;
  LSA_UINT16 portIdx_2;
  LSA_UINT16 k, numOfPorts;
  MRP_FWD_CTRL_DATA_TYPE fwdCtrl;
  MRP_MC_MODE_DATA_TYPE mcMode;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;
  
  MRP_LOWER_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_ll_init_modul_mrc[inst=%d] entered", instance);
  
  pDBedd->pInstData[instance].initCnt++;
  pDBedd->pInstData[instance].RPort_1 = pParams->RPort_1;
  pDBedd->pInstData[instance].RPort_2 = pParams->RPort_2;

  mrp_mib_write_rports(pChSys, instance, pDBedd->pInstData[instance].RPort_1, pDBedd->pInstData[instance].RPort_2);

  /**/
#ifdef MRP_CFG_PLUGIN_2_HSR
  if (!instance)
  {
    mrp_bs_set_ring_ports_and_mode (pChSys, pDBedd->pInstData[instance].RPort_1, pDBedd->pInstData[instance].RPort_2, MRP_MODE_T_MRP);
    mrp_ll_pre_init_modul_stby (pChSys, instance);  
  }
#endif

  /* only flush FDB entries related to ringports of this MRP instance */
  pFlushFilterParam = MRP_EDD_LOWER_GET_PPARAM_FLUSH_FILTERING_DB(pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_FLUSH_FILTERING_DB]);

  k = 0;
  pFlushFilterParam->PortID[k++] = pParams->RPort_1;
  pFlushFilterParam->PortID[k++] = pParams->RPort_2;
  pFlushFilterParam->PortCnt = k;


  /**/
  portIdx_1 = (LSA_UINT16)(pDBedd->pInstData[instance].RPort_1 - 1);
  portIdx_2 = (LSA_UINT16)(pDBedd->pInstData[instance].RPort_2 - 1);

  /*fwd ctrl*/
  fwdCtrl.pLLreqType = g_MrpllReqTbl_mrp_init_nsm;
  fwdCtrl.pWorkRqb = pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_WRK];
  fwdCtrl.pTestRqb = pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_TST];
  fwdCtrl.pTrfRqb  = 0;
  fwdCtrl.macAddrPrio = EDD_MULTICAST_FWD_PRIO_UNCHANGED;

  mrp_ll_fwd_ctrl_chg_all_ports (pChSys, &fwdCtrl, EDD_MULTICAST_FWD_DISABLE);

#ifndef MRP_CFG_DELAYED_LINK_UP
  /**/
  if (!pDBedd->activeMrmInstCnt)
  {
    pMcastFwdCtrl = MRP_EDD_LOWER_GET_PPARAM_MULTICAST_FWD_CTRL(pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_WRK]);
    pMcastFwdCtrl->pPortIDModeArray[portIdx_1].Mode = EDD_MULTICAST_FWD_ENABLE;
    pMcastFwdCtrl->pPortIDModeArray[portIdx_2].Mode = EDD_MULTICAST_FWD_ENABLE;
    /**/
    pMcastFwdCtrl = MRP_EDD_LOWER_GET_PPARAM_MULTICAST_FWD_CTRL(pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_TST]);
    pMcastFwdCtrl->pPortIDModeArray[portIdx_1].Mode = EDD_MULTICAST_FWD_ENABLE;
    pMcastFwdCtrl->pPortIDModeArray[portIdx_2].Mode = EDD_MULTICAST_FWD_ENABLE;
  }
  else
  {
    MRP_LOWER_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "mrp_ll_init_modul_mrc[inst=%d], FWD_CTRL not enabled for rports because activeMrmInstCnt=%d", instance, pDBedd->activeMrmInstCnt);
  }
#endif
  /**/
  mcMode.pWorkRqb = pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MULTICAST_WRK];
  mcMode.pTestRqb = pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MULTICAST_TST];
  mcMode.pTrfRqb  = 0;

  mcMode.modeTrf  = 0;
  mcMode.modeWork = EDD_MULTICAST_ENABLE;
  mcMode.modeTest = EDD_MULTICAST_DISABLE;  
  mrp_ll_mc_mode_set (&mcMode);

  numOfPorts = mrp_sys_get_num_of_ports (pChSys);

  pPSParam = MRP_EDD_LOWER_GET_PPARAM_SET_PORT_STATE(pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_SET_PS_SHADOW]);
  for (k = 0; k < numOfPorts; k++) 
  {
    pPSParam->PortIDState[k] = EDD_PORT_STATE_UNCHANGED;
  }
  pPSParam->PortIDState[portIdx_1] = EDD_PORT_STATE_BLOCKING;
  pPSParam->PortIDState[portIdx_2] = EDD_PORT_STATE_BLOCKING;
#ifdef MRP_CFG_DELAYED_LINK_UP
  mrp_edd_set_link_up_time(pChSys, pDBedd->pInstData[instance].RPort_1, MRP_VAL_LINK_UP_RING_T_CNT);
  mrp_edd_set_link_up_time(pChSys, pDBedd->pInstData[instance].RPort_2, MRP_VAL_LINK_UP_RING_T_CNT);
#endif
  /**/
  pPSParamTplgChgStart = MRP_EDD_LOWER_GET_PPARAM_SET_PORT_STATE(pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_SET_PS_TPLG_CHG_START]);
  for (k = 0; k < numOfPorts; k++) 
  {
    pPSParamTplgChgStart->PortIDState[k] = EDD_PORT_STATE_UNCHANGED;
  }
  pPSParamTplgChgStart->PortIDState[portIdx_1] = EDD_PORT_STATE_BLOCKING;
  pPSParamTplgChgStart->PortIDState[portIdx_2] = EDD_PORT_STATE_BLOCKING;

  mrp_ll_burst_req(pChSys, instance, g_MrpllReqTbl_mrp_init_nsm, LSA_TRUE);

  if (!instance)
  {
    mrp_ll_init_hsync (pChSys, instance);

    mrp_in_ll_in_init (pChSys, instance, LSA_FALSE, LSA_FALSE, LSA_FALSE);

    MRP_LOWER_TRACE_01 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_ll_init_modul_mrc[inst=%d]: mrp_ll_init_hsync and mrp_in_ll_in_init called", instance);
  }
}


/*===========================================================================
* FUNCTION : mrp_ll_deinit_modul_mrc
*----------------------------------------------------------------------------
* PURPOSE  : deinitialize EDD settings of one MRC instance
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            instance = affected MRP instance
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_ll_deinit_modul_mrc (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance)
{
  MRP_EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE pPSParam;
  MRP_EDD_UPPER_SWI_FLUSH_FILTERING_DB_PTR_TYPE pFlushFilterParam;
  LSA_UINT16 i, k, numOfPorts;
  MRP_FWD_CTRL_DATA_TYPE fwdCtrl;
  MRP_MC_MODE_DATA_TYPE mcMode;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  MRP_LOWER_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_ll_deinit_modul_mrc[inst=%d] entered", instance);

#ifdef MRP_CFG_PLUGIN_2_HSR
  if (!instance)
  {
    mrp_bs_set_ring_ports_and_mode (pChSys, 0, 0, MRP_MODE_T_INIT);
    mrp_ll_pre_deinit_modul_stby (pChSys, instance);  
  }
#endif

  if (!instance)
  {
    mrp_ll_deinit_hsync (pChSys, instance);
    mrp_in_ll_in_deinit (pChSys, instance);
  }

  numOfPorts = mrp_sys_get_num_of_ports (pChSys);

  /* as this MRP instance is deactivated, change RQB settings for FLUSH FDB to default values (= flush all) */
  pFlushFilterParam = MRP_EDD_LOWER_GET_PPARAM_FLUSH_FILTERING_DB(pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_FLUSH_FILTERING_DB]);

  k = 0;
  pFlushFilterParam->PortID[k++] = pDBedd->pInstData[instance].RPort_1;
  pFlushFilterParam->PortID[k++] = pDBedd->pInstData[instance].RPort_2;
  pFlushFilterParam->PortCnt = k;
  
  /* enable ringports */
  pPSParam = MRP_EDD_LOWER_GET_PPARAM_SET_PORT_STATE(pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_SET_PS_DEINIT]);
  for (i = 0; i < numOfPorts; i++) 
  {
    pPSParam->PortIDState[i] = EDD_PORT_STATE_UNCHANGED;
  }
  /* portState is always set to FORWARD, independent from linkState (pDBedd->pLinkData[pDBedd->pInstData[instance].RPort_2 - 1].currentState).
  * reason: if the port is DISABLEd, EDD wouldn't reconfigure it to FORWARD
  */
  pPSParam->PortIDState[pDBedd->pInstData[instance].RPort_1 - 1] = EDD_PORT_STATE_FORWARDING;
  pPSParam->PortIDState[pDBedd->pInstData[instance].RPort_2 - 1] = EDD_PORT_STATE_FORWARDING;

  /* fdb setting */
  fwdCtrl.pLLreqType = g_MrpllReqTbl_mrp_deinit_nsm;
  fwdCtrl.pWorkRqb = pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_WRK];
  fwdCtrl.pTestRqb = pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_TST];
  fwdCtrl.pTrfRqb  = 0;
  fwdCtrl.macAddrPrio = EDD_MULTICAST_FWD_PRIO_UNCHANGED;
  
  mrp_ll_fwd_ctrl_chg_all_ports (pChSys, &fwdCtrl, EDD_MULTICAST_FWD_ENABLE);

  mcMode.pWorkRqb = pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MULTICAST_WRK];
  mcMode.pTestRqb = pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MULTICAST_TST];
  mcMode.pTrfRqb  = 0;
  /* Attention: if at least one MRM instance is active, any MRP frame has to be forwarded to host, no port forwarding is allowed! */
  mcMode.modeTrf  = 0;
  mcMode.modeWork = EDD_MULTICAST_DISABLE;
  mcMode.modeTest = EDD_MULTICAST_DISABLE;
  
  mrp_ll_mc_mode_set (&mcMode);

#ifdef MRP_CFG_DELAYED_LINK_UP
  /**/
  mrp_edd_set_link_up_time(pChSys, pDBedd->pInstData[instance].RPort_1, MRP_VAL_LINK_UP_T_DEFAULT_CNT);
  mrp_edd_set_link_up_time(pChSys, pDBedd->pInstData[instance].RPort_2, MRP_VAL_LINK_UP_T_DEFAULT_CNT);
#endif
  /**/
  mrp_ll_burst_req(pChSys, instance, g_MrpllReqTbl_mrp_deinit_nsm, LSA_TRUE);
  
  if (pDBedd->pInstData[instance].initCnt)
  {
    pDBedd->pInstData[instance].initCnt--;
    
    if (!pDBedd->pInstData[instance].initCnt)
    {
      pDBedd->pInstData[instance].RPort_1 = 0;
      pDBedd->pInstData[instance].RPort_2 = 0;      
    }
  }
}

