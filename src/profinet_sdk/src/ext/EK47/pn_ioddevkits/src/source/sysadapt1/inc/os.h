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
/*  F i l e               &F: os.h                                      :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  OS abstraction layer interface definition                                */
/*                                                                           */
/*****************************************************************************/
#ifndef _OS_H
#define _OS_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif



// *-----------------------------------------------------------------------
// * includes
// *-----------------------------------------------------------------------
#include "compiler.h"
#include "pniobase.h"
#include "os_cfg.h"
#include "os_utils.h"
#include "iod_cfg2.h"
#include "os_taskprio.h"
#include <string.h>
#include <malloc.h>

/*
 *  Debug purposes
 */
//#define OS_DEBUG_RELEASES   1

// *-----------------------------------------------------------------------
// * defines
// *-----------------------------------------------------------------------
#define INVALID_TASK_ID		0

/*	if (!(cond)) { printf ("Fatal Error File %s, Line %d\n", (char*) _ThisFileName_, __LINE__); } \
*/
  #define OS_ASSERT(_cond_) \
	if (!(_cond_)) { LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_FATAL, "OS_ASSERT"); app_printf ("Fatal Error File %s, Line %d\n", (char*) __FILE__, __LINE__); while (1); }


// Task states
#define TASK_BLOCKED	0
#define TASK_ENABLED	1
#define TASK_OPERATING	2

// ****** dynamic memory Pools *****
#define MEMPOOL_DEFAULT        0
#define MEMPOOL_FAST           1        // maybe cached
#define MEMPOOL_RX_TX_BUF      2        // uncached
#define MEMPOOL_CACHED         3        // maybe cached
#define MEMPOOL_UNCACHED       4        // uncached

#if (PNIOD_PLATFORM & (PNIOD_PLATFORM_EB200P | PNIOD_PLATFORM_RZT2) )
    #define PN_CLOCKS_PER_SEC	1000				// scheduler tick in ticks per second
	#define RX_TX_MEM_SIZE		0x200000			// size for memory based pRxTxMem,  located in XX_MEM
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200_400)
    #define PN_CLOCKS_PER_SEC	1000				// scheduler tick in ticks per second
	#define RX_TX_MEM_SIZE		0x00700000			// size for memory based pRxTxMem,  located in XX_MEM - 7 MB
#elif (PNIOD_PLATFORM &  PNIOD_PLATFORM_DKSW)
    #define PN_CLOCKS_PER_SEC	1000				// scheduler tick in ticks per second
	#define RX_TX_MEM_SIZE	0x200000 //(IOD_CFG_PDEV_NUMOF_PORTS * 0x300000)   // 0x200000 -->     // size for memory based pRxTxMem,  located in XX_MEM
#else
	#error "no valid platform selected"
#endif

// ****** scheduling time *****
#ifndef PN_CLOCKS_PER_SEC
   #define BSP_TICKS_PER_SECOND	                PN_CLOCKS_PER_SEC
#endif

#define OS_PRINT_POST_QUEUE_DISABLE             500
#define OS_PRINT_POST_QUEUE_ENABLE              400

// * --------------------------------------------------------------------
// * to equalize network load peaks in case of shared device,
// * in this case EDD_CFG_CYCLE_BASE_FACTOR = 32 is approvable
// * --------------------------------------------------------------------
#if ((PNIOD_PLATFORM &  PNIOD_PLATFORM_DKSW) && \
     (EDD_CFG_CYCLE_BASE_FACTOR != 32) && (IOD_CFG_NUMOF_IO_AR != 1))
    #error "only EDD_CFG_CYCLE_BASE_FACTOR = 32 is approvable, if shared device is used"
#endif

#if (PNIOD_PLATFORM  &  PNIOD_PLATFORM_LINUX_PC)
PNIO_VOID   edds_timeout( PNIO_UINT16 timer_id, PNIO_UINT32  user_id);
#endif

#define OS_WAIT_FOR_OBJ	0xFFFFFFFFUL
#define OS_MSG_MAX_PRM	5U
typedef struct _OS_MSG_TYPE
{
	PNIO_UINT16  MsgId;
	PNIO_BOOL  	 bMsgFromIsr;
	PNIO_UINT32  DataSize;
	PNIO_VOID    *pvData;
}OS_MSG_TYPE, *OS_MSG_PTR_TYPE;

    // *------------------------------------------------------------------------------
    // *	os interface startup
    // *------------------------------------------------------------------------------
    PNIO_UINT32 OsInit (void);
    extern void MainAppl (void);
    extern PNIO_UINT32 g_nPnsMainTaskId;
    // *------------------------------------------------------------------------------
    // *	memory management
    // *------------------------------------------------------------------------------
    PNIO_UINT32 OsAllocF (void**  		ppMem,PNIO_UINT32	Length);
    PNIO_UINT32 OsAllocFX (void**  		ppMem,PNIO_UINT32	Length,PNIO_UINT32  PoolId);
    PNIO_UINT32 OsAlloc (void**		pMem,PNIO_UINT8	Value,PNIO_UINT32	Length);
    PNIO_UINT32 OsAllocX (void       **pMem, PNIO_UINT8	Value, PNIO_UINT32	Length,PNIO_UINT32 PoolId);
    PNIO_UINT32 OsFree (void* pMem);
    PNIO_UINT32 OsFreeX ( void           *pMem, PNIO_UINT32   PoolId);

    // *------------------------------------------------------------------------------
    // *	time management
    // *------------------------------------------------------------------------------
    PNIO_UINT32  OsAllocTimer (	PNIO_UINT16		*timer_id_ptr, PNIO_UINT16		timer_type, PNIO_UINT16		time_base,PNIO_VOID		*callback_timeout);
    PNIO_UINT32  OsStartTimer (	PNIO_UINT16		timer_id,PNIO_UINT32		user_id,PNIO_UINT16		delay	);
    PNIO_UINT32  OsStopTimer  (	PNIO_UINT16		timer_id );
    PNIO_UINT32  OsFreeTimer  (	PNIO_UINT16 timer_id );
    void         OsWait_ms    ( PNIO_UINT32 PauseTimeInterval_ms );
    void         OsWait_us    ( PNIO_UINT32 PauseTimeInterval_us );
    PNIO_UINT32  OsGetTime_us  ( void );
    PNIO_UINT32  OsGetTime_ms  ( void );
    PNIO_UINT32  OsGetUnixTime ( void );
    PNIO_UINT64 OsGetUuidTime  ( void );
	PNIO_UINT32 OsResetTimer(PNIO_UINT16 timer_id);
    #if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)
        // ***** optional hardware supported functions ****
        PNIO_UINT32 OsGetTimDiffSinceCycleStart(void);
    #endif

    // *------------------------------------------------------------------------------
    // *	semaphore
    // *------------------------------------------------------------------------------
    PNIO_UINT32     OsCreateMutex   ( PNIO_VOID * addr );
    void            OsEnterX		( PNIO_UINT32 MutexId );
    void            OsExitX		    ( PNIO_UINT32 MutexId );
    void            OsEnter		    ( void );
    void            OsExit			( void );
    void            OsEnterShort	( void );
    void            OsExitShort	    ( void );

    #if (MAXNUM_OF_BIN_SEMAPH != 0)		// optional
        PNIO_UINT32 OsCreateSem ( PNIO_VOID *pSem, PNIO_BOOL shared, PNIO_UINT32 value );
        PNIO_UINT32 OsAllocSemB ( PNIO_UINT32 *pSemId );
        PNIO_UINT32 OsTakeSemB ( PNIO_UINT32 SemId );
        PNIO_UINT32 OsGiveSemB ( PNIO_UINT32 SemId );
        PNIO_UINT32 OsFreeSemB ( PNIO_UINT32 SemId );
    #endif


    // *------------------------------------------------------------------------------
    // *	thread handling
    // *------------------------------------------------------------------------------
    PNIO_UINT32  OsRegisterThread           ( PNIO_UINT8*  pThreadName,PNIO_UINT32  ThreadPrio,PNIO_UINT32  NativeThreadID,PNIO_UINT32* pThreadId );
    PNIO_UINT32  OsCreateThread             ( void (*pThreadEntry)(void), PNIO_UINT8*  pThreadName, PNIO_UINT32  ThreadPrio, PNIO_UINT32* pThreadId );
    PNIO_UINT32  OsCreateThreadExt          ( void (*pThreadEntry)(void), PNIO_UINT8*  pThreadName, PNIO_UINT32  ThreadPrio, PNIO_UINT32* pThreadId, PNIO_UINT32 dwOptions);
    PNIO_UINT32  OsSetThreadPrio            ( PNIO_UINT32 ThreadId, PNIO_UINT32 NewThreadPrio );
    PNIO_UINT32  OsStartThread              ( PNIO_UINT32 ThreadId );
    PNIO_UINT32  OsWaitOnEnable             ( void );
    PNIO_UINT32  OsGetThreadId              ( void );

    // *------------------------------------------------------------------------------
    // *	messages
    // *------------------------------------------------------------------------------
    void        OsMessageQueuesInit         ( void );
    PNIO_UINT32	OsCreateMsgQueue            (PNIO_UINT32 ThreadId);
// $$$BJ why return INT32 instead of UINT32 in OsGetNumOfMessages ?
    PNIO_INT32  OsGetNumOfMessages          (PNIO_UINT32 TaskId);
    PNIO_UINT32 OsReadMessageBlocked        (void** ppMessage, PNIO_UINT32 ThreadId);
    PNIO_UINT32 OsReadMessageBlockedX       (void** ppMessage1, void** ppMessage2, PNIO_UINT32 ThreadId);
    PNIO_UINT32 OsSendMessage               (PNIO_UINT32 ThreadId,void* pMessage,PNIO_UINT32 MsgPrio);
    PNIO_UINT32 OsSendMessageToFront        (PNIO_UINT32  TaskId, void* pMessage, PNIO_UINT32  MsgPrio);
    PNIO_UINT32 OsSendMessageX              (PNIO_UINT32 ThreadId,void* pMessage1,void* pMessage2,PNIO_UINT32 MsgPrio);

    PNIO_UINT32 OsSendMsgExt(PNIO_UINT32 ThreadId, PNIO_UINT16 MsgId, PNIO_VOID *pvData, PNIO_UINT16 DataSize);
    PNIO_UINT32 OsSendMsgPrm(PNIO_UINT32 ThreadId, PNIO_UINT16 MsgId, PNIO_UINT32 p1, PNIO_UINT32 p2, PNIO_UINT32 p3, PNIO_UINT32 p4, PNIO_UINT32 p5);
    PNIO_VOID*  OsReadMsgExt(PNIO_UINT32 ThreadId, PNIO_UINT32 Timeout);
    PNIO_UINT32 OsSendMsgFromISR(PNIO_UINT32 ThreadId, PNIO_VOID *pvData);
    PNIO_VOID   OsFreeMsgExt(PNIO_VOID *pMem);

#define OsSendMsgPrm1(_TID, _MID, _P1)						OsSendMsgPrm(_TID, _MID, _P1,  0U,  0U,  0U,  0U)
#define OsSendMsgPrm2(_TID, _MID, _P1, _P2)					OsSendMsgPrm(_TID, _MID, _P1, _P2,  0U,  0U,  0U)
#define OsSendMsgPrm3(_TID, _MID, _P1, _P2, _P3)			OsSendMsgPrm(_TID, _MID, _P1, _P2, _P3,  0U,  0U)
#define OsSendMsgPrm4(_TID, _MID, _P1, _P2, _P3, _P4)		OsSendMsgPrm(_TID, _MID, _P1, _P2, _P3, _P4,  0U)
#define OsSendMsgPrm5(_TID, _MID, _P1, _P2, _P3, _P4, _P5)	OsSendMsgPrm(_TID, _MID, _P1, _P2, _P3, _P4, _P5)


    PNIO_VOID*  OsCreateQue(PNIO_UINT16 Items, PNIO_UINT16 ItemSize);
	PNIO_UINT32 OsQueRecv(PNIO_VOID *pQue, PNIO_UINT32 Timeout);
	PNIO_UINT32 OsQueSend(PNIO_VOID *pQue, PNIO_UINT32 Item, PNIO_UINT32 Timeout);
	PNIO_UINT32 OsQueSendFromISR(PNIO_VOID *pQue, PNIO_UINT32 Item);
	PNIO_UINT32 OsQueSendToFront(PNIO_VOID *pQue, PNIO_UINT32 Item, PNIO_UINT32 Timeout);

    PNIO_UINT32 OsTaskNotifyWait(PNIO_UINT32 TaskId);
    PNIO_UINT32 OsTaskNotifyTake(PNIO_UINT32 TaskId);
    PNIO_VOID OsTaskNotifyGiveFromISR(PNIO_UINT32 TaskId);
    PNIO_VOID OsTaskNotify(PNIO_UINT32 TaskId, PNIO_UINT32 u32Event);
    PNIO_VOID OsTaskNotifyFromISR(PNIO_UINT32 TaskId, PNIO_UINT32 u32Event);
    PNIO_VOID* OsTaskGetHandle(PNIO_UINT32 TaskId);
 //   PNIO_VOID* OsCreateQue()
    void        OsIntDisable (void);
    void        OsIntEnable  (void);
    void        OsEnterCritical (void);
    void        OsExitCritical  (void);

    PNIO_UINT32 OsHtonl(PNIO_UINT32 val32);
    PNIO_UINT16 OsHtons(PNIO_UINT16 val16);
    PNIO_UINT32 OsNtohl(PNIO_UINT32 val32);
    PNIO_UINT16 OsNtohs(PNIO_UINT16 val16);

    /* string.h */
    PNIO_VOID * OsMemCpy(PNIO_VOID * pDst, const PNIO_VOID * pSrc, PNIO_UINT32 size);
    PNIO_VOID * OsMemMove(PNIO_VOID * pDst, const PNIO_VOID * pSrc, PNIO_UINT32 size);
    PNIO_VOID * OsMemSet(PNIO_VOID * pDst,PNIO_INT val,PNIO_UINT32 size);
    PNIO_INT    OsMemCmp(const PNIO_VOID * pBuf1,const PNIO_VOID * pBuf2, PNIO_UINT32 size);
    PNIO_INT    OsStrCmp(const PNIO_INT8 * pBuf1, const PNIO_INT8 *pBuf2);
    PNIO_INT    OsStrnCmp(const PNIO_INT8 *pBuf1, const PNIO_INT8 *pBuf2, PNIO_UINT32 size);
    PNIO_INT8 * OsStrCpy(PNIO_INT8 *pDst,const PNIO_INT8 *pSrc);
    PNIO_INT8 * OsStrnCpy(PNIO_INT8 *pDst,const PNIO_INT8 *pSrc, PNIO_UINT32 size);
    PNIO_UINT32 OsStrLen(const PNIO_INT8 *pBuf);

    /* stdlib.h */
    PNIO_INT    OsRand();
    PNIO_VOID   OsSrand(PNIO_UINT val);
    PNIO_INT    OsAtoi(const PNIO_INT8 *pStr);
    PNIO_INT8 * OsStrChr(const PNIO_INT8 *pBuf, PNIO_INT c);

    PNIO_VOID*  OsTestPointer(void *ptr);

    PNIO_UINT8  OsGetChar (void);
    PNIO_UINT32 OsKeyScan32 (PNIO_INT8* pText, PNIO_UINT32 InputBase);
    PNIO_UINT32 OsKeyScanString ( PNIO_INT8* pText, PNIO_UINT8* pStr, PNIO_UINT32 MaxLen );

    // *------------------------------------------------------------------------------
    // *	other functions
    // *------------------------------------------------------------------------------
    void        OsReboot           (void);
    void        OsRebootService    (void);

	PNIO_UINT32 OsStoreTraceBuffer( const char * pBlock1Name, void* pBlock1Start, PNIO_UINT32 Block1Size,
	                                const char * pBlock2Name, void* pBlock2Start, PNIO_UINT32 Block2Size);

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS)
	//#define  ECOS_KERNEL_INSTRUMENTATION

    #ifdef  ECOS_KERNEL_INSTRUMENTATION
		#include <pkgconf/kernel.h>
		#include <cyg/kernel/instrmnt.h>

		#ifdef CYGDBG_KERNEL_INSTRUMENT_USER
			#define OS_INSTRUMENT_USER(_event_,_arg1_,_arg2_)	OsIntrumentUserEvent(_event_,_arg1_,_arg2_)
			#define OS_INSTRUMENT_USER_START(_arg1_,_arg2_) 	OsIntrumentUserStart(_arg1_,_arg2_)
			#define OS_INSTRUMENT_USER_STOP(_arg1_,_arg2_) 		OsIntrumentUserStop(_arg1_,_arg2_)
        #else
			#define OS_INSTRUMENT_USER(_event_,_arg1_,_arg2_)
			#define OS_INSTRUMENT_USER_START(_arg1_,_arg2_)
			#define OS_INSTRUMENT_USER_STOP(_arg1_,_arg2_)
		#endif
	#else
		#define OS_INSTRUMENT_USER(_event_,_arg1_,_arg2_)
		#define OS_INSTRUMENT_USER_START(_arg1_,_arg2_)
		#define OS_INSTRUMENT_USER_STOP(_arg1_,_arg2_)
    #endif
#else
	#define OS_INSTRUMENT_USER(_event_,_arg1_,_arg2_)
	#define OS_INSTRUMENT_USER_START(_arg1_,_arg2_)
	#define OS_INSTRUMENT_USER_STOP(_arg1_,_arg2_)
#endif


#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
