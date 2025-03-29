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
/*  F i l e               &F: pnpb.c                                    :F&  */
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

#include "pniousrd.h"
#include "trc_if.h"
#include "pndv_inc.h"

#include "pnpb_com.h"
#include "pnpb_peri.h"
#include "pnpb_sub_real.h"
#include "pnpb_sub_exp.h"
#include "pnpb_io.h"
#include "pnpb_cfg.h"
#include "pnpb_trc.h"
#include "pnpb.h"
#include "pnpb_api.h"

#include "os_taskprio.h"

#define LTRC_ACT_MODUL_ID   213
#define PNPB_MODULE_ID      213

// *---------------------------------------------
// * public variables
// *---------------------------------------------
volatile PNPB_INSTANCE   Pnpb;

// *---------------------------------------------
// * static variables
// *---------------------------------------------
PNIO_UINT32             TskId_Pnpb = 0;


// *---------------------------------------------
// * static functions
// *---------------------------------------------
static LSA_INT32   Task_Pnpb (void);



// *---------------------------------------------
// * public functions
// *---------------------------------------------

    void pnpb_varinit (void)
    {
        // *** init pnpb instance data ***
        OsMemSet ((void*) &Pnpb, 0, sizeof (Pnpb));
        pPnpbIf = NULL;
    }


    void pnpb_init (void)
    {
        PNIO_UINT32 Status;

        // *** init pnpb peri interface ***
        pnpb_peri_init ();
        pnpb_sub_exp_init();

        #if IOD_INCLUDE_REC8028_8029
            pnpb_io_init();
        #endif

        //TrcDkInit();

        pnbp_StartTaskPost();

        // *-----------------------------------------------------------
        // * create task PNPB
        // *-----------------------------------------------------------
        // *** create and start Task for EDD high priority context, add message queue to task ***
        Status = OsCreateThread ((void(*)(void))Task_Pnpb, (PNIO_UINT8*)"Pnio_Pnpb", TASK_PRIO_PNPB, &TskId_Pnpb);
        PNIO_APP_ASSERT(Status == PNIO_OK, "ERROR: OsCreateThread\n");
        Status = OsCreateMsgQueue (TskId_Pnpb);  // install the task message queue
        PNIO_APP_ASSERT(Status == PNIO_OK, "ERROR: OsCreateMsgQueue\n");
        Status = OsStartThread (TskId_Pnpb);
        PNIO_APP_ASSERT(Status == PNIO_OK, "ERROR: OsStartThread\n");
        Pnpb.State = PNPB_INIT;

        return;
    }


    void pnpb_open (pnpb_sys_parameter_ptr_t parameter_ptr)
    {
        // ***** save memory pointer of coupling interface ****
        pPnpbIf = parameter_ptr->pndvInterface;
        Pnpb.OpenDoneCbf = parameter_ptr->done_cbf;

        // *---------------------------------------
        // * open PNPB
        // *---------------------------------------
        {

            Pnpb.State = PNPB_OPEN;


            // **** start PNDV ****
            pnpb_write_event_to_pndv (  PNDV_EV_TO_PNDV_PNDV_START, // command
                                        0,                          // add value 1
                                        0,
                                        NULL);
            PNPB_TRIGGER_PNDV ();
        }
    }

    // *--------------------------------------------------------
    // *  Task_PNPB
    // *
    // *
    // *--------------------------------------------------------
    PNPB_CODE_FAST  static LSA_INT32  Task_Pnpb (void)
    {
      // Wait until own TaskID has been saved by the father process
      OsWaitOnEnable(); // must be first call in every task

      // *-----------------------------------------------------------
      // * loop forever
      // *-----------------------------------------------------------
      while (1)
      {
    	OsTakeSemB (Pnpb.SemId[SEM_PNPB_PERIF]);
    	Pnpb.SemEventAvailable = 0;

    	if (Pnpb.State >= PNPB_OPEN)
           pnpb_process_service_requests();
      }
      return (0); /*lint !e527 unreachable code */
    }


    PNPB_STATE      pnpb_get_state            (void)
    {
        return (Pnpb.State);
    }

    void pnpb_start (void)
    {
        Pnpb.State = PNPB_OPEN;

        // **** start PNDV ****
        pnpb_write_event_to_pndv (  PNDV_EV_TO_PNDV_PNDV_START, // command
                                    0,                          // add value 1
                                    0,
                                    NULL);
        PNPB_TRIGGER_PNDV ();

        // * ------------------------------------
        // * wait until PNDV has been started
        // * ------------------------------------
        while (pnpb_get_state () < PNPB_PNDV_STARTED)
        {
            OsWait_ms (10);
        }

        return;
    }

    void pnpb_activate (void)
    {
        pnpb_write_event_to_pndv (  PNDV_EV_TO_PNDV_PNDV_AKT, // command
                                    0,                          // add value 1
                                    0,
                                    NULL);
        PNPB_TRIGGER_PNDV ();
    }

    void pnpb_deactivate (void)
    {
        pnpb_write_event_to_pndv (  PNDV_EV_TO_PNDV_PNDV_DEAKT, // command
                                    0,                          // add value 1
                                    0,
                                    NULL);
        PNPB_TRIGGER_PNDV ();
    }

    void pnpb_handle_nau_irq (void)
    {
        return;
    }

    void pnpb_close (void)
    {
        // **** stop PNDV ****
        pnpb_write_event_to_pndv (  PNDV_EV_TO_PNDV_PNDV_STOP,  // command
                                        0,                          // add value 1
                                        0,
                                        NULL);

        PNPB_TRIGGER_PNDV ();

        return;
    }

    PNIO_UINT32 pnpb_set_dev_state (PNIO_UINT32 DevHndl,   // device handle
									PNIO_UINT32 DevState)  // device state CLEAR/OPERATE
    {
        PNIO_UINT8 add1 = 0;
        switch (DevState)
        {

            case PNIO_DEVSTAT_OPERATE:
                {
                    add1 = PNDV_PERI_STATE_OK;
                }
                break;
            case PNIO_DEVSTAT_CLEAR:
                {
                    add1 = PNDV_PERI_STATE_NOT_OK;
                }
                break;
            default:
                return (PNIO_NOT_OK);
        }

        PnpbReqSync    (PNDV_EV_TO_PNDV_PERI_STATE_IND,
                        add1,
                        0,
                        Pnpb.SemId[SEM_PNPB_START],
                        (PNIO_CBF_CMD_CONF) LSA_NULL);

        return (PNIO_OK);
    }


    // *----------------------------------------------------------------*
    // *
    // *  pnpb_device_ar_abort (...)
    // *
    // *----------------------------------------------------------------*
    // *  kills the specified AR
    // *  to hand over all the stored PDEV-record data to the stack.
    // *
    // *  Input:	   PNIO_UINT32          ArNum,		    // [in]  AR number (1...N)
    // *
    // *  Output:	   return               PNIO_OK, PNIO_NOT_OK
    // *
    // *----------------------------------------------------------------*
    PNIO_UINT32     pnpb_device_ar_abort (PNIO_UINT32 ArNum)
    {

        // *---------------------------------------------
        // *  write command to perif and trigger PNDV
        // *---------------------------------------------
        pnpb_write_event_to_pndv (PNDV_EV_TO_PNDV_AR_ABORT_REQ,
                                  ArNum - 1,
                                  0,
                                  NULL);
        PNPB_TRIGGER_PNDV ();
        return (PNIO_OK);
    }


    // *----------------------------------------------------------------*
    // *
    // *  pnpb_restore_rema_mem (...)
    // *
    // *----------------------------------------------------------------*
    // *  This function is called from the application at system startup,
    // *  to hand over all the stored PDEV-record data to the stack.
    // *
    // *  Input:	   PNIO_UINT32          DevHndl,		// [in]  Device Handle
    // *			   PNIO_UINT32,         MemSize         // size of the shadow memory
    // *			   PNIO_UINT8*	        pMem            // record data
    // *
    // *  Output:	   return               PNIO_OK, PNIO_NOT_OK
    // *
    // *
    // *----------------------------------------------------------------*
    PNIO_UINT32 pnpb_restore_rema_mem(PNIO_UINT32 MemSize, PNIO_UINT8* pMem)
    {
    	return (PNIO_OK);
    }

    // *----------------------------------------------------------------*
    // *  PnpbLsasFatalError (...)
    // *----------------------------------------------------------------*
    PNIO_VOID   PnpbLsasFatalError (LSA_FATAL_ERROR_TYPE* pLsaErr)
    {
        PNPB_ASSERT(pLsaErr != NULL);

        if(pLsaErr != NULL)
        {
            PNPB_PLATFORM_TRACE_09(LSA_TRACE_LEVEL_FATAL,
							  "Comp=%d Mod=%d L=%d ErrLen=%d EC1=%d EC2=%d EC3=%d EC4=%d pErr=0x%x\n",
							   pLsaErr->lsa_component_id,
							   pLsaErr->module_id,
							   pLsaErr->line,
							   pLsaErr->error_data_length,
							   pLsaErr->error_code[0],
							   pLsaErr->error_code[1],
							   pLsaErr->error_code[2],
							   pLsaErr->error_code[3],
							   pLsaErr->error_data_ptr);

            PNIO_Log (  PNIO_SINGLE_DEVICE_HNDL,
						PNIO_LOG_ERROR_FATAL,
						pLsaErr->lsa_component_id,
						pLsaErr->module_id,
						pLsaErr->line);
        }
    }


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
