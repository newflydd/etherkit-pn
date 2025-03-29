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
/*  C o m p o n e n t     &C: TCPIP (TCIP for Interniche Stack)         :C&  */
/*                                                                           */
/*  F i l e               &F: tcip_send.c                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  NRT send functionality                                                   */
/*                                                                           */
/*****************************************************************************/
#define LTRC_ACT_MODUL_ID  4
#define TCIP_MODULE_ID     4

#include "tcip_int.h"
#include "anyip_edd_send.h"

TCIP_FILE_SYSTEM_EXTENSION(TCIP_MODULE_ID) /* no semicolon */

/*---------------------------------------------------------------------------*/
/*  the RQBs are allocated and single linked, last->next == LSA_NULL         */
/*  the list is handled as a LIFO list,                                      */
/*  the latest used RQB will be the next to be used                          */
/*  this allows simple list routines, for the lists are associated to a      */
/*  edd channel, the start anchor is a parameter                             */
/*---------------------------------------------------------------------------*/
/* queue routine to retrieve a RQB from the preallocated list                */
/* INPUT: SendRqbs head of the queue                                         */
/*---------------------------------------------------------------------------*/
static EDD_RQB_TYPE * tcip_getEddSendRQB (EDD_UPPER_RQB_PTR_TYPE * SendRqbs)
{
	TCIP_LOG_FCT("tcip_getEddSendRQB")
	EDD_RQB_TYPE * pEddSendRQB;

	pEddSendRQB = *SendRqbs;

	if( is_not_null(pEddSendRQB) )
	{
		*SendRqbs = EDD_RQB_GET_NEXT_RQB_PTR(pEddSendRQB);
	}
	else
	{
		TCIP_UPPER_TRACE_02(0, LSA_TRACE_LEVEL_NOTE, "tcip_getEddSendRQB: SendRqbs 0x%x, pEddSendRQB 0x%x", SendRqbs, pEddSendRQB);
	}

	TCIP_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_CHAT,"returned RQB 0x%x",pEddSendRQB);

	return(pEddSendRQB);
}

/*---------------------------------------------------------------------------*/
/* queue routine to return a RQB back to the preallocated list               */
/* INPUT: SendRqbs head of the queue                                         */
/*        actSendRqb Rqb that is returned, becomes first element of queue    */
/*---------------------------------------------------------------------------*/
static LSA_VOID tcip_putEddSendRQB (EDD_UPPER_RQB_PTR_TYPE *SendRqbs, EDD_RQB_TYPE * actSendRqb)
{
	TCIP_LOG_FCT("tcip_putEddSendRQB")

	if( is_null(*SendRqbs) )
	{
		EDD_RQB_SET_NEXT_RQB_PTR(actSendRqb,LSA_NULL);
	}
	else
	{
		EDD_RQB_SET_NEXT_RQB_PTR(actSendRqb,*SendRqbs);
	}

	*SendRqbs = actSendRqb;

	TCIP_UPPER_TRACE_02(0, LSA_TRACE_LEVEL_CHAT,"inserted RQB 0x%x next = 0x%x",actSendRqb,TCIP_EDD_GET_NEXT_RQB_PTR(actSendRqb));
}

/*
 *  free send buf and RQB
 */
 /*lint -e{818} symbol channel Pointer parameter 'Symbol' (Location) could be declared ptr to const */
static LSA_VOID tcip_FreeSndBuf( TCIP_CHANNEL_CONST_PTR_TYPE channel, TCIP_EDD_LOWER_RQB_TYPE  *pRQB)
{
	TCIP_LOG_FCT("tcip_FreeSndBuf")

	EDD_UPPER_NRT_SEND_PTR_TYPE     pParam = LSA_NULL;
	LSA_UINT16                      ret16_val;

	pParam = TCIP_EDD_GET_PPARAM( pRQB, EDD_UPPER_NRT_SEND_PTR_TYPE );

	if ( is_not_null(pParam) )
	{
		TCIP_EDD_LOWER_MEM_PTR_TYPE pFrame = pParam->pBuffer;

		if ( is_not_null(pFrame) )
		{
			TCIP_EDD_FREE_TX_TRANSFER_BUFFER_MEM(&ret16_val, pFrame, channel->sysptr);
			TCIP_ASSERT(ret16_val == LSA_RET_OK);
		}
	}

	tcip_FreeRQB(channel->sysptr, pRQB );
}

/*---------------------------------------------------------------------------*/
/* initialisize a queue of EDD Send RQBs                                     */
/* INPUT: SendRqbs head of the queue                                         */
/*        Count  Nr of RQBs to add to queue                                  */
/*        EDD_handle handle of EDD to be used to process the send request    */
/*        pSys sys pointer                                                   */
/*---------------------------------------------------------------------------*/

LSA_VOID tcip_initEddSendRQBList(EDD_UPPER_RQB_PTR_TYPE *SendRqbs,
								LSA_UINT32       Count,
								EDD_HANDLE_LOWER_TYPE  EDD_handle,
								LSA_SYS_PTR_TYPE pSys)
{
	TCIP_LOG_FCT("tcip_initEddSendRQBList")
	LSA_UINT32 i;

	*SendRqbs = LSA_NULL;    /* there is no one up to now! */

	for (i = 0; i <Count; i++)
	{

		EDD_UPPER_RQB_PTR_TYPE pRQB = tcip_AllocRQB( pSys, sizeof(EDD_RQB_NRT_SEND_TYPE));

		if( is_null(pRQB) )
		{
			TCIP_UPPER_TRACE_00(0, LSA_TRACE_LEVEL_FATAL,"allocation of lower RQB failed check send-RQB configuration");
			TCIP_FATAL();
		}
		else
		{
			EDD_UPPER_NRT_SEND_PTR_TYPE pParam;
			pParam = TCIP_EDD_GET_PPARAM(pRQB, EDD_UPPER_NRT_SEND_PTR_TYPE);

#if TCIP_CFG_COPY_ON_SEND
			{
			void * pFrame;
			unsigned short sendBufferSize = ANYIP_SENDBUFFER_SIZE;

			TCIP_EDD_ALLOC_TX_TRANSFER_BUFFER_MEM(&pFrame, sendBufferSize, pSys);
			TCIP_ASSERT( pFrame != LSA_NULL );

			pParam->pBuffer = (EDD_UPPER_MEM_U8_PTR_TYPE)pFrame;
			}
#else
			pParam->pBuffer = LSA_NULL;
#endif

			TCIP_EDD_SET_OPCODE( pRQB, EDD_OPC_REQUEST, EDD_SRV_NRT_SEND);
			TCIP_EDD_SET_HANDLE( pRQB, 0);
			EDD_RQB_SET_HANDLE_LOWER(pRQB, EDD_handle);

			tcip_putEddSendRQB(SendRqbs,pRQB);
		}
	}
}


/*===========================================================================*/

/*
 * tcip_cancel_send_resources()
 * returns all queued PAKETS to the free pkt-list
 */
LSA_VOID tcip_cancel_send_resources(TCIP_CHANNEL_CONST_PTR_TYPE channel)
{
	EDD_RQB_TYPE *pRQB;

	while((pRQB = tcip_getEddSendRQB((EDD_UPPER_RQB_PTR_TYPE *)&(channel->edd.send_rqbs))) != LSA_NULL)
	{
		tcip_FreeSndBuf(channel, pRQB);
	}
}

/*===========================================================================*/
/*===========================================================================*/

/*lint -e{818} symbol channel Pointer parameter 'Symbol' (Location) could be declared ptr to const */
static LSA_VOID tcip_edd_nrt_send (TCIP_CHANNEL_CONST_PTR_TYPE  channel, ANYIP_PKTBUF_PTR packet, EDD_UPPER_RQB_PTR_TYPE  pRQB)
{
	TCIP_LOG_FCT ("tcip_edd_nrt_send")
	EDD_UPPER_NRT_SEND_PTR_TYPE pParam = TCIP_EDD_GET_PPARAM(pRQB, EDD_UPPER_NRT_SEND_PTR_TYPE);
	LSA_UINT32 txFrmGroup;
	unsigned char protocol_flags = 0;
	int        sendLen= anyip_edd_get_sendlen(packet);
	LSA_UINT8 *sendBuf = LSA_NULL;
	int ret = 0;

	if (sendLen > ANYIP_SENDBUFFER_SIZE) /* do not send */
	{
		TCIP_FATAL();
	}

#if TCIP_CFG_COPY_ON_SEND
	sendBuf = pParam->pBuffer;
#else
	sendBuf = (LSA_UINT8 *)anyip_edd_get_ip_pkt_ptr(packet);
#endif

	TCIP_ASSERT(sendBuf != LSA_NULL);

#if TCIP_CFG_COPY_ON_SEND
	ret = anyip_edd_cpy_pkt_to_buffer(packet, (char *)sendBuf, sendLen);
	anyip_edd_free_pkt(packet);
#else
	pParam->pBuffer = sendBuf;
#endif

	if (ret == 0)
	{
		protocol_flags = anyip_edd_get_protocol_flags((char *)sendBuf);

		if ((protocol_flags & ANYIP_IP) != 0) /* IP message */
		{
			if ((protocol_flags & ANYIP_UDP) != 0)
			{
				TCIP_UPPER_TRACE_00(0, LSA_TRACE_LEVEL_CHAT, "UDP packet to send");
				txFrmGroup = EDD_NRT_TX_GRP_IP_UDP;
			}
			else if ((protocol_flags & ANYIP_TCP) != 0)
			{
				TCIP_UPPER_TRACE_00(0, LSA_TRACE_LEVEL_CHAT, "TCP packet to send");
				txFrmGroup = EDD_NRT_TX_GRP_IP_TCP;
			}
			else /* ICMP or something else */
			{
				TCIP_UPPER_TRACE_00(0, LSA_TRACE_LEVEL_CHAT, "ICMP or other IP packet to send");
				txFrmGroup = EDD_NRT_TX_GRP_IP_ICMP;
			}
		}
		else /* ARP or something else */
		{
			TCIP_UPPER_TRACE_00(0, LSA_TRACE_LEVEL_CHAT, "ARP packet to send");
			txFrmGroup = EDD_NRT_TX_GRP_ARP;
		}

		/***/

		pParam->Length = (LSA_UINT32)sendLen;
		pParam->PortID = EDD_PORT_ID_AUTO;
		pParam->Priority = EDD_NRT_SEND_PRIO_0; /* for EDDI and EDDS, ignored by EDDP */
		pParam->TxFrmGroup = txFrmGroup;  /* for EDDP, ignored by EDDI and EDDS */

		/***/
#if !TCIP_CFG_COPY_ON_SEND
		TCIP_EDD_SET_USER_ID_PTR(pRQB, packet);
#endif

		TCIP_EDD_SET_HANDLE(pRQB, 0);
		EDD_RQB_SET_HANDLE_LOWER(pRQB, channel->edd.handle_lower);

		TCIP_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_NOTE, "RQB 0x%x", pRQB);

		TCIP_EDD_REQUEST_LOWER(pRQB, channel->sysptr);
	}

	return;
}

/*===========================================================================*/
/*===========================================================================*/

int tcip_pkt_send(void *tcip_channel, unsigned port_index) /* send callback function */
{
	TCIP_CHANNEL_PTR_TYPE channel = (TCIP_CHANNEL_PTR_TYPE)tcip_channel;

	if ((port_index <= EDD_CFG_MAX_PORT_CNT) && is_not_null(channel) && (channel->state == TCIP_CH_OPEN))
	{
		ANYIP_IFNET_PTR ifp = (ANYIP_IFNET_PTR)channel->p_if_data->port[port_index].ifp;
		/* get send RQB from channel's rqb pool */
		EDD_UPPER_RQB_PTR_TYPE  pRQB = tcip_getEddSendRQB((EDD_UPPER_RQB_PTR_TYPE *)&(channel->edd.send_rqbs));

		/* if there is currently no free rqb pass packet to pktSendQueue of destination channel */
		if (is_not_null(pRQB))
		{ 
			ANYIP_PKTBUF_PTR packet = anyip_edd_get_next_send_pkt(channel, ifp);
			if (is_null(packet)) /* no more packets queued: return Rqb to free Rqb list! */
			{
				tcip_putEddSendRQB((EDD_UPPER_RQB_PTR_TYPE *)&(channel->edd.send_rqbs), pRQB);
			}
			else /* one more packet to send */
			{
				tcip_edd_nrt_send(channel, packet, pRQB);
			}

			return 0;
		}
		else
		{
			TCIP_UPPER_TRACE_00(0, LSA_TRACE_LEVEL_ERROR, "No free rqb to pass packet to pktSendQueue of destination channel!");
		}
	}

	return -1;
}

LSA_VOID tcip_edd_nrt_send_done( TCIP_EDD_LOWER_RQB_PTR_TYPE pRQB, TCIP_CHANNEL_CONST_PTR_TYPE channel )
{
#if TCIP_CFG_COPY_ON_SEND == 0
	EDD_UPPER_NRT_SEND_PTR_TYPE  pParam = TCIP_EDD_GET_PPARAM(pRQB, EDD_UPPER_NRT_SEND_PTR_TYPE);

	ANYIP_PKTBUF_PTR packet = (ANYIP_PKTBUF_PTR)TCIP_RQB_GET_USER_ID_PTR(pRQB);
	anyip_edd_free_pkt(packet);

	pParam->pBuffer = LSA_NULL;

	TCIP_EDD_SET_USER_ID_PTR(pRQB, LSA_NULL);
#endif

	TCIP_UPPER_TRACE_02(0, LSA_TRACE_LEVEL_CHAT, "pRQB: 0x%x, handle 0x%x", pRQB, TCIP_EDD_GET_HANDLE(pRQB));

	/***/

	if( TCIP_CH_OPEN == channel->state )
	{
#ifdef ANYIP_EDD_DO_NOT_GET_NEXT_SEND_PKT
		ANYIP_PKTBUF_PTR next_packet = LSA_NULL;
#else
		ANYIP_IFNET_PTR ifp = (ANYIP_IFNET_PTR)channel->p_if_data->port[0].ifp;
		ANYIP_PKTBUF_PTR next_packet = anyip_edd_get_next_send_pkt(channel, ifp);
#endif /* ANYIP_EDD_DO_NOT_GET_NEXT_SEND_PKT */

		if(is_null(next_packet)) /* no more packets queued: return Rqb to free Rqb list! */
		{
			tcip_putEddSendRQB((EDD_UPPER_RQB_PTR_TYPE *)&(channel->edd.send_rqbs), pRQB);
		}
		else /* one more packet to send */
		{
			tcip_edd_nrt_send(channel, next_packet, pRQB);
		}

		TCIP_TRIGGER_SELECT();
	}
	else /* return Rqb to free Rqb list; NOTE: EDD send resources are cancelled before calling back */
	{
		tcip_putEddSendRQB((EDD_UPPER_RQB_PTR_TYPE *)&(channel->edd.send_rqbs), pRQB);
	}
}

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
