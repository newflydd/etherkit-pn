#include "anyip_edd_recv.h"
#include "ipport.h"
#include "tcpport.h"
#include "net.h"  /* NET struct definition */
#include "udp.h"
#include "ether.h"

ANYIP_PKTBUF_PTR anyip_edd_copy_recv_buffer(ANYIP_IFNET_PTR ifp, char *pBuffer, int length)
{
	LOCK_NET_RESOURCE(FREEQ_RESID);
	PACKET pkt = pk_alloc((unsigned int)length);
	UNLOCK_NET_RESOURCE(FREEQ_RESID);

	if (pkt)
	{
		memcpy(pkt->nb_buff, pBuffer, pkt->nb_blen);
		pkt->nb_prot = pkt->nb_buff + ETHHDR_SIZE;
		pkt->nb_plen = (unsigned int)length - ETHHDR_SIZE;
		pkt->nb_tstamp = (long)cticks;
		pkt->net = ifp; /* not: pRecvParam->PortID, see AP00819817 */

						/* not setting pkt->nb_plen; has to be the length of allocated buffer bytes; if another value is written, the packet is never freed --> see pk_free() */

		{ /* AP00788431: NF_NBPROT flag bit set --> set pkt->type and pkt->nb_prot */
			char *   eth;
			eth = (pkt->nb_buff + ETHHDR_BIAS);

#ifdef TCIP_CFG_VLAN /*INICHE_TCIP_ITGR enable support of VLAN tagged ethernet frames (IEEE 802.1 Q)*/
			/* VLAN Info: Tag Protocol Identifier (TPID):    2 Bytes = 0x8100 */
			/*            User_priority:                     3 Bit */
			/*            Canonical Format Indicator (CFI):  1 Bit */
			/*            VLAN Identifier (VID):            12 Bit */
			if ((unsigned short)ET_TYPE_GET(eth) == 0x8100)
			{
				/* skip VLAN Info (4 Bytes) */
				pkt->type = htons((unshort)ET_TYPE_GET(eth + 4));
				pkt->nb_prot = pkt->nb_buff + ETHHDR_SIZE /*+ ETHHDR_BIAS see 1221853*/ + 4; /*AP00922404 */
			}
			else
#endif
			{
#ifdef IEEE_802_3
				/* see if it's got snap baggage */
				if (ET_TYPE_GET(eth) <= 0x0600)
				{
					struct snap_hdr *snap;
					snap = (struct snap_hdr *)(pkt->nb_buff + ETHHDR_SIZE);
					pkt->type = (unshort)(snap->type);
					pkt->nb_prot = pkt->nb_buff + pkt->net->n_lnh;
				}
				else
				{
					pkt->type = htons((unshort)ET_TYPE_GET(eth));
					pkt->nb_prot = pkt->nb_buff + ETHHDR_SIZE;
				}
#else
				pkt->type = htons((unshort)ET_TYPE_GET(eth));
				pkt->nb_prot = pkt->nb_buff + pkt->net->n_lnh;
#endif
			}
		}
	}

	return (ANYIP_PKTBUF_PTR)pkt;
} //lint !e818 Pointer parameter 'pBuffer' could be declared as pointing to const

void anyip_edd_receive(ANYIP_IFNET_PTR ifp, ANYIP_PKTBUF_PTR pkt)
{
	ANYIP_UNUSED_ARG(ifp);

	LOCK_NET_RESOURCE(RXQ_RESID);
	putq(&rcvdq, (q_elt)pkt); /* give packet to InterNiche stack */
	UNLOCK_NET_RESOURCE(RXQ_RESID);

	SignalPktDemux();
}
