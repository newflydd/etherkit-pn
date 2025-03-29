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
/*  F i l e               &F: nare_cfg.c                                :F&  */
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
#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

/* ****************************************************************************
*
*   H i s t o r y :
*   see /_doc/im151_3pn_hist.doc
*
* ****************************************************************************/

#define LSAS_MODULE_ID  23

//#include "nare_inc.h"
#include "lsas_inc.h"
#include "lsas_int.h"

LSA_UINT32 glob_nare_sniffer = LSA_FALSE;

/*===========================================================================*/
/*====                    out functions to DCP                           ====*/
/*===========================================================================*/

/*=============================================================================
 * function name:  NARE_DCP_OPEN_CHANNEL_LOWER
 *
 * function:       open a communication channel to DCP lower layer
 *
 * parameters:     NARE_DCP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:  pointer to lower-
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

LSA_VOID    NARE_DCP_OPEN_CHANNEL_LOWER(
    NARE_DCP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE             sys_ptr
)
{
    LSA_UNUSED_ARG(sys_ptr);

    lower_rqb_ptr->args.channel.dcp_request_upper_done_ptr = lsas_dcp_to_nare_cbf;

    LSAS_RQB_DCP_REQUEST(lower_rqb_ptr, dcp_open_channel);
}


/*=============================================================================
 * function name:  NARE_DCP_CLOSE_CHANNEL_LOWER
 *
 * function:       close a communication channel to DCP lower layer
 *
 * parameters:     NARE_DCP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:  pointer to lower-
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

LSA_VOID    NARE_DCP_CLOSE_CHANNEL_LOWER(
    NARE_DCP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE             sys_ptr
)
{
    LSA_UNUSED_ARG(sys_ptr);

    LSAS_RQB_DCP_REQUEST(lower_rqb_ptr, dcp_close_channel);
}

/*=============================================================================
 * function name:  NARE_DCP_REQUEST_LOWER
 *
 * function:       send a request to a communication channel to DCP lower layer
 *
 * parameters:     NARE_DCP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:  pointer to lower-
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
LSA_VOID    NARE_DCP_REQUEST_LOWER(
    NARE_DCP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE             sys_ptr
)
{
    LSA_UNUSED_ARG(sys_ptr);

    LSAS_RQB_DCP_REQUEST(lower_rqb_ptr, dcp_request);
}


/*=============================================================================
 * function name:  NARE_REQUEST_UPPER_DONE
 *
 * function:       returns a finished request to the upper layer
 *
 * parameters:     NARE_UPPER_CALLBACK_FCT_PTR_TYPE
 *                                            prefix_request_upper_done_ptr:
 *                     pointer to prefix_request_upper_done_ptr, given by
 *                     prefix_open_channel()
 *
 *                 NARE_UPPER_RQB_PTR_TYPE  upper_rqb_ptr:  pointer to upper-
 *                                                            RQB
 *                 LSA_SYS_PTR_TYPE           sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *
 *===========================================================================*/

#ifndef NARE_REQUEST_UPPER_DONE
LSA_VOID    NARE_REQUEST_UPPER_DONE(
       NARE_UPPER_CALLBACK_FCT_PTR_TYPE  nare_request_upper_done_ptr,
       NARE_UPPER_RQB_PTR_TYPE           upper_rqb_ptr,
       LSA_SYS_PTR_TYPE                  sys_ptr
)
{
    nare_request_upper_done_ptr(upper_rqb_ptr);
}
#endif

/*=============================================================================
 * function name:  NARE_DCP_ALLOC_LOWER_RQB
 *
 * function:       allocate a lower-RQB for DCP synchronously
 *
 * parameters:     NARE_DCP_LOWER_RQB_PTR_TYPE  ...  *  lower_rqb_ptr_ptr:
 *                                   return value: pointer to lower-RQB-pointer
 *                                                 or LSA_NULL: The allocation
 *                                                 failed.
 *                 LSA_USER_ID_TYPE                   user_id:  not used
 *                 LSA_UINT16                         length:   length of RQB
 *                 LSA_SYS_PTR_TYPE                   sys_ptr:  system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID    NARE_DCP_ALLOC_LOWER_RQB(
    NARE_DCP_LOWER_RQB_PTR_TYPE   *  lower_rqb_ptr_ptr,
    LSA_USER_ID_TYPE                 user_id,
    LSA_UINT16                       length,
    LSA_SYS_PTR_TYPE                 sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(lower_rqb_ptr_ptr,length, LSA_COMP_ID_NARE);
}

/*=============================================================================
 * function name:  NARE_DCP_FREE_LOWER_RQB
 *
 * function:       free a lower-RQB from DCP
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      lower-RQB or
 *                                                      wrong system-pointer
 *                 NARE_DCP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:  pointer to lower-
 *                                                              RQB
 *                 LSA_SYS_PTR_TYPE             sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID    NARE_DCP_FREE_LOWER_RQB(
    LSA_UINT16                      *  ret_val_ptr,
    NARE_DCP_LOWER_RQB_PTR_TYPE        lower_rqb_ptr,
    LSA_SYS_PTR_TYPE                   sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,lower_rqb_ptr, LSA_COMP_ID_NARE);
}

/*=============================================================================
 * function name:  NARE_DCP_ALLOC_LOWER_MEM
 *
 * function:       allocate a lower-memory for DCP synchronously
 *
 * parameters:     NARE_DCP_LOWER_MEM_PTR_TYPE  ...  *  lower_mem_ptr_ptr:
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
LSA_VOID    NARE_DCP_ALLOC_LOWER_MEM(
    NARE_DCP_LOWER_MEM_PTR_TYPE   *  lower_mem_ptr_ptr,
    LSA_USER_ID_TYPE                 user_id,
    LSA_UINT16                       length,
    LSA_SYS_PTR_TYPE                 sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(lower_mem_ptr_ptr,length, LSA_COMP_ID_NARE);
}

/*=============================================================================
 * function name:  NARE_DCP_FREE_LOWER_MEM
 *
 * function:       free a lower-memory for DCP
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      lower-memory or
 *                                                      wrong system-pointer
 *                 NARE_DCP_LOWER_MEM_PTR_TYPE  lower_mem_ptr:  pointer to lower-
 *                                                              memory
 *                 LSA_SYS_PTR_TYPE             sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID    NARE_DCP_FREE_LOWER_MEM(
    LSA_UINT16                      *  ret_val_ptr,
    NARE_DCP_LOWER_MEM_PTR_TYPE        lower_mem_ptr,
    LSA_SYS_PTR_TYPE                   sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,lower_mem_ptr, LSA_COMP_ID_NARE);
}



/*=============================================================================
 * function name:  NARE_DCP_LOWER_TO_UPPER_MEM
 *
 * function:       convert lower-memory to upper-memory
 *
 * parameters:     NARE_UPPER_MEM_PTR_TYPE  ...  *  upper_mem_ptr_ptr:
 *                                   return value: pointer to upper-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: conversion
 *                                                 wasn't possible
 *                 NARE_DCP_LOWER_MEM_PTR_TYPE     lower_mem_ptr:
 *                                                 pointer to lower-memory
 *                 LSA_SYS_PTR_TYPE                sys_ptr:  system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID    NARE_DCP_LOWER_TO_UPPER_MEM(
    NARE_UPPER_MEM_PTR_TYPE   *      upper_mem_ptr_ptr,
    NARE_DCP_LOWER_MEM_PTR_TYPE      lower_mem_ptr,
    LSA_SYS_PTR_TYPE                 sys_ptr
)
{
    upper_mem_ptr_ptr = lower_mem_ptr;
    LSA_UNUSED_ARG(sys_ptr);
    LSA_UNUSED_ARG(upper_mem_ptr_ptr);
}


/*=============================================================================
 * function name:  NARE_DCP_UPPER_TO_LOWER_MEM
 *
 * function:       convert upper-memory to lower-memory
 *
 * parameters:     NARE_DCP_LOWER_MEM_PTR_TYPE  ..*  lower_mem_ptr_ptr:
 *                                   return value: pointer to lower-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: conversion
 *                                                 wasn't possible
 *                 NARE_UPPER_MEM_PTR_TYPE         upper_mem_ptr:
 *                                                 pointer to upper-memory
 *                 LSA_SYS_PTR_TYPE                sys_ptr:  system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID    NARE_DCP_UPPER_TO_LOWER_MEM(
    NARE_DCP_LOWER_MEM_PTR_TYPE     *   lower_mem_ptr_ptr,
    NARE_UPPER_MEM_PTR_TYPE             upper_mem_ptr,
    LSA_SYS_PTR_TYPE                    sys_ptr
)
{
    *lower_mem_ptr_ptr = upper_mem_ptr;
    LSA_UNUSED_ARG(sys_ptr);
}


/*=============================================================================
 * function name:  NARE_DCP_UPPER_RQB_TO_LOWER_MEM
 *
 * function:       convert upper-RQB memory to lower-memory
 *
 * parameters:     NARE_DCP_LOWER_MEM_PTR_TYPE  ..*  lower_mem_ptr_ptr:
 *                                   return value: pointer to lower-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: conversion
 *                                                 wasn't possible
 *                 NARE_UPPER_RQB_MEM_PTR_TYPE     upper_rqb_mem_ptr:
 *                                                 pointer to upper-memory
 *                 LSA_SYS_PTR_TYPE                sys_ptr:  system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID    NARE_DCP_UPPER_RQB_TO_LOWER_MEM(
    NARE_DCP_LOWER_MEM_PTR_TYPE     *   lower_mem_ptr_ptr,
    NARE_UPPER_RQB_MEM_PTR_TYPE         upper_rqb_mem_ptr,
    LSA_SYS_PTR_TYPE                    sys_ptr
)
{
    *lower_mem_ptr_ptr = upper_rqb_mem_ptr;
    LSA_UNUSED_ARG(sys_ptr);
}


/*===========================================================================*/
/*====                    out functions to EDD                           ====*/
/*===========================================================================*/

/*=============================================================================
 * function name:  NARE_EDD_OPEN_CHANNEL_LOWER
 *
 * function:       open a communication channel to EDD lower layer
 *
 * parameters:     NARE_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:  pointer to lower-
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
LSA_VOID    NARE_EDD_OPEN_CHANNEL_LOWER(
    NARE_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE             sys_ptr
)
{
    EDD_UPPER_OPEN_CHANNEL_PTR_TYPE edd_open = lower_rqb_ptr->pParam;

    edd_open->Cbf = lsas_edd_to_nare_cbf;

    lower_rqb_ptr->Service = 0; /* ??? EDD_PPH says 0 */
#ifdef LSAS_CFG_USE_EDDI
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr, eddi_open_channel);
#endif
#ifdef LSAS_CFG_USE_EDDP
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr, eddp_open_channel);
#endif
#ifdef LSAS_CFG_USE_EDDS
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr, edds_open_channel);
#endif

    LSA_UNUSED_ARG(sys_ptr);
}

/*=============================================================================
 * function name:  NARE_EDD_CLOSE_CHANNEL_LOWER
 *
 * function:       close a communication channel to EDD lower layer
 *
 * parameters:     NARE_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:  pointer to lower-
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
LSA_VOID    NARE_EDD_CLOSE_CHANNEL_LOWER(
    NARE_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE             sys_ptr
)
{
    LSA_UNUSED_ARG(sys_ptr);

    lower_rqb_ptr->Service = 0; /* ??? EDD_PPH says 0 */
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
 * function name:  NARE_EDD_REQUEST_LOWER
 *
 * function:       send a request to a communication channel to EDD lower layer
 *
 * parameters:     NARE_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:  pointer to lower-
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
LSA_VOID    NARE_EDD_REQUEST_LOWER(
    NARE_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE             sys_ptr
)
{
    LSA_UNUSED_ARG(sys_ptr);

    switch( NARE_EDD_RQB_GET_SERVICE(lower_rqb_ptr) )
    {
#if 0
        case EDD_SRV_NRT_SEND:
          case EDD_SRV_NRT_RECV:
          case EDD_SRV_NRT_CANCEL:
        {
            eddi_request(lower_rqb_ptr);
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
    }
}

/*=============================================================================
 * function name:  NARE_EDD_ALLOC_LOWER_RQB
 *
 * function:       allocate a lower-RQB for EDD synchronously
 *
 * parameters:     NARE_EDD_LOWER_RQB_PTR_TYPE  ...  *  lower_rqb_ptr_ptr:
 *                                   return value: pointer to lower-RQB-pointer
 *                                                 or LSA_NULL: The allocation
 *                                                 failed.
 *                 LSA_USER_ID_TYPE                   user_id:  not used
 *                 LSA_UINT16                         length:   length of RQB
 *                 LSA_SYS_PTR_TYPE                   sys_ptr:  system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID    NARE_EDD_ALLOC_LOWER_RQB(
    NARE_EDD_LOWER_RQB_PTR_TYPE   *  lower_rqb_ptr_ptr,
    LSA_USER_ID_TYPE                 user_id,
    LSA_UINT16                       length,
    LSA_SYS_PTR_TYPE                 sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(lower_rqb_ptr_ptr,length, LSA_COMP_ID_NARE);
}

/*=============================================================================
 * function name:  NARE_EDD_FREE_LOWER_RQB
 *
 * function:       free a lower-RQB from EDD
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      lower-RQB or
 *                                                      wrong system-pointer
 *                 NARE_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr:  pointer to lower-
 *                                                              RQB
 *                 LSA_SYS_PTR_TYPE             sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID    NARE_EDD_FREE_LOWER_RQB(
    LSA_UINT16                      *  ret_val_ptr,
    NARE_EDD_LOWER_RQB_PTR_TYPE        lower_rqb_ptr,
    LSA_SYS_PTR_TYPE                   sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_rqb_ptr, LSA_COMP_ID_NARE);
}

/*=============================================================================
 * function name:  NARE_EDD_ALLOC_LOWER_MEM
 *
 * function:       allocate a lower-memory for EDD synchronously
 *
 * parameters:     NARE_EDD_LOWER_MEM_PTR_TYPE  ...  *  lower_mem_ptr_ptr:
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
LSA_VOID    NARE_EDD_ALLOC_LOWER_MEM(
    NARE_EDD_LOWER_MEM_PTR_TYPE   *  lower_mem_ptr_ptr,
    LSA_USER_ID_TYPE                 user_id,
    LSA_UINT16                       length,
    LSA_SYS_PTR_TYPE                 sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(lower_mem_ptr_ptr,length, LSA_COMP_ID_NARE);
}

/*=============================================================================
 * function name:  NARE_EDD_FREE_LOWER_MEM
 *
 * function:       free a lower-memory for EDD
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      lower-memory or
 *                                                      wrong system-pointer
 *                 NARE_EDD_LOWER_MEM_PTR_TYPE  lower_mem_ptr:  pointer to lower-
 *                                                              memory
 *                 LSA_SYS_PTR_TYPE             sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID    NARE_EDD_FREE_LOWER_MEM(
    LSA_UINT16                      *  ret_val_ptr,
    NARE_EDD_LOWER_MEM_PTR_TYPE        lower_mem_ptr,
    LSA_SYS_PTR_TYPE                   sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_mem_ptr, LSA_COMP_ID_NARE);
}


/*=============================================================================
 * function name:  NARE_EDD_ALLOC_LOWER_TXMEM
 *
 * function:       allocate a lower memory for send PDUs
 *
 *                 This memory has to be DWORD-aligned!!
 *
 * parameters:     NARE_EDD_LOWER_MEM_PTR_TYPE  ...  * lower_mem_ptr_ptr:
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
LSA_VOID    NARE_EDD_ALLOC_LOWER_TXMEM(
    NARE_EDD_LOWER_MEM_PTR_TYPE   *lower_mem_ptr_ptr,
    LSA_USER_ID_TYPE               user_id,
    LSA_UINT16                     length,
    LSA_SYS_PTR_TYPE               sys_ptr
)
{
    LSAS_ALLOC_PACKET_MEM(lower_mem_ptr_ptr,length, LSA_COMP_ID_NARE);

    LSA_UNUSED_ARG( user_id );
    LSA_UNUSED_ARG( sys_ptr );
}


/*=============================================================================
 * function name:  NARE_EDD_FREE_LOWER_TXMEM
 *
 * function:       free a lower memory for send PDUs
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      lower-memory or
 *                                                      wrong system-pointer
 *                 NARE_EDD_LOWER_MEM_PTR_TYPE  lower_mem_ptr:pointer to lower-
 *                                                            memory
 *                 LSA_SYS_PTR_TYPE             sys_ptr:      system-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID    NARE_EDD_FREE_LOWER_TXMEM(
    LSA_UINT16                  *    ret_val_ptr,
    NARE_EDD_LOWER_MEM_PTR_TYPE      lower_mem_ptr,
    LSA_SYS_PTR_TYPE                 sys_ptr
)
{
    LSAS_FREE_PACKET_MEM(ret_val_ptr, lower_mem_ptr, LSA_COMP_ID_NARE);
}

/******************************************************************************
 *
 *                  nare_out
 *
 *****************************************************************************/

/*=============================================================================
 * function name:  NARE_GET_PATH_INFO
 *
 * function:       get system-pointer and detail-pointer of a communication
 *                 channel
 *
 * parameters:     LSA_UINT16              ...  *  ret_val_ptr:
 *                                      return value: LSA_RET_OK
 *                                                    LSA_RET_ERR_SYS_PATH
 *                 LSA_SYS_PTR_TYPE        ...  *  sys_ptr_ptr:
 *                                      return value: pointer to system-pointer
 *                 NARE_DETAIL_PTR_TYPE  ...  *  detail_ptr_ptr:
 *                                      return value: pointer to detail-pointer
 *                 LSA_SYS_PATH_TYPE               path: path of a
 *                                                       communication channel
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    NARE_GET_PATH_INFO(
    LSA_UINT16               *  ret_val_ptr,
    LSA_SYS_PTR_TYPE         *  sys_ptr_ptr,
    NARE_DETAIL_PTR_TYPE     *  detail_ptr_ptr,
    LSA_SYS_PATH_TYPE           path
)
{
    LSA_GET_PATH_INFO(ret_val_ptr, sys_ptr_ptr, detail_ptr_ptr, path, LSAS_PATH_LAYER_NARE);
}


/*=============================================================================
 * function name:  NARE_RELEASE_PATH_INFO
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
 *                 NARE_DETAIL_PTR_TYPE  detail_ptr:  detail-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID    NARE_RELEASE_PATH_INFO(
    LSA_UINT16               *  ret_val_ptr,
    LSA_SYS_PTR_TYPE            sys_ptr,
    NARE_DETAIL_PTR_TYPE        detail_ptr
)
{
    LSA_RELEASE_PATH_INFO(ret_val_ptr, sys_ptr, detail_ptr);
}

/*=============================================================================
 * function name:  NARE_ALLOC_TIMER
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
 *                 LSA_UINT16          time_base:   LSA_TIME_BASE_1MS,
 *                                                  LSA_TIME_BASE_10MS,
 *                                                  LSA_TIME_BASE_100MS,
 *                                                  LSA_TIME_BASE_1S,
 *                                                  LSA_TIME_BASE_10S or
 *                                                  LSA_TIME_BASE_100S
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID    NARE_ALLOC_TIMER(
    LSA_UINT16          *  ret_val_ptr,
    LSA_TIMER_ID_TYPE   *  timer_id_ptr,
    LSA_UINT16             timer_type,
    LSA_UINT16             time_base
)
{
    LSAS_ALLOC_TIMER(ret_val_ptr, timer_id_ptr, timer_type, time_base, nare_timeout, LSA_COMP_ID_NARE);
}

/*=============================================================================
 * function name:  NARE_START_TIMER
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
 *                                 system will call NARE_timeout().
 *                 LSA_TIMER_ID_TYPE   timer_id:  id of timer
 *                 LSA_USER_ID_TYPE    user_id:   id of prefix
 *                 LSA_UINT16          time:      running time
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID    NARE_START_TIMER(
    LSA_UINT16         *  ret_val_ptr,
    LSA_TIMER_ID_TYPE     timer_id,
    LSA_USER_ID_TYPE      user_id,
    LSA_UINT16            time
)
{
    LSAS_START_TIMER(ret_val_ptr, timer_id, user_id, time, LSA_COMP_ID_NARE);
}

/*=============================================================================
 * function name:  NARE_STOP_TIMER
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
 *                 LSA_TIMER_ID_TYPE  timer_id:  id of timer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID    NARE_STOP_TIMER(
    LSA_UINT16                      *  ret_val_ptr,
    LSA_TIMER_ID_TYPE                  timer_id
)
{
    LSAS_STOP_TIMER(ret_val_ptr, timer_id, LSA_COMP_ID_NARE);
}

/*=============================================================================
 * function name:  NARE_FREE_TIMER
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
 *                 LSA_TIMER_ID_TYPE  timer_id:  id of timer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID    NARE_FREE_TIMER(
    LSA_UINT16                      *  ret_val_ptr,
    LSA_TIMER_ID_TYPE                  timer_id
)
{
    LSAS_FREE_TIMER(ret_val_ptr, timer_id, LSA_COMP_ID_NARE);
}


/*=============================================================================
 * function name:  NARE_FATAL_ERROR
 *
 * function:       notify a fatal-error
 *
 * parameters:     LSA_UINT16                   length:     number of bytes
 *                 NARE_FATAL_ERROR_PTR_TYPE  error_ptr:  pointer to
 *                                                          LSA_FATAL_ERROR
 *
 * return value:   LSA_VOID
 *===========================================================================*/

#ifndef NARE_FATAL_ERROR
LSA_VOID    NARE_FATAL_ERROR(
    LSA_UINT16                 length,
    NARE_FATAL_ERROR_PTR_TYPE  error_ptr
);
#error NARE_FATAL_ERROR must be a macro
#endif

/*=============================================================================
 * function name:  NARE_ENTER
 *
 * function:       Protect sequence within NARE from beeing interrupted.
 *                 (reentrance protection)
 *
 * parameters:     LSA_VOID
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID    NARE_ENTER(
    LSA_VOID
)
{
    LSAS_OS_LOCK_ON_PNIO_TASK();
}

/*=============================================================================
 * function name:  NARE_EXIT
 *
 * function:       Finishes NARE_ENTER-Sequence.
 *
 * parameters:     LSA_VOID
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID    NARE_EXIT(
    LSA_VOID
)
{
    LSAS_OS_RELEASE_PNIO_TASK();
}

/*=============================================================================
 * function name:  NARE_ALLOC_LOCAL_MEM
 *
 * function:       allocate a local-memory
 *
 * parameters:     NARE_LOCAL_MEM_PTR_TYPE  ...  *  local_mem_ptr_ptr:
 *                                   return value: pointer to local-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: No memory
 *                                                 available
 *                 LSA_UINT32                      length: length of memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    NARE_ALLOC_LOCAL_MEM(
    NARE_LOCAL_MEM_PTR_TYPE   *  local_mem_ptr_ptr,
    LSA_UINT32                   length
)
{
    LSAS_ALLOC_COMP_MEM(local_mem_ptr_ptr,length, LSA_COMP_ID_NARE);
}

/*=============================================================================
 * function name:  NARE_FREE_LOCAL_MEM
 *
 * function:       frees local -memory
 *
 * parameters:              ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      local-memory
 *                 NARE_LOCAL_MEM_PTR_TYPE  local_mem_ptr:  pointer to local-
 *                                                            memory
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    NARE_FREE_LOCAL_MEM(
    LSA_UINT16                  *  ret_val_ptr,
    NARE_LOCAL_MEM_PTR_TYPE        local_mem_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,local_mem_ptr, LSA_COMP_ID_NARE);
}


/*=============================================================================
 * function name:  NARE_RQB_ERROR
 *
 * function:       notify a RQB-error in a user/system-request.
 *                 this function is called if there is a error (i.e. param)
 *                 within RQB which prevents notification of the caller
 *                 (i.e. missing call-back-function). the error-code is set
 *                 in "Status" in RQB. this error is typically a implementation
 *                 error.
 *
 * parameters:     NARE_UPPER_RQB_PTR_TYPE   pRQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/

#ifndef NARE_RQB_ERROR
LSA_VOID    NARE_RQB_ERROR(
    NARE_UPPER_RQB_PTR_TYPE  pRQB
)
#error NARE_RQB_ERROR must be a macro
#endif

/*=============================================================================
 * function name:  NARE_MEMSET_LOCAL
 *
 * function:       fills memory with value
 *
 * parameters:      NARE_LOCAL_MEM_PTR_TYPE        pMem
 *                  LSA_UINT8                      Value
 *                  LSA_UINT32                     Length
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    NARE_MEMSET_LOCAL(
    NARE_LOCAL_MEM_PTR_TYPE             pMem,
    LSA_UINT8                           Value,
    LSA_UINT32                          Length
)
{
    LSAS_MEMSET(pMem,Value,Length);
}

/*=============================================================================
 * function name:  NARE_MEMCOPY_UPPER
 *
 * function:       copy Length bytes memory from pSrc to pDst.
 *
 * parameters:      NARE_UPPER_MEM_PTR_TYPE         pDst
 *                  NARE_UPPER_MEM_PTR_TYPE         pSrc
 *                  LSA_UINT32                      Length
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    NARE_MEMCOPY_UPPER(
    NARE_UPPER_MEM_PTR_TYPE              pDst,
    NARE_UPPER_MEM_PTR_TYPE              pSrc,
    LSA_UINT32                          Length
)
{
    LSAS_MEMCPY(pDst,pSrc,Length);
}

/*=============================================================================
 * function name:  NARE_MEMCOPY_COMMON
 *
 * function:       copy Length bytes memory from pSrc to pDst.
 *
 * parameters:      NARE_COMMON_MEM_PTR_TYPE        pDst
 *                  NARE_COMMON_MEM_PTR_TYPE        pSrc
 *                  LSA_UINT32                      Length
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    NARE_MEMCOPY_COMMON(
    NARE_COMMON_MEM_PTR_TYPE            pDst,
    NARE_COMMON_MEM_PTR_TYPE            pSrc,
    LSA_UINT32                          Length
)
{
    LSAS_MEMCPY(pDst,pSrc,Length);
}


/*=============================================================================
 * function name:  NARE_ALLOC_UPPER_RQB_LOCAL
 *
 * function:       allocate an upper-RQB for local use
 *
 * parameters:     NARE_UPPER_RQB_PTR_TYPE  ...  *  upper_rqb_ptr_ptr:
 *                                   return value: pointer to local-memory-
 *                                                 pointer
 *                                                 or LSA_NULL: No memory
 *                                                 available
 *                 LSA_UINT16                      length:   length of RQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID    NARE_ALLOC_UPPER_RQB_LOCAL(
    NARE_UPPER_RQB_PTR_TYPE   *  upper_rqb_ptr_ptr,
    LSA_UINT16                   length
)
{
    LSAS_ALLOC_COMP_MEM(upper_rqb_ptr_ptr,length, LSA_COMP_ID_NARE);
}


/*=============================================================================
 * function name:  NARE_FREE_UPPER_RQB_LOCAL
 *
 * function:       free an upper-RQB allocated with NARE_ALLOC_UPPER_RQB_LOCAL
 *
 * parameters:     LSA_UINT16         ...  *  ret_val_ptr:
 *                     return value: LSA_RET_OK         ok
 *                                   LSA_RET_ERR_PARAM  no deallocation because
 *                                                      of wrong pointer to
 *                                                      upper-RQB or
 *                 NARE_UPPER_RQB_PTR_TYPE  upper_rqb_ptr:  pointer to upper-
 *                                                            RQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/
LSA_VOID    NARE_FREE_UPPER_RQB_LOCAL(
    LSA_UINT16                  *  ret_val_ptr,
    NARE_UPPER_RQB_PTR_TYPE        upper_rqb_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,upper_rqb_ptr, LSA_COMP_ID_NARE);
}


/*=============================================================================
 * function name:  NARE_DO_TIMER_REQUEST
 *
 * function:       Does a nare_system() request with the spezified RQB. This
 *                 function is called within NARE timer handler to signal
 *                 a Timer event to the main context of NARE.
 *                 Systemadaption has to call nare_system() with the specified
 *                 RQB. Before calling, the systemadaption should change to
 *                 taskstate.
 *
 * parameters:     NARE_UPPER_RQB_PTR_TYPE   pRQB
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    NARE_DO_TIMER_REQUEST(
    NARE_UPPER_RQB_PTR_TYPE  pRQB
)
{

    LSAS_RQB_NARE_REQUEST(pRQB, nare_request);
}


/*=============================================================================
 * function name:  NARE_ARP_RESPONSE_FRAME_CONTROL
 *
 * function:       Controls ARP-Sniffing
 *                 If turned on, systemadaption has to call
 *                 nare_ARPResponseFrameIn() with incoming frames for this
 *                 channel, till sniffer turned off.
 *
 * parameters:     LSA_UINT16                   NICId of EDD-Systemchannel
 *                 LSA_BOOL                     On
 *                                              LSA_TRUE: Turn on sniffer
 *                                              LSA_FALSE:Turn off sniffer
 *
 *
 * return value:   LSA_VOID
 *===========================================================================*/

LSA_VOID    NARE_ARP_RESPONSE_FRAME_CONTROL(
    LSA_UINT16                      NICId,
    LSA_BOOL                        On
)
{
    if (On)
    {
        glob_nare_sniffer = LSA_TRUE;
    }
}


/*=============================================================================
 * function name:  NARE_ARP_RESPONSE_FRAME_OUT
 *
 * function:       Returns a frame given to NARE via nare_ARPResponseFrameIn()
 *                 back to systemadaption. Called within nare_ARPResponseFrameIn()
 *
 * parameters:     LSA_UINT16                   NICId
 *                 NARE_EDD_LOWER_MEM_PTR_TYPE  pFrame
 *                 LSA_UINT16                   FrameLen
 *
 * return value:   LSA_VOID
 *===========================================================================*/


LSA_VOID    NARE_ARP_RESPONSE_FRAME_OUT(
    LSA_UINT16                      NICId,
    NARE_EDD_LOWER_MEM_PTR_TYPE     pFrame,
    LSA_UINT16                      FrameLen
)
{
    //for(;;);
    /* dont use this */
}

LSA_UINT16 nare_version ( LSA_UINT16 length, LSA_VERSION_TYPE *version )
{
  LSA_UINT8 *p_version, *p_v;
  LSA_UINT16 i;

  /* Initialize version struct */
  LSA_VERSION_TYPE v = {
                        PNIO_LSA_COMPONENT_ID, {'L','S','A','_','N','A','R','E','\0','\0'},
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

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*****************************************************************************/
/*  end of file.                                                             */
/*****************************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
