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
/*  F i l e               &F: pof_prm_state.c                           :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  POF-global functions                             */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
/*                                                                           */
/*****************************************************************************/

/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/
#define LTRC_ACT_MODUL_ID  12
#define POF_MODULE_ID      LTRC_ACT_MODUL_ID

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/
#include "pof_inc.h"            /* POF headerfiles */
#include "pof_dmi_type.h"
#include "pof_prm_type.h"
#include "pof_int.h"            /* internal header */
#include "pof_glb.h"
#include "pof_prm_req.h"
#include "pof_prm_state_diag.h"
#include "pof_prm_state.h"
#include "pof_dmi.h"

POF_FILE_SYSTEM_EXTENSION(POF_MODULE_ID)

/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/

/*===========================================================================*/
/*                                global Variables                           */
/*===========================================================================*/

static POF_SFP_PORT_DIAGNOSTIC_TYPE  sgSFP_Diagnostic = 
{
    0               // ActivationMask;
  , {               // SFP_DiagnosticChecks []
        {// Temperature
            SFP_PROPERTY_STATE_NORMAL   // state 
          , SFP_PROPERTY_STATE_NORMAL   // LastReportedState 
            // ExtChannelErrorType                              DiagEventSeverity,             AlarmAddress,               CheckBitMask,             
          , {  POF_PRM_EXT_CHANNEL_TEMPERATURE_HIGH_ERROR_TYPE, POF_PRM_CH_PROP_MAIN_DEMANDED, POF_SFP_ALARM_FLAGS_1,      SFP_TEMPERATURE_HIGH_BIT}
          , {  0,                                               0,                             POF_SFP_ALARM_FLAGS_1,      SFP_TEMPERATURE_LOW_BIT }
        }
      , {// TX_Bias
            SFP_PROPERTY_STATE_NORMAL   // state
          , SFP_PROPERTY_STATE_NORMAL   // LastReportedState 
            // ExtChannelErrorType                              DiagEventSeverity,             AlarmAddress,               CheckBitMask,           
          , {  POF_PRM_EXT_CHANNEL_TX_BIAS_HIGH_ERROR_TYPE,     POF_PRM_CH_PROP_MAIN_DEMANDED, POF_SFP_ALARM_FLAGS_1,      SFP_TX_BIAS_HIGH_BIT    }
          , {  POF_PRM_EXT_CHANNEL_TX_BIAS_LOW_ERROR_TYPE,      POF_PRM_CH_PROP_MAIN_DEMANDED, POF_SFP_ALARM_FLAGS_1,      SFP_TX_BIAS_LOW_BIT     }
        }
      , {// TX_Power
            SFP_PROPERTY_STATE_NORMAL   // state
          , SFP_PROPERTY_STATE_NORMAL   // LastReportedState 
            // ExtChannelErrorType                              DiagEventSeverity,             AlarmAddress,               CheckBitMask,           
          , {  POF_PRM_EXT_CHANNEL_TX_POWER_HIGH_ERROR_TYPE,    POF_PRM_CH_PROP_MAIN_DEMANDED, POF_SFP_ALARM_FLAGS_1,      SFP_TX_POWER_HIGH_BIT   }
          , {  POF_PRM_EXT_CHANNEL_TX_POWER_LOW_ERROR_TYPE,     POF_PRM_CH_PROP_MAIN_DEMANDED, POF_SFP_ALARM_FLAGS_1,      SFP_TX_POWER_LOW_BIT    }
        }
      , {// RX_Power
            SFP_PROPERTY_STATE_NORMAL   // state
          , SFP_PROPERTY_STATE_NORMAL   // LastReportedState 
            // ExtChannelErrorType                              DiagEventSeverity,             AlarmAddress,               CheckBitMask,           
          , {  POF_PRM_EXT_CHANNEL_RX_POWER_HIGH_ERROR_TYPE,    POF_PRM_CH_PROP_MAIN_DEMANDED, POF_SFP_ALARM_FLAGS_2,      SFP_RX_POWER_HIGH_BIT   }
          , {  POF_PRM_EXT_CHANNEL_RX_POWER_LOW_ERROR_TYPE,     POF_PRM_CH_PROP_MAIN_DEMANDED, POF_SFP_ALARM_FLAGS_2,      SFP_RX_POWER_LOW_BIT    }
        }
      , {// TX_FAULT
            SFP_PROPERTY_STATE_NORMAL   // state
          , SFP_PROPERTY_STATE_NORMAL   // LastReportedState 
            // ExtChannelErrorType                              DiagEventSeverity,             AlarmAddress,               CheckBitMask,           
          , {  POF_PRM_EXT_CHANNEL_TX_FAULT_ERROR_TYPE,         POF_PRM_CH_PROP_ERROR,         POF_SFP_STATUS_AND_CONTROL, SFP_TX_FAULT_BIT        }
          , {  0,                                               0,                             POF_SFP_STATUS_AND_CONTROL, 0                       }
        }
      , {// RX_LOSS
            SFP_PROPERTY_STATE_NORMAL   // state
          , SFP_PROPERTY_STATE_NORMAL   // LastReportedState 
            // ExtChannelErrorType                              DiagEventSeverity,             AlarmAddress,               CheckBitMask,           
          , {  POF_PRM_EXT_CHANNEL_RX_LOSS_ERROR_TYPE,          POF_PRM_CH_PROP_ERROR,         POF_SFP_STATUS_AND_CONTROL, SFP_RX_LOSS_BIT         }
          , {  0,                                               0,                             POF_SFP_STATUS_AND_CONTROL, 0                       }
        }
    }
};


/*===========================================================================*/
/*                            local function declaration                     */
/*===========================================================================*/
static POF_RSP    POF_LOCAL_FCT_ATTR  POF_PrmLastState_Prepare       (POF_UPPER_RQB_PTR_TYPE const pRQB,
                                                                      POF_HDB_PTR_TYPE       const pHDB,
                                                                      LSA_OPCODE_TYPE        const AppearOpCode );

static POF_RSP    POF_LOCAL_FCT_ATTR  POF_PrmLastState_Write         (POF_UPPER_RQB_PTR_TYPE const pRQB,
                                                                      POF_HDB_PTR_TYPE       const pHDB,
                                                                      LSA_OPCODE_TYPE        const AppearOpCode );

static POF_RSP    POF_LOCAL_FCT_ATTR  POF_PrmLastState_End           (POF_UPPER_RQB_PTR_TYPE const pRQB,
                                                                      POF_HDB_PTR_TYPE       const pHDB,
                                                                      LSA_OPCODE_TYPE        const AppearOpCode );

static LSA_VOID   POF_LOCAL_FCT_ATTR  POF_PrmState_Move_B_to_A       (POF_HDB_PTR_TYPE       const pHDB );

static LSA_VOID   POF_LOCAL_FCT_ATTR  POF_PrmState_Delete_all_B      (POF_HDB_PTR_TYPE       const pHDB );

static POF_RSP    POF_LOCAL_FCT_ATTR  POF_PrmState_Check_Consistency (POF_UPPER_RQB_PTR_TYPE const pRQB,
                                                                      POF_HDB_PTR_TYPE       const pHDB );
                                                                   
/*===========================================================================*/
/*                                 Macros                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/

/***************************************************************************/
/* F u n c t i o n:       POF_PrmState_Request()                           */
/*                                                                         */
/* D e s c r i p t i o n:                                                  */
/*                                                                         */
/* A r g u m e n t s:                                                      */
/*                                                                         */
/* Return Value:          POF_RSP                                          */
/*                                                                         */
/***************************************************************************/
POF_RSP  POF_LOCAL_FCT_ATTR  POF_PrmState_Request( POF_UPPER_RQB_PTR_TYPE  const  pRQB,
                                                   POF_HDB_PTR_TYPE        const  pHDB,
                                                   LSA_OPCODE_TYPE         const  AppearOpCode )
{
  POF_RSP  Response;

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_PrmState_Request, AppearOpCode:0x%X pHDB:0x%X", AppearOpCode, pHDB);

  switch (AppearOpCode)
  {
    case POF_OPC_PRM_WRITE:      
    {
      POF_PRM_INIT_DETAIL_ERR(&pHDB->PrmDetailErr);

      Response = pHDB->PrmStateFct(pRQB, pHDB, AppearOpCode); 

      if (LSA_FALSE != pHDB->PrmDetailErr.ErrValid)
      {
        POF_RQB_PRM_WRITE_SET(&pRQB->args.PrmWrite, pHDB->PrmDetailErr.ErrOffset);
      }
      else if (EDD_STS_OK != Response)
      {
        POF_RQB_PRM_WRITE_SET(&pRQB->args.PrmWrite, POF_PRM_ERR_OFFSET_DEFAULT);
      }
      return Response;
    }

    case POF_OPC_PRM_END:        
    {
      LSA_UINT16 PortIndex;
      POF_PRM_INIT_DETAIL_ERR(&pHDB->PrmDetailErr);
      if (POF_PRMChangePortState_IsAnySequenceRunning(pHDB, &PortIndex))
      {
        POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "    POF_PrmState_Request(): ERROR -> POF_OPC_PRM_END during a ChangePort Sequence at Port %d", PortIndex);
        Response = POF_RSP_ERR_SEQUENCE;
        POF_SET_DETAIL_ERR(Response);
      }
      else
      {
        Response = pHDB->PrmStateFct(pRQB, pHDB, AppearOpCode);

        if (LSA_FALSE != pHDB->PrmDetailErr.ErrValid)
        {
          POF_RQB_PRM_END_SET(&pRQB->args.PrmEnd, pHDB->PrmDetailErr.ErrPortID,
                                                  pHDB->PrmDetailErr.ErrIndex,
                                                  pHDB->PrmDetailErr.ErrOffset,
                                                  pHDB->PrmDetailErr.ErrFault?POF_PRM_ERR_FAULT_WRONG_OR_TOO_MANY_PARAMS:POF_PRM_ERR_FAULT_NO_OR_TOO_LESS_PARAMS);
        }
        else if (EDD_STS_OK != Response)
        {
          POF_RQB_PRM_END_SET(&pRQB->args.PrmEnd, POF_PRM_ERR_PORT_ID_DEFAULT,
                                                  POF_PRM_ERR_INDEX_DEFAULT,
                                                  POF_PRM_ERR_OFFSET_DEFAULT,
                                                  POF_PRM_ERR_FAULT_DEFAULT);
        }
        else
        {
          LSA_UINT32  j;
          for (j = 0; j < EDD_CFG_MAX_PORT_CNT; ++j) 
          {
            pRQB->args.PrmEnd.PortparamsNotApplicable[j] = pHDB->PortparamsNotApplicable[j]; 
          }
        }
      }
      return Response;
    }

    case POF_OPC_PRM_PREPARE:    
    {
      POF_MEMSET_LOCAL(&pHDB->PrmDetailErr, 0, sizeof(POF_PRM_DETAIL_ERR));

      Response = pHDB->PrmStateFct(pRQB, pHDB, AppearOpCode); 
      return Response;
    }

    case POF_OPC_PRM_COMMIT:
    {
      LSA_UINT16 PortIndex;
      if (POF_PRMChangePortState_IsAnySequenceRunning(pHDB, &PortIndex))
      {
        POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "    POF_PrmState_Request(): ERROR -> POF_OPC_PRM_COMMIT during a ChangePort Sequence at Port %d", PortIndex);
        Response = POF_RSP_ERR_SEQUENCE;
        POF_SET_DETAIL_ERR(Response);
      }
      else
      {
        Response = pHDB->PrmStateFct(pRQB, pHDB, AppearOpCode);
      }
      return Response;
    }

    case POF_OPC_PRM_CHANGE_PORT:
    {
      if (pHDB->PrmStateFct == POF_PrmLastState_End)
      {
        POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "    POF_PrmState_Request(): ERROR -> POF_OPC_PRM_CHANGE_PORT between PRM_END and PRM_COMMIT");
        Response = POF_RSP_ERR_SEQUENCE;
        POF_SET_DETAIL_ERR(Response);
      }
      else
      {
        Response = POF_RequestPrmChangePort(pRQB, pHDB);
      }
      return Response;
    }

    default:   
    {
      POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_PrmState_Request, AppearOpCode:0x%X", AppearOpCode);
      POF_FATAL("POF_PrmState_Request, AppearOpCode:", POF_FATAL_ERR_PRM_DIAG_STATE, AppearOpCode, 0);
      //lint -unreachable
      return POF_RSP_ERR_OPCODE;
    }
  }
}
/*---------------------- end [subroutine] ---------------------------------*/


/***************************************************************************/
/* F u n c t i o n:       POF_PrmState_PrmIni()                            */
/*                                                                         */
/* D e s c r i p t i o n:                                                  */
/*                                                                         */
/* A r g u m e n t s:                                                      */
/*                                                                         */
/* Return Value:          LSA_VOID                                         */
/*                                                                         */
/***************************************************************************/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_PrmState_PrmIni( POF_HDB_PTR_TYPE  const  pHDB )
{
  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_PrmState_PrmIni, pHDB:0x%X", pHDB);

  pHDB->PrmStateFct = (POF_PRM_STATE_FCT)POF_PrmLastState_Prepare;

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_PrmState_PrmIni, pHDB:0x%X", pHDB);
}
/*---------------------- end [subroutine] ---------------------------------*/


/***************************************************************************/
/* F u n c t i o n:       POF_PrmLastState_Prepare()                       */
/*                                                                         */
/* D e s c r i p t i o n:                                                  */
/*                                                                         */
/* A r g u m e n t s:                                                      */
/*                                                                         */
/* Return Value:          POF_RSP                                          */
/*                                                                         */
/***************************************************************************/
static  POF_RSP  POF_LOCAL_FCT_ATTR  POF_PrmLastState_Prepare( POF_UPPER_RQB_PTR_TYPE  const  pRQB, 
                                                               POF_HDB_PTR_TYPE        const  pHDB,
                                                               LSA_OPCODE_TYPE         const  AppearOpCode )
{
  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_PrmLastState_Prepare, AppearOpCode:0x%X", AppearOpCode);

  POF_PrmState_Delete_all_B(pHDB);

  switch (AppearOpCode)
  {
    case POF_OPC_PRM_PREPARE: 
    {
      return POF_RSP_OK; 
    }

    case POF_OPC_PRM_WRITE:   
    {
      pHDB->PrmStateFct = POF_PrmLastState_Write; /* next state WRITE */
      return POF_RequestPrmWrite(pRQB, pHDB);
    }

    case POF_OPC_PRM_END:        
    {
      pHDB->PrmStateFct = POF_PrmLastState_End;   /* next state END */
      return POF_PrmState_Check_Consistency(pRQB, pHDB); 
    }

    case POF_OPC_PRM_COMMIT: 
    {
      POF_SET_DETAIL_ERR(POF_ERR_SEQUENCE);
      return POF_RSP_ERR_SEQUENCE; 
    }

    default:
    {
      POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_PrmLastState_Prepare AppearOpCode: 0x%X", AppearOpCode);
      POF_FATAL("POF_PrmLastState_Prepare, AppearOpCode", POF_FATAL_ERR_PRM_DIAG_STATE, AppearOpCode, 0);
      //lint -unreachable
      return POF_RSP_ERR_OPCODE;
    }
  }  
}
/*---------------------- end [subroutine] ---------------------------------*/


/***************************************************************************/
/* F u n c t i o n:       POF_PrmLastState_Write()                         */
/*                                                                         */
/* D e s c r i p t i o n:                                                  */
/*                                                                         */
/* A r g u m e n t s:                                                      */
/*                                                                         */
/* Return Value:          POF_RSP                                          */
/*                                                                         */
/***************************************************************************/
static  POF_RSP  POF_LOCAL_FCT_ATTR  POF_PrmLastState_Write( POF_UPPER_RQB_PTR_TYPE  const  pRQB, 
                                                             POF_HDB_PTR_TYPE        const  pHDB,
                                                             LSA_OPCODE_TYPE         const  AppearOpCode )
{
  POF_RSP  Response;

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_PrmLastState_Write, AppearOpCode:0x%X", AppearOpCode);

  Response = POF_RSP_OK;

  switch (AppearOpCode)
  {
    case POF_OPC_PRM_PREPARE: 
    {
      pHDB->PrmStateFct = POF_PrmLastState_Prepare;     /* next state PREPARE */
      return POF_RSP_OK; 
    }

    case POF_OPC_PRM_WRITE:   
    {
      Response = POF_RequestPrmWrite(pRQB, pHDB);
      return Response;
    }

    case POF_OPC_PRM_END:        
    { 
      pHDB->PrmStateFct = POF_PrmLastState_End;         /* next state END */   
      Response = POF_PrmState_Check_Consistency(pRQB, pHDB); 
      return Response;
    }

    case POF_OPC_PRM_COMMIT: 
    {
      POF_SET_DETAIL_ERR(POF_RSP_ERR_SEQUENCE);
      return POF_RSP_ERR_SEQUENCE; 
    }

    default: 
    {
      POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_PrmLastState_Write, AppearOpCode:0x%X", AppearOpCode);
      POF_FATAL("POF_PrmLastState_Write, AppearOpCode:", POF_FATAL_ERR_PRM_DIAG_STATE, AppearOpCode, 0);
      //lint -unreachable
      return POF_RSP_ERR_OPCODE;
    }
  }  
}
/*---------------------- end [subroutine] ---------------------------------*/


/***************************************************************************/
/* F u n c t i o n:       POF_PrmLastState_End()                           */
/*                                                                         */
/* D e s c r i p t i o n:                                                  */
/*                                                                         */
/* A r g u m e n t s:                                                      */
/*                                                                         */
/* Return Value:          POF_RSP                                          */
/*                                                                         */
/***************************************************************************/
static  POF_RSP  POF_LOCAL_FCT_ATTR  POF_PrmLastState_End( POF_UPPER_RQB_PTR_TYPE  const  pRQB, 
                                                           POF_HDB_PTR_TYPE        const  pHDB,
                                                           LSA_OPCODE_TYPE         const  AppearOpCode )
{
  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_PrmLastState_End, AppearOpCode:0x%X", AppearOpCode);

  switch (AppearOpCode)
  {
    case POF_OPC_PRM_PREPARE: 
    {
      pHDB->PrmStateFct = POF_PrmLastState_Prepare; /* next state PREPARE */
      return POF_RSP_OK; 
    }

    case POF_OPC_PRM_END:        
    case POF_OPC_PRM_WRITE:   
    {
      POF_SET_DETAIL_ERR(POF_RSP_ERR_SEQUENCE);
      return POF_RSP_ERR_SEQUENCE; 
    }

    case POF_OPC_PRM_COMMIT:                  /* next state PREPARE */
    {
      POF_RQB_PRM_COMMIT_APPLYDEFAULTPORTPARAMS_GET(pRQB, pHDB->ApplyDefaultPortparams, EDD_CFG_MAX_PORT_CNT);
      POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"--- POF_PrmLastState_End() AppearOpCode:POF_OPC_PRM_COMMIT");

      pHDB->PrmStateFct = POF_PrmLastState_Prepare;
      POF_PrmState_Move_B_to_A(pHDB);
      return POF_RSP_OK;
    }

    default:  
    {
      POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_PrmLastState_End, AppearOpCode:0x%X", AppearOpCode);
      LSA_UNUSED_ARG(pRQB); //satisfy lint!
      POF_FATAL("POF_PrmLastState_End, AppearOpCode:", POF_FATAL_ERR_PRM_DIAG_STATE, AppearOpCode, 0);
      //lint -unreachable
      return POF_RSP_ERR_OPCODE;
    }
  }  
}
/*---------------------- end [subroutine] ---------------------------------*/


/***************************************************************************/
/* F u n c t i o n:       POF_PrmState_Move_B_to_A()                       */
/*                                                                         */
/* D e s c r i p t i o n:                                                  */
/*                                                                         */
/* A r g u m e n t s:                                                      */
/*                                                                         */
/* Return Value:          LSA_VOID                                         */
/*                                                                         */
/***************************************************************************/
static  LSA_VOID  POF_LOCAL_FCT_ATTR  POF_PrmState_Move_B_to_A( POF_HDB_PTR_TYPE  const  pHDB )
{
  LSA_UINT16  PortIndex;

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_PrmState_Move_B_to_A, pHDB:0x%X", pHDB);
  
  for (PortIndex = 0; PortIndex < pHDB->PortCnt; PortIndex++)
  {
    POF_EDD_PCTRL_PTR_TYPE  const  pPCtrl = &pHDB->pPCtrl[PortIndex];
    LSA_UINT32                     TempValue;
    LSA_UINT32                     SlidingBit;
    LSA_UINT32                     PropIndex;

    POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"--- POF_PrmState_Move_B_to_A() ApplyDefaultPortparams[Port%d]=%d",
                          PortIndex, pHDB->ApplyDefaultPortparams[PortIndex]);

    pPCtrl->RecordSet_A = pPCtrl->RecordSet_B;

    /* reset statemachine to state "start" */
    pPCtrl->DiagStateFct       = (PRM_STATE_DIAG_FCT)      POF_PRM_StateDiag_Start;

    /* reset threshold */
    pPCtrl->TimeThreshold.bMaintenanceRequired = LSA_FALSE;
    pPCtrl->TimeThreshold.bMaintenanceDemanded = LSA_FALSE;
    pPCtrl->TimeThreshold.bError               = LSA_FALSE;

    /* reset lost PrmIndication */
    pPCtrl->LostPrmInd.bLost                   = LSA_FALSE;
    pPCtrl->LostPrmInd.AppearEvent             = POF_PRM_EV_GOOD;
    pPCtrl->LostPrmInd.DisappearEvent          = POF_PRM_EV_GOOD;
    pPCtrl->LostPrmInd.AddValue                = 0;

    POF_MEMCOPY_COMMON(&pPCtrl->SFP_Diagnostic, &sgSFP_Diagnostic, sizeof(sgSFP_Diagnostic));
    pPCtrl->SFP_Diagnostic.ActivationMask  = POF_NTOH_32( pPCtrl->RecordSet_A.SFP_MaintenanceDemandedAdminStatus);
    TempValue                              = POF_NTOH_32( pPCtrl->RecordSet_A.SFP_ErrorAdminStatus);
    pPCtrl->SFP_Diagnostic.ActivationMask |= (TempValue << POF_TX_FAULT);       

    SlidingBit = 1;

    for (PropIndex = 0; PropIndex < POF_SFP_NUMBER_CHECK_DIAGNOSTICS; ++PropIndex, SlidingBit <<= 1)
    {
      if (SlidingBit & pPCtrl->SFP_Diagnostic.ActivationMask)
      {
        pPCtrl->SFP_Diagnostic.SFP_DiagnosticChecks[PropIndex].State = SFP_PROPERTY_STATE_NORMAL;
      }
      else
      {
        pPCtrl->SFP_Diagnostic.SFP_DiagnosticChecks[PropIndex].State = SFP_PROPERTY_STATE_ERROR;
      }
    }
    
    /* reset Flags, writing Threshold could during 2 seconds */
    /* create a good diagnostic for the first time           */
    /* see POF_PRM_StateDiag_Validate_TimeOut                */
    pPCtrl->DMIMap.A2_Mirror.u.ElementPOF.Flagbit_113 = 0; 
    pPCtrl->DMIMap.A2_Mirror.u.ElementPOF.Flagbit_117 = 0;

    if (pPCtrl->SFP_Diagnostic.ActivationMask)
    {
      /* create a good diagnostic for the first time           */
      POF_MEMSET_LOCAL(&pPCtrl->DMIMap.A2_Mirror.u.ElementSFP.StatusControl, 0, 4);
    }

    //transfer info about modular/compact port from PRM_END
    pHDB->IsModularPort[PortIndex] = pHDB->IsModularPortPrmEnd[PortIndex];

    /* Trigger Transceiver State Machine */
    if (pHDB->I2C[PortIndex].I2CAvailable == POF_I2C_AVAILABLE)
    {
        //Only start polling if needed (diagnostic record(s) both available and usable)
        POF_TransceiverSMTransition(PortIndex, POF_TRANSCEIVER_TRIGGER_START, pHDB);
        POF_TransceiverSMTransition(PortIndex, POF_TRANSCEIVER_TRIGGER_PRM_WRITTEN, pHDB);
    }
    else
    {
        POF_TransceiverSMTransition(PortIndex, POF_TRANSCEIVER_TRIGGER_STOP, pHDB);
    }

  }

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_PrmState_Move_B_to_A, pHDB:0x%X", pHDB);
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_PrmState_Delete_all_B()                 +*/
/*+  Input/Output          :    POF_UPPER_RQB_PTR_TYPE     pRQB             +*/
/*+                        :    POF_HDB_PTR_TYPE           pHDB             +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Request to get config parameters from POF                 +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static  LSA_VOID  POF_LOCAL_FCT_ATTR  POF_PrmState_Delete_all_B( POF_HDB_PTR_TYPE  const  pHDB )
{
  POF_EDD_PCTRL_PTR_TYPE  pPCtrl;
  LSA_UINT16              PortIndex;

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_PrmState_Delete_all_B, pHDB:0x%X", pHDB);

  for (PortIndex = 0; PortIndex < pHDB->PortCnt; PortIndex++)
  {
    pPCtrl = &pHDB->pPCtrl[PortIndex];

    pPCtrl->RecordSet_B.LN_FiberOpticType      = 0;         /* No fiber type adjusted */
    pPCtrl->RecordSet_B.LN_FiberOpticCableType = 0;         /* no cable specified     */

    pPCtrl->RecordSet_B.LN_MRPowerBudget       = 0;         /* CheckEnable + Threshold */
    pPCtrl->RecordSet_B.LN_MDPowerBudget       = 0;         /* CheckEnable + Threshold */
    pPCtrl->RecordSet_B.LN_ErrorPowerBudget    = 0;         /* CheckEnable + Threshold */

    pPCtrl->RecordSet_B.MRPowerBudget_0_1_dB   = 0;         /* PowerBudget in 0_1dB */
    pPCtrl->RecordSet_B.MDPowerBudget_0_1_dB   = 0;         /* PowerBudget in 0_1dB */

    pPCtrl->RecordSet_B.MRCheckEnable          = LSA_FALSE; /* CheckEnable OFF */
    pPCtrl->RecordSet_B.MDCheckEnable          = LSA_FALSE; /* CheckEnable OFF */
    pPCtrl->RecordSet_B.ErrorCheckEnable       = LSA_FALSE; /* CheckEnable OFF */

    pPCtrl->RecordSet_B.MRThresholdWritten     = LSA_FALSE;
    pPCtrl->RecordSet_B.MDThresholdWritten     = LSA_FALSE;
    
    pPCtrl->RecordSet_B.SFP_MaintenanceDemandedAdminStatus = 0UL;
    pPCtrl->RecordSet_B.SFP_ErrorAdminStatus               = 0UL;

    pPCtrl->RecordSet_B.RecordTypeWritten = POF_RECORD_TYPE_NONE; /* No Record written */
  }

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_PrmState_Delete_all_B, pHDB:0x%X", pHDB);
}
/*---------------------- end [subroutine] ---------------------------------*/


/***************************************************************************/
/* F u n c t i o n:       POF_PrmState_Check_Consistency()                 */
/*                                                                         */
/* D e s c r i p t i o n:                                                  */
/*                                                                         */
/* A r g u m e n t s:                                                      */
/*                                                                         */
/* Return Value:          POF_RSP                                          */
/*                                                                         */
/***************************************************************************/
static  POF_RSP  POF_LOCAL_FCT_ATTR  POF_PrmState_Check_Consistency( POF_UPPER_RQB_PTR_TYPE const pRQB,
                                                                     POF_HDB_PTR_TYPE       const pHDB )
{
  LSA_UINT16  PortIndex;

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_PrmState_Check_Consistency, pHDB:0x%X", pHDB);
  POF_MEMSET_LOCAL(pHDB->PortparamsNotApplicable, 0, sizeof(pHDB->PortparamsNotApplicable));

  for (PortIndex = 0; PortIndex < pHDB->PortCnt; PortIndex++)
  {
    POF_EDD_PCTRL_PTR_TYPE  const  pPCtrl    = &pHDB->pPCtrl[PortIndex];
    LSA_BOOL                       IsOptical = LSA_FALSE;
    LSA_BOOL                       IsPOF     = LSA_FALSE;
    LSA_BOOL                       IsSFP     = LSA_FALSE;

    //store info about modular/compact port
    if (   (POF_PRM_PORT_IS_MODULAR != pRQB->args.PrmEnd.IsModularPort[PortIndex])
        && (POF_PRM_PORT_IS_COMPACT != pRQB->args.PrmEnd.IsModularPort[PortIndex]) )
    {
      POF_PRM_SET_DETAIL_ERR(&pHDB->PrmDetailErr, PortIndex,
          POF_PRM_ERR_INDEX_DEFAULT,
          POF_PRM_ERR_OFFSET_DEFAULT,
          POF_PRM_ERR_FAULT_DEFAULT);

      POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_PrmState_Check_Consistency, illegal value for IsModularPort(%d) for UsrPortIndex %d", 
          pRQB->args.PrmEnd.IsModularPort[PortIndex], PortIndex);
      return POF_RSP_ERR_PARAM;
    }

    pHDB->IsModularPortPrmEnd[PortIndex] = pRQB->args.PrmEnd.IsModularPort[PortIndex];

    //Detect IsOptical and IsPOF locally:
    if (POF_PRMChangePortState_IsPulled(pHDB, PortIndex))
    {
      if (POF_PRM_PORT_IS_MODULAR == pRQB->args.PrmEnd.IsModularPort[PortIndex])
      {
        POF_PROGRAM_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_PrmState_Check_Consistency, missing CHANGE_PORT(PLUG), ChangePortState:%d PortIndex:0x%X",
                             pHDB->pPCtrl[PortIndex].PortModuleSm.State, PortIndex);
        pHDB->PortparamsNotApplicable[PortIndex] = POF_PRM_PORT_PARAMS_NOT_APPLICABLE;
      }
      else
      {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_PrmState_Check_Consistency, Compact Port(%d) is not plugged", PortIndex);
        POF_FATAL("POF_PrmState_Check_Consistency, Compact Port is not plugged:", POF_FATAL_ERR_INCONSISTENZ, PortIndex, 0);
        //lint -unreachable
        return POF_RSP_ERR_OPCODE;
      }
    }
    else
    {
      POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_PrmState_Check_Consistency, PortIndex:0x%X", PortIndex);
      if (POF_RECORD_TYPE_NONE == pPCtrl->RecordSet_B.RecordTypeWritten)
      {
        POF_PROGRAM_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_PrmState_Check_Consistency, POF_DMI_Validate_HW skipped, RecordTypeWritten:0x%X PortIndex:0x%X",
                             pPCtrl->RecordSet_B.RecordTypeWritten, PortIndex);
      }
      else
      {
        if (POF_IS_POF_PORT(PortIndex, pHDB))
        {
          if (pHDB->I2C[PortIndex].I2CAvailable == POF_I2C_AVAILABLE)
          {
            // I2C available try to read the HW values
            if (POF_DMI_Validate_HW(PortIndex, pHDB))
            {
              IsPOF     = LSA_TRUE;
              IsOptical = LSA_TRUE;
            }
            // else: I2C is available but the port is not plugged => this means that further down the default parameters will be applied
          }
          else
          {
            //No I2C - assume it磗 a POF module anyway
            IsPOF     = LSA_TRUE;
            IsOptical = LSA_TRUE;
          }

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
            //No I2C - assume it磗 a SFP module anyway
            IsSFP     = LSA_TRUE;
            IsOptical = LSA_TRUE;
          }
          
        }
        else if (POF_IS_FIBER_PORT(PortIndex, pHDB))
        {
          IsOptical = LSA_TRUE;
        }
      }
    }

    POF_PROGRAM_TRACE_03(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "POF_PrmState_Check_Consistency, IsOptical:%d IsPOF:%d PortIndex:0x%X",
                         IsPOF, IsOptical, PortIndex);

    //Check PDPortFODataAdjust (0x8062):
    if (POF_RECORD_TYPE_DATA_ADJUST_8062 & pPCtrl->RecordSet_B.RecordTypeWritten)
    {
      if (IsPOF)
      {
        //Check moved from POF_Write_FO_DataAdjust() due to avoid POF_IS_POF_PORT in PRM_WRITE
        LSA_UINT32 const Value = POF_NTOH_32(pPCtrl->RecordSet_B.LN_FiberOpticType);

        /* check FiberOpticType, only SI-POF, NA=0.5 -> POF or SI-PCF, NA=0.36 -> HCS allowed. See IEC 61158𤖫0 Ch. 5.2.18.2 Coding of the field FiberOpticType */
        if ((Value != 0x00000004UL) && (Value != 0x00000005UL))
        {
          POF_PROGRAM_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "POF_PrmState_Check_Consistency, Invalid FiberOpticType:0x%x for POF, PortIndex:0x%X",
              Value, PortIndex);

          if (POF_PRM_PORT_IS_MODULAR == pRQB->args.PrmEnd.IsModularPort[PortIndex])
          {
            pHDB->PortparamsNotApplicable[PortIndex] = POF_PRM_PORT_PARAMS_NOT_APPLICABLE;
          }
          else
          {
            POF_PRM_SET_DETAIL_ERR(&pHDB->PrmDetailErr, PortIndex,
                                                        POF_PRM_FO_DATA_ADJUST_INDEX,
                                                        POF_PRM_ERR_OFFSET_DEFAULT,
                                                        POF_PRM_ERR_FAULT_DEFAULT);
            return POF_RSP_ERR_PRM_CONSISTENCY;
          }
        }
      }
      else if (IsOptical)
      {
        // record allowed for SFP and other optical tranceivers but are not checked
      }
      else
      {
        if (POF_PRM_PORT_IS_MODULAR == pRQB->args.PrmEnd.IsModularPort[PortIndex])
        {
          pHDB->PortparamsNotApplicable[PortIndex] = POF_PRM_PORT_PARAMS_NOT_APPLICABLE;
        }
        else
        {
          POF_PRM_SET_DETAIL_ERR(&pHDB->PrmDetailErr, PortIndex,
                                                      POF_PRM_FO_DATA_ADJUST_INDEX,
                                                      POF_PRM_RECORD_FO_DATA_ADJUST_OFFSET_N_FiberOpticType,
                                                      POF_PRM_ERR_FAULT_DEFAULT);
          return POF_RSP_ERR_PRM_CONSISTENCY;
        }
      }
    }

    //Check PDPortFODataCheck (0x8061):
    // This Record was only written (in PRM write), if I2C is available!!!
    if (pPCtrl->RecordSet_B.MRCheckEnable || pPCtrl->RecordSet_B.MDCheckEnable)
    {
      //PDPortFODataCheck makes sense without PDPortFODataAdjust!! PDPortFODataCheck contains all needed information to perform the checks!
      if (!IsPOF)
      {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "POF_PrmState_Check_Consistency, PDPortFODataCheck for non-POF, PortIndex:0x%X",
                                PortIndex);
        if (POF_PRM_PORT_IS_MODULAR == pRQB->args.PrmEnd.IsModularPort[PortIndex])
        {
          pHDB->PortparamsNotApplicable[PortIndex] = POF_PRM_PORT_PARAMS_NOT_APPLICABLE;
        }
        else
        {
          POF_PRM_SET_DETAIL_ERR(&pHDB->PrmDetailErr, PortIndex,
                                                      POF_PRM_FO_DATA_CHECK_INDEX,
                                                      POF_PRM_ERR_OFFSET_DEFAULT,
                                                      POF_PRM_ERR_FAULT_DEFAULT);
          return POF_RSP_ERR_PRM_CONSISTENCY;
        }
      }
    }

    //Check PDPortSFPDataCheck (0x8063):
    // This Record was only written (in PRM write), if I2C is available!!!
    if (POF_RECORD_TYPE_SFP_DATA_CHECK_8063 & pPCtrl->RecordSet_B.RecordTypeWritten)
    {
      if (!(IsSFP || IsOptical))
      {
        POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "POF_PrmState_Check_Consistency, PDPortFODataCheck for non-POF, PortIndex:0x%X",
                                PortIndex);
        if (POF_PRM_PORT_IS_MODULAR == pRQB->args.PrmEnd.IsModularPort[PortIndex])
        {
          pHDB->PortparamsNotApplicable[PortIndex] = POF_PRM_PORT_PARAMS_NOT_APPLICABLE;
        }
        else
        {
          POF_PRM_SET_DETAIL_ERR(&pHDB->PrmDetailErr, PortIndex,
                                                      POF_PRM_SFP_DATA_CHECK_INDEX,
                                                      POF_PRM_ERR_OFFSET_DEFAULT, 
                                                      POF_PRM_ERR_FAULT_DEFAULT);
          return POF_RSP_ERR_PRM_CONSISTENCY;
        }
      }
    }
  }

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_PrmState_Check_Consistency, pHDB:0x%X", pHDB);

  return POF_RSP_OK;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*****************************************************************************/
/*  end of file pof_prm_state.c                                              */
/*****************************************************************************/
