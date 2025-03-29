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
/*  F i l e               &F: mrp_cfg.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Configuration for mrp:                                                   */
/*  Defines constants, types and macros for mrp.                             */
/*                                                                           */
/*  This file has to be overwritten during system integration, because       */
/*  some definitions depend on the different system, compiler or             */
/*  operating system.                                                        */
/*                                                                           */
/*****************************************************************************/
#ifndef MRP_CFG_H
#define MRP_CFG_H


#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef MRP_MESSAGE
/*  2008-01-07  mk    EDD_SRV_NRT_CANCEL added.                              */
/*  2008-02-29  mk    MRP_UPPER_TRACE_03 added.                              */
/*  2008-04-16  mk    MRP_UPPER_TRACE_04 added.                              */
/*  2008-07-02  mk    MRP_LOWER_TRACE_07 added.                              */
/*  2009-01-15  ds    MRP_UPPER_TRACE_05 added, MRP_UPPER_TRACE_04 corrected */
/*  2009-02-17  ds    MRRT services removed                                       */
/*  2009-07-02  ds    MRP flag MRP_CFG_COMP_MRRT removed                       */
/*  2009-10-15  ds    minor cleanups                                         */
/*  2010-01-27  ds    Each trace subsystem supports up to 7 arguments now    */
/*  2010-01-28  ds    Each trace subsystem supports up to 8 arguments now    */
/*  2010-02-08  ds    get rid of redundant HSR specific defines              */
/*  2010-02-16  ds    accelerated MRM link change handling implemented       */
/*  2010-04-16  ds    according to IEC 62439-2, MRP_linkChange handling is   */
/*                    mandatory for MRM and is enabled as default from now on*/
/*  2010-05-25  ds    OHA lower defines discarded                            */
/*  2010-07-19  ds    LSA trace unified according to RQ AP00999113           */
/*                                                                           */
#endif
/*****************************************************************************/

/*===========================================================================*/
/*                             compiler-switches                             */
/*===========================================================================*/

/*=============================================================================
 *       set local_mem static or dynamic (not used here!):
 * static (no use of MRP_ALLOC_LOCAL_MEM):
#define MRP_LOCAL_MEM_STATIC
 * or dynamic (use of MRP_ALLOC_LOCAL_MEM):
#define MRP_LOCAL_MEM_DYNAMIC
 *===========================================================================*/
#define MRP_LOCAL_MEM_DYNAMIC

/*=============================================================================
 *       clear global data (not used here!):
 * clear (MRP_DATA_TYPE mrp_data = {0};):
#define MRP_CLEAR_GLOBAL_DATA
 * or no clear:
#define MRP_NO_CLEAR_GLOBAL_DATA
 *===========================================================================*/
#define MRP_NO_CLEAR_GLOBAL_DATA


/*===========================================================================*/
/*  mrp allocates more tx buffer than needed. This extra space is used to    */
/*  place queue info.                                                        */
/*  If defined mrp put extra info to the end in tx buffer.                   */
/*===========================================================================*/
#undef  MRP_CFG_TX_BUFFER_EXTRA_INFO_TO_END

/*===========================================================================*/
/*  defines if file name will be in LSA_FATAL_ERROR_TYPE or not              */
/*===========================================================================*/
#define  MRP_CFG_NO_FATAL_FILE_INFO


/*===========================================================================*/
/*  defines the Trace mode for MRP                                           */
/*  0: no LSA trace  or external traces                                      */
/*  1: enable LSA trace and use LTRC (LSA-Trace module)                      */
/*     see mrp_trc.h [default]                                               */
/*  2: enable LSA indexed trace for multiple ethernet interface usage (N-IF) */
/*===========================================================================*/
#define MRP_CFG_TRACE_MODE 1


// ==> FIX warning #550-D: variable "traceIdx" was set but never used
#if MRP_CFG_TRACE_MODE == 1

    /*===========================================================================*/

    #ifndef MRP_UPPER_TRACE_00
    #define MRP_UPPER_TRACE_00(idx_, level_, msg_) {\
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_00(TRACE_SUBSYS_MRP_UPPER, level_, msg_); }
    #endif

    #ifndef MRP_UPPER_TRACE_01
    #define MRP_UPPER_TRACE_01(idx_, level_, msg_, para1_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_01(TRACE_SUBSYS_MRP_UPPER, level_, msg_, para1_); }
    #endif

    #ifndef MRP_UPPER_TRACE_02
    #define MRP_UPPER_TRACE_02(idx_, level_, msg_, para1_, para2_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_02(TRACE_SUBSYS_MRP_UPPER, level_, msg_, para1_, para2_); }
    #endif

    #ifndef MRP_UPPER_TRACE_03
    #define MRP_UPPER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_03(TRACE_SUBSYS_MRP_UPPER, level_, msg_, para1_, para2_, para3_); }
    #endif

    #ifndef MRP_UPPER_TRACE_04
    #define MRP_UPPER_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_04(TRACE_SUBSYS_MRP_UPPER, level_, msg_, para1_, para2_, para3_, para4_); }
    #endif

    #ifndef MRP_UPPER_TRACE_05
    #define MRP_UPPER_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_05(TRACE_SUBSYS_MRP_UPPER, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
    #endif

    #ifndef MRP_UPPER_TRACE_06
    #define MRP_UPPER_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_06(TRACE_SUBSYS_MRP_UPPER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
    #endif

    #ifndef MRP_UPPER_TRACE_07
    #define MRP_UPPER_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_){ \
      LSA_UNUSED_ARG(idx_);\
      LSA_TRACE_07(TRACE_SUBSYS_MRP_UPPER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
    #endif

    #ifndef MRP_UPPER_TRACE_08
    #define MRP_UPPER_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_){ \
      LSA_UNUSED_ARG(idx_);\
      LSA_TRACE_08(TRACE_SUBSYS_MRP_UPPER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
    #endif

    /*===========================================================================*/

    #ifndef MRP_LOWER_TRACE_00
    #define MRP_LOWER_TRACE_00(idx_, level_, msg_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_00(TRACE_SUBSYS_MRP_LOWER, level_, msg_); }
    #endif

    #ifndef MRP_LOWER_TRACE_01
    #define MRP_LOWER_TRACE_01(idx_, level_, msg_, para1_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_01(TRACE_SUBSYS_MRP_LOWER, level_, msg_, para1_); }
    #endif

    #ifndef MRP_LOWER_TRACE_02
    #define MRP_LOWER_TRACE_02(idx_, level_, msg_, para1_, para2_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_02(TRACE_SUBSYS_MRP_LOWER, level_, msg_, para1_, para2_); }
    #endif

    #ifndef MRP_LOWER_TRACE_03
    #define MRP_LOWER_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_03(TRACE_SUBSYS_MRP_LOWER, level_, msg_, para1_, para2_, para3_); }
    #endif

    #ifndef MRP_LOWER_TRACE_04
    #define MRP_LOWER_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_04(TRACE_SUBSYS_MRP_LOWER, level_, msg_, para1_, para2_, para3_, para4_); }
    #endif

    #ifndef MRP_LOWER_TRACE_05
    #define MRP_LOWER_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_05(TRACE_SUBSYS_MRP_LOWER, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
    #endif

    #ifndef MRP_LOWER_TRACE_06
    #define MRP_LOWER_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_06(TRACE_SUBSYS_MRP_LOWER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
    #endif

    #ifndef MRP_LOWER_TRACE_07
    #define MRP_LOWER_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_07(TRACE_SUBSYS_MRP_LOWER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
    #endif

    #ifndef MRP_LOWER_TRACE_08
    #define MRP_LOWER_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_08(TRACE_SUBSYS_MRP_LOWER, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
    #endif

    /*===========================================================================*/

    #ifndef MRP_SYSTEM_TRACE_00
    #define MRP_SYSTEM_TRACE_00(idx_, level_, msg_) { \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_00(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_); }
    #endif

    #ifndef MRP_SYSTEM_TRACE_01
    #define MRP_SYSTEM_TRACE_01(idx_, level_, msg_, para1_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_01(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_, para1_); }
    #endif

    #ifndef MRP_SYSTEM_TRACE_02
    #define MRP_SYSTEM_TRACE_02(idx_, level_, msg_, para1_, para2_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_02(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_, para1_, para2_); }
    #endif

    #ifndef MRP_SYSTEM_TRACE_03
    #define MRP_SYSTEM_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_03(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_, para1_, para2_, para3_); }
    #endif

    #ifndef MRP_SYSTEM_TRACE_04
    #define MRP_SYSTEM_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_04(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_, para1_, para2_, para3_, para4_); }
    #endif

    #ifndef MRP_SYSTEM_TRACE_05
    #define MRP_SYSTEM_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_05(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
    #endif

    #ifndef MRP_SYSTEM_TRACE_06
    #define MRP_SYSTEM_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_06(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
    #endif

    #ifndef MRP_SYSTEM_TRACE_07
    #define MRP_SYSTEM_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_07(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
    #endif

    #ifndef MRP_SYSTEM_TRACE_08
    #define MRP_SYSTEM_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_08(TRACE_SUBSYS_MRP_SYSTEM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
    #endif

    /*===========================================================================*/

    #ifndef MRP_PROGRAM_TRACE_00
    #define MRP_PROGRAM_TRACE_00(idx_, level_, msg_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_00(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_); }
    #endif

    #ifndef MRP_PROGRAM_TRACE_01
    #define MRP_PROGRAM_TRACE_01(idx_, level_, msg_, para1_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_01(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_, para1_); }
    #endif

    #ifndef MRP_PROGRAM_TRACE_02
    #define MRP_PROGRAM_TRACE_02(idx_, level_, msg_, para1_, para2_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_02(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_, para1_, para2_); }
    #endif

    #ifndef MRP_PROGRAM_TRACE_03
    #define MRP_PROGRAM_TRACE_03(idx_, level_, msg_, para1_, para2_, para3_){ \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_03(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_, para1_, para2_, para3_); }
    #endif

    #ifndef MRP_PROGRAM_TRACE_04
    #define MRP_PROGRAM_TRACE_04(idx_, level_, msg_, para1_, para2_, para3_, para4_) { \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_04(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_, para1_, para2_, para3_, para4_); }
    #endif

    #ifndef MRP_PROGRAM_TRACE_05
    #define MRP_PROGRAM_TRACE_05(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_) { \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_05(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_, para1_, para2_, para3_, para4_, para5_); }
    #endif

    #ifndef MRP_PROGRAM_TRACE_06
    #define MRP_PROGRAM_TRACE_06(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_) { \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_06(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_); }
    #endif

    #ifndef MRP_PROGRAM_TRACE_07
    #define MRP_PROGRAM_TRACE_07(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_) { \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_07(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_); }
    #endif

    #ifndef MRP_PROGRAM_TRACE_08
    #define MRP_PROGRAM_TRACE_08(idx_, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_) { \
        LSA_UNUSED_ARG(idx_);\
        LSA_TRACE_08(TRACE_SUBSYS_MRP_PROGRAM, level_, msg_, para1_, para2_, para3_, para4_, para5_, para6_, para7_, para8_); }
    #endif

#endif /* MRP_CFG_TRACE_MODE == 1 */



/*===========================================================================
 * If the LSA component LSA-trace isn't used for trace in the target system,
 * then the prefix trace macros can be defined here.
 *
 * See also cm_trc.h and lsa_cfg.h/txt.
 *
 *===========================================================================*/
/*===========================================================================*/
/*  component configuration defines:                                         */
/*===========================================================================*/

/*===========================================================================*/
/*  plugin configuration defines:                                            */
/*===========================================================================*/
//#define   MRP_CFG_PLUGIN_0_MRM
//#define   MRP_CFG_PLUGIN_1_MRM_EXT

/*---------------------------------------------------------------------------*/
/*  only applicable for MRM:                                                 */
/*       MRP_CFG_TST_SHORT_TIME: mandatory for IEC 62439-2                   */
/*       send test frames with a shorter test interval, if MRP LinkChange is */
/*       received (MRP_CFG_TST_SHORT_TIME_EXT must not be configured, if     */
/*       MRP_CFG_TST_SHORT_TIME is set)                                      */
/*                                                                           */
/*       MRP_CFG_TST_SHORT_TIME_EXT: optional for Siemens devices            */
/*       send test frames with a shorter test interval in an optimized       */
/*       manner, if MRP LinkChange is received. MRP reconfiguration time is  */
/*       accelerated, if this method is used                                 */
/*       (MRP_CFG_TST_SHORT_TIME must not be configured, if                  */
/*       MRP_CFG_TST_SHORT_TIME_EXT is set)                                  */
/*                                                                           */
/*       if MRP_CFG_TST_SHORT_TIME/MRP_CFG_TST_SHORT_TIME_EXT is set,        */
/*       than MRP_TSTShortT will be activated.                               */
/*---------------------------------------------------------------------------*/
#undef  MRP_CFG_TST_SHORT_TIME        /* enable standardized evaluation of MRP link change frames for standard MRM            */

/*===========================================================================*/
/*                              basic attributes                             */
/*===========================================================================*/


/*---------------------------------------------------------------------------*/
/*                         code- and data-attributes                         */
/*---------------------------------------------------------------------------*/



/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/

/*=============================================================================
 *      mrp request block (== upper request block)
 *===========================================================================*/

/*====  Definition of MRP-RQB-HEADER:  =====*/
#define MRP_RQB_HEADER            LSA_RQB_HEADER(MRP_UPPER_RQB_PTR_TYPE)

/*====  Definition of MRP-RQB-TRAILER:  =====*/
/* If necessary during system integration, additional parameters can be
 * added!
 */
#define MRP_RQB_TRAILER           LSA_RQB_TRAILER

/*=============================================================================
 *      lower request block
 *===========================================================================*/

#define MRP_EDD_LOWER_RQB_TYPE        EDD_RQB_TYPE

#define MRP_EDD_LOWER_RQB_PTR_TYPE                  /* pointer to lower-RQB (defined here to serve mrp_usr.h interface) */ \
    MRP_EDD_LOWER_RQB_TYPE *


/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/

/* maximum number of ethernet interfaces */
#define MRP_MAX_NO_OF_INTERFACES        4

/*===========================================================================*/
/*                                 macros                                    */
/*===========================================================================*/

/*=============================================================================
 * macro name:    MRP_ARG_UNUSED(arg_)
 *
 * macro:         In addition to LSA_UNUSED_ARG this macro is used for
 *                unused automatic variables which are declared as const and
 *                union.
 *
 * parameter:     arg_: unused variable
 *
 *                There's no prototype for that macro!
 *===========================================================================*/
#define MRP_ARG_UNUSED(arg_)          if (&arg_) {}


/*=============================================================================
 * macro name:    MRP_FILE_SYSTEM_EXTENSION(module_id_)
 *
 * macro:         System integrators can use the macro to smuggle something
 *                into the component source files. By default it's empty.
 *
 *                The system integrators are responsible for the contents of
 *                that macro.
 *
 *                Internal data of LSA components may not be changed!
 *
 * parameter:     Module-id
 *
 *                There's no prototype for that macro!
 *===========================================================================*/
#define MRP_FILE_SYSTEM_EXTENSION(module_id_)


/* system interface output macros (not used here!)
 * Note: There aren't any prototypes for these macros so that the target system
 * defines the pointer size and not mrp.
 * A function can be nevertheless invoked within the macro.
 * Attention: mrp mustn't cast the parameters at the call.
 */

/*=============================================================================
 * macro names:   MRP_PUT_INFO_X  (X = 0, 1, 2, 3, ..., 16)
 *
 * macro:         mrp informs the system.
 *                Some target systems need information of some LSA-components
 *                for diagnosis purposes.
 *                Example of information: Internal state changes
 *
 * parameters:    info_id_: kind if info
 *                para1_:   info 1
 *                para2_:   info 2
 *                 ...        ...
 *                para16_:  info 16
 *                          Examples for info: handle (of channel),
 *                                             internal state,
 *                                             error-code,
 *                                             rqb-pointer
 * return value:  LSA_VOID
 *===========================================================================*/
#define MRP_PUT_INFO_0(info_id_)
#define MRP_PUT_INFO_1(info_id_, para1_)
#define MRP_PUT_INFO_2(info_id_, para1_, para2_)
#define MRP_PUT_INFO_3(info_id_, para1_, para2_, para3_)
#define MRP_PUT_INFO_4(info_id_, para1_, para2_, para3_, para4_)
#define MRP_PUT_INFO_5(info_id_, para1_, para2_, para3_, para4_, para5_)
#define MRP_PUT_INFO_6(info_id_, para1_, para2_, para3_, para4_, para5_, \
                          para6_)
#define MRP_PUT_INFO_7(info_id_, para1_, para2_, para3_, para4_, para5_, \
                          para6_, para7_)
#define MRP_PUT_INFO_8(info_id_, para1_, para2_, para3_, para4_, para5_, \
                          para6_, para7_, para8_)
#define MRP_PUT_INFO_9(info_id_, para1_, para2_, para3_, para4_, para5_, \
                          para6_, para7_, para8_, para9_)
#define MRP_PUT_INFO_10(info_id_, para1_, para2_, para3_, para4_, para5_, \
                           para6_, para7_, para8_, para9_, para10_)
#define MRP_PUT_INFO_11(info_id_, para1_, para2_, para3_, para4_, para5_, \
                           para6_, para7_, para8_, para9_, para10_, para11_)
#define MRP_PUT_INFO_12(info_id_, para1_, para2_, para3_, para4_, para5_, \
                           para6_, para7_, para8_, para9_, para10_, para11_, \
                           para12_)
#define MRP_PUT_INFO_13(info_id_, para1_, para2_, para3_, para4_, para5_, \
                           para6_, para7_, para8_, para9_, para10_, para11_, \
                           para12_, para13_)
#define MRP_PUT_INFO_14(info_id_, para1_, para2_, para3_, para4_, para5_, \
                           para6_, para7_, para8_, para9_, para10_, para11_, \
                           para12_, para13_, para14_)
#define MRP_PUT_INFO_15(info_id_, para1_, para2_, para3_, para4_, para5_, \
                           para6_, para7_, para8_, para9_, para10_, para11_, \
                           para12_, para13_, para14_, para15_)
#define MRP_PUT_INFO_16(info_id_, para1_, para2_, para3_, para4_, para5_, \
                           para6_, para7_, para8_, para9_, para10_, para11_, \
                           para12_, para13_, para14_, para15_, para16_)




/*===========================================================================*/
/* LOWER LAYER MACROS                                                        */
/*===========================================================================*/

/* edd */
#define MRP_EDD_LOWER_SET_OPCODE(_pRQB, _value)     EDD_RQB_SET_OPCODE(_pRQB, _value)
#define MRP_EDD_LOWER_GET_OPCODE(_pRQB)             EDD_RQB_GET_OPCODE(_pRQB)

#define MRP_EDD_LOWER_SET_SERVICE(_pRQB, _value)    EDD_RQB_SET_SERVICE(_pRQB, _value)
#define MRP_EDD_LOWER_GET_SERVICE(_pRQB)            EDD_RQB_GET_SERVICE(_pRQB)

#define MRP_EDD_LOWER_SET_HANDLE(_pRQB, _value)     EDD_RQB_SET_HANDLE_LOWER(_pRQB, _value)
#define MRP_EDD_LOWER_GET_HANDLE(_pRQB)             EDD_RQB_GET_HANDLE_LOWER(_pRQB)

#define MRP_EDD_LOWER_SET_USERID_PTR(_pRQB, _value) EDD_RQB_SET_USERID_PTR(_pRQB, _value)
#define MRP_EDD_LOWER_GET_USERID_PTR(_pRQB)         EDD_RQB_GET_USERID_PTR(_pRQB)

#define MRP_EDD_LOWER_SET_RESPONSE(_pRQB, _value)   EDD_RQB_SET_RESPONSE(_pRQB, _value)
#define MRP_EDD_LOWER_GET_RESPONSE(_pRQB)           EDD_RQB_GET_RESPONSE(_pRQB)

#define MRP_EDD_LOWER_SET_PPARAM(_pRQB, _value)     EDD_RQB_SET_PPARAM(_pRQB, _value)

#define MRP_EDD_LOWER_GET_PPARAM_USED_DATA_PTR(_pRQB)      (MRP_EDD_LOWER_USED_DATA_PTR_TYPE)               EDD_RQB_GET_PPARAM(_pRQB)
#define MRP_EDD_LOWER_GET_PPARAM_OPEN_CHANNEL(_pRQB)       (MRP_EDD_LOWER_OPEN_CHANNEL_PTR_TYPE)            EDD_RQB_GET_PPARAM(_pRQB)
#define MRP_EDD_LOWER_GET_PPARAM_NRT_CANCEL(_pRQB)         (MRP_EDD_UPPER_NRT_CANCEL_PTR_TYPE)              EDD_RQB_GET_PPARAM(_pRQB)
#define MRP_EDD_LOWER_GET_PPARAM_GET_PARAMS(_pRQB)         (MRP_EDD_UPPER_GET_PARAMS_PTR_TYPE)              EDD_RQB_GET_PPARAM(_pRQB)
#define MRP_EDD_LOWER_GET_PPARAM_GET_PORT_PARAMS(_pRQB)    (MRP_EDD_UPPER_GET_PORT_PARAMS_PTR_TYPE)         EDD_RQB_GET_PPARAM(_pRQB)
#define MRP_EDD_LOWER_GET_PPARAM_NRT_SEND(_pRQB)           (MRP_EDD_UPPER_NRT_SEND_PTR_TYPE)                EDD_RQB_GET_PPARAM(_pRQB)
#define MRP_EDD_LOWER_GET_PPARAM_NRT_RECV(_pRQB)           (MRP_EDD_UPPER_NRT_RECV_PTR_TYPE)                EDD_RQB_GET_PPARAM(_pRQB)
#define MRP_EDD_LOWER_GET_PPARAM_LINK_STATUS_IND(_pRQB)    (MRP_EDD_UPPER_LINK_STATUS_IND_PROVIDE_PTR_TYPE) EDD_RQB_GET_PPARAM(_pRQB)
#define MRP_EDD_LOWER_GET_PPARAM_MULTICAST(_pRQB)          (MRP_EDD_UPPER_MULTICAST_PTR_TYPE)               EDD_RQB_GET_PPARAM(_pRQB)
#define MRP_EDD_LOWER_GET_PPARAM_MULTICAST_FWD_CTRL(_pRQB) (MRP_EDD_UPPER_MULTICAST_FWD_CTRL_PTR_TYPE)      EDD_RQB_GET_PPARAM(_pRQB)
#define MRP_EDD_LOWER_GET_PPARAM_FLUSH_FILTERING_DB(_pRQB) (MRP_EDD_UPPER_SWI_FLUSH_FILTERING_DB_PTR_TYPE)  EDD_RQB_GET_PPARAM(_pRQB)
#define MRP_EDD_LOWER_GET_PPARAM_SET_PORT_STATE(_pRQB)     (MRP_EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE)      EDD_RQB_GET_PPARAM(_pRQB)


#define MRP_EDD_LOWER_GET_NRT_TX_BUFFER(_pRQB)         (((EDD_UPPER_NRT_SEND_PTR_TYPE)(EDD_RQB_GET_PPARAM(_pRQB)))->pBuffer)

/* mrp */
#define MRP_RQB_SET_NEXT_RQB_PTR(rb,v)      LSA_RQB_SET_NEXT_RQB_PTR(rb,v)
#define MRP_RQB_SET_PREV_RQB_PTR(rb,v)      LSA_RQB_SET_PREV_RQB_PTR(rb,v)
#define MRP_RQB_SET_OPCODE(rb,v)            LSA_RQB_SET_OPCODE(rb,v)
#define MRP_RQB_SET_HANDLE(rb,v)            LSA_RQB_SET_HANDLE(rb,v)
#define MRP_RQB_SET_RESPONSE(rb,v)          LSA_RQB_SET_RESPONSE(rb,v)
#define MRP_RQB_SET_USER_ID_UVAR16(rb,v)    LSA_RQB_SET_USER_ID_UVAR16(rb, v)



#define MRP_RQB_GET_NEXT_RQB_PTR(rb)        LSA_RQB_GET_NEXT_RQB_PTR(rb)
#define MRP_RQB_GET_PREV_RQB_PTR(rb)        LSA_RQB_GET_PREV_RQB_PTR(rb)
#define MRP_RQB_GET_OPCODE(rb)              LSA_RQB_GET_OPCODE(rb)
#define MRP_RQB_GET_HANDLE(rb)              LSA_RQB_GET_HANDLE(rb)
#define MRP_RQB_GET_RESPONSE(rb)            LSA_RQB_GET_RESPONSE(rb)
#define MRP_RQB_GET_USER_ID_UVAR16(rb)      LSA_RQB_GET_USER_ID_UVAR16(rb)

#define MRP_RQB_GET_ARGS_PTR(rb)            ( &(rb)->args )


/*---------------------------------------------------------------------------*/
/*                       lower interface structures                          */
/*---------------------------------------------------------------------------*/
#define MRP_EDD_LOWER_MEM_PTR_TYPE                  /* pointer to lower-memory */\
    LSA_VOID *

#define MRP_EDD_RQB_OPEN_CHANNEL_TYPE                   EDD_RQB_OPEN_CHANNEL_TYPE
#define MRP_EDD_RQB_NRT_CANCEL_TYPE                     EDD_RQB_NRT_CANCEL_TYPE
#define MRP_EDD_RQB_GET_PARAMS_TYPE                     EDD_RQB_GET_PARAMS_TYPE
#define MRP_EDD_RQB_GET_PORT_PARAMS_TYPE                EDD_RQB_GET_PORT_PARAMS_TYPE
#define MRP_EDD_RQB_NRT_SEND_TYPE                       EDD_RQB_NRT_SEND_TYPE
#define MRP_EDD_RQB_NRT_RECV_TYPE                       EDD_RQB_NRT_RECV_TYPE
#define MRP_EDD_RQB_LINK_STATUS_IND_PROVIDE_TYPE        EDD_RQB_LINK_STATUS_IND_PROVIDE_TYPE
#define MRP_EDD_RQB_MULTICAST_TYPE                      EDD_RQB_MULTICAST_TYPE
#define MRP_EDD_RQB_MULTICAST_FWD_CTRL_TYPE             EDD_RQB_MULTICAST_FWD_CTRL_TYPE
#define MRP_EDD_RQB_SWI_FLUSH_FILTERING_DB_TYPE         EDD_RQB_SWI_FLUSH_FILTERING_DB_TYPE
#define MRP_EDD_RQB_SWI_SET_PORT_STATE_TYPE             EDD_RQB_SWI_SET_PORT_STATE_TYPE


#define MRP_EDD_LOWER_OPEN_CHANNEL_PTR_TYPE             EDD_UPPER_OPEN_CHANNEL_PTR_TYPE
#define MRP_EDD_UPPER_NRT_CANCEL_PTR_TYPE               EDD_UPPER_NRT_CANCEL_PTR_TYPE
#define MRP_EDD_UPPER_GET_PARAMS_PTR_TYPE               EDD_UPPER_GET_PARAMS_PTR_TYPE
#define MRP_EDD_UPPER_GET_PORT_PARAMS_PTR_TYPE          EDD_UPPER_GET_PORT_PARAMS_PTR_TYPE
#define MRP_EDD_UPPER_NRT_SEND_PTR_TYPE                 EDD_UPPER_NRT_SEND_PTR_TYPE
#define MRP_EDD_UPPER_NRT_RECV_PTR_TYPE                 EDD_UPPER_NRT_RECV_PTR_TYPE
#define MRP_EDD_UPPER_LINK_STATUS_IND_PROVIDE_PTR_TYPE  EDD_UPPER_LINK_STATUS_IND_PROVIDE_PTR_TYPE
#define MRP_EDD_UPPER_MULTICAST_PTR_TYPE                EDD_UPPER_MULTICAST_PTR_TYPE
#define MRP_EDD_UPPER_MULTICAST_FWD_CTRL_PTR_TYPE       EDD_UPPER_MULTICAST_FWD_CTRL_PTR_TYPE
#define MRP_EDD_UPPER_SWI_FLUSH_FILTERING_DB_PTR_TYPE   EDD_UPPER_SWI_FLUSH_FILTERING_DB_PTR_TYPE
#define MRP_EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE       EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE




/*------------------------------------------------------------------------------
// Put / Get
//----------------------------------------------------------------------------*/
#if 0
#define MRP_PUT16_HTON(base,offset,val)                                                     \
{/* put and convert from host byte order to network byte order */                           \
        LSAS_PUT16_HTON(base,offset,val);                                                       \
}                                                                                           \

#define MRP_GET16_NTOH(base,offset)                                                         \
(/* get and convert from network byte order to host byte order */                           \
        LSAS_GET16_NTOH(base,offset)                                                            \
)

#define MRP_GET32_NTOH(base, offset)                                                        \
(/* get and convert from network byte order to host byte order */                           \
        LSAS_GET32_NTOH(base,offset)                                                            \
)

#define MRP_PUT32_HTON(base, offset, val)                                                   \
{/* put and convert from host byte order to network byte order */                           \
        LSAS_PUT32_HTON(base,offset,val);                                                       \
}
#endif

/*----------------------------------------------------------------------------*/
#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif  /* of MRP_CFG_H */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
