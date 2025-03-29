
#ifndef EDDP_PLS_H                       /* ----- reinclude-protection ----- */
#define EDDP_PLS_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

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
/*  C o m p o n e n t     &C: EDDP (EthernetDeviceDriver for PN-IP)     :C&  */
/*                                                                           */
/*  F i l e               &F: eddp_pls.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Check the configuration                                                  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Version        Who  What                                     */
#ifdef EDDP_MESSAGE
/*  04.10.10    JS    initial version.                                       */
#endif
/*                                                                           */
/*****************************************************************************/

                

/*===========================================================================*/
/*                              compiler errors                              */
/*===========================================================================*/


#if !defined EDD_UPPER_IN_FCT_ATTR
#error "EDD_UPPER_IN_FCT_ATTR not defined in file edd_cfg.h"
#endif

#if !defined EDD_UPPER_OUT_FCT_ATTR
#error "EDD_UPPER_OUT_FCT_ATTR not defined in file edd_cfg.h"
#endif

#if !defined EDDP_SYSTEM_IN_FCT_ATTR
#error "EDD_SYSTEM_IN_FCT_ATTR not defined in file edd_cfg.h"
#endif

#if !defined EDDP_SYSTEM_OUT_FCT_ATTR
#error "EDD_SYSTEM_OUT_FCT_ATTR not defined in file edd_cfg.h"
#endif

#if !defined EDDP_LOCAL_FCT_ATTR
#error "EDDP_LOCAL_FCT_ATTR not defined in file edd_cfg.h"
#endif

#if !defined EDD_UPPER_RQB_ATTR
#error "EDD_UPPER_RQB_ATTR not defined in file edd_cfg.h"
#endif

#if !defined EDD_UPPER_MEM_ATTR
#error "EDD_UPPER_MEM_ATTR not defined in file edd_cfg.h"
#endif

#if !defined EDDP_LOWER_MEM_ATTR
#error "EDDP_LOWER_MEM_ATTR not defined in file edd_cfg.h"
#endif

#if !defined EDDP_SYSTEM_MEM_ATTR
#error "EDD_SYSTEM_MEM_ATTR not defined in file edd_cfg.h"
#endif

#if !defined EDDP_LOCAL_MEM_ATTR
#error "EDDP_LOCAL_MEM_ATTR not defined in file edd_cfg.h"
#endif

#if !defined EDD_COMMON_MEM_ATTR
#error "EDD_COMMON_MEM_ATTR not defined in file edd_cfg.h"
#endif

#if !defined LSA_SYS_PATH_TYPE
#error "LSA_SYS_PATH_TYPE not defined in file lsa_cfg.h"
#endif

#if !defined LSA_SYS_PTR_TYPE
#error "LSA_SYS_PTR_TYPE not defined in file lsa_cfg.h"
#endif

#if (EDD_FRAME_BUFFER_LENGTH < EDDP_RX_FRAME_BUFFER_LENGTH)
#error "EDD_FRAME_BUFFER_LENGTH < EDDP_RX_FRAME_BUFFER_LENGTH"
#endif

#if ( (!defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT)) && (!defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)))
#error "no PNIP-Revision is defined in file eddp_cfg.h"
#endif

   
#if ( (defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)) && ( !( (defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT)) || (defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)) ) )  )
#error "no PNIP-Revision (Rev1 or Rev2) is defined for Ertec200P in file eddp_cfg.h"
#endif    
    
// It is not allowed to change the EDDP_MAX_PORT_CNT to a value <> 4 
// because of internal structure of CMD-interface and CRT must be adapted.
#if (EDDP_MAX_PORT_CNT != 4)
#error "EDDP_MAX_PORT_CNT != 4"
#endif

// EDD_CFG_MAX_PORT_CNT used for EDD- API calls and must be greater or equal to EDDP_MAX_PORT_CNT!!
// EDDP_MAX_PORT_CNT used for Internal EDDP structures.
#ifdef EDDP_CFG_HW_ERTEC200P_SUPPORT
#if (EDD_CFG_MAX_PORT_CNT < EDDP_MAX_PORT_CNT_ERTEC200P)
#error "EDD_CFG_MAX_PORT_CNT < EDDP_MAX_PORT_CNT_ERTEC200P"
#endif
#endif 

//  EDDP_CFG_USR_CMD_TIMEOUT_TICKS_SYNCHRONOUS_1US must be greater than zero !!
#if (defined(EDDP_CFG_USR_CMD_API_INCLUDE) && (EDDP_CFG_USR_CMD_TIMEOUT_TICKS_SYNCHRONOUS_1US == 0))
#error "EDDP_CFG_USR_CMD_TIMEOUT_TICKS_SYNCHRONOUS_1US = 0, it must be greater than zero!"
#endif

#if (!defined(EDDP_CFG_HW_ERTEC200P_SUPPORT))
    #error   "at least EDDP_CFG_HW_ERTEC200P_SUPPORT should be set"
#endif


#if defined(EDDP_CFG_HW_HERA_SUPPORT)
#error "EDDP_CFG_HW_HERA_SUPPORT is no longer supported"
#endif

#if defined(EDDP_CFG_HW_PNIP_REV3_SUPPORT)
#error "EDDP_CFG_HW_PNIP_REV3_SUPPORT is no longer supported"
#endif

    
/*****************************************************************************/
/*  end of file EDDP_PLS.H                                                   */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_PLS_H */
