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
/*  F i l e               &F: sock_inc.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Implements system integration of SOCK using the pcPNIO framework.        */
/*                                                                           */
/*****************************************************************************/

#ifndef SOCK_INC_H                     /* ----- reinclude-protection ----- */
#define SOCK_INC_H


#ifdef __cplusplus
extern "C" { /* sock headers don't have it (16.12.2005) */
#endif

#include "glob_sys.h"

#include "sock_cfg.h"
#include "sock_usr.h"
#include "sock_sys.h"
#include "sock_trc.h"

#if SOCK_CFG_INTERNICHE
#include "tcip_inc.h"
#endif

#include "ltrc_if.h"

#include "sock_pls.h"



#if 1 /* pcIOX: no ip-stack defines (FALSE, TRUE, etc.) "above" SOCK */
# if defined (SOCK_MODULE_ID)
#  include "sock_low.h" /* pulls in the ipstack header files */
# else
#if   defined(CLRPC_INC_H)
#elif defined(NARE_INC_H)
#elif defined(OHA_INC_H)
#elif defined(SNMPX_INC_H)
#else
#pragma message("check this: who else needs sock?")
#endif
#define AF_INET 2                   // value see "ip.h", cross-checked in sock_pcpnio_startstop()
#define SOCKIF_AF_INET AF_INET      // or else "clrpc_*.c" does not compile, see sock_usr.h
#endif
#endif

/*- include system embedding layer ------------------------------------------*/
/* lsas */

#include "lsas_cfg.h"
#include "lsas_com.h"
#include "lsas_path_com.h"

#ifdef __cplusplus
}
#endif

/*****************************************************************************/
/*  end of file SOCK_INC.H                                                   */
/*****************************************************************************/

#endif /* of SOCK_INC.H */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
