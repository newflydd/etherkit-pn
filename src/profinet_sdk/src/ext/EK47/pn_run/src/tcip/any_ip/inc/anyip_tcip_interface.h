#ifndef __ANYIP_TCIP_INTERFACE_H__
#define __ANYIP_TCIP_INTERFACE_H__

/**************************************************************************//**
@file		anyip_tcip_interface.h
@brief		This file contains functions and definitions concerning the
			management of network interfaces.
******************************************************************************/
#include "anyip_inc.h"

/**************************************************************************//**
@ingroup		anyip_tcip_network
@brief          Creates and activates a network interface.
@details        Creates a network interface structure and initializes it.
				The stack internal interface structures have to be 
				initialized. The name and the index have to be fixed.
				The MAC address shall be assigned. The channel pointer 
				shall be stored for the use as reference for the SOCK component. 
				The admin status has to be set according to the ifAdminStatus parameter.

@param[in]      channel: pointer to the SOCK channel, the interface is
				assigned to
@param[in]      MAC_address: MAC address of the network device to be assigned
				to the interface
@param[in]      ifAdminStatus: admin state to set to the interface
				Possible Value   | Meaning
				-----------------|---------
				ANYIP_STATE_UP   | up
				ANYIP_STATE_DOWN | down
@param[in]      interface_id: identifier of the interface
@param[in]      port_index: Port number. The port 0 is equal to the physical interface.

@return         pointer to network interface in case of success
@return         NULL in case of errors
******************************************************************************/
ANYIP_IFNET_PTR anyip_tcip_create_and_activate_interface(void *channel, char *MAC_address, int ifAdminStatus, unsigned interface_id, unsigned port_index);

/**************************************************************************//**
@ingroup		anyip_tcip_network
@brief          Sets the interface description for a network interface.
@details        Sets the interface description of a given network interface 
				to p_ifDescr
@param[in]      ifp: pointer to the network interface
@param[in]		p_ifDescr: string to set as interface description
@param[in]      ifDescr_length: length of the string for the interface 
					description
******************************************************************************/
void anyip_tcip_set_if_description(ANYIP_IFNET_PTR ifp, char * p_ifDescr, int ifDescr_length);

/**************************************************************************//**
@ingroup		anyip_tcip_network
@brief          Returns the interface index
@details        Returns the interface index of a given network interface

@param[in]      ifp: pointer to the network interface

@return          interface index in case of success
@return         -1 in case of errors
******************************************************************************/
int anyip_tcip_get_if_index(ANYIP_IFNET_PTR ifp);

/**************************************************************************//**
@ingroup		anyip_tcip_network
@brief          Sets the admin state for a network interface.
@details        Sets the admin state of a given network interface to 
				"UP" or "DOWN"
@param[in]      ifp: pointer to the network interface
@param[in]      up_or_down: admin state to set to the interface
				Possible Value   | Meaning
				-----------------|---------
				ANYIP_STATE_UP   | up
				ANYIP_STATE_DOWN | down
@return          0 in case of success
@return         -1 in case of errors
******************************************************************************/
int anyip_tcip_set_admin_state(ANYIP_IFNET_PTR ifp, int up_or_down);

/**************************************************************************//**
@ingroup		anyip_tcip_network
@brief          Sets the link state for a network interface.
@details        Sets the link state of a given network interface to
				"UP" or "DOWN"
@param[in]      ifp: pointer to the network interface
@param[in]      up_or_down: link state to set to the interface
				Possible Value   | Meaning
				-----------------|---------
				ANYIP_STATE_UP   | up
				ANYIP_STATE_DOWN | down
@return          0 in case of success
@return         -1 in case of errors
******************************************************************************/
void anyip_tcip_set_link_state(ANYIP_IFNET_PTR ifp, int up_or_down);

/**************************************************************************//**
@ingroup		anyip_tcip_network
@brief          Sets the baudrate for a network interface.
@details        Sets the baudrate of a given network interface to
				the highest possible speed of transmission in bits per second
@param[in]      ifp: pointer to the network interface
@param[in]      baudrate: highest possible speed of transmission in 
				bits per second
******************************************************************************/
void anyip_tcip_set_baudrate(ANYIP_IFNET_PTR ifp, long baudrate);

/**************************************************************************//**
@ingroup		anyip_tcip_network
@brief          Deactivates and deletes a network interface.
@details        Deletes a given network interface. Deletes the interface 
				from the interface list. Frees all structures assigned to
				the interface.
@param[in]      ifp: pointer to the network interface
@return          0 in case of success
@return         -1 in case of errors
******************************************************************************/
int anyip_tcip_deactivate_and_delete_interface(ANYIP_IFNET_PTR ifp);

#endif /* __ANYIP_INTERFACE_H__ */
