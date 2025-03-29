/**************************************************************************//**
@file		sock_low_anyip.c
@brief		This file contains the implementation of sock_low_xxx functions.
			Theses functions are used in the SOCK component and must be
			implemented with the stack-specific socket interface.
			Documentation of the sock_low functions is done in file sock_low.h
			in the component SOCK.
******************************************************************************/

#include <tcip_iniche_cfg.h>
#include <sock_low_anyip.h>
#include <tcpport.h>
#include <sockcall.h>

/*------------------------------------------------------------------------------
// native socket descriptor sets, see sock_low_select
//----------------------------------------------------------------------------*/
static fd_set readSocketDescriptorSet;
static fd_set writeSocketDescriptorSet;
static fd_set exceptSocketDescriptorSet;

/*----------------------------------------------------------------------------*/
void
sock_low_init(
	void
) {
}

SOCK_LOW_HANDLE_TYPE
sock_low_socket(
	int type,
	int* native_err
) {
	SOCK_LOW_HANDLE_TYPE s = socket(PF_INET, (type == SOCK_LOW_STREAM) ? SOCK_STREAM : SOCK_DGRAM, 0);

	if (s == SOCKET_ERROR)
	{
		*native_err = 0; /* Interniche has no sys_errno */

		return SOCK_LOW_INVALID_HANDLE;
	}
	else
	{
		return s;
	}
}

/*----------------------------------------------------------------------------*/

unsigned int
sock_low_bind(
	SOCK_LOW_HANDLE_TYPE lower_handle,
	SOCK_LOW_ADDR_TYPE* address,
	int *native_err
) {
	if( bind(lower_handle, (struct sockaddr*)address, sizeof(*address)) < 0)
	{
		*native_err = sys_errno(lower_handle);

		switch(*native_err)
		{
		case EADDRINUSE:
			return ANYIP_ERR_ADDR_INUSE;

		case ENOMEM:
		case ENP_RESOURCE:
		case ENOBUFS:
			return ANYIP_ERR_RESOURCE;

		default:
			return ANYIP_ERR_LOC_NET;
		}
	}

	return ANYIP_OK;
}

/*----------------------------------------------------------------------------*/

unsigned int
sock_low_listen(
	SOCK_LOW_HANDLE_TYPE lower_handle,
	int backlog,
	int *native_err
) {
	if( listen(lower_handle, backlog) < 0 )
	{
		*native_err = sys_errno(lower_handle);

		switch(*native_err)
		{
		case EADDRINUSE:
			return ANYIP_ERR_ADDR_INUSE;

		default:
			return ANYIP_ERR_LOC_NET;
		}
	}

	return ANYIP_OK;
}

/*----------------------------------------------------------------------------*/
int
sock_low_map_connect_error(
	int error
) {
	switch(error)
	{
	case EINPROGRESS:
	case EWOULDBLOCK:
		return ANYIP_OK_ACTIVE;

	case ENOMEM:
	case ENP_RESOURCE:
	case ENOBUFS:
		return ANYIP_ERR_RESOURCE;

	case ECONNREFUSED:
		return ANYIP_ERR_REM_ABORT;

	case ENETUNREACH:
	case ETIMEDOUT:
		return ANYIP_ERR_REM_NET;

	default:
		return ANYIP_ERR_LOC_NET;
	}
}

unsigned int
sock_low_connect(
	SOCK_LOW_HANDLE_TYPE lower_handle,
	SOCK_LOW_ADDR_TYPE* address,
	int *native_err
) {
	if( connect(lower_handle, (struct sockaddr*)address, sizeof(*address)) < 0 )
	{
		*native_err = sys_errno(lower_handle);

		return (unsigned int)sock_low_map_connect_error(*native_err);
	}

	return ANYIP_OK;
}

/*----------------------------------------------------------------------------*/

SOCK_LOW_HANDLE_TYPE
sock_low_accept(
	SOCK_LOW_HANDLE_TYPE lower_handle,
	SOCK_LOW_ADDR_TYPE* address,
	int *native_err
) {
	int address_len = sizeof(*address);

	SOCK_LOW_HANDLE_TYPE s  = accept(lower_handle, (struct sockaddr*)address, &address_len);

	if (s == SOCKET_ERROR)
	{
		*native_err = sys_errno(lower_handle); /* for caller trace only */

		return SOCK_LOW_INVALID_HANDLE;
	}
	else
	{
		return (SOCK_LOW_HANDLE_TYPE)s;
	}
}

/*----------------------------------------------------------------------------*/

unsigned int
sock_low_close(
	SOCK_LOW_HANDLE_TYPE lower_handle
) {
	if (socketclose(lower_handle) < 0)
	{
		return ANYIP_ERR_LOC_NET; /* should never happen */
	}

	return ANYIP_OK;
}

/*----------------------------------------------------------------------------*/

unsigned int
sock_low_map_tcp_error(
	int error
) {
	switch(error)
	{
	case EWOULDBLOCK:
	case EINPROGRESS:
		return ANYIP_OK_ACTIVE;

	case ECONNABORTED:
	case ECONNRESET:
	case ESHUTDOWN:
		return ANYIP_ERR_REM_ABORT;

	case EPIPE: /* ETIMEDOUT error in socket, but t_send() and t_recv() yield EPIPE  */
	case ETIMEDOUT: /* connection dropped by stack's timer */
		return ANYIP_ERR_REM_NET;

	case ENOMEM:
	case ENP_RESOURCE:
	case ENOBUFS:
		return ANYIP_ERR_RESOURCE;

	default:
		return ANYIP_ERR_LOC_NET;
	}
}


unsigned int
sock_low_send(
	SOCK_LOW_HANDLE_TYPE lower_handle,
	char* buffer,
	int length,
	int* sent,
	int *native_err
) {
	/* note: The interniche stack can return 0 instead of error EWOULDBLOCK */

	*sent = send(
		lower_handle, 
		buffer,
		length,
		MSG_DONTWAIT
	);

	if (*sent < 0)
	{
		*native_err = sys_errno(lower_handle);

		return (unsigned int)sock_low_map_tcp_error(*native_err);
	}

	return ANYIP_OK;
}

/*----------------------------------------------------------------------------*/

unsigned int
sock_low_recv(
	SOCK_LOW_HANDLE_TYPE lower_handle,
	char *buffer,
	int length,
	int flags,
	int	*received,
	int	*native_err
) {
	*received = recv(
		lower_handle,
		buffer,
		length,
		(flags & SOCK_LOW_PEEK) ? MSG_PEEK : 0 /* only MSG_PEEK needed */
	);

	if (*received < 0)
	{
		*native_err = sys_errno(lower_handle);

		return (unsigned int)sock_low_map_tcp_error(*native_err);
	}

	return ANYIP_OK;
}

/*----------------------------------------------------------------------------*/

int
sock_low_map_udp_error(
	int err
) {
	switch(err) /* note: no ERR_REM_NET possible */
	{
	case EWOULDBLOCK:
		return ANYIP_OK_ACTIVE;

	case ENOMEM:
	case ENP_RESOURCE:
	case ENOBUFS:
		return ANYIP_ERR_RESOURCE;

	default:
		return ANYIP_ERR_LOC_NET;
	}
}


unsigned int
sock_low_sendto(
	SOCK_LOW_HANDLE_TYPE lower_handle,
	char *buffer,
	int length,
	SOCK_LOW_ADDR_TYPE* address,
	SOCK_LOW_ADDR_TYPE* local_address,
	int	*sent,
	int	*native_err
) {
	LSA_UNUSED_ARG(local_address); /* not supported */

	*sent = sendto( 
		lower_handle,
		buffer,
		length,
		MSG_DONTWAIT,
		(struct sockaddr*)address,
		(address) ? sizeof(*address) : 0
	);

	if (*sent < 0)
	{
		*native_err = sys_errno(lower_handle);

		return (unsigned int)sock_low_map_udp_error(*native_err);
	}

	return ANYIP_OK;
}

/*----------------------------------------------------------------------------*/

unsigned int
sock_low_recvfrom(
	SOCK_LOW_HANDLE_TYPE lower_handle,
	char *buffer,
	int length,
	int flags,
	SOCK_LOW_ADDR_TYPE* address,
	SOCK_LOW_ADDR_TYPE* local_address,
	int *received,
	int *native_err
) {
	LSA_INT address_len = sizeof(*address);

	LSA_UNUSED_ARG(local_address); /* not supported */

	*received = recvfrom(
		lower_handle,
		buffer,
		length,
		(flags & SOCK_LOW_PEEK) ? MSG_PEEK : 0,
		(struct sockaddr*)address,
		&address_len
	);

	if (*received < 0)
	{
		*native_err = sys_errno(lower_handle);

		return (unsigned int)sock_low_map_udp_error(*native_err);
	}

	return ANYIP_OK;
}

/*----------------------------------------------------------------------------*/

unsigned char
sock_low_get_socket_error(
	SOCK_LOW_HANDLE_TYPE lower_handle,
	int *socket_error,
	int *native_err
) {
	unsigned int len = sizeof(*socket_error);

	int ret = t_getsockopt(lower_handle
		, SOL_SOCKET
		, SO_ERROR
		, socket_error
		, (int)len);

	if (ret < 0)
	{
		*native_err = sys_errno(lower_handle); /* make return value of getsockopt() available */
		return 1; /* unable to retrieve SO_ERROR option -> socket is considered to be damaged */
	}
	else /* valid *socket_error from socket */
	{
		if (*socket_error
			&& *socket_error != EWOULDBLOCK
			&& *socket_error != EINPROGRESS
			)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

/*----------------------------------------------------------------------------*/

unsigned char
sock_low_setsockopt(
	SOCK_LOW_HANDLE_TYPE lower_handle,
	SOCK_LOW_SOCKOPT_TYPE option_name,
	void *option_value,
	int *native_err
) {
	int ret;

	switch (option_name)
	{
	case sock_low_sockopt_NonBlocking:
		ret = setsockopt(lower_handle, SOL_SOCKET, SO_NONBLOCK, option_value, sizeof(int));
		break;

	case sock_low_sockopt_TCPNoDelay:
		ret = setsockopt(lower_handle, IPPROTO_TCP, TCP_NODELAY, option_value, sizeof(int));
		break;

	case sock_low_sockopt_TCPNoAckDelay:
		ret = setsockopt(lower_handle, IPPROTO_TCP, TCP_NOACKDELAY, option_value, sizeof(int));
		break;

	case sock_low_sockopt_ReuseAddr:
		ret = setsockopt(lower_handle, SOL_SOCKET, SO_REUSEADDR, option_value, sizeof(int));
		break;

	case sock_low_sockopt_KeepAlive:
		ret = setsockopt(lower_handle, SOL_SOCKET, SO_KEEPALIVE, option_value, sizeof(int));
		break;

	case sock_low_sockopt_DontRoute:
		ret = setsockopt(lower_handle, SOL_SOCKET, SO_DONTROUTE, option_value, sizeof(int));
		break;

	case sock_low_sockopt_SendBuffer:
		ret = setsockopt(lower_handle, SOL_SOCKET, SO_SNDBUF, option_value, sizeof(int));
		break;

	case sock_low_sockopt_RecvBuffer:
		ret = setsockopt(lower_handle, SOL_SOCKET, SO_RCVBUF, option_value, sizeof(int));
		break;

	case sock_low_sockopt_Linger:
		{
		struct linger linger_value;
		linger_value.l_linger = *(int*)option_value;
		linger_value.l_onoff  = 1;
		ret = setsockopt(lower_handle, SOL_SOCKET, SO_LINGER, &linger_value, sizeof(linger_value));
		}
		break;

	case sock_low_sockopt_Broadcast:
		ret = setsockopt(lower_handle, SOL_SOCKET, SO_BROADCAST, option_value, sizeof(int));
		break;

	case sock_low_sockopt_OOBInline:
		ret = setsockopt(lower_handle, SOL_SOCKET, SO_OOBINLINE, option_value, sizeof(int));
		break;

	default:
		ret = -1; /* prevent compiler warning C4701: potentially uninitialized local variable 'ret' used */
		break;
	}

	if (ret < 0)
	{
		*native_err = sys_errno(lower_handle);

		return 0;
	}

	return 1;
}

/*----------------------------------------------------------------------------*/

unsigned int
sock_low_set_multicast_sockopts(
	SOCK_LOW_HANDLE_TYPE lower_handle,
	unsigned int interface_index,
	unsigned int interface_addr,
	unsigned int group_addr,
	char ip_multicast_ttl,
	int *native_err
) {
	struct ip_mreq stMreq;
	LSA_UINT8 mc_loop = 0;

	ANYIP_UNUSED_ARG(interface_addr);

	*native_err = 0;
	stMreq.imr_interface = interface_index;
	stMreq.imr_multiaddr = group_addr;

	if(setsockopt(lower_handle, IPPROTO_IP, IP_ADD_MEMBERSHIP, &stMreq, sizeof(stMreq)) != 0)
	{
		*native_err = sys_errno(lower_handle);
	}
	else if(setsockopt(lower_handle, IPPROTO_IP, IP_MULTICAST_IF, &interface_index, sizeof(interface_index)) != 0)
	{
		*native_err = sys_errno(lower_handle);
	}
	else if(setsockopt(lower_handle, IPPROTO_IP, IP_MULTICAST_LOOP, &mc_loop, sizeof(mc_loop)) != 0)
	{
		*native_err = sys_errno(lower_handle);
	}
	else if(setsockopt(lower_handle, IPPROTO_IP, IP_MULTICAST_TTL, &ip_multicast_ttl, sizeof(ip_multicast_ttl)) != 0)
	{
		*native_err = sys_errno(lower_handle);
	}

	switch(*native_err)
	{
	case 0:
		return ANYIP_OK;

	case ENOBUFS:
		return ANYIP_ERR_RESOURCE; /* memory allocation failed */

	case EINVAL:
		return ANYIP_ERR_PARAM; /* not a mc address used for IP_ADD_MEMBERSHIP */

	/* should never happen: */
	case EADDRNOTAVAIL:
	case EADDRINUSE:
	case ETOOMANYREFS:
	case EOPNOTSUPP:
	default:
		return ANYIP_ERR_LOC_NET;
	}
}

/*----------------------------------------------------------------------------*/

unsigned int
sock_low_get_sockname(
	SOCK_LOW_HANDLE_TYPE lower_handle,
	SOCK_LOW_ADDR_TYPE *address,
	int *native_err
) {
	int address_len = sizeof(struct sockaddr);

	if( t_getsockname(lower_handle, (struct sockaddr*)address, &address_len) < 0 )
	{
		*native_err = t_errno(lower_handle);

		switch(*native_err)
		{
		case ENOMEM:
			return ANYIP_ERR_RESOURCE;

		default:
			return ANYIP_ERR_LOC_NET;
		}

	}

	return ANYIP_OK;

}

/*----------------------------------------------------------------------------*/
#if (ANYIP_CFG_USE_SOCKET_EVENTS == 0)

int
sock_low_select(
	int timeout
) {
	return t_select( 
		&readSocketDescriptorSet,
		&writeSocketDescriptorSet,
		&exceptSocketDescriptorSet,
		(TPS * timeout) / 1000
	);
}

/*----------------------------------------------------------------------------*/

void
sock_low_readset_insert(
	SOCK_LOW_HANDLE_TYPE lower_handle
) {
	FD_SET(lower_handle, &readSocketDescriptorSet);
}

/*----------------------------------------------------------------------------*/

void
sock_low_writeset_insert(
	SOCK_LOW_HANDLE_TYPE lower_handle
) {
	FD_SET(lower_handle, &writeSocketDescriptorSet);
}

/*----------------------------------------------------------------------------*/

void
sock_low_exceptset_insert(
	SOCK_LOW_HANDLE_TYPE lower_handle
) {
	FD_SET(lower_handle, &exceptSocketDescriptorSet);
}

/*----------------------------------------------------------------------------*/

int
sock_low_in_readset(
	SOCK_LOW_HANDLE_TYPE lower_handle
) {
	return FD_ISSET(lower_handle, &readSocketDescriptorSet) ? 1 : 0;
}

/*----------------------------------------------------------------------------*/

int
sock_low_in_writeset(
	SOCK_LOW_HANDLE_TYPE lower_handle
) {
	return FD_ISSET(lower_handle, &writeSocketDescriptorSet) ? 1 : 0;
}

/*----------------------------------------------------------------------------*/

int
sock_low_in_exceptset(
	SOCK_LOW_HANDLE_TYPE lower_handle
) {
	return FD_ISSET(lower_handle, &exceptSocketDescriptorSet) ? 1 : 0;
}

#endif /* (ANYIP_CFG_USE_SOCKET_EVENTS == 0) */
/*----------------------------------------------------------------------------*/

void
sock_low_clear_all_sets(
	void
) {
	FD_ZERO(&readSocketDescriptorSet);
	FD_ZERO(&writeSocketDescriptorSet);
	FD_ZERO(&exceptSocketDescriptorSet);
}


/*----------------------------------------------------------------------------*/

unsigned char
sock_low_get_reason_code(
	int err
) {
	switch(err)
	{
	case EMSGSIZE: return SOCK_LOW_IPSTACK_EMSGSIZE;
	case EADDRNOTAVAIL: /* see in_pcbconnect() */
	case ENP_NO_ROUTE: return SOCK_LOW_IPSTACK_EHOSTUNREACH;
	default: return 0;
	}
}

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
