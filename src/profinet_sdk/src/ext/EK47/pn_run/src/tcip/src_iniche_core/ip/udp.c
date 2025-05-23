/*
 * v3.1 February 9, 2012 ip/udp.c (SNMP bugfix)
 * v3.1 August 30, 2011 ip/udp.c
 * Distributed per ref EMEA2011-3
 */
/*
 * FILENAME: udp.c
 *
 * Copyright 1997- 2000 By InterNiche Technologies Inc. All rights reserved
 *
 * UDP send and receive routines
 *
 * MODULE: INET
 *
 * ROUTINES: udpdemux(), udp_send(), udpswap(), udp_socket(), 
 * ROUTINES: udp_alloc(), udp_maxalloc(), udp_free(), udp_stats()
 *
 * PORTABLE: yes
 */

/* Additional Copyrights: */
/* Portions Copyright 1990, 1993 by NetPort Software. */
/* Portions Copyright 1986 by Carnegie Mellon  */
/* Portions Copyright 1984 by the Massachusetts Institute of Technology  */

#define LTRC_ACT_MODUL_ID 24 /* INICHE_TCIP_ITGR added for LSA traces */

#include "ipport.h"
#include "tcpport.h" /* INICHE_TCIP_ITGR added for InterNiche's SNMP bugfix */
#include "q.h"
#include "netbuf.h"
#include "net.h"
#include "ip.h"
#include "icmp.h"
#include "udp.h"

#ifdef IP_MULTICAST
#include "tcpport.h"
#endif
#ifdef USE_IGMPV3
#include "igmp3.h"
#endif


struct UdpMib  udp_mib;    /* udp stats block */

extern   UDPCONN  firstudp;

#ifdef IP_MULTICAST
extern int udp_soinput(PACKET pkt, void *so_ptr);
PACKET udp_clone(PACKET);
#endif


/* FUNCTION: udpdemux()
 *
 * This routine handles incoming UDP packets. They're handed to it by 
 * the internet layer. It demultiplexes the incoming packet based on 
 * the local port and upcalls the appropriate routine. 
 *
 * 
 * PARAM1: PACKET p
 *
 * RETURNS: 0 if OK or ENP error code
 */

int
udpdemux(PACKET p)
{
   struct ip * pip;  /* IP header below this layer */
   struct udp *   pup;  /* UDP header of this layer */
   struct ph   php;     /* pseudo header for checksumming */
   UDPCONN con;      /* UDP connection for table lookup */
   unsigned short osum, xsum; /* scratch checksum holders */
   unsigned plen; /* packet length */
   int   e, err = 0;    /* general error holder */
   int   demuxed = FALSE;
   u_short app_data_len; /* length of application data */

   /* First let's verify that it's a valid UDP packet. */
   pip = ip_head(p);       /* we'll need IP header info */
   pup = (struct udp*)ip_data(pip);   /*  also need UDP header */
   plen = htons(pup->ud_len);

   if (plen > p->nb_plen)
   {
#ifdef   NPDEBUG
      if ((NDEBUG & UPCTRACE) && (NDEBUG & TPTRACE))
#if 1 /* INICHE_TCIP_ITGR added LSA trace */
			TCIP_INICHE_TRACE_02(0, LSA_TRACE_LEVEL_ERROR
				, "UDP: bad len pkt: rcvd: %u, hdr: %u."
				, p->nb_plen, (unsigned int)(htons(pup->ud_len) + UDPLEN)
				);
#else
         dprintf("UDP: bad len pkt: rcvd: %u, hdr: %u.\n",
                 p->nb_plen, (unsigned int)(htons(pup->ud_len) + UDPLEN));
#endif
#endif
      udp_mib.udpInErrors++;
      udp_free(p);
      return ENP_BAD_HEADER;
   }

   osum = pup->ud_cksum;
   /* did other guy use checksumming? */
   if (osum)
   {
      if (plen & 1) ((char *)pup)[plen] = 0;
         php.ph_src = p->fhost;
      php.ph_dest = pip->ip_dest;
      php.ph_zero = 0;
      php.ph_prot = UDP_PROT;
      php.ph_len  = pup->ud_len;

      pup->ud_cksum = cksum(&php, sizeof(struct ph)>>1);
      xsum = ~cksum(pup, (plen+1)>>1);
      if (!xsum)
         xsum = 0xffff;
      pup->ud_cksum = osum;
      if (xsum != osum)
      {
#ifdef   NPDEBUG
         if ((NDEBUG & UPCTRACE) && (NDEBUG & TPTRACE))
         {
#if 1 /* INICHE_TCIP_ITGR added LSA trace */
			TCIP_INICHE_TRACE_06(0, LSA_TRACE_LEVEL_ERROR
				, "UDPDEMUX: bad xsum %04x right %04x from %u.%u.%u.%u"
				, osum, xsum, (unsigned)(p->fhost&0xff),(unsigned)((p->fhost>>8)&0xff),(unsigned)((p->fhost>>16)&0xff),(unsigned)(p->fhost>>24)
				);
#else
            dprintf("UDPDEMUX: bad xsum %04x right %04x from %u.%u.%u.%u\n",
                    osum, xsum, PUSH_IPADDR(p->fhost));
#endif
            if (NDEBUG & DUMP)
               ip_dump(p);
         }
#endif
         udp_mib.udpInErrors++;
         udp_free(p);
         return ENP_BAD_HEADER;
      }
   }

#if (BYTE_ORDER == LITTLE_ENDIAN)
   udpswap(pup);
#endif

   /* Prior to upcall, adjust nb_prot for size of IP and UDP headers */
   e = (sizeof(struct udp) + ip_hlen(pip));
   p->nb_plen -= e;
   p->nb_prot += e;

   /* drop any trailing padding */
   app_data_len = plen - sizeof(struct udp);
   if (p->nb_plen > app_data_len)
   {
      p->nb_plen = app_data_len;
   }

#ifdef   NPDEBUG
   if ((NDEBUG & UPCTRACE) && (NDEBUG & TPTRACE))
   {
#if 1 /* INICHE_TCIP_ITGR added LSA trace */
			TCIP_INICHE_TRACE_07(0, LSA_TRACE_LEVEL_CHAT
				, "UDP: pkt[%u] from %u.%u.%u.%u:%d to %d"
				, plen, (unsigned)(p->fhost&0xff),(unsigned)((p->fhost>>8)&0xff),(unsigned)((p->fhost>>16)&0xff),(unsigned)(p->fhost>>24), pup->ud_srcp, pup->ud_dstp
				);
#else
      dprintf("UDP: pkt[%u] from %u.%u.%u.%u:%d to %d\n",
       plen, PUSH_IPADDR(p->fhost), pup->ud_srcp, pup->ud_dstp);
#endif
   }
#endif

   /* check for special cases - we have a built-in snmp agent and 
    * echo server. We do SNMP before trying the demux table so 
    * external application programs can examine SNMP packets that 
    */
#ifdef PREBIND_AGENT
#ifdef INCLUDE_SNMPV3      /* If V3 is present, call SNMPV3 upcall */
   if (pup->ud_dstp == SNMP_PORT)
   {
      udp_mib.udpInDatagrams++;
      UNLOCK_NET_RESOURCE(NET_RESID);
      e = v3_udp_recv(p, pup->ud_srcp);      /* upcall imbedded snmp agent */
      LOCK_NET_RESOURCE(NET_RESID);
      if (e != ENP_NOT_MINE)
         return(e);
      /* else SNMP pkt was not for imbedded agent, fall to try demux table */
   }
#else                   /* Else call V1's upcall, if V1 is present */
   if (pup->ud_dstp == SNMP_PORT)
   {
      udp_mib.udpInDatagrams++;
      UNLOCK_NET_RESOURCE(NET_RESID);
      e = snmp_upc(p, pup->ud_srcp);      /* upcall imbedded snmp agent */
      LOCK_NET_RESOURCE(NET_RESID);
      if (e != ENP_NOT_MINE)
         return(e);
      /* else SNMP pkt was not for imbedded agent, fall to try demux table */
   }
#endif   /* INCLUDE_SNMPV3 */
#endif   /* PREBIND_AGENT */

   /* run through the demux table and try to upcall it */
   for (con = firstudp; con; con = con->u_next)
   {
      e = 0;
#ifdef IP_V6
      /* we only want to check UDP-over-IPv4 connections */
      if (!(con->u_flags & UDPCF_V4))
         continue;
#endif

      /* enforce all three aspects of tuple matching */
      if (con->u_lport && (con->u_lport != pup->ud_dstp))
         continue;
      if (con->u_fport && (con->u_fport != pup->ud_srcp))
         continue;
      if (con->u_fhost && (con->u_fhost != p->fhost))
         continue;

      /* 
       * Is this an exact (or wildcard) address match, multicast, broadcast, or
       * subnet directed broadcast? 
       */  
      if (!((0 == con->u_lhost) ||
            (con->u_lhost && (con->u_lhost == pip->ip_dest)) ||
            (pip->ip_dest == INADDR_BROADCAST) ||
            (pip->ip_dest == p->net->n_netbr)  ||
            IN_MULTICAST(ntohl(pip->ip_dest))))
      {
         continue;            /* no - continue */
      }

#if 1 /* INICHE_TCIP_ITGR see ID 954482 */
		if ((0 != con->u_lhost) /* not bound to INADDR_ANY ? */
			&& (con->u_lhost != p->net->n_ipaddr)) /* and socket not on the same net the packet came in ? */
		{
			continue;
		}
#endif

      /* if this endpoint has been bound to a local interface address,
       * make sure the packet was received on that interface address
       */
      if (!IN_MULTICAST(ntohl(pip->ip_dest)))
      {
         /* we found the right connection */
         udp_mib.udpInDatagrams++;
         if (con->u_rcv)         /* if upcall address is set... */
         {
#if 0 /* INICHE_TCIP_ITGR lock kept while calling basic io routine      */
            UNLOCK_NET_RESOURCE(NET_RESID);
#endif
            e = ((*con->u_rcv)(p, con->u_data));   /* upcall it */
#if 0 /* INICHE_TCIP_ITGR lock kept while calling basic io routine      */
            LOCK_NET_RESOURCE(NET_RESID);
#endif
         }
         else
            e = ENP_LOGIC;

         /* if error occurred in upcall or there was no upcall handler
            its up to this routine to free the packet buffer */
         if (e)
         {
            udp_mib.udpInErrors++;
            udp_free(p);
         }

         return(e);
      }
#ifdef IP_MULTICAST
#ifdef INCLUDE_TCP
      else
      {
         struct socket *soptr = (struct  socket *)con->u_data;

         /* In the case of multicast, check if there is multicast membership
          * attached to this socket and if so, is the incoming packet
          * addressed to the multicast address
          */
         if (con->u_rcv && soptr->inp_moptions)    /* sanity check */
         {
            int  i;
            struct ip_moptions *imo = soptr->inp_moptions;

            for (i = 0; i < imo->imo_num_memberships; ++i)
            {
               if ((imo->imo_membership[i]->inm_netp == p->net) &&
                   (imo->imo_membership[i]->inm_addr == pip->ip_dest))
               {
                  break;
               }
            }

            if (i < imo->imo_num_memberships)
            {
#ifdef USE_IGMPV3
               struct in_addr_source *msfsrc;

               /* receive data from any source */
               if ((imo->imo_msf[i]->msf_grpjoin != 0) &&
                   (imo->imo_msf[i]->msf_blkhead->numsrc == 0))
               {
                  goto end_of_search;
               }

               /* receive data from source in allow list */
               SLIST_FOREACH(imo->imo_msf[i]->msf_head->head, msfsrc)
               {
                  if (msfsrc->addr < pip->ip_src)
                     continue;
                  if (msfsrc->addr > pip->ip_src)
                     /* Terminate search, as there will be no match */
                     break;

                  goto end_of_search;
               }

               if (imo->imo_msf[i]->msf_blkhead->numsrc == 0)
               {
                  /* Drop data from source not in allow list when socket in include mode */
                  e = ENP_NOT_MINE;
                  goto end_of_search;
               }

               /* drop data from source in block list */
               SLIST_FOREACH(imo->imo_msf[i]->msf_blkhead->head, msfsrc)
               {
                  if (msfsrc->addr < pip->ip_src)
                     continue;
                  if (msfsrc->addr == pip->ip_src)
                  {
                     /* source blocked */
                     e = ENP_NOT_MINE;
                     break;
                  }

                  /* Terminate search, as there will be no match */
                  break;
               }

               end_of_search:
#endif /* USE_IGMPV3 */
               if (e != ENP_NOT_MINE)
               {
                  p->inuse++;
                  udp_mib.udpInDatagrams++;
#if 0 /* INICHE_TCIP_ITGR lock kept while calling basic io routine      */
				  UNLOCK_NET_RESOURCE(NET_RESID);
#endif
                  e = ((*con->u_rcv)(p, con->u_data));   /* upcall it */
#if 0 /* INICHE_TCIP_ITGR lock kept while calling basic io routine      */
                  LOCK_NET_RESOURCE(NET_RESID);
#endif

                  /* if error occurred in upcall or there was no upcall handler
                     its up to this routine to free the packet buffer */
                  if (e)
                  {
                     udp_mib.udpInErrors++;
                     udp_free(p);
                  }
                  if (e != ENP_NOT_MINE)
                     demuxed = TRUE;
                  else p->inuse--;
               }
            }
         }
         else
         {
            e = ENP_LOGIC;
         }

         if ((err == 0) && (e != ENP_NOT_MINE))
            err = e;    /* save first non-zero error code */
      }
#endif /* IP_MULTICAST */
#endif /* INCLUDE_TCP */
   }

   /* if we upcalled the packet, we are done */
   if (demuxed)
   {
      udp_free(p);            /* free the original packet */
      return (err);
   }

#ifdef DNS_CLIENT
   /* see if this is a DNS reply packet */
   if (pup->ud_srcp == DNS_PORT)    /* sent from DNS port? */
   {
      UNLOCK_NET_RESOURCE(NET_RESID);
      e = dns_upcall(p, pup->ud_dstp);    /* upcall dns client code */
      LOCK_NET_RESOURCE(NET_RESID);
      udp_mib.udpInDatagrams++;
      if (e != ENP_NOT_MINE)  /* if mine, return here */
      {
         /* the DNS upcall does not appear to free the packet, so
            we have to do it */
         udp_free(p);
         return(e);
      }
   }
#endif   /* DNS_CLIENT */

#ifdef RIP_SUPPORT
   /* see if this is a RIP packet */
   if (pup->ud_srcp == RIP_PORT)    /* sent from DNS port? */
   {
      UNLOCK_NET_RESOURCE(NET_RESID);
      e = rip_udp_recv(p, pup->ud_dstp);     /* upcall RIP code */
      LOCK_NET_RESOURCE(NET_RESID);
      /* note that the RIP upcall does appear to free the packet,
         so we don't do it */
      udp_mib.udpInDatagrams++;
      if (e != ENP_NOT_MINE)  /* if mine, return here */
         return(e);
   }
#endif   /* RIP_SUPPORT */

#ifdef UDP_ECHO_SVR
   /* finally, see if this is an echo packet */
   if (pup->ud_dstp == 7)     /* echo port */
   {
      udp_mib.udpInDatagrams++;
      UNLOCK_NET_RESOURCE(NET_RESID);
      e = udp_send(pup->ud_srcp, 7, p);      /* echo it */
      LOCK_NET_RESOURCE(NET_RESID);
      return(e);
   }
#endif   /* UDP_ECHO_SVR */

   /* Fall to here if packet is not for us. Check if the packet was 
    * sent to an ip broadcast address. If it was, don't send a 
    * destination unreachable. 
    */
   if ((pip->ip_dest == 0xffffffffL) ||   /* Physical cable broadcast addr*/
       (pip->ip_dest == p->net->n_netbr) ||   /* All subnet broadcast */
#if 0 /* INICHE_TCIP_ITGR see 957200 */
       (pip->ip_dest == p->net->n_netbr42) || /* All subnet bcast (4.2bsd) */
#endif
       (pip->ip_dest == p->net->n_subnetbr) || /* Our subnet broadcast */
       (IN_MULTICAST(ntohl(pip->ip_dest)))) /* multicast address */
   {
#ifdef   NPDEBUG
      if ((NDEBUG & UPCTRACE) && (NDEBUG & TPTRACE))
#if 1 /* INICHE_TCIP_ITGR added LSA trace */
			TCIP_INICHE_TRACE_00(0, LSA_TRACE_LEVEL_NOTE
				, "UDP: ignoring ip broadcast or multicast"
				);
#else
         dprintf("UDP: ignoring ip broadcast or multicast\n");
#endif
#endif
      udp_mib.udpInErrors++;
      udp_free(p);
      return ENP_NOT_MINE;
   }

#ifdef   NPDEBUG
   if ((NDEBUG & UPCTRACE) && (NDEBUG & TPTRACE))
   {
#if 1 /* INICHE_TCIP_ITGR added LSA trace */
			TCIP_INICHE_TRACE_01(0, LSA_TRACE_LEVEL_NOTE
				, "UDP: unexpected port %04x"
				, pup->ud_dstp);
#else
      dprintf("UDP: unexpected port %04x\n", pup->ud_dstp);
#endif
      if (NDEBUG & DUMP) 
         ip_dump(p);
   }
#endif

#ifdef FULL_ICMP
   /* send destination unreachable.  Swap back all the swapped information */
   /* so that the destun packet format is correct */

#if (BYTE_ORDER == LITTLE_ENDIAN)
   udpswap(pup);
#endif   /* BYTE_ORDER */
   
   icmp_destun(p->fhost, p->net->n_ipaddr, pip, DSTPORT, p->net);
#endif   /* FULL_ICMP */

   udp_mib.udpNoPorts++;
   udp_mib.udpInDatagrams++; /* INICHE_TCIP_ITGR AP00610150 patch */
   udp_free(p);
   return ENP_NOT_MINE;
}

/* FUNCTION: udp_send()
 *
 * udp_send() - Prepend a udp header on a packet, checksum it and 
 * pass it to IP for sending. PACKET p should have fhost set to 
 * target IP address, nb_prot pointing to udpdata, & nb_plen set 
 * before call. If you expect to get any response to this packet you 
 * should have opened a UDPCONN with udp_open() prior to calling 
 * this. 
 *
 * 
 * PARAM1: unshort fport
 * PARAM2: unshort lport
 * PARAM3: PACKET p
 *
 * RETURNS: Returns 0 if sent OK, else non-zero error code if error 
 * detected. 
 */

int
udp_send(unshort fport, unshort lport, PACKET p)
{
   struct udp* pup;
   struct ph   php;
   struct ip * pip;
   int         udplen;
   int         e;
   ip_addr     src_ip;    /* source IP, for checksumming purposes */

#ifdef   NPDEBUG
   if (NDEBUG & (INFOMSG|TPTRACE))
#if 1 /* INICHE_TCIP_ITGR added LSA trace */
			TCIP_INICHE_TRACE_07(0, LSA_TRACE_LEVEL_CHAT
				, "UDP: pkt [%u] %04x -> %u.%u.%u.%u:%04x"
				, p->nb_plen, lport, (unsigned)(p->fhost&0xff),(unsigned)((p->fhost>>8)&0xff),(unsigned)((p->fhost>>16)&0xff),(unsigned)(p->fhost>>24), fport
				);
#else
      dprintf("UDP: pkt [%u] %04x -> %u.%u.%u.%u:%04x\n", p->nb_plen, lport,
    PUSH_IPADDR(p->fhost), fport);
#endif
#endif

   LOCK_NET_RESOURCE(NET_RESID);
   /* prepend UDP header to upper layer's data */
   p->nb_prot -= sizeof(struct udp);
   pup = (struct udp*)p->nb_prot;
   udplen = p->nb_plen + sizeof(struct udp);
   p->nb_plen = udplen;
   if (udplen & 1)
      ((char *)pup)[udplen] = 0;

   pup->ud_len = (unshort)udplen;   /* fill in the UDP header */
   pup->ud_srcp = lport;
   pup->ud_dstp = fport;

#if (BYTE_ORDER == LITTLE_ENDIAN)
   udpswap(pup);
#endif   /* BYTE_ORDER */
   
#ifdef MULTI_HOMED
   /* getting the source IP address for a broadcast is a bit tricky: */
   if (p->fhost == 0xffffffff)
   {
      if (!p->net)
      {
         dtrap();    /* programmer forgot to select iface */
         /* it would appear that the callers of udp_send() expect it
            to do cleanup on failure, so free the packet buffer here */
         udp_free(p);
         UNLOCK_NET_RESOURCE(NET_RESID);
         return ENP_NO_IFACE;
      }
      src_ip = p->net->n_ipaddr;
   }
   else
#endif   /* MULTI_HOMED */

   /* set the IP addresses in the IP header. The pseudo header used for
    * checksumming overlays the addresses on the IP header area in the
    * buffer, so setting them there (which we need to do for cksum
    * anyway) sets up for IP too.
    */
   src_ip = ip_mymach(p->fhost);
   php.ph_src = src_ip;
   php.ph_dest = p->fhost;

#ifdef NO_UDP_CKSUM
   /* If no UDP checksum support, just zero the checksum field */
   pup->ud_cksum = 0;
#else
   /* finish filling in the pseudo header required for checksumming */
   php.ph_zero = 0;
   php.ph_prot = UDP_PROT;
   php.ph_len = pup->ud_len;
   pup->ud_cksum = cksum(&php, sizeof(struct ph)>>1);
   pup->ud_cksum = ~cksum(pup, (udplen+1)>>1);
   if (pup->ud_cksum == 0)
      pup->ud_cksum = 0xffff;
#endif

   /* need to fill in IP addresses at this layer too */
   pip = (struct ip *)(p->nb_prot - sizeof(struct ip));
   pip->ip_src = src_ip;
   pip->ip_dest = p->fhost;

   udp_mib.udpOutDatagrams++;

   p->nb_plen = udplen;       /* nb_prot was adjusted above */
   e = ip_write(UDP_PROT, p);
   UNLOCK_NET_RESOURCE(NET_RESID);
   return e;
}


/* FUNCTION: udpswap()
 *
 * udpswap(udp header) - swap the fields in a udp header for transmission
 * on the net. This is not needed on big endian CPUs.
 * 
 * PARAM1: struct udp *pup
 *
 * RETURNS: void
 */

#if (BYTE_ORDER == LITTLE_ENDIAN)
void
udpswap(struct udp *pup)
{

   pup->ud_srcp = htons(pup->ud_srcp);
   pup->ud_dstp = htons(pup->ud_dstp);
   pup->ud_len = htons(pup->ud_len);
   pup->ud_cksum = htons(pup->ud_cksum);
}
#endif   /* BYTE_ORDER */


/* FUNCTION: udp_socket()
 *
 * udp_socket() - Select a port number at random, but avoid reserved 
 * ports from 0 thru 1024. Also leave range from 1025 thru 1199 
 * available for explicit application use. 
 *
 * 
 * PARAM1: void
 *
 * RETURNS: Returns a useable port 
 * number in local endian 
 */

#if 1 /* INICHE_TCIP_ITGR made configurable */
#ifndef  MINSOCKET
#error "MINSOCKET must be defined in ipport.h"
#elif  MINSOCKET > 0xC000
#error "logic below will not work - see &0x3fff"
#endif
#else
#define  MINSOCKET   1200
#endif
static unshort usocket = 0;   /* next socket to grab */

unshort
udp_socket(void)
{
   UDPCONN tmp;

   if (usocket < MINSOCKET)
   {
      /* logic for for init and after wraps */
#if 1 /* INICHE_TCIP_ITGR */
      usocket = (unshort)(cticks & 0x3fff);
#else
      usocket = (unshort)(cticks & 0x7fff);
#endif
      if (usocket < MINSOCKET)
         usocket += MINSOCKET;
   }
   /* scan existing connections, making sure socket isn't in use */
   for (tmp = firstudp; tmp; tmp = tmp->u_next)
   {
      if (tmp->u_lport == usocket)
      {
         usocket++;     /* bump socket number */
         tmp = firstudp;   /* restart scan */
         continue;
      }
   }
   return usocket++;
}



/* FUNCTION: udp_alloc()
 *
 * udp_alloc(sizes) - returns a pointer to a packet buffer big enough 
 * for the specified sizes.
 *
 * 
 * PARAM1: int datalen
 * PARAM2: int optlen
 *
 * RETURNS:  Returns buffer, or NULL in no buffer was available. 
 */

PACKET
udp_alloc(int datalen, int optlen)
{
   int   len;
   PACKET p;

   len = (datalen + sizeof(struct udp) + 1) & ~1;
   LOCK_NET_RESOURCE(FREEQ_RESID);
   p = pk_alloc(len + UDPHDRSLEN + optlen);
   UNLOCK_NET_RESOURCE(FREEQ_RESID);

   if (p != (PACKET)NULL)
   {
      /* set prot pointers past end of UDP header */
      len = UDPHDRSLEN + optlen + sizeof(struct udp);
      p->nb_prot = p->nb_buff + len;
      p->nb_plen = 0;
   }

   return (p);
}

#ifdef IP_MULTICAST

/* FUNCTION: udp_clone()
 *
 * Make a copy of a UDP packet
 *
 * PARAM1: PACKET             packet to clone
 *
 * RETURN: PACKET             new packet or NULL
 */

PACKET
udp_clone(PACKET pkt)
{
   PACKET  newpkt;
   int length = pkt->nb_plen;
   int optlen = 0;

   newpkt = udp_alloc(length, optlen);
   if (newpkt)
   {
      char *buff = newpkt->nb_buff;
      char *prot = newpkt->nb_prot;
      unsigned blen = newpkt->nb_blen;
      unsigned flags = newpkt->flags;

      /* clone the packet header */
      MEMCPY(newpkt, pkt, sizeof(struct netbuf));

      /* fix up the packet-specific fields */
      newpkt->nb_buff = buff;
      newpkt->nb_prot = prot;
      newpkt->nb_blen = blen;
      newpkt->flags = flags | (pkt->flags & (PKF_BCAST | PKF_MCAST));
      newpkt->inuse = 1;
#ifdef IP_V6
      if (pkt->ip6_hdr)
         newpkt->ip6_hdr = (struct ipv6 *)
                 (newpkt->nb_buff + ((char *)pkt->ip6_hdr - pkt->nb_buff));
#endif  /* IP_V6 */

      /* clone the packet data */
      MEMCPY(newpkt->nb_prot - sizeof (struct udp), pkt->nb_prot - sizeof (struct udp), length + sizeof (struct udp));
   }

   return (newpkt);
}

#endif  /* IP_MULTICAST */


/* FUNCTION: udp_maxalloc()
 *
 * udp_maxalloc() - returns the largest allocation possible
 *                  using udp_alloc()
 *
 * RETURNS: an int indicating the largest allocation possible
 *          using udp_alloc(); i.e. if the sum of udp_alloc()
 *          arguments datalen + optlen is greater than the
 *          returned value, the allocation will fail
 */

int
udp_maxalloc(void)
{
#ifdef HEAPBUFS
   /* since UDPHDRSLEN also includes space for the link-layer header, the
    * expression used below causes a slight underestimation
    */
   return (0xFFFF - (UDPHDRSLEN + sizeof (struct udp)));
#else
   /* if heap buffers are not available, the largest size of a UDP datagram
    * is constrained by what will fit inside a big buffer */
   return ((bigbufsiz - (UDPHDRSLEN + sizeof (struct udp))));
#endif
}


/* FUNCTION: udp_free()
 *
 * udp_free() - frees an allocated packet buffer
 *
 * 
 * PARAM1: PACKET p
 *
 * RETURNS: void
 */

void
udp_free(PACKET p)
{
   LOCK_NET_RESOURCE(FREEQ_RESID);
   pk_free(p);
   UNLOCK_NET_RESOURCE(FREEQ_RESID);
}

#ifdef NET_STATS


/* FUNCTION: udp_stats()
 *
 * udp_stats() - print UDP stats
 *
 * 
 * PARAM1: void * pio
 *
 * RETURNS: 0
 */

int
udp_stats(void * pio)
{
   ns_printf(pio,"UDP MIB dump:\n");
   ns_printf(pio,"In: Good: %lu    No Port: %lu     Bad: %lu\n",
      udp_mib.udpInDatagrams, udp_mib.udpNoPorts, udp_mib.udpInErrors);
   ns_printf(pio,"Out: %lu\n", udp_mib.udpOutDatagrams);
   return 0;
}

/* FUNCTION: udpsock_list()
 * 
 * Display information about all the existing UDP sockets.
 *
 * PARAM1: void * pio
 *
 * RETURNS:  0 if successfull, else error code.
 */

/* for display purpose (udpsock_list()), maintain a list of
 * hardcoded UDP ports on which we are listening.
 */
static unshort sbindlist[] = {
#ifdef PREBIND_AGENT
   SNMP_PORT,
#endif
#ifdef DNS_CLIENT
   DNS_PORT,
#endif
#ifdef RIP_SUPPORT
   RIP_PORT,
#endif
#ifdef UDP_ECHO_SVR
   7,
#endif
   0     /* 0 marks the end of the list */
};

int
udpsock_list(void * pio)
{
   UDPCONN con;      /* UDP connection for table lookup */
   int i, len;
   struct socket *so = (struct socket *)NULL;

   if (!firstudp)
   {
      ns_printf(pio,"No UDP sockets\n");
      return 0;
   }

   ns_printf(pio, "Here is a list of all UDP sockets (normal/lightweight).\n");
   ns_printf(pio, "UDP sock, hosts          ,     ports,    IPv6 addrs");

   for (con = firstudp; con; con = con->u_next)
   {
#ifdef IP_V4
      if (con->u_flags & UDPCF_V4)
      {
         ns_printf(pio,"\n%lx,  %u.%u.%u.%u", con, PUSH_IPADDR(con->u_fhost));
         ns_printf(pio,"->%u.%u.%u.%u ", PUSH_IPADDR(con->u_lhost));
      }
      else
         ns_printf(pio,"\n%lx,  - (v6 socket) -  ", con);
#endif
      ns_printf(pio,", %u->%u, ", con->u_lport, con->u_fport);

#ifdef IP_V6
      if (con->u_flags & UDPCF_V6)
      {
         char  ip6buf[46];
         ns_printf(pio,"%s ", print_ip6(&con->u_f6host, &ip6buf[0]));
         ns_printf(pio,"-> %s", print_ip6(&con->u_l6host, &ip6buf[0]));
      }
#endif   /* IP_V6 */
   }
   ns_printf(pio,"\n");

   len = sizeof(sbindlist)/sizeof(unshort);
   if (len > 1)
   {
      ns_printf(pio,"UDP also listens on :");
      for (i=0; i<len-1; i++)
         ns_printf(pio," %u ", sbindlist[i]);
      ns_printf(pio,"\n");
   }

#if defined (IP_V4) && defined(USE_IGMPV3)
   /* print out multicast membership information for UDP/IPv4 sockets */
   ns_printf(pio, "\nMulticast membership for UDP/IPv4 connections:\n");
   for (con = firstudp; con; con = con->u_next)
   {
      if (con->u_flags & UDPCF_V4)
      {
         so = LONG2SO((u_long) con->u_data);
      
         if (sovalid (so) && so->inp_moptions)
         {
            ns_printf (pio, "[0x%lx]: ", con);
            igmp_print_membership_socket (so->inp_moptions, pio);
         }
      }
   }
#endif /* IP_V4 and USE_IGMPV3 */

   return 0;
}

#endif   /* NET_STATS */

#if defined(INCLUDE_SNMP) && defined(IP_V4)

/* FUNCTION: udptbl_search()
 *
 * This function searches for a matching entry in the UDP connection 
 * table.
 *
 * PARAM1: struct conn *      Pointer to a connection structure
 *
 * RETURN: This function returns TRUE if it was able to find a matching
 * entry; otherwise, it returns FALSE.
 */
bool_t
udptbl_search(struct conn *cp)
{
   UDPCONN con;
   int lport = ntohs(cp->lport);

   if (!firstudp)
      return (FALSE);

   for (con = firstudp; con; con = con->u_next)
   {
      if (con->u_flags & UDPCF_V6)
         continue;
      if (cp->laddr == con->u_lhost && lport == con->u_lport)
         return (TRUE);
      else
         continue;
   }

   return (FALSE);
}

/* FUNCTION: udptbl_find_lowest()
 *
 * This function searches for the "lowest" entry in the UDP connection 
 * table.
 *
 * PARAM1: struct conn *      Pointer to a connection structure (to store
 *                            the contents (local address and local port) 
 *                            of the lowest entry
 *
 * RETURN: This function returns TRUE if it was able to find the lowest
 * entry; otherwise, it returns FALSE.
 */
bool_t
udptbl_find_lowest(struct conn *cp)
{
   UDPCONN ucp, ucp_lowest = NULL;
   u_short lport;
   u_char conn_lowest[UDP_TBL_IDX_LEN]; /* "lowest" entry */
   u_char conn_curr[UDP_TBL_IDX_LEN]; /* current entry */

   if (!firstudp)
      return (FALSE);

   for (ucp = firstudp; ucp; ucp = ucp->u_next)
   {
      if (ucp->u_flags & UDPCF_V6)
         continue;

      MEMCPY(&conn_curr[UDP_TBL_IDX_LADDR_OFF], &ucp->u_lhost, sizeof(ip_addr));
      lport = htons(ucp->u_lport);
      MEMCPY(&conn_curr[UDP_TBL_IDX_LPORT_OFF], &lport, sizeof(u_short));
      if (ucp_lowest == NULL)
      {
         MEMCPY(conn_lowest, conn_curr, UDP_TBL_IDX_LEN);
         ucp_lowest = ucp;
      }
      else
      {
         if (MEMCMP(conn_curr, conn_lowest, UDP_TBL_IDX_LEN) < 0)
         {
            MEMCPY(conn_lowest, conn_curr, UDP_TBL_IDX_LEN);
            ucp_lowest = ucp;
         }
      }
   }

   if (ucp_lowest)
   {
      cp->laddr = ucp_lowest->u_lhost;
      cp->lport = htons(ucp_lowest->u_lport);
      return (TRUE);
   }

   return (FALSE);
}

/* FUNCTION: udptbl_find_highest()
 *
 * This function searches for the "highest" entry in the UDP connection 
 * table.
 *
 * PARAM1: struct conn *      Pointer to a connection structure (to store
 *                            the contents (local address and local port) 
 *                            of the highest entry
 *
 * RETURN: This function returns TRUE if it was able to find the highest
 * entry; otherwise, it returns FALSE.
 */
bool_t
udptbl_find_highest(struct conn *cp)
{
   UDPCONN ucp, ucp_highest = NULL; /* sp_highest -> ucp_highest */
   u_short lport;
   u_char conn_highest[UDP_TBL_IDX_LEN]; /* "highest" entry */
   u_char conn_curr[UDP_TBL_IDX_LEN]; /* current entry */

   if (!firstudp)
      return (FALSE);

   for (ucp = firstudp; ucp; ucp = ucp->u_next)
   {
      if (ucp->u_flags & UDPCF_V6)
         continue;

      MEMCPY(&conn_curr[UDP_TBL_IDX_LADDR_OFF], &ucp->u_lhost, sizeof(ip_addr));
      lport = htons(ucp->u_lport);
      MEMCPY(&conn_curr[UDP_TBL_IDX_LPORT_OFF], &lport, sizeof(u_short));

      if (ucp_highest == NULL)
      {
         MEMCPY(conn_highest, conn_curr, UDP_TBL_IDX_LEN);
         ucp_highest = ucp;
      }
      else
      {
         if (MEMCMP(conn_curr, conn_highest, UDP_TBL_IDX_LEN) > 0)
         {
            MEMCPY(conn_highest, conn_curr, UDP_TBL_IDX_LEN);
            ucp_highest = ucp;
         }
      }
   }

   if (ucp_highest)
   {
      cp->laddr = ucp_highest->u_lhost;
      cp->lport = htons(ucp_highest->u_lport);
      return (TRUE);
   }

   return (FALSE);
}

/* FUNCTION: udptbl_find_next_highest()
 *
 * This function searches for the "next highest" entry in the UDP connection 
 * table.
 *
 * PARAM1: struct conn *      Pointer to a connection structure representing
 *                            the entry for which a successor is desired
 * PARAM2: struct conn *      Pointer to a connection structure to store
 *                            the contents (local address and local port) 
 *                            of the next highest entry
 *
 * RETURN: This function returns TRUE if it was able to find the next highest
 * entry; otherwise, it returns FALSE.
 */
bool_t
udptbl_find_next_highest(struct conn *cp, struct conn *nextcp)
{
   UDPCONN ucp, ucp_nh = NULL;
   u_short lport;
   u_char conn_nh[UDP_TBL_IDX_LEN]; /* "next highest" entry */
   u_char conn_curr[UDP_TBL_IDX_LEN]; /* current entry */
   u_char conn_user[UDP_TBL_IDX_LEN]; /* caller-provided entry */

   if (!firstudp)
      return (FALSE);

   MEMCPY(&conn_user[UDP_TBL_IDX_LADDR_OFF], &cp->laddr, sizeof(ip_addr));
   MEMCPY(&conn_user[UDP_TBL_IDX_LPORT_OFF], &cp->lport, sizeof(u_short));

   for (ucp = firstudp; ucp; ucp = ucp->u_next)
   {
      if (ucp->u_flags & UDPCF_V6)
         continue;

      MEMCPY(&conn_curr[UDP_TBL_IDX_LADDR_OFF], &ucp->u_lhost, sizeof(ip_addr));
      lport = htons(ucp->u_lport);
      MEMCPY(&conn_curr[UDP_TBL_IDX_LPORT_OFF], &lport, sizeof(u_short));

      if (!ucp_nh)
      {
         if (MEMCMP(conn_curr, conn_user, UDP_TBL_IDX_LEN) > 0)
         {
            MEMCPY(conn_nh, conn_curr, UDP_TBL_IDX_LEN);
            ucp_nh = ucp;
         }
      }
      else
      {
         if (MEMCMP(conn_curr, conn_user, UDP_TBL_IDX_LEN) > 0 &&
             MEMCMP(conn_curr, conn_nh, UDP_TBL_IDX_LEN) < 0)
         {
            MEMCPY(conn_nh, conn_curr, UDP_TBL_IDX_LEN);
            ucp_nh = ucp;
         }
      }
   }

   if (ucp_nh)
   {
      nextcp->laddr = ucp_nh->u_lhost;
      nextcp->lport = htons(ucp_nh->u_lport);
      return (TRUE);
   }

   return (FALSE);
}

#endif /* INCLUDE_SNMP and IP_V4 */


/* end of file udp.c */

