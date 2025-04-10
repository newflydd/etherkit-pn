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
/*  F i l e               &F: pndv_inc.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*
 * pndv_inc.h
 *
 *  Created on: 04.07.2012
 *      Author: cn3dit09
 */

#ifndef PNDV_INC_H_
#define PNDV_INC_H_

/*- include the global system header ----------------------------------------*/

#include "dk_types.h"
#include "glob_sys.h"
#include "usrapp_cfg.h"
#include "iod_cfg.h"

#include "lsas_cfg.h"
#include "lldp_cfg.h"
/*- include the trace header ------------------------------------------------*/
#include "trace_cfg.h"
#include "edd_cfg.h"
#include "edd_usr.h"
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200_400)
    #include "eddi_cfg.h" //von iom ben�tigt!
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)
    #include "eddp_cfg.h"
	#include "gdma_cfg.h"
	#include "gdma_com.h"
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2_EDDP)
    #include "eddp_cfg.h"
	#include "gdma_cfg.h"
	#include "gdma_com.h"
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_DKSW)
    #include "edds_cfg.h"
#else
    #error ("no valid platform selected")
#endif

#include "cm_cfg.h"
#include "oha_cfg.h"

/*- include the lower-layer header ------------------------------------------*/

#include "clrpc_inc.h"

#include "cm_usr.h"
#include "cm_sys.h"
//#include "cm_trc.h"
//#include "cm_low.h"
//#include "cm_pls.h"

//#include "oha_sys.h"
#include "lldp_usr.h"
#include "oha_usr.h"

#include "iom_cfg.h"
#include "iom_com.h"

#include "iod_cfg1.h"

#include "pndv_cfg.h"
#include "pndv_com.h"

/*- include the lower-layer header ------------------------------------------*/

/*- include system embedding layer ------------------------------------------*/

#include "lsas_com.h"
#include "lsas_path_com.h"

#include "os.h"
#include "bspadapt.h"

#endif /* PNDV_INC_H_ */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
