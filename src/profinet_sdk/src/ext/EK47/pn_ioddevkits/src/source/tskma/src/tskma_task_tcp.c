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
/*  F i l e               &F: tskma_task_tcp.c                          :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*   TSKMA message handler for REM task                                      */
/*                                                                           */
/*****************************************************************************/

#include "tskma_inc.h"
#include "tskma_int.h"
#include "tskma_dat.h"

#define TSKMA_MODULE_ID 	TSKMA_MODULE_ID_TSKMA_TASK_TCP

#if DEBUG_TASKCNT
	extern PNIO_UINT32 		TaskCnt[MAXNUM_OF_TASKS];
#endif


//*----------------------------------------------------------------------------
//*	tskma_task_tcp_request()
//*
//* Service TCP requests
//*----------------------------------------------------------------------------
void tskma_task_tcp_request(TSKMA_RQB_S_PTR_T rqb_ptr)
{
    switch(TSKMA_RQB_GET_OPCODE(rqb_ptr))
    {
        case TSKMA_OPC_TCP_OPEN:
        {
        	// ...
        	TSKMA_RQB_SET_METHOD_PTR(rqb_ptr, tskma_task_stp_perform_cold_start_sm);
			TSKMA_OS_SEND_MAIL(TSKMA_TASK_NUM_STP, rqb_ptr);
            break;
        }
        case TSKMA_OPC_TCP_SOCK_TIMEOUT:
        {
            //tskma_task_tcp_sock_timeout();
            break;
        }
        case TSKMA_OPC_TCP_TCIP_TIMEOUT:
        {
            //tskma_task_tcp_tcip_timeout();
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
//*	tskma_task_tcp()
//*
//* TSKMA message handler for TCP task
//*----------------------------------------------------------------------------
OS_CODE_FAST void tskma_task_tcp (void)
{
	#if DEBUG_TASKCNT
	LSA_UINT32  taskID;
	#endif

    TSKMA_OS_WAIT_ON_TASKSTART();

	#if DEBUG_TASKCNT
	taskID = OsGetThreadId();
	#endif

    TSKMA_TASK_INIT_STATE(TSKMA_TASK_NUM_TCP );

    for(;;)
    {
        union
        {
            TSKMA_MAIL_ELEM_S_PTR_T     rqb_ptr;
            TSKMA_VOID_PTR_TYPE         void_ptr;
        }msg;

        TSKMA_OS_READ_MAIL(&msg.void_ptr, TSKMA_TASK_NUM_TCP);

        while( (msg.void_ptr) )
        {
        	LSA_RQB_GET_METHOD_PTR(msg.rqb_ptr) (msg.void_ptr);

        	TSKMA_OS_READ_MAIL(&msg.void_ptr, TSKMA_TASK_NUM_TCP);

        	#if DEBUG_TASKCNT
        	TaskCnt[taskID]++;
            #endif
        }

        TSKMA_WAKE_NEXT_TS_TASK(TSKMA_TASK_NUM_TCP);
    }
}

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
