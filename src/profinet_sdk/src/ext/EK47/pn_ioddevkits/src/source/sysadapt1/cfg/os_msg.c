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
/*  F i l e               &F: os_msg.c                                  :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  OS abstraction layer implementation for a windows operating system       */
/*                                                                           */
/*****************************************************************************/


// *---------------------------------------------------------------------
// *    OS system adation layer 
// *
// *
// *
// *
// *---------------------------------------------------------------------

#include "compiler.h"
#if (INCLUDE_DK_MSG_HANDLER)
 
    #include "os.h"
    #include "os_cfg.h"
	#include "trc_if.h"
	#include "lsa_cfg.h"


    #define LTRC_ACT_MODUL_ID   -1
    #define	IOD_MODULE_ID       -1


    // *--------------------------------------------------*
    // *   test and debugging
    // *--------------------------------------------------*
    #define DEBUG_TEST1             0

	#define MSG_ENTER		OsIntDisable();
	#define MSG_EXIT		OsIntEnable();

    // *---------------------------------*
    // * defines
    // *---------------------------------*
    #define MSG_QUEUE_LIMIT  MAXNUM_MSG_ENTRIES
    #define MSG_POOL_LIMIT   (MSG_QUEUE_LIMIT * MAXNUM_OF_TASKS)
    #define OS_MSGX_ID          0x3829acdc // any unique tag, to identify OsSendMessageX-Telegram

    typedef struct
    {
        PNIO_VOID*   pInfo;    // user info, the "netto" message data
        PNIO_VOID*   pNext;
    } MESSAGE;

    typedef struct
    {
        PNIO_UINT32	        FillStat;	            // fill level indicator
        PNIO_UINT32	        MaxFillStat;            // maximum value of fill level (for debug statistics only)
        PNIO_UINT32	        NumOfSnd;	            // number of sent messages
        PNIO_UINT32	        NumOfRec;	            // number of received messages
        PNIO_UINT32         SemId;                  // semaphore to wait for new message
        MESSAGE*            pQMsg1Head;             // high priority message queue
        MESSAGE*            pQMsg1Tail;             // high priority message queue
        PNIO_BOOL           IsCreated;                  
    } MESSAGE_BOX;

    // *---------------------------------*
    // * static variables
    // *---------------------------------*
    static PNIO_BOOL    OsMsgInitialized = PNIO_FALSE;
    static MESSAGE_BOX  MsgBox [MAXNUM_OF_TASKS];       // first message box, assigned to a task
    static MESSAGE      MsgMem[MSG_POOL_LIMIT];         // allocated memory for all message containers
    static MESSAGE*     MsgPool[MSG_POOL_LIMIT];        // contains pointers to all availaible message containers
    static PNIO_UINT32  MsgPoolFillInd = 0;             // number of available message blocks in the pool
    static PNIO_UINT32  MsgId;                          // unique message ID, for debug purpose only

    static MESSAGE*     MsgContainerAlloc()
    {
        MESSAGE* pMsg;
        MSG_ENTER;
        if (MsgPoolFillInd)
        {
            pMsg = MsgPool[MsgPoolFillInd - 1];
            MsgPoolFillInd--;
            pMsg->pNext = (MESSAGE*)0;
            MSG_EXIT;
            return (pMsg);
        }
        else
        {
        	MSG_EXIT;
            return ((MESSAGE*) 0);
        }

    }

    static PNIO_VOID MsgContainerFree(MESSAGE* pMsg)
    {
        OS_ASSERT (MsgPoolFillInd < MSG_POOL_LIMIT);
        OS_ASSERT (pMsg != (MESSAGE*)0);   

        MSG_ENTER;
        MsgPool[MsgPoolFillInd] = pMsg;
        MsgPoolFillInd++;
        MSG_EXIT;
   }



    // *-----------------------------------------------------------*
    // *  OsMsgInit ()
    // *    
    // *  basic initialization of message system, must be called first
    // *-----------------------------------------------------------*
    PNIO_VOID OsMsgInit(void)
    {
        int i;
        OsMemSet (MsgBox,  0, sizeof (MsgBox));
        OsMemSet (MsgPool, 0, sizeof (MsgPool));
        OsMemSet (MsgMem,  0, sizeof (MsgMem));

        for (i = 0; i < MSG_POOL_LIMIT; i++)
        {
            MsgPool[i] = &MsgMem[i];
        }

        MsgPoolFillInd      = MSG_POOL_LIMIT;    // pool is full of memory for messages
        OsMsgInitialized    = PNIO_TRUE;
        MsgId               = 0;                 // unique message ID, for debug purpose only
    }


    // *-----------------------------------------------------------*
    // *  OsCreateMsgQueue ()
    // *    
    // *  Create a message queue,  assigned to the spezified task
    // *
    // *
    // *-----------------------------------------------------------*
    PNIO_UINT32	OsCreateMsgQueue (PNIO_UINT32 TaskId)
    {
        PNIO_UINT32 Status;

        OS_ASSERT ((TaskId < MAXNUM_OF_TASKS) && (TaskId != 0));
        if (OsMsgInitialized == PNIO_FALSE)
        {
            OsMsgInit();
        }

        Status = OsAllocSemB (&(MsgBox[TaskId].SemId));
        OS_ASSERT (Status == PNIO_OK);

        MsgBox[TaskId].IsCreated = PNIO_TRUE;
        return (PNIO_OK);
    }





    // *-----------------------------------------------------------*
    // *  DebugReadMessageBlocked
    // *    
    // *  reads a message from the message queue of the calling thread
    // *  the function will be blocked if no message is available.
    // *  If pMessage == 0, the function will wait for a message, but
    // *  the message will be discarded.
    // *-----------------------------------------------------------*
    static PNIO_UINT32 DebugReadMessageBlocked (void** ppMessage, PNIO_UINT32 TaskId)
    {
        MESSAGE_BOX*  pMsgBox;
        MESSAGE*      pMsg;

        PNIO_BOOL     exit = PNIO_FALSE;

        OS_ASSERT (OsMsgInitialized == PNIO_TRUE);
        OS_ASSERT (TaskId < MAXNUM_OF_TASKS);
        OS_ASSERT (ppMessage != (void**)0);

        pMsgBox = &MsgBox[TaskId];

	    // *** Wait for Message ***
        if (pMsgBox->FillStat == 0)   // message box empty ?
        {
            while (exit == PNIO_FALSE)
            {
                // ***** wait for next message on semaphore
                OsTakeSemB (pMsgBox->SemId);
                if (pMsgBox->pQMsg1Head)
                {
                    exit = PNIO_TRUE;
                }
            }
        }

        // **** read next message, chain out and free message container ***
        MSG_ENTER;
        pMsg = pMsgBox->pQMsg1Head;
        pMsgBox->pQMsg1Head = pMsg->pNext;
        pMsgBox->FillStat--;
        if (pMsgBox->FillStat == 0)
        {
            pMsgBox->pQMsg1Tail = pMsgBox->pQMsg1Head;  // head = tail = 0
        }
        *ppMessage = (void**) pMsg->pInfo;
        MSG_EXIT;
        
        MsgContainerFree (pMsg);

	    return (PNIO_OK);
    }



    // *-----------------------------------------------------------*
    // *  DebugSendMessage
    // *    
    // *  sends a message to the message queue of the specified 
    // *  task
    // *
    // *
    // *-----------------------------------------------------------*
    static PNIO_UINT32 DebugSendMessage   (PNIO_UINT32 TaskId,
				                           void* pMessage,
				                           PNIO_UINT32 MsgPrio)
    {
        PNIO_BOOL	RetVal = 0;
        MESSAGE_BOX*  pMsgBox;
        MESSAGE*      pMsg;

        OS_ASSERT (OsMsgInitialized == PNIO_TRUE);
        OS_ASSERT (TaskId < MAXNUM_OF_TASKS);

        pMsgBox = &MsgBox[TaskId];
        if (pMsgBox->FillStat >= MSG_QUEUE_LIMIT)
        {
            return (PNIO_NOT_OK);        // queue is already full, no entry
        }

        pMsg = MsgContainerAlloc ();

        MSG_ENTER;
        if (pMsgBox->FillStat >= MSG_QUEUE_LIMIT)
        {
            MSG_EXIT;
            MsgContainerFree (pMsg);
            return (PNIO_NOT_OK);        // queue is already full, no entry
        }
        if (pMsg == (MESSAGE*)0)
        {
            MSG_EXIT;
            MsgContainerFree (pMsg);
            return (PNIO_NOT_OK); 
        }

        pMsg->pInfo = (MESSAGE*) pMessage;
        if (pMsgBox->FillStat)
        {
            (pMsgBox->pQMsg1Tail)->pNext = pMsg;
        }
        else
        {
            pMsgBox->pQMsg1Head = pMsg;
        }
        pMsgBox->pQMsg1Tail = pMsg;
        pMsgBox->FillStat++;
        MSG_EXIT;

        OsGiveSemB (pMsgBox->SemId);

        return (PNIO_OK); //ok
    }



    // *-----------------------------------------------------------*
    // *  OsReadMessageBlocked
    // *    
    // *  reads a message from the message queue of the calling thread
    // *  the function will be blocked if no message is available.
    // *  If pMessage == 0, the function will wait for a message, but
    // *  the message will be discarded.
    // *-----------------------------------------------------------*
    PNIO_UINT32 OsReadMessageBlocked (void** ppMessage, PNIO_UINT32 TaskId)
    {
        #if (DEBUG_TEST1)
            PNIO_UINT32 Status;
            PNIO_UINT32* pDebMsg;

            Status = DebugReadMessageBlocked (&pDebMsg, TaskId);

            if (pDebMsg)
            {
                OS_ASSERT (*(pDebMsg+4) == OS_MSGX_ID);
                *ppMessage  = (void*) *(pDebMsg+0);
                LSA_TRACE_04  (TaskProp[*(pDebMsg+2)].TraceSubsys, 
                               LSA_TRACE_LEVEL_NOTE_HIGH, 
							   "OsRdM (P=0x%x D=%s S=%s M=%d\n", 
							   *(pDebMsg+0),						// message ptr
							   TaskProp[*(pDebMsg+1)].TaskName,		// destination task
							   TaskProp[*(pDebMsg+2)].TaskName,		// source task
							   *(pDebMsg+3));						// message ID
                OsFree (pDebMsg);


            }
            else
                LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, 
                               LSA_TRACE_LEVEL_ERROR, 
                               "NULL-pointer received\n");
 
            return (Status);
		#else
			return (DebugReadMessageBlocked (ppMessage, TaskId));
        #endif

    }


    // *-----------------------------------------------------------*
    // *  OsReadMessageBlockedX
    // *    
    // *  reads a message from the message queue of the calling thread
    // *  the function will be blocked if no message is available.
    // *  If pMessage == 0, the function will wait for a message, but
    // *  the message will be discarded.
    // *-----------------------------------------------------------*
    PNIO_UINT32 OsReadMessageBlockedX (void** ppMessage1, void** ppMessage2, PNIO_UINT32 TaskId)
    {
        PNIO_UINT32 Status;
        PNIO_UINT32* pMsg;

        Status = OsReadMessageBlocked (&pMsg, TaskId);

        if (pMsg)
        {
            OS_ASSERT (*(pMsg+2) == OS_MSGX_ID);
            *ppMessage1 = (void*) *(pMsg+0);
            *ppMessage2 = (void*) *(pMsg+1);

            OsFree (pMsg);
        }
        else
            LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, 
                           LSA_TRACE_LEVEL_ERROR, 
                           "NULL-pointer received\n");

        return (Status);
    }
    // *-----------------------------------------------------------*
    // *  OsSendMessage
    // *    
    // *  sends a message to the message queue of the specified 
    // *  task
    // *
    // *
    // *-----------------------------------------------------------*
    PNIO_UINT32 OsSendMessage (PNIO_UINT32 TaskId,
				               void* pMessage1,
				               PNIO_UINT32 MsgPrio)
    {
        #if (DEBUG_TEST1)
			PNIO_UINT32 *pDebMsg;
			PNIO_UINT32 Status;
			static PNIO_UINT32 MsgId = 0;

			OsAlloc (&pDebMsg, 0, 5 * sizeof(PNIO_UINT32)); // size for pMessage1, pMessage2, Tag-Id

			MsgId++;
			*(pDebMsg+3) = MsgId;
			*(pDebMsg+0) = (PNIO_UINT32)pMessage1;
			*(pDebMsg+1) = (PNIO_UINT32)TaskId;
			*(pDebMsg+2) = (PNIO_UINT32)OsGetThreadId();
			*(pDebMsg+4) = (PNIO_UINT32)OS_MSGX_ID;


            LSA_TRACE_04  (TaskProp[*(pDebMsg+2)].TraceSubsys,
                           LSA_TRACE_LEVEL_NOTE_HIGH, 
                           "OsSndM (P=0x%x D=%s S=%s M=%d\n", 
						   *(pDebMsg+0),						// message ptr
						   TaskProp[*(pDebMsg+1)].TaskName,		// destination task
						   TaskProp[*(pDebMsg+2)].TaskName,		// destination task
						   *(pDebMsg+3));						// message ID

			Status = DebugSendMessage (TaskId, pDebMsg, MsgPrio);



			return (Status); 
		#else
			return (DebugSendMessage (TaskId, pMessage1, MsgPrio));
        #endif
    }

    // *-----------------------------------------------------------*
    // *  OsSendMessageX
    // *    
    // *  sends a expanded message to the message queue of the specified 
    // *  task. It is possible, to send 2 pointers in one message instead
    // *  of one pointer. So we can send a function pointer and a 
    // *  requestblock pointer in one one message, to execute a 
    // *  function call in a remote task.
    // *-----------------------------------------------------------*
    PNIO_UINT32 OsSendMessageX (PNIO_UINT32 TaskId,
				       void* pMessage1,
				       void* pMessage2,
				       PNIO_UINT32 MsgPrio)
    {
        PNIO_UINT32 *pMsg;

        OsAlloc (&pMsg, 0, 3 * sizeof(PNIO_UINT32)); // size for pMessage1, pMessage2, Tag-Id

        *(pMsg+0) = (PNIO_UINT32)pMessage1;
        *(pMsg+1) = (PNIO_UINT32)pMessage2;
        *(pMsg+2) = (PNIO_UINT32)OS_MSGX_ID;

        return (OsSendMessage (TaskId, pMsg, MsgPrio));
    }

    // *-----------------------------------------------------------*
    // *  OsGetNumOfMessages
    // *    
    // *  reads the number of messages in the specified Queue 
    // *-----------------------------------------------------------*
    PNIO_UINT32 OsGetNumOfMessages (PNIO_UINT32 TaskId)
    {
        OS_ASSERT (OsMsgInitialized == PNIO_TRUE);
        OS_ASSERT (TaskId < MAXNUM_OF_TASKS);
        return (MsgBox[TaskId].FillStat);
    }

#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
