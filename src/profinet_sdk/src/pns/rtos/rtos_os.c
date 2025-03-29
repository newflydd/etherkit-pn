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

	PNIO_UINT32 TskId_OsTimer = 0U;
    PNIO_UINT32 OsInitialized = 0U;
    PNIO_UINT32 PnioLogDest = 1;   // debug logging   0:none, 1:Console, 2:Tracebuffer

    // *---------------------------------*
    // * semaphore list
    // *---------------------------------*
    #if (MAXNUM_OF_BIN_SEMAPH != 0)	// optional
        OS_BSS_FAST SEM_ALLOC_ENTRY	Semaph [MAXNUM_OF_BIN_SEMAPH];		// free mutexes (may be used by dynamically allocate/free)
    #endif

//#pragma location="nocache"
   // static
	// TASK_PROPERTIES  TaskProp[MAXNUM_OF_TASKS];

    // *---------------------------------*
    // * mutexes
    // *---------------------------------*
//#pragma location="nocache"
    OS_BSS_FAST OS_MUX_TYPE NamedMutex[MAXNUM_OF_NAMED_MUTEXES];

    // *---------------------------------*
    // * event flags
    // *---------------------------------*
    #if (MAXNUM_OF_EVFLAGS != 0)	// optional
	    typedef struct
	    {
		    sem_t				EvHndl;		// event handle from the operating system
		    PNIO_UINT32			Allocated;	// allocate/free tag
	    } EVFLAG_ALLOC_ENTRY;

        static EVFLAG_ALLOC_ENTRY	EvFlag[MAXNUM_OF_EVFLAGS];		// free mutexes (may be used by dynamically allocate/free)
    #endif

    // *---------------------------------*
    // * timer
    // *---------------------------------*
//#pragma location="nocache"
    OS_BSS_FAST OS_TIMER         OsTimer[PCPNIO_TIMER_MAX];
	#if DEBUG_TIMCNT
		static PNIO_UINT32		OsTimCounter[PCPNIO_TIMER_MAX];
	#endif

	/* ------------------------------------------------------ */
	/* STACK ---------- have to be placed in DTCM ----------- */
	/* Valid only for EB200P and EB200P-2 based HW ---------- */
	/* ------------------------------------------------------ */
#if __ICCARM__
#pragma data_alignment=4
	OS_BSS_STACK_FAST  StackType_t TaskStack[MAXNUM_OF_TASKS][TASK_STACK_ITEMS] __attribute__((used, section("task_stack")));
#else
    OS_BSS_STACK_FAST  StackType_t TaskStack[MAXNUM_OF_TASKS][TASK_STACK_ITEMS] BSP_PLACE_IN_SECTION(BSP_UNINIT_SECTION_PREFIX "task_stack") BSP_ALIGN_VARIABLE(4);
#endif

    PNIO_INT32  TaskOsTimer 	 (void);
	#if LOG_MSG_SND_REC
		static PNIO_UINT32 MsqQueueMaxFillStatAll = 0;
	#endif

	#if 0
		static PNIO_UINT32 EnterShortSemId;
	#endif


  	// *----------------------------------------------------------------*
  	// *
  	// *  OsHtonl, OsNtohl  ()
  	// *  OsHtons, OsNtohs  ()
  	// *----------------------------------------------------------------*
  	// *  host to network conversion and vice versa
  	// *
  	// *----------------------------------------------------------------*

    PNIO_UINT16 OsHtons (PNIO_UINT16 val16)
    {
        #if PNIO_BIG_ENDIAN
            return (val16);
        #else
            return (((val16 & 0xFF)<<8 ) | ((val16 & 0xFF00)>>8));
        #endif
    }

    PNIO_UINT32 OsHtonl (PNIO_UINT32 val32)
    {
        #if PNIO_BIG_ENDIAN
            return (val32);
        #else
            return (((val32 & 0xFF)<<24 ) | ((val32 & 0xFF00)<<8) | ((val32 & 0xFF0000)>>8) | ((val32 & 0xFF000000)>>24));
        #endif
    }

    PNIO_UINT16 OsNtohs (PNIO_UINT16 val16)
    {
        #if PNIO_BIG_ENDIAN
            return (val16);
        #else
            return (((val16 & 0xFF)<<8 ) | ((val16 & 0xFF00)>>8));
        #endif
    }

    PNIO_UINT32 OsNtohl (PNIO_UINT32 val32)
    {
        #if PNIO_BIG_ENDIAN
            return (val32);
        #else
            return (((val32 & 0xFF)<<24 ) | ((val32 & 0xFF00)<<8) | ((val32 & 0xFF0000)>>8) | ((val32 & 0xFF000000)>>24));
        #endif
    }


   	// *----------------------------------------------------------------*
  	// *  Map standard functions to OS-Interface
  	// *
  	// *  OsMemCpy   ()   copy memory block
  	// *  OsMemMove  ()   move memory block
  	// *  OsMemSet   ()   fill memory block with a defined value
  	// *  OsdMemCmp  ()   compare Memory block with character
  	// *  OsStrChr   ()   find character in a string
  	// *  OsStrCmp   ()   compare string
  	// *  OsStrnCmp  ()   compare part of a string
  	// *  OsStrCpy   ()   copy string
  	// *  OsStrnCpy  ()   copy string with maxlen check
  	// *  OsStrLen   ()   evaluate string length
  	// *  OsAtoi     ()   convert ascii to integer value
  	// *  OsRand     ()   get a random number
  	// *  OsSrand    ()   initialize random number generator
  	// *----------------------------------------------------------------*

    PNIO_VOID * OsMemCpy(PNIO_VOID * pDst, const PNIO_VOID * pSrc, PNIO_UINT32 size)
    {
        if (size == 0)
            return pDst;
return (memcpy(pDst, pSrc, size));
#if 0
        if (((PNIO_UINT8*)pDst >= ((PNIO_UINT8*)pSrc + size)) || (((PNIO_UINT8*)pDst + size) <= (PNIO_UINT8*)pSrc))
        {
            ;
        }
        else
        {
            unsigned int i;
            if (pSrc < pDst)
            {
                for (i = size; i != 0; i--)
                {
                    *(((PNIO_UINT8*)pDst) + (i-1)) = *(((PNIO_UINT8*)pSrc) + (i-1));
                }
                return pDst;
            }
            else
            {
                for (i = 0; i < size; i++)
                {
                    *(((PNIO_UINT8*)pDst) + i) = *(((PNIO_UINT8*)pSrc) + i);
                }
                return pDst;
            }
        }
        return (memcpy(pDst, pSrc, size));
#endif
    }
    PNIO_VOID * OsMemMove(PNIO_VOID * pDst, const PNIO_VOID * pSrc, PNIO_UINT32 size)
    {
        return memmove(pDst,pSrc,size);
    }
    PNIO_VOID * OsMemSet(PNIO_VOID * pDst,PNIO_INT val,PNIO_UINT32 size)
    {
        return memset(pDst,val,size);
    }
    PNIO_INT    OsMemCmp(const PNIO_VOID * pBuf1,const PNIO_VOID * pBuf2, PNIO_UINT32 size)
    {
        return memcmp(pBuf1,pBuf2,size);
    }
    PNIO_INT    OsRand(void)
    {
        return rand();
    }
    PNIO_VOID   OsSrand(PNIO_UINT val)
    {
        srand(val);
    }
    PNIO_INT8 * OsStrChr(const PNIO_INT8 *pBuf, PNIO_INT c)
    {
        return strchr(pBuf,c);
    }
    PNIO_INT    OsStrCmp(const PNIO_INT8 * pBuf1, const PNIO_INT8 *pBuf2)
    {
        return strcmp(pBuf1,pBuf2);
    }
    PNIO_INT    OsStrnCmp(const PNIO_INT8 *pBuf1, const PNIO_INT8 *pBuf2, PNIO_UINT32 size)
    {
        return strncmp(pBuf1,pBuf2,size);
    }
    PNIO_INT8 * OsStrCpy(PNIO_INT8 *pDst,const PNIO_INT8 *pSrc)
    {
        return strcpy(pDst,pSrc);
    }
    PNIO_INT8 * OsStrnCpy(PNIO_INT8 *pDst,const PNIO_INT8 *pSrc, PNIO_UINT32 size)
    {
        return strncpy(pDst,pSrc,size);
    }
    PNIO_UINT32 OsStrLen(const PNIO_INT8 *pBuf)
    {
        return strlen(pBuf);
    }
    PNIO_INT    OsAtoi(const PNIO_INT8 *pStr)
    {
        return atoi(pStr);
    }

    void *calloc(size_t NumOfElem, size_t Length)
    {
    	return pvPortMalloc(NumOfElem * Length);
    }

    void *malloc( size_t Length)
    {
    	return pvPortMalloc(Length);
    }

    void free(void *pMem)
    {
    	if(NULL != pMem)
    	{
    		vPortFree(pMem);
    	}
    }

	// *-----------------------------------------------------------*
	// *  OsAllocX ()
	// *
	// *  Alloc a memory block from a specified memory pool.
	// *  The block is initialized with a predefined value.
	// *
	// *-----------------------------------------------------------*
    extern void *pvPortMalloc( size_t xSize );

    PNIO_UINT32 OsAllocX (void**  	ppMem,
                        PNIO_UINT8	Value,
                        PNIO_UINT32	Length,
                        PNIO_UINT32  PoolId)
    {
        OS_ASSERT (OsInitialized == PNIO_TRUE);

        if (Length == 0)
        {
            LSA_TRACE_03  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "Alloc: pMem = 0x%x, Length = %d, Pool = %d\n",
                           *ppMem,
                           Length,
                           PoolId);
            *ppMem = 0;
            return (PNIO_NOT_OK);
        }
        *ppMem = pvPortMalloc(Length); //malloc(Length);

        if(LSA_NULL == *ppMem)
        {
            return (LSA_RET_ERR_RESOURCE);
        }
        OsMemSet (*ppMem, (PNIO_UINT32) Value, Length);

       return (PNIO_OK);
    }


	// *-----------------------------------------------------------*
	// *  OsFreeX ()
	// *
	// *-----------------------------------------------------------*
	// *  deallocs an occupied memory block from a specified
	// *  memory pool (threadX provides the the pool selection for an
	// *  tx_byte_release (), so no case-switch is necessary.
	// *-----------------------------------------------------------*
    extern void vPortFree( void *pv );
    PNIO_UINT32 OsFreeX (void* pMem, PNIO_UINT32 PoolId)
    {
        LSA_UNUSED_ARG (PoolId);
        OS_ASSERT (OsInitialized == PNIO_TRUE);
        // ******* free memory ******
        if (pMem)
        {
        	vPortFree(pMem); // free(pMem);
        }

        return (LSA_OK);
    }

   	// *-----------------------------------------------------------*
	// *  OsAllocF ()
	// *
	// *-----------------------------------------------------------*
	// *  alloc a memory block from default pool.
    // *  The block is not initialized.
	// *-----------------------------------------------------------*
    PNIO_UINT32 OsAllocF (void**  		ppMem,
                          PNIO_UINT32	Length)
    {
       OS_ASSERT (OsInitialized == PNIO_TRUE);

        if (Length == 0)
         {
              LSA_TRACE_02  (TRACE_SUBSYS_IOD_SYSADAPT,
                             LSA_TRACE_LEVEL_ERROR,
                             "Alloc: pMem = 0x%x, Length = %d\n",
                             *ppMem,
                             Length);
             *ppMem = 0;
             return (PNIO_NOT_OK);
         }

        *ppMem = pvPortMalloc(Length); //malloc(Length);
        if(LSA_NULL == *ppMem){
            return PNIO_NOT_OK;
        }
        return LSA_OK;
    }

	// *-----------------------------------------------------------*
	// *  OsAllocFX ()
	// *
	// *-----------------------------------------------------------*
	// *  alloc a memory block from a specified pool.
    // *  The block is not initialized.
	// *-----------------------------------------------------------*
    PNIO_UINT32 OsAllocFX (void**  		ppMem,
                  PNIO_UINT32	Length,
                  PNIO_UINT32  PoolId)
    {
        return (OsAllocF(ppMem,Length));
    }

    // *-----------------------------------------------------------*
	// *  OsAlloc  ()
	// *
	// *-----------------------------------------------------------*
	// *  alloc a memory block from default pool and initialize
    // *  it with a predefined value.
	// *-----------------------------------------------------------*
    PNIO_UINT32 OsAlloc (void**		pMem,
                        PNIO_UINT8	Value,
                        PNIO_UINT32	Length)
    {
        return OsAllocX(pMem,Value,Length,MEMPOOL_DEFAULT);
    }

	// *-----------------------------------------------------------*
	// *  OsFree ()
	// *
	// *-----------------------------------------------------------*
	// *  free a memory block from default pool
	// *-----------------------------------------------------------*
    PNIO_UINT32 OsFree (void* pMem)
    {
    	if(LSA_NULL != pMem)
    	{
    		return OsFreeX(pMem,MEMPOOL_DEFAULT);
    	}
    	else
    	{
    		return PNIO_NOT_OK;
    	}
    }


    // *-----------------------------------------------------------*
	// *  OsWait_ms  ()
	// *
	// *-----------------------------------------------------------*
	// *  wait a predefined time in msec.
	// *-----------------------------------------------------------*
    void OsWait_ms (PNIO_UINT32 PauseTimeInterval_ms)
    {
    	if( (0U < PauseTimeInterval_ms) && (0xFFFFFFFFU != PauseTimeInterval_ms) )
    	{
    		vTaskDelay( pdMS_TO_TICKS( PauseTimeInterval_ms ) );
    	}
    	else
    	{
    		vTaskDelay( PauseTimeInterval_ms );
    	}
    }

	// *-----------------------------------------------------------*
	// *  OsEnter ()
	// *  occupy a default mutex
	// *-----------------------------------------------------------*
	// *  wait a predefined time in msec.
	// *-----------------------------------------------------------*
	void OsEnter		(void)
	{
	    OsEnterX(OS_MUTEX_DEFAULT);
	}

	// *-----------------------------------------------------------*
	// *  OsFree ()
	// *  free a default mutex
	// *-----------------------------------------------------------*
	// *  wait a predefined time in msec.
	void OsExit			(void)
	{
	    OsExitX(OS_MUTEX_DEFAULT);
	}

	void OsEnterCritical (void)
	{
		portENTER_CRITICAL();
	}

	void OsExitCritical  (void)
	{
		portEXIT_CRITICAL();
	}


	// *-----------------------------------------------------------*
	// *  OsIntDisable ()
	// *  disable interrupts
	// *-----------------------------------------------------------*
	// *  wait a predefined time in msec.
	int g_OsIntMask = 0;
	void OsIntDisable (void)
	{
		g_OsIntMask = ulPortSetInterruptMask();
	}


	// *-----------------------------------------------------------*
	// *  OsIntEnaable ()
	// *  enaable interrupts
	// *-----------------------------------------------------------*
	void OsIntEnable  (void)
	{
		vPortClearInterruptMask(0);
	}

    // *-----------------------------------------------------------*
    // *  OsGetTime_us ()
    // *
    // *  get the actual time, value in microseconds
    // *
    // *
    // *-----------------------------------------------------------*
    PNIO_UINT32 OsGetTime_us (void)
    {
        OS_ASSERT (OsInitialized == PNIO_TRUE);

#if 1
        return ( xTaskGetTickCount() );

#else
        #ifdef CYGSEM_KERNEL_SCHED_TIMEPART
                return (TICKS_TO_MS(cyg_current_time())*1000);
        #else
                return (cyg_current_time()*(CYGNUM_HAL_RTC_NUMERATOR/CYGNUM_HAL_RTC_DENOMINATOR/1000));
        #endif
#endif
    }


    // *-----------------------------------------------------------*
    // *  OsGetTimDiffSinceCycleStart ()
    // *
    // *  get the actual time, value in microseconds from hardware
    // *
    // *
    // *-----------------------------------------------------------*
    PNIO_UINT32 OsGetTimDiffSinceCycleStart(void)
    {
    	PNIO_UINT32 ret_val = 0;
    	#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)
    		ret_val = EVMA_GET_CYCLE_TIME();
		#endif
        return (ret_val);
    }

    // *-----------------------------------------------------------*
    // *  OsGetTime_ms ()
    // *
    // *  get the actual time, value in milliseconds
    // *
    // *
    // *-----------------------------------------------------------*
    PNIO_UINT32 OsGetTime_ms (void)
    {
    	return ( xTaskGetTickCount());
    }

    // *-----------------------------------------------------------*
    // *  OsGetUnixTime ()
    // *
    // *  get the actual time in UNIX format (seconds since 01.01.1970, 00:00:00
    // *
    // *
    // *-----------------------------------------------------------*
    PNIO_UINT32 OsGetUnixTime (void)
    {
     //  OS_ASSERT (OsInitialized == PNIO_TRUE);
       return ( xTaskGetTickCount() / 1000);
    }

    PNIO_UINT64 OsGetUuidTime ( void )
    {
        PNIO_UINT64 time;

        // RFC constant
        static const PNIO_UINT64 num100nsec1582    = 0x01b21dd213814000;

        time = xTaskGetTickCount() + num100nsec1582;

        // return
        return time;
    }

	PNIO_UINT32 OsQueSendFromISR(PNIO_VOID *pQue, PNIO_UINT32 Item)
	{
		PNIO_UINT32 status = PNIO_NOT_OK;
		if(LSA_NULL != pQue)
		{
			PNIO_UINT32 u32Msg = Item;
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			if( pdPASS == xQueueSendToFrontFromISR(pQue, &u32Msg, &xHigherPriorityTaskWoken))
			{
				status = PNIO_OK;
			}
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}

		return status;
	}

    PNIO_UINT32 OsQueSend(PNIO_VOID *pQue, PNIO_UINT32 Item, PNIO_UINT32 Timeout)
    {
    	PNIO_UINT32 status = PNIO_NOT_OK;
    	if(LSA_NULL != pQue)
    	{
    		PNIO_UINT32 u32Msg = Item;
    		if( (0U < Timeout) && (OS_WAIT_FOR_OBJ != Timeout) )
			{
				Timeout = pdMS_TO_TICKS( Timeout );
			}
    		if( pdPASS == xQueueSendToBack(pQue, &u32Msg, Timeout))
    		{
    			status = PNIO_OK;
    		}
    	}

    	return status;
    }

    PNIO_UINT32 OsQueSendToFront(PNIO_VOID *pQue, PNIO_UINT32 Item, PNIO_UINT32 Timeout)
    {
    	PNIO_UINT32 status = PNIO_NOT_OK;
    	if(LSA_NULL != pQue)
    	{
    		PNIO_UINT32 u32Msg = Item;
    		if( (0U < Timeout) && (OS_WAIT_FOR_OBJ != Timeout) )
			{
				Timeout = pdMS_TO_TICKS( Timeout );
			}
    		if( pdPASS == xQueueSendToFront(pQue, &u32Msg, Timeout))
    		{
    			status = PNIO_OK;
    		}
    	}

    	return status;
    }

    PNIO_UINT32 OsQueRecv(PNIO_VOID *pQue, PNIO_UINT32 Timeout)
    {
    	PNIO_UINT32 u32Msg = 0;

		if(LSA_NULL != pQue)
		{
			if( (0U < Timeout) && (OS_WAIT_FOR_OBJ != Timeout) )
			{
				Timeout = pdMS_TO_TICKS( Timeout );
			}

			xQueueReceive(pQue, &u32Msg, Timeout);
		}

		return u32Msg;
    }

    // *-----------------------------------------------------------*
    // *  OsReadMessageBlocked ()
    // *
    // *  reads a message from the message queue of the calling thread
    // *  the function will be blocked if no message is available.
    // *-----------------------------------------------------------*
    PNIO_UINT32 OsReadMessageBlocked (void** ppMessage, PNIO_UINT32 TaskId)
    {
        OS_ASSERT (OsInitialized == PNIO_TRUE);
      //  OS_ASSERT (ppMessage);

        if ( (LSA_NULL != ppMessage) &&
        	 (TaskId < MAXNUM_OF_TASKS) &&
			 (LSA_NULL != TaskProp[TaskId].MsgQueue) )
        {
        	PNIO_UINT32 u32Msg = 0;
			if(pdPASS == xQueueReceive(TaskProp[TaskId].MsgQueue, &u32Msg, portMAX_DELAY))// pdMS_TO_TICKS(0xFFFFFFFF)))
			{
				*((PNIO_UINT32*)ppMessage) = u32Msg;
				return PNIO_OK;
			}
			else
			{
				*((PNIO_UINT32*)ppMessage) = u32Msg;
				 return (PNIO_NOT_OK);
			}
        }
        else
        {
          OsWait_ms(10);
        	PNIO_printf (("Error in OsReadMessageBlocked: invalid Task Id()"));
        }

        #if LOG_MSG_SND_REC
			TaskProp[TaskId].MsgQueueFillStat = cyg_mbox_peek (TaskProp[TaskId].MsgQueue);
 	        TaskProp[TaskId].MsgQueueNumOfRec++;	// total number of received messages--
		#endif

		return ( PNIO_NOT_OK );
    }

    // *-----------------------------------------------------------*
    // *  OsReadMessageBlockedX ()
    // *
    // *  reads a message from the message queue of the calling thread,
    // *  including 2 message pointers.
    // *  the function will be blocked if no message is available.
    // *-----------------------------------------------------------*
    PNIO_UINT32 OsReadMessageBlockedX (void** ppMessage1, void** ppMessage2, PNIO_UINT32 TaskId)
    {
        PNIO_UINT32 Status;
        PNIO_UINT32* pMsg;

        Status = OsReadMessageBlocked ((void** )&pMsg, TaskId);
        //OS_ASSERT (Status == PNIO_OK);
        if(PNIO_OK == Status)
        {
			if (pMsg)
			{
				if (*(pMsg+2) == OS_MSGX_ID)
				{
					*ppMessage1 = (void*) *(pMsg+0);
					*ppMessage2 = (void*) *(pMsg+1);
				}
				else
				{
					Status = PNIO_NOT_OK;
					LSA_TRACE_02  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "not OS_MSGX_ID (0x%x) TskId%d \n", *(pMsg+2), TaskId);
				}

				OsFree (pMsg);
			}
			else
			{
				Status = PNIO_NOT_OK;
				LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "NullPtr received TskId%d\n", TaskId);
			}
        }
        else
        {
        	Status = PNIO_NOT_OK;
        	LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "OsReadMessageBlocked TskId%d\n", TaskId);
        }
        return (Status);
    }

    // *-----------------------------------------------------------*
    // *  OsSendMessageX
    // *
    // *  sends a expanded message to the message queue of the specified
    // *  task. It includes 2 pointers in one message instead
    // *  of one pointer. So we can send a function pointer and a
    // *  requestblock pointer in one one message, to execute a
    // *  function call in a remote task.
    // *-----------------------------------------------------------*
    PNIO_UINT32 OsSendMessageX (PNIO_UINT32 	TaskId,
						     void* 			pMessage1,
						     void* 			pMessage2,
						     PNIO_UINT32 	MsgPrio)
    {
        PNIO_UINT32 *pMsg = LSA_NULL;
        PNIO_UINT32 Status = PNIO_OK;

        OsAlloc ((void**)&pMsg, 0, 3 * sizeof(PNIO_UINT32)); // size for pMessage1, pMessage2, Tag-Id

        if(LSA_NULL != pMsg)
        {
			*(pMsg+0) = (PNIO_UINT32)pMessage1;
			*(pMsg+1) = (PNIO_UINT32)pMessage2;
			*(pMsg+2) = (PNIO_UINT32)OS_MSGX_ID;

			Status = OsSendMessage (TaskId, pMsg, MsgPrio);
        }

        if ( (Status == PNIO_NOT_OK) && (LSA_NULL != pMsg))
        {
            OsFree (pMsg);
        }
        return (Status);
    }


    // *-----------------------------------------------------------*
    // *  OsSendMessage
    // *
    // *  sends a message to the message queue of the specified
    // *  task
    // *
    // *
    // *-----------------------------------------------------------*

    OS_MSG_PTR_TYPE OsCreateMsgExt(PNIO_UINT16 DataSize)
    {
		OS_MSG_PTR_TYPE ptMsg = LSA_NULL;

		OsAlloc ((void**)&ptMsg, 0, sizeof(OS_MSG_TYPE) );

		if(LSA_NULL != ptMsg)
		{
			if(0U < DataSize)
			OsAlloc ((void**)&ptMsg->pvData, 0, DataSize);
			if(LSA_NULL != ptMsg->pvData)
			{
				ptMsg->DataSize = DataSize;
			}
			else
			{
				OsFree(ptMsg);
				ptMsg = LSA_NULL;
			}
		}
    	return ptMsg;
    }

#define MODE_BITS 0x1F
#define FIQ_MODE  0x11
#define IRQ_MODE  0x12
#define SVC_MODE  0x13
    inline static PNIO_BOOL isInterrupt(void)
    {
#ifdef _ICCARM_
            const PNIO_UINT32 mode = __get_CPSR() & MODE_BITS;
#else
      PNIO_UINT32 mode;
      __asm volatile("mrs %0, CPSR" : "=r"(mode));
      mode &= MODE_BITS;
#endif
            return (mode == FIQ_MODE) || (mode == IRQ_MODE) || (mode == SVC_MODE);
    }
    PNIO_UINT32 OsSendMessage (PNIO_UINT32  TaskId,
						     void*		 pMessage,
						     PNIO_UINT32  MsgPrio)
    {
		BaseType_t    status;
		PNIO_UINT32   MboxFillStat;
		volatile PNIO_BOOL     nonTaskContext;

		nonTaskContext = isInterrupt();
		// ***** return, if mbox is more than 90% full (tryput may not work correctly) ****
		if(nonTaskContext)
		{
			MboxFillStat = uxQueueMessagesWaitingFromISR(TaskProp[TaskId].MsgQueue);
		}
		else
		{
    		MboxFillStat = uxQueueMessagesWaiting(TaskProp[TaskId].MsgQueue);
    	}
		if (MboxFillStat > ((MAXNUM_MSG_ENTRIES * 9)/10))
		{ // no trace entry here --> otherwise recursive call of OsSendMessage
			return (PNIO_NOT_OK);
		}
		if(nonTaskContext)
		{
			status = xQueueSendFromISR(TaskProp[TaskId].MsgQueue, &pMessage, 0);
		}
		else
		{
			status = xQueueSend(TaskProp[TaskId].MsgQueue, &pMessage, 0);
		}

		if(pdPASS != status)
		{
			OsEnterShort();
			TaskProp[TaskId].MsgQueueErrCnt++;
			OsExitShort ();
			LSA_TRACE_02  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "xQueueSend(%s,%p) failed\n",TaskProp[TaskId].TaskName,pMessage);
			return (PNIO_NOT_OK);
    	}


        // **** if ok, refresh queuestate ***
		#if LOG_MSG_SND_REC
			//M_INTERR_DISABLE;
			TaskProp[TaskId].MsgQueueFillStat = MboxFillStat + 1;

			TaskProp[TaskId].MsgQueueNumOfSnd++;	// total number of sent messages++
			if (TaskProp[TaskId].MsgQueueFillStat >= TaskProp[TaskId].MsgQueueMaxFillStat) // save max fillstate value
				TaskProp[TaskId].MsgQueueMaxFillStat = TaskProp[TaskId].MsgQueueFillStat;
			if (TaskProp[TaskId].MsgQueueFillStat >= MsqQueueMaxFillStatAll) // save max fillstate value
				MsqQueueMaxFillStatAll = TaskProp[TaskId].MsgQueueFillStat;
			//M_INTERR_ENABLE;
		#endif

        return (PNIO_OK); //ok
    }
    PNIO_UINT32 OsSendMessageToFront (PNIO_UINT32  TaskId,
						     void*		 pMessage,
						     PNIO_UINT32  MsgPrio)
    {
    	PNIO_UINT32    status = PNIO_NOT_OK;

    	if ( (TaskId < MAXNUM_OF_TASKS) && (LSA_NULL != TaskProp[TaskId].MsgQueue) )  //&& (LSA_NULL != pMessage) )
    	{
    		//PNIO_UINT32   MboxFillStat = uxQueueSpacesAvailable(TaskProp[TaskId].MsgQueue);
    		PNIO_UINT32 u32Msg = (PNIO_UINT32)pMessage;
    		if( pdPASS == xQueueSendToFront(TaskProp[TaskId].MsgQueue, &u32Msg, 0))
    		{
    			status = PNIO_OK;
    		}
    	}
    	return status;
    }

    PNIO_UINT32 OsSendMsgExt(PNIO_UINT32 ThreadId, PNIO_UINT16 MsgId, PNIO_VOID *pvData, PNIO_UINT16 DataSize)
    {
    	PNIO_UINT32    status = PNIO_NOT_OK;

    	if ( (ThreadId < MAXNUM_OF_TASKS) && (LSA_NULL != TaskProp[ThreadId].MsgQueue) )
    	{
    		OS_MSG_PTR_TYPE ptMsg = OsCreateMsgExt(DataSize);

			if(LSA_NULL != ptMsg)
			{
				PNIO_UINT32 u32Msg = (PNIO_UINT32)ptMsg;
				ptMsg->MsgId = MsgId;
				ptMsg->DataSize = DataSize;
				if(0 < DataSize)
				{
					OsMemCpy(ptMsg->pvData, pvData, DataSize);
				}

				if( pdPASS == xQueueSendToBack(TaskProp[ThreadId].MsgQueue, &u32Msg, 0))
				{
					status = PNIO_OK;
				}
			}
    	}

    	return status;
    }

    PNIO_VOID OsFreeMsgExt(PNIO_VOID *pMem)
    {
    	OS_MSG_PTR_TYPE ptMsg = (OS_MSG_PTR_TYPE)pMem;
    	if( (LSA_NULL != ptMsg) && (LSA_FALSE == ptMsg->bMsgFromIsr))
    	{
    		if( (LSA_NULL != ptMsg->pvData) && (0U < ptMsg->DataSize))
    		{
    			OsFree(ptMsg->pvData);
    		}
    		ptMsg->pvData = LSA_NULL;
    	}
    }

    PNIO_UINT32 OsSendMsgFromISR(PNIO_UINT32 ThreadId, PNIO_VOID *pvData)
	{
		PNIO_UINT32    status = PNIO_NOT_OK;

		if ( (LSA_NULL != pvData) && (ThreadId < MAXNUM_OF_TASKS) && (LSA_NULL != TaskProp[ThreadId].MsgQueue) )
		{
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			PNIO_UINT32 u32Msg = (PNIO_UINT32)pvData;

			if( pdPASS == xQueueSendToFrontFromISR(TaskProp[ThreadId].MsgQueue, &u32Msg, &xHigherPriorityTaskWoken))
			{
				status = PNIO_OK;
			}
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}

		return status;
	}

    PNIO_VOID*  OsReadMsgExt(PNIO_UINT32 ThreadId, PNIO_UINT32 Timeout)
    {
    	PNIO_VOID *pvMsg = NULL;
		if ( (ThreadId < MAXNUM_OF_TASKS) && (LSA_NULL != TaskProp[ThreadId].MsgQueue) )
		{
			PNIO_UINT32 u32Msg = 0;

			if( (0U < Timeout) && (OS_WAIT_FOR_OBJ != Timeout) )
			{
				Timeout = pdMS_TO_TICKS( Timeout );
			}

			if(pdPASS == xQueueReceive(TaskProp[ThreadId].MsgQueue, &u32Msg, Timeout) )
			{
				pvMsg= (PNIO_VOID*)u32Msg;
			}
		}

		return pvMsg;
    }

    PNIO_UINT32 OsSendMsgPrm(PNIO_UINT32 ThreadId, PNIO_UINT16 MsgId, PNIO_UINT32 p1, PNIO_UINT32 p2, PNIO_UINT32 p3, PNIO_UINT32 p4, PNIO_UINT32 p5)
    {
    	PNIO_UINT32    status = PNIO_NOT_OK;
#if 1
    	PNIO_UINT32 PrmList[] = {p1, p2, p3, p4, p5};
    	status = OsSendMsgExt(ThreadId, MsgId, (PNIO_VOID*)&PrmList[0], sizeof(PrmList) );
#else
    	if ( (ThreadId < MAXNUM_OF_TASKS) && (LSA_NULL != TaskProp[ThreadId].MsgQueue) )
    	{
    		OS_MSG_PTR_TYPE ptMsg = OsCreateMsgExt( OS_MSG_MAX_PRM * sizeof(PNIO_UINT32) );

			if(LSA_NULL != ptMsg)
			{
				PNIO_UINT32 pPrmList = ptMsg->pvData;
				ptMsg->MsgId = MsgId;
				ptMsg->DataSize = ( OS_MSG_MAX_PRM * sizeof(PNIO_UINT32) );
				pPrmList[0U] = p1;
				pPrmList[1U] = p2;
				pPrmList[2U] = p3;
				pPrmList[3U] = p4;
				pPrmList[4U] = p5;
			}
    	}
#endif
    	return status;
    }


    #if (MAXNUM_OF_BIN_SEMAPH != 0)
	    // *-----------------------------------------------------------*
	    // *  OsTakeSemB()
	    // *  Occupies a binary semaphore. If semaphore is already
        // *  occupied, the function is blocking.
 	    // *-----------------------------------------------------------*
        PNIO_UINT32 OsTakeSemB (PNIO_UINT32 SemId)
        {
            OS_ASSERT (OsInitialized == PNIO_TRUE);

            if ( (0U < SemId) &&
            	 (SemId < MAXNUM_OF_BIN_SEMAPH) &&
            	 (LSA_NULL != Semaph[SemId].Mutex) &&
            	 (pdPASS == xSemaphoreTake(Semaph[SemId].Mutex, portMAX_DELAY)) )
            {
            	return (PNIO_OK);
            }

            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "error bin semaphore %d\n", SemId);
            return (PNIO_NOT_OK);
        }

	    // *-----------------------------------------------------------*
	    // *  OsGiveSemB()
	    // *  Deoccupies a binary semaphore, that has been occupied before
        // *  with function OsTakeSemaph
        // *  This function may be called from task- or interrupt context.
	    // *-----------------------------------------------------------*
        PNIO_UINT32 OsGiveSemB (PNIO_UINT32 SemId)
        {
            OS_ASSERT (OsInitialized == PNIO_TRUE);

            if ( (0U < SemId) &&
				 (SemId < MAXNUM_OF_BIN_SEMAPH) &&
				 (LSA_NULL != Semaph[SemId].Mutex) &&
            	 (pdPASS == xSemaphoreGive(Semaph[SemId].Mutex)) )
            {
            	return (PNIO_OK);
            }

            return (PNIO_NOT_OK);
        }
	    #endif

	// *-----------------------------------------------------------*
	// *  OsEnterShort()
	// *  occpupy critical section, but short and fast.
    // *  Can be implemented e.g. by scheduler lock.
 	// *-----------------------------------------------------------*
	void OsEnterShort(void)
	{
        #if 0
            OsTakeSemB (EnterShortSemId);
        #else
        //    vTaskSuspendAll();
            taskENTER_CRITICAL();
        #endif
	}

	// *-----------------------------------------------------------*
	// *  OsFreeShort()
	// *  free short critical
 	// *-----------------------------------------------------------*
	void OsExitShort(void)
	{
        #if 0
            OsGiveSemB (EnterShortSemId);
        #else
            //xTaskResumeAll();
            taskEXIT_CRITICAL();
        #endif
	}


    // *-----------------------------------------------------------*
    // *  OsEnterX ()
    // *
    // *  MutexId = 0....(MAXNUM_OF_NAMED_MUTEXES - 1)
    // *-----------------------------------------------------------*
    void OsEnterX(PNIO_UINT32 MutexId)
    {
        OS_ASSERT (OsInitialized == PNIO_TRUE);

        OS_ASSERT (MutexId < MAXNUM_OF_NAMED_MUTEXES);

        if(pdPASS == xSemaphoreTakeRecursive(NamedMutex[MutexId].mutex, portMAX_DELAY))
        {
        	 ++(NamedMutex[MutexId].count);
        }

#if 0
        if(!cyg_mutex_trylock(&(NamedMutex[MutexId].mutex)))
        {
            if(NamedMutex[MutexId].mutex.owner->unique_id != cyg_thread_get_id(cyg_thread_self()))
            {
                cyg_mutex_lock(&NamedMutex[MutexId].mutex);
            }
        }
#endif

    }

    // *-----------------------------------------------------------*
    // *  OsExitX()
    // *
    // *
    // *-----------------------------------------------------------*
    void OsExitX (PNIO_UINT32 MutexId)
    {
        // PNIO_UINT32 myid = OsGetThreadId();
        OS_ASSERT (OsInitialized == PNIO_TRUE);
        OS_ASSERT (MutexId < MAXNUM_OF_NAMED_MUTEXES)

        if(pdPASS == xSemaphoreGiveRecursive(NamedMutex[MutexId].mutex))
        {
        	--(NamedMutex[MutexId].count);
        }
        else
        {
        	OsEnterShort();
			while(1)
			{}
        }

#if 0
        if(NamedMutex[MutexId].mutex.owner->unique_id == cyg_thread_get_id(cyg_thread_self()))
        {
            --(NamedMutex[MutexId].count);
        }else
        {
            cyg_thread_info info;
            cyg_thread_get_info(cyg_thread_self(),cyg_thread_get_id(cyg_thread_self()),&info);
            LSA_TRACE_04  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL,
                    "OsExitX mutex freed from task %s/#%d,\nbut owned by %d/#%d",
                    info.name,
                    info.id,
                    NamedMutex[MutexId].mutex.owner->name,
                    NamedMutex[MutexId].mutex.owner->unique_id);

            OsEnterShort();
            while(1)
            {}
        }
        if(0 == NamedMutex[MutexId].count)
        {
            cyg_mutex_unlock(&NamedMutex[MutexId].mutex);
        }
#endif
    }

    PNIO_VOID* OsTaskGetHandle(PNIO_UINT32 TaskId)
    {
    	PNIO_VOID *pTaskHdle = LSA_NULL;
    	if((TaskId < MAXNUM_OF_TASKS) && (LSA_NULL != TaskProp[TaskId].hTaskHdl))
		{
    		pTaskHdle = TaskProp[TaskId].hTaskHdl;
		}
    	return pTaskHdle;
    }

    PNIO_VOID OsTaskNotifyFromISR(PNIO_UINT32 TaskId, PNIO_UINT32 u32Event)
	{
		if((TaskId < MAXNUM_OF_TASKS) && (LSA_NULL != TaskProp[TaskId].hTaskHdl))
		{
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			xTaskNotifyFromISR(TaskProp[TaskId].hTaskHdl, u32Event, eSetBits , &xHigherPriorityTaskWoken);
			portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
		}
	}

    PNIO_VOID OsTaskNotify(PNIO_UINT32 TaskId, PNIO_UINT32 u32Event)
	{
		if((TaskId < MAXNUM_OF_TASKS) && (LSA_NULL != TaskProp[TaskId].hTaskHdl))
		{
			xTaskNotify(TaskProp[TaskId].hTaskHdl, u32Event, eSetBits);
		}
	}

    PNIO_VOID OsTaskNotifyGiveFromISR(PNIO_UINT32 TaskId)
    {
    	if((TaskId < MAXNUM_OF_TASKS) && (LSA_NULL != TaskProp[TaskId].hTaskHdl))
    	{
    		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    		vTaskNotifyGiveFromISR(TaskProp[TaskId].hTaskHdl, &xHigherPriorityTaskWoken );
    		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    	}
    }

    PNIO_UINT32 OsTaskNotifyTake(PNIO_UINT32 TaskId)
	{
    	if((TaskId < MAXNUM_OF_TASKS) && (LSA_NULL != TaskProp[TaskId].hTaskHdl))
    	{
    		return ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
    	}

    	return 0;
	}

    PNIO_UINT32 OsTaskNotifyWait(PNIO_UINT32 TaskId)
	{
		uint32_t u32Events = 0U;
		if((TaskId < MAXNUM_OF_TASKS) && (LSA_NULL != TaskProp[TaskId].hTaskHdl))
		{
			xTaskNotifyWait(pdTRUE, ULONG_MAX, &u32Events, portMAX_DELAY);
		}
		return u32Events;
	}
    // *-----------------------------------------------------------*
    // *  TxTimerCallback()
    // *
    // *  adaption of the OS timer -calling interface to LSA Callback-
    // *  interface
    // *-----------------------------------------------------------*
    void OsTimerCallback2( TimerHandle_t xTimer )
	{
    	if(/*(0U < TskId_OsTimer) && */ (LSA_NULL != xTimer))
    	{
			OS_TIMER *ptTimer = (OS_TIMER*)pvTimerGetTimerID(xTimer);

			if( (LSA_NULL != ptTimer) && (PNIO_TRUE == ptTimer->Running))
			{
				if (ptTimer->timer_typ == LSA_TIMER_TYPE_ONE_SHOT)
				{
					ptTimer->Running = PNIO_FALSE;	// set timer state: not running
					//xTimerStop(xTimer, 0);
				}

				if(LSA_NULL != ptTimer->callback_timeout)
				{
					ptTimer->callback_timeout((PNIO_UINT16)ptTimer->TxTimerId, ptTimer->user_id);
				}

				//***** call user defined callback function *****
				//OsSendMessage(TskId_OsTimer, (void*) ptTimer->TxTimerId, OS_MBX_PRIO_NORM);
			}
    	}
	}


    PNIO_VOID OsNotifyTaskFromISR(PNIO_UINT32 TaskId)
    {
    	if((TaskId < MAXNUM_OF_TASKS) && (LSA_NULL != TaskProp[TaskId].hTaskHdl))
    	{
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			vTaskNotifyGiveFromISR(TaskProp[TaskId].hTaskHdl, &xHigherPriorityTaskWoken);
			portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    	}
    }

#if 0
    void OsTimerTask(void *pvParam)
    {
    	
    	uint32_t nNotify;
    	uint16_t nCnt = 0U;
    	while(1)
    	{
    		nNotify = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    		taskENTER_CRITICAL();
    		nCnt = 0U;
    		 while (nCnt < PCPNIO_TIMER_MAX)
			{
				if( (PNIO_TRUE == OsTimer[nCnt].Occupied) && (PNIO_TRUE == OsTimer[nCnt].Running) )
				{
					OsTimer[nCnt].nTickCnt++;

					if(OsTimer[nCnt].TimeVal_Ticks <= OsTimer[nCnt].nTickCnt )
					{
						OsTimer[nCnt].nTickCnt = 0;

						if (OsTimer[nCnt].timer_typ == LSA_TIMER_TYPE_ONE_SHOT)
						{
							OsTimer[nCnt].Running = PNIO_FALSE;	// set timer state: not running
						}

						//***** call user defined callback function *****
						OsSendMessage(TskId_OsTimer, (void*) nCnt, OS_MBX_PRIO_NORM);
					}
				}

				nCnt++;

			}

    		 taskEXIT_CRITICAL();


    	}
    }
#endif


    // *----------------------------- Task Handling -----------------------------------------------*

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

    void OsTaskEntry(void *pvParam)
    {
    	TASK_PROPERTIES *ptTaskProp = (TASK_PROPERTIES*)pvParam;
		if( (LSA_NULL != pvParam) && (LSA_NULL != ptTaskProp->pTaskEntry) )
		{
			while( (TASK_STATE_NONE == ptTaskProp->State) || (LSA_NULL == ptTaskProp->hTaskHdl) )
			{
				OsWait_ms(10);
			}


			while(TASK_STATE_EXIT != ptTaskProp->State)
			{
				switch(eTaskGetState(ptTaskProp->hTaskHdl))
				{
				case eRunning:	/* A task is querying the state of itself, so must be running. */
				//	while(TASK_STATE_OPERATING == ptTaskProp->State)
					{
						ptTaskProp->pTaskEntry();
					}
				//	OsWait_ms(10);
					break;
				case eReady:			/* The task being queried is in a read or pending ready list. */
				case eBlocked:		/* The task being queried is in the Blocked state. */
				case eSuspended:		/* The task being queried is in the Suspended state, or is in the Blocked state with an infinite time out. */
					break;
				case eDeleted:		/* The task being queried has been deleted, but its TCB has not yet been freed. */
					ptTaskProp->State = TASK_STATE_EXIT;
					break;
				case eInvalid:		/* Used as an 'invalid state' value. */
				default:
					break;
				}
			}
		}
    }

    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /*+                                                                       +*/
    /*+  function name         :    OsWaitOnEnable                           +*/
    /*+  input parameter       :    ThreadId                                  +*/
    /*+  output parameter      :    ----                                      +*/
    /*+                                                                       +*/
    /*+-----------------------------------------------------------------------+*/
    /*+                                                                       +*/
    /*+  description           :   waits until the task is enabled by the     +*/
    /*+                            creating task.                             +*/
    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    PNIO_UINT32 OsWaitOnEnable ()
    {
    	/*lint -esym(522,OsWaitOnEnable) lacks side-effects */
    	return (LSA_OK);
    }


    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /*+                                                                       +*/
    /*+  function name         :    OsGetThreadId                             +*/
    /*+  input parameter       :    --                                        +*/
    /*+  output parameter      :    ID of this thread                         +*/
    /*+                                                                       +*/
    /*+-----------------------------------------------------------------------+*/
    /*+                                                                       +*/
    /*+  description           :    returns the ID of the running thread      +*/
    /*+                                                                       +*/
    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    PNIO_UINT32  OsGetThreadId (void)
    {
		PNIO_INT32  i;
		TaskHandle_t  ThisThread;

		OS_ASSERT (OsInitialized == PNIO_TRUE);
        #if 0
		if (ECOS_intContext)
		{
			return (TskId_Post);
		}
        #endif
		ThisThread = xTaskGetCurrentTaskHandle();   // must be called from task state only, not from system or interrupt state
        for (i = 1; i < MAXNUM_OF_TASKS; i++)
        {
        	if (TaskProp[i].hTaskHdl == ThisThread)
        	{
        		return (i);
        	}
        }
		//   task not found, then return ID of a error task with message queue.
		//   if a message will be sent to this task, it will be received by the error task
		//   and a error message will be generated.
		//   a "unregistered" thread ID may be happen if a OS-Task (e.g. LSA-Timeout)
		//   calls a Callbackfunction, which sends a message and is executed in the context
		//   of the Timer Task.
        LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "invalid task not found\n");
		return (INVALID_TASK_ID);
    }


	// *-----------------------------------------------------------*
	// *  OsSetThreadPrio()
	// *  changes the priority of the specified thread
	// *
	// *  Input:  ThreadId        ID of the thread, which priority is to change
	// *          NewThreadPrio   new thread priority
	// *  output: return          LSA_OK, LSA_NOT_OK
	// *-----------------------------------------------------------*
	PNIO_UINT32 OsSetThreadPrio (PNIO_UINT32 ThreadId,
							   PNIO_UINT32 NewThreadPrio)
	{
	   OS_ASSERT (OsInitialized == PNIO_TRUE);
	   vTaskPrioritySet(TaskProp[ThreadId].hTaskHdl, NewThreadPrio);
	   return (LSA_OK);
	}


    // ******************************************************************************
    // * OsTestPointer
    // * This function checks, if a address pointer is invalid. If the pointer is
    // * ok, the pointer address is returned, otherwise an error handler is executed.
    // * There may be a need to modify the conditions, e.g. altera arm platforms use
    // * bit 31 to specify a cached/noncached memory.
    // ******************************************************************************
    PNIO_VOID*  OsTestPointer(void * ptr)
    {
    	if( ptr != 0 )
        { 	/* especially 0xCCCCCCCC */
		    if ((PNIO_UINT32) ptr >= 0x80000000L)
		    {
		        LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "invalid pointer\n");
		    }
	    }
        return (ptr);
    }

    // *--------------------------------------------*
    // *   TaskOsTimer (void)
    // *   task handler for OS-timeout requests
    // *--------------------------------------------*
#if 1

    void OsTimerCallback( TimerHandle_t xTimer )
	{
    	if(/*(0U < TskId_OsTimer) && */ (LSA_NULL != xTimer))
    	{
			OS_TIMER *ptTimer = (OS_TIMER*)pvTimerGetTimerID(xTimer);

			if( (LSA_NULL != ptTimer) && (PNIO_TRUE == ptTimer->Running))
			{
				if (ptTimer->timer_typ == LSA_TIMER_TYPE_ONE_SHOT)
				{
					ptTimer->Running = PNIO_FALSE;	// set timer state: not running
					//xTimerStop(xTimer, 0);
				}

				if( (0U < TskId_OsTimer) && (LSA_NULL != TaskProp[TskId_OsTimer].hTaskHdl))
				{
					xTaskNotifyGive(TaskProp[TskId_OsTimer].hTaskHdl);
				}
				//xTaskNotify(g_tRzPns.hPnsTask,  1, eSetBits);



				if(LSA_NULL != ptTimer->callback_timeout)
				{
				//	ptTimer->callback_timeout((PNIO_UINT16)ptTimer->TxTimerId, ptTimer->user_id);
				}

				//***** call user defined callback function *****
				//OsSendMessage(TskId_OsTimer, (void*) ptTimer->TxTimerId, OS_MBX_PRIO_NORM);
			}
    	}
	}

    PNIO_INT32  TaskOsTimer (void)
     {
 		__attribute__((unused))void* pVoid = LSA_NULL;			// pointer to Requestblock
 		__attribute__((unused))LSA_UINT32 taskID;
 		LSA_UINT16 OsTimerID = 0U;
 		OS_TIMER *ptTimer;
 		PNIO_UINT16 nCnt = 0U;
 		__attribute__((unused)) PNIO_UINT16 timer_id = 0U;
 		LSA_UINT32 TickCnt = 0U;

 		// Wait until own TaskID has been saved by the father process
 		OsWaitOnEnable(); // must be first call in every task
 		//taskID = OsGetThreadId();
 		taskID = TskId_OsTimer;
 		while(0U == TskId_OsTimer)
 		{
 			OsWait_ms(10);
 		}

 		if(0)//LSA_OK == OsAllocTimer(&OsTimerID, LSA_TIMER_TYPE_CYCLIC ,LSA_TIME_BASE_1MS , (PNIO_VOID*)OsTimerCallback))
 		{
 			if(LSA_NULL == OsTimer[OsTimerID].hTimer)
 			{
 				OsTimer[OsTimerID].hTimer = xTimerCreateStatic("Timer", OsTimer[OsTimerID].TimeVal_Ticks, pdTRUE, &OsTimer[OsTimerID] , (TimerCallbackFunction_t)OsTimerCallback, &OsTimer[OsTimerID].xTimerBuffer );
 			}
 		}

 		if(LSA_NULL != OsTimer[OsTimerID].hTimer)
 		{
 			OsStartTimer(OsTimerID, (PNIO_UINT32)&OsTimer[OsTimerID], 1);
 		}

 		// *-----------------------------------------------------------
 		// * loop forever
 		// *-----------------------------------------------------------
		const int runForever = 1;
 		while (runForever)
 		{

 			//if(PNIO_OK == OsReadMessageBlocked(&pVoid, taskID))
 			ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
 			TickCnt++;

 			{
 				//timer_id = (PNIO_UINT32) pVoid;
 				nCnt = 0U;
 				while(nCnt < PCPNIO_TIMER_MAX)
 				{
 					ptTimer = &OsTimer[nCnt++];

 					if( (LSA_NULL != ptTimer) && (0U < ptTimer->TxTimerId) && (OsTimerID != ptTimer->TxTimerId) &&
 						(PNIO_TRUE == ptTimer->Running) && (ptTimer->TimeVal_ms) )
 					{
 						if(0U == (TickCnt % ptTimer->TimeVal_Ticks) )
 						{
							if (ptTimer->timer_typ == LSA_TIMER_TYPE_ONE_SHOT)
							{
								ptTimer->Running = PNIO_FALSE;	// set timer state: not running
								//xTimerStop(xTimer, 0);
							}

							if(LSA_NULL != ptTimer->callback_timeout)
							{
								ptTimer->callback_timeout((PNIO_UINT16)ptTimer->TxTimerId, ptTimer->user_id);
							}
 						}
 					}
 				}
#if 0
 				//***** call user defined callback function *****
 				if( (timer_id < PCPNIO_TIMER_MAX) && (LSA_NULL != OsTimer[timer_id].hTimer) &&
 					(LSA_NULL != OsTimer[timer_id].callback_timeout) )
 				{
 					OsTimer[timer_id].callback_timeout((PNIO_UINT16)timer_id, OsTimer[timer_id].user_id);
 				}
#endif
 			}
 		}
 		return 0; /*lint !e527 Unreachable code */
     }
#else
    PNIO_INT32  TaskOsTimer (void)
    {
		void* pVoid = LSA_NULL;			// pointer to Requestblock
		LSA_UINT32 taskID;

		// Wait until own TaskID has been saved by the father process
		OsWaitOnEnable(); // must be first call in every task
		//taskID = OsGetThreadId();
		taskID = TskId_OsTimer;
		// *-----------------------------------------------------------
		// * loop forever
		// *-----------------------------------------------------------
		while (1)
		{
			PNIO_UINT16 nCnt = 0U;
			PNIO_UINT16 timer_id = 0U;
			if(PNIO_OK == OsReadMessageBlocked(&pVoid, taskID))
			{
				#if DEBUG_TASKCNT
					TaskCnt[taskID]++;  // for debugging only,  can be deleted
				#endif

				timer_id = (PNIO_UINT32) pVoid;

				#if _DEBUG_LOGGING_CIRC_BUF1
					// *** save actual time in a circular buffer for debugging ***
					//OsSetEntryCircBuf (FIRST_TIM_BUF + 0,  OS_ENTRY_DEFAULT );
					OsSetEntryCircBufX(FIRST_TIM_BUF + 0,  timer_id );
				#endif



				 //***** call user defined callback function *****
				if(LSA_NULL != OsTimer[timer_id].callback_timeout)
				{
					OsTimer[timer_id].callback_timeout((PNIO_UINT16)timer_id, OsTimer[timer_id].user_id);
				}
			}
		}
		return 0; /*lint !e527 Unreachable code */
    }
#endif


#endif


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
