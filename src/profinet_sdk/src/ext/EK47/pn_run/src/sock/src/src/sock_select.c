#define LTRC_ACT_MODUL_ID  19
#define SOCK_MODULE_ID     19

#include "sock_int.h"

#if SOCK_CFG_SNMP_ON && SOCK_CFG_USE_OBSD_SNMPD
#include "obsd_snmpd_snmpdext.h"
#endif

#if (ANYIP_CFG_USE_SOCKET_EVENTS == 0) /* OpenBSD Event Mechanism */
static LSA_VOID sock_socket_handle_except_mask(SOCK_SOCKET_PTR soc);
static LSA_VOID sock_socket_handle_read_mask(SOCK_SOCKET_PTR soc);
static LSA_VOID sock_socket_handle_write_mask(SOCK_SOCKET_PTR soc);
static LSA_VOID sock_socket_peek_connection(SOCK_SOCKET_PTR soc);
#endif /* OpenBSD Event Mechanism */

extern LSA_VOID sock_socket_handle_connect_error(SOCK_SOCKET_PTR soc, LSA_INT sock_error);


LSA_VOID
sock_doselect(
	SOCK_UPPER_RQB_PTR_TYPE rqb_ptr
) {
	SOCK_LOG_FCT("sock_doselect")

		SOCK_ENTER();

	SOCK_SYSTEM_TRACE_00(0, LSA_TRACE_LEVEL_CHAT
		, "Call from Select-Mimic"
	);
#if (ANYIP_CFG_USE_SOCKET_EVENTS == 0) /* OpenBSD Event Mechanism */
	sock_socket_do_select(rqb_ptr);
#else
	LSA_UNUSED_ARG(rqb_ptr);
#endif /* OpenBSD Event Mechanism */
	SOCK_EXIT();
}

#if (ANYIP_CFG_USE_SOCKET_EVENTS == 0) /* OpenBSD Event Mechanism */
/*------------------------------------------------------------------------------
// sock_socket_do_select
//----------------------------------------------------------------------------*/
LSA_VOID
sock_socket_do_select(
	SOCK_UPPER_RQB_PTR_TYPE rqb_ptr
) {
	SOCK_RQB_SET_RESPONSE(rqb_ptr, SOCK_RSP_OK_ACTIVE);

	sock_socket_execute_select(0);
}

/*------------------------------------------------------------------------------
// create read-write-except mask, call select, handle result
//----------------------------------------------------------------------------*/
LSA_VOID
sock_socket_execute_select(
	LSA_INT32 timeout
) {
	SOCK_LOG_FCT("sock_socket_execute_select")

	LSA_INT32 cnt = 0;
	LSA_INT32 i;

	sock_low_clear_all_sets();

	for (i = 0; i <= sock_data.active_sockets_last; i++)
	{
		SOCK_SOCKET_PTR soc = sock_data.active_sockets[i];

		if ((soc->state & SOCK_CN_STATE_ACTIVE_MASK) != 0) /* connected, connecting, listening */
		{
			if (sock_is_null(soc->open_close_rb))
			{
				if (sock_is_not_null(soc->recv.curr)
				   || (sock_data.do_tcp_disconnect_check /* only if timer has set flag, see AP01325244 */
				       && (soc->type == SOCK_LOW_STREAM)
					   && (soc->state == SOCK_CN_STATE_CONNECTED)
				      ) /* detect TCP RST and FIN, see AP01300966 */
#if SOCK_CFG_SNMP_ON && (SOCK_CFG_INTERNICHE) && (!SOCK_CFG_USE_OBSD_SNMPD)/* SNMP Agent Multiplexer */
				   || (sock_are_equal(soc, sock_data.snmp_dev) && (sock_data.snmp_pending_count == 0))
#endif
				   )
				{
					cnt++;
					sock_low_readset_insert(soc->lower_handle);
				}
			}

			if (sock_is_not_null(soc->accept.curr)
				&& (sock_data.socket_count < SOCK_CFG_MAX_SOCKETS)
				)
			{
				cnt++;
				sock_low_readset_insert(soc->lower_handle);
			}

			if(sock_is_not_null(soc->send.curr) /* write mask */
			   || sock_is_not_null(soc->connect_rb)
			   )
			{
				cnt++;
				sock_low_writeset_insert(soc->lower_handle);
			}

			if( (soc->type == SOCK_LOW_STREAM)/*!soc->sock_udp*/
				&& sock_data.do_tcp_disconnect_check
			  )  /* except mask for tcp sockets */
			{
				cnt++;
				sock_low_exceptset_insert(soc->lower_handle);
			}
		}
	}

	sock_socket_start_disconnect_timer();

	if( 0 == cnt )
	{
		return;
	}

	SOCK_PROTOCOL_TRACE_01(0, LSA_TRACE_LEVEL_CHAT, "number of events in masks = %d", cnt);

	{
	LSA_INT32     ret_val;
	ret_val = sock_low_select(timeout);

	if (SOCK_LOW_SOCKET_ERROR == ret_val)
	{
		return; /* error treatment*/
	}
	}

	cnt = 0;

	for (i=0; i<=sock_data.active_sockets_last; i++)
	{
		SOCK_SOCKET_PTR  soc = sock_data.active_sockets[i];

		if ((soc->state & SOCK_CN_STATE_ACTIVE_MASK) != 0)
		{
			if (sock_low_in_exceptset(soc->lower_handle)) /* (!) exception */
			{
				cnt++;
				sock_socket_handle_except_mask(soc);
				if(soc->type == SOCK_LOW_STREAM/*!soc->sock_udp*/)
				{
					continue; /* (!) do not check other masks */
				}
			}

			if (sock_low_in_readset(soc->lower_handle)) /* read mask */
			{
				cnt++;
				sock_socket_handle_read_mask(soc);
			}

			if (sock_low_in_writeset(soc->lower_handle)) /* write mask */
			{
				cnt++;
				sock_socket_handle_write_mask(soc);
			}
		}
	}

#if SOCK_CFG_SNMP_ON && SOCK_CFG_USE_OBSD_SNMPD
	obsd_snmpd_handle_events(0, LSA_NULL);
#endif

	SOCK_PROTOCOL_TRACE_01(0, LSA_TRACE_LEVEL_CHAT, "nunmber of signalled events in masks : %d", cnt);
}

/*------------------------------------------------------------------------------
// sock_socket_handle_except_mask():
// handle situation if socket is in except mask after select call
//----------------------------------------------------------------------------*/
static LSA_VOID
sock_socket_handle_except_mask(
	SOCK_SOCKET_PTR soc
) {
	SOCK_LOG_FCT("sock_socket_handle_except_mask")

		int sock_error;
	int native_error;

	sock_low_get_socket_error(soc->lower_handle, &sock_error, &native_error); /* erase error from socket */

	if (soc->type == SOCK_LOW_DGRAM)
	{
		return;
	}

	if (sock_is_not_null(soc->connect_rb)) /* connect failed -> return rqb */
	{
		sock_socket_handle_connect_error(soc, sock_error);
	}
	else if (sock_is_not_null(soc->accept.curr)) /* accept() failed -> try again */
	{
		/* if the stack stored the error in the listening socket (InterNiche does this) it was erased before (see getsockopt() above) */

		SOCK_LOWER_TRACE_03(0, LSA_TRACE_LEVEL_WARN
			, "listening socket handle(%d), lower_handle(0x%x), native_error(%d), n o t returning accept rqb"
			, soc->dev_handle, soc->lower_handle, sock_error
		);
	}
	else /* "normal" TCP socket (see above: if UDP -> return) -> cancel send and receive rqb's */
	{
		LSA_RESPONSE_TYPE response = (LSA_RESPONSE_TYPE)sock_low_map_tcp_error(sock_error);

		SOCK_LOWER_TRACE_03(0, LSA_TRACE_LEVEL_WARN
			, "exception on TCP socket handle(%d), lower_handle(0x%x), native_error(%d), returning all rqb's"
			, soc->dev_handle, soc->lower_handle, sock_error
		);

		sock_socket_return_all_resources(soc, response); /* -> CN_STATE_DISCONNECTING */
	}
}


/*------------------------------------------------------------------------------
// sock_socket_handle_read_mask():
// handle situation if socket is in read mask after select call
//----------------------------------------------------------------------------*/
static LSA_VOID
sock_socket_handle_read_mask(
	SOCK_SOCKET_PTR soc
) {
	SOCK_LOG_FCT("sock_socket_handle_read_mask")

		if (sock_is_not_null(soc->accept.curr)) /* client connect */
		{
			SOCK_PROTOCOL_TRACE_01(0, LSA_TRACE_LEVEL_CHAT
				, "listening socket_fd(0x%x) in read mask"
				, soc->lower_handle
			);

			sock_tcp_accept(soc);
		}
		else
		{
			if (soc->type == SOCK_LOW_STREAM /* tcp sockets: peek to detect TCP RST and FIN, see AP01300966 */
				&& sock_is_null(soc->recv.curr)
				)
			{
				sock_socket_peek_connection(soc);
			}
			else if (sock_is_not_null(soc->recv.curr))
			{
				SOCK_PROTOCOL_TRACE_01(0, LSA_TRACE_LEVEL_CHAT
					, "socket_fd(0x%x) in read mask"
					, soc->lower_handle
				);

				sock_socket_receive(soc);
			}
#if SOCK_CFG_SNMP_ON && (SOCK_CFG_INTERNICHE) && (!SOCK_CFG_USE_OBSD_SNMPD) /* SNMP Agent Multiplexer */
			else if (sock_are_equal(soc, sock_data.snmp_dev))  /* snmp device: read snmp request and dispatch request to snmp agent */
			{
				LSA_INT32 bytes_read = 0;

				LSA_RESPONSE_TYPE result = sock_udp_read(&bytes_read, soc, sock_data.snmp_recv_buf, SOCK_SNMP_BUFFER_SIZE, LSA_FALSE, &soc->rem_addr);

				if (result == SOCK_RSP_OK)
				{
					if (bytes_read > 0)
					{
						sock_data.snmp_pending_count = 1;

						sock_snmp_dispatch_request(sock_data.snmp_recv_buf, (LSA_UINT32)bytes_read);
					}
				}
				else
				{
					SOCK_LOWER_TRACE_01(0, LSA_TRACE_LEVEL_WARN, "error reading snmp socket, result(%d)", result);
				}
			}
#endif
		}
}

/*------------------------------------------------------------------------------
// sock_socket_handle_write_mask():
// handle situation if socket is in write mask after select call
//----------------------------------------------------------------------------*/
static LSA_VOID
sock_socket_handle_write_mask(
	SOCK_SOCKET_PTR soc
) {
	SOCK_LOG_FCT("sock_socket_handle_write_mask")

	if (sock_is_not_null(soc->connect_rb))
	{
		int sock_error;
		int native_error;

		sock_low_get_socket_error(soc->lower_handle, &sock_error, &native_error); /* check error condition on socket also, see AP01418351, AP01493449 */

		if (sock_error)
		{
			sock_socket_handle_connect_error(soc, sock_error);
		}
		else
		{
			sock_tcp_connect_client_socket(soc); /* conclude connect */
		}
	}
	else
	{
		SOCK_PROTOCOL_TRACE_02(0, LSA_TRACE_LEVEL_CHAT
			, "pending writes on lower_handle(0x%x), dev_handle(%d)"
			, soc->lower_handle, soc->dev_handle
		);

		sock_socket_send(soc);

		/***/

		if (sock_is_not_null(soc->open_close_rb) /* pending close active */
			&& sock_is_null(soc->send.curr)
			)
		{
			sock_socket_close(soc);
		}
	}
}

/*------------------------------------------------------------------------------
// sock_socket_peek_connection(): peeks into open socket to detect RST and FIN
//----------------------------------------------------------------------------*/
static LSA_VOID
sock_socket_peek_connection(
	SOCK_SOCKET_PTR soc
) {
	SOCK_LOG_FCT("sock_socket_peek_connection")

	char buf;
	LSA_INT result;
	LSA_INT native_error;

	/* note: remote abort detection will not work if socket has unread data */

	LSA_RESPONSE_TYPE response = (LSA_RESPONSE_TYPE)sock_low_recv(soc->lower_handle, &buf, sizeof(buf), SOCK_LOW_PEEK, &result, &native_error);

	if (response != SOCK_RSP_OK_ACTIVE) /* note: BSD Stack can return would block */
	{
		if (result == SOCK_LOW_SOCKET_ERROR /* RST */
			|| result == 0 /* FIN */
			)
		{
			SOCK_PROTOCOL_TRACE_03(0, LSA_TRACE_LEVEL_CHAT
				, "SOCKIF_Recv -> result(%d) for lower_handle(0x%x), handle(%d), return resources with ERR_REM_ABORT"
				, result, soc->lower_handle, soc->dev_handle
			);

			sock_socket_return_all_resources(soc, SOCK_RSP_ERR_REM_ABORT); /* see SOCK_OPC_EXCEPTION_NOTIFY */

			return;
		}
	}
}
#endif /* OpenBSD Event Mechanism */

#if (SOCK_CFG_USE_TCIP == 0) // NOT using TCIP

/*------------------------------------------------------------------------------
// trigger select with timeout parameter
//----------------------------------------------------------------------------*/
LSA_VOID
sock_doselect2(
	LSA_INT32 timeout
) {
	SOCK_LOG_FCT("sock_doselect2")

		SOCK_ENTER();

	SOCK_SYSTEM_TRACE_00(0, LSA_TRACE_LEVEL_CHAT, "Call from Select-Mimic");

	if (sock_data.socket_count > 0)
	{
#if (ANYIP_CFG_USE_SOCKET_EVENTS == 0) /* OpenBSD Event Mechanism */
		sock_socket_execute_select(timeout);
#else
		LSA_UNUSED_ARG(rqb_ptr);
#endif /* OpenBSD Event Mechanism */
	}

	SOCK_START_SELECT();

	SOCK_EXIT();
}

#endif /* (SOCK_CFG_USE_TCIP == 0) */


