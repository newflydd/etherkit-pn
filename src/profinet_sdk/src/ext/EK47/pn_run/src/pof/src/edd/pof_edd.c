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
/*  F i l e               &F: pof_edd.c                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  POF EDD functions                                */
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
#define LTRC_ACT_MODUL_ID  5
#define POF_MODULE_ID      LTRC_ACT_MODUL_ID

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/
#include "pof_inc.h"            /* POF headerfiles */
#include "pof_dmi_type.h"
#include "pof_prm_type.h"
#include "pof_int.h"            /* internal header */
#include "pof_prm_state_diag.h"
#include "pof_glb.h"
#include "pof_edd.h"
#include "pof_prm_state.h"

POF_FILE_SYSTEM_EXTENSION(POF_MODULE_ID)

/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/

/*===========================================================================*/
/*                                global Variables                           */
/*===========================================================================*/

/*===========================================================================*/
/*                            local function declaration                     */
/*===========================================================================*/
static LSA_VOID POF_LOCAL_FCT_ATTR POF_EDDCloseChannelDone(   POF_EDD_LOWER_RQB_PTR_TYPE const pRQB,
                                                              POF_HDB_PTR_TYPE           const pHDB );               

static LSA_VOID POF_LOCAL_FCT_ATTR POF_EDDLinkIndProvideDone( POF_EDD_LOWER_RQB_PTR_TYPE const pRQB,
                                                              POF_HDB_PTR_TYPE           const pHDB );

static LSA_VOID POF_LOCAL_FCT_ATTR POF_EDDGetParamsDone(      POF_EDD_LOWER_RQB_PTR_TYPE const pRQB,
                                                              POF_HDB_PTR_TYPE           const pHDB );
               
static LSA_VOID POF_LOCAL_FCT_ATTR POF_EDDOpenChannelDone(    POF_EDD_LOWER_RQB_PTR_TYPE const pRQB,
                                                              POF_HDB_PTR_TYPE           const pHDB );
static LSA_VOID POF_LOCAL_FCT_ATTR SFP_SetFctPtrs(            POF_EDD_PCTRL_PTR_TYPE           pPCtrl,
                                                              LSA_UINT8                  const OpticalTransType);   //?? move to SFP file
static LSA_VOID POF_LOCAL_FCT_ATTR SFP_Reset_Diagnostic(      POF_EDD_PCTRL_PTR_TYPE           pPCtrl);             //?? move to SFP file

/*===========================================================================*/
/*                                 Macros                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_EDDFreePortMgm()                        +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pHDB       : Pointer to EDD HDB                                        +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Free EDD Port management                                  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID  POF_LOCAL_FCT_ATTR  POF_EDDFreePortMgm( POF_HDB_PTR_TYPE  const  pHDB )
{   
  LSA_UINT32  Ctr;
  LSA_UINT16  FreeStat;

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_EDDFreePortMgm, pHDB:0x%X", pHDB);

  POF_CHECK_NULL_PTR(pHDB);

  FreeStat = 0;

  /* --------------------------------------------------------------------------*/
  /* Release Port-Memory                                                       */
  /* --------------------------------------------------------------------------*/
  if (LSA_HOST_PTR_ARE_EQUAL(pHDB->pPCtrl, LSA_NULL))
  {
      return;
  }

  //Release all unused Indications
  for (Ctr = 0; Ctr < pHDB->PortCnt; Ctr++)
  {
    while (pHDB->pPCtrl[Ctr].PrmIndicationQueue.Count)
    {
      POF_UPPER_RQB_PTR_TYPE  const  pIndRQB = (POF_UPPER_RQB_PTR_TYPE)(void *)POF_QueueRemoveFromBegin(&pHDB->pPCtrl[Ctr].PrmIndicationQueue);

      if (pIndRQB == LSA_NULL)
      {
          POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_EDDFreePortMgm, pIndRQB == 0");
          POF_FATAL("POF_EDDFreePortMgm, pIndRQB == 0", POF_FATAL_ERR_NULL_PTR, 0, 0);
          //lint -unreachable
          return;
      }

      POF_UserRequestFinish(pHDB, pIndRQB, POF_RSP_OK_CANCEL);
    }
  }

  /* ----------------------------------------------------------------------*/
  /* Free Port management                                                  */
  /* ----------------------------------------------------------------------*/
  POF_FREE_LOCAL_MEM(&FreeStat, pHDB->pPCtrl);
  POF_MEM_FREE_FAIL_CHECK(FreeStat);
  pHDB->pPCtrl = LSA_NULL;

  POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_EDDFreePortMgm");
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_EDDInitPortMgm()                        +*/
/*+  Input/Output          :                                                +*/
/*+  Result                :    POF_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  Result     : POF_RSP_OK                                                +*/
/*+               POF_RSP_ERR_RESOURCE                                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Initializes HDB with PORT-Managment (memory)              +*/
/*+               The PortCnt and PortMaxSender has to be set in pHDB       +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
POF_RSP  POF_LOCAL_FCT_ATTR  POF_EDDInitPortMgm( POF_HDB_PTR_TYPE  const  pHDB,
                                                 LSA_UINT32        const  PortCnt )
{   
  LSA_UINT32  SizeCtrlArray;
  LSA_UINT16  PortIndex;

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_EDDInitPortMgm, pHDB:0x%X PortCnt:%d", pHDB, PortCnt);

  POF_CHECK_NULL_PTR(pHDB);

  if (0 == PortCnt)
  {
    POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "POF_EDDInitPortMgm, PortCnt == 0");
    POF_FATAL("POF_EDDInitPortMgm, PortCnt == 0", POF_FATAL_ERR_NULL_PTR, 0, 0);
    //lint -unreachable
    return POF_RSP_ERR_RESOURCE;
  }

  SizeCtrlArray = sizeof(POF_EDD_PCTRL_TYPE) * PortCnt;

  POF_ALLOC_LOCAL_MEM((POF_LOCAL_MEM_PTR_TYPE POF_LOCAL_MEM_ATTR *)&pHDB->pPCtrl, SizeCtrlArray);

  if (LSA_HOST_PTR_ARE_EQUAL(pHDB->pPCtrl, LSA_NULL))
  {
    POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "POF_EDDInitPortMgm, Allocating memory failed");
    return POF_RSP_ERR_RESOURCE;
  }

  //initialize memory with 0
  POF_MEMSET_LOCAL(pHDB->pPCtrl, (LSA_UINT8)0, SizeCtrlArray);

  for (PortIndex = 0; PortIndex < pHDB->PortCnt; PortIndex++)
  {
    POF_EDD_PCTRL_PTR_TYPE  const  pPCtrl = &pHDB->pPCtrl[PortIndex];

    pPCtrl->TimeThreshold.bMaintenanceRequired = LSA_FALSE;
    pPCtrl->TimeThreshold.bMaintenanceDemanded = LSA_FALSE;
    pPCtrl->TimeThreshold.bError               = LSA_FALSE;

    pPCtrl->LinkStatus.Status                  = EDD_LINK_UNKNOWN;
    pPCtrl->LinkStatus.Mode                    = EDD_LINK_UNKNOWN;
    pPCtrl->LinkStatus.Speed                   = EDD_LINK_UNKNOWN;
    pPCtrl->LinkStatus.MediaType               = EDD_MEDIATYPE_UNKNOWN;
    pPCtrl->LinkStatus.OpticalTransType        = EDD_PORT_OPTICALTYPE_ISOTHER;

    pPCtrl->RecordSet_A.LN_FiberOpticType      = 0; /* No fiber type adjusted */
    pPCtrl->RecordSet_A.LN_FiberOpticCableType = 0; /* no cable specified     */

    pPCtrl->RecordSet_A.LN_MRPowerBudget       = 0; /* CheckEnable OFF        */
    pPCtrl->RecordSet_A.LN_MDPowerBudget       = 0; /* CheckEnable OFF        */
    pPCtrl->RecordSet_A.LN_ErrorPowerBudget    = 0; /* CheckEnable OFF        */
    
    pPCtrl->RecordSet_A.MRCheckEnable          = LSA_FALSE;
    pPCtrl->RecordSet_A.MDCheckEnable          = LSA_FALSE;
    pPCtrl->RecordSet_A.ErrorCheckEnable       = LSA_FALSE; 

    pPCtrl->RecordSet_A.MRThresholdWritten     = LSA_FALSE;
    pPCtrl->RecordSet_A.MDThresholdWritten     = LSA_FALSE;

    pPCtrl->RecordSet_A.MRPowerBudget_0_1_dB   = 0; /* PowerBudget in 0_1dB */
    pPCtrl->RecordSet_A.MDPowerBudget_0_1_dB   = 0; /* PowerBudget in 0_1dB */

    pPCtrl->RecordSet_A.RecordTypeWritten      = POF_RECORD_TYPE_NONE; /* No Record written */

    pPCtrl->DiagStateFct                       = (PRM_STATE_DIAG_FCT)      POF_PRM_StateDiag_Start;
    pPCtrl->ReadHW_MarginFct                   = (DMI_READ_HW_MARGIN_FCT)  DMI_ReadHW_MarginDummy;
    pPCtrl->ValidateTimeOutFct                 = (HW_VALIDATE_TIMEOUT_FCT) HW_Validate_TimeOutDummy;

    //ini PrmIndicationQueue
    pPCtrl->PrmIndicationQueue.Count           = 0;
    pPCtrl->PrmIndicationQueue.pFirst          = LSA_NULL;
    pPCtrl->PrmIndicationQueue.pLast           = LSA_NULL;

    pPCtrl->PortModuleSm.State                 = POF_PORT_MODULE_SM_STATE_PLUGGED;

    pPCtrl->TransceiverState                   = POF_TRANSCEIVER_STATE_STOPPED;

    pPCtrl->bLastFO_LED_State                  = LSA_FALSE;

    pPCtrl->LostPrmInd.bLost                   = LSA_FALSE;
    pPCtrl->LostPrmInd.AppearEvent             = POF_PRM_EV_GOOD;
    pPCtrl->LostPrmInd.DisappearEvent          = POF_PRM_EV_GOOD;
    pPCtrl->LostPrmInd.AddValue                = 0;
  }

  POF_PrmState_PrmIni(pHDB);

  POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_EDDInitPortMgm");
  
  return POF_RSP_OK;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_EDDLinkIndProvide()                     +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    POF_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  Result               : POF_RSP_OK                                      +*/
/*+                         POF_RSP_ERR_RESOURCE                            +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Sends NRT-Link Indication Provide Requests to EDD         +*/
/*+               (EDD_SRV_LINK_STATUS_IND_PROVIDE_EXT) for PortCnt ports.  +*/
/*+                                                                         +*/
/*+               Allocates RQB and does EDD-Requests                       +*/
/*+                                                                         +*/
/*+               Note: currently we have one Ind-Resource per Port         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
POF_RSP  POF_LOCAL_FCT_ATTR  POF_EDDLinkIndProvide( POF_HDB_PTR_TYPE  const  pHDB,
                                                    LSA_UINT32        const  PortCnt )
{
  POF_EDD_LOWER_RQB_PTR_TYPE                    pRQB;
  POF_EDD_RQB_LINK_STATUS_IND_PROVIDE_PTR_TYPE  pLink;
  LSA_SYS_PTR_TYPE                              pSys;
  LSA_USER_ID_TYPE                              UserId;
  LSA_UINT16                                    RetVal;
  LSA_UINT32                                    Ctr;

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_EDDLinkIndProvide, pHDB:0x%X", pHDB);

  POF_CHECK_NULL_PTR(pHDB);

  pSys          = pHDB->Params.pSys;
  UserId.uvar32 = 0;    

  /*---------------------------------------------------------------------------*/
  /* allocate and send Request to EDD                                          */
  /*---------------------------------------------------------------------------*/
  pHDB->LinkIndPendingCnt = 0; /* number of Link-Ind-Request pending within EDD */

  for (Ctr = 0; Ctr < PortCnt; Ctr++)
  {
    /* allocate RQB */
    POF_EDD_ALLOC_LOWER_RQB(&pRQB, UserId, (LSA_UINT16)sizeof(EDD_RQB_TYPE), pSys);
    if (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL))
    { 
      POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "EDD-Allocating lower EDD RQB-memory failed!");
      return POF_RSP_ERR_RESOURCE;      
    }

    POF_EDD_ALLOC_LOWER_MEM((POF_EDD_LOWER_MEM_PTR_TYPE POF_LOCAL_MEM_ATTR *)&pLink,
                            UserId,
                            (LSA_UINT16)sizeof(EDD_RQB_LINK_STATUS_IND_PROVIDE_TYPE),
                            pSys);
    if (LSA_HOST_PTR_ARE_EQUAL(pLink, LSA_NULL))
    {   
      POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "EDD-Allocating lower EDD memory failed!");
      POF_EDD_FREE_LOWER_RQB(&RetVal, pRQB, pSys);
      POF_MEM_FREE_FAIL_CHECK(RetVal);
      return POF_RSP_ERR_RESOURCE;      
    }

    POF_EDD_RQB_SET_OPCODE(       pRQB, EDD_OPC_REQUEST);
    POF_EDD_RQB_SET_SERVICE(      pRQB, EDD_SRV_LINK_STATUS_IND_PROVIDE_EXT);
    POF_EDD_RQB_SET_LOWER_HANDLE( pRQB, pHDB->EDDHandle);

    pRQB->pParam            = pLink;
    pLink->Status           = EDD_LINK_UNKNOWN;
    pLink->Speed            = EDD_LINK_UNKNOWN;
    pLink->Mode             = EDD_LINK_UNKNOWN;
    pLink->MAUType          = EDD_MAUTYPE_UNKNOWN;
    pLink->MediaType        = EDD_MEDIATYPE_UNKNOWN;
    pLink->OpticalTransType = EDD_PORT_OPTICALTYPE_ISOTHER;
    pLink->PhyStatus        = EDD_PHY_STATUS_UNKNOWN;

    pLink->PortID    = (LSA_UINT16)(Ctr + 1UL); /* PortId: 1 ..... n */

    POF_LOWER_TRACE_03(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, ">>>: EDD-Request: EDD_SRV_LINK_STATUS_IND_PROVIDE_EXT, Handle:0x%X EDDHandle:0x%X pRQB:0x%X",
                       pHDB->ThisHandle, pHDB->EDDHandle, pRQB);

    POF_EDD_REQUEST_LOWER(pRQB, pSys);

    pHDB->LinkIndPendingCnt++;  /* number of Link-Ind-Request pending within EDD */
  } //for-loop

  POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_EDDLinkIndProvide (Status:POF_RSP_OK)");

  return POF_RSP_OK;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_EDDLinkIndProvideDone()                 +*/
/*+  Input                 :    ...                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: EDD LinkStatus Indication Request done (Link changed)     +*/
/*+                                                                         +*/
/*+               Copy link-status to port-managment                        +*/
/*+                                                                         +*/
/*+               Calls further handling                                    +*/
/*+                                                                         +*/
/*+               Reprovides request to EDD                                 +*/
/*+                                                                         +*/
/*+               If Request was canceled we free the RQB and               +*/
/*+               don't reprovide.                                          +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static  LSA_VOID  POF_LOCAL_FCT_ATTR  POF_EDDLinkIndProvideDone( POF_EDD_LOWER_RQB_PTR_TYPE  const  pRQB,
                                                                 POF_HDB_PTR_TYPE            const  pHDB )
{
  LSA_UINT16                                    RetVal;
  POF_EDD_RQB_LINK_STATUS_IND_PROVIDE_PTR_TYPE  pLink;

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_EDDLinkIndProvideDone, pRQB:0x%X pHDB:0x%X", pRQB, pHDB);

  POF_LOWER_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "<<<: EDD_SRV_LINK_STATUS_IND_PROVIDE_EXT done, Handle:0x%X Response:0x%X",
                     pHDB->ThisHandle, POF_EDD_RQB_GET_RESPONSE(pRQB));

  /* if we are in OPEN-State we handle the Request. If not we are not open and  */
  /* we discard the request and don't reprovide it. This is the case if we are  */
  /* just closing the handle because of error while opening, or regular closing */
  /* Note: It is possible that we get a valid rqb while closing, so CANCEL      */
  /*       may not be enough to catch the rqb. We have to check the State too   */
  /*       to be sure that we don't reprovide while closing!                    */
  /* Note: It is important, that the Ind-Request will be provided in the last   */
  /*       State while opening the channel.                                     */

  if (pHDB->State != POF_HANDLE_STATE_OPEN) /* Close Channel called */
  {
    POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "EDD LinkInd while not in open-state, discard and don't reprovide");

    pHDB->LinkIndPendingCnt--;  /* number of LinkStatus-Request pending within EDD */

    POF_EDD_FREE_LOWER_MEM(&RetVal, pRQB->pParam, pHDB->Params.pSys);
    POF_MEM_FREE_FAIL_CHECK(RetVal);

    POF_EDD_FREE_LOWER_RQB(&RetVal, pRQB, pHDB->Params.pSys);
    POF_MEM_FREE_FAIL_CHECK(RetVal);
    return;
  }

  switch (POF_EDD_RQB_GET_RESPONSE(pRQB)) 
  {
    case EDD_STS_OK:
      /* ---------------------------------------------*/
      /* Successfully done the request.               */
      /* ---------------------------------------------*/
      pLink = (POF_EDD_RQB_LINK_STATUS_IND_PROVIDE_PTR_TYPE)pRQB->pParam;

      POF_PROGRAM_TRACE_05(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "EDD Link, Ch:%d Port:%d St/Sp/M:%d/%d/%d",
                           pHDB->ThisHandle, pLink->PortID, pLink->Status, pLink->Speed, pLink->Mode);

      if ((pLink->PortID > 0) && (pLink->PortID <= pHDB->PortCnt))
      {
        POF_EDD_PCTRL_PTR_TYPE  const  pPCtrl = &pHDB->pPCtrl[pLink->PortID - 1];

        /* all EDD-Link-Up-States will be mapped to EDD_LINK_UP */
        if ((pLink->Status != EDD_LINK_DOWN) && (pLink->Status != EDD_LINK_UNKNOWN))
        {
          pPCtrl->LinkStatus.Status = EDD_LINK_UP;
        }
        else
        {
          pPCtrl->LinkStatus.Status = EDD_LINK_DOWN;
        }

        pPCtrl->LinkStatus.Speed                   = pLink->Speed;
        pPCtrl->LinkStatus.Mode                    = pLink->Mode;
        pPCtrl->LinkStatus.MediaType               = pLink->MediaType;
        pPCtrl->LinkStatus.OpticalTransType        = pLink->OpticalTransType;

        pPCtrl->TimeThreshold.bMaintenanceRequired = LSA_FALSE;
        pPCtrl->TimeThreshold.bMaintenanceDemanded = LSA_FALSE;
        pPCtrl->TimeThreshold.bError               = LSA_FALSE;
        
        SFP_SetFctPtrs(pPCtrl, pLink->OpticalTransType);
        SFP_Reset_Diagnostic(pPCtrl);
      }                     

      POF_LOWER_TRACE_03(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, ">>>: EDD-Request: EDD_SRV_LINK_STATUS_IND_PROVIDE_EXT, Handle:0x%X EDDHandle:0x%X pRQB:0x%X", pHDB->ThisHandle, pHDB->EDDHandle, pRQB);
      POF_EDD_RQB_SET_LOWER_HANDLE(pRQB, pHDB->EDDHandle); /* set handle */
      POF_EDD_REQUEST_LOWER(pRQB, pHDB->Params.pSys);
      break;

    case EDD_STS_OK_CANCEL:     
      /* we canceled the request (e.g. on channel close) */
      /* Note: typically we don't get here, because the  */
      /*       channel state is <> POF_HANDLE_STATE_OPEN */
      /*       while closing!                            */

      POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "EDD LinkInd canceled, Response:0x%X", POF_EDD_RQB_GET_RESPONSE(pRQB));

      pHDB->LinkIndPendingCnt--;  /* number of LinkStatus-Request pending within EDD */
      POF_EDD_FREE_LOWER_MEM(&RetVal, pRQB->pParam, pHDB->Params.pSys);
      POF_MEM_FREE_FAIL_CHECK(RetVal);

      POF_EDD_FREE_LOWER_RQB(&RetVal, pRQB, pHDB->Params.pSys);
      POF_MEM_FREE_FAIL_CHECK(RetVal);
      break;

    default:
    {
      POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "EDD-ERROR: EDD_SRV_LINK_STATUS_IND_PROVIDE_EXT failed, Response:0x%X", POF_EDD_RQB_GET_RESPONSE(pRQB));
      POF_FATAL("POF_EDDLinkIndProvideDone", POF_FATAL_ERR_EDD, 0, 0);
      //lint -unreachable
      return;
    }
  }

  POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_EDDLinkIndProvideDone");
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_EDDGetParams()                          +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    POF_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  Result               : POF_RSP_OK                                      +*/
/*+                         POF_RSP_ERR_RESOURCE                            +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Get Params from EDD (e.g. MAC address)                    +*/
/*+               (Sends EDD_SRV_GET_MAC_ADDR request)                      +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
POF_RSP  POF_LOCAL_FCT_ATTR  POF_EDDGetParams( POF_HDB_PTR_TYPE  const  pHDB )
{
  POF_EDD_LOWER_RQB_PTR_TYPE       pRQB;
  POF_EDD_RQB_GET_PARAMS_PTR_TYPE  pGetParams;
  LSA_SYS_PTR_TYPE                 pSys;
  LSA_USER_ID_TYPE                 UserId;
  LSA_UINT16                       RetVal;

  POF_FUNCTION_TRACE_01(0, LSA_TRACE_LEVEL_CHAT, "IN: POF_EDDGetParams, pHandle:0x%X", pHDB);

  POF_CHECK_NULL_PTR(pHDB);

  pSys          = pHDB->Params.pSys;
  UserId.uvar32 = 0;              

  POF_EDD_ALLOC_LOWER_RQB(&pRQB, UserId, (LSA_UINT16)sizeof(EDD_RQB_TYPE), pSys);
  if (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL))
  {   
    POF_PROGRAM_TRACE_00(0, LSA_TRACE_LEVEL_UNEXP, "EDD-Allocating RQB lower-memory failed!");
    return POF_RSP_ERR_RESOURCE;
  }

  POF_EDD_ALLOC_LOWER_MEM((POF_EDD_LOWER_MEM_PTR_TYPE  POF_LOCAL_MEM_ATTR *)&pGetParams, 
                           UserId, 
                           (LSA_UINT16)sizeof(*pGetParams),
                           pSys);
  if (LSA_HOST_PTR_ARE_EQUAL(pGetParams, LSA_NULL))
  {   
    POF_PROGRAM_TRACE_00(0, LSA_TRACE_LEVEL_UNEXP, "EDD-Allocating MEM lower-memory failed!");
    POF_EDD_FREE_LOWER_RQB(&RetVal,pRQB,pSys);
    POF_MEM_FREE_FAIL_CHECK(RetVal);
    return POF_RSP_ERR_RESOURCE;
  }

  POF_EDD_RQB_SET_OPCODE(pRQB, EDD_OPC_REQUEST);
  POF_EDD_RQB_SET_SERVICE(pRQB, EDD_SRV_GET_PARAMS);
  POF_EDD_RQB_SET_LOWER_HANDLE(pRQB, pHDB->EDDHandle);

  pRQB->pParam               = pGetParams;

  POF_LOWER_TRACE_03(0, LSA_TRACE_LEVEL_NOTE, ">>>: EDD-Request: EDD_SRV_GET_PARAMS, Handle:0x%X EDDHandle:0x%X pRQB:0x%X",
                     pHDB->ThisHandle, pHDB->EDDHandle, pRQB);
  POF_EDD_REQUEST_LOWER(pRQB,pSys);

  POF_FUNCTION_TRACE_00(0, LSA_TRACE_LEVEL_CHAT, "OUT: POF_EDDGetParams");

  return POF_RSP_OK;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_EDDGetParamsDone()                      +*/
/*+  Input                 :    ...                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Get Params done handling                                  +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static  LSA_VOID  POF_LOCAL_FCT_ATTR  POF_EDDGetParamsDone( POF_EDD_LOWER_RQB_PTR_TYPE  const  pRQB,
                                                            POF_HDB_PTR_TYPE            const  pHDB )
{
  POF_RSP     Response; 
  LSA_UINT16  RetVal;

  POF_FUNCTION_TRACE_02(0, LSA_TRACE_LEVEL_CHAT, "IN: POF_EDDGetParamsDone, pRQB:0x%X pHDB:0x%X", pRQB, pHDB);

  POF_LOWER_TRACE_02(0, LSA_TRACE_LEVEL_NOTE, "<<<: EDD_SRV_GET_PARAMS done, Handle:0x%X Response:0x%X", pHDB->ThisHandle, POF_EDD_RQB_GET_RESPONSE(pRQB));

  Response = POF_RSP_OK;

  switch (POF_EDD_RQB_GET_RESPONSE(pRQB))
  {
    case EDD_STS_OK:
      /* ---------------------------------------------*/
      /* Successfully done the request.               */
      /* ---------------------------------------------*/
      pHDB->PortCnt      = ((POF_EDD_RQB_GET_PARAMS_PTR_TYPE)pRQB->pParam)->MaxPortCnt;
      pHDB->HardwareType = ((POF_EDD_RQB_GET_PARAMS_PTR_TYPE)pRQB->pParam)->HardwareType;
      pHDB->TraceIdx     = ((POF_EDD_RQB_GET_PARAMS_PTR_TYPE)pRQB->pParam)->TraceIdx;
      POF_PROGRAM_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "EDD_SRV_GET_PARAMS ok, PortCnt:%d HardwareType:0x%X", pHDB->PortCnt, pHDB->HardwareType);
      break;
        
    default: 
      POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_UNEXP, "POF_EDDGetParamsDone, EDD-Service EDD_SRV_GET_PARAMS failed, Response:0x%X", POF_EDD_RQB_GET_RESPONSE(pRQB));
      Response = POF_RSP_ERR_LL;
      break;
  }

  POF_EDD_FREE_LOWER_MEM(&RetVal, pRQB->pParam, pHDB->Params.pSys);
  POF_MEM_FREE_FAIL_CHECK(RetVal);

  POF_EDD_FREE_LOWER_RQB(&RetVal, pRQB,pHDB->Params.pSys);
  POF_MEM_FREE_FAIL_CHECK(RetVal);

  POF_UserChannelStateMachine(pHDB, Response); /* further actions taken in channel handler */

  POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_EDDGetParamsDone");
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_EDDGetPortParams()                      +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    POF_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pHDB                 : Pointer to HDB                                  +*/
/*+                                                                         +*/
/*+  Result               : POF_RSP_OK                                      +*/
/*+                         POF_RSP_ERR_RESOURCE                            +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Get Params from EDD (e.g. MAC address)                    +*/
/*+               (Sends EDD_SRV_GET_MAC_ADDR request)                      +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
POF_RSP  POF_LOCAL_FCT_ATTR  POF_EDDGetPortParams( POF_HDB_PTR_TYPE  const  pHDB )
{
  POF_EDD_LOWER_RQB_PTR_TYPE            pRQB;
  POF_EDD_RQB_GET_PORT_PARAMS_PTR_TYPE  pGetPortParams;
  LSA_SYS_PTR_TYPE                      pSys;
  LSA_USER_ID_TYPE                      UserId;
  LSA_UINT16                            RetVal;
  LSA_UINT32                            Ctr;

  POF_CHECK_NULL_PTR(pHDB);

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_EDDGetPortParams, pHDB:0x%X", pHDB);

  pSys          = pHDB->Params.pSys;
  UserId.uvar32 = 0;      
  
  /*---------------------------------------------------------------------------*/
  /* allocate and send Request to EDD                                          */
  /*---------------------------------------------------------------------------*/
  pHDB->LinkIndPendingCnt = 0; /* number of Link-Ind-Request pending within EDD */

  for (Ctr = 0; Ctr < pHDB->PortCnt; Ctr++)
  {
    POF_EDD_ALLOC_LOWER_RQB(&pRQB, UserId, (LSA_UINT16)sizeof(EDD_RQB_TYPE), pSys);
    if (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL))
    {   
      POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "EDD-Allocating RQB lower-memory failed!");
      return POF_RSP_ERR_RESOURCE;
    }

    POF_EDD_ALLOC_LOWER_MEM((POF_EDD_LOWER_MEM_PTR_TYPE POF_LOCAL_MEM_ATTR *)&pGetPortParams, 
                            UserId, 
                            (LSA_UINT16)sizeof(EDD_RQB_GET_PORT_PARAMS_TYPE), 
                            pSys);
    if (LSA_HOST_PTR_ARE_EQUAL(pGetPortParams, LSA_NULL))
    {   
      POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "EDD-Allocating MEM lower-memory failed!");
      POF_EDD_FREE_LOWER_RQB(&RetVal, pRQB, pSys);
      POF_MEM_FREE_FAIL_CHECK(RetVal);
      return POF_RSP_ERR_RESOURCE;
    }

    POF_EDD_RQB_SET_OPCODE(       pRQB, EDD_OPC_REQUEST);
    POF_EDD_RQB_SET_SERVICE(      pRQB, EDD_SRV_GET_PORT_PARAMS);
    POF_EDD_RQB_SET_LOWER_HANDLE( pRQB, pHDB->EDDHandle);

    pRQB->pParam           = pGetPortParams;
    pGetPortParams->PortID = (LSA_UINT16)(Ctr + 1UL); /* PortId: 1 ..... n */

    POF_LOWER_TRACE_03(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, ">>>: EDD-Request: EDD_SRV_GET_PORT_PARAMS (Handle: 0x%X,EDDHandle: 0x%X,pRQB: 0x%X)",
                       pHDB->ThisHandle, pHDB->EDDHandle, pRQB);

    POF_EDD_REQUEST_LOWER(pRQB,pSys);

    pHDB->LinkIndPendingCnt++;  /* number of Link-Ind-Request pending within EDD */
  } /* for */    

  POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_EDDGetPortParams");

  return POF_RSP_OK;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    SFP_SetFctPtrs ()                           +*/
/*+  Input                 :    ...                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:                                                           +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_VOID POF_LOCAL_FCT_ATTR SFP_SetFctPtrs(POF_EDD_PCTRL_PTR_TYPE       pPCtrl,
                                                  LSA_UINT8              const OpticalTransType)
{
    if (EDD_PORT_OPTICALTYPE_ISSFP == OpticalTransType)
    {
        pPCtrl->ReadHW_MarginFct   = (DMI_READ_HW_MARGIN_FCT)  POF_DMI_ReadSFP_Margin;
        pPCtrl->ValidateTimeOutFct = (HW_VALIDATE_TIMEOUT_FCT) SFP_Validate_TimeOut;
    }
    else if  (EDD_PORT_OPTICALTYPE_ISPOF == OpticalTransType)
    {
        pPCtrl->ReadHW_MarginFct   = (DMI_READ_HW_MARGIN_FCT)  POF_DMI_ReadHW_Margin;
        pPCtrl->ValidateTimeOutFct = (HW_VALIDATE_TIMEOUT_FCT) POF_Validate_TimeOut;
    }
    else
    {
        pPCtrl->ReadHW_MarginFct   = (DMI_READ_HW_MARGIN_FCT)  DMI_ReadHW_MarginDummy;
        pPCtrl->ValidateTimeOutFct = (HW_VALIDATE_TIMEOUT_FCT) HW_Validate_TimeOutDummy;
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    SFP_Reset_Diagnostic()                       +*/
/*+  Input                 :    ...                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:                                                           +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_VOID POF_LOCAL_FCT_ATTR SFP_Reset_Diagnostic(POF_EDD_PCTRL_PTR_TYPE  pPCtrl)
{
    LSA_UINT32 PropIndex;
    LSA_UINT32 SlidingBit = 1;

    for (PropIndex = 0; PropIndex < POF_SFP_NUMBER_CHECK_DIAGNOSTICS; ++PropIndex, SlidingBit <<= 1)
    {
        if (SlidingBit & pPCtrl->SFP_Diagnostic.ActivationMask)
        {
            pPCtrl->SFP_Diagnostic.SFP_DiagnosticChecks[PropIndex].State = SFP_PROPERTY_STATE_NORMAL;
        }
    }
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_EDDGetPortParamsDone()                  +*/
/*+  Input                 :    ...                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Get Params done handling                                  +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static  LSA_VOID  POF_LOCAL_FCT_ATTR  POF_EDDGetPortParamsDone( POF_EDD_LOWER_RQB_PTR_TYPE  const  pRQB,
                                                                POF_HDB_PTR_TYPE            const  pHDB )
{
    POF_RSP                               Response; 
    LSA_UINT16                            RetVal;
    POF_EDD_RQB_GET_PORT_PARAMS_PTR_TYPE  pPortParam;

    POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_EDDGetPortParamsDone, pRQB:0x%X pHDB:0x%X", pRQB, pHDB);

    POF_LOWER_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "<<<: EDD_SRV_GET_PARAMS done, Handle:0x%X Response:0x%X",
                        pHDB->ThisHandle, POF_EDD_RQB_GET_RESPONSE(pRQB));

    Response = POF_RSP_OK;

    switch (POF_EDD_RQB_GET_RESPONSE(pRQB)) 
    {
        case EDD_STS_OK:
            /* ---------------------------------------------*/
            /* Successfully done the request.               */
            /* ---------------------------------------------*/
            pPortParam = (POF_EDD_RQB_GET_PORT_PARAMS_PTR_TYPE)pRQB->pParam;

            POF_PROGRAM_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "POF_EDDGetPortParamsDone, PortID:%d MediaType:%d",
                                pPortParam->PortID, pPortParam->MediaType);

            if ((pPortParam->PortID > 0) && (pPortParam->PortID <= pHDB->PortCnt))
            {
                pHDB->pPCtrl[pPortParam->PortID - 1].LinkStatus.MediaType        = pPortParam->MediaType;
                pHDB->pPCtrl[pPortParam->PortID - 1].LinkStatus.OpticalTransType = pPortParam->OpticalTransType;

                SFP_SetFctPtrs(&pHDB->pPCtrl[pPortParam->PortID - 1], pPortParam->OpticalTransType);
            }

            break;
        
        default: 
            POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "POF_EDDGetPortParamsDone, EDD-ERROR: GET PARAMS failed, Response:0x%X", POF_EDD_RQB_GET_RESPONSE(pRQB));
            Response = POF_RSP_ERR_LL;
            break;
    }

    POF_EDD_FREE_LOWER_MEM(&RetVal, pRQB->pParam, pHDB->Params.pSys);
    POF_MEM_FREE_FAIL_CHECK(RetVal);

    POF_EDD_FREE_LOWER_RQB(&RetVal, pRQB, pHDB->Params.pSys);
    POF_MEM_FREE_FAIL_CHECK(RetVal);

    pHDB->LinkIndPendingCnt--;  

    POF_UserChannelStateMachine(pHDB, Response); /* further actions taken in channel handler */

    POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_EDDGetPortParamsDone");
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_EDDOpenChannel()                        +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    POF_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  Result               : POF_RSP_OK                                      +*/
/*+                         POF_RSP_ERR_RESOURCE                            +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Issues Open-Channel Request to EDD. The Request will be   +*/
/*+               finished by calling the EDD-Callbackfunction.             +*/
/*+               pof_edd_request_lower_done() if POF_RSP_OK.               +*/
/*+                                                                         +*/
/*+               Note: Only call if no open-channel request already running+*/
/*+                     We don't support multiple open request at a time.   +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
POF_RSP  POF_LOCAL_FCT_ATTR  POF_EDDOpenChannel( POF_HDB_PTR_TYPE  const  pHDB )
{
  POF_EDD_LOWER_RQB_PTR_TYPE               pRQB;
  POF_EDD_LOWER_RQB_OPEN_CHANNEL_PTR_TYPE  pOpen;
  LSA_USER_ID_TYPE                         UserId;
  LSA_UINT16                               RetVal;

  POF_FUNCTION_TRACE_01(0, LSA_TRACE_LEVEL_CHAT, "IN: POF_EDDOpenChannel, pHDB:0x%X", pHDB);

  UserId.uvar32 = 0;              

  POF_EDD_ALLOC_LOWER_RQB(&pRQB, UserId, (LSA_UINT16)sizeof(EDD_RQB_TYPE), pHDB->Params.pSys);
  if (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL))
  {
    POF_PROGRAM_TRACE_00(0, LSA_TRACE_LEVEL_UNEXP, "POF_EDDOpenChannel, EDD-Allocating RQB memory failed!");
    return POF_RSP_ERR_RESOURCE;
  }

  POF_EDD_ALLOC_LOWER_MEM((POF_EDD_LOWER_MEM_PTR_TYPE POF_LOCAL_MEM_ATTR *)&pOpen, 
                          UserId,
                          (LSA_UINT16)sizeof(EDD_RQB_OPEN_CHANNEL_TYPE),
                          pHDB->Params.pSys);
  if (LSA_HOST_PTR_ARE_EQUAL(pOpen, LSA_NULL))
  {   
    POF_PROGRAM_TRACE_00(0, LSA_TRACE_LEVEL_UNEXP, "POF_EDDOpenChannel, EDD-Allocating MEM memory failed!");
    POF_EDD_FREE_LOWER_RQB(&RetVal, pRQB, pHDB->Params.pSys);
    POF_MEM_FREE_FAIL_CHECK(RetVal);
    return POF_RSP_ERR_RESOURCE;
  }

  POF_EDD_RQB_SET_OPCODE(pRQB, EDD_OPC_OPEN_CHANNEL);
  POF_EDD_RQB_SET_SERVICE(pRQB, 0);

  pRQB->pParam       = pOpen;
  pOpen->HandleUpper = pHDB->ThisHandle;
  pOpen->SysPath     = pHDB->Params.SysPath;
  pOpen->Cbf         = pof_edd_request_lower_done;

  POF_LOWER_TRACE_02(0, LSA_TRACE_LEVEL_NOTE, ">>>: EDD-Request: EDD_OPC_OPEN_CHANNEL, Handle:0x%X pRQB:0x%X", pHDB->ThisHandle, pRQB);

  POF_EDD_OPEN_CHANNEL_LOWER(pRQB, pHDB->Params.pSys);

  POF_FUNCTION_TRACE_00(0, LSA_TRACE_LEVEL_CHAT, "OUT: POF_EDDOpenChannel");

  return POF_RSP_OK;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_EDDOpenChannelDone()                    +*/
/*+  Input                      ...                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB                  :    Pointer to lower RQB done                   +*/
/*+  pHDB                  :    Pointer to HDB for this Channel.            +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Open Channel done handling.                               +*/
/*+                                                                         +*/
/*+               - save EDD-Channel                                        +*/
/*+               - frees RQB memory                                        +*/
/*+               - call global state handling for open channel for further +*/
/*+                 actions.                                                +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static  LSA_VOID  POF_LOCAL_FCT_ATTR  POF_EDDOpenChannelDone( POF_EDD_LOWER_RQB_PTR_TYPE  const  pRQB,
                                                              POF_HDB_PTR_TYPE            const  pHDB )
{
  LSA_UINT16  RetVal;
  POF_RSP     Response = POF_RSP_OK;

  POF_FUNCTION_TRACE_02(0, LSA_TRACE_LEVEL_CHAT, "IN: POF_EDDOpenChannelDone, pRQB:0x%X pHDB:0x%X", pRQB, pHDB);

  POF_LOWER_TRACE_02(0, LSA_TRACE_LEVEL_NOTE, "<<<: EDD_OPC_OPEN_CHANNEL done, Handle:0x%X Response:0x%X", pHDB->ThisHandle, POF_EDD_RQB_GET_RESPONSE(pRQB));

  switch (POF_EDD_RQB_GET_RESPONSE(pRQB))
  {
    case EDD_STS_OK:
    {
      POF_PROGRAM_TRACE_00(0, LSA_TRACE_LEVEL_NOTE_LOW, "EDD-Open Channel successful");
      //save EDDHandle
      pHDB->EDDHandle = ((POF_EDD_LOWER_RQB_OPEN_CHANNEL_PTR_TYPE)pRQB->pParam)->HandleLower;

      break;
    }

    case EDD_STS_ERR_RESOURCE:
    {
      POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_UNEXP, "EDD-Open Channel failed, No resources, Response:0x%X", EDD_STS_ERR_RESOURCE);
      Response = POF_RSP_ERR_RESOURCE;
      break;
    }

    default:
    {
      POF_PROGRAM_TRACE_01(0, LSA_TRACE_LEVEL_UNEXP, "EDD-Open Channel failed, Response:0x%X", POF_EDD_RQB_GET_RESPONSE(pRQB));
      Response = POF_RSP_ERR_LL;
      break;
    }
  }

  POF_EDD_FREE_LOWER_MEM(&RetVal, pRQB->pParam, pHDB->Params.pSys);
  POF_MEM_FREE_FAIL_CHECK(RetVal);

  POF_EDD_FREE_LOWER_RQB(&RetVal, pRQB, pHDB->Params.pSys);
  POF_MEM_FREE_FAIL_CHECK(RetVal);

  POF_UserChannelStateMachine(pHDB, Response); //further actions taken in channel handler

  POF_FUNCTION_TRACE_00(0, LSA_TRACE_LEVEL_CHAT, "OUT: POF_EDDOpenChannelDone");
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_EDDCloseChannel()                       +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    POF_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+  pHDB                 : User HDB                                        +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Issues Close Channel Request to EDD. The Request will be  +*/
/*+               finished by calling the EDD-Callbackfunction.             +*/
/*+               POF_edd_request_lower_done()                              +*/
/*+                                                                         +*/
/*+               Note: Only call if no close-channel request already runs  +*/
/*+                     We don't support multiple open requests at a time.  +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
POF_RSP  POF_LOCAL_FCT_ATTR  POF_EDDCloseChannel( POF_HDB_PTR_TYPE  const  pHDB )
{
  POF_EDD_LOWER_RQB_PTR_TYPE  pRQB;
  LSA_USER_ID_TYPE            UserId;

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_EDDCloseChannel, pHDB:0x%X", pHDB);

  UserId.uvar32 = 0;              

  POF_EDD_ALLOC_LOWER_RQB(&pRQB, UserId, (LSA_UINT16)sizeof(EDD_RQB_TYPE),pHDB->Params.pSys);
  if (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL))
  {                                                     
    POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "POF_EDDCloseChannel, EDD-Allocating RQB memory failed!");
    return POF_RSP_ERR_RESOURCE;
  } 

  POF_EDD_RQB_SET_OPCODE(       pRQB, EDD_OPC_CLOSE_CHANNEL);
  POF_EDD_RQB_SET_SERVICE(      pRQB, 0);
  POF_EDD_RQB_SET_LOWER_HANDLE( pRQB, pHDB->EDDHandle);

  pRQB->pParam               = LSA_NULL;

  POF_LOWER_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, ">>>: EDD-Request: EDD_OPC_CLOSE_CHANNEL, EDDHandle:0x%X pRQB:0x%X", pHDB->EDDHandle, pRQB);

  POF_EDD_CLOSE_CHANNEL_LOWER(pRQB,pHDB->Params.pSys);

  POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_EDDCloseChannel");

  return POF_RSP_OK;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_EDDCloseChannelDone()                   +*/
/*+  Input                 :    ...                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB                  :    Pointer to lower RQB done                   +*/
/*+  pHDB                  :    Pointer to HDB for this Channel.            +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Close Channel done handling.                              +*/
/*+                                                                         +*/
/*+               - frees RQB memory                                        +*/
/*+               - call global state handling for open channel for further +*/
/*+                 actions.                                                +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static  LSA_VOID  POF_LOCAL_FCT_ATTR  POF_EDDCloseChannelDone( POF_EDD_LOWER_RQB_PTR_TYPE  const  pRQB,
                                                               POF_HDB_PTR_TYPE            const  pHDB )
{
  LSA_UINT16  RetVal;
  POF_RSP     Response;

  POF_FUNCTION_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_EDDCloseChannelDone, pRQB:0x%X pHDB:0x%X", pRQB, pHDB);

  Response = POF_RSP_OK;

  POF_LOWER_TRACE_02(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "<<<: EDD_OPC_CLOSE_CHANNEL done, Handle:0x%X Response:0x%X", pHDB->ThisHandle, POF_EDD_RQB_GET_RESPONSE(pRQB));

  if (POF_EDD_RQB_GET_RESPONSE(pRQB) == EDD_STS_OK)
  {
    POF_PROGRAM_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "EDD-Close Channel successful");
  }
  else 
  {
    POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "EDD-Close Channel failed, Response:0x%X", POF_EDD_RQB_GET_RESPONSE(pRQB));
    Response = POF_RSP_ERR_LL;
  }   

  POF_EDD_FREE_LOWER_RQB(&RetVal, pRQB, pHDB->Params.pSys);
  POF_MEM_FREE_FAIL_CHECK(RetVal);

  POF_UserChannelStateMachine(pHDB, Response); /* further actions taken in channel handler */

  POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_EDDCloseChannelDone");
}
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    pof_edd_request_lower_done()                +*/
/*+  Input                 :    ...                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB                  :    Pointer to lower RQB done                   +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Callbackfunction for EDD-requests.                        +*/
/*+               Fills Error-Structure in NDB with EDD-Response            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID  POF_EDD_LOWER_IN_FCT_ATTR  pof_edd_request_lower_done( POF_EDD_LOWER_RQB_PTR_TYPE  pRQB )
{
  POF_HDB_PTR_TYPE  pHDB;

  POF_ENTER();

  POF_CHECK_NULL_PTR(pRQB);

  /* EDD-request always done from an EDD-Channel! */
  pHDB = POF_GetUserHDBFromHandle(POF_EDD_RQB_GET_HANDLE(pRQB));

  if (LSA_HOST_PTR_ARE_EQUAL(pHDB, LSA_NULL))
  {
    POF_PROGRAM_TRACE_00(0, LSA_TRACE_LEVEL_FATAL, "pof_edd_request_lower_done, pHDB == LSA_NULL");
    POF_FATAL("pof_edd_request_lower_done, pHDB == LSA_NULL", POF_FATAL_ERR_HANDLE_INVALID, 0, 0);
    //lint -unreachable
    return;
  }

  POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: pof_edd_request_lower_done, pRQB:0x%X", pRQB);

  switch (POF_EDD_RQB_GET_OPCODE(pRQB))
  {
    case EDD_OPC_OPEN_CHANNEL:
      POF_EDDOpenChannelDone(pRQB,pHDB);
      break;

    case EDD_OPC_CLOSE_CHANNEL:
      POF_EDDCloseChannelDone(pRQB,pHDB);
      break;

    case EDD_OPC_REQUEST:
      switch (POF_EDD_RQB_GET_SERVICE(pRQB)) 
      {
        case EDD_SRV_LINK_STATUS_IND_PROVIDE_EXT:
          POF_EDDLinkIndProvideDone(pRQB,pHDB);
          break;

        case EDD_SRV_GET_PARAMS:
          POF_EDDGetParamsDone(pRQB,pHDB);
          break;

        case EDD_SRV_GET_PORT_PARAMS:
          POF_EDDGetPortParamsDone(pRQB,pHDB);
          break;
          
        default:
        {
          POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "pof_edd_request_lower_done, invalid Service:0x%X", POF_EDD_RQB_GET_SERVICE(pRQB));
          POF_FATAL("pof_edd_request_lower_done, invalid Service:", POF_FATAL_ERR_INCONSISTENZ, POF_EDD_RQB_GET_SERVICE(pRQB), 0);    
          //lint -unreachable
          return;
        }
      }
      break;

    default:
    {
      POF_PROGRAM_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "pof_edd_request_lower_done, invalid Opcode:0x%X", POF_EDD_RQB_GET_OPCODE(pRQB));
      POF_FATAL("pof_edd_request_lower_done, invalid Opcode:", POF_FATAL_ERR_INCONSISTENZ, POF_EDD_RQB_GET_OPCODE(pRQB), 0);   
      //lint -unreachable
      return;
    }
  }

  POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: pof_edd_request_lower_done");

  POF_EXIT();
} 
/*---------------------- end [subroutine] ---------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    POF_Get_Port_Index()                        +*/
/*+  Input/Output          :    ...                                         +*/
/*+  Result                :    POF_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output: PortId = EDD-Port-Id                                     +*/
/*+                                                                         +*/
/*+  Result     : POF_RSP_OK                                                +*/
/*+               POF_RSP_ERR_PRM_PORTID                                    +*/
/*+               POF_RSP_ERR_PARAM                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Calculates Port_Index from PortId                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
POF_RSP  POF_LOCAL_FCT_ATTR  POF_Get_Port_Index( LSA_UINT16                         const  PortId,
                                                 LSA_UINT16  POF_LOCAL_MEM_ATTR  *  const  pPort_Index,
                                                 LSA_BOOL                           const  bPrmService,
                                                 POF_HDB_PTR_TYPE                   const  pHDB )
{
    POF_FUNCTION_TRACE_01(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN: POF_Get_Port_Index, PortId:0x%X", PortId);

    if ((0 == PortId) || (PortId > pHDB->PortCnt))
    {
        POF_SET_DETAIL_ERR(POF_ERR_PORTID);

        if (bPrmService)
        {
            if (0 == PortId)
            {
                return POF_RSP_ERR_PRM_PORTID;
            }
            else
            {
                return POF_RSP_ERR_PARAM;
            }
        }
        else
        {
            return POF_RSP_ERR_PARAM;
        }  
    }

    *pPort_Index = (LSA_UINT16)(PortId - 1);

    POF_FUNCTION_TRACE_00(pHDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "OUT: POF_Get_Port_Index");

    return POF_RSP_OK;
}
/*---------------------- end [subroutine] ---------------------------------*/


/*****************************************************************************/
/*  end of file pof_edd.c                                                    */
/*****************************************************************************/
