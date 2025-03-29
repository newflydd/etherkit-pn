#ifndef CM_ICL_H
#define CM_ICL_H

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
/*  C o m p o n e n t     &C: CM (Context Management)                   :C&  */
/*                                                                           */
/*  F i l e               &F: cm_icl.h                                  :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Internal header file - client things                                     */
/*                                                                           */
/*****************************************************************************/

/*----------------------------------------------------------------------------*/

#define CM_COUNTER_INCREMENT(counter) {\
		(counter) += 1; \
		/* old: if ((counter) == 0) { (counter) = 1; } */ \
	}


/*----------------------------------------------------------------------------*/

#define CM_AR_IS_SINGLE(prop_)		(((prop_) & CM_AR_PROP_COMPANION_AR_MASK) == CM_AR_PROP_COMPANION_AR_SINGLE)
/*#define CM_AR_IS_FIRST(prop_)		(((prop_) & CM_AR_PROP_COMPANION_AR_MASK) == CM_AR_PROP_COMPANION_AR_FIRST)*/
/*#define CM_AR_IS_COMPANION(prop_)	(((prop_) & CM_AR_PROP_COMPANION_AR_MASK) == CM_AR_PROP_COMPANION_AR_COMPANION)*/

/*----------------------------------------------------------------------------*/

#define CM_CL_DEV_SCAN_CYCLE_10MS(device_mode_) \
	(((device_mode_) & CM_CL_DEVICE_MODE_SCAN_CYCLE_10MS_MASK) /* no shift */)

/*----------------------------------------------------------------------------*/

#define CM_CL_INVALID_DEVICE_NR		0xFFFF

/*----------------------------------------------------------------------------*/


	/* user indicated ar-state */
#define CM_CL_AR_NOI		0
#define CM_CL_AR_PASSIVE	1
#define CM_CL_AR_WF_IN		2
#define CM_CL_AR_INDATA		3
#define CM_CL_AR_WF_DOWN	4


	/* internal ar state see cm_cl_arsm() */
#define CM_DST_PASSIVE				2

	/* go active, CM_CL_AR_WF_IN */
#define CM_DST_WF_SCHEDULE			3			/* wf run from schedule */
#define CM_DST_NARE					4
/*#define CM_DST_REBIND				5*/
#define CM_DST_CONNECT				6
#define CM_DST_COM_START			7
#define CM_DST_COM_ACTIVATE			8
#define CM_DST_PRM					9
/*#define CM_DST_PRM_END			10*/
#define CM_DST_WF_APPL_READY_IND	11
#define CM_DST_WF_NEW_DATA			12

	/* wanted state, CM_CL_AR_INDATA */
#define CM_DST_IN_DATA				20	/* 0x14 */

	/* CM_CL_AR_WF_IN => CM_CL_AR_WF_DOWN */
#define CM_DST_WF_DOWN				30	/* 0x1e */

	/* CM_CL_AR_INDATA => CM_CL_AR_WF_DOWN **
#define CM_DST_ABORT				40	** 0x28 */


	/* guess device type */
#define CM_CL_DEV_TYPE_UNKNOWN		0
#define CM_CL_DEV_TYPE_V2			2 /* device uses SubmoduleState.FormatIndicator == 0 */
#define CM_CL_DEV_TYPE_V3			3 /* device uses SubmoduleState.FormatIndicator == 1 */


	/* application ready state */
#define CM_CL_APPL_READY_STATE_UNKNOWN_CONNECT	0
#define CM_CL_APPL_READY_STATE_UNKNOWN_PLUG		1
#define CM_CL_APPL_READY_STATE_UP				9


	/* prm-state, see new state-machine */
#define CM_CL_PRM_STATE_DOWN			0
#define CM_CL_PRM_STATE_PRM				2
#define CM_CL_PRM_STATE_UP				4
#define CM_CL_PRM_STATE_WF_DOWN			6


	/* prm-running sequence withing state PRM */
#define CM_CL_PRMMODE_IDLE				0
/*      CM_CL_PRMMODE_AR				1	// see cm_usr.h */
/*      CM_CL_PRMMODE_SUBMODULE			2	// see cm_usr.h */
/*      CM_CL_PRMMODE_COC				3	// see cm_usr.h */
#define CM_CL_PRMMODE_CONNECT			10
#define CM_CL_PRMMODE_PLUG				11


	/* alarm enabled state */
#define CM_CL_ALARM_ENABLED_IGNORE		0
#define CM_CL_ALARM_ENABLED_DELAY		1 /* keep  all APDU-Status and Alarms */
#define CM_CL_ALARM_ENABLED_FORWARD		3 /* forward all APDU-Status and Alarms */
/*
** old state: CM_CL_ALARM_ENABLED_FORWARD_DELAY_PLUG 2 // forward APDU-Status, forward all Alarms expect Plug and ReturnOfSubmodule
** old state: CM_CL_ALARM_ENABLED_PBE                4 // within PrmUpdate-Begin-End sequence, keep Plug/Release, reject all others
*/


	/* bindsm states */
#define CM_CL_BINDSM_STATE_UNKNOWN			0
#define CM_CL_BINDSM_STATE_IDENTIFIED		2


	/* CM_WF - wait_for bit helper */
#define CM_CL_WF_IS_SET(wait_for, wf_flag)	\
	(((wait_for[wf_flag & 0xF]) & (wf_flag)) != 0)

#define CM_CL_WF_SET(wait_for, wf_flag)		\
	{ CM_ASSERT(!CM_CL_WF_IS_SET(wait_for,wf_flag)); wait_for[wf_flag & 0xF] |=  (wf_flag & 0xFFFFFFF0); }

#define CM_CL_WF_CLEAR(wait_for, wf_flag)	\
	{ CM_ASSERT( CM_CL_WF_IS_SET(wait_for,wf_flag)); wait_for[wf_flag & 0xF] &= ~(wf_flag & 0xFFFFFFF0); }

#define CM_CL_WF_CLEAR_IF_SET(wait_for, wf_flag) \
	if (CM_CL_WF_IS_SET(wait_for, wf_flag)) { CM_CL_WF_CLEAR(wait_for, wf_flag); }


	/* client wait_for */
	/* index 0 */
#define CM_CL_WF_AR_ADD					0x00000010	/* ProvAdd + ConsAdd + AlarmAdd */
#define CM_CL_WF__00000020				0x00000020
#define CM_CL_WF__00000040				0x00000040
#define CM_CL_DO_CONS_SET2UNK_ENPRAS	0x00000080

#define CM_CL_WF_PROV_ACTIVATE			0x00000100	/* ProvActivate */
#define CM_CL_WF_PROV_ACTIVATE_AUX		0x00000200
#define CM_CL_DO_PROV_PASSIVATE_AUX		0x00000400
#define CM_CL_WF_PROV_PASSIVATE_AUX		0x00000800

#define CM_CL_WF_CONS_ACTIVATE			0x00001000	/* ConsActivate */
#define CM_CL_WF_CONS_ACTIVATE_SET2UNK	0x00002000	/* ConsActivate + Set2Unk */
#define CM_CL_WF_CONS_SET2UNK			0x00004000	/* ConsSet2Unknown */
#define CM_CL_WF_CONS_SET2UNK_ENPRAS	0x00008000	/* ConsSet2Unknown + EnableProviderAutostop */

#define CM_CL_WF_NARE_CNF				0x00010000
#define CM_CL_WF_NARE_CANCEL_CNF		0x00020000
#define CM_CL_WF_REBIND_CNF				0x00040000
#define CM_CL_WF_HELLO_FILTER_REPLACE	0x00080000

#define CM_CL_WF_RPC_CONNECT_CNF		0x00100000
#define CM_CL_WF_RPC_RELEASE_CNF		0x00200000
#define CM_CL_WF_RPC_PRM_END_CNF		0x00400000
#define CM_CL_WF_RPC_RECORD_CNF			0x00800000 /* user record read or write */

#define CM_CL_WF_READY_FOR_RTC3			0x01000000
#define CM_CL_DO_PRE_DATA_IND			0x02000000
#define CM_CL_WF_PRE_DATA_RSP			0x04000000
#define CM_CL_WF_IN_DATA_RSP			0x08000000

#define CM_CL_WF_DATA_STATUS_RSP		0x10000000
#define CM_CL_DO_PRE_OFFLINE_IND		0x20000000
#define CM_CL_WF_PRE_OFFLINE_RSP		0x40000000
#define CM_CL_WF__80000000				0x80000000

	/* index 1 */
#define CM_CL_DO_OFFLINE_NEW_SESSION	0x00000011
#define CM_CL_DO_OFFLINE_IND			0x00000021
#define CM_CL_WF_OFFLINE_RSP			0x00000041
#define CM_CL_WF_AR_ABORT_RSP			0x00000081

#define CM_CL_DO_FRAMES_PASSIVATE		0x00000101
#define CM_CL_WF_FRAMES_PASSIVATE_DONE	0x00000201
#define CM_CL_DO_ALARMS_STOP			0x00000401
#define CM_CL_WF_ALARMS_STOP_DONE		0x00000801

#define CM_CL_WF__00001001				0x00001001	/* was _DO_FRAMES_STOP */
#define CM_CL_WF__00002001				0x00002001	/* WAS _WF_FRAMES_STOP_DONE */
#define CM_CL_DO_PD_DETACH				0x00004001
#define CM_CL_WF_RPC_CANCEL_CNF			0x00008001

#define CM_CL_WF_ALARM_LOW_RSP			0x00010001
#define CM_CL_WF_ALARM_HIGH_RSP			0x00020001
#define CM_CL_WF_PRM_PLUG_ACK_CNF		0x00040001
#define CM_CL_WF__00080001				0x00080001

		/* for PRM-sm */
#define CM_CL_WF_PRM_DONE				0x00100001	/* accumulative bit for arsm() */
#define CM_CL_DO_PRM_START				0x00200001
#define CM_CL_DO_RPC_PRM_BEGIN			0x00400001
#define CM_CL_DO_RPC_PRM_END			0x00800001

#define CM_CL_WF_PRM_BEGIN_RSP			0x01000001
#define CM_CL_WF_PRM_INDATA_RSP			0x02000001
#define CM_CL_WF_RPC_PRM_WRITE_CNF		0x04000001
#define CM_CL_WF_PRM_APPL_RDY_IND		0x08000001

#define CM_CL_WF_RPC_PRM_BEGIN_CNF		0x10000001
#define CM_CL_DO_ALARM_ROSAL_IND		0x20000001
#define CM_CL_WF__40000001				0x40000001
#define CM_CL_WF__80000001				0x80000001


	/* device state */
#define CM_CL_DEV_ST_NOI			0
#define CM_CL_DEV_ST_PASSIVE		1
#define CM_CL_DEV_ST_ACTIVE			2
#define CM_CL_DEV_ST_REMOVING		3


	/* device _internal_ RunMode (one of CM_CL_RMODE_RESERVED_MASK) */
#define CM_CL_RMODE__I_HELLO_FILTER_DYNAMIC		0x01000000 /* AT+FSU: set edd-hello-filter if IOC-NoS is changed */


	/* device wait_for */
#define CM_CL_DEV_NOTHING_TODO					0x00000000

	/* add/remove */
#define CM_CL_DEV__WF_ADD_DONE					0x00000001
#define CM_CL_DEV__WF_REMOVE_DONE				0x00000002
#define CM_CL_DEV__00000004						0x00000004
#define CM_CL_DEV__00000008						0x00000008

#define CM_CL_DEV__DO_BIND_REQ					0x00000010
#define CM_CL_DEV__WF_BIND_CNF					0x00000020
#define CM_CL_DEV__DO_UNBIND_REQ				0x00000040
#define CM_CL_DEV__WF_UNBIND_CNF				0x00000080

#define CM_CL_DEV__DO_SET_HELLO_FILTER_REQ		0x00000100	/* set edd-hello-filter with device-add */
#define CM_CL_DEV__WF_SET_HELLO_FILTER_CNF		0x00000200
#define CM_CL_DEV__DO_CLEAR_HELLO_FILTER_REQ	0x00000400
#define CM_CL_DEV__WF_CLEAR_HELLO_FILTER_CNF	0x00000800

#define CM_CL_DEV__DO_FRAME_ADD_REQ				0x00001000
#define CM_CL_DEV__WF_FRAME_ADD_CNF				0x00002000
#define CM_CL_DEV__DO_FRAME_REMOVE_REQ			0x00004000
#define CM_CL_DEV__WF_FRAME_REMOVE_CNF			0x00008000

#define CM_CL_DEV__IS_RESET_TO_FACTORY			0x00010000
#define CM_CL_DEV__DO_FRAME_RTF_REQ				0x00020000
#define CM_CL_DEV__WF_FRAME_RTF_CNF				0x00040000
#define CM_CL_DEV__00080000						0x00080000

	/* event */
#define CM_CL_DEV__WF_SINGLE_INDATA				0x00100000

	/* dynamic hello-filter */
#define CM_CL_DEV__HELLO_FILTER_NAME_CHANGED	0x01000000
#define CM_CL_DEV__WF_HELLO_FILTER_REPLACE		0x02000000


	/* mapped nare response codes */
enum cm_cl_nare_rsp_enum
{
	CM_CL_NARE_OK,
	CM_CL_NARE_DO_SET,
	CM_CL_NARE_DO_IDENTIFY_FSU,
	CM_CL_NARE_ERR_LL,
	CM_CL_NARE_ERR_DCP_ID_MISMATCH,
	CM_CL_NARE_ERR_STATION_NAME_DIFFERENT,
	CM_CL_NARE_ERR_DCP_STATION_NOT_FOUND,
	CM_CL_NARE_ERR_DCP_MULTIPLE_STATIONS,
	CM_CL_NARE_ERR_DCP_SET,
	CM_CL_NARE_ERR_MULTIPLE_IP_USE,
	CM_CL_NARE_ERR_FAULTY_TLV
};
typedef enum cm_cl_nare_rsp_enum CM_CL_NARE_RSP_TYPE;


typedef struct cm_cl_nare_alias_tag {
	CM_LIST_TYPED_ENTRY_TYPE(cm_cl_nare_alias_tag)
					link;				/* link to next entry, see list-macros in cm_list.h */

	LSA_UINT16		name_length;		/* length of name */
	CM_COMMON_MEM_U8_PTR_TYPE
					name;				/* not null-terminated alias-name of IO-Device */
} CM_CL_NARE_ALIAS_TYPE;

typedef CM_CL_NARE_ALIAS_TYPE * CM_CL_NARE_ALIAS_PTR_TYPE;

typedef struct cm_cl_nare_tag {
	LSA_UINT32		properties;						/* CM_NARE_MODE_xxx */
	LSA_UINT16		device_name_length;				/* length of device_name */
	CM_UPPER_MEM_U8_PTR_TYPE
					device_name;					/* not null-terminated name of IO-Device */
	LSA_UINT16		tailored_device_name_length;	/* length of tailored device_name */
	CM_UPPER_MEM_U8_PTR_TYPE
					tailored_device_name;			/* not null-terminated tailored name of IO-Device */
	LSA_UINT32		device_ip;						/* ip address if CM_NARE_MODE_CONFIG_IP_FIXED */
	LSA_UINT32		device_subnet_mask;				/* subnet-mask if CM_CL_DEVICE_MODE_USE_DEF_ROUTER_YES */
	LSA_UINT32		device_gateway_ip;				/* default router if CM_CL_DEVICE_MODE_USE_DEF_ROUTER_YES */
	CM_LIST_TYPED_ENTRY_TYPE(cm_cl_nare_alias_tag)
					device_alias_list;				/* list of CM_CL_NARE_ALIAS_TYPE) */
} CM_CL_NARE_TYPE;

typedef CM_CL_NARE_TYPE * CM_CL_NARE_PTR_TYPE;


typedef struct cm_client_device_tag
{
	CM_CHANNEL_PTR_TYPE	channel;			/* backlink */

		/* from add */
	LSA_UINT16			device_nr;			/* key: local identification of this IO-Device; 1..CM_CL_MAX_DEVICE_NR */
	LSA_UINT32			device_mode;		/* see CM_CL_DEVICE_MODE_XXX */
	LSA_UINT32			device_properties;  /* see CM_CL_DEVICE_PROPERTIES_SUPPORT_XXX, see engineering tool "DeviceProperties"  */
	LSA_UINT32			run_mode;			/* see CM_CL_RMODE_XXX */
	LSA_UINT16			cmi_timeout100ms;	/* CM Initiator Activity Timeout */

	CM_CL_NARE_TYPE		dev_addr;			/* address info of this device */
	CM_ARCB_RPC_TYPE	dev_rpc;			/* various RPC infos */
	CM_AR_GRAPH_PTR_TYPE argr;				/* ptr to AR */

		/* internally used */
	LSA_UINT8			state;				/* noi, passive, active, removing, see CM_CL_DEV_ST_xxx, see DeviceControl */
	LSA_UINT8			dev_type;			/* see CM_CL_DEV_TYPE_xxx */

	LSA_UINT32			wf;					/* wait_for; see CM_CL_DEV__WF_xxx and CM_CL_DEV__DO_xxx  */

	CM_LIST_ENTRY_TYPE	event_q;			/* pool of notification resources */
	CM_LIST_ENTRY_TYPE	alarm_q;			/* pool of alarm resources, low and high */

	CM_UPPER_RQB_PTR_TYPE
						upper_add_rm;		/* current upper rqb for CM_OPC_CL_DEVICE_( ADD | REMOVE ) */
	CM_UPPER_RQB_PTR_TYPE
						upper_ctrl;			/* current upper rqb for CM_OPC_CL_DEVICE_CONTROL::ACTIVE/PASSIVE */
											/* must not merged with upper_add_rm, because DeviceDemove must always possible */
	CM_UPPER_RQB_PTR_TYPE
						upper_apdu;			/* current upper rqb for CM_OPC_CL_DEVICE_CONTROL::APDU_CONTROL */

/*	LSA_UINT16			apdu_status;		// current apdu set to device */

	LSA_VOID_PTR_TYPE	rpc_client_id;		/* clrpc binding from client to server */

	struct {
		LSA_UINT8					state;              /* state of bindsm; see CM_CL_BINDSM_XXX */
		LSA_UINT16					wait_for[1];        /* see CM_CL_BINDSM_DO_XXX*/
		LSA_UINT8					nare_mode;          /* modes to use for nare_set, see NARE_MODE_CONFIG_XXX*/
		LSA_UINT16					response;           /* stored response code for returning read codes */
		LSA_BOOL					force_nsu;          /* late error indicated multiple NoS/IP on last startup */
		LSA_UINT8					schedule_count;     /* count of schedule attempts for the first queued read */
		CM_RPC_LOWER_RQB_PTR_TYPE	rpc;                /* rpc rqb, allocated on bind */
	} bindsm;

} CM_CL_DEVICE_TYPE;

typedef CM_CL_DEVICE_TYPE * CM_CL_DEVICE_PTR_TYPE;

typedef CM_CL_DEVICE_TYPE const * CM_CL_DEVICE_CONST_PTR_TYPE;


/*----------------------------------------------------------------------------*/

	/* cl-upper-wait_for */
#define CM_CUWF_reserved_01				0x0001
#define CM_CUWF_reserved_02				0x0002
#define CM_CUWF_UNREG_IF_REQ			0x0004
#define CM_CUWF_UNREG_IF_CNF			0x0008

#define CM_CUWF_EPM_STOP_REQ			0x0010
#define CM_CUWF_EPM_STOP_CNF			0x0020
#define CM_CUWF_PDEV_SET_DOWN_CNF		0x0100	/* PDev wants to ResetToFactory, Set-NoS or Set-IPv4Suite */

	/* fixed config */
#define CM_CFG_MAX_CL_IMPLICIT_READ_IND		1	/* count of concurrent IOC.ReadImplicit.ind */


typedef struct cm_cl_channel_tag
{
	CM_UPPER_RQB_PTR_TYPE	upper;				/* current upper create / delete rqb */
	CM_UPPER_RQB_PTR_TYPE	upper_apdu;			/* current clear / operate rqb */
	LSA_UINT16				upper_wf;			/* see CM_CUWF_xxx */

		/* keep from create */
	LSA_UINT16				vendor_id;			/* req: for object uuid of controller */
	LSA_UINT16				device_id;			/* req: for object uuid of controller */
	LSA_UINT16				instance;			/* req: for object uuid of controller */
	LSA_UINT32				alarm_reject;		/* see cm_cl_alarm_type_is_supported() */
	LSA_BOOL				IOXS_Required;		/* for configuration of DiscardIOXS (TIA 1218229) */

	LSA_UINT16				clrpc_server_id;	/* id of registered clrpc-server */

		/* anchor of devices */
	LSA_UINT16				dev_cnt;			/* size of dev[], see cm_cl_create() */
/*	CM_CL_DEVICE_TYPE		dev[CM_CL_MAX_DEVICE_NR]; now alloc within cm_cl_create() */
	CM_CL_DEVICE_PTR_TYPE	dev;				/* use as dev[0..dev_cnt-1] */

	LSA_UINT16				nos_assignment;		/* CM_CL_CONTROL_NOS_ASSIGNMENT_(ON|OFF) */

		/* schedule */
	struct cm_cl_channel_schedule_tag {
		CM_ONESHOT_TYPE		oneshot_timer;
		CM_LIST_ENTRY_TYPE	ready2run_list;
		LSA_UINT16			running;
		LSA_UINT16			max_running;
		LSA_UINT16			timeout_msec;
		LSA_UINT16			wait_reconnect_ticks;
	}						schedule;

	struct cm_cl_channel_fsu_tag {
		LSA_INT16			dev_count;			/* count of fsu-devices */
	}						fsu;


		/* needed by the IOCReadReq.Read Implicit */
	CM_RPC_LOWER_RQB_PTR_TYPE
							ipnio_controller__read_implicit[CM_CFG_MAX_CL_IMPLICIT_READ_IND];

} CM_CL_CHANNEL_TYPE;


/*----------------------------------------------------------------------------*/

#define CM_CL_PDU_COC_HEADER	(2+2+2 + 1+1 + 4 + 2 + 2 + 1+1 + 2)

/*--- lsa opcode handler -----------------------------------------------------*/

LSA_VOID
cm_cl_channel_init (
	CM_CHANNEL_PTR_TYPE  channel
);

LSA_UINT16
cm_cl_channel_undo_init (
	CM_CHANNEL_PTR_TYPE  channel
);

LSA_VOID
cm_cl_request (
	CM_CHANNEL_PTR_TYPE channel,
	CM_UPPER_RQB_PTR_TYPE  rb
);

LSA_VOID
cm_cl_opc_create (
	CM_CHANNEL_PTR_TYPE  channel,
	CM_UPPER_RQB_PTR_TYPE  rb
);

LSA_VOID
cm_cl_rpc_register_if_obj_done (
	CM_CHANNEL_PTR_TYPE channel,
	CM_RPC_LOWER_RQB_PTR_TYPE rpc
);

LSA_VOID
cm_cl_opc_delete (
	CM_CHANNEL_PTR_TYPE  channel,
	CM_UPPER_RQB_PTR_TYPE  rb
);

LSA_VOID
cm_cl_delete_done (
	CM_CHANNEL_PTR_TYPE  channel,
	LSA_UINT16  event
);

LSA_VOID
cm_cl_rpc_unregister_done (
	CM_CHANNEL_PTR_TYPE channel,
	CM_RPC_LOWER_RQB_PTR_TYPE  rpc
);

LSA_VOID
cm_cl_opc_control (
	CM_CHANNEL_PTR_TYPE  channel,
	CM_UPPER_RQB_PTR_TYPE  rb
);

LSA_VOID
cm_cl_control_set_nos_assignment (
	CM_CHANNEL_PTR_TYPE  channel
);

LSA_VOID
cm_cl_opc_device_add (
	CM_CHANNEL_PTR_TYPE  channel,
	CM_UPPER_RQB_PTR_TYPE  rb
);

LSA_VOID
cm_cl_device_add_rm_done (
	CM_CHANNEL_PTR_TYPE  channel,
	CM_CL_DEVICE_PTR_TYPE  dev,
	LSA_UINT32  done,
	LSA_RESPONSE_TYPE  cm_rsp
);

LSA_VOID
cm_cl_opc_device_remove (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	CM_UPPER_RQB_PTR_TYPE  rb
);

LSA_VOID
cm_cl_opc_device_provide_resource (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	CM_UPPER_RQB_PTR_TYPE  rb
);

LSA_VOID
cm_cl_opc_device_control (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	CM_UPPER_RQB_PTR_TYPE  rb
);

LSA_VOID
cm_cl_opc_prm_update (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	CM_UPPER_RQB_PTR_TYPE  rb
);

LSA_VOID
cm_cl_prm_update_record_list_free (
	CM_AR_GRAPH_CONST_PTR_TYPE  argr,
	CM_UPPER_CL_PRM_UPDATE_CONST_PTR_TYPE  prm_update
);

	LSA_VOID
cm_cl_prm_update_record_list_activate (
	CM_AR_GRAPH_CONST_PTR_TYPE  argr,
	CM_UPPER_CL_PRM_UPDATE_CONST_PTR_TYPE  prm_update
);

LSA_VOID
cm_cl_opc_ar_prm_begin_rsp (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	CM_UPPER_RQB_PTR_TYPE  rb
);

LSA_VOID
cm_cl_opc_ar_prm_indata_rsp (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	CM_UPPER_RQB_PTR_TYPE  rb
);

LSA_VOID
cm_cl_opc_alarm_rsp (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	CM_UPPER_RQB_PTR_TYPE  rb
);

LSA_VOID
cm_cl_opc_alarm_send (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	CM_UPPER_RQB_PTR_TYPE  rb
);

LSA_VOID
cm_cl_opc_ar_abort_rsp (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	CM_UPPER_RQB_PTR_TYPE  rb
);

LSA_VOID
cm_cl_opc_ar_pre_data_rsp (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	CM_UPPER_RQB_PTR_TYPE  rb
);

LSA_VOID
cm_cl_opc_ar_pre_offline_rsp (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	CM_UPPER_RQB_PTR_TYPE  rb
);

LSA_VOID
cm_cl_opc_ar_in_data_rsp (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	CM_UPPER_RQB_PTR_TYPE  rb
);

LSA_VOID
cm_cl_opc_ar_offline_rsp (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	CM_UPPER_RQB_PTR_TYPE  rb
);

LSA_VOID
cm_cl_opc_ar_data_status_rsp (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	CM_UPPER_RQB_PTR_TYPE  rb
);

LSA_VOID
cm_cl_opc_logbook_read (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	CM_UPPER_RQB_PTR_TYPE  rb
);

LSA_UINT16
cm_cl_opc_record_read (
	CM_CHANNEL_PTR_TYPE  channel,
	CM_UPPER_RQB_PTR_TYPE  rb
);

LSA_VOID
cm_cl_rpc_read_done (
	CM_AR_GRAPH_PTR_TYPE  argr,
	CM_RPC_LOWER_RQB_CONST_PTR_TYPE  rpc
);

LSA_UINT16
cm_cl_opc_record_write (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	CM_UPPER_RQB_PTR_TYPE  rb
);

LSA_VOID
cm_cl_rpc_write_done (
	CM_CHANNEL_PTR_TYPE  channel,
	CM_AR_GRAPH_PTR_TYPE  argr,
	CM_RPC_LOWER_RQB_PTR_TYPE  rpc
);

LSA_VOID
cm_cl_rpc_prm_begin (
	CM_AR_GRAPH_PTR_TYPE  argr
);

LSA_VOID
cm_cl_rpc_prm_end (
	CM_AR_GRAPH_PTR_TYPE  argr
);


/*--- schedule interface -----------------------------------------------------*/

LSA_VOID
cm_cl_schedule_init (
	CM_CHANNEL_PTR_TYPE  channel,
	LSA_UINT16  max_running,
	LSA_UINT16  wait_reconnect_sec,
	LSA_UINT16  schedule_timeout_msec
);

LSA_VOID
cm_cl_schedule_add (
	CM_AR_GRAPH_PTR_TYPE  argr
);

LSA_VOID
cm_cl_schedule_done (
	CM_AR_GRAPH_PTR_TYPE  argr
);

LSA_VOID
cm_cl_schedule_fsu (
	CM_AR_GRAPH_PTR_TYPE  argr
);


/*--- channel interface ------------------------------------------------------*/

CM_CHANNEL_PTR_TYPE
cm_cl_channel_get (
	CM_CHANNEL_PTR_TYPE  channel
);

LSA_UINT16
cm_cl_client_free (
	CM_CHANNEL_PTR_TYPE  channel
);

LSA_VOID
cm_cl_rpc_register_if_obj (
	CM_CHANNEL_PTR_TYPE channel
);

LSA_UINT16
cm_cl_fsu_count (
	CM_CHANNEL_PTR_TYPE  channel,
	LSA_INT16  inc_dec_or_zero
);

/*--- device interface -------------------------------------------------------*/

LSA_VOID
cm_cl_dev_init (
	CM_CL_DEVICE_PTR_TYPE  dev
	);

CM_CL_DEVICE_PTR_TYPE
cm_cl_dev_from_device_nr (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	LSA_UINT16  device_nr
	);

CM_CL_DEVICE_PTR_TYPE
cm_cl_dev_first (
	CM_CHANNEL_CONST_PTR_TYPE  channel
	);

CM_CL_DEVICE_PTR_TYPE
cm_cl_dev_next (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	CM_CL_DEVICE_CONST_PTR_TYPE  dev
	);

LSA_VOID
cm_cl_dev_free (
	CM_CHANNEL_PTR_TYPE  channel,
	CM_CL_DEVICE_PTR_TYPE  dev
	);

LSA_VOID
cm_cl_dev_calc_resource_count (
	CM_CL_DEVICE_CONST_PTR_TYPE  dev,
	LSA_UINT16 *nr_of_events,
	LSA_UINT16 *nr_of_alarms
	);

LSA_VOID
cm_cl_dev_event_provide (
	CM_CL_DEVICE_PTR_TYPE  dev,
	CM_UPPER_RQB_PTR_TYPE  rb
	);
CM_UPPER_RQB_PTR_TYPE
cm_cl_dev_event_get (
	CM_CL_DEVICE_CONST_PTR_TYPE  dev
	);

LSA_VOID
cm_cl_dev_alarm_provide (
	CM_CL_DEVICE_PTR_TYPE  dev,
	CM_UPPER_RQB_PTR_TYPE  rb
	);
CM_UPPER_RQB_PTR_TYPE
cm_cl_dev_alarm_get (
	CM_CL_DEVICE_CONST_PTR_TYPE  dev,
	LSA_UINT16  prio
	);


LSA_UINT16		/* ret: count of argr */
cm_cl_dev_ar_count (
	CM_CL_DEVICE_CONST_PTR_TYPE  dev,
	LSA_UINT16 * cnt_passive,
	LSA_UINT16 * cnt_wf_data,
	LSA_UINT16 * cnt_indata,
	LSA_UINT16 * cnt_down
	);

LSA_UINT8
cm_cl_dev_ar_state ( /* ret: CM_CL_AR_xxx */
	CM_CL_DEVICE_CONST_PTR_TYPE  dev
	);

LSA_VOID
cm_cl_dev_activate (
	CM_CL_DEVICE_PTR_TYPE  dev
	);

LSA_VOID
cm_cl_dev_abort (
	CM_CL_DEVICE_CONST_PTR_TYPE  dev,
	LSA_UINT8  reason_code
	);

LSA_VOID
cm_cl_ar_com_apdu_status_control (
	CM_AR_GRAPH_PTR_TYPE  argr,
	LSA_UINT16  acp_mode
	);

LSA_VOID
cm_cl_devsm (
	CM_CL_DEVICE_PTR_TYPE  dev,
	LSA_UINT16  action,
	CM_AR_GRAPH_PTR_TYPE  action_argr
	);


/*--- ar-graph interface -----------------------------------------------------*/

LSA_VOID
cm_cl_ar_callback (
	CM_AR_GRAPH_PTR_TYPE  argr,
	LSA_UINT16  response,
	CM_UPPER_RQB_PTR_TYPE  upper
	);

/* callback */
LSA_BOOL
cm_cl_ar_init (
	CM_AR_GRAPH_PTR_TYPE  argr
	);

/* callback */
LSA_VOID
cm_cl_arcq_request_done (
	CM_AR_GRAPH_PTR_TYPE  argr,
	LSA_UINT8  arcq_opcode,
	LSA_UINT16  arcq_para1
	);

LSA_VOID
cm_cl_ar_undo_init (
	CM_AR_GRAPH_PTR_TYPE  argr
	);

LSA_VOID
cm_cl_ar_reinit (
	CM_AR_GRAPH_PTR_TYPE  argr
	);

LSA_VOID
cm_cl_ar_sustat_init (
	CM_AR_GRAPH_CONST_PTR_TYPE  argr
	);

LSA_VOID
cm_cl_ar_sustat_sm_reset (
	CM_AR_GRAPH_CONST_PTR_TYPE argr
	);

LSA_VOID
cm_cl_ar_sustat_sm_trigger (
	CM_AR_GRAPH_CONST_PTR_TYPE argr,
	CM_CL_SUSTAT_STATE_TYPE trigger
	);

LSA_VOID
cm_cl_ar_sustat_free (
		CM_CL_SUSTAT_DATA_PTR_TYPE * sustat
	);

CM_AR_GRAPH_PTR_TYPE
cm_cl_ar_from_device (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	LSA_UINT16  device_nr
	);

CM_AR_GRAPH_PTR_TYPE
cm_cl_ar_from_device_no_elog (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	LSA_UINT16  device_nr
	);

CM_AR_GRAPH_PTR_TYPE
cm_cl_ar_from_ar_uuid (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	CLRPC_UUID_CONST_PTR_TYPE  ar_uuid,
	LSA_BOOL  elog_if_not_found
	);

LSA_VOID
cm_cl_arsm (
	CM_AR_GRAPH_PTR_TYPE  argr
	);

LSA_VOID
cm_cl_arsm_down (
	CM_AR_GRAPH_PTR_TYPE  argr,
	CM_CL_DOWN_INFO_CONST_PTR_TYPE  down_info
	);

LSA_BOOL  /* return: true if ar is_active (INDATA); no callback to cm_cl_dev_sm() */
cm_cl_ar_activate (
	CM_AR_GRAPH_PTR_TYPE  argr
	);

LSA_BOOL  /* return: true if ar is_passive, no callback to cm_cl_dev_sm() */
cm_cl_ar_passivate (
	CM_AR_GRAPH_PTR_TYPE  argr,
	LSA_UINT8  reason
	);


/*--- ar-graph helper --------------------------------------------------------*/

LSA_UINT16
cm_cl_list_record_append (
	CM_AR_GRAPH_PTR_TYPE		argr,
	CM_LIST_ENTRY_PTR_TYPE		add_to_this_list,

	LSA_UINT32					api,
	LSA_UINT16					slot_nr,
	LSA_UINT16					subslot_nr,
	LSA_UINT32					record_index,
	LSA_UINT16					transfer_sequence,
	LSA_BOOL					mk_remanent,
	LSA_BOOL					is_local,
	LSA_UINT32					record_data_len,
	CM_COMMON_MEM_U8_PTR_TYPE	record_data,

	CM_PD_RECORD_PTR_TYPE     * rec_ptr_ptr /* out */
	);

CM_AR_GRAPH_SUBMODULE_PTR_TYPE
cm_cl_ar_graph_record_find_sub_coc_ap (
	CM_AR_GRAPH_CONST_PTR_TYPE  argr,
	LSA_UINT32  api,
	LSA_UINT16  slot_nr,
	LSA_UINT16  subslot_nr
	);

CM_AR_GRAPH_SUBMODULE_PTR_TYPE
cm_cl_ar_graph_record_find_coc_access_point (
	CM_PD_RECORD_CONST_PTR_TYPE  record
	);

/*--- event interface --------------------------------------------------------*/

LSA_VOID
cm_cl_ar_event_init (
	CM_AR_GRAPH_PTR_TYPE  argr
	);

LSA_VOID
cm_cl_ar_event_set_ip (
	CM_AR_GRAPH_PTR_TYPE  argr,
	CM_MAC_CONST_PTR_TYPE  device_mac_addr,
	LSA_UINT32   device_ip_addr,
	LSA_UINT32   device_subnet_mask,
	LSA_UINT32   device_gateway_ip
	);

LSA_BOOL
cm_cl_ar_event_bad_is_different (
	CM_AR_GRAPH_CONST_PTR_TYPE  argr,
	CM_CL_DOWN_INFO_CONST_PTR_TYPE  down_info
	);
LSA_VOID
cm_cl_ar_event_set_bad (
	CM_AR_GRAPH_PTR_TYPE  argr,
	CM_CL_DOWN_INFO_CONST_PTR_TYPE  down_info
	);
LSA_BOOL
cm_cl_ar_event_bad_has_changed (
	CM_AR_GRAPH_CONST_PTR_TYPE  argr
	);
LSA_VOID
cm_cl_ar_event_get_abort_error_cause (
	CM_AR_GRAPH_PTR_TYPE  argr,
	CM_UPPER_CL_ERROR_CAUSE_PTR_TYPE  error_cause
	);
LSA_VOID
cm_cl_ar_event_get_abort_ip (
	CM_AR_GRAPH_CONST_PTR_TYPE  argr,
	LSA_UINT32 *device_ip
	);
LSA_VOID
cm_cl_ar_event_get_indata_ip (
	CM_AR_GRAPH_PTR_TYPE  argr,
	CM_UPPER_CL_IP_SUITE_PTR_TYPE  ip_suite
	);
LSA_VOID
cm_cl_ar_event (
	CM_AR_GRAPH_PTR_TYPE  argr,
	LSA_OPCODE_TYPE  notify
	);


/*--- alarm interface --------------------------------------------------------*/

LSA_VOID
cm_cl_alarm_set_enabled_state (
	CM_AR_GRAPH_PTR_TYPE  argr,
	LSA_UINT16  enabled_state
	);

LSA_VOID
cm_cl_alarm_send_next (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	CM_AR_GRAPH_PTR_TYPE  argr,
	LSA_UINT16  prio
	);

LSA_VOID
cm_cl_alarm_provide (
	CM_AR_GRAPH_CONST_PTR_TYPE  argr,
	CM_UPPER_RQB_PTR_TYPE  rb
	);

LSA_BOOL
cm_cl_alarm_param_ok (
	CM_AR_GRAPH_CONST_PTR_TYPE  argr,
	LSA_UINT16  alarm_type,
	LSA_UINT32  api,
	LSA_UINT16  slot_nr,
	LSA_UINT16  subslot_nr,
	CM_AR_GRAPH_SUBMODULE_PTR_TYPE  *sub
	);

LSA_BOOL
cm_cl_alarm_is_device_alarm(
	CM_AR_GRAPH_CONST_PTR_TYPE  argr,
	LSA_UINT32  api,
	LSA_UINT16  slot,
	LSA_UINT16  subslot,
	LSA_UINT32	module_ident,
	LSA_UINT32  submodule_ident,
	LSA_UINT16  alarm_type,
	LSA_BOOL  *valid
	);

LSA_VOID
cm_cl_alarm_indication (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	CM_AR_GRAPH_PTR_TYPE  argr,
	LSA_UINT16  alarm_prio
	);

LSA_VOID
cm_cl_alarm_set_wait_for (
	CM_AR_GRAPH_PTR_TYPE  argr,
	LSA_UINT16  alarm_priority
	);

LSA_VOID
cm_cl_alarm_clear_wait_for(
	CM_AR_GRAPH_PTR_TYPE  argr,
	LSA_UINT16  alarm_priority
);

//*--- rpc interface ----------------------------------------------------------*/

LSA_VOID
cm_cl_rpc_lower_done(
	CM_CHANNEL_PTR_TYPE  channel,
	CM_RPC_LOWER_RQB_PTR_TYPE  rb
);

LSA_VOID
cm_cl_rpc_to_addr_info (
	CM_RPC_LOWER_RQB_CONST_PTR_TYPE  rpc,
	LSA_UINT16  error_code,
	LSA_UINT16 *lower_rsp,
	LSA_UINT16 *error_cause,
	LSA_UINT32 *pnio_status,
	LSA_UINT32 *nca_status
);

LSA_UINT16
cm_cl_rpc_bind (
	CM_CHANNEL_PTR_TYPE  channel,
	CM_CL_DEVICE_PTR_TYPE  dev
);

LSA_VOID
cm_cl_rpc_rebind (
	CM_AR_GRAPH_CONST_PTR_TYPE  argr
);

LSA_UINT16
cm_cl_rpc_unbind (
	CM_CHANNEL_PTR_TYPE  channel,
	CM_CL_DEVICE_CONST_PTR_TYPE  dev
);

LSA_VOID
cm_cl_rpc_cancel (
	CM_AR_GRAPH_PTR_TYPE  argr
);

LSA_VOID
cm_cl_rpc_connect (
	CM_CHANNEL_PTR_TYPE  channel,
	CM_AR_GRAPH_PTR_TYPE  argr
);

LSA_VOID
cm_cl_rpc_release (
	CM_CHANNEL_PTR_TYPE  channel,
	CM_AR_GRAPH_PTR_TYPE  argr
);

LSA_VOID
cm_cl_rpc_control (
	CM_CHANNEL_PTR_TYPE  channel,
	CM_AR_GRAPH_PTR_TYPE  argr,
	LSA_BOOL  plug,
	LSA_UINT16  alarm_seq_nr
);

LSA_VOID
cm_pnio_c_control_ind (
	CM_CHANNEL_PTR_TYPE  channel,
	CM_RPC_LOWER_RQB_PTR_TYPE  rpc
);

LSA_VOID
cm_pnio_c_control_rsp (
	CM_CHANNEL_PTR_TYPE  channel,
	CM_AR_GRAPH_CONST_PTR_TYPE  argr,
	CM_RPC_LOWER_RQB_PTR_TYPE  rpc,
	LSA_UINT32  pnio_code
);

LSA_VOID
cm_pnio_c_read_implicit_ind (
	CM_CHANNEL_PTR_TYPE  channel,
	CM_RPC_LOWER_RQB_PTR_TYPE  rpc
);

LSA_VOID
cm_pnio_c_read_implicit_rsp (
	CM_CHANNEL_PTR_TYPE  channel,
	CM_UPPER_RQB_PTR_TYPE pd
);


/*--- PD related -------------------------------------------------------------*/

LSA_BOOL
cm_cl_pdev_set_down_ind (
	CM_CHANNEL_PTR_TYPE channel,
	LSA_UINT8 reason /* CM_AR_REASON_DCP_RESET or CM_AR_REASON_DCP_NAME or CM_AR_REASON_IPSUITE */
);

LSA_VOID
cm_cl_pdev_set_down_check (
	CM_CHANNEL_PTR_TYPE  channel
);

/*--- record prm statemachine ------------------------------------------------*/

LSA_VOID
cm_cl_prm_init (
	CM_AR_GRAPH_PTR_TYPE  argr
);

LSA_VOID
cm_cl_prm_request (
	CM_AR_GRAPH_PTR_TYPE  argr,
	LSA_UINT16  prm_mode,
	CM_UPPER_RQB_PTR_TYPE  rqb
);

LSA_BOOL
cm_cl_prm_request_can_queue (
	CM_AR_GRAPH_CONST_PTR_TYPE  argr,
	LSA_UINT16  prm_mode
);

LSA_UINT16
cm_cl_prm_mode (
	CM_AR_GRAPH_CONST_PTR_TYPE  argr
);

LSA_VOID
cm_cl_prm_trigger (
	CM_AR_GRAPH_PTR_TYPE  argr,
	LSA_UINT32  done
);

LSA_VOID
cm_cl_prm_abort (
	CM_AR_GRAPH_PTR_TYPE  argr
);

/* callback */
LSA_VOID
cm_cl_prm_start_cnf (
	CM_AR_GRAPH_PTR_TYPE  argr
);

/* callback */
LSA_VOID
cm_cl_prm_begin_ind (
	CM_AR_GRAPH_PTR_TYPE  argr
);

/* callback */
LSA_VOID
cm_cl_prm_end_req (
	CM_AR_GRAPH_PTR_TYPE  argr
);

/* callback */
LSA_VOID
cm_cl_prm_up_ind (
	CM_AR_GRAPH_PTR_TYPE  argr
);

/* callback */
LSA_VOID
cm_cl_prm_idle_ind (
	CM_AR_GRAPH_PTR_TYPE  argr
);

/* callback */
LSA_VOID
cm_cl_prm_down_ind (
	CM_AR_GRAPH_PTR_TYPE  argr
);

/* helper */

LSA_VOID
cm_cl_prm_record_list_build (
	CM_AR_GRAPH_PTR_TYPE  argr,
	LSA_UINT8  prm_seq
);

LSA_VOID
cm_cl_prm_record_list_update (
	CM_AR_GRAPH_PTR_TYPE  argr
);

LSA_BOOL
cm_cl_sub_state_must_prmed (
	LSA_UINT16  sub_state
);
LSA_BOOL
cm_cl_sub_state_prmed_effect (
	LSA_UINT16  sub_state
);


/*--- prm-read statemachine for rpc-trigger ----------------------------------*/

LSA_VOID
cm_cl_prm_read_start (
	CM_AR_GRAPH_PTR_TYPE  argr
);

LSA_VOID
cm_cl_prm_read_retrigger (
	CM_AR_GRAPH_PTR_TYPE  argr
);

/*callback */
LSA_VOID
cm_cl_prm_read_timeout (
	CM_ONESHOT_PTR_TYPE  timeout,
	LSA_VOID_PTR_TYPE  void_argr
);

LSA_VOID
cm_cl_prm_read_rpc_trigger_req (
	CM_AR_GRAPH_PTR_TYPE  argr
);

LSA_VOID
cm_cl_prm_read_rpc_trigger_cnf (
	CM_AR_GRAPH_PTR_TYPE  argr,
	CM_UPPER_RQB_PTR_TYPE  upper
);

LSA_VOID
cm_cl_prm_read_stop (
	CM_AR_GRAPH_PTR_TYPE  argr
);


/*----------------------------------------------------------------------------*/

#define CM_CL_ARDY_TIMEOUT__RemoteApplReady		1
#define CM_CL_ARDY_TIMEOUT__PreDataInd			2

LSA_VOID
cm_cl_prm_ardy_timeout_start (
	CM_AR_GRAPH_PTR_TYPE  argr,
	LSA_UINT16  cm_cl_ardy_timeout
);

/*callback */
LSA_VOID
cm_cl_prm_ardy_timeout (
	CM_ONESHOT_PTR_TYPE  timeout,
	LSA_VOID_PTR_TYPE  void_argr
);

LSA_VOID
cm_cl_prm_ardy_timeout_stop (
	CM_AR_GRAPH_PTR_TYPE  argr
);


/*----------------------------------------------------------------------------*/

LSA_UINT16
cm_cl_acp_global_provider_control (
	CM_CHANNEL_PTR_TYPE channel,
	LSA_UINT16  apdu_tag
);

LSA_VOID
cm_cl_acp_global_provider_control_done(
	CM_CHANNEL_PTR_TYPE channel,
	CM_EDD_LOWER_RQB_PTR_TYPE  edd
);

/*---------------------------------------------------------------------------*/

CM_ACP_LOWER_RQB_PTR_TYPE
cm_cl_acp_alarm_indication (
	CM_AR_GRAPH_PTR_TYPE  argr,
	LSA_UINT16  prio,
	CM_ACP_LOWER_RQB_PTR_TYPE  acp
);

LSA_VOID
cm_cl_alarm_ack_send_done (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	CM_AR_GRAPH_PTR_TYPE  argr,
	CM_ACP_LOWER_RQB_CONST_PTR_TYPE  acp
);

LSA_VOID
cm_cl_alarm_send_done (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	CM_AR_GRAPH_PTR_TYPE  argr,
	CM_ACP_LOWER_RQB_PTR_TYPE  acp
);

LSA_VOID
cm_cl_alarm_ack_indication (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	CM_AR_GRAPH_PTR_TYPE  argr,
	LSA_UINT16  prio,
	CM_ACP_LOWER_RQB_PTR_TYPE  acp
);

/*--- ar_com statemachine ----------------------------------------------------*/

/* callback */
LSA_VOID
cm_cl_ar_com_start_done (
	CM_AR_GRAPH_PTR_TYPE  argr
);

/* callback */
LSA_VOID
cm_cl_ar_com_stop_done (
	CM_CHANNEL_PTR_TYPE  channel,
	CM_AR_GRAPH_PTR_TYPE  argr
);

/* callback */
LSA_VOID
cm_cl_ar_com_up (
	CM_AR_GRAPH_PTR_TYPE  argr
);

/* callback */
LSA_VOID
cm_cl_ar_com_rtc3_is_running (
	CM_AR_GRAPH_PTR_TYPE  argr
);

/* callback */
LSA_VOID
cm_cl_ar_com_down (
	CM_AR_GRAPH_PTR_TYPE  argr
);

LSA_VOID
cm_cl_ar_com_reset_done(
	CM_AR_GRAPH_PTR_TYPE  argr
);

LSA_VOID
cm_cl_ar_com_frame_passivate_aux_done (
	CM_AR_GRAPH_PTR_TYPE  argr
);

LSA_BOOL
cm_cl_ar_com_apdu_status_ind (
	CM_AR_GRAPH_PTR_TYPE  argr,
	LSA_UINT16  apdu_delta
);


/*--- nare interface ---------------------------------------------------------*/
CM_CL_NARE_RSP_TYPE
cm_cl_check_ip_suite (
	CM_AR_GRAPH_CONST_PTR_TYPE argr,
	LSA_UINT32 ip_addr,
	LSA_UINT32 subnet_mask,
	LSA_UINT32 gateway_ip
);

LSA_VOID
cm_cl_nare_ident (
	CM_AR_GRAPH_PTR_TYPE argr
);

/* callback */
LSA_VOID
cm_cl_nare_ident_done(
	CM_CHANNEL_PTR_TYPE channel,
	CM_NARE_LOWER_RQB_PTR_TYPE  nare
);

LSA_VOID
cm_cl_nare_set (
	CM_AR_GRAPH_PTR_TYPE argr
);

/* callback */
LSA_VOID
cm_cl_nare_set_done(
	CM_CHANNEL_PTR_TYPE channel,
	CM_NARE_LOWER_RQB_PTR_TYPE  nare
);

/* callback */
LSA_VOID
cm_cl_nare_cancel_done(
	CM_CHANNEL_PTR_TYPE channel,
	CM_NARE_LOWER_RQB_PTR_TYPE  nare
);

LSA_VOID
cm_cl_nare_test_ip (
	CM_AR_GRAPH_PTR_TYPE argr
);

/* callback */
LSA_VOID
cm_cl_nare_test_ip_done (
	CM_CHANNEL_PTR_TYPE channel,
	CM_NARE_LOWER_RQB_PTR_TYPE nare
);

/* callback */
LSA_VOID
cm_cl_nare_notify_ind (
	CM_CHANNEL_CONST_PTR_TYPE  channel,
	NARE_UPPER_IDENT_NOTIFY_PTR_TYPE  notify
);


/*--- oha interface ----------------------------------------------------------*/

#define CM_CL_U32_SET_DEV_AR(dev_ar, ar_nr)	((LSA_UINT32)(((ar_nr & 0x0FF) << 16) | (dev_ar & 0x0FFFF)))

#define CM_CL_U32_GET_DEVICE_NR(u32_)		((LSA_UINT16)( (u32_) & 0x0000FFFF))
#define CM_CL_U32_GET_AR_NR(u32_)			((LSA_UINT16)(((u32_) & 0x00FF0000) >> 16))

/* callback */
LSA_VOID
cm_cl_oha_hello_ind (
	CM_CHANNEL_CONST_PTR_TYPE channel,
	OHA_UPPER_HELLO_PTR_TYPE  hello
);


/*--- edd interface ----------------------------------------------------------*/

LSA_VOID
cm_cl_edd_set_dcp_hello_filter_done (
	CM_CHANNEL_PTR_TYPE  channel,
	LSA_UINT16  edd_rsp,
	EDD_UPPER_NRT_SET_DCP_HELLO_FILTER_PTR_TYPE  sf
);


/*--- bindsm -----------------------------------------*/
LSA_VOID
cm_cl_bindsm_startup (
	CM_AR_GRAPH_PTR_TYPE  argr
	);

LSA_VOID
cm_cl_bindsm_startup_optimized (
	CM_AR_GRAPH_PTR_TYPE  argr,
	LSA_BOOL  is_fsu
	);

LSA_VOID
cm_cl_bindsm_read (
	CM_AR_GRAPH_PTR_TYPE argr
	);

LSA_VOID
cm_cl_bindsm_set_unbound (
	CM_AR_GRAPH_CONST_PTR_TYPE argr,
	LSA_UINT8 reason_code,
	LSA_UINT16 error_cause
	);

LSA_BOOL
cm_cl_bindsm_is_identified (
	CM_AR_GRAPH_CONST_PTR_TYPE  argr
	);

LSA_VOID
cm_cl_bindsm_nare_set (
	CM_AR_GRAPH_PTR_TYPE  argr,
	LSA_UINT8  mode
	);

LSA_VOID
cm_cl_bindsm_nare_error (
	CM_AR_GRAPH_PTR_TYPE  argr,
	CM_CL_DOWN_INFO_CONST_PTR_TYPE  dinfo
	);

LSA_VOID
cm_cl_bindsm_trigger (
	CM_AR_GRAPH_PTR_TYPE  argr,
	LSA_UINT32  done
	);


/*--- MasterProject ----------------------------------------------------------*/

LSA_BOOL
cm_cl_mp_is_valid_alias_name (
	CM_UPPER_ARCB_NARE_ALIAS_CONST_PTR_TYPE  alias,
	CM_PD_MASTER_TAILOR_DATA_CONST_PTR_TYPE  mtd
	);

LSA_UINT32
cm_cl_mp_ar_graph_check_consistency (
	CM_CL_DEVICE_CONST_PTR_TYPE  dev,
	CM_AR_GRAPH_CONST_PTR_TYPE  argr,
	CM_PD_MASTER_TAILOR_DATA_PTR_TYPE  mtd
	);

LSA_UINT16
cm_cl_mp_tailor_name (
	CM_COMMON_MEM_U8_PTR_TYPE	dst,
	CM_COMMON_MEM_U8_PTR_TYPE	name,
	LSA_UINT16					namelen,
	CM_COMMON_MEM_U8_PTR_TYPE	ioc_nos,
	LSA_UINT16					ioc_nos_len
	);

LSA_BOOL
cm_cl_mp_tailor_ipsuite (
	CM_CL_DEVICE_PTR_TYPE	dev,
	LSA_UINT32				ioc_ipaddr,
	LSA_UINT32				ioc_netmask
	);

LSA_BOOL
cm_cl_mp_tailor (
	CM_CHANNEL_PTR_TYPE  channel,
	CM_AR_GRAPH_CONST_PTR_TYPE  argr,
	CM_PD_MASTER_TAILOR_DATA_CONST_PTR_TYPE  mtd,
	CM_CL_DOWN_INFO_PTR_TYPE  dinfo
	);

/*----------------------------------------------------------------------------*/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
#endif  /* of CM_ICL_H */
