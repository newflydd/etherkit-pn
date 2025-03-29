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
/*  F i l e               &F: iom_inc.h                                 :F&  */
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

#ifndef IOM_INC_H_
#define IOM_INC_H_

#include "dk_types.h"
#include "glob_sys.h"
#include "lsas_cfg.h"

#include "iom_cfg.h"
#include "edd_cfg.h"
#include "cm_cfg.h"
#include "trace_cfg.h"
#include "pndv_cfg.h"

#include "clrpc_inc.h"

#include "edd_usr.h"
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200_400)         /* for EDDI use this */
    #include "eddi_cfg.h"
    //#include "eddi_irte.h"
    #include "eddi_usr.h"
    //#include "eddi_sys.h"

#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P) /* for EDDP use this */

	#include "arm926.h"
	#include "hama_com.h"
	#include "ertec200p_reg.h"
    #include "bspadapt.h"

	#include "eddp_cfg.h"
    #include "eddp_usr.h"
    #include "eddp_sys.h"
	#include "gdma_cfg.h"
	#include "gdma_com.h"

#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_DKSW) /* for EDDS use this */
    #include "edds_cfg.h"
	#include "edds_usr.h"
    #include "edds_iobuf_usr.h"
    #include "edds_sys.h"
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2_EDDP) /* for EDDP use this */

	#include "arm926.h"
	//#include "hama_com.h"
	//#include "ertec200p_reg.h"
    #include "bspadapt.h"

	#include "eddp_cfg.h"
    #include "eddp_usr.h"
    #include "eddp_sys.h"
	#include "gdma_cfg.h"
	#include "gdma_com.h"
#else
    #error ("no valid platform selected")
#endif

#include "cm_usr.h" /* needs the pointers with attributes */
#include "cm_sys.h"


#include "pndv_com.h"

#include "iom_com.h"


#endif /* IOM_INC_H_ */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
