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
/*  F i l e               &F: acp_cfg.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  System integration of LSA-component ACP                                  */
/*  Using the PCPNIO framework.                                              */
/*                                                                           */
/*****************************************************************************/

#ifndef ACP_CFG_H
#define ACP_CFG_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif


/*
 * included by "acp_inc.h"
 */

/*===========================================================================*/
/*                             compiler-switches                             */
/*===========================================================================*/

#define ACP_DEBUG                   0

/*===========================================================================*/
/*                              basic attributes                             */
/*===========================================================================*/


/*===========================================================================*/
/*                         code- and data-attributes                         */
/*===========================================================================*/


/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/

/*------------------------------------------------------------------------------
// the lower layer
//----------------------------------------------------------------------------*/

#define ACP_LOWER_RQB_TYPE                      EDD_RQB_TYPE

#define ACP_LL_GET_NEXT_RQB_PTR(pRQB)           EDD_RQB_GET_NEXT_RQB_PTR (pRQB)
#define ACP_LL_GET_PREV_RQB_PTR(pRQB)           EDD_RQB_GET_PREV_RQB_PTR (pRQB)

#define ACP_LL_SET_OPCODE(pRQB, value)          EDD_RQB_SET_OPCODE (pRQB, value)
#define ACP_LL_GET_OPCODE(pRQB)                 EDD_RQB_GET_OPCODE (pRQB)

#define ACP_LL_SET_SERVICE(pRQB, value)         EDD_RQB_SET_SERVICE (pRQB, value)
#define ACP_LL_GET_SERVICE(pRQB)                EDD_RQB_GET_SERVICE (pRQB)

#define ACP_LL_SET_HANDLE(pRQB, value)          EDD_RQB_SET_HANDLE (pRQB, value)
#define ACP_LL_GET_HANDLE(pRQB)                 EDD_RQB_GET_HANDLE (pRQB)

#define ACP_LL_SET_RESPONSE(pRQB, value)        EDD_RQB_SET_RESPONSE (pRQB, value)
#define ACP_LL_GET_RESPONSE(pRQB)               EDD_RQB_GET_RESPONSE (pRQB)

#define ACP_LL_SET_USERID_PTR(pRQB, value)      EDD_RQB_SET_USERID_PTR (pRQB, value)
#define ACP_LL_GET_USERID_PTR(pRQB)             EDD_RQB_GET_USERID_PTR (pRQB)

#define ACP_LL_SET_PPARAM(pRQB, value)          EDD_RQB_SET_PPARAM (pRQB, value)
#define ACP_LL_GET_PPARAM(pRQB, type)           ((type)(EDD_RQB_GET_PPARAM (pRQB)))

/*===========================================================================*/
/* LSA-HEADER and LSA-TRAILER                                                */
/*===========================================================================*/

#define ACP_RQB_HEADER      LSA_RQB_HEADER(ACP_UPPER_RQB_PTR_TYPE)

#define ACP_RQB_TRAILER     LSA_RQB_TRAILER

#define ACP_RQB_SET_NEXT_RQB_PTR(rb,v)      LSA_RQB_SET_NEXT_RQB_PTR(rb,v)
#define ACP_RQB_SET_PREV_RQB_PTR(rb,v)      LSA_RQB_SET_PREV_RQB_PTR(rb,v)
#define ACP_RQB_SET_OPCODE(rb,v)            LSA_RQB_SET_OPCODE(rb,v)
#define ACP_RQB_SET_HANDLE(rb,v)            LSA_RQB_SET_HANDLE(rb,v)
#define ACP_RQB_SET_RESPONSE(rb,v)          LSA_RQB_SET_RESPONSE(rb,v)
#define ACP_RQB_SET_USER_ID_PTR(rb,v)       LSA_RQB_SET_USER_ID_PTR(rb,v)

#define ACP_RQB_GET_NEXT_RQB_PTR(rb)        LSA_RQB_GET_NEXT_RQB_PTR(rb)
#define ACP_RQB_GET_PREV_RQB_PTR(rb)        LSA_RQB_GET_PREV_RQB_PTR(rb)
#define ACP_RQB_GET_OPCODE(rb)              LSA_RQB_GET_OPCODE(rb)
#define ACP_RQB_GET_HANDLE(rb)              LSA_RQB_GET_HANDLE(rb)
#define ACP_RQB_GET_RESPONSE(rb)            LSA_RQB_GET_RESPONSE(rb)
#define ACP_RQB_GET_USER_ID_PTR(rb)         LSA_RQB_GET_USER_ID_PTR(rb)


/*------------------------------------------------------------------------------
// maximum number of open LSA channels
//----------------------------------------------------------------------------*/

# define ACP_CFG_MAX_CHANNELS               2

#ifndef ACP_CFG_MAX_CHANNELS
# error "ACP_CFG_MAX_CHANNELS not defined"   /* 4 .. System / UserIOC / UserIOD / UserIOM */
#endif


/*------------------------------------------------------------------------------
// enable / disable cast-warnings
//----------------------------------------------------------------------------*/
#ifndef ACP_CFG_CAST_WARN
# define ACP_CFG_CAST_WARN  0
#endif


/*------------------------------------------------------------------------------
// enable / disable ACP_ASSERT
//----------------------------------------------------------------------------*/

#if ACP_DEBUG

extern int acp_semicolon_missing; /* see acp_cfg.c */

/* check for condition: internal programming error */
# define ACP_ASSERT(cond) \
    if (! (cond)) { ACP_FATAL (0); } else acp_semicolon_missing = 1

#else

    /* no more programming error exists :-) */
# define ACP_ASSERT(cond) LSA_UNUSED_ARG(cond)

#endif

/*------------------------------------------------------------------------------
// ACP_STATIC_ASSERT
//  Does an ASSERT at compile time.
//  With Visual Studio 2010 you can use:   static_assert(expr_, msg_)
//  with C11 you can use:                  _Static_assert(expr_, msg_)
//  with cpp magic you can use:            { extern char _compile_time_assert[expr_]; }
//  else use runtime assert:               ACP_ASSERT(expr_)
//----------------------------------------------------------------------------*/

#ifndef ACP_STATIC_ASSERT
/* define ACP_STATIC_ASSERT(expr_, msg_)    static_assert(expr_, msg_) */
#  define ACP_STATIC_ASSERT(expr_, msg_)    ACP_ASSERT(expr_) /* use runtime assert */
#endif

/*------------------------------------------------------------------------------
//  Upper Layer
//----------------------------------------------------------------------------*/

#define ACP_ALLOC_UPPER_RQB(ptrptr, userid, length, sysptr)     \
{                                                               \
    LSAS_ALLOC_COMP_MEM(ptrptr, length, LSA_COMP_ID_ACP);   \
}                                                               \


#define ACP_FREE_UPPER_RQB(ret, ptr, sysptr)                    \
{                                                               \
    LSAS_FREE_COMP_MEM(ret, ptr, LSA_COMP_ID_ACP);          \
}                                                               \

#define ACP_ALLOC_UPPER_MEM(ptrptr, userid, length, sysptr)     \
{                                                               \
    LSAS_ALLOC_COMP_MEM(ptrptr, length, LSA_COMP_ID_ACP);   \
}                                                               \

#define ACP_FREE_UPPER_MEM(ret, ptr, sysptr)                    \
{                                                               \
    LSAS_FREE_COMP_MEM(ret, ptr, LSA_COMP_ID_ACP);          \
}                                                               \

/*------------------------------------------------------------------------------
//  Lower Layer
//----------------------------------------------------------------------------*/


#define ACP_ALLOC_LOWER_RQB(ptrptr, userid, length, sysptr)     \
{                                                               \
    LSAS_ALLOC_COMP_MEM((LSA_VOID **)ptrptr, length, LSA_COMP_ID_ACP);   \
}                                                               \

#define ACP_FREE_LOWER_RQB(ret, ptr, sysptr)                    \
{                                                               \
    LSAS_FREE_COMP_MEM(ret, ptr, LSA_COMP_ID_ACP);          \
}                                                               \

#define ACP_ALLOC_LOWER_MEM(ptrptr, userid, length, sysptr)     \
{                                                               \
    LSAS_ALLOC_COMP_MEM(ptrptr, length, LSA_COMP_ID_ACP);   \
}                                                               \

#define ACP_FREE_LOWER_MEM(ret, ptr, sysptr)                    \
{                                                               \
    LSAS_FREE_COMP_MEM(ret, ptr, LSA_COMP_ID_ACP);          \
}                                                               \

#ifdef PCIOX_CFG_USE_EDD

#define ACP_ALLOC_NRT_SEND_MEM(ptrptr, length, sysptr) EDD_ALLOC_DEV_SHARED_MEM(ptrptr, length, sysptr)
#define ACP_FREE_NRT_SEND_MEM(ret, ptr, sysptr)        EDD_FREE_DEV_SHARED_MEM(ret, ptr, sysptr)
#define ACP_ALLOC_NRT_RECV_MEM(ptrptr, length, sysptr) EDD_ALLOC_DEV_SHARED_MEM(ptrptr, length, sysptr)
#define ACP_FREE_NRT_RECV_MEM(ret, ptr, sysptr)        EDD_FREE_DEV_SHARED_MEM(ret, ptr, sysptr)

#else

#define ACP_ALLOC_NRT_SEND_MEM(ptrptr, length, sysptr)                       \
{                                                                            \
    LSAS_ALLOC_PACKET_MEM(ptrptr, length, LSA_COMP_ID_ACP);         \
}

#define ACP_FREE_NRT_SEND_MEM(ret, ptr, sysptr)                 \
{                                                               \
    *(ret) = LSA_RET_OK;                                        \
    LSAS_FREE_PACKET_MEM(ret, ptr, LSA_COMP_ID_ACP);          \
}                                                               \


#define ACP_ALLOC_NRT_RECV_MEM(ptrptr, length, sysptr)                      \
{                                                                           \
    LSAS_ALLOC_PACKET_MEM(ptrptr, length, LSA_COMP_ID_ACP);        \
}

#define ACP_FREE_NRT_RECV_MEM(ret, ptr, sysptr)                 \
{                                                               \
    *(ret) = LSA_RET_OK;                                        \
    LSAS_FREE_PACKET_MEM(ret, ptr, LSA_COMP_ID_ACP);          \
}                                                               \

#endif

/*------------------------------------------------------------------------------
//  System Layer
//----------------------------------------------------------------------------*/

#define ACP_ALLOC_LOCAL_MEM(ptrptr, length)                     \
{                                                               \
    LSAS_ALLOC_COMP_MEM(ptrptr, length, LSA_COMP_ID_ACP);   \
}                                                               \

#define ACP_FREE_LOCAL_MEM(ret, ptr)                            \
{                                                               \
    LSAS_FREE_COMP_MEM(ret, ptr, LSA_COMP_ID_ACP);          \
}                                                               \


#define ACP_MEMCPY(dst, src, len)                               \
{                                                               \
    LSAS_MEMCPY(dst, src, len);            \
}

#define ACP_GET_PATH_INFO(ret_val_ptr,sys_ptr_ptr,detail_ptr_ptr,path)                      \
{                                                                                           \
    LSA_GET_PATH_INFO(ret_val_ptr,sys_ptr_ptr,detail_ptr_ptr,path,LSAS_PATH_LAYER_ACP);  \
}                                                                                           \

#define ACP_RELEASE_PATH_INFO(ret_val_ptr,sys_ptr,detail_ptr)                               \
{                                                                                           \
    LSA_RELEASE_PATH_INFO(ret_val_ptr,sys_ptr,detail_ptr);                                  \
}                                                                                           \

/*------------------------------------------------------------------------------
// interface to BTRACE
//----------------------------------------------------------------------------*/

#ifndef ACP_FILE_SYSTEM_EXTENSION
# define ACP_FILE_SYSTEM_EXTENSION(module_id_)
#endif

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
//    0 .. no LSA trace
//    1 .. LSA trace [default]
//    2 .. LSA index trace
//----------------------------------------------------------------------------*/

#ifndef ACP_CFG_TRACE_MODE
# define ACP_CFG_TRACE_MODE  1
#endif

#if (ACP_CFG_TRACE_MODE == 1)
//Trace macros postponed due to the concept
#endif


/*----------------------------------------------------------------------------*/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif


#endif  /* of ACP_CFG_H */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
