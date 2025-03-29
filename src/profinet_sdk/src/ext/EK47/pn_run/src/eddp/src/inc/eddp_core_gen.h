#ifndef EDDP_CORE_GEN_H                  /* ----- reinclude-protection ----- */
#define EDDP_CORE_GEN_H

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
/*  F i l e               &F: eddp_core_gen.h                           :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Device/Handle                                                            */
/*  Defines constants, types, macros and prototyping for EDDP                */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  23.10.08    AB    initial version.                                       */
#endif
/*****************************************************************************/

/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/

/*===========================================================================*/
/*                                defines                                    */
/*===========================================================================*/
#define EDDP_GEN_QEUER_ENTRIES_COUNT    32

/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GENOpenChannel(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_LOCAL_HDB_PTR_TYPE     pHDB,
	LSA_HANDLE_TYPE             Handle,
	EDDP_SYSTEM_CDB_PTR_TYPE    pCDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GENDeviceSetup(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DSB_PTR_TYPE     pDSB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GENDeviceShutdown(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB);
													
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GENDeviceOpen(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DPB_PTR_TYPE     pDPB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GENDeviceIFSetup( 
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DSB_PTR_TYPE     pDSB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GENDeviceIFShutdown(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GENDeviceClose(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GENCloseChannel(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_LOCAL_HDB_PTR_TYPE     pHDB,
	LSA_HANDLE_TYPE             Handle);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GENLEDBlink_Toggle( 
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GENLEDBlink(
	EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);
												
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GENRequest(
	EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GEN_ISR_LinkChange(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT32                  EventNr);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GENIndicateLinkChange_Ext(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  PortID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GENIndicateLinkChange(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  PortID);
    
LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_GENFindFastestPort(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GEN_Get_Params(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GEN_Get_Port_Params(
	EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GEN_Add_Remove_Multicast_Address(
	EDD_UPPER_RQB_PTR_TYPE      pRQB,
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GEN_Add_Multicast_Address(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_MAC_ADR_TYPE            MAC_Address,
    LSA_UINT32*                 RefCnt);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GEN_Remove_Multicast_Address(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_MAC_ADR_TYPE            MAC_Address,
    LSA_UINT32*                 RefCnt);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GEN_Move_MC_Table_Up_Down(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  Index,
    LSA_BOOL                    MoveUp);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GEN_Multicast_Fwd_Ctrl(
	EDD_UPPER_MULTICAST_FWD_CTRL_PTR_TYPE   pParam,
	EDDP_LOCAL_DDB_PTR_TYPE                 pDDB);
	
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GENGetLinkStatus(
	EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);
    
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GENSetRemotePortState(
	EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GEN_Set_IP_Address(
	EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GEN_RequestResetStatistics(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GEN_RequestGetStatistics(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GEN_RequestGetStatisticsAll(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GEN_GetPortStatistics(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_LOCAL_GET_STATISTICS_PTR_TYPE      pStats,
    LSA_UINT32                              HwPortID);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GEN_GetIfaceStatistics(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_LOCAL_GET_STATISTICS_PTR_TYPE      pStats);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GEN_SubStatistics(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_LOCAL_GET_STATISTICS_PTR_TYPE      pNew,
    EDDP_LOCAL_GET_STATISTICS_PTR_TYPE      pSnap,
    LSA_UINT8                               Reset);

#if 1 /** RZ/T2M additional codes */
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_RequestSetLineDelay(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GEN_RequestSetSyncState(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);
#endif
/*****************************************************************************/
/*  end of file eddp_core_gen.h                                              */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_CORE_GEN_H */
