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
/*  F i l e               &F: gsy_inc.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*                      Frame for file "gsy_inc.h".                          */
/*                      ==============================                       */
/*                                                                           */
/*  Include-File:                                                            */
/*  Includes of prefix and of the different systems, compilers or            */
/*  operating system.                                                        */
/*                                                                           */
/*  This file has to be overwritten during system integration, because       */
/*  some includes depend on the different system, compiler or                */
/*  operating system.                                                        */
/*                                                                           */
/*****************************************************************************/

#ifndef GSY_INC_H                        /* ----- reinclude-protection ----- */
#define GSY_INC_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Version                  Who  What                           */
/*  2004-11-29  P01.00.00.00_00.01.00.00 lrg  from LSA_GLOB P2.1.0.0_0.4.1.1 */
/*  2005-06-02  P03.06.00.00_00.03.00.01 lrg  Prototype for PT2              */
/*                                                                           */
/*****************************************************************************/


/* general system definitions */

#include "glob_sys.h"

// * only if LTRC is used for trace
#include "ltrc_if.h"

#include "lsas_cfg.h"

#include "edd_cfg.h"
#include "edd_usr.h"

#ifdef GSY_INCLUDE_FOR_KRISC32
#include "eddp_k32_cfg.h"
#include "clp_inc.h"
#endif
#include "gsy_cfg.h"
#include "gsy_trc.h"
#include "gsy_low.h"
#include "gsy_usr.h"
#include "gsy_sys.h"
#include "gsy_pls.h"

/*- include system embedding layer ------------------------------------------*/
/* lsas */

#include "lsas_com.h"
#include "lsas_path_com.h"

/*****************************************************************************/
/*  end of file GSY_INC.H                                                    */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif
/*---------------------------------------------------------------------------*/
#endif  /* of GSY_INC_H */


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
