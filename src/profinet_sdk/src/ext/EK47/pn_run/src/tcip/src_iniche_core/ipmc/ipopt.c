/*
 * v3.1 August 30, 2011 ipmc/ipopt.c
 * Distributed per ref EMEA2011-3
 */
/*
 * FILENAME: ipopt.c
 *
 * Copyright 1997- 2002 By InterNiche Technologies Inc. All rights reserved
 *
 * This file contains the code to handle IP multicast options
 *
 *
 * MODULE: IPMC
 *
 * ROUTINES: ip_setmoptions(), ip_getmoptions(), 
 * ROUTINES: ip_freemoptions(), 
 *
 * PORTABLE: yes
 */

/* Additional Copyrights: */

/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. [rescinded 22 July 1999]
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "ipport.h"

#ifdef IP_MULTICAST

#include "tcpport.h"
#include "../tcp/in_pcb.h"
#include "../tcp/protosw.h"
#if defined (IGMP_V1) || defined (IGMP_V2) || defined (USE_IGMPV3)
#include "igmp_cmn.h"
#endif

#ifdef USE_IGMPV3
#include "igmp3.h"
#endif

/* base of linked lists for
 * multicast addresses indexed by
 * net index
 */

int   ip_setmoptions(int optname, struct socket *so, void *val);
int   ip_getmoptions(int optname, struct socket *so, void *arg);
void  ip_freemoptions(struct ip_moptions *imo);
void  igmp_joingroup(struct in_multi *inm);
void  igmp_leavegroup(void);
#ifdef USE_IGMPV3
int   ip_getmopt_sgaddr(void *val, int, NET *netp, ip_addr *gap, ip_addr *sap);
int process_igmpv3_opt(int optname, struct socket *so, void *val);
extern struct igmp_stats igmpstats;
#endif /* USE_IGMPV3 */

/* FUNCTION: IPADDR_TO_NETP()
 * 
 * PARAM1: ip_addr addr
 * PARAM2: NET* netp
 *
 * RETURNS: 
 */

void
IPADDR_TO_NETP(ip_addr addr, NET* netp)
{
#if 1 /* INICHE_TCIP_ITGR, see AP01546910 */
	int ifnum = addr-1; /* note: addr contains IfIndex which starts with 1 */

	*netp = if_getbynum(ifnum); /* search in netlist */ 
#else
   u_short  idx   =  0;
   *netp = nets[idx];
/*
 * If ip address is not specified, return the first intfc that supports
 * multicast
 */
   if (addr == AADDR) 
   {
      for (idx = 0; idx < ifNumber; idx++)
      {
         if (nets[idx]->n_mcastlist)
         {
            *netp = nets[idx];
            break;
         }
      }
   }
   else  /* ip address specified- return the corresponding interface */
   {
      while ((*netp != NULL) && ((*netp)->n_ipaddr != addr))
      {
         idx++;
         if (idx >= ifNumber)
         {
            *netp = NULL;
            break;
         }
         else
            *netp = nets[idx];
      }
   }
#endif
}

/*
 * Set the IP multicast options in response to user setsockopt().
 */


/* FUNCTION: ip_setmoptions()
 * 
 * PARAM1: int optname
 * PARAM2: struct socket *so
 * PARAM3: void *val
 *
 * RETURNS: 
 */

int
ip_setmoptions(int optname, struct socket * so, void * val)
{
   int   error =  0;
   u_short  i;
   struct ip_mreq *  mreq;
   struct net *netp  =  (struct net *) NULL;
   struct ip_moptions * imo   =  so->inp_moptions;
   struct ip_moptions **imop  =  &so->inp_moptions;
   ip_addr addr;
#ifdef USE_IGMPV3
   ip_addr *del_sa;
   u_short numsrc;
   int mode;
   int init;      /* indicate initial group join */
   int final;     /* indicate final group leave */
#endif /* USE_IGMPV3 */

   if (imo == NULL) 
   {
   /*
    * No multicast option buffer attached to the pcb;
    * allocate one and initialize to default values.
    */
#ifdef USE_IGMPV3    
        IGMPV3_ALLOC(imo, struct ip_moptions*, sizeof(*imo));
#else
        imo = (struct ip_moptions*)IM_ALLOC(sizeof(*imo));
#endif

      if (imo == NULL)
         return (ENOBUFS);
      *imop = imo;
      imo->imo_multicast_netp = NULL;
      imo->imo_multicast_ttl = IP_DEFAULT_MULTICAST_TTL;
      imo->imo_multicast_loop = IP_DEFAULT_MULTICAST_LOOP;
      imo->imo_num_memberships = 0;
   }

   switch (optname) 
   {

   case IP_MULTICAST_IF:
      /*
       * Select the interface for outgoing multicast packets.
       */
      addr = *(ip_addr *)val;
         /*
          * AADDR is used to remove a previous selection.
          * When no interface is selected, a default one is
          * chosen every time a multicast packet is sent.
          */
      if (addr == AADDR) 
      {
         imo->imo_multicast_netp = NULL;
         break;
      }
         /*
          * The selected interface is identified by its local
          * IP address.  Find the interface and confirm that
          * it supports multicasting.
          */
      IPADDR_TO_NETP(addr, &netp);
      if ((netp == NULL) || (netp->n_mcastlist) == NULL) 
      {
         error = EADDRNOTAVAIL;
         break;
      }
      if (addr != AADDR) 
         imo->imo_multicast_netp = netp;
      break;

   case IP_MULTICAST_TTL:
      /*
       * Set the IP time-to-live for outgoing multicast packets.
       */
      imo->imo_multicast_ttl = *(u_char *)val;
      break;

   case IP_MULTICAST_LOOP:
      /*
       * Set the loopback flag for outgoing multicast packets.
       * Must be zero or one.
       */
      if (*(u_char *)val > 1) 
      {
         error = EINVAL;
         break;
      }
      imo->imo_multicast_loop = *(u_char *)(val);
      break;

   case IP_ADD_MEMBERSHIP:
      /*
       * Add a multicast group membership.
       * Group must be a valid IP multicast address.
       */
      mreq = (struct ip_mreq *)val;
      if (!IN_MULTICAST(ntohl(mreq->imr_multiaddr))) 
      {
         error = EINVAL;
         break;
      }
      /*
       * If no interface address was provided, use the interface of
       * the route to the given multicast address.
       * For the Iniche stack implementation, look for a default
       * interface that supports multicast.
       */
      IPADDR_TO_NETP(mreq->imr_interface, &netp);
      /*
       * See if we found an interface, and confirm that it
       * supports multicast.
       */
      if (netp == NULL || (netp->n_mcastlist) == NULL) 
      {
         error = EADDRNOTAVAIL;
         break;
      }
      /*
       * See if the membership already exists or if all the
       * membership slots are full.
       */
      for (i = 0; i < imo->imo_num_memberships; ++i) 
      {
         if (imo->imo_membership[i]->inm_netp == netp &&
             imo->imo_membership[i]->inm_addr
             == mreq->imr_multiaddr)
         {
            break;
         }
      }
      if (i < imo->imo_num_memberships) 
      {
         error = EADDRINUSE;
         break;
      }
      if (i == IP_MAX_MEMBERSHIPS) 
      {
         error = ETOOMANYREFS;
         break;
      }
      /*
       * Everything looks good; add a new record to the multicast
       * address list for the given interface.
       */
#ifdef USE_IGMPV3
      /*
       * Even this request doesn't add any source filter, create
       * msf entry list. This is needed to indicate current msf state.
       */
      IMO_MSF_ALLOC(imo->imo_msf[i]);
      if (error != 0)
         break;
      init = 1;
      if ((imo->imo_membership[i] = in_addmulti2(&mreq->imr_multiaddr,
           netp, 4, 0, NULL, MCAST_EXCLUDE, init, &error)) == NULL)
      {
         IMO_MSF_FREE(imo->imo_msf[i]);
         break;
      }
#else
      if ((imo->imo_membership[i] =
          in_addmulti(&mreq->imr_multiaddr, netp, 4)) == NULL) 
      {
         error = ENOBUFS;
         break;
      }
#endif /* USE_IGMPV3 */
#ifdef USE_IGMPV3
      imo->imo_msf[i]->msf_grpjoin = 1;
#endif
      ++imo->imo_num_memberships;
      break;

   case IP_DROP_MEMBERSHIP:
      /*
       * Drop a multicast group membership.
       * Group must be a valid IP multicast address.
       */
      mreq = (struct ip_mreq *)val;
      if (!IN_MULTICAST(ntohl(mreq->imr_multiaddr))) 
      {
         error = EINVAL;
         break;
      }
      /*
       * If an interface address was specified, get a pointer
       * to its ifnet structure. If an interface address was not
       * specified, get a pointer to the first interface that
       * supports multicast.
       */
      IPADDR_TO_NETP(mreq->imr_interface, &netp);
      if (netp == NULL) 
      {
         error = EADDRNOTAVAIL;
         break;
      }

      /*
       * Find the membership in the membership array.
       */
      for (i = 0; i < imo->imo_num_memberships; ++i) 
      {
         if ((netp == NULL ||
             imo->imo_membership[i]->inm_netp == netp) &&
             imo->imo_membership[i]->inm_addr ==
             mreq->imr_multiaddr)
         {
            break;
         }
      }
      if (i == imo->imo_num_memberships) 
      {
         error = EADDRNOTAVAIL;
         break;
      }
      /*
       * Give up the multicast address record to which the
       * membership points.
       */
#ifdef USE_IGMPV3
      error = in_getmopt_source_list(imo->imo_msf[i], &numsrc, &del_sa, &mode);
      if (error != 0)
      {            
         in_undomopt_source_addr(imo->imo_msf[i], optname);
         if (del_sa != NULL)
            IGMPV3_FREE(del_sa);
         break;
      } 

      final = 1;
      in_delmulti2(imo->imo_membership[i], numsrc, del_sa, mode, final, &error);
      if (del_sa != NULL)
         IGMPV3_FREE(del_sa);
      in_freemopt_source_list(imo->imo_msf[i]);
      IMO_MSF_FREE(imo->imo_msf[i]); 
#else
      in_delmulti(imo->imo_membership[i]);
#endif /* USE_IGMPV3 */
      /*
       * Remove the gap in the membership array.
       */
      for (++i; i < imo->imo_num_memberships; ++i)
      {
         imo->imo_membership[i-1] = imo->imo_membership[i];
#ifdef USE_IGMPV3
         imo->imo_msf[i-1] = imo->imo_msf[i];
#endif
      }
      --imo->imo_num_memberships;
      break;

#ifdef USE_IGMPV3  
      case MCAST_JOIN_GROUP:
      case MCAST_LEAVE_GROUP:
      case IP_ADD_SOURCE_MEMBERSHIP:
      case MCAST_JOIN_SOURCE_GROUP:
      case IP_DROP_SOURCE_MEMBERSHIP:
      case MCAST_LEAVE_SOURCE_GROUP:
      case IP_BLOCK_SOURCE:
      case MCAST_BLOCK_SOURCE:
      case IP_UNBLOCK_SOURCE:
      case MCAST_UNBLOCK_SOURCE:
      case IP_MSFILTER:
         error = process_igmpv3_opt(optname, so, val);
         if ((error) && (IGMP_MSFON_OPS(optname) || IGMP_MSFOFF_OPS(optname)))
            ++igmpstats.igmpv3_filter_failed_count;
         else if (error == 0)
         {
            if IGMP_MSFON_OPS(optname)
               ++igmpstats.igmpv3_filter_added_count;
            else if IGMP_MSFOFF_OPS(optname)
               ++igmpstats.igmpv3_filter_deleted_count;
         }
      break;
#endif /* USE_IGMPV3 */

      default:
      error = EOPNOTSUPP;
      break;
   }

      /*
       * If all options have default values, no need to keep the mbuf.
       */
   if (imo->imo_multicast_netp == NULL &&
       imo->imo_multicast_ttl == IP_DEFAULT_MULTICAST_TTL &&
       imo->imo_multicast_loop == IP_DEFAULT_MULTICAST_LOOP &&
       imo->imo_num_memberships == 0) 
   {
#ifdef USE_IGMPV3   
      IGMPV3_FREE(*imop);
#else
      IM_FREE(*imop);
#endif
      *imop = NULL;
   }
   return (error);
}

/*
 * Return the IP multicast options in response to user getsockopt().
 */


/* FUNCTION: ip_getmoptions()
 * 
 * PARAM1: int optname
 * PARAM2: struct socket *so
 * PARAM3: void *val
 *
 * RETURNS: 
 */

int
ip_getmoptions(int optname, struct socket * so, void * val)
{
   u_char * ttl;
   u_char * loop;
   ip_addr *addr;
   struct ip_moptions*  imo   =  so->inp_moptions;

   /* The following code will be commented out for Iniche stack.
    * Don't allocate a buffer here.
    *
    *        *mp = m_get(M_WAIT, MT_SOOPTS);
    */
   switch (optname) 
   {

   case IP_MULTICAST_IF:
      addr = (ip_addr *)(val);
      if (imo == NULL || imo->imo_multicast_netp == NULL)
         *addr = AADDR;
      else
         *addr = imo->imo_multicast_netp->n_ipaddr;
      return (0);

   case IP_MULTICAST_TTL:
      ttl = (u_char *)val;
      if (imo == NULL)
         *ttl = IP_DEFAULT_MULTICAST_TTL;
      else
         *ttl = imo->imo_multicast_ttl;
      return (0);

   case IP_MULTICAST_LOOP:
      loop = (u_char *)val;
      if (imo == NULL)
         *loop = IP_DEFAULT_MULTICAST_LOOP;
      else
         *loop = imo->imo_multicast_loop;
      return (0);
#ifdef USE_IGMPV3
   case IP_MSFILTER:
      return ip_getmopt_srcfilter(so, val);
#endif
      default:
      return (EOPNOTSUPP);
   }
}

/*
 * Discard the IP multicast options.
 */


/* FUNCTION: ip_freemoptions()
 * 
 * PARAM1: struct ip_moptions *imo
 *
 * RETURNS: 
 */

void
ip_freemoptions(struct ip_moptions * imo)
{
   u_short  i;
#ifdef USE_IGMPV3
   ip_addr *del_sa = NULL;
   u_short numsrc;
   int mode;
   int final;     /* indicate final group leave */
   int error;
#endif

   if (imo != NULL) 
   {
      for (i = 0; i < imo->imo_num_memberships; ++i)
      {
#ifdef USE_IGMPV3
         error = in_getmopt_source_list(imo->imo_msf[i], &numsrc, &del_sa, &mode);
         if (error != 0)
         {            
            if (del_sa != NULL)
               IGMPV3_FREE(del_sa);
            continue;
         } 

         final = 1;
         in_delmulti2(imo->imo_membership[i], numsrc, del_sa, mode, final, &error);
         if (del_sa != NULL)
            IGMPV3_FREE(del_sa);
         in_freemopt_source_list(imo->imo_msf[i]);
         IMO_MSF_FREE(imo->imo_msf[i]); 
#else
         in_delmulti(imo->imo_membership[i]);
#endif /* USE_IGMPV3 */
      }
#ifdef USE_IGMPV3
      IGMPV3_FREE(imo);
#else
      npfree(imo);
#endif
   }
}



#endif   /* IP_MULTICAST */
