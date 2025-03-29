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
/*  F i l e               &F: posix_thread.c                            :F&  */
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
 * @file     posix_print.c
 * @brief    functions for thread handling
 *
 * interface for thread handling
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

   /**
    * @brief hold code execution in created thread during startup
    *
    * @param[in]    PNIO_UINT32     priority for thread
    *
    * @return       PNIO_UINT32     LSA_RET_OK      priority is set for this thread
    */
   PNIO_UINT32  OsWaitOnEnable ( void )
   {
        PNIO_UINT32 ThisThreadId   = INVALID_TASK_ID;	// save own task ID

        OS_ASSERT (OsInitialized == PNIO_TRUE);

        // Wait until own TaskID has been saved by the father process

        while (ThisThreadId  == INVALID_TASK_ID)
        {
	        PNIO_UINT32 i;
            PNIO_UINT32 PosixThreadId;
            PosixThreadId = pthread_self ();
            for (i = 1; i < MAXNUM_OF_TASKS; i++)
            {
              if (TaskProp[i].PosixThreadId == PosixThreadId)
              {
	            ThisThreadId = i;
                break;
              }
            }
            if (ThisThreadId  == INVALID_TASK_ID)
                OsWait_ms (4);
        }

        // wait until task blocking is deactivated by the creating task
        while (TaskProp[ThisThreadId].State != TASK_ENABLED)
        {
        	OsWait_ms (1);
        }

        TaskProp[ThisThreadId].State = TASK_OPERATING;    // Task is now operating

        return (PNIO_OK);
    }

   /**
    * @brief Set scheduler for main thread of application
    *
    * @param[in]    PNIO_UINT32     priority for thread
    * @return       PNIO_UINT32     LSA_RET_OK      priority is set for this thread
    */
   PNIO_UINT32 OsSetMainThreadId ( PNIO_UINT32 priority )
   	{

        struct sched_param sp = { .sched_priority = priority };
        //lint -e{578} Declaration of symbol 'Symbol' hides symbol 'Symbol' (Location)
        if ( sched_setscheduler(0, SCHED_RR, &sp) == -1 )
        {
            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "sched_setscheduler failed :%d\n\n", errno);
        }

        return (LSA_RET_OK);
   	}

   /**
    * @brief Send signal SIGINT to all running thread
    *
    * @return  void
    */
    void  OsThreadIdBacktraces (void)
    {
    	PNIO_INT32 i;

        for (i = 1; i < MAXNUM_OF_TASKS; i++)
        {
        	if (TaskProp[i].Occupied == PNIO_TRUE)
        	{
        		if ( pthread_kill(TaskProp[i].PosixThreadId, SIGINT ) != 0 )
        		{
        		    LSA_TRACE_02  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_NOTE, "pthread_kill failed for %d with %d\n\n", i, errno);
        		}
        	}
        }
    }

    /**
     * @brief Get thread id of thread from which function is called
     *
     * @return  PNIO_UINT32         thread's ID
     */
    PNIO_UINT32  OsGetThreadId (void)
    {
       PNIO_INT32 i;
       pthread_t PosixThreadId;


       OS_ASSERT (OsInitialized == PNIO_TRUE);

       PosixThreadId = pthread_self ();


       for (i = 1; i < MAXNUM_OF_TASKS; i++)
       {
          if (TaskProp[i].PosixThreadId == PosixThreadId)
	        return (i);
       }

       LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "OsGetThread Failed\n" );

       return (INVALID_TASK_ID);
    }

    /**
     * @brief Get thread name of thread
     *
     * @param[in]   PNIO_UINT32         [in] identifier of thread
     *
     * @return  PNIO_UINT8*         pointer to thread's name
     */
    PNIO_UINT8*  OsGetThreadName (PNIO_UINT32 NativeThreadId )
    {
       PNIO_UINT8 *pName = LSA_NULL;

       OS_ASSERT (OsInitialized == PNIO_TRUE);

       if (NativeThreadId < MAXNUM_OF_TASKS )
    	   pName = (PNIO_UINT8*) &TaskProp[NativeThreadId].TaskName[0];

       return pName;
    }

    /**
     * @brief Create thread
     *
     * @param[in]   void (* ) (void)    [in]  thread body
     * @param[in]   unsigned char *     [in]  pointer to thread name
     * @param[in]   PNIO_UINT32         [in]  priority of thread
     * @param[in]   PNIO_UINT32 *       [out] identifier of thread
     *
     * @return  PNIO_UINT32         LSA_RET_OK      thread is running
     */
    PNIO_UINT32  OsCreateThread ( void  (*TaskEntry)(void),unsigned char *pTaskName,PNIO_UINT32  TaskPrio,PNIO_UINT32* pTaskId )

      {

        int ret_val;
        struct sched_param th_param; // Scheduling parameters
        pthread_attr_t th_attr; // Attributes

        PNIO_INT32 i;
        PNIO_INT32 Index = INVALID;

        OS_ASSERT (OsInitialized == PNIO_TRUE);
        for (i = 1; i < MAXNUM_OF_TASKS; i++)		// we dont use index 0 here
        {
            OsEnter();
            if (TaskProp[i].Occupied == PNIO_FALSE)	// is it a free entry ??
            { // yes, now occupy it
                TaskProp[i].Occupied = PNIO_TRUE;		// occupie entry
                Index = i;
                OsExit();
                break;
            }
            OsExit();
        }

        if (Index == INVALID)
        { // ** error: no more task can be created **
            LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "Error in OsStartTask()\n");
            return (PNIO_NOT_OK);
        }

        // Init thread attributes
        ret_val = pthread_attr_init(&th_attr);
        if (ret_val != 0)
        {
            LSA_TRACE_02  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "pthread_attr_init %s failed with %d\n",pTaskName, ret_val);
        }
        // Enable explicitly to set individual scheduling parameters
        ret_val = pthread_attr_setinheritsched(&th_attr, PTHREAD_EXPLICIT_SCHED);
        if (ret_val != 0)
        {
            LSA_TRACE_02  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "pthread_attr_setinheritsched %s failed with %d\n",pTaskName, ret_val);
        }

        ret_val = pthread_attr_setstackaddr(&th_attr,  &TaskStack[Index][TASK_STACKSIZE_DEFAULT]);
        if (ret_val != 0)
        {
            LSA_TRACE_02  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "pthread_attr_setstacksize %s failed with %d\n",pTaskName, ret_val);
        }

        ret_val = pthread_attr_setstacksize(&th_attr, TASK_STACKSIZE_DEFAULT); /* 8 KB Stacksize */
        if (ret_val != 0)
        {
            LSA_TRACE_02  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "pthread_attr_setstacksize %s failed with %d\n",pTaskName, ret_val);
        }

        ret_val = pthread_attr_setscope(&th_attr, PTHREAD_SCOPE_SYSTEM);
        if (ret_val != 0)
        {
            LSA_TRACE_02  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "pthread_attr_setscope %s failed with %d\n",pTaskName, ret_val);
        }

        // Set scheduling policy to RR
        ret_val = pthread_attr_setschedpolicy(&th_attr, SCHED_RR);
        if (ret_val != 0)
        {
            LSA_TRACE_02  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "pthread_attr_setschedpolicy %s failed with %d\n",pTaskName, ret_val);
        }

        th_param.sched_priority = TaskPrio;
        ret_val = pthread_attr_setschedparam(&th_attr, &th_param);
        if (ret_val != 0)
        {
            LSA_TRACE_02  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "pthread_attr_setschedparam %s failed with %d\n",pTaskName, ret_val);
        }

        // Set argument to the task data structure used for creation
        //lint -e{611} Suspicious cast
        ret_val = pthread_create((pthread_t*)&(TaskProp[Index].PosixThreadId),
                                 &th_attr,
                                 (void*) TaskEntry,
                                 (void *) 0);
        if (ret_val != 0)
        {
            LSA_TRACE_02  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "pthread_create %s failed with %d\n",pTaskName, ret_val);
        }

        *pTaskId = Index;      // take the index as a os independent task ID

        OsMemCpy  ( (void*) &(TaskProp[Index].ThreadAttr), (void*) &th_attr, sizeof (th_attr) );
        OsMemCpy  ( (void*) &(TaskProp[Index].TaskName[0]),(void*) &pTaskName[0], MAXSIZE_TASKNAME );

        return (LSA_RET_OK);
     }

    /**
     * @brief Hold code execution to wait for already started thread
     *
     * @param[in]   PNIO_UINT32     id of thread
     *
     * @return  PNIO_UINT32     LSA_RET_OK                      thread is running
     */
    PNIO_UINT32  OsStartThread ( PNIO_UINT32 OsThreadId)
    {

        OS_ASSERT (OsInitialized == PNIO_TRUE);

        TaskProp[OsThreadId].State = TASK_ENABLED;
/*
	    while (TaskProp[OsThreadId].State != TASK_OPERATING)
        {
	        OsWait_ms (4);
        }
*/
        return ( LSA_RET_OK );
    }

#endif


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
