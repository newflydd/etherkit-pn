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
/*  F i l e               &F: eddp_prm_diag.c                           :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP PRM (Diagnose) handling                     */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  20.10.11    VS    initial version.                                       */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID   95
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_PRM_DIAG */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"            /* edd headerfiles */
#include "eddp_int.h"            /* internal header */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_PRMDiagIndicateFinish                                  +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                             LSA_UINT16                  PortID                          +*/
/*+                             LSA_UINT32                  IndDiagEntryCnt                 +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  PortID             : PortID to indicate for (0=interface, 1..EDDP_MAX_PORT_CNT)        +*/
/*+  IndDiagEntryCnt    : number of entries in indication RQB                               +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+  Description: This function indicates one single indication RQB of PRM.                 +*/
/*+                                                                                         +*/
/*+               Check if indication queue contains resource.                              +*/
/*+               If yes, check if an indications resource contains entries                 +*/
/*+               and if yes indicate it to user.                                           +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMDiagIndicateFinish(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PortID,
    LSA_UINT32                  IndDiagEntryCnt)
{
    EDD_RSP                                 Response;
    EDD_UPPER_RQB_PTR_TYPE                  pIndRQB;
    EDD_UPPER_PRM_INDICATION_PTR_TYPE       pIndParam;
    EDDP_DDB_PRM_PTR_TYPE                   pPrm;
    EDDP_LOCAL_HDB_PTR_TYPE                 pHDB;

    EDDP_ASSERT_FALSE(PortID > pDDB->SWI.PortCnt);
    EDDP_ASSERT_FALSE(IndDiagEntryCnt > EDD_SRV_PRM_INDICATION_DIAG_MAX_ENTRIES);

    pPrm        = &pDDB->PRM;
    Response    = EDDP_GlbHandleGetHDB (pPrm->EDD_Handle, &pHDB);
    EDDP_ASSERT_FALSE(Response != EDD_STS_OK);
    pIndRQB     = pPrm->PrmIndReq[PortID].pFirst;   // first RQB in indication queue of given PortID

    if (LSA_HOST_PTR_ARE_EQUAL(pIndRQB, LSA_NULL))
    {
        // no indication resource present
        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] EDDP_PRMDiagIndicateFinish(PortID=%d, IndDiagEntryCnt=%d): No indication resource present in PRM!", 
            PortID, IndDiagEntryCnt);
    }
    else
    {
        pIndParam = (EDD_UPPER_PRM_INDICATION_PTR_TYPE) pIndRQB->pParam;

        if (LSA_HOST_PTR_ARE_EQUAL(pIndParam, LSA_NULL))
        {
            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMDiagIndicateFinish(): FATAL -> pIndRQB->pParam=NULL!");
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        else
        {
            EDDP_PRM_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMDiagIndicateFinish(PortID=%d, IndDiagEntryCnt=%d): DoIndicate=0x%X, pIndRQB=0x%X, RqbQueue.Count=%d", 
                PortID, IndDiagEntryCnt, pPrm->DoIndicate[PortID], pIndRQB, pPrm->PrmIndReq[PortID].Count);

            /* ------------------------------------------------------------------------------------ */
            /* DoIndicate = TRUE    : indicate empty diagnosis or diagnosis after PrmCommit         */
            /* IndDiagEntryCnt > 0  : indicate diagnosis with diag parameters in diag[x] array      */
            /* ------------------------------------------------------------------------------------ */
            if ((pPrm->DoIndicate[PortID]) || (IndDiagEntryCnt > 0))
            {
                pIndRQB = EDDP_GlbRQBQueueRemoveFromBegin (&pPrm->PrmIndReq[PortID]);

                if (LSA_HOST_PTR_ARE_EQUAL(pIndRQB, LSA_NULL))
                {
                    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMDiagIndicateFinish(PortID=%d): FATAL -> pIndRQB=NULL!", PortID);
                    EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
                }

                // fill diag parameter in indication resource
                pIndParam->edd_port_id      = PortID; 
                pIndParam->diag_cnt         = (LSA_UINT16) IndDiagEntryCnt;
                pPrm->DoIndicate[PortID]    = LSA_FALSE;

                EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] EDDP_PRMDiagIndicateFinish(PortID=%d, IndDiagEntryCnt=%d): Indicate - new RqbQueue.Count=%d",
                    PortID, IndDiagEntryCnt, pPrm->PrmIndReq[PortID].Count);

                // finish this indication RQB
                EDDP_GlbRequestFinish (pHDB, pIndRQB, EDD_STS_OK);
            }  
        }
    }
}





/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_PRMDiagIndicateDiagOnePort                             +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                             LSA_UINT16                  PortID                          +*/
/*+                             LSA_BOOL                    isPrmCommit                     +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                     +*/
/*+  PortID         : PortID to indicate for (0=interface, 1..EDDP_MAX_PORT_CNT)            +*/
/*+  isPrmCommit    : LSA_TRUE  : the call is from PrmCommit                                +*/
/*+                 : LSA_FALSE : the call is not from PrmCommit                            +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Called to check if a Diag has to be indicated for PortID                  +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMDiagIndicateDiagOnePort(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PortID,
    LSA_BOOL                    isPrmCommit)
{
    LSA_UINT32                              IndDiagEntryCnt;
    EDD_UPPER_RQB_PTR_TYPE                  pIndRQB;
    EDD_UPPER_PRM_INDICATION_PTR_TYPE       pIndParam;
    EDDP_DDB_PRM_PTR_TYPE                   pPrm;

    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_PRMDiagIndicateDiagOnePort()");

    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMDiagIndicateDiagOnePort(PortID=%d, isPrmCommit=0x%X)", 
        PortID, isPrmCommit);

    EDDP_ASSERT_FALSE(PortID > pDDB->SWI.PortCnt);

    IndDiagEntryCnt = 0;
    pPrm            = &pDDB->PRM;
    pIndRQB         = pPrm->PrmIndReq[PortID].pFirst;

    // is Indication Resource present ?
    if ( !(LSA_HOST_PTR_ARE_EQUAL(pIndRQB, LSA_NULL)) )
    { 
        pIndParam = (EDD_UPPER_PRM_INDICATION_PTR_TYPE) pIndRQB->pParam;

        switch (PortID)
        {
            /* ---------------------------------------- */
            /* Interface                                */
            /* ---------------------------------------- */
            case 0:
            {
                if (isPrmCommit)
                {
                    // for PrmCommit: the current counter is PrmCommitIndDiagEntryCntIF
                    IndDiagEntryCnt = pDDB->PRM.PrmCommitIndDiagEntryCntIF;
                }

                // Check PDNC (frame drop)
                if (pPrm->FrameDrop.ErrorStatus != pPrm->FrameDrop.ErrorStatusIndicated)
                {
                    if (pPrm->FrameDrop.ErrorStatusIndicated != EDDP_DIAG_FRAME_DROP_OK)
                    {
                        /* DisAppear */
                        EDDP_PRMDiagSetupPrmIndEntryFrameDrop (&pIndParam->diag[IndDiagEntryCnt], pPrm->FrameDrop.ErrorStatusIndicated, LSA_FALSE);
                        IndDiagEntryCnt++;
                    }

                    if (pPrm->FrameDrop.ErrorStatus != EDDP_DIAG_FRAME_DROP_OK)
                    {
                        /* Appear */
                        EDDP_PRMDiagSetupPrmIndEntryFrameDrop (&pIndParam->diag[IndDiagEntryCnt], pPrm->FrameDrop.ErrorStatus, LSA_TRUE);
                        IndDiagEntryCnt++;
                    }

                    pPrm->FrameDrop.ErrorStatusIndicated = pPrm->FrameDrop.ErrorStatus;
                }

                if (IndDiagEntryCnt > EDD_SRV_PRM_INDICATION_DIAG_MAX_ENTRIES)
                {
                    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMDiagIndicateDiagOnePort(PortID=%d): FATAL -> IndDiagEntryCnt(%d) > IndDiagEntryCntMax(%d)!", 
                        PortID, IndDiagEntryCnt, EDD_SRV_PRM_INDICATION_DIAG_MAX_ENTRIES);
                    EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
                }

                if (isPrmCommit)
                {
                    // only increase the number of entries for indication RQB
                    // it will be indicated after all PrmCommit diagnosis are set into indication RQB
                    pPrm->PrmCommitIndDiagEntryCntIF = IndDiagEntryCnt;
                }
                else
                {
                    // indicate the RQB if something is filled
                    EDDP_PRMDiagIndicateFinish (pDDB, PortID, IndDiagEntryCnt);
                }
            }
            break;

            /* ---------------------------------------- */
            /* Ports(1..PortCnt)                        */
            /* ---------------------------------------- */
            default:
            {
                // nothing to check - only indicate the RQB
                EDDP_PRMDiagIndicateFinish (pDDB, PortID, IndDiagEntryCnt);
            }
            break;
        }
    }
    else
    {
        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] EDDP_PRMDiagIndicateDiagOnePort(PortID=%d): No indication resource present in PRM!", 
            PortID);
    }

    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMDiagIndicateDiagOnePort()");
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_PRMDiagIndicateDiagAllAfterPrmCommit                   +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB                           +*/
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
/*+  Description: Indicate all PRM diagnosis after PrmCommit.                               +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMDiagIndicateDiagAllAfterPrmCommit(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT32      iPortID;

    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_PRMDiagIndicateDiagAllAfterPrmCommit()");

    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMDiagIndicateDiagAllAfterPrmCommit()");

    /* ------------------------------------------------------------------------------------------------------------ */
    /* PrmCommit diagnosis for Interface(0)                                                                         */
    /* -> first put all interface diagnosis (Appear) into one Ind-RQB and than indicate                             */
    /* ------------------------------------------------------------------------------------------------------------ */
    iPortID                                 = 0;
    pDDB->PRM.PrmCommitIndDiagEntryCntIF    = 0;
    /* -------------------------------------------------------------------------------- */
    /* Empty PDNC diagnosis                                                             */
    /* -------------------------------------------------------------------------------- */
    EDDP_PRMDiagIndicateDiagOnePort (pDDB, (LSA_UINT16)iPortID, LSA_TRUE);
    /* -------------------------------------------------------------------------------- */
    /* Trigger LateError diagnosis (Appear) for all active Inbound pack groups          */
    /* -------------------------------------------------------------------------------- */
    EDDP_CRTLateErrorTriggerAppearDiagAllActiveInBound (pDDB, LSA_TRUE);
    /* -------------------------------------------------------------------------------- */
    /* Trigger MRT diagnosis (Appear) for all active MRPD consumers                     */
    /* -------------------------------------------------------------------------------- */
    EDDP_CRTConsMrtTriggerAppearDiagAllActiveMRPDConsumer (pDDB, LSA_TRUE);
    /* -------------------------------------------------------------------------------- */
    /* Indicate the RQB for Interface(0)                                                */
    /* -------------------------------------------------------------------------------- */
    EDDP_ASSERT_FALSE(pDDB->PRM.PrmCommitIndDiagEntryCntIF > EDD_SRV_PRM_INDICATION_DIAG_MAX_ENTRIES);
    EDDP_PRMDiagIndicateFinish (pDDB, (LSA_UINT16)iPortID, pDDB->PRM.PrmCommitIndDiagEntryCntIF);

    /* ------------------------------------------------------------------------------------------------------------ */
    /* PrmCommit diagnosis for Ports(1..PortCnt) -> only empty PDNC diagnosis                                       */
    /* ------------------------------------------------------------------------------------------------------------ */
    for ( iPortID=1; iPortID <= pDDB->SWI.PortCnt; iPortID++ )
    {
        EDDP_PRMDiagIndicateDiagOnePort (pDDB, (LSA_UINT16)iPortID, LSA_TRUE);
    }

    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMDiagIndicateDiagAllAfterPrmCommit()");
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_PRMDiagSetupPrmIndEntryFrameDrop                       +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDD_SRV_PRM_IND_DATA_PTR_TYPE       pPrmIndData             +*/
/*+                             LSA_UINT32                          ErrorStatus             +*/
/*+                             LSA_BOOL                            isAppear                +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pPrmIndData    : Pointer to Diag-entry within  indication RQB                          +*/
/*+  ErrorStatus    : ErrorStatus to Setup diag entry for                                   +*/
/*+                     - EDD_DIAG_FRAME_DROP_MAINTENANCE_REQUIRED                          +*/
/*+                     - EDD_DIAG_FRAME_DROP_MAINTENANCE_DEMANDED                          +*/
/*+                     - EDD_DIAG_FRAME_DROP_ERROR                                         +*/
/*+  isAppear       : LSA_TRUE  : diagnosis is APPEAR    (error is comming)                 +*/
/*+                   LSA_FALSE : diagnosis is DISAPPEAR (error is going)                   +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Setup/Fill diag parameters within indication RQB structure for FrameDrop. +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMDiagSetupPrmIndEntryFrameDrop(
    EDD_SRV_PRM_IND_DATA_PTR_TYPE       pPrmIndData,
    LSA_UINT32                          ErrorStatus,
    LSA_BOOL                            isAppear)
{
    switch (ErrorStatus)
    {
        case EDDP_DIAG_FRAME_DROP_MAINTENANCE_REQUIRED:
            pPrmIndData->ChannelProperties = EDD_SRV_PRM_CH_PROP_MAIN_REQUIRED;
        break;

        case EDDP_DIAG_FRAME_DROP_MAINTENANCE_DEMANDED:
            pPrmIndData->ChannelProperties = EDD_SRV_PRM_CH_PROP_MAIN_DEMANDED;
        break;

        case EDDP_DIAG_FRAME_DROP_ERROR:
        default:
            pPrmIndData->ChannelProperties = 0; 
        break;
    }

    pPrmIndData->ChannelProperties     |= (isAppear ? EDD_SRV_PRM_CH_PROP_APPEARS : EDD_SRV_PRM_CH_PROP_DISAPPEARS);
    pPrmIndData->ChannelErrorType       = EDD_SRV_PRM_CHANNEL_ERROR_TYPE_NC_MISMATCH;
    pPrmIndData->ExtChannelErrorType    = EDD_SRV_PRM_EXT_CHANNEL_ERROR_TYPE_NC_FD_NR;
    pPrmIndData->ExtChannelAddValue     = EDD_SRV_PRM_EXT_CHANNEL_ADD_NONE;
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMDiagIndicateIRDataDiag                          +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                        +*/
/*+                             LSA_UINT16                  DiagType                    +*/
/*+                             LSA_UINT16                  FrameID                     +*/
/*+                             LSA_BOOL                    isAppear                    +*/
/*+                             LSA_BOOL                    isEmptyDiag                 +*/
/*+                             LSA_BOOL                    isPrmCommit                 +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_BOOL                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  DiagType       : DiagType                                                          +*/
/*+                     - EDDP_PRM_IRFRAMEDATA_DIAG_TYPE_MRT                            +*/
/*+                     - EDDP_PRM_IRFRAMEDATA_DIAG_TYPE_DFP_LATE_ERR                   +*/
/*+  FrameID        : FrameID for "ExtChannelErrorType"                                 +*/
/*+  isAppear       : LSA_TRUE  : diagnosis is APPEAR    (error is comming)             +*/
/*+                   LSA_FALSE : diagnosis is DISAPPEAR (error is going)               +*/
/*+  isEmptyDiag    : LSA_TRUE  : indicate empty diagnosis (diag_cnt = 0)               +*/
/*+                 : LSA_FALSE : indicate no empty diagnosis (diag_cnt != 0)           +*/
/*+  isPrmCommit    : LSA_TRUE  : the call is from PrmCommit                            +*/
/*+                 : LSA_FALSE : the call is not from PrmCommit                        +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function fills the diag parameters and indicates the given       +*/
/*+               IRData diagnosis in "DiagType".                                       +*/
/*+               The given FrameID will be filled in ExtChannelErrorType.              +*/
/*+                                                                                     +*/
/*+               Return LSA_TRUE  : indication resource is present in PRM              +*/
/*+                      LSA_FALSE : indication resource is not present in PRM          +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMDiagIndicateIRDataDiag(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  DiagType,
    LSA_UINT16                  FrameID,
    LSA_BOOL                    isAppear,
    LSA_BOOL                    isEmptyDiag,
    LSA_BOOL                    isPrmCommit)
{
    LSA_BOOL                                State;
    LSA_UINT16                              PortID;
    LSA_UINT32                              IndDiagEntryCnt;
    EDD_UPPER_RQB_PTR_TYPE                  pIndRQB;
    EDD_UPPER_PRM_INDICATION_PTR_TYPE       pIndParam;

    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_PRMDiagIndicateIRDataDiag()");

    EDDP_PRM_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMDiagIndicateIRDataDiag(DiagType=0x%X, FrameID=0x%X, isAppear=0x%X, isEmptyDiag=0x%X, isPrmCommit=0x%X)", 
        DiagType, FrameID, isAppear, isEmptyDiag, isPrmCommit);

    State           = LSA_TRUE;
    PortID          = 0;    /* Note: 0=Interface (IRDataDiag only on interface) */
    IndDiagEntryCnt = 0;
    pIndRQB         = pDDB->PRM.PrmIndReq[PortID].pFirst;

    if (isPrmCommit)
    {
        // for PrmCommit: the current counter is PrmCommitIndDiagEntryCntIF
        IndDiagEntryCnt = pDDB->PRM.PrmCommitIndDiagEntryCntIF;
    }

    if (LSA_HOST_PTR_ARE_EQUAL(pIndRQB, LSA_NULL))
    { 
        // no indication resource present
        State = LSA_FALSE;
        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] EDDP_PRMDiagIndicateIRDataDiag(): No indication resource present in PRM!");
    }
    else
    {
        if (isEmptyDiag)
        {
            // indicate empty diagnosis with (diag_cnt = 0)
            pDDB->PRM.DoIndicate[PortID] = LSA_TRUE;
        }
        else
        {
            pIndParam = (EDD_UPPER_PRM_INDICATION_PTR_TYPE) pIndRQB->pParam;

            // fill diag parameters
            pIndParam->diag[IndDiagEntryCnt].ChannelProperties  = EDD_SRV_PRM_CH_PROP_MAIN_DEMANDED;
            pIndParam->diag[IndDiagEntryCnt].ChannelProperties |= (isAppear ? EDD_SRV_PRM_CH_PROP_APPEARS : EDD_SRV_PRM_CH_PROP_DISAPPEARS);

            switch (DiagType)
            {
                case EDDP_PRM_IRFRAMEDATA_DIAG_TYPE_MRT:
                {
                    pIndParam->diag[IndDiagEntryCnt].ChannelErrorType       = EDD_SRV_PRM_CHANNEL_ERROR_TYPE_MRPD_MISMATCH;
                    pIndParam->diag[IndDiagEntryCnt].ExtChannelErrorType    = EDD_SRV_PRM_EXT_CHANNEL_ERROR_TYPE_MRPD_DV + FrameID;
                }
                break;

                case EDDP_PRM_IRFRAMEDATA_DIAG_TYPE_DFP_LATE_ERR:
                {
                    pIndParam->diag[IndDiagEntryCnt].ChannelErrorType       = EDD_SRV_PRM_CHANNEL_ERROR_TYPE_DFP_MISMATCH;
                    pIndParam->diag[IndDiagEntryCnt].ExtChannelErrorType    = EDD_SRV_PRM_EXT_CHANNEL_ERROR_TYPE_DFP_FLE + FrameID;
                }
                break;

                default:
                {
                    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMDiagIndicateIRDataDiag(): FATAL -> The given DiagType(0x%X) is unknown!", 
                        DiagType);
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }
                break;
            }

            pIndParam->diag[IndDiagEntryCnt].ExtChannelAddValue = EDD_SRV_PRM_EXT_CHANNEL_ADD_NONE;

            IndDiagEntryCnt++;

            if (IndDiagEntryCnt > EDD_SRV_PRM_INDICATION_DIAG_MAX_ENTRIES)
            {
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMDiagIndicateIRDataDiag(): FATAL -> IndDiagEntryCnt(%d) > IndDiagEntryCntMax(%d)!", 
                    IndDiagEntryCnt, EDD_SRV_PRM_INDICATION_DIAG_MAX_ENTRIES);
                EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
            }
        }

        if (isPrmCommit)
        {
            // only increase the number of entries for indication RQB
            // it will be indicated after all PrmCommit diagnosis are set into indication RQB
            pDDB->PRM.PrmCommitIndDiagEntryCntIF = IndDiagEntryCnt;
        }
        else
        {
            // indicate the RQB if something is filled
            EDDP_PRMDiagIndicateFinish (pDDB, PortID, IndDiagEntryCnt);
        }
    }

    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMDiagIndicateIRDataDiag(): State=0x%X", State);

    return (State);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_PRMDiagPDIRFrameDataAddToMrtDiagPendingList            +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             LSA_UINT16                          PDIRFrameDataIdx        +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  PDIRFrameDataIdx   : Index in PDIRFrameData[x]                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Add given PDIRFrameData entry to MrtDiagPendingList.                      +*/
/*+                                                                                         +*/
/*+               Put this new PDIRFrameData to end of MrtDiagPendingList.                  +*/
/*+                                                                                         +*/
/*+               Only if active PDIRData_A is present!                                     +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMDiagPDIRFrameDataAddToMrtDiagPendingList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PDIRFrameDataIdx)
{
    EDDP_PRM_PDIRDATA_PTR_TYPE                                  pPDIRData;
    EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE                             pPDIRFrameData;
    EDDP_PRM_PDIRFRAMEDATA_MRT_DIAG_PENDING_GROUP_PTR_TYPE      pMrtDiagPendingGroup;

    // is PDIRData_A present ?
    if (EDDP_PRMPDIRDataIsPDIRDataPresent(pDDB, LSA_TRUE))
    {
        EDDP_ASSERT_FALSE(PDIRFrameDataIdx >= pDDB->PRM.pPDIRData_A->IRFrameDataArray_Count_All);
        pPDIRData               = pDDB->PRM.pPDIRData_A;
        pPDIRFrameData          = &pDDB->PRM.pPDIRData_A->pIRFrameDataArray[PDIRFrameDataIdx];
        pMrtDiagPendingGroup    = &pDDB->PRM.MrtDiagPendingGroup;

        /* ------------------------------------------------------------ */
        /* Put this new PDIRFrameData to end of MrtDiagPendingList      */
        /* ------------------------------------------------------------ */
        if (pMrtDiagPendingGroup->List.BeginIdx == EDDP_PRM_CHAIN_IDX_END)
        {
            // this PDIRFrameData entry is the first entry in MrtDiagPendingList
            pMrtDiagPendingGroup->List.BeginIdx             = PDIRFrameDataIdx;
            pMrtDiagPendingGroup->List.EndIdx               = PDIRFrameDataIdx;
            pPDIRFrameData->MrtDiag.PendingGroupLink.Prev   = EDDP_PRM_CHAIN_IDX_END;
            pPDIRFrameData->MrtDiag.PendingGroupLink.Next   = EDDP_PRM_CHAIN_IDX_END;
        }
        else
        {
            // this PDIRFrameData entry is not the first entry in MrtDiagPendingList
            pPDIRFrameData->MrtDiag.PendingGroupLink.Prev                                                   = pMrtDiagPendingGroup->List.EndIdx;
            pPDIRFrameData->MrtDiag.PendingGroupLink.Next                                                   = EDDP_PRM_CHAIN_IDX_END;
            pPDIRData->pIRFrameDataArray[pMrtDiagPendingGroup->List.EndIdx].MrtDiag.PendingGroupLink.Next   = PDIRFrameDataIdx;
            pMrtDiagPendingGroup->List.EndIdx                                                               = PDIRFrameDataIdx;
        }

        pMrtDiagPendingGroup->PDIRFrameDataCnt++;
    }
    else
    {
        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMDiagPDIRFrameDataAddToMrtDiagPendingList(): FATAL -> The record PDIRData is not present!");
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_PRMDiagPDIRFrameDataRemoveFromMrtDiagPendingList       +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             LSA_UINT16                          PDIRFrameDataIdx        +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  PDIRFrameDataIdx   : Index in PDIRFrameData[x]                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Remove given PDIRFrameData entry from MrtDiagPendingList.                 +*/
/*+                                                                                         +*/
/*+               Only if active PDIRData_A is present!                                     +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMDiagPDIRFrameDataRemoveFromMrtDiagPendingList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PDIRFrameDataIdx)
{
    LSA_UINT16                                                  Prev;
    LSA_UINT16                                                  Next;
    EDDP_PRM_PDIRDATA_PTR_TYPE                                  pPDIRData;
    EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE                             pPDIRFrameData;
    EDDP_PRM_PDIRFRAMEDATA_MRT_DIAG_PENDING_GROUP_PTR_TYPE      pMrtDiagPendingGroup;

    // is PDIRData_A present ?
    if (EDDP_PRMPDIRDataIsPDIRDataPresent(pDDB, LSA_TRUE))
    {
        EDDP_ASSERT_FALSE(PDIRFrameDataIdx >= pDDB->PRM.pPDIRData_A->IRFrameDataArray_Count_All);
        pPDIRData               = pDDB->PRM.pPDIRData_A;
        pPDIRFrameData          = &pDDB->PRM.pPDIRData_A->pIRFrameDataArray[PDIRFrameDataIdx];
        pMrtDiagPendingGroup    = &pDDB->PRM.MrtDiagPendingGroup;
        Prev                    = pPDIRFrameData->MrtDiag.PendingGroupLink.Prev;
        Next                    = pPDIRFrameData->MrtDiag.PendingGroupLink.Next;

        /* ------------------------------------------------------------ */
        /* Remove this PDIRFrameData entry from MrtDiagPendingList      */
        /* ------------------------------------------------------------ */
        if ((Prev == EDDP_PRM_CHAIN_IDX_END) || (Next == EDDP_PRM_CHAIN_IDX_END))
        {
            if (Prev == EDDP_PRM_CHAIN_IDX_END)
            {
                // this PDIRFrameData entry is first entry in MrtDiagPendingList
                if (Next == EDDP_PRM_CHAIN_IDX_END)
                {
                    // first and only one PDIRFrameData entry in MrtDiagPendingList
                    pMrtDiagPendingGroup->List.BeginIdx = EDDP_PRM_CHAIN_IDX_END;
                    pMrtDiagPendingGroup->List.EndIdx   = EDDP_PRM_CHAIN_IDX_END;
                }
                else
                {
                    // first PDIRFrameData entry in MrtDiagPendingList but not only one
                    pMrtDiagPendingGroup->List.BeginIdx                                 = Next;
                    pPDIRData->pIRFrameDataArray[Next].MrtDiag.PendingGroupLink.Prev    = EDDP_PRM_CHAIN_IDX_END;
                }
            }
            else
            {
                // last PDIRFrameData entry in MrtDiagPendingList but not only one
                pPDIRData->pIRFrameDataArray[Prev].MrtDiag.PendingGroupLink.Next    = EDDP_PRM_CHAIN_IDX_END;
                pMrtDiagPendingGroup->List.EndIdx                                   = Prev;
            }
        }
        else
        {
            // this PDIRFrameData entry is in the middle of the MrtDiagPendingList
            pPDIRData->pIRFrameDataArray[Prev].MrtDiag.PendingGroupLink.Next    = Next;
            pPDIRData->pIRFrameDataArray[Next].MrtDiag.PendingGroupLink.Prev    = Prev;
        }

        EDDP_ASSERT_FALSE(pMrtDiagPendingGroup->PDIRFrameDataCnt == 0);
        pMrtDiagPendingGroup->PDIRFrameDataCnt--;
    }
    else
    {
        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMDiagPDIRFrameDataRemoveFromMrtDiagPendingList(): FATAL -> The record PDIRData is not present!");
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_PRMDiagPDIRFrameDataAddToLateErrDiagPendingList        +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                             LSA_UINT16                  PDIRFrameDataIdx                +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  PDIRFrameDataIdx   : Index in PDIRFrameData[x]                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Add given PDIRFrameData entry to LateErrDiagPendingList.                  +*/
/*+                                                                                         +*/
/*+               Put this new PDIRFrameData to end of LateErrDiagPendingList.              +*/
/*+                                                                                         +*/
/*+               Only if active PDIRSubframeData_A and PDIRData_A are present!             +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMDiagPDIRFrameDataAddToLateErrDiagPendingList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PDIRFrameDataIdx)
{
    EDDP_PRM_PDIRDATA_PTR_TYPE                                      pPDIRData;
    EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE                                 pPDIRFrameData;
    EDDP_PRM_PDIRFRAMEDATA_LATE_ERR_DIAG_PENDING_GROUP_PTR_TYPE     pLateErrDiagPendingGroup;

    // is DFP (PDIRSubframeData_A and PDIRData_A) present ?
    if (EDDP_PRMPDIRSubframeDataIsDfpPresent(pDDB, LSA_TRUE))
    {
        EDDP_ASSERT_FALSE(PDIRFrameDataIdx >= pDDB->PRM.pPDIRData_A->IRFrameDataArray_Count_All);
        pPDIRData                   = pDDB->PRM.pPDIRData_A;
        pPDIRFrameData              = &pDDB->PRM.pPDIRData_A->pIRFrameDataArray[PDIRFrameDataIdx];
        pLateErrDiagPendingGroup    = &pDDB->PRM.LateErrDiagPendingGroup;

        /* ------------------------------------------------------------ */
        /* Put this new PDIRFrameData to end of LateErrDiagPendingList  */
        /* ------------------------------------------------------------ */
        if (pLateErrDiagPendingGroup->List.BeginIdx == EDDP_PRM_CHAIN_IDX_END)
        {
            // this PDIRFrameData entry is the first entry in LateErrDiagPendingList
            pLateErrDiagPendingGroup->List.BeginIdx             = PDIRFrameDataIdx;
            pLateErrDiagPendingGroup->List.EndIdx               = PDIRFrameDataIdx;
            pPDIRFrameData->LateErrDiag.PendingGroupLink.Prev   = EDDP_PRM_CHAIN_IDX_END;
            pPDIRFrameData->LateErrDiag.PendingGroupLink.Next   = EDDP_PRM_CHAIN_IDX_END;
        }
        else
        {
            // this PDIRFrameData entry is not the first entry in LateErrDiagPendingList
            pPDIRFrameData->LateErrDiag.PendingGroupLink.Prev                                                       = pLateErrDiagPendingGroup->List.EndIdx;
            pPDIRFrameData->LateErrDiag.PendingGroupLink.Next                                                       = EDDP_PRM_CHAIN_IDX_END;
            pPDIRData->pIRFrameDataArray[pLateErrDiagPendingGroup->List.EndIdx].LateErrDiag.PendingGroupLink.Next   = PDIRFrameDataIdx;
            pLateErrDiagPendingGroup->List.EndIdx                                                                   = PDIRFrameDataIdx;
        }

        pLateErrDiagPendingGroup->PackGrpCnt++;
    }
    else
    {
        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMDiagPDIRFrameDataAddToLateErrDiagPendingList(): FATAL -> The record PDIRSubframeData or PDIRData is not present!");
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_PRMDiagPDIRFrameDataRemoveFromLateErrDiagPendingList   +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                             LSA_UINT16                  PDIRFrameDataIdx                +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  PDIRFrameDataIdx   : Index in PDIRFrameData[x]                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Remove given PDIRFrameData entry from LateErrDiagPendingList.             +*/
/*+                                                                                         +*/
/*+               Only if active PDIRSubframeData_A and PDIRData_A are present!             +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMDiagPDIRFrameDataRemoveFromLateErrDiagPendingList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PDIRFrameDataIdx)
{
    LSA_UINT16                                                      Prev;
    LSA_UINT16                                                      Next;
    EDDP_PRM_PDIRDATA_PTR_TYPE                                      pPDIRData;
    EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE                                 pPDIRFrameData;
    EDDP_PRM_PDIRFRAMEDATA_LATE_ERR_DIAG_PENDING_GROUP_PTR_TYPE     pLateErrDiagPendingGroup;

    // is DFP (PDIRSubframeData_A and PDIRData_A) present ?
    if (EDDP_PRMPDIRSubframeDataIsDfpPresent(pDDB, LSA_TRUE))
    {
        EDDP_ASSERT_FALSE(PDIRFrameDataIdx >= pDDB->PRM.pPDIRData_A->IRFrameDataArray_Count_All);
        pPDIRData                   = pDDB->PRM.pPDIRData_A;
        pPDIRFrameData              = &pDDB->PRM.pPDIRData_A->pIRFrameDataArray[PDIRFrameDataIdx];
        pLateErrDiagPendingGroup    = &pDDB->PRM.LateErrDiagPendingGroup;
        Prev                        = pPDIRFrameData->LateErrDiag.PendingGroupLink.Prev;
        Next                        = pPDIRFrameData->LateErrDiag.PendingGroupLink.Next;

        /* ------------------------------------------------------------ */
        /* Remove this PDIRFrameData entry from LateErrDiagPendingList  */
        /* ------------------------------------------------------------ */
        if ((Prev == EDDP_PRM_CHAIN_IDX_END) || (Next == EDDP_PRM_CHAIN_IDX_END))
        {
            if (Prev == EDDP_PRM_CHAIN_IDX_END)
            {
                // this PDIRFrameData entry is first entry in LateErrDiagPendingList
                if (Next == EDDP_PRM_CHAIN_IDX_END)
                {
                    // first and only one PDIRFrameData entry in LateErrDiagPendingList
                    pLateErrDiagPendingGroup->List.BeginIdx = EDDP_PRM_CHAIN_IDX_END;
                    pLateErrDiagPendingGroup->List.EndIdx   = EDDP_PRM_CHAIN_IDX_END;
                }
                else
                {
                    // first PDIRFrameData entry in LateErrDiagPendingList but not only one
                    pLateErrDiagPendingGroup->List.BeginIdx                                 = Next;
                    pPDIRData->pIRFrameDataArray[Next].LateErrDiag.PendingGroupLink.Prev    = EDDP_PRM_CHAIN_IDX_END;
                }
            }
            else
            {
                // last PDIRFrameData entry in LateErrDiagPendingList but not only one
                pPDIRData->pIRFrameDataArray[Prev].LateErrDiag.PendingGroupLink.Next    = EDDP_PRM_CHAIN_IDX_END;
                pLateErrDiagPendingGroup->List.EndIdx                                   = Prev;
            }
        }
        else
        {
            // this PDIRFrameData entry is in the middle of the LateErrDiagPendingList
            pPDIRData->pIRFrameDataArray[Prev].LateErrDiag.PendingGroupLink.Next    = Next;
            pPDIRData->pIRFrameDataArray[Next].LateErrDiag.PendingGroupLink.Prev    = Prev;
        }

        EDDP_ASSERT_FALSE(pLateErrDiagPendingGroup->PackGrpCnt == 0);
        pLateErrDiagPendingGroup->PackGrpCnt--;
    }
    else
    {
        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMDiagPDIRFrameDataRemoveFromLateErrDiagPendingList(): FATAL -> The record PDIRSubframeData or PDIRData is not present!");
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMDiagTriggerMRTDiagSingle                        +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                        +*/
/*+                             LSA_UINT16                  PDIRFrameDataIdx            +*/
/*+                             LSA_BOOL                    isAppear                    +*/
/*+                             LSA_BOOL                    isEmptyDiag                 +*/
/*+                             LSA_BOOL                    isPrmCommit                 +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  PDIRFrameDataIdx   : index in PDIRData.PDIRFrameData[x]                            +*/
/*+  isAppear           : LSA_TRUE  : MRT diagnosis is APPEAR    (error is comming)     +*/
/*+                       LSA_FALSE : MRT diagnosis is DISAPPEAR (error is going)       +*/
/*+  isEmptyDiag        : LSA_TRUE  : indicate empty diagnosis (diag_cnt = 0)           +*/
/*+                     : LSA_FALSE : indicate no empty diagnosis (diag_cnt != 0)       +*/
/*+  isPrmCommit        : LSA_TRUE  : the call is from PrmCommit                        +*/
/*+                     : LSA_FALSE : the call is not from PrmCommit                    +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function triggers the MRT diagnosis (Appear/DisAppear)           +*/
/*+               of given PDIRFrameData entry.                                         +*/
/*+                                                                                     +*/
/*+               PDIRFrameDataIdx must alway valid for active PDIRData_A!              +*/
/*+               FrameID of this PDIRFrameData entry must be MRPD FrameID!             +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMDiagTriggerMRTDiagSingle(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PDIRFrameDataIdx,
    LSA_BOOL                    isAppear,
    LSA_BOOL                    isEmptyDiag,
    LSA_BOOL                    isPrmCommit)
{
    LSA_BOOL                            IndResourcePresent;
    LSA_BOOL                            isLastFiredDiagAppear, isLastRequestedDiagByPendingAppear;
    EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE     pPDIRFrameData;

    // is PDIRData_A present ?
    if (EDDP_PRMPDIRDataIsPDIRDataPresent(pDDB, LSA_TRUE))
    {
        EDDP_ASSERT_FALSE(PDIRFrameDataIdx >= pDDB->PRM.pPDIRData_A->IRFrameDataArray_Count_All);
        pPDIRFrameData = &pDDB->PRM.pPDIRData_A->pIRFrameDataArray[PDIRFrameDataIdx];
        EDDP_ASSERT_FALSE(!(EDDP_IS_IRT_REDUNDANT_FRAMEID(pPDIRFrameData->FrameID)));

        // init isLastFiredDiagAppear
        if (pPDIRFrameData->MrtDiag.Flags & EDDP_PRM_PDIRFRAMEDATA_MRT_FLAGS_LAST_FIRED_DIAG)
            isLastFiredDiagAppear = LSA_TRUE;
        else
            isLastFiredDiagAppear = LSA_FALSE;

        EDDP_PRM_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMDiagTriggerMRTDiagSingle(PDIRFrameDataIdx=%d, isAppear=0x%X, isEmptyDiag=0x%X, isPrmCommit=0x%X): FrameID=0x%X, MrtDiag-Flags=0x%X", 
            PDIRFrameDataIdx, isAppear, isEmptyDiag, isPrmCommit, pPDIRFrameData->FrameID, pPDIRFrameData->MrtDiag.Flags);

        // fire MRT diagnosis only if MRPD_WATCHDOG_ACTIVE is ON
        if (pPDIRFrameData->MrtDiag.Flags & EDDP_PRM_PDIRFRAMEDATA_MRT_FLAGS_MRPD_WATCHDOG_ACTIVE)
        {
            // fire only if:
            if (   ((!isPrmCommit) && (isLastFiredDiagAppear != isAppear))  /* no PrmCommit and last state of fired diagnosis has changed */
                || ((isPrmCommit) && (isAppear))                            /* PrmCommit and diagnosis is APPEAR */
               )
            {
                IndResourcePresent = EDDP_PRMDiagIndicateIRDataDiag (pDDB, EDDP_PRM_IRFRAMEDATA_DIAG_TYPE_MRT, pPDIRFrameData->FrameID, isAppear, isEmptyDiag, isPrmCommit);

                if (IndResourcePresent)
                {
                    // store new state of fired diagnosis
                    if (isAppear)
                        pPDIRFrameData->MrtDiag.Flags |= EDDP_PRM_PDIRFRAMEDATA_MRT_FLAGS_LAST_FIRED_DIAG;      // Appear was fired
                    else
                        pPDIRFrameData->MrtDiag.Flags &= ~EDDP_PRM_PDIRFRAMEDATA_MRT_FLAGS_LAST_FIRED_DIAG;     // DisAppear was fired
                }
                else
                {
                    // no indication resource in PRM!
                    // wait until new resources are present -> check Pending in EDDP_PRMDiagTriggerMRTDiagAllPending()
                    pPDIRFrameData->MrtDiag.Flags |= EDDP_PRM_PDIRFRAMEDATA_MRT_FLAGS_INDICATION_IS_PENDING;
                    // and put this PDIRFrameData entry to MrtDiagPendingList
                    EDDP_PRMDiagPDIRFrameDataAddToMrtDiagPendingList (pDDB, PDIRFrameDataIdx);

                    // store new state of requested diagnosis by pending
                    if (isAppear)
                        pPDIRFrameData->MrtDiag.Flags |= EDDP_PRM_PDIRFRAMEDATA_MRT_FLAGS_LAST_REQUESTED_DIAG_BY_PENDING;   // Appear is requested
                    else
                        pPDIRFrameData->MrtDiag.Flags &= ~EDDP_PRM_PDIRFRAMEDATA_MRT_FLAGS_LAST_REQUESTED_DIAG_BY_PENDING;  // DisAppear is requested
                }
            }
            else
            {
                // this else is only for following cases: The state has changed for example 0->1 and 1->0 without firing of diagnosis, so the pending must be deleted here!
                // user does not see this changing: 0->1 and 1->0
                // is something pending for this PDIRFrameData entry ?
                if (pPDIRFrameData->MrtDiag.Flags & EDDP_PRM_PDIRFRAMEDATA_MRT_FLAGS_INDICATION_IS_PENDING)
                {
                    // init isLastRequestedDiagByPendingAppear
                    if (pPDIRFrameData->MrtDiag.Flags & EDDP_PRM_PDIRFRAMEDATA_MRT_FLAGS_LAST_REQUESTED_DIAG_BY_PENDING)
                        isLastRequestedDiagByPendingAppear = LSA_TRUE;
                    else
                        isLastRequestedDiagByPendingAppear = LSA_FALSE;

                    // compare given "isAppear" with last requested diagnosis by pending
                    if (isAppear != isLastRequestedDiagByPendingAppear)
                    {
                        // delete pending flag (this diagnosis is no more pending)
                        pPDIRFrameData->MrtDiag.Flags &= ~EDDP_PRM_PDIRFRAMEDATA_MRT_FLAGS_INDICATION_IS_PENDING;
                        // remove this PDIRFrameData entry from MrtDiagPendingList
                        EDDP_PRMDiagPDIRFrameDataRemoveFromMrtDiagPendingList (pDDB, PDIRFrameDataIdx);

                        EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMDiagTriggerMRTDiagSingle(): given PDIRFrameDataIdx is still pending but new isAppear(0x%X) != isLastRequestedDiagByPendingAppear(0x%X), so the pending flag is deleted: new MrtDiag-Flags=0x%X", 
                            isAppear, isLastRequestedDiagByPendingAppear, pPDIRFrameData->MrtDiag.Flags);
                    }
                }
            }
        }
    }
    else
    {
        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMDiagTriggerMRTDiagSingle(): FATAL -> The record PDIRData is not present!");
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMDiagTriggerMRTDiagAllPending                    +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                        +*/
/*+                             LSA_BOOL                    isPrmCommit                 +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  isPrmCommit    : LSA_TRUE  : the call is from PrmCommit                            +*/
/*+                 : LSA_FALSE : the call is not from PrmCommit                        +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function triggers the MRT diagnosis of all entries in            +*/
/*+               MrtDiagPendingList.                                                   +*/
/*+                                                                                     +*/
/*+               Only if active PDIRData_A is present!                                 +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMDiagTriggerMRTDiagAllPending(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_BOOL                    isPrmCommit)
{
    LSA_UINT16                                                  PDIRFrameDataIdx;
    LSA_BOOL                                                    isAppear;
    EDDP_PRM_PDIRDATA_PTR_TYPE                                  pPDIRData;
    EDDP_PRM_PDIRFRAMEDATA_MRT_DIAG_PENDING_GROUP_PTR_TYPE      pMrtDiagPendingGroup;

    // is PDIRData_A present ?
    if (EDDP_PRMPDIRDataIsPDIRDataPresent(pDDB, LSA_TRUE))
    {
        pPDIRData               = pDDB->PRM.pPDIRData_A;
        pMrtDiagPendingGroup    = &pDDB->PRM.MrtDiagPendingGroup;
        PDIRFrameDataIdx        = pMrtDiagPendingGroup->List.BeginIdx;

        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMDiagTriggerMRTDiagAllPending(isPrmCommit=0x%X): MrtDiagPendingGroupCnt(%d)", 
            isPrmCommit, pMrtDiagPendingGroup->PDIRFrameDataCnt);

        // loop through all entries of MrtDiagPendingList
        while (PDIRFrameDataIdx != EDDP_PRM_CHAIN_IDX_END)
        {
            EDDP_ASSERT_FALSE(PDIRFrameDataIdx >= pPDIRData->IRFrameDataArray_Count_All);

            // is something pending for this PDIRFrameData entry ?
            if (pPDIRData->pIRFrameDataArray[PDIRFrameDataIdx].MrtDiag.Flags & EDDP_PRM_PDIRFRAMEDATA_MRT_FLAGS_INDICATION_IS_PENDING)
            {
                // delete pending flag
                pPDIRData->pIRFrameDataArray[PDIRFrameDataIdx].MrtDiag.Flags &= ~EDDP_PRM_PDIRFRAMEDATA_MRT_FLAGS_INDICATION_IS_PENDING;
                // remove this PDIRFrameData entry from MrtDiagPendingList
                EDDP_PRMDiagPDIRFrameDataRemoveFromMrtDiagPendingList (pDDB, PDIRFrameDataIdx);

                // is Appear or DisAppear pending ?
                if (pPDIRData->pIRFrameDataArray[PDIRFrameDataIdx].MrtDiag.Flags & EDDP_PRM_PDIRFRAMEDATA_MRT_FLAGS_LAST_REQUESTED_DIAG_BY_PENDING)
                    isAppear = LSA_TRUE;
                else
                    isAppear = LSA_FALSE;

                // trigger MRT diagnosis for this PDIRFrameData entry
                EDDP_PRMDiagTriggerMRTDiagSingle (pDDB, PDIRFrameDataIdx, isAppear, LSA_FALSE, isPrmCommit);
            }

            // next PDIRFrameDataIdx in this MrtDiagPendingList
            PDIRFrameDataIdx = pPDIRData->pIRFrameDataArray[PDIRFrameDataIdx].MrtDiag.PendingGroupLink.Next;
        }
    }
    else
    {
        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMDiagTriggerMRTDiagAllPending(): PDIRData_A is not present");
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMDiagTriggerLateErrorDiagAllPending              +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                        +*/
/*+                             LSA_BOOL                    isPrmCommit                 +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  isPrmCommit    : LSA_TRUE  : the call is from PrmCommit                            +*/
/*+                 : LSA_FALSE : the call is not from PrmCommit                        +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function triggers the LateError diagnosis of all entries in      +*/
/*+               LateErrDiagPendingList.                                               +*/
/*+                                                                                     +*/
/*+               Only if active PDIRSubframeData_A and PDIRData_A are present!         +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMDiagTriggerLateErrorDiagAllPending(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_BOOL                    isPrmCommit)
{
    LSA_UINT16                                                      PDIRFrameDataIdx;
    LSA_BOOL                                                        isAppear;
    EDDP_PRM_PDIRDATA_PTR_TYPE                                      pPDIRData;
    EDDP_PRM_PDIRFRAMEDATA_LATE_ERR_DIAG_PENDING_GROUP_PTR_TYPE     pLateErrDiagPendingGroup;

    // is DFP (PDIRSubframeData_A and PDIRData_A) present ?
    if (EDDP_PRMPDIRSubframeDataIsDfpPresent(pDDB, LSA_TRUE))
    {
        pPDIRData                   = pDDB->PRM.pPDIRData_A;
        pLateErrDiagPendingGroup    = &pDDB->PRM.LateErrDiagPendingGroup;
        PDIRFrameDataIdx            = pLateErrDiagPendingGroup->List.BeginIdx;

        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMDiagTriggerLateErrorDiagAllPending(isPrmCommit=0x%X): LateErrDiagPendingGroupCnt(%d)", 
            isPrmCommit, pLateErrDiagPendingGroup->PackGrpCnt);

        // loop through all entries of LateErrDiagPendingList
        while (PDIRFrameDataIdx != EDDP_PRM_CHAIN_IDX_END)
        {
            EDDP_ASSERT_FALSE(PDIRFrameDataIdx >= pPDIRData->IRFrameDataArray_Count_All);

            // is something pending for this PDIRFrameData entry ?
            if (pPDIRData->pIRFrameDataArray[PDIRFrameDataIdx].LateErrDiag.Flags & EDDP_PRM_PDIRFRAMEDATA_LATE_ERR_FLAGS_INDICATION_IS_PENDING)
            {
                // delete pending flag
                pPDIRData->pIRFrameDataArray[PDIRFrameDataIdx].LateErrDiag.Flags &= ~EDDP_PRM_PDIRFRAMEDATA_LATE_ERR_FLAGS_INDICATION_IS_PENDING;
                // remove this PDIRFrameData entry from LateErrDiagPendingList
                EDDP_PRMDiagPDIRFrameDataRemoveFromLateErrDiagPendingList (pDDB, PDIRFrameDataIdx);

                // is Appear or DisAppear pending ?
                if (pPDIRData->pIRFrameDataArray[PDIRFrameDataIdx].LateErrDiag.Flags & EDDP_PRM_PDIRFRAMEDATA_LATE_ERR_FLAGS_LAST_REQUESTED_DIAG_BY_PENDING)
                    isAppear = LSA_TRUE;
                else
                    isAppear = LSA_FALSE;

                // trigger LateError diagnosis for this PDIRFrameData entry
                EDDP_PRMDiagTriggerLateErrorDiagSingle (pDDB, PDIRFrameDataIdx, isAppear, LSA_FALSE, isPrmCommit);
            }

            // next PDIRFrameDataIdx in this LateErrDiagPendingList
            PDIRFrameDataIdx = pPDIRData->pIRFrameDataArray[PDIRFrameDataIdx].LateErrDiag.PendingGroupLink.Next;
        }
    }
    else
    {
        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMDiagTriggerLateErrorDiagAllPending(): PDIRData_A or PDIRSubframeData_A is not present!");
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMDiagTriggerLateErrorDiagSingle                  +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                        +*/
/*+                             LSA_UINT16                  PDIRFrameDataIdx            +*/
/*+                             LSA_BOOL                    isAppear                    +*/
/*+                             LSA_BOOL                    isEmptyDiag                 +*/
/*+                             LSA_BOOL                    isPrmCommit                 +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  PDIRFrameDataIdx   : index in PDIRData.PDIRFrameData[x]                            +*/
/*+  isAppear           : LSA_TRUE  : LateError diagnosis is APPEAR    (error is comming)+*/
/*+                       LSA_FALSE : LateError diagnosis is DISAPPEAR (error is going) +*/
/*+  isEmptyDiag        : LSA_TRUE  : indicate empty diagnosis (diag_cnt = 0)           +*/
/*+                     : LSA_FALSE : indicate no empty diagnosis (diag_cnt != 0)       +*/
/*+  isPrmCommit        : LSA_TRUE  : the call is from PrmCommit                        +*/
/*+                     : LSA_FALSE : the call is not from PrmCommit                    +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function triggers the LateError diagnosis (Appear/DisAppear)     +*/
/*+               of given PDIRFrameData entry.                                         +*/
/*+                                                                                     +*/
/*+               PDIRFrameDataIdx must alway valid for active PDIRData_A!              +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMDiagTriggerLateErrorDiagSingle(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PDIRFrameDataIdx,
    LSA_BOOL                    isAppear,
    LSA_BOOL                    isEmptyDiag,
    LSA_BOOL                    isPrmCommit)
{
    LSA_BOOL                            IndResourcePresent;
    LSA_BOOL                            isLastFiredDiagAppear, isLastRequestedDiagByPendingAppear;
    EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE     pPDIRFrameData;

    // is DFP (PDIRSubframeData_A and PDIRData_A) present ?
    if (EDDP_PRMPDIRSubframeDataIsDfpPresent(pDDB, LSA_TRUE))
    {
        EDDP_ASSERT_FALSE(PDIRFrameDataIdx >= pDDB->PRM.pPDIRData_A->IRFrameDataArray_Count_All);
        pPDIRFrameData = &pDDB->PRM.pPDIRData_A->pIRFrameDataArray[PDIRFrameDataIdx];

        // init isLastFiredDiagAppear
        if (pPDIRFrameData->LateErrDiag.Flags & EDDP_PRM_PDIRFRAMEDATA_LATE_ERR_FLAGS_LAST_FIRED_DIAG)
            isLastFiredDiagAppear = LSA_TRUE;
        else
            isLastFiredDiagAppear = LSA_FALSE;

        EDDP_PRM_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMDiagTriggerLateErrorDiagSingle(PDIRFrameDataIdx=%d, isAppear=0x%X, isEmptyDiag=0x%X, isPrmCommit=0x%X): FrameID=0x%X, LateErrDiag-Flags=0x%X", 
            PDIRFrameDataIdx, isAppear, isEmptyDiag, isPrmCommit, pPDIRFrameData->FrameID, pPDIRFrameData->LateErrDiag.Flags);

        // fire only if:
        if (   ((!isPrmCommit) && (isLastFiredDiagAppear != isAppear))  /* no PrmCommit and last state of fired diagnosis has changed */
            || ((isPrmCommit) && (isAppear))                            /* PrmCommit and diagnosis is APPEAR */
           )
        {
            IndResourcePresent = EDDP_PRMDiagIndicateIRDataDiag (pDDB, EDDP_PRM_IRFRAMEDATA_DIAG_TYPE_DFP_LATE_ERR, pPDIRFrameData->FrameID, isAppear, isEmptyDiag, isPrmCommit);

            if (IndResourcePresent)
            {
                // store new state of fired diagnosis
                if (isAppear)
                    pPDIRFrameData->LateErrDiag.Flags |= EDDP_PRM_PDIRFRAMEDATA_LATE_ERR_FLAGS_LAST_FIRED_DIAG;    // Appear was fired
                else
                    pPDIRFrameData->LateErrDiag.Flags &= ~EDDP_PRM_PDIRFRAMEDATA_LATE_ERR_FLAGS_LAST_FIRED_DIAG;   // DisAppear was fired
            }
            else
            {
                // no indication resource in PRM!
                // wait until new resources are present -> check Pending in EDDP_PRMDiagTriggerLateErrorDiagAllPending()
                pPDIRFrameData->LateErrDiag.Flags |= EDDP_PRM_PDIRFRAMEDATA_LATE_ERR_FLAGS_INDICATION_IS_PENDING;
                // and put this PDIRFrameData entry to LateErrDiagPendingList
                EDDP_PRMDiagPDIRFrameDataAddToLateErrDiagPendingList (pDDB, PDIRFrameDataIdx);

                // store new state of requested diagnosis by pending
                if (isAppear)
                    pPDIRFrameData->LateErrDiag.Flags |= EDDP_PRM_PDIRFRAMEDATA_LATE_ERR_FLAGS_LAST_REQUESTED_DIAG_BY_PENDING;     // Appear is requested
                else
                    pPDIRFrameData->LateErrDiag.Flags &= ~EDDP_PRM_PDIRFRAMEDATA_LATE_ERR_FLAGS_LAST_REQUESTED_DIAG_BY_PENDING;    // DisAppear is requested
            }
        }
        else
        {
            // this else is only for following cases: The state has changed for example 0->1 and 1->0 without firing of diagnosis, so the pending must be deleted here!
            // user does not see this changing: 0->1 and 1->0
            // is something pending for this PDIRFrameData entry ?
            if (pPDIRFrameData->LateErrDiag.Flags & EDDP_PRM_PDIRFRAMEDATA_LATE_ERR_FLAGS_INDICATION_IS_PENDING)
            {
                // init isLastRequestedDiagByPendingAppear
                if (pPDIRFrameData->LateErrDiag.Flags & EDDP_PRM_PDIRFRAMEDATA_LATE_ERR_FLAGS_LAST_REQUESTED_DIAG_BY_PENDING)
                    isLastRequestedDiagByPendingAppear = LSA_TRUE;
                else
                    isLastRequestedDiagByPendingAppear = LSA_FALSE;

                // compare given "isAppear" with last requested diagnosis by pending
                if (isAppear != isLastRequestedDiagByPendingAppear)
                {
                    // delete pending flag (this diagnosis is no more pending)
                    pPDIRFrameData->LateErrDiag.Flags &= ~EDDP_PRM_PDIRFRAMEDATA_LATE_ERR_FLAGS_INDICATION_IS_PENDING;
                    // remove this PDIRFrameData entry from LateErrDiagPendingList
                    EDDP_PRMDiagPDIRFrameDataRemoveFromLateErrDiagPendingList (pDDB, PDIRFrameDataIdx);

                    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMDiagTriggerLateErrorDiagSingle(): given PDIRFrameDataIdx is still pending but new isAppear(0x%X) != isLastRequestedDiagByPendingAppear(0x%X), so the pending flag is deleted: new LateErrDiag-Flags=0x%X", 
                        isAppear, isLastRequestedDiagByPendingAppear, pPDIRFrameData->LateErrDiag.Flags);
                }
            }
        }
    }
    else
    {
        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMDiagTriggerLateErrorDiagSingle(): PDIRData_A or PDIRSubframeData_A is not present");
    }
}

/*****************************************************************************/
/*  end of file eddp_prm_diag.c                                              */
/*****************************************************************************/

