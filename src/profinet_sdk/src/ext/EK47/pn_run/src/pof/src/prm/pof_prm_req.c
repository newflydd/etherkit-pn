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
/*  F i l e               &F: pof_prm_req.c                             :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  POF PRM REQ                                      */
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
#define LTRC_ACT_MODUL_ID  9
#define POF_MODULE_ID      LTRC_ACT_MODUL_ID

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/
#include "pof_inc.h"            /* POF headerfiles */
#include "pof_dmi_type.h"
#include "pof_prm_type.h"
#include "pof_int.h"            /* internal header */
#include "pof_prm_req.h"
#include "pof_prm_rec.h"
#include "pof_prm_state_diag.h"
#include "pof_prm_check.h"
#include "pof_glb.h"
#include "pof_edd.h"
#include "pof_dmi.h"

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
static  LSA_VOID  POF_LOCAL_FCT_ATTR  POF_SetPrmHeader(      POF_HDB_PTR_TYPE        const  pHDB,
                                                             LSA_UINT32              const  RecordIndex,
                                                             POF_UPPER_MEM_PTR_TYPE  const  pRecordData );
static  LSA_VOID  POF_LOCAL_FCT_ATTR  POF_SetPrmHeaderShort( POF_HDB_PTR_TYPE        const  pHDB,
                                                             LSA_UINT32              const  RecordIndex,
                                                             POF_UPPER_MEM_PTR_TYPE  const  pRecordData );
static  LSA_VOID  POF_LOCAL_FCT_ATTR  POF_PrmChangePort_Check_Consistency( 
                                                             POF_HDB_PTR_TYPE        const  pHDB,
                                                             LSA_UINT16                     PortIndex);

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


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_RequestPrmRead()                        +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    POF_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pHDB       : User HDB                                                  +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE        opcode:      POF_OPC_GET_CONFIG              +*/
/*+     LSA_HANDLE_TYPE        handle:                                      +*/
/*+     LSA_USER_ID_TYPE       user_id:                                     +*/
/*+     POF_RSP                Response     Response                        +*/
/*+     POF_RQB_ARGS_TYPE      args.GetConfig                               +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Request to get config parameters from POF                 +*/
/*+                                                                         +*/
/*+               Fills RQB with config data and confirms request.          +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
POF_RSP  POF_LOCAL_FCT_ATTR  POF_RequestPrmRead( POF_UPPER_RQB_PTR_TYPE  const  pRQB,
                                                 POF_HDB_PTR_TYPE        const  pHDB )
{
  POF_RSP                        Response;
  LSA_UINT16                     PortIndex;
  LSA_UINT32              const  RecordIndex = pRQB->args.PrmRead.record_index;
  POF_EDD_PCTRL_PTR_TYPE         pPCtrl;

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_RequestPrmRead, pHDB:0x%X", pHDB);

  //POF_RSP_ERR_PRM_PORTID or POF_RSP_ERR_PARAM
  Response = POF_Get_Port_Index(pRQB->args.PrmRead.edd_port_id, &PortIndex, LSA_TRUE, pHDB);
  if (Response != POF_RSP_OK) 
  {
    POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "POF_RequestPrmRead, POF_Get_Port_Index:0x%X", 
                         pRQB->args.PrmRead.edd_port_id);
    return Response;
  }  

  if (!POF_PRMChangePortState_IsPlugged(pHDB, PortIndex))
  {
    POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "POF_RequestPrmRead, Port(%d) is not plugged", 
                         PortIndex);
    return POF_RSP_ERR_PRM_PORTID;
  }  

  //POF_RSP_ERR_PRM_INDEX or POF_RSP_ERR_PRM_DATA
  Response = POF_CheckPrmRecord_Data_Read(pHDB,
                                          RecordIndex, 
                                          &pRQB->args.PrmRead.record_data_length);
  if (POF_RSP_OK != Response)
  {
    POF_PROGRAM_TRACE_03(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmRead, Response:0x%X RecordIndex:0x%X record_data_length:0x%X",
                         Response, RecordIndex, pRQB->args.PrmRead.record_data_length);
    return Response;
  }

  //POF_RSP_ERR_PARAM
  if (LSA_HOST_PTR_ARE_EQUAL(pRQB->args.PrmRead.record_data, LSA_NULL))          
  {
    POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmRead, record_data == LSA_NULL");
    return POF_RSP_ERR_PARAM;
  }

  pPCtrl = &pHDB->pPCtrl[PortIndex];

  switch (RecordIndex)
  {
    case POF_PRM_FO_DATA_REAL_INDEX: //record must be readable all time
    {
      LSA_BOOL                          bLinkUp;
      POF_PRM_RECORD_FO_DATA_REAL_TYPE  FO_Data_Real;
      LSA_UINT32                        Length = POF_PRM_FO_DATA_REAL_LENGTH;

      if  (  !POF_IS_FIBER_PORT(PortIndex, pHDB)  )
      {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "POF_RequestPrmRead, no Fiber Port, PortIndex:0x%X", PortIndex);
        POF_SET_DETAIL_ERR(POF_ERR_PORT_NO_POF);
        return POF_RSP_ERR_PRM_INDEX;
      }
      else if (  POF_IS_POF_PORT(         PortIndex, pHDB)
              && POF_TransceiverIsPlugged(PortIndex, pHDB)  )
      {
        POF_SetPrmHeader(pHDB, RecordIndex, (POF_UPPER_MEM_PTR_TYPE)&FO_Data_Real);

        bLinkUp = (LSA_BOOL)(EDD_LINK_UP == pPCtrl->LinkStatus.Status ? LSA_TRUE : LSA_FALSE);

        POF_Read_FO_DataReal(PortIndex, bLinkUp, &FO_Data_Real, pHDB);
      }
      else
      {
        //Data shorten in case of Fiber but no POF
        Length = POF_PRM_FO_DATA_REAL_SHORT_LENGTH;
        pRQB->args.PrmRead.record_data_length = Length;

        POF_SetPrmHeaderShort(pHDB, RecordIndex, (POF_UPPER_MEM_PTR_TYPE)&FO_Data_Real);
        POF_Read_FO_DataRealShort(PortIndex, &FO_Data_Real, pHDB);
      }

      //not sure that record_data will be a 32-Bit Address
      POF_MEMCOPY_COMMON((POF_COMMON_MEM_PTR_TYPE)pRQB->args.PrmRead.record_data,
                         (POF_COMMON_MEM_PTR_TYPE)&FO_Data_Real,
                         Length);
    } break;

    case POF_PRM_FO_DATA_CHECK_INDEX:
    {
      LSA_UINT32  FO_Data_Check[(sizeof(POF_PRM_RECORD_FO_DATA_CHECK_TYPE) / sizeof(LSA_UINT32)) + 1]; //Do not use POF_PRM_RECORD_FO_DATA_CHECK_TYPE for stack allocation, as compiler may 
                                                                                                        //ignore alignments for packed structs
      POF_UPPER_PRM_FO_DATA_CHECK_PTR_TYPE const pFO_Data_Check = (POF_UPPER_PRM_FO_DATA_CHECK_PTR_TYPE)((LSA_VOID *)&FO_Data_Check[0]);

      //Read not allowed for nonPOF COMPACT ports
      if  (!POF_IS_POF_PORT(PortIndex, pHDB)  &&  (POF_PRM_PORT_IS_COMPACT == pHDB->IsModularPort[PortIndex]))
      {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmRead PDPortFODataCheck, no Fiber Port and COMPACT port, PortIndex:0x%X", PortIndex);
        POF_SET_DETAIL_ERR(POF_ERR_PORT_NO_POF);
        return POF_RSP_ERR_PRM_INDEX;
      }
      else
      {
        //record not written
        if (0 == (POF_RECORD_TYPE_DATA_CHECK_8061 & pPCtrl->RecordSet_A.RecordTypeWritten) )
        {
            pRQB->args.PrmRead.record_data_length = 0;
            return POF_RSP_OK;
        }

        POF_SetPrmHeader(pHDB, RecordIndex, (POF_UPPER_MEM_PTR_TYPE)pFO_Data_Check);

        POF_Read_FO_DataCheck(PortIndex, pFO_Data_Check, pHDB);

        /* not sure that record_data will be a 32-Bit Address */
        POF_MEMCOPY_COMMON((POF_COMMON_MEM_PTR_TYPE)pRQB->args.PrmRead.record_data,
                            (POF_COMMON_MEM_PTR_TYPE)pFO_Data_Check,
                            POF_PRM_FO_DATA_CHECK_LENGTH);
      }
    } break;

    case POF_PRM_FO_DATA_ADJUST_INDEX:
    {
      LSA_UINT32  FO_Data_Adjust[(sizeof(POF_PRM_RECORD_FO_DATA_ADJUST_TYPE) / sizeof(LSA_UINT32)) + 1]; //Do not use POF_PRM_RECORD_FO_DATA_ADJUST_TYPE for stack allocation, as compiler may 
                                                                                                        //ignore alignments for packed structs
      POF_UPPER_PRM_FO_DATA_ADJUST_PTR_TYPE const pFO_Data_Adjust = (POF_UPPER_PRM_FO_DATA_ADJUST_PTR_TYPE)((LSA_VOID *)&FO_Data_Adjust[0]);

      //Read not allowed for nonPOF COMPACT ports
      if  (!POF_IS_POF_PORT(PortIndex, pHDB)  &&  (POF_PRM_PORT_IS_COMPACT == pHDB->IsModularPort[PortIndex]))
      {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmRead PDPortFODataAdjust, no Fiber Port and COMPACT port, PortIndex:0x%X", PortIndex);
        POF_SET_DETAIL_ERR(POF_ERR_PORT_NO_POF);
        return POF_RSP_ERR_PRM_INDEX;
      }
      else
      {
        //record not written
        if (0 == (POF_RECORD_TYPE_DATA_ADJUST_8062 & pPCtrl->RecordSet_A.RecordTypeWritten) )
        {
          pRQB->args.PrmRead.record_data_length = 0;
          return POF_RSP_OK;
        }
      
        POF_SetPrmHeader(pHDB, RecordIndex, (POF_UPPER_MEM_PTR_TYPE)pFO_Data_Adjust);

        POF_Read_FO_DataAdjust(PortIndex, pFO_Data_Adjust, pHDB);

        /* not sure that record_data will be a 32-Bit Address */
        POF_MEMCOPY_COMMON((POF_COMMON_MEM_PTR_TYPE)pRQB->args.PrmRead.record_data,
                           (POF_COMMON_MEM_PTR_TYPE)pFO_Data_Adjust,
                            POF_PRM_FO_DATA_ADJUST_LENGTH);
      }
    } break;

    case POF_PRM_SFP_DATA_CHECK_INDEX:
    {
      LSA_UINT32  SFP_Data_Check[(sizeof(POF_PRM_RECORD_SFP_DATA_CHECK_TYPE) / sizeof(LSA_UINT32)) + 1]; //Do not use POF_PRM_RECORD_FO_DATA_CHECK_TYPE for stack allocation, as compiler may 
                                                                                                            //ignore alignments for packed structs
      POF_UPPER_PRM_SFP_DATA_CHECK_PTR_TYPE const pSFP_Data_Check = (POF_UPPER_PRM_SFP_DATA_CHECK_PTR_TYPE)((LSA_VOID *)&SFP_Data_Check[0]);

      //Read not allowed for nonSFP COMPACT ports
      if  (!POF_IS_SFP_PORT(PortIndex, pHDB)  &&  (POF_PRM_PORT_IS_COMPACT == pHDB->IsModularPort[PortIndex]))
      {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmRead PDPortFODMIDataCheck, no Fiber Port and COMPACT port, PortIndex:0x%X", PortIndex);
        POF_SET_DETAIL_ERR(POF_ERR_PORT_NO_SFP);
        return POF_RSP_ERR_PRM_INDEX;
      }
      else
      {
        //record not written
        if (0 == (POF_RECORD_TYPE_SFP_DATA_CHECK_8063 & pPCtrl->RecordSet_A.RecordTypeWritten) )
        {
          pRQB->args.PrmRead.record_data_length = 0;
          return POF_RSP_OK;
        }

        POF_SetPrmHeader(pHDB, RecordIndex, (POF_UPPER_MEM_PTR_TYPE)pSFP_Data_Check);

        POF_Read_SFP_DataCheck(PortIndex, pSFP_Data_Check, pHDB);

        /* not sure that record_data will be a 32-Bit Address */
        POF_MEMCOPY_COMMON((POF_COMMON_MEM_PTR_TYPE)pRQB->args.PrmRead.record_data,
                            (POF_COMMON_MEM_PTR_TYPE)pSFP_Data_Check,
                            POF_PRM_SFP_DATA_CHECK_LENGTH);
      }
    } break;

    case POF_PRM_SFP_DIAG_TEST_INDEX:
    {
      LSA_UINT32  SFP_Diag_Test[(sizeof(POF_PRM_RECORD_SFP_DIAG_TEST_TYPE) / sizeof(LSA_UINT32)) + 1]; //Do not use POF_PRM_RECORD_FO_DATA_CHECK_TYPE for stack allocation, as compiler may 
                                                                                                            //ignore alignments for packed structs
      POF_UPPER_PRM_SFP_DIAG_TEST_PTR_TYPE const pSFP_Diag_Test = (POF_UPPER_PRM_SFP_DIAG_TEST_PTR_TYPE)((LSA_VOID *)&SFP_Diag_Test[0]);

      //Read not allowed for nonSFP COMPACT ports
      if  (!POF_IS_SFP_PORT(PortIndex, pHDB)  &&  (POF_PRM_PORT_IS_COMPACT == pHDB->IsModularPort[PortIndex]))
      {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmRead PDPortFODMIDataCheck, no Fiber Port and COMPACT port, PortIndex:0x%X", PortIndex);
        POF_SET_DETAIL_ERR(POF_ERR_PORT_NO_SFP);
        return POF_RSP_ERR_PRM_INDEX;
      }
      else if  (!pPCtrl->TestTranceiverDetected)
      {
//        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmRead PDPortFODMIDataCheck, no Fiber Port and COMPACT port, PortIndex:0x%X", PortIndex);
        POF_SET_DETAIL_ERR(POF_ERR_TEST_DIAG);
        return POF_RSP_ERR_PRM_INDEX; //POF_RSP_ERR_TRANSCEIVER_NOT_SUPPORTED;
      }
      else
      {
        POF_SetPrmHeader(pHDB, RecordIndex, (POF_UPPER_MEM_PTR_TYPE)pSFP_Diag_Test);

        POF_Read_SFP_DiagTest(PortIndex, pSFP_Diag_Test, pHDB);

        /* not sure that record_data will be a 32-Bit Address */
        POF_MEMCOPY_COMMON((POF_COMMON_MEM_PTR_TYPE)pRQB->args.PrmRead.record_data,
                           (POF_COMMON_MEM_PTR_TYPE)pSFP_Diag_Test,
                            POF_PRM_SFP_DATA_CHECK_LENGTH);
      }
    } break;

    case POF_PRM_FO_DATA_TRA_IND_TEST_INDEX:
    {
      LSA_UINT32  TransceiverIdent[(sizeof(POF_PRM_RECORD_TRANSCEIVER_INDENT_TYPE) / sizeof(LSA_UINT32)) + 1]; //Do not use POF_PRM_RECORD_TRANSCEIVER_INDENT_TYPE for stack allocation, as compiler may 
                                                                                                               //ignore alignments for packed structs
      POF_UPPER_PRM_TRANSCEIVER_INDENT_PTR_TYPE const pTransceiverIdent = (POF_UPPER_PRM_TRANSCEIVER_INDENT_PTR_TYPE)((LSA_VOID *)&TransceiverIdent[0]);
        
      //POF_RSP_ERR_PRM_INDEX, no POF-Port
      if (!POF_IS_POF_PORT(PortIndex, pHDB))
      {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "POF_RequestPrmRead, no POF-Port, PortIndex:0x%X", PortIndex);
        POF_SET_DETAIL_ERR(POF_ERR_PORT_NO_POF);
        return POF_RSP_ERR_PRM_INDEX;
      }

      POF_SetPrmHeader(pHDB, RecordIndex, (POF_UPPER_MEM_PTR_TYPE)pTransceiverIdent);

      POF_Read_Transceiver_Ident(PortIndex, pTransceiverIdent, pHDB);

      //not sure that record_data will be a 32-Bit Address
      POF_MEMCOPY_COMMON((POF_COMMON_MEM_PTR_TYPE)pRQB->args.PrmRead.record_data,
                         (POF_COMMON_MEM_PTR_TYPE)pTransceiverIdent,
                          POF_PRM_FO_DATA_TRA_IND_TEST_LENGTH);
    } break;

    default: //wrong RecordIndex
    {
      POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmRead, wrong RecordIndex:0x%X", RecordIndex);
      return POF_RSP_ERR_PRM_INDEX; //wrong RecordIndex
    }
  }

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_RequestPrmRead, pHDB:0x%X", pHDB);

  return POF_RSP_OK;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_RequestPrmIndication()                  +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    POF_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pHDB       : User HDB                                                  +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE        opcode:      POF_OPC_GET_CONFIG              +*/
/*+     LSA_HANDLE_TYPE        handle:                                      +*/
/*+     LSA_USER_ID_TYPE       user_id:                                     +*/
/*+     POF_RSP                Response     Response                        +*/
/*+     POF_RQB_ARGS_TYPE      args.GetConfig                               +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Request to get config parameters from POF                 +*/
/*+                                                                         +*/
/*+               Fills RQB with config data and confirms request.          +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
POF_RSP  POF_LOCAL_FCT_ATTR  POF_RequestPrmIndication( POF_LOCAL_MEM_BOOL_PTR_TYPE  const  pIndicate,
                                                       POF_UPPER_RQB_PTR_TYPE       const  pRQB,
                                                       POF_HDB_PTR_TYPE             const  pHDB )
{
  POF_RSP                 Response;
  LSA_UINT16              PortIndex;
  POF_EDD_PCTRL_PTR_TYPE  pPCtrl;

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_RequestPrmIndication, pRQB:0x%X", pRQB);

  Response = POF_Get_Port_Index(pRQB->args.PrmIndication.edd_port_id, &PortIndex, LSA_TRUE, pHDB);
  if (Response != POF_RSP_OK)
  {
    return Response;
  }

  *pIndicate = LSA_FALSE;

  pPCtrl = &pHDB->pPCtrl[PortIndex];

  /* RQB's assign a port */
  POF_QueueAddToEnd(&pPCtrl->PrmIndicationQueue, (POF_QUEUE_ELEMENT_PTR_TYPE)(void *)pRQB);


  /* must report a lost prm_indication */
  if (pPCtrl->LostPrmInd.bLost)
  {
    POF_PrmSendIndication(PortIndex,
                          pHDB,
                          pPCtrl->LostPrmInd.AppearEvent,
                          pPCtrl->LostPrmInd.DisappearEvent,
                          pPCtrl->LostPrmInd.AddValue);

    /* reset lost PrmIndication */
    pPCtrl->LostPrmInd.bLost           = LSA_FALSE;
    pPCtrl->LostPrmInd.AppearEvent     = POF_PRM_EV_GOOD;
    pPCtrl->LostPrmInd.DisappearEvent  = POF_PRM_EV_GOOD;
    pPCtrl->LostPrmInd.AddValue        = 0;
  }

  POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_RequestPrmIndication");

  return POF_RSP_OK;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_RequestPrmWrite()                       +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    POF_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pHDB       : User HDB                                                  +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE        opcode:      POF_OPC_GET_CONFIG              +*/
/*+     LSA_HANDLE_TYPE        handle:                                      +*/
/*+     LSA_USER_ID_TYPE       user_id:                                     +*/
/*+     POF_RSP                Response     Response                        +*/
/*+     POF_RQB_ARGS_TYPE      args.GetConfig                               +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Request to get config parameters from POF                 +*/
/*+                                                                         +*/
/*+               Fills RQB with config data and confirms request.          +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
POF_RSP  POF_LOCAL_FCT_ATTR  POF_RequestPrmWrite( POF_UPPER_RQB_PTR_TYPE  const  pRQB,
                                                  POF_HDB_PTR_TYPE        const  pHDB )
{
  POF_RSP            Response;
  LSA_UINT16         PortIndex;
  LSA_UINT32  const  RecordIndex = pRQB->args.PrmWrite.record_index;

  POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_RequestPrmWrite");

  /* POF_RSP_ERR_PRM_PORTID or POF_RSP_ERR_PARAM */
  Response = POF_Get_Port_Index(pRQB->args.PrmWrite.edd_port_id, &PortIndex, LSA_TRUE, pHDB);
  if (Response != POF_RSP_OK)
  {
    POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmWrite, POF_Get_Port_Index, edd_port_id:0x%X", 
                         pRQB->args.PrmWrite.edd_port_id);
    return Response;
  }  

  //POF_RSP_ERR_PRM_INDEX or POF_RSP_ERR_PRM_DATA
  Response = POF_CheckPrmRecord_Data_Write(pHDB,
                                           RecordIndex,
                                           pRQB->args.PrmWrite.record_data_length);
  if (POF_RSP_OK != Response)
  {
    POF_PROGRAM_TRACE_03(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmWrite, Response:0x%X RecordIndex:0x%X record_data_length:0x%X",
                         Response, RecordIndex, pRQB->args.PrmWrite.record_data_length);
    return Response;
  }

  //POF_RSP_ERR_PARAM
  if (LSA_HOST_PTR_ARE_EQUAL(pRQB->args.PrmWrite.record_data, LSA_NULL))          
  {
    POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmWrite, record_data == LSA_NULL");
    return POF_RSP_ERR_PARAM;
  }

  switch (RecordIndex) 
  {
    case POF_PRM_FO_DATA_CHECK_INDEX:
    {
      LSA_UINT32  FO_Data_Check[(sizeof(POF_PRM_RECORD_FO_DATA_CHECK_TYPE) / sizeof(LSA_UINT32)) + 1]; //Do not use POF_PRM_RECORD_FO_DATA_CHECK_TYPE for stack allocation, as compiler may 
                                                                                                       //ignore alignments for packed structs
      POF_UPPER_PRM_FO_DATA_CHECK_PTR_TYPE const pFO_Data_Check = (POF_UPPER_PRM_FO_DATA_CHECK_PTR_TYPE)((LSA_VOID *)&FO_Data_Check[0]);
        
      //Write not allowed for nonFiber COMPACT ports
      if  (   !POF_IS_FIBER_PORT(PortIndex, pHDB) 
           && (POF_PRM_PORT_IS_COMPACT == pHDB->IsModularPort[PortIndex]) )
      {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmWrite PDPortFODataCheck, no Fiber Port and COMPACT port, PortIndex:0x%X", PortIndex);
        POF_SET_DETAIL_ERR(POF_ERR_PORT_NO_POF);
        return POF_RSP_ERR_PRM_INDEX;
      }
      else
      {
        if (pHDB->I2C[PortIndex].I2CAvailable != POF_I2C_AVAILABLE)
        {
          POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmWrite, I2C not available for this port, RecordIndex:0x%X", RecordIndex);
          return POF_RSP_ERR_PRM_INDEX;
        }

        //not sure that record_data will be a 32-Bit Address
        POF_MEMCOPY_COMMON((POF_COMMON_MEM_PTR_TYPE)pFO_Data_Check,
                            (POF_COMMON_MEM_PTR_TYPE)pRQB->args.PrmWrite.record_data,
                            POF_PRM_FO_DATA_CHECK_LENGTH);

        //POF_RSP_ERR_PRM_BLOCK
        Response = POF_CheckPrmHeader(pHDB, RecordIndex, pFO_Data_Check);
        if (POF_RSP_OK != Response)
        {
          POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmWrite, CheckPrmHeader, RecordIndex:0x%X", RecordIndex);
          return Response;
        }

        //POF_RSP_ERR_PRM_BLOCK
        Response = POF_Write_FO_DataCheck(PortIndex, pFO_Data_Check, pHDB);
        if (POF_RSP_OK != Response)
        {
          POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmWrite, POF_Write_FO_DataCheck, RecordIndex:0x%X", RecordIndex);
          return Response;
        }
      }
    } break;

    case POF_PRM_FO_DATA_ADJUST_INDEX:
    {
      LSA_UINT32  FO_Data_Adjust[(sizeof(POF_PRM_RECORD_FO_DATA_ADJUST_TYPE) / sizeof(LSA_UINT32)) + 1]; //Do not use POF_PRM_RECORD_FO_DATA_ADJUST_TYPE for stack allocation, as compiler may 
                                                                                                         //ignore alignments for packed structs
      POF_UPPER_PRM_FO_DATA_ADJUST_PTR_TYPE const pFO_Data_Adjust = (POF_UPPER_PRM_FO_DATA_ADJUST_PTR_TYPE)((LSA_VOID *)&FO_Data_Adjust[0]);


      //Write not allowed for nonFiber COMPACT ports
      if  (   !POF_IS_FIBER_PORT(PortIndex, pHDB) 
           && (POF_PRM_PORT_IS_COMPACT == pHDB->IsModularPort[PortIndex]) )
      {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmWrite PDPortFODataAdjust, no Fiber Port and COMPACT port, PortIndex:0x%X", PortIndex);
        POF_SET_DETAIL_ERR(POF_ERR_PORT_NO_POF);
        return POF_RSP_ERR_PRM_INDEX;
      }
      else
      {
        /* not sure that record_data will be a 32-Bit Address */
        POF_MEMCOPY_COMMON((POF_COMMON_MEM_PTR_TYPE)pFO_Data_Adjust,
                           (POF_COMMON_MEM_PTR_TYPE)pRQB->args.PrmWrite.record_data,
                            POF_PRM_FO_DATA_ADJUST_LENGTH);

        //POF_RSP_ERR_PRM_BLOCK
        Response = POF_CheckPrmHeader(pHDB, RecordIndex, pFO_Data_Adjust);
        if (POF_RSP_OK != Response)
        {
          POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmWrite, CheckPrmHeader, RecordIndex:0x%X", RecordIndex);
          return Response;
        }
      
        //POF_RSP_ERR_PRM_BLOCK
        Response = POF_Write_FO_DataAdjust(PortIndex, pFO_Data_Adjust, pHDB);
        if (POF_RSP_OK != Response)
        {
          POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmWrite, POF_Write_FO_DataAdjust, RecordIndex:0x%X", RecordIndex);
          return Response;
        }
      }
    } break;

    case POF_PRM_SFP_DATA_CHECK_INDEX:
    {
      LSA_UINT32  SFP_Data_Check[(sizeof(POF_PRM_RECORD_SFP_DATA_CHECK_TYPE) / sizeof(LSA_UINT32)) + 1]; //Do not use POF_PRM_RECORD_SFP_DATA_CHECK_TYPE for stack allocation, as compiler may 
                                                                                                            //ignore alignments for packed structs
      POF_UPPER_PRM_SFP_DATA_CHECK_PTR_TYPE const pSFP_Data_Check = (POF_UPPER_PRM_SFP_DATA_CHECK_PTR_TYPE)((LSA_VOID *)&SFP_Data_Check[0]);
      
      //Write not allowed for nonSFP COMPACT ports
      if  (   !POF_IS_SFP_PORT(PortIndex, pHDB) 
           && (POF_PRM_PORT_IS_COMPACT == pHDB->IsModularPort[PortIndex]) )
      {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmWrite PDPortSFPDataCheck, no SFP Port and COMPACT port, PortIndex:0x%X", PortIndex);
        POF_SET_DETAIL_ERR(POF_ERR_PORT_NO_SFP);
        return POF_RSP_ERR_PRM_INDEX;
      }
      else
      {
        if (pHDB->I2C[PortIndex].I2CAvailable != POF_I2C_AVAILABLE)
        {
          POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmWrite, I2C not available for this port, RecordIndex:0x%X", RecordIndex);
          return POF_RSP_ERR_PRM_INDEX;
        }

        //not sure that record_data will be a 32-Bit Address
        POF_MEMCOPY_COMMON((POF_COMMON_MEM_PTR_TYPE)pSFP_Data_Check,
                           (POF_COMMON_MEM_PTR_TYPE)pRQB->args.PrmWrite.record_data,
                            POF_PRM_SFP_DATA_CHECK_LENGTH);

        //POF_RSP_ERR_PRM_BLOCK
        Response = POF_CheckPrmHeader(pHDB, RecordIndex, pSFP_Data_Check);
        if (POF_RSP_OK != Response)
        {
          POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmWrite, CheckPrmHeader, RecordIndex:0x%X", RecordIndex);
          return Response;
        }

        //POF_RSP_ERR_PRM_BLOCK
        Response = POF_Write_SFP_DataCheck(PortIndex, pSFP_Data_Check, pHDB);
        if (POF_RSP_OK != Response)
        {
          POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmWrite, POF_Write_SFP_DataCheck, RecordIndex:0x%X", RecordIndex);
          return Response;
        }
      }
    } break;

    case POF_PRM_SFP_DIAG_TEST_INDEX:
    {
      LSA_UINT32  SFP_Diag_Test[(sizeof(POF_PRM_RECORD_SFP_DIAG_TEST_TYPE) / sizeof(LSA_UINT32)) + 1]; //Do not use POF_PRM_RECORD_SFP_DIAG_TEST_TYPE for stack allocation, as compiler may 
                                                                                                       //ignore alignments for packed structs
      POF_UPPER_PRM_SFP_DIAG_TEST_PTR_TYPE const pSFP_Diag_Test = (POF_UPPER_PRM_SFP_DIAG_TEST_PTR_TYPE)((LSA_VOID *)&SFP_Diag_Test[0]);
      
      //Write not allowed for nonSFP COMPACT ports
      if  (   !POF_IS_SFP_PORT(PortIndex, pHDB) 
           && (POF_PRM_PORT_IS_COMPACT == pHDB->IsModularPort[PortIndex]) )
      {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmWrite PDPortSFPDiagTest, no SFP Port and COMPACT port, PortIndex:0x%X", PortIndex);
        POF_SET_DETAIL_ERR(POF_ERR_PORT_NO_SFP);
        return POF_RSP_ERR_PRM_INDEX;
      }
      else if  (!pHDB->pPCtrl[PortIndex].TestTranceiverDetected)
      {
//        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmRead PDPortFODMIDataCheck, no Fiber Port and COMPACT port, PortIndex:0x%X", PortIndex);
        POF_SET_DETAIL_ERR(POF_ERR_TEST_DIAG);
        return POF_RSP_ERR_PRM_INDEX; //POF_RSP_ERR_TRANSCEIVER_NOT_SUPPORTED;
      }
      else
      {
        if (pHDB->I2C[PortIndex].I2CAvailable != POF_I2C_AVAILABLE)
        {
          POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmWrite, I2C not available for this port, RecordIndex:0x%X", RecordIndex);
          return POF_RSP_ERR_PRM_INDEX;
        }

        //not sure that record_data will be a 32-Bit Address
        POF_MEMCOPY_COMMON((POF_COMMON_MEM_PTR_TYPE)pSFP_Diag_Test,
                           (POF_COMMON_MEM_PTR_TYPE)pRQB->args.PrmWrite.record_data,
                            POF_PRM_SFP_DIAG_TEST_LENGTH);

        //POF_RSP_ERR_PRM_BLOCK
        Response = POF_CheckPrmHeader(pHDB, RecordIndex, pSFP_Diag_Test);
        if (POF_RSP_OK != Response)
        {
          POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmWrite, CheckPrmHeader, RecordIndex:0x%X", RecordIndex);
          return Response;
        }

        //POF_RSP_ERR_PRM_BLOCK
        Response = POF_Write_SFP_DiagTest(PortIndex, pSFP_Diag_Test, pHDB);

        if (POF_RSP_OK != Response)
        {
          POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmWrite, POF_Write_SFP_DiagTest, RecordIndex:0x%X", RecordIndex);
          return Response;
        }
      }
    } break;

    default: //wrong RecordIndex
    {
      POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmWrite, wrong RecordIndex:0x%X", RecordIndex);
      return POF_RSP_ERR_PRM_INDEX; //wrong RecordIndex
    }
  }
  
  POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_RequestPrmWrite");

  return POF_RSP_OK;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_RequestPrmChangePort()                  +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    POF_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pHDB       : User HDB                                                  +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE        opcode:      POF_OPC_PRM_CHANGE_PORT         +*/
/*+     LSA_HANDLE_TYPE        handle:                                      +*/
/*+     LSA_USER_ID_TYPE       user_id:                                     +*/
/*+     POF_RSP                Response     Response                        +*/
/*+     POF_RQB_ARGS_TYPE      args.GetConfig                               +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Request to get config parameters from POF                 +*/
/*+                                                                         +*/
/*+               Fills RQB with config data and confirms request.          +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
POF_RSP  POF_LOCAL_FCT_ATTR  POF_RequestPrmChangePort( POF_UPPER_RQB_PTR_TYPE  const  pRQB,
                                                       POF_HDB_PTR_TYPE        const  pHDB )
{
    POF_RSP     Response;
    LSA_UINT16  PortIndex;

    POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_RequestPrmChangePort");

    //POF_RSP_ERR_PRM_PORTID or POF_RSP_ERR_PARAM
    Response = POF_Get_Port_Index(pRQB->args.PrmChangePort.edd_port_id, &PortIndex, LSA_TRUE, pHDB);
    if (Response != POF_RSP_OK) 
    {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmChangePort, edd_port_id:0x%X",
                             pRQB->args.PrmChangePort.edd_port_id);
        return Response;
    }

    switch (pRQB->args.PrmChangePort.module_state)
    {
        case POF_PRM_PORTMODULE_PULLED:
            Response = POF_PRMChangePortStateMachine(pHDB, POF_PRM_PORT_MODULE_SM_TRIGGER_PULL, PortIndex);
            break;

        case POF_PRM_PORTMODULE_PLUG_PREPARE:
            Response = POF_PRMChangePortStateMachine(pHDB, POF_PRM_PORT_MODULE_SM_TRIGGER_PLUG_PREPARE, PortIndex);
            if (POF_RSP_OK == Response)
            {
                pRQB->args.PrmChangePort.port_params_not_applicable = pHDB->PortparamsNotApplicable[PortIndex];
            }

            break;

        case POF_PRM_PORTMODULE_PLUG_COMMIT:
            pHDB->ApplyDefaultPortparams[PortIndex] = pRQB->args.PrmChangePort.apply_default_portparams;
            Response = POF_PRMChangePortStateMachine(pHDB, POF_PRM_PORT_MODULE_SM_TRIGGER_PLUG_COMMIT, PortIndex);
            break;

        default:
            POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_RequestPrmChangePort, unknown module_state:0x%X",
                                 pRQB->args.PrmChangePort.module_state);
            Response = POF_RSP_ERR_PARAM;
            break;
    }

    POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_RequestPrmChangePort");

    return Response;
}
/*---------------------- end [subroutine] ---------------------------------*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_PRMChangePortStateMachine               +*/
/*+  Input/Output          :    POF_HDB_PTR_TYPE                 pHDB       +*/
/*+  Input                 :    POF_PORT_MODULE_SM_STATE_TYPE    Trigger    +*/
/*+  Input                 :    LSA_UINT16                       PortIndex  +*/
/*+                                                                         +*/
/*+  Result                :    POF_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pHDB          : Pointer to User HDB                           (in/out) +*/
/*+  Trigger       : Trigger to control the statemachine           (in)     +*/
/*+  PortIndex     : Describes which Port should be handeled       (in)     +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:    Controls Change Port State Machine                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
POF_RSP  POF_LOCAL_FCT_ATTR POF_PRMChangePortStateMachine(
    POF_HDB_PTR_TYPE                    const pHDB,
    POF_PRM_PORT_MODULE_SM_TRIGGER_TYPE const Trigger,
    LSA_UINT16                                PortIndex)
{
    POF_RSP Response = POF_RSP_OK;
    POF_EDD_PCTRL_PTR_TYPE const pPCtrl = &pHDB->pPCtrl[PortIndex];

    switch(pPCtrl->PortModuleSm.State)
    {
        case POF_PORT_MODULE_SM_STATE_PULLED:
        {
            if(POF_PRM_PORT_MODULE_SM_TRIGGER_PULL == Trigger)
            {
                // state persists
                // No action required
            }
            else if(POF_PRM_PORT_MODULE_SM_TRIGGER_PLUG_PREPARE == Trigger)
            {
                POF_PrmChangePort_Check_Consistency(pHDB, PortIndex);

                /* change state */
                pPCtrl->PortModuleSm.State = POF_PORT_MODULE_SM_STATE_PLUGGING;
                POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_PRMChangePortStateMachine(), port:%d, POF_PORT_MODULE_SM_STATE_PULLED -> POF_PORT_MODULE_SM_STATE_PLUGGING", PortIndex);
            }
            else
            {
                POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_PRMChangePortStateMachine(): ERROR -> Wrong Trigger for State! State=0x%X, Trigger=0x%X",
                                      pPCtrl->PortModuleSm.State,
                                      Trigger);
                Response = POF_RSP_ERR_SEQUENCE;
                POF_SET_DETAIL_ERR(Response);
            }
        }
        break;

        case POF_PORT_MODULE_SM_STATE_PLUGGING:
        {
            if(POF_PRM_PORT_MODULE_SM_TRIGGER_PLUG_PREPARE == Trigger)
            {
                // state persists
                // No action required
            }
            else if(POF_PRM_PORT_MODULE_SM_TRIGGER_PULL == Trigger)
            {
                /* Trigger POF-Transceiver State Machine */
                POF_TransceiverSMTransition(PortIndex, POF_TRANSCEIVER_TRIGGER_STOP, pHDB);

                /* change state */
                pPCtrl->PortModuleSm.State = POF_PORT_MODULE_SM_STATE_PULLED;
                POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_PRMChangePortStateMachine(), port:%d, POF_PORT_MODULE_SM_STATE_PLUGGING -> POF_PORT_MODULE_SM_STATE_PULLED", PortIndex);
            }
            else if (POF_PRM_PORT_MODULE_SM_TRIGGER_PLUG_COMMIT == Trigger)
            {
                /* Trigger POF-Transceiver State Machine */
                POF_TRANSCEIVER_TRIGGER_TYPE trigger;

                if ( (pHDB->I2C[PortIndex].I2CAvailable == POF_I2C_AVAILABLE)
                    && (!pHDB->ApplyDefaultPortparams[PortIndex]) )
                {
                    //Only start polling if needed (diagnostic record(s) both available and usable)
                    trigger = POF_TRANSCEIVER_TRIGGER_START;
                }
                else
                {
                    trigger = POF_TRANSCEIVER_TRIGGER_STOP;
                }
                POF_TransceiverSMTransition(PortIndex, trigger, pHDB);

                /* change state */
                pPCtrl->PortModuleSm.State = POF_PORT_MODULE_SM_STATE_PLUGGED;
                POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_PRMChangePortStateMachine(), port:%d, POF_PORT_MODULE_SM_STATE_PLUGGING -> POF_PORT_MODULE_SM_STATE_PLUGGED", PortIndex);
            }
            else
            {
                POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_PRMChangePortStateMachine(): ERROR -> Wrong Trigger for State! State=0x%X, Trigger=0x%X",
                                      pPCtrl->PortModuleSm.State,
                                      Trigger);
                Response = POF_RSP_ERR_SEQUENCE;
                POF_SET_DETAIL_ERR(Response);
            }
        }
        break;

        case POF_PORT_MODULE_SM_STATE_PLUGGED:
        {
            if (POF_PRM_PORT_MODULE_SM_TRIGGER_PULL == Trigger)
            {
                LSA_UINT32  i;

                /* Trigger POF-Transceiver State Machine */
                POF_TransceiverSMTransition(PortIndex, POF_TRANSCEIVER_TRIGGER_STOP, pHDB);

                /* change state */
                pPCtrl->PortModuleSm.State = POF_PORT_MODULE_SM_STATE_PULLED;

                for (i = 0; i < POF_SFP_NUMBER_CHECK_DIAGNOSTICS; ++i)
                {
                  pPCtrl->SFP_Diagnostic.SFP_DiagnosticChecks[i].State             = SFP_PROPERTY_STATE_NORMAL;
                  pPCtrl->SFP_Diagnostic.SFP_DiagnosticChecks[i].LastReportedState = SFP_PROPERTY_STATE_NORMAL;
                }

                POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_PRMChangePortStateMachine(), port:%d, POF_PORT_MODULE_SM_STATE_PLUGGED -> POF_PORT_MODULE_SM_STATE_PULLED", PortIndex);
            }
            else if (POF_PRM_PORT_MODULE_SM_TRIGGER_PLUG_COMMIT == Trigger)
            {
                // state persists

                // Trigger POF-Transceiver State Machine only if I2C available!
                if (pHDB->I2C[PortIndex].I2CAvailable == POF_I2C_AVAILABLE)
                {
                    /* Trigger POF-Transceiver State Machine */
                    POF_TransceiverSMTransition(PortIndex, POF_TRANSCEIVER_TRIGGER_PRM_WRITTEN, pHDB);
                }
                else
                {
                    POF_TransceiverSMTransition(PortIndex, POF_TRANSCEIVER_TRIGGER_STOP, pHDB);
                }
            }
            else
            {
                POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_PRMChangePortStateMachine(): ERROR -> Wrong Trigger for State! State=0x%X, Trigger=0x%X",
                                      pPCtrl->PortModuleSm.State,
                                      Trigger);
                Response = POF_RSP_ERR_SEQUENCE;
                POF_SET_DETAIL_ERR(Response);
            }
        }
        break;

        default:
        {
            POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_PRMChangePortStateMachine(): ERROR -> Wrong State! State=0x%X",
                                  pPCtrl->PortModuleSm.State);
            Response = POF_RSP_ERR_SEQUENCE;
            POF_SET_DETAIL_ERR(Response);
        }
        break;
    }
    return Response;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_PRMChangePortState_IsPulled             +*/
/*+  Input                 :    POF_HDB_PTR_TYPE                 pHDB       +*/
/*+  Input                 :    LSA_UINT16                       PortIndex  +*/
/*+                                                                         +*/
/*+  Result                :    LSA_BOOL                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pHDB          : Pointer to User HDB                           (in)     +*/
/*+  PortIndex     : Describes which Port should be handeled       (in)     +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:    Returns LSA_TRUE in case of IsPulled State is set      +*/
/*+                  in the PRM Change Port State Machine,                  +*/
/*+                  otherwise LSA_FALSE                                    +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_BOOL POF_LOCAL_FCT_ATTR POF_PRMChangePortState_IsPulled(
    POF_HDB_PTR_TYPE              const pHDB,
    LSA_UINT16                    const PortIndex)
{
    POF_EDD_PCTRL_PTR_TYPE const pPCtrl = &pHDB->pPCtrl[PortIndex];
    return (pPCtrl->PortModuleSm.State == POF_PORT_MODULE_SM_STATE_PULLED);
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_PRMChangePortState_IsPlugged            +*/
/*+  Input                 :    POF_HDB_PTR_TYPE                 pHDB       +*/
/*+  Input                 :    LSA_UINT16                       PortIndex  +*/
/*+                                                                         +*/
/*+  Result                :    LSA_BOOL                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pHDB          : Pointer to User HDB                           (in)     +*/
/*+  PortIndex     : Describes which Port should be handeled       (in)     +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:    Returns LSA_TRUE in case of IsPlugged State is set     +*/
/*+                  in the PRM Change Port State Machine,                  +*/
/*+                  otherwise LSA_FALSE                                    +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_BOOL POF_LOCAL_FCT_ATTR POF_PRMChangePortState_IsPlugged(
    POF_HDB_PTR_TYPE              const pHDB,
    LSA_UINT16                    const PortIndex)
{
    POF_EDD_PCTRL_PTR_TYPE const pPCtrl = &pHDB->pPCtrl[PortIndex];
    return (pPCtrl->PortModuleSm.State == POF_PORT_MODULE_SM_STATE_PLUGGED);
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_PRMChangePortState_SequenceIsRunning    +*/
/*+  Input                 :    POF_HDB_PTR_TYPE                 pHDB       +*/
/*+  Output                :    LSA_UINT16                      *pPortIndex +*/
/*+                                                                         +*/
/*+  Result                :    LSA_BOOL                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pHDB          : Pointer to User HDB                           (in)     +*/
/*+  pPortIndex    : First Port in a ChangePort Sequence           (out)    +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:    Returns LSA_TRUE during the ChangePort Sequence        +*/
/*+                  in the PRM Change Port State Machine,                  +*/
/*+                  otherwise LSA_FALSE                                    +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_BOOL POF_LOCAL_FCT_ATTR POF_PRMChangePortState_IsAnySequenceRunning(
    POF_HDB_PTR_TYPE              const pHDB,
    LSA_UINT16                         *pPortIndex)
{
    LSA_UINT16 PortIndex;

    for (PortIndex = 0; PortIndex < pHDB->PortCnt; PortIndex++)
    {
        if (!POF_PRMChangePortState_IsPlugged(pHDB, PortIndex) && !POF_PRMChangePortState_IsPulled(pHDB, PortIndex))
        {
            if (pPortIndex)
            {
                *pPortIndex = PortIndex;
            }
            return LSA_TRUE;
        }
    }
    return LSA_FALSE;
}
/*---------------------- end [subroutine] ---------------------------------*/


/***************************************************************************/
/* F u n c t i o n:       POF_PrmChangePort_Check_Consistency()            */
/*                                                                         */
/* D e s c r i p t i o n:                                                  */
/*                                                                         */
/* A r g u m e n t s:                                                      */
/*                                                                         */
/* Return Value:          POF_RSP                                          */
/*                                                                         */
/***************************************************************************/
static  LSA_VOID POF_LOCAL_FCT_ATTR  POF_PrmChangePort_Check_Consistency( POF_HDB_PTR_TYPE       const pHDB,
                                                                          LSA_UINT16                   PortIndex)
{
    LSA_BOOL IsOptical = LSA_FALSE;
    LSA_BOOL IsPOF     = LSA_FALSE;
    LSA_BOOL IsSFP     = LSA_FALSE;

    if (POF_IS_POF_PORT(PortIndex, pHDB))
    {
        IsPOF     = LSA_TRUE;
        IsOptical = LSA_TRUE;
    }
    else if (POF_IS_SFP_PORT(PortIndex, pHDB))
    {
      if (pHDB->I2C[PortIndex].I2CAvailable == POF_I2C_AVAILABLE)
      {
        // I2C available try to read the HW values
        if (SFP_DMI_Validate_HW(PortIndex, pHDB))
        {
          IsSFP     = LSA_TRUE;
          IsOptical = LSA_TRUE;
        }
        // else: I2C is available but the port is not plugged => this means that further down the default parameters will be applied
      }
      else
      {
        //No I2C - assume it´s a SFP module anyway
        IsSFP     = LSA_TRUE;
        IsOptical = LSA_TRUE;
      }
    }
    else if (POF_IS_FIBER_PORT(PortIndex, pHDB))
    {
        IsOptical = LSA_TRUE;
    }

    POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_PrmChangePort_Check_Consistency, pHDB:0x%X PortIndex:%d", pHDB, PortIndex);
    pHDB->PortparamsNotApplicable[PortIndex] = POF_PRM_PORT_PARAMS_APPLICABLE;

    POF_PROGRAM_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_PrmChangePort_Check_Consistency, IsPOF:%d PortIndex:0x%X",
                         IsPOF, PortIndex);

    //Check PDPortFODataAdjust (0x8062):
    if (POF_RECORD_TYPE_DATA_ADJUST_8062 & pHDB->pPCtrl[PortIndex].RecordSet_A.RecordTypeWritten)
    {
        if (IsPOF)
        {
            LSA_UINT32 const Value = POF_NTOH_32(pHDB->pPCtrl[PortIndex].RecordSet_A.LN_FiberOpticType);

            /* check FiberOpticType, only SI-POF, NA=0.5 -> POF or SI-PCF, NA=0.36 -> HCS allowed */
            if ((Value != 0x00000004UL) && (Value != 0x00000005UL))
            {
                POF_PROGRAM_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_PrmChangePort_Check_Consistency, Invalid FiberOpticType:0x%x for POF, PortIndex:0x%X",
                                 Value, PortIndex);
                pHDB->PortparamsNotApplicable[PortIndex] = POF_PRM_PORT_PARAMS_NOT_APPLICABLE;
            }
        }
        else if (IsOptical)
        {
            // record allowed for SFP and other optical tranceivers but are not checked
        }
        else
        {
            POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_PrmChangePort_Check_Consistency, PDPortFODataAdjust for non-POF, PortIndex:0x%X",
                                PortIndex);
            pHDB->PortparamsNotApplicable[PortIndex] = POF_PRM_PORT_PARAMS_NOT_APPLICABLE;
        }
    }

    //Check PDPortFODataCheck (0x8061):
    // This Record was only written (in PRM write), if I2C is available!!!
    if (pHDB->pPCtrl[PortIndex].RecordSet_A.MRCheckEnable || pHDB->pPCtrl[PortIndex].RecordSet_A.MDCheckEnable)
    {
        //PDPortFODataCheck makes sense without PDPortFODataAdjust!! PDPortFODataCheck contains all needed information to perform the checks!
        if (!IsPOF)
        {
            POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_PrmChangePort_Check_Consistency, PDPortFODataCheck for non-POF, PortIndex:0x%X",
                                PortIndex);
            pHDB->PortparamsNotApplicable[PortIndex] = POF_PRM_PORT_PARAMS_NOT_APPLICABLE;
        }
    }

    //Check PDPortSFPDataCheck (0x8063):
    // This Record was only written (in PRM write), if I2C is available!!!
    if (POF_RECORD_TYPE_SFP_DATA_CHECK_8063 & pHDB->pPCtrl[PortIndex].RecordSet_A.RecordTypeWritten)
    {
        if (!(IsSFP || IsOptical))
        {
            if (POF_PRM_PORT_IS_MODULAR == pHDB->IsModularPort[PortIndex])
            {
                POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_PrmState_Check_Consistency, PDPortSFPDataCheck for non-POF, PortIndex:0x%X",
                                     PortIndex);
                pHDB->PortparamsNotApplicable[PortIndex] = POF_PRM_PORT_PARAMS_NOT_APPLICABLE;
            }
        }
    }

    POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_PrmChangePort_Check_Consistency, pHDB:0x%X", pHDB);
}
/*---------------------- end [subroutine] ---------------------------------*/


/*=============================================================================
 * function name:   POF_SetPrmHeader()
 *
 * parameters:
 *
 * return value:    LSA_VOID
 *
 * description :    
 *===========================================================================*/
static  LSA_VOID  POF_LOCAL_FCT_ATTR  POF_SetPrmHeader( POF_HDB_PTR_TYPE        const  pHDB,
                                                        LSA_UINT32              const  RecordIndex,
                                                        POF_UPPER_MEM_PTR_TYPE  const  pRecordData )
{
  POF_UPPER_PRM_RECORD_HEADER_PTR_TYPE  pHeader;

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_SetPrmHeader, RecordIndex:0x%X", RecordIndex);

  pHeader = (POF_UPPER_PRM_RECORD_HEADER_PTR_TYPE)pRecordData; 

  switch (RecordIndex)
  {
    case POF_PRM_FO_DATA_REAL_INDEX:
    {
      POF_UPPER_PRM_FO_DATA_REAL_PTR_TYPE  const  pFODataReal = (POF_UPPER_PRM_FO_DATA_REAL_PTR_TYPE)pRecordData;

      pHeader->N_BlockType              = POF_HTONT_16(POF_PRM_FO_DATA_REAL_BLOCKTYPE_1);
      pHeader->N_BlockLength            = POF_HTONT_16(POF_PRM_FO_DATA_REAL_BLOCK_LENGTH_BLOCK_1);
      pHeader->N_BlockVersionHigh       = POF_PRM_BLOCK_VERSION_HIGH;
      pHeader->N_BlockVersionLow        = POF_PRM_BLOCK_VERSION_LOW;
      pHeader->N_Padding1_1             = 0;
      pHeader->N_Padding1_2             = 0;

      pFODataReal->N_BlockType3         = POF_HTONT_16(POF_PRM_FO_DATA_REAL_BLOCKTYPE_3);
      pFODataReal->N_BlockLength3       = POF_HTONT_16(POF_PRM_FO_DATA_REAL_BLOCK_LENGTH_BLOCK_3);
      pFODataReal->N_BlockVersionHigh3  = POF_PRM_BLOCK_VERSION_HIGH;
      pFODataReal->N_BlockVersionLow3   = POF_PRM_BLOCK_VERSION_LOW;
      pFODataReal->N_Padding3_1         = 0;
      pFODataReal->N_Padding3_2         = 0;
    } break;

    case POF_PRM_FO_DATA_CHECK_INDEX:
    {
      pHeader->N_BlockType              = POF_HTONT_16(POF_PRM_FO_DATA_CHECK_BLOCKTYPE);
      pHeader->N_BlockLength            = POF_HTONT_16(POF_PRM_FO_DATA_CHECK_LENGTH - 4UL);
      pHeader->N_BlockVersionHigh       = POF_PRM_BLOCK_VERSION_HIGH;
      pHeader->N_BlockVersionLow        = POF_PRM_BLOCK_VERSION_LOW;
      pHeader->N_Padding1_1             = 0;
      pHeader->N_Padding1_2             = 0;
    } break;

    case POF_PRM_FO_DATA_ADJUST_INDEX:
    {
      pHeader->N_BlockType              = POF_HTONT_16(POF_PRM_FO_DATA_ADJUST_BLOCKTYPE);
      pHeader->N_BlockLength            = POF_HTONT_16(POF_PRM_FO_DATA_ADJUST_LENGTH - 4UL);
      pHeader->N_BlockVersionHigh       = POF_PRM_BLOCK_VERSION_HIGH;
      pHeader->N_BlockVersionLow        = POF_PRM_BLOCK_VERSION_LOW;
      pHeader->N_Padding1_1             = 0;
      pHeader->N_Padding1_2             = 0;
    } break;

    case POF_PRM_SFP_DATA_CHECK_INDEX:
    {
      pHeader->N_BlockType              = POF_HTONT_16(POF_PRM_SFP_DATA_CHECK_BLOCKTYPE);
      pHeader->N_BlockLength            = POF_HTONT_16(POF_PRM_SFP_DATA_CHECK_LENGTH - 4UL);
      pHeader->N_BlockVersionHigh       = POF_PRM_BLOCK_VERSION_HIGH;
      pHeader->N_BlockVersionLow        = POF_PRM_BLOCK_VERSION_LOW;
      pHeader->N_Padding1_1             = 0;
      pHeader->N_Padding1_2             = 0;
    } break;

    case POF_PRM_FO_DATA_TRA_IND_TEST_INDEX:
    {
      pHeader->N_BlockType              = POF_HTONT_16(POF_PRM_FO_DATA_TRA_IND_TEST_BLOCKTYPE);
      pHeader->N_BlockLength            = POF_HTONT_16(POF_PRM_FO_DATA_TRA_IND_TEST_LENGTH - 4UL);
      pHeader->N_BlockVersionHigh       = POF_PRM_BLOCK_VERSION_HIGH;
      pHeader->N_BlockVersionLow        = POF_PRM_BLOCK_VERSION_LOW;
      pHeader->N_Padding1_1             = 0;
      pHeader->N_Padding1_2             = 0;
    } break;

    case POF_PRM_SFP_DIAG_TEST_INDEX:
    {
      pHeader->N_BlockType              = POF_HTONT_16(POF_PRM_SFP_DIAG_TEST_BLOCKTYPE);
      pHeader->N_BlockLength            = POF_HTONT_16(POF_PRM_SFP_DIAG_TEST_LENGTH - 4UL);
      pHeader->N_BlockVersionHigh       = POF_PRM_BLOCK_VERSION_HIGH;
      pHeader->N_BlockVersionLow        = POF_PRM_BLOCK_VERSION_LOW;
      pHeader->N_Padding1_1             = 0;
      pHeader->N_Padding1_2             = 0;
    } break;

    default:
    {
      POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_SetPrmHeader, RecordIndex:0x%X", RecordIndex);
      LSA_UNUSED_ARG(pHDB); //satisfy lint!
      POF_FATAL("POF_SetPrmHeader", POF_FATAL_ERR_PRM_READ_RECORD_INDEX, 0, 0);
    } break;
  }

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_SetPrmHeader, RecordIndex:0x%X", RecordIndex);
}
/*---------------------- end [subroutine] ---------------------------------*/


/*=============================================================================
 * function name:   POF_SetPrmHeaderShort()
 *
 * parameters:
 *
 * return value:    LSA_VOID
 *
 * description :    
 *===========================================================================*/
static  LSA_VOID  POF_LOCAL_FCT_ATTR  POF_SetPrmHeaderShort( POF_HDB_PTR_TYPE   const  pHDB,
                                                        LSA_UINT32              const  RecordIndex,
                                                        POF_UPPER_MEM_PTR_TYPE  const  pRecordData )
{
  POF_UPPER_PRM_RECORD_HEADER_PTR_TYPE  pHeader;

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_SetPrmHeaderShort, RecordIndex:0x%X", RecordIndex);

  pHeader = (POF_UPPER_PRM_RECORD_HEADER_PTR_TYPE)pRecordData; 

  switch (RecordIndex)
  {
    case POF_PRM_FO_DATA_REAL_INDEX:
    {
      pHeader->N_BlockType              = POF_HTONT_16(POF_PRM_FO_DATA_REAL_BLOCKTYPE_1);
      pHeader->N_BlockLength            = POF_HTONT_16(POF_PRM_FO_DATA_REAL_SHORT_LENGTH - 4UL);
      pHeader->N_BlockVersionHigh       = POF_PRM_BLOCK_VERSION_HIGH;
      pHeader->N_BlockVersionLow        = POF_PRM_BLOCK_VERSION_LOW;
      pHeader->N_Padding1_1             = 0;
      pHeader->N_Padding1_2             = 0;
    } break;

    default:
    {
      POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_SetPrmHeaderShort, RecordIndex:0x%X", RecordIndex);
      LSA_UNUSED_ARG(pHDB); //satisfy lint!
      POF_FATAL("POF_SetPrmHeaderShort", POF_FATAL_ERR_PRM_READ_RECORD_INDEX, 0, 0);
    } break;
  }

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_SetPrmHeaderShort, RecordIndex:0x%X", RecordIndex);
}
/*---------------------- end [subroutine] ---------------------------------*/


/*****************************************************************************/
/*  end of file pof_prm_req.c                                                */
/*****************************************************************************/
