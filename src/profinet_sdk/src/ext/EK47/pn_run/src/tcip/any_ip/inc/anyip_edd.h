#ifndef __ANYIP_EDD_H__
#define __ANYIP_EDD_H__

/**************************************************************************//**
@file		anyip_edd.h
@brief		This file contains functions for used for interaction with the EDD
			which are not send or receive.
******************************************************************************/

#include "anyip_inc.h"

/**************************************************************************//**
@ingroup	anyip_edd_interface
@brief		Function to set MIB-2 statistic counters
@details	This function sets the given parameters to the corresponding
			IP-stack specific MIB-2 variables. The function is used to update
			the IP-stack specific MIB-2 statistic counters with the current
			EDD values.

@param[in]	ifp:	pointer to the network interface, which statistic counters
					should be updated
@param[in]	inOctets:	value of MIB-2 variable inOctets
@param[in]	inUcastPkts:	value of MIB-2 variable inUcastPkts
@param[in]	inNUcastPkts:	value of MIB-2 variable inNUcastPkts
@param[in]	inDiscards:	value of MIB-2 variable inDiscards
@param[in]	inErrors:	value of MIB-2 variable inErrors
@param[in]	inUnknownProtos:	value of MIB-2 variable inUnknownProtos
@param[in]	outOctets:	value of MIB-2 variable outOctets
@param[in]	outUcastPkts:	value of MIB-2 variable outUcastPkts
@param[in]	outNUcastPkts:	value of MIB-2 variable outNUcastPkts
@param[in]	outDiscards:	value of MIB-2 variable outDiscards
@param[in]	outErrors:	value of MIB-2 variable outErrors
@param[in]	outQLen:	value of MIB-2 variable outQLen
@param[in]	supportedCounters: bitmask defining supported MIB-2 counters
******************************************************************************/
void anyip_edd_set_statistic_counters(ANYIP_IFNET_PTR ifp,
	long inOctets,
	long inUcastPkts,
	long inNUcastPkts,
	long inDiscards,
	long inErrors,
	long inUnknownProtos,
	long outOctets,
	long outUcastPkts,
	long outNUcastPkts,
	long outDiscards,
	long outErrors,
	long outQLen,
	unsigned long supportedCounters);

/**************************************************************************//**
@ingroup	anyip_edd_interface
@brief		Function that is called, for an update of the MIB-2
			statistic counters.

@details	This function is the trigger for an update of the MIB-2 statistic
			counters from EDDx.

@note		This function should be used before an SNMP request is processed
			in order to update the MIB-2 statistic counters.
@note		This function is declared here, but implemented in tcip_mib.c if
			TCIP_CFG_USE_SNMPD is defined 1. No further implmentation is necessary.

@return		ANYIP_TCIP_STATISTICS_PENDING_YES, if an update request is currently
			processed
@return		ANYIP_TCIP_STATISTICS_PENDING_NO, if no update request is currently
			processed
******************************************************************************/
unsigned char tcip_mib2_statistics_update(void);

/**************************************************************************//**
@ingroup	anyip_edd_interface
@brief		Function to proceed after an update of MIB-2 statistic counters
@details    This function is called by TCIP when the MIB-2 statistic counter 
			update is completed. Then SNMP can proceed to handle the SNMP
			request further.
******************************************************************************/
void anyip_mib2_statistics_update_done(void);

/**************************************************************************//**
@ingroup		anyip_edd_interface
@brief			Function is declared here, but implemented in tcip_edd.c
@note			use this function in the IP stack code for setting the ARP 
				filter in EDD for a new address

@param[in]      ch: The TCIP channel for the interface, 
					should be stored in the interface data structure when 
					creating the interface
@param[in]      user_index: the index for EDD, where to store the ARP filter 
					address.
					Possible Value           | Meaning
					-------------------------|--------------------
					TCIP_CLUSTER_USER_INDEX  | Cluster IP Address
					TCIP_ARP_USER_INDEX      | Regular IP Address
@param[in]      ip_address: the ip_address in network byte order for which
					the ARP filter should be set.
******************************************************************************/
void tcip_edd_srv_arp_filter(void *ch, unsigned char user_index, unsigned long ip_address);

/* Defines used as user index for the function tcip_edd_srv_arp_filter */
#define TCIP_CLUSTER_USER_INDEX 1 /**<user index for setting the cluster IP address in the ARP filter of EDD */
#define TCIP_ARP_USER_INDEX 0 /**<user index for setting a standard IP address in the ARP filter of EDD */

#endif /* __ANYIP_EDD_SNMP_COUNTER_H__ */
