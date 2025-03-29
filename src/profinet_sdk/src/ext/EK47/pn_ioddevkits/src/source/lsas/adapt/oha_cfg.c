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
/*  F i l e               &F: oha_cfg.c                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Implements system integration of OHA using the pcPNIO framework.         */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
/*  20.07.04    VE    initial version.                                       */
/*****************************************************************************/

#define LSAS_MODULE_ID  24

//#define LTRC_ACT_MODUL_ID  16
//#define OHA_MODULE_ID      LTRC_ACT_MODUL_ID /* OHA_MODULE_ID_OHA_CFG */

#include "lsas_inc.h"
#include "lsas_int.h"
#include "lsas_dat.h"

#include "oha_trc.h"

/* BTRACE-IF */
OHA_FILE_SYSTEM_EXTENSION(OHA_MODULE_ID)


#ifndef OHA_ALLOC_TIMER
LSA_VOID  OHA_ALLOC_TIMER(
    LSA_UINT16  *ret_val_ptr,
    LSA_UINT16  *timer_id_ptr,
    LSA_UINT16   timer_type,
    LSA_UINT16   time_base
)
{
    LSAS_ALLOC_TIMER(ret_val_ptr, timer_id_ptr, timer_type, time_base, oha_timeout, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

#ifndef OHA_START_TIMER
LSA_VOID  OHA_START_TIMER(
    LSA_UINT16        *ret_val_ptr,
    LSA_UINT16         timer_id,
    LSA_USER_ID_TYPE   user_id,
    LSA_UINT16         time
)
{
    LSAS_START_TIMER(ret_val_ptr, timer_id, user_id, time, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

#ifndef OHA_STOP_TIMER
LSA_VOID  OHA_STOP_TIMER(
    LSA_UINT16  *ret_val_ptr,
    LSA_UINT16   timer_id
)
{
    LSAS_STOP_TIMER(ret_val_ptr, timer_id, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

#ifndef OHA_FREE_TIMER
LSA_VOID  OHA_FREE_TIMER(
    LSA_UINT16  *ret_val_ptr,
    LSA_UINT16   timer_id
)
{
    LSAS_FREE_TIMER(ret_val_ptr, timer_id, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef OHA_DO_TIMER_REQUEST
LSA_VOID  OHA_DO_TIMER_REQUEST(
    OHA_UPPER_RQB_PTR_TYPE pRQB
)
{

    LSAS_RQB_OHA_REQUEST(pRQB, oha_request);
}
#else
# error "by design a function"
#endif

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef OHA_FATAL_ERROR
#error "should be a macro"
LSA_VOID  OHA_FATAL_ERROR(
    LSA_UINT16                length,
    OHA_FATAL_ERROR_PTR_TYPE error_ptr
)
{
    pcpnio_fatal_error ("oha", length, error_ptr);
}
#else

#endif

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef OHA_ENTER
LSA_VOID   OHA_ENTER(
    LSA_VOID
)
{
    LSAS_OHA_ENTER();
}
#else
# error "by design a function"
#endif

#ifndef OHA_EXIT
LSA_VOID  OHA_EXIT(
    LSA_VOID
)
{
    LSAS_OHA_EXIT();
}
#else
# error "by design a function"
#endif

#ifndef OHA_ALLOC_LOCAL_MEM
LSA_VOID   OHA_ALLOC_LOCAL_MEM(
    OHA_LOCAL_MEM_PTR_TYPE  *local_mem_ptr_ptr,
    LSA_UINT32               length
)
{
    LSAS_ALLOC_COMP_MEM(local_mem_ptr_ptr,length, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

#ifndef OHA_FREE_LOCAL_MEM
LSA_VOID   OHA_FREE_LOCAL_MEM(
    LSA_UINT16               *ret_val_ptr,
    OHA_LOCAL_MEM_PTR_TYPE    local_mem_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,local_mem_ptr, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif
                                                                                             /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef OHA_DCP_OPEN_CHANNEL_LOWER
LSA_VOID   OHA_DCP_OPEN_CHANNEL_LOWER(
    OHA_DCP_LOWER_RQB_PTR_TYPE lower_rqb_ptr,
    LSA_SYS_PTR_TYPE           sys_ptr
)
{
    LSA_UNUSED_ARG(sys_ptr);

    lower_rqb_ptr->args.channel.dcp_request_upper_done_ptr = lsas_dcp_to_oha_cbf;

    LSAS_RQB_DCP_REQUEST(lower_rqb_ptr, dcp_open_channel);
}
#else
# error "by design a function"
#endif

#ifndef OHA_DCP_CLOSE_CHANNEL_LOWER
LSA_VOID   OHA_DCP_CLOSE_CHANNEL_LOWER(
    OHA_DCP_LOWER_RQB_PTR_TYPE lower_rqb_ptr,
    LSA_SYS_PTR_TYPE           sys_ptr
)
{
    LSA_UNUSED_ARG(sys_ptr);

    LSAS_RQB_DCP_REQUEST(lower_rqb_ptr, dcp_close_channel);
}
#else
# error "by design a function"
#endif

#ifndef OHA_DCP_REQUEST_LOWER
LSA_VOID   OHA_DCP_REQUEST_LOWER(
    OHA_DCP_LOWER_RQB_PTR_TYPE lower_rqb_ptr,
    LSA_SYS_PTR_TYPE           sys_ptr
)
{
    LSA_UNUSED_ARG(sys_ptr);

    LSAS_RQB_DCP_REQUEST(lower_rqb_ptr, dcp_request);
}
#else
# error "by design a function"
#endif

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef OHA_EDD_OPEN_CHANNEL_LOWER
LSA_VOID   OHA_EDD_OPEN_CHANNEL_LOWER(
    OHA_EDD_LOWER_RQB_PTR_TYPE lower_rqb_ptr,
    LSA_SYS_PTR_TYPE           sys_ptr
)

{
    EDD_UPPER_OPEN_CHANNEL_PTR_TYPE edd_open = lower_rqb_ptr->pParam;

    edd_open->Cbf = lsas_edd_to_oha_cbf;

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
#else
# error "by design a function"
#endif

#ifndef OHA_EDD_CLOSE_CHANNEL_LOWER
LSA_VOID   OHA_EDD_CLOSE_CHANNEL_LOWER(
    OHA_EDD_LOWER_RQB_PTR_TYPE lower_rqb_ptr,
    LSA_SYS_PTR_TYPE           sys_ptr
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
#else
# error "by design a function"
#endif

#ifndef OHA_EDD_REQUEST_LOWER
LSA_VOID   OHA_EDD_REQUEST_LOWER(
    OHA_EDD_LOWER_RQB_PTR_TYPE lower_rqb_ptr,
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
#else
# error "by design a function"
#endif

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef OHA_CFG_NO_SOCK

#ifndef OHA_SOCK_OPEN_CHANNEL_LOWER
LSA_VOID   OHA_SOCK_OPEN_CHANNEL_LOWER(
    OHA_SOCK_LOWER_RQB_PTR_TYPE lower_rqb_ptr,
    LSA_SYS_PTR_TYPE            sys_ptr
)
{
    SOCK_RQB_ARGS_CHANNEL_TYPE * sock_open = &(lower_rqb_ptr->args.channel);

    LSA_UNUSED_ARG(sys_ptr);

    sock_open->sock_request_upper_done_ptr = lsas_sock_to_oha_cbf;

    LSAS_RQB_SOCK_REQUEST(lower_rqb_ptr, sock_open_channel);
}
#else
# error "by design a function"
#endif

#ifndef OHA_SOCK_CLOSE_CHANNEL_LOWER
LSA_VOID   OHA_SOCK_CLOSE_CHANNEL_LOWER(
    OHA_SOCK_LOWER_RQB_PTR_TYPE lower_rqb_ptr,
    LSA_SYS_PTR_TYPE            sys_ptr
)
{
    LSA_UNUSED_ARG(sys_ptr);

    LSAS_RQB_SOCK_REQUEST(lower_rqb_ptr, sock_close_channel);
}
#else
# error "by design a function"
#endif

#ifndef OHA_SOCK_REQUEST_LOWER
LSA_VOID   OHA_SOCK_REQUEST_LOWER(
    OHA_SOCK_LOWER_RQB_PTR_TYPE lower_rqb_ptr,
    LSA_SYS_PTR_TYPE            sys_ptr
)
{
    LSA_UNUSED_ARG(sys_ptr);

    LSAS_RQB_SOCK_REQUEST(lower_rqb_ptr, sock_request);
}
#else
# error "by design a function"
#endif

#endif /* SOCK */

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifdef OHA_CFG_USE_SNMPX

#ifndef OHA_SNMPX_OPEN_CHANNEL_LOWER
LSA_VOID  OHA_SNMPX_OPEN_CHANNEL_LOWER(
    OHA_SNMPX_LOWER_RQB_PTR_TYPE lower_rqb_ptr,
    LSA_SYS_PTR_TYPE            sys_ptr
)
{
    OHA_FUNCTION_TRACE_00(0,LSA_TRACE_LEVEL_CHAT, "OHA_SNMPX_OPEN_CHANNEL_LOWER()");

    LSA_UNUSED_ARG(sys_ptr);
    OHA_ASSERT(lower_rqb_ptr->args.Channel.snmpx_request_upper_done_ptr == oha_snmpx_request_lower_done);

    lower_rqb_ptr->args.Channel.snmpx_request_upper_done_ptr = (SNMPX_CBF_TYPE)pcpnio_callback;

    pcpnio_post_thread("snmpx", (void(*)(void*))snmpx_open_channel, (struct pcpnio_header*)lower_rqb_ptr, (void(*)(void*))oha_snmpx_request_lower_done, LSA_TRUE);
}
#else
# error "by design a function"
#endif

#ifndef OHA_SNMPX_CLOSE_CHANNEL_LOWER
LSA_VOID   OHA_SNMPX_CLOSE_CHANNEL_LOWER(
    OHA_SNMPX_LOWER_RQB_PTR_TYPE lower_rqb_ptr,
    LSA_SYS_PTR_TYPE             sys_ptr
)
{
    OHA_FUNCTION_TRACE_00(0,LSA_TRACE_LEVEL_CHAT, "OHA_SNMPX_CLOSE_CHANNEL_LOWER()");

    LSA_UNUSED_ARG(sys_ptr);
    pcpnio_post_thread("snmpx", (void(*)(void*))snmpx_close_channel, (struct pcpnio_header*)lower_rqb_ptr, (void(*)(void*))oha_snmpx_request_lower_done, LSA_TRUE);
}
#else
# error "by design a function"
#endif

#ifndef OHA_SNMPX_REQUEST_LOWER
LSA_VOID   OHA_SNMPX_REQUEST_LOWER(
    OHA_SNMPX_LOWER_RQB_PTR_TYPE lower_rqb_ptr,
    LSA_SYS_PTR_TYPE             sys_ptr
)
{
    OHA_FUNCTION_TRACE_00(0,LSA_TRACE_LEVEL_CHAT, "OHA_SNMPX_REQUEST_LOWER()");

    LSA_UNUSED_ARG(sys_ptr);

    OHA_FUNCTION_TRACE_03(0,LSA_TRACE_LEVEL_CHAT, "rqb(%08x) h(%u) opc(%u)", lower_rqb_ptr, OHA_SNMPX_RQB_GET_HANDLE(lower_rqb_ptr), OHA_SNMPX_RQB_GET_OPCODE(lower_rqb_ptr));

    pcpnio_post_thread("snmpx", (void(*)(void*))snmpx_request, (struct pcpnio_header*)lower_rqb_ptr, (void(*)(void*))oha_snmpx_request_lower_done, LSA_TRUE);
}
#else
# error "by design a function"
#endif

#endif /* SNMPX */

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef OHA_CFG_NO_NARE

#ifndef OHA_NARE_OPEN_CHANNEL_LOWER
LSA_VOID    OHA_NARE_OPEN_CHANNEL_LOWER(
    OHA_NARE_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE            sys_ptr)
{
    LSA_UNUSED_ARG(sys_ptr);

    lower_rqb_ptr->args.channel.nare_request_upper_done_ptr  = lsas_nare_to_oha_cbf;

    LSAS_RQB_NARE_REQUEST(lower_rqb_ptr, nare_open_channel);
}
#else
#error "by design a function"
#endif

#ifndef OHA_NARE_CLOSE_CHANNEL_LOWER
LSA_VOID    OHA_NARE_CLOSE_CHANNEL_LOWER(
    OHA_NARE_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE             sys_ptr)
{
    LSA_UNUSED_ARG(sys_ptr);

    LSAS_RQB_NARE_REQUEST(lower_rqb_ptr, nare_close_channel);
}
#else
#error "by design a function"
#endif

#ifndef OHA_NARE_REQUEST_LOWER
LSA_VOID    OHA_NARE_REQUEST_LOWER(
    OHA_NARE_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE             sys_ptr)
{
    LSA_UNUSED_ARG(sys_ptr);

    LSAS_RQB_NARE_REQUEST(lower_rqb_ptr, nare_request);
}
#else
#error "by design a function"
#endif

#endif  /* NARE */

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef OHA_CFG_NO_MRP

#ifndef OHA_MRP_OPEN_CHANNEL_LOWER
LSA_VOID    OHA_MRP_OPEN_CHANNEL_LOWER(
    OHA_MRP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE            sys_ptr)
{
    OHA_FUNCTION_TRACE_00(0,LSA_TRACE_LEVEL_CHAT, "OHA_MRP_OPEN_CHANNEL_LOWER()");


    LSA_UNUSED_ARG(sys_ptr);

    lower_rqb_ptr->args.channel.open.pCbf = lsas_mrp_to_oha_cbf;

    LSAS_RQB_MRP_REQUEST(lower_rqb_ptr, mrp_open_channel);
}
#else
#error "by design a function"
#endif

#ifndef OHA_MRP_CLOSE_CHANNEL_LOWER
LSA_VOID    OHA_MRP_CLOSE_CHANNEL_LOWER(
    OHA_MRP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE            sys_ptr)
{
    OHA_FUNCTION_TRACE_00(0,LSA_TRACE_LEVEL_CHAT, "OHA_MRP_CLOSE_CHANNEL_LOWER()");


    LSA_UNUSED_ARG(sys_ptr);

    LSAS_RQB_MRP_REQUEST(lower_rqb_ptr, mrp_close_channel);
}
#else
#error "by design a function"
#endif

#ifndef OHA_MRP_REQUEST_LOWER
LSA_VOID    OHA_MRP_REQUEST_LOWER(
    OHA_MRP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE            sys_ptr)
{
    OHA_FUNCTION_TRACE_00(0,LSA_TRACE_LEVEL_CHAT, "OHA_MRP_REQUEST_LOWER()");

    LSAS_RQB_MRP_REQUEST(lower_rqb_ptr, mrp_request);

    LSA_UNUSED_ARG(sys_ptr);


}
#else
#error "by design a function"
#endif
#else  /* NO_MRP */
/*
LSA_VOID    OHA_MRP_OPEN_CHANNEL_LOWER(
    OHA_MRP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE            sys_ptr)
{}

LSA_VOID    OHA_MRP_CLOSE_CHANNEL_LOWER(
    OHA_MRP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE            sys_ptr)
{}

LSA_VOID    OHA_MRP_REQUEST_LOWER(
    OHA_MRP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE            sys_ptr)
{}
*/
#endif

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef OHA_LLDP_OPEN_CHANNEL_LOWER
LSA_VOID    OHA_LLDP_OPEN_CHANNEL_LOWER(
    OHA_LLDP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE             sys_ptr)
{
    LSA_UNUSED_ARG(sys_ptr);

    lower_rqb_ptr->args.channel.lldp_request_upper_done_ptr = lsas_lldp_to_oha_cbf;

    LSAS_RQB_LLDP_REQUEST(lower_rqb_ptr, lldp_open_channel);
}
#else
#error "by design a function"
#endif

#ifndef OHA_LLDP_CLOSE_CHANNEL_LOWER
LSA_VOID    OHA_LLDP_CLOSE_CHANNEL_LOWER(
    OHA_LLDP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE             sys_ptr)
{
    LSA_UNUSED_ARG(sys_ptr);

    LSAS_RQB_LLDP_REQUEST(lower_rqb_ptr, lldp_close_channel);
}
#else
#error "by design a function"
#endif

#ifndef OHA_LLDP_REQUEST_LOWER
LSA_VOID    OHA_LLDP_REQUEST_LOWER(
    OHA_LLDP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE             sys_ptr)
{
    LSA_UNUSED_ARG(sys_ptr);

    LSAS_RQB_LLDP_REQUEST(lower_rqb_ptr, lldp_request);
}
#else
#error "by design a function"
#endif

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


#ifndef OHA_MEMSET
LSA_VOID   OHA_MEMSET(
    OHA_LOCAL_MEM_PTR_TYPE pMem,
    LSA_UINT8              Value,
    LSA_UINT32             Length
)
{
    LSAS_MEMSET(pMem,Value,Length);         // lint !e668   Possibly passing a null pointer to function memset
}
#else
# error "by design a function"
#endif

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef OHA_REQUEST_UPPER_DONE
LSA_VOID    OHA_REQUEST_UPPER_DONE(
    OHA_UPPER_CALLBACK_FCT_PTR_TYPE  oha_request_upper_done_ptr,
    OHA_UPPER_RQB_PTR_TYPE           upper_rqb_ptr,
    LSA_SYS_PTR_TYPE                 sys_ptr
)
{
    oha_request_upper_done_ptr(upper_rqb_ptr);
}
#else
# error "by design a function"
#endif



#ifndef OHA_ALLOC_UPPER_RQB_LOCAL
LSA_VOID   OHA_ALLOC_UPPER_RQB_LOCAL(
    OHA_UPPER_RQB_PTR_TYPE  *upper_rqb_ptr_ptr,
    LSA_UINT16               length
)
{
    LSAS_ALLOC_COMP_MEM(upper_rqb_ptr_ptr,length, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

#ifndef OHA_FREE_UPPER_RQB_LOCAL
LSA_VOID   OHA_FREE_UPPER_RQB_LOCAL(
    LSA_UINT16               *ret_val_ptr,
    OHA_UPPER_RQB_PTR_TYPE    upper_rqb_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,upper_rqb_ptr, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif
/*----------------------------------------------------------------------------*/

#ifndef OHA_ALLOC_UPPER_MEM
LSA_VOID    OHA_ALLOC_UPPER_MEM(
    OHA_UPPER_MEM_PTR_TYPE     * upper_mem_ptr_ptr,
    LSA_USER_ID_TYPE             user_id,
    LSA_UINT16                   length,
    LSA_SYS_PTR_TYPE             sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(upper_mem_ptr_ptr,length, LSA_COMP_ID_OHA);
}
#else
#error "by design a function"
#endif

#ifndef OHA_FREE_UPPER_MEM
LSA_VOID    OHA_FREE_UPPER_MEM(
    LSA_UINT16                * ret_val_ptr,
    OHA_UPPER_MEM_PTR_TYPE      upper_mem_ptr,
    LSA_SYS_PTR_TYPE            sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,upper_mem_ptr, LSA_COMP_ID_OHA);
}
#else
#error "by design a function"
#endif

/*+++++++++++++++++++ DCP ++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef OHA_DCP_ALLOC_LOWER_RQB
LSA_VOID   OHA_DCP_ALLOC_LOWER_RQB(
    OHA_DCP_LOWER_RQB_PTR_TYPE  *lower_rqb_ptr_ptr,
    LSA_USER_ID_TYPE             user_id,
    LSA_UINT16                   length,
    LSA_SYS_PTR_TYPE             sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(lower_rqb_ptr_ptr,length, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif


#ifndef OHA_DCP_FREE_LOWER_RQB
LSA_VOID   OHA_DCP_FREE_LOWER_RQB(
    LSA_UINT16                   *ret_val_ptr,
    OHA_DCP_LOWER_RQB_PTR_TYPE    lower_rqb_ptr,
    LSA_SYS_PTR_TYPE              sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,lower_rqb_ptr, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef OHA_DCP_ALLOC_LOWER_MEM
LSA_VOID   OHA_DCP_ALLOC_LOWER_MEM(
    OHA_DCP_LOWER_MEM_PTR_TYPE  *lower_mem_ptr_ptr,
    LSA_USER_ID_TYPE             user_id,
    LSA_UINT16                   length,
    LSA_SYS_PTR_TYPE             sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(lower_mem_ptr_ptr,length, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

#ifndef OHA_DCP_FREE_LOWER_MEM
LSA_VOID   OHA_DCP_FREE_LOWER_MEM(
    LSA_UINT16                   *ret_val_ptr,
    OHA_DCP_LOWER_MEM_PTR_TYPE    lower_mem_ptr,
    LSA_SYS_PTR_TYPE              sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,lower_mem_ptr, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

/*++++++++++++++++ LLDP ++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef OHA_LLDP_ALLOC_LOWER_RQB
LSA_VOID   OHA_LLDP_ALLOC_LOWER_RQB(
    OHA_LLDP_LOWER_RQB_PTR_TYPE  *lower_rqb_ptr_ptr,
    LSA_USER_ID_TYPE              user_id,
    LSA_UINT16                    length,
    LSA_SYS_PTR_TYPE              sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(lower_rqb_ptr_ptr,length, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif



#ifndef OHA_LLDP_FREE_LOWER_RQB
LSA_VOID   OHA_LLDP_FREE_LOWER_RQB(
    LSA_UINT16                   *ret_val_ptr,
    OHA_LLDP_LOWER_RQB_PTR_TYPE   lower_rqb_ptr,
    LSA_SYS_PTR_TYPE              sys_ptr
)

{
    LSAS_FREE_COMP_MEM(ret_val_ptr,lower_rqb_ptr, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef OHA_LLDP_ALLOC_LOWER_MEM
LSA_VOID   OHA_LLDP_ALLOC_LOWER_MEM(
    OHA_LLDP_LOWER_MEM_PTR_TYPE  *lower_mem_ptr_ptr,
    LSA_USER_ID_TYPE              user_id,
    LSA_UINT16                    length,
    LSA_SYS_PTR_TYPE              sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(lower_mem_ptr_ptr,length, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

#ifndef OHA_LLDP_FREE_LOWER_MEM
LSA_VOID   OHA_LLDP_FREE_LOWER_MEM(
    LSA_UINT16                   *ret_val_ptr,
    OHA_LLDP_LOWER_MEM_PTR_TYPE   lower_mem_ptr,
    LSA_SYS_PTR_TYPE              sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,lower_mem_ptr, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

/*++++++++++++++++ NARE ++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef OHA_CFG_NO_NARE

#ifndef OHA_NARE_ALLOC_LOWER_RQB
LSA_VOID    OHA_NARE_ALLOC_LOWER_RQB(
    OHA_NARE_LOWER_RQB_PTR_TYPE   *  lower_rqb_ptr_ptr,
    LSA_USER_ID_TYPE                 user_id,
    LSA_UINT16                       length,
    LSA_SYS_PTR_TYPE                 sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(lower_rqb_ptr_ptr,length, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

#ifndef OHA_NARE_FREE_LOWER_RQB
LSA_VOID    OHA_NARE_FREE_LOWER_RQB(
    LSA_UINT16                      *  ret_val_ptr,
    OHA_NARE_LOWER_RQB_PTR_TYPE        lower_rqb_ptr,
    LSA_SYS_PTR_TYPE                   sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,lower_rqb_ptr, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef OHA_NARE_ALLOC_LOWER_MEM
LSA_VOID    OHA_NARE_ALLOC_LOWER_MEM(
    OHA_NARE_LOWER_MEM_PTR_TYPE   *  lower_mem_ptr_ptr,
    LSA_USER_ID_TYPE                 user_id,
    LSA_UINT16                       length,
    LSA_SYS_PTR_TYPE                 sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(lower_mem_ptr_ptr,length, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

#ifndef OHA_NARE_FREE_LOWER_MEM
LSA_VOID    OHA_NARE_FREE_LOWER_MEM(
    LSA_UINT16                      *  ret_val_ptr,
    OHA_NARE_LOWER_MEM_PTR_TYPE        lower_mem_ptr,
    LSA_SYS_PTR_TYPE                   sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,lower_mem_ptr, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

#endif /* OHA_CFG_NO_NARE */


/*++++++++++++++++ MRP ++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef OHA_CFG_NO_MRP

#ifndef OHA_MRP_ALLOC_LOWER_RQB
LSA_VOID   OHA_MRP_ALLOC_LOWER_RQB(
    OHA_MRP_LOWER_RQB_PTR_TYPE   * lower_rqb_ptr_ptr,
    LSA_USER_ID_TYPE               user_id,
    LSA_UINT16                     length,
    LSA_SYS_PTR_TYPE               sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(lower_rqb_ptr_ptr,length, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

#ifndef OHA_MRP_FREE_LOWER_RQB
LSA_VOID   OHA_MRP_FREE_LOWER_RQB(
    LSA_UINT16                    * ret_val_ptr,
    OHA_MRP_LOWER_RQB_PTR_TYPE      lower_rqb_ptr,
    LSA_SYS_PTR_TYPE                sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,lower_rqb_ptr, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef OHA_MRP_ALLOC_LOWER_MEM
LSA_VOID   OHA_MRP_ALLOC_LOWER_MEM(
    OHA_MRP_LOWER_MEM_PTR_TYPE   * lower_mem_ptr_ptr,
    LSA_USER_ID_TYPE               user_id,
    LSA_UINT16                     length,
    LSA_SYS_PTR_TYPE               sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(lower_mem_ptr_ptr,length, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

#ifndef OHA_MRP_FREE_LOWER_MEM
LSA_VOID   OHA_MRP_FREE_LOWER_MEM(
    LSA_UINT16                    * ret_val_ptr,
    OHA_MRP_LOWER_MEM_PTR_TYPE      lower_mem_ptr,
    LSA_SYS_PTR_TYPE                sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,lower_mem_ptr, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

#else
/*
LSA_VOID   OHA_MRP_ALLOC_LOWER_RQB(
    OHA_MRP_LOWER_RQB_PTR_TYPE   * lower_rqb_ptr_ptr,
    LSA_USER_ID_TYPE               user_id,
    LSA_UINT16                     length,
    LSA_SYS_PTR_TYPE               sys_ptr
)

{}
LSA_VOID   OHA_MRP_FREE_LOWER_RQB(
    LSA_UINT16                    * ret_val_ptr,
    OHA_MRP_LOWER_RQB_PTR_TYPE      lower_rqb_ptr,
    LSA_SYS_PTR_TYPE                sys_ptr
)
{}
LSA_VOID   OHA_MRP_ALLOC_LOWER_MEM(
    OHA_MRP_LOWER_MEM_PTR_TYPE   * lower_mem_ptr_ptr,
    LSA_USER_ID_TYPE               user_id,
    LSA_UINT16                     length,
    LSA_SYS_PTR_TYPE               sys_ptr
)
{}
LSA_VOID   OHA_MRP_FREE_LOWER_MEM(
    LSA_UINT16                    * ret_val_ptr,
    OHA_MRP_LOWER_MEM_PTR_TYPE      lower_mem_ptr,
    LSA_SYS_PTR_TYPE                sys_ptr
)
{}
*/
#endif /* OHA_CFG_NO_MRP */

/*+++++++++++++++++++ EDD +++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef OHA_EDD_ALLOC_LOWER_RQB
LSA_VOID   OHA_EDD_ALLOC_LOWER_RQB(
    OHA_EDD_LOWER_RQB_PTR_TYPE  *lower_rqb_ptr_ptr,
    LSA_USER_ID_TYPE             user_id,
    LSA_UINT16                   length,
    LSA_SYS_PTR_TYPE             sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(lower_rqb_ptr_ptr,length, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

#ifndef OHA_EDD_FREE_LOWER_RQB
LSA_VOID   OHA_EDD_FREE_LOWER_RQB(
    LSA_UINT16                   *ret_val_ptr,
    OHA_EDD_LOWER_RQB_PTR_TYPE    lower_rqb_ptr,
    LSA_SYS_PTR_TYPE              sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,lower_rqb_ptr, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef OHA_EDD_ALLOC_LOWER_MEM
LSA_VOID   OHA_EDD_ALLOC_LOWER_MEM(
    OHA_EDD_LOWER_MEM_PTR_TYPE  *lower_mem_ptr_ptr,
    LSA_USER_ID_TYPE             user_id,
    LSA_UINT16                   length,
    LSA_SYS_PTR_TYPE             sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(lower_mem_ptr_ptr,length, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

#ifndef OHA_EDD_FREE_LOWER_MEM
LSA_VOID   OHA_EDD_FREE_LOWER_MEM(
    LSA_UINT16                   *ret_val_ptr,
    OHA_EDD_LOWER_MEM_PTR_TYPE    lower_mem_ptr,
    LSA_SYS_PTR_TYPE              sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,lower_mem_ptr, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef OHA_CFG_NO_SOCK

#ifndef OHA_SOCK_ALLOC_LOWER_RQB
LSA_VOID   OHA_SOCK_ALLOC_LOWER_RQB(
    OHA_SOCK_LOWER_RQB_PTR_TYPE  *lower_rqb_ptr_ptr,
    LSA_USER_ID_TYPE              user_id,
    LSA_UINT16                    length,
    LSA_SYS_PTR_TYPE              sys_ptr
)

{
    LSAS_ALLOC_COMP_MEM(lower_rqb_ptr_ptr,length, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

#ifndef OHA_SOCK_FREE_LOWER_RQB
LSA_VOID   OHA_SOCK_FREE_LOWER_RQB(
    LSA_UINT16                   *ret_val_ptr,
    OHA_SOCK_LOWER_RQB_PTR_TYPE   lower_rqb_ptr,
    LSA_SYS_PTR_TYPE              sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,lower_rqb_ptr, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef OHA_SOCK_ALLOC_LOWER_MEM
LSA_VOID   OHA_SOCK_ALLOC_LOWER_MEM(
    OHA_SOCK_LOWER_MEM_PTR_TYPE  *lower_mem_ptr_ptr,
    LSA_USER_ID_TYPE              user_id,
    LSA_UINT16                    length,
    LSA_SYS_PTR_TYPE              sys_ptr
)
{
    LSAS_ALLOC_COMP_MEM(lower_mem_ptr_ptr,length, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

#ifndef OHA_SOCK_FREE_LOWER_MEM
LSA_VOID   OHA_SOCK_FREE_LOWER_MEM(
    LSA_UINT16                   *ret_val_ptr,
    OHA_SOCK_LOWER_MEM_PTR_TYPE   lower_mem_ptr,
    LSA_SYS_PTR_TYPE              sys_ptr
)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr,lower_mem_ptr, LSA_COMP_ID_OHA);
}
#else
# error "by design a function"
#endif

#endif /* SOCK */

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifdef OHA_CFG_USE_SNMPX

#ifndef OHA_SNMPX_ALLOC_LOWER_RQB
LSA_VOID   OHA_SNMPX_ALLOC_LOWER_RQB(
    OHA_SNMPX_LOWER_RQB_PTR_TYPE  *lower_rqb_ptr_ptr,
    LSA_USER_ID_TYPE               user_id,
    LSA_UINT16                     length,
    LSA_SYS_PTR_TYPE               sys_ptr
)
{
    OHA_ASSERT(length == sizeof(OHA_SNMPX_LOWER_RQB_TYPE));
    *lower_rqb_ptr_ptr = pcpnio_malloc(&user_id, length, sys_ptr, __FILE__, __LINE__);
    OHA_FUNCTION_TRACE_01(0,LSA_TRACE_LEVEL_CHAT,"OHA_SNMPX_ALLOC_LOWER_RQB(lower_rqb_ptr(%08x))", *lower_rqb_ptr_ptr);
    oha_mem_counter++;
}
#else
# error "by design a function"
#endif

#ifndef OHA_SNMPX_FREE_LOWER_RQB
LSA_VOID   OHA_SNMPX_FREE_LOWER_RQB(
    LSA_UINT16                   *ret_val_ptr,
    OHA_SNMPX_LOWER_RQB_PTR_TYPE  lower_rqb_ptr,
    LSA_SYS_PTR_TYPE              sys_ptr
)
{
    pcpnio_free(lower_rqb_ptr, sys_ptr);
    OHA_FUNCTION_TRACE_01(0,LSA_TRACE_LEVEL_CHAT,"OHA_SNMPX_FREE_LOWER_RQB(lower_rqb_ptr(%08x))", lower_rqb_ptr);
    *ret_val_ptr = LSA_RET_OK;
    oha_mem_counter--;
}
#else
# error "by design a function"
#endif

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef OHA_SNMPX_ALLOC_LOWER_MEM
LSA_VOID   OHA_SNMPX_ALLOC_LOWER_MEM(
    OHA_SNMPX_LOWER_MEM_PTR_TYPE  *lower_mem_ptr_ptr,
    LSA_USER_ID_TYPE               user_id,
    LSA_UINT16                     length,
    LSA_SYS_PTR_TYPE               sys_ptr
)
{
    *lower_mem_ptr_ptr = pcpnio_malloc(&user_id, length, sys_ptr, __FILE__, __LINE__);
    OHA_FUNCTION_TRACE_01(0,LSA_TRACE_LEVEL_CHAT,"OHA_SNMPX_ALLOC_LOWER_MEM(lower_mem_ptr(%08x))", *lower_mem_ptr_ptr);
    oha_mem_counter++;
}
#else
# error "by design a function"
#endif

#ifndef OHA_SNMPX_FREE_LOWER_MEM
LSA_VOID   OHA_SNMPX_FREE_LOWER_MEM(
    LSA_UINT16                   *ret_val_ptr,
    OHA_SNMPX_LOWER_MEM_PTR_TYPE  lower_mem_ptr,
    LSA_SYS_PTR_TYPE              sys_ptr
)
{
    pcpnio_free(lower_mem_ptr, sys_ptr);
    OHA_FUNCTION_TRACE_01(0,LSA_TRACE_LEVEL_CHAT,"OHA_SNMPX_FREE_LOWER_MEM(lower_mem_ptr(%08x))", lower_mem_ptr);
    *ret_val_ptr = LSA_RET_OK;
    oha_mem_counter--;
}
#else
# error "by design a function"
#endif

#endif /* SNMPX */

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef OHA_MEMCPY
LSA_VOID  OHA_MEMCPY(
    OHA_COMMON_MEM_PTR_TYPE pDst,
    OHA_COMMON_MEM_PTR_TYPE pSrc,
    LSA_UINT32              Length
)
{
    LSAS_MEMCPY(pDst,pSrc,Length);          // lint !e668,670   Possible access beyond array for function
}
#else
# error "by design a function"
#endif

#ifndef OHA_MEMMOVE
LSA_VOID  OHA_MEMMOVE(
    OHA_COMMON_MEM_PTR_TYPE pDst,
    OHA_COMMON_MEM_PTR_TYPE pSrc,
    LSA_UINT32              Length
)
{
    LSAS_MEMCPY(pDst,pSrc,Length);
}
#else
# error "by design a function"
#endif

#ifndef OHA_MEMCMP
LSA_BOOL  OHA_MEMCMP(
    OHA_COMMON_MEM_PTR_TYPE pBuf1,
    OHA_COMMON_MEM_PTR_TYPE pBuf2,
    LSA_UINT32              Length
)
{
    LSA_BOOL    ret_val;

    if (LSAS_MEMCMP(pBuf1, pBuf2, Length))  // lint !e670   Possible access beyond array for function
    {
        ret_val = LSA_FALSE;
    }
    else
    {
        ret_val = LSA_TRUE;
    }

    return ret_val;
}
#else
# error "by design a function"
#endif



/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++++++  SNMP-Agent  +++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*=============================================================================
 * function name:  OHA_SWAP_U16, _U32
 *
 * function:       swap "in place"
 *                 between "little" and "big" endian or vice versa
 *
 * documentation:  LSA_CLRPC_LSA-Interface.doc
 *
 *===========================================================================*/

#ifndef OHA_SWAP_U16
LSA_VOID    OHA_SWAP_U16(
    LSA_UINT16  * short_ptr /* correct alignment! */
)
{
    LSA_UINT16 temp;
    temp = *short_ptr;
    *short_ptr = (LSA_UINT16)(temp >> 8) | (LSA_UINT16)(temp << 8);
}
#else
# error "by design a function"
#endif

#ifndef OHA_SWAP_U32
LSA_VOID    OHA_SWAP_U32(
    LSA_UINT32  * long_ptr /* correct alignment! */
)
{
    LSA_UINT32 temp;
    temp = *long_ptr;
    *long_ptr = (temp >> 24)
        | ((temp >> 8) & 0x0000ff00)
        | ((temp << 8) & 0x00ff0000)
        | (temp << 24);
}
#else
# error "by design a function"
#endif

/*=============================================================================
 * function name:    OHA_HTONS ... host to network byte order, short integer
 *                      _NTOHS ... network to host byte order, short integer
 *
 * function:       byte order conversion
 *
 * documentation:  LSA_OHA_RoughDesign.doc
 *
 *===========================================================================*/
#ifndef OHA_HTONS
LSA_UINT16   OHA_HTONS(
    LSA_UINT16 host_short
)
{
    #ifndef OHA_BIG_ENDIAN
        OHA_SWAP_U16(&host_short);
    #endif

    return host_short;
}
#else
# error "by design a function"
#endif

#ifndef OHA_NTOHS
LSA_UINT16   OHA_NTOHS(
    LSA_UINT16 network_short
)
{
    #ifndef OHA_BIG_ENDIAN
        OHA_SWAP_U16(&network_short);
    #endif
    return network_short;
}
#else
# error "by design a function"
#endif

/*=============================================================================
 * function name:    OHA_HTONL ... host to network byte order, long  integer
 *                      _NTOHL ... network to host byte order, long  integer
 *
 * function:       byte order conversion
 *
 * documentation:  LSA_OHA_RoughDesign.doc
 *
 *===========================================================================*/
#ifndef OHA_HTONL
LSA_UINT32   OHA_HTONL(
    LSA_UINT32 host_long
)
{
    #ifndef OHA_BIG_ENDIAN
        OHA_SWAP_U32(&host_long);
    #endif
    return host_long;
}
#else
# error "by design a function"
#endif

#ifndef OHA_NTOHL
LSA_UINT32   OHA_NTOHL(
    LSA_UINT32 network_long
)
{
    #ifndef OHA_BIG_ENDIAN
        OHA_SWAP_U32(&network_long);
    #endif
    return network_long;
}
#else
# error "by design a function"
#endif

/*------------------------------------------------------------------------------
//  read data from PDU / write data to PDU
//----------------------------------------------------------------------------*/

#ifndef OHA_PUT8_HTON
LSA_VOID    OHA_PUT8_HTON( /* put and convert from host byte order to network byte order */
    LSA_VOID_PTR_TYPE base,
    LSA_INT offset,
    LSA_UINT8 val
)
{
    OHA_COMMON_MEM_U8_PTR_TYPE ptr = (OHA_COMMON_MEM_U8_PTR_TYPE)base;
    ptr[offset] = val;
}
#else
# error "by design a function"
#endif

#ifndef OHA_GET8_NTOH
LSA_UINT8    OHA_GET8_NTOH( /* get and convert from network byte order to host byte order */
    LSA_VOID_PTR_TYPE base,
    LSA_INT offset
)
{
    OHA_COMMON_MEM_U8_PTR_TYPE ptr = (OHA_COMMON_MEM_U8_PTR_TYPE)base;
    return ptr[offset];
}
#else
# error "by design a function"
#endif

#ifndef OHA_PUT16_HTON
LSA_VOID    OHA_PUT16_HTON ( /* put and convert from host byte order to network byte order */
    LSA_VOID_PTR_TYPE base,
    LSA_INT offset,
    LSA_UINT16 val
)
{
    OHA_COMMON_MEM_U8_PTR_TYPE ptr = (OHA_COMMON_MEM_U8_PTR_TYPE)base;
    ptr += offset;
    /* val = htons (val); */
    val = OHA_HTONS (val);
    OHA_MEMCPY (ptr, & val, sizeof(val));
}
#else
# error "by design a function"
#endif

#ifndef OHA_GET16_NTOH
LSA_UINT16    OHA_GET16_NTOH( /* get and convert from network byte order to host byte order */
    LSA_VOID_PTR_TYPE base,
    LSA_INT offset
)
{
    LSA_UINT16  val=0;
    OHA_COMMON_MEM_U8_PTR_TYPE ptr = (OHA_COMMON_MEM_U8_PTR_TYPE)base;
    ptr += offset;
    OHA_MEMCPY (& val, ptr, sizeof(val));
    /* val = ntohs (val); */
    val = OHA_NTOHS (val);
    return val;
}
#else
# error "by design a function"
#endif

#ifndef OHA_PUT32_HTON
LSA_VOID    OHA_PUT32_HTON( /* put and convert from host byte order to network byte order */
    LSA_VOID_PTR_TYPE base,
    LSA_INT offset,
    LSA_UINT32 val
)
{
    OHA_COMMON_MEM_U8_PTR_TYPE ptr = (OHA_COMMON_MEM_U8_PTR_TYPE)base;
    ptr += offset;
    /* val = htonl (val); */
    val = OHA_HTONL (val);
    OHA_MEMCPY (ptr, & val, sizeof(val));
}
#else
# error "by design a function"
#endif

#ifndef OHA_GET32_NTOH
LSA_UINT32    OHA_GET32_NTOH( /* get and convert from network byte order to host byte order */
    LSA_VOID_PTR_TYPE base,
    LSA_INT offset
)
{
    LSA_UINT32  val=0;
    OHA_COMMON_MEM_U8_PTR_TYPE ptr = (OHA_COMMON_MEM_U8_PTR_TYPE)base;
    ptr += offset;
    OHA_MEMCPY (& val, ptr, sizeof(val));
    /* val = ntohl (val); */
    val = OHA_NTOHL (val);
    return val;
}
#else
# error "by design a function"
#endif


/*=============================================================================
 * function name:  OHA_GET_SYS_UPTIME
 *
 * function:       The OHA_GET_SYS_UPTIME function retrieves the number of 10 milliseconds
 *                 that have elapsed since the system was started.
 *
 * documentation:  LSA_OHA_RoughDesign.doc
 *
 *===========================================================================*/
#ifndef OHA_GET_SYS_UPTIME
LSA_UINT32   OHA_GET_SYS_UPTIME (LSA_VOID)
{
    LSA_UINT32  dwStart;

    LSAS_OS_GET_UPTIME_MS(&dwStart);

    return dwStart/10;
}
#else
# error "by design a function"
#endif

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++++++  DCP-Server  +++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*=============================================================================
 * function name:  OHA_REPORT_DCP_SET_IND
 * function:       Trace incoming SET Requests
 *                 Called before calling "out functions" to write datasets
 * parameters:     OHA_DCP_INDICATION_TYPE DcpInd: DCP-Indication part
 *                 (see documentation of OHA_OPC_DCP_INDICATION)
 *                 LSA_UINT8 *  ReqPtr:                 -> SET request data
 *                 LSA_INT      ReqLen:                 Length of request
 * return value:   LSA_UINT8   Reason:      The first Result-field of the
 *                                          SET-Response will be set to this value.
 *
 *  OHA_DCP_REASON_OK (no error)        Positive response, accept a SET and deliver parameters
 *  OHA_DCP_REASON_NO_DATABASE          Option not supported
 *  OHA_DCP_REASON_NO_DATASET           uboption not supported or no dataset available
 *  OHA_DCP_REASON_DS_NOT_SET (default) Suboption not set (local reasons)
 *  OHA_DCP_REASON_NO_RESOURCE          (temporary) resource error (server)
 *  OHA_DCP_REASON_NOT_POSSIBLE         SET not possible (local reasons)
 *  OHA_DCP_REASON_NOT_APPLICABLE       SET not possible (application operation)
 *
 *===========================================================================*/
#ifndef OHA_REPORT_DCP_SET_IND
LSA_UINT8  OHA_REPORT_DCP_SET_IND(
    LSA_UINT16                          NicId,
    OHA_UPPER_DCP_INDICATION_PTR_TYPE   pDcpInd,
    LSA_UINT8                     *     ReqPtr,
    LSA_INT                             ReqLen
)
{
    LSA_UNUSED_ARG(NicId);
    LSA_UNUSED_ARG(pDcpInd);
    LSA_UNUSED_ARG(ReqPtr);
    LSA_UNUSED_ARG(ReqLen);

#if 0
    /* set an emergency Ip-Suite without a query user */
    pDcpInd->Event = OHA_MODE_EMERGENCY_SET;

    /* check the ethernet for double IP-address without a query user */
    if (pDcpInd->Options & OHA_CTRL_IP_SUITE)
    {
        pDcpInd->CheckOptions |= OHA_CTRL_IP_SUITE;
    }
    /* check the ethernet for double NameOfStation without a query user */
    if (pDcpInd->Options & OHA_CTRL_STATION_NAME)
    {
        pDcpInd->CheckOptions |= OHA_CTRL_STATION_NAME;
    }
#endif

#if 0
    /* if no dataset "end of transaction" is sent by the dcp client, we can fake this */
    pDcpInd->AdditionalOptions      |= OHA_CTRL_RESPONSE;
    pDcpInd->AdditionalCheckOptions |= OHA_CTRL_RESPONSE;
#endif

    return OHA_DCP_REASON_OK;          /* allow a Dcp Set */
}
#endif


/*=============================================================================
 * function name:  OHA_REPORT_DCP_SET_RES
 * function:       Trace End of SET Request
 *                 Called after calling all "out functions" to write datasets
 * parameters:     OHA_DCP_INDICATION_TYPE DcpInd: DCP-Indication part
 *                 (see documentation of OHA_OPC_DCP_INDICATION)
 *                 LSA_UINT8 * RspPtr:      -> SET response data
 *                 LSA_INT     RspLen:      Length of response
 * return value:   -
 *===========================================================================*/
#ifndef OHA_REPORT_DCP_SET_RES
LSA_VOID  OHA_REPORT_DCP_SET_RES(
    LSA_UINT16                    NicId,
    OHA_DCP_INDICATION_TYPE       DcpInd,
    LSA_UINT8                   * RspPtr,
    LSA_INT                       RspLen
)
{
    OHA_FUNCTION_TRACE_03(0,LSA_TRACE_LEVEL_CHAT,
                       "IN : OHA_REPORT_DCP_SET_RES(DcpInd: 0x%X, RspPtr: 0x%X, RspLen: 0x%X)",
                        &DcpInd, RspPtr, RspLen);

    LSA_UNUSED_ARG(NicId);
    LSA_UNUSED_ARG(DcpInd);
    LSA_UNUSED_ARG(RspPtr);
    LSA_UNUSED_ARG(RspLen);

#if 0
    /* if no dataset "end of transaction" is sent by the dcp client, we can fake this */
    /* If the control option is a query-option and the user says "no change", there is no indication */
    /* "No" comes from the user response (OHA_OPC_DCP_RESPONSE) by OHA_MODE_NO_SET or OHA_MODE_SET_NEG */
    if (DcpInd.Event != OHA_MODE_NO_SET && DcpInd.Event != OHA_MODE_SET_NEG )
    {
        LSA_EXTERN LSA_VOID  OHA_DcpIndicateEventExtUsers(LSA_UINT16 NicId,
                                                                            OHA_UPPER_DCP_INDICATION_PTR_TYPE pEvent);
        LSA_EXTERN LSA_UINT8 oha_db_read_set_access(LSA_UINT16 NicId, LSA_UINT8 Type, LSA_UINT8 SubType);

        if (oha_db_read_set_access(DcpInd.NicId,
                                   DCP_DBID_CTRL,
                                   OHA_DCP_RESPONSE) != 0) /* is it locked ? */
        {
            OHA_DCP_INDICATION_TYPE  Response;  /* indication for response */

            Response.NicId    = DcpInd.NicId;
            Response.Event    = OHA_EVENT_CONTROL;
            Response.Options  = 0; /* not NULL ! */
            Response.AdditionalOptions = OHA_CTRL_RESPONSE;
            Response.Remanent = 0;
            Response.Count    = 1;
            Response.Type     = OHA_DCP_OPTION_CTRL;
            Response.SubType  = OHA_DCP_RESPONSE;
            OHA_MEMCPY(&Response.SenderMACAddr, &DcpInd.SenderMACAddr, OHA_MAC_ADDR_SIZE);
            OHA_DcpIndicateEventExtUsers(DcpInd.NicId, &Response);
        }
    }
#endif


    if ( DcpInd.Options & OHA_CTRL_RESET_TO_FACTORY )
    {
        LSAS_RTF_IND();
    }
    else if ( DcpInd.Options & OHA_CTRL_RESET_COMMUNICATION)
    {
        /* according to Profinet V2.3, we have to support DCP suboption "SuboptionResetToFactory" that
         * allows to reset communication parameters (-> in the additional "BlockQualifier":
         * "Reset Communication Parameter" is the only supported reset target that OHA supports).
         * Till now, we just supported the DCP suboption "SuboptionFactoryReset". See IEC 61158 */
        LSAS_RESET_COMM_IND();
    }
    #ifdef LSAS_CFG_REPORT_NEW_IP_DATA
    else if ( DcpInd.Options & OHA_CTRL_IP_SUITE)
    {
        /* new IP address was set via DCPSet. Get the new IP address from the stack and tell the user about it
         * calling LSAS_CFG_REPORT_NEW_IP_DATA() [needed by Devkits]*/
        if(!lsas_data.oha.oha_ip_rqb_in_use)
        {
            lsas_data.oha.oha_ip_rqb_in_use = TRUE;

            lsas_data.oha.oha_ip_rqb.args.PrmData.record_data         = lsas_data.oha.ip_data_buffer;
            lsas_data.oha.oha_ip_rqb.args.PrmData.record_data_length  = sizeof(lsas_data.oha.ip_data_buffer);
            lsas_data.oha.oha_ip_rqb.args.PrmData.record_index        = OHA_RECORD_INDEX_IP_SUITE;

            LSA_RQB_SET_HANDLE(&lsas_data.oha.oha_ip_rqb, lsas_data.oha.handle);
            LSA_RQB_SET_OPCODE(&lsas_data.oha.oha_ip_rqb, OHA_OPC_PRM_READ);

            LSAS_RQB_OHA_REQUEST(&lsas_data.oha.oha_ip_rqb, oha_request);
        }
    }
    #endif
    /* add reports here */
    /* add reports here */

    OHA_FUNCTION_TRACE_00(0,LSA_TRACE_LEVEL_CHAT, "OUT: OHA_REPORT_DCP_SET_RES o.k.");
    return;
}
#endif //OHA_REPORT_DCP_SET_RES

/*=============================================================================
 * function name:  OHA_GET_PATH_INFO
 *
 * function:       get system-pointer and detail-pointer of a communication
 *                 channel
 *
 * parameters:     LSA_UINT16              ...  *  ret_val_ptr:
 *                                      return value: LSA_RET_OK
 *                                                    LSA_RET_ERR_SYS_PATH
 *                 LSA_SYS_PTR_TYPE        ...  *  sys_ptr_ptr:
 *                                      return value: pointer to system-pointer
 *                 OHA_DETAIL_PTR_TYPE  ...  *  detail_ptr_ptr:
 *                                      return value: pointer to detail-pointer
 *                 LSA_SYS_PATH_TYPE               path: path of a
 *                                                       communication channel
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef OHA_GET_PATH_INFO
LSA_VOID    OHA_GET_PATH_INFO(
    LSA_UINT16               *  ret_val_ptr,
    LSA_SYS_PTR_TYPE         *  sys_ptr_ptr,
    OHA_DETAIL_PTR_TYPE      *  detail_ptr_ptr,
    LSA_SYS_PATH_TYPE           path
)
{
    LSA_GET_PATH_INFO(ret_val_ptr, sys_ptr_ptr, detail_ptr_ptr, path, LSAS_PATH_LAYER_OHA);
}
#endif

/*=============================================================================
 * function name:  OHA_RELEASE_PATH_INFO
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
 *                 OHA_DETAIL_PTR_TYPE  detail_ptr:  detail-pointer
 *
 * return value:   LSA_VOID
 *===========================================================================*/
#ifndef OHA_RELEASE_PATH_INFO
LSA_VOID    OHA_RELEASE_PATH_INFO(
    LSA_UINT16           *  ret_val_ptr,
    LSA_SYS_PTR_TYPE        sys_ptr,
    OHA_DETAIL_PTR_TYPE     detail_ptr
)
{
    LSA_RELEASE_PATH_INFO(ret_val_ptr, sys_ptr, detail_ptr);
}
#endif

/* cfg_db */





LSA_UINT16 oha_version ( LSA_UINT16 length, LSA_VERSION_TYPE *version )
{
  LSA_UINT8 *p_version, *p_v;
  LSA_UINT16 i;

  /* Initialize version struct */
  LSA_VERSION_TYPE v = {
                        PNIO_LSA_COMPONENT_ID, {'L','S','A','_','O','H','A','\0','\0','\0'},
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
