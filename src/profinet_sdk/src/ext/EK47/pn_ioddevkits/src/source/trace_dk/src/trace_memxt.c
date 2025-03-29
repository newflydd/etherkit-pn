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
/*  F i l e               &F: trace_memxt.c                             :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Logging Output function into a circular buffer                           */
/*                                                                           */
/*  THIS MODULE HAS TO BE MODIFIED BY THE PNIO USER                          */
/*                                                                           */
/*****************************************************************************/

#include "compiler.h"

#if (PNIO_TRACE == PNIO_TRACE_DK_MEMXT)
    #include "lsas_inc.h"

    #include "os.h"
    #include "pniousrd.h"
    #include "pnio_trace.h"
    #include "trace_memxt.h"
    #include "os_taskprio.h"

    // *------------ defines -----------------*
    #define TRC_MODULE_ID   112


    #define OFFS_MSG2       		62			// column start for message text
    #define TRCBUF_MAX_MSG_LEN      300         // maximum buffer length
    #define BYTES_PER_LINE           16         // number of printed bytes per line in print_trace_array()
    #define  TRCBUF_SYNC_BY_SEM       1         // synchronize circular log buffer and print thread by semaphore

    #define XHIF_MAX_NUM_OF_TRACES   128        // how many traces will be sent in one package to the BBB
    #define XHIF_MAX_NUM_OF_TRACES_B (128*1024) // how many bytes will be sent in one package to the BBB

    // *------------ external functins ----------*
    extern PNIO_UINT32 FlashTraceBuf  (void* pBuf, PNIO_UINT32 BufSize);
	extern PNIO_UINT32 RestoreTraceBuf  (void* pBuf, PNIO_UINT32 BufSize);

    // *------------ static data  ----------*

    // **** format output message ***
    static PNIO_INT32           NextFreeBufInd;                  // first free buffer index(to overwrite with next message)
    static PNIO_INT32           OldestBufInd;                  // oldest buffer index
    static PNIO_INT32           WorkBufInd;                    // index of the first unprocessed message
    static PNIO_UINT32			BufferFill;						//used space in trace buffer

    typedef struct TRACES
    {
    	PNIO_INT32				OldestRecord;
    	PNIO_UINT32				BufferFill;
    	TRACE_MEMXT_ELEM     	TrcBuf[TRCBUF_MAXNUM_ENTRIES]; // trace buffer
    }TRACES;

    TRACES traces;
    static PNIO_BOOL            TrcBufOverflow;

    static PNIO_UINT32          BufSemId;                      // semaphore for print-buffer access
    static PNIO_UINT32          TrcCount      = 0;             // continuous counter, incremented at every
    static PNIO_UINT32          TskId_TrcMemXt = 0;
    static PNIO_UINT32          TrcMemXtSemId  = 0;
    static PNIO_INT8*           GetFilenameStartPoint(PNIO_INT8* pFile) ; 


    // *===========================================================================================================================================
    // *     STARTUP:   initialize trace system
    // *===========================================================================================================================================
    // *----------------------------------------------------------------*
    // *
    // *  Task_TrcMemXt (void)
    // *
    // *----------------------------------------------------------------*
    // *  process the binary trace buffer and expand the message
    // *
    // *----------------------------------------------------------------*
    // *  Input:    ----
    // *  Output:   ----
    // *
    // *----------------------------------------------------------------*
    static PNIO_INT32 Task_TrcMemXt(void)
    {

        // *----------------------------------------------------------
	    // * Synchronization to parent process
	    // *----------------------------------------------------------
	    OsWaitOnEnable();		    // must be first call in every task

	    while (1)
	    {
            #if TRCBUF_SYNC_BY_SEM
                // **** receive a cyclic trigger, to start initiate data rd/wr ***
                OsTakeSemB (TrcMemXtSemId);
            #else
                OsWait_ms (100);
            #endif

            //TrcBufPrintNext (10);
        }
        return (0);
    }


    // *----------------------------------------------------------------*
    // *   TrcInit (BufSize)
    // *----------------------------------------------------------------*
    // *   Inititalizes the Trace Module
    // *
    // *  input :  PNIO_UINT32  Bufsize   (Buffer-size in Bytes,
    // *                                  value must be aligned 4 byte.
    // *  out   :  PNIO_OK, PNIO_NOT_OK
    // *----------------------------------------------------------------*
    PNIO_UINT32     TrcInit	  (PNIO_UINT32 BufSize)
    {
        PNIO_UINT32 Status = PNIO_OK;

        LSA_UNUSED_ARG (BufSize);

        if (Status == PNIO_OK)
            Status = OsAllocSemB (&BufSemId);
        if (Status == PNIO_OK)
            Status = OsGiveSemB (BufSemId); // so the first instance has access without free first


        NextFreeBufInd     = 0;
        OldestBufInd     = 0;
        WorkBufInd       = 0;
        TrcBufOverflow   = PNIO_FALSE;
        BufferFill = 0;

        OsMemSet (traces.TrcBuf, 0, sizeof (traces.TrcBuf));

        #if TRCBUF_SYNC_BY_SEM
	        // *----------------------------------------------------------
	        // * creates semaphore, to trigger the low prio logging task,
            // * that prepares and handles the output string
	        // *----------------------------------------------------------
            OsAllocSemB (&TrcMemXtSemId);
        #endif

	    // *----------------------------------------------------------
	    // * creates the low prio logging task
	    // *----------------------------------------------------------
	    if (Status == PNIO_OK)
		    Status = OsCreateThread ((void  (*)(void))Task_TrcMemXt,
								    (PNIO_UINT8*)"Pnio_TrcMemXt",
								    TASK_PRIO_TRCMEMXT,
								    &TskId_TrcMemXt);	// EDD Task high priority context
	    if (Status == PNIO_OK)
		    Status = OsStartThread (TskId_TrcMemXt);    // start, after the task message queue has been installed


        return (Status);
    }


    // *===========================================================================================================================================
    // *     Realtime part:   store trace entries
    // *===========================================================================================================================================
    #if TRC_INCLUDE_FILENAME
        static void PrepareTrcBuf(TRACE_MEMXT_ELEM* pTrc, void* msg, void* File)
    #else
        static void PrepareTrcBuf(TRACE_MEMXT_ELEM* pTrc, void* msg)
    #endif
    {
        pTrc->TrcCount   = TrcCount;
        TrcCount = (TrcCount > 99999) ? 0 : TrcCount+1;

        pTrc->pFormStr   = msg;
        pTrc->pFile      = (PNIO_INT8*) File;
        pTrc->Type       = TRC_TYPE_PARAM;
        pTrc->TimStampL  =  OsGetTime_ms();
		
	#if TRC_STORE_FILENAME
		PNIO_INT8* p8; //file name pointer
        p8 = GetFilenameStartPoint((PNIO_INT8*)pTrc->pFile);
		OsMemCpy (&(pTrc->File[0]), p8,OsStrLen(p8));
	#endif

        { // limit length of format string to buffer length, if necessary take substitute format string
			PNIO_UINT32 StrLen = OsStrLen (msg);
            if (StrLen >= TRCBUF_MAX_FORMSTR_SIZE)
            {
                msg = "INVALID_FORMAT_STRING \n";
                StrLen = OsStrLen (msg);
            }
            #if TRC_STORE_FORMSTRING
                OsMemSet(pTrc->FormStr,NULL,sizeof(pTrc->FormStr));
                OsMemCpy (&(pTrc->FormStr[0]), msg, StrLen+1);
            #endif
        }


        OsEnterX (OS_MUTEX_LTRC);
        NextFreeBufInd ++;
        BufferFill++;
        if (NextFreeBufInd >= TRCBUF_MAXNUM_ENTRIES)
        {
            NextFreeBufInd = 0;
            TrcBufOverflow = PNIO_TRUE; // set TRUE once (value is never resetted)
        }
        if (TrcBufOverflow == PNIO_TRUE)
        {
            OldestBufInd = NextFreeBufInd;
            BufferFill = TRCBUF_MAXNUM_ENTRIES;
        }
        traces.BufferFill = BufferFill;
        OsExitX (OS_MUTEX_LTRC);

        #if TRCBUF_SYNC_BY_SEM
	        // *----------------------------------------------------------
	        // * creates semaphore, to trigger Task_CycleIO at new Cycle
            // * interrupt
	        // *----------------------------------------------------------
            OsGiveSemB (TrcMemXtSemId);
        #endif

    }


    void dk_trace_00 (
                #if TRC_INCLUDE_FILENAME
                    void* File,
                #endif
                      PNIO_UINT32 ModId, PNIO_UINT32 Line, PNIO_UINT32 subsys, PNIO_UINT32 level,  void* msg)
    {
        TRACE_MEMXT_ELEM* pTrc = &traces.TrcBuf[NextFreeBufInd];

        pTrc->ModId      = ModId;
        pTrc->Line       = Line;
        pTrc->SubSys     = subsys;
        pTrc->Level      = level;

        pTrc->NumOfPar   = 0;

        #if TRC_INCLUDE_FILENAME
            PrepareTrcBuf(pTrc, msg, File);
        #else
            PrepareTrcBuf(pTrc, msg);
        #endif
    }

    void dk_trace_01 (
                #if TRC_INCLUDE_FILENAME
                    void* File,
                #endif
                      PNIO_UINT32 ModId, PNIO_UINT32 Line, PNIO_UINT32 subsys, PNIO_UINT32 level,  void* msg,
                      PNIO_UINT32 para1)
    {
        TRACE_MEMXT_ELEM* pTrc = &traces.TrcBuf[NextFreeBufInd];

        pTrc->ModId      = ModId;
        pTrc->Line       = Line;
        pTrc->SubSys     = subsys;
        pTrc->Level      = level;

        pTrc->NumOfPar   = 1;
        pTrc->Par[0]     = para1;

        #if TRC_INCLUDE_FILENAME
            PrepareTrcBuf(pTrc, msg, File);
        #else
            PrepareTrcBuf(pTrc, msg);
        #endif
    }

    void dk_trace_02 (
                #if TRC_INCLUDE_FILENAME
                    void* File,
                #endif
                      PNIO_UINT32 ModId, PNIO_UINT32 Line, PNIO_UINT32 subsys, PNIO_UINT32 level,  void* msg,
                      PNIO_UINT32 para1, PNIO_UINT32 para2)
    {
        TRACE_MEMXT_ELEM* pTrc = &traces.TrcBuf[NextFreeBufInd];

        pTrc->ModId      = ModId;
        pTrc->Line       = Line;
        pTrc->SubSys     = subsys;
        pTrc->Level      = level;

        pTrc->NumOfPar   = 2;
        pTrc->Par[0]     = para1;
        pTrc->Par[1]     = para2;

        #if TRC_INCLUDE_FILENAME
            PrepareTrcBuf(pTrc, msg, File);
        #else
            PrepareTrcBuf(pTrc, msg);
        #endif
    }


    void dk_trace_03 (
                #if TRC_INCLUDE_FILENAME
                    void* File,
                #endif
                      PNIO_UINT32 ModId, PNIO_UINT32 Line, PNIO_UINT32 subsys, PNIO_UINT32 level,  void* msg,
                      PNIO_UINT32 para1, PNIO_UINT32 para2, PNIO_UINT32 para3)
    {
        TRACE_MEMXT_ELEM* pTrc = &traces.TrcBuf[NextFreeBufInd];

        pTrc->ModId      = ModId;
        pTrc->Line       = Line;
        pTrc->SubSys     = subsys;
        pTrc->Level      = level;

        pTrc->NumOfPar   = 3;
        pTrc->Par[0]     = para1;
        pTrc->Par[1]     = para2;
        pTrc->Par[2]     = para3;

        #if TRC_INCLUDE_FILENAME
            PrepareTrcBuf(pTrc, msg, File);
        #else
            PrepareTrcBuf(pTrc, msg);
        #endif
    }




    void dk_trace_04 (
                #if TRC_INCLUDE_FILENAME
                    void* File,
                #endif
                      PNIO_UINT32 ModId, PNIO_UINT32 Line, PNIO_UINT32 subsys, PNIO_UINT32 level,  void* msg,
                      PNIO_UINT32 para1, PNIO_UINT32 para2, PNIO_UINT32 para3, PNIO_UINT32 para4)
    {
        TRACE_MEMXT_ELEM* pTrc = &traces.TrcBuf[NextFreeBufInd];

        pTrc->ModId      = ModId;
        pTrc->Line       = Line;
        pTrc->SubSys     = subsys;
        pTrc->Level      = level;

        pTrc->NumOfPar   = 4;
        pTrc->Par[0]     = para1;
        pTrc->Par[1]     = para2;
        pTrc->Par[2]     = para3;
        pTrc->Par[3]     = para4;

        #if TRC_INCLUDE_FILENAME
            PrepareTrcBuf(pTrc, msg, File);
        #else
            PrepareTrcBuf(pTrc, msg);
        #endif
    }




    void dk_trace_05 (
                #if TRC_INCLUDE_FILENAME
                    void* File,
                #endif
                      PNIO_UINT32 ModId, PNIO_UINT32 Line, PNIO_UINT32 subsys, PNIO_UINT32 level,  void* msg,
                      PNIO_UINT32 para1, PNIO_UINT32 para2, PNIO_UINT32 para3, PNIO_UINT32 para4,
                      PNIO_UINT32 para5)
    {
        TRACE_MEMXT_ELEM* pTrc = &traces.TrcBuf[NextFreeBufInd];

        pTrc->ModId      = ModId;
        pTrc->Line       = Line;
        pTrc->SubSys     = subsys;
        pTrc->Level      = level;

        pTrc->NumOfPar   = 5;
        pTrc->Par[0]     = para1;
        pTrc->Par[1]     = para2;
        pTrc->Par[2]     = para3;
        pTrc->Par[3]     = para4;
        pTrc->Par[4]     = para5;

        #if TRC_INCLUDE_FILENAME
            PrepareTrcBuf(pTrc, msg, File);
        #else
            PrepareTrcBuf(pTrc, msg);
        #endif
    }



    void dk_trace_06 (
                #if TRC_INCLUDE_FILENAME
                    void* File,
                #endif
                      PNIO_UINT32 ModId, PNIO_UINT32 Line, PNIO_UINT32 subsys, PNIO_UINT32 level,  void* msg,
                      PNIO_UINT32 para1,  PNIO_UINT32 para2,  PNIO_UINT32 para3,  PNIO_UINT32 para4,
                      PNIO_UINT32 para5,  PNIO_UINT32 para6)
    {
        TRACE_MEMXT_ELEM* pTrc = &traces.TrcBuf[NextFreeBufInd];

        pTrc->ModId      = ModId;
        pTrc->Line       = Line;
        pTrc->SubSys     = subsys;
        pTrc->Level      = level;

        pTrc->NumOfPar   = 6;
        pTrc->Par[0]     = para1;
        pTrc->Par[1]     = para2;
        pTrc->Par[2]     = para3;
        pTrc->Par[3]     = para4;
        pTrc->Par[4]     = para5;
        pTrc->Par[5]     = para6;

        #if TRC_INCLUDE_FILENAME
            PrepareTrcBuf(pTrc, msg, File);
        #else
            PrepareTrcBuf(pTrc, msg);
        #endif
    }



    void dk_trace_07 (
                #if TRC_INCLUDE_FILENAME
                    void* File,
                #endif
                      PNIO_UINT32 ModId, PNIO_UINT32 Line, PNIO_UINT32 subsys, PNIO_UINT32 level,  void* msg,
                      PNIO_UINT32 para1,  PNIO_UINT32 para2,  PNIO_UINT32 para3,  PNIO_UINT32 para4,
                      PNIO_UINT32 para5,  PNIO_UINT32 para6,  PNIO_UINT32 para7)
   {
        TRACE_MEMXT_ELEM* pTrc = &traces.TrcBuf[NextFreeBufInd];

        pTrc->ModId      = ModId;
        pTrc->Line       = Line;
        pTrc->SubSys     = subsys;
        pTrc->Level      = level;

        pTrc->NumOfPar   = 7;
        pTrc->Par[0]     = para1;
        pTrc->Par[1]     = para2;
        pTrc->Par[2]     = para3;
        pTrc->Par[3]     = para4;
        pTrc->Par[4]     = para5;
        pTrc->Par[5]     = para6;
        pTrc->Par[6]     = para7;

        #if TRC_INCLUDE_FILENAME
            PrepareTrcBuf(pTrc, msg, File);
        #else
            PrepareTrcBuf(pTrc, msg);
        #endif
    }





    void dk_trace_08 (
                #if TRC_INCLUDE_FILENAME
                    void* File,
                #endif
                      PNIO_UINT32 ModId, PNIO_UINT32 Line, PNIO_UINT32 subsys, PNIO_UINT32 level,  void* msg,
                      PNIO_UINT32 para1,  PNIO_UINT32 para2,  PNIO_UINT32 para3,  PNIO_UINT32 para4,
                      PNIO_UINT32 para5,  PNIO_UINT32 para6,  PNIO_UINT32 para7,  PNIO_UINT32 para8)
    {
        TRACE_MEMXT_ELEM* pTrc = &traces.TrcBuf[NextFreeBufInd];

        pTrc->ModId      = ModId;
        pTrc->Line       = Line;
        pTrc->SubSys     = subsys;
        pTrc->Level      = level;

        pTrc->NumOfPar   = 8;
        pTrc->Par[0]     = para1;
        pTrc->Par[1]     = para2;
        pTrc->Par[2]     = para3;
        pTrc->Par[3]     = para4;
        pTrc->Par[4]     = para5;
        pTrc->Par[5]     = para6;
        pTrc->Par[6]     = para7;
        pTrc->Par[7]     = para8;

        #if TRC_INCLUDE_FILENAME
            PrepareTrcBuf(pTrc, msg, File);
        #else
            PrepareTrcBuf(pTrc, msg);
        #endif
    }




    void dk_trace_09 (
                #if TRC_INCLUDE_FILENAME
                    void* File,
                #endif
                      PNIO_UINT32 ModId, PNIO_UINT32 Line, PNIO_UINT32 subsys, PNIO_UINT32 level,  void* msg,
                      PNIO_UINT32 para1,  PNIO_UINT32 para2,  PNIO_UINT32 para3,  PNIO_UINT32 para4,
                      PNIO_UINT32 para5,  PNIO_UINT32 para6,  PNIO_UINT32 para7,  PNIO_UINT32 para8,
                      PNIO_UINT32 para9)
    {
        TRACE_MEMXT_ELEM* pTrc = &traces.TrcBuf[NextFreeBufInd];

        pTrc->ModId      = ModId;
        pTrc->Line       = Line;
        pTrc->SubSys     = subsys;
        pTrc->Level      = level;

        pTrc->NumOfPar   = 9;
        pTrc->Par[0]     = para1;
        pTrc->Par[1]     = para2;
        pTrc->Par[2]     = para3;
        pTrc->Par[3]     = para4;
        pTrc->Par[4]     = para5;
        pTrc->Par[5]     = para6;
        pTrc->Par[6]     = para7;
        pTrc->Par[7]     = para8;
        pTrc->Par[8]     = para9;

        #if TRC_INCLUDE_FILENAME
            PrepareTrcBuf(pTrc, msg, File);
        #else
            PrepareTrcBuf(pTrc, msg);
        #endif
    }




    void dk_trace_10 (
                #if TRC_INCLUDE_FILENAME
                    void* File,
                #endif
                      PNIO_UINT32 ModId, PNIO_UINT32 Line, PNIO_UINT32 subsys, PNIO_UINT32 level,  void* msg,
                      PNIO_UINT32 para1,  PNIO_UINT32 para2,  PNIO_UINT32 para3,  PNIO_UINT32 para4,
                      PNIO_UINT32 para5,  PNIO_UINT32 para6,  PNIO_UINT32 para7,  PNIO_UINT32 para8,
                      PNIO_UINT32 para9,  PNIO_UINT32 para10)
    {
        TRACE_MEMXT_ELEM* pTrc = &traces.TrcBuf[NextFreeBufInd];

        pTrc->ModId      = ModId;
        pTrc->Line       = Line;
        pTrc->SubSys     = subsys;
        pTrc->Level      = level;

        pTrc->NumOfPar   = 10;
        pTrc->Par[0]     = para1;
        pTrc->Par[1]     = para2;
        pTrc->Par[2]     = para3;
        pTrc->Par[3]     = para4;
        pTrc->Par[4]     = para5;
        pTrc->Par[5]     = para6;
        pTrc->Par[6]     = para7;
        pTrc->Par[7]     = para8;
        pTrc->Par[8]     = para9;
        pTrc->Par[9]     = para10;

        #if TRC_INCLUDE_FILENAME
            PrepareTrcBuf(pTrc, msg, File);
        #else
            PrepareTrcBuf(pTrc, msg);
        #endif
    }



    void dk_trace_11 (
                #if TRC_INCLUDE_FILENAME
                    void* File,
                #endif
                      PNIO_UINT32 ModId, PNIO_UINT32 Line, PNIO_UINT32 subsys, PNIO_UINT32 level,  void* msg,
                      PNIO_UINT32 para1,  PNIO_UINT32 para2,  PNIO_UINT32 para3,  PNIO_UINT32 para4,
                      PNIO_UINT32 para5,  PNIO_UINT32 para6,  PNIO_UINT32 para7,  PNIO_UINT32 para8,
                      PNIO_UINT32 para9,  PNIO_UINT32 para10, PNIO_UINT32 para11)
    {
        TRACE_MEMXT_ELEM* pTrc = &traces.TrcBuf[NextFreeBufInd];

        pTrc->ModId      = ModId;
        pTrc->Line       = Line;
        pTrc->SubSys     = subsys;
        pTrc->Level      = level;

        pTrc->NumOfPar   = 11;
        pTrc->Par[0]     = para1;
        pTrc->Par[1]     = para2;
        pTrc->Par[2]     = para3;
        pTrc->Par[3]     = para4;
        pTrc->Par[4]     = para5;
        pTrc->Par[5]     = para6;
        pTrc->Par[6]     = para7;
        pTrc->Par[7]     = para8;
        pTrc->Par[8]     = para9;
        pTrc->Par[9]     = para10;
        pTrc->Par[10]    = para11;

        #if TRC_INCLUDE_FILENAME
            PrepareTrcBuf(pTrc, msg, File);
        #else
            PrepareTrcBuf(pTrc, msg);
        #endif
    }




    void dk_trace_12 (
                #if TRC_INCLUDE_FILENAME
                    void* File,
                #endif
                      PNIO_UINT32 ModId, PNIO_UINT32 Line, PNIO_UINT32 subsys, PNIO_UINT32 level,  void* msg,
                      PNIO_UINT32 para1,  PNIO_UINT32 para2,  PNIO_UINT32 para3,  PNIO_UINT32 para4,
                      PNIO_UINT32 para5,  PNIO_UINT32 para6,  PNIO_UINT32 para7,  PNIO_UINT32 para8,
                      PNIO_UINT32 para9,  PNIO_UINT32 para10, PNIO_UINT32 para11, PNIO_UINT32 para12)
    {
        TRACE_MEMXT_ELEM* pTrc = &traces.TrcBuf[NextFreeBufInd];

        pTrc->ModId      = ModId;
        pTrc->Line       = Line;
        pTrc->SubSys     = subsys;
        pTrc->Level      = level;

        pTrc->NumOfPar   = 12;
        pTrc->Par[0]     = para1;
        pTrc->Par[1]     = para2;
        pTrc->Par[2]     = para3;
        pTrc->Par[3]     = para4;
        pTrc->Par[4]     = para5;
        pTrc->Par[5]     = para6;
        pTrc->Par[6]     = para7;
        pTrc->Par[7]     = para8;
        pTrc->Par[8]     = para9;
        pTrc->Par[9]     = para10;
        pTrc->Par[10]    = para11;
        pTrc->Par[11]    = para12;
        #if TRC_INCLUDE_FILENAME
            PrepareTrcBuf(pTrc, msg, File);
        #else
            PrepareTrcBuf(pTrc, msg);
        #endif
    }



    void dk_trace_13 (
                #if TRC_INCLUDE_FILENAME
                    void* File,
                #endif
                      PNIO_UINT32 ModId, PNIO_UINT32 Line, PNIO_UINT32 subsys, PNIO_UINT32 level,  void* msg,
                      PNIO_UINT32 para1,  PNIO_UINT32 para2,  PNIO_UINT32 para3,  PNIO_UINT32 para4,
                      PNIO_UINT32 para5,  PNIO_UINT32 para6,  PNIO_UINT32 para7,  PNIO_UINT32 para8,
                      PNIO_UINT32 para9,  PNIO_UINT32 para10, PNIO_UINT32 para11, PNIO_UINT32 para12,
                      PNIO_UINT32 para13)
    {
        TRACE_MEMXT_ELEM* pTrc = &traces.TrcBuf[NextFreeBufInd];

        pTrc->ModId      = ModId;
        pTrc->Line       = Line;
        pTrc->SubSys     = subsys;
        pTrc->Level      = level;

        pTrc->NumOfPar   = 13;
        pTrc->Par[0]     = para1;
        pTrc->Par[1]     = para2;
        pTrc->Par[2]     = para3;
        pTrc->Par[3]     = para4;
        pTrc->Par[4]     = para5;
        pTrc->Par[5]     = para6;
        pTrc->Par[6]     = para7;
        pTrc->Par[7]     = para8;
        pTrc->Par[8]     = para9;
        pTrc->Par[9]     = para10;
        pTrc->Par[10]    = para11;
        pTrc->Par[11]    = para12;
        pTrc->Par[12]    = para13;

        #if TRC_INCLUDE_FILENAME
            PrepareTrcBuf(pTrc, msg, File);
        #else
            PrepareTrcBuf(pTrc, msg);
        #endif
    }



    void dk_trace_14 (
                    #if TRC_INCLUDE_FILENAME
                        void* File,
                    #endif
                      PNIO_UINT32 ModId, PNIO_UINT32 Line, PNIO_UINT32 subsys, PNIO_UINT32 level,  void* msg,
                      PNIO_UINT32 para1,  PNIO_UINT32 para2,  PNIO_UINT32 para3,  PNIO_UINT32 para4,
                      PNIO_UINT32 para5,  PNIO_UINT32 para6,  PNIO_UINT32 para7,  PNIO_UINT32 para8,
                      PNIO_UINT32 para9,  PNIO_UINT32 para10, PNIO_UINT32 para11, PNIO_UINT32 para12,
                      PNIO_UINT32 para13,  PNIO_UINT32 para14)
    {
        TRACE_MEMXT_ELEM* pTrc = &traces.TrcBuf[NextFreeBufInd];

        pTrc->ModId      = ModId;
        pTrc->Line       = Line;
        pTrc->SubSys     = subsys;
        pTrc->Level      = level;

        pTrc->NumOfPar   = 14;
        pTrc->Par[0]     = para1;
        pTrc->Par[1]     = para2;
        pTrc->Par[2]     = para3;
        pTrc->Par[3]     = para4;
        pTrc->Par[4]     = para5;
        pTrc->Par[5]     = para6;
        pTrc->Par[6]     = para7;
        pTrc->Par[7]     = para8;
        pTrc->Par[8]     = para9;
        pTrc->Par[9]     = para10;
        pTrc->Par[10]    = para11;
        pTrc->Par[11]    = para12;
        pTrc->Par[12]    = para13;
        pTrc->Par[13]    = para14;

        #if TRC_INCLUDE_FILENAME
            PrepareTrcBuf(pTrc, msg, File);
        #else
            PrepareTrcBuf(pTrc, msg);
        #endif
    }





    void dk_trace_15 (
                    #if TRC_INCLUDE_FILENAME
                        void* File,
                    #endif
                      PNIO_UINT32 ModId, PNIO_UINT32 Line, PNIO_UINT32 subsys, PNIO_UINT32 level,  void* msg,
                      PNIO_UINT32 para1,  PNIO_UINT32 para2,  PNIO_UINT32 para3,  PNIO_UINT32 para4,
                      PNIO_UINT32 para5,  PNIO_UINT32 para6,  PNIO_UINT32 para7,  PNIO_UINT32 para8,
                      PNIO_UINT32 para9,  PNIO_UINT32 para10, PNIO_UINT32 para11, PNIO_UINT32 para12,
                      PNIO_UINT32 para13,  PNIO_UINT32 para14,  PNIO_UINT32 para15)
    {
        TRACE_MEMXT_ELEM* pTrc = &traces.TrcBuf[NextFreeBufInd];

        pTrc->ModId      = ModId;
        pTrc->Line       = Line;
        pTrc->SubSys     = subsys;
        pTrc->Level      = level;

        pTrc->NumOfPar   = 15;
        pTrc->Par[0]     = para1;
        pTrc->Par[1]     = para2;
        pTrc->Par[2]     = para3;
        pTrc->Par[3]     = para4;
        pTrc->Par[4]     = para5;
        pTrc->Par[5]     = para6;
        pTrc->Par[6]     = para7;
        pTrc->Par[7]     = para8;
        pTrc->Par[8]     = para9;
        pTrc->Par[9]     = para10;
        pTrc->Par[10]    = para11;
        pTrc->Par[11]    = para12;
        pTrc->Par[12]    = para13;
        pTrc->Par[13]    = para14;
        pTrc->Par[14]    = para15;

        #if TRC_INCLUDE_FILENAME
            PrepareTrcBuf(pTrc, msg, File);
        #else
            PrepareTrcBuf(pTrc, msg);
        #endif
    }





    void dk_trace_16 (
                      #if TRC_INCLUDE_FILENAME
                        void* File,
                      #endif
                      PNIO_UINT32 ModId, PNIO_UINT32 Line, PNIO_UINT32 subsys, PNIO_UINT32 level,  void* msg,
                      PNIO_UINT32 para1,  PNIO_UINT32 para2,  PNIO_UINT32 para3,  PNIO_UINT32 para4,
                      PNIO_UINT32 para5,  PNIO_UINT32 para6,  PNIO_UINT32 para7,  PNIO_UINT32 para8,
                      PNIO_UINT32 para9,  PNIO_UINT32 para10, PNIO_UINT32 para11, PNIO_UINT32 para12,
                      PNIO_UINT32 para13,  PNIO_UINT32 para14,  PNIO_UINT32 para15,  PNIO_UINT32 para16)
    {
        TRACE_MEMXT_ELEM* pTrc = &traces.TrcBuf[NextFreeBufInd];

        pTrc->ModId      = ModId;
        pTrc->Line       = Line;
        pTrc->SubSys     = subsys;
        pTrc->Level      = level;

        pTrc->NumOfPar   = 16;
        pTrc->Par[0]     = para1;
        pTrc->Par[1]     = para2;
        pTrc->Par[2]     = para3;
        pTrc->Par[3]     = para4;
        pTrc->Par[4]     = para5;
        pTrc->Par[5]     = para6;
        pTrc->Par[6]     = para7;
        pTrc->Par[7]     = para8;
        pTrc->Par[8]     = para9;
        pTrc->Par[9]     = para10;
        pTrc->Par[10]    = para11;
        pTrc->Par[11]    = para12;
        pTrc->Par[12]    = para13;
        pTrc->Par[13]    = para14;
        pTrc->Par[14]    = para15;
        pTrc->Par[15]    = para16;

        #if TRC_INCLUDE_FILENAME
            PrepareTrcBuf(pTrc, msg, File);
        #else
            PrepareTrcBuf(pTrc, msg);
        #endif
    }


    void dk_trace_array (
                         #if TRC_INCLUDE_FILENAME
                           void* File,
                         #endif
                         PNIO_UINT32 ModId,
                         PNIO_UINT32 Line,
                         PNIO_UINT32 subsys,
                         PNIO_UINT32 level,
                         void*       msg,
                         PNIO_UINT8* ptr, PNIO_UINT32 len)
    {
        TRACE_MEMXT_ELEM* pTrc = &traces.TrcBuf[NextFreeBufInd];
        if (len >= (MAXNUM_TRC_PAR * sizeof(PNIO_UINT32)))
        {
            len = MAXNUM_TRC_PAR * sizeof(PNIO_UINT32);
            OsMemCpy (&pTrc->Par, ptr, len);
        }
        else
        {
            OsMemCpy (&pTrc->Par, ptr, len);
            OsMemSet ( ((PNIO_UINT8*)&pTrc->Par) + len, 0, (MAXNUM_TRC_PAR * sizeof(PNIO_UINT32)) - len);   // fill rest with 0
        }

        pTrc->Type = TRC_TYPE_ARRAY;
        pTrc->TrcCount   = TrcCount;
        TrcCount = (TrcCount > 99999) ? 0 : TrcCount+1;

        pTrc->ModId      = ModId;
        pTrc->Line       = Line;
        pTrc->SubSys     = subsys;
        pTrc->Level      = level;
        pTrc->pFormStr   = (PNIO_INT8*) NULL;   // no format string --> print as an byte array
        pTrc->NumOfPar   = len;                  //`byte array: length in Byte, not in 32Bit values
        pTrc->pFormStr   = msg;

        #if TRC_INCLUDE_FILENAME
            pTrc->pFile = File;
            #if TRC_STORE_FILENAME
                OsStrnCpy (&pTrc->File[0], File, sizeof (pTrc->File));
            #endif
        #endif

        NextFreeBufInd ++;
        if (NextFreeBufInd >= TRCBUF_MAXNUM_ENTRIES)
        {
            NextFreeBufInd = 0;
            TrcBufOverflow = PNIO_TRUE;
        }

        #if TRCBUF_SYNC_BY_SEM
	        // *----------------------------------------------------------
	        // * creates semaphore, to trigger Task_CycleIO at new Cycle
            // * interrupt
	        // *----------------------------------------------------------
            OsGiveSemB (TrcMemXtSemId);
        #endif
    }


    // *===========================================================================================================================================
    // *     NON Realtime part:   prepare and visualize trace entries
    // *===========================================================================================================================================

    // *----------------------------------------------------------------*
    // *   TrcGetBufPtr    (void)
    // *----------------------------------------------------------------*
    // *   returns the start address of the Tracebuffer.
    // *
    // *  input :  --
    // *  out   :  pTrcBbuf          pointer to the trace buffer
    // *----------------------------------------------------------------*
    PNIO_UINT8*    TrcGetBufPtr    (void)
    {
        return ((void*) &traces);
    }

    // *----------------------------------------------------------------*
    // *   TrcGetBufSize    (void)
    // *----------------------------------------------------------------*
    // *   returns the length of the tracebuffer.
    // *
    // *  input :  --
    // *  out   :  pTrcBbuf          pointer to the trace buffer
    // *----------------------------------------------------------------*
    PNIO_UINT32    TrcGetBufSize    (void)
    {
        return (TrcGetTotalSize());
    }

    // *----------------------------------------------------------------*
    // *   TrcGetTotalSize    (void)
    // *----------------------------------------------------------------*
    // *   returns the total size of the tracebuffer.
    // *
    // *  input :  --
    // *  out   :  pTrcBbuf          pointer to the trace buffer
    // *----------------------------------------------------------------*
    PNIO_UINT32    TrcGetTotalSize    (void)
    {
        return (sizeof (traces));
    }



    // *----------------------------------------------------------------*
    // *   TrcStoreBuf    (void)
    // *----------------------------------------------------------------*
    // *   stores the buffer into a persistent memory
    // *
    // *  input :  --
    // *  out   :  PNIO_OK, PNIO_NOT_OK
    // *----------------------------------------------------------------*
    PNIO_UINT32 TrcStoreBuf    (void)
    {
    	PNIO_printf ("Flash Trace Buffer ...\n");
    	traces.OldestRecord = OldestBufInd;
    	traces.BufferFill = BufferFill;
       // FlashTraceBuf  ((void*) TrcGetBufPtr(), TrcGetTotalSize() );
        return (PNIO_OK);
    }



	void SaveTraceBuffer (void)
	{
		PNIO_UINT8* pTrc;
		PNIO_UINT32 FillStart;
		PNIO_UINT32 FillSize;

		OsTakeSemB (BufSemId);

		pTrc = TrcGetBufPtr ();
		FillStart = OldestBufInd;
		FillSize  = BufferFill;
		PNIO_ConsolePrintf ("SaveTraceBuffer (Ptr=0x%08X, Start = %d, Size=%d)\n", pTrc, FillStart, FillSize);
		PNIO_ConsolePrintf ("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

		TrcBufPrintNewest(FillSize);

		PNIO_ConsolePrintf ("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

		OsGiveSemB (BufSemId);
	}

	void RestoreTraceBuffer (void)
	{
		PNIO_UINT8* pTrc;
		PNIO_UINT32 TotalSize;
		PNIO_UINT32 FillStart;
		PNIO_UINT32 FillSize;

		pTrc = TrcGetBufPtr ();
		TotalSize = TrcGetTotalSize();

		// restore trace buffer from flash
	//	RestoreTraceBuf (pTrc, TotalSize);

		OsTakeSemB (BufSemId);
		// get trace buffer start and size
		FillStart = traces.OldestRecord;
		FillSize  = traces.BufferFill;
		if ((FillStart == 0xFFFFFFFF) || (FillSize == 0xFFFFFFFF) ||
		    (FillStart > TotalSize)   || (FillSize > TotalSize))
		{
			PNIO_ConsolePrintf ("ERROR (FlashedTraceBuffer): No valid trace buffer found on flash!\n");
			OsGiveSemB (BufSemId);
			return;
		}
		PNIO_ConsolePrintf ("FlashedTraceBuffer (Ptr=0x%08X, Start = %d, Size=%d)\n", pTrc, FillStart, FillSize);
		PNIO_ConsolePrintf ("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

		if(TRCBUF_MAXNUM_ENTRIES == FillSize)
		{
			NextFreeBufInd = FillStart;
		}
		else
		{
			NextFreeBufInd = FillSize;
		}
		OldestBufInd = FillStart;
		TrcBufPrintNewest(FillSize);

		PNIO_ConsolePrintf ("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		//  cleanup trace buffer
        NextFreeBufInd   = 0;
        OldestBufInd     = 0;
        WorkBufInd       = 0;
        TrcBufOverflow   = PNIO_FALSE;
        BufferFill = 0;

		OsGiveSemB (BufSemId);
	}


    void print_trace_array ( PNIO_UINT32 ModId,
                             PNIO_UINT32 Line,
                             PNIO_UINT32 subsys,
                             PNIO_UINT32 level,
                             PNIO_UINT8* ptr,
                             PNIO_UINT32 len)
    {
        PNIO_INT32 StrBufOfs = 0;   // offset in the string buffer (destination)
        PNIO_UINT32 ofs = 0;                // offset in the array buffer (source)
        PNIO_INT32 rest = len;
        PNIO_UINT8* pStrBuf;

        OsAlloc ((void**) &pStrBuf, ' ', 0x1000);

        StrBufOfs += PNIO_sprintf ((void*)(pStrBuf + StrBufOfs), "\n");
        while (rest > 0)
        {
            if (rest >= BYTES_PER_LINE)
            {
                StrBufOfs += PNIO_sprintf (
                          (void*)(pStrBuf + StrBufOfs),
                          "   %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x\n",
                          *(ptr+ofs+0),  *(ptr+ofs+1),  *(ptr+ofs+2),  *(ptr+ofs+3),
                          *(ptr+ofs+4),  *(ptr+ofs+5),  *(ptr+ofs+6),  *(ptr+ofs+7),
                          *(ptr+ofs+8),  *(ptr+ofs+9),  *(ptr+ofs+10), *(ptr+ofs+11),
                          *(ptr+ofs+12), *(ptr+ofs+13), *(ptr+ofs+14), *(ptr+ofs+15) );
                rest -= BYTES_PER_LINE;
                ofs  +=BYTES_PER_LINE;
            }
            else
            {
                int i;
                StrBufOfs += PNIO_sprintf ((void*)(pStrBuf + StrBufOfs), "   ");
                for (i = 0; i < rest; i++)
                {
                    if (i == 8)
                    {
                        StrBufOfs += PNIO_sprintf ((void*)(pStrBuf + StrBufOfs), "  ");
                    }
                    StrBufOfs += PNIO_sprintf ((void*)(pStrBuf + StrBufOfs), "%02x ", *(ptr+ofs+i));
                }
                rest = 0;
            }

            if (StrBufOfs >= ((5 * TRCBUF_MAX_MSG_LEN) - OFFS_MSG2))
                PNIO_Log (  PNIO_SINGLE_DEVICE_HNDL,
                            PNIO_LOG_ERROR_FATAL,
                            PNIO_PACKID_IOD,
                            TRC_MODULE_ID,
                            __LINE__);
        }

        PNIO_TrcPrintf ("%s\n", pStrBuf);

        OsFree ((void*)pStrBuf);
    }


    // *----------------------------------------------------------------*
    // *   TrcGetFillStart    (void)
    // *----------------------------------------------------------------*
    // *   returns the start position of the tracebuffer.
    // *
    // *  input :  --
    // *  out   :  pTrcBbuf          pointer to the trace buffer
    // *----------------------------------------------------------------*
    PNIO_UINT32    TrcGetFillStart    (void)
    {
        //return (TrcBufFillStart);
    	return 0;
    }


    // *----------------------------------------------------------------*
    // *   TrcGetFillState    (void)
    // *----------------------------------------------------------------*
    // *   returns the length of the tracebuffer.
    // *
    // *  input :  --
    // *  out   :  pTrcBbuf          pointer to the trace buffer
    // *----------------------------------------------------------------*
    PNIO_UINT32    TrcGetFillState   (void)
    {
        //return (TrcBufFillStart);
    	return 0;
    }


    // *----------------------------------------------------------------*
    // *   TrcBufPrintEntry   (void)
    // *----------------------------------------------------------------*
    // *  prepares and prints one entry
    // *
    // *  input :  pTrc       pointer to entry
    // *  out   :  --
    // *----------------------------------------------------------------*
    static void TrcPrintEntry (TRACE_MEMXT_ELEM* pTrc)
    {
        PNIO_INT8 buffer[TRCBUF_MAX_MSG_LEN];
        PNIO_UINT32 StrLength = 0;
    	PNIO_UINT32 TmpSize;
    	PNIO_UINT32 MsgSize;

        OsMemSet (&buffer, ' ', sizeof (buffer));
        if (pTrc->Type == TRC_TYPE_ARRAY)
        {
            #if TRC_INCLUDE_FILENAME
               {
                    PNIO_INT8* p8;
                    p8 = GetFilenameStartPoint((PNIO_INT8*)pTrc->pFile);
                    TmpSize = PNIO_sprintf ((PNIO_UINT8*) &buffer[0], "#%05d %09d: SY/MO/L=%03d/%03d/%04d %s LV=%01d", pTrc->TrcCount, pTrc->TimStampL, pTrc->SubSys, pTrc->ModId, pTrc->Line, p8, pTrc->Level);
                }
            #else
                TmpSize = PNIO_sprintf ((PNIO_UINT8*) &buffer[0], "#%05d %09d: SY/MO/L=%03d/%03d/%04d LV=%01d", pTrc->TrcCount, pTrc->TimStampL, pTrc->SubSys, pTrc->ModId, pTrc->Line, pTrc->Level);
            #endif
            PNIO_TrcPrintf ("%s", buffer);
            print_trace_array (  pTrc->ModId,
                                 pTrc->Line,
                                 pTrc->SubSys,
                                 pTrc->Level,
                                 (PNIO_UINT8*) &(pTrc->Par[0]),
                                 pTrc->NumOfPar);
            return;
        }

        if (pTrc->pFormStr == (PNIO_INT8*)0)
            return;

        switch (pTrc->NumOfPar)
        {
            case  0:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr);
                   break;
            case  1:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0]);
                   break;
            case  2:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1]);
                   break;
            case  3:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2]);
                   break;
            case  4:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3]);
                   break;
            case  5:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4]);
                   break;
            case  6:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5]);
                   break;
            case  7:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6]);
                  break;
            case  8:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7]);
                   break;
            case  9:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7],
                                                 pTrc->Par[8]);
                   break;
            case 10:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7],
                                                 pTrc->Par[8], pTrc->Par[9]);
                   break;
            case 11:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7],
                                                 pTrc->Par[8], pTrc->Par[9], pTrc->Par[10]);
                   break;
            case 12:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7],
                                                 pTrc->Par[8], pTrc->Par[9], pTrc->Par[10], pTrc->Par[11]);
                   break;
            case 13:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7],
                                                 pTrc->Par[8], pTrc->Par[9], pTrc->Par[10], pTrc->Par[11], pTrc->Par[12]);
                   break;
            case 14:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7],
                                                 pTrc->Par[8], pTrc->Par[9], pTrc->Par[10], pTrc->Par[11], pTrc->Par[12], pTrc->Par[13]);
                   break;
            case 15:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7],
                                                 pTrc->Par[8], pTrc->Par[9], pTrc->Par[10], pTrc->Par[11], pTrc->Par[12], pTrc->Par[13], pTrc->Par[14]);
                   break;
            case 16:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7],
                                                 pTrc->Par[8], pTrc->Par[9], pTrc->Par[10], pTrc->Par[11], pTrc->Par[12], pTrc->Par[13], pTrc->Par[14], pTrc->Par[15]);
                   break;

            default:
                   break;


        }

        if (StrLength > (TRCBUF_MAX_MSG_LEN - OFFS_MSG2))
            PNIO_Log (  PNIO_SINGLE_DEVICE_HNDL,
                        PNIO_LOG_ERROR_FATAL,
                        PNIO_PACKID_IOD,
                        TRC_MODULE_ID,
                        __LINE__);


        MsgSize = StrLength + OFFS_MSG2 + 3;

        // shorten message to max buffer length
        if (MsgSize >TRCBUF_MAX_MSG_LEN)
        {
            MsgSize = TRCBUF_MAX_MSG_LEN;
        }
        buffer[MsgSize-3] = 0x0d;    // add CR (overwrites end delimiter)
        buffer[MsgSize-2] = 0x0a;    // add line feet
        buffer[MsgSize-1]   = 0;       // new end delimiter

        #if TRC_INCLUDE_FILENAME
           {
                PNIO_INT8* p8;
                p8 = GetFilenameStartPoint((PNIO_INT8*)pTrc->pFile);
                TmpSize = PNIO_sprintf ((PNIO_UINT8*) &buffer[0], "#%05d %09d: SY/MO/L=%03d/%03d/%04d %s LV=%01d", pTrc->TrcCount, pTrc->TimStampL, pTrc->SubSys, pTrc->ModId, pTrc->Line, p8, pTrc->Level);
            }
        #else
            TmpSize = PNIO_sprintf ((PNIO_UINT8*) &buffer[0], "#%05d %09d: SY/MO/L=%03d/%03d/%04d LV=%01d", pTrc->TrcCount, pTrc->TimStampL, pTrc->SubSys, pTrc->ModId, pTrc->Line, pTrc->Level);
        #endif

        // change end delimiter from last PNIO_sprintf to ' '
        buffer[TmpSize] = ' ';

        PNIO_TrcPrintf ("%s\n", buffer);
    }


#if 0
    // *----------------------------------------------------------------*
    // *   TrcBufPrintRawEntry   (void)
    // *----------------------------------------------------------------*
    // *  prints one entry without preparation.
    // *  preparation here is done on the visualization-PC
    // *
    // *  input :  pTrc       pointer to entry
    // *  out   :  --
    // *----------------------------------------------------------------*
    static void TrcPrintRawEntry (TRACE_MEMXT_ELEM* pTrc)
    {
        PNIO_INT8 buffer[TRCBUF_MAX_MSG_LEN];
        PNIO_UINT32 StrLength;
    	PNIO_UINT32 TmpSize;
    	PNIO_UINT32 MsgSize;

        OsMemSet (&buffer, ' ', sizeof (buffer));
        if (pTrc->pFormStr == NULL)
        {
            print_trace_array (pTrc->ModId,
                         pTrc->Line,
                         pTrc->SubSys,
                         pTrc->Level,
                         (PNIO_UINT8*) &(pTrc->Par[0]),
                         pTrc->NumOfPar);
            return;
        }


        switch (pTrc->NumOfPar)
        {
            case  0:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr);
                   break;
            case  1:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0]);
                   break;
            case  2:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1]);
                   break;
            case  3:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2]);
                   break;
            case  4:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3]);
                   break;
            case  5:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4]);
                   break;
            case  6:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5]);
                   break;
            case  7:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6]);
                  break;
            case  8:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7]);
                   break;
            case  9:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7],
                                                 pTrc->Par[8]);
                   break;
            case 10:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7],
                                                 pTrc->Par[8], pTrc->Par[9]);
                   break;
            case 11:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7],
                                                 pTrc->Par[8], pTrc->Par[9], pTrc->Par[10]);
                   break;
            case 12:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7],
                                                 pTrc->Par[8], pTrc->Par[9], pTrc->Par[10], pTrc->Par[11]);
                   break;
            case 13:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7],
                                                 pTrc->Par[8], pTrc->Par[9], pTrc->Par[10], pTrc->Par[11], pTrc->Par[12]);
                   break;
            case 14:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7],
                                                 pTrc->Par[8], pTrc->Par[9], pTrc->Par[10], pTrc->Par[11], pTrc->Par[12], pTrc->Par[13]);
                   break;
            case 15:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7],
                                                 pTrc->Par[8], pTrc->Par[9], pTrc->Par[10], pTrc->Par[11], pTrc->Par[12], pTrc->Par[13], pTrc->Par[14]);
                   break;
            case 16:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &buffer[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7],
                                                 pTrc->Par[8], pTrc->Par[9], pTrc->Par[10], pTrc->Par[11], pTrc->Par[12], pTrc->Par[13], pTrc->Par[14], pTrc->Par[15]);
                   break;

            default:   StrLength = 0;       // 2016-04-19 fix04.5
                   break;


        }

        if (StrLength > (TRCBUF_MAX_MSG_LEN - OFFS_MSG2))
            PNIO_Log (  PNIO_SINGLE_DEVICE_HNDL,
                        PNIO_LOG_ERROR_FATAL,
                        PNIO_PACKID_IOD,
                        TRC_MODULE_ID,
                        __LINE__);


        MsgSize = StrLength + OFFS_MSG2 + 3;

        // shorten message to max buffer length
        if (MsgSize >TRCBUF_MAX_MSG_LEN)
        {
            MsgSize = TRCBUF_MAX_MSG_LEN;
        }
        buffer[MsgSize-3] = 0x0d;    // add CR (overwrites end delimiter)
        buffer[MsgSize-2] = 0x0a;    // add line feet
        buffer[MsgSize-1]   = 0;       // new end delimiter

        TmpSize = PNIO_sprintf ((PNIO_UINT8*) &buffer[0], "#%05d %09d: SY=%03d MO=%03d L%04d LV=%01d", pTrc->TrcCount, pTrc->TimStampL, pTrc->SubSys, pTrc->ModId, pTrc->Line, pTrc->Level);

        // change end delimiter from last PNIO_sprintf to ' '
        buffer[TmpSize] = ' ';

        PNIO_printf ("%s\n", buffer);
    }
#endif



    // *----------------------------------------------------------------*
    // *   TrcBufPrintNewest   (void)
    // *----------------------------------------------------------------*
    // *  allocates a buffer and transformes the messages into a printable
    // *  form.  The buffer has to be deleted with TrcBufFree() after use.
    // *
    // *  input :  NumOfEntries      number of entries to print
    // *        :  ppDstBuf          pointer to destination buffer pointer
    // *  out   :  pTrcBbuf          pointer to the trace buffer
    // *----------------------------------------------------------------*
    PNIO_VOID TrcBufPrintNewest (PNIO_UINT32 NumOfEntries)
    {
        PNIO_UINT32 i;
        PNIO_INT32 BufInd;
        TRACE_MEMXT_ELEM* pTrc;

        // find first entry to print

        BufInd = NextFreeBufInd - NumOfEntries ;
        if (BufInd < 0)
        {
            BufInd += TRCBUF_MAXNUM_ENTRIES;
        }

        // prepare for print
        for (i = 0; i < NumOfEntries; i++)
        {
            pTrc = &traces.TrcBuf[BufInd];

            // print buffer entry
            TrcPrintEntry (pTrc);

            // next element
            BufInd++;
            if (BufInd >= TRCBUF_MAXNUM_ENTRIES)
            {
                BufInd = 0;
            }
        }
    }

    // *----------------------------------------------------------------*
    // *   TrcBufPrintOldest   (void)
    // *----------------------------------------------------------------*
    // *   returns the length of the tracebuffer.
    // *
    // *  input :  NumOfEntries      number of entries to print
    // *        :
    // *  out   :  pTrcBbuf          pointer to the trace buffer
    // *----------------------------------------------------------------*
    PNIO_VOID TrcBufPrintOldest (PNIO_UINT32 NumOfEntries)
    {
        PNIO_UINT32 i;
        PNIO_INT32 BufInd;
        TRACE_MEMXT_ELEM* pTrc;

        // find first entry to print

        BufInd = OldestBufInd;

        // prepare for print
        for (i = 0; i < NumOfEntries; i++)
        {
            pTrc = &traces.TrcBuf[BufInd];

            // print buffer entry
            TrcPrintEntry (pTrc);

            // next element
            BufInd++;
            if (BufInd >= TRCBUF_MAXNUM_ENTRIES)
            {
                BufInd = 0;
            }
        }
    }

    // *----------------------------------------------------------------*
    // *   TrcBufPrintNext   (void)
    // *----------------------------------------------------------------*
    // *   returns the length of the tracebuffer.
    // *
    // *  input :  MaxNumOfEntries   maximum number of entries to print (0: print all)
    // *        :
    // *  out   :  pTrcBbuf          pointer to the trace buffer
    // *----------------------------------------------------------------*
    PNIO_VOID TrcBufPrintNext (PNIO_UINT32 MaxNumOfEntries)
    {
        PNIO_UINT32 i = 0;
        TRACE_MEMXT_ELEM* pTrc;

        // find first entry to print

        //
        if (MaxNumOfEntries == 0)
        {
            MaxNumOfEntries = TRCBUF_MAXNUM_ENTRIES;
        }

        // prepare for print
        while ((WorkBufInd != NextFreeBufInd) && (i < MaxNumOfEntries))
        {
            pTrc = &traces.TrcBuf[WorkBufInd];

            // print buffer entry
            TrcPrintEntry (pTrc);

            // next element
            WorkBufInd++;
            i++;

            if (WorkBufInd >= TRCBUF_MAXNUM_ENTRIES)
            {
                WorkBufInd = 0;
            }
        }
    }


    /*
     * Trace functionality for XHIF
     */

    static PNIO_UINT32 MemLastBufInd;
    static PNIO_UINT32 MemNumOfBytes;
    static PNIO_BOOL MemOverflow;

    PNIO_UINT32 print_trace_array_to_mem ( PNIO_UINT32 ModId,
                             PNIO_UINT32 Line,
                             PNIO_UINT32 subsys,
                             PNIO_UINT32 level,
                             PNIO_UINT8* ptr,
                             PNIO_UINT32 len,
                             PNIO_UINT8* MemPtr)
    {
        PNIO_INT32 StrBufOfs = 0;   // offset in the string buffer (destination)
        PNIO_UINT32 ofs = 0;                // offset in the array buffer (source)
        PNIO_UINT32 rest = len;

        OsMemSet (MemPtr, ' ', 0x1000);

        StrBufOfs += PNIO_sprintf ((void*)(MemPtr + StrBufOfs), "\n");
        while (rest > 0)
        {
            if (rest >= BYTES_PER_LINE)
            {
                StrBufOfs += PNIO_sprintf (
                          (void*)(MemPtr + StrBufOfs),
                          "   %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x\n",
                          *(ptr+ofs+0),  *(ptr+ofs+1),  *(ptr+ofs+2),  *(ptr+ofs+3),
                          *(ptr+ofs+4),  *(ptr+ofs+5),  *(ptr+ofs+6),  *(ptr+ofs+7),
                          *(ptr+ofs+8),  *(ptr+ofs+9),  *(ptr+ofs+10), *(ptr+ofs+11),
                          *(ptr+ofs+12), *(ptr+ofs+13), *(ptr+ofs+14), *(ptr+ofs+15) );
                rest -= BYTES_PER_LINE;
                ofs  +=BYTES_PER_LINE;
            }
            else
            {
                int i;
                StrBufOfs += PNIO_sprintf ((void*)(MemPtr + StrBufOfs), "   ");
                for (i = 0; i < rest; i++)
                {
                    if (i == 8)
                    {
                        StrBufOfs += PNIO_sprintf ((void*)(MemPtr + StrBufOfs), "  ");
                    }
                    StrBufOfs += PNIO_sprintf ((void*)(MemPtr + StrBufOfs), "%02x ", *(ptr+ofs+i));
                }
                rest = 0;
            }

            if (StrBufOfs >= ((5 * TRCBUF_MAX_MSG_LEN) - OFFS_MSG2))
                PNIO_Log (  PNIO_SINGLE_DEVICE_HNDL,
                            PNIO_LOG_ERROR_FATAL,
                            PNIO_PACKID_IOD,
                            TRC_MODULE_ID,
                            __LINE__);
        }

        return len;
    }

    // *----------------------------------------------------------------*
    // *   TrcPrintEntryToMem   ()
    // *----------------------------------------------------------------*
    // *  prepares and prints one entry
    // *
    // *  input :  pTrc       pointer to entry
    // *  input :  mPtr       pointer to buffer memory (where to store data)
    // *  out   :  StrLength  length of stored string
    // *----------------------------------------------------------------*
    static PNIO_UINT32 TrcPrintEntryToMem (TRACE_MEMXT_ELEM* pTrc, PNIO_UINT8* mPtr)
    {
        PNIO_UINT32 i;
        PNIO_UINT32 StrLength = 0;
        PNIO_UINT32 TmpSize;
        PNIO_UINT32 MsgSize;

        /* Avoid memory overflow! */
        if((MemNumOfBytes + TRCBUF_MAX_MSG_LEN) > XHIF_MAX_NUM_OF_TRACES_B)
        {
            MemOverflow = PNIO_TRUE;
            return 0;
        }

        /* Memset - fill buffer with spaces */
        for(i = 0; i < TRCBUF_MAX_MSG_LEN; i ++)
        {
            mPtr[i] = ' ';
        }

        if (pTrc->Type == TRC_TYPE_ARRAY)
        {
            #if TRC_INCLUDE_FILENAME
               {
                    PNIO_INT8* p8;
                    p8 = GetFilenameStartPoint((PNIO_INT8*)pTrc->pFile);
                    TmpSize = PNIO_sprintf ((PNIO_UINT8*) &mPtr[0], "#%05d %09d: SY/MO/L=%03d/%03d/%04d %s LV=%01d\n", pTrc->TrcCount, pTrc->TimStampL, pTrc->SubSys, pTrc->ModId, pTrc->Line, p8, pTrc->Level);
                }
            #else
                TmpSize = PNIO_sprintf ((PNIO_UINT8*) &mPtr[0], "#%05d %09d: SY/MO/L=%03d/%03d/%04d LV=%01d\n", pTrc->TrcCount, pTrc->TimStampL, pTrc->SubSys, pTrc->ModId, pTrc->Line, pTrc->Level);
            #endif

            /* Avoid memory overflow! */
            if((MemNumOfBytes + 0x1000) > XHIF_MAX_NUM_OF_TRACES_B)
            {
                MemOverflow = PNIO_TRUE;
                return 0;
            }

            /* Memset - fill rest of buffer with spaces */
            for(i = TRCBUF_MAX_MSG_LEN; i < 0x1000; i ++)
            {
                mPtr[i] = ' ';
            }

            TmpSize += print_trace_array_to_mem(pTrc->ModId, pTrc->Line,
                    pTrc->SubSys, pTrc->Level, (PNIO_UINT8*) &(pTrc->Par[0]),
                    pTrc->NumOfPar, &mPtr[TmpSize]);

            return TmpSize;
        }

        if (pTrc->pFormStr == (PNIO_INT8*)0)
            return 0;

        switch (pTrc->NumOfPar)
        {
            case  0:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &mPtr[OFFS_MSG2], pTrc->pFormStr);
                   break;
            case  1:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &mPtr[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0]);
                   break;
            case  2:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &mPtr[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1]);
                   break;
            case  3:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &mPtr[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2]);
                   break;
            case  4:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &mPtr[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3]);
                   break;
            case  5:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &mPtr[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4]);
                   break;
            case  6:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &mPtr[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5]);
                   break;
            case  7:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &mPtr[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6]);
                  break;
            case  8:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &mPtr[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7]);
                   break;
            case  9:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &mPtr[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7],
                                                 pTrc->Par[8]);
                   break;
            case 10:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &mPtr[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7],
                                                 pTrc->Par[8], pTrc->Par[9]);
                   break;
            case 11:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &mPtr[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7],
                                                 pTrc->Par[8], pTrc->Par[9], pTrc->Par[10]);
                   break;
            case 12:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &mPtr[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7],
                                                 pTrc->Par[8], pTrc->Par[9], pTrc->Par[10], pTrc->Par[11]);
                   break;
            case 13:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &mPtr[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7],
                                                 pTrc->Par[8], pTrc->Par[9], pTrc->Par[10], pTrc->Par[11], pTrc->Par[12]);
                   break;
            case 14:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &mPtr[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7],
                                                 pTrc->Par[8], pTrc->Par[9], pTrc->Par[10], pTrc->Par[11], pTrc->Par[12], pTrc->Par[13]);
                   break;
            case 15:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &mPtr[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7],
                                                 pTrc->Par[8], pTrc->Par[9], pTrc->Par[10], pTrc->Par[11], pTrc->Par[12], pTrc->Par[13], pTrc->Par[14]);
                   break;
            case 16:  StrLength = PNIO_sprintf ( (PNIO_UINT8*) &mPtr[OFFS_MSG2], pTrc->pFormStr,
                                                 pTrc->Par[0], pTrc->Par[1], pTrc->Par[2],  pTrc->Par[3],  pTrc->Par[4],  pTrc->Par[5],  pTrc->Par[6],  pTrc->Par[7],
                                                 pTrc->Par[8], pTrc->Par[9], pTrc->Par[10], pTrc->Par[11], pTrc->Par[12], pTrc->Par[13], pTrc->Par[14], pTrc->Par[15]);
                   break;

            default:
                   break;
        }

        if (StrLength > (TRCBUF_MAX_MSG_LEN - OFFS_MSG2))
            PNIO_Log (  PNIO_SINGLE_DEVICE_HNDL,
                        PNIO_LOG_ERROR_FATAL,
                        PNIO_PACKID_IOD,
                        TRC_MODULE_ID,
                        __LINE__);


        MsgSize = StrLength + OFFS_MSG2 + 3;

        // shorten message to max buffer length
        if (MsgSize >TRCBUF_MAX_MSG_LEN)
        {
            MsgSize = TRCBUF_MAX_MSG_LEN;
        }
        mPtr[MsgSize-3] = 0x0d;    // add CR (overwrites end delimiter)
        mPtr[MsgSize-2] = 0x0a;    // add line feet
        mPtr[MsgSize-1]   = 0;     // new end delimiter

        #if TRC_INCLUDE_FILENAME
           {
                PNIO_INT8* p8;
                p8 = GetFilenameStartPoint((PNIO_INT8*)pTrc->pFile);
                TmpSize = PNIO_sprintf ((PNIO_UINT8*) &mPtr[0], "#%05d %09d: SY/MO/L=%03d/%03d/%04d %s LV=%01d", pTrc->TrcCount, pTrc->TimStampL, pTrc->SubSys, pTrc->ModId, pTrc->Line, p8, pTrc->Level);
            }
        #else
            TmpSize = PNIO_sprintf ((PNIO_UINT8*) &mPtr[0], "#%05d %09d: SY/MO/L=%03d/%03d/%04d LV=%01d", pTrc->TrcCount, pTrc->TimStampL, pTrc->SubSys, pTrc->ModId, pTrc->Line, pTrc->Level);
        #endif

        // change end delimiter from last PNIO_sprintf to ' '
        mPtr[TmpSize] = ' ';

        return MsgSize;
    }

    // *----------------------------------------------------------------*
    // *   TrcBufPrintOldestToMem   ()
    // *----------------------------------------------------------------*
    // *   returns the length of the tracebuffer.
    // *
    // *  input :  NumOfEntries      number of entries to print
    // *        :  mPtr              pointer to destination buffer pointer
    // *  out   :  pTrcBbuf          pointer to the trace buffer
    // *----------------------------------------------------------------*
    PNIO_BOOL TrcBufPrintOldestToMem (PNIO_UINT32 NumOfEntries, PNIO_UINT8* mPtr)
    {
        PNIO_UINT32 bytes = 0;
        PNIO_UINT32 i;
        TRACE_MEMXT_ELEM* pTrc;
        PNIO_UINT8* pMem = mPtr;
        PNIO_BOOL lastTrc = PNIO_FALSE;

        // find first entry to print
        MemLastBufInd = OldestBufInd;

        // if trace buf is empty return
        if(MemLastBufInd == NextFreeBufInd)
        {
            lastTrc = PNIO_TRUE;
        }
        else
        {
        // prepare for print
        for (i = 0; i < NumOfEntries; i++)
        {
            pTrc = &traces.TrcBuf[MemLastBufInd];

            // print buffer entry
            bytes = TrcPrintEntryToMem (pTrc, pMem);

				if(MemOverflow == PNIO_TRUE)
            {
					lastTrc = PNIO_TRUE;
                break;
            }

            /* No overflow, add counters */
            pMem += bytes;
            MemNumOfBytes += bytes;

            // next element
            MemLastBufInd++;
            if (MemLastBufInd >= TRCBUF_MAXNUM_ENTRIES)
            {
                MemLastBufInd = 0;
            }

            if(MemLastBufInd == NextFreeBufInd)
            {
					lastTrc = PNIO_TRUE;
					break;
                }
            }
        }

        return lastTrc;
    }

    // *----------------------------------------------------------------*
    // *   TrcBufPrintNewestToMem   ()
    // *----------------------------------------------------------------*
    // *   returns the length of the tracebuffer.
    // *
    // *  input :  NumOfEntries      number of entries to print
    // *        :  mPtr              pointer to destination buffer pointer
    // *  out   :  pTrcBbuf          pointer to the trace buffer
    // *----------------------------------------------------------------*
    PNIO_BOOL TrcBufPrintNewestToMem (PNIO_UINT32 NumOfEntries, PNIO_UINT8* mPtr)
    {
        PNIO_UINT32 bytes = 0;
        PNIO_UINT32 i;
        TRACE_MEMXT_ELEM* pTrc;
        PNIO_UINT8* pMem = mPtr;
        PNIO_BOOL lastTrc = PNIO_FALSE;

        // find first entry to print
        MemLastBufInd = NextFreeBufInd - NumOfEntries ;
        if (MemLastBufInd < 0)
        {
        	MemLastBufInd += TRCBUF_MAXNUM_ENTRIES;
        }

        // if trace buf is empty return
        if(MemLastBufInd == NextFreeBufInd)
        {
            lastTrc = PNIO_TRUE;
        }
        else
        {
			// prepare for print
			for (i = 0; i < NumOfEntries; i++)
			{
				pTrc = &traces.TrcBuf[MemLastBufInd];

				// print buffer entry
				bytes = TrcPrintEntryToMem (pTrc, pMem);

				if(MemOverflow == PNIO_TRUE)
				{
					lastTrc = PNIO_TRUE;
					break;
				}

				/* No overflow, add counters */
				pMem += bytes;
				MemNumOfBytes += bytes;

				// next element
				MemLastBufInd++;
				if (MemLastBufInd >= TRCBUF_MAXNUM_ENTRIES)
				{
					MemLastBufInd = 0;
				}

				if(MemLastBufInd == NextFreeBufInd)
				{
					lastTrc = PNIO_TRUE;
                break;
                }
            }
        }

        return lastTrc;
    }

    // *----------------------------------------------------------------*
    // *   TrcBufPrintNextToMem   ()
    // *----------------------------------------------------------------*
    // *   returns the length of the tracebuffer.
    // *
    // *  input :  MaxNumOfEntries   maximum number of entries to print (0: print all)
    // *        :  mPtr              pointer to destination buffer pointer
    // *  out   :  pTrcBbuf          pointer to the trace buffer
    // *----------------------------------------------------------------*
    PNIO_BOOL TrcBufPrintNextToMem (PNIO_UINT32 MaxNumOfEntries, PNIO_UINT8* mPtr)
    {
        PNIO_UINT32 bytes = 0;
        PNIO_UINT32 i = 0;
        TRACE_MEMXT_ELEM* pTrc;
        PNIO_UINT8* pMem = mPtr;
        PNIO_BOOL lastTrc = PNIO_FALSE;

        // prepare for print
        while ((MemLastBufInd != NextFreeBufInd) && (i < MaxNumOfEntries))
        {
            pTrc = &traces.TrcBuf[MemLastBufInd];

            // print buffer entry
            bytes = TrcPrintEntryToMem (pTrc, pMem);

            if(MemOverflow == PNIO_TRUE)
            {
                PNIO_printf("\nMemOverflow\n");
                lastTrc = PNIO_TRUE;
                break;
            }

            /* No overflow, add counters */
            pMem += bytes;
            MemNumOfBytes += bytes;

            // next element
            MemLastBufInd++;
            i++;

            if (MemLastBufInd >= TRCBUF_MAXNUM_ENTRIES)
            {
                MemLastBufInd = 0;
            }

            if(MemLastBufInd == NextFreeBufInd)
            {
                lastTrc = PNIO_TRUE;
                break;
            }
        }

        return lastTrc;
    }

    // *----------------------------------------------------------------*
    // *   SendTraceBufferToMem   ()
    // *----------------------------------------------------------------*
    // *   returns the length of the tracebuffer.
    // *
    // *  input :  first_run is true if traces shall start from beginning
    // *		:  store is true when initial string shall be avoided
    // *        :  last will store true if there is no traces left to print
    // *        :  ptr points to memory where to store trace strings
    // *        :  len will store number of stored bytes
    // *  out   :  none
    // *----------------------------------------------------------------*
    void SendTraceBufferToMem (PNIO_BOOL FirstRun, PNIO_BOOL Store, PNIO_BOOL* last, PNIO_UINT8* MemPtr, PNIO_UINT32* len)
    {
        PNIO_UINT8* pTrc;
        PNIO_UINT8* pMem = MemPtr;
        PNIO_UINT8* pStrBuf;
        OsAlloc ((void**) &pStrBuf, 0, 0x2000);

        /* Clear trace memory */
        MemNumOfBytes = 0;
        MemOverflow = PNIO_FALSE;

        OsTakeSemB (BufSemId);
        pTrc = TrcGetBufPtr();

        *last = PNIO_TRUE;

        /* Start trace from begin? */
        if(FirstRun == PNIO_TRUE)
        {
            /* Init message */
        	if(Store == PNIO_FALSE)
        	{
                *len += PNIO_sprintf ((PNIO_UINT8*) &pStrBuf[*len], "SaveTraceBuffer (Ptr=0x%08X, Start = %d, Size=%d)\n", pTrc, OldestBufInd, BufferFill);
                *len += PNIO_sprintf ((PNIO_UINT8*) &pStrBuf[*len], "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        	}

            /* Put string to memory */
            *last = TrcBufPrintOldestToMem(XHIF_MAX_NUM_OF_TRACES, &pStrBuf[*len]);
            *len += MemNumOfBytes;
        }
        else
        {
            /* Trace continue */
            /* Put string to memory */
            *last = TrcBufPrintNextToMem(XHIF_MAX_NUM_OF_TRACES, pStrBuf);
            *len += MemNumOfBytes;
        }

        OsGiveSemB (BufSemId);

        /* Copy data to BBB */
        OsMemCpy(pMem, pStrBuf, *len);
    }


    // *----------------------------------------------------------------*
    // *   RestoreTraceBufferToMem   ()
    // *----------------------------------------------------------------*
    // *
    // *  input :  first_run is true if traces shall start from beginning
    // *        :  last will store true if there is no traces left to print
    // *        :  ptr points to memory where to store trace strings
    // *        :  len will store number of stored bytes
    // *  out   :  none
    // *----------------------------------------------------------------*
    void RestoreTraceBufferToMem (PNIO_BOOL FirstRun, PNIO_BOOL* last, PNIO_UINT8* MemPtr, PNIO_UINT32* len)
	{
		PNIO_UINT8* pTrc;
		PNIO_UINT32 TotalSize;
		PNIO_UINT32 FillStart;
		PNIO_UINT32 FillSize;

		PNIO_UINT8* pMem = MemPtr;
		PNIO_UINT8* pStrBuf;
		OsAlloc ((void**) &pStrBuf, 0, 0x2000);

		/* Clear trace memory */
		MemNumOfBytes = 0;
		MemOverflow = PNIO_FALSE;

		OsTakeSemB (BufSemId);
		pTrc = TrcGetBufPtr();
		TotalSize = TrcGetTotalSize();

		*last = PNIO_TRUE;

		/* Start trace from begin? */
		if(FirstRun == PNIO_TRUE)
		{
			// restore trace buffer from flash
		//	RestoreTraceBuf (pTrc, TotalSize);

			// get trace buffer start and size
			FillStart = traces.OldestRecord;
			FillSize  = traces.BufferFill;
			if ((FillStart == 0xFFFFFFFF) || (FillSize == 0xFFFFFFFF) ||
				(FillStart > TotalSize)   || (FillSize > TotalSize))
			{
				*len += PNIO_sprintf ((PNIO_UINT8*) &pStrBuf[*len], "ERROR (FlashedTraceBuffer): No valid trace buffer found on flash!\n");

				OsGiveSemB (BufSemId);
				return;
			}

			*len += PNIO_sprintf ((PNIO_UINT8*) &pStrBuf[*len], "FlashedTraceBuffer (Ptr=0x%08X, Start = %d, Size=%d)\n", pTrc, FillStart, FillSize);
			*len += PNIO_sprintf ((PNIO_UINT8*) &pStrBuf[*len], "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

			if(TRCBUF_MAXNUM_ENTRIES == FillSize)
			{
				NextFreeBufInd = FillStart;
			}
			else
			{
				NextFreeBufInd = FillSize;
			}

			 /* Put string to memory */
			OldestBufInd = FillStart;
			*last = TrcBufPrintNewestToMem(XHIF_MAX_NUM_OF_TRACES, &pStrBuf[*len]);
			*len += MemNumOfBytes;
		}
		else
		{
            /* Trace continue */
            /* Put string to memory */
            *last = TrcBufPrintNextToMem(XHIF_MAX_NUM_OF_TRACES, pStrBuf);
            *len += MemNumOfBytes;
		}

		if(*last == PNIO_TRUE)
		{
			//  cleanup trace buffer
			NextFreeBufInd   = 0;
			OldestBufInd     = 0;
			WorkBufInd       = 0;
			TrcBufOverflow   = PNIO_FALSE;
			BufferFill = 0;
		}

        /* Copy data to BBB */
        OsMemCpy(pMem, pStrBuf, *len);

		OsGiveSemB (BufSemId);
	}
    
// *----------------------------------------------------------------*
// *   GetFilenameStartPoint   ()
// *----------------------------------------------------------------*
// *           Parses the filename from text (which contains file location)
// *  input :  pFile = pTrc->pFile pointer
// *  out   :  start point of the filename 
// *----------------------------------------------------------------*
PNIO_INT8* GetFilenameStartPoint(PNIO_INT8 * pFile){
    PNIO_INT32  i = 40;   //max file name length size
    PNIO_INT8* p8;
        if (pFile)
            p8 = (PNIO_INT8*)pFile + OsStrLen (pFile);            // set on last possible character (will be set to end delimiter)
        else
            p8 = " ";
        while (i >= 0)    // strlen + 1 byte end delimiter
        {
            if ((*(p8) == 0x5c) || (*(p8) == 0x2f) )
                {
                    break;
                }
            else
                {
                    p8--;
                    i--;
                }
        }

    return p8;
}
#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
