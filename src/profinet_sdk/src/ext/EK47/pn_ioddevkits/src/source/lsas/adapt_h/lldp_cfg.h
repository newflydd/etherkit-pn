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
/*  F i l e               &F: lldp_cfg.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Configuration for LLDP:                                                  */
/*  Defines constants, types and macros for LLDP.                            */
/*                                                                           */
/*  This file has to be overwritten during system integration, because       */
/*  some definitions depend on the different system, compiler or             */
/*  operating system.                                                        */
/*                                                                           */
/*****************************************************************************/

#ifndef LLDP_CFG_H                       /* ----- reinclude-protection ----- */
#define LLDP_CFG_H

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
#ifdef LLDP_MESSAGE
/*  05.04.04    JS    initial version.                                       */
/*  13.10.04    JS    added support of external LSA- RQB HEADER - defines.   */
/*  01.06.05    JS    EDD_RQB_SET/GET_STATUS replaced by SET/GET_RESPONSE.   */
/*  27.09.05    JS    changed default of LLDP_CFG_TX_DELAY to 0s.            */
/*                    changed default of LLDP_CFG_USE_STOP_DELAY to #undef   */
/*                    changed default of LLDP_CFG_TX_REINIT_DELAY to 0s      */
/*  27.06.06    JS    added  LLDP_FILE_SYSTEM_EXTENSION                      */
/*  12.10.06    JS    using LSA RQB Header definitions                       */
/*  09.01.07    JS    added LLDP_CFG_NRT_SEND_PRIO                           */
/*  19.02.09    JS    changed default of LLDP_CFG_NRT_SEND_PRIO              */
#endif
/*****************************************************************************/

/*===========================================================================*/
/*                              defines                                      */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* Set some local_mem static or dynamic:                                     */
/* If LLDP_CFG_LOCAL_MEM_STATIC is not defined, all local-memory will be     */
/* allocated dynamically. If defined, some local memory is static.           */
/*---------------------------------------------------------------------------*/

#define LLDP_CFG_LOCAL_MEM_STATIC

/*---------------------------------------------------------------------------*/
/* If defined, all frame-buffers needed will be allocated on opening the     */
/* EDD systemchannel. This frame-buffers will stay till closing the channel. */
/* If not defined the frame-buffers will be allocated on need and freed      */
/* if not needed anymore (e.g. on every send)                                */
/*---------------------------------------------------------------------------*/

#define LLDP_CFG_EDD_ALLOC_TX_MEM_ON_OPEN

/*---------------------------------------------------------------------------*/
/* Number of Receive Resources used per Port. This defines the number of     */
/* EDD- Receive-Requests used per EDD-Channel. The number of Requests is     */
/* LLDP_CFG_RECV_RESOURCE_PER_PORT * PortCnt.                                */
/*---------------------------------------------------------------------------*/

#define LLDP_CFG_RECV_RESOURCE_PER_PORT   4

/*---------------------------------------------------------------------------*/
/* Defines if we should send a Shutdown-frame if stopping a started port.    */
/* If defined we send a shutdown, if not we dont.                            */
/*---------------------------------------------------------------------------*/

#define LLDP_CFG_SEND_SHUTDOWN_ON_STOP          /* set according to PSI. IEEE P802.1 AB/D11 defines that a shutdown
                                                   should be sent if the station goes offline */

/*---------------------------------------------------------------------------*/
/* Advanced protection and function handling settings.                       */
/*                                                                           */
/* LLDP is designed to use the input functions lldp_timeout() without        */
/* LLDP_ENTER() and LLDP_EXIT() even if called from a priority above standard*/
/* handling 1). The requirement for this is, that lldp_timeout() can not be  */
/* intercepted by standard handling.                                         */
/* If this requirement is not met, additional protection with LLDP_ENTER()   */
/* and LLDP_EXIT() is neccessary. If so the following defines may have to    */
/* be used to include protection within this function.                       */
/* LLDP_CFG_HANDLE_DO_TIMEOUT_INTERN is a little special, because it         */
/* simplyfies some handing if no context-change is neccessary.               */
/*                                                                           */
/* LLDP_CFG_USE_ENTER_EXIT_TIMEOUT                                           */
/*      defined: Uses the output macros LLDP_ENTER() and LLDP_EXIT() even    */
/*               with the timeout-handler lldp_timeout(). This must be       */
/*               used if it is neccessary to protect this function if it can */
/*               be interrupted by standard-handling 1)                      */
/*               Note that the output macro LLDP_DO_TIMER_REQUEST() is called*/
/*               within LLDP_ENTER() protected code sequence.                */
/*                                                                           */
/*      undef:   No LLDP_ENTER() and LLDP_EXIT() present within function     */
/*               lldp_timeout(). Can be used if function is not              */
/*               intercepted by standard handling 1)                         */
/*                                                                           */
/* LLDP_CFG_HANDLE_DO_TIMEOUT_INTERN:                                        */
/*      defined: LLDP does not call the output macro LLDP_DO_TIMER_REQUEST   */
/*               within lldp_timeout() but does the neccessary standard      */
/*               handling within lldp_timeout(). This can be set if no       */
/*               context change is neccessary or the function is protected   */
/*               with LLDP_ENTER() and LLDP_EXIT().                          */
/*      undef:   LLDP uses LLDP_DO_TIMER_REQUEST within lldp_timeout() to    */
/*               trigger a context change. Within lldp_timeout() very little */
/*               is done, so no protection with LLDP_ENTER() and LLDP_EXIT() */
/*               is neccessary if lldp_timeout() is called within a priority */
/*               above standard-input functions.                             */
/*                                                                           */
/* Defaults:                                                                 */
/*                                                                           */
/*  #undef LLDP_CFG_USE_ENTER_EXIT_TIMEOUT                                   */
/*  #undef LLDP_CFG_HANDLE_DO_TIMEOUT_INTERN                                 */
/*                                                                           */
/*                                                                           */
/* 1) Standard handling is the following:                                    */
/*    * lldp_init                                                            */
/*    * lldp_undo_init                                                       */
/*    * lldp_edd_request_lower_done                                          */
/*    * lldp_open_channel                                                    */
/*    * lldp_close_channel                                                   */
/*    * lldp_request                                                         */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#undef LLDP_CFG_USE_ENTER_EXIT_TIMEOUT
#undef LLDP_CFG_HANDLE_DO_TIMEOUT_INTERN

/*===========================================================================*/
/*                              basic attributes                             */
/*===========================================================================*/

/*===========================================================================*/
/*                         code- and data-attributes                         */
/*===========================================================================*/


/*---------------------------------------------------------------------------*/
/* Definition of RQB-HEADER:                                                 */
/*---------------------------------------------------------------------------*/

#define LLDP_RQB_HEADER LSA_RQB_HEADER(LLDP_UPPER_RQB_PTR_TYPE)

/*---------------------------------------------------------------------------*/
/* Definition of RQB-TRAILER                                                 */
/*                                                                           */
/* If necessary during system integration, additional parameters can be      */
/* added!                                                                    */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#define LLDP_RQB_TRAILER    LSA_RQB_TRAILER

/*---------------------------------------------------------------------------*/
/* set and get parameter of the rqb-header                                   */
/*---------------------------------------------------------------------------*/

#define LLDP_RQB_SET_NEXT_RQB_PTR      LSA_RQB_SET_NEXT_RQB_PTR
#define LLDP_RQB_GET_NEXT_RQB_PTR      LSA_RQB_GET_NEXT_RQB_PTR
#define LLDP_RQB_SET_PREV_RQB_PTR      LSA_RQB_SET_PREV_RQB_PTR
#define LLDP_RQB_GET_PREV_RQB_PTR      LSA_RQB_GET_PREV_RQB_PTR
#define LLDP_RQB_SET_OPCODE            LSA_RQB_SET_OPCODE
#define LLDP_RQB_GET_OPCODE            LSA_RQB_GET_OPCODE
#define LLDP_RQB_SET_HANDLE            LSA_RQB_SET_HANDLE
#define LLDP_RQB_GET_HANDLE            LSA_RQB_GET_HANDLE
#define LLDP_RQB_SET_USERID_UVAR32     LSA_RQB_SET_USER_ID_UVAR32
#define LLDP_RQB_GET_USERID_UVAR32     LSA_RQB_GET_USER_ID_UVAR32
#define LLDP_RQB_SET_USERID_UVAR16_1   LSA_RQB_SET_USER_ID_UVAR16_ARRAY_LOW
#define LLDP_RQB_GET_USERID_UVAR16_1   LSA_RQB_GET_USER_ID_UVAR16_ARRAY_LOW
#define LLDP_RQB_SET_USERID_UVAR16_2   LSA_RQB_SET_USER_ID_UVAR16_ARRAY_HIGH
#define LLDP_RQB_GET_USERID_UVAR16_2   LSA_RQB_GET_USER_ID_UVAR16_ARRAY_HIGH
#define LLDP_RQB_SET_RESPONSE          LSA_RQB_SET_RESPONSE
#define LLDP_RQB_GET_RESPONSE          LSA_RQB_GET_RESPONSE

/*===========================================================================*/
/* LOWER LAYER RQB MACROS to EDD                                             */
/*===========================================================================*/

#define LLDP_EDD_RQB_SET_NEXT_RQB_PTR(rb_, val_)   EDD_RQB_SET_NEXT_RQB_PTR(rb_, val_)
#define LLDP_EDD_RQB_SET_PREV_RQB_PTR(rb_, val_)   EDD_RQB_SET_PREV_RQB_PTR(rb_, val_)
#define LLDP_EDD_RQB_SET_OPCODE(rb_, val_)         EDD_RQB_SET_OPCODE(rb_, val_)
#define LLDP_EDD_RQB_SET_SERVICE(rb_, val_)        EDD_RQB_SET_SERVICE(rb_, val_)
#define LLDP_EDD_RQB_SET_HANDLE(rb_, val_)         EDD_RQB_SET_HANDLE(rb_, val_)
#define LLDP_EDD_RQB_SET_USERID_UVAR32(rb_, val_)  EDD_RQB_SET_USERID_UVAR32(rb_, val_)
#define LLDP_EDD_RQB_SET_RESPONSE(rb_, val_)       EDD_RQB_SET_RESPONSE(rb_, val_)

#define LLDP_EDD_RQB_GET_NEXT_RQB_PTR(rb_)         EDD_RQB_GET_NEXT_RQB_PTR(rb_)
#define LLDP_EDD_RQB_GET_PREV_RQB_PTR(rb_)         EDD_RQB_GET_PREV_RQB_PTR(rb_)
#define LLDP_EDD_RQB_GET_OPCODE(rb_)               EDD_RQB_GET_OPCODE(rb_)
#define LLDP_EDD_RQB_GET_SERVICE(rb_)              EDD_RQB_GET_SERVICE(rb_)
#define LLDP_EDD_RQB_GET_HANDLE(rb_)               EDD_RQB_GET_HANDLE(rb_)
#define LLDP_EDD_RQB_GET_USERID_UVAR32(rb_)        EDD_RQB_GET_USERID_UVAR32(rb_)
#define LLDP_EDD_RQB_GET_RESPONSE(rb_)             EDD_RQB_GET_RESPONSE(rb_)

/*===========================================================================*/
/* macro name:    LLDP_FILE_SYSTEM_EXTENSION(module_id_)                     */
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

#define LLDP_FILE_SYSTEM_EXTENSION(module_id_)

/*===========================================================================*/
/* LTRC - Support (LSA-Trace)                                                */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* define the Trace mode for LLDP                                            */
/* 0: no traces or external traces (default)                                 */
/* 1: enable Traces and use LTRC (LSA-Trace module) see LLDP_ltrc.h          */
/* 2: enables full index trace  (LSA-index Trace module)                     */
/*---------------------------------------------------------------------------*/

#define LLDP_CFG_TRACE_MODE   1

/*=============================================================================
 * If the LSA component LTRC isn't used for trace in the target system, then
 * the LLDP trace macros can be defined here. On default they are empty.
 *
 * See also files LLDP_trc.h and lsa_cfg.h/txt.
 *
 *===========================================================================*/

/* LTRC-IF */
/*
#define TRACE_SUBSYS_LLDP_UPPER    lldp
#define TRACE_SUBSYS_LLDP_LOWER    lldp
#define TRACE_SUBSYS_LLDP_SYSTEM   lldp
#define TRACE_SUBSYS_LLDP_FUNCTION lldp
#define TRACE_SUBSYS_LLDP_PROGRAM  lldp

#define TRACE_SUBSYS_LLDP_UPPER_IDX00    lldp
#define TRACE_SUBSYS_LLDP_LOWER_IDX00    lldp
#define TRACE_SUBSYS_LLDP_SYSTEM_IDX00   lldp
#define TRACE_SUBSYS_LLDP_FUNCTION_IDX00 lldp
#define TRACE_SUBSYS_LLDP_PROGRAM_IDX00  lldp
*/
//#define lldp_log_get_fct           "lldp" /* see LTRC_TRACE_01 */

extern int lldp_log_level;         /* see LTRC_LEVEL_TYPE */

/*===========================================================================*/
/* Debugging (for developer use)                                             */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* define to enable spezial additonal assertion-checks (e.g. NULL-Ptr)       */
/* This assertion will cause an fatal-error.                                 */
/*---------------------------------------------------------------------------*/

#undef LLDP_CFG_DEBUG_ASSERT

/*---------------------------------------------------------------------------*/
/* If LLDP_CFG_TRACE_MODE = 2 we use internal tracefunctions. For this we can*/
/* set the compile-level for traces. Traces above this level will not be     */
/* included into the program.                                                */
/* 0 : off (no traces)                                                       */
/* 1 : fatal errors                                                          */
/* 2 : errors                                                                */
/* 3 : unexpected situations                                                 */
/* 4 : note high                                                             */
/* 5 : note (upper/lower/system)                                             */
/* 6 : note low (program flow)                                               */
/* 7 : chat (function in/out)                                                */
/*---------------------------------------------------------------------------*/

#define LLDP_CFG_DEBUG_TRACE_COMPILE_LEVEL   4

/*---------------------------------------------------------------------------*/
/* For additional internal debugging                                         */
/*---------------------------------------------------------------------------*/

#undef LLDP_CFG_DEBUG_ENABLE

#ifdef  LLDP_CFG_DEBUG_ENABLE
#define LLDP_CFG_DEBUG_DUMP_FRAME     0  /* dump frames send/received        */
#endif

/*---------------------------------------------------------------------------*/

/*===========================================================================*/
/*                                 macros                                    */
/*===========================================================================*/


#define LLDP_FATAL_ERROR(_length, _error_ptr)                \
{                                                            \
    LSAS_FATAL_ERROR( _error_ptr); \
}

/*****************************************************************************/
/*  end of file LLDP_CFG.H                                                    */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of LLDP_CFG_H */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
