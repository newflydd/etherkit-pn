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
/*  F i l e               &F: cm_cl10.c                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  opcodes for client                                                       */
/*                                                                           */
/*  CM_OPC_CL_CREATE                                                         */
/*  CM_OPC_CL_DELETE                                                         */
/*  CM_OPC_CL_CONTROL                                                        */
/*                                                                           */
/*****************************************************************************/
#define LTRC_ACT_MODUL_ID	610
#define CM_MODULE_ID		610

#include "cm_int.h"

CM_FILE_SYSTEM_EXTENSION(CM_MODULE_ID) /* no semicolon */


/*----------------------------------------------------------------------------*/
#if CM_CFG_MAX_CLIENTS

#ifdef CM_MESSAGE
# pragma CM_MESSAGE("CM_CFG_MAX_CLIENTS          = " CM_STRINGIFY(CM_CFG_MAX_CLIENTS))
# pragma CM_MESSAGE("CM_CFG_MAX_CL_LOG_ENTRIES   = " CM_STRINGIFY(CM_CFG_MAX_CL_LOG_ENTRIES))
#endif /* CM_MESSAGE */


/*------------------------------------------------------------------------------
// local definitions
//----------------------------------------------------------------------------*/

static LSA_VOID
cm_cl_rpc_unregister_if_obj (
	CM_CHANNEL_PTR_TYPE channel
	);


/*------------------------------------------------------------------------------
// init channel, client part
//----------------------------------------------------------------------------*/
LSA_VOID
cm_cl_channel_init (
	CM_CHANNEL_PTR_TYPE  channel
	)
{
	channel->usr.cl.upper = LSA_NULL;
	channel->usr.cl.upper_apdu = LSA_NULL;
	channel->usr.cl.upper_wf = 0;

	channel->usr.cl.dev_cnt = 0;
/*	channel->usr.cl.dev = LSA_NULL; now an array */

	CM_CL_TRACE_01(channel->trace_idx, LSA_TRACE_LEVEL_NOTE_LOW
		, "connected to pd(%u)"
		, cm_peer_channel_get(channel, CM_PATH_TYPE_PD)->my_handle
		);
	CM_CL_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_CHAT
		, "sizeof(CM_CL_DEVICE_TYPE)=(%u)", sizeof(CM_CL_DEVICE_TYPE)
		);
	CM_CL_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_CHAT
		, "sizeof(CM_CL_CHANNEL_TYPE)=(%u)", sizeof(CM_CL_CHANNEL_TYPE)
		);
}


/*------------------------------------------------------------------------------
// undo_init channel, client part
//----------------------------------------------------------------------------*/
LSA_UINT16
cm_cl_channel_undo_init (
	CM_CHANNEL_PTR_TYPE  channel
	)
{
	/* NOTE: caller wipes out channel->usr.xxx when returning CM_OK */

	LSA_UNUSED_ARG (channel);
	/* all done in cm_close_channel and cl_delete */
	return CM_OK;
}


/*----------------------------------------------------------------------------*/
CM_CHANNEL_PTR_TYPE
cm_cl_channel_get (
	CM_CHANNEL_PTR_TYPE  channel
)
{
	CM_CHANNEL_PTR_TYPE  cl_channel = cm_peer_channel_get (channel, CM_PATH_TYPE_CLIENT);

	if (cm_is_not_null (cl_channel) && cl_channel->state == CM_CL_READY)
	{
		return cl_channel;
	}

	return LSA_NULL;
}


/*----------------------------------------------------------------------------*/
static LSA_UINT16
cm_cl_client_alloc (
	CM_CHANNEL_PTR_TYPE  channel,
	LSA_UINT16  max_devices
	)
{
	LSA_UINT32  alloc_size;

	CM_ASSERT (max_devices <= CM_CL_MAX_DEVICE_NR);

	channel->usr.cl.dev = LSA_NULL;

	alloc_size = sizeof(CM_CL_DEVICE_TYPE) * max_devices;

	channel->usr.cl.dev_cnt = max_devices;

	CM_ALLOC_LOCAL_MEM (
			CM_CAST_LOCAL_MEM_PTR_PTR (& channel->usr.cl.dev),
			alloc_size
		);
	if (cm_is_null (channel->usr.cl.dev)) {
		CM_CL_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
			, "no mem for dev[], alloc_size(0x%x)", alloc_size
			);
		cm_cl_client_free (channel);
		return CM_ERR_ALLOC_LOCAL;
	}

	{
	LSA_UINT16  idev;
	for (idev = 0; idev < channel->usr.cl.dev_cnt; ++idev)
	{
		CM_CL_DEVICE_PTR_TYPE  dev = & channel->usr.cl.dev[idev];

		cm_cl_dev_init (dev);
	}
	}

	return CM_OK;
}


/*----------------------------------------------------------------------------*/
LSA_UINT16
cm_cl_client_free (
	CM_CHANNEL_PTR_TYPE  channel
	)
{
	LSA_UINT16  rc;

	channel->usr.cl.dev_cnt = 0;

	if (cm_is_not_null (channel->usr.cl.dev)) {
		CM_FREE_LOCAL_MEM (& rc, channel->usr.cl.dev);
		CM_ASSERT (rc == LSA_RET_OK);
		channel->usr.cl.dev = LSA_NULL;
	}

	return CM_OK;
}


/*----------------------------------------------------------------------------*/
LSA_UINT16
cm_cl_fsu_count (
	CM_CHANNEL_PTR_TYPE  channel,
	LSA_INT16  inc_dec_or_zero
	)
{
	CM_ASSERT (cm_is_not_null (channel));

	switch (inc_dec_or_zero) {
	case  0:
		break;
	case +1:
		channel->usr.cl.fsu.dev_count += 1;
		CM_ASSERT (0 <= channel->usr.cl.fsu.dev_count
			&& channel->usr.cl.fsu.dev_count <= CM_CFG_MAX_CL_FSU_DEVICES
			);
		break;
	case -1:
		channel->usr.cl.fsu.dev_count -= 1;
		CM_ASSERT (0 <= channel->usr.cl.fsu.dev_count
			&& channel->usr.cl.fsu.dev_count <= CM_CFG_MAX_CL_FSU_DEVICES
			);
		break;
	default:
		CM_FATAL1 ((LSA_UINT32)inc_dec_or_zero);
		break;
	}
	return (LSA_UINT16)channel->usr.cl.fsu.dev_count;
}


/*------------------------------------------------------------------------------
// handle opcode CM_OPC_CL_CREATE
//		switches channel->state:
//		READY_CHANNEL
//			-> acp.global_prov.reset
//			-> rpc.interface_register(client)
//			-> rpc.obj_register
//			-> READY_CLIENT
//----------------------------------------------------------------------------*/
LSA_VOID
cm_cl_opc_create (
	CM_CHANNEL_PTR_TYPE  channel,
	CM_UPPER_RQB_PTR_TYPE  rb
	)
{
	LSA_UINT16  rc;
	CM_UPPER_CL_CREATE_PTR_TYPE  create;

		/* param check */
	CM_ASSERT (cm_is_not_null (rb));
	CM_ASSERT (cm_is_not_null (rb->args.void_ptr));

	create = rb->args.cl.create;

		/* sequence check */
	if (channel->state != CM_CH_READY)
	{
		CM_CL_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR, "wrong ch.state(%d)", channel->state);
		cm_callback (channel, CM_ERR_SEQUENCE, & rb);
		return;
	}

	if (! cm_pdusr_is_ready (channel))
	{
		CM_CL_TRACE_00(channel->trace_idx, LSA_TRACE_LEVEL_ERROR, "PD not ready (PD::CREATE not run?)");
		cm_callback(channel, CM_ERR_SEQUENCE, &rb);
		return;
	}

	cm_cl_channel_init(channel);

		/* param check */
	if (! (1 <= create->max_devices && create->max_devices <= CM_CL_MAX_DEVICE_NR))
	{
		CM_CL_TRACE_02 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR, "!(1 <= max_devices(%d) <= CM_CL_MAX_DEVICE_NR(%d))", create->max_devices, CM_CL_MAX_DEVICE_NR);
		cm_callback (channel, CM_ERR_PARAM, & rb);
		return;
	}

	if (create->IOXS_Required != CM_IOXS_REQUIRED_NO && create->IOXS_Required != CM_IOXS_REQUIRED_YES)
	{
		CM_CL_TRACE_01(channel->trace_idx, LSA_TRACE_LEVEL_ERROR, "IOXS_Required(%u) out of range", create->IOXS_Required);
		cm_callback(channel, CM_ERR_PARAM, &rb);
		return;
	}


	{ /* note: must not use strlen(), may access bytes behind the buffer */
	LSA_UINT16 idx;
	for (idx = 0; idx < sizeof(create->annotation); ++idx)
	{
		if( create->annotation[idx] == 0 ) {
			break;
		}
	}
	if (idx >= sizeof(create->annotation))
	{
		CM_CL_TRACE_00 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR, "annotation not zero-terminated");
		cm_callback (channel, CM_ERR_PARAM, & rb);
		return;
	}
	}

	if ((create->alarm_reject & CM_ALARM_PLUG_GROUP) != 0)
	{
		CM_CL_TRACE_00 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR, "alarm_reject must not contain any of CM_ALARM_PLUG_GROUP");
		cm_callback (channel, CM_ERR_PARAM, & rb);
		return;
	}

	if (! (create->initial_data_status == CM_CL_CONTROL_APDU_STATUS_RUN
	||	create->initial_data_status == CM_CL_CONTROL_APDU_STATUS_STOP))
	{
		CM_CL_TRACE_00 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR, "initial_data_status must be RUN or STOP");
		cm_callback (channel, CM_ERR_PARAM, & rb);
		return;
	}

		/* init client */
	rc = cm_cl_client_alloc (channel, rb->args.cl.create->max_devices);
	if (rc != CM_OK)
	{
		cm_callback (channel, rc, & rb);
		return;
	}

		/* init and keep needed things */
	channel->usr.cl.upper = LSA_NULL;
	channel->usr.cl.upper_apdu = LSA_NULL;
	channel->usr.cl.upper_wf = 0;

	channel->usr.cl.vendor_id = create->vendor_id;
	channel->usr.cl.device_id = create->device_id;
	channel->usr.cl.instance = create->instance;
	channel->usr.cl.alarm_reject = create->alarm_reject;
	channel->usr.cl.IOXS_Required = create->IOXS_Required == CM_IOXS_REQUIRED_YES ? LSA_TRUE : LSA_FALSE;

	channel->usr.cl.clrpc_server_id = CLRPC_INVALID_SERVER_ID;

/*	channel->usr.cl.dev_cnt = see cm_cl_client_alloc() */
/*	channel->usr.cl.dev = see cm_cl_client_alloc() */

	channel->usr.cl.nos_assignment = CM_CL_CONTROL_NOS_ASSIGNMENT_ON;

	cm_cl_schedule_init (channel
		, channel->detailptr->u.cl.max_connects_per_second
		, channel->detailptr->u.cl.min_reconnect_timeout
		, channel->detailptr->u.cl.schedule_timeout
		);

/*	channel->usr.cl.pd.send_clock = 0; // see cm_cl_channel_init() */

	channel->usr.cl.fsu.dev_count = 0;

		/* init done */
	CM_CL_TRACE_03 (channel->trace_idx, LSA_TRACE_LEVEL_NOTE_HIGH
		, "ClCreate: vendor_id(0x%x) device_id(0x%x) instance(0x%x)"
		, channel->usr.cl.vendor_id, channel->usr.cl.device_id, channel->usr.cl.instance
		);

		/* get channel infos (#201512) */
	{
	CM_EDD_CHANNEL_PARAMS_PTR_TYPE edd_params = cm_edd_channel_params(channel);
	rb->args.cl.create->hardware_type = edd_params->HardwareType;
	rb->args.cl.create->interface_id  = edd_params->InterfaceID;
	}

	/*
	 *  transition CM-CL::USER_READY --> CM-CL::GLOBAL_CTRL
	 */

	channel->usr.cl.upper = rb; /* keep */

	CM_ASSERT(channel->state == CM_CH_READY);
	channel->state = CM_CL_GLOBAL_CTRL;
	{
	CM_EDD_LOWER_RQB_PTR_TYPE  edd;
	EDD_UPPER_CSRT_PROVIDER_SET_STATE_PTR_TYPE global_prov;

	edd = cm_edd_alloc_rqb (channel, EDD_SRV_SRT_PROVIDER_SET_STATE);
	if (cm_is_null (edd)) {
		CM_CL_TRACE_00 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR, "!edd-rqb");
		cm_cl_client_free (channel);
		cm_callback(channel, CM_ERR_ALLOC_LOWER, &rb);
		return;
	}

	global_prov = CM_EDD_GET_PPARAM(edd, EDD_UPPER_CSRT_PROVIDER_SET_STATE_PTR_TYPE);

	global_prov->Mask = 0;
	global_prov->Status = 0;
	global_prov->GroupID = CM_PROVIDER_GROUP_IOC;

	if ((create->initial_data_status & CM_CL_CONTROL_APDU_STATUS_STOP)) {
		global_prov->Mask |= EDD_CSRT_DSTAT_BIT_STOP_RUN;
	}
	else if ((create->initial_data_status & CM_CL_CONTROL_APDU_STATUS_RUN)) {
		global_prov->Mask	|= EDD_CSRT_DSTAT_BIT_STOP_RUN;
		global_prov->Status |= EDD_CSRT_DSTAT_BIT_STOP_RUN;
	}

	cm_edd_request_lower (channel, EDD_SRV_SRT_PROVIDER_SET_STATE, channel, edd);
	/* => cm_cl_acp_global_provider_control_done() */
	}
}


/*----------------------------------------------------------------------------*/
LSA_VOID
cm_cl_rpc_register_if_obj (
	CM_CHANNEL_PTR_TYPE  channel
	)
{
	CM_RPC_LOWER_RQB_PTR_TYPE  rpc;
	LSA_UINT16  vendor_id;
	LSA_UINT16  device_id;
	LSA_UINT16  instance;
	CM_UPPER_MEM_U8_PTR_TYPE  annotation;

	CM_ASSERT (channel->state == CM_CL_REG_IFC);
	CM_ASSERT (cm_is_not_null(channel->usr.cl.upper));

	vendor_id  = channel->usr.cl.vendor_id;
	device_id  = channel->usr.cl.device_id;
	instance   = channel->usr.cl.instance;
	annotation = channel->usr.cl.upper->args.cl.create->annotation;

	channel->usr.cl.clrpc_server_id = CLRPC_INVALID_SERVER_ID; /* sanity */

	rpc = cm_rpc_alloc_rqb (channel);
	if (cm_is_not_null (rpc))
	{
		CLRPC_UPPER_SV_REGISTER_IF_OBJ_PTR_TYPE  reg = rpc->args.sv.reg_if_obj;

		cm_rpc_get_IPNIO_Controller (& reg->if_uuid
			, & reg->if_version_major, & reg->if_version_minor
			, & reg->opnum_max, & reg->pnio_signature
			);

		reg->udp_port            = 0; /* AP00619205: not from channel-details */
		reg->alloc_len           = channel->detailptr->u.cl.rpc.alloc_len;
		reg->recv_resource_count = channel->detailptr->u.cl.rpc.recv_resource_count;
		reg->send_resource_count = channel->detailptr->u.cl.rpc.send_resource_count;

		cm_rpc_make_object_uuid (& reg->object_uuid, vendor_id, device_id, instance);
		CM_MEMCPY (reg->annotation, annotation, sizeof(reg->annotation));
		reg->annotation[sizeof(reg->annotation) - 1] = 0; /* make it zero-terminated */
		reg->usr_id = 0;

			/* init result */
		reg->server_id = CLRPC_INVALID_SERVER_ID; /* sanity, out-parameter */

		cm_rpc_request_lower (channel, rpc, CLRPC_OPC_SV_REGISTER_IF_OBJ, 0, 0);
	}
	else
	{
		/*
		 * transition CM-CL::REG_IFC --> CM-CH::READY
		 */
		channel->state = CM_CH_READY;
		cm_cl_client_free (channel);

		CM_ASSERT (CM_RQB_GET_OPCODE (channel->usr.cl.upper) == CM_OPC_CL_CREATE);
		cm_callback (channel, CM_ERR_LOWER_LAYER, & channel->usr.cl.upper);
	}
}


/*----------------------------------------------------------------------------*/
LSA_VOID
cm_cl_rpc_register_if_obj_done (
	CM_CHANNEL_PTR_TYPE channel,
	CM_RPC_LOWER_RQB_PTR_TYPE rpc
	)
{
	LSA_UINT16  rc = CM_RPC_GET_RESPONSE (rpc);

	CM_ASSERT (channel->state == CM_CL_REG_IFC);

	if (rc == CLRPC_OK)
	{
		/*
		 * transitions CM-CL::REG_IFC --> CM-CL::READY
		 */
		CM_ASSERT (cm_is_not_null (channel->usr.cl.upper));
		CM_ASSERT (CM_RQB_GET_OPCODE (channel->usr.cl.upper) == CM_OPC_CL_CREATE);

			/* keep server_id */
		channel->usr.cl.clrpc_server_id = rpc->args.sv.reg_if_obj->server_id;
		CM_ASSERT (channel->usr.cl.clrpc_server_id != CLRPC_INVALID_SERVER_ID);
		cm_rpc_free_rqb (channel, rpc);

		CM_CL_TRACE_02 (channel->trace_idx, LSA_TRACE_LEVEL_CHAT
			, "register_if_obj: clrpc_server_id(%u),  now provide %u rpc-call-ind resources"
			, channel->usr.cl.clrpc_server_id, channel->detailptr->u.cl.rpc.call_resource_count
			);

			/* provide call-ind-resources */
		{
			LSA_UINT16 i;
			for( i = 0; i < channel->detailptr->u.cl.rpc.call_resource_count; ++i )
			{
				CM_RPC_LOWER_RQB_PTR_TYPE rpc_call_ind;

				rpc_call_ind = cm_rpc_alloc_rqb (channel);
				if( cm_is_null(rpc_call_ind) ) {
					CM_FATAL(); /* FATAL@rpc-provide: can make non-fatal? */
				}

				rpc_call_ind->args.sv.call->server_id = channel->usr.cl.clrpc_server_id;
				rpc_call_ind->args.sv.call->alloc_len = 0;
				rpc_call_ind->args.sv.call->ndr_data = LSA_NULL;

				cm_rpc_request_lower (channel, rpc_call_ind, CLRPC_OPC_SV_CALL_IND_PROVIDE, 0, 0);
			}
		}

			/* register within oha-channel */
		{
		CM_CHANNEL_PTR_TYPE  oha_channel = channel->lower.channel[CM_PATH_TYPE_OHA];
		if (cm_is_null (oha_channel)) {
			CM_FATAL();
		}
		cm_oha_register_client (oha_channel, channel);
		}

			/* register within nare-channel */
		{
		CM_CHANNEL_PTR_TYPE  nare_channel = channel->lower.channel[CM_PATH_TYPE_NARE];
		if (cm_is_null (nare_channel)) {
			CM_FATAL();
		}
		cm_nare_register_client (nare_channel, channel);
		}

		channel->state = CM_CL_READY;
		cm_callback (channel, CM_OK, & channel->usr.cl.upper);
	}
	else
	{
		CM_CL_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_UNEXP
			, "register_if_obj: clrpc-rsp(%u)", rc
			);
		cm_rpc_free_rqb (channel, rpc);

		/*
		 * transition CM-CL::REG_IFC --> CM-CH::READY
		 */
		channel->state = CM_CH_READY;
		cm_cl_client_free (channel);

		CM_ASSERT (CM_RQB_GET_OPCODE (channel->usr.cl.upper) == CM_OPC_CL_CREATE);
		cm_callback (channel, CM_ERR_LOWER_LAYER, & channel->usr.cl.upper);
	}
}


/*------------------------------------------------------------------------------
// handle opcode CM_OPC_CL_DELETE
//----------------------------------------------------------------------------*/
LSA_VOID
cm_cl_opc_delete (
	CM_CHANNEL_PTR_TYPE  channel,
	CM_UPPER_RQB_PTR_TYPE  rb
	)
{
	LSA_UINT16  idev;

		/* in proper state ? */
	if (channel->state != CM_CL_READY) {
		CM_CL_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR, "wrong ch.state(%u)", channel->state);
		cm_callback (channel, CM_ERR_SEQUENCE, & rb);
		return;
	}

		/* is there one device ? */
	for (idev = 0; idev < channel->usr.cl.dev_cnt; ++idev)
	{
		CM_CL_DEVICE_PTR_TYPE  dev = & channel->usr.cl.dev[idev];

		if (dev->state != CM_CL_DEV_ST_NOI)
		{
			CM_CL_TRACE_02 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR, "dev(%u) idev(%u) in use", dev->device_nr, idev);
			cm_callback (channel, CM_ERR_SEQUENCE, & rb);
			return;
		}
	}

	channel->usr.cl.upper = rb; /* keep */

		/* unregister within oha-channel */
	{
	CM_CHANNEL_PTR_TYPE  oha_channel = cm_oha_channel_get (channel);
	cm_oha_register_client (oha_channel, LSA_NULL);
	}

		/* unregister within nare-channel */
	{
	CM_CHANNEL_PTR_TYPE  nare_channel = cm_nare_channel_get (channel);
	cm_nare_register_client (nare_channel, LSA_NULL);
	}

	CM_ASSERT (channel->usr.cl.upper_wf == 0);
	channel->usr.cl.upper_wf = CM_CUWF_UNREG_IF_REQ;

		/*
		 * transition CM-CL::READY --> CM-CL::UNREGISTER
		 */
	CM_ASSERT(channel->state == CM_CL_READY);
	channel->state = CM_CL_UNREGISTER;

	cm_cl_channel_undo_init(channel);

	cm_cl_delete_done (channel, 0/*trigger*/);
}


/*----------------------------------------------------------------------------*/
static LSA_VOID
cm_cl_rpc_unregister_if_obj (
	CM_CHANNEL_PTR_TYPE  channel
	)
{
	CM_ASSERT(channel->state == CM_CL_UNREGISTER);

	{
	CM_RPC_LOWER_RQB_PTR_TYPE rpc;
	rpc = cm_rpc_alloc_rqb (channel);

	if (cm_is_null (rpc)) {
		CM_FATAL(); /* FATAL@rpc-alloc: can make non-fatal? */
		return;
	}

	CM_ASSERT (channel->usr.cl.clrpc_server_id != CLRPC_INVALID_SERVER_ID);
	rpc->args.sv.unreg_if_obj->server_id = channel->usr.cl.clrpc_server_id;

	cm_rpc_request_lower (channel, rpc, CLRPC_OPC_SV_UNREGISTER_IF_OBJ, 0, 0);
	}
}


/*----------------------------------------------------------------------------*/
LSA_VOID
cm_cl_rpc_unregister_done (
	CM_CHANNEL_PTR_TYPE  channel,
	CM_RPC_LOWER_RQB_PTR_TYPE  rpc
	)
{
	LSA_UINT16  rc;

	rc = CM_RPC_GET_RESPONSE (rpc);

	CM_CL_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_CHAT
		, "rc(%u)", rc
		);
	if (rc != CLRPC_OK) {
		CM_FATAL1 (rc);
	}

	channel->usr.cl.clrpc_server_id = CLRPC_INVALID_SERVER_ID; /* sanity */

	switch (channel->state) {
	case CM_CL_UNREGISTER:
		cm_rpc_free_rqb (channel, rpc);
		cm_cl_delete_done (channel, CM_CUWF_UNREG_IF_CNF);
		break;

	default:
		CM_FATAL1(channel->state);
		break;
	}

}


/*----------------------------------------------------------------------------*/
LSA_VOID
cm_cl_delete_done (
	CM_CHANNEL_PTR_TYPE  channel,
	LSA_UINT16  wf_event
	)
{
	CM_ASSERT(channel->state == CM_CL_UNREGISTER);

	CM_CL_TRACE_02 (channel->trace_idx, LSA_TRACE_LEVEL_CHAT
		, "ClDelete: upper_wf(0x%x) wf_event(0x%x)", channel->usr.cl.upper_wf, wf_event
		);

	if (wf_event != 0)
	{
		CM_ASSERT (channel->usr.cl.upper_wf & wf_event);
		channel->usr.cl.upper_wf &= ~ wf_event;
	}

		/* unregister interface */
	if (channel->usr.cl.upper_wf & CM_CUWF_UNREG_IF_REQ)
	{
		channel->usr.cl.upper_wf &= ~ CM_CUWF_UNREG_IF_REQ;

		channel->usr.cl.upper_wf |= CM_CUWF_UNREG_IF_CNF;
		cm_cl_rpc_unregister_if_obj (channel);
	}
	if (channel->usr.cl.upper_wf & CM_CUWF_UNREG_IF_CNF)
	{
		CM_CL_TRACE_00 (channel->trace_idx, LSA_TRACE_LEVEL_CHAT
			, "ClDelete: upper_wf(interface-unregister-cnf)"
			);
		return;
	}

		/* wait if one global_control is runnning */
	if (cm_is_not_null (channel->usr.cl.upper_apdu))
	{
		CM_CL_TRACE_00 (channel->trace_idx, LSA_TRACE_LEVEL_CHAT
			, "ClDelete: upper_wf(cm_cl_acp_global_provider_control_done())"
			);
		return;
	}

	CM_ASSERT (channel->usr.cl.upper_wf == 0);

	cm_cl_client_free (channel);

	channel->state = CM_CH_READY;

	CM_ASSERT (CM_RQB_GET_OPCODE (channel->usr.cl.upper) == CM_OPC_CL_DELETE);
	cm_callback (channel, CM_OK, & channel->usr.cl.upper);
}


/*------------------------------------------------------------------------------
// handle opcode CM_OPC_CL_CONTROL
//----------------------------------------------------------------------------*/
LSA_VOID
cm_cl_opc_control (
	CM_CHANNEL_PTR_TYPE  channel,
	CM_UPPER_RQB_PTR_TYPE  rb
	)
{
	LSA_UINT16  rc;

	if (channel->state != CM_CL_READY) {
		CM_CL_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR, "!ch.state(%d) != CM_CL_READY", channel->state);
		cm_callback (channel, CM_ERR_SEQUENCE, & rb);
		return;
	}

	if (cm_is_not_null (channel->usr.cl.upper_apdu)) {
		CM_CL_TRACE_00 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR, "another apdu pending");
		cm_callback (channel, CM_ERR_SEQUENCE, & rb);
		return;
	}

	channel->usr.cl.upper_apdu = rb; /* keep */

	switch (rb->args.cl.control->control_tag) {
	case CM_CL_CONTROL_PASSIVATE:
	case CM_CL_CONTROL_ACTIVATE:
		cm_callback (channel, CM_ERR_NOT_YET, & channel->usr.cl.upper_apdu);
		return;

	case CM_CL_CONTROL_APDU_STATUS:
		rc = cm_cl_acp_global_provider_control (channel, rb->args.cl.control->apdu_status);
		if (rc != CM_OK) {
			cm_callback (channel, rc, & channel->usr.cl.upper_apdu);
			return;
		}
		return;

	case CM_CL_CONTROL_NOS_ASSIGNMENT_ON:
	case CM_CL_CONTROL_NOS_ASSIGNMENT_OFF:
		cm_cl_control_set_nos_assignment (channel);
		return;

	default:
		CM_CL_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR, "!control_tag(%d)", rb->args.cl.control->control_tag);
		cm_callback (channel , CM_ERR_PARAM, & rb);
		return;
	}
}


/*----------------------------------------------------------------------------*/
LSA_VOID
cm_cl_control_set_nos_assignment (
	CM_CHANNEL_PTR_TYPE  channel
	)
{
	if (cm_is_not_null (channel->usr.cl.upper_apdu))
	{
		LSA_UINT16  set_nos = channel->usr.cl.upper_apdu->args.cl.control->control_tag;

		if (set_nos == CM_CL_CONTROL_NOS_ASSIGNMENT_ON  ||  set_nos == CM_CL_CONTROL_NOS_ASSIGNMENT_OFF)
		{
			LSA_UINT16  idev;

				/* is there one AR in state NARE ? */
			for (idev = 0; idev < channel->usr.cl.dev_cnt; ++idev)
			{
				CM_CL_DEVICE_PTR_TYPE  dev = & channel->usr.cl.dev[idev];
				if (dev->state == CM_CL_DEV_ST_ACTIVE)
				{
					if (cm_is_not_null(dev->argr))
					{
						if (dev->argr->cm.cl.state == CM_DST_NARE)
						{
							CM_CL_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_NOTE_HIGH
								, "dev(%u) in state NARE, delay set-nos-assignment", dev->argr->device_nr
								);
							return;
						}
					}
				}
			}

				/* no AR in state NARE */
			channel->usr.cl.nos_assignment = set_nos;
			cm_callback (channel, CM_OK, & channel->usr.cl.upper_apdu);
		}
	}
}


/*------------------------------------------------------------------------------
// handle opcode for CM-CL
//----------------------------------------------------------------------------*/
LSA_VOID
cm_cl_request (
	CM_CHANNEL_PTR_TYPE channel,
	CM_UPPER_RQB_PTR_TYPE  rb
	)
{
	if (cm_is_null(rb->args.void_ptr))	/* args must be there always - with some exceptions */
	{
		switch( CM_RQB_GET_OPCODE(rb) )
		{
		case CM_OPC_CL_DELETE:
			break;

		default:
			CM_CL_TRACE_00(channel->trace_idx, LSA_TRACE_LEVEL_ERROR, "no args");
			cm_callback (channel, CM_ERR_PARAM, & rb);
			return;
		}
	}

	/***/

	switch( CM_RQB_GET_OPCODE(rb) )
	{
	case CM_OPC_CL_CREATE:
		cm_cl_opc_create (channel, rb);
		return;

	case CM_OPC_CL_DELETE:
		cm_cl_opc_delete (channel, rb);
		return;

	case CM_OPC_CL_CONTROL:
		cm_cl_opc_control (channel, rb);
		return;


	case CM_OPC_CL_DEVICE_ADD:
		cm_cl_opc_device_add (channel, rb);
		return;

	case CM_OPC_CL_DEVICE_REMOVE:
		cm_cl_opc_device_remove (channel, rb);
		return;

	case CM_OPC_CL_DEVICE_CONTROL:
		cm_cl_opc_device_control (channel, rb);
		return;


	case CM_OPC_CL_DEVICE_PROVIDE_EVENT:
		cm_cl_opc_device_provide_resource (channel, rb);
		break;

	case CM_OPC_CL_AR_ABORT_RSP:
		cm_cl_opc_ar_abort_rsp (channel, rb);
		return;

	case CM_OPC_CL_AR_PRE_DATA_RSP:
		cm_cl_opc_ar_pre_data_rsp (channel, rb);
		return;

	case CM_OPC_CL_AR_PRE_OFFLINE_RSP:
		cm_cl_opc_ar_pre_offline_rsp (channel, rb);
		return;

	case CM_OPC_CL_AR_IN_DATA_RSP:
		cm_cl_opc_ar_in_data_rsp (channel, rb);
		return;

	case CM_OPC_CL_AR_OFFLINE_RSP:
		cm_cl_opc_ar_offline_rsp (channel, rb);
		return;

	case CM_OPC_CL_AR_DATA_STATUS_RSP:
		cm_cl_opc_ar_data_status_rsp (channel, rb);
		return;


	case CM_OPC_CL_DEVICE_PROVIDE_ALARM:
		cm_cl_opc_device_provide_resource (channel, rb);
		break;

	case CM_OPC_CL_ALARM_RSP:
		cm_cl_opc_alarm_rsp (channel, rb);
		return;

	case CM_OPC_CL_ALARM_SEND:
		cm_cl_opc_alarm_send (channel, rb);
		return;


	case CM_OPC_CL_RECORD_READ:
	case CM_OPC_CL_RECORD_READ_QUERY:
		{
			LSA_UINT16  rc = cm_cl_opc_record_read (channel, rb);
			if (rc != CM_OK_PENDING) {
				cm_callback (channel, rc, & rb);
			}
		}
		return;

	case CM_OPC_CL_RECORD_WRITE:
		{
			LSA_UINT16  rc = cm_cl_opc_record_write (channel, rb);
			if (rc != CM_OK) {
				cm_callback (channel, rc, & rb);
			}
		}
		return;

	case CM_OPC_CL_RECORD_WRITE_MULTIPLE:
		cm_callback (channel, CM_ERR_NOT_YET, & rb);
		return;

	case CM_OPC_CL_LOGBOOK_READ:
		cm_cl_opc_logbook_read (channel, rb);
		return;


	case CM_OPC_CL_PRM_UPDATE:
		cm_cl_opc_prm_update (channel, rb);
		return;

	case CM_OPC_CL_AR_PRM_BEGIN_RSP:
		cm_cl_opc_ar_prm_begin_rsp (channel, rb);
		return;

	case CM_OPC_CL_AR_PRM_INDATA_RSP:
		cm_cl_opc_ar_prm_indata_rsp (channel, rb);
		return;

	default:
		CM_CL_TRACE_02(channel->trace_idx, LSA_TRACE_LEVEL_FATAL
			, "unhandled opcode(%u), path-type(%u)"
			, CM_RQB_GET_OPCODE(rb), channel->path_type
			);

		CM_FATAL();
		/*lint -unreachable */
		return;
	}
}


#if 0/*VV*/
/*----------------------------------------------------------------------------*/
LSA_VOID
cm_cl_callback (
	CM_CHANNEL_PTR_TYPE channel,
	LSA_UINT16 response,
	CM_UPPER_RQB_PTR_TYPE *upper_ptr
	)
{
	CM_RQB_SET_HANDLE (rb, channel->handle_upper);
	CM_RQB_SET_RESPONSE (rb, response);

	CM_CL_TRACE_04 (channel->trace_idx, LSA_TRACE_LEVEL_NOTE
		, "<<< cm-rqb(0x%08x) h(%u) cm-opc(%u) cm-rsp(%u)"
		, rb, CM_RQB_GET_HANDLE (rb), CM_RQB_GET_OPCODE (rb), CM_RQB_GET_RESPONSE (rb)
		);

	CM_REQUEST_UPPER_DONE (channel->callback, rb, channel->sysptr);
}
#endif


/*---------------------------------------------------------------------------*/
#else
# ifdef CM_MESSAGE
#  pragma CM_MESSAGE("***** not compiling client part (CM_CFG_MAX_CLIENTS=" CM_STRINGIFY(CM_CFG_MAX_CLIENTS) ")")
# endif /* CM_MESSAGE */
#endif
/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
