#ifndef MRP_INT_H
#define MRP_INT_H


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
/*  F i l e               &F: mrp_int.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  internal headerfile                                                      */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef MRP_MESSAGE
/*  2008-01-07  mk    timer table size revised. hardware type check.         */
/*  2008-06-11  mk    CheckUUID added.                                       */
/*  2008-10-20  mk    MRP_DEFAULT_VAL_MRM_PRIO_MIN, mrp_rec_activate_role    */
/*                    added.                                                 */
/*  2009-01-15  ds    prototype for function mrp_rec_restore_if_check_dataset*/
/*                    added.                                                 */
/*  2009-02-10  ds    function mrp_rec_restore_if_check_dataset              */
/*                    renamed to mrp_rec_restore_saved_settings              */
/*                    new function mrp_rec_restore_alarm_settings            */
/*  2009-03-19  ds    mrp timer handling reworked                            */
/*  2009-07-02  ds    include hierarchy changed, timer calculation reworked  */
/*  2009-10-15  ds    MRP_LINK_INDICATION_CNT set to 1 (not used in src)     */ 
/*                    new element MRP_FWD_CTRL_DATA_TYPE in struct           */
/*                    MRP_MRC_DB_TYPE                                        */
/*  2009-10-22  ds    static timers not supported any longer, both MRP and   */
/*                    HSR use AUTO timers                                    */
/*  2010-02-16  ds    MRP based on 1 ms (HW) timer instead of 10 ms base     */
/*                    timer constants cleaned up, timer handling corrected   */
/*                    accelerated MRM link change handling implemented       */
/*                    MRP_CFG_TST_SHORT_TIME moved to mrp_cfg.h              */
/*  2010-05-25  ds    new OHA upper channel definitions                      */
/*  2010-06-28  ds    function mrp_sys_timer_stop returns a LSA_BOOL, if the */
/*                    timer can be stopped or is in pending state            */
/*  2010-07-19  ds    new feature MRP MIB implemented according to           */
/*                    IEC 62439-2                                            */
/*  2011-03-23  ds    SSR reworked to get rid of "Non-recoverable ring error"*/
/*                    in event log                                           */
/*  2011-07-21  ds    timer table new for cyclic component timer concept     */
/*  2012-04-04  ds    introduction of N MRP instances per interface          */
/*  2016-10-20  ds    usage of a 10 ms based LSA timer instead of 1 ms time  */
/*                    base                                                   */
/*  2017-06-28  ds    plugin 3 (MRP_RED_COUPLING_ENA) introduced             */
/*  2017-11-14  ds    MRP interconnection introduced                         */
/*  2018-10-19  hh    introduction of N MRP interconnection instances        */
/*                                                                           */
/*                                                                           */
#endif
/*****************************************************************************/

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

#include "mrp_inc.h"

#include "mrp_trc.h"
#include "mrp_pdu.h"

/*===========================================================================*/
/*                                 defines                                   */
/*===========================================================================*/
#define MRP_DEFAULT_DOMAIN_NAME_SIZE_MAX    240
#define MRP_NO_TIME_AVAILABLE               0

#define MRP_MAX_NO_OF_REQUEST_BURSTS        50

#define MRP_DEFAULT_INSTANCE                0

#define MRP_DEFAULT_IN_INSTANCE             0

/*---------------------------------------------------------------------------*/
/*                       MRP diagnosis                                       */
/*---------------------------------------------------------------------------*/
#define MRP_DIAG_CHAN_ERR_TYPE_REDUNDANCY_MISMATCH              0x8002

#define MRP_DIAG_EXT_CHAN_ERR_TYPE_MGR_ROLE_FAIL                0x8000
#define MRP_DIAG_EXT_CHAN_ERR_TYPE_MRP_RING_OPEN                0x8001
#define MRP_DIAG_EXT_CHAN_ERR_TYPE_MULTIPLE_MGR                 0x8003

/*---------------------------------------------------------------------------*/
/*                       nrt-api buffer parameter                            */
/*---------------------------------------------------------------------------*/
#define  MRP_NRT_TX_BUF_SIZE                0x600
#define  MRP_NRT_RX_BUF_SIZE                EDD_FRAME_BUFFER_LENGTH

/*---------------------------------------------------------------------------*/
/*                        nrt-api rx buffer count                            */
/*---------------------------------------------------------------------------*/
#define  MRP_NRT_RCV_RQB_PER_INST_CNT                10  /* MRP receive resources per MRP instance */

#define  MRP_IN_NRT_RCV_RQB_PER_IN_INST_CNT          10  /* MRP interconnection receive resources per MRP interconnection instance */
                                                         /* MRP interconnection resources: (3 frame_types) * (3 ports) = 9 -> 10 resources per inInstance */

#define MRP_GET_NRT_RCV_RQB_PER_MRP_INST(_noOfMrpInstances, _noOfMrpInInstances) \
  ( \
    (_noOfMrpInstances * (MRP_NRT_RCV_RQB_PER_INST_CNT + (_noOfMrpInInstances * MRP_IN_NRT_RCV_RQB_PER_IN_INST_CNT))) \
  )

/*---------------------------------------------------------------------------*/
/*                         link indication resources                         */
/*---------------------------------------------------------------------------*/
/* if DoCheck = true (default), the count MUST be 1 */
/* not used any longer, because only one resource needed: */
/*  #define  MRP_LINK_INDICATION_CNT       1              */

/*---------------------------------------------------------------------------*/
/*                             timer table definitions                       */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* number of timers needed for plugin 0 (MRM) and plugin 1 (Siemens-Auto-MRM)*/
/*---------------------------------------------------------------------------*/

  /* calculation of number of timers:     */
  /* auto timers for MRP (MRM/MRC and Auto-MRM): */
  /* - cyclic inst. timer (mrp_edd.c)*/
  /* - Linkup * NumPorts  (mrp_edd.c)*/
  /* - Topology-Change (Flush FDB) (mrp_edd.c)   */
  /* - MRPLinkup       (mrp_mrc.c)   */
  /* - MRPLinkdown     (mrp_mrc.c)   */
  /* - Alarming        (mrp_rec.c)   */
  /* - Test-Frame      (mrp0mrm.c)   */
  /* - Send TopologyChange Requ (mrm0mrm.c)   */
  /* - Other-MRM       (mrm0mrm.c)   */
  /* - Morphing        (mrm0mrm.c)   */

#if defined (MRP_CFG_PLUGIN_1_MRM_EXT)
# define  MRP_TIMER_TBL_SIZE_PLUGIN_MRM 4
#elif defined (MRP_CFG_PLUGIN_0_MRM)
# define  MRP_TIMER_TBL_SIZE_PLUGIN_MRM 4 
#else
# define  MRP_TIMER_TBL_SIZE_PLUGIN_MRM 0
#endif



#if defined (MRP_CFG_PLUGIN_1_MRM_EXT)
# define  MRP_TIMER_TBL_SIZE_PLUGIN_MRM 4
#elif defined (MRP_CFG_PLUGIN_0_MRM)
# define  MRP_TIMER_TBL_SIZE_PLUGIN_MRM 4 
#else
# define  MRP_TIMER_TBL_SIZE_PLUGIN_MRM 0
#endif

  /*---------------------------------------------------------------------------*/
  /*      number of timers needed for plugin 2 (HSR/Standby/ARD)               */
  /*---------------------------------------------------------------------------*/
#ifdef  MRP_CFG_PLUGIN_2_HSR
# define  MRP_TIMER_TBL_SIZE_PLUGIN_HSR       9
#else
# define  MRP_TIMER_TBL_SIZE_PLUGIN_HSR       0
#endif

  /*---------------------------------------------------------------------------*/
  /*      number of timers needed for MRP interconnection                      */
  /*---------------------------------------------------------------------------*/

  /* MRP interconnection timers (mrp_inbas.c)   */
  /* - send In_RstppTC request (RSTP+)     */
  /* - send In_TC request (MIM)     */
  /* - send In_Poll request (MIM)   */
  /* - send In_Lnk_Chg_Down (MIC)   */
  /* - send In_Lnk_Chg_Up (MIC)     */
# define  MRP_TIMER_TBL_SIZE_MRP_IN                  5

/*--------------------------------------------------------------------------------*/
/* timer pools for MRP/HSR timers                                                 */
/*--------------------------------------------------------------------------------*/
  typedef enum
  {
    MRP_TIMER_POOL_INTERFACE,
    MRP_TIMER_POOL_INSTANCE,
    MRP_TIMER_POOL_IN_INSTANCE
  }MRP_TIMER_POOL;


#define MRP_TIMER_INTERFACE 0xffff

/*----------------------------------------------------------------------------*/
/*      number of timers only triggered once per interface                    */
/*----------------------------------------------------------------------------*/
#define MRP_GET_IF_TIMER_NUM(_noOfPorts)\
  (\
  + (_noOfPorts)                                           /* mrp_edd.c: link change timer (linkUp) */\
  + 1                                                      /* mrp_edd.c: cyclic interface timer to update instance data */\
  )

/*--------------------------------------------------------------------------------*/
/* number of MRP/HSR timers related to all MRP/HSR instances within one interface */
/*--------------------------------------------------------------------------------*/
#define MRP_GET_INST_TIMER_NUM(_noOfMrpInstances, _noOfMrpInInstances)\
  (\
  + (_noOfMrpInstances * 1)                                                /* mrp_edd.c: topologyChange */\
  + (_noOfMrpInstances * 2)                                                /* mrp_mrc.c (MRPlinkChange timer (linkup/linkdown)) */\
  + (_noOfMrpInstances * MRP_TIMER_TBL_SIZE_PLUGIN_MRM)                    /* mrp0mrm.c + mrp1mrm.c */\
  + (_noOfMrpInstances  * 1)                                               /* mrp_rec.c: alarming after prm_commit  */\
  + (_noOfMrpInInstances * MRP_TIMER_TBL_SIZE_MRP_IN)                      /* mrp_inbas.c: MRP interconnection timer table               */\
  + MRP_TIMER_TBL_SIZE_PLUGIN_HSR                                          /* mrp2ard.c + mrp2mgr.c + mrp2bs.c + mrp2stdby.c*/\
  )
  /*NOTE: currently for each MRP_IN instance (and also for the MRP_RSTP+ instance) five timers are allocated. This is the case because the design
          does not distinguish between an MRP_IN and an MRP_RSTP+ instance in terms of timer. */

/*----------------------------------------------------------------------------*/
/*      overall number of MRP/HSR timers per interface                        */
/*----------------------------------------------------------------------------*/
#define MRP_GET_TIMER_TBL_SIZE(_noOfPorts, _noOfMrpInstances, _noOfMrpInInstances)   MRP_GET_IF_TIMER_NUM(_noOfPorts) + MRP_GET_INST_TIMER_NUM(_noOfMrpInstances, _noOfMrpInInstances)


/*---------------------------------------------------------------------------*/
/*                             time definitions                              */
/*---------------------------------------------------------------------------*/

  /*** common definitions ***/
#define  MRP_TIMER_TIME_BASE_DEZ                  10 /*  equals 10 ms */
#define  MRP_TIMER_TIME_BASE                      LSA_TIME_BASE_10MS

/*---------------------------------------------------------------------------*/
/*   MRP timer values: time in ms = XXX_T_CNT * MRP_TIMER_TIME_BASE_DEZ      */
/*---------------------------------------------------------------------------*/
#define  MRP_VAL_LINK_UP_RING_T_CNT              165
#define  MRP_VAL_LINK_UP_RING_SHORT_T_CNT          1
#define  MRP_VAL_LINK_UP_T_DEFAULT_CNT             0
#define  MRP_VAL_OTHER_RM_T_CNT                  100
#define  MRP_VAL_STOP_CLR_FDB_T_CNT              0xFFFFFFFFUL

/*---------------------------------------------------------------------------*/
/*                         MRP default values timer                          */
/*---------------------------------------------------------------------------*/
#define MRP_DEFAULT_VAL_MRM_TOP_CHG_10MS_T_CNT    1
#define MRP_DEFAULT_VAL_MRM_TOP_CHG_T_RES        10

#define MRP_DEFAULT_VAL_MRM_TST_SHORT_T_MS       10
#define MRP_DEFAULT_VAL_MRM_TST_DEFAULT_T_MS     20

#define MRP_DEFAULT_VAL_MRM_MORPHING_SHORT_T_MS  10
#define MRP_DEFAULT_VAL_MRM_MORPHING_LONG_T_MS 1000
#define MRP_DEFAULT_VAL_MRM_ALARM_DELAY_T_MS   5000

#define MRP_DEFAULT_VAL_MRC_LNK_DOWN_T_MS        20
#define MRP_DEFAULT_VAL_MRC_LNK_UP_T_MS          20

#define MRP_DEFAULT_VAL_MRM_T_1MS                 1

#define MRP_MIB_TIMETICKS_10MS                   10

#define MRP_DEFAULT_VAL_CYCLIC_INTERVAL_MS       10

/*---------------------------------------------------------------------------*/
/*                         MRP default values                                */
/*---------------------------------------------------------------------------*/
#define MRP_DEFAULT_VAL_MRM_TOP_NR_MAX           3
#define MRP_DEFAULT_VAL_MRM_TST_NR_MAX           3
#define MRP_DEFAULT_VAL_MRM_TST_OTHER_NR_MAX     5

#define MRP_DEFAULT_VAL_MRM_TST_SSR_NR_MAX      (1800/MRP_DEFAULT_VAL_MRM_TST_DEFAULT_T_MS)  
  /* number of test intervals to detect single side receive */
  /* parameters: */
  /* MRC link_up_hysteresis(= 1650 ms) + MRC IEC link_up_time(= 100 ms) + sanity(=50 ms) = 1800 ms */
  /* To reliably detect SSR, you have to wait for 1800 ms resp. 90 test frame cycles     */

#define MRP_DEFAULT_VAL_MRM_PRIO            0x8000
#define MRP_DEFAULT_VAL_MRM_PRIO_MIN        0xFFFF

#define MRP_DEFAULT_VAL_MRC_LNK_NR_MAX           4

#define MRP_DEFAULT_VAL_MRP_DEFAULT_DOMAIN_NAME  "default-mrpdomain"

#define MRP_DEFAULT_VAL_INST_RPORT_CNT           2

/*---------------------------------------------------------------------------*/
/*                    MRP Interconnection default values                     */
/*---------------------------------------------------------------------------*/
#define MRP_IC_DEFAULT_VAL_MIM_TOP_CHG_10MS_T_CNT      2      /* to reduce frame load of InTC frames, interval is increased from 10 ms -> 20 ms */
#define MRP_IC_DEFAULT_VAL_MIM_TOP_NR_MAX              4      /* to be able to even cover MRA failure TOPNRMax is increased 3 -> 4              */
#define MRP_IC_DEFAULT_VAL_MIM_LNK_STATUS_CHANGE_T_MS  20
#define MRP_IC_DEFAULT_VAL_MIM_LNK_STATUS_NR_MAX       8

#define MRP_IC_DEFAULT_VAL_MIC_LNK_DOWN_T_MS           20
#define MRP_IC_DEFAULT_VAL_MIC_LNK_UP_T_MS             20
#define MRP_IC_DEFAULT_VAL_MIC_LNK_NR_MAX              4

#define MRP_IC_DEFAULT_VAL_START_DELAY_100MS_T_CNT     2


#define MRP_IC_DEFAULT_VAL_MICPOSITION                 0

#define MRP_DEFAULT_VAL_MRP_IC_DEFAULT_DOMAIN_NAME  "default-mrpicdomain"

/*---------------------------------------------------------------------------*/
/* Defines for CheckDomainID settings (MRP Interconnection)                  */
/*---------------------------------------------------------------------------*/
#define MRP_IC_CHECK_DOMAINID_ON    1  
#define MRP_IC_CHECK_DOMAINID_OFF   0  

/*---------------------------------------------------------------------------*/
/* Defines for AlarmOutOfResource settings (MRP Indications)                 */
/*---------------------------------------------------------------------------*/
#define MRP_ALARM_OUT_OF_RESOURCE_FALSE         0
#define MRP_ALARM_OUT_OF_RESOURCE_TRUE          1

/*===========================================================================*/
/*            record data defines                                            */
/*===========================================================================*/
/* record defines */
#ifdef MRP_CFG_PLUGIN_2_HSR
  #define MRP_REC_DATA_IDX_PRV_HSR_ADJUST               0x80009000
  #define MRP_REC_DATA_IDX_PRV_STBY_ADJUST              0x80009001
  #define MRP_REC_DATA_IDX_PRV_HSR_REAL                 0x80009002
  #define MRP_REC_DATA_IDX_PRV_STBY_REAL                0x80009003
  #define MRP_REC_DATA_IDX_PRV_ARD_REAL                 0x80009004
  #define MRP_REC_DATA_IDX_PRV_HSR_CONFIG               0x8000D000
  
  #define MRP_REC_DATA_BLK_TYPE_PRV_HSR_ADJUST          0x9000
  #define MRP_REC_DATA_BLK_TYPE_PRV_STBY_ADJUST         0x9001
  #define MRP_REC_DATA_BLK_TYPE_PRV_HSR_REAL            0x9002
  #define MRP_REC_DATA_BLK_TYPE_PRV_STBY_REAL           0x9003
  #define MRP_REC_DATA_BLK_TYPE_PRV_ARD_REAL            0x9004
  #define MRP_REC_DATA_BLK_TYPE_PRV_HSR_CONFIG          0xD000

  #define MRP_REC_DATA_BLK_LEN_PRV_HSR_ADJUST           0x000A
  #define MRP_REC_DATA_BLK_LEN_PRV_STBY_ADJUST_MIN      0x0014
  #define MRP_REC_DATA_BLK_LEN_PRV_HSR_REAL             0x0014
  #define MRP_REC_DATA_BLK_LEN_PRV_STBY_REAL_MIN        0x0020
  #define MRP_REC_DATA_BLK_LEN_PRV_ARD_REAL             0x0008
  #define MRP_REC_DATA_BLK_LEN_PRV_HSR_CONFIG           0x0008

  #define MRP_REC_DATA_HSR_ROLE_OFF                     0x0000
  #define MRP_REC_DATA_HSR_ROLE_CL                      0x0001
  #define MRP_REC_DATA_HSR_ROLE_MGR                     0x0002
  #define MRP_REC_DATA_HSR_ROLE_ARD                     0x0003
  #define MRP_REC_DATA_HSR_ROLE_INVALIDATE_PROJ         0x0004

  #define MRP_REC_DATA_HSR_STATE_MGR_OFF                0x0000
  #define MRP_REC_DATA_HSR_STATE_MGR_ACTIVE             0x0001
  #define MRP_REC_DATA_HSR_STATE_MGR_PASSIVE            0x0002

  #define MRP_REC_DATA_HSR_DIAG_MGR_OTHER_RM            0x0001
  #define MRP_REC_DATA_HSR_DIAG_MGR_SINGLE_SIDE_RX      0x0002

  #define MRP_REC_DATA_STBY_STATE_OFF                   0x0000
  #define MRP_REC_DATA_STBY_STATE_MASTER_ACTIVE         0x0001
  #define MRP_REC_DATA_STBY_STATE_MASTER_PASSIVE        0x0002
  #define MRP_REC_DATA_STBY_STATE_MASTER_WAITING        0x0003
  #define MRP_REC_DATA_STBY_STATE_SLAVE_ACTIVE          0x0004
  #define MRP_REC_DATA_STBY_STATE_SLAVE_PASSIVE         0x0005
  #define MRP_REC_DATA_STBY_STATE_SLAVE_WAITING         0x0006

  #define MRP_REC_DATA_STBY_DIAG_PARTNER_DEAD           0x0001
  #define MRP_REC_DATA_STBY_DIAG_PARTNER_AMBIGUOUS      0x0002
  #define MRP_REC_DATA_STBY_DIAG_PARTNER_VERSION        0x0004

  #define MRP_REC_DATA_ARD_STATE_OFF                    0x0000
  #define MRP_REC_DATA_ARD_STATE_ON_MRP                 0x0001
  #define MRP_REC_DATA_ARD_STATE_ON_HSR_CLIENT          0x0002

  /*  maximum size of stby connection name (without final zero)  */
  #define MRP_STDBY_NAME_SIZE_MAX                           32 
  /*  minimum size of stby connection name (without final zero)  */
  #define MRP_STDBY_NAME_SIZE_MIN                            6
#endif

#define MRP_REC_DATA_IDX_PRV_DATA_ADJUST              0x80008052
#define MRP_REC_DATA_IDX_PRV_DATA_REAL                0x80008054

#define MRP_REC_DATA_IDX_IF_DATA_REAL                 0x8050
#define MRP_REC_DATA_IDX_IF_DATA_CHECK                0x8051
#define MRP_REC_DATA_IDX_IF_DATA_ADJUST               0x8052
#define MRP_REC_DATA_IDX_PORT_DATA_ADJUST             0x8053
#define MRP_REC_DATA_IDX_PORT_DATA_REAL               0x8054

#define MRP_REC_DATA_IDX_IC_PORT_DATA_ADJUST          0x8055
#define MRP_REC_DATA_IDX_IC_PORT_DATA_CHECK           0x8056
#define MRP_REC_DATA_IDX_IC_PORT_DATA_REAL            0x8057

#define MRP_REC_DATA_BLK_TYPE_PRV_DATA_ADJUST         0x8052
#define MRP_REC_DATA_BLK_TYPE_PRV_DATA_REAL           0x8054

#define MRP_REC_DATA_BLK_TYPE_IF_DATA_ADJUST          0x0211
#define MRP_REC_DATA_BLK_TYPE_IF_DATA_REAL            0x0212
#define MRP_REC_DATA_BLK_TYPE_IF_DATA_CHECK           0x0213

#define MRP_REC_DATA_BLK_TYPE_PORT_DATA_ADJUST        0x0214
#define MRP_REC_DATA_BLK_TYPE_PORT_DATA_REAL          0x0215

#define MRP_REC_DATA_BLK_TYPE_MANAGER_PARAMS          0x0216
#define MRP_REC_DATA_BLK_TYPE_CLIENT_PARAMS           0x0217
#define MRP_REC_DATA_BLK_TYPE_RT_MODE_MGR_DATA        0x0218
#define MRP_REC_DATA_BLK_TYPE_RING_STATE_DATA         0x0219
#define MRP_REC_DATA_BLK_TYPE_RT_STATE_DATA           0x021A
#define MRP_REC_DATA_BLK_TYPE_RT_MODE_CLIENT_DATA     0x021D

#define MRP_REC_DATA_BLK_TYPE_INST_DATA_ADJUST        0x0231
#define MRP_REC_DATA_BLK_TYPE_INST_DATA_REAL          0x0232
#define MRP_REC_DATA_BLK_TYPE_INST_DATA_CHECK         0x0233

#define MRP_REC_DATA_BLK_TYPE_IC_PORT_DATA_ADJUST     0x0234
#define MRP_REC_DATA_BLK_TYPE_IC_PORT_DATA_CHECK      0x0235
#define MRP_REC_DATA_BLK_TYPE_IC_PORT_DATA_REAL       0x0236

#define MRP_REC_DATA_BLK_VERS_HIGH1                     0x01

#define MRP_REC_DATA_BLK_VERS_LOW0                      0x00
#define MRP_REC_DATA_BLK_VERS_LOW1                      0x01
#define MRP_REC_DATA_BLK_VERS_LOW2                      0x02

#define MRP_REC_DATA_BLK_VERS_HIGH1_LOW0              0x0100  /*default */
#define MRP_REC_DATA_BLK_VERS_HIGH1_LOW1              0x0101
#define MRP_REC_DATA_BLK_VERS_HIGH1_LOW2              0x0102

#define MRP_REC_DATA_LEN_MIN                          0x000B
#define MRP_REC_DATA_LEN_MAX                          0xFFFE
#define MRP_REC_DATA_LEN_BLK_TYPE_AND_BLK_LEN         0x0004
#define MRP_REC_DATA_LEN_BLK_VERSION                  0x0002
#define MRP_REC_DATA_LEN_BLK_HEADER                   0x0006
#define MRP_REC_DATA_LEN_BLK_HEADER_PADDING           0x0002
#define MRP_REC_DATA_LEN_ROLE                         0x0002
#define MRP_REC_DATA_LEN_RPORT                        0x0002
#define MRP_REC_DATA_LEN_UUID_AND_ROLE                0x0012
#define MRP_REC_DATA_LEN_UUID                         0x0010
#define MRP_REC_DATA_LEN_ROLE_PADDING                 0x0002
#define MRP_REC_DATA_LEN_CHECK                        0x0004
#define MRP_REC_DATA_LEN_DOMAIN_NAME_FIELD            0x0001
#define MRP_REC_DATA_LEN_DOMAIN_NAME_MAX              0x00F0

#define MRP_REC_DATA_LEN_IC_DOMAINID                  0x0002
#define MRP_REC_DATA_LEN_IC_ROLE                      0x0002

#define MRP_REC_DATA_BLK_LEN_PRV_DATA_ADJUST          0x0008
#define MRP_REC_DATA_BLK_LEN_PRV_DATA_REAL            0x0020

#define MRP_REC_DATA_BLK_LEN_IF_DATA_ADJ_VERS0_MIN    0x0019
#define MRP_REC_DATA_BLK_LEN_IF_DATA_ADJ_VERS1_MIN    0x0004
#define MRP_REC_DATA_BLK_LEN_IF_DATA_REAL_VERS1_MIN   0x0019
#define MRP_REC_DATA_BLK_LEN_IF_DATA_REAL_VERS2_MIN   0x0004
#define MRP_REC_DATA_BLK_LEN_IF_DATA_CHECK            0x0018
#define MRP_REC_DATA_BLK_LEN_IF_DATA_CHECK_VERS1      0x0004

#define MRP_REC_DATA_BLK_LEN_PORT_DATA_ADJUST         0x0014
#define MRP_REC_DATA_BLK_LEN_PORT_DATA_REAL           0x0014
#define MRP_REC_DATA_BLK_LEN_MANAGER_PARAMS           0x0010
#define MRP_REC_DATA_BLK_LEN_CLIENT_PARAMS            0x0008
#define MRP_REC_DATA_BLK_LEN_RT_MANAGER_MODE          0x000C
#define MRP_REC_DATA_BLK_LEN_RING_STATE_DATA          0x0004
#define MRP_REC_DATA_BLK_LEN_RT_STATE_DATA            0x0004
#define MRP_REC_DATA_BLK_LEN_RT_CLIENT_MODE           0x0008

#define MRP_REC_DATA_BLK_LEN_IC_PORT_DATA_ADJUST_MIN  0x0009
#define MRP_REC_DATA_BLK_LEN_IC_MIM_PARAMS            0x000C
#define MRP_REC_DATA_BLK_LEN_IC_MIC_PARAMS            0x000C
#define MRP_REC_DATA_BLK_LEN_IC_PORT_DATA_CHECK       0x0008
#define MRP_REC_DATA_BLK_LEN_IC_PORT_DATA_REAL_MIN    0x000B
#define MRP_REC_DATA_BLK_LEN_IC_STATE_DATA            0x0002

#define MRP_REC_DATA_INST_RING_STATE_OPEN             0x0000
#define MRP_REC_DATA_INST_RING_STATE_CLOSED           0x0001

#define MRP_REC_DATA_INST_RT_ROLE_OFF                 0x0000
#define MRP_REC_DATA_INST_RT_ROLE_CLASS_1_2           0x0001

#define MRP_REC_DATA_INST_RT_STATE_RED_LOST           0x0000
#define MRP_REC_DATA_INST_RT_STATE_RED_AVAILABLE      0x0001

#define MRP_REC_DATA_CHECK_MRM                        0x0001
#define MRP_REC_DATA_CHECK_UUID                       0x0002
#define MRP_REC_DATA_CHECK_MASK                       0xFF000000

#define MRP_REC_IC_PORT_DATA_IC_STATE_OPEN            0x0000
#define MRP_REC_IC_PORT_DATA_IC_STATE_CLOSED          0x0001

#define MRP_REC_IC_PORT_DATA_CHECK_MIM                0x0001
#define MRP_REC_IC_PORT_DATA_CHECK_DOMAINID           0x0002
#define MRP_REC_IC_PORT_DATA_CHECK_MASK               0xFF000000

/* alarm channel properties */
#define MRM_ALARM_PROP_UNCHANGED                      0x0000
#define MRM_ALARM_PROP_MR_APPEAR                      0x0A00
#define MRM_ALARM_PROP_MR_DISAPPEAR                   0x1200
#define MRM_ALARM_PROP_MD_APPEAR                      0x0C00
#define MRM_ALARM_PROP_MD_DISAPPEAR                   0x1400
#define MRM_ALARM_PROP_ERROR_APPEAR                   0x0800
#define MRM_ALARM_PROP_ERROR_DISAPPEAR                0x1000


/* debug role info */
#define MRP_REC_DBG_ROLE_MRC                          0x00000001
#define MRP_REC_DBG_ROLE_MRM                          0x00000002
#define MRP_REC_DBG_ROLE_AUTO_MRM                     0x00000004
#define MRP_REC_DBG_ROLE_HSR_CL                       0x00000010
#define MRP_REC_DBG_ROLE_HSR_MGR                      0x00000020
#define MRP_REC_DBG_ROLE_ARD                          0x00000040
#define MRP_REC_DBG_ROLE_STB                          0x80000000

#define MRP_REC_DBG_ROLE_MIC                          0x00000100
#define MRP_REC_DBG_ROLE_MIM                          0x00000200 


/*===========================================================================*/
/*                               macros                                      */
/*===========================================================================*/
/* record read/write defines */
#define MRP_REC_DB_GET16_NTOH(_ptr,_val) { _val = MRP_GET16_NTOH(_ptr, 0); (_ptr) += sizeof(LSA_UINT16); }
#define MRP_REC_DB_PUT16_HTON(_ptr,_val) { MRP_PUT16_HTON(_ptr, 0, _val); (_ptr) += sizeof(LSA_UINT16); }
#define MRP_REC_DB_GET32_NTOH(_ptr,_val) { _val = MRP_GET32_NTOH(_ptr, 0); (_ptr) += sizeof(LSA_UINT32); }
#define MRP_REC_DB_PUT32_HTON(_ptr,_val) { MRP_PUT32_HTON(_ptr, 0, _val); (_ptr) += sizeof(LSA_UINT32); }

#define MRP_REC_DB_PUTN16_HTON(_dest, _src, _number) \
{ \
  for(LSA_UINT8 i = 0; i < _number; i++) \
  { \
    MRP_PUT16_HTON(_dest, 0, *_src); \
    (_dest) += sizeof(LSA_UINT16); \
    (_src) += sizeof(LSA_UINT16); \
  } \
}

/* record write: get err_offset of the record_data field which has a wrong content */
#define MRP_REC_DATA_OFFSET_GET(_ptr_base, _ptr_act)	((LSA_UINT32)((LSA_UINT8 *)_ptr_act  - (LSA_UINT8 *)_ptr_base))

#define MRP_REC_WRITE_ARGS_ERR_OFFSET_GET(_pArgs)			((_pArgs)->ctrl.prm_write.err_offset)
#define MRP_REC_WRITE_ARGS_ERR_OFFSET_SET(_pArgs,_val)	((_pArgs)->ctrl.prm_write.err_offset = (_val))

#define MRP_REC_PRM_END_ARGS_ERR_OFFSET_SET(_pArgs,_val)	((_pArgs)->ctrl.prm_end.err_offset = (_val))
#define MRP_REC_PRM_END_ARGS_ERR_PORT_ID_SET(_pArgs,_val)	((_pArgs)->ctrl.prm_end.err_port_id = (_val))
#define MRP_REC_PRM_END_ARGS_ERR_INDEX_SET(_pArgs,_val)	((_pArgs)->ctrl.prm_end.err_index = (_val))
#define MRP_REC_PRM_END_ARGS_ERR_FAULT_SET(_pArgs,_val)	((_pArgs)->ctrl.prm_end.err_fault = (_val))

  /*===========================================================================*/
  /*                              CBF types                                    */
  /*===========================================================================*/

  /* misc */
  typedef LSA_VOID   LSA_FCT_PTR(/*ATTR*/, MRP_TIMER_CBF_FCT_PTR_TYPE)(const MRP_CONST_CHANNEL_TYPE_COMMON, const LSA_UINT16 *pInstance, LSA_USER_ID_TYPE);
  typedef LSA_VOID   LSA_FCT_PTR(/*ATTR*/, MRP_HSRM_FCT_PTR_TYPE)(const MRP_CONST_CHANNEL_TYPE_SYSTEM);
  typedef LSA_VOID   LSA_FCT_PTR(/*ATTR*/, MRP_HSRS_FCT_PTR_TYPE)(const MRP_CONST_CHANNEL_TYPE_SYSTEM);

  /*===========================================================================*/
  /*                        types related to timers                            */
  /*===========================================================================*/

  typedef struct 
  {
    MRP_TIMER_CBF_FCT_PTR_TYPE pTimerCbf;
    LSA_USER_ID_TYPE           uid;
    LSA_UINT16                 instance[2];   /* instance[0]: new parameter for LSA V5.2 to determine the affected MRP(HSR) instance */
                                               /* instance[1]: new parameter for LSA V7.1 to determine the affected MRP interconnection instance (MRP instance must be 0) */
    LSA_UINT16                 type;           /* oneShot or cyclic timer */
    MRP_ENUM_TIMER_USE_T       purpose;
    MRP_TIMER_STS_T            state;
    MRP_UPPER_RQB_PTR_TYPE     pRQB;
    LSA_UINT32                 tx;
    LSA_UINT32                 rx;

    /* new: */
    LSA_UINT32                 startTicks;
    LSA_UINT32                 durationTicks;
  }MRP_TIMER_PARA_DB_TYPE;

  typedef struct 
  {
    MRP_CHANNEL_TYPE_COMMON   pCh;
    LSA_UINT16                timerTblSize;
    MRP_TIMER_PARA_DB_TYPE    *pTimerPara;
  }MRP_TIMER_DB_TYPE;

  /*===========================================================================*/
  /*                         LINK STATE types                                  */
  /*===========================================================================*/

  typedef enum 
  {
    MRP_LINK_STATE_DOWN  
    ,MRP_LINK_STATE_UP    
  }MRP_LINK_STATE_TYPE;


  /*===========================================================================*/
  /*                                 MRC types                                 */
  /*===========================================================================*/

  typedef enum 
  {
    MRP_MRC_EVENT_T_NONE
    ,MRP_MRC_EVENT_T_MAU_TYPE_CHG
    ,MRP_MRC_EVENT_T_TPLG_CHG
    ,MRP_MRC_EVENT_T_LINK_CHANGE
    ,MRP_MRC_EVENT_T_UP_TIMER
    ,MRP_MRC_EVENT_T_DOWN_TIMER
    ,MRP_MRC_EVENT_T_IN_FWD_REQ
    ,MRP_MRC_EVENT_T_IN_TX_REQ
  }MRP_MRC_EVENT_TYPE;

  /*===========================================================================*/
  /*                                MIB types                                  */
  /*===========================================================================*/

  typedef struct _MRP_MIB_RPORT_STRUCT
  {
    LSA_UINT16               mrpPortId;     /* portId of the physical ring-port */
    LSA_UINT32               mrpIfIndex;    /* ifIndex of the ring-port         */
    LSA_UINT16               mrpEddState;   /* edd related state of the ringport*/
    LSA_BOOL                 mrpLinkState;  /* link state of the ringport       */
    MRP_MIB_ENUM_RPORT_STATE mrpMibState;   /* operational mib state of the ring-port */
  }MRP_MIB_RPORT_STRUCT;


  typedef struct _MRP_MIB_DB_TYPE
  {
    LSA_UINT16                      instance;                   /* MRP instance within one interface */
    /* monitoring MIB params */
    LSA_UINT8                       domainUUID[16];             /* mrp domain uuid */
    LSA_UINT8                       domainName[MRP_DEFAULT_DOMAIN_NAME_SIZE_MAX + 1]; /* logical name for the MRP domain */
    LSA_UINT32                      domainNameLength;           /* length of the MRP domain name   */

    MRP_MIB_ENUM_DOMAIN_ROLE        domainAdminRole;            /* requested MRP role   */
    MRP_MIB_ENUM_DOMAIN_ROLE        domainOperRole;             /* operational MRP role */

    /* MRP MIB related and former service OHA_MRP_DATA_WRITE related */
    MRP_MIB_RPORT_STRUCT            domainDataRPort1;           /* data structure of 1st ringport */
    MRP_MIB_RPORT_STRUCT            domainDataRPort2;           /* data structure of 2nd ringport */

    LSA_UINT16                      domainState;                /* operational status of the MRP entity */
    /* see enum MRP_MIB_ENUM_DOMAIN_STATE   */
    LSA_UINT16                      domainError;                /* diagnosis data */
    /* see enum MRP_MIB_ENUM_DOMAIN_ERROR, values may be or-ed*/

    MRP_MIB_ENUM_DOMAIN_BLOCKED     domainBlocked;              /* constant, only "enabled (1)" is supported */
    LSA_UINT32                      domainVlanId;               /* VLAN ID assigned to the MRP protocol      */

    /* parameters only applicable to MRM role */
    LSA_UINT16                      domainMrmReactOnLinkChange; /* constant, only applicable if operRole =MRP manager */
    LSA_UINT16                      domainManagerPrio;          /* mrm prio, default value or provisioned */
    /* only applicable if operRole =MRP manager */

    LSA_UINT32                      domainRingOpenCount;        /* counter for ring state changes to open   */
    /* only applicable if operRole =MRP manager */

    LSA_UINT32                      domainLastRingOpenChange;   /* time difference since last ring open (in ms) */
    /* only applicable if operRole =MRP manager */
    LSA_UINT32                      ringOpenStartTime;          /* timestamp of ring open event */
    LSA_UINT32                      ringOpenEndTime;            /* timestamp of ring close event */

    LSA_UINT32                      domainResetRoundTripDelays; /* reset round trip delay values (min and max)*/
    /* only applicable if operRole =MRP manager */
    LSA_BOOL                        currentResetRoundTripDelayFlag; /* current operational setting of resetRoundTripDelayFlag*/
    /* flag is reset, as soon as counters were reset         */
    LSA_UINT32                      domainRoundTripDelayMax;    /* max round trip delay of a test frame (in ms) */
    /* only applicable if operRole =MRP manager */
    LSA_UINT32                      domainRoundTripDelayMin;    /* min round trip delay of a test frame since startup*/                                                                                                                            /* only applicable if operRole =MRP manager */
  }MRP_MIB_DB_TYPE;



  /*===========================================================================*/
  /*        MRP interconnection TX/FWD request types                           */
  /*===========================================================================*/

  typedef enum _MRP_IN_FRAME_TYPE
  {
    MRP_IN_FRAME_TYPE_NONE
    , MRP_IN_FRAME_TYPE_LINK_STATE_POLL
    , MRP_IN_FRAME_TYPE_LINK_CHANGE
    , MRP_IN_FRAME_TYPE_IN_TC

  }MRP_IN_FRAME_TYPE;


  /* tx info for MRP interconnection frames */
  typedef struct _MRP_IN_TX_REQ_INFO_TYPE
  {
    MRP_IN_FRAME_TYPE inFrameType;
    LSA_BOOL          isRstpOriginated;

    LSA_UINT16        inInstance;
    LSA_UINT16        inId;
    LSA_UINT16        rxPort;
    LSA_UINT8         rxSA[MRP_PACKET_VAL_MAC_ADDR_LEN];

    MRP_LINK_STATE_TYPE inLinkState; /* IN_LNK_CHG frames */
    LSA_UINT16          inLinkRole;  /* IN_LNK_CHG frames */
 
    LSA_UINT16          intervalTimeMs; /* IN_LNK_CHG and IN_TC frames */
    LSA_UINT16          portRole;       /* IN_LNK_CHG and IN_POLL frames */

    LSA_UINT8           domainUUID[MRP_PACKET_VAL_DOMAIN_UUID_LEN];
    LSA_UINT8           coupledDomainUUID[MRP_PACKET_VAL_DOMAIN_UUID_LEN];

  }MRP_IN_TX_REQ_INFO_TYPE;


  /*===========================================================================*/
  /*                                MRC types                                  */
  /*===========================================================================*/
  typedef struct _MRP_MRC_DB_TYPE MRP_MRC_DB_TYPE;
  typedef LSA_VOID   LSA_FCT_PTR(/*ATTR*/, MRP_MRC_FCT_PTR_TYPE)(MRP_MRC_DB_TYPE*);

  struct _MRP_MRC_DB_TYPE 
  {
    MRP_CHANNEL_TYPE_SYSTEM pChSys;
    LSA_UINT16              pInstance[2];                           /* id of this MRP interconnection instance            */
                                                                    /* pInstance[0]: related MRP instance                 */
                                                                    /* pInstance[1]: related MRP interconnection instance is set to MRP_IN_INSTANCE_NONE */
    /* variables */
    LSA_UINT8               DomainUUID[16]; /* DomainUUID                             */
    LSA_UINT32              Vlan;           /* VLAN signaled in MRP frames            */
    LSA_UINT16              RPort_1;        /* ring port 1                            */
    LSA_UINT16              RPort_2;        /* ring port 2                            */
    LSA_UINT16              PRM_RPort;      /* primary port idx                       */
    LSA_UINT16              SEC_RPort;      /* secondary port idx                     */
    LSA_UINT16              LNKNReturn;     /* retry for link change                  */

    /* internals */
    MRP_MRC_EVENT_TYPE      CurrentEvent;   /* current work event type                 */
    MRP_MRC_FCT_PTR_TYPE    pStateCbf;      /* current state                           */
    MRP_FWD_CTRL_DATA_TYPE  fwdCtrl;        /* FDB cfg data                            */

    /* event attributes */
    LSA_UINT16              RPort;          /* MRP_MRC_EVENT_T_MAU_TYPE_CHG: ring port id */
    MRP_LINK_STATE_TYPE     LinkState;      /* MRP_MRC_EVENT_T_MAU_TYPE_CHG: link state   */
    LSA_UINT16              RxTimeTcnt;     /* MRP_MRM_EVENT_T_TPLG_CHG: change time = tplgChangeInterval * MRP_TIMER_TIME_BASE */

    /* parameters */
    LSA_UINT16              LNKNRmax;       /* max retry for link change              */
    LSA_UINT16              LNKupTcnt;      /* link up time                           */
    LSA_UINT16              LNKdownTcnt;    /* link down time                         */
    LSA_UINT8               CheckUUID;      /* check attribute for UUID               */

    /* timer */
    LSA_UINT16              TimerIdxUp;
    LSA_UINT16              TimerIdxDown;

    /* misc */
    LSA_UINT8               runState;
    LSA_UINT8               isRingBreakEvt;

    /* MRP interconnection transfer data*/
    MRP_IN_TX_REQ_INFO_TYPE *pMrcInTxInfo;

    /* reference to interface database */
    MRP_IF_DB_TYPE          *pDBifRef;
    /* reference to MRP-MIB DB */
    MRP_MIB_DB_TYPE         *pDBmib;

    /* MRP interconnection (MIM) CBF to indicate MRP ring changes */
    MRP_IN_MIM_MRP_RING_CHG_FCT_PTR_TYPE *ppMrpInRingChgCbf;
 };


  /*===========================================================================*/
  /*                                chain-macros                               */
  /*===========================================================================*/

#define MRP_RQB_PUSH_TO_BOTTOM(_Bottom, _Top, _Block)  \
  {\
    MRP_RQB_SET_NEXT_RQB_PTR(_Block,_Bottom);         \
    MRP_RQB_SET_PREV_RQB_PTR(_Block,LSA_NULL);        \
    if( LSA_HOST_PTR_ARE_EQUAL((_Bottom), LSA_NULL))  \
      _Top = _Bottom = _Block; /* if queue is empty!*/\
    else \
    {\
      MRP_RQB_SET_PREV_RQB_PTR(_Bottom, _Block);      \
      _Bottom = _Block;                               \
    }\
  }


#define MRP_RQB_POP_FROM_TOP(_Bottom, _Top, _Block) \
  {\
    _Block = _Top;    \
    if( !LSA_HOST_PTR_ARE_EQUAL((_Top), LSA_NULL))  \
    {\
      _Top = MRP_RQB_GET_PREV_RQB_PTR(_Top);        \
      if( LSA_HOST_PTR_ARE_EQUAL((_Top), LSA_NULL)) \
        _Bottom = LSA_NULL; /* if queue is empty!*/ \
      else            \
        MRP_RQB_SET_NEXT_RQB_PTR(_Top, LSA_NULL);   \
    }\
  }

  /* currently not used in source code! */
#define MRP_RQB_PUSH_TO_TOP(_Bottom, _Top, _Block)    \
  {\
    MRP_RQB_SET_NEXT_RQB_PTR(_Block, LSA_NULL);   \
    MRP_RQB_SET_PREV_RQB_PTR(_Block, _Top);       \
    if(LSA_HOST_PTR_ARE_EQUAL((_Top), LSA_NULL))  \
    {\
      _Bottom = _Top = _Block; \
    } /* if queue is empty! */ \
    else \
    {\
      MRP_RQB_SET_NEXT_RQB_PTR(_Top,_Block); \
      _Top = _Block; \
    }\
  }


  /* currently not used in source code! */
#define MRP_RQB_POP_FROM_BOTTOM(_Bottom, _Top, _Block)  \
  {\
    _Block = _Bottom;   \
    if( !LSA_HOST_PTR_ARE_EQUAL((_Bottom), LSA_NULL))   \
    {\
      _Bottom = MRP_RQB_GET_NEXT_RQB_PTR(_Bottom);      \
      if( LSA_HOST_PTR_ARE_EQUAL((_Bottom), LSA_NULL))  \
        _Top = LSA_NULL; /* if queue is empty! */       \
      else  \
        _Bottom->prev_rqb_ptr = LSA_NULL;\
    }\
  }

  /*===========================================================================*/
  /*                                other macros                               */
  /*===========================================================================*/
#define MRP_EDD_ALLOC_RQB(_ch, _pRQB, _size, _service) \
  mrp_edd_alloc_rqb((_ch), &(_pRQB), (_size));\
  MRP_FATAL1(!LSA_HOST_PTR_ARE_EQUAL((_pRQB),LSA_NULL));\
  MRP_EDD_LOWER_SET_SERVICE((_pRQB), (_service));  

/*=============================================================================
 * function name:  mrp_fatal
 *
 * function:       undocumented
 *===========================================================================*/

/*lint -function(exit, mrp_fatal) */
LSA_VOID
mrp_fatal(
  LSA_UINT16 mrp_module_id,
  LSA_INT line,
  LSA_VOID* pFile,
  LSA_UINT16 fileNameLength
);

#ifdef MRP_CFG_NO_FATAL_FILE_INFO
  #ifndef MRP_FATAL
    #define MRP_FATAL  mrp_fatal(MRP_MODULE_ID, __LINE__, 0, 0)
  #endif
#else
  #ifndef MRP_FATAL
    #define MRP_FATAL  mrp_fatal(MRP_MODULE_ID, __LINE__, __FILE__, sizeof(__FILE__))
  #endif
#endif

#define MRP_FATAL1(cond)       \
  if (!(cond))                 \
  {                            \
    MRP_FATAL;                 \
  }//lint -e774 -e506

/* Defines for ardMode */
#define MRP_ARD_MODE_OFF     0x0000
#define MRP_ARD_MODE_MRP_ON  0x0001
#define MRP_ARD_MODE_HSRC_ON 0x0002

/* Defines for mrpMode */
#define MRP_MODE_OFF         0x0000
#define MRP_MODE_MRC_ON      0x0001
#define MRP_MODE_MRM_ON      0x0002

/* Defines for mrpOperRole */

typedef struct _MRP_MODE_DATA_TYPE
{
  LSA_UINT16 ardMode;
  LSA_UINT16 mrpMode;
  LSA_UINT16 mrpOperRole;
  LSA_UINT16 rPort1;
  LSA_UINT16 rPort2;
  LSA_UINT8  DomainUUID[16];
}MRP_MODE_DATA_TYPE;


  /*---------------------------------------------------------------------------*/
  /*                   record data types                                       */
  /*---------------------------------------------------------------------------*/

  typedef enum 
  {
    MRP_REC_ENUM_PARSE_LEN,
    MRP_REC_ENUM_PARSE_VALUE,
    MRP_REC_ENUM_PARSE_WRITE,
    MRP_REC_ENUM_PARSE_END
  }MRP_REC_ENUM_PARSE_TYPE;

  typedef enum 
  {
    MRP_REC_ENUM_DEFAULT_FROM_IF_DETAILS,
    MRP_REC_ENUM_DEFAULT_FROM_STD
  }MRP_REC_ENUM_DEFAULT_TYPE;

  typedef enum 
  {
#ifdef MRP_CFG_PLUGIN_2_HSR
    MRP_REC_ENUM_MODULE_STATE_ARD,
    MRP_REC_ENUM_MODULE_STATE_STBY,
    MRP_REC_ENUM_MODULE_STATE_HSR_RC,
    MRP_REC_ENUM_MODULE_STATE_HSR_MGR,
#endif
    MRP_REC_ENUM_MODULE_STATE_MRC,
#ifdef MRP_CFG_PLUGIN_0_MRM
    MRP_REC_ENUM_MODULE_STATE_MRM,
#endif
    MRP_REC_ENUM_MODULE_STATE_MIC, 
    MRP_REC_ENUM_MODULE_STATE_MIM,

    MRP_REC_ENUM_MODULE_STATE_COUNT
  }MRP_REC_ENUM_MODULE_STATE_TYPE;


/*---------------------------------------------------------------------------*/
/*         module state struct                                               */
/*---------------------------------------------------------------------------*/

  typedef struct 
  {
    LSA_BOOL start;
    LSA_BOOL stop;
    LSA_BOOL configured;
    LSA_BOOL running;
    LSA_BOOL paramChanged;
    LSA_BOOL rpChanged;
    LSA_BOOL roleChangeRestricted;
    LSA_UINT32 topoCtrl;
  }MRP_REC_MODULE_STATE_TYPE;

/*---------------------------------------------------------------------------*/
/*         IN module state struct                                               */
/*---------------------------------------------------------------------------*/

  typedef struct 
  {
    /* To avoid compiler errors (EDD_CFG_MAX_MRP_IN_INSTANCE_CNT + 1): in case of EDD_CFG_MAX_MRP_IN_INSTANCE_CNT = 0 the array has at least the size of 1! Still all internal checks are done with EDD_CFG_MAX_MRP_IN_INSTANCE_CNT. */
    MRP_REC_MODULE_STATE_TYPE moduleStateMIC[EDD_CFG_MAX_MRP_IN_INSTANCE_CNT + 1];
    MRP_REC_MODULE_STATE_TYPE moduleStateMIM[EDD_CFG_MAX_MRP_IN_INSTANCE_CNT + 1];
  }MRP_REC_IN_MODULE_STATE_TYPE;

/*---------------------------------------------------------------------------*/
/*         record data blocktypes                                            */
/*---------------------------------------------------------------------------*/
 /* BlockType: MrpMgrParams (opt.) */
  typedef struct 
  {
    LSA_BOOL   Written;
    LSA_UINT32 err_offset_base;
    
    LSA_UINT16 Prio;
    LSA_UINT16 TOPchgT_10ms;
    LSA_UINT16 TOPNRmax;
    LSA_UINT16 TSTshortTms;
    LSA_UINT16 TSTdefaultTms;
    LSA_UINT16 TSTNRmax;
  }MRP_MGR_PARAMS_TYPE;

  /* BlockType: MrpClientParams (opt.) */
  typedef struct 
  {
    LSA_BOOL   Written;
    LSA_UINT32 err_offset_base;
    
    LSA_UINT16 LNKdownT_ms;
    LSA_UINT16 LNKupT_ms;
    LSA_UINT16 LNKNRmax;
  }MRP_CLIENT_PARAMS_TYPE;

  /* BlockType: MrpRTModeManagerData (opt.) */
  typedef struct 
  {
    LSA_BOOL   Written;
    
    LSA_UINT32 RTMode;
  }MRP_RT_MGR_PARAMS_TYPE;

  /* BlockType: MrpRTModeClientData (opt.) */
  typedef struct 
  {
    LSA_BOOL   Written;
    
    LSA_UINT32 RTMode;
  }MRP_RT_CLIENT_PARAMS_TYPE;

  /* BlockType: MrpMimParams (opt.) */
  typedef struct
  {
    LSA_BOOL   Written;

    LSA_UINT16 TOPchgT_10ms; 
    LSA_UINT16 TOPNRmax;
    LSA_UINT16 LinkStatusChangeT_ms;
    LSA_UINT16 LinkStatusNRmax;
    LSA_UINT16 StartDelay_100ms;
    LSA_UINT16 Padding;
  }MRP_IC_MIM_PARAMS_TYPE;

  /* BlockType: MrpMicParams (opt.) */
  typedef struct
  {
    LSA_BOOL   Written;

    LSA_UINT16 LNKdownT_ms;
    LSA_UINT16 LNKupT_ms;
    LSA_UINT16 LNKNRmax;
    LSA_UINT16 StartDelay_100ms;
    LSA_UINT16 MICPosition;
    LSA_UINT16 Padding;
  }MRP_IC_MIC_PARAMS_TYPE;

/*---------------------------------------------------------------------------*/
/*    block header definitions                                               */
/*---------------------------------------------------------------------------*/

  typedef union
  {
    struct  
    {
      LSA_UINT8  dummy;
    }v_old;
    struct  
    {
      LSA_UINT16 blockType;          
      LSA_UINT16 blockLength;        
      LSA_UINT8  blockVersionHigh;
      LSA_UINT8  blockVersionLow;
      LSA_UINT8  padding;
      LSA_UINT8  instance;  /* if blockVersionLow = 1: 1...maxMrpInstances */
    }v_new;
  }MRP_INST_DATA_VERS_DEPENDENT_TYPE;

  typedef struct  
  {
    MRP_INST_DATA_VERS_DEPENDENT_TYPE v;
  }MRP_INST_DATA_HEADER_TYPE;
  
  
  typedef union
  {
    struct  
    {
      LSA_UINT8  padding1;
      LSA_UINT8  padding2;  /* if blockVersionLow = 0: 0*/  
    }v_old;
    struct  
    {
      LSA_UINT8  padding;
      LSA_UINT8  numOfMrpInstances;  /* if blockVersionLow = 1: 1...maxMrpInstances */
    }v_new;
  }MRP_IF_DATA_VERS_DEPENDENT_TYPE;
  
  typedef struct  
  {
    LSA_UINT16 blockType;          
    LSA_UINT16 blockLength;        
    LSA_UINT8  blockVersionHigh;
    LSA_UINT8  blockVersionLow;
    MRP_IF_DATA_VERS_DEPENDENT_TYPE v;
  }MRP_IF_DATA_HEADER_TYPE;

/*---------------------------------------------------------------------------*/
/*      dataset definitions                                                  */
/*---------------------------------------------------------------------------*/
  
  /* BlockType: PDPortMrpDataAdjust (blockType = 0xx0214, mandatory) */
  typedef struct 
  {
    LSA_BOOL   Written;

    MRP_INST_DATA_HEADER_TYPE head;
    LSA_UINT16 PortID;
    LSA_UINT8  DomainUUID[16];
  }MRP_PORT_DATA_ADJUST_TYPE;  
  
  
  /* BlockType: MrpInstanceDataCheck (blockType = 0x0233, optional) */
  typedef struct 
  {
    LSA_BOOL   Written;
    LSA_UINT32 err_offset_base;

    MRP_INST_DATA_HEADER_TYPE head;
    LSA_UINT8  DomainUUID[16];
    LSA_UINT32 Check;
  }MRP_INST_DATA_CHECK_TYPE;

  
  /* BlockType: MrpInstanceDataAdjust (blockType = 0x0231) */
  typedef struct 
  {
    LSA_BOOL   Written;
    LSA_UINT32 err_offset_base;

    MRP_INST_DATA_HEADER_TYPE head;

    LSA_UINT8  DomainUUID[16];
    LSA_UINT16 Role;
    LSA_UINT8  LengthDomainName;
    LSA_UINT8  DomainName[MRP_DEFAULT_DOMAIN_NAME_SIZE_MAX + 1];
    LSA_UINT8  LengthDomainNamePadding;
    MRP_MGR_PARAMS_TYPE mgr;
    MRP_CLIENT_PARAMS_TYPE client;
  }MRP_INST_DATA_ADJUST_TYPE;


  /* BlockType: Header of PDInterfaceMrpDataCheck-DS (blockType= 0x213, optional) */
  typedef struct 
  {
    LSA_BOOL   Written;

    MRP_IF_DATA_HEADER_TYPE head;
  }MRP_IF_DATA_CHECK_HEAD_TYPE;

  /* BlockType: Header of PDInterfaceMrpDataAdjust-DS (blockType= 0x211, mandatory) */
  typedef struct 
  {
    LSA_BOOL   Written;

    MRP_IF_DATA_HEADER_TYPE head;
  }MRP_IF_DATA_ADJUST_HEAD_TYPE;


  /* BlockType: PDPortMrpIcDataAdjust (blockType = 0xx0234, mandatory) */
  typedef struct
  {
    LSA_BOOL   Written;
    LSA_BOOL   InUse;    /* Flag if data-set was already copied from "para" data-set to "working" data-set. */

    MRP_INST_DATA_HEADER_TYPE head;

    LSA_UINT16 PortID;
    LSA_UINT16 DomainID;
    LSA_UINT16 Role;
    LSA_UINT8  LengthDomainName;
    LSA_UINT8  DomainName[MRP_DEFAULT_DOMAIN_NAME_SIZE_MAX + 1];
    LSA_UINT8  LengthDomainNamePadding;
    MRP_IC_MIM_PARAMS_TYPE mim;
    MRP_IC_MIC_PARAMS_TYPE mic;
  }MRP_IC_PORT_DATA_ADJUST_TYPE;

  /* BlockType: PDPortMrpIcDataCheck (blockType = 0xx0235, optional) */
  typedef struct
  {
    LSA_BOOL   Written;

    MRP_INST_DATA_HEADER_TYPE head;

    LSA_UINT16 PortID;
    LSA_UINT32 Check;
  }MRP_IC_PORT_DATA_CHECK_TYPE;

/*---------------------------------------------------------------------------*/
/*     private dataset definitions                                           */
/*---------------------------------------------------------------------------*/

#ifdef MRP_CFG_PLUGIN_2_HSR
  /* BlockType: PrivInterfaceHsrDataAdjust (private) */
  typedef struct 
  {
    LSA_BOOL   Written;
    
    LSA_UINT16 Role;
    LSA_UINT16 PortA;
    LSA_UINT16 PortB;
  }MRP_IF_HSR_ADJUST_TYPE;

  /* BlockType: PrivInterfaceStdbyDataAdjust (private) */
  typedef struct 
  {
    LSA_BOOL   Written;
    
    LSA_UINT8  LengthConName;
    LSA_UINT8  LengthConNamePadding;
    LSA_UINT8  ConName[MRP_STDBY_NAME_SIZE_MAX + 1];     /* + EOS */
    LSA_UINT8  PortCnt;
    LSA_UINT8  PortCntPadding;
    LSA_UINT8  *pPortID;
    LSA_UINT32 Capability;
    LSA_UINT32 WaitForPartner;
  }MRP_IF_STBY_ADJUST_TYPE;
#endif

  /* BlockType: PrivMrpDataAdjust (private) */
  typedef struct 
  {
    LSA_BOOL   Written;
    
    LSA_UINT16 MONdefaultT_ms;
    LSA_UINT16 MONNRmax;
  }MRP_IF_PRV_DATA_ADJUST_TYPE;


/*---------------------------------------------------------------------------*/
/*      alarm  definitions                                                   */
/*---------------------------------------------------------------------------*/
  /* alarm data */
  typedef struct 
  {
    LSA_BOOL   state;
    LSA_BOOL   preState;
    LSA_UINT16 property;
  }MRP_ALARM_TYPE;

  typedef struct 
  {
    MRP_ALARM_TYPE roleClient; /* not in use any longer as diag alarm */
    MRP_ALARM_TYPE morphing;
    MRP_ALARM_TYPE singleSideRx;
    MRP_ALARM_TYPE ringOpen;
    MRP_ALARM_TYPE multiRM;
  }MRP_ALARM_DB_TYPE;

/*---------------------------------------------------------------------------*/
/*     instance related record DB definitions                                */
/*---------------------------------------------------------------------------*/
  typedef struct 
  {
    LSA_UINT16 RPort_1; /* RPort helper */
    LSA_UINT16 RPort_2; /* RPort helper */
    MRP_ADMIN_ROLE_ENUM_T adminRole;
    MRP_INST_DATA_ADJUST_TYPE RecInstanceAdjust;
    MRP_INST_DATA_CHECK_TYPE RecInstanceCheck;
    MRP_INST_DATA_CHECK_TYPE RecInstanceUuidCheck;
    MRP_PORT_DATA_ADJUST_TYPE *RecPortAdjust;
    
#ifdef MRP_CFG_PLUGIN_2_HSR
    MRP_IF_HSR_ADJUST_TYPE RecPrivHsrAdjust;
    MRP_IF_STBY_ADJUST_TYPE RecPrivStByAdjust;
#endif
#ifdef MRP_CFG_PLUGIN_1_MRM_EXT
    MRP_IF_PRV_DATA_ADJUST_TYPE RecPrivDataAdjust;
#endif

    MRP_IC_PORT_DATA_ADJUST_TYPE *pRecIcPortAdjust;
    MRP_IC_PORT_DATA_CHECK_TYPE *pRecIcPortCheck;


  }MRP_REC_DB_INST_SET_TYPE;

  typedef struct  
  {
    /* para record-database-set */
    MRP_REC_DB_INST_SET_TYPE para;
    /* working database-set */
    MRP_REC_DB_INST_SET_TYPE work;
    /* last active working database, in case of role change delay (morphing), this database has to remain active till role has changed */
    MRP_REC_DB_INST_SET_TYPE saveWork;

    /* alarms */
    MRP_ALARM_DB_TYPE Alarm;
    MRP_ALARM_DB_TYPE AlarmIndicated; /* last indicated alarm to CMPD */
    LSA_BOOL AlarmBlocked;

    LSA_UINT16 TimerIdAlarm;
    LSA_UINT16 TimerAlarmTimeTcnt;

    LSA_UINT32 activeRedModules;

  }MRP_REC_INST_DATA_TYPE;

/*---------------------------------------------------------------------------*/
/*     interface related record DB definitions                               */
/*---------------------------------------------------------------------------*/
  typedef struct 
  {
    MRP_IF_DATA_ADJUST_HEAD_TYPE RecIfAdjustHead;
    MRP_IF_DATA_CHECK_HEAD_TYPE  RecIfCheckHead;  
  }MRP_REC_DB_IF_SET_TYPE;


  typedef enum 
  {
    MRP_REC_ENUM_STATE_PRM_INIT,
    MRP_REC_ENUM_STATE_PRM_PREPARE,
    MRP_REC_ENUM_STATE_PRM_WRITE,
    MRP_REC_ENUM_STATE_PRM_END,
    MRP_REC_ENUM_STATE_PRM_COMMIT_START,
    MRP_REC_ENUM_STATE_PRM_COMMIT,
  }MRP_REC_ENUM_STATE_PRM_TYPE;

  /* db */
  typedef struct _MRP_REC_DB_TYPE
  {
    /* channel references */
    MRP_CHANNEL_TYPE_SYSTEM pChSys;
    MRP_CHANNEL_TYPE_USER pChPrmUsr;
    
    /* queuing of alarm resources */
    MRP_RQB_TYPE **ppRqbIndicationBot;
    MRP_RQB_TYPE **ppRqbIndicationTop;

    LSA_UINT8 *pAlarmOutOfResource;
    
    /* common values for record-database-set */
    LSA_UINT16 NumOfPossibleRports;
    LSA_UINT16 NumOfPossibleInPorts;
    LSA_UINT16 *pRPortArray;
    LSA_UINT16 *pInPortArray;

    LSA_BOOL   PrmEnd;
    LSA_BOOL   firstPrmCommit;

    LSA_UINT16 PrmState;

    /* temporary values, used for PRM_WRITE checks */
    LSA_UINT16 tmpParsedProjRole;   /* this value is only valid during parsing of PRM_WRITE datasets */
    LSA_UINT8 *tmpParsedBaseAddr;   /* this value is only valid during parsing of PRM_WRITE datasets */

    /* defaults */
    LSA_UINT16 DefaultRPort_1;
    LSA_UINT16 DefaultRPort_2;
    LSA_UINT16 DefaultRoleInst0;
    LSA_UINT16 SupportedRoleInst0;
    LSA_UINT16 SupportedMultiRole;
    LSA_UINT16 DefaultPrio;
    LSA_UINT16 SupportedInRoleInst0;

    MRP_ALARM_TYPE commitCnf;
    MRP_ALARM_TYPE commitCnfIndicated;

    /* para record-database-set */
    MRP_REC_DB_IF_SET_TYPE para;
    /* working database-set */
    MRP_REC_DB_IF_SET_TYPE work;
    /* last active working database, in case of role change delay (morphing), this database has to remain active till role has changed */
    MRP_REC_DB_IF_SET_TYPE saveWork;

    /* instance related data */
    MRP_REC_INST_DATA_TYPE *pInstData;
    
  }MRP_REC_DB_TYPE;


  /*---------------------------------------------------------------------------*/
  /*                            forward declarations                           */
  /*---------------------------------------------------------------------------*/
  LSA_EXTERN LSA_VOID mrp_mrc_ac_stat1 (MRP_MRC_DB_TYPE *pDBmrc);
  LSA_EXTERN LSA_VOID mrp_mrc_de_idle (MRP_MRC_DB_TYPE *pDBmrc);
  LSA_EXTERN LSA_VOID mrp_mrc_pt (MRP_MRC_DB_TYPE *pDBmrc);
  LSA_EXTERN LSA_VOID mrp_mrc_de (MRP_MRC_DB_TYPE *pDBmrc);
  LSA_EXTERN LSA_VOID mrp_mrc_pt_idle (MRP_MRC_DB_TYPE *pDBmrc);



  /*===========================================================================*/
  /*                                   prototypes                              */
  /*===========================================================================*/


  /*---------------------------------------------------------------------------*/
  /*                           prototypes for mrp_sys.c                        */
  /*---------------------------------------------------------------------------*/
  LSA_HANDLE_TYPE mrp_sys_get_sys_ch_handle (const MRP_CONST_CHANNEL_TYPE_COMMON pCh);
  MRP_CHANNEL_TYPE_COMMON mrp_sys_get_pCh (const LSA_HANDLE_TYPE handle);
  LSA_UINT16 mrp_sys_get_edd_sys_channel_cnt (LSA_VOID);
  LSA_VOID mrp_sys_get_mrp_mode (const MRP_CONST_CHANNEL_TYPE_COMMON pCh, LSA_UINT16 instance, MRP_MODE_DATA_TYPE *pData);
  LSA_UINT8 mrp_sys_get_ed2_flag (const MRP_CONST_CHANNEL_TYPE_COMMON pCh);
  LSA_UINT16 mrp_sys_check_edd_details (const MRP_CONST_DETAIL_PTR_TYPE tmpDetailPtr, LSA_UINT16 *newEntry);
  LSA_UINT32 mrp_sys_map_timer_id_to_if_id (const LSA_UINT16 userId);
  LSA_UINT32 mrp_sys_cyclic_get_curr_time (void);
  MRP_CHANNEL_TYPE_SYSTEM mrp_sys_get_sys_ch_by_if_id (const LSA_UINT32 pnioInterfaceID);
  LSA_UINT32 mrp_sys_timer_get_elapsed_ticks (const LSA_UINT16 id);

  LSA_VOID mrp_sys_cyclic_timer_cbf (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, LSA_USER_ID_TYPE uid);
  LSA_VOID mrp_sys_timer_request (const MRP_CONST_CHANNEL_TYPE_COMMON pCh, const MRP_CONST_UPPER_RQB_PTR_TYPE pRQB);

  LSA_UINT16 mrp_sys_cyclic_timer_alloc (LSA_UINT16 type);
  LSA_UINT16 mrp_sys_timer_get (const MRP_CONST_CHANNEL_TYPE_COMMON pCh, const MRP_TIMER_POOL timerPool, LSA_UINT16 instance, LSA_UINT16 type, MRP_ENUM_TIMER_USE_T purpose, MRP_TIMER_CBF_FCT_PTR_TYPE pCbf);

  LSA_VOID mrp_sys_cyclic_timer_free (LSA_UINT16 id);
  LSA_VOID mrp_sys_timer_free (LSA_UINT16 id);

  LSA_VOID mrp_sys_cyclic_timer_start (LSA_UINT16 id, LSA_UINT32 durationTicks);
  LSA_VOID mrp_sys_timer_start (LSA_UINT16 id, LSA_UINT32 durationTicks);

  LSA_VOID mrp_sys_cyclic_timer_stop (LSA_UINT16 id);
  LSA_VOID mrp_sys_timer_stop (LSA_UINT16 id);

  LSA_UINT8 mrp_sys_cyclic_timer_state_check (LSA_UINT16 id);
  MRP_TIMER_STS_T mrp_sys_cyclic_timer_get_state (LSA_UINT16 id);
  MRP_TIMER_STS_T mrp_sys_timer_get_state (LSA_UINT16 id);
  LSA_UINT32 mrp_sys_timer_get_time_diff (LSA_UINT32 startTime, LSA_UINT32 endTime);

  LSA_UINT16 mrp_sys_map_port_to_instance (const MRP_CONST_CHANNEL_TYPE_COMMON pCh, LSA_UINT16 port);
  LSA_UINT16 mrp_sys_map_rcv_frame_to_instance (const MRP_CONST_CHANNEL_TYPE_COMMON pCh, LSA_UINT16 port, LSA_UINT8 *pBuffer);
  LSA_UINT16 mrp_sys_get_num_of_ports (const MRP_CONST_CHANNEL_TYPE_COMMON pCh);

  LSA_UINT16 mrp_sys_get_num_of_instances (const MRP_CONST_CHANNEL_TYPE_COMMON pCh);
  
  LSA_UINT16 mrp_sys_get_inport_cnt (const MRP_CONST_CHANNEL_TYPE_USER pChUsr);

  LSA_VOID mrp_sys_calc_ha_admin_role (const MRP_CONST_CHANNEL_TYPE_COMMON pCh);
  MRP_HA_ADMIN_ROLE_ENUM_T mrp_sys_get_ha_admin_role (const MRP_CONST_CHANNEL_TYPE_COMMON pCh);

  LSA_VOID mrp_sys_timer_init (const MRP_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance);
  LSA_VOID mrp_sys_timer_deinit (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys);

  LSA_UINT16 mrp_sys_if_db_init (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys);
  LSA_VOID mrp_sys_if_db_deinit (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys);

  LSA_UINT16 mrp_sys_ch_db_init (const MRP_CHANNEL_TYPE_SYSTEM pChSys);
  LSA_VOID mrp_sys_ch_db_deinit(const MRP_CHANNEL_TYPE_SYSTEM pChSys);

  LSA_VOID mrp_sys_init (const MRP_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance);
  LSA_VOID mrp_sys_deinit (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance);


  /*---------------------------------------------------------------------------*/
  /*                           prototypes for mrp_usr.c                        */
  /*---------------------------------------------------------------------------*/
  LSA_VOID mrp_usr_call_upper_cbf (const MRP_CONST_CHANNEL_TYPE_COMMON pCh, MRP_UPPER_RQB_PTR_TYPE pRQB);
  LSA_VOID mrp_usr_call_upper_cbf_proc_next_req (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB);
  
  MRP_CHANNEL_TYPE_USER mrp_usr_get_prm_usr_channel (const MRP_CONST_DETAIL_PTR_TYPE tmpDetailPtr);

  LSA_UINT8 mrp_usr_state_ind_done (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance);
  LSA_UINT16 mrp_usr_state_ind_provide_rqb (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB);

  LSA_UINT8 mrp_usr_red_state_ind_done (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance);
  LSA_UINT16 mrp_usr_red_state_ind_provide_rqb (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB);

  LSA_VOID mrp_usr_red_state_init (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance);
  LSA_VOID mrp_usr_red_state_deinit (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance);
  LSA_VOID mrp_usr_write_red_init_state (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance, MRP_USR_ENUM_RED_STATE_TYPE initState, LSA_UINT8 setFlag);
  LSA_VOID mrp_usr_write_red_start_state (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_USR_ENUM_RED_STATE_TYPE startState, LSA_UINT16 rport1, LSA_UINT16 rport2, const LSA_UINT8 *pStdbyPorts, LSA_UINT8 setFlag);

#ifdef MRP_CFG_PLUGIN_2_HSR 
  LSA_UINT8 mrp_usr_ind_hsr_provide_done (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance);
  LSA_UINT16 mrp_usr_ind_hsr_provide (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB);
#endif /* MRP_CFG_PLUGIN_2_HSR */

  LSA_VOID mrp_usr_release_channel_data (const MRP_CHANNEL_TYPE_COMMON pCh);
  LSA_VOID mrp_usr_finish_close_channel (const MRP_CHANNEL_TYPE_USER pChUsr);

  LSA_UINT16 mrp_usr_close_channel (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB);
  LSA_UINT16 mrp_usr_open_channel (const MRP_CHANNEL_TYPE_USER pChUsr, const MRP_CONST_CHANNEL_TYPE_USER pChPrmUsr);

  LSA_VOID mrp_usr_db_init (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance);
  LSA_VOID mrp_usr_db_deinit (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance);

  /*---------------------------------------------------------------------------*/
  /*                           prototypes for mrp_mrc.c                        */
  /*---------------------------------------------------------------------------*/
  LSA_VOID mrp_mrc_get_params (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_MRC_PARAMS_TYPE *pParams);
  LSA_VOID mrp_mrc_get_module_state (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance, MRP_REC_MODULE_STATE_TYPE *pModuleState);
  
  LSA_VOID mrp_mrc_stop (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance);
  LSA_VOID mrp_mrc_start (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_MRC_PARAMS_TYPE *pParams);

  LSA_VOID mrp_mrc_init_snd_rqb (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpMaxInstance);
  LSA_VOID mrp_mrc_init (const MRP_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance);
  LSA_VOID mrp_mrc_deinit (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance);

  /*---------------------------------------------------------------------------*/
  /*                           prototypes for mrp_oha.c                        */
  /*---------------------------------------------------------------------------*/
  LSA_UINT16 mrp_oha_get_status (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB);
  LSA_UINT16 mrp_oha_set_status (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB);
  LSA_UINT16 mrp_oha_ringstate_ind_provide_rqb (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB);

  LSA_VOID mrp_oha_write_current_ringstate_data (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance, MRP_USR_ENUM_TOPO_CTRL topoCtrl);
  LSA_UINT8 mrp_oha_ringstate_ind_done (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance);

  LSA_VOID mrp_oha_db_init (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 mrpMaxInstance);
  LSA_VOID mrp_oha_db_deinit (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 mrpMaxInstance);

  LSA_VOID mrp_oha_multi_usr_write_curr_ringstate_data (const MRP_CONST_CHANNEL_TYPE_USER pChSys, LSA_UINT16 instance, MRP_USR_ENUM_TOPO_CTRL topoCtrl);
  LSA_VOID mrp_oha_multi_usr_ringstate_ind_done (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance);

  MRP_MIB_ENUM_RPORT_STATE mrp_mib_map_edd_and_link_state_to_mib_state (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT16 rPort, LSA_UINT16 eddPortState, LSA_UINT8 linkState);
  LSA_VOID mrp_mib_write_link_state (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT16 rPort, LSA_UINT8 linkState);
  LSA_VOID mrp_mib_write_rports (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT16 rPort1, LSA_UINT16 rPort2);
  LSA_VOID mrp_mib_write_domain_state (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_MIB_ENUM_DOMAIN_STATE domainState);
  LSA_VOID mrp_mib_write_domain_error (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_MIB_ENUM_DOMAIN_ERROR domainError, LSA_UINT8 setFlag);
  LSA_VOID mrp_mib_write_domain_name (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT8 *pDomainName, LSA_UINT8 lengthDomainName);
  LSA_UINT8 mrp_mib_write_vlanid (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, const LSA_UINT32 vlanIn);
  LSA_VOID mrp_mib_write_domain_uuid (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT8 *pDomain);
  LSA_UINT8 mrp_mib_chk_currentResetRoundTripDelayFlag (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance);
  LSA_VOID mrp_mib_reset_round_trip_delays (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT32 resetDelays);
  LSA_VOID mrp_mib_write_round_trip_delays (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT32 delayMax, LSA_UINT32 delayMin);
  LSA_VOID mrp_mib_write_mrm_prio (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT16 mrmPrio);
  LSA_VOID mrp_mib_write_ringopen_cnt (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT16 ringOpenCnt);
  LSA_VOID mrp_mib_calc_ringopen_time (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance);
  LSA_VOID mrp_mib_write_admin_role (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_MIB_ENUM_DOMAIN_ROLE adminRole);
  LSA_VOID mrp_mib_write_oper_role (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_MIB_ENUM_DOMAIN_ROLE operRole);
  LSA_VOID mrp_mib_init_mrm_var (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance);

  LSA_VOID mrp_mib_db_init (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpMaxInstance);
  LSA_VOID mrp_mib_db_deinit (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpMaxInstance);


  /*---------------------------------------------------------------------------*/
  /*                           prototypes for mrp_rec.c                        */
  /*---------------------------------------------------------------------------*/
  MRP_ADMIN_ROLE_ENUM_T mrp_rec_map_proj_role_to_admin_role (const MRP_CONST_CHANNEL_TYPE_COMMON pCh, const LSA_UINT8 isNewProj, const MRP_PROJ_ROLE_ENUM_T projRole);
  MRP_PROJ_ROLE_ENUM_T mrp_rec_map_admin_role_to_proj_role (const MRP_CONST_CHANNEL_TYPE_COMMON pCh, const LSA_UINT8 isNewProj, const MRP_ADMIN_ROLE_ENUM_T adminRole);


  LSA_UINT16 mrp_rec_check_prm_vers_consistency (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, MRP_PRM_END_TYPE *pPrmEndRsp);
  LSA_UINT16 mrp_rec_check_prm_uuid_consistency (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, MRP_PRM_END_TYPE *pPrmEndRsp);
  LSA_UINT16 mrp_rec_check_prm_consistency (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, MRP_PRM_END_TYPE *pPrmEndRsp);
  
  LSA_UINT16 mrp_rec_prm_prepare (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB);
  LSA_UINT16 mrp_rec_prm_end (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB);
  LSA_VOID mrp_rec_prm_commit (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB);
  LSA_UINT16 mrp_rec_prm_change_port (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB);
  LSA_UINT16 mrp_rec_prm_read (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB);
  LSA_UINT16 mrp_rec_prm_write (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB);
  
  LSA_VOID mrp_rec_stop_modules (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance, MRP_REC_MODULE_STATE_TYPE *pModuleState, MRP_REC_IN_MODULE_STATE_TYPE *pInModuleState, LSA_UINT32 *pRunningModules);
  LSA_UINT32 mrp_rec_activate_role (const MRP_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance);
  LSA_VOID mrp_rec_restore_saved_settings (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance);
  LSA_VOID mrp_rec_restore_alarm_settings (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance);
  
  LSA_UINT16 mrp_rec_prm_ind_provide_rqb (const MRP_CHANNEL_TYPE_USER pChUsr, MRP_UPPER_RQB_PTR_TYPE pRQB);
  LSA_UINT8 mrp_rec_prm_ind_done (const MRP_CONST_CHANNEL_TYPE_USER pChUsr);
  LSA_UINT8 mrp_rec_prm_ind_port_done (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 portID);
  LSA_VOID mrp_rec_prm_ind_commit_cnf_done (const MRP_CONST_CHANNEL_TYPE_USER pChUsr);
  LSA_VOID mrp_rec_event_req (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance);
  
  LSA_VOID mrp_rec_init (const MRP_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance);
  LSA_VOID mrp_rec_deinit (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance);
  

#ifdef MRP_CFG_PLUGIN_0_MRM
#include "mrp0mrm.h"
#endif
#ifdef MRP_CFG_PLUGIN_1_MRM_EXT
#include "mrp1mrm.h"
#endif
#ifdef MRP_CFG_PLUGIN_2_HSR
#include "mrp2hsr.h"
#endif

/* MRP_IN: RSTP+/MRP interconnection structures */
#include "mrp_inbas.h"

  /*----------------------------------------------------------------------------*/
#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif


#endif  /* of MRP_INT_H */
