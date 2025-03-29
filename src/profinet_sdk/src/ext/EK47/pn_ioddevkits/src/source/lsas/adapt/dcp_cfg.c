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
/*  F i l e               &F: dcp_cfg.c                                 :F&  */
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


/*****************************************************************************/
/* contents:

    - internal functions

        - DCP_ALLOC_UPPER_RQB
        - DCP_FREE_UPPER_RQB
        - DCP_ALLOC_UPPER_MEM
        - DCP_FREE_UPPER_MEM
        - DCP_GET_PATH_INFO
        - DCP_RELEASE_PATH_INFO
        - DCP_ALLOC_LOCAL_MEM
        - DCP_FREE_LOCAL_MEM
        - DCP_LOWER_TO_UPPER_MEM
        - DCP_UPPER_TO_LOWER_MEM
        - DCP_UPPER_TO_ETH_MEM
        - DCP_LOWER_TO_LOWER_MEM
        - DCP_COPY_COMMON_MEM
        - DCP_COPY_UPPER_TO_LOCAL_MEM
        - DCP_COPY_RXETH_TO_LOCAL_MEM
        - DCP_COPY_LOCAL_TO_ETHTX_MEM
        - DCP_ALLOC_TIMER
        - DCP_START_TIMER
        - DCP_FREE_TIMER
        - DCP_ENTER
        - DCP_EXIT
        - DCP_FATAL_ERROR

    - system interface functions


    - user interface functions


*/
/*****************************************************************************/
/* 2do:

    -

*/
/*****************************************************************************/
/* include hierarchy */

#define LSAS_MODULE_ID  15

//#include "dcp_inc.h"
#include "lsas_inc.h"

#include "lsas_int.h"
LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_MEMSET_LOCAL(DCP_LOCAL_MEM_PTR_TYPE pMem, LSA_UINT8 Value, LSA_UINT32 Length)
{
    LSAS_MEMSET(pMem, Value, Length);
}

LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_DO_TIMER_REQUEST(DCP_UPPER_RQB_PTR_TYPE  pRQB)
{
    extern LSA_VOID  DCP_LOCAL_FCT_ATTR  dcp_TimeoutRequest(DCP_UPPER_RQB_PTR_TYPE  RbuPtr);

    LSAS_RQB_DCP_REQUEST(pRQB, dcp_TimeoutRequest);
}


LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_ALLOC_UPPER_RQB_LOCAL(DCP_UPPER_RQB_PTR_TYPE  DCP_LOCAL_MEM_ATTR *  upper_rqb_ptr_ptr,
                                                             LSA_UINT16 length)
{
    LSAS_ALLOC_COMP_MEM(upper_rqb_ptr_ptr,length, LSA_COMP_ID_DCP);
}


LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_FREE_UPPER_RQB_LOCAL(LSA_UINT16 DCP_LOCAL_MEM_ATTR *  ret_val_ptr,
                                                            DCP_UPPER_RQB_PTR_TYPE upper_rqb_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,upper_rqb_ptr, LSA_COMP_ID_DCP);
}


LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_ALLOC_UPPER_RQB(DCP_UPPER_RQB_PTR_TYPE  DCP_LOCAL_MEM_ATTR *       upper_rqb_ptr_ptr,
                                                       LSA_USER_ID_TYPE                                   user_id,
                                                       LSA_UINT16                                         length,
                                                       LSA_SYS_PTR_TYPE                                   sys_ptr)
{
    LSAS_ALLOC_COMP_MEM(upper_rqb_ptr_ptr,length, LSA_COMP_ID_DCP);
}

LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_FREE_UPPER_RQB(LSA_UINT16                 DCP_LOCAL_MEM_ATTR *   ret_val_ptr,
                                                      DCP_UPPER_RQB_PTR_TYPE                            upper_rqb_ptr,
                                                      LSA_SYS_PTR_TYPE                                  sys_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,upper_rqb_ptr, LSA_COMP_ID_DCP);
}

LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_ALLOC_UPPER_MEM(DCP_UPPER_MEM_PTR_TYPE  DCP_LOCAL_MEM_ATTR *       upper_mem_ptr_ptr,
                                                       LSA_USER_ID_TYPE                                   user_id,
                                                       LSA_UINT16                                         length,
                                                       LSA_SYS_PTR_TYPE                                   sys_ptr)
{
    LSAS_ALLOC_COMP_MEM(upper_mem_ptr_ptr,length, LSA_COMP_ID_DCP);
}


LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_FREE_UPPER_MEM(LSA_UINT16  DCP_LOCAL_MEM_ATTR *   ret_val_ptr,
                                                      DCP_UPPER_MEM_PTR_TYPE             upper_mem_ptr,
                                                      LSA_SYS_PTR_TYPE                   sys_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,upper_mem_ptr, LSA_COMP_ID_DCP);
}

LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_GET_PATH_INFO(LSA_UINT16              DCP_LOCAL_MEM_ATTR *     ret_val_ptr,
                                                     LSA_SYS_PTR_TYPE        DCP_LOCAL_MEM_ATTR *     sys_ptr_ptr,
                                                     DCP_DETAIL_PTR_TYPE     DCP_LOCAL_MEM_ATTR *     detail_ptr_ptr,
                                                     LSA_SYS_PATH_TYPE                                path)
{
    LSA_GET_PATH_INFO(ret_val_ptr,sys_ptr_ptr,detail_ptr_ptr,path,LSAS_PATH_LAYER_DCP);
}


LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_RELEASE_PATH_INFO(LSA_UINT16              DCP_LOCAL_MEM_ATTR *         ret_val_ptr,
                                                         LSA_SYS_PTR_TYPE                                     sys_ptr,
                                                         DCP_DETAIL_PTR_TYPE                                  detail_ptr)
{
    LSA_RELEASE_PATH_INFO(ret_val_ptr,sys_ptr,detail_ptr);
}



#ifdef DCP_LOCAL_MEM_DYNAMIC

LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_ALLOC_LOCAL_MEM(DCP_LOCAL_MEM_PTR_TYPE  DCP_LOCAL_MEM_ATTR *        local_mem_ptr_ptr,
                                                       LSA_UINT16                                          length)
{
    LSAS_ALLOC_COMP_MEM(local_mem_ptr_ptr,length, LSA_COMP_ID_DCP);
}


LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_FREE_LOCAL_MEM(LSA_UINT16                 DCP_LOCAL_MEM_ATTR *   ret_val_ptr,
                                                      DCP_LOCAL_MEM_PTR_TYPE                            local_mem_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,local_mem_ptr, LSA_COMP_ID_DCP);
}


#endif

LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_COPY_COMMON_MEM(LSA_VOID  DCP_COMMON_MEM_ATTR *                 src_common_mem_ptr,
                                                       LSA_VOID  DCP_COMMON_MEM_ATTR *                 dst_common_mem_ptr,
                                                       LSA_UINT16                                      length)
{
    LSAS_MEMCPY(dst_common_mem_ptr,src_common_mem_ptr,length);
}

LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR    DCP_COPY_UPPER_TO_LOCAL_MEM(DCP_UPPER_MEM_PTR_TYPE             src_ptr,
                                                                DCP_LOCAL_MEM_PTR_TYPE            dst_ptr,
                                                                LSA_UINT16                      length,
                                                                LSA_SYS_PTR_TYPE                sys_ptr)
{
    LSAS_MEMCPY(dst_ptr,src_ptr,length);    // lint !e669  possible data overrun for function memcpy
}

LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR    DCP_COPY_LOCAL_TO_UPPER_MEM(DCP_LOCAL_MEM_PTR_TYPE             src_ptr,
                                                                DCP_UPPER_MEM_PTR_TYPE            dst_ptr,
                                                                LSA_UINT16                      length,
                                                                LSA_SYS_PTR_TYPE                sys_ptr)
{
    LSAS_MEMCPY(dst_ptr,src_ptr,length);
}

LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR    DCP_COPY_RXETH_TO_LOCAL_MEM(DCP_ETH_RXVOID_PTR_TYPE             src_ptr,
                                                                DCP_LOCAL_MEM_PTR_TYPE            dst_ptr,
                                                                LSA_UINT16                      length,
                                                                LSA_SYS_PTR_TYPE                sys_ptr)
{
    LSAS_MEMCPY(dst_ptr,src_ptr,length);
}


LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_ALLOC_TIMER(LSA_UINT16  DCP_LOCAL_MEM_ATTR *               ret_val_ptr,
                                                   LSA_UINT16  DCP_LOCAL_MEM_ATTR *               timer_id_ptr,
                                                   LSA_UINT16                                     timer_type,
                                                   LSA_UINT16                                     time_base)
{
    LSAS_ALLOC_TIMER(ret_val_ptr,timer_id_ptr,timer_type,time_base,dcp_timeout, LSA_COMP_ID_DCP);
}

LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_START_TIMER(LSA_UINT16        DCP_LOCAL_MEM_ATTR *         ret_val_ptr,
                                                   LSA_UINT16                                     timer_id,
                                                   LSA_USER_ID_TYPE                               user_id,
                                                   LSA_UINT16                                     time)
{
    LSAS_START_TIMER(ret_val_ptr,timer_id,user_id,time, LSA_COMP_ID_DCP);
}

LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_STOP_TIMER(LSA_UINT16  DCP_LOCAL_MEM_ATTR *              ret_val_ptr,
                                                  LSA_UINT16                                    timer_id)
{
    LSAS_STOP_TIMER(ret_val_ptr,timer_id, LSA_COMP_ID_DCP);
}

LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_FREE_TIMER(LSA_UINT16  DCP_LOCAL_MEM_ATTR *              ret_val_ptr,
                                                  LSA_UINT16                                    timer_id)
{
    LSAS_FREE_TIMER(ret_val_ptr,timer_id, LSA_COMP_ID_DCP);
}

LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_ENTER(LSA_VOID)
{
    LSAS_OS_LOCK_ON_PNIO_TASK();
}

LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_EXIT(void)
{
    LSAS_OS_RELEASE_PNIO_TASK();
}

LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_FATAL_ERROR(LSA_UINT16 length, DCP_FATAL_ERROR_PTR_TYPE error_ptr)
{
    LSAS_FATAL_ERROR(error_ptr);
}

/*=============================================================================
 * function name:  DCP_OPEN_CHANNEL_LOWER
 *
 * function:       open a communication channel to lower layer
 *
 * parameters:     DCP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:  pointer to lower-
 *                                                            RQB
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *
 * RQB-parameters:
 *     RQB-header:
 *     LSA_OPCODE_TYPE    opcode:        *_OPC_OPEN_CHANNEL
 *     LSA_HANDLE_TYPE    handle:        channel-handle of dcp
 *     LSA_USER_ID_TYPE   user-id:       id of dcp
 *     RQB-args:
 *     LSA_HANDLE_TYPE    handle_upper:  channel-handle of dcp
 *     LSA_SYS_PATH_TYPE  sys_path:      system-path of channel
 *     LSA_VOID  LSA_FCT_PTR(DCP_LOWER_IN_FCT_ATTR, dcp_request_lower_done_ptr)
 *                                    (DCP_LOWER_RQB_PTR_TYPE lower_rqb_ptr)
 *                                       pointer to callback-function
 *
 * RQB-return values via callback-function:
 *     LSA_UINT16         response:      *_RSP_OK    if no error
 *     RQB-args:
 *     LSA_HANDLE_TYPE    handle:        channel-handle of lower layer
 *     All other RQB-parameters are unchanged.
 *===========================================================================*/
#ifndef DCP_OPEN_CHANNEL_LOWER
LSA_VOID  DCP_LOWER_OUT_FCT_ATTR  DCP_OPEN_CHANNEL_LOWER(
    DCP_LOWER_RQB_PTR_TYPE                              lower_rqb_ptr,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{

    DCP_ETH_RQB_CBF_PTR_SET(lower_rqb_ptr.RbEthPtr, lsas_edd_to_dcp_cbf);

#ifdef LSAS_CFG_USE_EDDI
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr.RbEthPtr, eddi_open_channel);
#endif

#ifdef LSAS_CFG_USE_EDDP
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr.RbEthPtr, eddp_open_channel);
#endif

#ifdef LSAS_CFG_USE_EDDS
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr.RbEthPtr, edds_open_channel);
#endif
    LSA_UNUSED_ARG( sys_ptr );
}
#endif

/*=============================================================================
 * function name:  DCP_CLOSE_CHANNEL_LOWER
 *
 * function:       close a communication channel to lower layer
 *
 * parameters:     DCP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:     pointer to lower-
 *                                                            RQB
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *
 * RQB-parameters:
 *     RQB-header:
 *     LSA_OPCODE_TYPE   opcode:   *_OPC_CLOSE_CHANNEL
 *     LSA_HANDLE_TYPE   handle:   channel-handle of lower layer
 *     LSA_USER_ID_TYPE  user-id:  id of dcp
 *
 * RQB-return values via callback-function:
 *     RQB-header:
 *     LSA_HANDLE_TYPE   handle:    channel-handle of dcp
 *     LSA_UINT16        response:  *_RSP_OK    if no error
 *                                  or others
 *     All other RQB-parameters are unchanged.
 *===========================================================================*/
#ifndef DCP_CLOSE_CHANNEL_LOWER
LSA_VOID  DCP_LOWER_OUT_FCT_ATTR  DCP_CLOSE_CHANNEL_LOWER(
    DCP_LOWER_RQB_PTR_TYPE                              lower_rqb_ptr,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
#ifdef LSAS_CFG_USE_EDDI
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr.RbEthPtr, eddi_close_channel);
#endif
#ifdef LSAS_CFG_USE_EDDP
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr.RbEthPtr, eddp_close_channel);
#endif
#ifdef LSAS_CFG_USE_EDDS
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr.RbEthPtr, edds_close_channel);
#endif
    LSA_UNUSED_ARG( sys_ptr );
}
#endif

/*=============================================================================
 * function name:  DCP_REQUEST_LOWER
 *
 * function:       send a request to a communication channel to lower layer
 *
 * parameters:     DCP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:     pointer to lower-
 *                                                            RQB
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *
 * RQB-parameters:
 *     RQB-header:
 *     LSA_OPCODE_TYPE   opcode:    *_OPC_*
 *     LSA_HANDLE_TYPE   handle:    channel-handle of lower layer
 *     LSA_USER_ID_TYPE  user-id:   id of dcp
 *     RQB-args:                    Depend on kind of request.
 *
 * RQB-return values via callback-function:
 *     RQB-header:
 *     LSA_OPCODE_TYPE   opcode:    *_OPC_*
 *     LSA_HANDLE_TYPE   handle:    channel-handle of dcp
 *     LSA_USER_ID_TYPE  user-id:   id of dcp
 *     LSA_UINT16        response:  *_RSP_OK    if no error
 *
 *     All other RQB-parameters depend on kind of request.
 *===========================================================================*/
#ifndef DCP_REQUEST_LOWER
LSA_VOID  DCP_LOWER_OUT_FCT_ATTR  DCP_REQUEST_LOWER(
    DCP_LOWER_RQB_PTR_TYPE                              lower_rqb_ptr,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    switch( DCP_ETH_RQB_SERVICE_GET(lower_rqb_ptr.RbEthPtr) )
    {
#if 0
        case EDD_SRV_NRT_SEND:
          case EDD_SRV_NRT_RECV:
          case EDD_SRV_NRT_CANCEL:
        {
            edd_request(lower_rqb_ptr.RbEthPtr);
        }
        break;
#endif

        case EDD_SRV_NRT_SEND_TIMESTAMP:
        {
            LSAS_ASSERT(0);
        }
        break;

        default:
        {
#ifdef LSAS_CFG_USE_EDDI
            if(LSAS_EDD_CHECK_RCV(lower_rqb_ptr.VoidPtr, eddi_request))
            {
                LSAS_RQB_EDD_REQUEST(lower_rqb_ptr.RbEthPtr, eddi_request);
            }
#endif
#ifdef LSAS_CFG_USE_EDDP
            if(LSAS_EDD_CHECK_RCV(lower_rqb_ptr.VoidPtr, eddp_request))
            {
                LSAS_RQB_EDD_REQUEST(lower_rqb_ptr.RbEthPtr, eddp_request);
            }
#endif
#ifdef LSAS_CFG_USE_EDDS
            if(LSAS_EDD_CHECK_RCV(lower_rqb_ptr.VoidPtr, edds_request))
            {
                LSAS_RQB_EDD_REQUEST(lower_rqb_ptr.RbEthPtr, edds_request);
            }
#endif
            /* LSAS_EDD_CHECK_RCV must care about rqb routing if it returns false */
        }
    }
}
#endif

/*=============================================================================
 * function name:  DCP_ALLOC_LOWER_RQB
 *
 * function:       allocate a lower-RQB
 *
 * parameters:     DCP_LOWER_RQB_PTR_TYPE  ...  *     lower_rqb_ptr_ptr:
 *                                   return value: pointer to lower-RQB-pointer
 *                                                 or LSA_NULL: The allocation
 *                                                 will be asynchronous, see
 *                                                 dcp_lower_rqb().
 *                 LSA_USER_ID_TYPE                   user_id:  id of dcp
 *                 LSA_UINT16                         length:   length of RQB
 *                 LSA_SYS_PTR_TYPE                   sys_ptr:  system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef DCP_ALLOC_LOWER_RQB
LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_ALLOC_LOWER_RQB(
    DCP_LOWER_RQB_PTR_TYPE  DCP_LOCAL_MEM_ATTR *        lower_rqb_ptr_ptr,
    LSA_USER_ID_TYPE                                    user_id,
    LSA_UINT16                                          length,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(lower_rqb_ptr_ptr, length, LSA_COMP_ID_DCP);

    LSA_UNUSED_ARG( user_id );
    LSA_UNUSED_ARG( sys_ptr );
}
#endif

/*=============================================================================
 * function name:  DCP_FREE_LOWER_RQB
 *
 * function:       free a lower-RQB
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      lower-RQB or
 *                                                      wrong system-pointer
 *                 DCP_LOWER_RQB_PTR_TYPE     lower_rqb_ptr:  pointer to lower-
 *                                                            RQB
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef DCP_FREE_LOWER_RQB
LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_FREE_LOWER_RQB(
    LSA_UINT16                 DCP_LOCAL_MEM_ATTR *     ret_val_ptr,
    DCP_LOWER_RQB_PTR_TYPE                              lower_rqb_ptr,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    LSAS_FREE_COMP_MEM( ret_val_ptr, lower_rqb_ptr.RbEthPtr, LSA_COMP_ID_DCP)

    LSA_UNUSED_ARG( sys_ptr );
}
#endif

/*=============================================================================
 * function name:  DCP_ALLOC_LOWER_MEM
 *
 * function:       allocate a lower-memory
 *
 * parameters:     DCP_LOWER_MEM_PTR_TYPE  ...  *     lower_mem_ptr_ptr:
 *                                   return value: pointer to lower-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: The allocation
 *                                                 will be asynchronous, see
 *                                                 dcp_lower_mem().
 *                 LSA_USER_ID_TYPE                   user_id:  id of dcp
 *                 LSA_UINT16                         length:   length of
 *                                                              memory
 *                 LSA_SYS_PTR_TYPE  sys_ptr:                   system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef DCP_ALLOC_LOWER_MEM
LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_ALLOC_LOWER_MEM(
    DCP_LOWER_MEM_PTR_TYPE  DCP_LOCAL_MEM_ATTR *        lower_mem_ptr_ptr,
    LSA_USER_ID_TYPE                                    user_id,
    LSA_UINT16                                          length,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM( lower_mem_ptr_ptr, length, LSA_COMP_ID_DCP);

    LSA_UNUSED_ARG( user_id );
    LSA_UNUSED_ARG( sys_ptr );
}
#endif

/*=============================================================================
 * function name:  DCP_FREE_LOWER_MEM
 *
 * function:       free a lower-memory
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      lower-memory or
 *                                                      wrong system-pointer
 *                 DCP_LOWER_MEM_PTR_TYPE  lower_mem_ptr:     pointer to lower-
 *                                                            memory
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef DCP_FREE_LOWER_MEM
LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_FREE_LOWER_MEM(
    LSA_UINT16                 DCP_LOCAL_MEM_ATTR *     ret_val_ptr,
    DCP_LOWER_MEM_PTR_TYPE                              lower_mem_ptr,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    LSAS_FREE_COMP_MEM( ret_val_ptr, lower_mem_ptr.MemEthPtr, LSA_COMP_ID_DCP);

    LSA_UNUSED_ARG( sys_ptr );
}
#endif

/*=============================================================================
 * function name:  DCP_ALLOC_LOWER_TXMEM
 *
 * function:       allocate a lower memory for send PDUs
 *
 * parameters:     DCP_LOWER_TXMEM_PTR_TYPE  ...  *     lower_mem_ptr_ptr:
 *                                   return value: pointer to lower-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: The allocation
 *                                                 will be asynchronous, see
 *                                                 dcp_lower_mem().
 *                 LSA_USER_ID_TYPE                   user_id:  id of dcp
 *                 LSA_UINT16                         length:   length of
 *                                                              memory
 *                 LSA_SYS_PTR_TYPE  sys_ptr:                   system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef DCP_ALLOC_LOWER_TXMEM
LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_ALLOC_LOWER_TXMEM(
    DCP_LOWER_TXMEM_PTR_TYPE  DCP_LOCAL_MEM_ATTR *      lower_mem_ptr_ptr,
    LSA_USER_ID_TYPE                                    user_id,
    LSA_UINT16                                          length,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    LSAS_ALLOC_PACKET_MEM((void**) lower_mem_ptr_ptr, length, LSA_COMP_ID_DCP);

    LSA_UNUSED_ARG( user_id );
    LSA_UNUSED_ARG( sys_ptr );
}
#endif

/*=============================================================================
 * function name:  DCP_FREE_LOWER_TXMEM
 *
 * function:       free a lower memory for send PDUs
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      lower-memory or
 *                                                      wrong system-pointer
 *                 DCP_LOWER_MEM_PTR_TYPE  lower_mem_ptr:     pointer to lower-
 *                                                            memory
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef DCP_FREE_LOWER_TXMEM
LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_FREE_LOWER_TXMEM(
    LSA_UINT16                 DCP_LOCAL_MEM_ATTR *     ret_val_ptr,
    DCP_LOWER_TXMEM_PTR_TYPE                            lower_mem_ptr,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    LSAS_FREE_PACKET_MEM( ret_val_ptr, lower_mem_ptr.TxEthPtr, LSA_COMP_ID_DCP);

    *(ret_val_ptr) = LSA_RET_OK;
}
#endif

/*=============================================================================
 * function name:  DCP_ALLOC_LOWER_RXMEM
 *
 * function:       allocate a lower memory for receive PDUs
 *
 * parameters:     DCP_LOWER_RXMEM_PTR_TYPE  ...  *     lower_mem_ptr_ptr:
 *                                   return value: pointer to lower-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: The allocation
 *                                                 will be asynchronous, see
 *                                                 dcp_lower_mem().
 *                 LSA_USER_ID_TYPE                   user_id:  id of dcp
 *                 LSA_UINT16                         length:   length of
 *                                                              memory
 *                 LSA_SYS_PTR_TYPE  sys_ptr:                   system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef DCP_ALLOC_LOWER_RXMEM
LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_ALLOC_LOWER_RXMEM(
    DCP_LOWER_RXMEM_PTR_TYPE  DCP_LOCAL_MEM_ATTR *      lower_mem_ptr_ptr,
    LSA_USER_ID_TYPE                                    user_id,
    LSA_UINT16                                          length,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    LSAS_ALLOC_PACKET_MEM( (void**)lower_mem_ptr_ptr, length, LSA_COMP_ID_DCP);

    LSA_UNUSED_ARG( user_id );
    LSA_UNUSED_ARG( sys_ptr );
}
#endif

/*=============================================================================
 * function name:  DCP_FREE_LOWER_RXMEM
 *
 * function:       free a lower memory for receive PDUs
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      lower-memory or
 *                                                      wrong system-pointer
 *                 DCP_LOWER_RXMEM_PTR_TYPE  lower_mem_ptr:     pointer to lower-
 *                                                            memory
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef DCP_FREE_LOWER_RXMEM
LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_FREE_LOWER_RXMEM(
    LSA_UINT16                 DCP_LOCAL_MEM_ATTR *     ret_val_ptr,
    DCP_LOWER_RXMEM_PTR_TYPE                            lower_mem_ptr,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    LSAS_FREE_PACKET_MEM( ret_val_ptr, lower_mem_ptr.RxEthPtr, LSA_COMP_ID_DCP);

    *(ret_val_ptr) = LSA_RET_OK;
}
#endif

/*=============================================================================
 * function name:  DCP_UPPER_TO_ETH_MEM
 *
 * function:       convert upper-memory to lower-ethernet-memory
 *
 * parameters:     DCP_ETH_MEM_PTR_TYPE  ...  *    eth_mem_ptr_ptr:
 *                                   return value:   pointer to lower-ethernet-
 *                                                   memory pointer
 *                                                   or LSA_NULL: conversion
 *                                                   wasn't possible
 *                 DCP_UPPER_MEM_PTR_TYPE          upper_mem_ptr:
 *                                                   pointer to upper-memory
 *                 LSA_SYS_PTR_TYPE                sys_ptr:  system pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef DCP_UPPER_TO_ETH_MEM
LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_UPPER_TO_ETH_MEM(
    DCP_ETH_MEM_PTR_TYPE  DCP_LOCAL_MEM_ATTR *            eth_mem_ptr_ptr,
    DCP_UPPER_MEM_PTR_TYPE                                upper_mem_ptr,
    LSA_SYS_PTR_TYPE                                      sys_ptr
)
{
    *eth_mem_ptr_ptr = upper_mem_ptr;

    LSA_UNUSED_ARG(sys_ptr);
}
#else
    #error "by design a fuction"
#endif

/*=============================================================================
 * function name:  DCP_ETH_TO_UPPER_MEM
 *
 * function:       convert lower ethernet memory to upper memory
 *
 * parameters:     DCP_UPPER_MEM_PTR_TYPE  ...  *  upper_mem_ptr_ptr:
 *                                   return value:   pointer to upper
 *                                                   memory pointer
 *                                                   or LSA_NULL: conversion
 *                                                   wasn't possible
 *                 DCP_UPPER_MEM_PTR_TYPE          eth_mem_ptr:
 *                                                   pointer to lower ethernet
 *                                                   memory
 *                 LSA_SYS_PTR_TYPE                sys_ptr:  system pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef DCP_ETH_TO_UPPER_MEM
LSA_VOID  DCP_SYSTEM_OUT_FCT_ATTR  DCP_ETH_TO_UPPER_MEM(
    DCP_UPPER_MEM_PTR_TYPE  DCP_LOCAL_MEM_ATTR *        upper_mem_ptr_ptr,
    DCP_ETH_MEM_PTR_TYPE                                eth_mem_ptr,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    *upper_mem_ptr_ptr = eth_mem_ptr;

    LSA_UNUSED_ARG(sys_ptr);
}
#else
    #error "by design a fuction"
#endif


LSA_UINT16 dcp_version ( LSA_UINT16 length, LSA_VERSION_TYPE *version )
{
  LSA_UINT8 *p_version, *p_v;
  LSA_UINT16 i;

  /* Initialize version struct */
  LSA_VERSION_TYPE v = {
                        PNIO_LSA_COMPONENT_ID, {'L','S','A','_','D','C','P','\0','\0','\0'},
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
