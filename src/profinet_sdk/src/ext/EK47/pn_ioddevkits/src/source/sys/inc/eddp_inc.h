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
/*  F i l e               &F: eddp_inc.h                                 :F&  */
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
/*  Includes of EDDP and of the different systems, compilers or              */
/*  operating system.                                                        */
/*                                                                           */
/*  This file has to be overwritten during system integration, because       */
/*  some includes depend on the different system, compiler or                */
/*  operating system.                                                        */
/*                                                                           */
/*****************************************************************************/

#ifndef EDDP_INC_H                     /* ----- reinclude-protection ----- */
#define EDDP_INC_H

/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  13.08.08    JS    initial version.                                       */
/*  04.10.10    JS    added eddp_pls.h                                       */
#endif
/*****************************************************************************/


/*-inlcude your global systsem header here ----------------------------------*/

/* the following LSA-headers or headers including these headers have to      */
/* be included here.                                                         */

/* LSA Includes
*/
#include "glob_sys.h"

//#include "arm926.h"
//#include "ertec200p_reg.h"
#include "lsas_cfg.h"

/*---------------------------------------------------------------------------*/

#include "edd_cfg.h"
#include "edd_usr.h"
#include "eddp_cfg.h"
#include "eddp_usr.h"
#include "eddp_sys.h"

#if EDDP_CFG_TRACE_MODE == 1
    #include "ltrc_if.h"
#endif


/*-include your output-macro-header here ------------------------------------*/


/*---------------------------------------------------------------------------*/

#include "eddp_out.h"
#include "eddp_pls.h"

/*- include system embedding layer ------------------------------------------*/
/* lsas */

#include "lsas_com.h"
#include "lsas_path_com.h"


/*****************************************************************************/
/*  end of file EDDP_INC.H                                                    */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/

#endif  /* of EDDP_INC_H */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
