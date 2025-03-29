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
/*  F i l e               &F: lldp_cfg.c                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  system adaption of this component                                        */
/*                                                                           */
/*****************************************************************************/

/* ***************************************************************************
*
*   H i s t o r y :
*   see /_doc/im151_3pn_hist.doc
*
* ****************************************************************************/

#define LSAS_MODULE_ID  21

//#include "lldp_inc.h"
#include "lsas_inc.h"

#include "lsas_int.h"

/******************************************************************************
 * routines from lldp_sys.h                                                   *
 ******************************************************************************/


/*=============================================================================
 * function name:  LLDP_GET_PATH_INFO
 *
 * function:       get system-pointer and detail-pointer of a communication
 *                 channel
 *
 * parameters:     LSA_UINT16              ...  *  ret_val_ptr:
 *                                      return value: LSA_RET_OK
 *                                                    LSA_RET_ERR_SYS_PATH
 *                 LSA_SYS_PTR_TYPE        ...  *  sys_ptr_ptr:
 *                                      return value: pointer to system-pointer
 *                 LLDP_DETAIL_PTR_TYPE  ...  *  detail_ptr_ptr:
 *                                      return value: pointer to detail-pointer
 *                 LSA_SYS_PATH_TYPE               path: path of a
 *                                                       communication channel
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID LLDP_GET_PATH_INFO(
    LSA_UINT16               *  ret_val_ptr,
    LSA_SYS_PTR_TYPE         *  sys_ptr_ptr,
    LLDP_DETAIL_PTR_TYPE     *  detail_ptr_ptr,
    LSA_SYS_PATH_TYPE           path
)
{
    LSA_GET_PATH_INFO(ret_val_ptr,sys_ptr_ptr,detail_ptr_ptr,path,LSAS_PATH_LAYER_LLDP);
}


/*=============================================================================
 * function name:  LLDP_RELEASE_PATH_INFO
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
 *                 LLDP_DETAIL_PTR_TYPE  detail_ptr:  detail-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    LLDP_RELEASE_PATH_INFO(
    LSA_UINT16               *  ret_val_ptr,
    LSA_SYS_PTR_TYPE            sys_ptr,
    LLDP_DETAIL_PTR_TYPE        detail_ptr
)
{
    LSA_RELEASE_PATH_INFO(ret_val_ptr,sys_ptr,detail_ptr);
}


/*=============================================================================
 * function name:  LLDP_ALLOC_TIMER
 *
 * function:       allocate a timer
 *
 * parameters:     LSA_UINT16  ...  *  ret_val_ptr:
 *                           return value: LSA_RET_OK            timer has been
 *                                                               allocated
 *                                         LSA_RET_ERR_NO_TIMER  no timer has
 *                                                               been allocated
 *                 LSA_UINT16  ...  *  timer_id_ptr:
 *                           return value: pointer to id of timer
 *                 LSA_UINT16          timer_type:  LSA_TIMER_TYPE_ONE_SHOT or
 *                                                  LSA_TIMER_TYPE_CYCLIC
 *                 LSA_UINT16          time_base:   LSA_TIME_BASE_1MS,
 *                                                  LSA_TIME_BASE_10MS,
 *                                                  LSA_TIME_BASE_100MS,
 *                                                  LSA_TIME_BASE_1S,
 *                                                  LSA_TIME_BASE_10S or
 *                                                  LSA_TIME_BASE_100S
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    LLDP_ALLOC_TIMER(
    LSA_UINT16   *  ret_val_ptr,
    LSA_UINT16   *  timer_id_ptr,
    LSA_UINT16      timer_type,
    LSA_UINT16      time_base
)
{
    LSAS_ALLOC_TIMER(ret_val_ptr, timer_id_ptr, timer_type, time_base, lldp_timeout, LSA_COMP_ID_LLDP);
}


/*=============================================================================
 * function name:  LLDP_START_TIMER
 *
 * function:       start a timer
 *
 * parameters:     LSA_UINT16  ...  *  ret_val_ptr:
 *                   return value: LSA_RET_OK                  timer has been
 *                                                             started
 *                                 LSA_RET_OK_TIMER_RESTARTED  timer has been
 *                                                             restarted
 *                                 LSA_RET_ERR_PARAM           timer hasn´t
 *                                                             been started
 *                                                             because of wrong
 *                                                             timer-id
 *                                 After the expiration of the running time
 *                                 system will call LLDP_timeout().
 *                 LSA_UINT16          timer_id:  id of timer
 *                 LSA_USER_ID_TYPE    user_id:   id of prefix
 *                 LSA_UINT16          time:      running time
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    LLDP_START_TIMER(
    LSA_UINT16         *  ret_val_ptr,
    LSA_UINT16            timer_id,
    LSA_USER_ID_TYPE      user_id,
    LSA_UINT16            time
)
{
    LSAS_START_TIMER(ret_val_ptr, timer_id, user_id, time, LSA_COMP_ID_LLDP);
}


/*=============================================================================
 * function name:  LLDP_STOP_TIMER
 *
 * function:       stop a timer
 *
 * parameters:     LSA_UINT16  ...  *  ret_val_ptr:
 *                   return value: LSA_RET_OK                    timer has been
 *                                                               stopped
 *                                 LSA_RET_OK_TIMER_NOT_RUNNING  timer was not
 *                                                               running; timer
 *                                                               is stopped
 *                                 LSA_RET_ERR_PARAM             timer hasn´t
 *                                                               stopped
 *                                                               because
 *                                                               of wrong
 *                                                               timer-id
 *                 LSA_UINT16          timer_id:  id of timer
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    LLDP_STOP_TIMER(
    LSA_UINT16   *  ret_val_ptr,
    LSA_UINT16      timer_id
)
{
    LSAS_STOP_TIMER(ret_val_ptr, timer_id, LSA_COMP_ID_LLDP);
}


/*=============================================================================
 * function name:  LLDP_FREE_TIMER
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
 *                              LSA_RET_ERR_PARAM             no deallocation
 *                                                            because of wrong
 *                                                            timer-id
 *                 LSA_UINT16          timer_id:  id of timer
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    LLDP_FREE_TIMER(
    LSA_UINT16   *  ret_val_ptr,
    LSA_UINT16      timer_id
)
{
    LSAS_FREE_TIMER(ret_val_ptr, timer_id, LSA_COMP_ID_LLDP);
}



/*=============================================================================
 * function name:  LLDP_FATAL_ERROR
 *
 * function:       notify a fatal-error
 *
 * parameters:     LSA_UINT16                   length:     number of bytes
 *                 LLDP_FATAL_ERROR_PTR_TYPE  error_ptr:  pointer to
 *                                                          LSA_FATAL_ERROR
 *
 * return value:   LSA_VOID
 *===========================================================================*/

/* NOTE: this routine will be implemented as a macro in lldp_cfg.h */
#ifndef LLDP_FATAL_ERROR
LSA_VOID    LLDP_FATAL_ERROR(
    LSA_UINT16                 length,
    LLDP_FATAL_ERROR_PTR_TYPE  error_ptr
);
#endif


/*=============================================================================
 * function name:  LLDP_ENTER
 *
 * function:       Protect sequence within LLDP from beeing interrupted.
 *                 (reentrance protection)
 *
 * parameters:     LSA_VOID
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    LLDP_ENTER(
    LSA_VOID
)
{
    LSAS_OS_LOCK_ON_ORG_TASK();
}

/*=============================================================================
 * function name:  LLDP_EXIT
 *
 * function:       Finishes LLDP_ENTER-Sequence.
 *
 * parameters:     LSA_VOID
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    LLDP_EXIT(
    LSA_VOID
)
{
    LSAS_OS_RELEASE_ORG_TASK();
}

/*=============================================================================
 * function name:  LLDP_ALLOC_LOCAL_MEM
 *
 * function:       allocate a local-memory
 *
 * parameters:     LLDP_LOCAL_MEM_PTR_TYPE  ...  *  local_mem_ptr_ptr:
 *                                   return value: pointer to local-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: No memory
 *                                                 available
 *                 LSA_UINT32                      length: length of memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    LLDP_ALLOC_LOCAL_MEM(
    LLDP_LOCAL_MEM_PTR_TYPE   *  local_mem_ptr_ptr,
    LSA_UINT32                   length
)
{
    LSAS_ALLOC_COMP_MEM(local_mem_ptr_ptr, length, LSA_COMP_ID_LLDP);
}

/*=============================================================================
 * function name:  LLDP_FREE_LOCAL_MEM
 *
 * function:       frees local -memory
 *
 * parameters:              ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      local-memory
 *                 LLDP_LOCAL_MEM_PTR_TYPE  local_mem_ptr:  pointer to local-
 *                                                            memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    LLDP_FREE_LOCAL_MEM(
    LSA_UINT16                  *  ret_val_ptr,
    LLDP_LOCAL_MEM_PTR_TYPE        local_mem_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, local_mem_ptr, LSA_COMP_ID_LLDP);
}



/*=============================================================================
 * function name:  LLDP_RQB_ERROR
 *
 * function:       notify a RQB-error in a user/system-request.
 *                 this function is called if there is a error (i.e. param)
 *                 within RQB which prevents notification of the caller
 *                 (i.e. missing call-back-function). the error-code is set
 *                 in "Status" in RQB. this error is typically a implementation
 *                 error.
 *
 * parameters:     LLDP_UPPER_RQB_PTR_TYPE   pRQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/

/* NOTE: this routine will be implemented as a macro in lldp_cfg.h */
#ifndef LLDP_RQB_ERROR
LSA_VOID    LLDP_RQB_ERROR(
    LLDP_UPPER_RQB_PTR_TYPE  pRQB
);
#endif

/*=============================================================================
 * function name:  LLDP_MEMSET_LOCAL
 *
 * function:       fills memory with value
 *
 * parameters:      LLDP_LOCAL_MEM_PTR_TYPE        pMem
 *                  LSA_UINT8                      Value
 *                  LSA_UINT32                     Length
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    LLDP_MEMSET_LOCAL(
    LLDP_LOCAL_MEM_PTR_TYPE             pMem,
    LSA_UINT8                           Value,
    LSA_UINT32                          Length
)
{
    LSAS_MEMSET(pMem,Value,Length);
}

/*=============================================================================
 * function name:  LLDP_MEMCOPY_COMMON
 *
 * function:       copy Length bytes memory from pSrc to pDst.
 *
 * parameters:      LLDP_COMMON_MEM_PTR_TYPE        pDst
 *                  LLDP_COMMON_MEM_PTR_TYPE        pSrc
 *                  LSA_UINT32                      Length
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    LLDP_MEMCOPY_COMMON(
    LLDP_COMMON_MEM_PTR_TYPE            pDst,
    LLDP_COMMON_MEM_PTR_TYPE            pSrc,
    LSA_UINT32                          Length
)
{
    LSAS_MEMCPY(pDst, pSrc, Length);                                    // lint !e668,669  Possibly passing a null pointer to function
}

/*=============================================================================
 * function name:  LLDP_MEMCMP_COMMON
 *
 * function:       compares Length bytes of memory from pBuf1 with pBuf2
 *
 * parameters:      LLDP_COMMON_MEM_PTR_TYPE        pBuf1
 *                  LLDP_COMMON_MEM_PTR_TYPE        pBuf2
 *                  LSA_UINT32                      Length
 *
 * return value:   LSA_TRUE:  buffers are the same
 *                 LSA_FALSE: buffers differ
 *===========================================================================*/

LSA_BOOL   LLDP_MEMCMP_COMMON(
    LLDP_COMMON_MEM_PTR_TYPE            pBuf1,
    LLDP_COMMON_MEM_PTR_TYPE            pBuf2,
    LSA_UINT32                          Length
)
{
    return (LSA_BOOL) ( LSAS_MEMCMP (pBuf1, pBuf2, Length) == 0 );      // lint !e668  Possibly passing a null pointer to function
}

/*=============================================================================
 * function name:  LLDP_ALLOC_UPPER_RQB_LOCAL
 *
 * function:       allocate an upper-RQB for local use
 *
 * parameters:     LLDP_UPPER_RQB_PTR_TYPE  ...  *  upper_rqb_ptr_ptr:
 *                                   return value: pointer to local-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: No memory
 *                                                 available
 *                 LSA_UINT16                      length:   length of RQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    LLDP_ALLOC_UPPER_RQB_LOCAL(
    LLDP_UPPER_RQB_PTR_TYPE   *  upper_rqb_ptr_ptr,
    LSA_UINT16                                    length
)
{
    LSAS_ALLOC_COMP_MEM(upper_rqb_ptr_ptr, length, LSA_COMP_ID_LLDP);
}

/*=============================================================================
 * function name:  LLDP_FREE_UPPER_RQB_LOCAL
 *
 * function:       free an upper-RQB allocated with LLDP_ALLOC_UPPER_RQB_LOCAL
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      upper-RQB or
 *                 LLDP_UPPER_RQB_PTR_TYPE  upper_rqb_ptr:  pointer to upper-
 *                                                            RQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    LLDP_FREE_UPPER_RQB_LOCAL(
    LSA_UINT16                  *  ret_val_ptr,
    LLDP_UPPER_RQB_PTR_TYPE                           upper_rqb_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, upper_rqb_ptr, LSA_COMP_ID_LLDP);
}

/*=============================================================================
 * function name:  LLDP_DO_TIMER_REQUEST
 *
 * function:       Does a LLDP_system() request with the spezified RQB. This
 *                 function is called within LLDP timer handler to signal
 *                 a Timer event to the main context of LLDP.
 *                 Systemadaption has to call LLDP_system() with the specified
 *                 RQB. Before calling, the systemadaption should change to
 *                 taskstate.
 *
 * parameters:     LLDP_UPPER_RQB_PTR_TYPE   pRQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    LLDP_DO_TIMER_REQUEST(
    LLDP_UPPER_RQB_PTR_TYPE  pRQB
)
{

    LSAS_RQB_LLDP_REQUEST(pRQB, lldp_request);
}


/*=============================================================================
 * function name:  LLDP_REQUEST_UPPER_DONE
 *
 * function:       returns a finished request to the upper layer
 *
 * parameters:     LLDP_UPPER_CALLBACK_FCT_PTR_TYPE
 *                                            prefix_request_upper_done_ptr:
 *                     pointer to prefix_request_upper_done_ptr, given by
 *                     prefix_open_channel()
 *
 *                 LLDP_UPPER_RQB_PTR_TYPE  upper_rqb_ptr:  pointer to upper-
 *                                                            RQB
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *
 *===========================================================================*/
#if 0
typedef  LSA_VOID  LSA_FCT_PTR( ,
                               LLDP_UPPER_CALLBACK_FCT_PTR_TYPE)(
                               LLDP_UPPER_RQB_PTR_TYPE  upper_rqb_ptr);
#endif

LSA_VOID    LLDP_REQUEST_UPPER_DONE(
       LLDP_UPPER_CALLBACK_FCT_PTR_TYPE  lldp_request_upper_done_ptr,
       LLDP_UPPER_RQB_PTR_TYPE           upper_rqb_ptr,
       LSA_SYS_PTR_TYPE                  sys_ptr
)
{
    lldp_request_upper_done_ptr(upper_rqb_ptr);
}

/******************************************************************************
 * routines from lldp_low.h                                                   *
 ******************************************************************************/


/*===========================================================================*/
/*====                    out functions to EDD                           ====*/
/*===========================================================================*/

/*=============================================================================
 * function name:  LLDP_EDD_OPEN_CHANNEL_LOWER
 *
 * function:       open a communication channel to EDD lower layer
 *
 * parameters:     LLDP_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:  pointer to lower-
 *                                                              RQB
 *                 LSA_SYS_PTR_TYPE             sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *
 * RQB-parameters:
 *     RQB-header:
 *     LSA_OPCODE_TYPE    opcode:        *_OPC_OPEN_CHANNEL
 *     LSA_HANDLE_TYPE    handle:        channel-handle of prefix
 *     LSA_USER_ID_TYPE   user-id:       id of prefix
 *     RQB-args:
 *     LSA_HANDLE_TYPE    handle_upper:  channel-handle of prefix
 *     LSA_SYS_PATH_TYPE  sys_path:      system-path of channel
 *     LSA_VOID  LSA_FCT_PTR(*_UPPER_OUT_FCT_ATTR, *_request_upper_done_ptr)
 *                                    (DCP_UPPER_RQB_PTR_TYPE upper_rqb_ptr)
 *                                       callback-function
 *
 * RQB-return values via callback-function:
 *     RQB-args:
 *     LSA_HANDLE_TYPE    handle:        channel-handle of lower layer
 *                        response:      *_RSP_OK
 *                                       *_RSP_ERR_SYNTAX
 *                                       *_RSP_ERR_RESOURCE
 *                                       *_RSP_ERR_SYS_PATH
 *                                       or others
 *     All other RQB-parameters are unchanged.
 *===========================================================================*/

LSA_VOID    LLDP_EDD_OPEN_CHANNEL_LOWER(
    LLDP_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE             sys_ptr
)
{
    LSA_UNUSED_ARG(sys_ptr);

    ((EDD_UPPER_OPEN_CHANNEL_PTR_TYPE)EDD_RQB_GET_PPARAM(lower_rqb_ptr))->Cbf
         = lsas_edd_to_lldp_cbf;
#ifdef LSAS_CFG_USE_EDDI
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr, eddi_open_channel);
#endif
#ifdef LSAS_CFG_USE_EDDP
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr, eddp_open_channel);
#endif
#ifdef LSAS_CFG_USE_EDDS
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr, edds_open_channel);
#endif
}

/*=============================================================================
 * function name:  LLDP_EDD_CLOSE_CHANNEL_LOWER
 *
 * function:       close a communication channel to EDD lower layer
 *
 * parameters:     LLDP_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:  pointer to lower-
 *                                                              RQB
 *                 LSA_SYS_PTR_TYPE             sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *
 * RQB-parameters:
 *     RQB-header:
 *     LSA_OPCODE_TYPE   opcode:   *_OPC_CLOSE_CHANNEL
 *     LSA_HANDLE_TYPE   handle:   channel-handle of lower layer
 *     LSA_USER_ID_TYPE  user-id:  id of prefix
 *
 * RQB-return values via callback-function:
 *     RQB-header:
 *     LSA_HANDLE_TYPE   handle:    channel-handle of prefix
 *     RQB-args:         response:  *_RSP_OK
 *                                  *_RSP_ERR_SYNTAX
 *                                  or others
 *     All other RQB-parameters are unchanged.
 *===========================================================================*/

LSA_VOID    LLDP_EDD_CLOSE_CHANNEL_LOWER(
    LLDP_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE             sys_ptr
)
{
    LSA_UNUSED_ARG(sys_ptr);
#ifdef LSAS_CFG_USE_EDDI
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr, eddi_close_channel);
#endif
#ifdef LSAS_CFG_USE_EDDP
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr, eddp_close_channel);
#endif
#ifdef LSAS_CFG_USE_EDDS
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr, edds_close_channel);
#endif
}

/*=============================================================================
 * function name:  LLDP_EDD_REQUEST_LOWER
 *
 * function:       send a request to a communication channel to EDD lower layer
 *
 * parameters:     LLDP_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:  pointer to lower-
 *                                                              RQB
 *                 LSA_SYS_PTR_TYPE             sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *
 * RQB-parameters:
 *     RQB-header:
 *     LSA_OPCODE_TYPE   opcode:    *_OPC_*
 *     LSA_HANDLE_TYPE   handle:    channel-handle of lower layer
 *     LSA_USER_ID_TYPE  user-id:   id of prefix
 *     RQB-args:                    Depend on kind of request.
 *
 * RQB-return values via callback-function:
 *     RQB-header:
 *     LSA_OPCODE_TYPE   opcode:    *_OPC_*
 *     LSA_HANDLE_TYPE   handle:    channel-handle of prefix
 *     LSA_USER_ID_TYPE  user-id:   id of prefix
 *     RQB-args:         response:  *_RSP_OK
 *                                  *_RSP_ERR_SYNTAX
 *                                  *_RSP_ERR_REF
 *                                  *_RSP_ERR_SEQUENCE
 *                                  or others
 *     All other RQB-parameters depend on kind of request.
 *===========================================================================*/

LSA_VOID LLDP_EDD_REQUEST_LOWER(
    LLDP_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE             sys_ptr
)
{
    LSA_UNUSED_ARG(sys_ptr);

#ifdef LSAS_CFG_USE_EDDI
            LSAS_RQB_EDD_REQUEST_HIGH(lower_rqb_ptr, eddi_request);
#endif
#ifdef LSAS_CFG_USE_EDDP
            LSAS_RQB_EDD_REQUEST_HIGH(lower_rqb_ptr, eddp_request);
#endif
#ifdef LSAS_CFG_USE_EDDS
            LSAS_RQB_EDD_REQUEST_HIGH(lower_rqb_ptr, edds_request);
#endif
}

/*=============================================================================
 * function name:  LLDP_EDD_ALLOC_LOWER_RQB
 *
 * function:       allocate a lower-RQB for EDD synchronously
 *
 * parameters:     LLDP_EDD_LOWER_RQB_PTR_TYPE  ...  *  lower_rqb_ptr_ptr:
 *                                   return value: pointer to lower-RQB-pointer
 *                                                 or LSA_NULL: The allocation
 *                                                 failed.
 *                 LSA_USER_ID_TYPE                   user_id:  not used
 *                 LSA_UINT16                         length:   length of RQB
 *                 LSA_SYS_PTR_TYPE                   sys_ptr:  system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    LLDP_EDD_ALLOC_LOWER_RQB(
    LLDP_EDD_LOWER_RQB_PTR_TYPE                      *  lower_rqb_ptr_ptr,
    LSA_USER_ID_TYPE                                    user_id,
    LSA_UINT16                                          length,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    LSA_UNUSED_ARG(user_id);
    LSA_UNUSED_ARG(sys_ptr);

    LSAS_ALLOC_COMP_MEM(lower_rqb_ptr_ptr, length, LSA_COMP_ID_LLDP);
}

/*=============================================================================
 * function name:  LLDP_EDD_FREE_LOWER_RQB
 *
 * function:       free a lower-RQB from EDD
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      lower-RQB or
 *                                                      wrong system-pointer
 *                 LLDP_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:  pointer to lower-
 *                                                              RQB
 *                 LSA_SYS_PTR_TYPE             sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    LLDP_EDD_FREE_LOWER_RQB(
    LSA_UINT16                                         *  ret_val_ptr,
    LLDP_EDD_LOWER_RQB_PTR_TYPE                           lower_rqb_ptr,
    LSA_SYS_PTR_TYPE                                      sys_ptr
)
{
    LSA_UNUSED_ARG(sys_ptr);

    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_rqb_ptr, LSA_COMP_ID_LLDP);
}

/*=============================================================================
 * function name:  LLDP_EDD_ALLOC_LOWER_MEM
 *
 * function:       allocate a lower-memory for EDD synchronously
 *
 * parameters:     LLDP_EDD_LOWER_MEM_PTR_TYPE  ...  *  lower_mem_ptr_ptr:
 *                                   return value: pointer to lower-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: failed.
 *                 LSA_USER_ID_TYPE                   user_id:  not used
 *                 LSA_UINT16                         length:   length of
 *                                                              memory
 *                 LSA_SYS_PTR_TYPE  sys_ptr:                   system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    LLDP_EDD_ALLOC_LOWER_MEM(
    LLDP_EDD_LOWER_MEM_PTR_TYPE                      *  lower_mem_ptr_ptr,
    LSA_USER_ID_TYPE                                    user_id,
    LSA_UINT16                                          length,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    LSA_UNUSED_ARG(user_id);
    LSA_UNUSED_ARG(sys_ptr);

    LSAS_ALLOC_COMP_MEM(lower_mem_ptr_ptr, length, LSA_COMP_ID_LLDP);
}

/*=============================================================================
 * function name:  LLDP_EDD_FREE_LOWER_MEM
 *
 * function:       free a lower-memory for EDD
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      lower-memory or
 *                                                      wrong system-pointer
 *                 LLDP_EDD_LOWER_MEM_PTR_TYPE  lower_mem_ptr:  pointer to lower-
 *                                                              memory
 *                 LSA_SYS_PTR_TYPE             sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    LLDP_EDD_FREE_LOWER_MEM(
    LSA_UINT16                                         *  ret_val_ptr,
    LLDP_EDD_LOWER_MEM_PTR_TYPE                           lower_mem_ptr,
    LSA_SYS_PTR_TYPE                                      sys_ptr
)
{
    LSA_UNUSED_ARG(sys_ptr);

    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_mem_ptr, LSA_COMP_ID_LLDP);
}

/*=============================================================================
 * function name:  LLDP_EDD_ALLOC_LOWER_TXMEM
 *
 * function:       allocate a lower memory for send PDUs
 *
 *                 This memory has to be DWORD-aligned!!
 *
 * parameters:     LLDP_EDD_LOWER_MEM_PTR_TYPE  ...  * lower_mem_ptr_ptr:
 *                                   return value: pointer to lower-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: failed.
 *                 LSA_USER_ID_TYPE                   user_id:  id of edd
 *                 LSA_UINT16                         length:   length of
 *                                                              memory
 *                 LSA_SYS_PTR_TYPE  sys_ptr:                   system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    LLDP_EDD_ALLOC_LOWER_TXMEM(
    LLDP_EDD_LOWER_MEM_PTR_TYPE                      *lower_mem_ptr_ptr,
    LSA_USER_ID_TYPE                                  user_id,
    LSA_UINT16                                        length,
    LSA_SYS_PTR_TYPE                                  sys_ptr
)
{
    LSAS_ALLOC_PACKET_MEM(lower_mem_ptr_ptr, length, LSA_COMP_ID_LLDP);

    LSA_UNUSED_ARG(sys_ptr);
    LSA_UNUSED_ARG(user_id);
}

/*=============================================================================
 * function name:  LLDP_EDD_FREE_LOWER_TXMEM
 *
 * function:       free a lower memory for send PDUs
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      lower-memory or
 *                                                      wrong system-pointer
 *                 LLDP_EDD_LOWER_MEM_PTR_TYPE  lower_mem_ptr:pointer to lower-
 *                                                            memory
 *                 LSA_SYS_PTR_TYPE             sys_ptr:      system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    LLDP_EDD_FREE_LOWER_TXMEM(
    LSA_UINT16                                     *    ret_val_ptr,
    LLDP_EDD_LOWER_MEM_PTR_TYPE                         lower_mem_ptr,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    LSAS_FREE_PACKET_MEM(ret_val_ptr, lower_mem_ptr, LSA_COMP_ID_LLDP);
    LSA_UNUSED_ARG(sys_ptr);
    *(ret_val_ptr) = LSA_RET_OK;
}

/*=============================================================================
 * function name:  LLDP_EDD_ALLOC_LOWER_RXMEM
 *
 * function:       allocate a lower memory for recv PDUs
 *
 *                 This memory has to be DWORD-aligned!!
 *
 * parameters:     LLDP_EDD_LOWER_MEM_PTR_TYPE  ...  * lower_mem_ptr_ptr:
 *                                   return value: pointer to lower-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: failed.
 *                 LSA_USER_ID_TYPE                   user_id:  id of edd
 *                 LSA_UINT16                         length:   length of
 *                                                              memory
 *                 LSA_SYS_PTR_TYPE  sys_ptr:                   system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    LLDP_EDD_ALLOC_LOWER_RXMEM(
    LLDP_EDD_LOWER_MEM_PTR_TYPE                      *lower_mem_ptr_ptr,
    LSA_USER_ID_TYPE                                  user_id,
    LSA_UINT16                                        length,
    LSA_SYS_PTR_TYPE                                  sys_ptr
)
{
    LSAS_ALLOC_PACKET_MEM(lower_mem_ptr_ptr, length, LSA_COMP_ID_LLDP);

    LSA_UNUSED_ARG(user_id);
    LSA_UNUSED_ARG(sys_ptr);
}

/*=============================================================================
 * function name:  LLDP_EDD_FREE_LOWER_RXMEM
 *
 * function:       free a lower memory for recv PDUs
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      lower-memory or
 *                                                      wrong system-pointer
 *                 LLDP_EDD_LOWER_MEM_PTR_TYPE  lower_mem_ptr:pointer to lower-
 *                                                            memory
 *                 LSA_SYS_PTR_TYPE             sys_ptr:      system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID  LLDP_EDD_FREE_LOWER_RXMEM(
    LSA_UINT16                                     *    ret_val_ptr,
    LLDP_EDD_LOWER_MEM_PTR_TYPE                         lower_mem_ptr,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    LSAS_FREE_PACKET_MEM(ret_val_ptr, lower_mem_ptr, LSA_COMP_ID_LLDP);
    LSA_UNUSED_ARG(sys_ptr);

    *(ret_val_ptr) = LSA_RET_OK;
}

LSA_UINT16 lldp_version ( LSA_UINT16 length, LSA_VERSION_TYPE *version )
{
  LSA_UINT8 *p_version, *p_v;
  LSA_UINT16 i;

  /* Initialize version struct */
  LSA_VERSION_TYPE v = {
                        PNIO_LSA_COMPONENT_ID, {'L','S','A','_','L','L','D','P','\0','\0'},
                        PNIO_KIND,             PNIO_VERSION,
                        PNIO_DISTRIBUTION,     PNIO_FIX,
                        PNIO_HOTFIX,           PNIO_PROJECT_NUMBER,
                        PNIO_INCREMENT,        PNIO_INTEGRATION_COUNTER,
                        PNIO_GEN_COUNTER
                       };

  /* Adapt length if necessary */
  if (length > sizeof(v))
  {
    length = sizeof(v);
  }

  /* Initialize pointers and copy version structure */
  p_version = (LSA_UINT8 *) version;
  p_v       = (LSA_UINT8 *) &v;
  for ( i=0;i<length; i++ ) { p_version[i] = p_v[i]; };

  return (LSA_UINT16)(sizeof(v) - length);
}
/*****************************************************************************/
/*  end of file.                                                             */
/*****************************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
