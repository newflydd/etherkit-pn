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
/*  F i l e               &F: cm_inc.h                                  :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  This file has to be overwritten during system integration, because       */
/*  some includes depend on the different system, compiler or                */
/*  operating system.                                                        */
/*                                                                           */
/*****************************************************************************/
#ifndef CM_INC_H
#define CM_INC_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*                                                                           */
/*  See cm_ver.h                                                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/

/* start with the basics */

/*- include the global system header ----------------------------------------*/

#include "glob_sys.h"

/*- include the lower-layer header ------------------------------------------*/

#include "lsas_cfg.h"

#include "cm_cfg.h"

#if CM_CFG_TRACE_MODE == 1
#include "ltrc_if.h"
#endif

#include "edd_inc.h"

#if CM_CFG_USE_MRP
#include "mrp_inc.h"
#endif

#if CM_CFG_USE_GSY
#include "gsy_inc.h"
#endif

#if CM_CFG_USE_POF
#include "pof_inc.h"
#endif

#if CM_CFG_USE_NARE
#include "nare_inc.h"
#endif

#include "oha_inc.h"

#include "clrpc_inc.h"

#include "acp_inc.h" /* note: the "minimal PDEV" needs the ACP headers too (alarm things) */


/*- include the CM header ---------------------------------------------------*/


#include "cm_usr.h" /* needs the pointers with attributes */
#include "cm_sys.h"
#include "cm_trc.h"
#include "cm_low.h"
#include "cm_pls.h"

/*- include system embedding layer ------------------------------------------*/
/* lsas */


#include "lsas_com.h"
#include "lsas_path_com.h"


#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif  /* of CM_INC_H */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
