/***********************************************************************************************************************
 * Copyright [2021-2024] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics Corporation and/or its affiliates and may only
 * be used with products of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.
 * Renesas products are sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for
 * the selection and use of Renesas products and Renesas assumes no liability.  No license, express or implied, to any
 * intellectual property right is granted by Renesas.  This software is protected under all applicable laws, including
 * copyright laws. Renesas reserves the right to change or discontinue this software and/or this documentation.
 * THE SOFTWARE AND DOCUMENTATION IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND
 * TO THE FULLEST EXTENT PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY,
 * INCLUDING WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE
 * SOFTWARE OR DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR
 * DOCUMENTATION (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER,
 * INCLUDING, WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY
 * LOST PROFITS, OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

#include "r_ether_rzt2.h"
#include "gptpcmn.h"
#include "pnorsyncif.h"
#include "dep/datatypes_dep.h"
#include "r_ether_switch_rzt2_ext.h"
#include "ecos_ertec_os_utils.h"
#include "lsa_cfg.h"
#include "lsa_sys.h"
#include "bspadapt.h"
#include "os.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
/* For syslog frames */
#include "debug.h"

__attribute__ ((always_inline)) inline u64 gptp_getCurrMilliSecTS(void)
{
  return xTaskGetTickCount() * portTICK_PERIOD_MS;
}

#ifdef GPTP_PROTO
void gptp_initTxBuf(struct gPTPd* gPTPd)
{
	struct ethhdr  *eh = (struct ethhdr *)  gPTPd->txBufSync;
	struct gPTPHdr *gh = (struct gPTPHdr *)&gPTPd->txBufSync[sizeof(struct ethhdr)];

	memset(gPTPd->txBufSync, 0, GPTP_TX_BUF_SIZE);

	/* Fill in the Ethernet header */
	eh->h_dest[0] = 0x01;
	eh->h_dest[1] = 0x80;
	eh->h_dest[2] = 0xC2;
	eh->h_dest[3] = 0x00;
	eh->h_dest[4] = 0x00;
	eh->h_dest[5] = 0x0E;
	
	Bsp_GetPortMacAddr(&eh->h_source[0], gPTPd->port + 1); 

	/* Fill in Ethertype field */
	eh->h_proto = htons(ETH_P_1588);

	/* Fill common gPTP header fields */
	gh->h.f.b1.tsSpec      = GPTP_TRANSPORT_L2;
	gh->h.f.b2.ptpVer      = GPTP_VERSION_NO;
	gh->h.f.srcPortIden[0] = eh->h_source[0];
	gh->h.f.srcPortIden[1] = eh->h_source[1];
	gh->h.f.srcPortIden[2] = eh->h_source[2];
	gh->h.f.srcPortIden[3] = 0xFF;
	gh->h.f.srcPortIden[4] = 0xFE;
	gh->h.f.srcPortIden[5] = eh->h_source[3];
	gh->h.f.srcPortIden[6] = eh->h_source[4];
	gh->h.f.srcPortIden[7] = eh->h_source[5];
	gh->h.f.sourceportid   = htons(gPTPd->port);
	
	memcpy(gPTPd->txBufSyFu, gPTPd->txBufSync, GPTP_TX_BUF_SIZE);
	memcpy(gPTPd->txBufDeRq, gPTPd->txBufSync, GPTP_TX_BUF_SIZE);
	memcpy(gPTPd->txBufDeRp, gPTPd->txBufSync, GPTP_TX_BUF_SIZE);
	memcpy(gPTPd->txBufReFu, gPTPd->txBufSync, GPTP_TX_BUF_SIZE);
	memcpy(gPTPd->txBufAnno, gPTPd->txBufSync, GPTP_TX_BUF_SIZE);
}
#else
void gptp_initTxBuf(struct gPTPd* gPTPd)
{	
	void prepareDelayReq      (struct gPTPd* gPTPd);
	void prepareDelayResp     (struct gPTPd* gPTPd);
	void prepareDelayRespFlwUp(struct gPTPd* gPTPd);
	void prepareSyncFlwup     (struct gPTPd* gPTPd);
	
	prepareDelayReq      (gPTPd);
	prepareDelayResp     (gPTPd);
	prepareDelayRespFlwUp(gPTPd);	
	prepareSyncFlwup     (gPTPd);
}
#endif

void gptp_resetTimer(struct gPTPd* gPTPd, u32 timerId)
{
	if(gPTPd->timers[timerId].timeInterval != 0)
	{
		gPTPd->timers[timerId].lastTS = gptp_getCurrMilliSecTS();
	}
}

void gptp_stopTimer(struct gPTPd* gPTPd, u32 timerId)
{
	gPTPd->timers[timerId].timeInterval = 0;
	gPTPd->timers[timerId].timerEvt     = GPTP_TIMER_INVALID;
	gPTPd->timers[timerId].lastTS       = 0;
}

void gptp_startTimer(struct gPTPd* gPTPd, u32 timerId, u16 timeInterval, u32 timerEvt)
{
	gPTPd->timers[timerId].timeInterval = timeInterval;
	gPTPd->timers[timerId].timerEvt     = timerEvt;
	gPTPd->timers[timerId].lastTS       = gptp_getCurrMilliSecTS();
}

// Note that second value is the larger one
// diffsign is -1 if result is negative, result contains always positive numbers
int gptp_timespec_absdiff(struct timespec *start, struct timespec *stop, struct timespec *result)
{
    int diffsign = 1;

    if (stop->tv_sec > start->tv_sec) 
		{
      gptp_timespec_diff(start, stop, result);
    } 
		else if(stop->tv_sec < start->tv_sec) 
		{
      gptp_timespec_diff(stop, start, result);
      diffsign = -1;
    }
		else if(stop->tv_nsec > start->tv_nsec) 
		{
      gptp_timespec_diff(start, stop, result);
    } 
		else 
		{
      gptp_timespec_diff(stop, start, result);
      diffsign = -1;
    }
    
    return diffsign;
}

// Calculate Stop - Start (second value must be the larger one)
void gptp_timespec_diff(struct timespec *start, struct timespec *stop, struct timespec *result)
{
	if ((stop->tv_nsec - start->tv_nsec) < 0) 
	{ // stop before start
		result->tv_sec  = stop->tv_sec  - start->tv_sec  - 1;
		result->tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
	} 
	else 
	{
		result->tv_sec  = stop->tv_sec  - start->tv_sec;
		result->tv_nsec = stop->tv_nsec - start->tv_nsec;
	}
	
	return;
}

void gptp_timespec_sum(struct timespec *start, struct timespec *stop, struct timespec *result)
{
	if ((stop->tv_nsec + start->tv_nsec) >= 1000000000) 
	{
		result->tv_sec  = stop->tv_sec + start->tv_sec + 1;
		result->tv_nsec = (stop->tv_nsec + start->tv_nsec) - 1000000000;
	} 
	else 
	{
		result->tv_sec  = stop->tv_sec  + start->tv_sec;
		result->tv_nsec = stop->tv_nsec + start->tv_nsec;
	}
	
	return;
}

void gptp_copyTSFromBuf(struct timespec *ts, u8 *src)
{
  struct gPTPTS *srcTS = (struct gPTPTS *)src;

  ts->tv_sec  = (u64)htonl((srcTS->s.lsb + ((u64)srcTS->s.msb << 32)));
  ts->tv_nsec = htonl(srcTS->ns);
}

void gptp_copyTSToBuf(struct timespec *ts, u8 *dest)
{
  struct gPTPTS *destTS = (struct gPTPTS *)dest;

  destTS->s.msb = htons((u16)((u64)ts->tv_sec >> 32));
  destTS->s.lsb = htonl((u32)ts->tv_sec);
  destTS->ns    = htonl((u32)ts->tv_nsec);
}

// Returns 1 if TS ok, otherwise 0
uint8_t getTxTS(struct gPTPd* gPTPd, struct timespec* ts, uint16_t idx)
{
	extern void *   txTsQueue[NUM_PORTS];
    
	if(0 == (EDD_RQB_NRT_SEND_TYPE*) OsQueRecv(gPTPd->txTsQueue[idx], 20UL))
	{
		//ERROR
#if PrintfDebug
    if (gPTPd->port == 1)
      printeth("P1: TX-Timestamp missing");
    else
      printeth("P2: TX-Timestamp missing");
      
#endif
		return 0;
	}

	return 1;
}

void getRxTS(struct gPTPd* gPTPd, struct timespec* ts)
{
  ts->tv_nsec = gPTPd->RSyncReceive.RxTime;
}


