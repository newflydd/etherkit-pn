#include "anyip_tcip_interface.h"
#include "anyip_tcip_address.h"
#include "anyip_edd_send.h"
#include "anyip_edd_send_internal.h"
#include "tcip_iniche_cfg.h"

#include "ipport.h"
#include "tcpport.h"
#include "netbuf.h"
#include "nptypes.h"
#include "net.h"
#include "ether.h"
#include "igmp.h"
#include "igmp_cmn.h"

static int tcip_iniche_create_device(NET ifp, void * bindinfo);
extern int tcip_iniche_pkt_send(PACKET packet);

ANYIP_IFNET_PTR anyip_tcip_create_and_activate_interface(void *channel, char *MAC_address, int ifAdminStatus, unsigned interface_id, unsigned port_index)
{
	char name[IF_NAMELEN] = "???"; /* only for IN_MENUS */
	NET ifp;

	ANYIP_UNUSED_ARG(interface_id);
	ANYIP_UNUSED_ARG(port_index);

	if (ni_create(&ifp, tcip_iniche_create_device, name, NULL) != 0)
	{
		return NULL;
	}

	ni_set_ipsuite(ifp, 0, 0, 0);

#if TCIP_CFG_ENABLE_MULTICAST
	in_addmulti(&igmp_all_hosts_group, ifp, 4);
#endif

	ifp->n_local = channel; /* used */
	memcpy(ifp->mib.ifPhysAddress, MAC_address, 6);
	ni_set_state(ifp, NI_UP); /* ifAdminStatus is always up */

	if (ifAdminStatus != ANYIP_STATE_UP)
	{
		ifp->mib.ifAdminStatus = NI_DOWN;
	}
	else
	{
		ifp->mib.ifAdminStatus = NI_UP;
	}

	return (ANYIP_IFNET_PTR)ifp;
} //lint !e818 Pointer parameter 'MAC_address' could be declared as pointing to const

void anyip_tcip_set_if_description(ANYIP_IFNET_PTR ifp, char * p_ifDescr, int ifDescr_length)
{
	struct net *my_ifp = (struct net *)ifp;
	memcpy(my_ifp->mib.ifDescr, p_ifDescr, (unsigned int)ifDescr_length);
	my_ifp->mib.ifDescr[ifDescr_length] = 0;
} //lint !e818 Pointer parameter 'p_ifDescr' could be declared as pointing to const

int anyip_tcip_get_if_index(ANYIP_IFNET_PTR ifp)
{
	struct net *my_ifp = (struct net *)ifp;
	return (int)(my_ifp->mib.ifIndex + 1); /* note: mib.ifIndex is zero-based, see declaration */
}

int anyip_tcip_set_admin_state(ANYIP_IFNET_PTR ifp, int up_or_down)
{
	NET my_ifp = (NET)ifp;
	if (up_or_down != ANYIP_STATE_UP)
	{
		my_ifp->mib.ifAdminStatus = NI_DOWN;
	}
	else
	{
		my_ifp->mib.ifAdminStatus = NI_UP;
	}

	return 0;
}

void anyip_tcip_set_link_state(ANYIP_IFNET_PTR ifp, int up_or_down)
{
	NET my_ifp = (NET)ifp;
	if (up_or_down != ANYIP_STATE_UP)
	{
		my_ifp->mib.ifOperStatus = NI_DOWN;
	}
	else
	{
		my_ifp->mib.ifOperStatus = NI_UP;
	}
}

void anyip_tcip_set_baudrate(ANYIP_IFNET_PTR ifp, long baudrate)
{
	NET my_ifp = (NET)ifp;
	my_ifp->mib.ifSpeed = (unsigned long)baudrate;
}
extern void vPortFree( void *pv );
int anyip_tcip_deactivate_and_delete_interface(ANYIP_IFNET_PTR ifp)
{
	struct net *my_ifp = (struct net *)ifp;
	my_ifp->n_local = 0; /* free */

	ni_set_ipsuite(my_ifp, 0, 0, 0);

	my_ifp->mib.ifSpeed = 0;

	ni_set_state(my_ifp, NI_DOWN); /* ifAdminStatus and ifOperStatus go down */

	TCIP_MEMSET(my_ifp->mib.ifPhysAddress, 0, 6);
	TCIP_MEMSET(my_ifp->mib.ifDescr, 0, 255 + 1);

	iniche_free_remaining_send_packets(ifp);

	//free (my_ifp->mib.ifDescr);
	//free(my_ifp->mib.ifPhysAddress);

	vPortFree (my_ifp->mib.ifDescr);
	vPortFree(my_ifp->mib.ifPhysAddress);

#if TCIP_CFG_ENABLE_MULTICAST
	struct in_multi * inm = lookup_mcast(igmp_all_hosts_group, my_ifp);
	if (inm != NULL)
	{
		in_delmulti(inm);
	}
#endif

	return ni_delete(my_ifp);
}

/* internal functions */

static int tcip_mcastlist(struct in_multi * multi_ptr)
{
	/* note: do nothng here, see tcip_srv_multicast() */
	LSA_UNUSED_ARG(multi_ptr);
	return 0;
}

static int tcip_iniche_create_device(NET ifp, void *bindinfo)
{
	ANYIP_UNUSED_ARG(bindinfo);

	u_char *mac_address = NULL;
	char *if_descr = NULL;

	OsAlloc(&mac_address, 0, 6);
	OsAlloc(&if_descr, 0, 255 + 1);

	if (mac_address == NULL || if_descr == NULL)
	{
		return -1;
	}

	ifp->n_local = 0; /* usage see tcip_iniche_activate_interfaces() */
	ifp->n_local2 = 0; /* unused */
	ifp->n_snbits = 0; /* nsref_complete.htm: n_snbits is currently unused */

	ifp->n_flags |= NF_BCAST;        /* Set bcast flag bit for ethernet    */
	ifp->n_flags |= NF_MCAST;        /* Set mcast flag bit for ethernet    */
	ifp->n_flags |= NF_NBPROT;       /* Set nbprot flag bit for ethernet   */

	if (ETHHDR_SIZE != (EDD_MAC_ADDR_SIZE + EDD_MAC_ADDR_SIZE + ETHHDR_BIAS/*see 1221853*/ + 2)) /* ETHHDR_SIZE is sizeof() */
	{
		return -1;
	}

#if EDD_MAC_ADDR_SIZE != 6
#error "check this!"
#endif

#if ET_MAXLEN != (EDD_IEEE_FRAME_LENGTH - EDD_VLAN_TAG_SIZE)
#error "check this!"
#endif

	ifp->n_hal = EDD_MAC_ADDR_SIZE;  /* hardware address length            */
	ifp->n_lnh = ETHHDR_SIZE;        /* space reserved for ethernet header */
	ifp->n_mtu = ET_MAXLEN;          /* max frame size                     */

	ifp->n_ipaddr = 0; /* 0.0.0.0 (no ip address) */
	ifp->snmask = 0; /* 0.0.0.0 (invalid) */
	ifp->n_defgw = 0; /* 0.0.0.0 (no gateway) */

	ifp->n_netbr = 0; /* sanity */
#if 0 /* see 957200 */
	ifp->n_netbr42 = 0; /* sanity */
#endif
	ifp->n_subnetbr = 0; /* sanity */

	ifp->n_mcastlist = tcip_mcastlist;

	ifp->igmp_admin_mode = IGMP_MODE_V2;
	ifp->igmp_oper_mode = ifp->igmp_admin_mode;

	ifp->igmp_timer1 = 0; /* compare to igmp_init() */

	ifp->n_init = NULL;
	ifp->n_close = NULL;
	ifp->n_stats = NULL;
	ifp->n_reg_type = NULL;
	ifp->n_setstate = NULL;
	ifp->pkt_send = tcip_iniche_pkt_send;
	ifp->raw_send = NULL; /* using ifp->pkt_send */

	{
		IFMIB mib = &ifp->mib;

		if (mib != ifp->n_mib) /* see ni_create() */
		{
			return -1;
		}

		mib->ifIndex = (u_long)if_netnumber(ifp); /* note: zero-based, see declaration (SNMP-code adds 1, see rfc1213.c) */

		mib->ifDescr = if_descr; /* pointer to ifDescr */

		mib->ifAdminStatus = NI_DOWN; /* see ni_create() */
		mib->ifOperStatus = NI_DOWN; /* see tcip_edd_ext_link_status_indication() */

		mib->ifType = ETHERNET;
		mib->ifMtu = ET_MAXLEN - (sizeof(struct ethhdr)); /* RQ1475679 - MTU is (frame length - header length); ET_MAXLEN = EDD_IEEE_FRAME_LENGTH - EDD_VLAN_TAG_SIZE see check above */
		mib->ifSpeed = 0;

		mib->ifPhysAddress = mac_address; /* see tcip_iniche_activate_interfaces() */
	}

#if 1 /* sado -- added sendqueue here */
	ifp->send_pkts.first_out = LSA_NULL;
	ifp->send_pkts.last_pkt = LSA_NULL;
	ifp->send_pkts.cur_len = 0;
	ifp->send_pkts.max_len = MAXPACKETS / TCIP_MAX_IF_COUNT;
#endif

	return 0;
}



