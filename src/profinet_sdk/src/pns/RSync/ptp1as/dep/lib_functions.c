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

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

#include "constants_dep.h"
#include "datatypes_dep.h"

#include "FreeRTOS_IP.h"
#include "NetworkBufferManagement.h"

extern ether_instance_ctrl_t   ether_ctrl;

/* Make sure we can recognize the endianness. */
#if (!defined(BYTE_ORDER) || !defined(BIG_ENDIAN) || !defined(LITTLE_ENDIAN))
#warning "Unknown platform endianness!"
#endif

int clock_settime(clockid_t clkid, struct timespec* ts)
{
   R_ETHSW_Type* p_reg_switch = (void *)R_ETHSW;
  
   p_reg_switch->ATIME_SEC0 = (uint32_t)ts->tv_sec;
   p_reg_switch->ATIME0 = (uint32_t)ts->tv_nsec;
   
   return 0;
}

void clock_reset(void)
{
   R_ETHSW_Type* p_reg_switch = (void *)R_ETHSW;

   p_reg_switch->ATIME_INC0 = 0x08;  
   p_reg_switch->ATIME_OFFS_CORR0 = 0;  
   p_reg_switch->ATIME_CORR0 = 0;  
   
   return; 
}
uint8_t wechselbit = 0;
int32_t integral_value = 0;
/*
   adjusts the rate in which time progresses if freq is non-null. 
   The unit of the rate of adjustment is nanoseconds per second.
  interval: Duration of correction, typically before the next sync frame arrives
*/
int clock_adjusttime(int diffsign, struct timex *tx, uint32_t interval)
{
   R_ETHSW_Type* p_reg_switch = (void *)R_ETHSW;
   static int lastdiffsign = 0;
  
//    Value to used for performing offset corrections without changing the drift correction. 
//    The value is used in two different ways. 
//    ? If ATIME_OFFS_CORR is zero, the value is immediately added to the current timer 
//    value when this register is written. I.e. the value must be given in true 
//    nanoseconds. When a negative value is given, the value is effectively subtracted. 
//    ? If ATIME_OFFS_CORR is non-zero, the value defines how many offset corrections 
//    will be performed. After each correction, the value is decremented by 1 until it 
//    reaches zero. When it has reached zero no more offset corrections are performed. 
//    The register then is also reset to zero and if enabled an offset event interrupt is 
//    generated. 
//    Note) ATIME_OFFS_CORR must be written prior to writing this register to define the 
//    intended function. 
  
  
  volatile int brkp = 0;
  
  /* tmp contains distance between additions we need to do */
  uint32_t tmp = 125000000 / (tx->time.tv_usec);       // Achtung:  usec ist eigentlich ns  Anzahl Takte, nach denen man eine zusätzlichen ns addiert
  
     
 // if(tmp > 0)
 //   p_reg_switch->ATIME_CORR0 = tmp; 
 // else
 //   brkp = 1;         // Echter Fehler, kann nicht sein.
 
  
  
  //Offset Correction
    if(diffsign == 1)
     p_reg_switch->ATIME_INC0 = (p_reg_switch->ATIME_INC0 & 0xFF80FFFF)  | 0x00090000;   // Add correction value of +1
    else
     p_reg_switch->ATIME_INC0 = (p_reg_switch->ATIME_INC0 & 0xFF80FFFF)  | 0x00070000;   // Add correction value of -1

     uint32_t interval_ms = interval / 1000000;
     tmp = tmp/1000;
     p_reg_switch->ATIME_OFFS_CORR0 = tmp * (interval_ms-100);     // Space in between, reduce to leave time for calculations
     p_reg_switch->ATIME_OFFSET0 = tx->time.tv_usec;         // How often we increment

     //pf2("Offset: Diff: %d Corr: %d Off = %d ", diffsign, p_reg_switch->ATIME_OFFS_CORR0, p_reg_switch->ATIME_OFFSET0);

  return 0; 
  
}

/*
   adjusts the rate in which time progresses if freq is non-null. 
   The unit of the rate of adjustment is nanoseconds per second.
   duration: ointer to duration between two sync frames
   
*/
#define PTPCLK 125000000
int clock_adjustdrift(int diffsign, struct timex *tx, struct timespec *duration)
{
   R_ETHSW_Type* p_reg_switch = (void *)R_ETHSW;
   static int lastdiffsign = 0;
  
//    Value to used for performing offset corrections without changing the drift correction. 
//    The value is used in two different ways. 
//    ? If ATIME_OFFS_CORR is zero, the value is immediately added to the current timer 
//    value when this register is written. I.e. the value must be given in true 
//    nanoseconds. When a negative value is given, the value is effectively subtracted. 
//    ? If ATIME_OFFS_CORR is non-zero, the value defines how many offset corrections 
//    will be performed. After each correction, the value is decremented by 1 until it 
//    reaches zero. When it has reached zero no more offset corrections are performed. 
//    The register then is also reset to zero and if enabled an offset event interrupt is 
//    generated. 
//    Note) ATIME_OFFS_CORR must be written prior to writing this register to define the 
//    intended function. 
  
  
  volatile int brkp = 0;
  
  /* tmp is the amount of required additional 1ns corrections per second  */
  uint32_t tmp0 = (uint32_t) (duration->tv_nsec/1000000);   // convert measure duration to ms
  uint32_t drift_clks   =  (uint32_t) tx->time.tv_usec  ;   // Amount of clock drifts 
  uint32_t tmp = drift_clks * 1000/tmp0; 
  
  //pf2("Drift Diff: %ld Tmp: %d tmp0: %d ",tx->time.tv_usec, tmp, tmp0);

  if (tmp < 5) return 1;

  
 uint32_t adjcounts = PTPCLK / p_reg_switch->ATIME_CORR0;   // Get previous amount of 1 ns adjustments per second
 
  if(diffsign != 1)  {                // Slow down
    //pf2("Slowing");
    if (p_reg_switch->ATIME_CORR0 == 0) {     // First time
         p_reg_switch->ATIME_INC0  = (p_reg_switch->ATIME_INC0 & 0xFFFF80FF) | (((p_reg_switch->ATIME_INC0 & 0x7f) - 1) << 8);    // Incrementierwert rein
         p_reg_switch->ATIME_CORR0 = PTPCLK/tmp;     // Space in between
    }
    else if (  ((p_reg_switch->ATIME_INC0 & 0x007f00) >> 8) < (p_reg_switch->ATIME_INC0 & 0x07f)) {  // Slowed down already
      if ( (0x7fffffff - tmp) > adjcounts)  {
          adjcounts += tmp;
          p_reg_switch->ATIME_CORR0 = PTPCLK / adjcounts; 
      }
      else
          p_reg_switch->ATIME_CORR0 = 0;   // Need to speed up into speed region, so turn off adjustment
    }
    else {                              // In speed up mode
      if (adjcounts > tmp)  {
          adjcounts -= tmp;
          p_reg_switch->ATIME_CORR0 = PTPCLK / adjcounts; 
      }
      else {    
        //pf2("ERROR: Cannot slow down further" );
      }
    }
  }
  else {       // Speed up
    //pf2("Speeding");
    if (p_reg_switch->ATIME_CORR0 == 0) {     // First time
         p_reg_switch->ATIME_INC0  = (p_reg_switch->ATIME_INC0 & 0xFFFF80FF) | (((p_reg_switch->ATIME_INC0 & 0x7f) + 1) << 8);    // Incrementierwert rein
         p_reg_switch->ATIME_CORR0 = PTPCLK/tmp;     // Space in between
    }
    else if (  ((p_reg_switch->ATIME_INC0 & 0x007f00) >> 8) > (p_reg_switch->ATIME_INC0 & 0x07f)) {  // Sped up already
      if ( (0x7fffffff - tmp) > adjcounts)  {
          adjcounts += tmp;
          p_reg_switch->ATIME_CORR0 = PTPCLK / adjcounts; 
      }
      else {
        //pf2("ERROR: Cannot speed up further" );
      }
    }
    else {                              // In slow down mode
      if (adjcounts > tmp)  {
          adjcounts -= tmp;
          p_reg_switch->ATIME_CORR0 = PTPCLK / adjcounts; 
      }
      else     
        p_reg_switch->ATIME_CORR0 = 0; 
    }
  }
  //pf2(" Drift: CORR0: %d ",p_reg_switch->ATIME_CORR0);

  return 0; 
  
}

/* buf contains complete frame incl. dest Addresss, source address etc. Acqure buffer from FreeRTOS and copy data into that to avoid 
buffer being overwriten by next PTP activity from PTP task as networking task runs independantly from that  */
ssize_t sendto(int sockfd, void *const buf, size_t len, int flags, uint8_t stampTag)
{
  ether_instance_ctrl_t * p_instance_ctrl = (ether_instance_ctrl_t *) &ether_ctrl;

  ether_frame_mgtag_t *p_ether_frame_mgtag;
 
  //app_printf("Len is %d   mgtaglength %d  size_tlength %d \n", len, sizeof( ether_mgtag_t), sizeof(size_t)); 
  NetworkBufferDescriptor_t * pxNetworkBuffer;
    
  /* Need to remove 2 bytes from the len value as pxGetNetworkBufferWithDescriptor adds them back for some 
     reason and then alligns on the word boundary, which may lead to an increase of the frame length 
     by 5 bytes. Those frames will however be rejected by some partner devices for PTP as being too long */ 
//TODO:-OS- use queue, next line removed
//  pxNetworkBuffer = pxGetNetworkBufferWithDescriptor( len - 2, ( TickType_t ) 0U );  // Fetch descriptor with buffer
                                                                            // Need to subtract 2 from the size as the pxGetNetw... functon will add them for some rason
  if (pxNetworkBuffer == NULL) {
    //app_printf("Could not allocate Buffer for Ethernet Frame TX.  Out of memory*******************************************************\n");
    return(0);
  }
  else {
    //app_printf("TXBuffer address is %x, storage is %x\n",pxNetworkBuffer, pxNetworkBuffer->pucEthernetBuffer);
  }
  memcpy(pxNetworkBuffer->pucEthernetBuffer, buf, len);
   //     display_memory_dump( pxNetworkBuffer->pucEthernetBuffer, 40 );
//TODO:-OS- use queue, removed only next line rest was uncommented
//  ether_write((ether_ctrl_t *)&ether_ctrl, pxNetworkBuffer, len+sizeof(ether_mgtag_t));
////////////////////////////////  //fsp_err_t err = R_ETHER_Write((ether_ctrl_t *)&ether_ctrl, buf, len);//-OS-, stampTag);
////////////////////////////////
//////////////////////////////////  app_printf("sendto len %d\n",len + sizeof(ether_mgtag_t));
//////////////////////////////// // display_memory_dump( (uint8_t *)buf, len+sizeof(ether_mgtag_t) );
//////////////////////////////// 
//////////////////////////////////  if (FSP_SUCCESS == err)
//////////////////////////////////    return len;
//////////////////////////////////  else    
//////////////////////////////////     return (ssize_t) 0;
  return (pxNetworkBuffer->xDataLength);
}

//TODO:-OS- replace
int clock_gettime(clockid_t clk_id, struct timespec *tp)
{
  R_ETHSW_Type* p_reg_switch = (void *)R_ETHSW;
  
  // p_reg_switch->ATIME_CTRL0 = 0x00001001; // Capture all
  p_reg_switch->ATIME_CTRL0 = p_reg_switch->ATIME_CTRL0 | CAPTURE; 
  
  // Wait for timeer read.
  while(p_reg_switch->ATIME_CTRL0 & CAPTURE);
  
  volatile int nano = p_reg_switch->ATIME0;
  volatile int secs = p_reg_switch->ATIME_SEC0;

  tp->tv_sec  = secs;
  tp->tv_nsec = nano;
  
  return 0;
}
