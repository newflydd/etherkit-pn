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
/*  F i l e               &F: eddp_prm_sm.c                             :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP PRM (State machine) handling                */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  11.11.10    VS    initial version.                                       */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID   94
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_PRM_SM */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"            /* edd headerfiles */
#include "eddp_int.h"            /* internal header */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID) 





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMCommitStateMachine                              +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             EDD_RSP                         ResponseSm              +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ResponseSm     : Response of last action                                           +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: state machine for PrmCommit.                                          +*/
/*+               First call is from PrmCommit and then from states.                    +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMCommitStateMachine(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDD_RSP                     ResponseSm)
{
    EDD_RSP                             Response;
    EDDP_PRM_COMMIT_STATE_TYPE          ActState;
    EDDP_PRM_COMMIT_STATE_TYPE          NextState;
    LSA_BOOL                            Ready;
    EDDP_PRM_COMMIT_SM_PTR_TYPE         pStateMachine;
    EDDP_LOCAL_HDB_PTR_TYPE             pHDB;

    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_PRMCommitStateMachine(ResponseSm=0x%X): SWI.IRTState=0x%X", 
        ResponseSm, pDDB->SWI.IRTState);

    Response        = EDD_STS_OK;
    pStateMachine   = &pDDB->PRM.PrmCommitSm;
    ActState        = pStateMachine->State;     // actual state of state machine
    NextState       = ActState;
    Ready           = LSA_FALSE;
    pHDB            = LSA_NULL;
    
    if (ResponseSm != EDD_STS_OK)
    {
        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMCommitStateMachine(): FATAL -> INCONSISTENCE! ResponseSm=0x%X", 
            ResponseSm);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    /* ------------------------------------------------------------------------ */
    /* finish handling of actual state before go to next state                  */
    /* ------------------------------------------------------------------------ */
    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMCommitStateMachine(): ActState(%d)", ActState);

    switch (ActState)
    {
        /* ------------------------------------------------------------ */
	    case EDDP_PRM_COMMIT_STATE_START:
        /* ------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            NextState               = EDDP_PRM_COMMIT_STATE_PASSIVATE_IRT;
        }
        break;

        /* ------------------------------------------------------------ */
        case EDDP_PRM_COMMIT_STATE_PASSIVATE_IRT:
        /* ------------------------------------------------------------ */
        {
            // this state is synchron
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMCommitStateMachine(): FATAL -> The State(%d) is synchron!", 
                ActState);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;

        /* ------------------------------------------------------------ */
        case EDDP_PRM_COMMIT_STATE_PASSIVATE_CRT_PACK:
        /* ------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            NextState               = EDDP_PRM_COMMIT_STATE_ACTIVATE_CRT_PACK;
        }
        break;

        /* ------------------------------------------------------------ */
	    case EDDP_PRM_COMMIT_STATE_ACTIVATE_CRT_PACK:
        /* ------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            NextState               = EDDP_PRM_COMMIT_STATE_ACTIVATE_IRT;
        }
        break;

        /* ------------------------------------------------------------ */
        case EDDP_PRM_COMMIT_STATE_ACTIVATE_IRT:
        /* ------------------------------------------------------------ */
        {
            // this state is synchron
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMCommitStateMachine(): FATAL -> The State(%d) is synchron!", 
                ActState);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;

        /* ------------------------------------------------------------ */
        case EDDP_PRM_COMMIT_STATE_SETUP_TRANSFER_END_ON:
        /* ------------------------------------------------------------ */
        {
            // this state is synchron
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMCommitStateMachine(): FATAL -> The State(%d) is synchron!", 
                ActState);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;

        /* ------------------------------------------------------------ */
	    case EDDP_PRM_COMMIT_STATE_FINISH:
        default:
        /* ------------------------------------------------------------ */
        {
            EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMCommitStateMachine(): FATAL -> Unexpected ActState(%d)", 
                ActState);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }

    /* ------------------------------------------------------------------------ */
    /* forward the state machine to next state                                  */
    /* ------------------------------------------------------------------------ */
    while (!Ready)
    {
        EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMCommitStateMachine(): loop -> NextState(%d)", 
            NextState);

        pStateMachine->State = NextState;

        switch (NextState)
        {
            /* ------------------------------------------------------------ */
	        case EDDP_PRM_COMMIT_STATE_PASSIVATE_IRT:
            /* ------------------------------------------------------------ */
            {
                // disable all IRT settings
                EDDP_SWI_PRM_Disable_IRT (pDDB);

                NextState = EDDP_PRM_COMMIT_STATE_PASSIVATE_CRT_PACK;
            }
            break;

            /* ------------------------------------------------------------ */
	        case EDDP_PRM_COMMIT_STATE_PASSIVATE_CRT_PACK:
            /* ------------------------------------------------------------ */
            {
                //  (is DFP present in new Record)        or (was DFP already active) ?
                if ((pDDB->PRM.PrmCommitSm.IsDfpPresentB) || (pDDB->PRM.PrmCommitSm.IsDfpPresentA))
                {
                    // DFP must be passivated first
                    // start the CRT-Passivate-Pack state machine
                    EDDP_ASSERT_FALSE(pDDB->CRT.PackPassivateSm.State != EDDP_CRT_PACK_PASSIVATE_STATE_IDLE);
                    pDDB->CRT.PackPassivateSm.State = EDDP_CRT_PACK_PASSIVATE_STATE_START;
                    pDDB->CRT.PackPassivateSm.Cbf   = EDDP_PRMCommitStateMachineCbf;
                    EDDP_CRTPassivatePackStateMachine (pDDB, EDD_STS_OK);

                    Ready = LSA_TRUE;
                }
                else
                {
                    NextState = EDDP_PRM_COMMIT_STATE_ACTIVATE_CRT_PACK;
                }
            }
            break;

            /* ------------------------------------------------------------ */
	        case EDDP_PRM_COMMIT_STATE_ACTIVATE_CRT_PACK:
            /* ------------------------------------------------------------ */
            {
                // is DFP (PDIRSubframeData and PDIRData) present in new RecordsetA ?
                if ( EDDP_PRMPDIRSubframeDataIsDfpPresent(pDDB, LSA_TRUE) )
                {
                    // start the CRT-Activate-Pack state machine
                    EDDP_ASSERT_FALSE(pDDB->CRT.PackActivateSm.State != EDDP_CRT_PACK_ACTIVATE_STATE_IDLE);
                    pDDB->CRT.PackActivateSm.State  = EDDP_CRT_PACK_ACTIVATE_STATE_START;
                    pDDB->CRT.PackActivateSm.Cbf    = EDDP_PRMCommitStateMachineCbf;
                    EDDP_CRTActivatePackStateMachine (pDDB, EDD_STS_OK);

                    Ready = LSA_TRUE;
                }
                else
                {
                    NextState = EDDP_PRM_COMMIT_STATE_ACTIVATE_IRT;
                }
            }
            break;

            /* ------------------------------------------------------------ */
	        case EDDP_PRM_COMMIT_STATE_ACTIVATE_IRT:
            /* ------------------------------------------------------------ */
            {
                // set new IRT configurations
                EDDP_SWI_PRM_Configure_IRT (pDDB);

                NextState = EDDP_PRM_COMMIT_STATE_SETUP_TRANSFER_END_ON;
            }
            break;

            /* ------------------------------------------------------------ */
	        case EDDP_PRM_COMMIT_STATE_SETUP_TRANSFER_END_ON:
            /* ------------------------------------------------------------ */
            {
                // this state is only for TransferEnd mode
                if (EDDP_APPL_TIMER_CFG_MODE_TRANSFER_END == EDDP_CRTGetApplTimerMode(pDDB))
                {
                    // is PDIRData_A present ?
                    if (EDDP_PRMPDIRDataIsPDIRDataPresent(pDDB, LSA_TRUE))
                    {
                        // PDIRData is present
                        EDDP_PRMSetupTransferEnd (pDDB, EDDP_PRM_TRANSFER_END_CASE_PDIRDATA_RED);
                    }
                    else
                    {
                        // PDIRData is not present
                        EDDP_PRMSetupTransferEnd (pDDB, EDDP_PRM_TRANSFER_END_CASE_NO_PARAMETRIZATION);
                    }
                }

                NextState = EDDP_PRM_COMMIT_STATE_FINISH;
            }
            break;

             /* ------------------------------------------------------------ */
	        case EDDP_PRM_COMMIT_STATE_FINISH:
            /* ------------------------------------------------------------ */
            {
                EDD_UPPER_PRM_COMMIT_PTR_TYPE       pRqbParam;
                EDD_UPPER_RQB_PTR_TYPE              pRQB;
                EDDP_GLB_CRT_RQB_HANDLER_PTR_TYPE   pRQBHandler;
                
                pRQBHandler = &pDDB->GlbCrtRQBHandler;

                /* remove first RQB */
                pRQB = EDDP_GlbRQBQueueRemoveFromBegin(&pRQBHandler->RqbInProgressQueue);
                
                if (   (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL))            /* valid RQB? */
                    || (EDD_SRV_PRM_COMMIT != EDD_RQB_GET_SERVICE(pRQB))   /* correct service? */
                    || (0 != pRQBHandler->RqbInProgressQueue.Count)        /* exclusive RQB? */
                   )
                {
                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMCommitStateMachine(): FATAL -> INCONSISTENCE! RQB wrong or sequence error!");
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }

                pRqbParam = (EDD_UPPER_PRM_COMMIT_PTR_TYPE) pRQB->pParam;
                
                /* ---------------------------------------------------- */
                /* set RsvIntervalRed values in PrmCommit-RQB to valid  */
                /* ---------------------------------------------------- */
                EDDP_PRMPDIRDataSetupRsvIntervalRedValues (pDDB, &(pRqbParam->RsvIntervalRed), LSA_TRUE, LSA_TRUE);

                /* ---------------------------------------------------- */
                /* get Handle for this RQB                              */
                /* ---------------------------------------------------- */
                Response = EDDP_GlbHandleGetHDB (EDD_RQB_GET_HANDLE_LOWER(pRQB), &pHDB);

                if (Response != EDD_STS_OK)
                {
                    EDDP_PRM_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "EDDP_PRMCommitStateMachine(): Invalid handle lower RQB!");
                    EDD_RQB_SET_RESPONSE(pRQB, EDD_STS_ERR_PARAM);
                    EDDP_RQB_ERROR(pRQB);
                }

                /* ---------------------------------------------------- */
                /* finish the PrmCommit request                         */
                /* ---------------------------------------------------- */
                EDDP_GlbRequestFinish (pHDB, pRQB, EDD_STS_OK);

                // trigger the finish events of this Service
                EDDP_GlbTriggerFinishEventAfterRQBConfirmation (pDDB, pHDB, EDD_SRV_PRM_COMMIT);

                /* ---------------------------------------------------- */
                /* finish the state machine                             */
                /* ---------------------------------------------------- */
                Ready                   = LSA_TRUE;
                pStateMachine->State    = EDDP_PRM_COMMIT_STATE_IDLE;

                /* ---------------------------------------------------- */
                /* call RQB-Handler for other RQBs in Queue             */
                /* ---------------------------------------------------- */
                EDDP_GlbCrtRQBHandler (pDDB);
            }
            break;

            /* ------------------------------------------------------------ */
        	case EDDP_PRM_COMMIT_STATE_START:
            default:
            /* ------------------------------------------------------------ */
            {
                EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PRMCommitStateMachine(): FATAL -> Unexpected NextState(%d)", 
                    NextState);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }

    EDDP_PRM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_PRMCommitStateMachine(): Response(0x%X), SWI.IRTState=0x%X", 
        Response, pDDB->SWI.IRTState);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_PRMCommitStateMachineCbf                           +*/
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
/*+  Response       : Response of last action                                           +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Callbackfunction of PrmCommit state machine.                          +*/
/*+               This is called, if asynchron state is finished.                       +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_PRMCommitStateMachineCbf(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDD_RSP                     Response)
{
    EDDP_PRM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_PRMCommitStateMachineCbf(): Response(0x%X)", Response);

    EDDP_PRMCommitStateMachine (pDDB, Response);
}

/*****************************************************************************/
/*  end of file eddp_prm_sm.c                                                */
/*****************************************************************************/
