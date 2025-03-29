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
/*  F i l e               &F: posix_os.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  internal interface of posix layer                                        */
/*                                                                           */
/*****************************************************************************/


#ifndef PNIO_SRC_SYSADAPT1_CFG_POSIX_OS_H_
#define PNIO_SRC_SYSADAPT1_CFG_POSIX_OS_H_

#include <time.h>

	PNIO_INT32 		__InterlockedDecrement 	            ( PNIO_INT32* pVal );
    PNIO_INT32 		__InterlockedExchange  	            ( PNIO_INT32* pVal, PNIO_UINT32 val );
    void	    	TaskOsTimer 			            ( void );
	void         	CreateTimerTick 		            ( void );
	time_t 			OsMeassureTimeGet 		            ( void );
    PNIO_UINT32     OsSetMainThreadId                   ( PNIO_UINT32 priority );
    PNIO_UINT16     Os_Locked_mutext_get_pthreadId      ( void * data );
	void  			OsThreadIdBacktraces 	            ( void );
	PNIO_UINT32 	BspSim_GetPDevLinkState             ( PNIO_UINT32 Port );
	PNIO_VOID       PrintRS232PostTask                  ( PNIO_VOID );

	#if (PNIO_DISABLE_OVERLOAD_CONTROL)
        #define TIMER_REDUCTION_RATIO      					64
    #else
        #define TIMER_REDUCTION_RATIO       				1
    #endif
    #define DEBUG_TEST1                 					0
    #define LTRC_ACT_MODUL_ID   							106
    #define	IOD_MODULE_ID       							LTRC_ACT_MODUL_ID
    #define LOG_MEM_ALLOCS          						0       // log memory alloc/free
    #define PRINT_MEM_ALLOCS        						0       // print memory alloc/free, only if LOG_MEM_ALLOCS is set
	#define TASKNAME_OS_TIMER								"Task_OS_Timer"
    #define TASKNAME_ASYNC_TIMER                            "Task_ASYNC_Timer"
    #define MAXNUM_OF_POSIX_SEMAPH       					(MAXNUM_OF_BIN_SEMAPH + MAXNUM_OF_TASKS)
    #define OS_DEBUG										1		// != 0: debug infos for OS are stored
    #define INVALID	                 						0xffff
    #define MAXNUM_MMTIMER_ENTRIES   						(10 * TIMER_REDUCTION_RATIO)
    #define PCPNIO_TIMER_ID_INVALID	 						0xFFFF
	#define MAX_ALIGN_SIZE      							8
	#define MSG_QUEUE_LIMIT  								MAXNUM_MSG_ENTRIES
	#define MSG_MAX_SIZE									20

	#define OS_LOG_FCT(fn_) \
    	static char* _os_log_fct = fn_;
#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
