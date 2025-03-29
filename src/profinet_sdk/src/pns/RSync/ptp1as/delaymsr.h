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

#ifndef GPTP_DELAY_MSR_H
#define GPTP_DELAY_MSR_H


/* state during boot */
#define DM_STATE_INIT			0
/* state after boot - operational */
#define DM_STATE_IDLE			1

/* state after sending a delay request */
#define DM_STATE_DELAY_RESP_WAIT	      2
/* state after receiving delay response (2 step mode) */
#define DM_STATE_DELAY_RESP_FLWUP_WAIT	3

/* index for timestamp array ts[] */
#define TX_DELAY_REQ 0
#define REMOTE_PORT_TIME_T2 1
/* depature time of remote delay response */
#define REMOTE_PORT_TIME_T3 2
#define RX_DELAY_RES 3
#define RX_DELAY_REQ 4
#define TX_DELAY_RES 5
// Defined in sync.h
// Memory for the values of the last measurement for drift calculation
#define REMOTE_PORT_TIME_T2_DEL 11
#define TX_DELAY_REQ_DEL 12

/* index for timestamp differnece array (diff[])*/
#define LOCAL_REQDELAY   0
#define REMOTE_RESDELAY  1
#define LOCAL_CABLEDELAY 2

/* Spirta tiemouts */
#define DEL_REQ_TO 100

/* State machine related */
void initDM             (struct gPTPd* gPTPd);
void dmHandleEvent      (struct gPTPd* gPTPd, int evtId);
void dmHandleStateChange(struct gPTPd* gPTPd, int toState);

/* Frame sending related */
uint32_t sendDelayReq       (struct gPTPd* gPTPd);
uint32_t sendDelayResp      (struct gPTPd* gPTPd);
void     sendDelayRespFlwUp (struct gPTPd* gPTPd);

/* send line delay to EDDP */
void setLineDelay(struct gPTPd * gPTPd);

/* Sets Line- and Cable-delay to 0 */
__attribute__ ((always_inline)) inline void invalidateCableDelay(struct gPTPd* gPTPd)
{
  gPTPd->cableDelay = 0;
  gPTPd->pChUsr->pChSys->Port[gPTPd->port].SyncLineDelay = 0;
  gPTPd->pChUsr->pChSys->Port[gPTPd->port].CableDelay    = 0;

  setLineDelay(gPTPd);
}

void clear_drift_reg(int);

#endif //GPTP_DELAY_MSR_H
