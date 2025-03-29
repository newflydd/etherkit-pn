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
/*  F i l e               &F: posix_timer.c                             :F&  */
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
 * @file     posix_timer.c
 * @brief    functions for time handling
 *
 * interface for time handling
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
   * @brief timer callback thread
   *
   * @return         void
   *
   */
    OS_CODE_FAST void  TaskOsTimer (void)
    {
        void* pVoid = LSA_NULL;
        LSA_UINT32 taskID;
        PNIO_UINT32 timer_id;

        OsWaitOnEnable();

        taskID = OsGetThreadId();

        while (1)
        {
            OsReadMessageBlocked(&pVoid, taskID);

            timer_id = (PNIO_UINT32) pVoid;

            (*OsTimer[timer_id].callback_timeout)((PNIO_UINT16)timer_id, OsTimer[timer_id].user_id);
        }
    }

    /**
     * @brief signal capture of posix 1ms timer
     *
     * @return         void
     *
     */
    OS_CODE_FAST static void TimerHandler ( void )
    {
        timer_t     timerid;
        PNIO_UINT32 timer_id;
        OS_TIMER     *timer;
        struct sigevent sev;
        struct itimerspec its;
        int s, sig, timer_overrun, tmp;

        OsWaitOnEnable();

        sev.sigev_notify = SIGEV_SIGNAL;
        sev.sigev_signo = SIGUSR1;
        sev.sigev_value.sival_ptr = &timerid;
        if ( timer_create(CLOCK_REALTIME, &sev, &timerid) == -1)
        {
            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "timer_getoverrun failed : %d\n", errno);
        }
        /* Start the timer */

        timer_overrun = timer_getoverrun(timerid);
        if ( timer_overrun == -1)
        {
            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "timer_getoverrun failed : %d\n", errno);
        }

        its.it_value.tv_sec = 0;
        its.it_value.tv_nsec = 500000;
        its.it_interval.tv_sec = 0;
        its.it_interval.tv_nsec = 500000;

        if ( timer_settime(timerid, 0, &its, LSA_NULL) == -1 )
        {
            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "timer_settime failed : %d\n", errno);
        }


        while (1)
        {
            s = sigwait(&GlobalSignalMaskSet, &sig);
            if (s != 0)
            {
                LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "sigwait:!failed(%d)\n", s);
            }

            for (timer_id = 0; timer_id < PCPNIO_TIMER_MAX; ++timer_id)
            {
                timer = & OsTimer[timer_id];

                if (timer->timer_typ == 0)
                    continue;

                if (timer->Running == 0)
                    continue;

                timer->elapse += 1;

                if (timer->elapse == timer->TimeVal_ms)
                {
                    //***** call user defined callback function *****
                    OsSendMessage(TskId_OsTimer, (PNIO_VOID*) timer_id, OS_MBX_PRIO_NORM);

                    timer->elapse = 0;

                    // restart
                    if (timer->timer_typ != LSA_TIMER_TYPE_CYCLIC)
                    {
                        OsEnterX(OS_MUTEX_TIMER);
                        timer->Running = 0;
                        OsExitX(OS_MUTEX_TIMER);
                    }
                }
            }

            if ( ( tmp = timer_getoverrun(timerid) ) != timer_overrun )
            {
                if ( tmp == -1)
                {
                    LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "timer_getoverrun failed : %d\n", errno);
                }

                timer_overrun = tmp;
                LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_NOTE, "timer:!overunned(%d)\n", timer_overrun);
            }
        }
    }

    /**
     * @brief create sw timer's routine
     *
     * @RETURN         void
     *
     * two threads -> one is signal capture of timer and send message
     * to second thread which call timer's callback
     */
  void CreateTimerTick (void)
  {
        PNIO_UINT32 Status = PNIO_OK;
        int s;

        sigemptyset(&GlobalSignalMaskSet);
        sigaddset(&GlobalSignalMaskSet, SIGUSR1);
        s = pthread_sigmask(SIG_BLOCK, &GlobalSignalMaskSet, LSA_NULL);
        if (s != 0)
        {
            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "timer_getoverrun failed : %d\n", s);
        }

        Status = OsCreateThread (TimerHandler, ( unsigned char * ) TASKNAME_ASYNC_TIMER, TASK_PRIO_ASYNC_TIMER, &TskId_AsyncTimer);
        PNIO_APP_ASSERT(Status == PNIO_OK, "ERROR: OsCreateThread\n");
        Status = OsStartThread (TskId_AsyncTimer);
        PNIO_APP_ASSERT(Status == PNIO_OK, "ERROR: OsCreateThread\n");

        Status = OsCreateThread (TaskOsTimer, ( unsigned char * ) TASKNAME_OS_TIMER, TASK_PRIO_OS_TIMER, &TskId_OsTimer);
        PNIO_APP_ASSERT(Status == PNIO_OK, "ERROR: OsCreateThread\n");
        Status = OsCreateMsgQueue (TskId_OsTimer);
        PNIO_APP_ASSERT(Status == PNIO_OK, "ERROR: OsCreateMsgQueue\n");
        Status = OsStartThread (TskId_OsTimer);
        PNIO_APP_ASSERT(Status == PNIO_OK, "ERROR: OsCreateThread\n");
  }

  /**
   * @brief allocate a timer
   *
   * @param[in]     timer_id            PNIO_UINT16 *  pointer to id of timer
   * @param[in]     timer_type          PNIO_UINT16    type of timer : LSA_TIMER_TYPE_ONE_SHOT,LSA_TIMER_TYPE_CYCLIC
   * @param[in]     time_base           PNIO_UINT16    time base of timer : LSA_TIME_BASE_1MS,LSA_TIME_BASE_10MS,LSA_TIME_BASE_100MS,LSA_TIME_BASE_1S
   * @param[in]     callback_timeout    PNIO_VOID *    callback function called after timeour of timer
   *
   *
   * @return    PNIO_UINT32         LSA_RET_OK              timer has been allocated
   *                                LSA_RET_ERR_NO_TIMER    no timer has been allocated
   *                                LSA_RET_ERR_PARAM       parameters of timer are wrong ( timer_typ, timer_base, callback_timeout
   */
    PNIO_UINT32  OsAllocTimer(PNIO_UINT16 *timer_id, PNIO_UINT16  timer_typ, PNIO_UINT16  timer_base, PNIO_VOID *callback_timeout)
    {
        PNIO_UINT16 i;
        OS_TIMER *timer;

        OS_ASSERT (OsInitialized == PNIO_TRUE);

        *timer_id = PCPNIO_TIMER_ID_INVALID;

        if (! (timer_base == LSA_TIME_BASE_1MS  ||  timer_base == LSA_TIME_BASE_10MS ||  timer_base == LSA_TIME_BASE_100MS  ||  timer_base == LSA_TIME_BASE_1S))
        {
            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_NOTE, "pcpnio_alloc_timer:!timer_base(%d)\n", timer_base);
            return LSA_RET_ERR_PARAM;
        }

        if (! (timer_typ == LSA_TIMER_TYPE_ONE_SHOT || timer_typ == LSA_TIMER_TYPE_CYCLIC))
        {
            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_NOTE, "pcpnio_alloc_timer:!timer_typ(%d)\n", timer_typ);
            return LSA_RET_ERR_PARAM;
        }

        if (callback_timeout == LSA_NULL )
        {
            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_NOTE, "pcpnio_alloc_timer:!callback(%d)\n", callback_timeout);
            return LSA_RET_ERR_PARAM;
        }

        for (i = 0; i < PCPNIO_TIMER_MAX; ++i)
        {
            timer = & OsTimer[i];

            if (timer->timer_typ == 0)
            {
                timer->timer_base = timer_base;
                timer->callback_timeout	= callback_timeout;
                timer->elapse = 0;
                timer->TimeVal_ms = 0;
                timer->timer_typ = timer_typ;
                timer->Occupied = LSA_TRUE;

                *timer_id = i;
                LSA_TRACE_04   (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_NOTE, "pcpnio_alloc_timer:tid(%d) base(%d) typ(%d) task_id(%d)\n", *timer_id, timer->timer_base, timer->timer_typ, OsGetThreadId() );

                return LSA_RET_OK;
            }
        }

        LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "pcpnio_alloc_timer:no more timer\n");

        return LSA_RET_ERR_NO_TIMER;
    }

    /**
     * @brief start a timer
     *
     * @param[in]          timer_id                   id of timer
     * @param[in]          user_id                    id of prefix
     * @param[in]          delay                      running time in msec
     *
     * @return         PNIO_UINT32      LSA_RET_OK                  timer has been started
     *                                  LSA_RET_OK_TIMER_RESTARTED  timer has been restarted
     *                                  LSA_RET_ERR_PARAM           timer hasn´t been started because of wrong timer-id
     */
    PNIO_UINT32 OsStartTimer (PNIO_UINT16 timer_id, PNIO_UINT32 user_id, PNIO_UINT16 _delay )
    {
        OS_TIMER    *timer;

        PNIO_UINT32 delay = TimerStretchFaktor * _delay;

        OS_ASSERT (OsInitialized == PNIO_TRUE);

        timer = (0 <= timer_id && timer_id < PCPNIO_TIMER_MAX) ? & OsTimer[timer_id] : LSA_NULL;

        if (timer == LSA_NULL  ||  timer->timer_typ == 0)
            return LSA_RET_ERR_PARAM;

        timer->user_id = user_id;

        switch (timer->timer_base)
        {
            case LSA_TIME_BASE_1MS:
                timer->TimeVal_ms = 1 * delay;
                break;
            case LSA_TIME_BASE_10MS:
                timer->TimeVal_ms = 10 * delay;
                break;
            case LSA_TIME_BASE_100MS:
                timer->TimeVal_ms = 100 * delay;
                break;
            case LSA_TIME_BASE_1S:
                timer->TimeVal_ms = 1000 * delay;
                break;
            case LSA_TIME_BASE_10S:
                timer->TimeVal_ms = 10000 * delay;
                break;
            case LSA_TIME_BASE_100S:
                timer->TimeVal_ms = 100000 * delay;
                break;
            default:
                return LSA_RET_ERR_PARAM;
        }

        OsEnterX(OS_MUTEX_TIMER);
        timer->Running = LSA_TRUE;
        OsExitX(OS_MUTEX_TIMER);

        LSA_TRACE_02   (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_NOTE, "pcpnio_start_timer:tid(%d) elapse(%d) mm(%d)\n", timer_id, timer->elapse);

        return LSA_RET_OK;
    }

    /**
     * @brief stops an active timer
     *
     * @param[in]   timer_id        id of timer
     *
     * @return  PNIO_UINT32     LSA_RET_OK                    timer has been stopped
     *                          LSA_RET_OK_TIMER_NOT_RUNNING  timer was not running
     *                          LSA_RET_ERR_PARAM             error occured
     */
    PNIO_UINT32 OsStopTimer ( PNIO_UINT16 timer_id )
    {
        OS_TIMER    *timer;

        OS_ASSERT (OsInitialized == PNIO_TRUE);

        timer = (0 <= timer_id && timer_id < PCPNIO_TIMER_MAX) ? & OsTimer[timer_id] : LSA_NULL;

        if (timer == LSA_NULL  ||  timer->timer_typ == 0)
        {
            LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "tried to stop invalid timer\n");
            return LSA_RET_ERR_PARAM;
        }

        if ( timer->Running == LSA_TRUE )
        {
            OsEnterX(OS_MUTEX_TIMER);
            timer->Running = LSA_FALSE;
            OsExitX(OS_MUTEX_TIMER);
        }

        LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_NOTE, "pcpnio_stop_timer:tid(%d) mm(/%d)\n", timer_id);

        return LSA_RET_OK;
    }

	/**
	 * @brief resets timer value
	 *
	 * @param[in]   timer_id        id of timer
	 *
	 * @return  PNIO_UINT32     LSA_RET_OK                    timer has been reset
	 *                          LSA_RET_ERR_PARAM             error occured
	 *
	 *	Reset can be performed only over timer, which is currently not running.
	 *	Thus, this function should be preceded by timer stop, or it should preceed timer start.
	 */
	PNIO_UINT32 OsResetTimer(PNIO_UINT16 timer_id)
	{
		OS_TIMER    *timer;

		OS_ASSERT(OsInitialized == PNIO_TRUE);

		timer = (0 <= timer_id && timer_id < PCPNIO_TIMER_MAX) ? &OsTimer[timer_id] : LSA_NULL;

		if (timer == LSA_NULL || timer->timer_typ == 0)
		{
			LSA_TRACE_00(TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "tried to reset invalid timer\n");
			return LSA_RET_ERR_PARAM;
		}

		OsEnterX(OS_MUTEX_TIMER);
		if (timer->Running == LSA_FALSE)
		{
			timer->elapse = 0;
			OsExitX(OS_MUTEX_TIMER);
		}
		else
		{
			OsExitX(OS_MUTEX_TIMER);
			LSA_TRACE_01(TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "cannot reset running timer: tid(%d) \n", timer_id);
			return LSA_RET_ERR_PARAM;
		}

		LSA_TRACE_01(TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_NOTE, "pcpnio_reset_timer:tid(%d) \n", timer_id);

		return LSA_RET_OK;
	}

    /**
     * @brief free a timer
     *
     * @param[in]   PNIO_UINT16     id of timer
     *
     * @return  PNIO_UINT32     LSA_RET_OK                      timer has been deallocated
     *                          LSA_RET_ERR_TIMER_IS_RUNNING    because timer is running timer has not been deallocated
     *                          LSA_RET_ERR_PARAM               no deallocation because of wrong timer-id
     */
    PNIO_UINT32   OsFreeTimer (	PNIO_UINT16 timer_id )
    {
        OS_TIMER *timer;

        OS_ASSERT (OsInitialized == PNIO_TRUE);

        timer = (0 <= timer_id && timer_id < PCPNIO_TIMER_MAX) ? & OsTimer[timer_id] : LSA_NULL;

        if (timer == LSA_NULL  ||  timer->timer_typ == 0)
            return LSA_RET_ERR_PARAM;

        if (timer->Running == LSA_TRUE )
            return LSA_RET_ERR_TIMER_IS_RUNNING;

        timer->Occupied = LSA_FALSE;
        timer->timer_typ = 0;

        LSA_TRACE_01   (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_NOTE, "pcpnio_free_timer:tid(%d) mm(%d)\n", timer_id);

        return LSA_RET_OK;
    }

    /**
     * @brief wait for time interval specified in us
     *
     * @param[in] PauseTime_ms PNIO_UINT32  : time interval in which this function will be blocked
     *
     * @return void
     *
     */
    void OsWait_us ( PNIO_UINT32 PauseTimeInterval_us )
    {
        struct timespec Rqtp_us;
        struct timespec Rmtp = {0, 0};

        Rqtp_us.tv_sec   = 0;
        Rqtp_us.tv_nsec  = PauseTimeInterval_us * 1000;

        if (nanosleep (&Rqtp_us, &Rmtp) != 0)
        {
            switch ( errno )
            {
                case EINTR:
                    LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_NOTE, "nanosleep was interrupted by signal \n");
                    break;
                default:
                    LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "Error in nanosleep :%d ()\n", errno );
                    break;
            }
        }
    }

    /**
     * @brief wait for time interval specified in ms
     *
     * @param[in] PauseTime_ms PNIO_UINT32  : time interval in which this function will be blocked
     *
     * @return  void
     *
     */
    void OsWait_ms (PNIO_UINT32 PauseTimeInterval_ms)
    {
      struct timespec Rqtp_ms;
      struct timespec Rmtp = {0, 0};
      //lint -e{578} suppress Declaration of symbol 'OsInitialized' hides symbol 'OsInitialized', extern is used
      if  (OsInitialized != PNIO_TRUE)
           LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "Error in OsWait_ms ()\n");

      Rqtp_ms.tv_sec   = PauseTimeInterval_ms / 1000;
      Rqtp_ms.tv_nsec  = PauseTimeInterval_ms % 1000;
	  Rqtp_ms.tv_nsec  = Rqtp_ms.tv_nsec* 1000000;

      if (nanosleep (&Rqtp_ms, &Rmtp) != 0)
      {
          switch ( errno )
          {
              case EINTR:
                  LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_NOTE, "nanosleep was interrupted by signal \n");
                  break;
              default:
                  LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "Error in nanosleep :%d ()\n", errno );
                  break;
          }
      }
    }

    /**
     * @brief Get time in sec of operation system
     *
     * @return  PNIO_UINT32 - time in usec
     *
     */
    PNIO_UINT32 OsGetTime_us (void)
    {
       PNIO_UINT32 Tim;
       struct timespec t;

       if (OsInitialized != PNIO_TRUE)
           LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "Error in OsGetTime_us ()\n");

       if ( clock_gettime (CLOCK_REALTIME, &t) == -1 )
           LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "Error in clock_gettime :%d ()\n", errno );

       Tim = (PNIO_UINT32) ( ( t.tv_sec * 1000000 ) + ( t.tv_nsec / 1000 ) );

       return (Tim);
    }

    /**
     * @brief Get time in sec of operation system
     *
     * @return  PNIO_UINT32 - time in msec
     *
     */
    PNIO_UINT32 OsGetTime_ms (void)
    {
        PNIO_UINT32 Tim;
        struct timespec t;

       if (OsInitialized != PNIO_TRUE)
           LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "Error in OsGetTime ()\n");

       if ( clock_gettime (CLOCK_REALTIME, &t) == -1 )
           LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "Error in clock_gettime :%d ()\n", errno );

       Tim = (PNIO_UINT32) ( ( t.tv_sec * 1000 ) + ( t.tv_nsec / 1000000 ) );

       return (Tim);
    }

    /**
     * @brief Get time in sec of operation system
     *
     * @return  PNIO_UINT32 - time in sec
     *
     */
    PNIO_UINT32 OsGetUnixTime (void)
    {
       PNIO_UINT32 Tim;
       struct timespec t;

       if (OsInitialized != PNIO_TRUE)
           LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "Error in OsGetUnixTime ()\n");

       if ( clock_gettime (CLOCK_REALTIME, &t) == -1 )
           LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "Error in clock_gettime :%d ()\n", errno );

       Tim = (PNIO_UINT32) ( t.tv_sec );

       return (Tim);
    }

    /**
     * @brief Get time for UUID generation
     *
     * @return PNIO_UINT64 - time in 100us
     */
    PNIO_UINT64 OsGetUuidTime ( void )
    {
        PNIO_UINT64 time_uuid;
        struct timespec t;

        // RFC constant
        static const PNIO_UINT64 num100nsec1582    = 0x01b21dd213814000;

        // get time
        if ( clock_gettime (CLOCK_REALTIME, &t) == -1 )
            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "Error in clock_gettime :%d ()\n", errno );

        // convert to 100us format
        time_uuid = t.tv_sec * 10000000;
        time_uuid = time_uuid + (t.tv_nsec / 100);
        time_uuid = time_uuid + num100nsec1582;

        // return
        return time_uuid;
    }
#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
