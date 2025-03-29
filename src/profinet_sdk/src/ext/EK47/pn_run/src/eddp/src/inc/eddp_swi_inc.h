#ifndef EDDP_SWI_INC_H                   /* ----- reinclude-protection ----- */
#define EDDP_SWI_INC_H

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
/*  C o m p o n e n t     &C: EDDP (EthernetDeviceDriver for PN-IP)     :C&  */
/*                                                                           */
/*  F i l e               &F: eddp_swi_inc.h                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Root headerfile for SWITCH component                                     */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  17.11.08    AB    initial version                                        */
#endif
/*****************************************************************************/

#include "eddp_swi.h"
#include "eddp_swi_pm.h"
#include "eddp_swi_fdb.h"
#include "eddp_swi_frameclass.h"
#include "eddp_swi_irt.h"


/*===========================================================================*/
/*                                  defines                                  */
/*===========================================================================*/
// Note: enabled by default. You can disable this feature for debug purpose only !!!
#define EDDP_SWI_USE_PORT_BARRIER


/*===========================================================================*/
#define EDDP_SWI_BARRIER_IDX_REST       0           // barrier for queues:  0, 1, 2, 3, 5, 9
#define EDDP_SWI_BARRIER_IDX_RTA        1           // barrier for queues:  4, 6
#define EDDP_SWI_BARRIER_IDX_MGM        2           // barrier for queues:  10, 11
#define EDDP_SWI_BARRIER_IDX_OFF        15          // barrier disabled

#define EDDP_SWI_BARRIER_QCW_REST       12          // 1K5 = 128Byte*12
#define EDDP_SWI_BARRIER_QCW_RTA        19          // 2K5 = 128Byte*19
#define EDDP_SWI_BARRIER_QCW_MGM        19          // 2K5 = 128Byte*19

#define EDDP_SWI_BARRIER_TIME_1MS       1000000     // 1ms = 0xF4240
/*===========================================================================*/

// max. port number depending on hardware type
#define EDDP_SWI_MAX_PORT_CNT_ERTEC200P         EDDP_MAX_PORT_CNT_ERTEC200P
#define EDDP_SWI_MAX_PORT_CNT_HERA              EDDP_MAX_PORT_CNT_HERA

// largest possible phy address
#define EDDP_SWI_LARGEST_POSSIBLE_PHY_ADDRESS   31

/*===========================================================================*/
/*                                 macros                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/


/*---------------------------------------------------------------------------*/
/* typedefs for DDB (used in eddp_dev.h)                                     */
/*---------------------------------------------------------------------------*/

typedef struct _EDDP_DDB_SWI_LINK_STATUS_TYPE   EDDP_LOCAL_MEM_ATTR *           EDDP_DDB_SWI_LINK_STATUS_PTR_TYPE;
typedef struct _EDDP_DDB_SWI_LINK_STATUS_TYPE   EDDP_LOCAL_MEM_ATTR const *     EDDP_CONST_DDB_SWI_LINK_STATUS_PTR_TO_CONST_TYPE;

typedef struct _EDDP_DDB_SWI_LINK_STATUS_TYPE
{
    LSA_UINT8                               Autoneg;
    LSA_UINT8                               LinkStatus;
    LSA_UINT8                               LinkSpeed;
    LSA_UINT8                               LinkDuplexity;
    LSA_UINT8                               PortStatus;
    LSA_UINT8                               PhyStatus;
    LSA_UINT8                               IRTPortStatus;
    LSA_UINT8                               RTClass2_PortStatus;
    LSA_UINT16		                        MAUType;
    LSA_UINT16		                        MAUTypeParam;
    EDD_UPPER_MEM_U8_PTR_TYPE               pMAUTypePosition;
    LSA_UINT16                              PortState;
    LSA_UINT8                               MediaType;
    LSA_UINT8                               OpticalTransType;
    LSA_UINT8                               ShortPreamble;          // 0: long preamble, 1: short preamble
    LSA_UINT8                               TxFragmentation;
    LSA_UINT8                               AddCheckEnabled;
    LSA_UINT8                               ChangeReason;
    LSA_UINT32                              LineDelayInTicksMeasured;
    LSA_UINT32                              CableDelayInNsMeasured;
    LSA_UINT32                              PRM_MaxLineRxDelay;
    LSA_UINT32                              RealPortTxDelay;
    LSA_UINT32                              RealPortRxDelay;
    LSA_UINT32                              MaxPortTxDelay;
    LSA_UINT32                              MaxPortRxDelay;
    LSA_UINT8                               SyncId0_TopoOk;
    LSA_UINT32                              AutonegCapAdvertised;
    LSA_UINT8                               IsWireless;
    LSA_UINT8                               IsMDIX;
    LSA_UINT8                               IsPulled;
    LSA_UINT8                               MRPRingPort;
    LSA_UINT8                               SupportsMRPInterconnPortConfig;
    LSA_UINT8                               LinkSpeedModeConfigured;
    LSA_UINT8                               LinkSpeedModeConfiguredParam;
    LSA_UINT32                              Neighbour_TopoState;    // store here the state of the neighbour given by EDDP_GENSetRemotePortState()
    EDD_RESERVED_INTERVAL_RX_TX_TYPE_RED    RsvIntervalRed;         // needed by EDD_SRV_GET_PORT_PARAMS
    EDD_RESERVED_INTERVAL_RX_TX_TYPE        RsvIntervalOrange;      // needed by EDD_SRV_GET_PORT_PARAMS   
    LSA_UINT8                               PortparamsNotApplicable;
} EDDP_DDB_SWI_LINK_STATUS_TYPE;
/* MRPRingPort          : see <edd_usr.h>                                                                               */

typedef struct _EDDP_DDB_SWI_PORT_MAP_TYPE
{
	LSA_UINT32         PortID_x_to_HardwarePort_y[EDDP_MAX_PORT_CNT];
	LSA_UINT32         PortID_x_to_ASICPort_y[EDDP_MAX_PORT_CNT];
	LSA_UINT32         PortID_x_to_SMIPort_y[EDDP_MAX_PORT_CNT];
	LSA_UINT32         SMI_PHY_Address_y[EDDP_MAX_PORT_CNT];

} EDDP_DDB_SWI_PORT_MAP_TYPE;


typedef struct _EDDP_DDB_SWI_LED_BLINK_TYPE     EDDP_LOCAL_MEM_ATTR *   EDDP_DDB_SWI_LED_BLINK_PTR_TYPE;
typedef struct _EDDP_DDB_SWI_LED_BLINK_TYPE
{
    LSA_BOOL	isBlinking;
    LSA_UINT32	TotalBlinkCountIn500ms;
    LSA_UINT32	TotalBlinkDurationInSeconds;
    LSA_BOOL	isLED_ON;
    LSA_UINT32	Timer;

} EDDP_DDB_SWI_LED_BLINK_TYPE;


typedef struct _EDDP_DDB_SWI_RTC3_PORT_STATE_TYPE   EDDP_LOCAL_MEM_ATTR *   EDDP_DDB_SWI_RTC3_PORT_STATE_PTR_TYPE;
typedef struct _EDDP_DDB_SWI_RTC3_PORT_STATE_TYPE
{
    LSA_UINT32      TopoState;
    LSA_UINT32      PDEVTopoState;
    LSA_UINT32      RemoteState;
    LSA_BOOL        Preamble;
} EDDP_DDB_SWI_RTC3_PORT_STATE_TYPE;
/* TopoState            : from EDD_SRV_SET_REMOTE_PORT_STATE                                                */
/* PDEVTopoState        : from EDD_SRV_SET_REMOTE_PORT_STATE                                                */
/* RemoteState          : from EDD_SRV_SET_REMOTE_PORT_STATE                                                */
/* Preamble             : generated within EDDP_SWI_Trigger_PreambleLength() as Param for the RTC3PSM       */
/*                        use predefined values: EDDP_SWI_PREAMBLE_NOT_OK, EDDP_SWI_PREAMBLE_OK             */
#define EDDP_SWI_PREAMBLE_NOT_OK    LSA_FALSE
#define EDDP_SWI_PREAMBLE_OK        LSA_TRUE


// with PRM_COMMIT we must store the current PRM settings for the preamble in these local members !!!
typedef struct _EDDP_DDB_SWI_PRAMBLE_PRM_CFG_TYPE
{
    LSA_UINT16      PreambleLength;             // long(0) or short(1) preamble length
    LSA_BOOL        PreambleLength_IsPresent;   // TRUE if written by PDPORT_DATA_ADJUST_PREAMBLELENGTH
} EDDP_DDB_SWI_PRAMBLE_PRM_CFG_TYPE;


typedef struct _EDDP_DDB_SWI_CYCLE_INFO_TYPE    EDD_UPPER_MEM_ATTR *    EDDP_DDB_SWI_CYCLE_INFO_PTR_TYPE;
typedef struct _EDDP_DDB_SWI_CYCLE_INFO_TYPE
{
    LSA_UINT16      CycleBaseFactor;
} EDDP_DDB_SWI_CYCLE_INFO_TYPE;
 

/* ------------------------------------------------------------------------------------ */
/* state machine "SendClockChange"                                                      */
/* ------------------------------------------------------------------------------------ */
typedef enum _EDDP_SWI_SEND_CLOCK_CHANGE_STATE_TYPE
{
    EDDP_SWI_SEND_CLOCK_CHANGE_STATE_IDLE                       = 0,
    EDDP_SWI_SEND_CLOCK_CHANGE_STATE_START                      = 1,
    EDDP_SWI_SEND_CLOCK_CHANGE_STATE_PASSIVATE_IRT              = 2,
    EDDP_SWI_SEND_CLOCK_CHANGE_STATE_PASSIVATE_CRT_PACK         = 3,
    EDDP_SWI_SEND_CLOCK_CHANGE_STATE_CHANGE_COMCYCLE            = 4,
    EDDP_SWI_SEND_CLOCK_CHANGE_STATE_SETUP_TRANSFER_END_OFF     = 5,
    EDDP_SWI_SEND_CLOCK_CHANGE_STATE_UPDATE_APPL_LENGTH_ON      = 6,
    EDDP_SWI_SEND_CLOCK_CHANGE_STATE_SETUP_TRANSFER_END_ON      = 7,
    EDDP_SWI_SEND_CLOCK_CHANGE_STATE_FINISH                     = 8
} EDDP_SWI_SEND_CLOCK_CHANGE_STATE_TYPE;

typedef struct _EDDP_SWI_SEND_CLOCK_CHANGE_SM_TYPE  EDDP_LOCAL_MEM_ATTR *   EDDP_SWI_SEND_CLOCK_CHANGE_SM_PTR_TYPE;
typedef struct _EDDP_SWI_SEND_CLOCK_CHANGE_SM_TYPE
{
    EDDP_SWI_SEND_CLOCK_CHANGE_STATE_TYPE       State;
    EDD_RSP                                     Response;
    EDD_UPPER_RQB_PTR_TYPE                      pRQB;
} EDDP_SWI_SEND_CLOCK_CHANGE_SM_TYPE;


/* isRTC3PSMRunningAsynMask			: Bit(0)=PortID1, Bit(1)=PortID2: is set if the RTC3PSM is running asynchron                    */
/* isRTC3PSMNewParamPresentMask		: Bit(0)=PortID1, Bit(1)=PortID2: is set if new RTC3-Params are present                         */
#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
#define EDDP_SWI_RTC3PSM_ASYN_PORT1_MASK            0x0001
#define EDDP_SWI_RTC3PSM_ASYN_PORT1_SHIFT           0
#define EDDP_SWI_RTC3PSM_ASYN_PORT2_MASK            0x0002
#define EDDP_SWI_RTC3PSM_ASYN_PORT2_SHIFT           1
#endif
/* IRTtopFwMode						: IRTtop Forwarding Mode (AbsoluteTime or RelativeTime) if PDIRData is present                  */
/*									  Init this parameter only at PrmCommit because the IRTtopFwMode is activated at PrmCommit!		*/
#define EDDP_SWI_IRTTOP_FW_MODE_INVALID             0xFF
#define EDDP_SWI_IRTTOP_FW_MODE_ABSOLUTETIME        (EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FORWARDINGMODE_ABSOLUTE)
#define EDDP_SWI_IRTTOP_FW_MODE_RELATIVETIME        (EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FORWARDINGMODE_RELATIVE)
/* IRTState							: IRT is active in HW                                                                           */
#define EDDP_SWI_IRTSTATE_IRT_TOP_MASK              0x01    // IRTtop is active in HW
#define EDDP_SWI_IRTSTATE_IRT_TOP_SHIFT             0
#define EDDP_SWI_IRTSTATE_IRT_DFP_MASK              0x02    // DFP is active in HW
#define EDDP_SWI_IRTSTATE_IRT_DFP_SHIFT             1
#define EDDP_SWI_IRTSTATE_BIT_OFF                   0x0     // Bit in IRTState activate
#define EDDP_SWI_IRTSTATE_BIT_ON                    0x1     // Bit in IRTState deactivate
/* DefaultRPortCount                : number of default ring ports from DPB															*/
/* MaxMRP_Instances                 : see <edd_usr.h>																				*/
/* MRPDefaultRoleInstance0          : see <edd_usr.h>																				*/
/* MRPSupportedRole                 : see <edd_usr.h>																				*/
/* MRPSupportedMultipleRole         : see <edd_usr.h>																				*/
/* SendClockChangeSm                : state machine for service SENDCLOCK_CHANGE                                                    */


typedef struct _EDDP_PHY_PORT_PARAMS_TYPE
{
    LSA_UINT32		        SMI_PHY_Address;
    LSA_UINT8               TxPort; // this bitmask defines all outbound ports for this port
    EDD_MAC_ADR_TYPE        Port_MAC_Address;  
    LSA_UINT8               IsWireless;
    LSA_UINT8               IsMDIX;
    EDDP_PHY_TYPE           PhyType;
    LSA_UINT8               IsGBitPhy;       
    LSA_UINT8               GBitSupport;     /* feature for HERA IFB, values are EDD_FEATURE_ENABLE and EDD_FEATURE_DISABLE */ 
} EDDP_PHY_PORT_PARAMS_TYPE;

#define EDDP_PHY_PORT_IS_GBIT_TRUE  1
#define EDDP_PHY_PORT_IS_GBIT_FALSE 2

typedef struct _EDDP_SWI_COMMAND_PARAMS
{
    EDDP_CMD_PARAM_TYPE     ClearUC_Table;
    EDDP_CMD_PARAM_TYPE     InsertUC_Entry;
    EDDP_CMD_PARAM_TYPE     Update_COMCYCLE;    
} EDDP_SWI_COMMAND_PARAMS;

typedef struct _EDDP_SWI_STATES
{
    LSA_UINT32      ReadyPHYsCnt;
    LSA_BOOL        PendingDeviceSetup2;
    LSA_BOOL        MC_Table_A_Active;
} EDDP_SWI_STATES;


/* ============================================================================ */
/*                    DDB-SWI Handle management structure                       */
/* ============================================================================ */
typedef struct _EDDP_DDB_SWI_TYPE  EDDP_LOCAL_MEM_ATTR *  EDDP_LOCAL_DDB_SWI_PTR_TYPE;
typedef struct _EDDP_DDB_SWI_TYPE
{   
    EDDP_PHY_FCT_TYPE			        PHY_FunctionPointers[EDDP_MAX_PORT_CNT +1]; //+1 to ease indexing with HwPortID [1..4]
    LSA_UINT32                          PHYAddressOffset;
    EDDP_SWI_MULTICAST_ENTRY_PTR_TYPE   pMC_Table_Shadow;
    LSA_UINT32                          MC_Table_Shadow_Entries_Count;
    LSA_UINT32                          MC_Table_Max_Entries;
    LSA_UINT32					        PortCnt;
    EDDP_SWI_STATES                     States;
    EDDP_PHY_PORT_PARAMS_TYPE	        PortParams[EDDP_MAX_PORT_CNT +1]; //+1 to ease indexing with HwPortID [1..4]
    EDDP_SWI_COMMAND_PARAMS             CommandParams;
    EDDP_RQB_QUEUE_TYPE                 WaitingRQBs_FDB; /* this queue is used by SWI when the FDB-Command-IF is busy */
    EDDP_RQB_QUEUE_TYPE                 PendingRQBs_FDB; /* this queue is used by SWI to remember FDB-RQBs when commands are running at Command Control asynchronously*/ 
    LSA_BOOL                            Multicast_Bridge_IEEE802RSTP_Forward;
    LSA_UINT32                          PTCP_TxDelay;
    LSA_UINT32                          PTCP_RxDelay;
    LSA_UINT32                          PRM_MaxBridgeDelay;

//  LSA_UINT32					        PortCnt;    // is calculated EDDP-internally
    EDDP_DDB_SWI_PORT_MAP_TYPE          PortMap;

    EDDP_DDB_SWI_LINK_STATUS_TYPE       LinkStatus[EDDP_MAX_PORT_CNT + 1];      // index 0 is auto (all ports)
    EDDP_DDB_SWI_LED_BLINK_TYPE         LEDBlink;

    EDD_MAC_ADR_TYPE                    IF_MAC_Address;
    EDD_IP_ADR_TYPE                     IF_IP_Address;

    LSA_BOOL                            SyncOk;
    LSA_BOOL                            AnnounceBoundary[EDDP_MAX_PORT_CNT +1][EDDP_PRM_DOMAIN_BOUNDARY_COUNT];   //+1 to ease indexing with Port ID

    EDDP_DDB_SWI_PRAMBLE_PRM_CFG_TYPE   PrmPreabmleCfg[EDDP_MAX_PORT_CNT+1];        //+1, local copy of preabmle settings after PRM_COMMIT

	EDDP_DDB_SWI_RTC3_PORT_STATE_TYPE   RTC3_PortState[EDDP_MAX_PORT_CNT +1];       //+1 to ease indexing with Port ID
    #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
	LSA_UINT8                           isRTC3PSMRunningAsynMask;
	LSA_UINT8                           isRTC3PSMNewParamPresentMask;
    #endif

	LSA_UINT32                          RtClass2_TopoState[EDDP_MAX_PORT_CNT +1];   //+1 to ease indexing with Port ID
    EDDP_DDB_SWI_CYCLE_INFO_TYPE        CycleInfo;
    LSA_UINT32                          DroppedFramesCounter[EDDP_MAX_PORT_CNT +1]; //+1 to ease indexing with Port ID

    LSA_UINT8                           IRTtopFwMode;
    LSA_UINT8                           IRTState;

    LSA_UINT32                          DefaultRPortCount;
    LSA_UINT32                          NoneRPortCount;
    LSA_UINT8                           MaxMRP_Instances;
    LSA_UINT8                           MaxMRPInterconn_Instances;
    LSA_UINT8                           MRPDefaultRoleInstance0; 
    LSA_UINT8                           MRPSupportedRole;
    LSA_UINT8                           MRPSupportedMultipleRole;
    LSA_UINT8                           SupportedMRPInterconnRole;
    LSA_UINT8                           MRAEnableLegacyMode;
    EDDP_SWI_SEND_CLOCK_CHANGE_SM_TYPE  SendClockChangeSm;
} EDDP_DDB_SWI_TYPE;


/* ============================================================================ */
/*                    HDB-SWI Handle management structure                       */
/* ============================================================================ */
typedef struct _EDDP_HDB_SWI_TYPE       EDDP_LOCAL_MEM_ATTR *       EDDP_LOCAL_HDB_SWI_PTR_TYPE;
typedef struct _EDDP_HDB_SWI_TYPE
{
    LSA_UINT32          RunningRQBCount;
} EDDP_HDB_SWI_TYPE;
/* RunningRQBCount      : count of current running RQBs in GlbCrt queue of this SWI channel         */


LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWIFillPHYLinkStatus(
    EDDP_PHY_LINK_STATUS_PTR_CONST_TYPE           pLinkStatus_In,
    EDDP_DDB_SWI_LINK_STATUS_PTR_TYPE             pLinkStatus_Out);

/*****************************************************************************/
/*  end of file EDDP_SWI_INC.H                                               */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_SWI_INC_H */
