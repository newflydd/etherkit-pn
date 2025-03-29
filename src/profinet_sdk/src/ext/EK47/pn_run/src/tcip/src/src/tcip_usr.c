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
/*  F i l e               &F: tcip_usr.c                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Implements the LSA user-interface                                        */
/*                                                                           */
/*****************************************************************************/
#define LTRC_ACT_MODUL_ID  2
#define TCIP_MODULE_ID     2

#include "tcip_int.h"

#if (TCIP_CFG_OPEN_BSD) && (OBSD_CFG_CLUSTER_IP_SUPPORTED == 1)
#include "obsd_pnio_cluster.h"
#endif
#include "anyip_tcip_interface.h"
#include "anyip_tcip_address.h"
#include "anyip_tcip.h"
#include "anyip_sys.h"

#if (TCIP_CFG_USE_OBSD_SNMPD == 1) || (TCIP_CFG_OPEN_BSD == 1)
#include "obsd_snmpd_snmpdext.h"
#endif

TCIP_FILE_SYSTEM_EXTENSION(TCIP_MODULE_ID) /* no semicolon */

#if (ANYIP_OK != TCIP_OK)
#error (incompatible TCIP_OK)
#endif

#if (ANYIP_ERR_RESOURCE != TCIP_ERR_RESOURCE)
#error (incompatible TCIP_ERR_RESOURCE)
#endif

#if (ANYIP_ERR_PARAM != TCIP_ERR_PARAM)
#error (incompatible TCIP_ERR_PARAM)
#endif

#if (ANYIP_ERR_LOWER_LAYER != TCIP_ERR_LOWER_LAYER)
#error (incompatible TCIP_ERR_LOWER_LAYER)
#endif

#if (ANYIP_TCIP_STATISTICS_PENDING_NO != 0)
#error (ANYIP_TCIP_STATISTICS_PENDING_NO must not be adapted)
#endif

#if (ANYIP_TCIP_STATISTICS_PENDING_YES != 1)
#error (ANYIP_TCIP_STATISTICS_PENDING_NO must not be adapted)
#endif

/*===========================================================================*/

/*------------------------------------------------------------------------------
//	sets the response of the given request-block and calls the
//        request-done-fct (given in TCIP_OPC_OPEN_CHANNEL)
//----------------------------------------------------------------------------*/
/*lint -e{818} symbol channel Pointer parameter 'Symbol' (Location) could be declared ptr to const */
LSA_VOID
tcip_callback( TCIP_CHANNEL_CONST_PTR_TYPE channel,
			   LSA_UINT16 response,
			   TCIP_UPPER_RQB_PTR_TYPE *upper_ptr )
{
	TCIP_LOG_FCT("tcip_callback")
	TCIP_UPPER_RQB_PTR_TYPE  rb;

	TCIP_ASSERT (is_not_null (upper_ptr));
	TCIP_ASSERT (is_not_null (*upper_ptr));

	rb = *upper_ptr;
	*upper_ptr = LSA_NULL;		/* sanity - never use this again */

	/*
	 *  memorize successful channel open
	 */
	if( TCIP_OK == response )
	{
		if( TCIP_OPC_OPEN_CHANNEL == TCIP_RQB_GET_OPCODE(rb))
		{
			tcip_data.tcip_channels_opened ++;
		}
	}

	TCIP_RQB_SET_HANDLE (rb, channel->handle_upper);
	TCIP_RQB_SET_RESPONSE (rb, response);

	TCIP_UPPER_TRACE_05 (channel->trace_idx, LSA_TRACE_LEVEL_NOTE
		, "<<< rqb(0x%08x) h(%u) opc(%u) rsp(%u) f-h(%u)"
		, rb, TCIP_RQB_GET_HANDLE (rb), TCIP_RQB_GET_OPCODE (rb), TCIP_RQB_GET_RESPONSE (rb), channel->my_handle
		);

	TCIP_REQUEST_UPPER_DONE (channel->callback, rb, channel->sysptr);
}

/*===========================================================================*/
/*===========================================================================*/

/*------------------------------------------------------------------------------
//	func: tcip_callback_and_free_channel: release all channel specific memory
//----------------------------------------------------------------------------*/

LSA_VOID tcip_callback_and_free_channel(
	TCIP_CHANNEL_CONST_PTR_TYPE channel,
	LSA_UINT16 response,
	TCIP_UPPER_RQB_PTR_TYPE rb
) {
	TCIP_LOG_FCT ("tcip_callback_and_free_channel")

	TCIP_UPPER_CALLBACK_FCT_PTR_TYPE  callback = channel->callback;
	TCIP_DETAIL_PTR_TYPE  detailptr = channel->detailptr;
	LSA_SYS_PTR_TYPE  sysptr = channel->sysptr;

	if (tcip_data.tcip_channels_opened > 0)
	{
		tcip_data.tcip_channels_opened --;
	}
	else
	{
		TCIP_FATAL();
	}

	/*
	 * NOTE: the previously used sequence was wrong!
	 *       > tcip_callback(channel, TCIP_OK, &rb);
	 *       > channel->state = TCIP_CST_FREE;
	 *
	 * ==> even if the callback posts into a queue, a call
	 *     to tcip_undo_init() can occur (due to preemption)
	 *     before we have the chance to reset the in-use flag
	 */

	TCIP_UPPER_TRACE_05 (channel->trace_idx, LSA_TRACE_LEVEL_NOTE
		, "<<< rqb(0x%08x) h(%u) opc(%u) rsp(%u) f-h(%u) channel-free"
		, rb, TCIP_RQB_GET_HANDLE (rb), TCIP_RQB_GET_OPCODE (rb), TCIP_RQB_GET_RESPONSE (rb), channel->my_handle
		);


	TCIP_RQB_SET_HANDLE (rb, channel->handle_upper);
	TCIP_RQB_SET_RESPONSE (rb, response);

#if TCIP_DEBUG
	TCIP_MEMSET(channel, 0xDD, sizeof(*channel)); /* wipe out */
#endif

	channel->state = TCIP_CH_FREE;				/* free the channel ... */

	TCIP_REQUEST_UPPER_DONE (callback, rb, sysptr); /* ... before calling back! */

	{
	LSA_UINT16  rc;
	TCIP_RELEASE_PATH_INFO (& rc, sysptr, detailptr);
	TCIP_ASSERT (rc == LSA_RET_OK);
	LSA_UNUSED_ARG(rc);
	}
}

/*===========================================================================*/
/*===========================================================================*/

/*
 *	get channel from handle or LSA_NULL if out-of-range
 */
TCIP_CHANNEL_PTR_TYPE tcip_channel_from_handle( LSA_HANDLE_TYPE handle)
{
	TCIP_LOG_FCT ("tcip_channel_from_handle")

	if( handle < TCIP_MAX_CHANNELS )
	{
		/* see tcip_open_channel() */
		return &tcip_data.tcip_channels[handle];
	}

	TCIP_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_ERROR, "handle(%u) out of range", handle);

	return LSA_NULL;
}

/*===========================================================================*/
/*===========================================================================*/

/*
 *	 returns arp channel pointer with passed group if found, otherwise LSA_NULL
 *   group id value must match, value itself not important
 */
static TCIP_CHANNEL_PTR_TYPE tcip_find_arp_channel_opened(LSA_UINT8 p_group_id )
{
	TCIP_CHANNEL_PTR_TYPE pChRet = LSA_NULL;
	LSA_HANDLE_TYPE hCh;

	for( hCh = 0; hCh < TCIP_MAX_CHANNELS; hCh++)
	{
		TCIP_CHANNEL_PTR_TYPE pCh = &tcip_data.tcip_channels[hCh];

		if( TCIP_CH_OPEN != pCh->state )
		{
			continue;
		}

		if( ( TCIP_PATH_TYPE_ARP == pCh->detailptr->path_type) &&
			( p_group_id == pCh->detailptr->group_id ) )
		{
			pChRet = pCh;
			break;
		}
	}
	return pChRet;
}

/*===========================================================================*/
/*===========================================================================*/

/*
 *  returns opt (ICMP, UDP or TCP) channel pointer with passed group id if found, otherwise LSA_NULL
 */
static TCIP_CHANNEL_PTR_TYPE tcip_find_opt_channel_opened(LSA_UINT8 p_group_id)
{
	TCIP_CHANNEL_PTR_TYPE pChRet = LSA_NULL;
	LSA_HANDLE_TYPE hCh;

	for( hCh = 0; hCh < TCIP_MAX_CHANNELS; hCh++)
	{
		TCIP_CHANNEL_PTR_TYPE pCh = &tcip_data.tcip_channels[hCh];
		LSA_UINT8 ch_path_type;

		if( TCIP_CH_OPEN != pCh->state )
		{
			continue;
		}

		ch_path_type = pCh->detailptr->path_type;

		if( ( p_group_id == pCh->detailptr->group_id  )
			&& ( TCIP_PATH_TYPE_ICMP == ch_path_type
				|| TCIP_PATH_TYPE_UDP == ch_path_type
				|| TCIP_PATH_TYPE_TCP == ch_path_type
				)
		  )

		{
			pChRet = pCh;
			break;
		}
	}
	return pChRet;
}

/*===========================================================================*/
/*===========================================================================*/

/*
 *  initialize send - and receive resources for one channel
 */
static LSA_VOID tcip_init_send_recv_resources(TCIP_CHANNEL_PTR_TYPE channel )
{
	TCIP_LOG_FCT("tcip_init_send_recv_resources")
	LSA_UINT32 i;
	LSA_UINT32 nr_of_recv = channel->detailptr->number_of_recv_resources;
	LSA_UINT32 nr_of_send = channel->detailptr->number_of_send_resources;

	channel->edd.pending_rcv_reqests = 0;   /* initial state*/

	/*
	 * initialize send resources for channel
	 */

	TCIP_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_CHAT, "providing %u send-resources", nr_of_send);

	tcip_initEddSendRQBList((EDD_UPPER_RQB_PTR_TYPE*)&( channel->edd.send_rqbs),
		               nr_of_send,
					   channel->edd.handle_lower,
					   channel->sysptr);


	TCIP_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_CHAT, "providing %u recv-resources", nr_of_recv);

	for ( i= 0; i < nr_of_recv; i++ )  /* provide receive resources for channel      */
	{
		tcip_AllocRcvBuf(channel);
	}
}

/*===========================================================================*/
/*===========================================================================*/

/*
 *  initialize arp channel: create interfaces and send-recv resources
 */
LSA_VOID tcip_init_arp_channel(TCIP_CHANNEL_CONST_PTR_TYPE channel)
{

	if( 0 == tcip_data.tcip_channels_opened ) /* first (of all) ARP channels */
	{
		/***/

#if ANYIP_CLOCK_RATE_HZ != 10
#error "check TCIP_ALLOC_TIMER(..., 100ms) and TCIP_START_TIMER(..., 1)"
#endif

		{
		LSA_USER_ID_TYPE uUserId;
		LSA_UINT16 retVal;
		uUserId.uvar32 = 0;
		TCIP_START_TIMER(&retVal, tcip_data.tcip_TimerID, uUserId, 1);
		TCIP_ASSERT(LSA_RET_OK == retVal);
		}
	}

	/***/

	channel->edd.pEdd = tcip_AllocRQB(channel->sysptr, sizeof(EDD_RQB_GET_STATISTICS_ALL_TYPE));

	if( is_null(channel->edd.pEdd) )
	{
		TCIP_FATAL(); /* a bug */
	}

	/***/

	tcip_create_and_activate_interfaces(channel);

	/***/

	tcip_edd_ext_link_status_provide(channel);

	tcip_init_send_recv_resources(channel);
}

/*===========================================================================*/
/*===========================================================================*/

/*
 *  initialize optional channels (icmp, udp and tcp):
 *  set interface pointer of arp channel;
 *  set channel pointer in net-structs of interface
 */
LSA_VOID tcip_init_opt_channel(TCIP_CHANNEL_CONST_PTR_TYPE channel)
{
	TCIP_CHANNEL_PTR_TYPE arp_channel = tcip_find_arp_channel_opened(channel->detailptr->group_id);

	if( is_null(arp_channel) )
	{
		TCIP_FATAL();
	}
	else
	{

		/*
		 * set interface struct pointer of arp channel for this channel also
		 */

		channel->p_if_data = arp_channel->p_if_data; /*all channel of one interface have the same pointer*/

		/***/

		tcip_init_send_recv_resources(channel);      /* init send-receive resources for new channel */
	}
}

/*===========================================================================*/
/*===========================================================================*/

/*
 *	deinit arp channel: remove interface entries, release interface struct
 */
static LSA_VOID tcip_deinit_arp_channel(TCIP_CHANNEL_PTR_TYPE channel)
{
	tcip_deactivate_and_delete_interfaces(channel);

	{
	LSA_UINT16 retval;
	TCIP_FREE_LOCAL_MEM(&retval, channel->p_if_data);
	TCIP_ASSERT(LSA_RET_OK == retval);
	}

	channel->p_if_data = LSA_NULL;

	/*
	 *  if last arp channel is closed, stop timer and clear reasm queue
	 */
	if( 1 == tcip_data.tcip_channels_opened )
	{
		{
		LSA_UINT16 retVal;
		TCIP_STOP_TIMER(&retVal, tcip_data.tcip_TimerID);
		TCIP_ASSERT(LSA_RET_OK == retVal);
		}
	//TCIP_IMPROVE: review stopping a cyclic timer, re-read prefix_sys.h

		/***/
	}
}

/*===========================================================================*/
/*===========================================================================*/

/*------------------------------------------------------------------------------
// reset channel's interface pointer
 //----------------------------------------------------------------------------*/
static LSA_VOID tcip_deinit_opt_channel(TCIP_CHANNEL_PTR_TYPE channel)
{
	/*
	 * set interface struct pointer to zero
	 */
	channel->p_if_data = LSA_NULL;
}

/*===========================================================================*/
/*===========================================================================*/

/*------------------------------------------------------------------------------
//	create a tcip instance and open a channel to the lower layer
//----------------------------------------------------------------------------*/
LSA_VOID tcip_open_channel ( TCIP_UPPER_RQB_PTR_TYPE  rb )
{
	TCIP_LOG_FCT("tcip_open_channel")

	TCIP_UPPER_OPEN_CHANNEL_PTR_TYPE open;
	TCIP_CHANNEL_PTR_TYPE channel;

	/* sanity checks */

	if (is_null (rb) || TCIP_RQB_GET_OPCODE (rb) != TCIP_OPC_OPEN_CHANNEL)
	{
		TCIP_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_FATAL, "no rqb(%08X) or wrong opcode or no args", rb);
		TCIP_FATAL(); /*lint -unreachable */
		return;
	}

#if (TCIP_DEBUG == 1)
	TCIP_RQB_SET_NEXT_RQB_PTR(rb, LSA_NULL); /* sanity */
	TCIP_RQB_SET_PREV_RQB_PTR(rb, LSA_NULL); /* sanity */
#endif

	open = &rb->args.open; /* shortcut */

	open->handle = TCIP_INVALID_HANDLE; /* sanity */

	if (open->tcip_request_upper_done_ptr == 0) /* AP01382171: dont' use macro is_null() */
	{
		TCIP_UPPER_TRACE_00(0, LSA_TRACE_LEVEL_FATAL, "no callback function given" );
		TCIP_FATAL(); /*lint -unreachable */
		return;
	}

	/* get path info and search a free tcip-handle*/

	channel = LSA_NULL;

	{
	TCIP_DETAIL_PTR_TYPE detailptr = LSA_NULL;
	LSA_SYS_PTR_TYPE sysptr;
	LSA_UINT16 rc;
	LSA_HANDLE_TYPE my_handle;
	LSA_RESPONSE_TYPE response;

	TCIP_GET_PATH_INFO (&rc, &sysptr, &detailptr, open->sys_path);

	if (rc != LSA_RET_OK || is_null (detailptr))
	{
		TCIP_UPPER_TRACE_02(0, LSA_TRACE_LEVEL_FATAL, "tcip_get_path_info: rc(%u) detailptr(%08x)", rc, detailptr);
		TCIP_FATAL(); /*lint -unreachable */
		return;
	}

	for (my_handle = 0; my_handle < TCIP_MAX_CHANNELS; ++my_handle)
	{
		channel = &tcip_data.tcip_channels[my_handle];
		if (channel->state == TCIP_CH_FREE) {
			#if TCIP_DEBUG
				TCIP_MEMSET(channel, 0xCC, sizeof(*channel)); /* to find uninitialized things */
				channel->state = TCIP_CH_FREE;
			#endif
			break; /* found */
		}
	}

	if ( my_handle >= TCIP_MAX_CHANNELS || is_null(channel) /* sanity */ )
	{
		TCIP_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_ERROR, "no free channel found, TCIP_MAX_CHANNELS(%u)", TCIP_MAX_CHANNELS );

		TCIP_RQB_SET_RESPONSE(rb, TCIP_ERR_RESOURCE);
		TCIP_RQB_SET_HANDLE(rb, open->handle_upper);
		TCIP_REQUEST_UPPER_DONE(open->tcip_request_upper_done_ptr, rb, sysptr);
		TCIP_RELEASE_PATH_INFO(&rc, sysptr, detailptr);
		TCIP_ASSERT(rc == LSA_RET_OK);
	}
	else if (detailptr->number_of_recv_resources == 0) /* receiving over all channels */
	{
		TCIP_UPPER_TRACE_02(0, LSA_TRACE_LEVEL_ERROR, "number_of_recv_resources 0 on path_type(%d), sys_path(%d)", detailptr->path_type, open->sys_path);

		TCIP_RQB_SET_RESPONSE(rb, TCIP_ERR_SYS_PATH);
		TCIP_RQB_SET_HANDLE(rb, open->handle_upper);
		TCIP_REQUEST_UPPER_DONE(open->tcip_request_upper_done_ptr, rb, sysptr);
		TCIP_RELEASE_PATH_INFO(&rc, sysptr, detailptr);
		TCIP_ASSERT(rc == LSA_RET_OK);
	}
	else
	{
		TCIP_CHANNEL_PTR_TYPE pChannelArp = tcip_find_arp_channel_opened(detailptr->group_id);  /*look for open ARP channel with group id of channel to be opened*/

		if (TCIP_PATH_TYPE_ARP == detailptr->path_type) /* ARP channel */
		{
			if (is_not_null(pChannelArp))
			{
				TCIP_UPPER_TRACE_02(0, LSA_TRACE_LEVEL_ERROR, "open ARP channel with same group_id(%d) found, sys_path(%d)", detailptr->group_id, open->sys_path);
				response = TCIP_ERR_SEQUENCE;
			}
			else if (detailptr->number_of_send_resources == 0)
			{
				TCIP_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_ERROR, "number_of_send_resources must not be 0 for ARP channel, sys_path(%d)", open->sys_path);
				response = TCIP_ERR_SYS_PATH;
			}
			else
			{
				response = TCIP_OK;
			}
		}
		else /* ICMP, UDP, TCP channel */
		{
			if (is_null(pChannelArp)) /* ARP-channel must be opened first for channel group, otherwise sequence error */
			{
				TCIP_UPPER_TRACE_02(0, LSA_TRACE_LEVEL_ERROR, "no open ARP channel with group_id(%d) found, sys_path(%d)", detailptr->group_id, open->sys_path);
				response = TCIP_ERR_SEQUENCE;
			}
			else if (detailptr->number_of_send_resources != 0) /* no sending over ICMP, UDP and TCP channel */
			{
				TCIP_UPPER_TRACE_02(0, LSA_TRACE_LEVEL_ERROR, "number_of_send_resources not 0 on path_type(%d), sys_path(%d)", detailptr->path_type, open->sys_path);
				response = TCIP_ERR_SYS_PATH;
			}
			else
			{
				response = TCIP_OK;
			}
		}

		if (response != TCIP_OK)
		{
			TCIP_RQB_SET_RESPONSE (rb, response);
			TCIP_RQB_SET_HANDLE (rb, open->handle_upper);
			TCIP_REQUEST_UPPER_DONE (open->tcip_request_upper_done_ptr, rb, sysptr);
			TCIP_RELEASE_PATH_INFO (&rc, sysptr, detailptr);
			TCIP_ASSERT (rc == LSA_RET_OK);
			return;
		}

		channel->state			= TCIP_CH_ALLOCATED;
		channel->my_handle		= my_handle;
		channel->handle_upper	= open->handle_upper;
		channel->syspath        = open->sys_path;
		channel->callback		= open->tcip_request_upper_done_ptr;
		channel->sysptr			= sysptr;
		channel->detailptr		= detailptr;
		channel->trace_idx		= 0; /* sanity, see below */
		channel->p_if_data      = LSA_NULL;

		/*
		 *   init lower data
		 */
		channel->edd.handle_lower = LSA_NULL;
		channel->edd.pending_rcv_reqests = 0;
		channel->edd.in_work_rcv_rqbs_number = 0;
		channel->edd.send_rqbs  = LSA_NULL;
		channel->edd.get_statistics_pending = LSA_FALSE;
		channel->edd.pEdd = LSA_NULL;

		TCIP_UPPER_TRACE_03(channel->trace_idx, LSA_TRACE_LEVEL_CHAT
			, "tcip_open_channel: h(%u) group_id(%u) path_type(%u)"
			, channel->my_handle, channel->detailptr->group_id, channel->detailptr->path_type
			);

		/***/

		open->handle = channel->my_handle;

		channel->curr = rb;

		/*
		 * open the lower layer
		 */

		anyip_init_channel((void *)channel);

		switch( channel->detailptr->path_type )
		{
		case TCIP_PATH_TYPE_ARP:
		case TCIP_PATH_TYPE_ICMP:
		case TCIP_PATH_TYPE_UDP:
		case TCIP_PATH_TYPE_TCP:
			tcip_open_channel_all(channel);
			break;

		default:
			TCIP_UPPER_TRACE_01(channel->trace_idx, LSA_TRACE_LEVEL_ERROR
				, "wrong path_type(%u)", channel->detailptr->path_type );

			tcip_callback_and_free_channel (channel, TCIP_ERR_SYS_PATH, channel->curr);
			break;
		}
	}
	}
}


/*------------------------------------------------------------------------------
//	close the channel after closing the channel to the lower layer
//----------------------------------------------------------------------------*/
LSA_VOID
tcip_close_channel (
	TCIP_UPPER_RQB_PTR_TYPE  rb
) {
	TCIP_LOG_FCT("tcip_close_channel")
	TCIP_CHANNEL_PTR_TYPE channel;

	/* sanity checks */

	TCIP_ASSERT (is_not_null (rb));

	channel = is_null(rb) ? LSA_NULL : tcip_channel_from_handle(TCIP_RQB_GET_HANDLE(rb));

	if (is_null(channel))
	{
		TCIP_UPPER_TRACE_02(0, LSA_TRACE_LEVEL_FATAL
		, "no rqb(%08X) or invalid handle(%u)", rb, (is_null(rb) ? 0 : TCIP_RQB_GET_HANDLE(rb)) );
		TCIP_FATAL(); /*lint -unreachable */
		return;
	}

#if TCIP_DEBUG
	TCIP_RQB_SET_NEXT_RQB_PTR(rb, LSA_NULL); /* sanity */
	TCIP_RQB_SET_PREV_RQB_PTR(rb, LSA_NULL); /* sanity */
#endif

	TCIP_UPPER_TRACE_03 (channel->trace_idx, LSA_TRACE_LEVEL_NOTE,
			">>> rqb(0x%08x) h(%u) opc(%u)",
			rb, TCIP_RQB_GET_HANDLE (rb), TCIP_RQB_GET_OPCODE (rb)
		);

	if ( ( TCIP_RQB_GET_OPCODE (rb) != TCIP_OPC_CLOSE_CHANNEL ) ||
		 ( channel->state != TCIP_CH_OPEN )  )
	{
		TCIP_UPPER_TRACE_02(channel->trace_idx, LSA_TRACE_LEVEL_ERROR,
			"h(%u) wrong opcode or state(%u) not ready",
			channel->my_handle, channel->state );
		tcip_callback (channel, TCIP_ERR_SEQUENCE, & rb);
		return;
	}

	channel->curr  = rb;

	/* sanity check: arp channel is the last one to be closed for one channel group */

	if( TCIP_PATH_TYPE_ARP == channel->detailptr->path_type )
	{
		TCIP_CHANNEL_PTR_TYPE pChannelOpt;

		pChannelOpt = tcip_find_opt_channel_opened(channel->detailptr->group_id);

		if( is_not_null(pChannelOpt) )   /* check: optional channel open for this group id ? --> sequence error */
		{
			TCIP_UPPER_TRACE_01(channel->trace_idx, LSA_TRACE_LEVEL_ERROR,
				"arp channel close: opt channel with same group_id(%d) found", channel->detailptr->group_id );
			tcip_callback(channel, TCIP_ERR_SEQUENCE, &channel->curr );
			return;
		}
	}

	/***/

	channel->state = TCIP_CH_CLOSING;
	TCIP_ASSERT(channel->edd.in_work_rcv_rqbs_number == 0);

	anyip_deinit_channel((void *)channel);

	/***/

	/* edd channel is closing -->  de-init interface functionality */

	if( TCIP_PATH_TYPE_ARP != channel->detailptr->path_type )
	{
		tcip_deinit_opt_channel(channel);
	}
	else
	{
		tcip_deinit_arp_channel(channel);

#if TCIP_CFG_ENABLE_MULTICAST
		/* remove entry for all hosts group (224.0.0.1 -> MAC: 01-00-5E-00-00-01 */
		{
		LSA_UINT64 address = 0x01005E000001;
		tcip_edd_srv_multicast(channel, 0, address, TCIP_SRV_MULTICAST_DISABLE);
		}
		return; /* not yet nrt_cancel, see tcip_edd_srv_multicast_done() */
#endif
	}

	/* close the lower layer; NOTE: EDD indication resources are cancelled before calling back */

	tcip_edd_nrt_cancel( channel );
}


/*------------------------------------------------------------------------------
//	receives a request from the user, dispatches it to the subsystem
//----------------------------------------------------------------------------*/
LSA_VOID
tcip_request (
	TCIP_UPPER_RQB_PTR_TYPE  rb
) {
	TCIP_LOG_FCT("tcip_request")

	if( is_null(rb) )
	{
		TCIP_FATAL();
	}
	else
	{
#if TCIP_DEBUG
		TCIP_RQB_SET_NEXT_RQB_PTR(rb, LSA_NULL); /* sanity */
		TCIP_RQB_SET_PREV_RQB_PTR(rb, LSA_NULL); /* sanity */
#endif

		if( TCIP_RQB_GET_OPCODE(rb) == TCIP_OPC_TIMER )
		{
			TCIP_EXCHANGE_LONG(&rb->args.timer.in_use, 0);
			anyip_ctick();
/* ANYIP_CFG_USE_SOCKET_EVENTS == 1 --> TRIGGER_SELECT is replaced by disconnect check timer in SOCK */
#if TCIP_CFG_SNMP_ON && ((TCIP_CFG_USE_OBSD_SNMPD == 1) || (TCIP_CFG_OPEN_BSD == 1))
			obsd_snmpd_handle_events(0, LSA_NULL); /* for routing messages */
#endif
		}
		else
		{
			TCIP_CHANNEL_PTR_TYPE channel = tcip_channel_from_handle(TCIP_RQB_GET_HANDLE(rb));

			if( is_null(channel) )
			{
				TCIP_UPPER_TRACE_02(0, LSA_TRACE_LEVEL_FATAL,
					"wrong handle(%u), opc(%u)",
					TCIP_RQB_GET_HANDLE (rb), TCIP_RQB_GET_OPCODE(rb)
					);

				TCIP_FATAL();
			}
			else
			{
				TCIP_UPPER_TRACE_03(channel->trace_idx, LSA_TRACE_LEVEL_NOTE,
					">>> rqb(0x%08x) h(%u) opc(%u)",
					rb, TCIP_RQB_GET_HANDLE (rb), TCIP_RQB_GET_OPCODE(rb)
					);

				TCIP_FATAL(); /* TCIP has no other opcodes */
			}
		}
	}
}

/*===========================================================================*/
/*===========================================================================*/

LSA_VOID tcip_create_and_activate_interfaces(TCIP_CHANNEL_PTR_TYPE arp_channel)
{
	TCIP_IF_DATA_PTR_TYPE p_if_data = arp_channel->p_if_data;
	LSA_UINT16 port_id;

	/*
	*  create and activate an interface and all associated ports (which are internal interfaces)
	*/
	for (port_id = 0/*!*/; port_id <= p_if_data->port_count; port_id++)
	{
		int ifAdminStatus;
		ANYIP_IFNET_PTR ifp = LSA_NULL; 

		if (port_id == 0) /* interface */
		{
			ifAdminStatus = 1; /* ifAdminStatus of interface is always up */
		}
		else /* ports */
		{
			ifAdminStatus = p_if_data->port[port_id].PhyStatus == EDD_PHY_STATUS_ENABLED ? ANYIP_STATE_UP : ANYIP_STATE_DOWN; /* see 1208466 */
		}

		ifp = anyip_tcip_create_and_activate_interface((void *)arp_channel, (char *)&p_if_data->port[port_id].MACAddr.MacAdr[0], ifAdminStatus, p_if_data->edd_interface_id, port_id);

		if (is_null(ifp))
		{
			TCIP_FATAL(); /* no interface can be created */
		}
		else
		{
			if (port_id == 0)
			{
				anyip_tcip_set_link_state(ifp, ANYIP_STATE_UP);
			}
			else
			{
				anyip_tcip_set_link_state(ifp, ANYIP_STATE_DOWN); /* ifOperStatus of ports is down until link indication, see tcip_edd_ext_link_status_indication() */
			}

			anyip_tcip_set_baudrate(ifp, (long)p_if_data->port[port_id].nominal_bandwidth);
			p_if_data->port[port_id].ifp = ifp;
			TCIP_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_NOTE, "Interface %u activated", anyip_tcip_get_if_index(ifp));
		}
	}
}

LSA_VOID tcip_deactivate_and_delete_interfaces(TCIP_CONST_CHANNEL_CONST_PTR_TYPE arp_channel)
{
	TCIP_IF_DATA_PTR_TYPE p_if_data = arp_channel->p_if_data;
	LSA_UINT16 port_id;

	/*
	*  reset all nets belonging to this interface
	*/
	for (port_id = 0/*!*/; port_id <= p_if_data->port_count; port_id++)
	{
		ANYIP_IFNET_PTR ifp = p_if_data->port[port_id].ifp;
		int if_index = anyip_tcip_get_if_index(ifp);

		if (anyip_tcip_deactivate_and_delete_interface(ifp) != 0)
		{
			TCIP_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_FATAL, "Error: could not deactivate and/or delete interface %u!", if_index);
			TCIP_FATAL();
		}

		TCIP_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_NOTE, "Interface %u deactivated and deleted", if_index);
	}
}

LSA_UINT8
tcip_set_ip_suite(
	LSA_UINT32  interface_id,
	LSA_UINT32	ip_address,
	LSA_UINT32	subnet_mask
)
{
	int i;

	/*
	*  find interface with matching interface id
	*/
	for (i = 0; i<TCIP_MAX_CHANNELS; i++)
	{
		TCIP_CHANNEL_PTR_TYPE ch;
		TCIP_IF_DATA_PTR_TYPE p_if_data;

		ch = &tcip_data.tcip_channels[i];

		if (TCIP_CH_OPEN != ch->state
			|| ch->detailptr->path_type != TCIP_PATH_TYPE_ARP
			)
		{
			continue;
		}

		TCIP_ASSERT(is_not_null(ch->p_if_data));
		p_if_data = ch->p_if_data;

		/*
		*  look for matching interface id
		*/
		if (interface_id == p_if_data->edd_interface_id)
		{
			int port;

			for (port = 0; port <= p_if_data->port_count; port++)
			{
				if (port == 0) /* OBSD_ITGR -- TCIP_IMPROVE: until SNMP needs it, set only on "port 0 = interface" */
				{
					ANYIP_IFNET_PTR ifp = p_if_data->port[port].ifp;
					int ret;

					if (ip_address == 0) /* IP-Address 0.0.0.0 means "no address" - e.g. for ResetToFactory */
					{
						ret = anyip_tcip_del_ipv4suite(ifp);
					}
					else
					{
						ret = anyip_tcip_set_ipv4suite(ifp, ip_address, subnet_mask);
					}

					if (ret != 0)
					{
						return TCIP_ERR_PARAM; /* not FATAL, see 1868581 */
					}
				}
			}

		}
	}

	return TCIP_OK;
}

LSA_UINT8
tcip_set_route(
	LSA_UINT32	ip_address,
	LSA_UINT32	subnet_mask,
	LSA_UINT32	gateway
)
{
	int ret = 0;

	if (ip_address || subnet_mask)
	{
		return TCIP_ERR_PARAM;
	}

	/* delete any existing route first */
	ret = anyip_tcip_del_route(ip_address, subnet_mask);

	if (gateway != 0)
	{
		ret = anyip_tcip_add_route(ip_address, subnet_mask, gateway);
	}

	if (ret != 0)
	{
		return TCIP_ERR_LOWER_LAYER; /* not FATAL, see 1868581 */
	}

	return TCIP_OK;
}

LSA_UINT8
tcip_set_ipv4_forwarding(
	LSA_UINT8	enable /* 0=disable, otherwise=enable */
)
{
	unsigned int tmp = enable ? 1 : 0; /* see obsd_set_ipv4forwarding() */
	int ret;

	ret = anyip_tcip_set_ipv4forwarding(tmp);

	if (ret != 0)
	{
		return TCIP_ERR_LOWER_LAYER;
	}

	return TCIP_OK;
}

LSA_UINT8
tcip_get_ipv4_forwarding(
	LSA_UINT8*	enable /* 0=disable, otherwise=enable */
)
{
	unsigned int tmp = 0;
	int ret;

	ret = anyip_tcip_get_ipv4forwarding(&tmp);

	if (ret != 0)
	{
		return TCIP_ERR_LOWER_LAYER;
	}

	*enable = (tmp == 1 ? 1 : 0); /* see obsd_get_ipv4forwarding() */

	return TCIP_OK;
}

/*===========================================================================*/
/*===========================================================================*/

/*
 * function:       returns the mac address for an existing network interface
 * documentation:  LSA_TCIP_Detailspec.doc
 */
LSA_UINT8
tcip_get_any_interface_mac_addr(
    LSA_UINT32* p_interface_id,
	LSA_UINT8*	p_mac_address,
	LSA_UINT16	mac_address_length
)
{
	int i;

	if( is_null(p_interface_id) )
	{
		return TCIP_ERR_PARAM;
	}

	if( is_null(p_mac_address) )
	{
		return TCIP_ERR_PARAM;
	}

	if( mac_address_length != 6 )
	{
		return TCIP_ERR_PARAM;
	}

	/*
	 *  find any interface
	 */
	for( i=0; i<TCIP_MAX_CHANNELS; i++ )
	{
		TCIP_CHANNEL_PTR_TYPE ch;

		ch = &tcip_data.tcip_channels[i];

		if( TCIP_CH_OPEN != ch->state )
		{
			continue;
		}

		TCIP_ASSERT(is_not_null(ch->p_if_data));

		*p_interface_id = ch->p_if_data->edd_interface_id;

		TCIP_MEMCPY(p_mac_address, ch->p_if_data->port[0/*!*/].MACAddr.MacAdr, mac_address_length);

		break;
	}

	if( TCIP_MAX_CHANNELS == i )  /* nothing found */
	{
		return TCIP_ERR_PARAM;
	}

	return TCIP_OK;
}

LSA_UINT16
tcip_set_timeparams(
	LSA_UINT32 keepalivetime,
	LSA_UINT32 connectiontimeout,
	LSA_UINT32 retransmittimeout,
	LSA_UINT32 retransmitcount,
	LSA_UINT32 no_change_value
) {
	/* note: range-check is done by caller */
#if (ANYIP_CFG_USE_TCPKEEPTIME == 1)
	int ret = 0;

	if (keepalivetime != no_change_value)
	{
		ret = anyip_tcip_set_tcp_keepidle((int)keepalivetime / 1000);

		if (ret != 0)
		{
			return LSA_RET_ERR_PARAM;
		}
	}


	if (connectiontimeout != no_change_value)
	{
		ret = anyip_tcip_set_tcp_keepinittime((int)connectiontimeout / 1000);
		if (ret != 0)
		{
			return LSA_RET_ERR_PARAM;
		}
	}

	if (retransmittimeout != no_change_value)
	{
		ret = anyip_tcip_set_tcp_keepintvl((int)retransmittimeout / 1000);
		if (ret != 0)
		{
			return LSA_RET_ERR_PARAM;
		}

		ret = anyip_tcip_set_tcp_rexmtmax((int)retransmittimeout / 1000);
		if (ret != 0)
		{
			return LSA_RET_ERR_PARAM;
		}
	}

	if (retransmitcount != no_change_value)
	{
		ret = anyip_tcip_set_tcp_keepcount((int)retransmitcount);
		if (ret != 0)
		{
			return LSA_RET_ERR_PARAM;
		}
	}
#else /* (ANYIP_CFG_USE_TCPKEEPTIME == 1) */
	LSA_UNUSED_ARG(keepalivetime);
	LSA_UNUSED_ARG(connectiontimeout);
	LSA_UNUSED_ARG(retransmittimeout);
	LSA_UNUSED_ARG(retransmitcount);
	LSA_UNUSED_ARG(no_change_value);
#endif /* (ANYIP_CFG_USE_TCPKEEPTIME == 1) */

	return LSA_RET_OK;
}

LSA_UINT16
tcip_get_timeparams(
	LSA_UINT32 *p_keepalivetime,
	LSA_UINT32 *p_connectiontimeout,
	LSA_UINT32 *p_retransmittimeout,
	LSA_UINT32 *p_retransmitcount
) {
#if (ANYIP_CFG_USE_TCPKEEPTIME == 1)
	*p_keepalivetime = (LSA_UINT32)anyip_tcip_get_tcp_keepidle() * 1000;
	*p_connectiontimeout = (LSA_UINT32)anyip_tcip_get_tcp_keepinittime() * 1000;
	*p_retransmittimeout = (LSA_UINT32)anyip_tcip_get_tcp_keepintvl() * 1000;
	*p_retransmitcount = (LSA_UINT32)anyip_tcip_get_tcp_keepcount();
	return LSA_RET_OK;

#else  /* (ANYIP_CFG_USE_TCPKEEPTIME == 1) */
	LSA_UNUSED_ARG(*p_keepalivetime);
	LSA_UNUSED_ARG(*p_connectiontimeout);
	LSA_UNUSED_ARG(*p_retransmittimeout);
	LSA_UNUSED_ARG(*p_retransmitcount);
	return LSA_RET_ERR_PARAM;
#endif /* (ANYIP_CFG_USE_TCPKEEPTIME == 1) */
}



#if TCIP_CFG_ENABLE_MULTICAST

/*---------------------------------------------------
// enable / disable multicast mac in EDDx MAC table
//--------------------------------------------------*/
LSA_RESPONSE_TYPE
tcip_srv_multicast(
	LSA_INT32 dev_handle,
	LSA_UINT32 interface_id,
	LSA_UINT32 mc_address, /* (!) network byte order */
	LSA_UINT8 mode
) {
	TCIP_LOG_FCT("tcip_srv_multicast")

	int i;
	TCIP_CHANNEL_PTR_TYPE ch = LSA_NULL;

	LSA_UINT64 multicast_address = 0x01005E000000;
	LSA_UINT32 mask = 0x007FFFFF;

	multicast_address |= (TCIP_HTONL(mc_address) & mask);

	/* find EDDx channel for local address */

	for( i=0; i<TCIP_MAX_CHANNELS; i++ )
	{
		ch = &tcip_data.tcip_channels[i];

		if( TCIP_CH_OPEN == ch->state
			&& ch->detailptr->path_type == TCIP_PATH_TYPE_ARP
			&& is_not_null(ch->p_if_data)
			)
		{
			if (ch->p_if_data->edd_interface_id == interface_id)
			{
				break; /* found */
			}
		}
	}

	if( i == TCIP_MAX_CHANNELS ) /* not found */
	{
		return TCIP_ERR_PARAM;
	}

	/***/

	/* add entry for group specified by user */

	tcip_edd_srv_multicast(ch, dev_handle, multicast_address, mode);

	return TCIP_OK;
}

#if (TCIP_CFG_OPEN_BSD) && (OBSD_CFG_CLUSTER_IP_SUPPORTED == 1) /* Cluster IP */

LSA_RESPONSE_TYPE tcip_srv_cluster_ip_config_set(
	LSA_UINT32 InterfaceID,
	LSA_UINT8  ClusterID,
	LSA_UINT32 ClusterIP,
	LSA_UINT32 ClusterSubnetMask
) {
	int i;
	TCIP_CHANNEL_PTR_TYPE ch = LSA_NULL;

	/* find EDDx channel for local address */

	for (i = 0; i<TCIP_MAX_CHANNELS; i++)
	{
		ch = &tcip_data.tcip_channels[i];

		if (TCIP_CH_OPEN == ch->state
			&& ch->detailptr->path_type == TCIP_PATH_TYPE_ARP
			&& is_not_null(ch->p_if_data)
			)
		{
			if (ch->p_if_data->edd_interface_id == InterfaceID)
			{
				break; /* found */
			}
		}
	}

	if (i == TCIP_MAX_CHANNELS) /* not found */
	{
		return TCIP_ERR_PARAM;
	}

	return (LSA_RESPONSE_TYPE)obsd_pnio_set_cluster_ip((ANYIP_IFNET_PTR)ch->p_if_data->port[0].ifp, InterfaceID, ClusterIP, ClusterSubnetMask, ClusterID);
}

LSA_RESPONSE_TYPE tcip_srv_cluster_ip_state_set(
	LSA_UINT32 InterfaceID,
	LSA_UINT8 ClusterIPState
) {
	int i;
	TCIP_CHANNEL_PTR_TYPE ch = LSA_NULL;

	/* find EDDx channel for local address */

	for (i = 0; i<TCIP_MAX_CHANNELS; i++)
	{
		ch = &tcip_data.tcip_channels[i];

		if (TCIP_CH_OPEN == ch->state
			&& ch->detailptr->path_type == TCIP_PATH_TYPE_ARP
			&& is_not_null(ch->p_if_data)
			)
		{
			if (ch->p_if_data->edd_interface_id == InterfaceID)
			{
				break; /* found */
			}
		}
	}

	if (i == TCIP_MAX_CHANNELS) /* not found */
	{
		return TCIP_ERR_PARAM;
	}

	if (obsd_pnio_set_cluster_state((ANYIP_IFNET_PTR)ch->p_if_data->port[0].ifp, ClusterIPState) == -1)
	{
		TCIP_UPPER_TRACE_01(ch->trace_idx, LSA_TRACE_LEVEL_WARN
			, "obsd_pnio_set_carp_role failed for interface %i"
			, anyip_tcip_get_if_index((ANYIP_IFNET_PTR)ch->p_if_data->port[0].ifp)
		);
		return TCIP_ERR_LOWER_LAYER;
	}
	else
	{
		return TCIP_OK;
	}
}

LSA_UINT8 tcip_srv_cluster_ip_state_get(
	LSA_UINT32 InterfaceID
) {
	int i;
	TCIP_CHANNEL_PTR_TYPE ch = LSA_NULL;
	int retval = -1;

	/* find EDDx channel for local address */

	for (i = 0; i<TCIP_MAX_CHANNELS; i++)
	{
		ch = &tcip_data.tcip_channels[i];

		if (TCIP_CH_OPEN == ch->state
			&& ch->detailptr->path_type == TCIP_PATH_TYPE_ARP
			&& is_not_null(ch->p_if_data)
			)
		{
			if (ch->p_if_data->edd_interface_id == InterfaceID)
			{
				break; /* found */
			}
		}
	}

	if (i == TCIP_MAX_CHANNELS) /* not found */
	{
		return TCIP_ERR_PARAM;
	}

	retval = obsd_pnio_get_cluster_state((ANYIP_IFNET_PTR)ch->p_if_data->port[0].ifp);

	switch (retval) {
	case 1:
		return TCIP_CLUSTER_IP_STATE_ACTIVE;
	case 0:
		return TCIP_CLUSTER_IP_STATE_INACTIVE;
	default:
		TCIP_UPPER_TRACE_01(ch->trace_idx, LSA_TRACE_LEVEL_WARN
			, "obsd_pnio_get_carp_role failed for interface %i"
			, anyip_tcip_get_if_index((ANYIP_IFNET_PTR)ch->p_if_data->port[0].ifp));

		return TCIP_CLUSTER_IP_STATE_INACTIVE;
	}

}
#endif /* Cluster IP */

#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
