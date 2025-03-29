#ifndef POF_SYS_H                       /* ----- reinclude-protection ----- */
#define POF_SYS_H

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
/*  C o m p o n e n t     &C: POF (Polymer Optical Fiber)               :C&  */
/*                                                                           */
/*  F i l e               &F: pof_sys.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  System interface                                                         */
/*  Defines constants, types, macros and prototyping for POF                 */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

/*===========================================================================*/
/*                              constants                                    */
/*===========================================================================*/
//POF transceiver memory blocks
#define POF_ADR_PAGE_A0             (LSA_UINT8)0xA0     //serial bus 2 wire address for data information
#define POF_ADR_PAGE_A2             (LSA_UINT8)0xA2     //serial bus 2 wire address for diagnostic information

/*---------------------------------------------------------------------------*/
/* fatal-errorcodes (used for POFFatalError())                               */
/*---------------------------------------------------------------------------*/
#define POF_FATAL_ERROR_TYPE                LSA_UINT32

#define POF_FATAL_NO_ERROR                  (POF_FATAL_ERROR_TYPE) 0x01
#define POF_FATAL_ERR_NULL_PTR              (POF_FATAL_ERROR_TYPE) 0x02
#define POF_FATAL_ERR_MEM_FREE              (POF_FATAL_ERROR_TYPE) 0x03
#define POF_FATAL_ERR_GET_PATH_INFO         (POF_FATAL_ERROR_TYPE) 0x04
#define POF_FATAL_ERR_RELEASE_PATH_INFO     (POF_FATAL_ERROR_TYPE) 0x05
#define POF_FATAL_ERR_INCONSISTENZ          (POF_FATAL_ERROR_TYPE) 0x06
#define POF_FATAL_ERR_TIMER_RESOURCE        (POF_FATAL_ERROR_TYPE) 0x07
#define POF_FATAL_ERR_HANDLE_INVALID        (POF_FATAL_ERROR_TYPE) 0x08
#define POF_FATAL_ERR_EDD                   (POF_FATAL_ERROR_TYPE) 0x09
#define POF_FATAL_ERR_PRM_READ_RECORD_INDEX (POF_FATAL_ERROR_TYPE) 0x0A
#define POF_FATAL_ERR_PRM_DIAG_STATE        (POF_FATAL_ERROR_TYPE) 0x0B
#define POF_FATAL_ERR_HW                    (POF_FATAL_ERROR_TYPE) 0x0C
#define POF_FATAL_ERR_OPEN_CHANNEL          (POF_FATAL_ERROR_TYPE) 0x0D


/*===========================================================================*/
/*                                 defines                                   */
/*===========================================================================*/
#define POF_DETAIL_PTR_TYPE                 /* pointer to POF_DETAIL */         \
    struct  pof_detail_tag  POF_SYSTEM_MEM_ATTR  *

#define POF_FATAL_ERROR_PTR_TYPE            /* pointer to LSA_FATAL_ERROR */    \
    struct  lsa_fatal_error_tag  POF_LOCAL_MEM_ATTR  *


/*===========================================================================*/
/*                                  types                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                   CDB (Channel Description Block)                         */
/*                     (used by macro POF_GET_PATH_INFO)                     */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* CDB- Channel Description Block                                            */
/* (used as detail-ptr in macro POF_GET_PATH_INFO)                           */
/*                                                                           */
/*---------------------------------------------------------------------------*/
//selection of transceiver interface

typedef enum pof_i2c_available_tag
{
    POF_I2C_NOT_AVAILABLE  = 0,      /**< I2C not available for this port! */
    POF_I2C_AVAILABLE      = 1       /**< I2C available for this port! */
} POF_I2C_AVAILABLE_TYPE;

typedef struct pof_detail_tag
{
    POF_I2C_AVAILABLE_TYPE   I2CAvailable[EDD_CFG_MAX_PORT_CNT]; /**< Indicator of I2C is available or not for the port. Index=PortIndex=PortId-1 */
    LSA_UINT16               I2CMuxSelect[EDD_CFG_MAX_PORT_CNT]; /**< I2C MultiplexerSelect value for the port. Index=PortIndex=PortId-1 */
} POF_CDB_TYPE;

#define POF_PORT_CNT_MAX    0xFF    /* max port cnt supported (255) = PortId 1..255 */


/*===========================================================================*/
/*                                 macros                                    */
/*===========================================================================*/

/*===========================================================================*/
/*                              external functions                           */
/*===========================================================================*/

/*===========================================================================*/
/*                              local functions                              */
/*===========================================================================*/

/*====  in functions  =====*/

/*=============================================================================
 * function name:  pof_init()
 *
 * function:       initialize POF
 *
 * parameters:     
 *
 * return value:   LSA_UINT16:  LSA_RET_OK
 *                              possibly:
 *                              LSA_RET_ERR_PARAM
 *                              LSA_RET_ERR_RESOURCE
 *                              or others
 *===========================================================================*/
LSA_UINT16  POF_SYSTEM_IN_FCT_ATTR  pof_init(
    LSA_VOID
);

/*=============================================================================
 * function name:  pof_undo_init()
 *
 * function:       undo the initialization of POF
 *
 * parameters:     LSA_VOID
 *
 * return value:   LSA_UINT16:  LSA_RET_OK
 *                              possibly:
 *                              LSA_RET_ERR_SEQUENCE
 *                              or others
 *===========================================================================*/
LSA_UINT16  POF_SYSTEM_IN_FCT_ATTR  pof_undo_init(
    LSA_VOID
);

/*=============================================================================
 * function name:  pof_timeout()
 *
 * function:       expiration of the running time of a timer
 *
 * parameters:     LSA_UINT16        timer_id:  id of timer
 *                 LSA_USER_ID_TYPE  user_id:   id of prefix
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID  POF_SYSTEM_IN_FCT_ATTR  pof_timeout(
    LSA_TIMER_ID_TYPE  const  timer_id,
    LSA_USER_ID_TYPE   const  user_id
);

/*****************************************************************************/
/*                                                                           */
/* Output-functions/macros                                                   */
/*                                                                           */
/*****************************************************************************/

/*=============================================================================
 * function name:  POF_GET_PATH_INFO()
 *
 * function:       get system-pointer and detail-pointer of a communication 
 *                 channel
 *
 * parameters:     LSA_UINT16              ...  *  ret_val_ptr:
 *                                      return value: LSA_RET_OK
 *                                                    LSA_RET_ERR_SYS_PATH
 *                 LSA_SYS_PTR_TYPE        ...  *  sys_ptr_ptr:
 *                                      return value: pointer to system-pointer
 *                 POF_DETAIL_PTR_TYPE  ...  *  detail_ptr_ptr:
 *                                      return value: pointer to detail-pointer
 *                 LSA_SYS_PATH_TYPE               path: path of a
 *                                                       communication channel  
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef POF_GET_PATH_INFO
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_GET_PATH_INFO(
    LSA_UINT16           POF_LOCAL_MEM_ATTR  *  ret_val_ptr,
    LSA_SYS_PTR_TYPE     POF_LOCAL_MEM_ATTR  *  sys_ptr_ptr,
    POF_DETAIL_PTR_TYPE  POF_LOCAL_MEM_ATTR  *  detail_ptr_ptr,
    LSA_SYS_PATH_TYPE                           path
);
#endif

/*=============================================================================
 * function name:  POF_RELEASE_PATH_INFO()
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
#ifndef POF_RELEASE_PATH_INFO
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_RELEASE_PATH_INFO(
    LSA_UINT16           POF_LOCAL_MEM_ATTR  *  ret_val_ptr,
    LSA_SYS_PTR_TYPE                            sys_ptr,
    POF_DETAIL_PTR_TYPE                         detail_ptr
);
#endif

/*=============================================================================
 * function name:  POF_ALLOC_TIMER()
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
#ifndef POF_ALLOC_TIMER
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_ALLOC_TIMER(
    LSA_UINT16         POF_LOCAL_MEM_ATTR  *  ret_val_ptr,
    LSA_TIMER_ID_TYPE  POF_LOCAL_MEM_ATTR  *  timer_id_ptr,
    LSA_UINT16                                timer_type,
    LSA_UINT16                                time_base
);
#endif

/*=============================================================================
 * function name:  POF_START_TIMER()
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
 *                 LSA_USER_ID_TYPE    user_id:   id of prefix
 *                 LSA_UINT16          time:      running time
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef POF_START_TIMER
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_START_TIMER(
    LSA_UINT16         POF_LOCAL_MEM_ATTR  *  ret_val_ptr,
    LSA_TIMER_ID_TYPE                         timer_id,
    LSA_USER_ID_TYPE                          user_id,
    LSA_UINT16                                time
);
#endif

/*=============================================================================
 * function name:  POF_STOP_TIMER()
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
#ifndef POF_STOP_TIMER
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_STOP_TIMER(
    LSA_UINT16         POF_LOCAL_MEM_ATTR  *  ret_val_ptr,
    LSA_TIMER_ID_TYPE                         timer_id
);
#endif

/*=============================================================================
 * function name:  POF_FREE_TIMER()
 *
 * function:       free a timer
 *
 * parameters:     LSA_UINT16  ...  *  ret_val_ptr:
 *                 return value: LSA_RET_OK                    timer has been
 *                                                             deallocated
 *                               LSA_RET_ERR_TIMER_IS_RUNNING  because timer is
 *                                                             running timer has
 *                                                             not been
 *                                                             deallocated
 *                               LSA_RET_ERR_PARAM             no deallocation
 *                                                             because of wrong
 *                                                             timer-id
 *                 LSA_UINT16    timer_id:  id of timer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef POF_FREE_TIMER
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_FREE_TIMER(
    LSA_UINT16         POF_LOCAL_MEM_ATTR  *  ret_val_ptr,
    LSA_TIMER_ID_TYPE                         timer_id
);
#endif

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
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR POF_WAIT_US(LSA_UINT16 Count);
#endif

/*=============================================================================
 * function name:  POF_FATAL_ERROR()
 *
 * function:       notify a fatal-error
 *
 * parameters:     LSA_UINT16                   length:     number of bytes
 *                 POF_FATAL_ERROR_PTR_TYPE  error_ptr:  pointer to
 *                                                          LSA_FATAL_ERROR
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef POF_FATAL_ERROR
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_FATAL_ERROR(
    LSA_UINT16                length,
    POF_FATAL_ERROR_PTR_TYPE  error_ptr
);
#endif

/*=============================================================================
 * function name:  POF_ENTER()
 *
 * function:       Protect sequence within POF from beeing interrupted.
 *                 (reentrance protection) 
 *
 * parameters:     LSA_VOID
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef POF_ENTER
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_ENTER(
    LSA_VOID
);
#endif

/*=============================================================================
 * function name:  POF_EXIT()
 *
 * function:       Finishes POF_ENTER-Sequence.
 *
 * parameters:     LSA_VOID
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef POF_EXIT
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_EXIT(
    LSA_VOID
);
#endif

/*=============================================================================
 * function name:  POF_ALLOC_LOCAL_MEM()
 *
 * function:       allocate a local-memory
 *
 * parameters:     POF_LOCAL_MEM_PTR_TYPE  ...  *  local_mem_ptr_ptr:
 *                                   return value: pointer to local-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: No memory 
 *                                                 available
 *                 LSA_UINT32                      length: length of memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef POF_ALLOC_LOCAL_MEM
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_ALLOC_LOCAL_MEM(
    POF_LOCAL_MEM_PTR_TYPE  POF_LOCAL_MEM_ATTR  *  local_mem_ptr_ptr,
    LSA_UINT32                                     length
);
#endif

/*=============================================================================
 * function name:  POF_FREE_LOCAL_MEM()
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
#ifndef POF_FREE_LOCAL_MEM
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_FREE_LOCAL_MEM(
    LSA_UINT16              POF_LOCAL_MEM_ATTR  *  ret_val_ptr,
    POF_LOCAL_MEM_PTR_TYPE                         local_mem_ptr
);
#endif

/*=============================================================================
 * function name:  POF_RQB_ERROR()
 *
 * function:       notify a RQB-error in a user/system-request. 
 *                 this function is called if there is an error (i.e. param) 
 *                 within RQB which prevents notification of the caller 
 *                 (i.e. missing call-back-function). the error-code is set
 *                 in "Status" in RQB. this error is typically an implementation
 *                 error.
 *
 * parameters:     POF_UPPER_RQB_PTR_TYPE   pRQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef POF_RQB_ERROR
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_RQB_ERROR(
    POF_UPPER_RQB_PTR_TYPE  pRQB
);
#endif

/*=============================================================================
 * function name:  POF_MEMSET_LOCAL()
 *
 * function:       fills memory with value
 *
 * parameters:     POF_LOCAL_MEM_PTR_TYPE         pMem
 *                 LSA_UINT8                      Value
 *                 LSA_UINT32                     Length
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef POF_MEMSET_LOCAL
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_MEMSET_LOCAL(
    POF_LOCAL_MEM_PTR_TYPE  pMem,                       
    LSA_UINT8               Value,
    LSA_UINT32              Length
);
#endif

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
);
#endif

/*=============================================================================
 * function name:  POF_MEMCOPY_COMMON()
 *
 * function:       copy Length bytes memory from pSrc to pDst.
 *
 * parameters:     POF_COMMON_MEM_PTR_TYPE        pDst
 *                 POF_COMMON_MEM_PTR_TYPE        pSrc
 *                 LSA_UINT32                     Length
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef POF_MEMCOPY_COMMON
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_MEMCOPY_COMMON(
    POF_COMMON_MEM_PTR_TYPE  pDst,                       
    POF_COMMON_MEM_PTR_TYPE  pSrc,                       
    LSA_UINT32               Length
);
#endif

/*=============================================================================
 * function name:  POF_ALLOC_UPPER_RQB_LOCAL()
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
#ifndef POF_ALLOC_UPPER_RQB_LOCAL
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_ALLOC_UPPER_RQB_LOCAL(
    POF_UPPER_RQB_PTR_TYPE  POF_LOCAL_MEM_ATTR  *  upper_rqb_ptr_ptr,
    LSA_UINT16                                     length
);
#endif

/*=============================================================================
 * function name:  POF_FREE_UPPER_RQB_LOCAL()
 *
 * function:       free an upper-RQB allocated with POF_ALLOC_UPPER_RQB_LOCAL
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      upper-RQB or
 *                 POF_UPPER_RQB_PTR_TYPE  upper_rqb_ptr:  pointer to upper-RQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef POF_FREE_UPPER_RQB_LOCAL
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_FREE_UPPER_RQB_LOCAL(
    LSA_UINT16              POF_LOCAL_MEM_ATTR  *  ret_val_ptr,
    POF_UPPER_RQB_PTR_TYPE                         upper_rqb_ptr
);
#endif

/*=============================================================================
 * function name:  POF_DO_TIMER_REQUEST()
 *
 * function:       Does a POF_system() request with the specified RQB. This
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
#ifndef POF_DO_TIMER_REQUEST
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_DO_TIMER_REQUEST(
    POF_UPPER_RQB_PTR_TYPE  pRQB
);
#endif

/*=============================================================================
 * function name:  POF_LOG_10()
 *
 * function:       Does a POF_system() request with the specified RQB. This
 *                 function is called within POF timer handler to signal
 *                 a Timer event to the main context of POF.  
 *                 Systemadaption has to call POF_system() with the specified
 *                 RQB. Before calling, the systemadaption should change to 
 *                 taskstate. 
 *                 Logarithm to basis 10, also called "common logarithm"
 *
 * parameters:     ...
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef POF_LOG_10
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_LOG_10(
    LSA_UINT32                      const  Arg_in_ppm,
    LSA_INT32   POF_LOCAL_MEM_ATTR      *  pResult_in_ppm /* fixed-point with 6 decimal places */
);
#endif

/*=============================================================================
 * function name:  POF_POW_10()
 *
 * function:       Does a POF_system() request with the specified RQB. This
 *                 function is called within POF timer handler to signal
 *                 a Timer event to the main context of POF.  
 *                 Systemadaption has to call POF_system() with the specified
 *                 RQB. Before calling, the systemadaption should change to 
 *                 taskstate. 
 *                 Logarithm to basis 10, also called "common logarithm"
 *
 * parameters:     ...
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef POF_POW_10
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_POW_10(
    LSA_INT16                   const  numerator, 
    LSA_UINT16                  const  denominator, 
    POF_LOCAL_MEM_U32_PTR_TYPE         pResult 
);
#endif

/*=============================================================================
 * function name:  POF_FO_LED()
 *
 * function:       Does a POF_system() request with the specified RQB. This
 *                 function is called within POF timer handler to signal
 *                 a Timer event to the main context of POF.  
 *                 Systemadaption has to call POF_system() with the specified
 *                 RQB. Before calling, the systemadaption should change to 
 *                 taskstate. 
 *                 Logarithm to basis 10, also called "common logarithm"
 *
 * parameters:     ...
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#define POF_SYS_FO_LED_OFF  0
#define POF_SYS_FO_LED_ON   1
#ifndef POF_FO_LED
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_FO_LED(
    LSA_UINT16        const  PortId,
    LSA_UINT8         const  ON,
    LSA_SYS_PTR_TYPE         sys_ptr
);
#endif

/*============================================================================= 
 * function name:  POF_REQUEST_UPPER_DONE()
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
#ifndef POF_REQUEST_UPPER_DONE
LSA_VOID  POF_SYSTEM_OUT_FCT_ATTR  POF_REQUEST_UPPER_DONE(
    POF_UPPER_CALLBACK_FCT_PTR_TYPE  Cbf,
    POF_UPPER_RQB_PTR_TYPE           upper_rqb_ptr,
    LSA_SYS_PTR_TYPE                 sys_ptr
);
#endif

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
    LSA_UINT32	               		   const  PortId,
    LSA_UINT16			   		       const  I2CMuxSelect,
    LSA_UINT8			   		       const  I2CDevAddr,
    LSA_UINT8			   		       const  I2COffsetCnt,
    LSA_UINT8			   		       const  I2COffset1,
    LSA_UINT8			   		       const  I2COffset2,
    LSA_UINT32                     	   const  Size,
    LSA_UINT8  EDD_UPPER_MEM_ATTR   *  const  pBuf
);
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
    LSA_UINT32	               		   const  PortId,
    LSA_UINT16			   		       const  I2CMuxSelect,
    LSA_UINT8			   		       const  I2CDevAddr,
    LSA_UINT8			   		       const  I2COffsetCnt,
    LSA_UINT8			   		       const  I2COffset1,
    LSA_UINT8			   		       const  I2COffset2,
    LSA_UINT32                     	   const  Size,
    LSA_UINT8  EDD_UPPER_MEM_ATTR   *  const  pBuf
);
#endif


/*****************************************************************************/
/*  end of file pof_sys.h                                                    */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of POF_SYS_H */
