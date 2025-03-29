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
/*  C o m p o n e n t     &C: PnIODDevkits                              :C&  */
/*                                                                           */
/*  F i l e               &F: ecos_os.c                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  OS abstraction layer implementation for ECOS                             */
/*                                                                           */
/*****************************************************************************/



// *---------------------------------------------------------------------
// *    OS system adation layer
// *
// *
// *
// *
// *
// *---------------------------------------------------------------------
#define PRINT_PNIOD_PLATFORM
#define USE_DEF_VA_LIST				// must be defined before include


#include "compiler.h"
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2)

    #define LTRC_ACT_MODUL_ID   112
    #define	IOD_MODULE_ID       112

    #define LOG_LSA_PACK_ID     LSA_COMP_ID_IODAPI
    #define LOG_LSA_MOD_ID      IOD_MODULE_ID

	#include "stdarg.h"
    #include "compiler_stdlibs.h"  // include standard header files
    #include "pniousrd.h"
    #include "edd_inc.h"
    #include "os.h"
    #include "os_cfg.h"
    #include "os_taskprio.h"
    #include "pnio_trace.h"
	#include "logadapt.h"
    #include "bspadapt.h"
	#include <stdlib.h>
	#include <stdio.h>
	#include <stdarg.h>
    #include <math.h>
    #include "usriod_cfg.h"

	#include "trc_if.h"
	#include "lsa_cfg.h"
	#include "lsa_sys.h"


    #include "tskma_int.h"
    #include "edd_usr.h"

	#include "rtos_os_utils.h"
#include "stdint.h"
#ifdef __ICCARM__
#include "iccarm_builtin.h"
#endif
#include "FreeRTOS.h"
#include "mpu_wrappers.h"
#include "portmacro.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
#include "task.h"
#include "limits.h"

#include "rtos_os_types.h"

#include "bsp_api.h"

TASK_PROPERTIES  TaskProp[MAXNUM_OF_TASKS];
//OS_TIMER         OsTimer[PCPNIO_TIMER_MAX];

#if (RS232_PRINT_POSTTHREAD)

	static PNIO_UINT32 TskId_PrintPost = 0;
    static PNIO_VOID   PrintRS232PostTask (PNIO_VOID);

   	static PNIO_UINT32 PrintPostQueueSize   = 0;
   	static PNIO_BOOL   PrintPostQueueEnable = PNIO_TRUE;

   	#define PRINT_POST_QUEUE_DISABLE	500		/* disable   messages to PrintRS232PostTask when queue is above */
	#define PRINT_POST_QUEUE_ENABLE		400		/* re-enable messages to PrintRS232PostTask when queue is below */

#endif

    // *-----------------------------------------------------------*
    // *  OsInit ()
    // *
    // *  must be called, before task mainAppl is started. Initializes and
    // *  provides system ressources like memory, interprocess communi-
    // *  cation mechanism, task handling.
    // *-----------------------------------------------------------*
    PNIO_UINT32 OsInit (void)
    {
		PNIO_UINT32 i;
        PNIO_UINT32 Status = LSA_OK;

		PNIO_UINT8 MutexName[12]; // length = 12 byte, MutexName[9] = Index

		OsInitialized = PNIO_FALSE;

		if (OsInitialized == PNIO_TRUE)
		{
			LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "OsInit called twice\n\n");
			return (PNIO_FALSE);
		}

		OsMemSet (&MutexName[0], 0, sizeof (MutexName));
		OsMemCpy (&MutexName[0], "PnioMutex  ", sizeof (MutexName));

		#if DEBUG_TASKCNT
			OsMemSet (&TaskCnt[0], 0, sizeof (TaskCnt));
		#endif


		// *----------------------------------------------------
		// * initialize data for taskmanagement
		// *----------------------------------------------------
		OsMemSet (&TaskProp[0], 0, sizeof (TaskProp));		// init memory for task properties
		for (i = 0; i < MAXNUM_OF_TASKS; i++)
		{
		  TaskProp[i].Occupied = PNIO_FALSE;
		}

		// *----------------------------------------------------
		// *  create all named mutexes for LSA_ENTER() and LSA_EXIT ()
		// *  (must be counting mutexes)
		// *----------------------------------------------------
		for (i = 0; i < MAXNUM_OF_NAMED_MUTEXES; i++)
		{
			MutexName[9] = '0' + (i / 10);
			MutexName[10]= '0' + (i % 10);
			/* NOTE eCOS Kernel Configuration - Priority Inversion Protection Protocols */
			NamedMutex[i].count = 0;
			//cyg_mutex_init(&(NamedMutex[i].mutex));
			//cyg_mutex_set_protocol(&NamedMutex[i].mutex,CYG_MUTEX_INHERIT);
			NamedMutex[i].mutex = xSemaphoreCreateRecursiveMutexStatic(&NamedMutex[i].xBuffer); //xSemaphoreCreateMutex();
		}
        // *----------------------------------------------------
        // *  create all binary mutexes (dynamically allocatable)
        // *  and initialize data for mutex management. Semaphore
        // *  must be  occupied after creation.
        // *----------------------------------------------------
		#if (MAXNUM_OF_BIN_SEMAPH != 0)		/* optional */
		OsMemSet (&Semaph[0], 0, sizeof (Semaph));	// init memory for task properties
		for (i = 0; i < MAXNUM_OF_BIN_SEMAPH; i++)
		{
			Semaph[i].Mutex = xSemaphoreCreateBinaryStatic(&Semaph[i].xBuffer);
		//	xSemaphoreTake(Semaph[i].Mutex, 0);
			// **** mark as free, i.e. not allocated ***
			Semaph[i].Allocated = PNIO_FALSE;
		}
		#endif /* (MAXNUM_OF_BIN_SEMAPH != 0) */

        #if (MAXNUM_OF_EVFLAGS != 0)
            // *----------------------------------------------------
            // *  create a Event Flag for use by the application.
		    // *  and initialize data for eventflag management
            // *----------------------------------------------------
		    OsMemSet (&EvFlag,	0, sizeof (EvFlag));	// init memory for task properties
		    for (i = 0; i < MAXNUM_OF_EVFLAGS; i++)
		    {
              status = sem_init(&EvFlag[i].EvHndl,0,0);
		      if (status == ERROR)
             	 LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "Error Event create\n", status);

		      EvFlag[i].Allocated = PNIO_FALSE;
		    }
        #endif


        // *----------------------------------------------------
        // * initialize timer management
        // *----------------------------------------------------
		#if DEBUG_TIMCNT
	        OsMemSet (&OsTimCounter[0], 0, sizeof (OsTimCounter));
		#endif
        OsMemSet (&OsTimer[0], 0, sizeof (OS_TIMER));            // init memory for the timers
        for (i = 0; i < PCPNIO_TIMER_MAX; i++)
        {
        	OsTimer[i].Occupied = PNIO_FALSE;
        	OsTimer[i].Running  = PNIO_FALSE;
         /* OsTimer[i].Used     = PNIO_FALSE; */
        }

        // **** mark OS adaptation initialized  before calling any OS - services ***
        OsInitialized = PNIO_TRUE;

        // *** set init value for Srand ****
	    OsSrand ( (unsigned) OsGetTime_ms());

        #if (RS232_PRINT_POSTTHREAD)
            Status = OsCreateThread  (PrintRS232PostTask, (PNIO_UINT8*)"PrintRS232PostTask", TASK_PRIO_PRINT_POST, &TskId_PrintPost);
            PNIO_APP_ASSERT(Status == PNIO_OK, "ERROR: OsCreateThread\n");
            Status = OsCreateMsgQueue (TskId_PrintPost);
            PNIO_APP_ASSERT(Status == PNIO_OK, "ERROR: OsCreateMsgQueue\n");
            Status = OsStartThread     (TskId_PrintPost);
            PNIO_APP_ASSERT(Status == PNIO_OK, "ERROR: OsStartThread\n");
        #endif
#if 0
        // *** start a high prior task, that handles timer events in  task context
        Status = OsCreateThread ((void  (*)(void))TaskOsTimer, (PNIO_UINT8*) "Task_OS_Timer", TASK_PRIO_OS_TIMER, &TskId_OsTimer);
        PNIO_APP_ASSERT(Status == PNIO_OK, "ERROR: OsCreateThread\n");
        Status = OsCreateMsgQueue (TskId_OsTimer);
        PNIO_APP_ASSERT(Status == PNIO_OK, "ERROR: OsCreateThread\n");
        Status = OsStartThread (TskId_OsTimer);
        PNIO_APP_ASSERT(Status == PNIO_OK, "ERROR: OsCreateThread\n");
#endif
        // **** initialize circular buffer for debug entries ***
#if  _DEBUG_LOGGING_CIRC_BUF
		OsInitCircBuf ();
#endif

#if(0 == IOD_USED_WITH_XHIF_HOST)
        /* If used with XHIF host, initialization is performed by
         * command send from host */
        Bsp_nv_data_init (LSA_NULL, 0);
#endif

        return (Status);
    }


    // *----------------------------------------------------------------*
  	// *
  	// *  OsGetChar (void)
  	// *
  	// *----------------------------------------------------------------*
  	// *  read a character (pressed key) from console
  	// *
  	// *----------------------------------------------------------------*
    PNIO_UINT8  OsGetChar (void)
    {
        volatile PNIO_UINT8 str[3];
        switch (PnioLogDest)
        {
            #if INCLUDE_LOGADAPT
                case 3:
                        Char = LogAdaptGetChar ();
                        break;
            #endif

            default:
                    scanf("%2s", (char*)&str);
                    while ((str[0] == 0x0a) || (str[0] == 0x00))   // hide carriage return and NUL
                    {
                        scanf("%2s", (char*)&str);
                    }
                    break;

        }
        return (str[0]);
    }

    /**
     * @brief get formated string from input
     *
     * @param[in]   pText       PNIO_INT8*     text which should be sent to stdout
     * @param[in]   *pStr   	PNIO_UINT32*   pointer to input buffer
     *
     * @return  PNIO_UINT32 - length of read string
     *
     */
    PNIO_UINT32 OsKeyScanString( PNIO_INT8* pText, PNIO_UINT8* pStr, PNIO_UINT32 MaxLen )
    {
    	PNIO_INT32 len;
    	PNIO_UINT32 NewChar;
    	if( pText )
        {
    		PNIO_printf( ( char* ) pText );
        }
    	for( len = 0; len < MaxLen; )
    	{
    		NewChar = getchar(  );
    		if( 0x0a == NewChar )	/* enter */
    		{
    			/* ignore first enter */
    			if( 0 == len )
    			{
    				/* do nothing, not even increment len */
    			}
    			else
    			{	/* some chars, then enter = valid input ended by enter */
					pStr[ len ] = 0x00;	/* \0 */
					break;
    			}
    		}
    		else
    		{
    			pStr[ len ] = NewChar;
    			len++;
    		}
    	}
    	if( len == MaxLen )
    	{
    		PNIO_printf( "ERROR: Input sequence too long\n" );
    		return 0;
    	}
    	return len;	/* do not count last \0 character */
    }


    // *----------------------------------------------------------------*
    // *
    // *  OsKeyScan32 (PNIO_UINT8* pText, PNIO_UINT32 InputBase)
    // *
    // *----------------------------------------------------------------*
    // *  prints the text and reads a value from console
    // *  new io data exchange.
    // *
    // *  Input:    pText       Textstring to print
    // *            InputBase   10:  decimal base (default), 16: hex base, others: default
    // *  Output:   return      Value
    // *----------------------------------------------------------------*
    PNIO_UINT32 OsKeyScan32 (PNIO_INT8* pText, PNIO_UINT32 InputBase)
    {
       PNIO_UINT32 val32;

       if (pText)
           PNIO_ConsolePrintf ((char*) pText);

       if (InputBase == 16)
           scanf ("%lx", &val32);
       else
           scanf ("%ld", &val32);

       return (val32);
    }


    // *-----------------------------------------------------------*
    // *  OsCreateMsgQueue ()
    // *
    // *  Create a message queue,  assigned to the spezified task
    // *
    // *
    // *-----------------------------------------------------------*
    PNIO_UINT32	OsCreateMsgQueueX (PNIO_UINT32 TaskId)
    {
		OS_ASSERT (OsInitialized == PNIO_TRUE);


		if ( (MAXNUM_OF_TASKS <= TaskId)) //||(TaskId == 0))
		{
			LSA_TRACE_02  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "Error OsCreateMsgQueue(%d) (MAXNUM_OF_TASKS==%d)\n",
						  TaskId, MAXNUM_OF_TASKS);

		}
		else
		{
			/**** create message queue ****/
			if(LSA_NULL == TaskProp[TaskId].MsgQueue)
			{
				TaskProp[TaskId].MsgQueue = xQueueCreateStatic( MAXNUM_MSG_ENTRIES, sizeof(uint32_t), (uint8_t*)&TaskProp[TaskId].xQueBuffer[0U], &TaskProp[TaskId].xStaticQueue);
			}

			if(LSA_NULL != TaskProp[TaskId].MsgQueue)
			{
				return (LSA_OK);
			}
		}

		return (PNIO_NOT_OK);   /* not ok */
    }

    PNIO_UINT32	OsCreateMsgQueue (PNIO_UINT32 TaskId)
    {
    	return OsCreateMsgQueueX(TaskId);
#if 0
		OS_ASSERT (OsInitialized == PNIO_TRUE);

		if(MAXNUM_OF_TASKS <= TaskId)
		{
			LSA_TRACE_02  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "Error OsCreateMsgQueue(%d) (MAXNUM_OF_TASKS==%d)\n",
						  TaskId, MAXNUM_OF_TASKS);

		}
		else
		{
			/**** create message queue ****/
			TaskProp[TaskId].MsgQueue = xQueueCreate( MAXNUM_MSG_ENTRIES, sizeof(uint32_t) );
			if(LSA_NULL != TaskProp[TaskId].MsgQueue)
			{
				return (LSA_OK);
			}
		}

		return (PNIO_NOT_OK);   /* not ok */
#endif
    }


#if 0
    PNIO_VOID* OsCreateQue(PNIO_VOID)
    {
    	PNIO_UINT16 nCnt = 0U;
    	PNIO_VOID *pHandle = LSA_NULL;
    	OS_MSGQ_PTR pOsQue = LSA_NULL;

		while( (LSA_NULL == pHandle) && (nCnt <OS_MAX_MSGQ))
		{
			pOsQue = &g_tOsMsgQue[nCnt++];
			if( LSA_NULL == pOsQue->xQueue)
			{
				pOsQue->xQueue = xQueueCreateStatic(OS_MAX_MSGQ_ITEM, sizeof(PNIO_UINT32), (uint8_t*)&pOsQue->xQueueBuffer[0U], &pOsQue->xStaticQueue);
				pOsQue->xQueueId = nCnt;
				pHandle = pOsQue;
				break;
			}
		}
    	return pHandle;
    }
#endif

    PNIO_VOID* OsCreateQue(PNIO_UINT16 Items, PNIO_UINT16 ItemSize)
    {
    	PNIO_VOID *pQue = LSA_NULL;
    	if( (0U < Items) && (0U < ItemSize) )
		{
    		pQue = xQueueCreate(Items, ItemSize);
		}
    	return pQue;
    }



    #if (MAXNUM_OF_BIN_SEMAPH != 0)
	    // *-----------------------------------------------------------*
	    // *  OsAllocSemB()
	    // *  Allocates a binary semaphore. The semaphore must be
        // *  occupied at initial state.
	    // *-----------------------------------------------------------*
        PNIO_UINT32 OsAllocSemB (PNIO_UINT32* pSemId)
        {
		    int i;

            OS_ASSERT (OsInitialized == PNIO_TRUE);

            if(LSA_NULL != pSemId)
            {
            	OsIntDisable();
				for (i = 1; i < MAXNUM_OF_BIN_SEMAPH; i++)
				{
					if (Semaph[i].Allocated == PNIO_FALSE)
					{
						Semaph[i].Allocated = PNIO_TRUE;
						OsIntEnable();
						*pSemId = i;
						return (PNIO_OK);
					}
				}

				OsIntEnable();
            }

		    LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "no more binary semaphore ressources\n");
		    return (PNIO_NOT_OK);


        }

	    // *-----------------------------------------------------------*
	    // *  OsFreeSemB()
	    // *  Frees a binary semaphore, that has been allocated before.
	    // *-----------------------------------------------------------*
	    PNIO_UINT32 OsFreeSemB (PNIO_UINT32 SemId)
	    {
            OS_ASSERT (OsInitialized == PNIO_TRUE);
            if ( (0U < SemId) && (SemId < MAXNUM_OF_BIN_SEMAPH) )
            {
            	Semaph[SemId].Allocated = PNIO_FALSE;
            	return (PNIO_OK);
            }

			LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "Error OsFreeSemaph %d\n", SemId);
			return (PNIO_NOT_OK);

	    }
	    // *-----------------------------------------------------------*
	    // *  OsAvailableSemB()
	    // *  Check if binary semaphore is available.
	    // *-----------------------------------------------------------*
	    PNIO_UINT32 OsAvailableSemB (PNIO_UINT32 SemId)
	    {
            OS_ASSERT (OsInitialized == PNIO_TRUE);
            if ( (0U < SemId) &&
            	 (SemId < MAXNUM_OF_BIN_SEMAPH) &&
            	 (LSA_NULL != Semaph[SemId].Mutex) &&
            	 (pdPASS == uxSemaphoreGetCount(Semaph[SemId].Mutex)) )
            {
            	return (PNIO_OK);
            }

            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "error bin semaphore %d\n", SemId);
            return (PNIO_NOT_OK);

	    }
    #endif


    // *-----------------------------------------------------------*
    // *  TxTimerCallback()
    // *
    // *  adaption of the OS timer -calling interface to LSA Callback-
    // *  interface
    // *-----------------------------------------------------------*
    extern void OsTimerCallback2( TimerHandle_t xTimer );

    /*=============================================================================
     * function name:  OsAllocTimer
     *
     * function:       allocate a timer
     *
     * parameters:     PNIO_UINT16  ...  *  timer_id:    return value: pointer to id of timer
     *
     *                 PNIO_UINT16          timer_type:  LSA_TIMER_TYPE_ONE_SHOT or
     *                                                  LSA_TIMER_TYPE_CYCLIC
     *                 PNIO_UINT16          time_base:   LSA_TIME_BASE_1MS,
     *                                                  LSA_TIME_BASE_10MS,
     *                                                  LSA_TIME_BASE_100MS,
     *                                                  LSA_TIME_BASE_1S,
     *
     *				   callback_timeout					wird bei Ablauf des Timers aufgerufen
     *
     *
     * return value:   LSA_RET_OK            timer has been allocated
     *                 LSA_RET_ERR_NO_TIMER  no timer has been allocated
     *===========================================================================*/
    PNIO_UINT32  OsAllocTimer
				    (PNIO_UINT16  *timer_id,
				     PNIO_UINT16  timer_typ,
				     PNIO_UINT16  timer_base,
				     PNIO_VOID	  *callback_timeout)
    {
        PNIO_UINT32 Index;
        PNIO_UINT32 i;
        OS_ASSERT (OsInitialized == PNIO_TRUE);

		// *---------------------------------------------------------------
		// plausibility check of the function parameters
		// *---------------------------------------------------------------
	    if (! (timer_base == LSA_TIME_BASE_1MS  ||  timer_base == LSA_TIME_BASE_10MS
	    ||  timer_base == LSA_TIME_BASE_100MS  ||  timer_base == LSA_TIME_BASE_1S) )
	    // not supported||  timer_base == LSA_TIME_BASE_10S  ||  timer_base == LSA_TIME_BASE_100S))
	    {
        	LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "pcpnio_alloc_timer:!timer_base\n");
		    return LSA_RET_ERR_PARAM;
	    }

	    if (! (timer_typ == LSA_TIMER_TYPE_ONE_SHOT || timer_typ == LSA_TIMER_TYPE_CYCLIC))
	    {
        	LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "pcpnio_alloc_timer:!timer_typ\n");
		    return LSA_RET_ERR_PARAM;
	    }

	    // *** search for free memory block for timer ***
	    Index = INVALID;
	    for (i = 1; i < PCPNIO_TIMER_MAX; i++)
	    {
			OsEnter();
			if (OsTimer[i].Occupied == PNIO_FALSE)
			{
				OsTimer[i].Occupied = PNIO_TRUE;
				/* OsTimer[i].Used  = PNIO_TRUE;*/
				Index = i;
				OsExit();
				break;
			}
			OsExit();
	    }

	    if (Index == INVALID)
	    {
        	LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "Error in OsAllocTimer\n");
            return (LSA_RET_ERR_PARAM);
	    }

	    switch (timer_base)
	    {
			case LSA_TIME_BASE_1MS:     OsTimer[Index].timer_base = 1;    	break;
			case LSA_TIME_BASE_10MS:    OsTimer[Index].timer_base = 10;   	break;
			case LSA_TIME_BASE_100MS:   OsTimer[Index].timer_base = 100; 	break;
			case LSA_TIME_BASE_1S:      OsTimer[Index].timer_base = 1000; 	break;
			case LSA_TIME_BASE_10S:	  	OsTimer[Index].timer_base = 10000; 	break;
			case LSA_TIME_BASE_100S:	OsTimer[Index].timer_base = 100000; break;
			default:
			{
			  LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "OsAllocTimer: Invalid timer base!\n");
			  return (LSA_RET_ERR_PARAM);
			}
	    }

	    OsTimer[Index].TimeVal_ms	= OsTimer[Index].timer_base;
	    OsTimer[Index].TimeVal_Ticks	= MS_TO_TICKS(OsTimer[Index].TimeVal_ms);

	    if(timer_typ == LSA_TIMER_TYPE_CYCLIC)
	    {
	    	OsTimer[Index].hTimer = xTimerCreateStatic("Timer", OsTimer[Index].TimeVal_Ticks, pdTRUE, &OsTimer[Index] , (TimerCallbackFunction_t)OsTimerCallback2, &OsTimer[Index].xTimerBuffer );
	    }
	    else
	    {
	    	OsTimer[Index].hTimer = xTimerCreateStatic("Timer", OsTimer[Index].TimeVal_Ticks, pdFALSE, &OsTimer[Index] , (TimerCallbackFunction_t)OsTimerCallback2,  &OsTimer[Index].xTimerBuffer );
	    }

	    if(LSA_NULL != OsTimer[Index].hTimer)
	    {
	    	//OsStopTimer(Index);
			OsTimer[Index].callback_timeout = (OsTimercallback) callback_timeout;	// save LSA callback function for timer-expiration
			OsTimer[Index].timer_typ        = timer_typ;	// save type (Cyclic/OneShot)
			OsTimer[Index].user_id = 0;
			OsTimer[Index].TxTimerId = Index;
			if(LSA_NULL != timer_id)
			{
				*timer_id = Index;		         // return timer id, value = 0...PCPNIO_TIMER_MAX-1
			}
			return (LSA_OK);
	    }

	    return (LSA_RET_ERR_RESOURCE);
    }


    /*=============================================================================
     * function name:  OsStartTimer
     *
     * function:       start a timer
     *
     * parameters:
     *                 PNIO_UINT16          timer_id:  id of timer
     *                 LSA_USER_ID_TYPE     user_id:   id of prefix
     *                 PNIO_UINT16          delay:     running time in msec
     *
     * return value:   LSA_RET_OK                  timer has been started
     *                 LSA_RET_OK_TIMER_RESTARTED  timer has been
     *                 restarted
     *                 LSA_RET_ERR_PARAM           timer hasn�t been started
     *                                             because of wrong timer-id
     *                                             After the expiration of the running
     *                                             time system will call edd_timeout().

     *===========================================================================*/
    PNIO_UINT32   OsStartTimer (PNIO_UINT16			timer_id,
							    PNIO_UINT32			user_id,
							    PNIO_UINT16			delay	)
    {
        OS_ASSERT (OsInitialized == PNIO_TRUE);

        if( (timer_id < PCPNIO_TIMER_MAX) )//&& (LSA_NULL != OsTimer[timer_id].hTimer))
        {
        	OS_TIMER         *pOsTimer = &OsTimer[timer_id];
        	PNIO_UINT32			TimeVal_Ticks = pOsTimer->TimeVal_Ticks;

        	OsStopTimer(timer_id);

			pOsTimer->TimeVal_ms	= pOsTimer->timer_base*delay;
			//pOsTimer->TimeVal_Ticks	= MS_TO_TICKS(pOsTimer->TimeVal_ms);
			TimeVal_Ticks = MS_TO_TICKS(pOsTimer->TimeVal_ms);
			// **** save user_id ***
			pOsTimer->user_id = user_id;
			// **** activate timer ***
			//pOsTimer->Running = PNIO_TRUE;						// set timer state: running

			if(LSA_NULL != pOsTimer->hTimer)
			{
				if(pOsTimer->TimeVal_Ticks != TimeVal_Ticks)
				{
					pOsTimer->TimeVal_Ticks = TimeVal_Ticks;
					xTimerChangePeriod(pOsTimer->hTimer, pOsTimer->TimeVal_Ticks, pOsTimer->TimeVal_Ticks);
				}

				xTimerStart(pOsTimer->hTimer, pOsTimer->TimeVal_Ticks);
			}
			else
			{
				pOsTimer->TimeVal_Ticks = TimeVal_Ticks;
			}

			pOsTimer->Running = PNIO_TRUE;						// set timer state: running

			return (LSA_OK);
        }

        return (LSA_RET_ERR_PARAM);
    }


    /*=============================================================================
     * function name:  OsStopTimer
     *
     * function:       stops an active timer
     *
     *
     *
     * return value:   LSA_RET_OK                    timer has been stopped
     *				   LSA_RET_OK_TIMER_NOT_RUNNING  timer was not running
     *				   LSA_RET_ERR_PARAM			 error occured
     *
     *===========================================================================*/
    PNIO_UINT32 OsStopTimer ( PNIO_UINT16		timer_id)
    {
       OS_ASSERT (OsInitialized == PNIO_TRUE);
       if( (timer_id < PCPNIO_TIMER_MAX) )//&& (LSA_NULL != OsTimer[timer_id].hTimer))
       {
			//if (OsTimer[timer_id].Running == PNIO_FALSE)		// timer is not running, return
			//  return (LSA_RET_OK_TIMER_NOT_RUNNING);

    	   OsTimer[timer_id].Running = PNIO_FALSE;
    	   if((LSA_NULL != OsTimer[timer_id].hTimer) && (pdPASS == xTimerIsTimerActive(OsTimer[timer_id].hTimer)) )
    	   {
    		   xTimerStop(OsTimer[timer_id].hTimer, OsTimer[timer_id].TimeVal_Ticks);
    	   }

			return (LSA_OK);
       }
       return (LSA_RET_ERR_PARAM);
    }

	/**
	 * @brief resets timer value
	 *
	 * @param[in]   timer_id        id of timer
	 *
	 * @return  PNIO_UINT32     LSA_RET_OK                    timer has been reset
	 *                          LSA_RET_ERR_PARAM             error occured
	 *
	 *	Reset can be performed only over timer, which is currently not running.
	 *	Thus, this function should be preceded by timer stop, or it should preceed timer start.
	 */
	PNIO_UINT32 OsResetTimer(PNIO_UINT16 timer_id)
	{
		if( (timer_id < PCPNIO_TIMER_MAX) && (LSA_NULL != OsTimer[timer_id].hTimer))
		{
			OsStopTimer(timer_id);
			xTimerReset(OsTimer[timer_id].hTimer, pdMS_TO_TICKS(10));
			return LSA_OK;
		}
		return (LSA_RET_ERR_PARAM);
	}


    /*=============================================================================
     * function name:  OsFreeTimer
     *
     * function:       free a timer
     *
     * parameters:     PNIO_UINT16  ...  *  ret_val_ptr:
     *                return value: LSA_RET_OK                    timer has been
     *                                                            deallocated
     *                              LSA_RET_ERR_TIMER_IS_RUNNING  because timer is
     *                                                            running timer has
     *                                                            not been
     *                                                            deallocated
     *                              LSA_RET_ERR_PARAM             no deallocation
     *                                                            because of wrong
     *                                                            timer-id
     *                 PNIO_UINT16          timer_id:  id of timer
     *
     * return value:   LSA_VOID
     *===========================================================================*/
    PNIO_UINT32   OsFreeTimer (	PNIO_UINT16 timer_id)
    {

       OS_ASSERT (OsInitialized == PNIO_TRUE);

       if( (timer_id < PCPNIO_TIMER_MAX) && (LSA_NULL != OsTimer[timer_id].hTimer))
		{
    	 //  if (OsTimer[timer_id].Running == PNIO_TRUE)
    	//	   return (LSA_RET_ERR_TIMER_IS_RUNNING);
    	   OsStopTimer(timer_id);
    	   OsWait_ms(10);
    	   xTimerDelete((TimerHandle_t)OsTimer[timer_id].hTimer, OsTimer[timer_id].TimeVal_Ticks);
    	   OsTimer[timer_id].Occupied = PNIO_FALSE;
			return LSA_OK;
		}
		return (LSA_RET_ERR_PARAM);
    }



    // *----------------------------- Task Handling -----------------------------------------------*
extern  void OsTaskEntry(void *pvParam);
    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /*+                                                                       +*/
    /*+  function name         :    OsCreateThread                            +*/
    /*+  input parameter       :    value	pause time in msec                +*/
    /*+  output parameter      :    0	    ok                                +*/
    /*+                                                                       +*/
    /*+-----------------------------------------------------------------------+*/
    /*+                                                                       +*/
    /*+  description           :    Installs a callback handler to the        +*/
    /*+                             specified interrupt                       +*/
    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    PNIO_UINT32 OsCreateThreadInternal (
					    void         (*pTaskEntry)(void),		// [in ]pointer to task handler
					    PNIO_UINT8*  TaskName,					// Taskname for Debugging
				        PNIO_UINT32  TaskPrio,					// [in ]task priority
				        PNIO_UINT32* pTaskId)					// [out] pointer to Task ID
    {
    	PNIO_INT32 i;
    	PNIO_INT32 Index = INVALID;

    	OS_ASSERT (OsInitialized == PNIO_TRUE);
    	for (i = 1; i < MAXNUM_OF_TASKS; i++)		// we dont use index 0 here
    	{
    		OsEnter();
    		if (TaskProp[i].Occupied == PNIO_FALSE)	// is it a free entry ??
    		{ // yes, now occupy it
    			TaskProp[i].Occupied = PNIO_TRUE;		// occupie entry
    			Index = i;
    			OsExit();
    			break;
    		}
            OsExit();
    	}

    	if (Index == INVALID)
    	{ // ** error: no more task can be created **
    		LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "Error in OsStartTask()\n");
		    return (PNIO_NOT_OK);
    	}

	    // create and start the task
    	TaskProp[Index].StackSize = TASK_STACKSIZE_DEFAULT;	// all stacks have Default-stacksize

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2)	/* Faster stack for EB200P */
    	TaskProp[Index].pTaskStack = &TaskStack[Index][0U];
#else
    //	TaskProp[Index].pTaskStack = malloc(TaskProp[Index].StackSize);
#endif
	   // cyg_thread_create(TaskPrio,(cyg_thread_entry_t*)pTaskEntry,0,(char*)TaskName,TaskProp[Index].pTaskStack,TaskProp[Index].StackSize,&TaskProp[Index].ThreadId, &TaskProp[Index].Thread);

    	if(LSA_NULL != pTaskId)
    	{
    		*pTaskId                   = Index;      // take the index as a os independent task ID
    	}

#if (0 < MAXSIZE_TASKNAME)
    	if(LSA_NULL != TaskName)
    	{
    		int nNamelen = strlen((const char*)TaskName);
    		if(MAXSIZE_TASKNAME < nNamelen)
    		{
    			nNamelen = MAXSIZE_TASKNAME;
    		}
    		OsMemCpy ((void*)   &(TaskProp[Index].TaskName[0]), (void*) &TaskName[0], MAXSIZE_TASKNAME);
    	}
#endif
    	TaskProp[Index].hTaskHdl = LSA_NULL;
        TaskProp[Index].pTaskEntry = pTaskEntry;
        TaskProp[Index].State = TASK_STATE_NONE;

        OsCreateMsgQueueX(Index);

    	TaskProp[Index].hTaskHdl = xTaskCreateStatic((TaskFunction_t)OsTaskEntry, (char*)TaskName, TASK_STACK_ITEMS, &TaskProp[Index], TaskPrio, TaskProp[Index].pTaskStack, &TaskProp[Index].xTaskBuffer);
    	if(LSA_NULL != TaskProp[Index].hTaskHdl)
    	{
    		TaskProp[Index].State = TASK_STATE_ENABLED;
    		//vTaskSuspend(TaskProp[Index].hTaskHdl);
    		return (PNIO_OK);
    	}
    	else
    	{
    		return (PNIO_NOT_OK);
    	}
    }

    PNIO_UINT32 OsCreateThread (
					    void         (*pTaskEntry)(void),		// [in ]pointer to task handler
					    PNIO_UINT8*  TaskName,					// Taskname for Debugging
				        PNIO_UINT32  TaskPrio,					// [in ]task priority
				        PNIO_UINT32* pTaskId)					// [out] pointer to Task ID
    {
    	PNIO_UINT32 TaskId = 0U;
    	PNIO_UINT32 Status = OsCreateThreadInternal(pTaskEntry, TaskName, TaskPrio, &TaskId);
    	if( (PNIO_OK == Status) && (LSA_NULL != TaskProp[TaskId].hTaskHdl) )
    	{
    		vTaskSuspend(TaskProp[TaskId].hTaskHdl);
    		if(NULL != pTaskId)
    		{
    			*(PNIO_UINT32*) pTaskId = TaskId;
    		}
    	}

    	if(NULL != pTaskId)
		{
			*(PNIO_UINT32*) pTaskId = TaskId;
		}

    	return Status;
    }

    PNIO_UINT32 OsCreateThreadExt (
						void         (*pTaskEntry)(void),		// [in ]pointer to task handler
						PNIO_UINT8*  TaskName,					// Taskname for Debugging
						PNIO_UINT32  TaskPrio,					// [in ]task priority
						PNIO_UINT32* pTaskId,					// [out] pointer to Task ID
						PNIO_UINT32  dwOptions)
	{
		PNIO_UINT32 TaskId = 0U;
		PNIO_UINT32 Status = OsCreateThreadInternal(pTaskEntry, TaskName, TaskPrio, &TaskId);

		if( (PNIO_OK == Status) && (LSA_NULL != TaskProp[TaskId].hTaskHdl) && (0U == (0x01 & dwOptions)) )
		{
			vTaskSuspend(TaskProp[TaskId].hTaskHdl);
		}

		if(NULL != pTaskId)
		{
			*(PNIO_UINT32*) pTaskId = TaskId;
		}

		return Status;
	}

    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /*+                                                                       +*/
    /*+  function name         :    OsStartThread                             +*/
    /*+  input parameter       :    ThreadId                                  +*/
    /*+  output parameter      :    ----                                      +*/
    /*+                                                                       +*/
    /*+-----------------------------------------------------------------------+*/
    /*+                                                                       +*/
    /*+  description           :   starts a thread, that was created with     +*/
    /*+                            OsCreateThread                             +*/
    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    PNIO_UINT32 OsStartThread ( PNIO_UINT32 ThreadId)
    {
    	OS_ASSERT (OsInitialized == PNIO_TRUE);
    	if ( (ThreadId < MAXNUM_OF_TASKS) && (LSA_NULL != TaskProp[ThreadId].hTaskHdl) )
    	{
			TASK_PROPERTIES *ptTaskProp = (TASK_PROPERTIES*)&TaskProp[ThreadId];

			switch(eTaskGetState(ptTaskProp->hTaskHdl))
			{
			case eRunning:	/* A task is querying the state of itself, so must be running. */
				ptTaskProp->State = TASK_STATE_OPERATING;
				break;
			case eReady:			/* The task being queried is in a read or pending ready list. */
			case eBlocked:		/* The task being queried is in the Blocked state. */
			case eSuspended:		/* The task being queried is in the Suspended state, or is in the Blocked state with an infinite time out. */
				vTaskResume(ptTaskProp->hTaskHdl);
				ptTaskProp->State = TASK_STATE_OPERATING;
				break;
			case eDeleted:		/* The task being queried has been deleted, but its TCB has not yet been freed. */
			case eInvalid:		/* Used as an 'invalid state' value. */
			default:
				break;

			}
    	}

    	return (PNIO_OK);
    }


    // *----------------------------------------------------------------*
    // *
    // *  PNIO_vsprintf (void)
    // *
    // *----------------------------------------------------------------*
    // *
    // *  printf on standard console
    // *
    // *  Input:    like the  printf() function
    // *  Output:   ----
    // *
    // *----------------------------------------------------------------*
    PNIO_UINT32 PNIO_vsprintf(PNIO_UINT8* pBuf, PNIO_INT8* fmt, PNIO_va_list argptr)
    {
        return (vsprintf ((char*)pBuf, fmt, argptr));
    }

    // *----------------------------------------------------------------*
    // *
    // *  PNIO_sprintf (void)
    // *
    // *----------------------------------------------------------------*
    // *
    // *  printf on standard console
    // *
    // *  Input:    like the  printf() function
    // *  Output:   ----
    // *
    // *----------------------------------------------------------------*
    PNIO_UINT32 PNIO_sprintf(PNIO_UINT8* pBuf, PNIO_INT8 * fmt, ...)
    {
        PNIO_UINT32 Cnt = 0;
	    va_list argptr = {0};
	    va_start(argptr, fmt);

 	    Cnt = PNIO_vsprintf(pBuf, fmt, argptr);
	    va_end (argptr);
        return Cnt;
    }

    // *----------------------------------------------------------------*
    // *
    // *  PNIO_snprintf (void)
    // *
    // *----------------------------------------------------------------*
    // *
    // *  printf on buffer
    // *
    // *----------------------------------------------------------------*
    PNIO_UINT32 PNIO_snprintf (PNIO_UINT8* str, PNIO_UINT32 count, PNIO_UINT8* fmt, ...)
    {
	    va_list argptr = {0};
	    LSA_INT ret;
	    LSA_UNUSED_ARG(count);
	    va_start(argptr, fmt);//(char*)(fmt));
	    ret = PNIO_vsprintf (str, (char*)(fmt), argptr);
	    if (strlen((const char *)str) >= count)
        	LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "error PNIO_snprintf\n");

	    va_end(argptr);
	    return ret;
    }

    #if (RS232_PRINT_POSTTHREAD)

        // *----------------------------------------------------------------*
        // *   TracePostTask (PNIO_VOID)
        // *----------------------------------------------------------------*
        // *  trace posttask, to print messages on lower task level
        // *
        // *  input :  ---
        // *  out   :  ---
        // *----------------------------------------------------------------*
        static PNIO_VOID PrintRS232PostTask (PNIO_VOID)
        {

            PNIO_INT8* pMsg = LSA_NULL;            // pointer to Requestblock
            LSA_UINT32 taskID;

            // Wait until own TaskID has been saved by the father process
            OsWaitOnEnable(); // must be first call in every task
            taskID = OsGetThreadId();

            // *-----------------------------------------------------------
            // * loop forever
            // *-----------------------------------------------------------
            while (1)
            {
              OsReadMessageBlocked ((void**)&pMsg, taskID);

              if (pMsg == 0)
              {
            	  printf ("ERROR: PrintRS232PostTask() pMsg=0\n");
              }
              else
              {
            	  printf ("%s", pMsg);
#if __GNUC__
            	  fflush(stdout);
#endif
                 OsFree (pMsg);
                 // remove message from queue and re-enable console print if possible
                 M_INTERR_DISABLE;
                 if (!PrintPostQueueSize)
                 {
                	 printf ("ERROR: PrintRS232PostTask() PrintPostQueueSize Underflow.\n");
                 }
                 PrintPostQueueSize--;
                 if (!PrintPostQueueEnable && (PrintPostQueueSize < PRINT_POST_QUEUE_ENABLE))
                 {
                	 PrintPostQueueEnable = PNIO_TRUE;
                 }
                 M_INTERR_ENABLE;
              }
            }
        }
    #endif


    // *----------------------------------------------------------------*
    // *   OsPrint
    // *----------------------------------------------------------------*
    // *  print to serial output
    // *
    // *  input :
    // *  out   :  ---
    // *----------------------------------------------------------------*
    static void OsPrint (PNIO_INT8 * fmt,
                         PNIO_va_list argptr)
    {
        #if (RS232_PRINT_POSTTHREAD)

            PNIO_UINT8  TmpBuf[RS232_BUF_SIZE];
            PNIO_UINT32 Status;
            PNIO_UINT32 Len;
            PNIO_UINT8* pConsBuf = LSA_NULL;	// send to print  in lower task context

            if (PrintPostQueueEnable)
            {
            	OsMemSet (&TmpBuf[0], ' ', RS232_BUF_SIZE);

            	if (PrintPostQueueSize < PRINT_POST_QUEUE_DISABLE)
            	{
            		Len  = PNIO_vsprintf(&TmpBuf[0], fmt, argptr);
                    M_INTERR_DISABLE;
            		PrintPostQueueSize++;
                    M_INTERR_ENABLE;
            	}
            	else
            	{
					Len  = PNIO_sprintf(&TmpBuf[0], "\nWARNING: OsPrint() Queue Limit Reached => Console Print DISABLED. \n\n");
                    M_INTERR_DISABLE;
            		PrintPostQueueSize++;
					PrintPostQueueEnable = PNIO_FALSE;
                    M_INTERR_ENABLE;
				}
            	if (Len > RS232_BUF_SIZE)
				{
            		printf ("WARNING: OsPrint() message to long (%d bytes) \n", (int) Len);
            		Len = RS232_BUF_SIZE;
				}

            	Status = OsAllocFX ((void**) &pConsBuf, Len+1, MEMPOOL_DEFAULT);
				if (Status == PNIO_OK)
				{
					OsMemCpy (pConsBuf, &TmpBuf[0], Len);
					*(pConsBuf+Len) = 0;    // terminate with zero
					OsSendMessage (TskId_PrintPost, (void*)pConsBuf, OS_MBX_PRIO_NORM);
				}
				else
				{
					printf ("ERROR: OsPrint() Memory Allocation of %d Bytes Failed \n", (int) Len);
				}
            }
        #else
            vprintf(fmt, argptr);
        #endif
    }


    // *----------------------------------------------------------------*
     // *
     // *  PNIO_log10 (double)
     // *
     // *----------------------------------------------------------------*
     // *
     // *  logarithm on basis 10
     // *
     // *  Input:    values
     // *  Output:   log10 of value
     // *
     // *----------------------------------------------------------------*
     double PNIO_log10(double value)
     {
         return log10(value);
     }


     // *----------------------------------------------------------------*
     // *
     // *  PNIO_pow (double,double)
     // *
     // *----------------------------------------------------------------*
     // *
     // *  returns base raised to the exp power (base^exp)
     // *
     // *  Input:    base
     // *  Input:    exponent
     // *  Output:   pow(base,exponent)
     // *
     // *----------------------------------------------------------------*
     double PNIO_pow(double base, double exponent)
     {
         return pow(base, exponent);
     }

     // *----------------------------------------------------------------*
     // *
     // *  PNIO_ConsolePrintf (void)
     // *
     // *----------------------------------------------------------------*
     // *
     // *  printf on standard console
     // *  indenpendent from log level and log destination
     // *
     // *  Input:    like the  printf() function
     // *  Output:   ----
     // *
     // *----------------------------------------------------------------*
 	void PNIO_ConsolePrintf(PNIO_INT8 * fmt, ...)
 	{
		va_list argptr = {0};
		va_start(argptr, fmt);
		OsPrint (fmt, argptr);
		va_end(argptr);
 	}

    // *----------------------------------------------------------------*
    // *
    // *  PNIO_ConsoleLog (void)
    // *
    // *----------------------------------------------------------------*
    // *
    // *  printf on standard console
    // *  if PnioLogDest is Console (1)
    // *
    // *  Input:    like the  printf() function
    // *  Output:   ----
    // *
    // *----------------------------------------------------------------*
	void PNIO_ConsoleLog(PNIO_INT8 * fmt, ...)
	{
		if (PnioLogDest == 1)
		{
			va_list argptr = {0};
			va_start(argptr, fmt);
			OsPrint (fmt, argptr);
			va_end(argptr);
		}
	}


    // *----------------------------------------------------------------*
    // *
    // *  PNIO_printf (void)
    // *
    // *----------------------------------------------------------------*
    // *
    // *  printf on standard console or memory trace buffer
    // *
    // *  Input:    like the  printf() function
    // *  Output:   ----
    // *
    // *----------------------------------------------------------------*
	void PNIO_printf(PNIO_INT8 * fmt, ...)
	{
		switch (PnioLogDest)
		{
			case 0:
					break;
			case 1:
					{
						va_list argptr = {0};
						va_start(argptr, fmt);
						OsPrint (fmt, argptr);
						va_end(argptr);
					}
					break;
			case 2:
					{
						va_list argptr = {0};
						va_start(argptr, fmt);
						OsPrint (fmt, argptr);
		                //TRACE_PRINT (fmt, (PNIO_va_list)argptr);
						va_end(argptr);
					}
					break;
            #if INCLUDE_LOGADAPT
			case 3:
					{
						va_list argptr = {0};
						va_start(argptr, fmt);
		                LogAdaptConsPrint (fmt, argptr);
						va_end(argptr);
					}
					break;
            #endif
			default:
			{

			}
		}
	}


    // *----------------------------------------------------------------*
    // *
    // *  PNIO_TrcPrintf (void)
    // *
    // *----------------------------------------------------------------*
    // *
    // *  printf of debug logging
    // *
    // *  Input:    like the  printf() function
    // *  Output:   ----
    // *
    // *----------------------------------------------------------------*
    void PNIO_TrcPrintf(PNIO_INT8 * fmt, ...)
    {
        switch (PnioLogDest)
        {
            case 0:
                    break;
            case 1: // **** print on stdout ****
	                {
	                	va_list argptr = {0};
		                va_start(argptr, fmt);
		                OsPrint (fmt, argptr);
		                va_end (argptr);
	                }
                    break;
            case 2: // **** print into tracebuffer memory ****
	                {
	                	va_list argptr = {0};
		                va_start(argptr, fmt);
						OsPrint (fmt, argptr);
		                //TRACE_PRINT (fmt, argptr);
	                    va_end (argptr);
	                }
                    break;
            #if INCLUDE_LOGADAPT
            	case 3:
				{
					PNIO_va_list argptr;
					va_start(argptr, fmt);
					LogAdaptTrcPrint (fmt, argptr);
					va_end(argptr);
				}
				break;
			#endif

            default:
                    break;

        }
    }


    // *--------------------------------------------*
    // *   OsReboot (void)
    // *   force reboot of the system
    // *--------------------------------------------*

		void OsReboot (void)
		{
        	PNIOUSR_LOWER_RQB_TYPE  *pReBootEvent;
			OsAlloc ((void**) &pReBootEvent, 0, sizeof (PNIOUSR_LOWER_RQB_TYPE));

			//Reboot functionality executed in EDD task context
			LSAS_RQB_SET_OPCODE  ( pReBootEvent, TSKMA_OPC_EDD_REQ_REBOOT );
			LSAS_RQB_EDD_REQUEST ( pReBootEvent, tskma_task_edd_request   );

			//Reboot in EDD task - nothing more to do here
			while (1) {}

		}
#define REBOOT_CPU    1
#define REBOOT_SYSTEM 2
#define REBOOT_MODE   REBOOT_CPU
extern void R_BSP_CPUResetAutoRelease(bsp_reset_t);
		void OsRebootService (void)
		{
			OsEnterCritical();
			R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_RESET);
#if (REBOOT_MODE == REBOOT_SYSTEM)
			R_BSP_SystemReset();
#else
			R_BSP_ModuleResetEnable(BSP_MODULE_RESET_ESC_ETH_SUBSYSTEM);
			R_BSP_ModuleResetEnable(BSP_MODULE_RESET_ETHSW);
			R_BSP_ModuleResetEnable(BSP_MODULE_RESET_GMAC0_PCLKM);
			R_BSP_ModuleResetEnable(BSP_MODULE_RESET_GMAC0_PCLKH);
			R_BSP_CPUResetAutoRelease(BSP_CFG_CORE_CR52);
#endif
			while(1){volatile int brk=0;}
		}

    // *--------------------------------------------*
    // *   Reset_Handler (void)
    // *   Called after software reset
    // *--------------------------------------------*
	extern void system_init(void);
	BSP_TARGET_ARM
	BSP_ATTRIBUTE_STACKLESS void Reset_Handler (void)
	{
#ifdef _RENESAS_RZT_
		if(1 == R_SYSC_NS->RSTSR0_b.SWR0F || (0 == R_SYSC_NS->RSTSR0_b.SWR0F && 1 == R_SYSC_NS->RSTSR0_b.SWR1F))
#endif
#ifdef _RENESAS_RZN_
		if(1 == R_SYSC_NS->RSTSR0_b.SWR0F)
#endif
		{
		    __asm volatile (
#if (0 == BSP_CFG_CPU)
				"mrc      p15, 0, r1, c9, c1, 1      \n"	/* read IMP_BTCMREGIONR */
				"ldr      r2, =0x00001FFC            \n"
				"and      r1, r1, r2                 \n"
				"ldr      r2, =0x00100003            \n"	/* TCM base address, Enable TCM */
				"orr      r1, r1, r2                 \n"
				"mcr      p15, 0, r1, c9, c1, 1      \n"	/* weite IMP_BTCMREGIONR */
#endif
				"ldr      r0, =0x00000003            \n"
				"mcr      p15, 0, r0, c11, c0, 0     \n"	/* write IMP_SLAVEPCTLR */
				"ldr      r1 ,=system_init           \n"
				"BLX      r1                         \n"
				::: "memory");
		}
	}

    // *--------------------------------------------*
    // *   OsPrintTaskProp (void)
    // *   print task properties
    // *   (optional, for debugging only)
    // *--------------------------------------------*
      void OsPrintTaskProp (void)
      {
        int TskId;
        for (TskId= 0; TskId < MAXNUM_OF_TASKS; TskId++)
        {
            if (TaskProp[TskId].Occupied)
            {
               // printf ("TaskID=%-3d, ThreadID=%-3d Name=%s\n",
               //         TskId, cyg_thread_get_id(TaskProp[TskId].ThreadId), TaskProp[TskId].TaskName);
            }
        }
      }


  	// *----------------------------------------------------------------*
  	// *
  	// *  OsStoreTraceBuffer
  	// *----------------------------------------------------------------*
  	// *  optional: store trace informations from ram into a flash memory,
  	// *  on demand, to save them over the next power off/on or reset.
  	// *
  	// *  Input:       BufferName
  	// *               BufferStart
  	// *               BufferSize
  	// *
  	// *  Output:      return               PNIO_OK, PNIO_NOT_OK
  	// *
  	// *----------------------------------------------------------------*
	PNIO_UINT32 OsStoreTraceBuffer( const char * pBlock1Name, void* pBlock1Start, PNIO_UINT32 Block1Size,
	                                const char * pBlock2Name, void* pBlock2Start, PNIO_UINT32 Block2Size)
  	{

        PNIO_printf ("OsStoreTraceBuffer for DKSW on ecos not yet implemented here, discard data\n");
  		return (PNIO_OK);
  	}


#endif


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
