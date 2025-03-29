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
/*  F i l e               &F: tskma_task_pno.c                          :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*   TSKMA message handler for PNO task                                      */
/*                                                                           */
/*****************************************************************************/


#include "tskma_inc.h"
#include "tskma_int.h"
#include "tskma_dat.h"

#define TSKMA_MODULE_ID		TSKMA_MODULE_ID_TSKMA_TASK_PNO

#if DEBUG_TASKCNT
	extern PNIO_UINT32 		TaskCnt[MAXNUM_OF_TASKS];
#endif


//*----------------------------------------------------------------------------
//*	tskma_task_pno_req_open()
//*
//* Initialize OHA database
//*----------------------------------------------------------------------------
void tskma_task_pno_req_open(void)
{
    LSAS_RQB_TYPE           rqb;
    LSAS_RQB_ARGS_TYPE      rqb_args;

    rqb.args = &rqb_args;

    TSKMA_MEMSET(&rqb_args, 0, sizeof(rqb_args));

    rqb_args.oha_db_init.device_id = TSKMA_IM_DEVICE_ID();
    rqb_args.oha_db_init.interface_id = TSKMA_IM_INTERFACE_ID();
    TSKMA_IM_GET_IP_SUITE(&rqb_args.oha_db_init.ip_suite_present, &rqb_args.oha_db_init.ip[0], &rqb_args.oha_db_init.subnet[0], &rqb_args.oha_db_init.gateway[0]);
    TSKMA_IM_GET_NAME_OF_STATION(&rqb_args.oha_db_init.name_of_station[0], (255+1));
    TSKMA_IM_GET_STATION_TYPE(&rqb_args.oha_db_init.station_type[0], sizeof(rqb_args.oha_db_init.station_type));
    TSKMA_IM_GET_ORDER_ID(&rqb_args.oha_db_init.order_id[0], (20+1));        		// order id, e.g. "6ES7 155-6AU00-0CN0 "
    TSKMA_IM_GET_SERIAL_NUMBER(&rqb_args.oha_db_init.serial_number[0], (16+1));
    rqb_args.oha_db_init.hw_revision = TSKMA_IM_GET_HW_REVISION;       				// hardware revision, 0..99999
    TSKMA_IM_GET_SW_REVISION(&rqb_args.oha_db_init.sw_revision[0], (10+1));       	// software revision, e.g. "V xx yy zz"
    TSKMA_IM_GET_SNMP_SYS_DESCR(&rqb_args.oha_db_init.sys_description[0], 256);
    rqb_args.oha_db_init.vendor_id = TSKMA_IM_VENDOR_ID();
    rqb_args.oha_db_init.device_id = TSKMA_IM_DEVICE_ID();
    rqb_args.oha_db_init.instance  = 1;

    LSAS_RQB_SET_OPCODE(&rqb, LSAS_OPC_INIT_OHA_DB);

    lsas_request(&rqb); // direct call don't send it via mail or change to dynamic mem allocation
}


//*----------------------------------------------------------------------------
//*	tskma_task_pno_request()
//*
//* Service PNO requests
//*----------------------------------------------------------------------------
void tskma_task_pno_request(TSKMA_RQB_S_PTR_T rqb_ptr)
{
    switch(TSKMA_RQB_GET_OPCODE(rqb_ptr))
    {
        case TSKMA_OPC_PNO_REQ_OPEN:
        {
            tskma_task_pno_req_open();
            TSKMA_RQB_SET_METHOD_PTR(rqb_ptr, tskma_task_stp_perform_cold_start_sm);
			TSKMA_OS_SEND_MAIL(TSKMA_TASK_NUM_STP, rqb_ptr);
            break;
        }
        default:
        {
            /* opcode not supported */
            TSKMA_FATAL(TSKMA_RQB_GET_OPCODE(rqb_ptr));
        }
    }
}


//*----------------------------------------------------------------------------
//*	tskma_task_pno()
//*
//* TSKMA message handler for PNO task
//*----------------------------------------------------------------------------
OS_CODE_FAST void tskma_task_pno (void)
{
    LSA_UINT32 ret_val;
	#if DEBUG_TASKCNT
	LSA_UINT32  taskID;
	#endif

    /* ----- establish semaphores ------------------------- */

    TSKMA_OS_SEM_B_CREATE(&tskma_sem_res.id_pnio_wait_oh_control_done, ret_val);
   	TSKMA_CHECK_FATAL(ret_val);

   	TSKMA_OS_SEM_B_GIVE(tskma_sem_res.id_pnio_wait_oh_control_done, ret_val);
   	TSKMA_CHECK_FATAL(ret_val);

    TSKMA_OS_SEM_B_CREATE(&tskma_sem_res.id_pnio_wait_oh_crc16_done, ret_val);
   	TSKMA_CHECK_FATAL(ret_val);

   	TSKMA_OS_SEM_B_GIVE(tskma_sem_res.id_pnio_wait_oh_crc16_done, ret_val);
   	TSKMA_CHECK_FATAL(ret_val);

    TSKMA_OS_SEM_B_CREATE(&tskma_sem_res.id_pnio_wait_rema_prm_done, ret_val);
   	TSKMA_CHECK_FATAL(ret_val);

   	TSKMA_OS_SEM_B_GIVE(tskma_sem_res.id_pnio_wait_rema_prm_done, ret_val);
   	TSKMA_CHECK_FATAL(ret_val);

    TSKMA_OS_SEM_B_CREATE(&tskma_sem_res.id_pnio_wait_rema_station_prm_done, ret_val);
   	TSKMA_CHECK_FATAL(ret_val);

   	TSKMA_OS_SEM_B_GIVE(tskma_sem_res.id_pnio_wait_rema_station_prm_done, ret_val);
   	TSKMA_CHECK_FATAL(ret_val);

    TSKMA_OS_SEM_B_CREATE(&tskma_sem_res.id_pnio_wait_nrt_rcv_resources, ret_val);
   	TSKMA_CHECK_FATAL(ret_val);

   	TSKMA_OS_SEM_B_GIVE(tskma_sem_res.id_pnio_wait_nrt_rcv_resources, ret_val);
   	TSKMA_CHECK_FATAL(ret_val);

    //TSKMA_OS_MBOX_CREATE(tskma_com_data.task_info[TSKMA_TASK_NUM_PNO].task_id, TSKMA_TASK_NUM_PNO);

    TSKMA_OS_WAIT_ON_TASKSTART();

	#if DEBUG_TASKCNT
	taskID = OsGetThreadId();
	#endif

    TSKMA_TASK_INIT_STATE(TSKMA_TASK_NUM_PNO);

    for(;;)
    {
        union
        {
            TSKMA_MAIL_ELEM_S_PTR_T     rqb_ptr;
            TSKMA_VOID_PTR_TYPE         void_ptr;

        }msg;

        TSKMA_OS_READ_MAIL(&msg.void_ptr, TSKMA_TASK_NUM_PNO);

        while( (msg.void_ptr) )
        {
            LSA_RQB_GET_METHOD_PTR(msg.rqb_ptr) (msg.void_ptr);

            TSKMA_OS_READ_MAIL(&msg.void_ptr, TSKMA_TASK_NUM_PNO);

            #if DEBUG_TASKCNT
            TaskCnt[taskID]++;
            #endif
        }

        TSKMA_WAKE_NEXT_TS_TASK(TSKMA_TASK_NUM_PNO);
    }
}


/*********************************************************************************************
 * tskma_task_pno_queue_nrt_resource()
 *
 * When PNIO components provide receive resources to the EDD, we enqueue it in a list and
 * pass it to the EDD before serving the receive requests. This way the EDD has always access to
 * the maximum amount of available receive resources.
 *********************************************************************************************/
LSA_BOOL tskma_task_pno_queue_nrt_resource(LSA_VOID* lower_rqb_ptr, LSA_UINT16 modul_id, LSA_VOID* method_ptr)
{
    Boolean 	return_value;

    EDD_RQB_TYPE* edd_lower_rqb_ptr = (EDD_RQB_TYPE*) lower_rqb_ptr;

    LSA_UNUSED_ARG(modul_id);

    if (   (EDD_RQB_GET_SERVICE(edd_lower_rqb_ptr) == EDD_SRV_NRT_RECV)
        && (LSA_RQB_GET_OPCODE (edd_lower_rqb_ptr) == EDD_OPC_REQUEST)
        && (tskma_data.cold_start.sm == TSKMA_COLD_START_SM_DONE))
    {
        LSA_UINT32 	ret_val;

        /* put indication resources into separate lane                        */
        /* so resources will be available BEFORE treating a receive interrupt */

        LSA_RQB_SET_METHOD_PTR(edd_lower_rqb_ptr, method_ptr); /*lint !e611 Suspicious cast */

    	TSKMA_OS_SEM_B_TAKE(tskma_sem_res.id_pnio_wait_nrt_rcv_resources, ret_val);
       	TSKMA_CHECK_FATAL(ret_val);

        tskma_put_blk_to_list__(&tskma_data.nrt_rcv.res_list, edd_lower_rqb_ptr);

        TSKMA_OS_SEM_B_GIVE(tskma_sem_res.id_pnio_wait_nrt_rcv_resources, ret_val);
       	TSKMA_CHECK_FATAL(ret_val);

        return_value = TRUE;
    }
    else
    {
        return_value = FALSE;
    }
    return (return_value);
}

/*********************************************************************************************
 * tskma_task_pno_serve_nrt_resource_list_local()
 *
 * give back nrt receive resources to edd before receive interrupt is served
 *********************************************************************************************/
LSA_VOID tskma_task_pno_serve_nrt_resource_list_local (void)
{
    struct tskma_list_cb     tmp_resource;
    LSA_UINT32 				 ret_val;
    LSA_UINT32 				 res_count;

    /* provide possible nrt ind resources before treating nrt indication that means               */
    /* indications resources are available and the dmacw can be delivered with destination memory */
    tskma_init_list__(&tmp_resource);

	TSKMA_OS_SEM_B_TAKE(tskma_sem_res.id_pnio_wait_nrt_rcv_resources, ret_val);
   	TSKMA_CHECK_FATAL(ret_val);

    if( tskma_list_not_empty__(&tskma_data.nrt_rcv.res_list) )
    {
        /* take a snapshot of the run list */
        tmp_resource = tskma_data.nrt_rcv.res_list;

        ( TSKMA_CAST_LIST_CB_PTR (tmp_resource.next_blk_ptr)) -> prev_blk_ptr = (struct tskma_list_cb *) & (tmp_resource);
        ( TSKMA_CAST_LIST_CB_PTR (tmp_resource.prev_blk_ptr)) -> next_blk_ptr = (struct tskma_list_cb *) & (tmp_resource);

        /* reset original list */
        tskma_init_list__(&tskma_data.nrt_rcv.res_list);
    }

    TSKMA_OS_SEM_B_GIVE(tskma_sem_res.id_pnio_wait_nrt_rcv_resources, ret_val);
   	TSKMA_CHECK_FATAL(ret_val);

    // put receive resources back to EDD
    res_count = 0;
    while( tskma_list_not_empty__(&tmp_resource) )
    {
        EDD_RQB_TYPE * rqb_ptr;

		tskma_get_remove_blk_from_list__(&tmp_resource, &rqb_ptr);
    	res_count++;

        LSA_RQB_GET_METHOD_PTR(rqb_ptr) (rqb_ptr);
    }
    // statistic of max queued receive resources
    if (res_count > tskma_data.nrt_rcv.res_max)
    {
    	tskma_data.nrt_rcv.res_max = res_count;
    }
}


/*****************************************************************************
 * tskma_task_pno_serve_nrt_resource_list()
 *
 * This function provides all enqueued (=freed) receive resources to the EDD.
 * It is called before received data is processed by the EDD(I)
 ******************************************************************************/
void tskma_task_pno_serve_nrt_resource_list (void *rqb_ptr_)
{
    EDD_RQB_TYPE * rqb_ptr = (EDD_RQB_TYPE *)rqb_ptr_;
    tskma_task_pno_serve_nrt_resource_list_local();

    TSKMA_EDD_INT_REQUEST(rqb_ptr);
}

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
