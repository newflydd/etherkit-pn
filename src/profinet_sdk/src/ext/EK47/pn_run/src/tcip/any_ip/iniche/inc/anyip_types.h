#ifndef __ANYIP_TYPES_H__
#define __ANYIP_TYPES_H__

/*---------------------------------------------------------------------------*/
/* Parameters defined in IP stack, exported for userland                     */
/* have to be adapted according the used stack -- checked in ip_pnio_types.c */
/*---------------------------------------------------------------------------*/
#define ANYIP_ECONNABORTED	5
#define ANYIP_EMSGSIZE		12
#define ANYIP_EHOSTUNREACH	-33
#define ANYIP_CLOCK_RATE_HZ	10
#define ANYIP_AF_INET			2
#define ANYIP_AF_INET6			3
#define ANYIP_ETHHDR_BIAS		0

#define ANYIP_SENDBUFFER_SIZE  1568 /* not EDD_FRAME_BUFFER_LENGTH, see AP00168784 */

/*--------------------------------------------------------------------------------*/
/* Parameters can be freely defined, only used for userland handover -- no checks */
/*--------------------------------------------------------------------------------*/
#define ANYIP_UNUSED_ARG(x) {(void)(x);}

#define ANYIP_IP  1
#define ANYIP_UDP 2
#define ANYIP_TCP 4

#define ANYIP_STATE_UP 1 /**<Definition of Link or Admin State UP -- shared with TCIP */
#define ANYIP_STATE_DOWN 0 /**<Definition of Link or Admin State DOWN -- shared with TCIP */

typedef char *ANYIP_PKTBUF_PTR;
typedef void *ANYIP_IFNET_PTR;
typedef long SOCK_LOW_HANDLE_TYPE; /* Interniche handle type */

/*-----------------------------------------------------------------------------------------*/
/* MUST NOT BE ADAPTED (compatiblitiy with PN stack), checked in tcip_sys.c and sock_sys.c */
/*-----------------------------------------------------------------------------------------*/

#define ANYIP_OK			    0x01 /**<Response code for success, can be used as return code in stack-specific implementations */
#define ANYIP_OK_ACTIVE		0x40 /**<Response code for ongoing operations in non-blocking context */
#define ANYIP_ERR_RESOURCE	0x84 /**<Response code for ressource error, can be used as return code in stack-specific implementations */
#define ANYIP_ERR_PARAM		0x86 /**<Response code for parameter error, can be used as return code in stack-specific implementations */
#define ANYIP_ERR_LOWER_LAYER	0xc0 /**<Response code for lower layer error, can be used as return code in stack-specific implementations */
#define ANYIP_ERR_REM_ABORT	0xc3 /**<Response code for remote network abort, can be used as return code in stack-specific implementations */
#define ANYIP_ERR_LOC_NET		0xc4 /**<Response code for local network error, can be used as return code in stack-specific implementations */
#define ANYIP_ERR_REM_NET		0xc5 /**<Response code for remote network error, can be used as return code in stack-specific implementations */
#define ANYIP_ERR_ADDR_INUSE  0xca /**<Response code if port is already in use, can be used as return code in stack-specific implementations */

#define ANYIP_FALSE 0
#define ANYIP_TRUE  (!ANYIP_FALSE)

#define ANYIP_TCIP_STATISTICS_PENDING_NO 0 /**<Indicates that no EDD statistics counter request is currently processed */
#define ANYIP_TCIP_STATISTICS_PENDING_YES 1 /**<Indicates that an EDD statistics counter request is currently processed */

#endif /* __ANYIP_TYPES_H__ */
