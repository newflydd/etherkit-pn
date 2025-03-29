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

#include "gptpcmn.h"
#include "eddp_rzt2.h"
#include "r_ether_switch_rzt2_ext.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "ecos_ertec_os_utils.h"

static uint8_t g_time_stamp_oldest_idx = 0;

void clock_settime(clockid_t clkid, struct timespec* ts)
{
	R_ETHSW_Type* p_reg_switch = (void *)R_ETHSW;
	
	p_reg_switch->ATIME_SEC0 = (uint32_t)ts->tv_sec;
	p_reg_switch->ATIME0     = (uint32_t)ts->tv_nsec;
}

ssize_t sendto(void *const buf, size_t len, int idx, struct timespec * timeStamp, struct gPTPd * gPTP)
{
	extern void* EDDP_LL_MNG_HANDLE(uint16_t InterfaceID);
	
	extern EDDP_LL_HANDLE_PTR_TYPE pHandle;
	extern uint32_t * ptp_time_stamp_nanos[2*NUM_TX_DESCRIPTOR];
	
	OsTakeSemB(pHandle->IfSendLock);
	
	if((0U < (pHandle->dwLinkStatusAll & (gPTP->port + 1))))
	{
		ether_instance_ctrl_t *ptIfCtrl = EDDP_LL_ETH_CTRL_PTR(pHandle->pIfHandle);
		
		if (NULL != timeStamp)
		{
			xQueueReset(gPTP->txTsQueue[idx]);
	
			gPTP->RSyncSendTime[idx].hRQB      = gPTP->txTsQueue[idx];
			gPTP->RSyncSendTime[idx].hHDB      = timeStamp;
			gPTP->RSyncSendTime[idx].TxRingIdx = idx;
			gPTP->RSyncSendTime[idx].pBuffer   = buf;
			gPTP->RSyncSendTime[idx].Length    = len;
			gPTP->RSyncSendTime[idx].PortID    = gPTP->port + 1;
			
			// Timestamp frame, all DelReq Messages and related use timer 1
			uint16_t frameid = htons(((RSYNC_PTCP *)(buf))->PduHdr.FrameId.Word); 
			if ((frameid == GSY_FRAMEID_DELAY_RSP ) || (frameid == GSY_FRAMEID_DELAY_REQ ) || (frameid == GSY_FRAMEID_DELAY_FU ) || (frameid == GSY_FRAMEID_DELAY_FURSP ))
			{
				ptIfCtrl->tx_manage.tim_num  = ETHSW_TIMER1; 
			}
			else
			{
				ptIfCtrl->tx_manage.tim_num  = ETHSW_TIMER0;     
			}
			ptIfCtrl->tx_manage.flg_func = ETHER_SWITCH_TX_FUNC_TIMESTAMP | ETHER_SWITCH_TX_FUNC_TIM_NUM;
			
			/* (timestamp_id != 0xff) => Indicates PTCP/gPTP frame.*/
			ptIfCtrl->tx_manage.timestamp_id = g_time_stamp_oldest_idx;
			
			if(0 == ptp_time_stamp_nanos[g_time_stamp_oldest_idx])
			{	 
				ptp_time_stamp_nanos[g_time_stamp_oldest_idx] = (uint32_t *) &gPTP->RSyncSendTime[idx];
			}
			
			/* Using vector to avoid TS overwrite/wait when send queue piles up.*/
			if(++g_time_stamp_oldest_idx >= (2*NUM_TX_DESCRIPTOR))
			{
				g_time_stamp_oldest_idx = 0;
			}
		}
		else
		{
			ptIfCtrl->tx_manage.flg_func = ETHER_SWITCH_TX_FUNC_NON;
			
			// Default if non PTCP/gPTP frame
			ptIfCtrl->tx_manage.timestamp_id = 0xff; 
		}
		
		if(ETHER_MINIMUM_FRAME_SIZE > len)
		{
			len = ETHER_MINIMUM_FRAME_SIZE;
		}
		
		ptIfCtrl->write_port = 0;
		
		if(0U < (gPTP->port + 1))
		{
			ptIfCtrl->write_port = (1U << (gPTP->port));
		}
		
		ether_instance_t *ptIf = EDDP_LL_CAST_ETH(pHandle->pIfHandle);
		
		if(0  == ptIf->p_api->write(ptIfCtrl, (void*)buf, len))
		{
			OsGiveSemB(pHandle->IfSendLock);
			return len;
		}
		else
		{
			//ERROR
			OsGiveSemB(pHandle->IfSendLock);
			return 0;
		}
	}
	
	OsGiveSemB(pHandle->IfSendLock);
	// No link
	return 0;
}

int clock_gettime(clockid_t clk_id, struct timespec *tp)
{
  R_ETHSW_Type* p_reg_switch = (void *)R_ETHSW;

  volatile int nano;
  volatile int secs;

  if (0 == clk_id)
  {
    p_reg_switch->ATIME_CTRL0  = p_reg_switch->ATIME_CTRL0 | CAPTURE; 
    
    // Wait for timer read.
    while(p_reg_switch->ATIME_CTRL0 & CAPTURE);
    
    nano = p_reg_switch->ATIME0;
    secs = p_reg_switch->ATIME_SEC0;
  }
  else 
  {
    p_reg_switch->ATIME_CTRL1  = p_reg_switch->ATIME_CTRL1 | CAPTURE; 
    
    // Wait for timer read.
    while(p_reg_switch->ATIME_CTRL1 & CAPTURE);
    
    nano = p_reg_switch->ATIME1;
    secs = p_reg_switch->ATIME_SEC1;
  }

  tp->tv_sec  = secs;
  tp->tv_nsec = nano;
  
  return 0;
}
