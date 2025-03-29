#ifndef TCIP_INT_H
#define TCIP_INT_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

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
/*  F i l e               &F: tcip_int.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Internal Headerfile                                                      */
/*                                                                           */
/*****************************************************************************/

#ifndef TCIP_MODULE_ID
# error "TCIP_MODULE_ID not defined!"
#endif

/*===========================================================================*/

#include "tcip_inc.h"
#include "anyip_inc.h"

#ifdef LSA_HOST_ENDIANESS_LITTLE
/* flips a byte pattern of 0102 to 0201 */
#define TCIP_FLIP16(C) ((LSA_UINT16) ( (((LSA_UINT16) (C))>>8) | ( ((LSA_UINT16)(C)) << 8)) )
/* flips a byte pattern of 01020304 to 04030201 */
#define TCIP_FLIP32(C) ((LSA_UINT32) (((C) >> 24) | (((C) & 0x00ff0000) >> 8) | (((C) & 0x0000ff00) <<  8) | (((C) << 24))) )
#else
#define TCIP_FLIP16(C)     (C)
#define TCIP_FLIP32(C)     (C)
#endif

#ifndef TCIP_NTOHS
#define TCIP_NTOHS(Value) ( TCIP_FLIP16(Value) )/* Network TO Host Short */
#endif

#ifndef TCIP_NTOHL
#define TCIP_NTOHL(Value) ( TCIP_FLIP32(Value) )/* Network TO Host Long */
#endif

#ifndef TCIP_HTONS
#define TCIP_HTONS(Value) ( TCIP_FLIP16(Value) )/* Host To Network Short */
#endif

#ifndef TCIP_HTONL
#define TCIP_HTONL(Value) ( TCIP_FLIP32(Value) )/* Host TO Network Long */
#endif

/*===========================================================================*/

#ifndef TCIP_LOG_FCT
#define TCIP_LOG_FCT(fn_)  /* empty */
#endif

/*===========================================================================*/

#define is_null(ptr_)			(LSA_HOST_PTR_ARE_EQUAL(TCIP_TEST_POINTER(ptr_), LSA_NULL))
#define is_not_null(ptr_)		(! LSA_HOST_PTR_ARE_EQUAL(TCIP_TEST_POINTER(ptr_), LSA_NULL))
#define are_equal(ptr1_, ptr2_)	(LSA_HOST_PTR_ARE_EQUAL(TCIP_TEST_POINTER(ptr1_), TCIP_TEST_POINTER(ptr2_)))

/*===========================================================================*/

/*===========================================================================*/
/*=== INTERFACE DATA ========================================================*/
/*===========================================================================*/

/*
 *  struct is created once for each interface
 */
typedef struct tcip_if_data_tag
{
	LSA_UINT32        nets_count; /* = total "interfaces" + total "ports" */
	LSA_UINT32        edd_interface_id;
	LSA_UINT16        port_count;

	struct {
		EDD_MAC_ADR_TYPE  MACAddr; /* interface or port mac address */

		LSA_UINT32        nominal_bandwidth; /* used for ifSpeed if link-down */

		ANYIP_IFNET_PTR  ifp;

		LSA_UINT8         PhyStatus; /* tcip_edd_get_portparams_done stores EDD's PhyStatus for each port */

	} port[1 + EDD_CFG_MAX_PORT_CNT]; /* index = PortID, 0 = the interface */

} TCIP_IF_DATA;

typedef struct tcip_if_data_tag * TCIP_IF_DATA_PTR_TYPE;


/*===========================================================================*/
/*=== CHANNEL ===============================================================*/
/*===========================================================================*/

enum tcip_channel_state_enum {
	TCIP_CH_FREE = 0,          /* this channel free */
	TCIP_CH_ALLOCATED,         /* channel is about openening (async going on for opening lower) */
	TCIP_CH_OPEN,              /* channel is open */
	TCIP_CH_CLOSING			   /* channel is about closing (async going on for closing lower);
							   --> state changes to CH_FREE when finished */
};


typedef struct tcip_channel_tag
{
	LSA_UINT16			  state;			/* see tcip_channel_state_enum */
	LSA_HANDLE_TYPE		  my_handle;		/* my own handle */
	LSA_HANDLE_TYPE		  handle_upper;		/* the handle of the user */
	LSA_SYS_PATH_TYPE     syspath;
	TCIP_UPPER_CALLBACK_FCT_PTR_TYPE callback;	/* the callback-function to the user */
	LSA_SYS_PTR_TYPE	  sysptr;			/* the 'hint' for the system adaption */
	TCIP_DETAIL_PTR_TYPE  detailptr;		/* pointer to channel-details */
	LSA_UINT32			  trace_idx;		/* for "LSA index trace" */
	TCIP_IF_DATA_PTR_TYPE p_if_data;		/* edd interface data - created when arp channel opens */

	/***/

	TCIP_UPPER_RQB_PTR_TYPE curr; /* current working upper request block for open / close channel */

	struct /* each channel has 1 lower-layer */
	{
		EDD_HANDLE_LOWER_TYPE handle_lower;

		/*
		 * resources
		 */
		LSA_VOID_PTR_TYPE send_rqbs;

		LSA_UINT32 in_work_rcv_rqbs_number; /* number of receive packets/rqb's used by iniche stack */

		LSA_UINT32 pending_rcv_reqests; /* used for resource cancellation */

		LSA_BOOL get_statistics_pending;

		TCIP_EDD_LOWER_RQB_PTR_TYPE pEdd; /* pre-allocated for EDD_SRV_GET_STATISTICS_ALL opcode */

	} edd;

} TCIP_CHANNEL_TYPE;

typedef struct tcip_channel_tag * TCIP_CHANNEL_PTR_TYPE;

typedef struct tcip_channel_tag * const TCIP_CHANNEL_CONST_PTR_TYPE;

typedef struct tcip_channel_tag const * TCIP_CONST_CHANNEL_PTR_TYPE;

typedef struct tcip_channel_tag const * const TCIP_CONST_CHANNEL_CONST_PTR_TYPE;

/*===========================================================================*/
/*=== GLOBAL DATA ===========================================================*/
/*===========================================================================*/

typedef struct tcip_data_tag
{

	TCIP_CHANNEL_TYPE  tcip_channels[TCIP_MAX_CHANNELS]; /* defined in tcip_usr.c */
	LSA_UINT8          tcip_channels_opened;             /* number of opened channels, defined in tcip_usr.c */

	LSA_TIMER_ID_TYPE  tcip_TimerID; /* see tcip_iniche_ctick() */
	TCIP_RQB_TYPE      tcip_TimerRQB;

	LSA_UINT32 get_statistics_pending;

} TCIP_DATA;

extern TCIP_DATA tcip_data;


/*===========================================================================*/

LSA_VOID tcip_create_and_activate_interfaces(TCIP_CHANNEL_PTR_TYPE arp_channel);
LSA_VOID tcip_deactivate_and_delete_interfaces(TCIP_CONST_CHANNEL_CONST_PTR_TYPE arp_channel);

/*===========================================================================*/
/*=== USER ==================================================================*/
/*===========================================================================*/

LSA_VOID tcip_callback( TCIP_CHANNEL_CONST_PTR_TYPE channel, LSA_UINT16 response, TCIP_UPPER_RQB_PTR_TYPE *upper_ptr );
LSA_VOID tcip_callback_and_free_channel(TCIP_CHANNEL_CONST_PTR_TYPE channel, LSA_UINT16 response, TCIP_UPPER_RQB_PTR_TYPE rb );

TCIP_CHANNEL_PTR_TYPE tcip_channel_from_handle( LSA_HANDLE_TYPE handle);

LSA_VOID tcip_init_arp_channel(TCIP_CHANNEL_CONST_PTR_TYPE channel);
LSA_VOID tcip_init_opt_channel(TCIP_CHANNEL_CONST_PTR_TYPE channel);

/*===========================================================================*/
/*=== EDD ===================================================================*/
/*===========================================================================*/

LSA_VOID tcip_open_channel_all(TCIP_CONST_CHANNEL_PTR_TYPE channel );
LSA_VOID tcip_edd_close_channel(TCIP_CONST_CHANNEL_PTR_TYPE channel);

LSA_VOID tcip_edd_nrt_cancel(TCIP_CHANNEL_CONST_PTR_TYPE channel );

LSA_VOID tcip_edd_nrt_recv( EDD_RQB_TYPE *pRQB, TCIP_CHANNEL_CONST_PTR_TYPE channel );

EDD_UPPER_RQB_PTR_TYPE tcip_AllocRQB(LSA_SYS_PTR_TYPE pSys, LSA_UINT16 param_length);
LSA_VOID tcip_FreeRQB( LSA_SYS_PTR_TYPE pSys, EDD_RQB_TYPE *pRQB );

/*===========================================================================*/
/*=== SEND ==================================================================*/
/*===========================================================================*/

LSA_VOID tcip_edd_nrt_send_done( TCIP_EDD_LOWER_RQB_PTR_TYPE  pRQB, TCIP_CHANNEL_CONST_PTR_TYPE   channel );

LSA_VOID tcip_initEddSendRQBList(EDD_UPPER_RQB_PTR_TYPE *SendRqbs, LSA_UINT32 Count, EDD_HANDLE_LOWER_TYPE  EDD_handle, LSA_SYS_PTR_TYPE pSys);

LSA_VOID tcip_cancel_send_resources(TCIP_CHANNEL_CONST_PTR_TYPE channel);


/*===========================================================================*/
/*=== RECV ==================================================================*/
/*===========================================================================*/

LSA_VOID tcip_edd_nrt_recv_done ( TCIP_EDD_LOWER_RQB_TYPE  *pRQB, TCIP_CHANNEL_CONST_PTR_TYPE channel );

LSA_VOID tcip_AllocRcvBuf(TCIP_CHANNEL_CONST_PTR_TYPE channel);
LSA_VOID tcip_FreeRcvBuf(TCIP_CONST_CHANNEL_PTR_TYPE channel, TCIP_EDD_LOWER_RQB_TYPE  *pRQB);


/*===========================================================================*/
/*=== MIB ===================================================================*/
/*===========================================================================*/

LSA_VOID
tcip_edd_ext_link_status_provide(
	TCIP_CONST_CHANNEL_PTR_TYPE channel
);

LSA_VOID
tcip_edd_ext_link_status_indication(
	TCIP_CONST_CHANNEL_PTR_TYPE channel,
	TCIP_EDD_LOWER_RQB_PTR_TYPE pRQB
);

LSA_VOID
tcip_edd_get_mib2_statistics_done(
	TCIP_CHANNEL_PTR_TYPE channel,
	TCIP_EDD_LOWER_RQB_CONST_PTR_TYPE pRQB
);


/*===========================================================================*/
/*=== MULTICAST SUPPORT =====================================================*/
/*===========================================================================*/

#if TCIP_CFG_ENABLE_MULTICAST
LSA_VOID
tcip_edd_srv_multicast(
	TCIP_CONST_CHANNEL_PTR_TYPE ch,
	LSA_INT32 dev_handle,
	LSA_UINT64 mc_address,
	LSA_UINT8 mode
);
#endif

/*===========================================================================*/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
#endif  /* of TCIP_INT_H */
