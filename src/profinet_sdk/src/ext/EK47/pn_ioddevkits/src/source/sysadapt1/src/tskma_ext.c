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
/*  F i l e               &F: tskma_ext.c                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*
 * tskma_ext.c
 *
 */

#include "tskma_inc.h"
#include "tskma_int.h"

#include "tskma_dat.h"

#define TSKMA_MODULE_ID TSKMA_MODULE_ID_TSKMA

void  TSKMA_TASK_INIT_STATE (PNIO_UINT32 TaskNum)
{
    while(!tskma_data.cold_start.done)
    {
        union
        {
            TSKMA_MAIL_ELEM_S_PTR_T     rqb_ptr;
            TSKMA_VOID_PTR_TYPE         void_ptr;
        }msg;

        msg.rqb_ptr = NULL;
        msg.void_ptr = NULL;

        if( (PNIO_OK == OsReadMessageBlocked(&msg.void_ptr, tskma_com_data.task_info[TaskNum].task_id)) &&
        	(NULL != msg.rqb_ptr) && ( (msg.rqb_ptr)->method_ptr ) )
        {
        	( (msg.rqb_ptr)->method_ptr ) (msg.void_ptr);
        }
        else
        {
          OsWait_ms(1);
        }
    }
}


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
