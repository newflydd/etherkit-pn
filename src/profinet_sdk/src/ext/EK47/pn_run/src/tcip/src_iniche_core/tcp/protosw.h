/*
 * v3.1 August 30, 2011 tcp/protosw.h
 * Distributed per ref EMEA2011-3
 */
/*
 * FILENAME: protosw.h
 *
 * Copyright 1997-2007 By InterNiche Technologies, Inc. All rights reserved.
 *
 * Protocol switch table.
 *
 * Each protocol has a handle initializing one of these structures,
 * which is used for protocol-protocol and system-protocol communication.
 *
 * A protocol is called through the pr_init entry before any other.
 * Thereafter it is called every 200ms through the pr_fasttimo entry and
 * every 500ms through the pr_slowtimo for timer based actions.
 * The system will call the pr_drain entry if it is low on space and
 * this should throw away any non-critical data.
 *
 * Protocols pass data between themselves as chains of mbufs using
 * the pr_input and pr_output hooks.  Pr_input passes data up (towards
 * UNIX) and pr_output passes it down (towards the imps); control
 * information passes up and down on pr_ctlinput and pr_ctloutput.
 * The protocol is responsible for the space occupied by any the
 * arguments to these entries and must dispose it.
 *
 * The userreq routine interfaces protocols to the system and is
 * described below.
 *
 * MODULE: INET
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

#ifndef PROTOSW_H
#define  PROTOSW_H   1

struct ifnet;  /* pre-declaration to supress warnings */

struct protosw
{
   short    pr_type;             /* socket type used for */
   short    pr_protocol;         /* protocol number */
   short    pr_flags;            /* see below */

   /* protocol-protocol hooks */
   void     (*pr_input) __P ((struct mbuf *, NET ifp));

#ifdef CTL_INPUT
   /* input to protocol (from below) */
   void     (*pr_ctlinput)__P ((int, struct sockaddr *));   
   /* control input (from below) */
#endif
#ifdef CTL_OUTPUT
   /* control output (from above) */
   int      (*pr_ctloutput) __P((int, struct socket *, int, int, void *));
#endif

   /* user-protocol hook */
   int      (*pr_usrreq) __P ((struct socket *, struct mbuf *, struct mbuf *));

   /* user request: see list below */
   /* utility hooks */
   void     (*pr_init) __P ((void));  /* initialization hook */
#ifdef DO_DELAY_ACKS
   int      (*pr_fasttimo) __P ((void));  /* fast timeout (200ms) */
#endif
      void     (*pr_slowtimo) __P ((void));  /* slow timeout (500ms) */
};

/*
 * Values for pr_flags
 */
#define     PR_ATOMIC   0x01     /* exchange atomic messages only */
#define     PR_ADDR        0x02  /* addresses given with messages */
/* in the current implementation, PR_ADDR needs PR_ATOMIC to work */
#define     PR_CONNREQUIRED   0x04     /* connection required by protocol */
#define     PR_WANTRCVD    0x08     /* want PRU_RCVD calls */
#define     PR_RIGHTS   0x10     /* passes capabilities */

/*
 * The arguments to usrreq are:
 *   (*protosw[].pr_usrreq)(up, req, m, nam, opt);
 * where up is a (struct socket *), req is one of these requests,
 * m is a optional mbuf chain containing a message,
 * nam is an optional mbuf chain containing an address,
 * and opt is a pointer to a socketopt structure or nil.
 * The protocol is responsible for disposal of the mbuf chain m,
 * the caller is responsible for any space held by nam and opt.
 * A non-zero return from usrreq gives an
 * UNIX error number which should be passed to higher level software.
 */
#define     PRU_ATTACH        0  /* attach protocol to up */
#define     PRU_DETACH        1  /* detach protocol from up */
#define     PRU_BIND       2  /* bind socket to address */
#define     PRU_LISTEN        3  /* listen for connection */
#define     PRU_CONNECT       4  /* establish connection to peer */
#define     PRU_ACCEPT        5  /* accept connection from peer */
#define     PRU_DISCONNECT       6  /* disconnect from peer */
#define     PRU_SHUTDOWN      7  /* won't send any more data */
#define     PRU_RCVD       8  /* have taken data; more room now */
#define     PRU_SEND       9  /* send this data */
#define     PRU_ABORT      10 /* abort (fast DISCONNECT, DETATCH) */
#define     PRU_CONTROL       11 /* control operations on protocol */
#define     PRU_SENSE      12    /* return status into m */
#define     PRU_RCVOOB        13 /* retrieve out of band data */
#define     PRU_SENDOOB       14 /* send out of band data */
#define     PRU_SOCKADDR      15 /* fetch socket's address */
#define     PRU_PEERADDR      16 /* fetch peer's address */
#define     PRU_CONNECT2      17 /* connect two sockets */

/* begin for protocols internal use */
#define     PRU_FASTTIMO      18 /* 200ms timeout */
#define     PRU_SLOWTIMO      19 /* 500ms timeout */
#define     PRU_PROTORCV      20 /* receive from below */
#define     PRU_PROTOSEND        21 /* send to below */
#define     PRU_NREQ       21

#ifdef PRUREQUESTS
char *   prurequests[]  =  {
   "ATTACH",   "DETACH",   "BIND",      "LISTEN",
   "CONNECT",   "ACCEPT",   "DISCONNECT",   "SHUTDOWN",
   "RCVD",      "SEND",      "ABORT",   "CONTROL",
   "SENSE",   "RCVOOB",   "SENDOOB",   "SOCKADDR",
   "PEERADDR",   "CONNECT2",   "FASTTIMO",   "SLOWTIMO",
   "PROTORCV",   "PROTOSEND",
};
#endif


/*
 * The arguments to the ctlinput routine are
 *   (*protosw[].pr_ctlinput)(cmd, arg);
 * where cmd is one of the commands below, and arg is
 * an optional argument (caddr_t).
 *
 * N.B. The IMP code, in particular, pressumes the values
 *      of some of the commands; change with extreme care.
 * TODO:
 *   spread out codes so new ICMP codes can be
 *   accomodated more easily
 */

#define  PRC_IFDOWN           0     /* interface transition */
#define  PRC_ROUTEDEAD        1     /* select new route if possible */
#define  PRC_QUENCH           4     /* some said to slow down */
#define  PRC_MSGSIZE          5     /* message size forced drop */
#define  PRC_HOSTDEAD         6     /* normally from IMP */
#define  PRC_HOSTUNREACH      7     /* ditto */
#define  PRC_UNREACH_NET      8     /* no route to network */
#define  PRC_UNREACH_HOST     9     /* no route to host */
#define  PRC_UNREACH_PROTOCOL 10    /* dst says bad protocol */
#define  PRC_UNREACH_PORT     11    /* bad port # */
#define  PRC_UNREACH_NEEDFRAG 12    /* IP_DF caused drop */
#define  PRC_UNREACH_SRCFAIL  13    /* source route failed */
#define  PRC_REDIRECT_NET     14    /* net routing redirect */
#define  PRC_REDIRECT_HOST    15    /* host routing redirect */
#define  PRC_REDIRECT_TOSNET  16    /* redirect for type of service & net */
#define  PRC_REDIRECT_TOSHOST 17    /* redirect for tos & host */
#define  PRC_TIMXCEED_INTRANS 18    /* packet lifetime expired in transit */
#define  PRC_TIMXCEED_REASS   19    /* lifetime expired on reass q */
#define  PRC_PARAMPROB        20    /* header incorrect */
#define  PRC_NCMDS            21


#ifdef PRCREQUESTS
char   * prcrequests[]  =  {
   "IFDOWN", "ROUTEDEAD", "#2", "#3",
   "QUENCH", "MSGSIZE", "HOSTDEAD", "HOSTUNREACH",
   "NET-UNREACH", "HOST-UNREACH", "PROTO-UNREACH", "PORT-UNREACH",
   "FRAG-UNREACH", "SRCFAIL-UNREACH", "NET-REDIRECT", "HOST-REDIRECT",
   "TOSNET-REDIRECT", "TOSHOST-REDIRECT", "TX-INTRANS", "TX-REASS",
   "PARAMPROB"
};
#endif


/*
 * The arguments to ctloutput are:
 *   (*protosw[].pr_ctloutput)(req, so, level, optname, optval);
 * req is one of the actions listed below, so is a (struct socket *),
 * level is an indication of which protocol layer the option is intended.
 * optname is a protocol dependent socket option request,
 * optval is a pointer to a mbuf-chain pointer, for value-return results.
 * The protocol is responsible for disposal of the mbuf chain *optval
 * if supplied,
 * the caller is responsible for any space held by *optval, when returned.
 * A non-zero return from usrreq gives an
 * UNIX error number which should be passed to higher level software.
 */

#define     PRCO_GETOPT    0
#define     PRCO_SETOPT    1
#define     PRCO_NCMDS     2
#ifdef PRCOREQUESTS

char   * prcorequests[] =
{
   "GETOPT",
   "SETOPT",
};
#endif

extern   struct protosw *  pffindtype  __P ((int, int));
extern   struct protosw *  pffindproto __P ((int, int, int));

#endif   /* PROTOSW_H */

/* end of file protosw.h */


