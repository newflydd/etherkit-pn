#ifndef TCIP_TRC_H                     /* ----- reinclude-protection ----- */
#define TCIP_TRC_H

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
/*  C o m p o n e n t     &C: TCPIP (TCIP for Interniche Stack)         :C&  */
/*                                                                           */
/*  F i l e               &F: tcip_trc.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  System interface for lsa-trace                                           */
/*                                                                           */
/*  See "Trace_fuer_LSA-Entwickler_akt.doc" in LSA_LTRC for more information */
/*                                                                           */
/*****************************************************************************/

/*
 *  TCIP uses the sub-systems:
 *    TRACE_SUBSYS_TCIP_UPPER      (UPPER Common)
 *    TRACE_SUBSYS_TCIP_MEM        (MEMORY)
 *    TRACE_SUBSYS_TCIP_INICHE     (InterNiche stack)
 *    note: the subsystems of LTRC are defined per enum
 */

/*
 *  LSA_TRACE_LEVEL_FATAL        Serious not tradable error occurred.
 *  LSA_TRACE_LEVEL_ERROR        Serious tradable error occurred.
 *  LSA_TRACE_LEVEL_UNEXP        Unexpected situation occurred.
 *  LSA_TRACE_LEVEL_WARN         Warning: Program exits productive branch.
 *
 *  LSA_TRACE_LEVEL_NOTE_HIGH    Important information productive case: High level.
 *  LSA_TRACE_LEVEL_NOTE         Important information productive case: Normal level.
 *  LSA_TRACE_LEVEL_NOTE_LOW     Important information productive case: Low level.
 *  LSA_TRACE_LEVEL_CHAT         Important code places within the software.
 */

/*=============================================================================
 * macro names:   TCIP_YYYYY_TRACE_XX  (XX = 00, 01, 02, 03, ..., 16)
 *
 * macros:        Trace entry of 0, 1, 2, ... or 16 parameters for the
 *                subsystem "YYYYY".
 *
 *                The system integrator must define the identifier
 *                TRACE_SUBSYS_TCIP_YYYYY (if LTRC is used in the file
 *                ltrc_sub.h/txt or if LTRC isn't used in a system file)!
 *
 * parameters:    level_:   LSA_TRACE_LEVEL_OFF, LSA_TRACE_LEVEL_FATAL,
 *                          LSA_TRACE_LEVEL_ERROR, LSA_TRACE_LEVEL_UNEXP,
 *                          LSA_TRACE_LEVEL_WARN, LSA_TRACE_LEVEL_NOTE_HIGH,
 *                          LSA_TRACE_LEVEL_NOTE, LSA_TRACE_LEVEL_NOTE_LOW or
 *                          LSA_TRACE_LEVEL_CHAT
 *                msg_:     string similar to "printf"
 *                para1_:   parameter 1
 *                para2_:   parameter 2
 *                 ...        ...
 *                para16_:  paramater 16
 *
 * return value:  LSA_VOID
 *===========================================================================*/


/*----------------------------------------------------------------------------*/
#if TCIP_CFG_TRACE_MODE == 0 /* no trace */

#define TCIP_UPPER_TRACE_00(idx_, level_, msg_)
#define TCIP_UPPER_TRACE_01(idx_, level_, msg_, para1_)
#define TCIP_UPPER_TRACE_02(idx_, level_, msg_, para1_, para2_)
#define TCIP_UPPER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)
#define TCIP_UPPER_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)
#define TCIP_UPPER_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)
#define TCIP_UPPER_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)
#define TCIP_UPPER_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)
#define TCIP_UPPER_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)

#define TCIP_MEM_TRACE_00(idx_, level_, msg_)
#define TCIP_MEM_TRACE_01(idx_, level_, msg_, para1_)
#define TCIP_MEM_TRACE_02(idx_, level_, msg_, para1_, para2_)
#define TCIP_MEM_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)

#define TCIP_INICHE_TRACE_00(idx_, level_, msg_)
#define TCIP_INICHE_TRACE_01(idx_, level_, msg_, para1_)
#define TCIP_INICHE_TRACE_02(idx_, level_, msg_, para1_, para2_)
#define TCIP_INICHE_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)

#define TCIP_OBSD_TRACE_00_EXT(idx_, level_, msg_, module_, line_)
#define TCIP_OBSD_TRACE_01_EXT(idx_, level_, msg_, module_, line_, para1_)
#define TCIP_OBSD_TRACE_02_EXT(idx_, level_, msg_, module_, line_, para1_, para2_)
#define TCIP_OBSD_TRACE_03_EXT(idx_, level_, msg_, module_, line_, para1_, para2_, para3_)
#define TCIP_OBSD_TRACE_04_EXT(idx_, level_, msg_, module_, line_, para1_, para2_, para3_, para4_)
#define TCIP_OBSD_TRACE_05_EXT(idx_, level_, msg_, module_, line_, para1_, para2_, para3_, para4_, para5_)
#define TCIP_OBSD_TRACE_06_EXT(idx_, level_, msg_, module_, line_, para1_, para2_, para3_, para4_, para5_, para6_)
#define TCIP_OBSD_TRACE_07_EXT(idx_, level_, msg_, module_, line_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)
#define TCIP_OBSD_TRACE_08_EXT(idx_, level_, msg_, module_, line_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)
#define TCIP_OBSD_TRACE_09_EXT(idx_, level_, msg_, module_, line_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)
#define TCIP_OBSD_TRACE_10_EXT(idx_, level_, msg_, module_, line_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_)
#define TCIP_OBSD_TRACE_11_EXT(idx_, level_, msg_, module_, line_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_)
#define TCIP_OBSD_TRACE_12_EXT(idx_, level_, msg_, module_, line_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_)
#define TCIP_OBSD_TRACE_13_EXT(idx_, level_, msg_, module_, line_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_)
#define TCIP_OBSD_TRACE_14_EXT(idx_, level_, msg_, module_, line_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_)
#define TCIP_OBSD_TRACE_15_EXT(idx_, level_, msg_, module_, line_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_)
#define TCIP_OBSD_TRACE_16_EXT(idx_, level_, msg_, module_, line_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_, para16_)

/*----------------------------------------------------------------------------*/
#elif TCIP_CFG_TRACE_MODE == 1 /* LSA trace */

#ifndef TCIP_UPPER_TRACE_00
#define TCIP_UPPER_TRACE_00(idx_, level_, msg_) \
	LSA_TRACE_00(TRACE_SUBSYS_TCIP_UPPER, level_, msg_)
#endif

#ifndef TCIP_UPPER_TRACE_01
#define TCIP_UPPER_TRACE_01(idx_, level_, msg_, para1_) \
	LSA_TRACE_01(TRACE_SUBSYS_TCIP_UPPER, level_, msg_, para1_)
#endif

#ifndef TCIP_UPPER_TRACE_02
#define TCIP_UPPER_TRACE_02(idx_, level_, msg_, para1_, para2_) \
	LSA_TRACE_02(TRACE_SUBSYS_TCIP_UPPER, level_, msg_, para1_, para2_)
#endif

#ifndef TCIP_UPPER_TRACE_03
#define TCIP_UPPER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) \
	LSA_TRACE_03(TRACE_SUBSYS_TCIP_UPPER, level_, msg_, para1_, para2_, para3_)
#endif

#ifndef TCIP_UPPER_TRACE_04
#define TCIP_UPPER_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) \
	LSA_TRACE_04(TRACE_SUBSYS_TCIP_UPPER, level_, msg_, para1_, para2_, para3_, para4_)
#endif

#ifndef TCIP_UPPER_TRACE_05
#define TCIP_UPPER_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) \
	LSA_TRACE_05(TRACE_SUBSYS_TCIP_UPPER, level_, msg_, para1_, para2_, para3_, para4_, para5_)
#endif

#ifndef TCIP_UPPER_TRACE_06
#define TCIP_UPPER_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) \
	LSA_TRACE_06(TRACE_SUBSYS_TCIP_UPPER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)
#endif

#ifndef TCIP_UPPER_TRACE_07
#define TCIP_UPPER_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) \
	LSA_TRACE_07(TRACE_SUBSYS_TCIP_UPPER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)
#endif

#ifndef TCIP_UPPER_TRACE_08
#define TCIP_UPPER_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) \
	LSA_TRACE_08(TRACE_SUBSYS_TCIP_UPPER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)
#endif

#ifndef TCIP_UPPER_TRACE_09
#define TCIP_UPPER_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_) \
	LSA_TRACE_09(TRACE_SUBSYS_TCIP_UPPER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)
#endif

/*----------------------------------------------------------------------------*/

#ifndef TCIP_MEM_TRACE_00
#define TCIP_MEM_TRACE_00(idx_, level_, msg_) \
	LSA_TRACE_00(TRACE_SUBSYS_TCIP_MEM, level_, msg_)
#endif

#ifndef TCIP_MEM_TRACE_01
#define TCIP_MEM_TRACE_01(idx_, level_, msg_, para1_) \
	LSA_TRACE_01(TRACE_SUBSYS_TCIP_MEM, level_, msg_, para1_)
#endif

#ifndef TCIP_MEM_TRACE_02
#define TCIP_MEM_TRACE_02(idx_, level_, msg_, para1_, para2_) \
	LSA_TRACE_02(TRACE_SUBSYS_TCIP_MEM, level_, msg_, para1_, para2_)
#endif

#ifndef TCIP_MEM_TRACE_03
#define TCIP_MEM_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) \
	LSA_TRACE_03(TRACE_SUBSYS_TCIP_MEM, level_, msg_, para1_, para2_, para3_)
#endif

/*----------------------------------------------------------------------------*/

#ifndef TCIP_INICHE_TRACE_00
#define TCIP_INICHE_TRACE_00(idx_, level_, msg_) \
	LSA_TRACE_00(TRACE_SUBSYS_TCIP_INICHE, level_, msg_)
#endif

#ifndef TCIP_INICHE_TRACE_01
#define TCIP_INICHE_TRACE_01(idx_, level_, msg_, para1_) \
	LSA_TRACE_01(TRACE_SUBSYS_TCIP_INICHE, level_, msg_, para1_)
#endif

#ifndef TCIP_INICHE_TRACE_02
#define TCIP_INICHE_TRACE_02(idx_, level_, msg_, para1_, para2_) \
	LSA_TRACE_02(TRACE_SUBSYS_TCIP_INICHE, level_, msg_, para1_, para2_)
#endif

#ifndef TCIP_INICHE_TRACE_03
#define TCIP_INICHE_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) \
	LSA_TRACE_03(TRACE_SUBSYS_TCIP_INICHE, level_, msg_, para1_, para2_, para3_)
#endif

#ifndef TCIP_INICHE_TRACE_04
#define TCIP_INICHE_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) \
	LSA_TRACE_04(TRACE_SUBSYS_TCIP_INICHE, level_, msg_, para1_, para2_, para3_, para4_)
#endif

#ifndef TCIP_INICHE_TRACE_05
#define TCIP_INICHE_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) \
	LSA_TRACE_05(TRACE_SUBSYS_TCIP_INICHE, level_, msg_, para1_, para2_, para3_, para4_, para5_)
#endif

#ifndef TCIP_INICHE_TRACE_06
#define TCIP_INICHE_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) \
	LSA_TRACE_06(TRACE_SUBSYS_TCIP_INICHE, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)
#endif

#ifndef TCIP_INICHE_TRACE_07
#define TCIP_INICHE_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) \
	LSA_TRACE_07(TRACE_SUBSYS_TCIP_INICHE, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)
#endif

#ifndef TCIP_INICHE_TRACE_08
#define TCIP_INICHE_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) \
	LSA_TRACE_08(TRACE_SUBSYS_TCIP_INICHE, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)
#endif

/*----------------------------------------------------------------------------*/

#ifndef TCIP_OBSD_TRACE_00_EXT
#define TCIP_OBSD_TRACE_00_EXT(idx_, level_, module_, line_, msg_) \
	LSA_TRACE_00_EXT(TRACE_SUBSYS_TCIP_OBSD, level_, module_, line_, msg_)
#endif

#ifndef TCIP_OBSD_TRACE_01_EXT
#define TCIP_OBSD_TRACE_01_EXT(idx_, level_, module_, line_, msg_, para1_) \
	LSA_TRACE_01_EXT(TRACE_SUBSYS_TCIP_OBSD, level_, module_, line_, msg_, para1_)
#endif

#ifndef TCIP_OBSD_TRACE_02_EXT
#define TCIP_OBSD_TRACE_02_EXT(idx_, level_, module_, line_, msg_, para1_, para2_) \
	LSA_TRACE_02_EXT(TRACE_SUBSYS_TCIP_OBSD, level_, module_, line_, msg_, para1_, para2_)
#endif

#ifndef TCIP_OBSD_TRACE_03_EXT
#define TCIP_OBSD_TRACE_03_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_) \
	LSA_TRACE_03_EXT(TRACE_SUBSYS_TCIP_OBSD, level_, module_, line_, msg_, para1_, para2_, para3_)
#endif

#ifndef TCIP_OBSD_TRACE_04_EXT
#define TCIP_OBSD_TRACE_04_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_) \
	LSA_TRACE_04_EXT(TRACE_SUBSYS_TCIP_OBSD, level_, module_, line_, msg_, para1_, para2_, para3_, para4_)
#endif

#ifndef TCIP_OBSD_TRACE_05_EXT
#define TCIP_OBSD_TRACE_05_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_) \
	LSA_TRACE_05_EXT(TRACE_SUBSYS_TCIP_OBSD, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_)
#endif

#ifndef TCIP_OBSD_TRACE_06_EXT
#define TCIP_OBSD_TRACE_06_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) \
	LSA_TRACE_06_EXT(TRACE_SUBSYS_TCIP_OBSD, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)
#endif

#ifndef TCIP_OBSD_TRACE_07_EXT
#define TCIP_OBSD_TRACE_07_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) \
	LSA_TRACE_07_EXT(TRACE_SUBSYS_TCIP_OBSD, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)
#endif

#ifndef TCIP_OBSD_TRACE_08_EXT
#define TCIP_OBSD_TRACE_08_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) \
	LSA_TRACE_08_EXT(TRACE_SUBSYS_TCIP_OBSD, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)
#endif

#ifndef TCIP_OBSD_TRACE_09_EXT
#define TCIP_OBSD_TRACE_09_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_) \
	LSA_TRACE_09_EXT(TRACE_SUBSYS_TCIP_OBSD, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)
#endif

#ifndef TCIP_OBSD_TRACE_10_EXT
#define TCIP_OBSD_TRACE_10_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_) \
	LSA_TRACE_10_EXT(TRACE_SUBSYS_TCIP_OBSD, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_)
#endif

#ifndef TCIP_OBSD_TRACE_11_EXT
#define TCIP_OBSD_TRACE_11_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_) \
	LSA_TRACE_11_EXT(TRACE_SUBSYS_TCIP_OBSD, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_)
#endif

#ifndef TCIP_OBSD_TRACE_12_EXT
#define TCIP_OBSD_TRACE_12_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_) \
	LSA_TRACE_12_EXT(TRACE_SUBSYS_TCIP_OBSD, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_)
#endif

#ifndef TCIP_OBSD_TRACE_13_EXT
#define TCIP_OBSD_TRACE_13_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_) \
	LSA_TRACE_13_EXT(TRACE_SUBSYS_TCIP_OBSD, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_)
#endif

#ifndef TCIP_OBSD_TRACE_14_EXT
#define TCIP_OBSD_TRACE_14_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_) \
	LSA_TRACE_14_EXT(TRACE_SUBSYS_TCIP_OBSD, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_)
#endif

#ifndef TCIP_OBSD_TRACE_15_EXT
#define TCIP_OBSD_TRACE_15_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_) \
	LSA_TRACE_15_EXT(TRACE_SUBSYS_TCIP_OBSD, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_)
#endif

#ifndef TCIP_OBSD_TRACE_16_EXT
#define TCIP_OBSD_TRACE_16_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_, para16_) \
	LSA_TRACE_16_EXT(TRACE_SUBSYS_TCIP_OBSD, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_, para16_)
#endif

/*----------------------------------------------------------------------------*/

#elif TCIP_CFG_TRACE_MODE == 2 /* LSA index trace */

#ifndef TCIP_UPPER_TRACE_00
#define TCIP_UPPER_TRACE_00(idx_, level_, msg_) \
	LSA_IDX_TRACE_00(TRACE_SUBSYS_TCIP_UPPER, idx_, level_, msg_)
#endif

#ifndef TCIP_UPPER_TRACE_01
#define TCIP_UPPER_TRACE_01(idx_, level_, msg_, para1_) \
	LSA_IDX_TRACE_01(TRACE_SUBSYS_TCIP_UPPER, idx_, level_, msg_, para1_)
#endif

#ifndef TCIP_UPPER_TRACE_02
#define TCIP_UPPER_TRACE_02(idx_, level_, msg_, para1_, para2_) \
	LSA_IDX_TRACE_02(TRACE_SUBSYS_TCIP_UPPER, idx_, level_, msg_, para1_, para2_)
#endif

#ifndef TCIP_UPPER_TRACE_03
#define TCIP_UPPER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) \
	LSA_IDX_TRACE_03(TRACE_SUBSYS_TCIP_UPPER, idx_, level_, msg_, para1_, para2_, para3_)
#endif

#ifndef TCIP_UPPER_TRACE_04
#define TCIP_UPPER_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) \
	LSA_IDX_TRACE_04(TRACE_SUBSYS_TCIP_UPPER, idx_, level_, msg_, para1_, para2_, para3_, para4_)
#endif

#ifndef TCIP_UPPER_TRACE_05
#define TCIP_UPPER_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) \
	LSA_IDX_TRACE_05(TRACE_SUBSYS_TCIP_UPPER, idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)
#endif

#ifndef TCIP_UPPER_TRACE_06
#define TCIP_UPPER_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) \
	LSA_IDX_TRACE_06(TRACE_SUBSYS_TCIP_UPPER, idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)
#endif

#ifndef TCIP_UPPER_TRACE_07
#define TCIP_UPPER_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) \
	LSA_IDX_TRACE_07(TRACE_SUBSYS_TCIP_UPPER, idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)
#endif

#ifndef TCIP_UPPER_TRACE_08
#define TCIP_UPPER_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) \
	LSA_IDX_TRACE_08(TRACE_SUBSYS_TCIP_UPPER, idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)
#endif

#ifndef TCIP_UPPER_TRACE_09
#define TCIP_UPPER_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_) \
	LSA_IDX_TRACE_09(TRACE_SUBSYS_TCIP_UPPER, idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)
#endif

/*----------------------------------------------------------------------------*/

#ifndef TCIP_MEM_TRACE_00
#define TCIP_MEM_TRACE_00(idx_, level_, msg_) \
	LSA_IDX_TRACE_00(TRACE_SUBSYS_TCIP_MEM, idx_, level_, msg_)
#endif

#ifndef TCIP_MEM_TRACE_01
#define TCIP_MEM_TRACE_01(idx_, level_, msg_, para1_) \
	LSA_IDX_TRACE_01(TRACE_SUBSYS_TCIP_MEM, idx_, level_, msg_, para1_)
#endif

#ifndef TCIP_MEM_TRACE_02
#define TCIP_MEM_TRACE_02(idx_, level_, msg_, para1_, para2_) \
	LSA_IDX_TRACE_02(TRACE_SUBSYS_TCIP_MEM, idx_, level_, msg_, para1_, para2_)
#endif

#ifndef TCIP_MEM_TRACE_03
#define TCIP_MEM_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) \
	LSA_IDX_TRACE_03(TRACE_SUBSYS_TCIP_MEM, idx_, level_, msg_, para1_, para2_, para3_)
#endif

/*----------------------------------------------------------------------------*/

#ifndef TCIP_INICHE_TRACE_00
#define TCIP_INICHE_TRACE_00(idx_, level_, msg_) \
	LSA_IDX_TRACE_00(TRACE_SUBSYS_TCIP_INICHE, idx_, level_, msg_)
#endif

#ifndef TCIP_INICHE_TRACE_01
#define TCIP_INICHE_TRACE_01(idx_, level_, msg_, para1_) \
	LSA_IDX_TRACE_01(TRACE_SUBSYS_TCIP_INICHE, idx_, level_, msg_, para1_)
#endif

#ifndef TCIP_INICHE_TRACE_02
#define TCIP_INICHE_TRACE_02(idx_, level_, msg_, para1_, para2_) \
	LSA_IDX_TRACE_02(TRACE_SUBSYS_TCIP_INICHE, idx_, level_, msg_, para1_, para2_)
#endif

#ifndef TCIP_INICHE_TRACE_03
#define TCIP_INICHE_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) \
	LSA_IDX_TRACE_03(TRACE_SUBSYS_TCIP_INICHE, idx_, level_, msg_, para1_, para2_, para3_)
#endif

/*----------------------------------------------------------------------------*/

#ifndef TCIP_OBSD_TRACE_00_EXT
#define TCIP_OBSD_TRACE_00_EXT(idx_, level_, module_, line_, msg_) \
	LSA_IDX_TRACE_00_EXT(TRACE_SUBSYS_TCIP_OBSD, idx_, level_, module_, line_, msg_)
#endif

#ifndef TCIP_OBSD_TRACE_01_EXT
#define TCIP_OBSD_TRACE_01_EXT(idx_, level_, module_, line_, msg_, para1_) \
	LSA_IDX_TRACE_01_EXT(TRACE_SUBSYS_TCIP_OBSD, idx_, level_, module_, line_, msg_, para1_)
#endif

#ifndef TCIP_OBSD_TRACE_02_EXT
#define TCIP_OBSD_TRACE_02_EXT(idx_, level_, module_, line_, msg_, para1_, para2_) \
	LSA_IDX_TRACE_02_EXT(TRACE_SUBSYS_TCIP_OBSD, idx_, level_, module_, line_, msg_, para1_, para2_)
#endif

#ifndef TCIP_OBSD_TRACE_03_EXT
#define TCIP_OBSD_TRACE_03_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_) \
	LSA_IDX_TRACE_03_EXT(TRACE_SUBSYS_TCIP_OBSD, idx_, level_, module_, line_, msg_, para1_, para2_, para3_)
#endif

#ifndef TCIP_OBSD_TRACE_04_EXT
#define TCIP_OBSD_TRACE_04_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_) \
	LSA_IDX_TRACE_04_EXT(TRACE_SUBSYS_TCIP_OBSD, idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_)
#endif

#ifndef TCIP_OBSD_TRACE_05_EXT
#define TCIP_OBSD_TRACE_05_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_) \
	LSA_IDX_TRACE_05_EXT(TRACE_SUBSYS_TCIP_OBSD, idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_)
#endif

#ifndef TCIP_OBSD_TRACE_06_EXT
#define TCIP_OBSD_TRACE_06_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) \
	LSA_IDX_TRACE_06_EXT(TRACE_SUBSYS_TCIP_OBSD, idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)
#endif

#ifndef TCIP_OBSD_TRACE_07_EXT
#define TCIP_OBSD_TRACE_07_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) \
	LSA_IDX_TRACE_07_EXT(TRACE_SUBSYS_TCIP_OBSD, idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)
#endif

#ifndef TCIP_OBSD_TRACE_08_EXT
#define TCIP_OBSD_TRACE_08_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) \
	LSA_IDX_TRACE_08_EXT(TRACE_SUBSYS_TCIP_OBSD, idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)
#endif

#ifndef TCIP_OBSD_TRACE_09_EXT
#define TCIP_OBSD_TRACE_09_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_) \
	LSA_IDX_TRACE_09_EXT(TRACE_SUBSYS_TCIP_OBSD, idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)
#endif

#ifndef TCIP_OBSD_TRACE_10_EXT
#define TCIP_OBSD_TRACE_10_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_) \
	LSA_IDX_TRACE_10_EXT(TRACE_SUBSYS_TCIP_OBSD, idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_)
#endif

#ifndef TCIP_OBSD_TRACE_11_EXT
#define TCIP_OBSD_TRACE_11_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_) \
	LSA_IDX_TRACE_11_EXT(TRACE_SUBSYS_TCIP_OBSD, idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_)
#endif

#ifndef TCIP_OBSD_TRACE_12_EXT
#define TCIP_OBSD_TRACE_12_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_) \
	LSA_IDX_TRACE_12_EXT(TRACE_SUBSYS_TCIP_OBSD, idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_)
#endif

#ifndef TCIP_OBSD_TRACE_13_EXT
#define TCIP_OBSD_TRACE_13_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_) \
	LSA_IDX_TRACE_13_EXT(TRACE_SUBSYS_TCIP_OBSD, idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_)
#endif

#ifndef TCIP_OBSD_TRACE_14_EXT
#define TCIP_OBSD_TRACE_14_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_) \
	LSA_IDX_TRACE_14_EXT(TRACE_SUBSYS_TCIP_OBSD, idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_)
#endif

#ifndef TCIP_OBSD_TRACE_15_EXT
#define TCIP_OBSD_TRACE_15_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_) \
	LSA_IDX_TRACE_15_EXT(TRACE_SUBSYS_TCIP_OBSD, idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_)
#endif

#ifndef TCIP_OBSD_TRACE_16_EXT
#define TCIP_OBSD_TRACE_16_EXT(idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_, para16_) \
	LSA_IDX_TRACE_16_EXT(TRACE_SUBSYS_TCIP_OBSD, idx_, level_, module_, line_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_, para16_)
#endif

#else

#error "TCIP_CFG_TRACE_MODE has unknown value"

#endif


/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
#endif  /* of TCIP_TRC_H */
