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
/*  F i l e               &F: posix_os.c                                :F&  */
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


/**
 * @file     posix_os.c
 * @brief    Initialization of posix OS interface
 * @author   Miroslav Dusek
 *
 * posix entry point : initialization
 *
 *
 */

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*                                                                           */
/*  Date        Version    What                                              */
/*---------------------------------------------------------------------------*/

#include "compiler.h"

#if (INCLUDE_POSIX_INTERFACE)
	#include "compiler_stdlibs.h"
    #include "os.h"
    #include "os_cfg.h"
    #include "os_taskprio.h"
    #include "pniousrd.h"
	#include "nv_data.h"
	#include "trc_if.h"
	#include "lsa_cfg.h"
	#include "lsa_sys.h"
    #include "tskma_cfg.h"
    #include "logadapt.h"
    #include "usriod_cfg.h"

	#include "posix_os.h"
	#include "posix_dat.h"

#ifdef OS_DEBUG_RELEASES
    #include "tests.h"
#endif

    /**
     * @name posix Initialization of OS interface
     * @{ */

    /**
     * @brief Init of os system adaption posix layer
     *
     *
     * @return         PNIO_UINT32     PNIO_OK         Initialized
     *
     */
    PNIO_UINT32 OsInit (void)
    {
	    PNIO_UINT32 i;
	    PNIO_UINT32 Status = PNIO_OK;

	    Cnt_OsAllocX = 0;
	    OsInitialized = PNIO_FALSE;
	    TimerStretchFaktor = 1;

        OsMemSet ((PNIO_INT8*)&TaskProp,	0, sizeof (TaskProp));	// init memory for task properties

        for (i = 0; i < MAXNUM_OF_TASKS; i++)
        {
	      TaskProp[i].Occupied = LSA_FALSE;
        }

        SchedPriorityMin = sched_get_priority_min (SCHED_RR);
        if (SchedPriorityMin == -1)
        {
            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "sched_get_priority_min failed with %d\n", errno );
        }

        SchedPriorityMax = sched_get_priority_max (SCHED_RR);
        if (SchedPriorityMax == -1)
        {
            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "sched_get_priority_max failed with %d\n", errno );
        }

        if ( ( SchedPriorityMin > TASK_PRIO_IDLE ) && (SchedPriorityMax < TASK_PRIO_APPL_CYCLE_IO ) )
		{
            LSA_TRACE_04  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "sched_priorites bad settings [%d > %d] or [%d < %d]\n", SchedPriorityMin, TASK_PRIO_IDLE, SchedPriorityMax, TASK_PRIO_APPL_CYCLE_IO );
		}

        for (i = 0; i < MAXNUM_OF_NAMED_MUTEXES; i++)
        {
            OsCreateMutex ( ( PNIO_VOID * ) &(StatMutex[i].Id) );
        }

		OsMemSet ((PNIO_INT8*)&Semaph,	0, sizeof (Semaph));

		for (i = 0; i < MAXNUM_OF_POSIX_SEMAPH; i++)
		{
		    OsCreateSem ( (PNIO_VOID *) &Semaph[i].SemHndl, 0, 0 );

		    Semaph[i].Allocated = LSA_FALSE;
		}
#if  _DEBUG_LOGGING_CIRC_BUF
        OsInitCircBuf ();
#endif

        OsMemSet ((void*)&pSwiTim, 0, sizeof (pSwiTim));
        OsMemSet ((void*)&OsTimer, 0, sizeof (OsTimer));

		for (i = 0; i < PCPNIO_TIMER_MAX; i++)
		{
			OsTimer[i].Occupied = PNIO_FALSE;
			OsTimer[i].Running  = PNIO_FALSE;
		}

		OsMessageQueuesInit();

        OsInitialized = PNIO_TRUE;

        OsSetMainThreadId( TASK_PRIO_APPL_CYCLE_IO );

	    srand ( (unsigned) OsGetTime_ms() );

		TskId_PrintPost = 0;
		PrintPostQueueSize = 0;
		PnioLogDest = 1;
		PrintPostQueueEnable = PNIO_TRUE;

		CreateTimerTick ();

        Status = OsCreateThread  (PrintRS232PostTask, (PNIO_UINT8*)"PrintRS232PostTask", TASK_PRIO_PRINT_POST, &TskId_PrintPost);
        PNIO_APP_ASSERT(Status == PNIO_OK, "ERROR: OsCreateThread\n");
        Status = OsCreateMsgQueue (TskId_PrintPost);
        PNIO_APP_ASSERT(Status == PNIO_OK, "ERROR: OsCreateMsgQueue\n");
        Status = OsStartThread     (TskId_PrintPost);
        PNIO_APP_ASSERT(Status == PNIO_OK, "ERROR: OsStartThread\n");

#if(0 == IOD_USED_WITH_XHIF_HOST)
        /* If used with XHIF host, initialization is performed by
         * command sent from host */
        Bsp_nv_data_init (NULL, 0);
#endif

#ifdef OS_DEBUG_RELEASES
        os_meassure_init();
#endif

        return (PNIO_OK);
    }

    /** @} */
#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
