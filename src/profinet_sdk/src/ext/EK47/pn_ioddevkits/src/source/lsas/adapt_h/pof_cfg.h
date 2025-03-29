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
/*  F i l e               &F: pof_cfg.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Configuration for POF:                                                   */
/*  Defines constants, types and macros for POF.                             */
/*                                                                           */
/*  This file has to be overwritten during system integration, because       */
/*  some definitions depend on the different system, compiler or             */
/*  operating system.                                                        */
/*                                                                           */
/*****************************************************************************/
#ifndef POF_CFG_H                     /* ----- reinclude-protection ----- */
#define POF_CFG_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif


/*===========================================================================*/
/*                              defines                                      */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* Maximum number of user-channels                                           */
/*---------------------------------------------------------------------------*/
#define POF_CFG_MAX_CHANNELS    2   /* max number of user-channels */

/*---------------------------------------------------------------------------*/
/* Simulate POF Diagnostic concept:                                          */
/* - enables Service POF_OPC_SET_DIAGNOSTIC                                  */
/* - enables Service POF_OPC_SET_RAW_HARDWARE                                */
/* - POF Transceiver are not accessed via I2C for diagnostic                 */
/*---------------------------------------------------------------------------*/
#undef  POF_CFG_SIMULATE_DIAGNOSTIC

/*---------------------------------------------------------------------------*/
/* Advanced protection and function handling settings.                       */
/*                                                                           */
/* POF is designed to use the input functions pof_timeout() without          */
/* POF_ENTER() and POF_EXIT() even if called from a priority above standard  */
/* handling 1). The requirement for this is, that pof_timeout() can not be   */
/* intercepted by standard handling.                                         */
/* If this requirement is not met, additional protection with POF_ENTER()    */
/* and POF_EXIT() is neccessary. If so the following defines may have to     */
/* be used to include protection within this function.                       */
/*                                                                           */
/* POF_CFG_USE_ENTER_EXIT_TIMEOUT                                            */
/*      defined: Uses the output macros POF_ENTER() and POF_EXIT() even      */
/*               with the Timeout-handler pof_timeout(). This must be        */
/*               used if it is neccessary to protect this function if it can */
/*               be interrupted by standard-handling 1)                      */
/*               Note that the output macro POF_DO_TIMER_REQUEST() is called */
/*               within POF_ENTER() protected code sequence.                 */
/*                                                                           */
/*      undef:   No POF_ENTER() and POF_EXIT() present within function       */
/*               pof_timeout(). Can be used if function is not               */
/*               intercepted by standard handling 1)                         */
/*                                                                           */
/* Defaults:                                                                 */
/*                                                                           */
/*  #undef POF_CFG_USE_ENTER_EXIT_TIMEOUT                                    */
/*                                                                           */
/* 1) Standard handling is the following:                                    */
/*    * pof_init                                                             */
/*    * pof_undo_init                                                        */
/*    * pof_edd_request_lower_done                                           */
/*    * pof_open_channel                                                     */
/*    * pof_close_channel                                                    */
/*    * pof_request                                                          */
/*    * part of pof_timeout() triggered by POF_DO_TIMER_REQUEST.             */
/*                                                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Enable manufacturer specific subblock "FiberOpticManufacturerSpecific"    */
/* in POF-Record "PDPortFODataReal"                                          */
/*                                                                           */
/* - All products, that are already supporting the POF functionality with    */
/*   PNIO Stack V4.2 or lower, must set this compilerswitch for              */
/*   compatibility reasons.                                                  */
/* - All new products don't set this compilerswitch!                         */
/*---------------------------------------------------------------------------*/
//#define POF_CFG_PRM_FO_MANUFACTURER_SPECIFIC_SUBBLOCK_ENABLE

/*===========================================================================*/
/*                              basic attributes                             */
/*===========================================================================*/

#define POF_ATTR_NEAR
#define POF_ATTR_FAR
#define POF_ATTR_SHUGE
#define POF_ATTR_HUGE

/*===========================================================================*/
/*                         code- and data-attributes                         */
/*===========================================================================*/

/*=====  code attributes  =====*/

#define POF_UPPER_IN_FCT_ATTR         POF_ATTR_HUGE  /* POF-user to POF  */
#define POF_UPPER_OUT_FCT_ATTR        POF_ATTR_HUGE  /* POF to POF -user */
#define POF_EDD_LOWER_IN_FCT_ATTR     POF_ATTR_HUGE  /* EDD  lower component to POF */
#define POF_EDD_LOWER_OUT_FCT_ATTR    POF_ATTR_HUGE  /* POF to EDD  lower component */
#define POF_SYSTEM_IN_FCT_ATTR        POF_ATTR_FAR   /* system to POF */
#define POF_SYSTEM_OUT_FCT_ATTR       POF_ATTR_FAR   /* POF to system */
#define POF_LOCAL_FCT_ATTR            POF_ATTR_NEAR  /* local function */

/*=====  data attributes  =====*/

#define POF_UPPER_RQB_ATTR            POF_ATTR_HUGE  /* upper-rqb data         */
#define POF_UPPER_MEM_ATTR            POF_ATTR_HUGE  /* upper-memory data      */
#define POF_EDD_LOWER_RQB_ATTR        POF_ATTR_HUGE  /* DCP  lower-rqb data    */
#define POF_EDD_LOWER_MEM_ATTR        POF_ATTR_HUGE  /* DCP  lower memory data */
#define POF_SYSTEM_MEM_ATTR           POF_ATTR_HUGE  /* system-memory data     */
#define POF_LOCAL_MEM_ATTR            POF_ATTR_HUGE  /* local data             */
#define POF_COMMON_MEM_ATTR           POF_ATTR_HUGE  /* greatest common data   */
                                                       /* attribute (matches all)*/

/*---------------------------------------------------------------------------*/
/* Definition of RQB-HEADER:                                                 */
/*                                                                           */
/* The order of the parameters can be changed!                               */
/* The length of the RQB-HEADER must be 2/4-byte-aligned according to the    */
/* system!                                                                   */
/* If necessary, dummy-bytes must be added!                                  */
/* If necessary during system integration, additional parameters can be added*/
/*                                                                           */
/* For example:                                                              */
/*   LSA_COMP_ID_TYPE                comp_id;    for lsa-component-id        */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#if defined (LSA_RQB_HEADER)

#define POF_RQB_HEADER      LSA_RQB_HEADER(POF_UPPER_RQB_PTR_TYPE)

#else

#error "How this? This is the PCPNIO-adaption"

#define POF_RQB_HEADER                                                        \
    POF_UPPER_RQB_PTR_TYPE       next_rqb_ptr;    /* mandatory */             \
                                    /* pointer to next RQB */                 \
    POF_UPPER_RQB_PTR_TYPE       prev_rqb_ptr;    /* mandatory */             \
                                    /* pointer to previous RQB */             \
    LSA_OPCODE_TYPE               opcode;          /* mandatory */            \
                                    /* req: opcode, see POF_usr.h */          \
                                    /* cnf: unchanged             */          \
    LSA_HANDLE_TYPE               handle;          /* mandatory */            \
                                    /* req: if POF_OPC_OPEN_CHANNEL: -   */   \
                                    /*      else: channel-handle of POF  */   \
                                    /* cnf:       channel-handle of user */   \
    LSA_UINT16                    response;        /* mandatory */            \
                                    /* req: -                       */        \
                                    /* cnf: response, see POF_usr.h */        \
    LSA_USER_ID_TYPE              user_id;         /* mandatory */            \
                                    /* req: id of user */                     \
                                    /* cnf: unchanged  */

#endif

/*---------------------------------------------------------------------------*/
/* Definition of RQB-TRAILER                                                 */
/*                                                                           */
/* If necessary during system integration, additional parameters can be      */
/* added!                                                                    */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#define POF_RQB_TRAILER     LSA_RQB_TRAILER


/*---------------------------------------------------------------------------*/
/* set and get parameter of the rqb-header                                   */
/*---------------------------------------------------------------------------*/

#define POF_RQB_SET_NEXT_RQB_PTR      LSA_RQB_SET_NEXT_RQB_PTR
#define POF_RQB_GET_NEXT_RQB_PTR      LSA_RQB_GET_NEXT_RQB_PTR
#define POF_RQB_SET_PREV_RQB_PTR      LSA_RQB_SET_PREV_RQB_PTR
#define POF_RQB_GET_PREV_RQB_PTR      LSA_RQB_GET_PREV_RQB_PTR
#define POF_RQB_SET_OPCODE            LSA_RQB_SET_OPCODE
#define POF_RQB_GET_OPCODE            LSA_RQB_GET_OPCODE
#define POF_RQB_SET_HANDLE            LSA_RQB_SET_HANDLE
#define POF_RQB_GET_HANDLE            LSA_RQB_GET_HANDLE
#define POF_RQB_SET_USERID_UVAR32     LSA_RQB_SET_USER_ID_UVAR32
#define POF_RQB_GET_USERID_UVAR32     LSA_RQB_GET_USER_ID_UVAR32
#define POF_RQB_GET_USERID            LSA_RQB_GET_USERID
#define POF_RQB_SET_USERID_UVAR16_1   LSA_RQB_SET_USER_ID_UVAR16_ARRAY_LOW
#define POF_RQB_GET_USERID_UVAR16_1   LSA_RQB_GET_USER_ID_UVAR16_ARRAY_LOW
#define POF_RQB_SET_USERID_UVAR16_2   LSA_RQB_SET_USER_ID_UVAR16_ARRAY_HIGH
#define POF_RQB_GET_USERID_UVAR16_2   LSA_RQB_GET_USER_ID_UVAR16_ARRAY_HIGH
#define POF_RQB_SET_RESPONSE          LSA_RQB_SET_RESPONSE
#define POF_RQB_GET_RESPONSE          LSA_RQB_GET_RESPONSE


/*===========================================================================*/
/* LOWER LAYER RQB MACROS to EDD                                             */
/*===========================================================================*/

#define POF_EDD_RQB_SET_NEXT_RQB_PTR(rb_, val_)   EDD_RQB_SET_NEXT_RQB_PTR(rb_, val_)
#define POF_EDD_RQB_SET_PREV_RQB_PTR(rb_, val_)   EDD_RQB_SET_PREV_RQB_PTR(rb_, val_)
#define POF_EDD_RQB_SET_OPCODE(rb_, val_)         EDD_RQB_SET_OPCODE(rb_, val_)
#define POF_EDD_RQB_SET_SERVICE(rb_, val_)        EDD_RQB_SET_SERVICE(rb_, val_)
#define POF_EDD_RQB_SET_HANDLE(rb_, val_)         EDD_RQB_SET_HANDLE(rb_, val_)
#define POF_EDD_RQB_SET_LOWER_HANDLE(rb_, val_)   EDD_RQB_SET_HANDLE_LOWER(rb_, val_)
#define POF_EDD_RQB_SET_USERID_UVAR32(rb_, val_)  EDD_RQB_SET_USERID_UVAR32(rb_, val_)
#define POF_EDD_RQB_SET_RESPONSE(rb_, val_)       EDD_RQB_SET_STATUS(rb_, val_)

#define POF_EDD_RQB_GET_NEXT_RQB_PTR(rb_)         EDD_RQB_GET_NEXT_RQB_PTR(rb_)
#define POF_EDD_RQB_GET_PREV_RQB_PTR(rb_)         EDD_RQB_GET_PREV_RQB_PTR(rb_)
#define POF_EDD_RQB_GET_OPCODE(rb_)               EDD_RQB_GET_OPCODE(rb_)
#define POF_EDD_RQB_GET_SERVICE(rb_)              EDD_RQB_GET_SERVICE(rb_)
#define POF_EDD_RQB_GET_HANDLE(rb_)               EDD_RQB_GET_HANDLE(rb_)
#define POF_EDD_RQB_GET_LOWER_HANDLE(rb_)         EDD_RQB_GET_HANDLE_LOWER(rb_)
#define POF_EDD_RQB_GET_USERID_UVAR32(rb_)        EDD_RQB_GET_USERID_UVAR32(rb_)
#define POF_EDD_RQB_GET_RESPONSE(rb_)             EDD_RQB_GET_RESPONSE(rb_)

/*---------------------------------------------------------------------------*
 * Opcodes, service codes ...  of lower RQB (ethernet device driver)
*/
#define POF_LOWER_RQB_PTR_TYPE                    EDD_UPPER_RQB_PTR_TYPE


/*===========================================================================*/
/* LSA-TRACE                                                                 */
/*===========================================================================*/
//#define POF_CFG_TRACE_MODE                0       //no Trace
#define POF_CFG_TRACE_MODE                1       //LSA-Trace
//#define POF_CFG_TRACE_MODE                  2       //LSA-IDX-Trace
/*
#define TRACE_SUBSYS_POF_UPPER      pof     //upper calls/responses
#define TRACE_SUBSYS_POF_LOWER      pof     //lower calls/responses
#define TRACE_SUBSYS_POF_SYSTEM     pof     //system function calls/responses
#define TRACE_SUBSYS_POF_FUNCTION   pof     //function in/out
#define TRACE_SUBSYS_POF_PROGRAM    pof     //program actions and errors
*/
/*
//LSA-IDX-Trace
#define TRACE_SUBSYS_POF_UPPER_IDX00        pof     //upper calls/responses
#define TRACE_SUBSYS_POF_LOWER_IDX00        pof     //lower calls/responses
#define TRACE_SUBSYS_POF_SYSTEM_IDX00       pof     //system function calls/responses
#define TRACE_SUBSYS_POF_FUNCTION_IDX00     pof     //function in/out
#define TRACE_SUBSYS_POF_PROGRAM_IDX00      pof     //program actions and errors
*/
#define pof_log_get_fct             "pof"   //see LTRC_TRACE_01

extern int pof_log_level;                   //see LTRC_LEVEL_TYPE

/*===========================================================================*/
/* Debugging (for developer use)                                             */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* define to enable special additional assertion-checks (e.g. NULL-Ptr)      */
/* This assertion will cause a fatal-error.                                  */
/*---------------------------------------------------------------------------*/

#undef POF_CFG_DEBUG_ASSERT

/*---------------------------------------------------------------------------*/
/* For additional internal debugging                                         */
/*---------------------------------------------------------------------------*/

#undef POF_CFG_DEBUG_ENABLE

#if defined (POF_CFG_DEBUG_ENABLE)
#define POF_CFG_DEBUG_RQB            0  /* logs user-RQB calls/confirmation */
#define POF_CFG_DEBUG_DUMP_FRAME     0  /* dump frames send/received        */
#endif

/*---------------------------------------------------------------------------*/

/*===========================================================================*/
/* macro name:    POF_FILE_SYSTEM_EXTENSION(module_id_)                      */
/*                                                                           */
/* macro:         System integrators can use the macro to smuggle something  */
/*                into the component source files. By default it's empty.    */
/*                                                                           */
/*                The system integrators are responsible for the contents of */
/*                that macro.                                                */
/*                                                                           */
/*                Internal data of LSA components may not be changed!        */
/*                                                                           */
/* parameter:     Module-id                                                  */
/*                                                                           */
/*                There's no prototype for that macro!                       */
/*===========================================================================*/

#define POF_FILE_SYSTEM_EXTENSION(module_id_)

/* Do not use this switch until you have problems with not enough memory -> typically this switch can save you */
/* some memory ( ca. 2 kByte ), depending on the full path length of the POF Files                             */
#define  POF_CFG_NO_FATAL_FILE_INFO

/*===========================================================================*/
/*                                 macros                                    */
/*===========================================================================*/

/* output function makros
 */

/* system access macros */

#define POF_RQB_ERROR( _upper_rqb_ptr )                                        \
{                                                                              \
    POF_FatalError((LSA_UINT32)__LINE__, (LSA_UINT8 *)(void *)0, (LSA_UINT32)POF_MODULE_ID, \
                    (LSA_UINT8  *)(void *)0, (LSA_UINT32)0, (LSA_UINT32)0, (LSA_UINT32)0);\
}

#define POF_FATAL_ERROR( _length, _error_ptr )                  \
{                                                               \
    LSAS_FATAL_ERROR( _error_ptr);     \
}

/*****************************************************************************/
/*  end of file pof_cfg.h/txt                                                */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of POF_CFG_H */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
