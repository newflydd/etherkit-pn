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
/*  F i l e               &F: clrpc_inc.h                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  The main include file of CLRPC                                           */
/*                                                                           */
/*  This file has to be overwritten during system integration, because       */
/*  some includes depend on the different system, compiler or                */
/*  operating system.                                                        */
/*                                                                           */
/*****************************************************************************/

#ifndef CLRPC_INC_H
#define CLRPC_INC_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*                                                                           */
/*  See clrpc_ver.h                                                          */
/*                                                                           */
/*---------------------------------------------------------------------------*/

/* start with the basics */

#include "glob_sys.h"

/*- include the lower-layer header ------------------------------------------*/

#include "sock_cfg.h"
#include "sock_usr.h"
#include "sock_sys.h"
#include "sock_trc.h"


/*- include the CLRPC header ------------------------------------------------*/

#include "clrpc_cfg.h"
#include "clrpc_uuid.h"

#include "iod_cfg2.h"	

#include "clrpc_trc.h"
#include "clrpc_usr.h" /* needs the pointers with attributes */
#include "clrpc_sys.h"
#include "clrpc_low.h"
#include "clrpc_pls.h"

/* lsas */

#include "lsas_cfg.h"
#include "lsas_com.h"
#include "lsas_path_com.h"

/* trace */
	
#include "ltrc_if.h"	


#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif  /* of CLRPC_INC_H */
 
/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
