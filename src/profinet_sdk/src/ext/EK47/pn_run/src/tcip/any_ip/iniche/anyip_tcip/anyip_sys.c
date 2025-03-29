#define LTRC_ACT_MODUL_ID  6
#define TCIP_MODULE_ID     6

#include "tcip_iniche_cfg.h"

#include "anyip_inc.h"
#include "anyip_sys.h"

#include "ipport.h"
#include "tcpport.h"

#include "ether.h"
#include "ip.h"
#include "nptcp.h"
#include "q.h"
#include "tcp_timr.h"
#include "arp.h"
#include "udp.h"
#include "net.h"
#include "snmpport.h"

#ifdef IP_FRAGMENTS
#include "ip_reasm.h"
#endif

#if TCIP_CFG_ENABLE_MULTICAST
#include "igmp_cmn.h"
#include "igmp.h"
#endif

static void tcip_iniche_store_allocated_buffers(void);
static void tcip_iniche_free_allocated_buffers(void);

#ifdef IP_FRAGMENTS
extern IREP h_ireq; /* head pointer to IP reassembly entry (IRE) queue */
#endif

					/*===========================================================================*/

#define NETPORT_BUFFERS_MAX  ((NUMBIGBUFS + NUMLILBUFS)*2 + 3)

typedef struct bufferStoreStruct {
	PACKET bigPackets[NUMBIGBUFS];                    /* List of big packets            */
	char*  bigBuffers[NUMBIGBUFS];                    /* List of big packet buffers     */
	PACKET lilPackets[NUMLILBUFS];                    /* List of small packets          */
	char*  lilBuffers[NUMLILBUFS];                    /* List of small packet buffers   */
	struct mbuf* netportBuffers[NETPORT_BUFFERS_MAX]; /* List of Netport buffers        */
} BUFFER_STORE_TYPE;

static BUFFER_STORE_TYPE tcip_Buffers;

static int tcip_inside_pktdemux;
unsigned long cticks; /* see nsref_complete.htm, chapter "Timers and Multitasking" */

/*===========================================================================*/

/*iface.c*/
#ifndef PLLISTLEN
#error "check this"
#endif
extern unshort  protlist[PLLISTLEN]; /* zero means unused entry */

									 /*asn1.c*/
									 /*dnsclnt.c*/
#ifdef DNS_CLIENT
extern unshort  dnsids;  /* seed for unique request IDs */

						 /* DNS client statistics: */
extern ulong      dnsc_errors;      /* protocol/implementation runtime errors */
extern ulong      dnsc_requests;    /* requests sent */
extern ulong      dnsc_replies;     /* replys received */
extern ulong      dnsc_good;        /* usable replys received */
extern ulong      dnsc_tmos;        /* timeouts */
extern ulong      dnsc_retry;       /* total retries */

#ifdef DNS_CLIENT_UPDT
extern ulong      dnsc_updates;     /* Dynamic DNS Updates sent */
#endif /* DNS_CLIENT_UPDT */

									/* pending requests, can be used as a flag to spin dnsc_check() task */
extern unsigned   dnsc_active;

/* retry info, can be overwritten by application code */
extern unsigned dns_firsttry; /* time to first retry, in seconds */
extern unsigned dns_trys;     /* max number of retrys */

extern ip_addr dns_servers[MAXDNSSERVERS]; /* need to be set from NV parameters */

#ifdef  DNS_CLIENT_UPDT
extern char    soa_mname[MAXDNSNAME];
#endif  /* DNS_CLIENT_UPDT */
#endif  /* dns_client */

/*et_arp.c*/
extern struct arptabent *   arpcache;   /* last ARP entry used */
extern struct arptabent  arp_table[MAXARPS];     /* the actual table */

#ifdef IEEE_802_3
#endif
												 /*icmp.c*/

												 /* the callback for application to get a peek at icmp du packets */
#ifdef FULL_ICMP
#ifdef NPDEBUG
#endif   /* NPDEBUG */
#endif

												 /*iface.c*/
extern queue netlist;          /* master list of nets, static & dynamic */
							   /*in_pcb.c*/
							   /*ip.c*/
#ifdef IP_ROUTING
#endif   /* IP_ROUTING */
							   /**ip_reasm.c*/
#ifdef IP_FRAGMENTS
#include <ip_reasm.h>
extern u_long ipr_curr_mem;
/* head pointer to IP reassembly entry (IRE) queue */
extern IREP h_ireq;
/* next invocation of timer tick processing routine */
extern u_long ire_cticks;
#endif
/*ipnet.c*/
#ifdef  LOSSY_IO
/* support for an option that allows us to deliberatly loose packets */
#endif   /* LOSSY_IO */
extern queue    rcvdq;         /* queue of all received (but undemuxed) packets */
							   /*ipport.c*/
extern int(*port_prep)(int already_found);
/*ipstart.c*/
extern NET   nets[MAXNETS];
extern unsigned ifNumber;        /* number of net interfaces */
								 /*extern void (*closers[NUMCLOSERS])(void); Define only local */
extern int nclosers;
/*nptcp.c*/
#ifdef INCLUDE_SNMP
#else
#endif
#ifdef TCP_WIN_SCALE
/* default value for window scaling option */
#endif   /* TCP_WIN_SCALE */
/* protocol switch table entry, for TCP: */
#ifdef UDP_SOCKETS
#endif   /* UDP_SOCKETS */
#ifdef NET_STATS
#endif   /* NET_STATS */
extern struct queue   soq;     /* A socket queue */
extern struct queue   mbufq;   /* A netport queue for in-use mbufs */
extern struct queue   mfreeq;  /* A netport queue for free mbufs */

#ifdef MULTI_HOMED
#endif   /* MULTI_HOMED */
							   /*parseip.c*/

							   /*pktalloc.c*/
extern queue   bigfreeq;    /* big free buffers */
extern queue   lilfreeq;    /* small free buffers */

extern unsigned lilbufs;      /* number of small bufs to init */
extern unsigned lilbufsiz;    /* big enough for most non-full size packets */
extern unsigned bigbufs;      /* number of big bufs to init */
extern unsigned bigbufsiz;    /* big enough for max. ethernet packet */

#ifdef NPDEBUG
#endif
							  /*q.c*/
							  /*rfc1213.c*/
#ifdef INCLUDE_SNMP
extern char     sys_descr[SYS_STRING_MAX];
extern char     sysContact[SYS_STRING_MAX];
extern char     sysLocation[SYS_STRING_MAX];
extern char     sysName[SYS_STRING_MAX];
#endif
/*snmp.c*/
/*snmp_age.c*/
#if (defined(INCLUDE_SNMP) || defined(INCLUDE_SNMPV3))
/* these can't be global in a re-entrant system */
#endif
/*snmp_aut.c*/
/*snmpport.c*/
#if (defined(INCLUDE_SNMP) || defined(INCLUDE_SNMPV3))
/*extern   u_char    varVal[MAX_TRAP_VAR]; Define only local*/ /* the actual variable data */
#endif

															   /*trap_out.c*/
#if (defined(INCLUDE_SNMP) || defined(INCLUDE_SNMPV3))
#ifdef ENABLE_SNMP_TRAPS   /* whole file is within this ifdef */
#ifdef V1_IP_SNMP_TRAPS
															   /* trap related agent values from mib2 system group */
															   /* static   oid   sysDescrOid[] = {1, 3, 6, 1, 2, 1, 1, 1, 0}; */
#endif
#endif
#endif
															   /*udp.c*/
															   /*udp_open.c*/
extern UDPCONN firstudp;

#ifndef INICHE_TIMERS
/* AP00834747 */
#error "check this: INICHE_TIMERS must be defined!"
#endif

void anyip_init_channel(void *channel)
{
	ANYIP_UNUSED_ARG(channel);
}

void anyip_deinit_channel(void *channel)
{
#ifdef IP_FRAGMENTS
	tcip_iniche_flush_reasm_cache(channel);
#else
	ANYIP_UNUSED_ARG(channel);
#endif
}

void anyip_ctick(void)
{
	cticks++;
	inet_timer(); /* Trigger ipstack internal timers and checks */
}

int anyip_init(void)
{
	/*iface.c*/
	TCIP_MEMSET(protlist, 0, sizeof(protlist));

	/*et_arp.c*/
	arpcache = (struct arptabent *)NULL;   /* last ARP entry used */
	TCIP_MEMSET(arp_table, 0, sizeof(arp_table));     /* the actual table */
													  /*iface.c*/
	TCIP_MEMSET(&netlist, 0, sizeof(netlist));          /* master list of nets, static & dynamic */
	TCIP_MEMSET(&rcvdq, 0, sizeof(rcvdq));         /* queue of all received (but undemuxed) packets */

												   /*ipport.c*/
	port_prep = NULL;
	TCIP_MEMSET(nets, 0, sizeof(nets));
	ifNumber = 0;        /* number of net interfaces */
	nclosers = 0;
	TCIP_MEMSET(&soq, 0, sizeof(soq));     /* A socket queue */
	TCIP_MEMSET(&mbufq, 0, sizeof(mbufq));   /* A netport queue for in-use mbufs */
	TCIP_MEMSET(&mfreeq, 0, sizeof(mfreeq));  /* A netport queue for free mbufs */
											  /*pktalloc.c*/
	TCIP_MEMSET(&bigfreeq, 0, sizeof(bigfreeq));    /* big free buffers */
	TCIP_MEMSET(&lilfreeq, 0, sizeof(lilfreeq));    /* small free buffers */

	lilbufs = NUMLILBUFS;      /* number of small bufs to init */
	lilbufsiz = LILBUFSIZE;    /* big enough for most non-full size packets */
	bigbufs = NUMBIGBUFS;      /* number of big bufs to init */
	bigbufsiz = BIGBUFSIZE;    /* big enough for max. ethernet packet */

#ifdef IP_FRAGMENTS  /* whole file can be ifdeffed out */
							   /* number of bytes currently used by the reassembly process - this is the sum of
							   * the lengths of the buffers used in the reassembly process */
	ipr_curr_mem = 0;
	/* head pointer to IP reassembly entry (IRE) queue */
	h_ireq = 0;
	/* next invocation of timer tick processing routine */
	ire_cticks = 0;
#endif

	sys_descr[0] = 0;
	sysContact[0] = 0;
	sysLocation[0] = 0;
	sysName[0] = 0;



#if (defined(INCLUDE_SNMP) || defined(INCLUDE_SNMPV3))
#include SNMPMIB_H_FILE
	SnmpMib.snmpEnableAuthenTraps = 2; /* AP 850537 auth.traps are disabled */
#endif

	firstudp = NULL;

#ifdef DNS_CLIENT    /* ifdef out whole file */
	dnsids = 0x1234;  /* seed for unique request IDs */

					  /* DNS client statistics: */
	dnsc_errors = 0;      /* protocol/implementation runtime errors */
	dnsc_requests = 0;    /* requests sent */
	dnsc_replies = 0;     /* replys received */
	dnsc_good = 0;        /* usable replys received */
	dnsc_tmos = 0;        /* timeouts */
	dnsc_retry = 0;       /* total retries */
#ifdef DNS_CLIENT_UPDT
	dnsc_updates = 0;     /* Dynamic DNS Updates sent */
#endif /* DNS_CLIENT_UPDT */
						  /* pending requests, can be used as a flag to spin dnsc_check() task */
	dnsc_active = 0;
	/* retry info, can be overwritten by application code */
	dns_firsttry = 4; /* time to first retry, in seconds */
	dns_trys = 5;     /* max number of retrys */
	TCIP_MEMSET(dns_servers, 0, sizeof(dns_servers));
#ifdef  DNS_CLIENT_UPDT
	TCIP_MEMSET(soa_mname, 0, sizeof(soa_mname));
#endif  /* DNS_CLIENT_UPDT */
#endif  /* dns_client */


	/*
	* main initialization of ipstack and allocation of packet queues
	*/
	tcip_inside_pktdemux = 0; /* initialize it in case of restart  */

	cticks = 0;

	agent_on = FALSE; /* Turn off interniche internal SNMP agent */

	MaxLnh = ETHHDR_SIZE;
	MaxMtu = ET_MAXLEN;

	netmain_init(); /* main init of iniche stack; allocates the packet queues  */

	tcip_iniche_store_allocated_buffers();

	return 0;
}

void anyip_deinit()
{
	/*
	* Free the stored allocated buffers and packages
	*/
	tcip_iniche_free_allocated_buffers();

	/* Cleanup routing table buffer */
#ifdef IP_ROUTING
	if (rt_mib != LSA_NULL)
	{
		RT_FREE(rt_mib);
	}
#endif /* IP_ROUTING */

	/*
	*  Initialize the IPSTACK queues at shutdown (for possible restart)
	*/
	TCIP_MEMSET(&bigfreeq, 0, sizeof(bigfreeq));
	TCIP_MEMSET(&lilfreeq, 0, sizeof(lilfreeq));
	TCIP_MEMSET(&mfreeq, 0, sizeof(mfreeq));
	TCIP_MEMSET(&netlist, 0, sizeof(netlist));
}

/* internal functions */
/*---------------------------------------------------------------------------*/
/* preventing stack-overflow with indirect calling of pktdemux() */

void SignalPktDemux(void)
{
	if (tcip_inside_pktdemux != 0)  /* check re-entrancy flag                       */
		return;               /* do not reenter pktdemux(), packet will wait. */
	tcip_inside_pktdemux++;        /* set re-entrany flag                          */
	pktdemux();               /* process low level packet input               */
	tcip_inside_pktdemux--;        /* clear re-entracy flag                        */
}

static void tcip_iniche_store_allocated_buffers(void)
{
	int lvi;
	PACKET tmp;
	struct mbuf* pNetBuf;

	/* Store the allocated Packets and buffers from big buffer queue    */
	TCIP_ASSERT(bigfreeq.q_len == NUMBIGBUFS);
	tmp = (PACKET)bigfreeq.q_head;
	for (lvi = 0; lvi < NUMBIGBUFS; lvi++)
	{
		tcip_Buffers.bigPackets[lvi] = tmp;
		if (tmp != LSA_NULL)
		{
			tcip_Buffers.bigBuffers[lvi] = tmp->nb_buff;
			tmp = tmp->next;
		}
	}

	/* Store the allocated Packets and buffers from small buffer queue  */
	TCIP_ASSERT(lilfreeq.q_len == NUMLILBUFS);
	tmp = (PACKET)lilfreeq.q_head;
	for (lvi = 0; lvi < NUMLILBUFS; lvi++)
	{
		tcip_Buffers.lilPackets[lvi] = tmp;
		if (tmp != LSA_NULL)
		{
			tcip_Buffers.lilBuffers[lvi] = tmp->nb_buff;
			tmp = tmp->next;
		}
	}

	/* Store the allocated netport packet buffer (used for UDP)    */
	TCIP_ASSERT(mfreeq.q_len == NETPORT_BUFFERS_MAX);
	pNetBuf = (struct mbuf*)mfreeq.q_head;
	for (lvi = 0; lvi < NETPORT_BUFFERS_MAX; lvi++)
	{
		tcip_Buffers.netportBuffers[lvi] = pNetBuf;
		pNetBuf = pNetBuf->next;
	}
}

/*---------------------------------------------------------------------------*/

static LSA_VOID tcip_iniche_free_allocated_buffers(void)
{
	int lvi;

	/* Free the stored allocated Packets and big buffers    */
	for (lvi = 0; lvi < NUMBIGBUFS; lvi++)
	{
		if (tcip_Buffers.bigBuffers[lvi] != LSA_NULL)
		{
			void *ptr = tcip_Buffers.bigBuffers[lvi];
#ifdef NPDEBUG
			ptr = (char*)ptr - ALIGN_TYPE; /* what a mess, see pk_init() */
			TCIP_ASSERT(*(long*)ptr == 'MMMM');
#endif
			BB_FREE(ptr);
			tcip_Buffers.bigBuffers[lvi] = LSA_NULL;
		}
		if (tcip_Buffers.bigPackets[lvi] != LSA_NULL)
		{
			NB_FREE(tcip_Buffers.bigPackets[lvi]);
			tcip_Buffers.bigPackets[lvi] = LSA_NULL;
		}
	}

	/* Free the stored allocated Packets and small buffers  */
	for (lvi = 0; lvi < NUMLILBUFS; lvi++)
	{
		if (tcip_Buffers.lilBuffers[lvi] != LSA_NULL)
		{
			void *ptr = tcip_Buffers.lilBuffers[lvi];
#ifdef NPDEBUG
			ptr = (char*)ptr - ALIGN_TYPE; /* what a mess, see pk_init() */
			TCIP_ASSERT(*(long*)ptr == 'MMMM');
#endif
			LB_FREE(ptr);
			tcip_Buffers.lilBuffers[lvi] = LSA_NULL;
		}

		if (tcip_Buffers.lilPackets[lvi] != LSA_NULL)
		{
			NB_FREE(tcip_Buffers.lilPackets[lvi]);
			tcip_Buffers.lilPackets[lvi] = LSA_NULL;
		}
	}

	/* Free the stored allocated netport buffers    */
	for (lvi = 0; lvi < NETPORT_BUFFERS_MAX; lvi++)
	{
		if (tcip_Buffers.netportBuffers[lvi] != LSA_NULL)
		{
			NB_FREE(tcip_Buffers.netportBuffers[lvi]);
			tcip_Buffers.netportBuffers[lvi] = LSA_NULL;
		}
	}
}
