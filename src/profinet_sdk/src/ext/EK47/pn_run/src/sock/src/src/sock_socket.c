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
/*  C o m p o n e n t     &C: SOCK (SOCKet interface)                   :C&  */
/*                                                                           */
/*  F i l e               &F: sock_socket.c                             :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*  Productive part of the Socket-Interface                                  */
/*                                                                           */
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID 12
#define SOCK_MODULE_ID    12

#include "sock_int.h"

#if (ANYIP_CFG_USE_SOCKET_EVENTS == 1)
#include "obsd_pnio_socket_events.h"
#endif

SOCK_FILE_SYSTEM_EXTENSION(SOCK_MODULE_ID)

/*===========================================================================*/

/*------------------------------------------------------------------------------
// allocate socket structure for the specified open socket descriptor
//----------------------------------------------------------------------------*/
SOCK_SOCKET_PTR
sock_socket_alloc(
	SOCK_CHANNEL_PTR_TYPE channel,
	SOCK_LOW_HANDLE_TYPE lower_handle,
	LSA_INT32 type,
	SOCKET_OPTION *specific_socket_option,
	SOCKET_OPTION_VAL_TYPE socket_option_value
) {
	SOCK_SOCKET_PTR soc = LSA_NULL;

	/* set socket options based on channel detail info of sysadaption */
	LSA_INT option_ret_val = sock_socket_set_options(channel, lower_handle, type);

#if (SOCK_CFG_OPEN_BSD)
	/* setting socket specific options only supported for OpenBSD stack */
	if (option_ret_val == SOCK_RSP_OK)
	{
		/* Set specific socket_options received from user rqb, if not NULL and valid */
		/* note: overruling the option settings from detail info                     */
		option_ret_val = sock_socket_set_specific_options(lower_handle, specific_socket_option, socket_option_value);
	}
#else
	/* following arguments are only used in the OBSD context */
	LSA_UNUSED_ARG(specific_socket_option);
	LSA_UNUSED_ARG(socket_option_value);
#endif

	if (option_ret_val != SOCK_RSP_OK)
	{
		sock_low_close(lower_handle);
		return LSA_NULL;
	}

	if( sock_data.free_head == SOCK_CFG_MAX_SOCKETS) /* end of free list */
	{
		sock_low_close(lower_handle);
		return LSA_NULL;
	}
	else
	{
		soc = &sock_data.sockets[sock_data.free_head];
		sock_data.free_head = soc->free_next;

		soc->lower_handle = lower_handle;
		soc->type = type;

#if (ANYIP_CFG_USE_SOCKET_EVENTS == 1) /* OpenBSD Event Mechanism */
		obsd_pnio_init_socket_events((unsigned long)soc->lower_handle, soc);
#else
		/* AP01316092, allow optimization. See also sock_socket_execute_select() */
		sock_data.active_sockets_last++;
		sock_data.active_sockets[sock_data.active_sockets_last] = soc; /* removed in sock_socket_free() */
		soc->active_handle = sock_data.active_sockets_last; /* store index, see sock_socket_free() */
#endif

		soc->state = SOCK_CN_STATE_DISABLED; /* initial state */
#if (ANYIP_CFG_USE_SOCKET_EVENTS == 1) /* OpenBSD Event Mechanism */
		soc->server_state = SOCK_CN_STATE_DISABLED; /* initial listening socket state, see 2004942 */
#endif

		soc->channel = channel;

		soc->loc_addr.sin_family = 0;
		soc->loc_addr.sin_port = 0;
		soc->loc_addr.sin_addr.s_addr = 0;
		{
		int i;
		for(i=0; i<8; i++)
		{
			soc->loc_addr.sin_zero[i] = 0;
		}
		}
		soc->rem_addr = soc->loc_addr;

		soc->mcast_interface_id = 0;
		soc->mcast_address = 0;

		soc->abort_resp = 0;

		soc->send.bytes_transferred = 0;
		sock_queue_init(&soc->send.queue);
		soc->send.curr = LSA_NULL;

		soc->recv.bytes_transferred = 0;
		sock_queue_init(&soc->recv.queue);
		soc->recv.curr = LSA_NULL;

		soc->connect_rb = LSA_NULL;
		soc->open_close_rb = LSA_NULL;

		sock_queue_init(&soc->accept.queue);
		soc->accept.curr = LSA_NULL;

		soc->exception_rb = LSA_NULL;

		sock_data.socket_count++;

		return soc;
	}
}

/*------------------------------------------------------------------------------
// free socket structure
//----------------------------------------------------------------------------*/
LSA_VOID
sock_socket_free(
	SOCK_SOCKET_PTR soc
) {
	SOCK_LOG_FCT("sock_socket_free")
#if (ANYIP_CFG_USE_SOCKET_EVENTS == 1) /* OpenBSD Event Mechanism */
		obsd_pnio_deinit_socket_events((unsigned long)soc->lower_handle);
#else
	/* AP01316092, allow optimization */
	if (soc->active_handle == sock_data.active_sockets_last) /* last entry */
	{
		 sock_data.active_sockets_last--;
	}
	else if (sock_data.active_sockets_last > 0 ) /* between first and last: overwrite with last entry  */
	{
		sock_data.active_sockets[soc->active_handle] = sock_data.active_sockets[sock_data.active_sockets_last];
		sock_data.active_sockets_last--;
		sock_data.active_sockets[soc->active_handle]->active_handle = soc->active_handle; /* (!) adapt active_handle for moved entry */
	}
	else if (sock_data.active_sockets_last < 0)
	{
		SOCK_FATAL();  /* (!) cannot delete from empty array */
	}
#endif

	soc->channel = LSA_NULL;

	if (soc->lower_handle != SOCK_LOW_INVALID_HANDLE)
	{
		sock_low_close(soc->lower_handle); /* soclose sets callback-function to NULL --> no more events are handled */

		soc->lower_handle = SOCK_LOW_INVALID_HANDLE;
	}

	sock_data.socket_count--;

	soc->free_next = sock_data.free_head;
	sock_data.free_head = soc->dev_handle;

	soc->state = SOCK_CN_STATE_FREE;
}


/*------------------------------------------------------------------------------
// get socket structure pointer from index ("handle")
//----------------------------------------------------------------------------*/
SOCK_SOCKET_PTR
sock_socket_from_handle(
	LSA_INT32 handle
) {
	if( handle >= SOCK_CFG_MAX_SOCKETS )
	{
		return LSA_NULL;
	}

	if( sock_data.sockets[handle].state == SOCK_CN_STATE_FREE )
	{
		return LSA_NULL;
	}

	return &sock_data.sockets[handle];
}


/*------------------------------------------------------------------------------
// get socket structure pointer from index and check channel assignment
//----------------------------------------------------------------------------*/
SOCK_SOCKET_PTR
sock_socket_from_channel(
	SOCK_CHANNEL_CONST_PTR_TYPE channel,
	LSA_INT32 handle
) {
	SOCK_SOCKET_PTR soc = sock_socket_from_handle(handle);

	if( sock_is_null(soc))
	{
		return LSA_NULL;
	}
	else if( !sock_are_equal(channel, soc->channel) )
	{
		return LSA_NULL;
	}
	else
	{
		return soc;
	}
}

/*------------------------------------------------------------------------------
// sock_socket_handle_connect_error():
// return mapped connect response and close socket if possible
//----------------------------------------------------------------------------*/
LSA_VOID
sock_socket_handle_connect_error(
	SOCK_SOCKET_PTR soc,
	LSA_INT sock_error
) {
	SOCK_LOG_FCT("sock_socket_handle_connect_error")

	LSA_OPCODE_TYPE opc = SOCK_RQB_GET_OPCODE(soc->connect_rb); /* (!) */

	LSA_RESPONSE_TYPE response = (LSA_RESPONSE_TYPE)sock_low_map_connect_error(sock_error);

	SOCK_PROTOCOL_TRACE_04(0, LSA_TRACE_LEVEL_NOTE
		, "connect failed for socket handle(%d), lower_handle(0x%x),native_error(%d), response(%d)"
		, soc->dev_handle, soc->lower_handle, sock_error, response
		);

	sock_usr_callback(soc->channel, response, &soc->connect_rb);

	sock_socket_return_exception_resource(soc, response);

	if( opc == SOCK_OPC_CONNECT ) /* close socket here since user has no handle */
	{
		sock_socket_free(soc);
	}
	else
	{
		soc->state = SOCK_CN_STATE_OPENED; /* reset to opened state */
	}
}

/*------------------------------------------------------------------------------
// sock_socket_start_disconnect_timer():
// resets sock_data.do_tcip_disconnect_check and starts the disconnect timer (1 s)
// Note: sock_data.do_tcip_disconnect_check is set to TRUE when the timer is
//       triggered.
//----------------------------------------------------------------------------*/
LSA_VOID sock_socket_start_disconnect_timer(LSA_VOID)
{
	if (sock_data.do_tcp_disconnect_check) /* reset flag, see AP01325244 */
	{
		LSA_UINT16 ret;
		LSA_USER_ID_TYPE user_id;

		sock_data.do_tcp_disconnect_check = LSA_FALSE;

		user_id.uvar32 = 0;

		SOCK_START_TIMER(&ret, sock_data.tcp_disconnect_check_timer_id, user_id, 1);

		if (ret != LSA_RET_OK)
		{
			SOCK_FATAL();
		}
	}
}

/*------------------------------------------------------------------------------
// sock_socket_close
// close rb present - return if possible
//----------------------------------------------------------------------------*/
LSA_VOID
sock_socket_close(
SOCK_SOCKET_PTR soc
) {
	SOCK_LOG_FCT("sock_socket_close")

#if SOCK_CFG_ENABLE_MULTICASTS
	if ( soc->mcast_interface_id != 0 ) /* release mcast resource */
	{
		LSA_RESPONSE_TYPE resp = tcip_srv_multicast(soc->dev_handle
							, soc->mcast_interface_id
							, soc->mcast_address
							, TCIP_SRV_MULTICAST_DISABLE
							);

		if( resp != TCIP_OK )
		{
			SOCK_FATAL(); /* must not fail */
		}

		soc->state = SOCK_CN_STATE_DISABLED; /* (!) must not use this socket any more */
		/*sado -- if state is disabled, events in eventlist are not handled (see sock_socket_do_events) */
	}
	else
#endif
	{
		SOCK_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_CHAT
			, "socket handle(%d) closed by application"
			, soc->dev_handle
			);

		if( soc->state != SOCK_CN_STATE_DISCONNECTING ) /* if not pending close */
		{
			sock_socket_return_all_resources(soc, SOCK_RSP_ERR_LOC_ABORT);
		}

		sock_usr_callback(soc->channel, SOCK_RSP_OK, &soc->open_close_rb);

		sock_socket_free(soc);
	}
}


/*------------------------------------------------------------------------------
// sock_socket_receive() :called if
// (1) resource was provided by user (receive request) or (!)
// (2) when select returns and resource is available
//----------------------------------------------------------------------------*/
LSA_VOID
sock_socket_receive(
	SOCK_SOCKET_PTR soc
) {
	while( sock_is_not_null(soc->recv.curr) )
	{
		LSA_RESPONSE_TYPE read_response;

		if( soc->recv.curr->args.data.data_length == 0 ) /* 0 byte read always ok */
		{
			read_response = SOCK_RSP_OK;
		}
		else if( soc->type == SOCK_LOW_DGRAM )
		{
			read_response = sock_udp_receive(soc, &(soc->recv.curr->args.data));
		}
		else
		{
			read_response = sock_tcp_receive(soc, &(soc->recv.curr->args.data));
		}

		if( read_response == SOCK_RSP_OK ) /* return rqb and look for next in queue */
		{
			soc->recv.bytes_transferred = 0; /* reset when rqb returns */

			sock_usr_callback(soc->channel, read_response, &soc->recv.curr); /* soc->recv.curr <- LSA_NULL */

			soc->recv.curr = sock_queue_dequeue(&soc->recv.queue);

		}
		else if( read_response == SOCK_RSP_OK_ACTIVE ) /*  would block */
		{
			break;
		}
		else /* return rqb and all resources with error (TCP only) */
		{
			sock_socket_return_all_resources(soc, read_response); /* sets socket to SOCK_CN_STATE_DISCONNECTING */

			break; /* read error on socket -> no reading from this socket */
		}
	}
}


/*------------------------------------------------------------------------------
// sock_socket_send(): called when
//	(1) send rqb was provided or
//  (2) socket is ready to write after select and send rqb is available
//----------------------------------------------------------------------------*/
LSA_VOID
sock_socket_send(
	SOCK_SOCKET_PTR soc
) {
	SOCK_LOG_FCT("sock_socket_send")

	while( sock_is_not_null(soc->send.curr) )
	{
		LSA_RESPONSE_TYPE send_response;

		if( soc->type == SOCK_LOW_DGRAM )
		{
			send_response = sock_udp_send(soc, &(soc->send.curr->args.data));
		}
		else /* tcp */
		{
			if( soc->send.curr->args.data.data_length == 0 ) /* 0 byte write always ok */
			{
				send_response = SOCK_RSP_OK;
			}
			else
			{
				send_response = sock_tcp_send(soc, &(soc->send.curr->args.data));

				if (send_response != SOCK_RSP_OK && send_response != SOCK_RSP_OK_ACTIVE) /* send error on tcp socket -> no sending with this socket */
				{
					sock_socket_return_all_resources(soc, send_response); /* sets socket to SOCK_CN_STATE_DISCONNECTING */

					break;
				}
			}
		}

		if( send_response == SOCK_RSP_OK_ACTIVE ) /* would block */
		{
			break;
		}

		/* return rqb and look for next in queue */

		soc->send.bytes_transferred = 0; /* reset when rqb returns */

		sock_usr_callback(soc->channel, send_response, &soc->send.curr); /* (!) sets soc->send.curr to LSA_NULL */

		soc->send.curr = sock_queue_dequeue(&soc->send.queue);
	}
}


/*------------------------------------------------------------------------------
// set socket struct state to SOCK_CN_STATE_DISCONNECTING
// and pass back all user resources
//----------------------------------------------------------------------------*/
LSA_VOID
sock_socket_return_all_resources(
	SOCK_SOCKET_PTR dev,
	LSA_UINT16 abort_resp
) {
	SOCK_LOG_FCT("sock_socket_return_all_resources")

	dev->abort_resp = abort_resp; /* used for response if rqb arrives for this socket  */

	dev->send.bytes_transferred = 0;
	dev->recv.bytes_transferred = 0;

	dev->state = SOCK_CN_STATE_DISCONNECTING; /* socket is about to close (user must send SOCK_OPC_CLOSE) and has no resources */


#if (SOCK_CFG_SNMP_ON) && (SOCK_CFG_INTERNICHE) && (!SOCK_CFG_USE_OBSD_SNMPD) /* SNMP Agent Multiplexer */

	if (sock_are_equal(dev, sock_data.snmp_dev))
	{
		if (sock_data.snmp_extern_count == 0)
		{
			sock_snmp_deactivate_int();
		}
		else
		{
			sock_data.snmp_intern_auto = LSA_TRUE;
		}
		return;
	}

#endif

	/* pass back all receive rqbs */
	sock_socket_return_receive_resources(dev, abort_resp);

	/* pass back all send rqbs */
	sock_socket_return_send_resources(dev, abort_resp);

	/* pass back connect rqb */
	sock_socket_return_connect_resource(dev, abort_resp);

	/* pass back accept notify rqbs */
	sock_socket_return_accept_notify_resources(dev, abort_resp);

	/* pass back except_notify rqb */
	sock_socket_return_exception_resource(dev, abort_resp);

	return;
}


/*------------------------------------------------------------------------------
// return send resources if present
//----------------------------------------------------------------------------*/
LSA_VOID
sock_socket_return_send_resources(
	SOCK_SOCKET_PTR soc,
	LSA_UINT16 response
) {
	SOCK_LOG_FCT("sock_socket_return_send_resources")

	if( sock_is_not_null(soc->send.curr))
	{
		SOCK_UPPER_RQB_PTR_TYPE rb;

		SOCK_PROTOCOL_TRACE_04(0, LSA_TRACE_LEVEL_CHAT
			, "confirm send rqb(0x%x), socket handle(0x%x), port(%d), error(0x%x)"
			, soc->send.curr, soc->dev_handle, SOCK_NTOHS(soc->loc_addr.sin_port), response
			);

		sock_usr_callback(soc->channel, response, &soc->send.curr);

		rb = sock_queue_dequeue(&soc->send.queue);

		while ( sock_is_not_null(rb) )
		{
			SOCK_PROTOCOL_TRACE_04(0, LSA_TRACE_LEVEL_CHAT
				, "confirm send rqb(0x%x), socket handle(0x%x), port(%d), error(0x%x)"
				, rb, soc->dev_handle, SOCK_NTOHS(soc->loc_addr.sin_port), response
				);

			sock_usr_callback(soc->channel, response, &rb);

			rb = sock_queue_dequeue(&soc->send.queue);
		}
	}
}


/*------------------------------------------------------------------------------
// return receive resources if present
//----------------------------------------------------------------------------*/
LSA_INT
sock_socket_return_receive_resources(
	SOCK_SOCKET_PTR soc,
	LSA_UINT16 response
) {
	SOCK_LOG_FCT("sock_socket_return_receive_resources")

	LSA_INT num = 0;

	if(sock_is_not_null(soc->recv.curr))
	{
		SOCK_UPPER_RQB_PTR_TYPE rb;

		SOCK_PROTOCOL_TRACE_04(0, LSA_TRACE_LEVEL_NOTE_LOW
			, "confirm receive rqb(0x%x), socket handle(0x%x), port(%d), error(0x%x)"
			, soc->recv.curr, soc->dev_handle, SOCK_NTOHS(soc->loc_addr.sin_port), response
			);

		sock_usr_callback(soc->channel, response, &soc->recv.curr);

		num++;

		rb = sock_queue_dequeue(&soc->recv.queue);

		while ( sock_is_not_null(rb) )
		{
			SOCK_PROTOCOL_TRACE_04(0, LSA_TRACE_LEVEL_NOTE_LOW
				, "confirm receive rqb(0x%x), socket handle(0x%x), port(%d), error(0x%x)"
				, rb, soc->dev_handle, SOCK_NTOHS(soc->loc_addr.sin_port), response
				);

			sock_usr_callback(soc->channel, response, &rb);

			num++;

			rb = sock_queue_dequeue(&soc->recv.queue);
		}
	}

	return num;
}


/*------------------------------------------------------------------------------
// return connect resource if present
//----------------------------------------------------------------------------*/
LSA_VOID
sock_socket_return_connect_resource(
	SOCK_SOCKET_PTR soc,
	LSA_UINT16 response
) {
	SOCK_LOG_FCT("sock_socket_return_connect_resource")

	if ( sock_is_not_null(soc->connect_rb) ) /* pass back connect_rb if present */
	{
		SOCK_PROTOCOL_TRACE_04(0, LSA_TRACE_LEVEL_CHAT
			, "SOCK_OPC_CONNECT(0x%x), socket handle(0x%x), port(%d), error(0x%x)"
			, soc->connect_rb, soc->dev_handle, SOCK_NTOHS(soc->loc_addr.sin_port), response
			);

		sock_usr_callback(soc->channel, response, &soc->connect_rb);
	}
}


/*------------------------------------------------------------------------------
// return accept notify resource if present
//----------------------------------------------------------------------------*/
LSA_VOID
sock_socket_return_accept_notify_resources(
	SOCK_SOCKET_PTR soc,
	LSA_UINT16 response
) {
	if(sock_is_not_null(soc->accept.curr))
	{
		SOCK_UPPER_RQB_PTR_TYPE rb;

		sock_usr_callback(soc->channel, response, &soc->accept.curr);

		rb = sock_queue_dequeue(&soc->accept.queue);

		while ( sock_is_not_null(rb) ) /* return pending SOCK_OPC_ACCEPT_NOTIFY */
		{
			sock_usr_callback(soc->channel, response, &rb);

			rb = sock_queue_dequeue(&soc->accept.queue);
		}
	}
}


/*------------------------------------------------------------------------------
// return except resource if present
//----------------------------------------------------------------------------*/
LSA_VOID
sock_socket_return_exception_resource(
	SOCK_SOCKET_PTR soc,
	LSA_UINT16 response
) {
	SOCK_LOG_FCT("sock_socket_return_exception_resource")

	if ( sock_is_not_null(soc->exception_rb) )
	{
		SOCK_PROTOCOL_TRACE_04(0, LSA_TRACE_LEVEL_NOTE_LOW
			, "SOCK_OPC_EXCEPT_NOTIFY(0x%x), socket handle(0x%x), port(%d), error(0x%x)"
			, soc->exception_rb, soc->dev_handle, SOCK_NTOHS(soc->loc_addr.sin_port), response
			);

		sock_usr_callback(soc->channel, response, &soc->exception_rb);
	}
}

#if (SOCK_CFG_OPEN_BSD)
/*------------------------------------------------------------------------------
// Sets the specific socket options for the specified open socket descriptor according
// to option info from user (contained in rqb)
//----------------------------------------------------------------------------*/
LSA_RESPONSE_TYPE
sock_socket_set_specific_options(
	SOCK_LOW_HANDLE_TYPE lower_handle,
	SOCKET_OPTION *specific_socket_option,
	SOCKET_OPTION_VAL_TYPE socket_option_value
) {
	SOCK_LOG_FCT("sock_socket_set_specific_options")

	LSA_INT32 option_val;
	LSA_INT native_error;

	// No socket option provided by user
	if ( specific_socket_option == 0)
	{
		// return without any action LSA_TRACE_LEVEL_NOTE
		SOCK_UPPER_TRACE_00(0, LSA_TRACE_LEVEL_NOTE, "sock_socket_set_specific_options(specific_socket_option not provided(=0), no action!");
		return SOCK_RSP_OK;
	}

	// option value signals no change of option
	if (socket_option_value == USE_CHANNEL_SETTING )
	{
		// return without any action
		SOCK_UPPER_TRACE_00(0, LSA_TRACE_LEVEL_NOTE, "sock_socket_set_specific_options(specific_socket_option_value == USE_CHANNEL_SETTING), no action necessary!");
		return SOCK_RSP_OK;
	}

	/* Plausibility check for specific socket option settings */
	if (
		(socket_option_value == ENABLE                     || socket_option_value == DISABLE ) &&
		(specific_socket_option->SO_REUSEADDR_ == LSA_TRUE || specific_socket_option->SO_LINGER_ == LSA_TRUE )
		)
	{
		/* Set socket options provided by user request */

		if (specific_socket_option->SO_REUSEADDR_ == LSA_TRUE)
		{
			/* set SO_REUSEADDR_ option according to info provided by the user */
			if (socket_option_value == ENABLE)
			{
				option_val = 1;
			}
			else
			{
				option_val = 0;
			}

			SOCK_UPPER_TRACE_02(0, LSA_TRACE_LEVEL_NOTE
				, "sock_socket_set_specific_options(lower_handle(%d), option=sock_low_sockopt_ReuseAddr, option_val(%d)), Enable/Disable REUSE_ADDR socket option"
				, lower_handle, option_val);

			if (!sock_low_setsockopt(lower_handle, sock_low_sockopt_ReuseAddr, &option_val, &native_error))
			{
				SOCK_UPPER_TRACE_03(0, LSA_TRACE_LEVEL_ERROR
					, "error sock_low_setsockopt(lower_handle(%d), option=sock_low_sockopt_ReuseAddr, option_val(%d), native_error(%d)"
					, lower_handle, option_val, native_error
				);

				return SOCK_RSP_ERR_LOC_NET;
			}
		}

		else if (specific_socket_option->SO_LINGER_ == LSA_TRUE)
		{
			/* set SO_LINGER_ option according to info provided by the user */
			if (socket_option_value == ENABLE)
			{
				/* ENABLE graceful_shutdown */
				/* Deactivate linger option, linger_time have to be 0 for non_blocking mode */
				int linger_time = 0;

				SOCK_UPPER_TRACE_02(0, LSA_TRACE_LEVEL_NOTE
					, "sock_socket_set_specific_options(lower_handle(%d), option=sock_low_sockopt_Linger_off, option_val(%d)), Enable GRACEFUL_SHUTDOWN(LINGER) socket option"
					, lower_handle, socket_option_value);
				if (!sock_low_setsockopt(lower_handle, sock_low_sockopt_Linger_off, &linger_time, &native_error))
				{
					SOCK_UPPER_TRACE_03(0, LSA_TRACE_LEVEL_ERROR
						, "error sock_low_setsockopt(lower_handle(%d), option=sock_low_sockopt_Linger_off, linger_time(%d), native_error(%d)"
						, lower_handle, linger_time, native_error
					);
					return SOCK_RSP_ERR_LOC_NET;
				}
			}
			else
			{
				/* DISABLE graceful_shutdown */
				/* specific_socket_option_value == DISABLE */
				/* Set linger option, linger_time have to be 0 for non_blocking mode */
				int linger_time = 0;

				SOCK_UPPER_TRACE_02(0, LSA_TRACE_LEVEL_NOTE
					, "sock_low_setsockopt(lower_handle(%d), option=sock_low_sockopt_Linger, option_val(%d), Disable GRACEFUL_SHUTDOWN(LINGER) socket option"
					, lower_handle, socket_option_value);

				if (!sock_low_setsockopt(lower_handle, sock_low_sockopt_Linger, &linger_time, &native_error))
				{
					SOCK_UPPER_TRACE_03(0, LSA_TRACE_LEVEL_ERROR
						, "error sock_low_setsockopt_off(lower_handle(%d), option=sock_low_sockopt_Linger, linger_time(%d), native_error(%d)"
						, lower_handle, linger_time, native_error
					);

					return SOCK_RSP_ERR_LOC_NET;
				}
			}
		}
	}
	else
	{
		/* invalid specific socket option setting received from user */
		SOCK_UPPER_TRACE_03(0, LSA_TRACE_LEVEL_WARN
			, "sock_socket_set_specific_options(specific_socket_option_value(%d), ReuseAddress(%d), Linger(%d), specific option settings not valid, ignored!"
			, (int)socket_option_value, specific_socket_option->SO_REUSEADDR_, specific_socket_option->SO_LINGER_);
	}

	return SOCK_RSP_OK;
}
#endif // SOCK_CFG_OPEN_BSD

/*------------------------------------------------------------------------------
// Sets the socket options for the specified open socket descriptor according
// to details pointers to send_option_val, rcv_option_val, udp may be NULL
//----------------------------------------------------------------------------*/
LSA_RESPONSE_TYPE
sock_socket_set_options(
	SOCK_CHANNEL_CONST_PTR_TYPE channel,
	SOCK_LOW_HANDLE_TYPE lower_handle,
	LSA_INT32 type
) {
	SOCK_LOG_FCT("sock_socket_set_options")

	LSA_INT32 option_val;
	LSA_INT native_error;

	/* switch socket to non-blocking mode */

	option_val = 1;
	if (!sock_low_setsockopt(lower_handle, sock_low_sockopt_NonBlocking, &option_val, &native_error))
	{
		SOCK_UPPER_TRACE_03(0, LSA_TRACE_LEVEL_ERROR
							, "error sock_low_setsockopt(lower_handle(%d), option=sock_low_sockopt_NonBlocking, option_val(%d), native_error(%d)"
							, lower_handle, option_val, native_error
							);

		return SOCK_RSP_ERR_LOC_NET;
	}

	if ( (type == SOCK_LOW_STREAM) && channel->detail_ptr->socket_option.SO_TCPNODELAY_) /* don't delay sending data */
	{
		option_val = 1;
		if (!sock_low_setsockopt(lower_handle, sock_low_sockopt_TCPNoDelay, &option_val, &native_error))
		{
			SOCK_UPPER_TRACE_03(0, LSA_TRACE_LEVEL_ERROR
								, "error sock_low_setsockopt(lower_handle(%d), option=sock_low_sockopt_TCPNoDelay, option_val(%d), native_error(%d)"
								, lower_handle, option_val, native_error
								);

			return SOCK_RSP_ERR_LOC_NET;
		}
	}

	if ( (type == SOCK_LOW_STREAM) && channel->detail_ptr->socket_option.SO_TCPNOACKDELAY_) /* don't delay sending ACKs */
	{
		option_val = 1;
		if (!sock_low_setsockopt(lower_handle, sock_low_sockopt_TCPNoAckDelay, &option_val, &native_error))
		{
			SOCK_UPPER_TRACE_03(0, LSA_TRACE_LEVEL_ERROR
								, "error sock_low_setsockopt(lower_handle(%d), option=sock_low_sockopt_TCPNoAckDelay, option_val(%d), native_error(%d)"
								, lower_handle, option_val, native_error
								);

			return SOCK_RSP_ERR_LOC_NET;
		}
	}

	if (channel->detail_ptr->socket_option.SO_REUSEADDR_)
	{
		option_val = 1;
		if (!sock_low_setsockopt(lower_handle, sock_low_sockopt_ReuseAddr, &option_val, &native_error))
		{
			SOCK_UPPER_TRACE_03(0, LSA_TRACE_LEVEL_ERROR
								, "error sock_low_setsockopt(lower_handle(%d), option=sock_low_sockopt_ReuseAddr, option_val(%d), native_error(%d)"
								, lower_handle, option_val, native_error
								);

			return SOCK_RSP_ERR_LOC_NET;
		}
	}

	if (type == SOCK_LOW_STREAM) /* set keep alive as requested (in all cases!) AP00938830 */
	{
		if(SOCK_KEEPALIVE_ACTIVE/* == LSA_TRUE */)
		{
			option_val = 1;
		}
		else
		{
			option_val = 0;
		}

		if (!sock_low_setsockopt(lower_handle, sock_low_sockopt_KeepAlive, &option_val, &native_error))
		{
			SOCK_UPPER_TRACE_03(0, LSA_TRACE_LEVEL_ERROR
								, "error sock_low_setsockopt(lower_handle(%d), option=sock_low_sockopt_KeepAlive, option_val(%d), native_error(%d)"
								, lower_handle, option_val, native_error
								);

			return SOCK_RSP_ERR_LOC_NET;
		}

		option_val = 1;
		if (!sock_low_setsockopt(lower_handle, sock_low_sockopt_OOBInline, &option_val, &native_error))
		{
			SOCK_UPPER_TRACE_03(0, LSA_TRACE_LEVEL_ERROR
								, "error sock_low_setsockopt(lower_handle(%d), option=sock_low_sockopt_OOBInline, option_val(%d), native_error(%d)"
								, lower_handle, option_val, native_error
								);

			return SOCK_RSP_ERR_LOC_NET;
		}
	}

	option_val = 1;
	if (channel->detail_ptr->socket_option.SO_DONTROUTE_)
	{
		if (!sock_low_setsockopt(lower_handle, sock_low_sockopt_DontRoute, &option_val, &native_error))
		{
			SOCK_UPPER_TRACE_03(0, LSA_TRACE_LEVEL_ERROR
								, "error sock_low_setsockopt(lower_handle(%d), option=sock_low_sockopt_DontRoute, option_val(%d), native_error(%d)"
								, lower_handle, option_val, native_error
								);

			return SOCK_RSP_ERR_LOC_NET;
		}
	}

	if (channel->detail_ptr->socket_option.SO_SNDBUF_)
	{
		option_val = channel->detail_ptr->send_buffer_max_len;
		if (!sock_low_setsockopt(lower_handle, sock_low_sockopt_SendBuffer, &option_val, &native_error))
		{
			SOCK_UPPER_TRACE_03(0, LSA_TRACE_LEVEL_ERROR
								, "error sock_low_setsockopt(lower_handle(%d), option=sock_low_sockopt_SendBuffer, option_val(%d), native_error(%d)"
								, lower_handle, option_val, native_error
								);

			return SOCK_RSP_ERR_LOC_NET;
		}
	}

	if (channel->detail_ptr->socket_option.SO_RCVBUF_)
	{
		option_val =  channel->detail_ptr->rec_buffer_max_len;
		if (!sock_low_setsockopt(lower_handle, sock_low_sockopt_RecvBuffer, &option_val, &native_error))
		{
			SOCK_UPPER_TRACE_03(0, LSA_TRACE_LEVEL_ERROR
								, "error sock_low_setsockopt(lower_handle(%d), option=sock_low_sockopt_RecvBuffer, option_val(%d), native_error(%d)"
								, lower_handle, option_val, native_error
								);

			/* Note: To be able to accept half open connections on server side, this error return must be ignored. */
			/*       Such a half open connection occurs, when a client opens a connection, sends its data and      */
			/*       then closes connection with FIN (shutdown the send direction) but stays available for receive */
			/*       until the server side also closes the connection. For details see RQ 4707601.                 */
			/* return SOCK_RSP_ERR_LOC_NET; */
		}
	}

	if (channel->detail_ptr->socket_option.SO_LINGER_)
	{
		int linger_time = (int)channel->detail_ptr->linger_time;

		if (!sock_low_setsockopt(lower_handle, sock_low_sockopt_Linger, &linger_time, &native_error))
		{
			SOCK_UPPER_TRACE_03(0, LSA_TRACE_LEVEL_ERROR
								, "error sock_low_setsockopt(lower_handle(%d), option=sock_low_sockopt_Linger, option_val(%d), native_error(%d)"
								, lower_handle, option_val, native_error
								);

			return SOCK_RSP_ERR_LOC_NET;
		}
	}

	if (type == SOCK_LOW_DGRAM) /* SO_BROADCAST applies only to datagram sockets */
	{
		if (channel->detail_ptr->socket_option.SO_BROADCAST_)
		{
			option_val = 1;

			if (!sock_low_setsockopt(lower_handle, sock_low_sockopt_Broadcast, &option_val, &native_error))
			{
				SOCK_UPPER_TRACE_03(0, LSA_TRACE_LEVEL_ERROR
									, "error sock_low_setsockopt(lower_handle(%d), option=sock_low_sockopt_Broadcast, option_val(%d), native_error(%d)"
									, lower_handle, option_val, native_error
									);

				return SOCK_RSP_ERR_LOC_NET;
			}
		}
	}

#if (SOCK_CFG_OPEN_BSD) && (OBSD_CFG_CLUSTER_IP_SUPPORTED == 1) /* Cluster IP*/
	if (type == SOCK_LOW_DGRAM)
	{
		option_val = 1;
		if (!sock_low_setsockopt(lower_handle, sock_low_sockopt_IPRecvdStAddr, &option_val, &native_error))
		{
			SOCK_UPPER_TRACE_03(0, LSA_TRACE_LEVEL_ERROR
								, "error sock_low_setsockopt(lower_handle(%d), option=sock_low_sockopt_IPRecvdStAddr, option_val(%d), native_error(%d)"
								, lower_handle, option_val, native_error
								);

			return SOCK_RSP_ERR_LOC_NET;
		}
	}
#endif /* Cluster IP */

#if defined SIO_UDP_CONNRESET
#if (SOCK_CFG_WINSOCK)  /* check if option available */
	/*
	Windows XP: Controls whether UDP PORT_UNREACHABLE messages are reported. Set to TRUE to enable reporting. Set to FALSE to disable reporting.
	SIO_UDP_CONNRESET
	*/
	if (type == SOCK_LOW_DGRAM)
	{
		option_val = 0; /* LSA_FALSE */
		if (!sock_low_setsockopt(lower_handle, sock_low_sockopt_Connreset, &option_val, &native_error))
		{
			SOCK_UPPER_TRACE_03(0, LSA_TRACE_LEVEL_ERROR
								, "error sock_low_setsockopt(lower_handle(%d), option=sock_low_sockopt_Connreset, option_val(%d), native_error(%d)"
								, lower_handle, option_val, native_error
								);

			return SOCK_RSP_ERR_LOC_NET;
		}
	}
#endif
#endif

	return SOCK_RSP_OK;
}


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
