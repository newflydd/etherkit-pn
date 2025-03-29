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
/*  F i l e               &F: tskma_inc.h                               :F&  */
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
 * tskma_inc.h
 *
 *  Created on: 12.07.2012
 *      Author: cn3dit09
 */

#ifndef TSKMA_INC_H_
#define TSKMA_INC_H_

/*- include the global system header ----------------------------------------*/
#include "glob_sys.h"
#include "dk_types.h"

/*- include the trace header ------------------------------------------------*/
#include "trace_cfg.h"
#include "lsas_cfg.h"
#include "cm_cfg.h"

/*- include the lower-layer header ------------------------------------------*/
#include "clrpc_inc.h"
#include "cm_usr.h"
#include "cm_sys.h"

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200_400)     /* for EDDI use this */
    #include "edd_cfg.h"
    #include "edd_usr.h"
    #include "eddi_cfg.h"
    #include "eddi_irte.h"
    #include "eddi_usr.h"
    #include "eddi_sys.h"
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)      /* for EDDP use this */
    #include "edd_cfg.h"
    #include "edd_usr.h"
    #include "eddp_cfg.h"
    #include "eddp_usr.h"
    #include "eddp_sys.h"
	#include "gdma_cfg.h"
	#include "gdma_com.h"
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_DKSW)        /* for standard ethernet controller use this */
    #include "edd_cfg.h"
    #include "edd_usr.h"
    #include "edd_inc.h"
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2_EDDP)      /* for EDDP use this */
    #include "edd_cfg.h"
    #include "edd_usr.h"
    #include "eddp_cfg.h"
    #include "eddp_usr.h"
    #include "eddp_sys.h"
	#include "gdma_cfg.h"
	#include "gdma_com.h"
#else
    #error ("no valid platform selected")
#endif

#include "lsas_com.h"
#include "lsas_usr.h"
#include "lsas_path_com.h"

#include "pndv_cfg.h"
#include "pndv_com.h"

#include "interface.h"

#include "pnpb_cfg.h"
#include "pnpb_com.h"

#include "pniousrd.h"
#include "nv_data.h"

#include "iom_cfg.h"
#include "iom_com.h"

#include "os_taskprio.h"

#define LTRC_ACT_MODUL_ID TSKMA_MODULE_ID

#endif /* TSKMA_INC_H_ */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
