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
/*  F i l e               &F: tskma_task_edd.c                          :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*   TSKMA message handler for EDD task                                      */
/*                                                                           */
/*****************************************************************************/


#include "tskma_inc.h"
#include "tskma_int.h"
#include "tskma_dat.h"

#define TSKMA_MODULE_ID 	TSKMA_MODULE_ID_TSKMA_TASK_EDD

#if DEBUG_TASKCNT
	extern PNIO_UINT32 		TaskCnt[MAXNUM_OF_TASKS];
#endif
	extern LSA_VOID lsas_edds_scheduler_edd_task(LSAS_EDD_RQB_TYPE *rqb);

//*----------------------------------------------------------------------------
//*	tskma_task_edd_request()
//*
//* Service EDD messages
//*----------------------------------------------------------------------------
void tskma_task_edd_request(TSKMA_RQB_S_PTR_T rqb_ptr)
{
    switch(TSKMA_RQB_GET_OPCODE(rqb_ptr))
    {
		case TSKMA_OPC_EDD_REQ_REBOOT:
		{
			OsRebootService();
			break;
		}
		case TSKMA_OPC_EDD_REQ_RESERVED:
		{
			// currently no own services in this task
			TSKMA_FATAL(TSKMA_RQB_GET_OPCODE(rqb_ptr));
			break;
		}
        default:
        {
            /* opcode not supported */
            TSKMA_FATAL(TSKMA_RQB_GET_OPCODE(rqb_ptr));
        }
    }
}
#if(PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2_EDDS)
PNIO_UINT32 g_EddTimerTaskLock = 0;
PNIO_UINT32 g_EddTimerTaskId = 0;

OS_CODE_FAST void tskma_task_edd_timer (void)
{
	LSAS_EDD_HANDLE hDDB = LSA_NULL;
	LSA_UINT16 nRedRatio = 0;
	while(LSA_NULL == hDDB)
	{
		OsWait_ms(10);
		hDDB = lsas_com_get_hddb();
	}

	while(1)
	{
		OsTakeSemB(g_EddTimerTaskLock);
		edds_scheduler(hDDB, EDDS_SCHEDULER_CYCLIC_CALL);
	}

}
#endif

//*----------------------------------------------------------------------------
//*	tskma_task_edd()
//*
//* TSKMA message handler for EDDx
//*----------------------------------------------------------------------------
OS_CODE_FAST void tskma_task_edd (void)
{
    LSA_UINT32 ret_val;
	#if DEBUG_TASKCNT
	LSA_UINT32  taskID;
	#endif
	LSAS_EDD_HANDLE hDDB = LSA_NULL;
	LSA_UINT16 EddTimerId = 0U;


    /* establish semaphores */
    TSKMA_OS_SEM_B_CREATE(&tskma_sem_res.id_edd_lock_com, ret_val);
   	TSKMA_CHECK_FATAL(ret_val);

   	TSKMA_OS_SEM_B_GIVE(tskma_sem_res.id_edd_lock_com, ret_val);
   	TSKMA_CHECK_FATAL(ret_val);

   	//TSKMA_OS_MBOX_CREATE(tskma_com_data.task_info[TSKMA_TASK_NUM_EDD].task_id, TSKMA_TASK_NUM_EDD);

    TSKMA_OS_WAIT_ON_TASKSTART();

#if(PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2_EDDS)
    ret_val = OsAllocSemB(&g_EddTimerTaskLock);

    if((PNIO_OK == ret_val) && (PNIO_OK == OsCreateThread(tskma_task_edd_timer, (PNIO_UINT8*)"EDD-Timer", 27, &g_EddTimerTaskId)) )
    {
    	OsStartThread(g_EddTimerTaskId);
    }
#endif

	#if DEBUG_TASKCNT
	taskID = OsGetThreadId();
	#endif

    TSKMA_TASK_INIT_STATE(TSKMA_TASK_NUM_EDD);

//    for(;;)
    {
    	LSAS_EDD_RQB_TYPE *pRQB;
        union
        {
            TSKMA_MAIL_ELEM_S_PTR_T     rqb_ptr;
            TSKMA_VOID_PTR_TYPE         void_ptr;
        }msg;

        msg.void_ptr = LSA_NULL;

    //    TSKMA_OS_READ_MAIL(&msg.void_ptr, TSKMA_TASK_NUM_EDD);

        while( 1)//(msg.void_ptr) )
        {
            //LSA_RQB_GET_METHOD_PTR(msg.rqb_ptr) (msg.void_ptr);

            TSKMA_OS_READ_MAIL(&msg.void_ptr, TSKMA_TASK_NUM_EDD);

            ret_val = (LSA_UINT32)msg.void_ptr;

            if(LSA_NULL == hDDB)
            {
            	hDDB = lsas_com_get_hddb();
            }

            switch(ret_val)
            {
#if(PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2_EDDS)
            case EDDS_SCHEDULER_INTERMEDIATE_CALL: /* Intermediate call */
            case EDDS_SCHEDULER_IRQ_RX: /* Receive Interrupt Request */
            case EDDS_SCHEDULER_IRQ_TX: /* Transmit Ready Interrupt Request */
            case EDDS_SCHEDULER_IRQ_RX_TX: /* Transmit Interrupt Request */
            case EDDS_SCHEDULER_IRQ_LK_CHG: /* Link change Interrupt Request */
            	if(LSA_NULL != hDDB)
				{
            		 edds_scheduler(hDDB, ret_val);
				}
            	break;
            case 0:
            	break;
#endif
            default:
            	pRQB = (LSAS_EDD_RQB_TYPE*) msg.void_ptr;
            	if(LSA_NULL  != pRQB->method_ptr)
            	{
            		//LSA_RQB_GET_METHOD_PTR(msg.rqb_ptr) (msg.void_ptr);
            		pRQB->method_ptr(pRQB);
            	}
            	break;
            }

            msg.void_ptr = LSA_NULL;

            #if DEBUG_TASKCNT
            TaskCnt[taskID]++;
            #endif
        }


        TSKMA_WAKE_NEXT_TS_TASK(TSKMA_TASK_NUM_EDD);
    }
}


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
