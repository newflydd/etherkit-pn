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
/*  F i l e               &F: ltrc_if.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*                       Frame for file "trc_if.h".                          */
/*                       ===========================                         */
/*                                                                           */
/*  Include-File:                                                            */
/*                                                                           */
/*  This file is prepared for subsystems will be abel to trace               */
/*  Includes of ltrc and of the different systems, compilers or              */
/*  operating system.                                                        */
/*                                                                           */
/*  This file has to be overwritten during system integration, because       */
/*  some includes depend on the different system, compiler or                */
/*  operating system.                                                        */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/

#ifndef LTRC_IF_H                        /* ----- reinclude-protection ----- */
#define LTRC_IF_H


#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

/*===================================================================================================================*/
#include "trc_if.h"

/*===================================================================================================================*/
/* >>> Define trace macros marking _idx as unused for CM, MRP to prevent from a big number of warnings : <<<         */
/*===================================================================================================================*/

/*===================================================================================================================*/
/*===================================================================================================================*/
/*====   CM    ======================================================================================================*/
/*===================================================================================================================*/
/*===================================================================================================================*/

#ifndef CM_UPPER_TRACE_00
#define CM_UPPER_TRACE_00(idx_, level_, msg_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_00(TRACE_SUBSYS_CM_UPPER, level_, msg_); }
#endif

#ifndef CM_UPPER_TRACE_01
#define CM_UPPER_TRACE_01(idx_, level_, msg_, para1_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_01(TRACE_SUBSYS_CM_UPPER, level_, msg_, para1_); }
#endif

#ifndef CM_UPPER_TRACE_02
#define CM_UPPER_TRACE_02(idx_, level_, msg_, para1_, para2_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_02(TRACE_SUBSYS_CM_UPPER, level_, msg_, para1_, para2_); }
#endif

#ifndef CM_UPPER_TRACE_03
#define CM_UPPER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_03(TRACE_SUBSYS_CM_UPPER, level_, msg_, para1_, para2_, para3_); }
#endif

#ifndef CM_UPPER_TRACE_04
#define CM_UPPER_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_04(TRACE_SUBSYS_CM_UPPER, level_, msg_, para1_, para2_, para3_, para4_); }
#endif

#ifndef CM_UPPER_TRACE_05
#define CM_UPPER_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_05(TRACE_SUBSYS_CM_UPPER, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
#endif

#ifndef CM_UPPER_TRACE_06
#define CM_UPPER_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_06(TRACE_SUBSYS_CM_UPPER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
#endif

#ifndef CM_UPPER_TRACE_07
#define CM_UPPER_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_07(TRACE_SUBSYS_CM_UPPER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
#endif

#ifndef CM_UPPER_TRACE_08
#define CM_UPPER_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_08(TRACE_SUBSYS_CM_UPPER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
#endif

#ifndef CM_UPPER_TRACE_09
#define CM_UPPER_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_09(TRACE_SUBSYS_CM_UPPER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_); }
#endif

/*----------------------------------------------------------------------------*/

#ifndef CM_PD_TRACE_00
#define CM_PD_TRACE_00(idx_, level_, msg_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_00(TRACE_SUBSYS_CM_PD, level_, msg_); }
#endif

#ifndef CM_PD_TRACE_01
#define CM_PD_TRACE_01(idx_, level_, msg_, para1_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_01(TRACE_SUBSYS_CM_PD, level_, msg_, para1_); }
#endif

#ifndef CM_PD_TRACE_02
#define CM_PD_TRACE_02(idx_, level_, msg_, para1_, para2_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_02(TRACE_SUBSYS_CM_PD, level_, msg_, para1_, para2_); }
#endif

#ifndef CM_PD_TRACE_03
#define CM_PD_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_03(TRACE_SUBSYS_CM_PD, level_, msg_, para1_, para2_, para3_); }
#endif

#ifndef CM_PD_TRACE_04
#define CM_PD_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_04(TRACE_SUBSYS_CM_PD, level_, msg_, para1_, para2_, para3_, para4_); }
#endif

#ifndef CM_PD_TRACE_05
#define CM_PD_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_05(TRACE_SUBSYS_CM_PD, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
#endif

#ifndef CM_PD_TRACE_06
#define CM_PD_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_06(TRACE_SUBSYS_CM_PD, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
#endif

#ifndef CM_PD_TRACE_07
#define CM_PD_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_07(TRACE_SUBSYS_CM_PD, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
#endif

#ifndef CM_PD_TRACE_08
#define CM_PD_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_08(TRACE_SUBSYS_CM_PD, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
#endif

#ifndef CM_PD_TRACE_09
#define CM_PD_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_09(TRACE_SUBSYS_CM_PD, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_); }
#endif

#ifndef CM_PD_TRACE_10
#define CM_PD_TRACE_10(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_10(TRACE_SUBSYS_CM_PD, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_); }
#endif

/*----------------------------------------------------------------------------*/

#ifndef CM_CL_TRACE_00
#define CM_CL_TRACE_00(idx_, level_, msg_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_00(TRACE_SUBSYS_CM_CL, level_, msg_); }
#endif

#ifndef CM_CL_TRACE_01
#define CM_CL_TRACE_01(idx_, level_, msg_, para1_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_01(TRACE_SUBSYS_CM_CL, level_, msg_, para1_); }
#endif

#ifndef CM_CL_TRACE_02
#define CM_CL_TRACE_02(idx_, level_, msg_, para1_, para2_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_02(TRACE_SUBSYS_CM_CL, level_, msg_, para1_, para2_); }
#endif

#ifndef CM_CL_TRACE_03
#define CM_CL_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_03(TRACE_SUBSYS_CM_CL, level_, msg_, para1_, para2_, para3_); }
#endif

#ifndef CM_CL_TRACE_04
#define CM_CL_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_04(TRACE_SUBSYS_CM_CL, level_, msg_, para1_, para2_, para3_, para4_); }
#endif

#ifndef CM_CL_TRACE_05
#define CM_CL_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_05(TRACE_SUBSYS_CM_CL, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
#endif

#ifndef CM_CL_TRACE_06
#define CM_CL_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_06(TRACE_SUBSYS_CM_CL, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
#endif

#ifndef CM_CL_TRACE_07
#define CM_CL_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_07(TRACE_SUBSYS_CM_CL, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
#endif

#ifndef CM_CL_TRACE_08
#define CM_CL_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_08(TRACE_SUBSYS_CM_CL, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
#endif

#ifndef CM_CL_TRACE_09
#define CM_CL_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_09(TRACE_SUBSYS_CM_CL, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_); }
#endif

#ifndef CM_CL_TRACE_10
#define CM_CL_TRACE_10(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_10(TRACE_SUBSYS_CM_CL, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_); }
#endif

#ifndef CM_CL_TRACE_11
#define CM_CL_TRACE_11(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_11(TRACE_SUBSYS_CM_CL, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_); }
#endif

#ifndef CM_CL_TRACE_12
#define CM_CL_TRACE_12(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_12(TRACE_SUBSYS_CM_CL, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_); }
#endif

/*----------------------------------------------------------------------------*/

#ifndef CM_MC_TRACE_00
#define CM_MC_TRACE_00(idx_, level_, msg_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_00(TRACE_SUBSYS_CM_MC, level_, msg_); }
#endif

#ifndef CM_MC_TRACE_01
#define CM_MC_TRACE_01(idx_, level_, msg_, para1_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_01(TRACE_SUBSYS_CM_MC, level_, msg_, para1_); }
#endif

#ifndef CM_MC_TRACE_02
#define CM_MC_TRACE_02(idx_, level_, msg_, para1_, para2_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_02(TRACE_SUBSYS_CM_MC, level_, msg_, para1_, para2_); }
#endif

#ifndef CM_MC_TRACE_03
#define CM_MC_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_03(TRACE_SUBSYS_CM_MC, level_, msg_, para1_, para2_, para3_); }
#endif

#ifndef CM_MC_TRACE_04
#define CM_MC_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_04(TRACE_SUBSYS_CM_MC, level_, msg_, para1_, para2_, para3_, para4_); }
#endif

#ifndef CM_MC_TRACE_05
#define CM_MC_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_05(TRACE_SUBSYS_CM_MC, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
#endif

#ifndef CM_MC_TRACE_06
#define CM_MC_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_06(TRACE_SUBSYS_CM_MC, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
#endif

#ifndef CM_MC_TRACE_07
#define CM_MC_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_07(TRACE_SUBSYS_CM_MC, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
#endif

#ifndef CM_MC_TRACE_08
#define CM_MC_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_08(TRACE_SUBSYS_CM_MC, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
#endif

/*----------------------------------------------------------------------------*/

#ifndef CM_SV_TRACE_00
#define CM_SV_TRACE_00(idx_, level_, msg_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_00(TRACE_SUBSYS_CM_SV, level_, msg_); }
#endif

#ifndef CM_SV_TRACE_01
#define CM_SV_TRACE_01(idx_, level_, msg_, para1_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_01(TRACE_SUBSYS_CM_SV, level_, msg_, para1_); }
#endif

#ifndef CM_SV_TRACE_02
#define CM_SV_TRACE_02(idx_, level_, msg_, para1_, para2_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_02(TRACE_SUBSYS_CM_SV, level_, msg_, para1_, para2_); }
#endif

#ifndef CM_SV_TRACE_03
#define CM_SV_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_03(TRACE_SUBSYS_CM_SV, level_, msg_, para1_, para2_, para3_); }
#endif

#ifndef CM_SV_TRACE_04
#define CM_SV_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_04(TRACE_SUBSYS_CM_SV, level_, msg_, para1_, para2_, para3_, para4_); }
#endif

#ifndef CM_SV_TRACE_05
#define CM_SV_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_05(TRACE_SUBSYS_CM_SV, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
#endif

#ifndef CM_SV_TRACE_06
#define CM_SV_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_06(TRACE_SUBSYS_CM_SV, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
#endif

#ifndef CM_SV_TRACE_07
#define CM_SV_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_07(TRACE_SUBSYS_CM_SV, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
#endif

#ifndef CM_SV_TRACE_08
#define CM_SV_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_08(TRACE_SUBSYS_CM_SV, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
#endif

#ifndef CM_SV_TRACE_09
#define CM_SV_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_09(TRACE_SUBSYS_CM_SV, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_); }
#endif

#ifndef CM_SV_TRACE_10
#define CM_SV_TRACE_10(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_10(TRACE_SUBSYS_CM_SV, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_); }
#endif

#ifndef CM_SV_TRACE_11
#define CM_SV_TRACE_11(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_11(TRACE_SUBSYS_CM_SV, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_); }
#endif

#ifndef CM_SV_TRACE_12
#define CM_SV_TRACE_12(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_12(TRACE_SUBSYS_CM_SV, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_); }
#endif

/*----------------------------------------------------------------------------*/

#ifndef CM_AR_TRACE_00
#define CM_AR_TRACE_00(idx_, level_, msg_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_00(TRACE_SUBSYS_CM_AR, level_, msg_); }
#endif

#ifndef CM_AR_TRACE_01
#define CM_AR_TRACE_01(idx_, level_, msg_, para1_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_01(TRACE_SUBSYS_CM_AR, level_, msg_, para1_); }
#endif

#ifndef CM_AR_TRACE_02
#define CM_AR_TRACE_02(idx_, level_, msg_, para1_, para2_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_02(TRACE_SUBSYS_CM_AR, level_, msg_, para1_, para2_); }
#endif

#ifndef CM_AR_TRACE_03
#define CM_AR_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_03(TRACE_SUBSYS_CM_AR, level_, msg_, para1_, para2_, para3_); }
#endif

#ifndef CM_AR_TRACE_04
#define CM_AR_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_04(TRACE_SUBSYS_CM_AR, level_, msg_, para1_, para2_, para3_, para4_); }
#endif

#ifndef CM_AR_TRACE_05
#define CM_AR_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_05(TRACE_SUBSYS_CM_AR, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
#endif

#ifndef CM_AR_TRACE_06
#define CM_AR_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_06(TRACE_SUBSYS_CM_AR, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
#endif

#ifndef CM_AR_TRACE_07
#define CM_AR_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_07(TRACE_SUBSYS_CM_AR, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
#endif

#ifndef CM_AR_TRACE_08
#define CM_AR_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_08(TRACE_SUBSYS_CM_AR, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
#endif

#ifndef CM_AR_TRACE_09
#define CM_AR_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_09(TRACE_SUBSYS_CM_AR, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_); }
#endif

#ifndef CM_AR_TRACE_10
#define CM_AR_TRACE_10(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_10(TRACE_SUBSYS_CM_AR, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_); }
#endif

#ifndef CM_AR_TRACE_11
#define CM_AR_TRACE_11(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_11(TRACE_SUBSYS_CM_AR, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_); }
#endif

#ifndef CM_AR_TRACE_12
#define CM_AR_TRACE_12(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_12(TRACE_SUBSYS_CM_AR, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_); }
#endif

/*----------------------------------------------------------------------------*/

#ifndef CM_ACP_TRACE_00
#define CM_ACP_TRACE_00(idx_, level_, msg_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_00(TRACE_SUBSYS_CM_ACP, level_, msg_); }
#endif

#ifndef CM_ACP_TRACE_01
#define CM_ACP_TRACE_01(idx_, level_, msg_, para1_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_01(TRACE_SUBSYS_CM_ACP, level_, msg_, para1_); }
#endif

#ifndef CM_ACP_TRACE_02
#define CM_ACP_TRACE_02(idx_, level_, msg_, para1_, para2_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_02(TRACE_SUBSYS_CM_ACP, level_, msg_, para1_, para2_); }
#endif

#ifndef CM_ACP_TRACE_03
#define CM_ACP_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_03(TRACE_SUBSYS_CM_ACP, level_, msg_, para1_, para2_, para3_); }
#endif

#ifndef CM_ACP_TRACE_04
#define CM_ACP_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_04(TRACE_SUBSYS_CM_ACP, level_, msg_, para1_, para2_, para3_, para4_); }
#endif

#ifndef CM_ACP_TRACE_05
#define CM_ACP_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_05(TRACE_SUBSYS_CM_ACP, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
#endif

#ifndef CM_ACP_TRACE_06
#define CM_ACP_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_06(TRACE_SUBSYS_CM_ACP, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
#endif

#ifndef CM_ACP_TRACE_07
#define CM_ACP_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_07(TRACE_SUBSYS_CM_ACP, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
#endif

#ifndef CM_ACP_TRACE_08
#define CM_ACP_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_08(TRACE_SUBSYS_CM_ACP, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
#endif

#ifndef CM_ACP_TRACE_09
#define CM_ACP_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_09(TRACE_SUBSYS_CM_ACP, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_); }
#endif

#ifndef CM_ACP_TRACE_10
#define CM_ACP_TRACE_10(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_10(TRACE_SUBSYS_CM_ACP, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_); }
#endif

/*----------------------------------------------------------------------------*/

#ifndef CM_EDD_TRACE_00
#define CM_EDD_TRACE_00(idx_, level_, msg_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_00(TRACE_SUBSYS_CM_EDD, level_, msg_); }
#endif

#ifndef CM_EDD_TRACE_01
#define CM_EDD_TRACE_01(idx_, level_, msg_, para1_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_01(TRACE_SUBSYS_CM_EDD, level_, msg_, para1_); }
#endif

#ifndef CM_EDD_TRACE_02
#define CM_EDD_TRACE_02(idx_, level_, msg_, para1_, para2_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_02(TRACE_SUBSYS_CM_EDD, level_, msg_, para1_, para2_); }
#endif

#ifndef CM_EDD_TRACE_03
#define CM_EDD_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_03(TRACE_SUBSYS_CM_EDD, level_, msg_, para1_, para2_, para3_); }
#endif

#ifndef CM_EDD_TRACE_04
#define CM_EDD_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_04(TRACE_SUBSYS_CM_EDD, level_, msg_, para1_, para2_, para3_, para4_); }
#endif

#ifndef CM_EDD_TRACE_05
#define CM_EDD_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_05(TRACE_SUBSYS_CM_EDD, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
#endif

#ifndef CM_EDD_TRACE_06
#define CM_EDD_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_06(TRACE_SUBSYS_CM_EDD, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
#endif

#ifndef CM_EDD_TRACE_07
#define CM_EDD_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_07(TRACE_SUBSYS_CM_EDD, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
#endif

#ifndef CM_EDD_TRACE_08
#define CM_EDD_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_08(TRACE_SUBSYS_CM_EDD, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
#endif

#ifndef CM_EDD_TRACE_09
#define CM_EDD_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_09(TRACE_SUBSYS_CM_EDD, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_); }
#endif

/*----------------------------------------------------------------------------*/

#ifndef CM_GSY_TRACE_00
#define CM_GSY_TRACE_00(idx_, level_, msg_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_00(TRACE_SUBSYS_CM_GSY, level_, msg_); }
#endif

#ifndef CM_GSY_TRACE_01
#define CM_GSY_TRACE_01(idx_, level_, msg_, para1_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_01(TRACE_SUBSYS_CM_GSY, level_, msg_, para1_); }
#endif

#ifndef CM_GSY_TRACE_02
#define CM_GSY_TRACE_02(idx_, level_, msg_, para1_, para2_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_02(TRACE_SUBSYS_CM_GSY, level_, msg_, para1_, para2_); }
#endif

#ifndef CM_GSY_TRACE_03
#define CM_GSY_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_03(TRACE_SUBSYS_CM_GSY, level_, msg_, para1_, para2_, para3_); }
#endif

#ifndef CM_GSY_TRACE_04
#define CM_GSY_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_04(TRACE_SUBSYS_CM_GSY, level_, msg_, para1_, para2_, para3_, para4_); }
#endif

#ifndef CM_GSY_TRACE_05
#define CM_GSY_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_05(TRACE_SUBSYS_CM_GSY, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
#endif

#ifndef CM_GSY_TRACE_06
#define CM_GSY_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_06(TRACE_SUBSYS_CM_GSY, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
#endif

#ifndef CM_GSY_TRACE_07
#define CM_GSY_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_07(TRACE_SUBSYS_CM_GSY, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
#endif

#ifndef CM_GSY_TRACE_12
#define CM_GSY_TRACE_12(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_12(TRACE_SUBSYS_CM_GSY, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_); }
#endif

/*----------------------------------------------------------------------------*/

#ifndef CM_NARE_TRACE_00
#define CM_NARE_TRACE_00(idx_, level_, msg_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_00(TRACE_SUBSYS_CM_NARE, level_, msg_); }
#endif

#ifndef CM_NARE_TRACE_01
#define CM_NARE_TRACE_01(idx_, level_, msg_, para1_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_01(TRACE_SUBSYS_CM_NARE, level_, msg_, para1_); }
#endif

#ifndef CM_NARE_TRACE_02
#define CM_NARE_TRACE_02(idx_, level_, msg_, para1_, para2_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_02(TRACE_SUBSYS_CM_NARE, level_, msg_, para1_, para2_); }
#endif

#ifndef CM_NARE_TRACE_03
#define CM_NARE_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_03(TRACE_SUBSYS_CM_NARE, level_, msg_, para1_, para2_, para3_); }
#endif

#ifndef CM_NARE_TRACE_04
#define CM_NARE_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_04(TRACE_SUBSYS_CM_NARE, level_, msg_, para1_, para2_, para3_, para4_); }
#endif

#ifndef CM_NARE_TRACE_05
#define CM_NARE_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_05(TRACE_SUBSYS_CM_NARE, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
#endif

#ifndef CM_NARE_TRACE_06
#define CM_NARE_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_06(TRACE_SUBSYS_CM_NARE, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
#endif

#ifndef CM_NARE_TRACE_07
#define CM_NARE_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_07(TRACE_SUBSYS_CM_NARE, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
#endif

#ifndef CM_NARE_TRACE_08
#define CM_NARE_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_08(TRACE_SUBSYS_CM_NARE, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
#endif

/*----------------------------------------------------------------------------*/

#ifndef CM_OHA_TRACE_00
#define CM_OHA_TRACE_00(idx_, level_, msg_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_00(TRACE_SUBSYS_CM_OHA, level_, msg_); }
#endif

#ifndef CM_OHA_TRACE_01
#define CM_OHA_TRACE_01(idx_, level_, msg_, para1_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_01(TRACE_SUBSYS_CM_OHA, level_, msg_, para1_); }
#endif

#ifndef CM_OHA_TRACE_02
#define CM_OHA_TRACE_02(idx_, level_, msg_, para1_, para2_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_02(TRACE_SUBSYS_CM_OHA, level_, msg_, para1_, para2_); }
#endif

#ifndef CM_OHA_TRACE_03
#define CM_OHA_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_03(TRACE_SUBSYS_CM_OHA, level_, msg_, para1_, para2_, para3_); }
#endif

#ifndef CM_OHA_TRACE_04
#define CM_OHA_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_04(TRACE_SUBSYS_CM_OHA, level_, msg_, para1_, para2_, para3_, para4_); }
#endif

#ifndef CM_OHA_TRACE_05
#define CM_OHA_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_05(TRACE_SUBSYS_CM_OHA, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
#endif

#ifndef CM_OHA_TRACE_06
#define CM_OHA_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_06(TRACE_SUBSYS_CM_OHA, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
#endif

#ifndef CM_OHA_TRACE_07
#define CM_OHA_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_07(TRACE_SUBSYS_CM_OHA, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
#endif

#ifndef CM_OHA_TRACE_08
#define CM_OHA_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_08(TRACE_SUBSYS_CM_OHA, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
#endif

#ifndef CM_OHA_TRACE_12
#define CM_OHA_TRACE_12(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_12(TRACE_SUBSYS_CM_OHA, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_); }
#endif

/*----------------------------------------------------------------------------*/

#ifndef CM_RPC_TRACE_00
#define CM_RPC_TRACE_00(idx_, level_, msg_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_00(TRACE_SUBSYS_CM_RPC, level_, msg_); }
#endif

#ifndef CM_RPC_TRACE_01
#define CM_RPC_TRACE_01(idx_, level_, msg_, para1_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_01(TRACE_SUBSYS_CM_RPC, level_, msg_, para1_); }
#endif

#ifndef CM_RPC_TRACE_02
#define CM_RPC_TRACE_02(idx_, level_, msg_, para1_, para2_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_02(TRACE_SUBSYS_CM_RPC, level_, msg_, para1_, para2_); }
#endif

#ifndef CM_RPC_TRACE_03
#define CM_RPC_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_03(TRACE_SUBSYS_CM_RPC, level_, msg_, para1_, para2_, para3_); }
#endif

#ifndef CM_RPC_TRACE_04
#define CM_RPC_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_04(TRACE_SUBSYS_CM_RPC, level_, msg_, para1_, para2_, para3_, para4_); }
#endif

#ifndef CM_RPC_TRACE_05
#define CM_RPC_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_05(TRACE_SUBSYS_CM_RPC, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
#endif

#ifndef CM_RPC_TRACE_06
#define CM_RPC_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_06(TRACE_SUBSYS_CM_RPC, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
#endif

#ifndef CM_RPC_TRACE_07
#define CM_RPC_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_07(TRACE_SUBSYS_CM_RPC, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
#endif

#ifndef CM_RPC_TRACE_08
#define CM_RPC_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_08(TRACE_SUBSYS_CM_RPC, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
#endif

/*----------------------------------------------------------------------------*/

#ifndef CM_MRP_TRACE_00
#define CM_MRP_TRACE_00(idx_, level_, msg_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_00(TRACE_SUBSYS_CM_MRP, level_, msg_); }
#endif

#ifndef CM_MRP_TRACE_01
#define CM_MRP_TRACE_01(idx_, level_, msg_, para1_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_01(TRACE_SUBSYS_CM_MRP, level_, msg_, para1_); }
#endif

#ifndef CM_MRP_TRACE_02
#define CM_MRP_TRACE_02(idx_, level_, msg_, para1_, para2_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_02(TRACE_SUBSYS_CM_MRP, level_, msg_, para1_, para2_); }
#endif

#ifndef CM_MRP_TRACE_03
#define CM_MRP_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_03(TRACE_SUBSYS_CM_MRP, level_, msg_, para1_, para2_, para3_); }
#endif

#ifndef CM_MRP_TRACE_04
#define CM_MRP_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_04(TRACE_SUBSYS_CM_MRP, level_, msg_, para1_, para2_, para3_, para4_); }
#endif

#ifndef CM_MRP_TRACE_05
#define CM_MRP_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_05(TRACE_SUBSYS_CM_MRP, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
#endif

#ifndef CM_MRP_TRACE_06
#define CM_MRP_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_06(TRACE_SUBSYS_CM_MRP, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
#endif

#ifndef CM_MRP_TRACE_07
#define CM_MRP_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_07(TRACE_SUBSYS_CM_MRP, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
#endif

#ifndef CM_MRP_TRACE_08
#define CM_MRP_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_08(TRACE_SUBSYS_CM_MRP, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
#endif

/*----------------------------------------------------------------------------*/

#ifndef CM_POF_TRACE_00
#define CM_POF_TRACE_00(idx_, level_, msg_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_00(TRACE_SUBSYS_CM_POF, level_, msg_); }
#endif

#ifndef CM_POF_TRACE_01
#define CM_POF_TRACE_01(idx_, level_, msg_, para1_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_01(TRACE_SUBSYS_CM_POF, level_, msg_, para1_); }
#endif

#ifndef CM_POF_TRACE_02
#define CM_POF_TRACE_02(idx_, level_, msg_, para1_, para2_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_02(TRACE_SUBSYS_CM_POF, level_, msg_, para1_, para2_); }
#endif

#ifndef CM_POF_TRACE_03
#define CM_POF_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_03(TRACE_SUBSYS_CM_POF, level_, msg_, para1_, para2_, para3_); }
#endif

#ifndef CM_POF_TRACE_04
#define CM_POF_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_04(TRACE_SUBSYS_CM_POF, level_, msg_, para1_, para2_, para3_, para4_); }
#endif

#ifndef CM_POF_TRACE_05
#define CM_POF_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_05(TRACE_SUBSYS_CM_POF, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
#endif

#ifndef CM_POF_TRACE_06
#define CM_POF_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_06(TRACE_SUBSYS_CM_POF, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
#endif

#ifndef CM_POF_TRACE_07
#define CM_POF_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_07(TRACE_SUBSYS_CM_POF, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
#endif

#ifndef CM_POF_TRACE_08
#define CM_POF_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_08(TRACE_SUBSYS_CM_POF, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
#endif

/*----------------------------------------------------------------------------*/

#ifndef CM_MEM_TRACE_00
#define CM_MEM_TRACE_00(idx_, level_, msg_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_00(TRACE_SUBSYS_CM_MEM, level_, msg_); }
#endif

#ifndef CM_MEM_TRACE_01
#define CM_MEM_TRACE_01(idx_, level_, msg_, para1_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_01(TRACE_SUBSYS_CM_MEM, level_, msg_, para1_); }
#endif

#ifndef CM_MEM_TRACE_02
#define CM_MEM_TRACE_02(idx_, level_, msg_, para1_, para2_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_02(TRACE_SUBSYS_CM_MEM, level_, msg_, para1_, para2_); }
#endif

#ifndef CM_MEM_TRACE_03
#define CM_MEM_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_03(TRACE_SUBSYS_CM_MEM, level_, msg_, para1_, para2_, para3_); }
#endif

#ifndef CM_MEM_TRACE_04
#define CM_MEM_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_04(TRACE_SUBSYS_CM_MEM, level_, msg_, para1_, para2_, para3_, para4_); }
#endif

#ifndef CM_MEM_TRACE_05
#define CM_MEM_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_05(TRACE_SUBSYS_CM_MEM, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
#endif

#ifndef CM_MEM_TRACE_06
#define CM_MEM_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_06(TRACE_SUBSYS_CM_MEM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
#endif

#ifndef CM_MEM_TRACE_07
#define CM_MEM_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_07(TRACE_SUBSYS_CM_MEM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
#endif

#ifndef CM_MEM_TRACE_08
#define CM_MEM_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_08(TRACE_SUBSYS_CM_MEM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
#endif


/*===================================================================================================================*/
/*===================================================================================================================*/
/*====   DCP   ======================================================================================================*/
/*===================================================================================================================*/
/*===================================================================================================================*/

#ifndef DCP_UPPER_TRACE_04
#define DCP_UPPER_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_04(TRACE_SUBSYS_DCP_UPPER, level_, msg_, para1_, para2_, para3_, para4_); }
#endif
#ifndef DCP_UPPER_TRACE_03
#define DCP_UPPER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_03(TRACE_SUBSYS_DCP_UPPER, level_, msg_, para1_, para2_, para3_); }
#endif
#ifndef DCP_UPPER_TRACE_02
#define DCP_UPPER_TRACE_02(idx_, level_, msg_, para1_, para2_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_02(TRACE_SUBSYS_DCP_UPPER, level_, msg_, para1_, para2_); }
#endif
#ifndef DCP_UPPER_TRACE_01
#define DCP_UPPER_TRACE_01(idx_, level_, msg_, para1_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_01(TRACE_SUBSYS_DCP_UPPER, level_, msg_, para1_); }
#endif
#ifndef DCP_LOWER_TRACE_04
#define DCP_LOWER_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_04(TRACE_SUBSYS_DCP_LOWER, level_, msg_, para1_, para2_, para3_, para4_); }
#endif
#ifndef DCP_LOWER_TRACE_03
#define DCP_LOWER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_03(TRACE_SUBSYS_DCP_LOWER, level_, msg_, para1_, para2_, para3_); }
#endif
#ifndef DCP_LOWER_TRACE_02
#define DCP_LOWER_TRACE_02(idx_, level_, msg_, para1_, para2_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_02(TRACE_SUBSYS_DCP_LOWER, level_, msg_, para1_, para2_); }
#endif
#ifndef DCP_LOWER_TRACE_01
#define DCP_LOWER_TRACE_01(idx_, level_, msg_, para1_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_01(TRACE_SUBSYS_DCP_LOWER, level_, msg_, para1_); }
#endif
#ifndef DCP_SYSTEM_TRACE_04
#define DCP_SYSTEM_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_04(TRACE_SUBSYS_DCP_SYSTEM, level_, msg_, para1_, para2_, para3_, para4_); }
#endif
#ifndef DCP_SYSTEM_TRACE_03
#define DCP_SYSTEM_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_03(TRACE_SUBSYS_DCP_SYSTEM, level_, msg_, para1_, para2_, para3_); }
#endif
#ifndef DCP_SYSTEM_TRACE_02
#define DCP_SYSTEM_TRACE_02(idx_, level_, msg_, para1_, para2_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_02(TRACE_SUBSYS_DCP_SYSTEM, level_, msg_, para1_, para2_); }
#endif
#ifndef DCP_SYSTEM_TRACE_01
#define DCP_SYSTEM_TRACE_01(idx_, level_, msg_, para1_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_01(TRACE_SUBSYS_DCP_SYSTEM, level_, msg_, para1_); }
#endif
#ifndef DCP_ERROR_TRACE_05
#define DCP_ERROR_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_,para5_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_05(TRACE_SUBSYS_DCP_ERROR, level_, msg_, para1_, para2_, para3_, para4_,para5_); }
#endif
#ifndef DCP_ERROR_TRACE_04
#define DCP_ERROR_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_04(TRACE_SUBSYS_DCP_ERROR, level_, msg_, para1_, para2_, para3_, para4_); }
#endif
#ifndef DCP_ERROR_TRACE_03
#define DCP_ERROR_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_03(TRACE_SUBSYS_DCP_ERROR, level_, msg_, para1_, para2_, para3_); }
#endif
#ifndef DCP_ERROR_TRACE_02
#define DCP_ERROR_TRACE_02(idx_, level_, msg_, para1_, para2_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_02(TRACE_SUBSYS_DCP_ERROR, level_, msg_, para1_, para2_); }
#endif
#ifndef DCP_ERROR_TRACE_01
#define DCP_ERROR_TRACE_01(idx_, level_, msg_, para1_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_01(TRACE_SUBSYS_DCP_ERROR, level_, msg_, para1_); }
#endif
#ifndef DCP_ERROR_TRACE_00
#define DCP_ERROR_TRACE_00(idx_, level_, msg_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_00(TRACE_SUBSYS_DCP_ERROR, level_, msg_); }
#endif
#ifndef DCP_FUNCTION_TRACE_07
#define DCP_FUNCTION_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_07(TRACE_SUBSYS_DCP_FUNCTION, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
#endif
#ifndef DCP_FUNCTION_TRACE_06
#define DCP_FUNCTION_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_06(TRACE_SUBSYS_DCP_FUNCTION, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
#endif
#ifndef DCP_FUNCTION_TRACE_05
#define DCP_FUNCTION_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_05(TRACE_SUBSYS_DCP_FUNCTION, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
#endif
#ifndef DCP_FUNCTION_TRACE_04
#define DCP_FUNCTION_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_04(TRACE_SUBSYS_DCP_FUNCTION, level_, msg_, para1_, para2_, para3_, para4_); }
#endif
#ifndef DCP_FUNCTION_TRACE_03
#define DCP_FUNCTION_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_03(TRACE_SUBSYS_DCP_FUNCTION, level_, msg_, para1_, para2_, para3_); }
#endif
#ifndef DCP_FUNCTION_TRACE_02
#define DCP_FUNCTION_TRACE_02(idx_, level_, msg_, para1_, para2_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_02(TRACE_SUBSYS_DCP_FUNCTION, level_, msg_, para1_, para2_); }
#endif
#ifndef DCP_FUNCTION_TRACE_01
#define DCP_FUNCTION_TRACE_01(idx_, level_, msg_, para1_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_01(TRACE_SUBSYS_DCP_FUNCTION, level_, msg_, para1_); }
#endif
#ifndef DCP_FUNCTION_TRACE_00
#define DCP_FUNCTION_TRACE_00(idx_, level_, msg_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_00(TRACE_SUBSYS_DCP_FUNCTION, level_, msg_); }
#endif
#ifndef DCP_SERVER_TRACE_04
#define DCP_SERVER_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_04(TRACE_SUBSYS_DCP_SERVER, level_, msg_, para1_, para2_, para3_, para4_); }
#endif
#ifndef DCP_SERVER_TRACE_03
#define DCP_SERVER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_03(TRACE_SUBSYS_DCP_SERVER, level_, msg_, para1_, para2_, para3_); }
#endif
#ifndef DCP_SERVER_TRACE_02
#define DCP_SERVER_TRACE_02(idx_, level_, msg_, para1_, para2_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_02(TRACE_SUBSYS_DCP_SERVER, level_, msg_, para1_, para2_); }
#endif
#ifndef DCP_SERVER_TRACE_01
#define DCP_SERVER_TRACE_01(idx_, level_, msg_, para1_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_01(TRACE_SUBSYS_DCP_SERVER, level_, msg_, para1_); }
#endif
#ifndef DCP_SRVERR_TRACE_04
#define DCP_SRVERR_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_04(TRACE_SUBSYS_DCP_SRVERR, level_, msg_, para1_, para2_, para3_, para4_); }
#endif
#ifndef DCP_SRVERR_TRACE_03
#define DCP_SRVERR_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_03(TRACE_SUBSYS_DCP_SRVERR, level_, msg_, para1_, para2_, para3_); }
#endif
#ifndef DCP_SRVERR_TRACE_02
#define DCP_SRVERR_TRACE_02(idx_, level_, msg_, para1_, para2_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_02(TRACE_SUBSYS_DCP_SRVERR, level_, msg_, para1_, para2_); }
#endif
#ifndef DCP_SRVERR_TRACE_01
#define DCP_SRVERR_TRACE_01(idx_, level_, msg_, para1_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_01(TRACE_SUBSYS_DCP_SRVERR, level_, msg_, para1_); }
#endif


/*===================================================================================================================*/
/*===================================================================================================================*/
/*====   GSY   ======================================================================================================*/
/*===================================================================================================================*/
/*===================================================================================================================*/

#ifndef GSY_UPPER_TRACE_01
#define GSY_UPPER_TRACE_01(idx_, level_, msg_, para1_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_01(TRACE_SUBSYS_GSY_UPPER, level_, msg_, para1_); }
#endif
#ifndef GSY_UPPER_TRACE_02
#define GSY_UPPER_TRACE_02(idx_, level_, msg_, para1_, para2_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_02(TRACE_SUBSYS_GSY_UPPER, level_, msg_, para1_, para2_); }
#endif
#ifndef GSY_UPPER_TRACE_03
#define GSY_UPPER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_03(TRACE_SUBSYS_GSY_UPPER, level_, msg_, para1_, para2_, para3_); }
#endif
#ifndef GSY_UPPER_TRACE_04
#define GSY_UPPER_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_04(TRACE_SUBSYS_GSY_UPPER, level_, msg_, para1_, para2_, para3_, para4_); }
#endif

#ifndef GSY_LOWER_TRACE_03
#define GSY_LOWER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_03(TRACE_SUBSYS_GSY_LOWER, level_, msg_, para1_, para2_, para3_); }
#endif

#ifndef GSY_SYSTEM_TRACE_00
#define GSY_SYSTEM_TRACE_00(idx_, level_, msg_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_00(TRACE_SUBSYS_GSY_SYSTEM, level_, msg_); }
#endif
#ifndef GSY_SYSTEM_TRACE_01
#define GSY_SYSTEM_TRACE_01(idx_, level_, msg_, para1_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_01(TRACE_SUBSYS_GSY_SYSTEM, level_, msg_, para1_); }
#endif
#ifndef GSY_SYSTEM_TRACE_02
#define GSY_SYSTEM_TRACE_02(idx_, level_, msg_, para1_, para2_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_02(TRACE_SUBSYS_GSY_SYSTEM, level_, msg_, para1_, para2_); }
#endif
#ifndef GSY_SYSTEM_TRACE_03
#define GSY_SYSTEM_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_03(TRACE_SUBSYS_GSY_SYSTEM, level_, msg_, para1_, para2_, para3_); }
#endif
#ifndef GSY_SYSTEM_TRACE_04
#define GSY_SYSTEM_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_04(TRACE_SUBSYS_GSY_SYSTEM, level_, msg_, para1_, para2_, para3_, para4_); }
#endif

#ifndef GSY_FUNCTION_TRACE_00
#define GSY_FUNCTION_TRACE_00(idx_, level_, msg_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_00(TRACE_SUBSYS_GSY_FUNCTION, level_, msg_); }
#endif
#ifndef GSY_FUNCTION_TRACE_01
#define GSY_FUNCTION_TRACE_01(idx_, level_, msg_, para1_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_01(TRACE_SUBSYS_GSY_FUNCTION, level_, msg_, para1_); }
#endif
#ifndef GSY_FUNCTION_TRACE_02
#define GSY_FUNCTION_TRACE_02(idx_, level_, msg_, para1_, para2_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_02(TRACE_SUBSYS_GSY_FUNCTION, level_, msg_, para1_, para2_); }
#endif
#ifndef GSY_FUNCTION_TRACE_03
#define GSY_FUNCTION_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_03(TRACE_SUBSYS_GSY_FUNCTION, level_, msg_, para1_, para2_, para3_); }
#endif
#ifndef GSY_FUNCTION_TRACE_04
#define GSY_FUNCTION_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_04(TRACE_SUBSYS_GSY_FUNCTION, level_, msg_, para1_, para2_, para3_, para4_); }
#endif

#ifndef GSY_ERROR_TRACE_00
#define GSY_ERROR_TRACE_00(idx_, level_, msg_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_00(TRACE_SUBSYS_GSY_ERROR, level_, msg_); }
#endif
#ifndef GSY_ERROR_TRACE_01
#define GSY_ERROR_TRACE_01(idx_, level_, msg_, para1_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_01(TRACE_SUBSYS_GSY_ERROR, level_, msg_, para1_); }
#endif
#ifndef GSY_ERROR_TRACE_02
#define GSY_ERROR_TRACE_02(idx_, level_, msg_, para1_, para2_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_02(TRACE_SUBSYS_GSY_ERROR, level_, msg_, para1_, para2_); }
#endif
#ifndef GSY_ERROR_TRACE_03
#define GSY_ERROR_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) { LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_03(TRACE_SUBSYS_GSY_ERROR, level_, msg_, para1_, para2_, para3_); }
#endif
#ifndef GSY_ERROR_TRACE_04
#define GSY_ERROR_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_04(TRACE_SUBSYS_GSY_ERROR, level_, msg_, para1_, para2_, para3_, para4_); }
#endif

#ifndef GSY_DIAG_TRACE_04
#define GSY_DIAG_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_04(TRACE_SUBSYS_GSY_DIAG, level_, msg_, para1_, para2_, para3_, para4_); }
#endif

#ifndef GSY_DEL_TRACE_08
#define GSY_DEL_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_08(TRACE_SUBSYS_GSY_DEL, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
#endif
#ifndef GSY_DEL_TRACE_09
#define GSY_DEL_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_09(TRACE_SUBSYS_GSY_DEL, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_); }
#endif
#ifndef GSY_DEL_TRACE_10
#define GSY_DEL_TRACE_10(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_10(TRACE_SUBSYS_GSY_DEL, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_); }
#endif
#ifndef GSY_DEL_TRACE_11
#define GSY_DEL_TRACE_11(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_11(TRACE_SUBSYS_GSY_DEL, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_); }
#endif

#ifndef GSY_FWD_TRACE_00
#define GSY_FWD_TRACE_00(idx_, level_, msg_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_00(TRACE_SUBSYS_GSY_FWD, level_, msg_); }
#endif
#ifndef GSY_FWD_TRACE_01
#define GSY_FWD_TRACE_01(idx_, level_, msg_, para1_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_01(TRACE_SUBSYS_GSY_FWD, level_, msg_, para1_); }
#endif
#ifndef GSY_FWD_TRACE_02
#define GSY_FWD_TRACE_02(idx_, level_, msg_, para1_, para2_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_02(TRACE_SUBSYS_GSY_FWD, level_, msg_, para1_, para2_); }
#endif
#ifndef GSY_FWD_TRACE_03
#define GSY_FWD_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_03(TRACE_SUBSYS_GSY_FWD, level_, msg_, para1_, para2_, para3_); }
#endif
#ifndef GSY_FWD_TRACE_04
#define GSY_FWD_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_04(TRACE_SUBSYS_GSY_FWD, level_, msg_, para1_, para2_, para3_, para4_); }
#endif
#ifndef GSY_FWD_TRACE_05
#define GSY_FWD_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_05(TRACE_SUBSYS_GSY_FWD, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
#endif
#ifndef GSY_FWD_TRACE_06
#define GSY_FWD_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_06(TRACE_SUBSYS_GSY_FWD, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
#endif

#ifndef GSY_PRM_TRACE_00
#define GSY_PRM_TRACE_00(idx_, level_, msg_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_00(TRACE_SUBSYS_GSY_PRM, level_, msg_); }
#endif
#ifndef GSY_PRM_TRACE_01
#define GSY_PRM_TRACE_01(idx_, level_, msg_, para1_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_01(TRACE_SUBSYS_GSY_PRM, level_, msg_, para1_); }
#endif
#ifndef GSY_PRM_TRACE_02
#define GSY_PRM_TRACE_02(idx_, level_, msg_, para1_, para2_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_02(TRACE_SUBSYS_GSY_PRM, level_, msg_, para1_, para2_); }
#endif
#ifndef GSY_PRM_TRACE_03
#define GSY_PRM_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_03(TRACE_SUBSYS_GSY_PRM, level_, msg_, para1_, para2_, para3_); }
#endif
#ifndef GSY_PRM_TRACE_04
#define GSY_PRM_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_04(TRACE_SUBSYS_GSY_PRM, level_, msg_, para1_, para2_, para3_, para4_); }
#endif
#ifndef GSY_PRM_TRACE_05
#define GSY_PRM_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_05(TRACE_SUBSYS_GSY_PRM, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
#endif
#ifndef GSY_PRM_TRACE_06
#define GSY_PRM_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_06(TRACE_SUBSYS_GSY_PRM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
#endif
#ifndef GSY_PRM_TRACE_07
#define GSY_PRM_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_07(TRACE_SUBSYS_GSY_PRM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
#endif
#ifndef GSY_PRM_TRACE_08
#define GSY_PRM_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_08(TRACE_SUBSYS_GSY_PRM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
#endif
#ifndef GSY_PRM_TRACE_09
#define GSY_PRM_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_09(TRACE_SUBSYS_GSY_PRM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_); }
#endif

#ifndef GSY_SYNC_TRACE_00
#define GSY_SYNC_TRACE_00(idx_, level_, msg_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_00(TRACE_SUBSYS_GSY_SYNC, level_, msg_); }
#endif
#ifndef GSY_SYNC_TRACE_01
#define GSY_SYNC_TRACE_01(idx_, level_, msg_, para1_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_01(TRACE_SUBSYS_GSY_SYNC, level_, msg_, para1_); }
#endif
#ifndef GSY_SYNC_TRACE_02
#define GSY_SYNC_TRACE_02(idx_, level_, msg_, para1_, para2_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_02(TRACE_SUBSYS_GSY_SYNC, level_, msg_, para1_, para2_); }
#endif
#ifndef GSY_SYNC_TRACE_03
#define GSY_SYNC_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_03(TRACE_SUBSYS_GSY_SYNC, level_, msg_, para1_, para2_, para3_); }
#endif
#ifndef GSY_SYNC_TRACE_04
#define GSY_SYNC_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_04(TRACE_SUBSYS_GSY_SYNC, level_, msg_, para1_, para2_, para3_, para4_); }
#endif
#ifndef GSY_SYNC_TRACE_05
#define GSY_SYNC_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_05(TRACE_SUBSYS_GSY_SYNC, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
#endif
#ifndef GSY_SYNC_TRACE_06
#define GSY_SYNC_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_06(TRACE_SUBSYS_GSY_SYNC, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
#endif
#ifndef GSY_SYNC_TRACE_07
#define GSY_SYNC_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_07(TRACE_SUBSYS_GSY_SYNC, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
#endif
#ifndef GSY_SYNC_TRACE_08
#define GSY_SYNC_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_08(TRACE_SUBSYS_GSY_SYNC, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
#endif
#ifndef GSY_SYNC_TRACE_09
#define GSY_SYNC_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_09(TRACE_SUBSYS_GSY_SYNC, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_); }
#endif
#ifndef GSY_SYNC_TRACE_10
#define GSY_SYNC_TRACE_10(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_10(TRACE_SUBSYS_GSY_SYNC, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_); }
#endif

#ifndef GSY_MASTER_TRACE_00
#define GSY_MASTER_TRACE_00(idx_, level_, msg_) { LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_00(TRACE_SUBSYS_GSY_MASTER, level_, msg_); }
#endif
#ifndef GSY_MASTER_TRACE_01
#define GSY_MASTER_TRACE_01(idx_, level_, msg_, para1_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_01(TRACE_SUBSYS_GSY_MASTER, level_, msg_, para1_); }
#endif
#ifndef GSY_MASTER_TRACE_02
#define GSY_MASTER_TRACE_02(idx_, level_, msg_, para1_, para2_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_02(TRACE_SUBSYS_GSY_MASTER, level_, msg_, para1_, para2_); }
#endif
#ifndef GSY_MASTER_TRACE_03
#define GSY_MASTER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) { LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_03(TRACE_SUBSYS_GSY_MASTER, level_, msg_, para1_, para2_, para3_); }
#endif
#ifndef GSY_MASTER_TRACE_04
#define GSY_MASTER_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) { LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_04(TRACE_SUBSYS_GSY_MASTER, level_, msg_, para1_, para2_, para3_, para4_); }
#endif
#ifndef GSY_MASTER_TRACE_05
#define GSY_MASTER_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_05(TRACE_SUBSYS_GSY_MASTER, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
#endif
#ifndef GSY_MASTER_TRACE_06
#define GSY_MASTER_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_06(TRACE_SUBSYS_GSY_MASTER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
#endif
#ifndef GSY_MASTER_TRACE_07
#define GSY_MASTER_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_07(TRACE_SUBSYS_GSY_MASTER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
#endif
#ifndef GSY_MASTER_TRACE_08
#define GSY_MASTER_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_08(TRACE_SUBSYS_GSY_MASTER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
#endif
#ifndef GSY_MASTER_TRACE_09
#define GSY_MASTER_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_09(TRACE_SUBSYS_GSY_MASTER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_); }
#endif

#ifndef GSY_SNDRCV_TRACE_BYTE_ARRAY
#define GSY_SNDRCV_TRACE_BYTE_ARRAY(idx_, level_, msg_, ptr_, len_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_BYTE_ARRAY(TRACE_SUBSYS_GSY_SNDRCV, level_, msg_, ptr_, len_); }
#endif
#ifndef GSY_ERROR_TRACE_BYTE_ARRAY
#define GSY_ERROR_TRACE_BYTE_ARRAY(idx_, level_, msg_, ptr_, len_)	{ LSA_UNUSED_ARG(idx_) \
		LSA_TRACE_BYTE_ARRAY(TRACE_SUBSYS_GSY_ERROR, level_, msg_, ptr_, len_); }
#endif

/*===================================================================================================================*/
/*===================================================================================================================*/
/*====   LLdp   ======================================================================================================*/
/*===================================================================================================================*/
/*===================================================================================================================*/

#ifndef LLDP_FUNCTION_TRACE_00
#define LLDP_FUNCTION_TRACE_00(idx_,level_, msg_) { LSA_UNUSED_ARG(idx_) \
	LSA_TRACE_00(TRACE_SUBSYS_LLDP_FUNCTION, level_, msg_); }
#endif

/*===================================================================================================================*/
/*===================================================================================================================*/
/*====   MRP   ======================================================================================================*/
/*===================================================================================================================*/
/*===================================================================================================================*/

#ifndef MRP_UPPER_TRACE_00
#define MRP_UPPER_TRACE_00(idx_, level_, msg_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_00(TRACE_SUBSYS_MRP_UPPER, level_, msg_); }
#endif

#ifndef MRP_UPPER_TRACE_01
#define MRP_UPPER_TRACE_01(idx_, level_, msg_, para1_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_01(TRACE_SUBSYS_MRP_UPPER, level_, msg_, para1_); }
#endif

#ifndef MRP_UPPER_TRACE_02
#define MRP_UPPER_TRACE_02(idx_, level_, msg_, para1_, para2_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_02(TRACE_SUBSYS_MRP_UPPER, level_, msg_, para1_, para2_); }
#endif

#ifndef MRP_UPPER_TRACE_03
#define MRP_UPPER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_03(TRACE_SUBSYS_MRP_UPPER, level_, msg_, para1_, para2_, para3_); }
#endif

#ifndef MRP_UPPER_TRACE_04
#define MRP_UPPER_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_04(TRACE_SUBSYS_MRP_UPPER, level_, msg_, para1_, para2_, para3_, para4_); }
#endif

#ifndef MRP_UPPER_TRACE_05
#define MRP_UPPER_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_05(TRACE_SUBSYS_MRP_UPPER, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
#endif

#ifndef MRP_UPPER_TRACE_06
#define MRP_UPPER_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_06(TRACE_SUBSYS_MRP_UPPER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
#endif

#ifndef MRP_UPPER_TRACE_07
#define MRP_UPPER_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) { LSA_UNUSED_ARG(idx_) \
  LSA_TRACE_07(TRACE_SUBSYS_MRP_UPPER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
#endif

#ifndef MRP_UPPER_TRACE_08
#define MRP_UPPER_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) { LSA_UNUSED_ARG(idx_) \
  LSA_TRACE_08(TRACE_SUBSYS_MRP_UPPER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
#endif

/*===========================================================================*/

#ifndef MRP_LOWER_TRACE_00
#define MRP_LOWER_TRACE_00(idx_, level_, msg_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_00(TRACE_SUBSYS_MRP_LOWER, level_, msg_); }
#endif

#ifndef MRP_LOWER_TRACE_01
#define MRP_LOWER_TRACE_01(idx_, level_, msg_, para1_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_01(TRACE_SUBSYS_MRP_LOWER, level_, msg_, para1_); }
#endif

#ifndef MRP_LOWER_TRACE_02
#define MRP_LOWER_TRACE_02(idx_, level_, msg_, para1_, para2_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_02(TRACE_SUBSYS_MRP_LOWER, level_, msg_, para1_, para2_); }
#endif

#ifndef MRP_LOWER_TRACE_03
#define MRP_LOWER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_03(TRACE_SUBSYS_MRP_LOWER, level_, msg_, para1_, para2_, para3_); }
#endif

#ifndef MRP_LOWER_TRACE_04
#define MRP_LOWER_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_04(TRACE_SUBSYS_MRP_LOWER, level_, msg_, para1_, para2_, para3_, para4_); }
#endif

#ifndef MRP_LOWER_TRACE_05
#define MRP_LOWER_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) { LSA_UNUSED_ARG(idx_) \
  LSA_TRACE_05(TRACE_SUBSYS_MRP_LOWER, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
#endif

#ifndef MRP_LOWER_TRACE_06
#define MRP_LOWER_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) { LSA_UNUSED_ARG(idx_) \
  LSA_TRACE_06(TRACE_SUBSYS_MRP_LOWER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
#endif

#ifndef MRP_LOWER_TRACE_07
#define MRP_LOWER_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_07(TRACE_SUBSYS_MRP_LOWER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
#endif

#ifndef MRP_LOWER_TRACE_08
#define MRP_LOWER_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) { LSA_UNUSED_ARG(idx_) \
  LSA_TRACE_08(TRACE_SUBSYS_MRP_LOWER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
#endif

/*===========================================================================*/

#ifndef MRP_SYSTEM_TRACE_00
#define MRP_SYSTEM_TRACE_00(idx_, level_, msg_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_00(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_); }
#endif

#ifndef MRP_SYSTEM_TRACE_01
#define MRP_SYSTEM_TRACE_01(idx_, level_, msg_, para1_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_01(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_, para1_); }
#endif

#ifndef MRP_SYSTEM_TRACE_02
#define MRP_SYSTEM_TRACE_02(idx_, level_, msg_, para1_, para2_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_02(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_, para1_, para2_); }
#endif

#ifndef MRP_SYSTEM_TRACE_03
#define MRP_SYSTEM_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_03(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_, para1_, para2_, para3_); }
#endif

#ifndef MRP_SYSTEM_TRACE_04
#define MRP_SYSTEM_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) { LSA_UNUSED_ARG(idx_) \
  LSA_TRACE_04(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_, para1_, para2_, para3_, para4_); }
#endif

#ifndef MRP_SYSTEM_TRACE_05
#define MRP_SYSTEM_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) { LSA_UNUSED_ARG(idx_) \
  LSA_TRACE_05(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
#endif

#ifndef MRP_SYSTEM_TRACE_06
#define MRP_SYSTEM_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) { LSA_UNUSED_ARG(idx_) \
  LSA_TRACE_06(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
#endif

#ifndef MRP_SYSTEM_TRACE_07
#define MRP_SYSTEM_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) { LSA_UNUSED_ARG(idx_) \
  LSA_TRACE_07(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
#endif

#ifndef MRP_SYSTEM_TRACE_08
#define MRP_SYSTEM_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) { LSA_UNUSED_ARG(idx_) \
  LSA_TRACE_08(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
#endif

/*===========================================================================*/

#ifndef MRP_PROGRAM_TRACE_00
#define MRP_PROGRAM_TRACE_00(idx_, level_, msg_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_00(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_); }
#endif

#ifndef MRP_PROGRAM_TRACE_01
#define MRP_PROGRAM_TRACE_01(idx_, level_, msg_, para1_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_01(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_, para1_); }
#endif

#ifndef MRP_PROGRAM_TRACE_02
#define MRP_PROGRAM_TRACE_02(idx_, level_, msg_, para1_, para2_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_02(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_, para1_, para2_); }
#endif

#ifndef MRP_PROGRAM_TRACE_03
#define MRP_PROGRAM_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_03(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_, para1_, para2_, para3_); }
#endif

#ifndef MRP_PROGRAM_TRACE_04
#define MRP_PROGRAM_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_04(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_, para1_, para2_, para3_, para4_); }
#endif

#ifndef MRP_PROGRAM_TRACE_05
#define MRP_PROGRAM_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_05(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
#endif

#ifndef MRP_PROGRAM_TRACE_06
#define MRP_PROGRAM_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_06(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
#endif

#ifndef MRP_PROGRAM_TRACE_07
#define MRP_PROGRAM_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) { LSA_UNUSED_ARG(idx_) \
    LSA_TRACE_07(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
#endif

#ifndef MRP_PROGRAM_TRACE_08
#define MRP_PROGRAM_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) { LSA_UNUSED_ARG(idx_) \
  LSA_TRACE_08(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
#endif


/*===================================================================================================================*/
/*===================================================================================================================*/
/*====   NARE   =====================================================================================================*/
/*===================================================================================================================*/
/*===================================================================================================================*/

#ifndef NARE_UPPER_TRACE_00
#define NARE_UPPER_TRACE_00(idx_, level_, msg_) { LSA_UNUSED_ARG(idx_) \
LSA_TRACE_00(TRACE_SUBSYS_NARE_UPPER, level_, msg_); }
#endif

#ifndef NARE_UPPER_TRACE_01
#define NARE_UPPER_TRACE_01(idx_, level_, msg_, para1_) { LSA_UNUSED_ARG(idx_) \
LSA_TRACE_01(TRACE_SUBSYS_NARE_UPPER, level_, msg_, para1_); }
#endif

#ifndef NARE_UPPER_TRACE_02
#define NARE_UPPER_TRACE_02(idx_, level_, msg_, para1_, para2_) { LSA_UNUSED_ARG(idx_) \
LSA_TRACE_02(TRACE_SUBSYS_NARE_UPPER, level_, msg_, para1_, para2_); }
#endif

#ifndef NARE_UPPER_TRACE_03
#define NARE_UPPER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) { LSA_UNUSED_ARG(idx_) \
LSA_TRACE_03(TRACE_SUBSYS_NARE_UPPER, level_, msg_, para1_, para2_, para3_); }
#endif

#ifndef NARE_UPPER_TRACE_04
#define NARE_UPPER_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) { LSA_UNUSED_ARG(idx_) \
LSA_TRACE_04(TRACE_SUBSYS_NARE_UPPER, level_, msg_, para1_, para2_, para3_, para4_); }
#endif

#ifndef NARE_LOWER_TRACE_00
#define NARE_LOWER_TRACE_00(idx_, level_, msg_) { LSA_UNUSED_ARG(idx_) \
LSA_TRACE_00(TRACE_SUBSYS_NARE_LOWER, level_, msg_); }
#endif

#ifndef NARE_LOWER_TRACE_01
#define NARE_LOWER_TRACE_01(idx_, level_, msg_, para1_) { LSA_UNUSED_ARG(idx_) \
LSA_TRACE_01(TRACE_SUBSYS_NARE_LOWER, level_, msg_, para1_); }
#endif

#ifndef NARE_LOWER_TRACE_02
#define NARE_LOWER_TRACE_02(idx_, level_, msg_, para1_, para2_) { LSA_UNUSED_ARG(idx_) \
LSA_TRACE_02(TRACE_SUBSYS_NARE_LOWER, level_, msg_, para1_, para2_); }
#endif

#ifndef NARE_LOWER_TRACE_03
#define NARE_LOWER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) { LSA_UNUSED_ARG(idx_) \
LSA_TRACE_03(TRACE_SUBSYS_NARE_LOWER, level_, msg_, para1_, para2_, para3_); }
#endif

#ifndef NARE_SYSTEM_TRACE_00
#define NARE_SYSTEM_TRACE_00(idx_, level_, msg_) { LSA_UNUSED_ARG(idx_) \
LSA_TRACE_00(TRACE_SUBSYS_NARE_SYSTEM, level_, msg_); }
#endif

#ifndef NARE_SYSTEM_TRACE_01
#define NARE_SYSTEM_TRACE_01(idx_, level_, msg_, para1_) { LSA_UNUSED_ARG(idx_) \
LSA_TRACE_01(TRACE_SUBSYS_NARE_SYSTEM, level_, msg_, para1_); }
#endif

#ifndef NARE_SYSTEM_TRACE_02
#define NARE_SYSTEM_TRACE_02(idx_, level_, msg_, para1_, para2_) { LSA_UNUSED_ARG(idx_) \
LSA_TRACE_02(TRACE_SUBSYS_NARE_SYSTEM, level_, msg_, para1_, para2_); }
#endif

#ifndef NARE_SYSTEM_TRACE_03
#define NARE_SYSTEM_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) { LSA_UNUSED_ARG(idx_) \
LSA_TRACE_03(TRACE_SUBSYS_NARE_SYSTEM, level_, msg_, para1_, para2_, para3_); }
#endif

#ifndef NARE_FUNCTION_TRACE_00
#define NARE_FUNCTION_TRACE_00(idx_, level_, msg_) { LSA_UNUSED_ARG(idx_) \
LSA_TRACE_00(TRACE_SUBSYS_NARE_FUNCTION, level_, msg_); }
#endif

#ifndef NARE_FUNCTION_TRACE_01
#define NARE_FUNCTION_TRACE_01(idx_, level_, msg_, para1_) { LSA_UNUSED_ARG(idx_) \
LSA_TRACE_01(TRACE_SUBSYS_NARE_FUNCTION, level_, msg_, para1_); }
#endif

#ifndef NARE_FUNCTION_TRACE_02
#define NARE_FUNCTION_TRACE_02(idx_, level_, msg_, para1_, para2_) { LSA_UNUSED_ARG(idx_) \
LSA_TRACE_02(TRACE_SUBSYS_NARE_FUNCTION, level_, msg_, para1_, para2_); }
#endif

#ifndef NARE_FUNCTION_TRACE_03
#define NARE_FUNCTION_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) { LSA_UNUSED_ARG(idx_) \
LSA_TRACE_03(TRACE_SUBSYS_NARE_FUNCTION, level_, msg_, para1_, para2_, para3_); }
#endif

#ifndef NARE_PROGRAM_TRACE_00
#define NARE_PROGRAM_TRACE_00(idx_, level_, msg_) { LSA_UNUSED_ARG(idx_) \
LSA_TRACE_00(TRACE_SUBSYS_NARE_PROGRAM, level_, msg_); }
#endif

#ifndef NARE_PROGRAM_TRACE_01
#define NARE_PROGRAM_TRACE_01(idx_, level_, msg_, para1_) { LSA_UNUSED_ARG(idx_) \
LSA_TRACE_01(TRACE_SUBSYS_NARE_PROGRAM, level_, msg_, para1_); }
#endif

#ifndef NARE_PROGRAM_TRACE_02
#define NARE_PROGRAM_TRACE_02(idx_, level_, msg_, para1_, para2_) { LSA_UNUSED_ARG(idx_) \
LSA_TRACE_02(TRACE_SUBSYS_NARE_PROGRAM, level_, msg_, para1_, para2_); }
#endif

#ifndef NARE_PROGRAM_TRACE_03
#define NARE_PROGRAM_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) { LSA_UNUSED_ARG(idx_) \
LSA_TRACE_03(TRACE_SUBSYS_NARE_PROGRAM, level_, msg_, para1_, para2_, para3_); }
#endif

#ifndef NARE_PROGRAM_TRACE_04
#define NARE_PROGRAM_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) { LSA_UNUSED_ARG(idx_) \
LSA_TRACE_04(TRACE_SUBSYS_NARE_PROGRAM, level_, msg_, para1_, para2_, para3_, para4_); }
#endif

#ifndef NARE_PROGRAM_TRACE_05
#define NARE_PROGRAM_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) { LSA_UNUSED_ARG(idx_) \
LSA_TRACE_05(TRACE_SUBSYS_NARE_PROGRAM, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
#endif

#ifndef NARE_PROGRAM_TRACE_06
#define NARE_PROGRAM_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) { LSA_UNUSED_ARG(idx_) \
LSA_TRACE_06(TRACE_SUBSYS_NARE_PROGRAM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
#endif

#ifndef NARE_PROGRAM_TRACE_08
#define NARE_PROGRAM_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) { LSA_UNUSED_ARG(idx_) \
LSA_TRACE_08(TRACE_SUBSYS_NARE_PROGRAM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
#endif

/*===================================================================================================================*/




/*---------------------------------------------------------------------------*/
#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif  /* of LTRC_IF_H */


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
