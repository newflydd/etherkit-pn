#ifndef SOCK_LOW_ANYIP_H
#define SOCK_LOW_ANYIP_H
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
/*  F i l e               &F: sock_low_anyip.h                          :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/* interface implemented by sock_xxx module of configured IP Stack           */
/*****************************************************************************/

#include "anyip_inc.h"

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif


typedef union
{
	union
	{
		struct
		{
			unsigned char s_b1, s_b2, s_b3, s_b4;
		} S_un_b;
		struct
		{
			unsigned short s_w1, s_w2;
		} S_un_w;
		unsigned int S_addr;
	} S_un;
	unsigned int s_addr;
} SockLowIF_IPAddr;

typedef struct
{
	short			sin_family;
	unsigned short  sin_port;
	SockLowIF_IPAddr sin_addr;
	char			sin_zero[8];
} SockLowIF_AddrIn;

typedef SockLowIF_AddrIn SOCK_LOW_ADDR_TYPE;

/*=============================================================================
// IP stack independent definitions
// ==========================================================================*/

#define SOCK_LOW_STREAM 1
#define SOCK_LOW_DGRAM  2

#define SOCK_LOW_SOCKET_ERROR -1

#define SOCK_LOW_PEEK 0x1

#define SOCK_LOW_IPSTACK_EMSGSIZE      1
#define SOCK_LOW_IPSTACK_EHOSTUNREACH  2

#define SOCK_LOW_INVALID_HANDLE ((SOCK_LOW_HANDLE_TYPE)-1)

#define SOCK_LOW_MAX(_fd1, _fd2) if ((_fd2) > (_fd1)) { (_fd1) = (_fd2);}


/*-----------------------------------------------------------------------------
// Option types for sock_low_getsockopt(), sock_low_setsockopt()
// --------------------------------------------------------------------------*/
typedef enum
{
	sock_low_sockopt_Type,
	sock_low_sockopt_Error,
	sock_low_sockopt_SendBuffer,
	sock_low_sockopt_RecvBuffer,
	sock_low_sockopt_NonBlocking,
	sock_low_sockopt_ReuseAddr,
	sock_low_sockopt_KeepAlive,
	sock_low_sockopt_DontRoute,
	sock_low_sockopt_Linger,
	sock_low_sockopt_Linger_off,
	sock_low_sockopt_Broadcast,
	sock_low_sockopt_TCPNoDelay,
	sock_low_sockopt_TCPNoAckDelay,
	sock_low_sockopt_OOBInline,
	sock_low_sockopt_IPRecvdStAddr,
	sock_low_sockopt_Connreset
} SOCK_LOW_SOCKOPT_TYPE;

/*=============================================================================
// sock_low_ interface declarations
// ==========================================================================*/

/**************************************************************************//**
@ingroup		sock_low_anyip
@brief  The function makes all IP stack specific initializations and has to be
called before all other sock_low_ ... functions.
******************************************************************************/
void
sock_low_init(void);

/**************************************************************************//**
@ingroup		sock_low_anyip
@brief			Creates an IP-stack specific socket
@details        The function creates an unbound socket and returns a handle
				containing the socket descriptor of the IP stack.

@param[in]		type: #SOCK_LOW_STREAM for TCP or #SOCK_LOW_DGRAM for UDP sockets
@param[out]		err: pointer to int receiving the native result code if not successful.

@return			socket handle, in case of success. 
@return			SOCK_LOW_INVALID_HANDLE: in case of errors.
				err contains native error code if system call failed
******************************************************************************/
SOCK_LOW_HANDLE_TYPE
sock_low_socket(
	int type,
	int* err
);

/**************************************************************************//**
@ingroup		sock_low_anyip
@brief          Assigns an address to an unnamed socket.
@details        The function binds an address to a socket. Newly created
				sockets have no address.

@param[in]		lower_handle: socket handle of created socket
@param[in]		address: pointer to an SOCK_LOW_ADDR_TYPE containing 
						 the new address
@param[out]		err: pointer to int receiving the native result code if 
					 not successful.

@return		ANYIP_OK, in case of success. 
@return		ANYIP_ERR_ADDR_INUSE, if address is already used
@return		ANYIP_ERR_RESOURCE, if stack has no memory left
@return		ANYIP_ERR_LOC_NET, if an other error has occured
******************************************************************************/
unsigned int
sock_low_bind(
	SOCK_LOW_HANDLE_TYPE lower_handle,
	SOCK_LOW_ADDR_TYPE* address,
	int* err
);

/**************************************************************************//**
@ingroup		sock_low_anyip
@brief          Tells the IP stack to listen on a socket.
@details        The function tells the IP stack to listen for incoming 
				connect requests on a socket.

@param[in]		lower_handle: socket handle of listening socket
@param[in]		backlog: maximum length of pending connections queue
@param[out]		err: pointer to int receiving the native result code if 
					 not successful.

@return		ANYIP_OK, in case of success. 
@return		ANYIP_ERR_ADDR_INUSE, if address is already used
@return		ANYIP_ERR_LOC_NET, if an other error has occured
******************************************************************************/
unsigned int
sock_low_listen(
	SOCK_LOW_HANDLE_TYPE lower_handle,
	int backlog,
	int* err
);

/**************************************************************************//**
@ingroup		sock_low_anyip
@brief          Tries to connect to a remote address.
@details        The function sends a connect request to a remote socket
				address.

@param[in]		lower_handle: socket handle of socket that should be connected
@param[in]		address: pointer to a #SOCK_LOW_ADDR_TYPE structure 
						 containing the peer IP address
@param[out]		err: pointer to int receiving the native result code if 
					 not successful.

@return		ANYIP_OK, in case of success.
@return		ANYIP_OK_ACTIVE, if connect is in progress (non-blocking IO)
@return		ANYIP_ERR_RESOURCE, if stack has no memory left
@return		ANYIP_ERR_REM_ABORT, if peer has refused connnection
@return		ANYIP_ERR_REM_NET, if remote net is not reachable or an timeout has occured
@return     ANYIP_ERR_MSGSIZE, if message size is too long
@return		ANYIP_ERR_HOSTUNREACH, if peer is unreachable
@return		ANYIP_ERR_LOC_NET, if an other error has occured
******************************************************************************/
unsigned int
sock_low_connect(
	SOCK_LOW_HANDLE_TYPE lower_handle,
	SOCK_LOW_ADDR_TYPE* address,
	int* err
);

/**************************************************************************//**
@ingroup		sock_low_anyip
@brief          Accepts the connection request of a remote socket
@details        The function accepts a connect request from a remote socket
				address, creates a new socket and and establishes a connection.

@param[in]		listening_sock_fd: socket handle of listening socket, that 
								   reported the connect request
@param[out]		address: pointer to a #SOCK_LOW_ADDR_TYPE structure 
						 containing the IP address of the new socket
@param[out]		err: pointer to int receiving the native result code if 
					 not successful.

@return		socket handle, in case of success.
@return		SOCK_LOW_INVALID_HANDLE, in case of an error.
******************************************************************************/
SOCK_LOW_HANDLE_TYPE
sock_low_accept(
	SOCK_LOW_HANDLE_TYPE listening_sock_fd,
	SOCK_LOW_ADDR_TYPE* address,
	int* err
	/* user will not see accept errors (ACCEPT_NOTIFY rqb is not returned if accept fails) */
);


/**************************************************************************//**
@ingroup		sock_low_anyip
@brief          Closes a socket
@details        The function closes the socket with the given socket handle.

@param[in]		lower_handle: socket handle of socket to be closed

@return		ANYIP_OK, in case of success.
@return		ANYIP_ERR_LOC_NET, if an other error has occured
******************************************************************************/
unsigned int
sock_low_close(
	SOCK_LOW_HANDLE_TYPE lower_handle
);

/**************************************************************************//**
@ingroup		sock_low_anyip
@brief          Sends a message to the network via a given socket
@details        The function sends a message via a given socket. The socket
				has to be connected to a peer.

@param[in]		lower_handle: socket handle of socket that is used to send
@param[in]		buffer: pointer to memory containing the message to send
@param[in]		length: length of the message in bytes
@param[out]		sent: pointer to the number of bytes that have been sent.
					  Is set to #SOCK_LOW_SOCKET_ERROR if error occurs.
@param[out]		err: pointer to int receiving the native result code if
					 not successful.

@return		ANYIP_OK, in case of success.
@return		ANYIP_OK_ACTIVE, if connect is in progress (non-blocking IO)
@return		ANYIP_ERR_RESOURCE, if stack has no memory left
@return		ANYIP_ERR_REM_ABORT, if peer has refused connnection
@return		ANYIP_ERR_REM_NET, if peer is not reachable or an timeout has occured
@return		ANYIP_ERR_LOC_NET, if an other error has occured
******************************************************************************/
unsigned int
sock_low_send(
	SOCK_LOW_HANDLE_TYPE lower_handle,
	char* buffer,
	int length,
	int* sent,
	int* err
);

/**************************************************************************//**
@ingroup		sock_low_anyip
@brief          Receives data from a given socket
@details        The function retrieves data received by the given socket.
				The socket has to be connected to a peer.

@param[in]		lower_handle: socket handle of socket that is used to receive
@param[out]		buffer: pointer to memory where received data is copied into
@param[in]		length: length of buffer in bytes
@param[in]		flags: If flags contains #SOCK_LOW_PEEK, data is copied into 
					   the buffer but not removed from the input queue
@param[out]		received: pointer to length of the message in bytes. If no
				data is available and the peer has performed a graceful 
				shutdown, it returns 0. In case of an error #SOCK_LOW_SOCKET_ERROR 
				is returned.
@param[out]		err: pointer to int receiving the native result code if
					 not successful.

@return		ANYIP_OK, in case of success.
@return		ANYIP_OK_ACTIVE, if connect is in progress (non-blocking IO)
@return		ANYIP_ERR_RESOURCE, if stack has no memory left
@return		ANYIP_ERR_REM_ABORT, if peer has refused connnection
@return		ANYIP_ERR_REM_NET, if peer is not reachable or an timeout has occured
@return		ANYIP_ERR_LOC_NET, if an other error has occured
******************************************************************************/
unsigned int
sock_low_recv(
	SOCK_LOW_HANDLE_TYPE lower_handle,
	char* buffer,
	int length,
	int flags,
	int* received,
	int* err
);

/**************************************************************************//**
@ingroup		sock_low_anyip
@brief          Sends a message to a given IP address via a specific socket.
@details        The function sends a message to a given IP address via a
				given socket. This is done for UDP messages if the UDP socket 
				is not connected.

@param[in]		lower_handle: socket handle of socket that is used to send
@param[in]		buffer: pointer to memory containing the message to send
@param[in]		length: length of the message in bytes
@param[in]		address: pointer to a #SOCK_LOW_ADDR_TYPE structure
					containing the destination address. Can be zero for
					connected udp sockets.
@param[in]		local_address: pointer to a #IP_PNIO_SOCKADDR_TYPE structure 
					containing the IP address which shall be used as source 
					address of the outgoing IP packet. The IP address received 
					with the local_address parameter of the ip_pnio_recv_from() 
					function must be used by UDP server applications which bind
					to ANY.
					Linux IP Stacks can set the IP_PKTINFO socket option and 
					implement this Function using sendmsg(). The Winsock 
					implementation can use the same option and implement this 
					function using WSASendMsg().
					IP Stacks which do not support setting the src address of 
					the outgoing IP packet can ignore the parameter.
@param[out]		sent: pointer to the number of bytes that have been sent.
					Is set to SOCK_LOW_SOCKET_ERROR if error occurs.
@param[out]		err: pointer to int receiving the native result code if
					not successful.

@return		ANYIP_OK, in case of success.
@return		ANYIP_OK_ACTIVE, if connect is in progress (non-blocking IO)
@return		ANYIP_ERR_RESOURCE, if stack has no memory left
@return		ANYIP_ERR_REM_ABORT, if peer has refused connnection
@return		ANYIP_ERR_REM_NET, if peer is not reachable or an timeout has occured
@return		ANYIP_ERR_LOC_NET, if an other error has occured
******************************************************************************/
unsigned int
sock_low_sendto(
	SOCK_LOW_HANDLE_TYPE lower_handle,
	char* buffer,
	int length,
	SOCK_LOW_ADDR_TYPE* address,
	SOCK_LOW_ADDR_TYPE* local_address,
	int* sent,
	int* err
);

/**************************************************************************//**
@ingroup		sock_low_anyip
@brief          Receives data from a given socket
@details        The function retrieves data received by the given socket and
				delivers the source IP address of the message.

@param[in]		lower_handle: socket handle of socket that is used to receive
@param[out]		buffer: pointer to memory where received data is copied into
@param[in]		length: length of buffer in bytes
@param[in]		flags: If flags contains #SOCK_LOW_PEEK, data is copied into 
					   the buffer but not removed from the input queue
@param[out]		address: pointer to a #SOCK_LOW_ADDR_TYPE structure 
					receiving the source address
@param[out]		local_address: pointer to a #SOCK_LOW_ADDR_TYPE structure 
					receiving the destination address of the IP packet that 
					conveyed this UDP message.
					For Linux IP stacks, set the IP_PKTINFO socket option and 
					implement this function using recvmsg(). The Winsock 
					implementation can use the same option and the Winsock 
					function WSARecvMsg().
					Implementations for IP Stacks not supporting the retrieval 
					of the destination address shall fill the structure with 0.
@param[out]		received: pointer to length of the message in bytes. If no
				data is available and the peer has performed a graceful 
				shutdown, it returns 0. In case of an error #SOCK_LOW_SOCKET_ERROR 
				is returned.
@param[out]		err: pointer to int receiving the native result code if
					 not successful.

@return		ANYIP_OK, in case of success.
@return		ANYIP_OK_ACTIVE, if connect is in progress (non-blocking IO)
@return		ANYIP_ERR_RESOURCE, if stack has no memory left
@return		ANYIP_ERR_REM_ABORT, if peer has refused connnection
@return		ANYIP_ERR_REM_NET, if peer is not reachable or an timeout has occured
@return		ANYIP_ERR_LOC_NET, if an other error has occured
******************************************************************************/
unsigned int
sock_low_recvfrom(
	SOCK_LOW_HANDLE_TYPE lower_handle,
	char* buffer,
	int length,
	int flags,
	SOCK_LOW_ADDR_TYPE* address,
	SOCK_LOW_ADDR_TYPE* local_address,
	int* received,
	int* err
);

/**************************************************************************//**
@ingroup		sock_low_anyip
@brief          Retrieves the latest socket error from a given socket
@details        The function retrieves the socket error stored in the socket.
				This is the value of the last error occured on this socket.

@param[in]		lower_handle: socket handle
@param[out]		socket_error: variable where socket error is stored into
@param[out]		err: pointer to int receiving the native result code if
					 not successful.

@return ANYIP_TRUE, if socket has error not equal to EWOULDBLOCK and EINPROGRESS,
otherwise ANYIP_FALSE

@brief  retrieves error stored in socket and examines the error code. If an
error code different than EWOULDBLOCK or EINPROGRESS is found, an error
condition is assumed and the function returns 1. The error
EWOULDBLOCK indicates the blocking condition for sockets working with
non-blocking IO. EINPROGRESS is returned by the connect system call on
sockets with non-blocking IO when a connect is already pending.
Therefore, these error codes do not indicate an error condition.
Remark: The function is used in event-driven mode only.
******************************************************************************/
unsigned char
sock_low_get_socket_error(
	SOCK_LOW_HANDLE_TYPE lower_handle,
	int* socket_error,
	int* err
);

/**************************************************************************//**
@ingroup		sock_low_anyip
@brief          Sets socket options
@details        The function sets options to a given socket. The option names
				have to be mapped to the IP specific options.

@param[in]		lower_handle: socket handle
@param[in]		option_name: a value fo the #SOCK_LOW_SOCKOPT_TYPE enumeration type.
					The following option_names are available:
					@li sock_low_sockopt_SendBuffer: send-Buffer length
					@li sock_low_sockopt_RecvBuffer: receive-Buffer length
					@li sock_low_sockopt_NonBlocking: set socket to non-blocking mode
					@li sock_low_sockopt_ReuseAddr: allow socket to bind to an address and port already in use
					@li sock_low_sockopt_KeepAlive: enables keep-alive for socket connection
					@li sock_low_sockopt_DontRoute: outgoing data are not routed to other interface
					@li sock_low_sockopt_Linger: enable linger on close, set linger time
					@li sock_low_sockopt_Broadcast: allow sending broadcast data
					@li sock_low_sockopt_TCPNoDelay: en/disable the Nagle algorithm
					@li sock_low_sockopt_TCPNoAckDelay: suppress delayed Acknowledgements
					@li sock_low_sockopt_OOBInline: enables reception of out-of-band data in band
					@li sock_low_sockopt_IPRecvdStAddr: enables reception of IP destination address
@param[in]		option_value: option specific variable containing the new 
					value of the option
@param[out]		err: pointer to int receiving the native result code if
					 not successful.

@return		ANYIP_TRUE, in case of success.
@return		ANYIP_FALSE, if case of an error.
******************************************************************************/
unsigned char
sock_low_setsockopt(
	SOCK_LOW_HANDLE_TYPE lower_handle,
	SOCK_LOW_SOCKOPT_TYPE option_name,
	void* option_value,
	int* err
);

/**************************************************************************//**
@ingroup		sock_low_anyip
@brief          Sets multicast socket options
@details        The function sets the required multicast options for a multicast 
				enabled socket. These are options like: IP_ADD_MEMBERSHIP, 
				IP_MULTICAST_IF, IP_MULTICAST_LOOP and IP_MULTICAST_TTL.

@param[in]		lower_handle: socket handle
@param[in]		interface_index: index of interface for outgoing multicast traffic
@param[in]		interface_address: IPv4 address of interface for outgoing multicast traffic
@param[in]		group_addr: multicast group address in network byte order
@param[in]		ip_multicast_ttl: time-to-live for sent multicast packets
@param[out]		err: pointer to int receiving the native result code if
					 not successful.

@return		ANYIP_OK, in case of success.
@return		ANYIP_ERR_RESOURCE, if stack has no memory left
@return		ANYIP_ERR_PARAM, if parameter was incorrect
@return		ANYIP_ERR_LOC_NET, if an other error has occured
******************************************************************************/
unsigned int
sock_low_set_multicast_sockopts(
	SOCK_LOW_HANDLE_TYPE lower_handle,
	unsigned int interface_index,
	unsigned int interface_addr,
	unsigned int group_addr,
	char ip_multicast_ttl,
	int *err
);

/**************************************************************************//**
@ingroup		sock_low_anyip
@brief          Retrieves the address of a given socket
@details        The function retrieves the locally-bound name of the 
				specified socket and stores this address in the #SOCK_LOW_ADDR_TYPE
				structure pointed to by the address argument.
				If the socket has not been bound to a local name, the value 
				stored in the object pointed to by address is unspecified.

@param[in]		lower_handle: socket handle
@param[out]		address: pointer to a #SOCK_LOW_ADDR_TYPE structure 
					where the local address of the socket will be returned.
@param[out]		err: pointer to int receiving the native result code if
					 not successful.

@return		ANYIP_OK, in case of success.
@return		ANYIP_ERR_RESOURCE, if stack has no memory left
@return		ANYIP_ERR_LOC_NET, if an other error has occured
******************************************************************************/
unsigned int
sock_low_get_sockname(
	SOCK_LOW_HANDLE_TYPE lower_handle,
	SOCK_LOW_ADDR_TYPE* address,
	int* err
);

#if (ANYIP_CFG_USE_SOCKET_EVENTS == 0)
/**************************************************************************//**
@ingroup		sock_low_anyip
@brief          Calls IP-specific select function
@details        Invokes the select() system call on the read-, write- and
				except- socket-sets and passes the timeout parameter. The 
				sets must have been created and prepared previously 
				by the user using sock_low_readset_insert(), 
				sock_low_writeset_insert() and sock_low_exceptset_insert().

@param[in]		timeout: timeout interval for select in milliseconds

@return		number of sockets in result sets, in case of success.
@return		#SOCK_LOW_SOCKET_ERROR, in case an error occurs.
******************************************************************************/
int
sock_low_select(
	int timeout
);

/**************************************************************************//**
@ingroup		sock_low_anyip
@brief          Insert socket handle into the read-socket-set
@details        The function inserts the handle of a socket that has become
				readable in the read-socket-set. The set is created by the user.

@param[in]		lower_handle: socket handle to be inserted
******************************************************************************/
void
sock_low_readset_insert(
	SOCK_LOW_HANDLE_TYPE lower_handle
);

/**************************************************************************//**
@ingroup		sock_low_anyip
@brief          Insert socket handle into the write-socket-set
@details        The function inserts the handle of a socket that has become
				writeable in the write-socket-set. The set is created by the user.

@param[in]		lower_handle: socket handle to be inserted
******************************************************************************/
void
sock_low_writeset_insert(
	SOCK_LOW_HANDLE_TYPE lower_handle
);

/**************************************************************************//**
@ingroup		sock_low_anyip
@brief          Insert socket handle into the except-socket-set
@details        The function inserts the handle of a socket where a connection
				operation has been carried out in the except-socket-set. 
				The set is created by the user.

@param[in]		lower_handle: socket handle to be inserted
******************************************************************************/
void
sock_low_exceptset_insert(
	SOCK_LOW_HANDLE_TYPE lower_handle
);

/**************************************************************************//**
@ingroup		sock_low_anyip
@brief          Finds out if a given socket is in the read-socket-set.

@param[in]		lower_handle: socket handle to be found

@return			ANYIP_TRUE, if lower_handle is in the read-socket-set
@return			ANYIP_FALSE, otherwise
******************************************************************************/
int
sock_low_in_readset(
	SOCK_LOW_HANDLE_TYPE lower_handle
);

/**************************************************************************//**
@ingroup		sock_low_anyip
@brief          Finds out if a given socket is in the write-socket-set.

@param[in]		lower_handle: socket handle to be found

@return			ANYIP_TRUE, if lower_handle is in the write-socket-set
@return			ANYIP_FALSE, otherwise
******************************************************************************/
int
sock_low_in_writeset(
	SOCK_LOW_HANDLE_TYPE lower_handle
);

/**************************************************************************//**
@ingroup		sock_low_anyip
@brief          Finds out if a given socket is in the except-socket-set.

@param[in]		lower_handle: socket handle to be found

@return			ANYIP_TRUE, if lower_handle is in the except-socket-set
@return			ANYIP_FALSE, otherwise
******************************************************************************/
int
sock_low_in_exceptset(
SOCK_LOW_HANDLE_TYPE lower_handle
);

#endif /* (ANYIP_CFG_USE_SOCKET_EVENTS == 0) */

/**************************************************************************//**
@ingroup		sock_low_anyip
@brief          Clear all sets created by the user.
@detail			The function clears the read-, write- and except-socket-sets.

******************************************************************************/
void
sock_low_clear_all_sets(
	void
);

/**************************************************************************//**
@ingroup		sock_low_anyip
@brief          Maps a native socket error code from connect() call 
				to an ANYIP_ error code

@param[in]		error: socket error code

@return			ANYIP error code according this list (OpenBSD native codes):
				socket error | ANYIP error
				---------------------------------
				EINPROGRESS  | ANYIP_OK_ACTIVE
				EWOULDBLOCK  | ANYIP_OK_ACTIVE
				ENOMEM		 | ANYIP_ERR_RESOURCE
				ENOBUFS		 | ANYIP_ERR_RESOURCE
				ECONNREFUSED | ANYIP_ERR_REM_ABORT
				ETIMEDOUT	 | ANYIP_ERR_REM_NET
				ENETUNREACH	 | ANYIP_ERR_REM_NET
				others	     | ANYIP_ERR_LOC_NET
******************************************************************************/
int
sock_low_map_connect_error(
	int error
);

/**************************************************************************//**
@ingroup		sock_low_anyip
@brief          Maps a native socket error code from TCP send() or recv() 
				call to an ANYIP_ error code

@param[in]		error: socket error code

@return			ANYIP error code according this list (OpenBSD native codes):
				socket error | IP_PNIO error
				---------------------------------
				EWOULDBLOCK  | ANYIP_OK_ACTIVE
				EINPROGRESS  | ANYIP_OK_ACTIVE
				ECONNABORTED | ANYIP_ERR_REM_ABORT
				ECONNRESET	 | ANYIP_ERR_REM_ABORT
				ESHUTDOWN	 | ANYIP_ERR_REM_ABORT
				ENOMEM		 | ANYIP_ERR_RESOURCE
				ENOBUFS		 | ANYIP_ERR_RESOURCE
				EPIPE		 | ANYIP_ERR_REM_NET
				ETIMEDOUT	 | ANYIP_ERR_REM_NET
				others	     | ANYIP_ERR_LOC_NET
******************************************************************************/
unsigned int
sock_low_map_tcp_error(
	int error
);

/**************************************************************************//**
@ingroup			sock_low_anyip
@param  [in] err Configured stack's native result code
@return SOCK_LOW_IPSTACK_EMSGSIZE, if result code is EMSGSIZE,
@return SOCK_LOW_IPSTACK_EHOSTUNREACH if result code is EHOSTUNREACH (BSD) or ENP_NO_ROUTE(Interniche), 
@return otherwise 0
******************************************************************************/
unsigned char
sock_low_get_reason_code(
	int err
);

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
#endif  /* of SOCK_LOW_H */
