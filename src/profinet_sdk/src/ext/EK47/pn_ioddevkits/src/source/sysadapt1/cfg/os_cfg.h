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
/*  F i l e               &F: os_cfg.h                                  :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  OS dependent configuration for PNIO Devkit                               */
/*                                                                           */
/*****************************************************************************/
#ifndef OS_CFG_H                     /* ----- reinclude-protection ----- */
#define OS_CFG_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

#include "compiler.h"


#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS)
    //#define SOCKERRORS_ALREADY
    #define EFAULT ESOCKTNOSUPPORT
    #define EHAVEOOB ENOERR
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_NETOS_NS9215)
    //#define SOCKERRORS_ALREADY
    //#define EFAULT ESOCKTNOSUPPORT
    //#define EHAVEOOB ENOERR
#else
	// nothing at the moment...
#endif

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_DKSW)
#define OS_BSS_FAST
#define OS_BSS_STACK_FAST
#define OS_DATA_FAST
#define OS_CODE_FAST
#elif(PNIOD_PLATFORM & PNIOD_PLATFORM_IAR)
#define OS_BSS_DTCM_SECTION_NAME            "xdata_bss_dtcm"
#define OS_BSS_STACK_SECTION_NAME            "bss_stack"
#define OS_DATA_DTCM_SECTION_NAME           "xdata_d_tcm"
#define OS_ITCM_SECTION_NAME                "xtext_i_tcm"

#define OS_BSS_FAST
#define OS_BSS_STACK_FAST
#define OS_DATA_FAST
#define OS_CODE_FAST

//#define OS_BSS_FAST                         __attribute__((used, section (OS_BSS_DTCM_SECTION_NAME)))
//#define OS_BSS_STACK_FAST                   __attribute__((used, section (OS_BSS_STACK_SECTION_NAME)))
//#define OS_DATA_FAST                        __attribute__ ((used, section (OS_DATA_DTCM_SECTION_NAME)))
//#define OS_CODE_FAST                        __attribute__ ((used, section (OS_ITCM_SECTION_NAME)))
#else
/*TCM usage*/
#define OS_BSS_DTCM_SECTION_NAME            ".xdata_bss_dtcm"
#define OS_BSS_STACK_SECTION_NAME            ".bss_stack"
#define OS_DATA_DTCM_SECTION_NAME           ".xdata_d_tcm"
#define OS_ITCM_SECTION_NAME                ".xtext_i_tcm"

#define OS_BSS_FAST                         __attribute__((section (OS_BSS_DTCM_SECTION_NAME)))
#define OS_BSS_STACK_FAST                   __attribute__((section (OS_BSS_STACK_SECTION_NAME)))
#define OS_DATA_FAST                        __attribute__ ((section (OS_DATA_DTCM_SECTION_NAME)))
#define OS_CODE_FAST                        __attribute__ ((section (OS_ITCM_SECTION_NAME)))
#endif

// *----------------------------------------------------------------*
// *    options  (default: 0)
// *----------------------------------------------------------------*

/*----- Tasks ----------------------*/
#define MAXNUM_OF_TASKS				 25		// number of usable tasks = MAXNUM_OF_TASKS - 1  !!
#define MAXSIZE_TASKNAME             0  // 32-> 0     // max number of bytes for taskname


// *----------------------------------------------------------------*
// *    default task stacksize
// *----------------------------------------------------------------*
#define TASK_STACKSIZE_DEFAULT       (OS_CFG_STACK_SIZE)	//4k default stack
 

// *----------------------------------------------------------------*
// *    M E S S A G E S
// *----------------------------------------------------------------*
#define OS_MBX_PRIO_NORM			  5		// default pnio message priority
#define MAXNUM_MSG_ENTRIES			 100    // maximal number of entries in a message queue
#define MSG_ENTRY_SIZE                8     // size of one message entry (must be 4,8,16,32,64 bytes)


// *----------------------------------------------------------------*
// *    Timer
// *----------------------------------------------------------------*
#define PCPNIO_TIMER_MAX		    40

// *----------------------------------------------------------------*
// *    Semaphores and flags
// *----------------------------------------------------------------*
#define MAXNUM_OF_NAMED_MUTEXES	    OS_MUTEX_Reserved  // maximum number of Mutexes for Enter ()/Exit()
#define MAXNUM_OF_BIN_SEMAPH	    35  			   // maximum number of bin semaphores

// *** default mutex for OsEnter, OsExit
#define OS_MUTEX_DEFAULT			0		// Mutex default, used from OsEnter

// *** mutexes for the other LSA layers ***
#define OS_MUTEX_TCPIP              1       // Mutex, used by IP-stack
#define OS_MUTEX_TCPIP_NET_RESID    2       // Mutex, used by IP-stack to lock ressources
#define OS_MUTEX_TCPIP_RXQ_RESID    3       // Mutex, used by IP-stack to lock ressources
#define OS_MUTEX_TCPIP_FREEQ_RESID  4       // Mutex, used by IP-stack to lock ressources
#define OS_MUTEX_SOCK				5		// Mutex, used by SOCK
#define OS_MUTEX_XX_MEM				6		// Mutex, used by XX_MEM
#define OS_MUTEX_NV_DATA	  		7		// Mutex, used by nv ram access (rd/wr)
#define OS_MUTEX_LTRC   	  		8		// Mutex, used by LTRC
#define OS_MUTEX_PNPB				9		// Mutex, used by PNPB for user api
#define OS_MUTEX_PNPB_IO            10       // Mutex, used by PNPB for user io access
#define OS_MUTEX_PNPB_RD_PERI       11       // Mutex, used by PNPB to read from pndv-perif
#define OS_MUTEX_PNPB_WR_PERI       12       // Mutex, used by PNPB to write to pndv-perif
#define OS_MUTEX_PNPB_PLUG          13      // Mutex, used by PNPB to lock against multiple plug/pull call from user threads
#define OS_MUTEX_IOM                14      // Mutex, used by IOM
#define OS_MUTEX_PNIO				15
#define OS_MUTEX_TIMER              16
#define OS_MUTEX_FLASH_DATA         17
#define OS_MUTEX_GSY                18
#define OS_MUTEX_PNPB_PULL          19
#define OS_MUTEX_EDDS				20
#define OS_MUTEX_EDDS_IFCTRL		21
#define OS_MUTEX_EDDS_INTERLOCK	    22
#define OS_MUTEX_USR_1		        23
#define OS_MUTEX_USR_2		        24
#define OS_MUTEX_USR_3		        25
#define OS_MUTEX_USR_4		        26
// *** must be the last number.. ***
#define OS_MUTEX_Reserved			27		// must be the last one, to find out number of mutexes


#if (PNIOD_PLATFORM & PNIOD_PLATFORM_DKSW)  // SOFT EDD only
    // *===============================================================================
    // *   defines for SOFT EDD only (not ERTEC EDD)
    // *===============================================================================


    #define TASK_UDP_REC_PAUSETIME		 4		// wait 4 msec before handle the next NRT telegram, to limit system load
	#define IO_UPDATE_POLLING_MS         4		// polling cycle time for IO update

    // *------------------------------------------------------
    // * the following define enables a workaround, if the
    // * operating system does not support thread priority
    // * changing on the fly.
    // * attention: no other task may have a priority between
    // * the priority of EDD-High and EDD-Low, if workaround is
    // * enabled !!. This may downgrade the realtime properties
    // * of PNIO.
    // *------------------------------------------------------
    #if 1 // always use mutex mechanism
        #define  EDD_SCHEDULED_BY_MUTEX	1
    #else
        #define  EDD_SCHEDULED_BY_MUTEX	0
    #endif

    // *------------------------------------------------------
    // *   EDD_CFG_CYCLE_BASE_FACTOR configures the base cycle
    // *   time for the SOFT-EDD  (don't care for ERTEC EDD)
    // *
    // *   NOTE: The possible IO bus cycle time must be greater
    // *   ore equal than the EDD base cycle time.
    // *   Also the edd base cycle time must be greater or equal
    // *   to schedule interrupt time of the operating system.
    // *
    // *   possible values for EDD_CFG_CYCLE_BASE_FACTOR are
    // *    32      1.0  msec
    // *
    // *------------------------------------------------------
    #define EDD_CFG_CYCLE_BASE_FACTOR	32  // must be 32 (1 msec), if shared device is used !!


#else	// SOFT EDD only

	#define IO_UPDATE_POLLING_MS         1		// polling cycle time for IO update

#endif



#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
