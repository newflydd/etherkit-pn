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
/*  F i l e               &F: compiler.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  compiler dependent definitions                                           */
/*                                                                           */
/*****************************************************************************/
#ifndef _COMPILER_H
#define _COMPILER_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif


// *------------------------------------------------------
// * include compiler1.h at the start of this file
// *------------------------------------------------------
#include "compiler1.h"      // don't change


// *========================================================*
// *   PLATFORM DEFINITIONS
// *========================================================*
// *---------------------------------------------
// * define platform  (select 1 of N)
// * we use at the moment
// *  PNIOD_PLATFORM_ECOS_DKSW_ATMEL PNIO developers kit for standard ethernet controller
// *  PNIOD_PLATFORM_ECOS_EB400      ERTEC 400  developers kit
// *  PNIOD_PLATFORM_ECOS_EB200      ERTEC 200  developers kit
// *  PNIOD_PLATFORM_ECOS_EB200P     ERTEC 200P evaluation kit
// *---------------------------------------------
// **** level 1 of platforms ***
#define PNIOD_PLATFORM_NONE              	   0x00000000	    // no valid platform
#define PNIOD_PLATFORM_GENERIC_DKSW      	   0x00000001	    // generic platform for DK_SW, can be used as a customer's template (default: same as DKSW_ATMEL)
#define PNIOD_PLATFORM_ECOS_DKSW_ATMEL   	   0x00000002	    // product developers kit atmel sam 9x25 (DK_SW)
#define PNIOD_PLATFORM_ECOS_EB200        	   0x00000004     // eCOS Platform ekit based eb400/200 + SOCK-lsa  + Interniche IP Stack
#define PNIOD_PLATFORM_ECOS_EB400        	   0x00000008     // eCOS Platform ekit based eb400/200 + SOCK-lsa  + Interniche IP Stack
#define PNIOD_PLATFORM_ECOS_EB200P       	   0x00000010     // eCOS Platform ekit based eb200p + SOCK-lsa  + Interniche IP Stack
#define PNIOD_PLATFORM_POSIX_EB200P       	   0x00000040     // eCOS Platform ekit based eb200p + SOCK-lsa  + Interniche IP Stack

#define PNIOD_PLATFORM_WIN_DKSW          	   0x00000080	    // test only: DK Std-Ethernet + Windows XP
#define PNIOD_PLATFORM_WIN_EB200P        	   0x00000100	    // test only: EB200P  + Windows XP
#define PNIOD_PLATFORM_WIN_EB200         	   0x00000200	    // test only: EB200   + Windows XP
#define PNIOD_PLATFORM_WIN_EB400         	   0x00000400	    // test only: EB400   + Windows XP

#define PNIOD_PLATFORM_NETOS_NS9215      	   0x00000800	    // test only: DK Std-Ethernet, NETOS 7.5 on Digi NS9215 controller
#define PNIOD_PLATFORM_LINUX_PC          	   0x00001000	    // test only:POSIX_DKSW
#define PNIOD_PLATFORM_RZT2_EDDS          	   0x00002000	    // RZT2-EDDS-FreeRTOS Integration
#define PNIOD_PLATFORM_RZT2_EDDP			   0x00004000	    // RZT2-EDDP-FreeRTOS Integration
// *---------------------------------------------
// * derived platform definitions
// *---------------------------------------------
// **** level 2 of platforms ***
#define PNIOD_PLATFORM_ECOS_EB200_400   (PNIOD_PLATFORM_ECOS_EB200      | PNIOD_PLATFORM_ECOS_EB400)
#define PNIOD_PLATFORM_ECOS             (PNIOD_PLATFORM_ECOS_EB200P     | PNIOD_PLATFORM_ECOS_EB200_400 | PNIOD_PLATFORM_ECOS_DKSW_ATMEL)
#define PNIOD_PLATFORM_POSIX            (PNIOD_PLATFORM_POSIX_EB200P    | PNIOD_PLATFORM_LINUX_PC)
#define PNIOD_PLATFORM_EB200            (PNIOD_PLATFORM_ECOS_EB200      | PNIOD_PLATFORM_WIN_EB200)
#define PNIOD_PLATFORM_EB400            (PNIOD_PLATFORM_ECOS_EB400      | PNIOD_PLATFORM_WIN_EB400)
#define PNIOD_PLATFORM_EB200P           (PNIOD_PLATFORM_ECOS_EB200P     | PNIOD_PLATFORM_WIN_EB200P | PNIOD_PLATFORM_POSIX_EB200P )
#define PNIOD_PLATFORM_ECOS_ERTEC       (PNIOD_PLATFORM_ECOS_EB200      | PNIOD_PLATFORM_ECOS_EB400 | PNIOD_PLATFORM_ECOS_EB200P | PNIOD_PLATFORM_POSIX_EB200P )

// **** level 3 of platforms ***
#define PNIOD_PLATFORM_EB200_400        (PNIOD_PLATFORM_EB200           | PNIOD_PLATFORM_EB400 )
#define PNIOD_PLATFORM_EBx00            (PNIOD_PLATFORM_EB200P          | PNIOD_PLATFORM_EB200          | PNIOD_PLATFORM_EB400)
#define PNIOD_PLATFORM_DKSW             (PNIOD_PLATFORM_ECOS_DKSW_ATMEL | PNIOD_PLATFORM_GENERIC_DKSW   | PNIOD_PLATFORM_NETOS_NS9215    | PNIOD_PLATFORM_WIN_DKSW  | PNIOD_PLATFORM_LINUX_PC | PNIOD_PLATFORM_RZT2_EDDS)


// **** level 4 of platforms ***
#define PNIOD_PLATFORM_WIN              (PNIOD_PLATFORM_WIN_EB200P      | PNIOD_PLATFORM_WIN_EB200      | PNIOD_PLATFORM_WIN_EB400       | PNIOD_PLATFORM_WIN_DKSW)
#define PNIOD_PLATFORM_GNU              (PNIOD_PLATFORM_ECOS_EB200P     | PNIOD_PLATFORM_ECOS_EB200     | PNIOD_PLATFORM_ECOS_EB400      \
                                       | PNIOD_PLATFORM_ECOS_DKSW_ATMEL | PNIOD_PLATFORM_NETOS_NS9215   | PNIOD_PLATFORM_LINUX_PC  | PNIOD_PLATFORM_POSIX_EB200P )

#define PNIOD_PLATFORM_IAR				(PNIOD_PLATFORM_RZT2_EDDS | PNIOD_PLATFORM_RZT2_EDDP)	// RZT2-FreeRTOS Integration
#define PNIOD_PLATFORM_RZT2				(PNIOD_PLATFORM_RZT2_EDDS | PNIOD_PLATFORM_RZT2_EDDP)	// RZT2-FreeRTOS Integration
#if 0

/* controllers */
#define PNIOD_PLATFORM_I686                     (1 << 0)
#define PNIOD_PLATFORM_ATMEL                    (1 << 1)
#define PNIOD_PLATFORM_ERTEC200                 (1 << 2)
#define PNIOD_PLATFORM_ERTEC400                 (1 << 3)
#define PNIOD_PLATFORM_ERTEC200P                (1 << 4)
#define PNIOD_PLATFORM_NS9215                   (1 << 5)

/* boards */
#define PNIOD_PLATFORM_DKSW                     (1 << 6)
#define PNIOD_PLATFORM_EB200                    (1 << 7)
#define PNIOD_PLATFORM_EB200P                   (1 << 8)
#define PNIOD_PLATFORM_EB400                    (1 << 9)

/* os interface */
#define PNIOD_PLATFORM_POSIX                    (1 << 10)
#define PNIOD_PLATFORM_NATIVE                   (1 << 11)

/* TCP/IP stack */
#define PNIOD_PLATFORM_TCP_NATIVE               (1 << 12)
#define PNIOD_PLATFORM_TCP_INSIDE_OS            (1 << 13)

/* compilers */
#define PNIOD_PLATFORM_GNU                      (1 << 14)
#define PNIOD_PLATFORM_WIN                      (1 << 15)

/* operation system */
#define PNIOD_PLATFORM_WIN                      (1 << 16)
#define PNIOD_PLATFORM_ECOS                     (1 << 17)
#define PNIOD_PLATFORM_LINUX                    (1 << 18)
#define PNIOD_PLATFORM_NETOS                    (1 << 19)

/* products */
#define PNIOD_PLATFORM_ECOS_EB200                     ( PNIOD_PLATFORM_ERTEC     | PNIOD_PLATFORM_EB200  | PNIOD_PLATFORM_ECOS | PNIOD_PLATFORM_NATIVE | PNIOD_PLATFORM_TCP_NATIVE | PNIOD_PLATFORM_GNU )
#define PNIOD_PLATFORM_ECOS_EB200B                    ( PNIOD_PLATFORM_ERTEC200P | PNIOD_PLATFORM_EB200P  | PNIOD_PLATFORM_ECOS | PNIOD_PLATFORM_NATIVE | PNIOD_PLATFORM_TCP_NATIVE | PNIOD_PLATFORM_GNU )
#define PNIOD_PLATFORM_ECOS_EB400                     ( PNIOD_PLATFORM_ERTEC400  | PNIOD_PLATFORM_EB400  | PNIOD_PLATFORM_ECOS | PNIOD_PLATFORM_NATIVE | PNIOD_PLATFORM_TCP_NATIVE | PNIOD_PLATFORM_GNU )
#define PNIOD_PLATFORM_ECOS_DKSW_ATMEL                ( PNIOD_PLATFORM_ERTEC     | PNIOD_PLATFORM_DKSW   | PNIOD_PLATFORM_ECOS | PNIOD_PLATFORM_NATIVE | PNIOD_PLATFORM_TCP_NATIVE | PNIOD_PLATFORM_GNU )

#define PNIOD_PLATFORM_ECOS_POSIX_EB200P              ( PNIOD_PLATFORM_ERTEC200P | PNIOD_PLATFORM_EB200P  | PNIOD_PLATFORM_ECOS | PNIOD_PLATFORM_POSIX | PNIOD_PLATFORM_TCP_NATIVE | PNIOD_PLATFORM_GNU)

#define PNIOD_PLATFORM_WIN_DKSW                       ( PNIOD_PLATFORM_I686 | PNIOD_PLATFORM_DKSW  | PNIOD_PLATFORM_WIN | PNIOD_PLATFORM_NATIVE | PNIOD_PLATFORM_TCP_NATIVE | PNIOD_PLATFORM_WIN )
#define PNIOD_PLATFORM_WIN_EB200P                     ( PNIOD_PLATFORM_I686 | PNIOD_PLATFORM_EB200  | PNIOD_PLATFORM_WIN | PNIOD_PLATFORM_NATIVE | PNIOD_PLATFORM_TCP_NATIVE | PNIOD_PLATFORM_WIN )
#define PNIOD_PLATFORM_WIN_EB200                      ( PNIOD_PLATFORM_I686 | PNIOD_PLATFORM_EB200P  | PNIOD_PLATFORM_WIN | PNIOD_PLATFORM_NATIVE | PNIOD_PLATFORM_TCP_NATIVE | PNIOD_PLATFORM_WIN )
#define PNIOD_PLATFORM_WIN_EB400                      ( PNIOD_PLATFORM_I686 | PNIOD_PLATFORM_EB400  | PNIOD_PLATFORM_WIN | PNIOD_PLATFORM_NATIVE | PNIOD_PLATFORM_TCP_NATIVE | PNIOD_PLATFORM_WIN )

#define PNIOD_PLATFORM_LINUX_POSIX_DKSW               ( PNIOD_PLATFORM_I686 | PNIOD_PLATFORM_DKSW  | PNIOD_PLATFORM_ECOS | PNIOD_PLATFORM_POSIX | PNIOD_PLATFORM_TCP_INSIDE_OS | PNIOD_PLATFORM_GNU )

#define PNIOD_PLATFORM_NETOS_NS9215_DKSW              ( PNIOD_PLATFORM_NS9215 | PNIOD_PLATFORM_DKSW | PNIOD_PLATFORM_NETOS |  PNIOD_PLATFORM_NATIVE | PNIOD_PLATFORM_TCP_NATIVE | PNIOD_PLATFORM_GNU )

#endif

// *--------------------------------------------------*
// *  automatic platform select
// *  (for test only, may be deleted)
// *  if no platform per auto_platform_select.h was
// *  defined, select a platform here.
// *--------------------------------------------------*
#if 1
	#include "auto_platform_select.h"
#else
	#define PNIOD_PLATFORM	          PNIOD_PLATFORM_ECOS_EB200
//	#define PNIOD_PLATFORM	          PNIOD_PLATFORM_ECOS_EB400
//	#define PNIOD_PLATFORM	          PNIOD_PLATFORM_NETOS_NS9215
#endif



// *-------------------------------------------------------------------------*
// *   platform: generic 32 bit microcontroller
// *-------------------------------------------------------------------------*
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_GENERIC_DKSW)
	// *--------------------------------------
	// * alignment orders und endian format
	// *--------------------------------------
	#define ATTR_PNIO_PACKED          // nothing __attribute__((packed))
	#define ATTR_PNIO_PACKED_PRE      // nothing

	#define PNIO_BIG_ENDIAN 	0

// *-------------------------------------------------------------------------*
// *  platform: EB200P, EB200, EB400,  tool chain GNU
// *-------------------------------------------------------------------------*
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_EBx00)
    // *--------------------------------------
    // * alignment orders und endian format
    // *--------------------------------------
    #define ATTR_PNIO_PACKED          __attribute__((packed))
    #define ATTR_PNIO_PACKED_PRE      // nothing

    // GNU Tool chain can be  set via Make configuration.
    #define TOOL_CHAIN_GNU      1

    #define PNIO_BIG_ENDIAN 	0


// *-------------------------------------------------------------------------*
// *   platform: DKSW, Atmel SAM 9x25,  tool chain GNU
// *-------------------------------------------------------------------------*
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_DKSW_ATMEL)
    // *--------------------------------------
    // * alignment orders und endian format
    // *--------------------------------------
    #define ATTR_PNIO_PACKED          // nothing __attribute__((packed))
    #define ATTR_PNIO_PACKED_PRE      // nothing

    #define PNIO_BIG_ENDIAN 	0

// *-------------------------------------------------------------------------*
// *   platform: DKSW, Digi NS9215,  tool chain GNU
// *-------------------------------------------------------------------------*
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_NETOS_NS9215)
    // *--------------------------------------
    // * alignment orders und endian format
    // *--------------------------------------
    #define ATTR_PNIO_PACKED          __attribute__((packed))
    #define ATTR_PNIO_PACKED_PRE      // nothing

    #define PNIO_BIG_ENDIAN 	1


// *-------------------------------------------------------------------------*
// *   TOOL CHAIN MICROSOFT   (only an example, not tested !!)
// *-------------------------------------------------------------------------*
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_WIN)
    // *--------------------------------------
    // * alignment orders und endian format
    // *--------------------------------------
    #define ATTR_PNIO_PACKED                // nothing
    #define ATTR_PNIO_PACKED_PRE            // nothing

    #define PNIO_BIG_ENDIAN 	0

    // **** suppress specific compiler warnings ****
    #pragma warning (disable : 4103 )
    #pragma warning (disable : 4244 )
    #pragma warning (disable : 4761 )

    #ifdef NULL
        #undef NULL
    #endif

// *-------------------------------------------------------------------------*
// *   platform: POSIX on LINUX  tool chain GNU
// *-------------------------------------------------------------------------*
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_LINUX_PC)
    // *--------------------------------------
    // * alignment orders und endian format
    // *--------------------------------------
    #define ATTR_PNIO_PACKED          __attribute__((packed))
    #define ATTR_PNIO_PACKED_PRE      // nothing

    #define PNIO_BIG_ENDIAN 	0
// *-------------------------------------------------------------------------*
// *  platform: RZT2,  tool chain IAR
// *-------------------------------------------------------------------------*
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2)
    // *--------------------------------------
    // * alignment orders und endian format
    // *--------------------------------------
    #define ATTR_PNIO_PACKED          __attribute__((packed))
    #define ATTR_PNIO_PACKED_PRE      // nothing

    // GNU Tool chain can be  set via Make configuration.
#ifdef __ICCARM__
    #define TOOL_CHAIN_IAR      1
#else
    #define TOOL_CHAIN_GNU      1
#endif

    #define PNIO_BIG_ENDIAN 	0
#else
	#error "platform not defined"
#endif

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_POSIX)
	#define INCLUDE_POSIX_INTERFACE		1
#endif

// *------------------------------------------------------
// * some definitions from standard header files
// *------------------------------------------------------
#ifndef NULL
#define NULL    (void*)0
#endif

// *------------------------------------------------------------
// * include/exclude special functions (0:default, 1: activated)
// *------------------------------------------------------------
#define  INCLUDE_DK_MSG_HANDLER     0
#define  INCLUDE_DK_STDLIB_HANDLER  0

// *------------------------------------------------------
// * DEBUG defines  (LSA trace macros, error logging)
// *
// * note: set to 1  increases code size.
// *------------------------------------------------------
#define  DEBUG_TASKCNT                  0      // 1: loop counter for all pn tasks

#define  DEBUG_1                        0      // for debugging only
#define  DEBUG_2                        0      // for debugging only
#define  DEBUG_3                        0      // for debugging only
#define  DEBUG_4                        0      // for debugging only


// *------------------------------------------------------
// * disable line info, to make code image independent from
// * adding or removing comment lines
// *------------------------------------------------------
#define  DISABLE_LINE_INFO          0      // 0: line info, 1: no line info, to keep image unchanged
#if (DISABLE_LINE_INFO)
	#undef  __LINE__
    #define __LINE__                0      // no line numbers, e.g. to keep image unchanged after including new comment lines
#endif

// *---- circular buffer traces, ONLY FOR DEBUGGING MODE !! ----
#define _DEBUG_LOGGING_CIRC_BUF     0     // NV data post
#define _DEBUG_LOGGING_CIRC_BUF1    0     // System Timer und Task-Timer
#define _DEBUG_LOGGING_CIRC_BUF2    0
#define _DEBUG_LOGGING_CIRC_BUF3    0
#define _DEBUG_LOGGING_CIRC_BUF4    0
#define _DEBUG_LOGGING_CIRC_BUF_FSU 0




/*-------------------------------------------------------*/
/*              1.  TRACE Settings                       */
/*-------------------------------------------------------*/
// ******* set trace output medium *******
#define PNIO_TRACE_NONE         0       // no LSA trace
#define PNIO_TRACE_DK_MEM       1       // trace output into circular memory and/or RS232 (DEFAULT)
#define PNIO_TRACE_DK_MEMXT     2       // extended trace output into circular memory and/or RS232 (DEFAULT)
#define PNIO_TRACE_DK_LSA       3       // trace output in LSA format to circular memory and/or RS232
#define PNIO_TRACE_DK_CONSOLE   4       // trace output on RS232 console
#define PNIO_TRACE_DK_UDP       5       // reserved
#define PNIO_TRACE_DK_TCP       6       // reserved

//#define PNIO_TRACE                PNIO_TRACE_DK_LSA
#define PNIO_TRACE                 PNIO_TRACE_NONE	// PNIO_TRACE_DK_MEMXT  // we should aspire to MEMXT for DK 4.4, because realtime performance is bettera
#define PNIO_TRACE_BUFFER_SIZE      1000

#if (PNIO_TRACE != PNIO_TRACE_NONE)
// For minimum side-affect of trace on real-time performance,
// default value is set to PNIO_LOG_WARNING_HIGH. It could be higher to increase performance of trace versus sacrifice of
// realtime performance or getting risks such as loss of AR.
#define PNIO_TRACE_COMPILE_LEVEL  PNIO_LOG_WARNING_HIGH
#else
#define PNIO_TRACE_COMPILE_LEVEL PNIO_LOG_DEACTIVATED
#endif
// *-------------------------------------------------
// *    LOGGING over UDP
// *-------------------------------------------------
//#define INCLUDE_LOGADAPT       1


// *------------------------------------------------------
// * fixes for the current version
// *------------------------------------------------------

// *------------------------------------------------------
// * include compiler2.h at the end of this file
// *------------------------------------------------------
#include "compiler2.h"      // don't change
#include "iod_cfg.h"      // don't change


#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif
 
/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
