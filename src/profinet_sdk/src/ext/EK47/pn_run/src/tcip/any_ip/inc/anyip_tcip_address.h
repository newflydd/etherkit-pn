#ifndef __ANYIP_TCIP_ADDRESS_H__
#define __ANYIP_TCIP_ADDRESS_H__

/**************************************************************************//**
@file		anyip_tcip_address.h
@brief		This file contains functions and definitions concerning the 
			IP stack address handling.
******************************************************************************/
#include "anyip_inc.h"

/**************************************************************************//**
@ingroup		anyip_tcip_address
@brief          Deletes the IPv4-address/network mask for a given network 
				interface and frees the resources
@details        Deletes the IPv4 network address/network mask and frees the 
				(kernel-)resources allocated during assignment of the IPv4-
				address. The network interface must be known within the IP-Stack.

@param[in]      ifp: The pointer to the network interface.

@return         0 in case of success
@return         -1 in case of errors
******************************************************************************/
int anyip_tcip_del_ipv4suite(ANYIP_IFNET_PTR ifp);

/**************************************************************************//**
@ingroup		anyip_tcip_address
@brief          Sets an IPv4-address/network mask for a given network interface.
@details        Sets an IPv4 network address and the according network mask for 
				an network interface of the IP-Stack. The network interface 
				must be known within the IP-Stack.

@li				The function can only set one IP-Address/mask to the interface.
				No alias-IPs are possible.
@li				If the interface already has an IP-Address the function
				will overwrite this address.

@param[in]      ifp: The pointer to the network interface.
@param[in]      ip_address: The IPv4 address to set in network byte order.
@param[in]      subnet_mask: The IPv4 network mask to set in network byte order.


@return         0 in case of success
@return         -1 in case of errors
******************************************************************************/
int anyip_tcip_set_ipv4suite(ANYIP_IFNET_PTR ifp, unsigned long ip_address, unsigned long subnet_mask);

/**************************************************************************//**
@ingroup		anyip_tcip_address
@brief          Deletes a route in the routing table.
@details        Deletes an IPv4 routing table entry. 

@note			If the input parameters are 0, the default gateway is deleted.

@param[in]      ip_address: the address of the routing table entry to 
							be deleted (in network byte order)
@param[in]		subnet_mask: the network mask of the routing table entry to 
							be deleted (in network byte order)

@return         0 in case of success
@return         -1 in case of errors
******************************************************************************/
int anyip_tcip_del_route(unsigned long ip_address, unsigned long subnet_mask);

/**************************************************************************//**
@ingroup		anyip_tcip_address
@brief          Adds an entry in the routing table.
@details        Adds a gateway for the given IPv4-address and subnet mask in
				the routing table. This leads to a new entry in the kernel 
				routing table. 

@note			If the address and the subnet mask are 0, the default gateway 
				is set.

@param[in]      ip_address: the IPv4-address for the routing table entry
				in network byte order.
@param[in]      subnet_mask: the IPv4-network mask for the routing table entry
				in network byte order.
@param[in]      gateway: the gateway for the ip_address and subnet_mask
				in network byte order.

@return         0 in case of success
@return         -1 in case of errors
******************************************************************************/
int anyip_tcip_add_route(unsigned long ip_address, unsigned long subnet_mask, unsigned long gateway);

/**************************************************************************//**
@ingroup		anyip_tcip_address
@brief			Enables or disables IPV4 forwarding.
@details		Forwarding of IPV4 packets to available network interfaces 
				and subnets is activated/deactivated. This way the IP stack 
				acts as a router for IPV4.

@param[in]		enable
				1 activates IPV4 forwarding
				0 deactivates IPV4 forwarding

@return			0 in case of success
@return			-1 in case of errors
******************************************************************************/
int anyip_tcip_set_ipv4forwarding(unsigned int enable);

/**************************************************************************//**
@ingroup		anyip_tcip_address
@brief			Returns whether IPV4 Forwarding is currently enabled.

@param[out]		out_is_enabled
				1 IPV4 Forwarding is enabled
				0 IPV4 Forwarding is disabled

@return			0 in case of success
@return			-1 in case of errors
******************************************************************************/
int anyip_tcip_get_ipv4forwarding(unsigned int *out_is_enabled);

#endif /* __ANYIP_TCIP_ADDRESS_H__ */
