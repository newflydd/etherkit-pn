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
/*  F i l e               &F: cm_pof.c                                  :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  POF (plastic optical fiber) things                                       */
/*                                                                           */
/*  - alloc POF-RQB                                                          */
/*  - free POF-RQB                                                           */
/*  - channel open                                                           */
/*  - channel info                                                           */
/*  - channel close                                                          */
/*  - lower-done callback function                                           */
/*                                                                           */
/*****************************************************************************/
#define LTRC_ACT_MODUL_ID	43
#define CM_MODULE_ID		43

#include "cm_int.h"

CM_FILE_SYSTEM_EXTENSION(CM_MODULE_ID) /* no semicolon */


#if CM_CFG_USE_POF

/*----------------------------------------------------------------------------*/

static LSA_VOID
cm_pof_prm_indication(
	CM_CHANNEL_PTR_TYPE channel,
	CM_POF_LOWER_RQB_PTR_TYPE pof /* provide: LSA_NULL */
);

static LSA_VOID
cm_pof_request_lower(
	CM_CHANNEL_PTR_TYPE channel,
	LSA_OPCODE_TYPE opcode,
	LSA_UINT16 device_nr,
	LSA_UINT16 ar_nr,
	CM_POF_LOWER_RQB_PTR_TYPE pof
);


/*===========================================================================*/
/*=== POF CHANNEL-INIT / -UNDO-INIT =========================================*/
/*===========================================================================*/

LSA_VOID
cm_pof_channel_init(
	CM_CHANNEL_PTR_TYPE channel
) {
	if( channel->state != CM_CH_READY ) {
		CM_FATAL(); /* see caller */
	}

	/* provide some PRM indication resources (must do it in context of CM-POF, not CM-PD) */

	cm_pof_prm_indication(channel, LSA_NULL);
}

/*===========================================================================*/

LSA_UINT16
cm_pof_channel_undo_init(
	CM_CHANNEL_CONST_PTR_TYPE channel
) {
	/* NOTE: caller wipes out channel->usr.xxx when returning CM_OK */

	if( channel->state != CM_CH_READY ) {
		CM_FATAL(); /* see caller */
	}

	if( cm_is_not_null(channel->lower.channel[CM_PATH_TYPE_PD]) ) {
		CM_FATAL(); /* cm-pd not unlinked */
	}

	return CM_OK;
}

/*-----------------------------------------------------------------------------
//	get system-channel
//---------------------------------------------------------------------------*/
CM_CHANNEL_PTR_TYPE
cm_pof_channel_get(
	CM_CHANNEL_PTR_TYPE  channel
) {
	channel = channel->lower.channel[CM_PATH_TYPE_POF];

	if( cm_is_null(channel) ) {
		CM_FATAL(); /* a bug */
	}

	return channel;
}

/*-----------------------------------------------------------------------------
//	alloc lower POF-RQB that fits for all requests
//---------------------------------------------------------------------------*/
static CM_POF_LOWER_RQB_PTR_TYPE
cm_pof_alloc_rqb  (
	CM_CHANNEL_PTR_TYPE			channel
)
{
	CM_CHANNEL_PTR_TYPE pof_channel = cm_pof_channel_get(channel);

	CM_POF_LOWER_RQB_PTR_TYPE lower_rqb_ptr;

		/* alloc the RQB */
	CM_POF_ALLOC_LOWER_RQB (&lower_rqb_ptr, cm_null_user_id, sizeof (CM_POF_LOWER_RQB_TYPE), pof_channel->sysptr);

	if (cm_is_null (lower_rqb_ptr)) {
		CM_POF_TRACE_02 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
			, "h(%u) cannot alloc POF RQB, len(%u)"
			, channel->my_handle, sizeof (CM_POF_LOWER_RQB_TYPE)
			);
		return LSA_NULL;
	}

#if CM_DEBUG
	CM_POF_SET_NEXT_RQB_PTR (lower_rqb_ptr, LSA_NULL); /* sanity */
	CM_POF_SET_PREV_RQB_PTR (lower_rqb_ptr, LSA_NULL); /* sanity */
#endif

	return lower_rqb_ptr;
}


/*-----------------------------------------------------------------------------
//	free the RQB
//---------------------------------------------------------------------------*/
LSA_VOID
cm_pof_free_rqb (
	CM_CHANNEL_PTR_TYPE  channel,
	CM_POF_LOWER_RQB_PTR_TYPE  lower_rqb_ptr
)
{
	CM_CHANNEL_PTR_TYPE pof_channel = cm_pof_channel_get(channel);
	LSA_UINT16	rc;

	CM_ASSERT (cm_is_not_null(lower_rqb_ptr));

	CM_POF_FREE_LOWER_RQB (
		& rc,
		lower_rqb_ptr,
		pof_channel->sysptr
		);
	CM_ASSERT (rc == LSA_RET_OK);
	LSA_UNUSED_ARG(rc);
}


/*-----------------------------------------------------------------------------
//	open the channel
//---------------------------------------------------------------------------*/
LSA_VOID
cm_pof_channel_open (
	CM_CHANNEL_PTR_TYPE  channel
)
{
	CM_POF_LOWER_RQB_PTR_TYPE  pof;

	CM_ASSERT (channel->path_type == CM_PATH_TYPE_POF);

	CM_ASSERT (cm_pof_channel_get(channel) == channel);

	CM_ASSERT (channel->state == CM_CH_OPEN);

	CM_ASSERT (channel->lower.handle == CM_POF_INVALID_HANDLE);

	/***/

	channel->lower.prealloc.pof = LSA_NULL;

	pof = cm_pof_alloc_rqb (channel);

	if (cm_is_null (pof)) {
		cm_channel_done (channel, CM_ERR_ALLOC_LOWER);
		return;
	}

	CM_POF_TRACE_02 (channel->trace_idx, LSA_TRACE_LEVEL_NOTE
		, "h(%u) sys_path(%u)"
		, channel->my_handle, channel->usr.ch.curr->args.channel.open->sys_path
		);

	pof->args.channel.handle = CM_POF_INVALID_HANDLE;
	pof->args.channel.handle_upper = channel->my_handle;
	pof->args.channel.sys_path = channel->usr.ch.curr->args.channel.open->sys_path;
	pof->args.channel.Cbf = cm_pof_request_lower_done;

	CM_POF_SET_USER_ID_HANDLE_DEV_AR (pof, channel->my_handle, 0, 0);

	CM_POF_SET_HANDLE (pof, CM_POF_INVALID_HANDLE);
	CM_POF_SET_OPCODE (pof, POF_OPC_OPEN_CHANNEL);

	CM_POF_OPEN_CHANNEL_LOWER (pof, channel->sysptr);
}


/*----------------------------------------------------------------------------*/
static LSA_VOID
cm_pof_channel_open_done (
	CM_CHANNEL_PTR_TYPE  channel,
	CM_POF_LOWER_RQB_PTR_TYPE  pof
)
{
	LSA_UINT16 rc;

	CM_ASSERT (channel->state == CM_CH_OPEN);

	if (CM_POF_GET_RESPONSE (pof) == POF_RSP_OK) {

		channel->lower.handle = pof->args.channel.handle;
		channel->lower.prealloc.pof = pof;

		CM_ASSERT(channel->lower.handle != CM_POF_INVALID_HANDLE);

		CM_POF_TRACE_02 (channel->trace_idx, LSA_TRACE_LEVEL_NOTE
			, "h(%u) handle(%u)"
			, channel->my_handle, channel->lower.handle
			);

		rc = CM_OK;
	}
	else {

		CM_ASSERT(channel->lower.handle == CM_POF_INVALID_HANDLE);

		CM_POF_TRACE_02 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
			, "h(%u) pof-rsp(%u)"
			, channel->my_handle, CM_POF_GET_RESPONSE (pof)
			);

		rc = CM_ERR_LOWER_LAYER;

		cm_pof_free_rqb(channel, pof);
	}

	cm_channel_done (channel, rc);
}


/*-----------------------------------------------------------------------------
//	close the channel
//---------------------------------------------------------------------------*/
LSA_VOID
cm_pof_channel_close (
	CM_CHANNEL_CONST_PTR_TYPE channel
)
{
	CM_POF_LOWER_RQB_PTR_TYPE  pof;

	CM_ASSERT (channel->path_type == CM_PATH_TYPE_POF);

	CM_ASSERT (channel->state == CM_CH_CLOSE);

	CM_ASSERT (cm_is_not_null(channel->lower.prealloc.pof));

	pof = channel->lower.prealloc.pof;

	CM_POF_SET_USER_ID_HANDLE_DEV_AR (pof, channel->my_handle, 0, 0);

	CM_POF_SET_HANDLE (pof, channel->lower.handle);
	CM_POF_SET_OPCODE (pof, POF_OPC_CLOSE_CHANNEL);

	CM_POF_CLOSE_CHANNEL_LOWER (pof, channel->sysptr);
}


/*----------------------------------------------------------------------------*/
static LSA_VOID
cm_pof_channel_close_done (
	CM_CHANNEL_PTR_TYPE channel,
	CM_POF_LOWER_RQB_PTR_TYPE  pof
) {
	LSA_UINT16 rc;

	CM_ASSERT (channel->state == CM_CH_CLOSE);

	rc = CM_POF_GET_RESPONSE (pof);

	if (rc == POF_RSP_OK) {

		cm_pof_free_rqb (channel, pof);

		channel->lower.handle = CM_POF_INVALID_HANDLE;
		channel->lower.prealloc.pof = LSA_NULL;

		CM_POF_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_NOTE
			, "h(%u) pof-closed"
			, channel->my_handle
			);

		rc = CM_OK;
	}
	else {

		/* don't invalidate pof-handle, channel is not closed! */

		CM_POF_TRACE_02 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
			, "h(%u) pof-rsp(%u)"
			, channel->my_handle, rc
			);

		rc = CM_ERR_LOWER_LAYER;
	}

	cm_channel_done (channel, rc);
}


/*===========================================================================*/
/*=== PRM INTERFACE =========================================================*/
/*===========================================================================*/

CM_PORT_UINT
cm_pof_prm_provide_mask(
	CM_CHANNEL_PTR_TYPE		channel
) {
	CM_EDD_CHANNEL_PARAMS_PTR_TYPE edd_params = cm_edd_channel_params(channel);
	CM_PORT_UINT mask;

	mask = ((CM_PORT_UINT)2 << edd_params->MaxPortCnt) - 1;

	mask &= ~0x01; /* not providing on the interface */

	return mask;
}

/*----------------------------------------------------------------------------*/

static LSA_VOID
cm_pof_prm_indication(
	CM_CHANNEL_PTR_TYPE channel,
	CM_POF_LOWER_RQB_PTR_TYPE pof /* provide: LSA_NULL */
) {
	if( cm_is_null(pof) ) { /* provide */

		CM_EDD_CHANNEL_PARAMS_PTR_TYPE edd_params = cm_edd_channel_params(channel);
		CM_PORT_UINT provide_mask = 0;
		LSA_UINT16 port_id;

		for( port_id = 1; port_id <= edd_params->MaxPortCnt; ++port_id ) {

			LSA_UINT16 max = 2/*!*/;
			LSA_UINT16 cnt;

			for( cnt = 0; cnt < max; ++cnt ) {

				pof = cm_pof_alloc_rqb (channel);

				if (cm_is_null (pof)) {
					CM_POF_TRACE_00(channel->trace_idx, LSA_TRACE_LEVEL_FATAL, "cannot pof-alloc");
					CM_FATAL();
				}

				pof->args.PrmIndication.edd_port_id = port_id; /* an in-parameter! */

				pof->args.PrmIndication.diag_cnt = 0; /* sanity (an out-paramter!) */

				cm_pof_request_lower (channel, POF_OPC_PRM_INDICATION, 0, 0, pof);
			}

			provide_mask |= ((CM_PORT_UINT)1 << port_id);
		}

		if( provide_mask != cm_pof_prm_provide_mask(channel) ) {
			CM_FATAL(); /* a bug */
		}
	}
	else { /* handle indication and do re-provide */

		CM_POF_TRACE_02(channel->trace_idx, LSA_TRACE_LEVEL_CHAT
			, "pof-prm.IND edd_port_id(%u) diag_cnt(%u)"
			, pof->args.PrmIndication.edd_port_id
			, pof->args.PrmIndication.diag_cnt
			);

		if( channel->state >= CM_CH_CLOSE ) {

			CM_POF_TRACE_00(channel->trace_idx, LSA_TRACE_LEVEL_NOTE
				, "channel is closing, freeing rqb"
				);

			CM_ASSERT(
				CM_POF_GET_RESPONSE(pof) == POF_RSP_OK
				|| CM_POF_GET_RESPONSE(pof) == POF_RSP_OK_CANCEL
				);

			cm_pof_free_rqb(channel, pof);
		}
		else if( CM_POF_GET_RESPONSE(pof) != POF_RSP_OK ) {

			CM_FATAL1(CM_POF_GET_RESPONSE(pof)); /* what? why? */
		}
		else {

			if( pof->args.PrmIndication.diag_cnt > POF_PRM_INDICATION_DIAG_MAX_ENTRIES ) {
				CM_FATAL();
			}

			/***/

			{
			CM_CHANNEL_PTR_TYPE pd_channel = channel->lower.channel[CM_PATH_TYPE_PD];

			if( cm_is_null(pd_channel) )
			{
				CM_POF_TRACE_00(channel->trace_idx, LSA_TRACE_LEVEL_CHAT, "pof-prm.IND but no pd-channel, ignored");
			}
			else
			{
				cm_pd_pof_prm_indication_done(pd_channel, &pof->args.PrmIndication);
			}
			}

			/***/

			/*
			pof->args.PrmIndication.edd_port_id = unchanged (an in-parameter!)
			*/

			pof->args.PrmIndication.diag_cnt = 0; /* sanity (an out-paramter!) */

			cm_pof_request_lower(channel, POF_OPC_PRM_INDICATION, 0, 0, pof); /* re-provide */
		}

	}
}

/*----------------------------------------------------------------------------*/

LSA_VOID
cm_pof_prm_prepare (
	CM_CHANNEL_PTR_TYPE channel
) {
	CM_CHANNEL_PTR_TYPE  pof_channel = cm_pof_channel_get(channel);
	CM_POF_LOWER_RQB_PTR_TYPE  pof = pof_channel->lower.prealloc.pof;

	CM_ASSERT (cm_is_not_null(pof));

	pof->args.PrmPrepare.dummy = 1;

	cm_pof_request_lower (channel, POF_OPC_PRM_PREPARE, 0, 0, pof);
}

/*----------------------------------------------------------------------------*/

LSA_VOID
cm_pof_prm_write (
	CM_CHANNEL_PTR_TYPE		channel,
	LSA_UINT16				edd_port_id,
	LSA_UINT16				slot_number,
	LSA_UINT16				subslot_number,
	LSA_UINT32				record_index,
	LSA_UINT32				record_data_length,
	CM_UPPER_MEM_PTR_TYPE	record_data
) {
	CM_CHANNEL_PTR_TYPE  pof_channel = cm_pof_channel_get(channel);
	CM_POF_LOWER_RQB_PTR_TYPE  pof = pof_channel->lower.prealloc.pof;

	CM_ASSERT (cm_is_not_null(pof));

	pof->args.PrmWrite.edd_port_id = edd_port_id;
	pof->args.PrmWrite.slot_number = slot_number;
	pof->args.PrmWrite.subslot_number = subslot_number;
	pof->args.PrmWrite.record_index = record_index;
	pof->args.PrmWrite.record_data_length = record_data_length;

	/* record_data used as reference, check this */
	pof->args.PrmWrite.record_data = CM_CAST_POF_MEM_U8_PTR (record_data);

	cm_pof_request_lower (channel, POF_OPC_PRM_WRITE, 0, 0, pof);
}

/*----------------------------------------------------------------------------*/

LSA_UINT16
cm_pof_prm_read (
	CM_CHANNEL_PTR_TYPE		channel,
	LSA_UINT16				edd_port_id,
	LSA_UINT16				slot_number,
	LSA_UINT16				subslot_number,
	LSA_UINT32				record_index,
	LSA_UINT32				record_data_length,
	CM_UPPER_MEM_PTR_TYPE	record_data
) {
	CM_POF_LOWER_RQB_PTR_TYPE  pof;

	pof = cm_pof_alloc_rqb (channel);
	if (cm_is_null (pof)) {
		return CM_ERR_ALLOC_LOWER;
	}

	pof->args.PrmRead.edd_port_id = edd_port_id;
	pof->args.PrmRead.slot_number = slot_number;
	pof->args.PrmRead.subslot_number = subslot_number;
	pof->args.PrmRead.record_index = record_index;
	pof->args.PrmRead.record_data_length = record_data_length;

	/* record_data used as reference, check this */
	pof->args.PrmRead.record_data = CM_CAST_POF_MEM_U8_PTR (record_data);

	cm_pof_request_lower (channel, POF_OPC_PRM_READ, 0, 0, pof);
	return CM_OK;
}

/*----------------------------------------------------------------------------*/

LSA_VOID
cm_pof_prm_end (
	CM_CHANNEL_PTR_TYPE channel
) {
	CM_CHANNEL_PTR_TYPE  pof_channel = cm_pof_channel_get(channel);
	CM_POF_LOWER_RQB_PTR_TYPE  pof = pof_channel->lower.prealloc.pof;

	CM_ASSERT (cm_is_not_null(pof));

#if POF_PRM_PORT_IS_COMPACT != 0 || POF_PRM_PORT_IS_MODULAR != 1
#error "POF_PRM_PORT_IS_COMPACT/POF_PRM_PORT_IS_MODULAR are not 0/1"
#endif

	cm_pd_prm_set_prm_port_flags (channel, pof->args.PrmEnd.IsModularPort
		, sizeof (pof->args.PrmEnd.IsModularPort), sizeof (pof->args.PrmEnd.IsModularPort[0])
		, CM_PDEV_PROP_TYPE_MODULAR
		);

	cm_pof_request_lower (channel, POF_OPC_PRM_END, 0, 0, pof);
}

/*----------------------------------------------------------------------------*/

LSA_VOID
cm_pof_prm_commit (
	CM_CHANNEL_PTR_TYPE channel
) {
	CM_CHANNEL_PTR_TYPE  pof_channel = cm_pof_channel_get(channel);
	CM_POF_LOWER_RQB_PTR_TYPE  pof = pof_channel->lower.prealloc.pof;

	CM_ASSERT (cm_is_not_null(pof));

#if POF_PRM_APPLY_RECORD_PORT_PARAMS != 0 || POF_PRM_APPLY_DEFAULT_PORT_PARAMS != 1
#error "POF_PRM_APPLY_RECORD_PORT_PARAMS/POF_PRM_APPLY_DEFAULT_PORT_PARAMS are not 0/1"
#endif

	cm_pd_prm_set_prm_port_flags (channel, pof->args.PrmCommit.ApplyDefaultPortparams
		, sizeof (pof->args.PrmCommit.ApplyDefaultPortparams), sizeof (pof->args.PrmCommit.ApplyDefaultPortparams[0])
		, CM_PDEV_PROP_DEFAULT_ENABLED);

	cm_pof_request_lower (channel, POF_OPC_PRM_COMMIT, 0, 0, pof);
}

/*===========================================================================*/
/*=== PULL/PLUG =============================================================*/
/*===========================================================================*/

LSA_VOID
cm_pof_pullplug_change_port (
	CM_CHANNEL_PTR_TYPE  channel,
	LSA_UINT8  submodule_state,
	LSA_UINT8  apply_default_portparams
	)
{
	CM_CHANNEL_PTR_TYPE  pof_channel = cm_pof_channel_get(channel);
	CM_POF_LOWER_RQB_PTR_TYPE  pof = pof_channel->lower.prealloc.pof;

	CM_ASSERT (cm_is_not_null(pof));

	pof->args.PrmChangePort.edd_port_id = channel->usr.pd.pullplug.port->port_id;
	pof->args.PrmChangePort.module_state = submodule_state;

#if POF_PRM_APPLY_RECORD_PORT_PARAMS != 0 || POF_PRM_APPLY_DEFAULT_PORT_PARAMS != 1
#error "POF_PRM_APPLY_RECORD_PORT_PARAMS/POF_PRM_APPLY_DEFAULT_PORT_PARAMS are not 0/1"
#endif

	pof->args.PrmChangePort.apply_default_portparams = apply_default_portparams;

	cm_pof_request_lower (channel, POF_OPC_PRM_CHANGE_PORT, 0, 0, pof);
}

/*----------------------------------------------------------------------------*/
static LSA_VOID
cm_pof_pullplug_change_port_done (
	CM_CHANNEL_PTR_TYPE  channel,
	CM_POF_LOWER_RQB_CONST_PTR_TYPE  pof
	)
{
	if (cm_pd_sc_map_response(LSA_COMP_ID_POF, CM_POF_GET_RESPONSE (pof)) != CM_OK)
	{
		CM_FATAL1 (CM_POF_GET_RESPONSE (pof));
	}

#if POF_PRM_PORT_PARAMS_APPLICABLE != 0 || POF_PRM_PORT_PARAMS_NOT_APPLICABLE != 1
#error "POF_PRM_PORT_PARAMS_APPLICABLE/POF_PRM_PORT_PARAMS_NOT_APPLICABLE are not 0/1"
#endif

	cm_pd_pullplug_sm_trigger (channel, CM_PD_SCBF_POF, pof->args.PrmChangePort.port_params_not_applicable);
}

/*===========================================================================*/
/*=== POF REQUEST LOWER =====================================================*/
/*===========================================================================*/

static LSA_VOID
cm_pof_request_lower(
	CM_CHANNEL_PTR_TYPE channel,
	LSA_OPCODE_TYPE opcode,
	LSA_UINT16 device_nr,
	LSA_UINT16 ar_nr,
	CM_POF_LOWER_RQB_PTR_TYPE pof
) {
	CM_CHANNEL_PTR_TYPE pof_channel = cm_pof_channel_get(channel);

	CM_POF_SET_USER_ID_HANDLE_DEV_AR (pof, channel->my_handle, device_nr, ar_nr);

	CM_POF_SET_HANDLE (pof, pof_channel->lower.handle);
	CM_POF_SET_OPCODE (pof, opcode);

	CM_POF_TRACE_05 (channel->trace_idx, LSA_TRACE_LEVEL_NOTE
		, ">>> pof-rqb(0x%08x) pof-opc(%u) f-h(%u) f-dev(%u) f-ar(%u)"
		, pof, CM_POF_GET_OPCODE (pof)
		, CM_POF_GET_USER_ID_HANDLE (pof), CM_POF_GET_USER_ID_DEV (pof), CM_POF_GET_USER_ID_AR (pof)
		);

	CM_POF_REQUEST_LOWER (pof, pof_channel->sysptr);
}

/*===========================================================================*/
/*=== POF LOWER DONE ========================================================*/
/*===========================================================================*/

LSA_VOID
cm_pof_request_lower_done (
	CM_POF_LOWER_RQB_PTR_TYPE  pof
) {
	CM_CHANNEL_PTR_TYPE  channel;

	if( cm_is_null(pof) ) {
		CM_POF_TRACE_00 (0, LSA_TRACE_LEVEL_FATAL, "cm_is_null(pof)");
		CM_FATAL ();
		return;
	}

	channel = cm_channel_from_handle (CM_POF_GET_USER_ID_HANDLE (pof));

	if( cm_is_null (channel) ) {

		CM_POF_TRACE_01 (0, LSA_TRACE_LEVEL_FATAL,
			"invalid handle(%u)",
			CM_POF_GET_USER_ID_HANDLE (pof)
			);
		CM_FATAL ();
		return;
	}

	switch (CM_POF_GET_RESPONSE (pof)) {
	case POF_RSP_OK:
	case POF_RSP_OK_CANCEL:
		CM_POF_TRACE_06 (channel->trace_idx, LSA_TRACE_LEVEL_NOTE
			, "<<< pof-rqb(0x%08x) pof-opc(%u) pof-rsp(%u) t-h(%u) t-dev(%u) t-ar(%u)"
			, pof, CM_POF_GET_OPCODE (pof), CM_POF_GET_RESPONSE (pof)
			, CM_POF_GET_USER_ID_HANDLE (pof), CM_POF_GET_USER_ID_DEV (pof), CM_POF_GET_USER_ID_AR (pof)
			);
		break;

	case POF_RSP_ERR_PRM_INDEX:
	/*case POF_RSP_ERR_PRM_PORTID:*/
		CM_POF_TRACE_06 (channel->trace_idx, LSA_TRACE_LEVEL_NOTE_HIGH /* AP00431325: don't trace with error-level if "not a pof-port" etc. */
			, "<<< pof-rqb(0x%08x) pof-opc(%u) pof-rsp(%u) t-h(%u) t-dev(%u) t-ar(%u)"
			, pof, CM_POF_GET_OPCODE (pof), CM_POF_GET_RESPONSE (pof)
			, CM_POF_GET_USER_ID_HANDLE (pof), CM_POF_GET_USER_ID_DEV (pof), CM_POF_GET_USER_ID_AR (pof)
			);
		break;

	default:
		CM_POF_TRACE_06 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
			, "<<< pof-rqb(0x%08x) pof-opc(%u) pof-rsp(%u) t-h(%u) t-dev(%u) t-ar(%u)"
			, pof, CM_POF_GET_OPCODE (pof), CM_POF_GET_RESPONSE (pof)
			, CM_POF_GET_USER_ID_HANDLE (pof), CM_POF_GET_USER_ID_DEV (pof), CM_POF_GET_USER_ID_AR (pof)
			);
		break;
	}

	switch( channel->path_type ) {

	case CM_PATH_TYPE_POF:

		switch (CM_POF_GET_OPCODE (pof)) {

		case POF_OPC_OPEN_CHANNEL:
			cm_pof_channel_open_done (channel, pof);
			break;

		case POF_OPC_CLOSE_CHANNEL:
			cm_pof_channel_close_done (channel, pof);
			break;

		case POF_OPC_PRM_INDICATION:
			cm_pof_prm_indication (channel, pof);
			break;

		default:
			CM_POF_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_FATAL
				, "unhandled opcode(%u)", CM_POF_GET_OPCODE (pof)
				);
			CM_FATAL (); /* should not happen */
			break;
		}
		break;

	case CM_PATH_TYPE_PD:

		switch (CM_POF_GET_OPCODE (pof)) {

		case POF_OPC_PRM_PREPARE:
			cm_pd_pof_prm_prepare_done (channel, pof);
			break;

		case POF_OPC_PRM_WRITE:
			cm_pd_pof_prm_write_done (channel, pof);
			break;

		case POF_OPC_PRM_END:
			cm_pd_pof_prm_end_done (channel, pof);
			break;

		case POF_OPC_PRM_COMMIT:
			cm_pd_pof_prm_commit_done (channel, pof);
			break;

		case POF_OPC_PRM_READ:
			cm_pd_pof_prm_read_done (channel, pof);
			break;

		case POF_OPC_PRM_CHANGE_PORT:
			cm_pof_pullplug_change_port_done (channel, pof);
			break;

		default:
			CM_POF_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_FATAL
				, "unhandled opcode(%u)", CM_POF_GET_OPCODE (pof)
				);
			CM_FATAL (); /* should not happen */
			break;
		}
		break;

	default:
		CM_POF_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_FATAL
			, "wrong path-type(%u)"
			, channel->path_type
			);
		CM_FATAL (); /* should not happen */
		break;
	}
}

/*===========================================================================*/

#else
#  ifdef CM_MESSAGE
#    pragma CM_MESSAGE ("compiled _WITHOUT_ POF; CM_CFG_USE_POF=" CM_STRINGIFY(CM_CFG_USE_POF))
#  endif /* CM_MESSAGE */
#endif /* CM_CFG_USE_POF */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
