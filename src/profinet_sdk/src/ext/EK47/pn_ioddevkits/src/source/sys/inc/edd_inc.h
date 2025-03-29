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
/*  F i l e               &F: edd_inc.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Include-File:                                                            */
/*  Includes of Edd and of the different systems, compilers or               */
/*  operating system.                                                        */
/*                                                                           */
/*  This file has to be overwritten during system integration, because       */
/*  some includes depend on the different system, compiler or                */
/*  operating system.                                                        */
/*                                                                           */
/*****************************************************************************/
#ifndef EDD_INC_H                     /* ----- reinclude-protection ----- */
#define EDD_INC_H

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
/*  12.04.02    JS    initial version. LSA P02.00.01.003                     */
/*  23.05.02    JS    ported to LSA_GLOB_P02.00.00.00_00.01.04.01            */
/*                                                                           */
/*****************************************************************************/

/*-inlcude your global systsem header here ----------------------------------*/

/* the following LSA-headers or headers including these headers have to      */
/* be included here.                                                         */

#include "compiler.h"
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200_400)
    #include "eddi_inc.h"
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)
    #include "eddp_inc.h"
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_DKSW)
    #include "edds_inc.h"
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2_EDDP)
    #include "eddp_inc.h"
#else
    #error ("no valid edd platform selected\n");
#endif

/*****************************************************************************/
/*  end of file EDD_INC.H                                                    */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif  /* of EDD_INC_H */


 
/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
