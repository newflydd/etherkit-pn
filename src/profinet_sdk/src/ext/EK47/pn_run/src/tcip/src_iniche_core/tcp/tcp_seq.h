/*
 * v3.1 August 30, 2011 tcp/tcp_seq.h
 * Distributed per ref EMEA2011-3
 */
/*
 * FILENAME: tcp_seq.h
 *
 * Copyright 1997-2007 By InterNiche Technologies, Inc. All rights reserved.
 *
 *
 * MODULE: TCP
 *
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

#ifndef _TCP_SEQ_H
#define  _TCP_SEQ_H  1

/*
 * TCP sequence numbers are 32 bit integers operated
 * on with modular arithmetic.  These macros can be
 * used to compare such integers.
 */
#define     SEQ_LT(a,b)    ((long)((a)-(b))  <  0)
#define     SEQ_LEQ(a,b)   ((long)((a)-(b))  <= 0)
#define     SEQ_GT(a,b)    ((long)((a)-(b))  >  0)
#define     SEQ_GEQ(a,b)   ((long)((a)-(b))  >= 0)


/*
 * Macros to initialize tcp sequence numbers for
 * send and receive from initial send and receive
 * sequence numbers.
 */
#define   tcp_rcvseqinit(tp) \
   (tp)->rcv_adv = (tp)->rcv_nxt = (tp)->irs + 1

#define   tcp_sendseqinit(tp) \
   (tp)->snd_una = (tp)->snd_nxt = (tp)->snd_max = (tp)->snd_up = \
       (tp)->iss

#define     TCP_ISSINCR    (long)(0x0001F4FF)   /* increment for tcp_iss each second */

#ifdef KERNEL
extern   tcp_seq     tcp_iss;    /* tcp initial send seq # */
#endif

#endif

/* end of file tcp_seq.h */


