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
/*  F i l e               &F: tskma.c                                   :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*   TSKMA init, open and fatal error                                        */
/*                                                                           */
/*****************************************************************************/


#include "tskma_inc.h"
#include "tskma_int.h"
#include "tskma_dat.h"

#define TSKMA_MODULE_ID 	TSKMA_MODULE_ID_TSKMA

#if DEBUG_TASKCNT
    extern PNIO_UINT32      TaskCnt[MAXNUM_OF_TASKS];
#endif

//*----------------------------------------------------------------------------
//*	tskma_init()
//*
//* @brief initialize tskma
//*----------------------------------------------------------------------------
LSA_VOID tskma_init()
{
    #if DEBUG_TASKCNT
    LSA_UINT32  taskID;

    for ( taskID = 0; taskID < MAXNUM_OF_TASKS; taskID ++ )
    {
        TaskCnt[taskID] = 0;
    }
    #endif

    TSKMA_MEMSET(&tskma_data, 0, sizeof(tskma_data));

    TSKMA_MEMSET(&tskma_com_data, 0 ,sizeof(tskma_com_data));

    TSKMA_MEMSET(&tskma_sem_res, 0, sizeof(tskma_sem_res));

    tskma_parity_error_count  = 0;
    tskma_parity_error_source = 0;
    tskma_access_error_count  = 0;
    tskma_access_error_source = 0;

    /* init nrt receive resource list */
    tskma_init_list__(&tskma_data.nrt_rcv.res_list);

    /* preset static rqbs */
    tskma_data.rqb.cyclic_ea_rqb.args = &tskma_data.rqb.cyclic_ea_rqb_args;

    tskma_com_data.task_info[TSKMA_TASK_NUM_APP].task_ptr = tskma_task_app;
    tskma_com_data.task_info[TSKMA_TASK_NUM_EDD].task_ptr = tskma_task_edd;
    tskma_com_data.task_info[TSKMA_TASK_NUM_ORG].task_ptr = tskma_task_org;
    tskma_com_data.task_info[TSKMA_TASK_NUM_PNO].task_ptr = tskma_task_pno;
    tskma_com_data.task_info[TSKMA_TASK_NUM_TCP].task_ptr = tskma_task_tcp;
    tskma_com_data.task_info[TSKMA_TASK_NUM_POF].task_ptr = tskma_task_pof;
    tskma_com_data.task_info[TSKMA_TASK_NUM_REM].task_ptr = tskma_task_rem;
    tskma_com_data.task_info[TSKMA_TASK_NUM_STP].task_ptr = tskma_task_stp;
    tskma_com_data.task_info[TSKMA_TASK_NUM_IDL].task_ptr = tskma_task_idl;

    tskma_com_data.task_info[TSKMA_TASK_NUM_APP].task_prio = TSKMA_TASK_PRIO_APP;
    tskma_com_data.task_info[TSKMA_TASK_NUM_EDD].task_prio = TSKMA_TASK_PRIO_EDD;
    tskma_com_data.task_info[TSKMA_TASK_NUM_ORG].task_prio = TSKMA_TASK_PRIO_ORG;
    tskma_com_data.task_info[TSKMA_TASK_NUM_PNO].task_prio = TSKMA_TASK_PRIO_PNO;
    tskma_com_data.task_info[TSKMA_TASK_NUM_TCP].task_prio = TSKMA_TASK_PRIO_TCP;
    tskma_com_data.task_info[TSKMA_TASK_NUM_POF].task_prio = TSKMA_TASK_PRIO_POF;
    tskma_com_data.task_info[TSKMA_TASK_NUM_REM].task_prio = TSKMA_TASK_PRIO_REM;
    tskma_com_data.task_info[TSKMA_TASK_NUM_STP].task_prio = TSKMA_TASK_PRIO_STP;
    tskma_com_data.task_info[TSKMA_TASK_NUM_IDL].task_prio = TSKMA_TASK_PRIO_IDL;

    tskma_com_data.task_info[TSKMA_TASK_NUM_APP].task_name_ptr = TSKMA_TASK_NAME_APP;
    tskma_com_data.task_info[TSKMA_TASK_NUM_EDD].task_name_ptr = TSKMA_TASK_NAME_EDD;
    tskma_com_data.task_info[TSKMA_TASK_NUM_ORG].task_name_ptr = TSKMA_TASK_NAME_ORG;
    tskma_com_data.task_info[TSKMA_TASK_NUM_PNO].task_name_ptr = TSKMA_TASK_NAME_PNO;
    tskma_com_data.task_info[TSKMA_TASK_NUM_TCP].task_name_ptr = TSKMA_TASK_NAME_TCP;
    tskma_com_data.task_info[TSKMA_TASK_NUM_POF].task_name_ptr = TSKMA_TASK_NAME_POF;
    tskma_com_data.task_info[TSKMA_TASK_NUM_REM].task_name_ptr = TSKMA_TASK_NAME_REM;
    tskma_com_data.task_info[TSKMA_TASK_NUM_STP].task_name_ptr = TSKMA_TASK_NAME_STP;
    tskma_com_data.task_info[TSKMA_TASK_NUM_IDL].task_name_ptr = TSKMA_TASK_NAME_IDL;
}

/*----------------------------------------------------------------------------
 *	tskma_open()
 *
 *  @brief open tskma
 *
 *  @param[in]  in_param_description
 *  @param[out] out_param_description
 *  @return     return_value_description
 *
 *  Calling this function will start the productive work.
 *  This function should must be called after tskma_init
 *  Using a static os this function would be called in context
 *  of the main() function and would be used to initiate the static os
 *  within TSKMA_OPEN_DONE.
 *  Using a dynamic os that should already be initiated and running, this function
 *  would create the tskma IDLE-Task that does the rest of the startup work
 *
 *----------------------------------------------------------------------------*/

LSA_UINT32 tskma_open()
{
    LSA_UINT32 ret_val;

    /* initiate the tskma idle task all other startup work is done there */

    /* prepare static rqb's */
    TSKMA_RQB_SET_METHOD_PTR(&tskma_data.app.rqb.trigger_pndv_to_pnpb, tskma_task_app_request);
    TSKMA_RQB_SET_OPCODE(&tskma_data.app.rqb.trigger_pndv_to_pnpb, TSKMA_OPC_APP_TRIGGER_PNPB);
    tskma_data.app.rqb.trigger_pndv_to_pnpb.static_rqb_in_use = LSA_FALSE;

    TSKMA_RQB_SET_METHOD_PTR(&tskma_data.app.rqb.trigger_pnpb_to_pndv, tskma_task_app_request);
    TSKMA_RQB_SET_OPCODE(&tskma_data.app.rqb.trigger_pnpb_to_pndv, TSKMA_OPC_APP_TRIGGER_PNDV);
    tskma_data.app.rqb.trigger_pnpb_to_pndv.static_rqb_in_use = LSA_FALSE;

    tskma_data.cold_start.done = LSA_FALSE;

    TSKMA_OS_TASK_CREATE(  tskma_com_data.task_info[TSKMA_TASK_NUM_IDL].task_ptr,
                           tskma_com_data.task_info[TSKMA_TASK_NUM_IDL].task_prio,
                          &tskma_com_data.task_info[TSKMA_TASK_NUM_IDL].task_id,
                           tskma_com_data.task_info[TSKMA_TASK_NUM_IDL].task_name_ptr,
                          &ret_val);
   	TSKMA_CHECK_FATAL(ret_val);

    TSKMA_OS_TASK_START(  tskma_com_data.task_info[TSKMA_TASK_NUM_IDL].task_id, &ret_val);
   	TSKMA_CHECK_FATAL(ret_val);

    TSKMA_OPEN_DONE();

    return TSKMA_RET_OK;
}


//*----------------------------------------------------------------------------
//*	tskma_start()
//*
//* @brief start tskma
//*----------------------------------------------------------------------------
LSA_VOID tskma_start(LSA_VOID)
{

}


LSA_FATAL_ERROR_TYPE tskma_fatal_var; /* for some debuggers that have problems with the stack-trace */

//*----------------------------------------------------------------------------
//*	tskma_fatal()
//*
//* @brief fatal error
//*----------------------------------------------------------------------------
LSA_VOID tskma_fatal (LSA_UINT16 tskma_module_id, LSA_INT line, LSA_UINT32 ec_0)
{
    tskma_fatal_var.lsa_component_id = LSA_COMP_ID_TSKMA;
    tskma_fatal_var.module_id        = tskma_module_id;
    tskma_fatal_var.line             = (LSA_UINT16)line;

    tskma_fatal_var.error_code[0]    = ec_0;
    tskma_fatal_var.error_code[1]    = 0;
    tskma_fatal_var.error_code[2]    = 0;
    tskma_fatal_var.error_code[3]    = 0;

    tskma_fatal_var.error_data_length = 0;
    tskma_fatal_var.error_data_ptr    = LSA_NULL;

    TSKMA_FATAL_ERROR(sizeof(tskma_fatal_var), &tskma_fatal_var);
    /* not reached */
}


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
