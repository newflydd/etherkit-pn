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
/*  F i l e               &F: ecos_os.c                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  OS abstraction layer implementation for ECOS                             */
/*                                                                           */
/*****************************************************************************/

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

// *---------------------------------------------------------------------
// *    OS system adation layer
// *
// *
// *
// *
// *
// *---------------------------------------------------------------------
#define PRINT_PNIOD_PLATFORM
#define USE_DEF_VA_LIST				// must be defined before include

#define PNIOUSR_LOWER_RQB_TYPE          EDD_RQB_TYPE

#define RS232_PRINT_POSTTHREAD      1   // 1: print messages in a separate low prio thread 0: print directly and wait until finish


	/**************************************************************************
	 * external functions
	 */
extern PNIO_UINT32  Bsp_nv_data_init (PNIO_VOID* nvDataInit, PNIO_UINT32 nvDataLen);
extern void app_printf(char* format, ...);

	/**************************************************************************/
//#define TICKS_TO_MS(Ticks)  0
#define MS_TO_TICKS  pdMS_TO_TICKS
#define OS_MSGX_ID               0x3829acdc // any unique tag, to identify OsSendMessageX-Telegram

extern PNIO_UINT32 TskId_OsTimer;
extern PNIO_UINT32 OsInitialized;

    // *-------------------------------------------------------------
    // * debugging Macros
    // *-------------------------------------------------------------
    #define MBX_WAIT_IF_FULL_QUEUE  0
    #define LOG_MEM_ALLOCS          0       // log memory alloc/free
    #define LOG_MSG_SND_REC         0       // test only: log numof send/recv. messages
    #define DEBUG_TIMCNT            0
    #define RS232_BUF_SIZE          512     // buffer size for RS232 messages

    extern PNIO_UINT32     PnioLogDest; //  1 --> 0   // debug logging   0:none, 1:Console, 2:Tracebuffer


    // *-------DEBUG--------
    extern PNIO_UINT32 os_log_level;
    #define OS_LOG_FCT(fn_) static char* _os_log_fct = fn_;

    #define OS_DEBUG		0		// != 0: debug infos for OS are stored

    // *-----------------------------------------------------
    // * defines
    // *-----------------------------------------------------
    #define INVALID	  0xffff

#define TASK_STATE_NONE     	0
#define TASK_STATE_BLOCKED     	1
#define TASK_STATE_ENABLED		2
#define TASK_STATE_OPERATING	3
#define TASK_STATE_EXIT			4
    // *---------------------------------*
    // * task list incl. message queues
    // *---------------------------------*
    typedef struct _TASK_PROPERTIES
    {
      PNIO_BOOL		Occupied;			// valid, if value = 1, 0: invalid
      PNIO_UINT32	State;		// Task state (BLOCKED/ENABLED/OPERATING)
      PNIO_UINT32	MsgQueueId;			// ID of the message queue
      PNIO_UINT32	MsgQueueFillStat;	// fill level indicator
      PNIO_UINT32	MsgQueueMaxFillStat;// maximum value of fill level (for debug statistics only)
      PNIO_UINT32	MsgQueueNumOfSnd;	// number of sent messages
      PNIO_UINT32	MsgQueueNumOfRec;	// number of received messages
      PNIO_UINT32	MsgQueueErrCnt;		// counter for message queue error
      PNIO_VOID*	pMsgQueueMem;       // pointer to the memalloc source data block
      PNIO_VOID*	pTaskStack;			// pointer to start of the task-stack
      PNIO_UINT32	StackSize;			// size in byte of the stack
      QueueHandle_t MsgQueue;           /* pointer to a task message queue (identifier) */
      StaticQueue_t  xStaticQueue;
      PNIO_UINT32    xQueBuffer[MAXNUM_MSG_ENTRIES];
      StaticTask_t xTaskBuffer;           /* task id in the system range                  */
      TaskHandle_t hTaskHdl;				/* thread structure */
#if (0 < MAXSIZE_TASKNAME)
      PNIO_UINT8 TaskName[MAXSIZE_TASKNAME];
#endif
      void (*pTaskEntry)(void);
    } TASK_PROPERTIES;


#define TASK_STACK_ITEMS  (TASK_STACKSIZE_DEFAULT/sizeof(StackType_t))
extern TASK_PROPERTIES  TaskProp[MAXNUM_OF_TASKS];
extern StackType_t TaskStack[MAXNUM_OF_TASKS][TASK_STACK_ITEMS];




#define OS_MAX_MSGQ_ITEM	10
#define OS_MAX_MSGQ				3
	typedef struct _OS_MSGQ
	{
		PNIO_UINT32	   xQueueId;			// ID of the message queue
		QueueHandle_t  xQueue;           /* pointer to a task message queue (identifier) */
		StaticQueue_t  xStaticQueue;
		PNIO_UINT32    xQueueBuffer[OS_MAX_MSGQ_ITEM];
	}OS_MSGQ, *OS_MSGQ_PTR;
	extern OS_MSGQ g_tOsMsgQue[OS_MAX_MSGQ];
    // *---------------------------------*
    // * semaphore list
    // *---------------------------------*
    #if (MAXNUM_OF_BIN_SEMAPH != 0)	// optional
	    typedef struct
	    {
	    	SemaphoreHandle_t  Mutex;		// Mutex ID
	    	StaticSemaphore_t xBuffer;
		    PNIO_UINT16			Allocated;	// allocate/free tag
	    } SEM_ALLOC_ENTRY;

        extern SEM_ALLOC_ENTRY	Semaph [MAXNUM_OF_BIN_SEMAPH];		// free mutexes (may be used by dynamically allocate/free)
    #endif


    // *---------------------------------*
    // * mutexes
    // *---------------------------------*
    typedef struct _OS_MUX_TYPE {
    	SemaphoreHandle_t mutex;
    	StaticSemaphore_t xBuffer;
    	PNIO_UINT32 count;
    } OS_MUX_TYPE;

    extern OS_MUX_TYPE NamedMutex[MAXNUM_OF_NAMED_MUTEXES];

    // *---------------------------------*
    // * timer
    // *---------------------------------*
        typedef PNIO_VOID (*OsTimercallback) (PNIO_UINT16 timer_id, PNIO_UINT32 user_id);

    typedef struct _OS_TIMER
    {
        PNIO_BOOL			Occupied;	        // PNIO_TRUE: timer is occupied, PNIO_FALSE: timer is free
        PNIO_BOOL			Running;	        // PNIO_FALSE: timer is not running, PNIO_TRUE: timer is running
        /* PNIO_BOOL		Used;               // Used to meassure usage of SWTimer */
        PNIO_UINT32		    TxTimerId;			// ID of the actual Timer
        OsTimercallback		callback_timeout;
        PNIO_UINT32     	user_id;			// user id, handled to the callback function
        PNIO_UINT32			timer_base;			// timer base 1/10/100 msec
        PNIO_UINT32			timer_typ;
        PNIO_UINT32			TimeVal_ms;			// timer value in msec
        PNIO_UINT32			TimeVal_Ticks;		// timer value in Ticks
        TimerHandle_t		hTimer;				// instance memory for the actual Timer
        StaticTimer_t 		xTimerBuffer;
        PNIO_UINT32         nTickCnt;
    } OS_TIMER;

    extern OS_TIMER         OsTimer[PCPNIO_TIMER_MAX];

    // *---------------------------------*
    // * interrupt locking
    // *---------------------------------*
    /* FreeRTOS does NOT supports recursive int_disable/_enable */
    #define M_INTERR_DISABLE   BSP_INTERRUPT_DISABLE
    #define M_INTERR_ENABLE    BSP_INTERRUPT_ENABLE


    void OsTaskEntry(void *pvParam);


#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
