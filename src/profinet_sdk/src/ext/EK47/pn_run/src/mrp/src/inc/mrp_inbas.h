#ifndef MRP_INBAS_H
#define MRP_INBAS_H

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
/*  F i l e               &F: mrp_inbas.h                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Basic definitions for MRP interconnection                                */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef MRP_MESSAGE   
/*                                                                           */
/*  2017-05-30  ds    File mrp_inbas.h introduced                            */
/*  2017-11-17  ds    Definitions for MIM and MIC introduced                 */
/*  2018-10-19  hh    introduction of N MRP interconnection instances        */
#endif
/*****************************************************************************/




/*---------------------------------------------------------------------------*/
/*                                 defines                                   */
/*---------------------------------------------------------------------------*/
#define MRP_IN_DEFAULT_VAL_IN_TC_NR_MAX                 4
#define MRP_IN_DEFAULT_VAL_IN_TC_CHG_T_CNT              2 /* to reduce frame load of InTC frames, interval is increased from 10 ms -> 20 ms*/
#define MRP_IN_DEFAULT_VAL_IN_TC_CHG_T_RES_MS          10 

#define MRP_IN_DEFAULT_VAL_IN_TC_CHG_DEFAULT_T_MS      20 /* to reduce frame load of InTC frames, interval is increased from 10 ms -> 20 ms*/

#define MRP_IN_DEFAULT_VAL_IN_ADD_POLL_NR_MAX           4

#define MRP_IN_DEFAULT_VAL_IN_POLL_NR_MAX               8
#define MRP_IN_DEFAULT_VAL_IN_POLL_CHG_T_CNT           20
#define MRP_IN_DEFAULT_VAL_IN_POLL_CHG_T_RES_MS        10

#define MRP_IN_DEFAULT_VAL_IN_POLL_DEFAULT_T_20MS      20
#define MRP_IN_DEFAULT_VAL_IN_POLL_SLOW_T_100MS       100
  
/* Additional InPoll requests: interlock time interval to avoid unnecessary InPoll requests (e.g. if MRP ring reconfiguration is already going on) */
#define MRP_IN_POLL_RETRIGGER_DELAY_MS                 80

#define MRP_IN_DEFAULT_VAL_IN_LNK_CHG_NR_MAX            4
#define MRP_IN_DEFAULT_VAL_IN_LNK_CHG_T_CNT            20
#define MRP_IN_DEFAULT_VAL_IN_LNK_CHG_T_RES_MS         10

#define MRP_IN_DEFAULT_VAL_IN_LNK_CHG_DEFAULT_T_20MS   20
#define MRP_IN_DEFAULT_VAL_IN_LNK_CHG_DEFAULT_T_100MS 100


#define MRP_IN_DEFAULT_VAL_IN_START_DELAY_T_CNT         2
#define MRP_IN_DEFAULT_VAL_IN_START_DELAY_T_RES_MS    100


#define MRP_IN_DEFAULT_VAL_START_DELAY_DEFAULT_T_MS   100
#define MRP_IN_DEFAULT_VAL_START_DELAY_ENDLESS     0xFFFF


#define MRP_IN_DEFAULT_VAL_IN_PRM_MIC_POS               0

#define MRP_IN_VAL_IN_MIC_POS_PRM_LNK                   0
#define MRP_IN_VAL_IN_MIC_POS_SEC_LNK                   1

#define MRP_IN_START_DELAY_LOW_LIMIT                    2
#define MRP_IN_START_DELAY_MEDIUM_LIMIT_LOW             3
#define MRP_IN_START_DELAY_MEDIUM_LIMIT_HIGH        60000
#define MRP_IN_START_DELAY_ENDLESS                  65535

   
#define MRP_IN_INSTANCE_NONE                       0xFFFF
#define MRP_IN_INSTANCE_FOREIGN_IN_ID              0xFFFE
#define MRP_IN_INSTANCE_DEFAULT                         0
#define MRP_IN_DEFAULT_VAL_IN_ID                   0xFFFF

/*---------------------------------------------------------------------------*/
/* defines for MRP interconnection open signaling                            */
/*---------------------------------------------------------------------------*/
#define MRP_IN_RED_LINK_IN_OPEN_PRM               0
#define MRP_IN_RED_LINK_IN_OPEN_SEC               1

/*---------------------------------------------------------------------------*/
/*   MRP timer values: time in ms = XXX_T_CNT * MRP_TIMER_TIME_BASE_DEZ      */
/*---------------------------------------------------------------------------*/
#define  MRP_IN_VAL_LINK_UP_IPORT_T_CNT              500

/*---------------------------------------------------------------------------*/
/*                                 types                                     */
/*---------------------------------------------------------------------------*/
  typedef enum _MRP_IN_EVENT_TYPE
  {
    MRP_IN_EVENT_T_NONE
    ,MRP_IN_EVENT_T_MAU_TYPE_CHG
    ,MRP_IN_EVENT_T_TPLG_CHG_WRK
    ,MRP_IN_EVENT_T_TPLG_CHG_TRF
    ,MRP_IN_EVENT_T_POLL_WRK
    ,MRP_IN_EVENT_T_POLL_TRF
    ,MRP_IN_EVENT_T_LNK_CHG_WRK
    ,MRP_IN_EVENT_T_LNK_CHG_TRF
    ,MRP_IN_EVENT_T_LNK_CHG_DOWN_TIMER
    ,MRP_IN_EVENT_T_LNK_CHG_UP_TIMER
    ,MRP_IN_EVENT_T_LNK_POLL_TIMER
    ,MRP_IN_EVENT_T_IN_TC_TIMER
    ,MRP_IN_EVENT_T_IN_TC_RSTPP_TIMER
    ,MRP_IN_EVENT_T_MRP_CHG_IND
  }MRP_IN_EVENT_TYPE;


  typedef enum _MRP_IN_ADMIN_ROLE_ENUM_T
  {
    MRP_IN_ADMIN_ROLE_OFF
    ,MRP_IN_ADMIN_ROLE_CLIENT
    ,MRP_IN_ADMIN_ROLE_MGR
  }MRP_IN_ADMIN_ROLE_ENUM_T;


  typedef enum _MRP_IN_PROJ_ROLE_ENUM_T
  {
    MRP_IN_PROJ_ROLE_OFF
    ,MRP_IN_PROJ_ROLE_CLIENT
    ,MRP_IN_PROJ_ROLE_MGR
  }MRP_IN_PROJ_ROLE_ENUM_T;

  typedef enum _MRP_INSTANCE_INDEX_ENUM_T
  {
    MRP_INSTANCE_INDEX = 0
    ,MRP_IN_INSTANCE_INDEX
  }MRP_INSTANCE_INDEX_ENUM_T;

  typedef struct _MRP_IN_TC_REQ_INFO_TYPE
  {
    MRP_IN_FRAME_TYPE inFrameType;
    LSA_UINT16        inId;
    LSA_UINT16        rxPort;

    LSA_UINT16        intervalTimeMs;
  }MRP_IN_TC_REQ_INFO_TYPE;



  /*---------------------------------------------------------------------------*/
  /* MRP interconnection resp. MRP_RSTP+ database                              */
  /*---------------------------------------------------------------------------*/

  typedef struct _MRP_IN_DB_TYPE MRP_IN_DB_TYPE;
  typedef struct _MRP_IN_MIC_DB_TYPE MRP_IN_MIC_DB_TYPE;
  typedef struct _MRP_IN_MIM_DB_TYPE MRP_IN_MIM_DB_TYPE;
  typedef struct _MRP_IN_EVENT_DB_TYPE MRP_IN_EVENT_DB_TYPE;


  /*---------------------------------------------------------------------------*/
  /*   CBF typedef for MIC                                                     */
  /*---------------------------------------------------------------------------*/
  typedef LSA_VOID   LSA_FCT_PTR (/*ATTR*/, MRP_IN_MIC_FCT_PTR_TYPE)(MRP_IN_DB_TYPE *pDBin);

  /*---------------------------------------------------------------------------*/
  /*  MIC database                                                             */
  /*---------------------------------------------------------------------------*/
  struct _MRP_IN_MIC_DB_TYPE
  {
    /* identifier */
    /* variables */
    LSA_UINT16               runState;

    MRP_CHANNEL_TYPE_USER    pChUsr4NextRole;                    /* next role user channel                 */
                                                                 /* Interconnection topology change params */
    /* record contents of PDPortMrpIcDataAdjust record */
    LSA_UINT16               MRPIC_LNKdownT_ms;
    LSA_UINT16               MRPIC_LNKupT_ms;
    LSA_UINT16               MRPIC_LNKNRmax;
    LSA_UINT16               MRPIC_StartDelay_100ms;
    LSA_UINT16               MRPIC_MICPosition;

    LSA_UINT8                MRPIC_CheckDomainID;

    /* internal SM parameters */
    LSA_UINT16               LNKchgNReturnUp;
    LSA_UINT16               LNKchgNReturnDown;

    LSA_UINT16               LNKNRmaxUp;
    LSA_UINT16               LNKNRmaxDown;

    LSA_UINT16               LNKdownTcnt_10ms;
    LSA_UINT16               LNKupTcnt_10ms;

    LSA_BOOL                 isMorphing;                         /* morphing activated                     */
    LSA_BOOL                 hasToWait4MimFrame;
    LSA_BOOL                 hasReceivedMimFrame;
    LSA_BOOL                 isCoupledMic;                      /* MIC position inherited from InPoll content (portRole field) */

    /* misc */

    /* internal SM event and state flags */
    MRP_IN_EVENT_TYPE        CurrentEvent;                       /* actual work event type                 */
    MRP_IN_MIC_FCT_PTR_TYPE  pInOperStateCbf;                    /* operational state of the MIC           */

                                                                 /* event attributes */
  };

 
  /*---------------------------------------------------------------------------*/
  /*   CBF typedef for MIM                                                    */
  /*---------------------------------------------------------------------------*/
  /* CBF type for the operational state of a MIM  */
  typedef LSA_VOID   LSA_FCT_PTR (/*ATTR*/, MRP_IN_MIM_FCT_PTR_TYPE)(MRP_IN_DB_TYPE *pDBin);



  /*---------------------------------------------------------------------------*/
  /*  MIM database                                                             */
  /*---------------------------------------------------------------------------*/
  struct _MRP_IN_MIM_DB_TYPE
  {
    /* identifier */
    /* variables */
    LSA_UINT16               runState;

    MRP_CHANNEL_TYPE_USER    pChUsr4NextRole;                    /* next role user channel                 */
                                          
    /* record contents of PDPortMrpIcDataAdjust record */
    LSA_UINT16               MRPIC_TOPchgT_10ms;
    LSA_UINT16               MRPIC_TOPNRmax;

    LSA_UINT16               MRPIC_LinkStatusChangeT_ms;
    LSA_UINT16               MRPIC_LinkStatusNRmax;
    LSA_UINT16               MRPIC_StartDelay_100ms;
    LSA_UINT8                MRPIC_CheckDomainID;

    /* internal SM variables */

                                                                 /* Interconnection topology change params */
    LSA_UINT16               TOPintervalTcnt;                    /* interconnection topology change interval time in timer base counts (n * MRP_TIMER_TIME_BASE)*/
    LSA_UINT16               TOPNReturn;

    LSA_UINT16               LNKSTATPollChgTcnt_Proj_10ms;
    LSA_UINT16               LNKSTATPollChgTcnt_Slow_10ms;

    LSA_UINT16               LNKSTATPollNReturnCurrent;
    LSA_UINT16               LNKSTATPollNRmaxCurrent;
    LSA_UINT16               LNKSTATPollTcntCurrent_10ms;

    LSA_BOOL                 addPoll;
    LSA_BOOL                 isMorphing;                         /* morphing activated                     */
    LSA_BOOL                 hasToWait4MicFrame;
    LSA_BOOL                 hasReceivedMicFrame;

    /* misc */

    /* internal SM event and state flags */
    MRP_IN_EVENT_TYPE        CurrentEvent;                       /* actual work event type                 */
    MRP_IN_MIM_FCT_PTR_TYPE  pInOperStateCbf;                    /* operational state of the MIM           */
  };



  /*---------------------------------------------------------------------------*/
  /*  MRP interconnection event database                                       */
  /*---------------------------------------------------------------------------*/
  struct _MRP_IN_EVENT_DB_TYPE
  {
    MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys;                           /* system channel to which this MRP_IN DB belongs */
    LSA_UINT16          CurrentEvent;                               /* actual work event type                 */

    /* event attributes */
    LSA_UINT16          evInstance;                                 /* this is the inInstance!! */

    LSA_UINT32          Vlan;                                       /* VLAN signaled in MRP frames            */

    LSA_UINT16          LinkState;
    MRP_LINK_ADD_INFO   LinkAddInfo;

    LSA_UINT16          RxPortRole;
    LSA_UINT16          RxLnkRole;
    LSA_UINT16          RxPort;                                          /* MRP_IN_EVENT_T_TPLG_CHG(WRK/TRF): RxPort        */
    LSA_UINT16          RxSequenceId;
    LSA_UINT16          RxTimeTcnt;                                      /* MRP_IN_EVENT_T_TPLG_CHG(WRK/TRF): change time = tplgChangeInterval * MRP_TIMER_TIME_BASE */
    LSA_UINT16          RxInId;                                          /* InId of received MRP interconnection frame*/
    LSA_UINT8           RxPeerPortMacAddr[MRP_PACKET_VAL_MAC_ADDR_LEN];  /* port MAC addr of the received MRP interconnection frame (port MAC address of the peer device at the MRP interconnection link */
    LSA_UINT8           RxSA[MRP_PACKET_VAL_MAC_ADDR_LEN];               /* MRP_IN_EVENT_T_TPLG_CHG(WRK/TRF): SA (interface source mac addr), interface MAC address of the received IN_TC */
    LSA_UINT8           RxUUID[MRP_PACKET_VAL_DOMAIN_UUID_LEN];          /* DomainUUID of received frame               */
  };


  /*---------------------------------------------------------------------------*/
  /*  MRP_IN base database                                                     */
  /*---------------------------------------------------------------------------*/
  /* MRP interconnection state machine CBF pointer type */
  typedef LSA_VOID   LSA_FCT_PTR (/*ATTR*/, MRP_IN_BASE_FCT_PTR_TYPE)(MRP_IN_DB_TYPE *pDBin);

  struct _MRP_IN_DB_TYPE
  {
    /* identifier */
    MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys;                           /* system channel to which this MRP_IN DB belongs */

    LSA_UINT16              pInstance[2];                           /* id of this MRP interconnection instance            */
                                                                    /* pInstance[0]: related MRP instance                 */
                                                                    /* pInstance[1]: related MRP interconnection instance */

    LSA_UINT16              runState;
    LSA_UINT8               isStartedFromMrm;                       /* LSA_TRUE if parent MRP instance is an MRM instance  */
                                                                    /* LSA_FALSE if parent MRP instance is an MRC instance */

    LSA_UINT16              IPort;                                  /* MRP interconnection port of this instance  */
    MRP_LINK_ADD_INFO       LinkAddInfo;                            /* IPort related additional info */

    LSA_UINT16              InID;                                   /* MRP interconnection Id already used in TX frames          */

    /* event attributes */
    MRP_IN_EVENT_DB_TYPE    rxPara;

    /* variables */
    MRP_IN_ADMIN_ROLE_ENUM_T  adminRole;                            /* admin role of MIM/MIC                    */
    MRP_IN_OPER_ROLE_ENUM_T   operRole;                             /* operational role of MIM/MIC              */

    LSA_UINT8            DomainUUID[MRP_PACKET_VAL_DOMAIN_UUID_LEN];        /* own DomainUUID: provisioned by means of dataset PDPortMrpIcDataAdjust */ 
    LSA_UINT8            CoupledDomainUUID[MRP_PACKET_VAL_DOMAIN_UUID_LEN]; /* DomainUUID of coupled MRP ring   */
    LSA_UINT8            own_SA[MRP_PACKET_VAL_MAC_ADDR_LEN];       /* own interface source mac addr of interconnection instance */
    
    /* internals */
    LSA_UINT32           prevTimeTicks;
    LSA_UINT32           currTimeTicks;

    /* state machine CBF for MRP interconnection (MIM/MIC/OFF) */
    MRP_IN_BASE_FCT_PTR_TYPE pInProjStateBaseCbf;                   /* MRP interconnection core state, is called by mrp_in_container_state() */
                                                                    /* possible values: mrp_in_proj_state_off_cbf, mrp_in_mic_proj_state_cbf, mrp_in_mim_proj_state_cbf */

    MRP_IN_BASE_FCT_PTR_TYPE pInOperStateBaseCbf;                   /* operational state, if MRP interconnection is provisioned to OFF */

    /* timer */
    LSA_UINT16          TimerIdInRstppTCsend;                       /* event attributes */

    LSA_UINT16          TimerIdInTCsend;
    LSA_UINT16          TimerIdInPollsend;
    LSA_UINT16          TimerIdInLnkChgDownSend;
    LSA_UINT16          TimerIdInLnkChgUpSend;

    /* RSTP+ triggered Interconnection topology change params */
    LSA_UINT16          TOPRstppNReturn;                            /* interconnection topology change count                  */
    LSA_UINT16          TOPRstppIntervalTcnt;                       /* interconnection topology change interval time in timer base counts (n * MRP_TIMER_TIME_BASE)*/
    LSA_UINT16          TOPRsttppMaxRepeat;                         /* max. repeat count for interconnection topology change    */

                                                                    /* transfer data to MRP state machine for InTC TX purposes */
    MRP_IN_TX_REQ_INFO_TYPE InTcTxInfo;
    MRP_IN_TX_REQ_INFO_TYPE InLnkChgTxInfo;
    MRP_IN_TX_REQ_INFO_TYPE InPollTxInfo;

    MRP_IN_TX_REQ_INFO_TYPE InTxInfo;

    /* MIM database */
    MRP_IN_MIM_DB_TYPE     DBmim;
    MRP_IN_MIC_DB_TYPE     DBmic;

    /* reference to interface database */
    MRP_IF_DB_TYPE      *pDBifRef;

    /* reference to MRP-MIB DB */
    MRP_MIB_DB_TYPE     *pDBmib;
  };

  /*---------------------------------------------------------------------------*/
  /* MIC params structure                                                      */
  /*---------------------------------------------------------------------------*/
  typedef struct _MRP_IN_MIC_PARAMS_TYPE
  {
    MRP_IN_ADMIN_ROLE_ENUM_T adminRole;    /* admin role of MIC */
    MRP_IN_OPER_ROLE_ENUM_T  operRole;     /* operational role of MIC */

    LSA_UINT16 runState;

    LSA_UINT16 IPort;
    LSA_UINT16 InID;

    LSA_UINT8  DomainUUID[MRP_PACKET_VAL_DOMAIN_UUID_LEN];  /* DomainUUID                             */
 
    LSA_UINT16 SoftStart;
    LSA_UINT16 SetDefaults;

    LSA_UINT16 MRPIC_LNKdownT_ms;
    LSA_UINT16 MRPIC_LNKupT_ms;
    LSA_UINT16 MRPIC_LNKNRmax;
    LSA_UINT16 MRPIC_StartDelay_100ms;
    LSA_UINT16 MRPIC_MICPosition;
    LSA_UINT8  MRPIC_CheckDomainID;
  }MRP_IN_MIC_PARAMS_TYPE;




  /*---------------------------------------------------------------------------*/
  /* MIM params structure                                                      */
  /*---------------------------------------------------------------------------*/
  /* interface */
  typedef struct _MRP_IN_MIM_PARAMS_TYPE
  {
    MRP_IN_ADMIN_ROLE_ENUM_T adminRole;    /* admin role of MIM */
    MRP_IN_OPER_ROLE_ENUM_T  operRole;     /* operational role of MIM */

    LSA_UINT16 IPort;
    LSA_UINT16 InID;

    LSA_UINT16 runState;

    LSA_UINT8  DomainUUID[MRP_PACKET_VAL_DOMAIN_UUID_LEN]; /* DomainUUID                             */
    LSA_UINT16 SoftStart;
    LSA_UINT16 SetDefaults;

    LSA_UINT16 MRPIC_TOPchgT_10ms;
    LSA_UINT16 MRPIC_TOPNRmax;
    LSA_UINT16 MRPIC_LinkStatusChangeT_ms;
    LSA_UINT16 MRPIC_LinkStatusNRmax;
    LSA_UINT16 MRPIC_StartDelay_100ms;
    LSA_UINT8  MRPIC_CheckDomainID;
  }MRP_IN_MIM_PARAMS_TYPE;


  /*---------------------------------------------------------------------------*/
  /*                            forward declarations                           */
  /*---------------------------------------------------------------------------*/
  LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_in_init[];
  LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_in_deinit[];

//  LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_in_tst_chg_host[];
  LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_in_trf_chg_host[];
  LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_in_ctrl_chg_host[];

  LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_in_tst_set_fdb[];
  LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_in_ctrl_set_fdb[];
  LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_in_trf_set_fdb[];
  
  LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_in_rstpp_tplg_chg_req[];
  LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_in_mim_tplg_chg_req[];

  LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_in_init_mic[];
  LSA_EXTERN const MRP_LL_REQ_TBL_T         g_MrpllReqTbl_mrp_in_init_mim[];


  /*---------------------------------------------------------------------------*/
  /*                           prototypes for mrp_inmim.c                      */
  /*---------------------------------------------------------------------------*/

  /*---------------------------------------------------------------------------*/
  /*                functions related to one MIM instance                      */
  /*---------------------------------------------------------------------------*/
  
  LSA_VOID mrp_in_mim_mrp_ring_chg_ind_cbf (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT16 inInstance);
  LSA_VOID mrp_in_mim_set_mrp_ring_chg_ind_callback (const MRP_IN_DB_TYPE *pDBin, MRP_IN_MIM_MRP_RING_CHG_FCT_PTR_TYPE cbf);
  LSA_VOID mrp_in_mim_remove_mrp_ring_chg_ind_callback (const MRP_IN_DB_TYPE *pDBin);


  LSA_VOID mrp_in_mim_oper_state_chk_ic (MRP_IN_DB_TYPE *pDBin);
  LSA_VOID mrp_in_mim_oper_state_chk_io (MRP_IN_DB_TYPE *pDBin);
  LSA_VOID mrp_in_mim_oper_state_sec_up (MRP_IN_DB_TYPE *pDBin);
  LSA_VOID mrp_in_mim_oper_state_prm_up (MRP_IN_DB_TYPE *pDBin);
  LSA_VOID mrp_in_mim_oper_state_ac_stat1 (MRP_IN_DB_TYPE *pDBin);

  /* MRP interconnection state "MIM" container function */
  LSA_VOID mrp_in_mim_proj_state_cbf (MRP_IN_DB_TYPE *pDBin);

  LSA_VOID mrp_in_mim_queue_inTC_req (MRP_IN_DB_TYPE *pDBin);
  LSA_VOID mrp_in_mim_edd_req_in_tplg_chg_req (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum);

  LSA_VOID mrp_in_mim_get_params (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, MRP_IN_MIM_PARAMS_TYPE *pParams);
  LSA_VOID mrp_in_mim_get_module_state (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance, LSA_UINT16 inInstance, MRP_REC_MODULE_STATE_TYPE *pModuleState, LSA_BOOL addStop);


  LSA_VOID mrp_in_mim_get_state (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 inInstance, LSA_UINT8 *interconnectionOpen);
  LSA_VOID mrp_in_mim_start (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 inInstance, MRP_IN_MIM_PARAMS_TYPE *pParams);
  LSA_VOID mrp_in_mim_stop (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 inInstance);
  LSA_VOID mrp_in_mim_init_var (MRP_IN_DB_TYPE *pDBin, MRP_IN_MIM_PARAMS_TYPE *pParams);

  LSA_VOID mrp_in_mim_init_add_poll_params (MRP_IN_DB_TYPE *pDBin);
  LSA_VOID mrp_in_mim_init_slow_poll_params (MRP_IN_DB_TYPE *pDBin);
  LSA_VOID mrp_in_mim_init_proj_poll_params (MRP_IN_DB_TYPE *pDBin);

  LSA_VOID mrp_in_mim_init_start_delay (MRP_IN_DB_TYPE *pDBin);


  /*---------------------------------------------------------------------------*/
  /*                           prototypes for mrp_inmic.c                      */
  /*---------------------------------------------------------------------------*/

  /*---------------------------------------------------------------------------*/
  /*                functions related to one MIC instance                      */
  /*---------------------------------------------------------------------------*/

  LSA_VOID mrp_in_mic_oper_state_ip_idle (MRP_IN_DB_TYPE *pDBin);
  LSA_VOID mrp_in_mic_oper_state_de (MRP_IN_DB_TYPE *pDBin);
  LSA_VOID mrp_in_mic_oper_state_pt (MRP_IN_DB_TYPE *pDBin);
  LSA_VOID mrp_in_mic_oper_state_ac_stat1 (MRP_IN_DB_TYPE *pDBin);

  /* MRP interconnection state "MIC" container function */
  LSA_VOID mrp_in_mic_proj_state_cbf (MRP_IN_DB_TYPE *pDBin);


  LSA_VOID mrp_in_mic_start (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 inInstance, MRP_IN_MIC_PARAMS_TYPE *pParams);
  LSA_VOID mrp_in_mic_stop (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 inInstance);

  LSA_VOID mrp_in_mic_init_proj_lnkChgUp_params (MRP_IN_DB_TYPE *pDBin);
  LSA_VOID mrp_in_mic_init_var (MRP_IN_DB_TYPE *pDBin, MRP_IN_MIC_PARAMS_TYPE *pParams);

  LSA_VOID mrp_in_mic_get_module_state (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, LSA_UINT16 instance, LSA_UINT16 inInstance, MRP_REC_MODULE_STATE_TYPE *pModuleState, LSA_BOOL addStop);
  LSA_VOID mrp_in_mic_get_params (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 inInstance, MRP_IN_MIC_PARAMS_TYPE *pParams);

  /*---------------------------------------------------------------------------*/
  /*                           prototypes for mrp_inbas.c                      */
  /*---------------------------------------------------------------------------*/


  /*---------------------------------------------------------------------------*/
  /*                functions related to one MRP_IN instance                   */
  /*---------------------------------------------------------------------------*/

  /* mrp_inbas.c*/

  LSA_UINT8 mrp_in_sys_get_in_fwd_rules_supported (const MRP_CONST_CHANNEL_TYPE_COMMON pCh);
  LSA_UINT16 mrp_in_sys_get_num_of_in_instances (const MRP_CONST_CHANNEL_TYPE_COMMON pCh);
  LSA_UINT16 mrp_in_sys_map_iport_to_inInstance (const MRP_CONST_CHANNEL_TYPE_COMMON pCh, LSA_UINT32 port);
  LSA_UINT16 mrp_in_sys_map_inId_to_inInstance (const MRP_CONST_CHANNEL_TYPE_COMMON pCh, LSA_UINT16 inId, LSA_UINT16 port);

  LSA_VOID mrp_in_ll_in_event_handler (const MRP_IN_DB_TYPE *pDBin);
  LSA_VOID mrp_in_ll_in_init (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_BOOL locHostInTest, LSA_BOOL locHostInCtrl, LSA_BOOL locHostInTrf);
  LSA_VOID mrp_in_ll_in_deinit (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance);

  LSA_VOID mrp_in_ll_in_trf_chg_host (const MRP_IN_DB_TYPE *pDBin, LSA_BOOL localHost);
  LSA_VOID mrp_in_ll_in_ctrl_chg_host (const MRP_IN_DB_TYPE *pDBin, LSA_BOOL localHost);

  LSA_VOID mrp_in_ll_fdb_in_tst_cfg (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, LSA_UINT16 rport_1, LSA_UINT16 rport_2);
  LSA_VOID mrp_in_ll_fdb_in_ctrl_cfg (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, LSA_UINT16 rport_1, LSA_UINT16 rport_2);
  LSA_VOID mrp_in_ll_fdb_in_trf_cfg (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, LSA_UINT16 rport_1, LSA_UINT16 rport_2);

  LSA_VOID mrp_in_write_current_in_open_diag (const MRP_IN_DB_TYPE *pDBin, LSA_UINT16 inOpenLink, LSA_UINT8 setFlag);
  LSA_VOID mrp_in_write_current_in_partner_role_mismatch(const MRP_IN_DB_TYPE* pDBin);
  LSA_VOID mrp_in_write_current_in_id_mismatch(const MRP_IN_DB_TYPE* pDBin);
  LSA_VOID mrp_in_write_current_iport_mismatch(const MRP_IN_DB_TYPE *pDBin, LSA_UINT8 setFlag);
  LSA_VOID mrp_in_write_current_cfm_disabled(const MRP_IN_DB_TYPE *pDBin);
  LSA_VOID mrp_in_write_current_event_diag (const MRP_IN_DB_TYPE *pDBin, LSA_UINT32 inDiag, LSA_UINT8 setFlag);
  LSA_VOID mrp_in_write_current_event_peer_macaddr (const MRP_IN_DB_TYPE *pDBin, LSA_UINT8 *pPeerMacAddr);
  LSA_VOID mrp_in_write_current_event_prm_info (const MRP_IN_DB_TYPE *pDBin, MRP_IN_OPER_ROLE_ENUM_T inOperRole, LSA_UINT16 MICPosition);

  LSA_UINT8 mrp_in_check_redstate_change (const MRP_CONST_CHANNEL_TYPE_USER pChPrivUsr, LSA_UINT16 instance, const MRP_OPC_RED_STATE_INDICATION_TYPE *pMrpRedStateCurrent);
  LSA_VOID mrp_in_write_red_init_state (const MRP_CONST_CHANNEL_TYPE_USER pChUsr, const LSA_UINT16 *pInstance, MRP_USR_ENUM_RED_IN_STATE_TYPE initState, LSA_UINT8 setFlag);
  LSA_VOID mrp_in_write_red_start_state (const MRP_IN_DB_TYPE *pDBin, MRP_USR_ENUM_RED_IN_STATE_TYPE startState, LSA_UINT16 inId, LSA_UINT16 inPortId, LSA_UINT8 setFlag);
  LSA_VOID mrp_in_write_in_rx_port(const MRP_IN_DB_TYPE *pDBin);
  LSA_VOID mrp_in_write_in_rx_inid(const MRP_IN_DB_TYPE *pDBin);
  LSA_VOID mrp_in_write_in_peer_oper_role(const MRP_IN_DB_TYPE *pDBin, LSA_UINT16 inPeerOperRole);
  LSA_VOID mrp_in_write_in_in_peer_mic_position(const MRP_IN_DB_TYPE *pDBin);

  /* Transmit function which is called in MRM resp. MRC state machine to send MRP interconnection frames on MRP ring ports */
  LSA_VOID mrp_in_to_mrp_send_req (LSA_BOOL isMrcSendReq, LSA_VOID_PTR_TYPE *pDB, LSA_BOOL isForward);

  /* Forward resp. send request called by MRP interconnection state machine to trigger MRP state machine*/
  LSA_VOID mrp_in_mrp_fwd_request (MRP_IN_DB_TYPE *pDBin);
  LSA_VOID mrp_in_mrp_tx_request (MRP_IN_DB_TYPE *pDBin);


  LSA_BOOL mrp_inTC_forward_is_needed (const MRP_IN_DB_TYPE *pDBin);

  LSA_VOID mrp_in_inTC_forward_to_iport (MRP_IN_DB_TYPE *pDBin);
  LSA_VOID mrp_in_inTC_forward_to_mrp (MRP_IN_DB_TYPE *pDBin);

  LSA_VOID mrp_in_inPoll_forward_to_iport (MRP_IN_DB_TYPE *pDBin);
  LSA_VOID mrp_in_inPoll_forward_to_mrp (MRP_IN_DB_TYPE *pDBin);

  LSA_BOOL mrp_inLnkChg_forward_is_needed (const MRP_IN_DB_TYPE *pDBin);

  LSA_VOID mrp_in_inLnkChg_forward_to_iport (MRP_IN_DB_TYPE *pDBin);
  LSA_VOID mrp_in_inLnkChg_forward_to_mrp (MRP_IN_DB_TYPE *pDBin);

  LSA_BOOL mrp_in_rx_from_different_interconn (MRP_IN_DB_TYPE *pDBin, LSA_BOOL *pDifferentMac, LSA_BOOL *pDifferentInId);

  /* RSTP+ originated InTC request which is to be triggered by an MRP interconnection instance */
  LSA_VOID mrp_in_queue_rstpp_in_tc_req (const MRP_IN_DB_TYPE *pDBin);
  LSA_VOID mrp_in_edd_req_in_rstpp_tplg_chg_req (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, const LSA_UINT16 *pInstance, MRP_LL_SREQ_RQB_ENUM_T rqbNum);

  /* operational state CBF, if MRP interconnection is provisioned to "OFF" */
  LSA_VOID mrp_in_oper_state_off_ac_stat1 (MRP_IN_DB_TYPE *pDBin);

  /* MRP interconnection state "OFF" container function (applicable if RSTP+ is configured or MRA/MRM running as IN_CONVERTER) */
  LSA_VOID mrp_in_proj_state_off_cbf (MRP_IN_DB_TYPE *pDBin);

  /* MRP interconnection central distribution function */
  LSA_VOID mrp_in_container_state (MRP_IN_DB_TYPE *pDBin);

  LSA_VOID mrp_in_update (MRP_IN_EVENT_DB_TYPE *pDBinEv, MRP_IN_EVENT_TYPE event);

  LSA_VOID mrp_in_base_init_var (MRP_IN_DB_TYPE *pDBin);
  LSA_VOID mrp_in_base_start (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance, LSA_UINT8 isStartedFromMrm);
  LSA_VOID mrp_in_base_stop (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 instance);


  /*---------------------------------------------------------------------------*/
  /*         interface related functions                                       */
  /*---------------------------------------------------------------------------*/
  LSA_UINT16 mrp_in_red_in_tc_request (const MRP_CHANNEL_TYPE_USER pChPrivUsr, MRP_UPPER_RQB_PTR_TYPE pRQB);
  LSA_UINT8 mrp_in_red_event_ind_done (const MRP_CONST_CHANNEL_TYPE_USER pChPrivUsr, const LSA_UINT16 *pInstance);
  LSA_UINT16 mrp_in_red_event_ind_provide_rqb (const MRP_CHANNEL_TYPE_USER pChPrivUsr, MRP_UPPER_RQB_PTR_TYPE pRQB);


  LSA_VOID mrp_in_init_snd_rqb (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpInMaxInstance);

  LSA_VOID mrp_in_init (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance);
  LSA_VOID mrp_in_deinit (const MRP_CONST_CHANNEL_TYPE_SYSTEM pChSys, LSA_UINT16 mrpMaxInstance, LSA_UINT16 mrpInMaxInstance);

  LSA_BOOL mrp_in_check_active_instance (const MRP_CONST_CHANNEL_TYPE_COMMON pChSys, LSA_UINT16 inInstance);

/*----------------------------------------------------------------------------*/
#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif  /* of MRP_INBAS_H */
