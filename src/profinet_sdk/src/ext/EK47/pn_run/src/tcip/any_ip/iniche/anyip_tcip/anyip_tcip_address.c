#include "anyip_tcip_address.h"
#include "ipport.h"
#include "netbuf.h"
#include "nptypes.h"
#include "net.h"
#include "ip.h"

extern unsigned long cticks;

static void iniche_rebuild_route_table(void);
int tcip_iniche_set_gateway(int gateway);
extern int tcip_iniche_pkt_send(PACKET packet);

int anyip_tcip_del_ipv4suite(ANYIP_IFNET_PTR ifp)
{
	int ret = ni_set_ipsuite(ifp, 0, 0, 0);
	if (ret == 0) iniche_rebuild_route_table(); /* AP01256605 */
	return ret;
}

int anyip_tcip_set_ipv4suite(ANYIP_IFNET_PTR ifp, unsigned long ip_address, unsigned long subnet_mask)
{
	int ret = ni_set_ipsuite(ifp, ip_address, subnet_mask, 0);
	if (ret == 0) iniche_rebuild_route_table(); /* AP01256605 */
	return ret;
}

int anyip_tcip_del_route(unsigned long ip_address, unsigned long subnet_mask)
{
	int retval = -1;
	ANYIP_UNUSED_ARG(ip_address); /* at the moment only the default gateway is valid */
	ANYIP_UNUSED_ARG(subnet_mask); /* at the moment only the default gateway is valid */

	retval = tcip_iniche_set_gateway(0);
	if (retval == 0) iniche_rebuild_route_table(); /* refresh gateway, see 2256680 and AP01256605 */
	return retval;
}

int anyip_tcip_add_route(unsigned long ip_address, unsigned long subnet_mask, unsigned long gateway)
{
	int retval = -1;
	ANYIP_UNUSED_ARG(ip_address); /* at the moment only the default gateway is valid */
	ANYIP_UNUSED_ARG(subnet_mask); /* at the moment only the default gateway is valid */

	retval = tcip_iniche_set_gateway((int)gateway);
	if (retval == 0) iniche_rebuild_route_table(); /* refresh gateway, see 2256680 and AP01256605 */
	return retval;
}

int anyip_tcip_set_ipv4forwarding(unsigned int enable) /* not implemented */
{
	ANYIP_UNUSED_ARG(enable);
	return -1;
}

int anyip_tcip_get_ipv4forwarding(unsigned int *out_is_enabled) /* not implemented */
{
	ANYIP_UNUSED_ARG(out_is_enabled);
	return -1;
}

/* internal functions */

static void
iniche_add_route(
	RTMIB	rtp,        /* route table entry */
	ip_addr  dest,       /* ultimate destination */
	ip_addr  mask,       /* net mask, 0xFFFFFFFF if dest is host address */
	ip_addr  nexthop,    /* where to forward to */
	long     type,       /* ipRouteType; 3=direct 4=indirect  */
	NET      ifp
) {
	rtp->ifp = ifp;
	rtp->ipRouteDest = dest;
	rtp->ipRouteIfIndex = (long)(if_netnumber(ifp)) + 1; /* set interface number */
	rtp->ipRouteMetric1 = (long)ip_mib.ipDefaultTTL;
	rtp->ipRouteMetric2 = -1;
	rtp->ipRouteMetric3 = -1;
	rtp->ipRouteMetric4 = -1;
	rtp->ipRouteNextHop = nexthop;
	rtp->ipRouteProto = IPRP_LOCAL;                 /* icmp, or whatever */
	rtp->ipRouteType = type;
	rtp->ipRouteAge = cticks;        /* timestamp it */
	rtp->ipRouteMask = mask;
	rtp->ipRouteMetric5 = -1;
}

/*---------------------------------------------------------------------------*/

/*
*   iniche_rebuild_route_table()
*   deletes ip route table and rebuilds it
*/
static void
iniche_rebuild_route_table(
	void
) {
	unsigned int route_index;
	NET ifp;

	int gateway_added = 0;

	for (route_index = 0; route_index < ipRoutes; route_index++)
	{
		rt_mib[route_index].ipRouteNextHop = 0;
	}

	route_index = 0;

	for (ifp = (NET)(netlist.q_head); ifp; ifp = ifp->n_next) /* see loop in if_getbynum() */
	{
		if (ifp->pkt_send == tcip_iniche_pkt_send /* is a TCIP interface */
			&& ifp->n_local != 0 /* in use */
			)
		{
			if (ifp->n_ipaddr != 0)
			{
				if (route_index < ipRoutes) /* sanity */
				{
					iniche_add_route(&rt_mib[route_index] /* append route for subnet */
						, (ifp->n_ipaddr & ifp->snmask)
						, ifp->snmask
						, ifp->n_ipaddr
						, 3 /* direct route */
						, ifp
					);

					route_index++;

					if (ifp->n_defgw != 0 /* add one default gateway if present */
						&& !gateway_added
						&& route_index < ipRoutes /* sanity */
						)
					{
						iniche_add_route(&rt_mib[route_index] /* append route for default gateway, see AP01256605 */
							, 0
							, 0
							, ifp->n_defgw
							, 4 /* indirect route */
							, ifp
						);

						route_index++;

						gateway_added = 1;
					}
				}
			}
		}
	}
}

/*
* tcip_iniche_set_gateway() sets the default gateway
*
* 1) loop over all nets:
*    if the net matches (gateway in the same subnet)set the gateway
*    if the gateway is not in the same subnet as the net, reset the gateway entry of the net

* 2) if the gateway was set for one interface, return TCIP_OK, otherwise TCIP_ERR_LOWER_LAYER.
*/
int tcip_iniche_set_gateway(
	int	gateway
) {
	NET ifp;

	if (gateway == 0) /* remove default gateway */
	{
		for (ifp = (NET)(netlist.q_head); ifp; ifp = ifp->n_next) /* see loop in if_getbynum() */
		{
			if (ifp->pkt_send == tcip_iniche_pkt_send /* is a TCIP interface */
				&& ifp->n_local != 0 /* in use */
				)
			{
				ifp->n_defgw = 0; /* remove gateway for this net */
			}
		}

		return 0;
	}
	else
	{
		int retval = -1;

		for (ifp = (NET)(netlist.q_head); ifp; ifp = ifp->n_next) /* see loop in if_getbynum() */
		{
			if (ifp->pkt_send == tcip_iniche_pkt_send /* is a TCIP interface */
				&& ifp->n_local != 0 /* in use */
				)
			{
				ifp->n_defgw = 0; /* remove gateway for this net */

				if (ifp->snmask != 0 /* subnet mask present, see below */
					&& (ifp->n_ipaddr & ifp->snmask) == ((unsigned long)gateway & ifp->snmask) /* gateway in same subnet */
					)
				{
					ifp->n_defgw = (unsigned long)gateway; /* gateway is in the same net -> set gateway for this net */

					retval = 0; /* found */
				}
			}
		}

		return retval;
	}
}
