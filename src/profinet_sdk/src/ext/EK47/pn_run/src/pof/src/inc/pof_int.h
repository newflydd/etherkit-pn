#ifndef POF_INT_H                     /* ----- reinclude-protection ----- */
#define POF_INT_H

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
/*  C o m p o n e n t     &C: POF (Polymer Optical Fiber)               :C&  */
/*                                                                           */
/*  F i l e               &F: pof_int.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Internal headerfile                                                      */
/*  Defines internal constants, types, data, macros and prototyping for      */
/*  POF.                                                                     */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
/*****************************************************************************/

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

/*===========================================================================*/
/*                                defines                                    */
/*===========================================================================*/

#define SFP_TEMPERATURE_HIGH_BIT        0x80
#define SFP_TEMPERATURE_LOW_BIT         0x00 // no check otherwise 0x40
#define SFP_VCC_HIGH_BIT                0x00 // no check otherwise 0x20
#define SFP_VCC_LOW_BIT                 0x00 // no check otherwise 0x10
#define SFP_TX_BIAS_HIGH_BIT            0x08
#define SFP_TX_BIAS_LOW_BIT             0x04
#define SFP_TX_POWER_HIGH_BIT           0x02
#define SFP_TX_POWER_LOW_BIT            0x01
#define SFP_RX_POWER_HIGH_BIT           0x80
#define SFP_RX_POWER_LOW_BIT            0x40

#define SFP_TX_FAULT_BIT                0x04
#define SFP_RX_LOSS_BIT                 0x02

#define SFP_DIAG_TRIGGER_NO_EFFECT      0x00000000
#define SFP_DIAG_TRIGGER_TEMP           0x00000002
#define SFP_DIAG_TRIGGER_TXBIAS_HIGH    0x00000008
#define SFP_DIAG_TRIGGER_TXBIAS_LOW     0x00000004
#define SFP_DIAG_TRIGGER_TXPOWER_HIGH   0x00000020
#define SFP_DIAG_TRIGGER_TXPOWER_LOW    0x00000010
#define SFP_DIAG_TRIGGER_RXPOWER_HIGH   0x00000080
#define SFP_DIAG_TRIGGER_RXPOWER_LOW    0x00000040
#define SFP_DIAG_TRIGGER_TXFAULT        0x00000100
#define SFP_DIAG_TRIGGER_RXLOSS         0x00000200

//#define SFP_DIAG_TRIGGER_TEMP_RESET_MASK        0xFFFFFFFC
//#define SFP_DIAG_TRIGGER_TXBIAS_RESET_MASK      0xFFFFFFC3
//#define SFP_DIAG_TRIGGER_TXPOWER_RESET_MASK     0xFFFFFC3F
//#define SFP_DIAG_TRIGGER_RXPOWER_RESET_MASK     0xFFFFC3FF
//#define SFP_DIAG_TRIGGER_TXFAULT_RESET_MASK     0xFFFF3FFF
//#define SFP_DIAG_TRIGGER_RXLOSS_RESET_MASK      0xFFFCFFFF

#define SFP_DIAG_TRIGGER_SET_ALL        (   SFP_DIAG_TRIGGER_TEMP           \
                                          | SFP_DIAG_TRIGGER_TXBIAS_HIGH    \
                                          | SFP_DIAG_TRIGGER_TXBIAS_LOW     \
                                          | SFP_DIAG_TRIGGER_TXPOWER_HIGH   \
                                          | SFP_DIAG_TRIGGER_TXPOWER_LOW    \
                                          | SFP_DIAG_TRIGGER_RXPOWER_HIGH   \
                                          | SFP_DIAG_TRIGGER_RXPOWER_LOW    \
                                          | SFP_DIAG_TRIGGER_TXFAULT        \
                                          | SFP_DIAG_TRIGGER_RXLOSS         \
                                        )

#define SFP_DIAG_TRIGGER_RESET_ALL        (SFP_DIAG_TRIGGER_SET_ALL >> 1)
#define SFP_DIAG_TRIGGER_ALLBITS_MASK     (SFP_DIAG_TRIGGER_SET_ALL | SFP_DIAG_TRIGGER_RESET_ALL)

/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/

/*===========================================================================*/
/*                   some internal configuration settings                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/
#define POF_LOCAL_MEM_BOOL_PTR_TYPE            /* pointer to local-memory */ \
LSA_BOOL                    POF_COMMON_MEM_ATTR *

#define POF_COMMON_MEM_BOOL_PTR_TYPE            /* pointer to common-memory */ \
LSA_BOOL                    POF_COMMON_MEM_ATTR *

/* predefine */
typedef struct _POF_QUEUE_ELEMENT_TYPE * POF_QUEUE_ELEMENT_PTR_TYPE;

typedef struct _POF_QUEUE_ELEMENT_TYPE
{
  POF_QUEUE_ELEMENT_PTR_TYPE  next_ptr;
  POF_QUEUE_ELEMENT_PTR_TYPE  prev_ptr;

} POF_QUEUE_ELEMENT_TYPE; 

typedef struct _POF_QUEUE_TYPE
{
  POF_QUEUE_ELEMENT_PTR_TYPE  pFirst;
  POF_QUEUE_ELEMENT_PTR_TYPE  pLast;
  LSA_UINT32                  Count;

} POF_QUEUE_TYPE;

typedef struct _POF_QUEUE_TYPE * POF_QUEUE_PTR_TYPE;

/*---------------------------------------------------------------------------*/
/* This structure is  used for internal request initiated by timeout-handler.*/
/* bInUse is set if the RQB is underway (from timeout to POF_system(). If    */
/* it arrives POF_system() bInUse is reset to LSA_FALSE. So only one RQB is  */
/* used for every event.                                                     */
/*---------------------------------------------------------------------------*/
typedef struct _POF_INTERNAL_REQUEST_STRUCT 
{
  LSA_BOOL                   bInUse;    /* LSA_TRUE if in rqb-queue  */
  POF_UPPER_RQB_PTR_TYPE     pRQB;           

} POF_INTERNAL_REQUEST_TYPE;

typedef volatile POF_INTERNAL_REQUEST_TYPE  POF_LOCAL_MEM_ATTR * POF_INTERNAL_REQUEST_PTR_TYPE;    
  
/*===========================================================================*/
/* Global Timer management                                                   */
/* This is used to asign a LSA-Timer ID to an static RQB                     */
/* This management is deviceindependend and used for all devices.            */
/*===========================================================================*/
typedef struct _POF_TIMER_INFO_TYPE
{
  LSA_BOOL                       bInUse;
  LSA_TIMER_ID_TYPE              TimerID;
  POF_INTERNAL_REQUEST_TYPE      IntRQB;
  LSA_VOID                       LSA_FCT_PTR(POF_LOCAL_FCT_ATTR, Cbf) 
                                 (POF_INTERNAL_REQUEST_PTR_TYPE pIntRQB);    

} POF_TIMER_INFO_TYPE;

typedef struct _POF_TIMER_INFO_TYPE POF_LOCAL_MEM_ATTR * POF_TIMER_INFO_PTR_TYPE;

#define POF_TIMER_ID_INVALID   0xFFFF 

/*===========================================================================*/
/* Handle-management-structures                                              */
/*===========================================================================*/

/* general handle parameters */
typedef struct _POF_HANDLE_PARAMS_TYPE
{
  LSA_SYS_PATH_TYPE              SysPath;
  LSA_SYS_PTR_TYPE               pSys;
  POF_DETAIL_PTR_TYPE            pDetail;
  LSA_HANDLE_TYPE                HandleUpper;
  LSA_VOID                       LSA_FCT_PTR(POF_UPPER_OUT_FCT_ATTR, Cbf)(POF_UPPER_RQB_PTR_TYPE pRQB);

} POF_HANDLE_PARAMS_TYPE;                                                   


/*======================================================================================*/
/*                      Change Media Type in Run types                                  */
/*======================================================================================*/

typedef enum POF_PORT_MODULE_SM_STATE_TYPE_
{
    POF_PORT_MODULE_SM_STATE_PULLED,
    POF_PORT_MODULE_SM_STATE_PLUGGING,
    POF_PORT_MODULE_SM_STATE_PLUGGED
} POF_PORT_MODULE_SM_STATE_TYPE;

typedef enum POF_PRM_PORT_MODULE_SM_TRIGGER_TYPE_
{
    POF_PRM_PORT_MODULE_SM_TRIGGER_PULL         = 0,
    POF_PRM_PORT_MODULE_SM_TRIGGER_PLUG_PREPARE = 1,    
    POF_PRM_PORT_MODULE_SM_TRIGGER_PLUG_COMMIT  = 2
} POF_PRM_PORT_MODULE_SM_TRIGGER_TYPE;

typedef struct _POF_PRM_PORT_MODULE_SM_TYPE
{
    POF_PORT_MODULE_SM_STATE_TYPE              State;
} POF_PRM_PORT_MODULE_SM_TYPE;


/* --------------------------------------------------------------------------*/
/* For POF Channel Port handling                                             */
/* --------------------------------------------------------------------------*/

/* defines for Status/Speed/Mode see POF_RQB_GET_LINK_STATUS_TYPE */
/* Link Status of a Port */
typedef struct _POF_LINK_STATUS_TYPE
{
  LSA_UINT8     Status;
  LSA_UINT8     Speed; /* not used yet */
  LSA_UINT8     Mode;  /* not used yet */
  LSA_UINT8     MediaType;
  LSA_UINT8     OpticalTransType;
} POF_LINK_STATUS_TYPE;

/* LowerLayer (POF) Request PORT Control (PCtrl) */
typedef struct _POF_EDD_PCTRL_TYPE POF_LOCAL_MEM_ATTR * POF_EDD_PCTRL_PTR_TYPE;

/* typedef struct _POF_PRM_TYPE POF_LOCAL_MEM_ATTR * POF_PRM_PTR_TYPE; */

typedef struct _POF_HDB_TYPE POF_LOCAL_MEM_ATTR * POF_HDB_PTR_TYPE;

typedef LSA_VOID (POF_LOCAL_FCT_ATTR *PRM_STATE_DIAG_FCT)( LSA_UINT16         const PortId,
                                                           POF_HDB_PTR_TYPE   const pHDB,
                                                           POF_PRM_DIAG_EVENT const AppearEvent,
                                                           LSA_UINT32         const AddValue);
typedef LSA_BOOL (POF_LOCAL_FCT_ATTR *DMI_READ_HW_MARGIN_FCT)
                                                          (LSA_UINT16         const PortId,
                                                           POF_HDB_PTR_TYPE   const pHDB);

typedef LSA_VOID ( POF_LOCAL_FCT_ATTR *HW_VALIDATE_TIMEOUT_FCT)
                                                          (LSA_UINT16         const PortId,
                                                           POF_HDB_PTR_TYPE   const pHDB,
                                                           LSA_BOOL           const bCommitCall );

/* States of the Transceiver state machine */
typedef enum POF_TRANSCEIVER_STATE_TYPE_
{
    POF_TRANSCEIVER_STATE_STOPPED,
    POF_TRANSCEIVER_STATE_PULLED,
    POF_TRANSCEIVER_STATE_WRITE_THR,
    POF_TRANSCEIVER_STATE_PLUGGED
} POF_TRANSCEIVER_STATE_TYPE;

/* Trigger events of the Transceiver state machine */
typedef enum POF_TRANSCEIVER_TRIGGER_TYPE_
{
    POF_TRANSCEIVER_TRIGGER_START,
    POF_TRANSCEIVER_TRIGGER_STOP,
    POF_TRANSCEIVER_TRIGGER_CYCLIC,
    POF_TRANSCEIVER_TRIGGER_PRM_WRITTEN
} POF_TRANSCEIVER_TRIGGER_TYPE;

/* flag verified by a second read -> creating a Threshold by timing */
typedef struct _POF_TIMING_THRESHOLD_TYPE
{
    LSA_BOOL            bMaintenanceRequired;
    LSA_BOOL            bMaintenanceDemanded;
    LSA_BOOL            bError;

} POF_TIMING_THRESHOLD_TYPE;

//function pointer for SFP-Register-Evaluation
typedef POF_PRM_DIAG_EVENT ( POF_LOCAL_FCT_ATTR *POF_SFP_EVALUATION_FCT)( LSA_UINT8 const * const pDiagnosticRegisters);

typedef enum _POF_SFP_STATE_VALUES
{
    SFP_PROPERTY_STATE_ABOVE_HIGH,
    SFP_PROPERTY_STATE_NORMAL,
    SFP_PROPERTY_STATE_BELOW_LOW,
    SFP_PROPERTY_STATE_ERROR
} POF_SFP_STATE_VALUES;

typedef enum _POF_SFP_REGISTERS_ADDRESS
{
    POF_SFP_STATUS_AND_CONTROL = 0
    , POF_SFP_DONOT_CARE_1
    , POF_SFP_ALARM_FLAGS_1
    , POF_SFP_ALARM_FLAGS_2
    , POF_SFP_REGISTER_ARRAY_SIZE
} POF_SFP_REGISTERS_ADDRESS;


typedef enum _POF_SFP_CHECK_DIAGNOSTIC_TYPE
{
    POF_SFP_TEMPERATURE = 0,
    POF_SFP_TX_BIAS,
    POF_SFP_TX_POWER,
    POF_SFP_RX_POWER,
    POF_TX_FAULT,
    POF_RX_LOSS,
    POF_SFP_NUMBER_CHECK_DIAGNOSTICS
} POF_SFP_CHECK_DIAGNOSTIC_TYPE;


typedef struct _POF_SFP_DIAGNOSTIC_TYPE
{
    const LSA_UINT16                  ExtChannelErrorType;
    const LSA_UINT16                  DiagEventSeverity;
    const POF_SFP_REGISTERS_ADDRESS   AlarmAddress;
    const LSA_UINT8                   CheckBitMask;
} POF_SFP_DIAGNOSTIC_TYPE;

typedef struct _POF_SFP_DIAGNOSTIC_PAIR_TYPE
{
    POF_SFP_STATE_VALUES              LastReportedState;
    POF_SFP_STATE_VALUES              State;
    POF_SFP_DIAGNOSTIC_TYPE           HighValue;
    POF_SFP_DIAGNOSTIC_TYPE           LowValue;
} POF_SFP_DIAGNOSTIC_PAIR_TYPE;

typedef struct _POF_SFP_PORT_DIAGNOSTIC_TYPE
{
    LSA_UINT32                        ActivationMask;
    POF_SFP_DIAGNOSTIC_PAIR_TYPE      SFP_DiagnosticChecks [POF_SFP_NUMBER_CHECK_DIAGNOSTICS];
} POF_SFP_PORT_DIAGNOSTIC_TYPE;


typedef struct _POF_SFP_PORT_DIAGNOSTIC_TYPE POF_LOCAL_MEM_ATTR * POF_SFP_PORT_DIAGNOSTIC_PTR_TYPE;

/*--------------------------------------------------------------------------*/


typedef struct _POF_SFP_DIAG_TEST_TYPE
{
    const   LSA_UINT32                ManipulationBitMask;
    const   LSA_UINT8                 ManipulationWriteAddress;
    const   LSA_UINT8                 ManipulationByteNumber;
    const   LSA_UINT16                ManipulationMask;
    union {
  /*const*/ LSA_UINT8                 Value_8[2];
  /*const*/ LSA_UINT16                Value;
          } Original;
    union {
  /*const*/ LSA_UINT8                 Value_8[2];
  /*const*/ LSA_UINT16                Value;
          } Manipulation;
} POF_SFP_DIAG_TEST_TYPE;

typedef struct _POF_SFP_DIAG_TEST_PAIR_TYPE
{
    POF_SFP_DIAG_TEST_TYPE            HighValue;
    POF_SFP_DIAG_TEST_TYPE            LowValue;
} POF_SFP_DIAG_TEST_PAIR_TYPE;

#define SFP_TRANCEIVER_MAX_SIGNATURE_LENHGTH        20
#define SFP_TRANCEIVER_MAX_PASSWORD_LENHGTH          4

typedef struct _POF_SFP_PORT_DIAG_TEST_TYPE
{
    const LSA_UINT8                   Signature[SFP_TRANCEIVER_MAX_SIGNATURE_LENHGTH];
    const LSA_UINT16                  SignatureLength;
    const LSA_UINT8                   SignatureAddress;
    const LSA_UINT8                   PasswordAddress;
/*const*/ LSA_UINT8                   Password      [SFP_TRANCEIVER_MAX_PASSWORD_LENHGTH + 2];  // +TableSelect +FillByte
/*const*/ LSA_UINT8                   ResetPassword [SFP_TRANCEIVER_MAX_PASSWORD_LENHGTH + 2];  // +TableSelect +FillByte
    const LSA_UINT16                  PasswordLength;
          LSA_UINT32                  CurrentManipulationStatus;
          LSA_UINT32                  SupportedManipulationMask;
    POF_SFP_DIAG_TEST_PAIR_TYPE       SFP_DiagTestData [POF_SFP_NUMBER_CHECK_DIAGNOSTICS];
} POF_SFP_PORT_DIAG_TEST_TYPE;

typedef struct _POF_SFP_PORT_DIAG_TESTS
{
    const LSA_UINT32                  NumberTestTranceivers;      // for now just one
    POF_SFP_PORT_DIAG_TEST_TYPE       TestTranceivers [1];
} POF_SFP_PORT_DIAG_TESTS;


typedef struct _POF_SFP_PORT_DIAG_TESTS POF_LOCAL_MEM_ATTR * POF_SFP_PORT_DIAG_TESTS_PTR_TYPE;



/* no resource exist for send a prm-indication */
typedef struct _POF_LOST_PRM_INDICATION_TYPE
{
    LSA_BOOL                          bLost;
    POF_PRM_DIAG_EVENT                AppearEvent;
    POF_PRM_DIAG_EVENT                DisappearEvent;
    LSA_UINT32                        AddValue;

} POF_LOST_PRM_INDICATION_TYPE;

typedef struct _POF_EDD_PCTRL_TYPE
{
    POF_LINK_STATUS_TYPE                LinkStatus;             /* Current LinkStatus               */
    POF_RECORD_SET_TYPE                 RecordSet_A;            /* Current RecordSet                */
    POF_RECORD_SET_TYPE                 RecordSet_B;            /* Recordset for xx_prm_xx Service  */
    PRM_STATE_DIAG_FCT                  DiagStateFct;
    DMI_READ_HW_MARGIN_FCT              ReadHW_MarginFct;
    HW_VALIDATE_TIMEOUT_FCT             ValidateTimeOutFct;     /* function pointer for prm-state-diag-validate */
    I2C_DMI_MAP_TYPE                    DMIMap;                 /* Description see SFF-8472 MSA     */
    POF_QUEUE_TYPE                      PrmIndicationQueue;     /* Prm-Indication-Queue             */
    POF_PRM_PORT_MODULE_SM_TYPE         PortModuleSm;           /* statemachine for PRM Port states */
    POF_TRANSCEIVER_STATE_TYPE          TransceiverState;       /* Pulled or Plugged                */
    LSA_BOOL                            bLastFO_LED_State;      /* last state of fo-led             */
    POF_TIMING_THRESHOLD_TYPE           TimeThreshold;          /* creating a Threshold by timing   */
    POF_LOST_PRM_INDICATION_TYPE        LostPrmInd;             /* Lost PrmIndication               */
    POF_SFP_PORT_DIAGNOSTIC_TYPE        SFP_Diagnostic;         /* SFP Diagnostic Administration    */
    LSA_BOOL                            TestTranceiverDetected; /* Status of Test Receiver Detection*/
    POF_SFP_PORT_DIAG_TEST_TYPE         SFP_DiagTest;           /* SFP Diagnostic Test Manipulation */
} POF_EDD_PCTRL_TYPE;

//function pointer for prm-state-machine
typedef POF_RSP  ( POF_LOCAL_FCT_ATTR *POF_PRM_STATE_FCT)( POF_UPPER_RQB_PTR_TYPE  const pRQB, 
                                                           POF_HDB_PTR_TYPE        const pHDB,
                                                           LSA_OPCODE_TYPE         const AppearOpCode );

//I2C defines
#define POF_I2C_STD_OFFSET_CNT  1       //standard offset count within POF = 1

//I2C control data
typedef struct _POF_I2C_BLOCK_TYPE
{
    POF_I2C_AVAILABLE_TYPE        I2CAvailable;
    LSA_UINT16                    I2CMuxSelect;
} POF_I2C_BLOCK_TYPE;

/* --------------------------------------------------------------------------*/
/* For UserChannel                                                           */
/* --------------------------------------------------------------------------*/
typedef struct _POF_HDB_TYPE
{
    LSA_UINT16                    State;                      /* handle state                                         */
    LSA_HANDLE_TYPE               ThisHandle;                 /* this handle                                          */
    LSA_UINT32                    TraceIdx;                   /* trace-index for LSA-Trace                            */
    POF_HANDLE_PARAMS_TYPE        Params;                     /* general handle parameters                            */
    POF_UPPER_RQB_PTR_TYPE        pOpenOrCLoseRQB;            /* Open or Close Request-RQB                            */
    POF_RSP                       OpenResponse;               /* Response from Open if error                          */
    EDD_HANDLE_LOWER_TYPE         EDDHandle;                  /* If Open the lower EDD handle                         */
    LSA_UINT32                    PortCnt;                    /* Number of Ports                                      */
    LSA_UINT32                    HardwareType;               /* EDD-HW-Type from EDD_SRV_GET_PARAMS                  */
    POF_EDD_PCTRL_PTR_TYPE        pPCtrl;                     /* pointer to array of Port Controls                    */
    LSA_UINT32                    LinkIndPendingCnt;          /* Number of Link-Ind-Req pending in EDD                */
    POF_PRM_STATE_FCT             PrmStateFct;                /* function pointer for prm-state                       */
    POF_PRM_DETAIL_ERR            PrmDetailErr;               /*                                                      */
    POF_I2C_BLOCK_TYPE            I2C                    [EDD_CFG_MAX_PORT_CNT]; /* I2C parameters, Index=PortIndex=PortId-1   */
    LSA_UINT8                     PortparamsNotApplicable[EDD_CFG_MAX_PORT_CNT]; /* special info for Change Media Type         */
    LSA_UINT8                     ApplyDefaultPortparams [EDD_CFG_MAX_PORT_CNT]; /* special info for Change Media Type         */
    LSA_UINT8                     IsModularPortPrmEnd    [EDD_CFG_MAX_PORT_CNT]; /* Modular/Compact port info from PRM_END     */
    LSA_UINT8                     IsModularPort          [EDD_CFG_MAX_PORT_CNT]; /* Modular/Compact port info after PRM_COMMIT */
} POF_HDB_TYPE;

//State (of LSA-Channel):
#define POF_HANDLE_STATE_FREE                           0
#define POF_HANDLE_STATE_CLOSED                         1
#define POF_HANDLE_STATE_OPEN_CHANNEL_EDD_RUNNING       2
#define POF_HANDLE_STATE_CLOSE_CHANNEL_EDD_RUNNING      3
#define POF_HANDLE_STATE_GET_PARAMS_EDD_RUNNING         4   /* Set GetParams of EDD running                         */
#define POF_HANDLE_STATE_GET_PORT_PARAMS_EDD_RUNNING    5   /* Set GetPortParams of EDD running                     */
#define POF_HANDLE_STATE_CLOSE_ERROR_EDD_RUNNING        6   /* close because of open error                          */
#define POF_HANDLE_STATE_CLOSE_WAIT_TIMER               7   /* wait for timer while closing                         */
#define POF_HANDLE_STATE_OPEN                           8

//TraceIdx:
/* Trace index useable with LSA-Trace (indexed traces)              (out) */
/*  0   : no special index or unknown                                     */
/*        (because only one interface used or id not present)             */
/*  1..x: trace index. Typically the index references the EDD interface,  */
/*        which shall be unique for all EDD interfaces. The TraceIdx will */
/*        be configured by systemadaption on EDD startup.                 */

/* POF unambiguous ErrorCode -> see pof_err.h */
/* will be set, if response != EDD_STS_OK     */
/* see also EDD_SET_DETAIL_ERR                */
typedef struct _EDD_DETAIL_ERR_TYPE
{
      LSA_UINT32  bSet;
      LSA_UINT32  Line; 
      LSA_UINT32  ModuleID; 
      LSA_UINT32  Error; 
      LSA_UINT32  Value; 
} EDD_DETAIL_ERR_TYPE;

typedef struct _POF_HDB_INFO_TYPE
{
    LSA_UINT16                    MaxHandleCnt;    /* Number of max. handles supported */
                                                   /* currently only 1                 */
    LSA_UINT16                    UsedHandleCnt;   /* currently used user-channels     */
                                                   /* currently only 1                 */
    /* for every device! */
    POF_HDB_TYPE                  HDB[POF_CFG_MAX_CHANNELS]; 
                                                   /* to array of MaxHandle */
                                                   /* POF_HDB_TYPE types    */

    /* possibly move to Device or Channel */
    EDD_DETAIL_ERR_TYPE           ErrDetail;       /* Detail Error in RQB */
} POF_HDB_INFO_TYPE;

/* --------------------------------------------------------------------------*/
/* global POF-management (handle independend)                                */
/* --------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* maximum number of timer needed -> do not change !!!                       */
/*---------------------------------------------------------------------------*/
#define POF_CFG_MAX_TIMER   1   /* number of Timers for all  */
                                /* devices                   */

/* FatalError */
#define  POF_MAX_EXCP_INFO  0x140   /* Size of LSAError.error_data_length */

typedef struct _POF_EXCP_TYPE
{
    POF_FATAL_ERROR_TYPE      Error;
    LSA_UINT32                ModuleID;
    LSA_UINT32                Line;
    LSA_UINT8                 sInfo[POF_MAX_EXCP_INFO];
} POF_EXCP_TYPE;

typedef struct _POF_GLOB_TYPE
{
    POF_TIMER_INFO_TYPE       Timer[POF_CFG_MAX_TIMER];  /* Timermanagement     */
    LSA_TIMER_ID_TYPE         CyclicTimerIndex;          /* Timer_ID of cylcic  */
                                                         /* 1s timer.           */
    LSA_UINT32                CyclicTimerCnt;            /* TimerCounter        */
    LSA_UINT32                CyclicTimerUserCnt;        /* number of user of   */
                                                         /* timer               */
    POF_EXCP_TYPE             Excp;                      /* see also POF_FatalError */
} POF_GLOB_TYPE;

/*typedef struct _POF_GLOB_TYPE  POF_LOCAL_MEM_ATTR *   POF_GLOB_PTR_TYPE; */

/* --------------------------------------------------------------------------*/
/* global POF-management anchor                                              */
/* --------------------------------------------------------------------------*/
typedef struct _POF_MGM_TYPE  POF_LOCAL_MEM_ATTR *   POF_MGM_PTR_TYPE;

typedef struct _POF_MGM_TYPE
{
    POF_GLOB_TYPE        Glob;       /* Global Mgm      */
    POF_HDB_INFO_TYPE    HUser;      /* User-Handle Mgm */
} POF_MGM_TYPE;

/*===========================================================================*/
/*                                  data                                     */
/*===========================================================================*/
LSA_EXTERN POF_MGM_PTR_TYPE    g_pPOFInfo;      /* global POF-INFO-Structure */

/*===========================================================================*/
/*                                macros                                     */
/*===========================================================================*/
#define POF_MGM_GET_GLOB_PTR()     (&(g_pPOFInfo->Glob))
#define POF_MGM_GET_HUSER_PTR()    (&(g_pPOFInfo->HUser))

#if defined (POF_CFG_NO_FATAL_FILE_INFO)
#define POF_SET_DETAIL_ERR(_Error)                                  \
   if ( (POF_MGM_GET_HUSER_PTR())->ErrDetail.bSet == 0 )            \
   {                                                                \
     (POF_MGM_GET_HUSER_PTR())->ErrDetail.Error    = _Error;        \
     (POF_MGM_GET_HUSER_PTR())->ErrDetail.ModuleID = POF_MODULE_ID; \
     (POF_MGM_GET_HUSER_PTR())->ErrDetail.Line     = 0;             \
     (POF_MGM_GET_HUSER_PTR())->ErrDetail.bSet     = 1;             \
   }
#else
#define POF_SET_DETAIL_ERR(_Error)                                  \
   if ( (POF_MGM_GET_HUSER_PTR())->ErrDetail.bSet == 0 )            \
   {                                                                \
     (POF_MGM_GET_HUSER_PTR())->ErrDetail.Error    = _Error;        \
     (POF_MGM_GET_HUSER_PTR())->ErrDetail.ModuleID = POF_MODULE_ID; \
     (POF_MGM_GET_HUSER_PTR())->ErrDetail.Line     = __LINE__;      \
     (POF_MGM_GET_HUSER_PTR())->ErrDetail.bSet     = 1;             \
   }
#endif


#if 1
#define POF_PRM_SET_ERR_OFFSET(pHDB, Offset)        \
{                                                   \
    pHDB->PrmDetailErr.ErrOffset += (Offset);       \
    pHDB->PrmDetailErr.ErrValid   = LSA_TRUE;       \
}
#else
LSA_VOID  POF_PRM_SET_ERR_OFFSET( POF_HDB_PTR_TYPE  const  pHDB,
                                  LSA_UINT32        const  Offset);
#endif

/** macro to set all values for PrmDetailErr
*
* @param[in]   pDetailErr      pointer to struct POF_PRM_DETAIL_ERR_TYPE
* @param[in]   _ErrPortID      LSA_UINT16 value for ErrPortID
* @param[in]   _ErrIndex       LSA_UINT32 value for ErrIndex
* @param[in]   _ErrOffset      LSA_UINT32 value for ErrOffset
* @param[in]   _ErrFault       LSA_UINT8  value for ErrFault
*/

#define POF_PRM_SET_DETAIL_ERR(pDetailErr, _ErrPortIDIndex, _ErrIndex, _ErrOffset, _ErrFault)       \
{                                                                                              \
    ((POF_PRM_DETAIL_ERR_PTR_TYPE)pDetailErr)->ErrPortID   = (LSA_UINT16)(_ErrPortIDIndex + 1); \
    ((POF_PRM_DETAIL_ERR_PTR_TYPE)pDetailErr)->ErrIndex    = (LSA_UINT32)(_ErrIndex);          \
    ((POF_PRM_DETAIL_ERR_PTR_TYPE)pDetailErr)->ErrOffset   = (LSA_UINT32)(_ErrOffset);         \
    ((POF_PRM_DETAIL_ERR_PTR_TYPE)pDetailErr)->ErrFault    = (LSA_UINT8) (_ErrFault);          \
    ((POF_PRM_DETAIL_ERR_PTR_TYPE)pDetailErr)->ErrValid    =  LSA_TRUE;                        \
}

/** macro to first init PrmDetailErr with default values
*
* @param[in]   pDetailErr      pointer to struct POF_PRM_DETAIL_ERR_TYPE
*/
#define POF_PRM_INIT_DETAIL_ERR(pDetailErr)                                                    \
{                                                                                              \
    ((POF_PRM_DETAIL_ERR_PTR_TYPE)pDetailErr)->ErrPortID   = POF_PRM_ERR_PORT_ID_DEFAULT;      \
    ((POF_PRM_DETAIL_ERR_PTR_TYPE)pDetailErr)->ErrIndex    = POF_PRM_ERR_INDEX_DEFAULT;        \
    ((POF_PRM_DETAIL_ERR_PTR_TYPE)pDetailErr)->ErrOffset   = POF_PRM_ERR_OFFSET_DEFAULT;       \
    ((POF_PRM_DETAIL_ERR_PTR_TYPE)pDetailErr)->ErrFault    = POF_PRM_ERR_FAULT_DEFAULT;        \
    ((POF_PRM_DETAIL_ERR_PTR_TYPE)pDetailErr)->ErrValid    = LSA_FALSE;                        \
}

/*===========================================================================*/
/*                           Access to network bytes                         */
/*===========================================================================*/
#ifndef POF_CFG_BIG_ENDIAN

/* flips a byte pattern of 0102 to 0201 */
#define POF_FLIP16(C)                                           \
          (LSA_UINT16)  (  (((LSA_UINT16)(C) >> 8) & 0x00FF)    \
                         | (((LSA_UINT16)(C) << 8) & 0xFF00) )  \

/* flips a byte pattern of 01020304 to 04030201 */
#define POF_FLIP32(var)                                    \
                     (  ((( (var)) & 0xFF000000L) >> 24)   \
                      + ((( (var)) & 0x00FF0000L) >>  8)   \
                      + ((( (var)) & 0x0000FF00L) <<  8)   \
                      + ((( (var)) & 0x000000FFL) << 24))  \

#else //POF_CFG_BIG_ENDIAN

#define POF_FLIP16(var)      (var)
#define POF_FLIP32(var)      (var)

#endif //POF_CFG_BIG_ENDIAN

#ifndef POF_NTOH_16
#define POF_NTOH_16(Value) ( POF_FLIP16(Value) )/* Network To Host / Hardware Short */
#endif

#ifndef POF_NTOH_32
#define POF_NTOH_32(Value) ( POF_FLIP32(Value) )/* Network To Host / Hardware Long  */
#endif

#ifndef POF_HTONT_16
#define POF_HTONT_16(Value) ( POF_FLIP16(Value) )/* Host To Network / Hardware Short */
#endif

#ifndef POF_HTONT_32
#define POF_HTONT_32(Value) ( POF_FLIP32(Value) )/* Host TO Network / Hardware Long  */
#endif
/*===========================================================================*/
/*                           chain macros for rqb                            */
/*===========================================================================*/

/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/

LSA_VOID POF_LOCAL_FCT_ATTR POF_TransceiverSMTransition(     LSA_UINT16                     const PortIndex,
                                                             POF_TRANSCEIVER_TRIGGER_TYPE         Trigger,
                                                             POF_HDB_PTR_TYPE               const pHDB);
                                                             
LSA_BOOL POF_LOCAL_FCT_ATTR POF_TransceiverIsPulled(         LSA_UINT16                     const PortIndex,
                                                             POF_HDB_PTR_TYPE               const pHDB);
LSA_BOOL POF_LOCAL_FCT_ATTR POF_TransceiverIsPlugged(        LSA_UINT16                     const PortIndex,
                                                             POF_HDB_PTR_TYPE               const pHDB);


POF_RSP  POF_LOCAL_FCT_ATTR POF_PRMChangePortStateMachine(   POF_HDB_PTR_TYPE               const pHDB,
                                                             POF_PRM_PORT_MODULE_SM_TRIGGER_TYPE
                                                                                            const Trigger,
                                                             LSA_UINT16                           PortIndex);

LSA_BOOL POF_LOCAL_FCT_ATTR POF_PRMChangePortState_IsPulled( POF_HDB_PTR_TYPE               const pHDB,
                                                             LSA_UINT16                     const PortIndex);
LSA_BOOL POF_LOCAL_FCT_ATTR POF_PRMChangePortState_IsPlugged(POF_HDB_PTR_TYPE               const pHDB,
                                                             LSA_UINT16                     const PortIndex);
LSA_BOOL POF_LOCAL_FCT_ATTR POF_PRMChangePortState_IsAnySequenceRunning(POF_HDB_PTR_TYPE    const pHDB,
                                                             LSA_UINT16                          *pPortIndex);


/*===========================================================================*/
/*                                   macros                                  */
/*===========================================================================*/

/*===========================================================================*/
/*                                  prototypes                               */
/*===========================================================================*/

/*===========================================================================*/
/*                              compiler errors                              */
/*===========================================================================*/
#if !defined POF_UPPER_IN_FCT_ATTR
#error "pof_UPPER_IN_FCT_ATTR not defined in file pof_cfg.h"
#endif

#if !defined POF_UPPER_OUT_FCT_ATTR
#error "pof_UPPER_OUT_FCT_ATTR not defined in file pof_cfg.h"
#endif

#if !defined POF_SYSTEM_IN_FCT_ATTR
#error "pof_SYSTEM_IN_FCT_ATTR not defined in file pof_cfg.h"
#endif

#if !defined POF_SYSTEM_OUT_FCT_ATTR
#error "pof_SYSTEM_OUT_FCT_ATTR not defined in file pof_cfg.h"
#endif

#if !defined POF_LOCAL_FCT_ATTR
#error "pof_LOCAL_FCT_ATTR not defined in file pof_cfg.h"
#endif

#if !defined POF_UPPER_RQB_ATTR
#error "pof_UPPER_RQB_ATTR not defined in file pof_cfg.h"
#endif

#if !defined POF_UPPER_MEM_ATTR
#error "pof_UPPER_MEM_ATTR not defined in file pof_cfg.h"
#endif

#if !defined POF_SYSTEM_MEM_ATTR
#error "pof_SYSTEM_MEM_ATTR not defined in file pof_cfg.h"
#endif

#if !defined POF_LOCAL_MEM_ATTR
#error "pof_LOCAL_MEM_ATTR not defined in file pof_cfg.h"
#endif

#if !defined LSA_SYS_PATH_TYPE
#error "LSA_SYS_PATH_TYPE not defined in file lsa_cfg.h"
#endif

#if !defined LSA_SYS_PTR_TYPE
#error "LSA_SYS_PTR_TYPE not defined in file lsa_cfg.h"
#endif

#if defined (POF_CFG_PRM_FO_STATE_INFORMATION_SUBBLOCK_ENABLE)
#error "POF_CFG_PRM_FO_STATE_INFORMATION_SUBBLOCK_ENABLE in file pof_cfg.h no longer existent"
#endif


/*****************************************************************************/
/*  end of file pof_int.h                                                    */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of POF_INT_H */
