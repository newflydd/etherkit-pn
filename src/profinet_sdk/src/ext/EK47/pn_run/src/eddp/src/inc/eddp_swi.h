#ifndef EDDP_SWI_H                       /* ----- reinclude-protection ----- */
#define EDDP_SWI_H

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
/*  F i l e               &F: eddp_swi.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Headerfile for SWITCH component                                          */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  06.11.08    AB    initial version                                        */
/*  13.12.10    JS    added EDDP_SWI_REG_UC_AGING_DEFAULT_INIT                */
#endif
/*****************************************************************************/

/*===========================================================================*/
/*                                  defines                                  */
/*===========================================================================*/
#define EDDP_SWI_BLOCKED_PRIOMASK_FORWARDING                0x00000000      // clear all bits
#define EDDP_SWI_BLOCKED_PRIOMASK_BLOCKING                  0x0000C3FF      // set block queue-prio 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 14, 15

#define EDDP_SWI_SYNC_MAX_TIME_LIST_ENTRIES_PER_PORT        EDDP_PRM_MAX_ASSIGNMENTS*6     
#define EDDP_SWI_SYNC_TIME_LIST_ENTRY_FOR_GREEN_PHASE       1

#define EDDP_SWI_FRAME_AGING_CYCLE_CALC_FACTOR_REV3         32000           // calculation factor for frame aging cycle

#define EDDP_SWI_ETHERTYPE_DLR                              0x80E1          //Ethertype for Ethernet/IP DLR frames

/* ------------------------------------------------------------------------ */
/* Values for SWI-Register                                                  */
/* ------------------------------------------------------------------------ */
// register "Traffic_Mode"
#define EDDP_SWI_REG_TRAFFIC_MODE_INIT                      0x00000AAA

// register "IRT_DelaySafety"
#define EDDP_SWI_REG_IRT_DELAYSAFETY_100M_INIT_PNIP_REV12   0x0828      // 2088 ns
#define EDDP_SWI_REG_IRT_DELAYSAFETY_100M_INIT_PNIP_REV3    0x0BB8      // 3000 ns, TFS2073493
#define EDDP_SWI_REG_IRT_DELAYSAFETY_1G_INIT                0xFA        // 250 ns

// register "SafetyMargin"
#define EDDP_SWI_REG_SAFETYMARGIN_100M_INIT_PNIP_REV1       0x230       // 560 ns
#define EDDP_SWI_REG_SAFETYMARGIN_100M_INIT_PNIP_REV2       0x50        // 80 ns
#define EDDP_SWI_REG_SAFETYMARGIN_1G_INIT                   0x50        // 80 ns

// register "UC_Aging"
#define EDDP_SWI_REG_UC_AGING_DEFAULT_INIT                  0x7530      // 30000 ms

// register "FrameAging_Time_Px"
#define EDDP_SWI_REG_FRAMEAGING_TIME_INIT                   0x3BA30000

// mask for QueuePrio8-RTC3 in register FrameAgingStatus
#define EDDP_SWI_REG_FRAMEAGING_STATUS_QUEUEPRIO8_RTC3_MASK         0x100
// mask for QueuePrio15-RTC3 in register FrameAgingStatus
#define EDDP_SWI_REG_FRAMEAGING_STATUS_QUEUEPRIO15_RTC3_MASK        0x8000

// mask MACTBufferOverflowErr in register MacErrorStatus
#define EDDP_SWI_REG_MACERRORSTATUS_MACTBUFFEROVERFLOWERR_MASK      0x100
// mask MACTBufferUnderflowErr in register MacErrorStatus
#define EDDP_SWI_REG_MACERRORSTATUS_MACTBUFFERUNDERFLOWERR_MASK     0x200

// register "HOL_Control"
#define EDDP_SWI_REG_HOL_CONTROL_PA_INIT                    0x0000
//#define EDDP_SWI_REG_HOL_CONTROL_PB_INIT                    (EDDP_SWI_REG_HOL_CONTROL_PA_INIT)
#define EDDP_SWI_REG_HOL_CONTROL_IFA_INIT                   0xFFFE
//#define EDDP_SWI_REG_HOL_CONTROL_IFB_INIT                   (EDDP_SWI_REG_HOL_CONTROL_IFA_INIT)

// register "PreambleLength_Px"
#define EDDP_SWI_REG_PREAMBLELENGTH_SHORT_PREAMBLE          0x00010001
#define EDDP_SWI_REG_PREAMBLELENGTH_LONG_PREAMBLE           0x00070007

// YellowSafetyMargin for register "TxFrameLength_Px"
#define EDDP_SWI_REG_TXFRAMELENGTH_YELLOW_SAFETY_MARGIN_PNIP_REV1   32
#define EDDP_SWI_REG_TXFRAMELENGTH_YELLOW_SAFETY_MARGIN_PNIP_REV2   26

/* ------------------------------------------------------------------------ */

/* ---------------------------------------------------------------------------------------------------------------- */
/* QDB-Barrier                                                                                                      */
/* last update: 2014-03-20                                                                                          */
/* ---------------------------------------------------------------------------------------------------------------- */
/* !!!!! EDDP works only with PERIF and ACW_Wait=1 in PPM-ACW, so <EDDP_CRT_SUPPORT_ACW_WAIT_ON> must be defined    */
/* ---------------------------------------------------------------------------------------------------------------- */
/* ---------------------------- */
/* ERTEC200P        - Rev1      */
/* ERTEC200P_FPGA   - Rev1      */
/* ---------------------------- */
#define EDDP_SWI_QDB_BARRIER_IF_RES_0_ERTEC200P_REV1        48      // Interface    - OTHER
#define EDDP_SWI_QDB_BARRIER_IF_RES_1_ERTEC200P_REV1        24      // Interface    - RTA
#define EDDP_SWI_QDB_BARRIER_IF_RES_2_ERTEC200P_REV1        2       // Interface    - MRP
#define EDDP_SWI_QDB_BARRIER_IF_RES_3_ERTEC200P_REV1        12      // Interface    - LLDP
#define EDDP_SWI_QDB_BARRIER_IF_RES_4_ERTEC200P_REV1        16      // Interface    - PTCP
#define EDDP_SWI_QDB_BARRIER_IF_RES_5_ERTEC200P_REV1        82      // Interface    - RT_L
#define EDDP_SWI_QDB_BARRIER_IF_RES_6_ERTEC200P_REV1        15      // Interface    - RT_H, RT_H1
#define EDDP_SWI_QDB_BARRIER_IF_RES_7_ERTEC200P_REV1        15      // Interface    - RT_H2

#define EDDP_SWI_QDB_BARRIER_P_RES_0_ERTEC200P_REV1         150     // Port         - OTHER
#define EDDP_SWI_QDB_BARRIER_P_RES_1_ERTEC200P_REV1         6       // Port         - RTA
#define EDDP_SWI_QDB_BARRIER_P_RES_2_ERTEC200P_REV1         4       // Port         - MRP
#define EDDP_SWI_QDB_BARRIER_P_RES_3_ERTEC200P_REV1         24      // Port         - LLDP
#define EDDP_SWI_QDB_BARRIER_P_RES_4_ERTEC200P_REV1         28      // Port         - PTCP
#define EDDP_SWI_QDB_BARRIER_P_RES_5_ERTEC200P_REV1         50      // Port         - RT_L
#define EDDP_SWI_QDB_BARRIER_P_RES_6_ERTEC200P_REV1         30      // Port         - RT_H

#define EDDP_SW_QDB_MAX_ERTEC200P_REV1                      560
/* ---------------------------- */
/* ERTEC200P        - Rev2      */
/* ERTEC200P_FPGA   - Rev2      */
/* ---------------------------- */
#define EDDP_SWI_QDB_BARRIER_IF_RES_0_ERTEC200P_REV2        48      // Interface    - OTHER
#define EDDP_SWI_QDB_BARRIER_IF_RES_1_ERTEC200P_REV2        24      // Interface    - RTA
#define EDDP_SWI_QDB_BARRIER_IF_RES_2_ERTEC200P_REV2        2       // Interface    - MRP
#define EDDP_SWI_QDB_BARRIER_IF_RES_3_ERTEC200P_REV2        12      // Interface    - LLDP
#define EDDP_SWI_QDB_BARRIER_IF_RES_4_ERTEC200P_REV2        16      // Interface    - PTCP
#define EDDP_SWI_QDB_BARRIER_IF_RES_5_ERTEC200P_REV2        82      // Interface    - RT_L
#define EDDP_SWI_QDB_BARRIER_IF_RES_6_ERTEC200P_REV2        15      // Interface    - RT_H, RT_H1
#define EDDP_SWI_QDB_BARRIER_IF_RES_7_ERTEC200P_REV2        15      // Interface    - RT_H2

#define EDDP_SWI_QDB_BARRIER_P_RES_0_ERTEC200P_REV2         150     // Port         - OTHER
#define EDDP_SWI_QDB_BARRIER_P_RES_1_ERTEC200P_REV2         6      // Port         - RTA
#define EDDP_SWI_QDB_BARRIER_P_RES_2_ERTEC200P_REV2         4       // Port         - MRP
#define EDDP_SWI_QDB_BARRIER_P_RES_3_ERTEC200P_REV2         24      // Port         - LLDP
#define EDDP_SWI_QDB_BARRIER_P_RES_4_ERTEC200P_REV2         28      // Port         - PTCP
#define EDDP_SWI_QDB_BARRIER_P_RES_5_ERTEC200P_REV2         150     // Port         - RT_L
#define EDDP_SWI_QDB_BARRIER_P_RES_6_ERTEC200P_REV2         30      // Port         - RT_H

#define EDDP_SW_QDB_MAX_ERTEC200P_REV2                      660
     
/* ---------------------------- */
/* HERA             - Rev3      */
/* HERA_FPGA        - Rev3      */
/* Interface A                  */
/* ---------------------------- */
#define EDDP_SWI_QDB_BARRIER_IF_RES_0_HERA_IF_A_REV3        96      // Interface    - OTHER
#define EDDP_SWI_QDB_BARRIER_IF_RES_1_HERA_IF_A_REV3        24      // Interface    - RTA
#define EDDP_SWI_QDB_BARRIER_IF_RES_2_HERA_IF_A_REV3        12      // Interface    - MRP
#define EDDP_SWI_QDB_BARRIER_IF_RES_3_HERA_IF_A_REV3        12      // Interface    - LLDP
#define EDDP_SWI_QDB_BARRIER_IF_RES_4_HERA_IF_A_REV3        26      // Interface    - PTCP
#define EDDP_SWI_QDB_BARRIER_IF_RES_5_HERA_IF_A_REV3        172     // Interface    - RT_L1
#define EDDP_SWI_QDB_BARRIER_IF_RES_6_HERA_IF_A_REV3        24      // Interface    - RT_H, RT_H1
#define EDDP_SWI_QDB_BARRIER_IF_RES_7_HERA_IF_A_REV3        24      // Interface    - RT_H2

#define EDDP_SWI_QDB_BARRIER_IF_RES_8_HERA_IF_A_REV3        24      // Interface    - RT_H3
#define EDDP_SWI_QDB_BARRIER_IF_RES_9_HERA_IF_A_REV3        24      // Interface    - RT_H4
#define EDDP_SWI_QDB_BARRIER_IF_RES_10_HERA_IF_A_REV3       172     // Interface    - RT_L2
#define EDDP_SWI_QDB_BARRIER_IF_RES_11_HERA_IF_A_REV3       172     // Interface    - RT_L3
#define EDDP_SWI_QDB_BARRIER_IF_RES_12_HERA_IF_A_REV3       172     // Interface    - RT_L4

#define EDDP_SWI_QDB_BARRIER_P_RES_0_HERA_IF_A_REV3         344     // Port         - OTHER
#define EDDP_SWI_QDB_BARRIER_P_RES_1_HERA_IF_A_REV3         96      // Port         - RTA
#define EDDP_SWI_QDB_BARRIER_P_RES_2_HERA_IF_A_REV3         8       // Port         - MRP
#define EDDP_SWI_QDB_BARRIER_P_RES_3_HERA_IF_A_REV3         48      // Port         - LLDP
#define EDDP_SWI_QDB_BARRIER_P_RES_4_HERA_IF_A_REV3         80      // Port         - PTCP
#define EDDP_SWI_QDB_BARRIER_P_RES_5_HERA_IF_A_REV3         344     // Port         - RT_L
#define EDDP_SWI_QDB_BARRIER_P_RES_6_HERA_IF_A_REV3         60      // Port         - RT_H

#define EDDP_SW_QDB_MAX_HERA_IF_A_REV3                      2224

/* ---------------------------- */
/* HERA             - Rev3      */
/* HERA_FPGA        - Rev3      */
/* Interface B                  */
/* ---------------------------- */
#define EDDP_SWI_QDB_BARRIER_IF_RES_0_HERA_IF_B_REV3        48      // Interface    - OTHER
#define EDDP_SWI_QDB_BARRIER_IF_RES_1_HERA_IF_B_REV3        24      // Interface    - RTA
#define EDDP_SWI_QDB_BARRIER_IF_RES_2_HERA_IF_B_REV3        12      // Interface    - MRP
#define EDDP_SWI_QDB_BARRIER_IF_RES_3_HERA_IF_B_REV3        12      // Interface    - LLDP
#define EDDP_SWI_QDB_BARRIER_IF_RES_4_HERA_IF_B_REV3        22      // Interface    - PTCP
#define EDDP_SWI_QDB_BARRIER_IF_RES_5_HERA_IF_B_REV3        172     // Interface    - RT_L1
#define EDDP_SWI_QDB_BARRIER_IF_RES_6_HERA_IF_B_REV3        24      // Interface    - RT_H, RT_H1
#define EDDP_SWI_QDB_BARRIER_IF_RES_7_HERA_IF_B_REV3        24      // Interface    - RT_H2

#define EDDP_SWI_QDB_BARRIER_IF_RES_10_HERA_IF_B_REV3       172     // Interface    - RT_L2

#define EDDP_SWI_QDB_BARRIER_P_RES_0_HERA_IF_B_REV3         172     // Port         - OTHER
#define EDDP_SWI_QDB_BARRIER_P_RES_1_HERA_IF_B_REV3         48      // Port         - RTA
#define EDDP_SWI_QDB_BARRIER_P_RES_2_HERA_IF_B_REV3         4       // Port         - MRP
#define EDDP_SWI_QDB_BARRIER_P_RES_3_HERA_IF_B_REV3         48      // Port         - LLDP
#define EDDP_SWI_QDB_BARRIER_P_RES_4_HERA_IF_B_REV3         40      // Port         - PTCP
#define EDDP_SWI_QDB_BARRIER_P_RES_5_HERA_IF_B_REV3         172     // Port         - RT_L
#define EDDP_SWI_QDB_BARRIER_P_RES_6_HERA_IF_B_REV3         30      // Port         - RT_H

#define EDDP_SW_QDB_MAX_HERA_IF_B_REV3                      1140

typedef struct _EDDP_BARRIER_VALUE_TYPE EDDP_LOCAL_MEM_ATTR *   EDDP_BARRIER_VALUE_PTR_TYPE;
typedef struct _EDDP_BARRIER_VALUE_TYPE
{
    LSA_UINT32     P_0;
    LSA_UINT32     P_1;
    LSA_UINT32     P_2;
    LSA_UINT32     P_3;
    LSA_UINT32     P_4;
    LSA_UINT32     P_5;
    LSA_UINT32     P_6;
    LSA_UINT32     P_7;
    LSA_UINT32     P_8;
    LSA_UINT32     P_9;
    LSA_UINT32     P_10;
    LSA_UINT32     P_11;
    LSA_UINT32     P_12;
    LSA_UINT32     P_13;
    LSA_UINT32     P_14;
    LSA_UINT32     P_15;

    LSA_UINT32     IF_0;
    LSA_UINT32     IF_1;
    LSA_UINT32     IF_2;
    LSA_UINT32     IF_3;
    LSA_UINT32     IF_4;
    LSA_UINT32     IF_5;
    LSA_UINT32     IF_6;
    LSA_UINT32     IF_7;
    LSA_UINT32     IF_8;
    LSA_UINT32     IF_9;
    LSA_UINT32     IF_10;
    LSA_UINT32     IF_11;
    LSA_UINT32     IF_12;
    LSA_UINT32     IF_13;
    LSA_UINT32     IF_14;
    LSA_UINT32     IF_15;

} EDDP_BARRIER_VALUE_TYPE;

/* ---------------------------------------------------------------------------------------------------------------- */


/* ---------------------------------------- */
/* PTCP Rx/Tx Delay Values (ns)             */
/* ---------------------------------------- */
#define EDDP_SWI_PTCP_RX_DELAY_FPGA                 0
#define EDDP_SWI_PTCP_RX_DELAY_ERTEC200P            0
#define EDDP_SWI_PTCP_TX_DELAY_FPGA                 0
#define EDDP_SWI_PTCP_TX_DELAY_ERTEC200P            0

/* ---------------------------------------- */
/* defines for CycleBaseFactor              */
/* ---------------------------------------- */
#define EDDP_SWI_CYCLE_BASE_FACTOR_DISABLE_IRT      0


/*===========================================================================*/
/*                                 macros                                    */
/*===========================================================================*/


/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/


/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWIRequest( 
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWIDeviceSetup1(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DSB_PTR_TYPE     pDSB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWI_Init_Portmapping_HERA(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWIDeviceSetup2(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DSB_PTR_TYPE     pDSB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWIDeviceShutdown(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);
													
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWIDeviceOpen(
    EDDP_LOCAL_DDB_PTR_TYPE	    pDDB,
    EDDP_UPPER_DPB_PTR_TYPE     pDPB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWIDeviceIFSetup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DSB_PTR_TYPE	    pDSB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWIDeviceIFShutdown(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWIDeviceClose(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

EDD_RSP  EDDP_LOCAL_FCT_ATTR  EDDP_SWI_PHY_Reg_Write(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32			 	    HwPortID,
    LSA_UINT32				    RegisterNumber,
    LSA_UINT32				    Value);

EDD_RSP  EDDP_LOCAL_FCT_ATTR  EDDP_SWI_PHY_Reg_Read(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32				    HwPortID,
    LSA_UINT32				    RegisterNumber,
    LSA_UINT32*                 pValue);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_CommandDoneCbf_FDB(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_CMD_PARAM_PTR_TYPE     pParam,
    EDD_RSP                     Response);
    
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_CommandDoneCbf_GSYNC(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_CMD_PARAM_PTR_TYPE     pParam,
    EDD_RSP                     Response);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_FinishPHYSetup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32				    HwPortID,
    LSA_BOOL                    bKeepPhyStatus);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWI_Request_Set_Port_State( 
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Set_Port_State_HW(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PortState,
    LSA_UINT32                  HwPortID);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_SWI_IsFdbCmdRunning(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWIOpenChannel(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB,
    LSA_HANDLE_TYPE             Handle,
    EDDP_SYSTEM_CDB_PTR_TYPE    pCDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWICloseChannel(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_LOCAL_HDB_PTR_TYPE     pHDB,
	LSA_HANDLE_TYPE             Handle);
	
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWI_Set_HOL_Barriers(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);
    
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWI_Change_ComCycle( 
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_ISR_MacErr(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_ISR_FragmentationFault(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_ISR_FrameAging_Expired(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT32                  EventNr);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_ISR_QCW_Empty(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_ISR_HOL_Expired_IFA(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_ISR_HOL_Expired_PA(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWISendClockChangeStateMachine(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDD_RSP                     ResponseSm);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWISendClockChangeStateMachineCbf(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDD_RSP                     Response);



/*****************************************************************************/
/*  end of file EDDP_SWI.H                                                   */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_SWI_H */
