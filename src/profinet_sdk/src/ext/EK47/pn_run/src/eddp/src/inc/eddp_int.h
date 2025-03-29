
#ifndef EDDP_INT_H                       /* ----- reinclude-protection ----- */
#define EDDP_INT_H

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
/*  F i l e               &F: eddp_int.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Internal headerfile. includes all internal headers. prototypes           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  14.08.08    JS    initial version.                                       */
/*  22.12.08    LRG   eddp_k32_inc.h, EDDP_MODULE_ID_EDDP_K32                */
/*  19.05.09    LRG   EDDP_MODULE_ID_EDDP_K32_FW                             */
/*  04.10.10    JS    moved compile errors to eddp_pls.h                     */
/*  24.11.10    JS    changes to prevent compiler warnings                   */
#endif
/*****************************************************************************/

#include "eddp_trc.h"                    /* Trace                            */
#include "eddp_core_dbg.h"               /* Debug                            */
#include "eddp_core_glb.h"               /* Global                           */
#include "eddp_core_sm.h"                /* Statemachine                     */
#include "eddp_hal_inc.h"                /* HAL-Include before eddp_dev.h    */
#include "eddp_cmd_inc.h"                /* CMD-Control                      */
#include "eddp_prm_inc.h"                /* PRM-Include before eddp_dev.h and before eddp_crt_inc.h */
#include "eddp_crt_inc.h"                /* CRT-Include before eddp_dev.h    */
#include "eddp_nrt_inc.h"                /* NRT-Include before eddp_dev.h    */
#include "eddp_swi_inc.h"                /* SWI-Include                      */
#include "eddp_k32_inc.h"                /* K32-Include before eddp_dev.h    */
#include "eddp_core_isr.h"               /* InterruptServiceRoutine          */
#include "eddp_i2c.h"                    /* I2C headerfile (internal API)    */

#include "eddp_core_dev.h"
#include "eddp_core_gen.h"               /* Generic Core                     */

#include "eddp_hwf_val.h"                /* hardware filter                  */
#include "eddp_hal_usr.h"                /* HAL access                       */
#if( 0 == (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200P))
#include "eddp_llif.h"
#endif
#if (EDD_CFG_CSRT_MAX_PROVIDER_GROUP > EDDP_CRT_MAX_PROVIDER_GROUP)
    #error  "EDD_CFG_CSRT_MAX_PROVIDER_GROUP > EDDP_CRT_MAX_PROVIDER_GROUP"
#endif


#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

/* ========================================================================= */
/*                                                                           */
/* ModuelIDs                                                                 */
/*                                                                           */
/* Note : This is only for reference. The value is hardcoded in every C-File!*/
/*                                                                           */
/* ========================================================================= */

/* EDDP module IDs used in C-Files */

/* CORE: 1..20 */
#define EDDP_MODULE_ID_EDDP_CORE_SYS         1
#define EDDP_MODULE_ID_EDDP_CORE_DEV         2
#define EDDP_MODULE_ID_EDDP_CORE_GLB         3
#define EDDP_MODULE_ID_EDDP_CORE_USR         4
#define EDDP_MODULE_ID_EDDP_CORE_DBG         5
#define EDDP_MODULE_ID_EDDP_CORE_SM          6
#define EDDP_MODULE_ID_EDDP_CORE_OC          7
#define EDDP_MODULE_ID_EDDP_CORE_TIM         8
#define EDDP_MODULE_ID_EDDP_CORE_GEN         9
#define EDDP_MODULE_ID_EDDP_CORE_ISR         10

/* CRT: 21..40 */
#define EDDP_MODULE_ID_EDDP_CRT             21
#define EDDP_MODULE_ID_EDDP_CRT_CONS        22
#define EDDP_MODULE_ID_EDDP_CRT_PROV        23
#define EDDP_MODULE_ID_EDDP_CRT_CONS_SB     24
#define EDDP_MODULE_ID_EDDP_CRT_INIT        25
#define EDDP_MODULE_ID_EDDP_CRT_UTIL        26
#define EDDP_MODULE_ID_EDDP_CRT_INFO        27
#define EDDP_MODULE_ID_EDDP_CRT_CHECK       28
#define EDDP_MODULE_ID_EDDP_CRT_CMD         29
#define EDDP_MODULE_ID_EDDP_CRT_CPM_ISR     30
#define EDDP_MODULE_ID_EDDP_CRT_PPM_ISR     31
#define EDDP_MODULE_ID_EDDP_CRT_PROV_ACW    32
#define EDDP_MODULE_ID_EDDP_CRT_CONS_ACW    33
#define EDDP_MODULE_ID_EDDP_CRT_CONS_DFP    34
#define EDDP_MODULE_ID_EDDP_CRT_PROV_DFP    35
#define EDDP_MODULE_ID_EDDP_CRT_SM          36

/* HAL: 41..60 */
#define EDDP_MODULE_ID_EDDP_HAL_RAM         41
#define EDDP_MODULE_ID_EDDP_HAL_ISR         42

/* NRT: 61..80 */
#define EDDP_MODULE_ID_EDDP_NRT             61
#define EDDP_MODULE_ID_EDDP_NRT_HWC         62
#define EDDP_MODULE_ID_EDDP_NRT_HWF         63
#define EDDP_MODULE_ID_EDDP_NRT_USR         64
#define EDDP_MODULE_ID_EDDP_NRT_SND         65
#define EDDP_MODULE_ID_EDDP_NRT_RCV         66
#define EDDP_MODULE_ID_EDDP_NRT_ISR         67
#define EDDP_MODULE_ID_EDDP_NRT_UTIL        68

/* SWI: 81..90 */
#define EDDP_MODULE_ID_EDDP_SWI             81
#define EDDP_MODULE_ID_EDDP_SWI_PM          82
#define EDDP_MODULE_ID_EDDP_SWI_FDB         83
#define EDDP_MODULE_ID_EDDP_SWI_FRAMECLASS  84
#define EDDP_MODULE_ID_EDDP_SWI_IRT         85

/* PRM: 91..100 */
#define EDDP_MODULE_ID_EDDP_PRM             91
#define EDDP_MODULE_ID_EDDP_PRM_PDIRDATA    92
#define EDDP_MODULE_ID_EDDP_PRM_DFP         93
#define EDDP_MODULE_ID_EDDP_PRM_SM          94
#define EDDP_MODULE_ID_EDDP_PRM_DIAG        95

/* CMD: 101..110 */
#define EDDP_MODULE_ID_EDDP_CMD             101

/* PHY: 111..120 */
#define EDDP_MODULE_ID_EDDP_PHY             111
#define EDDP_MODULE_ID_EDDP_PHY_BCM         112
#define EDDP_MODULE_ID_EDDP_PHY_NSC         113
#define EDDP_MODULE_ID_EDDP_PHY_NSC_OUT     114
#define EDDP_MODULE_ID_EDDP_PHY_NEC         115
#define EDDP_MODULE_ID_EDDP_PHY_NEC_OUT     116

/* K32: 121..122 */
#define EDDP_MODULE_ID_EDDP_K32             121
#define EDDP_MODULE_ID_EDDP_K32_FW          122

/* I2C: 131..131 */
#define EDDP_MODULE_ID_EDDP_I2C             131

/* UsrCmd: 140..150 */
#define EDDP_MODULE_ID_EDDP_USR_CMD_API    140
#define EDDP_MODULE_ID_EDDP_USR_CMD_INTERN 141

#define EDDP_MODULE_ID_EDDP_USR_SYSRED_API      151

/*---------------------------------------------------------------------------*/
/* MRPD + DFP in ring is not released in V14.                                */
/* There is no controller for tests.                                         */
/* Function mu�t be looked in PNStack.                                       */
/*---------------------------------------------------------------------------*/
#define EDDP_DONT_USE_MRPD_WITH_DFP

/* No of ARP filters. Setting is internal until all EDDs support the same amount again */
#define EDDP_MAX_ARP_FILTER                1 

/* ========================================================================= */
/*                                                                           */
/* Internal configs                                                          */
/*                                                                           */
/* ========================================================================= */
#define EDDP_LED_BLINK_ON_OFF_DURATION_IN_100ms     5
#define EDDP_APPLY_IRTPU_RESTRICTIONS // Restrictions to IRT-PU until further notice: 1) SCF is limited to 125us, 187.5us and every value above or equal to 250us in TIAv14
                                      //                                              2) For SCF<8 RR is limited to 1
//ResourceType_VLAN is not supported by EDDP. For needed changes, search for "ResourceType_VLANSupport"

//#define EDDP_CFG_DUMP_NEC_REGS_ON_LINKDOWN //if defined, NEC-PHY regs and several MAC statistic counters get traced on a linkdown event.

/* ========================================================================= */
/* pragma message for EDDP                                                   */ 
/* ========================================================================= */

/*---------------------------------------------------------------------------*/
/* stringify                                                                 */
/*---------------------------------------------------------------------------*/

#define EDDP_STRINGIFY_(n)  # n
#define EDDP_STRINGIFY(n)   EDDP_STRINGIFY_(n) /* make it a string */

/*---------------------------------------------------------------------------*/
/* pragma message                                                            */
/* intended usage: #pragma EDDP_PRAGMA_MESSAGE(...)                          */ 
/*---------------------------------------------------------------------------*/

#ifdef _MSC_VER /* Microsoft, pragma-args are expanded */
#define EDDP_PRAGMA_MESSAGE(str) message( __FILE__ "(" EDDP_STRINGIFY(__LINE__) ") : " str)
#else
#undef EDDP_PRAGMA_MESSAGE
#endif 

/*===========================================================================*/
/*                                  external                                 */
/*===========================================================================*/

LSA_EXTERN  EDDP_LOCAL_INFO_PTR_TYPE        pEDDPInfo;          /* main EDDP management */

/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/

/* eddp_glb.c */
LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GlbFatalError(
    EDDP_FATAL_ERROR_TYPE  Error,
    LSA_UINT16             ModuleID,
    LSA_UINT32             Line);

LSA_EXTERN LSA_VOID  EDDP_LOCAL_FCT_ATTR  EDDP_GlbQueueInit(
    EDDP_QUEUE_PTR_TYPE     pQueue);
    
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GlbQueueAddToEnd(
    EDDP_QUEUE_PTR_TYPE             pQueue,
    EDDP_QUEUE_ELEMENT_PTR_TYPE     pNewElement);

LSA_EXTERN EDDP_QUEUE_ELEMENT_PTR_TYPE  EDDP_LOCAL_FCT_ATTR  EDDP_GlbQueueRemoveFromBegin(
    EDDP_QUEUE_PTR_TYPE    pQueue);

LSA_EXTERN LSA_BOOL  EDDP_LOCAL_FCT_ATTR  EDDP_GlbRQBQueueRemoveFromQueue(
    EDDP_RQB_QUEUE_PTR_TYPE    pQueue,
    EDD_UPPER_RQB_PTR_TYPE     pExtractRQB);


LSA_EXTERN LSA_VOID  EDDP_LOCAL_FCT_ATTR  EDDP_GlbRQBQueueInit( 
    EDDP_RQB_QUEUE_PTR_TYPE         pQueue);

LSA_EXTERN LSA_VOID  EDDP_LOCAL_FCT_ATTR  EDDP_GlbRQBQueueAddToEnd( 
    EDDP_RQB_QUEUE_PTR_TYPE            pQueue,
    EDD_UPPER_RQB_PTR_TYPE            pNewElement );
        
EDD_UPPER_RQB_PTR_TYPE  EDDP_LOCAL_FCT_ATTR  EDDP_GlbRQBQueueRemoveFromBegin(
    EDDP_RQB_QUEUE_PTR_TYPE    pQueue);

EDD_UPPER_RQB_PTR_TYPE  EDDP_LOCAL_FCT_ATTR  EDDP_GlbRQBQueueRemoveFromEnd(
    EDDP_RQB_QUEUE_PTR_TYPE    pQueue);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_GlbRQBQueueIsEmpty(
    EDDP_RQB_QUEUE_PTR_TYPE pQueue);

EDD_UPPER_RQB_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_GlbRQBPeekFirstElement(
    EDDP_RQB_QUEUE_PTR_TYPE pQueue);

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GlbRequestFinish(
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB,
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDD_RSP                     Response);

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GlbCallCbfLow(
    LSA_VOID LSA_FCT_PTR(EDD_UPPER_OUT_FCT_ATTR, Cbf)(EDD_UPPER_RQB_PTR_TYPE pRQB),
    EDD_UPPER_RQB_PTR_TYPE       pRQB,
    LSA_UINT32                   ModuleId,
    LSA_UINT32                   LineNr);

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GlbCallCbf(
    LSA_VOID LSA_FCT_PTR(EDD_UPPER_OUT_FCT_ATTR, Cbf)(EDD_UPPER_RQB_PTR_TYPE pRQB),
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GlbCallChannelCbfLow(
    LSA_VOID LSA_FCT_PTR(EDD_UPPER_OUT_FCT_ATTR, Cbf)(EDD_UPPER_RQB_PTR_TYPE pRQB),
    EDD_UPPER_RQB_PTR_TYPE       pRQB,
    LSA_SYS_PTR_TYPE             pSys,
    LSA_UINT32                   ModuleId,
    LSA_UINT32                   LineNr);

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GlbCallChannelCbf(
    LSA_VOID LSA_FCT_PTR(EDD_UPPER_OUT_FCT_ATTR, Cbf)(EDD_UPPER_RQB_PTR_TYPE pRQB),
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_SYS_PTR_TYPE            pSys);

LSA_EXTERN EDD_UPPER_RQB_PTR_TYPE EDDP_LOCAL_FCT_ATTR   EDDP_GlbAllocInternalRQB(
    LSA_VOID);

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GlbFreeInternalRQB(
    EDD_UPPER_RQB_PTR_TYPE pRQB);

LSA_EXTERN EDDP_LOCAL_MEM_PTR_TYPE EDDP_LOCAL_FCT_ATTR   EDDP_GlbAllocLocalMem(
    LSA_UINT32 Length);

LSA_EXTERN LSA_UINT16 EDDP_LOCAL_FCT_ATTR   EDDP_GlbFreeLocalMem(
    EDDP_LOCAL_MEM_PTR_TYPE  pMem);

LSA_EXTERN EDD_UPPER_MEM_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_GlbAllocUpperMemLocal(
    LSA_UINT16 Length);

LSA_EXTERN LSA_UINT16 EDDP_LOCAL_FCT_ATTR   EDDP_GlbFreeUpperMemLocal(
    EDD_UPPER_MEM_PTR_TYPE  pMem);

LSA_EXTERN EDD_UPPER_MEM_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_GlbAllocUpperRQBLocal(
    LSA_UINT16 Length);

LSA_EXTERN LSA_UINT16 EDDP_LOCAL_FCT_ATTR   EDDP_GlbFreeUpperRQBLocal(
    EDD_UPPER_RQB_PTR_TYPE  pMem);

LSA_EXTERN EDD_UPPER_RQB_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_GlbAllocInternalRQB(LSA_VOID);

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GlbFreeInternalRQB(
    EDD_UPPER_RQB_PTR_TYPE pRQB);

LSA_EXTERN EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GlbHandleGetHDB( 
    EDD_HANDLE_LOWER_TYPE                        Handle,
    EDDP_LOCAL_HDB_PTR_TYPE EDDP_LOCAL_MEM_ATTR *ppHDB);

LSA_EXTERN LSA_VOID  EDDP_LOCAL_FCT_ATTR EDDP_GlbSetupInternalRQB(
    EDD_UPPER_RQB_PTR_TYPE          pRQB,
    EDD_SERVICE                     Service,
    LSA_HANDLE_TYPE                 Handle,
    EDDP_HANDLE                     hDDB,
    LSA_UINT32                      ID,
    LSA_UINT32                      Param) ;

LSA_EXTERN EDD_UPPER_MEM_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_GlbAllocUpperRxMem(
    EDDP_LOCAL_DDB_PTR_TYPE   pDDB,
    LSA_UINT32                Length);

LSA_EXTERN LSA_UINT16 EDDP_LOCAL_FCT_ATTR   EDDP_GlbFreeUpperRxMem(
    EDDP_LOCAL_DDB_PTR_TYPE   pDDB,
    EDD_UPPER_MEM_PTR_TYPE    pMem);

LSA_EXTERN EDD_UPPER_MEM_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_GlbAllocUpperTxMem(
    EDDP_LOCAL_DDB_PTR_TYPE   pDDB,
    LSA_UINT32                Length);

LSA_EXTERN LSA_UINT16 EDDP_LOCAL_FCT_ATTR   EDDP_GlbFreeUpperTxMem(
    EDDP_LOCAL_DDB_PTR_TYPE   pDDB,
    EDD_UPPER_MEM_PTR_TYPE    pMem);

LSA_EXTERN EDD_UPPER_MEM_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_GlbAllocUpperDevMem(
    EDDP_LOCAL_DDB_PTR_TYPE   pDDB,
    LSA_UINT32                Length);

LSA_EXTERN LSA_UINT16 EDDP_LOCAL_FCT_ATTR   EDDP_GlbFreeUpperDevMem(
    EDDP_LOCAL_DDB_PTR_TYPE   pDDB,
    EDD_UPPER_MEM_PTR_TYPE    pMem);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_GlbIsAligned(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  Value,
    LSA_UINT16                  Alignment);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GlbCrtRQBHandler(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB);

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_GlbRequestFromGlbCrtRQBHandler(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GlbTriggerFinishEventAfterRQBConfirmation(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB,
    EDD_SERVICE                 Service);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GlbTraceByteArray(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_UPPER_MEM_U8_PTR_TYPE   pBuffer,
    LSA_UINT16                  Length);

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_GlbCrtRQBHandlerCheckAbortConditions(
    EDDP_LOCAL_DDB_PTR_TYPE           pDDB,
    EDDP_GLB_CRT_RQB_HANDLER_PTR_TYPE pRQBHandler);

/* eddp_dev.c */

LSA_EXTERN LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CoreDevHandleUsesDDB(
    EDDP_LOCAL_DDB_PTR_TYPE pDDB);

LSA_EXTERN EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreDevHandleNew(
    LSA_HANDLE_TYPE          EDDP_LOCAL_MEM_ATTR         *pHandle,
    EDDP_LOCAL_HDB_PTR_TYPE  EDDP_LOCAL_MEM_ATTR         *ppHDB);

LSA_EXTERN EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreDevHandleGetHDB( 
    LSA_HANDLE_TYPE                             Handle,
    EDDP_LOCAL_HDB_PTR_TYPE EDDP_LOCAL_MEM_ATTR *ppHDB);

LSA_EXTERN EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreDevHandleRel(
    LSA_HANDLE_TYPE Handle);

LSA_EXTERN EDD_RSP  EDDP_LOCAL_FCT_ATTR  EDDP_CoreDevGetDDB(
    EDDP_HANDLE                                  hDDB, 
    EDDP_LOCAL_DDB_PTR_TYPE EDDP_LOCAL_MEM_ATTR * ppDDB);

LSA_EXTERN LSA_VOID  EDDP_LOCAL_FCT_ATTR EDDP_CoreDevOpenRequest(
    EDD_UPPER_RQB_PTR_TYPE pRQB);

LSA_EXTERN LSA_VOID  EDDP_LOCAL_FCT_ATTR EDDP_CoreDevCloseRequest(
    EDD_UPPER_RQB_PTR_TYPE pRQB);

LSA_EXTERN LSA_VOID  EDDP_LOCAL_FCT_ATTR EDDP_CoreDevSetupRequest(
    EDD_UPPER_RQB_PTR_TYPE pRQB);

LSA_EXTERN LSA_VOID  EDDP_LOCAL_FCT_ATTR EDDP_CoreDevShutdownRequest(
    EDD_UPPER_RQB_PTR_TYPE pRQB);

LSA_EXTERN EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreDeviceOpen(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    EDDP_UPPER_DPB_PTR_TYPE            pDPB);

LSA_EXTERN EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreDeviceClose(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB);

/* eddp_sm.c */

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreDevIFShutdownStateMachine(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_RSP                     Response);

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreDevIFSetupStateMachine(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_RSP                     Response);

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreDevSetupStateMachine(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_RSP                     Response);

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreDevShutdownStateMachine(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_RSP                     Response);

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreCBFDeviceSetup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_RSP                     Response);

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreCBFDeviceShutdown(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_RSP                     Response);

/* eddp_dbg */
#ifdef EDDP_CFG_DETAIL_ERROR_INFO
LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_DbgSetDetailError(
    EDDP_LOCAL_DDB_PTR_TYPE   pDDB,
    EDD_RSP                   Response,    
    LSA_UINT32                ModuleId,
    LSA_UINT32                LineNr );
#endif

#ifdef EDDP_CFG_DETAIL_ERROR_INFO
LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_DbgSetDetailErrorLocal(
    EDDP_DBG_DETAIL_ERR_PTR_TYPE   pErrDetail,
    EDD_RSP                        Response,    
    LSA_UINT32                     ModuleId,
    LSA_UINT32                     LineNr);
#endif


/* eddp_tim.c */

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreTimerMgmSetup(LSA_VOID);

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreTimerMgmFree(LSA_VOID);

LSA_EXTERN EDD_RSP  EDDP_LOCAL_FCT_ATTR EDDP_CoreTimerAlloc(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_UINT16                      TimerType,
    LSA_UINT16                      TimerBase,
    LSA_UINT32                      UserID,
    LSA_VOID                        LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, Cbf) 
                                               (EDDP_INTERNAL_REQUEST_PTR_TYPE   pIntRQB),
    EDDP_LOCAL_MEM_U32_PTR_TYPE     pEDDPTimerID);

LSA_EXTERN EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreTimerFree(
    LSA_UINT32      EDDPTimerID);

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreTimerStart(
    LSA_UINT32      EDDPTimerID,
    LSA_UINT16      Time);

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreTimerStop(
    LSA_UINT32      EDDPTimerID);

LSA_EXTERN LSA_BOOL  EDDP_LOCAL_FCT_ATTR EDDP_CoreTimerRQBFree(
    LSA_UINT32 EDDPTimerID);

LSA_EXTERN LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CoreTimerRQBCheckFree(
    LSA_UINT32      EDDPTimerID);

LSA_EXTERN LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreTimerRequest(
    EDD_UPPER_RQB_PTR_TYPE pRQB);

LSA_EXTERN EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreTimerDeviceSetup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DSB_PTR_TYPE     pDSB);

LSA_EXTERN EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreTimerDeviceShutdown(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB);

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_SWI_GEN_PhySTS_StateMachine(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_PHYSTS_SM_TRIGGER_TYPE Trigger,
    LSA_UINT32                  HwPortIndex);

/*****************************************************************************/
/*  end of file EDDP_INT.H                                                   */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_INT_H */


