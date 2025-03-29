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
/*  F i l e               &F: tskma_task_stp.c                          :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*   TSKMA message handler for STARTUP task                                  */
/*                                                                           */
/*****************************************************************************/


#include "tskma_inc.h"
#include "tskma_int.h"
#include "tskma_dat.h"

#define TSKMA_MODULE_ID 	TSKMA_MODULE_ID_TSKMA_TASK_STP

#if DEBUG_TASKCNT
	extern PNIO_UINT32 		TaskCnt[MAXNUM_OF_TASKS];
#endif

static LSA_UINT32 TskmaSemId = 0;

TSKMA_DATA_S_T tskma_data;
//*----------------------------------------------------------------------------
//*	tskma_task_stp_perform_cold_start()
//*
//* Initially trigger cold start state machine
//*----------------------------------------------------------------------------
void tskma_task_stp_perform_cold_start(void)
{
	tskma_data.cold_start.sm = TSKMA_COLD_START_SM_ALLOC;
	tskma_task_stp_perform_cold_start_sm();
}


//*----------------------------------------------------------------------------
//*	tskma_task_stp_perform_cold_start_sm()
//*
//* Cold start state machine
//*----------------------------------------------------------------------------
void tskma_task_stp_perform_cold_start_sm(void)
{
    switch(tskma_data.cold_start.sm)
    {
    	case TSKMA_COLD_START_SM_ALLOC:
    	{
    		tskma_data.cold_start.sm = TSKMA_COLD_START_SM_APP_INIT;
    		// allocate semaphore
    		OsAllocSemB (&TskmaSemId);
    		tskma_data.cold_start.lsas_rqb.args = &tskma_data.cold_start.lsas_rqb_args;
    	}
    	// no break
        //lint -fallthrough
        case TSKMA_COLD_START_SM_APP_INIT:
    	{
    		tskma_data.cold_start.sm = TSKMA_COLD_START_SM_INIT_PNIO_STACK;
			TSKMA_RQB_SET_OPCODE(&tskma_data.cold_start.tskma_rqb, TSKMA_OPC_APP_INIT);
			TSKMA_RQB_SET_METHOD_PTR(&tskma_data.cold_start.tskma_rqb, tskma_task_app_request);
			TSKMA_OS_SEND_MAIL(TSKMA_TASK_NUM_APP, &tskma_data.cold_start.tskma_rqb);
			break;
    	}
    	case TSKMA_COLD_START_SM_INIT_PNIO_STACK:
    	{
    		tskma_data.cold_start.sm = TSKMA_COLD_START_SM_SETUP_PNIO_STACK;

    		lsas_init();

    		TSKMA_RQB_SET_OPCODE(&tskma_data.cold_start.lsas_rqb, LSAS_OPC_INIT_PNIO_STACK);
			TSKMA_RQB_SET_METHOD_PTR(&tskma_data.cold_start.lsas_rqb, lsas_request);
			TSKMA_OS_SEND_MAIL(TSKMA_TASK_NUM_PNO, &tskma_data.cold_start.lsas_rqb);
    		break;
    	}
    	case TSKMA_COLD_START_SM_SETUP_PNIO_STACK:
    	{
    		tskma_data.cold_start.sm = TSKMA_COLD_START_SM_APP_OPEN_01;
			TSKMA_RQB_SET_OPCODE(&tskma_data.cold_start.lsas_rqb, LSAS_OPC_SETUP_PNIO_STACK);
			TSKMA_RQB_SET_METHOD_PTR(&tskma_data.cold_start.lsas_rqb, lsas_request);
			TSKMA_OS_SEND_MAIL(TSKMA_TASK_NUM_EDD,&tskma_data.cold_start.lsas_rqb);
			break;
    	}
    	case TSKMA_COLD_START_SM_APP_OPEN_01:
    	{
    		tskma_data.cold_start.sm = TSKMA_COLD_START_SM_APP_BACKUP_MATCH_NOS;
    		TSKMA_RQB_SET_OPCODE(&tskma_data.cold_start.tskma_rqb, TSKMA_OPC_APP_OPEN_01);
			TSKMA_RQB_SET_METHOD_PTR(&tskma_data.cold_start.tskma_rqb, tskma_task_app_request);
			TSKMA_OS_SEND_MAIL(TSKMA_TASK_NUM_APP, &tskma_data.cold_start.tskma_rqb);
			break;
    	}
    	case TSKMA_COLD_START_SM_APP_BACKUP_MATCH_NOS:
    	{
    		tskma_data.cold_start.sm = TSKMA_COLD_START_SM_TCP_OPEN;
			TSKMA_RQB_SET_OPCODE(&tskma_data.cold_start.tskma_rqb, TSKMA_OPC_APP_BACKUP_MATCH_NOS);
			TSKMA_RQB_SET_METHOD_PTR(&tskma_data.cold_start.tskma_rqb, tskma_task_app_request);
			TSKMA_OS_SEND_MAIL(TSKMA_TASK_NUM_APP, &tskma_data.cold_start.tskma_rqb);
			break;
    	}
    	case TSKMA_COLD_START_SM_TCP_OPEN:
    	{
    		tskma_data.cold_start.sm = TSKMA_COLD_START_SM_PNO_REQ_OPEN;
			TSKMA_RQB_SET_OPCODE(&tskma_data.cold_start.tskma_rqb, TSKMA_OPC_TCP_OPEN);
			TSKMA_RQB_SET_METHOD_PTR(&tskma_data.cold_start.tskma_rqb, tskma_task_tcp_request);
			TSKMA_OS_SEND_MAIL(TSKMA_TASK_NUM_TCP, &tskma_data.cold_start.tskma_rqb);
			break;
    	}
    	case TSKMA_COLD_START_SM_PNO_REQ_OPEN:
    	{
    		tskma_data.cold_start.sm = TSKMA_COLD_START_SM_OPEN_PNIO_STACK;
    		TSKMA_RQB_SET_OPCODE(&tskma_data.cold_start.tskma_rqb, TSKMA_OPC_PNO_REQ_OPEN);
    		TSKMA_RQB_SET_METHOD_PTR(&tskma_data.cold_start.tskma_rqb, tskma_task_pno_request);
    		TSKMA_OS_SEND_MAIL(TSKMA_TASK_NUM_PNO, &tskma_data.cold_start.tskma_rqb);
    		break;
    	}
    	case TSKMA_COLD_START_SM_OPEN_PNIO_STACK:
    	{
    		tskma_data.cold_start.sm = TSKMA_COLD_START_SM_APP_OPEN_02;
			/* insert open channel here */
			LSA_RQB_SET_OPCODE(&tskma_data.cold_start.lsas_rqb, LSAS_OPC_OPEN_PNIO_STACK);
			LSA_RQB_SET_METHOD_PTR(&tskma_data.cold_start.lsas_rqb, lsas_request);
			TSKMA_OS_SEND_MAIL(TSKMA_TASK_NUM_STP, &tskma_data.cold_start.lsas_rqb)
			break;
    	}
    	case TSKMA_COLD_START_SM_APP_OPEN_02:
    	{
    		tskma_data.cold_start.sm = TSKMA_COLD_START_SM_APP_START;
    		TSKMA_RQB_SET_OPCODE(&tskma_data.cold_start.tskma_rqb, TSKMA_OPC_APP_OPEN_02);
    		TSKMA_RQB_SET_METHOD_PTR(&tskma_data.cold_start.tskma_rqb, tskma_task_app_request);
    		TSKMA_OS_SEND_MAIL(TSKMA_TASK_NUM_APP, &tskma_data.cold_start.tskma_rqb);
			break;
    	}
    	case TSKMA_COLD_START_SM_APP_START:
    	{
    		tskma_data.cold_start.sm = TSKMA_COLD_START_SM_APP_CYCLIC_EA;
    		TSKMA_RQB_SET_OPCODE(&tskma_data.cold_start.tskma_rqb, TSKMA_OPC_APP_START);
    		TSKMA_RQB_SET_METHOD_PTR(&tskma_data.cold_start.tskma_rqb, tskma_task_app_request);
    		TSKMA_OS_SEND_MAIL(TSKMA_TASK_NUM_APP, &tskma_data.cold_start.tskma_rqb);
			break;
    	}
    	case TSKMA_COLD_START_SM_APP_CYCLIC_EA:
    	{
    		tskma_data.cold_start.sm = TSKMA_COLD_START_SM_DONE;
            // init cyclic EA request
    		TSKMA_RQB_SET_OPCODE    (&tskma_data.rqb.cyclic_ea_rqb, TSKMA_OPC_APP_CYCLIC_EA);
    		TSKMA_RQB_SET_METHOD_PTR(&tskma_data.rqb.cyclic_ea_rqb, tskma_task_app_request);
    		// trigger initial pndv and pnpb perform service
    		TSKMA_RQB_SET_OPCODE    (&tskma_data.cold_start.tskma_rqb, TSKMA_OPC_APP_INIT_EA);
    		TSKMA_RQB_SET_METHOD_PTR(&tskma_data.cold_start.tskma_rqb, tskma_task_app_request);
    		TSKMA_OS_SEND_MAIL(TSKMA_TASK_NUM_APP, &tskma_data.cold_start.tskma_rqb);
    		break;
    	}
    	case TSKMA_COLD_START_SM_DONE:
    	{
    		/* after initialization all volatile options are expected to be performed */
    		//apma_control_reset_volatile_options();
    		//(*ERTEC200_SCRB).handrad_counter1.reg = 0;
    		/* startup was observed by watchdog and has been completed at this point */
    		TSKMA_SERVE_WD();
    		break;
    	}
    	case TSKMA_COLD_START_SM_CLOSED:
    	default:
    	{
    		TSKMA_FATAL(tskma_data.cold_start.sm);
    	}
    }
}


//*----------------------------------------------------------------------------
//*	tskma_task_stp_request()
//*
//* Service STARTUP requests
//*----------------------------------------------------------------------------
void tskma_task_stp_request(TSKMA_RQB_S_PTR_T rqb_ptr)
{
    switch(TSKMA_RQB_GET_OPCODE(rqb_ptr))
    {
        case TSKMA_OPC_STP_PERFORM_COLD_START:
        {
            if (tskma_data.cold_start.done == LSA_FALSE)
            {
                LSAS_RQB_PTR_TYPE   lsas_rqb_ptr;

                TSKMA_ALLOC_MEM(&lsas_rqb_ptr, sizeof(LSAS_RQB_TYPE));

                lsas_init();

                TSKMA_RQB_SET_OPCODE(lsas_rqb_ptr, LSAS_OPC_INIT_PNIO_STACK);
                TSKMA_RQB_SET_METHOD_PTR(lsas_rqb_ptr, lsas_request);
                TSKMA_OS_SEND_MAIL(TSKMA_TASK_NUM_PNO, lsas_rqb_ptr);

                /* if reaching here the request has been processed */
                TSKMA_FREE_MEM(lsas_rqb_ptr);
            }
            else
            {
                /* only allowed during startup */
                TSKMA_FATAL(tskma_data.cold_start.done);
            }

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
//*	tskma_task_stp()
//*
//* TSKMA message handler for STARTUP task
//*----------------------------------------------------------------------------
void tskma_task_stp (void)
{
	#if DEBUG_TASKCNT
	LSA_UINT32  taskID;
	#endif
    //TSKMA_OS_MBOX_CREATE(tskma_com_data.task_info[TSKMA_TASK_NUM_STP].task_id, TSKMA_TASK_NUM_STP);

    TSKMA_OS_WAIT_ON_TASKSTART();

	#if DEBUG_TASKCNT
	taskID = OsGetThreadId();
	#endif

    // startup the device ...
    tskma_task_stp_perform_cold_start();
    // ... startup done when reaching here

    TSKMA_TASK_INIT_STATE(TSKMA_TASK_NUM_STP);

    for(;;)
    {
        union
        {
            TSKMA_MAIL_ELEM_S_PTR_T     rqb_ptr;
            TSKMA_VOID_PTR_TYPE         void_ptr;
        }msg;

        TSKMA_OS_READ_MAIL(&msg.void_ptr, TSKMA_TASK_NUM_STP);

        while( (msg.void_ptr) )
        {
            LSA_RQB_GET_METHOD_PTR(msg.rqb_ptr) (msg.void_ptr);

            TSKMA_OS_READ_MAIL(&msg.void_ptr, TSKMA_TASK_NUM_STP);

            #if DEBUG_TASKCNT
            TaskCnt[taskID]++;
            #endif
        }

        TSKMA_WAKE_NEXT_TS_TASK(TSKMA_TASK_NUM_STP);
    }
}

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
