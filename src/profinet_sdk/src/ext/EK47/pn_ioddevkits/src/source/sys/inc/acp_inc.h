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
/*  F i l e               &F: acp_inc.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  The main include file for ACP                                            */
/*                                                                           */
/*  Includes of acp and of the different systems, compilers or               */
/*  operating system.                                                        */
/*                                                                           */
/*  This file has to be overwritten during system integration, because       */
/*  some includes depend on the different system, compiler or                */
/*  operating system.                                                        */
/*                                                                           */
/*****************************************************************************/
#ifndef ACP_INC_H
#define ACP_INC_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif


/*- include the global system header ----------------------------------------*/

#include "glob_sys.h"

/*- include the trace header ------------------------------------------------*/

#include "ltrc_if.h"

/*- include the lower-layer header ------------------------------------------*/

#include "edd_inc.h"


/*- include the ACP header --------------------------------------------------*/

#include "acp_cfg.h"

#include "acp_usr.h"
#include "acp_sys.h"
#include "acp_low.h"
#include "acp_pls.h"

/*- include the lower-layer header ------------------------------------------*/

#include "lsas_cfg.h"
#include "lsas_com.h"
#include "lsas_path_com.h"

#include "os.h"



#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif  /* of ACP_INC_H */


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
