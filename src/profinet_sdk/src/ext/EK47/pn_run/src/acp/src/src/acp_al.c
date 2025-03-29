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
/*  C o m p o n e n t     &C: ACP (Alarm Consumer Provider)             :C&  */
/*                                                                           */
/*  F i l e               &F: acp_al.c                                  :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Implements the PROFINET IO alarm handling                                */
/*                                                                           */
/*****************************************************************************/
#define LTRC_ACT_MODUL_ID  6
#define ACP_MODULE_ID      LTRC_ACT_MODUL_ID

#include "acp_int.h"

ACP_FILE_SYSTEM_EXTENSION(ACP_MODULE_ID)


/*----------------------------------------------------------------------------*/
/* common */
#if ACP_ALARM_PRIORITY_LOW != 0  || ACP_ALARM_PRIORITY_HIGH != 1
# error "alarm priority wrong index"
#endif

/*----------------------------------------------------------------------------
	from IEC 61156 Part 6

	BlockHeader ::=
		BlockType(u16), BlockLength(u16), BlockVersionHigh(u8), BlockVersionLow(u8)

	AlarmItem ::=
		UserStructureIdentifier(u16), Data(*)

	AlarmNotification-PDU ::=
		BlockHeader,
		AlarmType(u16), API(u32), SlotNumber(u16), SubslotNumber(u16),
		ModuleIdentNumber(u32), SubmoduleIdentNumber(u32), AlarmSpecifier(u16),
		[AlarmItem]

	AlarmAck-PDU ::=
		BlockHeader,
		AlarmType(u16), API(u32), SlotNumber(u16), SubslotNumber(u16),
		AlarmSpecifier(u16), PNIOStatus

----------------------------------------------------------------------------*/

#define ACP_ALARM_BLOCK_VERSION			0x0100	/**< BlockVersionHigh, BlockVersionLow (MSB-first) */

#define ACP_ALARM_BLOCK_REQUEST_HIGH	0x0001
#define ACP_ALARM_BLOCK_REQUEST_LOW		0x0002
#define ACP_ALARM_BLOCK_ACK_HIGH		0x8001
#define ACP_ALARM_BLOCK_ACK_LOW			0x8002

#define ACP_ALARM_BLOCK_HEADER_PDU_SIZE \
	(sizeof(LSA_UINT16) + sizeof(LSA_UINT16) + sizeof(LSA_UINT16))

#define ACP_ALARM_REQUEST_SIZE \
	(ACP_ALARM_BLOCK_HEADER_PDU_SIZE \
	+ sizeof(LSA_UINT16) + sizeof(LSA_UINT32) + sizeof(LSA_UINT16) + sizeof(LSA_UINT16) \
	+ sizeof(LSA_UINT32) + sizeof(LSA_UINT32) + sizeof(LSA_UINT16))

#define ACP_ALARM_ACK_SIZE \
	(ACP_ALARM_BLOCK_HEADER_PDU_SIZE \
	+ sizeof(LSA_UINT16) + sizeof(LSA_UINT32) + sizeof(LSA_UINT16) + sizeof(LSA_UINT16) \
	+ sizeof(LSA_UINT16) + sizeof(LSA_UINT32))



/*----------------------------------------------------------------------------*/

ACP_LOCAL_ALARM_PTR
acp_alarm_find (
	ACP_CHANNEL_CONST_PTR  channel,
	LSA_UINT16  alarm_ref,
	LSA_BOOL  do_elog
	)
{
	ACP_ASSERT (channel->detailptr->channel_type == ACP_CHANNEL_TYPE_USER);

	if (channel->detailptr->type.user.alarm_ref_base <= alarm_ref
		&& alarm_ref < channel->detailptr->type.user.alarm_ref_base + channel->detailptr->type.user.alarm_nr_of_ref)
	{
		alarm_ref = ACP_CAST_U16 (alarm_ref - channel->detailptr->type.user.alarm_ref_base);
		return & channel->alarms[alarm_ref];
	}

	if (do_elog) {
		ACP_RTA_TRACE_04 (channel->trace_idx, LSA_TRACE_LEVEL_UNEXP
			, "h(%u): alarm_ref(%u) not in range(%u:%u)"
			, channel->my_handle, alarm_ref
			, channel->detailptr->type.user.alarm_ref_base
			, channel->detailptr->type.user.alarm_nr_of_ref
			);
	}
	return LSA_NULL;
}


/*------------------------------------------------------------------------------
//	returns the count of open alarms
//----------------------------------------------------------------------------*/
LSA_UINT16
acp_alarm_count_open (
	ACP_CHANNEL_CONST_PTR  channel
	)
{
	LSA_UINT16  cnt = 0;
	LSA_UINT16  i;

	for (i = 0; i < channel->detailptr->type.user.alarm_nr_of_ref; ++i) {
		if (channel->alarms[i].state != ACP_ALARM_ST_FREE) {
			++cnt;
		}
	}
	return cnt;
}


/*------------------------------------------------------------------------------
//	handle opcode ACP_OPC_ALARM_OPEN
//----------------------------------------------------------------------------*/
LSA_VOID
acp_op_alarm_open (
	ACP_CHANNEL_PTR  channel,
	LSA_BOOL  is_udp_alarm
	)
{
	ACP_UPPER_RQB_PTR_TYPE  rb;
	ACP_LOCAL_ALARM_PTR  alarm;

#if ACP_DEBUG
	{
	LSA_INT diff = ACP_ALARM_USER_DATA_OFFSET - ACP_ALARM_REQUEST_SIZE - 2 /* UserDataLen */;
	ACP_ASSERT (diff == 0);
	}
#endif

	rb = channel->upper;
	ACP_ASSERT (acp_is_not_null (rb->args));

		/* must call channel-info first */
	if (channel->mac_addr[0] == 0xFF) {
		ACP_RTA_TRACE_00 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
			, "local mac_addr not set, must call channel-info first"
			);
		acp_callback (channel, ACP_ERR_SEQUENCE);
		return;
	}

		/* search alarm_id */
	alarm = acp_alarm_find (channel, rb->args->alarm.open.alarm_ref, LSA_TRUE);
	if (acp_is_null (alarm)) {
		acp_callback (channel, ACP_ERR_PARAM);
		return;
	}
	if (alarm->state != ACP_ALARM_ST_FREE) {
		ACP_RTA_TRACE_02 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
			, "aref(%u) state(%u) != ACP_ALARM_ST_FREE"
			, alarm->alarm_ref, alarm->state
			);
		acp_callback (channel, ACP_ERR_SEQUENCE);
		return;
	}

	if (is_udp_alarm) {
		if (acp_is_null_ip (rb->args->alarm.open.dst_ip)) {
			ACP_RTA_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
				, "aref(%u) dst_ip is null ip"
				, alarm->alarm_ref
				);
			acp_callback (channel, ACP_ERR_PARAM);
			return;
		}
		acp_callback (channel, ACP_ERR_UNSUPPORTED);
		return;
	}

		/* see IEC 61158-6 */
	if (! (rb->args->alarm.open.timeout100ms > 0)) {
		ACP_RTA_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
			, "! (timeout100ms(%u) > 0)"
			, rb->args->alarm.open.timeout100ms
			);
		acp_callback (channel, ACP_ERR_PARAM);
		return;
	}
	if (! (3 <= rb->args->alarm.open.retry_count && rb->args->alarm.open.retry_count <= 15)) {
		ACP_RTA_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
			, "! (3 <= retry_count(%u) <= 15)"
			, rb->args->alarm.open.retry_count
			);
		acp_callback (channel, ACP_ERR_PARAM);
		return;
	}


		/* local alarm-data */
	alarm->alarm_ref = rb->args->alarm.open.alarm_ref; /* the key for EDD && ACP */
	alarm->channel = channel;
	alarm->usr_id = rb->args->alarm.open.usr_id;
	alarm->state = ACP_ALARM_ST_WF_OPEN;
	alarm->is_udp = is_udp_alarm;

	AcpListInitialize(&alarm->send_list[ACP_ALARM_PRIORITY_LOW]);
	AcpListInitialize(&alarm->send_list[ACP_ALARM_PRIORITY_HIGH]);

#if ACP_FRAG_MODE
	AcpListInitialize(&alarm->send_list[ACP_ASRT_PRIORITY_FRAG]);
	AcpListInitialize(&alarm->frag_provide_list);
#endif

	ACP_RTA_TRACE_10 (channel->trace_idx, LSA_TRACE_LEVEL_NOTE_HIGH
		, "h(%u): aref(%u) dst-aref(%u) dst-mac(%02x-%02x-%02x-%02x-%02x-%02x) usr_id(0x%x)"
		, channel->my_handle, alarm->alarm_ref, alarm->dst_alarm_ref
		, alarm->dst_mac[0], alarm->dst_mac[1], alarm->dst_mac[2]
		, alarm->dst_mac[3], alarm->dst_mac[4], alarm->dst_mac[5]
		, alarm->usr_id
		);

	{
	ACP_RTA_PTR  rta = acp_rta_from_alarm (alarm);

	acp_rta_open (channel, rta,
			rb->args->alarm.open.alarm_ref,
			rb->args->alarm.open.dst_alarm_ref,
			rb->args->alarm.open.dst_mac,
			rb->args->alarm.open.dst_ip,
			rb->args->alarm.open.src_ip,
			rb->args->alarm.open.vlan_id,
			rb->args->alarm.open.timeout100ms,
			rb->args->alarm.open.retry_count
		);
	return;
	}
}


/*------------------------------------------------------------------------------
//	handle response for ACP_OPC_ALARM_OPEN
//----------------------------------------------------------------------------*/
LSA_VOID
acp_rta_open_done (
	ACP_CHANNEL_PTR  channel,
	ACP_RTA_PTR  rta,
	LSA_UINT16  rsp
	)
{
	ACP_LOCAL_ALARM_PTR  alarm = acp_alarm_from_rta (rta);

	LSA_UNUSED_ARG (alarm);

	ACP_ASSERT (acp_is_not_null (channel->upper));
	ACP_ASSERT (ACP_RQB_GET_OPCODE(channel->upper) == ACP_OPC_ALARM_OPEN);

	if (rsp == ACP_OK) {
		alarm->state = ACP_ALARM_ST_OPEN;

		ACP_RTA_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_NOTE_HIGH
			, "aref(%u) Ok"
			, alarm->alarm_ref
			);
	}
	else {
		alarm->state = ACP_ALARM_ST_FREE;

		ACP_RTA_TRACE_02 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
			, "aref(%u) ERROR(%u)"
			, alarm->alarm_ref, rsp
			);
	}

	acp_callback (channel, rsp);
}


/*------------------------------------------------------------------------------
//	handle opcode ACP_OPC_ALARM_CLOSE
//----------------------------------------------------------------------------*/
LSA_VOID
acp_op_alarm_close (
	ACP_CHANNEL_PTR  channel
	)
{
	ACP_UPPER_RQB_PTR_TYPE  rb;
	ACP_LOCAL_ALARM_PTR  alarm;

	rb = channel->upper;
	ACP_ASSERT (acp_is_not_null (rb));

	alarm = acp_alarm_find (channel, rb->args->alarm.close.alarm_ref, LSA_TRUE);
	if (acp_is_null (alarm)) {
		acp_callback (channel, ACP_ERR_PARAM);
		return;
	}

	if (rb->args->alarm.close.pnio_status_reason < ACP_PNIO_STATUS_REASON_MIN)
	{
		ACP_RTA_TRACE_03 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
			, "h(%u): pnio_status_reason(%u) not in range[%u..]"
			, channel->my_handle, rb->args->alarm.close.pnio_status_reason
			, ACP_PNIO_STATUS_REASON_MIN
			);
		acp_callback (channel, ACP_ERR_PARAM);
		return;
	}

	if (! (alarm->state & (ACP_ALARM_ST_OPEN | ACP_ALARM_ST_ERROR))) {
		ACP_RTA_TRACE_02 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
			, "aref(%u) state(%u) != _ST_OPEN | _ST_ERROR"
			, alarm->alarm_ref, alarm->state
			);
		acp_callback (channel, ACP_ERR_SEQUENCE);
		return;
	}

	ACP_RTA_TRACE_04 (channel->trace_idx, LSA_TRACE_LEVEL_NOTE_HIGH
		, "aref(%u) state(%u) reason(%u) pnio_version(0x%08x)"
		, alarm->alarm_ref, alarm->state, rb->args->alarm.close.pnio_status_reason
		, rb->args->alarm.close.error_info.pnio_version
		);

	alarm->state = ACP_ALARM_ST_WF_FREE;
	alarm->close_reason = rb->args->alarm.close.pnio_status_reason;

	{
	ACP_RTA_PTR  rta = acp_rta_from_alarm (alarm);

	acp_rta_close (channel, rta);
	return;
	}
}


/*------------------------------------------------------------------------------
//	handle response for ACP_OPC_ALARM_CLOSE
//----------------------------------------------------------------------------*/
LSA_VOID
acp_rta_close_done (
	ACP_CHANNEL_PTR  channel,
	ACP_RTA_PTR  rta,
	LSA_UINT16  rsp
	)
{
	ACP_LOCAL_ALARM_PTR  alarm = acp_alarm_from_rta (rta);

	LSA_UNUSED_ARG (alarm);

	ACP_ASSERT (acp_is_not_null (channel->upper));
	ACP_ASSERT (ACP_RQB_GET_OPCODE(channel->upper) == ACP_OPC_ALARM_CLOSE);

	if (rsp == ACP_OK) {
		alarm->state = ACP_ALARM_ST_FREE;

		ACP_RTA_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_NOTE_HIGH
			, "aref(%u) Ok"
			, alarm->alarm_ref
			);
	}
	else {
		alarm->state = ACP_ALARM_ST_ERROR; /* cannot fall back to original state */

		ACP_RTA_TRACE_02 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
			, "aref(%u) ERROR(%u)"
			, alarm->alarm_ref, rsp
			);
	}

	acp_callback (channel, rsp);
}


/*----------------------------------------------------------------------------*/
LSA_UINT16
acp_alarm_build_data (
	ACP_CHANNEL_PTR  channel,
	ACP_LOCAL_ALARM_CONST_PTR  alarm,
	LSA_UINT8  prio,
	EDD_UPPER_MEM_U8_PTR_TYPE  dp,
	LSA_UINT16  dp_size
	)
{
	ACP_UPPER_RQB_PTR_TYPE  rb;
	LSA_UINT16  is_ack;
	LSA_UINT16  block_type;
	LSA_UINT16  block_length;
	LSA_UINT16  written;
	EDD_UPPER_MEM_U8_PTR_TYPE  dp_start;

	LSA_UNUSED_ARG (channel);

	ACP_ASSERT (acp_is_not_null (channel));
	ACP_ASSERT (acp_is_not_null (alarm));
	ACP_ASSERT (prio == ACP_ALARM_PRIORITY_LOW || prio == ACP_ALARM_PRIORITY_HIGH);
	ACP_ASSERT (acp_is_not_null (dp));

	rb = AcpListIterFirst(&alarm->send_list[prio], ACP_UPPER_RQB_PTR_TYPE);
	ACP_ASSERT (acp_is_not_null (rb));

	is_ack = 0;
	if ((ACP_RQB_GET_OPCODE (rb) == ACP_OPC_ALARM_ACK_SEND)) {
		is_ack = 1;
	}

	written = 0;
	dp_start = dp;

	if (is_ack) {
		block_length = ACP_ALARM_ACK_SIZE - 2*sizeof(LSA_UINT16);
		/* means  total_size-sizeof(AlarmHeader.Type)-sizeof(AlarmHeader.Length) */
		written += ACP_ALARM_ACK_SIZE;

		if (rb->args->alarm.data.priority == ACP_ALARM_PRIORITY_HIGH) {
			block_type = ACP_ALARM_BLOCK_ACK_HIGH;
		}
		else {
			block_type = ACP_ALARM_BLOCK_ACK_LOW;
		}
	}
	else {
		block_length = ACP_ALARM_REQUEST_SIZE - 2*sizeof(LSA_UINT16);
		/* means total_size-sizeof(AlarmHeader.Type)-sizeof(AlarmHeader.Length) */
		written += ACP_ALARM_REQUEST_SIZE;

		if (rb->args->alarm.data.user_length > 0) {
			block_length = ACP_CAST_U16 (block_length + sizeof(LSA_UINT16) + rb->args->alarm.data.user_length);
			written = ACP_CAST_U16 (written + sizeof(LSA_UINT16) + rb->args->alarm.data.user_length);
		}

		if (rb->args->alarm.data.priority == ACP_ALARM_PRIORITY_HIGH) {
			block_type = ACP_ALARM_BLOCK_REQUEST_HIGH;
		}
		else {
			block_type = ACP_ALARM_BLOCK_REQUEST_LOW;
		}
	}

	if (written > dp_size) {
		ACP_RTA_TRACE_02 (channel->trace_idx, LSA_TRACE_LEVEL_UNEXP
			, "need.written(%u) > dp_size(%u)"
			, written, dp_size
			);
		return 0;
	}

#if ACP_DEBUG
/*	{LSA_INT i; for (i = 0; i < dp_size; ++i) {	dp[i] = 0xCC; }} */
#endif

	acp_dp_set_u16 (dp, block_type);
	acp_dp_set_u16 (dp, block_length);
	acp_dp_set_u16 (dp, ACP_ALARM_BLOCK_VERSION);

	acp_dp_set_u16 (dp, rb->args->alarm.data.alarm_type);
	acp_dp_set_u32 (dp, rb->args->alarm.data.api);
	acp_dp_set_u16 (dp, rb->args->alarm.data.slot_nr);
	acp_dp_set_u16 (dp, rb->args->alarm.data.subslot_nr);

		/* if alarm-ack */
	if (is_ack) {
		acp_dp_set_u16 (dp, rb->args->alarm.data.alarm_specifier);
		acp_dp_set_u32 (dp, rb->args->alarm.data.pnio_status);

		ACP_RTA_TRACE_08 (channel->trace_idx, LSA_TRACE_LEVEL_NOTE
			, "ALARM-ACK:aref(%u) block(0x%04x) type(%u) api(%u) slot(%u) subslot(%u) spec(0x%04x) pnio-stat(0x%08x)"
			, alarm->alarm_ref, block_type, rb->args->alarm.data.alarm_type
			, rb->args->alarm.data.api
			, rb->args->alarm.data.slot_nr, rb->args->alarm.data.subslot_nr
			, rb->args->alarm.data.alarm_specifier, rb->args->alarm.data.pnio_status
			);
		return written;
	}

		/* else alarm */
	acp_dp_set_u32 (dp, rb->args->alarm.data.module_ident);
	acp_dp_set_u32 (dp, rb->args->alarm.data.submodule_ident);
	acp_dp_set_u16 (dp, rb->args->alarm.data.alarm_specifier);

		/* optional user_tag, user_data */
	if (rb->args->alarm.data.user_length > 0) {
		acp_dp_set_u16 (dp, rb->args->alarm.data.user_tag);
		ACP_ASSERT (acp_is_not_null (rb->args->alarm.data.user_data));
		ACP_MEMCPY (dp,
				rb->args->alarm.data.user_data,
				rb->args->alarm.data.user_length
			);
		dp += rb->args->alarm.data.user_length;
	}

	ACP_RTA_TRACE_11 (channel->trace_idx, LSA_TRACE_LEVEL_NOTE
		, "ALARM-SEND:aref(%u) block(0x%04x) type(%u) api(%u) slot(%u) subslot(%u) spec(0x%04x) mod_ident(%u) sub_ident(%u)  usr-length(%u) usr-tag(0x%04x)"
		, alarm->alarm_ref, block_type, rb->args->alarm.data.alarm_type
		, rb->args->alarm.data.api
		, rb->args->alarm.data.slot_nr, rb->args->alarm.data.subslot_nr
		, rb->args->alarm.data.alarm_specifier
		, rb->args->alarm.data.module_ident, rb->args->alarm.data.submodule_ident
		, rb->args->alarm.data.user_length, rb->args->alarm.data.user_tag
		);

	ACP_ASSERT (written == (LSA_UINT16)(dp - dp_start));
	return written;
}

#if ACP_FRAG_MODE
/*----------------------------------------------------------------------------*/
LSA_UINT16
acp_frag_build (
	ACP_CHANNEL_PTR  channel,
	ACP_LOCAL_ALARM_CONST_PTR  alarm,
	EDD_UPPER_MEM_U8_PTR_TYPE  dp
	)
{
	ACP_UPPER_RQB_PTR_TYPE rb;
	ACP_FRAG_SEND_PTR_TYPE send_args;
	LSA_UINT32 offset;
	LSA_UINT16 max_payload_len;
	LSA_UINT16 payload_len;
	LSA_UINT16 block_len;
	LSA_UINT16 written = 0;

	LSA_UNUSED_ARG(channel);

	rb = AcpListIterFirst(&alarm->send_list[ACP_ASRT_PRIORITY_FRAG], ACP_UPPER_RQB_PTR_TYPE);
	send_args = &rb->args->alarm.frag_send;

	offset = alarm->rta_sm[ACP_ASRT_PRIORITY_FRAG].frag_offset;

	max_payload_len = EDD_IEEE_FRAME_LENGTH - (ACP_PDU_ETHERNET_HEADER_LENGTH + ACP_PDU_RTA_HEADER_LENGTH + ACP_PDU_FRAG_HEADER_LENGTH);

	if(max_payload_len > ACP_PDU_MAX_VAR_PART_LEN - ACP_PDU_FRAG_HEADER_LENGTH)
	{
		max_payload_len = ACP_PDU_MAX_VAR_PART_LEN - ACP_PDU_FRAG_HEADER_LENGTH;
	}

	/******************** ALARM DATA ********************/

	ACP_ASSERT(ACP_RQB_GET_OPCODE (rb) == ACP_OPC_FRAG_SEND);

	LSA_UINT16 cnt;
	LSA_UINT8* mem_ptr;

	if(send_args->data_length-offset > max_payload_len)
	{
		payload_len = max_payload_len;
	}
	else
	{
		payload_len = send_args->data_length-offset;
	}

	block_len = ACP_PDU_FRAG_HEADER_LENGTH + payload_len;

	ACP_ASSERT(ACP_PDU_FRAG_HEADER_LENGTH == 8);
	acp_dp_set_u32 (dp, send_args->data_length);
	acp_dp_set_u32 (dp, alarm->rta_sm[ACP_ASRT_PRIORITY_FRAG].frag_offset); //set frag_offset

	written += 8;

	mem_ptr = send_args->data + offset;

	for(cnt=0; cnt<payload_len; cnt++)
	{
		LSA_UINT8 curr;
		acp_dp_get_u8(curr,mem_ptr);
		acp_dp_set_u8(dp, curr);
		written ++;
	}

	mem_ptr = LSA_NULL;
	ACP_ASSERT(written == block_len);

	return payload_len;
}
#endif

/*----------------------------------------------------------------------------*/
LSA_VOID
acp_op_alarm_send (
	ACP_CHANNEL_PTR  channel,
	ACP_UPPER_RQB_PTR_TYPE  rb
	)
{
	ACP_LOCAL_ALARM_PTR  alarm;
	LSA_UINT16  prio;

	alarm = acp_alarm_find (channel, rb->args->alarm.data.alarm_ref, LSA_TRUE);
	if (acp_is_null (alarm)) {
		acp_callback_async (channel, ACP_ERR_PARAM, & rb);
		return;
	}
	if (alarm->state != ACP_ALARM_ST_OPEN) {
		if (alarm->state == ACP_ALARM_ST_ERROR) {
			ACP_RTA_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_NOTE
				, "state(%u/_ST_ERROR) != _ST_OPEN", alarm->state
				);
			acp_callback_async (channel, ACP_ERR_ASRT, & rb);
		}
		else {
			ACP_RTA_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
				, "state(%u) != _ST_OPEN", alarm->state
				);
			acp_callback_async (channel, ACP_ERR_SEQUENCE, & rb);
		}
		return;
	}

		/* param check */
	if (EDD_IEEE_FRAME_LENGTH < ACP_RTA_MIN_SEND_LEN + ACP_ALARM_REQUEST_SIZE + rb->args->alarm.data.user_length) {
		ACP_RTA_TRACE_04 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
			, "max(%u) < min(%u) + al_req_block(%u) + usr_len(%u)"
			, EDD_IEEE_FRAME_LENGTH, ACP_RTA_MIN_SEND_LEN, ACP_ALARM_REQUEST_SIZE
			, rb->args->alarm.data.user_length
			);
		acp_callback_async (channel, ACP_ERR_PARAM, & rb);
		return;
	}

	if (rb->args->alarm.data.user_length > 0) {
		if (acp_is_null (rb->args->alarm.data.user_data)
		||	((rb->args->alarm.data.user_max_length < rb->args->alarm.data.user_length + 0/*ACP_ALARM_USER_DATA_OFFSET*/)))
		{
			ACP_RTA_TRACE_04 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
				, "user_max_length(%u) user_length(%u) user_data(%08x) offset(%u)"
				, rb->args->alarm.data.user_max_length, rb->args->alarm.data.user_length
				, rb->args->alarm.data.user_data, ACP_ALARM_USER_DATA_OFFSET
				);
			acp_callback_async (channel, ACP_ERR_PARAM, & rb);
			return;
		}
	}

	prio = rb->args->alarm.data.priority;

	if (ACP_ALARM_PRIORITY_LOW != prio  &&  ACP_ALARM_PRIORITY_HIGH != prio) {
		ACP_RTA_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
			, "wrong priority(%u)"
			, prio
			);
		acp_callback_async (channel, ACP_ERR_PARAM, & rb);
		return;
	}

	/* add to send-list, if it is the n. request the sending process is running */
	if (AcpListIsEmpty((ACP_LIST_ENTRY_PTR_TYPE)&alarm->send_list[prio])) {
		ACP_RTA_PTR  rta;

		AcpListInsertTailT(&alarm->send_list[prio], rb);

		ACP_RTA_TRACE_02 (channel->trace_idx, LSA_TRACE_LEVEL_CHAT
			, "aref(%u/%u) start sm"
			, alarm->alarm_ref, prio
			);

		rta = acp_rta_from_alarm (alarm);
		ACP_ASSERT (acp_is_not_null (rta));

		acp_rta_send (channel, rta, prio);
	}
	else {
		ACP_RTA_TRACE_03 (channel->trace_idx, LSA_TRACE_LEVEL_CHAT
			, "aref(%u/%u) queued(%08x)"
			, alarm->alarm_ref, prio, rb
			);
		AcpListInsertTailT(&alarm->send_list[prio], rb);
	}
}


/*----------------------------------------------------------------------------*/
LSA_VOID
acp_rta_send_done (
	ACP_CHANNEL_CONST_PTR  channel,
	ACP_RTA_PTR  rta,
	LSA_UINT16  prio,
	LSA_UINT16  rsp,
	LSA_UINT32  pnio_status
	)
{
	ACP_LOCAL_ALARM_PTR  alarm;
	ACP_UPPER_RQB_PTR_TYPE  rb;

	alarm = acp_alarm_from_rta (rta);

	if(AcpListIsEmpty(&alarm->send_list[prio])) {
		ACP_RTA_TRACE_02 (channel->trace_idx, LSA_TRACE_LEVEL_UNEXP
			, "canceled in the meantime, aref(%u/%u)"
			, rta->alarm_ref, prio
			);
		return;
	}
	else {
		AcpListRemoveHead(&alarm->send_list[prio], rb, ACP_UPPER_RQB_PTR_TYPE);
	}

	rb->args->alarm.data.pnio_status = pnio_status;

	if (rsp == ACP_OK) {
		acp_callback_async (channel, ACP_OK, & rb);
	}
	else if (rsp == ACP_OK_CANCEL) {
		acp_callback_async (channel, ACP_OK_CANCEL, & rb);
	}
	else {
		alarm->state = ACP_ALARM_ST_ERROR;
		ACP_RTA_TRACE_04 (channel->trace_idx, LSA_TRACE_LEVEL_WARN
			, "h(%u): send aSRT-Error: new state(%u) rsp(%u) pnio_status(0x%08x)"
			, channel->my_handle, alarm->state, rsp, pnio_status
			);
		acp_callback_async (channel, ACP_ERR_ASRT, & rb);
		return;
	}
}


/*-Opcode-----------------------------------------------------------------------
//	func: acp_op_alarm_ack_send
//----------------------------------------------------------------------------*/
LSA_VOID
acp_op_alarm_ack_send (
	ACP_CHANNEL_PTR  channel,
	ACP_UPPER_RQB_PTR_TYPE  rb
	)
{
	rb->args->alarm.data.user_length = 0;
	acp_op_alarm_send (channel, rb);
}


/*-Opcode-----------------------------------------------------------------------
//	func: acp_op_alarm_provide
//----------------------------------------------------------------------------*/
LSA_VOID
acp_op_alarm_provide (
	ACP_CHANNEL_PTR  channel,
	ACP_UPPER_RQB_PTR_TYPE  rb
	)
{
	LSA_UINT16  prio;

	ACP_ASSERT (ACP_RQB_GET_OPCODE (rb) == ACP_OPC_ALARM_IND_RES_PROVIDE);

	prio = rb->args->alarm.data.priority;

	if (prio != ACP_ALARM_PRIORITY_HIGH  && prio != ACP_ALARM_PRIORITY_LOW) {
		ACP_RTA_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
			, "unknown priority(%u)"
			, prio
			);
		acp_callback_async (channel, ACP_ERR_PARAM, & rb);
		return;
	}
	if (rb->args->alarm.data.user_max_length == 0xFFFF) {
		ACP_RTA_TRACE_00 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
			, "user_max_length == 0xFFFF not allowed"
			);
		acp_callback_async (channel, ACP_ERR_PARAM, & rb);
		return;
	}
	if (rb->args->alarm.data.user_max_length > 0) {
		if (acp_is_null (rb->args->alarm.data.user_data)) {
			ACP_RTA_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
				, "user_max_length(%u), !user_data(null)"
				, rb->args->alarm.data.user_max_length
				);
			acp_callback_async (channel, ACP_ERR_PARAM, & rb);
			return;
		}
		if (rb->args->alarm.data.user_max_length < ACP_ALARM_USER_DATA_OFFSET) {
			ACP_RTA_TRACE_02 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
				, "user_max_length(%u) < ACP_ALARM_USER_DATA_OFFSET(%u)"
				, rb->args->alarm.data.user_max_length, ACP_ALARM_USER_DATA_OFFSET
				);
			acp_callback_async (channel, ACP_ERR_PARAM, & rb);
			return;
		}
	}
	ACP_RTA_TRACE_03 (channel->trace_idx, LSA_TRACE_LEVEL_CHAT
		, "upper(%08x) prio(%u) user_max_length(%u)"
		, rb, prio, rb->args->alarm.data.user_max_length
		);

	AcpListInsertTailT(&channel->alarm_provide[prio], rb);

	/*
	//	note:	lower RTA-ind will not queued in acp_rta_sm_recv_data(),
	//			so a simple provide is enough
	*/
}

#if ACP_FRAG_MODE
/*-Opcode-----------------------------------------------------------------------
//	func: acp_op_frag_send
//----------------------------------------------------------------------------*/
LSA_VOID
acp_op_frag_send (
	ACP_CHANNEL_PTR  channel,
	ACP_UPPER_RQB_PTR_TYPE  rb
	)
{
	ACP_FRAG_SEND_PTR_TYPE args;
	args = &rb->args->alarm.frag_send;

	/* ERR-CHECK PARAM */
	if(args->data == LSA_NULL && args->data_length != 0)
	{
		ACP_RTA_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
						, "NULL pointer for sending (%u) bytes"
						, args->data_length
						);
		acp_callback_async(channel,ACP_ERR_PARAM,&rb);
		return;
	}

	/* ERR-CHECK SEQUENCE */
	ACP_LOCAL_ALARM_PTR  alarm;
	alarm = acp_alarm_find (channel, args->alarm_ref, LSA_TRUE);
	if (acp_is_null (alarm)) {
		ACP_RTA_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
						, "Alarm with the reference(%u) couldn't be find."
						, args->alarm_ref
						);
		acp_callback_async (channel, ACP_ERR_SEQUENCE, & rb);
		return;
	}
	if (alarm->state != ACP_ALARM_ST_OPEN) {
		if (alarm->state == ACP_ALARM_ST_ERROR) {
			ACP_RTA_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_NOTE
						, "state(%u/_ST_ERROR) != _ST_OPEN", alarm->state
						);
			acp_callback_async (channel, ACP_ERR_ASRT, & rb);
		}
		else {
			ACP_RTA_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
						, "state(%u) != _ST_OPEN", alarm->state
						);
			acp_callback_async (channel, ACP_ERR_SEQUENCE, & rb);
		}
		return;
	}

	if (AcpListIsEmpty((ACP_LIST_ENTRY_PTR_TYPE)&alarm->send_list[ACP_ASRT_PRIORITY_FRAG])) {
			ACP_RTA_PTR  rta;

			AcpListInsertTailT(&alarm->send_list[ACP_ASRT_PRIORITY_FRAG], rb);

			ACP_RTA_TRACE_02 (channel->trace_idx, LSA_TRACE_LEVEL_CHAT
				, "aref(%u/%u) start sm"
				, alarm->alarm_ref, ACP_ASRT_PRIORITY_FRAG
				);

			rta = acp_rta_from_alarm (alarm);
			ACP_ASSERT (acp_is_not_null (rta));

			rta->rta_sm[ACP_ASRT_PRIORITY_FRAG].data_length = rb->args->alarm.frag_send.data_length;
			rta->rta_sm[ACP_ASRT_PRIORITY_FRAG].frag_offset = 0x0;

			acp_rta_send (channel, rta, ACP_ASRT_PRIORITY_FRAG);
		}
		else {
			ACP_RTA_PTR  rta;
			ACP_RTA_TRACE_03 (channel->trace_idx, LSA_TRACE_LEVEL_CHAT
				, "aref(%u/%u) queued(%08x)"
				, alarm->alarm_ref, ACP_ASRT_PRIORITY_FRAG, rb
				);

			rta = acp_rta_from_alarm (alarm);

			rta->rta_sm[ACP_ASRT_PRIORITY_FRAG].data_length = rb->args->alarm.frag_send.data_length;
			rta->rta_sm[ACP_ASRT_PRIORITY_FRAG].frag_offset = 0x0;
			ACP_ASSERT (acp_is_not_null (rta));

			AcpListInsertTailT(&alarm->send_list[ACP_ASRT_PRIORITY_FRAG], rb);
		}
}

/*-Opcode-----------------------------------------------------------------------
//	func: acp_op_frag_recv
//----------------------------------------------------------------------------*/
LSA_VOID
acp_op_frag_recv (
	ACP_CHANNEL_PTR  channel,
	ACP_UPPER_RQB_PTR_TYPE  rb
	)
{
	ACP_FRAG_RECV_PTR_TYPE args;
	args = &rb->args->alarm.frag_recv;

	/* ERR-CHECK PARAM */
	if(args->data == LSA_NULL && args->data_max_length != 0)
	{
		ACP_RTA_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
			, "NULL pointer for receiving maximum (%u) bytes"
			, args->data_max_length
			);
		acp_callback_async(channel,ACP_ERR_PARAM,&rb);
		return;
	}

	/* ERR-CHECK SEQUENCE */
	ACP_LOCAL_ALARM_PTR  alarm;
	alarm = acp_alarm_find (channel, args->alarm_ref, LSA_TRUE);
	if (acp_is_null (alarm))
	{
		ACP_RTA_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
			, "Alarm with the reference(%u) couldn't be find."
			, args->alarm_ref
			);
		acp_callback_async (channel, ACP_ERR_SEQUENCE, & rb);
		return;
	}
	if (alarm->state != ACP_ALARM_ST_OPEN)
	{
		if (alarm->state == ACP_ALARM_ST_ERROR)
		{
			ACP_RTA_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_NOTE
				, "state(%u/_ST_ERROR) != _ST_OPEN", alarm->state
				);
			acp_callback_async (channel, ACP_ERR_ASRT, & rb);
		}
		else
		{
			ACP_RTA_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_ERROR
				, "state(%u) != _ST_OPEN", alarm->state
				);
			acp_callback_async (channel, ACP_ERR_SEQUENCE, & rb);
		}
		return;
	}

	ACP_RTA_TRACE_02 (channel->trace_idx, LSA_TRACE_LEVEL_CHAT
		, "upper(%08x) user_max_length(%u)"
		, rb, rb->args->alarm.data.user_max_length
		);
	AcpListInsertTailT(&alarm->frag_provide_list, rb);
	/* note: nrt-recv is discarded if no upper resources are available, see acp_frag_recv_data() */
}

LSA_UINT16
acp_frag_indication_check_data (
	ACP_LOCAL_ALARM_CONST_PTR  alarm,
	ACP_UPPER_RQB_PTR_TYPE  upper,
	EDD_UPPER_MEM_U8_PTR_TYPE  dp,
	ACP_RTA_HEADER_CONST_PTR  hdr,
	ACP_RTA_SM_PTR  rtasm
	)
{
	LSA_UINT16 payload_length;
	LSA_UINT32 data_length, offset;
	LSA_UINT16 cnt;

	acp_dp_get_u32(data_length,dp);
	acp_dp_get_u32(offset,dp);

	if(rtasm->data_length == 0)
	{
		rtasm->data_length = data_length;
	}

	if(rtasm->data_length != data_length)
	{
		ACP_RTA_TRACE_02 (alarm->channel->trace_idx, LSA_TRACE_LEVEL_UNEXP
			, "different data_length transported (%u) than before (%u)"
			, data_length
			, rtasm->data_length
			);
		return ACP_ERR_LOWER_LAYER;
	}

	if(rtasm->upper_buffer_len < data_length)
	{
		ACP_RTA_TRACE_01 (alarm->channel->trace_idx, LSA_TRACE_LEVEL_UNEXP
			, "data_length to big for receiving buffer (%u)"
			, data_length
			);
		return ACP_ERR_LOWER_LAYER;
	}

	payload_length = hdr->var_part_len - ACP_PDU_FRAG_HEADER_LENGTH;

	if(offset + payload_length > ACP_CFG_MAX_FRAG_BUFFER_LENGTH)
	{
		ACP_RTA_TRACE_02 (alarm->channel->trace_idx, LSA_TRACE_LEVEL_UNEXP
			, "offset(%u) + payload length(%u) too big for receive buffer"
			, offset, data_length
			);
		return ACP_ERR_LOWER_LAYER;
	}

	if(offset + payload_length > data_length){
		ACP_RTA_TRACE_02 (alarm->channel->trace_idx, LSA_TRACE_LEVEL_UNEXP
			, "offset(%u) + payload length(%u) bigger than the whole data"
			, offset, data_length
			);
		return ACP_ERR_LOWER_LAYER;
	}

	for(cnt = 0; cnt<payload_length; cnt++)
	{
		acp_dp_get_u8(*(upper->args->alarm.frag_recv.data+offset),dp);
		offset ++;
	}
	rtasm->frag_offset = offset;

	return ACP_OK;

}

#endif

/*----------------------------------------------------------------------------*/
LSA_UINT16
acp_alarm_indication_set_data (
	ACP_LOCAL_ALARM_CONST_PTR  alarm,
	ACP_UPPER_RQB_PTR_TYPE  upper,
	EDD_UPPER_MEM_U8_PTR_TYPE  dp,
	LSA_UINT16  dp_len,
	LSA_UINT16  prio
	)
{
#if ACP_CFG_TRACE_MODE == 2 /* LSA index trace */
	ACP_CHANNEL_PTR  channel = alarm->channel;
#endif
	EDD_UPPER_MEM_U8_PTR_TYPE  dp_pdu;
	LSA_UINT16  block_type;
	LSA_UINT16  block_length;
	LSA_UINT16  block_version;
	LSA_UINT16  cpy_len;

	upper->args->alarm.data.alarm_ref	= alarm->alarm_ref;
	upper->args->alarm.data.usr_id		= alarm->usr_id;
	upper->args->alarm.data.priority	= prio;
	upper->args->alarm.data.pnio_status	= ACP_PNIO_STATUS_OK;

	if (dp_len < ACP_ALARM_ACK_SIZE) {
		ACP_RTA_TRACE_02 (channel->trace_idx, LSA_TRACE_LEVEL_UNEXP
			, "dp_len(%u) < alarm_ack_size(%u)"
			, dp_len, ACP_ALARM_ACK_SIZE
			);
		return ACP_ERR_LOWER_LAYER;
	}

		/* keep for copy to user_data */
	dp_pdu = dp;

		/* header type */
	acp_dp_get_u16 (block_type, dp);

	if (prio == ACP_ALARM_PRIORITY_LOW  &&  block_type == ACP_ALARM_BLOCK_ACK_LOW) {
		ACP_RQB_SET_OPCODE (upper, ACP_OPC_ALARM_ACK_INDICATION);
	}
	else if (prio == ACP_ALARM_PRIORITY_HIGH  &&  block_type == ACP_ALARM_BLOCK_ACK_HIGH) {
		ACP_RQB_SET_OPCODE (upper, ACP_OPC_ALARM_ACK_INDICATION);
	}
	else if (prio == ACP_ALARM_PRIORITY_LOW  &&  block_type == ACP_ALARM_BLOCK_REQUEST_LOW) {
		ACP_RQB_SET_OPCODE (upper, ACP_OPC_ALARM_INDICATION);
	}
	else if (prio == ACP_ALARM_PRIORITY_HIGH  &&  block_type == ACP_ALARM_BLOCK_REQUEST_HIGH) {
		ACP_RQB_SET_OPCODE (upper, ACP_OPC_ALARM_INDICATION);
	}
	else {
		ACP_RTA_TRACE_02 (channel->trace_idx, LSA_TRACE_LEVEL_UNEXP
			, "prio(%u) && block_type(0x%04x) unknown"
			, prio, block_type
			);
		return ACP_ERR_LOWER_LAYER;
	}

		/* header data */
	acp_dp_get_u16 (block_length, dp);
	acp_dp_get_u16 (block_version, dp);

	if (block_version != ACP_ALARM_BLOCK_VERSION) {
		ACP_RTA_TRACE_01 (channel->trace_idx, LSA_TRACE_LEVEL_UNEXP
			, "!block_version(0x%04x)"
			, block_version
			);
		return ACP_ERR_LOWER_LAYER;
	}
	if (block_length != dp_len - 2*sizeof(LSA_UINT16)) {
		ACP_RTA_TRACE_03 (channel->trace_idx, LSA_TRACE_LEVEL_UNEXP
			, "block_length(%u) != dp_len(%u) - (%u)"
			, block_length, dp_len, (2*sizeof(LSA_UINT16))
			);
		return ACP_ERR_LOWER_LAYER;
	}

		/* common data */
	acp_dp_get_u16 (upper->args->alarm.data.alarm_type, dp);
	acp_dp_get_u32 (upper->args->alarm.data.api, dp);
	acp_dp_get_u16 (upper->args->alarm.data.slot_nr, dp);
	acp_dp_get_u16 (upper->args->alarm.data.subslot_nr, dp);

		/* if alarm-ack */
	if (ACP_RQB_GET_OPCODE (upper) == ACP_OPC_ALARM_ACK_INDICATION) {
		acp_dp_get_u16 (upper->args->alarm.data.alarm_specifier, dp);
		acp_dp_get_u32 (upper->args->alarm.data.pnio_status, dp);
		return ACP_OK;
	}

		/* else alarm */
	if (dp_len < ACP_ALARM_REQUEST_SIZE) {
		ACP_RTA_TRACE_02 (channel->trace_idx, LSA_TRACE_LEVEL_UNEXP
			, "dp_len(%u) < req(%u)"
			, dp_len, ACP_ALARM_REQUEST_SIZE
			);
		return ACP_ERR_LOWER_LAYER;
	}

	acp_dp_get_u32 (upper->args->alarm.data.module_ident, dp);
	acp_dp_get_u32 (upper->args->alarm.data.submodule_ident, dp);
	acp_dp_get_u16 (upper->args->alarm.data.alarm_specifier, dp);

		/* optional part */
	upper->args->alarm.data.user_tag = 0;
	upper->args->alarm.data.user_length = 0;

		/* copy AlarmNotification-PDU to user_data */
	cpy_len = dp_len;
	if (upper->args->alarm.data.user_max_length >= cpy_len) {
		ACP_ASSERT (upper->args->alarm.data.user_data);
		ACP_MEMCPY (upper->args->alarm.data.user_data, dp_pdu, cpy_len);

		if (cpy_len > ACP_ALARM_USER_DATA_OFFSET) {
			upper->args->alarm.data.user_length = (LSA_UINT16)(cpy_len - ACP_ALARM_USER_DATA_OFFSET);
		}
	}
	else {
		upper->args->alarm.data.user_length = 0xFFFF;

		ACP_RTA_TRACE_02 (channel->trace_idx, LSA_TRACE_LEVEL_UNEXP
			, "too little user_data: max(%u) need(%u)"
			, upper->args->alarm.data.user_max_length, cpy_len
			);
	}

		/* user_tag */
	cpy_len = (LSA_UINT16)(dp_len - ACP_ALARM_REQUEST_SIZE);
	if (cpy_len >= sizeof(LSA_UINT16)) {

		acp_dp_get_u16 (upper->args->alarm.data.user_tag, dp);
		cpy_len -= sizeof(LSA_UINT16);

		if (cpy_len +  ACP_ALARM_USER_DATA_OFFSET > upper->args->alarm.data.user_max_length) {
			ACP_RTA_TRACE_02 (channel->trace_idx, LSA_TRACE_LEVEL_UNEXP
				, "cpy_len(%u) > alloc_len(%u), truncated"
				, cpy_len, upper->args->alarm.data.user_max_length
				);
			upper->args->alarm.data.user_length = 0xFFFF;
			cpy_len = upper->args->alarm.data.user_max_length;
		}
		else {
			ACP_ASSERT (upper->args->alarm.data.user_length == cpy_len);
		}
	}

	return ACP_OK;
}


/*------------------------------------------------------------------------------
//	callback from acp_rta_sm
//----------------------------------------------------------------------------*/
LSA_VOID
acp_rta_indication (
	ACP_CHANNEL_CONST_PTR  channel,
	ACP_RTA_PTR  rta,
	LSA_UINT16  prio,
	LSA_UINT16  rsp,
	ACP_UPPER_RQB_PTR_TYPE * upper
	)
{
	ACP_LOCAL_ALARM_PTR  alarm;

	LSA_UNUSED_ARG (prio);

	alarm = acp_alarm_from_rta (rta);

	ACP_ASSERT (acp_is_not_null (upper));
	ACP_ASSERT (acp_is_not_null (*upper));
	ACP_ASSERT (ACP_RQB_GET_OPCODE(*upper) == ACP_OPC_ALARM_INDICATION
			|| ACP_RQB_GET_OPCODE(*upper) == ACP_OPC_ALARM_ACK_INDICATION
		);

	if (rsp == ACP_OK) {
		ACP_RTA_TRACE_07 (channel->trace_idx, LSA_TRACE_LEVEL_NOTE_HIGH
			, "h(%u): aref(%u/%u) state(%u) opcode(%u) rsp(%u) upper(%08x)"
			, channel->my_handle, alarm->alarm_ref, prio, alarm->state
			, ACP_RQB_GET_OPCODE (*upper), rsp, *upper
			);
	}
	else {
		alarm->state = ACP_ALARM_ST_ERROR;

		ACP_RTA_TRACE_07 (channel->trace_idx, LSA_TRACE_LEVEL_WARN
			, "h(%u): aref(%u/%u) state(%u) opcode(%u) rsp(%u) upper(%08x)"
			, channel->my_handle, alarm->alarm_ref, prio, alarm->state
			, ACP_RQB_GET_OPCODE (*upper), rsp, *upper
			);
	}

	acp_callback_async (channel, rsp, upper);
}


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
