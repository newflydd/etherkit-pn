#ifndef __ANYIP_EDD_RECV_H__
#define __ANYIP_EDD_RECV_H__

/**************************************************************************//**
@file		anyip_edd_recv.h
@brief		This file contains functions and definitions concerning the
			reception of data for an IP stack.
******************************************************************************/
#include "anyip_inc.h"

/**************************************************************************//**
@ingroup		anyip_edd_interface
@brief          Copies the received Ethernet frame from RQB-receive buffer to
				IP-packet buffer.
@details        Copies the received Ethernet frame from RQB-receive buffer in an
				IP-packet buffer. The IP-packet buffer is allocated and has to be freed
				by the IP-stack after receiving is finished.
@param[in]      ifp: pointer to the network interface
@param[in]      pBuffer: pointer to the RQB-receive buffer containing Ethernet frame
@param[in]		length: length of the received Ethenet frame
@return         pointer to the newly allocated IP-packet buffer in case of success
@return         NULL in case of errors
******************************************************************************/
ANYIP_PKTBUF_PTR anyip_edd_copy_recv_buffer(ANYIP_IFNET_PTR ifp, char *pBuffer, int length);

/**************************************************************************//**
@ingroup		anyip_edd_interface
@brief          Sends received Ethernet frame to the IP stack to further processing.
@details        Sends an IP-packet buffer (containing the received Ethernet frame)
				to the IP stack, where it can be further processed. After
				processing, the IP-packet buffer has to be freed.
@param[in]      ifp: pointer to the network interface
@param[in]      pkt: pointer to IP-packet buffer, which contains 
				the received Ethernet frame.
******************************************************************************/
void anyip_edd_receive(ANYIP_IFNET_PTR ifp, ANYIP_PKTBUF_PTR pkt);

#endif /* __ANYIP_EDD_RECV_H__ */
