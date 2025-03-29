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
/*  F i l e               &F: tskma_task_idl.c                          :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*   TSKMA message handler for IDLE task                                     */
/*                                                                           */
/*****************************************************************************/


#include "tskma_inc.h"
#include "tskma_int.h"
#include "tskma_dat.h"

#define TSKMA_MODULE_ID 	TSKMA_MODULE_ID_TSKMA_TASK_IDL

#if DEBUG_TASKCNT
	extern PNIO_UINT32 		TaskCnt[MAXNUM_OF_TASKS];
#endif


/*----------------------------------------------------------------------------
 *	tskma_task_idl()
 *
 *  @brief TSKMA message handler for IDLE task
 *
 *  The tskma idle task, as the task with the lowest priority, is used to bring
 *  all other tasks to life and initiate the startup process.
 *  Using a dynamic OS the other tasks are created.
 *  Using a static OS all other tasks are already running and is assumed that
 *  the idle task is the last task to be established.
 *  In any of the above cases the higher tasks would be suspendet waiting on there
 *  empty mailboxes.
 *  The only thing doing in this task is to send a trigger mail to STP task.
 *----------------------------------------------------------------------------*/
extern PNIO_UINT32 TskId_OsTimer;
extern PNIO_INT32  TaskOsTimer (void);
void tskma_task_idl (void)
{
    LSA_UINT32  ret_val;
    LSA_UINT32 	task_count;
	#if DEBUG_TASKCNT
		LSA_UINT32  taskID;
	#endif

    //TSKMA_OS_MBOX_CREATE(tskma_com_data.task_info[TSKMA_TASK_NUM_IDL].task_id, TSKMA_TASK_NUM_IDL); /* no need for mbox in this task */
    //TSKMA_TASK_INIT_STATE(TSKMA_TASK_NUM_IDL); /* this task has no init state */

    TSKMA_OS_WAIT_ON_TASKSTART();
	#if DEBUG_TASKCNT
		taskID = OsGetThreadId();
	#endif


	//OsIntDisable();

//	TskId_OsTimer = tskma_com_data.task_info[TSKMA_TASK_NUM_IDL].task_id;
//	OsCreateMsgQueue (TskId_OsTimer);



    /* perform cold start */

	// now start all tasks beginng with the highest priority
    // starting the last task (stp-task) will bring up the system
    for(task_count = 0; task_count < (TSKMA_TASK_NUM - 1/* IDLE TASK already started */); task_count++)
    {
        TSKMA_OS_TASK_CREATE(  tskma_com_data.task_info[task_count].task_ptr,
                               tskma_com_data.task_info[task_count].task_prio,
                              &tskma_com_data.task_info[task_count].task_id,
                               tskma_com_data.task_info[task_count].task_name_ptr,
                              &ret_val);
       	TSKMA_CHECK_FATAL(ret_val);
    }

    // create message queues, before all tasks are started. So messages can already be stored, before
    // the destination task is running.
    //for(task_count = 0; task_count < (TSKMA_TASK_NUM - 1); task_count++)
    TSKMA_OS_MBOX_CREATE(tskma_com_data.task_info[TSKMA_TASK_NUM_APP].task_id, TSKMA_TASK_NUM_APP);
    TSKMA_OS_MBOX_CREATE(tskma_com_data.task_info[TSKMA_TASK_NUM_EDD].task_id, TSKMA_TASK_NUM_EDD);
    TSKMA_OS_MBOX_CREATE(tskma_com_data.task_info[TSKMA_TASK_NUM_ORG].task_id, TSKMA_TASK_NUM_ORG);
    TSKMA_OS_MBOX_CREATE(tskma_com_data.task_info[TSKMA_TASK_NUM_PNO].task_id, TSKMA_TASK_NUM_PNO);
    TSKMA_OS_MBOX_CREATE(tskma_com_data.task_info[TSKMA_TASK_NUM_POF].task_id, TSKMA_TASK_NUM_POF);
    TSKMA_OS_MBOX_CREATE(tskma_com_data.task_info[TSKMA_TASK_NUM_REM].task_id, TSKMA_TASK_NUM_REM);
    TSKMA_OS_MBOX_CREATE(tskma_com_data.task_info[TSKMA_TASK_NUM_STP].task_id, TSKMA_TASK_NUM_STP);
    TSKMA_OS_MBOX_CREATE(tskma_com_data.task_info[TSKMA_TASK_NUM_TCP].task_id, TSKMA_TASK_NUM_TCP);

    for(task_count = 0; task_count < (TSKMA_TASK_NUM - 1); task_count++)
    {
        TSKMA_OS_TASK_START(tskma_com_data.task_info[task_count].task_id, &ret_val);
    }

    // reaching here when device startup is done
    /* leave the init state of the tasks and start the timeslicing */
	#if 1
    // continue with initial priority definition
	#else
    // set all tasks to EDD priority
    for(task_count = TSKMA_TASK_NUM_EDD/*exclude app task */; task_count < (TSKMA_TASK_NUM - 1/* IDLE TASK remains low */); task_count++)
    {
        OsSetThreadPrio(tskma_com_data.task_info[task_count].task_id,
                        tskma_com_data.task_info[TSKMA_TASK_NUM_EDD].task_prio);
    }
	#endif

    tskma_data.cold_start.done = LSA_TRUE;

 //   OsIntEnable();

#if 0
 //   TaskOsTimer();
#else
    /* we'll should never get here */
    for(;;)
    {
        TSKMA_IDLE_LOOP_CHECK();
		#if DEBUG_TASKCNT
			  TaskCnt[taskID]++;
		#endif
    }
#endif
}

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
