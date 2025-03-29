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
/*  F i l e               &F: eddp_crt_cons_dfp.c                       :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  CRT (Cycle Real Time) for EDDP  -> Consumer DFP                          */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who         What                                             */
#ifdef EDDP_MESSAGE
/*  17.11.10    Sergeev     Erstellung                                       */
#endif
/*****************************************************************************/


/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/

#define LTRC_ACT_MODUL_ID   34
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_CRT_CONS_DFP */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"           /* eddp headerfiles */
#include "eddp_int.h"           /* internal header  */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)

#define EDDP_PACKRAM1_ASWD_SIZE     64                                  // 64 Byte WD/AS fields
#define EDDP_PACKRAM1_OFFS_SIZE     32                                  // offset fields 48..63
#define EDDP_PACKRAM_GRPCTRL_SIZE   0x60                                // 96 Byte ctrl data
#define EDDP_PACKRAM_GRPDATA_SIZE   1440                                // 1440 Byte for DFP frame from neighbour
#define EDDP_PACKRAM_GRPBLK_SIZE    (EDDP_PACKRAM_GRPCTRL_SIZE + EDDP_PACKRAM_GRPDATA_SIZE) // 96+1440
//#define EDDP_PACKRAM_X_SIZE       (PNIP_RAM_PACKDATA_RAM_LEN>>1)   // divided into RAM0 + RAM1
#define EDDP_PACKRAM_X_SIZE         (EDDP_PACKRAM_GRPBLK_SIZE*EDDP_CRT_MAX_PACK_FRAMES_ERTEC200P)   // divided into RAM0 + RAM1


// ***** values for PNIP_REG_INBOUNDPACK_CONTROL_0_x.INBOUND_RXPORT_DIS
#define EDDP_INBOUND_RX1_ALLOWED    0x0E
#define EDDP_INBOUND_RX2_ALLOWED    0x0D
#define EDDP_INBOUND_RX3_ALLOWED    0x0B
#define EDDP_INBOUND_RX4_ALLOWED    0x07
#define EDDP_INBOUND_RX_DISABLED    0x0F


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    _EDDP_CRTPackRamResetDGs                               +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE                   pDDB         +*/
/*+                      LSA_UINT8                                 PackGrpIdx   +*/
/*+                                                                             +*/
/*+  Result         :    EDD_RSP                                                +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  PackGrpIdx     : specify the pack group                                    +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: internal function, set all Offset_DG to zero (invalid).       +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_CRTPackRamResetDGs(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT8                   PackGrpIdx)
{
    LSA_UINT32  nPackRamIntOffs;

    EDDP_ASSERT_FALSE(PackGrpIdx >= EDDP_CRT_MAX_PACK_FRAMES_ERTEC200P);

    // ***** RAM_0 --> clear all Offset_DG[0..47]
    nPackRamIntOffs = (PackGrpIdx*EDDP_PACKRAM_GRPBLK_SIZE);
    EDDP_HALMemset(pDDB, EDDP_HAL_RAMTYPE_PACK_DATA, 0, nPackRamIntOffs, EDDP_PACKRAM_GRPCTRL_SIZE, 0);
    // ***** RAM_1 --> clear (all WD/AS fields and) all Offset_DG[48..63] 
    nPackRamIntOffs += EDDP_PACKRAM_X_SIZE;
    EDDP_HALMemset(pDDB, EDDP_HAL_RAMTYPE_PACK_DATA, 0, nPackRamIntOffs, EDDP_PACKRAM_GRPCTRL_SIZE, 0);

    return (EDD_STS_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    _EDDP_CRTPackRamSetDgPos                               +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE                   pDDB         +*/
/*+                      LSA_UINT8                                 PackGrpIdx   +*/
/*+                      LSA_UINT16                                Offset_DG    +*/
/*+                      LSA_UINT8                                 PosT         +*/
/*+                      LSA_UINT8                                 Len          +*/
/*+                      LSA_BOOL                                  UpdateOffs   +*/
/*+                                                                             +*/
/*+  Result         :    EDD_RSP                                                +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  PackGrpIdx     : specify the pack group                                    +*/
/*+  Offset_DG      : first call:    Offset_DG = 0                              +*/
/*+                   next call:     Offset_DG = return value from last call    +*/
/*+  PosT           : value for PosT within RAM0 (0 for Terminator)             +*/
/*+  Len            : value for Len within RAM0  (PosT|Len=0 --> Terminator)    +*/
/*+  UpdateOffs     : TRUE: first (DG) entry, FALSE: further (DG) entries       +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: internal function,                                            +*/
/*+               initialize PackRAM with given entry/terminator.               +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR _EDDP_CRTPackRamSetDgPos(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT8                   PackGrpIdx,
    LSA_UINT16                  Offset_DG,
    LSA_UINT8                   PosT,
    LSA_UINT8                   Len,
    LSA_BOOL                    UpdateOffs)
{
    EDD_COMMON_MEM_U8_PTR_TYPE  pPackRamDgGrp;
    EDD_COMMON_MEM_U8_PTR_TYPE  pPackRamPtr;
    LSA_UINT8                   RdPosT, RdLen;

    EDDP_ASSERT_FALSE(PackGrpIdx >= EDDP_CRT_MAX_PACK_FRAMES_ERTEC200P);
    EDDP_ASSERT_FALSE(PosT >= 64);

    // ***** calculate the right 1,5KB field within RAM0
    pPackRamDgGrp = (EDD_COMMON_MEM_U8_PTR_TYPE) EDDP_HALGetRAMBaseAddress (pDDB, EDDP_HAL_RAMTYPE_PACK_DATA, LSA_NULL);
    pPackRamDgGrp += (PackGrpIdx*EDDP_PACKRAM_GRPBLK_SIZE);

    // ***** calculate the Offset_DG beginning from 0x60 within RAM0
    if (Offset_DG == 0)
    {   // ***** set first DG frame position
        Offset_DG = EDDP_PACKRAM_GRPCTRL_SIZE;
    }
    else
    {   // ***** calculate next DG frame position
        pPackRamPtr = (pPackRamDgGrp + Offset_DG);
        RdPosT      = EDDP_HAL_MEM8_READ(pDDB, (pPackRamPtr+0));  // RAM0.PosT
        RdLen       = EDDP_HAL_MEM8_READ(pDDB, (pPackRamPtr+1));  // RAM0.Len
        EDDP_ASSERT_FALSE(!(RdPosT | RdLen)); // expect here values different from terminator !!!
        Offset_DG += (6+RdLen);         // add frame fields: PosT, Len, CC, DS, Data[RdLen], DCS[2]
    }

    // ***** write DG-entry data (PosT, Len, ...)
    pPackRamPtr = (pPackRamDgGrp + Offset_DG);
    EDDP_HAL_MEM8_WRITE(pDDB, pPackRamPtr, PosT);                     // RAM0.PosT
    EDDP_HAL_MEM8_WRITE(pDDB, pPackRamPtr+EDDP_PACKRAM_X_SIZE, 0x00); // RAM1.MetaPosT
    pPackRamPtr++;
    EDDP_HAL_MEM8_WRITE(pDDB, pPackRamPtr, Len);                      // RAM0.Len
    EDDP_HAL_MEM8_WRITE(pDDB, pPackRamPtr+EDDP_PACKRAM_X_SIZE, 0x00); // RAM1.MetaLen

    if ((PosT) || (Len))
    {   // ***** is no terminator --> set fields: CC=0 and DS=Surrogat
        pPackRamPtr++;
        EDDP_HAL_MEM8_WRITE(pDDB, pPackRamPtr, 0x00);                 // RAM0.CC
        pPackRamPtr++;
        EDDP_HAL_MEM8_WRITE(pDDB, pPackRamPtr, 0x80);                 // RAM0.DS = Surrogat
    }

    if (UpdateOffs)
    {   // ***** write offset to subframe into "Offset_DG Pos?"
        pPackRamPtr = (pPackRamDgGrp + (PosT<<1)); // every entry "Offset_DG Pos?" is 2 byte
        if (PosT > 47)
        {   // ***** special case: Pos 48..63 stored in RAM1 behinde "WD/AS Pos 63"
            pPackRamPtr += (EDDP_PACKRAM_X_SIZE - ((64-48)<<1) );
        }
        EDDP_HAL_MEM16_WRITE(pDDB, pPackRamPtr, Offset_DG);
    }

    return (Offset_DG);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    _EDDP_CRTCmdSmCbfCpm                                   +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE                   pDDB         +*/
/*+                      EDDP_CMD_PARAM_PTR_TYPE                   pCmdParam    +*/
/*+                      EDD_RSP                                   ResponseCmd  +*/
/*+                                                                             +*/
/*+  Result         :    LSA_VOID                                               +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  pCmdParam      : Pointer to command struct (used cmd parameter)            +*/
/*+  ResponseCmd    : result of cmd execution                                   +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: Internal function, CBF for asynchronous cmd calls (Pack SM).  +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR _EDDP_CRTCmdSmCbfCpm(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_CMD_PARAM_PTR_TYPE     pCmdParam,
    EDD_RSP                     ResponseCmd)
{
    EDD_RSP                     Response = EDD_STS_OK;
    EDDP_CRT_PACK_SM_PTR_TYPE   SmPtr = &pDDB->CRT.Cons.CrtPackSm;

    LSA_UNUSED_ARG(pCmdParam);

    EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] _EDDP_CRTCmdSmCbfCpm(): SmPtr=0x%X, ExecuteOff=%d, ResponseCmd=0x%X, CurrIdx=%d", 
        SmPtr->Smf, SmPtr->ExecuteOff, ResponseCmd, SmPtr->CurrIdx);

    EDDP_ASSERT_FALSE(ResponseCmd != EDD_STS_OK);
    EDDP_ASSERT_NO_NULL_FCT_PTR(SmPtr->Smf);

    Response = SmPtr->Smf(pDDB, SmPtr->ExecuteOff, LSA_NULL);   // call here with CBF=NULL to go on within the SM
    EDDP_ASSERT_FALSE((Response != EDD_STS_OK) && (Response != EDD_STS_OK_PENDING));
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    _EDDP_CRTInboundGetRxPort                              +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_PRM_PDIRDATA_PTR_TYPE             pPDIRData       +*/
/*+                      EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_PTR_TYPE       +*/
/*+                                                             pSubFrameBlock  +*/
/*+                                                                             +*/
/*+  Result         :    LSA_UINT16                                             +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pPDIRData      : Pointer to PDIRDATA                                       +*/
/*+  pSubFrameBlock : pointer to selected SUBFRAMEBLOCK (from array)            +*/
/*+  bRedundantIdx  : true for MRPD                                             +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: internal function, return right RxPort                        +*/
/*+               if Cons. exist --> return this RxPort, else from the Prov.    +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 _EDDP_CRTInboundGetRxPort(
    EDDP_PRM_PDIRDATA_PTR_TYPE                          pPDIRData, 
    EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_PTR_TYPE    pSubFrameBlock,
    LSA_BOOL                                            bRedundantIdx)
{
    LSA_UINT16      ArrayIdx;

    if (bRedundantIdx)
    {
        ArrayIdx = pSubFrameBlock->PDIRFrameDataConsIdxRed1;
    }
    else
    {
        ArrayIdx = pSubFrameBlock->PDIRFrameDataConsIdx;
    }

    if (ArrayIdx != EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
    {   // ***** if consumer is configured --> use this RxPort at first
        return (pPDIRData->pIRFrameDataArray[ArrayIdx].RxPort);
    }

    return (0);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname       :   EDDP_CRTInitInboundDevOpen                                      +*/
/*+                                                                                         +*/
/*+  Input/Output       :   EDDP_LOCAL_DDB_PTR_TYPE         pDDB                            +*/
/*+                     :   EDDP_UPPER_DPB_PTR_TYPE         pDPB                            +*/
/*+                                                                                         +*/
/*+  Result             :   EDD_RSP                                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  pDDB       : Pointer to Device Description Block                                       +*/
/*+  pDPB       : Pointer to Device Parameter Block                                         +*/
/*+                                                                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: This function initializes internal SW structures at time of DEV_OPEN      +*/
/*+                 -> init struct EDDP_CRT_INBOUND_MNGM_TYPE                               +*/
/*+                 -> EDDP-CORE has inited the DDB already with 0                          +*/
/*+                 -> write only values != 0                                               +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTInitInboundDevOpen(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_UPPER_DPB_PTR_TYPE     pDPB)
{
    LSA_UINT32                              i;
    EDD_COMMON_MEM_U8_PTR_TYPE              pPackRam1Start;
    EDDP_CRT_INBOUND_MNGM_PTR_TYPE      pInboundMgmt;

    LSA_UNUSED_ARG(pDPB);

    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTInitInboundDevOpen()");

    pInboundMgmt    = &pDDB->CRT.Inbound;
    pPackRam1Start  = (EDD_COMMON_MEM_U8_PTR_TYPE) EDDP_HALGetRAMBaseAddress (pDDB, EDDP_HAL_RAMTYPE_PACK_DATA, LSA_NULL);
    pPackRam1Start += EDDP_PACKRAM_X_SIZE;  // add offset to RAM1 within PackRAM space

    /* ---------------------------------------------------------------- */
    /* InBound pack groups                                              */
    /* ---------------------------------------------------------------- */
    for ( i=0; i < EDDP_CRT_MAX_INBOUND_GRP_ERTEC200P; i++ )
    {
        // init some members, remaining parts within PrmCommit
        pInboundMgmt->InboundGrps[i].pPackRam1Start = pPackRam1Start;
        pInboundMgmt->InboundGrps[i].ReInitAsValue  = EDDP_CRT_INBOUND_REINITASVALUE_INIT;
        pInboundMgmt->InboundGrps[i].PackGrpId      = (LSA_UINT8) i;

        // init LateErrDiag data
        pInboundMgmt->InboundGrps[i].LateErrDiag.TimerType              = EDDP_CRT_INBOUND_LATE_ERR_TIMER_TYPE_INVALID;
        pInboundMgmt->InboundGrps[i].LateErrDiag.PrmPDIRFrameDataIdx    = EDDP_CRT_CHAIN_IDX_END;

        pPackRam1Start += EDDP_PACKRAM_GRPBLK_SIZE;
    }

    pInboundMgmt->isLateErrLSATimerRunning = LSA_FALSE;

    #ifdef EDDP_MESSAGE
    #ifdef EDDP_PRAGMA_MESSAGE
    #pragma EDDP_PRAGMA_MESSAGE("Workaround CRT.WrkarPackGrpDisable for PNIP-Revision(1) - AP01372025")
    #endif /* EDDP_PRAGMA_MESSAGE */
    #endif /* EDDP_MESSAGE */
    #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
    if ( EDDP_HAL_HW_PNIP_REV1 == EDDP_HALGetPnipRevision(pDDB) )
    {
        LSA_UINT32      iTimerUser;

        pDDB->CRT.WrkarPackGrpDisable.isLSATimerRunning = LSA_FALSE;

        for ( iTimerUser=0; iTimerUser < (EDDP_CRT_WRKAR_PACKGRP_DISABLE_TIMER_USER_MAX_NUMBER); iTimerUser++ )
        {
            pDDB->CRT.WrkarPackGrpDisable.TimerUser[iTimerUser].isTimerUsed = LSA_FALSE;
        }
    }
    #endif

    return (EDD_STS_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname       :   EDDP_CRTInitInboundDevSetup                                     +*/
/*+                                                                                         +*/
/*+  Input/Output       :   EDDP_LOCAL_DDB_PTR_TYPE         pDDB                            +*/
/*+                     :   EDDP_UPPER_DSB_PTR_TYPE         pDSB                            +*/
/*+                                                                                         +*/
/*+  Result             :   EDD_RSP                                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  pDDB       : Pointer to Device Description Block                                       +*/
/*+  pDSB       : Pointer to Device Setup Block                                             +*/
/*+                                                                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: This function is called at time of DEV_SETUP                              +*/
/*+                 -> write defaults into HW RAM                                           +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTInitInboundDevSetup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_UPPER_DSB_PTR_TYPE     pDSB)
{
    LSA_UINT32      PackGrpIdx;

    LSA_UNUSED_ARG(pDSB);

    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTInitInboundDevSetup(reset parts of PackRAM)");

    for ( PackGrpIdx=0; PackGrpIdx < EDDP_CRT_MAX_INBOUND_GRP_ERTEC200P; PackGrpIdx++ )
    {
        _EDDP_CRTPackRamResetDGs ( pDDB, (LSA_UINT8)PackGrpIdx );
    }

    return (EDD_STS_OK);
}



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname       :   EDDP_CRTSetRxTxDP2BoundaryAndInbound                            +*/
/*+                                                                                         +*/
/*+  Input/Output       :   EDDP_LOCAL_DDB_PTR_TYPE         pDDB                            +*/
/*+                     :	LSA_UINT32                      HwPortID                        +*/
/*+                     :	LSA_BOOL                        isSetBoundary                   +*/
/*+                                                                                         +*/
/*+  Result             :   LSA_VOID                                                        +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  pDDB         : Pointer to Device Description Block                               (in)  +*/
/*+  HwPortID     : Hardware port ID                                                  (in)  +*/
/*+  isSetBoundary  : LSA_TRUE or LSA_FALSE                                           (in)  +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: This function is called to set/clr the DP2 boundary and inbound if used.  +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define EDDP_CRT_INBOUNDPACK_X_VALID_AND_INUSE (PNIP_REG_INBOUNDPACK_0__MSK_INBOUND_VALID | PNIP_REG_INBOUNDPACK_0__MSK_INBOUND_INUSE)

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTSetRxTxDP2BoundaryAndInbound(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32				    HwPortID,
    LSA_BOOL                    isSetBoundary)
{
    LSA_UINT32                              RegAddrFlowControl, RegValueFlowControl;
    LSA_UINT32                              InboundCtrlRegAddr, InboundPackRegAddr;
    LSA_UINT32                              InboundRegVal, InUseChkTo;
    LSA_UINT16                              InBoundIdx;
    LSA_UINT8                               InboundUsedMask, InboundActiveMask, InboundInActiveMask;
    EDDP_CRT_INBOUND_MNGM_PTR_TYPE      pInboundMgmt;

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CRTSetRxTxDP2BoundaryAndInbound(HwPortID=%d, isSetBoundary=0x%X)", 
        HwPortID, isSetBoundary);

    RegAddrFlowControl = (HwPortID==1 ? PNIP_REG_FLOWCONTROL_P1 : 
                         (HwPortID==2 ? PNIP_REG_FLOWCONTROL_P2 : 
                         (HwPortID==3 ? PNIP_REG_FLOWCONTROL_P3 : 
                         (HwPortID==4 ? PNIP_REG_FLOWCONTROL_P4 : 0))));
    EDDP_ASSERT_FALSE(!RegAddrFlowControl);

    /* ---------------------------------------------------- */
    /* enable/disable DP2-Boundaries in FlowControl_Px      */
    /* ---------------------------------------------------- */
    RegValueFlowControl = EDDP_HAL_REG32_READ(pDDB, RegAddrFlowControl);
    pInboundMgmt        = &pDDB->CRT.Inbound;
    InboundUsedMask     = pInboundMgmt->InboundUsedMask[HwPortID];  // all used inbound groups for given HwPortID

    if (isSetBoundary)
    {
        /* ---------------------------------------- */
        /* disable full RED                         */
        /* ---------------------------------------- */
        // all enabled groups for given port
        InboundActiveMask = (InboundUsedMask & pInboundMgmt->InboundActiveMask);

        EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTSetRxTxDP2BoundaryAndInbound(HwPortID=%d): InboundUsedMask=0x%X, InboundActiveMask=0x%X",
            HwPortID, InboundUsedMask, InboundActiveMask);

        if (InboundActiveMask != 0)
        {   // ***** disable PackGrp
            InboundCtrlRegAddr = PNIP_REG_INBOUNDPACK_CONTROL_0_0;
            InboundPackRegAddr = PNIP_REG_INBOUNDPACK_0;

            for ( InBoundIdx=0; InBoundIdx < EDDP_CRT_MAX_PACK_FRAMES_ERTEC200P; InBoundIdx++ )
            {
                if (InboundActiveMask & ((LSA_UINT32)1<<InBoundIdx))
                {
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTSetRxTxDP2BoundaryAndInbound(): disable PackGrp(%d)", 
                        InBoundIdx);

                    // ***** disable RX path
                    InboundRegVal = EDDP_HAL_REG32_READ(pDDB, InboundCtrlRegAddr);
                    EDDP_SET_BIT_VALUE32(InboundRegVal, EDDP_INBOUND_RX_DISABLED, PNIP_REG_INBOUNDPACK_CONTROL_0_0__MSK_INBOUND_RXPORT_DIS, PNIP_REG_INBOUNDPACK_CONTROL_0_0_SHFT_INBOUND_RXPORT_DIS);
                    EDDP_HAL_REG32_WRITE(pDDB, InboundCtrlRegAddr, InboundRegVal);
                    // ***** disable PackGrp
                    InboundRegVal = EDDP_HAL_REG32_READ(pDDB, InboundPackRegAddr);
                    EDDP_CLR_BIT_VALUE32(InboundRegVal, PNIP_REG_INBOUNDPACK_0__MSK_INBOUND_VALID);
                    EDDP_HAL_REG32_WRITE(pDDB, InboundPackRegAddr, InboundRegVal);

                    // this PackGrp must be connected to PRM -> PDIRData.PDIRFrameData[x]
                    EDDP_ASSERT_FALSE(pInboundMgmt->InboundGrps[InBoundIdx].LateErrDiag.PrmPDIRFrameDataIdx == EDDP_CRT_CHAIN_IDX_END);
                    // delete this PackGrp in register "Late_Err_Status"
                    EDDP_CRTLateErrorDeleteLateErrStatus (pDDB, (LSA_UINT8)InBoundIdx);
                    // stop LateError timer of this PackGrp
                    EDDP_CRTLateErrorTimerStopPackGrp (pDDB, (LSA_UINT8)InBoundIdx);
                    // trigger LateError diagnosis (DisAppear)
                    EDDP_PRMDiagTriggerLateErrorDiagSingle (pDDB, pInboundMgmt->InboundGrps[InBoundIdx].LateErrDiag.PrmPDIRFrameDataIdx, LSA_FALSE, LSA_FALSE, LSA_FALSE);

                    #ifdef EDDP_MESSAGE
                    #ifdef EDDP_PRAGMA_MESSAGE
                    #pragma EDDP_PRAGMA_MESSAGE("Workaround CRT.WrkarPackGrpDisable for PNIP-Revision(1) - AP01372025")
                    #endif /* EDDP_PRAGMA_MESSAGE */
                    #endif /* EDDP_MESSAGE */
                    #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
                    if ( EDDP_HAL_HW_PNIP_REV1 == EDDP_HALGetPnipRevision(pDDB) )
                    {
                        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] EDDP_CRTSetRxTxDP2BoundaryAndInbound(HwPortID=%d): workaround PackGrp-Disable - PackGrp(%d) restore data", 
                            HwPortID, InBoundIdx);

                        // restore invalid "ACW.DRM/Destination" of this inbound group
                        EDDP_ASSERT_FALSE(pInboundMgmt->InboundGrps[InBoundIdx].WrkarPackGrpDisable.AcwId == EDDP_CRT_CHAIN_IDX_END);
                        EDDP_CRTProvAcwSetupDrmAndDestination (pDDB, pInboundMgmt->InboundGrps[InBoundIdx].WrkarPackGrpDisable.AcwId, LSA_TRUE);
                        // if redundant FrameID(x+1) is present
                        if (pInboundMgmt->InboundGrps[InBoundIdx].WrkarPackGrpDisable.AcwIdRed1 != EDDP_CRT_CHAIN_IDX_END)
                        {
                            EDDP_CRTProvAcwSetupDrmAndDestination (pDDB, pInboundMgmt->InboundGrps[InBoundIdx].WrkarPackGrpDisable.AcwIdRed1, LSA_TRUE);
                        }
                        // restore invalid "InBoundPack_x.TxPort"
                        EDDP_ASSERT_FALSE(pInboundMgmt->InboundGrps[InBoundIdx].WrkarPackGrpDisable.TxPort == EDDP_CRT_INBOUND_TXPORT_INVALID);
                        InboundRegVal = EDDP_HAL_REG32_READ(pDDB, InboundPackRegAddr);
                        EDDP_SET_BIT_VALUE32 (InboundRegVal, pInboundMgmt->InboundGrps[InBoundIdx].WrkarPackGrpDisable.TxPort, PNIP_REG_INBOUNDPACK_0__MSK_INBOUND_TXPORT, PNIP_REG_INBOUNDPACK_0_SHFT_INBOUND_TXPORT);
                        EDDP_HAL_REG32_WRITE(pDDB, InboundPackRegAddr, InboundRegVal);
                        // enable interrupt "ISR_InBound_Late_ERR" of this PackGrp
                        EDDP_CRTLateErrorSetupLateErrMask (pDDB, (LSA_UINT8)InBoundIdx, LSA_FALSE);
                    }
                    #endif

                    pInboundMgmt->InboundActiveMask &= ~((LSA_UINT32)1<<InBoundIdx);
                }
                InboundCtrlRegAddr += 4;
                InboundPackRegAddr += 4;
            }
        }

        // ***** disable full RED: RX --> reject frames,  TX --> reject
        EDDP_SET_BIT_VALUE32(RegValueFlowControl, 0x00000001, PNIP_REG_FLOWCONTROL_P1__MSK_RXBOUNDARY_DP2, PNIP_REG_FLOWCONTROL_P1_SHFT_RXBOUNDARY_DP2);
        EDDP_SET_BIT_VALUE32(RegValueFlowControl, 0x00000001, PNIP_REG_FLOWCONTROL_P1__MSK_TXBOUNDARY_DP2, PNIP_REG_FLOWCONTROL_P1_SHFT_TXBOUNDARY_DP2);
        EDDP_HAL_REG32_WRITE(pDDB, RegAddrFlowControl, RegValueFlowControl);
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTSetRxTxDP2BoundaryAndInbound(HwPortID=%d): set Rx/Tx-Boundary-DP2 active", 
            HwPortID);
    }
    else
    {
        /* ---------------------------------------- */
        /* enable full RED                          */
        /* ---------------------------------------- */
        // all disabled groups for given port
        InboundInActiveMask = (InboundUsedMask & ~(pInboundMgmt->InboundActiveMask));

        EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTSetRxTxDP2BoundaryAndInbound(HwPortID=%d): InboundUsedMask=0x%X, InboundInActiveMask=0x%X",
            HwPortID, InboundUsedMask, InboundInActiveMask);

        if (InboundInActiveMask)
        {   // ***** reinit AutoStop-Counter within Inbound PackRAM1
            InboundPackRegAddr = PNIP_REG_INBOUNDPACK_0;

            for ( InBoundIdx=0; InBoundIdx < EDDP_CRT_MAX_PACK_FRAMES_ERTEC200P; InBoundIdx++, InboundPackRegAddr+=4 )
            {
                if (InboundInActiveMask & ((LSA_UINT32)1<<InBoundIdx))
                {   // ***** group is disabled and must be reinitialized
                    InUseChkTo = 10000;
                    do
                    {   // ***** poll valid- and inuse bit
                        InboundRegVal = EDDP_HAL_REG32_READ(pDDB, InboundPackRegAddr);
                        InUseChkTo--;
                        if (!InUseChkTo)
                        {
                            EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTSetRxTxDP2BoundaryAndInbound(): FATAL -> timeout!!! InboundPackRegAddr=0x%X, InboundRegVal=0x%X, PackGrp(%d)",
                                InboundPackRegAddr, InboundRegVal, InBoundIdx);
                            EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
                        }
                    } while (InboundRegVal & EDDP_CRT_INBOUNDPACK_X_VALID_AND_INUSE);

                    // ***** fill AS-values
                    //       NOTE: setting of all WD/AS fields is faster than decoding used fields only !!!
                    EDDP_MEMSET_COMMON(pInboundMgmt->InboundGrps[InBoundIdx].pPackRam1Start, pInboundMgmt->InboundGrps[InBoundIdx].ReInitAsValue, EDDP_PACKRAM1_ASWD_SIZE);
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTSetRxTxDP2BoundaryAndInbound(): fill AS/WD-space within RAM1 for PackGrp(%d)", 
                        InBoundIdx);
                }
            }
        }

        // ***** enable full RED: RX --> receive frames,  TX --> delegate
        EDDP_SET_BIT_VALUE32(RegValueFlowControl, 0x00000000, PNIP_REG_FLOWCONTROL_P1__MSK_RXBOUNDARY_DP2, PNIP_REG_FLOWCONTROL_P1_SHFT_RXBOUNDARY_DP2);
        EDDP_SET_BIT_VALUE32(RegValueFlowControl, 0x00000000, PNIP_REG_FLOWCONTROL_P1__MSK_TXBOUNDARY_DP2, PNIP_REG_FLOWCONTROL_P1_SHFT_TXBOUNDARY_DP2);
        EDDP_HAL_REG32_WRITE(pDDB, RegAddrFlowControl, RegValueFlowControl);
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTSetRxTxDP2BoundaryAndInbound(HwPortID=%d): set Rx/Tx-Boundary-DP2 inactive", 
            HwPortID);

        if (InboundInActiveMask)
        {   // ***** enable PackGrps
            InboundCtrlRegAddr  = PNIP_REG_INBOUNDPACK_CONTROL_0_0;
            InboundPackRegAddr  = PNIP_REG_INBOUNDPACK_0;

            for ( InBoundIdx=0; InBoundIdx<EDDP_CRT_MAX_PACK_FRAMES_ERTEC200P; InBoundIdx++, InboundCtrlRegAddr += 4, InboundPackRegAddr += 4 )
            {
                if (InboundInActiveMask & ((LSA_UINT32)1<<InBoundIdx))
                {   
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTSetRxTxDP2BoundaryAndInbound(): enable PackGrp(%d)", 
                        InBoundIdx);

                    // disable interrupt "ISR_InBound_Late_ERR" of this PackGrp
                    EDDP_CRTLateErrorSetupLateErrMask (pDDB, (LSA_UINT8)InBoundIdx, LSA_TRUE);

                    // ***** enable RX path
                    InboundRegVal = EDDP_HAL_REG32_READ(pDDB, InboundCtrlRegAddr);
                    EDDP_SET_BIT_VALUE32(InboundRegVal, pInboundMgmt->InboundGrps[InBoundIdx].RxDisPort, PNIP_REG_INBOUNDPACK_CONTROL_0_0__MSK_INBOUND_RXPORT_DIS, PNIP_REG_INBOUNDPACK_CONTROL_0_0_SHFT_INBOUND_RXPORT_DIS);
                    EDDP_HAL_REG32_WRITE(pDDB, InboundCtrlRegAddr, InboundRegVal);
                    // ***** enable PackGrp
                    InboundRegVal = EDDP_HAL_REG32_READ(pDDB, InboundPackRegAddr);
                    EDDP_SET_BIT_VALUE32(InboundRegVal, 1, PNIP_REG_INBOUNDPACK_0__MSK_INBOUND_VALID, PNIP_REG_INBOUNDPACK_0_SHFT_INBOUND_VALID);
                    EDDP_HAL_REG32_WRITE(pDDB, InboundPackRegAddr, InboundRegVal);

                    // start LateError timer of this PackGrp
                    EDDP_CRTLateErrorTimerStartPackGrp (pDDB, (LSA_UINT8)InBoundIdx, EDDP_CRT_INBOUND_LATE_ERR_TIMER_TYPE_PACKGRP_ACTIVATE);

                    pInboundMgmt->InboundActiveMask |= (1<<InBoundIdx);
                }
            }
        }
    }

    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTSetRxTxDP2BoundaryAndInbound()");
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTInboundStateMachine                            +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE                   pDDB         +*/
/*+                      LSA_BOOL                                  ExecuteOff   +*/
/*+                      EDDP_CRT_CBF_PTR_TYPE                     Cbf          +*/
/*+                                                                             +*/
/*+  Result         :    EDD_RSP                                                +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  ExecuteOff     : EDDP_CRT_SET_INBOUND_OFF or EDDP_CRT_SET_INBOUND_ON       +*/
/*+  Cbf            : Callback to caller                                        +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: Activate/Passivate InBound pack.                              +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTInboundStateMachine(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_BOOL                    ExecuteOff,
    EDDP_CRT_CBF_PTR_TYPE       Cbf)
{
    EDD_RSP                                                         ResponseCmd;
    LSA_UINT16                                                      FrameID, TxPortID, DisPort, AutoStopMax;
    LSA_UINT32                                                      HwPortID;
    LSA_UINT8                                                       InBoundIdx, PackDgIdx;
    LSA_UINT32                                                      Value;
    LSA_BOOL                                                        SetDgOffs, bPutDgIntoPackRAM;
    LSA_UINT16                                                      Offset_DG, SubIdx, SubEnd;
    LSA_UINT8                                                       PackPosT, PackLen;
    LSA_UINT16                                                      PDIRFrameDataProvIdx;
    LSA_BOOL                                                        isFIDRedundantSet, bUseReverseDirection;
    EDDP_CRT_PACK_SM_PTR_TYPE                                       SmPtr;
    EDDP_CRT_INBOUND_SM_STATE                                       NextOnState;
    EDDP_CRT_INBOUND_MNGM_PTR_TYPE                              pInbound;
    EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SUBFRAMEDATA_PTR_TYPE   pSubFrameData;
    EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE                                 pPDIRFrameData;
    EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_PTR_TYPE                pSubFrameBlock;

    SmPtr = &pDDB->CRT.Cons.CrtPackSm;

    if (Cbf != 0)
    {   // ***** initialization of SM
        SmPtr->ExecuteOff                        = ExecuteOff;                       // store inparam from caller
        SmPtr->Smf                               = EDDP_CRTInboundStateMachine;      // bind right SM
        SmPtr->SM.Inbound.State                  = EDDP_CRT_INBOUND_SM_IDLE;
        SmPtr->SM.Inbound.RegInboundPack_X       = PNIP_REG_INBOUNDPACK_0;           // reg-addr
        SmPtr->SM.Inbound.RegInboundPackCtl_0_X  = PNIP_REG_INBOUNDPACK_CONTROL_0_0; // reg-addr
        SmPtr->SM.Inbound.RegInboundPackCtl_1_X  = PNIP_REG_INBOUNDPACK_CONTROL_1_0; // reg-addr

        SmPtr->SM.Inbound.InBoundIdx = 0;   // index for pack-grp (equal with CurrIdx for Inbound OFF)

        EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTInboundStateMachine(Cbf=0x%X, ExecuteOff=0x%X, Smf=0x%X)",
            Cbf, ExecuteOff, SmPtr->Smf);

        EDDP_ASSERT_FALSE( !((0 == SmPtr->Cbf)) ); // expect SM idle/free !!!
        SmPtr->Cbf      = Cbf;                                          // store CBF from caller
        SmPtr->CurrIdx  = 0;                                            // index (used as index into PRM-array for Inbound ON)

        if (ExecuteOff)
        {   // ***** Inbound OFF
            SmPtr->CurrEndCnt = EDDP_CRT_MAX_PACK_FRAMES_ERTEC200P; // 0..7
        }
        else
        {   // ***** Inbound ON
            SmPtr->pPDIRData            = pDDB->PRM.pPDIRData_A;            // used for Inbound ON
            SmPtr->pPDIRSubFrameData    = pDDB->PRM.pPDIRSubframeData_A;    // used for Inbound ON
            SmPtr->CurrEndCnt           = SmPtr->pPDIRSubFrameData->NumberOfSubframeBlocks;    // step through all PRM items, using InboundIdx for indexing the pack-grp
        }
    }
    else
    {   // ***** additional HW access after CMD OFF, going to next index/pack-grp/reg-addr
        if (ExecuteOff)
        {   // ***** Inbound OFF
            Value = EDDP_HAL_REG32_READ(pDDB, SmPtr->SM.Inbound.RegInboundPackCtl_0_X);
            EDDP_SET_BIT_VALUE32(Value, EDDP_INBOUND_RX_DISABLED, PNIP_REG_INBOUNDPACK_CONTROL_0_0__MSK_INBOUND_RXPORT_DIS, PNIP_REG_INBOUNDPACK_CONTROL_0_0_SHFT_INBOUND_RXPORT_DIS);
            EDDP_HAL_REG32_WRITE(pDDB, SmPtr->SM.Inbound.RegInboundPackCtl_0_X, Value);
            _EDDP_CRTPackRamResetDGs(pDDB, SmPtr->SM.Inbound.InBoundIdx);
            SmPtr->CurrIdx++;                               // next index
        }
        else
        {   // ***** Inbound ON --> no additional HW access after on-CMD, but special case for redundant grp
            if (SmPtr->SM.Inbound.State != EDDP_CRT_INBOUND_SM_EN_RED_GRP)
            {
                SmPtr->CurrIdx++;                       // next index
            }
        }
        SmPtr->SM.Inbound.InBoundIdx++;                 // next pack-grp
        SmPtr->SM.Inbound.RegInboundPack_X += 4;        // next reg-addr
        SmPtr->SM.Inbound.RegInboundPackCtl_0_X += 4;   // next reg-addr
        SmPtr->SM.Inbound.RegInboundPackCtl_1_X += 4;   // next reg-addr
        EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTInboundStateMachine(): go on with CurrIdx=%d, PackGrp(%d), RegInboundPack_X=0x%X, Smf=0x%X",
            SmPtr->CurrIdx, SmPtr->SM.Inbound.InBoundIdx, SmPtr->SM.Inbound.RegInboundPack_X, SmPtr->Smf);
    }

    ResponseCmd = EDD_STS_OK;
    pInbound    = &pDDB->CRT.Inbound;

    // stop the Inbound-LateErrLSATimer if it is running
    pInbound->LateErrPackGrpTimerUsedMask = 0;
    if (pInbound->isLateErrLSATimerRunning)
    {
        EDDP_CRTLateErrorStopLSATimer (pDDB);
    }

    while ((SmPtr->CurrIdx < SmPtr->CurrEndCnt) && (ResponseCmd == EDD_STS_OK))
    {
        InBoundIdx = SmPtr->SM.Inbound.InBoundIdx;

        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
        // delete data for WrkarPackGrpDisable
        pInbound->InboundGrps[InBoundIdx].WrkarPackGrpDisable.TxPort    = EDDP_CRT_INBOUND_TXPORT_INVALID;
        pInbound->InboundGrps[InBoundIdx].WrkarPackGrpDisable.AcwId     = EDDP_CRT_CHAIN_IDX_END;
        pInbound->InboundGrps[InBoundIdx].WrkarPackGrpDisable.AcwIdRed1 = EDDP_CRT_CHAIN_IDX_END;
        #endif

        if (SmPtr->ExecuteOff)
        {
            /* ------------------------------------ */
            /* Inbound OFF                          */
            /* ------------------------------------ */
            if (pInbound->InboundUsedMask[0] & ((LSA_UINT32)1<<InBoundIdx))
            {
                // ***** clear all active information for this inbound group
                pInbound->InboundActiveMask &= ~((LSA_UINT32)1<<InBoundIdx);

                for ( PackDgIdx=0; PackDgIdx<= EDDP_MAX_PORT_CNT; PackDgIdx++ )
                {   // ***** reset all used flags
                    pInbound->InboundUsedMask[PackDgIdx] &= ~((LSA_UINT32)1<<InBoundIdx);
                }

                pInbound->InboundGrps[InBoundIdx].InboundDgCnt                      = 0;
                // delete LateErrDiag data
                pInbound->InboundGrps[InBoundIdx].LateErrDiag.TimerType             = EDDP_CRT_INBOUND_LATE_ERR_TIMER_TYPE_INVALID;
                pInbound->InboundGrps[InBoundIdx].LateErrDiag.TimeTicks             = 0;
                pInbound->InboundGrps[InBoundIdx].LateErrDiag.PrmPDIRFrameDataIdx   = EDDP_CRT_CHAIN_IDX_END;
            }

            // ***** read PNIP_REG_INBOUNDPACK_x
            Value = EDDP_HAL_REG32_READ(pDDB, SmPtr->SM.Inbound.RegInboundPack_X);

            if (Value & PNIP_REG_INBOUNDPACK_0__MSK_INBOUND_VALID)
            {
                // ***** disable PackGrp(Idx) with asynchronous command
                SmPtr->CmdParam.CmdOpcode = EDDP_CMD_PACK_DISABLE_GRP;
                SmPtr->CmdParam.param_union.Pack.DisableGrp.DisPackGrp = InBoundIdx;
                SmPtr->CmdParam.param_union.Pack.DisableGrp.IsInGrp = 1;
                SmPtr->CmdParam.Cbf  = _EDDP_CRTCmdSmCbfCpm;

                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTInboundStateMachine(): PackGrp(%d), RegInboundPack_X=0x%X",
                    InBoundIdx, Value);

                ResponseCmd = EDDP_CmdRequest(pDDB, &SmPtr->CmdParam);
                EDDP_ASSERT_FALSE(ResponseCmd != EDD_STS_OK_PENDING);
                break;
            }
            else
            {
                SmPtr->CurrIdx++;                           // next index
                SmPtr->SM.Inbound.InBoundIdx++;             // next pack-grp for Inbound OFF
                SmPtr->SM.Inbound.RegInboundPack_X += 4;    // next reg-addr
            }
        }
        else
        {
            /* ------------------------------------ */
            /* Inbound ON                           */
            /* ------------------------------------ */
            pSubFrameBlock  = &SmPtr->pPDIRSubFrameData->SubframeBlockArray[SmPtr->CurrIdx];
            FrameID         = pSubFrameBlock->FrameID;

            if (pSubFrameBlock->HwConfig & EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_INBOUND_PACK_MASK)
            {
                isFIDRedundantSet       = LSA_FALSE;
                bUseReverseDirection    = LSA_FALSE;
                NextOnState             = EDDP_CRT_INBOUND_SM_IDLE;

                switch (SmPtr->SM.Inbound.State)
                {
                    case EDDP_CRT_INBOUND_SM_IDLE:
                    {
                        PDIRFrameDataProvIdx = pSubFrameBlock->PDIRFrameDataProvIdx;

                        if (pSubFrameBlock->HwConfig & EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_INBOUND_PACK_RED1_MASK)
                        {   // ***** PRM has detected: need 2 inbound settings with: FrameID(x) and FrameID(x+1) and FID_redundant=0 in both entries
                            NextOnState = EDDP_CRT_INBOUND_SM_EN_RED_GRP;
                        }
                        else
                        {   // ***** check here for non MRPD or MRPD with one inbound entry + FID_redundant=1
                            if (pSubFrameBlock->PDIRFrameDataProvIdxRed1 != EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
                            {   // ***** MRPD with one inbound entry + FID_redundant=1
                                isFIDRedundantSet = LSA_TRUE;
                            }
                        }
                    }
                    break;

                    case EDDP_CRT_OUTBOUND_SM_EN_RED_GRP:
                    {
                        PDIRFrameDataProvIdx = pSubFrameBlock->PDIRFrameDataProvIdxRed1;
                        FrameID |= 1;   // second entry for FrameID(x+1)

                        if (pSubFrameBlock->SFIOCRProperties.DFPRedundantPathLayout == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_DOWN)
                        {
                            bUseReverseDirection = LSA_TRUE;
                        }
                    }
                    break;

                    default:
                    {
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTInboundStateMachine(): FATAL -> unknown SM.Inbound.State=%d",
                            SmPtr->SM.Inbound.State);
                        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
                        PDIRFrameDataProvIdx = EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED;
                    }
                    break;
                }

                // ***** with INBOUND_PACK_MASK we expect always a valid PDIRFrameDataProvIdx
                if (PDIRFrameDataProvIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
                {
                    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTInboundStateMachine(): FATAL -> PDIRFrameDataProvIdx/Red1 is invalid!");
                    EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
                }
                pPDIRFrameData = &SmPtr->pPDIRData->pIRFrameDataArray[PDIRFrameDataProvIdx];

                // ***** create PackRAM --> depends on DFPmode
                SetDgOffs   = LSA_TRUE;
                Offset_DG   = 0;

                if (pSubFrameBlock->SFIOCRProperties.DFPmode != EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_DFPMODE_END_NODE)
                {   // ***** this is Device
                    bPutDgIntoPackRAM = bUseReverseDirection;
                    // ***** last device: set terminator only because this device sends the start-DG
                    // ***** other device in line: set DG structure of all neighbours in direction to last device
                    SubEnd = pSubFrameBlock->NumberOfSubframeData;
                    EDDP_ASSERT_FALSE(SubEnd>255);
                    PackDgIdx = 0;
                    // init LateErrDiag data
                    pInbound->InboundGrps[InBoundIdx].LateErrDiag.TimerType             = EDDP_CRT_INBOUND_LATE_ERR_TIMER_TYPE_INVALID;
                    pInbound->InboundGrps[InBoundIdx].LateErrDiag.TimeTicks             = 0;
                    pInbound->InboundGrps[InBoundIdx].LateErrDiag.PrmPDIRFrameDataIdx   = PDIRFrameDataProvIdx;

                    bPutDgIntoPackRAM = LSA_FALSE;

                    if (bUseReverseDirection)
                    {   // ***** bUseReverseDirection=1: search reverse!!! find own DG and all pending DGs are relevant for the PackRAM 
                        SubIdx = (SubEnd-1);
                    }
                    else
                    {   // ***** bUseReverseDirection=0: find own DG and all pending DGs are relevant for the PackRAM
                        SubIdx = 0;
                    }

                    while (SubEnd)
                    {
                        pSubFrameData = &pSubFrameBlock->SubframeDataArray[SubIdx];

                        if (pSubFrameBlock->SFIOCRProperties.DFPmode == pSubFrameData->Position)
                        {
                            bPutDgIntoPackRAM = LSA_TRUE;
                            EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTInboundStateMachine(): PackGrp(%d), OwnPosT=%d, FrameID=0x%X",
                                InBoundIdx, pSubFrameData->Position, FrameID);
                        }
                        else
                        {
                            if (bPutDgIntoPackRAM)
                            {
                                EDDP_ASSERT_FALSE(pSubFrameData->Position>255);
                                EDDP_ASSERT_FALSE(pSubFrameData->DataLength>255);
                                PackPosT  = (LSA_UINT8)pSubFrameData->Position;
                                PackLen   = (LSA_UINT8)pSubFrameData->DataLength;
                                Offset_DG = _EDDP_CRTPackRamSetDgPos(pDDB, InBoundIdx, Offset_DG, PackPosT, PackLen, SetDgOffs);

                                EDDP_CRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTInboundStateMachine(): PackGrp(%d), set PosT=%d, Len=%d, Offset_DG=0x%X, FrameID=0x%X",
                                    InBoundIdx, PackPosT, PackLen, Offset_DG, FrameID);

                                pInbound->InboundGrps[InBoundIdx].InboundDGs[PackDgIdx].PackPosT  = PackPosT;
                                pInbound->InboundGrps[InBoundIdx].InboundDGs[PackDgIdx].PackLen   = PackLen;
                                pInbound->InboundGrps[InBoundIdx].InboundDGs[PackDgIdx].Offset_DG = Offset_DG;
                                pInbound->InboundGrps[InBoundIdx].InboundDgCnt++;
                                PackDgIdx++;
                                SetDgOffs = LSA_FALSE;
                            }
                        }

                        if (bUseReverseDirection)
                        {   // ***** reverse: End-->0
                            SubIdx--;
                        }
                        else
                        {   // ***** normal:  0-->End
                            SubIdx++;
                        }

                        SubEnd--;
                    }
                } 
                else
                {   
                    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTInboundStateMachine(FrameID=0x%X, HwConfig=0x%X, DFPmode=%d): no Inbound expected here !!!",
                        FrameID, pSubFrameBlock->HwConfig, pSubFrameBlock->SFIOCRProperties.DFPmode);
                    SmPtr->CurrIdx++;                           // next index
                    continue;
                }

                // ***** set terminator
                // ***** Controller/MultiDevice: EDDP_PACKRAM_GRPCTRL_SIZE
                // ***** Device: full DG_Len + EDDP_PACKRAM_GRPCTRL_SIZE
                Offset_DG = _EDDP_CRTPackRamSetDgPos(pDDB, InBoundIdx, Offset_DG, 0x00, 0x00, SetDgOffs);   

                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTInboundStateMachine(): set terminator PackGrp(%d), Offset_DG=0x%X", 
                    InBoundIdx, Offset_DG);

                EDDP_ASSERT_FALSE((pSubFrameBlock->SFIOCRProperties.DFPmode == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_DFPMODE_END_NODE));
                {   // ***** Device: configure InBound here
                    // ***** read PNIP_REG_INBOUNDPACK_x
                    Value = EDDP_HAL_REG32_READ(pDDB, SmPtr->SM.Inbound.RegInboundPack_X);
                    // ***** set Interface-Bit
                    EDDP_SET_BIT_VALUE32(Value, EDDP_IF_A, PNIP_REG_INBOUNDPACK_0__MSK_INBOUND_INTERFACE, PNIP_REG_INBOUNDPACK_0_SHFT_INBOUND_INTERFACE);
                    // ***** set Redundant-Bit
                    if (isFIDRedundantSet)
                    {   EDDP_SET_BIT_VALUE32(Value, 1, PNIP_REG_INBOUNDPACK_0__MSK_FID_REDUNDANT, PNIP_REG_INBOUNDPACK_0_SHFT_FID_REDUNDANT);    }
                    else
                    {   EDDP_SET_BIT_VALUE32(Value, 0, PNIP_REG_INBOUNDPACK_0__MSK_FID_REDUNDANT, PNIP_REG_INBOUNDPACK_0_SHFT_FID_REDUNDANT);    }

                    // ***** set TxPort (Note: set here TX port equal to SndACW only, because HW uses TX port from ACW)
                    TxPortID = pPDIRFrameData->TxPort;
                    EDDP_ASSERT_FALSE(TxPortID==0);
                    HwPortID = EDDP_SWI_get_HwPortID (pDDB, TxPortID);
                    EDDP_SET_BIT_VALUE32(Value, HwPortID-1, PNIP_REG_INBOUNDPACK_0__MSK_INBOUND_TXPORT, PNIP_REG_INBOUNDPACK_0_SHFT_INBOUND_TXPORT);

                    #ifdef EDDP_MESSAGE
                    #ifdef EDDP_PRAGMA_MESSAGE
                    #pragma EDDP_PRAGMA_MESSAGE("Workaround CRT.WrkarPackGrpDisable for PNIP-Revision(1) - AP01372025")
                    #endif /* EDDP_PRAGMA_MESSAGE */
                    #endif /* EDDP_MESSAGE */
                    #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
                    if ( EDDP_HAL_HW_PNIP_REV1 == EDDP_HALGetPnipRevision(pDDB) )
                    {
                        // store data for WrkarPackGrpDisable
                        pInbound->InboundGrps[InBoundIdx].WrkarPackGrpDisable.TxPort = (LSA_UINT8) (HwPortID-1);

                        pInbound->InboundGrps[InBoundIdx].WrkarPackGrpDisable.AcwId = EDDP_CRTProvDfpGetAcwIDFromInBoundPackGroupFrameID (pDDB, FrameID, LSA_FALSE);
                        EDDP_ASSERT_FALSE(pInbound->InboundGrps[InBoundIdx].WrkarPackGrpDisable.AcwId == EDDP_CRT_CHAIN_IDX_END);

                        if (isFIDRedundantSet)
                        {
                            pInbound->InboundGrps[InBoundIdx].WrkarPackGrpDisable.AcwIdRed1 = EDDP_CRTProvDfpGetAcwIDFromInBoundPackGroupFrameID (pDDB, FrameID, LSA_TRUE);
                            EDDP_ASSERT_FALSE(pInbound->InboundGrps[InBoundIdx].WrkarPackGrpDisable.AcwIdRed1 == EDDP_CRT_CHAIN_IDX_END);
                        }

                    }
                    #endif

                    // ***** set FrameID
                    EDDP_SET_BIT_VALUE32(Value, FrameID, PNIP_REG_INBOUNDPACK_0__MSK_INBOUND_FRAMEID, PNIP_REG_INBOUNDPACK_0_SHFT_INBOUND_FRAMEID);
                    // ***** write PNIP_REG_INBOUNDPACK_x
                    EDDP_HAL_REG32_WRITE(pDDB, SmPtr->SM.Inbound.RegInboundPack_X, Value);
                    // ***** set used flag for port and interface mask
                    if (pPDIRFrameData->TxPort <= EDDP_MAX_PORT_CNT)
                    {   // ***** store outgoing port (TX)
                        HwPortID = EDDP_SWI_get_HwPortID (pDDB, pPDIRFrameData->TxPort);
                        pInbound->InboundUsedMask[HwPortID] |= (1<<InBoundIdx);
                    }
                    pInbound->InboundUsedMask[0] |= (1<<InBoundIdx);

                    // ***** read PNIP_REG_INBOUNDPACK_CONTROL_0_x
                    Value = EDDP_HAL_REG32_READ(pDDB, SmPtr->SM.Inbound.RegInboundPackCtl_0_X);
                    // set DG_WDog_max (DistributedWatchDogFactor)
                    if (isFIDRedundantSet)
                    {
                        // MRPD with one inbound entry for FrameID(x) and (x+1) with same TxPort, so DG-Watchdog is double
                        EDDP_ASSERT_FALSE((2 * pSubFrameBlock->SFIOCRProperties.DistributedWatchDogFactor) > EDDP_PRM_PNIP_MAX_DISTRIBUTEDWATCHDOGFACTOR);
                        EDDP_SET_BIT_VALUE32( Value, 
                                              (2 * pSubFrameBlock->SFIOCRProperties.DistributedWatchDogFactor), 
                                              PNIP_REG_INBOUNDPACK_CONTROL_0_0__MSK_LBUF_WDOG_MAX, 
                                              PNIP_REG_INBOUNDPACK_CONTROL_0_0_SHFT_LBUF_WDOG_MAX);
                    }
                    else
                    {
                        EDDP_SET_BIT_VALUE32( Value, 
                                              pSubFrameBlock->SFIOCRProperties.DistributedWatchDogFactor, 
                                              PNIP_REG_INBOUNDPACK_CONTROL_0_0__MSK_LBUF_WDOG_MAX, 
                                              PNIP_REG_INBOUNDPACK_CONTROL_0_0_SHFT_LBUF_WDOG_MAX);
                    }

                    // set AutoStopMax
                    AutoStopMax = pSubFrameBlock->SFIOCRProperties.RestartFactorForDistributedWD;
                    EDDP_ASSERT_FALSE(!(EDDP_PRM_IS_DFP_RESTARTFACTORFORDISTRIBUTEDWD_IN_VALID_RANGE(AutoStopMax)));

                    if (AutoStopMax)
                    {   // Note: BaseTime counts with 64ms --> convert AutoStopMax
                        #if (EDDP_CRT_REG_AUTOSTOP_BASETIME_INIT == 0x7A1794)
                            // ***** (AutoStopMax*100)/64ms
                            AutoStopMax = (LSA_UINT16)((((LSA_UINT32)AutoStopMax*100)>>6)+1);
                        #else
                            // ***** (UserAutoStop * 100ms / BaseTime*8ns)+1
                            //       (UserAutoStop * (100ms/8) / BaseTime)+1
                            AutoStopMax = (LSA_UINT16)(((LSA_UINT32)AutoStopMax * (100000000L>>3)) / EDDP_CRT_REG_AUTOSTOP_BASETIME_INIT)+1;
                        #endif
                        EDDP_ASSERT_FALSE( AutoStopMax > (PNIP_REG_INBOUNDPACK_CONTROL_0_0__MSK_AUTOSTOP_MAX >> PNIP_REG_INBOUNDPACK_CONTROL_0_0_SHFT_AUTOSTOP_MAX) );
                    }

                    pDDB->CRT.Inbound.InboundGrps[SmPtr->SM.Inbound.InBoundIdx].ReInitAsValue = (LSA_UINT8) ((EDDP_CRT_INBOUND_REINITASVALUE_INIT) | AutoStopMax);

                    EDDP_SET_BIT_VALUE32(Value, AutoStopMax, PNIP_REG_INBOUNDPACK_CONTROL_0_0__MSK_AUTOSTOP_MAX, PNIP_REG_INBOUNDPACK_CONTROL_0_0_SHFT_AUTOSTOP_MAX);

                    // init RxDisPort
                    // get RxPort from PRM
                    if (PDIRFrameDataProvIdx == pSubFrameBlock->PDIRFrameDataProvIdxRed1)
                    {   // MRPD with FrameID(x+1)
                        DisPort = _EDDP_CRTInboundGetRxPort (SmPtr->pPDIRData, pSubFrameBlock, LSA_TRUE);
                    }
                    else
                    {   // MRPD "im Stich" or non MRPD
                        DisPort = _EDDP_CRTInboundGetRxPort (SmPtr->pPDIRData, pSubFrameBlock, LSA_FALSE);
                    }
                    // convert DisPort to mask value RxAllowed
                    if ((DisPort > 0) && (DisPort <= pDDB->SWI.PortCnt))
                    {   // convert to HwPort
                        HwPortID    = EDDP_SWI_get_HwPortID (pDDB, DisPort);

                        DisPort     = ((HwPortID==1) ? EDDP_INBOUND_RX1_ALLOWED : 
                                      ((HwPortID==2) ? EDDP_INBOUND_RX2_ALLOWED : 
                                      ((HwPortID==3) ? EDDP_INBOUND_RX3_ALLOWED :
                                      ((HwPortID==4) ? EDDP_INBOUND_RX4_ALLOWED : 0))));
                    }
                    else
                    {   // all RxPorts are disabled (no receive)
                        DisPort = EDDP_INBOUND_RX_DISABLED;
                    }
                    EDDP_ASSERT_FALSE(DisPort == 0);
                    // store RxDisPort in InBound management
                    pDDB->CRT.Inbound.InboundGrps[SmPtr->SM.Inbound.InBoundIdx].RxDisPort = DisPort;

                    // set RxDisPort in register here always to all disabled - it will updated if the InBound pack group is activated on RTC3PSM
                    EDDP_SET_BIT_VALUE32(Value, EDDP_INBOUND_RX_DISABLED, PNIP_REG_INBOUNDPACK_CONTROL_0_0__MSK_INBOUND_RXPORT_DIS, PNIP_REG_INBOUNDPACK_CONTROL_0_0_SHFT_INBOUND_RXPORT_DIS);
                    // ***** write PNIP_REG_INBOUNDPACK_CONTROL_0_x
                    EDDP_HAL_REG32_WRITE(pDDB, SmPtr->SM.Inbound.RegInboundPackCtl_0_X, Value);

                    // ***** read PNIP_REG_INBOUNDPACK_CONTROL_1_x
                    Value = EDDP_HAL_REG32_READ(pDDB, SmPtr->SM.Inbound.RegInboundPackCtl_1_X);
                    // InBoundPack_Control_1_x .InBound_Pos_Nr = own DG (but not relevant if Pack_kind = 0)
                    EDDP_SET_BIT_VALUE32(Value, pSubFrameBlock->SFIOCRProperties.DFPmode, PNIP_REG_INBOUNDPACK_CONTROL_1_0__MSK_INBOUND_POS_NR, PNIP_REG_INBOUNDPACK_CONTROL_1_0_SHFT_INBOUND_POS_NR);
                    // InBoundPack_Control_1_x .Pack_kind = 0
                    EDDP_SET_BIT_VALUE32(Value, 0, PNIP_REG_INBOUNDPACK_CONTROL_1_0__MSK_PACK_KIND, PNIP_REG_INBOUNDPACK_CONTROL_1_0_SHFT_PACK_KIND);
                    // InBoundPack_Control_1_x.FrameDataLength = Header-Bytes + FrameDataLength + APDU-Bytes
                    //                                         = DA + SA + VLAN + EtherType + FrameID  + FrameDataLength + APDU-Bytes
                    //                                         = 20 + FrameDataLength + 4
                    // FrameDataLength = pPDIRFrameData->DataLength (or: HCS + all relevant DGs + Terminator)
                    // HCS=2,  sum of all DGs: (6+Dg.Len) , Terminator=2
                    // --> FrameDataLength used by PackCtrl to have the data length for (time) calculation
                    EDDP_SET_BIT_VALUE32(Value, pPDIRFrameData->DataLength, PNIP_REG_INBOUNDPACK_CONTROL_1_0__MSK_FRAMEDATALENGTH, PNIP_REG_INBOUNDPACK_CONTROL_1_0_SHFT_FRAMEDATALENGTH);
                    // ***** write PNIP_REG_INBOUNDPACK_CONTROL_1_x
                    EDDP_HAL_REG32_WRITE(pDDB, SmPtr->SM.Inbound.RegInboundPackCtl_1_X, Value);

                    // ***** PackGroup will be enabled by the RTC3PSM !!!
                    SmPtr->SM.Inbound.State = NextOnState;
                    if (SmPtr->SM.Inbound.State != EDDP_CRT_INBOUND_SM_EN_RED_GRP)
                    {
                        SmPtr->CurrIdx++;                           // next index
                    }
                    SmPtr->SM.Inbound.InBoundIdx++;                 // next pack-grp
                    SmPtr->SM.Inbound.RegInboundPack_X += 4;        // next reg-addr
                    SmPtr->SM.Inbound.RegInboundPackCtl_0_X += 4;   // next reg-addr
                    SmPtr->SM.Inbound.RegInboundPackCtl_1_X += 4;   // next reg-addr

                    EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTInboundStateMachine(): go on with: CurrIdx=%d, PackGrp(%d), RegInboundPack_X=0x%X, Smf=0x%X",
                        SmPtr->CurrIdx, SmPtr->SM.Inbound.InBoundIdx, SmPtr->SM.Inbound.RegInboundPack_X, SmPtr->Smf);
                }
            }
            else
            {   // ***** increment CurrIdx only !!!
                SmPtr->CurrIdx++;   // next index
            }
        }
        
    }

    // ***** is SM completed?
    if (SmPtr->CurrIdx >= SmPtr->CurrEndCnt)
    {
        EDDP_ASSERT_FALSE(ResponseCmd != EDD_STS_OK);         // expect EDD_STS_OK on completion

        if (Cbf == 0)
        {   // ***** CBF allowed only if SM is called by own CBF --> with parameter Cbf = LSA_NULL !!!
            EDDP_ASSERT_NO_NULL_FCT_PTR(SmPtr->Cbf);           // expect valid cbf-pointer
            Cbf         = SmPtr->Cbf;                   // temporary store for clearing SmPtr->Cbf before calling the CBF
            SmPtr->Cbf  = 0;                     // set SM idle/free !!!
            Cbf(pDDB, EDD_STS_OK);                      // return to the caller
        }
        else
        {
            SmPtr->Cbf = 0;                     // set SM idle/free !!!
        }
    }

    return (ResponseCmd);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTOutboundStateMachine                           +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE                   pDDB         +*/
/*+                      LSA_BOOL                                  ExecuteOff   +*/
/*+                      EDDP_CRT_CBF_PTR_TYPE                     Cbf          +*/
/*+                                                                             +*/
/*+  Result         :    EDD_RSP                                                +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  ExecuteOff     : EDDP_CRT_SET_OUTBOUND_OFF or EDDP_CRT_SET_OUTBOUND_ON     +*/
/*+  Cbf            : Callback to caller                                        +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: Activate/Passivate OutBound pack.                             +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTOutboundStateMachine(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_BOOL                    ExecuteOff,
    EDDP_CRT_CBF_PTR_TYPE       Cbf)
{
    EDD_RSP                                             ResponseCmd;
    LSA_UINT16                                          FrameID, TxPortID;
    LSA_UINT32                                          HwPortID;
    LSA_UINT32                                          Value;
    LSA_UINT16                                          PDIRFrameDataConsIdx;
    EDDP_CRT_PACK_SM_PTR_TYPE                           SmPtr;
    EDDP_CRT_OUTBOUND_SM_STATE                          NextOnState;
    EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE                     pPDIRFrameData, pPDIRFrameDataTemp;
    EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_PTR_TYPE    pSubFrameBlock;

    SmPtr = &pDDB->CRT.Cons.CrtPackSm;

    if (Cbf != 0)
    {   // ***** initialization of SM
        SmPtr->ExecuteOff                       = ExecuteOff;                      // store inparam from caller
        SmPtr->Smf                              = EDDP_CRTOutboundStateMachine;    // bind right SM
        SmPtr->SM.Outbound.State                = EDDP_CRT_OUTBOUND_SM_IDLE;
        SmPtr->SM.Outbound.RegOutboundPack_X    = PNIP_REG_OUTBOUNDPACK_0;         // reg-addr
        SmPtr->SM.Outbound.OutBoundIdx          = 0;                               // index for pack-grp (equal with CurrIdx for Outbound OFF)

        EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTOutboundStateMachine(Cbf=0x%X, ExecuteOff=%d, Smf=0x%X)",
            Cbf, ExecuteOff, SmPtr->Smf);

        EDDP_ASSERT_FALSE( ! (0 == SmPtr->Cbf) );             // expect SM idle/free !!!
        SmPtr->Cbf                              = Cbf;                              // store CBF from caller
        SmPtr->CurrIdx                          = 0;                                // index (used as index into PRM-array for Outbound ON)

        if (ExecuteOff)
        {   // ***** Outbound OFF
            SmPtr->CurrEndCnt           = EDDP_CRT_MAX_PACK_FRAMES_ERTEC200P;       // 0..7
        }
        else
        {   // ***** Outbound ON
            SmPtr->pPDIRData            = pDDB->PRM.pPDIRData_A;                    // used for Outbound ON
            SmPtr->pPDIRSubFrameData    = pDDB->PRM.pPDIRSubframeData_A;            // used for Outbound ON
            SmPtr->CurrEndCnt           = SmPtr->pPDIRSubFrameData->NumberOfSubframeBlocks;    // step through all PRM items,  using OutBoundIdx for indexing the pack-grp
        }
    }
    else
    {   
        if (SmPtr->ExecuteOff)
        {   // ***** no additional HW access after off-CMD, going to next index/pack-grp/reg-addr
            SmPtr->SM.Outbound.State    = EDDP_CRT_OUTBOUND_SM_IDLE;
            SmPtr->CurrIdx++;   // next index
        }
        else
        {   // ***** no additional HW access after on-CMD, but special case for redundant grp
            if (SmPtr->SM.Outbound.State != EDDP_CRT_OUTBOUND_SM_EN_RED_GRP)
            {
                SmPtr->CurrIdx++;                   // next index
            }
        }
        SmPtr->SM.Outbound.OutBoundIdx++;           // next pack-grp
        SmPtr->SM.Outbound.RegOutboundPack_X += 4;  // next reg-addr

        EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTOutboundStateMachine(): go on with: CurrIdx=%d, PackGrp(%d), RegOutboundPack_X=0x%X, Smf=0x%X", 
            SmPtr->CurrIdx, SmPtr->SM.Outbound.OutBoundIdx, SmPtr->SM.Outbound.RegOutboundPack_X, SmPtr->Smf);
    }

    ResponseCmd = EDD_STS_OK;

    while ((SmPtr->CurrIdx < SmPtr->CurrEndCnt) && (ResponseCmd == EDD_STS_OK))
    {
        if (SmPtr->ExecuteOff)
        {
            /* ------------------------------------ */
            /* Outbound OFF                         */
            /* ------------------------------------ */
            Value = EDDP_HAL_REG32_READ(pDDB, SmPtr->SM.Outbound.RegOutboundPack_X);

            if (Value & PNIP_REG_OUTBOUNDPACK_0__MSK_OUTBOUND_VALID)
            {   // ***** disable OutGrp(Idx) with asynchronous CMD
                SmPtr->CmdParam.CmdOpcode = EDDP_CMD_PACK_DISABLE_GRP;
                SmPtr->CmdParam.param_union.Pack.DisableGrp.DisPackGrp = SmPtr->SM.Outbound.OutBoundIdx;
                SmPtr->CmdParam.param_union.Pack.DisableGrp.IsInGrp = 0;
                SmPtr->CmdParam.Cbf  = _EDDP_CRTCmdSmCbfCpm;

                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTOutboundStateMachine(): disable PackGrp(%d), RegOutboundPack_X=0x%X", 
                    SmPtr->SM.Outbound.OutBoundIdx, Value);

                ResponseCmd = EDDP_CmdRequest(pDDB, &SmPtr->CmdParam);
                EDDP_ASSERT_FALSE(ResponseCmd != EDD_STS_OK_PENDING);
                break;
            }
            else
            {
                SmPtr->CurrIdx++;                           // next index
                SmPtr->SM.Outbound.OutBoundIdx++;           // next pack-grp for Outbound OFF
                SmPtr->SM.Outbound.RegOutboundPack_X += 4;  // next reg-addr
            }
        }
        else
        {
            /* ------------------------------------ */
            /* Outbound ON                          */
            /* ------------------------------------ */
            pSubFrameBlock  = &SmPtr->pPDIRSubFrameData->SubframeBlockArray[SmPtr->CurrIdx];
            FrameID         = pSubFrameBlock->FrameID;

            if (pSubFrameBlock->HwConfig & EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_OUTBOUND_PACK_MASK)
            {
                // ***** read PNIP_REG_OUTBOUNDPACK_x
                Value = EDDP_HAL_REG32_READ(pDDB, SmPtr->SM.Outbound.RegOutboundPack_X);
                // ***** set Interface-Bit
                EDDP_SET_BIT_VALUE32(Value, EDDP_IF_A, PNIP_REG_OUTBOUNDPACK_0__MSK_OUTBOUND_INTERFACE, PNIP_REG_OUTBOUNDPACK_0_SHFT_OUTBOUND_INTERFACE);

                // ***** set Redundant-Bit --> default no
                EDDP_SET_BIT_VALUE32(Value, 0, PNIP_REG_OUTBOUNDPACK_0__MSK_FID_REDUNDANT, PNIP_REG_OUTBOUNDPACK_0_SHFT_FID_REDUNDANT);

                NextOnState = EDDP_CRT_OUTBOUND_SM_IDLE;

                switch (SmPtr->SM.Outbound.State)
                {
                    case EDDP_CRT_OUTBOUND_SM_IDLE:
                    {
                        PDIRFrameDataConsIdx = pSubFrameBlock->PDIRFrameDataConsIdx;

                        if (pSubFrameBlock->HwConfig & EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_OUTBOUND_PACK_RED1_MASK)
                        {   // ***** PRM has detected: need two outbound settings with: FrameID=x and FrameID=x+1 and FID=0 in both entries
                            NextOnState = EDDP_CRT_OUTBOUND_SM_EN_RED_GRP;
                        }
                        else
                        {   // ***** check here for non MRPD   or   MRPD with one outbound entry + FID=1
                            if (pSubFrameBlock->PDIRFrameDataConsIdxRed1 != EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
                            {   // ***** MRPD with one outbound entry + FID=1
                                EDDP_SET_BIT_VALUE32(Value, 1, PNIP_REG_OUTBOUNDPACK_0__MSK_FID_REDUNDANT, PNIP_REG_OUTBOUNDPACK_0_SHFT_FID_REDUNDANT);
                            }
                        }
                    }
                    break;

                    case EDDP_CRT_OUTBOUND_SM_EN_RED_GRP:
                    {
                        PDIRFrameDataConsIdx = pSubFrameBlock->PDIRFrameDataConsIdxRed1;
                        FrameID |= 1;   // second entry for: FrameID=x+1
                    }
                    break;

                    default:
                    {
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTOutboundStateMachine(): FATAL -> unknown SM.Outbound.State=%d",
                            SmPtr->SM.Outbound.State);
                        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
                        PDIRFrameDataConsIdx = EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED;
                    }
                    break;
                }

                // ***** with OUTBOUND_PACK_MASK we expect always a valid PDIRFrameDataConsIdx
                if (PDIRFrameDataConsIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
                {
                    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTOutboundStateMachine(): FATAL -> PDIRFrameDataConsIdx/Red1 is invalid!");
                    EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
                }
                pPDIRFrameData = &SmPtr->pPDIRData->pIRFrameDataArray[PDIRFrameDataConsIdx];

                // ***** set TX-Port
                TxPortID = 0;
                if (pPDIRFrameData->isDoubleFrameID)
                {
                    // FrameID is double in PDIRFrameData: we need provider entry
                    if ( (EDDP_IS_IRT_REDUNDANT_FRAMEID(pSubFrameBlock->FrameID)) && ((pSubFrameBlock->FrameID+1) == FrameID) )
                    {
                        // only for redundant FrameID(x+1)
                        EDDP_ASSERT_FALSE(pSubFrameBlock->PDIRFrameDataProvIdxRed1 == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
                        pPDIRFrameDataTemp = &SmPtr->pPDIRData->pIRFrameDataArray[pSubFrameBlock->PDIRFrameDataProvIdxRed1];
                        TxPortID = pPDIRFrameDataTemp->TxPort;
                    }
                    else
                    {
                        // all other FrameIDs
                        EDDP_ASSERT_FALSE(pSubFrameBlock->PDIRFrameDataProvIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
                        pPDIRFrameDataTemp = &SmPtr->pPDIRData->pIRFrameDataArray[pSubFrameBlock->PDIRFrameDataProvIdx];
                        TxPortID = pPDIRFrameDataTemp->TxPort;
                    }
                }
                else
                {
                    // FrameID is one in PDIRFrameData
                    if (pPDIRFrameData->SndRcvRole == EDDP_PRM_PDIRFRAMEDATA_ROLE_CONSUMER)
                    {   // ***** only local receiption --> set TX with RX-Port (special case !!!)
                        TxPortID = pPDIRFrameData->RxPort;  // no forwarding if TX==RX
                    }
                    else
                    {   // ***** forwarding --> set given TX port
                        TxPortID = pPDIRFrameData->TxPort;
                    }
                }
                EDDP_ASSERT_FALSE(TxPortID==0);
                HwPortID = EDDP_SWI_get_HwPortID (pDDB, TxPortID);
                EDDP_SET_BIT_VALUE32(Value, HwPortID-1, PNIP_REG_OUTBOUNDPACK_0__MSK_OUTBOUND_TXPORT, PNIP_REG_OUTBOUNDPACK_0_SHFT_OUTBOUND_TXPORT);

                // ***** set Wo_DCS-Bit
                if (pSubFrameBlock->SFIOCRProperties.SFCRC16)
                {   // ***** check all DGs with chksum
                    EDDP_SET_BIT_VALUE32(Value, 0, PNIP_REG_OUTBOUNDPACK_0__MSK_OUTBOUND_WO_DCS, PNIP_REG_OUTBOUNDPACK_0_SHFT_OUTBOUND_WO_DCS); 
                }
                else
                {   // ***** no chksum checking
                    EDDP_SET_BIT_VALUE32(Value, 1, PNIP_REG_OUTBOUNDPACK_0__MSK_OUTBOUND_WO_DCS, PNIP_REG_OUTBOUNDPACK_0_SHFT_OUTBOUND_WO_DCS); 
                }
                // ***** set POS_NR (0 if EndNode, else DFPmode holds PosNr from 1..63)
                EDDP_SET_BIT_VALUE32(Value, pSubFrameBlock->SFIOCRProperties.DFPmode, PNIP_REG_OUTBOUNDPACK_0__MSK_OUTBOUND_POSNR, PNIP_REG_OUTBOUNDPACK_0_SHFT_OUTBOUND_POSNR);
                // ***** set FrameID
                EDDP_SET_BIT_VALUE32(Value, FrameID, PNIP_REG_OUTBOUNDPACK_0__MSK_OUTBOUND_FRAMEID, PNIP_REG_OUTBOUNDPACK_0_SHFT_OUTBOUND_FRAMEID);
                // ***** write back to PNIP_REG_OUTBOUNDPACK_x
                EDDP_HAL_REG32_WRITE(pDDB, SmPtr->SM.Outbound.RegOutboundPack_X, Value);

                // ***** enable OutGrp[Idx] with asynchronous CMD
                SmPtr->CmdParam.CmdOpcode = EDDP_CMD_PACK_ENABLE_GRP;
                SmPtr->CmdParam.param_union.Pack.EnableGrp.EnPackGrp = SmPtr->SM.Outbound.OutBoundIdx;
                SmPtr->CmdParam.param_union.Pack.EnableGrp.IsInGrp = 0;
                SmPtr->CmdParam.Cbf  = _EDDP_CRTCmdSmCbfCpm;

                EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTOutboundStateMachine(): enable PackGrp(%d), CurrIdx=%d, FrameID=0x%X",
                    SmPtr->SM.Outbound.OutBoundIdx, SmPtr->CurrIdx, FrameID);

                ResponseCmd = EDDP_CmdRequest(pDDB, &SmPtr->CmdParam);
                EDDP_ASSERT_FALSE(ResponseCmd != EDD_STS_OK_PENDING);

                SmPtr->SM.Outbound.State = NextOnState;
            }
            else
            {   // ***** increment CurrIdx only !!!
                SmPtr->CurrIdx++;   // next index
            }
        }
        
    }

    // ***** is SM completed?
    if (SmPtr->CurrIdx >= SmPtr->CurrEndCnt)
    {
        EDDP_ASSERT_FALSE(ResponseCmd != EDD_STS_OK);         // expect EDD_STS_OK on completion

        if (Cbf == 0)
        {   // ***** CBF allowed only if SM is called by own CBF --> with parameter Cbf = LSA_NULL !!!
            EDDP_ASSERT_NO_NULL_FCT_PTR(SmPtr->Cbf);           // expect valid cbf-pointer
            Cbf         = SmPtr->Cbf;                   // temporary store for clearing SmPtr->Cbf before calling the CBF
            SmPtr->Cbf  = 0;                     // set SM idle/free !!!
            Cbf(pDDB, EDD_STS_OK);                      // return to the caller
        }
        else
        {
            SmPtr->Cbf  = 0;                     // set SM idle/free !!!
        }
    }

    return (ResponseCmd);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTRcvAcwDGStateMachine                           +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE                   pDDB         +*/
/*+                      LSA_BOOL                                  ExecuteOff   +*/
/*+                      EDDP_CRT_CBF_PTR_TYPE                     Cbf          +*/
/*+                                                                             +*/
/*+  Result         :    EDD_RSP                                                +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  ExecuteOff     : EDDP_CRT_SET_RCV_ACWDG_OFF or EDDP_CRT_SET_RCV_ACWDG_ON   +*/
/*+  Cbf            : Callback to caller                                        +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: activate/passivate consumer pack groups.                      +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTRcvAcwDGStateMachine(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_BOOL                    ExecuteOff,
    EDDP_CRT_CBF_PTR_TYPE       Cbf)
{
    EDD_RSP                                             ResponseCmd;
    LSA_UINT16                                          AcwContainerIdx, DgIdx, CurrContainerIdx;
    LSA_UINT16                                          PosNr, SubIdx, SubEnd, FrameID;
    LSA_UINT32                                          NextOffs_0, NextOffs_1; 
    LSA_UINT16                                          NextIdx;
    EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE                     pPDIRFrameData;
    EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_PTR_TYPE    pSubFrameBlock;
    EDDP_CRT_PACK_SM_PTR_TYPE                           SmPtr;
    EDDP_CRT_CONSUMER_CONTAINER_PTR_TYPE                pConsContainerEntry;
    EDDP_CRT_CONSUMER_DG_PTR_TYPE                       pConsDgEntry;

    SmPtr = &pDDB->CRT.Cons.CrtPackSm;

    if (Cbf != 0)
    {   // ***** initialization of SM
        SmPtr->ExecuteOff                    = ExecuteOff;                      // store inparam from caller
        SmPtr->Smf                           = EDDP_CRTRcvAcwDGStateMachine;    // bind right SM

        EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTRcvAcwDGStateMachine(Cbf=0x%X, ExecuteOff=%d, Smf=0x%X)",
            Cbf, ExecuteOff, SmPtr->Smf);

        EDDP_ASSERT_FALSE( ! (0 == SmPtr->Cbf) );         // expect SM idle/free !!!
        SmPtr->Cbf                          = Cbf;                              // store CBF from caller
        SmPtr->CurrIdx                      = 0;                                // index (used as index into PRM-array for Outbound ON)
        SmPtr->SM.RcvDg.State               = EDDP_CRT_RCVDG_SM_IDLE;           // start state
        SmPtr->SM.RcvDg.CurrContainerIdx    = 0;                                // start index

        if (ExecuteOff)
        {   // ***** RcvDg OFF
            SmPtr->CurrEndCnt   = pDDB->CRT.CfgParam.ConsumerContainerMaxNumber;
        }
        else
        {   // ***** RcvDg ON
            SmPtr->pPDIRData            = pDDB->PRM.pPDIRData_A;                // used for RcvDg ON
            SmPtr->pPDIRSubFrameData    = pDDB->PRM.pPDIRSubframeData_A;        // used for RcvDg ON
            SmPtr->CurrEndCnt           = SmPtr->pPDIRSubFrameData->NumberOfSubframeBlocks;    // step through all PRM items,  using OutBoundIdx for indexing the pack-grp
        }
    }
    else
    {   // ***** go on with SM into while loop !!!
        EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTRcvAcwDGStateMachine(): go on with: CurrIdx=%d, CurrContainerIdx=%d, State=%d, Smf=0x%X",
            SmPtr->CurrIdx, SmPtr->SM.RcvDg.CurrContainerIdx, SmPtr->SM.RcvDg.State, SmPtr->Smf);
    }

    pConsContainerEntry = pDDB->CRT.Cons.pConsContainerEntry;
    pConsDgEntry        = pDDB->CRT.Cons.pConsDgEntry;
    ResponseCmd         = EDD_STS_OK;

    while ((SmPtr->CurrIdx < SmPtr->CurrEndCnt) && (ResponseCmd == EDD_STS_OK))
    {
        if (SmPtr->ExecuteOff)
        {
            /* ------------------------------------ */
            /* RcvDg OFF                            */
            /* ------------------------------------ */
            if (pConsContainerEntry[SmPtr->CurrIdx].DgCount)
            {
                AcwContainerIdx = pDDB->CRT.CfgParam.ConsumerMaxNumber;   // user configured numbers of consumers
                AcwContainerIdx += SmPtr->CurrIdx;

                switch (SmPtr->SM.RcvDg.State)
                {
                    case EDDP_CRT_RCVDG_SM_IDLE:
                    {
                        // ***** Container ACW_OFF: as asynchronous cmd !!!!
                        EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTRcvAcwDGStateMachine(): ACW_OFF, CurrIdx=%d, State=%d, Smf=0x%X",
                            SmPtr->CurrIdx, SmPtr->SM.RcvDg.State, SmPtr->Smf);

                        ResponseCmd = EDDP_CRTConsumerDoAsynAcwOnOffCmd (pDDB, AcwContainerIdx, LSA_FALSE, _EDDP_CRTCmdSmCbfCpm);
                        EDDP_ASSERT_FALSE(ResponseCmd != EDD_STS_OK_PENDING);

                        SmPtr->SM.RcvDg.State = EDDP_CRT_RCVDG_SM_ACWOFF;
                    }
                    break;

                    case EDDP_CRT_RCVDG_SM_ACWOFF:
                    {
                        // ***** Container ACW_OUT: as asynchronous cmd !!!!
                        EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTRcvAcwDGStateMachine(): ACW_OUT, CurrIdx=%d, State=%d, Smf=0x%X",
                            SmPtr->CurrIdx, SmPtr->SM.RcvDg.State, SmPtr->Smf);

                        ResponseCmd = EDDP_CRTConsumerDoAsynAcwOutCmd (pDDB, AcwContainerIdx, _EDDP_CRTCmdSmCbfCpm);
                        EDDP_ASSERT_FALSE(ResponseCmd != EDD_STS_OK_PENDING);

                        SmPtr->SM.RcvDg.State = EDDP_CRT_RCVDG_SM_ACWOUT;
                    }
                    break;

                    case EDDP_CRT_RCVDG_SM_ACWOUT:
                    {
                        // ***** release connected DGs
                        while(pConsContainerEntry[SmPtr->CurrIdx].DgCount)
                        {   // ***** try to remove first DG from list and decrease DgCount 
                            DgIdx = EDDP_CRTConsumerRemoveFirstDgItem (pDDB, &pConsContainerEntry[SmPtr->CurrIdx]);

                            if (DgIdx != EDDP_CRT_CHAIN_IDX_END)
                            {   // ***** EDDP_CRTConsumerRemoveFirstDgItem() has removed a DG item
                                EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTRcvAcwDGStateMachine(): ACW_OUT, CurrIdx=%d/DgIdx=%d, Smf=0x%X",
                                    SmPtr->CurrIdx, DgIdx, SmPtr->Smf);

                                pConsDgEntry[DgIdx].ConnectedConsumerId = EDDP_CRT_CHAIN_IDX_END;   // set link invalid
                                EDDP_CRTConsDeInitConsAcwDgHwApi (pDDB, DgIdx);
                            }
                        }

                        // ***** release Container ACW
                        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTRcvAcwDGStateMachine(): release AcwContainerIdx=%d, Smf=0x%X",
                            SmPtr->CurrIdx, SmPtr->Smf);

                        EDDP_CRTConsDeInitContainerAcwHwApi(pDDB, AcwContainerIdx);
                        pDDB->CRT.Cons.RBaseHandler.FctRemoveAcwFromRBase(pDDB, AcwContainerIdx);

                        SmPtr->CurrIdx++;   // next index
                        SmPtr->SM.RcvDg.State = EDDP_CRT_RCVDG_SM_IDLE;
                    }
                    break;
                    // case EDDP_CRT_RCVDG_SM_ACWIN:
                    // case EDDP_CRT_RCVDG_SM_ACWON:

                    default:
                    {
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTRcvAcwDGStateMachine(): FATAL -> unknown SM.RcvDg.State=%d",
                            SmPtr->SM.RcvDg.State);
                        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
                    }
                    break;
                }
            }        
            else
            {
                SmPtr->CurrIdx++;                           // next index
            }
        }
        else
        {
            /* ------------------------------------ */
            /* RcvDg ON                             */
            /* ------------------------------------ */
            pSubFrameBlock = &SmPtr->pPDIRSubFrameData->SubframeBlockArray[SmPtr->CurrIdx];

            if (pSubFrameBlock->HwConfig & EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_RCV_ACW_MASK)
            {   // ***** block is relevant for RCV
                if (pSubFrameBlock->HwConfig & EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_RCV_ACW_RED1_MASK)
                {   // ***** Note: this flag is never expected here !!!
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTRcvAcwDGStateMachine(): FATAL -> this flag of HwConfig(RCV_ACW_RED1_MASK) is never expected here! HwConfig=0x%X",
                        pSubFrameBlock->HwConfig);
                    EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
                }

                FrameID             = pSubFrameBlock->FrameID;
                CurrContainerIdx    = SmPtr->SM.RcvDg.CurrContainerIdx;
                AcwContainerIdx     = pDDB->CRT.CfgParam.ConsumerMaxNumber;   // user configured numbers of consumers
                AcwContainerIdx    += SmPtr->SM.RcvDg.CurrContainerIdx;

                switch (SmPtr->SM.RcvDg.State)
                {
                    case EDDP_CRT_RCVDG_SM_IDLE:
                    {
                        // ***** check PRM and build PackRAM + start ACWIN-cmd
                        PosNr = (LSA_UINT8)pSubFrameBlock->SFIOCRProperties.DFPmode;
                        EDDP_ASSERT_FALSE(pSubFrameBlock->PDIRFrameDataConsIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
                        // ***** information for complete frame in: pPDIRFrameData
                        pPDIRFrameData  = &SmPtr->pPDIRData->pIRFrameDataArray[pSubFrameBlock->PDIRFrameDataConsIdx];
                        DgIdx           = EDDP_CRT_CHAIN_IDX_END;

                        if (PosNr == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_DFPMODE_END_NODE)
                        {   // ***** this is Controller/MultiDevice
                            SubEnd = pSubFrameBlock->NumberOfSubframeData;

                            EDDP_CRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTRcvAcwDGStateMachine(): Controller/MultiDevice: FrameID=0x%X, CurrIdx=%d, NumberOfSubframeData=%d, DFPmode=%d, Smf=0x%X",
                                pSubFrameBlock->FrameID, SmPtr->CurrIdx, pSubFrameBlock->NumberOfSubframeData, pSubFrameBlock->SFIOCRProperties.DFPmode, SmPtr->Smf);

                            for ( SubIdx=0; SubIdx < SubEnd; SubIdx++ )
                            {   // ***** use all DGs
                                DgIdx = EDDP_CRTConsumerGetFreeDgItem(pDDB);
                                EDDP_ASSERT_FALSE(DgIdx == EDDP_CRT_CHAIN_IDX_END);
                                pConsDgEntry[DgIdx].pSubFrameData = &pSubFrameBlock->SubframeDataArray[SubIdx]; 
                                EDDP_CRTConsumerAppendDgItem (pDDB, &pConsContainerEntry[CurrContainerIdx], DgIdx);
                                // do not delete the empty spaces in this trace text
                                EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--]    DG-Params: DgIdx=%d, DG-PosT=%d, DG-Len=%d, DG-Offset=%d", 
                                    DgIdx, 
                                    pSubFrameBlock->SubframeDataArray[SubIdx].Position, 
                                    pSubFrameBlock->SubframeDataArray[SubIdx].DataLength, 
                                    pSubFrameBlock->SubframeDataArray[SubIdx].Offset);
                            }
                        }
                        else
                        {   // ***** this is Device
                            DgIdx = EDDP_CRTConsumerGetFreeDgItem(pDDB);
                            EDDP_ASSERT_FALSE(DgIdx == EDDP_CRT_CHAIN_IDX_END);

                            SubEnd = pSubFrameBlock->NumberOfSubframeData;
                            pConsDgEntry[DgIdx].pSubFrameData = LSA_NULL;

                            EDDP_CRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTRcvAcwDGStateMachine(): Device: FrameID=0x%X, CurrIdx=%d, NumberOfSubframeData=%d, DFPmode=%d, Smf=0x%X",
                                pSubFrameBlock->FrameID, SmPtr->CurrIdx, pSubFrameBlock->NumberOfSubframeData, pSubFrameBlock->SFIOCRProperties.DFPmode, SmPtr->Smf);

                            for ( SubIdx=0; SubIdx < SubEnd; SubIdx++ )
                            {   // ***** use own DG only
                                if (pSubFrameBlock->SubframeDataArray[SubIdx].Position == PosNr)
                                {   // ***** store pointer to DG parameter
                                    pConsDgEntry[DgIdx].pSubFrameData = &pSubFrameBlock->SubframeDataArray[SubIdx]; 
                                    // do not delete the empty spaces in this trace text
                                    EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--]    DG-Params: DgIdx=%d, DG-PosT=%d, DG-Len=%d, DG-Offset=%d", 
                                        DgIdx, 
                                        pSubFrameBlock->SubframeDataArray[SubIdx].Position, 
                                        pSubFrameBlock->SubframeDataArray[SubIdx].DataLength, 
                                        pSubFrameBlock->SubframeDataArray[SubIdx].Offset);
                                    break;
                                }
                            }
                            EDDP_ASSERT_NO_NULL_PTR(pConsDgEntry[DgIdx].pSubFrameData);
                            EDDP_CRTConsumerAppendDgItem(pDDB, &pConsContainerEntry[CurrContainerIdx], DgIdx);
                        }

                        EDDP_ASSERT_FALSE(pConsContainerEntry[CurrContainerIdx].DgCount == 0);

                        // ***** set pointer to PRM parameter 
                        pConsContainerEntry[CurrContainerIdx].ConsAcwTypeInfo.FrameID = FrameID;
                        pConsContainerEntry[CurrContainerIdx].pPDIRFrameData = pPDIRFrameData;
                        if (pSubFrameBlock->SFIOCRProperties.SFCRC16)
                        {   pConsContainerEntry[CurrContainerIdx].HasChksum = LSA_TRUE;     }
                        else
                        {   pConsContainerEntry[CurrContainerIdx].HasChksum = LSA_FALSE;    }

                        // ***** set redundant flag for this container ACW if MRDP enebled --> check PDIRFrameDataConsIdxRed1
                        if (pSubFrameBlock->PDIRFrameDataConsIdxRed1 != EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
                        {   pConsContainerEntry[CurrContainerIdx].IsRedundant = LSA_TRUE;   }
                        else
                        {   pConsContainerEntry[CurrContainerIdx].IsRedundant = LSA_FALSE;  }
                        // ***** ACW.DgOffset from:    0=first DG(UP), 1=last DG(DOWN)
                        pConsContainerEntry[CurrContainerIdx].Acw_DG_Order = pSubFrameBlock->SFIOCRProperties.DFPRedundantPathLayout;

                        // ***** build DG linkage
                        //       starting with BeginIdx and follow the DgLink.Next
                        // example:       ACW
                        //                .ACW_DG_order = 0/1
                        //                .ACW_DG_offset      ACW_DG1           ACW_DG2           ACW_DG3
                        //                  |     +----a----> .NextOffs_0 ----> .NextOffs_0 ----> .NextOffs_0 = 0xFFFF
                        //                  +----------b----> .NextOffs_1 <---- .NextOffs_1 <---- .NextOffs_1 <--+
                        //                                       +-----------------------------------------------+
                        // link-a: ACW.DG_order=0 || FrameID.0=0
                        // link-b: ACW.DG_order=1 && FrameID.0=1
                        NextIdx = pConsContainerEntry[CurrContainerIdx].ConsDgList.EndIdx;

                        if (NextIdx != EDDP_CRT_CHAIN_IDX_END)
                        {
                            NextOffs_1  = EDDP_CRTGetPnipAddrShift(pDDB, pConsDgEntry[NextIdx].pHwAcwDgApi, EDDP_CRT_SHIFT_VALUE_3);    // Address must be 64Bit aligned!
                            DgIdx       = pConsContainerEntry[CurrContainerIdx].ConsDgList.BeginIdx;

                            while (DgIdx != EDDP_CRT_CHAIN_IDX_END)
                            {
                                NextIdx = pConsDgEntry[DgIdx].DgLink.Next;
                                if (NextIdx == EDDP_CRT_CHAIN_IDX_END)
                                {   // ***** end of DG list
                                    NextOffs_0 = PNIP_R2_ACWDG_RCV_WORD1_ACW_DG_NEXT_OFFSET_INIT_VALUE;
                                }
                                else
                                {   // ***** NextOffs_0 = Offset >> 3
                                    NextOffs_0 = EDDP_CRTGetPnipAddrShift(pDDB, pConsDgEntry[NextIdx].pHwAcwDgApi, EDDP_CRT_SHIFT_VALUE_3); // Address must be 64Bit aligned!
                                }
                                // ***** basic initialization only !!!
                                pDDB->CRT.Cons.AcwHandler.FctBasicInitAcwDgHwApi(pDDB, DgIdx, NextOffs_0, NextOffs_1);

                                EDDP_CRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTRcvAcwDGStateMachine(): CurrIdx=%d, DgIdx=%d, Next0=0x%X, Next1=0x%X, Smf=0x%X",
                                    SmPtr->CurrIdx, DgIdx, NextOffs_0, NextOffs_1, SmPtr->Smf);

                                // ***** NextOffs_1 = Offset >> 3
                                NextOffs_1 = EDDP_CRTGetPnipAddrShift(pDDB, pConsDgEntry[DgIdx].pHwAcwDgApi, EDDP_CRT_SHIFT_VALUE_3);       // Address must be 64Bit aligned!
                                DgIdx = NextIdx;

                            }
                        }
                        else
                        {
                            EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "[H:--] EDDP_CRTRcvAcwDGStateMachine(): missing ACW_DGs, CurrIdx=%d, AcwContainerIdx=%d, Smf=0x%X",
                                SmPtr->CurrIdx, AcwContainerIdx, SmPtr->Smf);
                        }
                        // ***** Container ACW_IN: as asynchronous CMD !!!
                        //       Note: The ACW.DgOffset will be set in EDDP_CRTConsumerDoAsynAcwInCmd() !!!
                        EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTRcvAcwDGStateMachine(): ACW_IN, CurrIdx=%d, AcwContainerIdx=%d, Smf=0x%X",
                            SmPtr->CurrIdx, AcwContainerIdx, SmPtr->Smf);

                        ResponseCmd = EDDP_CRTConsumerDoAsynAcwInCmd(pDDB, AcwContainerIdx, _EDDP_CRTCmdSmCbfCpm);
                        EDDP_ASSERT_FALSE(ResponseCmd != EDD_STS_OK_PENDING);

                        SmPtr->SM.RcvDg.State = EDDP_CRT_RCVDG_SM_ACWIN;
                    }
                    break;

                    case EDDP_CRT_RCVDG_SM_ACWIN:
                    {
                        // ***** Container ACW_ON: as asynchronous CMD !!!
                        EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTRcvAcwDGStateMachine(): ACW_ON, CurrIdx=%d, AcwContainerIdx=%d, Smf=0x%X",
                            SmPtr->CurrIdx, AcwContainerIdx, SmPtr->Smf);

                        ResponseCmd = EDDP_CRTConsumerDoAsynAcwOnOffCmd(pDDB, AcwContainerIdx, LSA_TRUE, _EDDP_CRTCmdSmCbfCpm);
                        EDDP_ASSERT_FALSE(ResponseCmd != EDD_STS_OK_PENDING);

                        SmPtr->SM.RcvDg.State = EDDP_CRT_RCVDG_SM_ACWON;
                    }
                    break;

                    case EDDP_CRT_RCVDG_SM_ACWON:
                    {
                        SmPtr->CurrIdx++;                           // next index
                        SmPtr->SM.RcvDg.CurrContainerIdx++;
                        SmPtr->SM.RcvDg.State = EDDP_CRT_RCVDG_SM_IDLE;

                        EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTRcvAcwDGStateMachine(): ACW_ON returned, CurrIdx=%d, AcwContainerIdx=%d, Smf=0x%X",
                            SmPtr->CurrIdx, AcwContainerIdx, SmPtr->Smf);
                    }
                    break;

                    // case EDDP_CRT_RCVDG_SM_ACWOFF:
                    // case EDDP_CRT_RCVDG_SM_ACWOUT:
                    default:
                    {
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTRcvAcwDGStateMachine(): FATAL -> unknown SM.RcvDg.State=%d",
                            SmPtr->SM.RcvDg.State);
                        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
                    }
                    break;
                }
            }
            else
            {   // ***** increment CurrIdx only !!!
                SmPtr->CurrIdx++;                           // next index
                SmPtr->SM.RcvDg.State = EDDP_CRT_RCVDG_SM_IDLE;
            }
        }
        
    }

    // ***** is SM completed?
    if (SmPtr->CurrIdx >= SmPtr->CurrEndCnt)
    {
        EDDP_ASSERT_FALSE(ResponseCmd != EDD_STS_OK);         // expect EDD_STS_OK on completion

        if (Cbf == 0)
        {   // ***** CBF allowed only if SM is called by own CBF --> with parameter Cbf = LSA_NULL !!!
            EDDP_ASSERT_NO_NULL_FCT_PTR(SmPtr->Cbf);           // expect valid cbf-pointer
            Cbf         = SmPtr->Cbf;                   // temporary store for clearing SmPtr->Cbf before calling the CBF
            SmPtr->Cbf  = 0;                     // set SM idle/free !!!
            Cbf(pDDB, EDD_STS_OK);                      // return to the caller
        }
        else
        {
            SmPtr->Cbf = 0;                      // set SM idle/free !!!
        }
    }

    return (ResponseCmd);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTLateErrorStartLSATimer                              +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Start the LSA-Timer for Inbound-LateError.                                +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTLateErrorStartLSATimer(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDDP_CRT_INBOUND_MNGM_PTR_TYPE      pInboundMgmt;

    pInboundMgmt = &pDDB->CRT.Inbound;

    EDDP_CoreTimerStart (pInboundMgmt->LateErrLSATimer, EDDP_CRT_INBOUND_LATE_ERR_TIME);
    pInboundMgmt->isLateErrLSATimerRunning = LSA_TRUE;
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTLateErrorStopLSATimer                               +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Stop the LSA-Timer for Inbound-LateError.                                 +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTLateErrorStopLSATimer(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDDP_CRT_INBOUND_MNGM_PTR_TYPE      pInboundMgmt;

    pInboundMgmt = &pDDB->CRT.Inbound;

    EDDP_CoreTimerStop (pInboundMgmt->LateErrLSATimer);
    pInboundMgmt->isLateErrLSATimerRunning = LSA_FALSE;
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTLateErrorTimerStartPackGrp                          +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                             LSA_UINT8                   PackGrpId                       +*/
/*+                             LSA_UINT8                   TimerType                       +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                                         +*/
/*+  PackGrpId  : InBound-PackGroup                                                         +*/
/*+  TimerType  : TimerType                                                                 +*/
/*+                 - TIMER_TYPE_INVALID                                                    +*/
/*+                 - TIMER_TYPE_PACKGRP_ACTIVATE                                           +*/
/*+                 - TIMER_TYPE_PACKGRP_LATE_ERROR                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Start the LateError timer (TimeTicks) of given InBound-PackGroup.         +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTLateErrorTimerStartPackGrp(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT8                   PackGrpId,
    LSA_UINT8                   TimerType)
{
    LSA_UINT32                              PackGrpMask;
    EDDP_CRT_INBOUND_MNGM_PTR_TYPE      pInboundMgmt;

    EDDP_ASSERT_FALSE(PackGrpId >= EDDP_CRT_MAX_PACK_FRAMES_ERTEC200P);

    pInboundMgmt    = &pDDB->CRT.Inbound;
    PackGrpMask     = 1;
    EDDP_SHIFT_VALUE_LEFT(PackGrpMask, PackGrpId);

    EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTLateErrorTimerStartPackGrp(PackGrpId=%d, TimerType=0x%X, 1s-Timer): TimerUsedMask=0x%X, isLSATimerRunning=0x%X", 
        PackGrpId, TimerType, pInboundMgmt->LateErrPackGrpTimerUsedMask, pInboundMgmt->isLateErrLSATimerRunning);

    // Timer is used by given PackGroup
    EDDP_SET_BIT_VALUE8(pInboundMgmt->LateErrPackGrpTimerUsedMask, 1, PackGrpMask, PackGrpId);
    pInboundMgmt->InboundGrps[PackGrpId].LateErrDiag.TimerType  = TimerType;
    pInboundMgmt->InboundGrps[PackGrpId].LateErrDiag.TimeTicks  = 0;

    // Start the LSA-Timer for Inbound if it is not yet running
    if (!(pInboundMgmt->isLateErrLSATimerRunning))
    {
        EDDP_CRTLateErrorStartLSATimer (pDDB);
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTLateErrorTimerStopPackGrp                           +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                             LSA_UINT8                   PackGrpId                       +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                                         +*/
/*+  PackGrpId  : InBound-PackGroup                                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Stop the LateError timer (TimeTicks) of given InBound-PackGroup.          +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTLateErrorTimerStopPackGrp(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT8                   PackGrpId)
{
    LSA_UINT32                              PackGrpMask;
    EDDP_CRT_INBOUND_MNGM_PTR_TYPE      pInboundMgmt;

    EDDP_ASSERT_FALSE(PackGrpId >= EDDP_CRT_MAX_PACK_FRAMES_ERTEC200P);

    pInboundMgmt    = &pDDB->CRT.Inbound;
    PackGrpMask     = 1;
    EDDP_SHIFT_VALUE_LEFT(PackGrpMask, PackGrpId);

    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTLateErrorTimerStopPackGrp(PackGrpId=%d): TimerUsedMask=0x%X, isLSATimerRunning=0x%X", 
        PackGrpId, pInboundMgmt->LateErrPackGrpTimerUsedMask, pInboundMgmt->isLateErrLSATimerRunning);

    // Timer is no more used by given PackGroup
    EDDP_SET_BIT_VALUE8(pInboundMgmt->LateErrPackGrpTimerUsedMask, 0, PackGrpMask, PackGrpId);
    pInboundMgmt->InboundGrps[PackGrpId].LateErrDiag.TimerType  = EDDP_CRT_INBOUND_LATE_ERR_TIMER_TYPE_INVALID;
    pInboundMgmt->InboundGrps[PackGrpId].LateErrDiag.TimeTicks  = 0;

    // Stop the LSA-Timer for Inbound if it is running and if no PackGrp has it in use
    if ((pInboundMgmt->isLateErrLSATimerRunning) && (pInboundMgmt->LateErrPackGrpTimerUsedMask == 0))
    {
        EDDP_CRTLateErrorStopLSATimer (pDDB);
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTLateErrorTimerPassed                                +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: This function is called by TimerRequest if the "Inbound-LateErrLSATimer"  +*/
/*+               has passed.                                                               +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTLateErrorTimerPassed(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT32                          i;
    LSA_UINT8                           MaxPackGrp;
    LSA_UINT32                          RegInBoundLateErrMask, RegInBoundLateErrStatus;
    LSA_UINT32                          PackGrpLoopMask;
    EDDP_CRT_INBOUND_MNGM_PTR_TYPE      pInboundMgmt;

    pInboundMgmt    = &pDDB->CRT.Inbound;
    PackGrpLoopMask = 1;
    MaxPackGrp      = 0;

    // at least one PackGrp timer must be in use
    EDDP_ASSERT_FALSE(pInboundMgmt->LateErrPackGrpTimerUsedMask == 0);

    /* -------------------------------------------------------------------------------------------- */
    /* Init MaxPackGrp                                                                              */
    /* -------------------------------------------------------------------------------------------- */
    switch (pDDB->Hardware)
    {
        case EDDP_HW_ERTEC200P:
        case EDDP_HW_ERTEC200P_FPGA:
        case EDDP_HW_HERA_FPGA:    
        {
            MaxPackGrp = EDDP_CRT_MAX_PACK_FRAMES_ERTEC200P;
        }
        break;

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTLateErrorTimerPassed(): FATAL -> Hardware(0x%X) is not supported!", 
                pDDB->Hardware);
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }

    /* ---------------------------------------------------------------- */
    /* Look for PackGrp which timer is used                             */
    /* ---------------------------------------------------------------- */
    for ( i=0; i < MaxPackGrp; i++ )
    {
        if (pInboundMgmt->LateErrPackGrpTimerUsedMask & PackGrpLoopMask)
        {
            // this PackGrp must be connected to PRM -> PDIRData.PDIRFrameData[x]
            EDDP_ASSERT_FALSE(pInboundMgmt->InboundGrps[i].LateErrDiag.PrmPDIRFrameDataIdx == EDDP_CRT_CHAIN_IDX_END);

            // count the TimeTicks of this PackGrp
            pInboundMgmt->InboundGrps[i].LateErrDiag.TimeTicks++;

            // is maximum of TimeTicks reached ?
            if (pInboundMgmt->InboundGrps[i].LateErrDiag.TimeTicks >= EDDP_CRT_INBOUND_LATE_ERR_MAX_TIME_TICKS)
            {
                // timer of this PackGrp has passed
                // read InBound-LateError register
                RegInBoundLateErrMask   = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_INBOUND_LATE_ERR_MASK);
                RegInBoundLateErrStatus = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_INBOUND_LATE_ERR_STATUS);

//              pDDB = pDDB->pIF[pInboundMgmt->InboundGrps[i].IfIdx];

                EDDP_CRT_TRACE_07(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTLateErrorTimerPassed(PackGrpId=%d, TimerType=0x%X): RegLateErrMask=0x%X, RegLateErrStatus=0x%X, InboundUsedMask=0x%X, InboundActiveMask=0x%X, isLSATimerRunning=0x%X", 
                    i, 
                    pInboundMgmt->InboundGrps[i].LateErrDiag.TimerType, 
                    RegInBoundLateErrMask, 
                    RegInBoundLateErrStatus, 
                    pInboundMgmt->InboundUsedMask[0],
                    pInboundMgmt->InboundActiveMask, 
                    pInboundMgmt->isLateErrLSATimerRunning);

                // which TimerType has this PackGrp ?
                switch (pInboundMgmt->InboundGrps[i].LateErrDiag.TimerType)
                {
                    /* -------------------------------------------------------- */
                    /* Timer was started from PackGrp Activate                  */
                    /* -------------------------------------------------------- */
                    case EDDP_CRT_INBOUND_LATE_ERR_TIMER_TYPE_PACKGRP_ACTIVATE:
                    {
                        // stop timer of this PackGrp
                        EDDP_CRTLateErrorTimerStopPackGrp (pDDB, (LSA_UINT8)i);
                        // delete this PackGrp in register "Late_Err_Status"
                        EDDP_CRTLateErrorDeleteLateErrStatus (pDDB, (LSA_UINT8)i);
                        // enable interrupt "ISR_InBound_Late_ERR" of this PackGrp
                        EDDP_CRTLateErrorSetupLateErrMask (pDDB, (LSA_UINT8)i, LSA_FALSE);
                    }
                    break;

                    /* -------------------------------------------------------- */
                    /* Timer was started from LateError interrupt               */
                    /* -------------------------------------------------------- */
                    case EDDP_CRT_INBOUND_LATE_ERR_TIMER_TYPE_PACKGRP_LATE_ERROR:
                    {
                        // check whether the InBound-LateError is still present
                        if (RegInBoundLateErrStatus & PackGrpLoopMask)
                        {
                            // InBound-LateError is still present
                            // delete this PackGrp in register "Late_Err_Status"
                            EDDP_CRTLateErrorDeleteLateErrStatus (pDDB, (LSA_UINT8)i);
                            // and start timer of this PackGrp new
                            EDDP_CRTLateErrorTimerStartPackGrp (pDDB, (LSA_UINT8)i, EDDP_CRT_INBOUND_LATE_ERR_TIMER_TYPE_PACKGRP_LATE_ERROR);
                        }
                        else
                        {
                            // InBound-LateError is no more present
                            // stop timer of this PackGrp
                            EDDP_CRTLateErrorTimerStopPackGrp (pDDB, (LSA_UINT8)i);
                            // enable interrupt "ISR_InBound_Late_ERR" of this PackGrp
                            EDDP_CRTLateErrorSetupLateErrMask (pDDB, (LSA_UINT8)i, LSA_FALSE);
                            // trigger LateError diagnosis (DisAppear)
                            EDDP_PRMDiagTriggerLateErrorDiagSingle (pDDB, pInboundMgmt->InboundGrps[i].LateErrDiag.PrmPDIRFrameDataIdx, LSA_FALSE, LSA_FALSE, LSA_FALSE);
                        }
                    }
                    break;

                    /* -------------------------------------------------------- */
                    /* TimerType is unknown                                     */
                    /* -------------------------------------------------------- */
                    default:
                    {
                        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTLateErrorTimerPassed(PackGrpId=%d): FATAL -> TimerType(0x%X) is unknown!", 
                            i, pInboundMgmt->InboundGrps[i].LateErrDiag.TimerType);
                        EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                    }
                    break;
                }
            }
        }

        // next PackGrp in loop
        EDDP_SHIFT_VALUE_LEFT(PackGrpLoopMask, 1);
    }
}






/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTLateErrorSetupLateErrMask                           +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                             LSA_UINT8                   PackGrpId                       +*/
/*+                             LSA_BOOL                    isMask                          +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                                         +*/
/*+  PackGrpId  : InBound-PackGroup                                                         +*/
/*+  isMask     : Mask or Demask the interrupt "ISR_InBound_Late_ERR"                       +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Enable/Disable the ISR_InBound_Late_ERR interrupt of given PackGrp.       +*/
/*+               Mask/Demask the bit in register "Late_Err_Mask".                          +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTLateErrorSetupLateErrMask(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT8                   PackGrpId,
    LSA_BOOL                    isMask)
{
    LSA_UINT32      RegValTemp;
    LSA_UINT32      PackGrpMask;
    LSA_UINT8       BitValTemp;

    EDDP_ASSERT_FALSE(PackGrpId >= EDDP_CRT_MAX_PACK_FRAMES_ERTEC200P);

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTLateErrorSetupLateErrMask(PackGrpId=%d, isMask=0x%X)", 
        PackGrpId, isMask);

    if (isMask)
        BitValTemp = 1;
    else
        BitValTemp = 0;

    PackGrpMask = 1;
    EDDP_SHIFT_VALUE_LEFT(PackGrpMask, PackGrpId);

    RegValTemp = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_INBOUND_LATE_ERR_MASK);
    EDDP_SET_BIT_VALUE32(RegValTemp, BitValTemp, PackGrpMask, PackGrpId);
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_INBOUND_LATE_ERR_MASK, RegValTemp);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTLateErrorDeleteLateErrStatus                        +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                             LSA_UINT8                   PackGrpId                       +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                                         +*/
/*+  PackGrpId  : InBound-PackGroup                                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Delete the bit in register "Late_Err_Status" of given PackGrp.            +*/
/*+               Delete = write "1".                                                       +*/
/*+                      = acknowledge Late_Err_Status                                      +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTLateErrorDeleteLateErrStatus(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT8                   PackGrpId)
{
    LSA_UINT32      RegValTemp;
    LSA_UINT32      PackGrpMask;

    EDDP_ASSERT_FALSE(PackGrpId >= EDDP_CRT_MAX_PACK_FRAMES_ERTEC200P);

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTLateErrorDeleteLateErrStatus(PackGrpId=%d)", 
        PackGrpId);

    PackGrpMask = 1;
    EDDP_SHIFT_VALUE_LEFT(PackGrpMask, PackGrpId);

    RegValTemp = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_INBOUND_LATE_ERR_STATUS);
    EDDP_SET_BIT_VALUE32(RegValTemp, 1, PackGrpMask, PackGrpId);
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_INBOUND_LATE_ERR_STATUS, RegValTemp);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTLateErrorTriggerAppearDiagAllActiveInBound          +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                             LSA_BOOL                    isPrmCommit                     +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                     +*/
/*+  isPrmCommit    : LSA_TRUE  : the call is from PrmCommit                                +*/
/*+                 : LSA_FALSE : the call is not from PrmCommit                            +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Trigger LateError diagnosis (Appear) for all active InBound pack groups   +*/
/*+               that have LateError current.                                              +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTLateErrorTriggerAppearDiagAllActiveInBound(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_BOOL                    isPrmCommit)
{
    LSA_UINT32                          i;
    LSA_UINT8                           MaxPackGrp;
    LSA_UINT32                          RegInBoundLateErrMask, RegInBoundLateErrStatus;
    LSA_UINT32                          PackGrpLoopMask;
    EDDP_CRT_INBOUND_MNGM_PTR_TYPE      pInboundMgmt;

    pInboundMgmt    = &pDDB->CRT.Inbound;
    PackGrpLoopMask = 1;
    MaxPackGrp      = 0;

    // only if some active PackGrp are present
    if (pInboundMgmt->InboundActiveMask != 0)
    {
        // read InBound-LateError register
        RegInBoundLateErrMask   = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_INBOUND_LATE_ERR_MASK);
        RegInBoundLateErrStatus = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_INBOUND_LATE_ERR_STATUS);

        EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTLateErrorTriggerAppearDiagAllActiveInBound(isPrmCommit=0x%X): InboundActiveMask=0x%X, RegLateErrMask=0x%X, RegLateErrStatus=0x%X", 
            isPrmCommit, pInboundMgmt->InboundActiveMask, RegInBoundLateErrMask, RegInBoundLateErrStatus);

        // is some LateError present ?
        if (RegInBoundLateErrStatus != 0)
        {
            /* ------------------------------------------------ */
            /* Init MaxPackGrp                                  */
            /* ------------------------------------------------ */
            switch (pDDB->Hardware)
            {
                case EDDP_HW_ERTEC200P:
                case EDDP_HW_ERTEC200P_FPGA:
                case EDDP_HW_HERA_FPGA:    
                {
                    MaxPackGrp = EDDP_CRT_MAX_PACK_FRAMES_ERTEC200P;
                }
                break;

                default:
                {
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTLateErrorTriggerAppearDiagAllActiveInBound(): FATAL -> Hardware(0x%X) is not supported!", 
                        pDDB->Hardware);
                    EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                }
                break;
            }

            /* ------------------------------------------------ */
            /* Look for PackGrp that:                           */
            /*  - is active                                     */
            /*  - has InBound-LateError                         */
            /*  - LateError interrupt is enable or PrmCommit    */
            /* ------------------------------------------------ */
            for ( i=0; i < MaxPackGrp; i++ )
            {
                if (   (pInboundMgmt->InboundActiveMask & PackGrpLoopMask)              /* PackGrp is active */
                    && (RegInBoundLateErrStatus & PackGrpLoopMask)                      /* Late_Err_Status is set */
                    && (!(RegInBoundLateErrMask & PackGrpLoopMask) || (isPrmCommit))    /* Late_Err_Mask is not set or is PrmCommit */
                   )
                {
                    // this PackGrp must be connected to PRM -> PDIRData.PDIRFrameData[x]
                    EDDP_ASSERT_FALSE(pInboundMgmt->InboundGrps[i].LateErrDiag.PrmPDIRFrameDataIdx == EDDP_CRT_CHAIN_IDX_END);

//                  pDDB = pDDB->pIF[pInboundMgmt->InboundGrps[i].IfIdx];

                    // trigger LateError diagnosis (Appear)
                    EDDP_PRMDiagTriggerLateErrorDiagSingle (pDDB, pInboundMgmt->InboundGrps[i].LateErrDiag.PrmPDIRFrameDataIdx, LSA_TRUE, LSA_FALSE, isPrmCommit);
                }

                // next PackGrp in loop
                EDDP_SHIFT_VALUE_LEFT(PackGrpLoopMask, 1);
            }
        }
    }
    else
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTLateErrorTriggerAppearDiagAllActiveInBound(isPrmCommit=0x%X): no active PackGrp are present", 
            isPrmCommit);
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTWrkarPackGrpDisableTimerPassed                      +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: This function is called by TimerRequest if the                            +*/
/*+               "WrkarPackGrpDisable-LSATimer" has passed.                                +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTWrkarPackGrpDisableTimerPassed(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT32          iTimerUser;
    LSA_UINT32          PortID, HwPortID;
    LSA_UINT32          reg_val, reg_address;

    EDDP_ASSERT_FALSE( EDDP_HAL_HW_PNIP_REV1 != EDDP_HALGetPnipRevision(pDDB) );
    // at least one entry in TimerUser[x] must be in use
    EDDP_ASSERT_FALSE(pDDB->CRT.WrkarPackGrpDisable.TimerUserCnt == 0);

    /* ---------------------------------------------------------------- */
    /* Look for entries in TimerUser[x] which timer is used currently   */
    /* ---------------------------------------------------------------- */
    for ( iTimerUser=0; iTimerUser < (EDDP_CRT_WRKAR_PACKGRP_DISABLE_TIMER_USER_MAX_NUMBER); iTimerUser++ )
    {
        if (pDDB->CRT.WrkarPackGrpDisable.TimerUser[iTimerUser].isTimerUsed)
        {
            // count the TimeTicks of this timer user
            pDDB->CRT.WrkarPackGrpDisable.TimerUser[iTimerUser].TimeTicks++;

            // is maximum of TimeTicks reached ?
            if (pDDB->CRT.WrkarPackGrpDisable.TimerUser[iTimerUser].TimeTicks >= EDDP_CRT_WRKAR_PACKGRP_DISABLE_MAX_TIME_TICKS)
            {
                EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTWrkarPackGrpDisableTimerPassed(TimerUserIdx=%d): TimerUserCnt=%d, isRTC3PSMNewParamPresentMask=0x%X", 
                    iTimerUser, pDDB->CRT.WrkarPackGrpDisable.TimerUserCnt, pDDB->SWI.isRTC3PSMNewParamPresentMask);

                // which TimerUser[x] has passed ?
                switch (iTimerUser)
                {
                    /* -------------------------------------------------------- */
                    /* Timer was started from RTC3PSM for PortID-1/2            */
                    /* -------------------------------------------------------- */
                    case EDDP_CRT_WRKAR_PACKGRP_DISABLE_TIMER_USER_IDX_RTC3PSM_P1:
                    case EDDP_CRT_WRKAR_PACKGRP_DISABLE_TIMER_USER_IDX_RTC3PSM_P2:
                    {
                        if (iTimerUser == EDDP_CRT_WRKAR_PACKGRP_DISABLE_TIMER_USER_IDX_RTC3PSM_P1)
                            PortID = 1;
                        else
                            PortID = 2;

                        HwPortID    = EDDP_SWI_get_HwPortID (pDDB, PortID);
                        reg_val     = pDDB->CRT.WrkarPackGrpDisable.TimerUser[iTimerUser].RegValuePhaseEnable;

                        reg_address = (HwPortID==1 ? PNIP_REG_PHASEENABLE_P1 : 
                                      (HwPortID==2 ? PNIP_REG_PHASEENABLE_P2 : 0));

                        // IMPORTANT - The passivate order of RED-Phase in HW
                        //      1) set Rx/Tx-Boundary-DP2 active
                        EDDP_CRTSetRxTxDP2BoundaryAndInbound (pDDB, HwPortID, LSA_TRUE);
                        //      2) setup DomainBoundaryPort for "Store-and-Forward"
                        EDDP_SWI_Setup_DomainBoundaryPort (pDDB, PortID, LSA_FALSE);
                        //      3) passivate the RED-Phase
                        EDDP_HAL_REG32_WRITE(pDDB, reg_address, reg_val);

                        // notify user of port status changes
                        EDDP_GENIndicateLinkChange_Ext (pDDB, PortID);
                        // delete "isRTC3PSMRunningAsynMask"
                        EDDP_SWI_Setup_RTC3PSMRunningAsynMask (pDDB, PortID, LSA_FALSE);
                        // stop timer of this timer user
                        EDDP_CRTWrkarPackGrpDisableTimerStop (pDDB, (LSA_UINT16)iTimerUser);

                        // are new RTC3PSM parameters present ?
                        if (EDDP_SWI_IsSet_RTC3PSMNewParamPresentMask(pDDB, PortID))
                        {
                            // trigger RTC3 port state machine again
                            EDDP_SWI_Trigger_RTC3_PortState_Rev1 (pDDB, PortID);
                        }
                        else
                        {
                            // call RQB-Handler if new RQBs are present in queue
                            EDDP_GlbCrtRQBHandler (pDDB);
                        }
                    }
                    break;

                    /* -------------------------------------------------------- */
                    /* Timer was started from CRT-Passivate-Pack state machine  */
                    /* -------------------------------------------------------- */
                    case EDDP_CRT_WRKAR_PACKGRP_DISABLE_TIMER_USER_IDX_CRT_PASSIVE_PACK_SM:
                    {
                        // stop timer of this timer user
                        EDDP_CRTWrkarPackGrpDisableTimerStop (pDDB, (LSA_UINT16)iTimerUser);
                        // go back to state machine
                        EDDP_CRTPassivatePackStateMachineCbf (pDDB, EDD_STS_OK);
                    }
                    break;

                    /* -------------------------------------------------------- */
                    /* TimerUser[x] is unknown                                  */
                    /* -------------------------------------------------------- */
                    default:
                    {
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTWrkarPackGrpDisableTimerPassed(): FATAL -> TimerUserIdx(%d) is unknown!", 
                            iTimerUser);
                        EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                    }
                    break;
                }
            }
        }
    }
}
#endif





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTWrkarPackGrpDisableStartLSATimer                    +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Start the LSA-Timer for workaround "PackGrp Disable".                     +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTWrkarPackGrpDisableStartLSATimer(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDDP_ASSERT_FALSE( EDDP_HAL_HW_PNIP_REV1 != EDDP_HALGetPnipRevision(pDDB) );

    EDDP_CoreTimerStart (pDDB->CRT.WrkarPackGrpDisable.LSATimer, EDDP_CRT_WRKAR_PACKGRP_DISABLE_TIME);
    pDDB->CRT.WrkarPackGrpDisable.isLSATimerRunning = LSA_TRUE;
}
#endif





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTWrkarPackGrpDisableStopLSATimer                     +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Stop the LSA-Timer for workaround "PackGrp Disable".                      +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTWrkarPackGrpDisableStopLSATimer(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDDP_ASSERT_FALSE( EDDP_HAL_HW_PNIP_REV1 != EDDP_HALGetPnipRevision(pDDB) );

    EDDP_CoreTimerStop (pDDB->CRT.WrkarPackGrpDisable.LSATimer);
    pDDB->CRT.WrkarPackGrpDisable.isLSATimerRunning = LSA_FALSE;
}
#endif





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTWrkarPackGrpDisableTimerStart                       +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                             LSA_UINT16                  TimerUserIdx                    +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                     +*/
/*+  TimerUserIdx   : Index in array TimerUser[x]                                           +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Start the Timer for workaround "PackGrp Disable" of given timer user.     +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTWrkarPackGrpDisableTimerStart(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  TimerUserIdx)
{
    EDDP_ASSERT_FALSE( EDDP_HAL_HW_PNIP_REV1 != EDDP_HALGetPnipRevision(pDDB) );
    EDDP_ASSERT_FALSE(TimerUserIdx >= EDDP_CRT_WRKAR_PACKGRP_DISABLE_TIMER_USER_MAX_NUMBER);
    EDDP_ASSERT_FALSE(pDDB->CRT.WrkarPackGrpDisable.TimerUser[TimerUserIdx].isTimerUsed);

    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTWrkarPackGrpDisableTimerStart(TimerUserIdx=%d): isLSATimerRunning=0x%X, TimerUserCnt=%d", 
        TimerUserIdx, pDDB->CRT.WrkarPackGrpDisable.isLSATimerRunning, pDDB->CRT.WrkarPackGrpDisable.TimerUserCnt);

    // Timer is used by given timer user
    pDDB->CRT.WrkarPackGrpDisable.TimerUser[TimerUserIdx].isTimerUsed   = LSA_TRUE;
    pDDB->CRT.WrkarPackGrpDisable.TimerUser[TimerUserIdx].TimeTicks     = 0;
    pDDB->CRT.WrkarPackGrpDisable.TimerUserCnt++;

    // Start the LSA-Timer for "PackGrp Disable" if it is not yet running
    if (!(pDDB->CRT.WrkarPackGrpDisable.isLSATimerRunning))
    {
        EDDP_CRTWrkarPackGrpDisableStartLSATimer (pDDB);
    }
}
#endif





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTWrkarPackGrpDisableTimerStop                        +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                             LSA_UINT16                  TimerUserIdx                    +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                     +*/
/*+  TimerUserIdx   : Index in array TimerUser[x]                                           +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Stop the Timer for workaround "PackGrp Disable" of given timer user.      +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTWrkarPackGrpDisableTimerStop(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  TimerUserIdx)
{
    EDDP_ASSERT_FALSE( EDDP_HAL_HW_PNIP_REV1 != EDDP_HALGetPnipRevision(pDDB) );
    EDDP_ASSERT_FALSE(TimerUserIdx >= EDDP_CRT_WRKAR_PACKGRP_DISABLE_TIMER_USER_MAX_NUMBER);
    EDDP_ASSERT_FALSE(!(pDDB->CRT.WrkarPackGrpDisable.TimerUser[TimerUserIdx].isTimerUsed));

    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTWrkarPackGrpDisableTimerStop(TimerUserIdx=%d): isLSATimerRunning=0x%X, TimerUserCnt=%d", 
        TimerUserIdx, pDDB->CRT.WrkarPackGrpDisable.isLSATimerRunning, pDDB->CRT.WrkarPackGrpDisable.TimerUserCnt);

    // Timer is no more used by given timer user
    pDDB->CRT.WrkarPackGrpDisable.TimerUser[TimerUserIdx].isTimerUsed   = LSA_FALSE;
    pDDB->CRT.WrkarPackGrpDisable.TimerUser[TimerUserIdx].TimeTicks     = 0;
    EDDP_ASSERT_FALSE(pDDB->CRT.WrkarPackGrpDisable.TimerUserCnt == 0);
    pDDB->CRT.WrkarPackGrpDisable.TimerUserCnt--;

    // Stop the LSA-Timer for "PackGrp Disable" if it is running and if no user has it in use
    if ((pDDB->CRT.WrkarPackGrpDisable.isLSATimerRunning) && (pDDB->CRT.WrkarPackGrpDisable.TimerUserCnt == 0))
    {
        EDDP_CRTWrkarPackGrpDisableStopLSATimer (pDDB);
    }
}
#endif
