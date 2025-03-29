#define LTRC_ACT_MODUL_ID 20
#define SOCK_MODULE_ID    20

#include "sock_int.h"
#if (ANYIP_CFG_USE_SOCKET_EVENTS == 1) /* OpenBSD Event Mechanism */
#include "obsd_pnio_socket_events.h"

LSA_VOID sock_socket_do_connected_event(SOCK_SOCKET_PTR soc_ptr);

/*-----------------------------------------------------------------------------
// treat "readable" event on socket; disconnect makes socket "readable" also
//----------------------------------------------------------------------------*/
static LSA_VOID
sock_socket_do_readable_event(
	SOCK_SOCKET_PTR soc
) {
	SOCK_LOG_FCT("sock_socket_do_readable_event")

	if (sock_is_null(soc->open_close_rb))
	{
		if (sock_is_not_null(soc->recv.curr))
		{
			sock_socket_receive(soc);
		}
		else if (sock_is_not_null(soc->connect_rb))
		{
			sock_socket_do_connected_event(soc); /* ETIMEDOUT when connecting, see 1442237 */
		}
		else if (soc->type == SOCK_LOW_STREAM
			&& soc->state == SOCK_CN_STATE_CONNECTED
			)
		{
			char buf;
			LSA_INT result;
			LSA_INT native_error;
			LSA_RESPONSE_TYPE response;

			SOCK_PROTOCOL_TRACE_02(0, LSA_TRACE_LEVEL_CHAT
				, "(!)no read rqb socket lower_handle(0x%08x) handle(%d), -> peek socket and retrieve possible socket error"
				, soc->lower_handle
				, soc->dev_handle
			);

			/* note: remote abort detection will not work if socket has unread data */

			response = (LSA_RESPONSE_TYPE)sock_low_recv(soc->lower_handle
				, &buf
				, sizeof(buf)
				, SOCK_LOW_PEEK
				, &result
				, &native_error
			);

			if (response != SOCK_RSP_OK_ACTIVE) /* SOCK_RSP_OK or mapped error code */
			{
				if (result == 0) /* FIN */
				{
					SOCK_PROTOCOL_TRACE_03(0, LSA_TRACE_LEVEL_CHAT
						, "SOCKIF_Recv -> result(%d) for lower_handle(0x%x), handle(%d), graceful shutdown detected, return resources with ERR_REM_ABORT"
						, result
						, soc->lower_handle
						, soc->dev_handle
					);

					sock_socket_return_all_resources(soc, SOCK_RSP_ERR_REM_ABORT);  /* 'graceful shutdown' */
				}
				else /* <0: RST; >0: unread data present, but still RST possible. therefore, check socket error in both cases */
				{
					int socket_error = 0;
					int errnom = 0;
					LSA_BOOL has_error;

					has_error = sock_low_get_socket_error(soc->lower_handle
						, &socket_error
						, &errnom
					);

					if (has_error) /* not EWOULDBLOCK or EINPROGRESS */
					{
						if (errnom) /* getsockopt() failed -> local problem -> ERR_LOC_NET */
						{
							SOCK_LOWER_TRACE_03(0, LSA_TRACE_LEVEL_ERROR
								, "TCP socket handle(%d), lower_handle(0x%x): obsd getsockopt(SO_ERROR) failed with errno(%d), return resources with ERR_LOC_NET"
								, soc->dev_handle
								, soc->lower_handle
								, errnom
							);

							sock_socket_return_all_resources(soc, SOCK_RSP_ERR_LOC_NET); /* -> CN_STATE_DISCONNECTING */
						}
						else /* socket has error */
						{
							response = (LSA_RESPONSE_TYPE)sock_low_map_tcp_error(socket_error);

							SOCK_LOWER_TRACE_04(0, LSA_TRACE_LEVEL_WARN
								, "TCP socket handle(%d), lower_handle(0x%x) readable, but socket_error(%d) in socket, return resources with response(%d)"
								, soc->dev_handle
								, soc->lower_handle
								, socket_error
								, response
							);

							sock_socket_return_all_resources(soc, response); /* -> CN_STATE_DISCONNECTING */
						}
					}
				}
			}
		}
	}
}


/*-----------------------------------------------------------------------------
// treat "writeable" event on socket
//----------------------------------------------------------------------------*/
static LSA_VOID
sock_socket_do_writable_event(
	SOCK_SOCKET_PTR soc
) {
	SOCK_LOG_FCT("sock_socket_do_writeable_event")

		if (sock_is_null(soc->send.curr))
		{
			SOCK_PROTOCOL_TRACE_02(0, LSA_TRACE_LEVEL_CHAT
				, "(!)no write rqb socket lower_handle(0X%x) handle(%d)"
				, soc->lower_handle, soc->dev_handle
			);
		}

	sock_socket_send(soc);

	/***/

	if (sock_is_not_null(soc->open_close_rb) /* pending close active */
		&& sock_is_null(soc->send.curr)
		)
	{
		SOCK_PROTOCOL_TRACE_02(0, LSA_TRACE_LEVEL_CHAT
			, "(!)pending close active for socket lower_handle(0X%x) handle(%d), close socket now"
			, soc->lower_handle, soc->dev_handle
		);

		sock_socket_close(soc);
	}
}


/*-----------------------------------------------------------------------------
// treat "soisconnected" event on socket (obsd_kernel_uipc_socket2.c)
//----------------------------------------------------------------------------*/
LSA_VOID
sock_socket_do_connected_event(
	SOCK_SOCKET_PTR soc
) {
	SOCK_LOG_FCT("sock_socket_do_connected_event")

		if (sock_is_not_null(soc->accept.curr)) /* client connect */
		{
			SOCK_PROTOCOL_TRACE_01(0, LSA_TRACE_LEVEL_CHAT
				, "listening socket_fd(0x%x) in read mask"
				, soc->lower_handle
			);

			sock_tcp_accept(soc);
		}
		else if (sock_is_not_null(soc->connect_rb))
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
	/* else do nothing */
}

void sock_socket_do_events(int flag, void* context)
{
	SOCK_SOCKET_PTR soc = (SOCK_SOCKET_PTR)context;

	if (soc->state != SOCK_CN_STATE_DISABLED)
	{
		if (flag & OBSD_PNIO_READ_FLAG)
		{
			sock_socket_do_readable_event(soc);
		}

		if (flag & OBSD_PNIO_WRITE_FLAG)
		{
			sock_socket_do_writable_event(soc);
		}

		if (flag & OBSD_PNIO_CONNECT_FLAG)
		{
			sock_socket_do_connected_event(soc);
		}
	}
}

#endif
