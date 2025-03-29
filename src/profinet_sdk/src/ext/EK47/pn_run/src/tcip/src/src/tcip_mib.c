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
/*  F i l e               &F: tcip_mib.c                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  MIB integration                                                          */
/*                                                                           */
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID  7
#define TCIP_MODULE_ID     7

#include "tcip_int.h"
#include "anyip_tcip_interface.h"
#include "anyip_tcip.h"
#include "anyip_edd.h"

TCIP_FILE_SYSTEM_EXTENSION(TCIP_MODULE_ID) /* no semicolon */

/*===========================================================================*/
/*===========================================================================*/

/*
 * function:       returns interface index
 * documentation:  LSA_TCIP_Detailspec.doc
 */
LSA_UINT8
tcip_get_mib2_ifindex(
    LSA_UINT32  interface_id,
	LSA_UINT16	port_id,
	LSA_UINT32* IfIndex
)
{
	int i;
	LSA_UINT8 ret = TCIP_ERR_PARAM;

	/*
	 * find channel with matching interface_id
	 */
	for( i=0; i<TCIP_MAX_CHANNELS; i++ )
	{
		TCIP_CHANNEL_PTR_TYPE ch = &tcip_data.tcip_channels[i];
		TCIP_IF_DATA_PTR_TYPE p_if_data;

		if( TCIP_CH_OPEN != ch->state )
		{
			continue;
		}

		TCIP_ASSERT(is_not_null(ch->p_if_data));
		p_if_data = ch->p_if_data;

		/*
		 *  look for matching interface id
		 */
		if( interface_id == p_if_data->edd_interface_id )
		{
			int if_index = -1;

			if( port_id > p_if_data->port_count )
			{
				ret = TCIP_ERR_PARAM;
				break;
			}

			if_index = anyip_tcip_get_if_index(p_if_data->port[port_id].ifp);
			if (if_index >= 0)
			{
				*IfIndex = (LSA_UINT32)if_index;
			}
			else
			{
				ret = TCIP_ERR_PARAM;
				break;
			}

			ret = TCIP_OK;

			break;
		}
	}

	return ret;
}

/*===========================================================================*/
/*===========================================================================*/

/*
 *   sets interface description string
 */
LSA_UINT8
tcip_set_mib2_ifgroup_data(
	LSA_UINT32 interface_id,
	LSA_UINT16 port_id,
	LSA_UINT8* p_ifDescr,
	LSA_UINT16 ifDescr_length
)
{
	TCIP_LOG_FCT("tcip_set_mib2_ifgroup_data")
	int i;
	LSA_UINT8 ret = TCIP_ERR_PARAM;

	if( is_null(p_ifDescr) || ifDescr_length > 255 )
	{
		TCIP_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_ERROR, "passed interface description string longer than 255 characters (%d)", ifDescr_length);
		return TCIP_ERR_PARAM;
	}

	/*
	 *  find interface with matching interface id
	 */
	for( i=0; i<TCIP_MAX_CHANNELS; i++ )
	{
		TCIP_CHANNEL_PTR_TYPE ch;
		TCIP_IF_DATA_PTR_TYPE p_if_data;

		ch = &tcip_data.tcip_channels[i];

		if( TCIP_CH_OPEN != ch->state )
		{
			continue;
		}

		TCIP_ASSERT(is_not_null(ch->p_if_data));
		p_if_data = ch->p_if_data;

		/*
		 *  look for matching interface id
		 */
		if( interface_id == p_if_data->edd_interface_id )
		{
			ANYIP_IFNET_PTR ifp = p_if_data->port[port_id].ifp;

			if( port_id > p_if_data->port_count )
			{
				ret = TCIP_ERR_PARAM;
				break;
			}

			anyip_tcip_set_if_description(ifp, (char *)p_ifDescr, ifDescr_length); /* also sets MIB II ifDescr */

			ret = TCIP_OK;
			break;
		}
	}

	return ret;
}

/*===========================================================================*/
/*===========================================================================*/

/*
 *   sets parameters of mib2 system group
 */
LSA_UINT8
tcip_set_mib2_sysgroup_data(
	LSA_UINT8 const* p_sysName,
	LSA_UINT16 sysName_length,
	LSA_UINT8 const* p_sysDescr,
	LSA_UINT16 sysDescr_length,
	LSA_UINT8 const* p_sysContact,
	LSA_UINT16 sysContact_length,
	LSA_UINT8 const* p_sysLocation,
	LSA_UINT16 sysLocation_length
)
{
	TCIP_LOG_FCT("tcip_set_mib2_sysgroup_data")

	if ( is_not_null(p_sysName) && sysName_length > 255 )
	{
		TCIP_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_ERROR, "passed sysName string longer than 255 characters (%d)", sysName_length);
		return TCIP_ERR_PARAM;
	}

	if ( is_not_null(p_sysDescr) && sysDescr_length > 255 )
	{
		TCIP_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_ERROR, "passed sysDescr string longer than 255 characters (%d)", sysDescr_length);
		return TCIP_ERR_PARAM;
	}

	if ( is_not_null(p_sysContact) && sysContact_length > 255 )
	{
		TCIP_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_ERROR, "passed sysContact string longer than 255 characters (%d)", sysContact_length);
		return TCIP_ERR_PARAM;
	}

	if ( is_not_null(p_sysLocation) && sysLocation_length > 255 )
	{
		TCIP_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_ERROR, "passed sysLocation string longer than 255 characters (%d)", sysLocation_length);
		return TCIP_ERR_PARAM;
	}

#if (TCIP_CFG_SNMP_ON)
	if ( is_not_null(p_sysName)) /* sado -- omit comparison of length >= 0; Greenhills compiler warning - useless comparision of unsigned integer */
	{
		anyip_tcip_set_mib2_sysName((const char *)p_sysName, sysName_length);
	}

	if ( is_not_null(p_sysDescr))
	{
		anyip_tcip_set_mib2_sysDescr((const char *)p_sysDescr, sysDescr_length);
	}

	if ( is_not_null(p_sysContact))
	{
		anyip_tcip_set_mib2_sysContact((const char *)p_sysContact, sysContact_length);
	}

	if ( is_not_null(p_sysLocation))
	{
		anyip_tcip_set_mib2_sysLocation((const char *)p_sysLocation, sysLocation_length);
	}
#endif

	return TCIP_OK;
}


/*===========================================================================*/
/*===========================================================================*/

void tcip_mib2_written(const void * snmp_data)
{
#if TCIP_CFG_SNMP_ON
	if( snmp_data != 0 )
	{
		char *s_data = (char *)snmp_data;
		if(s_data == anyip_tcip_get_mib2_sysDescr())
		{
			/* unreachable, not writable via SNMP */
		}
		else if(s_data == anyip_tcip_get_mib2_sysName())
		{
			TCIP_MIB2_SYSGROUP_DATA_WRITTEN(s_data, LSA_NULL, LSA_NULL);
		}
		else if(s_data == anyip_tcip_get_mib2_sysContact())
		{
			TCIP_MIB2_SYSGROUP_DATA_WRITTEN(LSA_NULL, s_data, LSA_NULL);
		}
		else if(s_data == anyip_tcip_get_mib2_sysLocation())
		{
			TCIP_MIB2_SYSGROUP_DATA_WRITTEN(LSA_NULL, LSA_NULL, s_data);
		}
		else
		{
			/* do nothing */
		}
	}
#else
	LSA_UNUSED_ARG(snmp_data);
#endif
}


/*===========================================================================*/
/*===========================================================================*/
#if TCIP_CFG_SNMP_ON
unsigned char
tcip_mib2_statistics_update(
	void
) {
	TCIP_LOG_FCT("tcip_mib2_statistics_update")

	if( tcip_data.get_statistics_pending != 0 )
	{
		TCIP_FATAL();
	}
	else
	{
		int i;

		for( i=0; i<TCIP_MAX_CHANNELS; i++ )
		{
			TCIP_CHANNEL_PTR_TYPE channel = &tcip_data.tcip_channels[i];

			if( channel->state == TCIP_CH_OPEN )
			{
				if( channel->detailptr->path_type == TCIP_PATH_TYPE_ARP )
				{
					if( channel->edd.get_statistics_pending )
					{
						TCIP_FATAL();
					}
					else
					{
						TCIP_IF_DATA_PTR_TYPE p_if_data = channel->p_if_data;
						LSA_UINT16 port_id;
						TCIP_EDD_LOWER_RQB_PTR_TYPE pEdd;
						EDD_UPPER_GET_STATISTICS_ALL_PTR_TYPE pStat;

						pEdd = channel->edd.pEdd;

						pStat = TCIP_EDD_GET_PPARAM(pEdd, EDD_UPPER_GET_STATISTICS_ALL_PTR_TYPE);

						pStat->Reset = EDD_STATISTICS_DO_NOT_RESET_VALUES; /* AP00913629 do not reset counters */
						for (port_id = 0; port_id <= p_if_data->port_count; port_id++) /* RQ 1916135 -- new EDD Interface */
						{
							pStat->sMIB[port_id].RequestedCounters = 0;
						}

						TCIP_EDD_SET_OPCODE(pEdd, EDD_OPC_REQUEST, EDD_SRV_GET_STATISTICS_ALL );
						TCIP_EDD_SET_HANDLE(pEdd, 0);
						EDD_RQB_SET_HANDLE_LOWER(pEdd, channel->edd.handle_lower);

						TCIP_UPPER_TRACE_02(0, LSA_TRACE_LEVEL_NOTE,
							">>> EDD_SRV_GET_STATISTICS (EDDHandle: 0x%X,pEdd: 0x%X)"
							, TCIP_EDD_GET_HANDLE(pEdd), pEdd
							);

						TCIP_EDD_REQUEST_LOWER(pEdd, channel->sysptr);

						/***/

						channel->edd.get_statistics_pending = LSA_TRUE;

						tcip_data.get_statistics_pending ++;
					}
				}
			}
		}
	}

	/***/

	if( tcip_data.get_statistics_pending != 0 )
	{
		return ANYIP_TCIP_STATISTICS_PENDING_YES;
	}

	return ANYIP_TCIP_STATISTICS_PENDING_NO;
}
#endif
/*===========================================================================*/
#if TCIP_CFG_SNMP_ON
LSA_VOID
tcip_edd_get_mib2_statistics_done(
	TCIP_CHANNEL_PTR_TYPE channel,
	TCIP_EDD_LOWER_RQB_CONST_PTR_TYPE pRQB
) {
	if(
		TCIP_EDD_GET_RESPONSE(pRQB) != EDD_STS_OK
		|| ! channel->edd.get_statistics_pending )
	{
		TCIP_FATAL();
	}
	else
	{
		TCIP_IF_DATA_PTR_TYPE p_if_data = channel->p_if_data;
		EDD_UPPER_GET_STATISTICS_ALL_PTR_TYPE pStat = TCIP_EDD_GET_PPARAM(pRQB, EDD_UPPER_GET_STATISTICS_ALL_PTR_TYPE);
		LSA_UINT16 port_id;

		for( port_id = 0/*!*/; port_id <= p_if_data->port_count; port_id ++ )
		{
			ANYIP_IFNET_PTR ifp = p_if_data->port[port_id].ifp;
			EDD_UPPER_GET_STATISTICS_MIB_PTR_TYPE pVal = &pStat->sMIB[port_id];

			anyip_edd_set_statistic_counters(ifp,
									(long)pVal->InOctets,         /* 10: EDD_MIB_SUPPORT_INOCTETS */
									(long)pVal->InUcastPkts,      /* 11: EDD_MIB_SUPPORT_INUCASTPKTS */
									(long)pVal->InNUcastPkts,     /* 12: EDD_MIB_SUPPORT_INNUCASTPKTS */
									(long)pVal->InDiscards,       /* 13: EDD_MIB_SUPPORT_INDISCARDS */
									(long)pVal->InErrors,         /* 14: EDD_MIB_SUPPORT_INERRORS */
									(long)pVal->InUnknownProtos,  /* 15: EDD_MIB_SUPPORT_INUNKNOWNPROTOS */
									(long)pVal->OutOctets,        /* 16: EDD_MIB_SUPPORT_OUTOCTETS */
									(long)pVal->OutUcastPkts,     /* 17: EDD_MIB_SUPPORT_OUTUCASTPKTS */
									(long)pVal->OutNUcastPkts,    /* 18: EDD_MIB_SUPPORT_OUTNUCASTPKTS */
									(long)pVal->OutDiscards,      /* 19: EDD_MIB_SUPPORT_OUTDISCARDS */
									(long)pVal->OutErrors,        /* 20: EDD_MIB_SUPPORT_OUTERRORS */
									(long)pVal->OutQLen,          /* 21: EDD_MIB_SUPPORT_OUTQLEN */
									(unsigned long)pVal->SupportedCounters);
		}

		/***/
		channel->edd.get_statistics_pending = LSA_FALSE;

		/*
		 *  close channel now
		 */
		if( channel->state == TCIP_CH_CLOSING )
		{
			tcip_edd_close_channel(channel);
		}

		if( tcip_data.get_statistics_pending < 1 )
		{
			TCIP_FATAL(); /* a bug */
		}
		else
		{
			if (tcip_data.get_statistics_pending > 0)
			{
				tcip_data.get_statistics_pending --;
			}
			else
			{
				TCIP_FATAL();
			}

			if( tcip_data.get_statistics_pending == 0 )
			{
#if (TCIP_CFG_INTERNICHE) || (TCIP_CFG_OPEN_BSD) || (TCIP_CFG_USE_OBSD_SNMPD)
				TCIP_MIB2_STATISTICS_UPDATE_DONE();
#else
				anyip_mib2_statistics_update_done();
#endif
			}
		}
	}
}
#endif
/*===========================================================================*/
/*===========================================================================*/

LSA_VOID
tcip_edd_ext_link_status_provide(
	TCIP_CONST_CHANNEL_PTR_TYPE channel
) {
	TCIP_LOG_FCT("tcip_edd_ext_link_status_provide")
	LSA_UINT16 port_id;

	for ( port_id = 1; port_id <= channel->p_if_data->port_count; port_id++ )
	{
		TCIP_EDD_LOWER_RQB_PTR_TYPE pRQB;
		EDD_UPPER_LINK_STATUS_IND_PROVIDE_PTR_TYPE pLink;

		pRQB = tcip_AllocRQB(channel->sysptr, sizeof(EDD_RQB_LINK_STATUS_IND_PROVIDE_TYPE));

		if ( is_null(pRQB) )
		{
			TCIP_FATAL(); /* a bug */
		}

		pLink = TCIP_EDD_GET_PPARAM(pRQB, EDD_UPPER_LINK_STATUS_IND_PROVIDE_PTR_TYPE);

		TCIP_MEMSET(pLink, 0, sizeof(*pLink)); /* note: actually not necessery */

		pLink->PortID = port_id;
		pLink->Status = EDD_LINK_UNKNOWN; /* force an indication (the status in EDD is either UP or DOWN, never unknown) */
		pLink->PhyStatus = EDD_PHY_STATUS_UNKNOWN;

		TCIP_EDD_SET_OPCODE(pRQB, EDD_OPC_REQUEST, EDD_SRV_LINK_STATUS_IND_PROVIDE_EXT);
		TCIP_EDD_SET_HANDLE(pRQB, 0);
		EDD_RQB_SET_HANDLE_LOWER(pRQB, channel->edd.handle_lower);

		TCIP_UPPER_TRACE_02(0, LSA_TRACE_LEVEL_NOTE,
			">>> EDD_SRV_LINK_STATUS_IND_PROVIDE_EXT (EDDHandle: 0x%X,pRQB: 0x%X)"
			, TCIP_EDD_GET_HANDLE(pRQB), pRQB
			);

		TCIP_EDD_REQUEST_LOWER(pRQB, channel->sysptr);
	}
}

/*===========================================================================*/

LSA_VOID
tcip_edd_ext_link_status_indication(
	TCIP_CONST_CHANNEL_PTR_TYPE channel,
	TCIP_EDD_LOWER_RQB_PTR_TYPE pRQB
) {
	TCIP_LOG_FCT("tcip_edd_ext_link_status_indication")
	EDD_UPPER_LINK_STATUS_IND_PROVIDE_PTR_TYPE pLink = TCIP_EDD_GET_PPARAM(pRQB, EDD_UPPER_LINK_STATUS_IND_PROVIDE_PTR_TYPE);

	TCIP_UPPER_TRACE_05(channel->trace_idx, LSA_TRACE_LEVEL_NOTE,
		">>> pRQB: 0x%X, PortID: %u, St/Sp/M: %u/%u/%u"
		, pRQB, pLink->PortID
		, pLink->Status, pLink->Speed, pLink->Mode
		);

	if( channel->state != TCIP_CH_OPEN )
	{
		tcip_FreeRQB(channel->sysptr, pRQB );
	}
	else if( TCIP_EDD_GET_RESPONSE(pRQB) != EDD_STS_OK )
	{
		TCIP_FATAL();
	}
	else if( pLink->PortID < 1 || pLink->PortID > channel->p_if_data->port_count )
	{
		TCIP_FATAL();
	}
	else /* ok */
	{
		{
			ANYIP_IFNET_PTR ifp = channel->p_if_data->port[pLink->PortID].ifp;

			if (pLink->PhyStatus == EDD_PHY_STATUS_DISABLED) /* evaluate PhyStatus */
			{
				anyip_tcip_set_admin_state(ifp, ANYIP_STATE_DOWN); /* DOWN, see 1199575 */
				anyip_tcip_set_link_state(ifp, ANYIP_STATE_DOWN);
			}
			else
			{
				switch (pLink->Status)
				{
				case EDD_LINK_UP:
				case EDD_LINK_UP_CLOSED:
				case EDD_LINK_UP_DISABLED:

					anyip_tcip_set_link_state(ifp, ANYIP_STATE_UP);

					switch (pLink->Speed)
					{
					case EDD_LINK_SPEED_10:
						anyip_tcip_set_baudrate(ifp, 10000000);
						break;
					case EDD_LINK_SPEED_100:
						anyip_tcip_set_baudrate(ifp, 100000000);
						break;
					case EDD_LINK_SPEED_1000:
						anyip_tcip_set_baudrate(ifp, 1000000000);
						break;
					case EDD_LINK_SPEED_10000: //TCIP_IMPROVE: was soll man hier melden, ifHighSpeed gibt es nicht
						anyip_tcip_set_baudrate(ifp, 0xFFFFFFFF); /* MIB: use ifHighSpeed */
						break;
					default:
						anyip_tcip_set_baudrate(ifp, 0);
						break;
					}
					break;

				default: /* EDD_LINK_DOWN */
					anyip_tcip_set_link_state(ifp, ANYIP_STATE_DOWN);

					anyip_tcip_set_baudrate(ifp, (long)channel->p_if_data->port[pLink->PortID].nominal_bandwidth);
					break;
				}

				anyip_tcip_set_admin_state(ifp, ANYIP_STATE_UP); /* UP, see 1199575 */
			}
		}
		/* reprovide */

		TCIP_EDD_SET_HANDLE(pRQB, 0);
		EDD_RQB_SET_HANDLE_LOWER(pRQB, channel->edd.handle_lower);

		TCIP_EDD_REQUEST_LOWER(pRQB, channel->sysptr);
	}
}

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
