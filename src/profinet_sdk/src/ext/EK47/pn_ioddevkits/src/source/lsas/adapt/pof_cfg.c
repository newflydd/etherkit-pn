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
/*  F i l e               &F: pof_cfg.c                                 :F&  */
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

/* ****************************************************************************
*
*   H i s t o r y :
*   see /_doc/im151_3pn_hist.doc
*
* ****************************************************************************/

#define LSAS_MODULE_ID  26

//#include "pof_inc.h"
#include "lsas_inc.h"
#include "lsas_int.h"

#ifdef LSAS_CFG_USE_POF

/*---------------------------------------------------------------------------*/
/* user specific data structure for portid channeling                        */
/*---------------------------------------------------------------------------*/

typedef void  LSA_FCT_PTR(EDD_UPPER_OUT_FCT_ATTR, EDD_CALLBACK_FCT_PTR_TYPE)(EDD_UPPER_RQB_PTR_TYPE upper_rqb_ptr);

/*===========================================================================*/
/*                                constants                                  */
/*===========================================================================*/

/* Teiler des Basistakts zur Bereitstellung von 1us */
#define POF_US_DIVIDER              50

/******************************************************************************
 *                                                                            *
 *                                                                            *
 *  function prototypes in pof_sys.h                                          *
 *                                                                            *
 *                                                                            *
 ******************************************************************************/

/*=============================================================================
 * function name:  POF_GET_PATH_INFO
 *
 * function:       get system-pointer and detail-pointer of a communication
 *                 channel
 *
 * parameters:     LSA_UINT16              ...  *  ret_val_ptr:
 *                                      return value: LSA_RET_OK
 *                                                    LSA_RET_ERR_SYS_PATH
 *                 LSA_SYS_PTR_TYPE        ...  *  sys_ptr_ptr:
 *                                      return value: pointer to system-pointer
 *                 GSY_DETAIL_PTR_TYPE  ...  *  detail_ptr_ptr:
 *                                      return value: pointer to detail-pointer
 *                 LSA_SYS_PATH_TYPE               path: path of a
 *                                                       communication channel
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_GET_PATH_INFO
(
  LSA_UINT16              POF_LOCAL_MEM_ATTR *  ret_val_ptr,
  LSA_SYS_PTR_TYPE        POF_LOCAL_MEM_ATTR *  sys_ptr_ptr,
  POF_DETAIL_PTR_TYPE     POF_LOCAL_MEM_ATTR *  detail_ptr_ptr,
  LSA_SYS_PATH_TYPE                             path
)
{
    LSA_GET_PATH_INFO(ret_val_ptr, sys_ptr_ptr, detail_ptr_ptr,
                      path, LSAS_PATH_LAYER_POF);
}

/*=============================================================================
 * function name:  POF_RELEASE_PATH_INFO
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
 *                 POF_DETAIL_PTR_TYPE  detail_ptr:  detail-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_RELEASE_PATH_INFO(
    LSA_UINT16              POF_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_SYS_PTR_TYPE        sys_ptr,
    POF_DETAIL_PTR_TYPE     detail_ptr
)
{
    LSA_RELEASE_PATH_INFO(ret_val_ptr, sys_ptr, detail_ptr);
}

/*=============================================================================
 * function name:  POF_ALLOC_TIMER
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
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_ALLOC_TIMER(
    LSA_UINT16  POF_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_UINT16  POF_LOCAL_MEM_ATTR *  timer_id_ptr,
    LSA_UINT16                        timer_type,
    LSA_UINT16                        time_base
)
{
    LSAS_ALLOC_TIMER(ret_val_ptr, timer_id_ptr, timer_type, time_base,
                    pof_timeout, LSA_COMP_ID_POF);
}

/*=============================================================================
 * function name:  POF_START_TIMER
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
 *                                 system will call POF_timeout().
 *                 LSA_UINT16          timer_id:  id of timer
 *                 LSA_USER_ID_TYPE    user_id:   id of POF
 *                 LSA_UINT16          time:      running time
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_START_TIMER(
    LSA_UINT16        POF_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_UINT16                              timer_id,
    LSA_USER_ID_TYPE                        user_id,
    LSA_UINT16                              time
)
{
    LSAS_START_TIMER(ret_val_ptr, timer_id, user_id,
                    time, LSA_COMP_ID_POF);
}

/*=============================================================================
 * function name:  POF_STOP_TIMER
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
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_STOP_TIMER(
    LSA_UINT16  POF_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_UINT16                         timer_id
)
{
    LSAS_STOP_TIMER(ret_val_ptr, timer_id, LSA_COMP_ID_POF);
}

/*=============================================================================
 * function name:  POF_FREE_TIMER
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
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_FREE_TIMER(
    LSA_UINT16  POF_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_UINT16                         timer_id
)
{
    LSAS_FREE_TIMER(ret_val_ptr, timer_id, LSA_COMP_ID_POF);
}

/*=============================================================================
 * function name:  POF_WAIT_US
 *
 * function:       Function waits one Count us
 *
 * parameters:     LSA_UINT16       Count
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef POF_WAIT_US
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR POF_WAIT_US(LSA_UINT16 Count)
{
	LSAS_WAIT_10NS(Count * 100);
}
#endif

/*=============================================================================
 * function name:  POF_FATAL_ERROR
 *
 * function:       notify a fatal-error
 *
 * parameters:     LSA_UINT16                   length:     number of bytes
 *                 POF_FATAL_ERROR_PTR_TYPE  error_ptr:  pointer to
 *                                                          LSA_FATAL_ERROR
 *
 * return value:   LSA_VOID
 *===========================================================================*/

/* provided as macro in pof_cfg.h */
#ifndef POF_FATAL_ERROR
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_FATAL_ERROR(
    LSA_UINT16                 length,
    POF_FATAL_ERROR_PTR_TYPE  error_ptr
)
{
    /* !! must not be a function, by design !! */
}
#endif

/*=============================================================================
 * function name:  POF_ENTER
 *
 * function:       Protect sequence within POF from beeing interrupted.
 *                 (reentrance protection)
 *
 * parameters:     LSA_VOID
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_ENTER(
    LSA_VOID
)
{
    LSAS_OS_LOCK_ON_POF_TASK();
}

/*=============================================================================
 * function name:  POF_EXIT
 *
 * function:       Finishes POF_ENTER-Sequence.
 *
 * parameters:     LSA_VOID
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_EXIT(
    LSA_VOID
)
{
    LSAS_OS_RELEASE_POF_TASK();
}

/*=============================================================================
 * function name:  POF_ALLOC_LOCAL_MEM
 *
 * function:       allocate a local-memory
 *
 * parameters:     POF_LOCAL_MEM_PTR_TYPE  ...  *  local_mem_ptr_ptr:
 *                                   return value: pointer to local-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: No memory
 *                                                 available
 *                 LSA_UINT16                         length: length of memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_ALLOC_LOCAL_MEM(
    POF_LOCAL_MEM_PTR_TYPE  POF_LOCAL_MEM_ATTR *  local_mem_ptr_ptr,
    LSA_UINT32                                      length
)
{
    LSAS_ALLOC_COMP_MEM(local_mem_ptr_ptr, length, LSA_COMP_ID_POF);
}

/*=============================================================================
 * function name:  POF_FREE_LOCAL_MEM
 *
 * function:       frees local -memory
 *
 * parameters:              ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      local-memory
 *                 POF_LOCAL_MEM_PTR_TYPE  local_mem_ptr:  pointer to local-
 *                                                            memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_FREE_LOCAL_MEM(
    LSA_UINT16                 POF_LOCAL_MEM_ATTR *  ret_val_ptr,
    POF_LOCAL_MEM_PTR_TYPE                           local_mem_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, local_mem_ptr, LSA_COMP_ID_POF);
}


/*=============================================================================
 * function name:  POF_RQB_ERROR
 *
 * function:       notify a RQB-error in a user/system-request.
 *                 this function is called if there is a error (i.e. param)
 *                 within RQB which prevents notification of the caller
 *                 (i.e. missing call-back-function). the error-code is set
 *                 in "Status" in RQB. this error is typically a implementation
 *                 error.
 *
 * parameters:     POF_UPPER_RQB_PTR_TYPE   pRQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/

/* provided as macro in gsy_cfg.h */
#ifndef POF_RQB_ERROR
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_RQB_ERROR(
    POF_UPPER_RQB_PTR_TYPE  pRQB
)
{
    /* !! must not be a function, by design !! */
}
#endif

/*=============================================================================
 * function name:  POF_MEMSET_LOCAL
 *
 * function:       fills memory with value
 *
 * parameters:      POF_LOCAL_MEM_PTR_TYPE         pMem
 *                  LSA_UINT8                      Value
 *                  LSA_UINT32                     Length
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_MEMSET_LOCAL(
    POF_LOCAL_MEM_PTR_TYPE             pMem,
    LSA_UINT8                          Value,
    LSA_UINT32                         Length
)
{
    LSAS_MEMSET( pMem, Value, Length );
}

/*=============================================================================
 * function name:  POF_MEMCMP_COMMON()
 *
 * function:       compares Length bytes memory from pBuf1 to pBuf2.
 *
 * parameters:     const POF_COMMON_MEM_PTR_TYPE        pBuf1
 *                 const POF_COMMON_MEM_PTR_TYPE        pBuf2
 *                       LSA_UINT                       Length
 *
 * return value:   LSA_INT :  0 if identical
 *===========================================================================*/
#ifndef POF_MEMCMP_COMMON
LSA_INT  POF_SYSTEM_OUT_FCT_ATTR  POF_MEMCMP_COMMON(
    const POF_COMMON_MEM_PTR_TYPE  pBuf1,
    const POF_COMMON_MEM_PTR_TYPE  pBuf2,
          LSA_UINT                 Length
)
{
	return LSAS_MEMCMP(pBuf1, pBuf2, Length);
}
#endif

/*=============================================================================
 * function name:  POF_MEMCOPY_COMMON
 *
 * function:       copy Length bytes memory from pSrc to pDst.
 *
 * parameters:      POF_COMMON_MEM_PTR_TYPE        pDst
 *                  POF_COMMON_MEM_PTR_TYPE        pSrc
 *                  LSA_UINT32                      Length
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_MEMCOPY_COMMON(
    POF_COMMON_MEM_PTR_TYPE            pDst,
    POF_COMMON_MEM_PTR_TYPE            pSrc,
    LSA_UINT32                         Length
)
{
    LSAS_MEMCPY(pDst, pSrc, Length);
}

/*=============================================================================
 * function name:  POF_ALLOC_UPPER_RQB_LOCAL
 *
 * function:       allocate an upper-RQB for local use
 *
 * parameters:     POF_UPPER_RQB_PTR_TYPE  ...  *  upper_rqb_ptr_ptr:
 *                                   return value: pointer to local-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: No memory
 *                                                 available
 *                 LSA_UINT16                      length:   length of RQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_ALLOC_UPPER_RQB_LOCAL(
    POF_UPPER_RQB_PTR_TYPE  POF_LOCAL_MEM_ATTR *  upper_rqb_ptr_ptr,
    LSA_UINT16                                    length
)
{
    LSAS_ALLOC_COMP_MEM(upper_rqb_ptr_ptr, length, LSA_COMP_ID_POF);
}

/*=============================================================================
 * function name:  POF_FREE_UPPER_RQB_LOCAL
 *
 * function:       free an upper-RQB allocated with GSY_ALLOC_UPPER_RQB_LOCAL
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      upper-RQB or
 *                 POF_UPPER_RQB_PTR_TYPE  upper_rqb_ptr:  pointer to upper-
 *                                                            RQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_FREE_UPPER_RQB_LOCAL(
    LSA_UINT16                 POF_LOCAL_MEM_ATTR *  ret_val_ptr,
    POF_UPPER_RQB_PTR_TYPE                           upper_rqb_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, upper_rqb_ptr, LSA_COMP_ID_POF);
}

/*=============================================================================
 * function name:  POF_DO_TIMER_REQUEST
 *
 * function:       Does a POF_system() request with the spezified RQB. This
 *                 function is called within POF timer handler to signal
 *                 a Timer event to the main context of POF.
 *                 Systemadaption has to call POF_system() with the specified
 *                 RQB. Before calling, the systemadaption should change to
 *                 taskstate.
 *
 * parameters:     POF_UPPER_RQB_PTR_TYPE   pRQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_DO_TIMER_REQUEST(
    POF_UPPER_RQB_PTR_TYPE  pRQB
)
{

    LSAS_RQB_POF_REQUEST(pRQB, pof_request);
}

/*=============================================================================
 * function name:  POF_LOG_10
 *
 * function:       Frank Does a POF_system() request with the spezified RQB. This
 *                 function is called within POF timer handler to signal
 *                 a Timer event to the main context of POF.
 *                 Systemadaption has to call POF_system() with the specified
 *                 RQB. Before calling, the systemadaption should change to
 *                 taskstate.
 *                 Logarithmus zur Basis 10, auch bezeichnet als Zehnerlogarithmus
 *
 * parameters:     POF_UPPER_RQB_PTR_TYPE   pRQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR POF_LOG_10
(
  LSA_UINT32  const Arg_in_ppm,
  LSA_INT32         POF_LOCAL_MEM_ATTR  * pResult_in_ppm /*Festkomma mit 6 Nachstellen*/
)
{
    double LogOut;

    LogOut = LSAS_LOG_10 ((double)Arg_in_ppm);

    LogOut *= 1000000; /* ppm, parts per million */

    *pResult_in_ppm= (LSA_UINT32)LogOut;
}


/*=============================================================================
 * function name:  POF_POW_10
 *
 * function:       Frank Does a POF_system() request with the spezified RQB. This
 *                 function is called within POF timer handler to signal
 *                 a Timer event to the main context of POF.
 *                 Systemadaption has to call POF_system() with the specified
 *                 RQB. Before calling, the systemadaption should change to
 *                 taskstate.
 *                 Logarithmus zur Basis 10, auch bezeichnet als Zehnerlogarithmus
 *
 * parameters:     POF_UPPER_RQB_PTR_TYPE   pRQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef POF_POW_10
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR POF_POW_10
(
  LSA_INT16   const numerator,
  LSA_UINT16  const denominator,
  POF_LOCAL_MEM_U32_PTR_TYPE  pResult
)
{
    *pResult = (LSA_UINT32) LSAS_POW (10, (double)numerator/(double)denominator);
}
#endif


/*===========================================================================*/
/*====                    out functions to EDD                           ====*/
/*===========================================================================*/

/*=============================================================================
 * function name:  POF_EDD_OPEN_CHANNEL_LOWER
 *
 * function:       open a communication channel to EDD lower layer
 *
 * parameters:     POF_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:  pointer to lower-
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
LSA_VOID  POF_EDD_LOWER_OUT_FCT_ATTR  POF_EDD_OPEN_CHANNEL_LOWER(
    POF_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE             sys_ptr
)
{
    LSA_UNUSED_ARG(sys_ptr);

    ((EDD_UPPER_OPEN_CHANNEL_PTR_TYPE)EDD_RQB_GET_PPARAM(lower_rqb_ptr))->Cbf
         = (EDD_CALLBACK_FCT_PTR_TYPE)lsas_edd_to_pof_cbf;
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
 * function name:  POF_EDD_CLOSE_CHANNEL_LOWER
 *
 * function:       close a communication channel to EDD lower layer
 *
 * parameters:     POF_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:  pointer to lower-
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
LSA_VOID  POF_EDD_LOWER_OUT_FCT_ATTR  POF_EDD_CLOSE_CHANNEL_LOWER
(
  POF_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
  LSA_SYS_PTR_TYPE            sys_ptr
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
 * function name:  POF_EDD_REQUEST_LOWER
 *
 * function:       send a request to a communication channel to EDD lower layer
 *
 * parameters:     POF_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:  pointer to lower-
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
LSA_VOID  POF_EDD_LOWER_OUT_FCT_ATTR  POF_EDD_REQUEST_LOWER
(
  POF_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
  LSA_SYS_PTR_TYPE            sys_ptr
)
{
    LSA_UNUSED_ARG(sys_ptr);
#ifdef LSAS_CFG_USE_EDDI
    if(LSAS_EDD_CHECK_RCV(lower_rqb_ptr, eddi_request))
    {
        LSAS_RQB_EDD_REQUEST(lower_rqb_ptr, eddi_request);
    }
    /* LSAS_EDD_CHECK_RCV must care about rqb routing if it returns false */
#endif
#ifdef LSAS_CFG_USE_EDDP
    if(LSAS_EDD_CHECK_RCV(lower_rqb_ptr, eddp_request))
    {
        LSAS_RQB_EDD_REQUEST(lower_rqb_ptr, eddp_request);
    }
    /* LSAS_EDD_CHECK_RCV must care about rqb routing if it returns false */
#endif
#ifdef LSAS_CFG_USE_EDDS
    if(LSAS_EDD_CHECK_RCV(lower_rqb_ptr, edds_request))
    {
        LSAS_RQB_EDD_REQUEST(lower_rqb_ptr, edds_request);
    }
    /* LSAS_EDD_CHECK_RCV must care about rqb routing if it returns false */
#endif
}

/*=============================================================================
 * function name:  POF_EDD_ALLOC_LOWER_RQB
 *
 * function:       allocate a lower-RQB for EDD synchronously
 *
 * parameters:     POF_EDD_LOWER_RQB_PTR_TYPE  ...  *  lower_rqb_ptr_ptr:
 *                                   return value: pointer to lower-RQB-pointer
 *                                                 or LSA_NULL: The allocation
 *                                                 failed.
 *                 LSA_USER_ID_TYPE                   user_id:  not used
 *                 LSA_UINT16                         length:   length of RQB
 *                 LSA_SYS_PTR_TYPE                   sys_ptr:  system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_EDD_ALLOC_LOWER_RQB
(
  POF_EDD_LOWER_RQB_PTR_TYPE  POF_LOCAL_MEM_ATTR *  lower_rqb_ptr_ptr,
  LSA_USER_ID_TYPE                                  user_id,
  LSA_UINT16                                        length,
  LSA_SYS_PTR_TYPE                                  sys_ptr
)
{
    LSA_UNUSED_ARG(user_id);
    LSA_UNUSED_ARG(sys_ptr);

    LSAS_ALLOC_COMP_MEM(lower_rqb_ptr_ptr, length, LSA_COMP_ID_POF);
}

/*=============================================================================
 * function name:  POF_EDD_FREE_LOWER_RQB
 *
 * function:       free a lower-RQB from EDD
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      lower-RQB or
 *                                                      wrong system-pointer
 *                 POF_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:  pointer to lower-
 *                                                              RQB
 *                 LSA_SYS_PTR_TYPE             sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_EDD_FREE_LOWER_RQB
(
  LSA_UINT16                     POF_LOCAL_MEM_ATTR *  ret_val_ptr,
  POF_EDD_LOWER_RQB_PTR_TYPE                           lower_rqb_ptr,
  LSA_SYS_PTR_TYPE                                     sys_ptr
)
{
    LSA_UNUSED_ARG(sys_ptr);

    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_rqb_ptr, LSA_COMP_ID_POF);
}

/*=============================================================================
 * function name:  POF_EDD_ALLOC_LOWER_MEM
 *
 * function:       allocate a lower-memory for EDD synchronously
 *
 * parameters:     POF_EDD_LOWER_MEM_PTR_TYPE  ...  *  lower_mem_ptr_ptr:
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
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_EDD_ALLOC_LOWER_MEM
(
  POF_EDD_LOWER_MEM_PTR_TYPE  POF_LOCAL_MEM_ATTR *  lower_mem_ptr_ptr,
  LSA_USER_ID_TYPE                                  user_id,
  LSA_UINT16                                        length,
  LSA_SYS_PTR_TYPE                                  sys_ptr
)
{
    LSA_UNUSED_ARG(user_id);
    LSA_UNUSED_ARG(sys_ptr);

    LSAS_ALLOC_COMP_MEM(lower_mem_ptr_ptr, length, LSA_COMP_ID_POF);
}

/*=============================================================================
 * function name:  POF_EDD_FREE_LOWER_MEM
 *
 * function:       free a lower-memory for EDD
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      lower-memory or
 *                                                      wrong system-pointer
 *                 POF_EDD_LOWER_MEM_PTR_TYPE  lower_mem_ptr:  pointer to lower-
 *                                                              memory
 *                 LSA_SYS_PTR_TYPE             sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_EDD_FREE_LOWER_MEM
(
  LSA_UINT16                     POF_LOCAL_MEM_ATTR *  ret_val_ptr,
  POF_EDD_LOWER_MEM_PTR_TYPE                           lower_mem_ptr,
  LSA_SYS_PTR_TYPE                                     sys_ptr
)
{
    LSA_UNUSED_ARG(sys_ptr);

    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_mem_ptr, LSA_COMP_ID_POF);
}

/*=============================================================================
 * function name:  POF_FO_LED
 *
 * function:       Frank Does a POF_system() request with the spezified RQB. This
 *                 function is called within POF timer handler to signal
 *                 a Timer event to the main context of POF.
 *                 Systemadaption has to call POF_system() with the specified
 *                 RQB. Before calling, the systemadaption should change to
 *                 taskstate.
 *                 Logarithmus zur Basis 10, auch bezeichnet als Zehnerlogarithmus
 *
 * parameters:     POF_UPPER_RQB_PTR_TYPE   pRQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR POF_FO_LED
(
  LSA_UINT16        const PortID,
  LSA_UINT8         const ON,
  LSA_SYS_PTR_TYPE  sys_ptr
)
{
    LSAS_POF_FO_LED(PortID, ON, sys_ptr);
}

/*=============================================================================
 * function name:  POF_REQUEST_UPPER_DONE
 *
 * function:       returns a finished request to the upper layer
 *
 * parameters:     POF_UPPER_CALLBACK_FCT_PTR_TYPE  Cbf:
 *                     pointer to callback-function, given by pof_open_channel()
 *
 *                 POF_UPPER_RQB_PTR_TYPE  upper_rqb_ptr: pointer to upper-RQB
 *                 LSA_SYS_PTR_TYPE        sys_ptr:       system-pointer
 *
 * return value:   LSA_VOID
 *
 *===========================================================================*/

LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_REQUEST_UPPER_DONE
(
    POF_UPPER_CALLBACK_FCT_PTR_TYPE  Cbf,
    POF_UPPER_RQB_PTR_TYPE           upper_rqb_ptr,
    LSA_SYS_PTR_TYPE                 sys_ptr
)
{
    Cbf(upper_rqb_ptr);
}

LSA_UINT16 pof_version ( LSA_UINT16 length, LSA_VERSION_TYPE *version )
{
  LSA_UINT8 *p_version, *p_v;
  LSA_UINT16 i;

  /* Initialize version struct */
  LSA_VERSION_TYPE v = {
                        PNIO_LSA_COMPONENT_ID, {'L','S','A','_','P','O','F','\0','\0','\0'},
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

/*=============================================================================
 * function name:  POF_I2C_WRITE_OFFSET()
 *
 * function:       Executes I2C write transfer to the I2C device of the selected port
 *
 * parameters:     ...
 *
 * return value:   LSA_VOID
 *
 *===========================================================================*/
#ifndef POF_I2C_WRITE_OFFSET
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_I2C_WRITE_OFFSET(
    LSA_UINT16  POF_LOCAL_MEM_ATTR  *  const  ret_val_ptr,
    LSA_SYS_PTR_TYPE                   const  sys_ptr,
    LSA_UINT32                         const  PortId,
    LSA_UINT16                         const  I2CMuxSelect,
    LSA_UINT8                          const  I2CDevAddr,
    LSA_UINT8                          const  I2COffsetCnt,
    LSA_UINT8                          const  I2COffset1,
    LSA_UINT8                          const  I2COffset2,
    LSA_UINT32                         const  Size,
    LSA_UINT8  EDD_UPPER_MEM_ATTR   *  const  pBuf
)
{
#ifdef LSAS_CFG_USE_EDDP   /* for EDDP use this */
    *ret_val_ptr = eddp_I2C_Write_Offset( lsas_com_get_hddb(),  //IDB handle (refer to service EDDP_SRV_DEV_OPEN)
                               PortId,                  //User-PortId 1...4
                               I2CMuxSelect,            //can be used for HW-multiplexer-control
                               I2CDevAddr,              //0...127
                               I2COffsetCnt,            //0...2
                               I2COffset1,              //optional: 0...255
                               I2COffset2,              //optional: 0...255
                               1,                       //retry count (if transfer fails)
                               10 * 1000, // waiting time after an error
                               Size,                    //in bytes, 1...300
                               pBuf );
#elif defined(LSAS_CFG_USE_EDDI)
    *ret_val_ptr = eddi_I2C_Write_Offset( lsas_com_get_hddb(),  //IDB handle (refer to service EDDP_SRV_DEV_OPEN)
                                PortId,                  //User-PortId 1...4
                                I2CMuxSelect,            //can be used for HW-multiplexer-control
                                I2CDevAddr,              //0...127
                                I2COffsetCnt,            //0...2
                                I2COffset1,              //optional: 0...255
                                I2COffset2,              //optional: 0...255
                                1,                       //retry count (if transfer fails)
                                10 * 1000, // waiting time after an error
                                Size,                    //in bytes, 1...300
                                pBuf );
#endif
}
#endif

/*=============================================================================
 * function name:  POF_I2C_READ_OFFSET()
 *
 * function:       Executes I2C read transfer from the I2C device of the selected port
 *
 * parameters:     ...
 *
 * return value:   LSA_VOID
 *
 *===========================================================================*/
#ifndef POF_I2C_READ_OFFSET
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_I2C_READ_OFFSET(
    LSA_UINT16  POF_LOCAL_MEM_ATTR  *  const  ret_val_ptr,
    LSA_SYS_PTR_TYPE                   const  sys_ptr,
    LSA_UINT32                         const  PortId,
    LSA_UINT16                         const  I2CMuxSelect,
    LSA_UINT8                          const  I2CDevAddr,
    LSA_UINT8                          const  I2COffsetCnt,
    LSA_UINT8                          const  I2COffset1,
    LSA_UINT8                          const  I2COffset2,
    LSA_UINT32                         const  Size,
    LSA_UINT8  EDD_UPPER_MEM_ATTR   *  const  pBuf
)
{
    LSAS_SERVE_WATCHDOG();

#ifdef LSAS_CFG_USE_EDDP   /* for EDDP use this */
    *ret_val_ptr = eddp_I2C_Read_Offset( lsas_com_get_hddb(),  //IDB handle (refer to service EDDP_SRV_DEV_OPEN)
                                 PortId,        //User-PortId 1...4
                                 I2CMuxSelect,  //can be used for HW-multiplexer-control
                                 I2CDevAddr,    //0...127
                                 I2COffsetCnt,  //0...2
                                 I2COffset1,    //optional: 0...255
                                 I2COffset2,    //optional: 0...255
                                 1,                       //retry count (if transfer fails)
                                 10 * 1000, // waiting time after an error
                                 Size,          //in bytes, 1...300
                                 pBuf);
#elif defined(LSAS_CFG_USE_EDDI)
    *ret_val_ptr = eddi_I2C_Read_Offset( lsas_com_get_hddb(),  //IDB handle (refer to service EDDP_SRV_DEV_OPEN)
                                PortId,        //User-PortId 1...4
                                I2CMuxSelect,  //can be used for HW-multiplexer-control
                                I2CDevAddr,    //0...127
                                I2COffsetCnt,  //0...2
                                I2COffset1,    //optional: 0...255
                                I2COffset2,    //optional: 0...255
                                1,                       //retry count (if transfer fails)
                                10 * 1000, // waiting time after an error
                                Size,          //in bytes, 1...300
                                pBuf);
#endif
}

#endif

#endif

/*** end of file *************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
