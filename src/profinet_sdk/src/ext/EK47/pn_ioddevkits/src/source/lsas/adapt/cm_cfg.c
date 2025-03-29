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
/*  F i l e               &F: cm_cfg.c                                  :F&  */
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

#define LSAS_MODULE_ID  14

#define CM_MODULE_ID 0xFF01 /* see CM_ASSERT */

//#include "cm_inc.h"
#include "lsas_inc.h"
#include "lsas_int.h"

//CM_FILE_SYSTEM_EXTENSION(CM_MODULE_ID) /* no semicolon */

/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/

int cm_log_level = 1;

/*----------------------------------------------------------------------------*/

void cm_pcpnio_startstop (int start)
{
    if (start) {
        static CM_INIT_TYPE cm_init_param;
        LSA_UINT16  rc;

        cm_init_param.oneshot_lsa_time_base   = LSA_TIME_BASE_100MS;
        cm_init_param.oneshot_lsa_time_factor = 1;

        rc = cm_init(&cm_init_param);
        CM_ASSERT(rc == LSA_RET_OK);
        LSA_UNUSED_ARG(rc);
    }
    else {
        LSA_UINT16  rc;

        rc = cm_undo_init();
        CM_ASSERT(rc == LSA_RET_OK);
        LSA_UNUSED_ARG(rc);
    }
}

/*------------------------------------------------------------------------------
// debug helper
//----------------------------------------------------------------------------*/
#if CM_DEBUG

char *cm_uuid2a (CLRPC_UUID_PTR_TYPE uuid) {
{
    static LSA_UINT8 s_uuid[CLRPC_MAX_UUID_STRING];
    clrpc_uuid_to_string (uuid, s_uuid);
    return (char*)s_uuid;
}

char *cm_mac2a (CM_MAC_PTR_TYPE mac) {
    static LSA_UINT8 s_mac[18+1];
    CM_SPRINTF (s_mac, "%02x-%02x-%02x-%02x-%02x-%02x",
            mac->mac[0], mac->mac[1], mac->mac[2],
            mac->mac[3], mac->mac[4], mac->mac[5]
        );
    return (char*)s_mac;
}

char *cm_ip2a (LSA_UINT32 ip) {
    static LSA_UINT8 s_ip[16+1];
    CM_SPRINTF (s_ip, "%d.%d.%d.%d",
//          (ip >> 24) & 0xFF,  (ip >> 16) & 0xFF, (ip >> 8) & 0xFF,  (ip >> 0) & 0xFF
            (ip >> 0) & 0xFF,  (ip >> 8) & 0xFF, (ip >> 16) & 0xFF,  (ip >> 24) & 0xFF
        );
    return (char*)s_ip;
}

#endif

/*------------------------------------------------------------------------------
//    System Layer (prototypes in cm_sys.h)
//----------------------------------------------------------------------------*/

void  CM_GET_PATH_INFO(LSA_UINT16 * ret_val_ptr, LSA_SYS_PTR_TYPE *  sys_ptr_ptr,
                       CM_DETAIL_PTR_TYPE  * detail_ptr_ptr, LSA_SYS_PATH_TYPE path)
{
    LSA_GET_PATH_INFO(ret_val_ptr, sys_ptr_ptr, detail_ptr_ptr,path, LSAS_PATH_LAYER_CM);
}


/*----------------------------------------------------------------------------*/

void    CM_RELEASE_PATH_INFO    (LSA_UINT16 *  ret_val_ptr, LSA_SYS_PTR_TYPE sys_ptr, CM_DETAIL_PTR_TYPE detail_ptr)
{
    LSA_RELEASE_PATH_INFO(ret_val_ptr, sys_ptr, detail_ptr);
}

/*----------------------------------------------------------------------------*/

void    CM_ALLOC_LOCAL_MEM(CM_LOCAL_MEM_PTR_TYPE *  local_mem_ptr_ptr, LSA_UINT32 length)
{
    LSAS_ALLOC_COMP_MEM(local_mem_ptr_ptr, length, LSA_COMP_ID_CM);
}

/*----------------------------------------------------------------------------*/

void    CM_FREE_LOCAL_MEM(LSA_UINT16 * ret_val_ptr, CM_LOCAL_MEM_PTR_TYPE local_mem_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, local_mem_ptr, LSA_COMP_ID_CM);
}

/*----------------------------------------------------------------------------*/

void    CM_ALLOC_TIMER(LSA_UINT16 *  ret_val_ptr, LSA_UINT16 *  timer_id_ptr, LSA_UINT16 timer_type, LSA_UINT16 time_base)
{
    LSAS_ALLOC_TIMER(ret_val_ptr, timer_id_ptr, timer_type, time_base, cm_timeout, LSA_COMP_ID_CM);
}

/*----------------------------------------------------------------------------*/

void    CM_START_TIMER(LSA_UINT16 *  ret_val_ptr, LSA_UINT16 timer_id, LSA_USER_ID_TYPE user_id, LSA_UINT16 time)
{
    LSAS_START_TIMER(ret_val_ptr, timer_id, user_id, time, LSA_COMP_ID_CM);
}

/*----------------------------------------------------------------------------*/

void    CM_STOP_TIMER(LSA_UINT16 *  ret_val_ptr, LSA_UINT16 timer_id)
{
    LSAS_STOP_TIMER(ret_val_ptr, timer_id, LSA_COMP_ID_CM);
}

/*----------------------------------------------------------------------------*/

void    CM_FREE_TIMER(LSA_UINT16 *  ret_val_ptr, LSA_UINT16 timer_id)
{
    LSAS_FREE_TIMER(ret_val_ptr, timer_id, LSA_COMP_ID_CM);
}



/*----------------------------------------------------------------------------*/

void    CM_ENTER(void)
{
    LSAS_OS_LOCK_ON_PNIO_TASK();
}


/*----------------------------------------------------------------------------*/

void    CM_EXIT(void)
{
    LSAS_OS_RELEASE_PNIO_TASK();
}

/*------------------------------------------------------------------------------
//    Upper Layer (prototypes in cm_usr.h)
//----------------------------------------------------------------------------*/
LSA_VOID CM_REQUEST_UPPER_DONE(
    CM_UPPER_CALLBACK_FCT_PTR_TYPE  cm_request_upper_done_ptr,
    CM_UPPER_RQB_PTR_TYPE           upper_rqb_ptr,
    LSA_SYS_PTR_TYPE                sys_ptr
)
{
    cm_request_upper_done_ptr(upper_rqb_ptr);
}

void    CM_ALLOC_UPPER_RQB(CM_UPPER_RQB_PTR_TYPE  *  upper_rqb_ptr_ptr, LSA_USER_ID_TYPE user_id, LSA_UINT16 length,
                           LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_ALLOC_COMP_MEM(upper_rqb_ptr_ptr, length, LSA_COMP_ID_CM);

    LSA_UNUSED_ARG(user_id);
    LSA_UNUSED_ARG(sys_ptr);
}


/*----------------------------------------------------------------------------*/

void    CM_FREE_UPPER_RQB(LSA_UINT16 *  ret_val_ptr, CM_UPPER_RQB_PTR_TYPE upper_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, upper_rqb_ptr, LSA_COMP_ID_CM);

    LSA_UNUSED_ARG(sys_ptr);
}

/*----------------------------------------------------------------------------*/

void    CM_ALLOC_UPPER_MEM(CM_UPPER_MEM_PTR_TYPE *  upper_mem_ptr_ptr, LSA_USER_ID_TYPE user_id, LSA_UINT32 length,
                           LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_ALLOC_COMP_MEM(upper_mem_ptr_ptr, length, LSA_COMP_ID_CM);

    LSA_UNUSED_ARG(user_id);
    LSA_UNUSED_ARG(sys_ptr);
}

/*----------------------------------------------------------------------------*/

void    CM_FREE_UPPER_MEM(LSA_UINT16 *  ret_val_ptr, CM_UPPER_MEM_PTR_TYPE upper_mem_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, upper_mem_ptr, LSA_COMP_ID_CM);

    LSA_UNUSED_ARG(sys_ptr);
}

/*----------------------------------------------------------------------------*/

void    CM_REQUEST_LOCAL(CM_UPPER_RQB_PTR_TYPE upper_rqb_ptr)
{

    LSAS_RQB_CM_REQUEST(upper_rqb_ptr, cm_request);
}

/*----------------------------------------------------------------------------*/

LSA_VOID   CM_GET_CYCLE_COUNTER(
    LSA_UINT16    *  ret_val_ptr,
    LSA_SYS_PTR_TYPE sys_ptr,
    LSA_UINT64    *  cycle_counter_ptr
)
{
#ifdef LSAS_CFG_USE_EDDI
    EDDI_CYCLE_COUNTER_TYPE   tmp;



    if( eddi_SRTProviderGetCycleCounter(lsas_com_get_hddb(), &tmp) != EDD_STS_OK )
    {
        CM_FATAL();
    }

    *cycle_counter_ptr = ((LSA_UINT64)tmp.High << 32) | tmp.Low;


    LSA_UNUSED_ARG(sys_ptr);

    *ret_val_ptr = (LSA_UINT16)LSA_RET_OK;
#endif
#ifdef LSAS_CFG_USE_EDDP
    EDDP_CYCLE_COUNTER_TYPE   tmp;

    if( eddp_SRTProviderGetCycleCounter(lsas_com_get_hddb(), &tmp) != EDD_STS_OK )
    {
        CM_FATAL();
    }

    *cycle_counter_ptr = ((LSA_UINT64)tmp.High << 32) | tmp.Low;


    LSA_UNUSED_ARG(sys_ptr);

    *ret_val_ptr = (LSA_UINT16)LSA_RET_OK;
#endif
#ifdef LSAS_CFG_USE_EDDS
    EDDS_CYCLE_COUNTER_TYPE   tmp;

    if( edds_SRTProviderGetCycleCounter(lsas_com_get_hddb(), &tmp) != EDD_STS_OK )
    {
        CM_FATAL();
    }

    *cycle_counter_ptr = ((LSA_UINT64)tmp.High << 32) | tmp.Low;


    LSA_UNUSED_ARG(sys_ptr);

    *ret_val_ptr = (LSA_UINT16)LSA_RET_OK;
#endif
}

LSA_UINT16 CM_GET_PRIMARY_AR(
    LSA_UINT16        srshm_index,
    LSA_SYS_PTR_TYPE  sys_ptr
)
{
    LSA_UINT16  srshm_value;

    LSA_UNUSED_ARG(sys_ptr);

    srshm_value = 0;

    LSAS_CFG_GET_PRIMARY_AR_SESSION_KEY(srshm_index, &srshm_value);

    return srshm_value;
}


/******************************************************************************/
/*   Lower Layer Interface                                                    */
/******************************************************************************/

/******************************************************************************/
/*   open channel calls                                                       */
/******************************************************************************/
typedef void  LSA_FCT_PTR(/* attr. */, acp_request_upper_done_ptr_type)(struct acp_rqb_tag *);
typedef void  LSA_FCT_PTR(/* attr. */, clrpc_request_upper_done_ptr_type)(CLRPC_UPPER_RQB_PTR_TYPE);
typedef void  LSA_FCT_PTR(/* attr. */, EDD_CALLBACK_FCT_PTR_TYPE)(EDD_UPPER_RQB_PTR_TYPE upper_rqb_ptr);
typedef void  LSA_FCT_PTR(/* attr. */, GSY_CALLBACK_FCT_PTR_TYPE)(GSY_UPPER_RQB_PTR_TYPE upper_rqb_ptr);
#if CM_CFG_USE_POF
typedef void  LSA_FCT_PTR(/* attr. */, POF_CALLBACK_FCT_PTR_TYPE)(POF_UPPER_RQB_PTR_TYPE upper_rqb_ptr);
#endif

void    CM_ACP_OPEN_CHANNEL_LOWER(CM_ACP_LOWER_RQB_PTR_TYPE lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    lower_rqb_ptr->args->channel.open.acp_request_upper_done_ptr
        = (acp_request_upper_done_ptr_type)lsas_acp_to_cm_cbf;

    LSAS_RQB_ACP_REQUEST(lower_rqb_ptr, acp_open_channel);

    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_EDD_OPEN_CHANNEL_LOWER(CM_EDD_LOWER_RQB_PTR_TYPE lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    ((EDD_UPPER_OPEN_CHANNEL_PTR_TYPE)EDD_RQB_GET_PPARAM(lower_rqb_ptr))->Cbf
         = (EDD_CALLBACK_FCT_PTR_TYPE)lsas_edd_to_cm_cbf;
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

void    CM_GSY_OPEN_CHANNEL_LOWER(CM_GSY_LOWER_RQB_PTR_TYPE  lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    lower_rqb_ptr->args.channel.gsy_request_upper_done_ptr
        = (GSY_CALLBACK_FCT_PTR_TYPE)lsas_gsy_to_cm_cbf;

    LSAS_RQB_GSY_REQUEST(lower_rqb_ptr, gsy_open_channel);

    LSA_UNUSED_ARG(sys_ptr);
}

void    CM_NARE_OPEN_CHANNEL_LOWER(CM_NARE_LOWER_RQB_PTR_TYPE lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    lower_rqb_ptr->args.channel.nare_request_upper_done_ptr
        = lsas_nare_to_cm_cbf;

    LSAS_RQB_NARE_REQUEST(lower_rqb_ptr, nare_open_channel);

    LSA_UNUSED_ARG(sys_ptr);
}

void    CM_OHA_OPEN_CHANNEL_LOWER(CM_OHA_LOWER_RQB_PTR_TYPE  lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    lower_rqb_ptr->args.channel.oha_request_upper_done_ptr
        = (OHA_UPPER_CALLBACK_FCT_PTR_TYPE)lsas_oha_to_cm_cbf;

    LSAS_RQB_OHA_REQUEST(lower_rqb_ptr, oha_open_channel);

    LSA_UNUSED_ARG(sys_ptr);
}

void    CM_RPC_OPEN_CHANNEL_LOWER(CM_RPC_LOWER_RQB_PTR_TYPE  lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    lower_rqb_ptr->args.channel.open->clrpc_request_upper_done_ptr
       = (clrpc_request_upper_done_ptr_type)lsas_clrpc_to_cm_cbf;

    LSAS_RQB_CLRPC_REQUEST(lower_rqb_ptr, clrpc_open_channel);

    LSA_UNUSED_ARG(sys_ptr);
}


#if CM_CFG_USE_MRP
void    CM_MRP_OPEN_CHANNEL_LOWER(CM_MRP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    lower_rqb_ptr->args.channel.open.pCbf
        = (MRP_UPPER_CALLBACK_FCT_PTR_TYPE)lsas_mrp_to_cm_cbf;

    LSAS_RQB_MRP_REQUEST(lower_rqb_ptr, mrp_open_channel);

    LSA_UNUSED_ARG(sys_ptr);
}
#endif

#if CM_CFG_USE_POF
void    CM_POF_OPEN_CHANNEL_LOWER(CM_POF_LOWER_RQB_PTR_TYPE  lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    lower_rqb_ptr->args.channel.Cbf
        = (POF_CALLBACK_FCT_PTR_TYPE)lsas_pof_to_cm_cbf;


    LSAS_RQB_POF_REQUEST(lower_rqb_ptr, pof_open_channel);

    LSA_UNUSED_ARG(sys_ptr);
}
#endif

/******************************************************************************/
/*   close channel calls                                                       */
/******************************************************************************/

void    CM_ACP_CLOSE_CHANNEL_LOWER(CM_ACP_LOWER_RQB_PTR_TYPE lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{

    LSAS_RQB_ACP_REQUEST(lower_rqb_ptr, acp_close_channel);

    LSA_UNUSED_ARG(sys_ptr);
}

void    CM_EDD_CLOSE_CHANNEL_LOWER (CM_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
#ifdef LSAS_CFG_USE_EDDI
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr, eddi_close_channel);
#endif
#ifdef LSAS_CFG_USE_EDDP
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr, eddp_close_channel);
#endif

#ifdef LSAS_CFG_USE_EDDS
    LSAS_RQB_EDD_REQUEST(lower_rqb_ptr, edds_close_channel);
#endif

    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_GSY_CLOSE_CHANNEL_LOWER (CM_GSY_LOWER_RQB_PTR_TYPE  lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{

    LSAS_RQB_GSY_REQUEST(lower_rqb_ptr, gsy_close_channel);

    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_NARE_CLOSE_CHANNEL_LOWER (CM_NARE_LOWER_RQB_PTR_TYPE lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_RQB_NARE_REQUEST(lower_rqb_ptr, nare_close_channel);

    LSA_UNUSED_ARG(sys_ptr);
}

void    CM_OHA_CLOSE_CHANNEL_LOWER (CM_OHA_LOWER_RQB_PTR_TYPE  lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{

    LSAS_RQB_OHA_REQUEST(lower_rqb_ptr, oha_close_channel);

    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_RPC_CLOSE_CHANNEL_LOWER(CM_RPC_LOWER_RQB_PTR_TYPE  lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{

    LSAS_RQB_CLRPC_REQUEST(lower_rqb_ptr, clrpc_close_channel);

    LSA_UNUSED_ARG(sys_ptr);
}


#if CM_CFG_USE_MRP
void    CM_MRP_CLOSE_CHANNEL_LOWER (CM_MRP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{

    LSAS_RQB_MRP_REQUEST(lower_rqb_ptr, mrp_close_channel);

    LSA_UNUSED_ARG(sys_ptr);
}
#endif

#if CM_CFG_USE_POF
void    CM_POF_CLOSE_CHANNEL_LOWER (CM_POF_LOWER_RQB_PTR_TYPE  lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{

    LSAS_RQB_POF_REQUEST(lower_rqb_ptr, pof_close_channel);

    LSA_UNUSED_ARG(sys_ptr);
}
#endif


/*=============================================================================
 * function name:  CM_REQUEST_LOWER
 *
 * function:       send a request to a communication channel to lower layer
 *
 *
 * documentation:  LSA_CM_Detailspec.doc
 *
 *===========================================================================*/

void    CM_ACP_REQUEST_LOWER (CM_ACP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr, LSA_SYS_PTR_TYPE           sys_ptr)
{

    LSAS_RQB_ACP_REQUEST(lower_rqb_ptr, acp_request);

    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_EDD_REQUEST_LOWER (CM_EDD_LOWER_RQB_PTR_TYPE  lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
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

    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_GSY_REQUEST_LOWER (CM_GSY_LOWER_RQB_PTR_TYPE  lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{

    LSAS_RQB_GSY_REQUEST(lower_rqb_ptr, gsy_request);

    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_NARE_REQUEST_LOWER (CM_NARE_LOWER_RQB_PTR_TYPE  lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_RQB_GSY_REQUEST(lower_rqb_ptr, nare_request);

    LSA_UNUSED_ARG(sys_ptr);
}

void    CM_OHA_REQUEST_LOWER (CM_OHA_LOWER_RQB_PTR_TYPE  lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{

    LSAS_RQB_OHA_REQUEST(lower_rqb_ptr, oha_request);

    LSA_UNUSED_ARG(sys_ptr);
}

void    CM_RPC_REQUEST_LOWER(CM_RPC_LOWER_RQB_PTR_TYPE  lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{

    LSAS_RQB_CLRPC_REQUEST(lower_rqb_ptr, clrpc_request);

    LSA_UNUSED_ARG(sys_ptr);
}


#if CM_CFG_USE_MRP
void    CM_MRP_REQUEST_LOWER (CM_MRP_LOWER_RQB_PTR_TYPE  lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{

    LSAS_RQB_MRP_REQUEST(lower_rqb_ptr, mrp_request);

    LSA_UNUSED_ARG(sys_ptr);
}
#endif

#if CM_CFG_USE_POF
void    CM_POF_REQUEST_LOWER (CM_POF_LOWER_RQB_PTR_TYPE  lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{

    LSAS_RQB_POF_REQUEST(lower_rqb_ptr, pof_request);

    LSA_UNUSED_ARG(sys_ptr);
}
#endif


/*=============================================================================
 * function name:  CM_ALLOC_LOWER_RQB
 *
 * function:       allocate a lower-RQB
 *
 * documentation:  LSA_CM_Detailspec.doc
 *
 *===========================================================================*/

void    CM_ACP_ALLOC_LOWER_RQB(CM_ACP_LOWER_RQB_PTR_TYPE *  lower_rqb_ptr_ptr, LSA_USER_ID_TYPE user_id,
                               LSA_UINT16 length, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_ALLOC_COMP_MEM(lower_rqb_ptr_ptr, length, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(user_id);
    LSA_UNUSED_ARG(sys_ptr);
}



void    CM_EDD_ALLOC_LOWER_RQB(CM_EDD_LOWER_RQB_PTR_TYPE *  lower_rqb_ptr_ptr, LSA_USER_ID_TYPE user_id,
                               LSA_UINT16 length, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_ALLOC_COMP_MEM(lower_rqb_ptr_ptr, length, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(user_id);
    LSA_UNUSED_ARG(sys_ptr);
}

void    CM_GSY_ALLOC_LOWER_RQB(CM_GSY_LOWER_RQB_PTR_TYPE  *  lower_rqb_ptr_ptr,  LSA_USER_ID_TYPE user_id,
                               LSA_UINT16 length, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_ALLOC_COMP_MEM(lower_rqb_ptr_ptr, length, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(user_id);
    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_NARE_ALLOC_LOWER_RQB(CM_NARE_LOWER_RQB_PTR_TYPE  *  lower_rqb_ptr_ptr, LSA_USER_ID_TYPE user_id,
                                LSA_UINT16 length, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_ALLOC_COMP_MEM(lower_rqb_ptr_ptr, length, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(user_id);
    LSA_UNUSED_ARG(sys_ptr);
}

void    CM_OHA_ALLOC_LOWER_RQB(CM_OHA_LOWER_RQB_PTR_TYPE  *  lower_rqb_ptr_ptr, LSA_USER_ID_TYPE user_id,
                               LSA_UINT16 length, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_ALLOC_COMP_MEM(lower_rqb_ptr_ptr, length, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(user_id);
    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_RPC_ALLOC_LOWER_RQB(CM_RPC_LOWER_RQB_PTR_TYPE  *  lower_rqb_ptr_ptr, LSA_USER_ID_TYPE user_id,
                               LSA_UINT16 length, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_ALLOC_COMP_MEM(lower_rqb_ptr_ptr, length, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(user_id);
    LSA_UNUSED_ARG(sys_ptr);
}


#if CM_CFG_USE_MRP
void    CM_MRP_ALLOC_LOWER_RQB(CM_MRP_LOWER_RQB_PTR_TYPE  *  lower_rqb_ptr_ptr, LSA_USER_ID_TYPE user_id,
                               LSA_UINT16 length, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_ALLOC_COMP_MEM(lower_rqb_ptr_ptr, length, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(user_id);
    LSA_UNUSED_ARG(sys_ptr);
}
#endif

#if CM_CFG_USE_POF
void    CM_POF_ALLOC_LOWER_RQB(CM_POF_LOWER_RQB_PTR_TYPE  *  lower_rqb_ptr_ptr, LSA_USER_ID_TYPE user_id,
                               LSA_UINT16 length, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_ALLOC_COMP_MEM(lower_rqb_ptr_ptr, length, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(user_id);
    LSA_UNUSED_ARG(sys_ptr);
}
#endif



/*=============================================================================
 * function name:  CM_FREE_LOWER_RQB
 *
 * function:       free a lower-RQB
 *
 * documentation:  LSA_CM_Detailspec.doc
 *
 *===========================================================================*/

void    CM_ACP_FREE_LOWER_RQB(LSA_UINT16 *  ret_val_ptr, CM_ACP_LOWER_RQB_PTR_TYPE lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_rqb_ptr, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_EDD_FREE_LOWER_RQB(LSA_UINT16 *  ret_val_ptr, CM_EDD_LOWER_RQB_PTR_TYPE lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_rqb_ptr, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_GSY_FREE_LOWER_RQB(LSA_UINT16 *  ret_val_ptr, CM_GSY_LOWER_RQB_PTR_TYPE lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_rqb_ptr, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_NARE_FREE_LOWER_RQB(LSA_UINT16 *  ret_val_ptr, CM_NARE_LOWER_RQB_PTR_TYPE lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_rqb_ptr, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_OHA_FREE_LOWER_RQB(LSA_UINT16 *  ret_val_ptr, CM_OHA_LOWER_RQB_PTR_TYPE lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_rqb_ptr, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_RPC_FREE_LOWER_RQB(LSA_UINT16 *  ret_val_ptr, CM_RPC_LOWER_RQB_PTR_TYPE lower_rqb_ptr,LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_rqb_ptr, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(sys_ptr);
}


#if CM_CFG_USE_MRP
void    CM_MRP_FREE_LOWER_RQB(LSA_UINT16 *  ret_val_ptr, CM_MRP_LOWER_RQB_PTR_TYPE lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_rqb_ptr, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(sys_ptr);
}
#endif

#if CM_CFG_USE_POF
void    CM_POF_FREE_LOWER_RQB(LSA_UINT16 *  ret_val_ptr,CM_POF_LOWER_RQB_PTR_TYPE lower_rqb_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_rqb_ptr, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(sys_ptr);
}
#endif


/*=============================================================================
 * function name:  CM_ALLOC_LOWER_MEM
 *
 * function:       allocate a lower-memory
 *
 * documentation:  LSA_CM_Detailspec.doc
 *
 *===========================================================================*/

void    CM_ACP_ALLOC_LOWER_MEM(CM_ACP_LOWER_MEM_PTR_TYPE  *  lower_mem_ptr_ptr, LSA_USER_ID_TYPE user_id,
                               LSA_UINT16 length, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_ALLOC_COMP_MEM(lower_mem_ptr_ptr, length, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(user_id);
    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_ACP_ALLOC_ALARM_MEM(CM_ACP_LOWER_MEM_PTR_TYPE *  lower_alarm_ptr_ptr, LSA_UINT16 length, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_ALLOC_COMP_MEM(lower_alarm_ptr_ptr, length, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_EDD_ALLOC_LOWER_MEM(CM_EDD_LOWER_MEM_PTR_TYPE *  lower_mem_ptr_ptr, LSA_USER_ID_TYPE user_id,
                               LSA_UINT16 length, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_ALLOC_COMP_MEM(lower_mem_ptr_ptr, length, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(user_id);
    LSA_UNUSED_ARG(sys_ptr);
}

void    CM_GSY_ALLOC_LOWER_MEM(CM_GSY_LOWER_MEM_PTR_TYPE *  lower_mem_ptr_ptr, LSA_USER_ID_TYPE user_id,
                               LSA_UINT16 length, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_ALLOC_COMP_MEM(lower_mem_ptr_ptr, length, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(user_id);
    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_NARE_ALLOC_LOWER_MEM(CM_NARE_LOWER_MEM_PTR_TYPE *  lower_mem_ptr_ptr, LSA_USER_ID_TYPE user_id,
                                LSA_UINT16 length, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_ALLOC_COMP_MEM(lower_mem_ptr_ptr, length, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(user_id);
    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_OHA_ALLOC_LOWER_MEM(CM_OHA_LOWER_MEM_PTR_TYPE *  lower_mem_ptr_ptr, LSA_USER_ID_TYPE user_id,
                               LSA_UINT16 length, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_ALLOC_COMP_MEM(lower_mem_ptr_ptr, length, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(user_id);
    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_RPC_ALLOC_LOWER_MEM(CM_RPC_LOWER_MEM_PTR_TYPE *  lower_mem_ptr_ptr, LSA_USER_ID_TYPE user_id,
                               LSA_UINT16 length, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_ALLOC_COMP_MEM(lower_mem_ptr_ptr, length, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(user_id);
    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_MRP_ALLOC_LOWER_MEM(CM_MRP_LOWER_MEM_PTR_TYPE *  lower_mem_ptr_ptr, LSA_USER_ID_TYPE user_id,
                               LSA_UINT16 length, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_ALLOC_COMP_MEM(lower_mem_ptr_ptr, length, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(user_id);
    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_POF_ALLOC_LOWER_MEM(CM_POF_LOWER_MEM_PTR_TYPE *  lower_mem_ptr_ptr, LSA_USER_ID_TYPE user_id,
                               LSA_UINT16 length, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_ALLOC_COMP_MEM(lower_mem_ptr_ptr, length, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(user_id);
    LSA_UNUSED_ARG(sys_ptr);
}


/*=============================================================================
 * function name:  CM_FREE_LOWER_MEM
 *
 * function:       free a lower-memory
 *
 * documentation:  LSA_CM_Detailspec.doc
 *
 *===========================================================================*/

void    CM_ACP_FREE_LOWER_MEM(LSA_UINT16 *  ret_val_ptr, CM_ACP_LOWER_MEM_PTR_TYPE lower_mem_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_mem_ptr, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_ACP_FREE_ALARM_MEM(LSA_UINT16 *  ret_val_ptr, CM_ACP_LOWER_MEM_PTR_TYPE lower_mem_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_mem_ptr, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_EDD_FREE_LOWER_MEM(LSA_UINT16 *  ret_val_ptr, CM_EDD_LOWER_MEM_PTR_TYPE lower_mem_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_mem_ptr, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_GSY_FREE_LOWER_MEM(LSA_UINT16 *  ret_val_ptr, CM_GSY_LOWER_MEM_PTR_TYPE lower_mem_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_mem_ptr, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_NARE_FREE_LOWER_MEM(LSA_UINT16 *  ret_val_ptr, CM_NARE_LOWER_MEM_PTR_TYPE lower_mem_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_mem_ptr, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_OHA_FREE_LOWER_MEM(LSA_UINT16 *  ret_val_ptr, CM_OHA_LOWER_MEM_PTR_TYPE lower_mem_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_mem_ptr, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_RPC_FREE_LOWER_MEM(LSA_UINT16 *  ret_val_ptr, CM_RPC_LOWER_MEM_PTR_TYPE lower_mem_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_mem_ptr, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_MRP_FREE_LOWER_MEM(LSA_UINT16 *  ret_val_ptr, CM_MRP_LOWER_MEM_PTR_TYPE lower_mem_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_mem_ptr, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(sys_ptr);
}


void    CM_POF_FREE_LOWER_MEM(LSA_UINT16 *  ret_val_ptr, CM_POF_LOWER_MEM_PTR_TYPE lower_mem_ptr, LSA_SYS_PTR_TYPE sys_ptr)
{
    LSAS_FREE_COMP_MEM(ret_val_ptr, lower_mem_ptr, LSA_COMP_ID_CM);
    LSA_UNUSED_ARG(sys_ptr);
}


LSA_UINT16 cm_version ( LSA_UINT16 length, LSA_VERSION_TYPE *version )
{
  LSA_UINT8 *p_version, *p_v;
  LSA_UINT16 i;

  /* Initialize version struct */
  LSA_VERSION_TYPE v = {
                        PNIO_LSA_COMPONENT_ID, {'L','S','A','_','C','M','\0','\0','\0','\0'},
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


#ifndef CM_PUT8_HTON
LSA_VOID CM_PUT8_HTON( /* put and convert from host byte order to network byte order */
    LSA_VOID_PTR_TYPE base,
    LSA_UINT32 offset,
    LSA_UINT8 val
)
{
    LSA_UINT8 *ptr = base;
    ptr += offset;
    CM_MEMCPY (ptr, & val, sizeof(val));
}
#else
#error "by design a function"
#endif

#ifndef CM_GET8_NTOH
LSA_UINT8 CM_GET8_NTOH( /* get and convert from network byte order to host byte order */
    LSA_VOID_PTR_TYPE base,
    LSA_UINT32 offset
) {
    return *(LSA_UINT8*)( (LSA_UINT8*)base + offset );
}
#else
#error "by design a function"
#endif

#ifndef CM_PUT16_HTON
LSA_VOID CM_PUT16_HTON( /* put and convert from host byte order to network byte order */
    LSA_VOID_PTR_TYPE base,
    LSA_UINT32 offset,
    LSA_UINT16 val
)
{
    LSA_UINT8 *ptr = base;
    ptr += offset;
    /* val = htons (val); */
    val = CM_HTON16 (val);
    CM_MEMCPY (ptr, & val, sizeof(val));
}
#else
#error "by design a function"
#endif

#ifndef CM_GET16_NTOH
LSA_UINT16 CM_GET16_NTOH( /* get and convert from network byte order to host byte order */
    LSA_VOID_PTR_TYPE base,
    LSA_UINT32 offset
)
{
    LSA_UINT16  val;
    LSA_UINT8   *ptr = base;
    ptr += offset;
    CM_MEMCPY (& val, ptr, sizeof(val));
    /* val = ntohs (val); */
    val = CM_NTOH16 (val);
    return val;
}
#else
#error "by design a function"
#endif

#ifndef CM_PUT32_HTON
LSA_VOID CM_PUT32_HTON( /* put and convert from host byte order to network byte order */
    LSA_VOID_PTR_TYPE base,
    LSA_UINT32 offset,
    LSA_UINT32 val
)
{
    LSA_UINT8 *ptr = base;
    ptr += offset;
    /* val = htonl (val); */
    val = CM_HTON32 (val);
    CM_MEMCPY (ptr, & val, sizeof(val));
}
#else
#error "by design a function"
#endif

#ifndef CM_GET32_NTOH
LSA_UINT32 CM_GET32_NTOH( /* get and convert from network byte order to host byte order */
    LSA_VOID_PTR_TYPE base,
    LSA_UINT32 offset
)
{
    LSA_UINT32  val;
    LSA_UINT8   *ptr = base;
    ptr += offset;
    CM_MEMCPY (& val, ptr, sizeof(val));
    /* val = ntohl (val); */
    val = CM_NTOH32 (val);
    return val;
}
#else
#error "by design a function"
#endif

/*****************************************************************************/
/*  end of file.                                                             */
/*****************************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
