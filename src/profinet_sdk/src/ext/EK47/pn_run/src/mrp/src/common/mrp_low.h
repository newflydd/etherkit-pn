#ifndef MRP_LOW_H
#define MRP_LOW_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

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
/*  F i l e               &F: mrp_low.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Lower component interface                                                */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef MRP_MESSAGE
/*  2008-01-07  mk    new nrt cancel type. blocking wait removed.            */
/*                    mgr_snd_req new. set/reset stdby merged to cfgStdby.   */
/*  2008-06-06  mk    port params edded to mrp_ll_fdb_cfg.                   */     
/*                    g_MrpllReqTbl_hsr_set_fdb_ard added.                   */
/*  2008-06-11  mk    topoCtrl added.                                        */
/*  2008-06-30  mk    new: mrp_edd_force_link_up.                            */
/*  2008-10-20  mk    edd_wait_time removed.                                 */
/*  2009-07-02  ds    include hierarchy changed, timerSwiBlocking discarded  */
/*                    performance measurement traces introduced              */
/*  2009-10-15  ds    cleanups for LL services (fwdCtrl)                     */
/*                    new struct MRP_FWD_CTRL_DATA_TYPE                      */
/*  2009-10-20  ds    cleanups for LL services (MC enable/disable)           */
/*                    new struct MRP_MC_MODE_DATA_TYPE                       */
/*  2009-10-22  ds    cleanups for LL services (OHA topo ctrl)               */
/*  2010-05-25  ds    new OHA upper channel definitions                      */
/*  2010-07-19  ds    new feature MRP MIB implemented according to           */
/*                    IEC 62439-2                                            */
/*  2011-01-14  ds    N-IF: rework of burst DB, user req map and various     */
/*                    variable names                                         */
/*  2012-04-04  ds    introduction of N MRP instances per interface          */
/*  2013-11-28  mk    RQ827954: erroneous connected ring line detection      */
/*  2016-04-19  mk    TFS1514074::increase rqb count for hrpmrg/stdby        */
/*  2017-06-28  ds    plugin 3 (MRP_RED_COUPLING_ENA) introduced             */
/*  2017-10-05  ds    plugin 3 (MRP_RED_COUPLING_ENA removed                 */
/*  2017-11-21  ds    introduction of MRP interconnection                    */
/*  2018-10-19  hh    introduction of N MRP interconnection instances        */
/*                                                                           */
#endif
/*****************************************************************************/


/*---------------------------------------------------------------------------*/
/*                                defines                                    */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*  if defined then link change will be delayed.                             */
/*---------------------------------------------------------------------------*/
#define   MRP_CFG_DELAYED_LINK_UP

/*---------------------------------------------------------------------------*/
/*  MRP lower layer send rqb threshold define                                */
/*---------------------------------------------------------------------------*/
#define MRP_EDD_GET_SND_RQB_NO_THRESHOLD 0xFFFFFFFFU

/*---------------------------------------------------------------------------*/
/*                           (callback) tables                               */
/*---------------------------------------------------------------------------*/
# define MRP_LL_RCV_CBF_MRP_IN_CNT 1  /* max. 2 CBF without MRP_IN rcv, max. 3 CBF with MRP_IN rcv */


#ifdef MRP_CFG_PLUGIN_2_HSR
# define MRP_LL_RCV_CBF_HSR_CNT    2  /* max. 4 CBF without MRP_IN rcv, max. 5 CBF with MRP_IN rcv */
#else
# define MRP_LL_RCV_CBF_HSR_CNT    0
#endif


/* number of max. allowed RCV callback functions */
#define MRP_LL_RCV_CBF_TBL_SIZE    2 + MRP_LL_RCV_CBF_HSR_CNT + MRP_LL_RCV_CBF_MRP_IN_CNT

/* #undef MRP_CFG_PLUGIN_2_HSR:  max. 2 CBF without MRP_IN rcv, max. 3 CBF with MRP_IN rcv */
/* #define MRP_CFG_PLUGIN_2_HSR: max. 4 CBF without MRP_IN rcv, max. 5 CBF with MRP_IN rcv */



#define MRP_LL_LINK_CHG_CBF_TBL_SIZE  (  1 /* rc/mgr */\
                                       + 1 /* stdby */   \
                                       + 1 /* mrc */ \
                                       + 1 /* mrm */ \
                                       + 1 /* MIM */ \
                                       + 1 /* MIC */)


/*---------------------------------------------------------------------------*/
/*  MRP performance measurement defines                                      */
/*---------------------------------------------------------------------------*/
#ifdef MRP_CFG_PERF_TRACE

#include <config.h>
#include "eddi_inc.h"
#define ERTEC_TIME (*(volatile LSA_INT32 * const) (IRTE_SWITCH_START+0x00011414))

#endif

/*---------------------------------------------------------------------------*/
/*  MRP base defines (MRC)                                                   */
/*---------------------------------------------------------------------------*/
#define MRP_LL_SND_RQB_ENUM_NSM_WRK_LNK_CHG    \
  MRP_LL_SND_RQB_ENUM_NSM_WRK_LNK_CHG_BOT = 0  \
  ,MRP_LL_SND_RQB_ENUM_NSM_WRK_LNK_CHG_TOP

#define MRP_LL_SND_RES_NSM_WRK_LNK_CHG         \
  3                                            \
  ,MRP_LL_SND_RQB_ENUM_NSM_WRK_LNK_CHG_TOP


/*---------------------------------------------------------------------------*/
/*  MRP_IN base defines (MRP_RSTP+)                                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*  IN_WRK MC address: 01:15:4e:00:00:04                                     */
/*---------------------------------------------------------------------------*/
#define  MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_ORIG  \
  MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_ORIG_BOT     \
  ,MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_ORIG_TOP

#define  MRP_LL_SND_RES_MRP_IN_WRK_TC_ORIG       \
  6,                                             \
  MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_ORIG_TOP



/*---------------------------------------------------------------------------*/
/*  MRP_IN base defines (MRP interconnection)                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*  IN_TST MC address: 01:15:4e:00:00:03                                     */
/*---------------------------------------------------------------------------*/

/* Nothing needed because we don't send any IN_TST frames. */

/*---------------------------------------------------------------------------*/
/*  IN_WRK MC address: 01:15:4e:00:00:04                                     */
/*---------------------------------------------------------------------------*/

/* enum for MRP_IN_WRK */
#define MRP_LL_SND_RQB_ENUM_MRP_IN_WRK        \
  MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_BOT       \
  ,MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_TOP      \
  ,MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_POLL_BOT    \
  ,MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_POLL_TOP    \
  ,MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_LNK_CHG_BOT \
  ,MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_LNK_CHG_TOP

/* number of resources for MRP_IN_WRK */
#define  MRP_LL_SND_RES_MRP_IN_WRK          \
  6,                                        \
  MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_TOP,    \
  8,                                        \
  MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_POLL_TOP,  \
  8,                                        \
  MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_LNK_CHG_TOP

/*---------------------------------------------------------------------------*/
/*  IN_TRF MC address: 01:15:4e:00:00:05                                     */
/*---------------------------------------------------------------------------*/

/* enum for MRP_IN_TRF */
#define MRP_LL_SND_RQB_ENUM_MRP_IN_TRF        \
  MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_TC_BOT       \
  ,MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_TC_TOP      \
  ,MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_POLL_BOT    \
  ,MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_POLL_TOP    \
  ,MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_LNK_CHG_BOT \
  ,MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_LNK_CHG_TOP

/* number of resources for MRP_IN_TRF */
#define  MRP_LL_SND_RES_MRP_IN_TRF          \
  3,                                        \
  MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_TC_TOP,    \
  4,                                        \
  MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_POLL_TOP,  \
  4,                                        \
  MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_LNK_CHG_TOP


/*---------------------------------------------------------------------------*/
/*  MRP plugin defines                                                       */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*  Plugin 0 (MRP manager)                                                   */
/*---------------------------------------------------------------------------*/

#ifdef MRP_CFG_PLUGIN_0_MRM

#define  MRP_LL_SND_RQB_ENUM_PLUGIN_MRM   \
  MRP_LL_SND_RQB_ENUM_NSM_TST_BOT         \
  ,MRP_LL_SND_RQB_ENUM_NSM_TST_TOP        \
  ,MRP_LL_SND_RQB_ENUM_NSM_TPL_CHG_BOT    \
  ,MRP_LL_SND_RQB_ENUM_NSM_TPL_CHG_TOP

#define  MRP_LL_SND_RES_PLUGIN_MRM        \
  6,                                      \
  MRP_LL_SND_RQB_ENUM_NSM_TST_TOP,        \
  8,                                      \
  MRP_LL_SND_RQB_ENUM_NSM_TPL_CHG_TOP
#endif /* MRP_CFG_PLUGIN_0_MRM */


/*---------------------------------------------------------------------------*/
/*  Plugin 1 (Siemens MRP automanager)                                       */
/*---------------------------------------------------------------------------*/

#ifdef MRP_CFG_PLUGIN_1_MRM_EXT

#define  MRP_LL_SND_RQB_ENUM_PLUGIN_MRM_EXT  \
  MRP_LL_SND_RQB_ENUM_NSM_EXT_BOT            \
  ,MRP_LL_SND_RQB_ENUM_NSM_EXT_TOP           \

#define  MRP_LL_SND_RES_PLUGIN_MRM_EXT       \
  6,                                         \
  MRP_LL_SND_RQB_ENUM_NSM_EXT_TOP

#endif /* MRP_CFG_PLUGIN_1_MRM_EXT */


/*---------------------------------------------------------------------------*/
/*  Plugin 2 (HSR/Stdby)                                                     */
/*---------------------------------------------------------------------------*/

#ifdef MRP_CFG_PLUGIN_2_HSR

#define MRP_LL_SND_RQB_ENUM_PLUGIN_HSR    \
  MRP_LL_SND_RQB_ENUM_TPLG_CHG_BOT        \
  ,MRP_LL_SND_RQB_ENUM_TPLG_CHG_TOP       \
  ,MRP_LL_SND_RQB_ENUM_TST_BOT            \
  ,MRP_LL_SND_RQB_ENUM_TST_TOP            \
  ,MRP_LL_SND_RQB_ENUM_STBY_BOT           \
  ,MRP_LL_SND_RQB_ENUM_STBY_TOP           \
  ,MRP_LL_SND_RQB_ENUM_TPLG_CHG_STDBY_BOT \
  ,MRP_LL_SND_RQB_ENUM_TPLG_CHG_STDBY_TOP

#define MRP_LL_SND_RES_PLUGIN_HSR   \
  6,                                \
  MRP_LL_SND_RQB_ENUM_TPLG_CHG_TOP, \
  6,                                \
  MRP_LL_SND_RQB_ENUM_TST_TOP,      \
  6,                                \
  MRP_LL_SND_RQB_ENUM_STBY_TOP,     \
  6,                                \
  MRP_LL_SND_RQB_ENUM_TPLG_CHG_STDBY_TOP

#endif /* MRP_CFG_PLUGIN_2_HSR */



/*---------------------------------------------------------------------------*/
/*                                  types                                    */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* send rqb enum types (= array index to snd rqb max. resource array)        */
/*---------------------------------------------------------------------------*/
typedef enum _MRP_LL_SND_RQB_ENUM_T
{
  MRP_LL_SND_RQB_ENUM_TBL_START = 0

  /* MRP client: MRP_LinkChange frames */
  ,MRP_LL_SND_RQB_ENUM_NSM_WRK_LNK_CHG  /* MRP_LL_SND_RQB_ENUM_NSM_WRK_LNK_CHG_BOT = 0, MRP_LL_SND_RQB_ENUM_NSM_WRK_LNK_CHG_TOP */

  /* IN_ORIGINATOR (RSTP+) */
  ,MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_ORIG   /* MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_ORIG_BOT, MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_ORIG_TOP */

  /* MRP interconnection: WRK frames */  
  ,MRP_LL_SND_RQB_ENUM_MRP_IN_WRK           /* MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_BOT, MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_TOP           */
                                            /* MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_POLL_BOT, MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_POLL_TOP       */
                                            /* MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_LNK_CHG_BOT, MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_LNK_CHG_TOP */

  /* MRP interconnection: TRF frames */
  ,MRP_LL_SND_RQB_ENUM_MRP_IN_TRF           /* MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_TC_BOT, MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_TC_TOP           */
                                            /* MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_POLL_BOT, MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_POLL_TOP       */
                                            /* MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_LNK_CHG_BOT, MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_LNK_CHG_TOP */


#ifdef MRP_CFG_PLUGIN_0_MRM
  ,MRP_LL_SND_RQB_ENUM_PLUGIN_MRM           /* MRP_LL_SND_RQB_ENUM_NSM_TST_BOT, MRP_LL_SND_RQB_ENUM_NSM_TST_TOP                       */
                                            /* MRP_LL_SND_RQB_ENUM_NSM_TPL_CHG_BOT, MRP_LL_SND_RQB_ENUM_NSM_TPL_CHG_TOP               */

#endif
#ifdef MRP_CFG_PLUGIN_1_MRM_EXT
  , MRP_LL_SND_RQB_ENUM_PLUGIN_MRM_EXT      /* MRP_LL_SND_RQB_ENUM_NSM_EXT_BOT, MRP_LL_SND_RQB_ENUM_NSM_EXT_TOP                       */
#endif
#ifdef MRP_CFG_PLUGIN_2_HSR
  ,MRP_LL_SND_RQB_ENUM_PLUGIN_HSR
#endif
  ,MRP_LL_SND_RQB_ENUM_TBL_END
}MRP_LL_SND_RQB_ENUM_T;

#define MRP_LL_SND_RQB_ENUM_IN_TYPES_START_OFFSET   (LSA_UINT16)MRP_LL_SND_RQB_ENUM_MRP_IN_WRK_TC_ORIG_BOT
#define MRP_LL_SND_RQB_ENUM_IN_TYPES_END_OFFSET     (LSA_UINT16)MRP_LL_SND_RQB_ENUM_MRP_IN_TRF_LNK_CHG_TOP
#define MRP_LL_SND_RQB_ENUM_IN_TYPES_TBL_END        (LSA_UINT16)((MRP_LL_SND_RQB_ENUM_IN_TYPES_END_OFFSET - MRP_LL_SND_RQB_ENUM_IN_TYPES_START_OFFSET) + 1)

/*---------------------------------------------------------------------------*/
/* completion status of burst requests to LL EDD                             */
/*---------------------------------------------------------------------------*/
typedef enum _MRP_LL_ENUM_BURST_TRACE_TYPE
{
   MRP_LL_ENUM_BURST_TRACE_START  = '0'
  ,MRP_LL_ENUM_BURST_TRACE_FINISH = '1'
  ,MRP_LL_ENUM_BURST_TRACE_RESET  = 'R'
  ,MRP_LL_ENUM_BURST_TRACE_QUEUED = 'q'
  ,MRP_LL_ENUM_BURST_TRACE_DENIED = 'D'
  ,MRP_LL_ENUM_BURST_TRACE_WORKING= 'W'
}MRP_LL_ENUM_BURST_TRACE_TYPE;

/*---------------------------------------------------------------------------*/
/* types for different EDD services used by MRP                              */
/*---------------------------------------------------------------------------*/

typedef union _MRP_EDD_LOWER_USED_DATA_TYPE
{
  MRP_EDD_RQB_OPEN_CHANNEL_TYPE            ch_op;
  MRP_EDD_RQB_NRT_CANCEL_TYPE              nrt_cancel;
  MRP_EDD_RQB_GET_PARAMS_TYPE              get_params;
  MRP_EDD_RQB_NRT_SEND_TYPE                snd;
  MRP_EDD_RQB_NRT_RECV_TYPE                rcv;
  MRP_EDD_RQB_LINK_STATUS_IND_PROVIDE_TYPE link_provide;
  MRP_EDD_RQB_MULTICAST_TYPE               multicast;
  MRP_EDD_RQB_MULTICAST_FWD_CTRL_TYPE      mcast_fwd_ctrl;
  MRP_EDD_RQB_SWI_FLUSH_FILTERING_DB_TYPE  flush_db;
  MRP_EDD_RQB_SWI_SET_PORT_STATE_TYPE      set_port_state;
} MRP_EDD_LOWER_USED_DATA_TYPE;

typedef MRP_EDD_LOWER_USED_DATA_TYPE *MRP_EDD_LOWER_USED_DATA_PTR_TYPE;

/*---------------------------------------------------------------------------*/
/*                            private types                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* enum type for EDD requests triggered by MRP                               */
/*---------------------------------------------------------------------------*/
typedef enum _MRP_LL_REQ_RQB_ENUM_T
{
   MRP_LL_REQ_RQB_ENUM_MULTICAST
  ,MRP_LL_REQ_RQB_ENUM_MULTICAST_FWD_CTRL
  ,MRP_LL_REQ_RQB_ENUM_FLUSH_FILTERING_DB
  ,MRP_LL_REQ_RQB_ENUM_PORT_STATE
  ,MRP_LL_REQ_RQB_ENUM_TBL_END
}MRP_LL_REQ_RQB_ENUM_T; 

/*---------------------------------------------------------------------------*/
/* enum type for shadow edd rqb's used to store temp values                  */
/*---------------------------------------------------------------------------*/
typedef enum _MRP_LL_SREQ_RQB_ENUM_T
{
   MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_START
  ,MRP_LL_SREQ_RQB_ENUM_MULTICAST_WRK = MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_START
  ,MRP_LL_SREQ_RQB_ENUM_MULTICAST_TST

  ,MRP_LL_SREQ_RQB_ENUM_MULTICAST_HSYNC

  ,MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_TST
  ,MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_WRK
  ,MRP_LL_SREQ_RQB_ENUM_MULTICAST_IN_TRF
#ifdef MRP_CFG_PLUGIN_2_HSR
  ,MRP_LL_SREQ_RQB_ENUM_MULTICAST_HSR_TPLG_CHG
  ,MRP_LL_SREQ_RQB_ENUM_MULTICAST_HSR_TST     
  ,MRP_LL_SREQ_RQB_ENUM_MULTICAST_STBY_PARTNER
  ,MRP_LL_SREQ_RQB_ENUM_MULTICAST_STBY_COM    
#endif
  ,MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_END
  
  ,MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_TBL_START
  ,MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_WRK = MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_TBL_START
  ,MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_TST

  ,MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_HSYNC

  ,MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_TST
  ,MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_WRK
  ,MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_IN_TRF

#ifdef MRP_CFG_PLUGIN_2_HSR
  ,MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_HSR_TPLG_CHG
  ,MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_HSR_TST     
  ,MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_STBY_PARTNER
  ,MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_STBY_COM    
#endif
  ,MRP_LL_SREQ_RQB_ENUM_MCAST_FWD_CTRL_TBL_END
  
  ,MRP_LL_SREQ_RQB_ENUM_FLUSH_FILTERING_DB_TBL_START
  ,MRP_LL_SREQ_RQB_ENUM_FLUSH_FILTERING_DB = MRP_LL_SREQ_RQB_ENUM_FLUSH_FILTERING_DB_TBL_START
#ifdef MRP_CFG_PLUGIN_2_HSR
  ,MRP_LL_SREQ_RQB_ENUM_FLUSH_FILTERING_DB_HSR
#endif
  ,MRP_LL_SREQ_RQB_ENUM_FLUSH_FILTERING_DB_TBL_END
  
  /* */
  ,MRP_LL_SREQ_RQB_ENUM_SET_PS_TBL_START
  ,MRP_LL_SREQ_RQB_ENUM_SET_PS_TPLG_CHG_START = MRP_LL_SREQ_RQB_ENUM_SET_PS_TBL_START
  ,MRP_LL_SREQ_RQB_ENUM_SET_PS_DEINIT
  ,MRP_LL_SREQ_RQB_ENUM_SET_PS_SHADOW
  ,MRP_LL_SREQ_RQB_ENUM_SET_PS_DEINIT_MRPI
#ifdef MRP_CFG_PLUGIN_2_HSR
  ,MRP_LL_SREQ_RQB_ENUM_SET_PS_HSR_TPLG_CHG_START
  ,MRP_LL_SREQ_RQB_ENUM_SET_PS_HSR
  ,MRP_LL_SREQ_RQB_ENUM_SET_PS_HSR_ECORI_DE
  ,MRP_LL_SREQ_RQB_ENUM_SET_PS_HSR_DEINIT
#endif
  ,MRP_LL_SREQ_RQB_ENUM_SET_PS_TBL_END
  
  ,MRP_LL_SREQ_RQB_ENUM_TBL_END
}MRP_LL_SREQ_RQB_ENUM_T; 

/*---------------------------------------------------------------------------*/
/* port state enum type                                                      */
/*---------------------------------------------------------------------------*/
/* nsm */
typedef enum _MRP_PORT_STATE_TYPE
{
   MRP_PORT_STATE_FORWARD
  ,MRP_PORT_STATE_BLOCKED    
  ,MRP_PORT_STATE_DISABLED
}MRP_PORT_STATE_TYPE;


/*---------------------------------------------------------------------------*/
/* Defines for CheckUUID settings                                            */
/*---------------------------------------------------------------------------*/
#define MRP_CHECK_UUID_ON    1
#define MRP_CHECK_UUID_OFF   0

/*---------------------------------------------------------------------------*/
/* Defines for set defaults settings                                         */
/*---------------------------------------------------------------------------*/
#define MRP_SET_DEFAULTS_ON    1
#define MRP_SET_DEFAULTS_OFF   0

/*---------------------------------------------------------------------------*/
/* Defines for soft start behavior                                           */
/*---------------------------------------------------------------------------*/
#define MRP_SET_SOFT_START_ON    1
#define MRP_SET_SOFT_START_OFF   0

/*---------------------------------------------------------------------------*/
/*   CBF typedef for MIM                                                    */
/*---------------------------------------------------------------------------*/
/* CBF type for MRP indications of MRP ring events to the MIM state machine */
typedef LSA_VOID   LSA_FCT_PTR (/*ATTR*/, MRP_IN_MIM_MRP_RING_CHG_FCT_PTR_TYPE)(const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT16 inInstance);


/*---------------------------------------------------------------------------*/
/* MRC params structure                                                      */
/*---------------------------------------------------------------------------*/
typedef struct _MRP_MRC_PARAMS_TYPE
{
  LSA_UINT16 RPort_1;
  LSA_UINT16 RPort_2;
  LSA_UINT16 PRM_RPort;
  LSA_UINT16 SEC_RPort;
  LSA_UINT16 Mode;
  LSA_UINT8  DomainUUID[16];
  LSA_UINT16 LNKdownT_ms;
  LSA_UINT16 LNKupT_ms;
  LSA_UINT16 LNKNRmax;
  LSA_UINT8  SetDefaults;
  LSA_UINT8  CheckUUID;
  LSA_VOID   *pMrcInTxInfo;
  MRP_IN_MIM_MRP_RING_CHG_FCT_PTR_TYPE *ppMrpInRingChgCbf;
}MRP_MRC_PARAMS_TYPE;

/*---------------------------------------------------------------------------*/
/* MRM params structure                                                      */
/*---------------------------------------------------------------------------*/
/* interface */
typedef struct _MRP_MRM_PARAMS_TYPE
{
  MRP_ADMIN_ROLE_ENUM_T adminRole;    /* admin role of MRM */
  MRP_OPER_ROLE_ENUM_T  operRole;     /* operational role of MRM */
  MRP_MRM_EDITION_ENUM_T edition;     /* edition of IEC62439-2, the MRM/MRA shall use for frame layout */
  LSA_UINT16 RPort_1;
  LSA_UINT16 RPort_2;
  LSA_UINT16 PRM_RPort;
  LSA_UINT16 SEC_RPort;
  LSA_UINT16 Not_rcv_RPort;
  LSA_UINT16 Mode; /* MRP_OPERATION_OFF, MRP_OPERATION_ON */
  LSA_UINT8  SetDefaults;
  LSA_UINT8  SoftStart;
  LSA_UINT8  DomainUUID[16];
  LSA_UINT16 Prio;
  LSA_UINT16 TOPchgT_10ms;
  LSA_UINT16 TOPNRmax;
  LSA_UINT16 TSTshortTms;
  LSA_UINT16 TSTdefaultTms;
  LSA_UINT16 TSTNRmax;
  LSA_UINT16 Transitions;
  LSA_UINT16 NewMgrPrio;
  LSA_UINT8  NewMgr[6];
  LSA_UINT16 NewMgrCnt;
  LSA_UINT32 DelayResetCnt;
  LSA_UINT32 MaxDelay;
  LSA_UINT32 MinDelay;
#ifdef MRP_CFG_PLUGIN_1_MRM_EXT
  LSA_UINT16 MONdefaultT_ms;
  LSA_UINT16 MONNRmax;
#endif
  LSA_UINT8  CheckUUID;
  LSA_VOID   *pMrmInTxInfo;
  MRP_IN_MIM_MRP_RING_CHG_FCT_PTR_TYPE *ppMrpInRingChgCbf;
}MRP_MRM_PARAMS_TYPE;


/*-------------------------------------------------------------------------------------------*/
/* additional link info structure to indicate if an additional link check protocol is active */
/*-------------------------------------------------------------------------------------------*/

typedef struct _MRP_LINK_ADD_INFO
{
  LSA_UINT8 AddCheckEnabled;
  LSA_UINT8 ChangeReason;
} MRP_LINK_ADD_INFO;


/*---------------------------------------------------------------------------*/
/* CBF types                                                                 */
/*---------------------------------------------------------------------------*/
typedef LSA_UINT8  LSA_FCT_PTR (/*ATTR*/, MRP_LL_RCV_FCT_PTR_TYPE)(const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT8 *pBuffer, LSA_UINT16 userDataOffset, LSA_UINT16 len, LSA_UINT16 port);
typedef LSA_UINT8  LSA_FCT_PTR (/*ATTR*/, MRP_LL_LINK_CHG_FCT_PTR_TYPE)(const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT16 port, MRP_LINK_ADD_INFO addInfo, LSA_UINT8 isLinkUp);
typedef LSA_VOID   LSA_FCT_PTR (/*ATTR*/, MRP_LL_TPLG_CHG_FINISH_FCT_PTR_TYPE)(const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance);

typedef LSA_VOID   LSA_FCT_PTR (/*ATTR*/, MRP_LL_RQ_FCT_PTR_TYPE)(MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum);

/*---------------------------------------------------------------------------*/
/* EDD request mapping table                                                 */
/*---------------------------------------------------------------------------*/
typedef struct _MRP_LL_REQ_TBL_T
{
  MRP_LL_RQ_FCT_PTR_TYPE cbf;
  MRP_LL_SREQ_RQB_ENUM_T rqbNum;
  EDD_SERVICE            service;
}MRP_LL_REQ_TBL_T;

/*---------------------------------------------------------------------------*/
/* burst database table                                                      */
/*---------------------------------------------------------------------------*/
typedef struct _MRP_LL_BURST_DB_T
{
  const MRP_LL_REQ_TBL_T     *pReqTblBegin;
  struct _MRP_LL_BURST_DB_T  *pNext;
  
  LSA_UINT8                  queued;
  LSA_UINT8                  reset;
  LSA_UINT16                 pInstance[2];                          /* id of this MRP interconnection instance            */
                                                                    /* pInstance[0]: related MRP instance                 */
                                                                    /* pInstance[1]: related MRP interconnection instance */
}MRP_LL_BURST_DB_T;


/*---------------------------------------------------------------------------*/
/* queue for lower layer requests                                            */
/*---------------------------------------------------------------------------*/

typedef struct _MRP_LL_QUEUE
{
  MRP_LL_BURST_DB_T               *pPendingLLburst;    /* pointer to pending lower layer burst */
  const MRP_LL_REQ_TBL_T          *pPendingLLreq;      /* pointer to pending lower layer request */
} MRP_LL_QUEUE;

/*---------------------------------------------------------------------------*/
/*  forward control structure                                                */
/*---------------------------------------------------------------------------*/
typedef struct _MRP_FWD_CTRL_DATA_TYPE
{
  MRP_EDD_LOWER_RQB_PTR_TYPE      pTestRqb;
  MRP_EDD_LOWER_RQB_PTR_TYPE      pWorkRqb;
  MRP_EDD_LOWER_RQB_PTR_TYPE      pTrfRqb;
  const MRP_LL_REQ_TBL_T          *pLLreqType;
  LSA_UINT16                      macAddrPrio;
}MRP_FWD_CTRL_DATA_TYPE;

/*---------------------------------------------------------------------------*/
/* multicast mode structure                                                  */
/*---------------------------------------------------------------------------*/
typedef struct _MRP_MC_MODE_DATA_TYPE
{
  MRP_EDD_LOWER_RQB_PTR_TYPE      pTestRqb;
  MRP_EDD_LOWER_RQB_PTR_TYPE      pWorkRqb;
  MRP_EDD_LOWER_RQB_PTR_TYPE      pTrfRqb;
  LSA_UINT8                       modeTest;
  LSA_UINT8                       modeWork;
  LSA_UINT8                       modeTrf;
}MRP_MC_MODE_DATA_TYPE;


/*---------------------------------------------------------------------------*/
/* link table structure                                                      */
/*---------------------------------------------------------------------------*/
typedef struct _MRP_LINK_TBL_TYPE
{
  LSA_UINT16        timer_index_up;
  LSA_UINT16        timer_upTimeTcnt;

  LSA_UINT8         state;
  LSA_UINT8         preState;
  LSA_UINT8         currentState;

  MRP_LINK_ADD_INFO addLinkInfo;
}MRP_LINK_TBL_TYPE;

/*---------------------------------------------------------------------------*/
/* (optional) performance measurement type                                   */
/*---------------------------------------------------------------------------*/

#define MRP_SET_PERF_FLAG_ON        1
#define MRP_SET_PERF_FLAG_OFF       0

typedef struct _MRP_PERF_MEASUREMENT_TYPE
{
  LSA_UINT32 startTime;
  LSA_UINT32 endTime;
  LSA_UINT32 timeDiff;
  LSA_UINT8  perfFlag;
}MRP_PERF_MEASUREMENT_TYPE;


/*---------------------------------------------------------------------------*/
/* EDD instance related database                                             */
/*---------------------------------------------------------------------------*/

typedef struct _MRP_EDD_SEND_RQB_DATA_SET
{
  MRP_EDD_LOWER_RQB_PTR_TYPE      pDBsendRqb;
  LSA_UINT16                      sendRqbCnt;
  LSA_UINT16                      sendRqbMissCnt;
  LSA_UINT16                      sendRqbMaxCnt;
}MRP_EDD_SEND_RQB_DATA_SET;

typedef struct _MRP_EDD_INST_DATA
{
  /* burst handling */
  /* ----- */
  MRP_LL_BURST_DB_T               *pDBllBurst;         /* pointer to lower layer burst database */

  LSA_UINT16                      initCnt;
  /* ring ports */
  LSA_UINT16                      RPort_1;
  LSA_UINT16                      RPort_2;

  /* instance timer */
  LSA_UINT16                      TimerIdTplgChg;
  
  MRP_EDD_LOWER_RQB_PTR_TYPE      pDBshadowRqb[MRP_LL_SREQ_RQB_ENUM_TBL_END];

  /* MRP Instance send recoureces */
  MRP_EDD_SEND_RQB_DATA_SET       sendRqbInstData[MRP_LL_SND_RQB_ENUM_TBL_END];

  /* MRP interconnection Instance send recoureces seperate for each inInstance! */
  /* To avoid compiler errors (EDD_CFG_MAX_MRP_IN_INSTANCE_CNT + 1): in case of EDD_CFG_MAX_MRP_IN_INSTANCE_CNT = 0 the array has at least the size of 1! Still all internal checks are done with EDD_CFG_MAX_MRP_IN_INSTANCE_CNT. */
  MRP_EDD_SEND_RQB_DATA_SET       sendRqbInInstData[EDD_CFG_MAX_MRP_IN_INSTANCE_CNT + 1][MRP_LL_SND_RQB_ENUM_IN_TYPES_TBL_END];

  /* linkChg */
  MRP_LL_LINK_CHG_FCT_PTR_TYPE    pLinkChgCbfFctTbl[MRP_LL_LINK_CHG_CBF_TBL_SIZE];
  /* rcv */
  MRP_LL_RCV_FCT_PTR_TYPE         pRcvCbfFctTbl[MRP_LL_RCV_CBF_TBL_SIZE];
  
  /* tplgChg */
  LSA_UINT8                       TplgChgRunning;
  MRP_LL_TPLG_CHG_FINISH_FCT_PTR_TYPE  TplgChgFinishFct;                  /* only for HSR */

  /* performance measurement (optional) */
#ifdef MRP_CFG_PERF_TRACE
MRP_PERF_MEASUREMENT_TYPE         perfCapture;
#endif

}MRP_EDD_INST_DATA;

/*---------------------------------------------------------------------------*/
/* typedef of lower layer interface DB                                       */
/*---------------------------------------------------------------------------*/
typedef struct _MRP_IF_DB_TYPE MRP_IF_DB_TYPE;


/*---------------------------------------------------------------------------*/
/* EDD database                                                              */
/*---------------------------------------------------------------------------*/
typedef struct _MRP_EDD_DB_TYPE
{

  /* lower layer request queue */
  MRP_LL_QUEUE                    llQueue;
  
  /* edd rqb tbl */
  MRP_EDD_LOWER_RQB_PTR_TYPE      pDBlowerReqRqb[MRP_LL_REQ_RQB_ENUM_TBL_END];
  MRP_EDD_UPPER_MULTICAST_PTR_TYPE pCurrentMCsetting[MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_END - MRP_LL_SREQ_RQB_ENUM_MULTICAST_TBL_START];  /* current active multicast settings for any MC address */
  /* burst handling */
  /* -------------- */
  LSA_UINT16                      llBurstMaxCnt;       /* max. number of applicable bursts for one instance */

  /* user channel references */
  MRP_CHANNEL_TYPE_USER           pChUsr_finishUpper;
  MRP_CHANNEL_TYPE_USER           pChUsr_finishClose;
  
  MRP_CHANNEL_TYPE_USER           pChUsr_prmCommit;
  MRP_UPPER_RQB_PTR_TYPE          pRQB_prmCommit;

  /* variables */
  /* --------- */  
  MRP_LINK_TBL_TYPE               *pLinkData;
  
  /* snd */
  LSA_UINT8                       **ppPortMacAddr;
  LSA_UINT16                      PortID4MacAddr;  
  
  /* channel attributes */
  MRP_CHANNEL_TYPE_SYSTEM         pChSys;
  MRP_CHANNEL_TYPE_USER           pChUsr[MRP_MAX_USER_CHANNELS_PER_IF]; /* channel reference to associated common user */
                                                                         /* channels, needed for mrp_ll_event_handler   */
  MRP_CHANNEL_TYPE_USER           pChPrivUsr;                            /* channel reference to associated private     */
                                                                         /* user channel, needed for mrp_ll_event_handler*/

  LSA_SYS_PTR_TYPE		            ll_edd_sysptr;
  
  /* interface timer */
  /* ----- */
  LSA_UINT16                      TimerIdCyclic;
  
  LSA_UINT16                      activeMrmInstCnt;
  
  /* instance related data */
  MRP_EDD_INST_DATA               *pInstData;

}MRP_EDD_DB_TYPE;

/*---------------------------------------------------------------------------*/
/* port data structure                                                       */
/*---------------------------------------------------------------------------*/

typedef struct _MRP_PORT_DATA_TYPE
{
  LSA_UINT8 moduleState;
  LSA_UINT8 rpType;
  LSA_UINT8 inPortType;
  LSA_UINT8 isModularPort;
}MRP_PORT_DATA_TYPE;


/*---------------------------------------------------------------------------*/
/* Defines for HSYNC FWD rules support                                       */
/*---------------------------------------------------------------------------*/
#define MRP_HSYNC_FWD_RULES_SUPPORTED_ON      1
#define MRP_HSYNC_FWD_RULES_SUPPORTED_OFF     0

/*---------------------------------------------------------------------------*/
/* Defines for HSYNC application                                             */
/*---------------------------------------------------------------------------*/
#define MRP_HSYNC_APPL_EXISTS_ON              1
#define MRP_HSYNC_APPL_EXISTS_OFF             0


/*---------------------------------------------------------------------------*/
/* Defines for specific FWD behavior (src port ID supported)                 */
/*---------------------------------------------------------------------------*/
#define MRP_FWD_SRC_PORT_ID_SUPPORTED_ON      1
#define MRP_FWD_SRC_PORT_ID_SUPPORTED_OFF     0


/*---------------------------------------------------------------------------*/
/* Defines for MRP interconnection FWD ctrl of MC_INCONTROL address          */
/*---------------------------------------------------------------------------*/
#define MRP_IN_FWD_RULES_SUPPORTED_ON         1
#define MRP_IN_FWD_RULES_SUPPORTED_OFF        0

/*---------------------------------------------------------------------------*/
/* Defines for MRP interconnection originator facility                       */
/*---------------------------------------------------------------------------*/
#define MRP_IN_ORIGINATOR_ON                  1
#define MRP_IN_ORIGINATOR_OFF                 0


/*---------------------------------------------------------------------------*/
/* lower layer interface DB                                                  */
/*---------------------------------------------------------------------------*/
struct _MRP_IF_DB_TYPE
{
  LSA_UINT16                      numOfPorts;                    /* number of Ethernet ports of this interface              */
  LSA_UINT32                      pnioInterfaceID;               /* PNIO interfaceID (pnioInterfaceID = eddxInterfaceID - 1)*/
  LSA_UINT8                       macAddr[6];                    /* MAC address of device                                   */
  LSA_UINT32                      hwType;                        /* Hardware Type                                           */

  LSA_UINT8                       mrpDefaultRoleInst0;           /* MRP default role for MRP base instance                  */
  LSA_UINT8                       mrpSupportedRoleInst0;         /* MRP role capability for MRP base instance               */
  LSA_UINT8                       mrpMaxInstance;                /* Number of MRP instances supported for this interface    */

  LSA_UINT8                       mraEnableLegacyMode;           /* Default value 0: MRA 2.0, value 1: MRA 1.0              */
  LSA_UINT8                       mrpSupportedMultiRole;         /* MRP role capability, if more than one MRP instance is   */
                                                                 /* provisioned for this interface                          */
  LSA_UINT8                       mrpInMaxInstance;              /* Number of MRP interconnection instances supported for this interface including RSTP+ instances    */
  LSA_UINT8                       mrpInSupportedRoleInst0;       /* MRP interconnection role capability for MRP base instance               */
  LSA_UINT8                       mrpRstpInstances;              /* Number of MRP RSTP+ instances.  */

  LSA_UINT8                       mrpInFwdRulesSupported;        /* FWD rules for MRP interconnection MC MC_INCONTROL supported             */
  LSA_UINT8                       mrpInOriginator;               /* Originator capability for MRP interconnection MC MC_INCONTROL supported */
                                                                 /* Device can be originator for MRP_InTC frames requested by an application*/
  LSA_UINT8                       eddSrcPortIDSupported;         /* port specific FDB forwarding of MC frames is not supported by any Ethernet controller             */
                                                                 /* LSA_TRUE: if srcPortID other than EDD_PORT_ID_AUTO is supported                                   */
                                                                 /* LSA_FALSE: if only srcPortID=EDD_PORT_ID_AUTO is supported for service EDD_SRV_MULTICAST_FWD_CTRL */
  LSA_UINT8                       hsyncFwdRulesSupported;        /* FWD rules for an additional proprietary high-availability feature (HSYNC) supported               */
                                                                 /* LSA_TRUE: additional FWD rules for a proprietary MC address have to be maintained by MRP          */
                                                                 /* LSA_FALSE: no additional FWD rule support by MRP                                                  */
  LSA_UINT8                       hsyncApplExists;               /* an application for an additional proprietary high-availability feature (HSYNC) exists             */
                                                                 /* LSA_TRUE: an additional proprietary application exists,                                           */
                                                                 /*           MRP has to maintain the forwarding of proprietary MC address frames to host             */
                                                                 /* LSA_FALSE: no additional proprietary application exists                                           */
  LSA_UINT8                       reserved;
  MRP_HA_ADMIN_ROLE_ENUM_T        hsyncAdminRole;                /* admin role for a proprietary high availability feature */
   
  MRP_PORT_DATA_TYPE              *pPortData;                    /* reference to the array of ports                             */
  LSA_UINT16                      *pSequenceID;                  /* reference to the array of sequenceIDs used by MRP instances */
};


/*===========================================================================*/
/*                          macros                                           */
/*===========================================================================*/

#define MRP_EDD_LOWER_GET_SYS_CHANNEL_FROM_RQB(pRQB) (MRP_CHANNEL_TYPE_SYSTEM)(MRP_EDD_LOWER_GET_USERID_PTR(pRQB))


/*===========================================================================*/
/*                          private prototyping                              */
/*===========================================================================*/

/* mrp_edd.c */
LSA_VOID mrp_edd_process_req_lower_done (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB);
LSA_VOID mrp_edd_process_open_lower_done (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB);
LSA_VOID mrp_edd_process_close_lower_done (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB);

LSA_VOID mrp_edd_req_open_ch_sys (const MRP_CHANNEL_TYPE_SYSTEM pChSys);
LSA_VOID mrp_edd_req_open_ch_sys_done (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB);
LSA_UINT16 mrp_edd_open_channel (const MRP_CHANNEL_TYPE_SYSTEM pChSys);

LSA_VOID mrp_edd_req_close_ch_sys (const MRP_CHANNEL_TYPE_SYSTEM pChSys);
LSA_VOID mrp_edd_req_close_ch_sys_done (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB);
LSA_VOID mrp_edd_req_nrt_cancel (const MRP_CHANNEL_TYPE_SYSTEM pChSys);
LSA_VOID mrp_edd_req_nrt_cancel_done (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB);
LSA_VOID mrp_edd_close_channel (const MRP_CHANNEL_TYPE_SYSTEM pChSys);

LSA_VOID mrp_edd_check_if_params (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys);
LSA_VOID mrp_edd_check_if_port_params (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys);
LSA_UINT8 mrp_edd_get_new_mc_mode (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_LL_SREQ_RQB_ENUM_T rqbNum);

LSA_VOID mrp_edd_req_get_params (const MRP_CHANNEL_TYPE_SYSTEM pChSys);
LSA_VOID mrp_edd_req_get_params_done (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB);
LSA_VOID mrp_edd_req_get_port_params (const MRP_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 portID);
LSA_VOID mrp_edd_req_get_port_params_done (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB);
LSA_VOID mrp_edd_req_oha_data_write (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum);
LSA_VOID mrp_edd_req_oha_data_write_done (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_LL_SREQ_RQB_ENUM_T rqbNum);
LSA_VOID mrp_edd_req_snd (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB, LSA_UINT16 port, LSA_UINT16 length, LSA_UINT8 macStamp);
LSA_VOID mrp_edd_req_snd_done (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB);
LSA_VOID mrp_edd_req_rcv (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB);
LSA_VOID mrp_edd_req_rcv_done (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB);
LSA_VOID mrp_edd_req_linkChg (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB);
LSA_VOID mrp_edd_req_linkChg_done (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB);
LSA_VOID mrp_edd_req_set_multicast (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum);
LSA_VOID mrp_edd_req_set_multicast_done (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum);
LSA_VOID mrp_edd_req_set_multicast_fwd_ctrl (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum);
LSA_VOID mrp_edd_req_set_multicast_fwd_ctrl_done (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum);
LSA_VOID mrp_edd_req_flush_filtering_db (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum);
LSA_VOID mrp_edd_req_flush_filtering_db_done (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum);
LSA_VOID mrp_edd_write_rport_states_to_mib (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_LL_SREQ_RQB_ENUM_T rqbNum);
LSA_VOID mrp_edd_req_port_state (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum);
LSA_VOID mrp_edd_req_port_state_done (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum);
LSA_VOID mrp_edd_req_link_retrigger (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum);
LSA_VOID mrp_edd_req_in_link_retrigger (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum);
LSA_VOID mrp_edd_req_finish_upper (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum);
LSA_VOID mrp_edd_req_finish_close (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum);
LSA_VOID mrp_edd_req_prm_commit (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum);

#ifdef MRP_CFG_PLUGIN_0_MRM
LSA_VOID mrp_edd_req_tplg_chg_req (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum);
#endif

LSA_VOID mrp_edd_req_tplg_chg_finished (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum);
LSA_VOID mrp_edd_req_oha_topo_change_ind (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT8 checkUUID);

/* mrp_edd.c interface */
LSA_VOID mrp_edd_alloc_rqb (const MRP_CHANNEL_TYPE_SYSTEM pChSys, MRP_EDD_LOWER_RQB_PTR_TYPE *ppRQB, const LSA_UINT16 size);
LSA_VOID mrp_edd_free_rqb (MRP_EDD_LOWER_RQB_PTR_TYPE pRQB);

LSA_VOID mrp_edd_link_state_ind (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 port, LSA_UINT8 isLinkUp);
LSA_VOID mrp_edd_timer_cbf_link (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pIfInstance, LSA_USER_ID_TYPE uid);

LSA_VOID mrp_edd_set_rcv_callback (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_LL_RCV_FCT_PTR_TYPE cbf);
LSA_VOID mrp_edd_remove_rcv_callback (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_LL_RCV_FCT_PTR_TYPE cbf);
LSA_VOID mrp_edd_set_link_chg_callback (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_LL_LINK_CHG_FCT_PTR_TYPE cbf);
LSA_VOID mrp_edd_remove_link_chg_callback (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_LL_LINK_CHG_FCT_PTR_TYPE cbf);
LSA_VOID mrp_edd_set_tplg_chg_callback (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_LL_TPLG_CHG_FINISH_FCT_PTR_TYPE cbf);
LSA_VOID mrp_edd_remove_tplg_chg_callback (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_LL_TPLG_CHG_FINISH_FCT_PTR_TYPE cbf);
LSA_VOID mrp_edd_get_ring_ports (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT16 *port0, LSA_UINT16 *port1);
LSA_UINT16 mrp_edd_get_snd_rqb_max (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SND_RQB_ENUM_T poolID);
LSA_UINT8 mrp_edd_get_snd_rqb (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SND_RQB_ENUM_T poolID, MRP_EDD_LOWER_RQB_PTR_TYPE *ppRQB, LSA_UINT32 threshold);
LSA_VOID mrp_edd_set_snd_rqb (const LSA_UINT16 *pInstance, MRP_LL_SND_RQB_ENUM_T poolID, MRP_EDD_LOWER_RQB_PTR_TYPE pRQB);
LSA_VOID mrp_edd_set_link_up_time (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 port, LSA_UINT16 upTime);
LSA_UINT8 mrp_edd_force_link_up (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 port);

LSA_VOID mrp_edd_set_port_state (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, LSA_UINT16 port, MRP_PORT_STATE_TYPE state);
LSA_VOID mrp_edd_init_finish_upper (const MRP_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, const MRP_CHANNEL_TYPE_USER pChUsr);
LSA_VOID mrp_edd_init_finish_close (const MRP_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, const MRP_CHANNEL_TYPE_USER pChUsr);
LSA_UINT16 mrp_edd_init_prm_commit (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB);


/* mrp_low.c */
LSA_UINT16 mrp_ll_get_mrp_base_req_tbl (const MRP_LL_REQ_TBL_T** ppReqTbl, LSA_UINT16 maxCnt);
LSA_UINT16 mrp_ll_get_mrp_in_base_req_tbl(const MRP_LL_REQ_TBL_T** ppReqTbl, LSA_UINT16 maxCnt);

LSA_VOID mrp_ll_burst_state_trace (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance);
LSA_VOID mrp_ll_burst_trace (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_ENUM_BURST_TRACE_TYPE state, const MRP_LL_REQ_TBL_T *pReqTbl);
LSA_VOID mrp_ll_burst_do (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys);

/* macro map the original function to the new function with MRP_IN functionality */
#define mrp_ll_burst_req(_pChSys, _instance, _pReqTbl, _resetAllowed) \
{\
  LSA_UINT16    pInstance[2]; \
  pInstance[MRP_INSTANCE_INDEX] = _instance; \
  pInstance[MRP_IN_INSTANCE_INDEX] = MRP_IN_INSTANCE_NONE; \
  mrp_ll_in_burst_req(_pChSys, pInstance, _pReqTbl, _resetAllowed); \
}
LSA_VOID mrp_ll_in_burst_req(const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, const MRP_LL_REQ_TBL_T *pReqTbl, LSA_UINT8 resetAllowed);


LSA_VOID mrp_ll_add_user_reference (const MRP_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 pathType, const MRP_CHANNEL_TYPE_USER pChUsr);
LSA_VOID mrp_ll_remove_user_reference (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 pathType, const MRP_CONST_CHANNEL_TYPE_USER pChUsr);


LSA_VOID mrp_ll_event_handler (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance);
LSA_VOID mrp_ll_red_state_handler (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance);
LSA_VOID mrp_ll_init_modul_mrc (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, const MRP_MRC_PARAMS_TYPE *pParams);
LSA_VOID mrp_ll_deinit_modul_mrc (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance);
LSA_VOID mrp_ll_set_port_state_param (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, MRP_EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE pPSParam, LSA_UINT16 port, MRP_PORT_STATE_TYPE portState);
LSA_VOID mrp_ll_init_hsync (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance);
LSA_VOID mrp_ll_deinit_hsync (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance);

LSA_VOID mrp_ll_mc_mode_set (const MRP_MC_MODE_DATA_TYPE *mcMode);
LSA_UINT8 mrp_ll_clear_fdb (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT32 durationTicks);
LSA_VOID mrp_ll_fdb_cfg (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, const MRP_FWD_CTRL_DATA_TYPE *pFwdCtrl, LSA_UINT16 rport_1, LSA_UINT16 rport_2);
LSA_VOID mrp_ll_fdb_hsync_cfg (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, const MRP_FWD_CTRL_DATA_TYPE *pFwdCtrl, LSA_UINT16 rport_1, LSA_UINT16 rport_2);

LSA_VOID mrp_ll_fwd_ctrl_chg_rports (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const MRP_FWD_CTRL_DATA_TYPE *pFwdCtrl, LSA_UINT16 rport_1, LSA_UINT16 rport_2, LSA_UINT16 fwdMode);
LSA_VOID mrp_ll_fwd_ctrl_chg_all_ports_dis_rports_ena (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const MRP_FWD_CTRL_DATA_TYPE *pFwdCtrl, LSA_UINT16 rport_1, LSA_UINT16 rport_2);
LSA_VOID mrp_ll_fwd_ctrl_chg_all_ports_dis_rports_mode_ena (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const MRP_FWD_CTRL_DATA_TYPE *pFwdCtrl, LSA_UINT16 rport_1, LSA_UINT16 rport_2, LSA_UINT8 mode);
LSA_VOID mrp_ll_fwd_ctrl_chg_all_ports (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const MRP_FWD_CTRL_DATA_TYPE *pFwdCtrl, LSA_UINT16 fwdMode);

/* mrp0low.c */
#ifdef MRP_CFG_PLUGIN_0_MRM
LSA_UINT16 mrp_ll_get_mrm_req_tbl (const MRP_LL_REQ_TBL_T** ppReqTbl, LSA_UINT16 maxCnt);
LSA_VOID mrp_ll_init_modul_mrm (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, const MRP_MRM_PARAMS_TYPE *pParams);
LSA_VOID mrp_ll_deinit_modul_mrm (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance);
#endif

/* mrp1low.c */
#ifdef MRP_CFG_PLUGIN_1_MRM_EXT
LSA_UINT16 mrp_ll_get_mrm_ext_req_tbl (const MRP_LL_REQ_TBL_T** ppReqTbl, LSA_UINT16 maxCnt);
LSA_VOID mrp_ll_init_modul_mrm_ext (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance);
LSA_VOID mrp_ll_deinit_modul_mrm_ext (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT16 port0, LSA_UINT16 port1);
#endif

LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_init_nsm[];
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_deinit_nsm[];
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_ind_topo_change[];
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_finish_upper[];
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_ps_set[];
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_ps_deinit_mrpi_set[];
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_finish_close[];
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_flush_fdb[];
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_prm_commit[];
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_init_hsync[];
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_deinit_hsync[];
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_hsync_set_fdb[];

#ifdef MRP_CFG_DELAYED_LINK_UP
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_set_fdb[];
#endif

#ifdef MRP_CFG_PLUGIN_0_MRM
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrm_tplg_chg_req[];
#endif
#ifdef MRP_CFG_PLUGIN_1_MRM_EXT
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrm_ext_init_nsm_mode_chg[];
#endif
#ifdef MRP_CFG_PLUGIN_2_HSR
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_hsr_mgr_snd_req[];
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_hsr_init_ard[];
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_hsr_deinit_ard[];
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_hsr_set_fdb_ard[];
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_hsr_mode_change_ard[];
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_hsr_init_rcrm[];
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_hsr_deinit_rcrm[];
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_hsr_init_stby[];
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_hsr_cfg_stby_fdb[];
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_hsr_tplg_chg[];
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_hsr_mgr_ps[];
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_hsr_mgr_ps_ecori_de[];
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_hsr_stby_ps[];
LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_hsr_tplg_chg_snd[];
#endif /* MRP_CFG_PLUGIN_2_HSR */

/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/

/*====  out functions  =====*/

/*=============================================================================
 * function name:  MRP_EDD_OPEN_CHANNEL_LOWER
 *
 * function:       open a communication channel to lower layer
 *
 * parameters:     MRP_EDD_LOWER_RQB_PTR_TYPE lower_rqb_ptr:  pointer to lower-
 *                                                            RQB
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *
 * RQB-parameters: 
 *   RQB-header:
 *   LSA_OPCODE_TYPE    opcode:        *_OPC_OPEN_CHANNEL
 *   LSA_HANDLE_TYPE    handle:        channel-handle of MRP
 *   LSA_USER_ID_TYPE   user-id:       id of MRP
 *   RQB-args:
 *   LSA_HANDLE_TYPE    handle_upper:  channel-handle of MRP
 *   LSA_SYS_PATH_TYPE  sys_path:      system-path of channel                 
 *   LSA_VOID  LSA_FCT_PTR(mrp_request_lower_done_ptr)(MRP_EDD_LOWER_RQB_PTR_TYPE lower_rqb_ptr)
 *                                     pointer to callback-function
 *
 * RQB-return values via callback-function:
 *   RQB-args:
 *   LSA_HANDLE_TYPE    handle:        channel-handle of lower layer
 *   All other RQB-parameters are unchanged.
 *===========================================================================*/
#ifndef MRP_EDD_OPEN_CHANNEL_LOWER
LSA_VOID   MRP_EDD_OPEN_CHANNEL_LOWER(
  MRP_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
  LSA_SYS_PTR_TYPE            sys_ptr
);
#endif


/*=============================================================================
 * function name:  MRP_EDD_CLOSE_CHANNEL_LOWER
 *
 * function:       close a communication channel to lower layer
 *
 * parameters:     MRP_EDD_LOWER_RQB_PTR_TYPE lower_rqb_ptr:  pointer to lower-
 *                                                            RQB
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *
 * RQB-parameters:
 *     RQB-header:
 *     LSA_OPCODE_TYPE   opcode:   *_OPC_CLOSE_CHANNEL
 *     LSA_HANDLE_TYPE   handle:   channel-handle of lower layer
 *     LSA_USER_ID_TYPE  user-id:  id of mrp
 *
 * RQB-return values via callback-function:
 *     RQB-header:
 *     LSA_HANDLE_TYPE   handle:    channel-handle of mrp
 *     RQB-args:         response:  *_RSP_OK
 *                                  *_RSP_ERR_SYNTAX
 *                                  or others
 *     All other RQB-parameters are unchanged.
 *===========================================================================*/
#ifndef MRP_EDD_CLOSE_CHANNEL_LOWER
LSA_VOID   MRP_EDD_CLOSE_CHANNEL_LOWER(
  MRP_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
  LSA_SYS_PTR_TYPE            sys_ptr
);
#endif


/*=============================================================================
 * function name:  MRP_EDD_REQUEST_LOWER
 *
 * function:       send a request to a communication channel to lower layer
 *
 * parameters:     MRP_EDD_LOWER_RQB_PTR_TYPE lower_rqb_ptr:  pointer to lower-
 *                                                            RQB
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *
 *===========================================================================*/
#ifndef MRP_EDD_REQUEST_LOWER
LSA_VOID   MRP_EDD_REQUEST_LOWER (
  MRP_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
  LSA_SYS_PTR_TYPE            sys_ptr
);
#endif



/*=============================================================================
 * function name:  MRP_EDD_ALLOC_LOWER_RQB
 *
 * function:       allocate a lower-RQB
 *
 * parameters:     MRP_EDD_LOWER_RQB_PTR_TYPE  ...  *  lower_rqb_ptr_ptr:
 *                                   return value: pointer to lower-RQB-pointer
 *                                                 or LSA_NULL: The allocation
 *                                                 failed.
 *                 LSA_UINT16                      length:   length of RQB
 *                 LSA_SYS_PTR_TYPE                sys_ptr:  system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef MRP_EDD_ALLOC_LOWER_RQB
LSA_VOID   MRP_EDD_ALLOC_LOWER_RQB(
  MRP_EDD_LOWER_RQB_PTR_TYPE  *  lower_rqb_ptr_ptr,
  LSA_UINT16                                    length,
  LSA_SYS_PTR_TYPE                              sys_ptr
);
#endif


/*=============================================================================
 * function name:  MRP_EDD_FREE_LOWER_RQB
 *
 * function:       free a lower-RQB
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      lower-RQB or
 *                                                      wrong system-pointer
 *                 MRP_EDD_LOWER_RQB_PTR_TYPE lower_rqb_ptr:  pointer to lower-
 *                                                            RQB
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef MRP_EDD_FREE_LOWER_RQB
LSA_VOID MRP_EDD_FREE_LOWER_RQB(
  LSA_UINT16                    *ret_val_ptr,
  MRP_EDD_LOWER_RQB_PTR_TYPE    lower_rqb_ptr,
  LSA_SYS_PTR_TYPE              sys_ptr
);
#endif


/*=============================================================================
 * function name:  MRP_EDD_ALLOC_LOWER_MEM
 *
 * function:       allocate a lower-memory
 *
 * parameters:     MRP_EDD_LOWER_MEM_PTR_TYPE  ...  *  lower_mem_ptr_ptr:
 *                                   return value: pointer to lower-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: The allocation
 *                                                 failed.
 *                 LSA_UINT16                      length:   length of memory
 *                 LSA_SYS_PTR_TYPE  sys_ptr:      system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef MRP_EDD_ALLOC_LOWER_MEM
LSA_VOID MRP_EDD_ALLOC_LOWER_MEM(
  MRP_EDD_LOWER_MEM_PTR_TYPE    *lower_mem_ptr_ptr,
  LSA_UINT16                    length,
  LSA_SYS_PTR_TYPE              sys_ptr
);
#endif


/*=============================================================================
 * function name:  MRP_EDD_FREE_LOWER_MEM
 *
 * function:       free a lower-memory
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      lower-memory or
 *                                                      wrong system-pointer
 *                 MRP_EDD_LOWER_MEM_PTR_TYPE lower_mem_ptr:  pointer to lower-
 *                                                            memory
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef MRP_EDD_FREE_LOWER_MEM
LSA_VOID   MRP_EDD_FREE_LOWER_MEM(
  LSA_UINT16                    *ret_val_ptr,
  MRP_EDD_LOWER_MEM_PTR_TYPE    lower_mem_ptr_ptr,
  LSA_SYS_PTR_TYPE              sysptr
);
#endif


/*=============================================================================
 * function name:  MRP_EDD_ALLOC_LOWER_??_MEM
 *
 * function:       Allocates length tx/rx memory and returns the pointer in
 *                 lower_mem_ptr_ptr. NULL if no memory avail.
 *                 The memory-ptr must be DWORD aligned.
 *
 * parameters:     MRP_EDD_LOWER_MEM_PTR_TYPE  lower_mem_ptr_ptr:  
 *                                             pointer to lower-communication-memory
 *                 LSA_UINT32 length:          size of memory
 *                 LSA_SYS_PTR_TYPE            sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef MRP_EDD_ALLOC_LOWER_TX_MEM
LSA_VOID   MRP_EDD_ALLOC_LOWER_TX_MEM( 
  MRP_EDD_LOWER_MEM_PTR_TYPE    *lower_mem_ptr_ptr,
  LSA_UINT32                    length,
  LSA_SYS_PTR_TYPE							sys_ptr
);
#endif

#ifndef MRP_EDD_ALLOC_LOWER_RX_MEM
LSA_VOID   MRP_EDD_ALLOC_LOWER_RX_MEM( 
  MRP_EDD_LOWER_MEM_PTR_TYPE    *lower_mem_ptr_ptr,
  LSA_UINT32                    length,
  LSA_SYS_PTR_TYPE							sys_ptr
);
#endif


/*=============================================================================
 * function name:  MRP_EDD_FREE_LOWER_??_MEM
 *
 * function:       frees tx/rx communication memory
 *
 * parameters:     LSA_UINT16  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      lower-memory or
 *                                                      wrong system-pointer
 *                 MRP_EDD_LOWER_MEM_PTR_TYPE lower_mem_ptr_ptr:  
 *                                            pointer to lower-communication-memory
 *                 LSA_SYS_PTR_TYPE           sys_ptr:  system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef MRP_EDD_FREE_LOWER_TX_MEM
LSA_VOID   MRP_EDD_FREE_LOWER_TX_MEM( 
  LSA_UINT16                      *ret_val_ptr,
  MRP_EDD_LOWER_MEM_PTR_TYPE      lower_mem_ptr_ptr,
  LSA_SYS_PTR_TYPE							  sys_ptr
);
#endif

#ifndef MRP_EDD_FREE_LOWER_RX_MEM
LSA_VOID   MRP_EDD_FREE_LOWER_RX_MEM( 
  LSA_UINT16                      *ret_val_ptr,
  MRP_EDD_LOWER_MEM_PTR_TYPE      lower_mem_ptr_ptr,
  LSA_SYS_PTR_TYPE							  sys_ptr
);
#endif

/*====  in functions  =====*/

/*=============================================================================
 * function name:  mrp_edd_request_lower_done
 *
 * function:       lower done callback-function
 *
 * parameters:     MRP_EDD_LOWER_RQB_PTR_TYPE pRQB: 
 *                                        pointer to lower-communication-memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID mrp_edd_request_lower_done(
  MRP_EDD_LOWER_RQB_PTR_TYPE pRQB
  );


/*----------------------------------------------------------------------------*/
#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif  /* of MRP_LOW_H */


