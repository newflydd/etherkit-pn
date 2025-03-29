#ifndef SOCK_USR_H
#define SOCK_USR_H
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
/*  F i l e               &F: sock_usr.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/* User-Interface of component SOCK                                          */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

/*------------------------------------------------------------------------------
//	OPCODES
//	SOCK_RQB_TYPE::opcode
//----------------------------------------------------------------------------*/

#define SOCK_OPC_OPEN_CHANNEL              1
#define SOCK_OPC_CLOSE_CHANNEL             2

#define SOCK_OPC_LISTEN                    3
#define SOCK_OPC_ACCEPT_NOTIFY             4
#define SOCK_OPC_CONNECT                   5
#define SOCK_OPC_CONNECT_WITH_OPEN         6
#define SOCK_OPC_SEND                      7
#define SOCK_OPC_RECEIVE                   8
#define SOCK_OPC_OPEN                      9
#define SOCK_OPC_CLOSE                    10
#define SOCK_OPC_EXCEPTION_NOTIFY         11
#define SOCK_OPC_CANCEL_CONNECTS          12

#define SOCK_OPC_UDP_OPEN                 13
#define SOCK_OPC_UDP_SEND                 14
#define SOCK_OPC_UDP_RECEIVE              15
#define SOCK_OPC_UDP_MULTICAST_OPEN       16
#define SOCK_OPC_UDP_CONNECT			  34

#define SOCK_OPC_SNMP_AGENT_ACTIVATE      18
#define SOCK_OPC_SNMP_AGENT_DEACTIVATE    19
#define SOCK_OPC_SNMP_RECEIVE_PROVIDE     20
#define SOCK_OPC_SNMP_RECEIVE_IND         21
#define SOCK_OPC_SNMP_RECEIVE_RSP         22

#define SOCK_OPC_SET_ADDR_INFO            23
#define SOCK_OPC_ADDR_INFO_IND            24

#define SOCK_OPC_SET_MIB2_IFGROUP_DATA    25
#define SOCK_OPC_SET_MIB2_SYSGROUP_DATA   26
#define SOCK_OPC_MIB2_SYSGROUP_DATA_IND   27
#define SOCK_OPC_GET_MIB2_IFINDEX         28

#define SOCK_OPC_reserved29               29 /* removed since V6.0i18 */

#define SOCK_OPC_GET_ANY_MAC_ADDR         30

#define SOCK_OPC_CNTRL_TIMEPARAMS         31

#define SOCK_OPC_SELECT_TRIGGER			  33

#define SOCK_OPC_CLUSTER_IP_CONFIG_SET    35
#define SOCK_OPC_CLUSTER_IP_STATE_SET       36
#define SOCK_OPC_CLUSTER_IP_GET           37
#define SOCK_OPC_CLUSTER_IP_IND           38

#define SOCK_OPC_SET_ROUTE                39

#define SOCK_OPC_IPV4_FORWARDING_SET      40
#define SOCK_OPC_IPV4_FORWARDING_GET      41

#define SOCK_OPC_UNUSED                 0xff

/*------------------------------------------------------------------------------
//	ERROR_CODES
//	SOCK_RQB_TYPE::response
//----------------------------------------------------------------------------*/

#define SOCK_RSP_OK						LSA_RSP_OK   /* without errors */

#define SOCK_RSP_ERR_RESOURCE           LSA_RSP_ERR_RESOURCE
#define SOCK_RSP_ERR_PARAM              LSA_RSP_ERR_PARAM
#define SOCK_RSP_ERR_SEQUENCE           LSA_RSP_ERR_SEQUENCE
#define SOCK_RSP_ERR_SYS_PATH           LSA_RSP_ERR_SYS_PATH

/* SOCK OK code starting at  LSA_RSP_OK_OFFSET = 0x40 */

#define SOCK_RSP_OK_ACTIVE             (LSA_RSP_OK_OFFSET +  0)    /* (0x40) RQB is active */
#define SOCK_RSP_OK_WITHDRAW           (LSA_RSP_OK_OFFSET +  1)    /* (0x41) RQB cancellation without error */
#define SOCK_RSP_OK_ENP_SEND_PENDING   (LSA_RSP_OK_OFFSET +  2)    /* (0x42) DomainNameService - Request is still running */

/* SOCK ERROR codes starting at LSA_RSP_ERR_OFFSET = 0xC0 */

#define SOCK_RSP_ERR_SYNTAX            (LSA_RSP_ERR_OFFSET +  0)   /* (0xc0) RQB has syntax error */
#define SOCK_RSP_ERR_REF               (LSA_RSP_ERR_OFFSET +  1)   /* (0xc1) wrong sock_fd */
#define SOCK_RSP_ERR_LOC_ABORT         (LSA_RSP_ERR_OFFSET +  2)   /* (0xc2) local connection abort */
#define SOCK_RSP_ERR_REM_ABORT         (LSA_RSP_ERR_OFFSET +  3)   /* (0xc3) remote connection abort */
#define SOCK_RSP_ERR_LOC_NET           (LSA_RSP_ERR_OFFSET +  4)   /* (0xc4) local net error */
#define SOCK_RSP_ERR_REM_NET           (LSA_RSP_ERR_OFFSET +  5)   /* (0xc5) remote net error */
#define SOCK_RSP_ERR_TIMEOUT           (LSA_RSP_ERR_OFFSET +  6)   /* (0xc6) timeout error  */
#define SOCK_RSP_ERR_LOWER_LAYER       (LSA_RSP_ERR_OFFSET +  7)   /* (0xc7) error from lower-layer */
#define SOCK_RSP_ERR_PROTOCOL          (LSA_RSP_ERR_OFFSET +  8)   /* (0xc8) protocol error */
#define SOCK_RSP_ERR_INVALID_HANDLE    (LSA_RSP_ERR_OFFSET +  9)   /* (0xc9) invalid channel handle */
#define SOCK_RSP_ERR_ADDR_INUSE        (LSA_RSP_ERR_OFFSET + 10)   /* (0xca) port already in use */
//#define SOCK_RSP_ERR_ADDR_NOT_AVAIL    (LSA_RSP_ERR_OFFSET + 11)   /* (0xcb) cannot use requested address */

#define SOCK_RSP_UNUSED                 0xff


/*------------------------------------------------------------------------------
//	socket types
//----------------------------------------------------------------------------*/

#define SOCK_AF_INET    2


/*------------------------------------------------------------------------------
//	user constants
//----------------------------------------------------------------------------*/

#define SOCK_MAX_MIBII_IFSTRING_LENGTH 255

/*------------------------------------------------------------------------------
//	invalid socket handle
//----------------------------------------------------------------------------*/

#define SOCK_INVALID_SOCKET_HANDLE ((LSA_INT32)-1)

/*------------------------------------------------------------------------------
//	global types
//----------------------------------------------------------------------------*/

#if defined(s_addr)
#   undef s_addr
#endif
#include "sys_pck1.h"
typedef union
{
	union
	{
		struct
		{
			LSA_UINT8 s_b1, s_b2, s_b3, s_b4;
		} S_un_b;
		struct
		{
			LSA_UINT16 s_w1, s_w2;
		} S_un_w;
		LSA_UINT32 S_addr;
	} S_un;
	LSA_UINT32 s_addr;
} SockIF_IPAddr;

typedef struct
{
	LSA_INT16     sin_family;
	LSA_UINT16    sin_port;
	SockIF_IPAddr sin_addr;
	char		  sin_zero[8];
} SockIF_AddrIn;

typedef SockIF_AddrIn SOCK_SOCKADDR_TYPE;

#define SOCK_UPPER_MEM_PTR_TYPE LSA_VOID *

/*-----------------------------------------------------------------------------
// sock common request block type
//---------------------------------------------------------------------------*/

struct sock_rqb_tag;

#define SOCK_UPPER_RQB_PTR_TYPE struct sock_rqb_tag *

#define SOCK_MAC_ADDR_SIZE  6

typedef struct sock_mac_addr_tag
{
	LSA_UINT8       Adr[SOCK_MAC_ADDR_SIZE];
} SOCK_MAC_ADR_TYPE;

/*===========================================================================*/

typedef struct sock_ip_suite_tag
{
	SockIF_IPAddr       IpAddr;  /* IP address */
	SockIF_IPAddr       NetMask; /* associated net mask */
} SOCK_IP_SUITE_TYPE;

typedef SOCK_IP_SUITE_TYPE * SOCK_UPPER_IP_SUITE_PTR_TYPE;

/*===========================================================================*/

#define SOCK_NAME_OF_STATION_LENGTH_MAX 255

#define SOCK_NAME_REMANENT_NO  0
#define SOCK_NAME_REMANENT_YES 1

typedef struct sock_name_tag
{
	LSA_UINT8  *pName;    /* NameOfStation */
	LSA_UINT16  Length;   /* length of NameOfStation */
	LSA_UINT8   Remanent; /* Remanenz flag system params of mib2 */
                          /* SOCK_NAME_REMANENT_NO ..... parameters are not stored remanent */
                          /* SOCK_NAME_REMANENT_YES .... parameters are stored remanent */
} SOCK_NAME_TYPE;

typedef SOCK_NAME_TYPE * SOCK_UPPER_NAME_PTR_TYPE;

/*------------------------------------------------------------------------------
//	SOCKET specific options
//----------------------------------------------------------------------------*/

typedef enum
{
	USE_CHANNEL_SETTING,   /* keep default setting from channel detail info */
	ENABLE,                /* activate socket option   */
	DISABLE                /* deactivate socket option */
} SOCKET_OPTION_VAL_TYPE;

typedef SOCKET_OPTION_VAL_TYPE * SOCKET_OPTION_VAL_PTR_TYPE;

/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_OPEN_CHANNEL
//---------------------------------------------------------------------------*/

typedef LSA_VOID LSA_FCT_PTR(/*ATTR*/, SOCK_UPPER_CALLBACK_FCT_PTR_TYPE)(SOCK_UPPER_RQB_PTR_TYPE upper_rqb_ptr);

#define SOCK_INVALID_HANDLE  ((LSA_HANDLE_TYPE)-1)

#define SOCK_NO_HANDLE       SOCK_INVALID_HANDLE /* deprecated! do not use! */

typedef struct sock_rqb_args_channel_tag
{
	LSA_HANDLE_TYPE         handle;         /* req: -                               */
											/* cnf: channel-handle of sock          */
	LSA_HANDLE_TYPE         handle_upper;   /* req: channel-handle of user          */
											/* cnf: -                               */
	LSA_SYS_PATH_TYPE       sys_path;       /* req: system-path of channel          */
											/* cnf: -                               */
	SOCK_UPPER_CALLBACK_FCT_PTR_TYPE  sock_request_upper_done_ptr;
											/* req: pointer to callback-function    */
											/* cnf: -                               */
} SOCK_RQB_ARGS_CHANNEL_TYPE;

typedef SOCK_RQB_ARGS_CHANNEL_TYPE * SOCK_RQB_ARGS_CHANNEL_PTR_TYPE;


/*-----------------------------------------------------------------------------
// OPCODE: SOCK_OPC_CLOSE_CHANNEL
//---------------------------------------------------------------------------*/

/* no paramters */


/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_LISTEN
//---------------------------------------------------------------------------*/

typedef struct sock_rqb_args_listen_tag
{
	LSA_INT32               sock_fd;    /* Req: - */
										/* Cnf: socket descriptor of created server socket */
	SOCK_SOCKADDR_TYPE      sockaddr;   /* Req: address of server socket, incl. port number of wellknown port */
										/* Cnf: - */
	LSA_INT16               backlog;    /* Req: max. number of waiting connection establishment requests */
										/*      max. length of queue for connection establishment requests */
										/* Cnf: - */
	SOCKET_OPTION_VAL_TYPE  reuse_addr; /* Req: socket specific option REUSE_ADDRESS specified by user */
										/* Cnf: - */
} SOCK_RQB_ARGS_LISTEN_TYPE;


typedef struct sock_rqb_args_listen_tag * SOCK_RQB_ARGS_LISTEN_PTR_TYPE;


/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_CONNECT
// OPCODE:	SOCK_OPC_ACCEPT_NOTIFY
//---------------------------------------------------------------------------*/

typedef struct sock_rqb_args_conn_tag
{
	LSA_INT32               sock_fd;    /* Req: - */
										/* Cnf: socket descriptor of the created socket */
	SOCK_SOCKADDR_TYPE      loc_addr;   /* Req: 0 */
										/* Cnf: source address of the station */
	SOCK_SOCKADDR_TYPE      rem_addr;   /* ACCEPT-Req: 0*/
										/* ACEPT-Cnf: source address of the remote partner */
										/* CONNECT-Req: destination address of the connection establishment desire */
										/* CONNECT-Cnf: - */
										/* in the addresses are filled IP address and port*/
	LSA_UINT32				InterfaceID;/* SOCK_OPC_ACCEPT_NOTIFY only: Req: - */
										/* Cnf: InterfaceID of interface the new socket belongs to */
	LSA_UINT8				reason;     /* Cnf: additional error information on failure if supported, see documentation */
	SOCKET_OPTION_VAL_TYPE  reuse_addr; /* Req: socket specific option REUSE_ADDRESS specified by user */
										/* Cnf: - */
} SOCK_RQB_ARGS_CONN_TYPE;

typedef struct sock_rqb_args_conn_tag * SOCK_RQB_ARGS_CONN_PTR_TYPE;


/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_UDP_CONNECT
//---------------------------------------------------------------------------*/

typedef struct sock_rqb_args_udp_conn_tag
{
	LSA_INT32               sock_fd;    /* Req: udp socket descriptor */
										/* Cnf: unchanged */
	SOCK_SOCKADDR_TYPE      rem_addr;   /* destination address */
										/* Cnf: unchanged */
} SOCK_RQB_ARGS_UDP_CONN_TYPE;

typedef struct sock_rqb_args_udp_conn_tag * SOCK_RQB_ARGS_UDP_CONN_PTR_TYPE;

typedef struct sock_rqb_args_udp_conn_tag const * SOCK_RQB_ARGS_UDP_CONN_CONST_PTR_TYPE;

/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_SEND
// OPCODE:	SOCK_OPC_UDP_SEND
// OPCODE:	SOCK_OPC_RECEIVE
// OPCODE:	SOCK_OPC_UDP_RECEIVE
//---------------------------------------------------------------------------*/

#define SOCK_SEND_ALL           1 /* note: this flag has no effect (for compatibility only) */
#define SOCK_READ_ALL           2
#define SOCK_PEEK               4

/**
 * For UDP send operations:
 * Indicate that routing decisions for outgoing UDP packets shall be overwritten by SOCK, 
 * such that CARP/Cluster IP interfaces are not used. 
 *
 * For UDP receive operations:
 * Incomming packets from Cluster IP interfaces shall be dropped.
 *
 * Rationale:
 * Normally, routing decisions are left to the IP stack. If a Cluster IP interface is a
 * viable hop for a packet, it may be selected. Aside from services that bind explicitly
 * to the IP of a Cluster IP interface, this is also true for services that bind to ANY_IP.
 * Normally, Cluster IP should be transparent to the client, so this behavior is fine.
 *
 * However, services "CLRPC" and "SNMP" MUST NOT receive or send on Cluster IP interfaces.
 * They can opt for setting this flag on RQBs so that SOCK enforces usage of a "standard"
 * IP/Ethernet interface. 
 * CLRPS must not use Cluster IP interfaces because Profinet is generally not compatible 
 * to Cluster IP. Therefore Profinet UDP services (specifically CLRPC) must not use them. 
 * For SNMP the problem is that multiple devices/entities are represented by the 
 * same IP address. So if a user queried data from a Cluster IP address and the roles of 
 * Primary and Backup change, the client would receive wildly varying data.
 *
 * @warning		Other SOCK users MUST NOT set the flag. 
 *
 * @note		For a more detailed description see LSA/SOCK specification, chapter 
 *				"UDP Opcodes" or SOCK interface documentation.
 * 
 * @note		For imlementation specific reasons, SNMP does not actually use the flag 
 *				but another mechanism to filter out traffic from and to CARP interfaces. 
 */
#define SOCK_STANDARD_IP_ONLY   8 /* confirm UDP datagrams sent to the standard IP address only  */

#define SOCK_IPSTACK_EMSGSIZE      1
#define SOCK_IPSTACK_EHOSTUNREACH  2

typedef struct sock_rqb_args_data_tag
{
	LSA_INT32                   sock_fd;        /* Req: socket descriptor of desired socket */
												/* Cnf: unchanged to request */
	SOCK_SOCKADDR_TYPE          loc_addr;       /* *****  relevant only for UDP: ********/
												/* SOCK_OPC_UDP_SEND: source address for outgoing UDP packet */
												/* SOCK_OPC_UDP_RECEIVE: destination address of received UDP packet */
	SOCK_SOCKADDR_TYPE          rem_addr;       /* *****  relevant only for UDP: ********/
												/* Req: SOCKADDR-Structure with the Destination-IP-Address and the Destination-Port */
												/*      127.0.0.1 stands for the local computer */
												/*      255.255.255.255 is the Broadcast-Address */
												/* Cnf: unchanged to request */
	LSA_UINT16                  data_length;    /* Req: number of bytes that have to be sent or have to be received */
												/* Cnf: unchanged to request */
	LSA_UINT16                  buffer_length;  /* Req: length of buffer */
												/* Cnf: unchanged to request */
	LSA_UINT16                  flags;          /* Req: indication of a special mode */
												/*      RECEIVE:    SOCK_PEEK   readout receive buffer for test purposes, date are further on in the buffer */
												/*                  READ_ALL    wait until all desired data were received */
												/*                              otherwise only the data are passed to the top that were received with the first t_recv */
												/* Cnf: relating to Request unchanged */
	LSA_UINT16                  offset;         /* Req: Offset in the buffer, from that it should be sent, */
												/*      or from this the received data should be entered */
	SOCK_UPPER_MEM_PTR_TYPE     buffer_ptr;     /* Req: pointer on the buffer with the send-data, for the received data */
												/* Cnf: relating to Request unchanged */
	LSA_UINT8                   reason;         /* Cnf: additional error information on failure if supported, see documentation */
} SOCK_RQB_ARGS_DATA_TYPE;

typedef struct sock_rqb_args_data_tag * SOCK_RQB_ARGS_DATA_PTR_TYPE;


/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_CLOSE
//---------------------------------------------------------------------------*/

typedef struct sock_rqb_args_close_tag
{
	LSA_INT32               sock_fd;    /* Req: Socketdescriptor of desired socket */
										/* Cnf: Socktedescriptor */
	SOCKET_OPTION_VAL_TYPE  graceful_shutdown;  /* Req: socket specific option LINGER specified by user */
												/* Cnf: - */
} SOCK_RQB_ARGS_CLOSE_TYPE;

typedef struct sock_rqb_args_close_tag * SOCK_RQB_ARGS_CLOSE_PTR_TYPE;


/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_CANCEL_CONNECTS
//---------------------------------------------------------------------------*/

/* no paramters */


/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_EXCEPTION_NOTIFY
//---------------------------------------------------------------------------*/

typedef struct sock_rqb_exception_tag
{
	LSA_INT32               sock_fd;    /* Req: Socketdescriptor of the socket that has to be monitored*/
										/* Cnf:  */
} SOCK_RQB_ARGS_EXCEPTION_TYPE;

typedef struct sock_rqb_exception_tag * SOCK_RQB_ARGS_EXCEPTION_PTR_TYPE;


/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_UDP_OPEN
//---------------------------------------------------------------------------*/

typedef struct sock_udp_open_tag
{
	LSA_INT32              sock_fd;     /* Cnf: socket descriptor */
	SOCK_SOCKADDR_TYPE     loc_addr;    /* Req: local IP address and portnumber */
	SOCKET_OPTION_VAL_TYPE reuse_addr;  /* Req: socket specific option value for REUSE_ADDRESS, specified by user */
										/* Cnf: - */
} SOCK_RQB_ARGS_UDP_OPEN_TYPE;

typedef struct sock_udp_open_tag * SOCK_RQB_ARGS_UDP_OPEN_PTR_TYPE;


/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_SNMP_AGENT_ACTIVATE
//---------------------------------------------------------------------------*/

#define SOCK_NO_SNMP_AGENT          0
#define SOCK_INTERNER_SNMP_AGENT    1
#define SOCK_EXTERNER_SNMP_AGENT    2

typedef struct sock_snmp_activate_tag
{
	LSA_UINT8       type;                   /* Req: 1 = internal SNMP agent */
											/*      2 = external SNMP agent */
	LSA_UINT16      snmp_handle;            /* Response: handle of SOCK     */
} SOCK_RQB_ARGS_SNMP_ACTIVATE_TYPE;


/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_SNMP_AGENT_DEACTIVATE
//---------------------------------------------------------------------------*/

typedef struct sock_snmp_deactivate_tag
{
	LSA_UINT16      snmp_handle;            /* Req: handle                  */
} SOCK_RQB_ARGS_SNMP_DEACTIVATE_TYPE;

typedef struct sock_snmp_deactivate_tag * SOCK_RQB_ARGS_SNMP_DEACTIVATE_PTR_TYPE;


/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_SNMP_RECEIVE_PROVIDE
// INDICATION:	SOCK_OPC_SNMP_RECEIVE_IND
// OPCODE:	SOCK_OPC_SNMP_RECEIVE_RSP
//---------------------------------------------------------------------------*/

#define SOCK_SNMP_BUFFER_SIZE		1400

#define SOCK_SNMP_DONE_NO  0
#define SOCK_SNMP_DONE_YES 1

typedef struct sock_snmp_receive_tag
{
	LSA_UINT32              snmp_handle;        /* Req: handle of SNMP agent, was allocated at Activate */
	SOCK_SOCKADDR_TYPE      rem_addr;           /* Ind: SOCKADDR-Struktur, includes the address of the remote partner */
	LSA_UINT16              data_length;        /* Ind: number of received bytes */
	LSA_UINT16              buffer_size;        /* Req: length of recv_buffer in bytes, must be equal to SOCK_SNMP_BUFFER_SIZE */
	SOCK_UPPER_MEM_PTR_TYPE buffer;				/* Req/Ind: pointer on the buffer with received data */
	LSA_UINT8               snmp_done;          /* Con/Req:                     */
												/* SOCK_SNMP_DONE_NO ... SNMP agent hasn't processed the request */
												/* SOCK_SNMP_DONE_YES .. SNMP agent has processed the request */
} SOCK_RQB_ARGS_SNMP_RECEIVE_TYPE;

typedef struct sock_snmp_receive_tag * SOCK_RQB_ARGS_SNMP_RECEIVE_PTR_TYPE;


/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_OPEN
//---------------------------------------------------------------------------*/

typedef struct sock_open_tag
{
	LSA_INT32              sock_fd; /* Req:  - */
									/* Cnf: socket descriptor of the applied socket */
} SOCK_RQB_ARGS_OPEN_TYPE;

typedef struct sock_open_tag * SOCK_RQB_ARGS_OPEN_PTR_TYPE;


/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_CONNECT_WITH_OPEN
//---------------------------------------------------------------------------*/

typedef SOCK_RQB_ARGS_CONN_TYPE      SOCK_RQB_ARGS_CONNECT_WITH_OPEN_TYPE;
typedef SOCK_RQB_ARGS_CONN_PTR_TYPE  SOCK_RQB_ARGS_CONNECT_WITH_OPEN_PTR_TYPE;


/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_UDP_MULTICAST_OPEN
//---------------------------------------------------------------------------*/

typedef struct sock_udp_open_multicast_tag
{
	LSA_INT32           sock_fd;        /* Cnf: Socket-Descriptor */
	LSA_UINT32          interface_id;   /* Req: ID of interface used */
	SOCK_SOCKADDR_TYPE  loc_addr;       /* Req: local IP address and portnumber */
	SockIF_IPAddr       group_addr;     /* Req: multicast group address (224.0.0.3 to 239.255.255.255, see http://www.iana.org/assignments/multicast-addresses/multicast-addresses.xml) */
	LSA_UINT8           multicast_ttl;  /* Req: TTL value of multicast traffic */
	SOCKET_OPTION_VAL_TYPE reuse_addr;  /* Req: socket specific option REUSE_ADDRESS specified by user */
										/* Cnf: - */
} SOCK_RQB_ARGS_UDP_OPEN_MULTICAST_TYPE;

typedef SOCK_RQB_ARGS_UDP_OPEN_MULTICAST_TYPE * SOCK_RQB_ARGS_UDP_OPEN_MULTICAST_PTR_TYPE;


/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_SELECT_TRIGGER
//---------------------------------------------------------------------------*/

typedef struct sock_select_trigger_tag
{
	volatile SOCK_EXCHANGE_TYPE inUse;
} SOCK_RQB_ARGS_SELECT_TRIGGER_TYPE;


/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_SET_ADDR_INFO
//---------------------------------------------------------------------------*/

#define SOCK_MAX_INTERFACE_ID 16 /* normativ */

#define SOCK_CURRENT_IPSUITE_IS_EMERGENCY_NO  0
#define SOCK_CURRENT_IPSUITE_IS_EMERGENCY_YES 1

#define SOCK_CURRENT_IPSUITE_CONFLICT_DETECTED_NO  0
#define SOCK_CURRENT_IPSUITE_CONFLICT_DETECTED_YES 1

typedef struct sock_addr_info_tag
{
	LSA_UINT32          InterfaceID;  /* Req: 1 .. SOCK_MAX_INTERFACE_ID */

	SOCK_NAME_TYPE      NoS; /* NameOfStation (the interface) name */
							 /* if not interested in NoS changes, set NoS.pName = LSA_NULL and NoS.Length = 0 */
	SOCK_IP_SUITE_TYPE  CurrentIPSuite; /* the currently active IP suite */
	SOCK_IP_SUITE_TYPE  ExpectedIPSuite; /* expected  IP suite, may be 0.0.0.0 */
	SOCK_IP_SUITE_TYPE  EmergencyIPSuite; /* emergency IP suite, may be 0.0.0.0 */

	/* which IPSuite is conflicting?
	* if CurrentIPSuite_is_emergency == SOCK_CURRENT_IPSUITE_IS_EMERGENCY_NO   --> look into ExpectedIPSuite
	* if CurrentIPSuite_is_emergency == SOCK_CURRENT_IPSUITE_IS_EMERGENCY_YES  --> look into EmergencyIPSuite
	*/

	LSA_UINT8           CurrentIPSuiteIsEmergency;  /* SOCK_CURRENT_IPSUITE_IS_EMERGENCY_YES if CurrentIPSuite = EmergencyIPSuite,    */
													/* SOCK_CURRENT_IPSUITE_IS_EMERGENCY_NO  if CurrentIPSuite = ExpectedIPSuite or 0.0.0.0/0.0.0.0/0.0.0.0 */
	LSA_UINT8           CurrentIPSuiteConflictDetected; /* SOCK_CURRENT_IPSUITE_CONFLICT_DETECTED_YES if the CurrentIPSuite is conflicting with another station, CurrentIPSuite is reset to 0.0.0.0 */

} SOCK_ADDR_INFO_TYPE;

typedef SOCK_ADDR_INFO_TYPE SOCK_RQB_ARGS_ADDR_INFO_TYPE;

typedef SOCK_ADDR_INFO_TYPE SOCK_RQB_ARGS_SET_ADDR_INFO_TYPE; /* Req: InterfaceID, NameOfStation and IP-Suite parameters */

typedef SOCK_ADDR_INFO_TYPE * SOCK_ADDR_INFO_PTR_TYPE;

typedef SOCK_ADDR_INFO_TYPE const * SOCK_ADDR_INFO_CONST_PTR_TYPE;

/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_GET_ANY_MAC_ADDR
//---------------------------------------------------------------------------*/

typedef struct sock_get_any_mac_addr
{
	SOCK_MAC_ADR_TYPE   MacAddr;            /* Cnf: MAC address of NIC */
	LSA_UINT32          InterfaceID;        /* Cnf: Interface Id of referenced interface */
} SOCK_RQB_ARGS_GET_ANY_MAC_ADDR_TYPE;

typedef struct sock_get_any_mac_addr * SOCK_RQB_ARGS_GET_ANY_MAC_ADDR_PTR_TYPE;


/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_CNTRL_TIMEPARAMS
//---------------------------------------------------------------------------*/

#define SOCK_GET_TIMEPARAMS                 1
#define SOCK_SET_TIMEPARAMS                 2
#define SOCK_SET_TIMEPARAMS_CLOSE_TCP_CONN  4

#define SOCK_NO_CHANGE_TIMEPARMS            0xfffffff0
#define SOCK_KEEPALIVE_OFF                  0

typedef struct sock_timeparams_tag
{
	LSA_UINT32          timeopt;
	LSA_UINT32          keepalivetime;
	LSA_UINT32          connectiontimeout;
	LSA_UINT32          retranstimeout;
	LSA_UINT32          retransmitcount;
} SOCK_RQB_ARGS_TIMEPARAMS;

typedef struct sock_timeparams_tag * SOCK_RQB_ARGS_TIMEPARAMS_PTR;


/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_SET_MIB2_IFGROUP_DATA
//---------------------------------------------------------------------------*/

typedef struct sock_mib2_ifgroup_data_tag
{
	LSA_UINT32     InterfaceID;    /* Interface Id of referenced interface */
	LSA_UINT16     PortID;         /* Port Id of referenced interface */
	SOCK_NAME_TYPE ifDescr;        /* description string of referenced interface */
} SOCK_MIB2_IFGROUP_DATA_TYPE;

typedef struct sock_mib2_ifgroup_data_tag * SOCK_MIB2_IFGROUP_DATA_PTR_TYPE;


/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_SET_MIB2_SYSGROUP_DATA
// INDICATION:	SOCK_OPC_MIB2_SYSGROUP_DATA_IND
//---------------------------------------------------------------------------*/

typedef struct sock_mib2_sysgroup_data_tag
{
	SOCK_NAME_TYPE  sysName;        /* system name	*/
	SOCK_NAME_TYPE  sysDescr;       /* system description	*/
	SOCK_NAME_TYPE  sysContact;     /* system contact information */
	SOCK_NAME_TYPE  sysLocation;    /* system location */

} SOCK_MIB2_SYSGROUP_DATA_TYPE;

typedef struct sock_mib2_sysgroup_data_tag * SOCK_MIB2_SYSGROUP_DATA_PTR_TYPE;
typedef struct sock_mib2_sysgroup_data_tag const * SOCK_MIB2_SYSGROUP_DATA_CONST_PTR_TYPE;

/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_GET_MIB2_IFINDEX
//---------------------------------------------------------------------------*/

typedef struct sock_getmib2_ifindex_tag
{
	LSA_UINT32 InterfaceID;  /* Interface Id of referenced interface */
	LSA_UINT16 PortID;  /* Port Id of referenced interface */
	LSA_UINT32 IfIndex; /* Interface Id of referenced interface */

} SOCK_GET_MIB2_IFINDEX_TYPE;

typedef struct sock_getmib2_ifindex_tag * SOCK_GET_MIB2_IFINDEX_PTR_TYPE;


/*-----------------------------------------------------------------------------
// INDICATION:	SOCK_OPC_ADDR_INFO_IND
//---------------------------------------------------------------------------*/

typedef struct sock_addr_info_ind_tag
{
	SOCK_ADDR_INFO_TYPE  AddrInfo[SOCK_CFG_MAX_INTERFACE_CNT]; /* IN/OUT, space for MAX infos, key = AddrInfo[].InterfaceID */
	LSA_UINT8            InterfacesCount; /* IN; Number of interfaces which shall be indicated  */
	LSA_UINT32			 InterfacesChanged; /* OUT; bit field: index of record in AddrInfo indicated -> bit ( 1 << index) is set on indication */

} SOCK_ADDR_INFO_IND_TYPE;

typedef struct sock_addr_info_ind_tag * SOCK_ADDR_INFO_IND_PTR_TYPE;

/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_CLUSTER_IP_CONFIG_SET
//---------------------------------------------------------------------------*/

typedef struct sock_cluster_ip_config_tag
{
	LSA_UINT32 	       InterfaceID;			/* Interface Id of referenced interface */
	LSA_UINT8          ClusterID;   	   	/* Cluster ID */
	SockIF_IPAddr      ClusterIPAddress;   	/* Cluster IP */
	SockIF_IPAddr      ClusterSubnetMask;  	/* Cluster subnet mask */
} SOCK_CLUSTER_IP_CONFIG_TYPE;

typedef struct sock_cluster_ip_config_tag * SOCK_CLUSTER_IP_CONFIG_PTR_TYPE;

/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_CLUSTER_IP_STATE_SET
//---------------------------------------------------------------------------*/

#define SOCK_CLUSTER_IP_STATE_INACTIVE 0
#define SOCK_CLUSTER_IP_STATE_ACTIVE  1

typedef struct sock_cluster_ip_state_tag
{
	LSA_UINT32    InterfaceID;    /* Interface Id of referenced interface */
	LSA_UINT8     ClusterIPState; /* SOCK_CLUSTER_IP_STATE_INACTIVE or SOCK_CLUSTER_IP_STATE_ACTIVE */

} SOCK_CLUSTER_IP_STATE_TYPE;

typedef struct sock_cluster_ip_state_tag * SOCK_CLUSTER_IP_STATE_PTR_TYPE;


/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_CLUSTER_IP_GET
//---------------------------------------------------------------------------*/

typedef struct sock_cluster_ip_info_tag
{
	LSA_UINT32     InterfaceID;       /* Interface Id of referenced interface */
	LSA_UINT8      ClusterID;         /* Cluster ID */
	LSA_UINT8      ClusterIPState;    /* SOCK_CLUSTER_IP_STATE_INACTIVE or SOCK_CLUSTER_IP_STATE_ACTIVE */
	SockIF_IPAddr  ClusterIPAddress;  /* Cluster IP */
	SockIF_IPAddr  ClusterSubnetMask; /* Cluster subnet mask */
} SOCK_CLUSTER_IP_INFO_TYPE;

typedef struct sock_cluster_ip_info_tag * SOCK_CLUSTER_IP_INFO_PTR_TYPE;

/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_CLUSTER_IP_IND
//---------------------------------------------------------------------------*/

#define SOCK_CLUSTER_IP_NO_CONFLICT 0
#define SOCK_CLUSTER_IP_CONFLICT_DETECTED 1

typedef struct sock_cluster_ip_ind_tag
{
	SOCK_CLUSTER_IP_INFO_TYPE Info;
	LSA_UINT8 ClusterIPConflict; /* SOCK_CLUSTER_IP_NO_CONFLICT or SOCK_CLUSTER_IP_CONFLICT_DETECTED */
} SOCK_CLUSTER_IP_IND_TYPE;

typedef struct sock_cluster_ip_ind_tag * SOCK_CLUSTER_IP_IND_PTR_TYPE;


/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_SET_ROUTE
//---------------------------------------------------------------------------*/

typedef struct sock_route_tag
{
	SockIF_IPAddr IpAddr; /* IP address */
	SockIF_IPAddr NetMask; /* associated net mask */
	SockIF_IPAddr Gateway; /* associated gateway address */
} SOCK_ROUTE_TYPE;

typedef struct sock_set_route_tag
{
	SOCK_ROUTE_TYPE route;
} SOCK_SET_ROUTE_TYPE;

typedef struct sock_set_route_tag* SOCK_SET_ROUTE_PTR_TYPE;

/*-----------------------------------------------------------------------------
// OPCODE:	SOCK_OPC_IPV4_FORWARDING_SET
// OPCODE:	SOCK_OPC_IPV4_FORWARDING_GET
//---------------------------------------------------------------------------*/

#define SOCK_IP_FORWARDING_OFF 0x00
#define SOCK_IP_FORWARDING_ON  0x01

typedef struct sock_ip_forwarding_tag
{
	LSA_UINT8 State; /* SOCK_IP_FORWARDING_OFF, etc. */
} SOCK_IP_FORWARDING_TYPE;

typedef struct sock_ip_forwarding_tag * SOCK_IP_FORWARDING_PTR_TYPE;

/*-----------------------------------------------------------------------------
// sock request block definition
//---------------------------------------------------------------------------*/

typedef union sock_rqb_args_tag
{
	SOCK_RQB_ARGS_CHANNEL_TYPE              channel; /* SOCK_OPC_OPEN_CHANNEL */
	SOCK_RQB_ARGS_LISTEN_TYPE               listen; /* SOCK_OPC_LISTEN */
	SOCK_RQB_ARGS_CONN_TYPE                 conn; /* SOCK_OPC_CONNECT, SOCK_OPC_ACCEPT_NOTIFY */
	SOCK_RQB_ARGS_DATA_TYPE                 data; /* SOCK_OPC_SEND, SOCK_OPC_UDP_SEND, SOCK_OPC_RECEIVE, SOCK_OPC_UDP_RECEIVE */
	SOCK_RQB_ARGS_CLOSE_TYPE                close; /* SOCK_OPC_CLOSE */
	SOCK_RQB_ARGS_EXCEPTION_TYPE            except_rq; /* SOCK_OPC_EXCEPTION_NOTIFY */
	SOCK_RQB_ARGS_UDP_OPEN_TYPE             udp_open; /* SOCK_OPC_UDP_OPEN */
	SOCK_RQB_ARGS_UDP_OPEN_MULTICAST_TYPE   udp_open_multicast; /* SOCK_OPC_UDP_MULTICAST_OPEN */
	SOCK_RQB_ARGS_SNMP_ACTIVATE_TYPE        snmp_act; /* SOCK_OPC_SNMP_AGENT_ACTIVATE */
	SOCK_RQB_ARGS_SNMP_DEACTIVATE_TYPE      snmp_deact; /* SOCK_OPC_SNMP_AGENT_DEACTIVATE */
	SOCK_RQB_ARGS_SNMP_RECEIVE_TYPE         snmp_recv; /* SOCK_OPC_SNMP_RECEIVE_PROVIDE, SOCK_OPC_SNMP_RECEIVE_IND, SOCK_OPC_SNMP_RECEIVE_RSP */
	SOCK_RQB_ARGS_OPEN_TYPE                 open; /* SOCK_OPC_OPEN */
	SOCK_RQB_ARGS_CONNECT_WITH_OPEN_TYPE    connect_with_open; /* SOCK_OPC_CONNECT_WITH_OPEN */
	SOCK_RQB_ARGS_SET_ADDR_INFO_TYPE        set_addr_info; /* SOCK_OPC_SET_ADDR_INFO */
	SOCK_RQB_ARGS_GET_ANY_MAC_ADDR_TYPE     get_any_mac_addr; /* SOCK_OPC_GET_ANY_MAC_ADDR */
	SOCK_ADDR_INFO_IND_TYPE                 addr_info_ind; /* SOCK_OPC_ADDR_INFO_IND */
	SOCK_MIB2_IFGROUP_DATA_TYPE             set_mib2_ifgroup_data; /* SOCK_OPC_SET_MIB2_IFGROUP_DATA */
	SOCK_MIB2_SYSGROUP_DATA_TYPE            set_mib2_sysgroup_data; /* SOCK_OPC_SET_MIB2_SYSGROUP_DATA */
	SOCK_MIB2_SYSGROUP_DATA_TYPE            mib2_sysgroup_data_ind; /* SOCK_OPC_MIB2_SYSGROUP_DATA_IND */
	SOCK_GET_MIB2_IFINDEX_TYPE              get_mib2_ifindex; /* SOCK_OPC_GET_MIB2_IFINDEX */
	SOCK_RQB_ARGS_TIMEPARAMS                timeparam_rq; /* SOCK_OPC_CNTRL_TIMEPARAMS */
	SOCK_RQB_ARGS_SELECT_TRIGGER_TYPE       select_trigger; /* SOCK_OPC_SELECT_TRIGGER */
	SOCK_RQB_ARGS_UDP_CONN_TYPE             udp_conn; /* SOCK_OPC_UDP_CONNECT */
	SOCK_CLUSTER_IP_CONFIG_TYPE             cluster_ip_config; /* SOCK_OPC_CLUSTER_IP_CONFIG_SET */
	SOCK_CLUSTER_IP_STATE_TYPE              cluster_ip_state; /* SOCK_OPC_CLUSTER_IP_STATE_SET*/
	SOCK_CLUSTER_IP_INFO_TYPE               cluster_ip_get; /* SOCK_OPC_CLUSTER_IP_GET */
	SOCK_CLUSTER_IP_IND_TYPE                cluster_ip_ind; /* SOCK_OPC_CLUSTER_IP_IND */
	SOCK_SET_ROUTE_TYPE                     set_route; /* SOCK_OPC_SET_ROUTE */
	SOCK_IP_FORWARDING_TYPE                 ip_forwarding; /* SOCK_OPC_IPV4_FORWARDING_SET, SOCK_OPC_IPV4_FORWARDING_GET */
} SOCK_RQB_ARGS_TYPE;


typedef struct sock_rqb_tag
{
	SOCK_RQB_HEADER                                 /* insert xx-lsa-header as define */
	LSA_RQB_LOCAL_ERROR_TYPE          err;          /* not set by SOCK (usage deprecated) */
	SOCK_RQB_ARGS_TYPE                args;         /* see  SOCK_RQB_ARGS_TYPE     */
	SOCK_RQB_TRAILER                                /* insert additional trailer   */
} SOCK_RQB_TYPE;

#include "sys_unpck.h"

/*-----------------------------------------------------------------------------
// user functions
//---------------------------------------------------------------------------*/

/*=============================================================================
 * function name:  sock_open_channel
 *
 * function:       open a communication channel
 *
 * documentation:  LSA_SOCK_USR_SPEC_en.doc
 *===========================================================================*/
LSA_VOID sock_open_channel(
	SOCK_UPPER_RQB_PTR_TYPE  upper_rqb_ptr
);


/*=============================================================================
 * function name:  sock_close_channel
 *
 * function:       close a communication channel
 *
 * documentation:  LSA_SOCK_USR_SPEC_en.doc
 *===========================================================================*/
LSA_VOID sock_close_channel(
	SOCK_UPPER_RQB_PTR_TYPE  upper_rqb_ptr
);


/*=============================================================================
 * function name:  sock_request
 *
 * function:       send a request to a communication channel
 *
 * documentation:  LSA_SOCK_USR_SPEC_en.doc
 *===========================================================================*/
LSA_VOID sock_request(
	SOCK_UPPER_RQB_PTR_TYPE  upper_rqb_ptr
);


/*=============================================================================
 * function name:  sock_doselect
 *
 * function:       start of Select-mimic
 *
 * documentation:  LSA_SOCK_USR_SPEC_en.doc
 *===========================================================================*/
LSA_VOID sock_doselect(
	SOCK_UPPER_RQB_PTR_TYPE rqb_ptr
);


/*=============================================================================
 * function name:  sock_doselect2
 *
 * function:       start of Select-mimic
 *
 * documentation:  LSA_SOCK_USR_SPEC_en.doc
 *===========================================================================*/
LSA_VOID sock_doselect2(
	LSA_INT32 timeout
);

/*---------------------------------------------------------------------------*/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
#endif  /* of SOCK_USR_H */
