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
/*  F i l e               &F: trace_lsa_dat.c                           :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  internal data.                                                           */
/*                                                                           */
/*****************************************************************************/



/*****************************************************************************/
/* contents:

    - trace_dat_dummy

*/

/* module ID */
#define TRACE_MODULE_ID    141
#define LTRC_ACT_MODUL_ID  141


/*****************************************************************************/
/* include hierarchy */

#include "pniobase.h"
#if (PNIO_TRACE == PNIO_TRACE_DK_LSA)
    #include "version_dk.h"
    #include "trace_dk.h"

    #include "trace_lsa_inc.h"

    #define TRACE_EXTERN_ATTR

    #include "trace_lsa_com.h"
    #include "trace_lsa_dat.h"

    /*****************************************************************************/
    /* dummy function */
    void trace_dat_dummy(void)
    {
    }
#endif

/*** end of file *************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
