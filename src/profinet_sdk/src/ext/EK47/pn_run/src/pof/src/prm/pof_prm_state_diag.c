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
/*  F i l e               &F: pof_prm_state_diag.c                      :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  POF PRM STATE DIAG                               */
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
#define LTRC_ACT_MODUL_ID  10
#define POF_MODULE_ID      LTRC_ACT_MODUL_ID

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/
#include "pof_inc.h"   
#include "pof_dmi_type.h"
#include "pof_prm_type.h"
#include "pof_int.h"
#include "pof_prm_req.h"
#include "pof_prm_rec.h"
#include "pof_prm_state_diag.h"
#include "pof_glb.h"

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
static LSA_VOID POF_LOCAL_FCT_ATTR PrmSendFO_LED         (LSA_UINT16         const PortIndex,
                                                          LSA_BOOL           const bON,
                                                          POF_HDB_PTR_TYPE   const pHDB);

static LSA_VOID POF_LOCAL_FCT_ATTR PrmStateDiag_Good     (LSA_UINT16         const PortIndex,
                                                          POF_HDB_PTR_TYPE   const pHDB,
                                                          POF_PRM_DIAG_EVENT const AppearEvent,
                                                          LSA_UINT32         const AddValue);

static LSA_VOID POF_LOCAL_FCT_ATTR PrmStateDiag_Required (LSA_UINT16         const PortIndex,
                                                          POF_HDB_PTR_TYPE   const pHDB,
                                                          POF_PRM_DIAG_EVENT const AppearEvent,
                                                          LSA_UINT32         const AddValue);

static LSA_VOID POF_LOCAL_FCT_ATTR PrmStateDiag_Demanded (LSA_UINT16         const PortIndex, 
                                                          POF_HDB_PTR_TYPE   const pHDB,
                                                          POF_PRM_DIAG_EVENT const AppearEvent,
                                                          LSA_UINT32         const AddValue);

static LSA_VOID POF_LOCAL_FCT_ATTR PrmStateDiag_Error    (LSA_UINT16         const PortIndex,
                                                          POF_HDB_PTR_TYPE   const pHDB,
                                                          POF_PRM_DIAG_EVENT const AppearEvent,
                                                          LSA_UINT32         const AddValue);

static LSA_VOID POF_LOCAL_FCT_ATTR PrmStateSetMaintenance(POF_HDB_PTR_TYPE           const pHDB,
                                                          POF_LOCAL_MEM_U16_PTR_TYPE const pCH_Prop,
                                                          POF_PRM_DIAG_EVENT         const Event);

static LSA_VOID POF_LOCAL_FCT_ATTR POF_SFP_EvaluateProperty(
                                                           LSA_UINT16                     const PortIndex,
                                                           POF_HDB_PTR_TYPE               const pHDB,
                                                           POF_UPPER_RQB_PTR_TYPE         const pIndRQB,
                                                           POF_SFP_DIAGNOSTIC_PAIR_TYPE * const pCheckValue, 
                                                           LSA_UINT8 const              * const pDiagnosticRegisters);

/*===========================================================================*/
/*                                static Variables                           */
/*===========================================================================*/


/***************************************************************************/
/* F u n c t i o n:       POF_PRM_StateDiag_Validate_TimeOut()             */
/*                                                                         */
/* D e s c r i p t i o n: check diagnostic-events cyclic                   */
/*                                                                         */
/* A r g u m e n t s:                                                      */
/*                                                                         */
/* Return Value:          LSA_VOID                                         */
/*                                                                         */
/***************************************************************************/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_PRM_StateDiag_Validate_TimeOut( POF_HDB_PTR_TYPE  const  pHDB,
                                                                  LSA_BOOL          const  bCommitCall )
{
  LSA_UINT16  PortIndex;

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_PRM_StateDiag_Validate_TimeOut, bCommitCall:0x%X pHDB:0x%X", bCommitCall, pHDB);
  
  for (PortIndex = 0; PortIndex < pHDB->PortCnt; PortIndex++)
  {
    POF_EDD_PCTRL_PTR_TYPE  const  pPCtrl = &pHDB->pPCtrl[PortIndex];

    if ((!POF_TransceiverIsPlugged(PortIndex, pHDB)) && (!bCommitCall)) /* when commit all ports must deliver a ind */
    {
      continue;
    }

    pPCtrl->ValidateTimeOutFct(PortIndex, pHDB, bCommitCall);
  }
  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_PRM_StateDiag_Validate_TimeOut, bCommitCall:0x%X pHDB:0x%X", bCommitCall, pHDB);
}
/*---------------------- end [subroutine] ---------------------------------*/


/***************************************************************************/
/* F u n c t i o n:       HW_Validate_TimeOutDummy()                       */
/*                                                                         */
/* D e s c r i p t i o n: check diagnostic-events cyclic                   */
/*                                                                         */
/* A r g u m e n t s:                                                      */
/*                                                                         */
/* Return Value:          LSA_VOID                                         */
/*                                                                         */
/***************************************************************************/
LSA_VOID  POF_LOCAL_FCT_ATTR  HW_Validate_TimeOutDummy( LSA_UINT16        const PortIndex,
                                                        POF_HDB_PTR_TYPE  const pHDB,
                                                        LSA_BOOL          const bCommitCall )
{
    POF_EDD_PCTRL_PTR_TYPE  const  pPCtrl = &pHDB->pPCtrl[PortIndex];

    if /* Start, all ports must be deliver a prm-indication */
         (bCommitCall)
    {
        POF_PROGRAM_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: HW_Validate_TimeOutDummy, GOOD, PortIndex:0x%X bCommitCall:0x%X", PortIndex, bCommitCall);

        pPCtrl->TimeThreshold.bMaintenanceRequired = LSA_FALSE;
        pPCtrl->TimeThreshold.bMaintenanceDemanded = LSA_FALSE;
        pPCtrl->TimeThreshold.bError               = LSA_FALSE;

        pPCtrl->DiagStateFct(PortIndex, pHDB, POF_PRM_EV_GOOD, 0 /*AddValue*/);

        /* FO-LED off */
        PrmSendFO_LED(PortIndex, LSA_FALSE, pHDB);
    }
    POF_FUNCTION_TRACE_03(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: HW_Validate_TimeOutDummy, PortIndex:0x%X, bCommitCall:0x%X pHDB:0x%X", PortIndex, bCommitCall, pHDB);
}

/***************************************************************************/
/* F u n c t i o n:       POF_Validate_TimeOut()                           */
/*                                                                         */
/* D e s c r i p t i o n: check diagnostic-events cyclic                   */
/*                                                                         */
/* A r g u m e n t s:                                                      */
/*                                                                         */
/* Return Value:          LSA_VOID                                         */
/*                                                                         */
/***************************************************************************/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_Validate_TimeOut( LSA_UINT16        const PortIndex,
                                                    POF_HDB_PTR_TYPE  const pHDB,
                                                    LSA_BOOL          const bCommitCall )
{
    POF_EDD_PCTRL_PTR_TYPE  const  pPCtrl = &pHDB->pPCtrl[PortIndex];
    LSA_UINT32                     AddValue;

    POF_FUNCTION_TRACE_03(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_Validate_TimeOut, PortIndex:0x%X, bCommitCall:0x%X pHDB:0x%X", PortIndex, bCommitCall, pHDB);
  

    /* Maintenance Demanded 0 dB = Alarm */
    if (   (pPCtrl->LinkStatus.Status == EDD_LINK_UP)
        && (pPCtrl->DMIMap.A2_Mirror.u.ElementPOF.Flagbit_113 & POF_FLAG_RX_OMA_MARGIN_ALARM)
        && (pPCtrl->RecordSet_A.MDCheckEnable))
    {
      POF_PROGRAM_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_Validate_TimeOut, MD, PortIndex:0x%X bMaintenanceDemanded:0x%X",
                           PortIndex, pPCtrl->TimeThreshold.bMaintenanceDemanded);

      /* flag verified by a second read -> creating a Threshold by timing */
      if (!pPCtrl->TimeThreshold.bMaintenanceDemanded)
      {
        pPCtrl->TimeThreshold.bMaintenanceDemanded = LSA_TRUE;
        return;
      }
      else
      {
        pPCtrl->TimeThreshold.bMaintenanceDemanded = LSA_FALSE;
      }

      POF_GetPowerBudget(PortIndex, &AddValue, pHDB);

      pPCtrl->DiagStateFct(PortIndex, pHDB, POF_PRM_EV_DEMANDED, AddValue);

      /* FO-LED on */
      PrmSendFO_LED(PortIndex, LSA_TRUE, pHDB);
    }
    /* Maintenance Required 2dB = Warning */
    else if (   (pPCtrl->LinkStatus.Status == EDD_LINK_UP)
             && (pPCtrl->DMIMap.A2_Mirror.u.ElementPOF.Flagbit_117 & POF_FLAG_RX_OMA_MARGIN_WARN)
             && (pPCtrl->RecordSet_A.MRCheckEnable))
    {
      POF_PROGRAM_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_Validate_TimeOut, MR, PortIndex:0x%X bMaintenanceRequired:0x%X", 
                           PortIndex, pPCtrl->TimeThreshold.bMaintenanceRequired);

      /* flag verified by a second read -> creating a Threshold by timing */
      if (!pPCtrl->TimeThreshold.bMaintenanceRequired)
      {
        pPCtrl->TimeThreshold.bMaintenanceRequired = LSA_TRUE;
        return;
      }
      else
      {
        pPCtrl->TimeThreshold.bMaintenanceRequired = LSA_FALSE;
      }

      POF_GetPowerBudget(PortIndex, &AddValue, pHDB);

      pPCtrl->DiagStateFct(PortIndex, pHDB, POF_PRM_EV_REQUIRED, AddValue);

      /* FO-LED off */
      PrmSendFO_LED(PortIndex, LSA_FALSE, pHDB);
    }
    /* Error not supported */
    else if (pPCtrl->RecordSet_A.ErrorCheckEnable)
    {
      /* flag verified by a second read -> creating a Threshold by timing */
      if (!pPCtrl->TimeThreshold.bError)
      {
        pPCtrl->TimeThreshold.bError = LSA_TRUE;
        return;
      }
      else
      {
        pPCtrl->TimeThreshold.bError = LSA_FALSE;
      }

      POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_Validate_TimeOut");
      POF_FATAL("POF_PRM_StateDiag_Validate_TimeOut", POF_FATAL_ERR_PRM_DIAG_STATE, 0, 0);
      //lint -unreachable
      return;
    }
    else
    {
      if /* Start, all ports must be deliver a prm-indication */
         (  ((pPCtrl->DiagStateFct == (PRM_STATE_DIAG_FCT)POF_PRM_StateDiag_Start) && bCommitCall)
          || (pPCtrl->RecordSet_A.MRCheckEnable || /* Good */
              pPCtrl->RecordSet_A.MDCheckEnable || 
              pPCtrl->RecordSet_A.ErrorCheckEnable))
      {
        POF_PROGRAM_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_Validate_TimeOut, GOOD, PortIndex:0x%X bCommitCall:0x%X", PortIndex, bCommitCall);

        pPCtrl->TimeThreshold.bMaintenanceRequired = LSA_FALSE;
        pPCtrl->TimeThreshold.bMaintenanceDemanded = LSA_FALSE;
        pPCtrl->TimeThreshold.bError               = LSA_FALSE;

        /* when commit, all ports must deliver a ind, but no InitTransceiver have no PowerBudget */
        if ((!POF_TransceiverIsPlugged(PortIndex, pHDB)) && bCommitCall)
        {
          AddValue = 0;
        }
        else
        {
          POF_GetPowerBudget(PortIndex, &AddValue, pHDB); 
        }

        pPCtrl->DiagStateFct(PortIndex, pHDB, POF_PRM_EV_GOOD, AddValue);

        /* FO-LED off */
        PrmSendFO_LED(PortIndex, LSA_FALSE, pHDB);
      }
    }
  
    POF_FUNCTION_TRACE_03(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_Validate_TimeOut, PortIndex:0x%X, bCommitCall:0x%X pHDB:0x%X", PortIndex, bCommitCall, pHDB);
}
/*---------------------- end [subroutine] ---------------------------------*/



/***************************************************************************/
/* F u n c t i o n:       SFP_Validate_TimeOut()                           */
/*                                                                         */
/* D e s c r i p t i o n: check diagnostic-events cyclic                   */
/*                                                                         */
/* A r g u m e n t s:                                                      */
/*                                                                         */
/* Return Value:          LSA_VOID                                         */
/*                                                                         */
/***************************************************************************/
LSA_VOID  POF_LOCAL_FCT_ATTR  SFP_Validate_TimeOut( LSA_UINT16        const PortIndex,
                                                    POF_HDB_PTR_TYPE  const pHDB,
                                                    LSA_BOOL          const bCommitCall )
{
    POF_UPPER_RQB_PTR_TYPE          pIndRQ;
    POF_EDD_PCTRL_PTR_TYPE  const   pPCtrl    = &pHDB->pPCtrl[PortIndex];
    LSA_UINT16                      PropIndex;
    LSA_UINT32                      SlidingBit;

    POF_FUNCTION_TRACE_03(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: SFP_Validate_TimeOut, PortIndex:0x%X, bCommitCall:0x%X pHDB:0x%X", PortIndex, bCommitCall, pHDB);

    if (   ( ( pPCtrl->SFP_Diagnostic.ActivationMask == 0) || (!POF_TransceiverIsPlugged(PortIndex, pHDB)))
        && (!bCommitCall)) /* when commit all ports must deliver an ind */
    {
        return;
    }

    SlidingBit = 1;
    pIndRQ     = (POF_UPPER_RQB_PTR_TYPE)(void *)POF_QueueRemoveFromBegin(&pPCtrl->PrmIndicationQueue);

    if (LSA_HOST_PTR_ARE_EQUAL(pIndRQ, LSA_NULL))
    {
        POF_FUNCTION_TRACE_03(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: SFP_Validate_TimeOut - no resource, PortIndex:0x%X, bCommitCall:0x%X pHDB:0x%X", PortIndex, bCommitCall, pHDB);
    }
    else
    {
        pIndRQ->args.PrmIndication.diag_cnt = 0;

        for (PropIndex = 0; PropIndex < POF_SFP_NUMBER_CHECK_DIAGNOSTICS; ++PropIndex)
        {
            if (SlidingBit & pPCtrl->SFP_Diagnostic.ActivationMask)
            {
                POF_SFP_EvaluateProperty(PortIndex, pHDB, pIndRQ, &pPCtrl->SFP_Diagnostic.SFP_DiagnosticChecks[PropIndex],
                                         &pPCtrl->DMIMap.A2_Mirror.u.ElementSFP.StatusControl);   //  SFP_ADR_STATUS_CONTROL
            }
            SlidingBit <<= 1;
        }

        //??
        if ((pIndRQ->args.PrmIndication.diag_cnt > 0) || bCommitCall)
        {
            POF_UserRequestFinish(pHDB, pIndRQ, POF_RSP_OK);
        }
        else
        {
            POF_QueueAddToEnd(&pPCtrl->PrmIndicationQueue, (POF_QUEUE_ELEMENT_PTR_TYPE)(void *)pIndRQ);
        }
        POF_FUNCTION_TRACE_03(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: SFP_Validate_TimeOut, PortIndex:0x%X, bCommitCall:0x%X pHDB:0x%X", PortIndex, bCommitCall, pHDB);
    }
}
/*---------------------- end [subroutine] ---------------------------------*/


/***************************************************************************/
/* F u n c t i o n:       POF_PrmSendIndication()                          */
/*                                                                         */
/* D e s c r i p t i o n:                                                  */
/*                                                                         */
/* A r g u m e n t s:                                                      */
/*                                                                         */
/* Return Value:          LSA_VOID                                         */
/*                                                                         */
/***************************************************************************/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_PrmSendIndication( LSA_UINT16          const  PortIndex,
                                                     POF_HDB_PTR_TYPE    const  pHDB,
                                                     POF_PRM_DIAG_EVENT  const  AppearEvent,
                                                     POF_PRM_DIAG_EVENT  const  DisappearEvent,
                                                     LSA_UINT32          const  AddValue )
{
  POF_UPPER_RQB_PTR_TYPE                             pIndRQB;
  POF_PRM_IND_DATA_TYPE   POF_LOCAL_MEM_ATTR      *  pDiag;
  POF_EDD_PCTRL_PTR_TYPE                      const  pPCtrl = &pHDB->pPCtrl[PortIndex];

  POF_FUNCTION_TRACE_04(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_PrmSendIndication, PortIndex:0x%X AddValue:0x%X AppearEvent:0x%X DisappearEvent:0x%X",
                        PortIndex, AddValue, AppearEvent, DisappearEvent);

  /* no indication required */
  if (AppearEvent == DisappearEvent)
  {
    return;
  }

  pIndRQB = (POF_UPPER_RQB_PTR_TYPE)(void *)POF_QueueRemoveFromBegin(&pPCtrl->PrmIndicationQueue);
    
  if (LSA_HOST_PTR_ARE_EQUAL(pIndRQB, LSA_NULL))
  {
    POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "POF_PrmSendIndication, no IndicationResources left, PortIndex:0x%X", PortIndex);

    if (!pPCtrl->LostPrmInd.bLost) //save only the first change
    {
      /* no resource for indication exist, save for the next prm_indication */
      pPCtrl->LostPrmInd.bLost          = LSA_TRUE;
      pPCtrl->LostPrmInd.DisappearEvent = DisappearEvent; 
    }

    pPCtrl->LostPrmInd.AppearEvent = AppearEvent;
    pPCtrl->LostPrmInd.AddValue    = AddValue;

    return;
  }
  
  pIndRQB->args.PrmIndication.edd_port_id = (LSA_UINT16)(PortIndex + 1);

  /*** diag_cnt == 0 ***/

  pIndRQB->args.PrmIndication.diag_cnt = 0;

  if ((AppearEvent == POF_PRM_EV_GOOD) && (DisappearEvent == POF_PRM_EV_START))
  {
    /* Commit -> Good */
    POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_PrmSendIndication, Commit -> Good");

    POF_UserRequestFinish(pHDB, pIndRQB, EDD_STS_OK);
    return;
  }

  /*** diag_cnt == 1 ***/

  pDiag                                = &pIndRQB->args.PrmIndication.diag[0];
  pIndRQB->args.PrmIndication.diag_cnt = 1;

  pDiag->ChannelProperties   = POF_PRM_CH_PROP_NONE;
  pDiag->ChannelErrorType    = POF_PRM_CHANNEL_ERROR_TYPE;
  pDiag->ExtChannelErrorType = POF_PRM_EXT_CHANNEL_ERROR_TYPE;
  pDiag->ExtChannelAddValue  = AddValue;

  /* Commit -> MR / MD / Error, only Appear */
  if (DisappearEvent == POF_PRM_EV_START)
  {
    pDiag->ChannelProperties |= POF_PRM_CH_PROP_APPEARS; 

    PrmStateSetMaintenance(pHDB, &pDiag->ChannelProperties, AppearEvent); /* MR / MD / Error */

    POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_PrmSendIndication, Commit -> MR MD Error, only Appear");

    POF_UserRequestFinish(pHDB, pIndRQB, POF_RSP_OK);
    return;
  }

  /* Good -> MR / MD / Error, only Appear */
  if (DisappearEvent == POF_PRM_EV_GOOD)
  {
    pDiag->ChannelProperties |= POF_PRM_CH_PROP_APPEARS; 

    PrmStateSetMaintenance(pHDB, &pDiag->ChannelProperties, AppearEvent); /* MR / MD / Error */

    POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_PrmSendIndication, Good -> MR MD Error, only Appear");
     
    POF_UserRequestFinish(pHDB, pIndRQB, POF_RSP_OK);
    return;
  }
  
  /* MR / MD / Error -> Good, only Disappear */
  if (AppearEvent == POF_PRM_EV_GOOD)
  {
    pDiag->ChannelProperties |= POF_PRM_CH_PROP_DISAPPEARS;

    PrmStateSetMaintenance(pHDB, &pDiag->ChannelProperties, DisappearEvent); /* MR / MD / Error */

    POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_PrmSendIndication, MR MD Error -> Good, only Disappear");

    POF_UserRequestFinish(pHDB, pIndRQB, POF_RSP_OK);
    return;
  }

  /*** diag_cnt == 2 ***/

  /* Disappear */
  pDiag->ChannelProperties |= POF_PRM_CH_PROP_DISAPPEARS;

  PrmStateSetMaintenance(pHDB, &pDiag->ChannelProperties, DisappearEvent); /* MR / MD / Error */

  /* Appear */
  pDiag                                = &pIndRQB->args.PrmIndication.diag[1];
  pIndRQB->args.PrmIndication.diag_cnt = 2;

  pDiag->ChannelProperties   = POF_PRM_CH_PROP_NONE;
  pDiag->ChannelErrorType    = POF_PRM_CHANNEL_ERROR_TYPE;
  pDiag->ExtChannelErrorType = POF_PRM_EXT_CHANNEL_ERROR_TYPE;
  pDiag->ExtChannelAddValue  = AddValue;

  pDiag->ChannelProperties  |= POF_PRM_CH_PROP_APPEARS;

  PrmStateSetMaintenance(pHDB, &pDiag->ChannelProperties, AppearEvent);

  POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "POF_PrmSendIndication, MR MD Error disapppear");
  
  POF_UserRequestFinish(pHDB, pIndRQB, POF_RSP_OK);

  POF_FUNCTION_TRACE_03(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_PrmSendIndication, PortIndex:0x%X AppearEvent:0x%X DisappearEvent:0x%X", 
                        PortIndex, AppearEvent, DisappearEvent);
}
/*---------------------- end [subroutine] ---------------------------------*/


/***************************************************************************/
/* F u n c t i o n:       PrmSendFO_LED()                                  */
/*                                                                         */
/* D e s c r i p t i o n:                                                  */
/*                                                                         */
/* A r g u m e n t s:                                                      */
/*                                                                         */
/* Return Value:          LSA_VOID                                         */
/*                                                                         */
/***************************************************************************/
static  LSA_VOID  POF_LOCAL_FCT_ATTR  PrmSendFO_LED( LSA_UINT16        const  PortIndex,
                                                     LSA_BOOL          const  bON,
                                                     POF_HDB_PTR_TYPE  const  pHDB )
{
  POF_EDD_PCTRL_PTR_TYPE  const  pPCtrl = &pHDB->pPCtrl[PortIndex];

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: PrmSendFO_LED, PortIndex:0x%X ON:0x%X", PortIndex, bON);

  if (bON == pPCtrl->bLastFO_LED_State)
  {
    return;
  }

  pPCtrl->bLastFO_LED_State = bON;

  /* switch FO_LED on/off */
  POF_FO_LED((LSA_UINT16)(PortIndex + 1), bON?POF_SYS_FO_LED_ON:POF_SYS_FO_LED_OFF, pHDB->Params.pSys);

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: PrmSendFO_LED, PortIndex:0x%X ON:0x%X", PortIndex, bON);
}
/*---------------------- end [subroutine] ---------------------------------*/


/***************************************************************************/
/* F u n c t i o n:       POF_PRM_StateDiag_Start()                        */
/*                                                                         */
/* D e s c r i p t i o n:                                                  */
/*                                                                         */
/* A r g u m e n t s:                                                      */
/*                                                                         */
/* Return Value:          LSA_VOID                                         */
/*                                                                         */
/***************************************************************************/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_PRM_StateDiag_Start( LSA_UINT16              const  PortIndex,
                                                       POF_HDB_PTR_TYPE        const  pHDB,
                                                       POF_PRM_DIAG_EVENT      const  AppearEvent,
                                                       LSA_UINT32              const  AddValue )
{   
  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_PRM_StateDiag_Start, PortIndex:0x%X AppearEvent:0x%X", PortIndex, AppearEvent);

  switch (AppearEvent)
  {
    case POF_PRM_EV_GOOD: 
    {
      /* special after PRM_COMMIT */
      pHDB->pPCtrl[PortIndex].DiagStateFct = (PRM_STATE_DIAG_FCT)PrmStateDiag_Good;
    } break;

    case POF_PRM_EV_REQUIRED:
    {
      pHDB->pPCtrl[PortIndex].DiagStateFct = (PRM_STATE_DIAG_FCT)PrmStateDiag_Required;
    } break;

    case POF_PRM_EV_DEMANDED:
    {
      pHDB->pPCtrl[PortIndex].DiagStateFct = (PRM_STATE_DIAG_FCT)PrmStateDiag_Demanded;
    } break;

    case POF_PRM_EV_ERROR:
    {
      pHDB->pPCtrl[PortIndex].DiagStateFct = (PRM_STATE_DIAG_FCT)PrmStateDiag_Error;
    } break;

    case POF_PRM_EV_START:
    default:
    {
      POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_PRM_StateDiag_Start, AppearEvent:0x%X", AppearEvent);
      POF_FATAL("POF_PRM_StateDiag_Start, AppearEvent:", POF_FATAL_ERR_PRM_DIAG_STATE, AppearEvent, 0); 
      //lint -unreachable
      return;
    }
  }  

  POF_PrmSendIndication(PortIndex, pHDB, AppearEvent, POF_PRM_EV_START, AddValue);

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_PRM_StateDiag_Start, PortIndex:0x%X AppearEvent:0x%X", PortIndex, AppearEvent);
}
/*---------------------- end [subroutine] ---------------------------------*/


/***************************************************************************/
/* F u n c t i o n:       PrmStateDiag_Good()                              */
/*                                                                         */
/* D e s c r i p t i o n:                                                  */
/*                                                                         */
/* A r g u m e n t s:                                                      */
/*                                                                         */
/* Return Value:          LSA_VOID                                         */
/*                                                                         */
/***************************************************************************/
static  LSA_VOID  POF_LOCAL_FCT_ATTR  PrmStateDiag_Good( LSA_UINT16          const  PortIndex,
                                                         POF_HDB_PTR_TYPE    const  pHDB,
                                                         POF_PRM_DIAG_EVENT  const  AppearEvent,
                                                         LSA_UINT32          const  AddValue )
{
  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: PrmStateDiag_Good, PortIndex:0x%X AppearEvent:0x%X", PortIndex, AppearEvent);

  switch (AppearEvent)
  {
    case POF_PRM_EV_GOOD: 
    {
      return; //nothing to do
    }

    case POF_PRM_EV_REQUIRED:
    {
      pHDB->pPCtrl[PortIndex].DiagStateFct = (PRM_STATE_DIAG_FCT)PrmStateDiag_Required;
      break;
    }

    case POF_PRM_EV_DEMANDED:
    {
      pHDB->pPCtrl[PortIndex].DiagStateFct = (PRM_STATE_DIAG_FCT)PrmStateDiag_Demanded;
      break;
    }

    case POF_PRM_EV_ERROR:
    {
      pHDB->pPCtrl[PortIndex].DiagStateFct = (PRM_STATE_DIAG_FCT)PrmStateDiag_Error;
      break;
    }

    case POF_PRM_EV_START:
    default:  
    {
      POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "PrmStateDiag_Good, AppearEvent:0x%X", AppearEvent);
      POF_FATAL("PrmStateDiag_Good, AppearEvent:", POF_FATAL_ERR_PRM_DIAG_STATE, AppearEvent, 0); 
      //lint -unreachable
      return;
    }
  }  

  POF_PrmSendIndication(PortIndex, pHDB, AppearEvent, POF_PRM_EV_GOOD, AddValue);

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: PrmStateDiag_Good, PortIndex:0x%X AppearEvent:0x%X", PortIndex, AppearEvent);
}
/*---------------------- end [subroutine] ---------------------------------*/


/***************************************************************************/
/* F u n c t i o n:       PrmStateDiag_Required()                          */
/*                                                                         */
/* D e s c r i p t i o n:                                                  */
/*                                                                         */
/* A r g u m e n t s:                                                      */
/*                                                                         */
/* Return Value:          LSA_VOID                                         */
/*                                                                         */
/***************************************************************************/
static  LSA_VOID  POF_LOCAL_FCT_ATTR  PrmStateDiag_Required( LSA_UINT16             const  PortIndex,
                                                             POF_HDB_PTR_TYPE       const  pHDB,
                                                             POF_PRM_DIAG_EVENT     const  AppearEvent,
                                                             LSA_UINT32             const  AddValue )
{
  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: PrmStateDiag_Required, PortIndex:0x%X AppearEvent:0x%X", PortIndex, AppearEvent);

  switch (AppearEvent)
  {
    case POF_PRM_EV_GOOD: 
    {
      pHDB->pPCtrl[PortIndex].DiagStateFct = (PRM_STATE_DIAG_FCT)PrmStateDiag_Good;
    } break;

    case POF_PRM_EV_REQUIRED:
    {
      return; //nothing to do
    }

    case POF_PRM_EV_DEMANDED:
    {
      pHDB->pPCtrl[PortIndex].DiagStateFct = (PRM_STATE_DIAG_FCT)PrmStateDiag_Demanded;
    } break;

    case POF_PRM_EV_ERROR:
    {
      pHDB->pPCtrl[PortIndex].DiagStateFct = (PRM_STATE_DIAG_FCT)PrmStateDiag_Error;
    } break;

    case POF_PRM_EV_START:
    default:
    {
      POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "PrmStateDiag_Required, AppearEvent:0x%X", AppearEvent);
      POF_FATAL("PrmStateDiag_Required, AppearEvent:", POF_FATAL_ERR_PRM_DIAG_STATE, AppearEvent, 0); 
      //lint -unreachable
      return;
    }
  }  

  POF_PrmSendIndication(PortIndex, pHDB, AppearEvent, POF_PRM_EV_REQUIRED, AddValue);

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: PrmStateDiag_Required, PortIndex:0x%X AppearEvent:0x%X", PortIndex, AppearEvent);
}
/*---------------------- end [subroutine] ---------------------------------*/


/***************************************************************************/
/* F u n c t i o n:                                                        */
/*                                                                         */
/* D e s c r i p t i o n:                                                  */
/*                                                                         */
/* A r g u m e n t s:                                                      */
/*                                                                         */
/* Return Value:                                                           */
/*                                                                         */
/***************************************************************************/
static  LSA_VOID  POF_LOCAL_FCT_ATTR  PrmStateDiag_Demanded( LSA_UINT16          const  PortIndex,
                                                             POF_HDB_PTR_TYPE    const  pHDB,
                                                             POF_PRM_DIAG_EVENT  const  AppearEvent,
                                                             LSA_UINT32          const  AddValue )
{
  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: PrmStateDiag_Demanded, PortIndex:0x%X AppearEvent:0x%X", PortIndex, AppearEvent);

  switch (AppearEvent)
  {
    case POF_PRM_EV_GOOD: 
    {
      pHDB->pPCtrl[PortIndex].DiagStateFct = (PRM_STATE_DIAG_FCT)PrmStateDiag_Good;
    } break;

    case POF_PRM_EV_REQUIRED: 
    {
      pHDB->pPCtrl[PortIndex].DiagStateFct = (PRM_STATE_DIAG_FCT)PrmStateDiag_Required;
    } break;

    case POF_PRM_EV_DEMANDED: 
    {
      return; //nothing to do
    }

    case POF_PRM_EV_ERROR:
    {
      pHDB->pPCtrl[PortIndex].DiagStateFct = (PRM_STATE_DIAG_FCT)PrmStateDiag_Error;
    } break;

    case POF_PRM_EV_START:
    default: 
    {
      POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "PrmStateDiag_Demanded, AppearEvent:0x%X", AppearEvent);
      POF_FATAL("PrmStateDiag_Demanded, AppearEvent:", POF_FATAL_ERR_PRM_DIAG_STATE, AppearEvent, 0); 
      //lint -unreachable
      return;
    }
  }  

  POF_PrmSendIndication(PortIndex, pHDB, AppearEvent, POF_PRM_EV_DEMANDED, AddValue);

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: PrmStateDiag_Demanded, PortIndex:0x%X AppearEvent:0x%X", PortIndex, AppearEvent);
}
/*---------------------- end [subroutine] ---------------------------------*/


/***************************************************************************/
/* F u n c t i o n:                                                        */
/*                                                                         */
/* D e s c r i p t i o n:                                                  */
/*                                                                         */
/* A r g u m e n t s:                                                      */
/*                                                                         */
/* Return Value:                                                           */
/*                                                                         */
/***************************************************************************/
static  LSA_VOID  POF_LOCAL_FCT_ATTR  PrmStateDiag_Error( LSA_UINT16          const  PortIndex,
                                                          POF_HDB_PTR_TYPE    const  pHDB,
                                                          POF_PRM_DIAG_EVENT  const  AppearEvent,
                                                          LSA_UINT32          const  AddValue )
{
  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: PrmStateDiag_Error, PortIndex:0x%X AppearEvent:0x%X", PortIndex, AppearEvent);

  switch (AppearEvent)
  {
    case POF_PRM_EV_GOOD: 
    {
      pHDB->pPCtrl[PortIndex].DiagStateFct = (PRM_STATE_DIAG_FCT)PrmStateDiag_Good;
    } break;

    case POF_PRM_EV_REQUIRED: 
    {
      pHDB->pPCtrl[PortIndex].DiagStateFct = (PRM_STATE_DIAG_FCT)PrmStateDiag_Required;
    } break;

    case POF_PRM_EV_DEMANDED:
    {
      pHDB->pPCtrl[PortIndex].DiagStateFct = (PRM_STATE_DIAG_FCT)PrmStateDiag_Demanded;
    } break;

    case POF_PRM_EV_ERROR: 
    {
      return; //nothing to do
    }

    case POF_PRM_EV_START:
    default:
    {
      POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "PrmStateDiag_Error, AppearEvent:0x%X", AppearEvent);
      POF_FATAL("PrmStateDiag_Error, AppearEvent:", POF_FATAL_ERR_PRM_DIAG_STATE, AppearEvent, 0); 
      //lint -unreachable
      return;
    }
  }  

  POF_PrmSendIndication(PortIndex, pHDB, AppearEvent, POF_PRM_EV_ERROR, AddValue);

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: PrmStateDiag_Error, PortIndex:0x%X AppearEvent:0x%X", PortIndex, AppearEvent);
}
/*---------------------- end [subroutine] ---------------------------------*/


/***************************************************************************/
/* F u n c t i o n:                                                        */
/*                                                                         */
/* D e s c r i p t i o n:                                                  */
/*                                                                         */
/* A r g u m e n t s:                                                      */
/*                                                                         */
/* Return Value:                                                           */
/*                                                                         */
/***************************************************************************/
static  LSA_VOID  POF_LOCAL_FCT_ATTR  PrmStateSetMaintenance( POF_HDB_PTR_TYPE            const  pHDB,
                                                              POF_LOCAL_MEM_U16_PTR_TYPE  const  pProper,
                                                              POF_PRM_DIAG_EVENT          const  Event )
{
  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: PrmStateSetMaintenance, Event:0x%X Proper:0x%X", Event, *pProper);

  switch (Event)
  {
    case POF_PRM_EV_REQUIRED:  
    {
      *pProper |=  POF_PRM_CH_PROP_MAIN_REQUIRED; 
      *pProper &= ~POF_PRM_CH_PROP_MAIN_DEMANDED; 
    } break;

    case POF_PRM_EV_DEMANDED: 
    {
      *pProper &= ~POF_PRM_CH_PROP_MAIN_REQUIRED; 
      *pProper |=  POF_PRM_CH_PROP_MAIN_DEMANDED; 
    } break;

    case POF_PRM_EV_ERROR:     
    {
      *pProper &= ~POF_PRM_CH_PROP_MAIN_REQUIRED; 
      *pProper &= ~POF_PRM_CH_PROP_MAIN_DEMANDED; 
    } break;

    case POF_PRM_EV_GOOD:
    case POF_PRM_EV_START:
    default:
    {
      POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "PrmStateSetMaintenance, Event:0x%X", Event);
      LSA_UNUSED_ARG(pHDB); //satisfy lint!
      POF_FATAL("PrmStateSetMaintenance, Event:", POF_FATAL_ERR_PRM_DIAG_STATE, Event, 0);
      //lint -unreachable
      return;
    }
  }

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: PrmStateSetMaintenance, Event:0x%X Proper:0x%X", Event, *pProper);
}
/*---------------------- end [subroutine] ---------------------------------*/


static LSA_BOOL POF_LOCAL_FCT_ATTR SFP_PropertyStateSignaled (POF_SFP_DIAGNOSTIC_TYPE const * const  pDiagnosticCheck,
                                                              LSA_UINT8               const * const  pDiagnosticRegisters)
{
    LSA_BOOL bSignaled = (pDiagnosticRegisters[pDiagnosticCheck->AlarmAddress] & pDiagnosticCheck->CheckBitMask) != 0;
    
    return bSignaled;
}


LSA_VOID POF_LOCAL_FCT_ATTR POF_SFP_IndicateChange(LSA_UINT16                      const PortIndex, 
                                                   POF_UPPER_RQB_PTR_TYPE          const pIndRQB, 
                                                   POF_SFP_DIAGNOSTIC_TYPE const * const pChangeValue,
                                                   LSA_UINT16                      const ChannelPropBase)
{
    POF_RQB_PRM_INDICATION_TYPE                 *  pPrmIndication = &pIndRQB->args.PrmIndication;    /* indication diagnostic    */
    POF_PRM_IND_DATA_TYPE   POF_LOCAL_MEM_ATTR  *  pDiag          = &pPrmIndication->diag[pPrmIndication->diag_cnt];

    pPrmIndication->edd_port_id = (LSA_UINT16)(PortIndex + 1);

    pDiag->ChannelProperties   = ChannelPropBase | pChangeValue->DiagEventSeverity;
    pDiag->ChannelErrorType    = POF_PRM_CHANNEL_ERROR_TYPE;
    pDiag->ExtChannelErrorType = pChangeValue->ExtChannelErrorType;
    pDiag->ExtChannelAddValue  = 0;
 
    pPrmIndication->diag_cnt++;
    POF_FUNCTION_TRACE_05(0, LSA_TRACE_LEVEL_CHAT, "POF_SFP_IndicateChange, EDD-Port-ID: 0x%X, CH-Properties 0x%4X, "
                                                   "CH-ErrorType 0x%4X, EXT.CH-ErrorType 0x%4X, diag_cnt %d", 
        pPrmIndication->edd_port_id,
        pDiag->ChannelProperties,
        pDiag->ChannelErrorType,
        pDiag->ExtChannelErrorType,
        pPrmIndication->diag_cnt);
    return;
}


static LSA_VOID POF_LOCAL_FCT_ATTR POF_SFP_EvaluateProperty(LSA_UINT16                     const  PortIndex,
                                                            POF_HDB_PTR_TYPE               const  pHDB,
                                                            POF_UPPER_RQB_PTR_TYPE         const  pIndRQB,
                                                            POF_SFP_DIAGNOSTIC_PAIR_TYPE * const  pCheckValue, 
                                                            LSA_UINT8 const              * const  pDiagnosticRegisters)
{
    POF_SFP_DIAGNOSTIC_TYPE  *  pHighValue   = &pCheckValue->HighValue;
    POF_SFP_DIAGNOSTIC_TYPE  *  pLowValue    = &pCheckValue->LowValue;
    POF_SFP_STATE_VALUES        NextState;


    if (SFP_PropertyStateSignaled(pHighValue, pDiagnosticRegisters))
    {
        NextState = SFP_PROPERTY_STATE_ABOVE_HIGH;
    }
    else if (SFP_PropertyStateSignaled(pLowValue, pDiagnosticRegisters))
    {
        NextState = SFP_PROPERTY_STATE_BELOW_LOW;
    }
    else
    {
        NextState = SFP_PROPERTY_STATE_NORMAL;
    }

    if (NextState != pCheckValue->LastReportedState)
    {
        if ((SFP_PROPERTY_STATE_NORMAL == NextState) && (SFP_PROPERTY_STATE_NORMAL == pCheckValue->State))
        {
            if (SFP_PROPERTY_STATE_ABOVE_HIGH == pCheckValue->LastReportedState)
            {
                POF_SFP_IndicateChange(PortIndex, pIndRQB, pHighValue, POF_PRM_CH_PROP_DISAPPEARS);
            }
            else
            {
                POF_SFP_IndicateChange(PortIndex, pIndRQB, pLowValue, POF_PRM_CH_PROP_DISAPPEARS);
            }
            pCheckValue->LastReportedState = SFP_PROPERTY_STATE_NORMAL;
        }
        else if ((SFP_PROPERTY_STATE_ABOVE_HIGH == NextState) && (SFP_PROPERTY_STATE_ABOVE_HIGH == pCheckValue->State))
        {
            POF_SFP_IndicateChange(PortIndex, pIndRQB, pHighValue, POF_PRM_CH_PROP_APPEARS);

            if (SFP_PROPERTY_STATE_NORMAL != pCheckValue->LastReportedState)
            {
                POF_SFP_IndicateChange(PortIndex, pIndRQB, pLowValue, POF_PRM_CH_PROP_DISAPPEARS);
            }
            pCheckValue->LastReportedState = SFP_PROPERTY_STATE_ABOVE_HIGH;
        }
        else if ((SFP_PROPERTY_STATE_BELOW_LOW  == NextState) && (SFP_PROPERTY_STATE_BELOW_LOW  == pCheckValue->State))
        {
            POF_SFP_IndicateChange(PortIndex, pIndRQB, pLowValue, POF_PRM_CH_PROP_APPEARS);

            if (SFP_PROPERTY_STATE_NORMAL != pCheckValue->LastReportedState)
            {
                POF_SFP_IndicateChange(PortIndex, pIndRQB, pHighValue, POF_PRM_CH_PROP_DISAPPEARS);
            }
            pCheckValue->LastReportedState = SFP_PROPERTY_STATE_BELOW_LOW;
        }
    }
    pCheckValue->State = NextState;

    LSA_UNUSED_ARG(pHDB); //satisfy lint!
    return;
}

/*---------------------end SFP helper functions------------------------------*/

/*****************************************************************************/
/*  end of file pof_prm_state_diag.c                                         */
/*****************************************************************************/
