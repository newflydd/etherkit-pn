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
/*  F i l e               &F: lsas_inc.h                                :F&  */
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


/**
 * @file    lsas_inc.h
 * @brief   brief description
 * @author  cn3dit09
 * @version
 * @date    24.02.2012
 *
 * long description
 *
 */

/*****************************************************************************/

#ifndef LSAS_INC_H_
#define LSAS_INC_H_

/* suppressed warning 0236: controlling expression is constant */
//#pragma ghs nowarning 0236

/* basic lsa settings */
#include "dk_types.h"
#include "glob_sys.h"
#include "lsas_cfg.h"
#include "pniousrd.h"
#include "bspadapt.h"

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200_400) /* EDDI */
    /* processor register access (timer, port, ...) */
    #include "ertec200.h"
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P) /* for EDDP use this */
    #include "arm926.h"
    #include "ertec200p_reg.h"
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2_EDDP) /* for EDDP use this */
    #include "arm926.h"
    #include "pnip_reg.h"
#endif

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB400) /* EDDI */
    #include "ertec_x00.h"
#endif

#include "trace_cfg.h"
#include "ltrc_if.h"

#include "edd_cfg.h"
#include "edd_usr.h"
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200_400)         /* for EDDI use this */
    #include "eddi_cfg.h"
    #include "eddi_irte.h"
    #include "eddi_usr.h"
    #include "eddi_sys.h"
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P) /* for EDDP use this */
    #include "eddp_cfg.h"
    #include "eddp_usr.h"
    #include "eddp_sys.h"
	#include "evma_cfg.h"
	#include "evma_com.h"
	#include "gdma_cfg.h"
	#include "gdma_com.h"
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_DKSW) /* for EDDS use this */

    #include "edds_cfg.h"
    #include "edds_usr.h"
    #include "edds_iobuf_usr.h"
    #include "edds_sys.h"
    #include "edds_nrt_inc.h"
    #include "edds_srt_inc.h"
	#include "edds_dev.h"
	#include "edds_llif.h"
#ifndef LSAS_EDDS_ENTER_CRITICAL
 #define LSAS_EDDS_ENTER_CRITICAL()
#endif

#ifndef LSAS_EDDS_EXIT_CRITICAL
 #define LSAS_EDDS_EXIT_CRITICAL()
#endif
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2_EDDP) /* for EDDP use this */
    #include "eddp_cfg.h"
    #include "eddp_usr.h"
    #include "eddp_sys.h"
	#include "evma_cfg.h"
	#include "evma_com.h"
	#include "gdma_cfg.h"
	#include "gdma_com.h"
#else
    #error ("no valid platform selected")
#endif
#include "tcip_cfg.h"
#include "tcip_usr.h"
#include "tcip_sys.h"
#include "tcip_low.h"
#include "sock_cfg.h"
#include "sock_usr.h"
#include "sock_sys.h"
#include "pof_cfg.h"
#include "pof_usr.h"
#include "pof_low.h"
#include "pof_sys.h"
#include "mrp_cfg.h"
#include "mrp_sys.h"
#include "mrp_usr.h"
#include "lldp_cfg.h"
#include "lldp_usr.h"
#include "lldp_sys.h"
#include "lldp_low.h"
#include "dcp_cfg.h"
#include "dcp_usr.h"
#include "dcp_sys.h"
#include "dcp_low.h"
#include "nare_cfg.h"
#include "nare_usr.h"
#include "nare_sys.h"
#include "nare_low.h"
#include "oha_cfg.h"
#include "oha_usr.h"
#include "oha_sys.h"
#include "oha_low.h"
#include "oha_trc.h"
#include "gsy_cfg.h"
#include "gsy_usr.h"
#include "gsy_sys.h"
#include "gsy_low.h"

#include "clrpc_cfg.h"
#include "clrpc_uuid.h"
#include "clrpc_usr.h"
#include "clrpc_sys.h"
#include "clrpc_low.h"
#include "acp_cfg.h"
#include "acp_usr.h"
#include "acp_sys.h"
#include "acp_low.h"
#include "cm_cfg.h"
#include "cm_usr.h"
#include "cm_sys.h"
#include "cm_low.h"

#include "iom_cfg.h"
#include "iom_com.h"

#if 0
    #include "st_cfg.h"
    #include "st_com.h"

    #include "fs_cfg.h"
    #include "fs_com.h"
    #include "fwup_cfg.h"

    #include "trace_cfg.h"
    #include "led_cfg.h"
#endif
#include "pndv_cfg.h"
#include "pndv_com.h"
#include "interface.h"
#if 0
    #include "..\..\1_boot\src\_cfg\boot_cfg.h"
    #include "boot_com.h"
#endif

#include "iod_cfg1.h"


#include "lsas_com.h"
#include "lsas_usr.h"
#include "lsas_path_com.h"

#ifndef LTRC_ACT_MODUL_ID
	#define LTRC_ACT_MODUL_ID  LSAS_MODULE_ID   /* LSAS_MODULE_ID has to be unique for each xxx.c that includes lsas_inc.h file */
#endif

#endif /* LSAS_INC_H_ */

/*** end of file *************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
