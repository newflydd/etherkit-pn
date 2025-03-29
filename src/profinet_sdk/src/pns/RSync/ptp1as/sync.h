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

#ifndef GPTP_SYNC_H
#define GPTP_SYNC_H

/* State-Machine states */
#define CS_STATE_INIT          0
#define CS_STATE_GRAND_MASTER  1
#define CS_STATE_SLAVE         2

/* indices for time-stamp value array ts[]*/
// Timestamp at which sync frame was received
// Note that indexes 0..5 are part od delaymsr.c

#define TX_SYNC_FW             6
#define RX_SYNC_FROM_MASTER    7
// Master time contained in sync or sync follow up frame
#define RX_SYNC_FU_FROM_MASTER 8
#define SYN_DELAY_SUM   9
#define LOCALTIME 							10

/* indices for time-stamp value array sync[]*/
/* Arrival time of frame at port */
#define TX_MASTER_SENT_TIME     0
#define OFFSET_TO_MASTER        1
#define DIFFCORRVAL             2    // Used in syncdel
#define DIFF_RX_TS_CURRENT_TIME 2
#define TARGET_ACTUAL_TIME      3
#define MASTER_DELAY 						4
#define TX_MASTER_ARRIVAL_TIME  5
#define TS_RX_LOCAL_TIME        6 
#define CLOCKDRIFT   						7 

/* sync state machine related functions */
void initCS             (struct gPTPd* gPTPd);
void reloadCSTimeout    (struct gPTPd* gPTPd);
void csHandleEvent      (struct gPTPd* gPTPd, int evtId);
void csSetState         (struct gPTPd* gPTPd, bool gmMaster);
void csHandleStateChange(struct gPTPd* gPTPd, int toState);

/* frame send functions, if we are master */
uint32_t sendSync     (struct gPTPd* gPTPd);
void     sendSyncFlwup(struct gPTPd* gPTPd);

/* sync frame forwarding */
void forwardSync  (struct gPTPd* gPTPd, uint8_t * frameBuffer, uint32_t isSync);

/* Sync algorithm related */
void clock_settime    (clockid_t clkid, struct timespec* ts);

/* HW switch related */
uint32_t ethsw_get_time_sec(uint8_t timer);

/* inform PNO statck about sync state changes */
void syncChanged(struct gPTPd* gPTPd, LSA_UINT32 state);

#endif
