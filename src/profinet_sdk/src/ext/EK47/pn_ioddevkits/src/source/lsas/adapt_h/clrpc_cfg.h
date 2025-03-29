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
/*  F i l e               &F: clrpc_cfg.h                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Implements system integration of CLRPC using the pcPNIO framework.       */
/*                                                                           */
/*****************************************************************************/
#ifndef CLRPC_CFG_H
#define CLRPC_CFG_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif


/*
 * included by "clrpc_inc.h"
 */

/*===========================================================================*/
/*                             compiler-switches                             */
/*===========================================================================*/

#define CLRPC_DEBUG                   0

/*===========================================================================*/
/*                         code- and data-attributes                         */
/*===========================================================================*/

/*===========================================================================*/
/* CLRPC-header and CLRPC-trailer                                            */
/*===========================================================================*/

#if ! defined(LSA_RQB_HEADER) || ! defined(LSA_RQB_TRAILER)
# error "LSA_RQB_HEADER or LSA_RQB_TRAILER not defined in lsa_cfg.h"
#endif


#define CLRPC_RQB_HEADER                        LSA_RQB_HEADER(CLRPC_UPPER_RQB_PTR_TYPE)
#define CLRPC_RQB_TRAILER                       LSA_RQB_TRAILER


#define CLRPC_RQB_SET_NEXT_RQB_PTR(rb_, val_)   LSA_RQB_SET_NEXT_RQB_PTR(rb_, val_)
#define CLRPC_RQB_GET_NEXT_RQB_PTR(rb_)         LSA_RQB_GET_NEXT_RQB_PTR(rb_)

#define CLRPC_RQB_SET_PREV_RQB_PTR(rb_, val_)   LSA_RQB_SET_PREV_RQB_PTR(rb_, val_)
#define CLRPC_RQB_GET_PREV_RQB_PTR(rb_)         LSA_RQB_GET_PREV_RQB_PTR(rb_)

#define CLRPC_RQB_SET_OPCODE(rb_, val_)         LSA_RQB_SET_OPCODE(rb_, val_)
#define CLRPC_RQB_GET_OPCODE(rb_)               LSA_RQB_GET_OPCODE(rb_)

#define CLRPC_RQB_SET_HANDLE(rb_, val_)         LSA_RQB_SET_HANDLE(rb_, val_)
#define CLRPC_RQB_GET_HANDLE(rb_)               LSA_RQB_GET_HANDLE(rb_)

#define CLRPC_RQB_SET_RESPONSE(rb_, val_)       LSA_RQB_SET_RESPONSE(rb_, val_)
#define CLRPC_RQB_GET_RESPONSE(rb_)             LSA_RQB_GET_RESPONSE(rb_)

#define CLRPC_RQB_SET_USER_ID_PTR(rb_, val_)    LSA_RQB_SET_USER_ID_PTR(rb_, val_)
#define CLRPC_RQB_GET_USER_ID_PTR(rb_)          LSA_RQB_GET_USER_ID_PTR(rb_)


/*===========================================================================*/
/* LOWER LAYER MACROS                                                        */
/*===========================================================================*/

#define CLRPC_LL_INVALID_HANDLE ((LSA_HANDLE_TYPE)-1) /*check this - NOTE: do not check with #if, CPP sees type-cast as an undefined symbol and assumes zero... */
#define CLRPC_LL_INVALID_SOCKET ((LSA_INT32)-1) /*check this - NOTE: do not check with #if, CPP sees type-cast as an undefined symbol and assumes zero... */

#define CLRPC_LOWER_RQB_TYPE  SOCK_RQB_TYPE

#define CLRPC_LL_SET_NEXT_RQB_PTR(rb_, val_)    LSA_RQB_SET_NEXT_RQB_PTR(rb_, val_)
#define CLRPC_LL_SET_PREV_RQB_PTR(rb_, val_)    LSA_RQB_SET_PREV_RQB_PTR(rb_, val_)
#define CLRPC_LL_SET_OPCODE(rb_, val_)          LSA_RQB_SET_OPCODE(rb_, val_)
#define CLRPC_LL_SET_HANDLE(rb_, val_)          LSA_RQB_SET_HANDLE(rb_, val_)
#define CLRPC_LL_SET_RESPONSE(rb_, val_)        LSA_RQB_SET_RESPONSE(rb_, val_)
#define CLRPC_LL_SET_USER_ID_PTR(rb_, val_)     LSA_RQB_SET_USER_ID_PTR(rb_, val_)

#define CLRPC_LL_GET_NEXT_RQB_PTR(rb_)          LSA_RQB_GET_NEXT_RQB_PTR(rb_)
#define CLRPC_LL_GET_PREV_RQB_PTR(rb_)          LSA_RQB_GET_PREV_RQB_PTR(rb_)
#define CLRPC_LL_GET_OPCODE(rb_)                LSA_RQB_GET_OPCODE(rb_)
#define CLRPC_LL_GET_HANDLE(rb_)                LSA_RQB_GET_HANDLE(rb_)
#define CLRPC_LL_GET_RESPONSE(rb_)              LSA_RQB_GET_RESPONSE(rb_)
#define CLRPC_LL_GET_USER_ID_PTR(rb_)           LSA_RQB_GET_USER_ID_PTR(rb_)



/*=============================================================================
 *       set local_mem static or dynamic:
 * static (no use of CLRPC_ALLOC_LOCAL_MEM):
 * #define CLRPC_LOCAL_MEM_STATIC
 * or dynamic (use of CLRPC_ALLOC_LOCAL_MEM):
 * #define CLRPC_LOCAL_MEM_DYNAMIC
 *===========================================================================*/

#define CLRPC_LOCAL_MEM_DYNAMIC /* don't change. there is no choice! */

/*=============================================================================
 *       clear global data:
 * clear (CLRPC_DATA_TYPE clrpc_data = {0};):
 * #define CLRPC_CLEAR_GLOBAL_DATA
 * or no clear:
 * #define CLRPC_NO_CLEAR_GLOBAL_DATA
 *===========================================================================*/

#define CLRPC_NO_CLEAR_GLOBAL_DATA /* don't change. there is no choice! */


/*===========================================================================*/
/* configurable things                                                       */
/*===========================================================================*/

    /* 1 = use casts to avoid some warnings
    //-----------------------------------------------------------*/
#define CLRPC_CFG_USE_CASTS 1
#ifndef CLRPC_CFG_USE_CASTS
# define CLRPC_CFG_USE_CASTS 0
#endif

    /* configure the number of communication channels
    //-----------------------------------------------------------*/
#define CLRPC_CFG_MAX_CHANNELS                 2
#ifndef CLRPC_CFG_MAX_CHANNELS
# error "CLRPC_CFG_MAX_CHANNELS not defined" /* per NIC: 1(PATH_TYPE_EPM) + 2(PATH_TYPE_USER, CM and Easy-Supervisor) */
#endif



    /* configure the client part
    //-----------------------------------------------------------*/
#define CLRPC_CFG_MAX_CLIENTS                  (DEV_CFG_MAX_ARS_RT + DEV_CFG_MAX_ARS_DA)
                                                /* RT_AR's + Device Access */
                                                /* 0=no client-part  or  number of clients per channel */
                                                /* AR's */
/* CLRPC_CFG_MAX_CLIENTS =  ptr.rqb->args.sv.dev_add->max_ars_RTClass1_2    = 2;
                          + ptr.rqb->args.sv.dev_add->max_ars_RTClass3      = 1;
                          + ptr.rqb->args.sv.dev_add->max_ars_DeviceAccess  = 1; */
#ifndef CLRPC_CFG_MAX_CLIENTS
# error "CLRPC_CFG_MAX_CLIENTS not defined"  /* 0=no client-part  or  number of clients per channel */
#endif


    /* configure the server part
    // max number of interfaces
    //    3: Endpoint-Mapper-If, IPNIO-Controller-If, IPNIO-Server-If
    //-----------------------------------------------------------*/
# define CLRPC_CFG_MAX_SERVERS                  3/* 0=no server-part  or  total number of servers (interfaces) */
                                                 /* consumer, iod, end point mapper */
#ifndef CLRPC_CFG_MAX_SERVERS
# error "CLRPC_CFG_MAX_SERVERS not defined" /* 0=no server-part  or  total number of servers (interfaces) */
#endif


    /* configure the server part
    // max number of 'workers' (for all interfaces)
    //  e.g. _MAX_CL_DEVICES + _MAX_SV_DEVICES * (_MAX_AR + _MAX_SV_READ_IMPL)
    //-----------------------------------------------------------*/
# define CLRPC_CFG_MAX_SCALLS                  CLRPC_CFG_MAX_CLIENTS /*3*/ + CLRPC_CFG_MAX_READ_IMPLICIT_PER_OBJ_UUID /*10*/ + 20 /*sock_recv_resource_count*/ + 20 /*Reserve*/ /*old  100 */
#ifndef CLRPC_CFG_MAX_SCALLS
# error "CLRPC_CFG_MAX_SCALLS not defined"
#endif

/* configure the server part
// AP00830411: max number of contexts (simultaneous lookup requests for endpoint mapper)
//-----------------------------------------------------------*/
#define CLRPC_CFG_MAX_CONTEXTS 6
#ifndef CLRPC_CFG_MAX_CONTEXTS
# error "CLRPC_CFG_MAX_CONTEXTS not defined"
#endif

    /* configure the server part
    // max number of read implicit per object_uuid
    //  valid for all interfaces with pnio-signature
    //  0: no read implicit allowed
    //  n: n parallel read implicits are allowed on every registered object-uuid
    //-----------------------------------------------------------*/
#define CLRPC_CFG_MAX_READ_IMPLICIT_PER_OBJ_UUID   (10)        /* 5..SyncLogger(4) + reserve(1) */

#ifndef CLRPC_CFG_MAX_READ_IMPLICIT_PER_OBJ_UUID
# error "CLRPC_CFG_MAX_READ_IMPLICIT_PER_OBJ_UUID"  /* some for SyncLogger + reserve */
#endif


    /* configure the integer data representation
     * the definition must reflect the hardware. there is no choice!
    //-----------------------------------------------------------*/
#ifdef LSAS_CFG_BIG_ENDIAN
	#define CLRPC_CFG_DREP_INT                      CLRPC_DREP_INT_BIG /* see clrpc_usr.h, CLRPC_DREP_INT_XXX */
#else
    #define CLRPC_CFG_DREP_INT                      CLRPC_DREP_INT_LITTLE /* see clrpc_usr.h, CLRPC_DREP_INT_XXX */
#endif

    /* configure the character data representation
     * the definition must reflect the hardware. there is no choice!
    //-----------------------------------------------------------*/
#define CLRPC_CFG_DREP_CHAR                     CLRPC_DREP_CHAR_ASCII /* see clrpc_usr.h, CLRPC_DREP_CHAR_XXX */

    /* configure the floating-point data representation
     * the definition must reflect the hardware. there is no choice!
    //-----------------------------------------------------------*/
#define CLRPC_CFG_DREP_FLOAT                    CLRPC_DREP_FLOAT_IEEE /* see clrpc_usr.h, CLRPC_DREP_FLOAT_XXX */


/*===========================================================================*/
/*                                 macros                                    */
/*===========================================================================*/


/*------------------------------------------------------------------------------
//  Assert
//
//  ==> must be empty in Release-Version!
//----------------------------------------------------------------------------*/

#if CLRPC_DEBUG

/* check if condition holds or call fatal (programming error) */
#define CLRPC_ASSERT(cond_)                 \
{                                           \
    LSA_BOOL tmp_cond = (LSA_BOOL)(cond_);  \
                                            \
    if (!(tmp_cond))                        \
    {                                       \
        CLRPC_FATAL_ERROR(0, NIL);          \
    }                                       \
}                                           \

#else

#define CLRPC_ASSERT(cond_)   LSA_UNUSED_ARG(cond_)

#endif


/*------------------------------------------------------------------------------
// Test a pointer
//
//  ==> must be empty in Release-Version!
//----------------------------------------------------------------------------*/

#if CLRPC_DEBUG

#define CLRPC_TEST_POINTER(ptr_)  (ptr_)

#define CLRPC_TEST_ALIGN2(ptr_)   (ptr_) /* no test */
#define CLRPC_TEST_ALIGN4(ptr_)   (ptr_) /* no test */

#else

#define CLRPC_TEST_POINTER(ptr_)  (ptr_)

#define CLRPC_TEST_ALIGN2(ptr_)   (ptr_) /* no test */
#define CLRPC_TEST_ALIGN4(ptr_)   (ptr_) /* no test */

#endif


/*------------------------------------------------------------------------------
//  System Layer (prototypes in clrpc_sys.h)
//----------------------------------------------------------------------------*/
#if 0 /*not used */
#define CLRPC_LOWER_TO_UPPER_MEM\
{\
    not defined !\
}

#define CLRPC_UPPER_TO_LOWER_MEM\
{\
    not defined !\
}

#define CLRPC_LOWER_TO_LOWER_MEM\
{\
    not defined !\
}

#define CLRPC_COPY_COMMON_MEM\
{\
    not defined !\
}
#endif

#define CLRPC_FATAL_ERROR(length,error_ptr)                     \
{                                                               \
    LSAS_FATAL_ERROR( error_ptr);    \
}

/*------------------------------------------------------------------------------
// interface to LTRC
//----------------------------------------------------------------------------*/

/*=============================================================================
 * If the LSA component LTRC isn't used for trace in the target system, then
 * the prefix trace macros can be defined here.
 *
 * See also files acp_trc.h and lsa_cfg.h/txt.
 *
 *===========================================================================*/

/*------------------------------------------------------------------------------
// interface to LSA trace
//    0 = no LSA trace
//    1 = LSA trace [default]
//----------------------------------------------------------------------------*/

#ifndef CLRPC_CFG_TRACE_MODE
# define CLRPC_CFG_TRACE_MODE  1
#endif

#if (CLRPC_CFG_TRACE_MODE == 1)
//Trace-Makros Konzeptbedingt verschoben
#endif


/*------------------------------------------------------------------------------
// interface to BTRACE
//----------------------------------------------------------------------------*/

#ifndef CLRPC_FILE_SYSTEM_EXTENSION
# define CLRPC_FILE_SYSTEM_EXTENSION(module_id_)    /* nothing */
#endif


/*----------------------------------------------------------------------------*/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif  /* of CLRPC_CFG_H */
 
/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
