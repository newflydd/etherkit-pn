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
/*  F i l e               &F: mrp_inbas.c                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Basic functionality for MRP interconnection                              */ 
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef MRP_MESSAGE

/*  2017-05-30  ds    initial version of file introduced                     */
/*  2018-10-19  hh    introduction of N MRP interconnection instances        */
/*                                                                           */
/*                                                                           */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID   41
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



/*=== used to init mrp intst/inctrl/intrf MC mode ===*/
const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_in_init[] =
{
  /* first and last entry must be LSA_NULL (dummy) !!! */
  /* cbf                                     , rqbNum                                     , service  */
  { LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END               , 0x0000 }
  ,{ mrp_edd_req_set_multicast               , MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_TST      , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_done          , MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_TST      , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl      , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_TST , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl_done , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_TST , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_set_multicast               , MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_WRK      , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_done          , MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_WRK      , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl      , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_WRK , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl_done , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_WRK , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_set_multicast               , MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_TRF      , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_done          , MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_TRF      , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl      , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_TRF , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl_done , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_TRF , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END               , 0x0000 }
};

/*=== used to deinit mrp intst/inctrl/intrf MC mode ===*/
const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_in_deinit[] =
{
  { LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END               , 0x0000 }
  ,{ mrp_edd_req_set_multicast               , MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_TST      , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_done          , MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_TST      , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl      , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_TST , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl_done , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_TST , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_set_multicast               , MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_WRK      , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_done          , MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_WRK      , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl      , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_WRK , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl_done , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_WRK , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_set_multicast               , MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_TRF      , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_done          , MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_TRF      , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl      , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_TRF , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl_done , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_TRF , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END               , 0x0000 }
};


/*=== used to chg host for mrp IN_TST MC mode ===*/
const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_in_tst_chg_host[] =
{
  /* first and last entry must be LSA_NULL (dummy) !!! */
  /* cbf                                     , rqbNum                                     , service  */
  { LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END               , 0x0000 }
  ,{ mrp_edd_req_set_multicast               , MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_TST      , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_done          , MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_TST      , EDD_SRV_MULTICAST }
  ,{ LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END               , 0x0000 }
};

/*=== used to chg host for mrp IN_CTRL MC mode ===*/
const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_in_ctrl_chg_host[] =
{
  /* first and last entry must be LSA_NULL (dummy) !!! */
  /* cbf                                     , rqbNum                                     , service  */
  { LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END               , 0x0000 }
  ,{ mrp_edd_req_set_multicast               , MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_WRK      , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_done          , MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_WRK      , EDD_SRV_MULTICAST }
  ,{ LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END               , 0x0000 }
};

/*=== used to chg host for mrp IN_TRF MC mode ===*/
const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_in_trf_chg_host[] =
{
  /* first and last entry must be LSA_NULL (dummy) !!! */
  /* cbf                                     , rqbNum                                     , service  */
  { LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END               , 0x0000 }
  ,{ mrp_edd_req_set_multicast               , MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_TRF      , EDD_SRV_MULTICAST }
  ,{ mrp_edd_req_set_multicast_done          , MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_TRF      , EDD_SRV_MULTICAST }
  ,{ LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END               , 0x0000 }
};


/* used to change mrp intst forwarding FDB settings */
const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_in_tst_set_fdb[] =
{
   { LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END                   , 0x0000 }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl      , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_TST     , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl_done , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_TST     , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END                   , 0x0000 }
};

/* used to change mrp inctrl forwarding FDB settings */
const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_in_ctrl_set_fdb[] =
{
  { LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END                   , 0x0000 }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl      , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_WRK     , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl_done , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_WRK     , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END                   , 0x0000 }
};

/* used to change mrp intrf forwarding FDB settings */
const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_in_trf_set_fdb[] =
{
   { LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END                   , 0x0000 }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl      , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_TRF     , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ mrp_edd_req_set_multicast_fwd_ctrl_done , MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_TRF     , EDD_SRV_MULTICAST_FWD_CTRL }
  ,{ LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END                   , 0x0000 }
};


/*=== interconnection topology change request originated by RSTPP (send frame) ===*/
const MRP_LL_REQ_TBL_T g_MrpllReqTbl_mrp_in_rstpp_tplg_chg_req[] =
{
  { LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END                , 0x0000 }
  ,{ mrp_in_edd_req_in_rstpp_tplg_chg_req   , MRP_LL_SREQ_RQB_ENUM_TBL_END                , 0x0000 }
  ,{ LSA_NULL                               , MRP_LL_SREQ_RQB_ENUM_TBL_END                , 0x0000 }
};

/*=== interconnection topology change request originated by MIM (send frame) ===*/
const MRP_LL_REQ_TBL_T g_MrpllReqTbl_mrp_in_mim_tplg_chg_req[] =
{
  { LSA_NULL                                , MRP_LL_SREQ_RQB_ENUM_TBL_END                , 0x0000 }
  ,{ mrp_in_mim_edd_req_in_tplg_chg_req     , MRP_LL_SREQ_RQB_ENUM_TBL_END                , 0x0000 }
  ,{ LSA_NULL                               , MRP_LL_SREQ_RQB_ENUM_TBL_END                , 0x0000 }
};

/*---------------------------------------------------------------------------*/
/*                            forward declarations                           */
/*---------------------------------------------------------------------------*/
LSA_EXTERN LSA_VOID mrp_mrc_update (MRP_MRC_DB_TYPE *pDBmrc, MRP_MRC_EVENT_TYPE event);


/*===========================================================================
* FUNCTION : mrp_in_ll_in_event_handler
*----------------------------------------------------------------------------
* PURPOSE  : signaling the MRP interconnection event changes of an MRP
*            interconnection instance
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS : signal state changes from MIM resp. MIC
*==========================================================================*/
LSA_VOID mrp_in_ll_in_event_handler (const MRP_IN_DB_TYPE *pDBin)
{
  const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys = pDBin->pChSys;
  const LSA_UINT16 *pInstance = pDBin->pInstance;

  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  MRP_ASSERT (!LSA_HOST_PTR_ARE_EQUAL (pDBedd, LSA_NULL));

  if (!LSA_HOST_PTR_ARE_EQUAL (pDBedd->pChPrivUsr, LSA_NULL))
  {
    (void) (mrp_in_red_event_ind_done (pDBedd->pChPrivUsr, pInstance));
  }
}

/*===========================================================================
* FUNCTION : mrp_in_ll_fdb_in_tst_cfg
*----------------------------------------------------------------------------
* PURPOSE  : set FDB for MC_INTST forwarding
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            instance = affected MRP instance
*            rport_1, rport2 = ring ports. If rport == 0 than forward disabled.
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_ll_fdb_in_tst_cfg (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, LSA_UINT16 rport_1, LSA_UINT16 rport_2)
{
  MRP_FWD_CTRL_DATA_TYPE fwdCtrl;
  LSA_UINT8 inFwdRulesSupported;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  inFwdRulesSupported = mrp_in_sys_get_in_fwd_rules_supported (pChSys);

  if (inFwdRulesSupported != MRP_IN_FWD_RULES_SUPPORTED_OFF && !pInstance[MRP_INSTANCE_INDEX])
  {    
    /* enabling of MC_INTST FWD is only allowed, if one MRP instance is active */
    MRP_LOWER_TRACE_03 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_ll_fdb_in_tst_cfg[inst=%d]: MC_INTST FWD enable for rport1=%d, rport2=%d", pInstance[MRP_INSTANCE_INDEX], rport_1, rport_2);

    fwdCtrl.pLLreqType = g_MrpllReqTbl_mrp_in_tst_set_fdb;
    fwdCtrl.pWorkRqb = 0;
    fwdCtrl.pTestRqb = pDBedd->pInstData[pInstance[MRP_INSTANCE_INDEX]].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_TST];
    fwdCtrl.pTrfRqb  = 0;
    fwdCtrl.macAddrPrio = EDD_MULTICAST_FWD_PRIO_UNCHANGED;

    mrp_ll_fwd_ctrl_chg_all_ports_dis_rports_ena (pChSys, &fwdCtrl, rport_1, rport_2);

    mrp_ll_in_burst_req(pChSys, pInstance, g_MrpllReqTbl_mrp_in_tst_set_fdb, LSA_TRUE);
  }
}

/*===========================================================================
* FUNCTION : mrp_in_ll_fdb_in_ctrl_cfg
*----------------------------------------------------------------------------
* PURPOSE  : set FDB for MC_INCTRL forwarding
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            instance = affected MRP instance
*            rport_1, rport2 = ring ports. If rport == 0 than forward disabled.
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_ll_fdb_in_ctrl_cfg (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, LSA_UINT16 rport_1, LSA_UINT16 rport_2)
{
  MRP_FWD_CTRL_DATA_TYPE fwdCtrl;
  LSA_UINT8 inFwdRulesSupported;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  inFwdRulesSupported = mrp_in_sys_get_in_fwd_rules_supported (pChSys);

  if (inFwdRulesSupported != MRP_IN_FWD_RULES_SUPPORTED_OFF && !pInstance[MRP_INSTANCE_INDEX])
  {    
    /* enabling of MC_INCTRL FWD is only allowed, if one MRP instance is active */
    MRP_LOWER_TRACE_03 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_ll_fdb_in_ctrl_cfg[inst=%d]: MC_INCTRL FWD enable for rport1=%d, rport2=%d", pInstance[MRP_INSTANCE_INDEX], rport_1, rport_2);

    fwdCtrl.pLLreqType = g_MrpllReqTbl_mrp_in_ctrl_set_fdb;
    fwdCtrl.pWorkRqb = pDBedd->pInstData[pInstance[MRP_INSTANCE_INDEX]].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_WRK];
    fwdCtrl.pTestRqb = 0;
    fwdCtrl.pTrfRqb  = 0;
    fwdCtrl.macAddrPrio = EDD_MULTICAST_FWD_PRIO_UNCHANGED;

    mrp_ll_fwd_ctrl_chg_all_ports_dis_rports_ena (pChSys, &fwdCtrl, rport_1, rport_2);

    mrp_ll_in_burst_req(pChSys, pInstance, g_MrpllReqTbl_mrp_in_ctrl_set_fdb, LSA_TRUE);
  }
}

/*===========================================================================
* FUNCTION : mrp_in_ll_fdb_in_trf_cfg
*----------------------------------------------------------------------------
* PURPOSE  : set FDB for MC_INTRF forwarding
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            instance = affected MRP instance
*            rport_1, rport2 = ring ports. If rport == 0 than forward disabled.
*----------------------------------------------------------------------------
* COMMENTS :  THIS FUNCTION IS NOT USED YET!
*==========================================================================*/
LSA_VOID mrp_in_ll_fdb_in_trf_cfg (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, LSA_UINT16 rport_1, LSA_UINT16 rport_2)
{
  MRP_FWD_CTRL_DATA_TYPE fwdCtrl;
  LSA_UINT8 inFwdRulesSupported;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  inFwdRulesSupported = mrp_in_sys_get_in_fwd_rules_supported (pChSys);

  if (inFwdRulesSupported != MRP_IN_FWD_RULES_SUPPORTED_OFF && !pInstance[MRP_INSTANCE_INDEX])
  {
    /* enabling of MC_INCTRL FWD is only allowed, if one MRP instance is active */
    MRP_LOWER_TRACE_03 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_ll_fdb_in_trf_cfg[inst=%d]: MC_INTRF FWD enable for rport1=%d, rport2=%d", pInstance[MRP_INSTANCE_INDEX], rport_1, rport_2);

    fwdCtrl.pLLreqType  = g_MrpllReqTbl_mrp_in_trf_set_fdb;
    fwdCtrl.pWorkRqb    = 0;
    fwdCtrl.pTestRqb    = 0;
    fwdCtrl.pTrfRqb     = pDBedd->pInstData[pInstance[MRP_INSTANCE_INDEX]].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_TRF];
    fwdCtrl.macAddrPrio = EDD_MULTICAST_FWD_PRIO_UNCHANGED;

    mrp_ll_fwd_ctrl_chg_all_ports_dis_rports_ena (pChSys, &fwdCtrl, rport_1, rport_2);

    mrp_ll_in_burst_req(pChSys, pInstance, g_MrpllReqTbl_mrp_in_trf_set_fdb, LSA_TRUE);
  }
}


/*===========================================================================
* FUNCTION : mrp_in_ll_in_init
*----------------------------------------------------------------------------
* PURPOSE  : initialize EDD settings for MC_INCTRL address
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys            = channel reference for edd system channel
*            instance          = affected MRP instance
*            locHostInCtrl     = local host on/off for MC_INCTRL
*            locHostInTrf      = local host on/off for MC_INTRF
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_ll_in_init (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_BOOL locHostInTest, LSA_BOOL locHostInCtrl, LSA_BOOL locHostInTrf)
{
  MRP_FWD_CTRL_DATA_TYPE fwdCtrl;
  MRP_MC_MODE_DATA_TYPE  mcMode;
  LSA_UINT8 inFwdRulesSupported;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  inFwdRulesSupported = mrp_in_sys_get_in_fwd_rules_supported (pChSys);

  if (inFwdRulesSupported != MRP_IN_FWD_RULES_SUPPORTED_OFF && !instance)
  {
    /*fwd ctrl*/
    fwdCtrl.pLLreqType  = g_MrpllReqTbl_mrp_in_init;
    fwdCtrl.pTestRqb    = pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_TST];
    fwdCtrl.pWorkRqb    = pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_WRK];
    fwdCtrl.pTrfRqb     = pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_TRF];
    fwdCtrl.macAddrPrio = EDD_MULTICAST_FWD_PRIO_ORG;

    mrp_ll_fwd_ctrl_chg_all_ports (pChSys, &fwdCtrl, EDD_MULTICAST_FWD_DISABLE);

    mcMode.pTestRqb = pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_TST];
    mcMode.pWorkRqb = pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_WRK];
    mcMode.pTrfRqb  = pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_TRF];

    if (locHostInTest == LSA_TRUE)
      mcMode.modeTest = EDD_MULTICAST_ENABLE;
    else
      mcMode.modeTest = EDD_MULTICAST_DISABLE;

    if (locHostInCtrl == LSA_TRUE)
      mcMode.modeWork = EDD_MULTICAST_ENABLE;
    else
      mcMode.modeWork = EDD_MULTICAST_DISABLE;

    if (locHostInTrf == LSA_TRUE)
      mcMode.modeTrf = EDD_MULTICAST_ENABLE;
    else
      mcMode.modeTrf = EDD_MULTICAST_DISABLE;

    mrp_ll_mc_mode_set (&mcMode);

    MRP_LOWER_TRACE_04 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_ll_in_init[inst=%d]: MC_INTST/MC_INCTRL/MC_INTRF: FWD any port=DIS queued, DISABLE hostFwdInCtrl=%d, hostFwdInTest=%d, hostFwdInTrf=%d queued",
                        instance, mcMode.modeWork, mcMode.modeTest, mcMode.modeTrf);

    mrp_ll_burst_req (pChSys, instance, fwdCtrl.pLLreqType, LSA_TRUE);
  }
  else /* inFwdRulesSupported == MRP_IN_FWD_RULES_SUPPORTED_OFF */
  {
    /* nothing to do, default flooding */
  }
}


/*===========================================================================
* FUNCTION : mrp_in_ll_in_ctrl_chg_host
*----------------------------------------------------------------------------
* PURPOSE  : change host settings for MC_INCTRL address
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pDBin = pointer to one instance of mrp interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_ll_in_ctrl_chg_host (const MRP_IN_DB_TYPE *pDBin, LSA_BOOL localHost)
{
  MRP_MC_MODE_DATA_TYPE  mcMode;
  LSA_UINT8 inFwdRulesSupported;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pDBin->pChSys->pChSysDB->pEdd;

  inFwdRulesSupported = mrp_in_sys_get_in_fwd_rules_supported (pDBin->pChSys);

  if (inFwdRulesSupported != MRP_IN_FWD_RULES_SUPPORTED_OFF && !pDBin->pInstance[MRP_INSTANCE_INDEX])
  {
    mcMode.pWorkRqb = pDBedd->pInstData[pDBin->pInstance[MRP_INSTANCE_INDEX]].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_WRK];
    mcMode.pTestRqb = 0;
    mcMode.modeTest = 0;
    mcMode.pTrfRqb  = 0;
    mcMode.modeTrf  = 0;

    if (localHost == LSA_TRUE)
      mcMode.modeWork = EDD_MULTICAST_ENABLE;
    else
      mcMode.modeWork = EDD_MULTICAST_DISABLE;

    mrp_ll_mc_mode_set (&mcMode);

    MRP_LOWER_TRACE_03 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_ll_in_ctrl_chg_host[inst=%d/inInst=%d]: MC_INCTRL, hostFwd=%d queued", pDBin->pInstance[MRP_INSTANCE_INDEX], pDBin->pInstance[MRP_IN_INSTANCE_INDEX], mcMode.modeWork);

    mrp_ll_in_burst_req (pDBin->pChSys, pDBin->pInstance, g_MrpllReqTbl_mrp_in_ctrl_chg_host, LSA_TRUE);
  }
  else /* inFwdRulesSupported == MRP_IN_FWD_RULES_SUPPORTED_OFF */
  {
    /* nothing to do, default flooding */
  }
}


/*===========================================================================
* FUNCTION : mrp_in_ll_in_trf_chg_host
*----------------------------------------------------------------------------
* PURPOSE  : change host settings for MC_INTRF address
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pDBin = pointer to one instance of mrp interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_ll_in_trf_chg_host (const MRP_IN_DB_TYPE *pDBin, LSA_BOOL localHost)
{
  MRP_MC_MODE_DATA_TYPE  mcMode;
  LSA_UINT8 inFwdRulesSupported;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pDBin->pChSys->pChSysDB->pEdd;

  inFwdRulesSupported = mrp_in_sys_get_in_fwd_rules_supported (pDBin->pChSys);

  if (inFwdRulesSupported != MRP_IN_FWD_RULES_SUPPORTED_OFF && !pDBin->pInstance[MRP_INSTANCE_INDEX])
  {
    mcMode.pTrfRqb = pDBedd->pInstData[pDBin->pInstance[MRP_INSTANCE_INDEX]].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_TRF];
    mcMode.pWorkRqb = 0;
    mcMode.modeWork = 0;
    mcMode.pTestRqb = 0;
    mcMode.modeTest = 0;

    if (localHost == LSA_TRUE)
      mcMode.modeTrf = EDD_MULTICAST_ENABLE;
    else
      mcMode.modeTrf = EDD_MULTICAST_DISABLE;

    mrp_ll_mc_mode_set (&mcMode);

    MRP_LOWER_TRACE_03 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_ll_in_trf_chg_host[inst=%d/inInst=%d]: MC_INTRF, hostFwd=%d queued", pDBin->pInstance[MRP_INSTANCE_INDEX], pDBin->pInstance[MRP_IN_INSTANCE_INDEX], mcMode.modeTrf);

    mrp_ll_in_burst_req (pDBin->pChSys, pDBin->pInstance, g_MrpllReqTbl_mrp_in_trf_chg_host, LSA_TRUE);
  }
  else /* inFwdRulesSupported == MRP_IN_FWD_RULES_SUPPORTED_OFF */
  {
    /* nothing to do, default flooding */
  }
}

/*===========================================================================
* FUNCTION : mrp_in_ll_in_deinit
*----------------------------------------------------------------------------
* PURPOSE  : deinit EDD settings for MC_INCTRL/MC_INTRF address
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys      = channel reference for edd system channel
*            instance    = affected MRP instance
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_ll_in_deinit (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance)
{
  MRP_FWD_CTRL_DATA_TYPE fwdCtrl;
  MRP_MC_MODE_DATA_TYPE  mcMode;
  LSA_UINT8 inFwdRulesSupported;
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pChSys->pChSysDB->pEdd;

  inFwdRulesSupported = mrp_in_sys_get_in_fwd_rules_supported (pChSys);

  if (inFwdRulesSupported != MRP_IN_FWD_RULES_SUPPORTED_OFF && !instance)
  {
    MRP_LOWER_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_ll_in_deinit[inst=%d]: FWD to any port=ENA, DISABLE hostFWD=TRUE", instance);

    /*fwd ctrl*/
    fwdCtrl.pLLreqType  = g_MrpllReqTbl_mrp_in_deinit;
    fwdCtrl.pTestRqb    = pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_TST];
    fwdCtrl.pWorkRqb    = pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_WRK];
    fwdCtrl.pTrfRqb     = pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_TRF];
    fwdCtrl.macAddrPrio = EDD_MULTICAST_FWD_PRIO_UNCHANGED;

    mrp_ll_fwd_ctrl_chg_all_ports (pChSys, &fwdCtrl, EDD_MULTICAST_FWD_ENABLE);

    /* multicast configuration */
    mcMode.pTestRqb = pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_TST];
    mcMode.pWorkRqb = pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_WRK];
    mcMode.pTrfRqb  = pDBedd->pInstData[instance].pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_TRF];

    mcMode.modeTest = EDD_MULTICAST_DISABLE;
    mcMode.modeWork = EDD_MULTICAST_DISABLE;
    mcMode.modeTrf  = EDD_MULTICAST_DISABLE;

    mrp_ll_mc_mode_set (&mcMode);

    mrp_ll_burst_req (pChSys, instance, fwdCtrl.pLLreqType, LSA_TRUE);
  }
  else /* inFwdRulesSupported == MRP_IN_FWD_RULES_SUPPORTED_OFF */
  {
    /* nothing to do, default flooding */
  }
}

/*===========================================================================
* FUNCTION : mrp_in_check_active_instance
*----------------------------------------------------------------------------
* PURPOSE  : Check if an MRP_IN instance is running
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys    = channel reference
* OUTPUTS  : LSA_TRUE if at least one instance is still running
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_BOOL mrp_in_check_active_instance (const MRP_CONST_CHANNEL_TYPE_COMMON pChSys, LSA_UINT16 inInstance)
{
  MRP_IN_DB_TYPE *pDBin;
  MRP_IN_DB_TYPE *pDBinBase = (MRP_IN_DB_TYPE *)pChSys->pChSysDB->pMrpIn;
  LSA_UINT16 mrpInMaxInstance, i;

  mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pChSys);

  if (mrpInMaxInstance)
  {
    for(i = 0; i < mrpInMaxInstance; i++)
    {
      if(i == inInstance) /* ignor the state of the requesting inInstance */
      {
        continue;
      }
      pDBin = &pDBinBase[i];

      if(pDBin->runState == MRP_IN_OPERATION_ON)
      {
        return LSA_TRUE;
      }
    }
  }
  return LSA_FALSE;
}

/*===========================================================================
* FUNCTION : mrp_in_rstpp_inTC_send_to_mrp
*----------------------------------------------------------------------------
* PURPOSE  : send interconnection topology change packets requested by RSTPP
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys    = channel reference
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_VOID mrp_in_rstpp_inTC_send_to_mrp (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys)
{
  LSA_UINT32 durationTicks;
  MRP_IN_DB_TYPE *pDBin;
  MRP_IN_DB_TYPE *pDBinBase = (MRP_IN_DB_TYPE *)pChSys->pChSysDB->pMrpIn;
  LSA_UINT16 mrpInMaxInstance;

  mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pChSys);
  
  MRP_FATAL1 ((mrpInMaxInstance > 0) && (mrpInMaxInstance <= EDD_CFG_MAX_MRP_IN_INSTANCE_CNT));
  pDBin = &pDBinBase[mrpInMaxInstance - 1]; /* with respect to RSTP+ triggered In_TCs, always the "last" MRP interconnection instance is used */

  durationTicks = (LSA_UINT32)(pDBin->TOPRstppNReturn * pDBin->TOPRstppIntervalTcnt);
  pDBin->InTcTxInfo.intervalTimeMs = (LSA_UINT16)(durationTicks * MRP_TIMER_TIME_BASE_DEZ);

  MRP_PROGRAM_TRACE_03 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_rstpp_inTC_send_to_mrp[inInst=%d], intervalTimeMs=%d, NRet=%d", MRP_DEFAULT_INSTANCE, pDBin->InTcTxInfo.intervalTimeMs, pDBin->TOPRstppNReturn);

  if (pDBin->TOPRstppNReturn > 0)
  {
    pDBin->TOPRstppNReturn--;
    mrp_sys_timer_stop (pDBin->TimerIdInRstppTCsend);
    mrp_sys_timer_start (pDBin->TimerIdInRstppTCsend, pDBin->TOPRstppIntervalTcnt);
  }
  else
  {
    pDBin->TOPRstppNReturn = pDBin->TOPRsttppMaxRepeat;
  }

  /* provide TX info for MRC/MRM */
  pDBin->InTxInfo = pDBin->InTcTxInfo;

  mrp_in_mrp_tx_request (pDBin);
}


/*===========================================================================
* FUNCTION : mrp_in_oper_state_off_ac_stat1
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
LSA_VOID mrp_in_oper_state_off_ac_stat1 (MRP_IN_DB_TYPE *pDBin)
{
  LSA_BOOL rxFromDifferentIn, differentMac, differentInId;

  MRP_PROGRAM_TRACE_02 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_oper_state_off_ac_stat1[inInst=%d] entered: event=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.CurrentEvent);

  switch (pDBin->rxPara.CurrentEvent)
  {
    case MRP_IN_EVENT_T_NONE:
    break;

    case MRP_IN_EVENT_T_MAU_TYPE_CHG:
    break;

    case MRP_IN_EVENT_T_IN_TC_RSTPP_TIMER:
    {
      mrp_in_rstpp_inTC_send_to_mrp (pDBin->pChSys);
    }
    break;

    case MRP_IN_EVENT_T_TPLG_CHG_WRK:
    {
      rxFromDifferentIn = mrp_in_rx_from_different_interconn (pDBin, &differentMac, &differentInId);

      if (rxFromDifferentIn)
      {
        mrp_in_inTC_forward_to_mrp (pDBin);
      }
    }
    break;

    default:
    break;
  }
}

/*===========================================================================
* FUNCTION : mrp_in_proj_state_off_cbf
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
LSA_VOID mrp_in_proj_state_off_cbf (MRP_IN_DB_TYPE *pDBin)
{
  MRP_PROGRAM_TRACE_02 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_proj_state_off_cbf[inInst=%d] entered: event=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.CurrentEvent);

  /* call operational processing of MRP interconnection events in provisioned state "OFF" */
  pDBin->pInOperStateBaseCbf (pDBin);
}



/*===========================================================================
* FUNCTION : mrp_in_container_state
*----------------------------------------------------------------------------
* PURPOSE  : Default MRP interconnection container state (base state function).
*            This function calls the core state CBFs (OFF, MIC, MIM).
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of MRP interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_container_state (MRP_IN_DB_TYPE *pDBin)
{
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBin->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_in_container_state[inInst=%d] entered: event=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.CurrentEvent);

  switch (pDBin->rxPara.CurrentEvent)
  {
    case MRP_IN_EVENT_T_NONE:
    {
      MRP_PROGRAM_TRACE_00 (traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "mrp_in_container_state: Input event=NONE is NOT valid");
    }
    break;

    case MRP_IN_EVENT_T_IN_TC_RSTPP_TIMER:
    {
      if (pDBin->pInProjStateBaseCbf == mrp_in_proj_state_off_cbf) /* MRP interconnection is NOT configured*/
      {
        pDBin->pInProjStateBaseCbf (pDBin);
      }
      else
      {
        MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_in_container_state: event=%d (port=%d, IPort=%d) received while MRP interconnection is configured",
                              pDBin->rxPara.CurrentEvent, pDBin->rxPara.RxPort, pDBin->IPort);
      }
    }
    break;


    case MRP_IN_EVENT_T_MAU_TYPE_CHG:
    case MRP_IN_EVENT_T_LNK_CHG_UP_TIMER:
    case MRP_IN_EVENT_T_LNK_CHG_DOWN_TIMER:
    case MRP_IN_EVENT_T_LNK_POLL_TIMER:
    case MRP_IN_EVENT_T_IN_TC_TIMER:
    case MRP_IN_EVENT_T_MRP_CHG_IND:
    {
      if (pDBin->pInProjStateBaseCbf != mrp_in_proj_state_off_cbf) /* MRP interconnection is configured*/
      {
        mrp_in_write_current_cfm_disabled(pDBin);
        pDBin->pInProjStateBaseCbf (pDBin);
      }
      else
      {
        MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_in_container_state: event=%d (port=%d, IPort=%d) received while MRP interconnection is not configured",
                              pDBin->rxPara.CurrentEvent, pDBin->rxPara.RxPort, pDBin->IPort);
      }
    }
    break;


    case MRP_IN_EVENT_T_TPLG_CHG_WRK:
    {
      if (pDBin->pInProjStateBaseCbf != mrp_in_proj_state_off_cbf) /* MRP interconnection is configured*/
      {
        if (pDBin->rxPara.RxInId == pDBin->InID)
        {
          pDBin->pInProjStateBaseCbf (pDBin); /* forward IN_TC (WRK) to MIC or MIM state machine */
        }
        else
        {
          MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_container_state: IN_TC(WRK) received on port=%d has foreign inId=%d (configured inId=%d)",
                                pDBin->rxPara.RxPort, pDBin->rxPara.RxInId, pDBin->InID);

          mrp_in_inTC_forward_to_mrp (pDBin);
        }
      }
      else /* MRP interconnection is not configured for this instance -> RSTP+ instance or IN_CONVERTER */
      {
        pDBin->pInProjStateBaseCbf (pDBin);
      }
    }
    break;

    case MRP_IN_EVENT_T_POLL_WRK:
    {
      if (pDBin->pInProjStateBaseCbf != mrp_in_proj_state_off_cbf) /* MRP interconnection is configured*/
      {
        if (pDBin->rxPara.RxInId == pDBin->InID)
        {
          pDBin->pInProjStateBaseCbf (pDBin); /* forward IN_POLL (WRK) to MIC or MIM state machine */
        }
        else
        {
          MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_in_container_state: IN_POLL(WRK) received on port=%d has wrong inId=%d (configured inId=%d)",
                                pDBin->rxPara.RxPort, pDBin->rxPara.RxInId, pDBin->InID);
        }
      }
      else
      {
        MRP_PROGRAM_TRACE_01 (traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_container_state: IN_POLL(WRK) received on rxPort=%d while MRP interconnection is OFF", pDBin->rxPara.RxPort);
      }
    }
    break;

    case MRP_IN_EVENT_T_LNK_CHG_WRK:
    {
      if (pDBin->pInProjStateBaseCbf != mrp_in_proj_state_off_cbf) /* MRP interconnection is configured*/
      {
        if (pDBin->rxPara.RxInId == pDBin->InID)
        {
          mrp_in_write_current_iport_mismatch(pDBin, LSA_FALSE);
          pDBin->pInProjStateBaseCbf (pDBin); /* forward IN_LNK_CHG (WRK) to MIC or MIM state machine */
        }
        else
        {
          MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_in_container_state: IN_LNK_CHG(WRK) received on port=%d has wrong inId=%d (configured inId=%d)",
                                pDBin->rxPara.RxPort, pDBin->rxPara.RxInId, pDBin->InID);
        }
      }
      else
      {
        MRP_PROGRAM_TRACE_01 (traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_container_state: IN_LNK_CHG(WRK) received on rxPort=%d while MRP interconnection is OFF", pDBin->rxPara.RxPort);
      }
    }
    break;

    case MRP_IN_EVENT_T_TPLG_CHG_TRF:
    {
      if (pDBin->pInProjStateBaseCbf != mrp_in_proj_state_off_cbf) /* MRP interconnection is configured*/
      {
        mrp_in_write_in_rx_inid(pDBin);
        if (pDBin->rxPara.RxPort == pDBin->IPort)
        {
          if (pDBin->rxPara.RxInId == pDBin->InID)
          {
            mrp_in_write_in_rx_port(pDBin);
            pDBin->pInProjStateBaseCbf (pDBin); /* forward IN_TC (TRF) to MIC or MIM state machine */
          }
          else
          {
            MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_in_container_state: IN_TC(TRF) received on IPort=%d has wrong inId=%d (configured inId=%d)",
                                  pDBin->rxPara.RxPort, pDBin->rxPara.RxInId, pDBin->InID);
          }
        }
        else
        {
          if(pDBin->rxPara.RxInId == pDBin->InID)
            mrp_in_write_in_rx_port(pDBin);
          MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_container_state: IN_TC(TRF) received on wrong rxPort=%d (IPort=%d)", pDBin->rxPara.RxPort, pDBin->IPort);
        }
      }
      else
      {
        MRP_PROGRAM_TRACE_01 (traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_container_state: IN_TC(TRF) received on rxPort=%d while MRP interconnection is OFF", pDBin->rxPara.RxPort);
      }
    }
    break;

    case MRP_IN_EVENT_T_POLL_TRF:
    {
      if (pDBin->pInProjStateBaseCbf != mrp_in_proj_state_off_cbf) /* MRP interconnection is configured*/
      {
        mrp_in_write_in_rx_inid(pDBin);
        if (pDBin->rxPara.RxPort == pDBin->IPort)
        {
          mrp_in_write_in_in_peer_mic_position(pDBin);
          if (pDBin->rxPara.RxInId == pDBin->InID)
          {
            mrp_in_write_in_rx_port(pDBin);
            pDBin->pInProjStateBaseCbf (pDBin); /* forward IN_POLL (TRF) to MIC or MIM state machine */
          }
          else
          {
            MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_in_container_state: IN_POLL(TRF) received on IPort=%d has wrong inId=%d (configured inId=%d)",
                                  pDBin->rxPara.RxPort, pDBin->rxPara.RxInId, pDBin->InID);
          }
        }
        else
        {
          if(pDBin->rxPara.RxInId == pDBin->InID)
            mrp_in_write_in_rx_port(pDBin);
          MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_container_state: IN_POLL(TRF) received on wrong rxPort=%d (IPort=%d)", pDBin->rxPara.RxPort, pDBin->IPort);
        }
      }
      else
      {
        MRP_PROGRAM_TRACE_01 (traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_container_state: IN_POLL(TRF) received on rxPort=%d while MRP interconnection is OFF", pDBin->rxPara.RxPort);
      }
    }
    break;

    case MRP_IN_EVENT_T_LNK_CHG_TRF:
    {
      if (pDBin->pInProjStateBaseCbf != mrp_in_proj_state_off_cbf) /* MRP interconnection is configured*/
      {
        mrp_in_write_in_rx_inid(pDBin);
        if (pDBin->rxPara.RxPort == pDBin->IPort)
        {
          mrp_in_write_in_in_peer_mic_position(pDBin);
          if (pDBin->rxPara.RxInId == pDBin->InID)
          {
            mrp_in_write_in_rx_port(pDBin);
            pDBin->pInProjStateBaseCbf (pDBin); /* forward IN_LNK_CHG (TRF) to MIC or MIM state machine */
          }
          else
          {
            MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_in_container_state: IN_LNK_CHG(TRF) received on IPort=%d has wrong inId=%d (configured inId=%d)",
                                  pDBin->rxPara.RxPort, pDBin->rxPara.RxInId, pDBin->InID);
          }
        }
        else
        {
          if(pDBin->rxPara.RxInId == pDBin->InID)
            mrp_in_write_in_rx_port(pDBin);
          MRP_PROGRAM_TRACE_02 (traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_container_state: IN_LNK_CHG(TRF) received on wrong rxPort=%d (IPort=%d)", pDBin->rxPara.RxPort, pDBin->IPort);
        }
        mrp_in_ll_in_event_handler (pDBin);
      }
      else
      {
        MRP_PROGRAM_TRACE_01 (traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_container_state: IN_LNK_CHG(TRF) received on rxPort=%d while MRP interconnection is OFF", pDBin->rxPara.RxPort);
      }
    }
    break;

    default:
    {
      MRP_PROGRAM_TRACE_01 (traceIdx, LSA_TRACE_LEVEL_FATAL, "mrp_in_container_state: Invalid input event = %d", pDBin->rxPara.CurrentEvent);
      MRP_FATAL;
    }
    break;
  }
}


/*===========================================================================
* FUNCTION : mrp_in_sys_get_num_of_in_instances
*----------------------------------------------------------------------------
* PURPOSE  : return the number of MRP interconnection instances for
*            a given system channel
*----------------------------------------------------------------------------
* RETURNS  : numOfInInstances = number of MRP interconnection instances
*            for a given system channel
*----------------------------------------------------------------------------
* INPUTS   : pCh = channel reference for system or user channel
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_UINT16 mrp_in_sys_get_num_of_in_instances (const MRP_CONST_CHANNEL_TYPE_COMMON pCh)
{
  LSA_UINT16 numOfInInstances;
  MRP_IF_DB_TYPE *pDBif = LSA_NULL;

  numOfInInstances = 0;

  if (!LSA_HOST_PTR_ARE_EQUAL (pCh, LSA_NULL))
  {
    if (!LSA_HOST_PTR_ARE_EQUAL (pCh->pChSysDB, LSA_NULL))
    {
      pDBif = (MRP_IF_DB_TYPE *)(pCh->pChSysDB->pInterfacePara);

      if (!LSA_HOST_PTR_ARE_EQUAL (pDBif, LSA_NULL))
      {
        numOfInInstances = pDBif->mrpInMaxInstance;
      }
    }
  }

  return numOfInInstances;
}

/*===========================================================================
* FUNCTION : mrp_in_sys_map_iport_to_inInstance
*----------------------------------------------------------------------------
* PURPOSE  : return the MRP interconnection instance associated with one port
*----------------------------------------------------------------------------
* RETURNS  : inInstance = associated instance to a port
*----------------------------------------------------------------------------
* INPUTS   : pCh  = channel reference for system or user channel
*            port = port to be checked as MRP interconnection port
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_UINT16 mrp_in_sys_map_iport_to_inInstance (const MRP_CONST_CHANNEL_TYPE_COMMON pCh, LSA_UINT32 port)
{
  LSA_UINT16 inInstance, maxInInstances, i;
  MRP_IN_DB_TYPE *pDBin;
  MRP_IN_DB_TYPE *pDBinBase;

  maxInInstances = mrp_in_sys_get_num_of_in_instances (pCh);

  inInstance = MRP_IN_INSTANCE_NONE;

  if (maxInInstances)
  {
    pDBinBase = (MRP_IN_DB_TYPE *)pCh->pChSysDB->pMrpIn;

    for (i = 0; i < maxInInstances; i++)
    {
      pDBin = &pDBinBase[i];

      /* An MRP interconnection port is only active if an MRP interconnection role is configured */
      if (pDBin->runState == MRP_IN_OPERATION_ON && pDBin->operRole != MRP_IN_OPER_ROLE_OFF)
      {
        /* Each MRP interconnection instance with operRole != OFF has to have an MRP interconnection port configured -> compare to any active MRP-I instance */
        if (port == pDBin->IPort)
        {
          inInstance = i;
          break;
        }
      }
    }
  }

  return inInstance;
}


/*===========================================================================
* FUNCTION : mrp_in_sys_map_inId_to_inInstance
*----------------------------------------------------------------------------
* PURPOSE  : return the MRP interconnection instance associated with one inId
*----------------------------------------------------------------------------
* RETURNS  : inInstance = associated instance to a inId
*----------------------------------------------------------------------------
* INPUTS   : pCh  = channel reference for system or user channel
*            inId = interconnection instance ID
*            port = port to be checked as MRP interconnection port
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_UINT16 mrp_in_sys_map_inId_to_inInstance (const MRP_CONST_CHANNEL_TYPE_COMMON pCh, LSA_UINT16 inId, LSA_UINT16 port)
{
  LSA_UINT16 inInstance, maxInInstances, i;
  MRP_IN_DB_TYPE *pDBin;
  MRP_IN_DB_TYPE *pDBinBase;

  /* first priority for mapping: Interconnection port, on which a frame is received */
  inInstance = mrp_in_sys_map_iport_to_inInstance (pCh, port);

  /* just in case no inInstance was found due to the port number the inId is used for further search. This is needed to generate the diagnoses info 
    in case that the port number is wrong. */
  if (inInstance == MRP_IN_INSTANCE_NONE)
  {
    maxInInstances = mrp_in_sys_get_num_of_in_instances (pCh);

    /* second priority for mapping: Interconnection instance, which is configured with this InID */
    if (maxInInstances)
    {
      pDBinBase = (MRP_IN_DB_TYPE *)pCh->pChSysDB->pMrpIn;

      for (i = 0; i < maxInInstances; i++)
      {
        pDBin = &pDBinBase[i];

        /* if the MRP interconnection instance is configured with a MRP interconnection role, then the inId has to match as well */
        if (pDBin->runState == MRP_IN_OPERATION_ON && pDBin->operRole != MRP_IN_OPER_ROLE_OFF)
        {
          if (inId == pDBin->InID) /* compare the received MRP interconnection ID with the InID of an active MRP interconnection instance */
          {
            inInstance = i;
            break;
          }
          else
          {
            inInstance = MRP_IN_INSTANCE_FOREIGN_IN_ID;
          }
        }
      }

      if (inInstance == MRP_IN_INSTANCE_NONE || inInstance == MRP_IN_INSTANCE_FOREIGN_IN_ID)
      {
        for (i = 0; i < maxInInstances; i++)
        {
          pDBin = &pDBinBase[i];

          /* if an MRP interconnection instance is active but no MRP interconnection role is configured -> RSTP+ usecase or MRA/MRM running as IN_CONVERTER */
          /* this should just be the case for the last inInstance!!! */
          if (pDBin->runState == MRP_IN_OPERATION_ON && pDBin->operRole == MRP_IN_OPER_ROLE_OFF)
          {
            inInstance = i;
            break;
          }
        }
      }
    }
  }

  return inInstance;
}


/*===========================================================================
* FUNCTION : mrp_in_sys_get_in_fwd_rules_supported
*----------------------------------------------------------------------------
* PURPOSE  : return the static configuration flag for support of
*            MC_INCTRL address forwarding
*----------------------------------------------------------------------------
* RETURNS  : inFwdRulesSupported = support for MC_INCTRL, configured by means of
*                                  GET_PARAMS flags
*----------------------------------------------------------------------------
* INPUTS   : pCh = channel reference for system or user channel
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_UINT8 mrp_in_sys_get_in_fwd_rules_supported (const MRP_CONST_CHANNEL_TYPE_COMMON pCh)
{
  MRP_IF_DB_TYPE *pDBif = LSA_NULL;
  LSA_UINT8 inFwdRulesSupported;

  inFwdRulesSupported = MRP_IN_FWD_RULES_SUPPORTED_OFF;

  if (!LSA_HOST_PTR_ARE_EQUAL (pCh, LSA_NULL))
  {
    if (!LSA_HOST_PTR_ARE_EQUAL (pCh->pChSysDB, LSA_NULL))
    {
      pDBif = (MRP_IF_DB_TYPE *)(pCh->pChSysDB->pInterfacePara);

      if (!LSA_HOST_PTR_ARE_EQUAL (pDBif, LSA_NULL))
      {
        inFwdRulesSupported = pDBif->mrpInFwdRulesSupported;
      }
    }
  }

  return inFwdRulesSupported;
}


/*===========================================================================
* FUNCTION : mrp_in_rx_from_different_interconn
*----------------------------------------------------------------------------
* PURPOSE  : check, if a received frame was sent from another MRP
*            interconnection device
*----------------------------------------------------------------------------
* RETURNS  : LSA_TRUE:  different MRP_IN instance has sent the frame
*            LSA_FALSE: local MRP_IN instance has sent the frame
*----------------------------------------------------------------------------
* INPUTS   : *pDBin         = pointer to one instance of MRP_IN database
* OUTPUTS  : *pDifferentMac = LSA_TRUE: receive frame has a different MAC address
*                             LSA_FALSE: receive frame has the same MAC address
*            *pDifferentInId= LSA_TRUE: receive frame has a different InId
*                             LSA_FALSE: receive frame has the same InId
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_BOOL mrp_in_rx_from_different_interconn (MRP_IN_DB_TYPE *pDBin, LSA_BOOL *pDifferentMac, LSA_BOOL *pDifferentInId)
{
  LSA_INT32 macCmpDifference;

  macCmpDifference = MRP_MEMCMP (pDBin->rxPara.RxSA, pDBin->own_SA, 6);

  if (!macCmpDifference)
    *pDifferentMac = LSA_FALSE;
  else
    *pDifferentMac = LSA_TRUE;

  if (pDBin->InID == pDBin->rxPara.RxInId)
  {
    *pDifferentInId = LSA_FALSE;
  }
  else
  {
    *pDifferentInId = LSA_TRUE;
  }

  return (*pDifferentInId | *pDifferentMac);
}

/*===========================================================================
* FUNCTION : mrp_inLnkChg_forward_is_needed
*----------------------------------------------------------------------------
* PURPOSE  : check, if duplicates of MRP InLnkChg frames have to be eliminated
*            (MRP interconnection forward request for a received InLnkChg frame
*            is not necessary)
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of mrp interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_BOOL mrp_inLnkChg_forward_is_needed (const MRP_IN_DB_TYPE *pDBin)
{
  LSA_BOOL isInFwdNeeded = LSA_TRUE;

  /* if the same frame is received again with the same interval value (e.g. MRP ring is closed, MIC/MRA combination at the coupling device) -> don't forward it once again */
  if (pDBin->rxPara.RxPortRole == pDBin->InTxInfo.portRole &&
      (pDBin->rxPara.RxInId    == pDBin->InTxInfo.inId     && (pDBin->rxPara.RxTimeTcnt * MRP_TIMER_TIME_BASE_DEZ) == pDBin->InTxInfo.intervalTimeMs))
    isInFwdNeeded = LSA_FALSE;

  return isInFwdNeeded;
}


/*===========================================================================
* FUNCTION : mrp_in_inLnkChg_forward_to_mrp
*----------------------------------------------------------------------------
* PURPOSE  : MRP interconnection forward request for a received InLinkChange frame
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin    = pointer to one instance of mrp interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_inLnkChg_forward_to_mrp (MRP_IN_DB_TYPE *pDBin)
{
  MRP_PROGRAM_TRACE_02 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_inLnkChg_forward_to_mrp[inInst=%d] entered: rxPort=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort);

  pDBin->InTxInfo.rxPort         = pDBin->rxPara.RxPort;
  pDBin->InTxInfo.isRstpOriginated = LSA_FALSE;

  pDBin->InTxInfo.inInstance     = pDBin->pInstance[MRP_IN_INSTANCE_INDEX];
  pDBin->InTxInfo.inId           = pDBin->rxPara.RxInId;

  pDBin->InTxInfo.inFrameType    = MRP_IN_FRAME_TYPE_LINK_CHANGE;

  pDBin->InTxInfo.intervalTimeMs = (LSA_UINT16) (pDBin->rxPara.RxTimeTcnt * MRP_TIMER_TIME_BASE_DEZ);
  pDBin->InTxInfo.inLinkState    = (MRP_LINK_STATE_TYPE) pDBin->rxPara.LinkState;
  pDBin->InTxInfo.portRole       = pDBin->rxPara.RxPortRole;
  pDBin->InTxInfo.inLinkRole     = pDBin->rxPara.RxLnkRole;

  MRP_MEMCPY (pDBin->InTxInfo.rxSA, pDBin->rxPara.RxSA, MRP_PACKET_VAL_MAC_ADDR_LEN);
  MRP_MEMCPY (pDBin->InTxInfo.domainUUID, pDBin->rxPara.RxUUID, sizeof (pDBin->InTxInfo.domainUUID)); /* DomainUUID of the InLnkChg originating device */

  mrp_in_mrp_fwd_request (pDBin);
  /* trigger MRP statemachine to forward InPoll frames */
  /* MRM/MRA: forward InPoll frames */
  /* MRC:     forward InPoll frames */
}



/*===========================================================================
* FUNCTION : mrp_in_inLnkChg_forward_to_iport
*----------------------------------------------------------------------------
* PURPOSE  : MRP interconnection forward request for a received InLinkChange 
*            frame
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin    = pointer to one instance of mrp interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_inLnkChg_forward_to_iport (MRP_IN_DB_TYPE *pDBin)
{
  MRP_EDD_LOWER_RQB_PTR_TYPE pRQB = LSA_NULL;

  MRP_PROGRAM_TRACE_02 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_in_inLnkChg_forward_to_iport[inInst=%d] entered: rxPort=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort);

  if (pDBin->pChSys->state != MRP_CH_STATE_CLOSING)
  {
    (void) (mrp_edd_get_snd_rqb (pDBin->pChSys, pDBin->pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_LNK_CHG_BOT, &pRQB, MRP_EDD_GET_SND_RQB_NO_THRESHOLD));

    if (!LSA_HOST_PTR_ARE_EQUAL (pRQB, LSA_NULL))
    {
      /* send parameter */
      MRP_IN_PACKET_STRUCT_LNK_CHG_TYPE *pFrameInLnkChg;

      pFrameInLnkChg = (MRP_IN_PACKET_STRUCT_LNK_CHG_TYPE*)MRP_EDD_LOWER_GET_NRT_TX_BUFFER (pRQB); /*lint !e826 CM 10/11/2016 cast on purpose */
      pFrameInLnkChg->payload.inId     = MRP_HTON16 (pDBin->rxPara.RxInId);
      pFrameInLnkChg->payload.portRole = MRP_HTON16 (pDBin->rxPara.RxPortRole);

      pFrameInLnkChg->payload.sequenceID = MRP_HTON16 (pDBin->pDBifRef->pSequenceID[MRP_DEFAULT_INSTANCE]);
      pFrameInLnkChg->payload.interval   = MRP_HTON16 ((LSA_UINT16) (pDBin->rxPara.RxTimeTcnt * MRP_TIMER_TIME_BASE_DEZ));

      pFrameInLnkChg->payload.linkRole = MRP_HTON16 (pDBin->rxPara.RxLnkRole);

      if (pDBin->rxPara.LinkState == MRP_LINK_STATE_UP)
      {
        pFrameInLnkChg->payload.tlv = MRP_HTON16 (MRP_IN_PACKET_VAL_TLV_LNK_UP);
      }
      else
      {
        pFrameInLnkChg->payload.tlv = MRP_HTON16 (MRP_IN_PACKET_VAL_TLV_LNK_DOWN);
      }

      MRP_MEMCPY (pFrameInLnkChg->payload.sa, pDBin->rxPara.RxSA, MRP_PACKET_VAL_MAC_ADDR_LEN);
      MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInLnkChg->payload.domainUUID, pDBin->rxPara.RxUUID, sizeof (pFrameInLnkChg->payload.domainUUID)); /* DomainUUID of the InLnkChg originating device */

      MRP_PROGRAM_TRACE_07 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_inLnkChg_forward_to_iport[inInst=%d]: inFrameType=%d, inId=%d, sequID=%u, interval=%d*10ms, origPort=%d, destPort=%d",
                            pDBin->pInstance[MRP_IN_INSTANCE_INDEX], MRP_IN_FRAME_TYPE_LINK_CHANGE, pDBin->rxPara.RxInId, pDBin->pDBifRef->pSequenceID[MRP_DEFAULT_INSTANCE], pDBin->rxPara.RxTimeTcnt, pDBin->rxPara.RxPort, pDBin->IPort);
      mrp_edd_req_snd (pRQB, pDBin->IPort, MRP_IN_PACKET_SIZE_DEFAULT, LSA_TRUE);

      pDBin->pDBifRef->pSequenceID[MRP_DEFAULT_INSTANCE]++;
    }
  }
}


/*===========================================================================
* FUNCTION : mrp_in_inPoll_forward_to_mrp
*----------------------------------------------------------------------------
* PURPOSE  : MRP interconnection forward request for a received InPoll frame
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin    = pointer to one instance of mrp interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_inPoll_forward_to_mrp (MRP_IN_DB_TYPE *pDBin)
{
  MRP_PROGRAM_TRACE_02 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_inPoll_forward_to_mrp[inInst=%d] entered: rxPort=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort);

  pDBin->InTxInfo.rxPort           = pDBin->rxPara.RxPort;
  pDBin->InTxInfo.isRstpOriginated = LSA_FALSE;

  pDBin->InTxInfo.inInstance       = pDBin->pInstance[MRP_IN_INSTANCE_INDEX];
  pDBin->InTxInfo.inId             = pDBin->rxPara.RxInId;
  pDBin->InTxInfo.inFrameType      = MRP_IN_FRAME_TYPE_LINK_STATE_POLL;
  pDBin->InTxInfo.portRole         = pDBin->rxPara.RxPortRole;

  MRP_MEMCPY (pDBin->InTxInfo.rxSA, pDBin->rxPara.RxSA, MRP_PACKET_VAL_MAC_ADDR_LEN);
  MRP_MEMCPY (pDBin->InTxInfo.domainUUID, pDBin->rxPara.RxUUID, sizeof (pDBin->InTxInfo.domainUUID));

  mrp_in_mrp_fwd_request (pDBin);
  /* trigger MRP statemachine to forward InPoll frames */
  /* MRM/MRA: forward InPoll frames */
  /* MRC:     forward InPoll frames */
}


/*===========================================================================
* FUNCTION : mrp_in_inPoll_forward_to_iport
*----------------------------------------------------------------------------
* PURPOSE  : MRP interconnection forward request for a received InPoll frame
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of mrp interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_inPoll_forward_to_iport (MRP_IN_DB_TYPE *pDBin)
{
  MRP_EDD_LOWER_RQB_PTR_TYPE pRQB = LSA_NULL;

  MRP_PROGRAM_TRACE_02 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_in_inPoll_forward_to_iport[inInst=%d] entered: rxPort=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort);

  if (pDBin->pChSys->state != MRP_CH_STATE_CLOSING)
  {
    (void) (mrp_edd_get_snd_rqb (pDBin->pChSys, pDBin->pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_POLL_BOT, &pRQB, MRP_EDD_GET_SND_RQB_NO_THRESHOLD));

    if (!LSA_HOST_PTR_ARE_EQUAL (pRQB, LSA_NULL))
    {
      /* send parameter */
      MRP_IN_PACKET_STRUCT_POLL_TYPE *pFrameInPoll;

      pFrameInPoll = (MRP_IN_PACKET_STRUCT_POLL_TYPE*)MRP_EDD_LOWER_GET_NRT_TX_BUFFER (pRQB); /*lint !e826 CM 10/11/2016 cast on purpose */
      pFrameInPoll->payload.inId       = MRP_HTON16 (pDBin->rxPara.RxInId);
      pFrameInPoll->payload.portRole   = MRP_HTON16 (pDBin->rxPara.RxPortRole);

      pFrameInPoll->payload.sequenceID = MRP_HTON16 (pDBin->pDBifRef->pSequenceID[MRP_DEFAULT_INSTANCE]);

      MRP_MEMCPY (pFrameInPoll->payload.sa, pDBin->rxPara.RxSA, MRP_PACKET_VAL_MAC_ADDR_LEN);
      MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInPoll->payload.domainUUID, pDBin->rxPara.RxUUID, sizeof (pFrameInPoll->payload.domainUUID));

      MRP_PROGRAM_TRACE_06 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_inPoll_forward_to_iport[inInst=%d]: inFrameType=%d, inId=%d, sequID=%u, origPort=%d, destPort=%d",
                            pDBin->pInstance[MRP_IN_INSTANCE_INDEX], MRP_IN_FRAME_TYPE_LINK_STATE_POLL, pDBin->rxPara.RxInId, pDBin->pDBifRef->pSequenceID[MRP_DEFAULT_INSTANCE], pDBin->rxPara.RxPort, pDBin->IPort);
      mrp_edd_req_snd (pRQB, pDBin->IPort, MRP_IN_PACKET_SIZE_DEFAULT, LSA_TRUE);

      pDBin->pDBifRef->pSequenceID[MRP_DEFAULT_INSTANCE]++;
    }
  }
}

/*===========================================================================
* FUNCTION : mrp_inTC_forward_is_needed
*----------------------------------------------------------------------------
* PURPOSE  : check, if duplicates of MRP InTC frames have to be eliminated
*            (MRP interconnection forward request for a received InTC frame
*            is not necessary)
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of mrp interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_BOOL mrp_inTC_forward_is_needed (const MRP_IN_DB_TYPE *pDBin)
{
  LSA_BOOL isInFwdNeeded = LSA_TRUE;

  /* if the same frame is received at both ring ports and with the same interval value (e.g. MRP ring is closed, MIC/MRA combination at the coupling device) -> don't forward it once again */
  if (pDBin->rxPara.RxPort != pDBin->InTxInfo.rxPort &&
      (pDBin->rxPara.RxInId == pDBin->InTxInfo.inId  && (pDBin->rxPara.RxTimeTcnt * MRP_TIMER_TIME_BASE_DEZ) == pDBin->InTxInfo.intervalTimeMs))
    isInFwdNeeded = LSA_FALSE;

  return isInFwdNeeded;
}

/*===========================================================================
* FUNCTION : mrp_in_inTC_forward_to_iport
*----------------------------------------------------------------------------
* PURPOSE  : MRP interconnection forward request for a received InTC frame
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of mrp interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_inTC_forward_to_iport (MRP_IN_DB_TYPE *pDBin)
{
  MRP_EDD_LOWER_RQB_PTR_TYPE pRQB = LSA_NULL;

  MRP_PROGRAM_TRACE_02 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_in_inTC_forward_to_iport[inInst=%d] entered: rxPort=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort);

  if (pDBin->pChSys->state != MRP_CH_STATE_CLOSING)
  {
    (void) (mrp_edd_get_snd_rqb (pDBin->pChSys, pDBin->pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_TC_BOT, &pRQB, MRP_EDD_GET_SND_RQB_NO_THRESHOLD));

    if (!LSA_HOST_PTR_ARE_EQUAL (pRQB, LSA_NULL))
    {
      /* send parameter */
      MRP_IN_PACKET_STRUCT_TPLG_CHG_TYPE *pFrameInTplgChg;

      pFrameInTplgChg = (MRP_IN_PACKET_STRUCT_TPLG_CHG_TYPE*)MRP_EDD_LOWER_GET_NRT_TX_BUFFER (pRQB); /*lint !e826 CM 10/11/2016 cast on purpose */
      pFrameInTplgChg->payload.inId       = MRP_HTON16 (pDBin->rxPara.RxInId);

      pFrameInTplgChg->payload.interval   = MRP_HTON16 ((LSA_UINT16) (pDBin->rxPara.RxTimeTcnt * MRP_TIMER_TIME_BASE_DEZ));
      pFrameInTplgChg->payload.sequenceID = MRP_HTON16 (pDBin->pDBifRef->pSequenceID[MRP_DEFAULT_INSTANCE]);

      MRP_MEMCPY (pFrameInTplgChg->payload.sa, pDBin->rxPara.RxSA, MRP_PACKET_VAL_MAC_ADDR_LEN);
      MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInTplgChg->payload.domainUUID, pDBin->DomainUUID, sizeof (pFrameInTplgChg->payload.domainUUID));

      /* Append the UUID of the MRP interconnection coupling device to the InTC frame. This is a manufacturer specific option. */
      MRP_MEMCPY ((LSA_VOID_PTR_TYPE)(&pFrameInTplgChg->payload.uSuffix.inTcOption), (LSA_VOID_PTR_TYPE)MRP_IN_PACKET_VAL_OPTION_COUPLED_UUID, sizeof (pFrameInTplgChg->payload.uSuffix.inTcOption));
      MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInTplgChg->payload.uSuffix.inTcOption.coupledDomainUUID, pDBin->CoupledDomainUUID, sizeof (pFrameInTplgChg->payload.uSuffix.inTcOption.coupledDomainUUID));

      MRP_PROGRAM_TRACE_07 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_inTC_forward_to_iport[inInst=%d]: inFrameType=%d, inId=%d, sequID=%u, interval=%d*10ms, origPort=%d, destPort=%d",
                            pDBin->pInstance[MRP_IN_INSTANCE_INDEX], MRP_IN_FRAME_TYPE_IN_TC, pDBin->rxPara.RxInId, pDBin->pDBifRef->pSequenceID[MRP_DEFAULT_INSTANCE], pDBin->rxPara.RxTimeTcnt, pDBin->rxPara.RxPort, pDBin->IPort);
      mrp_edd_req_snd (pRQB, pDBin->IPort, MRP_IN_PACKET_SIZE_IN_TPLG_CHG, LSA_TRUE);

      pDBin->pDBifRef->pSequenceID[MRP_DEFAULT_INSTANCE]++;
    }
  }
}


/*===========================================================================
* FUNCTION : mrp_in_inTC_forward_to_mrp
*----------------------------------------------------------------------------
* PURPOSE  : MRP interconnection forward request for a received InTC frame
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of mrp interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_inTC_forward_to_mrp (MRP_IN_DB_TYPE *pDBin)
{
  LSA_BOOL sendFrame = LSA_TRUE;
  
  MRP_PROGRAM_TRACE_02 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_inTC_forward_to_mrp[inInst=%d] entered: rxPort=%d", pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort);

  /* only forward InTc to MRP, if the frame is not a duplicate of an already forwarded InTc */
  if (pDBin->operRole == MRP_IN_OPER_ROLE_MIC)
  {
    if (pDBin->InTxInfo.inFrameType == MRP_IN_FRAME_TYPE_IN_TC && 
        pDBin->rxPara.RxInId        == pDBin->InTxInfo.inId    && ((pDBin->rxPara.RxTimeTcnt * MRP_TIMER_TIME_BASE_DEZ) == pDBin->InTxInfo.intervalTimeMs)
       )
      sendFrame = LSA_FALSE;
  }
  else /* pDBin->operRole = MRP_IN_OPER_ROLE_MIM or pDBin->operRole = MRP_IN_OPER_ROLE_OFF */
  {
    if (pDBin->InTcTxInfo.inFrameType == MRP_IN_FRAME_TYPE_IN_TC &&
        pDBin->rxPara.RxInId          == pDBin->InTcTxInfo.inId  && ((pDBin->rxPara.RxTimeTcnt * MRP_TIMER_TIME_BASE_DEZ) == pDBin->InTcTxInfo.intervalTimeMs)
       )
      sendFrame = LSA_FALSE;

  }
  /* only forward InTc to MRP, if the frame is not a duplicate of an already forwarded InTc */
  if (sendFrame)
  {
    pDBin->InTxInfo.rxPort = pDBin->rxPara.RxPort;
    pDBin->InTxInfo.isRstpOriginated = LSA_FALSE;

    pDBin->InTxInfo.inInstance     = pDBin->pInstance[MRP_IN_INSTANCE_INDEX];
    pDBin->InTxInfo.inId = pDBin->rxPara.RxInId;
    pDBin->InTxInfo.inFrameType = MRP_IN_FRAME_TYPE_IN_TC;
    pDBin->InTxInfo.intervalTimeMs = (LSA_UINT16) (pDBin->rxPara.RxTimeTcnt * MRP_TIMER_TIME_BASE_DEZ);

    MRP_MEMCPY (pDBin->InTxInfo.rxSA, pDBin->rxPara.RxSA, MRP_PACKET_VAL_MAC_ADDR_LEN);

    MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pDBin->InTxInfo.domainUUID, (LSA_VOID_PTR_TYPE)pDBin->DomainUUID, sizeof (pDBin->InTxInfo.domainUUID));
    MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pDBin->InTxInfo.coupledDomainUUID, (LSA_VOID_PTR_TYPE)pDBin->CoupledDomainUUID, sizeof (pDBin->InTxInfo.coupledDomainUUID));

    mrp_in_mrp_fwd_request (pDBin);
    /* trigger MRP statemachine to forward InTC frames */
    /* MRM/MRA: send TC frames */
    /* MRC:     forward InTC frames */
  }
}

/*===========================================================================
* FUNCTION : mrp_in_to_mrp_send_req
*----------------------------------------------------------------------------
* PURPOSE  : send an MRP interconnection frame on both ring ports
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : isMrcSendReq = LSA_TRUE: TX request is for MRC SM, LSA_FALSE: TX request is for MRM SM
*            *pDB         = pointer to MRC or MRM database
*            isForward    = MRP interconnection frame to be forwarded from a
*                           received frame or to be sent originally
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_to_mrp_send_req (LSA_BOOL isMrcSendReq, LSA_VOID_PTR_TYPE *pDB, LSA_BOOL isForward)
{
  LSA_UINT8 toggle;
  LSA_UINT16 rport;
  LSA_UINT16 portRole;
  LSA_UINT16 *pSequenceID;
  LSA_UINT8 *pMacAddr;
  LSA_UINT8 *pDomainUUID;
  LSA_UINT16 rPort1, rPort2;
  LSA_UINT16 PRM_RPort;
  MRP_EDD_LOWER_RQB_PTR_TYPE pRQB;
  MRP_IN_TX_REQ_INFO_TYPE *pTxInfo;
  MRP_CHANNEL_TYPE_SYSTEM pChSys;
  MRP_CH_STATE state;
  MRP_MRC_DB_TYPE *pDBmrc;
  MRP_IN_FRAME_TYPE inFrameType;
#ifdef MRP_CFG_PLUGIN_0_MRM
  MRP_MRM_DB_TYPE *pDBmrm;
#endif
  LSA_UINT16  pInstance[2];

#ifdef MRP_CFG_PLUGIN_0_MRM /* Always MRC if MRM Plugin is not active */
  if (isMrcSendReq) /* The TX request is done in MRC SM */
  {
#else
  LSA_UNUSED_ARG(isMrcSendReq);
#endif
    /*lint -e826*/
    pDBmrc = (MRP_MRC_DB_TYPE *)pDB; 

    pChSys      = pDBmrc->pChSys;

    pInstance[MRP_INSTANCE_INDEX]    = pDBmrc->pInstance[MRP_INSTANCE_INDEX];
    state       = pDBmrc->pChSys->state;
    pMacAddr    = pDBmrc->pDBifRef->macAddr;
    pSequenceID = pDBmrc->pDBifRef->pSequenceID;
    rPort1      = pDBmrc->RPort_1;
    rPort2      = pDBmrc->RPort_2;
    PRM_RPort   = pDBmrc->PRM_RPort;
    pDomainUUID = &pDBmrc->DomainUUID[0];

    pTxInfo     = pDBmrc->pMrcInTxInfo;
    inFrameType = pDBmrc->pMrcInTxInfo->inFrameType;
#ifdef MRP_CFG_PLUGIN_0_MRM
  }
  else /* The TX request is done in MRM SM. */
  {
    pDBmrm = (MRP_MRM_DB_TYPE *)pDB;

    pChSys      = pDBmrm->pChSys;
    pInstance[MRP_INSTANCE_INDEX]    = pDBmrm->pInstance[MRP_INSTANCE_INDEX];
    state       = pDBmrm->pChSys->state;
    pMacAddr    = pDBmrm->pDBifRef->macAddr;
    pSequenceID = pDBmrm->pDBifRef->pSequenceID;
    rPort1      = pDBmrm->RPort_1;
    rPort2      = pDBmrm->RPort_2;
    PRM_RPort   = pDBmrm->PRM_RPort;
    pDomainUUID = &pDBmrm->DomainUUID[0];

    pTxInfo = pDBmrm->pMrmInTxInfo;
    inFrameType = pDBmrm->pMrmInTxInfo->inFrameType;
  }
#endif

  pInstance[MRP_IN_INSTANCE_INDEX]  = pTxInfo->inInstance;

  switch (inFrameType)
  {
    case MRP_IN_FRAME_TYPE_IN_TC:
    {
      MRP_LL_SND_RQB_ENUM_T poolID;

      if (pTxInfo->isRstpOriginated == LSA_TRUE)
      {
        poolID = MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_ORIG_BOT;
      }
      else
      {
        poolID = MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_BOT;
      }

      for (toggle = 0; toggle < 2 && state != MRP_CH_STATE_CLOSING; toggle++)
      {
        (void) (mrp_edd_get_snd_rqb (pChSys, pInstance, poolID, &pRQB, MRP_EDD_GET_SND_RQB_NO_THRESHOLD));
        if (!LSA_HOST_PTR_ARE_EQUAL (pRQB, LSA_NULL))
        {
          /* send parameter */
          MRP_IN_PACKET_STRUCT_TPLG_CHG_TYPE *pFrameInTplgChg;

          pFrameInTplgChg = (MRP_IN_PACKET_STRUCT_TPLG_CHG_TYPE*)MRP_EDD_LOWER_GET_NRT_TX_BUFFER (pRQB); /*lint !e826 CM 10/11/2016 cast on purpose */
          pFrameInTplgChg->payload.inId       = MRP_HTON16 (pTxInfo->inId);

          pFrameInTplgChg->payload.interval   = MRP_HTON16 (pTxInfo->intervalTimeMs);
          pFrameInTplgChg->payload.sequenceID = MRP_HTON16 (pSequenceID[pInstance[MRP_INSTANCE_INDEX]]);

          /* The InTC frame has to be sent with the domainUUID of the MRP ring, the MRP device is residing in. */
          MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInTplgChg->payload.domainUUID, pDomainUUID, sizeof (pFrameInTplgChg->payload.domainUUID));

          /* Append the UUID of the MRP interconnection coupling device to the InTC frame. This is a manufacturer specific option. */
          MRP_MEMCPY ((LSA_VOID_PTR_TYPE)(&pFrameInTplgChg->payload.uSuffix.inTcOption), (LSA_VOID_PTR_TYPE)MRP_IN_PACKET_VAL_OPTION_COUPLED_UUID, sizeof (pFrameInTplgChg->payload.uSuffix.inTcOption));
          MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInTplgChg->payload.uSuffix.inTcOption.coupledDomainUUID, pTxInfo->coupledDomainUUID, sizeof (pFrameInTplgChg->payload.uSuffix.inTcOption.coupledDomainUUID));

          if (isForward)
          {
            /* in case of forwarding an MRP interconnection frame, the original MRP interface source MAC address of the sending device has to be kept */
            MRP_MEMCPY (pFrameInTplgChg->payload.sa, pTxInfo->rxSA, MRP_PACKET_VAL_MAC_ADDR_LEN);
          }
          else
          {
            MRP_MEMCPY (pFrameInTplgChg->payload.sa, pMacAddr, MRP_PACKET_VAL_MAC_ADDR_LEN);
          }

          if (toggle & 1)
          {
            rport = rPort1;
          }
          else
          {
            rport = rPort2;
          }
          MRP_PROGRAM_TRACE_07 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_to_mrp_send_req[inst=%d/inInst=%d]: IN_TPLG_CHG(inId=%d, interval=%d ms, sequID=%u), origPort=%d, destPort=%d",
                                pInstance[MRP_INSTANCE_INDEX], pInstance[MRP_IN_INSTANCE_INDEX], pTxInfo->inId, pTxInfo->intervalTimeMs, pSequenceID[pInstance[MRP_INSTANCE_INDEX]], pTxInfo->rxPort, rport);
          mrp_edd_req_snd (pRQB, rport, MRP_IN_PACKET_SIZE_IN_TPLG_CHG, LSA_TRUE);

          pSequenceID[pInstance[MRP_INSTANCE_INDEX]]++;        
        }
      }
    }
    break;

    case MRP_IN_FRAME_TYPE_LINK_CHANGE:
    {
      for (toggle = 0; toggle < 2 && state != MRP_CH_STATE_CLOSING; toggle++)
      {
        (void) (mrp_edd_get_snd_rqb (pChSys, pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_LNK_CHG_BOT, &pRQB, MRP_EDD_GET_SND_RQB_NO_THRESHOLD));
        if (!LSA_HOST_PTR_ARE_EQUAL (pRQB, LSA_NULL))
        {
          /* send parameter */
          MRP_IN_PACKET_STRUCT_LNK_CHG_TYPE *pFrameInLnkChg;

          pFrameInLnkChg = (MRP_IN_PACKET_STRUCT_LNK_CHG_TYPE*)MRP_EDD_LOWER_GET_NRT_TX_BUFFER (pRQB); /*lint !e826 CM 10/11/2016 cast on purpose */

          if (pTxInfo->inLinkState == MRP_LINK_STATE_UP)
          {
            pFrameInLnkChg->payload.tlv = MRP_HTON16 (MRP_IN_PACKET_VAL_TLV_LNK_UP);
          }
          else
          {
            pFrameInLnkChg->payload.tlv = MRP_HTON16 (MRP_IN_PACKET_VAL_TLV_LNK_DOWN);
          }

          pFrameInLnkChg->payload.inId       = MRP_HTON16 (pTxInfo->inId);
          pFrameInLnkChg->payload.interval   = MRP_HTON16 (pTxInfo->intervalTimeMs);
          pFrameInLnkChg->payload.linkRole   = MRP_HTON16 (pTxInfo->inLinkRole);
          pFrameInLnkChg->payload.sequenceID = MRP_HTON16 (pSequenceID[pInstance[MRP_INSTANCE_INDEX]]);

          if (toggle & 1)
          {
            rport = rPort1;
          }
          else
          {
            rport = rPort2;
          }

          if (isForward)
          {
            /* in case of forwarding an MRP interconnection frame, the original MRP interface source MAC address of the sending device has to be kept */
            portRole = pTxInfo->portRole;
            MRP_MEMCPY (pFrameInLnkChg->payload.sa, pTxInfo->rxSA, MRP_PACKET_VAL_MAC_ADDR_LEN);
            MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInLnkChg->payload.domainUUID, pTxInfo->domainUUID, sizeof (pFrameInLnkChg->payload.domainUUID));
          }
          else
          {
            if (rport == PRM_RPort)
            {
              portRole = MRP_IN_PACKET_VAL_PORT_ROLE_PRM;
            }
            else
            {
              portRole = MRP_IN_PACKET_VAL_PORT_ROLE_SEC;
            }

            MRP_MEMCPY (pFrameInLnkChg->payload.sa, pMacAddr, MRP_PACKET_VAL_MAC_ADDR_LEN);
            MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInLnkChg->payload.domainUUID, pDomainUUID, sizeof (pFrameInLnkChg->payload.domainUUID));
          }
          pFrameInLnkChg->payload.portRole = MRP_HTON16 (portRole);

          MRP_PROGRAM_TRACE_09 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_to_mrp_send_req[inst=%d/inInst=%d]: IN_LNK_CHG(inId=%d, interval=%d ms, portRole=%d, lnkRole=0x%x, sequID=%u), origPort=%d, destPort=%d", pInstance[MRP_INSTANCE_INDEX], pTxInfo->inInstance, pTxInfo->inId, pTxInfo->intervalTimeMs, portRole, pTxInfo->inLinkRole, pSequenceID[pInstance[MRP_INSTANCE_INDEX]], pTxInfo->rxPort, rport);
          mrp_edd_req_snd (pRQB, rport, MRP_IN_PACKET_SIZE_DEFAULT, LSA_TRUE);

          pSequenceID[pInstance[MRP_INSTANCE_INDEX]]++;
        }
      }
    }
    break;

    case MRP_IN_FRAME_TYPE_LINK_STATE_POLL:
    {
      for (toggle = 0; toggle < 2 && state != MRP_CH_STATE_CLOSING; toggle++)
      {
        (void) (mrp_edd_get_snd_rqb (pChSys, pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_POLL_BOT, &pRQB, MRP_EDD_GET_SND_RQB_NO_THRESHOLD));
        if (!LSA_HOST_PTR_ARE_EQUAL (pRQB, LSA_NULL))
        {
          /* send parameter */
          MRP_IN_PACKET_STRUCT_POLL_TYPE *pFrameInPoll;

          pFrameInPoll = (MRP_IN_PACKET_STRUCT_POLL_TYPE*)MRP_EDD_LOWER_GET_NRT_TX_BUFFER (pRQB); /*lint !e826 CM 10/11/2016 cast on purpose */

          pFrameInPoll->payload.inId       = MRP_HTON16 (pTxInfo->inId);
          pFrameInPoll->payload.sequenceID = MRP_HTON16 (pSequenceID[pInstance[MRP_INSTANCE_INDEX]]);

          if (toggle & 1)
          {
            rport = rPort1;
          }
          else
          {
            rport = rPort2;
          }

          if (isForward)
          {
            /* in case of forwarding an MRP interconnection frame, the original MRP interface source MAC address of the sending device has to be kept */
            portRole = pTxInfo->portRole;
            MRP_MEMCPY (pFrameInPoll->payload.sa, pTxInfo->rxSA, MRP_PACKET_VAL_MAC_ADDR_LEN);
            MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInPoll->payload.domainUUID, pTxInfo->domainUUID, sizeof (pFrameInPoll->payload.domainUUID));
          }
          else
          {
            if (rport == PRM_RPort)
            {
              portRole = MRP_IN_PACKET_VAL_PORT_ROLE_PRM;
            }
            else
            {
              portRole = MRP_IN_PACKET_VAL_PORT_ROLE_SEC;
            }

            MRP_MEMCPY (pFrameInPoll->payload.sa, pMacAddr, MRP_PACKET_VAL_MAC_ADDR_LEN);
            MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInPoll->payload.domainUUID, pDomainUUID, sizeof (pFrameInPoll->payload.domainUUID));
          }
          pFrameInPoll->payload.portRole = MRP_HTON16 (portRole);

          MRP_PROGRAM_TRACE_07 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_to_mrp_send_req[inst=%d/inInst=%d]: IN_POLL(inId=%d, portRole=%d, sequID=%u), origPort=%d, destPort=%d",
                                pInstance[MRP_INSTANCE_INDEX], pInstance[MRP_IN_INSTANCE_INDEX], pTxInfo->inId, portRole, pSequenceID[pInstance[MRP_INSTANCE_INDEX]], pTxInfo->rxPort, rport);
          mrp_edd_req_snd (pRQB, rport, MRP_IN_PACKET_SIZE_DEFAULT, LSA_TRUE);

          pSequenceID[pInstance[MRP_INSTANCE_INDEX]]++;
        }
      }
    }
    break;

    case MRP_IN_FRAME_TYPE_NONE:
      MRP_FATAL;
    break;

    default:
    break;
  }
}


/*===========================================================================
* FUNCTION : mrp_in_mrp_fwd_request
*----------------------------------------------------------------------------
* PURPOSE  : forward a received MRP interconnection frame to MRP SM
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   :  pDBin     = pointer to MRP interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mrp_fwd_request (MRP_IN_DB_TYPE *pDBin)
{
  MRP_MODE_DATA_TYPE mrpData;
  MRP_MRC_DB_TYPE *pDBmrc;
#ifdef MRP_CFG_PLUGIN_0_MRM
  MRP_MRM_DB_TYPE *pDBmrm;
  MRP_MRM_DB_TYPE *pDBmrmBase = (MRP_MRM_DB_TYPE *)pDBin->pChSys->pChSysDB->pMrm;
#endif
  MRP_MRC_DB_TYPE *pDBmrcBase = (MRP_MRC_DB_TYPE *)pDBin->pChSys->pChSysDB->pMrc;

#ifdef MRP_CFG_PLUGIN_0_MRM
  pDBmrm = &pDBmrmBase[pDBin->pInstance[MRP_INSTANCE_INDEX]];
#endif
  pDBmrc = &pDBmrcBase[pDBin->pInstance[MRP_INSTANCE_INDEX]];

  MRP_PROGRAM_TRACE_01 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_mrp_fwd_request[inInst=%d] entered", pDBin->pInstance[MRP_IN_INSTANCE_INDEX]);

  mrp_sys_get_mrp_mode (pDBin->pChSys, pDBin->pInstance[MRP_INSTANCE_INDEX], &mrpData);

  if (mrpData.mrpMode == MRP_MODE_MRM_ON)
  {
    if (mrpData.mrpOperRole == MRP_OPER_ROLE_CLIENT) /* MRA is active in operational role MRC */
    {
      pDBmrc->pMrcInTxInfo = &pDBin->InTxInfo;
      mrp_mrc_update (pDBmrc, MRP_MRC_EVENT_T_IN_FWD_REQ);
    }
    else /* MRA/MRM is active in operational role MRA/MRM */
    {
#ifdef MRP_CFG_PLUGIN_0_MRM
      pDBmrm->pMrmInTxInfo = &pDBin->InTxInfo;
      mrp_mrm_update (pDBmrm, MRP_MRM_EVENT_T_IN_FWD_REQ);
#endif
    }
  }
  else /* MRC module is active */
  {
    if (mrpData.mrpMode == MRP_MODE_MRC_ON)
    {
      pDBmrc->pMrcInTxInfo = &pDBin->InTxInfo;
      mrp_mrc_update (pDBmrc, MRP_MRC_EVENT_T_IN_FWD_REQ);
    }
    else
    {
      MRP_PROGRAM_TRACE_01 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_in_mrp_fwd_request[inInst=%d] called while MRP is OFF", pDBin->pInstance[MRP_IN_INSTANCE_INDEX]);
    }
  }
}


/*===========================================================================
* FUNCTION : mrp_in_mrp_tx_request
*----------------------------------------------------------------------------
* PURPOSE  : send MRP interconnection frames on MRP ring ports
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   :  pDBin     = pointer to MRP interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_mrp_tx_request (MRP_IN_DB_TYPE *pDBin)
{
  MRP_MODE_DATA_TYPE mrpData;
  MRP_MRC_DB_TYPE *pDBmrc;
#ifdef MRP_CFG_PLUGIN_0_MRM
  MRP_MRM_DB_TYPE *pDBmrm;
  MRP_MRM_DB_TYPE *pDBmrmBase = (MRP_MRM_DB_TYPE *)pDBin->pChSys->pChSysDB->pMrm;
#endif
  MRP_MRC_DB_TYPE *pDBmrcBase = (MRP_MRC_DB_TYPE *)pDBin->pChSys->pChSysDB->pMrc;

#ifdef MRP_CFG_PLUGIN_0_MRM
  pDBmrm = &pDBmrmBase[pDBin->pInstance[MRP_INSTANCE_INDEX]];
#endif
  pDBmrc = &pDBmrcBase[pDBin->pInstance[MRP_INSTANCE_INDEX]];

  MRP_PROGRAM_TRACE_01 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_mrp_tx_request[inInst=%d]", pDBin->pInstance[MRP_IN_INSTANCE_INDEX]);

  mrp_sys_get_mrp_mode (pDBin->pChSys, pDBin->pInstance[MRP_INSTANCE_INDEX], &mrpData);

  if (mrpData.mrpMode == MRP_MODE_MRM_ON) /* MRM module is active */
  {
    if (mrpData.mrpOperRole == MRP_OPER_ROLE_CLIENT) /* MRA is active in operational role MRC */
    {
      pDBmrc->pMrcInTxInfo = &pDBin->InTxInfo;
      mrp_mrc_update (pDBmrc, MRP_MRC_EVENT_T_IN_TX_REQ);
    }
    else /* MRA/MRM is active in operational role MRA/MRM */
    {
#ifdef MRP_CFG_PLUGIN_0_MRM
      pDBmrm->pMrmInTxInfo = &pDBin->InTxInfo;
      mrp_mrm_update (pDBmrm, MRP_MRM_EVENT_T_IN_TX_REQ);
#endif
    }
  }
  else /* MRC module is active */
  {
    if (mrpData.mrpMode == MRP_MODE_MRC_ON)
    {
      pDBmrc->pMrcInTxInfo = &pDBin->InTxInfo;
      mrp_mrc_update (pDBmrc, MRP_MRC_EVENT_T_IN_TX_REQ);
    }
    else
    {
      MRP_PROGRAM_TRACE_01 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_in_mrp_tx_request[inInst=%d] called while MRP is OFF", pDBin->pInstance[MRP_IN_INSTANCE_INDEX]);
    }
  }
}


/*===========================================================================
* FUNCTION : mrp_in_edd_req_in_rstpp_tplg_chg_req
*----------------------------------------------------------------------------
* PURPOSE  : queue interconnection topology change snd requests
*            originated by RSTPP
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference for edd system channel
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            rqbNum   = edd req reference
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS : due to synchronization with lower layer send request must
*            wait for acknowledged edd requests.
*==========================================================================*/
LSA_VOID mrp_in_edd_req_in_rstpp_tplg_chg_req (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum)
{
  LSA_UNUSED_ARG (rqbNum);
  LSA_UNUSED_ARG(pInstance);   /* hhr: to please lint in case traces are disabled. */

  MRP_PROGRAM_TRACE_01 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_edd_req_in_rstpp_tplg_chg_req[inInst=%d] entered", pInstance[MRP_IN_INSTANCE_INDEX]);

  mrp_in_rstpp_inTC_send_to_mrp (pChSys);

  mrp_ll_burst_do (pChSys);
}


/*===========================================================================
* FUNCTION : mrp_in_queue_rstpp_in_tc_req
*----------------------------------------------------------------------------
* PURPOSE  : queue a burst to send InTC requests
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of MRP_IN database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_queue_rstpp_in_tc_req (const MRP_IN_DB_TYPE *pDBin)
{
  MRP_EDD_DB_TYPE *pDBedd = (MRP_EDD_DB_TYPE *)pDBin->pChSys->pChSysDB->pEdd;

  if (!pDBedd->pInstData[pDBin->pInstance[MRP_INSTANCE_INDEX]].TplgChgRunning)
    mrp_ll_in_burst_req (pDBin->pChSys, pDBin->pInstance, g_MrpllReqTbl_mrp_in_rstpp_tplg_chg_req, LSA_FALSE);
  else
  {
    MRP_PROGRAM_TRACE_02 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[inst=%d/inInst=%d] mrp_in_queue_rstpp_in_tc_req skipped, because FDB deletion is already running", pDBin->pInstance[MRP_INSTANCE_INDEX], pDBin->pInstance[MRP_IN_INSTANCE_INDEX]);
  }
}

/*===========================================================================
* FUNCTION : mrp_in_check_redstate_change
*----------------------------------------------------------------------------
* PURPOSE  : compares last indicated MRP interconnection redundancy state with 
*            current MRP interconnection redundancy state
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
LSA_UINT8 mrp_in_check_redstate_change (const MRP_CONST_CHANNEL_TYPE_USER pChPrivUsr, LSA_UINT16 instance, const MRP_OPC_RED_STATE_INDICATION_TYPE *pMrpRedStateCurrent)
{
  LSA_UINT8 inChanged;
  LSA_UINT16 mrpInMaxInstance, i;
  MRP_OPC_RED_STATE_INDICATION_TYPE *pRedStateIndLastSignaled;
  MRP_USR_DB_TYPE *pDBusr = (MRP_USR_DB_TYPE *)pChPrivUsr->pChUsrDB->pUsr;

  inChanged = LSA_FALSE;

  mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pChPrivUsr);

  if (mrpInMaxInstance)
  {
    pRedStateIndLastSignaled = &pDBusr->pInstData[instance].RedStateIndLastSignaled.redStateInd;

    for (i = 0; i < mrpInMaxInstance; i++)
    {
      if (pRedStateIndLastSignaled->inState[i].initState  != pMrpRedStateCurrent->inState[i].initState  ||
          pRedStateIndLastSignaled->inState[i].startState != pMrpRedStateCurrent->inState[i].startState ||
          pRedStateIndLastSignaled->inState[i].inId       != pMrpRedStateCurrent->inState[i].inId       ||
          pRedStateIndLastSignaled->inState[i].inPortId   != pMrpRedStateCurrent->inState[i].inPortId)
      {
        inChanged = LSA_TRUE;
        break;
      }
    }
  }

  return inChanged;
}

/*===========================================================================
* FUNCTION : mrp_in_write_red_init_state
*----------------------------------------------------------------------------
* PURPOSE  : write the current MRP interconnection init state to usr database
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChUsr       = channel reference to user channel
*            pInstance    = affected instance in array format (contains 
*                            MRP instance and MRP interconnection instance)
*            initState    = MRP interconnection init module, which has to be set/reset
*            setFlag      = LSA_TRUE := set initState, LSA_FALSE := reset initState
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_write_red_init_state (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, const LSA_UINT16 *pInstance, MRP_USR_ENUM_RED_IN_STATE_TYPE initState, LSA_UINT8 setFlag)
{
  MRP_CHANNEL_TYPE_USER pChPrmUsr;
  MRP_USR_DB_TYPE *pDBusr;
  MRP_RED_IN_STATE_TYPE *pDBusrInState;
  LSA_UINT16 mrpInMaxInstance;
  LSA_UINT8 change = LSA_TRUE;

  pChPrmUsr = mrp_usr_get_prm_usr_channel (pChUsr->detailptr);

  if (!LSA_HOST_PTR_ARE_EQUAL (pChPrmUsr, LSA_NULL))
  {
    /* the init state must only be written, if this function is called during establishment of PRM user channel */
    if (pChUsr->detailptr->PathType == MRP_PATH_TYPE_USER && pChUsr->detailptr->type.user.isPrmInterface == MRP_IS_PRM_INTERFACE_ON)
    {
      pDBusr = (MRP_USR_DB_TYPE *)pChUsr->pChUsrDB->pUsr;

      mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pChUsr);

      if (mrpInMaxInstance)
      {
        pDBusrInState = &pDBusr->pInstData[pInstance[MRP_INSTANCE_INDEX]].RedStateIndCurrent.redStateInd.inState[pInstance[MRP_IN_INSTANCE_INDEX]];

        MRP_PROGRAM_TRACE_05 (pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_in_write_red_init_state[inst=%d/inInst=%d]: initState to change=0x%x, setFlag=%d, current initState=0x%x",
                              pInstance[MRP_INSTANCE_INDEX], pInstance[MRP_IN_INSTANCE_INDEX], initState, setFlag, pDBusrInState->initState);

        if (setFlag)
        {
          if ((pDBusrInState->initState & initState) == initState)
            change = LSA_FALSE;

          pDBusrInState->initState |= initState;
        }
        else
        {
          if ((pDBusrInState->initState & initState) == 0)
            change = LSA_FALSE;

          pDBusrInState->initState &= ~initState;
        }

        if (change)
        {
          MRP_PROGRAM_TRACE_03 (pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_write_red_init_state[inst=%d/inInst=%d]: new current initState=0x%x",
                                pInstance[MRP_INSTANCE_INDEX], pInstance[MRP_IN_INSTANCE_INDEX], pDBusrInState->initState);
        }
      }
    }
    else
      MRP_PROGRAM_TRACE_02 (pChUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_in_write_red_init_state[inst=%d/inInst=%d]: pChPrmUsr must not be NULL", pInstance[MRP_INSTANCE_INDEX], pInstance[MRP_IN_INSTANCE_INDEX]);
  }
}

/*===========================================================================
* FUNCTION : mrp_in_write_red_start_state
*----------------------------------------------------------------------------
* PURPOSE  : write the current MRP interconnection start state to usr database
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin       = pointer to one instance of mrp interconnection database
*            startState   = MRP interconnection startState, which has to be set/reset
*            inId         = MRP interconnection id of the affected
*                           MRP interconnection instance
*            inPortId     = MRP interconnection port
*            setFlag      = LSA_TRUE := set initState, LSA_FALSE := reset initState
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_write_red_start_state (const MRP_IN_DB_TYPE *pDBin, MRP_USR_ENUM_RED_IN_STATE_TYPE startState, LSA_UINT16 inId, LSA_UINT16 inPortId, LSA_UINT8 setFlag)
{
  MRP_CHANNEL_TYPE_USER pChPrmUsr;
  MRP_RED_IN_STATE_TYPE *pDBusrInState;
  LSA_UINT16 mrpInMaxInstance;
  MRP_USR_DB_TYPE *pDBusr;
  LSA_UINT8 change = LSA_TRUE;

  const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys = pDBin->pChSys;
  const LSA_UINT16 *pInstance = pDBin->pInstance;

  pChPrmUsr = mrp_usr_get_prm_usr_channel (pChSys->detailptr);

  if (!LSA_HOST_PTR_ARE_EQUAL (pChPrmUsr, LSA_NULL))
  {
    /* PRM is only possible, if a PRM user channel was established before -> startState data are linked to PRM user channel */
    /* If the startState is linked to usr2 channel, some of the provisioning action would probably have been completed before. */
    pDBusr = (MRP_USR_DB_TYPE *)pChPrmUsr->pChUsrDB->pUsr;

    mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pChSys);

    if (mrpInMaxInstance)
    {
      pDBusrInState = &pDBusr->pInstData[pInstance[MRP_INSTANCE_INDEX]].RedStateIndCurrent.redStateInd.inState[pInstance[MRP_IN_INSTANCE_INDEX]];

      MRP_PROGRAM_TRACE_05 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_in_write_red_start_state[inst=%d/inInst=%d]: startState to change=0x%x, setFlag=%d, current startState=0x%x",
                            pInstance[MRP_INSTANCE_INDEX], pInstance[MRP_IN_INSTANCE_INDEX], startState, setFlag, pDBusrInState->startState);

      if (setFlag)
      {
        if ((pDBusrInState->startState & startState) == startState)
          change = LSA_FALSE;

        pDBusrInState->startState |= startState;
      }
      else /* setFlag == LSA_FALSE */
      {
        if ((pDBusrInState->startState & startState) == 0)
          change = LSA_FALSE;

        pDBusrInState->startState &= ~startState;
      }


      /* neither MIM nor MIC active at this instance */
      if (!pDBusrInState->startState)
      {
        pDBusrInState->inId     = 0;
        pDBusrInState->inPortId = 0;
      }
      else /* any other parameters changed ??? */
      {
        pDBusrInState->inId     = inId;
        pDBusrInState->inPortId = inPortId;
      }

      if (change)
      {
        MRP_PROGRAM_TRACE_05 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_write_red_start_state[inst=%d/inInst=%d]: new current startState=0x%x, inId=%d, inPortId=%d",
                              pInstance[MRP_INSTANCE_INDEX], pInstance[MRP_IN_INSTANCE_INDEX], pDBusrInState->startState, pDBusrInState->inId, pDBusrInState->inPortId);
      }
    }
  }
  else
    MRP_PROGRAM_TRACE_02 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_in_write_red_start_state[inst=%d/inInst=%d]: pChPrmUsr must not be NULL", pInstance[MRP_INSTANCE_INDEX], pInstance[MRP_IN_INSTANCE_INDEX]);
}

/*===========================================================================
* FUNCTION : mrp_in_write_in_rx_port
*----------------------------------------------------------------------------
* PURPOSE  : write the current receive port of MRP_INTRF frames
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of mrp interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_write_in_rx_port(const MRP_IN_DB_TYPE *pDBin)
{
  LSA_UINT16 mrpInMaxInstance;
  MRP_CHANNEL_TYPE_USER pChPrmUsr;
  MRP_USR_DB_TYPE *pDBusr;
  MRP_IN_INST_EVENT_DATA *pDBinInstEvent;

  const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys = pDBin->pChSys;

  pChPrmUsr = mrp_usr_get_prm_usr_channel (pChSys->detailptr);

  if (!LSA_HOST_PTR_ARE_EQUAL(pChPrmUsr, LSA_NULL))
  {
    /* PRM is only possible, if a PRM user channel was established before -> binding of inEvent data to this channel ensures that no event data is lost */
    pDBusr = (MRP_USR_DB_TYPE *)pChPrmUsr->pChUsrDB->pUsr;

    mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances(pChSys);

    if (mrpInMaxInstance)
    {
      pDBinInstEvent = &pDBusr->pInstData[pDBin->pInstance[MRP_INSTANCE_INDEX]].pInInstEventData[pDBin->pInstance[MRP_IN_INSTANCE_INDEX]];
      if (pDBin->rxPara.RxPort != pDBinInstEvent->InEventIndCurrent.inStateInd.inRxPort)
      {
        MRP_PROGRAM_TRACE_03(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_in_write_in_rx_port[inInst=%d]: rxPort to change=0x%x, current rxPort=0x%x",
          pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxPort, pDBinInstEvent->InEventIndCurrent.inStateInd.inRxPort);

        pDBinInstEvent->InEventIndCurrent.inStateInd.inRxPort = pDBin->rxPara.RxPort;
      }
    }
  }
  else
  {
    MRP_PROGRAM_TRACE_01 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_in_write_in_rx_port[inInst=%d]: pChPrmUsr must not be NULL", pDBin->pInstance[MRP_INSTANCE_INDEX]);
  }
}

/*===========================================================================
* FUNCTION : mrp_in_write_in_rx_inid
*----------------------------------------------------------------------------
* PURPOSE  : write the current InID where MRP_INTRF frames are received from
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of mrp interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_write_in_rx_inid(const MRP_IN_DB_TYPE *pDBin)
{
  LSA_UINT16 mrpInMaxInstance;
  MRP_CHANNEL_TYPE_USER pChPrmUsr;
  MRP_USR_DB_TYPE *pDBusr;
  MRP_IN_INST_EVENT_DATA *pDBinInstEvent;

  const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys = pDBin->pChSys;

  pChPrmUsr = mrp_usr_get_prm_usr_channel (pChSys->detailptr);

  if (!LSA_HOST_PTR_ARE_EQUAL(pChPrmUsr, LSA_NULL))
  {
    /* PRM is only possible, if a PRM user channel was established before -> binding of inEvent data to this channel ensures that no event data is lost */
    pDBusr = (MRP_USR_DB_TYPE *)pChPrmUsr->pChUsrDB->pUsr;

    mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances(pChSys);

    if (mrpInMaxInstance)
    {
      pDBinInstEvent = &pDBusr->pInstData[pDBin->pInstance[MRP_INSTANCE_INDEX]].pInInstEventData[pDBin->pInstance[MRP_IN_INSTANCE_INDEX]];
      if (pDBin->rxPara.RxInId != pDBinInstEvent->InEventIndCurrent.inStateInd.inRxInID)
      {
        MRP_PROGRAM_TRACE_03(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_in_write_in_rx_inid[inInst=%d]: inRxInID to change=0x%x, current inRxInID=0x%x",
          pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBin->rxPara.RxInId, pDBinInstEvent->InEventIndCurrent.inStateInd.inRxInID);

        pDBinInstEvent->InEventIndCurrent.inStateInd.inRxInID = pDBin->rxPara.RxInId;
      }
    }
  }
  else
  {
    MRP_PROGRAM_TRACE_01 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_in_write_in_rx_inid[inInst=%d]: pChPrmUsr must not be NULL", pDBin->pInstance[MRP_IN_INSTANCE_INDEX]);
  }
}

/*===========================================================================
* FUNCTION : mrp_in_write_in_peer_oper_role
*----------------------------------------------------------------------------
* PURPOSE  : write the current MRP Interconnection peer oper role
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of mrp interconnection database
*            inPeerOperRole = oper role of the peer interconnection device
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_write_in_peer_oper_role(const MRP_IN_DB_TYPE *pDBin, LSA_UINT16 inPeerOperRole)
{
  LSA_UINT16 mrpInMaxInstance;
  MRP_CHANNEL_TYPE_USER pChPrmUsr;
  MRP_USR_DB_TYPE *pDBusr;
  MRP_IN_INST_EVENT_DATA *pDBinInstEvent;
    
  const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys = pDBin->pChSys; 

  pChPrmUsr = mrp_usr_get_prm_usr_channel (pChSys->detailptr);

  if (!LSA_HOST_PTR_ARE_EQUAL(pChPrmUsr, LSA_NULL))
  {
    /* PRM is only possible, if a PRM user channel was established before -> binding of inEvent data to this channel ensures that no event data is lost */
    pDBusr = (MRP_USR_DB_TYPE *)pChPrmUsr->pChUsrDB->pUsr;

    mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances(pChSys);

    if (mrpInMaxInstance)
    {
      pDBinInstEvent = &pDBusr->pInstData[pDBin->pInstance[MRP_INSTANCE_INDEX]].pInInstEventData[pDBin->pInstance[MRP_IN_INSTANCE_INDEX]];
      if (inPeerOperRole != pDBinInstEvent->InEventIndCurrent.inStateInd.inPeerOperRole)
      {
        MRP_PROGRAM_TRACE_03(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_in_write_in_peer_oper_role[inInst=%d]: inPeerOperRole to change=0x%x, current inPeerOperRole=0x%x",
          pDBin->pInstance[MRP_IN_INSTANCE_INDEX], inPeerOperRole, pDBinInstEvent->InEventIndCurrent.inStateInd.inPeerOperRole);

        pDBinInstEvent->InEventIndCurrent.inStateInd.inPeerOperRole = inPeerOperRole;
      }
    }
  }
  else
  {
    MRP_PROGRAM_TRACE_01 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_in_write_in_peer_oper_role[inInst=%d]: pChPrmUsr must not be NULL", pDBin->pInstance[MRP_IN_INSTANCE_INDEX]);
  }
}

/*===========================================================================
* FUNCTION : mrp_in_write_in_in_peer_mic_position
*----------------------------------------------------------------------------
* PURPOSE  : write the current MRP Interconnection peer MIC position
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of mrp interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_write_in_in_peer_mic_position(const MRP_IN_DB_TYPE *pDBin)
{
  LSA_UINT16 mrpInMaxInstance;
  MRP_CHANNEL_TYPE_USER pChPrmUsr;
  MRP_USR_DB_TYPE *pDBusr;
  MRP_IN_INST_EVENT_DATA *pDBinInstEvent;
  LSA_UINT16 inPeerMICPosition;

  const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys = pDBin->pChSys;
  const MRP_IN_EVENT_DB_TYPE* rxPara = &(pDBin->rxPara);
  
  pChPrmUsr = mrp_usr_get_prm_usr_channel (pChSys->detailptr);

  if (!LSA_HOST_PTR_ARE_EQUAL(pChPrmUsr, LSA_NULL))
  {
    /* PRM is only possible, if a PRM user channel was established before -> binding of inEvent data to this channel ensures that no event data is lost */
    pDBusr = (MRP_USR_DB_TYPE *)pChPrmUsr->pChUsrDB->pUsr;

    mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances(pChSys);

    if (mrpInMaxInstance)
    {    
      if (rxPara->CurrentEvent == MRP_IN_EVENT_T_LNK_CHG_TRF)
      {
        if ((rxPara->RxLnkRole & 0x0001) == MRP_IN_PACKET_VAL_LNK_ROLE_BIT0_PRM && rxPara->RxPortRole == MRP_IN_PACKET_VAL_PORT_ROLE_IN)
          inPeerMICPosition = MRP_IN_VAL_IN_MIC_POS_PRM_LNK;
        else
          inPeerMICPosition = MRP_IN_VAL_IN_MIC_POS_SEC_LNK;
      }
      else if (rxPara->CurrentEvent == MRP_IN_EVENT_T_POLL_TRF)
      {
        if (rxPara->RxPortRole == MRP_IN_PACKET_VAL_PORT_ROLE_IN)
          inPeerMICPosition = MRP_IN_RED_EVT_PARA_DONT_CARE;
        else
          inPeerMICPosition = MRP_IN_VAL_IN_MIC_POS_PRM_LNK;
      }
      else
        return;

      if (inPeerMICPosition == MRP_IN_RED_EVT_PARA_DONT_CARE)
        mrp_in_write_in_peer_oper_role(pDBin, EDD_SUPPORTED_MRP_INTERCONN_ROLE_CAP_MANAGER);
      else
        mrp_in_write_in_peer_oper_role(pDBin, EDD_SUPPORTED_MRP_INTERCONN_ROLE_CAP_CLIENT);

      pDBinInstEvent = &pDBusr->pInstData[pDBin->pInstance[MRP_INSTANCE_INDEX]].pInInstEventData[pDBin->pInstance[MRP_IN_INSTANCE_INDEX]];
      if (inPeerMICPosition != pDBinInstEvent->InEventIndCurrent.inStateInd.inPeerMICPosition)
      {
        MRP_PROGRAM_TRACE_03(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_in_write_in_in_peer_mic_position[inInst=%d]: inPeerMICPosition to change=0x%x, current inPeerMICPosition=0x%x",
          pDBin->pInstance[MRP_IN_INSTANCE_INDEX], inPeerMICPosition, pDBinInstEvent->InEventIndCurrent.inStateInd.inPeerMICPosition);

        pDBinInstEvent->InEventIndCurrent.inStateInd.inPeerMICPosition = inPeerMICPosition;
      }
    }
  }
  else
  {
    MRP_PROGRAM_TRACE_01 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_in_write_in_in_peer_mic_position[inInst=%d]: pChPrmUsr must not be NULL", pDBin->pInstance[MRP_INSTANCE_INDEX]);
  }
}

/*===========================================================================
* FUNCTION : mrp_in_write_current_in_open_diag
*----------------------------------------------------------------------------
* PURPOSE  : write the current MRP interconnection open event state to the database
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin       = pointer to one instance of mrp interconnection database
*            inOpenLink   = link of the MRP interconnection (PRM or SEC) for which 
*                           a link open diag is to be raised or cleared (MD or ERROR)
*            setFlag      = LSA_TRUE := set inDiag, LSA_FALSE := reset inDiag
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_write_current_in_open_diag (const MRP_IN_DB_TYPE *pDBin, LSA_UINT16 inOpenLink, LSA_UINT8 setFlag)
{
  LSA_UINT16 mrpInMaxInstance;
  MRP_CHANNEL_TYPE_USER pChPrmUsr;
  MRP_USR_DB_TYPE *pDBusr;
  MRP_IN_INST_EVENT_DATA *pDBinInstEvent;
  const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys = pDBin->pChSys;
  const LSA_UINT16 *pInstance = pDBin->pInstance;
  MRP_LINK_ADD_INFO addLinkInfo = pDBin->rxPara.LinkAddInfo;

  pChPrmUsr = mrp_usr_get_prm_usr_channel (pChSys->detailptr);

  if (!LSA_HOST_PTR_ARE_EQUAL (pChPrmUsr, LSA_NULL))
  {
    /* PRM is only possible, if a PRM user channel was established before -> binding of inEvent data to this channel ensures that no event data is lost */
    pDBusr = (MRP_USR_DB_TYPE *)pChPrmUsr->pChUsrDB->pUsr;

    mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pChSys);

    if (mrpInMaxInstance)
    {
      pDBinInstEvent = &pDBusr->pInstData[pInstance[MRP_INSTANCE_INDEX]].pInInstEventData[pInstance[MRP_IN_INSTANCE_INDEX]];

      LSA_UNUSED_ARG(pDBinInstEvent);   /* hhr: to please lint in case traces are disabled. */

      MRP_PROGRAM_TRACE_04 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_in_write_current_in_open_diag[inInst=%d]: inOpenLink to change=0x%x, setFlag=%d, current inDiag=0x%08x",
                            pInstance[MRP_IN_INSTANCE_INDEX], inOpenLink, setFlag, pDBinInstEvent->InEventIndCurrent.inStateInd.inDiag);

      if (setFlag)
      {
        if (addLinkInfo.AddCheckEnabled == EDD_LINK_ADD_CHECK_ENABLED && addLinkInfo.ChangeReason != EDD_LINK_CHANGE_REASON_NONE) /* MRP interconnection link was changed by CFM */
        {
          if (addLinkInfo.ChangeReason == EDD_LINK_CHANGE_REASON_RDI)
          {
            MRP_PROGRAM_TRACE_04 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "mrp_in_write_current_in_open_diag[inInst=%d]: inOpenLink to change=0x%x(0=PRM, 1=SEC), CFM=ON, reason=RDI, setFlag=%d, current inDiag=0x%08x",
                                  pInstance[MRP_IN_INSTANCE_INDEX], inOpenLink, setFlag, pDBinInstEvent->InEventIndCurrent.inStateInd.inDiag);

            if (inOpenLink == MRP_IN_RED_LINK_IN_OPEN_PRM)
            {
              mrp_in_write_current_event_diag (pDBin, MRP_IN_RED_DIAG_IN_OPEN_PRM_ERR, setFlag);
            }
            else
            {
              mrp_in_write_current_event_diag (pDBin, MRP_IN_RED_DIAG_IN_OPEN_SEC_ERR, setFlag);
            }
          }
          else /* any other change reason */
          {
            MRP_PROGRAM_TRACE_05 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "mrp_in_write_current_in_open_diag[inInst=%d]: inOpenLink to change=0x%x(0=PRM, 1=SEC), CFM=ON, reason=%d, setFlag=%d, current inDiag=0x%08x",
                                  pInstance[MRP_IN_INSTANCE_INDEX], inOpenLink, addLinkInfo.ChangeReason, setFlag, pDBinInstEvent->InEventIndCurrent.inStateInd.inDiag);

            if (inOpenLink == MRP_IN_RED_LINK_IN_OPEN_PRM)
            {
              mrp_in_write_current_event_diag (pDBin, MRP_IN_RED_DIAG_IN_OPEN_PRM_ERR, LSA_FALSE);
              mrp_in_write_current_event_diag (pDBin, MRP_IN_RED_DIAG_IN_OPEN_PRM_MD, LSA_TRUE);

            }
            else
            {
              mrp_in_write_current_event_diag (pDBin, MRP_IN_RED_DIAG_IN_OPEN_SEC_ERR, LSA_FALSE);
              mrp_in_write_current_event_diag (pDBin, MRP_IN_RED_DIAG_IN_OPEN_SEC_MD, LSA_TRUE);
            }
          }
        }
        else
        {
          if (inOpenLink == MRP_IN_RED_LINK_IN_OPEN_PRM)
          {
            mrp_in_write_current_event_diag (pDBin, MRP_IN_RED_DIAG_IN_OPEN_PRM_ERR, LSA_FALSE);
            mrp_in_write_current_event_diag (pDBin, MRP_IN_RED_DIAG_IN_OPEN_PRM_MD, LSA_TRUE);
          }
          else
          {
            mrp_in_write_current_event_diag (pDBin, MRP_IN_RED_DIAG_IN_OPEN_SEC_ERR, LSA_FALSE);
            mrp_in_write_current_event_diag (pDBin, MRP_IN_RED_DIAG_IN_OPEN_SEC_MD, LSA_TRUE);
          }
        }
      }
      else /* setFlag = LSA_FALSE */
      {
        if (inOpenLink == MRP_IN_RED_LINK_IN_OPEN_PRM)
        {
          mrp_in_write_current_event_diag (pDBin, MRP_IN_RED_DIAG_IN_OPEN_PRM_ERR, LSA_FALSE);
          mrp_in_write_current_event_diag (pDBin, MRP_IN_RED_DIAG_IN_OPEN_PRM_MD, LSA_FALSE);
        }
        else
        {
          mrp_in_write_current_event_diag (pDBin, MRP_IN_RED_DIAG_IN_OPEN_SEC_ERR, LSA_FALSE);
          mrp_in_write_current_event_diag (pDBin, MRP_IN_RED_DIAG_IN_OPEN_SEC_MD, LSA_FALSE);
        }
      }
    }
  }
  else
  {
    MRP_PROGRAM_TRACE_01 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_in_write_current_in_open_diag[inInst=%d]: pChPrmUsr must not be NULL", pInstance[MRP_INSTANCE_INDEX]);
  }
}

/*===========================================================================
* FUNCTION : mrp_in_write_current_in_partner_role_mismatch
*----------------------------------------------------------------------------
* PURPOSE  : triggers set/reset of partner role mismatch
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pDBin = General Interconnection DB
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_write_current_in_partner_role_mismatch(const MRP_IN_DB_TYPE* pDBin)
{
  LSA_UINT8 setFlag;

  /* If MIM, this indication is not raised */
  if (pDBin->DBmim.runState == LSA_TRUE)
    setFlag = LSA_FALSE;

  /* If Primary MIC, partner is primary MIC aswell */
  else if (pDBin->DBmic.MRPIC_MICPosition == MRP_IN_VAL_IN_MIC_POS_PRM_LNK)
  {
    setFlag = (pDBin->rxPara.RxLnkRole & MRP_IN_PACKET_VAL_LNK_ROLE_BIT0_SEC);
    if (!setFlag && pDBin->rxPara.CurrentEvent == MRP_IN_EVENT_T_POLL_TRF && pDBin->rxPara.RxPortRole == MRP_IN_PACKET_VAL_PORT_ROLE_IN)
      setFlag = LSA_TRUE;
  }
  /* If Secondary Coupled MIC, partner should be MIM */
  else
  {
    if (pDBin->rxPara.CurrentEvent == MRP_IN_EVENT_T_LNK_CHG_TRF)
      setFlag = LSA_TRUE;

    else if (pDBin->rxPara.CurrentEvent == MRP_IN_EVENT_T_POLL_TRF)
    {
      if (pDBin->rxPara.RxPortRole == MRP_IN_PACKET_VAL_PORT_ROLE_IN)
        setFlag = LSA_FALSE;
      else
        return;
    }

    else if (pDBin->rxPara.CurrentEvent == MRP_IN_EVENT_T_TPLG_CHG_TRF)
      return;

    else
      return;
  }

  if (setFlag == LSA_TRUE)
  {
    MRP_PROGRAM_TRACE_00 (pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "Setting Indication MRP_IN_RED_EVT_PARTNER_ROLE_MISMATCH");
  }
  mrp_in_write_current_event_diag(pDBin, MRP_IN_RED_EVT_PARTNER_ROLE_MISMATCH, setFlag);
}

/*===========================================================================
* FUNCTION : mrp_in_write_current_in_id_mismatch
*----------------------------------------------------------------------------
* PURPOSE  : triggers set/reset of InID mismatch
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pDBin = General Interconnection DB
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_write_current_in_id_mismatch(const MRP_IN_DB_TYPE* pDBin)
{
  LSA_UINT8 setFlag = LSA_FALSE;

  if (pDBin->InID != pDBin->rxPara.RxInId)
    setFlag = LSA_TRUE;

  mrp_in_write_current_event_diag(pDBin, MRP_IN_RED_EVT_INID_MISMATCH, setFlag);
}

/*===========================================================================
* FUNCTION : mrp_in_write_current_iport_mismatch
*----------------------------------------------------------------------------
* PURPOSE  : triggers set/reset of Interconnection Port mismatch
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of mrp interconnection database
*            setFlag      = LSA_TRUE := set inDiag, LSA_FALSE := reset inDiag
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_write_current_iport_mismatch(const MRP_IN_DB_TYPE *pDBin, LSA_UINT8 setFlag)
{
  mrp_in_write_current_event_diag(pDBin, MRP_IN_RED_EVT_IPORT_MISMATCH, setFlag);
}

/*===========================================================================
* FUNCTION : mrp_in_write_current_cfm_disabled
*----------------------------------------------------------------------------
* PURPOSE  : triggers set/reset of CFM disabled
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of mrp interconnection database
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_write_current_cfm_disabled(const MRP_IN_DB_TYPE *pDBin)
{
  LSA_UINT8 setFlag = !(pDBin->LinkAddInfo).AddCheckEnabled;

  mrp_in_write_current_event_diag(pDBin, MRP_IN_RED_EVT_CFM_DISABLED, setFlag);
}

/*===========================================================================
* FUNCTION : mrp_in_write_current_event_diag
*----------------------------------------------------------------------------
* PURPOSE  : write the current MRP interconnection event state to the database
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of mrp interconnection database
*            inDiag       = MRP interconnection event to be set/reset
*            setFlag      = LSA_TRUE := set inDiag, LSA_FALSE := reset inDiag
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_write_current_event_diag (const MRP_IN_DB_TYPE *pDBin, LSA_UINT32 inDiag, LSA_UINT8 setFlag)
{
  LSA_UINT16 mrpInMaxInstance;
  MRP_CHANNEL_TYPE_USER pChPrmUsr;
  MRP_USR_DB_TYPE *pDBusr;
  MRP_IN_INST_EVENT_DATA *pDBinInstEvent;
  LSA_UINT8 change = LSA_TRUE;

  const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys = pDBin->pChSys;
  const LSA_UINT16 *pInstance = pDBin->pInstance;

  pChPrmUsr = mrp_usr_get_prm_usr_channel (pChSys->detailptr);

  if (!LSA_HOST_PTR_ARE_EQUAL (pChPrmUsr, LSA_NULL))
  {
    /* PRM is only possible, if a PRM user channel was established before -> binding of inEvent data to this channel ensures that no event data is lost */
    pDBusr = (MRP_USR_DB_TYPE *)pChPrmUsr->pChUsrDB->pUsr;

    mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pChSys);

    if (mrpInMaxInstance)
    {
      pDBinInstEvent = &pDBusr->pInstData[pInstance[MRP_INSTANCE_INDEX]].pInInstEventData[pInstance[MRP_IN_INSTANCE_INDEX]];

      MRP_PROGRAM_TRACE_04 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_in_write_current_event_diag[inInst=%d]: inDiag to change=0x%x, setFlag=%d, current inDiag=0x%08x",
                            pInstance[MRP_IN_INSTANCE_INDEX], inDiag, setFlag, pDBinInstEvent->InEventIndCurrent.inStateInd.inDiag);

      if (setFlag)
      {
        if ((pDBinInstEvent->InEventIndCurrent.inStateInd.inDiag & inDiag) == inDiag) /* diag is already set */
        {
          change = LSA_FALSE;
        }
        /* MD diag must not supersede ERR diag */
        if ( ((inDiag & MRP_IN_RED_DIAG_IN_OPEN_PRM_MD) && (pDBinInstEvent->InEventIndCurrent.inStateInd.inDiag & MRP_IN_RED_DIAG_IN_OPEN_PRM_ERR)) ||
             ((inDiag & MRP_IN_RED_DIAG_IN_OPEN_SEC_MD) && (pDBinInstEvent->InEventIndCurrent.inStateInd.inDiag & MRP_IN_RED_DIAG_IN_OPEN_SEC_ERR))
           )
        {
          MRP_PROGRAM_TRACE_03 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "mrp_in_write_current_event_diag[inInst=%d]: inDiag=0x%x(MD) not changed because ERR diag already set, current inDiag=0x%08x",
                                pInstance[MRP_IN_INSTANCE_INDEX], inDiag, pDBinInstEvent->InEventIndCurrent.inStateInd.inDiag);
          change = LSA_FALSE;
        }
        else
        {
          /* ERROR diag supersedes MD diag */
          if (inDiag & MRP_IN_RED_DIAG_IN_OPEN_SEC_ERR)
          {
            pDBinInstEvent->InEventIndCurrent.inStateInd.inDiag &= ~MRP_IN_RED_DIAG_IN_OPEN_SEC_MD;
          }

          /* ERROR diag supersedes MD diag */
          if (inDiag & MRP_IN_RED_DIAG_IN_OPEN_PRM_ERR)
          {
            pDBinInstEvent->InEventIndCurrent.inStateInd.inDiag &= ~MRP_IN_RED_DIAG_IN_OPEN_PRM_MD;
          }

          pDBinInstEvent->InEventIndCurrent.inStateInd.inDiag |= inDiag;
        }
      }
      else
      {
        if ((pDBinInstEvent->InEventIndCurrent.inStateInd.inDiag & inDiag) == 0)
          change = LSA_FALSE;

        pDBinInstEvent->InEventIndCurrent.inStateInd.inDiag &= ~inDiag;
      }

      if (change)
      {
        MRP_PROGRAM_TRACE_03 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_write_current_event_diag[inst=%d/inInst=%d]: new current inDiag=0x%08x",
                              pInstance[MRP_INSTANCE_INDEX], pInstance[MRP_IN_INSTANCE_INDEX], pDBinInstEvent->InEventIndCurrent.inStateInd.inDiag);
      }
    }
    else
    {
      MRP_PROGRAM_TRACE_02 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_in_write_current_event_diag[inst=%d/inInst=%d]: pChPrmUsr must not be NULL", pInstance[MRP_INSTANCE_INDEX], pInstance[MRP_IN_INSTANCE_INDEX]);
    }
  }
}


/*===========================================================================
* FUNCTION : mrp_in_write_current_event_peer_macaddr
*----------------------------------------------------------------------------
* PURPOSE  : write the current MRP interconnection peer MAC address
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin = pointer to one instance of mrp interconnection database
*            pPeerMacAddr = pointer to the MRP interconnection partner MAC address
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_write_current_event_peer_macaddr (const MRP_IN_DB_TYPE *pDBin, LSA_UINT8 *pPeerMacAddr)
{
  LSA_UINT16 mrpInMaxInstance;
  MRP_CHANNEL_TYPE_USER pChPrmUsr;
  MRP_USR_DB_TYPE *pDBusr;
  MRP_IN_INST_EVENT_DATA *pDBinInstEvent;

  const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys = pDBin->pChSys;
  const LSA_UINT16 *pInstance = pDBin->pInstance;

  pChPrmUsr = mrp_usr_get_prm_usr_channel (pChSys->detailptr);

  if (!LSA_HOST_PTR_ARE_EQUAL (pChPrmUsr, LSA_NULL))
  {
    /* PRM is only possible, if a PRM user channel was established before -> binding of inEvent data to this channel ensures that no event data is lost */
    pDBusr = (MRP_USR_DB_TYPE *)pChPrmUsr->pChUsrDB->pUsr;

    mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pChSys);

    if (mrpInMaxInstance)
    {
      pDBinInstEvent = &pDBusr->pInstData[pInstance[MRP_INSTANCE_INDEX]].pInInstEventData[pInstance[MRP_IN_INSTANCE_INDEX]];

      MRP_MEMCPY (pDBinInstEvent->InEventIndCurrent.inStateInd.inPeerMacAddress, pPeerMacAddr, MRP_PACKET_VAL_MAC_ADDR_LEN);

      MRP_PROGRAM_TRACE_07 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_in_write_current_event_peer_macaddr[inInst=%d]: peerMacAddr=%x:%x:%x:%x:%x:%x", pInstance[MRP_IN_INSTANCE_INDEX], pPeerMacAddr[0], pPeerMacAddr[1], pPeerMacAddr[2], pPeerMacAddr[3], pPeerMacAddr[4], pPeerMacAddr[5]);
    }
  }
}

/*===========================================================================
* FUNCTION : mrp_in_write_current_event_prm_info
*----------------------------------------------------------------------------
* PURPOSE  : write the current MRP interconnection event related PRM info
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : *pDBin       = pointer to one instance of mrp interconnection database
*            inOperRole   = MRP interconnection operational role
*            MICPosition  = MIC position
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_write_current_event_prm_info (const MRP_IN_DB_TYPE *pDBin, MRP_IN_OPER_ROLE_ENUM_T inOperRole, LSA_UINT16 MICPosition)
{
  LSA_UINT16 mrpInMaxInstance;
  MRP_CHANNEL_TYPE_USER pChPrmUsr;
  MRP_USR_DB_TYPE *pDBusr;
  MRP_IN_INST_EVENT_DATA *pDBinInstEvent;
  const LSA_UINT16 *pInstance = pDBin->pInstance;
  LSA_UINT16 inId = pDBin->InID;
  LSA_UINT16 inPortId = pDBin->IPort;

  const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys = pDBin->pChSys;

  pChPrmUsr = mrp_usr_get_prm_usr_channel (pChSys->detailptr);

  if (!LSA_HOST_PTR_ARE_EQUAL (pChPrmUsr, LSA_NULL))
  {
    /* PRM is only possible, if a PRM user channel was established before -> binding of inEvent data to this channel ensures that no event data is lost */
    pDBusr = (MRP_USR_DB_TYPE *)pChPrmUsr->pChUsrDB->pUsr;

    mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pChSys);

    if (mrpInMaxInstance)
    {
      pDBinInstEvent = &pDBusr->pInstData[pInstance[MRP_INSTANCE_INDEX]].pInInstEventData[pInstance[MRP_IN_INSTANCE_INDEX]];

      pDBinInstEvent->InEventIndCurrent.inStateInd.inId          = inId;
      pDBinInstEvent->InEventIndCurrent.inStateInd.inPortId      = inPortId;
      pDBinInstEvent->InEventIndCurrent.inStateInd.inOperRole    = inOperRole;
      pDBinInstEvent->InEventIndCurrent.inStateInd.inMICPosition = MICPosition;

      MRP_PROGRAM_TRACE_06 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_in_write_current_event_prm_info[inst/inInst=%d/%d]: inId=%d, inPortId=%d, inOperRole=%d inMICPosition=%d",
                            pInstance[MRP_INSTANCE_INDEX], pInstance[MRP_IN_INSTANCE_INDEX], inId, inPortId, inOperRole, MICPosition);
    }
  }
}


/*===========================================================================
* FUNCTION : mrp_in_get_current_event_state
*----------------------------------------------------------------------------
* PURPOSE  : get current MRP interconnection event state
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChPrivUsr           = channel reference
*            pInstance = affected instance in array format (contains
*                         MRP instance and MRP interconnection instance)
*            pMrpInEventCurrent  = pointer to struct of type MRP_OPC_RED_IN_EVENT_INDICATION_TYPE
* OUTPUTS  : *pMrpInEventCurrent = current MRP interconnection event data
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_VOID mrp_in_get_current_event_state (const MRP_CONST_CHANNEL_TYPE_USER pChPrivUsr, const LSA_UINT16 *pInstance, MRP_OPC_RED_IN_EVENT_INDICATION_TYPE *pMrpInEventCurrent)
{
  MRP_CHANNEL_TYPE_USER pChPrmUsr;
  MRP_USR_DB_TYPE *pDBusr;
  MRP_IN_INST_EVENT_DATA *pDBinInstEvent;

  pChPrmUsr = mrp_usr_get_prm_usr_channel (pChPrivUsr->detailptr);

  if (!LSA_HOST_PTR_ARE_EQUAL (pChPrmUsr, LSA_NULL))
  {
    /* PRM is only possible, if a PRM user channel was established before -> binding of inEvent data to this channel ensures that no event data is lost */
    pDBusr = (MRP_USR_DB_TYPE *)pChPrmUsr->pChUsrDB->pUsr;
    pDBinInstEvent = &pDBusr->pInstData[pInstance[MRP_INSTANCE_INDEX]].pInInstEventData[pInstance[MRP_IN_INSTANCE_INDEX]];

    *pMrpInEventCurrent = pDBinInstEvent->InEventIndCurrent.inStateInd;
  }
  else
    MRP_PROGRAM_TRACE_02 (pChPrivUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_ERROR, "mrp_in_get_current_event_state[inst/inInst=%d/%d]: pChPrmUsr must not be NULL", pInstance[MRP_INSTANCE_INDEX], pInstance[MRP_IN_INSTANCE_INDEX]);
}

/*===========================================================================
* FUNCTION : mrp_in_check_event_change
*----------------------------------------------------------------------------
* PURPOSE  : compares last indicated MRP interconnection event data with current 
*            MRP interconnection event data
*----------------------------------------------------------------------------
* RETURNS  : LSA_TRUE or LSA_FALSE (difference in ringState)
*----------------------------------------------------------------------------
* INPUTS   : pChPrivUsr       = channel reference
*            pInstance = affected instance in array format (contains
*                         MRP instance and MRP interconnection instance)
*            pMrpInEventCurrent = pointer to struct with current redundancy state variables
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_UINT8 mrp_in_check_event_change (const MRP_CONST_CHANNEL_TYPE_USER pChPrivUsr, const LSA_UINT16 *pInstance, MRP_OPC_RED_IN_EVENT_INDICATION_TYPE *pMrpInEventCurrent)
{
  LSA_UINT8 inEventDiff;
  MRP_OPC_RED_IN_EVENT_INDICATION_TYPE *pInEventLastSignaled;
  MRP_USR_DB_TYPE *pDBusr = (MRP_USR_DB_TYPE *)pChPrivUsr->pChUsrDB->pUsr;

  pInEventLastSignaled = &pDBusr->pInstData[pInstance[MRP_INSTANCE_INDEX]].pInInstEventData[pInstance[MRP_IN_INSTANCE_INDEX]].InEventIndLastSignaled.inStateInd;

  inEventDiff = LSA_FALSE;

  if (pInEventLastSignaled->inId              != pMrpInEventCurrent->inId              ||
      pInEventLastSignaled->inPortId          != pMrpInEventCurrent->inPortId          ||
      pInEventLastSignaled->inOperRole        != pMrpInEventCurrent->inOperRole        ||
      pInEventLastSignaled->inDiag            != pMrpInEventCurrent->inDiag            ||
      pInEventLastSignaled->inMICPosition     != pMrpInEventCurrent->inMICPosition     ||
      pInEventLastSignaled->inRxPort          != pMrpInEventCurrent->inRxPort          || 
      pInEventLastSignaled->inRxInID          != pMrpInEventCurrent->inRxInID          ||
      pInEventLastSignaled->inPeerOperRole    != pMrpInEventCurrent->inPeerOperRole    ||
      pInEventLastSignaled->inPeerMICPosition != pMrpInEventCurrent->inPeerMICPosition ||
    
      MRP_MEMCMP (pInEventLastSignaled->inPeerMacAddress, pMrpInEventCurrent->inPeerMacAddress, MRP_PACKET_VAL_MAC_ADDR_LEN)
     )
  {
    inEventDiff = LSA_TRUE;
  }

  return inEventDiff;
}


/*===========================================================================
* FUNCTION : mrp_in_red_event_ind_done
*----------------------------------------------------------------------------
* PURPOSE  : indicate state changes of an MRP interconnection instance
*----------------------------------------------------------------------------
* RETURNS  : LSA_TRUE  = RQB in queue
*            LSA_FALSE = no RQB in queue
*----------------------------------------------------------------------------
* INPUTS   : pChPrivUsr = channel reference for private user channel (USER2)
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_UINT8 mrp_in_red_event_ind_done (const MRP_CONST_CHANNEL_TYPE_USER pChPrivUsr, const LSA_UINT16 *pInstance)
{
  MRP_UPPER_RQB_PTR_TYPE pRQB;
  MRP_UPPER_ARGS_PTR_TYPE pArgs;
  LSA_UINT8 inEventDiff;
  LSA_UINT16 resp = MRP_RSP_OK;
  MRP_USR_DB_TYPE *pDBusr = (MRP_USR_DB_TYPE *)pChPrivUsr->pChUsrDB->pUsr;
  MRP_IN_INST_EVENT_DATA *pDBinInstEvent;
  MRP_OPC_RED_IN_EVENT_INDICATION_TYPE mrpInEventCurrent;

  pDBinInstEvent = &pDBusr->pInstData[pInstance[MRP_INSTANCE_INDEX]].pInInstEventData[pInstance[MRP_IN_INSTANCE_INDEX]];
  MRP_MEMSET (&mrpInEventCurrent, 0, sizeof (MRP_OPC_RED_IN_EVENT_INDICATION_TYPE));

  /* get infos */
  /* get current MRP interconnection events */
  mrp_in_get_current_event_state (pChPrivUsr, pInstance, &mrpInEventCurrent);
  inEventDiff = mrp_in_check_event_change (pChPrivUsr, pInstance, &mrpInEventCurrent);

  if (pChPrivUsr->state == MRP_CH_STATE_CLOSING || inEventDiff)
  {
    MRP_RQB_POP_FROM_TOP (pDBinInstEvent->pInEventIndRqbB, pDBinInstEvent->pInEventIndRqbT, pRQB);

    if (LSA_HOST_PTR_ARE_EQUAL (pRQB, LSA_NULL))
    {
      MRP_UPPER_TRACE_02 (pChPrivUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "<< MRP_OPC_RED_IN_EVENT_INDICATION done[inInst=%d]: out of resource, myHandle =%d", pInstance[MRP_IN_INSTANCE_INDEX], pChPrivUsr->myHandle);
      pDBinInstEvent->InEventIndOutOfResource = MRP_IND_RESOURCE_MISSING_ON;

      return LSA_FALSE;
    }

    pArgs = MRP_RQB_GET_ARGS_PTR (pRQB);


    pArgs->state.state_ind.inStateInd = pDBinInstEvent->InEventIndLastSignaled.inStateInd = pDBinInstEvent->InEventIndCurrent.inStateInd = mrpInEventCurrent;

    if (pChPrivUsr->state == MRP_CH_STATE_CLOSING)
    {
      resp = MRP_RSP_OK_CANCEL;
      MRP_RQB_SET_RESPONSE (pRQB, resp);
    }
    MRP_UPPER_TRACE_04 (pChPrivUsr->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "<< MRP_OPC_RED_IN_EVENT_INDICATION done[inInst=%d]: cbf called, myHandle =%d, resp=0x%x, inDiag=0x%x",
                        pInstance[MRP_IN_INSTANCE_INDEX], pChPrivUsr->myHandle, resp, pArgs->state.state_ind.inStateInd.inDiag);
    mrp_usr_call_upper_cbf (pChPrivUsr, pRQB);
  }
  return LSA_TRUE;
}

/*===========================================================================
* FUNCTION : mrp_in_red_event_ind_provide_rqb
*----------------------------------------------------------------------------
* PURPOSE  : set indication rqbs for signaling MRP interconnection events
*----------------------------------------------------------------------------
* RETURNS  : MRP_RET_OK/MRP_RET_ERR
*----------------------------------------------------------------------------
* INPUTS   : pChPrivUsr = channel reference to private user channel (USER2)
*            pRQB       = upper rqb reference
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS : queues indication requests
*==========================================================================*/
LSA_UINT16 mrp_in_red_event_ind_provide_rqb (const MRP_CHANNEL_TYPE_USER pChPrivUsr, MRP_UPPER_RQB_PTR_TYPE pRQB)
{
  LSA_UINT16 mrpInMaxInstance;
  MRP_UPPER_ARGS_PTR_TYPE pArgs;
  MRP_USR_DB_TYPE *pDBusr = (MRP_USR_DB_TYPE *)pChPrivUsr->pChUsrDB->pUsr;
  MRP_IN_INST_EVENT_DATA *pDBinInstEvent;
  LSA_UINT16  pInstance[2];
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChPrivUsr->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  pArgs = MRP_RQB_GET_ARGS_PTR (pRQB);

  MRP_ASSERT (pArgs->state.state_ind.inStateInd.mrpInstance > 0 && pArgs->state.state_ind.inStateInd.mrpInstance <= EDD_CFG_MAX_MRP_INSTANCE_CNT);
  pInstance[MRP_INSTANCE_INDEX] = pArgs->state.state_ind.inStateInd.mrpInstance - 1;
  
  MRP_ASSERT (pArgs->state.state_ind.inStateInd.mrpInInstance > 0 && pArgs->state.state_ind.inStateInd.mrpInInstance <= EDD_CFG_MAX_MRP_IN_INSTANCE_CNT);
  pInstance[MRP_IN_INSTANCE_INDEX] = pArgs->state.state_ind.inStateInd.mrpInInstance - 1;

  mrpInMaxInstance = mrp_in_sys_get_num_of_in_instances (pChPrivUsr);

  MRP_ASSERT (pArgs->state.state_ind.inStateInd.mrpInInstance > 0 && pArgs->state.state_ind.inStateInd.mrpInInstance <= mrpInMaxInstance);

  if (mrpInMaxInstance)
  {
    pDBinInstEvent = &pDBusr->pInstData[pInstance[MRP_INSTANCE_INDEX]].pInInstEventData[pInstance[MRP_IN_INSTANCE_INDEX]];

    if (pChPrivUsr->state != MRP_CH_STATE_CLOSING)
    {
      if (pInstance[MRP_INSTANCE_INDEX] < mrp_sys_get_num_of_instances (pChPrivUsr))
      {
        if (pInstance[MRP_IN_INSTANCE_INDEX] < mrpInMaxInstance)
        {
          MRP_UPPER_TRACE_06 (traceIdx, LSA_TRACE_LEVEL_NOTE, ">> MRP_OPC_RED_IN_EVENT_INDICATION[inst=%d/inInst=%d]: rqb provided, pChPrivUsr=0x%x, state=%d, myHandle=%d, outOfRes=%d, resp=MRP_RSP_OK",
                              pInstance[MRP_INSTANCE_INDEX], pInstance[MRP_IN_INSTANCE_INDEX], pChPrivUsr, pChPrivUsr->state, pChPrivUsr->myHandle, pDBinInstEvent->InEventIndOutOfResource);

          MRP_RQB_SET_RESPONSE (pRQB, MRP_RSP_OK);

          MRP_RQB_PUSH_TO_BOTTOM (pDBinInstEvent->pInEventIndRqbB, pDBinInstEvent->pInEventIndRqbT, pRQB);
          pChPrivUsr->pRQB = LSA_NULL;

          if (pDBinInstEvent->InEventIndOutOfResource == MRP_IND_RESOURCE_MISSING_ON)
          {
            pDBinInstEvent->InEventIndOutOfResource = MRP_IND_RESOURCE_MISSING_OFF;

            (void) (mrp_in_red_event_ind_done (pChPrivUsr, pInstance));
          }
          return MRP_RET_OK;
        }
        else
        {
          MRP_UPPER_TRACE_05 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> MRP_OPC_RED_IN_EVENT_INDICATION[inst=%d/inInst=%d]: rqb provided for wrong inInst, pChPrivUsr=0x%x, state=%d, myHandle=%d, resp=MRP_RSP_ERR_PRM_DATA",
                              pInstance[MRP_INSTANCE_INDEX], pInstance[MRP_IN_INSTANCE_INDEX], pChPrivUsr, pChPrivUsr->state, pChPrivUsr->myHandle);

          MRP_RQB_SET_RESPONSE (pRQB, MRP_RSP_ERR_PRM_DATA);

          return MRP_RET_ERR;
        }
      }
      else
      {
        MRP_UPPER_TRACE_05 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> MRP_OPC_RED_IN_EVENT_INDICATION[inst=%d/inInst=%d]: rqb provided for wrong MRP inst, pChPrivUsr=0x%x, state=%d, myHandle=%d, resp=MRP_RSP_ERR_PRM_DATA",
                            pInstance[MRP_INSTANCE_INDEX], pInstance[MRP_IN_INSTANCE_INDEX], pChPrivUsr, pChPrivUsr->state, pChPrivUsr->myHandle);

        MRP_RQB_SET_RESPONSE (pRQB, MRP_RSP_ERR_PRM_DATA);

        return MRP_RET_ERR;
      }
    }
    else
    {
      MRP_UPPER_TRACE_05 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> MRP_OPC_RED_IN_EVENT_INDICATION[inst=%d/inInst=%d]: rqb provided, pChPrivUsr=0x%x, state=%d, myHandle=%d, resp=MRP_RSP_ERR_SEQUENCE",
                          pInstance[MRP_INSTANCE_INDEX], pInstance[MRP_IN_INSTANCE_INDEX], pChPrivUsr, pChPrivUsr->state, pChPrivUsr->myHandle);

      MRP_RQB_SET_RESPONSE (pRQB, MRP_RSP_ERR_SEQUENCE);

      return MRP_RET_ERR;
    }
  }
  else
  {
    MRP_UPPER_TRACE_05 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> MRP_OPC_RED_IN_EVENT_INDICATION[inst=%d/inInst=%d]: rqb provided but mrpInMaxInst=0, pChPrivUsr=0x%x, state=%d, myHandle=%d, resp=MRP_RSP_ERR_PRM_DATA",
                        pInstance[MRP_INSTANCE_INDEX], pInstance[MRP_IN_INSTANCE_INDEX], pChPrivUsr, pChPrivUsr->state, pChPrivUsr->myHandle);

    MRP_RQB_SET_RESPONSE (pRQB, MRP_RSP_ERR_PRM_DATA);

    return MRP_RET_ERR;
  }
}



/*===========================================================================
* FUNCTION : mrp_in_red_in_tc_request
*----------------------------------------------------------------------------
* PURPOSE  : request sending of MRP_InTC frames for MRP
*            ring reconfiguration purpose - triggered by means of an
*            (external) application (e.g. RSTP+)
*----------------------------------------------------------------------------
* RETURNS  : MRP_RET_OK/MRP_RET_ERR
*----------------------------------------------------------------------------
* INPUTS   : pChPrivUsr = channel reference to private user channel (USER2)
*            pRQB       = upper rqb reference
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_UINT16 mrp_in_red_in_tc_request (const MRP_CHANNEL_TYPE_USER pChPrivUsr, MRP_UPPER_RQB_PTR_TYPE pRQB)
{
  LSA_UINT16 instance;
  MRP_UPPER_ARGS_PTR_TYPE pArgs;
  MRP_MODE_DATA_TYPE mrpData;
  MRP_IN_DB_TYPE *pDBin;
  MRP_IN_DB_TYPE *pDBinBase = (MRP_IN_DB_TYPE *)pChPrivUsr->pChSysDB->pMrpIn;
  MRP_IF_DB_TYPE *pDBif = (MRP_IF_DB_TYPE *)pChPrivUsr->pChSysDB->pInterfacePara;
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChPrivUsr->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */
  

  pArgs = MRP_RQB_GET_ARGS_PTR (pRQB);

  MRP_ASSERT (pArgs->ctrl.in_tc_request.mrpInstance > 0 && pArgs->ctrl.in_tc_request.mrpInstance <= EDD_CFG_MAX_MRP_INSTANCE_CNT);
  instance = pArgs->ctrl.in_tc_request.mrpInstance - 1;  
  MRP_ASSERT (instance == 0); /* must be 0, because only single MRP instance allowed here */

  if (pChPrivUsr->state != MRP_CH_STATE_CLOSING)
  {
    if (instance < mrp_sys_get_num_of_instances (pChPrivUsr))
    {
      MRP_FATAL1 ((pDBif->mrpInMaxInstance > 0) && (pDBif->mrpInMaxInstance <= EDD_CFG_MAX_MRP_IN_INSTANCE_CNT));
      /* The RSTP+ coupling feature is assigned to the "last" interconnection instance */
      pDBin = &pDBinBase[pDBif->mrpInMaxInstance - 1];

      MRP_UPPER_TRACE_07 (traceIdx, LSA_TRACE_LEVEL_NOTE, ">> MRP_OPC_RED_IN_TC_REQUEST[inst=%d/inInst=%d] triggered, pChPrivUsr=0x%x, state=%d, myHandle=%d, originator=%d, reason=%d",
                          instance, pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pChPrivUsr, pChPrivUsr->state, pChPrivUsr->myHandle, pArgs->ctrl.in_tc_request.originator, pArgs->ctrl.in_tc_request.reason);

      if (pDBif->mrpInOriginator == MRP_IN_ORIGINATOR_OFF || pDBif->mrpInFwdRulesSupported == MRP_IN_FWD_RULES_SUPPORTED_OFF || pDBif->mrpSupportedRoleInst0 == EDD_MRP_ROLE_NONE)
      {
        MRP_UPPER_TRACE_08 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> MRP_OPC_RED_IN_TC_REQUEST[inst=%d/inInst=%d]: request rejected (inOriginator=%d/inFwdSupport=%d/suppRoleInst0=%d), pChPrivUsr=0x%x, state=%d, myHandle=%d, resp=MRP_RSP_ERR_PARAM",
                            instance, pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pDBif->mrpInOriginator, pDBif->mrpInFwdRulesSupported, pDBif->mrpSupportedRoleInst0, pChPrivUsr, pChPrivUsr->state, pChPrivUsr->myHandle);

        MRP_RQB_SET_RESPONSE (pRQB, MRP_RSP_ERR_PARAM);
        return MRP_RET_ERR;

      }
      else
      {
        mrp_sys_get_mrp_mode (pChPrivUsr, instance, &mrpData);

        if (mrpData.ardMode == MRP_ARD_MODE_HSRC_ON || mrpData.mrpMode == MRP_MODE_OFF)
        {
          MRP_UPPER_TRACE_05 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> MRP_OPC_RED_IN_TC_REQUEST[inst=%d/inInst=%d]: request rejected (operational role), pChPrivUsr=0x%x, state=%d, myHandle=%d, resp=MRP_RSP_ERR_PARAM",
                              instance, pDBin->pInstance[MRP_IN_INSTANCE_INDEX], pChPrivUsr, pChPrivUsr->state, pChPrivUsr->myHandle);

          MRP_RQB_SET_RESPONSE (pRQB, MRP_RSP_ERR_PARAM);
          return MRP_RET_ERR;
        }
        else
        {
          pDBin->InID                         = pArgs->ctrl.in_tc_request.inId;

          pDBin->InTcTxInfo.inId              = pArgs->ctrl.in_tc_request.inId;
          pDBin->InTcTxInfo.inFrameType       = MRP_IN_FRAME_TYPE_IN_TC;
          pDBin->InTcTxInfo.isRstpOriginated  = LSA_TRUE;
          pDBin->InTcTxInfo.rxPort            = pArgs->ctrl.in_tc_request.origPort;

          MRP_MEMCPY (pDBin->InTcTxInfo.rxSA, pDBin->pDBifRef->macAddr, MRP_PACKET_VAL_MAC_ADDR_LEN);

          mrp_in_queue_rstpp_in_tc_req (pDBin);

          MRP_RQB_SET_RESPONSE (pRQB, MRP_RSP_OK);
          mrp_usr_call_upper_cbf_proc_next_req (pChPrivUsr, pRQB);

          return MRP_RET_OK;
        }
      }
    }
    else
    {
      MRP_UPPER_TRACE_04 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> MRP_OPC_RED_IN_TC_REQUEST[inst=%d]: request rejected, pChPrivUsr=0x%x, state=%d, myHandle=%d, resp=MRP_RSP_ERR_PARAM",
                          instance, pChPrivUsr, pChPrivUsr->state, pChPrivUsr->myHandle);

      MRP_RQB_SET_RESPONSE (pRQB, MRP_RSP_ERR_PARAM);
      return MRP_RET_ERR;
    }
  }
  else
  {
    MRP_UPPER_TRACE_04 (traceIdx, LSA_TRACE_LEVEL_ERROR, ">> MRP_OPC_RED_IN_TC_REQUEST[inst=%d]: request rejected, pChPrivUsr=0x%x, state=%d, myHandle=%d, resp=MRP_RSP_ERR_SEQUENCE",
                        instance, pChPrivUsr, pChPrivUsr->state, pChPrivUsr->myHandle);

    MRP_RQB_SET_RESPONSE (pRQB, MRP_RSP_ERR_SEQUENCE);
    return MRP_RET_ERR;
  }
}

/*===========================================================================
* FUNCTION : mrp_in_update
*----------------------------------------------------------------------------
* PURPOSE  : updates MRP interconnection state machine
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : *pDBinEv = pointer to event data
*            event    = current event type
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : this function is called to signal state changes.
*==========================================================================*/
LSA_VOID mrp_in_update (MRP_IN_EVENT_DB_TYPE *pDBinEv, MRP_IN_EVENT_TYPE event)
{
  LSA_UINT16 inInstance;
  MRP_IN_DB_TYPE *pDBin;
  MRP_IN_DB_TYPE *pDBinBase = (MRP_IN_DB_TYPE *)pDBinEv->pChSys->pChSysDB->pMrpIn;
  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pDBinEv->pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */
  

  pDBinEv->CurrentEvent = event;

  MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_in_update enter: event=%d, inInst=%d", event, pDBinEv->evInstance);

  switch (pDBinEv->CurrentEvent)
  {
    case MRP_IN_EVENT_T_MAU_TYPE_CHG:
    {
      pDBin = &pDBinBase[pDBinEv->evInstance];

      pDBin->rxPara      = *pDBinEv;
      pDBin->LinkAddInfo = pDBinEv->LinkAddInfo;

      if (pDBin->rxPara.LinkState == MRP_LINK_STATE_DOWN)
      {
        mrp_in_write_current_event_diag(pDBin, MRP_IN_RED_EVT_PARTNER_ROLE_MISMATCH, LSA_FALSE);
        mrp_in_write_current_event_diag(pDBin, MRP_IN_RED_EVT_INID_MISMATCH, LSA_FALSE);
      }

      /* container state for MRP interconnection called */
      mrp_in_container_state (pDBin);
    }
    break;

    case MRP_IN_EVENT_T_LNK_CHG_DOWN_TIMER:
    case MRP_IN_EVENT_T_LNK_CHG_UP_TIMER:
    case MRP_IN_EVENT_T_LNK_POLL_TIMER:
    case MRP_IN_EVENT_T_IN_TC_TIMER:
    case MRP_IN_EVENT_T_IN_TC_RSTPP_TIMER:
    case MRP_IN_EVENT_T_MRP_CHG_IND:
    {
      pDBin = &pDBinBase[pDBinEv->evInstance];

      pDBin->rxPara = *pDBinEv;

      /* container state for MRP interconnection called */
      mrp_in_container_state (pDBin);
    }
    break;

    case MRP_IN_EVENT_T_TPLG_CHG_TRF:
    case MRP_IN_EVENT_T_LNK_CHG_TRF:
    case MRP_IN_EVENT_T_POLL_TRF:
    {
      inInstance = mrp_in_sys_map_inId_to_inInstance (pDBinEv->pChSys, pDBinEv->RxInId, pDBinEv->RxPort);

      if (inInstance != MRP_IN_INSTANCE_NONE && inInstance != MRP_IN_INSTANCE_FOREIGN_IN_ID) /* event mapped to an MRP interconnection instance */
      {
        pDBin = &pDBinBase[inInstance];

        pDBin->rxPara = *pDBinEv;

        if (pDBin->rxPara.RxPort == pDBin->IPort)
        {
          mrp_in_write_current_event_peer_macaddr (pDBin, pDBin->rxPara.RxPeerPortMacAddr);
          MRP_MEMCPY (pDBin->CoupledDomainUUID, (LSA_VOID_PTR_TYPE)pDBin->rxPara.RxUUID, sizeof (pDBin->CoupledDomainUUID));
          mrp_in_write_current_in_partner_role_mismatch(pDBin); 
          mrp_in_write_current_in_id_mismatch(pDBin);
        }
        else
        {
          mrp_in_write_current_iport_mismatch(pDBin, LSA_TRUE);
        }

        /* container state for MRP interconnection called */
        mrp_in_container_state (pDBin);
      }
      else     /* event not mapped to an MRP interconnection instance */
      {
        MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "mrp_in_update: event=%d, rxPort=%d, rxInId=%d couldn't be mapped to any MRP interconnection inst.",
                              event, pDBinEv->RxPort, pDBinEv->RxInId);
      }
    }
    break;

    default: /* any other event */
    {
      inInstance = mrp_in_sys_map_inId_to_inInstance (pDBinEv->pChSys, pDBinEv->RxInId, pDBinEv->RxPort);

      if (inInstance != MRP_IN_INSTANCE_NONE) /* event mapped to an MRP interconnection instance */
      {
        if (inInstance == MRP_IN_INSTANCE_FOREIGN_IN_ID)
        {
          if (pDBinEv->CurrentEvent == MRP_IN_EVENT_T_TPLG_CHG_WRK)
          {
            /* For foreign InId use the default inInstance */
            pDBin = &pDBinBase[MRP_IN_INSTANCE_DEFAULT];
            pDBin->rxPara = *pDBinEv;

            /* container state for MRP interconnection called */
            mrp_in_container_state (pDBin);
          }
          else
          {
            MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_update: event=%d, rxPort=%d, rxInId=%d couldn't be mapped to any MRP interconnection inst.",
                                  event, pDBinEv->RxPort, pDBinEv->RxInId);
          }
        }
        else
        {
          pDBin = &pDBinBase[inInstance];
          pDBin->rxPara = *pDBinEv;

          /* container state for MRP interconnection called */
          mrp_in_container_state (pDBin);
        }
      }
      else     /* event not mapped to an MRP interconnection instance */
      {
        MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "mrp_in_update: event=%d, rxPort=%d, rxInId=%d couldn't be mapped to any MRP interconnection inst.",
                              event, pDBinEv->RxPort, pDBinEv->RxInId);
      }
    }
    break;
  }
  
  MRP_PROGRAM_TRACE_00(traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_in_update exit");
}


/*===========================================================================
* FUNCTION : mrp_in_timer_cbf_rstpp_inTC_send
*----------------------------------------------------------------------------
* PURPOSE  : next timer interval for sending in topology change frames 
*            originated by RSTP+ has expired
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys    = channel reference
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            uid       = user identification==timer id
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_VOID mrp_in_timer_cbf_rstpp_inTC_send (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, LSA_USER_ID_TYPE uid)
{
  MRP_IN_EVENT_DB_TYPE DBinEv;
  MRP_IN_EVENT_DB_TYPE *pDBinEv = &DBinEv;

  LSA_UNUSED_ARG (uid);

  MRP_MEMSET (pDBinEv, 0, sizeof (MRP_IN_EVENT_DB_TYPE));

  pDBinEv->pChSys = pChSys;
  pDBinEv->evInstance = pInstance[MRP_IN_INSTANCE_INDEX];

  mrp_in_update (pDBinEv, MRP_IN_EVENT_T_IN_TC_RSTPP_TIMER);  
}


/*===========================================================================
* FUNCTION : mrp_in_timer_cbf_inTC_send
*----------------------------------------------------------------------------
* PURPOSE  : next timer interval for sending in topology change frames
*            has expired
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys   = channel reference
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            uid      = user identification==timer id
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_VOID mrp_in_timer_cbf_inTC_send (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, LSA_USER_ID_TYPE uid)
{
  MRP_IN_EVENT_DB_TYPE DBinEv;
  MRP_IN_EVENT_DB_TYPE *pDBinEv = &DBinEv;

  LSA_UNUSED_ARG (uid);

  MRP_MEMSET (pDBinEv, 0, sizeof (MRP_IN_EVENT_DB_TYPE));

  pDBinEv->pChSys = pChSys;
  pDBinEv->evInstance = pInstance[MRP_IN_INSTANCE_INDEX];

  mrp_in_update (pDBinEv, MRP_IN_EVENT_T_IN_TC_TIMER);
}


/*===========================================================================
* FUNCTION : mrp_in_timer_cbf_inPoll_send
*----------------------------------------------------------------------------
* PURPOSE  : next timer interval for sending InPoll frames
*            has expired
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys    = channel reference
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            uid       = user identification==timer id
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_VOID mrp_in_timer_cbf_inPoll_send (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, LSA_USER_ID_TYPE uid)
{
  MRP_IN_EVENT_DB_TYPE DBinEv;
  MRP_IN_EVENT_DB_TYPE *pDBinEv = &DBinEv;

  LSA_UNUSED_ARG (uid);

  MRP_MEMSET (pDBinEv, 0, sizeof (MRP_IN_EVENT_DB_TYPE));

  pDBinEv->pChSys = pChSys;
  pDBinEv->evInstance = pInstance[MRP_IN_INSTANCE_INDEX];

  mrp_in_update (pDBinEv, MRP_IN_EVENT_T_LNK_POLL_TIMER);
}


/*===========================================================================
* FUNCTION : mrp_in_timer_cbf_inLnkChg_down_send
*----------------------------------------------------------------------------
* PURPOSE  : next timer interval for sending In_Lnk_chg_Down frames
*            has expired
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys    = channel reference
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            uid       = user identification==timer id
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_VOID mrp_in_timer_cbf_inLnkChg_down_send (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, LSA_USER_ID_TYPE uid)
{
  MRP_IN_EVENT_DB_TYPE DBinEv;
  MRP_IN_EVENT_DB_TYPE *pDBinEv = &DBinEv;

  LSA_UNUSED_ARG (uid);

  MRP_MEMSET (pDBinEv, 0, sizeof (MRP_IN_EVENT_DB_TYPE));

  pDBinEv->pChSys     = pChSys;
  pDBinEv->evInstance = pInstance[MRP_IN_INSTANCE_INDEX];

  mrp_in_update (pDBinEv, MRP_IN_EVENT_T_LNK_CHG_DOWN_TIMER);
}

/*===========================================================================
* FUNCTION : mrp_in_timer_cbf_inLnkChg_up_send
*----------------------------------------------------------------------------
* PURPOSE  : next timer interval for sending In_Lnk_chg_up frames
*            has expired
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys    = channel reference
*            pInstance = affected instance in array format (contains 
*                         MRP instance and MRP interconnection instance)
*            uid       = user identification==timer id
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
static LSA_VOID mrp_in_timer_cbf_inLnkChg_up_send (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, LSA_USER_ID_TYPE uid)
{
  MRP_IN_EVENT_DB_TYPE DBinEv;
  MRP_IN_EVENT_DB_TYPE *pDBinEv = &DBinEv;

  LSA_UNUSED_ARG (uid);

  MRP_MEMSET (pDBinEv, 0, sizeof (MRP_IN_EVENT_DB_TYPE));

  pDBinEv->pChSys = pChSys;
  pDBinEv->evInstance = pInstance[MRP_IN_INSTANCE_INDEX];

  mrp_in_update (pDBinEv, MRP_IN_EVENT_T_LNK_CHG_UP_TIMER);
}

/*===========================================================================
* FUNCTION : mrp_in_rcv
*----------------------------------------------------------------------------
* PURPOSE  : receive functions for mrp interconnection packets
*----------------------------------------------------------------------------
* RETURNS  : LSA_TRUE := known MRP frame type received
*----------------------------------------------------------------------------
* INPUTS   : pChSys         = channel reference
*            instance       = affected MRP instance
*            *pBuffer       = pointer to nrt buffer
*            userDataOffset = offset to mrp_in user data
*            len            = packet length
*            port           = receive port 1..numOfPorts
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
static LSA_UINT8 mrp_in_rcv (MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT8 *pBuffer, LSA_UINT16 userDataOffset, LSA_UINT16 len, LSA_UINT16 port)
{
  /* userDataOffset always points to Byte 18 (VLAN tagged) resp. Byte 14 (no VLAN tag) of MRP frame (->ver) */
  LSA_UINT16 *pVersion = (LSA_UINT16*)(pBuffer + userDataOffset); /*lint !e826 CM 10/11/2016 cast on purpose */
  LSA_BOOL isInWork, isInTrf;
  MRP_IN_EVENT_DB_TYPE DBinEv;
  MRP_IN_EVENT_DB_TYPE *pDBinEv = &DBinEv;
  LSA_UINT16 inInstance;

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  LSA_UNUSED_ARG(len);
  
  isInWork = 0;
  isInTrf = 0;
  MRP_MEMSET (pDBinEv, 0, sizeof (MRP_IN_EVENT_DB_TYPE));

  inInstance = mrp_in_sys_map_iport_to_inInstance (pChSys, port);

  LSA_UNUSED_ARG(inInstance);

  MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_CHAT, "mrp_in_rcv[inst=%d/inInst=%d] entered", instance, inInstance); 
  
  if (MRP_PACKET_VAL_VERSION == MRP_NTOH16(*pVersion))
  {
    MRP_PACKET_STRUCT_HEADER_TYPE *pFrameHdr = (MRP_PACKET_STRUCT_HEADER_TYPE *)pBuffer; /*lint !e826 CM 10/11/2016 cast on purpose */

    if (mrp_mib_write_vlanid (pChSys, instance, pFrameHdr->vlan.vlan_u32)) /* if valid VLAN ID */
    {
      pDBinEv->Vlan = MRP_NTOH32(pFrameHdr->vlan.vlan_u32);
    }

    if (!MRP_MEMCMP (pBuffer, (LSA_VOID_PTR_TYPE)MRP_IN_PACKET_VAL_DST_ADR_TRF, MRP_PACKET_VAL_MAC_ADDR_LEN))
    {
      MRP_MEMCPY (pDBinEv->RxPeerPortMacAddr, pFrameHdr->src, MRP_PACKET_VAL_MAC_ADDR_LEN);

      isInTrf = 1;
    }
    else
    {
      if (!MRP_MEMCMP (pBuffer, (LSA_VOID_PTR_TYPE)MRP_IN_PACKET_VAL_DST_ADR_WRK, MRP_PACKET_VAL_MAC_ADDR_LEN))
      {
        isInWork = 1;
      }
    }

    if (isInTrf || isInWork)
    {
      /* pointer to payload of MRP frame (Byte 20 resp. 16) */
      MRP_IN_PACKET_STRUCT_PAYLOAD_TPLG_CHG_TYPE *pFrameInTplgChg = (MRP_IN_PACKET_STRUCT_PAYLOAD_TPLG_CHG_TYPE*)(pBuffer + userDataOffset + MRP_PACKET_VAL_MRP_VERSION_LEN); /*lint !e826 CM 10/11/2016 cast on purpose */
      MRP_IN_PACKET_STRUCT_PAYLOAD_POLL_TYPE *pFrameInPoll = (MRP_IN_PACKET_STRUCT_PAYLOAD_POLL_TYPE*)(pBuffer + userDataOffset + MRP_PACKET_VAL_MRP_VERSION_LEN); /*lint !e826 CM 10/11/2016 cast on purpose */
      MRP_IN_PACKET_STRUCT_PAYLOAD_LNK_CHG_TYPE *pFrameInLnkChg = (MRP_IN_PACKET_STRUCT_PAYLOAD_LNK_CHG_TYPE*)(pBuffer + userDataOffset + MRP_PACKET_VAL_MRP_VERSION_LEN); /*lint !e826 CM 10/11/2016 cast on purpose */
      LSA_UINT16 interval;

      pDBinEv->pChSys = pChSys;

      if (MRP_IN_PACKET_VAL_TLV_TPLG_CHG == MRP_NTOH16 (pFrameInTplgChg->tlv))
      {
        pDBinEv->RxPort = port;
        MRP_MEMCPY (pDBinEv->RxSA, (LSA_VOID_PTR_TYPE)pFrameInTplgChg->sa, MRP_PACKET_VAL_MAC_ADDR_LEN);
        pDBinEv->RxInId = MRP_NTOH16 (pFrameInTplgChg->inId);
        /* MRP_MRM_EVENT_T_TPLG_CHG(RxTimeTcnt) */
        interval = MRP_NTOH16 (pFrameInTplgChg->interval);
        pDBinEv->RxTimeTcnt = (LSA_UINT16)(interval / MRP_TIMER_TIME_BASE_DEZ);
        pDBinEv->RxSequenceId = MRP_NTOH16 (pFrameInTplgChg->sequenceID);
        MRP_MEMCPY (pDBinEv->RxUUID, (LSA_VOID_PTR_TYPE)pFrameInTplgChg->domainUUID, sizeof (pDBinEv->RxUUID));
        MRP_PROGRAM_TRACE_05 (traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_rcv[inst=%d] IN_TPLG_CHG: inId=%d, port=%d, sequID=%u, Time=%u ms",
                              instance,
                              pDBinEv->RxInId,
                              port,
                              pDBinEv->RxSequenceId,
                              interval);
        MRP_PROGRAM_TRACE_BYTE_ARRAY(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_rcv IN_TPLG_CHG from=", pFrameInTplgChg->sa, MRP_PACKET_VAL_MAC_ADDR_LEN);

        if (isInTrf)
        {
          /* Frames with MC_INTRF MC address have to be checked only, if MRP interconnection is configured! */
          /* Don't care with respect to MRP_RSTP+! */
          mrp_in_update (pDBinEv, MRP_IN_EVENT_T_TPLG_CHG_TRF);
        }
        else
        {
          mrp_in_update (pDBinEv, MRP_IN_EVENT_T_TPLG_CHG_WRK);
        }

        return LSA_TRUE;
      }

      if (MRP_IN_PACKET_VAL_TLV_POLL == MRP_NTOH16 (pFrameInPoll->tlv))
      {
        pDBinEv->RxPort = port;
        MRP_MEMCPY (pDBinEv->RxSA, (LSA_VOID_PTR_TYPE)pFrameInPoll->sa, MRP_PACKET_VAL_MAC_ADDR_LEN);
        pDBinEv->RxPortRole = MRP_NTOH16 (pFrameInPoll->portRole);
        pDBinEv->RxInId = MRP_NTOH16 (pFrameInPoll->inId);
        pDBinEv->RxSequenceId = MRP_NTOH16 (pFrameInPoll->sequenceID);
        MRP_MEMCPY (pDBinEv->RxUUID, (LSA_VOID_PTR_TYPE)pFrameInPoll->domainUUID, sizeof (pDBinEv->RxUUID));
        inInstance = mrp_in_sys_map_inId_to_inInstance (pDBinEv->pChSys, pDBinEv->RxInId, pDBinEv->RxPort); /*lint -e838 */
        MRP_PROGRAM_TRACE_06 (traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_rcv[inst=%d/inInst=%d] IN_POLL: inId=%d, port=%d, sequID=%u, portRole=%d",
                              instance,
                              inInstance,
                              pDBinEv->RxInId,
                              port,
                              pDBinEv->RxSequenceId,
                              pDBinEv->RxPortRole);
        MRP_PROGRAM_TRACE_BYTE_ARRAY(traceIdx, LSA_TRACE_LEVEL_NOTE, "from MAC=", pFrameInPoll->sa, MRP_PACKET_VAL_MAC_ADDR_LEN);
        LSA_UNUSED_ARG(inInstance);   /* hhr: to please the compiler in case traces are disabled. */

        if (isInTrf)
        {
          /* Frames with MC_INTRF MC address have to be checked only, if MRP interconnection is configured! */
          /* Don't care with respect to MRP_RSTP+! */
          mrp_in_update (pDBinEv, MRP_IN_EVENT_T_POLL_TRF);
        }
        else
        {
          mrp_in_update (pDBinEv, MRP_IN_EVENT_T_POLL_WRK);
        }

        return LSA_TRUE; 
      }

      if (MRP_IN_PACKET_VAL_TLV_LNK_DOWN == MRP_NTOH16 (pFrameInLnkChg->tlv))
      {
        pDBinEv->LinkState = MRP_LINK_STATE_DOWN;
        pDBinEv->RxPort    = port;

        MRP_MEMCPY (pDBinEv->RxSA, (LSA_VOID_PTR_TYPE)pFrameInLnkChg->sa, MRP_PACKET_VAL_MAC_ADDR_LEN);
        pDBinEv->RxPortRole = MRP_NTOH16(pFrameInLnkChg->portRole);
        pDBinEv->RxInId = MRP_NTOH16(pFrameInLnkChg->inId);
        interval = MRP_NTOH16(pFrameInLnkChg->interval);
        pDBinEv->RxTimeTcnt = (LSA_UINT16)(interval / MRP_TIMER_TIME_BASE_DEZ);        
        pDBinEv->RxLnkRole = MRP_NTOH16 (pFrameInLnkChg->linkRole);
        pDBinEv->RxSequenceId = MRP_NTOH16(pFrameInLnkChg->sequenceID);
        MRP_MEMCPY (pDBinEv->RxUUID, (LSA_VOID_PTR_TYPE)pFrameInLnkChg->domainUUID, sizeof (pDBinEv->RxUUID));

        pDBinEv->LinkAddInfo.AddCheckEnabled = (LSA_UINT8)((pDBinEv->RxLnkRole >> MRP_IN_PACKET_VAL_LNK_ADD_CHECK_BIT_OFFSET) & 0x0F);
        pDBinEv->LinkAddInfo.ChangeReason = (LSA_UINT8)((pDBinEv->RxLnkRole >> MRP_IN_PACKET_VAL_LNK_CHANGE_REASON_BIT_OFFSET) & 0x0F);

        inInstance = mrp_in_sys_map_inId_to_inInstance (pDBinEv->pChSys, pDBinEv->RxInId, pDBinEv->RxPort); /*lint -e838 */
        MRP_PROGRAM_TRACE_08 (traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_rcv[inst=%d/inInst=%d] IN_LNK_CHG_DOWN: inId=%d, port=%d, sequID=%u, portRole=%d, AddCheckEna=%d, ChangeReason=%d",
                              instance,
                              inInstance,
                              pDBinEv->RxInId,
                              port,
                              pDBinEv->RxSequenceId, 
                              pDBinEv->RxPortRole,
                              pDBinEv->LinkAddInfo.AddCheckEnabled, 
                              pDBinEv->LinkAddInfo.ChangeReason);
        MRP_PROGRAM_TRACE_BYTE_ARRAY(traceIdx, LSA_TRACE_LEVEL_NOTE, "from MAC=", pFrameInLnkChg->sa, MRP_PACKET_VAL_MAC_ADDR_LEN);
        LSA_UNUSED_ARG(inInstance);   /* hhr: to please the compiler in case traces are disabled. */

        if (isInTrf)
        {
          /* Frames with MC_INTRF MC address have to be checked only, if MRP interconnection is configured! */
          /* Don't care with respect to MRP_RSTP+! */
          mrp_in_update (pDBinEv, MRP_IN_EVENT_T_LNK_CHG_TRF);
        }
        else
        {
          mrp_in_update (pDBinEv, MRP_IN_EVENT_T_LNK_CHG_WRK);
        }

        return LSA_TRUE;
      }

      if (MRP_IN_PACKET_VAL_TLV_LNK_UP == MRP_NTOH16 (pFrameInLnkChg->tlv))
      {
        pDBinEv->LinkState = MRP_LINK_STATE_UP;
        pDBinEv->RxPort    = port;

        MRP_MEMCPY (pDBinEv->RxSA, (LSA_VOID_PTR_TYPE)pFrameInLnkChg->sa, MRP_PACKET_VAL_MAC_ADDR_LEN);
        pDBinEv->RxPortRole = MRP_NTOH16 (pFrameInLnkChg->portRole);
        pDBinEv->RxInId = MRP_NTOH16 (pFrameInLnkChg->inId);
        interval = MRP_NTOH16 (pFrameInLnkChg->interval);
        pDBinEv->RxTimeTcnt = (LSA_UINT16)(interval / MRP_TIMER_TIME_BASE_DEZ);
        pDBinEv->RxLnkRole = MRP_NTOH16 (pFrameInLnkChg->linkRole);
        pDBinEv->RxSequenceId = MRP_NTOH16 (pFrameInLnkChg->sequenceID);
        MRP_MEMCPY (pDBinEv->RxUUID, (LSA_VOID_PTR_TYPE)pFrameInLnkChg->domainUUID, sizeof (pDBinEv->RxUUID));

        pDBinEv->LinkAddInfo.AddCheckEnabled = (LSA_UINT8)((pDBinEv->RxLnkRole >> MRP_IN_PACKET_VAL_LNK_ADD_CHECK_BIT_OFFSET) & 0x0F);
        pDBinEv->LinkAddInfo.ChangeReason = (LSA_UINT8)((pDBinEv->RxLnkRole >> MRP_IN_PACKET_VAL_LNK_CHANGE_REASON_BIT_OFFSET) & 0x0F);

        inInstance = mrp_in_sys_map_inId_to_inInstance (pDBinEv->pChSys, pDBinEv->RxInId, pDBinEv->RxPort); /*lint -e838 */
        MRP_PROGRAM_TRACE_08 (traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_rcv[inst=%d/inInst=%d] IN_LNK_CHG_UP: inId=%d, port=%d, sequID=%u, portRole=%d, AddCheckEna=%d, ChangeReason=%d",
                              instance,
                              inInstance,
                              pDBinEv->RxInId,
                              port,
                              pDBinEv->RxSequenceId,
                              pDBinEv->RxPortRole,
                              pDBinEv->LinkAddInfo.AddCheckEnabled,
                              pDBinEv->LinkAddInfo.ChangeReason);
        MRP_PROGRAM_TRACE_BYTE_ARRAY(traceIdx, LSA_TRACE_LEVEL_NOTE, "from MAC=", pFrameInLnkChg->sa, MRP_PACKET_VAL_MAC_ADDR_LEN);
        LSA_UNUSED_ARG(inInstance);   /* hhr: to please the compiler in case traces are disabled. */

        if (isInTrf)
        {
          /* Frames with MC_INTRF MC address have to be checked only, if MRP interconnection is configured! */
          /* Don't care with respect to MRP_RSTP+! */
          mrp_in_update (pDBinEv, MRP_IN_EVENT_T_LNK_CHG_TRF);
        }
        else
        {
          mrp_in_update (pDBinEv, MRP_IN_EVENT_T_LNK_CHG_WRK);
        }

        return LSA_TRUE;
      }
    }
  }
  return LSA_FALSE;
}



/*===========================================================================
* FUNCTION : mrp_in_init_snd_rqb
*----------------------------------------------------------------------------
* PURPOSE  : init send RQBs for MRP interconnection resp. MRP_RSTP+
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys           = channel reference
*            mrpMaxInstance   = maximum number of MRP instances to be initialized
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_in_init_snd_rqb (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpInMaxInstance)
{
  MRP_EDD_LOWER_RQB_PTR_TYPE pRQB;
  MRP_IN_PACKET_STRUCT_TPLG_CHG_TYPE *pFrameInTplgChg;
  MRP_IN_PACKET_STRUCT_POLL_TYPE *pFrameInPoll;
  MRP_IN_PACKET_STRUCT_LNK_CHG_TYPE *pFrameInLnkChg;
  LSA_UINT16 cnt;
  LSA_UINT16 i, k;
  MRP_IF_DB_TYPE *pDBif = (MRP_IF_DB_TYPE *)pChSys->pChSysDB->pInterfacePara;
  LSA_UINT16  pInstance[2];

  pInstance[MRP_INSTANCE_INDEX]   = MRP_DEFAULT_INSTANCE;

  if (mrpInMaxInstance > 0)
  {
    for (i = 0; i < mrpInMaxInstance; i++)
    {
      pInstance[MRP_IN_INSTANCE_INDEX]  = i;
      /*---------------------------------------------------------------------------*/
      /*  RQB initialization MRP_IN_TC (WRK) from RSTP+ originator                 */
      /*---------------------------------------------------------------------------*/
      for (k = 0, cnt = mrp_edd_get_snd_rqb_max (pChSys, pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_ORIG_BOT); k < cnt; k++)
      {
        /* IN_TC resources used by an IN_ORIGINATOR with WRK MC address */
        (void)(mrp_edd_get_snd_rqb (pChSys, pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_ORIG_BOT, &pRQB, MRP_EDD_GET_SND_RQB_NO_THRESHOLD));

        MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL));

        pFrameInTplgChg = (MRP_IN_PACKET_STRUCT_TPLG_CHG_TYPE*)MRP_EDD_LOWER_GET_NRT_TX_BUFFER(pRQB); /*lint !e826 CM 10/11/2016 cast on purpose */

        MRP_MEMCPY((LSA_VOID_PTR_TYPE)pFrameInTplgChg, (LSA_VOID_PTR_TYPE)MRP_PACKET_VAL_DEFAULT, sizeof(MRP_PACKET_VAL_DEFAULT) - 1);

        MRP_MEMCPY((LSA_VOID_PTR_TYPE)pFrameInTplgChg->hdr.dst, (LSA_VOID_PTR_TYPE)MRP_IN_PACKET_VAL_DST_ADR_WRK, sizeof(pFrameInTplgChg->hdr.dst));
        MRP_MEMCPY((LSA_VOID_PTR_TYPE)pFrameInTplgChg->hdr.src, pDBif->macAddr, sizeof(pFrameInTplgChg->hdr.src));

        pFrameInTplgChg->payload.tlv = MRP_HTON16(MRP_IN_PACKET_VAL_TLV_TPLG_CHG);
        MRP_MEMCPY((LSA_VOID_PTR_TYPE)pFrameInTplgChg->payload.sa, pDBif->macAddr, sizeof(pFrameInTplgChg->payload.sa));
        pFrameInTplgChg->payload.tlv_common = MRP_HTON16(MRP_PACKET_VAL_TLV_COMMON);
        MRP_MEMSET((LSA_VOID_PTR_TYPE)pFrameInTplgChg->payload.domainUUID, 0xFF, sizeof(pFrameInTplgChg->payload.domainUUID));

        /* Append the UUID of the MRP interconnection coupling device to the InTC frame. This is a manufacturer specific option. */
        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)(&pFrameInTplgChg->payload.uSuffix.inTcOption), (LSA_VOID_PTR_TYPE)MRP_IN_PACKET_VAL_OPTION_COUPLED_UUID, sizeof (pFrameInTplgChg->payload.uSuffix.inTcOption));
        MRP_MEMSET ((LSA_VOID_PTR_TYPE)pFrameInTplgChg->payload.uSuffix.inTcOption.coupledDomainUUID, 0xFF, sizeof (pFrameInTplgChg->payload.uSuffix.inTcOption.coupledDomainUUID));

        mrp_edd_set_snd_rqb (pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_ORIG_BOT, pRQB);
      }

      /*----------------------------------------------------------------------------------*/
      /*  RQB initialization MRP_IN_TC (WRK) from MRP interconnection coupling device     */
      /*----------------------------------------------------------------------------------*/
      for (k = 0, cnt = mrp_edd_get_snd_rqb_max (pChSys, pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_BOT); k < cnt; k++)
      {
        /* IN_TC resources used by an MRP interconnection coupling device with WRK MC address (01:15:4e:00:00:04) */
        (void) (mrp_edd_get_snd_rqb (pChSys, pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_BOT, &pRQB, MRP_EDD_GET_SND_RQB_NO_THRESHOLD));

        MRP_FATAL1 (!LSA_HOST_PTR_ARE_EQUAL (pRQB, LSA_NULL));

        pFrameInTplgChg = (MRP_IN_PACKET_STRUCT_TPLG_CHG_TYPE*)MRP_EDD_LOWER_GET_NRT_TX_BUFFER (pRQB); /*lint !e826 CM 10/11/2016 cast on purpose */

        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInTplgChg, (LSA_VOID_PTR_TYPE)MRP_PACKET_VAL_DEFAULT, sizeof (MRP_PACKET_VAL_DEFAULT) - 1);

        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInTplgChg->hdr.dst, (LSA_VOID_PTR_TYPE)MRP_IN_PACKET_VAL_DST_ADR_WRK, sizeof (pFrameInTplgChg->hdr.dst));
        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInTplgChg->hdr.src, pDBif->macAddr, sizeof (pFrameInTplgChg->hdr.src));

        pFrameInTplgChg->payload.tlv = MRP_HTON16 (MRP_IN_PACKET_VAL_TLV_TPLG_CHG);
        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInTplgChg->payload.sa, pDBif->macAddr, sizeof (pFrameInTplgChg->payload.sa));
        pFrameInTplgChg->payload.tlv_common = MRP_HTON16 (MRP_PACKET_VAL_TLV_COMMON);
        MRP_MEMSET ((LSA_VOID_PTR_TYPE)pFrameInTplgChg->payload.domainUUID, 0xFF, sizeof (pFrameInTplgChg->payload.domainUUID));

        /* Append the UUID of the MRP interconnection coupling device to the InTC frame. This is a manufacturer specific option. */
        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)(&pFrameInTplgChg->payload.uSuffix.inTcOption), (LSA_VOID_PTR_TYPE)MRP_IN_PACKET_VAL_OPTION_COUPLED_UUID, sizeof (pFrameInTplgChg->payload.uSuffix.inTcOption));
        MRP_MEMSET ((LSA_VOID_PTR_TYPE)pFrameInTplgChg->payload.uSuffix.inTcOption.coupledDomainUUID, 0xFF, sizeof (pFrameInTplgChg->payload.uSuffix.inTcOption.coupledDomainUUID));

        mrp_edd_set_snd_rqb (pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_BOT, pRQB);
      }

      /*----------------------------------------------------------------------------------*/
      /*  RQB initialization MRP_IN_TC (TRF) from MRP interconnection coupling device     */
      /*----------------------------------------------------------------------------------*/
      for (k = 0, cnt = mrp_edd_get_snd_rqb_max (pChSys, pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_TC_BOT); k < cnt; k++)
      {
        /* IN_TC resources used by an MRP interconnection coupling device with TRF MC address (01:15:4e:00:00:05) */
        (void) (mrp_edd_get_snd_rqb (pChSys, pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_TC_BOT, &pRQB, MRP_EDD_GET_SND_RQB_NO_THRESHOLD));

        MRP_FATAL1 (!LSA_HOST_PTR_ARE_EQUAL (pRQB, LSA_NULL));

        pFrameInTplgChg = (MRP_IN_PACKET_STRUCT_TPLG_CHG_TYPE*)MRP_EDD_LOWER_GET_NRT_TX_BUFFER (pRQB); /*lint !e826 CM 10/11/2016 cast on purpose */

        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInTplgChg, (LSA_VOID_PTR_TYPE)MRP_PACKET_VAL_DEFAULT, sizeof (MRP_PACKET_VAL_DEFAULT) - 1);

        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInTplgChg->hdr.dst, (LSA_VOID_PTR_TYPE)MRP_IN_PACKET_VAL_DST_ADR_TRF, sizeof (pFrameInTplgChg->hdr.dst));
        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInTplgChg->hdr.src, pDBif->macAddr, sizeof (pFrameInTplgChg->hdr.src));

        pFrameInTplgChg->payload.tlv = MRP_HTON16 (MRP_IN_PACKET_VAL_TLV_TPLG_CHG);
        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInTplgChg->payload.sa, pDBif->macAddr, sizeof (pFrameInTplgChg->payload.sa));
        pFrameInTplgChg->payload.tlv_common = MRP_HTON16 (MRP_PACKET_VAL_TLV_COMMON);
        MRP_MEMSET ((LSA_VOID_PTR_TYPE)pFrameInTplgChg->payload.domainUUID, 0xFF, sizeof (pFrameInTplgChg->payload.domainUUID));

        /* Append the UUID of the MRP interconnection coupling device to the InTC frame. This is a manufacturer specific option. */
        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)(&pFrameInTplgChg->payload.uSuffix.inTcOption), (LSA_VOID_PTR_TYPE)MRP_IN_PACKET_VAL_OPTION_COUPLED_UUID, sizeof (pFrameInTplgChg->payload.uSuffix.inTcOption));
        MRP_MEMSET ((LSA_VOID_PTR_TYPE)pFrameInTplgChg->payload.uSuffix.inTcOption.coupledDomainUUID, 0xFF, sizeof (pFrameInTplgChg->payload.uSuffix.inTcOption.coupledDomainUUID));

        mrp_edd_set_snd_rqb (pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_TC_BOT, pRQB);
      }

      /*----------------------------------------------------------------------------------*/
      /*  RQB initialization MRP_IN_POLL (WRK) from MRP interconnection coupling device   */
      /*----------------------------------------------------------------------------------*/
      for (k = 0, cnt = mrp_edd_get_snd_rqb_max (pChSys, pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_POLL_BOT); k < cnt; k++)
      {
        /* IN_TC resources used by an MRP interconnection coupling device with WRK MC address (01:15:4e:00:00:04) */
        (void) (mrp_edd_get_snd_rqb (pChSys, pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_POLL_BOT, &pRQB, MRP_EDD_GET_SND_RQB_NO_THRESHOLD));

        MRP_FATAL1 (!LSA_HOST_PTR_ARE_EQUAL (pRQB, LSA_NULL));

        pFrameInPoll = (MRP_IN_PACKET_STRUCT_POLL_TYPE*)MRP_EDD_LOWER_GET_NRT_TX_BUFFER (pRQB); /*lint !e826 CM 10/11/2016 cast on purpose */

        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInPoll, (LSA_VOID_PTR_TYPE)MRP_PACKET_VAL_DEFAULT, sizeof (MRP_PACKET_VAL_DEFAULT) - 1);

        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInPoll->hdr.dst, (LSA_VOID_PTR_TYPE)MRP_IN_PACKET_VAL_DST_ADR_WRK, sizeof (pFrameInPoll->hdr.dst));
        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInPoll->hdr.src, pDBif->macAddr, sizeof (pFrameInPoll->hdr.src));

        pFrameInPoll->payload.tlv = MRP_HTON16 (MRP_IN_PACKET_VAL_TLV_POLL);
        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInPoll->payload.sa, pDBif->macAddr, sizeof (pFrameInPoll->payload.sa));
        pFrameInPoll->payload.portRole = MRP_HTON16 (MRP_IN_PACKET_VAL_PORT_ROLE_PRM);
        pFrameInPoll->payload.tlv_common = MRP_HTON16 (MRP_PACKET_VAL_TLV_COMMON);
        MRP_MEMSET ((LSA_VOID_PTR_TYPE)pFrameInPoll->payload.domainUUID, 0xFF, sizeof (pFrameInPoll->payload.domainUUID));

        mrp_edd_set_snd_rqb (pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_POLL_BOT, pRQB);
      }

      /*----------------------------------------------------------------------------------*/
      /*  RQB initialization MRP_IN_POLL (TRF) from MRP interconnection coupling device   */
      /*----------------------------------------------------------------------------------*/
      for (k = 0, cnt = mrp_edd_get_snd_rqb_max (pChSys, pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_POLL_BOT); k < cnt; k++)
      {
        /* IN_TC resources used by an MRP interconnection coupling device with TRF MC address (01:15:4e:00:00:05) */
        (void) (mrp_edd_get_snd_rqb (pChSys, pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_POLL_BOT, &pRQB, MRP_EDD_GET_SND_RQB_NO_THRESHOLD));

        MRP_FATAL1 (!LSA_HOST_PTR_ARE_EQUAL (pRQB, LSA_NULL));

        pFrameInPoll = (MRP_IN_PACKET_STRUCT_POLL_TYPE*)MRP_EDD_LOWER_GET_NRT_TX_BUFFER (pRQB); /*lint !e826 CM 10/11/2016 cast on purpose */

        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInPoll, (LSA_VOID_PTR_TYPE)MRP_PACKET_VAL_DEFAULT, sizeof (MRP_PACKET_VAL_DEFAULT) - 1);

        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInPoll->hdr.dst, (LSA_VOID_PTR_TYPE)MRP_IN_PACKET_VAL_DST_ADR_TRF, sizeof (pFrameInPoll->hdr.dst));
        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInPoll->hdr.src, pDBif->macAddr, sizeof (pFrameInPoll->hdr.src));

        pFrameInPoll->payload.tlv = MRP_HTON16 (MRP_IN_PACKET_VAL_TLV_POLL);
        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInPoll->payload.sa, pDBif->macAddr, sizeof (pFrameInPoll->payload.sa));
        pFrameInPoll->payload.portRole = MRP_HTON16 (MRP_IN_PACKET_VAL_PORT_ROLE_IN);
        pFrameInPoll->payload.tlv_common = MRP_HTON16 (MRP_PACKET_VAL_TLV_COMMON);
        MRP_MEMSET ((LSA_VOID_PTR_TYPE)pFrameInPoll->payload.domainUUID, 0xFF, sizeof (pFrameInPoll->payload.domainUUID));

        mrp_edd_set_snd_rqb (pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_POLL_BOT, pRQB);
      }

      /*-------------------------------------------------------------------------------------*/
      /*  RQB initialization MRP_IN_LNK_CHG (WRK) from MRP interconnection coupling device   */
      /*-------------------------------------------------------------------------------------*/
      for (k = 0, cnt = mrp_edd_get_snd_rqb_max (pChSys, pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_LNK_CHG_BOT); k < cnt; k++)
      {
        /* IN_TC resources used by an MRP interconnection coupling device with WRK MC address (01:15:4e:00:00:04) */
        (void) (mrp_edd_get_snd_rqb (pChSys, pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_LNK_CHG_BOT, &pRQB, MRP_EDD_GET_SND_RQB_NO_THRESHOLD));

        MRP_FATAL1 (!LSA_HOST_PTR_ARE_EQUAL (pRQB, LSA_NULL));

        pFrameInLnkChg = (MRP_IN_PACKET_STRUCT_LNK_CHG_TYPE*)MRP_EDD_LOWER_GET_NRT_TX_BUFFER (pRQB); /*lint !e826 CM 10/11/2016 cast on purpose */

        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInLnkChg, (LSA_VOID_PTR_TYPE)MRP_PACKET_VAL_DEFAULT, sizeof (MRP_PACKET_VAL_DEFAULT) - 1);

        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInLnkChg->hdr.dst, (LSA_VOID_PTR_TYPE)MRP_IN_PACKET_VAL_DST_ADR_WRK, sizeof (pFrameInLnkChg->hdr.dst));
        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInLnkChg->hdr.src, pDBif->macAddr, sizeof (pFrameInLnkChg->hdr.src));

        pFrameInLnkChg->payload.tlv = MRP_HTON16 (MRP_IN_PACKET_VAL_TLV_TPLG_CHG);
        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInLnkChg->payload.sa, pDBif->macAddr, sizeof (pFrameInLnkChg->payload.sa));
        pFrameInLnkChg->payload.portRole = MRP_HTON16 (MRP_IN_PACKET_VAL_PORT_ROLE_PRM);
        pFrameInLnkChg->payload.linkRole = MRP_HTON16 (MRP_IN_PACKET_VAL_LNK_ROLE_BIT0_PRM);
        pFrameInLnkChg->payload.tlv_common = MRP_HTON16 (MRP_PACKET_VAL_TLV_COMMON);
        MRP_MEMSET ((LSA_VOID_PTR_TYPE)pFrameInLnkChg->payload.domainUUID, 0xFF, sizeof (pFrameInLnkChg->payload.domainUUID));

        mrp_edd_set_snd_rqb (pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_LNK_CHG_BOT, pRQB);
      }

      /*-------------------------------------------------------------------------------------*/
      /*  RQB initialization MRP_IN_LNK_CHG (TRF) from MRP interconnection coupling device   */
      /*-------------------------------------------------------------------------------------*/
      for (k = 0, cnt = mrp_edd_get_snd_rqb_max (pChSys, pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_LNK_CHG_BOT); k < cnt; k++)
      {
        /* IN_TC resources used by an MRP interconnection coupling device with TRF MC address (01:15:4e:00:00:05) */
        (void) (mrp_edd_get_snd_rqb (pChSys, pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_LNK_CHG_BOT, &pRQB, MRP_EDD_GET_SND_RQB_NO_THRESHOLD));

        MRP_FATAL1 (!LSA_HOST_PTR_ARE_EQUAL (pRQB, LSA_NULL));

        pFrameInLnkChg = (MRP_IN_PACKET_STRUCT_LNK_CHG_TYPE*)MRP_EDD_LOWER_GET_NRT_TX_BUFFER (pRQB); /*lint !e826 CM 10/11/2016 cast on purpose */

        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInLnkChg, (LSA_VOID_PTR_TYPE)MRP_PACKET_VAL_DEFAULT, sizeof (MRP_PACKET_VAL_DEFAULT) - 1);

        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInLnkChg->hdr.dst, (LSA_VOID_PTR_TYPE)MRP_IN_PACKET_VAL_DST_ADR_TRF, sizeof (pFrameInLnkChg->hdr.dst));
        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInLnkChg->hdr.src, pDBif->macAddr, sizeof (pFrameInLnkChg->hdr.src));

        pFrameInLnkChg->payload.tlv = MRP_HTON16 (MRP_IN_PACKET_VAL_TLV_TPLG_CHG);
        MRP_MEMCPY ((LSA_VOID_PTR_TYPE)pFrameInLnkChg->payload.sa, pDBif->macAddr, sizeof (pFrameInLnkChg->payload.sa));
        pFrameInLnkChg->payload.portRole = MRP_HTON16 (MRP_IN_PACKET_VAL_PORT_ROLE_IN);
        pFrameInLnkChg->payload.linkRole = MRP_HTON16 (MRP_IN_PACKET_VAL_LNK_ROLE_BIT0_PRM);
        pFrameInLnkChg->payload.tlv_common = MRP_HTON16 (MRP_PACKET_VAL_TLV_COMMON);
        MRP_MEMSET ((LSA_VOID_PTR_TYPE)pFrameInLnkChg->payload.domainUUID, 0xFF, sizeof (pFrameInLnkChg->payload.domainUUID));

        mrp_edd_set_snd_rqb (pInstance, MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_LNK_CHG_BOT, pRQB);
      }
    }
  }
}


/*===========================================================================
* FUNCTION : mrp_in_base_init_var
*----------------------------------------------------------------------------
* PURPOSE  : initialize variables for MRP interconnection
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : *pDBin     = pointer to one instance of MRP interconnection database
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_in_base_init_var (MRP_IN_DB_TYPE *pDBin)
{
  MRP_IF_DB_TYPE *pDBif = (MRP_IF_DB_TYPE *)pDBin->pChSys->pChSysDB->pInterfacePara;

  MRP_PROGRAM_TRACE_01(pDBin->pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "mrp_in_base_init_var[inInst=%d] entered", pDBin->pInstance[MRP_IN_INSTANCE_INDEX]);

  /* variables */
  MRP_MEMSET (pDBin->DomainUUID, 0xFF, sizeof(pDBin->DomainUUID));                 /* DomainUUID of own MRP ring     */
  MRP_MEMSET (pDBin->CoupledDomainUUID, 0xFF, sizeof (pDBin->CoupledDomainUUID));  /* DomainUUID of coupled MRP ring */
  MRP_MEMCPY (pDBin->own_SA, pDBif->macAddr, MRP_PACKET_VAL_MAC_ADDR_LEN);

  pDBin->InID                 = MRP_IN_DEFAULT_VAL_IN_ID;
  pDBin->pInOperStateBaseCbf  = mrp_in_oper_state_off_ac_stat1;

  pDBin->TOPRstppIntervalTcnt = (LSA_UINT16)((MRP_IN_DEFAULT_VAL_IN_TC_CHG_T_CNT * MRP_IN_DEFAULT_VAL_IN_TC_CHG_T_RES_MS) / MRP_TIMER_TIME_BASE_DEZ);
  pDBin->TOPRsttppMaxRepeat   = MRP_IN_DEFAULT_VAL_IN_TC_NR_MAX;
  pDBin->TOPRstppNReturn      = pDBin->TOPRsttppMaxRepeat;
  /* NOTE: InTC is REPEATED MRPIC_TOPNRmax times */
  /* first InTC is sent with timestamp SetupInterconnTopologyChangeReq(MRP_IN_TOPNRmax X time) -> number of sent InTC frames per port: MRPIC_TOPNRmax + 1 */

  /* internals */
  pDBin->rxPara.CurrentEvent  = MRP_IN_EVENT_T_NONE;
  pDBin->rxPara.Vlan          = 0;
  pDBin->prevTimeTicks        = mrp_sys_cyclic_get_curr_time ();

  /* event attributes */
  MRP_MEMSET (pDBin->rxPara.RxSA, 0, MRP_PACKET_VAL_MAC_ADDR_LEN);
  MRP_MEMSET (pDBin->rxPara.RxPeerPortMacAddr, 0, MRP_PACKET_VAL_MAC_ADDR_LEN);
  MRP_MEMSET (pDBin->rxPara.RxUUID, 0, 16);
  pDBin->rxPara.RxTimeTcnt        = 0;
  pDBin->rxPara.RxPort            = 0;
}




/*===========================================================================
* FUNCTION : mrp_in_base_stop
*----------------------------------------------------------------------------
* PURPOSE  : stop one MRP interconnection base instance
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys     = channel reference
*            inInstance = (MRP) interconnection instance to be stopped
* OUTPUTS  : 
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_in_base_stop (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 inInstance)
{
  LSA_UINT8 inFwdRulesSupported;
  MRP_IN_DB_TYPE *pDBin;
  MRP_IN_DB_TYPE *pDBinBase = (MRP_IN_DB_TYPE *)pChSys->pChSysDB->pMrpIn;

  pDBin = &pDBinBase[inInstance];

  MRP_PROGRAM_TRACE_01(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_base_stop[inInst=%d] entered", inInstance);
  
  mrp_sys_timer_stop (pDBin->TimerIdInRstppTCsend);
  mrp_sys_timer_stop (pDBin->TimerIdInTCsend);
  mrp_sys_timer_stop (pDBin->TimerIdInPollsend);
  mrp_sys_timer_stop (pDBin->TimerIdInLnkChgDownSend);
  mrp_sys_timer_stop (pDBin->TimerIdInLnkChgUpSend);

  pDBin->pInProjStateBaseCbf = mrp_in_proj_state_off_cbf;

  inFwdRulesSupported = mrp_in_sys_get_in_fwd_rules_supported (pChSys);

  if (inFwdRulesSupported != MRP_IN_FWD_RULES_SUPPORTED_OFF && !inInstance)
  {
    mrp_edd_remove_rcv_callback (pChSys, MRP_DEFAULT_INSTANCE, mrp_in_rcv);
  }
  mrp_in_base_init_var (pDBin);

  pDBin->runState = MRP_IN_OPERATION_OFF;

  MRP_PROGRAM_TRACE_01 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_base_stop[inInst=%d] exit", inInstance);
}


/*===========================================================================
* FUNCTION : mrp_in_base_start
*----------------------------------------------------------------------------
* PURPOSE  : start one MRP interconnection base instance
*----------------------------------------------------------------------------
* RETURNS  :
*----------------------------------------------------------------------------
* INPUTS   : pChSys           = channel reference
*            inInstance       = (MRP) interconnection instance to be started
*            isStartedFromMrm = is parent MRP instance an MRM instance or 
*                               an MRC instance
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS :
*==========================================================================*/
LSA_VOID mrp_in_base_start (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 inInstance, LSA_UINT8 isStartedFromMrm)
{
  LSA_UINT8 inFwdRulesSupported;
  MRP_IN_DB_TYPE *pDBin;
  MRP_IN_DB_TYPE *pDBinBase = (MRP_IN_DB_TYPE *)pChSys->pChSysDB->pMrpIn;

  pDBin = &pDBinBase[inInstance];

  MRP_PROGRAM_TRACE_01 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_base_start[inInst=%d] entered", inInstance);

  mrp_in_base_init_var (pDBin);

  pDBin->runState         = MRP_IN_OPERATION_ON;
  pDBin->isStartedFromMrm = isStartedFromMrm;

  pDBin->pInProjStateBaseCbf = mrp_in_proj_state_off_cbf;

  inFwdRulesSupported = mrp_in_sys_get_in_fwd_rules_supported (pChSys);

  if (inFwdRulesSupported != MRP_IN_FWD_RULES_SUPPORTED_OFF && !inInstance)
  {
    mrp_edd_set_rcv_callback (pChSys, MRP_DEFAULT_INSTANCE, mrp_in_rcv);
  }

  MRP_PROGRAM_TRACE_01 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_base_start[inInst=%d] exit", inInstance);
}



/*===========================================================================
* FUNCTION : mrp_in_init
*----------------------------------------------------------------------------
* PURPOSE  : initialize MRP interconnection data structures
*----------------------------------------------------------------------------
* RETURNS  : 
*----------------------------------------------------------------------------
* INPUTS   : pChSys           = channel reference
*            mrpMaxInstance   = maximum number of MRP instances to be initialized
*            mrpInMaxInstance = maximum number of MRP interconnection instances
*                               to be initialized
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_in_init (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance)
{
  LSA_UINT32 timerIdx;
  LSA_UINT16 i;
  MRP_IN_DB_TYPE *pDBin;
  MRP_MIB_DB_TYPE *pDBmibBase;
  MRP_IN_DB_TYPE *pDBinBase = LSA_NULL;

  MRP_TRACE_CREATE_LOCAL_VARIABLE(traceIdx, pChSys->pChSysDB->traceIdx);   /* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */

  MRP_PROGRAM_TRACE_02(traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_init entered, mrpMaxInstance=%d, mrpInMaxInstance=%d", mrpMaxInstance, mrpInMaxInstance);
  
  MRP_ASSERT (mrpInMaxInstance <= EDD_CFG_MAX_MRP_IN_INSTANCE_CNT);

  if (mrpMaxInstance)
  {
    if (mrpInMaxInstance)
    {
      MRP_ALLOC_LOCAL_MEM ((MRP_LOCAL_MEM_PTR_TYPE*)(&pDBinBase), (LSA_UINT16)(sizeof (MRP_IN_DB_TYPE) * mrpInMaxInstance));
      MRP_FATAL1 (!LSA_HOST_PTR_ARE_EQUAL (pDBinBase, LSA_NULL));
      pChSys->pChSysDB->pMrpIn = pDBinBase;

      pDBmibBase = (MRP_MIB_DB_TYPE *)pChSys->pChSysDB->pMrpMIB;

      for (i = 0; i < mrpInMaxInstance; i++)
      {
        pDBin = &pDBinBase[i];
        /* preset zero */
        MRP_MEMSET (pDBin, 0, sizeof (MRP_IN_DB_TYPE));

        pDBin->pDBmib = &pDBmibBase[i];

        pDBin->pChSys   = pChSys;
        pDBin->pDBifRef = (MRP_IF_DB_TYPE *)pChSys->pChSysDB->pInterfacePara;

        pDBin->adminRole = MRP_IN_ADMIN_ROLE_OFF;
        pDBin->operRole  = MRP_IN_OPER_ROLE_OFF;

        pDBin->pInstance[MRP_INSTANCE_INDEX] = MRP_DEFAULT_INSTANCE;
        pDBin->pInstance[MRP_IN_INSTANCE_INDEX] = i;
      }

      for (i = 0; i < mrpInMaxInstance; i++)
      {
        pDBin = &pDBinBase[i];

        pDBin->TimerIdInRstppTCsend = mrp_sys_timer_get (pChSys, MRP_TIMER_POOL_IN_INSTANCE, i, LSA_TIMER_TYPE_ONE_SHOT, MRP_TIMER_ENUM_MRP_RSTPP_IN_TC_SEND, (MRP_TIMER_CBF_FCT_PTR_TYPE)mrp_in_timer_cbf_rstpp_inTC_send);
        timerIdx = pDBin->TimerIdInRstppTCsend % MRP_MAX_NO_OF_TIMER_IDS_PER_INTERFACE; 
        MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_init[inInst=%d]: timer_get called for TimerIdInRstppTCsend: id=%d, idx=%d", i, pDBin->TimerIdInRstppTCsend, timerIdx);

        pDBin->TimerIdInTCsend = mrp_sys_timer_get (pChSys, MRP_TIMER_POOL_IN_INSTANCE, i, LSA_TIMER_TYPE_ONE_SHOT, MRP_TIMER_ENUM_MRP_IN_TC_SEND, (MRP_TIMER_CBF_FCT_PTR_TYPE)mrp_in_timer_cbf_inTC_send);
        timerIdx = pDBin->TimerIdInTCsend % MRP_MAX_NO_OF_TIMER_IDS_PER_INTERFACE;  /*lint -e838 */
        MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_init[inInst=%d]: timer_get called for TimerIdInTCsend: id=%d, idx=%d", i, pDBin->TimerIdInTCsend, timerIdx);

        pDBin->TimerIdInPollsend = mrp_sys_timer_get (pChSys, MRP_TIMER_POOL_IN_INSTANCE, i, LSA_TIMER_TYPE_ONE_SHOT, MRP_TIMER_ENUM_MRP_IN_POLL_SEND, (MRP_TIMER_CBF_FCT_PTR_TYPE)mrp_in_timer_cbf_inPoll_send);
        timerIdx = pDBin->TimerIdInPollsend % MRP_MAX_NO_OF_TIMER_IDS_PER_INTERFACE;
        MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_init[inInst=%d]: timer_get called for TimerIdInPollsend: id=%d, idx=%d", i, pDBin->TimerIdInPollsend, timerIdx);

        pDBin->TimerIdInLnkChgDownSend = mrp_sys_timer_get (pChSys, MRP_TIMER_POOL_IN_INSTANCE, i, LSA_TIMER_TYPE_ONE_SHOT, MRP_TIMER_ENUM_MRP_IN_LNK_CHG_DOWN_SEND, (MRP_TIMER_CBF_FCT_PTR_TYPE)mrp_in_timer_cbf_inLnkChg_down_send);
        timerIdx = pDBin->TimerIdInLnkChgDownSend % MRP_MAX_NO_OF_TIMER_IDS_PER_INTERFACE;
        MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_init[inInst=%d]: timer_get called for TimerIdInLnkChgDownSend: id=%d, idx=%d", i, pDBin->TimerIdInLnkChgDownSend, timerIdx);
        
        pDBin->TimerIdInLnkChgUpSend = mrp_sys_timer_get (pChSys, MRP_TIMER_POOL_IN_INSTANCE, i, LSA_TIMER_TYPE_ONE_SHOT, MRP_TIMER_ENUM_MRP_IN_LNK_CHG_UP_SEND, (MRP_TIMER_CBF_FCT_PTR_TYPE)mrp_in_timer_cbf_inLnkChg_up_send);
        timerIdx = pDBin->TimerIdInLnkChgUpSend % MRP_MAX_NO_OF_TIMER_IDS_PER_INTERFACE;
        MRP_PROGRAM_TRACE_03 (traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_init[inInst=%d]: timer_get called for TimerIdInLnkChgUpSend: id=%d, idx=%d", i, pDBin->TimerIdInLnkChgUpSend, timerIdx);

        LSA_UNUSED_ARG(timerIdx);   /* hhr: to please lint in case traces are disabled. */

      }
    }
    else
    {
      MRP_PROGRAM_TRACE_00 (traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "mrp_in_init: NO mrp interconnection instance initialized");
    }
  }
}


/*===========================================================================
* FUNCTION : mrp_in_deinit
*----------------------------------------------------------------------------
* PURPOSE  : uninitialize mrp interconnection
*----------------------------------------------------------------------------
* RETURNS  : MRP_RET_OK
*----------------------------------------------------------------------------
* INPUTS   : pChSys           = channel reference
*            mrpMaxInstance   = maximum number of MRP instances to be initialized
*            mrpInMaxInstance = maximum number of MRP interconnection instances
*                               to be initialized
* OUTPUTS  :
*----------------------------------------------------------------------------
* COMMENTS : 
*==========================================================================*/
LSA_VOID mrp_in_deinit (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance)
{
  LSA_UINT16  rc, i;
  MRP_IN_DB_TYPE *pDBin;
  MRP_IN_DB_TYPE *pDBinBase = (MRP_IN_DB_TYPE *)pChSys->pChSysDB->pMrpIn;
  MRP_PROGRAM_TRACE_02(pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE, "mrp_in_deinit entered, mrpMaxInstance=%d, mrpInMaxInstance=%d", mrpMaxInstance, mrpInMaxInstance);

  MRP_ASSERT (mrpInMaxInstance <= EDD_CFG_MAX_MRP_IN_INSTANCE_CNT);

  if (mrpMaxInstance)
  {
    if (mrpInMaxInstance)
    {
      for (i = 0; i < mrpInMaxInstance; i++)
      {
        pDBin = &pDBinBase[i];

        mrp_sys_timer_free (pDBin->TimerIdInRstppTCsend);

        mrp_sys_timer_free (pDBin->TimerIdInTCsend);
        mrp_sys_timer_free (pDBin->TimerIdInPollsend);
        mrp_sys_timer_free (pDBin->TimerIdInLnkChgDownSend);
        mrp_sys_timer_free (pDBin->TimerIdInLnkChgUpSend);
      }

      MRP_FREE_LOCAL_MEM (&rc, (MRP_LOCAL_MEM_PTR_TYPE)pDBinBase);
      MRP_FATAL1 (rc == LSA_RET_OK);
      pChSys->pChSysDB->pMrpIn = LSA_NULL;
    }
  }
  else
  {
    MRP_PROGRAM_TRACE_00 (pChSys->pChSysDB->traceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "mrp_in_deinit: NO mrp interconnection instance to be uninitialized");
  }
}



