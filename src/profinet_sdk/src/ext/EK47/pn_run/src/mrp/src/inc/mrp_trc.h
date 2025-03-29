#ifndef MRP_TRC_H
#define MRP_TRC_H

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
/*  F i l e               &F: mrp_trc.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  System interface for lsa-trace                                           */
/*  Defines lsa-trace macros for MRP.                                        */
/*                                                                           */
/*      Here the macros are adapted to the lsa component LTRC.               */
/*                                                                           */
/*      If LTRC shall not be used, then either the LSA_TRACE macros used     */
/*      here must be defined by the system in the file lsa_cfg.h/txt or      */
/*      directly the MRP-trace macro must be redefined on the desired        */
/*      function/macro in the mrp_cfg.h/txt file.                            */
/*      If the system isn't able to store trace data then an "empty          */
/*      definition" must be implemented in file mrp_cfg.h/txt (or the        */
/*      LSA_TRACE macros must be defined "empty" in lsa_cfg.h/txt).          */
/*                                                                           */
/*      See also MRP_CFG_TRACE_MODE in mrp_cfg.h/txt                         */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef MRP_MESSAGE
/*  2008-02-29  mk    MRP_UPPER_TRACE_03 added.                              */
/*  2008-04-16  mk    MRP_UPPER_TRACE_04 added.                              */
/*  2008-06-30  mk    MRP_LOWER_TRACE_07 added.                              */
/*  2009-01-15  ds    MRP_UPPER_TRACE_05 added, MRP_UPPER_TRACE_04 corrected */
/*  2009-03-19  ds    MRP_UPPER_TRACE_06 added.                              */
/*  2009-10-22  ds    Each trace subsystem supports up to 7 arguments now    */
/*  2010-01-28  ds    Each trace subsystem supports up to 8 arguments now    */
/*  2010-07-19  ds    LSA trace unified according to RQ AP00999113           */
/*  2010-11-05  ds    N-IF: MRP_CFG_TRACE_MODE == 2 introduced for indexed   */
/*                    trace purposes on multiple ethernet interfaces         */
/*  2018-10-19  hh    introduction of N MRP interconnection instances        */
/*                                                                           */
/*                                                                           */
#endif
/*****************************************************************************/

/*===========================================================================*/
/* Usage of traces within MRP: (have to be defined by LTRC or System!)       */
/*                                                                           */
/* Trace-Subsystems used                                                     */
/*                                                                           */
/* TRACE_SUBSYS_MRP_UPPER    :    Upper-Interface calls/response             */
/* TRACE_SUBSYS_MRP_LOWER    :    Lower-Interface calls/response             */
/* TRACE_SUBSYS_MRP_SYSTEM   :    System-Interface calls/response            */
/* TRACE_SUBSYS_MRP_PROGRAM  :    Program flow and errors                    */
/*                                                                           */
/* NOTE: the subsystems of LTRC are defined per enum                         */
/*                                                                           */
/* Trace-Level usage:                                                        */
/*                                                                           */
/* LSA_TRACE_LEVEL_FATAL      :    Fatal errors                              */
/* LSA_TRACE_LEVEL_ERROR      :    Errors                                    */
/* LSA_TRACE_LEVEL_UNEXP      :    Unexpected errors                         */
/* LSA_TRACE_LEVEL_WARN       :    Warnings                                  */
/* LSA_TRACE_LEVEL_NOTE_HIGH  :    More important situations                 */
/* LSA_TRACE_LEVEL_NOTE       :    Upper/Lower/System calls/responses        */
/*                                 More important notes                      */
/* LSA_TRACE_LEVEL_NOTE_LOW   :    Normal program flow                       */
/* LSA_TRACE_LEVEL_CHAT       :    Function in/out                           */
/*                                                                           */
/*===========================================================================*/


/*===========================================================================*/
/* TRACE-References have to be defined by system adaption.                   */
/*===========================================================================*/

/*===========================================================================*/
/*                                 macros                                    */
/*===========================================================================*/

/* system interface output macros
 *
 * Note: There aren't any prototypes for these macros so that the target system
 * defines the pointer size and not the MRP. MRP doesn't cast the
 * parameters at the call.
 * A function can nevertheless be invoked within the macro.
 *
 * The subsystem YYYYY (see below) must be defined by the LSA component
 * developer. It can be =
 *   [empty]:   If this component uses only one subsystem.
 *    UPPER:     All requirements of the upper user are written into this
 *               subsystem.
 *               Calls of the user are marked with                   >>
 *               Confirmations to the user are marked with           <<
 *    LOWER:     All requirements to the lower layer are written into this
 *               subsystem.
 *               Requests to the lower layer are marked with         >>
 *               Confirmations from the lower layer are marked with  <<
 *    MEMORY:    All memory requests for RQBs, data buffer, TA-Buffer, ...
 *               are written into this subsystem. Conversions of UPPER_TO_LOWER
 *               and reversed also belong to this subsystem.
 *    SYSTEM:    All other requirements to / from the system are written into
 *               this subsystem. For example: Timers, path information, copying
 *               processes of COMMON memory, ....
 *               For functions which the system invokes:
 *               Calls of the system are marked with                 -->
 *               The end of the functions are marked with            <--
 *    PROTOCOL:  The protocol is written down here.
 *    FUNCTION:  (Internal) function call trace.
 *
 *	  If the component developer wants to make trace entries of  other things
 *    than above mentioned, he can define other subsystems.
 */


/*=============================================================================
 * macro names:   MRP_YYYYY_TRACE_XX  (XX = 00, 01, 02, 03, ..., 16)
 *
 * macros:        Trace entry of 0, 1, 2, ... or 16 parameters for the
 *                subsystem "YYYYY".
 *
 *                The system integrator must define the identifier
 *                TRACE_SUBSYS_MRP_YYYYY (if LTRC is used in the file
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
 *                para16_:  parameter 16
 *
 * return value:  LSA_VOID
 *===========================================================================*/


#if MRP_CFG_TRACE_MODE == 1

/*===========================================================================*/
/* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */ 
#ifndef MRP_TRACE_CREATE_LOCAL_VARIABLE
#define MRP_TRACE_CREATE_LOCAL_VARIABLE(name_, variable_) \
  LSA_UINT32 name_ = variable_
#endif

#ifndef MRP_UPPER_TRACE_00
#define MRP_UPPER_TRACE_00(idx_, level_, msg_) \
    do { (void)(idx_); LSA_TRACE_00(TRACE_SUBSYS_MRP_UPPER, level_, msg_); } while (0)
#endif

#ifndef MRP_UPPER_TRACE_01
#define MRP_UPPER_TRACE_01(idx_, level_, msg_, para1_) \
    do { (void)(idx_); LSA_TRACE_01(TRACE_SUBSYS_MRP_UPPER, level_, msg_, para1_); } while (0)
#endif

#ifndef MRP_UPPER_TRACE_02
#define MRP_UPPER_TRACE_02(idx_, level_, msg_, para1_, para2_) \
    do { (void)(idx_); LSA_TRACE_02(TRACE_SUBSYS_MRP_UPPER, level_, msg_, para1_, para2_); } while (0)
#endif

#ifndef MRP_UPPER_TRACE_03
#define MRP_UPPER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) \
    do { (void)(idx_); LSA_TRACE_03(TRACE_SUBSYS_MRP_UPPER, level_, msg_, para1_, para2_, para3_); } while (0)
#endif

#ifndef MRP_UPPER_TRACE_04
#define MRP_UPPER_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) \
    do { (void)(idx_); LSA_TRACE_04(TRACE_SUBSYS_MRP_UPPER, level_, msg_, para1_, para2_, para3_, para4_); } while (0)
#endif

#ifndef MRP_UPPER_TRACE_05
#define MRP_UPPER_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) \
    do { (void)(idx_); LSA_TRACE_05(TRACE_SUBSYS_MRP_UPPER, level_, msg_, para1_, para2_, para3_, para4_, para5_); } while (0)
#endif

#ifndef MRP_UPPER_TRACE_06
#define MRP_UPPER_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) \
    do { (void)(idx_); LSA_TRACE_06(TRACE_SUBSYS_MRP_UPPER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); } while (0)
#endif

#ifndef MRP_UPPER_TRACE_07
#define MRP_UPPER_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) \
    do { (void)(idx_); LSA_TRACE_07(TRACE_SUBSYS_MRP_UPPER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); } while (0)
#endif

#ifndef MRP_UPPER_TRACE_08
#define MRP_UPPER_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) \
    do { (void)(idx_); LSA_TRACE_08(TRACE_SUBSYS_MRP_UPPER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); } while (0)
#endif

#ifndef MRP_UPPER_TRACE_09
#define MRP_UPPER_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_) \
    do { (void)(idx_); LSA_TRACE_09(TRACE_SUBSYS_MRP_UPPER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_); } while (0)
#endif

#ifndef MRP_UPPER_TRACE_BYTE_ARRAY
#define MRP_UPPER_TRACE_BYTE_ARRAY(idx_, level_, msg_, para1_, para2_) \
    do { (void)(idx_); LSA_TRACE_BYTE_ARRAY(TRACE_SUBSYS_MRP_UPPER, level_, msg_, para1_, para2_); } while (0)
#endif


/*===========================================================================*/

#ifndef MRP_LOWER_TRACE_00
#define MRP_LOWER_TRACE_00(idx_, level_, msg_) \
    do { (void)(idx_); LSA_TRACE_00(TRACE_SUBSYS_MRP_LOWER, level_, msg_); } while (0)
#endif

#ifndef MRP_LOWER_TRACE_01
#define MRP_LOWER_TRACE_01(idx_, level_, msg_, para1_) \
    do { (void)(idx_); LSA_TRACE_01(TRACE_SUBSYS_MRP_LOWER, level_, msg_, para1_); } while (0)
#endif

#ifndef MRP_LOWER_TRACE_02
#define MRP_LOWER_TRACE_02(idx_, level_, msg_, para1_, para2_) \
    do { (void)(idx_); LSA_TRACE_02(TRACE_SUBSYS_MRP_LOWER, level_, msg_, para1_, para2_); } while (0)
#endif

#ifndef MRP_LOWER_TRACE_03
#define MRP_LOWER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) \
    do { (void)(idx_); LSA_TRACE_03(TRACE_SUBSYS_MRP_LOWER, level_, msg_, para1_, para2_, para3_); } while (0)
#endif

#ifndef MRP_LOWER_TRACE_04
#define MRP_LOWER_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) \
    do { (void)(idx_); LSA_TRACE_04(TRACE_SUBSYS_MRP_LOWER, level_, msg_, para1_, para2_, para3_, para4_); } while (0)
#endif

#ifndef MRP_LOWER_TRACE_05
#define MRP_LOWER_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) \
    do { (void)(idx_); LSA_TRACE_05(TRACE_SUBSYS_MRP_LOWER, level_, msg_, para1_, para2_, para3_, para4_, para5_); } while (0)
#endif

#ifndef MRP_LOWER_TRACE_06
#define MRP_LOWER_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) \
    do { (void)(idx_); LSA_TRACE_06(TRACE_SUBSYS_MRP_LOWER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); } while (0)
#endif

#ifndef MRP_LOWER_TRACE_07
#define MRP_LOWER_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) \
    do { (void)(idx_); LSA_TRACE_07(TRACE_SUBSYS_MRP_LOWER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); } while (0)
#endif

#ifndef MRP_LOWER_TRACE_08
#define MRP_LOWER_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) \
    do { (void)(idx_); LSA_TRACE_08(TRACE_SUBSYS_MRP_LOWER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); } while (0)
#endif

#ifndef MRP_LOWER_TRACE_09
#define MRP_LOWER_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_) \
    do { (void)(idx_); LSA_TRACE_09(TRACE_SUBSYS_MRP_LOWER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_); } while (0)
#endif

#ifndef MRP_LOWER_TRACE_BYTE_ARRAY
#define MRP_LOWER_TRACE_BYTE_ARRAY(idx_, level_, msg_, para1_, para2_) \
    do { (void)(idx_); LSA_TRACE_BYTE_ARRAY(TRACE_SUBSYS_MRP_LOWER, level_, msg_, para1_, para2_); } while (0)
#endif

/*===========================================================================*/

#ifndef MRP_SYSTEM_TRACE_00
#define MRP_SYSTEM_TRACE_00(idx_, level_, msg_) \
    do { (void)(idx_); LSA_TRACE_00(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_); } while (0)
#endif

#ifndef MRP_SYSTEM_TRACE_01
#define MRP_SYSTEM_TRACE_01(idx_, level_, msg_, para1_) \
    do { (void)(idx_); LSA_TRACE_01(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_, para1_); } while (0)
#endif

#ifndef MRP_SYSTEM_TRACE_02
#define MRP_SYSTEM_TRACE_02(idx_, level_, msg_, para1_, para2_) \
    do { (void)(idx_); LSA_TRACE_02(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_, para1_, para2_); } while (0)
#endif

#ifndef MRP_SYSTEM_TRACE_03
#define MRP_SYSTEM_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) \
    do { (void)(idx_); LSA_TRACE_03(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_, para1_, para2_, para3_); } while (0)
#endif

#ifndef MRP_SYSTEM_TRACE_04
#define MRP_SYSTEM_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) \
    do { (void)(idx_); LSA_TRACE_04(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_, para1_, para2_, para3_, para4_); } while (0)
#endif

#ifndef MRP_SYSTEM_TRACE_05
#define MRP_SYSTEM_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) \
    do { (void)(idx_); LSA_TRACE_05(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_, para1_, para2_, para3_, para4_, para5_); } while (0)
#endif

#ifndef MRP_SYSTEM_TRACE_06
#define MRP_SYSTEM_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) \
    do { (void)(idx_); LSA_TRACE_06(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); } while (0)
#endif

#ifndef MRP_SYSTEM_TRACE_07
#define MRP_SYSTEM_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) \
    do { (void)(idx_); LSA_TRACE_07(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); } while (0)
#endif

#ifndef MRP_SYSTEM_TRACE_08
#define MRP_SYSTEM_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) \
    do { (void)(idx_); LSA_TRACE_08(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); } while (0)
#endif

#ifndef MRP_SYSTEM_TRACE_09
#define MRP_SYSTEM_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_) \
    do { (void)(idx_); LSA_TRACE_09(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_); } while (0)
#endif

#ifndef MRP_SYSTEM_TRACE_BYTE_ARRAY
#define MRP_SYSTEM_TRACE_BYTE_ARRAY(idx_, level_, msg_, para1_, para2_) \
    do { (void)(idx_); LSA_TRACE_BYTE_ARRAY(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_, para1_, para2_); } while (0)
#endif

/*===========================================================================*/

#ifndef MRP_PROGRAM_TRACE_00
#define MRP_PROGRAM_TRACE_00(idx_, level_, msg_) \
    do { (void)(idx_); LSA_TRACE_00(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_); } while (0)
#endif

#ifndef MRP_PROGRAM_TRACE_01
#define MRP_PROGRAM_TRACE_01(idx_, level_, msg_, para1_) \
    do { (void)(idx_); LSA_TRACE_01(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_, para1_); } while (0)
#endif

#ifndef MRP_PROGRAM_TRACE_02
#define MRP_PROGRAM_TRACE_02(idx_, level_, msg_, para1_, para2_) \
    do { (void)(idx_); LSA_TRACE_02(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_, para1_, para2_); } while (0)
#endif

#ifndef MRP_PROGRAM_TRACE_03
#define MRP_PROGRAM_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) \
    do { (void)(idx_); LSA_TRACE_03(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_, para1_, para2_, para3_); } while (0)
#endif

#ifndef MRP_PROGRAM_TRACE_04
#define MRP_PROGRAM_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) \
    do { (void)(idx_); LSA_TRACE_04(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_, para1_, para2_, para3_, para4_); } while (0)
#endif

#ifndef MRP_PROGRAM_TRACE_05
#define MRP_PROGRAM_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) \
    do { (void)(idx_); LSA_TRACE_05(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_, para1_, para2_, para3_, para4_, para5_); } while (0)
#endif

#ifndef MRP_PROGRAM_TRACE_06
#define MRP_PROGRAM_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) \
    do { (void)(idx_); LSA_TRACE_06(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); } while (0)
#endif

#ifndef MRP_PROGRAM_TRACE_07
#define MRP_PROGRAM_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) \
    do { (void)(idx_); LSA_TRACE_07(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); } while (0)
#endif

#ifndef MRP_PROGRAM_TRACE_08
#define MRP_PROGRAM_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) \
    do { (void)(idx_); LSA_TRACE_08(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); } while (0)
#endif

#ifndef MRP_PROGRAM_TRACE_09
#define MRP_PROGRAM_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_) \
    do { (void)(idx_); LSA_TRACE_09(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_); } while (0)
#endif

#ifndef MRP_PROGRAM_TRACE_BYTE_ARRAY
#define MRP_PROGRAM_TRACE_BYTE_ARRAY(idx_, level_, msg_, para1_, para2_) \
    do { (void)(idx_); LSA_TRACE_BYTE_ARRAY(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_, para1_, para2_); } while (0)
#endif

/*===========================================================================*/
#elif MRP_CFG_TRACE_MODE == 2

/* hhr: to reduce code size and increase performance (if many sequential traces) us a local variable. */ 
#ifndef MRP_TRACE_CREATE_LOCAL_VARIABLE
#define MRP_TRACE_CREATE_LOCAL_VARIABLE(name_, variable_) \
  LSA_UINT32 name_ = variable_
#endif

#ifndef MRP_UPPER_TRACE_00
#define MRP_UPPER_TRACE_00(idx_, level_, msg_) \
  LSA_IDX_TRACE_00(TRACE_SUBSYS_MRP_UPPER, idx_, level_, msg_)
#endif

#ifndef MRP_UPPER_TRACE_01
#define MRP_UPPER_TRACE_01(idx_, level_, msg_, para1_) \
  LSA_IDX_TRACE_01(TRACE_SUBSYS_MRP_UPPER, idx_, level_, msg_, para1_)
#endif

#ifndef MRP_UPPER_TRACE_02
#define MRP_UPPER_TRACE_02(idx_, level_, msg_, para1_, para2_) \
  LSA_IDX_TRACE_02(TRACE_SUBSYS_MRP_UPPER, idx_, level_, msg_, para1_, para2_)
#endif

#ifndef MRP_UPPER_TRACE_03
#define MRP_UPPER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) \
  LSA_IDX_TRACE_03(TRACE_SUBSYS_MRP_UPPER, idx_, level_, msg_, para1_, para2_, para3_)
#endif

#ifndef MRP_UPPER_TRACE_04
#define MRP_UPPER_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) \
  LSA_IDX_TRACE_04(TRACE_SUBSYS_MRP_UPPER, idx_, level_, msg_, para1_, para2_, para3_, para4_)
#endif

#ifndef MRP_UPPER_TRACE_05
#define MRP_UPPER_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) \
  LSA_IDX_TRACE_05(TRACE_SUBSYS_MRP_UPPER, idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)
#endif

#ifndef MRP_UPPER_TRACE_06
#define MRP_UPPER_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) \
  LSA_IDX_TRACE_06(TRACE_SUBSYS_MRP_UPPER, idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)
#endif

#ifndef MRP_UPPER_TRACE_07
#define MRP_UPPER_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) \
  LSA_IDX_TRACE_07(TRACE_SUBSYS_MRP_UPPER, idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)
#endif

#ifndef MRP_UPPER_TRACE_08
#define MRP_UPPER_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) \
  LSA_IDX_TRACE_08(TRACE_SUBSYS_MRP_UPPER, idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)
#endif

#ifndef MRP_UPPER_TRACE_09
#define MRP_UPPER_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_) \
  LSA_IDX_TRACE_09(TRACE_SUBSYS_MRP_UPPER, idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)
#endif

#ifndef MRP_UPPER_TRACE_BYTE_ARRAY
#define MRP_UPPER_TRACE_BYTE_ARRAY(idx_, level_, msg_, para1_, para2_) \
  LSA_IDX_TRACE_BYTE_ARRAY(TRACE_SUBSYS_MRP_UPPER, idx_, level_, msg_, para1_, para2_)
#endif

/*===========================================================================*/

#ifndef MRP_LOWER_TRACE_00
#define MRP_LOWER_TRACE_00(idx_, level_, msg_) \
  LSA_IDX_TRACE_00(TRACE_SUBSYS_MRP_LOWER, idx_, level_, msg_)
#endif

#ifndef MRP_LOWER_TRACE_01
#define MRP_LOWER_TRACE_01(idx_, level_, msg_, para1_) \
  LSA_IDX_TRACE_01(TRACE_SUBSYS_MRP_LOWER, idx_, level_, msg_, para1_)
#endif

#ifndef MRP_LOWER_TRACE_02
#define MRP_LOWER_TRACE_02(idx_, level_, msg_, para1_, para2_) \
  LSA_IDX_TRACE_02(TRACE_SUBSYS_MRP_LOWER, idx_, level_, msg_, para1_, para2_)
#endif

#ifndef MRP_LOWER_TRACE_03
#define MRP_LOWER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) \
  LSA_IDX_TRACE_03(TRACE_SUBSYS_MRP_LOWER, idx_, level_, msg_, para1_, para2_, para3_)
#endif

#ifndef MRP_LOWER_TRACE_04
#define MRP_LOWER_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) \
  LSA_IDX_TRACE_04(TRACE_SUBSYS_MRP_LOWER, idx_, level_, msg_, para1_, para2_, para3_, para4_)
#endif

#ifndef MRP_LOWER_TRACE_05
#define MRP_LOWER_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) \
  LSA_IDX_TRACE_05(TRACE_SUBSYS_MRP_LOWER, idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)
#endif

#ifndef MRP_LOWER_TRACE_06
#define MRP_LOWER_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) \
  LSA_IDX_TRACE_06(TRACE_SUBSYS_MRP_LOWER, idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)
#endif

#ifndef MRP_LOWER_TRACE_07
#define MRP_LOWER_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) \
  LSA_IDX_TRACE_07(TRACE_SUBSYS_MRP_LOWER, idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)
#endif

#ifndef MRP_LOWER_TRACE_08
#define MRP_LOWER_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) \
  LSA_IDX_TRACE_08(TRACE_SUBSYS_MRP_LOWER, idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)
#endif

#ifndef MRP_LOWER_TRACE_09
#define MRP_LOWER_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_) \
  LSA_IDX_TRACE_09(TRACE_SUBSYS_MRP_LOWER, idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)
#endif

#ifndef MRP_LOWER_TRACE_BYTE_ARRAY
#define MRP_LOWER_TRACE_BYTE_ARRAY(idx_, level_, msg_, para1_, para2_) \
  LSA_IDX_TRACE_BYTE_ARRAY(TRACE_SUBSYS_MRP_LOWER, idx_, level_, msg_, para1_, para2_)
#endif

/*===========================================================================*/

#ifndef MRP_SYSTEM_TRACE_00
#define MRP_SYSTEM_TRACE_00(idx_, level_, msg_) \
  LSA_IDX_TRACE_00(TRACE_SUBSYS_MRP_SYSTEM, idx_, level_, msg_)
#endif

#ifndef MRP_SYSTEM_TRACE_01
#define MRP_SYSTEM_TRACE_01(idx_, level_, msg_, para1_) \
  LSA_IDX_TRACE_01(TRACE_SUBSYS_MRP_SYSTEM, idx_, level_, msg_, para1_)
#endif

#ifndef MRP_SYSTEM_TRACE_02
#define MRP_SYSTEM_TRACE_02(idx_, level_, msg_, para1_, para2_) \
  LSA_IDX_TRACE_02(TRACE_SUBSYS_MRP_SYSTEM, idx_, level_, msg_, para1_, para2_)
#endif

#ifndef MRP_SYSTEM_TRACE_03
#define MRP_SYSTEM_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) \
  LSA_IDX_TRACE_03(TRACE_SUBSYS_MRP_SYSTEM, idx_, level_, msg_, para1_, para2_, para3_)
#endif

#ifndef MRP_SYSTEM_TRACE_04
#define MRP_SYSTEM_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) \
  LSA_IDX_TRACE_04(TRACE_SUBSYS_MRP_SYSTEM, idx_, level_, msg_, para1_, para2_, para3_, para4_)
#endif

#ifndef MRP_SYSTEM_TRACE_05
#define MRP_SYSTEM_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) \
  LSA_IDX_TRACE_05(TRACE_SUBSYS_MRP_SYSTEM, idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)
#endif

#ifndef MRP_SYSTEM_TRACE_06
#define MRP_SYSTEM_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) \
  LSA_IDX_TRACE_06(TRACE_SUBSYS_MRP_SYSTEM, idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)
#endif

#ifndef MRP_SYSTEM_TRACE_07
#define MRP_SYSTEM_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) \
  LSA_IDX_TRACE_07(TRACE_SUBSYS_MRP_SYSTEM, idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)
#endif

#ifndef MRP_SYSTEM_TRACE_08
#define MRP_SYSTEM_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) \
  LSA_IDX_TRACE_08(TRACE_SUBSYS_MRP_SYSTEM, idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)
#endif

#ifndef MRP_SYSTEM_TRACE_09
#define MRP_SYSTEM_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_) \
  LSA_IDX_TRACE_09(TRACE_SUBSYS_MRP_SYSTEM, idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)
#endif

#ifndef MRP_SYSTEM_TRACE_BYTE_ARRAY
#define MRP_SYSTEM_TRACE_BYTE_ARRAY(idx_, level_, msg_, para1_, para2_) \
  LSA_IDX_TRACE_BYTE_ARRAY(TRACE_SUBSYS_MRP_SYSTEM, idx_, level_, msg_, para1_, para2_)
#endif


/*===========================================================================*/

#ifndef MRP_PROGRAM_TRACE_00
#define MRP_PROGRAM_TRACE_00(idx_, level_, msg_) \
  LSA_IDX_TRACE_00(TRACE_SUBSYS_MRP_PROGRAM, idx_, level_, msg_)
#endif

#ifndef MRP_PROGRAM_TRACE_01
#define MRP_PROGRAM_TRACE_01(idx_, level_, msg_, para1_) \
  LSA_IDX_TRACE_01(TRACE_SUBSYS_MRP_PROGRAM, idx_, level_, msg_, para1_)
#endif

#ifndef MRP_PROGRAM_TRACE_02
#define MRP_PROGRAM_TRACE_02(idx_, level_, msg_, para1_, para2_) \
  LSA_IDX_TRACE_02(TRACE_SUBSYS_MRP_PROGRAM, idx_, level_, msg_, para1_, para2_)
#endif

#ifndef MRP_PROGRAM_TRACE_03
#define MRP_PROGRAM_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_) \
  LSA_IDX_TRACE_03(TRACE_SUBSYS_MRP_PROGRAM, idx_, level_, msg_, para1_, para2_, para3_)
#endif

#ifndef MRP_PROGRAM_TRACE_04
#define MRP_PROGRAM_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) \
  LSA_IDX_TRACE_04(TRACE_SUBSYS_MRP_PROGRAM, idx_, level_, msg_, para1_, para2_, para3_, para4_)
#endif

#ifndef MRP_PROGRAM_TRACE_05
#define MRP_PROGRAM_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) \
  LSA_IDX_TRACE_05(TRACE_SUBSYS_MRP_PROGRAM, idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)
#endif

#ifndef MRP_PROGRAM_TRACE_06
#define MRP_PROGRAM_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) \
  LSA_IDX_TRACE_06(TRACE_SUBSYS_MRP_PROGRAM, idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)
#endif

#ifndef MRP_PROGRAM_TRACE_07
#define MRP_PROGRAM_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) \
  LSA_IDX_TRACE_07(TRACE_SUBSYS_MRP_PROGRAM, idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)
#endif

#ifndef MRP_PROGRAM_TRACE_08
#define MRP_PROGRAM_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) \
  LSA_IDX_TRACE_08(TRACE_SUBSYS_MRP_PROGRAM, idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)
#endif

#ifndef MRP_PROGRAM_TRACE_09
#define MRP_PROGRAM_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_) \
  LSA_IDX_TRACE_09(TRACE_SUBSYS_MRP_PROGRAM, idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)
#endif

#ifndef MRP_PROGRAM_TRACE_BYTE_ARRAY
#define MRP_PROGRAM_TRACE_BYTE_ARRAY(idx_, level_, msg_, para1_, para2_) \
  LSA_IDX_TRACE_BYTE_ARRAY(TRACE_SUBSYS_MRP_PROGRAM, idx_, level_, msg_, para1_, para2_)
#endif

/*===========================================================================*/
#elif MRP_CFG_TRACE_MODE == 0 /* no trace */

#define MRP_TRACE_CREATE_LOCAL_VARIABLE(name_, variable_)

/*===========================================================================*/

#define MRP_UPPER_TRACE_00(idx_, level_, msg_)
#define MRP_UPPER_TRACE_01(idx_, level_, msg_, para1_)
#define MRP_UPPER_TRACE_02(idx_, level_, msg_, para1_, para2_)
#define MRP_UPPER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)
#define MRP_UPPER_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)
#define MRP_UPPER_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)
#define MRP_UPPER_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)
#define MRP_UPPER_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)
#define MRP_UPPER_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)
#define MRP_UPPER_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)

#define MRP_UPPER_TRACE_BYTE_ARRAY(idx_, level_, msg_, para1_, para2_)
/*===========================================================================*/

#define MRP_LOWER_TRACE_00(idx_, level_, msg_)
#define MRP_LOWER_TRACE_01(idx_, level_, msg_, para1_)
#define MRP_LOWER_TRACE_02(idx_, level_, msg_, para1_, para2_)
#define MRP_LOWER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)
#define MRP_LOWER_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)
#define MRP_LOWER_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)
#define MRP_LOWER_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)
#define MRP_LOWER_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)
#define MRP_LOWER_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)
#define MRP_LOWER_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)

#define MRP_LOWER_TRACE_BYTE_ARRAY(idx_, level_, msg_, para1_, para2_)
/*===========================================================================*/

#define MRP_SYSTEM_TRACE_00(idx_, level_, msg_)
#define MRP_SYSTEM_TRACE_01(idx_, level_, msg_, para1_)
#define MRP_SYSTEM_TRACE_02(idx_, level_, msg_, para1_, para2_)
#define MRP_SYSTEM_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)
#define MRP_SYSTEM_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)
#define MRP_SYSTEM_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)
#define MRP_SYSTEM_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)
#define MRP_SYSTEM_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)
#define MRP_SYSTEM_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)
#define MRP_SYSTEM_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)

#define MRP_SYSTEM_TRACE_BYTE_ARRAY(idx_, level_, msg_, para1_, para2_)
/*===========================================================================*/

#define MRP_PROGRAM_TRACE_00(idx_, level_, msg_)
#define MRP_PROGRAM_TRACE_01(idx_, level_, msg_, para1_)
#define MRP_PROGRAM_TRACE_02(idx_, level_, msg_, para1_, para2_)
#define MRP_PROGRAM_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_)
#define MRP_PROGRAM_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_)
#define MRP_PROGRAM_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_)
#define MRP_PROGRAM_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_)
#define MRP_PROGRAM_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_)
#define MRP_PROGRAM_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_)
#define MRP_PROGRAM_TRACE_09(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_, para9_)

#define MRP_PROGRAM_TRACE_BYTE_ARRAY(idx_, level_, msg_, para1_, para2_)
/*===========================================================================*/


#else

#error "MRP_CFG_TRACE_MODE has unknown value"

#endif /* if MRP_CFG_TRACE_MODE == 1 */

/*===========================================================================*/

/*----------------------------------------------------------------------------*/
#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of MRP_TRC_H */
