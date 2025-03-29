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
/*  F i l e               &F: eddp_crt_prov_dfp.c                       :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  CRT (Cycle Real Time) for EDDP  -> Provider DFP                          */
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

#define LTRC_ACT_MODUL_ID   35
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_CRT_PROV_DFP */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"           /* eddp headerfiles */
#include "eddp_int.h"           /* internal header  */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvDfpPackGroupActivateFromPDIRSubframeData        +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB                         +*/
/*+                             LSA_BOOL                       InActiveRecord_A             +*/
/*+                                                                                         +*/
/*+  Result                :    EDD_RSP                                                     +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  InActiveRecord_A   : LSA_TRUE  - search in active record buffer (A)                    +*/
/*+                       LSA_FALSE - search in new record buffer (B)                       +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: This function adds all new provider pack groups to CRT management and     +*/
/*+               activates these pack groups:                                              +*/
/*+                 - add all provider pack groups from PDIRSubframeData to CRT and init    +*/
/*+                 - start the Provider-Pack-Group-Activate-All state machine              +*/
/*+                   only for ACWs, not for ACW_DGs                                        +*/
/*+                                                                                         +*/
/*+               Call this function only if DFP is present.                                +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTProvDfpPackGroupActivateFromPDIRSubframeData(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_BOOL                    InActiveRecord_A)
{
    EDD_RSP                                 Response;
    LSA_UINT32                              iSubfrBlock;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_PRM_PDIRSUBFRAMEDATA_PTR_TYPE      pPDIRSubframeData;

    Response   = EDD_STS_OK;
    pProvMngm  = &pDDB->CRT.Prov;

    // is DFP (PDIRSubframeData and PDIRData) present ?
    if ( EDDP_PRMPDIRSubframeDataIsDfpPresent(pDDB, InActiveRecord_A) )
    {
        if (pProvMngm->PackGroupUsedCnt > 0)
        {
            // all provider pack groups must be removed before adding of new pack groups
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpPackGroupActivateFromPDIRSubframeData(): FATAL -> PackGroupUsedCnt(%d) > 0!", 
                pProvMngm->PackGroupUsedCnt);
            EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
        }

        if (InActiveRecord_A)
            pPDIRSubframeData = pDDB->PRM.pPDIRSubframeData_A;
        else
            pPDIRSubframeData = pDDB->PRM.pPDIRSubframeData_B;

        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProvDfpPackGroupActivateFromPDIRSubframeData(InActiveRecord_A=0x%X): NumberOfSubframeBlocks=%d", 
            InActiveRecord_A, pPDIRSubframeData->NumberOfSubframeBlocks);

        /* ------------------------------------------------ */
        /* first add new pack groups to CRT management      */
        /* look for all FrameIDs in PDIRSubframeData,       */
        /* that have send ACW in PDIRData                   */
        /* ------------------------------------------------ */
        for ( iSubfrBlock=0; iSubfrBlock < pPDIRSubframeData->NumberOfSubframeBlocks; iSubfrBlock++ )
        {
            // is this entry configured as send ACW ?
            if (pPDIRSubframeData->SubframeBlockArray[iSubfrBlock].HwConfig & EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_SND_ACW_MASK)
            {
                if (pPDIRSubframeData->SubframeBlockArray[iSubfrBlock].PDIRFrameDataProvIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
                {
                    EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpPackGroupActivateFromPDIRSubframeData(): FATAL -> For PDIRSubframeData.FrameID(0x%X) is SubframeBlock.HwConfig(0x%X) = Send-ACW but SubframeBlock.PDIRFrameDataProvIdx is invalid! NumberOfSubframeBlocks=%d, iSubfrBlock=%d", 
                        pPDIRSubframeData->SubframeBlockArray[iSubfrBlock].FrameID, 
                        pPDIRSubframeData->SubframeBlockArray[iSubfrBlock].HwConfig, 
                        pPDIRSubframeData->NumberOfSubframeBlocks, 
                        iSubfrBlock);
                    EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
                }
                else
                {
                    // add new provider pack group from PRM to CRT management
                    EDDP_CRTProvDfpPackGroupAddToCRTMngmFromPDIRSubframeData (pDDB, (LSA_UINT16)iSubfrBlock, LSA_FALSE, InActiveRecord_A);

                    // is this entry configured as send ACW with FrameID(x+1) ?
                    // init pack group for FrameID(x+1) always after FrameID(x)
                    if (pPDIRSubframeData->SubframeBlockArray[iSubfrBlock].HwConfig & EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_SND_ACW_RED1_MASK)
                    {
                        // the FrameID(x) in SubframeBlock must be redundant
                        if (EDDP_IS_IRT_REDUNDANT_FRAMEID(pPDIRSubframeData->SubframeBlockArray[iSubfrBlock].FrameID))
                        {
                            if (pPDIRSubframeData->SubframeBlockArray[iSubfrBlock].PDIRFrameDataProvIdxRed1 == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
                            {
                                EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpPackGroupActivateFromPDIRSubframeData(): FATAL -> PDIRSubframeData.FrameID(0x%X) for Send-ACW is redundant but SubframeBlock.PDIRFrameDataProvIdxRed1 is invalid! SubframeBlock.HwConfig=0x%X, NumberOfSubframeBlocks=%d, iSubfrBlock=%d", 
                                    pPDIRSubframeData->SubframeBlockArray[iSubfrBlock].FrameID, 
                                    pPDIRSubframeData->SubframeBlockArray[iSubfrBlock].HwConfig, 
                                    pPDIRSubframeData->NumberOfSubframeBlocks, 
                                    iSubfrBlock);
                                EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
                            }
                            else
                            {
                                // add new Redundant provider pack group with FrameID(x+1) from PRM to CRT management
                                EDDP_CRTProvDfpPackGroupAddToCRTMngmFromPDIRSubframeData (pDDB, (LSA_UINT16)iSubfrBlock, LSA_TRUE, InActiveRecord_A);
                            }
                        }
                        else
                        {
                            EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpPackGroupActivateFromPDIRSubframeData(): FATAL -> in HwConfig(0x%X) is configured send ACW with FrameID(x+1) but PDIRSubframeData.FrameID(0x%X) in not redundant! NumberOfSubframeBlocks=%d, iSubfrBlock=%d", 
                                pPDIRSubframeData->SubframeBlockArray[iSubfrBlock].HwConfig, 
                                pPDIRSubframeData->SubframeBlockArray[iSubfrBlock].FrameID, 
                                pPDIRSubframeData->NumberOfSubframeBlocks, 
                                iSubfrBlock);
                            EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
                        }
                    }
                }
            }
        }

        // are there some new pack groups present ?
        if (pProvMngm->PackGroupUsedCnt > 0)
        {
            // start the Provider-Pack-Group-Activate-All state machine for all pack groups
            EDDP_ASSERT_FALSE(pProvMngm->PackGroupActivateAllSm.State != EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_IDLE);
            pProvMngm->PackGroupActivateAllSm.State  = EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_START;
            pProvMngm->PackGroupActivateAllSm.Cbf    = EDDP_CRTActivatePackStateMachineCbf;
            EDDP_CRTProviderPackGroupActivateAllStateMachine (pDDB, EDD_STS_OK);
            // set Response to PENDING
            Response = EDD_STS_OK_PENDING;
        }
    }
    else
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpPackGroupActivateFromPDIRSubframeData(InActiveRecord_A=0x%X): FATAL -> The record PDIRSubframeData or PDIRData is not present!", 
            InActiveRecord_A);
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvDfpPackGroupAddToCRTMngmFromPDIRSubframeData    +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB                         +*/
/*+                             LSA_UINT16                     SubframeBlockIdx             +*/
/*+                             LSA_BOOL                       isRedundant1                 +*/
/*+                             LSA_BOOL                       InActiveRecord_A             +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  SubframeBlockIdx   : index in PDIRSubframeData.SubframeBlock[x]                        +*/
/*+  isRedundant1       : add redundant FrameID(x+1)                                        +*/
/*+  InActiveRecord_A   : LSA_TRUE  - search in active record buffer (A)                    +*/
/*+                       LSA_FALSE - search in new record buffer (B)                       +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Add new provider pack group from given from PDIRSubframeData              +*/
/*+               to CRT management and init it.                                            +*/
/*+                                                                                         +*/
/*+               Call this function only if DFP is present in.                             +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvDfpPackGroupAddToCRTMngmFromPDIRSubframeData(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  SubframeBlockIdx,
    LSA_BOOL                    isRedundant1,
    LSA_BOOL                    InActiveRecord_A)
{
    LSA_UINT32                                          iSubfrData;
    LSA_UINT16                                          NewPackGrId, NewAcwID, NewAcwDGID, NewDataStatusId;
    LSA_BOOL                                            GetNewACWDG;
    LSA_UINT16                                          AcwDGIdx;
    LSA_UINT32                                          FastForwardingMulticastMACAdd;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE                     pProvMngm;
    EDDP_CRT_PROVIDER_PACK_GROUP_PTR_TYPE               pPackGroupNew, pPackGroupRedX;  // pPackGroupRedX   : redundant FrameID(x)
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE                      pAcwNew, pAcwRedX;              // pAcwRedX         : redundant FrameID(x)
    EDDP_CRT_PROVIDER_DS_PTR_TYPE                       pDataStatus;
    EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE                     pPDIRFrameData;
    EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_PTR_TYPE    pSubframeBlock;

    pProvMngm       = &pDDB->CRT.Prov;
    pPDIRFrameData  = LSA_NULL;
    pSubframeBlock  = LSA_NULL;
    pPackGroupRedX  = LSA_NULL;
    pAcwRedX        = LSA_NULL;

    // is DFP (PDIRSubframeData and PDIRData) present ?
    if ( EDDP_PRMPDIRSubframeDataIsDfpPresent(pDDB, InActiveRecord_A) )
    {
        /* ---------------------------------------------------------------- */
        /* init local pointer                                               */
        /* ---------------------------------------------------------------- */
        if (InActiveRecord_A)
        {
            EDDP_ASSERT_FALSE(SubframeBlockIdx >= pDDB->PRM.pPDIRSubframeData_A->NumberOfSubframeBlocks);
            pSubframeBlock = &pDDB->PRM.pPDIRSubframeData_A->SubframeBlockArray[SubframeBlockIdx];

            // is this redundant call ?
            if (isRedundant1)
            {
                EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataProvIdxRed1 == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
                pPDIRFrameData = &pDDB->PRM.pPDIRData_A->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataProvIdxRed1];
            }
            else
            {
                EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataProvIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
                pPDIRFrameData = &pDDB->PRM.pPDIRData_A->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataProvIdx];
            }
        }
        else
        {
            EDDP_ASSERT_FALSE(SubframeBlockIdx >= pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks);
            pSubframeBlock = &pDDB->PRM.pPDIRSubframeData_B->SubframeBlockArray[SubframeBlockIdx];

            // is this redundant call ?
            if (isRedundant1)
            {
                EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataProvIdxRed1 == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
                pPDIRFrameData = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataProvIdxRed1];
            }
            else
            {
                EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataProvIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
                pPDIRFrameData = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataProvIdx];
            }
        }

        /* ---------------------------------------------------------------- */
        /* check combination Redundant with FrameID                         */
        /* ---------------------------------------------------------------- */
        if ( (isRedundant1) && ( !(EDDP_IS_IRT_REDUNDANT_FRAMEID(pSubframeBlock->FrameID))) )
        {
            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpPackGroupAddToCRTMngmFromPDIRSubframeData(isRedundant1=0x%X): FATAL -> PDIRSubframeData.FrameID(0x%X) is not redundant", 
                isRedundant1, pSubframeBlock->FrameID);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }

        /* ---------------------------------------------------------------- */
        /* if isRedundant1, the PackGroup with FrameID(x) is always         */
        /* last PackGroup in PackGroupUsedList, because the call with       */
        /* FrameID(x+1) is always after call with FrameID(x).               */
        /* init pointer "pAcwRedX" to PackGroup with FrameID(x).            */
        /* ---------------------------------------------------------------- */
        if (isRedundant1)
        {
            EDDP_ASSERT_FALSE(pProvMngm->PackGroupUsedList.EndIdx == EDDP_CRT_CHAIN_IDX_END);
            pPackGroupRedX  = &pProvMngm->pProvPackGroupEntry[pProvMngm->PackGroupUsedList.EndIdx];
            EDDP_ASSERT_FALSE(pPackGroupRedX->AcwId == EDDP_CRT_CHAIN_IDX_END);
            pAcwRedX        = &pProvMngm->pAcwEntry[pPackGroupRedX->AcwId];
        }

        /* ---------------------------------------------------------------- */
        /* check TxPortID for this PackGroup                                */
        /* ---------------------------------------------------------------- */
        if (pPDIRFrameData->TxPort == 0)
        {
            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpPackGroupAddToCRTMngmFromPDIRSubframeData(): FATAL -> for FrameID(0x%X) is TxPortID=0! TxPortGroupArray(0x%X)", 
                pPDIRFrameData->FrameID, pPDIRFrameData->TxPortGroupArray);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }

        /* ---------------------------------------------------------------- */
        /* get and init new DataStatus entry                                */
        /* ---------------------------------------------------------------- */
        if (isRedundant1)
        {
            // FrameID(x+1) has the same DataStatus entry like FrameID(x)
            NewDataStatusId = pAcwRedX->DataStatusId;
        }
        else
        {
            NewDataStatusId = EDDP_CRTDataStatusAddToUsedList (pDDB);

		    if (NewDataStatusId == EDDP_CRT_CHAIN_IDX_END)
            {
                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpPackGroupAddToCRTMngmFromPDIRSubframeData(): FATAL -> No more DataStatus entries! ProviderUsedCnt=%d, AcwUsedCnt=%d", 
                    pProvMngm->ProviderUsedCnt, pProvMngm->AcwUsedCnt);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            else
            {
                pDataStatus = &pProvMngm->pDSEntry[NewDataStatusId];

                // switch to current PNIP revision
                switch (EDDP_HALGetPnipRevision(pDDB))
                {
                    #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
                    case EDDP_HAL_HW_PNIP_REV1:
                    {
                        // this is the DataStatus in APDU-Status
                        pDataStatus->LocalDataStatus    = EDDP_CRT_PACK_FRAME_DSTAT_LOCAL_INIT_VALUE;
                        pDataStatus->DS_Selector        = EDDP_CRT_PACK_FRAME_GROUP_ID;
                        pDataStatus->pHwDSApiPnipR1     = &pProvMngm->pHwBaseDSApiPnipR1[NewDataStatusId];

                        // set Bit "DataStatus.Ignore" (dependent of SFCRC16)
                        switch (pSubframeBlock->SFIOCRProperties.SFCRC16)
                        {
                            case EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_SFCRC16_0:
                            {
                                EDDP_SET_BIT_VALUE16( pDataStatus->LocalDataStatus, 
                                                      0, 
                                                      EDDP_CRT_PACK_FRAME_DSTAT_LOCAL_IGNORE_MASK, 
                                                      EDDP_CRT_PACK_FRAME_DSTAT_LOCAL_IGNORE_SHIFT)
                            }
                            break;

                            case EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_SFCRC16_1:
                            {
                                EDDP_SET_BIT_VALUE16( pDataStatus->LocalDataStatus, 
                                                      1, 
                                                      EDDP_CRT_PACK_FRAME_DSTAT_LOCAL_IGNORE_MASK, 
                                                      EDDP_CRT_PACK_FRAME_DSTAT_LOCAL_IGNORE_SHIFT)
                            }
                            break;

                            default:
                            {
                                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpPackGroupAddToCRTMngmFromPDIRSubframeData(): FATAL -> SFIOCRProperties.SFCRC16(0x%X) is unknown!", 
                                    pSubframeBlock->SFIOCRProperties.SFCRC16);
                                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                            }
                            break;
                        }

                        // write DataStatus data to APICtrl-RAM
                        EDDP_HAL_MEM8_WRITE (pDDB, &pDataStatus->pHwDSApiPnipR1->DataStatus,  pDataStatus->LocalDataStatus);
                        EDDP_HAL_MEM8_WRITE (pDDB, &pDataStatus->pHwDSApiPnipR1->DS_Selector, pDataStatus->DS_Selector);
                    }
                    break;
                    #endif

                    #if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
                    case EDDP_HAL_HW_PNIP_REV2:
                    case EDDP_HAL_HW_PNIP_REV3:
                    {
                        // this is the DataStatus in APDU-Status
                        pDataStatus->LocalDataStatus    = EDDP_CRT_PACK_FRAME_DSTAT_LOCAL_INIT_VALUE;
                        pDataStatus->DS_Selector        = EDDP_CRT_PACK_FRAME_GROUP_ID;
                        pDataStatus->pHwDSApiPnipR2     = &pProvMngm->pHwBaseDSApiPnipR2[NewDataStatusId];

                        // set Bit "DataStatus.Ignore" (dependent of SFCRC16)
                        switch (pSubframeBlock->SFIOCRProperties.SFCRC16)
                        {
                            case EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_SFCRC16_0:
                            {
                                EDDP_SET_BIT_VALUE16( pDataStatus->LocalDataStatus, 
                                                      0, 
                                                      EDDP_CRT_PACK_FRAME_DSTAT_LOCAL_IGNORE_MASK, 
                                                      EDDP_CRT_PACK_FRAME_DSTAT_LOCAL_IGNORE_SHIFT)
                            }
                            break;

                            case EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_SFCRC16_1:
                            {
                                EDDP_SET_BIT_VALUE16( pDataStatus->LocalDataStatus, 
                                                      1, 
                                                      EDDP_CRT_PACK_FRAME_DSTAT_LOCAL_IGNORE_MASK, 
                                                      EDDP_CRT_PACK_FRAME_DSTAT_LOCAL_IGNORE_SHIFT)
                            }
                            break;

                            default:
                            {
                                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpPackGroupAddToCRTMngmFromPDIRSubframeData(): FATAL -> SFIOCRProperties.SFCRC16(0x%X) is unknown!", 
                                    pSubframeBlock->SFIOCRProperties.SFCRC16);
                                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                            }
                            break;
                        }

                        // write DataStatus data to APICtrl-RAM
                        EDDP_HAL_MEM8_WRITE (pDDB, &pDataStatus->pHwDSApiPnipR2->DataStatus,  pDataStatus->LocalDataStatus);
                        EDDP_HAL_MEM8_WRITE (pDDB, &pDataStatus->pHwDSApiPnipR2->DS_Selector, pDataStatus->DS_Selector);
                    }
                    break;
                    #endif
    
                    default:
                    {
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpPackGroupAddToCRTMngmFromPDIRSubframeData(): FATAL -> Version(%d) of PNIP is unknown", 
                            EDDP_HALGetPnipRevision(pDDB));
                        EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                    }
                    break;
                }

            }
        }

        /* ---------------------------------------------------------------- */
        /* add new ACW to AcwUsedList                                       */
        /* copy ACW specific parameter                                      */
        /* ---------------------------------------------------------------- */
        // get new ACW from FreeList
        NewAcwID = EDDP_CRTProvAcwAddToUsedList (pDDB);

        if (NewAcwID == EDDP_CRT_CHAIN_IDX_END)
        {
            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpPackGroupAddToCRTMngmFromPDIRSubframeData(): FATAL -> No more free ACW! ProviderUsedCnt=%d, AcwUsedCnt=%d", 
                pProvMngm->ProviderUsedCnt, pProvMngm->AcwUsedCnt);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        pAcwNew = &pProvMngm->pAcwEntry[NewAcwID];

        // init this new ACW with data
        pAcwNew->ROGPhase                       = EDDP_CRT_PROV_ACW_PHASE_R;
        pAcwNew->DataStatusId                   = NewDataStatusId;
        /* AddParam */
        pAcwNew->AddParam.FrameID               = pPDIRFrameData->FrameID;
        pAcwNew->AddParam.CycleReductionRatio   = pPDIRFrameData->ReductionRatio;
        pAcwNew->AddParam.CyclePhase            = pPDIRFrameData->Phase;
        pAcwNew->AddParam.TxPortID              = pPDIRFrameData->TxPort;
        pAcwNew->AddParam.FrameSendOffset       = pPDIRFrameData->FrameSendOffset;
        /* DfpParam */
        if (isRedundant1)
        {
            pAcwNew->DfpParam.ACW_DG_order      = pSubframeBlock->SFIOCRProperties.DFPRedundantPathLayout;
        }
        else
        {
            pAcwNew->DfpParam.ACW_DG_order      = EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_HIGH;
        }
        pAcwNew->DfpParam.PrmSubframeBlockIdx   = SubframeBlockIdx; // connect this ACW with PDIRSubframeData
        pAcwNew->DfpParam.All_DG_Len = 0;

        /* ---------------------------------------------------------------- */
        /* get new PackGroup for this ACW                                   */
        /* ---------------------------------------------------------------- */
        NewPackGrId = EDDP_CRTProvDfpPackGroupAddToUsedList (pDDB);

        if (NewPackGrId == EDDP_CRT_CHAIN_IDX_END)
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpPackGroupAddToCRTMngmFromPDIRSubframeData(): FATAL -> No more free PackGroups! PackGroupUsedCnt=%d", 
                pProvMngm->PackGroupUsedCnt);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        // init new PackGroup and connect it with ACW
        pPackGroupNew               = &pProvMngm->pProvPackGroupEntry[NewPackGrId];
        pPackGroupNew->AcwId        = pAcwNew->AcwId;
        pPackGroupNew->isRedundant1 = isRedundant1;
        // connect this new ACW to the PackGroup
        pAcwNew->DfpParam.PackGrId  = NewPackGrId;

        EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProvDfpPackGroupAddToCRTMngmFromPDIRSubframeData(SubframeBlockIdx=%d, isRedundant1=0x%X, InActiveRecord_A=0x%X): NewPackGrId=%d", 
            SubframeBlockIdx, isRedundant1, InActiveRecord_A, NewPackGrId);
        // do not delete the empty spaces in this trace text
        EDDP_CRT_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--]    ACW-Params: NewAcwID=%d, FrameID=0x%X, TxPortID=%d, FSO=0x%X, RR=%d, Ph=%d", 
            pAcwNew->AcwId, 
            pAcwNew->AddParam.FrameID, 
            pAcwNew->AddParam.TxPortID, 
            pAcwNew->AddParam.FrameSendOffset, 
            pAcwNew->AddParam.CycleReductionRatio, 
            pAcwNew->AddParam.CyclePhase);
        // do not delete the empty spaces in this trace text
        EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--]    DFP-Params: NumberOfSubframeData=%d, DFPmode=%d, HwConfig=0x%X, ACW_DG_order=0x%X", 
            pSubframeBlock->NumberOfSubframeData, 
            pSubframeBlock->SFIOCRProperties.DFPmode, 
            pSubframeBlock->HwConfig, 
            pAcwNew->DfpParam.ACW_DG_order);

        /* ---------------------------------------------------------------- */
        /* ACW_DGs                                                          */
        /*  - DFPmode = END_NODE (all ACW_DGs)                              */
        /*  - DFPmode = Position (only one ACW_DG)                          */
        /* ---------------------------------------------------------------- */
        if (isRedundant1)
        {
            // ACW_DGs are already used by PackGroup with FrameID(x)
            // connect new PackGroup-ACW(x+1) to these ACW_DGs
            // loop through all ACW_DGs of ACW(x)

			iSubfrData = 0;

            AcwDGIdx = pAcwRedX->AcwDGList.BeginIdx;
            while (AcwDGIdx != EDDP_CRT_CHAIN_IDX_END)
            {
                EDDP_ASSERT_FALSE(AcwDGIdx >= pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber);
                // add ACW_DG to ACW(x+1)
                EDDP_CRTProvAcwDGAddToAcwRed1 (pDDB, pAcwRedX->AcwId, pAcwNew->AcwId, AcwDGIdx);
                // next AcwDGIdx in this ACW
                AcwDGIdx = pProvMngm->pAcwDGEntry[AcwDGIdx].AcwDGAcwLink.Next;
            }


        }
        else
        {
            // get new ACW_DGs
            for ( iSubfrData=0; iSubfrData < (pSubframeBlock->NumberOfSubframeData); iSubfrData++ )
            {
                GetNewACWDG = LSA_FALSE;

                if (pSubframeBlock->SFIOCRProperties.DFPmode == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_DFPMODE_END_NODE)
                {
                    // all ACW_DGs of this pack frame
                    GetNewACWDG = LSA_TRUE;
                }
                else
                {
                    // only one ACW_DG with own Subframe position in "DFPmode"      -> DFPmode = SubframeData.Position
                    // search this ACW_DG in SubframeDataArray
                    if (pSubframeBlock->SFIOCRProperties.DFPmode == pSubframeBlock->SubframeDataArray[iSubfrData].Position)
                    {
                        GetNewACWDG = LSA_TRUE;
                    }
                }

                if (GetNewACWDG)
                {
                    // get new ACW_DG from FreeList
                    NewAcwDGID = EDDP_CRTProvAcwDGAddToUsedList (pDDB);

                    if (NewAcwDGID == EDDP_CRT_CHAIN_IDX_END)
                    {
                        EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpPackGroupAddToCRTMngmFromPDIRSubframeData(): FATAL -> No more free ACW_DG! ProviderUsedCnt=%d, AcwUsedCnt=%d, AcwDGUsedCnt=%d", 
                            pProvMngm->ProviderUsedCnt, pProvMngm->AcwUsedCnt, pProvMngm->AcwDGUsedCnt);
                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                    }
                    else
                    {
                        EDDP_CRT_PROVIDER_ACW_DG_PTR_TYPE pAcwDG;
                        pAcwDG = &pProvMngm->pAcwDGEntry[NewAcwDGID];
                        // add ACW_DG to ACW
                        EDDP_CRTProvAcwDGAddToAcw (pDDB, pAcwNew->AcwId, pAcwDG->AcwDGId);

                        // copy parameter to ACW_DG
                        pAcwDG->AddParam.DG_PosT    = pSubframeBlock->SubframeDataArray[iSubfrData].Position;
                        pAcwDG->AddParam.DG_Len     = pSubframeBlock->SubframeDataArray[iSubfrData].DataLength;
                        pAcwDG->AddParam.DG_Offset  = pSubframeBlock->SubframeDataArray[iSubfrData].Offset;

                        // sum of all ACW_DG lengths
                        pAcwNew->DfpParam.All_DG_Len += pAcwDG->AddParam.DG_Len;

                        // do not delete the empty spaces in this trace text
                        EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--]    DG-Params: NewAcwDGID=%d, DG-PosT=%d, DG-Len=%d, DG-Offset=%d", 
                            NewAcwDGID, pAcwDG->AddParam.DG_PosT, pAcwDG->AddParam.DG_Len, pAcwDG->AddParam.DG_Offset);

                        if (pSubframeBlock->SFIOCRProperties.DFPmode != EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_DFPMODE_END_NODE)
                        {
                            // only one ACW_DG - exit FOR loop
                            iSubfrData = pSubframeBlock->NumberOfSubframeData;
                        }
                    }
                }
            }
        }

        // check this pack frame has ACW_DGs
        if (pAcwNew->DfpParam.AcwDGCnt == 0)
        {
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpPackGroupAddToCRTMngmFromPDIRSubframeData(): FATAL -> This pack frame has no ACW_DG!");
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }

        /* ---------------------------------------------------------------- */
        /* set DstMAC to new ACW                                            */
        /* ---------------------------------------------------------------- */
        // is "FrameDataProperties" present in PDIRData ?
        if ( EDDP_PRMPDIRDataIsFrameDataPropertiesPresent(pDDB, InActiveRecord_A) )
        {
            // get parameter PDIRData.FrameDataProperties.FastForwardingMulticastMACAdd
            FastForwardingMulticastMACAdd = 
                EDDP_PRMPDIRDataGetFrameDataProperties (pDDB, EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_SHIFT, InActiveRecord_A);
            // FrameDataProperties(LEGACY) is not permitted
            if (FastForwardingMulticastMACAdd == EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_LEGACY)
            {
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpPackGroupAddToCRTMngmFromPDIRSubframeData(): FATAL -> PDIRData.FrameDataProperties is Legacy!");
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            // set DstMAC to ACW
            EDDP_CRTProvAcwSetDstMACFromPDIRData (pDDB, pAcwNew->AcwId, (LSA_UINT16)FastForwardingMulticastMACAdd);
        }
        else
        {
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpPackGroupAddToCRTMngmFromPDIRSubframeData(): FATAL -> PDIRData.FrameDataProperties is not present!");
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }

        // do the following steps only for Device (for Controller, this will be done at Provider-Activate)
        if (pSubframeBlock->SFIOCRProperties.DFPmode != EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_DFPMODE_END_NODE)
        {
            EDDP_CRTProvDfpAcwAcwDGInitAndChainHwApi(pDDB, pAcwNew->AcwId, isRedundant1);
        }
    }
    else
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpPackGroupAddToCRTMngmFromPDIRSubframeData(InActiveRecord_A=0x%X): FATAL -> The record PDIRSubframeData or PDIRData is not present!", 
            InActiveRecord_A);
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvDfpAcwAcwDGInitAndChainHwApi                    +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB                         +*/
/*+                             LSA_UINT16                     AcwID                        +*/
/*+                             LSA_BOOL                       isRedundant1                 +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  AcwID              : Index in ACW array                                                +*/
/*+  isRedundant1       : add redundant FrameID(x+1)                                        +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: This function does several initializations, which were originally done    +*/
/*+               in the function EDDP_CRTProvDfpPackGroupAddToCRTMngmFromPDIRSubframeData: +*/
/*+               - init ACW in APICtrl-RAM                                                 +*/
/*+               - add ACW to TBase tree and AcwPhaseList                                  +*/
/*+               - init all ACW_DGs in APICtrl-RAM (not for for redundant ACW(x+1))        +*/
/*+               - chain this ACW in HW                                                    +*/
/*+               - chain all ACW_DGs of this ACW in HW                                     +*/
/*+                                                                                         +*/
/*+               These steps were sourced out as they can also be executed from            +*/
/*+               Provider-Activate, for Controller. For Device, they are executed at       +*/
/*+               PrmCommit.                                                                +*/
/*+                                                                                         +*/
/*+               Call this function only if DFP is present.                                +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvDfpAcwAcwDGInitAndChainHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID,
    LSA_BOOL                    isRedundant1)
{
    LSA_UINT16                                          AcwDGIdx;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE                     pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE                      pAcw;

    pProvMngm = &pDDB->CRT.Prov;
    EDDP_ASSERT_FALSE(AcwID == EDDP_CRT_CHAIN_IDX_END);
    pAcw = &pProvMngm->pAcwEntry[AcwID];

    /* ---------------------------------------------------------------- */
    /* init ACW in APICtrl-RAM                                          */
    /* and add this ACW to TBase tree                                   */
    /* ---------------------------------------------------------------- */
    // init ACW in APICtrl-RAM
    pProvMngm->AcwHandler.FctInitHwApiPackFrame (pDDB, pAcw->AcwId, LSA_TRUE);
    // chain this ACW in AcwPhaseList
    EDDP_CRTProvAcwAddToTBase (pDDB, pAcw->AcwId, LSA_TRUE);
    
    /* -------------------------------- */
    /* init all ACW_DGs in APICtrl-RAM  */
    /* -------------------------------- */
    // is this redundant call ?
    if (isRedundant1)
    {
        // nothing to do
        // this call is for redundant ACW(x+1) - ACW_DGs are already inited by ACW(x)
    }
    else
    {
        AcwDGIdx = pAcw->AcwDGList.BeginIdx;
        while (AcwDGIdx != EDDP_CRT_CHAIN_IDX_END)
        {
            EDDP_ASSERT_FALSE(AcwDGIdx >= pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber);
            // init ACW_DG in APICtrl-RAM
            pProvMngm->AcwHandler.FctDGInitHwApiFromPrmCommit (pDDB, AcwDGIdx);
            // next AcwDGIdx in this ACW
            AcwDGIdx = pProvMngm->pAcwDGEntry[AcwDGIdx].AcwDGAcwLink.Next;
        }
    }

    /* ------------------------------------------------ */
    /* chain this ACW in HW (update ACW.ACW_nextoffset) */
    /* ------------------------------------------------ */
    pDDB->CRT.Prov.AcwHandler.FctUpdateAcwNextOffsetAcwHwApi (pDDB, pAcw->AcwId);

    /* ------------------------------------------------ */
    /* chain all ACW_DGs of this ACW in HW              */
    /*  -> update ACW.ACW_DG_offset (if first ACW_DG)   */
    /*  -> update ACW_DG.ACW_DG_nextoffset0/1           */
    /*  -> only after all ACW_DGs are available in HW   */
    /* ------------------------------------------------ */
    // is this redundant call ?
    if (isRedundant1)
    {
        // ACW_DGs will be chained in AcwDGAcwLinkRed1
        // this call is for redundant ACW(x+1) - it is dependent of ACW_DG_order
        switch (pAcw->DfpParam.ACW_DG_order)
        {
            case EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_HIGH:
            {
                // ACW_DG order is same like in ACW(x)
                AcwDGIdx = pAcw->AcwDGList.BeginIdx;
                while (AcwDGIdx != EDDP_CRT_CHAIN_IDX_END)
                {
                    EDDP_ASSERT_FALSE(AcwDGIdx >= pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber);
                    // chain ACW_DG in HW (update ACW_DG.ACW_DG_nextoffset)
                    pDDB->CRT.Prov.AcwHandler.FctUpdateAcwDGNextOffsetAcwHwApi (pDDB, pAcw->AcwId, AcwDGIdx);
                    // next AcwDGIdx in this ACW
                    AcwDGIdx = pProvMngm->pAcwDGEntry[AcwDGIdx].AcwDGAcwLinkRed1.Next;
                }
            }
            break;

            case EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_DOWN:
            {
                // ACW_DG order is reversed like in ACW(x)
                AcwDGIdx = pAcw->AcwDGList.EndIdx;
                while (AcwDGIdx != EDDP_CRT_CHAIN_IDX_END)
                {
                    EDDP_ASSERT_FALSE(AcwDGIdx >= pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber);
                    // chain ACW_DG in HW (update ACW_DG.ACW_DG_nextoffset)
                    pDDB->CRT.Prov.AcwHandler.FctUpdateAcwDGNextOffsetAcwHwApi (pDDB, pAcw->AcwId, AcwDGIdx);
                    // next AcwDGIdx in this ACW
                    AcwDGIdx = pProvMngm->pAcwDGEntry[AcwDGIdx].AcwDGAcwLinkRed1.Prev;
                }
            }
            break;

            default:
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpPackGroupAddToCRTMngmFromPDIRSubframeData(): FATAL -> ACW_DG_order(0x%X) is unknown!",
                    pAcw->DfpParam.ACW_DG_order);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }
    else
    {
        // ACW_DGs are chained in AcwDGAcwLink
        AcwDGIdx = pAcw->AcwDGList.BeginIdx;
        while (AcwDGIdx != EDDP_CRT_CHAIN_IDX_END)
        {
            EDDP_ASSERT_FALSE(AcwDGIdx >= pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber);
            // chain ACW_DG in HW (update ACW_DG.ACW_DG_nextoffset)
            pDDB->CRT.Prov.AcwHandler.FctUpdateAcwDGNextOffsetAcwHwApi (pDDB, pAcw->AcwId, AcwDGIdx);
            // next AcwDGIdx in this ACW
            AcwDGIdx = pProvMngm->pAcwDGEntry[AcwDGIdx].AcwDGAcwLink.Next;
        }
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvDfpPackGroupRemoveFromCRTMngm                   +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB                         +*/
/*+                             LSA_UINT16                     PackGrId                     +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                     +*/
/*+  PackGrId       : PackGrId to be removed                                                +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Remove one provider pack group from CRT management.                       +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvDfpPackGroupRemoveFromCRTMngm(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PackGrId)
{
    LSA_UINT16                              AcwIdx, AcwDGIdx;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
//  EDDP_CRT_PROVIDER_DDB_MNGM_PTR_TYPE     pProvMngm;
    EDDP_CRT_PROVIDER_PACK_GROUP_PTR_TYPE   pPackGroup;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;

    EDDP_ASSERT_FALSE(PackGrId >= pDDB->CRT.CfgParam.ProviderPackGroupMaxNumber);

//  pProvMngm       = &pDDB->CRT.Prov;
    pProvMngm       = &pDDB->CRT.Prov;
    pPackGroup      = &pProvMngm->pProvPackGroupEntry[PackGrId];
    EDDP_ASSERT_FALSE(pPackGroup->AcwId == EDDP_CRT_CHAIN_IDX_END);
    AcwIdx          = pPackGroup->AcwId;
    pAcw            = &pProvMngm->pAcwEntry[AcwIdx];

    EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProvDfpPackGroupRemoveFromCRTMngm(PackGrId=%d): isRedundant1=0x%X, FrameID=0x%X, PackGroupUsedCnt=%d", 
        PackGrId, pPackGroup->isRedundant1, pAcw->AddParam.FrameID, pProvMngm->PackGroupUsedCnt);

    // is this pack group redundant with FrameID(x+1) ?
    if (pPackGroup->isRedundant1)
    {
        /* ---------------------------------------------------- */
        /* remove only ACW_DGs from ACWRed1                     */
        /* ---------------------------------------------------- */
        // which ACW_DG order has this ACWRed1 ?
        switch (pAcw->DfpParam.ACW_DG_order)
        {
            case EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_HIGH:
            {
                AcwDGIdx = pAcw->AcwDGList.BeginIdx;
                while (AcwDGIdx != EDDP_CRT_CHAIN_IDX_END)
                {
                    EDDP_ASSERT_FALSE(AcwDGIdx >= pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber);
                    // remove ACW_DG from ACWRed1
                    EDDP_CRTProvAcwDGRemoveFromAcwRed1 (pDDB, pPackGroup->AcwId, AcwDGIdx);
                    // next AcwDGIdx in this ACW
                    AcwDGIdx = pProvMngm->pAcwDGEntry[AcwDGIdx].AcwDGAcwLinkRed1.Next;
                }
            }
            break;

            case EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_DOWN:
            {
                AcwDGIdx = pAcw->AcwDGList.EndIdx;
                while (AcwDGIdx != EDDP_CRT_CHAIN_IDX_END)
                {
                    EDDP_ASSERT_FALSE(AcwDGIdx >= pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber);
                    // remove ACW_DG from ACWRed1
                    EDDP_CRTProvAcwDGRemoveFromAcwRed1 (pDDB, pPackGroup->AcwId, AcwDGIdx);
                    // next AcwDGIdx in this ACW
                    AcwDGIdx = pProvMngm->pAcwDGEntry[AcwDGIdx].AcwDGAcwLinkRed1.Prev;
                }
            }
            break;

            default:
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpPackGroupRemoveFromCRTMngm(): FATAL -> ACW_DG_order(0x%X) is unknown!", 
                    pAcw->DfpParam.ACW_DG_order);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }
    else
    {
        /* ---------------------------------------------------- */
        /* remove all of this pack group                        */
        /* ---------------------------------------------------- */
        // delete DataStatus in APICtrl-RAM
        EDDP_CRTProviderDeleteDataStatusHwApi (pDDB, pAcw->DataStatusId);
        // remove DataStatus from UsedList
        EDDP_CRTDataStatusRemoveFromUsedList (pDDB, pAcw->DataStatusId);

        // remove all ACW_DGs of this pack group
        AcwDGIdx = pAcw->AcwDGList.BeginIdx;
        while (AcwDGIdx != EDDP_CRT_CHAIN_IDX_END)
        {
            EDDP_ASSERT_FALSE(AcwDGIdx >= pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber);
            // remove ACW_DG from ACW
            EDDP_CRTProvAcwDGRemoveFromAcw (pDDB, AcwDGIdx);
            // remove ACW_DG from UsedList
            EDDP_CRTProvAcwDGRemoveFromUsedList (pDDB, AcwDGIdx);
            // next AcwDGIdx in this ACW
            AcwDGIdx = pProvMngm->pAcwDGEntry[AcwDGIdx].AcwDGAcwLink.Next;
        }
    }

    // disconnect the ACW of this PackGroup from PDIRSubframeData and from pack group
    pAcw->DfpParam.PrmSubframeBlockIdx  = EDDP_CRT_CHAIN_IDX_END;
    pAcw->DfpParam.PackGrId             = EDDP_CRT_CHAIN_IDX_END;
    // this PackGroup has no more connected ACW
    pPackGroup->AcwId                   = EDDP_CRT_CHAIN_IDX_END;

    // remove this pack group from UsedList
    EDDP_CRTProvDfpPackGroupRemoveFromUsedList (pDDB, PackGrId);
    // remove ACW from UsedList
    EDDP_CRTProvAcwRemoveFromUsedList (pDDB, AcwIdx);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvDfpPackGroupAddToUsedList                       +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_UINT16                                                  +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Add new PackGroup to UsedList.                                            +*/
/*+                 -> get first free index from PackGroupFreeList and put it to            +*/
/*+                    PackGroupUsedList                                                    +*/
/*+                 -> the NewPackGrId will be returned in return value                     +*/
/*+                                                                                         +*/
/*+                 -> if (NewPackGrId == EDDP_CRT_CHAIN_IDX_END)                           +*/
/*+                     - no more free PackGroup                                            +*/
/*+                     - set error by caller of this function                              +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTProvDfpPackGroupAddToUsedList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT16                              NewPackGrId;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
//  EDDP_CRT_PROVIDER_DDB_MNGM_PTR_TYPE     pProvMngm;
    EDDP_CRT_PROVIDER_PACK_GROUP_PTR_TYPE   pPackGroup;

//  pProvMngm       = &pDDB->CRT.Prov;
    pProvMngm       = &pDDB->CRT.Prov;
    NewPackGrId     = pProvMngm->PackGroupFreeList.BeginIdx;

    if (NewPackGrId == EDDP_CRT_CHAIN_IDX_END)
    {
        // no more free PackGroup
        // set error by caller of this function
    }
    else
    {
        pPackGroup = &pProvMngm->pProvPackGroupEntry[NewPackGrId];

        /* ------------------------------------------------------------ */
	    /* Remove this new pack group from free-chain list              */
	    /* ------------------------------------------------------------ */
        if (pPackGroup->UsedFreeLink.Next == EDDP_CRT_CHAIN_IDX_END)
        {
            // this PackGroup is the last free PackGroup
            pProvMngm->PackGroupFreeList.BeginIdx    = EDDP_CRT_CHAIN_IDX_END;
            pProvMngm->PackGroupFreeList.EndIdx      = EDDP_CRT_CHAIN_IDX_END;
        }
        else
        {
            // this PackGroup is not the last free PackGroup
            pProvMngm->PackGroupFreeList.BeginIdx                                            = pPackGroup->UsedFreeLink.Next;
            pProvMngm->pProvPackGroupEntry[pPackGroup->UsedFreeLink.Next].UsedFreeLink.Prev  = EDDP_CRT_CHAIN_IDX_END;
        }

        /* ------------------------------------------------------------ */
        /* Put this new PackGroup to end used-chain list                */
        /* ------------------------------------------------------------ */
        if (pProvMngm->PackGroupUsedList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
        {
            // this PackGroup is the first PackGroup in the used list
            pProvMngm->PackGroupUsedList.BeginIdx    = NewPackGrId;
            pProvMngm->PackGroupUsedList.EndIdx      = NewPackGrId;
            pPackGroup->UsedFreeLink.Prev               = EDDP_CRT_CHAIN_IDX_END;
            pPackGroup->UsedFreeLink.Next               = EDDP_CRT_CHAIN_IDX_END;
        }
        else
        {
            // this PackGroup is not the first PackGroup in the used list
            pPackGroup->UsedFreeLink.Prev           = pProvMngm->PackGroupUsedList.EndIdx;
            pPackGroup->UsedFreeLink.Next           = EDDP_CRT_CHAIN_IDX_END;
            pProvMngm->pProvPackGroupEntry[pProvMngm->PackGroupUsedList.EndIdx].UsedFreeLink.Next = NewPackGrId;
            pProvMngm->PackGroupUsedList.EndIdx  = NewPackGrId;
        }

        pProvMngm->PackGroupUsedCnt++;
    }

    return (NewPackGrId);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvDfpPackGroupRemoveFromUsedList                  +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             LSA_UINT16                          PackGrId                +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                     +*/
/*+  PackGrId       : Index in PackGroup array                                              +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Remove PackGroup from UsedList                                            +*/
/*+                 -> remove this PackGroup from PackGroupUsedList and put it to           +*/
/*+                    PackGroupFreeList                                                    +*/
/*+                 -> no check PackGrId, it must be valid !                                +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvDfpPackGroupRemoveFromUsedList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PackGrId)
{
    LSA_UINT16                              Prev;
    LSA_UINT16                              Next;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
//  EDDP_CRT_PROVIDER_DDB_MNGM_PTR_TYPE     pProvMngm;
    EDDP_CRT_PROVIDER_PACK_GROUP_PTR_TYPE   pPackGroup;

//  pProvMngm       = &pDDB->CRT.Prov;
    pProvMngm       = &pDDB->CRT.Prov;
    pPackGroup      = &pProvMngm->pProvPackGroupEntry[PackGrId];
    Prev            = pPackGroup->UsedFreeLink.Prev;
    Next            = pPackGroup->UsedFreeLink.Next;

    // are there some PackGroups in CRT management ?
    if (pProvMngm->PackGroupUsedCnt == 0)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpPackGroupRemoveFromUsedList(PackGrId=%d): FATAL -> PackGroupUsedCnt=0!", 
            PackGrId);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        /* ------------------------------------------------------------ */
        /* Remove this PackGroup from used-chain list                   */
        /* ------------------------------------------------------------ */
        if ((Prev == EDDP_CRT_CHAIN_IDX_END) || (Next == EDDP_CRT_CHAIN_IDX_END))
        {
            if (Prev == EDDP_CRT_CHAIN_IDX_END)
            {
                // this PackGroup is first PackGroup in used list
                if (Next == EDDP_CRT_CHAIN_IDX_END)
                {
                    // first and only one PackGroup in used list
                    pProvMngm->PackGroupUsedList.BeginIdx    = EDDP_CRT_CHAIN_IDX_END;
                    pProvMngm->PackGroupUsedList.EndIdx      = EDDP_CRT_CHAIN_IDX_END;
                }
                else
                {
                    // first PackGroup in used list but not only one
                    pProvMngm->PackGroupUsedList.BeginIdx                    = Next;
                    pProvMngm->pProvPackGroupEntry[Next].UsedFreeLink.Prev   = EDDP_CRT_CHAIN_IDX_END;
                }
            }
            else
            {
                // last PackGroup in used list but not only one
                pProvMngm->pProvPackGroupEntry[Prev].UsedFreeLink.Next   = EDDP_CRT_CHAIN_IDX_END;
                pProvMngm->PackGroupUsedList.EndIdx                      = Prev;
            }
        }
        else
        {
            // this PackGroup is in the middle of the used list
            pProvMngm->pProvPackGroupEntry[Prev].UsedFreeLink.Next   = Next;
            pProvMngm->pProvPackGroupEntry[Next].UsedFreeLink.Prev   = Prev;
        }

        /* ------------------------------------------------------------ */
        /* Put this PackGroup to begin of free-chain list               */
        /* ------------------------------------------------------------ */
        if (pProvMngm->PackGroupFreeList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
        {
            // no PackGroup in free list
            pPackGroup->UsedFreeLink.Prev               = EDDP_CRT_CHAIN_IDX_END;
            pPackGroup->UsedFreeLink.Next               = EDDP_CRT_CHAIN_IDX_END;
            pProvMngm->PackGroupFreeList.BeginIdx    = PackGrId;
            pProvMngm->PackGroupFreeList.EndIdx      = PackGrId;
        }
        else
        {
            // one ore more PackGroup in free list
            pPackGroup->UsedFreeLink.Prev               = EDDP_CRT_CHAIN_IDX_END;
            pPackGroup->UsedFreeLink.Next               = pProvMngm->PackGroupFreeList.BeginIdx;
            pProvMngm->pProvPackGroupEntry[pProvMngm->PackGroupFreeList.BeginIdx].UsedFreeLink.Prev = PackGrId;
            pProvMngm->PackGroupFreeList.BeginIdx    = PackGrId;
        }

        EDDP_ASSERT_FALSE(pProvMngm->PackGroupUsedCnt == 0);
        pProvMngm->PackGroupUsedCnt--;
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvDfpPackGroupPassivate                           +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB                +*/
/*+                                                                                         +*/
/*+  Result                :    EDD_RSP                                                     +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: This function passivates all provider pack groups in CRT management:      +*/
/*+                 - start the Provider-Pack-Group-Passivate-All state machine             +*/
/*+                   only for ACWs, not for ACW_DGs                                        +*/
/*+                 - remove all provider pack groups from CRT management                   +*/
/*+                                                                                         +*/
/*+               If no provider pack group in CRT is present, nothing to do.               +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTProvDfpPackGroupPassivate(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP                             Response;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE     pProvMngm;

    Response    = EDD_STS_OK;
    pProvMngm   = &pDDB->CRT.Prov;

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProvDfpPackGroupPassivate(): PackGroupUsedCnt=%d", 
        pProvMngm->PackGroupUsedCnt);

    // are there some pack groups present in CRT ?
    if (pProvMngm->PackGroupUsedCnt > 0)
    {
        // start the Provider-Pack-Group-Passivate-All state machine for all pack groups
        EDDP_ASSERT_FALSE(pProvMngm->PackGroupPassivateAllSm.State != EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_IDLE);
        pProvMngm->PackGroupPassivateAllSm.State     = EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_START;
        pProvMngm->PackGroupPassivateAllSm.Cbf       = EDDP_CRTPassivatePackStateMachineCbf;
        EDDP_CRTProviderPackGroupPassivateAllStateMachine (pDDB, EDD_STS_OK);
        // set Response to PENDING
        Response = EDD_STS_OK_PENDING;
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProvDfpConnectToPackGroup                       +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                        +*/
/*+                             LSA_UINT16                  ProviderID                  +*/
/*+                             LSA_UINT8                   SFPosition                  +*/
/*+                                                                                     +*/
/*+  Result                :    EDD_RSP                                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  ProviderID         : ProviderID                                                    +*/
/*+  SFPosition         : SFPosition from provider service                              +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Search AcwDG with given SFPosition in PackGroupUsedList and connect   +*/
/*+               the given provider to this pack group.                                +*/
/*+                                                                                     +*/
/*+               The given SFPosition must valid, if no FATAL!                         +*/
/*+                                                                                     +*/
/*+               If no pack group is present in CRT, ERR_SEQUENCE.                     +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTProvDfpConnectToPackGroup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID,
    LSA_UINT8                   SFPosition)
{
    EDD_RSP                                     Response;
    LSA_UINT16                                  PackGrIdx, AcwDGIdx;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE             pProvMngm;
//  EDDP_CRT_PROVIDER_DDB_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE                  pProvider;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE              pAcw;
    EDDP_CRT_PROVIDER_ACW_DG_PTR_TYPE           pAcwDG;
    EDDP_CRT_PROVIDER_PACK_GROUP_PTR_TYPE       pPackGroup;

    Response        = EDD_STS_ERR_PARAM;
    pProvMngm    = &pDDB->CRT.Prov;
    pProvMngm    = &pDDB->CRT.Prov;
    pProvider       = &pProvMngm->pProvEntry[ProviderID];

    if (!(EDDP_CRT_IS_DFP_SFPOSITION_IN_VALID_RANGE(SFPosition)))
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpConnectToPackGroup(): FATAL -> The given SFPosition(%d) is out of range!", 
            SFPosition);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        // are there some pack groups present in CRT ?
        if (pProvMngm->PackGroupUsedCnt == 0)
        {
            Response = EDD_STS_ERR_SEQUENCE;
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTProvDfpConnectToPackGroup(): ERROR -> There is no pack groups in CRT management!");
        }
        else
        {
            if (pProvider->DfpParam.PackGrId == EDDP_CRT_CHAIN_IDX_END)
            {
                /* -------------------------------------------------------- */
                /* this provider is not yet connected to a pack group       */
                /* search in all PackGroupUsedList                          */
                /* -------------------------------------------------------- */
                // get first pack group from PackGroupUsedList
                PackGrIdx = pProvMngm->PackGroupUsedList.BeginIdx;

                // loop through all used pack groups from BeginIdx to EndIdx
                while (PackGrIdx != EDDP_CRT_CHAIN_IDX_END)
                {
                    EDDP_ASSERT_FALSE(PackGrIdx >= pDDB->CRT.CfgParam.ProviderPackGroupMaxNumber);

                    pPackGroup  = &pProvMngm->pProvPackGroupEntry[PackGrIdx];
                    EDDP_ASSERT_FALSE(pPackGroup->AcwId == EDDP_CRT_CHAIN_IDX_END);
                    pAcw        = &pProvMngm->pAcwEntry[pPackGroup->AcwId];

                    if (pProvider->AddParam.FrameID == pAcw->AddParam.FrameID)
                    {
                        // FrameID is found - now search ACW_DG with given SFPosition in this pack group
                        // loop through all ACW_DGs of this ACW
                        AcwDGIdx = pAcw->AcwDGList.BeginIdx;
                        
                        while (AcwDGIdx != EDDP_CRT_CHAIN_IDX_END)
                        {
                            EDDP_ASSERT_FALSE(AcwDGIdx >= pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber);

                            pAcwDG = &pProvMngm->pAcwDGEntry[AcwDGIdx];

                            if (SFPosition == pAcwDG->AddParam.DG_PosT)
                            {
                                // ACW_DG with given SFPosition is found
                                Response = EDD_STS_OK;

                                // connect provider to pack group, to ACW_DG and to ACW
                                pProvider->DfpParam.PackGrId    = PackGrIdx;
                                pProvider->DfpParam.AcwDGId     = pAcwDG->AcwDGId;
                                // add ACW to provider
                                EDDP_CRTProvAcwAddToProvider (pDDB, pProvider->ProviderId, pAcw->AcwId);
                                // only relevant for HERA: save DataOffset for later checks
                                pAcwDG->DfpParamCheck.DataOffset = pProvider->DfpParamCheck.DataOffset;

                                // exit while loop
                                AcwDGIdx = EDDP_CRT_CHAIN_IDX_END;
                            }
                            else
                            {
                                // next AcwDGIdx in this ACW
                                AcwDGIdx = pProvMngm->pAcwDGEntry[AcwDGIdx].AcwDGAcwLink.Next;
                            }
                        }
                        // exit while loop
                        PackGrIdx = EDDP_CRT_CHAIN_IDX_END;
                    }
                    else
                    {
                        // next pack group from PackGroupUsedList
                        PackGrIdx = pProvMngm->pProvPackGroupEntry[PackGrIdx].UsedFreeLink.Next;
                    }
                }
            }
            else
            {
                // this provider is already connected to a pack group
                EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProvDfpConnectToPackGroup(ProviderID=%d, SFPosition=%d): This provider is already connected to PackGrId(%d)", 
                    ProviderID, SFPosition, pProvider->DfpParam.PackGrId);
            }
        }
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProvDfpAcwDGControlDoneCbf                      +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             EDD_RSP                         Response                +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  Response       : Response                                                          +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Callbackfunction of DFP-Provider-Control: ACW_DG-Activate/Passivate.  +*/
/*+                                                                                     +*/
/*+               Finish the PROVIDER_CONTROL request.                                  +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvDfpAcwDGControlDoneCbf(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT16                  ProviderID,
	EDD_RSP                     Response)
{
    EDDP_LOCAL_HDB_PTR_TYPE                     pHDB;
    EDDP_GLB_CRT_RQB_HANDLER_PTR_TYPE           pRQBHandler;
    EDD_UPPER_CSRT_PROVIDER_CONTROL_PTR_TYPE    pRqbParam;
    EDD_UPPER_RQB_PTR_TYPE                      pRQB;

    pHDB        = LSA_NULL;
    pRQBHandler = &pDDB->GlbCrtRQBHandler;

    /* remove first RQB */
    pRQB = EDDP_GlbRQBQueueRemoveFromBegin(&pRQBHandler->RqbInProgressQueue);
    
    if (   (Response != EDD_STS_OK) 
        || (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL)) 
        || (EDD_SRV_SRT_PROVIDER_CONTROL != EDD_RQB_GET_SERVICE(pRQB))
       )
    {
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpAcwDGControlDoneCbf(Response=0x%X): FATAL -> INCONSISTENCE! No RQB (pRQB=0x%X) in work or wrong service detected!", 
            Response, pRQB);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        pRqbParam = (EDD_UPPER_CSRT_PROVIDER_CONTROL_PTR_TYPE) pRQB->pParam;

        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProvDfpAcwDGControlDoneCbf(): ProviderID=%d, Mode=0x%X", 
            pRqbParam->ProviderID, pRqbParam->Mode);

        if (pRqbParam->ProviderID != ProviderID)
        {
            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpAcwDGControlDoneCbf(): FATAL -> ProviderID does not match (ProviderID=%d, RQB-ProviderID=%d)!", 
                              ProviderID, pRqbParam->ProviderID);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        
        if ((pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE) && (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_PASSIVATE))
        {
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpAcwDGControlDoneCbf(): FATAL -> MODE_ACTIVATE and MODE_PASSIVATE!");
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }

        /* -------------------------------- */
        /* is Passivate requested ?         */
        /* -------------------------------- */
        if (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_PASSIVATE)
        {
            // is change DataStatus requested in CONTROL service ?
            if (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_STATUS)
            {
                // change provider DataStatus
                EDDP_CRTProviderChangeDataStatus (pDDB, pRqbParam->ProviderID, pRqbParam->Status, pRqbParam->Mask);
            }

            // is remove AutoStop requested in CONTROL service ?
            if (   (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_AUTOSTOP)
                && (pRqbParam->AutoStopConsumerID == EDD_CONSUMER_ID_REMOVE_AS)
               )
            {
                // delete the ACW_DG.Scoreboard_offset
                // note: distinction between PNIP_REV2 and PNIP_REV3 will be done via mask value in the function
                pDDB->CRT.Prov.AcwHandler.FctWriteSBOffset (pDDB, pRqbParam->ProviderID, PNIP_R2_ACW_SND_WORD3_SCOREBOARD_OFFSET_OFF_VALUE);
            }

            // disconnect the DFP-provider from pack group
            EDDP_CRTProvDfpDisconnectFromPackGroup (pDDB, pRqbParam->ProviderID);
        }

        /* ------------------------------------ */
        /* get Handle for this RQB              */
        /* ------------------------------------ */
        Response = EDDP_GlbHandleGetHDB (EDD_RQB_GET_HANDLE_LOWER(pRQB), &pHDB);

        if (Response != EDD_STS_OK)
        {
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "EDDP_CRTProvDfpAcwDGControlDoneCbf(): Invalid handle lower in RQB!");
            EDD_RQB_SET_RESPONSE(pRQB, EDD_STS_ERR_PARAM);
            EDDP_RQB_ERROR(pRQB);
        }

        /* ------------------------------------ */
        /* finish the PROVIDER_CONTROL request  */
        /* ------------------------------------ */
        EDDP_GlbRequestFinish (pHDB, pRQB, EDD_STS_OK);

        // trigger the finish events of this Service
        EDDP_GlbTriggerFinishEventAfterRQBConfirmation (pDDB, pHDB, EDD_SRV_SRT_PROVIDER_CONTROL);

        /* -------------------------------------------------------- */
        /* call RQB-Handler for other RQBs in Queue                 */
        /* -------------------------------------------------------- */
        EDDP_GlbCrtRQBHandler (pDDB);
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProvDfpAcwDGRemoveDoneCbf                       +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             EDD_RSP                         Response                +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  Response       : Response                                                          +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Callbackfunction of DFP-Provider-Remove: ACW_DG-Activate/Passivate.   +*/
/*+                                                                                     +*/
/*+               Remove provider from CRT management and                               +*/
/*+               finish the PROVIDER_REMOVE request.                                   +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvDfpAcwDGRemoveDoneCbf(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID,
	EDD_RSP                     Response)
{
    EDDP_LOCAL_HDB_PTR_TYPE                     pHDB;
    EDDP_GLB_CRT_RQB_HANDLER_PTR_TYPE           pRQBHandler;
    EDD_UPPER_CSRT_PROVIDER_REMOVE_PTR_TYPE     pRqbParam;
    EDD_UPPER_RQB_PTR_TYPE                      pRQB;

    pHDB        = LSA_NULL;
    pRQBHandler = &pDDB->GlbCrtRQBHandler;

    /* remove first RQB */
    pRQB = EDDP_GlbRQBQueueRemoveFromBegin(&pRQBHandler->RqbInProgressQueue);
    
    if (   (Response != EDD_STS_OK) 
        || (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL)) 
        || (EDD_SRV_SRT_PROVIDER_REMOVE != EDD_RQB_GET_SERVICE(pRQB))
       )
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpAcwDGRemoveDoneCbf(Response=0x%X): FATAL -> INCONSISTENCE! No RQB in work or wrong service detected!", 
            Response);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        pRqbParam = (EDD_UPPER_CSRT_PROVIDER_REMOVE_PTR_TYPE) pRQB->pParam;

        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProvDfpAcwDGRemoveDoneCbf(): ProviderID=%d", 
            pRqbParam->ProviderID);

        if (pRqbParam->ProviderID != ProviderID)
        {
            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvDfpAcwDGRemoveDoneCbf(): FATAL -> ProviderID does not match (ProviderID=%d, RQB-ProviderID=%d)!", 
                ProviderID, pRqbParam->ProviderID);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        
        // disconnect DFP-Provider from pack group
        EDDP_CRTProvDfpDisconnectFromPackGroup (pDDB, pRqbParam->ProviderID);
        // remove DFP-Provider from CRT management
        EDDP_CRTProviderRemoveFromCRTMngm (pDDB, pRqbParam->ProviderID);

        /* ------------------------------------ */
        /* get Handle for this RQB              */
        /* ------------------------------------ */
        Response = EDDP_GlbHandleGetHDB (EDD_RQB_GET_HANDLE_LOWER(pRQB), &pHDB);

        if (Response != EDD_STS_OK)
        {
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "EDDP_CRTProvDfpAcwDGRemoveDoneCbf(): Invalid handle lower in RQB!");
            EDD_RQB_SET_RESPONSE(pRQB, EDD_STS_ERR_PARAM);
            EDDP_RQB_ERROR(pRQB);
        }

        /* ------------------------------------ */
        /* finish the PROVIDER_REMOVE request  */
        /* ------------------------------------ */
        EDDP_GlbRequestFinish (pHDB, pRQB, EDD_STS_OK);

        // trigger the finish events of this Service
        EDDP_GlbTriggerFinishEventAfterRQBConfirmation (pDDB, pHDB, EDD_SRV_SRT_PROVIDER_REMOVE);

        /* -------------------------------------------------------- */
        /* call RQB-Handler for other RQBs in Queue                 */
        /* -------------------------------------------------------- */
        EDDP_GlbCrtRQBHandler (pDDB);
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvDfpDisconnectFromPackGroup                      +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             LSA_UINT16                          ProviderID              +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ProviderID         : Provider to be removed                                            +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: This function disconnects the given DFP-Provider from its pack group.     +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvDfpDisconnectFromPackGroup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID)
{
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE              pProvider;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;

    EDDP_ASSERT_FALSE(ProviderID >= pDDB->CRT.CfgParam.ProviderMaxNumber);

    pProvMngm   = &pDDB->CRT.Prov;
    pProvider   = &pProvMngm->pProvEntry[ProviderID];

    // check the given provider is DFP
    EDDP_ASSERT_FALSE( !(pProvider->ProviderFlags & EDDP_CRT_PROV_FLAGS_IS_DFP) );
    // check the given provider has ACW
    EDDP_ASSERT_FALSE(pProvider->AcwList.BeginIdx == EDDP_CRT_CHAIN_IDX_END);
    pAcw = &pProvMngm->pAcwEntry[pProvider->AcwList.BeginIdx];
    // check the given provider has ACW_DG
    EDDP_ASSERT_FALSE(pProvider->DfpParam.AcwDGId == EDDP_CRT_CHAIN_IDX_END);

    // this provider has no more connection to a pack group and to ACW_DG
    pProvider->DfpParam.PackGrId    = EDDP_CRT_CHAIN_IDX_END;
    pProvider->DfpParam.AcwDGId     = EDDP_CRT_CHAIN_IDX_END;

    // remove ACW from provider
    EDDP_CRTProvAcwRemoveFromProvider (pDDB, ProviderID, pAcw->AcwId);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvDfpGetAcwIDFromInBoundPackGroupFrameID          +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                             LSA_UINT16                  FrameID                         +*/
/*+                             LSA_BOOL                    isFIDRedundantSet               +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_UINT16                                                  +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  FrameID            : FrameID for searching                                             +*/
/*+  isFIDRedundantSet  : LSA_TRUE, if "InBoundPack_x.FID_redundant" is set.                +*/
/*+                         -> 1 redundant InBound-Pack and 2 Send-ACWs on PPM.             +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Get AcwIdx of ACW that is connected to a PackGroup with given FrameID.    +*/
/*+               PackGroup is connected to ACW, so search the given FrameID in ACW         +*/
/*+               of this PackGroup.                                                        +*/
/*+                                                                                         +*/
/*+               InBound-PackGroups are stored in pProvPackGroupEntry[x].                  +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTProvDfpGetAcwIDFromInBoundPackGroupFrameID(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  FrameID,
    LSA_BOOL                    isFIDRedundantSet)
{
    LSA_UINT16                                          AcwId;
    LSA_UINT16                                          PackGrIdx;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE                     pProvMngm;
//  EDDP_CRT_PROVIDER_DDB_MNGM_PTR_TYPE                 pProvMngm;
    EDDP_CRT_PROVIDER_PACK_GROUP_PTR_TYPE               pPackGroup;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE                      pAcw;
    EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_PTR_TYPE    pSubframeBlock;

    EDDP_ASSERT_FALSE(EDDP_HAL_HW_PNIP_REV1 != EDDP_HALGetPnipRevision(pDDB));

    pProvMngm    = &pDDB->CRT.Prov;
    pProvMngm    = &pDDB->CRT.Prov;
    AcwId           = EDDP_CRT_CHAIN_IDX_END;

    // are there some PackGroups in CRT management ?
    if (pProvMngm->PackGroupUsedCnt > 0)
    {
        PackGrIdx = pProvMngm->PackGroupUsedList.BeginIdx;

        while (PackGrIdx != EDDP_CRT_CHAIN_IDX_END)
        {
            // pointer to this PackGroup
            EDDP_ASSERT_FALSE(PackGrIdx >= pDDB->CRT.CfgParam.ProviderPackGroupMaxNumber);
            pPackGroup  = &pProvMngm->pProvPackGroupEntry[PackGrIdx];
            // pointer to ACW of this PackGroup
            EDDP_ASSERT_FALSE(pPackGroup->AcwId == EDDP_CRT_CHAIN_IDX_END);
            pAcw        = &pProvMngm->pAcwEntry[pPackGroup->AcwId];
            // pointer to SubframeBlock of this ACW
            EDDP_ASSERT_FALSE(pAcw->DfpParam.PrmSubframeBlockIdx >= pDDB->PRM.pPDIRSubframeData_A->NumberOfSubframeBlocks);
            pSubframeBlock = &pDDB->PRM.pPDIRSubframeData_A->SubframeBlockArray[pAcw->DfpParam.PrmSubframeBlockIdx];

            if (isFIDRedundantSet)
            {
                /* ---------------------------------------- */
                /* InBoundPack_x.FID_redundant = 1          */
                /* ---------------------------------------- */
                EDDP_ASSERT_FALSE(!(EDDP_IS_IRT_REDUNDANT_FRAMEID(FrameID)));

                // the given FrameID is redundant FrameID(x) - search the Send-ACW with this FrameID(x+1) and ACW.InBound_Pack=1
                if ( ((FrameID+1) == pAcw->AddParam.FrameID) && (pSubframeBlock->HwConfig & EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_SND_ACW_INBOUND_PACK_MASK))
                {
                    // FrameID is found - return the AcwId and exit loop
                    AcwId       = pAcw->AcwId;
                    PackGrIdx   = EDDP_CRT_CHAIN_IDX_END;
                }
                else
                {
                    // next PackGroup
                    PackGrIdx   = pPackGroup->UsedFreeLink.Next;
                }
            }
            else
            {
                /* ---------------------------------------- */
                /* InBoundPack_x.FID_redundant = 0          */
                /* ---------------------------------------- */
                // the same FrameID and this InBound-PackGroup has Send-ACW with ACW.InBound_Pack=1
                if ((FrameID == pAcw->AddParam.FrameID) && (pSubframeBlock->HwConfig & EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_SND_ACW_INBOUND_PACK_MASK))
                {
                    // FrameID is found - return the AcwId and exit loop
                    AcwId       = pAcw->AcwId;
                    PackGrIdx   = EDDP_CRT_CHAIN_IDX_END;
                }
                else
                {
                    // next PackGroup
                    PackGrIdx   = pPackGroup->UsedFreeLink.Next;
                }
            }
        }
    }

    return (AcwId);
}
#endif
