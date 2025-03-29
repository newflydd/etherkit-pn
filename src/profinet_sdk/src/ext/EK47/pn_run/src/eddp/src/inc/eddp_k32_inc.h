
#ifndef EDDP_K32_INC_H                   /* ----- reinclude-protection ----- */
#define EDDP_K32_INC_H

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
/*  F i l e               &F: eddp_k32_inc.h                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Internal K32 headerfile                                                  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  2008-12-22  lrg   initial version.                                       */
/*  2009-02-16  lrg   EDDP_K32RemotePortState(), EDDP_K32DelaySet()          */
/*  2009-11-03  lrg   Flusskontrolle fuer EDD_SRV_PTCP_SYNC/DELAY_CONTROL    */
/*                    DelayReqQueue, SyncReqQueue                            */
/*  2010-10-11  111010lrg001: EDDP_CFG_LOCAL_MEM_STATIC, g_EDDP_HDB_K32      */
/*  2016-03-02  hm    TimestampInfo added to DDB_K32                         */
#endif
/*****************************************************************************/

#include "eddp_k32_cfg.h"                /* Krisc32 RQB structures */
#include "eddp_k32_fw.h"

/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/

/*===========================================================================*/
/* K32 handle (channel) management-structures                                */
/*===========================================================================*/


/*---------------------------------------------------------------------------*/
/* Managment information for device                                          */
/* in eddp_dev.h  EDDP_DDB_TYPE.EDDP_DDB_K32_TYPE (K32)                      */
/*---------------------------------------------------------------------------*/

typedef struct _EDDP_DDB_K32_MEM_BLOCK_TYPE
{
    EDD_COMMON_MEM_PTR_TYPE         pMemBase;
    LSA_UINT32                      MemSize;
} EDDP_DDB_K32_MEM_BLOCK_TYPE;

typedef struct _EDDP_DDB_K32_TYPE       EDDP_LOCAL_MEM_ATTR *    EDDP_LOCAL_DDB_K32_PTR_TYPE;
typedef struct _EDDP_DDB_K32_TYPE
{
    EDD_COMMON_MEM_PTR_TYPE         pK32TCMBase;
    EDD_UPPER_KRISC32_RQB_PTR_TYPE  pK32StartRQB;                   // Pointer to K32 Start RQB
    LSA_UINT32                      tcmSize;
    LSA_UINT32                      tileSize;
    EDDP_DDB_K32_MEM_BLOCK_TYPE     K32ATCMBlock;
    EDDP_DDB_K32_MEM_BLOCK_TYPE     K32BTCMBlock;
    EDDP_DDB_K32_MEM_BLOCK_TYPE     K32DDR3Block;
    LSA_UINT16                      RRQBTraceLevel;
    EDDP_TIMESTAMP_INFO_TYPE        TimestampInfo;                      // FT1204141: for time sync indication

    LSA_UINT32                      HandleCnt;
    EDDP_LOCAL_HDB_PTR_TYPE         pHDB;                               // Pointer to GSY sync channel
    LSA_BOOL                        TopoStateOk [EDDP_MAX_PORT_CNT];    // Last Topo state
    EDD_UPPER_KRISC32_RQB_PTR_TYPE  pK32PortRQB [EDDP_MAX_PORT_CNT];    // Pointer to K32 Port RQBs
    EDD_UPPER_KRISC32_RQB_PTR_TYPE  pK32DelayRQB[EDDP_MAX_PORT_CNT];    // Pointer to K32 Delay control RQBs
    EDD_UPPER_KRISC32_RQB_PTR_TYPE  pK32ClockRQB;                       // Pointer to K32 Clock control RQB
    EDD_UPPER_KRISC32_RQB_PTR_TYPE  pK32TimeRQB;                        // Pointer to K32 Time Control RQB
    EDD_UPPER_KRISC32_RQB_PTR_TYPE  pK32CtrlRQB;                        // Pointer to K32 Control RQB
} EDDP_DDB_K32_TYPE;


/*---------------------------------------------------------------------------*/
/* Managment information for channel                                         */
/* in eddp_dev.h  EDDP_HDB_TYPE.EDDP_LOCAL_HDB_K32_PTR_TYPE (pK32)           */
/*---------------------------------------------------------------------------*/

typedef struct _EDDP_HDB_K32_TYPE       EDDP_LOCAL_MEM_ATTR *    EDDP_LOCAL_HDB_K32_PTR_TYPE;
typedef struct _EDDP_HDB_K32_TYPE
{
    EDDP_RQB_QUEUE_TYPE         DelayReqQueue[EDDP_MAX_PORT_CNT];   // Request queue for delay RQBs
    EDDP_RQB_QUEUE_TYPE         SyncReqQueue[2];                    // Request queue for sync RQBs
    EDDP_RQB_QUEUE_TYPE         CmdReqQueue;                        // Request queue for all Command RQBs
    EDDP_RQB_QUEUE_TYPE         DiagReqQueue;                       // Request queue for Diag RQBs
    EDDP_LOCAL_DDB_K32_PTR_TYPE pK32DDB;                            // common K32 mgm
    EDD_HANDLE_LOWER_TYPE       EDD_Handle;                         // 
} EDDP_HDB_K32_TYPE;

#ifdef EDDP_CFG_LOCAL_MEM_STATIC            //111010lrg001
LSA_EXTERN    EDDP_LOCAL_MEM_ATTR EDDP_HDB_K32_TYPE g_EDDP_HDB_K32[EDDP_CFG_MAX_CHANNELS];
#endif

/*===========================================================================*/
/*                                  data                                     */
/*===========================================================================*/


/*===========================================================================*/
/*                                 macros                                    */
/*===========================================================================*/


/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32DeviceOpen(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    EDDP_UPPER_DPB_PTR_TYPE            pDPB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32DeviceSetup(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    EDDP_UPPER_DSB_PTR_TYPE            pDSB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32DeviceIFSetup(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    EDDP_UPPER_DSB_PTR_TYPE            pDSB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32DeviceIFShutdown(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32DeviceShutdown(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32DeviceClose(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32OpenChannel(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE            pHDB,
    LSA_HANDLE_TYPE                    Handle,
    EDDP_SYSTEM_CDB_PTR_TYPE           pCDB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_K32Request(
    EDD_UPPER_RQB_PTR_TYPE             pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE            pHDB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32RemotePortState(
    EDD_UPPER_RQB_PTR_TYPE             pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE            pHDB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32CloseChannel(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE            pHDB,
    LSA_HANDLE_TYPE                    Handle);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_K32GetRQB(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    K32_RQB_PTR_TYPE                   pK32RQB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_K32AckRQB(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    K32_RQB_PTR_TYPE                   pK32RQB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32PutRQB(
    EDD_UPPER_KRISC32_RQB_PTR_TYPE     pK32RQB,
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB
    );

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_K32TopoStateSet (
    LSA_UINT16                         PortId,
    LSA_UINT32                         TopoState,
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_K32DelaySet (
    LSA_UINT16                         PortId,
    LSA_UINT32                         LineDelay,
    LSA_UINT32                         CableDelay,
    LSA_UINT16                         Response,
    EDDP_LOCAL_HDB_PTR_TYPE            pHDB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_UPPER_KRISC32_RQB_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_K32RQBAlloc (
    LSA_VOID);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32RQBFree (
    EDD_UPPER_KRISC32_RQB_PTR_TYPE     pRQB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32InterfaceInit(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_K32InterfaceDeInit(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_K32CopyUpperRQB(
    EDDP_LOCAL_DDB_PTR_TYPE pDDB,
    EDD_UPPER_KRISC32_RQB_PTR_TYPE  pK32RBu,
    volatile K32_RQB_TYPE*          pK32RBl);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_K32_RRQB_SEND(
    EDDP_LOCAL_DDB_PTR_TYPE pDDB,
    K32_RQB_PTR_TYPE pRRQB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_K32_RRQB_GET(
    EDDP_LOCAL_DDB_PTR_TYPE pDDB,
    K32_RQB_PTR_TYPE* ppRRQB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_K32_RRQB_ACK(
    EDDP_LOCAL_DDB_PTR_TYPE pDDB,
    K32_RQB_PTR_TYPE pRRQB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_K32_RRQB_GET_ACK(
    EDDP_LOCAL_DDB_PTR_TYPE pDDB,
    K32_RQB_PTR_TYPE* ppRRQB);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_K32_RRQB_FINALIZE(
    EDDP_LOCAL_DDB_PTR_TYPE pDDB,
    K32_RQB_PTR_TYPE pRRQB);

/*=============================================================================
 * function name:  EDDP_K32_ISR_SW_EVENT0
 *
 * function:       Interrupt callback funktion.
 *
 * parameters:     -
 *
 * return value:   none
 *
 * NOTE: this funktion is listed in EDDP_IsrFctTable[]
 *       and should not called directly.
 *===========================================================================*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_K32_ISR_SW_EVENT0(
    EDDP_LOCAL_DDB_PTR_TYPE  pDDB,
    LSA_UINT32               EventNr );

/*=============================================================================
 * function name:  EDDP_K32_ISR_WD
 *
 * function:       Interrupt callback funktion.
 *
 * parameters:     -
 *
 * return value:   none
 *
 * NOTE: this funktion is listed in EDDP_IsrFctTable[]
 *       and should not called directly.
 *===========================================================================*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_K32_ISR_WD(
    EDDP_LOCAL_DDB_PTR_TYPE  pDDB,
    LSA_UINT32               EventNr );

/*****************************************************************************/
/*  end of file EDDP_K32_INC.H                                               */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_K32_INC_H */

