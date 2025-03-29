#ifndef MRP_PLS_H
#define MRP_PLS_H


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
/*  C o m p o n e n t     &C: MRP (Media Redundancy Protocol)           :C&  */
/*                                                                           */
/*  F i l e               &F: mrp_pls.h                                 :F&  */
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
/*  Date        Who   What                                                   */
#ifdef MRP_MESSAGE
/*  2008-01-07  mk    check MRP_MAX_CHANNELS against multi nic configuration.*/
/*  2010-02-16  ds    accelerated MRM link change handling implemented       */
/*  2010-05-25  ds    get rid of some redundant OHA channel checks           */
/*                                                                           */
#endif
/*****************************************************************************/

                
/*===========================================================================*/
/*                              compile errors                               */
/*===========================================================================*/

#if !defined LSA_SYS_PATH_TYPE
#error "LSA_SYS_PATH_TYPE not defined in file lsa_cfg.h"
#endif

#if !defined LSA_SYS_PTR_TYPE
#error "LSA_SYS_PTR_TYPE not defined in file lsa_cfg.h"
#endif

#if !defined MRP_LOCAL_MEM_STATIC && !defined MRP_LOCAL_MEM_DYNAMIC
#error "MRP_LOCAL_MEM_STATIC or MRP_LOCAL_MEM_DYNAMIC is missing in file mrp_cfg.h!"
#endif

#if defined MRP_LOCAL_MEM_STATIC && defined MRP_LOCAL_MEM_DYNAMIC
#error "define MRP_LOCAL_MEM_STATIC or MRP_LOCAL_MEM_DYNAMIC in file mrp_cfg.h!"
#endif

#if defined MRP_LOCAL_MEM_STATIC
#error "define MRP_LOCAL_MEM_STATIC not supported yet!"
#endif

#if !defined MRP_CLEAR_GLOBAL_DATA && !defined MRP_NO_CLEAR_GLOBAL_DATA
#error "MRP_CLEAR_GLOBAL_DATA or MRP_NO_CLEAR_GLOBAL_DATA is missing in file mrp_cfg.h!"
#endif

#if defined MRP_CLEAR_GLOBAL_DATA && defined MRP_NO_CLEAR_GLOBAL_DATA
#error "define MRP_CLEAR_GLOBAL_DATA or MRP_NO_CLEAR_GLOBAL_DATA in file mrp_cfg.h!"
#endif

#if defined MRP_CLEAR_GLOBAL_DATA
#error "define MRP_CLEAR_GLOBAL_DATA not supported yet!"
#endif

#if defined(MRP_CFG_PLUGIN_1_MRM_EXT) && !defined(MRP_CFG_PLUGIN_0_MRM)
#error "define MRP_CFG_PLUGIN_0_MRM if MRP_CFG_PLUGIN_1_MRM_EXT is defined in file mrp_cfg.h!"
#endif

#if !defined(MRP_MAX_NO_OF_INTERFACES)
#error "MRP_MAX_NO_OF_INTERFACES has to be defined in mrp_cfg.h!"
#endif

#if (MRP_MAX_CHANNELS % MRP_MAX_CHANNELS_PER_IF) != 0
#error "define MRP_MAX_CHANNELS in file mrp_sys.h is ambiguous!"
#endif

#if (MRP_MAX_CHANNELS > (MRP_MAX_CHANNELS_PER_IF * MRP_MAX_NO_OF_INTERFACES))
#error "define MRP_MAX_CHANNELS in mrp_sys.h exceeds (MRP_MAX_CHANNELS_PER_IF * MRP_MAX_NO_OF_INTERFACES)!"
#endif

#if (MRP_MAX_CHANNELS < (MRP_MAX_CHANNELS_PER_IF * MRP_MAX_NO_OF_INTERFACES))
#error "define MRP_MAX_CHANNELS in mrp_sys.h deceeds (MRP_MAX_CHANNELS_PER_IF * MRP_MAX_NO_OF_INTERFACES)!"
#endif

#if defined (MRP_CFG_TST_SHORT_TIME) && !defined (MRP_CFG_PLUGIN_0_MRM)
#error "MRP_CFG_TST_SHORT_TIME shall only be set, if MRP_CFG_PLUGIN_0_MRM is set!!!"
#endif



/*****************************************************************************/
/*  end of file MRP_PLS.H                                                    */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif  /* of MRP_PLS_H */
