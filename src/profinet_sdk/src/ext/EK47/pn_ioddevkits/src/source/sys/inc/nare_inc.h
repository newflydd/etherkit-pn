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
/*  F i l e               &F: nare_inc.h                                :F&  */
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
/*  Includes of NARE and of the different systems, compilers or              */
/*  operating system.                                                        */
/*                                                                           */
/*  This file has to be overwritten during system integration, because       */
/*  some includes depend on the different system, compiler or                */
/*  operating system.                                                        */
/*                                                                           */
/*****************************************************************************/

#ifndef NARE_INC_H                     /* ----- reinclude-protection ----- */
#define NARE_INC_H


#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
/*  24.06.03    JS    initial version.                                       */
/*  19.12.03    JS    LTRC (LSA-Trace) support.                              */
/*  28.10.03    JS    define "NARE_CFG_NO_SOCK" moved to nare_cfg.txt        */
/*  07.06.06    JS    lsa_usr.h added.                                       */
/*  07.03.07    JS    moved nare_cfg.h above sock headers (because           */
/*                    NARE_CFG_NO_SOCK is defined within nare_cfg.h)         */
/*****************************************************************************/


/*-inlcude your global systsem header here ----------------------------------*/

/* the following LSA-headers or headers including these headers have to      */
/* be included here.                                                         */

#include "glob_sys.h"

/*---------------------------------------------------------------------------*/
#include "lsas_cfg.h"
/* interface to EDD (needed by DCP) ---------------------------------------- */
#include "edd_cfg.h"
#include "edd_usr.h"

/* lower interface to DCP -------------------------------------------------- */
#include "dcp_cfg.h"
#include "dcp_usr.h"
#include "dcp_sys.h"

#include "nare_cfg.h"

#ifndef NARE_CFG_NO_SOCK
/* lower interface to SOCK --------------------------------------------------*/
#include "sock_cfg.h"
#include "sock_usr.h"
#include "sock_sys.h"
#endif

/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/

#include "nare_usr.h"
#include "nare_sys.h"

#if NARE_CFG_TRACE_MODE == 1
#include "ltrc_if.h"
#endif

/*-include your output-macro-header here ------------------------------------*/


/*---------------------------------------------------------------------------*/

#include "nare_out.h"
#include "nare_low.h"

/*- include system embedding layer ------------------------------------------*/
/* lsas */

#include "lsas_com.h"
#include "lsas_path_com.h"

/*****************************************************************************/
/*  end of file NARE_INC.H                                                    */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif


#endif  /* of NARE_INC_H */


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
