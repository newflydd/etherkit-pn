/** @file eddp_prm_dfp.c 
 */
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
/*  F i l e               &F: eddp_prm_dfp.c                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP PRM (DFP) handling                          */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  21.09.10    VS    initial version.                                       */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID   93
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_PRM_DFP */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"            /* edd headerfiles */
#include "eddp_int.h"            /* internal header */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID) 





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMPDIRSubframeDataRecord_Write        +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE      pDDB           +*/
/*+                             EDD_UPPER_MEM_U8_PTR_TYPE    pRecord        +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pRecord    : Pointer to PRM record                                     +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PRM_BLOCK                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function parses the PDIRSubframeData record (pRecord)+*/
/*+               and stores the extracted data into DDB.                   +*/
/*+               Moreover, the original record is stored in record set B.  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRSubframeDataRecord_Write(
   	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
   	EDD_UPPER_MEM_U8_PTR_TYPE   pRecord)
{
    EDD_RSP                                             Response;
    LSA_UINT16                                          val_16;
    LSA_UINT32                                          val_32;
    LSA_UINT32                                          iSubfrBlock, iSubfrData;
    LSA_UINT32                                          SFIOCRProperties;
    LSA_UINT32                                          record_length;
    LSA_UINT8                                           BlockVersionLow;
    LSA_UINT8                                           BlockVersionHigh;
    EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_PTR_TYPE    pSubframeBlock;
    EDD_UPPER_MEM_U8_PTR_TYPE                           pRecord_Begin;
    EDD_UPPER_MEM_U8_PTR_TYPE                           pRecord_Tmp;
    EDD_UPPER_MEM_U8_PTR_TYPE                           pRecord_RecordLength;
    EDDP_PRM_DETAIL_ERR_PTR_TYPE                        pDetailErr;

    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_PRMPDIRSubframeDataRecord_Write()");

    Response                = EDD_STS_OK;
    pDetailErr              = &pDDB->PrmDetailErr;
    pRecord_Begin           = pRecord;
    record_length           = 0;
    pRecord_RecordLength    = pRecord;

    /* -------------------------------- */
    /* PDIRSubframeData                 */
    /* -------------------------------- */
    // check BlockType for PDIRSubframeData
    pRecord_Tmp = pRecord;
    EDDP_GET_U16_INCR(pRecord, val_16);
    if (val_16 != EDDP_PRM_PDIRSUBFRAMEDATA_BLOCKTYPE)
    {
        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
        Response = EDD_STS_ERR_PRM_BLOCK;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataRecord_Write(): ERROR -> PDIRSubframeData.BlockType(0x%X) != EDDP_PRM_PDIRSUBFRAMEDATA_BLOCKTYPE(0x%X)!", 
            val_16, EDDP_PRM_PDIRSUBFRAMEDATA_BLOCKTYPE);
    }
    else
    {
        // check BlockLength of PDIRSubframeData
        pRecord_RecordLength = pRecord;
        pRecord_Tmp = pRecord;
        EDDP_GET_U16_INCR(pRecord, val_16);
        record_length = (val_16 + 4);
        if (record_length > pDDB->PRM.pRecordsetB->PDIRSubframeData_Length)
        {
            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
            Response = EDD_STS_ERR_PRM_BLOCK;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataRecord_Write(): ERROR -> record_length(%d) > PDIRSubframeData_Length(%d)!", 
                record_length, pDDB->PRM.pRecordsetB->PDIRSubframeData_Length);
        }
        else
        {
            // check BlockVersionHigh of PDIRSubframeData
            pRecord_Tmp = pRecord;
            BlockVersionHigh = *pRecord++;
            if (BlockVersionHigh != EDDP_PRM_PDIRSUBFRAMEDATA_BLOCKVERSIONHIGH)
            {
                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataRecord_Write(): ERROR -> PDIRSubframeData.BlockVersionHigh(0x%X) != EDDP_PRM_PDIRSUBFRAMEDATA_BLOCKVERSIONHIGH(0x%X)!", 
                    BlockVersionHigh, EDDP_PRM_PDIRSUBFRAMEDATA_BLOCKVERSIONHIGH);
            }
            else
            {
                // check BlockVersionLow of PDIRSubframeData
                pRecord_Tmp = pRecord;
                BlockVersionLow = *pRecord++;
                if (BlockVersionLow != EDDP_PRM_PDIRSUBFRAMEDATA_BLOCKVERSIONLOW)
                {
                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                    Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataRecord_Write(): ERROR -> PDIRSubframeData.BlockVersionLow(0x%X) != EDDP_PRM_PDIRSUBFRAMEDATA_BLOCKVERSIONLOW(0x%X)!", 
                        BlockVersionLow, EDDP_PRM_PDIRSUBFRAMEDATA_BLOCKVERSIONLOW);
                }
            }
        }
    }

    /* -------------------------------- */
    /* SubframeBlock (pack group)       */
    /* -------------------------------- */
    if (Response == EDD_STS_OK)
    {
        // store NumberOfSubframeBlocks
        pRecord_Tmp = pRecord;
        EDDP_GET_U16_INCR(pRecord, val_16);
        pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks = val_16;

        if ( !(EDDP_PRM_IS_DFP_NUMBER_OF_SUBFRAMEBLOCKS_IN_VALID_RANGE(pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks)) )
        {
            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
            Response = EDD_STS_ERR_PRM_BLOCK;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataRecord_Write(): ERROR -> NumberOfSubframeBlocks(%d) is out of range [%d...%d]", 
                pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks, EDDP_PRM_PDIRSUBFRAMEDATA_MIN_NUMBER_OF_SUBFRAMEBLOCKS, EDDP_PRM_PDIRSUBFRAMEDATA_MAX_NUMBER_OF_SUBFRAMEBLOCKS);
        }
        else
        {
            for ( iSubfrBlock=0; iSubfrBlock < pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks; iSubfrBlock++ )
            {
                pSubframeBlock = &pDDB->PRM.pPDIRSubframeData_B->SubframeBlockArray[iSubfrBlock];

                // check BlockType for SubframeBlock
                pRecord_Tmp = pRecord;
                EDDP_GET_U16_INCR(pRecord, val_16);
                if (val_16 != EDDP_PRM_PDIRSUBFRAMEBLOCK_BLOCKTYPE)
                {
                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                    Response = EDD_STS_ERR_PRM_BLOCK;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataRecord_Write(): ERROR -> SubframeBlock.BlockType(0x%X) != EDDP_PRM_PDIRSUBFRAMEBLOCK_BLOCKTYPE(0x%X)! iSubfrBlock(%d)", 
                        val_16, EDDP_PRM_PDIRSUBFRAMEBLOCK_BLOCKTYPE, iSubfrBlock);
                    // exit FOR loop (SubframeBlock)
                    iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                }
                else
                {
                    // BlockLength of SubframeBlock
                    pRecord_Tmp = pRecord;
                    EDDP_GET_U16_INCR(pRecord, val_16);
                    // NumberOfSubframeData = ( BlockLength - BlockVersionHigh(1) - BlockVersionLow(1) - FrameID(2) - SFIOCRProperties(4) ) / SubframeData(4)
                    pSubframeBlock->NumberOfSubframeData = (val_16 - 1 - 1 - 2 - 4) / 4;

                    if ( !(EDDP_PRM_IS_DFP_NUMBER_OF_SUBFRAMEDATA_IN_VALID_RANGE(pSubframeBlock->NumberOfSubframeData)) )
                    {
                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                        Response = EDD_STS_ERR_PRM_BLOCK;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_PRM_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataRecord_Write(): ERROR -> NumberOfSubframeData(%d) is out of range [%d...%d]! iSubfrBlock(%d)", 
                            pSubframeBlock->NumberOfSubframeData, EDDP_PRM_PDIRSUBFRAMEDATA_MIN_NUMBER_OF_SUBFRAMEDATA, EDDP_PRM_PDIRSUBFRAMEDATA_MAX_NUMBER_OF_SUBFRAMEDATA, iSubfrBlock);
                        // exit FOR loop (SubframeBlock)
                        iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                    }
                    else
                    {
                        // check BlockVersionHigh of SubframeBlock
                        pRecord_Tmp = pRecord;
                        BlockVersionHigh = *pRecord++;
                        if (BlockVersionHigh != EDDP_PRM_PDIRSUBFRAMEBLOCK_BLOCKVERSIONHIGH)
                        {
                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                            Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataRecord_Write(): ERROR -> SubframeBlock.BlockVersionHigh(0x%X) != EDDP_PRM_PDIRSUBFRAMEBLOCK_BLOCKVERSIONHIGH(0x%X)! iSubfrBlock(%d)", 
                                BlockVersionHigh, EDDP_PRM_PDIRSUBFRAMEBLOCK_BLOCKVERSIONHIGH, iSubfrBlock);
                            // exit FOR loop (SubframeBlock)
                            iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                        }
                        else
                        {
                            // check BlockVersionLow of SubframeBlock
                            pRecord_Tmp = pRecord;
                            BlockVersionLow = *pRecord++;
                            if (BlockVersionLow != EDDP_PRM_PDIRSUBFRAMEBLOCK_BLOCKVERSIONLOW)
                            {
                                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataRecord_Write(): ERROR -> SubframeBlock.BlockVersionLow(0x%X) != EDDP_PRM_PDIRSUBFRAMEBLOCK_BLOCKVERSIONLOW(0x%X)! iSubfrBlock(%d)", 
                                    BlockVersionLow, EDDP_PRM_PDIRSUBFRAMEBLOCK_BLOCKVERSIONLOW, iSubfrBlock);
                                // exit FOR loop (SubframeBlock)
                                iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                            }
                            else
                            {
                                // store FrameID
                                pRecord_Tmp = pRecord;
                                EDDP_GET_U16_INCR(pRecord, val_16);
                                pSubframeBlock->FrameID = val_16;

                                // only IRT FrameID in SubframeBlock allowed
                                if (!(EDDP_IS_IRT_FRAMEID(pSubframeBlock->FrameID)))
                                {
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataRecord_Write(): ERROR -> PDIRSubframeData.FrameID(0x%X) is not IRT FrameID!", 
                                        pSubframeBlock->FrameID);
                                    // exit FOR loop (SubframeBlock)
                                    iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                                }
                                else
                                {
                                    // for redundant FrameID only FrameID(x) allowed
                                    if ((EDDP_IS_IRT_REDUNDANT_FRAMEID(pSubframeBlock->FrameID)) && (pSubframeBlock->FrameID & 0x1))
                                    {
                                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                        Response = EDD_STS_ERR_PRM_BLOCK;
                                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataRecord_Write(): ERROR -> PDIRSubframeData.FrameID(0x%X) is redundant but not even number!", 
                                            pSubframeBlock->FrameID);
                                        // exit FOR loop (SubframeBlock)
                                        iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                                    }
                                }

                                // get SFIOCRProperties
                                pRecord_Tmp = pRecord;
                                EDDP_GET_U32_INCR(pRecord, SFIOCRProperties);

                                EDDP_PRM_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMPDIRSubframeDataRecord_Write(): NumberOfSubframeBlocks(%d), FrameID(0x%X), SFIOCRProperties(0x%X), NumberOfSubframeData(%d), iSubfrBlock(%d)", 
                                    pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks, 
                                    pSubframeBlock->FrameID, 
                                    SFIOCRProperties, 
                                    pSubframeBlock->NumberOfSubframeData, 
                                    iSubfrBlock);

                                // store SFIOCRProperties.DistributedWatchDogFactor
                                pSubframeBlock->SFIOCRProperties.DistributedWatchDogFactor = 
                                    EDDP_GET_BIT_VALUE( SFIOCRProperties,
                                                        EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DIST_WD_FACTOR_MASK, 
                                                        EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DIST_WD_FACTOR_SHIFT);
                                if ( !(EDDP_PRM_IS_DFP_DISTRIBUTEDWATCHDOGFACTOR_IN_VALID_RANGE(pSubframeBlock->SFIOCRProperties.DistributedWatchDogFactor)) )
                                {
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataRecord_Write(): ERROR -> In SFIOCRProperties(0x%X) the DistributedWatchDogFactor(%d) is out of range [%d...%d]! iSubfrBlock(%d)", 
                                        SFIOCRProperties, 
                                        pSubframeBlock->SFIOCRProperties.DistributedWatchDogFactor, 
                                        EDDP_PRM_PDIRSUBFRAMEDATA_MIN_DISTRIBUTEDWATCHDOGFACTOR, 
                                        EDDP_PRM_PDIRSUBFRAMEDATA_MAX_DISTRIBUTEDWATCHDOGFACTOR, 
                                        iSubfrBlock);
                                    // exit FOR loop (SubframeBlock)
                                    iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                                }

                                // store SFIOCRProperties.RestartFactorForDistributedWD
                                pSubframeBlock->SFIOCRProperties.RestartFactorForDistributedWD = 
                                    EDDP_GET_BIT_VALUE( SFIOCRProperties,
                                                        EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_REST_FACTOR_DIST_WD_MASK, 
                                                        EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_REST_FACTOR_DIST_WD_SHIFT);
                                if ( !(EDDP_PRM_IS_DFP_RESTARTFACTORFORDISTRIBUTEDWD_IN_VALID_RANGE(pSubframeBlock->SFIOCRProperties.RestartFactorForDistributedWD)) )
                                {
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataRecord_Write(): ERROR -> In SFIOCRProperties(0x%X) the RestartFactorForDistributedWD(%d) is out of range [%d...%d]! iSubfrBlock(%d)", 
                                        SFIOCRProperties, 
                                        pSubframeBlock->SFIOCRProperties.RestartFactorForDistributedWD, 
                                        EDDP_PRM_PDIRSUBFRAMEDATA_MIN_RESTARTFACTORFORDISTRIBUTEDWD, 
                                        EDDP_PRM_PDIRSUBFRAMEDATA_MAX_RESTARTFACTORFORDISTRIBUTEDWD, 
                                        iSubfrBlock);
                                    // exit FOR loop (SubframeBlock)
                                    iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                                }
                                else
                                {
                                    if (pSubframeBlock->SFIOCRProperties.DistributedWatchDogFactor == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DIST_WD_FACTOR_OFF)
                                    {
                                        // DistributedWatchDogFactor is OFF - RestartFactorForDistributedWD must be OFF too
                                        if (pSubframeBlock->SFIOCRProperties.RestartFactorForDistributedWD != EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_REST_FACTOR_DIST_WD_OFF)
                                        {
                                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                            Response = EDD_STS_ERR_PRM_BLOCK;
                                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                            EDDP_PRM_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataRecord_Write(): ERROR -> In SFIOCRProperties(0x%X) the combination of DistributedWatchDogFactor(%d) and RestartFactorForDistributedWD(%d) is not permitted! iSubfrBlock(%d)", 
                                                SFIOCRProperties, 
                                                pSubframeBlock->SFIOCRProperties.DistributedWatchDogFactor, 
                                                pSubframeBlock->SFIOCRProperties.RestartFactorForDistributedWD, 
                                                iSubfrBlock);
                                            // exit FOR loop (SubframeBlock)
                                            iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                                        }
                                    }
                                }

                                // store SFIOCRProperties.DFPmode
                                pSubframeBlock->SFIOCRProperties.DFPmode = 
                                    EDDP_GET_BIT_VALUE( SFIOCRProperties,
                                                        EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPMODE_MASK, 
                                                        EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPMODE_SHIFT);

                                // store SFIOCRProperties.DFPDirection
                                pSubframeBlock->SFIOCRProperties.DFPDirection = 
                                    EDDP_GET_BIT_VALUE( SFIOCRProperties,
                                                        EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPDIRECTION_MASK, 
                                                        EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPDIRECTION_SHIFT);
                                if (   (pSubframeBlock->SFIOCRProperties.DFPDirection != EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPDIRECTION_INBOUND)
                                    && (pSubframeBlock->SFIOCRProperties.DFPDirection != EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPDIRECTION_OUTBOUND)
                                   )
                                {
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataRecord_Write(): ERROR -> In SFIOCRProperties(0x%X) the DFPDirection(0x%X) is unknown! iSubfrBlock(%d)", 
                                        SFIOCRProperties, pSubframeBlock->SFIOCRProperties.DFPDirection, iSubfrBlock);
                                    // exit FOR loop (SubframeBlock)
                                    iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                                }

                                // store SFIOCRProperties.DFPRedundantPathLayout
                                pSubframeBlock->SFIOCRProperties.DFPRedundantPathLayout = 
                                    EDDP_GET_BIT_VALUE( SFIOCRProperties,
                                                        EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_MASK, 
                                                        EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_SHIFT);
                                if (   (pSubframeBlock->SFIOCRProperties.DFPRedundantPathLayout != EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_HIGH)
#ifndef EDDP_DONT_USE_MRPD_WITH_DFP
                                    && (pSubframeBlock->SFIOCRProperties.DFPRedundantPathLayout != EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_DOWN)
#endif
                                   )
                                {
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataRecord_Write(): ERROR -> In SFIOCRProperties(0x%X) the DFPRedundantPathLayout(0x%X) is unknown! iSubfrBlock(%d)", 
                                        SFIOCRProperties, pSubframeBlock->SFIOCRProperties.DFPRedundantPathLayout, iSubfrBlock);
                                    // exit FOR loop (SubframeBlock)
                                    iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                                }

                                // store SFIOCRProperties.SFCRC16
                                pSubframeBlock->SFIOCRProperties.SFCRC16 = 
                                EDDP_GET_BIT_VALUE( SFIOCRProperties,
                                                    EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_SFCRC16_MASK, 
                                                    EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_SFCRC16_SHIFT);

                                #ifdef EDDP_MESSAGE
                                #ifdef EDDP_PRAGMA_MESSAGE
                                #pragma EDDP_PRAGMA_MESSAGE("Workaround deactivate outbound frame cut for PNIP-Revision(1) - AP01556511")
                                #endif /* EDDP_PRAGMA_MESSAGE */
                                #endif /* EDDP_MESSAGE */
                                #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
                                if (   (EDDP_HAL_HW_PNIP_REV1 == EDDP_HALGetPnipRevision(pDDB))
                                    && (EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPDIRECTION_OUTBOUND == pSubframeBlock->SFIOCRProperties.DFPDirection)
                                   )
                                {
                                    if (pSubframeBlock->SFIOCRProperties.SFCRC16 != EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_SFCRC16_0)
                                    {
                                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                        Response = EDD_STS_ERR_PRM_BLOCK;
                                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                        EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataRecord_Write(): ERROR -> In SFIOCRProperties(0x%X) the SFCRC16(0x%X) is unknown or activate for PNIP-Revision(1)! iSubfrBlock(%d)", 
                                            SFIOCRProperties, pSubframeBlock->SFIOCRProperties.SFCRC16, iSubfrBlock);
                                        // exit FOR loop (SubframeBlock)
                                        iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                                    }
                                }
                                else
                                {
                                #endif
                                if (   (pSubframeBlock->SFIOCRProperties.SFCRC16 != EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_SFCRC16_0)
                                    && (pSubframeBlock->SFIOCRProperties.SFCRC16 != EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_SFCRC16_1)
                                   )
                                {
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataRecord_Write(): ERROR -> In SFIOCRProperties(0x%X) the SFCRC16(0x%X) is unknown! iSubfrBlock(%d)", 
                                        SFIOCRProperties, pSubframeBlock->SFIOCRProperties.SFCRC16, iSubfrBlock);
                                    // exit FOR loop (SubframeBlock)
                                    iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                                }
                                #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
                                }
                                #endif

                                /* -------------------------------- */
                                /* SubframeData (array of all DGs)  */
                                /* -------------------------------- */
                                if (Response == EDD_STS_OK)
                                {
                                    for ( iSubfrData=0; iSubfrData < pSubframeBlock->NumberOfSubframeData; iSubfrData++ )
                                    {
                                        // get SubframeData
                                        pRecord_Tmp = pRecord;
                                        EDDP_GET_U32_INCR(pRecord, val_32);

                                        // store SubframeData.Position
                                        pSubframeBlock->SubframeDataArray[iSubfrData].Position = 
                                            EDDP_GET_BIT_VALUE( val_32,
                                                                EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SUBFRAMEDATA_POSITION_MASK, 
                                                                EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SUBFRAMEDATA_POSITION_SHIFT);
                                        // following checking rule is dependent of DFPmode
                                        if (pSubframeBlock->SFIOCRProperties.DFPmode == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_DFPMODE_END_NODE)
                                        {
                                            // IO-Controller
                                            if ( !(EDDP_PRM_IS_DFP_POSITION_IN_VALID_RANGE_IOC(pSubframeBlock->SubframeDataArray[iSubfrData].Position)) )
                                            {
                                                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                                Response = EDD_STS_ERR_PRM_BLOCK;
                                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                                EDDP_PRM_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataRecord_Write(): ERROR -> SubframeData.Position(%d) for IOC is out of range [%d...%d]! iSubfrBlock(%d), iSubfrData(%d)", 
                                                    pSubframeBlock->SubframeDataArray[iSubfrData].Position, EDDP_PRM_PDIRSUBFRAMEDATA_MIN_POSITION, EDDP_PRM_PDIRSUBFRAMEDATA_MAX_POSITION_IOC, iSubfrBlock, iSubfrData);
                                                // exit FOR loop (SubframeBlock)
                                                iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                                                // exit FOR loop (SubframeData)
                                                iSubfrData  = pSubframeBlock->NumberOfSubframeData;
                                            }
                                        }
                                        else
                                        {
                                            // IO-Device
                                            if ( !(EDDP_PRM_IS_DFP_POSITION_IN_VALID_RANGE_IOD(pSubframeBlock->SubframeDataArray[iSubfrData].Position)) )
                                            {
                                                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                                Response = EDD_STS_ERR_PRM_BLOCK;
                                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                                EDDP_PRM_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataRecord_Write(): ERROR -> SubframeData.Position(%d) for IOD is out of range [%d...%d]! iSubfrBlock(%d), iSubfrData(%d)", 
                                                    pSubframeBlock->SubframeDataArray[iSubfrData].Position, EDDP_PRM_PDIRSUBFRAMEDATA_MIN_POSITION, EDDP_PRM_PDIRSUBFRAMEDATA_MAX_POSITION_IOD, iSubfrBlock, iSubfrData);
                                                // exit FOR loop (SubframeBlock)
                                                iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                                                // exit FOR loop (SubframeData)
                                                iSubfrData  = pSubframeBlock->NumberOfSubframeData;
                                            }
                                        }

                                        // store SubframeData.DataLength
                                        pSubframeBlock->SubframeDataArray[iSubfrData].DataLength = 
                                            EDDP_GET_BIT_VALUE( val_32,
                                                                EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SUBFRAMEDATA_DATALENGTH_MASK, 
                                                                EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SUBFRAMEDATA_DATALENGTH_SHIFT);
                                        if ( !(EDDP_PRM_IS_DFP_DATALENGTH_IN_VALID_RANGE(pSubframeBlock->SubframeDataArray[iSubfrData].DataLength)) )
                                        {
                                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                            Response = EDD_STS_ERR_PRM_BLOCK;
                                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                            EDDP_PRM_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataRecord_Write(): ERROR -> SubframeData.DataLength(%d) is out of range [%d...%d]! iSubfrBlock(%d), iSubfrData(%d)", 
                                                pSubframeBlock->SubframeDataArray[iSubfrData].DataLength, EDDP_PRM_PDIRSUBFRAMEDATA_MIN_DATALENGTH, EDDP_PRM_PDIRSUBFRAMEDATA_MAX_DATALENGTH, iSubfrBlock, iSubfrData);
                                            // exit FOR loop (SubframeBlock)
                                            iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                                            // exit FOR loop (SubframeData)
                                            iSubfrData  = pSubframeBlock->NumberOfSubframeData;
                                        }

                                        // calculate and store SubframeData.Offset
                                        if (Response == EDD_STS_OK)
                                        {
                                            if (iSubfrData == 0)
                                            {
                                                // first Subframe
                                                // SFHeader(2) + SFPosition(1) + SFDataLength(1) + SFCycleCounter(1) + DataStatusSubframe(1)
                                                pSubframeBlock->SubframeDataArray[iSubfrData].Offset = 2 + 1 + 1 + 1 + 1;
                                            }
                                            else
                                            {
                                                // other Subframes
                                                //  last Subframe       [ Offset + IOData(SFDataLength) + SFCRC16(2) ]
                                                //  + current Subframe  [ SFPosition(1) + SFDataLength(1) + SFCycleCounter(1) + DataStatusSubframe(1) ]
                                                pSubframeBlock->SubframeDataArray[iSubfrData].Offset =
                                                    pSubframeBlock->SubframeDataArray[iSubfrData-1].Offset + pSubframeBlock->SubframeDataArray[iSubfrData-1].DataLength + 2
                                                    + 1 + 1 + 1 + 1;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (Response == EDD_STS_OK)
    {
        // check if number of read bytes is same as record length
        if ( ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin) != record_length )
        {
            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_RecordLength - pRecord_Begin);
            Response = EDD_STS_ERR_PRM_BLOCK; 
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataRecord_Write(): ERROR -> number of read bytes(%d) != record_length(%d)!", 
                ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin), record_length);
        }
    }

    if (Response == EDD_STS_OK)
    {
        // store record in record set B
        EDDP_MEMCOPY_COMMON(pDDB->PRM.pRecordsetB->pPDIRSubframeData, pRecord_Begin, record_length);
        pDDB->PRM.pRecordsetB->PDIRSubframeData_Length        = record_length;
        pDDB->PRM.pRecordsetB->PDIRSubframeData_IsPresent     = LSA_TRUE;
    }

    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMPDIRSubframeDataRecord_Write(): Response=0x%X", Response);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRSubframeDataIsDfpPresent                    +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_BOOL                        InActiveRecord_A        +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_BOOL                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  InActiveRecord_A   : LSA_TRUE  - search in active record buffer (A)                +*/
/*+                       LSA_FALSE - search in new record buffer (B)                   +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function gets info whether the DFP is present.                   +*/
/*+               Are PDIRData and PDIRSubframeData present?                            +*/
/*+                                                                                     +*/
/*+               Return LSA_TRUE  : PDIRSubframeData is present                        +*/
/*+                      LSA_FALSE : PDIRSubframeData is not present                    +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRSubframeDataIsDfpPresent(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_BOOL                        InActiveRecord_A)
{
    LSA_BOOL        State = LSA_FALSE;

    if (InActiveRecord_A)
    {
        // search in active record buffer A - are PDIRData_A and PDIRSubframeData_A present ?
        if ((pDDB->PRM.pRecordsetA->PDIRData_IsPresent) && (pDDB->PRM.pRecordsetA->PDIRSubframeData_IsPresent))
            State = LSA_TRUE;
    }
    else
    {
        // search in active record buffer B - are PDIRData_B and PDIRSubframeData_B present ?
        if ((pDDB->PRM.pRecordsetB->PDIRData_IsPresent) && (pDDB->PRM.pRecordsetB->PDIRSubframeData_IsPresent))
            State = LSA_TRUE;
    }

    return (State);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRSubframeDataCheckAndValidate                +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                                                                                     +*/
/*+  Result                :    EDD_RSP                                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks and validates the PDIRSubframeData record        +*/
/*+               against the PDIRData record.                                          +*/
/*+                                                                                     +*/
/*+               If all checks are ok, chain the FrameIDs from PDIRSubframeData with   +*/
/*+               FrameIDs in PDIRData.                                                 +*/
/*+                                                                                     +*/
/*+               Call this function only if both records (PDIRData and PDIRSubframeData)+*/
/*+               are present in RecordsetB!                                            +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRSubframeDataCheckAndValidate(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP                                             Response;
    EDDP_PRM_DETAIL_ERR_PTR_TYPE                        pDetailErr;
    LSA_UINT32                                          iSubfrBlock, iPDIRFrData;
    LSA_UINT32                                          TxPortCount;
    LSA_UINT32                                          InBound_PackCtrl_Cnt, OutBound_PackCtrl_Cnt;
    EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE                     pPDIRFrameData;
    EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_PTR_TYPE    pSubframeBlock;

    Response                = EDD_STS_OK;
    pDetailErr              = &pDDB->PrmDetailErr;
    InBound_PackCtrl_Cnt    = 0;
    OutBound_PackCtrl_Cnt   = 0;

    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_PRMPDIRSubframeDataCheckAndValidate(): NumberOfSubframeBlocks=%d", 
        pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks);

    // is DFP (PDIRSubframeData and PDIRData) present ?
    if ( EDDP_PRMPDIRSubframeDataIsDfpPresent(pDDB, LSA_FALSE) )
    {
        /* ------------------------------------------------------------ */
        /* search all FrameIDs from PDIRSubframeData in PDIRData        */
        /* ------------------------------------------------------------ */
        for ( iSubfrBlock=0; iSubfrBlock < pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks; iSubfrBlock++ )
        {
            /* -------------------------------------------------------- */
            /* init parameters in SubframeBlock                         */
            /* -------------------------------------------------------- */
            pSubframeBlock                              = &pDDB->PRM.pPDIRSubframeData_B->SubframeBlockArray[iSubfrBlock];
            pSubframeBlock->HwConfig                    = 0;
            pSubframeBlock->PDIRFrameDataProvIdx        = EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED;
            pSubframeBlock->PDIRFrameDataConsIdx        = EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED;
            pSubframeBlock->PDIRFrameDataProvIdxRed1    = EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED;
            pSubframeBlock->PDIRFrameDataConsIdxRed1    = EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED;

            /* -------------------------------------------------------- */
            /* PDIRSubframeData against PDIRData                        */
            /* -------------------------------------------------------- */
            /* loop through all PDIRFrameData and connect all           */
            /* SubframeBlock-PDIRFrameDataIdx                           */
            /* -------------------------------------------------------- */
            for ( iPDIRFrData=0; iPDIRFrData < pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All; iPDIRFrData++ )
            {
                pPDIRFrameData = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[iPDIRFrData];

                // only for IRT FrameID range
                if (EDDP_IS_IRT_FRAMEID(pPDIRFrameData->FrameID))
                {
                    // SubframeBlock-FrameID(x) == PDIRFrameData-FrameID(x)
                    if (pSubframeBlock->FrameID == pPDIRFrameData->FrameID)
                    {
                        if (pPDIRFrameData->SndRcvRole == EDDP_PRM_PDIRFRAMEDATA_ROLE_PROVIDER)
                        {
                            // Provider
                            EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataProvIdx != EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
                            pSubframeBlock->PDIRFrameDataProvIdx = (LSA_UINT16) iPDIRFrData;
                        }
                        else
                        {
                            // Consumer
                            EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataConsIdx != EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
                            pSubframeBlock->PDIRFrameDataConsIdx = (LSA_UINT16) iPDIRFrData;
                        }

                        // init "TxPort" from TxPortGroupArray always before "HwConfig" -> TxPortID: 1..4
                        pPDIRFrameData->TxPort = EDDP_PRMPDIRSubframeDataGetTxPortIDFromTxPortGroupArray (pDDB, pPDIRFrameData->TxPortGroupArray);
                        // store this SubframeBlock index in PDIRFrameData
                        pPDIRFrameData->SubframeBlockIdx = (LSA_UINT16) iSubfrBlock;
                    }

                    // only for redundant FrameID(x+1)
                    // SubframeBlock-FrameID(x) == PDIRFrameData-FrameID(x+1)
                    if ((EDDP_IS_IRT_REDUNDANT_FRAMEID(pSubframeBlock->FrameID)) && ((pSubframeBlock->FrameID+1) == pPDIRFrameData->FrameID))
                    {
                        if (pPDIRFrameData->SndRcvRole == EDDP_PRM_PDIRFRAMEDATA_ROLE_PROVIDER)
                        {
                            // Provider-FrameID(x+1)
                            EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataProvIdxRed1 != EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
                            pSubframeBlock->PDIRFrameDataProvIdxRed1 = (LSA_UINT16) iPDIRFrData;
                        }
                        else
                        {
                            // Consumer-FrameID(x+1)
                            EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataConsIdxRed1 != EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
                            pSubframeBlock->PDIRFrameDataConsIdxRed1 = (LSA_UINT16) iPDIRFrData;
                        }

                        // init "TxPort" from TxPortGroupArray always before "HwConfig" -> TxPortID: 1..4
                        pPDIRFrameData->TxPort = EDDP_PRMPDIRSubframeDataGetTxPortIDFromTxPortGroupArray (pDDB, pPDIRFrameData->TxPortGroupArray);
                        // store this SubframeBlock index in PDIRFrameData
                        pPDIRFrameData->SubframeBlockIdx = (LSA_UINT16) iSubfrBlock;
                    }
                }
            }

            /* -------------------------------------------------------------------- */
            /* following checking rules are dependent of DFPmode and DFPDirection   */
            /* and must be checked bevor init "HwConfig"                            */
            /* -------------------------------------------------------------------- */
            if (pSubframeBlock->SFIOCRProperties.DFPmode == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_DFPMODE_END_NODE)
            {
                /* -------------------------------------------------------- */
                /* IO-Controller: InBound                                   */
                /* -------------------------------------------------------- */
                if (pSubframeBlock->SFIOCRProperties.DFPDirection == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPDIRECTION_INBOUND)
                {
                    // consumer must be present in PDIRFrameData
                    if (pSubframeBlock->PDIRFrameDataConsIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
                    {
                        // invalid direction/SFCRC settings
                        EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDIRSUBFRAMEDATA_RECORD_INDEX, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                        Response = EDD_STS_ERR_PRM_CONSISTENCY;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataCheckAndValidate(): ERROR -> PDIRSubframeData.FrameID(0x%X) is InBound on Controller but no consumer is found in PDIRFrameData!", 
                            pSubframeBlock->FrameID);
                        // exit FOR loop - PDIRSubframeData
                        iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                    }
                    else
                    {
                        // if redundant FrameID(x), consumer(x+1) must be present in PDIRFrameData
                        if ( (EDDP_IS_IRT_REDUNDANT_FRAMEID(pSubframeBlock->FrameID)) && (pSubframeBlock->PDIRFrameDataConsIdxRed1 == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED) )
                        {
                            // invalid direction/SFCRC settings
                            EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDIRSUBFRAMEDATA_RECORD_INDEX, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                            Response = EDD_STS_ERR_PRM_CONSISTENCY;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataCheckAndValidate(): ERROR -> PDIRSubframeData.FrameID(0x%X) is InBound redundant on Controller but no consumer(x+1) is found in PDIRFrameData!", 
                                pSubframeBlock->FrameID);
                            // exit FOR loop - PDIRSubframeData
                            iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                        }
                    }
                }

                /* -------------------------------------------------------- */
                /* IO-Controller: OutBound                                  */
                /* -------------------------------------------------------- */
                if (   (Response == EDD_STS_OK)
                    && (pSubframeBlock->SFIOCRProperties.DFPDirection == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPDIRECTION_OUTBOUND)
                   )
                {
                    // provider must be present in PDIRFrameData
                    if (pSubframeBlock->PDIRFrameDataProvIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
                    {
                        // invalid direction/SFCRC settings
                        EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDIRSUBFRAMEDATA_RECORD_INDEX, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                        Response = EDD_STS_ERR_PRM_CONSISTENCY;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataCheckAndValidate(): ERROR -> PDIRSubframeData.FrameID(0x%X) is OutBound on Controller but no provider is found in PDIRFrameData!", 
                            pSubframeBlock->FrameID);
                        // exit FOR loop - PDIRSubframeData
                        iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                    }
                    else
                    {
                        // if redundant FrameID(x), provider(x+1) must be present in PDIRFrameData
                        if ( (EDDP_IS_IRT_REDUNDANT_FRAMEID(pSubframeBlock->FrameID)) && (pSubframeBlock->PDIRFrameDataProvIdxRed1 == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED) )
                        {
                            // invalid direction/SFCRC settings
                            EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDIRSUBFRAMEDATA_RECORD_INDEX, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                            Response = EDD_STS_ERR_PRM_CONSISTENCY;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataCheckAndValidate(): ERROR -> PDIRSubframeData.FrameID(0x%X) is OutBound redundant on Controller but no provider(x+1) is found in PDIRFrameData!", 
                                pSubframeBlock->FrameID);
                            // exit FOR loop - PDIRSubframeData
                            iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                        }
                    }
                }
            }
            else
            {
                /* -------------------------------------------------------- */
                /* IO-Device: InBound                                       */
                /* -------------------------------------------------------- */
                if (pSubframeBlock->SFIOCRProperties.DFPDirection == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPDIRECTION_INBOUND)
                {
                    // provider must be present in PDIRFrameData
                    if (pSubframeBlock->PDIRFrameDataProvIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
                    {
                        // invalid direction/SFCRC settings
                        EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDIRSUBFRAMEDATA_RECORD_INDEX, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                        Response = EDD_STS_ERR_PRM_CONSISTENCY;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataCheckAndValidate(): ERROR -> PDIRSubframeData.FrameID(0x%X) is InBound on Device but no provider is found in PDIRFrameData!", 
                            pSubframeBlock->FrameID);
                        // exit FOR loop - PDIRSubframeData
                        iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                    }
                    else
                    {
                        // if redundant FrameID(x), provider(x+1) must be present in PDIRFrameData
                        if ( (EDDP_IS_IRT_REDUNDANT_FRAMEID(pSubframeBlock->FrameID)) && (pSubframeBlock->PDIRFrameDataProvIdxRed1 == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED) )
                        {
                            // invalid direction/SFCRC settings
                            EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDIRSUBFRAMEDATA_RECORD_INDEX, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                            Response = EDD_STS_ERR_PRM_CONSISTENCY;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataCheckAndValidate(): ERROR -> PDIRSubframeData.FrameID(0x%X) is InBound redundant on Device but no provider(x+1) is found in PDIRFrameData!", 
                                pSubframeBlock->FrameID);
                            // exit FOR loop - PDIRSubframeData
                            iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                        }
                    }
                }

                /* -------------------------------------------------------- */
                /* IO-Device: OutBound                                      */
                /* -------------------------------------------------------- */
                if (   (Response == EDD_STS_OK)
                    && (pSubframeBlock->SFIOCRProperties.DFPDirection == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPDIRECTION_OUTBOUND)
                   )
                {
                    // consumer must be present in PDIRFrameData
                    if (pSubframeBlock->PDIRFrameDataConsIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
                    {
                        // invalid direction/SFCRC settings
                        EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDIRSUBFRAMEDATA_RECORD_INDEX, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                        Response = EDD_STS_ERR_PRM_CONSISTENCY;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataCheckAndValidate(): ERROR -> PDIRSubframeData.FrameID(0x%X) is OutBound on Device but no consumer is found in PDIRFrameData!", 
                            pSubframeBlock->FrameID);
                        // exit FOR loop - PDIRSubframeData
                        iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                    }
                    else
                    {
                        // if redundant FrameID(x), consumer(x+1) must be present in PDIRFrameData
                        if ( (EDDP_IS_IRT_REDUNDANT_FRAMEID(pSubframeBlock->FrameID)) && (pSubframeBlock->PDIRFrameDataConsIdxRed1 == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED) )
                        {
                            // invalid direction/SFCRC settings
                            EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDIRSUBFRAMEDATA_RECORD_INDEX, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                            Response = EDD_STS_ERR_PRM_CONSISTENCY;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataCheckAndValidate(): ERROR -> PDIRSubframeData.FrameID(0x%X) is OutBound redundant on Device but no consumer(x+1) is found in PDIRFrameData!", 
                                pSubframeBlock->FrameID);
                            // exit FOR loop - PDIRSubframeData
                            iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                        }
                    }
                }
            }

            /* -------------------------------------------------------- */
            /* init "HwConfig" of this SubframeBlock                    */
            /* -------------------------------------------------------- */
            if (Response == EDD_STS_OK)
            {
                pSubframeBlock->HwConfig = EDDP_PRMPDIRSubframeDataGetHwConfigFromSubframeBlock (pDDB, (LSA_UINT16)iSubfrBlock);
                EDDP_ASSERT_FALSE(pSubframeBlock->HwConfig == 0);
            }

            /* -------------------------------------------------------- */
            /* checks for this SubframeBlock (pack group)               */
            /* -------------------------------------------------------- */
            if (Response == EDD_STS_OK)
            {
                // check the PDIRSubframeData.FrameID is found in PDIRData
                if (   (pSubframeBlock->PDIRFrameDataProvIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED) 
                    && (pSubframeBlock->PDIRFrameDataConsIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED) 
                   )
                {
                    //Invalid PF length, type, no match with PDIRData
                    EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDD_PRM_ERR_INDEX_DEFAULT, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                    Response = EDD_STS_ERR_PRM_CONSISTENCY;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataCheckAndValidate(): ERROR -> PDIRSubframeData.FrameID(0x%X) is not found in PDIRData!", 
                        pSubframeBlock->FrameID);
                    // exit FOR loop - PDIRSubframeData
                    iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                }
                else
                {
                    /* -------------------------------------------------------- */
                    /* checks for provider                                      */
                    /* -------------------------------------------------------- */
                    if (pSubframeBlock->PDIRFrameDataProvIdx != EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
                    {
                        // this SubframeBlock is provider - set pointer to PDIRFrameData[provider]
                        pPDIRFrameData = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataProvIdx];

                        // check the "TxPortGroupArray" has exact one TxPort in PDIRData
                        TxPortCount = EDDP_PRMGetPortCountFromTxPortGroupArray (pDDB, pPDIRFrameData->TxPortGroupArray);

                        if (TxPortCount != 1)
                        {
                            // Invalid PF length, type, no match with PDIRData
                            EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDD_PRM_ERR_INDEX_DEFAULT, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                            Response = EDD_STS_ERR_PRM_CONSISTENCY;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataCheckAndValidate(): ERROR -> PDIRSubframeData.FrameID(0x%X) is provider and has TxPortCount(%d) not 1 in PDIRData! TxPortGroupArray=0x%X", 
                                pSubframeBlock->FrameID, TxPortCount, pPDIRFrameData->TxPortGroupArray);
                            // exit FOR loop - PDIRSubframeData
                            iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                        }
                        else
                        {
                            // following checking rules are dependent of DFPmode
                            if (pSubframeBlock->SFIOCRProperties.DFPmode == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_DFPMODE_END_NODE)
                            {
                                // IO-Controller uses all ACW_DGs
                                if (pSubframeBlock->NumberOfSubframeData > EDDP_CRTGetProviderAcwDGMaxNumber(pDDB))
                                {
                                    // Too many packframes/subframes
                                    EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDIRSUBFRAMEDATA_RECORD_INDEX, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                                    Response = EDD_STS_ERR_PRM_CONSISTENCY;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataCheckAndValidate(): ERROR -> NumberOfSubframeData(%d) > ProviderAcwDGMaxNumber(%d)", 
                                        pSubframeBlock->NumberOfSubframeData, EDDP_CRTGetProviderAcwDGMaxNumber(pDDB));
                                    // exit FOR loop - PDIRSubframeData
                                    iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                                }
                            }
                        }
                    }

                    /* -------------------------------------------------------- */
                    /* checks for consumer                                      */
                    /* -------------------------------------------------------- */
                    if (   (Response == EDD_STS_OK)
                        && (pSubframeBlock->PDIRFrameDataConsIdx != EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
                       )
                    {
                        // this SubframeBlock is consumer - set pointer to PDIRFrameData[consumer]
                        pPDIRFrameData = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataConsIdx];

                        // check the "TxPortGroupArray" has maximum one TxPort in PDIRData
                        TxPortCount = EDDP_PRMGetPortCountFromTxPortGroupArray (pDDB, pPDIRFrameData->TxPortGroupArray);

                        if (TxPortCount > 1)
                        {
                            // Invalid PF length, type, no match with PDIRData
                            EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDD_PRM_ERR_INDEX_DEFAULT, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                            Response = EDD_STS_ERR_PRM_CONSISTENCY;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataCheckAndValidate(): ERROR -> PDIRSubframeData.FrameID(0x%X) is consumer and has TxPortCount(%d) > 1 in PDIRData! TxPortGroupArray=0x%X", 
                                pSubframeBlock->FrameID, TxPortCount, pPDIRFrameData->TxPortGroupArray);
                            // exit FOR loop - PDIRSubframeData
                            iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                        }
                        else
                        {
                            // following checking rule is dependent of DFPmode
                            if (pSubframeBlock->SFIOCRProperties.DFPmode == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_DFPMODE_END_NODE)
                            {
                                // IO-Controller uses all ACW_DGs
                                if (pSubframeBlock->NumberOfSubframeData > EDDP_CRTGetConsumerAcwDGMaxNumber(pDDB))
                                {
                                    // Too many packframes/subframes
                                    EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDIRSUBFRAMEDATA_RECORD_INDEX, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                                    Response = EDD_STS_ERR_PRM_CONSISTENCY;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataCheckAndValidate(): ERROR -> NumberOfSubframeData(%d) > ConsumerAcwDGMaxNumber(%d)", 
                                        pSubframeBlock->NumberOfSubframeData, EDDP_CRTGetConsumerAcwDGMaxNumber(pDDB));
                                    // exit FOR loop - PDIRSubframeData
                                    iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                                }

                                // checks for redundant FrameID
                                if ((Response == EDD_STS_OK) && (EDDP_IS_IRT_REDUNDANT_FRAMEID(pSubframeBlock->FrameID)))
                                {
                                    switch (EDDP_HALGetPnipRevision(pDDB))
                                    {
                                        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
                                        case EDDP_HAL_HW_PNIP_REV1:
                                        {
                                            // MRT diagnosis (with PN-IP Rev1): activated "FrameDetails.MediaRedundancyWatchDog" is not allowed for Controller use case
                                            if (pPDIRFrameData->MrtDiag.Flags & EDDP_PRM_PDIRFRAMEDATA_MRT_FLAGS_MRPD_WATCHDOG_ACTIVE)
                                            {
                                                // MediaRedundancyWatchDog is not allowed for DFP-Consumer as Controller
                                                EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDIRSUBFRAMEDATA_RECORD_INDEX, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                                                Response = EDD_STS_ERR_PRM_CONSISTENCY;
                                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                                EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataCheckAndValidate(): ERROR -> FrameDetails.MediaRedundancyWatchDog(ON) is not allowed for DFP-Consumer as Controller use case with PN-IP Rev1! PDIRSubframeData.FrameID(0x%X)", 
                                                    pSubframeBlock->FrameID);
                                                // exit FOR loop - PDIRSubframeData
                                                iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                                            }
                                        }
                                        break;
                                        #endif

                                        #if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
                                        case EDDP_HAL_HW_PNIP_REV2:
                                        case EDDP_HAL_HW_PNIP_REV3:
                                        {
                                            // MRT diagnosis (with PN-IP Rev2): activated "FrameDetails.MediaRedundancyWatchDog" is allowed for Controller use case
                                        }
                                        break;
                                        #endif
                                        
                                        default:
                                        {
                                            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRSubframeDataCheckAndValidate(): FATAL -> Version(%d) of PNIP is unknown", 
                                                EDDP_HALGetPnipRevision(pDDB));
                                            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    /* -------------------------------------------------------- */
                    /* check Subframe-DataLength against PDIRData-DataLength    */
                    /* -------------------------------------------------------- */
                    if (Response == EDD_STS_OK)
                    {
                        /* Set Error in EDDP_PRMDIRSubframDataCheckSubFrameDataLength() */
                        Response = EDDP_PRMPDIRSubframeDataCheckSubframeDataLength (pDDB, (LSA_UINT16)iSubfrBlock);

                        if (Response != EDD_STS_OK)
                        {
                            // exit FOR loop - PDIRSubframeData
                            iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                        }
                    }
                }
            }

            /* -------------------------------------------- */
            /* checks for redundant FrameID                 */
            /* -------------------------------------------- */
            if ((Response == EDD_STS_OK) && (EDDP_IS_IRT_REDUNDANT_FRAMEID(pSubframeBlock->FrameID)))
            {
                // check the PDIRSubframeData.FrameID(x+1) is found in PDIRData
                // at least one FrameID(x+1) as provider or consumer must be present in PDIRData
                if (   (pSubframeBlock->PDIRFrameDataProvIdxRed1 == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED) 
                    && (pSubframeBlock->PDIRFrameDataConsIdxRed1 == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED) 
                   )
                {
                    // Invalid PF length, type, no match with PDIRData
                    EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDD_PRM_ERR_INDEX_DEFAULT, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                    Response = EDD_STS_ERR_PRM_CONSISTENCY;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataCheckAndValidate(): ERROR -> PDIRSubframeData.FrameID(0x%X) is redundant but no FrameID(x+1) is found in PDIRData!", 
                        pSubframeBlock->FrameID);
                    // exit FOR loop - PDIRSubframeData
                    iSubfrBlock = pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks;
                }

            }

            /* -------------------------------------------- */
            /* modify InBound_PackCtrl_Cnt of this          */
            /* SubframeBlock                                */
            /* -------------------------------------------- */
            if (pSubframeBlock->HwConfig & EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_INBOUND_PACK_MASK)
            {
                InBound_PackCtrl_Cnt++;
            }
            if (pSubframeBlock->HwConfig & EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_INBOUND_PACK_RED1_MASK)
            {
                InBound_PackCtrl_Cnt++;
            }
            /* -------------------------------------------- */
            /* modify OutBound_PackCtrl_Cnt of this         */
            /* SubframeBlock                                */
            /* -------------------------------------------- */
            if (pSubframeBlock->HwConfig & EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_OUTBOUND_PACK_MASK)
            {
                OutBound_PackCtrl_Cnt++;
            }
            if (pSubframeBlock->HwConfig & EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_OUTBOUND_PACK_RED1_MASK)
            {
                OutBound_PackCtrl_Cnt++;
            }
        }

        /* -------------------------------------------- */
        /* check PackCtrl-Counts are in valid range     */
        /* -------------------------------------------- */
        if (Response == EDD_STS_OK)
        {
            // check number of all InBounds
            if (InBound_PackCtrl_Cnt > EDDP_PRM_PDIRSUBFRAMEDATA_MAX_INBOUND_PACK_FRAMES)
            {
                // Too many packframes/subframes
                EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDD_PRM_ERR_INDEX_DEFAULT, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                Response = EDD_STS_ERR_PRM_CONSISTENCY;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataCheckAndValidate(): ERROR -> InBound_PackCtrl_Cnt(%d) > InBound_PackCtrl_Cnt_Max(%d)!", 
                    InBound_PackCtrl_Cnt, EDDP_PRM_PDIRSUBFRAMEDATA_MAX_INBOUND_PACK_FRAMES);
            }
            else
            {
                // check number of all OutBounds
                if (OutBound_PackCtrl_Cnt > EDDP_PRM_PDIRSUBFRAMEDATA_MAX_OUTBOUND_PACK_FRAMES)
                {
                    // Too many packframes/subframes
                    EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDD_PRM_ERR_INDEX_DEFAULT, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                    Response = EDD_STS_ERR_PRM_CONSISTENCY;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataCheckAndValidate(): ERROR -> OutBound_PackCtrl_Cnt(%d) > OutBound_PackCtrl_Cnt_Max(%d)!", 
                        OutBound_PackCtrl_Cnt, EDDP_PRM_PDIRSUBFRAMEDATA_MAX_OUTBOUND_PACK_FRAMES);
                }
            }
        }
    }
    else
    {
        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRSubframeDataCheckAndValidate(): FATAL -> The record PDIRSubframeData(0x%X) or PDIRData(0x%X) is not present!", 
            pDDB->PRM.pRecordsetB->PDIRSubframeData_IsPresent, pDDB->PRM.pRecordsetB->PDIRData_IsPresent);
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }

    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMPDIRSubframeDataCheckAndValidate(): InBound_PackCtrl_Cnt=%d, OutBound_PackCtrl_Cnt=%d, Response=0x%X", 
        InBound_PackCtrl_Cnt, OutBound_PackCtrl_Cnt, Response);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname   :    EDDP_PRMPDIRSubframeDataGetHwConfigFromSubframeBlock           +*/
/*+                                                                                     +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB                            +*/
/*+                      LSA_UINT16                     SubframeBlockIdx                +*/
/*+                                                                                     +*/
/*+  Result         :    LSA_UINT16                                                     +*/
/*+                      -> see masks of "HwConfig" in PDIRSubframeData.SubframeBlock   +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  SubframeBlockIdx   : index in PDIRSubframeData.SubframeBlock[x]                    +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function gets the "HwConfig" bits for given SubframeBlockIdx     +*/
/*+               from PDIRSubframeData.SubframeBlock[SubframeBlockIdx].                +*/
/*+                                                                                     +*/
/*+               Call this function only if both records (PDIRData and PDIRSubframeData)+*/
/*+               are present in RecordsetB!                                            +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRSubframeDataGetHwConfigFromSubframeBlock(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  SubframeBlockIdx)
{
    LSA_UINT16                                          HwConfig;
    LSA_UINT16                                          DFPmode;
    EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_PTR_TYPE    pSubframeBlock;
    EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE                     pPDIRFrameDataProv, pPDIRFrameDataCons, pPDIRFrameDataProvRed1, pPDIRFrameDataConsRed1;

    HwConfig                = 0;
    pPDIRFrameDataProv      = LSA_NULL;
    pPDIRFrameDataCons      = LSA_NULL;
    pPDIRFrameDataProvRed1  = LSA_NULL;
    pPDIRFrameDataConsRed1  = LSA_NULL;

    // is DFP (PDIRSubframeData and PDIRData) present ?
    if ( EDDP_PRMPDIRSubframeDataIsDfpPresent(pDDB, LSA_FALSE) )
    {
        EDDP_ASSERT_FALSE(SubframeBlockIdx >= pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks);
        pSubframeBlock  = &pDDB->PRM.pPDIRSubframeData_B->SubframeBlockArray[SubframeBlockIdx];
        DFPmode         = pSubframeBlock->SFIOCRProperties.DFPmode;

        /* -------------------------------- */
        /* init pPDIRFrameData              */
        /* -------------------------------- */
        if (pSubframeBlock->PDIRFrameDataProvIdx != EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
        {
            EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataProvIdx >= pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All);
            pPDIRFrameDataProv = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataProvIdx];
            EDDP_ASSERT_FALSE(pPDIRFrameDataProv->SndRcvRole == EDDP_PRM_PDIRFRAMEDATA_ROLE_INVALID);
        }

        if (pSubframeBlock->PDIRFrameDataConsIdx != EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
        {
            EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataConsIdx >= pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All);
            pPDIRFrameDataCons = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataConsIdx];
            EDDP_ASSERT_FALSE(pPDIRFrameDataCons->SndRcvRole == EDDP_PRM_PDIRFRAMEDATA_ROLE_INVALID);
        }

        if (pSubframeBlock->PDIRFrameDataProvIdxRed1 != EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
        {
            EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataProvIdxRed1 >= pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All);
            pPDIRFrameDataProvRed1 = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataProvIdxRed1];
            EDDP_ASSERT_FALSE(pPDIRFrameDataProvRed1->SndRcvRole == EDDP_PRM_PDIRFRAMEDATA_ROLE_INVALID);
        }

        if (pSubframeBlock->PDIRFrameDataConsIdxRed1 != EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
        {
            EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataConsIdxRed1 >= pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All);
            pPDIRFrameDataConsRed1 = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataConsIdxRed1];
            EDDP_ASSERT_FALSE(pPDIRFrameDataConsRed1->SndRcvRole == EDDP_PRM_PDIRFRAMEDATA_ROLE_INVALID);
        }

        /* -------------------------------- */
        /* set bits for HwConfig            */
        /* -------------------------------- */
        if (DFPmode == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_DFPMODE_END_NODE)
        {
            /* ---------------------------------------------------- */
            /* end node (Controller/MultiDevice)                    */
            /* ---------------------------------------------------- */
            if ( !(LSA_HOST_PTR_ARE_EQUAL(pPDIRFrameDataProv, LSA_NULL)) )
            {
                // Provider needs
                // Send ACW with ACW_DG
                EDDP_SET_BIT_VALUE16( HwConfig, 
                                      1, 
                                      EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_SND_ACW_MASK, 
                                      EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_SND_ACW_SHIFT)
            }

            if ( !(LSA_HOST_PTR_ARE_EQUAL(pPDIRFrameDataCons, LSA_NULL)) )
            {
                // Consumer needs
                // OutBound-Pack in Pack-Control
                EDDP_SET_BIT_VALUE16( HwConfig, 
                                      1, 
                                      EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_OUTBOUND_PACK_MASK, 
                                      EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_OUTBOUND_PACK_SHIFT)
                // Receive ACW with ACW_DG
                EDDP_SET_BIT_VALUE16( HwConfig, 
                                      1, 
                                      EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_RCV_ACW_MASK, 
                                      EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_RCV_ACW_SHIFT)
            }

            if ( !(LSA_HOST_PTR_ARE_EQUAL(pPDIRFrameDataProvRed1, LSA_NULL)) )
            {
                // Provider needs Send ACW(x+1) with ACW_DG
                EDDP_SET_BIT_VALUE16( HwConfig, 
                                      1, 
                                      EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_SND_ACW_RED1_MASK, 
                                      EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_SND_ACW_RED1_SHIFT)
            }

            if ( !(LSA_HOST_PTR_ARE_EQUAL(pPDIRFrameDataConsRed1, LSA_NULL)) )
            {
                // needs Consumer OutBound-Pack in Pack-Control for FrameID(x+1) ?
                if ( !(LSA_HOST_PTR_ARE_EQUAL(pPDIRFrameDataCons, LSA_NULL)) )
                {
                    if (pPDIRFrameDataCons->RxPort != pPDIRFrameDataConsRed1->RxPort)
                    {
                        // OutBound-Pack in Pack-Control for FrameID(x+1)
                        EDDP_SET_BIT_VALUE16( HwConfig, 
                                              1, 
                                              EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_OUTBOUND_PACK_RED1_MASK, 
                                              EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_OUTBOUND_PACK_RED1_SHIFT)
                    }
                }
            }
        }
        else
        {
            /* ---------------------------------------------------- */
            /* Subframe position (Device)                           */
            /* ---------------------------------------------------- */
            if (pSubframeBlock->SFIOCRProperties.DFPDirection == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPDIRECTION_INBOUND)
            {
                /* -------------------------------------------- */
                /* InBound - PDIRFrameData entry as provider    */
                /* -------------------------------------------- */
                EDDP_ASSERT_NO_NULL_PTR(pPDIRFrameDataProv);
                // Provider needs
                // InBound-Pack in Pack-Control
                EDDP_SET_BIT_VALUE16( HwConfig, 
                                      1, 
                                      EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_INBOUND_PACK_MASK, 
                                      EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_INBOUND_PACK_SHIFT)
                // Send ACW with ACW_DG
                EDDP_SET_BIT_VALUE16( HwConfig, 
                                      1, 
                                      EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_SND_ACW_MASK, 
                                      EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_SND_ACW_SHIFT)
                // Send ACW.InBound_Pack = 1
                EDDP_SET_BIT_VALUE16( HwConfig, 
                                      1, 
                                      EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_SND_ACW_INBOUND_PACK_MASK, 
                                      EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_SND_ACW_INBOUND_PACK_SHIFT)

                if ( !(LSA_HOST_PTR_ARE_EQUAL(pPDIRFrameDataProvRed1, LSA_NULL)) )
                {
                    // Provider needs Send ACW(x+1) with ACW_DG
                    EDDP_SET_BIT_VALUE16( HwConfig, 
                                          1, 
                                          EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_SND_ACW_RED1_MASK, 
                                          EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_SND_ACW_RED1_SHIFT)

                    // needs Provider InBound-Pack in Pack-Control for FrameID(x+1) ?
                    if (   (pPDIRFrameDataProv->TxPort != pPDIRFrameDataProvRed1->TxPort)
                        || (pSubframeBlock->SFIOCRProperties.DFPRedundantPathLayout == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_DOWN)
                        )
                    {
                        // InBound-Pack in Pack-Control for FrameID(x+1)
                        EDDP_SET_BIT_VALUE16( HwConfig, 
                                              1, 
                                              EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_INBOUND_PACK_RED1_MASK, 
                                              EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_INBOUND_PACK_RED1_SHIFT)
                    }
                }
            }
            else
            {
                /* -------------------------------------------- */
                /* OutBound - PDIRFrameData entry as consumer   */
                /* -------------------------------------------- */
                EDDP_ASSERT_NO_NULL_PTR(pPDIRFrameDataCons);
                // Consumer needs
                // OutBound-Pack in Pack-Control
                EDDP_SET_BIT_VALUE16( HwConfig, 
                                      1, 
                                      EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_OUTBOUND_PACK_MASK, 
                                      EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_OUTBOUND_PACK_SHIFT)
                // Receive ACW with ACW_DG
                EDDP_SET_BIT_VALUE16( HwConfig, 
                                      1, 
                                      EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_RCV_ACW_MASK, 
                                      EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_RCV_ACW_SHIFT)
            
                // needs Consumer OutBound-Pack in Pack-Control for FrameID(x+1) ?
                if ( !(LSA_HOST_PTR_ARE_EQUAL(pPDIRFrameDataConsRed1, LSA_NULL)) )
                {
                    if (pPDIRFrameDataConsRed1->isDoubleFrameID)
                    {
                        // Consumer-FrameID(x+1) is double in PDIRFrameData (as provider and consumer)
                        EDDP_ASSERT_NO_NULL_PTR(pPDIRFrameDataProvRed1);

                        // if FrameID(x+1) has inverse DG order
                        if (pSubframeBlock->SFIOCRProperties.DFPRedundantPathLayout == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_DOWN)
                        {
                            // OutBound-Pack in Pack-Control for FrameID(x+1)
                            EDDP_SET_BIT_VALUE16( HwConfig, 
                                                  1, 
                                                  EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_OUTBOUND_PACK_RED1_MASK, 
                                                  EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_OUTBOUND_PACK_RED1_SHIFT)
                        }
                    }
                    else
                    {
                        // Consumer-FrameID(x+1) is one in PDIRFrameData (only as consumer)
                        if (pPDIRFrameDataCons->RxPort != pPDIRFrameDataConsRed1->RxPort)
                        {
                            // OutBound-Pack in Pack-Control for FrameID(x+1)
                            EDDP_SET_BIT_VALUE16( HwConfig, 
                                                  1, 
                                                  EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_OUTBOUND_PACK_RED1_MASK, 
                                                  EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_OUTBOUND_PACK_RED1_SHIFT)
                        }
                    }
                }
            }
        }
    }
    else
    {
        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRSubframeDataGetHwConfigFromSubframeBlock(): FATAL -> The record PDIRSubframeData(0x%X) or PDIRData(0x%X) is not present!", 
            pDDB->PRM.pRecordsetB->PDIRSubframeData_IsPresent, pDDB->PRM.pRecordsetB->PDIRData_IsPresent);
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }

    return (HwConfig);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRSubframeDataIsSameDfpActive                 +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_BOOL                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function gets info whether the same DFP configuration is         +*/
/*+               already active in RecordsetA.                                         +*/
/*+                                                                                     +*/
/*+               These records are compared between RecordsetA and RecordsetB:         +*/
/*+                 - PDSyncData                                                        +*/
/*+                 - PDIRData                                                          +*/
/*+                 - PDIRSubframeData                                                  +*/
/*+                                                                                     +*/
/*+               Return LSA_TRUE  : same DFP is already active                         +*/
/*+                      LSA_FALSE : new DFP is comming                                 +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRSubframeDataIsSameDfpActive(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB)
{
    LSA_BOOL        State = LSA_FALSE;

    if ( (   (pDDB->PRM.pRecordsetA->PDSyncData_IsPresent == pDDB->PRM.pRecordsetB->PDSyncData_IsPresent)
          && (pDDB->PRM.pRecordsetB->PDSyncData_IsPresent == pDDB->PRM.pRecordsetB->PDSyncData_Skip)
         )
         &&
         (   (pDDB->PRM.pRecordsetA->PDIRData_IsPresent == pDDB->PRM.pRecordsetB->PDIRData_IsPresent)
          && (pDDB->PRM.pRecordsetB->PDIRData_IsPresent == pDDB->PRM.pRecordsetB->PDIRData_Skip)
         )
         &&
         (   (pDDB->PRM.pRecordsetA->PDIRSubframeData_IsPresent == pDDB->PRM.pRecordsetB->PDIRSubframeData_IsPresent)
          && (pDDB->PRM.pRecordsetB->PDIRSubframeData_IsPresent == pDDB->PRM.pRecordsetB->PDIRSubframeData_Skip)
         )
       )
    {
        // skip: same DFP configuration is already active
        State = LSA_TRUE;
    }

    return (State);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_PRMPDIRSubframeDataGetTxPortIDFromTxPortGroupArray+*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                       +*/
/*+                      LSA_UINT8                   TxPortGroupArray           +*/
/*+                                                                             +*/
/*+  Result         :    LSA_UINT16                                             +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                     +*/
/*+  TxPortGroupArray   : TxPortGroupArray from PRM-Record                      +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function gets the TxPortID from given                    +*/
/*+               TxPortGroupArray of PRM-Record.                               +*/
/*+                                                                             +*/
/*+               This is DFP, so TxPortGroupArray must have:                   +*/
/*+                 - exact one TxPort for provider                             +*/
/*+                 - maximum one TxPort for consumer                           +*/
/*+               If no, FATAL!                                                 +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRSubframeDataGetTxPortIDFromTxPortGroupArray(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT8                   TxPortGroupArray)
{
    LSA_UINT16          TxPortID;
    LSA_UINT32          TxPortCount;

    TxPortID    = 0;
    TxPortCount = EDDP_PRMGetPortCountFromTxPortGroupArray (pDDB, TxPortGroupArray);

    if (TxPortCount > 1)
    {
        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRSubframeDataGetTxPortIDFromTxPortGroupArray(TxPortGroupArray=0x%X): FATAL -> TxPortCount(%d) > 1", 
            TxPortGroupArray, TxPortCount);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        EDDP_SHIFT_VALUE_RIGHT(TxPortGroupArray, 1);    // skip Bit(0) API
        // loop through the TxPortGroupArray
        while (TxPortGroupArray != 0)
        {
            TxPortID++;
            if (TxPortGroupArray & 0x1)
            {
                // it is expecting maximum one TxPort, so exit WHILE loop
                TxPortGroupArray = 0;
            }
            EDDP_SHIFT_VALUE_RIGHT(TxPortGroupArray, 1);    // next port of this TxPortGroupArray
        }
    }

    return (TxPortID);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+																										+*/
/*+  Functionname          :    EDDP_PRMPDIRSubframeDataGet_CSF_SDU_DataLength                          +*/
/*+																										+*/
/*+  Input/Output          :    LSA_BOOL											sumFromZero			+*/
/*+                             LSA_UINT16											DfpPosition			+*/
/*+                             EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_PTR_TYPE	pSubframeBlock		+*/
/*+								LSA_UINT16*											pRet_val1			+*/
/*+								LSA_UINT16*											pRet_val2			+*/
/*+																										+*/
/*+-------------------------------------------------------------------------------------				+*/
/*+																										+*/
/*+  Input/Output:																						+*/
/*+																										+*/
/*+  sumFromZero            : indicates how to add the subframe lengths.								+*/
/*+								LSA_TRUE: add the lengths of the subframes, starting from zero			+*/
/*+										--> length[DG1...DG(x-1)DG(x)]									+*/
/*+								LSA_FASE: add the lengths of the subframes, starting from				+*/
/*+															pSubframeBlock->NumberOfSubframeData		+*/
/*+										--> length[DG(x)DG(x-1)...DG(1)]								+*/
/*+  DfpPosition			: last SubframeLength entry to add (DG(x), x==DfpPosition					+*/
/*+  pSubframeBlock			: Pointer to PDIRSubframeData.SubframeBlock[x]								+*/
/*+	 pRet_val1				: Pointer to a LSA_UINT16 value, where the length with own SumframeLength	+*/
/*+								will be stored.															+*/
/*+								sumFromZero == LSA_TRUE:												+*/
/*+									*pRet_val1 = length[DG1...DG(x-1)DG(x)]								+*/
/*+							  sumFromZero == LSA_FALSE:													+*/
/*+									*pRef_val1 = length[DG(x)DG(x-1)...DG(1)] (with length of DG(x))   	+*/
/*+	 pRet_val2				: Pointer to a LSA_UINT16 value, where the length without own SumframeLength+*/
/*+								will be stored.															+*/
/*+								sumFromZero == LSA_TRUE:												+*/
/*+									*pRet_val2 = length[DG1...DG(x-1)]									+*/
/*+							  sumFromZero == LSA_FALSE:													+*/
/*+									*pRet_val2 = length[DG(x-1)...DG(1)] (without length of DG(x)) 		+*/
/*+-------------------------------------------------------------------------------------				+*/
/*+																										+*/
/*+  Description:	This function adds the SubframeLengths within SubframeBlock->SubframeDataArray[]	+*/
/*+					and returns the sum of the SumframeLengths starting from the start of the array		+*/	
/*+					(sumFromZero==LSA_TRUE)	or the end of the array (sumFromZero == LSA_FALSE).			+*/
/*+					The two return values are this sum (*pRet_val1) and the sum reduced by the local	+*/
/*+					SumframeLength(*pRet_val2)														    +*/
/*+																										+*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRSubframeDataGet_CSF_SDU_DataLength(
	LSA_BOOL                                            sumFromZero, 
	LSA_UINT16                                          DfpPosition, 
	EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_PTR_TYPE    pSubframeBlock, 
	LSA_UINT16*                                         pRet_val1, 
	LSA_UINT16*                                         pRet_val2)
{
    LSA_UINT16	    iSubfrData;
	LSA_UINT16      CSF_SDU_DataLength;
	LSA_UINT16	    SubframeLength;

    CSF_SDU_DataLength  = 0;
    SubframeLength      = 0;

    EDDP_ASSERT_NO_NULL_PTR(pSubframeBlock);

    if (sumFromZero)
	{
        /* -------------------------------------------------------- */
        /* SubframeDataArray starting from zero                     */
        /* -------------------------------------------------------- */
        // loop through the SubFrameData array until the DfpPosition of this subframe and add the SFDataLength of all subframes
		iSubfrData = 0;
		do
		{
            EDDP_ASSERT_FALSE(iSubfrData >= (pSubframeBlock->NumberOfSubframeData)); /* Overflow of the variable after a increment of the value iSubfrData*/
			SubframeLength = (pSubframeBlock->SubframeDataArray[iSubfrData].DataLength) + EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEHEADER_AND_SFCRC16_LENGTH;
			CSF_SDU_DataLength += SubframeLength;
		} while (pSubframeBlock->SubframeDataArray[iSubfrData++].Position != DfpPosition); 
	}
	else
	{
        /* -------------------------------------------------------- */
        /* SubframeDataArray starting from NumberOfSubframeData     */
        /* -------------------------------------------------------- */
        // loop through the SubFrameData array until the DfpPosition of this subframe and add the SFDataLength of all subframes
		iSubfrData = (pSubframeBlock->NumberOfSubframeData - 1);
		do
		{
			EDDP_ASSERT_FALSE(iSubfrData == EDDP_PRM_INVALID_VALUE_16) /* Underflow of the variable after a decrement of the value 0*/
			SubframeLength = (pSubframeBlock->SubframeDataArray[iSubfrData].DataLength) + EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEHEADER_AND_SFCRC16_LENGTH;
			CSF_SDU_DataLength += SubframeLength;
  
		} while (pSubframeBlock->SubframeDataArray[iSubfrData--].Position != DfpPosition); 
	}

    *pRet_val1 = CSF_SDU_DataLength;
	*pRet_val2 = CSF_SDU_DataLength - SubframeLength;
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRSubframeDataCheckSubframeDataLength         +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB                     +*/
/*+                             LSA_UINT16                     SubframeBlockIdx         +*/
/*+                                                                                     +*/
/*+  Result                :    EDD_RSP                                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  SubframeBlockIdx   : index in PDIRSubframeData.SubframeBlock[x]                    +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks the sum of all Subframe-DataLength               +*/
/*+               against the PDIRData-DataLength.                                      +*/
/*+                                                                                     +*/
/*+               Call this function only if both records (PDIRData and PDIRSubframeData)+*/
/*+               are present in RecordsetB!                                            +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRSubframeDataCheckSubframeDataLength(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  SubframeBlockIdx)
{
    EDD_RSP                                             Response;
    LSA_UINT32                                          iSubfrData;
    LSA_UINT16                                          SubframeLength;
    LSA_UINT16                                          CSF_SDU_DataLengthProv, CSF_SDU_DataLengthCons;
    LSA_UINT16                                          CSF_SDU_DataLengthProvRed1, CSF_SDU_DataLengthConsRed1;
	LSA_UINT16											DfpPosition;
    EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_PTR_TYPE    pSubframeBlock;
	EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE                     pPDIRFrameDataProv;
	EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE                     pPDIRFrameDataProvRed1;
    EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE                     pPDIRFrameDataCons;
	EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE                     pPDIRFrameDataConsRed1;
    EDDP_PRM_DETAIL_ERR_PTR_TYPE                        pDetailErr;

    Response                    = EDD_STS_OK;
    pDetailErr                  = &pDDB->PrmDetailErr;
    CSF_SDU_DataLengthProv      = 0;
    CSF_SDU_DataLengthCons      = 0;
    CSF_SDU_DataLengthProvRed1  = 0;
    CSF_SDU_DataLengthConsRed1  = 0;
    pSubframeBlock              = LSA_NULL;
    pPDIRFrameDataProv		    = LSA_NULL;
    pPDIRFrameDataProvRed1      = LSA_NULL;
    pPDIRFrameDataCons		    = LSA_NULL;
    pPDIRFrameDataConsRed1	    = LSA_NULL;

    /* is DFP (PDIRSubframeData and PDIRData) present ? */
    if ( EDDP_PRMPDIRSubframeDataIsDfpPresent(pDDB, LSA_FALSE) )
    {
        EDDP_ASSERT_FALSE(SubframeBlockIdx >= pDDB->PRM.pPDIRSubframeData_B->NumberOfSubframeBlocks);
        pSubframeBlock = &pDDB->PRM.pPDIRSubframeData_B->SubframeBlockArray[SubframeBlockIdx];

        /* ------------------------------------ */
        /* calculate CSF_SDU_DataLength         */
        /* ------------------------------------ */
        /* ---------------------------------------------------------------------------------------------------------------------------------------- */
		/* CSF_SDU_DataLength = SFHeaderCRC16(2) + Subframe1(x) + Subframe2(x) + … + SFEndDelimiter(2)												*/
		/* Subframe-Length = SFPosition(1) + SFDataLength(1) + SFCycleCounter(1) + DataStatusSubframe(1) + IOData(SFDataLength) + SFCRC16(2)		*/
		/*																																			*/
		/* For DFP, Frames will be shortened in OutBound-direction (with PNIP Controller, SFCRC16 == 1) or not (SOC1 Controller, SFCRC16 == 0)      */
		/*			Frames will be extended in InBound-direction (with both controllers, SFCRC = don't care)										*/
        /* ---------------------------------------------------------------------------------------------------------------------------------------- */

        if (pSubframeBlock->SFIOCRProperties.DFPmode == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_DFPMODE_END_NODE)
		{
            /* ---------------------------------------------------------------------------------------- */
            /* Controller                                                                               */
            /* ---------------------------------------------------------------------------------------- */
			for ( iSubfrData=0; iSubfrData < pSubframeBlock->NumberOfSubframeData ; iSubfrData++ )
			{
				SubframeLength = (pSubframeBlock->SubframeDataArray[iSubfrData].DataLength) + EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEHEADER_AND_SFCRC16_LENGTH;
				CSF_SDU_DataLengthProv += SubframeLength;
			}
            /* whole packgroup, same length for provider and consumer */
			CSF_SDU_DataLengthCons = CSF_SDU_DataLengthProv;

            if (pSubframeBlock->SFIOCRProperties.DFPDirection == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPDIRECTION_INBOUND)
			{
                /* ------------------------------------ */
                /* IOC - InBound                        */
                /* ------------------------------------ */
				/* Only consumer here since the consumer does not forward the InBound frame */
				EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataConsIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
				pPDIRFrameDataCons = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataConsIdx];

                if (EDDP_IS_IRT_REDUNDANT_FRAMEID(pSubframeBlock->FrameID))
				{
					/* For MRPD we have two consumers (x) and (x+1) */
					EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataConsIdxRed1 == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
                    pPDIRFrameDataConsRed1		= &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataConsIdxRed1];	
					CSF_SDU_DataLengthConsRed1  = CSF_SDU_DataLengthCons;   /* it's the same length also for the redundant frames */
				}
            }
			else if (pSubframeBlock->SFIOCRProperties.DFPDirection == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPDIRECTION_OUTBOUND)
			{
                /* ------------------------------------ */
                /* IOC - OutBound                       */
                /* ------------------------------------ */
				/* Only provider here since the controller is the one who creates the OutBound frame */
				EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataProvIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
				pPDIRFrameDataProv = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataProvIdx];

                if (EDDP_IS_IRT_REDUNDANT_FRAMEID(pSubframeBlock->FrameID))
				{
					/* For MRPD we have two providers (x) and (x+1) */
					EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataProvIdxRed1 == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
                    pPDIRFrameDataProvRed1		= &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataProvIdxRed1];
					CSF_SDU_DataLengthProvRed1  = CSF_SDU_DataLengthProv;   /* it's the same length also for the redundant frames */
				}
			}
            else
            {
                EDDP_PRM_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRSubframeDataCheckSubframeDataLength(SubframeBlockIdx=%d, FrameID=0x%X, DFPmode=%d): FATAL -> IOC DFPDirection(0x%X) is invalid!", 
                    SubframeBlockIdx, pSubframeBlock->FrameID, pSubframeBlock->SFIOCRProperties.DFPmode, pSubframeBlock->SFIOCRProperties.DFPDirection);
                EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
            }
		}
		else
		{
            /* ---------------------------------------------------------------------------------------- */
            /* Device                                                                                   */
            /* We have to check if it's outbound or inbound direction and if MRPD is active or not      */
            /* ---------------------------------------------------------------------------------------- */
			DfpPosition = pSubframeBlock->SFIOCRProperties.DFPmode;

			if (EDDP_IS_IRT_REDUNDANT_FRAMEID(pSubframeBlock->FrameID)) 
			{
                /* -------------------------------------------------------------------- */
                /* IOD and MRPD                                                         */
                /* DataLengths for redundant frames also exist and have to be checked   */
                /* -------------------------------------------------------------------- */
				if (pSubframeBlock->SFIOCRProperties.DFPDirection == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPDIRECTION_INBOUND)
				{
                    /* ------------------------------------ */
                    /* IOD - InBound MRPD                   */
                    /* ------------------------------------ */
                    EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataProvIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
					pPDIRFrameDataProv		= &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataProvIdx];
					EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataProvIdxRed1 == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
					pPDIRFrameDataProvRed1  = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataProvIdxRed1];

                    /* ------------------------------------------------------------------------------------------------------------ */
					/* Check if the local Datagram is either at the start or at the end of the packgroup							*/
					/* in this case, one of the consumers is not parametrized (start of pack-group in inbound direction)			*/
					/* The length of the one of the redundant frames is equal to the local SFDataLength	(only SFrame in packgroup   */ 
					/* The length of the of the other redundant frame is equal to the Length of the whole packgroup					*/
                    /* ------------------------------------------------------------------------------------------------------------ */
                    /* "last" device for one FrameID that sends the first InBound frame */
                    if (  (   (pSubframeBlock->SFIOCRProperties.DFPmode == pSubframeBlock->SubframeDataArray[(pSubframeBlock->NumberOfSubframeData-1)].Position)
                           || (pSubframeBlock->SFIOCRProperties.DFPmode == pSubframeBlock->SubframeDataArray[0].Position)
                          )
                            /* if Both Consumers are present, it's the first PNIP Devices in a stich */
						&& !(   (pSubframeBlock->PDIRFrameDataConsIdx != EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
							 && (pSubframeBlock->PDIRFrameDataConsIdxRed1 != EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
							)
					   )
					{
                        /* if both consumers are not present, one of them is missing and the other is there. */
                        /* For one FrameID, this is the "last device" in MRPD that sends the fist inbound frame. */ 
						if (   (pSubframeBlock->PDIRFrameDataConsIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
							&& (pSubframeBlock->PDIRFrameDataConsIdxRed1 == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
                           )
						{
                            /* Last device in PNIP stich. Here we have no consumers at all */
							/* --> Do nothing */
						}
						else
						{
							if (pSubframeBlock->PDIRFrameDataConsIdxRed1 == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
							{
                                /* Either this or redundant must be defined */
								EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataConsIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
								pPDIRFrameDataCons = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataConsIdx];
							}
							else
							{
								EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataConsIdxRed1 == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);	
								pPDIRFrameDataConsRed1 = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataConsIdxRed1];
							}
						}
					}
					else
					{
                        /* both consumers (x) and (x+1) are present in PDIRFrameData */
                        EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataConsIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);	
						pPDIRFrameDataCons		= &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataConsIdx];
						EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataConsIdxRed1 == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
						pPDIRFrameDataConsRed1  = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataConsIdxRed1];
					}
					/* Now that we know which Consumers are present, calculate the Datalengths. */
                    /* Lengths for non-existent consumers will be calculated as 0, but ignored at the check further down. */

					/* DG(x) + DG(x-1) ... DG1 */
					EDDP_PRMPDIRSubframeDataGet_CSF_SDU_DataLength (LSA_FALSE, DfpPosition, pSubframeBlock, &CSF_SDU_DataLengthProv, &CSF_SDU_DataLengthCons);

					/* DG(1) + DG(2) + DG(3) ... DG(x) */
					if (pSubframeBlock->SFIOCRProperties.DFPRedundantPathLayout == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_DOWN)
					{
						EDDP_PRMPDIRSubframeDataGet_CSF_SDU_DataLength (LSA_TRUE, DfpPosition, pSubframeBlock, &CSF_SDU_DataLengthProvRed1, &CSF_SDU_DataLengthConsRed1);
					}
					else
					{
                        /* DG(x) + DG(x-1) ... DG1. Since its the same as CSF_SDU_DataLengthProv and CSF_SDU_DataLengthCons, just copy here */
						CSF_SDU_DataLengthProvRed1 = CSF_SDU_DataLengthProv;
						CSF_SDU_DataLengthConsRed1 = CSF_SDU_DataLengthCons;
					}	
				}
				else if (pSubframeBlock->SFIOCRProperties.DFPDirection == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPDIRECTION_OUTBOUND) 
				{
                    /* ------------------------------------ */
                    /* IOD - OutBound MRPD                  */
                    /* ------------------------------------ */
                    if (pSubframeBlock->SFIOCRProperties.SFCRC16 == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_SFCRC16_0)
					{
						for ( iSubfrData=0; iSubfrData < pSubframeBlock->NumberOfSubframeData ; iSubfrData++ )
						{
							SubframeLength = (pSubframeBlock->SubframeDataArray[iSubfrData].DataLength) + EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEHEADER_AND_SFCRC16_LENGTH;
							CSF_SDU_DataLengthProv += SubframeLength;
						}

                        if (pSubframeBlock->PDIRFrameDataProvIdx != EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
						{
							/* Last Device for PNIP in Stich */
							pPDIRFrameDataProv = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataProvIdx];

                            if (pSubframeBlock->PDIRFrameDataProvIdxRed1 != EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
                            {
                                pPDIRFrameDataProvRed1 = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataProvIdxRed1];
                            }
						}

                        EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataConsIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
                        pPDIRFrameDataCons		= &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataConsIdx];
                        EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataConsIdxRed1 == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
						pPDIRFrameDataConsRed1  = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataConsIdxRed1];

                        /* whole packgroup, same length for all consumers and providers */
                        CSF_SDU_DataLengthCons      = CSF_SDU_DataLengthProv;
						CSF_SDU_DataLengthConsRed1  = CSF_SDU_DataLengthCons;
						CSF_SDU_DataLengthProvRed1  = CSF_SDU_DataLengthProv;
					}
					else if (pSubframeBlock->SFIOCRProperties.SFCRC16 == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_SFCRC16_1)
					{
						/* OutBound and redundant path */
                        EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataConsIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
						pPDIRFrameDataCons		= &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataConsIdx];
						EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataConsIdxRed1 == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
						pPDIRFrameDataConsRed1  = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataConsIdxRed1];
					
                        /* ------------------------------------------------------------------------------------------------------------ */
						/* Check if the local Datagram is either at the start or at the end of the packgroup			                */
						/* in this case, one of the providers is not parametrized (end of pack-group in outbound direction)             */
						/* The length of the one of the redundant frames is equal to the local SFDataLength                             */ 
						/* The length of the of the other redundant frame is equal to the Length of the whole packgroup                 */
                        /* ------------------------------------------------------------------------------------------------------------ */

                        /* "last" device for one FrameID that consumes the OutBound Frame */
						if ( (   (pSubframeBlock->SFIOCRProperties.DFPmode == pSubframeBlock->SubframeDataArray[(pSubframeBlock->NumberOfSubframeData-1)].Position)
                                 || (pSubframeBlock->SFIOCRProperties.DFPmode == pSubframeBlock->SubframeDataArray[0].Position)
                                )
                                   /* if Both Providers are present, it's the first PNIP Devices are in a stich */
							&& !(   (pSubframeBlock->PDIRFrameDataProvIdx != EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
								 && (pSubframeBlock->PDIRFrameDataProvIdxRed1 != EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
								)
                              )
						{
                            /* if both providers are not present, one of them is missing and the other is there. */
                            /* For one FrameID, this is the "last device" in MRPD that consumers the outbound frame */ 
							if (   (pSubframeBlock->PDIRFrameDataProvIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED) 
								&& (pSubframeBlock->PDIRFrameDataProvIdxRed1 == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
                               )
							{
                                /* Last device in stich. Here we have no no providers at all */
								/* --> Do nothing */
							}
							else
							{
								if (pSubframeBlock->PDIRFrameDataProvIdxRed1 == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED)
								{
									EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataProvIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);	
									pPDIRFrameDataProv = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataProvIdx];
								}
								else
								{
                                    /* Either this or redundant must be defined */
									EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataProvIdxRed1 == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
									pPDIRFrameDataProvRed1 = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataProvIdxRed1];
								}
							}
						}
						else
						{
							/* Here both (x) and (x+1) Outbound-Frames are forwarded to the 'next' device (x to one port, x+1 to the other port). We have two Providers */
                            EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataProvIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
							pPDIRFrameDataProv		= &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataProvIdx];
                            EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataProvIdxRed1 == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
							pPDIRFrameDataProvRed1  = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataProvIdxRed1];
						}

						/* Now that we know which Providers are present, calculate the DataLengths. */
                        /* Lengths for non-existent providers will be calculated as 0, but ignored at the check further down. */

						/* PathLayout: DG(1)...DG(X), but array is DG(x)...DG(1) */
						EDDP_PRMPDIRSubframeDataGet_CSF_SDU_DataLength (LSA_TRUE, DfpPosition, pSubframeBlock, &CSF_SDU_DataLengthCons, &CSF_SDU_DataLengthProv);

						if (pSubframeBlock->SFIOCRProperties.DFPRedundantPathLayout == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_DOWN)
						{
                            /* DG(x) + DG(x-1) ... DG1 */
							EDDP_PRMPDIRSubframeDataGet_CSF_SDU_DataLength (LSA_FALSE, DfpPosition, pSubframeBlock, &CSF_SDU_DataLengthConsRed1, &CSF_SDU_DataLengthProvRed1);
						}
						else
						{
                            /* DG(1) + DG(2) ... DGx. Since its the same as CSF_SDU_DataLengthProv and CSF_SDU_DataLengthCons, just copy here */
							/* same as in line topology */
							CSF_SDU_DataLengthConsRed1 = CSF_SDU_DataLengthCons;
							CSF_SDU_DataLengthProvRed1 = CSF_SDU_DataLengthProv;
						}
					}
                    else
                    {
                        EDDP_PRM_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRSubframeDataCheckSubframeDataLength(SubframeBlockIdx=%d, FrameID=0x%X, DFPmode=%d, DFPDirection=0x%X): FATAL -> IOD-MRPD SFCRC16(0x%X) is invalid!", 
                            SubframeBlockIdx, pSubframeBlock->FrameID, pSubframeBlock->SFIOCRProperties.DFPmode, pSubframeBlock->SFIOCRProperties.DFPDirection, pSubframeBlock->SFIOCRProperties.SFCRC16);
                        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
                    }
				}
                else
                {
                    EDDP_PRM_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRSubframeDataCheckSubframeDataLength(SubframeBlockIdx=%d, FrameID=0x%X, DFPmode=%d): FATAL -> IOD-MRPD DFPDirection(0x%X) is invalid!", 
                        SubframeBlockIdx, pSubframeBlock->FrameID, pSubframeBlock->SFIOCRProperties.DFPmode, pSubframeBlock->SFIOCRProperties.DFPDirection);
                    EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
                }
			}
			else
			{
                /* -------------------------------------------------------------------- */
                /* IOD and no MRPD                                                      */
                /* -------------------------------------------------------------------- */
                if (pSubframeBlock->SFIOCRProperties.DFPDirection == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPDIRECTION_INBOUND)
				{
                    /* ------------------------------------ */
                    /* IOD - InBound no MRPD                */
                    /* ------------------------------------ */
					if (pSubframeBlock->SFIOCRProperties.DFPmode == pSubframeBlock->SubframeDataArray[(pSubframeBlock->NumberOfSubframeData-1)].Position)
					{
                        /* last device in line, inbound -> only provider */
						EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataProvIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
						pPDIRFrameDataProv		= &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataProvIdx];
						SubframeLength			= (pSubframeBlock->SubframeDataArray[(pSubframeBlock->NumberOfSubframeData-1)].DataLength) + EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEHEADER_AND_SFCRC16_LENGTH;
						CSF_SDU_DataLengthProv  = SubframeLength;
					}
					else
					{
                        /* not last device in line, InBound -> provider and consumer are present */
						EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataProvIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
						pPDIRFrameDataProv  = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataProvIdx];
						EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataConsIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
						pPDIRFrameDataCons  = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataConsIdx];

						EDDP_PRMPDIRSubframeDataGet_CSF_SDU_DataLength (LSA_FALSE, DfpPosition, pSubframeBlock, &CSF_SDU_DataLengthProv, &CSF_SDU_DataLengthCons);
					}
				}
				else if (pSubframeBlock->SFIOCRProperties.DFPDirection == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPDIRECTION_OUTBOUND)
				{
                    /* ------------------------------------ */
                    /* IOD - OutBound no MRPD               */
                    /* ------------------------------------ */
					if (pSubframeBlock->SFIOCRProperties.SFCRC16 == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_SFCRC16_0)
					{
                        /* The controller doesnt support SFCRC16. No trunctation of the frames. The Frame length is the sum of the length of all subframes (whole packgroup) */
						for ( iSubfrData=0; iSubfrData < pSubframeBlock->NumberOfSubframeData ; iSubfrData++ )
						{
							SubframeLength = (pSubframeBlock->SubframeDataArray[iSubfrData].DataLength) + EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEHEADER_AND_SFCRC16_LENGTH;
							CSF_SDU_DataLengthProv += SubframeLength;
						}

                        if (pSubframeBlock->SFIOCRProperties.DFPmode != pSubframeBlock->SubframeDataArray[0].Position)
						{
							/* if its not the last device in the line topology, we also have a provider to forward the frame */
							EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataProvIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
							pPDIRFrameDataProv = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataProvIdx];
						}

						EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataConsIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
						pPDIRFrameDataCons      = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataConsIdx];
						CSF_SDU_DataLengthCons  = CSF_SDU_DataLengthProv; // whole packgroup, same length
					}
					else if (pSubframeBlock->SFIOCRProperties.SFCRC16 == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_SFCRC16_1)
					{
						/* OutBound and non-redundant path, Frame-Trunctation active */
						if (pSubframeBlock->SFIOCRProperties.DFPmode == pSubframeBlock->SubframeDataArray[0].Position)
						{
                            /* last device in line, outbound -> only consumer here */
							EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataConsIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
							pPDIRFrameDataCons = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataConsIdx];

							SubframeLength = (pSubframeBlock->SubframeDataArray[0].DataLength) + EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEHEADER_AND_SFCRC16_LENGTH;
							CSF_SDU_DataLengthCons = SubframeLength;
						}
						else
						{
                            /* not last device in line, OutBound -> provider and consumer present */
							EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataConsIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
							pPDIRFrameDataCons  = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataConsIdx];
							EDDP_ASSERT_FALSE(pSubframeBlock->PDIRFrameDataProvIdx == EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED);
							pPDIRFrameDataProv  = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[pSubframeBlock->PDIRFrameDataProvIdx];

							EDDP_PRMPDIRSubframeDataGet_CSF_SDU_DataLength (LSA_TRUE, DfpPosition, pSubframeBlock, &CSF_SDU_DataLengthCons, &CSF_SDU_DataLengthProv);
						}
					}
                    else
                    {
                        EDDP_PRM_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRSubframeDataCheckSubframeDataLength(SubframeBlockIdx=%d, FrameID=0x%X, DFPmode=%d, DFPDirection=0x%X): FATAL -> IOD SFCRC16(0x%X) is invalid!", 
                            SubframeBlockIdx, pSubframeBlock->FrameID, pSubframeBlock->SFIOCRProperties.DFPmode, pSubframeBlock->SFIOCRProperties.DFPDirection, pSubframeBlock->SFIOCRProperties.SFCRC16);
                        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
                    }
                }
                else
                {
                    EDDP_PRM_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRSubframeDataCheckSubframeDataLength(SubframeBlockIdx=%d, FrameID=0x%X, DFPmode=%d): FATAL -> IOD DFPDirection(0x%X) is invalid!", 
                        SubframeBlockIdx, pSubframeBlock->FrameID, pSubframeBlock->SFIOCRProperties.DFPmode, pSubframeBlock->SFIOCRProperties.DFPDirection);
                    EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
                }
			}
		}

        /* -------------------------------------------------------- */
        /* check DataLength                                         */
        /* -------------------------------------------------------- */
        /* CSF_SDU_DataLength must be incremented at [ SFHeaderCRC16(2) + SFEndDelimiter(2) ] */
		CSF_SDU_DataLengthProv		+= (EDDP_PRM_PDIRSUBFRAMEDATA_SFHEADERCRC16_SFENDDELIMITER_LENGTH);
		CSF_SDU_DataLengthCons		+= (EDDP_PRM_PDIRSUBFRAMEDATA_SFHEADERCRC16_SFENDDELIMITER_LENGTH);
		CSF_SDU_DataLengthProvRed1	+= (EDDP_PRM_PDIRSUBFRAMEDATA_SFHEADERCRC16_SFENDDELIMITER_LENGTH);
		CSF_SDU_DataLengthConsRed1  += (EDDP_PRM_PDIRSUBFRAMEDATA_SFHEADERCRC16_SFENDDELIMITER_LENGTH); 

        /* -------------------------------------------------------- */
        /* Provider FrameID(x)                                      */
        /* -------------------------------------------------------- */
		/* We have to check the DataLengths one by one, since not all entries have to be present at all cases (last Device in line, 'last device' in MRPD */
		if (!(LSA_HOST_PTR_ARE_EQUAL(pPDIRFrameDataProv, LSA_NULL)))
		{
            if (pPDIRFrameDataProv->DataLength <= EDD_CSRT_DATALEN_MIN)
			{
			    if (CSF_SDU_DataLengthProv > pPDIRFrameDataProv->DataLength)
			    {
                    EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDD_PRM_ERR_INDEX_DEFAULT, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
			        Response = EDD_STS_ERR_PRM_CONSISTENCY;
			    }
			}
			else
			{
			    if (CSF_SDU_DataLengthProv != pPDIRFrameDataProv->DataLength)
			    {
                    EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDD_PRM_ERR_INDEX_DEFAULT, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
			        Response = EDD_STS_ERR_PRM_CONSISTENCY;
			    }
			}

            if (Response != EDD_STS_OK)
			{
				EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
				EDDP_PRM_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataCheckSubframeDataLength(SubframeBlockIdx=%d, FrameID=0x%X, DFPmode=%d, DFPDirection=%d): ERROR -> CSF-SDU-DataLength(Prov=%d) does not match with PDIRData-DataLength(Prov=%d)!", 
				    SubframeBlockIdx, pSubframeBlock->FrameID, pSubframeBlock->SFIOCRProperties.DFPmode, pSubframeBlock->SFIOCRProperties.DFPDirection, CSF_SDU_DataLengthProv, pPDIRFrameDataProv->DataLength);
				return (Response);
			}
		}

        /* -------------------------------------------------------- */
        /* Provider FrameID(x+1)                                    */
        /* -------------------------------------------------------- */
        if (!(LSA_HOST_PTR_ARE_EQUAL(pPDIRFrameDataProvRed1, LSA_NULL)))
		{
            if (pPDIRFrameDataProvRed1->DataLength <= EDD_CSRT_DATALEN_MIN)
			{
			    if (CSF_SDU_DataLengthProvRed1 > pPDIRFrameDataProvRed1->DataLength)
			    {
                    EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDD_PRM_ERR_INDEX_DEFAULT, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
			        Response = EDD_STS_ERR_PRM_CONSISTENCY;
			    }
			}
			else
			{
			    if (CSF_SDU_DataLengthProvRed1 != pPDIRFrameDataProvRed1->DataLength)
			    {
                    EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDD_PRM_ERR_INDEX_DEFAULT, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
			        Response = EDD_STS_ERR_PRM_CONSISTENCY;
			    }
			}

            if (Response != EDD_STS_OK)
			{
				EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
				EDDP_PRM_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataCheckSubframeDataLength(SubframeBlockIdx=%d, FrameID=0x%X, DFPmode=%d, DFPDirection=%d): ERROR -> CSF-SDU-DataLength(ProvRed1=%d) does not match with PDIRData-DataLength(ProvRed1=%d)!", 
				    SubframeBlockIdx, pSubframeBlock->FrameID, pSubframeBlock->SFIOCRProperties.DFPmode, pSubframeBlock->SFIOCRProperties.DFPDirection, CSF_SDU_DataLengthProvRed1, pPDIRFrameDataProvRed1->DataLength);
                return (Response);
			}
		}

        /* -------------------------------------------------------- */
        /* Consumer FrameID(x)                                      */
        /* -------------------------------------------------------- */
        if (!(LSA_HOST_PTR_ARE_EQUAL(pPDIRFrameDataCons, LSA_NULL)))
		{
			if (pPDIRFrameDataCons->DataLength <= EDD_CSRT_DATALEN_MIN)
			{
			    if (CSF_SDU_DataLengthCons > pPDIRFrameDataCons->DataLength)
			    {
                    EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDD_PRM_ERR_INDEX_DEFAULT, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
			        Response = EDD_STS_ERR_PRM_CONSISTENCY;
			    }
			}
			else
			{
			    if (CSF_SDU_DataLengthCons != pPDIRFrameDataCons->DataLength)
			    {
                    EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDD_PRM_ERR_INDEX_DEFAULT, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
			        Response = EDD_STS_ERR_PRM_CONSISTENCY;
			    }
			}

            if (Response != EDD_STS_OK)
			{
				EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
				EDDP_PRM_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataCheckSubframeDataLength(SubframeBlockIdx=%d, FrameID=0x%X, DFPmode=%d, DFPDirection=%d): ERROR -> CSF-SDU-DataLength(Cons=%d) does not match with PDIRData-DataLength(Cons=%d)!", 
				    SubframeBlockIdx, pSubframeBlock->FrameID, pSubframeBlock->SFIOCRProperties.DFPmode, pSubframeBlock->SFIOCRProperties.DFPDirection, CSF_SDU_DataLengthCons, pPDIRFrameDataCons->DataLength);
                return (Response);
			}
		}

        /* -------------------------------------------------------- */
        /* Consumer FrameID(x+1)                                    */
        /* -------------------------------------------------------- */
        if (!(LSA_HOST_PTR_ARE_EQUAL(pPDIRFrameDataConsRed1, LSA_NULL)))
		{
            if (pPDIRFrameDataConsRed1->DataLength <= EDD_CSRT_DATALEN_MIN)
			{
			    if (CSF_SDU_DataLengthConsRed1 > pPDIRFrameDataConsRed1->DataLength)
			    {
                    EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDD_PRM_ERR_INDEX_DEFAULT, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
			        Response = EDD_STS_ERR_PRM_CONSISTENCY;
			    }
			}
			else
			{
			    if (CSF_SDU_DataLengthConsRed1 != pPDIRFrameDataConsRed1->DataLength)
			    {
                    EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDD_PRM_ERR_INDEX_DEFAULT, EDD_PRM_ERR_OFFSET_DEFAULT, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
			        Response = EDD_STS_ERR_PRM_CONSISTENCY;
			    }
			}

            if (Response != EDD_STS_OK)
			{
				EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
				EDDP_PRM_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRSubframeDataCheckSubframeDataLength(SubframeBlockIdx=%d, FrameID=0x%X, DFPmode=%d, DFPDirection=%d): ERROR -> CSF-SDU-DataLength(ConsRed1=%d) does not match with PDIRData-DataLength(ConsRed1=%d)!", 
				    SubframeBlockIdx, pSubframeBlock->FrameID, pSubframeBlock->SFIOCRProperties.DFPmode, pSubframeBlock->SFIOCRProperties.DFPDirection, CSF_SDU_DataLengthConsRed1, pPDIRFrameDataConsRed1->DataLength);
                return (Response);
			}
		}
    }
    else
    {
        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRSubframeDataCheckSubframeDataLength(): FATAL -> The record PDIRSubframeData(0x%X) or PDIRData(0x%X) is not present!", 
            pDDB->PRM.pRecordsetB->PDIRSubframeData_IsPresent, pDDB->PRM.pRecordsetB->PDIRData_IsPresent);
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }

    return (Response);
}

/*****************************************************************************/
/*  end of file eddp_prm_dfp.c                                               */
/*****************************************************************************/
