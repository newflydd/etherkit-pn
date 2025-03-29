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
/*  F i l e               &F: tcip_cfg.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Configuration module                                                     */
/*                                                                           */
/*****************************************************************************/
#ifndef TCIP_CFG_H                       /* ----- reinclude-protection ----- */
#define TCIP_CFG_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif


/*===========================================================================*/
/*=== configuration =========================================================*/
/*===========================================================================*/

#define TCIP_DEBUG  0 /* 0 or 1 */


#define TCIP_CFG_INTERNICHE 1
#define TCIP_CFG_OPEN_BSD 0

/*===========================================================================*/
/*===========================================================================*/

#define TCIP_CFG_TRACE_MODE 1

#ifndef TCIP_CFG_TRACE_MODE
#error "TCIP_CFG_TRACE_MODE not defined"
#endif

#ifndef TCIP_FILE_SYSTEM_EXTENSION
# define TCIP_FILE_SYSTEM_EXTENSION(module_id_)  /* nothing */
#endif

/*===========================================================================*/
/*===========================================================================*/
#ifdef LSAS_CFG_BIG_ENDIAN
    #define TCIP_CFG_BIG_ENDIAN 1
#else
    #define TCIP_CFG_BIG_ENDIAN 0
#endif

/*===========================================================================*/
/*===========================================================================*/

#ifndef TCIP_CFG_COPY_ON_SEND
 #if EDD_CFG_MAX_INTERFACE_CNT > 1
  #define TCIP_CFG_COPY_ON_SEND  1
 #else
  #define TCIP_CFG_COPY_ON_SEND  0
#endif
#endif

/*===========================================================================*/
/*===========================================================================*/

#define TCIP_CFG_MAX_SOCKETS  16


/*
   sysServices OBJECT-TYPE
       SYNTAX      INTEGER (0..127)
       MAX-ACCESS  read-only
       STATUS      current
       DESCRIPTION
               "A value which indicates the set of services that this
               entity may potentially offer.  The value is a sum.
               This sum initially takes the value zero. Then, for
               each layer, L, in the range 1 through 7, that this node
               performs transactions for, 2 raised to (L - 1) is added
               to the sum.  For example, a node which performs only
               routing functions would have a value of 4 (2^(3-1)).
               In contrast, a node which is a host offering application
               services would have a value of 72 (2^(4-1) + 2^(7-1)).
               Note that in the context of the Internet suite of
               protocols, values should be calculated accordingly:

                    layer      functionality
                      1        physical (e.g., repeaters)
                      2        datalink/subnetwork (e.g., bridges)
                      3        internet (e.g., supports the IP)
                      4        end-to-end  (e.g., supports the TCP)
                      7        applications (e.g., supports the SMTP)

               For systems including OSI protocols, layers 5 and 6
               may also be counted."
*/

#define TCIP_CFG_ENABLE_MULTICAST 1


/*===========================================================================*/
/*===========================================================================*/

#define TCIP_CFG_QUEUE_CHECK_ENABLE  (TCIP_DEBUG ? 1 : 0) /* see macro QUEUE_CHECK */

#define TCIP_CFG_SOC_CHECK_ENABLE  (TCIP_DEBUG ? 1 : 0) /* see macro SOC_CHECK */

#define TCIP_CFG_SOC_CHECK_MORE  0 /* 1 enables SOC_CHECK in t_recv(), t_send(), t_select() */

/*===========================================================================*/
/*===========================================================================*/

/*===========================================================================*/
/*===========================================================================*/

/*===========================================================================*/
/*===========================================================================*/

#define TCIP_CFG_SNMP_ON 1 /* enable SNMP services */

/*===========================================================================*/
/*===========================================================================*/

#define TCIP_CFG_KEEPALIVE_IDLE_TIMEOUT     30 /* TCP Keep-Alive Idle Time in seconds; for default value undefine the parameter */
#define TCIP_CFG_KEEPALIVE_PROBE_INTERVAL   1  /* TCP Keep-Alive Interval in seconds; for default value undefine the parameter */
#define TCIP_CFG_KEEPALIVE_PROBE_COUNT      5  /* TCP Keep-Alive Probe Count; for default value undefine the parameter */


/*===========================================================================*/
/*===========================================================================*/



#if TCIP_DEBUG

#define TCIP_TEST_POINTER(ptr_)  (pcpnio_test_pointer(ptr_)) /* test for strange values */

/* check if condition holds or call fatal (programming error) */
#define TCIP_ASSERT(cond_) { if (!(cond_)) { TCIP_FATAL(); } }

#else

#define TCIP_TEST_POINTER(ptr_)  (ptr_) /* don't test */

#define TCIP_ASSERT(cond_) { /* empty-block in release version */ }

#endif

/*===========================================================================*/
/*                         code- and data-attributes                         */
/*===========================================================================*/

/*
 * all known platforms have a "flat" memory-model
 */

#define TCIP_FCTATTR_FLAT  /* nothing */
#define TCIP_MEMATTR_FLAT  /* nothing */

/*
 * code attributes
 */

#define TCIP_UPPER_IN_FCT_ATTR        TCIP_FCTATTR_FLAT  /* tcip-user to tcip */
#define TCIP_UPPER_OUT_FCT_ATTR       TCIP_FCTATTR_FLAT  /* tcip to tcip-user */

#define TCIP_EDD_LOWER_IN_FCT_ATTR    TCIP_FCTATTR_FLAT  /* lower component to tcip */
#define TCIP_EDD_LOWER_OUT_FCT_ATTR   TCIP_FCTATTR_FLAT  /* tcip to lower component */

#define TCIP_SYSTEM_IN_FCT_ATTR       TCIP_FCTATTR_FLAT  /* system to tcip */
#define TCIP_SYSTEM_OUT_FCT_ATTR      TCIP_FCTATTR_FLAT  /* tcip to system */

#define TCIP_LOCAL_FCT_ATTR           TCIP_FCTATTR_FLAT  /* local function */

/*
 * data attributes
 */

#define TCIP_UPPER_RQB_ATTR       TCIP_MEMATTR_FLAT  /* upper-rqb data */
#define TCIP_UPPER_MEM_ATTR       TCIP_MEMATTR_FLAT  /* upper-memory data */

#define TCIP_EDD_LOWER_RQB_ATTR   TCIP_MEMATTR_FLAT  /* lower-rqb data */
#define TCIP_EDD_LOWER_MEM_ATTR   TCIP_MEMATTR_FLAT  /* lower-memory data */

#define TCIP_SYSTEM_MEM_ATTR      TCIP_MEMATTR_FLAT  /* system-memory data */

#define TCIP_LOCAL_MEM_ATTR       TCIP_MEMATTR_FLAT  /* local data */

#define TCIP_COMMON_MEM_ATTR      TCIP_MEMATTR_FLAT  /* greatest common data attribute */

/*===========================================================================*/
/*===========================================================================*/

/*
 * TCIP-HEADER and TCIP-TRAILER
 */

#if ! defined(LSA_RQB_HEADER) || ! defined(LSA_RQB_TRAILER)
# error "LSA_RQB_HEADER or LSA_RQB_TRAILER not defined in lsa_cfg.h"
#endif


#define TCIP_RQB_HEADER \
    LSA_RQB_HEADER(TCIP_UPPER_RQB_PTR_TYPE) /* standard LSA-header comes first */ \
    /* end of TCIP_RQB_HEADER */


#define TCIP_RQB_TRAILER \
    LSA_RQB_TRAILER /* standard LSA-trailer comes last */ \
    /* end of TCIP_RQB_TRAILER */


#define TCIP_RQB_SET_NEXT_RQB_PTR(rb_, val_)    LSA_RQB_SET_NEXT_RQB_PTR(rb_, val_)
#define TCIP_RQB_GET_NEXT_RQB_PTR(rb_)            LSA_RQB_GET_NEXT_RQB_PTR(rb_)

#define TCIP_RQB_SET_PREV_RQB_PTR(rb_, val_)    LSA_RQB_SET_PREV_RQB_PTR(rb_, val_)
#define TCIP_RQB_GET_PREV_RQB_PTR(rb_)            LSA_RQB_GET_PREV_RQB_PTR(rb_)

#define TCIP_RQB_SET_OPCODE(rb_, val_)            LSA_RQB_SET_OPCODE(rb_, val_)
#define TCIP_RQB_GET_OPCODE(rb_)                LSA_RQB_GET_OPCODE(rb_)

#define TCIP_RQB_SET_HANDLE(rb_, val_)            LSA_RQB_SET_HANDLE(rb_, val_)
#define TCIP_RQB_GET_HANDLE(rb_)                LSA_RQB_GET_HANDLE(rb_)

#define    TCIP_RQB_SET_USER_ID(rb_, val_)            LSA_RQB_SET_USER_ID(rb_, val_)
#define    TCIP_RQB_GET_USER_ID(rb_)                LSA_RQB_GET_USER_ID(rb_)

#define TCIP_RQB_SET_USER_ID_PTR(rb_, val_)        LSA_RQB_SET_USER_ID_PTR(rb_, val_)
#define TCIP_RQB_GET_USER_ID_PTR(rb_)            LSA_RQB_GET_USER_ID_PTR(rb_)

#define TCIP_RQB_SET_RESPONSE(rqb_, v_)            LSA_RQB_SET_RESPONSE(rqb_, v_)
#define TCIP_RQB_GET_RESPONSE(rqb_)                LSA_RQB_GET_RESPONSE(rqb_)

/*===========================================================================*/
/*===========================================================================*/

/*
 * lower layer macros for EDD
 */

#define TCIP_EDD_INVALID_HANDLE    0xFF /* EDD doesn't export one */

#define TCIP_EDD_LOWER_RQB_TYPE    EDD_RQB_TYPE

#define TCIP_EDD_SET_NEXT_RQB_PTR(rb_, val_)    EDD_RQB_SET_NEXT_RQB_PTR(rb_, val_)
#define TCIP_EDD_SET_PREV_RQB_PTR(rb_, val_)    EDD_RQB_SET_PREV_RQB_PTR(rb_, val_)
#define TCIP_EDD_SET_OPCODE(rb_, opc_, svc_)    { EDD_RQB_SET_OPCODE(rb_, opc_); EDD_RQB_SET_SERVICE(rb_, svc_); }
#define TCIP_EDD_SET_HANDLE(rb_, val_)            EDD_RQB_SET_HANDLE(rb_, val_)
#define TCIP_EDD_SET_USER_ID_PTR(rb_, val_)        EDD_RQB_SET_USERID_PTR(rb_, val_)
#define TCIP_EDD_SET_RESPONSE(rb_, val_)        EDD_RQB_SET_RESPONSE(rb_, val_)

#define TCIP_EDD_GET_NEXT_RQB_PTR(rb_)            EDD_RQB_GET_NEXT_RQB_PTR(rb_)
#define TCIP_EDD_GET_PREV_RQB_PTR(rb_)            EDD_RQB_GET_PREV_RQB_PTR(rb_)
#define TCIP_EDD_GET_OPCODE(rb_)                EDD_RQB_GET_OPCODE(rb_)
#define TCIP_EDD_GET_SERVICE(rb_)                EDD_RQB_GET_SERVICE(rb_)
#define TCIP_EDD_GET_HANDLE(rb_)                EDD_RQB_GET_HANDLE(rb_)
#define TCIP_EDD_GET_USER_ID_PTR(rb_)            EDD_RQB_GET_USERID_PTR(rb_)
#define TCIP_EDD_GET_RESPONSE(rb_)                EDD_RQB_GET_RESPONSE(rb_)

#define TCIP_EDD_GET_USER_ID_HANDLE(rb_) ( /* get channel_handle */ \
    LSA_RQB_GET_USER_ID_UVAR8_ARRAY_2(rb_) \
    )

/*===========================================================================*/
/*===========================================================================*/

#ifndef TCIP_FILE_SYSTEM_EXTENSION
# define TCIP_FILE_SYSTEM_EXTENSION(module_id_)  /* nothing */
#endif

/*===========================================================================*/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*********************
 * self defined
 */

#define ICMP_RESPONSE(buf_ptr, buf_length)                                  \
{                                                                           \
    LSAS_TCPIP_ICMP_RESPONSE(buf_ptr, buf_length);                          \
}


#ifndef min
#define  min(x,y)          ((x) <= (y) ? (x) : (y))
#endif

#ifndef max
#define  max(x,y)          ((x) >= (y) ? (x) : (y))
#endif

#endif  /* of TCIP_CFG_H (reinclude protection) */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
