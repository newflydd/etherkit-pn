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
/*  F i l e               &F: sock_cfg.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Configuration for LSA to socket-Interface                                */
/*  Defines constants, types and macros for SOCK.                            */
/*                                                                           */
/*  This file has to be overwritten during system integration, because       */
/*  some definitions depend on the different system, compiler or             */
/*  operating system.                                                        */
/*                                                                           */
/*****************************************************************************/
#ifndef SOCK_CFG_H
#define SOCK_CFG_H

#define  SOCK_CFG_USE_TCIP 1

#if 0 /* pcIOX */

void sock_pcpnio_startstop(int start);

#define SOCK_DEBUG  PCPNIO_DEBUG /* 0 or 1 */

/* LTRC-IF */

#define TRACE_SUBSYS_SOCK_UPPER     sock
#define TRACE_SUBSYS_SOCK_LOWER     sock
#define TRACE_SUBSYS_SOCK_SYSTEM    sock
#define TRACE_SUBSYS_SOCK_PROTOCOL  sock

extern int sock_log_level;          /* see LTRC_LEVEL_TYPE */
#define sock_log_get_fct            "sock" /* see LTRC_TRACE_01 */


#else

#define SOCK_DEBUG  		0

#endif

/*==============================================================================*/

/* -------------------------------------------------------------------------------------*/
/* enables fixing the LSA_SOCK to interniche TCP IP with the lower layer EDD/ EDD-ERTEC */
/* -------------------------------------------------------------------------------------*/

/* Define one of the following switches here:
    SOCK_CFG_INTERNICHE InterNiche Stack
    SOCK_CFG_OPEN_BSD   OpenBSD Stack
    SOCK_CFG_WINSOCK    winsock
    SOCK_CFG_CUSTOM     other stack used to implement the SockIF_xx functions
-----------------------------------------------------------------------------*/

/* Interniche configuration */
#ifdef LSAS_CFG_USE_EXTERNAL_TCPIP_STACK
#define SOCK_CFG_INTERNICHE  0
#define SOCK_CFG_OPEN_BSD    0
#define SOCK_CFG_WINSOCK     0
#else
#define SOCK_CFG_INTERNICHE  1
#define SOCK_CFG_OPEN_BSD    0
#define SOCK_CFG_WINSOCK     0
#endif

/* -------------------------------------------------------------------------------------*/


#if SOCK_CFG_INTERNICHE

/*=============================================================================
 *       socket functions for Version 1.9 (default) or 2.0
 * which supports IPV6
#define SOCK_CFG_INTERNICHE_V20 1
 *===========================================================================*/
#define SOCK_CFG_INTERNICHE_V20 1

/*=============================================================================
    // If SOCK_CFG_INTERNICHE_DEF_FD is 1, the  SOCKIF_FD_...
    // macros have to be defined
    // If SOCK_CFG_INTERNICHE_DEF_FD is 1, the necessary header files have to
    // be included !
 *===========================================================================*/
#define SOCK_CFG_INTERNICHE_DEF_FD 0

#endif /*SOCK_CFG_INTERNICHE*/

/*============================================================================*/
/*
 *       set local_mem static or dynamic:
 * static, don't use (no use of SOCK_ALLOC_LOCAL_MEM):
 * #define SOCK_LOCAL_MEM_STATIC
 * dynamic, must be used (use of SOCK_ALLOC_LOCAL_MEM):
 */
#define SOCK_LOCAL_MEM_DYNAMIC

/*============================================================================*/
/*       clear global data:
 * clear, don't use (SOCK_DATA_TYPE sock_data = {0};)
 * #define SOCK_CLEAR_GLOBAL_DATA
 * or no clear:
 */
#define SOCK_NO_CLEAR_GLOBAL_DATA

/*============================================================================*/

/*  Die SOCK_Komponente unterstuetzt UDP-Dienste     */
#define SOCK_CFG_UDP_ON 1 /* 0 or 1 */

/*============================================================================*/

/* die SOCK Komponente unterstuetzt Multicast      */
/*#define SOCK_CFG_ENABLE_MULTICASTS 1            */
/* die SOCK Komponent unterstuetzt Multicast nicht */
#ifdef LSAS_CFG_USE_EXTERNAL_TCPIP_STACK
#define SOCK_CFG_ENABLE_MULTICASTS 0
#else
#define SOCK_CFG_ENABLE_MULTICASTS 1
#endif

/* use ANY binding for multicast sockets */
#define SOCK_CFG_BIND_MULTICAST_SOCKETS_TO_ANY 0 /* 0 or 1.   0 is default behaviour as before (stack <=7.0). It is used in service SOCK_OPC_UDP_MULTICAST_OPEN,
                                                              which is not used by the current PNIO stack V7.1 or user. */

/*============================================================================*/

/* Die SOCK_Komponente unterstuetzt SNMP-Dienste      */
#define SOCK_CFG_SNMP_ON          1

/*============================================================================*/

/* SOCK fuehrt keine Timer-Ueberwachung von Aufrufen durch */
/* #define SOCK_CFG_TIMER_OFF 1  */

/* SOCK fuehrt die Timer-Ueberwachung von Aufrufen durch */
#define SOCK_CFG_TIMER_OFF 0

/*============================================================================*/
/*============================================================================*/

/* Maximale Anzahl der LSA-Kanaele (bzw. SOCK_CHANNEL_TYPE Datenstrukturen) */
#define SOCK_CFG_MAX_CHANNELS           20

/*============================================================================*/

/* maximum number of supported sockets */
#if SOCK_CFG_INTERNICHE
/* note: SOCK_CFG_MAX_SOCKETS is derived internally from TCIP_CFG_MAX_SOCKETS */
#define SOCK_CFG_MAX_SOCKETS 32
#else /* other IP stacks */
#define SOCK_CFG_MAX_SOCKETS 32 /* configure as needed, see sock_low.h */
#endif

/* maximum number of supported interfaces */
#define SOCK_CFG_MAX_INTERFACE_CNT      1 /* EDD_CFG_MAX_INTERFACE_CNT */



/* Trace der Kommunikation mit dem unterlagerten Stack. */
#define SOCK_STACK_TRACE_01(level, msg, para1)
#define SOCK_STACK_TRACE_02(level, msg, para1, para2)

/* Trace der Requestbloecke der uebergeordneten LSA-Komponente. */
#define SOCK_UPPER_REQ_TRACE_BYTE_ARRAY(level, msg, ptr, len)
#define SOCK_UPPER_RSP_TRACE_BYTE_ARRAY(level, msg, ptr, len)




/*===========================================================================*/
/*                          basic attributes for sock                        */
/*===========================================================================*/

/*===========================================================================*/
/*                      code- and data-attributes for sock                   */
/*===========================================================================*/

/*===========================================================================*/
/*                         code- and data-attributes                         */
/*===========================================================================*/

/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/

#define SOCK_EXCHANGE_TYPE long

/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/




/****************************************************************************/
/* Macro-Definitionen                                                       */
/****************************************************************************/

#if SOCK_DEBUG
#define SOCK_TEST_POINTER(ptr_)  (pcpnio_test_pointer(ptr_)) /* test for strange values */
#else
#define SOCK_TEST_POINTER(ptr_) ptr_
#endif

/*
 * Ausgangs-Funktionen zum System (sock_sys.h)
 */
#define SOCK_START_SELECT() \
{\
        LSAS_START_SOCK_SELECT();\
}

#define SOCK_ALLOC_TIMER(ret_val_ptr_, timer_id_ptr_, timer_type_, time_base_)                                          \
{                                                                                                                       \
    LSAS_ALLOC_TIMER(ret_val_ptr_, timer_id_ptr_, timer_type_, time_base_, sock_timeout, LSA_COMP_ID_SOCK);    \
}

#define SOCK_FREE_TIMER(ret_val_ptr_, timer_id_)        LSAS_FREE_TIMER(ret_val_ptr_, timer_id_, LSA_COMP_ID_SOCK);

#define SOCK_START_TIMER(ret_val_ptr_, timer_id_, user_id_, time_)                          \
{                                                                                           \
    LSAS_START_TIMER(ret_val_ptr_, timer_id_, user_id_, time_, LSA_COMP_ID_SOCK);  \
}
#define SOCK_STOP_TIMER(ret_val_ptr_, timer_id_)        LSAS_STOP_TIMER(ret_val_ptr_, timer_id_, LSA_COMP_ID_SOCK);\

/*--------------------------------------------------------------------------*/
/* set and get parameter of the rqb-header                                  */
/*--------------------------------------------------------------------------*/

#ifdef  LSA_RQB_HEADER
#define SOCK_RQB_SET_NEXT_RQB_PTR(rb_, val_)    LSA_RQB_SET_NEXT_RQB_PTR(rb_, val_)
#define SOCK_RQB_SET_PREV_RQB_PTR(rb_, val_)    LSA_RQB_SET_PREV_RQB_PTR(rb_, val_)

#define SOCK_RQB_SET_OPCODE(rb_, val_)          LSA_RQB_SET_OPCODE(rb_, val_)
#define SOCK_RQB_GET_OPCODE(rb_)                LSA_RQB_GET_OPCODE(rb_)

#define SOCK_RQB_SET_HANDLE(rb_, val_)          LSA_RQB_SET_HANDLE(rb_, val_)
#define SOCK_RQB_GET_HANDLE(rb_)                LSA_RQB_GET_HANDLE(rb_)

#define SOCK_RQB_SET_RESPONSE(rb_, val_)        LSA_RQB_SET_RESPONSE(rb_, val_)
#define SOCK_RQB_GET_RESPONSE(rb_)              LSA_RQB_GET_RESPONSE(rb_)

#define SOCK_RQB_GET_NEXT_RQB_PTR(rb_)          LSA_RQB_GET_NEXT_RQB_PTR(rb_)
#define SOCK_RQB_GET_PREV_RQB_PTR(rb_)          LSA_RQB_GET_PREV_RQB_PTR(rb_)

#define SOCK_RQB_SET_USER_ID_PTR(_pRQB, _Value) LSA_RQB_SET_USER_ID_PTR(_pRQB, _Value)
#define SOCK_RQB_GET_USER_ID_PTR(_pRQB)         LSA_RQB_GET_USER_ID_PTR(_pRQB)

#else
#error "How this? LSA_RQB_HEADER must be befined at this point"
#endif

/*=============================================================================
 *   CRUN functions
 *===========================================================================*/
/*=============================================================================
 * function name:  SOCK_STRLEN
 *
 * function:       Returns the length of str.
 *                 The length of a C string is determined by the terminating
 *                 null-character: A C string is as long as the amount of
 *                 characters between the beginning of the string and the
 *                 terminating null character.
 *
 * parameters:     const char *                    str
 *
 * return value:   LSA_UINT32
 *============================================================================*/

#define SOCK_STRLEN(str) LSAS_STRLEN(str)

/*=============================================================================
 * function name:  SOCK_MEMSET
 *
 * function:       fills memory with value
 *
 * parameters:      SOCK_LOCAL_MEM_PTR_TYPE        pMem
 *                  LSA_UINT8                      Value
 *                  LSA_UINT32                     Length
 *
 * return value:   LSA_VOID
 *============================================================================*/

#define SOCK_MEMSET(pMem,Value,Length) LSAS_MEMSET(pMem,Value,Length)

/*=============================================================================
 * function name:  SOCK_MEMCPY
 *
 * function:       copy Length bytes memory from pSrc to pDst.
 *
 * parameters:      SOCK_COMMON_MEM_PTR_TYPE        pDst
 *                  SOCK_COMMON_MEM_PTR_TYPE        pSrc
 *                  LSA_UINT32                      Length
 *
 * return value:   LSA_VOID
 *===========================================================================*/

#define SOCK_MEMCPY(pDst,pSrc,Length) LSAS_MEMCPY(pDst,pSrc,Length)

/*=============================================================================
 * function name:  SOCK_MEMMOVE
 *
 * function:       copy Length bytes memory from pSrc to pDst.
 *
 * parameters:      SOCK_COMMON_MEM_PTR_TYPE        pDst
 *                  SOCK_COMMON_MEM_PTR_TYPE        pSrc
 *                  LSA_UINT32                     Length
 *
 * return value:   LSA_VOID
 *===========================================================================*/

#define SOCK_MEMMOVE(pDst,pSrc,Length) LSAS_MEMMOVE(pDst,pSrc,Length)

/*=============================================================================
 * function name:  SOCK_MEMCMP
 *
 * function:       compares Length bytes of memory from pBuf1 with pBuf2
 *
 * parameters:      SOCK_COMMON_MEM_PTR_TYPE        pBuf1
 *                  SOCK_COMMON_MEM_PTR_TYPE        pBuf2
 *                  LSA_UINT32                      Length
 *
 * return value:   0   : buffers are the same
 *                 !=0 : buffers differ
 *===========================================================================*/

#define SOCK_MEMCMP(pBuf1,pBuf2, Length) LSAS_MEMCMP(pBuf1,pBuf2, Length)

/*------------------------------------------------------------------------------
// interface to LSA trace
//    0 = no LSA trace
//    1 = LSA trace [default]
//----------------------------------------------------------------------------*/

#ifndef SOCK_CFG_TRACE_MODE
# define SOCK_CFG_TRACE_MODE  1
#endif

/*------------------------------------------------------------------------------
// interface to BTRACE
//----------------------------------------------------------------------------*/

#ifndef SOCK_FILE_SYSTEM_EXTENSION
# define SOCK_FILE_SYSTEM_EXTENSION(module_id_)    /* nothing */
#endif


/*===========================================================================*/
/*                                types for sock                             */
/*===========================================================================*/

/*=============================================================================
 *      sock common request block type
=============================================================================*/

#define SOCK_RQB_HEADER   LSA_RQB_HEADER(SOCK_UPPER_RQB_PTR_TYPE)

#define SOCK_RQB_TRAILER  /* nothing by default */

#endif

/*****************************************************************************/
/*  end of file                                                              */
/*****************************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
