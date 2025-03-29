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
/*  F i l e               &F: dk_types.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  data types.                                                              */
/*                                                                           */
/*****************************************************************************/



/*****************************************************************************/
/* reinclude protection */


#ifndef TYPES_H
#define TYPES_H


/*****************************************************************************/
/* basic data types */


typedef unsigned    long                    UnsignedOpt;

typedef unsigned    char                    Unsigned8;

typedef unsigned    short                   Unsigned16;

typedef unsigned    long                    Unsigned32;

typedef signed      char                    Signed8;

typedef signed      short                   Signed16;

typedef signed      long                    Signed32;

typedef UnsignedOpt                         Boolean;

#ifndef __cplusplus
//typedef UnsignedOpt                         bool;
#endif

#define bool_bit   UnsignedOpt

#ifndef FALSE
#define FALSE                               (0)
#endif
#ifndef TRUE
#define TRUE                                (!FALSE)
#endif

#ifndef NIL
#define NIL                                 (0)
#endif


/*****************************************************************************/
/* reinclude-protection */


#else
    #pragma message ("The header TYPES.H is included twice or more !")
#endif


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
