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
/*  C o m p o n e n t     &C: POF (Polymer Optical Fiber)               :C&  */
/*                                                                           */
/*  F i l e               &F: pof_prm_check.c                           :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  POF PRM Check                                    */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
/*****************************************************************************/

/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/
#define LTRC_ACT_MODUL_ID  7
#define POF_MODULE_ID      LTRC_ACT_MODUL_ID

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/
#include "pof_inc.h"            /* POF headerfiles */
#include "pof_dmi_type.h"
#include "pof_prm_type.h"
#include "pof_int.h"            /* internal header */
#include "pof_prm_req.h"
#include "pof_glb.h"
#include "pof_prm_check.h"

POF_FILE_SYSTEM_EXTENSION(POF_MODULE_ID)

/*===========================================================================*/
/*                                define                                     */
/*===========================================================================*/

/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/

/*===========================================================================*/
/*                                global Variables                           */
/*===========================================================================*/

/*===========================================================================*/
/*                            local function declaration                     */
/*===========================================================================*/

/*===========================================================================*/
/*                                 Macros                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/

/*===========================================================================*/
/*                                 Macros                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/

static POF_RSP  POF_LOCAL_FCT_ATTR  POF_CheckPrmRecord_Data_Read_Length( POF_UPPER_MEM_U32_PTR_TYPE  const  pRecordDataLength,
                                                                         LSA_UINT32                  const  RecordLenghtRequired )
{
    if (*pRecordDataLength < RecordLenghtRequired)
    {
        POF_SET_DETAIL_ERR(POF_ERR_PRM_CHECK_RECORD_DATA_LENGTH);
        return POF_RSP_ERR_PRM_DATA; 
    } 

    *pRecordDataLength = RecordLenghtRequired;
    return POF_RSP_OK;
}

/*=============================================================================
 * function name:   POF_CheckPrmRecord_Data_Read()
 *
 * parameters:      
 *
 * return value:    POF_RSP
 *
 * description :    
 *===========================================================================*/
POF_RSP  POF_LOCAL_FCT_ATTR  POF_CheckPrmRecord_Data_Read( POF_HDB_PTR_TYPE            const  pHDB,
                                                           LSA_UINT32                  const  RecordIndex,
                                                           POF_UPPER_MEM_U32_PTR_TYPE  const  pRecordDataLength )
{
    POF_RSP  Response;

    //check RecordIndex and RecordDataLength
    switch (RecordIndex)
    {
        case POF_PRM_FO_DATA_REAL_INDEX:
        {
            Response = POF_CheckPrmRecord_Data_Read_Length(pRecordDataLength, POF_PRM_FO_DATA_REAL_LENGTH);
        } break;

        case POF_PRM_FO_DATA_CHECK_INDEX:
        {
            Response = POF_CheckPrmRecord_Data_Read_Length(pRecordDataLength, POF_PRM_FO_DATA_CHECK_LENGTH);
        } break;

        case POF_PRM_FO_DATA_ADJUST_INDEX:
        {
            Response = POF_CheckPrmRecord_Data_Read_Length(pRecordDataLength, POF_PRM_FO_DATA_ADJUST_LENGTH);
        } break;

        case POF_PRM_SFP_DATA_CHECK_INDEX:
        {
            Response = POF_CheckPrmRecord_Data_Read_Length(pRecordDataLength, POF_PRM_SFP_DATA_CHECK_LENGTH);
        } break;

        case POF_PRM_SFP_DIAG_TEST_INDEX:
        {
            Response = POF_CheckPrmRecord_Data_Read_Length(pRecordDataLength, POF_PRM_SFP_DIAG_TEST_LENGTH);
        } break;

        case POF_PRM_FO_DATA_TRA_IND_TEST_INDEX:
        {
            Response = POF_CheckPrmRecord_Data_Read_Length(pRecordDataLength, POF_PRM_FO_DATA_TRA_IND_TEST_LENGTH);
        } break;

        default: //wrong RecordIndex
        {
            POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_CheckPrmRecord_Data_Read, wrong RecordIndex:0x%X", RecordIndex);
            LSA_UNUSED_ARG(pHDB); //satisfy lint!
            Response = POF_RSP_ERR_PRM_INDEX; //wrong RecordIndex
        }
    }
    return Response;
}
/*---------------------- end [subroutine] ---------------------------------*/


static POF_RSP  POF_LOCAL_FCT_ATTR  POF_CheckPrmRecord_Data_Write_Length( LSA_UINT32        const  RecordDataLength,
                                                                          LSA_UINT32        const  RecordLengthRequired )
{
    if (RecordDataLength != RecordLengthRequired)
    {
        POF_SET_DETAIL_ERR(POF_ERR_PRM_CHECK_RECORD_DATA_LENGTH);
        return POF_RSP_ERR_PRM_DATA; 
    }
    return POF_RSP_OK;
}

/*=============================================================================
 * function name:   POF_CheckPrmRecord_Data_Write()
 *
 * parameters:      
 *
 * return value:    POF_RSP
 *
 * description :    
 *===========================================================================*/
POF_RSP  POF_LOCAL_FCT_ATTR  POF_CheckPrmRecord_Data_Write( POF_HDB_PTR_TYPE  const  pHDB,
                                                            LSA_UINT32        const  RecordIndex,
                                                            LSA_UINT32        const  RecordDataLength )
{
    POF_RSP  Response;

    //check RecordIndex and RecordDataLength
    switch (RecordIndex)
    {
        case POF_PRM_FO_DATA_CHECK_INDEX:
        {
            Response = POF_CheckPrmRecord_Data_Write_Length(RecordDataLength, POF_PRM_FO_DATA_CHECK_LENGTH);
        } break;

        case POF_PRM_FO_DATA_ADJUST_INDEX:
        {
            Response = POF_CheckPrmRecord_Data_Write_Length(RecordDataLength, POF_PRM_FO_DATA_ADJUST_LENGTH);
        } break;

        case POF_PRM_SFP_DATA_CHECK_INDEX:
        {
            Response = POF_CheckPrmRecord_Data_Write_Length(RecordDataLength, POF_PRM_SFP_DATA_CHECK_LENGTH);
        } break;

        case POF_PRM_SFP_DIAG_TEST_INDEX:
        {
            Response = POF_CheckPrmRecord_Data_Write_Length(RecordDataLength, POF_PRM_SFP_DIAG_TEST_LENGTH);
        } break;

        default: //wrong RecordIndex
        {
          POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_CheckPrmRecord_Data_Write, wrong RecordIndex:0x%X", RecordIndex);
          LSA_UNUSED_ARG(pHDB); //satisfy lint!
          return POF_RSP_ERR_PRM_INDEX; //wrong RecordIndex
        }
    }
    return Response;
}
/*---------------------- end [subroutine] ---------------------------------*/


static POF_RSP  POF_LOCAL_FCT_ATTR  POF_CheckPrmHeaderSpecific( POF_HDB_PTR_TYPE         const  pHDB,
                                                                POF_COMMON_MEM_PTR_TYPE  const  pRecordData,
                                                                LSA_UINT16               const  BlockType,
                                                                LSA_UINT16               const  BlockLength)
                                                                //LSA_UINT8                const  BlockVersionHigh,
                                                                //LSA_UINT8                const  BlockVersionLow,
                                                                //LSA_UINT8                const  Padding1,
                                                                //LSA_UINT8                const  Padding2)
{
    POF_UPPER_PRM_RECORD_HEADER_PTR_TYPE  pHeader = (POF_UPPER_PRM_RECORD_HEADER_PTR_TYPE) pRecordData; 

    if (pHeader->N_BlockType != POF_HTONT_16(BlockType))
    {
        POF_SET_DETAIL_ERR(POF_ERR_PRM_CHECK_BLOCK_TYPE);
        POF_PRM_SET_ERR_OFFSET(pHDB, POF_PRM_BLOCKHEADER_OFFSET_BlockType);
        return POF_RSP_ERR_PRM_BLOCK;
    }

    if (pHeader->N_BlockLength != POF_HTONT_16(BlockLength - 4UL))
    {
        POF_SET_DETAIL_ERR(POF_ERR_PRM_CHECK_BLOCK_1_LENGTH);
        POF_PRM_SET_ERR_OFFSET(pHDB, POF_PRM_BLOCKHEADER_OFFSET_BlockLength);
        return POF_RSP_ERR_PRM_BLOCK;
    }

    if (pHeader->N_BlockVersionHigh != POF_PRM_BLOCK_VERSION_HIGH)
    {
        POF_SET_DETAIL_ERR(POF_ERR_PRM_CHECK_VERSION_HIGH);
        POF_PRM_SET_ERR_OFFSET(pHDB, POF_PRM_BLOCKHEADER_OFFSET_BlockVersionHigh);
        return POF_RSP_ERR_PRM_BLOCK;
    }

    if (pHeader->N_BlockVersionLow != POF_PRM_BLOCK_VERSION_LOW)
    {
        POF_SET_DETAIL_ERR(POF_ERR_PRM_CHECK_VERSION_LOW);
        POF_PRM_SET_ERR_OFFSET(pHDB, POF_PRM_BLOCKHEADER_OFFSET_BlockVersionLow);
        return POF_RSP_ERR_PRM_BLOCK;
    }

    if (pHeader->N_Padding1_1 != 0)
    {
        POF_SET_DETAIL_ERR(POF_ERR_PRM_CHECK_PADDING_1);
        POF_PRM_SET_ERR_OFFSET(pHDB, POF_PRM_BLOCKHEADER_OFFSET_Padding1_1);
        return POF_RSP_ERR_PRM_BLOCK;
    }

    if (pHeader->N_Padding1_2 != 0)
    {
        POF_SET_DETAIL_ERR(POF_ERR_PRM_CHECK_PADDING_2);
        POF_PRM_SET_ERR_OFFSET(pHDB, POF_PRM_BLOCKHEADER_OFFSET_Padding1_2);
        return POF_RSP_ERR_PRM_BLOCK;
    }
    return POF_RSP_OK;
}
/*=============================================================================
 * function name:   POF_CheckPrmHeader()
 *
 * parameters:      
 *
 * return value:    POF_RSP
 *
 * description :    
 *===========================================================================*/
POF_RSP  POF_LOCAL_FCT_ATTR  POF_CheckPrmHeader( POF_HDB_PTR_TYPE         const  pHDB,
                                                 LSA_UINT32               const  RecordIndex,
                                                 POF_COMMON_MEM_PTR_TYPE  const  pRecordData )
{
    POF_RSP  Response = POF_RSP_OK;

    POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_CheckPrmHeader, RecordIndex:0x%X", RecordIndex);

    switch (RecordIndex)
    {
        case POF_PRM_FO_DATA_CHECK_INDEX:
        {
            Response = POF_CheckPrmHeaderSpecific(pHDB, pRecordData, POF_PRM_FO_DATA_CHECK_BLOCKTYPE, POF_PRM_FO_DATA_CHECK_LENGTH);
                                                  //POF_PRM_BLOCK_VERSION_HIGH, POF_PRM_BLOCK_VERSION_LOW);
        } break;
    
        case POF_PRM_FO_DATA_ADJUST_INDEX:
        {
            Response = POF_CheckPrmHeaderSpecific(pHDB, pRecordData, POF_PRM_FO_DATA_ADJUST_BLOCKTYPE, POF_PRM_FO_DATA_ADJUST_LENGTH);
        } break;

        case POF_PRM_SFP_DATA_CHECK_INDEX:
        {
            Response = POF_CheckPrmHeaderSpecific(pHDB, pRecordData, POF_PRM_SFP_DATA_CHECK_BLOCKTYPE, POF_PRM_SFP_DATA_CHECK_LENGTH);
        } break;

        case POF_PRM_SFP_DIAG_TEST_INDEX:
        {
            Response = POF_CheckPrmHeaderSpecific(pHDB, pRecordData, POF_PRM_SFP_DIAG_TEST_BLOCKTYPE, POF_PRM_SFP_DIAG_TEST_LENGTH);
        } break;

        default:
        {
            POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_CheckPrmHeader, RecordIndex:0x%X", RecordIndex);
            LSA_UNUSED_ARG(pHDB); //satisfy lint!
            POF_FATAL("POF_CheckPrmHeader", POF_FATAL_ERR_PRM_READ_RECORD_INDEX, 0, 0);
        } break;
    }

    POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_CheckPrmHeader, RecordIndex:0x%X", RecordIndex);

    return Response;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*****************************************************************************/
/*  end of file pof_prm_check.c                                              */
/*****************************************************************************/
