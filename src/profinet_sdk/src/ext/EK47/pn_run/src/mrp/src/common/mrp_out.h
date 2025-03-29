#ifndef MRP_OUT_H
#define MRP_OUT_H

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
/*  C o m p o n e n t     &C: MRP (Media Redundancy Protocol)           :C&  */
/*                                                                           */
/*  F i l e               &F: mrp_out.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  system interface                                                         */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who         What                                             */
#ifdef MRP_MESSAGE
/*  2010-07-21  ds    new encapsulation macro for upper layer callbacks      */
/*                                                                           */
#endif
/*****************************************************************************/


/*===========================================================================*/
/*                                prototyping                                */
/*===========================================================================*/



/*=============================================================================
 * function name:  MRP_ALLOC_UPPER_RQB_LOCAL
 *
 * function:       allocate an upper-RQB for local use 
 *
 * parameters:     MRP_UPPER_RQB_PTR_TYPE  ...  *  upper_rqb_ptr_ptr:
 *                                   return value: pointer to local-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: No memory 
 *                                                 available
 *                 LSA_UINT16                      length:   length of RQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef MRP_ALLOC_UPPER_RQB_LOCAL
LSA_VOID   MRP_ALLOC_UPPER_RQB_LOCAL(
    MRP_UPPER_RQB_PTR_TYPE    *upper_rqb_ptr_ptr,
    LSA_UINT16                length
);
#endif


/*=============================================================================
 * function name:  MRP_FREE_UPPER_RQB_LOCAL
 *
 * function:       free an upper-RQB allocated with MRP_ALLOC_UPPER_RQB_LOCAL
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      upper-RQB or
 *                 MRP_UPPER_RQB_PTR_TYPE  upper_rqb_ptr:  pointer to upper-
 *                                                            RQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef MRP_FREE_UPPER_RQB_LOCAL
LSA_VOID   MRP_FREE_UPPER_RQB_LOCAL(
    LSA_UINT16                  *ret_val_ptr,
    MRP_UPPER_RQB_PTR_TYPE      upper_rqb_ptr
);
#endif


/*=============================================================================
 * function name:  MRP_ALLOC_LOCAL_MEM
 *
 * function:       allocate a local-memory
 *
 * parameters:     MRP_LOCAL_MEM_PTR_TYPE  ...  *  local_mem_ptr_ptr:
 *                                   return value: pointer to local-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: No memory
 *                                                 available
 *                 LSA_UINT16                         length: length of memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef MRP_ALLOC_LOCAL_MEM
LSA_VOID   MRP_ALLOC_LOCAL_MEM(
    MRP_LOCAL_MEM_PTR_TYPE    *local_mem_ptr_ptr,
    LSA_UINT16                length
);
#endif


/*=============================================================================
 * function name:  MRP_FREE_LOCAL_MEM
 *
 * function:       free a local-memory
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      local-memory
 *                 MRP_LOCAL_MEM_PTR_TYPE  local_mem_ptr:  pointer to local-
 *                                                            memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef MRP_FREE_LOCAL_MEM
LSA_VOID   MRP_FREE_LOCAL_MEM(
    LSA_UINT16                    *ret_val_ptr,
    MRP_LOCAL_MEM_PTR_TYPE        local_mem_ptr
);
#endif


/*=============================================================================
 * function name:  MRP_REQUEST_LOCAL     
 *
 * function:       Does a mrp_request() with the specified RQB. This
 *                 function is called within mrp_timeout handler to signal
 *                 a timer event to the main context of MRP.  
 *                 System adaption has to call mrp_request() with the specified
 *                 RQB. Before calling, the system adaption should change to 
 *                 task state. 
 *
 * parameters:     MRP_UPPER_RQB_PTR_TYPE   pRQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/

#ifndef MRP_REQUEST_LOCAL
LSA_VOID   MRP_REQUEST_LOCAL(
    MRP_UPPER_RQB_PTR_TYPE  pRQB
);
#endif

/*============================================================================= 
* function name:  MRP_REQUEST_UPPER_DONE 
* 
* function:       returns a finished request to the upper layer 
* 
* parameters:     MRP_UPPER_CALLBACK_FCT_PTR_TYPE mrp_request_upper_done_ptr: 
*                     pointer to mrp_request_upper_done_ptr, given by mrp_open_channel() 
*                 MRP_UPPER_RQB_PTR_TYPE  upper_rqb_ptr:  pointer to upper RQB 
*                 LSA_SYS_PTR_TYPE           sys_ptr:     system-pointer
* 
* return value:   LSA_VOID 
* 
*===========================================================================*/
#ifndef MRP_REQUEST_UPPER_DONE
LSA_VOID   MRP_REQUEST_UPPER_DONE(    
  MRP_UPPER_CALLBACK_FCT_PTR_TYPE mrp_request_upper_done_ptr,    
  MRP_UPPER_RQB_PTR_TYPE          upper_rqb_ptr,    
  LSA_SYS_PTR_TYPE                sys_ptr 
  );
#endif

/*=============================================================================
 * function name:  MRP_GET_PATH_INFO
 *
 * function:       get system-pointer and detail-pointer of a communication
 *                 channel
 *
 * parameters:     LSA_UINT16              ...  *  ret_val_ptr:
 *                                      return value: LSA_RET_OK
 *                                                    LSA_RET_ERR_SYS_PATH
 *                 LSA_SYS_PTR_TYPE        ...  *  sys_ptr_ptr:
 *                                      return value: pointer to system-pointer
 *                 MRP_DETAIL_PTR_TYPE  ...  *  detail_ptr_ptr:
 *                                      return value: pointer to detail-pointer
 *                 LSA_SYS_PATH_TYPE               path: path of a
 *                                                       communication channel
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef MRP_GET_PATH_INFO
LSA_VOID MRP_GET_PATH_INFO(
    LSA_UINT16                *ret_val_ptr,
    LSA_SYS_PTR_TYPE          *sys_ptr_ptr,
    MRP_DETAIL_PTR_TYPE       *detail_ptr_ptr,
    LSA_SYS_PATH_TYPE         path
);
#endif


/*=============================================================================
 * function name:  MRP_RELEASE_PATH_INFO
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
 *                 MRP_DETAIL_PTR_TYPE  detail_ptr:  detail-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef MRP_RELEASE_PATH_INFO
LSA_VOID   MRP_RELEASE_PATH_INFO(
    LSA_UINT16              *ret_val_ptr,
    LSA_SYS_PTR_TYPE        sys_ptr,
    MRP_DETAIL_PTR_TYPE     detail_ptr
);
#endif


/*=============================================================================*/
/*                             TIMER-FUNCTIONS                                 */
/*=============================================================================*/

/*=============================================================================
 * function name:  MRP_ALLOC_TIMER
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
#ifndef MRP_ALLOC_TIMER
LSA_VOID   MRP_ALLOC_TIMER(
    LSA_UINT16    *ret_val_ptr,
    LSA_UINT16    *timer_id_ptr,
    LSA_UINT16    timer_type,
    LSA_UINT16    time_base
);
#endif

/*=============================================================================
 * function name:  MRP_START_TIMER
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
 *                                 system will call edd_timeout().
 *                 LSA_UINT16          timer_id:  id of timer
 *                 LSA_USER_ID_TYPE    user_id:   id of prefix
 *                 LSA_UINT16          time:      running time
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef MRP_START_TIMER
LSA_VOID   MRP_START_TIMER(
    LSA_UINT16            *ret_val_ptr,
    LSA_UINT16            timer_id,
    LSA_USER_ID_TYPE      user_id,
    LSA_UINT16            time
);
#endif


/*=============================================================================
 * function name:  MRP_STOP_TIMER
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
#ifndef MRP_STOP_TIMER
LSA_VOID MRP_STOP_TIMER(
    LSA_UINT16      *ret_val_ptr,
    LSA_UINT16      timer_id
);
#endif


/*=============================================================================
 * function name:  MRP_FREE_TIMER
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
#ifndef MRP_FREE_TIMER
LSA_VOID  MRP_FREE_TIMER(
    LSA_UINT16    *ret_val_ptr,
    LSA_UINT16    timer_id
);
#endif


/*=============================================================================
 * function name:  MRP_FATAL_ERROR
 *
 * function:       notify a fatal-error
 *
 * parameters:     LSA_UINT16                length:     number of bytes
 *                 MRP_FATAL_ERROR_PTR_TYPE  error_ptr:  pointer to
 *                                                       LSA_FATAL_ERROR
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef MRP_FATAL_ERROR
/*lint -function(exit, MRP_FATAL_ERROR) */
LSA_VOID   MRP_FATAL_ERROR(
    LSA_UINT16                   length,
    MRP_FATAL_ERROR_PTR_TYPE     error_ptr
);
#endif




/*===========================================================================*/
/* non-LSA-predefined             prototyping                                */
/*===========================================================================*/

/*=============================================================================
 * function name:  MRP_MEMSET          
 *
 * function:       fills memory with value
 *
 * parameters:		MRP_LOCAL_MEM_PTR_TYPE         pMem
 *					LSA_UINT8                      Value
 *					LSA_UINT32                     Length
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef MRP_MEMSET     
LSA_VOID MRP_MEMSET(
  LSA_VOID_PTR_TYPE pMem,						
  LSA_UINT8			    Value,
  LSA_UINT32			  Length
);
#endif


/*=============================================================================
 * function name:  MRP_MEMCPY          
 *
 * function:       copy memory 
 *
 * parameters:		 MRP_LOCAL_MEM_PTR_TYPE pMemDst: destination pointer
 *                 MRP_LOCAL_MEM_PTR_TYPE pMemSrc: source pointer
 *                 LSA_INT length: size of bytes to copy
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef MRP_MEMCPY     
LSA_VOID MRP_MEMCPY(
  LSA_VOID_PTR_TYPE pMemDst,
  LSA_VOID_PTR_TYPE pMemSrc,
  LSA_UINT length
);
#endif


/*=============================================================================
 * function name:  MRP_MEMCMP          
 *
 * function:       cmp memory 
 *
 * parameters:      MRP_LOCAL_MEM_PTR_TYPE pBuf1: buffer 1
 *                  MRP_LOCAL_MEM_PTR_TYPE pBuf2: buffer 2
 *                  LSA_INT length: size of bytes to compare
 *
 * return value:   LSA_INT:  <0 = buffer 1 less than buffer 2
 *                            0 = buffer 1 identical to buffer 2
 *                           >0 = buffer 1 greater than buffer 2   
 *===========================================================================*/
#ifndef MRP_MEMCMP     
LSA_INT MRP_MEMCMP(
  LSA_VOID_PTR_TYPE pBuf1,
  LSA_VOID_PTR_TYPE pBuf2,
  LSA_UINT length
);
#endif


/*=============================================================================
 * function name:  MRP_HTON32          
 *
 * function:       convert value from host to network byte order 
 *
 * parameters:     LSA_UINT32 value: host byte order
 *
 * return value:   LSA_UINT32: value in network byte order
 *===========================================================================*/
#ifndef MRP_HTON32
LSA_UINT32 MRP_HTON32(
  LSA_UINT32 value
);
#endif


/*=============================================================================
 * function name:  MRP_NTOH32          
 *
 * function:       convert value from network to host byte order 
 *
 * parameters:     LSA_UINT32 value: network byte order
 *
 * return value:   LSA_UINT32: value in hoste byte order
 *===========================================================================*/
#ifndef MRP_NTOH32
LSA_UINT32 MRP_NTOH32(
  LSA_UINT32 value
);
#endif


/*=============================================================================
 * function name:  MRP_HTON16          
 *
 * function:       convert value from host to network byte order 
 *
 * parameters:     LSA_UINT16 value: host byte order
 *
 * return value:   LSA_UINT16: value in network byte order
 *===========================================================================*/
#ifndef MRP_HTON16
LSA_UINT16 MRP_HTON16(
  LSA_UINT16 value
);
#endif


/*=============================================================================
 * function name:  MRP_NTOH16          
 *
 * function:       convert value from network to host byte order 
 *
 * parameters:     LSA_UINT16 value: network byte order
 *
 * return value:   LSA_UINT16: value in hoste byte order
 *===========================================================================*/
#ifndef MRP_NTOH16
LSA_UINT16 MRP_NTOH16(
  LSA_UINT16 value
);
#endif


/*=============================================================================
 * function name:  MRP_PUT16_HTON          
 *
 * function:       put and convert from host byte order to network byte order 
 *                 without alignment requirements
 *
 * parameters:     LSA_VOID *base_ptr: 
 *                 LSA_INT offset:
 *                 LSA_UINT16 val:
 *
 * return value:   LSA_VOID
 *
 * important:      WE WILL IMPLEMENT variants for mem-attribs
 *                 when needed. All known platforms are "flat".
 *===========================================================================*/
#ifndef MRP_PUT16_HTON
LSA_VOID  MRP_PUT16_HTON(
  LSA_VOID    *base_ptr,
  LSA_INT     offset,
  LSA_UINT16  val
);
#endif


/*=============================================================================
 * function name:  MRP_GET16_NTOH          
 *
 * function:       get and convert from network byte order to host byte order
 *                 without alignment requirements
 *
 * parameters:     LSA_VOID *base_ptr: 
 *                 LSA_INT offset:
 *                 
 *
 * return value:   LSA_UINT16 value
 *
 * important:      WE WILL IMPLEMENT variants for mem-attribs
 *                 when needed. All known platforms are "flat".
 *===========================================================================*/
#ifndef MRP_GET16_NTOH
LSA_UINT16  MRP_GET16_NTOH( 
  LSA_VOID *base_ptr,
  LSA_INT   offset
);
#endif


/*=============================================================================
 * function name:  MRP_PUT32_HTON          
 *
 * function:       put and convert from host byte order to network byte order 
 *                 without alignment requirements
 *
 * parameters:     LSA_VOID *base_ptr: 
 *                 LSA_INT offset:
 *                 LSA_UINT16 val:
 *
 * return value:   LSA_VOID
 *
 * important:      WE WILL IMPLEMENT variants for mem-attribs
 *                 when needed. All known platforms are "flat".
 *===========================================================================*/
#ifndef MRP_PUT32_HTON
LSA_VOID  MRP_PUT32_HTON(
  LSA_VOID    *base_ptr,
  LSA_INT     offset,
  LSA_UINT32  val
);
#endif


/*=============================================================================
 * function name:  MRP_GET32_NTOH          
 *
 * function:       get and convert from network byte order to host byte order
 *                 without alignment requirements
 *
 * parameters:     LSA_VOID *base_ptr: 
 *                 LSA_INT offset:
 *                 
 *
 * return value:   LSA_UINT16 value
 *
 * important:      WE WILL IMPLEMENT variants for mem-attribs
 *                 when needed. All known platforms are "flat".
 *===========================================================================*/
#ifndef MRP_GET32_NTOH
LSA_UINT32  MRP_GET32_NTOH( 
  LSA_VOID *base_ptr,
  LSA_INT   offset
);
#endif


/*=============================================================================
 * function name:  MRP_GET_MS_CNT_VAL
 *
 * function:       get counter value from 1ms counter
 *
 * parameters:     LSA_UINT32*:  pointer to buffer for counter
 *
 * return value:   
 *===========================================================================*/
#ifndef MRP_GET_MS_CNT_VAL     
LSA_VOID MRP_GET_MS_CNT_VAL(
  LSA_UINT32 *counterVal
);
#endif


/*=============================================================================
 * function name:  MRP_ASSERT
 *
 * function:       in debug version prints message and aborts the program.
 *                 in release version define it (in mrp_cfg.h) to do nothing.
 *
 * parameters:     LSA_INT :  expression (including pointers) that evaluates 
 *                            to nonzero or 0
 *
 * return value:   
 *===========================================================================*/
#ifndef MRP_ASSERT     
LSA_VOID MRP_ASSERT(
  LSA_INT cond
);
#endif


#ifdef MRP_CFG_PLUGIN_2_HSR
/*=============================================================================
 * function name:  MRP_STBY_GET_MY_MAC_ADDR
 *
 * function:       get a special mac address for standby communication     
 *                 protocol. 
 *
 * parameters:     LSA_UINT8* :  array of 6 byte for mac address 
 *
 * return value:   
 *===========================================================================*/
#ifndef MRP_STBY_GET_MY_MAC_ADDR
LSA_VOID MRP_STBY_GET_MY_MAC_ADDR(
  LSA_UINT8 *macAddr
);
#endif

/*=============================================================================
 * function name:  MRP_STBY_GET_PHYS_RUNNING
 *
 * function:       returns if phys running. 
 *
 * parameters:     LSA_TRUE, LSA_FALSE
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef MRP_STBY_GET_PHYS_RUNNING
LSA_VOID MRP_STBY_GET_PHYS_RUNNING(
  LSA_BOOL *phyStatus
);
#endif
#endif /* MRP_CFG_PLUGIN_2_HSR */

/*----------------------------------------------------------------------------*/
#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif  /* of MRP_OUT_H */
