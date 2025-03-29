#ifndef __ANYIP_EDD_SEND_H__
#define __ANYIP_EDD_SEND_H__

/**************************************************************************//**
@file		anyip_edd_send.h
@brief		This file contains functions and definitions concerning the
			sending of data of a IP stack.
******************************************************************************/
#include "anyip_inc.h"

/* API functions for IP -- MUST be implemented */

/**************************************************************************//**
@ingroup		anyip_edd_interface
@brief          Returns bitfield encoding the protocol of an Ethernet frame
				contained in an RQB-send buffer.
@details        Analyzes an RQB-send buffer containing an Ethernet frame and encodes 
				the used protocol in a bitfield

@param[in]      sendBuf:	pointer to the RQB-send buffer, where Ethernet frame is 
							stored

@return         bitfield with bitmasks set according to the protocol
				Protocol | bitmask
				---------|-------------
				IP       | ANYIP_IP
				UDP      | ANYIP_UDP
				TCP      | ANYIP_TCP
******************************************************************************/
unsigned char anyip_edd_get_protocol_flags(char *sendBuf);

/**************************************************************************//**
@ingroup		anyip_edd_interface
@brief          Copies Ethernet frame from IP-packet buffer into RQB-send buffer.
@details        Copies an Ethernet frame wrapped in an IP-packet buffer
				to an RQB-send buffer.

@param[in]      packet:		pointer to IP-packet buffer containing Ethernet frame
@param[in]      sendBuf:	pointer to the RQB-send buffer
@param[in]		sendLen:	length of Ethernet frame to be copied

@return         0 in case of success
@return			-1 in case of an error
******************************************************************************/
int anyip_edd_cpy_pkt_to_buffer(ANYIP_PKTBUF_PTR packet, char *sendBuf, int sendLen);

/**************************************************************************//**
@ingroup		anyip_edd_interface
@brief          Returns the length of an Ethernet frame in an IP-packet buffer.
@details        Returns the length of an Ethernet frame wrapped in an 
				IP-packet buffer.

@param[in]      packet:	pointer to IP-packet buffer containing Ethernet frame
					to be sent

@return         length of the Ethernet frame to be sent
@return			[INTERNICHE, CUSTOM, LINUX] -1 in case of an error
******************************************************************************/
int anyip_edd_get_sendlen(ANYIP_PKTBUF_PTR packet);

/**************************************************************************//**
@ingroup		anyip_edd_interface
@brief          Returns the pointer to the Ethernet frame to be sent.
@details        Returns the pointer to the Ethernet frame to be sent.

@param[in]      packet:	pointer to the IP-packet buffer containing the Ethernet 
					frame.

@return         pointer to the Ethernet frame to be sent
@return			[INTERNICHE, CUSTOM, LINUX] NULL in case of an error
******************************************************************************/
char *anyip_edd_get_ip_pkt_ptr(ANYIP_PKTBUF_PTR packet);

/**************************************************************************//**
@ingroup		anyip_edd_interface
@brief          Frees an IP-packet buffer.

@param[in]      packet:	pointer to IP-packet buffer
******************************************************************************/
void anyip_edd_free_pkt(ANYIP_PKTBUF_PTR packet);

/**************************************************************************//**
@ingroup		anyip_edd_interface
@brief          Returns an IP-packet buffer containing the next Ethernet frame to 
				be sent.
@details        The function tcip_pkt_send() triggers the sending of an 
				Ethernet frame. The IP-packet buffers containing the packets 
				are stored in an IP stack-specific manner. This function gets 
				the IP-packet buffer from the IP stack which contains the next 
				Ethernet frame to be sent.

@param[in]      channel:	TCIP channel which is associated to the network 
							interface
@param[in]      ifp:		pointer to the network interface, which wants to send 
							an Ethernet frame

@return         IP-packet buffer containing the next Ethernet frame to 
				be sent in case of success
@return			NULL in case of an error
******************************************************************************/
ANYIP_PKTBUF_PTR anyip_edd_get_next_send_pkt(void *channel, ANYIP_IFNET_PTR ifp);

/**************************************************************************//**
@ingroup	anyip_edd_interface
@brief		Triggers the sending of an Ethernet frame.
@details	This function is used in IP-stack specific functions to trigger 
			TCIP to get an Ethernet frame from a particular network interface to 
			send it.

@note		This function is declared here, but implemented in tcip_send.c
			No IP-stack specific implementation is necessary.

@note		Use this function to trigger sending of packets via EDD!

@param[in]	tcip_channel:	TCIP channel associated with the network interface, 
							which wants to send an Ethernet frame
@param[in]	port_index:		Index of the port to which the packet should be sent

@return     0 in case of success
@return		-1 in case of an error
******************************************************************************/
int tcip_pkt_send(void *tcip_channel, unsigned port_index);

#endif /* __ANYIP_EDD_SEND_H__ */
