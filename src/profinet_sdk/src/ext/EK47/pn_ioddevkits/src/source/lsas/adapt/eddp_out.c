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
/*  F i l e               &F: eddp_out.c                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Systemadaption for EDDP dummy output macros.                             */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
/*  15.08.08    js    initial version.                                       */
/*  24.02.09    lrg   K32                                                    */
/*  17.03.10    lrg   EDDP_K32_TCM_DUMP() filled with code                   */
/*  26.03.10    MA    EDDP_K32_TCM_DUMP removed. EDDP_FATAL_ERROR now calls  */
/*                    eddp_K32GetData to get K32 internal data and dumps     */
/*                    them with sysp_K32Dump.                                */
/*****************************************************************************/

/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/
//#define LTRC_ACT_MODUL_ID  141
//#define EDDP_MODULE_ID     LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_SYSP_OUT */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#define LSAS_MODULE_ID 16

#include "lsas_inc.h"

#include "lsas_int.h"

#ifdef LSAS_CFG_USE_EDDP

/*===========================================================================*/
/*                                local types                                */
/*===========================================================================*/

typedef struct SYS_CDB_TYPE_
{
  EDDP_CDB_TYPE       CDBHead;
  EDDP_CDB_NRT_TYPE   CDBNRT;
  EDDP_CDB_PRM_TYPE     CDBPRM;
} SYS_CDB_TYPE;

/*===========================================================================*/
/*                                global Variables                           */
/*===========================================================================*/

/* --------------------------------------------------------------------------*/
/* for detail-ptr (Channel descripton block)                                 */
/* --------------------------------------------------------------------------*/

SYS_CDB_TYPE SysCDB_NRT_IP;
SYS_CDB_TYPE SysCDB_CRT;
SYS_CDB_TYPE SysCDB_K32;
SYS_CDB_TYPE SysCDB_ALL;



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname       :    SysEDDPTimeoutFct                              +*/
/*+  Input/Output       :    LSA_UINT16    timer_id                         +*/
/*+                          LSA_USER_ID_TYPE user_id                       +*/
/*+  Result             :    LSA_VOID                                       +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  timer_id           : ID of timer                                       +*/
/*+  user_id            : user_id from LSA_START_TIMER                      +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Timer Call-Back-Function                                  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID SysEDDPTimeoutFct(LSA_UINT16 timer_id,LSA_USER_ID_TYPE user_id)
{
    eddp_timeout(timer_id, user_id); /* call EDDP input function for timeout */
}


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
    LSA_SYS_PATH_TYPE                             path
)
{
    LSA_GET_PATH_INFO(ret_val_ptr, sys_ptr_ptr, detail_ptr_ptr, path, LSAS_PATH_LAYER_EDD);
}
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
    LSA_UINT16              EDDP_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_SYS_PTR_TYPE        sys_ptr,
    EDDP_DETAIL_PTR_TYPE  detail_ptr
)
{
    LSA_RELEASE_PATH_INFO(ret_val_ptr, sys_ptr, detail_ptr);
}
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
    EDDP_LOCAL_MEM_PTR_TYPE  EDDP_LOCAL_MEM_ATTR *  local_mem_ptr_ptr,
    LSA_UINT32                                    length
)
{
    //*local_mem_ptr_ptr = SysEDDPMalloc(length);
    LSAS_ALLOC_COMP_MEM(local_mem_ptr_ptr, length, LSA_COMP_ID_EDDP);
    //LSAS_ALLOC_PACKET_MEM(local_mem_ptr_ptr, length, LSA_COMP_ID_EDDP);
}
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
    LSA_UINT16                 EDDP_LOCAL_MEM_ATTR *  ret_val_ptr,
    EDDP_LOCAL_MEM_PTR_TYPE                           local_mem_ptr
)
{
    //SysEDDPFree(local_mem_ptr);
    //*ret_val_ptr = LSA_RET_OK;
    LSAS_FREE_COMP_MEM(ret_val_ptr, local_mem_ptr, LSA_COMP_ID_EDDP);
    //LSAS_FREE_PACKET_MEM(ret_val_ptr, local_mem_ptr, LSA_COMP_ID_EDDP);
}
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
    LSA_UINT16         EDDP_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_TIMER_ID_TYPE  EDDP_LOCAL_MEM_ATTR *  timer_id_ptr,
    LSA_UINT16                               timer_type,
    LSA_UINT16                               time_base
)
{

    LSA_UINT16   id;
    LSA_UINT16   RetCode;

    LSAS_ALLOC_TIMER(&RetCode, &id, timer_type,
                    time_base, SysEDDPTimeoutFct, LSA_COMP_ID_EDDP);

    *timer_id_ptr = id;

    if ( RetCode != LSA_RET_OK )
    {
       *ret_val_ptr = LSA_RET_ERR_NO_TIMER;
    }
    else
    {
       *ret_val_ptr = LSA_RET_OK;
    }
}
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
 *                                 LSA_RET_ERR_PARAM           timer hasn´t
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
    LSA_UINT16        EDDP_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_TIMER_ID_TYPE                          timer_id,
    LSA_USER_ID_TYPE                           user_id,
    LSA_UINT16                                 time
)
{
    LSA_USER_ID_TYPE dummy;

    dummy.uvar32 = user_id.uvar32; //???

    LSAS_START_TIMER( ret_val_ptr, timer_id, dummy, time, LSA_COMP_ID_EDDP );
}
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
 *                                 LSA_RET_ERR_PARAM             timer hasn´t
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
    LSA_UINT16  EDDP_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_TIMER_ID_TYPE                 timer_id
)
{
    //*ret_val_ptr = LSA_STOP_TIMER(timer_id);
    LSAS_STOP_TIMER(ret_val_ptr, timer_id, LSA_COMP_ID_EDDP);
}
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
    LSA_UINT16  EDDP_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_TIMER_ID_TYPE                 timer_id
)
{
    //LSA_FREE_TIMER(timer_id);
    LSAS_FREE_TIMER(ret_val_ptr, timer_id, LSA_COMP_ID_EDDP);

    *ret_val_ptr = LSA_RET_OK;
}
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
    LSA_UINT16                 length,
    EDDP_FATAL_ERROR_PTR_TYPE  error_ptr
)
{
#if 0
    static LSA_BOOL alreadyInFatalError = LSA_FALSE;
    LSA_UINT8*  k32Data;
    LSA_INT     k32DataLength;

    // handle recursion of potential FatalError in eddp_K32GetData()/sysp_K32Dump()
    if(LSA_FALSE != alreadyInFatalError)
    {
        return;
    }else
    {
        alreadyInFatalError = LSA_TRUE;
    }

    if( EDD_STS_OK ==  (SyshDDB, &k32Data, &k32DataLength) )
    {
        //sysp_K32Dump(k32Data,k32DataLength);
    }
#endif
    LSAS_FATAL_ERROR( error_ptr);
}
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
)
{
    Cbf(upper_rqb_ptr);

    LSA_UNUSED_ARG(sys_ptr);
}
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
    EDD_UPPER_RQB_PTR_TYPE  pRQB
)
{
#if (SYSP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pRQB);
#endif

}
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
    EDD_COMMON_MEM_PTR_TYPE             pMem,
    LSA_UINT8                           value,
    LSA_UINT32                          length
)
{
    LSAS_MEMSET(pMem, value, length);       // lint !e668  possibly passing a null pointer
}
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
    EDD_COMMON_MEM_PTR_TYPE             pDst,
    EDD_COMMON_MEM_PTR_TYPE             pSrc,
    LSA_UINT32                          length
)
{
    LSAS_MEMCPY(pDst, pSrc, length);
}
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
 * return value:   LSA_TRUE : buffers are the same
 *                 LSA_FALSE: buffers differ
 *===========================================================================*/

#ifndef EDDP_MEMCMP_COMMON
LSA_UINT8  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_MEMCMP_COMMON(
    EDD_COMMON_MEM_PTR_TYPE             pBuf1,
    EDD_COMMON_MEM_PTR_TYPE             pBuf2,
    LSA_UINT32                          length
)
{
    LSA_UINT8   ret_val;

    if (LSAS_MEMCMP(pBuf1, pBuf2, length))
    {
        ret_val = LSA_FALSE;
    }
    else
    {
        ret_val = LSA_TRUE;
    }

    return ret_val;
}
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
    EDD_UPPER_RQB_PTR_TYPE  EDDP_LOCAL_MEM_ATTR *  upper_rqb_ptr_ptr,
    LSA_UINT16                                    length
)
{
    //LSAS_ALLOC_COMP_MEM(upper_rqb_ptr_ptr, length, LSA_COMP_ID_EDDP);
    LSAS_ALLOC_PACKET_MEM(upper_rqb_ptr_ptr, length, LSA_COMP_ID_EDDP);
}
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
    LSA_UINT16                 EDDP_LOCAL_MEM_ATTR *  ret_val_ptr,
    EDD_UPPER_RQB_PTR_TYPE                           upper_rqb_ptr
)
{
    //LSAS_FREE_COMP_MEM(ret_val_ptr, upper_rqb_ptr, LSA_COMP_ID_EDDP);
    LSAS_FREE_PACKET_MEM(ret_val_ptr, upper_rqb_ptr, LSA_COMP_ID_EDDP);
}
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
    EDD_UPPER_MEM_PTR_TYPE  EDDP_LOCAL_MEM_ATTR *  upper_mem_ptr_ptr,
    LSA_UINT16                                    length
)
{
    //LSAS_ALLOC_COMP_MEM(upper_mem_ptr_ptr, length, LSA_COMP_ID_EDDP);
    LSAS_ALLOC_PACKET_MEM(upper_mem_ptr_ptr, length, LSA_COMP_ID_EDDP);
}
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
    LSA_UINT16                 EDDP_LOCAL_MEM_ATTR *  ret_val_ptr,
    EDD_UPPER_MEM_PTR_TYPE                           upper_mem_ptr
)
{
    //LSAS_FREE_COMP_MEM(ret_val_ptr, upper_mem_ptr, LSA_COMP_ID_EDDP);
    LSAS_FREE_PACKET_MEM(ret_val_ptr, upper_mem_ptr, LSA_COMP_ID_EDDP);
}
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
    EDDP_SYS_HANDLE                               hSysDev,
    EDD_UPPER_MEM_PTR_TYPE  EDDP_LOCAL_MEM_ATTR * upper_mem_ptr_ptr,
    LSA_UINT32                                    length
)
{
    LSAS_ALLOC_PACKET_MEM(upper_mem_ptr_ptr, length, LSA_COMP_ID_EDDP);

    LSA_UNUSED_ARG(hSysDev);
}
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
    EDDP_SYS_HANDLE                                  hSysDev,
    LSA_UINT16                 EDDP_LOCAL_MEM_ATTR * ret_val_ptr,
    EDD_UPPER_MEM_PTR_TYPE                           upper_mem_ptr
)
{

    LSAS_FREE_PACKET_MEM(ret_val_ptr, upper_mem_ptr, LSA_COMP_ID_EDDP);

//    *(ret_val_ptr) = LSA_RET_OK;

    LSA_UNUSED_ARG(hSysDev);
}
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
    EDDP_SYS_HANDLE                               hSysDev,
    EDD_UPPER_MEM_PTR_TYPE  EDDP_LOCAL_MEM_ATTR * upper_mem_ptr_ptr,
    LSA_UINT32                                    length
)
{
    LSAS_ALLOC_PACKET_MEM(upper_mem_ptr_ptr, length, LSA_COMP_ID_EDDP);

    LSA_UNUSED_ARG(hSysDev);
}
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
    EDDP_SYS_HANDLE                                  hSysDev,
    LSA_UINT16                 EDDP_LOCAL_MEM_ATTR * ret_val_ptr,
    EDD_UPPER_MEM_PTR_TYPE                           upper_mem_ptr
)
{
    LSAS_FREE_PACKET_MEM(ret_val_ptr, upper_mem_ptr, LSA_COMP_ID_EDDP);

    LSA_UNUSED_ARG(hSysDev);
}
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
    EDDP_SYS_HANDLE                               hSysDev,
    EDD_UPPER_MEM_PTR_TYPE  EDDP_LOCAL_MEM_ATTR * upper_mem_ptr_ptr,
    LSA_UINT32                                    length
)
{
    LSAS_ALLOC_PACKET_MEM(upper_mem_ptr_ptr, length, LSA_COMP_ID_EDDP);

    LSA_UNUSED_ARG(hSysDev);
}
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
    EDDP_SYS_HANDLE                                  hSysDev,
    LSA_UINT16                 EDDP_LOCAL_MEM_ATTR * ret_val_ptr,
    EDD_UPPER_MEM_PTR_TYPE                           upper_mem_ptr
)
{
    LSAS_FREE_PACKET_MEM(ret_val_ptr, upper_mem_ptr, LSA_COMP_ID_EDDP);

    LSA_UNUSED_ARG(hSysDev);
}
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
    EDDP_SYS_HANDLE                     hSysDev,
    EDD_UPPER_MEM_PTR_TYPE              Ptr,
    LSA_UINT32 EDDP_LOCAL_MEM_ATTR *    pAddr
)
{
    * pAddr = (LSA_UINT32)Ptr;

    LSA_UNUSED_ARG(hSysDev);
}
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
    EDDP_SYS_HANDLE                     hSysDev,
    EDD_UPPER_MEM_PTR_TYPE              Ptr,
    LSA_UINT32 EDDP_LOCAL_MEM_ATTR *    pAddr
)
{
    * pAddr = (LSA_UINT32)Ptr;

    LSA_UNUSED_ARG(hSysDev);
}
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
    EDDP_SYS_HANDLE                     hSysDev,
    EDD_UPPER_MEM_PTR_TYPE              Ptr,
    LSA_UINT32 EDDP_LOCAL_MEM_ATTR *    pAddr
)
{
    * pAddr = (LSA_UINT32)Ptr;

    LSA_UNUSED_ARG(hSysDev);
}
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
    EDDP_SYS_HANDLE                     hSysDev,
    EDD_UPPER_MEM_PTR_TYPE              Ptr,
    LSA_UINT32 EDDP_LOCAL_MEM_ATTR *    pAddr
)
{
    * pAddr = (LSA_UINT32)Ptr;

    LSA_UNUSED_ARG(hSysDev);
}
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
    EDDP_SYS_HANDLE                     hSysDev,
    EDD_UPPER_MEM_PTR_TYPE              Ptr,
    LSA_UINT32 EDDP_LOCAL_MEM_ATTR *    pAddr
)
{
    * pAddr = (LSA_UINT32)Ptr;

    LSA_UNUSED_ARG(hSysDev);
}
#endif

/*=============================================================================
 * function name:   EDDP_RX_MEM_ADDR_PNIP_TO_UPPER
 *
 * function:        convert a 32-Bit physical pnip address to upper rx buffer
 *                  memory
 *
 * parameters:      EDDP_SYS_HANDLE                hSysDev
 *                  LSA_UINT32                     Addr
 *                  EDD_UPPER_MEM_PTR_TYPE        *pPtr
 *
 * return value:    LSA_VOID
 *===========================================================================*/

#ifndef EDDP_RX_MEM_ADDR_PNIP_TO_UPPER
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_RX_MEM_ADDR_PNIP_TO_UPPER(
    EDDP_SYS_HANDLE                     hSysDev,
    LSA_UINT32                          Addr,
    EDD_UPPER_MEM_PTR_TYPE         *    pPtr
)
{
    * pPtr = (LSA_VOID *)Addr;

    LSA_UNUSED_ARG(hSysDev);
}
#endif

/*=============================================================================
 * function name:   EDDP_TX_MEM_ADDR_PNIP_TO_UPPER
 *
 * function:        convert a 32-Bit physical pnip address to upper tx buffer
 *                  memory
 *
 * parameters:      EDDP_SYS_HANDLE                hSysDev
 *                  LSA_UINT32                     Addr
 *                  EDD_UPPER_MEM_PTR_TYPE        *pPtr
 *
 * return value:    LSA_VOID
 *===========================================================================*/

#ifndef EDDP_TX_MEM_ADDR_PNIP_TO_UPPER
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_TX_MEM_ADDR_PNIP_TO_UPPER(
    EDDP_SYS_HANDLE                     hSysDev,
    LSA_UINT32                          Addr,
    EDD_UPPER_MEM_PTR_TYPE         *    pPtr
)
{
    * pPtr = (LSA_VOID *)Addr;

    LSA_UNUSED_ARG(hSysDev);
}
#endif

/*=============================================================================
 * function name:   EDDP_DEV_MEM_ADDR_PNIP_TO_UPPER
 *
 * function:        convert a 32-Bit physical pnip address to upper device
 *                  memory
 *
 * parameters:      EDDP_SYS_HANDLE                hSysDev
 *                  LSA_UINT32                     Addr
 *                  EDD_UPPER_MEM_PTR_TYPE        *pPtr
 *
 * return value:    LSA_VOID
 *===========================================================================*/

#ifndef EDDP_DEV_MEM_ADDR_PNIP_TO_UPPER
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_DEV_MEM_ADDR_PNIP_TO_UPPER(
    EDDP_SYS_HANDLE                     hSysDev,
    LSA_UINT32                          Addr,
    EDD_UPPER_MEM_PTR_TYPE         *    pPtr
)
{
    * pPtr = (LSA_VOID *)Addr;

    LSA_UNUSED_ARG(hSysDev);
}
#endif

/*=============================================================================
 * function name:   EDDP_IO_EXTRAM_ADDR_PNIP_TO_UPPER
 *
 * function:        convert a 32-Bit physical pnip address to upper IO buffer
 *                  memory (external RAM)
 *
 * parameters:      EDDP_SYS_HANDLE                hSysDev
 *                  LSA_UINT32                     Addr
 *                  EDD_UPPER_MEM_PTR_TYPE        *pPtr
 *
 * return value:    LSA_VOID
 *===========================================================================*/

#ifndef EDDP_IO_EXTRAM_ADDR_PNIP_TO_UPPER
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_IO_EXTRAM_ADDR_PNIP_TO_UPPER(
    EDDP_SYS_HANDLE                     hSysDev,
    LSA_UINT32                          Addr,
    EDD_UPPER_MEM_PTR_TYPE         *    pPtr
)
{
    * pPtr = (LSA_VOID *)Addr;

    LSA_UNUSED_ARG(hSysDev);
}
#endif

/*=============================================================================
 * function name:   EDDP_IO_PERIF_ADDR_PNIP_TO_UPPER
 *
 * function:        convert a 32-Bit physical pnip address to upper IO buffer
 *                  memory (PERIF)
 *
 * parameters:      EDDP_SYS_HANDLE                hSysDev
 *                  LSA_UINT32                     Addr
 *                  EDD_UPPER_MEM_PTR_TYPE        *pPtr
 *
 * return value:    LSA_VOID
 *===========================================================================*/

#ifndef EDDP_IO_PERIF_ADDR_PNIP_TO_UPPER
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_IO_PERIF_ADDR_PNIP_TO_UPPER(
    EDDP_SYS_HANDLE                     hSysDev,
    LSA_UINT32                          Addr,
    EDD_UPPER_MEM_PTR_TYPE         *    pPtr
)
{
    * pPtr = (LSA_VOID *)Addr;

    LSA_UNUSED_ARG(hSysDev);
}
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
 * function:       Protect sequences from beeing interrupted by ISR-context of
 *                 edd (eddp_interrupt()).
 *
 * parameters:     LSA_VOID
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_ENTER_INT
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_ENTER_INT(
    LSA_VOID
)
{
    LSAS_OS_SEM_TAKE_EDD_LOCK_IO();
}
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
)
{
    LSAS_OS_SEM_GIVE_EDD_LOCK_IO();
}
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
)
{
    LSAS_OS_SEM_TAKE_EDD_LOCK_IO();
}
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
)
{
    LSAS_OS_SEM_GIVE_EDD_LOCK_IO();
}
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
)
{
    LSAS_OS_SEM_TAKE_EDD_LOCK_I2C();
}
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
)
{
    LSAS_OS_SEM_GIVE_EDD_LOCK_I2C();
}
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
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_I2C_SELECT(
    LSA_UINT8 EDDP_LOCAL_MEM_ATTR  *  const  ret_val_ptr,
    EDDP_SYS_HANDLE                   const  hSysDev,
    LSA_UINT16                        const  PortId,
    LSA_UINT16                        const  I2CMuxSelect
)
{
    LSAS_EDDP_I2C_SELECT(ret_val_ptr, (LSA_UINT32)hSysDev, PortId,I2CMuxSelect);
}
#endif

/*=============================================================================
 * function name:  EDDP_DO_TIMER_REQUEST
 *
 * function:       Does trigger a eddp_system() request with the spezified RQB.
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
    EDDP_SYS_HANDLE           hSysDev,
    EDD_UPPER_RQB_PTR_TYPE    pRQB
)
{
    pRQB->comp_id = LSA_COMP_ID_EDDP; /* so this RQB can be differed */
    //UsrDoEddpService( &RQB );
    LSAS_RQB_EDD_REQUEST(pRQB, eddp_system);

    LSA_UNUSED_ARG(hSysDev);
}
#endif

/*=============================================================================
 * function name:  EDDP_DO_INTERRUPT_REQUEST
 *
 * function:       Does trigger a eddp_system() request with the spezified RQB.
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
    EDDP_SYS_HANDLE           hSysDev,
    EDD_UPPER_RQB_PTR_TYPE    pRQB
)
{
    //sha SYSP_POST_EDDP_REQUEST(pRQB);
    pRQB->comp_id = LSA_COMP_ID_EDDP; /* so this RQB can be differed */
    //LSA_RQB_SET_OPCODE(pRQB, EDD_OPC_REQUEST_INTERRUPT);
    LSAS_RQB_EDD_REQUEST(pRQB, eddp_system);

    LSA_UNUSED_ARG(hSysDev);
}
#endif


/*****************************************************************************/
/*                                                                           */
/*                   Prototypes for PHY interface                            */
/*                                                                           */
/*****************************************************************************/


/*=============================================================================
 * function name:  EDDP_K32_RESET
 *
 * function:       set/clear reset of KRISC32.
 *                 The TCM is not affected by this function.
 *
 * parameters:     LSA_BOOL on  reset should be set (LAS_TRUE)
 *                 or cleared (LSA_FALSE).
 *
 * return value:   none
 *===========================================================================*/
#ifndef EDDP_K32_RESET
LSA_VOID EDDP_SYSTEM_OUT_FCT_ATTR EDDP_K32_RESET(
    EDDP_SYS_HANDLE        hSysDev,
    LSA_BOOL                on)
{
    if( LSA_FALSE == on )
    {
        // release asynchronous reset for KRISC32 core
      //TODO:  clear_bit__( REG32(U_SCRB__ASYN_RES_CTRL_REG), U_SCRB__ASYN_RES_CTRL_REG__RES_SOFT_KRISC32_CORE );
    }
    else
    {
        // set asynchronous reset for KRISC32 core
    	//TODO:  set_bit__( REG32(U_SCRB__ASYN_RES_CTRL_REG), U_SCRB__ASYN_RES_CTRL_REG__RES_SOFT_KRISC32_CORE );
    }
}
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
 *
 *                 The following values within PHY_CONFIG must be setup:
 *
 *                 Bit    Identifier          access    Value   comment
 *                 -----------------------------------------------------------------------
 *                 0      P1_PHY_ENB          rw        1       shall be 1
 *                 1      P1_FX_MODE          rw        0/1     depends on hardware
 *                 2..5   P1_PHY_MODE         rw        0b1111  shall be 0b1111 (loopback)
 *                 6      P1_PHY_AUTOMDIXEN   rw        1       shall be 1
 *                 7      reserved            -
 *                 8      P2_PHY_ENB          rw        1       shall be 1
 *                 9      P2_FX_MODE          rw        0/1     depends on hardware
 *                 10..13 P2_PHY_MODE         r         -       reserved
 *                 14     P2_PHY_AUTOMDIXEN   rw        1       shall be 1
 *                 15     reserved            -
 *
 *                 with FX_MODE = 0:  PHY_CONFIG = 0x7D7D
 *                 with FX_MODE = 1:  PHY_CONFIG = 0x7F7F
 *
 *
 * parameters:     EDDP_SYS_HANDLE          hSysDev
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#define SCRB_PHY_CONFIG_REG_OFF     0x003C
#ifndef EDDP_ERTEC200P_INIT_PHY_CONFIG_REG
LSA_VOID EDDP_SYSTEM_OUT_FCT_ATTR EDDP_ERTEC200P_INIT_PHY_CONFIG_REG(
		LSA_UINT32 			hSysDev,
		LSA_UINT8		   Cmd,
		LSA_UINT32		   HWPortID)
{
    //HWxWriteScrbReg(0x003C, 0x7D7D);    // Write PHY_CONFIG Register within SCRB
	//TODO: WRITE_USHORT(U_SCRB__PHY_CONFIG, 0x7D7D);
    LSA_UNUSED_ARG(hSysDev);
}
#endif

/*=============================================================================
 * function name:  EDDP_SIGNAL_SENDCLOCK_CHANGE
 *
 * function:       informs the application about a changed sendclock
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef EDDP_SIGNAL_SENDCLOCK_CHANGE
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_SIGNAL_SENDCLOCK_CHANGE(EDDP_SYS_HANDLE     hSysDev,
                                                                  LSA_UINT32          CycleBaseFactor,
                                                                  LSA_UINT8           Mode)
{
  if(Mode == EDDP_SENDCLOCK_CHANGE_MODE_FINISHED)
  {
      LSAS_SET_SENDCLOCK_REDUCTION(CycleBaseFactor);
  }
}
#endif

/*=============================================================================
 * macro name   : EDDP_SIGNAL_TIME_SYNC_STATE_CHANGE
 * function     : Signal a change of the Time Sync State to the system adaption
 * parameters   : -
 * return value : no
 *===========================================================================*/
#ifndef EDDP_SIGNAL_TIME_SYNC_STATE_CHANGE
LSA_VOID  EDDP_SYSTEM_OUT_FCT_ATTR  EDDP_SIGNAL_TIME_SYNC_STATE_CHANGE()
{
    LSAS_SIGNAL_TIME_SYNC_STATE_CHANGE();
}
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
                                                     LSA_UINT32       ticks_10ns )
{
    LSAS_WAIT_10NS(ticks_10ns);
}
#endif

LSA_UINT16 eddp_version ( LSA_UINT16 length, LSA_VERSION_TYPE *version )
{
  LSA_UINT8 *p_version, *p_v;
  LSA_UINT16 i;

  /* Initialize version struct */
  LSA_VERSION_TYPE v = {
                        PNIO_LSA_COMPONENT_ID, {'L','S','A','_','E','D','D','P','\0','\0'},
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

#endif /* LSAS_CFG_USE_EDDP */
/*****************************************************************************/
/*  end of file EDD_OUT.C                                                    */
/*****************************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
