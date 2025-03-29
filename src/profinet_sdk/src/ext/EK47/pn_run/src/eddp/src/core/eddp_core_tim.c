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
/*  F i l e               &F: eddp_core_tim.c                           :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP timer handling functions                    */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  13.10.08    js    initial version.                                       */
/*  24.11.10    JS    changes to prevent compiler warnings                   */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID  8                 
#define EDDP_MODULE_ID     LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_CORE_TIM */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"            /* edd headerfiles */
#include "eddp_int.h"            /* internal header */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID) 

#ifdef __cplusplus
extern "C"
{
#endif

/*===========================================================================*/
/*                                global Variables                           */
/*===========================================================================*/


/*===========================================================================*/
/*                                external functions                         */
/*===========================================================================*/

/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreTimerMgmSetup                      +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Initializes the EDDP global Timer-Management              +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreTimerMgmSetup(LSA_VOID)
{
	LSA_UINT32                       i;

  	for ( i= 0; i< EDDP_MAX_TIMER; i++ )
  	{
    	pEDDPInfo->Timer[i].InUse   = LSA_FALSE;
    	pEDDPInfo->Timer[i].Running = LSA_FALSE;
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreTimerMgmFree                       +*/
/*+  Input/Output          :    ---                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Release EDDP global Timer-Management                      +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreTimerMgmFree(LSA_VOID)
{
    /* only if no more timers in use */
    if (pEDDPInfo->TimerUsedCnt)
    {
        EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CoreTimerMgmFree(): FATAL -> Timers are still in use! TimerUsedCnt=%d", 
            pEDDPInfo->TimerUsedCnt);
		EDDP_FATAL(EDDP_FATAL_ERR_TIMER_RESOURCE);
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :   EDDP_CoreTimerSetup                          +*/
/*+  Input/Output          :   LSA_TIMER_ID_TYPE                TimerID     +*/
/*+                            EDDP_LOCAL_DDB_PTR_TYPE          pDDB        +*/
/*+                            EDDP_UPPER_RQB_PTR_TYPE          pRQB        +*/
/*+                            LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, Cbf) (..)   +*/
/*+                            EDDP_LOCAL_MEM_U32_PTR_TYPE      pEDDPTimerID+*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  TimerID      LSA-Timer ID put into timer-management               (in) +*/
/*+  pDDB       : Pointer to DDB                                       (in) +*/
/*+  pRQB         Pointer to intenal RQB put into timer-management          +*/
/*+  Cbf          Optional Callbackfunction called when timer elapses.      +*/
/*+               Gets pointer to IntRQB as parameter                       +*/
/*+  pEDDPTimerID EDDP internal Timer ID (= index ) returned                +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Puts new Timer into Timer-management                      +*/
/*+               if a Cbf is present, it is called if the timer elapses.   +*/
/*+               if no Cbf is present, if the timer elapses the output     +*/
/*+               Macro EDDP_DO_TIMER_REQUEST is called with the rqb        +*/
/*+               specified (only if rqb is not underway).                  +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreTimerSetup(
	LSA_TIMER_ID_TYPE               TimerID,
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
	EDD_UPPER_RQB_PTR_TYPE          pRQB,
	LSA_VOID                        LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, Cbf) 
											   (EDDP_INTERNAL_REQUEST_PTR_TYPE   pIntRQB),
    EDDP_LOCAL_MEM_U32_PTR_TYPE     pEDDPTimerID)
{
	EDDP_LOCAL_TIMER_INFO_PTR_TYPE  pTimer;
	LSA_UINT16                      i;
	LSA_BOOL                        Found;

	pTimer  = pEDDPInfo->Timer;
	i       = 0;
	Found   = LSA_FALSE;

	while (( i < EDDP_MAX_TIMER ) && ( ! Found ))
	{
		if (! pTimer[i].InUse )
		{
				Found = LSA_TRUE;
				pTimer[i].TimerID       = TimerID;
                pTimer[i].pDDB          = pDDB;
				pTimer[i].IntRQB.pRQB   = pRQB;
				pTimer[i].IntRQB.InUse  = LSA_FALSE;
				pTimer[i].Cbf           = Cbf;

				/* set InUse last to be sure other entrys are */
				/* valid. Not that the Timer Type is volatile */
                /* so the sequence will not be changed by a   */
                /* compiler                                   */

				pTimer[i].InUse         = LSA_TRUE;  
                *pEDDPTimerID           = i;
		};
		i++;
	}

	if ( ! Found ) 
	{
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CoreTimerSetup(): FATAL -> No more Timer-Resources!");
		EDDP_FATAL(EDDP_FATAL_ERR_TIMER_RESOURCE);
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :   EDDP_CoreTimerAlloc                          +*/
/*+  Input/Output          :   EDDP_LOCAL_DDB_PTR_TYPE          pDDB        +*/
/*+                            LSA_UINT16                       TimerType   +*/
/*+                            LSA_UINT16                       TimerBase   +*/
/*+                            LSA_UINT32                       UserID      +*/
/*+                            LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, Cbf) (..)   +*/
/*+                            EDDP_LOCAL_MEM_U32_PTR_TYPE      pEDDPTimerID+*/
/*+                                                                         +*/
/*+  Result                :   EDD_RSP_OK                                   +*/
/*+                            EDD_RSP_ERR_RESOURCE                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DDB                                       (in) +*/
/*+  TimerType    LSA-TimerType                                        (in) +*/
/*+               LSA_TIMER_TYPE_ONE_SHOT                                   +*/
/*+               LSA_TIMER_TYPE_CYCLIC                                     +*/
/*+  TimerBase    LSA-TimerBase                                        (in) +*/
/*+               LSA_TIME_BASE_1MS etc                                     +*/
/*+  UserID       ID put into internal RQB                             (in) +*/
/*+  Cbf          Optional Callbackfunction called when timer elapses. (in) +*/
/*+               Gets pointer to IntRQB as parameter                       +*/
/*+               LSA_NULL for standardhandling                             +*/
/*+  pEDDPTimerID EDDP internal Timer ID (= index ) returned           (out)+*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Allocates LSA-Timer and setup EDDP-Timer management       +*/
/*+                                                                         +*/
/*+               An internal RQB is allocated and setup with hDDB,ID       +*/
/*+               and Param. If a Cbf is specified this parameters maybe    +*/
/*+               overritten within the Cbf. The Cbf is called if the timer +*/
/*+               triggers eddp_timeout().                                  +*/
/*+               pEDDPTimerID points to the EDDP timer ID needed for       +*/
/*+               further timer-functions as input parameter               +*/
/*+                                                                         +*/
/*+               Within the RQB opcode is set to EDD_OPC_SYSTEM and        +*/
/*+               service is set to EDDP_SRV_DEV_TIMER                      +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP  EDDP_LOCAL_FCT_ATTR EDDP_CoreTimerAlloc(
	EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
	LSA_UINT16                      TimerType,
	LSA_UINT16                      TimerBase,
	LSA_UINT32                      UserID,
	LSA_VOID                        LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, Cbf) 
											   (EDDP_INTERNAL_REQUEST_PTR_TYPE   pIntRQB),
    EDDP_LOCAL_MEM_U32_PTR_TYPE     pEDDPTimerID)
{
    LSA_UINT16                          RetVal;
    EDD_RSP                             Response;
    LSA_TIMER_ID_TYPE                   TimerID;
    EDD_UPPER_RQB_PTR_TYPE              pRQB;
    EDDP_UPPER_DEV_INTERNAL_PTR_TYPE    pParam;

    EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CoreTimerAlloc(TimerType=%d, TimerBase=%d)",
        TimerType, TimerBase);

    Response    = EDD_STS_OK;
    pRQB        = EDDP_GlbAllocInternalRQB();

    if( LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL) )
    {
    	Response = EDD_STS_ERR_RESOURCE;
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CoreTimerAlloc(): ERROR -> Allocation RQB memory failed");
    }
    else
    {
        EDDP_SYSTEM_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] -->: Calling EDDP_ALLOC_TIMER (TimerType=0x%X, TimerBase=0x%X).",
            TimerType, TimerBase);
       	
    	EDDP_ALLOC_TIMER(&RetVal, &TimerID, TimerType, TimerBase);

    	if (RetVal == LSA_RET_OK)
    	{
            EDDP_SYSTEM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] <--: Calling EDDP_ALLOC_TIMER success. TimerID=%d.", TimerID);

            pParam = (EDDP_UPPER_DEV_INTERNAL_PTR_TYPE)pRQB->pParam;
            EDDP_ASSERT_NO_NULL_PTR(pParam);

		    EDDP_CoreTimerSetup(TimerID,pDDB,pRQB,Cbf,pEDDPTimerID);

    		/* setup RQB */
            EDDP_GlbSetupInternalRQB(pRQB,
                                     EDDP_SRV_DEV_TIMER,
                                     0, /* handle dont care */
                                     pDDB->hDDB,
                                     UserID,
                                     *pEDDPTimerID); /* param is used for EDDTimerID! */

            pEDDPInfo->TimerUsedCnt++;
	    }
	    else
	    {
    		Response = EDD_STS_ERR_RESOURCE;
            EDDP_SYSTEM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] <--: Calling EDDP_ALLOC_TIMER failed (Response=0x%X)", RetVal);
            EDDP_GlbFreeInternalRQB(pRQB);
	    }
    }

    EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CoreTimerAlloc(): Response=0x%X, EDDPTimerID=0x%X", 
        Response, *pEDDPTimerID);

    return (Response);

}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreTimerFree                          +*/
/*+  Input/Output          :    LSA_UINT32                   EDDPTimerID    +*/
/*+  Result                :    EDD_RSP_OK                                  +*/
/*+                             EDD_RPS_ERR_SEQUENCE                        +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  EDDPTimerID   EDDP-Timer ID to free in management                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Frees timer in timermanagement and releases LSA-Timer     +*/
/*+               Fees internal RQB. If RQB still in use we abort with error+*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP  EDDP_LOCAL_FCT_ATTR EDDP_CoreTimerFree(
    LSA_UINT32      EDDPTimerID)
{
    EDDP_LOCAL_TIMER_INFO_PTR_TYPE  pTimer;
    LSA_UINT16                      RetVal;
    LSA_TIMER_ID_TYPE               TimerID;
    EDD_RSP                         Response;

    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CoreTimerFree(EDDPTimerID=0x%X)", 
        EDDPTimerID);
					       
    Response = EDD_STS_OK;

    if (EDDPTimerID < EDDP_MAX_TIMER)
    {
    	pTimer = &pEDDPInfo->Timer[EDDPTimerID];

	    if (pTimer->InUse)
	    {
		    if ( ! pTimer->IntRQB.InUse )
		    {
    			TimerID = pTimer->TimerID;

    	       	EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] -->: Calling EDDP_STOP_TIMER (TimerID=0x%X)",
                    TimerID);
                
	    		EDDP_STOP_TIMER(&RetVal,TimerID); /* just for sure. dont matter if already*/
                                                  /* stopped                              */ 

		       	EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] <--: EDDP_STOP_TIMER (Response=0x%X)",
                    RetVal);
                EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] -->: Calling EDDP_FREE_TIMER (TimerID=0x%X)",
                    TimerID);

                EDDP_FREE_TIMER(&RetVal,TimerID);
		       	
                EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] <--: EDDP_FREE_TIMER (Response=0x%X)",
                    RetVal);

    			pTimer->InUse = LSA_FALSE;

        		/* free internal RQB for Timer */
                EDDP_GlbFreeInternalRQB(pTimer->IntRQB.pRQB);

    			pTimer->IntRQB.pRQB     = LSA_NULL;
	    		pTimer->IntRQB.InUse    = LSA_FALSE;

                EDDP_ASSERT(pEDDPInfo->TimerUsedCnt);
                pEDDPInfo->TimerUsedCnt--;
		    }
    		else
	    	{
		    	Response = EDD_STS_ERR_SEQUENCE;
		   	    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CoreTimerFree(): ERROR -> RQB still in use! TimerID=0x%X",
                    EDDPTimerID);
		    }
    	}        
    }
    else
    {
       	EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CoreTimerFree(): FATAL -> Invalid TimerID(0x%X)",
            EDDPTimerID);
    	EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }

    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CoreTimerFree(): Response=0x%X", 
        Response);

    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreTimerStart                         +*/
/*+  Input/Output          :    LSA_UINT32                   EDDPTimerID    +*/
/*+                             LSA_UINT16                   Time           +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  EDDPTimerID   EDDP-Timer ID to free in management                      +*/
/*+  Time          running time                                             +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Starts Timer previously allocated with EDDP_TimerAlloc    +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID  EDDP_LOCAL_FCT_ATTR EDDP_CoreTimerStart(
	LSA_UINT32      EDDPTimerID,
	LSA_UINT16      Time)
{
    LSA_UINT16                      RetVal;
    LSA_USER_ID_TYPE                Dummy;
    EDDP_LOCAL_TIMER_INFO_PTR_TYPE  pTimer;

    EDDP_CORE_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CoreTimerStart(EDDPTimerID=0x%X, Time=0x%X)", 
        EDDPTimerID, Time);

    if (EDDPTimerID < EDDP_MAX_TIMER) 
    {
	    pTimer = &pEDDPInfo->Timer[EDDPTimerID];

	    if (pTimer->InUse)
        {
    		Dummy.uvar32 = EDDPTimerID; /* we use user-id for EDDP-Timer ID */

            EDDP_SYSTEM_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] -->: Calling EDDP_START_TIMER (TimerType=0x%X, Time=0x%X).",
                pTimer->TimerID, Time);

	    	EDDP_START_TIMER(&RetVal, pTimer->TimerID, Dummy, Time);

    		if (RetVal == LSA_RET_ERR_PARAM)
	    	{
		       	EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] <--: FATAL -> Calling EDDP_START_TIMER failed (Response=0x%X)",
                    RetVal);
			    EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    		}
            else
            {
                pTimer->Running = LSA_TRUE;
		       	EDDP_SYSTEM_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] <--: Calling EDDP_START_TIMER successful");
            }
	    }
    	else
	    {
	   	    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_TimerStart(): FATAL -> Timer not in use! TimerID=0x%X",
                EDDPTimerID);
    		EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
	    }
    }	
    else
    {
       	EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_TimerStart(): FATAL -> Wrong TimerID(0x%X)",
            EDDPTimerID);
    	EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }

    EDDP_CORE_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CoreTimerStart()");	
}	


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreTimerStop                          +*/
/*+  Input/Output          :    LSA_UINT32                   EDDPTimerID    +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  EDDPTimerID   EDDP-Timer ID from EDDP_TimerAlloc()                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Starts Timer.                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID  EDDP_LOCAL_FCT_ATTR EDDP_CoreTimerStop(
	LSA_UINT32      EDDPTimerID)
{
    LSA_UINT16                      RetVal;
    EDDP_LOCAL_TIMER_INFO_PTR_TYPE  pTimer;

    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CoreTimerStop(EDDPTimerID=0x%X)", EDDPTimerID);

    if (EDDPTimerID < EDDP_MAX_TIMER) 
    {
	    pTimer = &pEDDPInfo->Timer[EDDPTimerID];

    	if (pTimer->InUse)
        {
	       	EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] -->: Calling EDDP_STOP_TIMER (TimerID=0x%X)",
                pTimer->TimerID);

            EDDP_STOP_TIMER(&RetVal, pTimer->TimerID);

            pTimer->Running = LSA_FALSE;

	       	EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] <--: EDDP_STOP_TIMER (Response=0x%X)", RetVal);
        }
    }	
    else
    {
   	    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_TimerStart(): FATAL -> Wrong TimerID(0x%X)",
            EDDPTimerID);
	    EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }

    EDDP_CORE_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CoreTimerStop()");	
}	



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreTimerRQBFree                       +*/
/*+  Input/Output          :    LSA_UINT32                   EDDPTimerID    +*/
/*+  Result                :    LSA_BOOL                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  EDDPTimerID   EDDP-Timer ID to free in management                      +*/
/*+                                                                         +*/
/*+  Result:       LSA_TRUE:  Timer is running                              +*/
/*+                LSA_FALSE: Timer is stopped                              +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Called after RQB was received and is free for further use +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL  EDDP_LOCAL_FCT_ATTR EDDP_CoreTimerRQBFree(
	LSA_UINT32      EDDPTimerID)
{
	EDDP_LOCAL_TIMER_INFO_PTR_TYPE   pTimer;
    LSA_BOOL                         Running = LSA_FALSE;

    if (EDDPTimerID < EDDP_MAX_TIMER)
    {
    	pTimer = &pEDDPInfo->Timer[EDDPTimerID];
    	pTimer->IntRQB.InUse = LSA_FALSE;
        Running = pTimer->Running;
    }
    else
    {
   	    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CoreTimerRQBFree(): FATAL -> Wrong TimerID(0x%X)",
            EDDPTimerID);
	    EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }

    return (Running);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreTimerRQBCheckFree                  +*/
/*+  Input/Output          :    LSA_UINT32                   EDDPTimerID    +*/
/*+  Result                :    LSA_BOOL                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  EDDPTimerID    : EDDP-Timer ID to free in management                   +*/
/*+                                                                         +*/
/*+  Result : LSA_TRUE: Timer RQB is not in use                             +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Check if Timer RQB is free.                               +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CoreTimerRQBCheckFree(
	LSA_UINT32 EDDPTimerID)
{
	EDDP_LOCAL_TIMER_INFO_PTR_TYPE  pTimer;
	LSA_BOOL                        State = LSA_FALSE;

    if (EDDPTimerID < EDDP_MAX_TIMER) 
    {
    	pTimer = &pEDDPInfo->Timer[EDDPTimerID];

        if (pTimer->IntRQB.InUse)
            State = LSA_FALSE;
        else 
            State = LSA_TRUE;
    }
    else
    {
   	    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CoreTimerRQBCheckFree(): FATAL -> Wrong TimerID(0x%X)",
            EDDPTimerID);
	    EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }

	return (State);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreTimerCheckShutdown                 +*/
/*+  Input/Output          :    LSA_UINT32                   EDDPTimerID    +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  EDDPTimerID   EDDP-Timer ID                                            +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called if we are shutting down a Device  +*/
/*+               and some RQBs still underway from DO-Macro. If no more    +*/
/*+               RQB is underway we can continue shutting down by calling  +*/
/*+               the Core EDDP_CoreDevShutdownStateMachine()               +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CoreTimerCheckShutdown(
	LSA_UINT32 EDDPTimerID)
{
	EDDP_LOCAL_TIMER_INFO_PTR_TYPE   pTimer;
    EDDP_LOCAL_DDB_PTR_TYPE          pDDB;

    if (EDDPTimerID < EDDP_MAX_TIMER)
    {
    	pTimer  = &pEDDPInfo->Timer[EDDPTimerID];
        pDDB    = pTimer->pDDB;

        if (pDDB->Core.Timer.ShutdownPending )
        {
            if (pDDB->Core.Timer.RQBInUseCnt--) /* must not be 0 befor decrement */
            {
                if (!pDDB->Core.Timer.RQBInUseCnt)
                {
                    /* no more RQBs in Use by Device. we can now finish the  */
                    /* Shutdown by calling the Core Statemachine             */
                    pDDB->Core.Timer.ShutdownPending = LSA_FALSE;

               	    EDDP_CORE_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CoreTimerCheckShutdown(): All RQBs now free. Calling Core");
                    EDDP_CoreCBFDeviceShutdown(pDDB,EDD_STS_OK);
                }
            }
            else
            {
           	    EDDP_CORE_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CoreTimerCheckShutdown(): FATAL -> RQBInUseCnt is 0!?");
        	    EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
            }
        }
    }
    else
    {
   	    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CoreTimerCheckShutdown(): FATAL -> Wrong TimerID(0x%X).",
            EDDPTimerID);
	    EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreTimerRequest                       +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE     pRQB             +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         Opcode:     EDD_OPC_SYSTEM                  +*/
/*+     LSA_HANDLE_TYPE         Handle:                                     +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user (not used)           +*/
/*+     LSA_COMP_ID_TYPE        CompId:     Cmponent-ID                     +*/
/*+     EDD_SERVICE             Service:    EDD_SRV_DEV_TIMER               +*/
/*+     LSA_RESULT              Status:     Return status                   +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:     EDDP_UPPER_DEV_INTERNAL_PTR_TYPE+*/
/*+                                                                         +*/
/*+  pParam points to EDDP_RQB_DEV_INTERNAL_TYPE                            +*/
/*+                                                                         +*/
/*+     hDDB     : Device Handle                                            +*/
/*+     ID       : spezifies the timer reason                               +*/
/*+     Param    : not used.                                                +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Request initiated from eddp_timeout() for further low     +*/
/*+               prio handling of timer events.                            +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID  EDDP_LOCAL_FCT_ATTR EDDP_CoreTimerRequest(
    EDD_UPPER_RQB_PTR_TYPE pRQB)
{
    EDD_RSP                            Response;
    LSA_BOOL                           Running;
    LSA_UINT32                         EDDPTimerID;
    EDDP_UPPER_DEV_INTERNAL_PTR_TYPE   pRqbParam;
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB = LSA_NULL;

    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CoreTimerRequest(pRQB=0x%X)", pRQB);

    Response = EDD_STS_OK;

    if (LSA_HOST_PTR_ARE_EQUAL(pRQB->pParam, LSA_NULL))
    {
        Response = EDD_STS_ERR_PARAM;
	    EDDP_CORE_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CoreTimerRequest(): ERROR -> RQB->pParam is NULL!");
	    EDD_RQB_SET_RESPONSE(pRQB, EDD_STS_ERR_PARAM);
	    EDDP_RQB_ERROR(pRQB);
    }
    else
    {

        pRqbParam = (EDDP_UPPER_DEV_INTERNAL_PTR_TYPE) pRQB->pParam;

        EDDP_CORE_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CoreTimerRequest(): Timer Event! (UserId=%d, Param=%d)",
            pRqbParam->UserID, pRqbParam->Param);

        /* Param ist used for EDDPTimerID! */
        EDDPTimerID = pRqbParam->Param;

        /* Free RQB for new events and if the Timer is still running. */
        Running = EDDP_CoreTimerRQBFree(EDDPTimerID);

        if (Running)
        {
            /* Get pDDB */
            Response = EDDP_CoreDevGetDDB (pRqbParam->hDDB,&pDDB);

            if (Response != EDD_STS_OK)
            {
                EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CoreTimerRequest(): FATAL -> Invalid hDDB(0x%X)", pRqbParam->hDDB);
                EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
            }

            /* switch to TimerID specific service function */
            switch (pRqbParam->UserID)
            {
                case EDDP_TIMER_BLINK:
                {
                    EDDP_GENLEDBlink_Toggle (pDDB);
                }
                break;

                case EDDP_TIMER_FRAME_DROP:
                {
                    // check if frame drop limits have been reached within the last second
                    EDDP_PRMCheckFrameDrop (pDDB);
                }
                break;

                case EDDP_TIMER_PPM_HOL_NOT_EMPTY:
                {
                    EDDP_CRTPpmHolNotEmptyTimerPassed (pDDB);
                }
                break;

                case EDDP_TIMER_PPM_NOT_FINISHED:
                {
                    EDDP_CRTPpmNotFinishedTimerPassed (pDDB);
                }
                break;

                case EDDP_TIMER_INBOUND_LATE_ERR:
                {
                    EDDP_CRTLateErrorTimerPassed (pDDB);
                }
                break;

                #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
                case EDDP_TIMER_WRKAR_PACKGRP_DISABLE:
                {
                    #ifdef EDDP_MESSAGE
                    #ifdef EDDP_PRAGMA_MESSAGE
                    #pragma EDDP_PRAGMA_MESSAGE("Workaround CRT.WrkarPackGrpDisable for PNIP-Revision(1) - AP01372025")
                    #endif /* EDDP_PRAGMA_MESSAGE */
                    #endif /* EDDP_MESSAGE */
                    if ( EDDP_HAL_HW_PNIP_REV1 == EDDP_HALGetPnipRevision(pDDB) )
                    {
                        EDDP_CRTWrkarPackGrpDisableTimerPassed (pDDB);
                    }
                    else
                    {
                        EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CoreTimerRequest(): FATAL -> EDDP_TIMER_WRKAR_PACKGRP_DISABLE is only for PN-IP Rev1! Timer-UserID=0x%X", 
                            pRqbParam->UserID);
	                    EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
                    }
                }
                break;
                #endif

                default:
                {
                    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CoreTimerRequest(): FATAL -> Wrong Timer-UserID(0x%X)", pRqbParam->UserID);
	                EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
                }
                break;
            }
        }
        else // ! Running
        {
            EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CoreTimerRequest(): Timer no more running! Skipping. (UserId=0x%X).",
                pRqbParam->UserID);

            /* check if we are shutting down device and wait for all */
            /* RQBs to be freed. This only needs to be checked for   */
            /* timers still stopped.                                 */
            EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CoreTimerRequest(): Check for pending shutdowns (UserId=0x%X).",
                pRqbParam->UserID);
            EDDP_CoreTimerCheckShutdown(EDDPTimerID);                
        }
    }

    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CoreTimerRequest(): Response=0x%X", 
        Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreTimerDeviceSetup                   +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             EDDP_UPPER_DSB_PTR_TYPE        pDSB         +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  pDSB       : Pointer to DeviceSetupBlock                         (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDD_SRV_DEV_SETUP                                         +*/
/*+               Initializes Timer management on setup device.             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreTimerDeviceSetup(
	EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
	EDDP_UPPER_DSB_PTR_TYPE            pDSB)
{

#if (EDDP_CFG_TRACE_MODE ==  0)
    LSA_UNUSED_ARG(pDSB);
#endif

    EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] IN: EDDP_CoreTimerDeviceSetup(pDDB=0x%X, pDSB=0x%X)", pDDB, pDSB);
    pDDB->Core.Timer.RQBInUseCnt = 0;
    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] OUT: EDDP_CoreTimerDeviceSetup(): Response=0x%X", EDD_STS_OK);
    
    return (EDD_STS_OK);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreTimerDeviceShutdown                +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE           pDDB      +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_OK_PENDING                                        +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDDP_SRV_DEV_SHUTDOWN or EDDP_SRV_DEV_SETUP on error      +*/
/*+               Shutdown the Timer managment of Device.                   +*/
/*+                                                                         +*/
/*+               If some timer RQBs still in use (e.g. because the shutdown+*/
/*+               comes in before the RQB from the DO-Macro) we have to wait+*/
/*+               for all RQBs to be free -> PENDING.                       +*/
/*+                                                                         +*/
/*+               Note that all timers must be stopped by application before+*/
/*+               this function is called! So it must be called after the   +*/
/*+               shutdown of the other functions!                          +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreTimerDeviceShutdown(
	EDDP_LOCAL_DDB_PTR_TYPE            pDDB)
{
	EDDP_LOCAL_TIMER_INFO_PTR_TYPE   pTimer;
    EDD_RSP                          Response = EDD_STS_OK;
    LSA_UINT32                       i;

    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CoreTimerDeviceShutdown(pDDB=0x%X)", pDDB);

    pDDB->Core.Timer.RQBInUseCnt = 0;

    /* ---------------------------------------------------------------------*/
    /* check if a timer RQB is still underway and if so we must wait before */
    /* we continue to shutdown.                                             */
    /* ---------------------------------------------------------------------*/

    pTimer = &pEDDPInfo->Timer[0];

    for ( i=0; i < EDDP_MAX_TIMER; i++ ) 
    {
        if (   (pTimer->InUse) 
            && (LSA_HOST_PTR_ARE_EQUAL(pTimer->pDDB, pDDB))
           )
        {
            /* -------------------------------------------------------------------*/
            /* The timer must be stopped here! if not -> programming error        */
            /* -------------------------------------------------------------------*/
            if ( pTimer->Running )
            {
                EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CoreTimerDeviceShutdown(): FATAL -> Timer still Running! (0x%X)", i);
                EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
            }

            /* -------------------------------------------------------------------*/
            /* if this timer RQB is still in use we must wait till it arrives     */
            /* this may be possible if the shutdown is done before the last timer */
            /* event from DO_Macro arrvied within EDDP (this is very unlikely but */
            /* possible.                                                          */
            /* e.g. TimerEvent -> DO-MACRO -> shutdown before DO-Macro RQB arrived*/
            /* -------------------------------------------------------------------*/
            if ( pTimer->IntRQB.InUse)
            {
                /* number of RQBs still underway for this Device */
                pDDB->Core.Timer.RQBInUseCnt++;
            }
        }
        pTimer++;
    }

    if (pDDB->Core.Timer.RQBInUseCnt)
    {
        pDDB->Core.Timer.ShutdownPending = LSA_TRUE;

        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CoreTimerDeviceShutdown(): RQB underway. Delay shutdown! RQBInUseCnt=%d", 
            pDDB->Core.Timer.RQBInUseCnt);
        Response = EDD_STS_OK_PENDING;            
    }

    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] OUT: EDDP_CoreTimerDeviceShutdown(): Response=0x%X", Response);
    
    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    eddp_timeout                                +*/
/*+  Input/Output          :    LSA_TIMER_ID_TYPE       timer_id            +*/
/*+                             LSA_USER_ID_TYPE        user_id             +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  user_id      : User id of timer (Used for EDDPTimerID)                 +*/
/*+  timer_id	  : Unused argument from system adaption call; to provide 	+*/
/*					a similar API like the other EDDI/EDDS timeout functions+*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Called from systemadaption on timerevent. May be called   +*/
/*+               within ISR-Handler.                                       +*/
/*+                                                                         +*/
/*+               LSA-Systemfunction                                        +*/
/*+                                                                         +*/
/*+               We search the timermanagement for the timer and if a cbf  +*/
/*+               is configured we call the cbf to do further handling      +*/
/*+                                                                         +*/
/*+               If no cbf is present we call EDDP_DO_TIMER_REQUEST        +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_SYSTEM_IN_FCT_ATTR eddp_timeout(
	LSA_TIMER_ID_TYPE       timer_id,
	LSA_USER_ID_TYPE        user_id)
{
	EDDP_LOCAL_TIMER_INFO_PTR_TYPE   pTimer;
	LSA_UINT32                       EDDPTimerID;

	LSA_UNUSED_ARG(timer_id);

	EDDPTimerID = (LSA_UINT16) user_id.uvar32;
    
	EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] --> eddp_timeout(TimerID=0x%X)", EDDPTimerID);
   
	if (EDDPTimerID < EDDP_MAX_TIMER)
	{
		pTimer = &pEDDPInfo->Timer[EDDPTimerID];
        
        /* is timer in use and running ? */
        if ((pTimer->InUse) && (pTimer->Running))
        {
			/* if a callbackfunction is present, call it */
			if (! (0 == pTimer->Cbf) )
			{
				 pTimer->Cbf(&pTimer->IntRQB);
			}
			else /* do standard handling..*/
			{
				if (!pTimer->IntRQB.InUse)
				{   
					/* Note that the RQB is already filled up in startup! */
					pTimer->IntRQB.InUse = LSA_TRUE;

        		   	// EDDP_SYSTEM_TRACE_01(LSA_TRACE_LEVEL_CHAT,"[H:--] eddp_timeout(): calling EDDP_DO_TIMER_REQUEST (pRQB=0x%X)", pTimer->IntRQB.pRQB);
					EDDP_DO_TIMER_REQUEST(pTimer->pDDB->hSysDev,pTimer->IntRQB.pRQB);
				}   
			}            
		}		
		else
		{
		   	EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] eddp_timeout(): Timer no more in use or not running! TimerID=0x%X",
                EDDPTimerID);
		}
	}
	else
	{
	   	EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] eddp_timeout(): FATAL -> Invalid TimerID(0x%X).",
            EDDPTimerID);
		EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
	}

	EDDP_SYSTEM_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] <-- eddp_timeout()");	
}


#ifdef __cplusplus
}
#endif
/*****************************************************************************/
/*  end of file eddp_core_tim.c                                              */
/*****************************************************************************/
