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
/*  F i l e               &F: pof_inc.h                                 :F&  */
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
/*  Includes of POF and of the different systems, compilers or               */
/*  operating system.                                                        */
/*                                                                           */
/*  This file has to be overwritten during system integration, because       */
/*  some includes depend on the different system, compiler or                */
/*  operating system.                                                        */
/*                                                                           */
/*****************************************************************************/

#ifndef POF_INC_H                     /* ----- reinclude-protection ----- */
#define POF_INC_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif


/*-include your global system header here -----------------------------------*/

/* the following LSA-headers or headers including these headers have to      */
/* be included here.                                                         */

#include "glob_sys.h"

/*---------------------------------------------------------------------------*/
#include "lsas_cfg.h"
/* interface to EDD -------------------------------------------------------- */
#include "edd_cfg.h"
#include "edd_usr.h"

/*---------------------------------------------------------------------------*/

#include "pof_cfg.h"
#include "pof_usr.h"
#include "pof_sys.h"
#include "pof_pck.h"
#include "pof_err.h"

#if defined (POF_CFG_TRACE_MODE) && ((POF_CFG_TRACE_MODE == 1) || (POF_CFG_TRACE_MODE == 2)) //LSA-Trace

//#include "sys_inc.h"
#include "ltrc_if.h" //only if LTRC is used for trace

#endif

/*-include your output-macro-header here ------------------------------------*/

/*---------------------------------------------------------------------------*/

#include "pof_trc.h"
#include "pof_low.h"

/*-include your output-macro-header here ------------------------------------*/


/*---------------------------------------------------------------------------*/
#include "pof_trc.h"
#include "pof_low.h"


/*---------------------------------------------------------------------------*/
/* for EDDI_GET_PATH_INFO, variable path                                      */
/*---------------------------------------------------------------------------*/

/*- include system embedding layer ------------------------------------------*/
/* lsas */

#include "lsas_com.h"
#include "lsas_path_com.h"

/*****************************************************************************/
/*  end of file POF_INC.H                                                    */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif


#endif  /* of POF_INC_H */


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
