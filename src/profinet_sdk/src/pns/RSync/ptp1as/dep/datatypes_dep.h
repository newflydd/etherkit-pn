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

#ifndef DATATYPES_DEP_H_
#define DATATYPES_DEP_H_

#ifdef __ICCARM__
#include <time64.h>
#else
#include <time.h>
#endif
#include "ether_config.h"
#include "constants_dep.h"

struct ethhdr {
	unsigned char	h_dest[ETH_ALEN];	 /* destination eth addr	*/
	unsigned char	h_source[ETH_ALEN];	/* source ether addr	*/
	uint16_t		  h_proto;		/* packet type ID field	*/
} __attribute__((packed));

#ifdef __ICCARM__
typedef uint8_t clockid_t;
#endif
typedef int     ssize_t;       

/** "ETHE" in ASCII.  Used to determine if the control block is open. */
#define ETHER_OPEN     (0x45544845U)
#define PTPCLK         125000000

struct timeval {
   long tv_sec;     /* seconds */
   long tv_usec;    /* microseconds */
};

/* forward decl. */
struct gPTPd;
                                
//PROTOS
ssize_t sendto(void *const buf, size_t len, int idx, struct timespec * timeStamp, struct gPTPd * gPTP);

unsigned int get_port_num( uint8_t port_bit );
unsigned int        sleep(unsigned int seconds);

int clock_gettime(clockid_t clk_id, struct timespec *tp);

#endif /*DATATYPES_DEP_H_*/
