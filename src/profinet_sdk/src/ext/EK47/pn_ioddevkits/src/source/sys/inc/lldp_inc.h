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
/*  F i l e               &F: lldp_inc.h                                :F&  */
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
/*  Includes of LLDP and of the different systems, compilers or              */
/*  operating system.                                                        */
/*                                                                           */
/*  This file has to be overwritten during system integration, because       */
/*  some includes depend on the different system, compiler or                */
/*  operating system.                                                        */
/*                                                                           */
/*****************************************************************************/

#ifndef LLDP_INC_H                     /* ----- reinclude-protection ----- */
#define LLDP_INC_H

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
/*  05.04.04    JS    initial version.                                       */
/*                                                                           */
/*****************************************************************************/


/*-inlcude your global systsem header here ----------------------------------*/

/* the following LSA-headers or headers including these headers have to      */
/* be included here.                                                         */

#include "glob_sys.h"

/*---------------------------------------------------------------------------*/
#include "lsas_cfg.h"

/* interface to EDD -------------------------------------------------------- */
#include "edd_cfg.h"
#include "edd_usr.h"

/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/

#include "lldp_cfg.h"
#include "lldp_usr.h"
#include "lldp_sys.h"
#include "lldp_pck.h"

#include "lldp_low.h"

/*- include system embedding layer ------------------------------------------*/
/* lsas */


#include "lsas_com.h"
#include "lsas_path_com.h"
#include "ltrc_if.h"


/*****************************************************************************/
/*  end of file LLDP_INC.H                                                    */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif


#endif  /* of LLDP_INC_H */


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
