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
/*  F i l e               &F: stcp_cfg.c                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Lower component interface                                                */
/*  Defines constants, types, macros and prototyping for NARE                */
/*                                                                           */
/*****************************************************************************/
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
/*                                                                           */
/*****************************************************************************/


#define LSAS_MODULE_ID  29


#include "lsas_inc.h"
#include "lsas_int.h"

#ifdef LSAS_CFG_USE_STCP


/*=============================================================================
 * function name:  STCP_ALLOC_UPPER_RQB
 *
 * function:       allocate an upper-RQB
 *
 * parameters:     L4_UPPER_RQB_PTR_TYPE  ...  *  upper_rqb_ptr_ptr:
 *                                   return value: pointer to upper-RQB-pointer
 *                                                 or LSA_NULL: The allocation
 *                                                 will be asynchronous, see
 *                                                 stcp_upper_rqb().
 *                 LSA_USER_ID_TYPE               user_id:  id of stcp
 *                 LSA_UINT16                     length:   length of RQB
 *                 LSA_SYS_PTR_TYPE               sys_ptr:  system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID  STCP_SYSTEM_OUT_FCT_ATTR  STCP_ALLOC_UPPER_RQB(
    L4_UPPER_RQB_PTR_TYPE  STCP_LOCAL_MEM_ATTR *  upper_rqb_ptr_ptr,
    LSA_USER_ID_TYPE                              user_id,
    LSA_UINT16                                    length,
    LSA_SYS_PTR_TYPE                              sys_ptr
)
{

    LSAS_ALLOC_COMP_MEM(upper_rqb_ptr_ptr, length, LSA_COMP_ID_STCP);

    LSA_UNUSED_ARG(user_id);
    LSA_UNUSED_ARG(sys_ptr);
}

/*=============================================================================
 * function name:  STCP_FREE_UPPER_RQB
 *
 * function:       free an upper-RQB
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      upper-RQB or
 *                                                      wrong system-pointer
 *                 L4_UPPER_RQB_PTR_TYPE  upper_rqb_ptr:  pointer to upper-
 *                                                        RQB
 *                 LSA_SYS_PTR_TYPE           sys_ptr:    system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID  STCP_SYSTEM_OUT_FCT_ATTR  STCP_FREE_UPPER_RQB(
    LSA_UINT16                 STCP_LOCAL_MEM_ATTR * ret_val_ptr,
    L4_UPPER_RQB_PTR_TYPE                            upper_rqb_ptr,
    LSA_SYS_PTR_TYPE                                 sys_ptr
)
{

    LSAS_FREE_COMP_MEM(ret_val_ptr, upper_rqb_ptr, LSA_COMP_ID_STCP);

    LSA_UNUSED_ARG(sys_ptr);
}


/*=============================================================================
 * function name:  STCP_ALLOC_UPPER_MEM
 *
 * function:       allocate an upper-memory
 *
 * parameters:     L4_UPPER_MEM_PTR_TYPE  ...  *  upper_mem_ptr_ptr:
 *                                   return value: pointer to upper-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: The allocation
 *                                                 will be asynchronous, see
 *                                                 stcp_upper_mem().
 *                 LSA_USER_ID_TYPE                user_id:  id of stcp
 *                 LSA_UINT16                      length:   length of
 *                                                           memory
 *                 LSA_SYS_PTR_TYPE                sys_ptr:  system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  STCP_SYSTEM_OUT_FCT_ATTR  STCP_ALLOC_UPPER_MEM(
    L4_UPPER_MEM_PTR_TYPE  STCP_LOCAL_MEM_ATTR *  upper_mem_ptr_ptr,
    LSA_USER_ID_TYPE                              user_id,
    LSA_UINT16                                    length,
    LSA_SYS_PTR_TYPE                              sys_ptr
)
{

    LSAS_ALLOC_COMP_MEM(upper_mem_ptr_ptr, length, LSA_COMP_ID_STCP);

    LSA_UNUSED_ARG(user_id);
    LSA_UNUSED_ARG(sys_ptr);
}

/*=============================================================================
 * function name:  STCP_FREE_UPPER_MEM
 *
 * function:       free an upper-memory
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      upper-memory or
 *                                                      wrong system-pointer
 *                 L4_UPPER_MEM_PTR_TYPE  upper_mem_ptr: pointer to upper-
 *                                                       memory
 *                 LSA_SYS_PTR_TYPE           sys_ptr:   system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  STCP_SYSTEM_OUT_FCT_ATTR  STCP_FREE_UPPER_MEM(
    LSA_UINT16                 STCP_LOCAL_MEM_ATTR * ret_val_ptr,
    L4_UPPER_MEM_PTR_TYPE                            upper_mem_ptr,
    LSA_SYS_PTR_TYPE                                 sys_ptr
)
{

    LSAS_FREE_COMP_MEM(ret_val_ptr, upper_mem_ptr, LSA_COMP_ID_STCP);

    LSA_UNUSED_ARG(sys_ptr);
}


/*=============================================================================
 * function name:       STCP_CHECK_FOR_LOCAL
 * Function:            Query whether the TSAP routing is to be performed directly
 *
 * parameters:     LSA_BOOL  ...  *  ret_val_ptr:
 *                     Returns:  STCP_FALSE  Directly to the TSAP routing:
 *                                           STCP searches for an RQB with
 *                                           opcode L4_OPC_AWAIT_CONNECT_TG.
 *                                           If no RQB exists the connection
 *                                           setup is rejected.
 *                               STCP_TRUE   STCP searches for an RQB with opcode
 *                                           L4_OPC_AWAIT_CONNECT_NU,
 *                                           L4_OPC_AWAIT_CONNECT_NT,
 *                                           L4_OPC_AWAIT_CONNECT_U or
 *                                           L4_OPC_AWAIT_CONNECT_T.
 *                                           STCP uses the RQB which has the most
 *                                           appropriate TA buffer.
 *                                           If no (appropriate) RQB exists the
 *                                           connection setup is rejected.
 *                 STCP_ROUTE_INFO_PTR_TYPE  route_info
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID STCP_UPPER_OUT_FCT_ATTR STCP_CHECK_FOR_LOCAL(
    LSA_BOOL        STCP_LOCAL_MEM_ATTR * ret_val_ptr,
    STCP_ROUTE_INFO_PTR_TYPE              route_info
)
{
    *ret_val_ptr = TRUE;
}

/*****************************************************************************/

/*=============================================================================
 * function name:  STCP_GET_PATH_INFO
 *
 * function:       get system-pointer and detail-pointer of a communication
 *                 channel
 *                  The two pointers are directly
 *                  written to the local data area of the STCP.
 *
 * parameters:     LSA_UINT16              ...  *  ret_val_ptr:
 *                                      return value: LSA_RET_OK
 *                                                    LSA_RET_ERR_SYS_PATH
 *                 LSA_SYS_PTR_TYPE        ...  *  sys_ptr_ptr:
 *                                      return value: pointer to system-pointer
 *                 STCP_DETAIL_PTR_TYPE  ...  *  detail_ptr_ptr:
 *                                      return value: pointer to detail-pointer
 *                 LSA_SYS_PATH_TYPE               path: path of a
 *                                                       communication channel
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  STCP_SYSTEM_OUT_FCT_ATTR  STCP_GET_PATH_INFO(
    LSA_UINT16              STCP_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_SYS_PTR_TYPE        STCP_LOCAL_MEM_ATTR *  sys_ptr_ptr,
    STCP_DETAIL_PTR_TYPE  STCP_LOCAL_MEM_ATTR   *  detail_ptr_ptr,
    LSA_SYS_PATH_TYPE                              path
)
{
    LSA_GET_PATH_INFO(ret_val_ptr, sys_ptr_ptr, detail_ptr_ptr,path, LSAS_PATH_LAYER_STCP);

}

/*****************************************************************************/
/*=============================================================================
 * function name:  STCP_RELEASE_PATH_INFO
 *
 * function:       release system-pointer and detail-pointer of a communication
 *                 channel
 *
 * parameters:     LSA_UINT16      ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no release because of
 *                                                      wrong system-pointer or
 *                                                      wrong detail-pointer
 *                 LSA_SYS_PTR_TYPE        sys_ptr:     system-pointer
 *                 STCP_DETAIL_PTR_TYPE    detail_ptr:  detail-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID STCP_SYSTEM_OUT_FCT_ATTR STCP_RELEASE_PATH_INFO(
    LSA_UINT16              STCP_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_SYS_PTR_TYPE        sys_ptr,
    STCP_DETAIL_PTR_TYPE    detail_ptr
)
{
    LSA_RELEASE_PATH_INFO(ret_val_ptr, sys_ptr, detail_ptr);
}


#ifdef STCP_LOCAL_MEM_DYNAMIC
/*=============================================================================
 * function name:  STCP_ALLOC_LOCAL_MEM
 *
 * function:       allocate a local memory
 *
 * parameters:     STCP_LOCAL_MEM_PTR_TYPE  ...  *  local_mem_ptr_ptr:
 *                                   return value: pointer to local-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: No memory
 *                                                 available
 *                 LSA_UINT16                      length: length of memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  STCP_SYSTEM_OUT_FCT_ATTR  STCP_ALLOC_LOCAL_MEM(
    LSA_VOID STCP_LOCAL_MEM_PTR_TYPE  STCP_LOCAL_MEM_ATTR *  local_mem_ptr_ptr,
    LSA_UINT16                                          length
)
{
    LSAS_ALLOC_COMP_MEM(local_mem_ptr_ptr, length, LSA_COMP_ID_STCP);
}



/*=============================================================================
 * function name:  STCP_FREE_LOCAL_MEM
 *
 * function:       free a local-memory
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      local-memory
 *                 STCP_LOCAL_MEM_PTR_TYPE  local_mem_ptr:  pointer to local-
 *                                                            memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID STCP_SYSTEM_OUT_FCT_ATTR STCP_FREE_LOCAL_MEM(
    LSA_UINT16                 STCP_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_VOID STCP_LOCAL_MEM_PTR_TYPE                  local_mem_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, local_mem_ptr, LSA_COMP_ID_STCP);
}

#endif  /* STCP_LOCAL_MEM_DYNAMIC */

#ifdef NOT_USED_AT_THE_MOMENT
/*=============================================================================
 * function name:  STCP_LOWER_TO_UPPER_MEM
 *
 * function:       convert lower-memory to upper-memory
 *
 * parameters:     STCP_UPPER_MEM_PTR_TYPE  ...  * upper_mem_ptr_ptr:
 *                                   return value: pointer to upper-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: system couldn't
 *                                                 convert memory; STCP
 *                                                 should allocate upper-memory
 *                                                 and copy lower- to upper-
 *                                                 memory
 *                 STCP_LOWER_MEM_PTR_TYPE         lower_mem_ptr:
 *                                                 pointer to lower-memory
 *                 LSA_SYS_PTR_TYPE                sys_ptr:  system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef STCP_LOWER_TO_UPPER_MEM
LSA_VOID  STCP_SYSTEM_OUT_FCT_ATTR  STCP_LOWER_TO_UPPER_MEM(
    STCP_UPPER_MEM_PTR_TYPE  STCP_LOCAL_MEM_ATTR *  upper_mem_ptr_ptr,
    STCP_LOWER_MEM_PTR_TYPE                         lower_mem_ptr,
    LSA_SYS_PTR_TYPE                                sys_ptr
);
#endif

/*=============================================================================
 * function name:  STCP_UPPER_TO_LOWER_MEM
 *
 * function:       convert upper-memory to lower-memory
 *
 * parameters:     STCP_LOWER_MEM_PTR_TYPE  ...  * lower_mem_ptr_ptr:
 *                                   return value: pointer to lower-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: system couldn't
 *                                                 convert memory; STCP
 *                                                 should allocate lower-memory
 *                                                 and copy upper- to lower-
 *                                                 memory
 *                 STCP_UPPER_MEM_PTR_TYPE         upper_mem_ptr:
 *                                                 pointer to upper-memory
 *                 LSA_SYS_PTR_TYPE                sys_ptr:  system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef STCP_UPPER_TO_LOWER_MEM
LSA_VOID  STCP_SYSTEM_OUT_FCT_ATTR  STCP_UPPER_TO_LOWER_MEM(
    STCP_LOWER_MEM_PTR_TYPE  STCP_LOCAL_MEM_ATTR *  lower_mem_ptr_ptr,
    STCP_UPPER_MEM_PTR_TYPE                         upper_mem_ptr,
    LSA_SYS_PTR_TYPE                                sys_ptr
);
#endif

#endif /*NOT_USED_AT_THE_MOMENT*/
/*****************************************************************************/

/*=============================================================================
 * function name:  STCP_ALLOC_TIMER
 *
 * function:       allocate a timer
 *
 * parameters:     LSA_UINT16  ...  *  ret_val_ptr:
 *                           return value: LSA_RET_OK            timer has been
 *                                                               allocated
 *                                         LSA_RET_ERR_NO_TIMER  no timer has
 *                                                               been allocated
 *                 LSA_TIMER_ID_TYPE ...  *  timer_id_ptr:
 *                           return value: pointer to id of timer
 *                 LSA_UINT16          timer_type:  LSA_TIMER_TYPE_ONE_SHOT or
 *                                                  LSA_TIMER_TYPE_CYCLIC
 *                 LSA_UINT16          time_base:   LSA_TIME_BASE_1MS,
 *                                                  LSA_TIME_BASE_10MS,
 *                                                  LSA_TIME_BASE_100MS,
 *                                                  LSA_TIME_BASE_1S,
 *                                                  LSA_TIME_BASE_10S or
 *                                                  LSA_TIME_BASE_100S
 *                 LSA_SYS_PTR_TYPE    sys_ptr:     system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID  STCP_SYSTEM_OUT_FCT_ATTR  STCP_ALLOC_TIMER(
    LSA_UINT16  STCP_LOCAL_MEM_ATTR *           ret_val_ptr,
    LSA_TIMER_ID_TYPE  STCP_LOCAL_MEM_ATTR *    timer_id_ptr,
    LSA_UINT16                                  timer_type,
    LSA_UINT16                                  time_base,
    LSA_SYS_PTR_TYPE                            sys_ptr
)
{
    LSAS_ALLOC_TIMER(ret_val_ptr, timer_id_ptr, timer_type, time_base, lsas_stcp_timeout_wrap, LSA_COMP_ID_STCP);
}

/*****************************************************************************/
/*=============================================================================
 * function name:  STCP_FREE_TIMER
 *
 * function:       free a timer
 *
 * parameters:     LSA_UINT16  ...  *  ret_val_ptr:
 *                return value: LSA_RET_OK                    timer has been
 *                                                            deallocated
 *                              LSA_RET_ERR_TIMER_IS_RUNNING  because timer is
 *                                                            running timer has
 *                                                            not been
 *                                                            deallocated
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                            because of wrong
 *                                                            timer-id
 *                 LSA_TIMER_ID_TYPE   timer_id:  id of timer
 *                 LSA_SYS_PTR_TYPE    sys_ptr:   system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID STCP_SYSTEM_OUT_FCT_ATTR STCP_FREE_TIMER(
    LSA_UINT16  STCP_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_TIMER_ID_TYPE                  timer_id,
    LSA_SYS_PTR_TYPE                   sys_ptr
)
{
    LSAS_FREE_TIMER(ret_val_ptr, timer_id, LSA_COMP_ID_STCP);
}

/*****************************************************************************/
/*=============================================================================
 * function name:  STCP_START_TIMER
 *
 * function:       start a timer
 *
 * parameters:     LSA_UINT16  ...  *  ret_val_ptr:
 *                   return value: LSA_RET_OK                  timer has been
 *                                                             started
 *                                 LSA_RET_OK_TIMER_RESTARTED  timer has been
 *                                                             restarted
 *                                 LSA_RET_ERR_PARAM           timer hasn't
 *                                                             been started
 *                                                             because of wrong
 *                                                             timer-id
 *                                 After the expiration of the running time
 *                                 system will call prefix_timeout().
 *                 LSA_TIMER_ID_TYPE   timer_id:  id of timer
 *                 LSA_USER_ID_TYPE    user_id:   id of prefix
 *                 LSA_UINT16          time:      running time
 *                 LSA_SYS_PTR_TYPE    sys_ptr:   system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  STCP_SYSTEM_OUT_FCT_ATTR  STCP_START_TIMER(
    LSA_UINT16        STCP_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_TIMER_ID_TYPE                        timer_id,
    LSA_USER_ID_TYPE                         user_id,
    LSA_UINT16                               time,
    LSA_SYS_PTR_TYPE                         sys_ptr
)
{
    LSA_START_TIMER(ret_val_ptr, timer_id, user_id, time, LSA_COMP_ID_STCP);
}


/*****************************************************************************/
/*=============================================================================
 * function name:  STCP_STOP_TIMER
 *
 * function:       stop a timer
 *
 * parameters:     LSA_UINT16  ...  *  ret_val_ptr:
 *                           return value: LSA_RET_OK            timer has been
 *                                                               stopped
 *                                 LSA_RET_OK_TIMER_NOT_RUNNING  timer was not
 *                                                               running; timer
 *                                                               is stopped
 *                                 LSA_RET_ERR_PARAM             timer hasn't
 *                                                               stopped
 *                                                               because
 *                                                               of wrong
 *                                                               timer-id
 *                 LSA_TIMER_ID_TYPE   timer_id:  id of timer
 *                 LSA_SYS_PTR_TYPE    sys_ptr:   system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  STCP_SYSTEM_OUT_FCT_ATTR  STCP_STOP_TIMER(
    LSA_UINT16  STCP_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_TIMER_ID_TYPE                  timer_id,
    LSA_SYS_PTR_TYPE                   sys_ptr
)
{
    LSA_STOP_TIMER(ret_val_ptr, timer_id, LSA_COMP_ID_STCP);
}



/*=============================================================================
 * function name:  STCP_ENTER
 *
 * function:       set reentrance lock
 *                 start of a non interruptable part
 *
 * parameters:     LSA_VOID
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID STCP_SYSTEM_OUT_FCT_ATTR STCP_ENTER()
{
    LSAS_STCP_ENTER();
}

/*****************************************************************************/
/*=============================================================================
 * function name:  STCP_EXIT
 *
 * function:       cancel reentrance lock
 *                 end of a non interruptable part.
 *
 * parameters:     LSA_VOID
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID STCP_SYSTEM_OUT_FCT_ATTR STCP_EXIT()
{
    LSAS_STCP_EXIT();
}

/*****************************************************************************/
/*=============================================================================
 * function name:  STCP_FATAL_ERROR
 *
 * function:       notify a fatal-error
 *
 * parameters:     LSA_UINT16                   length:     number of bytes
 *                 STCP_FATAL_ERROR_PTR_TYPE  error_ptr:  pointer to
 *                                                          LSA_FATAL_ERROR
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef STCP_FATAL_ERROR
LSA_VOID STCP_SYSTEM_OUT_FCT_ATTR STCP_FATAL_ERROR(
    LSA_UINT16                   length,
    STCP_FATAL_ERROR_PTR_TYPE  error_ptr
)
{
    #error "by design a macro"
}
#endif

/****************************************************************************/
/*=============================================================================
 * function name:  STCP_HTON_16
 *
 * function:      converts a 16-bit input from host to TCP/IP network byte ordering; i.e. swap it
 *
 * parameters:     LSA_UINT16   para    16-bit number in host byte order
 *
 * return value:   value in TCP/IP network byte order
 *===========================================================================*/
LSA_UINT16 STCP_SYSTEM_OUT_FCT_ATTR STCP_HTON_16(
        LSA_UINT16 para
)
{
    return (LSA_HTONS(para));
}

/****************************************************************************/
/*=============================================================================
 * function name:  STCP_NTOH_16
 *
 * function:      converts a 16-bit input from TCP/IP network to host byte ordering; i.e. swap it
 *
 * parameters:     LSA_UINT16   para    16-bit number in TCP/IP network byte ordering
 *
 * return value:   value in host byte order
 *===========================================================================*/
LSA_UINT16 STCP_SYSTEM_OUT_FCT_ATTR STCP_NTOH_16(
        LSA_UINT16 para
)
{
    return (LSA_NTOHS(para));
}


/*=============================================================================
 * function name:  STCP_OPEN_CHANNEL_LOWER
 *
 * function:       open a communication channel to lower layer
 *
 * parameters:     STCP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:  pointer to lower-
 *                                                            RQB
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *
 * RQB parameters:
 *     RQB-header:
 *     LSA_OPCODE_TYPE    opcode:        *_OPC_OPEN_CHANNEL
 *     LSA_HANDLE_TYPE    handle:        channel-handle of stcp
 *     LSA_USER_ID_TYPE   user-id:       id of stcp
 *     RQB-args:
 *     LSA_HANDLE_TYPE    handle_upper:  channel-handle of stcp
 *     LSA_SYS_PATH_TYPE  sys_path:      system-path of channel
 *     LSA_VOID  LSA_FCT_PTR(*_UPPER_OUT_FCT_ATTR, stcp_request_lower_done_ptr)
 *                                    (STCP_UPPER_RQB_PTR_TYPE lower_rqb_ptr)
 *                                       pointer to callback-function
 *
 * RQB-return values via callback-function:
 *      LSA_UINT16        response:      *_RSP_OK
 *                                       *_RSP_ERR_RESOURCE
 *                                       *_RSP_ERR_SYS_PATH
 *                                       possibly:
 *                                       *_RSP_ERR_PARAM
 *                                       *_RSP_ERR_LOWER_LAYER
 *                                       or others
 *     RQB-args:
 *     LSA_HANDLE_TYPE    handle:        channel-handle of lower layer
 *     All other RQB-parameters are unchanged.
 *===========================================================================*/

LSA_VOID  STCP_LOWER_OUT_FCT_ATTR  STCP_OPEN_CHANNEL_LOWER(
    STCP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE         sys_ptr
)
{
    LSA_UNUSED_ARG( sys_ptr );

    lower_rqb_ptr->args.channel.sock_request_upper_done_ptr = lsas_sock_to_stcp_cbf;

    LSAS_RQB_SOCK_REQUEST(lower_rqb_ptr, sock_open_channel);    
}

/*=============================================================================
 * function name:  STCP_CLOSE_CHANNEL_LOWER
 *
 * function:       close a communication channel to lower layer
 *
 * parameters:     STCP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:  pointer to lower-
 *                                                            RQB
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *
 * RQB parameters:
 *     RQB-header:
 *     LSA_OPCODE_TYPE   opcode:   *_OPC_CLOSE_CHANNEL
 *     LSA_HANDLE_TYPE   handle:   channel-handle of lower layer
 *     LSA_USER_ID_TYPE  user-id:  id of stcp
 *
 * RQB-return values via callback-function:
 *     RQB-header:
 *     LSA_HANDLE_TYPE   handle:    channel-handle of stcp
 *     RQB-args:         response:  *_RSP_OK
 *                                  or others
 *     All other RQB-parameters are unchanged.
 *===========================================================================*/
LSA_VOID  STCP_LOWER_OUT_FCT_ATTR  STCP_CLOSE_CHANNEL_LOWER(
    STCP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE         sys_ptr
)
{
    LSA_UNUSED_ARG( sys_ptr );

    LSAS_RQB_SOCK_REQUEST(lower_rqb_ptr, sock_close_channel);

}


/*=============================================================================
 * function name:  STCP_REQUEST_LOWER
 *
 * function:       send a request to a communication channel to lower layer
 *
 * parameters:     STCP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:  pointer to lower-
 *                                                            RQB
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *
 * RQB parameters:
 *     RQB-header:
 *     LSA_OPCODE_TYPE   opcode:    *_OPC_*
 *     LSA_HANDLE_TYPE   handle:    channel-handle of lower layer
 *     LSA_USER_ID_TYPE  user-id:   id of stcp
 *     RQB-args:                    Depend on kind of request.
 *
 * RQB-return values via callback-function:
 *     RQB-header:
 *     LSA_OPCODE_TYPE   opcode:    *_OPC_*
 *     LSA_HANDLE_TYPE   handle:    channel-handle of stcp
 *     LSA_USER_ID_TYPE  user-id:   id of stcp
 *     RQB-args:         response:  *_RSP_OK
 *                                  *_RSP_ERR_REF
 *                                  *_RSP_ERR_SEQUENCE
 *                                  possibly:
 *                                  *_RSP_OK_EOM
 *                                  *_RSP_ERR_RESOURCE
 *                                  *_RSP_ERR_PARAM
 *                                  *_RSP_ERR_LOC_ABORT
 *                                  *_RSP_ERR_REM_ABORT
 *                                  *_RSP_ERR_LOC_NET
 *                                  *_RSP_ERR_REM_NET
 *                                  *_RSP_ERR_PROTOCOL
 *                                  *_RSP_ERR_TIMEOUT
 *                                  *_RSP_ERR_DEV
 *                                  *_RSP_ERR_ROUTE
 *                                  *_RSP_ERR_OWN_ADDR
 *                                  *_RSP_ERR_LOWER_LAYER
 *                                  *_RSP_ERR_WITHDRAW
 *                                  or others
 *     All other RQB-parameters depend on kind of request.
 *===========================================================================*/
LSA_VOID  STCP_LOWER_OUT_FCT_ATTR  STCP_REQUEST_LOWER(
    STCP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE         sys_ptr
)
{
    LSA_UNUSED_ARG(  sys_ptr );

    LSAS_RQB_SOCK_REQUEST(lower_rqb_ptr, sock_request);
}

/*=============================================================================
 * function name:  STCP_ALLOC_LOWER_RQB
 *
 * function:       allocate a lower-RQB
 *
 * parameters:     STCP_LOWER_RQB_PTR_TYPE  ...  *  lower_rqb_ptr_ptr:
 *                                   return value: pointer to lower-RQB-pointer
 *                                                 or LSA_NULL: The allocation
 *                                                 will be asynchronous, see
 *                                                 stcp_lower_rqb().
 *                 LSA_USER_ID_TYPE                   user_id:  id of stcp
 *                 LSA_UINT16                         length:   length of RQB
 *                 LSA_SYS_PTR_TYPE                   sys_ptr:  system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  STCP_SYSTEM_OUT_FCT_ATTR  STCP_ALLOC_LOWER_RQB(
    STCP_LOWER_RQB_PTR_TYPE  STCP_LOCAL_MEM_ATTR *  lower_rqb_ptr_ptr,
    LSA_USER_ID_TYPE                                    user_id,
    LSA_UINT16                                          length,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(lower_rqb_ptr_ptr, length, LSA_COMP_ID_STCP);

    LSA_UNUSED_ARG( user_id );
    LSA_UNUSED_ARG( sys_ptr );
}


/*=============================================================================
 * function name:  STCP_FREE_LOWER_RQB
 *
 * function:       free a lower-RQB
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      lower-RQB or
 *                                                      wrong system-pointer
 *                 STCP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:  pointer to lower-
 *                                                            RQB
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  STCP_SYSTEM_OUT_FCT_ATTR  STCP_FREE_LOWER_RQB(
    LSA_UINT16                 STCP_LOCAL_MEM_ATTR *  ret_val_ptr,
    STCP_LOWER_RQB_PTR_TYPE                           lower_rqb_ptr,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_rqb_ptr, LSA_COMP_ID_STCP);

    LSA_UNUSED_ARG( sys_ptr );
}

/*=============================================================================
 * function name:  STCP_ALLOC_LOWER_MEM
 *
 * function:       allocate a lower-memory
 *
 * parameters:     STCP_LOWER_MEM_PTR_TYPE  ...  *  lower_mem_ptr_ptr:
 *                                   return value: pointer to lower-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: The allocation
 *                                                 will be asynchronous, see
 *                                                 stcp_lower_mem().
 *                 LSA_USER_ID_TYPE                   user_id:  id of stcp
 *                 LSA_UINT16                         length:   length of
 *                                                              memory
 *                 LSA_SYS_PTR_TYPE                   sys_ptr:  system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  STCP_SYSTEM_OUT_FCT_ATTR  STCP_ALLOC_LOWER_MEM(
    STCP_LOWER_MEM_PTR_TYPE  STCP_LOCAL_MEM_ATTR *  lower_mem_ptr_ptr,
    LSA_USER_ID_TYPE                                user_id,
    LSA_UINT16                                      length,
    LSA_SYS_PTR_TYPE                                sys_ptr
)
{
    LSAS_ALLOC_PACKET_MEM(lower_mem_ptr_ptr,length, LSA_COMP_ID_STCP);

    LSA_UNUSED_ARG( user_id );
    LSA_UNUSED_ARG( sys_ptr );
}

/*=============================================================================
 * function name:  STCP_FREE_LOWER_MEM
 *
 * function:       free a lower-memory
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      lower-memory or
 *                                                      wrong system-pointer
 *                 STCP_LOWER_MEM_PTR_TYPE  lower_mem_ptr:  pointer to lower-
 *                                                            memory
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  STCP_SYSTEM_OUT_FCT_ATTR  STCP_FREE_LOWER_MEM(
    LSA_UINT16                 STCP_LOCAL_MEM_ATTR *  ret_val_ptr,
    STCP_LOWER_MEM_PTR_TYPE                           lower_mem_ptr,
    LSA_SYS_PTR_TYPE                                  sys_ptr
)
{
    LSAS_FREE_PACKET_MEM(ret_val_ptr, lower_mem_ptr, LSA_COMP_ID_NARE);
    LSA_UNUSED_ARG( sys_ptr );
}



/****************************************************************************/
/*  not used:
#ifndef STCP_LOW_RQB_GET_SIZE
STCP_UWORD STCP_LOWER_OUT_FCT_ATTR STCP_LOW_RQB_GET_SIZE(
    STCP_LOWER_RQB_PTR low_rqb_ptr
);*/
/* Function: get the length of the request block
 *           returns either the absolute length (depending on the opcode) or
 *           the maximum in the system
 *
 * Returns:  STCP_UWORD  length of the request block
 */
/*
#endif
*/
/****************************************************************************/

#ifndef STCP_LOW_RQB_SET_NEXT
LSA_VOID STCP_LOWER_OUT_FCT_ATTR STCP_LOW_RQB_SET_NEXT(
    STCP_LOWER_RQB_PTR_TYPE low_rqb_ptr,
    STCP_LOWER_RQB_PTR_TYPE next_rqb_ptr
);
/* Function: write the link pointer  to a lower layer RQB.
 *
 * Returns:  LSA_VOID
 */
#endif
/****************************************************************************/

#ifndef STCP_LOW_RQB_GET_NEXT
STCP_LOWER_RQB_PTR_TYPE STCP_LOWER_OUT_FCT_ATTR STCP_LOW_RQB_GET_NEXT(
    STCP_LOWER_RQB_PTR_TYPE low_rqb_ptr
);
/* Function: read the link pointer from a lower layer RQB.
 *
 * Returns:  Content of the link pointer
 */
#endif
/****************************************************************************/

#ifndef STCP_LOW_RQB_SET_OPCODE
LSA_VOID STCP_LOWER_OUT_FCT_ATTR STCP_LOW_RQB_SET_OPCODE(
    STCP_LOWER_RQB_PTR_TYPE     low_rqb_ptr,
    LSA_OPCODE_TYPE              opcode
);
/* Function: write the opcode to a lower layer RQB.
 *
 * Returns:  LSA_VOID
 */
#endif
/****************************************************************************/

#ifndef STCP_LOW_RQB_GET_OPCODE
LSA_OPCODE_TYPE STCP_LOWER_OUT_FCT_ATTR STCP_LOW_RQB_GET_OPCODE(
    STCP_LOWER_RQB_PTR_TYPE low_rqb_ptr
);
/* Function: read the opcode from a lower layer RQB.
 *
 * Returns:  Content of opcode
 */
#endif
/****************************************************************************/


#ifndef STCP_LOW_RQB_SET_HANDLE
LSA_VOID STCP_LOWER_OUT_FCT_ATTR STCP_LOW_RQB_SET_HANDLE(
    STCP_LOWER_RQB_PTR_TYPE     low_rqb_ptr,
    LSA_HANDLE_TYPE              handle
);
/* Function: write the handle to a lower layer RQB.
 *
 * Returns:  LSA_VOID
 */
#endif
/****************************************************************************/

#ifndef STCP_LOW_RQB_GET_HANDLE
LSA_HANDLE_TYPE STCP_LOWER_OUT_FCT_ATTR STCP_LOW_RQB_GET_HANDLE(
    STCP_LOWER_RQB_PTR_TYPE low_rqb_ptr
);
/* Function: read the handle from a lower layer RQB.
 *
 * Returns:  Content of the handle
 */
#endif
/****************************************************************************/

#ifndef STCP_LOW_RQB_SET_USER_ID
LSA_VOID STCP_LOWER_OUT_FCT_ATTR STCP_LOW_RQB_SET_USER_ID(
    STCP_LOWER_RQB_PTR_TYPE     low_rqb_ptr,
    LSA_USER_ID_TYPE              user_id
);
/* Function: write the user_id to a lower layer RQB.
 *
 * Returns:  LSA_VOID
 */
#endif
/****************************************************************************/

#ifndef STCP_LOW_RQB_GET_USER_ID
LSA_USER_ID_TYPE STCP_LOWER_OUT_FCT_ATTR STCP_LOW_RQB_GET_USER_ID(
    STCP_LOWER_RQB_PTR_TYPE low_rqb_ptr
);
/* Function: read the user_id from a lower layer RQB.
 *
 * Returns:  Content of the user_id
 */
#endif
/****************************************************************************/


#ifndef STCP_LOW_RQB_SET_RESPONSE
LSA_VOID STCP_LOWER_OUT_FCT_ATTR STCP_LOW_RQB_SET_RESPONSE(
    STCP_LOWER_RQB_PTR_TYPE     low_rqb_ptr,
    LSA_UINT16              opcode
);
/* Function: write the response to a lower layer RQB.
 *
 * Returns:  LSA_VOID
 */
#endif
/****************************************************************************/

#ifndef STCP_LOW_RQB_GET_RESPONSE
LSA_UINT16 STCP_LOWER_OUT_FCT_ATTR STCP_LOW_RQB_GET_RESPONSE(
    STCP_LOWER_RQB_PTR_TYPE low_rqb_ptr
);
/* Function: read the response from a lower layer RQB.
 *
 * Returns:  Content of the response
 */
#endif
/****************************************************************************/

#ifndef STCP_LOW_RQB_USER_ID_INIT
LSA_VOID STCP_LOWER_OUT_FCT_ATTR STCP_LOW_RQB_USER_ID_INIT(
    STCP_LOWER_RQB_PTR_TYPE low_rqb_ptr
);
/* Function: initialize the user_id to an unused value
 *
 * Returns:  LSA_VOID
 */
#endif
/****************************************************************************/

#ifndef STCP_LOW_RQB_SET_USER_ID_CN_ID
LSA_VOID STCP_LOWER_OUT_FCT_ATTR STCP_LOW_RQB_SET_USER_ID_CN_ID(
    STCP_LOWER_RQB_PTR_TYPE low_rqb_ptr,
        LSA_USER_ID_TYPE                user_id
);
/* Function: write the cn_id into the user_id to a lower layer RQB.
 *
 * Returns:  LSA_VOID
 */
#endif
/****************************************************************************/

#ifndef STCP_LOW_RQB_GET_USER_ID_CN_ID
L4_CN_ID_TYPE STCP_LOWER_OUT_FCT_ATTR STCP_LOW_RQB_GET_USER_ID_CN_ID(
    STCP_LOWER_RQB_PTR_TYPE low_rqb_ptr
);
/* Function: read the cn_id from the user_id from a lower layer RQB
 *
 * Returns:  cn_id
 */
#endif
/****************************************************************************/

#ifndef STCP_LOW_RQB_SET_USER_ID_DEVICE_ID
LSA_VOID STCP_LOWER_OUT_FCT_ATTR STCP_LOW_RQB_SET_USER_ID_DEVICE_ID(
    STCP_LOWER_RQB_PTR_TYPE low_rqb_ptr,
        L4_CN_ID_TYPE           device_id
);
/* Function: write the device_id into the user_id to a lower layer RQB.
 *
 * Returns:  LSA_VOID
 */
#endif
/****************************************************************************/

#ifndef STCP_LOW_RQB_GET_USER_ID_DEVICE_ID
L4_CN_ID_TYPE STCP_LOWER_OUT_FCT_ATTR STCP_LOW_RQB_GET_USER_ID_DEVICE_ID(
    STCP_LOWER_RQB_PTR_TYPE low_rqb_ptr
);
/* Function: read the device_id from the user_id from a lower layer RQB.
 *
 * Returns:  device_id
 */
#endif
/****************************************************************************/

/*--------------------------*/
/* Open channel             */
/*--------------------------*/

#ifndef STCP_LOW_RQB_SET_CHA_OPEN_HANDLE_UPPER
LSA_VOID STCP_LOWER_OUT_FCT_ATTR STCP_LOW_RQB_SET_CHA_OPEN_HANDLE_UPPER(
    STCP_LOWER_RQB_PTR_TYPE low_rqb_ptr,
        LSA_HANDLE_TYPE         handle_upper
);
/* Function: write the handle_upper to a lower layer RQB.
 *
 * Returns:  LSA_VOID
 */
#endif
/****************************************************************************/

#ifndef STCP_LOW_RQB_GET_CHA_OPEN_HANDLE_UPPER
LSA_HANDLE_TYPE STCP_LOWER_OUT_FCT_ATTR STCP_LOW_RQB_GET_CHA_OPEN_HANDLE_UPPER(
    STCP_LOWER_RQB_PTR_TYPE low_rqb_ptr
);
/* Function: read the handle_upper from a lower layer RQB.
 *
 * Returns:  handle_upper
 */
#endif
/****************************************************************************/

#ifndef STCP_LOW_RQB_SET_CHA_OPEN_PATH
LSA_VOID STCP_LOWER_OUT_FCT_ATTR STCP_LOW_RQB_SET_CHA_OPEN_PATH(
    STCP_LOWER_RQB_PTR_TYPE low_rqb_ptr,
        LSA_SYS_PATH_TYPE               sys_path
);
/* Function: write the sys_path to a lower layer RQB.
 *
 * Returns:  LSA_VOID
 */
#endif
/****************************************************************************/

#ifndef STCP_LOW_RQB_GET_CHA_OPEN_CBF
LSA_VOID LSA_FCT_PTR(STCP_LOWER_IN_FCT_ATTR, STCP_LOW_RQB_GET_CHA_OPEN_CBF(
    STCP_LOWER_RQB_PTR_TYPE low_rqb_ptr
))(STCP_LOWER_RQB_PTR_TYPE);
/* Function: read the callback function from a lower layer RQB.
 *
 * Returns:  Pointer to the CBF
 */
#endif
/****************************************************************************/

#ifndef STCP_LOW_RQB_SET_CHA_OPEN_CBF

LSA_VOID STCP_LOWER_OUT_FCT_ATTR STCP_LOW_RQB_SET_CHA_OPEN_CBF(
    STCP_LOWER_RQB_PTR_TYPE low_rqb_ptr,
    LSA_VOID LSA_FCT_PTR (STCP_LOWER_IN_FCT_ATTR, sock_request_upper_done_ptr) (STCP_LOWER_RQB_PTR_TYPE low_rqb_ptr)
);
/* Function: write the callback function to a lower layer RQB.
 *
 * Returns:  LSA_VOID
 */
#endif
/****************************************************************************/

#ifndef STCP_LOW_RQB_SET_CHA_OPEN_HANDLE
LSA_VOID STCP_LOWER_OUT_FCT_ATTR STCP_LOW_RQB_SET_CHA_OPEN_HANDLE(
    STCP_LOWER_RQB_PTR_TYPE low_rqb_ptr,
        LSA_HANDLE_TYPE         handle
);
/* Function: write the handle to a lower layer RQB.
 *
 * Returns:  LSA_VOID
 */
#endif
/****************************************************************************/

#ifndef STCP_LOW_RQB_GET_CHA_OPEN_HANDLE
LSA_HANDLE_TYPE STCP_LOWER_OUT_FCT_ATTR STCP_LOW_RQB_GET_CHA_OPEN_HANDLE(
    STCP_LOWER_RQB_PTR_TYPE low_rqb_ptr
);
/* Function: read the handle from a lower layer RQB.
 *
 * Returns:  handle
 */
#endif

/*=============================================================================
 * Function: Copy global memory area.
 *           The memory areas of source and destination must not overlap!
 *
 * Returns:  LSA_VOID
 *===========================================================================*/
LSA_VOID  STCP_SYSTEM_OUT_FCT_ATTR  STCP_COPY_COMMON_MEM(
    LSA_VOID  STCP_COMMON_MEM_ATTR *  src_ptr,
    LSA_VOID  STCP_COMMON_MEM_ATTR *  dst_ptr,
    LSA_UINT16                         length           /* Length in bytes */
)
{
    LSAS_MEMCPY(dst_ptr,src_ptr,length);
}

#endif /* CFG_STCP_FW_COMP_ON */

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
