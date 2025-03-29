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
/*  F i l e               &F: os_taskprio.h                             :F&  */
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
#ifndef OS_TASKPRIO_H                     /* ----- reinclude-protection ----- */
#define OS_TASKPRIO_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif


// *----------------------------------------------------------------*
// *    T A S K S
// *----------------------------------------------------------------*
/*----- Task Priority ----------------------*/
#include "compiler.h"
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_NETOS_NS9215)
	#define TASK_PRIO_OS_TIMER		1
	#define TASK_PRIO_APPL_CYCLE_IO 2    // highest prio for IO data exchange application
	#define TASK_PRIO_EDD           3
	#define TASK_PRIO_ORG           4
	#define TASK_PRIO_PNO           5
	#define TASK_PRIO_IP_STACK      6
	#define TASK_PRIO_POF           7
	#define TASK_PRIO_REM           8
	#define TASK_PRIO_STARTUP       9
	#define TASK_PRIO_TSKMA         10
	#define TASK_PRIO_PNPB_OWN_IND  11
	#define TASK_PRIO_PNPB          12
	#define TASK_PRIO_OS_UDP_SOCKET 13
	#define TASK_PRIO_LOW			14
	#define TASK_PRIO_PRINT_POST    15
	#define TASK_PRIO_NV_DATA		16
	#define TASK_PRIO_MAIN			17
    #define TASK_PRIO_TRCMEMXT		18
	#define TASK_PRIO_IDLE			19

#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_GENERIC_DKSW)
	#define TASK_PRIO_OS_TIMER		1
	#define TASK_PRIO_APPL_CYCLE_IO 2    // highest prio for IO data exchange application
	#define TASK_PRIO_EDD           3
	#define TASK_PRIO_ORG           4
	#define TASK_PRIO_PNO           5
	#define TASK_PRIO_IP_STACK      6
	#define TASK_PRIO_POF           7
	#define TASK_PRIO_REM           8
	#define TASK_PRIO_STARTUP       9
	#define TASK_PRIO_TSKMA         10
	#define TASK_PRIO_PNPB_OWN_IND  11
	#define TASK_PRIO_PNPB          12
	#define TASK_PRIO_OS_UDP_SOCKET 13
	#define TASK_PRIO_LOW			14
	#define TASK_PRIO_PRINT_POST    15
	#define TASK_PRIO_NV_DATA		16
	#define TASK_PRIO_MAIN			17
    #define TASK_PRIO_TRCMEMXT		18
	#define TASK_PRIO_IDLE			19

#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_DKSW_ATMEL)
	#define TASK_PRIO_OS_TIMER		1
	#define TASK_PRIO_APPL_CYCLE_IO 2    /* highest prio for IO data exchange application */
	#define TASK_PRIO_EDD           3	 /* EDD higher than all tasks below */
	#define TASK_PRIO_ORG           4
	#define TASK_PRIO_PNO           4
	#define TASK_PRIO_IP_STACK      4
	#define TASK_PRIO_POF           4
	#define TASK_PRIO_PNPB_OWN_IND  4
	#define TASK_PRIO_PNPB          4
	#define TASK_PRIO_OS_UDP_SOCKET 4
	#define TASK_PRIO_LOW			4
	#define TASK_PRIO_PRINT_POST    4
	#define TASK_PRIO_REM           5	/* flash access */
	#define TASK_PRIO_NV_DATA		5	/* flash access */
	#define TASK_PRIO_MAIN			5
	#define TASK_PRIO_STARTUP       5
	#define TASK_PRIO_TSKMA         6	/* IDLE */
    #define TASK_PRIO_TRCMEMXT		6
	#define TASK_PRIO_IDLE			7

#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_WIN_DKSW)
	#define TASK_PRIO_OS_TIMER		20
    #define TASK_PRIO_APPL_CYCLE_IO 19    // highest prio for IO data exchange application
    #define TASK_PRIO_EDD           18
    #define TASK_PRIO_ORG           17
    #define TASK_PRIO_PNO           16
    #define TASK_PRIO_IP_STACK      15
    #define TASK_PRIO_POF           14
    #define TASK_PRIO_REM           13
    #define TASK_PRIO_STARTUP       12
    #define TASK_PRIO_TSKMA         11
    #define TASK_PRIO_PNPB_OWN_IND  10
    #define TASK_PRIO_PNPB           9
    #define TASK_PRIO_OS_UDP_SOCKET  8
    #define TASK_PRIO_LOW			 7
    #define TASK_PRIO_PRINT_POST     6
    #define TASK_PRIO_NV_DATA		 5
    #define TASK_PRIO_MAIN			 4
    #define TASK_PRIO_TRCMEMXT		 2
    #define TASK_PRIO_IDLE			 1

#elif (PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200P | PNIOD_PLATFORM_ECOS_EB200 | PNIOD_PLATFORM_ECOS_EB400))
	#ifdef CYGDAT_NET_STACK_CFG
		#include CYGDAT_NET_STACK_CFG
	#endif
	#include <pkgconf/kernel.h>

	/* ECOS taskpriorities for standard scheduler */
	#define TASK_PRIO_OS_TIMER		            1
    #define TASK_PRIO_APPL_CYCLE_IO             2    // highest prio for IRT C3 and sync. application
    #define TASK_PRIO_EDD                       3
    #define TASK_PRIO_ORG                       3
    #define TASK_PRIO_PNO                       3
    #define TASK_PRIO_IP_STACK                  3
    #define TASK_PRIO_POF                       3
    #define TASK_PRIO_REM                       3
    #define TASK_PRIO_STARTUP                   3
    #define TASK_PRIO_TSKMA                     3
    #define TASK_PRIO_PNPB_OWN_IND              3
    #define TASK_PRIO_PNPB                      3
    #define TASK_PRIO_OS_UDP_SOCKET             3
    #define TASK_PRIO_LOW			            3
	#if (PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200P))
	    #define TASK_PRIO_PRINT_POST                4
	    #define TASK_PRIO_NV_DATA		            4
	#else
        #define TASK_PRIO_PRINT_POST                3
        #define TASK_PRIO_NV_DATA		            3
	#endif
    #define TASK_PRIO_MAIN                      5
    #define TASK_PRIO_APPL_CYCLE_IO_LOW         4
    #define TASK_PRIO_TRCMEMXT		            5
    #define TASK_PRIO_IDLE			            6

#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_LINUX_PC)
	#define TASK_PRIO_ASYNC_TIMER               30
	#define TASK_PRIO_OS_TIMER                  30
	#define TASK_PRIO_APPL_CYCLE_IO             29    // highest prio for IO data exchange application
	#define TASK_PRIO_EDD                       28
	#define TASK_PRIO_ORG                       28
	#define TASK_PRIO_PNO                       28
	#define TASK_PRIO_IP_STACK                  28
	#define TASK_PRIO_POF                       28
	#define TASK_PRIO_REM                       28
	#define TASK_PRIO_STARTUP                   28
	#define TASK_PRIO_TSKMA                     28
	#define TASK_PRIO_PNPB_OWN_IND              28
	#define TASK_PRIO_PNPB                      28
	#define TASK_PRIO_OS_UDP_SOCKET             28
	#define TASK_PRIO_LOW                       28
	#define TASK_PRIO_PRINT_POST                27
	#define TASK_PRIO_NV_DATA                   27
	#define TASK_PRIO_MAIN                      27
    #define TASK_PRIO_APPL_CYCLE_IO_LOW         27
	#define TASK_PRIO_TRCMEMXT                  26
	#define TASK_PRIO_IDLE                      25
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_POSIX_EB200P )
    #define TASK_PRIO_ASYNC_TIMER               30
    #define TASK_PRIO_OS_TIMER                  30
    #define TASK_PRIO_APPL_CYCLE_IO             29    // highest prio for IO data exchange application
    #define TASK_PRIO_EDD                       28
    #define TASK_PRIO_ORG                       28
    #define TASK_PRIO_PNO                       28
    #define TASK_PRIO_IP_STACK                  28
    #define TASK_PRIO_POF                       28
    #define TASK_PRIO_REM                       28
    #define TASK_PRIO_STARTUP                   28
    #define TASK_PRIO_TSKMA                     28
    #define TASK_PRIO_PNPB_OWN_IND              28
    #define TASK_PRIO_PNPB                      28
    #define TASK_PRIO_OS_UDP_SOCKET             28
    #define TASK_PRIO_LOW                       28
    #define TASK_PRIO_PRINT_POST                27
    #define TASK_PRIO_NV_DATA                   27
    #define TASK_PRIO_MAIN                      26
    #define TASK_PRIO_APPL_CYCLE_IO_LOW         27
    #define TASK_PRIO_TRCMEMXT                  26
    #define TASK_PRIO_IDLE                      25
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2 )
#if 0
	#define TASK_PRIO_ASYNC_TIMER               8
	#define TASK_PRIO_OS_TIMER                  8
	#define TASK_PRIO_APPL_CYCLE_IO             7    // highest prio for IO data exchange application
	#define TASK_PRIO_EDD                       6
	#define TASK_PRIO_ORG                       6
	#define TASK_PRIO_PNO                       6
	#define TASK_PRIO_IP_STACK                  6
	#define TASK_PRIO_POF                       6
	#define TASK_PRIO_REM                       6
	#define TASK_PRIO_STARTUP                   6
	#define TASK_PRIO_TSKMA                     6
	#define TASK_PRIO_PNPB_OWN_IND              6
	#define TASK_PRIO_PNPB                      6
	#define TASK_PRIO_OS_UDP_SOCKET             6
	#define TASK_PRIO_LOW                       6
	#define TASK_PRIO_PRINT_POST                5
	#define TASK_PRIO_NV_DATA                   5
	#define TASK_PRIO_MAIN                      4
	#define TASK_PRIO_APPL_CYCLE_IO_LOW         5
	#define TASK_PRIO_TRCMEMXT                  4
	#define TASK_PRIO_IDLE                      3

#define TASK_PRIO_OS_TIMER                  29
    #define TASK_PRIO_APPL_CYCLE_IO             29    // highest prio for IO data exchange application
    #define TASK_PRIO_EDD                       29
    #define TASK_PRIO_ORG                       28
    #define TASK_PRIO_PNO                       28
    #define TASK_PRIO_IP_STACK                  28
    #define TASK_PRIO_POF                       28
    #define TASK_PRIO_REM                       28
    #define TASK_PRIO_STARTUP                   28
    #define TASK_PRIO_TSKMA                     28
    #define TASK_PRIO_PNPB_OWN_IND              28
    #define TASK_PRIO_PNPB                      28
    #define TASK_PRIO_OS_UDP_SOCKET             28
    #define TASK_PRIO_LOW                       28
    #define TASK_PRIO_PRINT_POST                28
    #define TASK_PRIO_NV_DATA                   28
    #define TASK_PRIO_MAIN                      28
    #define TASK_PRIO_APPL_CYCLE_IO_LOW         28
    #define TASK_PRIO_TRCMEMXT                  26
    #define TASK_PRIO_IDLE                      30

#else
#if 0
//    #define TASK_PRIO_ASYNC_TIMER               30
    #define TASK_PRIO_OS_TIMER                  20
    #define TASK_PRIO_APPL_CYCLE_IO             16    // highest prio for IO data exchange application
    #define TASK_PRIO_EDD                       14
    #define TASK_PRIO_ORG                       14
    #define TASK_PRIO_PNO                       14
    #define TASK_PRIO_IP_STACK                  14
    #define TASK_PRIO_POF                       14
    #define TASK_PRIO_REM                       14
    #define TASK_PRIO_STARTUP                   14
    #define TASK_PRIO_TSKMA                     14
    #define TASK_PRIO_PNPB_OWN_IND              14
    #define TASK_PRIO_PNPB                      14
    #define TASK_PRIO_OS_UDP_SOCKET             14
    #define TASK_PRIO_LOW                       13
    #define TASK_PRIO_PRINT_POST                12
    #define TASK_PRIO_NV_DATA                   14
    #define TASK_PRIO_MAIN                      13
    #define TASK_PRIO_APPL_CYCLE_IO_LOW         13
    #define TASK_PRIO_TRCMEMXT                  12
    #define TASK_PRIO_IDLE                      16
#else
#define TASK_PRIO_ASYNC_TIMER               29
#define TASK_PRIO_OS_TIMER                  29
#define TASK_PRIO_APPL_CYCLE_IO             27  //-OS-- 28    // highest prio for IO data exchange application
#define TASK_PRIO_EDD                       26
#define TASK_PRIO_ORG                       25
#define TASK_PRIO_PNO                       25
#define TASK_PRIO_IP_STACK                  25
#define TASK_PRIO_POF                       25
#define TASK_PRIO_REM                       25
#define TASK_PRIO_STARTUP                   25
#define TASK_PRIO_TSKMA                     25
#define TASK_PRIO_PNPB_OWN_IND              25
#define TASK_PRIO_PNPB                      25
#define TASK_PRIO_OS_UDP_SOCKET             25
#define TASK_PRIO_LOW                       23
#define TASK_PRIO_PRINT_POST                24
#define TASK_PRIO_NV_DATA                   24
#define TASK_PRIO_MAIN                      24
#define TASK_PRIO_APPL_CYCLE_IO_LOW         23
#define TASK_PRIO_TRCMEMXT                  22
#define TASK_PRIO_IDLE                      26
#define TASK_PRIO_EDDP_LL_CTRL              24     // 26 Axel  /* EDDP_LL_CTRL_*/
#define TASK_PRIO_LED_CTRL                  24     /* BspLed_CtrlTask*/
#endif
#endif
#else
	#error "platform not defined"
#endif



#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
