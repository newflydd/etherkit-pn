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
/*  F i l e               &F: iod_cfg2.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  device configuration file                                                */
/*                                                                           */
/*****************************************************************************/
#ifndef IOD_CFG2_H
#define IOD_CFG2_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif


#include "compiler.h"

// *-------------------------------------------------------------------------------------
// *
// *   DEVICE ressources for APIs, slots, subslots, AR's
// *
// *   (values can be changed in user device (recommendation: change only if necessary)
// *-------------------------------------------------------------------------------------

#define IOD_CFG_NUMOF_AR                    (IOD_CFG_NUMOF_IO_AR + IOD_CFG_NUMOF_DEV_ACCESS_AR)
#define IOD_CFG_MAX_NUMOF_SUBSLOTS          IOD_CFG_NUMOF_SUBSLOTS // (IOD_CFG_MAX_SLOT_NUMBER + 2 + IOD_CFG_PDEV_NUMOF_PORTS)  // total max. subslots (Modules+Head+Ports)

// return TRUE if submodule supports ProfiEnergy
#define IOD_ENABLE_PROFIENERGY(_SLOT_NR, _SUBSLOT_NR, _MODUL_IDENT, _SUBMODUL_IDENT)		1

#if IOD_INCLUDE_IM5
#define IOD_ENABLE_IM5		1
#else
#define IOD_ENABLE_IM5		0
#endif


#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
