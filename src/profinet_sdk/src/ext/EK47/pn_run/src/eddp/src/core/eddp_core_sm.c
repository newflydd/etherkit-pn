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
/*  F i l e               &F: eddp_core_sm.c                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP Core Statemachines for Device Setup/Shutdown*/
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  01.10.08    JS    initial version.                                       */
/*  22.12.08    LRG   K32                                                    */
/*  07.05.09    JS    fix: return correct response code on error.            */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID  6                 
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_CORE_SM */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"            /* eddp headerfiles */
#include "eddp_int.h"            /* internal header  */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID) 

/*===========================================================================*/
/*                                  Types                                    */
/*===========================================================================*/

/* --------------------------------------------------------------------------*/
/* DEVICE_SETUP Fct-table                                                    */
/* --------------------------------------------------------------------------*/

typedef struct _EDDP_DDB_CORE_SM_SETUP_FCT_TBL_TYPE
{
    EDD_RSP                 LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, DeviceSetupFct)
                                       (EDDP_LOCAL_DDB_PTR_TYPE   pDDB,
                                        EDDP_UPPER_DSB_PTR_TYPE   pDSB);

    EDD_RSP                 LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, DeviceShutdownFct)
                                       (EDDP_LOCAL_DDB_PTR_TYPE   pDDB);

    LSA_UINT32              SetupMsk;

} EDDP_DDB_CORE_SM_SETUP_FCT_TBL_TYPE;

/* --------------------------------------------------------------------------*/
/* DEVICE_SHUTDOWN Fct-table                                                 */
/* --------------------------------------------------------------------------*/

typedef struct _EDDP_DDB_CORE_SM_SHUTDOWN_FCT_TBL_TYPE
{
    EDD_RSP                 LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, DeviceShutdownFct)
                                       (EDDP_LOCAL_DDB_PTR_TYPE   pDDB);

    LSA_UINT32              SetupMsk;

} EDDP_DDB_CORE_SM_SHUTDOWN_FCT_TBL_TYPE;


/*===========================================================================*/
/*                                Constants                                  */
/*===========================================================================*/

/* --------------------------------------------------------------------------*/
/* DEVICE_SETUP Fct-table                                                    */
/* --------------------------------------------------------------------------*/

/* Note: EDDP_CoreISRDeviceSetup   must be before components using ints!     */
/* Note: EDDP_CoreTimerDeviceSetup must be before components using timer!    */

static EDDP_DDB_CORE_SM_SETUP_FCT_TBL_TYPE  EDDP_CoreDeviceSetupFctTable[] =
{
    {EDDP_HALDeviceSetup        ,   EDDP_HALDeviceShutdown      , EDDP_DDB_CMP_HAL_SETUP		},  
    {EDDP_CoreTimerDeviceSetup  ,   EDDP_CoreTimerDeviceShutdown, EDDP_DDB_CMP_CORE_TIMER_SETUP	},  
    {EDDP_CoreISRDeviceSetup    ,   EDDP_CoreISRDeviceShutdown  , EDDP_DDB_CMP_CORE_ISR_SETUP	},
    {EDDP_CMDDeviceSetup		,	EDDP_CMDDeviceShutdown		, EDDP_DDB_CMP_CMD_SETUP		},
    {EDDP_SWIDeviceSetup1		,	EDDP_SWIDeviceShutdown		, EDDP_DDB_CMP_SWI_SETUP1		},
    {EDDP_GENDeviceSetup        ,   EDDP_GENDeviceShutdown      , EDDP_DDB_CMP_GEN_SETUP		},
    {EDDP_NRTDeviceSetup        ,   EDDP_NRTDeviceShutdown      , EDDP_DDB_CMP_NRT_SETUP		},  
    {EDDP_CRTDeviceSetup        ,   EDDP_CRTDeviceShutdown      , EDDP_DDB_CMP_CRT_SETUP		},
    {EDDP_K32DeviceSetup        ,   EDDP_K32DeviceShutdown      , EDDP_DDB_CMP_K32_SETUP		},
    {EDDP_SWIDeviceSetup2		,	EDDP_SWIDeviceShutdown		, EDDP_DDB_CMP_SWI_SETUP2		},

    {EDDP_CoreISRDeviceIFSetup	,   EDDP_CoreISRDeviceIFShutdown, EDDP_IDB_CMP_CORE_ISR_SETUP	},
    {EDDP_PRMDeviceIFSetup		,   EDDP_PRMDeviceIFShutdown    , EDDP_IDB_CMP_PRM_SETUP		},	
    {EDDP_GENDeviceIFSetup		,   EDDP_GENDeviceIFShutdown    , EDDP_IDB_CMP_GEN_SETUP		}, 
    {EDDP_NRTDeviceIFSetup		,   EDDP_NRTDeviceIFShutdown    , EDDP_IDB_CMP_NRT_SETUP		},  
    {EDDP_CRTDeviceIFSetup		,   EDDP_CRTDeviceIFShutdown    , EDDP_IDB_CMP_CRT_SETUP		},  
    {EDDP_K32DeviceIFSetup		,   EDDP_K32DeviceIFShutdown    , EDDP_IDB_CMP_K32_SETUP		},  
    {EDDP_SWIDeviceIFSetup		,   EDDP_SWIDeviceIFShutdown    , EDDP_IDB_CMP_SWI_SETUP		},
    {LSA_NULL					,	LSA_NULL					, 0								}, /* end of list */
};

/* --------------------------------------------------------------------------*/
/* DEVICE_SHUTDOWN Fct-table                                                 */
/* --------------------------------------------------------------------------*/

/* Note: EDDP_CoreTimerDeviceShutdown must be after components using timer!  */
/* Note: EDDP_CoreISRDeviceShutdown   must be after components using ints    */

static EDDP_DDB_CORE_SM_SHUTDOWN_FCT_TBL_TYPE  EDDP_CoreDeviceShutdownFctTable[] =
{
    { EDDP_SWIDeviceIFShutdown      , EDDP_IDB_CMP_SWI_SETUP		},
    { EDDP_PRMDeviceIFShutdown      , EDDP_IDB_CMP_PRM_SETUP		},
    { EDDP_CRTDeviceIFShutdown      , EDDP_IDB_CMP_CRT_SETUP		},
    { EDDP_NRTDeviceIFShutdown      , EDDP_IDB_CMP_NRT_SETUP		},
    { EDDP_GENDeviceIFShutdown      , EDDP_IDB_CMP_GEN_SETUP		},
    { EDDP_K32DeviceIFShutdown      , EDDP_IDB_CMP_K32_SETUP		},
    { EDDP_CoreISRDeviceIFShutdown  , EDDP_IDB_CMP_CORE_ISR_SETUP   },

    { EDDP_K32DeviceShutdown		, EDDP_DDB_CMP_K32_SETUP		},
    { EDDP_CRTDeviceShutdown		, EDDP_DDB_CMP_CRT_SETUP		},
    { EDDP_NRTDeviceShutdown		, EDDP_DDB_CMP_NRT_SETUP		},  
    { EDDP_GENDeviceShutdown		, EDDP_DDB_CMP_GEN_SETUP		},
    { EDDP_SWIDeviceShutdown		, EDDP_DDB_CMP_SWI_SETUP1		},
    { EDDP_SWIDeviceShutdown		, EDDP_DDB_CMP_SWI_SETUP2		},		
    { EDDP_K32DeviceShutdown		, EDDP_DDB_CMP_K32_SETUP		},
    { EDDP_CMDDeviceShutdown		, EDDP_DDB_CMP_CMD_SETUP		},
    { EDDP_CoreISRDeviceShutdown	, EDDP_DDB_CMP_CORE_ISR_SETUP	},  
    { EDDP_CoreTimerDeviceShutdown	, EDDP_DDB_CMP_CORE_TIMER_SETUP	},
    { EDDP_HALDeviceShutdown		, EDDP_DDB_CMP_HAL_SETUP		},
    { LSA_NULL						, 0								}/* end of list */
};


/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreDevSetupStateMachine               +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             EDD_RSP                        Response     +*/
/*+  Result                :    ---                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DDB                                       (in) +*/
/*+  Response   : Response of last action.                             (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Statemachine for DEV_SETUP                                +*/
/*+               Called from Core on DEV_SETUP Request.                    +*/
/*+               Will use the Sate within pDDB->Core.Setup.State for States+*/
/*+                                                                         +*/
/*+               EDDP_DDB_STATE_SETUP_IDLE        (if statemachine is idle)+*/
/*+               EDDP_DDB_STATE_SETUP_START       (start the statemachine )+*/
/*+               EDDP_DDB_STATE_SETUP_DO_SETUP    (calling Setupfunctions) +*/
/*+               EDDP_DDB_STATE_SETUP_DO_SHUTDOWN (calling shutdownfunct)  +*/
/*+                                                                         +*/
/*+ Start Statemachine with  EDDP_DDB_STATE_SETUP_START                     +*/
/*+                                                                         +*/
/*+               On error on setup we shutdown all previously setup        +*/
/*+               components in reverse order.                              +*/
/*+                                                                         +*/
/*+               Each subkomponent within  EDDP_CoreDeviceSetupFctTable[]  +*/
/*+               will be setup                                             +*/
/*+                                                                         +*/
/*+ Each subcomponent called can return with EDD_STS_OK_PENDING if the      +*/
/*+ subcomponent will handle the call asychnchronously. If so the           +*/
/*+ callbackfunction EDDP_CoreCBFDeviceSetup() has to be called by the      +*/
/*+ subcomponent to continue!.                                              +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreDevSetupStateMachine(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_RSP                     Response)
{
    LSA_UINT32                             ActState;
    LSA_UINT32                             NextState;
    LSA_BOOL                               Ready;
    EDD_RSP                                Response2 = EDD_STS_OK;
    EDDP_LOCAL_DDB_CORE_SM_SETUP_PTR_TYPE  pStateMachine;
    EDDP_UPPER_DEV_SETUP_PTR_TYPE          pRqbParam;

    EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN :EDDP_CoreDevSetupStateMachine(pDDB: 0x%X, Response: 0x%X)", 
        pDDB, Response);

    pStateMachine   = &pDDB->Core.Setup;
    ActState        = pStateMachine->State;  /* Get actual State of Statemachine */

    /*---------------------------------------------------------------------------*/
    /* finish handling before advance to next state/fct                          */
    /*---------------------------------------------------------------------------*/
    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CoreDevSetupStateMachine. ActState : %d", 
        ActState);

    EDDP_ASSERT_NO_NULL_PTR(pStateMachine->pRQB);

    Ready     = LSA_FALSE;
    NextState = ActState;

    switch (ActState)
    {
        /*-------------------------------------------------------------*/
        case EDDP_DDB_STATE_SETUP_START:
        /*-------------------------------------------------------------*/
        {
            pStateMachine->Response     = EDD_STS_OK;
            pDDB->Core.CmpSetup         = 0;    /* no component Setup yet */
            pStateMachine->FctTblIdx    = 0;    /* Start with Fct-Tbl Idx 0 */
            NextState                   = EDDP_DDB_STATE_SETUP_DO_SETUP;
        }
        break;

        /*-------------------------------------------------------------*/
        case EDDP_DDB_STATE_SETUP_DO_SETUP:
        /*-------------------------------------------------------------*/
        {
            /* A Setup finished. Check if OK. If not start Shutdown */
            EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CoreDevSetupStateMachine[S:%d,I:%d]. Calling DeviceSetupFct Async Response: 0x%X",
                NextState, pStateMachine->FctTblIdx, Response);

            switch (Response)
            {
                case EDD_STS_OK:
                {
                    /* Success. Mark as setup and continue */
                    pDDB->Core.CmpSetup |= EDDP_CoreDeviceSetupFctTable[pStateMachine->FctTblIdx].SetupMsk;
                    pStateMachine->FctTblIdx++;
                }
                break;

                case EDD_STS_OK_PENDING:
                {
                    EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CoreDevSetupStateMachine[S:%d,I:%d]: FATAL -> Unexpected Response: %d",
                        ActState, pStateMachine->FctTblIdx, Response);
                    EDDP_FATAL(EDDP_FATAL_ERR_DEV_SETUP);
                }
                break;

                default:
                {
                    /* Error. Start Shutdown handling */

                    pStateMachine->Response = Response; /* save for request response */

                    if (pStateMachine->FctTblIdx)
                    {
                        pStateMachine->FctTblIdx--;
                        NextState = EDDP_DDB_STATE_SETUP_DO_SHUTDOWN;
                    }
                    else
                    {   
                        /* Nothing to shutdown. -> Finish */
                        NextState = EDDP_DDB_STATE_SETUP_IDLE;
                    }
                }
                break;
            }
        }
        break;
    
        /*-------------------------------------------------------------*/
        case EDDP_DDB_STATE_SETUP_DO_SHUTDOWN:
        /*-------------------------------------------------------------*/
        {
            EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CoreDevSetupStateMachine[S:%d,I:%d]. Calling DeviceShutdownFct Async Response: 0x%X",
                NextState, pStateMachine->FctTblIdx, Response);

            switch (Response)
            {
                case EDD_STS_OK:
                {
                    pDDB->Core.CmpSetup &= ~EDDP_CoreDeviceSetupFctTable[pStateMachine->FctTblIdx].SetupMsk;

                    if (pStateMachine->FctTblIdx) /* not the last one ?*/
                    {
                        pStateMachine->FctTblIdx--;
                    }
                    else
                    {   
                        /* Nothing to shutdown. -> Finish */
                        NextState = EDDP_DDB_STATE_SETUP_IDLE;
                    }
                }
                break;

                default:
                {
                    /* Error. Start Shutdown handling */

                    EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CoreDevSetupStateMachine[S:%d,I:%d]: FATAL -> Shutdown failed! Response: 0x%X",
                        ActState, pStateMachine->FctTblIdx, Response);
                    EDDP_FATAL(EDDP_FATAL_ERR_DEV_SETUP);
                }
                break;
            }
        }
        break;

        /*-------------------------------------------------------------*/
        case EDDP_DDB_STATE_SETUP_IDLE:
        default:
        /*-------------------------------------------------------------*/
        {
            EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CoreDevSetupStateMachine(): FATAL -> Unexpected State: %d",
                ActState);
            EDDP_FATAL(EDDP_FATAL_ERR_DEV_SETUP);
        }
        break;
    }


    /* -----------------------------------------------------------------------*/
    /* forward in statemachine till an asynchroneous request is underway which*/
    /* we have to wait for.                                                   */ 
    /* -----------------------------------------------------------------------*/
    while (! Ready)
    {
        EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CoreDevSetupStateMachine[S:%d,I:%d] loop",
            NextState, pStateMachine->FctTblIdx);

        pStateMachine->State = NextState;

        switch (NextState)
        {
            /*-------------------------------------------------------------*/
            case EDDP_DDB_STATE_SETUP_DO_SETUP:
            /*-------------------------------------------------------------*/
            {
                if (! (0 == EDDP_CoreDeviceSetupFctTable[pStateMachine->FctTblIdx].DeviceSetupFct) )
                {
                    EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CoreDevSetupStateMachine[S:%d,I:%d]. Calling DeviceSetupFct",
                        NextState, pStateMachine->FctTblIdx);

                    Response2 = EDDP_CoreDeviceSetupFctTable[pStateMachine->FctTblIdx].DeviceSetupFct(pDDB,pStateMachine->pDSB);

                    EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CoreDevSetupStateMachine[S:%d,I:%d]. Calling DeviceSetupFct: Response: 0x%X",
                        NextState, pStateMachine->FctTblIdx, Response2);
            
                    switch (Response2)
                    {
                        case EDD_STS_OK_PENDING:
                            Ready = LSA_TRUE;
                        break;

                        case EDD_STS_OK:
                        {
                            /* mark as setup */
                            pDDB->Core.CmpSetup |= EDDP_CoreDeviceSetupFctTable[pStateMachine->FctTblIdx].SetupMsk;
                            pStateMachine->FctTblIdx++;
                        }
                        break;

                        default:
                        {
                            /* on error shutdown previously opened parts */
                            /* Error. Start Shutdown handling */

                            pStateMachine->Response = Response2; /* save for request response */

                            if (pStateMachine->FctTblIdx)
                            {
                                pStateMachine->FctTblIdx--;
                                NextState = EDDP_DDB_STATE_SETUP_DO_SHUTDOWN ;
                            }
                            else
                            {   
                                /* Nothing to shutdown. -> Finish */
                                NextState = EDDP_DDB_STATE_SETUP_IDLE;
                            }
                        }
                        break;
                    } /* switch */
                }
                else
                {
                    NextState = EDDP_DDB_STATE_SETUP_IDLE;
                }
            }
            break;


            /*-------------------------------------------------------------*/
            case EDDP_DDB_STATE_SETUP_DO_SHUTDOWN:
            /*-------------------------------------------------------------*/
            {
                EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CoreDevSetupStateMachine[S:%d,I:%d]. Calling DeviceShutdown()",
                    NextState, pStateMachine->FctTblIdx);

                Response2 = EDDP_CoreDeviceSetupFctTable[pStateMachine->FctTblIdx].DeviceShutdownFct(pDDB);

                switch (Response2)
                {
                    case EDD_STS_OK_PENDING:
                        Ready = LSA_TRUE;
                    break;

                    case EDD_STS_OK:
                    {
                        pDDB->Core.CmpSetup &= ~EDDP_CoreDeviceSetupFctTable[pStateMachine->FctTblIdx].SetupMsk;

                        if (pStateMachine->FctTblIdx) /* not the last one ?*/
                        {
                            pStateMachine->FctTblIdx--;
                        }
                        else
                        {   
                            /* Nothing to shutdown. -> Finish */
                            NextState = EDDP_DDB_STATE_SETUP_IDLE;
                        }
                    }
                    break;

                    default:
                    {
                        /* we dont expect an error! */
                        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CoreDevSetupStateMachine(): FATAL -> Shutdown of SWI failed! Response: 0x%X",
                            Response2);
                        EDDP_FATAL(EDDP_FATAL_ERR_DEV_SETUP);
                    }
                    break;
                }
            }
            break;


            /*-------------------------------------------------------------*/
            case EDDP_DDB_STATE_SETUP_IDLE:
            /*-------------------------------------------------------------*/
                Ready = LSA_TRUE;
            break;

            /*-------------------------------------------------------------*/
            case EDDP_DDB_STATE_SETUP_START:
            default:
            /*-------------------------------------------------------------*/
            {
                EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CoreDevSetupStateMachine(): FATAL -> Unexpected State: %d",
                    NextState);
                EDDP_FATAL(EDDP_FATAL_ERR_DEV_SETUP);
            }
            break;
        }
    }

    /* -------------------------------------------------------------------------*/
    /* if we now in IDLE-State we finised (successful or not)                   */
    /* -------------------------------------------------------------------------*/
    if (NextState == EDDP_DDB_STATE_SETUP_IDLE) 
    {
        /* if the Request was successful the device is no setup. */
        if (pStateMachine->Response == EDD_STS_OK)
        {
            pDDB->DevIsSetup = LSA_TRUE;
        }

        pRqbParam = (EDDP_UPPER_DEV_SETUP_PTR_TYPE) pStateMachine->pRQB->pParam;

        /* finished the request. Call CBF */
        EDDP_SYSTEM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"[H:--] <-- Request: EDDP_SRV_DEV_SETUP finished. Response: 0x%X",
            pStateMachine->Response);

        EDD_RQB_SET_RESPONSE(pStateMachine->pRQB, pStateMachine->Response);
        EDDP_GlbCallCbf(pRqbParam->Cbf,pStateMachine->pRQB,pDDB);
    }

    EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT:EDDP_CoreDevSetupStateMachine()");
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreDevShutdownStateMachine            +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             EDD_RSP                        Response     +*/
/*+  Result                :    ---                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DDB                                       (in) +*/
/*+  Response   : Response of last action.                             (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Statemachine for DEV_SHUTDOWN                             +*/
/*+               Called from Core on DEV_SHUTDOWN Request.                 +*/
/*+               Will use the State within pDDB->Core.Setup.State for State+*/
/*+                                                                         +*/
/*+               EDDP_DDB_STATE_SHUTDOWN_IDLE     (if statemachine is idle)+*/
/*+               EDDP_DDB_STATE_SHUTDOWN_START    (start the statemachine )+*/
/*+               EDDP_DDB_STATE_SHUTDOWN_DO_SHUTDOWN (shutdown components) +*/
/*+                                                                         +*/
/*+ Start Statemachine with  EDDP_DDB_STATE_SHUTDOWN_START                  +*/
/*+                                                                         +*/
/*+ Will shutdown all setup components from EDDP_CoreDeviceShutdownFctTable +*/
/*+ Components not Setup (pDDB->Core.CmpSetup) will not be shutdown!        +*/
/*+                                                                         +*/
/*+ Each subcomponent called can return with EDD_STS_OK_PENDING if the      +*/
/*+ subcomponent will handle the call asychnchronously. If so the           +*/
/*+ callbackfunction EDDP_CoreCBFDeviceShutdown() has to be called by the   +*/
/*+ subcomponent to continue!.                                              +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreDevShutdownStateMachine(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_RSP                     Response)
{
    LSA_UINT32                                  ActState;
    LSA_UINT32                                  NextState;
    LSA_BOOL                                    Ready;
    EDD_RSP                                     Response2 = EDD_STS_OK;
    EDDP_LOCAL_DDB_CORE_SM_SHUTDOWN_PTR_TYPE    pStateMachine;
    EDDP_UPPER_DEV_SHUTDOWN_PTR_TYPE            pRqbParam;

    EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN :EDDP_CoreDevShutdownStateMachine(pDDB: 0x%X, Response: 0x%X)",
        pDDB, Response);

    pStateMachine   = &pDDB->Core.Shutdown;
    ActState        = pStateMachine->State;  /* Get actual State of Statemachine */

    /*---------------------------------------------------------------------------*/
    /* finish handling before advance to next state                              */
    /*---------------------------------------------------------------------------*/

    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CoreDevShutdownStateMachine. ActState : %d", ActState);

    EDDP_ASSERT_NO_NULL_PTR(pStateMachine->pRQB);

    Ready     = LSA_FALSE;
    NextState = ActState;

    switch (ActState)
    {
        /*-------------------------------------------------------------*/
        case EDDP_DDB_STATE_SHUTDOWN_START:
        /*-------------------------------------------------------------*/
        {
            pStateMachine->Response     = EDD_STS_OK;
            pStateMachine->FctTblIdx    = 0;  /* Start with Fct-Tbl Idx 0 */
            NextState                   = EDDP_DDB_STATE_SHUTDOWN_DO_SHUTDOWN;
        }
        break;

        /*-------------------------------------------------------------*/
        case EDDP_DDB_STATE_SHUTDOWN_DO_SHUTDOWN:
        /*-------------------------------------------------------------*/
        {
            /* A Setup finished. Check if OK. If not finish */

            EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CoreDevShutdownStateMachine[S:%d,I:%d]. Calling DeviceSetupFct Async Response: 0x%X",
                NextState, pStateMachine->FctTblIdx, Response);

            switch (Response)
            {
                case EDD_STS_OK:
                {
                    /* Success. Remove mark as setup and continue */
                    pDDB->Core.CmpSetup &= ~EDDP_CoreDeviceShutdownFctTable[pStateMachine->FctTblIdx].SetupMsk;
                    pStateMachine->FctTblIdx++;
                }
                break;

                case EDD_STS_OK_PENDING:
                {
                    EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CoreDevShutdownStateMachine[S:%d,I:%d]: FATAL -> Unexpected Response: %d",
                        ActState, pStateMachine->FctTblIdx, Response);
                    EDDP_FATAL(EDDP_FATAL_ERR_DEV_SHUTDOWN);
                }
                break;

                default:
                {
                    /* Error. Finish handling */
                    pStateMachine->Response = Response; /* save for request response */
                    NextState = EDDP_DDB_STATE_SETUP_IDLE;
                }
                break;
            }
        }
        break;
    
        /*-------------------------------------------------------------*/
        case EDDP_DDB_STATE_SHUTDOWN_IDLE:
        default:
        /*-------------------------------------------------------------*/
        {
            EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CoreDevShutdownStateMachine(): FATAL -> Unexpected State: %d", ActState);
            EDDP_FATAL(EDDP_FATAL_ERR_DEV_SHUTDOWN);
        }
        break;
    }

    /* -----------------------------------------------------------------------*/
    /* forward in statemachine till an asynchroneous request is underway which*/
    /* we have to wait for.                                                   */ 
    /* -----------------------------------------------------------------------*/
    while (! Ready)
    {

        EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CoreDevShutdownStateMachine[S:%d,I:%d] loop",
            NextState, pStateMachine->FctTblIdx);

        pStateMachine->State = NextState;

        switch (NextState)
        {
            /*-------------------------------------------------------------*/
            case EDDP_DDB_STATE_SHUTDOWN_DO_SHUTDOWN:
            /*-------------------------------------------------------------*/
            {
                if (! (0 == EDDP_CoreDeviceShutdownFctTable[pStateMachine->FctTblIdx].DeviceShutdownFct) )
                {
                    /* only shutdown if setup */
                    if (pDDB->Core.CmpSetup & EDDP_CoreDeviceShutdownFctTable[pStateMachine->FctTblIdx].SetupMsk)
                    {
                        EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CoreDevShutdownStateMachine[S:%d,I:%d]. Calling DeviceShutdownFct",
                            NextState, pStateMachine->FctTblIdx);

                        Response2 = EDDP_CoreDeviceShutdownFctTable[pStateMachine->FctTblIdx].DeviceShutdownFct(pDDB);

                        EDDP_CORE_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CoreDevShutdownStateMachine[S:%d,I:%d]. Calling DeviceShutdownFct: Response: 0x%X",
                            NextState, pStateMachine->FctTblIdx, Response2);
            
                        switch (Response2)
                        {
                            case EDD_STS_OK_PENDING:
                                Ready = LSA_TRUE;
                            break;

                            case EDD_STS_OK:
                            {
                                /* mark as not setup */
                                pDDB->Core.CmpSetup &= ~EDDP_CoreDeviceShutdownFctTable[pStateMachine->FctTblIdx].SetupMsk;
                                pStateMachine->FctTblIdx++;
                            }
                            break;

                            default:
                            {
                                /* on error finish*/
                                pStateMachine->Response = Response2; /* save for request response */
                                NextState = EDDP_DDB_STATE_SETUP_IDLE;
                            }
                            break;
                        }
                    }
                    else
                    {
                        /* already shutdown. goto next */
                        EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CoreDevShutdownStateMachine[S:%d,I:%d]. Already Shutdown. Continue",
                            NextState, pStateMachine->FctTblIdx);
                        pStateMachine->FctTblIdx++;
                    }
                }
                else
                {
                    NextState = EDDP_DDB_STATE_SETUP_IDLE;
                }
            }
            break;

            /*-------------------------------------------------------------*/
            case EDDP_DDB_STATE_SHUTDOWN_IDLE:
            /*-------------------------------------------------------------*/
                Ready = LSA_TRUE;
            break;

            /*-------------------------------------------------------------*/
            case EDDP_DDB_STATE_SHUTDOWN_START:
            default:
            /*-------------------------------------------------------------*/
            {
                EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CoreDevShutdownStateMachine(): FATAL -> Unexpected State: %d",
                    ActState);
                EDDP_FATAL(EDDP_FATAL_ERR_DEV_SHUTDOWN);
            }
            break;
        }
    }

    /* -------------------------------------------------------------------------*/
    /* if we now in IDLE-State we finished (successful or not)                  */
    /* -------------------------------------------------------------------------*/
    if (NextState == EDDP_DDB_STATE_SHUTDOWN_IDLE) 
    {
        /* if the Request was successful the device is no setup. */
        if (pStateMachine->Response == EDD_STS_OK)
        {
            pDDB->DevIsSetup    = LSA_FALSE;
            pDDB->Core.CmpSetup = 0; /* should already be */
        }

        pRqbParam = (EDDP_UPPER_DEV_SHUTDOWN_PTR_TYPE) pStateMachine->pRQB->pParam;

        /* finished the request. Call CBF */
        EDDP_SYSTEM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"[H:--] <-- Request: EDDP_SRV_DEV_SHUTDOWN finished. Response: 0x%X", pStateMachine->Response);

        EDD_RQB_SET_RESPONSE(pStateMachine->pRQB, pStateMachine->Response);
        EDDP_GlbCallCbf(pRqbParam->Cbf,pStateMachine->pRQB,pDDB);
    }

    EDDP_CORE_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT:EDDP_CoreDevShutdownStateMachine()");
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreCBFDeviceSetup                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             EDD_RSP                        Response     +*/
/*+  Result                :    ---                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DDB                                       (in) +*/
/*+  Response   : Response of last action.                             (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Callbackfunction of subcomponents used with DeviceSetup.  +*/
/*+               Used with asynchronous behaviour                          +*/
/*+               Calling the DeviceSetup Statemachine.                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreCBFDeviceSetup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_RSP                     Response)
{
    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CoreCBFDeviceSetup(): Response(0x%X)", Response);

    EDDP_CoreDevSetupStateMachine (pDDB, Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreCBFDeviceShutdown                  +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             EDD_RSP                        Response     +*/
/*+  Result                :    ---                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DDB                                       (in) +*/
/*+  Response   : Response of last action.                             (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Callbackfunction of subcomponents used with DeviceShutdown+*/
/*+               Used with asynchronous behaviour                          +*/
/*+               Calling the DeviceShutdown Statemachine                   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreCBFDeviceShutdown(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_RSP                     Response)
{
    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CoreCBFDeviceShutdown(): Response(0x%X)", Response);

    /* ---------------------------------------------------------------- */
    /* special handling. if we shutdown while in setup we must call the */
    /* setup-statemachine!                                              */
    /* ---------------------------------------------------------------- */
    if (pDDB->Core.Setup.State == EDDP_DDB_STATE_SETUP_DO_SHUTDOWN)
    {
        EDDP_CoreDevSetupStateMachine(pDDB,Response);
    }
    else
    {
        EDDP_CoreDevShutdownStateMachine(pDDB,Response);
    }
}

/*****************************************************************************/
/*  end of file eddp_core_sm.c                                               */
/*****************************************************************************/
