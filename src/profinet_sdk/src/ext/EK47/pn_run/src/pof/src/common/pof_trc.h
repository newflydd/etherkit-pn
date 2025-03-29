#ifndef POF_TRC_H       //reinclude-protection
#define POF_TRC_H

#ifdef __cplusplus      //If C++ - compiler: Use C linkage
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
/*  C o m p o n e n t     &C: POF (Polymer Optical Fiber)               :C&  */
/*                                                                           */
/*  F i l e               &F: pof_trc.h                                 :F&  */
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
/*  Defines lsa-trace macros for POF.                                        */
/*                                                                           */
/*      Here the macros are adapted to the lsa component LTRC.               */
/*                                                                           */
/*      If LTRC shall not be used, then either the LSA_TRACE macros used     */
/*      here must be defined by the system in the file lsa_cfg.h/txt or      */
/*      directly the POF-trace macro must be redefined on the desired        */
/*      function/macro in the pof_cfg.h/txt file.                            */
/*      If the system isn't able to store trace data then an "empty          */
/*      definition" must be implemented in file pof_cfg.h/txt (or the        */
/*      LSA_TRACE macros must be defined "empty" in lsa_cfg.h/txt).          */
/*                                                                           */
/*****************************************************************************/


/*****************************************************************************/
/* system interface output macros
 *
 * Note: There aren't any prototypes for these macros so that the target system
 * defines the pointer size and not the POF. Prefix doesn't cast the
 * parameters at the call.
 * A function can be nevertheless invoked within the macro.
 *
 * The subsystem YYYYY (see below) must be defined by the LSA component
 * developer. It can be =
 *    [empty]:   If this component uses only one subsystem.
 *    UPPER:     All requirements of the upper user are written into this
 *               subsystem.
 *               Calls of the user are marked with                   >>>
 *               Confirmations to the user are marked with           <<<
 *    LOWER:     All requirements to the lower layer are written into this
 *               subsystem.
 *               Requests to the lower layer are marked with         >>>
 *               Confirmations from the lower layer are marked with  <<<
 *    SYSTEM:    All other requirements to / from the system are written into
 *               this subsystem. For example: Timers, path information, copying
 *               processes of COMMON memory, ....
 *               For functions which the system invokes:
 *               Calls of the system are marked with                 -->
 *               The end of the functions are marked with            <--
 *    FUNCTION:  (Internal) function call trace.
 *    PROGRAM:   Subsystem for Programm flow and errors (general part of POF) - rest
 */

/*==================================================================================*/
/* Usage of traces within POF:                                                      */
/*                                                                                  */
/* Trace-Subsystem usage:                                                           */
/*                                                                                  */
/* TRACE_SUBSYS_POF_UPPER    :    Upper-Interface calls/response                    */
/* TRACE_SUBSYS_POF_LOWER    :    Lower-Interface calls/response                    */
/* TRACE_SUBSYS_POF_SYSTEM   :    System-Interface calls/response                   */
/* TRACE_SUBSYS_POF_FUNCTION :    Function In/Out-Calls                             */
/* TRACE_SUBSYS_POF_PROGRAM  :    Program flow and errors                           */
/*                                                                                  */
/* Trace-Level usage:                                                               */
/*                                                                                  */
/* LSA_TRACE_LEVEL_FATAL        Serious not tradable error occurred.                */
/* LSA_TRACE_LEVEL_ERROR        Serious tradable error occurred.                    */
/* LSA_TRACE_LEVEL_UNEXP        Unexpected situation occurred.                      */
/* LSA_TRACE_LEVEL_WARN         Warning: Program exits productive branch.           */
/*                                                                                  */
/* LSA_TRACE_LEVEL_NOTE_HIGH    Important information productive case: High level.  */
/* LSA_TRACE_LEVEL_NOTE         Important information productive case: Normal level.*/
/* LSA_TRACE_LEVEL_NOTE_LOW     Important information productive case: Low level.   */
/* LSA_TRACE_LEVEL_CHAT         Important code places within the software.          */
/*                                                                                  */
/*==================================================================================*/

/*=============================================================================
 * macro names:   POF_YYYYY_TRACE_XX  (XX = 00, 01, 02, 03, ..., 16)
 *
 * macros:        Trace entry of 0, 1, 2, ... or 16 parameters for the
 *                subsystem "YYYYY".
 *
 *                The system integrator must define the identifier
 *                TRACE_SUBSYS_POF_YYYYY (if LTRC is used in the file
 *                ltrc_sub.h/txt or if LTRC isn't used in a system file)!
 *
 * parameters:    level_:   LSA_TRACE_LEVEL_OFF,   LSA_TRACE_LEVEL_FATAL,
 *                          LSA_TRACE_LEVEL_ERROR, LSA_TRACE_LEVEL_UNEXP,
 *                          LSA_TRACE_LEVEL_WARN,  LSA_TRACE_LEVEL_NOTE_HIGH,
 *                          LSA_TRACE_LEVEL_NOTE,  LSA_TRACE_LEVEL_NOTE_LOW or
 *                          LSA_TRACE_LEVEL_CHAT
 *                msg_:     string similar to "printf"
 *                para1_:   parameter 1
 *                para2_:   parameter 2
 *                 ...        ...
 *                para16_:  paramater 16
 *
 * return value:  LSA_VOID
 *===========================================================================*/

/*****************************************************************************/

#if !defined (POF_CFG_TRACE_MODE)
#error "POF_CFG_TRACE_MODE not defined in pof_cfg.h!"
#elif (POF_CFG_TRACE_MODE != 0) && (POF_CFG_TRACE_MODE != 1) && (POF_CFG_TRACE_MODE != 2)
#error "POF_CFG_TRACE_MODE defined wrong in pof_cfg.h!"
#endif  

/*****************************************************************************/

#if (POF_CFG_TRACE_MODE == 0) //no Trace

/*****************************************************************************/

//POF_UPPER_TRACE
#define POF_UPPER_TRACE_00(idx_, level_, msg_)
#define POF_UPPER_TRACE_01(idx_, level_, msg_, para1_)
#define POF_UPPER_TRACE_02(idx_, level_, msg_, para1_, para2_)
#define POF_UPPER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)
#define POF_UPPER_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)
#define POF_UPPER_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)
#define POF_UPPER_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)
#define POF_UPPER_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)
#define POF_UPPER_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)
#define POF_UPPER_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)
#define POF_UPPER_TRACE_10(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_)
#define POF_UPPER_TRACE_11(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_)
#define POF_UPPER_TRACE_12(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_)
#define POF_UPPER_TRACE_13(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_)
#define POF_UPPER_TRACE_14(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_)
#define POF_UPPER_TRACE_15(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15)
#define POF_UPPER_TRACE_16(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_, para16_)

//POF_LOWER_TRACE
#define POF_LOWER_TRACE_00(idx_, level_, msg_)
#define POF_LOWER_TRACE_01(idx_, level_, msg_, para1_)
#define POF_LOWER_TRACE_02(idx_, level_, msg_, para1_, para2_)
#define POF_LOWER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)
#define POF_LOWER_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)
#define POF_LOWER_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)
#define POF_LOWER_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)
#define POF_LOWER_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)
#define POF_LOWER_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)
#define POF_LOWER_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)
#define POF_LOWER_TRACE_10(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_)
#define POF_LOWER_TRACE_11(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_)
#define POF_LOWER_TRACE_12(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_)
#define POF_LOWER_TRACE_13(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_)
#define POF_LOWER_TRACE_14(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_)
#define POF_LOWER_TRACE_15(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15)
#define POF_LOWER_TRACE_16(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_, para16_)

//POF_SYSTEM_TRACE
#define POF_SYSTEM_TRACE_00(idx_, level_, msg_)
#define POF_SYSTEM_TRACE_01(idx_, level_, msg_, para1_)
#define POF_SYSTEM_TRACE_02(idx_, level_, msg_, para1_, para2_)
#define POF_SYSTEM_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)
#define POF_SYSTEM_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)
#define POF_SYSTEM_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)
#define POF_SYSTEM_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)
#define POF_SYSTEM_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)
#define POF_SYSTEM_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)
#define POF_SYSTEM_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)
#define POF_SYSTEM_TRACE_10(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_)
#define POF_SYSTEM_TRACE_11(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_)
#define POF_SYSTEM_TRACE_12(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_)
#define POF_SYSTEM_TRACE_13(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_)
#define POF_SYSTEM_TRACE_14(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_)
#define POF_SYSTEM_TRACE_15(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15)
#define POF_SYSTEM_TRACE_16(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_, para16_)

//POF_FUNCTION_TRACE
#define POF_FUNCTION_TRACE_00(idx_, level_, msg_)
#define POF_FUNCTION_TRACE_01(idx_, level_, msg_, para1_)
#define POF_FUNCTION_TRACE_02(idx_, level_, msg_, para1_, para2_)
#define POF_FUNCTION_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)
#define POF_FUNCTION_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)
#define POF_FUNCTION_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)
#define POF_FUNCTION_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)
#define POF_FUNCTION_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)
#define POF_FUNCTION_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)
#define POF_FUNCTION_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)
#define POF_FUNCTION_TRACE_10(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_)
#define POF_FUNCTION_TRACE_11(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_)
#define POF_FUNCTION_TRACE_12(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_)
#define POF_FUNCTION_TRACE_13(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_)
#define POF_FUNCTION_TRACE_14(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_)
#define POF_FUNCTION_TRACE_15(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15)
#define POF_FUNCTION_TRACE_16(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_, para16_)

//POF_PROGRAM_TRACE
#define POF_PROGRAM_TRACE_00(idx_, level_, msg_)
#define POF_PROGRAM_TRACE_01(idx_, level_, msg_, para1_)
#define POF_PROGRAM_TRACE_02(idx_, level_, msg_, para1_, para2_)
#define POF_PROGRAM_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)
#define POF_PROGRAM_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)
#define POF_PROGRAM_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)
#define POF_PROGRAM_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)
#define POF_PROGRAM_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)
#define POF_PROGRAM_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)
#define POF_PROGRAM_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)
#define POF_PROGRAM_TRACE_10(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_)
#define POF_PROGRAM_TRACE_11(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_)
#define POF_PROGRAM_TRACE_12(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_)
#define POF_PROGRAM_TRACE_13(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_)
#define POF_PROGRAM_TRACE_14(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_)
#define POF_PROGRAM_TRACE_15(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15)
#define POF_PROGRAM_TRACE_16(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_, para16_)

/*****************************************************************************/

#elif (POF_CFG_TRACE_MODE == 1) //LSA-Trace

/*****************************************************************************/

//POF_UPPER_TRACE
#ifndef POF_UPPER_TRACE_00
#define POF_UPPER_TRACE_00(idx_, level_, msg_)\
    LSA_TRACE_00(TRACE_SUBSYS_POF_UPPER, level_, msg_)
#endif
#ifndef POF_UPPER_TRACE_01
#define POF_UPPER_TRACE_01(idx_, level_, msg_, para1_)\
    LSA_TRACE_01(TRACE_SUBSYS_POF_UPPER, level_, msg_, (para1_))
#endif
#ifndef POF_UPPER_TRACE_02
#define POF_UPPER_TRACE_02(idx_, level_, msg_, para1_, para2_)\
    LSA_TRACE_02(TRACE_SUBSYS_POF_UPPER, level_, msg_, (para1_), (para2_))
#endif
#ifndef POF_UPPER_TRACE_03
#define POF_UPPER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)\
    LSA_TRACE_03(TRACE_SUBSYS_POF_UPPER, level_, msg_, (para1_), (para2_), (para3_))
#endif
#ifndef POF_UPPER_TRACE_04
#define POF_UPPER_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)\
    LSA_TRACE_04(TRACE_SUBSYS_POF_UPPER, level_, msg_, (para1_), (para2_), (para3_), (para4_))
#endif
#ifndef POF_UPPER_TRACE_05
#define POF_UPPER_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)\
    LSA_TRACE_05(TRACE_SUBSYS_POF_UPPER, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_))
#endif
#ifndef POF_UPPER_TRACE_06
#define POF_UPPER_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)\
    LSA_TRACE_06(TRACE_SUBSYS_POF_UPPER, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_))
#endif
#ifndef POF_UPPER_TRACE_07
#define POF_UPPER_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)\
    LSA_TRACE_07(TRACE_SUBSYS_POF_UPPER, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_))
#endif
#ifndef POF_UPPER_TRACE_08
#define POF_UPPER_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)\
    LSA_TRACE_08(TRACE_SUBSYS_POF_UPPER, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_))
#endif
#ifndef POF_UPPER_TRACE_09
#define POF_UPPER_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)\
    LSA_TRACE_09(TRACE_SUBSYS_POF_UPPER, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_))
#endif
#ifndef POF_UPPER_TRACE_10
#define POF_UPPER_TRACE_10(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_)\
    LSA_TRACE_10(TRACE_SUBSYS_POF_UPPER, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_))
#endif
#ifndef POF_UPPER_TRACE_11
#define POF_UPPER_TRACE_11(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_)\
    LSA_TRACE_11(TRACE_SUBSYS_POF_UPPER, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_))
#endif
#ifndef POF_UPPER_TRACE_12
#define POF_UPPER_TRACE_12(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_)\
    LSA_TRACE_12(TRACE_SUBSYS_POF_UPPER, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_))
#endif
#ifndef POF_UPPER_TRACE_13
#define POF_UPPER_TRACE_13(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_)\
    LSA_TRACE_13(TRACE_SUBSYS_POF_UPPER, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_))
#endif
#ifndef POF_UPPER_TRACE_14
#define POF_UPPER_TRACE_14(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_)\
    LSA_TRACE_14(TRACE_SUBSYS_POF_UPPER, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_))
#endif
#ifndef POF_UPPER_TRACE_15
#define POF_UPPER_TRACE_15(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_)\
    LSA_TRACE_15(TRACE_SUBSYS_POF_UPPER, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_), (para15_))
#endif
#ifndef POF_UPPER_TRACE_16
#define POF_UPPER_TRACE_16(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_, para16_)\
    LSA_TRACE_16(TRACE_SUBSYS_POF_UPPER, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_), (para15_), (para16_))
#endif

//POF_LOWER_TRACE
#ifndef POF_LOWER_TRACE_00
#define POF_LOWER_TRACE_00(idx_, level_, msg_)\
    LSA_TRACE_00(TRACE_SUBSYS_POF_LOWER, level_, msg_)
#endif
#ifndef POF_LOWER_TRACE_01
#define POF_LOWER_TRACE_01(idx_, level_, msg_, para1_)\
    LSA_TRACE_01(TRACE_SUBSYS_POF_LOWER, level_, msg_, (para1_))
#endif
#ifndef POF_LOWER_TRACE_02
#define POF_LOWER_TRACE_02(idx_, level_, msg_, para1_, para2_)\
    LSA_TRACE_02(TRACE_SUBSYS_POF_LOWER, level_, msg_, (para1_), (para2_))
#endif
#ifndef POF_LOWER_TRACE_03
#define POF_LOWER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)\
    LSA_TRACE_03(TRACE_SUBSYS_POF_LOWER, level_, msg_, (para1_), (para2_), (para3_))
#endif
#ifndef POF_LOWER_TRACE_04
#define POF_LOWER_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)\
    LSA_TRACE_04(TRACE_SUBSYS_POF_LOWER, level_, msg_, (para1_), (para2_), (para3_), (para4_))
#endif
#ifndef POF_LOWER_TRACE_05
#define POF_LOWER_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)\
    LSA_TRACE_05(TRACE_SUBSYS_POF_LOWER, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_))
#endif
#ifndef POF_LOWER_TRACE_06
#define POF_LOWER_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)\
    LSA_TRACE_06(TRACE_SUBSYS_POF_LOWER, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_))
#endif
#ifndef POF_LOWER_TRACE_07
#define POF_LOWER_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)\
    LSA_TRACE_07(TRACE_SUBSYS_POF_LOWER, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_))
#endif
#ifndef POF_LOWER_TRACE_08
#define POF_LOWER_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)\
    LSA_TRACE_08(TRACE_SUBSYS_POF_LOWER, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_))
#endif
#ifndef POF_LOWER_TRACE_09
#define POF_LOWER_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)\
    LSA_TRACE_09(TRACE_SUBSYS_POF_LOWER, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_))
#endif
#ifndef POF_LOWER_TRACE_10
#define POF_LOWER_TRACE_10(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_)\
    LSA_TRACE_10(TRACE_SUBSYS_POF_LOWER, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_))
#endif
#ifndef POF_LOWER_TRACE_11
#define POF_LOWER_TRACE_11(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_)\
    LSA_TRACE_11(TRACE_SUBSYS_POF_LOWER, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_))
#endif
#ifndef POF_LOWER_TRACE_12
#define POF_LOWER_TRACE_12(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_)\
    LSA_TRACE_12(TRACE_SUBSYS_POF_LOWER, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_))
#endif
#ifndef POF_LOWER_TRACE_13
#define POF_LOWER_TRACE_13(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_)\
    LSA_TRACE_13(TRACE_SUBSYS_POF_LOWER, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_))
#endif
#ifndef POF_LOWER_TRACE_14
#define POF_LOWER_TRACE_14(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_)\
    LSA_TRACE_14(TRACE_SUBSYS_POF_LOWER, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_))
#endif
#ifndef POF_LOWER_TRACE_15
#define POF_LOWER_TRACE_15(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_)\
    LSA_TRACE_15(TRACE_SUBSYS_POF_LOWER, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_), (para15_))
#endif
#ifndef POF_LOWER_TRACE_16
#define POF_LOWER_TRACE_16(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_, para16_)\
    LSA_TRACE_16(TRACE_SUBSYS_POF_LOWER, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_), (para15_), (para16_))
#endif

//POF_SYSTEM_TRACE
#ifndef POF_SYSTEM_TRACE_00
#define POF_SYSTEM_TRACE_00(idx_, level_, msg_)\
    LSA_TRACE_00(TRACE_SUBSYS_POF_SYSTEM, level_, msg_)
#endif
#ifndef POF_SYSTEM_TRACE_01
#define POF_SYSTEM_TRACE_01(idx_, level_, msg_, para1_)\
    LSA_TRACE_01(TRACE_SUBSYS_POF_SYSTEM, level_, msg_, (para1_))
#endif
#ifndef POF_SYSTEM_TRACE_02
#define POF_SYSTEM_TRACE_02(idx_, level_, msg_, para1_, para2_)\
    LSA_TRACE_02(TRACE_SUBSYS_POF_SYSTEM, level_, msg_, (para1_), (para2_))
#endif
#ifndef POF_SYSTEM_TRACE_03
#define POF_SYSTEM_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)\
    LSA_TRACE_03(TRACE_SUBSYS_POF_SYSTEM, level_, msg_, (para1_), (para2_), (para3_))
#endif
#ifndef POF_SYSTEM_TRACE_04
#define POF_SYSTEM_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)\
    LSA_TRACE_04(TRACE_SUBSYS_POF_SYSTEM, level_, msg_, (para1_), (para2_), (para3_), (para4_))
#endif
#ifndef POF_SYSTEM_TRACE_05
#define POF_SYSTEM_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)\
    LSA_TRACE_05(TRACE_SUBSYS_POF_SYSTEM, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_))
#endif
#ifndef POF_SYSTEM_TRACE_06
#define POF_SYSTEM_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)\
    LSA_TRACE_06(TRACE_SUBSYS_POF_SYSTEM, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_))
#endif
#ifndef POF_SYSTEM_TRACE_07
#define POF_SYSTEM_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)\
    LSA_TRACE_07(TRACE_SUBSYS_POF_SYSTEM, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_))
#endif
#ifndef POF_SYSTEM_TRACE_08
#define POF_SYSTEM_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)\
    LSA_TRACE_08(TRACE_SUBSYS_POF_SYSTEM, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_))
#endif
#ifndef POF_SYSTEM_TRACE_09
#define POF_SYSTEM_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)\
    LSA_TRACE_09(TRACE_SUBSYS_POF_SYSTEM, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_))
#endif
#ifndef POF_SYSTEM_TRACE_10
#define POF_SYSTEM_TRACE_10(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_)\
    LSA_TRACE_10(TRACE_SUBSYS_POF_SYSTEM, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_))
#endif
#ifndef POF_SYSTEM_TRACE_11
#define POF_SYSTEM_TRACE_11(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_)\
    LSA_TRACE_11(TRACE_SUBSYS_POF_SYSTEM, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_))
#endif
#ifndef POF_SYSTEM_TRACE_12
#define POF_SYSTEM_TRACE_12(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_)\
    LSA_TRACE_12(TRACE_SUBSYS_POF_SYSTEM, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_))
#endif
#ifndef POF_SYSTEM_TRACE_13
#define POF_SYSTEM_TRACE_13(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_)\
    LSA_TRACE_13(TRACE_SUBSYS_POF_SYSTEM, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_))
#endif
#ifndef POF_SYSTEM_TRACE_14
#define POF_SYSTEM_TRACE_14(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_)\
    LSA_TRACE_14(TRACE_SUBSYS_POF_SYSTEM, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_))
#endif
#ifndef POF_SYSTEM_TRACE_15
#define POF_SYSTEM_TRACE_15(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_)\
    LSA_TRACE_15(TRACE_SUBSYS_POF_SYSTEM, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_), (para15_))
#endif
#ifndef POF_SYSTEM_TRACE_16
#define POF_SYSTEM_TRACE_16(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_, para16_)\
    LSA_TRACE_16(TRACE_SUBSYS_POF_SYSTEM, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_), (para15_), (para16_))
#endif

//POF_FUNCTION_TRACE
#ifndef POF_FUNCTION_TRACE_00
#define POF_FUNCTION_TRACE_00(idx_, level_, msg_)\
    LSA_TRACE_00(TRACE_SUBSYS_POF_FUNCTION, level_, msg_)
#endif
#ifndef POF_FUNCTION_TRACE_01
#define POF_FUNCTION_TRACE_01(idx_, level_, msg_, para1_)\
    LSA_TRACE_01(TRACE_SUBSYS_POF_FUNCTION, level_, msg_, (para1_))
#endif
#ifndef POF_FUNCTION_TRACE_02
#define POF_FUNCTION_TRACE_02(idx_, level_, msg_, para1_, para2_)\
    LSA_TRACE_02(TRACE_SUBSYS_POF_FUNCTION, level_, msg_, (para1_), (para2_))
#endif
#ifndef POF_FUNCTION_TRACE_03
#define POF_FUNCTION_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)\
    LSA_TRACE_03(TRACE_SUBSYS_POF_FUNCTION, level_, msg_, (para1_), (para2_), (para3_))
#endif
#ifndef POF_FUNCTION_TRACE_04
#define POF_FUNCTION_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)\
    LSA_TRACE_04(TRACE_SUBSYS_POF_FUNCTION, level_, msg_, (para1_), (para2_), (para3_), (para4_))
#endif
#ifndef POF_FUNCTION_TRACE_05
#define POF_FUNCTION_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)\
    LSA_TRACE_05(TRACE_SUBSYS_POF_FUNCTION, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_))
#endif
#ifndef POF_FUNCTION_TRACE_06
#define POF_FUNCTION_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)\
    LSA_TRACE_06(TRACE_SUBSYS_POF_FUNCTION, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_))
#endif
#ifndef POF_FUNCTION_TRACE_07
#define POF_FUNCTION_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)\
    LSA_TRACE_07(TRACE_SUBSYS_POF_FUNCTION, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_))
#endif
#ifndef POF_FUNCTION_TRACE_08
#define POF_FUNCTION_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)\
    LSA_TRACE_08(TRACE_SUBSYS_POF_FUNCTION, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_))
#endif
#ifndef POF_FUNCTION_TRACE_09
#define POF_FUNCTION_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)\
    LSA_TRACE_09(TRACE_SUBSYS_POF_FUNCTION, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_))
#endif
#ifndef POF_FUNCTION_TRACE_10
#define POF_FUNCTION_TRACE_10(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_)\
    LSA_TRACE_10(TRACE_SUBSYS_POF_FUNCTION, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_))
#endif
#ifndef POF_FUNCTION_TRACE_11
#define POF_FUNCTION_TRACE_11(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_)\
    LSA_TRACE_11(TRACE_SUBSYS_POF_FUNCTION, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_))
#endif
#ifndef POF_FUNCTION_TRACE_12
#define POF_FUNCTION_TRACE_12(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_)\
    LSA_TRACE_12(TRACE_SUBSYS_POF_FUNCTION, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_))
#endif
#ifndef POF_FUNCTION_TRACE_13
#define POF_FUNCTION_TRACE_13(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_)\
    LSA_TRACE_13(TRACE_SUBSYS_POF_FUNCTION, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_))
#endif
#ifndef POF_FUNCTION_TRACE_14
#define POF_FUNCTION_TRACE_14(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_)\
    LSA_TRACE_14(TRACE_SUBSYS_POF_FUNCTION, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_))
#endif
#ifndef POF_FUNCTION_TRACE_15
#define POF_FUNCTION_TRACE_15(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_)\
    LSA_TRACE_15(TRACE_SUBSYS_POF_FUNCTION, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_), (para15_))
#endif
#ifndef POF_FUNCTION_TRACE_16
#define POF_FUNCTION_TRACE_16(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_, para16_)\
    LSA_TRACE_16(TRACE_SUBSYS_POF_FUNCTION, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_), (para15_), (para16_))
#endif

//POF_PROGRAM_TRACE
#ifndef POF_PROGRAM_TRACE_00
#define POF_PROGRAM_TRACE_00(idx_, level_, msg_)\
    LSA_TRACE_00(TRACE_SUBSYS_POF_PROGRAM, level_, msg_)
#endif
#ifndef POF_PROGRAM_TRACE_01
#define POF_PROGRAM_TRACE_01(idx_, level_, msg_, para1_)\
    LSA_TRACE_01(TRACE_SUBSYS_POF_PROGRAM, level_, msg_, (para1_))
#endif
#ifndef POF_PROGRAM_TRACE_02
#define POF_PROGRAM_TRACE_02(idx_, level_, msg_, para1_, para2_)\
    LSA_TRACE_02(TRACE_SUBSYS_POF_PROGRAM, level_, msg_, (para1_), (para2_))
#endif
#ifndef POF_PROGRAM_TRACE_03
#define POF_PROGRAM_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)\
    LSA_TRACE_03(TRACE_SUBSYS_POF_PROGRAM, level_, msg_, (para1_), (para2_), (para3_))
#endif
#ifndef POF_PROGRAM_TRACE_04
#define POF_PROGRAM_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)\
    LSA_TRACE_04(TRACE_SUBSYS_POF_PROGRAM, level_, msg_, (para1_), (para2_), (para3_), (para4_))
#endif
#ifndef POF_PROGRAM_TRACE_05
#define POF_PROGRAM_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)\
    LSA_TRACE_05(TRACE_SUBSYS_POF_PROGRAM, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_))
#endif
#ifndef POF_PROGRAM_TRACE_06
#define POF_PROGRAM_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)\
    LSA_TRACE_06(TRACE_SUBSYS_POF_PROGRAM, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_))
#endif
#ifndef POF_PROGRAM_TRACE_07
#define POF_PROGRAM_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)\
    LSA_TRACE_07(TRACE_SUBSYS_POF_PROGRAM, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_))
#endif
#ifndef POF_PROGRAM_TRACE_08
#define POF_PROGRAM_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)\
    LSA_TRACE_08(TRACE_SUBSYS_POF_PROGRAM, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_))
#endif
#ifndef POF_PROGRAM_TRACE_09
#define POF_PROGRAM_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)\
    LSA_TRACE_09(TRACE_SUBSYS_POF_PROGRAM, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_))
#endif
#ifndef POF_PROGRAM_TRACE_10
#define POF_PROGRAM_TRACE_10(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_)\
    LSA_TRACE_10(TRACE_SUBSYS_POF_PROGRAM, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_))
#endif
#ifndef POF_PROGRAM_TRACE_11
#define POF_PROGRAM_TRACE_11(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_)\
    LSA_TRACE_11(TRACE_SUBSYS_POF_PROGRAM, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_))
#endif
#ifndef POF_PROGRAM_TRACE_12
#define POF_PROGRAM_TRACE_12(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_)\
    LSA_TRACE_12(TRACE_SUBSYS_POF_PROGRAM, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_))
#endif
#ifndef POF_PROGRAM_TRACE_13
#define POF_PROGRAM_TRACE_13(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_)\
    LSA_TRACE_13(TRACE_SUBSYS_POF_PROGRAM, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_))
#endif
#ifndef POF_PROGRAM_TRACE_14
#define POF_PROGRAM_TRACE_14(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_)\
    LSA_TRACE_14(TRACE_SUBSYS_POF_PROGRAM, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_))
#endif
#ifndef POF_PROGRAM_TRACE_15
#define POF_PROGRAM_TRACE_15(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_)\
    LSA_TRACE_15(TRACE_SUBSYS_POF_PROGRAM, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_), (para15_))
#endif
#ifndef POF_PROGRAM_TRACE_16
#define POF_PROGRAM_TRACE_16(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_, para16_)\
    LSA_TRACE_16(TRACE_SUBSYS_POF_PROGRAM, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_), (para15_), (para16_))
#endif

/*****************************************************************************/

#elif (POF_CFG_TRACE_MODE == 2) //LSA-IDX-Trace

/*****************************************************************************/

//POF_UPPER_TRACE
#ifndef POF_UPPER_TRACE_00
#define POF_UPPER_TRACE_00(idx_, level_, msg_)\
    LSA_IDX_TRACE_00(TRACE_SUBSYS_POF_UPPER, idx_, level_, msg_)
#endif
#ifndef POF_UPPER_TRACE_01
#define POF_UPPER_TRACE_01(idx_, level_, msg_, para1_)\
    LSA_IDX_TRACE_01(TRACE_SUBSYS_POF_UPPER, idx_, level_, msg_, (para1_))
#endif
#ifndef POF_UPPER_TRACE_02
#define POF_UPPER_TRACE_02(idx_, level_, msg_, para1_, para2_)\
    LSA_IDX_TRACE_02(TRACE_SUBSYS_POF_UPPER, idx_, level_, msg_, (para1_), (para2_))
#endif
#ifndef POF_UPPER_TRACE_03
#define POF_UPPER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)\
    LSA_IDX_TRACE_03(TRACE_SUBSYS_POF_UPPER, idx_, level_, msg_, (para1_), (para2_), (para3_))
#endif
#ifndef POF_UPPER_TRACE_04
#define POF_UPPER_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)\
    LSA_IDX_TRACE_04(TRACE_SUBSYS_POF_UPPER, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_))
#endif
#ifndef POF_UPPER_TRACE_05
#define POF_UPPER_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)\
    LSA_IDX_TRACE_05(TRACE_SUBSYS_POF_UPPER, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_))
#endif
#ifndef POF_UPPER_TRACE_06
#define POF_UPPER_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)\
    LSA_IDX_TRACE_06(TRACE_SUBSYS_POF_UPPER, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_))
#endif
#ifndef POF_UPPER_TRACE_07
#define POF_UPPER_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)\
    LSA_IDX_TRACE_07(TRACE_SUBSYS_POF_UPPER, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_))
#endif
#ifndef POF_UPPER_TRACE_08
#define POF_UPPER_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)\
    LSA_IDX_TRACE_08(TRACE_SUBSYS_POF_UPPER, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_))
#endif
#ifndef POF_UPPER_TRACE_09
#define POF_UPPER_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)\
    LSA_IDX_TRACE_09(TRACE_SUBSYS_POF_UPPER, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_))
#endif
#ifndef POF_UPPER_TRACE_10
#define POF_UPPER_TRACE_10(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_)\
    LSA_IDX_TRACE_10(TRACE_SUBSYS_POF_UPPER, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_))
#endif
#ifndef POF_UPPER_TRACE_11
#define POF_UPPER_TRACE_11(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_)\
    LSA_IDX_TRACE_11(TRACE_SUBSYS_POF_UPPER, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_))
#endif
#ifndef POF_UPPER_TRACE_12
#define POF_UPPER_TRACE_12(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_)\
    LSA_IDX_TRACE_12(TRACE_SUBSYS_POF_UPPER, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_))
#endif
#ifndef POF_UPPER_TRACE_13
#define POF_UPPER_TRACE_13(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_)\
    LSA_IDX_TRACE_13(TRACE_SUBSYS_POF_UPPER, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_))
#endif
#ifndef POF_UPPER_TRACE_14
#define POF_UPPER_TRACE_14(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_)\
    LSA_IDX_TRACE_14(TRACE_SUBSYS_POF_UPPER, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_))
#endif
#ifndef POF_UPPER_TRACE_15
#define POF_UPPER_TRACE_15(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_)\
    LSA_IDX_TRACE_15(TRACE_SUBSYS_POF_UPPER, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_), (para15_))
#endif
#ifndef POF_UPPER_TRACE_16
#define POF_UPPER_TRACE_16(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_, para16_)\
    LSA_IDX_TRACE_16(TRACE_SUBSYS_POF_UPPER, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_), (para15_), (para16_))
#endif

//POF_LOWER_TRACE
#ifndef POF_LOWER_TRACE_00
#define POF_LOWER_TRACE_00(idx_, level_, msg_)\
    LSA_IDX_TRACE_00(TRACE_SUBSYS_POF_LOWER, idx_, level_, msg_) 
#endif
#ifndef POF_LOWER_TRACE_01
#define POF_LOWER_TRACE_01(idx_, level_, msg_, para1_)\
    LSA_IDX_TRACE_01(TRACE_SUBSYS_POF_LOWER, idx_, level_, msg_, (para1_))
#endif
#ifndef POF_LOWER_TRACE_02
#define POF_LOWER_TRACE_02(idx_, level_, msg_, para1_, para2_)\
    LSA_IDX_TRACE_02(TRACE_SUBSYS_POF_LOWER, idx_, level_, msg_, (para1_), (para2_))
#endif
#ifndef POF_LOWER_TRACE_03
#define POF_LOWER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)\
    LSA_IDX_TRACE_03(TRACE_SUBSYS_POF_LOWER, idx_, level_, msg_, (para1_), (para2_), (para3_))
#endif
#ifndef POF_LOWER_TRACE_04
#define POF_LOWER_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)\
    LSA_IDX_TRACE_04(TRACE_SUBSYS_POF_LOWER, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_))
#endif
#ifndef POF_LOWER_TRACE_05
#define POF_LOWER_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)\
    LSA_IDX_TRACE_05(TRACE_SUBSYS_POF_LOWER, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_))
#endif
#ifndef POF_LOWER_TRACE_06
#define POF_LOWER_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)\
    LSA_IDX_TRACE_06(TRACE_SUBSYS_POF_LOWER, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_))
#endif
#ifndef POF_LOWER_TRACE_07
#define POF_LOWER_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)\
    LSA_IDX_TRACE_07(TRACE_SUBSYS_POF_LOWER, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_))
#endif
#ifndef POF_LOWER_TRACE_08
#define POF_LOWER_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)\
    LSA_IDX_TRACE_08(TRACE_SUBSYS_POF_LOWER, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_))
#endif
#ifndef POF_LOWER_TRACE_09
#define POF_LOWER_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)\
    LSA_IDX_TRACE_09(TRACE_SUBSYS_POF_LOWER, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_))
#endif
#ifndef POF_LOWER_TRACE_10
#define POF_LOWER_TRACE_10(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_)\
    LSA_IDX_TRACE_10(TRACE_SUBSYS_POF_LOWER, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_))
#endif
#ifndef POF_LOWER_TRACE_11
#define POF_LOWER_TRACE_11(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_)\
    LSA_IDX_TRACE_11(TRACE_SUBSYS_POF_LOWER, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_))
#endif
#ifndef POF_LOWER_TRACE_12
#define POF_LOWER_TRACE_12(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_)\
    LSA_IDX_TRACE_12(TRACE_SUBSYS_POF_LOWER, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_))
#endif
#ifndef POF_LOWER_TRACE_13
#define POF_LOWER_TRACE_13(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_)\
    LSA_IDX_TRACE_13(TRACE_SUBSYS_POF_LOWER, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_))
#endif
#ifndef POF_LOWER_TRACE_14
#define POF_LOWER_TRACE_14(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_)\
    LSA_IDX_TRACE_14(TRACE_SUBSYS_POF_LOWER, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_))
#endif
#ifndef POF_LOWER_TRACE_15
#define POF_LOWER_TRACE_15(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_)\
    LSA_IDX_TRACE_15(TRACE_SUBSYS_POF_LOWER, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_), (para15_))
#endif
#ifndef POF_LOWER_TRACE_16
#define POF_LOWER_TRACE_16(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_, para16_)\
    LSA_IDX_TRACE_16(TRACE_SUBSYS_POF_LOWER, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_), (para15_), (para16_))
#endif

//POF_SYSTEM_TRACE
#ifndef POF_SYSTEM_TRACE_00
#define POF_SYSTEM_TRACE_00(idx_, level_, msg_)\
    LSA_IDX_TRACE_00(TRACE_SUBSYS_POF_SYSTEM, idx_, level_, msg_)
#endif
#ifndef POF_SYSTEM_TRACE_01
#define POF_SYSTEM_TRACE_01(idx_, level_, msg_, para1_)\
    LSA_IDX_TRACE_01(TRACE_SUBSYS_POF_SYSTEM, idx_, level_, msg_, (para1_))
#endif
#ifndef POF_SYSTEM_TRACE_02
#define POF_SYSTEM_TRACE_02(idx_, level_, msg_, para1_, para2_)\
    LSA_IDX_TRACE_02(TRACE_SUBSYS_POF_SYSTEM, idx_, level_, msg_, (para1_), (para2_))
#endif
#ifndef POF_SYSTEM_TRACE_03
#define POF_SYSTEM_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)\
    LSA_IDX_TRACE_03(TRACE_SUBSYS_POF_SYSTEM, idx_, level_, msg_, (para1_), (para2_), (para3_))
#endif
#ifndef POF_SYSTEM_TRACE_04
#define POF_SYSTEM_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)\
    LSA_IDX_TRACE_04(TRACE_SUBSYS_POF_SYSTEM, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_))
#endif
#ifndef POF_SYSTEM_TRACE_05
#define POF_SYSTEM_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)\
    LSA_IDX_TRACE_05(TRACE_SUBSYS_POF_SYSTEM, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_))
#endif
#ifndef POF_SYSTEM_TRACE_06
#define POF_SYSTEM_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)\
    LSA_IDX_TRACE_06(TRACE_SUBSYS_POF_SYSTEM, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_))
#endif
#ifndef POF_SYSTEM_TRACE_07
#define POF_SYSTEM_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)\
    LSA_IDX_TRACE_07(TRACE_SUBSYS_POF_SYSTEM, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_))
#endif
#ifndef POF_SYSTEM_TRACE_08
#define POF_SYSTEM_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)\
    LSA_IDX_TRACE_08(TRACE_SUBSYS_POF_SYSTEM, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_))
#endif
#ifndef POF_SYSTEM_TRACE_09
#define POF_SYSTEM_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)\
    LSA_IDX_TRACE_09(TRACE_SUBSYS_POF_SYSTEM, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_))
#endif
#ifndef POF_SYSTEM_TRACE_10
#define POF_SYSTEM_TRACE_10(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_)\
    LSA_IDX_TRACE_10(TRACE_SUBSYS_POF_SYSTEM, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_))
#endif
#ifndef POF_SYSTEM_TRACE_11
#define POF_SYSTEM_TRACE_11(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_)\
    LSA_IDX_TRACE_11(TRACE_SUBSYS_POF_SYSTEM, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_))
#endif
#ifndef POF_SYSTEM_TRACE_12
#define POF_SYSTEM_TRACE_12(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_)\
    LSA_IDX_TRACE_12(TRACE_SUBSYS_POF_SYSTEM, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_))
#endif
#ifndef POF_SYSTEM_TRACE_13
#define POF_SYSTEM_TRACE_13(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_)\
    LSA_IDX_TRACE_13(TRACE_SUBSYS_POF_SYSTEM, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_))
#endif
#ifndef POF_SYSTEM_TRACE_14
#define POF_SYSTEM_TRACE_14(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_)\
    LSA_IDX_TRACE_14(TRACE_SUBSYS_POF_SYSTEM, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_))
#endif
#ifndef POF_SYSTEM_TRACE_15
#define POF_SYSTEM_TRACE_15(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_)\
    LSA_IDX_TRACE_15(TRACE_SUBSYS_POF_SYSTEM, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_), (para15_))
#endif
#ifndef POF_SYSTEM_TRACE_16
#define POF_SYSTEM_TRACE_16(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_, para16_)\
    LSA_IDX_TRACE_16(TRACE_SUBSYS_POF_SYSTEM, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_), (para15_), (para16_))
#endif

//POF_FUNCTION_TRACE
#ifndef POF_FUNCTION_TRACE_00
#define POF_FUNCTION_TRACE_00(idx_, level_, msg_)\
    LSA_IDX_TRACE_00(TRACE_SUBSYS_POF_FUNCTION, idx_, level_, msg_)
#endif
#ifndef POF_FUNCTION_TRACE_01
#define POF_FUNCTION_TRACE_01(idx_, level_, msg_, para1_)\
    LSA_IDX_TRACE_01(TRACE_SUBSYS_POF_FUNCTION, idx_, level_, msg_, (para1_))
#endif
#ifndef POF_FUNCTION_TRACE_02
#define POF_FUNCTION_TRACE_02(idx_, level_, msg_, para1_, para2_)\
    LSA_IDX_TRACE_02(TRACE_SUBSYS_POF_FUNCTION, idx_, level_, msg_, (para1_), (para2_))
#endif
#ifndef POF_FUNCTION_TRACE_03
#define POF_FUNCTION_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)\
    LSA_IDX_TRACE_03(TRACE_SUBSYS_POF_FUNCTION, idx_, level_, msg_, (para1_), (para2_), (para3_))
#endif
#ifndef POF_FUNCTION_TRACE_04
#define POF_FUNCTION_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)\
    LSA_IDX_TRACE_04(TRACE_SUBSYS_POF_FUNCTION, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_))
#endif
#ifndef POF_FUNCTION_TRACE_05
#define POF_FUNCTION_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)\
    LSA_IDX_TRACE_05(TRACE_SUBSYS_POF_FUNCTION, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_))
#endif
#ifndef POF_FUNCTION_TRACE_06
#define POF_FUNCTION_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)\
    LSA_IDX_TRACE_06(TRACE_SUBSYS_POF_FUNCTION, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_))
#endif
#ifndef POF_FUNCTION_TRACE_07
#define POF_FUNCTION_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)\
    LSA_IDX_TRACE_07(TRACE_SUBSYS_POF_FUNCTION, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_))
#endif
#ifndef POF_FUNCTION_TRACE_08
#define POF_FUNCTION_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)\
    LSA_IDX_TRACE_08(TRACE_SUBSYS_POF_FUNCTION, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_))
#endif
#ifndef POF_FUNCTION_TRACE_09
#define POF_FUNCTION_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)\
    LSA_IDX_TRACE_09(TRACE_SUBSYS_POF_FUNCTION, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_))
#endif
#ifndef POF_FUNCTION_TRACE_10
#define POF_FUNCTION_TRACE_10(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_)\
    LSA_IDX_TRACE_10(TRACE_SUBSYS_POF_FUNCTION, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_))
#endif
#ifndef POF_FUNCTION_TRACE_11
#define POF_FUNCTION_TRACE_11(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_)\
    LSA_IDX_TRACE_11(TRACE_SUBSYS_POF_FUNCTION, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_))
#endif
#ifndef POF_FUNCTION_TRACE_12
#define POF_FUNCTION_TRACE_12(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_)\
    LSA_IDX_TRACE_12(TRACE_SUBSYS_POF_FUNCTION, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_))
#endif
#ifndef POF_FUNCTION_TRACE_13
#define POF_FUNCTION_TRACE_13(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_)\
    LSA_IDX_TRACE_13(TRACE_SUBSYS_POF_FUNCTION, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_))
#endif
#ifndef POF_FUNCTION_TRACE_14
#define POF_FUNCTION_TRACE_14(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_)\
    LSA_IDX_TRACE_14(TRACE_SUBSYS_POF_FUNCTION, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_))
#endif
#ifndef POF_FUNCTION_TRACE_15
#define POF_FUNCTION_TRACE_15(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_)\
    LSA_IDX_TRACE_15(TRACE_SUBSYS_POF_FUNCTION, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_), (para15_))
#endif
#ifndef POF_FUNCTION_TRACE_16
#define POF_FUNCTION_TRACE_16(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_, para16_)\
    LSA_IDX_TRACE_16(TRACE_SUBSYS_POF_FUNCTION, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_), (para15_), (para16_))
#endif

//POF_PROGRAM_TRACE
#ifndef POF_PROGRAM_TRACE_00
#define POF_PROGRAM_TRACE_00(idx_, level_, msg_)\
    LSA_IDX_TRACE_00(TRACE_SUBSYS_POF_PROGRAM, idx_, level_, msg_)
#endif
#ifndef POF_PROGRAM_TRACE_01
#define POF_PROGRAM_TRACE_01(idx_, level_, msg_, para1_)\
    LSA_IDX_TRACE_01(TRACE_SUBSYS_POF_PROGRAM, idx_, level_, msg_, (para1_))
#endif
#ifndef POF_PROGRAM_TRACE_02
#define POF_PROGRAM_TRACE_02(idx_, level_, msg_, para1_, para2_)\
    LSA_IDX_TRACE_02(TRACE_SUBSYS_POF_PROGRAM, idx_, level_, msg_, (para1_), (para2_))
#endif
#ifndef POF_PROGRAM_TRACE_03
#define POF_PROGRAM_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)\
    LSA_IDX_TRACE_03(TRACE_SUBSYS_POF_PROGRAM, idx_, level_, msg_, (para1_), (para2_), (para3_))
#endif
#ifndef POF_PROGRAM_TRACE_04
#define POF_PROGRAM_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)\
    LSA_IDX_TRACE_04(TRACE_SUBSYS_POF_PROGRAM, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_))
#endif
#ifndef POF_PROGRAM_TRACE_05
#define POF_PROGRAM_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)\
    LSA_IDX_TRACE_05(TRACE_SUBSYS_POF_PROGRAM, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_))
#endif
#ifndef POF_PROGRAM_TRACE_06
#define POF_PROGRAM_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)\
    LSA_IDX_TRACE_06(TRACE_SUBSYS_POF_PROGRAM, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_))
#endif
#ifndef POF_PROGRAM_TRACE_07
#define POF_PROGRAM_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)\
    LSA_IDX_TRACE_07(TRACE_SUBSYS_POF_PROGRAM, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_))
#endif
#ifndef POF_PROGRAM_TRACE_08
#define POF_PROGRAM_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)\
    LSA_IDX_TRACE_08(TRACE_SUBSYS_POF_PROGRAM, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_))
#endif
#ifndef POF_PROGRAM_TRACE_09
#define POF_PROGRAM_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)\
    LSA_IDX_TRACE_09(TRACE_SUBSYS_POF_PROGRAM, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_))
#endif
#ifndef POF_PROGRAM_TRACE_10
#define POF_PROGRAM_TRACE_10(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_)\
    LSA_IDX_TRACE_10(TRACE_SUBSYS_POF_PROGRAM, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_))
#endif
#ifndef POF_PROGRAM_TRACE_11
#define POF_PROGRAM_TRACE_11(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_)\
    LSA_IDX_TRACE_11(TRACE_SUBSYS_POF_PROGRAM, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_))
#endif
#ifndef POF_PROGRAM_TRACE_12
#define POF_PROGRAM_TRACE_12(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_)\
    LSA_IDX_TRACE_12(TRACE_SUBSYS_POF_PROGRAM, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_))
#endif
#ifndef POF_PROGRAM_TRACE_13
#define POF_PROGRAM_TRACE_13(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_)\
    LSA_IDX_TRACE_13(TRACE_SUBSYS_POF_PROGRAM, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_))
#endif
#ifndef POF_PROGRAM_TRACE_14
#define POF_PROGRAM_TRACE_14(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_)\
    LSA_IDX_TRACE_14(TRACE_SUBSYS_POF_PROGRAM, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_))
#endif
#ifndef POF_PROGRAM_TRACE_15
#define POF_PROGRAM_TRACE_15(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_)\
    LSA_IDX_TRACE_15(TRACE_SUBSYS_POF_PROGRAM, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_), (para15_))
#endif
#ifndef POF_PROGRAM_TRACE_16
#define POF_PROGRAM_TRACE_16(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_, para10_, para11_, para12_, para13_, para14_, para15_, para16_)\
    LSA_IDX_TRACE_16(TRACE_SUBSYS_POF_PROGRAM, idx_, level_, msg_, (para1_), (para2_), (para3_), (para4_), (para5_), (para6_), (para7_), (para8_), (para9_), (para10_), (para11_), (para12_), (para13_), (para14_), (para15_), (para16_))
#endif

#endif //POF_CFG_TRACE_MODE


/*****************************************************************************/
/*  end of file pof_trc.h                                                    */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of POF_TRC_H */
