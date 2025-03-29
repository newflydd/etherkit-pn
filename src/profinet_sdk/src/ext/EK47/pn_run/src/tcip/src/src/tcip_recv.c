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
/*  F i l e               &F: tcip_recv.c                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  NRT receive functionality                                                */
/*                                                                           */
/*****************************************************************************/
#define LTRC_ACT_MODUL_ID  5
#define TCIP_MODULE_ID     5

#include "tcip_int.h"
#include "anyip_edd_recv.h"

TCIP_FILE_SYSTEM_EXTENSION(TCIP_MODULE_ID) /* no semicolon */

/*===========================================================================*/

#define TCIP_RECV_BUFFER_SIZE  EDD_FRAME_BUFFER_LENGTH /* see EDD_SRV_NRT_RECV */

/*===========================================================================*/

LSA_VOID tcip_AllocRcvBuf(TCIP_CHANNEL_CONST_PTR_TYPE channel)
{
	EDD_UPPER_RQB_PTR_TYPE pRQB = tcip_AllocRQB( channel->sysptr, sizeof(EDD_RQB_NRT_RECV_TYPE));

	if( is_null(pRQB) )
	{
		TCIP_FATAL(); /* a bug */
	}
	else
	{
		EDD_UPPER_MEM_PTR_TYPE pRxFrame;

		TCIP_EDD_ALLOC_RX_TRANSFER_BUFFER_MEM(&pRxFrame, TCIP_RECV_BUFFER_SIZE, channel->sysptr);

		if (is_null(pRxFrame))
		{
			TCIP_FATAL();
		}
		else
		{
			EDD_UPPER_NRT_RECV_PTR_TYPE pParam = TCIP_EDD_GET_PPARAM( pRQB, EDD_UPPER_NRT_RECV_PTR_TYPE );

			pParam->pBuffer = pRxFrame;
			pParam->RequestID  = 0; /* not used */
			pParam->RequestCnt = 0;

			TCIP_EDD_SET_USER_ID_PTR(pRQB, LSA_NULL); /* unused */

			TCIP_EDD_SET_OPCODE( pRQB, EDD_OPC_REQUEST, EDD_SRV_NRT_RECV);
			TCIP_EDD_SET_HANDLE( pRQB, 0);
			EDD_RQB_SET_HANDLE_LOWER( pRQB, channel->edd.handle_lower);

			tcip_edd_nrt_recv(pRQB, channel);
		}
	}
}

LSA_VOID tcip_FreeRcvBuf(TCIP_CONST_CHANNEL_PTR_TYPE channel, TCIP_EDD_LOWER_RQB_TYPE  *pRQB)
{
	EDD_UPPER_NRT_RECV_PTR_TYPE     pParam = LSA_NULL;
	LSA_UINT16                      ret16_val;

	pParam = TCIP_EDD_GET_PPARAM( pRQB, EDD_UPPER_NRT_RECV_PTR_TYPE );

	if ( is_not_null(pParam) )
	{
		TCIP_EDD_LOWER_MEM_PTR_TYPE pFrame;

		pFrame = pParam->pBuffer;

		if ( is_not_null(pFrame) )
		{
			TCIP_EDD_FREE_RX_TRANSFER_BUFFER_MEM(&ret16_val, pFrame, channel->sysptr);
			TCIP_ASSERT(ret16_val == LSA_RET_OK);
		}
	}

	tcip_FreeRQB(channel->sysptr, pRQB );
}

/*===========================================================================*/
/*===========================================================================*/

LSA_VOID tcip_edd_nrt_recv_done ( TCIP_EDD_LOWER_RQB_TYPE  *pRQB, TCIP_CHANNEL_CONST_PTR_TYPE channel )
{
	TCIP_LOG_FCT("tcip_edd_nrt_recv_done")

	LSA_UINT16 response = TCIP_EDD_GET_RESPONSE(pRQB);

	if (channel->edd.pending_rcv_reqests > 0)
	{
		channel->edd.pending_rcv_reqests --;
	}
	else
	{
		TCIP_FATAL();
	}

	if( TCIP_CH_OPEN != channel->state ) /* closing */
	{
		TCIP_ASSERT(response == EDD_STS_OK || response == EDD_STS_OK_CANCEL);

		tcip_FreeRcvBuf( channel, pRQB);
	}
	else if( response != EDD_STS_OK )
	{
		TCIP_FATAL(); /* not as documented */
	}
	else /* ok */
	{
		EDD_UPPER_NRT_RECV_PTR_TYPE pRecvParam = TCIP_EDD_GET_PPARAM(pRQB, EDD_UPPER_NRT_RECV_PTR_TYPE);
		ANYIP_IFNET_PTR ifp = channel->p_if_data->port[0/*!*/].ifp; /* not: pRecvParam->PortID, see AP00819817 */
		ANYIP_PKTBUF_PTR pkt = LSA_NULL;

		if( pRecvParam->FrameFilter == EDD_NRT_FRAME_ARP )
		{
			/* peek at "opcode" at offset 6, 16 bits in network byte order (ARP reply = 2, see arp.h, struct arp_hdr) */

			if( pRecvParam->UserDataLength >= 8 )
			{
				LSA_UINT8 *op = &pRecvParam->pBuffer[pRecvParam->UserDataOffset + 6];

				if( op[0] == 0 && op[1] == 2 )
				{
					TCIP_ARP_RESPONSE_RECEIVED(channel->detailptr->group_id, pRecvParam->pBuffer, pRecvParam->IOCount);
				}
			}
		}

		/***/

		if( pRecvParam->FrameFilter == EDD_NRT_FRAME_IP_TCP ) /* see 948166, do not allow TCP frames with broadcast bit */
		{
			LSA_UINT8 mac1 = pRecvParam->pBuffer[ANYIP_ETHHDR_BIAS];

			if (mac1 & 0x01) /* check broadcast bit */
			{
				tcip_edd_nrt_recv(pRQB, channel); /* reprovide */

				return;
			}
		}

		pkt = anyip_edd_copy_recv_buffer(ifp, (char *)pRecvParam->pBuffer, (int)pRecvParam->IOCount);

		/* packet is copied to buf, return RQB to EDD */
		tcip_edd_nrt_recv(pRQB, channel);

		if (pkt)
		{
			anyip_edd_receive(ifp, pkt);
			TCIP_TRIGGER_SELECT();
		}
	}
}

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
