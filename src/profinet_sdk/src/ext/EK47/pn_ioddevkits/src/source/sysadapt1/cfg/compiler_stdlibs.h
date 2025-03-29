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
/*  F i l e               &F: compiler_stdlibs.h                        :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  platform dependent standard include files                                */
/*                                                                           */
/*****************************************************************************/
#ifndef _COMPILER_STDLIBS_H
#define _COMPILER_STDLIBS_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

#ifdef NULL
    #undef NULL
#endif

    // *--------------------------------------------------*
    // *   TOOL variants
    // *--------------------------------------------------*


// *-------------------------------------------------------------------------*
// *   TOOL CHAIN ECOS, GNU Tools
// *-------------------------------------------------------------------------*
#if (PNIOD_PLATFORM & (PNIOD_PLATFORM_ECOS_EB200_400 | PNIOD_PLATFORM_ECOS_EB200P))
  // *----------------------------
  // * include standard files
  // *----------------------------
  #include "ecos_ertec_os_utils.h"

// *-------------------------------------------------------------------------*
// *   TOOL CHAIN generic
// *-------------------------------------------------------------------------*
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_GENERIC_DKSW)
  // *----------------------------
  // * include standard files
  // *----------------------------


// *-------------------------------------------------------------------------*
// *   TOOL CHAIN generic
// *-------------------------------------------------------------------------*
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_DKSW_ATMEL)
  // *----------------------------
  // * include standard files
  // *----------------------------


// *-------------------------------------------------------------------------*
// *   TOOL CHAIN GNU_ARM   (only an example, not tested !!)
// *-------------------------------------------------------------------------*
#elif (PNIOD_PLATFORM &  PNIOD_PLATFORM_NETOS_NS9215)

  // *----------------------------
  // * include standard files
  // *----------------------------
  #include <stdlib.h>
  #include <tx_api.h>

  //#include <snmpapi.h>
  #include <sysAccess.h>
  #include <nastatus.h>
  #include <math.h>

// *-------------------------------------------------------------------------*
// *   TOOL CHAIN MICROSOFT   (only an example, not tested !!)
// *-------------------------------------------------------------------------*
#elif (PNIOD_PLATFORM &  PNIOD_PLATFORM_WIN)
   #include <stdlib.h>
   #include <stdio.h>
   #include <string.h>
   #include <sys/timeb.h>
   #include <sys/types.h>
   #include <time.h>
   #include <process.h>
   #include <conio.h>
   #include <malloc.h>
   #include <conio.h>
   #include <windows.h>
   #include <math.h>
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_POSIX_EB200P )
   #include <pkgconf/posix.h>
   #include "ecos_ertec_os_utils.h"
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_LINUX_PC)
   #include "linux_dksw_os_utils.h"
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2)
  // #include "rtos_utils.h"
	#include <stdlib.h>
   #include <stdio.h>
   #include <string.h>
  #include <stdarg.h>
#else
    #error "platform not defined\n"
#endif

    #ifdef HAVE_STDIO_H
        #include <stdio.h>
    #endif
    #ifdef HAVE_STDLIB_H
        #include <stdlib.h>
    #endif
    #ifdef HAVE_STDINT_H
        #include <stdint.h>
    #endif
    #ifdef HAVE_STDDEF_H
        #include <stddef.h>
    #endif
    #ifdef HAVE_STDARG_H
        #include <stdarg.h>
    #endif
    #ifdef HAVE_STRING_H
        #include <string.h>
    #endif
    #ifdef HAVE_MATH_H
        #include <math.h>
    #endif

#ifdef INCLUDE_POSIX_INTERFACE
    #ifdef HAVE_SYS_STAT_H
        #include <sys/stat.h>
    #endif
    #ifdef HAVE_SYS_TYPES_H
        #include <sys/types.h>
    #endif
    #ifdef HAVE_FCNTL_H
        #include <fcntl.h>
    #endif
    #ifdef HAVE_ARPA_INET_H
        #include <arpa/inet.h>
    #endif
    #ifdef HAVE_PTHREAD_H
        #include <pthread.h>
    #endif
    #ifdef HAVE_SIGNAL_H
        #include <signal.h>
    #endif
    #ifdef HAVE_SEMAPHORE_H
        #include <semaphore.h>
    #endif
    #ifdef HAVE_INTTYPES_H
        #include <inttypes.h>
    #endif
    #ifdef HAVE_MQUEUES_H
        #include <mqueue.h>
    #endif
    #ifdef HAVE_ERRNO_H
        #include <errno.h>
    #endif
#endif

#ifndef NULL
    #define  NULL   (void*)0;
#endif


#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif
 
/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
