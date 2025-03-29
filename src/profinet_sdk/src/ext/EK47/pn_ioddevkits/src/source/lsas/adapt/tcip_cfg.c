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
/*  F i l e               &F: tcip_cfg.c                                :F&  */
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

#define LSAS_MODULE_ID  28

//#include "apma_inc.h"
#include "lsas_inc.h"
#include "lsas_int.h"

#ifndef LSAS_CFG_USE_EXTERNAL_TCPIP_STACK

#include "tcip_trc.h"
#include "tcip_low.h"
#include "tcip_pls.h"

#ifdef NPDEBUG
  #define DBUFLINES                 100     /* Number of trace lines */
#else
  #define DBUFLINES                 1
#endif
#define DBUFLEN                     132     /* Length of trace line */

char dbuf[DBUFLINES][DBUFLEN];  /*dprintf*/



/********************************************************************************/
/* functions of stack                                                           */
/********************************************************************************/




/*------------------------------------------------------------------------------*/
/*    F U N C T I O N - Description:                                            */
/*                                                                              */
/*    Function for tcp/ip stack.                                                */
/*  > input parameter:                                                          */
/*      - none                                                                  */
/*  > output parameter:                                                         */
/*      - none                                                                  */
/*------------------------------------------------------------------------------*/
#if 0
#ifndef dtrap
void dtrap(void)
{
    return;
}
#endif


/*------------------------------------------------------------------------------*/
/*    F U N C T I O N - Description:                                            */
/*                                                                              */
/*    Function for tcp/ip stack.                                                */
/*  > input parameter:                                                          */
/*      - fmt: Pointer to print format                                          */
/*      - ...: print parameter                                                  */
/*  > output parameter:                                                         */
/*      - none                                                                  */
/*------------------------------------------------------------------------------*/
void dprintf(char *fmt, ... )
{
#if 0
   va_list a;

   va_start(a, fmt);

   if (dbuf_ix >= DBUFLINES)
      dbuf_ix = 0;
   vsprintf(&dbuf[dbuf_ix][0], fmt, a);
   dbuf_ix++;

   va_end(a);
#endif
}
#endif
#if 0
/*------------------------------------------------------------------------------*/
/*    F U N C T I O N - Description:                                            */
/*                                                                              */
/*    Function for tcp/ip stack.                                                */
/*  > input parameter:                                                          */
/*      - msg: ip address                                                       */
/*  > output parameter:                                                         */
/*      - char pointer to ip string                                             */
/*------------------------------------------------------------------------------*/
char     ipreturn[18];     /* buffer for return */

char * print_ipad(unsigned long ipaddr)
{
   struct l2b  ip;

   ip.ip.iplong = ipaddr;
   LSAS_SPRINTF(ipreturn, "%u.%u.%u.%u",
    ip.ip.ipchar[0],
    ip.ip.ipchar[1],
    ip.ip.ipchar[2],
    ip.ip.ipchar[3]);

   return ipreturn;
}
#endif

/******************************************************************************
 ******************************************************************************
 ******************************************************************************
 ******************************************************************************
 ***********************      user out adaption     ***************************
 ******************************************************************************
 ******************************************************************************
 ******************************************************************************
 ******************************************************************************/

/*=============================================================================
 * function name:  TCIP_REQUEST_UPPER_DONE
 *
 * function:       returns a finished request to the upper layer
 *
 * parameters:     TCIP_UPPER_CALLBACK_FCT_PTR_TYPE
 *                     tcip_request_upper_done_ptr:
 *                     pointer to tcip_request_upper_done_ptr, given by
 *                     tcip_open_channel()
 *
 *                 TCIP_UPPER_RQB_PTR_TYPE  upper_rqb_ptr:  pointer to upper-RQB
 *                 LSA_SYS_PTR_TYPE       sys_ptr:        system-pointer
 *
 * return value:   LSA_VOID
 *
 *===========================================================================*/
#ifndef TCIP_REQUEST_UPPER_DONE
LSA_VOID  TCIP_UPPER_OUT_FCT_ATTR  TCIP_REQUEST_UPPER_DONE(
    TCIP_UPPER_CALLBACK_FCT_PTR_TYPE  tcip_request_upper_done_ptr,
    TCIP_UPPER_RQB_PTR_TYPE           upper_rqb_ptr,
    LSA_SYS_PTR_TYPE                  sys_ptr
)
{
    tcip_request_upper_done_ptr(upper_rqb_ptr);
}

#endif


/*=============================================================================
 * function name:  TCIP_ALLOC_UPPER_RQB_LOCAL
 *
 * function:       allocate an upper rqb for usage in tcip
 *
 * documentation:  LSA_TCIP_Detailspec.doc
 *
 *===========================================================================*/
#ifndef TCIP_ALLOC_UPPER_RQB_LOCAL
LSA_VOID  TCIP_SYSTEM_OUT_FCT_ATTR TCIP_ALLOC_UPPER_RQB_LOCAL(
    TCIP_UPPER_RQB_PTR_TYPE  TCIP_LOCAL_MEM_ATTR * upper_rqb_ptr_ptr,
    LSA_UINT16                                     length
)
{
    LSAS_ALLOC_COMP_MEM(upper_rqb_ptr_ptr,length, LSA_COMP_ID_TCIP);
}
#endif

/*=============================================================================
 * function name:  TCIP_FREE_UPPER_RQB_LOCAL
 *
 * function:       allocate an upper rqb for usage in tcip
 *
 * documentation:  LSA_TCIP_Detailspec.doc
 *
 *===========================================================================*/
#ifndef TCIP_FREE_UPPER_RQB_LOCAL
LSA_VOID  TCIP_SYSTEM_OUT_FCT_ATTR TCIP_FREE_UPPER_RQB_LOCAL(
    LSA_UINT16                 TCIP_LOCAL_MEM_ATTR * ret_val_ptr,
    TCIP_UPPER_RQB_PTR_TYPE                          upper_rqb_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, upper_rqb_ptr, LSA_COMP_ID_TCIP);
}
#endif


/*=============================================================================
 * function name:  TCIP_ALLOC_UPPER_RQB
 *
 * function:       allocate an upper-RQB
 *
 * documentation:  LSA_TCIP_Detailspec.doc
 *
 *===========================================================================*/
#ifndef TCIP_ALLOC_UPPER_RQB
LSA_VOID  TCIP_SYSTEM_OUT_FCT_ATTR
TCIP_ALLOC_UPPER_RQB(
    TCIP_UPPER_RQB_PTR_TYPE  TCIP_LOCAL_MEM_ATTR   *  upper_rqb_ptr_ptr,
    LSA_USER_ID_TYPE                              user_id,
    LSA_UINT16                                    length,
    LSA_SYS_PTR_TYPE                              sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(upper_rqb_ptr_ptr,length, LSA_COMP_ID_TCIP);
}
#endif


/*=============================================================================
 * function name:  TCIP_FREE_UPPER_RQB
 *
 * function:       free an upper-RQB
 *
 * documentation:  LSA_TCIP_Detailspec.doc
 *
 *===========================================================================*/
#ifndef TCIP_FREE_UPPER_RQB
LSA_VOID  TCIP_SYSTEM_OUT_FCT_ATTR
TCIP_FREE_UPPER_RQB(
    LSA_UINT16                 TCIP_LOCAL_MEM_ATTR   *  ret_val_ptr,
    TCIP_UPPER_RQB_PTR_TYPE                             upper_rqb_ptr,
    LSA_SYS_PTR_TYPE                                  sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, upper_rqb_ptr, LSA_COMP_ID_TCIP);
}
#endif


/*=============================================================================
 * function name:  TCIP_REQUEST_LOCAL
 *
 * function:       send a request to "self"
 *
 * documentation:  LSA_TCIP_Detailspec.doc
 *
 * maybe this should go into tcip_sys.h, but there is no upper-rqb-type...
 *
 *===========================================================================*/

#ifndef TCIP_REQUEST_LOCAL
LSA_VOID  TCIP_SYSTEM_OUT_FCT_ATTR
TCIP_REQUEST_LOCAL(
    TCIP_UPPER_RQB_PTR_TYPE  upper_rqb_ptr
)
{

    LSAS_RQB_TCPIP_REQUEST(upper_rqb_ptr, tcip_request);
}
#endif

/******************************************************************************
 ******************************************************************************
 ******************************************************************************
 ******************************************************************************
 ***********************       sys out adaption     ***************************
 ******************************************************************************
 ******************************************************************************
 ******************************************************************************
 ******************************************************************************/

/*=============================================================================
 * function name:  TCIP_GET_PATH_INFO
 *
 * function:       get system-pointer and detail-pointer of a communication
 *                 channel
 *
 * documentation:  LSA_TCIP_Detailspec.doc
 *
 *===========================================================================*/
#ifndef TCIP_GET_PATH_INFO
LSA_VOID  TCIP_SYSTEM_OUT_FCT_ATTR
TCIP_GET_PATH_INFO(
    LSA_UINT16              TCIP_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_SYS_PTR_TYPE        TCIP_LOCAL_MEM_ATTR *  sys_ptr_ptr,
    TCIP_DETAIL_PTR_TYPE    TCIP_LOCAL_MEM_ATTR *  detail_ptr_ptr,
    LSA_SYS_PATH_TYPE       path
)
{
    LSA_GET_PATH_INFO(ret_val_ptr, sys_ptr_ptr,
                      detail_ptr_ptr, path, LSAS_PATH_LAYER_TCIP);
}
#endif


/*=============================================================================
 * function name:  TCIP_RELEASE_PATH_INFO
 *
 * function:       release system-pointer and detail-pointer of a communication
 *                 channel
 *
 * documentation:  LSA_TCIP_Detailspec.doc
 *
 *===========================================================================*/
#ifndef TCIP_RELEASE_PATH_INFO
LSA_VOID  TCIP_SYSTEM_OUT_FCT_ATTR
TCIP_RELEASE_PATH_INFO(
    LSA_UINT16 TCIP_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_SYS_PTR_TYPE                  sys_ptr,
    TCIP_DETAIL_PTR_TYPE              detail_ptr
)
{
    LSA_RELEASE_PATH_INFO(ret_val_ptr, sys_ptr, detail_ptr);
}
#endif


/*=============================================================================
 * function name:  TCIP_ALLOC_LOCAL_MEM
 *
 * function:       allocate a local-memory
 *
 * documentation:  LSA_TCIP_Detailspec.doc
 *
 *===========================================================================*/
#ifndef TCIP_ALLOC_LOCAL_MEM
LSA_VOID  TCIP_SYSTEM_OUT_FCT_ATTR
TCIP_ALLOC_LOCAL_MEM(
    TCIP_LOCAL_MEM_PTR_TYPE  TCIP_LOCAL_MEM_ATTR *  local_mem_ptr_ptr,
    LSA_UINT16                                      length
)
{
    LSAS_ALLOC_COMP_MEM(local_mem_ptr_ptr,length, LSA_COMP_ID_TCIP);
}
#endif


/*=============================================================================
 * function name:  TCIP_FREE_LOCAL_MEM
 *
 * function:       free a local-memory
 *
 * documentation:  LSA_TCIP_Detailspec.doc
 *
 *===========================================================================*/
#ifndef TCIP_FREE_LOCAL_MEM
LSA_VOID  TCIP_SYSTEM_OUT_FCT_ATTR
TCIP_FREE_LOCAL_MEM(
    LSA_UINT16                 TCIP_LOCAL_MEM_ATTR *  ret_val_ptr,
    TCIP_LOCAL_MEM_PTR_TYPE                           local_mem_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, local_mem_ptr, LSA_COMP_ID_TCIP);
}
#endif


/*=============================================================================
 * function name:  TCIP_STRLEN
 *
 * function:       Returns the length of str.
 *                 The length of a C string is determined by the terminating
 *                 null-character: A C string is as long as the amount of
 *                 characters between the beginning of the string and the
 *                 terminating null character.
 *============================================================================*/
#ifndef TCIP_STRLEN
LSA_UINT32  TCIP_SYSTEM_OUT_FCT_ATTR
TCIP_STRLEN(
    const LSA_CHAR LSA_COMMON_MEM_ATTR * p_str
)
{
    return LSAS_STRLEN(p_str);
}
#endif


/*=============================================================================
 * function name:  TCIP_MEMSET, TCIP_MEMCPY, TCIP_MEMCMP
 *
 * function:       macro for stdlib function memset
 *
 * important:      WE WILL IMPLEMENT variants for mem-attribs
 *                 when needed. All known platforms are "flat"
 *
 *===========================================================================*/

#ifndef TCIP_MEMSET
LSA_VOID  TCIP_SYSTEM_OUT_FCT_ATTR
TCIP_MEMSET(
    LSA_VOID_PTR_TYPE ptr,
    LSA_INT val,
    LSA_UINT len
)
{
    LSAS_MEMSET(ptr,val,len);
}
#endif

#ifndef TCIP_MEMCPY
LSA_VOID  TCIP_SYSTEM_OUT_FCT_ATTR
TCIP_MEMCPY(
    LSA_VOID_PTR_TYPE dst,
    LSA_VOID_PTR_TYPE src,
    LSA_UINT len
)
{
    LSAS_MEMCPY(dst,src,len);       // lint !e668 Possibly passing a null pointer to function memcpy
}
#endif

#ifndef TCIP_MEMMOVE
LSA_VOID  TCIP_SYSTEM_OUT_FCT_ATTR
TCIP_MEMMOVE(
    LSA_VOID_PTR_TYPE dst,
    LSA_VOID_PTR_TYPE src,
    LSA_UINT len
)
{
    LSAS_MEMMOVE(dst,src,len);
}
#endif

#ifndef TCIP_MEMCMP
LSA_INT TCIP_SYSTEM_OUT_FCT_ATTR
TCIP_MEMCMP(
    LSA_VOID_PTR_TYPE buf1,
    LSA_VOID_PTR_TYPE buf2,
    LSA_UINT len
)
{
   return LSAS_MEMCMP(buf1, buf2, len);
}
#endif


/*=============================================================================
 * function name:  TCIP_ALLOC_TIMER
 *
 * function:       allocate a timer
 *
 * documentation:  LSA_TCIP_Detailspec.doc
 *
 *===========================================================================*/
#ifndef TCIP_ALLOC_TIMER
LSA_VOID  TCIP_SYSTEM_OUT_FCT_ATTR
TCIP_ALLOC_TIMER(
    LSA_UINT16  TCIP_LOCAL_MEM_ATTR        *  ret_val_ptr,
    LSA_TIMER_ID_TYPE  TCIP_LOCAL_MEM_ATTR *  timer_id_ptr,
    LSA_UINT16                              timer_type,
    LSA_UINT16                              time_base
)
{
    LSAS_ALLOC_TIMER(ret_val_ptr, timer_id_ptr, timer_type, time_base, tcip_timeout, LSA_COMP_ID_TCIP);
}
#endif


/*=============================================================================
 * function name:  TCIP_START_TIMER
 *
 * function:       start a timer
 *
 * documentation:  LSA_TCIP_Detailspec.doc
 *
 *===========================================================================*/
#ifndef TCIP_START_TIMER
LSA_VOID  TCIP_SYSTEM_OUT_FCT_ATTR
TCIP_START_TIMER(
    LSA_UINT16        TCIP_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_TIMER_ID_TYPE                      timer_id,
    LSA_USER_ID_TYPE                       user_id,
    LSA_UINT16                             time
)
{
    LSAS_START_TIMER(ret_val_ptr, timer_id, user_id, time, LSA_COMP_ID_TCIP);
}
#endif


/*=============================================================================
 * function name:  TCIP_STOP_TIMER
 *
 * function:       stop a timer
 *
 * documentation:  LSA_TCIP_Detailspec.doc
 *
 *===========================================================================*/
#ifndef TCIP_STOP_TIMER
LSA_VOID  TCIP_SYSTEM_OUT_FCT_ATTR
TCIP_STOP_TIMER(
    LSA_UINT16  TCIP_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_TIMER_ID_TYPE                timer_id
)
{
    LSAS_STOP_TIMER(ret_val_ptr, timer_id, LSA_COMP_ID_TCIP);
}
#endif


/*=============================================================================
 * function name:  TCIP_FREE_TIMER
 *
 * function:       free a timer
 *
 * documentation:  LSA_TCIP_Detailspec.doc
 *
 *===========================================================================*/
#ifndef TCIP_FREE_TIMER
LSA_VOID  TCIP_SYSTEM_OUT_FCT_ATTR
TCIP_FREE_TIMER(
    LSA_UINT16  TCIP_LOCAL_MEM_ATTR *  ret_val_ptr,
    LSA_TIMER_ID_TYPE                timer_id
)
{
    LSAS_FREE_TIMER(ret_val_ptr, timer_id, LSA_COMP_ID_TCIP);
}
#endif


/*=============================================================================
 * function name:  TCIP_ARP_RESPONSE_RECEIVED
 *
 * function:       is called when an ARP frame with opcode 2 is received
 *                 intended useage: ARP sniffer, see nare_ARPResponseFrameIn()
 *
 * documentation:  LSA_TCIP_Detailspec.doc
 *
 *===========================================================================*/
extern LSA_UINT32 glob_nare_sniffer;
#ifndef TCIP_ARP_RESPONSE_RECEIVED
LSA_VOID TCIP_ARP_RESPONSE_RECEIVED(
    LSA_UINT16                 group_id, /* must have the same value as NARE_CDB_PATH_PARAMS_TYPE::NICId */
    EDD_UPPER_MEM_U8_PTR_TYPE  frame_buf,
    LSA_UINT16                 frame_len
)
{

    /* nare_ARPResponseFrameIn uses enter exit, if configured, as it runs in a different task */
    if (glob_nare_sniffer == LSA_TRUE)
    {
        nare_ARPResponseFrameIn(group_id, frame_buf, frame_len);
    }
}
#endif

/*=============================================================================
 * function name:  TCIP_FATAL_ERROR
 *
 * function:       notify a fatal-error
 *
 * documentation:  LSA_TCIP_Detailspec.doc
 *
 *===========================================================================*/
#ifndef TCIP_FATAL_ERROR
LSA_VOID  TCIP_SYSTEM_OUT_FCT_ATTR
TCIP_FATAL_ERROR(
    LSA_UINT16                  length,
    TCIP_FATAL_ERROR_PTR_TYPE   error_ptr
)
{
    LSAS_FATAL_ERROR( error_ptr);
}
#endif


/*=============================================================================
 * function name:  TCIP_EXCHANGE_LONG
 *
 * function:       thread-save exchange of a "long", returns the previous value
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef TCIP_EXCHANGE_LONG
long  TCIP_SYSTEM_OUT_FCT_ATTR
TCIP_EXCHANGE_LONG(
    long volatile *ptr,
    long val
)
{
    /*
     * used to set/reset inused flag of timer rqb
     * don't see any need to do this with interrupts blocked
     */
    long ret_val;

    LSAS_TCIP_ENTER();

    ret_val = *ptr;

    *ptr = val;

    LSAS_TCIP_EXIT();

    return ret_val;
}
#endif

/******************************************************************************
 ******************************************************************************
 ******************************************************************************
 ******************************************************************************
 ***********************      lower out adaption    ***************************
 ******************************************************************************
 ******************************************************************************
 ******************************************************************************
 ******************************************************************************/

/*=============================================================================
 * function name:  TCIP_OPEN_CHANNEL_LOWER
 *
 * function:       open a communication channel to lower layer
 *
 * documentation:  LSA_TCIP_Detailspec.doc
 *
 *===========================================================================*/

#ifndef TCIP_EDD_OPEN_CHANNEL_LOWER
LSA_VOID TCIP_EDD_LOWER_OUT_FCT_ATTR
TCIP_EDD_OPEN_CHANNEL_LOWER(
    TCIP_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE             sys_ptr
)
{
    EDD_UPPER_OPEN_CHANNEL_PTR_TYPE edd_open = lower_rqb_ptr->pParam;

    edd_open->Cbf = lsas_edd_to_tcip_cbf;
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
#endif


/*=============================================================================
 * function name:  TCIP_CLOSE_CHANNEL_LOWER
 *
 * function:       close a communication channel to lower layer
 *
 * documentation:  LSA_TCIP_Detailspec.doc
 *
 *===========================================================================*/

#ifndef TCIP_EDD_CLOSE_CHANNEL_LOWER
LSA_VOID  TCIP_EDD_LOWER_OUT_FCT_ATTR
TCIP_EDD_CLOSE_CHANNEL_LOWER(
    TCIP_EDD_LOWER_RQB_PTR_TYPE     lower_rqb_ptr,
    LSA_SYS_PTR_TYPE                sys_ptr
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
#endif


/*=============================================================================
 * function name:  TCIP_REQUEST_LOWER
 *
 * function:       send a request to a communication channel to lower layer
 *
 *
 * documentation:  LSA_TCIP_Detailspec.doc
 *
 *===========================================================================*/

#ifndef TCIP_EDD_REQUEST_LOWER
LSA_VOID  TCIP_EDD_LOWER_OUT_FCT_ATTR
TCIP_EDD_REQUEST_LOWER(
    TCIP_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE           sys_ptr
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
#endif

/*=============================================================================
 * function name:  TCIP_ALLOC_LOWER_RQB
 *
 * function:       allocate a lower-RQB
 *
 * documentation:  LSA_TCIP_Detailspec.doc
 *
 *===========================================================================*/

#ifndef TCIP_EDD_ALLOC_LOWER_RQB
LSA_VOID  TCIP_SYSTEM_OUT_FCT_ATTR
TCIP_EDD_ALLOC_LOWER_RQB(
    TCIP_EDD_LOWER_RQB_PTR_TYPE  TCIP_LOCAL_MEM_ATTR   *  lower_rqb_ptr_ptr,
    LSA_USER_ID_TYPE                                  user_id,
    LSA_UINT16                                        length,
    LSA_SYS_PTR_TYPE                                  sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(lower_rqb_ptr_ptr,length, LSA_COMP_ID_TCIP);
}
#endif

/*=============================================================================
 * function name:  TCIP_FREE_LOWER_RQB
 *
 * function:       free a lower-RQB
 *
 * documentation:  LSA_TCIP_Detailspec.doc
 *
 *===========================================================================*/

#ifndef TCIP_EDD_FREE_LOWER_RQB
LSA_VOID  TCIP_SYSTEM_OUT_FCT_ATTR
TCIP_EDD_FREE_LOWER_RQB(
    LSA_UINT16                 TCIP_LOCAL_MEM_ATTR *  ret_val_ptr,
    TCIP_EDD_LOWER_RQB_PTR_TYPE                       lower_rqb_ptr,
    LSA_SYS_PTR_TYPE                                  sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_rqb_ptr, LSA_COMP_ID_TCIP);
}
#endif

/*=============================================================================
 * function name:  TCIP_EDD_ALLOC_LOWER_MEM
 *
 * function:       allocate a lower-memory
 *
 * documentation:  LSA_TCIP_Detailspec.doc
 *
 *===========================================================================*/

#ifndef TCIP_EDD_ALLOC_LOWER_MEM
LSA_VOID  TCIP_SYSTEM_OUT_FCT_ATTR
TCIP_EDD_ALLOC_LOWER_MEM(
    TCIP_EDD_LOWER_MEM_PTR_TYPE  TCIP_LOCAL_MEM_ATTR    *  lower_mem_ptr_ptr,
    LSA_USER_ID_TYPE                                    user_id,
    LSA_UINT16                                          length,
    LSA_SYS_PTR_TYPE                                    sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(lower_mem_ptr_ptr,length, LSA_COMP_ID_TCIP);
}
#endif

/*=============================================================================
 * function name:  TCIP_EDD_FREE_LOWER_MEM
 *
 * function:       free a lower-memory
 *
 * documentation:  LSA_TCIP_Detailspec.doc
 *
 *===========================================================================*/

#ifndef TCIP_EDD_FREE_LOWER_MEM
LSA_VOID  TCIP_SYSTEM_OUT_FCT_ATTR
TCIP_EDD_FREE_LOWER_MEM(
    LSA_UINT16                 TCIP_LOCAL_MEM_ATTR  *  ret_val_ptr,
    TCIP_EDD_LOWER_MEM_PTR_TYPE                     lower_mem_ptr,
    LSA_SYS_PTR_TYPE                                sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_mem_ptr, LSA_COMP_ID_TCIP);
}
#endif

/*===========================================================================*/

LSA_VOID TCIP_EDD_ALLOC_RX_TRANSFER_BUFFER_MEM(
    TCIP_EDD_LOWER_MEM_PTR_TYPE * lower_mem_ptr_ptr,
    LSA_UINT16                    length,
    LSA_SYS_PTR_TYPE              sys_ptr )
{
    LSAS_ALLOC_PACKET_MEM(lower_mem_ptr_ptr, length, LSA_COMP_ID_TCIP);

    LSA_UNUSED_ARG(sys_ptr);
}

/*===========================================================================*/

LSA_VOID TCIP_EDD_FREE_RX_TRANSFER_BUFFER_MEM(
    LSA_UINT16                  * ret_val_ptr,
    TCIP_EDD_LOWER_MEM_PTR_TYPE   lower_mem_ptr,
    LSA_SYS_PTR_TYPE              sys_ptr )
{
    LSAS_FREE_PACKET_MEM(ret_val_ptr, lower_mem_ptr, LSA_COMP_ID_TCIP);
    LSA_UNUSED_ARG(sys_ptr);
}

/*===========================================================================*/

LSA_VOID TCIP_EDD_ALLOC_TX_TRANSFER_BUFFER_MEM(
    TCIP_EDD_LOWER_MEM_PTR_TYPE * lower_mem_ptr_ptr,
    LSA_UINT16                    length,
    LSA_SYS_PTR_TYPE              sys_ptr )
{
    LSAS_ALLOC_PACKET_MEM(lower_mem_ptr_ptr, length, LSA_COMP_ID_TCIP);

    LSA_UNUSED_ARG(sys_ptr);
}

/*===========================================================================*/

LSA_VOID TCIP_EDD_FREE_TX_TRANSFER_BUFFER_MEM(
    LSA_UINT16                  * ret_val_ptr,
    TCIP_EDD_LOWER_MEM_PTR_TYPE   lower_mem_ptr,
    LSA_SYS_PTR_TYPE              sys_ptr )
{
    LSAS_FREE_PACKET_MEM(ret_val_ptr, lower_mem_ptr, LSA_COMP_ID_TCIP);
    LSA_UNUSED_ARG(sys_ptr);
}


/*=============================================================================
 * function name:  SOCK_ENTER_INTERNICHE
 *
 * function:       set reentrance lock
 *                 Start of uninterruptible region for resource ID res.
 *
 * parameters:     resource ID
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef LOCK_NET_RESOURCE
LSA_VOID LOCK_NET_RESOURCE(
    LSA_INT res
)
{

    LSAS_OS_LOCK_ON_TCPIP_RESOURCE(res);

}
#else
#pragma error "by design a function"
#endif

/*=============================================================================
 * function name:  SOCK_EXIT_INTERNICHE
 *
 * function:       cancel reentrance lock
 *                 End of uninterruptible region for resource ID res.
 *
 * parameters:     resource ID
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef UNLOCK_NET_RESOURCE
LSA_VOID UNLOCK_NET_RESOURCE(
    LSA_INT res
)
{

    LSAS_OS_RELEASE_TCPIP_RESOURCE(res);

}
#else
#pragma error "by design a function"
#endif

extern void tcip_iniche_panic(char *file, int line);

void  tcp_sleep(void * timeout)
{
    tcip_iniche_panic(__FILE__, __LINE__);
}


void  tcp_wakeup(void * wake)
{
    TCIP_TRIGGER_SELECT();
    //tcip_iniche_panic(__FILE__, __LINE__);
}

LSA_UINT16 tcpip_version ( LSA_UINT16 length, LSA_VERSION_TYPE *version )
{
  LSA_UINT8 *p_version, *p_v;
  LSA_UINT16 i;

  /* Initialize version struct */
  LSA_VERSION_TYPE v = {
                        PNIO_LSA_COMPONENT_ID, {'L','S','A','_','T','C','P','I','P','\0'},
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

#endif
/*****************************************************************************/
/*  end of file.                                                             */
/*****************************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
