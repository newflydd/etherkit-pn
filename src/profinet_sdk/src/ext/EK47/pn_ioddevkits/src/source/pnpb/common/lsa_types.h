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
/*  F i l e               &F: lsa_types.h                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  export information.                                                      */
/*                                                                           */
/*****************************************************************************/
#ifndef LSA_TYPES_H
#define LSA_TYPES_H


#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

#include "compiler.h"



    /*===========================================================================*/
    /* basic types for lsa according ANSI C1999 Standard (ISO/IEC 9899:1999)     */
    /*                                                                           */
    /* ANSI C1999: "The header <stdint.h> declares sets of integer types having  */
    /* specified widths."                                                        */
    /* For example: uint8_t, uint16_t, uint32_t, int8_t, int16_t, int32_t        */
    /*===========================================================================*/

    #define LSA_UINT8                   unsigned char
                       /* UNSIGNED-8-Bit:                   0 ...           +255 */
    #define LSA_UINT16                  unsigned short
                       /* UNSIGNED-16-Bit:                  0 ...        +65 535 */
    #define LSA_UINT32                  unsigned long
                       /* UNSIGNED-32-Bit:                  0 ... +4 294 967 295 */

    #define LSA_UINT                    unsigned int
                       /* 16-, 32-, etc. bit according to compiler               */
                       /* minimal range:                    0 ...        +65 535 */

    #define LSA_INT8                    char
                             /* SIGNED-8-Bit:            -128 ...           +127 */
    #define LSA_INT16                   short
                             /* SIGNED-16-Bit:        -32 768 ...        +32 767 */
    #define LSA_INT32                   long
                             /* SIGNED-32-Bit: -2 147 483 648 ... +2 147 483 647 */
    #define LSA_INT                     int

    #define LSA_CHAR                    char


    /* SIGNED-64-Bit:                                                      */



    #if (PNIOD_PLATFORM & PNIOD_PLATFORM_WIN)
        #define LSA_UINT64                  unsigned __int64 /*  0 ... +18 446 744 073 709 551 615 */
        #define LSA_INT64                   __int64          /*  -9 223 372 036 854 775 808 ... 0 ...  +9 223 372 036 854 775 807 */
    #elif (PNIOD_PLATFORM & PNIOD_PLATFORM_GNU)
        #define LSA_UINT64                  unsigned long long
        #define LSA_INT64                   long long
    #elif (PNIOD_PLATFORM & PNIOD_PLATFORM_GENERIC_DKSW)
        #define LSA_UINT64                  unsigned long long
        #define LSA_INT64                   long long
    #elif (PNIOD_PLATFORM & PNIOD_PLATFORM_IAR)
        #define LSA_UINT64                  unsigned long long
        #define LSA_INT64                   long long
    #else
        #error ("platform must support 64 bit data types")
    #endif


    /*===========================================================================*/
    /*                            other types for lsa                            */
    /*===========================================================================*/
    #define LSA_VOID                    void
    #define LSA_VOID_PTR_TYPE           void *
    #define LSA_BOOL                    LSA_UINT32         /* LSA_FALSE, LSA_TRUE */


/*---------------------------------------------------------------------------*/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All Rights Reserved.      */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}

#endif


#endif

/*** end of file *************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
