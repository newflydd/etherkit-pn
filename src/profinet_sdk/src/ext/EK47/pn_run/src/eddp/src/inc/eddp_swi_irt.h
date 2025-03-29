#ifndef EDDP_SWI_IRT_H                    /* ----- reinclude-protection ----- */
#define EDDP_SWI_IRT_H

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
/*  F i l e               &F: eddp_swi_irt.h                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Headerfile for SWITCH IRT related functions                              */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  25.06.09    AB    initial version                                        */
#endif
/*****************************************************************************/
/*===========================================================================*/
/*                                  defines                                  */
/*===========================================================================*/
/* ------------------------------------------------------------------------------------------------------------------------ */
// defines for phase control
#define EDDP_SWI_PHASE_ENABLE_OFF                                   (PNIP_REG_PHASEENABLE_P1_RST__VAL)
#define EDDP_SWI_PHASE_ENABLE_RX_RED                                (PNIP_REG_PHASEENABLE_P1__MSK_ENABLE_RXRED)
#define EDDP_SWI_PHASE_ENABLE_RX_ORANGE                             (PNIP_REG_PHASEENABLE_P1__MSK_ENABLE_RXORANGE)
#define EDDP_SWI_PHASE_ENABLE_TX_RED                                (PNIP_REG_PHASEENABLE_P1__MSK_ENABLE_TXRED)
#define EDDP_SWI_PHASE_ENABLE_TX_ORANGE                             (PNIP_REG_PHASEENABLE_P1__MSK_ENABLE_TXORANGE)
#define EDDP_SWI_PHASE_ENABLE_START_TX_RED                          (PNIP_REG_PHASEENABLE_P1__MSK_START_TXRED)
/* ------------------------------------------------------------------------------------------------------------------------ */
// defines for IRT-Table
#define EDDP_SWI_IRT_ENTRY_TXPORT_1                                 0x00000002      // TxPort(1)
#define EDDP_SWI_IRT_ENTRY_TXPORT_2                                 0x00000004      // TxPort(2)
#define EDDP_SWI_IRT_ENTRY_TXPORT_3                                 0x00000008      // TxPort(3)
#define EDDP_SWI_IRT_ENTRY_TXPORT_4                                 0x00000010      // TxPort(4)
#define EDDP_SWI_IRT_ENTRY_TXPORTS                                  0x0000001E      // TxPort(4:1)
#define EDDP_SWI_IRT_ENTRY_TXPORTS_API                              0x0000001F      // TxPort(4:0)
#define EDDP_SWI_IRT_ENTRY_EXACT_TIME                               0x00000040
#define EDDP_SWI_IRT_ENTRY_MINIMUM_TIME                             0x00000000
#define EDDP_SWI_IRT_ENTRY_VALID                                    0x00000080
/* ------------------------------------------------------------------------------------------------------------------------ */
// defines for IRT_DelayTime
#define EDDP_SWI_IRT_DELAYTIME_COMPENSATION                         75              // 75 ns
#define EDDP_SWI_IRT_DELAYTIME_MAX                                  262000          // 262 µs
/* ------------------------------------------------------------------------------------------------------------------------ */


/*===========================================================================*/
/*                                 macros                                    */
/*===========================================================================*/
#define EDDP_SWI_SET_PHASE_ENTRY_RED(bActive, AddrOffset)                       \
            (   ((bActive) ? 0x00000002 : 0x00000003)                           \
                | (((LSA_UINT32)AddrOffset) << 8)                               \
            )

#define EDDP_SWI_SET_PHASE_ENTRY_ORANGE(bActive, AddrOffset)                    \
            (   ((bActive) ? 0x00000004 : 0x0000000C)                           \
                | (((LSA_UINT32)AddrOffset) << 16)                              \
            )

#define EDDP_SWI_SET_PHASE_ENTRY_GREEN(bActive, AddrOffset)                     \
            (   ((bActive) ? 0x00000000 : 0x00000030)                           \
                | (((LSA_UINT32)AddrOffset) << 24)                              \
            )

#define EDDP_SWI_SET_PHASE_ENTRY_TO_ALL_GREEN(phase_entry)                      \
            (phase_entry = phase_entry & ~0x0000003F)

/* -------------------------------------------------------------------------------------------- */
/* IRT entry for "AbsoluteTime-Mode" (31:0)                                                     */
/* -------------------------------------------------------------------------------------------- */
/*  4:0     TxPort      : API(0), Port(1:4) -> it can be ORed                                   */
/*  5       reserved    : ---                                                                   */
/*  6       ExactTime   : exact(0x1), minimum(0x0)                                              */
/*  7       Valid       : this entry is valid / not valid                                       */
/*  31:8    TxTime      : sending or forwarding time of the frame (8ns resolution)              */
/* -------------------------------------------------------------------------------------------- */
#define EDDP_SWI_SET_IRT_ENTRY_ABSOLUTE(TxPort, ExactTime, TxTime)                  \
            (  (TxPort & EDDP_SWI_IRT_ENTRY_TXPORTS_API)                            \
             | (ExactTime)                                                          \
             | (EDDP_SWI_IRT_ENTRY_VALID)                                           \
             | ((TxTime / EDDP_UNIT_RESOLUTION_8) << 8)                             \
            )

/* -------------------------------------------------------------------------------------------- */
/* IRT entry for "RelativeTime-Mode" (7:0)                                                      */
/* -------------------------------------------------------------------------------------------- */
/*  4:0     TxPort      : API(0), Port(1:4) -> it can be ORed                                   */
/*  5       reserved    : ---                                                                   */
/*  6       ExactTime   : exact(0x1), minimum(0x0)                                              */
/*  7       Valid       : this entry is valid / not valid                                       */
/* -------------------------------------------------------------------------------------------- */
#define EDDP_SWI_SET_IRT_ENTRY_RELATIVE(TxPort, ExactTime)                          \
            (  (TxPort & EDDP_SWI_IRT_ENTRY_TXPORTS_API)                            \
             | (ExactTime)                                                          \
             | (EDDP_SWI_IRT_ENTRY_VALID)                                           \
            )


/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                  data                                     */
/*===========================================================================*/


/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_PRM_Configure_IRT (
    EDDP_LOCAL_DDB_PTR_TYPE      pDDB );

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_PRM_Disable_IRT(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Setup_Phase_List_Base_Addr (
	EDDP_LOCAL_DDB_PTR_TYPE      pDDB );

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Setup_Phases_RTC3 (
	EDDP_LOCAL_DDB_PTR_TYPE      pDDB );
	
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Setup_IRT_Table(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Setup_FastForwarding(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Setup_TxFragmentation(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Setup_DomainBoundaryPort(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  PortID,
    LSA_BOOL                    isIRTPortStatusRun);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Set_IRT_DelayTime(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT32                  PortID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Trigger_PreambleLength(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  PortID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Trigger_RTC3_PortState(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  PortID);

#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Trigger_RTC3_PortState_Rev1(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  PortID);
#endif
	
#if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Trigger_RTC3_PortState_RevOther(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  PortID);
#endif

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWI_Check_PDIRData(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Reset_RsvInterval_Red_Orange_Param(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  PortID);

#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Setup_RTC3PSMNewParamPresentMask(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  PortID,
    LSA_BOOL                    isSet);
#endif

#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_SWI_IsSet_RTC3PSMNewParamPresentMask(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  PortID);
#endif

#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Setup_RTC3PSMRunningAsynMask(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  PortID,
    LSA_BOOL                    isSet);
#endif

#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_SWI_IsSet_RTC3PSMRunningAsynMask(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  PortID);
#endif

#ifdef EDDP_PRM_TEST_RECORD_ON
LSA_VOID EDDP_SWI_TEST_Enable_Only_Red_Phase (
    EDDP_LOCAL_DDB_PTR_TYPE      pDDB,
    LSA_UINT32                   HwPortID);
    
LSA_VOID EDDP_SWI_TEST_Enable_Red_and_Green_Phase (
    EDDP_LOCAL_DDB_PTR_TYPE      pDDB,
    LSA_UINT32                   HwPortID);    
#endif
/*****************************************************************************/
/*  end of file EDDP_SWI_IRT.H                                               */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_SWI_IRT_H */
