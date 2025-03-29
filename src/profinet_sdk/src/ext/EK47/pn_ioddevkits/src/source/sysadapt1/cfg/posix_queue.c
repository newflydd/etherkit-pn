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
/*  F i l e               &F: posix_queue.c                             :F&  */
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
 * @file     posix_queue.c
 * @brief    functions for IPC communcation
 *
 * interface for IPC communcation
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

	#define OS_MSGX_ID          	0x3829acdc
	#define DEFAULT_QUEUE			"/queue"

    int measurement_ready;
    int measurement_id;
    int measurement_data;
    int measurement_value;

    /**
     * @brief Initialize of structures for message queues
     *
     * @return  void    no return value
     */
	void OsMessageQueuesInit ( void )
	{
		int i = 0;

		for ( i = 0; i < MAXNUM_OF_TASKS; i++)
		{
			MessagePool[i].id = 0;
			MessagePool[i].name = LSA_NULL;
		}

	    measurement_ready = 0;
	    measurement_id = 0;
	    measurement_data = 0;
	    measurement_value = 0;
	}

    /**
     * @brief Destroy all message queues
     *
     * @return  void    no return value
     */
	void OsMessageQueuesDestroy ( void )
	{
		int i = 0;

		for ( i = 0; i < MAXNUM_OF_TASKS; i++)
		{
			if ( MessagePool[i].name != LSA_NULL )
			{
	    		mq_close ( MessagePool[i].id );
	    		mq_unlink( MessagePool[i].name );

	    		MessagePool[i].id = 0;
	    		MessagePool[i].name = LSA_NULL;
			}
		}
	}

    /**
     * @brief Create name of message queue using ThreadId and const char array DEFAULT_QUEUE
     *
     * @param[out]  buf      as char **       name of queue
     * @param[in]  ThreadId as PNIO_UINT32   threadId for which the name will be created
     *
     * @return void         no return vale
     */
	static void OsCreateMsgQueueId ( char ** buf, int ThreadId)
	{
    	char *f, *tmp;
    	unsigned int i;

        tmp = (char *) malloc ( 10 );

        f =  ( char *) DEFAULT_QUEUE;

        for ( i = 0; i < 6; i ++)
        {
            * ( tmp + i ) = f[i];
        }

        * ( tmp + 6 ) = '.';
        * ( tmp + 7 ) = 0x30 + ( ( char ) (ThreadId / 10 ) );
        * ( tmp + 8 ) = 0x30 + ( ( char ) (ThreadId % 10 ) );
        * ( tmp + 9 ) = 0;

        *buf = tmp;
	}

	/**
     * @brief Create message queue for Thread which is taken from parameter
     *
     * @param[in] ThreadId as PNIO_UINT32
     *
     * @return PNIO_UINT32	success : PNIO_OK
     * 						error   : PNIO_NOT_OK
     */
    PNIO_UINT32	OsCreateMsgQueue      (PNIO_UINT32 ThreadId)
    {
    	PNIO_UINT32 status = 0;
    	struct mq_attr *attr, newattr;

        OS_ASSERT (ThreadId < MAXNUM_OF_TASKS);

        OsCreateMsgQueueId(&MessagePool[ThreadId].name, ThreadId);
        mq_unlink( MessagePool[ThreadId].name );

        OsAlloc( (void **) &attr, 0, sizeof ( struct mq_attr ) );

    	attr->mq_flags = 0;
    	attr->mq_maxmsg = MSG_QUEUE_LIMIT;
    	attr->mq_msgsize = sizeof(PNIO_UINT32);
    	attr->mq_curmsgs = 0;

    	MessagePool[ThreadId].id  = mq_open( MessagePool[ThreadId].name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH, attr);

    	if ( ( PNIO_INT32 ) MessagePool[ThreadId].id  == -1 )
        {
                LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "creation of message queue was failed : %d \n", errno );
				return PNIO_NOT_OK;
        }

        status = mq_getattr (MessagePool[ThreadId].id, &newattr);

        if ( (status != 0) || (newattr.mq_msgsize != attr->mq_msgsize) )
        {
            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "creation of message queue was failed : %d \n", errno );
            
            mq_close ( MessagePool[ThreadId].id );
            mq_unlink( MessagePool[ThreadId].name );

            free(attr);
            free(MessagePool[ThreadId].name);

            return PNIO_NOT_OK;
        }
        LSA_TRACE_04  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_NOTE, "message queue %s is created for %d Thread ID: (msg_size: %d, max_msg: %d)\n", MessagePool[ThreadId].name, (int) ThreadId, (int) newattr.mq_msgsize, (int) newattr.mq_maxmsg );
        return PNIO_OK;
    }

    /**
     * @brief Receive data from message queue
     *
     * @param[in]   ThreadId    - Thread for which waiting message in queue will be computed
     *
     * @return  PNIO_INT32 - success : number of messages
     *                        failed  : -1
     */
    PNIO_INT32 OsGetNumOfMessages    (PNIO_UINT32 ThreadId)
    {
    	PNIO_UINT32 ret = 0;
    	struct mq_attr  mttr;

        OS_ASSERT (ThreadId < MAXNUM_OF_TASKS);

    	ret = mq_getattr (MessagePool[ThreadId].id, &mttr);
        if (ret != 0)
        {
            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "attempt to get number of waiting messages in queue was failed : %d \n", errno );
            return -1;
        }

    	return (PNIO_INT32) mttr.mq_curmsgs;
    }

    /**
     * @brief Receive data from message queue
     *
     * @param[in]   ppMessage   - pointer to pointer of received data
     * @param[in]   ThreadId    - Thread for which data will be received through message queue
     *
     * @return  PNIO_UINT32 - success : PNIO_OK
     *                        failed  : PNIO_NOT_OK
     */
    OS_CODE_FAST PNIO_UINT32 OsReadMessageBlocked  (void** ppMessage, PNIO_UINT32 ThreadId)
    {
    	PNIO_INT32 ret = 0;
    	PNIO_INT32 ptrMsg;

        OS_ASSERT (ThreadId < MAXNUM_OF_TASKS);
        OS_ASSERT (ppMessage != LSA_NULL);

    	ret = mq_receive ( MessagePool[ThreadId].id, ( PNIO_INT8 *) &ptrMsg, sizeof (PNIO_UINT32), LSA_NULL );
        if ( ret < 0 )
        {
            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "receiving of data was failed with errno : %d \n", errno );
        	return PNIO_NOT_OK;
        }

        *ppMessage = (void *) ptrMsg;

    	return PNIO_OK;
    }

    /**
     * @brief Receive data from message queue, data are comming in two parts
     *
     * @param[in]   pMessage1   - pointer to pointer of received data
     * @param[in]   pMessage1   - pointer to pointer of received data
     * @param[in]   ThreadId    - Thread for which data will be received through message queue
     *
     * @return  PNIO_UINT32 - success : PNIO_OK
     *                        failed  : PNIO_NOT_OK
     */
    OS_CODE_FAST PNIO_UINT32 OsReadMessageBlockedX (void** ppMessage1, void** ppMessage2, PNIO_UINT32 ThreadId)
    {
    	PNIO_UINT32 ret = 0;
        PNIO_UINT32* pMsg;

    	ret = OsReadMessageBlocked ((void **)&pMsg, ThreadId);

        if (pMsg)
        {
            OS_ASSERT (*(pMsg+2) == OS_MSGX_ID);
            *ppMessage1 = (void*) *(pMsg+0);
            *ppMessage2 = (void*) *(pMsg+1);

            OsFree (pMsg);
        }
        else
            LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "NULL-pointer received\n");

    	return ret;
    }

    /**
     * @brief Send data which are taken from parameter
     *
     * @param[in]   ThreadId    - Thread whom data will be sent through message queue
     * @param[in]   pMessage1   - pointer to transmiting data
     * @param[in]   MsgPrio     - priority of message
     *
     * @return  PNIO_UINT32 - success : PNIO_OK
     *                        failed  : PNIO_NOT_OK
     */
    OS_CODE_FAST PNIO_UINT32 OsSendMessage (PNIO_UINT32 ThreadId,
						                    void* pMessage,
						                    PNIO_UINT32 MsgPrio)
    {
    	PNIO_UINT32 		ret = 0;

        OS_ASSERT (ThreadId < MAXNUM_OF_TASKS);

        ret = mq_send ( MessagePool[ThreadId].id, (const char *)&pMessage, sizeof (PNIO_UINT32), (unsigned int) LSA_NULL );

        if ( ret < 0 )
        {
            LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "transmit of data was failed with errno : %d \n", errno );
        	return PNIO_NOT_OK;
        }

        return PNIO_OK;
    }

    /**
     * @brief Send data which are taken from two parameters
     *
     * @param[in]   ThreadId    - Thread whom data will be sent through message queue
     * @param[in]   pMessage1   - First part of transmiting data
     * @param[in]   pMessage2   - Second part of transmitting data
     * @param[in]   MsgPrio     - priority of message
     *
     * @return  PNIO_UINT32 - success : PNIO_OK
     *                        failed  : PNIO_NOT_OK
     */
    OS_CODE_FAST PNIO_UINT32 OsSendMessageX (PNIO_UINT32 ThreadId,
						                      void* pMessage1,
						                      void* pMessage2,
						                      PNIO_UINT32 MsgPrio)
    {
    	PNIO_UINT32 ret = 0;
        PNIO_UINT32 *pMsg;

    	OsAlloc( (void **) &pMsg, 0, 3 * sizeof(PNIO_UINT32) );

        *(pMsg+0) = (PNIO_UINT32)pMessage1;
        *(pMsg+1) = (PNIO_UINT32)pMessage2;
        *(pMsg+2) = (PNIO_UINT32)OS_MSGX_ID;

        ret = OsSendMessage(ThreadId,pMsg,MsgPrio);

    	return ret;
    }
#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
