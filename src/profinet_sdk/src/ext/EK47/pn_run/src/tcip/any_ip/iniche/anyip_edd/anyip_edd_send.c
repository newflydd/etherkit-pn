#include "anyip_edd_send.h"
#include "anyip_edd_send_internal.h"
#include "ipport.h"
#include "q.h"
#include "netbuf.h"
#include "net.h"
#include "ether.h"
#include "arp.h"
#include "ip.h"
#include "udp.h"

int tcip_put_pkt(TCIP_PACKET_HOLD_PTR_TYPE pkt_list, PACKET pkt);
ANYIP_PKTBUF_PTR tcip_get_pkt(TCIP_PACKET_HOLD_PTR_TYPE pkt_list);

void iniche_free_remaining_send_packets(ANYIP_IFNET_PTR ifp)
{
	struct net *my_ifp = (struct net *)ifp;
	PACKET pkt;

	while ((pkt = (PACKET)tcip_get_pkt(&(my_ifp->send_pkts))) != NULL)
	{
		LOCK_NET_RESOURCE(FREEQ_RESID);
		pk_free(pkt);
		UNLOCK_NET_RESOURCE(FREEQ_RESID);
	}
}

unsigned char anyip_edd_get_protocol_flags(char *sendBuf)
{
	unsigned char flags = 0;
	uint16_t IpTp = IPTP;
	uint16_t ethType = ET_TYPE_GET(sendBuf); //lint !e734 - Loss of precision (initialization) (17 bits to 16 bits)

	if (ethType == (uint16_t)htons(IpTp)) /* IP message */
	{
		flags |= ANYIP_IP;
		unsigned char ipProt = ((struct ip*)(sendBuf + ETHHDR_SIZE))->ip_prot;

		if (ipProt == UDP_PROT)
		{
			flags |= ANYIP_UDP;
		}
		else if (ipProt == TCPTP)
		{
			flags |= ANYIP_TCP;
		}
	}

	return flags;
} //lint !e818 Pointer parameter 'sendBuf' could be declared as pointing to const

int anyip_edd_cpy_pkt_to_buffer(ANYIP_PKTBUF_PTR packet, char *sendBuf, int sendLen)
{
	PACKET pkt = (PACKET)packet;
	char  *ip_pkt = pkt->nb_prot + ETHHDR_BIAS;
	if (ip_pkt)
	{
		TCIP_MEMCPY(sendBuf, ip_pkt, (LSA_UINT)sendLen);
		return 0;
	}
	else return -1;
}

int anyip_edd_get_sendlen(ANYIP_PKTBUF_PTR packet)
{
	PACKET pkt = (PACKET)packet;
	return (int)pkt->nb_plen - ETHHDR_BIAS;
}

char *anyip_edd_get_ip_pkt_ptr(ANYIP_PKTBUF_PTR packet)
{
	PACKET pkt = (PACKET)packet;
	return (char *)(pkt->nb_prot + ETHHDR_BIAS);
}

void anyip_edd_free_pkt(ANYIP_PKTBUF_PTR packet)
{
	PACKET pkt = (PACKET)packet;
	LOCK_NET_RESOURCE(FREEQ_RESID);
	pk_free(pkt);
	UNLOCK_NET_RESOURCE(FREEQ_RESID);
}

ANYIP_PKTBUF_PTR anyip_edd_get_next_send_pkt(void *channel, ANYIP_IFNET_PTR ifp)
{
	struct net *my_ifp = (struct net *)ifp;
	ANYIP_UNUSED_ARG(channel);
	return tcip_get_pkt(&(my_ifp->send_pkts));
}

/* Internal */
/*---------------------------------------------------------------------------*/
/* check availability of send rqb on EDD ARP channel, see 1824771            */
/*---------------------------------------------------------------------------*/
int tcip_put_pkt(TCIP_PACKET_HOLD_PTR_TYPE pkt_list, PACKET pkt)
{
	int ret_val;
	LOCK_NET_RESOURCE(FREEQ_RESID);
	pkt->next = LSA_NULL;

	if (pkt_list->cur_len == pkt_list->max_len)
	{
		ret_val = LSA_FALSE;
	}
	else
	{
		if (!(pkt_list->first_out))
		{
			pkt_list->first_out = pkt;
		}
		else
		{
			((PACKET)(pkt_list->last_pkt))->next = pkt;
		}

		pkt_list->last_pkt = pkt;

		pkt_list->cur_len++;

		ret_val = LSA_TRUE;
	}

	UNLOCK_NET_RESOURCE(FREEQ_RESID);
	return ret_val;
}

/*===========================================================================*/

ANYIP_PKTBUF_PTR tcip_get_pkt(TCIP_PACKET_HOLD_PTR_TYPE pkt_list)
{
	PACKET pkt;

	LOCK_NET_RESOURCE(FREEQ_RESID);

	pkt = pkt_list->first_out;

	if (pkt)
	{
		pkt_list->first_out = pkt->next;
		pkt->next = LSA_NULL; /* AP00936006 */
		pkt_list->cur_len--;
	}

	if (!(pkt_list->first_out))
	{
		pkt_list->last_pkt = LSA_NULL;

		pkt_list->cur_len = 0; /* sanity */
	}

	UNLOCK_NET_RESOURCE(FREEQ_RESID);
	return (ANYIP_PKTBUF_PTR)pkt;
}

int tcip_iniche_pkt_send(PACKET packet)
{
	struct net *ifp = packet->net;
	int inserted = tcip_put_pkt(&(ifp->send_pkts), packet);
	if (!inserted) /* queue has maximum length -> insertion failed, see 973233 */
	{
		LOCK_NET_RESOURCE(FREEQ_RESID);
		pk_free(packet);
		UNLOCK_NET_RESOURCE(FREEQ_RESID);
		return ENP_SENDERR;
	}
	else
	{
		tcip_pkt_send(ifp->n_local, 0);
		return 0;
	}
}

int iniche_is_send_resource_available(ANYIP_IFNET_PTR ifp)
{
	NET my_ifp = (NET)ifp;
	if (my_ifp->send_pkts.cur_len + 30 < my_ifp->send_pkts.max_len)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


