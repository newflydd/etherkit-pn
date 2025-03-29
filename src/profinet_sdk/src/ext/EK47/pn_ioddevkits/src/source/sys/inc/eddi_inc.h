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
/*  F i l e               &F: eddi_inc.h                                :F&  */
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
/*  Includes of Edd and of the different systems, compilers or               */
/*  operating system.                                                        */
/*                                                                           */
/*  This file has to be overwritten during system integration, because       */
/*  some includes depend on the different system, compiler or                */
/*  operating system.                                                        */
/*                                                                           */
/*****************************************************************************/
#ifndef EDDI_INC_H           //reinclude-protection
#define EDDI_INC_H

#ifdef __cplusplus           //If C++ - compiler: Use C linkage
extern "C"
{
#endif

/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
/*  12.04.02    JS    initial version. LSA P02.00.01.003                     */
/*  23.05.02    JS    ported to LSA_GLOB_P02.00.00.00_00.01.04.01            */
/*                                                                           */
/*****************************************************************************/

/* include your global system headers here ----------------------------------*/

/* the following LSA-headers or headers including these headers have to      */
/* be included here.                                                         */
//#include <stdlib.h>
//#include <memory.h>

#include "glob_sys.h"
#include "lsas_cfg.h"
/*---------------------------------------------------------------------------*/
#ifdef LSAS_CFG_USE_EDDI
#include "edd_cfg.h"
#include "eddi_cfg.h"
//lint -save -e762  -e962 -e760  Redundantly declared symbol, Macro
#include "edd_usr.h"
#include "eddi_usr.h"
//lint -restore

#if defined (EDDI_CFG_TRACE_MODE) && ((EDDI_CFG_TRACE_MODE == 1) || (EDDI_CFG_TRACE_MODE == 2)) //LSA-Trace

//#include "sys_inc.h"
#include "ltrc_if.h" //only if LTRC is used for trace

#endif

#include "eddi_sys.h"
#include "eddi_irte.h"
#include "eddi_trc.h"

/*-include your output-macro-header here ------------------------------------*/

//lint -save -e762  -e962 -e760  Redundantly declared symbol, Macro
//#include "sys_out.h"

#include "eddi_out.h"
//lint -restore

#include "eddi_pls.h"

#include "eddi_err.h"

/* lsas */

#include "lsas_com.h"
#include "lsas_path_com.h"

/*---------------------------------------------------------------------------*/

//lint -esym(755, SYS_PATH*)

/*---------------------------------------------------------------------------*/
/* for EDDI_GET_PATH_INFO, variable path                                     */
/*---------------------------------------------------------------------------*/
#define SYS_PATH_GET_CP_NR(sys_path)        (sys_path >> 12)
#define SYS_PATH_SET_CP_NR(sys_path, cp_nr) ((LSA_UINT16)((cp_nr << 12) | sys_path))

#define SYS_PATH_NRT_CHA_IF_0       0x0000    /* receive all frames */
#define SYS_PATH_NRT_CHA_IF_1       0x0001    /* receive all frames */
#define SYS_PATH_NRT_CHB_IF_0       0x0002    /* receive all frames */
#define SYS_PATH_NRT_CHB_IF_1       0x0003    /* receive all frames */
#define SYS_PATH_CRT                0x0004

#define SYS_PATH_NRT_CHB_IF_0_FT    0x0005    /* use frametrigger unit in REV 6, ... */

#define SYS_PATH_SWI_0              0x000d
#define SYS_PATH_SWI_1              0x000e
#define SYS_PATH_SWI_2              0x000f
#define SYS_PATH_SWI_3              0x0010

// SYNC Path (used by GSY)
#define SYS_PATH_SYNC                              0x0011   /* Note: Use with GSY! (includes PTCP) */

// NRT Filter
#define SYS_PATH_NRT_CHA_IF_0_FRAME_LEN_STDBY      0x0013
#define SYS_PATH_NRT_CHA_IF_0_FRAME_ASRT           0x0014
#define SYS_PATH_NRT_CHA_IF_0_FRAME_IP             0x0015
#define SYS_PATH_NRT_CHA_IF_0_FRAME_DCP            0x0016
#define SYS_PATH_NRT_CHA_IF_0_FRAME_PTCP_ANNO      0x0017   /* Note: Only for test! */
#define SYS_PATH_NRT_CHA_IF_0_FRAME_LLDP           0x0018   /* Note: Only for test! */
#define SYS_PATH_NRT_CHA_IF_0_FRAME_LEN_DCP        0x0019
#define SYS_PATH_NRT_CHA_IF_0_FRAME_LEN_TSYNC      0x001a
#define SYS_PATH_NRT_CHA_IF_0_FRAME_LEN_OTHER      0x001b
#define SYS_PATH_NRT_CHA_IF_0_FRAME_OTHER          0x001c
#define SYS_PATH_NRT_CHA_IF_0_FRAME_UDP_ASRT       0x001d
#define SYS_PATH_NRT_CHA_IF_0_FRAME_UDP_DCP        0x001e
#define SYS_PATH_NRT_CHA_IF_0_FRAME_KOMBI          0x001f

#define SYS_PATH_NRT_CHA_IF_1_FRAME_ASRT           0x0020
#define SYS_PATH_NRT_CHA_IF_1_FRAME_IP             0x0021
#define SYS_PATH_NRT_CHA_IF_1_FRAME_DCP            0x0022
#define SYS_PATH_NRT_CHA_IF_1_FRAME_PTCP_ANNO      0x0023

#define SYS_PATH_NRT_CHA_IF_0_FRAME_MRP            0x0024

#define SYS_PATH_NRT_CHA_IF_0_FRAME_DCP_HELLO      0x0026
#define SYS_PATH_NRT_CHA_IF_0_FRAME_UDP_DCP_HELLO  0x0027
#define SYS_PATH_NRT_CHA_IF_0_FRAME_LEN_HSR        0x0028

#define SYS_PATH_NRT_CHA_IF_0_FRAME_IP_ICMP        0x0080
#define SYS_PATH_NRT_CHA_IF_0_FRAME_IP_IGMP        0x0081
#define SYS_PATH_NRT_CHA_IF_0_FRAME_IP_TCP         0x0082
#define SYS_PATH_NRT_CHA_IF_0_FRAME_IP_UDP         0x0083
#define SYS_PATH_NRT_CHA_IF_0_FRAME_IP_OTHER       0x0084
#define SYS_PATH_NRT_CHA_IF_0_FRAME_ARP            0x0085
#define SYS_PATH_NRT_CHA_IF_0_FRAME_RARP           0x0086
#define SYS_PATH_NRT_CHA_IF_0_FRAME_IP_ALL         0x0087

#define SYS_PATH_NRT_CHB_IF_0_FRAME_LLDP           0x0088   /* Use with LLDP! */

/* PRM */
#define SYS_PATH_PRM                               0x0025

/* Only for test! */
#define SYS_PATH_NRT_CHB_IF_0_FRAME_ASRT           0x0030
#define SYS_PATH_NRT_CHB_IF_0_FRAME_IP             0x0031
#define SYS_PATH_NRT_CHB_IF_0_FRAME_DCP            0x0032
#define SYS_PATH_NRT_CHB_IF_0_FRAME_PTCP_SYNC      0x0033

/* Only for test! */
#define SYS_PATH_NRT_CHB_IF_1_FRAME_ASRT           0x0040
#define SYS_PATH_NRT_CHB_IF_1_FRAME_IP             0x0041
#define SYS_PATH_NRT_CHB_IF_1_FRAME_DCP            0x0042
#define SYS_PATH_NRT_CHB_IF_1_FRAME_PTP            0x0043

#define SYS_PATH_NRT_CHB_IF_0_FRAME_MONITOR        0x0050

#define SYS_PATH_NRT_CHA_IF_0_FRAME_NULL           0x0060
#define SYS_PATH_NRT_CHA_IF_1_FRAME_NULL           0x0061
#define SYS_PATH_NRT_CHB_IF_0_FRAME_NULL           0x0062
#define SYS_PATH_NRT_CHB_IF_1_FRAME_NULL           0x0063

#define SYS_PATH_GENERAL                           0x0070

//#include "lsas_path.h"
#endif
/*---------------------------------------------------------------------------*/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif //EDDI_INC_H


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
