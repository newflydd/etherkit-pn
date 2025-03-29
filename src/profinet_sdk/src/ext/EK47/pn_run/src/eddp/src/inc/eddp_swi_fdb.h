#ifndef EDDP_SWI_FDB_H                    /* ----- reinclude-protection ----- */
#define EDDP_SWI_FDB_H

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
/*  F i l e               &F: eddp_swi_fdb.h                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Headerfile for SWITCH FDB-Control functions                              */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  14.01.09    AB    initial version                                        */
#endif
/*****************************************************************************/
/*===========================================================================*/
/*                                  defines                                  */
/*===========================================================================*/
#define EDDP_SWI_FDB_SIZE_OF_MC_TABLE_32BIT	                    896
#define	EDDP_SWI_FDB_SIZE_OF_IRT_TABLE_A_32BIT                  1024

#define EDDP_SWI_FDB_SIZE_OF_UC_TABLE_A_32BIT_ERTEC_200P	    128    
#define EDDP_SWI_FDB_SIZE_OF_UC_TABLE_A_32BIT_HERA_IFA    	    4096
#define EDDP_SWI_FDB_SIZE_OF_UC_TABLE_A_32BIT_HERA_IFB    	    2048
#define EDDP_SWI_FDB_NUMBER_OF_USER_HERA                        2
#define EDDP_SWI_FDB_NUMBER_OF_PARAM_HERA                       32

// following defines are used at setup and of MC table
#define EDDP_SWI_FDB_FORWARD_FILTER                 0
#define EDDP_SWI_FDB_FORWARD_API_PORTS              1
#define EDDP_SWI_FDB_FORWARD_API                    2
#define EDDP_SWI_FDB_FORWARD_PORTS                  3
// following defines are used at setup of MC table or at EDD_SRV_MULTICAST_FWD_CTRL
#define EDDP_SWI_FDB_NO_SPECIAL_HANDLING            0
#define EDDP_SWI_FDB_SPECIAL_HANDLING_RSTP          1
#define EDDP_SWI_FDB_GROUP_RT_2_QUER                2
#define EDDP_SWI_FDB_GROUP_MRP_1                    3
#define EDDP_SWI_FDB_GROUP_MRP_2                    4
#define EDDP_SWI_FDB_GROUP_HSYNC                    5
#define EDDP_SWI_FDB_GROUP_MRP_IC_1                 6
#define EDDP_SWI_FDB_GROUP_MRP_IC_2                 7
#define EDDP_SWI_FDB_GROUP_MRP_IC_3                 8
#define EDDP_SWI_FDB_GROUP_DLR_1                    9
#define EDDP_SWI_FDB_GROUP_DLR_2                    10
#define EDDP_SWI_FDB_GROUP_DLR_3                    11


/*===========================================================================*/
/*                                 macros                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/

typedef struct _EDDP_SWI_MULTICAST_ENTRY_TYPE   EDDP_LOCAL_MEM_ATTR *   EDDP_SWI_MULTICAST_ENTRY_PTR_TYPE;
typedef struct _EDDP_SWI_MULTICAST_ENTRY_TYPE
{
    LSA_UINT32      MAC;
    LSA_UINT16      LowMAC;
    LSA_UINT16      HighMAC;
    LSA_UINT8	    Valid;
    LSA_UINT8	    FrameHandling;
    LSA_UINT8	    Redundancy;
    LSA_UINT8	    Fragmentation;
    LSA_UINT8	    BridgeDelay_Enable;     /* only used in PN-IP Rev1 */
    LSA_UINT8	    PriorityMapping;
    LSA_UINT8	    TxPort_API;
    LSA_UINT8	    LD_API;
    LSA_UINT8	    MgmtENable_API;
    LSA_UINT8	    MgmtPrio_API;
    LSA_UINT8	    TxPort_1;
    LSA_UINT8	    LD_1;
    LSA_UINT8	    MgmtENable_1;
    LSA_UINT8	    MgmtPrio_1;
    LSA_UINT8	    TxPort_2;
    LSA_UINT8	    LD_2;
    LSA_UINT8	    MgmtENable_2;
    LSA_UINT8	    MgmtPrio_2;
    LSA_UINT8	    TxPort_3;
    LSA_UINT8	    LD_3;
    LSA_UINT8	    MgmtENable_3;
    LSA_UINT8	    MgmtPrio_3;
    LSA_UINT8	    TxPort_4;
    LSA_UINT8	    LD_4;
    LSA_UINT8	    MgmtENable_4;
    LSA_UINT8	    MgmtPrio_4;
    LSA_UINT8	    UseType;
    LSA_UINT8	    DefaultTrafficClass;
    LSA_UINT8	    APIType;
    LSA_UINT8	    ResourceType;
    LSA_UINT8	    FastForwarding;
    LSA_BOOL        Static;                 /* marking this entry as 'static' -> can not be deleted */
    LSA_UINT32      RefCnt;                 /* stores RefCnt of Service EDD_SRV_MULTICAST */
    LSA_UINT8       ForwardingRule_API;     /* only used for MC-Table setup and restoring default settings */
    LSA_UINT8       ForwardingRule_Ports;   /* only used for MC-Table setup and restoring default settings */
    LSA_UINT8       SpecialHandling;        /* indicates special handling for certain static MC entries */
} EDDP_SWI_MULTICAST_ENTRY_TYPE;


/*===========================================================================*/
/*                                  data                                     */
/*===========================================================================*/
#if ( (defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT)) || (defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)) )
LSA_EXTERN const EDDP_SWI_MULTICAST_ENTRY_TYPE  EDDP_SWI_multicast_table_ERTEC200P[];
#endif

/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWI_fdb_setup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR _EDDP_SWI_switch_MC_table(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR _EDDP_SWI_Setup_Shadow_MC_Table(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR _EDDP_SWI_Setup_MC_Default_Px(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_Change_MC_Entry_Parameters(
	EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_SWI_MULTICAST_ENTRY_PTR_TYPE       pMC_Entry,
    LSA_UINT32                              Index);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_MC_Entry_Set_To_Default(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  Index);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWI_Change_Forwarding(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  Index,
    EDD_RQB_PORTID_MODE_TYPE    PortIDMode,
    LSA_BOOL                    Forward);
    
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWI_ChangeAnnounceFwd(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  Index,
    LSA_UINT32                  PortID,
    LSA_BOOL                    Forward,
	LSA_UINT8                   SyncId);
    
LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_SWI_MC_Entry_Get_Index(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_MAC_ADR_TYPE            MAC_Address);
    
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_SWI_MC_Entry_Set_DomainBoundary(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  Index,
    LSA_UINT32                  PortID,
    LSA_BOOL                    Ingress,
    LSA_BOOL                    Egress,
    LSA_BOOL                    isDomainBoundaryAnnounce);

/*****************************************************************************/
/*  end of file EDDP_SWI_FDB.H                                               */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_SWI_FDB_H */
