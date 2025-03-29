/** @file eddp_prm_pdirdata.c 
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
/*  F i l e               &F: eddp_prm_pdirdata.c                       :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP PRM (PDIRData) handling                     */
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

#define LTRC_ACT_MODUL_ID   92
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_PRM_PDIRDATA */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"            /* edd headerfiles */
#include "eddp_int.h"            /* internal header */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMPDIRDataRecord_Write                +*/
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
/*+  Description: This function parses the PDIRData record (pRecord)        +*/
/*+               and stores the extracted data into DDB.                   +*/
/*+               Moreover, the original record is stored in record set B.  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataRecord_CheckMaxPortDelays(
   	EDDP_LOCAL_DDB_PTR_TYPE         const pDDB,
   	EDDP_PRM_PDIRGLOBALDATA_TYPE  * const pPDIRGlobalData,
    LSA_UINT32                      const HWPortIndex,
    LSA_UINT32                    * const pErrOffset)
{

    if (HWPortIndex < pPDIRGlobalData->NumberOfPorts)
    {
        // check MaxPortTxDelay
        if ( !(EDDP_PRM_IS_MAXPORTTXDELAY_IN_VALID_RANGE(pPDIRGlobalData->DelayValues[HWPortIndex].MaxPortTxDelay, pDDB->SWI.LinkStatus[HWPortIndex+1].MaxPortTxDelay)) )
        {
            EDDP_PRM_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H:--] EDDP_PRMPDIRDataRecord_CheckMaxPortDelays(): PDIRGlobalData.MaxPortTxDelay(%d) is out of range [%d...%d]! Port(%d)", 
                pPDIRGlobalData->DelayValues[HWPortIndex].MaxPortTxDelay, 
                pDDB->SWI.LinkStatus[HWPortIndex+1].MaxPortTxDelay, 
                EDDP_PRM_PDIRGLOBALDATA_MAXPORTTXDELAY_MAX, 
                HWPortIndex+1);
            *pErrOffset = EDD_PRM_ERR_OFFSET_DEFAULT; 
            return EDD_STS_ERR_PARAM;
        }

        // check MaxPortRxDelay
        if ( !(EDDP_PRM_IS_MAXPORTRXDELAY_IN_VALID_RANGE(pPDIRGlobalData->DelayValues[HWPortIndex].MaxPortRxDelay, pDDB->SWI.LinkStatus[HWPortIndex+1].MaxPortRxDelay)) )
        {
            EDDP_PRM_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H:--] EDDP_PRMPDIRDataRecord_CheckMaxPortDelays(): PDIRGlobalData.MaxPortRxDelay(%d) is out of range [%d...%d]! Port(%d)", 
                pPDIRGlobalData->DelayValues[HWPortIndex].MaxPortRxDelay, 
                pDDB->SWI.LinkStatus[HWPortIndex+1].MaxPortRxDelay, 
                EDDP_PRM_PDIRGLOBALDATA_MAXPORTTXDELAY_MAX, 
                HWPortIndex+1);
            *pErrOffset = EDD_PRM_ERR_OFFSET_DEFAULT; 
            return EDD_STS_ERR_PARAM;
        }
    }
    return EDD_STS_OK;
}



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMPDIRDataRecord_Write                +*/
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
/*+  Description: This function parses the PDIRData record (pRecord)        +*/
/*+               and stores the extracted data into DDB.                   +*/
/*+               Moreover, the original record is stored in record set B.  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataRecord_Write(
   	EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
   	EDD_UPPER_MEM_U8_PTR_TYPE       pRecord)
{
    EDD_RSP                         Response;
    LSA_BOOL                        isRedPresent;
    LSA_UINT16                      val_16;
    LSA_UINT32                      val_32;
    LSA_UINT32                      i, j;
    LSA_UINT32                      PDIRData_RecordLength, PDIRGlobalData_RecordLength, PDIRFrameData_RecordLength, PDIRBeginEndData_RecordLength;
    LSA_UINT8                       BlockVersionLow, BlockVersionHigh;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_Begin;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_Tmp;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_PDIRData_RecordLength;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_PDIRBeginEndData_RecordLength;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_PDIRGlobalData_RecordLength;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_PDIRFrameData_RecordLength;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_PDIRGlobalData_Begin;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_PDIRFrameData_Begin;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_PDIRBeginEndData_Begin;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_PDIRGlobalData_BlockVersionLow;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_PDIRFrameData_BlockVersion;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_StartOfRedFrameID;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_EndOfRedFrameID;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_TxRedOrangePeriodBegin;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_TxOrangePeriodBegin;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_TxGreenPeriodBegin;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_RxRedOrangePeriodBegin;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_RxOrangePeriodBegin;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_RxGreenPeriodBegin;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_PDIRGlobalData_MaxLineRxDelay[EDDP_MAX_PORT_CNT];
    EDDP_PRM_DETAIL_ERR_PTR_TYPE    pDetailErr;

    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_PRMPDIRDataRecord_Write()");

    Response                        = EDD_STS_OK;
    pDetailErr                      = &pDDB->PrmDetailErr;
    isRedPresent                    = LSA_FALSE;
    /* PDIRData */
    PDIRData_RecordLength           = 0;
    pRecord_PDIRData_RecordLength   = LSA_NULL;
    pRecord_Begin                   = pRecord;
    /* PDIRGlobalData */
    PDIRGlobalData_RecordLength     = 0;
    pRecord_PDIRGlobalData_RecordLength = LSA_NULL;
    pRecord_PDIRGlobalData_Begin    = LSA_NULL;
    pRecord_PDIRGlobalData_BlockVersionLow = LSA_NULL;
    /* PDIRFrameData */
    PDIRFrameData_RecordLength      = 0;
    pRecord_PDIRFrameData_RecordLength = LSA_NULL;
    pRecord_PDIRFrameData_Begin     = LSA_NULL;
    /* PDIRBeginEndData */
    PDIRBeginEndData_RecordLength   = 0;
    pRecord_PDIRBeginEndData_RecordLength = LSA_NULL;
    pRecord_PDIRBeginEndData_Begin  = LSA_NULL;
    
    for (i = 0; i < EDDP_MAX_PORT_CNT; i++)
    {
        pRecord_PDIRGlobalData_MaxLineRxDelay[i] = LSA_NULL;
    }

    /* ------------------------------------------------------------------------ */
    /* delete values in new record PDIRData_B                                   */
    /* ------------------------------------------------------------------------ */
    pDDB->PRM.pPDIRData_B->MRPDConsumerWithOddFrameID_IsPresent             = LSA_FALSE;
    // LocalTransferStartEnd
    pDDB->PRM.pPDIRData_B->LocalTransferStartEnd.RxLocalTransferEndNs       = 0;
    pDDB->PRM.pPDIRData_B->LocalTransferStartEnd.TxLocalTransferStartNs     = 0;
    pDDB->PRM.pPDIRData_B->LocalTransferStartEnd.TxLocalTransferEndNs       = 0;
    pDDB->PRM.pPDIRData_B->LocalTransferStartEnd.RxMaxFSOWithFrameLengthNs  = 0;
    pDDB->PRM.pPDIRData_B->LocalTransferStartEnd.TxMinFSONs                 = 0;
    pDDB->PRM.pPDIRData_B->LocalTransferStartEnd.TxMaxFSOWithFrameLengthNs  = 0;
    // EndOfRedPhaseIFPorts
    pDDB->PRM.pPDIRData_B->EndOfRedPhaseIFPorts.RxNs                        = 0;
    pDDB->PRM.pPDIRData_B->EndOfRedPhaseIFPorts.TxNs                        = 0;
    pDDB->PRM.pPDIRData_B->EndOfRedPhaseIFPorts.RxTxNs                      = 0;

    /* ------------------------------------------------------------------------ */
    /* PDIRData                                                                 */
    /* ------------------------------------------------------------------------ */
    // check BlockType for PDIRData
    pRecord_Tmp = pRecord;
    EDDP_GET_U16_INCR(pRecord, val_16);
    if (val_16 != EDDP_PRM_PDIRDATA_BLOCKTYPE)
    {
        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
        Response = EDD_STS_ERR_PRM_BLOCK;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRData.BlockType(0x%X) != EDDP_PRM_PDIRDATA_BLOCKTYPE(0x%X)", 
            val_16, EDDP_PRM_PDIRDATA_BLOCKTYPE);
    }
    else
    {
        // check BlockLength of PDIRData
        pRecord_PDIRData_RecordLength = pRecord;
        pRecord_Tmp = pRecord;
        EDDP_GET_U16_INCR(pRecord, val_16);
        PDIRData_RecordLength = (val_16 + 4);
        if (PDIRData_RecordLength > pDDB->PRM.pRecordsetB->PDIRData_Length)
        {
            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
            Response = EDD_STS_ERR_PRM_BLOCK;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRData_RecordLength(%d) > PDIRData_Length(%d)!", 
                PDIRData_RecordLength, pDDB->PRM.pRecordsetB->PDIRData_Length);
        }
        else
        {
            // check BlockVersionHigh
            pRecord_Tmp = pRecord;
            BlockVersionHigh = *pRecord++;
            if (BlockVersionHigh != EDDP_PRM_PDIRDATA_BLOCKVERSIONHIGH)
            {
                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRData.BlockVersionHigh(0x%X) does not match!", 
                    BlockVersionHigh);
            }
            else
            {
                // check BlockVersionLow
                pRecord_Tmp = pRecord;
                BlockVersionLow = *pRecord++;
                if (BlockVersionLow != EDDP_PRM_PDIRDATA_BLOCKVERSIONLOW)
                {
                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                    Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRData.BlockVersionLow(0x%X) does not match!", 
                        BlockVersionLow);
                }
                else
                {
                    // check padding bytes
                    pRecord_Tmp = pRecord;
                    if (*pRecord++ != 0)
                    {
                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                        Response = EDD_STS_ERR_PRM_BLOCK;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRData.Padding is not '0'!");
                    }
                    else
                    {
                        // check padding bytes
                        pRecord_Tmp = pRecord;
                        if (*pRecord++ != 0)
                        {
                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                            Response = EDD_STS_ERR_PRM_BLOCK;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRData.Padding is not '0'!");
                        }
                        else
                        {
                            // check SlotNumber
                            pRecord_Tmp = pRecord;
                            EDDP_GET_U16_INCR(pRecord, val_16);
                            if (val_16 != pDDB->PRM.pPDIRData_B->SlotNumber)
                            {
                                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                Response = EDD_STS_ERR_PRM_BLOCK;
                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> Slotnumber(0x%X) != PDIRDataB.SlotNumber(0x%X)", 
                                    val_16, pDDB->PRM.pPDIRData_B->SlotNumber);
                            }
                            else
                            {
                                // check SubslotNumber
                                pRecord_Tmp = pRecord;
                                EDDP_GET_U16_INCR(pRecord, val_16);
                                if (val_16 != pDDB->PRM.pPDIRData_B->SubslotNumber)
                                {
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> Subslotnumber(0x%X) != PDIRDataB.SubslotNumber(0x%X)", 
                                        val_16, pDDB->PRM.pPDIRData_B->SubslotNumber);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    /* ------------------------------------------------------------------------ */
    /* PDIRData.PDIRGlobalData                                                  */
    /* ------------------------------------------------------------------------ */
    if (Response == EDD_STS_OK)
    {
        pRecord_PDIRGlobalData_Begin = pRecord;

        // check BlockType for PDIRGlobalData
        pRecord_Tmp = pRecord;
        EDDP_GET_U16_INCR(pRecord, val_16);
        if (val_16 != EDDP_PRM_PDIRGLOBALDATA_BLOCKTYPE)
        {
            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
            Response = EDD_STS_ERR_PRM_BLOCK;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRGlobalData.BlockType(0x%X) != EDDP_PRM_PDIRGLOBALDATA_BLOCKTYPE(0x%X)", 
                val_16, EDDP_PRM_PDIRGLOBALDATA_BLOCKTYPE);
        }

        // get BlockLength of PDIRGlobalData (this is checked below)
        pRecord_PDIRGlobalData_RecordLength = pRecord;
        EDDP_GET_U16_INCR(pRecord, val_16);
        PDIRGlobalData_RecordLength = (val_16 + 4);

        if (Response == EDD_STS_OK)
        {
            // check BlockVersionHigh for PDIRGlobalData
            pRecord_Tmp = pRecord;
            BlockVersionHigh = *pRecord++;
            if (BlockVersionHigh != EDDP_PRM_PDIRGLOBALDATA_BLOCKVERSIONHIGH)
            {
                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRGlobalData.BlockVersionHigh(0x%X) != EDDP_PRM_PDIRGLOBALDATA_BLOCKVERSIONHIGH(0x%X)", 
                    BlockVersionHigh, EDDP_PRM_PDIRGLOBALDATA_BLOCKVERSIONHIGH);
            }
            else
            {
                // check BlockVersionLow for PDIRGlobalData
                pRecord_PDIRGlobalData_BlockVersionLow = pRecord;
                pRecord_Tmp = pRecord;
                pDDB->PRM.pPDIRData_B->PDIRGlobalData.BlockVersionLow = *pRecord++;
                if (   (pDDB->PRM.pPDIRData_B->PDIRGlobalData.BlockVersionLow != EDDP_PRM_PDIRGLOBALDATA_BLOCKVERSIONLOW) 
                    && (pDDB->PRM.pPDIRData_B->PDIRGlobalData.BlockVersionLow != 0x02)
                   )
                {
                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                    Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRGlobalData.BlockVersionLow(0x%X) does not match!", 
                        pDDB->PRM.pPDIRData_B->PDIRGlobalData.BlockVersionLow);
                }
                else
                {
                    // check padding bytes
                    pRecord_Tmp = pRecord;
                    if (*pRecord++ != 0)
                    {
                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                        Response = EDD_STS_ERR_PRM_BLOCK;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRGlobalData.Padding is not '0'!");
                    }
                    else
                    {
                        pRecord_Tmp = pRecord;
                        if (*pRecord++ != 0)
                        {
                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                            Response = EDD_STS_ERR_PRM_BLOCK;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRGlobalData.Padding is not '0'!");
                        }
                    }
                }
            }
        }
    }

    if (Response == EDD_STS_OK)
    {
        // store IRDataUUID
        EDDP_GET_U32_INCR(pRecord, val_32);
        pDDB->PRM.pPDIRData_B->PDIRGlobalData.IRDataUUID.UUID_part_1              = val_32;
        EDDP_GET_U16_INCR(pRecord, val_16);
        pDDB->PRM.pPDIRData_B->PDIRGlobalData.IRDataUUID.UUID_part_2              = val_16;
        EDDP_GET_U16_INCR(pRecord, val_16);
        pDDB->PRM.pPDIRData_B->PDIRGlobalData.IRDataUUID.UUID_part_3              = val_16;
        EDDP_GET_U16_INCR(pRecord, val_16);
        pDDB->PRM.pPDIRData_B->PDIRGlobalData.IRDataUUID.UUID_part_4              = val_16;    
        pDDB->PRM.pPDIRData_B->PDIRGlobalData.IRDataUUID.UUID_part_5_array[0]     = *pRecord++;
        pDDB->PRM.pPDIRData_B->PDIRGlobalData.IRDataUUID.UUID_part_5_array[1]     = *pRecord++;
        pDDB->PRM.pPDIRData_B->PDIRGlobalData.IRDataUUID.UUID_part_5_array[2]     = *pRecord++;
        pDDB->PRM.pPDIRData_B->PDIRGlobalData.IRDataUUID.UUID_part_5_array[3]     = *pRecord++;
        pDDB->PRM.pPDIRData_B->PDIRGlobalData.IRDataUUID.UUID_part_5_array[4]     = *pRecord++;
        pDDB->PRM.pPDIRData_B->PDIRGlobalData.IRDataUUID.UUID_part_5_array[5]     = *pRecord++;

        // store MaxBridgeDelay
        pRecord_Tmp = pRecord;
        EDDP_GET_U32_INCR(pRecord, val_32);
        pDDB->PRM.pPDIRData_B->PDIRGlobalData.MaxBridgeDelay = val_32;
        // check MaxBridgeDelay
        if ( !(EDDP_PRM_IS_MAXBRIDGEDELAY_IN_VALID_RANGE(pDDB->PRM.pPDIRData_B->PDIRGlobalData.MaxBridgeDelay)) )
        {
            pDetailErr->OffsetMaxBridgeDelay = pRecord_Tmp - pRecord_Begin;
            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
            Response = EDD_STS_ERR_PRM_BLOCK;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRGlobalData.MaxBridgeDelay(%d) is out of range [%d...%d]", 
                pDDB->PRM.pPDIRData_B->PDIRGlobalData.MaxBridgeDelay, EDDP_PRM_PDIRGLOBALDATA_MAXBRIDGEDELAY_MIN, EDDP_PRM_PDIRGLOBALDATA_MAXBRIDGEDELAY_MAX);
        }
    }

    if (Response == EDD_STS_OK)
    {
        // store NumberOfPorts
        pRecord_Tmp = pRecord;
        EDDP_GET_U32_INCR(pRecord, val_32);
        pDDB->PRM.pPDIRData_B->PDIRGlobalData.NumberOfPorts = val_32;
        // check NumberOfPorts
        if (pDDB->PRM.pPDIRData_B->PDIRGlobalData.NumberOfPorts > pDDB->SWI.PortCnt)
        {
            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
            Response = EDD_STS_ERR_PRM_BLOCK;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRGlobalData.NumberOfPorts(%d) > SWI.PortCnt(%d)!", 
                pDDB->PRM.pPDIRData_B->PDIRGlobalData.NumberOfPorts, pDDB->SWI.PortCnt);
        }

        if (Response == EDD_STS_OK)
        {
            // store MaxPortTxDelay, MaxPortRxDelay, [MaxLineRxDelay, YellowTime]
            for ( i=0; i < pDDB->PRM.pPDIRData_B->PDIRGlobalData.NumberOfPorts; i++ )
            {
                pDDB->PRM.pPDIRData_B->PDIRGlobalData.DelayValues[i].YellowTime_IsPresent = LSA_FALSE;

                // store MaxPortTxDelay
                pRecord_Tmp = pRecord;
                EDDP_GET_U32_INCR(pRecord, val_32);
                pDDB->PRM.pPDIRData_B->PDIRGlobalData.DelayValues[i].MaxPortTxDelay = val_32;

                // store MaxPortRxDelay
                pRecord_Tmp = pRecord;
                EDDP_GET_U32_INCR(pRecord, val_32);
                pDDB->PRM.pPDIRData_B->PDIRGlobalData.DelayValues[i].MaxPortRxDelay = val_32;

                /* ------------------------------------ */
                /* Parameter for PDIRGlobalData V1.2    */
                /* ------------------------------------ */
                if ((Response == EDD_STS_OK) && (pDDB->PRM.pPDIRData_B->PDIRGlobalData.BlockVersionLow == 0x2))
                {
                    // store MaxLineRxDelay
                    pRecord_PDIRGlobalData_MaxLineRxDelay[i] = pRecord;
                    EDDP_GET_U32_INCR(pRecord, val_32);
                    pDDB->PRM.pPDIRData_B->PDIRGlobalData.DelayValues[i].MaxLineRxDelay = val_32;
                    // check MaxLineRxDelay
                    if ( !(EDDP_PRM_IS_MAXLINERXDELAY_IN_VALID_RANGE(pDDB->PRM.pPDIRData_B->PDIRGlobalData.DelayValues[i].MaxLineRxDelay)) )
                    {
                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_PDIRGlobalData_MaxLineRxDelay[i] - pRecord_Begin);
                        Response = EDD_STS_ERR_PRM_BLOCK;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_PRM_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRGlobalData.MaxLineRxDelay(%d) is out of range [%d...%d]! Port(%d)", 
                            pDDB->PRM.pPDIRData_B->PDIRGlobalData.DelayValues[i].MaxLineRxDelay, 
                            EDDP_PRM_PDIRGLOBALDATA_MAXLINERXDELAY_MIN, 
                            EDDP_PRM_PDIRGLOBALDATA_MAXLINERXDELAY_MAX, 
                            i+1);
                        // exit FOR loop "NumberOfPorts"
                        i = pDDB->PRM.pPDIRData_B->PDIRGlobalData.NumberOfPorts;
                    }
                    else
                    {
                        // store YellowTime
                        pRecord_Tmp = pRecord;
                        EDDP_GET_U32_INCR(pRecord, val_32);
                        pDDB->PRM.pPDIRData_B->PDIRGlobalData.DelayValues[i].YellowTime = val_32;
                        // check YellowTime
                        if ( !(EDDP_PRMPDIRDataChekYellowTimeInValidRange(pDDB, pDDB->PRM.pPDIRData_B->PDIRGlobalData.DelayValues[i].YellowTime)) )
                        {
                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                            Response = EDD_STS_ERR_PRM_BLOCK;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRGlobalData.YellowTime(%d) is out of range [%d/%d...%d]! Port(%d)", 
                                pDDB->PRM.pPDIRData_B->PDIRGlobalData.DelayValues[i].YellowTime, 
                                EDDP_PRM_PDIRGLOBALDATA_YELLOWTIME_MIN_PNIP_REV1, 
                                EDDP_PRM_PDIRGLOBALDATA_YELLOWTIME_MIN_PNIP_REV2,
                                EDDP_PRM_PDIRGLOBALDATA_YELLOWTIME_MAX, 
                                i+1);
                            // exit FOR loop "NumberOfPorts"
                            i = pDDB->PRM.pPDIRData_B->PDIRGlobalData.NumberOfPorts;
                        }
                        // YellowTime is present in PDIRGlobalData
                        pDDB->PRM.pPDIRData_B->PDIRGlobalData.DelayValues[i].YellowTime_IsPresent = LSA_TRUE;
                    }
                }
            }
        }
    }

    // check if number of read bytes (in PDIRGlobalData) is same as record length
    if (Response == EDD_STS_OK)
    {
        if ( ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_PDIRGlobalData_Begin) != PDIRGlobalData_RecordLength )
        {
            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_PDIRGlobalData_RecordLength - pRecord_Begin);
            Response = EDD_STS_ERR_PRM_BLOCK;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> number of read PDIRGlobalData bytes(%d) != PDIRGlobalData_RecordLength(%d)", 
                ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_PDIRGlobalData_Begin), PDIRGlobalData_RecordLength);
        }
    }

    /* ------------------------------------------------------------------------ */
    /* PDIRData.PDIRFrameData                                                   */
    /*  -> after PDIRGlobalData, because of checking                            */
    /*     PDIRFrameData.BlockVersion against PDIRGlobalData.BlockVersion       */
    /* ------------------------------------------------------------------------ */
    if (Response == EDD_STS_OK)
    {
        pRecord_PDIRFrameData_Begin = pRecord;

        // check if next BlockType is PDIRFrameData
        EDDP_GET_U16_INCR(pRecord, val_16);
        if (val_16 == EDDP_PRM_PDIRFRAMEDATA_BLOCKTYPE)
        {
            // get BlockLength and calculate number of IRData entries (BlockLength is checked below)
            pRecord_PDIRFrameData_RecordLength = pRecord;
            pRecord_Tmp = pRecord;
            EDDP_GET_U16_INCR(pRecord, val_16);
            PDIRFrameData_RecordLength = (val_16 + 4);
            // store of IRData entries
            pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All = ((val_16 - 4) / EDDP_PRM_PDIRFRAMEDATA_ONE_ENTRY_SIZE);
            // check number of IRData entries
            if (pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All > pDDB->PRM.pPDIRData_B->IRFrameDataArray_Max_Entries)
            {
                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                Response = EDD_STS_ERR_PRM_BLOCK;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.NumberOfIRDataEntries(%d) > NumberOfIRDataEntriesMax(%d)!", 
                    pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All, pDDB->PRM.pPDIRData_B->IRFrameDataArray_Max_Entries);
            }

            if (Response == EDD_STS_OK)
            {
                // check BlockVersion of PDIRFrameData
                // V1.0     : without   field "FrameDataProperties"
                // V1.1     : with      field "FrameDataProperties"
                pRecord_PDIRFrameData_BlockVersion     = pRecord;
                BlockVersionHigh                       = *pRecord++;
                pRecord_Tmp                            = pRecord;
                BlockVersionLow                        = *pRecord++;

                if ((BlockVersionHigh == 0x01) && (BlockVersionLow == 0x00))
                {
                    // BlockVersion V1.0
                    // BlockVersion-PDIRFrameData V1.0 is only with BlockVersion-PDIRGlobalData V1.1 permitted
                    if (pDDB->PRM.pPDIRData_B->PDIRGlobalData.BlockVersionLow != 0x1)
                    {
                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_PDIRGlobalData_BlockVersionLow - pRecord_Begin);
                        Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.BlockVersion V1.0 is only permitted with PDIRGlobalData.BlockVersion V1.1! PDIRGlobalData.BlockVersionLow=0x%X", 
                            pDDB->PRM.pPDIRData_B->PDIRGlobalData.BlockVersionLow);
                    }
                    else
                    {
                        // FrameDataProperties is not present
                        pDDB->PRM.pPDIRData_B->FrameDataProperties = 0x0;
                        pDDB->PRM.pPDIRData_B->FrameDataProperties_IsPresent = LSA_FALSE;
                        // only use AbsoluteTime-Mode
                        pDDB->PRM.pPDIRData_B->AbsoluteTimeMode = LSA_TRUE;

                        // check padding bytes
                        pRecord_Tmp = pRecord;
                        if (*pRecord++ != 0)
                        {
                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                            Response = EDD_STS_ERR_PRM_BLOCK;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.Padding is not '0'!");
                        }
                        else
                        {
                            // check padding bytes
                            pRecord_Tmp = pRecord;
                            if (*pRecord++ != 0)
                            {
                                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                Response = EDD_STS_ERR_PRM_BLOCK;
                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.Padding is not '0'!");
                            }
                        }
                    }

                    if (Response == EDD_STS_OK)
                    {
                        pDDB->PRM.pPDIRData_B->Sync_IsPresent = LSA_FALSE;

                        // FrameData
                        for ( i=0; i < pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All; i++ )
                        {
                            // store FrameSendOffset
                            pRecord_Tmp = pRecord;
                            EDDP_GET_U32_INCR(pRecord, val_32);
                            pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].OffsetFrameSendOffset = pRecord_Tmp - pRecord_Begin;
                            pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].FrameSendOffset = val_32;
                            // check FrameSendOffset will be done on PrmEnd

                            // store DataLength
                            pRecord_Tmp = pRecord;
                            EDDP_GET_U16_INCR(pRecord, val_16);
                            pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].DataLength = val_16;
                            // check DataLength
                            if ( !(EDDP_PRM_IS_DATALENGTH_IN_VALID_RANGE(pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].DataLength)) )
                            {
                                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                Response = EDD_STS_ERR_PRM_BLOCK;
                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                EDDP_PRM_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.DataLength(%d) is out of range [%d...%d], FrameDataArray-Entry=%d!", 
                                    pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].DataLength, 
                                    EDDP_PRM_PDIRFRAMEDATA_DATALENGTH_MIN, 
                                    EDDP_PRM_PDIRFRAMEDATA_DATALENGTH_MAX, 
                                    i);
                                // exit FOR loop "FrameData"
                                i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                            }

                            if (Response == EDD_STS_OK)
                            {
                                // store ReductionRatio
                                pRecord_Tmp = pRecord;
                                EDDP_GET_U16_INCR(pRecord, val_16);
                                pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].ReductionRatio = val_16;
                                // check ReductionRatio (range)
                                if ( !(EDDP_PRM_IS_REDUCTIONRATIO_RTC3_IN_VALID_RANGE(pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].ReductionRatio)) )
                                {
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.ReductionRatio(%d) is out of range [%d...%d], FrameDataArray-Entry=%d", 
                                        pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].ReductionRatio, 
                                        EDD_CSRT_CYCLE_REDUCTION_RATIO_RTC3_MIN,
                                        EDD_CSRT_CYCLE_REDUCTION_RATIO_RTC3_MAX,
                                        i);
                                    // exit FOR loop "IRFrameDataArray_Count_All"
                                    i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                                }
                                // check ReductionRatio (power of two)
                                if ((Response == EDD_STS_OK) && !(EDDP_CRTCheckIsCycleReductionRatioPowerOfTwo(pDDB, pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].ReductionRatio)))
                                {
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.ReductionRatio(%d) is not a power of two, FrameDataArray-Entry=%d", 
                                        pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].ReductionRatio,
                                        i);
                                    // exit FOR loop "IRFrameDataArray_Count_All"
                                    i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                                }

                                // store Phase
                                pRecord_Tmp = pRecord;
                                EDDP_GET_U16_INCR(pRecord, val_16);
                                pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].Phase = val_16;
                                // check Phase against ReductionRatio
                                if (pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].Phase > pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].ReductionRatio)
                                {
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.Phase(%d) > ReductionRatio(%d), FrameDataArray-Entry=%d!", 
                                        pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].Phase, 
                                        pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].ReductionRatio, 
                                        i);
                                    // exit FOR loop "FrameData"
                                    i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                                }
                            }

                            if (Response == EDD_STS_OK)
                            {
                                // store FrameID
                                pRecord_Tmp = pRecord;
                                EDDP_GET_U16_INCR(pRecord, val_16);
                                pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].OffsetFrameID = pRecord_Tmp - pRecord_Begin;
                                pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].FrameID = val_16;
                                // store Flag if Sync is present
                                if (pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].FrameID == EDD_SRT_FRAMEID_PTCP_SYNC_RT_CLASS3)
                                {
                                    pDDB->PRM.pPDIRData_B->Sync_IsPresent = LSA_TRUE;
                                }
                                else
                                {
                                    // check this FrameID is in valid range
                                    if ( !(EDDP_IS_IRT_FRAMEID(pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].FrameID)) )
                                    {
                                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                        Response = EDD_STS_ERR_PRM_BLOCK;
                                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                        EDDP_PRM_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.FrameID(0x%X) is out of range [0x%X...0x%X], FrameDataArray-Entry=%d!", 
                                            pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].FrameID, EDD_SRT_FRAMEID_IRT_START, EDD_SRT_FRAMEID_IRT_STOP, i);
                                        // exit FOR loop "IRFrameDataArray_Count_All"
                                        i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                                    }
                                }
                            }

                            if (Response == EDD_STS_OK)
                            {
                                // store EtherType
                                pRecord_Tmp = pRecord;
                                EDDP_GET_U16_INCR(pRecord, val_16);
                                pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].EtherType = val_16;
                                // check EtherType
                                if (pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].EtherType != EDDP_CRT_ETHER_TYPE)
                                {
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.EtherType(0x%X) != 0x%X, FrameDataArray-Entry=%d!", 
                                        pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].EtherType, EDDP_CRT_ETHER_TYPE, i);
                                    // exit FOR loop "IRFrameDataArray_Count_All"
                                    i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                                }
                            }

                            if (Response == EDD_STS_OK)
                            {
                                // store RxPort
                                pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].RxPort = *pRecord++;
                                // store FrameDetails
                                pRecord_Tmp = pRecord;
                                pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].FrameDetails = *pRecord++;
                                // check FrameDetails (always after store RxPort, because RxPort will be used here!!!)
                                Response = EDDP_PRMPDIRDataCheckFrameDetails (pDDB, (LSA_UINT16)i);
                                if (Response != EDD_STS_OK) // that means Response == EDD_STS_ERR_PRM_CONSISTENCY
                                {
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                    // exit FOR loop "IRFrameDataArray_Count_All"
                                    i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                                }
                            }

                            if (Response == EDD_STS_OK)
                            {
                                // store NumberOfTxPortGroups
                                pRecord_Tmp = pRecord;
                                pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].NumberOfTxPortGroups = *pRecord++;
                                // check NumberOfTxPortGroups
                                if (pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].NumberOfTxPortGroups != 0x01)
                                {
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.NumberOfTxPortGroups(%d) != 1", 
                                        pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].NumberOfTxPortGroups);
                                    // exit FOR loop "IRFrameDataArray_Count_All"
                                    i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                                }
                            }

                            if (Response == EDD_STS_OK)
                            {
                                // store TxPortGroupArray
                                pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].TxPortGroupArray  = *pRecord++;
                                // init with invalid
                                pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].TxPort            = 0;
                                pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].SndRcvRole        = EDDP_PRM_PDIRFRAMEDATA_ROLE_INVALID;
                            }

                            if (Response == EDD_STS_OK)
                            {
                                // check padding bytes
                                pRecord_Tmp = pRecord;
                                if (*pRecord++ != 0)
                                {
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.Padding is not '0'!");
                                    // exit FOR loop "IRFrameDataArray_Count_All"
                                    i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                                }
                                else
                                {
                                    // check padding bytes
                                    pRecord_Tmp = pRecord;
                                    if (*pRecord++ != 0)
                                    {
                                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                        Response = EDD_STS_ERR_PRM_BLOCK;
                                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.Padding is not '0'!");
                                        // exit FOR loop "IRFrameDataArray_Count_All"
                                        i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                                    }
                                }
                            }

                            if (Response == EDD_STS_OK)
                            {
                                pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].isDoubleFrameID   = LSA_FALSE;
                                pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].SubframeBlockIdx  = EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED;
                            }
                        }
                    }
                }
                else
                {
                    if ((BlockVersionHigh == 0x01) && (BlockVersionLow == 0x01))
                    {
                        // BlockVersion V1.1
                        // BlockVersion-PDIRFrameData V1.1 is only with BlockVersion-PDIRGlobalData V1.2 permitted
                        if (pDDB->PRM.pPDIRData_B->PDIRGlobalData.BlockVersionLow != 0x2)
                        {
                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_PDIRGlobalData_BlockVersionLow - pRecord_Begin);
                            Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.BlockVersion V1.1 is only permitted with PDIRGlobalData.BlockVersion V1.2! PDIRGlobalData.BlockVersionLow=0x%X", 
                                pDDB->PRM.pPDIRData_B->PDIRGlobalData.BlockVersionLow);
                        }
                        else
                        {
                            // check padding bytes
                            pRecord_Tmp = pRecord;
                            if (*pRecord++ != 0)
                            {
                                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                Response = EDD_STS_ERR_PRM_BLOCK;
                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.Padding is not '0'!");
                            }
                            else
                            {
                                // check padding bytes
                                pRecord_Tmp = pRecord;
                                if (*pRecord++ != 0)
                                {
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.Padding is not '0'!");
                                }
                            }
                        }

                        if (Response == EDD_STS_OK)
                        {
                            // store FrameDataProperties
                            pRecord_Tmp = pRecord;
                            EDDP_GET_U32_INCR(pRecord, val_32);
                            pDDB->PRM.pPDIRData_B->FrameDataProperties = val_32;
                            pDDB->PRM.pPDIRData_B->FrameDataProperties_IsPresent = LSA_TRUE;

                            // check FrameDataProperties.ForwardingMode: AbsoluteTime or RelativeTime-Mode?
                            val_32 = EDDP_GET_BIT_VALUE( pDDB->PRM.pPDIRData_B->FrameDataProperties,
                                                         EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FORWARDINGMODE_MASK, 
                                                         EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FORWARDINGMODE_SHIFT);

                            if ( (val_32 == EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FORWARDINGMODE_ABSOLUTE) 
                                && (EDD_DPB_FEATURE_IRTFWDMODE_SUPPORTED_ABSOLUTE & pDDB->FeatureSupport.IRTForwardingModeSupported) )
                            {
                                pDDB->PRM.pPDIRData_B->AbsoluteTimeMode = LSA_TRUE;       // use AbsoluteTime-Mode
                            }
                            else
                            {
                                if ( (val_32 == EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FORWARDINGMODE_RELATIVE)
                                    && (EDD_DPB_FEATURE_IRTFWDMODE_SUPPORTED_RELATIVE & pDDB->FeatureSupport.IRTForwardingModeSupported) )
                                {
                                    pDDB->PRM.pPDIRData_B->AbsoluteTimeMode = LSA_FALSE;      // use RelativeTime-Mode
                                }
                                else
                                {
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.FrameDataProperties.ForwardingMode(0x%X) is not valid!", 
                                        val_32);
                                    // exit FOR loop "IRFrameDataArray_Count_All"
                                    i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                                }
                            }

                            // check FrameDataProperties.FastForwardingMulticastMACAdd
                            if (Response == EDD_STS_OK)
                            {
                                val_32 = EDDP_GET_BIT_VALUE( pDDB->PRM.pPDIRData_B->FrameDataProperties,
                                                             EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_MASK, 
                                                             EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_SHIFT);
                                if (   (val_32 != EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_LEGACY)
                                    && (val_32 != EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_RTC3)
                                    && (val_32 != EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_FFW)
                                   )
                                {
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.FrameDataProperties.FastForwardingMulticastMACAdd(0x%X) is not valid!", 
                                        val_32);
                                    // exit FOR loop "IRFrameDataArray_Count_All"
                                    i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                                }
                            }

                            // check FrameDataProperties.FragmentationMode
                            if (Response == EDD_STS_OK)
                            {
                                val_32 = EDDP_GET_BIT_VALUE( pDDB->PRM.pPDIRData_B->FrameDataProperties,
                                                             EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_MASK, 
                                                             EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_SHIFT);

                                if (   (val_32 != EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_OFF)
                                    && (val_32 != EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_ON_FRAG_SIZE_128)
                                    && (val_32 != EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_ON_FRAG_SIZE_256)
                                   )
                                {
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.FrameDataProperties.FragmentationMode(0x%X) is not valid!", 
                                        val_32);
                                    // exit FOR loop "IRFrameDataArray_Count_All"
                                    i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                                }
                                else if( (EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_OFF != val_32)
                                    && (EDD_DPB_FEATURE_FRAGTYPE_SUPPORTED_NO == pDDB->FeatureSupport.FragmentationtypeSupported) )
                                {
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> fragmentation type not supported (0x%X)!",
                                        val_32);
                                    // exit FOR loop "IRFrameDataArray_Count_All"
                                    i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                                }
                                else
                                {
                                    // PN-IP Rev1: Fragmentation is not supported
                                    #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
                                    if (EDDP_HAL_HW_PNIP_REV1 == EDDP_HALGetPnipRevision(pDDB))
                                    {
                                        if (   (val_32 == EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_ON_FRAG_SIZE_128)
                                            || (val_32 == EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_ON_FRAG_SIZE_256)
                                           )
                                        {
                                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                            pDetailErr->OffsetFragmentationOn = pRecord_Tmp - pRecord_Begin;
                                            Response = EDD_STS_ERR_PRM_BLOCK;
                                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.FrameDataProperties.FragmentationMode(0x%X): Fragmentation = ON is not supported with PN-IP Rev1!", 
                                                val_32);
                                            // exit FOR loop "IRFrameDataArray_Count_All"
                                            i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                                        }
                                    }
                                    #endif  // EDDP_CFG_HW_PNIP_REV1_SUPPORT
                                }
                            }
                        }

                        if (Response == EDD_STS_OK)
                        {
                            pDDB->PRM.pPDIRData_B->Sync_IsPresent = LSA_FALSE;

                            // FrameData
                            for ( i=0; i < pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All; i++ )
                            {
                                // store FrameSendOffset
                                EDDP_GET_U32_INCR(pRecord, val_32);
                                pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].OffsetFrameSendOffset = pRecord_Tmp - pRecord_Begin;
                                pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].FrameSendOffset = val_32;
                                // check FrameSendOffset will be done on PrmEnd

                                // store DataLength
                                pRecord_Tmp = pRecord;
                                EDDP_GET_U16_INCR(pRecord, val_16);
                                pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].DataLength = val_16;
                                // check DataLength
                                if ( !(EDDP_PRM_IS_DATALENGTH_IN_VALID_RANGE(pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].DataLength)) )
                                {
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.DataLength(%d) is out of range [%d...%d], FrameDataArray-Entry=%d!", 
                                        pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].DataLength, 
                                        EDDP_PRM_PDIRFRAMEDATA_DATALENGTH_MIN, 
                                        EDDP_PRM_PDIRFRAMEDATA_DATALENGTH_MAX, 
                                        i);
                                    // exit FOR loop "FrameData"
                                    i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                                }

                                if (Response == EDD_STS_OK)
                                {
                                    // store ReductionRatio
                                    pRecord_Tmp = pRecord;
                                    EDDP_GET_U16_INCR(pRecord, val_16);
                                    pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].ReductionRatio = val_16;
                                    // check ReductionRatio (range)
                                    if ( !(EDDP_PRM_IS_REDUCTIONRATIO_RTC3_IN_VALID_RANGE(pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].ReductionRatio)) )
                                    {
                                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                        Response = EDD_STS_ERR_PRM_BLOCK;
                                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                        EDDP_PRM_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.ReductionRatio(%d) is out of range [%d...%d], FrameDataArray-Entry=%d", 
                                            pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].ReductionRatio, 
                                            EDD_CSRT_CYCLE_REDUCTION_RATIO_RTC3_MIN,
                                            EDD_CSRT_CYCLE_REDUCTION_RATIO_RTC3_MAX,
                                            i);
                                        // exit FOR loop "IRFrameDataArray_Count_All"
                                        i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                                    }
                                    // check ReductionRatio (power of two)
                                    if ((Response == EDD_STS_OK) && !(EDDP_CRTCheckIsCycleReductionRatioPowerOfTwo(pDDB, pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].ReductionRatio)))
                                    {
                                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                        Response = EDD_STS_ERR_PRM_BLOCK;
                                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.ReductionRatio(%d) is not a power of two, FrameDataArray-Entry=%d", 
                                            pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].ReductionRatio,
                                            i);
                                        // exit FOR loop "IRFrameDataArray_Count_All"
                                        i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                                    }

                                    if (EDD_STS_OK == Response)
                                    {
                                        // store Phase
                                        pRecord_Tmp = pRecord;
                                        EDDP_GET_U16_INCR(pRecord, val_16);
                                        pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].Phase = val_16;
                                        // check Phase against ReductionRatio
                                        if (pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].Phase > pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].ReductionRatio)
                                        {
                                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                            Response = EDD_STS_ERR_PRM_BLOCK;
                                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                            EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.Phase(%d) > ReductionRatio(%d), FrameDataArray-Entry=%d!", 
                                                pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].Phase, 
                                                pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].ReductionRatio, 
                                                i);
                                            // exit FOR loop "FrameData"
                                            i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                                        }
                                    }
                                }

                                if (Response == EDD_STS_OK)
                                {
                                    // store FrameID
                                    pRecord_Tmp = pRecord;
                                    EDDP_GET_U16_INCR(pRecord, val_16);
                                    pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].OffsetFrameID = pRecord_Tmp - pRecord_Begin;
                                    pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].FrameID = val_16;
                                    // store Flag if Sync is present
                                    if (pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].FrameID == EDD_SRT_FRAMEID_PTCP_SYNC_RT_CLASS3)
                                    {
                                        pDDB->PRM.pPDIRData_B->Sync_IsPresent = LSA_TRUE;
                                    }
                                    else
                                    {
                                        // check this FrameID is in valid range
                                        if ( !(EDDP_IS_IRT_FRAMEID(pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].FrameID)) )
                                        {
                                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                            Response = EDD_STS_ERR_PRM_BLOCK;
                                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                            EDDP_PRM_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.FrameID(0x%X) is out of range [0x%X...0x%X], FrameDataArray-Entry=%d, AbsoluteTimeMode=0x%X", 
                                                pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].FrameID, EDD_SRT_FRAMEID_IRT_START, 
                                                EDD_SRT_FRAMEID_IRT_STOP, i, 
                                                pDDB->PRM.pPDIRData_B->AbsoluteTimeMode);
                                            // exit FOR loop "IRFrameDataArray_Count_All"
                                            i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                                        }
                                    }
                                }

                                if (Response == EDD_STS_OK)
                                {
                                    // store EtherType
                                    pRecord_Tmp = pRecord;
                                    EDDP_GET_U16_INCR(pRecord, val_16);
                                    pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].EtherType = val_16;
                                    // check EtherType
                                    if (pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].EtherType != EDDP_CRT_ETHER_TYPE)
                                    {
                                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                        Response = EDD_STS_ERR_PRM_BLOCK;
                                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                        EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.EtherType(0x%X) != 0x%X, FrameDataArray-Entry=%d!", 
                                            pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].EtherType, EDDP_CRT_ETHER_TYPE, i);
                                        // exit FOR loop "IRFrameDataArray_Count_All"
                                        i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                                    }
                                }

                                if (Response == EDD_STS_OK)
                                {
                                    // store RxPort
                                    pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].RxPort = *pRecord++;

                                    // store FrameDetails
                                    pRecord_Tmp = pRecord;
                                    pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].FrameDetails = *pRecord++;
                                    // check FrameDetails (always after store RxPort, because RxPort will be used here!!!)
                                    Response = EDDP_PRMPDIRDataCheckFrameDetails (pDDB, (LSA_UINT16)i);
                                    if (Response != EDD_STS_OK)
                                    {
                                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                        // exit FOR loop "IRFrameDataArray_Count_All"
                                        i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                                    }
                                }

                                if (Response == EDD_STS_OK)
                                {
                                    // store NumberOfTxPortGroups
                                    pRecord_Tmp = pRecord;
                                    pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].NumberOfTxPortGroups = *pRecord++;
                                    // check NumberOfTxPortGroups
                                    if (pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].NumberOfTxPortGroups != 0x01)
                                    {
                                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                        Response = EDD_STS_ERR_PRM_BLOCK;
                                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.NumberOfTxPortGroups(%d) != 1", 
                                            pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].NumberOfTxPortGroups);
                                        // exit FOR loop "IRFrameDataArray_Count_All"
                                        i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                                    }
                                }

                                if (Response == EDD_STS_OK)
                                {
                                    // store TxPortGroupArray
                                    pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].TxPortGroupArray  = *pRecord++;
                                    // init with invalid
                                    pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].TxPort            = 0;
                                    pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].SndRcvRole        = EDDP_PRM_PDIRFRAMEDATA_ROLE_INVALID;
                                }

                                if (Response == EDD_STS_OK)
                                {
                                    // check padding bytes
                                    pRecord_Tmp = pRecord;
                                    if (*pRecord++ != 0)
                                    {
                                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                        Response = EDD_STS_ERR_PRM_BLOCK;
                                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.Padding is not '0'!");
                                        // exit FOR loop "IRFrameDataArray_Count_All"
                                        i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                                    }
                                    else
                                    {
                                        // check padding bytes
                                        pRecord_Tmp = pRecord;
                                        if (*pRecord++ != 0)
                                        {
                                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                                            Response = EDD_STS_ERR_PRM_BLOCK;
                                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.Padding is not '0'!");
                                            // exit FOR loop "IRFrameDataArray_Count_All"
                                            i = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                                        }
                                    }
                                }

                                if (Response == EDD_STS_OK)
                                {
                                    pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].isDoubleFrameID   = LSA_FALSE;
                                    pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].SubframeBlockIdx  = EDDP_PRM_PDIRSUBFRAMEDATA_PDIRFRAMEDATA_IDX_UNDEFINED;
                                }
                            }
                        }
                    }
                    else
                    {
                        // unknown BlockVersion
                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_PDIRFrameData_BlockVersion - pRecord_Begin);
                        Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRFrameData.BlockVersion does not match! BlockVersionHigh=0x%X, BlockVersionLow=0x%X", 
                            BlockVersionHigh, BlockVersionLow);
                    }
                }
            }
        }
        else
        {
            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_PDIRFrameData_Begin - pRecord_Begin);
            Response = EDD_STS_ERR_PRM_BLOCK;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> Blocktype(0x%X) found, expected 0x%X", 
               val_16 , EDDP_PRM_PDIRFRAMEDATA_BLOCKTYPE);
        }
    }

    // check if number of read bytes (in PDIRFrameData) is same as record length
    if (Response == EDD_STS_OK)
    {
        if ( ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_PDIRFrameData_Begin) != PDIRFrameData_RecordLength )
        {
            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_PDIRFrameData_RecordLength - pRecord_Begin);
            Response = EDD_STS_ERR_PRM_BLOCK;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> number of read PDIRFrameData bytes(%d) != PDIRFrameData_RecordLength(%d)", 
                ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_PDIRFrameData_Begin), PDIRFrameData_RecordLength);
        }
    }

    /* ------------------------------------------------------------------------ */
    /* PDIRData.PDIRBeginEndData                                                */
    /* ------------------------------------------------------------------------ */
    if (Response == EDD_STS_OK)
    {
        pRecord_PDIRBeginEndData_Begin = pRecord;

        // check BlockType of PDIRBeginEndData
        pRecord_Tmp = pRecord;
        EDDP_GET_U16_INCR(pRecord, val_16);
        if (val_16 != EDDP_PRM_PDIRBEGINENDDATA_BLOCKTYPE)
        {
            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
            Response =  EDD_STS_ERR_PRM_BLOCK;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRBeginEndData.BlockType(0x%X) != EDDP_PRM_PDIRBEGINENDDATA_BLOCKTYPE(0x%X)", 
                val_16, EDDP_PRM_PDIRBEGINENDDATA_BLOCKTYPE);
        }

        // get BlockLength of PDIRBeginEndData (this is checked below)
        pRecord_PDIRBeginEndData_RecordLength = pRecord;
        EDDP_GET_U16_INCR(pRecord, val_16);
        PDIRBeginEndData_RecordLength = (val_16 + 4);

        if (Response == EDD_STS_OK)
        {
            // check BlockVersionHigh of PDIRBeginEndData
            pRecord_Tmp = pRecord;
            BlockVersionHigh = *pRecord++;
            if (BlockVersionHigh != EDDP_PRM_PDIRBEGINENDDATA_BLOCKVERSIONHIGH)
            {
                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRBeginEndData.BlockVersionHigh(0x%X) != EDDP_PRM_PDIRBEGINENDDATA_BLOCKVERSIONHIGH(0x%X)", 
                    BlockVersionHigh, EDDP_PRM_PDIRBEGINENDDATA_BLOCKVERSIONHIGH);
            }
            else
            {
                // check BlockVersionLow of PDIRBeginEndData
                pRecord_Tmp = pRecord;
                BlockVersionLow = *pRecord++;
                if (BlockVersionLow != EDDP_PRM_PDIRBEGINENDDATA_BLOCKVERSIONLOW)
                {
                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                    Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRBeginEndData.BlockVersionLow(0x%X) != EDDP_PRM_PDIRBEGINENDDATA_BLOCKVERSIONLOW(0x%X)", 
                        BlockVersionLow, EDDP_PRM_PDIRBEGINENDDATA_BLOCKVERSIONLOW);
                }
                else
                {
                    // check padding bytes
                    pRecord_Tmp = pRecord;
                    if (*pRecord++ != 0)
                    {
                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                        Response = EDD_STS_ERR_PRM_BLOCK;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRBeginEndData.Padding is not '0'!");
                    }
                    else
                    {
                        // check padding bytes
                        pRecord_Tmp = pRecord;
                        if (*pRecord++ != 0)
                        {
                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                            Response = EDD_STS_ERR_PRM_BLOCK;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRBeginEndData.Padding is not '0'!");
                        }
                    }
                }
            }
        }

        if (Response == EDD_STS_OK)
        {
            // store StartOfRedFrameID
            pRecord_StartOfRedFrameID = pRecord;
            EDDP_GET_U16_INCR(pRecord, val_16);
            pDDB->PRM.pPDIRData_B->PDIRBeginEndData.StartOfRedFrameID = val_16;
            // is StartOfRedFrameID a Sync-FrameID ?
            if (pDDB->PRM.pPDIRData_B->PDIRBeginEndData.StartOfRedFrameID == EDD_SRT_FRAMEID_PTCP_SYNC_RT_CLASS3)
            {
                // the Sync-FrameID is ignored and is overwritten with IRT-FrameID-Start
                pDDB->PRM.pPDIRData_B->PDIRBeginEndData.StartOfRedFrameID = EDD_SRT_FRAMEID_IRT_START;
            }

            // store EndOfRedFrameID
            pRecord_EndOfRedFrameID = pRecord;
            EDDP_GET_U16_INCR(pRecord, val_16);
            pDDB->PRM.pPDIRData_B->PDIRBeginEndData.EndOfRedFrameID = val_16;

            // check FrameID (Start/End) is in valid range - only for RelativeTimeMode
            if (!(pDDB->PRM.pPDIRData_B->AbsoluteTimeMode))
            {
                if (!(EDDP_IS_IRT_FRAMEID(pDDB->PRM.pPDIRData_B->PDIRBeginEndData.StartOfRedFrameID)))
                {
                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_StartOfRedFrameID - pRecord_Begin);
                    Response = EDD_STS_ERR_PRM_BLOCK;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRBeginEndData.StartOfRedFrameID(0x%X) is out of range [0x%X...0x%X]!", 
                        pDDB->PRM.pPDIRData_B->PDIRBeginEndData.StartOfRedFrameID, 
                        EDD_SRT_FRAMEID_IRT_START, EDD_SRT_FRAMEID_IRT_STOP);
                }
                else
                {
                    if (!(EDDP_IS_IRT_FRAMEID(pDDB->PRM.pPDIRData_B->PDIRBeginEndData.EndOfRedFrameID)))
                    {
                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_EndOfRedFrameID - pRecord_Begin);
                        Response = EDD_STS_ERR_PRM_BLOCK;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRBeginEndData.EndOfRedFrameID(0x%X) is out of range [0x%X...0x%X]!", 
                            pDDB->PRM.pPDIRData_B->PDIRBeginEndData.EndOfRedFrameID, 
                            EDD_SRT_FRAMEID_IRT_START, EDD_SRT_FRAMEID_IRT_STOP);
                    }
                    else
                    {
                        if (pDDB->PRM.pPDIRData_B->PDIRBeginEndData.StartOfRedFrameID > pDDB->PRM.pPDIRData_B->PDIRBeginEndData.EndOfRedFrameID)
                        {
                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_StartOfRedFrameID - pRecord_Begin);
                            Response = EDD_STS_ERR_PRM_BLOCK;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRBeginEndData.StartOfRedFrameID(0x%X) >= EndOfRedFrameID(0x%X)!", 
                                pDDB->PRM.pPDIRData_B->PDIRBeginEndData.StartOfRedFrameID, pDDB->PRM.pPDIRData_B->PDIRBeginEndData.EndOfRedFrameID);
                        }
                    }
                }
            }
        }

        if (Response == EDD_STS_OK)
        {
            // store NumberOfPorts
            pRecord_Tmp = pRecord;
            EDDP_GET_U32_INCR(pRecord, val_32);
            pDDB->PRM.pPDIRData_B->PDIRBeginEndData.NumberOfPorts = val_32;
            // check NumberOfPorts
            if (pDDB->PRM.pPDIRData_B->PDIRBeginEndData.NumberOfPorts > pDDB->SWI.PortCnt)
            {
                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRBeginEndData.NumberOfPorts(%d) > SWI.PortCnt(%d)!", 
                    pDDB->PRM.pPDIRData_B->PDIRBeginEndData.NumberOfPorts, pDDB->SWI.PortCnt);
            }

            if (Response == EDD_STS_OK)
            {
                // store BeginEndData
                for ( i=0; i < pDDB->PRM.pPDIRData_B->PDIRBeginEndData.NumberOfPorts; i++ )
                {
                    pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].isRedPort = LSA_FALSE;

                    // store NumberOfAssignments
                    pRecord_Tmp = pRecord;
                    EDDP_GET_U32_INCR(pRecord, val_32);
                    pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].NumberOfAssignments = val_32;
                    // check NumberOfAssignments
                    if ( !(EDDP_PRM_IS_NUMBEROFASSIGNMENTS_IN_VALID_RANGE(pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].NumberOfAssignments)) )
                    {
                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                        Response = EDD_STS_ERR_PRM_BLOCK;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_PRM_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRBeginEndData.NumberOfAssignments(%d) is out of range [%d...%d]! Port(%d)", 
                            pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].NumberOfAssignments, 
                            EDDP_PRM_PDIRBEGINENDDATA_NUMBEROFASSIGNMENTS_MIN, 
                            EDDP_PRM_PDIRBEGINENDDATA_NUMBEROFASSIGNMENTS_MAX, 
                            i);
                        // exit FOR loop "NumberOfPorts"
                        i = pDDB->PRM.pPDIRData_B->PDIRBeginEndData.NumberOfPorts;
                    }

                    if (Response == EDD_STS_OK)
                    {
                        for ( j=0; j < pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].NumberOfAssignments; j++ )
                        {
                            // store TxRedOrangePeriodBegin
                            pRecord_TxRedOrangePeriodBegin = pRecord;
                            EDDP_GET_U32_INCR(pRecord, val_32);
                            pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxRedOrangePeriodBegin = val_32;
                            // store TxOrangePeriodBegin
                            pRecord_TxOrangePeriodBegin = pRecord;
                            EDDP_GET_U32_INCR(pRecord, val_32);
                            pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxOrangePeriodBegin = val_32;
                            // store TxGreenPeriodBegin
                            pRecord_TxGreenPeriodBegin = pRecord;
                            EDDP_GET_U32_INCR(pRecord, val_32);
                            pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxGreenPeriodBegin = val_32;

                            // store RxRedOrangePeriodBegin
                            pRecord_RxRedOrangePeriodBegin = pRecord;
                            EDDP_GET_U32_INCR(pRecord, val_32);
                            pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxRedOrangePeriodBegin = val_32;
                            // store RxOrangePeriodBegin
                            pRecord_RxOrangePeriodBegin = pRecord;
                            EDDP_GET_U32_INCR(pRecord, val_32);
                            pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxOrangePeriodBegin = val_32;
                            // store RxGreenPeriodBegin
                            pRecord_RxGreenPeriodBegin = pRecord;
                            EDDP_GET_U32_INCR(pRecord, val_32);
                            pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxGreenPeriodBegin = val_32;

                            // check PeriodBegin
                            if (   (!(EDDP_PRM_IS_PERIODBEGIN_IN_VALID_RANGE(pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxRedOrangePeriodBegin))) 
                                || (!(EDDP_PRM_IS_PERIODBEGIN_IN_VALID_RANGE(pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxOrangePeriodBegin))) 
                                || (!(EDDP_PRM_IS_PERIODBEGIN_IN_VALID_RANGE(pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxGreenPeriodBegin))) 
                                || (!(EDDP_PRM_IS_PERIODBEGIN_IN_VALID_RANGE(pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxRedOrangePeriodBegin))) 
                                || (!(EDDP_PRM_IS_PERIODBEGIN_IN_VALID_RANGE(pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxOrangePeriodBegin))) 
                                || (!(EDDP_PRM_IS_PERIODBEGIN_IN_VALID_RANGE(pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxGreenPeriodBegin))) 
                               )
                            {
                                if ( !(EDDP_PRM_IS_PERIODBEGIN_IN_VALID_RANGE(pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxRedOrangePeriodBegin)) )
                                {
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_TxRedOrangePeriodBegin - pRecord_Begin);
                                }
                                if ( !(EDDP_PRM_IS_PERIODBEGIN_IN_VALID_RANGE(pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxOrangePeriodBegin)) )
                                {
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_TxOrangePeriodBegin - pRecord_Begin);
                                }
                                if ( !(EDDP_PRM_IS_PERIODBEGIN_IN_VALID_RANGE(pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxGreenPeriodBegin)) )
                                {
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_TxGreenPeriodBegin - pRecord_Begin);
                                }
                                if ( !(EDDP_PRM_IS_PERIODBEGIN_IN_VALID_RANGE(pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxRedOrangePeriodBegin)) )
                                {
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_RxRedOrangePeriodBegin - pRecord_Begin);
                                }
                                if ( !(EDDP_PRM_IS_PERIODBEGIN_IN_VALID_RANGE(pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxOrangePeriodBegin)) )
                                {
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_RxOrangePeriodBegin - pRecord_Begin);
                                }
                                if ( !(EDDP_PRM_IS_PERIODBEGIN_IN_VALID_RANGE(pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxGreenPeriodBegin)) )
                                {
                                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_RxGreenPeriodBegin - pRecord_Begin);
                                }
                                Response = EDD_STS_ERR_PRM_BLOCK;
                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRBeginEndData: one of PeriodBegin parameters is out of range [%d...%d]! Port(%d)!", 
                                    EDDP_PRM_PDIRBEGINENDDATA_PERIODBEGIN_MIN, EDDP_PRM_PDIRBEGINENDDATA_PERIODBEGIN_MAX, i+1);
                                // do not delete the empty spaces in this trace text
                                EDDP_PRM_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--]    PeriodBegin-Param: TxRedOrangePeriodBegin(%d), TxOrangePeriodBegin(%d), TxGreenPeriodBegin(%d), RxRedOrangePeriodBegin(%d), RxOrangePeriodBegin(%d), RxGreenPeriodBegin(%d)", 
                                    pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxRedOrangePeriodBegin, 
                                    pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxOrangePeriodBegin, 
                                    pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxGreenPeriodBegin, 
                                    pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxRedOrangePeriodBegin, 
                                    pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxOrangePeriodBegin, 
                                    pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxGreenPeriodBegin);
                                // exit FOR loop "NumberOfAssignments"
                                j = pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].NumberOfAssignments;
                                // exit FOR loop "NumberOfPorts"
                                i = pDDB->PRM.pPDIRData_B->PDIRBeginEndData.NumberOfPorts;
                            }

                            if (Response == EDD_STS_OK)
                            {
                                // check ORANGE period Rx/Tx is not supported
                                if (EDDP_PRMPDIRDataCheckOrangePhaseAvailable (
                                            pDDB,
                                            pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxRedOrangePeriodBegin, 
                                            pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxOrangePeriodBegin, 
                                            pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxGreenPeriodBegin, 
                                            pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxRedOrangePeriodBegin, 
                                            pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxOrangePeriodBegin, 
                                            pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxGreenPeriodBegin)
                                   )
                                {
                                    if ( !(EDDP_PRM_IS_PERIODBEGIN_IN_VALID_RANGE(pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxRedOrangePeriodBegin)) )
                                    {
                                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_TxRedOrangePeriodBegin - pRecord_Begin);
                                    }
                                    if ( !(EDDP_PRM_IS_PERIODBEGIN_IN_VALID_RANGE(pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxOrangePeriodBegin)) )
                                    {
                                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_TxOrangePeriodBegin - pRecord_Begin);
                                    }
                                    if ( !(EDDP_PRM_IS_PERIODBEGIN_IN_VALID_RANGE(pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxGreenPeriodBegin)) )
                                    {
                                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_TxGreenPeriodBegin - pRecord_Begin);
                                    }
                                    if ( !(EDDP_PRM_IS_PERIODBEGIN_IN_VALID_RANGE(pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxRedOrangePeriodBegin)) )
                                    {
                                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_RxRedOrangePeriodBegin - pRecord_Begin);
                                    }
                                    if ( !(EDDP_PRM_IS_PERIODBEGIN_IN_VALID_RANGE(pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxOrangePeriodBegin)) )
                                    {
                                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_RxOrangePeriodBegin - pRecord_Begin);
                                    }
                                    if ( !(EDDP_PRM_IS_PERIODBEGIN_IN_VALID_RANGE(pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxGreenPeriodBegin)) )
                                    {
                                        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_RxGreenPeriodBegin - pRecord_Begin);
                                    }
                                    Response = EDD_STS_ERR_PRM_BLOCK;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRBeginEndData: ORANGE period Rx/Tx is not supported! Port(%d)!", 
                                        i+1);
                                    // exit FOR loop "NumberOfAssignments"
                                    j = pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].NumberOfAssignments;
                                    // exit FOR loop "NumberOfPorts"
                                    i = pDDB->PRM.pPDIRData_B->PDIRBeginEndData.NumberOfPorts;
                                }
                            }

                            // No check of MaxLineRxDelay will be done here (TFS3986111). 
                            // MaxLineRxDelay has to valid in relative mode only, and only if the port has a forwarding consumer. In relative mode,
                            // forwarders do not appear in the PDIRData, so that EDDP cannot check wether a check has to be done at all.

                            // init flag "isRedPort"
                            if (Response == EDD_STS_OK)
                            {
                                // is there red Rx/Tx available on this port?
                                if (EDDP_PRMPDIRDataCheckRedPhaseAvailable ( 
                                            pDDB,
                                            pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxRedOrangePeriodBegin, 
                                            pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxOrangePeriodBegin, 
                                            pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].TxGreenPeriodBegin, 
                                            pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxRedOrangePeriodBegin, 
                                            pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxOrangePeriodBegin, 
                                            pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].Assignments[j].RxGreenPeriodBegin)
                                   )
                                {
                                    // at least one port is red
                                    isRedPresent = LSA_TRUE;
                                    // this port has red phase
                                    pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].isRedPort = LSA_TRUE;
                                }
                            }
                        }
                    }

                    if (Response == EDD_STS_OK)
                    {
                        // store NumberOfPhases
                        pRecord_Tmp = pRecord;
                        EDDP_GET_U32_INCR(pRecord, val_32);
                        pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].NumberOfPhases = val_32;
                        // check NumberOfPhases
                        if ( !(EDDP_PRM_IS_NUMBEROFPHASES_IN_VALID_RANGE(pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].NumberOfPhases)) )
                        {
                            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                            Response = EDD_STS_ERR_PRM_BLOCK;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRBeginEndData.NumberOfPhases(%d) is out of range [%d...%d]! Port(%d)", 
                                pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].NumberOfPhases, 
                                EDDP_PRM_PDIRBEGINENDDATA_NUMBEROFPHASES_MIN, 
                                EDDP_PRM_PDIRBEGINENDDATA_NUMBEROFPHASES_MAX, 
                                i+1);
                            // exit FOR loop "NumberOfPorts"
                            i = pDDB->PRM.pPDIRData_B->PDIRBeginEndData.NumberOfPorts;
                        }

                        if (Response == EDD_STS_OK)
                        {
                            for ( j=0; j < pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].NumberOfPhases; j++ )
                            {
                                // store TxPhaseAssignment
                                EDDP_GET_U16_INCR(pRecord, val_16);
                                pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].PhaseAssignmentArray[j].TxPhaseAssignment = val_16;
                                // store RxPhaseAssignment
                                EDDP_GET_U16_INCR(pRecord, val_16);
                                pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[i].PhaseAssignmentArray[j].RxPhaseAssignment = val_16;            
                            }
                        }
                    }
                }
            }

            // check whether PDIRBeginEndData contains red phase
            if ( !(isRedPresent) && (EDD_STS_OK == Response) )
            {
                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_PDIRBeginEndData_Begin - pRecord_Begin);
                Response = EDD_STS_ERR_PRM_BLOCK;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> PDIRBeginEndData: no red phase is present");
            }
        }
    }

    // check if number of read bytes (in PDIRBeginEndData) is same as record length
    if (Response == EDD_STS_OK)
    {
        if ( ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_PDIRBeginEndData_Begin) != PDIRBeginEndData_RecordLength )
        {
            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_PDIRBeginEndData_RecordLength - pRecord_Begin);
            Response = EDD_STS_ERR_PRM_BLOCK;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> number of read PDIRBeginEndData bytes(%d) != PDIRBeginEndData_RecordLength(%d)", 
                ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_PDIRBeginEndData_Begin), PDIRBeginEndData_RecordLength);
        }
    }

    // check if number of read bytes (in PDIRData) is same as record length
    if (Response == EDD_STS_OK)
    {
        if ( ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin) != PDIRData_RecordLength )
        {
            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_PDIRData_RecordLength - pRecord_Begin);
            Response = EDD_STS_ERR_PRM_BLOCK;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataRecord_Write(): ERROR -> number of read PDIRData bytes(%d) != PDIRData_RecordLength(%d)", 
                ((LSA_UINT32)pRecord - (LSA_UINT32)pRecord_Begin), PDIRData_RecordLength);
        }
        else
        {
            // store record in record set B
            EDDP_MEMCOPY_COMMON(pDDB->PRM.pRecordsetB->pPDIRData, pRecord_Begin, PDIRData_RecordLength);
            pDDB->PRM.pRecordsetB->PDIRData_Length    = PDIRData_RecordLength;
            pDDB->PRM.pRecordsetB->PDIRData_IsPresent = LSA_TRUE;
        }
    }

    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMPDIRDataRecord_Write(): Response=0x%X", Response);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_PRMPDIRApplicationDataRecord_Write     +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDD_UPPER_MEM_U8_PTR_TYPE   pRecord         +*/
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
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function parses the PDIRApplicationData record       +*/
/*+               (pRecord) and stores the extracted data into DDB.         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRApplicationDataRecord_Write(
   	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
   	EDD_UPPER_MEM_U8_PTR_TYPE   pRecord)
{
    EDD_RSP                         Response;
    LSA_UINT16                      val_16;
    LSA_UINT32                      record_length;
    LSA_UINT8                       BlockVersionHigh, BlockVersionLow;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_Begin;
    EDD_UPPER_MEM_U8_PTR_TYPE       pRecord_Tmp;
    EDDP_PRM_DETAIL_ERR_PTR_TYPE    pDetailErr;

    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_PRMPDIRApplicationDataRecord_Write()");

    Response        = EDD_STS_OK;
    pDetailErr      = &pDDB->PrmDetailErr;
    pRecord_Begin   = pRecord;
    record_length   = 0;

    // check BlockType
    pRecord_Tmp = pRecord;
    EDDP_GET_U16_INCR(pRecord, val_16);
    if (val_16 != EDDP_PRM_PDIRAPPLICATIONDATA_BLOCK_TYPE)
    {
        EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
        Response = EDD_STS_ERR_PRM_BLOCK;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRApplicationDataRecord_Write(): ERROR -> BlockType(0x%X) != EDDP_PRM_PDIRAPPLICATIONDATA_BLOCK_TYPE(0x%X)!", 
            val_16, EDDP_PRM_PDIRAPPLICATIONDATA_BLOCK_TYPE);
    }
    else
    {
        // check BlockLength
        pRecord_Tmp = pRecord;
        EDDP_GET_U16_INCR(pRecord, val_16);
        record_length = (val_16 + 4);
        if (record_length > pDDB->PRM.pRecordsetB->PDIRApplicationData_Length)
        {
            EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
            Response = EDD_STS_ERR_PRM_BLOCK;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRApplicationDataRecord_Write(): ERROR -> Record length(%d) does not match with length in RQB->record_data_length(%d)!", 
                record_length, pDDB->PRM.pRecordsetB->PDIRApplicationData_Length);
        }
        else
        {
            // check BlockVersionHigh
            pRecord_Tmp = pRecord;
            BlockVersionHigh = *pRecord++;
            if (BlockVersionHigh != EDDP_PRM_PDIRAPPLICATIONDATA_BLOCKVERSIONHIGH)
            {
                EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRApplicationDataRecord_Write(): ERROR -> BlockVersionHigh(0x%X) does not match (PDIRApplicationData)!", 
                    BlockVersionHigh);
            }
            else
            {
                // check BlockVersionLow
                pRecord_Tmp = pRecord;
                BlockVersionLow = *pRecord++;
                if (BlockVersionLow != EDDP_PRM_PDIRAPPLICATIONDATA_BLOCKVERSIONLOW)
                {
                    EDDP_PRM_SET_ERR_OFFSET(pDetailErr, pRecord_Tmp - pRecord_Begin);
                    Response = EDD_STS_ERR_PRM_VERSION_CONFLICT;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRApplicationDataRecord_Write(): ERROR -> BlockVersionLow(0x%X) does not match (PDIRApplicationData)!", 
                        BlockVersionLow);
                }
            }
        }
    }

    if (Response == EDD_STS_OK)
    {
        // store ConsumerCnt
        EDDP_GET_U16_INCR(pRecord, pDDB->PRM.pPDIRApplicationData_B->ConsumerCnt);
        // store ConsumerIODataLength
        EDDP_GET_U16_INCR(pRecord, pDDB->PRM.pPDIRApplicationData_B->ConsumerIODataLength);

        // store record in record set B
        EDDP_MEMCOPY_COMMON(pDDB->PRM.pRecordsetB->pPDIRApplicationData, pRecord_Begin, record_length);
        pDDB->PRM.pRecordsetB->PDIRApplicationData_Length       = record_length;
        pDDB->PRM.pRecordsetB->PDIRApplicationData_IsPresent    = LSA_TRUE;
    }

    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMPDIRApplicationDataRecord_Write(): Response=0x%X", Response);
    
    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRDataCheckMaxBridgeDelay                     +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT32                      MaxBridgeDelay          +*/
/*+                                                                                     +*/
/*+  Result                :    EDD_RSP                                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  MaxBridgeDelay : MaxBridgeDelay that must be checked                               +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks the given MaxBridgeDelay against                 +*/
/*+               minimum MaxBridgeDelay dependent of:                                  +*/
/*+                 - Standard or FastForwarding                                        +*/
/*+                 - long or short Preamble                                            +*/
/*+                 - AbsoluteTime- or RelativeTime-Mode                                +*/
/*+                                                                                     +*/
/*+               PDIRData record must be present for checking.                         +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataCheckMaxBridgeDelay(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_UINT32                      MaxBridgeDelay)
{
    EDD_RSP                         Response;
    EDDP_PRM_DETAIL_ERR_PTR_TYPE    pDetailErr;
    LSA_UINT32                      MaxBridgeDelay_Min;
    LSA_UINT32                      PortID, RedPortCnt;
    LSA_BOOL                        FastForwarding;
    LSA_BOOL                        LongPreamble;

    Response        = EDD_STS_OK;
    pDetailErr      = &pDDB->PrmDetailErr;
    FastForwarding  = LSA_FALSE;
    LongPreamble    = LSA_FALSE;
    RedPortCnt      = 0;

    // check MaxBridgeDelay only if PDIRData record is present
    if (EDDP_PRMPDIRDataIsPDIRDataPresent(pDDB, LSA_FALSE))
    {
        /* ---------------------------------------- */
        /* Standard or FastForwarding ?             */
        /* ---------------------------------------- */
        // is "FrameDataProperties" present in PDIRData ?
        if ( EDDP_PRMPDIRDataIsFrameDataPropertiesPresent(pDDB, LSA_FALSE) )
        {
            if (EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_FFW == 
                    EDDP_PRMPDIRDataGetFrameDataProperties (pDDB, EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_SHIFT, LSA_FALSE)
               )
               FastForwarding = LSA_TRUE;
        }

        /* ---------------------------------------------------------------------------------------- */
        /* long or short Preamble ?                                                                 */
        /*  - short Preamble: ports with RED phase and PortDataAdjust.PreambleLength = 1            */
        /*  - else long Preamble                                                                    */
        /*  - no RED ports in PDIRData: long Preamble                                               */
        /* ---------------------------------------------------------------------------------------- */
        for ( PortID=1; PortID <= pDDB->SWI.PortCnt; PortID++ )
        {
            // following checks only if still short Preamble is set
            if ( !(LongPreamble) )
            {
                if (pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[PortID-1].isRedPort)
                {
                    RedPortCnt++;

                    // is PreambleLength present ?
                    if (pDDB->PRM.PortDataAdjust.Parameters[PortID].PreambleLength_IsPresent)
                    {
                        // PDPortDataAdjust record with "PreambleLength" for this port is present
                        if (pDDB->PRM.PortDataAdjust.Parameters[PortID].PreambleLength == 0)
                            LongPreamble = LSA_TRUE;    // long Preamble
                        else
                            LongPreamble = LSA_FALSE;   // short Preamble
                    }
                    else
                    {
                        // PDPortDataAdjust record with "PreambleLength" for this port is not present
                        LongPreamble = LSA_TRUE;    // long Preamble
                    }
                }
            }
        }

        if (RedPortCnt == 0)
        {
            // no RED ports in PDIRData! This should have been stated in PrmWrite!
            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataCheckMaxBridgeDelay(): FATAL -> no RED ports in PDIRData!");
            EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
        }

        /* ---------------------------------------- */
        /* set MaxBridgeDelay_Min dependent of      */
        /* AbsoluteTime- or RelativeTime-Mode       */
        /* ---------------------------------------- */
        if (pDDB->PRM.pPDIRData_B->AbsoluteTimeMode)
        {
            /* -------------------- */
            /* AbsoluteTime-Mode    */
            /* -------------------- */
            if (FastForwarding)
            {
                // FastFortwarding
                if (LongPreamble)
                    MaxBridgeDelay_Min = pDDB->PRM.CfgParam.Min_MaxBridgeDelay.AbsoluteFFWLong;
                else
                    MaxBridgeDelay_Min = pDDB->PRM.CfgParam.Min_MaxBridgeDelay.AbsoluteFFWShort;
            }
            else
            {
                // Standard
                if (LongPreamble)
                    MaxBridgeDelay_Min = pDDB->PRM.CfgParam.Min_MaxBridgeDelay.AbsoluteStandardLong;
                else
                    MaxBridgeDelay_Min = pDDB->PRM.CfgParam.Min_MaxBridgeDelay.AbsoluteStandardShort;
            }
        }
        else
        {
            /* -------------------- */
            /* RelativeTime-Mode    */
            /* -------------------- */
            if (FastForwarding)
            {
                // FastFortwarding
                if (LongPreamble)
                    MaxBridgeDelay_Min = pDDB->PRM.CfgParam.Min_MaxBridgeDelay.RelativeFFWLong;
                else
                    MaxBridgeDelay_Min = pDDB->PRM.CfgParam.Min_MaxBridgeDelay.RelativeFFWShort;
            }
            else
            {
                // Standard
                if (LongPreamble)
                    MaxBridgeDelay_Min = pDDB->PRM.CfgParam.Min_MaxBridgeDelay.RelativeStandardLong;
                else
                    MaxBridgeDelay_Min = pDDB->PRM.CfgParam.Min_MaxBridgeDelay.RelativeStandardShort;
            }
        }

        /* ---------------------------------------- */
        /* now check MaxBridgeDelay                 */
        /* ---------------------------------------- */
        if (MaxBridgeDelay < MaxBridgeDelay_Min)
        {
            EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDIRDATA_RECORD_INDEX, pDetailErr->OffsetMaxBridgeDelay, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
            Response = EDD_STS_ERR_PRM_CONSISTENCY;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataCheckMaxBridgeDelay(): ERROR -> given MaxBridgeDelay(%d) < MaxBridgeDelay_Min(%d), LongPreamble=0x%X, FastForwarding=0x%X, AbsoluteTimeMode=0x%X", 
                MaxBridgeDelay, MaxBridgeDelay_Min, LongPreamble, FastForwarding, pDDB->PRM.pPDIRData_B->AbsoluteTimeMode);
        }
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRDataCheckFrameDetails                       +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      PDIRFrameDataIdx        +*/
/*+                                                                                     +*/
/*+  Result                :    EDD_RSP                                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  PDIRFrameDataIdx   : index in PDIRData.PDIRFrameData[x]                            +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks the PRM.PDIRData.FrameDetails:                   +*/
/*+                 - FrameDetails.SyncFrame                                            +*/
/*+                     EDDP supports only 0x0 (No SyncFrame)                           +*/
/*+                 - FrameDetails.MeaningFrameSendOffset                               +*/
/*+                     EDDP supports only 0x0 (FrameSendOffset is a point of time      +*/
/*+                     for receiving or transmitting of a frame)                       +*/
/*+                 - FrameDetails.MediaRedundancyWatchDog                              +*/
/*+                     If enable, check whether the FrameID is in redundant range.     +*/
/*+                                                                                     +*/
/*+               The FrameID and FrameDetails must be already copied to PDIRData_B.    +*/
/*+                                                                                     +*/
/*+               This check is only for IRT-FrameIDs, not for Sync-FrameIDs!           +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataCheckFrameDetails(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_UINT16                      PDIRFrameDataIdx)
{
    EDD_RSP                             Response;
    LSA_UINT8                           ValueTemp_8;
    EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE     pPDIRFrameData;

    EDDP_ASSERT_FALSE(PDIRFrameDataIdx >= pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All);

    Response                                            = EDD_STS_OK;
    ValueTemp_8                                         = 0;
    pPDIRFrameData                                      = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[PDIRFrameDataIdx];
    // delete MrtDiagPendingList
    pPDIRFrameData->MrtDiag.PendingGroupLink.Prev       = EDDP_PRM_CHAIN_IDX_END;
    pPDIRFrameData->MrtDiag.PendingGroupLink.Next       = EDDP_PRM_CHAIN_IDX_END;
    pPDIRFrameData->MrtDiag.Flags                       = 0;
    // delete LateErrDiagPendingGroup
    pPDIRFrameData->LateErrDiag.PendingGroupLink.Prev   = EDDP_PRM_CHAIN_IDX_END;
    pPDIRFrameData->LateErrDiag.PendingGroupLink.Next   = EDDP_PRM_CHAIN_IDX_END;
    pPDIRFrameData->LateErrDiag.Flags                   = 0;


    // only for IRT FrameID range
    if (EDDP_IS_IRT_FRAMEID(pPDIRFrameData->FrameID))
    {
        // FrameDetails.SyncFrame
        ValueTemp_8 = EDDP_GET_BIT_VALUE( pPDIRFrameData->FrameDetails,
                                          EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_SYNCFRAME_MASK, 
                                          EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_SYNCFRAME_SHIFT);
        if (ValueTemp_8 != EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_SYNCFRAME_NO_SYNC)
        {
            Response = EDD_STS_ERR_PRM_CONSISTENCY;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataCheckFrameDetails(): ERROR -> PDIRFrameData.FrameDetails.SyncFrame(0x%X) is not supported! FrameID=0x%X, FrameDetails=0x%X", 
                ValueTemp_8, pPDIRFrameData->FrameID, pPDIRFrameData->FrameDetails);
        }
        else
        {
            // FrameDetails.MeaningFrameSendOffset
            ValueTemp_8 = EDDP_GET_BIT_VALUE( pPDIRFrameData->FrameDetails,
                                              EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_MEANINGFRAMESENDOFFSET_MASK, 
                                              EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_MEANINGFRAMESENDOFFSET_SHIFT);
            if (ValueTemp_8 != EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_MEANINGFRAMESENDOFFSET_POINT_OF_TIME)
            {
                Response = EDD_STS_ERR_PRM_CONSISTENCY;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataCheckFrameDetails(): ERROR -> PDIRFrameData.FrameDetails.MeaningFrameSendOffset(0x%X) is not supported! FrameID=0x%X, FrameDetails=0x%X", 
                    ValueTemp_8, pPDIRFrameData->FrameID, pPDIRFrameData->FrameDetails);
            }
            else
            {
                // FrameDetails.MediaRedundancyWatchDog
                ValueTemp_8 = EDDP_GET_BIT_VALUE( pPDIRFrameData->FrameDetails,
                                                  EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_MEDIAREDUNDANCYWATCHDOG_MASK, 
                                                  EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_MEDIAREDUNDANCYWATCHDOG_SHIFT);
                if (   (ValueTemp_8 != EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_MEDIAREDUNDANCYWATCHDOG_MRT_OFF)
                    && (ValueTemp_8 != EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_MEDIAREDUNDANCYWATCHDOG_MRT_ON)
                   )
                {
                    Response = EDD_STS_ERR_PRM_CONSISTENCY;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataCheckFrameDetails(): ERROR -> PDIRFrameData.FrameDetails.MediaRedundancyWatchDog(0x%X) is not valid! FrameID=0x%X, FrameDetails=0x%X", 
                        ValueTemp_8, pPDIRFrameData->FrameID, pPDIRFrameData->FrameDetails);
                }

                // is "MediaRedundancyWatchDog" active ?
                if ((Response == EDD_STS_OK) && (ValueTemp_8 == EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_MEDIAREDUNDANCYWATCHDOG_MRT_ON))
                {
                    switch (EDDP_HALGetPnipRevision(pDDB))
                    {
                        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
                        case EDDP_HAL_HW_PNIP_REV1:
                        {
                            // not supported with PN-IP Rev1
                            Response = EDD_STS_ERR_PRM_CONSISTENCY;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataCheckFrameDetails(): ERROR -> PDIRFrameData.FrameDetails.MediaRedundancyWatchDog(0x%X) is not supported with PN-IP Rev1! FrameID=0x%X, FrameDetails=0x%X", 
                                ValueTemp_8, pPDIRFrameData->FrameID, pPDIRFrameData->FrameDetails);
                        }
                        break;
                        #endif

                        #if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
                        case EDDP_HAL_HW_PNIP_REV2:
                        case EDDP_HAL_HW_PNIP_REV3:
                        {
                            // only for redundant FrameID range allowed
                            if ( !(EDDP_IS_IRT_REDUNDANT_FRAMEID(pPDIRFrameData->FrameID)) )
                            {
                                Response = EDD_STS_ERR_PRM_CONSISTENCY;
                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataCheckFrameDetails(): ERROR -> PDIRFrameData.FrameDetails.MediaRedundancyWatchDog(0x%X) is only for redundant FrameID allowed! FrameID=0x%X, FrameDetails=0x%X", 
                                    ValueTemp_8, pPDIRFrameData->FrameID, pPDIRFrameData->FrameDetails);
                            }
                            else
                            {
                                // for Provider not allowed
                                if (pPDIRFrameData->RxPort == 0)
                                {
                                    Response = EDD_STS_ERR_PRM_CONSISTENCY;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataCheckFrameDetails(): ERROR -> PDIRFrameData.FrameDetails.MediaRedundancyWatchDog(0x%X) is not allowed for provider! FrameID=0x%X, FrameDetails=0x%X", 
                                        ValueTemp_8, pPDIRFrameData->FrameID, pPDIRFrameData->FrameDetails);
                                }
                            }

                            // if all checks are ok activate the MRT diagnosis in PRM
                            if (Response == EDD_STS_OK)
                            {
					            pPDIRFrameData->MrtDiag.Flags |= EDDP_PRM_PDIRFRAMEDATA_MRT_FLAGS_MRPD_WATCHDOG_ACTIVE;
                            }
                        }
                        break;
                        #endif
                        
                        default:
                        {
                            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataCheckFrameDetails(): FATAL -> Version(%d) of PNIP is unknown", 
                                EDDP_HALGetPnipRevision(pDDB));
                            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                        }
                        break;
                    }
                }
            }
        }
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRDataCheckRedPhaseAvailable                  +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT32                      TxRedOrangePeriodBegin  +*/
/*+                             LSA_UINT32                      TxOrangePeriodBegin     +*/
/*+                             LSA_UINT32                      TxGreenPeriodBegin      +*/
/*+                             LSA_UINT32                      RxRedOrangePeriodBegin  +*/
/*+                             LSA_UINT32                      RxOrangePeriodBegin     +*/
/*+                             LSA_UINT32                      RxGreenPeriodBegin      +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_BOOL                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  PeriodBegin/End    : parameters for begin/end of phases                            +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks whether the RED phase is available               +*/
/*+               in given PeriodBegin/End parameters.                                  +*/
/*+                                                                                     +*/
/*+               One of these combinations is valid if red is defined:                 +*/
/*+                 Tx RED and ORANGE period is defined     : TxRedOrangePeriodBegin < TxOrangePeriodBegin < TxGreenPeriodBegin     +*/
/*+                 Tx only a RED period is defined         : TxRedOrangePeriodBegin < TxOrangePeriodBegin = TxGreenPeriodBegin     +*/
/*+                 Rx RED and ORANGE period is defined     : RxRedOrangePeriodBegin < RxOrangePeriodBegin < RxGreenPeriodBegin     +*/
/*+                 Rx only a RED period is defined         : RxRedOrangePeriodBegin < RxOrangePeriodBegin = RxGreenPeriodBegin     +*/
/*+                                                                                     +*/
/*+               Return LSA_TRUE  : RED phase is available in Rx or Tx                 +*/
/*+                      LSA_FALSE : RED phase is not available                         +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataCheckRedPhaseAvailable(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_UINT32                      TxRedOrangePeriodBegin,
	LSA_UINT32                      TxOrangePeriodBegin,
    LSA_UINT32                      TxGreenPeriodBegin,
    LSA_UINT32                      RxRedOrangePeriodBegin,
    LSA_UINT32                      RxOrangePeriodBegin,
    LSA_UINT32                      RxGreenPeriodBegin)
{
    LSA_BOOL        State = LSA_FALSE;

    LSA_UNUSED_ARG(pDDB);

            /* TxRedOrangePeriodBegin < TxOrangePeriodBegin < TxGreenPeriodBegin */
    if (   ((TxRedOrangePeriodBegin < TxOrangePeriodBegin) && (TxOrangePeriodBegin < TxGreenPeriodBegin))
            /* TxRedOrangePeriodBegin < TxOrangePeriodBegin = TxGreenPeriodBegin */
        || ((TxRedOrangePeriodBegin < TxOrangePeriodBegin) && (TxOrangePeriodBegin == TxGreenPeriodBegin))
            /* RxRedOrangePeriodBegin < RxOrangePeriodBegin < RxGreenPeriodBegin */
        || ((RxRedOrangePeriodBegin < RxOrangePeriodBegin) && (RxOrangePeriodBegin < RxGreenPeriodBegin))
            /* RxRedOrangePeriodBegin < RxOrangePeriodBegin = RxGreenPeriodBegin */
        || ((RxRedOrangePeriodBegin < RxOrangePeriodBegin) && (RxOrangePeriodBegin == RxGreenPeriodBegin))
       )
    {
        State = LSA_TRUE;

        EDDP_PRM_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] EDDP_PRMPDIRDataCheckRedPhaseAvailable(): Yes - TxRedOrangePeriodBegin(%d), TxOrangePeriodBegin(%d), TxGreenPeriodBegin(%d), RxRedOrangePeriodBegin(%d), RxOrangePeriodBegin(%d), RxGreenPeriodBegin(%d)", 
            TxRedOrangePeriodBegin, TxOrangePeriodBegin, TxGreenPeriodBegin, RxRedOrangePeriodBegin, RxOrangePeriodBegin, RxGreenPeriodBegin);
    }

    return (State);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRDataCheckOrangePhaseAvailable               +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT32                      TxRedOrangePeriodBegin  +*/
/*+                             LSA_UINT32                      TxOrangePeriodBegin     +*/
/*+                             LSA_UINT32                      TxGreenPeriodBegin      +*/
/*+                             LSA_UINT32                      RxRedOrangePeriodBegin  +*/
/*+                             LSA_UINT32                      RxOrangePeriodBegin     +*/
/*+                             LSA_UINT32                      RxGreenPeriodBegin      +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_BOOL                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  PeriodBegin/End    : parameters for begin/end of phases                            +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks whether the ORANGE phase is available            +*/
/*+               in given PeriodBegin/End parameters.                                  +*/
/*+                                                                                     +*/
/*+               One of these combinations is valid if ORANGE is defined:              +*/
/*+                 Tx ORANGE period is defined     : TxOrangePeriodBegin < TxGreenPeriodBegin  +*/
/*+                 Rx ORANGE period is defined     : RxOrangePeriodBegin < RxGreenPeriodBegin  +*/
/*+                                                                                     +*/
/*+               Return LSA_TRUE  : ORANGE phase is available in Rx or Tx              +*/
/*+                      LSA_FALSE : ORANGE phase is not available                      +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataCheckOrangePhaseAvailable(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_UINT32                      TxRedOrangePeriodBegin,
	LSA_UINT32                      TxOrangePeriodBegin,
    LSA_UINT32                      TxGreenPeriodBegin,
    LSA_UINT32                      RxRedOrangePeriodBegin,
    LSA_UINT32                      RxOrangePeriodBegin,
    LSA_UINT32                      RxGreenPeriodBegin)
{
    LSA_BOOL        State = LSA_FALSE;

    LSA_UNUSED_ARG(pDDB);
    LSA_UNUSED_ARG(TxRedOrangePeriodBegin);
    LSA_UNUSED_ARG(RxRedOrangePeriodBegin);

    if ((TxOrangePeriodBegin < TxGreenPeriodBegin) || (RxOrangePeriodBegin < RxGreenPeriodBegin))
    {
        State = LSA_TRUE;

        EDDP_PRM_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] EDDP_PRMPDIRDataCheckOrangePhaseAvailable(): Yes - TxRedOrangePeriodBegin(%d), TxOrangePeriodBegin(%d), TxGreenPeriodBegin(%d), RxRedOrangePeriodBegin(%d), RxOrangePeriodBegin(%d), RxGreenPeriodBegin(%d)", 
            TxRedOrangePeriodBegin, TxOrangePeriodBegin, TxGreenPeriodBegin, RxRedOrangePeriodBegin, RxOrangePeriodBegin, RxGreenPeriodBegin);
    }

    return (State);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRDataCheckRedundantFrameID_B                 +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      PDIRFrameDataIdx        +*/
/*+                                                                                     +*/
/*+  Result                :    EDD_RSP                                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  PDIRFrameDataIdx   : index in PDIRData.PDIRFrameData[x]                            +*/
/*+                       of redundant FrameID(x) or (x+1)                              +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks for given index in PDIRFrameData-Array Record_B: +*/
/*+                                                                                     +*/
/*+               FrameID(x) found:                                                     +*/
/*+                 * Only for Provider:                                                +*/
/*+                     - PDIRData-Record must contain the couple FrameID(x+1)          +*/
/*+                                                                                     +*/
/*+               FrameID(x+1) found:                                                   +*/
/*+                 * Only for Provider:                                                +*/
/*+                     - PDIRData-Record must contain the couple FrameID(x)            +*/
/*+                     - at least one FrameID(x) must have the same ReductionRatio and Phase+*/
/*+                     - FrameID(x+1) may have exact one TxPort                        +*/
/*+                 * For Consumer/Forwarder:                                           +*/
/*+                     - FrameID(x+1) may have not more than one TxPort                +*/
/*+                                                                                     +*/
/*+               The FrameID(x) of given index must be in redundant FrameID range.     +*/
/*+               If no -> ERR_PRM_DATA!                                                +*/
/*+                                                                                     +*/
/*+               Return EDD_STS_OK             : all checks are successful             +*/
/*+                      EDD_STS_ERR_PRM_DATA   : one check ist not successful          +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataCheckRedundantFrameID_B(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_UINT16                      PDIRFrameDataIdx)
{
    EDD_RSP             Response;
    LSA_UINT32          i;
    LSA_UINT16          RedundFrameID;
    LSA_UINT16          RedundReductionRatio;
    LSA_UINT16          RedundPhase;
    LSA_UINT32          TxPortCount;

    EDDP_ASSERT_FALSE(PDIRFrameDataIdx >= pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All);

    Response                = EDD_STS_OK;
    RedundFrameID           = pDDB->PRM.pPDIRData_B->pIRFrameDataArray[PDIRFrameDataIdx].FrameID;
    RedundReductionRatio    = pDDB->PRM.pPDIRData_B->pIRFrameDataArray[PDIRFrameDataIdx].ReductionRatio;
    RedundPhase             = pDDB->PRM.pPDIRData_B->pIRFrameDataArray[PDIRFrameDataIdx].Phase;

    if (EDDP_IS_IRT_REDUNDANT_FRAMEID(RedundFrameID))
    {
        if (RedundFrameID & 0x1)
        {
            /* ---------------------------- */
            /* this is FrameID(x+1)         */
            /* ---------------------------- */
            if (pDDB->PRM.pPDIRData_B->pIRFrameDataArray[PDIRFrameDataIdx].RxPort == 0)
            {
                // following checks are for provider only

                // PDIRData-Record must contain the FrameID(x)
                if ( !(EDDP_PRMPDIRDataIsRedundantFrameIDCoupleAvailable(pDDB, RedundFrameID, LSA_FALSE)) )
                {
                    Response = EDD_STS_ERR_PRM_CONSISTENCY;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataCheckRedundantFrameID_B(): ERROR -> For redundant provider FrameID(0x%X) there is no couple FrameID available in PDIRData!", 
                        RedundFrameID);
                }
                else
                {
                    // at least one FrameID(x) must have the same ReductionRatio and Phase
                    Response = EDD_STS_ERR_PRM_CONSISTENCY;
                    for ( i=0; i < pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All; i++ )
                    {
                        // look for FrameID(x) with the same ReductionRatio and Phase
                        if (   (pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].ReductionRatio == RedundReductionRatio)
                            && (pDDB->PRM.pPDIRData_B->pIRFrameDataArray[i].Phase == RedundPhase)
                           )
                        {
                            Response    = EDD_STS_OK;
                            i           = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;    // exit FOR loop
                        }
                    }

                    if (Response != EDD_STS_OK)
                    {
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataCheckRedundantFrameID_B(): ERROR -> For redundant provider FrameID(0x%X) there is no couple FrameID with the same ReductionRatio and Phase available in PDIRData!", 
                            RedundFrameID);
                    }
                    else
                    {
                        // FrameID(x+1) may have exact one TxPort
                        TxPortCount = EDDP_PRMGetPortCountFromTxPortGroupArray (pDDB, 
                                            pDDB->PRM.pPDIRData_B->pIRFrameDataArray[PDIRFrameDataIdx].TxPortGroupArray);

                        if (TxPortCount != 1)
                        {
                            Response = EDD_STS_ERR_PRM_CONSISTENCY;
                            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                            EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataCheckRedundantFrameID_B(): ERROR -> The redundant provider FrameID(0x%X) has TxPortCount(%d) in PDIRData! TxPortGroupArray=0x%X", 
                                RedundFrameID, TxPortCount, pDDB->PRM.pPDIRData_B->pIRFrameDataArray[PDIRFrameDataIdx].TxPortGroupArray);
                        }
                    }
                }
            }
            else
            {
                // following checks are for consumer/forwarder

                // FrameID(x+1) may have not more than one TxPort
                TxPortCount = EDDP_PRMGetPortCountFromTxPortGroupArray (pDDB, 
                                    pDDB->PRM.pPDIRData_B->pIRFrameDataArray[PDIRFrameDataIdx].TxPortGroupArray);

                if (TxPortCount > 1)
                {
                    Response = EDD_STS_ERR_PRM_CONSISTENCY;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataCheckRedundantFrameID_B(): ERROR -> The redundant consumer/forwarder FrameID(0x%X) has TxPortCount(%d) in PDIRData! TxPortGroupArray=0x%X", 
                        RedundFrameID, TxPortCount, pDDB->PRM.pPDIRData_B->pIRFrameDataArray[PDIRFrameDataIdx].TxPortGroupArray);
                }
            }
        }
        else
        {
            /* ---------------------------- */
            /* this is FrameID(x)           */
            /* ---------------------------- */
            if (pDDB->PRM.pPDIRData_B->pIRFrameDataArray[PDIRFrameDataIdx].RxPort == 0)
            {
                // following checks are for provider only

                // PDIRData-Record must contain the FrameID(x+1)
                if ( !(EDDP_PRMPDIRDataIsRedundantFrameIDCoupleAvailable(pDDB, RedundFrameID, LSA_FALSE)) )
                {
                    Response = EDD_STS_ERR_PRM_CONSISTENCY;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataCheckRedundantFrameID_B(): ERROR -> For redundant provider FrameID(0x%X) there is no couple FrameID available in PDIRData!", 
                        RedundFrameID);
                }
            }
        }
    }
    else
    {
        Response = EDD_STS_ERR_PRM_CONSISTENCY;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataCheckRedundantFrameID_B(): ERROR -> The FrameID(0x%X) is not in redundant range!", 
            RedundFrameID);
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRDataGetDataLen                              +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      FrameID                 +*/
/*+                             LSA_BOOL                        InActiveRecord_A        +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_UINT16                                              +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  FrameID            : IRT FrameID                                                   +*/
/*+  InActiveRecord_A   : LSA_TRUE  - search in active record buffer (A)                +*/
/*+                       LSA_FALSE - search in new record buffer (B)                   +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function gets the parameter "DataLen" from PDIRData-Record.      +*/
/*+                                                                                     +*/
/*+               The given FrameID must be in IRT FrameID range. If no, FATAL!         +*/
/*+                                                                                     +*/
/*+               If the given FrameID is not found in PDIRFrameData, FATAL!            +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataGetDataLen(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_UINT16                      FrameID,
    LSA_BOOL                        InActiveRecord_A)
{
    LSA_UINT16                      DataLen;
    LSA_UINT32                      i;
    LSA_BOOL                        FrameIDFound;
    EDDP_PRM_PDIRDATA_PTR_TYPE      pPDIRData;

    DataLen         = 0;
    FrameIDFound    = LSA_FALSE;
    pPDIRData       = LSA_NULL;

    // only for IRT FrameID range
    if (EDDP_IS_IRT_FRAMEID(FrameID))
    {
        if (InActiveRecord_A)
        {
            // search in active record buffer A - is PDIRData_A present ?
            if (pDDB->PRM.pRecordsetA->PDIRData_IsPresent)
                pPDIRData = pDDB->PRM.pPDIRData_A;
        }
        else
        {
            // search in new record buffer B - is PDIRData_B present ?
            if (pDDB->PRM.pRecordsetB->PDIRData_IsPresent)
                pPDIRData = pDDB->PRM.pPDIRData_B;
        }

        if (LSA_HOST_PTR_ARE_EQUAL(pPDIRData, LSA_NULL))
        {
            // PDIRData is not present!
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataGetDataLen(InActiveRecord_A=0x%X): FATAL -> PDIRData is not present!", 
                InActiveRecord_A);
            EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
        }
        else
        {
            for ( i=0; i < pPDIRData->IRFrameDataArray_Count_All; i++ )
            {
                if (pPDIRData->pIRFrameDataArray[i].FrameID == FrameID)
                {
                    DataLen         = pPDIRData->pIRFrameDataArray[i].DataLength;
                    i               = pPDIRData->IRFrameDataArray_Count_All;  // exit FOR loop
                    FrameIDFound    = LSA_TRUE;
                }
            }
        }

        if ( !(FrameIDFound) )
        {
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataGetDataLen(): FATAL -> The given FrameID(0x%X) is not found in PDIRData-Record!", 
                FrameID);
            EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
        }
    }
    else
    {
        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataGetDataLen(): FATAL -> The given FrameID(0x%X) is not in IRT range!", 
            FrameID);
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }

    return (DataLen);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRDataGetFrameDataProperties                  +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      Properties              +*/
/*+                             LSA_BOOL                        InActiveRecord_A        +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_UINT32                                              +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  Properties         : which FrameDataProperties:                                    +*/
/*+                         - FRAMEDATAPROPERTIES_FORWARDINGMODE_SHIFT                  +*/
/*+                         - FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_SHIFT   +*/
/*+                         - FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_SHIFT               +*/
/*+  InActiveRecord_A   : LSA_TRUE  - search in active record buffer (A)                +*/
/*+                       LSA_FALSE - search in new record buffer (B)                   +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function gets the data of parameter "FrameDataProperties"        +*/
/*+               from PDIRData-Record.                                                 +*/
/*+                                                                                     +*/
/*+               "FrameDataProperties" must be present in PDIRData. If no, FATAL!      +*/
/*+               To check this, use the function                                       +*/
/*+               EDDP_PRMPDIRDataIsFrameDataPropertiesPresent()                        +*/
/*+               before getting FrameDataProperties!                                   +*/
/*+                                                                                     +*/
/*+               The function can get this data of FrameDataProperties:                +*/
/*+                 - FrameDataProperties.ForwardingMode                                +*/
/*+                         -> ForwardingMode   = AbsoluteTime                          +*/
/*+                         -> ForwardingMode   = RelativeTime                          +*/
/*+                 - FrameDataProperties.FastForwardingMulticastMACAdd                 +*/
/*+                         -> FastForwardingMulticastMACAdd    = legacy                +*/
/*+                         -> FastForwardingMulticastMACAdd    = RTC3                  +*/
/*+                         -> FastForwardingMulticastMACAdd    = FastForwarding        +*/
/*+                 - FrameDataProperties.FragmentationMode                             +*/
/*+                         -> FragmentationMode    = Fragmentation is OFF              +*/
/*+                         -> FragmentationMode    = Fragmentation is ON               +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataGetFrameDataProperties(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_UINT16                      Properties,
    LSA_BOOL                        InActiveRecord_A)
{
    LSA_UINT32                      FrameDataProperties;
    EDDP_PRM_PDIRDATA_PTR_TYPE      pPDIRData;

    FrameDataProperties = 0;
    pPDIRData           = LSA_NULL;

    /* ---------------------------------------------------------------- */
    /* "FrameDataProperties" must be present in PDIRData                */
    /* ---------------------------------------------------------------- */
    if ( !(EDDP_PRMPDIRDataIsFrameDataPropertiesPresent(pDDB, InActiveRecord_A)) )
    {
        // FrameDataProperties is not present in PDIRData!
        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataGetFrameDataProperties(InActiveRecord_A=0x%X): FATAL -> FrameDataProperties is not present in PDIRData!", 
            InActiveRecord_A);
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }

    /* ---------------------------------------------------------------- */
    /* In PDIRData_A or PDIRData_B ?                                    */
    /* ---------------------------------------------------------------- */
    if (InActiveRecord_A)
    {
        // search in active record buffer A - is PDIRData_A present ?
        if (pDDB->PRM.pRecordsetA->PDIRData_IsPresent)
            pPDIRData = pDDB->PRM.pPDIRData_A;
    }
    else
    {
        // search in new record buffer B - is PDIRData_B present ?
        if (pDDB->PRM.pRecordsetB->PDIRData_IsPresent)
            pPDIRData = pDDB->PRM.pPDIRData_B;
    }

    /* ---------------------------------------------------------------- */
    /* Get FrameDataProperties                                          */
    /* ---------------------------------------------------------------- */
    if (LSA_HOST_PTR_ARE_EQUAL(pPDIRData, LSA_NULL))
    {
        // PDIRData is not present!
        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataGetFrameDataProperties(InActiveRecord_A=0x%X): FATAL -> PDIRData is not present!", 
            InActiveRecord_A);
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }
    else
    {
        switch (Properties)
        {
            /* ---------------------------------------------------- */
            /* FrameDataProperties.ForwardingMode                   */
            /* ---------------------------------------------------- */
            case EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FORWARDINGMODE_SHIFT:
            {
                FrameDataProperties = EDDP_GET_BIT_VALUE( pPDIRData->FrameDataProperties, 
                                                          EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FORWARDINGMODE_MASK, 
                                                          EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FORWARDINGMODE_SHIFT);
            }
            break;

            /* ---------------------------------------------------- */
            /* FrameDataProperties.FastForwardingMulticastMACAdd    */
            /* ---------------------------------------------------- */
            case EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_SHIFT:
            {
                FrameDataProperties = EDDP_GET_BIT_VALUE( pPDIRData->FrameDataProperties, 
                                                          EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_MASK, 
                                                          EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_SHIFT);
            }
            break;

            /* ---------------------------------------------------- */
            /* FrameDataProperties.FragmentationMode                */
            /* ---------------------------------------------------- */
            case EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_SHIFT:
            {
                FrameDataProperties = EDDP_GET_BIT_VALUE( pPDIRData->FrameDataProperties, 
                                                          EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_MASK, 
                                                          EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_SHIFT);
            }
            break;

            default:
            {
                EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataGetFrameDataProperties(): FATAL -> The given Properties(0x%X) is unknown!", 
                    Properties);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }

    return (FrameDataProperties);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname   : EDDP_PRMPDIRDataGetRTC3PortInfoTx                                 +*/
/*+                                                                                     +*/
/*+  Input/Output   : EDDP_LOCAL_DDB_PTR_TYPE                   pDDB                    +*/
/*+                   LSA_UINT16                                FrameID                 +*/
/*+                   LSA_BOOL                                  isRedundant             +*/
/*+                   LSA_BOOL                                  InActiveRecord_A        +*/
/*+                   EDDP_PRM_RTC3_ALL_PORTS_INFO_PTR_TYPE     pPortInfoArray          +*/
/*+                                                                                     +*/
/*+  Result         : EDD_RSP                                                           +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input:                                                                             +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/ 
/*+  FrameID        : FrameID of corresponding provider                                 +*/
/*+  isRedundant    : Is there a redundant FrameID+1 for the given Frame ID?            +*/
/*+  InActiveRecord_A: LSA_TRUE  - search in active record buffer (A)                   +*/
/*+                    LSA_FALSE - search in new record buffer (B)                      +*/
/*+  pPortInfoArray : Address for port info array. Indexed by TxPortID-1.               +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+  Description: This function is called by CRT to get FrameSendOffset and             +*/
/*+               TxPorts for a given FrameID(x) and eventually also for its            +*/
/*+               redundant FrameID(x+1).                                               +*/
/*+               Caller has to provide the FrameID and if this FrameID has             +*/
/*+               a redundant pair (Parameter 'isRedundant').                           +*/
/*+               The collected data is then stored in 'pPortInfoArray'.                +*/
/*+               The index of this array represents the TxPortID-1!                    +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataGetRTC3PortInfoTx(
    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB,
    LSA_UINT16                                  FrameID,
    LSA_BOOL                                    isRedundant,
    LSA_BOOL                                    InActiveRecord_A,
    EDDP_PRM_RTC3_ALL_PORTS_INFO_PTR_TYPE       pPortInfoArray)
{
    EDD_RSP                         Response;
    LSA_UINT32                      i, k;
    LSA_UINT32                      TxPorts;
    EDDP_PRM_PDIRDATA_PTR_TYPE      pPDIRData;

    EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMPDIRDataGetRTC3PortInfoTx(FrameID=0x%X, isRedundant=0x%X, pPortInfoArray=0x%X)", 
        FrameID, isRedundant, pPortInfoArray);

    EDDP_ASSERT_FALSE(!(EDDP_IS_IRT_FRAMEID(FrameID)));

    Response    = EDD_STS_ERR_PARAM;
    pPDIRData   = LSA_NULL;

    // delete pPortInfoArray first
    for ( i=0; i < EDDP_MAX_PORT_CNT; i++ )
    {
        pPortInfoArray->PortInfoArray[i].FrameID            = 0;
        pPortInfoArray->PortInfoArray[i].ReductionRatio     = 0;
        pPortInfoArray->PortInfoArray[i].Phase              = 0;
        pPortInfoArray->PortInfoArray[i].FrameSendOffset    = EDDP_CRT_FRAME_SEND_OFFSET_UNDEFINED;
        pPortInfoArray->PortInfoArray[i].MCFrameID          = 0;
        pPortInfoArray->PortInfoArray[i].MCReductionRatio   = 0;
        pPortInfoArray->PortInfoArray[i].MCPhase            = 0;
        pPortInfoArray->PortInfoArray[i].MCFrameSendOffset  = EDDP_CRT_FRAME_SEND_OFFSET_UNDEFINED;
    }

    if (InActiveRecord_A)
    {
        // search in active record buffer A - is PDIRData_A present ?
        if (pDDB->PRM.pRecordsetA->PDIRData_IsPresent)
            pPDIRData = pDDB->PRM.pPDIRData_A;
    }
    else
    {
        // search in new record buffer B - is PDIRData_B present ?
        if (pDDB->PRM.pRecordsetB->PDIRData_IsPresent)
            pPDIRData = pDDB->PRM.pPDIRData_B;
    }

    if (LSA_HOST_PTR_ARE_EQUAL(pPDIRData, LSA_NULL))
    {
        // PDIRData is not present!
        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataGetRTC3PortInfoTx(InActiveRecord_A=0x%X): FATAL -> PDIRData is not present!", 
            InActiveRecord_A);
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }
    else
    {
        // IMPORTANT: always search the whole array
        // because one FrameID can have more entries (case of MultiCast: one FrameID at several ports with different FrameSendOffsets)
        for ( i=0; i < pPDIRData->IRFrameDataArray_Count_All; i++ )
        {
            if (pPDIRData->pIRFrameDataArray[i].FrameID == FrameID)
            {
                TxPorts = pPDIRData->pIRFrameDataArray[i].TxPortGroupArray;
                EDDP_SHIFT_VALUE_RIGHT(TxPorts, 1); // skip Bit(0) API
                k = 0;

                // loop through all ports of this FrameID (case of MultiCast: one FrameID at several ports with the same FrameSendOffset)
                while (TxPorts != 0)
                {
                    if (TxPorts & 0x1)
                    {
                        if (pPortInfoArray->PortInfoArray[k].FrameSendOffset == EDDP_CRT_FRAME_SEND_OFFSET_UNDEFINED)
                        {
                            pPortInfoArray->PortInfoArray[k].FrameID            = pPDIRData->pIRFrameDataArray[i].FrameID;
                            pPortInfoArray->PortInfoArray[k].ReductionRatio     = pPDIRData->pIRFrameDataArray[i].ReductionRatio;
                            pPortInfoArray->PortInfoArray[k].Phase              = pPDIRData->pIRFrameDataArray[i].Phase;
                            pPortInfoArray->PortInfoArray[k].FrameSendOffset    = pPDIRData->pIRFrameDataArray[i].FrameSendOffset;
                            Response = EDD_STS_OK;
                        }
                        else
                        {
                            // this FrameID is a MultiCast-FrameID
                            pPortInfoArray->PortInfoArray[k].MCFrameID          = pPDIRData->pIRFrameDataArray[i].FrameID;
                            pPortInfoArray->PortInfoArray[k].MCReductionRatio   = pPDIRData->pIRFrameDataArray[i].ReductionRatio;
                            pPortInfoArray->PortInfoArray[k].MCPhase            = pPDIRData->pIRFrameDataArray[i].Phase;
                            pPortInfoArray->PortInfoArray[k].MCFrameSendOffset  = pPDIRData->pIRFrameDataArray[i].FrameSendOffset;
                            Response = EDD_STS_OK;
                        }
                    }
                    k++; // next entry in PortInfoArray
                    EDDP_SHIFT_VALUE_RIGHT(TxPorts, 1); // next port of this FrameID
                }
            }

            // is this FrameID redundant ?
            if ( (isRedundant) && (pPDIRData->pIRFrameDataArray[i].FrameID == (FrameID+1)) )
            {
                TxPorts = pPDIRData->pIRFrameDataArray[i].TxPortGroupArray;
                EDDP_SHIFT_VALUE_RIGHT(TxPorts, 1); // skip Bit(0) API
                k = 0;

                // loop through all ports of this FrameID(x+1)
                // normaly the redundant FrameID(x+1) may have exact one TxPort
                while (TxPorts != 0)
                {
                    if (TxPorts & 0x1)
                    {
                        if (pPortInfoArray->PortInfoArray[k].FrameSendOffset == EDDP_CRT_FRAME_SEND_OFFSET_UNDEFINED)
                        {
                            pPortInfoArray->PortInfoArray[k].FrameID            = pPDIRData->pIRFrameDataArray[i].FrameID;
                            pPortInfoArray->PortInfoArray[k].ReductionRatio     = pPDIRData->pIRFrameDataArray[i].ReductionRatio;
                            pPortInfoArray->PortInfoArray[k].Phase              = pPDIRData->pIRFrameDataArray[i].Phase;
                            pPortInfoArray->PortInfoArray[k].FrameSendOffset    = pPDIRData->pIRFrameDataArray[i].FrameSendOffset;
                            Response = EDD_STS_OK;
                        }
                        else
                        {
                            // this FrameID is a MultiCast-FrameID
                            pPortInfoArray->PortInfoArray[k].MCFrameID          = pPDIRData->pIRFrameDataArray[i].FrameID;
                            pPortInfoArray->PortInfoArray[k].MCReductionRatio   = pPDIRData->pIRFrameDataArray[i].ReductionRatio;
                            pPortInfoArray->PortInfoArray[k].MCPhase            = pPDIRData->pIRFrameDataArray[i].Phase;
                            pPortInfoArray->PortInfoArray[k].MCFrameSendOffset  = pPDIRData->pIRFrameDataArray[i].FrameSendOffset;
                            Response = EDD_STS_OK;
                        }
                    }
                    k++; // next entry in PortInfoArray
                    EDDP_SHIFT_VALUE_RIGHT(TxPorts, 1); // next port of this FrameID
                }
            }
        }
    }

    return (Response);
}



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRDataIsFrameDataPropertiesPresent            +*/
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
/*+  Description: This function gets info whether the "FrameDataProperties" is present  +*/
/*+               in PDIRData-Record.                                                   +*/
/*+                                                                                     +*/
/*+               Check only if PDIRData is present! If no -> FATAL!                    +*/
/*+                                                                                     +*/
/*+               Return LSA_TRUE  : FrameDataProperties is present in PDIRData         +*/
/*+                                     -> PDIRFrameData(V1.1)                          +*/
/*+                      LSA_FALSE : FrameDataProperties is not present in PDIRData     +*/
/*+                                     -> PDIRFrameData(V1.0)                          +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataIsFrameDataPropertiesPresent(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_BOOL                        InActiveRecord_A)
{
    EDDP_PRM_PDIRDATA_PTR_TYPE      pPDIRData = LSA_NULL;

    if (InActiveRecord_A)
    {
        // search in active record buffer A - is PDIRData_A present ?
        if (pDDB->PRM.pRecordsetA->PDIRData_IsPresent)
            pPDIRData = pDDB->PRM.pPDIRData_A;
    }
    else
    {
        // search in new record buffer B - is PDIRData_B present ?
        if (pDDB->PRM.pRecordsetB->PDIRData_IsPresent)
            pPDIRData = pDDB->PRM.pPDIRData_B;
    }

    if (LSA_HOST_PTR_ARE_EQUAL(pPDIRData, LSA_NULL))
    {
        // PDIRData is not present!
        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataIsFrameDataPropertiesPresent(InActiveRecord_A=0x%X): FATAL -> PDIRData is not present!", 
            InActiveRecord_A);
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }

    return (pPDIRData->FrameDataProperties_IsPresent);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRDataIsMRPDWatchDogActive                    +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      RedundFrameID           +*/
/*+                             EDD_COMMON_MEM_U16_PTR_TYPE     pPDIRFrameDataIdx       +*/
/*+                             LSA_BOOL                        InActiveRecord_A        +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_BOOL                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  RedundFrameID      : redundant FrameID                                             +*/
/*+  pPDIRFrameDataIdx  : address for writing of PDIRFrameDataIdx of found FrameID     +*/
/*+  InActiveRecord_A   : LSA_TRUE  - search in active record buffer (A)                +*/
/*+                       LSA_FALSE - search in new record buffer (B)                   +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks for given FrameID whether                        +*/
/*+               the MediaRedundancyWatchDog is active.                                +*/
/*+                                                                                     +*/
/*+               The given FrameID must be in redundant FrameID range. If no, FATAL!   +*/
/*+                                                                                     +*/
/*+               If the given FrameID is not found in PDIRFrameData, FATAL!            +*/
/*+                                                                                     +*/
/*+               Return LSA_TRUE  : The MediaRedundancyWatchDog is active.             +*/
/*+                      LSA_FALSE : The MediaRedundancyWatchDog is not active.         +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataIsMRPDWatchDogActive(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_UINT16                      RedundFrameID,
    EDD_COMMON_MEM_U16_PTR_TYPE     pPDIRFrameDataIdx,
    LSA_BOOL                        InActiveRecord_A)
{
    LSA_BOOL                        IsMRPDWatchDogActive;
    LSA_UINT32                      i;
    LSA_UINT8                       ValueTemp_8;
    LSA_BOOL                        FrameIDFound;
    EDDP_PRM_PDIRDATA_PTR_TYPE      pPDIRData;

    IsMRPDWatchDogActive    = LSA_FALSE;
    FrameIDFound            = LSA_FALSE;
    pPDIRData               = LSA_NULL;

    // only for redundant IRT FrameID range
    if (EDDP_IS_IRT_REDUNDANT_FRAMEID(RedundFrameID))
    {
        if (InActiveRecord_A)
        {
            // search in active record buffer A - is PDIRData_A present ?
            if (pDDB->PRM.pRecordsetA->PDIRData_IsPresent)
                pPDIRData = pDDB->PRM.pPDIRData_A;
        }
        else
        {
            // search in new record buffer B - is PDIRData_B present ?
            if (pDDB->PRM.pRecordsetB->PDIRData_IsPresent)
                pPDIRData = pDDB->PRM.pPDIRData_B;
        }

        if (LSA_HOST_PTR_ARE_EQUAL(pPDIRData, LSA_NULL))
        {
            // PDIRData is not present!
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataIsMRPDWatchDogActive(InActiveRecord_A=0x%X): FATAL -> PDIRData is not present!", 
                InActiveRecord_A);
            EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
        }
        else
        {
            for ( i=0; i < pPDIRData->IRFrameDataArray_Count_All; i++ )
            {
                // look for FrameID as Consumer
                if (   (pPDIRData->pIRFrameDataArray[i].FrameID == RedundFrameID)
                    && (   (pPDIRData->pIRFrameDataArray[i].SndRcvRole == EDDP_PRM_PDIRFRAMEDATA_ROLE_CONSUMER) 
                        || (pPDIRData->pIRFrameDataArray[i].SndRcvRole == EDDP_PRM_PDIRFRAMEDATA_ROLE_CONSUMER_FORWARDER)
                       )
                   )
                {
                    // get value of FrameDetails.MediaRedundancyWatchDog
                    ValueTemp_8 = EDDP_GET_BIT_VALUE( pPDIRData->pIRFrameDataArray[i].FrameDetails, 
                                                      EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_MEDIAREDUNDANCYWATCHDOG_MASK, 
                                                      EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_MEDIAREDUNDANCYWATCHDOG_SHIFT);
                    if (ValueTemp_8 == EDDP_PRM_PDIRFRAMEDATA_FRAMEDETAILS_MEDIAREDUNDANCYWATCHDOG_MRT_ON)
                    {
                        IsMRPDWatchDogActive = LSA_TRUE;
                        *pPDIRFrameDataIdx = (LSA_UINT16) i;
                    }

                    i               = pPDIRData->IRFrameDataArray_Count_All;  // exit FOR loop
                    FrameIDFound    = LSA_TRUE;
                }
            }
        }

        if ( !(FrameIDFound) )
        {
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataIsMRPDWatchDogActive(): FATAL -> The given FrameID(0x%X) is not found in PDIRData-Record!", 
                RedundFrameID);
            EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
        }
    }
    else
    {
        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataIsMRPDWatchDogActive(): FATAL -> The given FrameID(0x%X) is not redundant!", 
            RedundFrameID);
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }

    return (IsMRPDWatchDogActive);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRDataIsRedundantFrameIDCoupleAvailable       +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      RedundFrameID           +*/
/*+                             LSA_BOOL                        InActiveRecord_A        +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_BOOL                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  RedundFrameID      : redundant FrameID(x) or (x+1)                                 +*/
/*+  InActiveRecord_A   : LSA_TRUE  - search in active record buffer (A)                +*/
/*+                       LSA_FALSE - search in new record buffer (B)                   +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks for given redundant FrameID(x) or (x+1) whether  +*/
/*+               the PDIRData-Record contains the couple of this FrameID:              +*/
/*+                 - if the given FrameID is (x), look for (x+1)                       +*/
/*+                 - if the given FrameID is (x+1), look for (x)                       +*/
/*+                                                                                     +*/
/*+               The given FrameID must be in redundant FrameID range.                 +*/
/*+               If no, no check!                                                      +*/
/*+                                                                                     +*/
/*+               In given parameter "InActiveRecord_A" the caller has to say in which  +*/
/*+               record buffer the couple to FrameID must be looked for (in A or B).   +*/
/*+                                                                                     +*/
/*+               This function can not be used if DFP is present, because              +*/
/*+               the FrameID(x) or FrameID(x+1) can be present double in PDIRData!     +*/
/*+                                                                                     +*/
/*+               Return LSA_TRUE  : PDIRData contains the couple to given FrameID      +*/
/*+                      LSA_FALSE : PDIRData does not contain the couple to given FrameID+*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataIsRedundantFrameIDCoupleAvailable(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_UINT16                      RedundFrameID,
    LSA_BOOL                        InActiveRecord_A)
{
    LSA_BOOL                        State;
    LSA_UINT32                      i;
    LSA_UINT16                      CoupleFrameID;  // FrameID, which is looked for
    EDDP_PRM_PDIRDATA_PTR_TYPE      pPDIRData;

    State       = LSA_FALSE;
    pPDIRData   = LSA_NULL;

    // only for redundant IRT FrameID range
    if (EDDP_IS_IRT_REDUNDANT_FRAMEID(RedundFrameID))
    {
        if (InActiveRecord_A)
        {
            // search in active record buffer A - is PDIRData_A present ?
            if (pDDB->PRM.pRecordsetA->PDIRData_IsPresent)
                pPDIRData = pDDB->PRM.pPDIRData_A;
        }
        else
        {
            // search in new record buffer B - is PDIRData_B present ?
            if (pDDB->PRM.pRecordsetB->PDIRData_IsPresent)
                pPDIRData = pDDB->PRM.pPDIRData_B;
        }

        if (LSA_HOST_PTR_ARE_EQUAL(pPDIRData, LSA_NULL))
        {
            // PDIRData is not present!
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataIsRedundantFrameIDCoupleAvailable(InActiveRecord_A=0x%X): FATAL -> PDIRData is not present!", 
                InActiveRecord_A);
            EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
        }
        else
        {
            if (RedundFrameID & 0x1)
                CoupleFrameID = RedundFrameID - 1;  // look for FrameID(x)
            else
                CoupleFrameID = RedundFrameID + 1;  // look for FrameID(x+1)

            for ( i=0; i < pPDIRData->IRFrameDataArray_Count_All; i++ )
            {
                if (pPDIRData->pIRFrameDataArray[i].FrameID == CoupleFrameID)
                {
                    State   = LSA_TRUE;
                    i       = pPDIRData->IRFrameDataArray_Count_All;    // exit FOR loop
                }
            }
        }
    }
    else
    {
        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] EDDP_PRMPDIRDataIsRedundantFrameIDCoupleAvailable(): The given RedundFrameID(0x%X) is not redundant!", 
            RedundFrameID);
    }

    return (State);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRDataIsPDIRDataPresent                       +*/
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
/*+  Description: This function gets info whether the "PDIRData" is present.            +*/
/*+                                                                                     +*/
/*+               Return LSA_TRUE  : PDIRData is present                                +*/
/*+                      LSA_FALSE : PDIRData is not present                            +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataIsPDIRDataPresent(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_BOOL                        InActiveRecord_A)
{
    LSA_BOOL        State = LSA_FALSE;

    if (InActiveRecord_A)
    {
        // search in active record buffer A - is PDIRData_A present ?
        if (pDDB->PRM.pRecordsetA->PDIRData_IsPresent)
            State = LSA_TRUE;
    }
    else
    {
        // search in new record buffer B - is PDIRData_B present ?
        if (pDDB->PRM.pRecordsetB->PDIRData_IsPresent)
            State = LSA_TRUE;
    }

    return (State);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRDataCheckDoubleFrameID                      +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                        +*/
/*+                             LSA_UINT16                  PDIRFrameDataIdx            +*/
/*+                                                                                     +*/
/*+  Result                :    EDD_RSP                                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  PDIRFrameDataIdx   : index in PDIRData.PDIRFrameData[x]                            +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks the given FrameID is double in PDIRFrameData.    +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataCheckDoubleFrameID(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PDIRFrameDataIdx)
{
    EDD_RSP                             Response;
    EDDP_PRM_DETAIL_ERR_PTR_TYPE        pDetailErr;
    LSA_UINT32                          j;
    EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE     pPDIRFrameData;

    EDDP_ASSERT_FALSE(!(pDDB->PRM.pRecordsetB->PDIRData_IsPresent));
    EDDP_ASSERT_FALSE(PDIRFrameDataIdx >= pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All);

    Response        = EDD_STS_OK;
    pDetailErr      = &pDDB->PrmDetailErr;
    pPDIRFrameData  = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[PDIRFrameDataIdx];

    for ( j=PDIRFrameDataIdx+1; j < pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All; j++ )
    {
        if (pPDIRFrameData->FrameID == pDDB->PRM.pPDIRData_B->pIRFrameDataArray[j].FrameID)
        {
            pPDIRFrameData->isDoubleFrameID                             = LSA_TRUE;
            pDDB->PRM.pPDIRData_B->pIRFrameDataArray[j].isDoubleFrameID   = LSA_TRUE;

            // only for IRT FrameID range
            if (EDDP_IS_IRT_FRAMEID(pPDIRFrameData->FrameID))
            {
                if (   (pPDIRFrameData->RxPort == 0) 
                    && (pDDB->PRM.pPDIRData_B->pIRFrameDataArray[j].RxPort == 0)
                    )
                {
                    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] EDDP_PRMPDIRDataCheckDoubleFrameID(): There are double FrameIDs(0x%X) with RxPort=0 (Provider)", 
                        pPDIRFrameData->FrameID);
                }
                else
                {
                    if (pDDB->PRM.pRecordsetB->PDIRSubframeData_IsPresent)
                    {
                        // for DFP it is no error
                    }
                    else
                    {
                        EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDIRDATA_RECORD_INDEX, pDDB->PRM.pPDIRData_B->pIRFrameDataArray[j].OffsetFrameID, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                        Response = EDD_STS_ERR_PRM_CONSISTENCY;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataCheckDoubleFrameID(): ERROR -> There are double FrameIDs(0x%X) with RxPort!=0 (Consumer/Forwarder)", 
                            pPDIRFrameData->FrameID);
                        // exit FOR loop
                        j = pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All;
                    }
                }
            }
        }
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname   :    EDDP_PRMPDIRDataInitSndRcvRole                                 +*/
/*+                                                                                     +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB                            +*/
/*+                      LSA_UINT16                     PDIRFrameDataIdx                +*/
/*+                      LSA_BOOL                       InActiveRecord_A                +*/
/*+                                                                                     +*/
/*+  Result         :    LSA_VOID                                                       +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  PDIRFrameDataIdx   : index in PDIRData.PDIRFrameData[x]                            +*/
/*+  InActiveRecord_A   : LSA_TRUE  - search in active record buffer (A)                +*/
/*+                       LSA_FALSE - search in new record buffer (B)                   +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function inits the "SndRcvRole" of given PDIRFrameData.          +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataInitSndRcvRole(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PDIRFrameDataIdx,
    LSA_BOOL                    InActiveRecord_A)
{
    LSA_UINT32                          TxPortCount;
    EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE     pPDIRFrameData;

    // is PDIRData present ?
    if (EDDP_PRMPDIRDataIsPDIRDataPresent(pDDB, InActiveRecord_A))
    {
        if (InActiveRecord_A)
        {
            // PDIRData_A
            EDDP_ASSERT_FALSE(PDIRFrameDataIdx >= pDDB->PRM.pPDIRData_A->IRFrameDataArray_Count_All);
            pPDIRFrameData = &pDDB->PRM.pPDIRData_A->pIRFrameDataArray[PDIRFrameDataIdx];
        }
        else
        {
            // PDIRData_B
            EDDP_ASSERT_FALSE(PDIRFrameDataIdx >= pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All);
            pPDIRFrameData = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[PDIRFrameDataIdx];
        }

        if (pPDIRFrameData->TxPortGroupArray == 0)
        {
            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataInitSndRcvRole(): FATAL -> The given TxPortGroupArray is 0x0!");
            EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
        }
        else
        {
            if (pPDIRFrameData->RxPort == 0)
            {
                /* ------------ */
                /* Provider     */
                /* ------------ */
                pPDIRFrameData->SndRcvRole = EDDP_PRM_PDIRFRAMEDATA_ROLE_PROVIDER;
            }
            else
            {
                /* ------------ */
                /* Consumer     */
                /* Forwarder    */
                /* ------------ */
                TxPortCount = EDDP_PRMGetPortCountFromTxPortGroupArray (pDDB, pPDIRFrameData->TxPortGroupArray);

                if (pPDIRFrameData->TxPortGroupArray & 0x1)
                {
                    // Bit(0) API is present
                    if (TxPortCount == 0)
                        pPDIRFrameData->SndRcvRole = EDDP_PRM_PDIRFRAMEDATA_ROLE_CONSUMER;
                    else
                        pPDIRFrameData->SndRcvRole = EDDP_PRM_PDIRFRAMEDATA_ROLE_CONSUMER_FORWARDER;
                }
                else
                {
                    // Bit(0) API is not present
                    if (TxPortCount == 0)
                    {
                        // case of (TxPortGroupArray=0x0) is not possible
                        // this is already checked above
                    }
                    else
                        pPDIRFrameData->SndRcvRole = EDDP_PRM_PDIRFRAMEDATA_ROLE_FORWARDER;
                }
            }
        }
    }
    else
    {
        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataInitSndRcvRole(InActiveRecord_A=0x%X): FATAL -> The record PDIRData is not present!", 
            InActiveRecord_A);
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRDataIsSameIrtActive                         +*/
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
/*+  Description: This function gets info whether the same IRT configuration is         +*/
/*+               already active in RecordsetA.                                         +*/
/*+                                                                                     +*/
/*+               These records are compared between RecordsetA and RecordsetB:         +*/
/*+                 - PDSyncData                                                        +*/
/*+                 - PDIRData                                                          +*/
/*+                                                                                     +*/
/*+               Return LSA_TRUE  : same IRT is already active                         +*/
/*+                      LSA_FALSE : new IRT is comming                                 +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataIsSameIrtActive(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB)
{
    LSA_BOOL        State = LSA_FALSE;

    /*
    Info for next "if": 'PDSyncData' and 'PDIRData' have to be evaluated together!

    Example 1: Previous config was RTC3 so there is 'PDSyncData' AND 'PDIRData' in Recordset A.
               New config is RTC2 so there is only 'PDSyncData' which is the same as in Recordset A
               but overall the new config is different to the previous one.
               ((PDSyncData_Skip) && (!(PDIRData_Skip))) -> NO SKIP

    Example 2: Previous config was RTC2 so there is only 'PDSyncData' in Recordset A.
               New config is the same as previous one (there is only 'PDSyncData' wich is equal to the one in Recordset A).
               ((PDSyncData_Skip) && (!(PDIRData_Skip))) -> SKIP

    Solution: Also the _IsPresent variables have to be taken into account for this logic.

    In psydo code it would look like this:

    if (pDDB->PRM.RecordsetA.PDSyncData_IsPresent equals pDDB->PRM.RecordsetB.PDSyncData_IsPresent equals pDDB->PRM.RecordsetB.PDSyncData_Skip) and
       (pDDB->PRM.RecordsetA.PDIRData_IsPresent   equals pDDB->PRM.RecordsetB.PDIRData_IsPresent   equals pDDB->PRM.RecordsetB.PDSyncData_Skip)
    then config set A equals config set B for the the records 'PDSyncData' and 'PDIRData'.            
    */

    if ( (   (pDDB->PRM.pRecordsetA->PDSyncData_IsPresent == pDDB->PRM.pRecordsetB->PDSyncData_IsPresent)
          && (pDDB->PRM.pRecordsetB->PDSyncData_IsPresent == pDDB->PRM.pRecordsetB->PDSyncData_Skip)
         )
         &&
         (   (pDDB->PRM.pRecordsetA->PDIRData_IsPresent == pDDB->PRM.pRecordsetB->PDIRData_IsPresent)
          && (pDDB->PRM.pRecordsetB->PDIRData_IsPresent == pDDB->PRM.pRecordsetB->PDIRData_Skip)
         )
       )
    {
        // skip: same IRT configuration is already active
        State = LSA_TRUE;
    }

    return (State);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRDataIsSameIRApplicationDataActive           +*/
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
/*+  Description: This function gets info whether the same IRApplicationData            +*/
/*+               configuration is already active in RecordsetA.                        +*/
/*+                                                                                     +*/
/*+               This record is compared between RecordsetA and RecordsetB:            +*/
/*+                 - PDIRApplicationData                                               +*/
/*+                                                                                     +*/
/*+               Return LSA_TRUE  : same IRApplicationData is already active           +*/
/*+                      LSA_FALSE : new IRApplicationData is comming                   +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataIsSameIRApplicationDataActive(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB)
{
    LSA_BOOL        State = LSA_FALSE;

    if (   (pDDB->PRM.pRecordsetA->PDIRApplicationData_IsPresent == pDDB->PRM.pRecordsetB->PDIRApplicationData_IsPresent)
        && (pDDB->PRM.pRecordsetB->PDIRApplicationData_IsPresent == pDDB->PRM.pRecordsetB->PDIRApplicationData_Skip)
       )
    {
        // skip: same IRApplicationData configuration is already active
        State = LSA_TRUE;
    }

    return (State);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRDataCalculateEndOfRedPhaseNsIF              +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB            +*/
/*+                             LSA_BOOL                                InActiveRecord_A+*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
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
/*+  Description: This function calculates the parameters:                              +*/
/*+                 - EndOfRedPhaseIFPorts.RxNs                                         +*/
/*+                 - EndOfRedPhaseIFPorts.TxNs                                         +*/
/*+                 - EndOfRedPhaseIFPorts.RxTxNs                                       +*/
/*+                                                                                     +*/
/*+               -> from PDIRData.PDIRBeginEndData.BeginEndDataArray[x].               +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataCalculateEndOfRedPhaseNsIF(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    LSA_BOOL                        InActiveRecord_A)
{
    LSA_UINT32                      iPort, iAssignment;
    EDDP_PRM_PDIRDATA_PTR_TYPE      pPDIRData;

    pPDIRData       = LSA_NULL;

    // only if PDIRData record is present
    if (EDDP_PRMPDIRDataIsPDIRDataPresent(pDDB, InActiveRecord_A))
    {
        if (InActiveRecord_A)
        {
            // search in active record buffer A - is PDIRData_A present ?
            if (pDDB->PRM.pRecordsetA->PDIRData_IsPresent)
                pPDIRData = pDDB->PRM.pPDIRData_A;
        }
        else
        {
            // search in new record buffer B - is PDIRData_B present ?
            if (pDDB->PRM.pRecordsetB->PDIRData_IsPresent)
                pPDIRData = pDDB->PRM.pPDIRData_B;
        }

        if (LSA_HOST_PTR_ARE_EQUAL(pPDIRData, LSA_NULL))
        {
            // PDIRData is not present!
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataCalculateEndOfRedPhaseNsIF(InActiveRecord_A=0x%X): FATAL -> PDIRData is not present!", 
                InActiveRecord_A);
            EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
        }
        else
        {
            // delete values
            pPDIRData->EndOfRedPhaseIFPorts.RxNs    = 0;
            pPDIRData->EndOfRedPhaseIFPorts.TxNs    = 0;
            pPDIRData->EndOfRedPhaseIFPorts.RxTxNs  = 0;

            // and calculate new values
            for ( iPort=0; iPort < pPDIRData->PDIRBeginEndData.NumberOfPorts; iPort++ )
            {
                for ( iAssignment=0; iAssignment < pPDIRData->PDIRBeginEndData.BeginEndDataArray[iPort].NumberOfAssignments; iAssignment++ )
                {
                    /* ------------------------------------ */
                    /* EndOfRedPhaseIFPorts.RxNs            */
                    /* ------------------------------------ */
                    // is Rx red phase present in BeginEndDataArray ?
                    if (   (pPDIRData->PDIRBeginEndData.BeginEndDataArray[iPort].Assignments[iAssignment].RxRedOrangePeriodBegin) 
                        != (pPDIRData->PDIRBeginEndData.BeginEndDataArray[iPort].Assignments[iAssignment].RxOrangePeriodBegin)
                       )
                    {
                        if (pPDIRData->PDIRBeginEndData.BeginEndDataArray[iPort].Assignments[iAssignment].RxOrangePeriodBegin > pPDIRData->EndOfRedPhaseIFPorts.RxNs)
                        {
                            pPDIRData->EndOfRedPhaseIFPorts.RxNs = pPDIRData->PDIRBeginEndData.BeginEndDataArray[iPort].Assignments[iAssignment].RxOrangePeriodBegin;
                        }
                    }

                    /* ------------------------------------ */
                    /* EndOfRedPhaseIFPorts.TxNs            */
                    /* ------------------------------------ */
                    // is Tx red phase present in BeginEndDataArray ?
                    if (   (pPDIRData->PDIRBeginEndData.BeginEndDataArray[iPort].Assignments[iAssignment].TxRedOrangePeriodBegin) 
                        != (pPDIRData->PDIRBeginEndData.BeginEndDataArray[iPort].Assignments[iAssignment].TxOrangePeriodBegin)
                       )
                    {
                        if (pPDIRData->PDIRBeginEndData.BeginEndDataArray[iPort].Assignments[iAssignment].TxOrangePeriodBegin > pPDIRData->EndOfRedPhaseIFPorts.TxNs)
                        {
                            pPDIRData->EndOfRedPhaseIFPorts.TxNs = pPDIRData->PDIRBeginEndData.BeginEndDataArray[iPort].Assignments[iAssignment].TxOrangePeriodBegin;
                        }
                    }
                }
            }

            /* ------------------------------------ */
            /* EndOfRedPhaseIFPorts.RxTxNs          */
            /* ------------------------------------ */
            // Rx
            if (pPDIRData->EndOfRedPhaseIFPorts.RxNs > pPDIRData->EndOfRedPhaseIFPorts.RxTxNs)
            {
                pPDIRData->EndOfRedPhaseIFPorts.RxTxNs = pPDIRData->EndOfRedPhaseIFPorts.RxNs;
            }
            // Tx
            if (pPDIRData->EndOfRedPhaseIFPorts.TxNs > pPDIRData->EndOfRedPhaseIFPorts.RxTxNs)
            {
                pPDIRData->EndOfRedPhaseIFPorts.RxTxNs = pPDIRData->EndOfRedPhaseIFPorts.TxNs;
            }
        }
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRDataGetEndOfRedPhaseNsIF                    +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB            +*/
/*+                             EDDP_PRM_RSV_INTERVAL_RED_MODE_TYPE     Mode            +*/
/*+                             LSA_BOOL                                InActiveRecord_A+*/
/*+                                                                                     +*/
/*+  Result                :    LSA_UINT32                                              +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  Mode               : Mode - see defines                                            +*/
/*+  InActiveRecord_A   : LSA_TRUE  - search in active record buffer (A)                +*/
/*+                       LSA_FALSE - search in new record buffer (B)                   +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Get current value for end of red phase of interface and all ports     +*/
/*+               for given Mode.                                                       +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataGetEndOfRedPhaseNsIF(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_PRM_RSV_INTERVAL_RED_MODE_TYPE     Mode,
    LSA_BOOL                                InActiveRecord_A)
{
    LSA_UINT32                      EndOfRedPhaseNs = 0;
    EDDP_PRM_PDIRDATA_PTR_TYPE      pPDIRData       = LSA_NULL;

    if (InActiveRecord_A)
    {
        // search in active record buffer A - is PDIRData_A present ?
        if (pDDB->PRM.pRecordsetA->PDIRData_IsPresent)
            pPDIRData = pDDB->PRM.pPDIRData_A;
    }
    else
    {
        // search in new record buffer B - is PDIRData_B present ?
        if (pDDB->PRM.pRecordsetB->PDIRData_IsPresent)
            pPDIRData = pDDB->PRM.pPDIRData_B;
    }

    // get only if PDIRData is present
    if ( !(LSA_HOST_PTR_ARE_EQUAL(pPDIRData, LSA_NULL)) )
    {
        switch (Mode)
        {
            case EDDP_PRM_RSV_INTERVAL_RED_MODE_RX:
                EndOfRedPhaseNs = pPDIRData->EndOfRedPhaseIFPorts.RxNs;
            break;

            case EDDP_PRM_RSV_INTERVAL_RED_MODE_TX:
                EndOfRedPhaseNs = pPDIRData->EndOfRedPhaseIFPorts.TxNs;
            break;

            case EDDP_PRM_RSV_INTERVAL_RED_MODE_RX_TX:
                EndOfRedPhaseNs = pPDIRData->EndOfRedPhaseIFPorts.RxTxNs;
            break;

            default:
            {
                EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataGetEndOfRedPhaseNsIF(): FATAL -> The given Mode(0x%X) is unknown!", 
                    Mode);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }

    return (EndOfRedPhaseNs);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname   :    EDDP_PRMPDIRDataSetupLastLocalIRTFrame                         +*/
/*+                                                                                     +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB                            +*/
/*+                      LSA_UINT16                     PDIRFrameDataIdx                +*/
/*+                      LSA_BOOL                       InActiveRecord_A                +*/
/*+                                                                                     +*/
/*+  Result         :    LSA_VOID                                                       +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  PDIRFrameDataIdx   : index in PDIRData.PDIRFrameData[x]                            +*/
/*+  InActiveRecord_A   : LSA_TRUE  - search in active record buffer (A)                +*/
/*+                       LSA_FALSE - search in new record buffer (B)                   +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function sets up the last local sent/received IRT frame from     +*/
/*+               PDIRData.PDIRFrameData.                                               +*/
/*+                                                                                     +*/
/*+               Data of last frame are saved in                                       +*/
/*+               PDIRData_A/B.LastLocalIRTFrameArray[PortID]                           +*/
/*+                 Rx/Tx-EndLocalNs        = FrameSendOffset                           +*/
/*+                 Rx/Tx-LastLocalFrameLen = DataLength                                +*/
/*+                                                                                     +*/
/*+               Call this function always after init of "SndRcvRole"!                 +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataSetupLastLocalIRTFrame(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PDIRFrameDataIdx,
    LSA_BOOL                    InActiveRecord_A)
{
    LSA_UINT32                          PortID;
    LSA_UINT32                          TxPorts;
    EDDP_PRM_PDIRDATA_PTR_TYPE          pPDIRData = LSA_NULL;
    EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE     pPDIRFrameData;

    // is PDIRData present ?
    if (EDDP_PRMPDIRDataIsPDIRDataPresent(pDDB, InActiveRecord_A))
    {
        if (InActiveRecord_A)
        {
            // search in active record buffer A - is PDIRData_A present ?
            if (pDDB->PRM.pRecordsetA->PDIRData_IsPresent)
                pPDIRData = pDDB->PRM.pPDIRData_A;
        }
        else
        {
            // search in new record buffer B - is PDIRData_B present ?
            if (pDDB->PRM.pRecordsetB->PDIRData_IsPresent)
                pPDIRData = pDDB->PRM.pPDIRData_B;
        }

        if (LSA_HOST_PTR_ARE_EQUAL(pPDIRData, LSA_NULL))
        {
            // PDIRData is not present!
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataSetupLastLocalIRTFrame(InActiveRecord_A=0x%X): FATAL -> PDIRData is not present!", 
                InActiveRecord_A);
            EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
        }
        else
        {
            EDDP_ASSERT_FALSE(PDIRFrameDataIdx >= pPDIRData->IRFrameDataArray_Count_All);
            pPDIRFrameData = &pPDIRData->pIRFrameDataArray[PDIRFrameDataIdx];

            // only for IRT FrameID range
            if (EDDP_IS_IRT_FRAMEID(pPDIRFrameData->FrameID))
            {
                // local sent/received frame (Provider or Consumer)
                switch (pPDIRFrameData->SndRcvRole)
                {
                    /* -------------------------------- */
                    /* Provider - Tx                    */
                    /* -------------------------------- */
                    case EDDP_PRM_PDIRFRAMEDATA_ROLE_PROVIDER:
                    {
                        // get TxPorts of this frame
                        TxPorts = pPDIRFrameData->TxPortGroupArray;
                        // TxPorts must be always present
                        EDDP_ASSERT_FALSE(TxPorts == 0);
                        // skip Bit(0) API
                        EDDP_SHIFT_VALUE_RIGHT(TxPorts, 1);

                        PortID = 1;
                        // loop through all ports of this send frame
                        while (TxPorts != 0)
                        {
                            EDDP_ASSERT_FALSE((PortID < 1) || (PortID > pDDB->SWI.PortCnt));

                            if (TxPorts & 0x1)
                            {
                                if (pPDIRFrameData->FrameSendOffset > pPDIRData->LastLocalIRTFrameArray[PortID-1].TxEndLocalNs)
                                {
                                    pPDIRData->LastLocalIRTFrameArray[PortID-1].TxEndLocalNs        = pPDIRFrameData->FrameSendOffset;
                                    pPDIRData->LastLocalIRTFrameArray[PortID-1].TxLastLocalFrameLen = pPDIRFrameData->DataLength;
                                }
                            }

                            EDDP_SHIFT_VALUE_RIGHT(TxPorts, 1); // next port of this frame
                            PortID++;
                        }
                    }
                    break;

                    /* -------------------------------- */
                    /* Consumer - Rx                    */
                    /* -------------------------------- */
                    case EDDP_PRM_PDIRFRAMEDATA_ROLE_CONSUMER:
                    case EDDP_PRM_PDIRFRAMEDATA_ROLE_CONSUMER_FORWARDER:
                    {
                        // get PortID of this frame
                        PortID = pPDIRFrameData->RxPort;
                        // receive PortID must be always valid
                        EDDP_ASSERT_FALSE((PortID < 1) || (PortID > pDDB->SWI.PortCnt));

                        if (pPDIRFrameData->FrameSendOffset > pPDIRData->LastLocalIRTFrameArray[PortID-1].RxEndLocalNs)
                        {
                            pPDIRData->LastLocalIRTFrameArray[PortID-1].RxEndLocalNs        = pPDIRFrameData->FrameSendOffset;
                            pPDIRData->LastLocalIRTFrameArray[PortID-1].RxLastLocalFrameLen = pPDIRFrameData->DataLength;
                        }
                    }
                    break;

                    /* -------------------------------- */
                    /* Forwarder                        */
                    /* -------------------------------- */
                    case EDDP_PRM_PDIRFRAMEDATA_ROLE_FORWARDER:
                    {
                        // nothing to do
                    }
                    break;

                    default:
                    {
                        EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataSetupLastLocalIRTFrame(PDIRFrameDataIdx=%d, InActiveRecord_A=0x%X): FATAL -> SndRcvRole(%d) is unknown!", 
                            PDIRFrameDataIdx, InActiveRecord_A, pPDIRFrameData->SndRcvRole);
                        EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                    }
                    break;
                }
            }
        }
    }
    else
    {
        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataSetupLastLocalIRTFrame(InActiveRecord_A=0x%X): FATAL -> The record PDIRData is not present!", 
            InActiveRecord_A);
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRDataCheckFrameSendOffset                    +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                        +*/
/*+                             LSA_UINT16                  PDIRFrameDataIdx            +*/
/*+                                                                                     +*/
/*+  Result                :    EDD_RSP                                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  PDIRFrameDataIdx   : index in PDIRData.PDIRFrameData[x]                            +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks the FrameSendOffset is in valid range            +*/
/*+               of given PDIRFrameData entry.                                         +*/
/*+               The check is dependent of Hardware (ERTEC200P/FPGA) and               +*/
/*+               long/short Preamble.                                                  +*/
/*+                                                                                     +*/
/*+               The FrameSendOffset must be already copied to PDIRData_B.             +*/
/*+                                                                                     +*/
/*+               This check is only for IRT-FrameIDs, not for Sync-FrameIDs!           +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataCheckFrameSendOffset(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PDIRFrameDataIdx)
{
    EDD_RSP                             Response;
    EDDP_PRM_DETAIL_ERR_PTR_TYPE        pDetailErr;
    LSA_UINT32                          PortID, RedPortCnt;
    LSA_UINT32                          MinFSO;
    LSA_BOOL                            LongPreamble;
    EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE     pPDIRFrameData;

    EDDP_ASSERT_FALSE(!(pDDB->PRM.pRecordsetB->PDIRData_IsPresent));
    EDDP_ASSERT_FALSE(PDIRFrameDataIdx >= pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All);

    Response        = EDD_STS_OK;
    pDetailErr      = &pDDB->PrmDetailErr;
    MinFSO          = 0;
    LongPreamble    = LSA_FALSE;
    RedPortCnt      = 0;
    pPDIRFrameData  = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[PDIRFrameDataIdx];

    // only for IRT FrameID range
    if (EDDP_IS_IRT_FRAMEID(pPDIRFrameData->FrameID))
    {
        /* ---------------------------------------- */
        /* init MinFSO                              */
        /* ---------------------------------------- */
        switch (EDDP_HALGetPnipRevision_Ext(pDDB))
        {
            #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
            case EDDP_HAL_HW_PNIP_R1A1:
                MinFSO = EDDP_PRM_PDIRFRAMEDATA_FRAMESENDOFFSET_MIN_ERTEC200P_PNIP_REV1;
            break;

            case EDDP_HAL_HW_PNIP_R1F1:
                MinFSO = EDDP_PRM_PDIRFRAMEDATA_FRAMESENDOFFSET_MIN_FPGA_PNIP_REV1;
            break;
            #endif

            #ifdef EDDP_CFG_HW_PNIP_REV2_SUPPORT
            case EDDP_HAL_HW_PNIP_R2A1:
                MinFSO = EDDP_PRM_PDIRFRAMEDATA_FRAMESENDOFFSET_MIN_ERTEC200P_PNIP_REV2;
            break;

	        case EDDP_HAL_HW_PNIP_R2F1:
                MinFSO = EDDP_PRM_PDIRFRAMEDATA_FRAMESENDOFFSET_MIN_FPGA_PNIP_REV2;
            break;
            #endif

            default:
            {
                EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataCheckFrameSendOffset(): FATAL -> PnipRevision(%d) is unknown", 
                    EDDP_HALGetPnipRevision_Ext(pDDB));
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }
            break;
        }

        /* ---------------------------------------------------------------------------------------- */
        /* long or short Preamble ?                                                                 */
        /*  - short Preamble: ports with RED phase and PortDataAdjust.PreambleLength = 1            */
        /*  - else long Preamble                                                                    */
        /*  - no RED ports in PDIRData: long Preamble                                               */
        /* ---------------------------------------------------------------------------------------- */
        for ( PortID=1; PortID <= pDDB->SWI.PortCnt; PortID++ )
        {
            // following checks only if still short Preamble is set
            if ( !(LongPreamble) )
            {
                if (pDDB->PRM.pPDIRData_B->PDIRBeginEndData.BeginEndDataArray[PortID-1].isRedPort)
                {
                    RedPortCnt++;

                    // is PreambleLength present ?
                    if (pDDB->PRM.PortDataAdjust.Parameters[PortID].PreambleLength_IsPresent)
                    {
                        // PDPortDataAdjust record with "PreambleLength" for this port is present
                        if (pDDB->PRM.PortDataAdjust.Parameters[PortID].PreambleLength == 0)
                            LongPreamble = LSA_TRUE;    // long Preamble
                        else
                            LongPreamble = LSA_FALSE;   // short Preamble
                    }
                    else
                    {
                        // PDPortDataAdjust record with "PreambleLength" for this port is not present
                        LongPreamble = LSA_TRUE;    // long Preamble
                    }
                }
            }
        }

        if (RedPortCnt == 0)
        {
            // no RED ports in PDIRData! This should have been stated in PrmWrite!
            EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataCheckFrameSendOffset(): FATAL -> no RED ports in PDIRData!");
            EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
        }

        /* ---------------------------------------- */
        /* check FrameSendOffset                    */
        /* ---------------------------------------- */
        if (LongPreamble)
        {
            // long Preamble
            if (pPDIRFrameData->FrameSendOffset < MinFSO)
            {
                EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDIRDATA_RECORD_INDEX, pPDIRFrameData->OffsetFrameSendOffset, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                Response = EDD_STS_ERR_PRM_CONSISTENCY;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataCheckFrameSendOffset(PDIRFrameDataIdx=%d): ERROR -> FrameSendOffset(%d) < MinFSO(%d) for long Preamble", 
                    PDIRFrameDataIdx, pPDIRFrameData->FrameSendOffset, MinFSO);
            }
        }
        else
        {
            // short Preamble
            if (pPDIRFrameData->FrameSendOffset < (MinFSO - EDDP_PRM_PDIRFRAMEDATA_FRAMESENDOFFSET_SHORTENED_VALUE))
            {
                EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDIRDATA_RECORD_INDEX, pPDIRFrameData->OffsetFrameSendOffset, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
                Response = EDD_STS_ERR_PRM_CONSISTENCY;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_PRM_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataCheckFrameSendOffset(PDIRFrameDataIdx=%d): ERROR -> FrameSendOffset(%d) < MinFSO(%d)-%d for short Preamble", 
                    PDIRFrameDataIdx, pPDIRFrameData->FrameSendOffset, MinFSO, (EDDP_PRM_PDIRFRAMEDATA_FRAMESENDOFFSET_SHORTENED_VALUE));
            }
        }
    }

    return (Response);
}



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRDataCheckRR                                 +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                        +*/
/*+                             LSA_UINT16                  PDIRFrameDataIdx            +*/
/*+                                                                                     +*/
/*+  Result                :    EDD_RSP                                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  PDIRFrameDataIdx   : index in PDIRData.PDIRFrameData[x]                            +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks if the RR is in valid range                      +*/
/*+               of given PDIRFrameData entry.                                         +*/
/*+               The FrameSendOffset must be already copied to PDIRData_B.             +*/
/*+                                                                                     +*/
/*+               This check is only for IRT-FrameIDs, not for Sync-FrameIDs!           +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#if defined (EDDP_APPLY_IRTPU_RESTRICTIONS)
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataCheckRR(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PDIRFrameDataIdx)
{
    EDD_RSP                             Response;
    EDDP_PRM_DETAIL_ERR_PTR_TYPE        pDetailErr;
    EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE     pPDIRFrameData;

    EDDP_ASSERT_FALSE(!(pDDB->PRM.pRecordsetB->PDIRData_IsPresent));
    EDDP_ASSERT_FALSE(PDIRFrameDataIdx >= pDDB->PRM.pPDIRData_B->IRFrameDataArray_Count_All);

    Response        = EDD_STS_OK;
    pDetailErr      = &pDDB->PrmDetailErr;
    pPDIRFrameData  = &pDDB->PRM.pPDIRData_B->pIRFrameDataArray[PDIRFrameDataIdx];

    // only for IRT FrameID range
    if (EDDP_IS_IRT_FRAMEID(pPDIRFrameData->FrameID))
    {
        //Restriction for IRT-PU: RR != 1 and SCF<8 is not allowed
        if (   (pDDB->SWI.CycleInfo.CycleBaseFactor < 8)
            && (1 != pPDIRFrameData->ReductionRatio) )
        {
            EDDP_PRM_SET_DETAIL_ERR(pDetailErr, EDD_PRM_ERR_PORT_ID_DEFAULT, EDDP_PRM_PDIRDATA_RECORD_INDEX, pPDIRFrameData->ReductionRatio, EDD_PRM_ERR_FAULT_DEFAULT, LSA_TRUE);
            Response = EDD_STS_ERR_PRM_CONSISTENCY;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_PRM_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PRMPDIRDataCheckRR(PDIRFrameDataIdx=%d): ERROR -> RR(0x%X) != 1, SCF=(0x%X)", 
                PDIRFrameDataIdx, pPDIRFrameData->ReductionRatio, pDDB->SWI.CycleInfo.CycleBaseFactor);
        }
    }

    return (Response);
}
#endif //(EDDP_APPLY_IRTPU_RESTRICTIONS)



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRDataChekYellowTimeInValidRange              +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                        +*/
/*+                             LSA_UINT32                  YellowTime                  +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_BOOL                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                                     +*/
/*+  YellowTime : Yellowtime for checking                                               +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+  Description: This function checks the given YellowTime is in valid                 +*/
/*+               range.                                                                +*/
/*+               The check is dependent of PNIP revision.                              +*/
/*+                                                                                     +*/
/*+               Return LSA_TRUE  : YellowTime is in valid range                       +*/
/*+                      LSA_FALSE : Yellowtime is not in valid range                   +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataChekYellowTimeInValidRange(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
   	LSA_UINT32                  YellowTime)
{
    LSA_BOOL        State = LSA_FALSE;

    #ifdef EDDP_MESSAGE
    #ifdef EDDP_PRAGMA_MESSAGE
    #pragma EDDP_PRAGMA_MESSAGE("MinYellowTime is depending PNIP revision")
    #endif /* EDDP_PRAGMA_MESSAGE */
    #endif /* EDDP_MESSAGE */
    // switch to current PNIP revision
    switch (EDDP_HALGetPnipRevision(pDDB))
    {
        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
        case EDDP_HAL_HW_PNIP_REV1:
        {
            if ((YellowTime >= EDDP_PRM_PDIRGLOBALDATA_YELLOWTIME_MIN_PNIP_REV1) && (YellowTime <= EDDP_PRM_PDIRGLOBALDATA_YELLOWTIME_MAX))
            {
                State = LSA_TRUE;
            }
        }
        break;
        #endif

        #if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
        case EDDP_HAL_HW_PNIP_REV2:
        case EDDP_HAL_HW_PNIP_REV3:
        {
            if ((YellowTime >= EDDP_PRM_PDIRGLOBALDATA_YELLOWTIME_MIN_PNIP_REV2) && (YellowTime <= EDDP_PRM_PDIRGLOBALDATA_YELLOWTIME_MAX))
            {
                State = LSA_TRUE;
            }
        }
        break;
        #endif

        default:
        {
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataChekYellowTimeInValidRange(): FATAL -> Version(%d) of PNIP is unknown", 
                EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }

    return (State);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRDataGetLocalTransferEndNs                   +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                         pDDB    +*/
/*+                             EDDP_PRM_LOCAL_TRANSFER_START_END_MODE_TYPE     Mode    +*/
/*+                             LSA_BOOL										InActiveRecord_A+*/
/*+                                                                                     +*/
/*+  Result                :    LSA_UINT32                                              +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  Mode               : Mode - see defines                                            +*/
/*+  InActiveRecord_A   : LSA_TRUE  - search in active record buffer (A)                +*/
/*+                       LSA_FALSE - search in new record buffer (B)                   +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Get current value for local TransferEnd for given Mode.               +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataGetLocalTransferEndNs(
    EDDP_LOCAL_DDB_PTR_TYPE                         pDDB,
    EDDP_PRM_LOCAL_TRANSFER_START_END_MODE_TYPE     Mode,
    LSA_BOOL                                        InActiveRecord_A)
{
    LSA_UINT32                      TransferStartEndNs  = 0;
    EDDP_PRM_PDIRDATA_PTR_TYPE      pPDIRData           = LSA_NULL;

    if (InActiveRecord_A)
    {
        // search in active record buffer A - is PDIRData_A present ?
        if (pDDB->PRM.pRecordsetA->PDIRData_IsPresent)
            pPDIRData = pDDB->PRM.pPDIRData_A;
    }
    else
    {
        // search in new record buffer B - is PDIRData_B present ?
        if (pDDB->PRM.pRecordsetB->PDIRData_IsPresent)
            pPDIRData = pDDB->PRM.pPDIRData_B;
    }

    // get only if PDIRData is present
    if ( !(LSA_HOST_PTR_ARE_EQUAL(pPDIRData, LSA_NULL)) )
    {
        switch (Mode)
        {
            case EDDP_PRM_LOCAL_TRANSFER_START_END_MODE_RX_END:
                TransferStartEndNs = pPDIRData->LocalTransferStartEnd.RxLocalTransferEndNs;
            break;

            case EDDP_PRM_LOCAL_TRANSFER_START_END_MODE_TX_END:
                TransferStartEndNs = pPDIRData->LocalTransferStartEnd.TxLocalTransferEndNs;
            break;

            default:
            {
                EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataGetLocalTransferEndNs(): FATAL -> The given Mode(0x%X) is unknown!", 
                    Mode);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }

    return (TransferStartEndNs);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRDataGetLocalTransferStartNs                 +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                         pDDB    +*/
/*+                             EDDP_PRM_LOCAL_TRANSFER_START_END_MODE_TYPE     Mode    +*/
/*+                             LSA_BOOL                                        InActiveRecord_A+*/
/*+                                                                                     +*/
/*+  Result                :    LSA_UINT32                                              +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  Mode               : Mode - see defines                                            +*/
/*+  InActiveRecord_A   : LSA_TRUE  - search in active record buffer (A)                +*/
/*+                       LSA_FALSE - search in new record buffer (B)                   +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Get current value for local TransferStart for given Mode.             +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_INT32 EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataGetLocalTransferStartNs(
    EDDP_LOCAL_DDB_PTR_TYPE                         pDDB,
    EDDP_PRM_LOCAL_TRANSFER_START_END_MODE_TYPE     Mode,
    LSA_BOOL                                        InActiveRecord_A)
{
    LSA_INT32                       TransferStartEndNs  = 0;
    EDDP_PRM_PDIRDATA_PTR_TYPE      pPDIRData           = LSA_NULL;

    if (InActiveRecord_A)
    {
        // search in active record buffer A - is PDIRData_A present ?
        if (pDDB->PRM.pRecordsetA->PDIRData_IsPresent)
            pPDIRData = pDDB->PRM.pPDIRData_A;
    }
    else
    {
        // search in new record buffer B - is PDIRData_B present ?
        if (pDDB->PRM.pRecordsetB->PDIRData_IsPresent)
            pPDIRData = pDDB->PRM.pPDIRData_B;
    }

    // get only if PDIRData is present
    if ( !(LSA_HOST_PTR_ARE_EQUAL(pPDIRData, LSA_NULL)) )
    {
        switch (Mode)
        {
            case EDDP_PRM_LOCAL_TRANSFER_START_END_MODE_TX_START:
                TransferStartEndNs = pPDIRData->LocalTransferStartEnd.TxLocalTransferStartNs;
            break;

            default:
            {
                EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataGetLocalTransferStartNs(): FATAL -> The given Mode(0x%X) is unknown!", 
                    Mode);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }

    return (TransferStartEndNs);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname   :   EDDP_PRMPDIRDataCalculateLocalRxApplicationDelay                +*/
/*+                                                                                     +*/
/*+  Input/Output   :   EDDP_LOCAL_DDB_PTR_TYPE     pDDB                                +*/
/*+                     LSA_BOOL                    isMRPDConsOddPresent                +*/
/*+                     LSA_BOOL                    InActiveRecord_A                    +*/
/*+                                                                                     +*/
/*+  Result         :   LSA_UINT32                                                      +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB                   : Pointer to DeviceDescriptionBlock                         +*/
/*+  isMRPDConsOddPresent   : is MRPD consumer with odd FrameID(x+1) ?                  +*/
/*+  InActiveRecord_A       : LSA_TRUE  - search in active record buffer (A)            +*/
/*+                           LSA_FALSE - search in new record buffer (B)               +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function calculates the LocalRxApplicationDelay.                 +*/
/*+               Take values from record PDIRApplicationData if parametrized else      +*/
/*+               the worst case is used.                                               +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataCalculateLocalRxApplicationDelay(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_BOOL                    isMRPDConsOddPresent,
    LSA_BOOL                    InActiveRecord_A)
{
    LSA_UINT32 LocalRxApplicationDelay = 0;    

    /* ------------------------------------------------------------------------ */
    /* calculate the LocalRxApplicationDelay dependent of PNIP revision         */
    /* ------------------------------------------------------------------------ */
    switch (EDDP_HALGetPnipRevision_Ext(pDDB))
    {
        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
        case EDDP_HAL_HW_PNIP_R1A1:
        case EDDP_HAL_HW_PNIP_R1F1:  
        {
            LSA_UINT16 ConsumerCnt = 0;
            LSA_UINT16 ConsumerIODataLength = 0;
            LSA_UINT32 ValueTemp_32 = 0;
            EDDP_PRM_PDIRAPPLICATIONDATA_PTR_TYPE pPDIRApplicationData = LSA_NULL;
            
            /* ------------------------------------------------------------------------ */
            /* init pPDIRApplicationData                                                */
            /* ------------------------------------------------------------------------ */
            if (InActiveRecord_A)
            {
                // search in active record buffer A - is PDIRApplicationData_A present ?
                if (pDDB->PRM.pRecordsetA->PDIRApplicationData_IsPresent)
                    pPDIRApplicationData = pDDB->PRM.pPDIRApplicationData_A;
            }
            else
            {
                // search in active record buffer B - is PDIRApplicationData_B present ?
                if (pDDB->PRM.pRecordsetB->PDIRApplicationData_IsPresent)
                    pPDIRApplicationData = pDDB->PRM.pPDIRApplicationData_B;
            }
            
            // Check if PDIRApplicationData is present
            if (LSA_HOST_PTR_ARE_EQUAL(pPDIRApplicationData, LSA_NULL))
            {
                // no PDIRApplicationData -> worst case
                ConsumerCnt             = EDDP_CRTGetConsumerMaxNumber (pDDB);
                ConsumerIODataLength    = EDDP_PRM_PDIRAPPLICATIONDATA_MAX_CONSUMER_IODATA_LENGTH;
            }
            else
            {
                ConsumerCnt             = pPDIRApplicationData->ConsumerCnt;
                ConsumerIODataLength    = pPDIRApplicationData->ConsumerIODataLength;

                if (ConsumerCnt == 0)
                {
                    // no entry for ConsumerCnt -> worst case
                    ConsumerCnt = EDDP_CRTGetConsumerMaxNumber (pDDB);
                }
            }

            if (isMRPDConsOddPresent)
            {
                /* ---------------------------- */
                /* with MRPD                    */
                /* ---------------------------- */
                // at least one MRPD consumer with odd FrameID(x+1)
                if ((ConsumerIODataLength > 0) && (ConsumerIODataLength <= EDDP_PRM_PDIRAPPLICATIONDATA_MAX_CONSUMER_IODATA_LENGTH))
                {
                    /* ------------------------------------------------------------------------ */
                    /* with MRPD and IODataLength[1..1440]                                      */
                    /* ------------------------------------------------------------------------ */
                    // LocalRxApplicationDelay = MAX(ConsumerIODataLength * 22,75 ns + (ConsumerCnt * 40 ns + 2424 ns); 3500 ns)

                    ValueTemp_32 = ConsumerIODataLength * 2275;

                    if (ValueTemp_32 % 100 != 0)
                    {
                        // round up
                        ValueTemp_32 = ((LSA_UINT32)(ValueTemp_32 / 100)) + 1;
                    }
                    else
                    {
                        ValueTemp_32 = (LSA_UINT32)(ValueTemp_32 / 100);
                    }

                    LocalRxApplicationDelay = ValueTemp_32 + (ConsumerCnt * 40 + 2424);

                    if (LocalRxApplicationDelay < 3500)
                    {
                        LocalRxApplicationDelay = 3500;
                    }
                }
                else
                {
                    /* ------------------------------------------------------------------------ */
                    /* with MRPD and ((IODataLength = 0) or (IODataLength > 1440))              */
                    /* -> worst case                                                            */
                    /* ------------------------------------------------------------------------ */
                    LocalRxApplicationDelay = 32760 + (ConsumerCnt * 40 + 2424);
                }
            }
            else
            {
                /* ---------------------------- */
                /* without MRPD                 */
                /* ---------------------------- */
                if ((ConsumerIODataLength > 0) && (ConsumerIODataLength <= EDDP_PRM_PDIRAPPLICATIONDATA_MAX_CONSUMER_IODATA_LENGTH))
                {
                    /* ------------------------------------------------------------------------ */
                    /* without MRPD and IODataLength[1..1440]                                   */
                    /* ------------------------------------------------------------------------ */
                    // LocalRxApplicationDelay = MAX(ConsumerIODataLength * 13,25 ns + (ConsumerCnt * 80 ns + 98 ns); 2500 ns)

                    ValueTemp_32 = ConsumerIODataLength * 1325 ;

                    if (ValueTemp_32 % 100 != 0)
                    {
                        // round up
                        ValueTemp_32 = ((LSA_UINT32)(ValueTemp_32 / 100)) + 1;
                    }
                    else
                    {
                        ValueTemp_32 = (LSA_UINT32)(ValueTemp_32 / 100);
                    }

                    LocalRxApplicationDelay = (ValueTemp_32 + (ConsumerCnt * 80 + 98));

                    if (LocalRxApplicationDelay < 2500)
                    {
                        LocalRxApplicationDelay = 2500;
                    }
                }
                else
                {
                    /* ------------------------------------------------------------------------ */
                    /* without MRPD and ((IODataLength = 0) or (IODataLength > 1440))           */
                    /* -> worst case                                                            */
                    /* ------------------------------------------------------------------------ */
                    LocalRxApplicationDelay = 19080 + (ConsumerCnt * 80 + 98);
                }
            }
        }
        break;
        #endif

        #ifdef EDDP_CFG_HW_PNIP_REV2_SUPPORT
        case EDDP_HAL_HW_PNIP_R2A1:
		{
            LocalRxApplicationDelay = EDDP_CRT_CPM_TRANSFEREND_DELAY_ASIC_REV2;
		}
		break;
        case EDDP_HAL_HW_PNIP_R2F1:
        {
            LocalRxApplicationDelay = EDDP_CRT_CPM_TRANSFEREND_DELAY_FPGA_REV2;
        }
        break;
        #endif
                                                
        default:
        {
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataCalculateLocalRxApplicationDelay(): FATAL -> PnipRevision(%d) is unknown", 
                EDDP_HALGetPnipRevision_Ext(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }

    return (LocalRxApplicationDelay);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname   :    EDDP_PRMPDIRDataCalculateLocalTransferStartEndIntermediateValues+*/
/*+                                                                                     +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB                            +*/
/*+                      LSA_UINT16                     PDIRFrameDataIdx                +*/
/*+                      LSA_BOOL                       InActiveRecord_A                +*/
/*+                                                                                     +*/
/*+  Result         :    LSA_VOID                                                       +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  PDIRFrameDataIdx   : index in PDIRData.PDIRFrameData[x]                            +*/
/*+  InActiveRecord_A   : LSA_TRUE  - search in active record buffer (A)                +*/
/*+                       LSA_FALSE - search in new record buffer (B)                   +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function calulates the intermediate values for the calculation   +*/
/*+               of TransferStart/End values for a given PDIRFrameData entry.          +*/
/*+                                                                                     +*/
/*+               Call this function always after init of "SndRcvRole"!                 +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataCalculateLocalTransferStartEndIntermediateValues(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  PDIRFrameDataIdx,
    LSA_BOOL                    InActiveRecord_A)
{
    LSA_UINT16                          FrameLength;
    LSA_UINT32                          EndTimeNewNs;
    EDDP_PRM_PDIRDATA_PTR_TYPE          pPDIRData;
    EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE     pPDIRFrameData;

    pPDIRData       = LSA_NULL;
    pPDIRFrameData  = LSA_NULL;

    // is PDIRData present ?
    if (EDDP_PRMPDIRDataIsPDIRDataPresent(pDDB, InActiveRecord_A))
    {
        if (InActiveRecord_A)
        {
            // search in active record buffer A - is PDIRData_A present ?
            if (pDDB->PRM.pRecordsetA->PDIRData_IsPresent)
                pPDIRData = pDDB->PRM.pPDIRData_A;
        }
        else
        {
            // search in new record buffer B - is PDIRData_B present ?
            if (pDDB->PRM.pRecordsetB->PDIRData_IsPresent)
                pPDIRData = pDDB->PRM.pPDIRData_B;
        }

        if (LSA_HOST_PTR_ARE_EQUAL(pPDIRData, LSA_NULL))
        {
            // PDIRData is not present!
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataCalculateLocalTransferStartEndIntermediateValues(InActiveRecord_A=0x%X): FATAL -> PDIRData is not present!", 
                InActiveRecord_A);
            EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
        }
        else
        {
            EDDP_ASSERT_FALSE(PDIRFrameDataIdx >= pPDIRData->IRFrameDataArray_Count_All);
            pPDIRFrameData = &pPDIRData->pIRFrameDataArray[PDIRFrameDataIdx];

            // only for IRT FrameID range
            if (EDDP_IS_IRT_FRAMEID(pPDIRFrameData->FrameID))
            {
                // local sent/received frame (Provider or Consumer)
                switch (pPDIRFrameData->SndRcvRole)
                {
                    /* -------------------------------- */
                    /* Provider - Tx                    */
                    /* -------------------------------- */
                    case EDDP_PRM_PDIRFRAMEDATA_ROLE_PROVIDER:
                    {
                        FrameLength     = pPDIRFrameData->DataLength + EDDP_CRT_RT_FRAME_LEN_HEADER_APDU_FCS;
                        EndTimeNewNs    = pPDIRFrameData->FrameSendOffset + (FrameLength * EDDP_TIME_BYTE_RESOLUTION_RT_FRAME_NS);
                            
                        // TxMinFSONs
                        if (   (pDDB->PRM.pPDIRData_B->LocalTransferStartEnd.TxMinFSONs == 0)
                            || (pPDIRFrameData->FrameSendOffset < pDDB->PRM.pPDIRData_B->LocalTransferStartEnd.TxMinFSONs)
                           )
                        {
                            pDDB->PRM.pPDIRData_B->LocalTransferStartEnd.TxMinFSONs = pPDIRFrameData->FrameSendOffset;
                        }

                        // TxMaxFSOWithFrameLengthNs
                        if (EndTimeNewNs > pDDB->PRM.pPDIRData_B->LocalTransferStartEnd.TxMaxFSOWithFrameLengthNs)
                        {
                            pDDB->PRM.pPDIRData_B->LocalTransferStartEnd.TxMaxFSOWithFrameLengthNs = EndTimeNewNs;
                        }
                    }
                    break;

                    /* -------------------------------- */
                    /* Consumer - Rx                    */
                    /* -------------------------------- */
                    case EDDP_PRM_PDIRFRAMEDATA_ROLE_CONSUMER:
                    case EDDP_PRM_PDIRFRAMEDATA_ROLE_CONSUMER_FORWARDER:
                    {
                        FrameLength     = pPDIRFrameData->DataLength + EDDP_CRT_RT_FRAME_LEN_HEADER_APDU_FCS;
                        EndTimeNewNs    = pPDIRFrameData->FrameSendOffset + (FrameLength * EDDP_TIME_BYTE_RESOLUTION_RT_FRAME_NS);

                        // RxMaxFSOWithFrameLengthNs
                        if (EndTimeNewNs > pDDB->PRM.pPDIRData_B->LocalTransferStartEnd.RxMaxFSOWithFrameLengthNs)
                        {
                            pDDB->PRM.pPDIRData_B->LocalTransferStartEnd.RxMaxFSOWithFrameLengthNs = EndTimeNewNs;
                        }
                    }
                    break;

                    /* -------------------------------- */
                    /* Forwarder                        */
                    /* -------------------------------- */
                    case EDDP_PRM_PDIRFRAMEDATA_ROLE_FORWARDER:
                    {
                        // nothing to do
                    }
                    break;

                    default:
                    {
                        EDDP_SWI_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataCalculateLocalTransferStartEndIntermediateValues(PDIRFrameDataIdx=%d, InActiveRecord_A=0x%X): FATAL -> SndRcvRole(%d) is unknown!", 
                            PDIRFrameDataIdx, InActiveRecord_A, pPDIRFrameData->SndRcvRole);
                        EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                    }
                    break;
                }
            }
        }
    }
    else
    {
        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataCalculateLocalTransferStartEndIntermediateValues(InActiveRecord_A=0x%X): FATAL -> The record PDIRData is not present!", 
            InActiveRecord_A);
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRDataCalculateLocalTransferStartEndNs        +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                        +*/
/*+                             LSA_BOOL                    InActiveRecord_A            +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
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
/*+  Description: This function calculates the parameters:                              +*/
/*+                 - LocalTransferStartEnd.RxLocalTransferEndNs                        +*/
/*+                 - LocalTransferStartEnd.TxLocalTransferStartNs                      +*/
/*+                 - LocalTransferStartEnd.TxLocalTransferEndNs                        +*/
/*+                                                                                     +*/
/*+               Call this function on PrmEnd.                                         +*/
/*+               Both records PDIRData and PDIRApplicationData are needed.             +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataCalculateLocalTransferStartEndNs(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB, 
    LSA_BOOL                    InActiveRecord_A)
{
    LSA_UINT32                  TxLocalApplicationFeed;
    EDDP_PRM_PDIRDATA_PTR_TYPE  pPDIRData = LSA_NULL;

    // only if PDIRData record is present
    if (EDDP_PRMPDIRDataIsPDIRDataPresent(pDDB, InActiveRecord_A))
    {
        if (InActiveRecord_A)
        {
            // search in active record buffer A - is PDIRData_A present ?
            if (pDDB->PRM.pRecordsetA->PDIRData_IsPresent)
                pPDIRData = pDDB->PRM.pPDIRData_A;
        }
        else
        {
            // search in new record buffer B - is PDIRData_B present ?
            if (pDDB->PRM.pRecordsetB->PDIRData_IsPresent)
                pPDIRData = pDDB->PRM.pPDIRData_B;
        }

        if (LSA_HOST_PTR_ARE_EQUAL(pPDIRData, LSA_NULL))
        {
            // PDIRData is not present!
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataCalculateLocalTransferStartEndNs(InActiveRecord_A=0x%X): FATAL -> PDIRData is not present!", 
                InActiveRecord_A);
            EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
        }
        else
        {
            // delete values
            pPDIRData->LocalTransferStartEnd.RxLocalTransferEndNs   = 0;
            pPDIRData->LocalTransferStartEnd.TxLocalTransferStartNs = 0;
            pPDIRData->LocalTransferStartEnd.TxLocalTransferEndNs   = 0;

            // and calculate new values

            /* ------------------------------------------------------------------------------------------------------------ */
            /* RxLocalTransferEndNs = MAX(FSO_Rx + Framelength_Rx * 80) – MaxBridgeDelay + LocalRxApplicationDelay          */
            /* ------------------------------------------------------------------------------------------------------------ */
            // RxMaxFSOWithFrameLengthNs = 0: if no local consumer are present in PDIRData
            if (pPDIRData->LocalTransferStartEnd.RxMaxFSOWithFrameLengthNs > 0)
            {
                pPDIRData->LocalTransferStartEnd.RxLocalTransferEndNs = (  pPDIRData->LocalTransferStartEnd.RxMaxFSOWithFrameLengthNs 
                                                                         - pPDIRData->PDIRGlobalData.MaxBridgeDelay 
                                                                         + (EDDP_PRMPDIRDataCalculateLocalRxApplicationDelay(pDDB, pPDIRData->MRPDConsumerWithOddFrameID_IsPresent, LSA_FALSE))
                                                                        );
            }

            /* ------------------------------------------------------------------------------------------------------------ */
            /* TxLocalTransferStartNs = MIN(FSO_Tx) – Tx.LocalApplicationFeed (FSO_Leadtime or Fetchtime_Delta)             */
            /* ------------------------------------------------------------------------------------------------------------ */
            // TxMinFSONs = 0: if no local provider are present in PDIRData
            if (pPDIRData->LocalTransferStartEnd.TxMinFSONs > 0)
            {                
                TxLocalApplicationFeed = EDDP_CRTGetFSOLeadtimeOrFetchtimeDelta(pDDB);
                
                /* TxLocalTransferStart can not be negativ for ERTEC200P */
                if(EDDP_HAL_HW_ERTEC200P == EDDP_HALGetChipVersion(pDDB))
                {
                    if(pPDIRData->LocalTransferStartEnd.TxMinFSONs < TxLocalApplicationFeed)
                    {
                        EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataCalculateLocalTransferStartEndNs(): FATAL -> TxLocalTransferStart can not be negative for ERTEC200P! TxMinFSONs=0x%X, TxLocalApplicationFeed=0x%X", 
                                pPDIRData->LocalTransferStartEnd.TxMinFSONs, TxLocalApplicationFeed);
                        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
                    }
                }
                
                /* check upper LSA_INT32 limit */
                if(pPDIRData->LocalTransferStartEnd.TxMinFSONs > TxLocalApplicationFeed)
                {
                   if(EDDP_PRM_LIMIT_LSA_INT32_MAX < (pPDIRData->LocalTransferStartEnd.TxMinFSONs - TxLocalApplicationFeed))
                   {
                       EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataCalculateLocalTransferStartEndNs(): FATAL -> TxLocalTransferStart exceeds upper LSA_INT32 limit");
                       EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
                   }
                }
                
                pPDIRData->LocalTransferStartEnd.TxLocalTransferStartNs = (LSA_INT32)(pPDIRData->LocalTransferStartEnd.TxMinFSONs - TxLocalApplicationFeed);
            }

            /* ------------------------------------------------------------------------------------------------------------ */
            /* TxLocalTransferEndNs = MAX(FSO_Tx + FrameLength_Tx*80)                                                       */
            /* ------------------------------------------------------------------------------------------------------------ */
            // TxMaxFSOWithFrameLengthNs = 0: if no local provider are present in PDIRData
            pPDIRData->LocalTransferStartEnd.TxLocalTransferEndNs = pPDIRData->LocalTransferStartEnd.TxMaxFSOWithFrameLengthNs;
        }
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMPDIRDataSetupRsvIntervalRedValues               +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                +*/
/*+                             EDD_RESERVED_INTERVAL_IF_RED_TYPE   *pRsvIntervalRed    +*/
/*+                             LSA_BOOL                            isValid             +*/
/*+                             LSA_BOOL                            InActiveRecord_A    +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  pRsvIntervalRed    : address of struct EDD_RESERVED_INTERVAL_IF_RED_TYPE           +*/
/*+  isValid            : set values to valid else delete it                            +*/
/*+  InActiveRecord_A   : LSA_TRUE  - search in active record buffer (A)                +*/
/*+                       LSA_FALSE - search in new record buffer (B)                   +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function sets the values of RsvIntervalRed.                      +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataSetupRsvIntervalRedValues(
    EDDP_LOCAL_DDB_PTR_TYPE             pDDB,
    EDD_RESERVED_INTERVAL_IF_RED_TYPE   *pRsvIntervalRed,
    LSA_BOOL                            isValid,
    LSA_BOOL                            InActiveRecord_A)
{
    EDDP_ASSERT_FALSE((LSA_HOST_PTR_ARE_EQUAL(pRsvIntervalRed, LSA_NULL)));

    if (isValid)
    {
        /* -------------------------------------------------------- */
        /* set RsvIntervalRed values to valid                       */
        /* -------------------------------------------------------- */
        pRsvIntervalRed->RxEndNs                = EDDP_PRMPDIRDataGetEndOfRedPhaseNsIF    (pDDB, EDDP_PRM_RSV_INTERVAL_RED_MODE_RX,               InActiveRecord_A);
        pRsvIntervalRed->RxLocalTransferEndNs   = EDDP_PRMPDIRDataGetLocalTransferEndNs   (pDDB, EDDP_PRM_LOCAL_TRANSFER_START_END_MODE_RX_END,   InActiveRecord_A);
        pRsvIntervalRed->TxEndNs                = EDDP_PRMPDIRDataGetEndOfRedPhaseNsIF    (pDDB, EDDP_PRM_RSV_INTERVAL_RED_MODE_TX,               InActiveRecord_A);
        pRsvIntervalRed->TxLocalTransferStartNs = EDDP_PRMPDIRDataGetLocalTransferStartNs (pDDB, EDDP_PRM_LOCAL_TRANSFER_START_END_MODE_TX_START, InActiveRecord_A);
        pRsvIntervalRed->TxLocalTransferEndNs   = EDDP_PRMPDIRDataGetLocalTransferEndNs   (pDDB, EDDP_PRM_LOCAL_TRANSFER_START_END_MODE_TX_END,   InActiveRecord_A);
    }
    else
    {
        /* -------------------------------------------------------- */
        /* delete RsvIntervalRed values                             */
        /* -------------------------------------------------------- */
        pRsvIntervalRed->RxEndNs                = 0;
        pRsvIntervalRed->RxLocalTransferEndNs   = 0;
        pRsvIntervalRed->TxEndNs                = 0;
        pRsvIntervalRed->TxLocalTransferStartNs = 0;
        pRsvIntervalRed->TxLocalTransferEndNs   = 0;
    }

    EDDP_PRM_TRACE_08(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] EDDP_PRMPDIRDataSetupRsvIntervalRedValues(isValid=0x%X, InActiveRecord_A=0x%X): SendClockNs=%d, RxEndNs=%d, RxLocalTransferEndNs=%d, TxEndNs=%d, TxLocalTransferStartNs=%d, TxLocalTransferEndNs=%d", 
        isValid, InActiveRecord_A, 
        (pDDB->SWI.CycleInfo.CycleBaseFactor * EDDP_CRT_CYCLE_LENGTH_BASE_NS), 
        pRsvIntervalRed->RxEndNs, 
        pRsvIntervalRed->RxLocalTransferEndNs, 
        pRsvIntervalRed->TxEndNs, 
        pRsvIntervalRed->TxLocalTransferStartNs, 
        pRsvIntervalRed->TxLocalTransferEndNs);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname         :    EDDP_PRMPDIRDataIsFrameIDPresent                         +*/
/*+                                                                                     +*/
/*+  Input/Output         :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                     +*/
/*+                            LSA_UINT16                      FrameID                  +*/
/*+                            LSA_BOOL                        isCrtProv                +*/
/*+                            EDD_COMMON_MEM_U16_PTR_TYPE     pCycleReductionRatio     +*/
/*+                            EDD_COMMON_MEM_U16_PTR_TYPE     pCyclePhase              +*/
/*+                            LSA_BOOL                        InActiveRecord_A         +*/
/*+                                                                                     +*/
/*+  Result               :    LSA_BOOL                                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB                   : Pointer to DeviceDescriptionBlock                         +*/
/*+  FrameID                : IRT FrameID                                               +*/
/*+  isCrtProv              : LSA_TRUE  - CRT call from provider                        +*/
/*+                           LSA_FALSE - CRT call from consumer                        +*/
/*+  pCycleReductionRatio   : Pointer to ReductionRatio                                 +*/
/*+  pCyclePhase            : Pointer to Phase                                          +*/
/*+  InActiveRecord_A       : LSA_TRUE  - search in active record buffer (A)            +*/
/*+                           LSA_FALSE - search in new record buffer (B)               +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks whether the given FrameID (with the expected     +*/
/*+               role) is present in PDIRFrameData . If the FrameID is found,          +*/
/*+               some additional information is collected from PDIRFrameData           +*/
/*+               (ReductionRatio and Phase).                                           +*/
/*+                                                                                     +*/
/*+               The given FrameID must be in IRT FrameID range. If not, no check!     +*/
/*+                                                                                     +*/
/*+               Return LSA_TRUE  : FrameID is present in PDIRFrameData                +*/
/*+                      LSA_FALSE : FrameID is not resent in PDIRFrameData             +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_PRMPDIRDataIsFrameIDPresent(
    EDDP_LOCAL_DDB_PTR_TYPE                          pDDB,
    LSA_UINT16                                       FrameID,
    LSA_BOOL                                         isCrtProv,
    EDD_COMMON_MEM_U16_PTR_TYPE                      pCycleReductionRatio,
    EDD_COMMON_MEM_U16_PTR_TYPE                      pCyclePhase,
    LSA_BOOL                                         InActiveRecord_A)
{
    LSA_BOOL                        State;
    LSA_UINT32                      i;
    EDDP_PRM_PDIRDATA_PTR_TYPE      pPDIRData;

    State       = LSA_FALSE;
    pPDIRData   = LSA_NULL;

    // only for IRT FrameID range
    if (EDDP_IS_IRT_FRAMEID(FrameID))
    {
        if (InActiveRecord_A)
        {
            // search in active record buffer A - is PDIRData_A present?
            if (pDDB->PRM.pRecordsetA->PDIRData_IsPresent)
                pPDIRData = pDDB->PRM.pPDIRData_A;
        }
        else
        {
            // search in new record buffer B - is PDIRData_B present?
            if (pDDB->PRM.pRecordsetB->PDIRData_IsPresent)
                pPDIRData = pDDB->PRM.pPDIRData_B;
        }

        if (LSA_HOST_PTR_ARE_EQUAL(pPDIRData, LSA_NULL))
        {
            // PDIRData is not present!
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMPDIRDataIsFrameIDPresent(InActiveRecord_A=0x%X): FATAL -> PDIRData is not present!", 
                InActiveRecord_A);
            EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
        }
        else
        {
            for ( i=0; i < pPDIRData->IRFrameDataArray_Count_All; i++ )
            {
                if (isCrtProv)
                {
                    // search the given FrameID in PDIRFrameData as provider
                    if (   (pPDIRData->pIRFrameDataArray[i].FrameID == FrameID)
                        && (pPDIRData->pIRFrameDataArray[i].SndRcvRole == EDDP_PRM_PDIRFRAMEDATA_ROLE_PROVIDER)
                       )
                    {
                        // collect information for this FrameID
                        *pCycleReductionRatio = pPDIRData->pIRFrameDataArray[i].ReductionRatio;
                        *pCyclePhase          = pPDIRData->pIRFrameDataArray[i].Phase;

                        State   = LSA_TRUE;
                        i       = pPDIRData->IRFrameDataArray_Count_All;  // exit FOR loop
                    }
                }
                else
                {
                    // search the given FrameID in PDIRFrameData as consumer
                    if (   (pPDIRData->pIRFrameDataArray[i].FrameID == FrameID)
                        && (pPDIRData->pIRFrameDataArray[i].SndRcvRole != EDDP_PRM_PDIRFRAMEDATA_ROLE_PROVIDER)
                       )
                    {
                        // collect information for this FrameID
                        *pCycleReductionRatio = pPDIRData->pIRFrameDataArray[i].ReductionRatio;
                        *pCyclePhase          = pPDIRData->pIRFrameDataArray[i].Phase;

                        State   = LSA_TRUE;
                        i       = pPDIRData->IRFrameDataArray_Count_All;  // exit FOR loop
                    }
                }
            }
        }
    }

    return (State);
}

/*****************************************************************************/
/*  end of file eddp_prm_pdirdata.c                                          */
/*****************************************************************************/
