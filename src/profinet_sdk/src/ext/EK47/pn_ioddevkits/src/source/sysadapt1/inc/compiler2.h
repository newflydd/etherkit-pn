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
/*  F i l e               &F: compiler2.h                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  compiler dependent definitions, for development                          */
/*  only, do not change                                                      */
/*  THIS FILE MUST BE INCLUDED AT THE END OF FILE                            */
/*  COMPILER.H                                                               */
/*                                                                           */
/*****************************************************************************/
#ifndef _COMPILER2_H
#define _COMPILER2_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif


// *------------------------------------------------------
// * check for undefined macros...
// *------------------------------------------------------
#ifndef PNIO_BIG_ENDIAN
    #error "PNIO_BIG_ENDIAN must be defined  in file auto_platform_select.h"
#endif

#if (PNIOD_PLATFORM == PNIOD_PLATFORM_NONE)
    #error "PNIO_PLATFORM must be defined in file compiler.h"
#endif



#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif
 
/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
