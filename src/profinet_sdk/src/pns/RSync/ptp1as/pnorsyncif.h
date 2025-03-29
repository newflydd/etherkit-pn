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

#ifndef GPTP_PNOIF_H
#define GPTP_PNOIF_H

/* enable RSyncDebug for debugging */
#define RSyncDebug
//#define GPTP_PROTO

#define EDDP_LL_EV_RSYNC (0x8000U)
#define NUM_PORTS 2

/* Endianness of all FRAMEIDs are swapped */
#define FRAMEID_SYNC			  0x8000
#define FRAMEID_TIME			  0x8100
#define FRAMEID_FUSYNC			0x2000
#define FRAMEID_FUTIME			0x2100
#define FRAMEID_SYNC_FU			0x20FF
#define FRAMEID_TIME_FU			0x21FF
#define FRAMEID_ANNOUNCE		0x00FF
#define FRAMEID_DELAY_REQ		0x40FF
#define FRAMEID_DELAY_FURSP	0x41FF
#define FRAMEID_DELAY_FU		0x42FF
#define FRAMEID_DELAY_RSP		0x43FF

/* Endianness of all FRAMEIDs are NOT  swapped */
#define FRAMEID_SYNC_MIN		 0x0080
#define FRAMEID_SYNC_MAX		 0x009F
#define FRAMEID_FUSYNC_MIN	 0x0020  
#define FRAMEID_FUSYNC_MAX   0x003F
#define FRAMEID_FU_MIN		  	0xff20 
#define FRAMEID_FU_MAX			 0xff3F
#define FRAMEID_ANNOUNCE_MIN  0xFF00
#define FRAMEID_ANNOUNCE_MAX  0xFF1F 

extern void * bufferQueue;
extern void * rxTsQueue[NUM_PORTS];

#endif
