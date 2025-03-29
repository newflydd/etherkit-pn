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
/*  F i l e               &F: eep_dat.c                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  internal data                                                            */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   H i s t o r y :                                                         */
/*                                                                           */
/*   see /_doc/im155_6pn_hist.doc                                            */
/*                                                                           */
/*****************************************************************************/

/* contents:

    -

*/
/*****************************************************************************/
/* 2do:

    -

*/
/*****************************************************************************/
/* include hierarchy */

#include "compiler.h"
    #if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_DKSW_ATMEL)

    #include "eep_inc.h"

    #define EEP_DAT_EXTERN_ATTR

    #include "eep_dat.h"       /* EEPROM configuration */
#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
