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
/*  F i l e               &F: eddp_cfg.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Configuration for EDDP:                                                  */
/*  Defines constants, types and macros for edd.                             */
/*                                                                           */
/*  This file has to be overwritten during system integration, because       */
/*  some definitions depend on the different system, compiler or             */
/*  operating system.                                                        */ 
/*                                                                           */
/*****************************************************************************/

#ifndef EDDP_CFG_H                       /* ----- reinclude-protection ----- */
#define EDDP_CFG_H

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
#ifdef EDDP_MESSAGE
/*  13.08.08    JS    initial version.                                           */
/*  10.03.09    LRG   SYNC_TRACE -> K32_TRACE                                    */
/*  21.08.09    JS    added EDDP_CFG_IFB_SUPPORT                                 */
/*  22.10.09    SFR   tactical switch to LTRC only                               */
/*  19.01.10    lrg   EDDP_CFG_DEBUG_KRISC32_TRACE_COMPILE_LEVEL                 */
/*  20.01.10    JS    changed define EDDP_CFG_USE_PROTECTION_WITHIN_INTERRUPT    */
/*                    to EDDP_CFG_USE_ENTER_EXIT_INT_WITHIN_ISR                  */
/*  26.03.10    MA    renamed EDDP_CFG_DEBUG_KRISC32_TRACE_COMPILE_LEVEL         */
/*                    to EDDP_CFG_DEBUG_KRISC32_TRACE_LEVEL                      */
/*  22.07.10    JS    AP00999106: moved EDDP_xxxxx_TRACE_xx to eddp_trc.h        */
/*                                removed EDDP compile levels                    */
/*  23.07.10    JS    removed EDDP_CFG_TEST_MACRO_SUPPORT                        */
/*  19.10.10    MA    removed EDDP_CFG_DEBUG_KRISC32_TRACE_LEVEL                 */
/*  27.04.11    SFR   PHY CONFIG                                                 */
/*  02.08.11    VS    changed comment of EDDP_CFG_DETAIL_ERROR_INFO              */
/*  27.04.12    VS    added define EDDP_CFG_MAX_USER_CMD_HANDLES                 */
/*                    added define EDDP_CFG_USR_CMD_API_SUPPORT                  */
/*  03.07.12    VS    added define EDDP_CFG_USR_CMD_API_INCLUDE                  */
/*  18.02.13    MH    added define EDDP_CFG_SYSRED_API_SUPPORT                   */
/*                    added define EDDP_CFG_SYSRED_API_INCLUDE                   */
/*  20.02.15    AA    removed EDDP_CFG_IFB_SUPPORT                               */
/*  27.11.15    AD    added define EDDP_CFG_HW_ERTEC200P_SUPPORT and             */
/*                    undefine EDDP_CFG_HW_HERA_SUPPORT                          */
/*  20.07.18    AD    removed define EDDP_CFG_SYSRED_API_INCLUDE                 */

#endif
/*****************************************************************************/

/*===========================================================================*/
/*                              defines                                      */
/*===========================================================================*/

#define LSA_DROP_UNUSED_ARG LSA_UNUSED_ARG


#define EDDP_CFG_HW_ERTEC200P_SUPPORT

/*---------------------------------------------------------------------------*/
/* maximum number of devices supported                                       */
/*---------------------------------------------------------------------------*/

#define EDDP_CFG_MAX_DEVICES		1

/*---------------------------------------------------------------------------*/
/* maximum number of communication channels supported                        */
/* Note: Maximum is limited by LSA_HANDLE_TYPE range!                        */
/* Note: This is the number over all devices (sum!)                          */
/*---------------------------------------------------------------------------*/

#define EDDP_CFG_MAX_CHANNELS		20

/*---------------------------------------------------------------------------*/
/* maximum number of user command handles                                    */
/*---------------------------------------------------------------------------*/

#define EDDP_CFG_MAX_USER_CMD_HANDLES       (EDDP_CFG_MAX_DEVICES)

/*---------------------------------------------------------------------------*/
/* System Redundancy Support API support enabled                             */
/*---------------------------------------------------------------------------*/

#define EDDP_CFG_SYSRED_API_SUPPORT

/*---------------------------------------------------------------------------*/
/* maximum number of user Sysrem Redundancy handles                          */
/*---------------------------------------------------------------------------*/

#define EDDP_CFG_MAX_USER_SYSRED_HANDLES       (EDD_INTERFACE_ID_MAX)



/*---------------------------------------------------------------------------*/
/* Set some local_mem static or dynamic:                                     */
/* If EDDP_CFG_LOCAL_MEM_STATIC is not defined, all local-memory will be     */
/* allocated dynamically. If defined, some local memory is static.           */
/*---------------------------------------------------------------------------*/

#define EDDP_CFG_LOCAL_MEM_STATIC 

/*---------------------------------------------------------------------------*/
/* Host-Byte-Order format:                                                   */
/* big-endian   : define EDD_CFG_BIG_ENDIAN                                  */
/* little-endian: undef EDD_CFG_BIG_ENDIAN  (e.g. intel,ibc32)               */
/*---------------------------------------------------------------------------*/

#ifdef LSA_HOST_ENDIANESS_BIG
#define EDDP_CFG_BIG_ENDIAN
#endif        

/*---------------------------------------------------------------------------*/
/* Define which PN-IP Revision shall be supported by EDDP.                   */
/*                                                                           */
/* The PN-IP Revisions have fixed assignments to ASIC Steps:                 */
/*                                                                           */
/* EDDP_CFG_HW_PNIP_REV1_SUPPORT : ERTEC200P Step1 (PN-IP I4)                */
/* EDDP_CFG_HW_PNIP_REV2_SUPPORT : ERTEC200P Step2 (PN-IP I5)                */
/* EDDP_CFG_HW_PNIP_REV3_SUPPORT : HERA            (PN-IP I6)                */
/*---------------------------------------------------------------------------*/
/* Define which PN-IP Revision shall be supported with ERTEC200P             */
/* At least one has to be defined! Undefine to exclude support               */
/*---------------------------------------------------------------------------*/

#ifdef EDDP_CFG_HW_ERTEC200P_SUPPORT
#if(PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200P)
#define EDDP_CFG_HW_PNIP_REV1_SUPPORT            // support ERTEC200P Step1
#endif
#define EDDP_CFG_HW_PNIP_REV2_SUPPORT            // support ERTEC200P Step2
#endif

/*---------------------------------------------------------------------------*/
/* EDDP shall support the user command interface                             */
/*---------------------------------------------------------------------------*/

#define EDDP_CFG_USR_CMD_API_SUPPORT

/*---------------------------------------------------------------------------*/
/* Functionality "user command interface" will be included when compiling    */
/*---------------------------------------------------------------------------*/

#define EDDP_CFG_USR_CMD_API_INCLUDE

#ifdef EDDP_CFG_USR_CMD_API_INCLUDE
// timeout check of synchronous command: one Tick = 1us
#define EDDP_CFG_USR_CMD_TIMEOUT_TICKS_SYNCHRONOUS_1US      100000  // 100000 x 1us = 100ms
#endif

/*---------------------------------------------------------------------------*/
/* Number of entries in the User Command SharedMem CList                     */
/*---------------------------------------------------------------------------*/
#define EDDP_CFG_USR_CMD_SHAREDMEM_MAXENTRIES               16

/*---------------------------------------------------------------------------*/
/* If a a memory-free call error should result in an fatal-error call        */
/* with EDDP_FATAL_ERR_MEM_FREE define EDDP_CFG_FREE_ERROR_IS_FATAL. If not  */
/* memory-free errors will be ignored.                                       */
/*---------------------------------------------------------------------------*/

#define EDDP_CFG_FREE_ERROR_IS_FATAL

/*---------------------------------------------------------------------------*/
/* macro name:    EDDP_FILE_SYSTEM_EXTENSION(module_id_)                     */
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
/*---------------------------------------------------------------------------*/

#define EDDP_FILE_SYSTEM_EXTENSION(module_id_)

/*---------------------------------------------------------------------------*/
/* Type of systemhandles used by system for device and interface.            */
/*---------------------------------------------------------------------------*/

#define EDDP_SYS_HANDLE   LSA_UINT32

/*===========================================================================*/
/*                         code- and data-attributes                         */
/*===========================================================================*/

/*=====  code attributes  =====*/
#define EDDP_SYSTEM_IN_FCT_ATTR   EDD_ATTR_FAR   /* system to prefix */
#define EDDP_SYSTEM_OUT_FCT_ATTR  EDD_ATTR_FAR   /* eddp to system   */
#define EDDP_LOWER_OUT_FCT_ATTR   EDD_ATTR_FAR   /* eddp lower out   */
#define EDDP_LOCAL_FCT_ATTR       EDD_ATTR_NEAR  /* local function   */


/*=====  data attributes  =====*/
#define EDDP_LOWER_MEM_ATTR       EDD_ATTR_HUGE  /* lower-memory data */
#define EDDP_SYSTEM_MEM_ATTR      EDD_ATTR_HUGE  /* system-memory data */
#define EDDP_LOCAL_MEM_ATTR       EDD_ATTR_SHUGE /* local data */

/*===========================================================================*/
/* LTRC - Support (LSA-Trace)                                                */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* define the Trace mode for EDDP                                            */
/* 0: no traces or external traces (default)                                 */
/* 1: enable traces and use LTRC (LSA-Trace module) see eddp_ltrc.h          */
/* 2: enable indexed traces and use LTRC (LSA-Trace module) see eddp_ltrc.h  */
/*---------------------------------------------------------------------------*/

#define EDDP_CFG_TRACE_MODE  0     // trace off


/*---------------------------------------------------------------------------*/
/* Protection:                                                               */
/*                                                                           */
/* EDDP_CFG_USE_ENTER_EXIT_INT_WITHIN_ISR  :                                 */
/*                                                                           */
/*   defined: Call EDDP_ENTER_INT and EDDP_EXIT_INT protection macros within */
/*            eddp_interrupt().                                              */
/*                                                                           */
/*   undef  : Dont use EDDP_ENTER_INT and EDDP_EXIT_INT protection macros    */
/*            within eddp_interrupt()                                        */
/*                                                                           */
/*   Shall be defined if eddp_interrupt() can be interrupted by EDDP low     */
/*   prio context or common input function. Typically the interrupthandler   */
/*   can not be interrupted by lower prior task in a realtime operating      */
/*   system. Within windows operating system it is possible!                 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#define EDDP_CFG_USE_ENTER_EXIT_INT_WITHIN_ISR


/*===========================================================================*/
/* Debugging                                                                 */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* Fill DetailError(Response, ModuleId, LineNr) for DebugInfo in services:	 */
/*	- EDD_SRV_SRT_PROVIDER_CONTROL											 */
/*	- EDD_SRV_SRT_CONSUMER_CONTROL											 */
/*---------------------------------------------------------------------------*/

#define EDDP_CFG_DETAIL_ERROR_INFO

/*---------------------------------------------------------------------------*/
/* Reentrance/prio check for EDD function context.                           */
/* Adds a check for reentrance/prio mismatch within EDD.                     */
/* Can be activated to check for mismatch with context prios of EDD requests */
/* Should only be used for testing because it slows down the EDD.            */
/* If a mismatch is detected the EDD issues a fatal error with errorcode     */
/* EDDP_FATAL_ERR_REENTRANCE                                                 */
/*---------------------------------------------------------------------------*/

#define  EDDP_CFG_REENTRANCE_CHECK

/*---------------------------------------------------------------------------*/
/* Assertions                                                                */
/* Adds some additional error checks (assertions). causing a fatal error     */
/* on occurence.                                                             */
/*---------------------------------------------------------------------------*/

#define EDDP_CFG_DEBUG_ASSERT


/*---------------------------------------------------------------------------*/
/* If defined, extra trace info will be logged in interrupt routines.        */
/* For internal use only.                                                    */
/*---------------------------------------------------------------------------*/

#undef EDDP_CFG_USE_ISR_EXTRA_TRACE


/*===========================================================================*/
/*                            Default PHY adaption                           */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* If defined, the internal PHY adaptaion (for NEC and/or NSC) is included   */
/*---------------------------------------------------------------------------*/
#define EDDP_CFG_USE_DEFAULT_PHY_ADAPTION
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200P)
/*===========================================================================*/
/*                          PHY NEC INTERNAL CONFIG                          */
/*          (only if EDDP_CFG_USE_DEFAULT_PHY_ADAPTION is defined)           */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* If defined, the internal NEC PHY functions will be included with EDDP     */
/*---------------------------------------------------------------------------*/
#define EDDP_CFG_PHY_NEC_SUPPORT

/*---------------------------------------------------------------------------*/
/* If defined, the MEDIA Type of a port is set by the NEC PHY functions      */
/* to EDD_MEDIATYPE_COPPER_CABLE.                                            */
/* If not defined, systemadaption has to implement the output macro          */
/* EDDP_PHY_NEC_GET_MEDIA_TYPE().                                            */
/*---------------------------------------------------------------------------*/
//#define EDDP_CFG_PHY_NEC_MEDIA_TYPE_INTERNAL

/*---------------------------------------------------------------------------*/
/* If defined, the MAU Type of a port is set by the NEC PHY functions        */
/* to fixed values (see docu).                                               */
/* If not defined, systemadaption has to implement the output macro          */
/* EDDP_PHY_NEC_GET_MAU_TYPE() and                                           */
/* EDDP_PHY_NEC_CHECK_MAU_TYPE()                                             */
/*---------------------------------------------------------------------------*/
//#define EDDP_CFG_PHY_NEC_MAU_TYPE_INTERNAL

/*---------------------------------------------------------------------------*/
/* If defined, no LED blinking with NEC PHY functions is supported           */
/* If not defined, systemadaption has to implement the PHY functions         */
/* EDDP_PHY_NEC_LED_BlinkBegin()                                             */
/* EDDP_PHY_NEC_LED_BlinkSetMode()                                           */
/* EDDP_PHY_NEC_LED_BlinkEnd()                                               */
/*---------------------------------------------------------------------------*/
#define EDDP_CFG_PHY_NEC_NO_LED_BLINK


/*===========================================================================*/
/*                          PHY NSC INTERNAL CONFIG                          */
/*          (only if EDDP_CFG_USE_DEFAULT_PHY_ADAPTION is defined)           */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* If defined, the internal NSC PHY functions will be included with EDDP     */
/*---------------------------------------------------------------------------*/
#define EDDP_CFG_PHY_NSC_SUPPORT

/*---------------------------------------------------------------------------*/
/* If defined, the MEDIA Type of a port is set by the NSC PHY functions      */
/* to EDD_MEDIATYPE_COPPER_CABLE.                                            */
/* If not defined, systemadaption has to implement the output macro          */
/* EDDP_PHY_NSC_GET_MEDIA_TYPE().                                            */
/*---------------------------------------------------------------------------*/
//#define EDDP_CFG_PHY_NSC_MEDIA_TYPE_INTERNAL

/*---------------------------------------------------------------------------*/
/* If defined, the MAU Type of a port is set by the NSC PHY functions        */
/* to fixed values (see docu).                                               */
/* If not defined, systemadaption has to implement the output macro          */
/* EDDP_PHY_NSC_GET_MAU_TYPE() and                                           */
/* EDDP_PHY_NSC_CHECK_MAU_TYPE()                                             */
/*---------------------------------------------------------------------------*/
//#define EDDP_CFG_PHY_NSC_MAU_TYPE_INTERNAL

/*---------------------------------------------------------------------------*/
/* If defined, no LED blinking with NSC PHY functions is supported           */
/* If not defined, systemadaption has to implement the PHY functions         */
/* EDDP_PHY_NSC_LED_BlinkBegin()                                             */
/* EDDP_PHY_NSC_LED_BlinkSetMode()                                           */
/* EDDP_PHY_NSC_LED_BlinkEnd()                                               */
/*---------------------------------------------------------------------------*/
#define EDDP_CFG_PHY_NSC_NO_LED_BLINK
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2_EDDP)
#define EDDP_CFG_PHY_RZT2_SUPPORT
#endif

/*===========================================================================*/
/*                                 macros                                    */
/*===========================================================================*/

#define EDDP_RQB_ERROR(_pRQB)\
{\
    EDDP_GlbFatalError((LSA_UINT32)_pRQB,                           \
                    EDDP_MODULE_ID,                     \
                    __LINE__);                          \
}
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200P)
#define EDDP_K32_RESET( _H_SYS_DEV, _ON)\
{\
    if( LSA_FALSE == _ON )\
    {\
        /* release asynchronous reset for KRISC32 core */\
        clear_bit__( REG32(U_SCRB__ASYN_RES_CTRL_REG), U_SCRB__ASYN_RES_CTRL_REG__RES_SOFT_KRISC32_CORE );\
    }\
    else\
    {\
        /* set asynchronous reset for KRISC32 core */\
        set_bit__( REG32(U_SCRB__ASYN_RES_CTRL_REG), U_SCRB__ASYN_RES_CTRL_REG__RES_SOFT_KRISC32_CORE );\
    }\
}
#endif

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200P)
#define EDDP_ERTEC200P_INIT_PHY_CONFIG_REG( _H_SYS_DEV, _CMD, _HW_PORT_ID)          \
{                                                                                   \
    LSA_UINT32 reg_val = 0x7D7D;                                                    \
    if(LSAS_PORT_IS_FO(1))                                                          \
    {                                                                               \
        reg_val |= 0x02;                                                            \
    }                                                                               \
    if(LSAS_PORT_IS_FO(2))                                                          \
    {                                                                               \
        reg_val |= 0x200;                                                           \
    }                                                                               \
    WRITE_USHORT(U_SCRB__PHY_CONFIG, reg_val);                                      \
    LSA_UNUSED_ARG(_H_SYS_DEV);                                                     \
    LSA_UNUSED_ARG(_HW_PORT_ID);                                                    \
}
#endif
#define EDDP_NRT_SEND_HOOK(_H_SYS_IF, _P_RQB)

/*****************************************************************************/
/*  end of file EDDP_CFG.H                                                   */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_CFG_H */


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
