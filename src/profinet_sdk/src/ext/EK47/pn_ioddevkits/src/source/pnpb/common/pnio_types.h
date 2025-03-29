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
/*  F i l e               &F: pnio_types.h                              :F&  */
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
#ifndef PNIO_TYPES_H
#define PNIO_TYPES_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif


    #include "lsa_types.h"

    typedef     LSA_UINT8      PNIO_UINT8;
    typedef     LSA_UINT16     PNIO_UINT16;
    typedef     LSA_UINT32     PNIO_UINT32;
    typedef     LSA_UINT64     PNIO_UINT64;
    typedef     LSA_INT8       PNIO_INT8;
    typedef     LSA_INT16      PNIO_INT16;
    typedef     LSA_INT32      PNIO_INT32;
    typedef     LSA_BOOL       PNIO_BOOL;
    typedef     LSA_VOID       PNIO_VOID;

    typedef     LSA_INT        PNIO_INT;
    typedef     LSA_UINT       PNIO_UINT;
    typedef     PNIO_UINT32    PNIO_UINTOpt;



#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*** end of file *************************************************************/
#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
