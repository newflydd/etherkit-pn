/*
 * v3.1 August 30, 2011 tcp/tcp_timr.h
 * Distributed per ref EMEA2011-3
 */
/*
 * FILENAME: tcp_timr.h
 *
 * Copyright 1997-2007 By InterNiche Technologies, Inc. All rights reserved.
 *
 *
 * MODULE: TCP
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

#ifndef _TCP_TIMER_H
#define  _TCP_TIMER_H   1

#if 1 /* INICHE_TCIP_ITGR */
extern int tcip_ConnectionTimeout;
extern int tcip_RetransmitCntMax;
extern int tcip_RetransmitTimeout;
#endif

/*
 * Definitions of the TCP timers.  These timers are counted
 * down PR_SLOWHZ times a second.
 */
#define     TCPT_NTIMERS   4

#define     TCPT_REXMT     0     /* retransmit */
#define     TCPT_PERSIST   1     /* retransmit persistance */
#define     TCPT_KEEP      2     /* keep alive */
#define     TCPT_2MSL      3     /* 2*msl quiet time timer */

/*
 * The TCPT_REXMT timer is used to force retransmissions.
 * The TCP has the TCPT_REXMT timer set whenever segments
 * have been sent for which ACKs are expected but not yet
 * received.  If an ACK is received which advances tp->snd_una,
 * then the retransmit timer is cleared (if there are no more
 * outstanding segments) or reset to the base value (if there
 * are more ACKs expected).  Whenever the retransmit timer goes off,
 * we retransmit one unacknowledged segment, and do a backoff
 * on the retransmit timer.
 *
 * The TCPT_PERSIST timer is used to keep window size information
 * flowing even if the window goes shut.  If all previous transmissions
 * have been acknowledged (so that there are no retransmissions in progress),
 * and the window is too small to bother sending anything, then we start
 * the TCPT_PERSIST timer.  When it expires, if the window is nonzero,
 * we go to transmit state.  Otherwise, at intervals send a single byte
 * into the peer's window to force him to update our window information.
 * We do this at most as often as TCPT_PERSMIN time intervals,
 * but no more frequently than the current estimate of round-trip
 * packet time.  The TCPT_PERSIST timer is cleared whenever we receive
 * a window update from the peer.
 *
 * The TCPT_KEEP timer is used to keep connections alive.  If an
 * connection is idle (no segments received) for TCPTV_KEEP_INIT amount of time,
 * but not yet established, then we drop the connection.  Once the connection
 * is established, if the connection is idle for TCPTV_KEEP_IDLE time
 * (and keepalives have been enabled on the socket), we begin to probe
 * the connection.  We force the peer to send us a segment by sending:
 *   <SEQ=SND.UNA-1><ACK=RCV.NXT><CTL=ACK>
 * This segment is (deliberately) outside the window, and should elicit
 * an ack segment in response from the peer.  If, despite the TCPT_KEEP
 * initiated segments we cannot elicit a response from a peer in TCPT_MAXIDLE
 * amount of time probing, then we drop the connection.
 */

#define     TCP_TTL        30    /* default time to live for TCP segs */

/*
 * Time constants.
 */

extern   int   TCPTV_MSL;  /* max seg lifetime - NOT a constant! */

#define     TCPTV_SRTTBASE    0     /* base roundtrip time; if 0, no idea yet */

#define  TCPTV_SRTTDFLT    (3*PR_SLOWHZ)        /* assumed RTT if no info */
#define  TCPTV_PERSMIN     (5*PR_SLOWHZ)        /* retransmit persistance */
#define  TCPTV_PERSMAX     (60*PR_SLOWHZ)       /* maximum persist interval */
#if 1 /* INICHE_TCIP_ITGR */
#define  TCPTV_KEEP_INIT    tcip_ConnectionTimeout
#else
#define  TCPTV_KEEP_INIT   (75*PR_SLOWHZ)       /* initial connect keep alive */
#endif

#if 1 /* INICHE_TCIP_ITGR */
#define  TCPTV_KEEP_IDLE   (30*PR_SLOWHZ)       /* dflt time before probing */
#else
#define  TCPTV_KEEP_IDLE   (120*60*PR_SLOWHZ)   /* dflt time before probing */
#endif

#if 1 /* INICHE_TCIP_ITGR */
#define  TCPTV_KEEPINTVL   (1*PR_SLOWHZ)        /* default probe interval */
#else
#define  TCPTV_KEEPINTVL   (75*PR_SLOWHZ)       /* default probe interval */
#endif

#if 1 /* INICHE_TCIP_ITGR */
#define  TCPTV_KEEPCNT     tcip_RetransmitCntMax
#else
#define  TCPTV_KEEPCNT     8                    /* max probes before drop */
#endif

#define  TCPTV_MIN         (1*PR_SLOWHZ)        /* minimum allowable value */

/*Code removed by gmg 23 Aug 18 RQ 3163413, interniche: initialisation fixed*/

#define  TCPTV_REXMTMAX    (64*PR_SLOWHZ)       /* max allowable REXMT value */
															

#define  TCP_LINGERTIME    120                  /* linger at most 2 minutes */
#define  TCP_MAXRXTSHIFT   12                   /* maximum retransmits */

/* Maximum amount to delay an ACK in ms, assuming there is no reason the
 * ACK should be sent immediately. Granularity is one ctick (TPS).
 * Value > 300ms may cause other side to retransmit forcing an immediate ACK
 */
#if 1 /* INICHE_TCIP_ITGR see AP01519172 */
#define  TCP_DELAYACKTIME   (100 * TPS/1000)   /* INICHE_TCIP_ITGR: As used in V3.0 and lower */
#else
#define  TCP_DELAYACKTIME   (200 * TPS/1000)   /* Commonly used value */
#endif
#define  TCP_MAXDELAYACK    (500 * TPS/1000)   /* Max allowed by TCP standard */

#ifdef   TCPTIMERS
char *   tcptimers[] =
{
   "REXMT",
   "PERSIST",
   "KEEP",
   "2MSL"
};
#endif

/*
 * Force a time value to be in a certain range.
 */
#define   TCPT_RANGESET(tv, value, tvmin, tvmax) { \
   (tv) = (short)(value); \
   if ((tv) < (tvmin)) \
      (tv) = (short)(tvmin); \
   else if ((tv) > (tvmax)) \
      (tv) = (short)(tvmax); \
}

extern   int   tcp_keepidle;     /* time before keepalive probes begin */
extern   int   tcp_keepintvl;    /* time between keepalive probes */
extern   int   tcp_maxidle;      /* time to drop after starting probes */
extern   int   tcp_ttl;          /* time to live for TCP segs */

extern   unsigned char  tcp_backoff[TCP_MAXRXTSHIFT   +  1];

#endif   /* _TCP_TIMER_H */

/* end of file tcp_timr.h */


