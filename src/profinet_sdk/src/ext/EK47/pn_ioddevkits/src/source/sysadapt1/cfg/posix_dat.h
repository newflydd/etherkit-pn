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
/*  F i l e               &F: posix_dat.h                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  internal data interface of posix layer                                   */
/*                                                                           */
/*****************************************************************************/


#ifndef PNIO_SRC_SYSADAPT1_CFG_POSIX_DAT_H_
#define PNIO_SRC_SYSADAPT1_CFG_POSIX_DAT_H_

#ifndef LINUX_POSIX_DAT_EXTERN_ATTR
	#define LINUX_POSIX_DAT_EXTERN_ATTR extern
#else
	#define LINUX_POSIX_DAT_EXTERN_ATTR_DECLARE
#endif

    typedef LSA_VOID (*OS_TIMER_CBF) (LSA_TIMER_ID_TYPE, LSA_USER_ID_TYPE);

    typedef enum TimerStatus
    {
      TS_FREE = 0,
      TS_RUNNING,
      TS_STOPPED
    }TimerStatus;

    typedef struct
    {
        PNIO_UINT32    NumPoolDefault;
        PNIO_UINT32    NumPoolFast;
        PNIO_UINT32    NumPoolUncached;
        PNIO_UINT32    NumPoolCached;
        PNIO_UINT32    NumPoolRxTxBuf;

        PNIO_UINT32    SumSize_PoolDefault;
        PNIO_UINT32    SumSize_PoolFast;
        PNIO_UINT32    SumSize_PoolUncached;
        PNIO_UINT32    SumSize_PoolCached;
        PNIO_UINT32    SumSize_PoolRxTxBuf;
        PNIO_UINT32    MaxBlockLen;
        PNIO_VOID*     MaxBlockPtr;
    } MEMPOOL_STATISTICS;

	struct
    {
        PNIO_UINT32 Waiting;
        PNIO_UINT32 ThisTskGotIt;
        PNIO_UINT32 ThisTskLast;
        PNIO_UINT32 EnterCnt;
    }  OsDeb;

    typedef struct
    {
      LSA_BOOL				Occupied;			// valid, if value = 1, 0: invalid
      PNIO_UINT32 volatile   State;				// Task state (BLOCKED/ENABLED/OPERATING)
      pthread_t 		    PosixThreadId;			// task id in the system range
	  PNIO_UINT32			TraceSubsys;
      unsigned char			TaskName[MAXSIZE_TASKNAME];
      pthread_attr_t        ThreadAttr;
    } TASK_PROPERTIES;

	typedef struct
	{
	    pthread_mutex_t		    Id;	        // event handle from the operating system
	    unsigned int            num;
	} OS_MUTEX;

	typedef struct
	{
		sem_t				SemHndl;		// event handle from the operating system
		PNIO_UINT16			Allocated;	// allocate/free tag
	} SEM_ALLOC_ENTRY;

    typedef struct
    {
        PNIO_UINT32  UsrMemSize;
        PNIO_UINT32  PoolId;
        PNIO_UINT32  MemAddr;
        PNIO_UINT32  reserve;   // align reserve, header-length = 16 bytes
    }   OSMEM_STRUC_HEADER;

    typedef struct
    {
        OSMEM_STRUC_HEADER  hdr;
        PNIO_UINT8  body[1];
    }   OSMEM_STRUC;

    typedef struct {
        PNIO_BOOL			Occupied;	        // PNIO_TRUE: timer is occupied, PNIO_FALSE: timer is free
        PNIO_BOOL			Running;	        // PNIO_FALSE: timer is not running, PNIO_TRUE: timer is running

        PNIO_VOID			(*callback_timeout) (PNIO_UINT16 timer_id,
											     PNIO_UINT32 user_id);
	    PNIO_UINT16			timer_base;
	    PNIO_UINT16			timer_typ;
	    PNIO_UINT32     	user_id;
	    PNIO_INT32			elapse;             // remain of time until elapse
	    PNIO_INT32			TimeVal_ms;         // timer preload value in msec
	    PNIO_UINT32         stat_start_time;
	    PNIO_UINT32         stat_time;
	    PNIO_UINT32         stat_count;
    } OS_TIMER;

    typedef struct {
    	mqd_t 		id;
    	PNIO_INT8 	*name;
	} MESSAGE_PROPERTIES;

	#if DEBUG_TASKCNT
    LINUX_POSIX_DAT_EXTERN_ATTR PNIO_UINT32 						TaskCnt[MAXNUM_OF_TASKS];  			// for debugging only,  can be deleted
	#endif
        LINUX_POSIX_DAT_EXTERN_ATTR PNIO_UINT32                     TskId_PrintPost;
        LINUX_POSIX_DAT_EXTERN_ATTR PNIO_UINT32                     PrintPostQueueSize;
        LINUX_POSIX_DAT_EXTERN_ATTR PNIO_BOOL                       PrintPostQueueEnable;

		LINUX_POSIX_DAT_EXTERN_ATTR MEMPOOL_STATISTICS  			MemStat;
		LINUX_POSIX_DAT_EXTERN_ATTR PNIO_UINT32         			TskId_OsTimer;
		LINUX_POSIX_DAT_EXTERN_ATTR PNIO_UINT32                     TskId_AsyncTimer;
		LINUX_POSIX_DAT_EXTERN_ATTR PNIO_UINT32         			Cnt_OsAllocX;    				// counter for number of calls for OsAllocX, OsFree
		LINUX_POSIX_DAT_EXTERN_ATTR PNIO_INT32         		    	SchedPriorityMin;
		LINUX_POSIX_DAT_EXTERN_ATTR PNIO_INT32         	     		SchedPriorityMax;

		LINUX_POSIX_DAT_EXTERN_ATTR PNIO_UINT32     				OsInitialized;

		LINUX_POSIX_DAT_EXTERN_ATTR OS_BSS_FAST TASK_PROPERTIES  				TaskProp[MAXNUM_OF_TASKS];
		LINUX_POSIX_DAT_EXTERN_ATTR OS_BSS_FAST OS_MUTEX	  					StatMutex[MAXNUM_OF_NAMED_MUTEXES];
		LINUX_POSIX_DAT_EXTERN_ATTR OS_BSS_FAST SEM_ALLOC_ENTRY				    Semaph[MAXNUM_OF_POSIX_SEMAPH];
		LINUX_POSIX_DAT_EXTERN_ATTR OS_BSS_FAST MESSAGE_PROPERTIES				MessagePool[MAXNUM_OF_TASKS];
		LINUX_POSIX_DAT_EXTERN_ATTR OS_BSS_FAST OS_TIMER	    				OsTimer[PCPNIO_TIMER_MAX];

		LINUX_POSIX_DAT_EXTERN_ATTR OS_BSS_STACK_FAST __attribute__( ( aligned ( 32 ) ) ) PNIO_UINT8 TaskStack [MAXNUM_OF_TASKS][ OS_CFG_STACK_SIZE ];

		LINUX_POSIX_DAT_EXTERN_ATTR PNIO_UINT32  					TimerStretchFaktor;

		LINUX_POSIX_DAT_EXTERN_ATTR PNIO_INT32 						IntDisable;

		LINUX_POSIX_DAT_EXTERN_ATTR volatile PNIO_CYCLE_INFO_PTR 	pSwiTim[4];				    	// hardwaretimer in ERTEC switch
		LINUX_POSIX_DAT_EXTERN_ATTR PNIO_UINT32     				PnioLogDest;   					// debug logging   0:none, 1:Console, 2:Tracebuffer
		LINUX_POSIX_DAT_EXTERN_ATTR PNIO_UINT32 					SetVal;

		LINUX_POSIX_DAT_EXTERN_ATTR sigset_t                        GlobalSignalMaskSet;

#endif /* PNIO_SRC_SYSADAPT1_CFG_LINUX_POSIX_DAT_H_ */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
