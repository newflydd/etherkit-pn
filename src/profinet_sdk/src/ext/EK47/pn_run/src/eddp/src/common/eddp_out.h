
#ifndef EDDP_OUT_H                       /* ----- reinclude-protection ----- */
#define EDDP_OUT_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

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
/*  C o m p o n e n t     &C: EDDP (EthernetDeviceDriver for PN-IP)     :C&  */
/*                                                                           */
/*  F i l e               &F: eddp_out.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  EDDP output macros framework. Have to be filled by systemadaption.       */
/*  This file declares prototypes with no function if macro is not defined   */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  14.08.08    JS    initial version.                                       */
/*  20.07.10    JS    added EDDP_REQUEST_UPPER_DONE                          */
/*  17.04.12    VS    added output macro EDDP_SIGNAL_SENDCLOCK_CHANGE        */
/*  09.03.16    HM    added output macro EDDP_SIGNAL_TIME_SYNC_STATE_CHANGE  */
#endif
/*****************************************************************************/


/*****************************************************************************/
/*                                                                           */
/* Output-functions for System                                               */
/*                                                                           */
/*****************************************************************************/

/*=============================================================================
 * function name:  EDDP_GET_PATH_INFO
 *
 * function:       get system-pointer and detail-pointer of a communication 
 *                 channel
 *
 * parameters:     LSA_UINT16              ...  *  ret_val_ptr:
 *                                      return value: LSA_RET_OK
 *                                                    LSA_RET_ERR_SYS_PATH
 *                 LSA_SYS_PTR_TYPE        ...  *  sys_ptr_ptr:
 *                                      return value: pointer to system-pointer
 *                 EDDP_DETAIL_PTR_TYPE  ...  *  detail_ptr_ptr:
 *                                      return value: pointer to detail-pointer
 *                 LSA_SYS_PATH_TYPE               path: path of a
 *                                                       communication channel  
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_GET_PATH_INFO
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_GET_PATH_INFO(
    LSA_UINT16              EDDP_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_SYS_PTR_TYPE        EDDP_LOCAL_MEM_ATTR *  sys_ptr_ptr,
    EDDP_DETAIL_PTR_TYPE    EDDP_LOCAL_MEM_ATTR *  detail_ptr_ptr,
    LSA_SYS_PATH_TYPE                              path
);
#endif

/*=============================================================================
 * function name:  EDDP_RELEASE_PATH_INFO
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
 *                 EDDP_DETAIL_PTR_TYPE  detail_ptr:  detail-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_RELEASE_PATH_INFO
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_RELEASE_PATH_INFO(
    LSA_UINT16              EDDP_LOCAL_MEM_ATTR *   ret_val_ptr,
    LSA_SYS_PTR_TYPE                                sys_ptr,
    EDDP_DETAIL_PTR_TYPE                            detail_ptr
);
#endif

/*=============================================================================
 * function name:  EDDP_ALLOC_LOCAL_MEM
 *
 * function:       allocate a local-memory
 *
 * parameters:     EDDP_LOCAL_MEM_PTR_TYPE  ...  *  local_mem_ptr_ptr:
 *                                   return value: pointer to local-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: No memory 
 *                                                 available
 *                 LSA_UINT32                      length: length of memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_ALLOC_LOCAL_MEM
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_ALLOC_LOCAL_MEM(
    EDDP_LOCAL_MEM_PTR_TYPE EDDP_LOCAL_MEM_ATTR *   local_mem_ptr_ptr,
    LSA_UINT32                                      length
);
#endif

/*=============================================================================
 * function name:  EDDP_FREE_LOCAL_MEM
 *
 * function:       frees local -memory
 *
 * parameters:              ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      local-memory
 *                 EDDP_LOCAL_MEM_PTR_TYPE  local_mem_ptr:  pointer to local-
 *                                                            memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_FREE_LOCAL_MEM
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_FREE_LOCAL_MEM(
    LSA_UINT16                  EDDP_LOCAL_MEM_ATTR *   ret_val_ptr,
    EDDP_LOCAL_MEM_PTR_TYPE                             local_mem_ptr
);
#endif

/*=============================================================================
 * function name:  EDDP_ALLOC_TIMER
 *
 * function:       allocate a timer
 *
 * parameters:     LSA_UINT16  ...  *  ret_val_ptr:
 *                           return value: LSA_RET_OK            timer has been
 *                                                               allocated 
 *                                         LSA_RET_ERR_NO_TIMER  no timer has 
 *                                                               been allocated 
 *                 LSA_TIMER_ID_TYPE  ...  *  timer_id_ptr:
 *                           return value: pointer to id of timer
 *                 LSA_UINT16          timer_type:  LSA_TIMER_TYPE_ONE_SHOT or
 *                                                  LSA_TIMER_TYPE_CYCLIC
 *                 LSA_UINT16          time_base:   LSA_TIME_BASE_1US,
 *                                                  LSA_TIME_BASE_10US,
 *                                                  LSA_TIME_BASE_100US,
 *                                                  LSA_TIME_BASE_1MS,
 *                                                  LSA_TIME_BASE_10MS,
 *                                                  LSA_TIME_BASE_100MS,
 *                                                  LSA_TIME_BASE_1S,
 *                                                  LSA_TIME_BASE_10S or
 *                                                  LSA_TIME_BASE_100S
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_ALLOC_TIMER
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_ALLOC_TIMER(
    LSA_UINT16          EDDP_LOCAL_MEM_ATTR *   ret_val_ptr,
    LSA_TIMER_ID_TYPE   EDDP_LOCAL_MEM_ATTR *   timer_id_ptr,
    LSA_UINT16                                  timer_type,
    LSA_UINT16                                  time_base
);
#endif

/*=============================================================================
 * function name:  EDDP_START_TIMER
 *
 * function:       start a timer
 *
 * parameters:     LSA_UINT16  ...  *  ret_val_ptr:
 *                   return value: LSA_RET_OK                  timer has been
 *                                                             started
 *                                 LSA_RET_OK_TIMER_RESTARTED  timer has been
 *                                                             restarted
 *                                 LSA_RET_ERR_PARAM           timer hasn�t
 *                                                             been started
 *                                                             because of wrong
 *                                                             timer-id
 *                                 After the expiration of the running time 
 *                                 system will call eddp_timeout().
 *                 LSA_TIMER_ID_TYPE   timer_id:  id of timer
 *                 LSA_USER_ID_TYPE    user_id:   id of prefix
 *                 LSA_UINT16          time:      running time
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_START_TIMER
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_START_TIMER(
    LSA_UINT16          EDDP_LOCAL_MEM_ATTR *   ret_val_ptr,
    LSA_TIMER_ID_TYPE                           timer_id,
    LSA_USER_ID_TYPE                            user_id,
    LSA_UINT16                                  time
);
#endif

/*=============================================================================
 * function name:  EDDP_STOP_TIMER
 *
 * function:       stop a timer
 *
 * parameters:     LSA_UINT16  ...  *  ret_val_ptr:
 *                   return value: LSA_RET_OK                    timer has been
 *                                                               stopped
 *                                 LSA_RET_OK_TIMER_NOT_RUNNING  timer was not
 *                                                               running; timer
 *                                                               is stopped
 *                                 LSA_RET_ERR_PARAM             timer hasn�t
 *                                                               stopped
 *                                                               because
 *                                                               of wrong 
 *                                                               timer-id
 *                 LSA_TIMER_ID_TYPE          timer_id:  id of timer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_STOP_TIMER
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_STOP_TIMER(
    LSA_UINT16          EDDP_LOCAL_MEM_ATTR *   ret_val_ptr,
    LSA_TIMER_ID_TYPE                           timer_id
);
#endif

/*=============================================================================
 * function name:  EDDP_FREE_TIMER
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
 *                 LSA_TIMER_ID_TYPE    timer_id:  id of timer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_FREE_TIMER
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_FREE_TIMER(
    LSA_UINT16          EDDP_LOCAL_MEM_ATTR *   ret_val_ptr,
    LSA_TIMER_ID_TYPE                           timer_id
);
#endif

/*=============================================================================
 * function name:  EDDP_FATAL_ERROR
 *
 * function:       notify a fatal-error
 *
 * parameters:     LSA_UINT16                   length:     number of bytes
 *                 EDDP_FATAL_ERROR_PTR_TYPE  error_ptr:  pointer to
 *                                                          LSA_FATAL_ERROR
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_FATAL_ERROR
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_FATAL_ERROR(
    LSA_UINT16                  length,
    EDDP_FATAL_ERROR_PTR_TYPE   error_ptr
);
#endif

/*============================================================================= 
 * function name:  EDDP_REQUEST_UPPER_DONE 
 * 
 * function:       returns a finished user request (RQB) from eddp_open_channel(), 
 *                 eddp_request() or eddp_close_channel() to the upper layer.     
 * 
 * parameters:     EDD_UPPER_CALLBACK_FCT_PTR_TYPE  Cbf    :   
 *                     pointer to Callbackfunction (CBF) from eddp_open_channel(). 
 * 
 *                 EDD_UPPER_RQB_PTR_TYPE     upper_rqb_ptr:  pointer to upper- 
 *                                                            RQB 
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 * 
 * return value:   LSA_VOID 
 * 
 *===========================================================================*/
#ifndef EDDP_REQUEST_UPPER_DONE
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR EDDP_REQUEST_UPPER_DONE(    
	EDD_UPPER_CALLBACK_FCT_PTR_TYPE     Cbf,    
	EDD_UPPER_RQB_PTR_TYPE              upper_rqb_ptr,    
	LSA_SYS_PTR_TYPE                    sys_ptr 
);
#endif

/*=============================================================================
 * function name:  EDDP_RQB_ERROR
 *
 * function:       notify a RQB-error in a user/system-request. 
 *                 this function is called if there is a error (i.e. param) 
 *                 within RQB which prevents notification of the caller 
 *                 (i.e. missing call-back-function). the error-code is set
 *                 in "Status" in RQB. this error is typically a implementation
 *                 error.
 *
 * parameters:     EDD_UPPER_RQB_PTR_TYPE   pRQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_RQB_ERROR
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_RQB_ERROR(
    EDD_UPPER_RQB_PTR_TYPE      pRQB
);
#endif

/*=============================================================================
 * function name:  EDDP_MEMSET_COMMON
 *
 * function:       fills memory with value
 *
 * parameters:      EDD_COMMON_MEM_PTR_TYPE        pMem
 *                  LSA_UINT8                      Value
 *                  LSA_UINT32                     Length
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_MEMSET_COMMON
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_MEMSET_COMMON(
    EDD_COMMON_MEM_PTR_TYPE         pMem,                       
    LSA_UINT8                       Value,
    LSA_UINT32                      Length
);
#endif

/*=============================================================================
 * function name:  EDDP_MEMCOPY_COMMON
 *
 * function:       copy Length bytes memory from pSrc to pDst.
 *
 * parameters:      EDD_UPPER_MEM_PTR_TYPE         pDst
 *                  EDD_UPPER_MEM_PTR_TYPE         pSrc
 *                  LSA_UINT32                     Length
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_MEMCOPY_COMMON
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_MEMCOPY_COMMON(
    EDD_COMMON_MEM_PTR_TYPE         pDst,                       
    EDD_COMMON_MEM_PTR_TYPE         pSrc,                       
    LSA_UINT32                      Length
);
#endif

/*=============================================================================
 * function name:  EDDP_MEMCMP_COMMON
 *
 * function:       compares memory of pBuf1 with memory of pBuf2.
 *
 * parameters:      EDD_UPPER_MEM_PTR_TYPE         pBuf1
 *                  EDD_UPPER_MEM_PTR_TYPE         pBuf2
 *                  LSA_UINT32                     Length
 *
 * return value:        !=0 : buffers are the same
 *                        0: buffers differ
 *===========================================================================*/
#ifndef EDDP_MEMCMP_COMMON
LSA_UINT8 EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_MEMCMP_COMMON(
    EDD_COMMON_MEM_PTR_TYPE         pBuf1,                       
    EDD_COMMON_MEM_PTR_TYPE         pBuf2,                       
    LSA_UINT32                      Length
);
#endif

/*=============================================================================
 * function name:  EDDP_ALLOC_UPPER_RQB_LOCAL
 *
 * function:       allocate an upper-RQB for local use 
 *                 the memory has to be initialized with 0  
 *
 * parameters:     EDD_UPPER_RQB_PTR_TYPE  ...  *  upper_rqb_ptr_ptr:
 *                                   return value: pointer to local-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: No memory 
 *                                                 available
 *                 LSA_UINT16                      length:   length of RQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_ALLOC_UPPER_RQB_LOCAL
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_ALLOC_UPPER_RQB_LOCAL(
    EDD_UPPER_RQB_PTR_TYPE  EDDP_LOCAL_MEM_ATTR *   upper_rqb_ptr_ptr,
    LSA_UINT16                                      length
);
#endif

/*=============================================================================
 * function name:  EDDP_FREE_UPPER_RQB_LOCAL
 *
 * function:       free an upper-RQB allocated with EDDP_ALLOC_UPPER_RQB_LOCAL
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      upper-RQB or
 *                 EDD_UPPER_RQB_PTR_TYPE  upper_rqb_ptr:  pointer to upper-
 *                                                            RQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_FREE_UPPER_RQB_LOCAL
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_FREE_UPPER_RQB_LOCAL(
    LSA_UINT16              EDDP_LOCAL_MEM_ATTR *   ret_val_ptr,
    EDD_UPPER_RQB_PTR_TYPE                          upper_rqb_ptr
);
#endif

/*=============================================================================
 * function name:  EDDP_ALLOC_UPPER_MEM_LOCAL
 *
 * function:       allocate an upper-mem for local use 
 *                 the memory has to be initialized with 0  
 *
 * parameters:     EDD_UPPER_MEM_PTR_TYPE  ...  *  upper_mem_ptr_ptr:
 *                                   return value: pointer to local-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: No memory 
 *                                                 available
 *                 LSA_UINT16                      length:   length of memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_ALLOC_UPPER_MEM_LOCAL
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_ALLOC_UPPER_MEM_LOCAL(
    EDD_UPPER_MEM_PTR_TYPE  EDDP_LOCAL_MEM_ATTR *   upper_mem_ptr_ptr,
    LSA_UINT16                                      length
);
#endif

/*=============================================================================
 * function name:  EDDP_FREE_UPPER_MEM_LOCAL
 *
 * function:       free an upper-mem allocated with EDDP_ALLOC_UPPER_MEM_LOCAL
 *
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      upper-mem or
 *                 EDD_UPPER_MEM_PTR_TYPE  upper_mem_ptr:  pointer to upper-
 *                                                         memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_FREE_UPPER_MEM_LOCAL
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_FREE_UPPER_MEM_LOCAL(
    LSA_UINT16              EDDP_LOCAL_MEM_ATTR *   ret_val_ptr,
    EDD_UPPER_MEM_PTR_TYPE                          upper_mem_ptr
);
#endif

/*=============================================================================
 * function name:  EDDP_ALLOC_UPPER_RX_MEM
 *
 * function:       allocate memory for receive transfer data which
 *                 must be accesseable by the pnip (most likley 
 *                 it must be an uncached area). The memory resides in upper 
 *                 memory-space. 
 *
 *                 The memory has to be at least 32Bit aligned!!!
 *
 * parameters:     EDDP_SYS_HANDLE                 hSysDev
 *                 EDD_UPPER_MEM_PTR_TYPE  ...  *  upper_mem_ptr_ptr:
 *                                   return value: pointer to upper-transfer-
 *                                                 memory-pointer
 *                                                 or LSA_NULL: No memory 
 *                                                 available
 *                 LSA_UINT32                      length: length of memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_ALLOC_UPPER_RX_MEM
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_ALLOC_UPPER_RX_MEM(
    EDDP_SYS_HANDLE                                 hSysDev,
    EDD_UPPER_MEM_PTR_TYPE  EDDP_LOCAL_MEM_ATTR *   upper_mem_ptr_ptr,
    LSA_UINT32                                      length
);
#endif

/*=============================================================================
 * function name:  EDDP_FREE_UPPER_RX_MEM
 *
 * function:       frees rx memory (from user or allocated with 
 *                 EDDP_ALLOC_RX_MEM)
 *
 * parameters:     EDDP_SYS_HANDLE            hSysDev
 *                          ...            *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      local-memory
 *                 PREFIX_UPPER_MEM_PTR_TYPE  upper_mem_ptr:  pointer to upper-
 *                                                            transfer memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_FREE_UPPER_RX_MEM
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_FREE_UPPER_RX_MEM(
    EDDP_SYS_HANDLE                                 hSysDev,
    LSA_UINT16              EDDP_LOCAL_MEM_ATTR *   ret_val_ptr,
    EDD_UPPER_MEM_PTR_TYPE                          upper_mem_ptr
);
#endif

/*=============================================================================
 * function name:  EDDP_ALLOC_UPPER_TX_MEM
 *
 * function:       allocate memory for transmit transfer data which
 *                 must be accesseable by the pnip (most likley 
 *                 it must be an uncached area). The memory resides in upper 
 *                 memory-space. 
 *
 *                 The memory has to be at least 32Bit aligned!!!
 *
 * parameters:     EDDP_SYS_HANDLE                 hSysDev
 *                 EDD_UPPER_MEM_PTR_TYPE  ...  *  upper_mem_ptr_ptr:
 *                                   return value: pointer to upper-transfer-
 *                                                 memory-pointer
 *                                                 or LSA_NULL: No memory 
 *                                                 available
 *                 LSA_UINT32                      length: length of memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_ALLOC_UPPER_TX_MEM
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_ALLOC_UPPER_TX_MEM(
    EDDP_SYS_HANDLE                                 hSysDev,
    EDD_UPPER_MEM_PTR_TYPE  EDDP_LOCAL_MEM_ATTR *   upper_mem_ptr_ptr,
    LSA_UINT32                                      length
);
#endif

/*=============================================================================
 * function name:  EDDP_FREE_UPPER_TX_MEM
 *
 * function:       frees rx memory EDDP_ALLOC_TX_MEM
 *
 * parameters:     EDDP_SYS_HANDLE            hSysDev
 *                          ...            *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      local-memory
 *                 PREFIX_UPPER_MEM_PTR_TYPE  upper_mem_ptr:  pointer to upper-
 *                                                            transfer memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_FREE_UPPER_TX_MEM
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_FREE_UPPER_TX_MEM(
    EDDP_SYS_HANDLE                                 hSysDev,
    LSA_UINT16              EDDP_LOCAL_MEM_ATTR *   ret_val_ptr,
    EDD_UPPER_MEM_PTR_TYPE                          upper_mem_ptr
);
#endif

/*=============================================================================
 * function name:  EDDP_ALLOC_UPPER_DEV_MEM
 *
 * function:       allocate memory for device management which
 *                 must be accesseable by the pnip (most likley 
 *                 it must be an uncached area). The memory resides in upper 
 *                 memory-space. 
 *
 *                 The memory has to be at least 32Bit aligned!!!
 *
 * parameters:     EDDP_SYS_HANDLE                 hSysDev
 *                 EDD_UPPER_MEM_PTR_TYPE  ...  *  upper_mem_ptr_ptr:
 *                                   return value: pointer to upper-device-
 *                                                 memory-pointer
 *                                                 or LSA_NULL: No memory 
 *                                                 available
 *                 LSA_UINT32                      length: length of memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_ALLOC_UPPER_DEV_MEM
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_ALLOC_UPPER_DEV_MEM(
    EDDP_SYS_HANDLE                                 hSysDev,
    EDD_UPPER_MEM_PTR_TYPE  EDDP_LOCAL_MEM_ATTR *   upper_mem_ptr_ptr,
    LSA_UINT32                                      length
);
#endif

/*=============================================================================
 * function name:  EDDP_FREE_UPPER_DEV_MEM
 *
 * function:       frees rx memory EDDP_ALLOC_DEV_MEM
 *
 * parameters:     EDDP_SYS_HANDLE            hSysDev
 *                          ...            *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      local-memory
 *                 PREFIX_UPPER_MEM_PTR_TYPE  upper_mem_ptr:  pointer to upper-
 *                                                            transfer memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_FREE_UPPER_DEV_MEM
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_FREE_UPPER_DEV_MEM(
    EDDP_SYS_HANDLE                                 hSysDev,
    LSA_UINT16              EDDP_LOCAL_MEM_ATTR *   ret_val_ptr,
    EDD_UPPER_MEM_PTR_TYPE                          upper_mem_ptr
);
#endif

/*=============================================================================
 * function name:   EDDP_RX_MEM_ADDR_UPPER_TO_PNIP
 *
 * function:        convert upper rx buffer pointer to 32-Bit physical pnip 
 *                  address
 *
 * parameters:      EDDP_SYS_HANDLE                hSysDev
 *                  EDD_UPPER_MEM_PTR_TYPE         Ptr
 *                  LSA_UINT32 EDDP_LOCAL_MEM_ATTR *pAddr
 *
 * return value:    LSA_VOID
 *===========================================================================*/
#ifndef EDDP_RX_MEM_ADDR_UPPER_TO_PNIP
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_RX_MEM_ADDR_UPPER_TO_PNIP(           
    EDDP_SYS_HANDLE                                 hSysDev,
    EDD_UPPER_MEM_PTR_TYPE                          Ptr,
    LSA_UINT32              EDDP_LOCAL_MEM_ATTR *   pAddr
);                                                                  
#endif

/*=============================================================================
 * function name:   EDDP_TX_MEM_ADDR_UPPER_TO_PNIP
 *
 * function:        convert upper tx buffer pointer to 32-Bit physical pnip 
 *                  address
 *
 * parameters:      EDDP_SYS_HANDLE                hSysDev
 *                  EDD_UPPER_MEM_PTR_TYPE         Ptr
 *                  LSA_UINT32 EDDP_LOCAL_MEM_ATTR *pAddr
 *
 * return value:    LSA_VOID
 *===========================================================================*/
#ifndef EDDP_TX_MEM_ADDR_UPPER_TO_PNIP
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_TX_MEM_ADDR_UPPER_TO_PNIP(           
    EDDP_SYS_HANDLE                                 hSysDev,
    EDD_UPPER_MEM_PTR_TYPE                          Ptr,
    LSA_UINT32              EDDP_LOCAL_MEM_ATTR *   pAddr
);                                                                  
#endif

/*=============================================================================
 * function name:   EDDP_DEV_MEM_ADDR_UPPER_TO_PNIP
 *
 * function:        convert upper device memory pointer to 32-Bit physical pnip 
 *                  address
 *
 * parameters:      EDDP_SYS_HANDLE                hSysDev
 *                  EDD_UPPER_MEM_PTR_TYPE         Ptr
 *                  LSA_UINT32 EDDP_LOCAL_MEM_ATTR *pAddr
 *
 * return value:    LSA_VOID
 *===========================================================================*/
#ifndef EDDP_DEV_MEM_ADDR_UPPER_TO_PNIP
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_DEV_MEM_ADDR_UPPER_TO_PNIP(           
    EDDP_SYS_HANDLE                                 hSysDev,
    EDD_UPPER_MEM_PTR_TYPE                          Ptr,
    LSA_UINT32              EDDP_LOCAL_MEM_ATTR *   pAddr
);                                                                  
#endif

/*=============================================================================
 * function name:   EDDP_IO_EXTRAM_ADDR_UPPER_TO_PNIP
 *
 * function:        convert upper IO buffer (external RAM) pointer to 32-Bit 
 *                  physical pnip address
 *
 * parameters:      EDDP_SYS_HANDLE                hSysDev
 *                  EDD_UPPER_MEM_PTR_TYPE         Ptr
 *                  LSA_UINT32 EDDP_LOCAL_MEM_ATTR *pAddr
 *
 * return value:    LSA_VOID
 *===========================================================================*/
#ifndef EDDP_IO_EXTRAM_ADDR_UPPER_TO_PNIP
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_IO_EXTRAM_ADDR_UPPER_TO_PNIP(
    EDDP_SYS_HANDLE                                 hSysDev,
    EDD_UPPER_MEM_PTR_TYPE                          Ptr,
    LSA_UINT32              EDDP_LOCAL_MEM_ATTR *   pAddr
);
#endif

/*=============================================================================
 * function name:   EDDP_IO_PERIF_ADDR_UPPER_TO_PNIP
 *
 * function:        convert upper IO buffer (PERIF) pointer to 32-Bit 
 *                  physical pnip address
 *
 * parameters:      EDDP_SYS_HANDLE                hSysDev
 *                  EDD_UPPER_MEM_PTR_TYPE         Ptr
 *                  LSA_UINT32 EDDP_LOCAL_MEM_ATTR *pAddr
 *
 * return value:    LSA_VOID
 *===========================================================================*/
#ifndef EDDP_IO_PERIF_ADDR_UPPER_TO_PNIP
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_IO_PERIF_ADDR_UPPER_TO_PNIP(
    EDDP_SYS_HANDLE                                 hSysDev,
    EDD_UPPER_MEM_PTR_TYPE                          Ptr,
    LSA_UINT32              EDDP_LOCAL_MEM_ATTR *   pAddr
);
#endif


/*=============================================================================
 * function name:  EDDP_SWAP16
 *
 * function:       swaps a UINT16 value 
 *
 * parameters:     LSA_UINT16
 *
 * return value:   LSA_UINT16
 *===========================================================================*/

/* Note: will be implemented within EDD with general handling if not defined */
/*       by systemadaption.                                                  */      

/*=============================================================================
 * function name:  EDDP_SWAP32
 *
 * function:       swaps a UINT32 value 
 *
 * parameters:     LSA_UINT32
 *
 * return value:   LSA_UINT32
 *===========================================================================*/

/* Note: will be implemented within EDD with general handling if not defined */
/*       by systemadaption.                                                  */      

/*=============================================================================
 * function name:  EDDP_SWAP64
 *
 * function:       swaps a UINT64 value
 *
 * parameters:     LSA_UINT64
 *
 * return value:   LSA_UINT64
 *===========================================================================*/

/* Note: will be implemented within EDD with general handling if not defined */
/*       by systemadaption.                                                  */      

/*=============================================================================
 * function name:  EDDP_ENTER_INT
 *
 * function:       Protect sequences from being interrupted by ISR-context of
 *                 edd (eddp_interrupt()).
 *
 * parameters:     LSA_VOID
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_ENTER_INT
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_ENTER_INT(
    LSA_VOID
);
#endif

/*=============================================================================
 * function name:  EDDP_EXIT_INT
 *
 * function:       Finishes EDDP_ENTER_INT sequence.
 *
 * parameters:     LSA_VOID
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_EXIT_INT
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_EXIT_INT(
    LSA_VOID
);
#endif

/*=============================================================================
 * function name:  EDDP_ENTER_IO
 *
 * function:       This call identifies the start of a short protected area.
 *                 This call is used to protect write accesses to the provider DataStatus which may occur if the 
 *                 SysRed API and the EDDP try to change the DataStuatus at the same time.
 *
 * parameters:     LSA_VOID
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_ENTER_IO
LSA_VOID EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_ENTER_IO( 
    LSA_VOID 
);
#endif

/*=============================================================================
 * function name:  EDDP_EXIT_IO
 *
 * function:       This function unlocks the protected area that was protected by EDDP_ENTER_IO
 *
 * parameters:     LSA_VOID
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_EXIT_IO
LSA_VOID EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_EXIT_IO( 
    LSA_VOID
);
#endif

/*=============================================================================
 * function name:  EDDP_ENTER_I2C
 *
 * function:       Protect I2C sequences from being interrupted.
 *
 * parameters:     LSA_VOID
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_ENTER_I2C
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_ENTER_I2C(
    LSA_VOID
);
#endif

/*=============================================================================
 * function name:  EDDP_EXIT_I2C
 *
 * function:       Finishes EDDP_ENTER_I2C sequence.
 *
 * parameters:     LSA_VOID
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_EXIT_I2C
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_EXIT_I2C(
    LSA_VOID
);
#endif

/*=============================================================================
 * function name:  EDDP_I2C_SELECT
 *
 * function:       Selects an I2C port for the following I2C transfers.
 *
 * parameters:     ...
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_I2C_SELECT
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR 	EDDP_I2C_SELECT(
	LSA_UINT8 EDDP_LOCAL_MEM_ATTR  *  const  ret_val_ptr,
	EDDP_SYS_HANDLE					  const  hSysDev,
	LSA_UINT16                        const  PortId,
	LSA_UINT16          			  const  I2CMuxSelect
);
#endif

/*=============================================================================
 * function name:  EDDP_DO_TIMER_REQUEST
 *
 * function:       Does trigger a eddp_system() request with the specified RQB. 
 *                 This function is called within Edd timer handler to signal
 *                 an timer event to the main driver context. The 
 *                 systemadaption has to call eddp_system() with the specified
 *                 RQB. Before calling, the systemadaption may have to change
 *                 to taskstate. 
 *
 * parameters:     EDDP_SYS_HANDLE          hSysDev
 *                 EDD_UPPER_RQB_PTR_TYPE   pRQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_DO_TIMER_REQUEST
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_DO_TIMER_REQUEST(
    EDDP_SYS_HANDLE             hSysDev,
    EDD_UPPER_RQB_PTR_TYPE      pRQB
);
#endif

/*=============================================================================
 * function name:  EDDP_DO_INTERRUPT_REQUEST
 *
 * function:       Does trigger a eddp_system() request with the specified RQB. 
 *                 This function is called within Edd interrupt handler to signal
 *                 an interrupt event to the main driver context. The 
 *                 systemadaption has to call eddp_system() with the specified
 *                 RQB. Before calling, the systemadaption may have to change
 *                 to taskstate. 
 *
 * parameters:     EDDP_SYS_HANDLE          hSysDev
 *                 EDD_UPPER_RQB_PTR_TYPE   pRQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_DO_INTERRUPT_REQUEST
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_DO_INTERRUPT_REQUEST(
    EDDP_SYS_HANDLE             hSysDev,
    EDD_UPPER_RQB_PTR_TYPE      pRQB
);
#endif

/*=============================================================================
 * macro name   : EDDP_SIGNAL_SENDCLOCK_CHANGE
 * function     : Signal a change in the sendclock to the system adaption
 * parameters   : EDDP_SYS_HANDLE       hSysDev
 *                LSA_UINT32            CycleBaseFactor
 *                LSA_UINT8             Mode:   - EDDP_SENDCLOCK_CHANGE_MODE_STARTING
 *                                              - EDDP_SENDCLOCK_CHANGE_MODE_FINISHED
 * return value : no
 *===========================================================================*/
#ifndef EDDP_SIGNAL_SENDCLOCK_CHANGE
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_SIGNAL_SENDCLOCK_CHANGE( 
	EDDP_SYS_HANDLE     hSysDev,
	LSA_UINT32          CycleBaseFactor,
	LSA_UINT8           Mode);
#endif

/*=============================================================================
 * macro name   : EDDP_SIGNAL_TIME_SYNC_STATE_CHANGE
 * function     : Signal a change of the Time Sync State to the system adaption
 * parameters   : -
 * return value : no
 *===========================================================================*/
#ifndef EDDP_SIGNAL_TIME_SYNC_STATE_CHANGE
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_SIGNAL_TIME_SYNC_STATE_CHANGE();
#endif


/*=============================================================================
 * function name:  EDDP_WAIT_10_NS()
 *
 * function:       This function waits 10 ns * ticks_10ns.
 *
 * parameters:     EDDP_SYS_HANDLE      hSysDev
 *                 ticks_10ns           waiting-time in units of 10ns
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_WAIT_10_NS
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_WAIT_10_NS( EDDP_SYS_HANDLE  hSysDev,
                                                     LSA_UINT32       ticks_10ns );
#endif

/*****************************************************************************/
/*                                                                           */
/*                   Prototypes for PHY interface                            */
/*                                                                           */
/*****************************************************************************/

/*=============================================================================
 * function name:  EDDP_PHY_GetPhyAddress
 *
 * function:       This is a synchron outbound function which returns 
 *                 the PHY address (at the SMI interface) of the passed 
 *                 hardware port ID.
 *                 This function is called by the EDDP before any access 
 *                 to the PHYs is performed. 
 * NOTE:           Set OUIValue to zero (unknown) or to a specific value if the 
 *                 type of connected PHY is known. In this case the returned
 *                 PHY address will be right for the supported PHY drivers
 *                 in this package.
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_GetPhyAddress(
    EDDP_HANDLE			hDDB,
    EDDP_SYS_HANDLE     hSysDev,
    LSA_UINT32			HWPortID,
    LSA_UINT32          PHYAddressOffset,
    LSA_UINT32*         pPhyAddress,
    LSA_UINT32*         pOUI1RegAddress,
    LSA_UINT32*         pOUI2RegAddress,
    LSA_UINT32*         pOUIUnequalValue);

/*=============================================================================
 * function name:  EDDP_PHY_GetFunctionPointers
 *
 * function:       This is a synchron outbound function which returns 
 *                 a structure of function pointers that are provided by 
 *                 system adaption in order to control the PHYs. System 
 *                 adaption can select the right functions by evaluating the
 *                 OUIValue parameter (vendor register value).
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_GetFunctionPointers(
    EDDP_HANDLE			    hDDB,
    EDDP_SYS_HANDLE		    hSysDev,
    LSA_UINT32			    HWPortID,
    LSA_UINT32              OUIValue,
    EDDP_PHY_FCT_PTR_TYPE   pFunctionPointers,
    EDDP_PHY_TYPE      *    pPHYType);

/*===========================================================================*/
/*                          PHY NEC API CONFIG                               */
/*===========================================================================*/

/*=============================================================================
 * function name:  EDDP_PHY_NEC_CheckMAUType
 *
 * function:       This function checks if the given MAUType is supported by 
 *                 the hardware and returns the related Speed and Duplexity.
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NEC_CHECK_MAU_TYPE(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID,
    LSA_UINT16              MAUType,
    LSA_UINT32              *pSpeed,
    LSA_UINT32              *pDuplexity);

/*=============================================================================
 * function name:  EDDP_PHY_NEC_GET_MAU_TYPE
 *
 * function:       Get MAUType of a port for internal NEC PHY. 
 *
 * Note:           EDDP expects this function to execute with success.
 *                 Otherwise a fatal error must be generated.
 *===========================================================================*/
#ifndef EDDP_PHY_NEC_GET_MAU_TYPE
LSA_VOID EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NEC_GET_MAU_TYPE(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID,
    LSA_UINT32              Speed,
    LSA_UINT32              Duplexity,
    LSA_UINT16             *pMauType);
#endif

/*=============================================================================
 * function name:  EDDP_PHY_NEC_GET_MEDIA_TYPE
 *
 * function:       Get MediaType of a port for internal NEC PHY.
 *
 * Note:           EDDP expects this function to execute with success.
 *                 Otherwise a fatal error must be generated.
 *===========================================================================*/
#ifndef EDDP_PHY_NEC_GET_MEDIA_TYPE
LSA_VOID EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NEC_GET_MEDIA_TYPE(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID,
    LSA_UINT8               *pMediaType,
    LSA_UINT8               *pOpticalTransType,
    LSA_UINT8		        *pFxTransceiverType);
#endif


/*===========================================================================*/
/*                          PHY NSC API CONFIG                               */
/*===========================================================================*/

/*=============================================================================
 * function name:  EDDP_PHY_NSC_CheckMAUType
 *
 * function:       This function checks if the given MAUType is supported by 
 *                 the hardware and returns the related Speed and Duplexity.
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NSC_CHECK_MAU_TYPE(
    EDDP_HANDLE         hDDB,
    EDDP_SYS_HANDLE     hSysDev,
    LSA_UINT32          HwPortID,
    LSA_UINT16          MAUType,
    LSA_UINT32          *pSpeed,
    LSA_UINT32          *pDuplexity);

/*=============================================================================
 * function name:  EDDP_PHY_NSC_GET_MAU_TYPE
 *
 * function:       Get MAUType of a port for internal NSC PHY. 
 *
 * Note:           EDDP expects this function to execute with success.
 *                 Otherwise a fatal error must be generated.
 *===========================================================================*/
#ifndef EDDP_PHY_NSC_GET_MAU_TYPE
LSA_VOID EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NSC_GET_MAU_TYPE(
    EDDP_HANDLE         hDDB,
    EDDP_SYS_HANDLE     hSysDev,
    LSA_UINT32          HwPortID,
    LSA_UINT32          Speed,
    LSA_UINT32          Duplexity,
    LSA_UINT16          *pMauType);
#endif

/*=============================================================================
 * function name:  EDDP_PHY_NSC_GET_MEDIA_TYPE
 *
 * function:       Get MediaType of a port for internal NSC PHY.
 *
 * Note:           EDDP expects this function to execute with success.
 *                 Otherwise a fatal error must be generated.
 *===========================================================================*/
#ifndef EDDP_PHY_NSC_GET_MEDIA_TYPE
LSA_VOID EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NSC_GET_MEDIA_TYPE(
    EDDP_HANDLE         hDDB,
    EDDP_SYS_HANDLE     hSysDev,
    LSA_UINT32          HwPortID,
    LSA_UINT8           *pMediaType,
    LSA_UINT8           *pOpticalTransType,
    LSA_UINT8           *pFxTransceiverType);
#endif

/*===========================================================================*/
/*                          PHY BCM5461S API CONFIG                          */
/*===========================================================================*/

/*=============================================================================
 * function name:  EDDP_PHY_BCM5461S_CheckMAUType
 *
 * function:       This function checks if the given MAUType is supported by 
 *                 the hardware and returns the related Speed and Duplexity.
 *===========================================================================*/
#ifndef EDDP_PHY_BCM5461S_CHECK_MAU_TYPE
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_BCM5461S_CHECK_MAU_TYPE(
    EDDP_HANDLE         hDDB,
    EDDP_SYS_HANDLE     hSysDev,
    LSA_UINT32          HwPortID,
    LSA_UINT16          MAUType,
    LSA_UINT32          *pSpeed,
    LSA_UINT32          *pDuplexity);
#endif

/*=============================================================================
 * function name:  EDDP_PHY_BCM5461S_GET_MAU_TYPE
 *
 * function:       Get MAUType of a port for internal BCM5461S PHY. 
 *
 * Note:           EDDP expects this function to execute with success.
 *                 Otherwise a fatal error must be generated.
 *===========================================================================*/
#ifndef EDDP_PHY_BCM5461S_GET_MAU_TYPE
LSA_VOID EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_BCM5461S_GET_MAU_TYPE(
    EDDP_HANDLE         hDDB,
    EDDP_SYS_HANDLE     hSysDev,
    LSA_UINT32          HwPortID,
    LSA_UINT32          Speed,
    LSA_UINT32          Duplexity,
    LSA_UINT16          *pMauType);
#endif

/*=============================================================================
 * function name:  EDDP_PHY_BCM5461S_GET_MEDIA_TYPE
 *
 * function:       Get MediaType of a port for internal BCM5461S PHY.
 *
 * Note:           EDDP expects this function to execute with success.
 *                 Otherwise a fatal error must be generated.
 *===========================================================================*/
#ifndef EDDP_PHY_BCM5461S_GET_MEDIA_TYPE
LSA_VOID EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_BCM5461S_GET_MEDIA_TYPE(
    EDDP_HANDLE         hDDB,
    EDDP_SYS_HANDLE     hSysDev,
    LSA_UINT32          HwPortID,
    LSA_UINT8           *pMediaType,
    LSA_UINT8           *pOpticalTransType,
    LSA_UINT8           *pFxTransceiverType);
#endif

/*===========================================================================*/
/*                          PHY TI API CONFIG                                */
/*===========================================================================*/

/*=============================================================================
 * function name:  EDDP_PHY_TI_CheckMAUType
 *
 * function:       This function checks if the given MAUType is supported by 
 *                 the hardware and returns the related Speed and Duplexity.
 *===========================================================================*/
#ifndef EDDP_PHY_TI_CHECK_MAU_TYPE
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_TI_CHECK_MAU_TYPE(
    EDDP_HANDLE         hDDB,
    EDDP_SYS_HANDLE     hSysDev,
    LSA_UINT32          HwPortID,
    LSA_UINT16          MAUType,
    LSA_UINT32          *pSpeed,
    LSA_UINT32          *pDuplexity);
#endif

/*=============================================================================
 * function name:  EDDP_PHY_TI_GET_MAU_TYPE
 *
 * function:       Get MAUType of a port for internal TI PHY. 
 *
 * Note:           EDDP expects this function to execute with success.
 *                 Otherwise a fatal error must be generated.
 *===========================================================================*/
#ifndef EDDP_PHY_TI_GET_MAU_TYPE
LSA_VOID EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_TI_GET_MAU_TYPE(
    EDDP_HANDLE         hDDB,
    EDDP_SYS_HANDLE     hSysDev,
    LSA_UINT32          HwPortID,
    LSA_UINT32          Speed,
    LSA_UINT32          Duplexity,
    LSA_UINT16          *pMauType);
#endif

/*=============================================================================
 * function name:  EDDP_PHY_TI_GET_MEDIA_TYPE
 *
 * function:       Get MediaType of a port for internal TI PHY.
 *
 * Note:           EDDP expects this function to execute with success.
 *                 Otherwise a fatal error must be generated.
 *===========================================================================*/
#ifndef EDDP_PHY_TI_GET_MEDIA_TYPE
LSA_VOID EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_TI_GET_MEDIA_TYPE(
    EDDP_HANDLE         hDDB,
    EDDP_SYS_HANDLE     hSysDev,
    LSA_UINT32          HwPortID,
    LSA_UINT8           *pMediaType,
    LSA_UINT8           *pOpticalTransType,
    LSA_UINT8           *pFxTransceiverType);
#endif

/*=============================================================================
 * function name:  EDDP_K32_RESET
 *
 * function:       set/clear reset of KRISC32.
 *                 The TCM is not affected by this function.
 *
 * parameters:     LSA_UINT8on  reset should be set (LAS_TRUE)
 *                 or cleared (       0).
 *
 * return value:   none 
 *===========================================================================*/
#ifndef EDDP_K32_RESET
LSA_VOID EDDP_SYSTEM_OUT_FCT_ATTR EDDP_K32_RESET(
	EDDP_SYS_HANDLE         hSysDev,
	LSA_UINT8				on);
#endif

/*=============================================================================
 * function name:  EDDP_ERTEC200P_INIT_PHY_CONFIG_REG
 *
 * function:       Inits the PHY_CONFIG Register of the ERTEC200+ SCRB.
 *                 (SystemControlRegisterBlock, see ERTEC200+ description)
 *                 This function is called by EDDP on device setup before
 *                 the PHYs will be enabled. The systemadaption has to 
 *                 write the "InitValue" to the PHY_CONFIG Register of 
 *                 the ERTEC200+ so the ERTEC200+ Phy will startup with 
 *                 proper init parameters.
 *
 *                 Note: This configuration is only used with the ERTEC200+
 *                       internal PHY. With external PHYs it is not used and 
 *                       don�t care. With other hardware (SCRB-Register not 
 *                       present within the ASIC) it will not be called by the EDD. 
 *                 Note: This is only the powerup configuration and will be
 *                       reconfigured on PHY init within EDDP! The PHY have to be
 *                       enabled and shall be set in loopback mode.
 *
 *                 The values that shall be setup within PHY_CONFIG - see <EDDP_SYS.DOC>
 *
 * parameters:     EDDP_SYS_HANDLE          hSysDev
 *                 LSA_UINT8                Cmd,
 *                 LSA_UINT32               HWPortID
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_ERTEC200P_INIT_PHY_CONFIG_REG
LSA_VOID EDDP_SYSTEM_OUT_FCT_ATTR EDDP_ERTEC200P_INIT_PHY_CONFIG_REG(
    EDDP_SYS_HANDLE    hSysDev,
    LSA_UINT8		   Cmd,
	LSA_UINT32		   HWPortID
);

#define EDDP_PHY_CONFIG_CMD_INIT_ALL	    0
#define EDDP_PHY_CONFIG_CMD_ENABLE_PHY	    1
#define EDDP_PHY_CONFIG_CMD_DISABLE_PHY	    2
#define EDDP_PHY_CONFIG_CMD_SET_FX_MODE	    3
#define EDDP_PHY_CONFIG_CMD_SET_COPPER_MODE	4

#endif 
/*=============================================================================
 * macro name   : EDDP_NRT_SEND_HOOK
 * function     : The macro gets the RQB and can modify the framebuffer 
                  content referenced by pPuffer within the RQB. All 
                  parameters within the RQB shall not be changed! 
 * parameters   : EDDP_SYS_HANDLE           hSysDev
 *                EDD_UPPER_RQB_PTR_TYPE    pRQB
 * return value : no
 *===========================================================================*/
#ifndef EDDP_NRT_SEND_HOOK
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_NRT_SEND_HOOK( 
    EDDP_SYS_HANDLE         hSysDev,
    EDD_UPPER_RQB_PTR_TYPE  pRQB);
#endif

/*****************************************************************************/
/*  end of file EDDP_OUT.H                                                   */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_OUT_H */
