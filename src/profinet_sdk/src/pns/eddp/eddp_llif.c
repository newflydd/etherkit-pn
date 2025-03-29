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

#ifdef EDDP_MESSAGE
#endif
/*****************************************************************************/

/*===========================================================================*/
/* trace module data                                                         */
/*===========================================================================*/
#define LTRC_ACT_MODUL_ID  22
#define EDDP_MODULE_ID     LTRC_ACT_MODUL_ID  /* Ids 16..31 reserved for LL */

/**
 * \addtogroup RZT2
 * \ingroup LLIF
 * @{
 */

/*===========================================================================*/
/* includes                                                                  */
/*===========================================================================*/
#include "gptpcmn.h"

#include "os.h"
#include "lsas_inc.h"
#include "lsas_int.h"
#include "lsas_dat.h"
#include "eddp_inc.h"
#include "eddp_int.h"           /* internal header  */
#include "pnio_types.h"
//#include "common_data.h"
#include "ether_config.h"
#include "eddp_types.h"
#include "eddp_core_dev.h"

#include "pnip_reg.h"
#include "pnip_xrsl.h"
#include "eddp_cmd_usr.h"
#include "eddp_hal_usr.h"

#include "gsy_int.h"
#include "gsy_low.h"

#include "nv_data.h"

#include "pndv_int.h"
#include "pndv_dat.h"
#include "pnpb_com.h"
#include "edd_cfg.h"
#include "eddp_phy.h"
#include "eddp_llif.h"
#include "eddp_rzt2.h"
#include "r_ether_switch_rzt2_ext.h"
#include "clrpc_pdu.h"
#include "pnpb_io.h"
/* RSync interface */
#include "pnorsyncif.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "ecos_ertec_os_utils.h"

//#pragma pack (4)

#define EDDP_LL_LOCK_ENTER(_LOCK)		//OsEnterX(_LOCK)
#define EDDP_LL_LOCK_EXIT(_LOCK)		//OsExitX(_LOCK)

#if __ICCARM__
#pragma location="nocache"
volatile EDDP_LL_HANDLE_TYPE  	 g_tHandle;
#else
volatile EDDP_LL_HANDLE_TYPE     g_tHandle BSP_PLACE_IN_SECTION(BSP_UNINIT_SECTION_PREFIX "nocache");
#endif

EDDP_LL_HANDLE_PTR_TYPE  g_ptHandle = LSA_NULL;


#if __ICCARM__
#pragma location=".noncache_buffer.eth"
#pragma data_alignment=4
ether_instance_descriptor_t g_tx_descriptor_base;

#pragma location=".noncache_buffer.eth"
#pragma data_alignment=4
ether_instance_descriptor_t g_tx_descriptors_rtc3[DEV_CFG_MAX_ARS_RTC3];

#pragma location=".noncache_buffer.eth"
#pragma data_alignment=4
ether_instance_descriptor_t g_tx_descriptors_rtcx[DEV_CFG_MAX_ARS_RTC1_RTC2];

#pragma location=".noncache_buffer.eth"
#pragma data_alignment=4
RTCX_PKT g_tRtcRxPktList[IOD_CFG_NUMOF_IO_AR];
#pragma location=".noncache_buffer.eth"
#pragma data_alignment=4
RTCX_PKT g_tRtcTxPktList[IOD_CFG_NUMOF_IO_AR];
#else
ether_instance_descriptor_t g_tx_descriptor_base BSP_PLACE_IN_SECTION(BSP_UNINIT_SECTION_PREFIX ".noncache_buffer.eth") BSP_ALIGN_VARIABLE(4);
ether_instance_descriptor_t g_tx_descriptors_rtc3[DEV_CFG_MAX_ARS_RTC3] BSP_PLACE_IN_SECTION(BSP_UNINIT_SECTION_PREFIX ".noncache_buffer.eth") BSP_ALIGN_VARIABLE(4);
ether_instance_descriptor_t g_tx_descriptors_rtcx[DEV_CFG_MAX_ARS_RTC1_RTC2] BSP_PLACE_IN_SECTION(BSP_UNINIT_SECTION_PREFIX ".noncache_buffer.eth") BSP_ALIGN_VARIABLE(4);
RTCX_PKT g_tRtcRxPktList[IOD_CFG_NUMOF_IO_AR] BSP_PLACE_IN_SECTION(BSP_UNINIT_SECTION_PREFIX ".noncache_buffer.eth") BSP_ALIGN_VARIABLE(4);
RTCX_PKT g_tRtcTxPktList[IOD_CFG_NUMOF_IO_AR] BSP_PLACE_IN_SECTION(BSP_UNINIT_SECTION_PREFIX ".noncache_buffer.eth") BSP_ALIGN_VARIABLE(4);
#endif

typedef struct ar_information
{
	LSA_UINT16 ConsumerId;
	LSA_UINT16 ProviderId;
}AR_INFO;
AR_INFO ArInfo[IOD_CFG_NUMOF_IO_AR];

LSA_UINT32  g_TxPhaseState = EDDP_LL_PHASE_YELLOW;
LSA_BOOL   g_CpmOn = LSA_FALSE;
LSA_BOOL   g_PpmOn = LSA_FALSE;
LSA_UINT32 g_SendClock =(PROFINET_CYCLE_TIME_INIT / 31250U);
LSA_UINT16 g_CmpTimeout = 0;
LSA_UINT32 g_CyclCntr;
EDD_RQB_NRT_RECV_TYPE RSyncReceive[10][NUM_PORTS];
LSA_UINT32 cycle_time_new = 0;
tdma_update_data_t tdma_update_data;
uint8_t g_bProfinetSetup = 0;
#if IOD_INCLUDE_STORM_FILTER_DCP_SW
static LSA_UINT8 *g_StationNamePrt=NULL;
static LSA_INT g_StationNameLen=0;
#endif /* IOD_INCLUDE_STORM_FILTER_DCP_SW */
#if IOD_INCLUDE_STORM_FILTER_ETHER_SW
static LSA_BOOL                      g_EtherTypeFilterInit=LSA_FALSE;
static EDDP_ETHER_TYPE_FILTER_TYPE   g_EtherTypeFiletr[EDDP_LL_ETHER_TYPE_FILTER_MAX]={0};
static LSA_UINT32                    g_EtherTypeFilterCounter=0;
static LSA_UINT16                    g_EtherTypeFilterTimerID;
#endif /* IOD_INCLUDE_STORM_FILTER_ETHER_SW */

void printeth(char * printdata);
void printethFromISR(char * printdata); 
void pps_out(uint8_t timer, uint32_t start_sec, uint32_t start, uint32_t period); 

LSA_VOID setPortNrForProv(LSA_UINT32 ArNum, LSA_UINT32 portNr);

LSA_VOID_PTR_TYPE  EDDP_LL_MNG_HANDLE(LSA_UINT16 InterfaceID)
{
	LSA_VOID_PTR_TYPE pvHandle = LSA_NULL;
	if(1U == InterfaceID)
	{
		pvHandle = g_ptHandle;
	}

	return pvHandle;
}

LSA_VOID EDDP_LL_ENTER(LSA_VOID)
{
	EDDP_LL_HANDLE_PTR_TYPE pHandle = (EDDP_LL_HANDLE_PTR_TYPE)EDDP_LL_MNG_HANDLE(1);
	if(LSA_NULL != pHandle)
	{
		OsTakeSemB(pHandle->IfCtrlLock);
	}
}

LSA_VOID EDDP_LL_EXIT(LSA_VOID)
{
	EDDP_LL_HANDLE_PTR_TYPE pHandle = (EDDP_LL_HANDLE_PTR_TYPE)EDDP_LL_MNG_HANDLE(1);
	if(LSA_NULL != pHandle)
	{
		OsGiveSemB(pHandle->IfCtrlLock);
	}
}

LSA_VOID_PTR_TYPE  EDDP_LL_GetIfHandle(LSA_UINT16 InterfaceID)
{
	LSA_VOID_PTR_TYPE pvIf = LSA_NULL;
	if( (1U == InterfaceID) && (LSA_NULL != g_ether0.p_api) &&
		(LSA_NULL != g_ether0.p_cfg) && (LSA_NULL != g_ether0.p_ctrl) )
	{
		pvIf = (LSA_VOID_PTR_TYPE)&g_ether0;
	}

	return pvIf;
}

void EDDP_LL_SwiSetupInd(void)
{
	while(EDDP_LL_PHASE_YELLOW != g_TxPhaseState);
}


void EDDP_LL_ISR_NewCycle(void)
{
	g_CyclCntr++;
}

// @kochn: add/remove static entries upon ConnectRequest / ReleaseRequest
LSA_VOID EDDP_LL_AddRemoveStaticMacEntry(LSA_UINT8 *pFrame, LSA_UINT16 PortID, ETH_HDR_MGMTTAG_PTR pEthHdr, LSA_UINT16 vlan_id)
{
#if 0
	CLRPC_HEADER_PTR_TYPE rpc_header = (CLRPC_HEADER_PTR_TYPE)(pFrame + 6);

	if (rpc_header->rpc_vers == 4 && rpc_header->ptype == CLRPC_PT_REQUEST)
	{
		switch (rpc_header->opnum)
		{
			case CLRPC_PNIO_OPNUM_CONNECT:
				if (-1 == ethsw_port_fdb_add(PortID, pEthHdr->src, vlan_id)) {
					ethsw_port_fdb_del(PortID, pEthHdr->src, vlan_id);
					ethsw_port_fdb_add(PortID, pEthHdr->src, vlan_id);
				}
				break;
			case CLRPC_PNIO_OPNUM_RELEASE:
				ethsw_port_fdb_del(PortID, pEthHdr->src, vlan_id);
				break;
			default:
				break;
		}
	}
#endif
}

static LSA_UINT32 EDDP_LL_GetIRTActiveHostHwPortBits( EDDP_LOCAL_DDB_PTR_TYPE pDDB )
{
    LSA_UINT32 IRTActiveHostHwPortBits = 0;

    /** Check port state to decide target ports to send RTC3 frame. */
	for( LSA_UINT32 PortID = 1; PortID <= pDDB->SWI.PortCnt; PortID++ )
	{
		if ( pDDB->SWI.LinkStatus[PortID].IRTPortStatus == EDD_IRT_PORT_ACTIVE_TX_RX )
		{
			/** Get HW Port ID */
			LSA_UINT32 HwPortID = EDDP_SWI_get_HwPortID (pDDB, PortID);

			/** Set the HW port to target ports */
			IRTActiveHostHwPortBits |= ( 1 << HwPortID );
		}
	}

    return IRTActiveHostHwPortBits;
}

static LSA_UINT32 EDDP_LL_GetIRTActiveRemoteHwPortBits( EDDP_LOCAL_DDB_PTR_TYPE pDDB )
{
    LSA_UINT32 IRTActiveRemoteHwPortBits = 0;

	for( LSA_UINT32 PortID = 1; PortID <= pDDB->SWI.PortCnt; PortID++ )
	{
		if ( pDDB->SWI.RTC3_PortState[PortID].RemoteState == EDD_SET_REMOTE_PORT_STATE_RT_CLASS3_RXTX )
		{
			/** Get HW Port ID */
			LSA_UINT32 HwPortID = EDDP_SWI_get_HwPortID (pDDB, PortID);

			/** Set the HW port to target ports */
			IRTActiveRemoteHwPortBits |= ( 1 << HwPortID );
		}
	}

    return IRTActiveRemoteHwPortBits;
}

static LSA_UINT32 IRTActiveHwPortBits_Last = 0;
LSA_VOID EDDP_LL_EnablePatternMatcher( LSA_VOID )
{
    LSA_UINT32 IRTActiveEthSwPortBits = (IRTActiveHwPortBits_Last >> 1) & 0x03;  // IRT Port enabled ?
    LSA_UINT32 IRTInactiveEthSwPortBits = (~IRTActiveEthSwPortBits) & 0x03;

    // Now enable the pattern matcher that are required for the valid RTC3 ports and disable for the others
    if(IRTActiveEthSwPortBits)
    {
        ethsw_disable_pattern_matcher(IRTActiveEthSwPortBits, PATTERN_DISC_IRT_RED_MASK);
    }

    if(IRTInactiveEthSwPortBits)
    {
        ethsw_enable_pattern_matcher(IRTInactiveEthSwPortBits, PATTERN_DISC_IRT_RED_MASK);
    }
}

LSA_VOID EDDP_LL_ControlRTC3MulticastFowardingDependingOnPortStatus( EDDP_LOCAL_DDB_PTR_TYPE pDDB )
{
    /** Get each port status */
    LSA_UINT32 IRTActiveHostHwPortBits = EDDP_LL_GetIRTActiveHostHwPortBits( pDDB );
    LSA_UINT32 IRTActiveRemoteHwPortBits = EDDP_LL_GetIRTActiveRemoteHwPortBits( pDDB );

    /** Active Hardware Port Bits which are acitve on both host side and remote side. */
    LSA_UINT32 IRTActiveHwPortBits = (IRTActiveHostHwPortBits & IRTActiveRemoteHwPortBits);

    /** Check if the acitve ports is updated */
    if( IRTActiveHwPortBits_Last != IRTActiveHwPortBits )
    {
        IRTActiveHwPortBits_Last = IRTActiveHwPortBits;
        EDDP_LL_EnablePatternMatcher();
    }
}

static LSA_UINT32 g_Tdma_counter = 1;

void EDDP_LL_ISR_PhaseStartRed(void)
{
	EDDP_LL_HANDLE_PTR_TYPE pHandle = (EDDP_LL_HANDLE_PTR_TYPE)&g_tHandle;
	ether_instance_t *ptIf = EDDP_LL_CAST_ETH(pHandle->pIfHandle);
	ether_instance_ctrl_t *ptIfCtrl = (ether_instance_ctrl_t*)ptIf->p_ctrl;
	volatile  R_GMAC_Type * p_reg_etherc = (R_GMAC_Type *) ptIfCtrl->p_reg_etherc;
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
	LSA_UINT32 Current_Host_Transmit_Descriptor = p_reg_etherc->Current_Host_Transmit_Descriptor;
        __attribute__((unused))PNDV_AR_DATA_T *pAr = &pndv_data.ar[0U];
        // Change cycle time on the fly. Must be done as last action of the CTVs of the TDMA controller.
        // Yellow time etc. mus be adjusted accordingly (so they do not overshoot the next cycle start)

        if (cycle_time_new > 0)
        {
          p_reg_ethsw->TDMA_CYCLE = cycle_time_new;
          cycle_time_new = 0;
        }
        //p_reg_ethsw->INT_CONFIG |= (1<<29);    // Re-Ensable global TSM interrupt. Was dsabled to make sure the timestamp interrupt would not interfere with this interrupt



	g_TxPhaseState = EDDP_LL_PHASE_RED;


	/** Resolve the type of the device description block. */
	EDDP_LOCAL_DDB_PTR_TYPE pDDB = (EDDP_LOCAL_DDB_PTR_TYPE) pHandle->hDDB;

	/** TX target ports */
	LSA_UINT32 rtc3_tx_active_ports = 0;

	/** Check port state to decide target ports to send RTC3 frame. */
	for( LSA_UINT32 PortID = 1; PortID <= pDDB->SWI.PortCnt; PortID++ )
	{
		if ( ( pDDB->SWI.LinkStatus[PortID].IRTPortStatus == EDD_IRT_PORT_ACTIVE_TX_RX ) &&
		     ( pDDB->SWI.RTC3_PortState[PortID].RemoteState == EDD_SET_REMOTE_PORT_STATE_RT_CLASS3_RXTX ) )
		{
			/** Get HW Port ID */
			LSA_UINT32 HwPortID = EDDP_SWI_get_HwPortID (pDDB, PortID);

			/** Set the HW port to target ports */
			rtc3_tx_active_ports |= ( 1 << HwPortID );
		}
	}
	p_reg_ethsw->MMCTL_QFLUSH = 0x03 << 24 | 0x08 << 16 | 0x03;   // Flush RTC3 TX queue so all remaining (delayed) frames are deleted 

	if( (0U < pHandle->dwLinkStatusAll) && (LSA_NULL != ptIf) && (LSA_NULL != ptIfCtrl) && (LSA_NULL != p_reg_etherc) )
	{
		LSA_UINT16  ActiveRtc3Cnt = 0;

		if( (TX_DESCR_ADDR_NRT_FIRST <= p_reg_etherc->Current_Host_Transmit_Descriptor) &&
			(TX_DESCR_ADDR_NRT_LAST  >= p_reg_etherc->Current_Host_Transmit_Descriptor) )
		{
			ptIfCtrl->Prev_Host_Transmit_Descriptor = p_reg_etherc->Current_Host_Transmit_Descriptor;
		}

#ifdef PrintfDebug    
		if( ( 0U < rtc3_tx_active_ports ) && (0U < pHandle->Rtc3TxCnt) && (0U < NumOfAr) && g_tRtcTxPktList[0].Status != EDD_STS_OK) {
      //printethFromISR("No RTC3 TX  Data"); 
    }
#endif
		if( ( 0U < rtc3_tx_active_ports ) && (0U < pHandle->Rtc3TxCnt) && (0U < NumOfAr) )
		{
			LSA_UINT16 nCnt = 0U;

			while(nCnt < IOD_CFG_NUMOF_IO_AR)
			{
				if(g_tRtcTxPktList[nCnt].Status == EDD_STS_OK)
				{
					EDDP_CRT_PROVIDER_PTR_TYPE pProvider = (EDDP_CRT_PROVIDER_PTR_TYPE)g_tRtcTxPktList[nCnt].pResource;

					if( (FRAMEID_RTC3_START <= pProvider->AddParam.FrameID) && (FRAMEID_RTC3_END >= pProvider->AddParam.FrameID) )
					{

						g_tx_descriptors_rtc3[0].des0 = (ETHER_TDES0_OWN | ETHER_TDES0_TCH);
						// Suppress empty frames consistig just of 00000. This might happen if the AR is not jet clearly shut down after an Alarm.
						uint32_t *value;
						value = (uint32_t *) g_tx_descriptors_rtc3[0].des2;
						if (*value == 0) {
							//app_printf_from_ISR("*******ERROR %x\n", *value);
							break;
						}

						if( (0U < g_tx_descriptors_rtc3[0].des1) &&
						    (0U < g_tx_descriptors_rtc3[0].des2))
						{
							g_tx_descriptors_rtc3[0].des0 |= (ETHER_TDES0_LS | ETHER_TDES0_FS | ETHER_TDES0_TER);
							ActiveRtc3Cnt++;

							// the status is changed to EDD_STS_OK_NO_DATA because the frame is ready to be sent.
							g_tRtcTxPktList[nCnt].Status = EDD_STS_OK_NO_DATA;
						}
					}
				}
				nCnt++;
			}
		}

		if(0U < ActiveRtc3Cnt)
		{
			if(p_reg_etherc->Current_Host_Transmit_Descriptor != (uint32_t)ptIfCtrl->p_tx_descriptor_base)
			{
				p_reg_etherc->Transmit_Descriptor_List_Address_b.TDESLA_32bit = (uint32_t) ptIfCtrl->p_tx_descriptor_base >> 2;
			}

			ptIfCtrl->p_tx_descriptor_base->des0 = (ETHER_TDES0_OWN | ETHER_TDES0_TCH);
			ptIfCtrl->p_tx_descriptor_base->des3 = (uint32_t)ptIfCtrl->p_tx_descriptor_rtc3;
			p_reg_etherc->Operation_Mode_b.ST = 1U;     /* (Re-)Start TX */
			__asm("dmb");
			p_reg_etherc->Transmit_Poll_Demand = 0U;    /* Trigger transmission */
		}
		else if(0U < pHandle->RtcxTxCnt)
		{
			p_reg_etherc->Transmit_Descriptor_List_Address_b.TDESLA_32bit = (uint32_t) ptIfCtrl->p_tx_descriptor_base >> 2;
			p_reg_etherc->Operation_Mode_b.ST = 1U;     /* (Re-)Start TX */
		}
	}


}

/* This interrupt is called during the RED phase by the TDMA controller to adjust the pattern matchers when the early green phase starts and one of the two active ports needs to be opened for the
NRT frames
The pattern matchers are already prepared in this interrupt they are only (de-)activated

*/         
                     
extern uint8_t early_green_port[16];     // Mask for the port, that needs to be switched to GREEN mode first. Not set when 0, otherwise either 01 or 02
void EDDP_LL_ISR_PatternAdjust(void)
{
	EDDP_LL_HANDLE_PTR_TYPE pHandle = (EDDP_LL_HANDLE_PTR_TYPE)&g_tHandle;
	ether_instance_t *ptIf = EDDP_LL_CAST_ETH(pHandle->pIfHandle);
	ether_instance_ctrl_t *ptIfCtrl = (ether_instance_ctrl_t*)ptIf->p_ctrl;
	volatile  R_GMAC_Type * p_reg_etherc = (R_GMAC_Type *) ptIfCtrl->p_reg_etherc;
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
	LSA_UINT32 Current_Host_Transmit_Descriptor = p_reg_etherc->Current_Host_Transmit_Descriptor;
    uint32_t phase = p_reg_ethsw->TDMA_CTR1 & 0x0f;
#ifdef _RENESAS_RZT_
        if (p_reg_ethsw->RXMATCH_CONFIG[0] > 0)   // Pattern matcher already enabled by somebody
#endif
#ifdef _RENESAS_RZN_
        if (p_reg_ethsw->RXMATCH_CONFIG[0] > 0)   // Pattern matcher already enabled by somebody
#endif
        {
          ethsw_enable_pattern_matcher(early_green_port[phase], PATTERN_DISC_IRT_MASK);
        }

}



      /* RT and NRT Descriptors are stored in the respective buffers tx_descriptors[n] and g_tx_descriptors_rtcx[n]. The descriptors for RT frames are organized
         in a bottom up schema, starting from index 0 up to the max. These must be transmitted all during a green phase.
          The NRT decriptors are organized in a ring buffer structure. The pointer ptIfCtrl->p_tx_descriptor always points to the first empty
         descriptor in the ring. Transmission should start on the the first busy descriptor as indicated by the OWN bit within the descriptor and transmit as many as possible.
         Transmission is stopped by the GMAC within the yellow phase.
         To find the first used descriptor we count backwards until we find the first unused descriptor and start transmission from there.
         Transmission is always started from p_tx_descriptor_base. This descriptor is always empty and simply lnks to the first descriptor that should be transmitted.
         This can be either the first in the g_tx_descriptors_rtcx[n] buffer (if used)  or the first active descriptor in the tx_descriptors[n] ring buffer.
         The two blocks then need to be put in a chain, i. e. the last vaid RT descriptor must link to the first valid NRT descriptor */
uint32_t test_time = 0;
uint32_t tdma_set_time = 0;
void EDDP_LL_ISR_PhaseStartGreen(void)
{
	EDDP_LL_HANDLE_PTR_TYPE pHandle = (EDDP_LL_HANDLE_PTR_TYPE)&g_tHandle;
	ether_instance_t *ptIf = EDDP_LL_CAST_ETH(pHandle->pIfHandle);
	ether_instance_ctrl_t *ptIfCtrl = (ether_instance_ctrl_t*)ptIf->p_ctrl;
	volatile  R_GMAC_Type * p_reg_etherc = (R_GMAC_Type *) ptIfCtrl->p_reg_etherc;
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
        ether_instance_descriptor_t* p_tx_descriptor_nrt_start = LSA_NULL;     // Points to first valid NRT descriptor.
        ether_instance_descriptor_t* p_tx_descriptor_active = LSA_NULL;
                LSA_UINT8 CntDes_NRT = 0U;    // Amount of NRT frames
		LSA_UINT16 nCnt = 0U;            // Counter for RT frames
		LSA_UINT16 ActiveRtcxCnt = 0U;   // Amount of RT frames

                // Check for NRT frames first
			ether_instance_descriptor_t* p_tx_descriptor = LSA_NULL;
                ether_instance_descriptor_t* p_tx_descriptor_down = LSA_NULL;

    // Check if TDMA Controller needs to be updated
    if (tdma_update_data.update_tdma == 1)    //
    {
      uint32_t cycle_time = tdma_update_data.cycle_time;      // get new cycle_time. This is profinet cycle time, i. e. would need to me multiplied by the phase number to ghet the interval
      uint64_t interval_time = cycle_time * tdma_update_data.num_phases;
      uint64_t curr_time = ethsw_get_time(ETHSW_TIMER0);     // Fetch current time
      uint32_t curr_time_sec = ethsw_get_time_sec(ETHSW_TIMER0);
      if (curr_time_sec) {    // Adjust for next valid seconds value to take care of 4ms*16 phases restart time
        curr_time += (1000000000 * ((uint64_t) curr_time_sec));
      }
      uint64_t interval_start = curr_time - (curr_time % interval_time);    // last interval start
      interval_start += interval_time;  // Set to next interval start
      if ((interval_start - curr_time) < cycle_time)    // We are at the last phase of the planned interval
      {
        p_reg_ethsw->TDMA_CONFIG_b.TDMA_ENA = 0 ;
        p_reg_ethsw->TDMA_TCV_START = tdma_update_data.start_address;
        p_reg_ethsw->TCV_SEQ_LAST   = tdma_update_data.end_address;
        p_reg_ethsw->TDMA_CYCLE    = interval_time;
        tdma_set_time = interval_start;
        p_reg_ethsw->TDMA_START = interval_start % 1000000000;        // Starttime at next next interval start + adju
        p_reg_ethsw->TDMA_CONFIG_b.TDMA_ENA = 1 ;
        tdma_update_data.update_tdma = 0;
        // Start Phase counter
        p_reg_ethsw->TDMA_CTR1 = 0x100 | (((tdma_update_data.num_phases-1) << 16));    // Overflow at NumPhases
        pps_out(ETHSW_TIMER0, curr_time_sec, interval_start % 1000000000, interval_time);

      }
    }


	if( (0U < pHandle->dwLinkStatusAll) && (LSA_NULL != ptIf) && (LSA_NULL != ptIfCtrl) && (LSA_NULL != p_reg_etherc) )
	{

                p_reg_etherc->Operation_Mode_b.ST = 0U;     /* Stop TX */
                                        p_tx_descriptor = ptIfCtrl->p_tx_descriptor;               // Pointer to first empty descriptor
                do  // Go backwards through all NRT frames to find first valid frame in ring buffer
                {
                  if(p_tx_descriptor == &tx_descriptors[0])
                  {
                          p_tx_descriptor_down = &tx_descriptors[(NUM_TX_DESCRIPTOR-1U)];   // Turover at Index 0
                          if ((p_tx_descriptor_down->des0) & ETHER_TDES0_OWN)
                          {
                            p_tx_descriptor_down->des3 = (uint32_t) &tx_descriptors[0];   // Last desc. in row points back to first
                            p_tx_descriptor_down->des0 |= (ETHER_TDES0_TCH);  // Set Cain flag
                          }
                  }
                  else
                  {
                          p_tx_descriptor_down = p_tx_descriptor-1;
                          if ((p_tx_descriptor_down->des0) & ETHER_TDES0_OWN) {
                            p_tx_descriptor_down->des3 = (uint32_t) p_tx_descriptor;  // Last descriptor points back to base after finished
                            p_tx_descriptor_down->des0 |= (ETHER_TDES0_TCH);  // Set Cain flag
                          }
                  }
                  if ((p_tx_descriptor_down->des0) & ETHER_TDES0_OWN)    // next over is used
                  {
                    p_tx_descriptor = p_tx_descriptor_down;
                    if (CntDes_NRT == 0) {
                      p_tx_descriptor_down->des0 |= (ETHER_TDES0_TCH);  // Set Chain flag
                      p_tx_descriptor->des0 |= (ETHER_TDES0_IC);  // Set Interrupt flag for last frame in row
                      p_tx_descriptor->des3 = (uint32_t) ptIfCtrl->p_tx_descriptor_base;  // Last descriptor points back to base after finished
                    }
                  }
                  CntDes_NRT++;
                  if (CntDes_NRT > (NUM_TX_DESCRIPTOR-1U))
                  {
                    break;     // We can only have NUM_TX_DESCRIPTOR descriptors, so we have now turned around
                  }
              }
              while ((p_tx_descriptor_down-> des0) & ETHER_TDES0_OWN);

                CntDes_NRT--; // CntDes_NRT counts at least once in the while loop
                if (CntDes_NRT > 0)
		{
                  p_tx_descriptor_nrt_start = p_tx_descriptor;    // p_tx_descriptor_nrt_start now points to first used NRT descriptor
                }


        // Now check for RT frames
		if( (0U < pHandle->RtcxTxCnt) && (0U < NumOfAr))
		{
			nCnt = DEV_CFG_MAX_ARS_RTC1_RTC2;

			EDDP_LOCAL_DDB_PTR_TYPE pDDB = pHandle->hDDB;
			EDDP_CRT_PROVIDER_MNGM_PTR_TYPE pProvMngm = &pDDB->CRT.Prov;
			EDDP_CRT_PROVIDER_PTR_TYPE pProvider;
			EDDP_CRT_PROVIDER_ACW_PTR_TYPE pAcw;
                        
			while(nCnt)   // Set Control flags for active descriptors and adjust link registers
			{
				nCnt--;
				if(g_tRtcTxPktList[nCnt].Status == EDD_STS_OK)
				{
					pProvider = g_tRtcTxPktList[nCnt].pResource;
					pAcw      = &pProvMngm->pAcwEntry[pProvider->AcwList.EndIdx];

					if( ((g_Tdma_counter % pAcw->AddParam.CycleReductionRatio) == 0) &&
					    (FRAMEID_RTC2_START <= pProvider->AddParam.FrameID) && (FRAMEID_RTC2_END >= pProvider->AddParam.FrameID))
					{

						g_tx_descriptors_rtcx[nCnt].des0 = (ETHER_TDES0_OWN | ETHER_TDES0_TCH);

						if(ActiveRtcxCnt)    // There is one more left
						{
							g_tx_descriptors_rtcx[nCnt].des3 = (uint32_t)p_tx_descriptor_active; // Point current descriptor to next descriptor for chain
						}
						else  // Highest descritor reached (one left).
						{
							g_tx_descriptors_rtcx[nCnt].des0 |= (ETHER_TDES0_IC);   // Interrupt on completion  ????
							if(0U < p_tx_descriptor_nrt_start)
							{
								g_tx_descriptors_rtcx[nCnt].des3 = (uint32_t) p_tx_descriptor_nrt_start; // Chain continues transmitting NRT frames
							}
							else
							{
								//g_tx_descriptors_rtcx[nCnt].des0 |= ETHER_TDES0_TER;
								g_tx_descriptors_rtcx[nCnt].des3 = (uint32_t) ptIfCtrl->p_tx_descriptor_base;  // No NRT frames, return to base.
							}
						}
						// Check if descriptor is used and if yes set control bits
						if( (0U < g_tx_descriptors_rtcx[nCnt].des1) &&
						    (0U < g_tx_descriptors_rtcx[nCnt].des2))
						{
							g_tx_descriptors_rtcx[nCnt].des0 |= (ETHER_TDES0_LS | ETHER_TDES0_FS);

							//SG MRP
							setPortNrForProv(nCnt, 0);

							p_tx_descriptor_active = &g_tx_descriptors_rtcx[nCnt];
							ActiveRtcxCnt++;

							// the status is changed to EDD_STS_OK_NO_DATA because the frame is ready to be sent.
							g_tRtcTxPktList[nCnt].Status = EDD_STS_OK_NO_DATA;
						}
					}
				}
			}
		}
                if((0U < ActiveRtcxCnt) || (0U < CntDes_NRT))   // At least one active descriptor detected
                {
                        if(p_reg_etherc->Current_Host_Transmit_Descriptor != (uint32_t)ptIfCtrl->p_tx_descriptor_base)
                        {
                            p_reg_etherc->Operation_Mode_b.ST = 0U;     /* Stop TX to write new start address*/
                            p_reg_etherc->Transmit_Descriptor_List_Address_b.TDESLA_32bit = (uint32_t) ptIfCtrl->p_tx_descriptor_base >> 2;
//				}
	}
                        if (0U < ActiveRtcxCnt)
                        {
                          ptIfCtrl->p_tx_descriptor_base->des3 = (uint32_t)p_tx_descriptor_active;
                        }
                        else
                        {
                           ptIfCtrl->p_tx_descriptor_base->des3 = (uint32_t)p_tx_descriptor_nrt_start;  // p_tx_descriptor now points to first used NRT descriptor
                        }
                        ptIfCtrl->p_tx_descriptor_base->des0 = (ETHER_TDES0_OWN | ETHER_TDES0_TCH);
                        p_reg_etherc->Operation_Mode_b.ST = 1U;     /* (Re-)Start TX */
                        __asm("dmb");
                        p_reg_etherc->Transmit_Poll_Demand = 0U;    /* Trigger transmission */
                }
	}   // dwLinkStatusAll
	g_TxPhaseState = EDDP_LL_PHASE_GREEN;   // Marks the phase when the green phase is active, but the interrupt handling is done for the send trigger handler
                                                // which then haldles "late" gerren frames in the late part of the green phase

    g_Tdma_counter = (LSA_UINT32)ethsw_tda_counter_value_get();

    g_CyclCntr = g_Tdma_counter;
    if( (0U < pHandle->dwLinkStatusAll) && (LSA_NULL != ptIf) &&
            (LSA_NULL != ptIfCtrl) && (LSA_NULL != p_reg_etherc) && (0U < NumOfAr))
    {
        OsTaskNotifyFromISR(TskId_CycleIO, (PNIO_EV_TI_VALID) );
    }
    EDDP_LL_ConsumerDhtExpireCheck(((EDDP_LOCAL_DDB_PTR_TYPE)pHandle->hDDB)->InterfaceID);                

    portYIELD_FROM_ISR(pdTRUE); 
}

void EDDP_LL_ISR_PhaseStartYellow(void)
{
	EDDP_LL_HANDLE_PTR_TYPE pHandle = (EDDP_LL_HANDLE_PTR_TYPE)&g_tHandle;
	ether_instance_t *ptIf = EDDP_LL_CAST_ETH(pHandle->pIfHandle);
	ether_instance_ctrl_t *ptIfCtrl = (ether_instance_ctrl_t*)ptIf->p_ctrl;
	volatile  R_GMAC_Type * p_reg_etherc = (R_GMAC_Type *) ptIfCtrl->p_reg_etherc;
        R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;


	g_TxPhaseState = EDDP_LL_PHASE_YELLOW;
        p_reg_etherc->Operation_Mode_b.ST    = 0;     /* Stop TX */

#ifdef _RENESAS_RZT_
        if (p_reg_ethsw->RXMATCH_CONFIG[0] > 0)   // Only if  Pattern matcher already enabled by somebody
#endif
#ifdef _RENESAS_RZN_
        if (p_reg_ethsw->RXMATCH_CONFIG[0] > 0)   // Only if  Pattern matcher already enabled by somebody
#endif
        {
    /** Control RTC3 frame forwarding */
            EDDP_LL_ControlRTC3MulticastFowardingDependingOnPortStatus(pHandle->hDDB);
            //EDDP_LL_EnablePatternMatcher();
            ethsw_disable_pattern_matcher(0x03, PATTERN_DISC_IRT_MASK);

        }
}

void vEtherSwitchISRCallback(ether_switch_callback_args_t * p_args)
{
	__attribute__((unused))uint8_t nCnt = 0;
	__attribute__((unused))uint8_t nMaxPrm = 0;
	__attribute__((unused))OS_MSG_PTR_TYPE ptMsg = LSA_NULL;
	EDDP_LL_HANDLE_PTR_TYPE pHandle = LSA_NULL;
	__attribute__((unused))ether_switch_callback_args_t *ptPrm = LSA_NULL;

	if(LSA_NULL != p_args)
	{
		pHandle = (EDDP_LL_HANDLE_PTR_TYPE)EDDP_LL_MNG_HANDLE(p_args->channel+1U);
	}

		pHandle->dwLinkEvent |= EDDP_LL_EV_LINK_CHG;

		OsTaskNotifyFromISR(pHandle->IfCtrlTaskId, EDDP_LL_EV_LINK);
}

void vEtherISRCallback (ether_callback_args_t * p_args)
{
	EDDP_LL_HANDLE_PTR_TYPE pHandle = LSA_NULL;

	if(LSA_NULL != p_args)
	{
		pHandle = (EDDP_LL_HANDLE_PTR_TYPE)EDDP_LL_MNG_HANDLE(p_args->channel+1U);
	}

	if( (LSA_NULL != pHandle) && (0U < (EDDP_LLIF_STATE_OPEN & pHandle->u16State)) )
	{
		ether_instance_t *ptIf = EDDP_LL_CAST_ETH(pHandle->pIfHandle);
		ether_instance_ctrl_t *ptIfCtrl = (ether_instance_ctrl_t*)ptIf->p_ctrl;
		__attribute__((unused))volatile  R_GMAC_Type * p_reg_etherc = (R_GMAC_Type *) ptIfCtrl->p_reg_etherc;


		switch((uint8_t)p_args->event)
		{
		case ETHER_EVENT_SBD_INTERRUPT:
		{
//			LSA_UINT32 u32Event = 0U;
			__attribute__((unused))RZT2_GMAC_STATUS tStatus;

			tStatus.u32 = p_args->status_ether;

			if (0U < ((ETHER_GMAC_INTERRUPT_FACTOR_RECEPTION /*| 0x00000100UL*/) & p_args->status_ether) )
			{
				// Direct request to EDD, not via EDDP_LL_CTRL_TASK
				eddp_interrupt(lsas_com_get_hddb(), 2);     // EDDP_NRT_ISR_Acyc_Rcv_IFA()
//				u32Event |= EDDP_LL_EV_RCV;
			}

			//if(LSA_TRUE == tStatus.tBits.TI)
			if( 0U < (0x00000007UL & p_args->status_ether) )
			{
//              Sending completion is notified from EtherSwich events(ethSendEvt).
//				if(LSA_TRUE == tStatus.tBits.TI)
//				{
//					u32Event |= EDDP_LL_EV_SND;
//				}
			}

//			if(0U < u32Event)
//			{
//				OsTaskNotifyFromISR(pHandle->IfCtrlTaskId, u32Event);
//			}

		}break;
		case ETHER_EVENT_PMT_INTERRUPT:
			break;
		default:
			break;
		}
	}
    portYIELD_FROM_ISR(pdTRUE);
}


LSA_VOID* EDDP_LL_GetRtcRcvPkt(EDDP_LL_HANDLE_PTR_TYPE pHandle, LSA_UINT16 FrameID, LSA_UINT8 *MacAddr)
{
	RTCX_PKT_PTR ptRx = LSA_NULL;
	if(LSA_NULL != MacAddr)
	{
		LSA_UINT16 nCnt = 0U;

		__attribute__((unused))RTCX_HDR_PTR_NV ptHdr;

		while( (LSA_NULL == ptRx) && (nCnt < IOD_CFG_NUMOF_IO_AR) )
		{
			ptHdr = (RTCX_HDR_PTR_NV)&g_tRtcRxPktList[nCnt].Buffer[0U];

			if( (LSA_NULL != g_tRtcRxPktList[nCnt].pResource) &&
				(ptHdr->src[0] == MacAddr[0]) &&
				(ptHdr->src[1] == MacAddr[1]) &&
				(ptHdr->src[2] == MacAddr[2]) &&
				(ptHdr->src[3] == MacAddr[3]) &&
				(ptHdr->src[4] == MacAddr[4]) &&
				(ptHdr->src[5] == MacAddr[5]) &&
				(FrameID == __REV16(ptHdr->frame_id) ) )
			{
				ptRx = &g_tRtcRxPktList[nCnt];
			}
			nCnt++;
		}
	}
	return ptRx;
}

void delay_timer_cbf(timer_callback_args_t * p_args)
{
    if (TIMER_EVENT_CYCLE_END == p_args->event)
    {
        // Restart one shot timer
    //	OsTaskNotifyFromISR(TskId_CycleIO, PNIO_EV_TI);
    }
}

LSA_VOID* EDDP_LL_GetRtcSndPkt(EDDP_LL_HANDLE_PTR_TYPE pHandle, LSA_UINT16 FrameID, LSA_UINT8 *MacAddr)
{
	RTCX_PKT_PTR ptRx = LSA_NULL;
	if(LSA_NULL != MacAddr)
	{
		LSA_UINT16 nCnt = 0U;

		RTCX_SND_HDR_PTR ptHdr;

		while( (LSA_NULL == ptRx) && (nCnt < IOD_CFG_NUMOF_IO_AR) )
		{
			ptHdr = (RTCX_SND_HDR_PTR)&g_tRtcTxPktList[nCnt].Buffer[0U];

			if( (LSA_NULL != g_tRtcTxPktList[nCnt].pResource) &&
				(ptHdr->src[0] == MacAddr[0]) &&
				(ptHdr->src[1] == MacAddr[1]) &&
				(ptHdr->src[2] == MacAddr[2]) &&
				(ptHdr->src[3] == MacAddr[3]) &&
				(ptHdr->src[4] == MacAddr[4]) &&
				(ptHdr->src[5] == MacAddr[5]) &&
				(FrameID == __REV16(ptHdr->frame_id) ) )
			{
				ptRx = &g_tRtcTxPktList[nCnt];
			}
			nCnt++;
		}
	}
	return ptRx;
}

LSA_UINT32 EDDP_LL_GetArIdxfromConsumerId(LSA_UINT16 ConsumerId)
{
	for(LSA_UINT32 ArIdx = 0; ArIdx < IOD_CFG_NUMOF_IO_AR; ArIdx++)
	{
		if(ConsumerId == ArInfo[ArIdx].ConsumerId)
		{
			return ArIdx;
		}
	}

	return IOD_CFG_NUMOF_IO_AR;
}

LSA_VOID* EDDP_LL_GetRtcRcvPktfromConsumerId(LSA_UINT16 ConsumerId)
{
	LSA_UINT32 ArIdx;
	EDDP_CRT_CONSUMER_PTR_TYPE pConsumer;

	for(ArIdx = 0; ArIdx < IOD_CFG_NUMOF_IO_AR; ArIdx++)
	{
		pConsumer = (EDDP_CRT_CONSUMER_PTR_TYPE)g_tRtcRxPktList[ArIdx].pResource;
		if((LSA_NULL != pConsumer) && (ConsumerId == pConsumer->ConsumerId))
		{
			return &g_tRtcRxPktList[ArIdx];
		}
	}

	for(ArIdx = 0; ArIdx < IOD_CFG_NUMOF_IO_AR; ArIdx++)
	{
		if(LSA_NULL == g_tRtcRxPktList[ArIdx].pResource)
		{
			ArInfo[ArIdx].ConsumerId = ConsumerId;
			return &g_tRtcRxPktList[ArIdx];
		}
	}

	return LSA_NULL;
}

LSA_VOID EDDP_LL_ReleaseRtcRcvPkt(LSA_UINT16 ConsumerId)
{
	EDDP_CRT_CONSUMER_PTR_TYPE pConsumer;

	for(LSA_UINT32 ArIdx = 0; ArIdx < IOD_CFG_NUMOF_IO_AR; ArIdx++)
	{
		pConsumer = (EDDP_CRT_CONSUMER_PTR_TYPE)g_tRtcRxPktList[ArIdx].pResource;
		if((LSA_NULL != pConsumer) && (ConsumerId == pConsumer->ConsumerId))
		{
			ArInfo[ArIdx].ConsumerId = EDDP_CRT_CHAIN_IDX_END;
			g_tRtcRxPktList[ArIdx].pResource = LSA_NULL;
		}
	}
}

LSA_UINT32 EDDP_LL_GetArIdxfromProviderId(LSA_UINT16 ProviderId)
{
	for(LSA_UINT32 ArIdx = 0; ArIdx < IOD_CFG_NUMOF_IO_AR; ArIdx++)
	{
		if(ProviderId == ArInfo[ArIdx].ProviderId)
		{
			return ArIdx;
		}
	}

	return IOD_CFG_NUMOF_IO_AR;
}

LSA_VOID* EDDP_LL_GetRtcSndPktfromProviderId(LSA_UINT16 ProviderId)
{
	LSA_UINT32 ArIdx;
	EDDP_CRT_PROVIDER_PTR_TYPE pProvider;

	for(ArIdx = 0; ArIdx < IOD_CFG_NUMOF_IO_AR; ArIdx++)
	{
		pProvider = (EDDP_CRT_PROVIDER_PTR_TYPE)g_tRtcTxPktList[ArIdx].pResource;
		if((LSA_NULL != pProvider) && (ProviderId == pProvider->ProviderId))
		{
			return &g_tRtcTxPktList[ArIdx];
		}
	}

	for(ArIdx = 0; ArIdx < IOD_CFG_NUMOF_IO_AR; ArIdx++)
	{
		if(LSA_NULL == g_tRtcTxPktList[ArIdx].pResource)
		{
			ArInfo[ArIdx].ProviderId = ProviderId;
			return &g_tRtcTxPktList[ArIdx];
		}
	}

	return LSA_NULL;
}

LSA_VOID EDDP_LL_ReleaseRtcSndPkt(LSA_UINT16 ProviderId)
{
	EDDP_CRT_PROVIDER_PTR_TYPE pProvider;

	for(LSA_UINT32 ArIdx = 0; ArIdx < IOD_CFG_NUMOF_IO_AR; ArIdx++)
	{
		pProvider = (EDDP_CRT_PROVIDER_PTR_TYPE)g_tRtcTxPktList[ArIdx].pResource;
		if((LSA_NULL != pProvider) && (ProviderId == pProvider->ProviderId))
		{
			ArInfo[ArIdx].ProviderId = EDDP_CRT_CHAIN_IDX_END;
			OsMemSet(&g_tRtcTxPktList[ArIdx], 0, sizeof(RTCX_PKT));
		}
	}
}

LSA_VOID* EDDP_LL_AllocTxDescriptor(LSA_UINT32 ArNum, LSA_UINT16 FrameID)
{
	__attribute__((unused))LSA_UINT16 Cnt = 0U;
	LSA_UINT16 MaxItems = 0U;
	ether_instance_descriptor_t *ptList = LSA_NULL;
	ether_instance_descriptor_t *ptDescriptor = LSA_NULL;

	if( (FRAMEID_RTC3_START <= FrameID) &&
		(FRAMEID_RTC3_END   >= FrameID)  )
	{
		return &g_tx_descriptors_rtc3[0];
	}
	else
	{
		ptList = &g_tx_descriptors_rtcx[0];
		MaxItems = DEV_CFG_MAX_ARS_RTC1_RTC2;
	}

#if 1
	if((LSA_NULL != ptList) && (ArNum < MaxItems))
	{
		ptDescriptor = &ptList[ArNum];
	}
#elif 0
	if((LSA_NULL != ptList) && (0U < MaxItems))
	{
		while( (LSA_NULL == ptDescriptor) && (Cnt < MaxItems) )
		{
			if( (0U == ptList[Cnt].des1) && (0U == ptList[Cnt].des2) )
			{
				ptDescriptor = &ptList[Cnt];
			}
			Cnt++;
		}
	}
#endif
	return ptDescriptor;
}

LSA_VOID EDDP_LL_FreeTxDescriptor(ether_instance_descriptor_t *ptDescriptor)
{
	__attribute__((unused))LSA_UINT16 Cnt = 0U;
	__attribute__((unused))LSA_UINT16 MaxItems = 0U;
	__attribute__((unused))ether_instance_descriptor_t *ptList = LSA_NULL;
	__attribute__((unused))LSA_UINT32 u32Descriptor = (LSA_UINT32)ptDescriptor;

	if(LSA_NULL != ptDescriptor)
	{
#if 0 	/** @ishikawa: Disable these process because this RED Tx interruption may be using this descriptor. */
		ptDescriptor->des0 = 0U;
		ptDescriptor->des1 = 0U;
		ptDescriptor->des2 = 0U;
#endif
#if 0
		if( (TX_DESCR_ADDR_RTC3_FIRST <= u32Descriptor) &&
			(TX_DESCR_ADDR_RTC3_LAST  >= u32Descriptor) )
		{
			ptList = &g_tx_descriptors_rtc3[0];
			MaxItems = DEV_CFG_MAX_ARS_RTC3;
		}
		else
		{
			if( (TX_DESCR_ADDR_RTCX_FIRST <= u32Descriptor) &&
				(TX_DESCR_ADDR_RTCX_LAST  >= u32Descriptor) )
			{
				ptList = &g_tx_descriptors_rtcx[0];
				MaxItems = DEV_CFG_MAX_ARS_RTC1_RTC2;
			}
		}
#endif
	}
#if 0
	if((LSA_NULL != ptList) && (0U < MaxItems))
	{
		while( (LSA_NULL == ptDescriptor) && (Cnt < MaxItems) )
		{
			if(u32Descriptor == (LSA_UINT32)&ptList[Cnt])
			{
				ptDescriptor->des1 = 0U;
				ptDescriptor->des2 = 0U;
			}
			Cnt++;
		}
	}
#endif
}

LSA_VOID EDDP_LL_DatXchOutput(LSA_UINT8 *pAddr, PNIO_UINT32 ArNum)
{
	RTCX_PKT_PTR pRx = &g_tRtcRxPktList[ArNum];
	RTCX_HDR_PTR_NV pRxHdr = (RTCX_HDR_PTR_NV)&pRx->Buffer[0];
	PNDV_AR_DATA_T *pAr = &pndv_data.ar[ArNum];
	OsMemCpy(pAddr, &pRxHdr->Data[0U],  (pAr->output_cr_io_data_length + pAr->output_cr_ioxs_length));
}

LSA_VOID EDDP_LL_DatXchInputAPDU(PNIO_UINT32 ArNum)
{
	EDDP_LL_HANDLE_PTR_TYPE pHandle = (EDDP_LL_HANDLE_PTR_TYPE)&g_tHandle;
	EDDP_LOCAL_DDB_PTR_TYPE pDDB    = pHandle->hDDB;
        
	RTCX_PKT_PTR pTx = &g_tRtcTxPktList[ArNum];
	EDDP_CRT_PROVIDER_PTR_TYPE pProvider = pTx->pResource;

	EDDP_CRT_PROVIDER_MNGM_PTR_TYPE pProvMngm = &pDDB->CRT.Prov;

	if(!(pTx->Status == EDD_STS_OK || pTx->Status == EDD_STS_OK_NO_DATA))
	{
		return;
	}
	EDDP_CRT_PROVIDER_DS_PTR_TYPE pDataStatus = &pProvMngm->pDSEntry[pProvider->DataStatusId];

	pTx->pApdu->data_status = 0x35U;
#ifdef EDDP_CFG_SYSRED_API_SUPPORT
        EDDP_ENTER_IO();
#endif
        switch (EDDP_HALGetPnipRevision(pDDB))  // read current DataStatus from APICtrl-RAM
        {
#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
            case EDDP_HAL_HW_PNIP_REV1:
                pTx->pApdu->data_status = EDDP_HAL_MEM8_READ(pDDB, &(pDataStatus->pHwDSApiPnipR1->DataStatus)); // get value from hw --> READ
            break;
#endif

#if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
            case EDDP_HAL_HW_PNIP_REV2:
            case EDDP_HAL_HW_PNIP_REV3:
                pTx->pApdu->data_status = EDDP_HAL_MEM8_READ(pDDB, &(pDataStatus->pHwDSApiPnipR2->DataStatus)); // get value from hw --> READ
            break;
#endif
            
            default:
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_LL_DatXchInputAPDU(): FATAL -> Version(%d) of PNIP is unknown", 
                    EDDP_HALGetPnipRevision(pDDB));
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            break;
        }
#ifdef EDDP_CFG_SYSRED_API_SUPPORT
        EDDP_EXIT_IO();
#endif
        
	pTx->pApdu->transfer_status = 0x00U;
	pTx->pApdu->cycle_counter = __REV16(g_CyclCntr * g_SendClock);//send_clock 8, 16 32, 64,
	pTx->State = 1U;
	pTx->Status = EDD_STS_OK;
}
LSA_VOID EDDP_LL_DatXchInput(LSA_UINT8 *pAddr, PNIO_UINT32 ArNum)
{
	RTCX_PKT_PTR pTx = &g_tRtcTxPktList[ArNum];

	if(!(pTx->Status == EDD_STS_OK || pTx->Status == EDD_STS_OK_NO_DATA))
	{
		return;
	}

	OsMemCpy(&pTx->Buffer[sizeof(RTCX_SND_HDR_NV)], pAddr,pTx->IoDataSize );
}

EDD_RSP EDDP_LL_K32PutRQB( EDD_UPPER_KRISC32_RQB_PTR_TYPE pK32RQB, EDDP_LOCAL_DDB_PTR_TYPE pDDB)
{
	EDD_RSP Status = EDD_STS_ERR_PARAM;
	EDDP_LL_HANDLE_PTR_TYPE pHandle =  (EDDP_LL_HANDLE_PTR_TYPE)EDDP_LL_MNG_HANDLE(pDDB->InterfaceID);
	if( (LSA_NULL != pK32RQB) && (LSA_NULL != pDDB) && (LSA_NULL != pHandle) )
	{
		if(PNIO_OK == OsSendMsgPrm1(pHandle->IfCtrlTaskId, EDDP_LL_EV_K32_REQ, (LSA_UINT32)pK32RQB))
		{
			OsTaskNotify(pHandle->IfCtrlTaskId, EDDP_LL_EV_MAIL);
			Status = EDD_STS_OK;
		}
		else
		{
			Status = EDD_STS_ERR_SERVICE;
		}
	}
	return Status;
}

LSA_VOID_PTR_TYPE EDDP_LL_K32_RRQB_GET(EDDP_LOCAL_DDB_PTR_TYPE  pDDB,
							 LSA_UINT32               EventNr )
{
	LSA_UNUSED_ARG(EventNr)
	EDD_UPPER_KRISC32_RQB_PTR_TYPE pRQB = LSA_NULL;
	EDDP_LL_HANDLE_PTR_TYPE pHandle =  (EDDP_LL_HANDLE_PTR_TYPE)EDDP_LL_MNG_HANDLE(pDDB->InterfaceID);
	EventNr = (EDDP_SW_EVENT0 - EDDP_ISR_LOW_MAX_EVENTS);

	if( (LSA_NULL != pDDB) && (LSA_NULL != pHandle) && (LSA_NULL != pHandle->hQK32Rsp))
	{
		pRQB = (EDD_UPPER_KRISC32_RQB_PTR_TYPE) OsQueRecv(pHandle->hQK32Rsp, 0U);
	}

	return ((LSA_VOID_PTR_TYPE)pRQB);
}

LSA_VOID_PTR_TYPE EDDP_LL_CmdRequestPrm(EDDP_LOCAL_DDB_PTR_TYPE pDDB)
{
	EDDP_LL_HANDLE_PTR_TYPE pHandle =  (EDDP_LL_HANDLE_PTR_TYPE)EDDP_LL_MNG_HANDLE(pDDB->InterfaceID);
	EDDP_CMD_PARAM_PTR_TYPE pParam = LSA_NULL;

	if( (LSA_NULL != pDDB) && (LSA_NULL != pHandle) && (LSA_NULL != pHandle->hQCmdRsp) )
	{
		pParam = (EDDP_CMD_PARAM_PTR_TYPE) OsQueRecv(pHandle->hQCmdRsp, 0);
	}

	return pParam;
}

EDD_RSP EDDP_LL_CmdRequest(EDDP_LOCAL_DDB_PTR_TYPE pDDB, EDDP_CMD_PARAM_PTR_TYPE pParam)
{
	EDD_RSP Status = EDD_STS_ERR_PARAM;
	EDDP_LL_HANDLE_PTR_TYPE pHandle =  (EDDP_LL_HANDLE_PTR_TYPE)EDDP_LL_MNG_HANDLE(pDDB->InterfaceID);

	if( (LSA_NULL != pDDB) && (LSA_NULL != pParam) && (LSA_NULL != pHandle)  )
	{
		Status = EDD_STS_ERR_RESOURCE;
		if (pParam->Cbf == 0)
		{
			EDDP_LL_ExeCmd(pHandle, pHandle->hDDB, pParam);
			if( EDDP_CMD_CPM_NOP == (EDDP_CMD_GROUP_MASK & pParam->CmdOpcode) )
			{
				EDDP_CRTCmdReleaseInternalCmdParamElement(pDDB);
			}
			pParam->Response = EDD_STS_OK;
			Status = EDD_STS_OK;
		}
		else
		{
			// remember calling interface for returning it later with CBF
			pParam->pDDB = pDDB;
			pParam->InUse = LSA_TRUE;
			if(PNIO_OK == OsSendMsgPrm1(pHandle->IfCtrlTaskId, EDDP_LL_EV_CMD_REQ, (LSA_UINT32)pParam))
			{
				OsTaskNotify(pHandle->IfCtrlTaskId, EDDP_LL_EV_MAIL);
				Status = EDD_STS_OK_PENDING;
			}
		}
	}
	return Status;
}

LSA_VOID EDDP_LL_ExeKrisc(EDDP_LL_HANDLE_PTR_TYPE pHandle, EDDP_LOCAL_DDB_PTR_TYPE pDDB, EDD_UPPER_KRISC32_RQB_PTR_TYPE pK32UpperRQB)
{
	__attribute__((unused))EDD_UPPER_RQB_PTR_TYPE         pRQB = LSA_NULL;
	LSA_BOOL bKriscIrq = LSA_FALSE;

	if( (LSA_NULL != pDDB) && (LSA_NULL != pK32UpperRQB) && (LSA_NULL != pHandle)  )
	{
		__attribute__((unused))EDDP_ISR_GROUP_INFO_PTR_TYPE    pGroup = &pDDB->Core.Isr.Group;

		switch (pK32UpperRQB->Hdr.Opcode)
		{
		case K32_OPC_GSY_SYNC: // EDD_SRV_PTCP_SYNC_CONTROL
			//pRQB =(EDD_UPPER_RQB_PTR_TYPE) pK32UpperRQB->Hdr.UserId.void_ptr;
			// TODO: SYNC Control.....
#if 0
			pK32UpperRQB->Params.SyncReq.SyncId; 			// 0 = clock, 1 = time
			pK32UpperRQB->Params.SyncReq.SlaveState;		// START, OFF
			pK32UpperRQB->Params.SyncReq.MasterState;		// START, ACTIVE, SLEEP, OFF
			pK32UpperRQB->Params.SyncReq.LengthTLV;			// Length of TLV chain in SyncFrame (24 or 52)
			pK32UpperRQB->Params.SyncReq.PLLWindow;			// max. offset of slave time from master in nanoseconds
			pK32UpperRQB->Params.SyncReq.SyncSendFactor;	// Master send interval in milliseconds
			pK32UpperRQB->Params.SyncReq.TakeoverFactor;	// Master takeover timeout as multiple of master send interval
			pK32UpperRQB->Params.SyncReq.TimeoutFactor;		// Slave timeout als  as multiple of master send interval
			pK32UpperRQB->Params.SyncReq.TLV; 				// SyncFrame TLVs: Subdomain [,Time,TimeExt, Master]  // EDDP_MEMCOPY_COMMON(pK32UpperRQB->Params.SyncReq.TLV,pParam->TLV, pParam->LengthTLV);
#endif
			bKriscIrq = LSA_TRUE;
			break;

		case K32_OPC_GSY_DELAY_REQ:
			// TODO: Build PTCP-Delay-Req. Frame and send it out via specified PortID!!

		//	EDDP_NRT_TX_GRP_SYNC;

#if 0
			pRQB =(EDD_UPPER_RQB_PTR_TYPE) pK32UpperRQB->Hdr.UserId.void_ptr;
			if( (LSA_NULL != pRQB) && (LSA_NULL != pRQB->pParam) )
			{
				EDD_UPPER_PTCP_DELAY_CONTROL_PTR_TYPE pParam = (EDD_UPPER_PTCP_DELAY_CONTROL_PTR_TYPE)pRQB->pParam;
				pParam->PortMacAddr.MacAdr[0];
			}
			pK32UpperRQB->Params.DelayReq.PortID;
			pK32UpperRQB->Params.DelayReq.ReqActive;
			pK32UpperRQB->Params.DelayReq.RspActive;
			pK32UpperRQB->Params.DelayReq.RxPortDelay;
			pK32UpperRQB->Params.DelayReq.TxPortDelay;
#endif
			bKriscIrq = LSA_TRUE;
			break;

		case K32_OPC_START:
		case K32_OPC_STOP:
		case K32_OPC_SETUP:
		case K32_OPC_GSY_TOPO_STATE:
			bKriscIrq = LSA_TRUE;
			break;

		case K32_OPC_GSY_DELAY_IND:

			break;

		case K32_OPC_GSY_DIAG:


		default:
			EDDP_K32_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] *** EDDP_K32AckRQB() Invalid opcode=%u", Opcode);
			break;
		}


		if(LSA_TRUE == bKriscIrq)
		{
			if(PNIO_OK == OsQueSend(pHandle->hQK32Rsp, (LSA_UINT32)pK32UpperRQB, 0U))
			{
				pK32UpperRQB->Hdr.Response = K32_RSP_OK;
				pHandle->dwEddpIrqMid |= (1UL << (EDDP_SW_EVENT0 - EDDP_ISR_LOW_MAX_EVENTS));
			}
		}

	}

}


/**
 * \brief           Act the LLIF behavior the EDD_SRV_SET_REMOTE_PORT_STATE service
 *                  This function is called after the process of ERTEC native EDDP.
 *
 * \param[in]       pRQB    Pointer to Request Block
 * \param[in]       pHDB    Pointer to Handler management Block
 *
 * \retval          void
 */
static LSA_VOID EDDP_LL_ActSetRemotePortStateService(EDD_UPPER_RQB_PTR_TYPE pRQB, EDDP_LOCAL_HDB_PTR_TYPE pHDB)
{
	LSA_UNUSED_ARG(pRQB);

    /** Control RTC3 frame forwarding */
    EDDP_LL_ControlRTC3MulticastFowardingDependingOnPortStatus(pHDB->pDDB);

    return;
}

EDD_RSP EDDP_LL_CoreRequest(EDD_UPPER_RQB_PTR_TYPE pRQB, EDDP_LOCAL_HDB_PTR_TYPE pHDB)
{
	EDD_RSP Response = EDD_STS_ERR_PARAM;

	if( (LSA_NULL != pRQB) && (LSA_NULL != pHDB) )
	{
		EDD_SERVICE Service = EDD_RQB_GET_SERVICE(pRQB);
		Response    = EDD_STS_OK;

		switch(Service)
		{
		//case EDD_SRV_SYNC_TYPE:
		case EDD_SRV_SYNC_RECV:
			Response = EDDP_NRTRecv(pRQB, pHDB->pDDB, pHDB);
			break;
		case EDD_SRV_SYNC_SEND:
			Response    = EDDP_NRTSend(pRQB, pHDB->pDDB, pHDB);//-OS-EDD_STS_OK;
			break;
		case EDD_SRV_SYNC_CANCEL:
		case EDD_SRV_SYNC_SET_RATE:
			Response    = EDD_STS_OK;
			break;
	/*=====  EDD_OPC_REQUEST: PTCP-Services (0x6100..0x61FF)  ====*/
		//case EDD_SRV_PTCP_TYPE:
		case EDD_SRV_PTCP_DELAY_CONTROL:
		case EDD_SRV_PTCP_SYNC_CONTROL:
		case EDD_SRV_PTCP_DIAG_IND_PROVIDE:
			Response    = EDD_STS_OK;
			break;

        case EDD_SRV_SET_REMOTE_PORT_STATE:
            EDDP_LL_ActSetRemotePortStateService(pRQB, pHDB);

		default:
			Response = EDD_STS_ERR_SERVICE;
			EDD_RQB_SET_RESPONSE(pRQB, Response);
			EDDP_UPPER_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "EDDP_CoreUserRequest(): ERROR -> Unknown/Unsupported Service! (Service: 0x%X, pHDB: 0x%X)",
				Service, pHDB);
			break;
		}
	}
	return Response;
}

LSA_VOID EDDP_LL_LINK_CHANGE_DONE(EDDP_LOCAL_DDB_PTR_TYPE  pDDB)
{
	EDDP_LL_HANDLE_PTR_TYPE pHandle =  (EDDP_LL_HANDLE_PTR_TYPE)EDDP_LL_MNG_HANDLE(pDDB->InterfaceID);
	ether_instance_ctrl_t * ptIfCtrl = EDDP_LL_ETH_CTRL_PTR(pHandle->pIfHandle);

	if( (LSA_NULL != pHandle) && (LSA_NULL != pHandle->pIfHandle) &&
		(LSA_NULL != ptIfCtrl) && (LSA_NULL != ptIfCtrl->p_ether_cfg)  &&
		(LSA_NULL != ptIfCtrl->p_ether_cfg->p_ether_phy_instance) &&
		(LSA_NULL != ptIfCtrl->p_ether_cfg->p_ether_switch_instance) )
	{
		LSA_UINT16 PortID = 0U;
		ether_phy_instance_t *ptPhy;
		ether_phy_instance_t *ptPhyInst;
		ether_switch_instance_t *ptSwi;
		EDDP_LL_PORTCTRL_PTR_TYPE ptPortCtrl;
		EDDP_LINK_STAT_PTR_TYPE  ptLinkStat;
		uint32_t     link_speed_duplex  = 0U;
		uint32_t     local_pause_bits   = 0U;
		uint32_t     partner_pause_bits = 0U;
		uint32_t     transmit_pause_set = 0U;
		uint32_t     receive_pause_set  = 0U;
		LSA_UINT32   LinkMode           = 0U;
		LSA_UINT32   LinkSpeed          = 0U;
		LSA_UINT32   MAUType        	= 0U;
		LSA_UINT8    LinkStatus 		= EDD_LINK_DOWN;
		LSA_UINT32	 dwLinkStatusAll = pHandle->dwLinkStatusAll;
		volatile R_GMAC_Type *  p_reg_etherc = (R_GMAC_Type*) ptIfCtrl->p_reg_etherc;
		ptSwi = (ether_switch_instance_t*)ptIfCtrl->p_ether_cfg->p_ether_switch_instance;
		ptPhyInst =  (ether_phy_instance_t*)ptIfCtrl->p_ether_cfg->p_ether_phy_instance;
	//	ptPhy = (ether_phy_instance_t*)&ptPhyInst[0];

		while(PortID < EDD_CFG_MAX_PORT_CNT)
		{
			ptPhy = (ether_phy_instance_t*)&ptPhyInst[PortID];
			ptIfCtrl->link_proc_port = (1U << PortID);
			ptPortCtrl  = &pHandle->tPortCtrl[PortID];
			ptLinkStat  = &ptPortCtrl->tLinkStat;
			LinkStatus 	= EDD_LINK_DOWN;

			if( (EDDP_PHY_POWERUP == ptPortCtrl->PhyPowerMode) &&
				(EDD_STS_OK == EDDP_LL_LinkStatToHost(ptPortCtrl->LinkSpeedModeCfg, LSA_NULL, LSA_NULL, LSA_NULL, LSA_NULL )) )
			{
				if(FSP_SUCCESS == ptPhy->p_api->linkStatusGet(ptPhy->p_ctrl))
				{
					LinkStatus = EDD_LINK_UP;
				}
				if(LinkStatus == EDD_LINK_UP)
				{
					if(EDD_LINK_UP != ptLinkStat->Status)
					{
						if(EDD_LINK_AUTONEG == ptPortCtrl->LinkSpeedModeCfg)
						{
							if(EDD_AUTONEG_OFF == ptLinkStat->Autoneg)
							{
								if(FSP_SUCCESS == ptPhy->p_api->startAutoNegotiate(ptPhy->p_ctrl))
								{
									ptLinkStat->Autoneg = EDD_AUTONEG_ON;
								}
							}
							else
							{
								if(FSP_SUCCESS == ptPhy->p_api->linkPartnerAbilityGet(ptPhy->p_ctrl, &link_speed_duplex, &local_pause_bits, &partner_pause_bits))
								{
									ptLinkStat->Autoneg = EDD_AUTONEG_OFF;
									if( (EDD_STS_OK  == EDDP_LL_LinkStatToEddp(link_speed_duplex, &LinkSpeed, &LinkMode, &MAUType)) &&
										(FSP_SUCCESS == R_ETHER_ACCESS_Set_convctrl (ptIfCtrl->p_reg_access, PortID, link_speed_duplex)) &&
										(FSP_SUCCESS == ptSwi->p_api->confSpeed( ptSwi->p_ctrl, (ether_switch_port_t)PortID, link_speed_duplex)) )
									{
										ptLinkStat->Status = EDD_LINK_UP;
										ptLinkStat->Speed   = LinkSpeed;
										ptLinkStat->Mode    = LinkMode;
										ptLinkStat->MAUType = MAUType;
										ptLinkStat->Autoneg = EDD_AUTONEG_OFF;
										ptIfCtrl->link_establish_status[PortID] = ETHER_LINK_STATUS_UP;
										ptIfCtrl->link_change[PortID] = ETHER_LINK_CHANGE_LINK_UP;
										pHandle->dwLinkStatusAll |= (1U << PortID);
										PNIP_HAL_PHY_SET_LINK((PortID+1), ptLinkStat->Status, LinkSpeed, LinkMode);
									}
								}
							}
						}
						else   // Not Autoneg
						{
							if( (EDD_STS_OK == EDDP_LL_LinkStatToHost(ptPortCtrl->LinkSpeedModeCfg, &LinkSpeed, &LinkMode, &MAUType, (unsigned long *)&link_speed_duplex)) &&
								(FSP_SUCCESS == R_ETHER_ACCESS_Set_convctrl (ptIfCtrl->p_reg_access, PortID, link_speed_duplex)) &&
								(FSP_SUCCESS == ptSwi->p_api->confSpeed( ptSwi->p_ctrl, (ether_switch_port_t)PortID, link_speed_duplex)) )
							{
								ptLinkStat->Status = EDD_LINK_UP;
								ptLinkStat->Mode    = LinkMode;
								ptLinkStat->Speed   = LinkSpeed;
								ptLinkStat->MAUType	= MAUType;
								ptLinkStat->Autoneg = EDD_AUTONEG_OFF;
								ptIfCtrl->link_establish_status[PortID] = ETHER_LINK_STATUS_UP;
								ptIfCtrl->link_change[PortID] = ETHER_LINK_CHANGE_LINK_UP;
								pHandle->dwLinkStatusAll |= (1U << PortID);
								PNIP_HAL_PHY_SET_LINK((PortID+1), EDD_LINK_UP, ptLinkStat->Speed, ptLinkStat->Mode);   // Axel: Sets basicall only REGSTATUS register
							}
						}
					}   // Link Up and was down
					else
					{
#if 0
						if(LSA_TRUE == ptPortCtrl->SetLinkPending)
						{
							PNIP_HAL_PHY_SET_LINK((PortID+1), EDD_LINK_UP, ptLinkStat->Speed, ptLinkStat->Mode);
							ptPortCtrl->SetLinkPending = LSA_FALSE;
						}
#endif
					}

					if( (0U == dwLinkStatusAll) && (dwLinkStatusAll != pHandle->dwLinkStatusAll) )  // Got new good Link status
					{
						dwLinkStatusAll = pHandle->dwLinkStatusAll;

						if( (EDD_LINK_MODE_FULL == LinkMode) &&
							(ETHER_FLOW_CONTROL_ENABLE == ptIfCtrl->p_ether_cfg->flow_control) )
						{
							/* pause frame can be used when  when passing through the switch */
							/* NOTE: not confirmed this operation !!! */
							uint32_t flow_control = ETHER_FLOW_CONTROL_PT |      /* Pause Time (b31-b16) */
													ETHER_FLOW_CONTROL_PLT|      /* Pause Low Threshold */
													ETHER_FLOW_CONTROL_FCA_BPA;  /* Flow Control Busy or Backpressure Activate */

							/** Enable PAUSE for full duplex link depending on the pause resolution results */
							ether_pause_resolution(local_pause_bits, partner_pause_bits, &transmit_pause_set, &receive_pause_set);

							if (ETHER_PAUSE_XMIT_ON == transmit_pause_set)
							{
								/* Enable automatic PAUSE frame transmission */
								flow_control |= ETHER_FLOW_CONTROL_TFE; /* Transmit Flow Control Enable */
							}

							if (ETHER_PAUSE_RECV_ON == receive_pause_set)
							{
								/* Enable reception of PAUSE frames */
								flow_control |= ETHER_FLOW_CONTROL_RFE; /* Receive Flow Control Enable */
							}

							p_reg_etherc->Flow_Control = flow_control;
						}   // end flow control


						/* Enable receive and transmit. */
						p_reg_etherc->MAC_Configuration_b.RE = 1;
						p_reg_etherc->MAC_Configuration_b.TE = 1;
					}
				}  // Link is up
				else
				{        // Link down
					if( (EDD_AUTONEG_ON != ptLinkStat->Autoneg) &&
						(EDD_LINK_DOWN != ptLinkStat->Status) )
					{
						PNIP_HAL_PHY_SET_LINK((PortID+1), EDD_LINK_DOWN, 0, 0);

						ptLinkStat->Status  = EDD_LINK_DOWN;
						ptLinkStat->Speed   = EDD_LINK_UNKNOWN;
						ptLinkStat->Mode    = EDD_LINK_UNKNOWN;
						ptLinkStat->MAUType = EDD_MAUTYPE_UNKNOWN;
						ptLinkStat->Autoneg = EDD_AUTONEG_OFF;
						ptIfCtrl->link_establish_status[PortID] = ETHER_LINK_STATUS_DOWN;
						ptIfCtrl->link_change[PortID] = ETHER_LINK_CHANGE_LINK_DOWN;
						pHandle->dwLinkStatusAll &= (~(1UL << PortID));
						ethsw_queue_flush((1UL << PortID), QFLUSH_QUEUE_MASK_ALL, QFLUSH_ACTION_CLOSE2);

#if 0
						EDDP_LL_PHY_CTRL(pHandle, 1, EDDP_PHY_CLOSE, LSA_FALSE);
						EDDP_LL_PHY_CTRL(pHandle, 2, EDDP_PHY_CLOSE, LSA_FALSE);
						EDDP_LL_PHY_CTRL(pHandle, 1, EDDP_PHY_POWERUP, LSA_FALSE);
						EDDP_LL_PHY_CTRL(pHandle, 2, EDDP_PHY_POWERUP, LSA_FALSE);
#endif

					}

					if( (EDD_LINK_AUTONEG == ptPortCtrl->LinkSpeedModeCfg) &&
						(EDD_AUTONEG_OFF == ptLinkStat->Autoneg) &&
						(FSP_SUCCESS == ptPhy->p_api->startAutoNegotiate(ptPhy->p_ctrl)) )
					{
						ptLinkStat->Autoneg = EDD_AUTONEG_ON;
					}
				}   // Link is down
			}

			local_pause_bits    = 0U;
			partner_pause_bits  = 0U;
			link_speed_duplex   = 0U;
			LinkMode            = 0U;
			LinkSpeed           = 0U;

			PortID++;
		}

	}
}

static int syncIdx = 0;
LSA_VOID EDDP_LL_NRT_RECV_DONE(EDDP_LOCAL_DDB_PTR_TYPE pDDB)
{
	EDDP_LL_HANDLE_PTR_TYPE pHandle =  (EDDP_LL_HANDLE_PTR_TYPE)EDDP_LL_MNG_HANDLE(pDDB->InterfaceID);
	ether_instance_ctrl_t *ptIfCtrl = EDDP_LL_ETH_CTRL_PTR(pHandle->pIfHandle);

	if( (NULL != pHandle) && (EDDP_LLIF_STATE_OPERATIONAL == pHandle->u16State) && (LSA_NULL != pHandle->pRxBuf) &&
		(LSA_NULL != pHandle->hDDB) && (LSA_NULL != ptIfCtrl) && (LSA_NULL != ptIfCtrl->p_reg_etherc) )
	{
		LSA_UINT32 nFrameSize = 0U;
		LSA_UINT32 PortID = 0U;

		uint32_t received_size = 0U;
		ether_frame_mgtag_t *p_ether_frame_mgtag = NULL;

		LSA_UINT8   nBufCnt = 0U;
		LSA_UINT8  *pRcvBuf = LSA_NULL;

		EDDP_LL_LOCK_ENTER(pHandle->IfCtrlLock);

		while(nBufCnt++ < NUM_RX_DESCRIPTOR)
		{
			if(ETHER_RDES0_OWN != (ptIfCtrl->p_rx_descriptor->des0 & ETHER_RDES0_OWN))
			{
        		pRcvBuf = (LSA_UINT8*)ptIfCtrl->p_rx_descriptor->des2;
				p_ether_frame_mgtag = (ether_frame_mgtag_t *)ptIfCtrl->p_rx_descriptor->des2;
				received_size = (ptIfCtrl->p_rx_descriptor->des0 & ETHER_RDES0_FL_MASK) >> ETHER_RDES0_FL_SHIFT;

				if( ( (ETHER_RDES0_ES != (ptIfCtrl->p_rx_descriptor->des0 & ETHER_RDES0_ES)) && (0U < pHandle->dwLinkStatusAll) &&
					  ((ETHER_RDES0_FS|ETHER_RDES0_LS) == (ptIfCtrl->p_rx_descriptor->des0 & (ETHER_RDES0_FS|ETHER_RDES0_LS))) &&
					  ((6U + 6U + 2U + 2U + 4U) <= received_size) && (ETHER_BUFFER_SIZE >= received_size ) ) )
				{
					nFrameSize = (received_size - ( sizeof(p_ether_frame_mgtag->mgtag) + ETHER_FCS_SIZE) );

					PortID = (1U << ((p_ether_frame_mgtag->mgtag.control_data[1] & ETHER_MANTAG_CONTROL_DATA1_PORT_MASK) >> ETHER_MANTAG_CONTROL_DATA1_PORT_SHIFT));

					// Axel Copied this here so it wont be overwritten be next frame
					LSA_UINT32 nsec = ((uint32_t)p_ether_frame_mgtag->mgtag.control_data2[0] << 24)
													| ((uint32_t)p_ether_frame_mgtag->mgtag.control_data2[1] << 16)
													| ((uint32_t)p_ether_frame_mgtag->mgtag.control_data2[2] << 8)
													| (uint32_t)p_ether_frame_mgtag->mgtag.control_data2[3];
					
					if ((nsec < SWITCH_RX_TS_DELAY) && ((p_ether_frame_mgtag->mgtag.control_data[1] & ETHER_MANTAG_CONTROL_DATA1_TIME_MASK) == 0)) 
					{
						nsec += 1000000000;
					}
					/* Subtract delay from actual timestamp to measurement within switch */
					nsec = nsec - SWITCH_RX_TS_DELAY;
          LSA_UINT16 FrameID = __REV16 (*((LSA_UINT16 *)p_ether_frame_mgtag->data)); 
					if(
#ifdef GPTP_PROTO
						  *((LSA_UINT16 *)p_ether_frame_mgtag->type) == 0xf788 /* gPTP */
#endif
                        (*((LSA_UINT16 *)p_ether_frame_mgtag->type) == ETH_TYPE_PROFINET)
						&&(*((LSA_UINT16 *)p_ether_frame_mgtag->data) == FRAMEID_DELAY_REQ
                          || *((LSA_UINT16 *)p_ether_frame_mgtag->data) == FRAMEID_DELAY_FURSP
                          || *((LSA_UINT16 *)p_ether_frame_mgtag->data) == FRAMEID_DELAY_FU
                          || *((LSA_UINT16 *)p_ether_frame_mgtag->data) == FRAMEID_DELAY_RSP

                          || ((FRAMEID_SYNC_MIN   <= FrameID) && (FRAMEID_SYNC_MAX   >= FrameID))
                          || ((FRAMEID_FUSYNC_MIN <= FrameID) && (FRAMEID_FUSYNC_MAX >= FrameID))
                          || ((FRAMEID_FU_MIN     <= FrameID) && (FRAMEID_FU_MAX     >= FrameID))
						))
					{
						RSyncReceive[syncIdx][PortID-1].pBuffer    = pRcvBuf;
						RSyncReceive[syncIdx][PortID-1].RxTime     = nsec;
						RSyncReceive[syncIdx][PortID-1].PortID     = PortID;
						RSyncReceive[syncIdx][PortID-1].RequestCnt = nFrameSize;
						/* check if RSync has started, otherwise no queue available */
            if ((FRAMEID_SYNC_MIN   <= FrameID) && (FRAMEID_SYNC_MAX   >= FrameID))
            {
              //printeth("syncrx");
            }
						if(NULL != rxTsQueue[PortID-1])
						{
							if(0 == OsQueSend(rxTsQueue[PortID-1], (PNIO_UINT32)&RSyncReceive[syncIdx][PortID-1], 0U))
							{
								/* queue should not pile up, otherwise TS's are outdated */
								xQueueReset(rxTsQueue[PortID-1]);
							}
							else
							{
								++syncIdx;
								syncIdx = syncIdx % 10;
							}
						}
					}
					else
					{
						EDDP_LL_FilterFrame((LSA_VOID_PTR_TYPE)pHandle, PortID, pRcvBuf, nFrameSize, nsec);
					}
				}
				ptIfCtrl->p_rx_descriptor->des0 |= ETHER_RDES0_OWN;
			}
			else
			{
				break;
			}

			if ( ETHER_RDES1_RER == (ptIfCtrl->p_rx_descriptor->des1 &  ETHER_RDES1_RER))
			{
				ptIfCtrl->p_rx_descriptor = ptIfCtrl->p_ether_cfg->p_rx_descriptors;
			}
			else
			{
				ptIfCtrl->p_rx_descriptor++;
			}
		}

		if(0U < pHandle->dwRxStatReg)
		{
			PNIP_HAL_REG32_WRITE( PNIP_REG_RCVSTATUS_IFA, (EDDP_ARD_NRT_USED_RINGS & pHandle->dwRxStatReg) );
			pHandle->dwRxStatReg = 0U;
		}

	}
}

LSA_VOID EDDP_LL_CTRL_TASK(LSA_VOID)
{
	PNIO_UINT32 u32Events = 0U;
	EDDP_LL_HANDLE_PTR_TYPE pHandle;
	EDDP_ISR_GROUP_INFO_PTR_TYPE pGroup;
	EDDP_LOCAL_DDB_PTR_TYPE   pDDB;

	pHandle =  (EDDP_LL_HANDLE_PTR_TYPE)EDDP_LL_MNG_HANDLE(1);

	while( (LSA_NULL == pHandle) || (LSA_NULL == pHandle->pIfHandle) )
	{
		pHandle = EDDP_LL_CAST_HDL(EDDP_LL_MNG_HANDLE(1));
		OsWait_ms(10);
	}

	pDDB = (EDDP_LOCAL_DDB_PTR_TYPE)pHandle->hDDB;
	pGroup = &pDDB->Core.Isr.Group;

	OsGiveSemB(pHandle->IfCtrlLock);
	OsGiveSemB(pHandle->IfSendLock);

	while(1)
	{
		u32Events = OsTaskNotifyWait(pHandle->IfCtrlTaskId);

		if(0U < (EDDP_LL_EV_LINK & u32Events) )
		{
			pHandle->dwEddpIrqLow |= PNIP_REG_IRQ2EVENT_LOW_0__MSK_INT_STATUSCHANGE;
		}

		if(0U < (EDDP_LL_EV_DHTEXPIRE & u32Events) )
		{
			EDDP_LL_ConsumerDhtExpireCheck(pDDB->InterfaceID);
		}

		EDDP_LL_CheckMailbox(pHandle, 0);

		if(0U < pHandle->dwEddpIrqLow)
		{
			PNIP_HAL_REG32_WRITE(PNIP_REG_IRQ2EVENT_LOW_0, pHandle->dwEddpIrqLow);
			pHandle->bEddpIrqGrp = LSA_TRUE;
			pHandle->dwEddpIrqLow = 0U;
		}

		if(0U < pHandle->dwEddpIrqMid)
		{
			PNIP_HAL_REG32_WRITE(PNIP_REG_IRQ2EVENT_MID_0, pHandle->dwEddpIrqMid);
			pHandle->bEddpIrqGrp = LSA_TRUE;
			pHandle->dwEddpIrqMid = 0U;
		}

		if(LSA_TRUE == pHandle->bEddpIrqGrp)
		{
			pHandle->bEddpIrqGrp = LSA_FALSE;
			pGroup->IntRQB.InUse = LSA_TRUE; /* RQB is underway.. */
			EDDP_DO_INTERRUPT_REQUEST(pDDB->hSysDev, pGroup->IntRQB.pRQB);
		}
	}
}

LSA_VOID EDDP_LL_CheckMailbox(EDDP_LL_HANDLE_PTR_TYPE pHandle, LSA_UINT32 Timeout)
{
	if(LSA_NULL != pHandle)
	{
		EDDP_LOCAL_DDB_PTR_TYPE   pDDB = (EDDP_LOCAL_DDB_PTR_TYPE)pHandle->hDDB;
		__attribute__((unused))EDDP_ISR_GROUP_INFO_PTR_TYPE    pGroup = &pDDB->Core.Isr.Group;
		OS_MSG_PTR_TYPE ptMsg = (OS_MSG_PTR_TYPE) OsReadMsgExt(pHandle->IfCtrlTaskId, Timeout);

		while(LSA_NULL != ptMsg)
		{
			LSA_UINT32 *pPrm = (LSA_UINT32*)ptMsg->pvData;

			switch(ptMsg->MsgId)
			{
			case EDDP_LL_EV_CMD_REQ:
				if(LSA_NULL != pPrm)
				{
					EDDP_LL_ExeCmd(pHandle, pHandle->hDDB, (EDDP_CMD_PARAM_PTR_TYPE)pPrm[0]);
				}
				break;
			case EDDP_LL_EV_K32_REQ:
				if(LSA_NULL != pPrm)
				{
					EDDP_LL_ExeKrisc(pHandle, pHandle->hDDB, (EDD_UPPER_KRISC32_RQB_PTR_TYPE)pPrm[0]);
				}
				break;
			default:
				break;
			}

			OsFreeMsgExt(ptMsg);

			ptMsg = (OS_MSG_PTR_TYPE) OsReadMsgExt(pHandle->IfCtrlTaskId, 0);

		}
	}
}

LSA_UINT32 eddp_ll_PhyRegRead(LSA_UINT32 InterfaceID, LSA_UINT16 PortID, LSA_UINT32 Reg)
{
	LSA_UINT32 u32Value = 0U;
	EDDP_LL_HANDLE_PTR_TYPE pHandle = EDDP_LL_MNG_HANDLE(InterfaceID);
	if( (LSA_NULL != pHandle) && (0U < PortID) && (PortID <= 3 ) )
	{
		ether_instance_ctrl_t *ptIfCtrl = EDDP_LL_ETH_CTRL_PTR(pHandle->pIfHandle);
		ether_phy_instance_t *ptPhyInst = (ether_phy_instance_t*)ptIfCtrl->p_ether_cfg->p_ether_phy_instance;
		ether_phy_instance_t *ptPhy = (ether_phy_instance_t*)&ptPhyInst[(PortID-1)];
		ether_phy_instance_ctrl_t *p_instance_ctrl = (ether_phy_instance_ctrl_t *) ptPhy->p_ctrl;
		if( (LSA_NULL != p_instance_ctrl) && (ETHER_PHY_OPEN == p_instance_ctrl->open) )
		{
			switch(Reg)
			{
			case ETHER_PHY_REG_CONTROL:
			case ETHER_PHY_REG_STATUS:
			case ETHER_PHY_REG_IDENTIFIER1:
			case ETHER_PHY_REG_IDENTIFIER2:
			case ETHER_PHY_REG_AN_ADVERTISEMENT:
			case ETHER_PHY_REG_AN_LINK_PARTNER:
			case ETHER_PHY_REG_AN_EXPANSION:
			case ETHER_PHY_REG_1000BT_CONTROL:
			case ETHER_PHY_REG_1000BT_STATUS:
				u32Value = ether_phy_read(ptPhy->p_ctrl, Reg);
				if(ETHER_PHY_REG_STATUS == Reg)
				{
					u32Value = ether_phy_read(ptPhy->p_ctrl, Reg);
				}
				break;
			default:
				break;
			}
		}
	}

	return u32Value;
}

LSA_UINT32 eddp_ll_PhyRegWrite(LSA_UINT32 InterfaceID, LSA_UINT16 PortID, LSA_UINT32 Reg,  LSA_UINT32 Val)
{
	EDDP_LL_HANDLE_PTR_TYPE pHandle = EDDP_LL_MNG_HANDLE(InterfaceID);
	if( (LSA_NULL != pHandle) && (0U < PortID) && (PortID <= 3 ) )
	{
		ether_instance_ctrl_t *ptIfCtrl = EDDP_LL_ETH_CTRL_PTR(pHandle->pIfHandle);
		ether_phy_instance_t *ptPhyInst = (ether_phy_instance_t*)ptIfCtrl->p_ether_cfg->p_ether_phy_instance;
		ether_phy_instance_t *ptPhy = (ether_phy_instance_t*)&ptPhyInst[(PortID-1)];
		ether_phy_instance_ctrl_t *p_instance_ctrl = (ether_phy_instance_ctrl_t *) ptPhy->p_ctrl;
		if( (LSA_NULL != p_instance_ctrl) && (ETHER_PHY_OPEN == p_instance_ctrl->open) )
		{
				ether_phy_write(ptPhy->p_ctrl, Reg, Val);
		}
	}
}

EDD_RSP EDDP_LL_LinkStatToEddp(LSA_UINT32 LinkCfg, LSA_UINT32 *pSpeed, LSA_UINT32 *pMode, LSA_UINT32 *pMAUType)
{
	EDD_RSP Status = EDD_STS_OK;
	LSA_UINT32 LinkSpeed = EDD_LINK_UNKNOWN;
	LSA_UINT32 LinkMode  = EDD_LINK_UNKNOWN;
	LSA_UINT32 MAUType   = EDD_MAUTYPE_UNKNOWN;

	switch (LinkCfg)
	{
		case ETHER_PHY_LINK_SPEED_100F:
			LinkSpeed = EDD_LINK_SPEED_100;
			LinkMode  = EDD_LINK_MODE_FULL;
			MAUType   = EDDP_PHY_MAUTYPE_100BASETXFD;
			break;
		case ETHER_PHY_LINK_SPEED_100H:
			LinkSpeed = EDD_LINK_SPEED_100;
			LinkMode  = EDD_LINK_MODE_HALF;
			MAUType   = EDDP_PHY_MAUTYPE_100BASETXHD;
			break;
		case ETHER_PHY_LINK_SPEED_10F:
			LinkSpeed = EDD_LINK_SPEED_10;
			LinkMode  = EDD_LINK_MODE_FULL;
			MAUType   = EDDP_PHY_MAUTYPE_10BASETXFD;
			break;
		case ETHER_PHY_LINK_SPEED_10H:
			LinkSpeed = EDD_LINK_SPEED_10;
			LinkMode  = EDD_LINK_MODE_HALF;
			MAUType   = EDDP_PHY_MAUTYPE_10BASETXHD;
			break;
		default:
			Status = EDD_STS_ERR_PARAM;
			break;
	}

	if(LSA_NULL != pSpeed)
	{
		*((LSA_UINT32*)pSpeed) = (LSA_UINT32)LinkSpeed;
	}

	if(LSA_NULL != pMode)
	{
		*((LSA_UINT32*)pMode) = (LSA_UINT32)LinkMode;
	}

	if(LSA_NULL != pMAUType)
	{
		*((LSA_UINT32*)pMAUType) = (LSA_UINT32)MAUType;
	}

	return Status;
}

EDD_RSP EDDP_LL_LinkStatToHost(LSA_UINT32 EddpLinkCfg, LSA_UINT32 *pSpeed, LSA_UINT32 *pMode, LSA_UINT32 *pMAUType, LSA_UINT32 *pHostLinkCfg)
{
	EDD_RSP Status = EDD_STS_OK;
	LSA_UINT32 HostLink = 0U;
	LSA_UINT32 LinkSpeed = EDD_LINK_UNKNOWN;
	LSA_UINT32 LinkMode  = EDD_LINK_UNKNOWN;
	LSA_UINT32 MAUType   = EDD_MAUTYPE_UNKNOWN;

	switch (EddpLinkCfg)
	{
	case EDD_LINK_AUTONEG:
		HostLink = ETHER_PHY_LINK_SPEED_NO_LINK;
		break;
	case EDD_LINK_100MBIT_HALF:
		HostLink = ETHER_PHY_LINK_SPEED_100H;
		LinkSpeed = EDD_LINK_SPEED_100;
		LinkMode  = EDD_LINK_MODE_HALF;
		MAUType   = EDDP_PHY_MAUTYPE_100BASETXHD;
		break;
	case EDD_LINK_100MBIT_FULL:
		HostLink = ETHER_PHY_LINK_SPEED_100F;
		LinkSpeed = EDD_LINK_SPEED_100;
		LinkMode  = EDD_LINK_MODE_FULL;
		MAUType   = EDDP_PHY_MAUTYPE_100BASETXFD;
		break;
	case EDD_LINK_10MBIT_HALF:
		HostLink = ETHER_PHY_LINK_SPEED_10H;
		LinkSpeed = EDD_LINK_SPEED_10;
		LinkMode  = EDD_LINK_MODE_HALF;
		MAUType   = EDDP_PHY_MAUTYPE_10BASETXHD;
		break;
	case EDD_LINK_10MBIT_FULL:
		HostLink = ETHER_PHY_LINK_SPEED_10F;
		LinkSpeed = EDD_LINK_SPEED_10;
		LinkMode  = EDD_LINK_MODE_FULL;
		MAUType   = EDDP_PHY_MAUTYPE_10BASETXFD;
		break;
	case EDD_LINK_DISABLED:
		HostLink = ETHER_PHY_LINK_SPEED_NO_LINK;
	    break;
    
	default:
		Status = EDD_STS_ERR_PARAM;
		break;
	}

	if(LSA_NULL != pSpeed)
	{
		*((LSA_UINT32*)pSpeed) = (LSA_UINT32)LinkSpeed;
	}

	if(LSA_NULL != pMode)
	{
		*((LSA_UINT32*)pMode) = (LSA_UINT32)LinkMode;
	}

	if(LSA_NULL != pMAUType)
	{
		*((LSA_UINT32*)pMAUType) = (LSA_UINT32)MAUType;
	}

	if(LSA_NULL != pHostLinkCfg)
	{
		*((LSA_UINT32*)pHostLinkCfg) = (LSA_UINT32)HostLink;
	}

	return Status;
}

EDD_RSP EDDP_LL_GET_LINK_STATUS(LSA_UINT32 InterfaceID, LSA_UINT16 PortID, LSA_UINT32 *pLink, LSA_UINT32 *pSpeed, LSA_UINT32 *pDuplexity)
{
	EDD_RSP Status = EDD_STS_ERR_PARAM;
	EDDP_LL_HANDLE_PTR_TYPE pHandle = EDDP_LL_MNG_HANDLE(InterfaceID);
	if( (LSA_NULL != pHandle) && (0U < PortID) && (PortID <= 3 ) )
	{
		Status = EDD_STS_OK;
	}

	return Status;
}

EDD_RSP EDDP_LL_PHY_CTRL(EDDP_LL_HANDLE_PTR_TYPE pHandle, LSA_UINT32 PortID, LSA_UINT32 PhyCtrl, LSA_BOOL bUpdateReg)
{
	EDD_RSP Status = EDD_STS_ERR_PARAM;

	ether_instance_ctrl_t * ptIfCtrl = EDDP_LL_ETH_CTRL_PTR(pHandle->pIfHandle);

	if( (LSA_NULL != pHandle) && (1U <= PortID)  && (PortID <= EDD_CFG_MAX_PORT_CNT ) &&
		(LSA_NULL != ptIfCtrl) && (LSA_NULL != ptIfCtrl->p_ether_cfg)  &&
		(LSA_NULL != ptIfCtrl->p_ether_cfg->p_ether_phy_instance) )
	{
		__attribute__((unused))R_GMAC_Type * p_reg_access = (R_GMAC_Type *) ptIfCtrl->p_reg_access;
		__attribute__((unused))ether_switch_instance_t *ptSwi = (ether_switch_instance_t*)ptIfCtrl->p_ether_cfg->p_ether_switch_instance;
		ether_phy_instance_t *ptPhyInst = (ether_phy_instance_t*)ptIfCtrl->p_ether_cfg->p_ether_phy_instance;
		ether_phy_instance_t *ptPhy = &ptPhyInst[(PortID -1U)];
		ether_phy_instance_t *ptPhy0 = &ptPhyInst[0];
		ether_phy_instance_ctrl_t *ptPhyCtrl0 = (ether_phy_instance_ctrl_t *) ptPhy0->p_ctrl;
		ether_phy_instance_ctrl_t *ptPhyCtrl = (ether_phy_instance_ctrl_t *) ptPhy->p_ctrl;
		__attribute__((unused))ether_phy_instance_ctrl_t *p_instance_ctrl = (ether_phy_instance_ctrl_t *) ptPhy->p_ctrl;
		__attribute__((unused))LSA_UINT8 PHYIF_RSTn = (1U << (PortID-1));
		// R_ETHER_PHY_Open();

		if(LSA_NULL != ptPhyCtrl)
		{
			EDDP_LL_PORTCTRL_PTR_TYPE ptPortCtrl = &pHandle->tPortCtrl[(PortID -1U)];
			EDDP_LINK_STAT_PTR_TYPE  ptLinkStat  = &ptPortCtrl->tLinkStat;
			__attribute__((unused))LSA_UINT8            PhyPowerMode = 0;
			Status = EDD_STS_OK;

			switch (ptPortCtrl->LinkSpeedModeCfg)
			{
			case EDD_LINK_AUTONEG:
				((ether_phy_cfg_t*)ptPhy->p_cfg)->link_speed = ETHER_PHY_AUTONEG; 
				break; 
			case EDD_LINK_100MBIT_FULL: 
				((ether_phy_cfg_t*)ptPhy->p_cfg)->link_speed = ETHER_PHY_LINK_SPEED_100F; 
				break;
			default: 
				((ether_phy_cfg_t*)ptPhy->p_cfg)->link_speed = ETHER_PHY_LINK_SPEED_NO_LINK;

				//      uint32_t u32Value = ether_phy_read((ether_phy_instance_ctrl_t *) ptPhy, 0);   
				//      ether_phy_write((ether_phy_instance_ctrl_t *)ptPhy, 0, u32Value | 0x04800);     // we have no config, so lets put the PHY down so we are not running with a wrong setup. 
        
				break; 
			}
			switch (PhyCtrl)
			{
			case EDDP_PHY_POWERUP:

				if( /*( ETHER_PHY_OPEN != ptPhyCtrl->open) && */
					(EDD_STS_OK == EDDP_LL_LinkStatToHost(ptPortCtrl->LinkSpeedModeCfg, LSA_NULL, LSA_NULL, LSA_NULL, LSA_NULL )) )
				{ 
#if 0
          // AG: This part was only reuqired for the FPGA board at some time, as it duplicates the follwing block but this needs to be checked again
					if((ptPhyCtrl != ptPhyCtrl0) && (ETHER_PHY_OPEN != ptPhyCtrl0->open))
					{
						if(FSP_SUCCESS != ptPhy0->p_api->open((ether_phy_ctrl_t*)ptPhy0->p_ctrl, (ether_phy_cfg_t*)ptPhy0->p_cfg))
						{
							Status = EDD_STS_ERR_RESOURCE;
						}
					}
#endif
				//	p_reg_access->CONVRST_b.PHYIF_RSTn |= (PHYIF_RSTn);   /* Assert RESET */
				//	R_BSP_SoftwareDelay (200, BSP_DELAY_UNITS_MICROSECONDS);

					if( (EDD_STS_OK == Status) &&
						(FSP_SUCCESS != ptPhy->p_api->open((ether_phy_ctrl_t*)ptPhy->p_ctrl, (ether_phy_cfg_t*)ptPhy->p_cfg)) )
					{
						Status = EDD_STS_ERR_RESOURCE;
					}
				}

				if(EDD_STS_OK == Status)
				{
					ptPortCtrl->PhyPowerMode = EDDP_PHY_POWERUP;
					ptLinkStat->PhyStatus = EDD_PHY_STATUS_ENABLED;
					ptLinkStat->IsPulled = EDD_PORTMODULE_IS_PLUGGED;

					if(LSA_TRUE == bUpdateReg)
					{
						PNIP_HAL_PHY_SET_POWER(PortID, PhyCtrl);
					}
				}
				break;
			case EDDP_PHY_POWERDOWN:
				//ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_CONTROL, ETHER_PHY_CONTROL_RESET);
				//	ptPhy->p_api->close(ptPhy->p_ctrl);
				//	p_reg_access->CONVRST_b.PHYIF_RSTn &= (~PHYIF_RSTn);   /* Assert RESET */
				//R_BSP_SoftwareDelay (200, BSP_DELAY_UNITS_MICROSECONDS);
				eddp_ll_PhyRegWrite (0x01, PortID, EDDP_LL_PHY_REG_BCMR, (0x04800/*Power Down*/));
				ptPortCtrl->LinkSpeedModeCfg = EDD_LINK_UNCHANGED;
				ptPortCtrl->PhyPowerMode = EDDP_PHY_POWERDOWN;
				ptLinkStat->PhyStatus 	 = EDD_PHY_STATUS_DISABLED;
				ptLinkStat->Mode         = EDD_LINK_UNKNOWN;
				ptLinkStat->Speed     	 = EDD_LINK_UNKNOWN;
				ptLinkStat->Status    	 = EDD_LINK_DOWN;
				ptLinkStat->Autoneg      = EDD_AUTONEG_OFF;
				ptLinkStat->MAUType	 	 = EDD_MAUTYPE_UNKNOWN;          /* MAU Type */
				ptLinkStat->IsPulled	 = EDD_PORTMODULE_IS_PULLED;//EDD_PORTMODULE_IS_PLUGGED;    /* ChangeMediaTypeinRun value */
				ptLinkStat->PortState    = EDD_PORT_STATE_DISABLE; //EDD_PORT_STATE_FORWARDING;    /* State  of Port */
				//ptPhy->p_api->close((ether_phy_ctrl_t*)ptPhy->p_ctrl); 
				break;
			case EDDP_PHY_CLOSE:
				ptPhy->p_api->close(ptPhy->p_ctrl);
				break;
			default :
				Status = EDD_STS_ERR_PARAM;
				break;
			}
		}
	}
	return Status;
}

extern LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_GEN_ISR_LinkChange(
	    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	    LSA_UINT32                  EventNr);
EDD_RSP EDDP_LL_SET_LINK(LSA_UINT32 InterfaceID, LSA_UINT16 PortID, LSA_UINT8 LinkCfg)
{
	EDD_RSP Status = EDD_STS_ERR_PARAM;
	EDDP_LL_HANDLE_PTR_TYPE pHandle = (EDDP_LL_HANDLE_PTR_TYPE)EDDP_LL_MNG_HANDLE(1U);

	if( (LSA_NULL != pHandle) && (1U <= PortID)  && (PortID < RZT2_NUMBER_OF_PORTS ) )
	{
		EDDP_LL_PORTCTRL_PTR_TYPE ptPortCtrl = &pHandle->tPortCtrl[(PortID -1U)];
		EDDP_LINK_STAT_PTR_TYPE  ptLinkStat  = &ptPortCtrl->tLinkStat;
		ether_instance_ctrl_t * ptIfCtrl = EDDP_LL_ETH_CTRL_PTR(pHandle->pIfHandle);
		ether_phy_instance_t *ptPhyInst = (ether_phy_instance_t*)ptIfCtrl->p_ether_cfg->p_ether_phy_instance;
		__attribute__((unused))ether_phy_instance_t *ptPhy = &ptPhyInst[(PortID -1U)];

		if(EDD_LINK_AUTONEG == LinkCfg)
		{
			ptPortCtrl->LinkSpeedModeCfg = EDD_LINK_AUTONEG;
			Status = EDD_STS_OK;

			if( (ptPortCtrl->PhyPowerMode == EDDP_PHY_POWERUP) &&
				(EDD_LINK_UP == ptLinkStat->Status) )
			{
#if 1
#if 0
				ptPortCtrl->SetLinkPending = LSA_TRUE;
				PNIP_HAL_PHY_SET_LINK((PortID+1), EDD_LINK_UP, ptLinkStat->Speed, ptLinkStat->Mode);
				EDDP_GEN_ISR_LinkChange((EDDP_LOCAL_DDB_PTR_TYPE)pHandle->hDDB, 0);
				PNIP_HAL_PHY_SET_LINK((PortID+1), EDD_LINK_UP, ptLinkStat->Speed, ptLinkStat->Mode);
				EDDP_GEN_ISR_LinkChange((EDDP_LOCAL_DDB_PTR_TYPE)pHandle->hDDB, 0);
				//ptPortCtrl->SetLinkPending = LSA_TRUE;
				//OsTaskNotify(pHandle->IfCtrlTaskId, EDDP_LL_EV_LINK);
#endif
#else
				LSA_UINT16 PortID =(1UL << (PortID-1U));
				if( 0U == ((~PortID) & pHandle->dwLinkStatusAll) )
				{
					ether_disable(ptIfCtrl);
					pHandle->dwLinkStatusAll = 0U;
				}
				else
				{
					pHandle->dwLinkStatusAll &= (~PortID);
				}

				if(EDD_AUTONEG_OFF == ptLinkStat->Autoneg)
				{
					if(FSP_SUCCESS == ptPhy->p_api->startAutoNegotiate(ptPhy->p_ctrl))
					{
						ptLinkStat->Autoneg = EDD_AUTONEG_ON;
					}
					else
					{
						ptLinkStat->Mode     = EDD_LINK_UNKNOWN;
						ptLinkStat->Speed    = EDD_LINK_UNKNOWN;
						ptLinkStat->Status   = EDD_LINK_DOWN;
						ptLinkStat->Autoneg  = EDD_AUTONEG_OFF;
						ptLinkStat->MAUType	 = EDD_MAUTYPE_UNKNOWN;
						ptLinkStat->Status   = EDD_LINK_DOWN;
					}
				}
#endif
			}
		}
		else
		{
			Status = EDDP_LL_LinkStatToHost(LinkCfg, LSA_NULL, LSA_NULL, LSA_NULL, LSA_NULL);
			if(EDD_STS_OK == Status)
			{
				ptPortCtrl->LinkSpeedModeCfg = LinkCfg;
			}
		}




	}

	return Status;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_LL_PHY_SetPowerDown                   +*/
/*+  Input/Output          :    EDDP_HANDLE             hDDB                +*/
/*+                        :    EDDP_SYS_HANDLE         hSysDev             +*/
/*+                        :    LSA_UINT32              HwPortID            +*/
/*+                        :    LSA_BOOL                PowerDown           +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+   hDDB       : EDDP Handle                                         (in) +*/
/*+   hSysDev    : Handle of system adaption                           (in) +*/
/*+   HwPortID   : Hardware port ID of the related PHY                 (in) +*/
/*+   PowerDown  : LSA_TRUE turns the PHY off / LSA_FALSE := on        (in) +*/
/*+                                                                         +*/
/*+  Results     : EDD_STS_OK                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function turns the PHY into software power down mode +*/
/*+               ON or OFF. When software power mode is OFF there will be  +*/
/*+               no link and the PHY does not receive or send any data but +*/
/*+               can still be controlled via SMI.                          +*/
/*+                                                                         +*/
/*+       Note:   EDDP expects this function to return with success. Any    +*/
/*+               other return value than EDD_STS_OK  will be interpreted  +*/
/*+               as a fatal error.                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_LL_PHY_SetPowerDown(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID,
    LSA_UINT8               PowerDown)
{
    __attribute__((unused))LSA_UINT32      RegVal;
    EDD_RSP Status = EDD_STS_ERR_PARAM;
    EDDP_LOCAL_DDB_PTR_TYPE pDDB = (EDDP_LOCAL_DDB_PTR_TYPE)hDDB;
  //  EDDP_LL_HANDLE_PTR_TYPE pHandle = EDDP_LL_MNG_HANDLE(pDDB->InterfaceID);

	if( (LSA_NULL != pDDB) && (0U < HwPortID) )
	{
		Status = EDDP_LL_PHY_CTRL(EDDP_LL_MNG_HANDLE(pDDB->InterfaceID), HwPortID, PowerDown, LSA_TRUE);
	}


    return Status;
}


EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR RZT2_PHY_CheckPowerDown(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID,
    LSA_UINT8*              pIsPowerDown)
{
    LSA_UINT32      RegVal;
	  EDDP_LOCAL_DDB_PTR_TYPE     pDDB;

    LSA_UNUSED_ARG(hSysDev);
    
    EDDP_CoreDevGetDDB (hDDB, &pDDB);
    RegVal = eddp_ll_PhyRegRead(pDDB->InterfaceID, HwPortID, ETHER_PHY_REG_CONTROL);
    //eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_PHY_NEC_REG_BCMR, &RegVal);

    if(RegVal & 0x0800/*power down*/)
    {
        *pIsPowerDown = EDDP_PHY_POWERDOWN;
    }
    else
    {
        *pIsPowerDown = EDDP_PHY_POWERUP;
    }

    return EDD_STS_OK;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_LL_PHY_CheckPowerDown                 +*/
/*+  Input/Output          :    EDDP_HANDLE             hDDB                +*/
/*+                        :    EDDP_SYS_HANDLE         hSysDev             +*/
/*+                        :    LSA_UINT32              HwPortID            +*/
/*+                        :    LSA_BOOL                pIsPowerDown        +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+   hDDB       : EDDP Handle                                         (in) +*/
/*+   hSysDev    : Handle of system adaption                           (in) +*/
/*+   HwPortID   : Hardware port ID of the related PHY                 (in) +*/
/*+   pIsPowerDown: PowerDown state of the related PHY (LSA_FALSE if   (out)+*/
/*+                 not PowerUp)                                            +*/
/*+                                                                         +*/
/*+  Results     : EDD_STS_OK if PHY supports this check                    +*/
/*+                EDD_STS_ERR_NOT_SUPPORTED if PHY does not support        +*/
/*+                this check                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function checks if the PHY is in powerdown state.    +*/
/*+               Needed for service EDD_SRV_PRM_CHANGE_PORT.               +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_LL_PHY_CheckPowerDown(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID,
    LSA_UINT8*              pIsPowerDown)
{
	EDD_RSP Status = EDD_STS_ERR_PARAM;

    EDDP_LOCAL_DDB_PTR_TYPE pDDB = (EDDP_LOCAL_DDB_PTR_TYPE)hDDB;
    EDDP_LL_HANDLE_PTR_TYPE pHandle = EDDP_LL_MNG_HANDLE(pDDB->InterfaceID);

 	if( (LSA_NULL != pDDB) && (LSA_NULL != pHandle) && (LSA_NULL != pIsPowerDown) &&
 		(1U <= HwPortID)  && (HwPortID <= EDD_CFG_MAX_PORT_CNT ) )
 	{
 		EDDP_LL_PORTCTRL_PTR_TYPE ptPortCtrl = &pHandle->tPortCtrl[(HwPortID -1U)];
 		__attribute__((unused))EDDP_LINK_STAT_PTR_TYPE  ptLinkStat  = &ptPortCtrl->tLinkStat;
 		*pIsPowerDown = ptPortCtrl->PhyPowerMode;
 		Status = EDD_STS_OK;
 	}

    return Status;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_LL_PHY_GetLinkStatus                  +*/
/*+  Input/Output          :    EDDP_HANDLE			            hDDB        +*/
/*+                        :    EDDP_SYS_HANDLE		            hSysDev     +*/
/*+                        :    LSA_UINT32						Hardware    +*/
/*+                        :    LSA_UINT32			            HwPortID    +*/
/*+                        :    EDDP_PHY_LINK_STATUS_PTR_TYPE   pLinkStatus +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+   hDDB                  : EDDP Handle                              (in) +*/
/*+   hSysDev               : Handle of system adaption                (in) +*/
/*+   Hardware              : internal HW identifier                   (in) +*/
/*+   HwPortID              : Hardware port ID of the related PHY      (in) +*/
/*+                                                                         +*/
/*+   Elements of pLinkStatus:                                              +*/
/*+    Link                 : IN: PHY register containing link information  +*/
/*+                           OUT: Current link status                      +*/
/*+    Speed                : IN: PHY register containing speed information +*/
/*+                           OUT: Current speed status                     +*/
/*+    Duplexity            : IN: PHY reg containing duplexity information  +*/
/*+                           OUT: Current duplexity status                 +*/
/*+    MAUType              : Current MAU Type                         (out)+*/
/*+    MediaType            : Current Media Type                       (out)+*/
/*+    AutonegCapAdvertised : Current advertised capabilities @auto-neg(out)+*/
/*+    PHY_TxDelay          : Transmit PHY delay in nano seconds (ns)  (out)+*/
/*+    PHY_RxDelay          : Receive PHY delay in nano seconds (ns)   (out)+*/
/*+    IsWireless           : Is this port a wireless port?            (out)+*/
/*+    IsMDIX               : Is this a "Crossed Port"?                (out)+*/
/*+                                                                         +*/
/*+  Results                : EDD_STS_OK                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function determines the current link status, speed   +*/
/*+               and duplexity from the register values passed when the    +*/
/*+               function is called. It then stores this information in    +*/
/*+               Link, Speed and Duplexity. Additionally, the current      +*/
/*+               MAUType, MediaType, AutonegCapAdvertised, TxDelay, RxDelay+*/
/*+               IsWireless and IsMDIX are returned.                       +*/
/*+               If the transceiver does not support auto-negotiation it   +*/
/*+               has to return ・ｽ0・ｽ for ・ｽAutonegCapAdvertised・ｽ.             +*/
/*+                                                                         +*/
/*+       Note:   EDDP expects this function to return with success. Any    +*/
/*+               other return value than ・ｽEDD_STS_OK・ｽ will be interpreted  +*/
/*+               as a fatal error.                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_LL_PHY_GetLinkStatus(
	EDDP_HANDLE				        hDDB,
	EDDP_SYS_HANDLE			        hSysDev,
	LSA_UINT32						Hardware,
    LSA_UINT32				        HwPortID,
    EDDP_PHY_LINK_STATUS_PTR_TYPE   pLinkStatus)
{
	EDD_RSP Status = EDD_STS_ERR_PARAM;
    LSA_UINT32		RegVal = 0;
	LSA_UINT32      DevId = 0;
    EDD_RSP         Response;

    EDDP_LOCAL_DDB_PTR_TYPE pDDB = (EDDP_LOCAL_DDB_PTR_TYPE)hDDB;
	EDDP_LL_HANDLE_PTR_TYPE pHandle = EDDP_LL_MNG_HANDLE(pDDB->InterfaceID);

	DevId = pDDB->DevId;
	//EDDP_ASSERT_FALSE(DevId >= EDDP_CFG_MAX_DEVICES);

	if( (LSA_NULL != hDDB) && (LSA_NULL != pLinkStatus) && (LSA_NULL != pHandle) &&
		(0U < HwPortID) && (HwPortID <= EDDP_MAX_PORT_CNT) )
	{
		EDDP_LL_PORTCTRL_PTR_TYPE ptPortCtrl = &pHandle->tPortCtrl[(HwPortID -1U)];
		EDDP_LINK_STAT_PTR_TYPE  ptLinkStat  = &ptPortCtrl->tLinkStat;

		if(LSA_TRUE == ptPortCtrl->SetLinkPending)
		{
			pLinkStatus->Speed      = EDD_LINK_UNKNOWN;
			pLinkStatus->Duplexity  = EDD_LINK_UNKNOWN;
			pLinkStatus->MAUType    = EDD_MAUTYPE_UNKNOWN;
			pLinkStatus->Link = EDD_LINK_DOWN;
			ptPortCtrl->SetLinkPending = LSA_FALSE;
		}
		else
		{
			pLinkStatus->Link = ptLinkStat->Status;
			pLinkStatus->Speed = ptLinkStat->Speed;
			pLinkStatus->Duplexity = ptLinkStat->Mode;
		}

		if(EDD_LINK_UNCHANGED > ptPortCtrl->LinkSpeedModeCfg)
		{
			g_EDDP_PHY_LINK_SPEED_MODE_CONFIGURED[DevId][HwPortID-1] = ptPortCtrl->LinkSpeedModeCfg;
		}
		else
		{
			g_EDDP_PHY_LINK_SPEED_MODE_CONFIGURED[DevId][HwPortID-1] = EDD_LINK_AUTONEG;
		}

		g_EDDP_PHY_MEDIATYPE        [DevId][HwPortID-1] = EDD_MEDIATYPE_COPPER_CABLE;

		// ***** information if transceiver is TX or FX
		pLinkStatus->MediaType          = g_EDDP_PHY_MEDIATYPE        [DevId][HwPortID-1];
		pLinkStatus->OpticalTransType   = g_EDDP_PHY_OPTICALTRANSTYPE [DevId][HwPortID-1];
		pLinkStatus->FxTransceiverType  = g_EDDP_PHY_FXTRANSCEIVERTYPE[DevId][HwPortID-1];

		EDDP_SWI_TRACE_04(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE,"[H:--] EDDP_LL_PHY_GetLinkStatus(HwPortID=%d): RegLink=%Xh, RegSpeed=%Xh, RegDuplexity=%X",
			HwPortID, pLinkStatus->Link, pLinkStatus->Speed, pLinkStatus->Duplexity);

		// ***** Examine passed parameters in order to determine link, speed and duplexity.
		pLinkStatus->AutonegCapAdvertised   = EDD_AUTONEG_CAP_100BASETXFD;
		//EDDP-LL: pLinkStatus->Link                   = (pLinkStatus->Link & EDDP_LL_PHY_LINKSTATUSBITMASK) ? EDD_LINK_UP : EDD_LINK_DOWN;

		//Attention: TFS2141324: Due to polling times, LinkStatus can still be UP (previous pollcycle) while real LinkStatus is DOWN already and speed/duplexity undefined.
		//           ==> 10MBit or HD have to be allowed!
		if ( (0U < pHandle->dwLinkStatusAll) && (pLinkStatus->Link == EDD_LINK_UP) && (LSA_FALSE == ptPortCtrl->SetLinkPending))
		{
			//EDDP-LL: pLinkStatus->Speed     = ( pLinkStatus->Speed & EDDP_LL_PHY_SPEEDBITMASK )         ? EDD_LINK_SPEED_10  : EDD_LINK_SPEED_100;
			//EDDP-LL: pLinkStatus->Duplexity = ( pLinkStatus->Duplexity & EDDP_LL_PHY_DUPLEXITYBITMASK ) ? EDD_LINK_MODE_FULL : EDD_LINK_MODE_HALF;
			// ***** Determine current MAUType
			EDDP_LL_PHY_Get_MAU_Type(hDDB, hSysDev, HwPortID, pLinkStatus->Speed, pLinkStatus->Duplexity, &pLinkStatus->MAUType);
			if(EDD_MAUTYPE_UNKNOWN == pLinkStatus->MAUType)
			{
				pLinkStatus->Speed      = EDD_LINK_UNKNOWN;
				pLinkStatus->Duplexity  = EDD_LINK_UNKNOWN;
				pLinkStatus->MAUType    = EDD_MAUTYPE_UNKNOWN;
				pLinkStatus->Link = EDD_LINK_DOWN;
			}
		}
		else
		{
			pLinkStatus->Speed      = EDD_LINK_UNKNOWN;
			pLinkStatus->Duplexity  = EDD_LINK_UNKNOWN;
			pLinkStatus->MAUType    = EDD_MAUTYPE_UNKNOWN;
		}

		if (pLinkStatus->MediaType == EDD_MEDIATYPE_FIBER_OPTIC_CABLE)
		{
		   // ***** FIBER_OPTIC. fixed.
		   pLinkStatus->AutonegCapAdvertised = EDD_AUTONEG_CAP_NONE;
		}
		else
		{

			// Advertisement
		  // RZT2:  eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_LL_PHY_REG_NEG_ADVERTISEMENT, &RegVal);

			RegVal = eddp_ll_PhyRegRead(pDDB->InterfaceID, HwPortID, ETHER_PHY_REG_AN_ADVERTISEMENT);

			if (RegVal & ETHER_PHY_AN_ADVERTISEMENT_10H)     { pLinkStatus->AutonegCapAdvertised |= EDD_AUTONEG_CAP_10BASET    ; }   // 10Mbps able
			if (RegVal & ETHER_PHY_AN_ADVERTISEMENT_10F)     { pLinkStatus->AutonegCapAdvertised |= EDD_AUTONEG_CAP_10BASETFD  ; }   // 10Mbps with full duplex
			if (RegVal & ETHER_PHY_AN_ADVERTISEMENT_100H)    { pLinkStatus->AutonegCapAdvertised |= EDD_AUTONEG_CAP_100BASETX  ; }   // 100BASE-TX able
			if (RegVal & ETHER_PHY_AN_ADVERTISEMENT_100F)    { pLinkStatus->AutonegCapAdvertised |= EDD_AUTONEG_CAP_100BASETXFD; }   // 100BASE-TX able with full duplex
			if (RegVal & ETHER_PHY_AN_ADVERTISEMENT_100_T4)  { pLinkStatus->AutonegCapAdvertised |= EDD_AUTONEG_CAP_100BASET4  ; }   // 100BASE-T4 able

			if (RegVal & ETHER_PHY_AN_ADVERTISEMENT_PAUSE)   { pLinkStatus->AutonegCapAdvertised |= EDD_AUTONEG_CAP_FDXPAUSE  ; }   // Pause Operation
		}

		// ***** set Port delay values
		Response = EDDP_LL_PHY_LoadDelayValues(hDDB, Hardware, pLinkStatus);
		if (EDD_STS_OK != Response)
		{
			//Traceentries are made in EDDP_LL_PHY_LoadDelayValues
			return (Response);
		}

		// ***** set IsWireles
		pLinkStatus->IsWireless = g_EDDP_PHY_ISWIRELESS[DevId][HwPortID-1];
		// ***** set IsMDIX
		pLinkStatus->IsMDIX     = g_EDDP_PHY_MDI_X_MAPPING[DevId][HwPortID-1];

		//pLinkStatus->LinkSpeedModeConfigured     = g_EDDP_PHY_LINK_SPEED_MODE_CONFIGURED[DevId][HwPortID-1];
		Status = EDD_STS_OK;
	}


    return Status;
}

LSA_VOID EDDP_LL_GetPortStatistics(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_LOCAL_GET_STATISTICS_PTR_TYPE      pStats,
    LSA_UINT32                              HwPortID)
{
    EDDP_LL_HANDLE_PTR_TYPE pHandle = (EDDP_LL_HANDLE_PTR_TYPE)EDDP_LL_MNG_HANDLE(1);
		R_ETHSW_Type  * p_Reg_Ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;

    if( (NULL != pHandle) && (EDDP_LLIF_STATE_OPERATIONAL == pHandle->u16State) &&
    	  (EDD_CFG_MAX_PORT_CNT >= HwPortID) && (LSA_NULL != pStats)
	    )
    {
    	EDDP_LOCAL_GET_STATISTICS_PTR_TYPE ptPortStat;
		  OsMemSet(pStats, 0, sizeof(EDDP_GET_STATISTICS_TYPE));

		if(0U < HwPortID)
		{
			pHandle->tPortCtrl[(HwPortID-1)].tStatistic.InOctets  = p_Reg_Ethsw->AOCTETSRECEIVEDOK_P0;
			pHandle->tPortCtrl[(HwPortID-1)].tStatistic.OutOctets = p_Reg_Ethsw->AOCTETSTRANSMITTEDOK_P0;
			pHandle->tPortCtrl[(HwPortID-1)].tStatistic.InBroadcastPkts = p_Reg_Ethsw->IFINBROADCASTPKTS_P0;
			pHandle->tPortCtrl[(HwPortID-1)].tStatistic.OutBroadcastPkts = p_Reg_Ethsw->IFOUTBROADCASTPKTS_P1;
			pHandle->tPortCtrl[(HwPortID-1)].tStatistic.InDiscards = p_Reg_Ethsw->IFOUTDISCARDS_P0;
			pHandle->tPortCtrl[(HwPortID-1)].tStatistic.OutDiscards = p_Reg_Ethsw->IFOUTDISCARDS_P1;
			pHandle->tPortCtrl[(HwPortID-1)].tStatistic.InErrors = p_Reg_Ethsw->IFINERRORS_P0;
			pHandle->tPortCtrl[(HwPortID-1)].tStatistic.OutErrors = p_Reg_Ethsw->IFOUTERRORS_P1;
			pHandle->tPortCtrl[(HwPortID-1)].tStatistic.InUcastPkts = p_Reg_Ethsw->IFINUCASTPKTS_P1;
			pHandle->tPortCtrl[(HwPortID-1)].tStatistic.OutUcastPkts = p_Reg_Ethsw->IFOUTUCASTPKTS_P1;
			pHandle->tPortCtrl[(HwPortID-1)].tStatistic.InMulticastPkts = p_Reg_Ethsw->IFINMULTICASTPKTS_P1;
			pHandle->tPortCtrl[(HwPortID-1)].tStatistic.OutMulticastPkts = p_Reg_Ethsw->IFOUTMULTICASTPKTS_P1;


			if(EDD_LINK_UP == pHandle->tPortCtrl[(HwPortID-1)].tLinkStat.Status)
			{
				pHandle->tPortCtrl[(HwPortID-1)].tStatistic.SupportedCounters  = RZT2_MIB2_SUPPORTED_COUNTERS_IF;
				ptPortStat = &pHandle->tPortCtrl[(HwPortID-1)].tStatistic;
				OsMemCpy(pStats, ptPortStat, sizeof(EDDP_GET_STATISTICS_TYPE));
			}
		}
		else
		{
			int nCnt = 0;

			pStats->SupportedCounters  = RZT2_MIB2_SUPPORTED_COUNTERS_IF;

			while(nCnt < EDD_CFG_MAX_PORT_CNT)
			{
				ptPortStat = &pHandle->tPortCtrl[nCnt].tStatistic;

				if(EDD_LINK_UP == pHandle->tPortCtrl[nCnt].tLinkStat.Status)
				{
					pStats->RequestedCounters 	+= ptPortStat->InBroadcastPkts;

					/* RFC2863_COUNTER - End */
					pStats->SupportedCounters 	+= ptPortStat->InBroadcastPkts;
					pStats->InOctets						+= ptPortStat->InOctets;
					pStats->InUcastPkts					+= ptPortStat->InUcastPkts;
					pStats->InNUcastPkts				+= ptPortStat->InNUcastPkts;
					pStats->InDiscards					+= ptPortStat->InDiscards;
					pStats->InErrors						+= ptPortStat->InErrors;
					pStats->InUnknownProtos			+= ptPortStat->InUnknownProtos;
					pStats->OutOctets						+= ptPortStat->OutOctets;
					pStats->OutUcastPkts				+= ptPortStat->OutUcastPkts;
					pStats->OutNUcastPkts				+= ptPortStat->OutNUcastPkts;
					pStats->OutDiscards					+= ptPortStat->OutDiscards;
					pStats->OutErrors						+= ptPortStat->OutErrors;
					pStats->OutQLen							+= ptPortStat->OutQLen;

					/* RFC2863_COUNTER */
					pStats->InMulticastPkts		+= ptPortStat->InMulticastPkts;
					pStats->InBroadcastPkts		+= ptPortStat->InBroadcastPkts;
					pStats->OutMulticastPkts	+= ptPortStat->OutMulticastPkts;
					pStats->OutBroadcastPkts	+= ptPortStat->OutBroadcastPkts;
					pStats->InHCOctets				+= ptPortStat->InHCOctets;
					pStats->InHCUcastPkts			+= ptPortStat->InHCUcastPkts;
					pStats->InHCMulticastPkts	+= ptPortStat->InHCMulticastPkts;
					pStats->InHCBroadcastPkts	+= ptPortStat->InHCBroadcastPkts;
					pStats->OutHCOctets				+= ptPortStat->OutHCOctets;
					pStats->OutHCUcastPkts		+= ptPortStat->OutHCUcastPkts;
					pStats->OutHCMulticastPkts	+= ptPortStat->OutHCMulticastPkts;
					pStats->OutHCBroadcastPkts	+= ptPortStat->OutHCBroadcastPkts;
				}
				nCnt++;
			}
		}
  }
}


EDD_RSP EDDP_LL_ExeCmd(EDDP_LL_HANDLE_PTR_TYPE pHandle, EDDP_LOCAL_DDB_PTR_TYPE pDDB, EDDP_CMD_PARAM_PTR_TYPE pParam)
{
	EDD_RSP   Response = EDD_STS_OK;
 //   LSA_UINT32                  param_address_pnip;
 //   LSA_UINT32                  target_param_address;
   // LSA_UINT32                  target_param_address_offset;
    LSA_UINT32                  register_value;
    LSA_UINT32                  user_id;
    LSA_UINT32                  i;
    LSA_BOOL                    isAsync;

    // EDD_COMMON_MEM_PTR_TYPE     cbl_address;
    // EDD_COMMON_MEM_PTR_TYPE     param_address;
    // EDDP_CMD_COMMAND_TYPE*      pCommandBlock;
    // EDDP_CMD_CBL_PARAM_TYPE*    pCommandParamBlock;

    if( (LSA_NULL != pDDB) && (LSA_NULL != pParam) && (LSA_NULL != pHandle) )
    {
    	// EDDP_ISR_GROUP_INFO_PTR_TYPE    pGroup = &pDDB->Core.Isr.Group;

		register_value = 0;
		isAsync = LSA_TRUE;

		if (pParam->Cbf == 0) // get addresses for synchronous command processing
		{
			//user_id = EDDP_CMD_USER_ID_HOST_SYNC;     // indicates synchronous command processing on Host/ARM
			isAsync = LSA_FALSE;
		}
		else // asynchronous command processing
		{
			//user_id = EDDP_CMD_USER_ID_HOST_ASYNC;  // indicates asynchronous command processing on Host/ARM
		}

		switch (pParam->CmdOpcode)
		{
        case EDDP_CMD_FILTER_STOP:
        	if(0U < pParam->param_union.Filter.StopFilter.StopFilterMask)
        	{

        	}
        	break;

        case EDDP_CMD_CPM_NOP: 	break;

        case EDDP_CMD_CPM_ON_OFF:

        	OsIntDisable();
        	g_CpmOn = pParam->param_union.Crt.Cpm.OnOff.CpmOn;
        	OsIntEnable();

        	if(pParam->param_union.Crt.Cpm.OnOff.CpmOn)
        	{// EDDP_CMD_CPM_CPM_ACTIVATE

        	}
        	else
        	{// EDDP_CMD_CPM_CPM_DEACTIVATE

        	}
        	break;

        case EDDP_CMD_CPM_ACW_ON_OFF:
            // ***** ON:  set ActiveBit, OneShotBit in ACW, set Valid, WDT, MRT, DHT, Again, Frame_received in TSB
            // ***** OFF: clr ActiveBit in ACW, clr DHT, Valid in TSB

        	if(pParam->param_union.Crt.Cpm.ACW_OnOff.Acw_On)
        	{// EDDP_CMD_CPM_ACW_ACTIVATE
				EDDP_CRT_CONSUMER_MNGM_PTR_TYPE pConsMngm = &pDDB->CRT.Cons;
				EDDP_CRT_CONSUMER_PTR_TYPE pConsumer = &pConsMngm->pConsEntry[pParam->param_union.Crt.Cpm.ACW_OnOff.AcwID];
				if(pParam->param_union.Crt.Cpm.ACW_OnOff.AcwID < pDDB->CRT.CfgParam.ConsumerMaxNumber)
				{
					RTCX_PKT_PTR pRx = EDDP_LL_GetRtcRcvPktfromConsumerId(pConsumer->ConsumerId);
					RTCX_HDR_PTR_NV pHdr = (RTCX_HDR_PTR_NV)&pRx->Buffer[0U];
					OsMemCpy(&pHdr->src[0U], &pConsumer->Param.SrcMAC.MacAdr[0U], 6);
				}
        	}
        	else
        	{// EDDP_CMD_CPM_ACW_DEACTIVATE

			}
        	break;

        case EDDP_CMD_CPM_ACW_IN_OUT:
            // ***** IN:  set link in prev. ACW (update ACW_netxoffset to new inserted ACW)
            // ***** OUT: update link in prev. ACW to position of ACW after the ACW that will be cut out
        	//if (pParam->param_union.Crt.Cpm.ACW_InOut.AcwID <= pDDB->CMD.MaxAcwCountPerCmd)
			{
        		EDDP_CRT_CONSUMER_MNGM_PTR_TYPE pConsMngm  = &pDDB->CRT.Cons;
				EDDP_CRT_CONSUMER_PTR_TYPE pConsumer   	   = &pConsMngm->pConsEntry[pParam->param_union.Crt.Cpm.ACW_InOut.AcwID];
				if(pParam->param_union.Crt.Cpm.ACW_InOut.AcwID < pDDB->CRT.CfgParam.ConsumerMaxNumber)
				{
					RTCX_PKT_PTR pRx = EDDP_LL_GetRtcRcvPktfromConsumerId(pConsumer->ConsumerId);
					RTCX_HDR_PTR_NV pHdr = (RTCX_HDR_PTR_NV)&pRx->Buffer[0U];

					if(LSA_TRUE == pParam->param_union.Crt.Cpm.ACW_InOut.Word0.Acw_In)
					{
#ifdef PrintfDebug
						printeth("CPM_ACWIN");
#endif
						// RTC3 (Advanced) mode enables DHTExpire check when RTC frame is received
						if((pConsumer->Param.Properties & EDD_CSRT_CONS_PROP_RTCLASS_MASK) == EDD_CSRT_CONS_PROP_RTCLASS_1
						|| (pConsumer->Param.Properties & EDD_CSRT_CONS_PROP_RTCLASS_MASK) == EDD_CSRT_CONS_PROP_RTCLASS_2
						|| (pConsumer->Param.Properties & EDD_CSRT_CONS_PROP_RTC3_MODE_LEGACY) == EDD_CSRT_CONS_PROP_RTC3_MODE_LEGACY)
						{
							ether_instance_t *ptEth = EDDP_LL_CAST_ETH(pHandle->pIfHandle);
							ether_switch_instance_ctrl_t *ptSwCtrl = (ether_switch_instance_ctrl_t*)ptEth->p_cfg->p_ether_switch_instance->p_ctrl;
							R_ETHSW_Type *ptEthSw = (R_ETHSW_Type*)(R_ETHSW_Type*) ptSwCtrl->p_reg_switch;
							LSA_UINT8 useTimer = ptEthSw->COMMAND_CONFIG_P3_b.TIMER_SEL;
							OsIntDisable();
							pConsumer->DHTExpireCheckEnable = LSA_TRUE;
							pConsumer->FirstPacket = LSA_FALSE;
							pConsumer->DHTExpireCount = 0;
							OsIntEnable();
						}
						OsMemSet(pRx, 0, sizeof(RTCX_PKT));
						OsMemCpy(&pHdr->src[0U], &pConsumer->Param.SrcMAC.MacAdr[0U], 6);
						OsMemCpy(&pHdr->dst[0U], &pHandle->MacAddrIF[0], 6);
//						pHdr->vlan_tag = __REV16(0x8100U); // VLAN-Tag
//						pHdr->vlan_id  = __REV16(0xC000U); // VLAN-ID = 0, Priority = 6,
						pHdr->eth_type = __REV16(0x8892U); // Ether-Type: PROFINET
						pHdr->frame_id = __REV16(pConsumer->Param.FrameID);
						pRx->BuffSize = (sizeof(RTCX_PKT) + pConsumer->Param.DataLen + 2U +1U +1U);
						pRx->pResource = pConsumer;

						if ((FRAMEID_RTC3_START <= pConsumer->Param.FrameID) &&
							(FRAMEID_RTC3_END   >= pConsumer->Param.FrameID))
						{
							if (pConsumer->Param.Properties & EDD_CSRT_CONS_PROP_RTC3_MODE_LEGACY)
							{
								setup_pattern_matcher_IRT_Legacy(pConsumer->Param.FrameID); // IRT Legacy
							}
							else
							{
								setup_pattern_matcher_IRT_Advanced(pConsumer->Param.FrameID); // IRT Advanced
							}

							ethsw_queue_flush(QFLUSH_PORT_MASK_3, QFLUSH_QUEUE_MASK_3, QFLUSH_ACTION_CLOSE1);
						}
						else
						{	// RT Advanced and RT Legacy
							uint32_t ArIdx = EDDP_LL_GetArIdxfromConsumerId(pConsumer->ConsumerId);
							setup_pattern_matcher_RT(pConsumer->Param.FrameID, ArIdx);
						}
					}
					else
					{
#ifdef PrintfDebug
						printeth("CPM_ACWOUT");
#endif
						EDDP_LL_ReleaseRtcRcvPkt(pConsumer->ConsumerId);
						pConsumer->DHTExpireCheckEnable = LSA_FALSE;
						pConsumer->FirstPacket = LSA_FALSE;
						pConsumer->DHTExpireCount = 0;
					//	OsMemSet(pRx, 0, sizeof(RTCX_PKT));
					//	Response = EDDP_LL_RemoveConsumer(pHandle, pConsumer, pAcw);

					}
				}
			}

        	// configure command for APICtrl-Ram
        	if(pParam->param_union.Crt.Cpm.ACW_InOut.Word0.Acw_In)
        	{// EDDP_CMD_CPM_PARAM_ACW_IN

        	}
        	else
        	{ // EDDP_CMD_CPM_PARAM_ACW_OUT

        	}

        	if(pParam->param_union.Crt.Cpm.ACW_InOut.Word0.Acw_DG)
        	{// EDDP_CMD_CPM_PARAM_IS_ACW_DG

        	}
        	else
        	{// EDDP_CMD_CPM_PARAM_IS_ACW

        	}
        	// pParam->param_union.Crt.Cpm.ACW_InOut.Word0.ApiAdr_NextPointer_Prev
        	// register_value = pParam->param_union.Crt.Cpm.ACW_InOut.Word1.ApiAdr_Acw_New;

        	break;

        case EDDP_CMD_CPM_ACW_SET_UNKNOWN:
            // ***** WDT-, MRT- und DHT-Reload, clr AGAIN + Frame_received
        	if(pParam->param_union.Crt.Cpm.SetUnknown.Acw_DG)
        	{// EDDP_CMD_CPM_IS_ACW_DG

        	}
        	else
        	{// EDDP_CMD_CPM_IS_ACW

        	}
        	// pParam->param_union.Crt.Cpm.SetUnknown.ApiAdr_Acw
        	break;

        case EDDP_CMD_CPM_ACW_XCHANGE_BUFF:

            // ***** exchange buffers and check that old buffer is free
            // register_value = 0x40000000; /* NOP */
            break;

        /* PPM ------------------------------------------------ */
        case EDDP_CMD_PPM_NOP:
            // register_value = 0x40000000; /* NOP */
            break;

        case EDDP_CMD_PPM_ON_OFF:
        //	OsIntDisable();
			g_PpmOn = pParam->param_union.Crt.Ppm.OnOff.PpmOn;
		//	OsIntEnable();

        	if(pParam->param_union.Crt.Ppm.OnOff.PpmOn)
        	{// EDDP_CMD_PPM_PPM_ACTIVATE

        	}
        	else
        	{// EDDP_CMD_PPM_PPM_DEACTIVATE

        	}
        	break;

        case EDDP_CMD_PPM_ACW_ON_OFF:
			// ***** Note: it seems that all arrays can be used with Acw_Count for range: 0..3
			if (pParam->param_union.Crt.Ppm.ACW_OnOff.Acw_Count <= pDDB->CMD.MaxAcwCountPerCmd)
			{
				for ( i=0; i < pParam->param_union.Crt.Ppm.ACW_OnOff.Acw_Count; i++ )
				{
					if(pParam->param_union.Crt.Ppm.ACW_OnOff.Acw_On)
					{ // EDDP_CMD_PPM_ACW_ACTIVATE

					}
					else
					{// EDDP_CMD_PPM_ACW_DEACTIVATE

					}

					if(pParam->param_union.Crt.Ppm.ACW_OnOff.Acw_DG)
					{// EDDP_CMD_PPM_IS_ACW_DG

					}
					else
					{// EDDP_CMD_PPM_IS_ACW

					}

					if(pParam->param_union.Crt.Ppm.ACW_OnOff.FirstSend)
					{// EDDP_CMD_PPM_FIRST_SEND_ACTIVATE

					}
					else
					{// EDDP_CMD_PPM_FIRST_SEND_DEACTIVATE

					}

					// pParam->param_union.Crt.Ppm.ACW_OnOff.ApiAdr_Acw[i]
					// EDDP_SET_CBL_HIGH_32_WORD_WCRC(register_value, PNIP_R2_REG_PPMCOMMAND_IF_CONTROL, ((i+1 == pParam->param_union.Crt.Ppm.ACW_OnOff.Acw_Count) ? LSA_TRUE : LSA_FALSE));

				}
			}
			else
			{
				EDDP_CMD_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] _EDDP_CMD_FillCommandBlockAndParam(): FATAL -> Ppm.ACW_OnOff.Acw_Count(%d) > AcwMaxNr(%d) per command!",
									  pParam->param_union.Crt.Ppm.ACW_OnOff.Acw_Count, pDDB->CMD.MaxAcwCountPerCmd);
				//	EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
			}
			break;

        case EDDP_CMD_PPM_ACW_IN_OUT:
            // ***** Note: it seems that all arrays can be used with Acw_Count for range: 0..EDDP_CMD_MAX_ACW_CNT_PER_CMD
            if (  (pParam->param_union.Crt.Ppm.ACW_InOut.Acw_Count <= pDDB->CMD.MaxAcwCountPerCmd) )
            {
            	EDDP_CRT_PROVIDER_MNGM_PTR_TYPE pProvMngm  = &pDDB->CRT.Prov;
				EDDP_CRT_PROVIDER_PTR_TYPE pProvider   	   = &pProvMngm->pProvEntry[pParam->param_union.Crt.Ppm.ACW_InOut.ProviderId];
#if 1 // Change to establish RT communication tentatively
				EDDP_CRT_PROVIDER_ACW_PTR_TYPE  pAcw       = &pProvMngm->pAcwEntry[pProvider->AcwList.EndIdx]; //pProvider->AuxAcwId];
				if( (pProvider->AcwList.BeginIdx < pDDB->CRT.CfgParam.ProviderAcwMaxNumber) ) //&& (pProvider->ProviderId < IOD_CFG_NUMOF_IO_AR) )
#else
				EDDP_CRT_PROVIDER_ACW_PTR_TYPE  pAcw       = &pProvMngm->pAcwEntry[pProvider->AuxAcwId];
				if( (pProvider->AcwList.BeginIdx < pDDB->CRT.CfgParam.ProviderAcwMaxNumber) && (pProvider->ProviderId < IOD_CFG_NUMOF_IO_AR) )
#endif
				{
					RTCX_PKT_PTR pTx = EDDP_LL_GetRtcSndPktfromProviderId(pProvider->ProviderId);
					RTCX_SND_HDR_PTR_NV pHdr = (RTCX_SND_HDR_PTR_NV)&pTx->Buffer[0U];                // TX Buffer
					ether_instance_descriptor_t *ptDescrptor;

          __attribute__((unused))       LSA_UINT32 ppmCycleTime =  (g_SendClock * 31250U);
					__attribute__((unused))static LSA_UINT32 green_phase_begin = 0;
					__attribute__((unused))static LSA_UINT32 tx_frame_start_offset = 0;

					R_BSP_IrqDisable(ETHSW_TDMAOUT0_IRQn);
					R_BSP_IrqDisable(ETHSW_TDMAOUT1_IRQn);

					if(LSA_TRUE == pParam->param_union.Crt.Ppm.ACW_InOut.Acw_In)
					{
						//Response = EDDP_LL_AddProvider(pHandle, pProvider, pAcw);
						// write new ApplicationLength into CMD-Ram
#ifdef PrintfDebug
						printeth("PPM_ACWIN");
#endif
						OsMemSet(pTx, 0, sizeof(RTCX_PKT));
						pTx->pDescriptor = EDDP_LL_AllocTxDescriptor(EDDP_LL_GetArIdxfromProviderId(pProvider->ProviderId), pProvider->AddParam.FrameID);

						if(LSA_NULL != pTx->pDescriptor)                                 // Should never happen as we just claimed a new descriptor
						{
							__attribute__((unused))LSA_UINT16 FrameSize = 0U;
							__attribute__((unused))LSA_UINT8 *pData = &pTx->Buffer[sizeof(RTCX_SND_HDR)];        // Pointer to frame header

							OsMemCpy(&pHdr->dst[0U], &pAcw->AddParam.DstMAC.MacAdr[0U], 6);
							OsMemCpy(&pHdr->src[0U], &pHandle->MacAddrIF[0], 6);
							ptDescrptor =(ether_instance_descriptor_t*) pTx->pDescriptor;
							// Prepare frame header
							pTx->pResource = pProvider;
							pHdr->mgmt_tag        = 0x01E0U;
							pHdr->mgmt_tag_data   = 0x0000U;
							pHdr->mgmt_tag_data20 = 0x0000U;
							pHdr->mgmt_tag_data21 = 0x0000U;
							//pHdr->vlan_tag = __REV16(0x8100U); // VLAN-Tag
							//pHdr->vlan_id  = __REV16(0xC000U); // VLAN-ID = 0, Priority = 6,
							pHdr->eth_type = __REV16(0x8892U); // Ether-Type: PROFINET
							pHdr->frame_id = __REV16(pProvider->AddParam.FrameID);

							pTx->BuffSize   = sizeof(RTCX_SND_HDR_NV);
							if(EDDP_LL_RTCX_DATA_MIN > pProvider->AddParam.DataLen)
							{
								pTx->BuffSize  += EDDP_LL_RTCX_DATA_MIN;
							}
							else
							{
								pTx->BuffSize  += pProvider->AddParam.DataLen;
							}
							pTx->IoDataSize = pProvider->AddParam.DataLen;

							//Controls the port to transmit
							if(0U < pAcw->AddParam.TxPortID)
							{
								pTx->PortID = (pAcw->AddParam.TxPortID == 1)? ETH_PORT_1 : ETH_PORT_2;
								pTx->HwPortID = (1U << (pTx->PortID -1U));
								pHdr->mgmt_tag_data |= TX_MGTAG_DATA_FORWARD_FORCE;
								pHdr->mgmt_tag_data    = __REV16(pHdr->mgmt_tag_data);
								pHdr->mgmt_tag_data21 |= ((pTx->HwPortID << TX_MGTAG_DATA2_PORT_SHIFT) & TX_MGTAG_DATA2_PORT_MASK);
								pHdr->mgmt_tag_data21  = __REV16(pHdr->mgmt_tag_data21);
							}

                            // Profinet related stuff (cycle counter, valid, etc.)
							pTx->pApdu = (RTC_APDU_PTR) &pTx->Buffer[pTx->BuffSize];
							pTx->BuffSize += sizeof(RTC_APDU);

                            // Set up cycle counter
							g_CyclCntr = 0;

							if( (0U == pHdr->dst[0U]) && (0U == pHdr->dst[1U]) &&                         // No destination address ?
								(0U == pHdr->dst[2U]) && (0U == pHdr->dst[3U]) &&
								(0U == pHdr->dst[4U]) && (0U == pHdr->dst[5U]) )
							{
								pTx->Status = EDD_STS_OK_PENDING;
							}
							else
							{
								ptDescrptor->des1 = pTx->BuffSize;
								ptDescrptor->des2 = (LSA_UINT32)&pTx->Buffer[0U];
								pTx->Status = EDD_STS_OK_NO_DATA;
//								PnUsr_ActivateIoDatXch();
							}

							if( (FRAMEID_RTC3_START <= pProvider->AddParam.FrameID) &&
								(FRAMEID_RTC3_END   >= pProvider->AddParam.FrameID)  )
							{
								pHdr->mgmt_tag_data   |= 0x000AU;   // 0a08  -> no tx-timestamping;  override queue  so we go into queue 2 to make sure RTC3 are sent in red phase
#if 0
								// setup Phases and fso for PPM
								LSA_UINT32 ppmCycleTime =  (g_SendClock * 31250U);
								static LSA_UINT32 green_phase_begin = 0;
								static LSA_UINT32 tx_frame_start_offset = 0;
#endif
								if(pParam->pDDB->SWI.LinkStatus[pTx->PortID].RsvIntervalRed.Tx.EndNs != 0)
								{
									green_phase_begin = pParam->pDDB->SWI.LinkStatus[pTx->PortID].RsvIntervalRed.Tx.EndNs;
								}
								if(pAcw->AddParam.FrameSendOffset!= EDDP_CRT_FRAME_SEND_OFFSET_UNDEFINED)
								{
									tx_frame_start_offset = pAcw->AddParam.FrameSendOffset;
								}
								pHandle->Rtc3TxCnt++;
							}
							else
							{
								pHandle->RtcxTxCnt++;
							}
							//ptDescrptor->des1 = pTx->DataSize;
							//ptDescrptor->des2 = (uint32_t)&pTx->Buffer[0U];

							Response = EDD_STS_OK;
						}
						else
						{
							Response = EDD_STS_ERR_SEQUENCE;
						}

					}
					else
					{
						//Response = EDDP_LL_RemoveProvider(pHandle, pProvider, pAcw);

						//RTCX_PKT_PTR pTx = &g_tRtcTxPktList[pProvider->ProviderId];
#ifdef PrintfDebug
						printeth("PPM_ACWOUT");
#endif

						if(pParam->param_union.Crt.Ppm.ACW_InOut.AcwId == EDDP_CRT_CHAIN_IDX_END)
						{
							if( (FRAMEID_RTC3_START <= pProvider->AddParam.FrameID) &&
								(FRAMEID_RTC3_END   >= pProvider->AddParam.FrameID) )
							{
								if(0U < pHandle->Rtc3TxCnt)
								{
									pHandle->Rtc3TxCnt--;
								}
							}
							else
							{
								if(0U < pHandle->RtcxTxCnt)
								{
									pHandle->RtcxTxCnt--;
								}
							}

							EDDP_LL_ReleaseRtcSndPkt(pProvider->ProviderId);

							Response =EDD_STS_OK ;

						}
					}

					R_BSP_IrqEnableNoClear(ETHSW_TDMAOUT0_IRQn);
					R_BSP_IrqEnableNoClear(ETHSW_TDMAOUT1_IRQn);
#if 0
					if((green_phase_begin!=0) && (tx_frame_start_offset!= 0))
					{
						LSA_UINT32 yellow_phase_begin =  ppmCycleTime - YELLOW_PHASE_DURATION;
						setup_profinet((g_SendClock * 31250U), FOS_INIT, GREEN_START_INIT, YELLOW_START_INIT, 0U, 0);
                                                tx_frame_start_offset = 0;
						green_phase_begin = 0;
					}
#endif
				}


            	//pParam->param_union.Crt.pExternalCmdParam_ApiCtrlRam;

				for ( i=0; i < pParam->param_union.Crt.Ppm.ACW_InOut.Acw_Count; i++ )
				{
			//		target_param_address = (LSA_UINT32)(&pParam->param_union.Crt.pExternalCmdParam_ApiCtrlRam[i*2]);

					// write first 32bit 'Parameterblock' into API-Ctrl-Ram
					// register_value = ((pParam->param_union.Crt.Ppm.ACW_InOut.Acw_In ? EDDP_CMD_PPM_PARAM_ACW_IN : EDDP_CMD_PPM_PARAM_ACW_OUT) |
					//				  (pParam->param_union.Crt.Ppm.ACW_InOut.ApiAdr_NextPointer_Prev[i] << EDDP_CMD_PPM_SHIFT_PARAM_NEXT_POINTER));

				}
			}
            else
            {
            	EDDP_CMD_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] _EDDP_CMD_FillCommandBlockAndParam(): FATAL -> Ppm.ACW_InOut.Acw_Count(%d) > AcwMaxNr(%d) per command!",
            	                    pParam->param_union.Crt.Ppm.ACW_OnOff.Acw_Count, pDDB->CMD.MaxAcwCountPerCmd);
            	                // EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;

        case EDDP_CMD_PPM_ACW_MOVE:
            // configure command for APICtrl-Ram
          //  target_param_address        = (LSA_UINT32)pParam->param_union.Crt.pExternalCmdParam_ApiCtrlRam;
           // target_param_address_offset = EDDP_HALGetPnipRAMOffsetToRAM (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, (EDD_COMMON_MEM_PTR_TYPE)target_param_address);

            // write first 32bit 'Parameterblock' into APICtrl-Ram
//            register_value = (((pParam->param_union.Crt.Ppm.ACW_Move.ApiAdr_NextPointer_New_Prev & EDDP_CMD_PPM_NEW_NEXT_PART_1_BITMASK) << EDDP_CMD_PPM_NEW_NEXT_PART_1_SHIFT) |
  //                            ((pParam->param_union.Crt.Ppm.ACW_Move.ApiAdr_NextPointer_Old_Prev) << EDDP_CMD_PPM_MOVE_NEXT_OLD_SHIFT));

            break;

        case EDDP_CMD_PPM_ACW_XCHANGE_BUFF: break;
        case EDDP_CMD_PACK_NOP: break; // register_value = 0x40000000; // NOP
        case EDDP_CMD_PACK_DISABLE_GRP:
            // ***** PackGrp 0..7 and bit for In=1/Out=0
        	if(pParam->param_union.Pack.DisableGrp.IsInGrp)
        	{// PNIP_REG_PACKCOMMAND_IF_CONTROL__VAL_PACKGRP_IN

        	}
        	else
        	{// PNIP_REG_PACKCOMMAND_IF_CONTROL__VAL_PACKGRP_OUT

        	}
        	break;
        case EDDP_CMD_PACK_ENABLE_GRP:
            // ***** PackGrp 0..7 and bit for In=1/Out=0
        	if(pParam->param_union.Pack.EnableGrp.IsInGrp)
        	{// PNIP_REG_PACKCOMMAND_IF_CONTROL__VAL_PACKGRP_IN

        	}
        	else
        	{// PNIP_REG_PACKCOMMAND_IF_CONTROL__VAL_PACKGRP_OUT

        	}
        	// pParam->param_union.Pack.EnableGrp.EnPackGrp
        	break;
        case EDDP_CMD_PACK_DIS_ENA_GRP:
            // ***** EnPackGrp 0..7, DisPackGrp 0..7 and bit for In=1/Out=0
        	if(pParam->param_union.Pack.DisEnaGrp.IsInGrp)
        	{// PNIP_REG_PACKCOMMAND_IF_CONTROL__VAL_PACKGRP_IN

        	}
        	else
        	{// PNIP_REG_PACKCOMMAND_IF_CONTROL__VAL_PACKGRP_OUT

        	}
        	// pParam->param_union.Pack.DisEnaGrp.DisPackGrp
        	// (pParam->param_union.Pack.DisEnaGrp.EnPackGrp & (LSA_UINT32)7) << (1+3) )
        	break;
        /*
        --> verify command with InBoundPack_x.Inbound_Valid
        --> verify command with OutBoundPack_x.Outbound_Valid
        */

        /* ------------------------------------------------ */
        /* FDB                                              */
        /* ------------------------------------------------ */
        case EDDP_CMD_FDB_CLEAR_UC_TABLE:
            /*
            register_value = (((pParam->param_union.Fdb.ClearUCTable.Delete_Only_Dynamic_Entries ? EDDP_CMD_FDB_DELETE_ONLY_DYNAMIC_ENTRIES : EDDP_CMD_FDB_DELETE_ALL_ENTRIES) << PNIP_REG_FDBCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE) |
                              ((pParam->param_union.Fdb.ClearUCTable.Delete_Only_Dynamic_Entries_Port1 ? EDDP_CMD_FDB_DELETE_ONLY_DYNAMIC_ENTRIES_PORTX : EDDP_CMD_FDB_DELETE_ONLY_DYNAMIC_ENTRIES_ALL_PORT) << (PNIP_REG_FDBCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE + 1)) |
                              ((pParam->param_union.Fdb.ClearUCTable.Delete_Only_Dynamic_Entries_Port2 ? EDDP_CMD_FDB_DELETE_ONLY_DYNAMIC_ENTRIES_PORTX : EDDP_CMD_FDB_DELETE_ONLY_DYNAMIC_ENTRIES_ALL_PORT) << (PNIP_REG_FDBCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE + 2)) |
                              ((pParam->param_union.Fdb.ClearUCTable.Delete_Only_Dynamic_Entries_Port3 ? EDDP_CMD_FDB_DELETE_ONLY_DYNAMIC_ENTRIES_PORTX : EDDP_CMD_FDB_DELETE_ONLY_DYNAMIC_ENTRIES_ALL_PORT) << (PNIP_REG_FDBCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE + 3)) |
                              ((pParam->param_union.Fdb.ClearUCTable.Delete_Only_Dynamic_Entries_Port4 ? EDDP_CMD_FDB_DELETE_ONLY_DYNAMIC_ENTRIES_PORTX : EDDP_CMD_FDB_DELETE_ONLY_DYNAMIC_ENTRIES_ALL_PORT) << (PNIP_REG_FDBCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE + 4)) |
                              (user_id << PNIP_REG_FDBCOMMAND_IF_CONTROL_SHFT_USER_ID) |
                              (PNIP_REG_FDBCOMMAND_IF_CONTROL__VAL_CONFREQUEST_ENABLE << PNIP_REG_FDBCOMMAND_IF_CONTROL_SHFT_CONFREQUEST) |
                              (PNIP_REG_FDBCOMMAND_IF_CONTROL__VAL_IF_SELECT_IFA) << PNIP_REG_FDBCOMMAND_IF_CONTROL_SHFT_IF_SELECT |
                              (PNIP_REG_FDBCOMMAND_IF_CONTROL__VAL_F_CODE_CLEAR_UC_TABLE << PNIP_REG_FDBCOMMAND_IF_CONTROL_SHFT_F_CODE));

                              EDDP_SET_CBL_HIGH_32_WORD_WCRC(register_value, PNIP_REG_FDBCOMMAND_IF_CONTROL, LSA_TRUE);
            */
          ethsw_switch_fdb_flush(ETHSW_MAC_CLEAR_DYNAMIC); //SG for MRP
          setPortNrForProv(0, (ETH_PORT_1 | ETH_PORT_2)); //SG for MRP

        	break;

        case EDDP_CMD_FDB_INSERT_UC_ENTRY:
            // write first 32bit of UC-Entry into CMD-Ram
#if 0
            register_value = ((LSA_UINT32)(pParam->param_union.Fdb.InsertUCEntry.TxPort)                                                                                                           |
                              (LSA_UINT32)(pParam->param_union.Fdb.InsertUCEntry.Static           ? EDDP_CMD_FDB_PARAM_STATIC_ACTIVATE : EDDP_CMD_FDB_PARAM_STATIC_DEACTIVATE)                     |
                              (LSA_UINT32)(pParam->param_union.Fdb.InsertUCEntry.Aging            ? EDDP_CMD_FDB_PARAM_AGING_ACTIVATE : EDDP_CMD_FDB_PARAM_AGING_DEACTIVATE)                       |
                              (LSA_UINT32)(pParam->param_union.Fdb.InsertUCEntry.RxPort << EDDP_CMD_FDB_PARAM_SHIFT_RXPORT)                                                                        |
                              (LSA_UINT32)(pParam->param_union.Fdb.InsertUCEntry.Learning_Disable ? EDDP_CMD_FDB_PARAM_LEARNING_DISABLE_ACTIVATE : EDDP_CMD_FDB_PARAM_LEARNING_DISABLE_DEACTIVATE) |
                              (LSA_UINT32)(pParam->param_union.Fdb.InsertUCEntry.Mgmt_Enable      ? EDDP_CMD_FDB_PARAM_MGMT_ENABLE_ACTIVATE : EDDP_CMD_FDB_PARAM_MGMT_ENABLE_DEACTIVATE)           |
                              (LSA_UINT32)(pParam->param_union.Fdb.InsertUCEntry.Mgmt_Prio        ? EDDP_CMD_FDB_PARAM_MGMT_PRIO_ACTIVATE : EDDP_CMD_FDB_PARAM_MGMT_PRIO_DEACTIVATE)               |
                              (LSA_UINT32)(pParam->param_union.Fdb.InsertUCEntry.UC_MAC_Address.MacAdr[5] << EDDP_CMD_FDB_PARAM_SHIFT_MAC_ADDR_5)                                                  |
                              (LSA_UINT32)(pParam->param_union.Fdb.InsertUCEntry.UC_MAC_Address.MacAdr[4] << EDDP_CMD_FDB_PARAM_SHIFT_MAC_ADDR_4));

          //  EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandParamBlock[0].Low_32Bit), register_value);
            // write second 32bit of UC entry into CMD-Ram
            register_value = (LSA_UINT32)(pParam->param_union.Fdb.InsertUCEntry.UC_MAC_Address.MacAdr[3])                                         |
                             (LSA_UINT32)(pParam->param_union.Fdb.InsertUCEntry.UC_MAC_Address.MacAdr[2] <<  EDDP_CMD_FDB_PARAM_SHIFT_MAC_ADDR_2) |
                             (LSA_UINT32)(pParam->param_union.Fdb.InsertUCEntry.UC_MAC_Address.MacAdr[1] << EDDP_CMD_FDB_PARAM_SHIFT_MAC_ADDR_1)  |
                             (LSA_UINT32)(pParam->param_union.Fdb.InsertUCEntry.UC_MAC_Address.MacAdr[0] << EDDP_CMD_FDB_PARAM_SHIFT_MAC_ADDR_0);
#endif
          //  EDDP_HAL_MEM32_WRITE(pDDB, &(pCommandParamBlock[0].High_32Bit), register_value);

            // configure command for copying param_struct from CMD-Ram into FDB-Ram
         //   target_param_address = (LSA_UINT32)EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_FDB_TABLE, EDDP_HAL_RAMSUB_FDB_TABLE_PARAM_STRUCT, 0);
         //   target_param_address = EDDP_HAL_H2P_ADDR(pDDB, target_param_address);

            // is synchronous command processing ?
            if (user_id == EDDP_CMD_USER_ID_HOST_SYNC)
            {
                // for synchonous command processing we use User_ID '1'
              //  target_param_address += 16;
            }
            break;

        /* ------------------------------------------------ */
        /* VLAN                                             */
        /* ------------------------------------------------ */
        /* ------------------------------------------------ */
        /* SYNC                                             */
        /* ------------------------------------------------ */
        case EDDP_CMD_GSYNC_UPDATE_COMCYCL:
            // write new CommunicationLength into CMD-Ram
            // register_value = pParam->param_union.Gsync.UpdateComCycl.CommunicationLength_1ns;
            // configure command for copying param_struct from CMD-Ram into Sync-Ram
            break;

        case EDDP_CMD_GSYNC_CONNECT_SYNC_A:
            // configure "Connect_SYNC_A" command for given "ApplicationTimer_x"
            /* There is a macro that checks PNIP_R2_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE and PNIP_R3_REG_SYNCCOMMAND_IF_CONTROL_SHFT_COMMANDVALUE has the same value. */
            /* Checked in eddp_hal_pnip_val.h. We use the R2 version. */

        	// --> pParam->param_union.Gsync.ConnectSyncA.ApplTimerNr:  PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_SELECT_APPL_TIMER_2
        	// EDDP_SET_CBL_HIGH_32_WORD_WCRC(register_value, PNIP_REG_SYNCCOMMAND_IF_CONTROL, LSA_TRUE);

        	break;

        case EDDP_CMD_GSYNC_UPDATE_APPCYCL:
        {
        	LSA_UINT32 CycleTime = (g_SendClock * 31250U);
            // write new ApplicationLength into CMD-Ram
            if(pParam->param_union.Gsync.UpdateAppCycl.ApplicationLengthNs != CycleTime)
            {
            	CycleTime = pParam->param_union.Gsync.UpdateAppCycl.ApplicationLengthNs;
                // start with right cycle time but initial other parameters
                setup_profinet(pDDB);
            	g_SendClock = (pParam->param_union.Gsync.UpdateAppCycl.ApplicationLengthNs / 31250U);
            }

            if (register_value < 8)
                register_value = 0; // because of 8ns granularity in register "ApplTimer_x"

        }
        break;

        case EDDP_CMD_GSYNC_UPDATE_BASECYCL:
            // write new PPM_Starttime into CMD-Ram
          //  register_value = pParam->param_union.Gsync.UpdateBaseCycl.PPM_Starttime_Ns;
            break;
        default:
            EDDP_CMD_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL,"[H:--] _EDDP_CMD_FillCommandBlockAndParam(): FATAL -> Command Opcode not supported! CmdOpcode=0x%x", pParam->CmdOpcode);
          //  EDDP_FATAL(EDDP_FATAL_ERR_PARAM);

            Response = EDD_STS_ERR_PARAM;
        break;
		}

		pParam->Response = Response;

        if(EDD_STS_OK == Response)
        {
			if( (LSA_TRUE == isAsync) &&(LSA_NULL != pHandle->hQCmdRsp) &&
				(PNIO_OK == OsQueSend(pHandle->hQCmdRsp, (PNIO_UINT32)pParam, 0U)) )
			{
				pHandle->dwEddpIrqLow = (LSA_UINT32) (1UL << EDDP_INT_REQDONE_IFA);
				Response = EDD_STS_OK;
			}
        }
        else
        {
        	if( (LSA_TRUE == isAsync) &&(LSA_NULL != pHandle->hQCmdRsp) &&
				(PNIO_OK == OsQueSend(pHandle->hQCmdRsp, (PNIO_UINT32)pParam, 0U)) )
			{
				pHandle->dwEddpIrqLow = (LSA_UINT32) (1UL << EDDP_INT_REQDONE_IFA);
				Response = EDD_STS_OK;
			}
        }
    }

    EDDP_CMD_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW,"[H:--] _EDDP_CMD_FillCommandBlockAndParam(): CmdOpcode=0x%X, register_value=0x%X, param_address=0x%X, user_id=%d", pParam->CmdOpcode, register_value, param_address, user_id);

    return (Response);
}

LSA_VOID setPortNrForProv(LSA_UINT32 ArNum, LSA_UINT32 portNr)
{
      static LSA_UINT8 ppmPortNr[IOD_CFG_NUMOF_IO_AR] = {0};

      if((ArNum >= IOD_CFG_NUMOF_IO_AR) || (ppmPortNr[ArNum] == portNr))
      {
         return; // do nothing
      }
      else
      {
          if(portNr == 0) // request for writing of pre saved port nr to mgm tag of provider frame
          {
              RTCX_PKT_PTR pTx = &g_tRtcTxPktList[ArNum];

              if((ppmPortNr[ArNum]!= 0) && (ppmPortNr[ArNum]!= pTx->PortID)) // do only if pre saved and last used tx port numbers are different
              {
                  RTCX_SND_HDR_PTR_NV pHdr = (RTCX_SND_HDR_PTR_NV)&pTx->Buffer[0U];
                  pTx->PortID = ppmPortNr[ArNum];
                  pTx->HwPortID = pTx->PortID;
                  pHdr->mgmt_tag_data |= TX_MGTAG_DATA_FORWARD_FORCE;
                  pHdr->mgmt_tag_data    = __REV16(pHdr->mgmt_tag_data);
                  pHdr->mgmt_tag_data21 = ((pTx->HwPortID << TX_MGTAG_DATA2_PORT_SHIFT) & TX_MGTAG_DATA2_PORT_MASK);
                  pHdr->mgmt_tag_data21  = __REV16(pHdr->mgmt_tag_data21);
              }
          }
          else // save new tx port nr localy
          {
             ppmPortNr[ArNum] = portNr;
          }

       }
}

LSA_VOID EDDP_LL_CRTProvAcwInitHwApiRTFrame(
	    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	    LSA_UINT16                  ProviderID,
	    LSA_UINT16                  AcwID)
{
        // SG DEAD CODE
        return;
#if 0
	EDDP_LL_HANDLE_PTR_TYPE pHandle =  (EDDP_LL_HANDLE_PTR_TYPE)EDDP_LL_MNG_HANDLE(pDDB->InterfaceID);

	if( (LSA_NULL != pDDB) && (LSA_NULL != pHandle) &&
		(LSA_NULL != pDDB->CRT.Prov.pProvEntry) &&
		(LSA_NULL != pDDB->CRT.Prov.pAcwEntry) )
	{
	    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm = &pDDB->CRT.Prov;
	    EDDP_CRT_PROVIDER_PTR_TYPE              pProvider = &pProvMngm->pProvEntry[ProviderID];
	    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw = &pProvMngm->pAcwEntry[AcwID];


	    // FrameID to be sent
	    // pAcw->AddParam.FrameID;

	    // DP_Raw_Mode  : DataPlane specification for IRTtop
	    // DP2_Type     : DP1/DP2 (DP2 = send only if Red is active)
	    switch (pAcw->ROGPhase)
	    {
	        case EDDP_CRT_PROV_ACW_PHASE_G:
	        case EDDP_CRT_PROV_ACW_PHASE_O:
	        {
	            if (pAcw->AcwId == pProvider->AuxAcwId)
	            {
	                // AUX-ACW with DataPlane specification because this FrameID is in IRT-Table available and
	                // all FrameIDs in IRT-Table are DP2 automatically!
	                // The AUX-ACW must be no DP2

	            	// PNIP_ACW_SND_WORD1_DP_RAW_MODE_ON_VALUE
	            }
	            else
	            {
	                // all other ACWs without DataPlane specification
	            	// -> PNIP_ACW_SND_WORD1_DP_RAW_MODE_OFF_VALUE
	            }
	            // DP2_Type = DP1
	            // -> PNIP_ACW_SND_WORD1_DP2_TYPE_DP1_VALUE
	        }
	        break;

	        case EDDP_CRT_PROV_ACW_PHASE_R:
	        {
	            // with DataPlane specification : PNIP_ACW_SND_WORD1_DP_RAW_MODE_ON_VALUE
	            // DP2_Type = DP2 : PNIP_ACW_SND_WORD1_DP2_TYPE_DP2_VALUE
	        }
	        break;

	        default:   break;
	    }
	    /* ----------------------------------------------------------------------------------------------- */
	    // EtherType: Index 0-3 to select one of Register "Local_SPort_Etype_0-3" : PNIP_ACW_SND_WORD1_ETHERTYPE_SELECT_INDEX0_VALUE

	    /* ----------------------------------------------------------------------------------------------- */
	    // InBound_Pack: Bit for info the pack frame is (not) a InBound-Pack-Frame: PNIP_ACW_SND_WORD1_INBOUND_PACK_OFF_VALUE
	    /* ----------------------------------------------------------------------------------------------- */
	    // RT_class: RT-Frame or RToverUDP-Frame; pProvider->RTClass
	    /* ----------------------------------------------------------------------------------------------- */
	    // ACW_DG_order: ascending or descending order for sending of ACW_DGs
	    switch (pAcw->DfpParam.ACW_DG_order)
	    {
	        case EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_HIGH: // --> PNIP_ACW_SND_WORD1_ACW_DG_ORDER_HIGH_VALUE
	        	break;
	        case EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_DOWN: // --> PNIP_ACW_SND_WORD1_ACW_DG_ORDER_DOWN_VALUE
	        	break;
	        default: break;
	    }
	    /* ----------------------------------------------------------------------------------------------- */
	    // DRM: Destination Raw Mode for port: PNIP_ACW_SND_WORD1_DRM_OFF_VALUE

	    /* ----------------------------------------------------------------------------------------------- */
	    // Destination: port specification (PortID)
	    switch (pAcw->ROGPhase)
	    {
	        case EDDP_CRT_PROV_ACW_PHASE_G:
	        case EDDP_CRT_PROV_ACW_PHASE_O:
	        	// -> PNIP_ACW_SND_WORD1_DESTINATION_OFF_VALUE
	        break;

	        case EDDP_CRT_PROV_ACW_PHASE_R:
	            if (pAcw->AddParam.TxPortID == EDD_PORT_ID_AUTO)
	            {
	            	// -> PNIP_ACW_SND_WORD1_DESTINATION_OFF_VALUE
	            }
	            else
	            {
	                // get Hw-PortID from User-PortID: PNIP_ACW_SND_WORD1_DESTINATION_MASK
	            	EDDP_SWI_get_HwPortID(pDDB, pAcw->AddParam.TxPortID);
	            }
	        break;

	        default: break;
	    }
	    /* ----------------------------------------------------------------------------------------------- */

	    // write HOST value to APICtrl-RAM
	   // EDDP_HAL_MEM64_WRITE (pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->ACW_Word1, ACW_Word64);

	    /* ----------------------------------------------------------------------------------------------- */
	    /* ----------------------------------------------------------------------------------------------- */
	    /* ACW.Word2                                                                                       */
	    /* ----------------------------------------------------------------------------------------------- */

	    // DstMAC-Address to be sent    -> pAcw->AddParam.DstMAC.MacAdr[5]
	    /* ----------------------------------------------------------------------------------------------- */
	    // Q_Select: info which queue is used by frame  trigger
	    switch (pAcw->ROGPhase)
	    {
	        case EDDP_CRT_PROV_ACW_PHASE_G:
	        case EDDP_CRT_PROV_ACW_PHASE_O:
	           // ValueTemp = PNIP_ACW_SND_WORD2_Q_SELECT_PHASE_VALUE;    // Green, Orange: Phase-Queue-14
	        break;

	        case EDDP_CRT_PROV_ACW_PHASE_R:
	            //ValueTemp = PNIP_ACW_SND_WORD2_Q_SELECT_TIME_VALUE;     // Red: Time-Queue-15
	        break;

	        default: break;
	    }

	    /* ----------------------------------------------------------------------------------------------- */
	    // PhasenInfo: which phases are used in QCW
	    switch (pAcw->ROGPhase)
	    {
	        case EDDP_CRT_PROV_ACW_PHASE_G:
	            // Green phase and TransferType
	            // ValueTemp = ( PNIP_ACW_SND_WORD2_PHASENINFO_GREEN_ON_VALUE | PNIP_ACW_SND_WORD2_PHASENINFO_TRANSFERTYPE_OFF_VALUE);
	        break;

	        case EDDP_CRT_PROV_ACW_PHASE_O:
	            // Orange, Green phases and TransferType
	            //ValueTemp = ( PNIP_ACW_SND_WORD2_PHASENINFO_ORANGE_ON_VALUE | PNIP_ACW_SND_WORD2_PHASENINFO_GREEN_ON_VALUE | PNIP_ACW_SND_WORD2_PHASENINFO_TRANSFERTYPE_ON_VALUE);
	        break;

	        case EDDP_CRT_PROV_ACW_PHASE_R:
	            // Red, Orange, Green phases and TransferType
#if 0
	            ValueTemp = ( PNIP_ACW_SND_WORD2_PHASENINFO_RED_ON_VALUE |
	                          PNIP_ACW_SND_WORD2_PHASENINFO_ORANGE_ON_VALUE |
	                          PNIP_ACW_SND_WORD2_PHASENINFO_GREEN_ON_VALUE |
	                          PNIP_ACW_SND_WORD2_PHASENINFO_TRANSFERTYPE_ON_VALUE);
#endif
	        break;

	        default: break;
	    }

	    /* ----------------------------------------------------------------------------------------------- */
	    // Tx_TimeMode: for sending in Time Queue-15 (Minumum/ExactTime): PNIP_ACW_SND_WORD2_TX_TIMEMODE_EXACT
	    /* ----------------------------------------------------------------------------------------------- */
#if 0
	    // DA_Select: select the source of DstMAC-Address
	    if (EDDP_IS_ZERO_MAC(pAcw->AddParam.DstMAC))
	    {
	        // Bit for info: get DstMAC-Address from Register "Router_MAC_DA"
	       // ValueTemp = PNIP_ACW_SND_WORD2_DA_SELECT_REG_VALUE;
	    }
	    else
	    {
	        // Bit for info: get DstMAC-Address from ACW.MAC_DA
	       // ValueTemp = PNIP_ACW_SND_WORD2_DA_SELECT_ACW_VALUE;
	    }
#endif
	    /* ----------------------------------------------------------------------------------------------- */
	    // VLAN_Tag: Frame with or without VLAN_Tag
	    switch (pAcw->ROGPhase)
	    {
	        case EDDP_CRT_PROV_ACW_PHASE_G:
	        case EDDP_CRT_PROV_ACW_PHASE_O: // Frame with VLAN_Tag:   PNIP_ACW_SND_WORD2_VLAN_TAG_ON_VALUE
	        	break;

	        case EDDP_CRT_PROV_ACW_PHASE_R: // PNIP_ACW_SND_WORD2_VLAN_TAG_OFF_VALUE
	        	break;

	        default: break;
	    }
	    /* ----------------------------------------------------------------------------------------------- */
	    // VLAN_Prio: VLAN priority:  PNIP_ACW_SND_WORD2_VLAN_PRIO_INIT_VALUE
	    /* ----------------------------------------------------------------------------------------------- */
	    // UseType_Raw_Mode: Bit for info how get the "UseType" : PNIP_ACW_SND_WORD2_USETYPE_RAW_MODE_FRAME_CLASS_VALUE
	    /* ----------------------------------------------------------------------------------------------- */
	    // UseType specification: PNIP_ACW_SND_WORD2_USETYPE_STD_VALUE
	    /* ----------------------------------------------------------------------------------------------- */

	    /* ----------------------------------------------------------------------------------------------- */
	    /* ----------------------------------------------------------------------------------------------- */
	    /* ACW.Word3                                                                                       */
	    /* ----------------------------------------------------------------------------------------------- */

	    // Scoreboard_offset: Pointer to Score Board of this Consumer (AutoStopConsumerID):  PNIP_R2_ACW_SND_WORD3_SCOREBOARD_OFFSET_OFF_VALUE
	    /* ----------------------------------------------------------------------------------------------- */
	    // DataStatus_offset: index to DataStatus struct in APICtrl-RAM:  pProvider->DataStatusId
	    /* ----------------------------------------------------------------------------------------------- */
	    // ACW_Wait and CT_Schwelle set always together for PERIF and Extern RAM :
	    #ifdef EDDP_CRT_SUPPORT_ACW_WAIT_ON
	    // --> PNIP_ACW_SND_WORD3_ACW_WAIT_ON_VALUE
	    #else
	    // PNIP_ACW_SND_WORD3_ACW_WAIT_OFF_VALUE, PNIP_ACW_SND_WORD3_CT_SCHWELLE_1_VALUE
	    #endif

	    /* ----------------------------------------------------------------------------------------------- */
	    // Tx-Time: sending time (Time Queue-15)
	   // ValueTemp = 0;
	    switch (pAcw->ROGPhase)
	    {
	        case EDDP_CRT_PROV_ACW_PHASE_G:
	        case EDDP_CRT_PROV_ACW_PHASE_O:
	           // ValueTemp = PNIP_ACW_SND_WORD3_TX_TIME_OFF_VALUE;   // Green, Orange
	        break;

	        case EDDP_CRT_PROV_ACW_PHASE_R:
	          //  ValueTemp = ( (pAcw->AddParam.FrameSendOffset) / (EDDP_UNIT_RESOLUTION_8) );    // Red
	        break;

	        default: break;
	    }

	    /* ----------------------------------------------------------------------------------------------- */
	    // reserved and DS_Handling (dependent of PN-IP revision)
		// DS_Handling: is this System-Redundancy-Provider?
		if (pProvider->ProviderFlags & EDDP_CRT_PROV_FLAGS_IS_SYSRED)
		{
			// SRD provider: PNIP_R2_ACW_SND_WORD3_DS_HANDLING_SAVE_VALUE
		}
		else
		{
			// not SRD provider: PNIP_R2_ACW_SND_WORD3_DS_HANDLING_NOT_SAVE_VALUE
		}

	    /* ----------------------------------------------------------------------------------------------- */
	    // Ressource_Raw_Mode: get "ResourceType" by ACW.ResourceType: PNIP_ACW_SND_WORD3_RESSOURCE_RAW_MODE_ACW_VALUE
	    /* ----------------------------------------------------------------------------------------------- */
	    // ResourceType
	    /* ---------------------------------------- */
	    /* ResourceType(RT_L)   : RTC2, RTC1        */
	    /* ResourceType(RT_H1)  : RTC3 Port1        */
	    /* ResourceType(RT_H2)  : RTC3 Port2        */
	    /* ---------------------------------------- */
	    switch (pAcw->ROGPhase)
	    {
	        case EDDP_CRT_PROV_ACW_PHASE_G:
	        case EDDP_CRT_PROV_ACW_PHASE_O: // ValueTemp = PNIP_ACW_SND_WORD3_RESSOURCE_TYPE_RT_L_VALUE;
	        	break;
	        case EDDP_CRT_PROV_ACW_PHASE_R: // // get Hw-PortID from User-PortID: EDDP_SWI_get_HwPortID(pDDB, pAcw->AddParam.TxPortID)
	        	break;
	        default: break;
	    }

	    /* ----------------------------------------------------------------------------------------------- */

	    /* ----------------------------------------------------------------------------------------------- */
	    /* ----------------------------------------------------------------------------------------------- */
	    /* ACW.Word4                                                                                       */
	    /* ----------------------------------------------------------------------------------------------- */

		// use PERIF
		//LSA_UINT32 User_Addr = ((pDDB->CRT.pIOBaseAdrPerif) + (pProvider->AddParam.DataOffset));
		// convert upper memory pointer to 32-Bit physical pnip address
		//EDDP_IO_PERIF_ADDR_UPPER_TO_PNIP (pDDB->hSysDev, User_Addr, &Pnip_Addr);
		// the Pnip_Addr must be 8 byte aligned - check it

	    /* ----------------------------------------------------------------------------------------------- */
	    // DataStart_offset: tbd. for IRT+

	    /* ----------------------------------------------------------------------------------------------- */
	    // DataLength: data length of only IO-Data between FrameID and APDU-Status
		// pProvider->AddParam.DataLen
	    /* ----------------------------------------------------------------------------------------------- */

	    /* ----------------------------------------------------------------------------------------------- */

	    /* ----------------------------------------------------------------------------------------------- */
	    // CR_Number: pProvider->AddParam.CRNumber  , EDDP_CRT_MAX_CRNUMBER_PERIF_ERTEC200P
	    /* ----------------------------------------------------------------------------------------------- */

	    /* ----------------------------------------------------------------------------------------------- */
	    // App_Group: pProvider->AddParam.AppGroup
	    /* ----------------------------------------------------------------------------------------------- */

	    /* ----------------------------------------------------------------------------------------------- */
	    /* ----------------------------------------------------------------------------------------------- */
	    /* ACW.Word5: only for RToverUDP (ACW.RT_class = UDP)                                              */
	    /* ----------------------------------------------------------------------------------------------- */
	    /* ----------------------------------------------------------------------------------------------- */
	    if (pProvider->RTClass == EDDP_CRT_RTCLASS_UDP)
	    {
	        /* ----------------------------------------------------------------------------------------------- */
	        // IP_SA_select: select Source IP Address
	        // IFA:     ValueTemp = PNIP_ACW_SND_WORD5_IP_SA_SELECT_INDEX0_VALUE_VALUE;

	        /* ----------------------------------------------------------------------------------------------- */
	        // UDP_SP_select: get UDP-SrcPort aus Register "Local_SPort_Etype_x"
	        /* ----------------------------------------------------------------------------------------------- */
	        // IP_DA: Destination IP Address : pProvider->AddParam.DstIP.IPAdr
	        /* ----------------------------------------------------------------------------------------------- */
	        // UDP_DP: UDP Destination-Port: EDDP_CRT_ETHER_TYPE: 0x8892
	        /* ----------------------------------------------------------------------------------------------- */
	    }

	    /* ----------------------------------------------------------------------------------------------- */
	    /* ----------------------------------------------------------------------------------------------- */
	    /* ACW.PpmSwData                                                                                   */
	    /* ----------------------------------------------------------------------------------------------- */
	    /* ----------------------------------------------------------------------------------------------- */
	   // EDDP_HAL_MEM16_WRITE (pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->PpmSwData.ProviderId, ProviderID);

	    /* ----------------------------------------------------------------------------------------------- */
	    /* ACW in HW available                                                                             */
	    /* ----------------------------------------------------------------------------------------------- */
	    // EDDP_CRT_PROV_ACW_STATE_BIT_ON
	}
#endif
}
uint32_t phasetime; 
LSA_VOID  EDDP_LL_FilterFrame(EDDP_LL_HANDLE_PTR_TYPE pHandle, LSA_UINT16 PortID, LSA_VOID *pvFrame, LSA_UINT16 Size,  LSA_UINT32 pvTimeStamp)
{
      R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;

	LSA_UINT32  RxRingIdx = 0xFF;

	EDDP_LOCAL_DDB_PTR_TYPE     pDDB = (EDDP_LOCAL_DDB_PTR_TYPE)pHandle->hDDB;

	if( (LSA_NULL != pHandle) && (LSA_NULL != pHandle->hDDB) &&  (LSA_NULL != pvFrame) )
	{
		ETH_HDR_MGMTTAG_PTR pEthHdr   = (ETH_HDR_MGMTTAG_PTR)pvFrame;
		LSA_UINT8  *pEthDat   =  (LSA_UINT8*) ((LSA_UINT32)pvFrame + sizeof(ETH_HDR) + MGMT_TAG_SIZE);
		LSA_UINT16  EthType   = pEthHdr->type;
		LSA_UINT16  DataSize  = Size;     // Size has already the MGMTTAG subtracted
		LSA_UINT16 TypeOffset = 0U;
		LSA_UINT16 FrameID    = 0U;
		LSA_UINT16 UsrChIdx   = 0xFF;
		LSA_UINT16 VLanID     = 0U;
		LSA_BOOL   bNrtFrame  = LSA_TRUE;

		if(ETH_TYPE_VLAN == EthType)
		{
			EthType      = ((ETH_HDR_VLAN_MGMTTAG_PTR)pvFrame)->type;
			VLanID       = ((ETH_HDR_VLAN_MGMTTAG_PTR)pvFrame)->vlan_id;
			TypeOffset   = 2;
			pEthDat      = (LSA_UINT8*) ((LSA_UINT32)pvFrame + sizeof(ETH_HDR_VLAN_MGMTTAG));
			DataSize    -= 4U;
		}
#if IOD_INCLUDE_STORM_FILTER_ETHER_SW
		// Check the number of receptions per cycle by Ether type.
		if(EDD_STS_OK != EDDP_LL_EtherTypeFilter(htons(EthType)))
		{
			// Frame dropped due to reception limit
			return;
		}
#endif /* IOD_INCLUDE_STORM_FILTER_ETHER_SW */

		switch(EthType)
		{
		case ETH_TYPE_PROFINET:
		{
			FrameID = (LSA_UINT16)__REV16((LSA_UINT16) *((LSA_UINT16*)&pEthDat[0U])); //EDDP_GET_U16(pEthDat);
			uint32_t SeqID =  (LSA_UINT16)__REV16((LSA_UINT16) *((LSA_UINT16*)&pEthDat[14U])); //EDDP_GET_U16(pEthDat);

			if( (FRAMEID_RTCX_START <= FrameID) && (FRAMEID_RTCX_END >= FrameID) )
			{
				bNrtFrame = LSA_FALSE;
				// Processes RTC frames.
				RxRingIdx = EDDP_ARD_INDEX_DROP;
				RTCX_PKT_PTR  ptRtcRcv = (RTCX_PKT_PTR)EDDP_LL_GetRtcRcvPkt(pHandle, FrameID, &pEthHdr->src[0]);
				if(LSA_NULL != ptRtcRcv)
				{
					LSA_BOOL FirstPacket  = (ptRtcRcv->CSBstage == EDDP_LLIF_CSB_NOT_ACTIVATE) ? LSA_TRUE : LSA_FALSE;
					// Frame validity check and scoreboard notification determination
					LSA_UINT16 ConsumerID = EDDP_LL_ConsumerFrameValidityCheck((EDDP_LOCAL_DDB_PTR_TYPE)pHandle->hDDB, (LSA_UINT8*)pEthHdr, pEthDat, (DataSize - sizeof(ETH_HDR)), FirstPacket);
					if(ConsumerID != EDDP_CRT_CHAIN_IDX_END)
					{
						// Received RTC frames are referenced in Task_CycleIO -> EDDP_LL_To_Valid_Ar
						ptRtcRcv->BuffSize  = DataSize;
						ptRtcRcv->PortID    = PortID;

						OsMemCpy(&ptRtcRcv->Buffer[0U], pEthHdr, 12);
						OsMemCpy(&ptRtcRcv->Buffer[12U], &(pEthHdr->type), 2);
						OsMemCpy(&ptRtcRcv->Buffer[sizeof(ETH_HDR) - TypeOffset], pEthDat - TypeOffset, DataSize - sizeof(ETH_HDR) + TypeOffset);

						LSA_UINT32 ArIdx = EDDP_LL_GetArIdxfromConsumerId(ConsumerID);
						setPortNrForProv(ArIdx, ptRtcRcv->PortID);
						EDDP_CRT_CONSUMER_PTR_TYPE pConsumer = &pDDB->CRT.Cons.pConsEntry[ConsumerID];
						ether_instance_t *ptEth = EDDP_LL_CAST_ETH(pHandle->pIfHandle);
						ether_switch_instance_ctrl_t *ptSwCtrl = (ether_switch_instance_ctrl_t*)ptEth->p_cfg->p_ether_switch_instance->p_ctrl;
						R_ETHSW_Type *ptEthSw = (R_ETHSW_Type*)(R_ETHSW_Type*) ptSwCtrl->p_reg_switch;
						LSA_UINT8 useTimer = ptEthSw->COMMAND_CONFIG_P3_b.TIMER_SEL;
						// Extension of DHT Expiration timer
						OsIntDisable();
						pConsumer->DHTExpireCheckEnable = LSA_TRUE;
						pConsumer->FirstPacket = LSA_TRUE;
						pConsumer->DHTExpireCount = 0;
						OsIntEnable();

						/**
						 * When RTCx frame is received from peer of AR connection , invoke EDDP_INT_CSCOREBOARD_CHANGED_IFA interrupt
						 * to send RTCx frame after AR connection establishment.
						*/
						LSA_UINT32 CSBValue = 0;
                        // IRT comuunication
						if( (FRAMEID_RTC3_START <= FrameID) && (FRAMEID_RTC3_END >= FrameID) )
						{
							/** Flags to be set to Consumer Score board. */
							switch( ptRtcRcv->CSBstage )
							{
							case EDDP_LLIF_CSB_NOT_ACTIVATE: /** First interrupts */
								/**
								 * Set CSB Api register value to 0x008201e8 which is observed in EDDP_CRTScoreboardHandler() on ERTEC.
								 * TODO: The validty of this flags should be verified.
								 */
								CSBValue = 0
									| PNIP_CPM_CSB_WORD1_RCVD_IN_DP2_CHD_MASK	/// 0x00800000
									| PNIP_CPM_CSB_WORD1_DHT_EXPIRED_CHD_MASK	/// 0x00020000
									| PNIP_CPM_CSB_WORD1_FRM_RCVD_MASK			/// 0x00000100
									| PNIP_CPM_CSB_WORD1_RCVD_IN_DP2_MASK		/// 0x00000080
									| PNIP_CPM_CSB_WORD1_DS_PROBL_IND_MASK		/// 0x00000040
									| PNIP_CPM_CSB_WORD1_DS_RUNSTOP_MASK	   	/// 0x00000020
									| PNIP_CPM_CSB_WORD1_DS_STATE_MASK			/// 0x00000008
									;

								if(LSA_FALSE == pConsumer->ScoreBoard.WaitSetToUnknown)
								{
									/** Increment CSB interrupt stage */
									ptRtcRcv->CSBstage = EDDP_LLIF_CSB_ACTIVATE_STAGE1;
								}
								break;

							case EDDP_LLIF_CSB_ACTIVATE_STAGE1: /** Second interrupts */
								/**
								 * Set CSB Api register value to 0x000201e8 which is observed in EDDP_CRTScoreboardHandler() on ERTEC.
								 * TODO: The validty of this flags should be verified.
								 */
								CSBValue = 0
									//| PNIP_CPM_CSB_WORD1_RCVD_IN_DP2_CHD_MASK	/// 0x00800000
									| PNIP_CPM_CSB_WORD1_DHT_EXPIRED_CHD_MASK	/// 0x00020000
									| PNIP_CPM_CSB_WORD1_FRM_RCVD_MASK			/// 0x00000100
									| PNIP_CPM_CSB_WORD1_RCVD_IN_DP2_MASK		/// 0x00000080
									| PNIP_CPM_CSB_WORD1_DS_PROBL_IND_MASK		/// 0x00000040
									| PNIP_CPM_CSB_WORD1_DS_RUNSTOP_MASK	   	/// 0x00000020
									| PNIP_CPM_CSB_WORD1_DS_STATE_MASK			/// 0x00000008
									;

                                if(LSA_TRUE == pConsumer->ScoreBoard.WaitSetToUnknown)
								{
									/** Increment CSB interrupt stage */
									ptRtcRcv->CSBstage = EDDP_LLIF_CSB_ACTIVATE_STAGE2;
								}
								break;

							default:
								/** Do nothing. */
								break;
							}
						}
                        // RT Communication
						else
						{
							/** Flags to be set to Consumer Score board. */
							switch( ptRtcRcv->CSBstage )
							{
							case EDDP_LLIF_CSB_NOT_ACTIVATE: /** First interrupts */
								/**
								 * Set CSB Api register value to 0x00020168 which is observed in EDDP_CRTScoreboardHandler() on ERTEC.
								 * TODO: The validty of this flags should be verified.
								 */
								CSBValue = 0
									| PNIP_CPM_CSB_WORD1_DHT_EXPIRED_CHD_MASK	/// 0x00020000
									| PNIP_CPM_CSB_WORD1_FRM_RCVD_MASK			/// 0x00000100
									| PNIP_CPM_CSB_WORD1_DS_PROBL_IND_MASK		/// 0x00000040
									| PNIP_CPM_CSB_WORD1_DS_RUNSTOP_MASK	   	/// 0x00000020
                                    | PNIP_CPM_CSB_WORD1_DS_STATE_MASK			/// 0x00000008
									;

								if(LSA_FALSE == pConsumer->ScoreBoard.WaitSetToUnknown)
								{
									/** Increment CSB interrupt stage */
									ptRtcRcv->CSBstage = EDDP_LLIF_CSB_ACTIVATE_STAGE1;
								}
								break;
							case EDDP_LLIF_CSB_ACTIVATE_STAGE1: /** Second interrupts */
								/**
								 * Set CSB Api register value to 0x00020168 which is observed in EDDP_CRTScoreboardHandler() on ERTEC.
								 * TODO: The validty of this flags should be verified.
								 */
								CSBValue = 0
									| PNIP_CPM_CSB_WORD1_DHT_EXPIRED_CHD_MASK	/// 0x00020000
									| PNIP_CPM_CSB_WORD1_FRM_RCVD_MASK			/// 0x00000100
									| PNIP_CPM_CSB_WORD1_DS_PROBL_IND_MASK		/// 0x00000040
									| PNIP_CPM_CSB_WORD1_DS_RUNSTOP_MASK	   	/// 0x00000020
									| PNIP_CPM_CSB_WORD1_DS_STATE_MASK			/// 0x00000008
									;

                                if(LSA_TRUE == pConsumer->ScoreBoard.WaitSetToUnknown)
								{
									/** Increment CSB interrupt stage */
									ptRtcRcv->CSBstage = EDDP_LLIF_CSB_ACTIVATE_STAGE2;
								}
								break;
							default:
								if(LSA_TRUE == pConsumer->ScoreBoard.WaitSetToUnknown)
								{
									// Indication of the 1stRTC frame is required
									ptRtcRcv->CSBstage = EDDP_LLIF_CSB_NOT_ACTIVATE;
								}
								break;
							}                    
						}
						// Set request for EDDP_INT_CSCOREBOARD_CHANGED_IFA interrupt
						if(CSBValue)
						{
                          	/** Set the CSB value to CSB  */
							EDDP_LL_SetCurrrentConsumerScoreboard(pDDB,ConsumerID,CSBValue);

							/** Generate the interrupt EDDP_INT_CSCOREBOARD_CHANGED_IFA */
							pHandle->dwEddpIrqMid |= (1UL << (EDDP_INT_CSCOREBOARD_CHANGED_IFA - EDDP_ISR_LOW_MAX_EVENTS));
						}

						/* Update APDUStatus */
						RTC_APDU_PTR pRcvStat = (RTC_APDU_PTR)&ptRtcRcv->Buffer[ptRtcRcv->BuffSize - 4U/* APDUStatus Offset */];
						pConsumer->ConsAcwTypeInfo.pHwApduApi->CycleCounter     = (LSA_UINT16)__REV16(pRcvStat->cycle_counter);
						pConsumer->ConsAcwTypeInfo.pHwApduApi->TransferStatus  = pRcvStat->transfer_status;
						pConsumer->ConsAcwTypeInfo.pHwApduApi->DataStatus      = pRcvStat->data_status;
						// Request EDDP_INT_CSCOREBOARD_CHANGED_IFA interrupt
						if(0U < pHandle->dwEddpIrqMid)
						{
							// Scoreboard changed, request interrupt handler EDDP_CRT_ISR_CSB_changed_IFA
							EDDP_ISR_GROUP_INFO_PTR_TYPE pGroup = &pDDB->Core.Isr.Group;
							PNIP_HAL_REG32_WRITE(PNIP_REG_IRQ2EVENT_MID_0, pHandle->dwEddpIrqMid);
							pHandle->dwEddpIrqMid = 0U;
							pGroup->IntRQB.InUse = LSA_TRUE; /* RQB is underway.. */
							EDDP_DO_INTERRUPT_REQUEST(pDDB->hSysDev, pGroup->IntRQB.pRQB);
						}
					}
				}
			}
			else
			{
				switch(FrameID)
				{
				case FRAMEID_ALARM_HIGH:
				case FRAMEID_ALARM_LOW:
				case FRAMEID_EVENT_LOW:
					RxRingIdx = EDDP_ARD_INDEX_ALARM;
					UsrChIdx  = EDDP_NRT_FILTER_ASRT_IDX;
					break;

				case FRAMEID_DCP_HELLO:
					RxRingIdx = EDDP_ARD_INDEX_DCP;
					UsrChIdx  = EDDP_NRT_FILTER_UDP_DCP_HELLO_IDX;
					break;

				case FRAMEID_DCP_IDENT_REQ:
#if IOD_INCLUDE_STORM_FILTER_DCP_SW
				{
					DCP_HDR_PTR pDcpHdr = (DCP_HDR_PTR)pEthDat;
					if(DCP_DBID_IDENT == pDcpHdr->option
					&& pDcpHdr->suboption == DCP_DSID_IDNAME)
					{
						if(g_StationNameLen)
						{
							LSA_UINT16 NameLen = (LSA_UINT16)__REV16((LSA_UINT16)pDcpHdr->dcpblock_length);
							if(NameLen != g_StationNameLen)
							{
								break;
							}
							else if(memcmp(pDcpHdr->data, g_StationNamePrt, NameLen))
							{
								break;
							}
						}
					}
				}
#endif /* IOD_INCLUDE_STORM_FILTER_DCP_SW */
				case FRAMEID_DCP:
				case FRAMEID_DCP_IDENT_RSP:
					RxRingIdx = EDDP_ARD_INDEX_DCP;
					UsrChIdx  = EDDP_NRT_FILTER_DCP_IDX;
					break;

				default:
					RxRingIdx = EDDP_ARD_INDEX_OTHER;
					UsrChIdx  = EDDP_NRT_FILTER_OTHER_IDX;
					break;
				}
			}
		}break;
		case ETH_TYPE_IPV4:
		{
			IPV4_HDR_PTR pHdr =(IPV4_HDR_PTR)pEthDat;
			if( ( (ETH_IPV4_H4500 == pHdr->VerLen) || (ETH_IPV4_H4000 == pHdr->VerLen)) )
			{
				switch(pHdr->protocol)
				{
				case 1:
					RxRingIdx = EDDP_ARD_INDEX_IP_MISC;
					UsrChIdx  = EDDP_NRT_FILTER_IP_ICMP_IDX;
					break;
				case 2:
					RxRingIdx = EDDP_ARD_INDEX_IP_MISC;
					UsrChIdx  = EDDP_NRT_FILTER_IP_IGMP_IDX;
					break;
				case 6:
					RxRingIdx = EDDP_ARD_INDEX_IP_TCP;
					UsrChIdx  = EDDP_NRT_FILTER_IP_TCP_IDX;
					break;
				case 17:
				{
					LSA_UINT8 *pFrame = (LSA_UINT8*)((LSA_UINT32)pEthDat + sizeof(IPV4_HDR) + 2);
					LSA_UINT16 UDPDstPort  = EDDP_GET_U16(pFrame);

					EDDP_LL_AddRemoveStaticMacEntry(pFrame, PortID, pEthHdr, VLanID);

					if( (0x01U & pEthHdr->dst[0]) && (EDD_SYS_UDP_WHITELIST_FILTER_ON == pDDB->NRT.NrtIFCfg.RxFilterUDP_Broadcast) )
					{
						if (   (UDPDstPort == EDDP_NRT_UDP_DST_PORT_NTP_SNTP) ||
						       (UDPDstPort == EDDP_NRT_UDP_DST_PORT_DHCP_CLIENT) )
						{
							RxRingIdx = EDDP_ARD_INDEX_IP_UDP;
							UsrChIdx  = EDDP_NRT_FILTER_IP_UDP_IDX;
						}
					}
					else
					{
						if( (0 == (0x01U & pEthHdr->dst[0])) && (EDD_SYS_UDP_WHITELIST_FILTER_ON == pDDB->NRT.NrtIFCfg.RxFilterUDP_Unicast) )
						{
							 if (   ((UDPDstPort >= EDDP_NRT_UDP_DST_PORT_SNMP_MIN) && (UDPDstPort <= EDDP_NRT_UDP_DST_PORT_SNMP_MAX))
                                 || (UDPDstPort == EDDP_NRT_UDP_DST_PORT_IANA_EPM)
#if (EDDP_NRT_UDP_DST_PORT_FREE_MAX==0xFFFF)
								 || (UDPDstPort >= EDDP_NRT_UDP_DST_PORT_FREE_MIN)
#else
								 || ((UDPDstPort >= EDDP_NRT_UDP_DST_PORT_FREE_MIN) && (UDPDstPort <= EDDP_NRT_UDP_DST_PORT_FREE_MAX))
#endif
								 || (UDPDstPort == EDDP_NRT_UDP_DST_PORT_DHCP_CLIENT)
								 || (UDPDstPort == EDDP_NRT_UDP_DST_PORT_NTP_SNTP) )
							 {
								 RxRingIdx = EDDP_ARD_INDEX_IP_UDP;
								 UsrChIdx  = EDDP_NRT_FILTER_IP_UDP_IDX;
							 }
						}
					}
				}break;
				default:
					RxRingIdx = EDDP_ARD_INDEX_IP_MISC;
					UsrChIdx  = EDDP_NRT_FILTER_IP_OTHER_IDX;
					break;
				}
			}

		}break;
		case ETH_TYPE_ARP:	// 0x0806U
			{
#if IOD_INCLUDE_STORM_FILTER_ARP_SW
				ARP_HDR_PTR pArpHdr = (ARP_HDR_PTR)pEthDat;
				if(pDDB->SWI.IF_IP_Address.IPAdr[0] == pArpHdr->tpa[0]
				&& pDDB->SWI.IF_IP_Address.IPAdr[1] == pArpHdr->tpa[1]
				&& pDDB->SWI.IF_IP_Address.IPAdr[2] == pArpHdr->tpa[2]
				&& pDDB->SWI.IF_IP_Address.IPAdr[3] == pArpHdr->tpa[3]
				)
#endif /* IOD_INCLUDE_STORM_FILTER_ARP_SW */
        	    {
 					RxRingIdx = EDDP_ARD_INDEX_IP_MISC;
					UsrChIdx  = EDDP_NRT_FILTER_ARP_IDX;
				}
        	}
			break;
		case ETH_TYPE_MRP:	// 0x88E3U
		case ETH_TYPE_DLR:	// 0x88E1U
			RxRingIdx = EDDP_ARD_INDEX_MRP;
			UsrChIdx  = EDDP_NRT_FILTER_MRP_IDX;
			break;
		case ETH_TYPE_LLDP:	// 0x88CCU
			RxRingIdx = EDDP_ARD_INDEX_LLDP;
			UsrChIdx  = EDDP_NRT_FILTER_LLDP_IDX;
			break;
		case ETH_TYPE_IPV6:
			return;
			break;
		default:
			RxRingIdx = EDDP_ARD_INDEX_OTHER;
			UsrChIdx  = EDDP_NRT_FILTER_OTHER_IDX;
			break;
		}

		if(
			bNrtFrame &&
			(EDDP_HW_MAX_RX_RINGS > RxRingIdx) &&
			(EDDP_NRT_MAX_FDC_CNT > UsrChIdx) &&
			(LSA_NULL != pDDB->NRT.FilterInfo[UsrChIdx].pNrtHDB) &&
			(0U < pDDB->NRT.FilterInfo[UsrChIdx].pNrtHDB->RxUsrQueue.Count))
		{
			EDDP_LOCAL_DDB_NRT_PTR_TYPE pNRTIface = &pDDB->NRT;
			EDDP_LOCAL_HDB_NRT_PTR_TYPE pNrtHDB   = pDDB->NRT.FilterInfo[UsrChIdx].pNrtHDB;
			EDDP_NRT_RX_RING_TYPE_PTR  pRxHwDescrRingMgm = &pDDB->NRT.RxHwDescrRingMgm[RxRingIdx];
			__attribute__((unused))PNIP_LOCAL_DMACW_RCV_ITEM_PTR_TYPE pRxRingItem = &pRxHwDescrRingMgm->pRxRing[pRxHwDescrRingMgm->DoneIdx];
			EDD_UPPER_MEM_PTR_TYPE RxBufAddr    = pRxHwDescrRingMgm->pFrameBufferArray->FrameBufferUpperAddr[pRxHwDescrRingMgm->DoneIdx];
			EDD_UPPER_RQB_PTR_TYPE pRQB         = EDDP_GlbRQBQueueRemoveFromBegin (&pNrtHDB->RxUsrQueue);
			EDD_UPPER_NRT_RECV_PTR_TYPE pRQBRcv = pRQB->pParam;
			EDDP_LOCAL_HDB_PTR_TYPE pHDB        = LSA_NULL;

			if( (EDD_STS_OK == EDDP_GlbHandleGetHDB (pNrtHDB->EDD_Handle, &pHDB) ) &&
				(LSA_NULL != pRQB) && (LSA_NULL != pRQB->pParam) && (LSA_NULL != pHDB) )
			{
				pRQBRcv = pRQB->pParam;

				if(LSA_NULL == pRQBRcv->pBuffer)
				{
					pRQBRcv->pBuffer = RxBufAddr;
				}

				if(LSA_NULL != pRQBRcv->pBuffer)
				{
					OsMemCpy(pRQBRcv->pBuffer, pEthHdr, 12);   //sizeof(ETH_HDR));   // Cope Src + Dest MAC Addr
					OsMemCpy(&pRQBRcv->pBuffer[12], &(pEthHdr->type), 2);   //   // Copy Type

					OsMemCpy(&pRQBRcv->pBuffer[sizeof(ETH_HDR) - TypeOffset], pEthDat - TypeOffset, DataSize - sizeof(ETH_HDR) + TypeOffset);

					pRQBRcv->IOCount        = DataSize;
					pRQBRcv->UserDataLength = pRQBRcv->IOCount - EDD_NRT_MIN_SND_LEN;   // (DST mac) 6 Byte + (SRC mac) 6 Byte + (LNG/Type) 2 Byte
					pRQBRcv->UserDataOffset = EDD_NRT_MIN_SND_LEN;
					pRQBRcv->PortID         = PortID;
					pRQBRcv->FrameFilter    = pNRTIface->FilterInfo[UsrChIdx].UsrFrameFilter;

					pRQBRcv->FrameFilterInfo    = 0x0;

					for(LSA_UINT32 idx = 0; idx < pDDB->NRT.DcpHelloFilterCnt; idx++)
					{
						if(pNRTIface->FilterInfo[UsrChIdx].HWFrameFilter == (EDDP_FDC_DCP_HELLO | (idx << 10)))
						{
							pRQBRcv->FrameFilterInfo    = pHDB->pDDB->NRT.pDCPHelloFilter[idx].UserID;
							break;
						}
					}

					pRQBRcv->RequestCnt = pDDB->NRT.FilterInfo[UsrChIdx].pNrtHDB->RxUsrQueue.Count;
			      	pRQBRcv->RxTime = pvTimeStamp;

					EDDP_GlbRequestFinish (pHDB, pRQB, EDD_STS_OK);

					if ( pEthHdr->dst[0] & 1 )
					{
						// counts BC/MC rx-packets
						pNRTIface->Stats.InNUcastPkts ++;
						if(pRQBRcv->pBuffer[0] == 0xFF)
						{
							// counts BC rx-packets
							pNRTIface->Stats.InBroadcastPkts++;
						}
						else
						{
							// counts MC rx-packets
							pNRTIface->Stats.InMulticastPkts++;
						}
					}
					else
					{
						// counts UC rx-packets
						pNRTIface->Stats.InUcastPkts ++;
					}

					if ((++pRxHwDescrRingMgm->DoneIdx) >= pRxHwDescrRingMgm->ItemCount)
					{
						pRxHwDescrRingMgm->DoneIdx = 0;
					}
				}
			}
			}
		}
}

LSA_RESULT EDDP_LL_TX_BUFF_AVAILABLE(LSA_UINT32 InterfaceID)
{
    LSA_RESULT  Status = EDD_STS_OK_NO_DATA;
    EDDP_LL_HANDLE_PTR_TYPE   pHandle = (EDDP_LL_HANDLE_PTR_TYPE)EDDP_LL_MNG_HANDLE(InterfaceID);
    ether_instance_ctrl_t *ptIfCtrl = EDDP_LL_ETH_CTRL_PTR(pHandle->pIfHandle);

	if( (NULL != pHandle) && (EDDP_LLIF_STATE_OPERATIONAL == pHandle->u16State) &&  (0U < pHandle->dwLinkStatusAll) &&
		(LSA_NULL != pHandle->pIfHandle) && (LSA_NULL != ptIfCtrl) && (LSA_NULL != ptIfCtrl->p_reg_etherc) )
	{
		// LSA_UINT16 nDescrCnt = 0U;
	//	ether_instance_descriptor_t * p_tx_descriptor;
//		ether_instance_descriptor_t * p_tx_descriptors= ptIfCtrl->p_ether_cfg->p_tx_descriptors;

		if(0U == (ptIfCtrl->p_tx_descriptor->des0 & ETHER_TDES0_OWN))
		{
			Status = EDD_STS_OK;
		}
	}

    return Status;
}

uint32_t count_nrt = 0;
uint32_t count_nrt_switch = 0;
/**
 * \brief           lower layer that queues and ethernet frame (preparation for transmission)
 *
 * \param[in]       pLLManagement           lower layer handle
 * \param[in]       pBufferAddr             address of the send buffer (given from EDDP/application)
 * \param[in]       Length                  length of the frame
 * \param[in]       PortID                  port ID for transmission
 *
 * \note            see EDDP_LLIF.DOCX for more informations
 *
 * \retval          EDD_STS_OK              good
 * \retval          EDD_STS_ERR_RESOURCE    any parameter (input) cause the error (see traces for details)
 */
LSA_RESULT EDDP_LOCAL_FCT_ATTR EDDP_LL_NRT_SEND(
		LSA_UINT32 InterfaceID,
		EDD_UPPER_MEM_PTR_TYPE  pBufferAddr,
		LSA_UINT32              Length,
		LSA_UINT32              PortID,
		LSA_VOID*               hUpper)
{
	LSA_RESULT                 Status = EDD_STS_ERR_RESOURCE;
	EDDP_LL_HANDLE_PTR_TYPE   pHandle = (EDDP_LL_HANDLE_PTR_TYPE)EDDP_LL_MNG_HANDLE(InterfaceID);

	OsTakeSemB(pHandle->IfSendLock);

	ether_instance_t *ptIf = EDDP_LL_CAST_ETH(pHandle->pIfHandle);
	ether_instance_ctrl_t *ptIfCtrl = EDDP_LL_ETH_CTRL_PTR(pHandle->pIfHandle);
	EDDP_SWI_TRACE_04(pHandle->TraceIdx,LSA_TRACE_LEVEL_NOTE_LOW, "IN :EDDP_LL_SEND(pLLM: 0x%X, pBufferAddr: 0x%X, Length: 0x%X, PortID: %d)", pHandle, pBufferAddr, Length, PortID);

	if( (NULL != pHandle) && (EDDP_LLIF_STATE_OPERATIONAL == pHandle->u16State) &&
		 (LSA_NULL != ptIf) && (LSA_NULL != ptIfCtrl) && (LSA_NULL != ptIfCtrl->p_reg_etherc) &&
		 (0U < pHandle->dwLinkStatusAll) && (LSA_NULL != pBufferAddr)  )
	{
		ptIfCtrl->tx_manage.flg_func = ETHER_SWITCH_TX_FUNC_NON;

		if(ETHER_MINIMUM_FRAME_SIZE > Length)
		{
		    uint8_t *addr = (uint8_t *) ((uint32_t)pBufferAddr + Length);
		    for (; Length<ETHER_MINIMUM_FRAME_SIZE; Length++)
		    {
		        *addr = 0;
		        addr++;
		    }
		}

		ptIfCtrl->write_port = 0;

		if(0U < PortID)
		{
			ptIfCtrl->write_port = (1U << (PortID-1U));
		}

		if(FSP_SUCCESS == ptIf->p_api->write(ptIfCtrl, (void*)pBufferAddr, Length))
		{
			Status = EDD_STS_OK;
		}
		else
		{
			Status = EDD_STS_ERR_TX;
		}

		EDDP_SWI_TRACE_01(pHandle->TraceIdx,LSA_TRACE_LEVEL_NOTE_LOW, "OUT:EDDP_LL_SEND(), Status: 0x%X", Status);
	}
	else
	{
		Status = EDD_STS_ERR_PARAM;
		EDDP_SWI_TRACE_01(pHandle->TraceIdx,LSA_TRACE_LEVEL_ERROR, "EDDP_LL_SEND: invalid frame length (%u)", Length);
	}

	OsGiveSemB(pHandle->IfSendLock);

	return Status;
}

/**
 * \brief           lower layer that triggers the transmission of queued ethernet frames
 *
 * \param[in]       pLLManagement           lower layer handle
 *
 * \note            see EDDP_LLIF.DOCX for more informations   AG: This is called from the stack to retrigger the GMAC to send some more frame that came up during the green phase
                     so they are transmitted during the current phase and do not have to wait for the next phase.
 *
 * \retval          LSA_VOID
 */
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_LL_SEND_TRIGGER(LSA_UINT32 InterfaceID)
{
	EDDP_LL_HANDLE_PTR_TYPE   pHandle = (EDDP_LL_HANDLE_PTR_TYPE)EDDP_LL_MNG_HANDLE(InterfaceID);
    ether_instance_ctrl_t *ptIfCtrl = EDDP_LL_ETH_CTRL_PTR(pHandle->pIfHandle);

    EDDP_SWI_TRACE_01(pHandle->TraceIdx,LSA_TRACE_LEVEL_NOTE_LOW, "IN :EDDP_LL_SEND_TRIGGER(pLLM: 0x%X)", pLLManagement);

	if( (NULL != pHandle) && (EDDP_LLIF_STATE_OPERATIONAL == pHandle->u16State) &&
		(LSA_NULL != pHandle->pIfHandle) && (LSA_NULL != ptIfCtrl) && (LSA_NULL != ptIfCtrl->p_reg_etherc) )
    {
		volatile R_GMAC_Type *p_reg_etherc =  (R_GMAC_Type *) ptIfCtrl->p_reg_etherc;
		ether_instance_descriptor_t * p_tx_descriptor_last = ptIfCtrl->p_tx_descriptor;
	    if (p_reg_etherc->Status_b.TPS == 0)    // TX still running, nothing to do
	    {
	      return;
	    }
		if(EDDP_LL_PHASE_GREEN == g_TxPhaseState)  // Still green phase and transmission of previous frames stopped
		{
            //*********  Only used to set tx ISR after last frame
            // go down one descriptor
			if (ptIfCtrl->p_tx_descriptor == ptIfCtrl->p_ether_cfg->p_tx_descriptors)       // Revert to last vaid frame/descriptor
			{
				p_tx_descriptor_last = &g_tx_descriptors_nrt[(NUM_TX_DESCRIPTOR-1)];
			}
			else
			{
				p_tx_descriptor_last--;
			}
            p_tx_descriptor_last->des0 |= (ETHER_TDES0_IC);  // | ETHER_TDES0_TER); -> AG: Interupt on completion -> are we using this ?

			ether_instance_descriptor_t *p_tx_descriptor = LSA_NULL;
			R_BSP_IrqDisable(ETHSW_TDMAOUT1_IRQn);
            /* Check if there is a trasmission ongoing, in this case no action is required. If the tx that was initiated by the
               green ISR was already handled and we have meanwhile additional frames we need to restart the transmission, so they can be send within the green
               phase. */
            if( (p_reg_etherc->Current_Host_Transmit_Descriptor == (uint32_t)ptIfCtrl->p_tx_descriptor_base) &&
                    (TX_DESCR_ADDR_NRT_FIRST <= ptIfCtrl->p_tx_descriptor_base->des3 ) &&
                    (TX_DESCR_ADDR_NRT_LAST  >= ptIfCtrl->p_tx_descriptor_base->des3 ) )        // Transmission not jet started or finished (wrapped around once)
			{
                uint8_t desc_valid = 0;   // Got a valif frame to be sent
   				p_reg_etherc->Transmit_Descriptor_List_Address_b.TDESLA_32bit = (uint32_t) ptIfCtrl->p_tx_descriptor_base >> 2;
                // Find first valid descriptor
                ether_instance_descriptor_t* p_tx_descriptor_down = LSA_NULL;
                p_tx_descriptor = ptIfCtrl->p_tx_descriptor;               // Pointer to first empty descriptor
                LSA_UINT8 CntDes = 0U;
                p_reg_etherc->Operation_Mode_b.ST = 0U;     /* Stop TX */
                // Find the first valid descriptor and restart from there
                do
                {
                    // Reduce by 1 with overflow handling
					if(p_tx_descriptor == &tx_descriptors[0])
					{
					      p_tx_descriptor_down = &tx_descriptors[(NUM_TX_DESCRIPTOR-1U)];   // Turover at Index 0
					      if ((p_tx_descriptor_down->des0) & ETHER_TDES0_OWN) {
					        p_tx_descriptor_down->des3 = (uint32_t) &tx_descriptors[0];   // Last desc. in row points back to first
					        p_tx_descriptor_down->des0 |= (ETHER_TDES0_TCH);  // Set Cain flag
					      }
					}
					else
					{
					      p_tx_descriptor_down = p_tx_descriptor-1;
					      if ((p_tx_descriptor_down->des0) & ETHER_TDES0_OWN) {
					        p_tx_descriptor_down->des3 = (uint32_t) p_tx_descriptor;  // Last descriptor points back to base after finished
					        p_tx_descriptor_down->des0 |= (ETHER_TDES0_TCH);  // Set Cain flag
					      }
					}
	                // Now check if this is a valid descriptor (contains data)
					if ((p_tx_descriptor_down->des0) & ETHER_TDES0_OWN)    // next over is used
					{
						p_tx_descriptor = p_tx_descriptor_down;
						if (CntDes == 0) {
							p_tx_descriptor_down->des0 |= (ETHER_TDES0_TCH);  // Set Cain flag
							p_tx_descriptor->des0 |= (ETHER_TDES0_IC);  // Set Interrupt flag for last frame in row
							p_tx_descriptor->des3 = (uint32_t) ptIfCtrl->p_tx_descriptor_base;  // Last descriptor points back to base after finished
						}
                        desc_valid = 1;
					}
					CntDes++;
					if (CntDes > (NUM_TX_DESCRIPTOR-1U)) {
						break;     // We can only have 4 descriptors, so we have now turned around
					}
                }
                while ((p_tx_descriptor_down-> des0) & ETHER_TDES0_OWN);
                if (desc_valid)
                {
					ptIfCtrl->p_tx_descriptor_base->des0 = (ETHER_TDES0_OWN | ETHER_TDES0_TCH);
					ptIfCtrl->p_tx_descriptor_base->des3 = (uint32_t)p_tx_descriptor;
					ptIfCtrl->Prev_Host_Transmit_Descriptor = 0U;
					p_reg_etherc->Transmit_Descriptor_List_Address_b.TDESLA_32bit = (uint32_t) ptIfCtrl->p_tx_descriptor_base >> 2;
					p_reg_etherc->Operation_Mode_b.ST = 1U;     /* (Re-)Start TX */
					__asm("dmb");
					p_reg_etherc->Transmit_Poll_Demand_b.TPD = 0;
				}
			}
			R_BSP_IrqEnableNoClear(ETHSW_TDMAOUT1_IRQn);
    	}
    }
    EDDP_SWI_TRACE_00(pHandle->TraceIdx,LSA_TRACE_LEVEL_NOTE_LOW, "OUT:EDDP_LL_SEND_TRIGGER()");
}


/**
 * \brief       helper function that compares to mac addresses
 *
 * \param[in]   mac1, mac2  mac addresses to be compared
 *
 * \retval      0           equal to the bool'ish false
 * \retval      1           equal to the bool'ish true
 */
static LSA_BOOL rzt2_compareMac( LSA_UINT8 *pMac1, LSA_UINT8 *pMac2)
{
    LSA_UINT8 result = LSA_FALSE;

    if( (LSA_NULL != pMac1)    &&
    	(LSA_NULL != pMac2)    &&
    	(pMac1[0] == pMac2[0]) &&
    	(pMac1[1] == pMac2[1]) &&
		(pMac1[2] == pMac2[2]) &&
		(pMac1[3] == pMac2[3]) &&
		(pMac1[4] == pMac2[4]) &&
		(pMac1[5] == pMac2[5]) )
    {
        result = LSA_TRUE;
    }

    return result;
}

LSA_UINT16 rzt2_LookupMac(EDD_MAC_ADR_PTR_TYPE ptTab, LSA_UINT16 Items, EDD_MAC_ADR_PTR_TYPE ptMac)
{
	LSA_UINT16 nIndex = 0xFFFF;
	LSA_UINT16 nCnt = 0;

	if(LSA_NULL != ptTab)
	{
		while( (0xFFFF == nIndex) && (nCnt < Items) )
		{
			if(LSA_TRUE == rzt2_compareMac(&ptTab[nCnt].MacAdr[0U], &ptMac->MacAdr[0U]))
			{
				nIndex = nCnt;
			}
			nCnt++;
		}
	}

	return nIndex;
}

LSA_UINT32 EDDP_LL_InitTxDescriptors(EDDP_LL_HANDLE_PTR_TYPE pHandle)
{
	LSA_UINT32  Status = EDD_STS_OK;
	ether_instance_t *ptIf = EDDP_LL_CAST_ETH(pHandle->pIfHandle);
	ether_instance_ctrl_t *ptIfCtrl = (ether_instance_ctrl_t*)ptIf->p_ctrl;
	volatile  R_GMAC_Type * p_reg_etherc = (R_GMAC_Type *) ptIfCtrl->p_reg_etherc;
	__attribute__((unused))LSA_UINT16  nCnt = 0U;

	OsMemSet(&g_tx_descriptor_base, 	 0, sizeof(g_tx_descriptor_base));
	OsMemSet(&g_tx_descriptors_rtc3[0U], 0, sizeof(g_tx_descriptors_rtc3));
	OsMemSet(&g_tx_descriptors_rtcx[0U], 0, sizeof(g_tx_descriptors_rtcx));

	ptIfCtrl->p_tx_descriptor_top = &tx_descriptors[(NUM_TX_DESCRIPTOR - 1U)];

	ptIfCtrl->p_tx_descriptor_base = &g_tx_descriptor_base;
	ptIfCtrl->p_tx_descriptor_rtc3 = &g_tx_descriptors_rtc3[0U];
	ptIfCtrl->p_tx_descriptor_rtcx = &g_tx_descriptors_rtcx[0U];    // The frames addresses by this desc. are sent as first in the green phase (i. e. RTC1,2)

	// Initialize base descriptor. This is basically empty, it just points the GMAC to the first descriptor to use, either red_descriptor or standard descriptor
	ptIfCtrl->p_tx_descriptor_base->des0 = (ETHER_TDES0_OWN | ETHER_TDES0_TCH);
	ptIfCtrl->p_tx_descriptor_base->des1 = 0U;  // As this is only a dummy descriptor Buffer empty, go to next descriptor
	ptIfCtrl->p_tx_descriptor_base->des2 = 0U;  // Address of buffer, as this is empty it is 0
	ptIfCtrl->p_tx_descriptor_base->des3 = (uint32_t) &g_tx_descriptors_nrt[0U];  // Address of next descriptor, now red , changed by interrupts

	/* Initialize Transmit Descriptor List Address */
	p_reg_etherc->Transmit_Descriptor_List_Address_b.TDESLA_32bit = (uint32_t) (((uint32_t)ptIfCtrl->p_tx_descriptor_base) >> 2U);

	return Status;
}

/**
 * \brief       lower layer api function that initializes the lower layer (interface)
 *
 * \param[in]   pLLManagement   lower layer handle
 * \param[in]   hDDB            edds handle (handle device description block)
 * \param[in]   pDPB            pointer to DPB (device parameter block)
 * \param[in]   TraceIdx        used Trace ID for this function/lower layer
 * \param[out]  pCaps           capabilities of the lower layer
 *
 * \note        see EDDP_LLIF.DOCX for more informations
 *
 * \retval      EDD_STS_OK              good
 * \retval      EDD_STS_ERR_HW          error with some reset...
 */
EDD_RSP EDDP_LL_SWIDeviceOpen(EDDP_LOCAL_DDB_PTR_TYPE pDDB, EDDP_UPPER_DPB_PTR_TYPE     pDPB)
{
	LSA_RESULT Status = EDD_STS_ERR_RESOURCE;
	EDDP_LL_HANDLE_PTR_TYPE pHandle = LSA_NULL;

	EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_LL_SWIDeviceOpen(pDDB=0x%X, pDPB=0x%X)", pDDB, pDPB);

    if( (LSA_NULL != pDDB) && (LSA_NULL != pDPB) &&
    	(1U == pDDB->InterfaceID) && (LSA_NULL == g_ptHandle) )
    {
    	pHandle =(EDDP_LL_HANDLE_PTR_TYPE) &g_tHandle;
    	g_ptHandle = pHandle;
    	OsMemSet(pHandle, 0, sizeof(EDDP_LL_HANDLE_TYPE));

		/* setup handle */
		pHandle->InterfaceID = pDPB->InterfaceID;
		pHandle->pIfHandle 	 = EDDP_LL_GetIfHandle(pDPB->InterfaceID);
		pHandle->hSysDev     = pDPB->hSysDev;
		pHandle->TraceIdx    = pDDB->TraceIdx;
		pHandle->hDPB 		 = pDPB;
		pHandle->hDDB        = pDDB;
		// pHandle->IfCtrlLock  = OS_MUTEX_EDDP_IFCTRL;
		OsAllocSemB(&pHandle->IfCtrlLock);
		OsAllocSemB(&pHandle->IfSendLock);
		//pHandle->hMailbox = OsCreateQue(20, sizeof(LSA_UINT32));

		LSAS_ALLOC_PACKET_MEM (&pHandle->pRxBuf, EDD_CFG_RX_FRAME_BUFFER_LENGTH, LSA_COMP_ID_EDDP);

		pHandle->hQK32Rsp = OsCreateQue(20, sizeof(LSA_UINT32));
		pHandle->hQCmdRsp = OsCreateQue(20, sizeof(LSA_UINT32));

		OsMemSet(&g_tRtcRxPktList[0U],   0, sizeof(g_tRtcRxPktList));
		OsMemSet(&g_tRtcTxPktList[0U],   0, sizeof(g_tRtcTxPktList));

		for (LSA_UINT32 ArNum = 0; ArNum < IOD_CFG_NUMOF_IO_AR; ArNum++)
		{
			ArInfo[ArNum].ConsumerId = EDDP_CRT_CHAIN_IDX_END;
			ArInfo[ArNum].ProviderId = EDDP_CRT_CHAIN_IDX_END;
		}

		if(PNIO_OK == OsCreateThread(EDDP_LL_CTRL_TASK, (PNIO_UINT8*)"EDDP_LL-IFCTRL", TASK_PRIO_EDDP_LL_CTRL, &pHandle->IfCtrlTaskId))
		{
			OsStartThread(pHandle->IfCtrlTaskId);
			OsTakeSemB(pHandle->IfCtrlLock);

			if(FSP_SUCCESS == EDDP_LL_ETH_OPEN(EDDP_LL_CAST_ETH(pHandle->pIfHandle)))
			{
                 EDDP_LL_InitTxDescriptors(pHandle);
				 pHandle->u16State |= EDDP_LLIF_STATE_OPEN;
				 Status = EDD_STS_OK;
			}

			OsGiveSemB(pHandle->IfCtrlLock);
			Status = EDD_STS_OK;
		}
    }
    else
    {
    	Status = EDD_STS_ERR_PARAM;
    }

	return Status;

}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_SWIDeviceSetup1                        +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             EDDP_UPPER_DSB_PTR_TYPE        pDSB         +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  pDSB       : Pointer to DeviceSetupBlock                         (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDDP_SRV_DEV_SETUP                                        +*/
/*+               Part 1 of Setup Device for SWI.                           +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_LL_SWIDeviceSetup1( EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
													 EDDP_UPPER_DSB_PTR_TYPE     pDSB)
{
    EDD_RSP             Response = EDD_STS_ERR_PARAM;
    EDDP_LL_HANDLE_PTR_TYPE pHandle = (EDDP_LL_HANDLE_PTR_TYPE)EDDP_LL_MNG_HANDLE(pDDB->InterfaceID);

    EDDP_SWI_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_LL_SWIDeviceSetup1(pDDB=0x%X, pDSB=0x%X)", pDDB, pDSB);

    if( (LSA_NULL != pDDB) && (LSA_NULL != pDSB) && (NULL != pHandle) && (LSA_NULL != pHandle->pIfHandle) &&
		(EDDP_LLIF_STATE_OPEN == pHandle->u16State) && (EDDP_LLIF_STATE_SETUP != pHandle->u16State) )
	{
    	LSA_UINT32          HwPortID, PortID;
    	EDDP_LL_PORTCTRL_PTR_TYPE ptPort;
     	ether_instance_t *ptEth = EDDP_LL_CAST_ETH(pHandle->pIfHandle);
		ether_instance_ctrl_t *ptIfCtrl = EDDP_LL_ETH_CTRL_PTR(pHandle->pIfHandle);
		volatile R_GMAC_Type *  p_reg_etherc = (R_GMAC_Type*) ptIfCtrl->p_reg_etherc;
		ether_switch_instance_ctrl_t *ptSwCtrl = (ether_switch_instance_ctrl_t*)ptEth->p_cfg->p_ether_switch_instance->p_ctrl;
		R_ETHSW_Type *ptEthSw = (R_ETHSW_Type*)(R_ETHSW_Type*) ptSwCtrl->p_reg_switch;

		LSA_UINT32                mac_h;
		LSA_UINT32                mac_l;
		ether_switch_cmd_t       switch_cmd;
		ether_switch_cmd_t          mac_cmd;     /* MAC table set command */
		ether_switch_mactab_entry_t mac_entry;   /* MAC table entry */
		LSA_UINT32                 latency=0;
		LSA_UINT8 MacAddr[6] = {0,0,0,0,0,0};


		//	ether_cfg_t *ptEthCfg = (ether_cfg_t*) g_ether0.p_cfg;
		//	ptEthCfg->multicast =  ETHER_MULTICAST_ENABLE;

		OsMemSet(&mac_cmd, 0, sizeof(mac_cmd));
		OsMemSet(&switch_cmd, 0, sizeof(mac_cmd));

		ptEthSw->MGMT_CONFIG = 0x43;  // @kochn: Set b6 (ENABLE) to forward BPDU frames only to MGMT port (3)

		switch_cmd.data.p_latency_egress =(uint32_t*) &latency;
		switch_cmd.flg_set = ETHER_SWITCH_FALSE;

		mac_cmd.data.p_mac_tab = &mac_entry;
		mac_cmd.cmd = ETHER_SWITCH_CMD_MACTAB_SET;
		mac_entry.priority = 0U;
		mac_entry.portmask = ( (1 << ETHER_SWITCH_PORT(0)) |
							   (1 << ETHER_SWITCH_PORT(1)) |
							   (1 << ETHER_SWITCH_PORT(2)) |
								   ETHER_SWITCH_PORT_HOST  |
								   ETHER_SWITCH_PORT_MASK);

		Bsp_GetMacAddr ((PNIO_UINT8*)&MacAddr[0]);
		OsMemCpy(&pHandle->MacAddrIF[0],&MacAddr[0], 6);
		ether_configure_mac(EDDP_LL_ETH_CTRL_PTR(pHandle->pIfHandle), &MacAddr[0U], ETHER_NO_USE_MAGIC_PACKET_DETECT);
		mac_entry.addr = (ether_switch_mac_addr_t *) &MacAddr[0U];
	//	ether_switch_mac_tab_set((R_ETHSW_Type*)ptSwCtrl->p_reg_switch, &mac_cmd);

		// Initialize MAC/PORT handling
		for ( PortID=1; PortID <= pDDB->SWI.PortCnt; PortID++ )
		{
			HwPortID = EDDP_SWI_get_HwPortID (pDDB, PortID);

			/* ----------------------------------------------------------------------*/
			/* Set up port MAC addresses                                             */
			/* ----------------------------------------------------------------------*/

			/* Set MAC address */
			mac_l = ( (LSA_UINT32)(pDDB->SWI.PortParams[HwPortID].Port_MAC_Address.MacAdr[0])       |
					  (LSA_UINT32)(pDDB->SWI.PortParams[HwPortID].Port_MAC_Address.MacAdr[1]) << 8  |
					  (LSA_UINT32)(pDDB->SWI.PortParams[HwPortID].Port_MAC_Address.MacAdr[2]) << 16 |
					  (LSA_UINT32)(pDDB->SWI.PortParams[HwPortID].Port_MAC_Address.MacAdr[3]) << 24  );

			mac_h = ( (LSA_UINT32)(pDDB->SWI.PortParams[HwPortID].Port_MAC_Address.MacAdr[4])      |
					  (LSA_UINT32)(pDDB->SWI.PortParams[HwPortID].Port_MAC_Address.MacAdr[5]) << 8  );


			switch(PortID)
			{
			case 1U:
				p_reg_etherc->MAR1_H_b.ADDRHI = mac_h;
				p_reg_etherc->MAR1_L_b.ADDRLO  = mac_l;
				break;
			case 2U:
				p_reg_etherc->MAR2_H_b.ADDRHI = mac_h;
				p_reg_etherc->MAR2_L_b.ADDRLO  = mac_l;
				break;
			case 3U:
				p_reg_etherc->MAR3_H_b.ADDRHI = mac_h;
				p_reg_etherc->MAR3_L_b.ADDRLO  = mac_l;
				break;
			case 4U:
				p_reg_etherc->MAR4_H_b.ADDRHI = mac_h;
				p_reg_etherc->MAR4_L_b.ADDRLO  = mac_l;
				break;
			default:
				break;
			}

		    ptPort = &pHandle->tPortCtrl[(HwPortID-1)];
			ptPort->PortID 			 			    = (HwPortID);
			ptPort->PhyPowerMode	 			    = EDDP_PHY_POWER_OFF;
			ptPort->LinkSpeedModeCfg 			    = EDD_LINK_DISABLED;//EDD_LINK_AUTONEG; /* default LinkStat is autoneg */
			ptPort->TxLatency			 			= 0U;
			ptPort->RxLatency			 			= 0U;
			ptPort->tLinkStat.Mode       			= EDD_LINK_UNKNOWN;
			ptPort->tLinkStat.Speed      			= EDD_LINK_UNKNOWN;
			ptPort->tLinkStat.Status     			= EDD_LINK_DOWN;
			ptPort->tLinkStat.Autoneg    			= EDD_AUTONEG_UNKNOWN;
			ptPort->tLinkStat.MAUType	 			= EDD_MAUTYPE_UNKNOWN;          /* MAU Type */
			ptPort->tLinkStat.MediaType	 			= EDD_MEDIATYPE_COPPER_CABLE;//EDD_MEDIATYPE_UNKNOWN;        /* Media Type */
			ptPort->tLinkStat.PortStatus 			= EDD_PORT_NOT_PRESENT;     		/* Status of Port */
			ptPort->tLinkStat.PhyStatus  			= EDD_PHY_STATUS_DISABLED; // EDD_PHY_STATUS_UNKNOWN;       /* PHYStatus of Port */
			ptPort->tLinkStat.IsPulled	 			= EDD_PORTMODULE_IS_PULLED;//EDD_PORTMODULE_IS_PLUGGED;    /* ChangeMediaTypeinRun value */
			ptPort->tLinkStat.PortState  			= EDD_PORT_STATE_DISABLE; //EDD_PORT_STATE_FORWARDING;    /* State  of Port */
			ptPort->tLinkStat.ChangeReason 			= EDD_LINK_CHANGE_REASON_NONE;
			ptPort->tLinkStat.AddCheckEnabled 		= EDD_LINK_ADD_CHECK_DISABLED;
			ptPort->tLinkStat.OpticalTransType 		= EDD_PORT_OPTICALTYPE_ISOTHER; /* is POF/SFP/OTHER */
			ptPort->tLinkStat.AutonegCapAdvertised 	= 0U;   /* autoneg advertised capabil. */
			ptPort->tLinkStat.LineDelay  			= 0U;   /* LineDelay set by EDD_SRV_SET_LINE_DELAY */
			ptPort->tLinkStat.CableDelayNs 			= 0U;   /* CableDel. set by EDD_SRV_SET_LINE_DELAY */
			ptPort->dwAutonegCap |= (EDD_AUTONEG_CAP_100BASETXFD | EDD_AUTONEG_CAP_100BASETX | EDD_AUTONEG_CAP_10BASETFD | EDD_AUTONEG_CAP_10BASET);

			mac_entry.addr = (ether_switch_mac_addr_t *) &pDDB->SWI.PortParams[HwPortID].Port_MAC_Address.MacAdr[0];
		//	ether_switch_mac_tab_set((R_ETHSW_Type*)ptSwCtrl->p_reg_switch, &mac_cmd);

			switch_cmd.port    = HwPortID;
			switch_cmd.cmd     = ETHER_SWITCH_CMD_LATENCY_EGRESS;
			if(FSP_SUCCESS == R_ETHER_SWITCH_Control ((ether_switch_ctrl_t *)ptSwCtrl, &switch_cmd) )
			{
				//ptPort->TxLatency = latency;
				latency=0U;
			}

			switch_cmd.cmd     = ETHER_SWITCH_CMD_LATENCY_INGRESS;
			if(FSP_SUCCESS == R_ETHER_SWITCH_Control ((ether_switch_ctrl_t *)ptSwCtrl, &switch_cmd) )
			{
				//ptPort->RxLatency = latency;
				latency=0U;
			}

		} //end for


		// set up FDB Control
	//	EDDP_SWI_fdb_setup (pDDB);

		// Initialize transceiver handling
		// Transceiver initialization can be done only now.
		// This ensures no entrys are generated in MAC Table till now.

		profinet_filter_conf();

	    /* Set up for Profinet usage. This should probably be put to the EDD ater, but for testing leave it here */
	    p_ptp_callback = ethSendEvt;
      
        // Put PHY in RESET as we have no config, will be released with open() 
        R_BSP_PinAccessEnable();
        /* Write Low-output to ESC_RESETOUT# as PHY reset */
#ifdef _RENESAS_RZT_
        R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) BSP_IO_PORT_20_PIN_7, (bsp_io_level_t)0);
#endif
#ifdef _RENESAS_RZN_
        R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) BSP_IO_PORT_13_PIN_4, (bsp_io_level_t)0);
#endif
        R_BSP_PinAccessDisable();
		/* Enable all PHYs (release reset) and set MDIO_HOLD */
		EDDP_LL_PHY_CTRL(pHandle, 1, EDDP_PHY_POWERUP, LSA_TRUE);
		EDDP_LL_PHY_CTRL(pHandle, 2, EDDP_PHY_POWERUP, LSA_TRUE);

#if IOD_INCLUDE_STORM_FILTER_ETHER_SW
		// Setup software filter
		EDDP_LL_EhterTypeFilterInit();
		EDDP_LL_AddEtherTypeFilter(EDDP_LL_SWFILTER_ETHTYPE_IPV4,
									EDDP_LL_SWFILTER_PERIOD_IPV4,
									EDDP_LL_SWFILTER_LIMIT_IPV4);
#endif /* IOD_INCLUDE_STORM_FILTER_ETHER_SW */

		pHandle->u16State |= EDDP_LLIF_STATE_SETUP;

	    /* Set up for Profinet usage. This should probably be put to the EDD ater, but for testing leave it here */
	    start_profinet(pDDB);

		Response = EDD_STS_OK;
		ether_switch_enable_tx_timestamp_icu (0x03);     // Enable tx timestamp interupt with portmsk 3
    }

    EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_LL_SWIDeviceSetup1(): Response=0x%X", Response);

    return (Response);
}
#if 0
/**
 * \brief           lower layer api function that stops the ethernet controller
 *
 * \param[in]       pLLManagement           lower layer handle
 *
 * \note            see EDDP_LLIF.DOCX for more informations
 *
 * \retval          EDD_STS_OK              good
 * \retval          EDD_STS_ERR_HW          reset error
 */
LSA_RESULT EDDP_LOCAL_FCT_ATTR EDDP_LL_SHUTDOWN(
        LSA_VOID_PTR_TYPE   pLLManagement)
{
    LSA_RESULT                  Status = EDD_STS_OK;
    EDDP_LL_HANDLE_PTR_TYPE   pHandle = (EDDP_LL_HANDLE_PTR_TYPE) pLLManagement;

    if( (NULL != pHandle) && (EDDP_LLIF_STATE_OPERATIONAL == pHandle->u16State) )
    {
		EDDP_SWI_TRACE_00(pHandle->TraceIdx,LSA_TRACE_LEVEL_CHAT, "IN :EDDP_LL_SHUTDOWN()");
		EDDP_LL_LOCK_ENTER(pHandle->IfCtrlLock);
#if 1
	//	EDDP_LL_ETH_SHUTDOWN(EDDP_LL_CAST_ETH(pHandle->pIfHandle));
#else
		if(FSP_SUCCESS == EDDP_LL_ETH_SHUTDOWN(EDDP_LL_CAST_ETH(pHandle->pIfHandle)))
		{
			pHandle->u16State &= (~EDDP_LLIF_STATE_SETUP);
		}
		else
		{
			Status = EDD_STS_ERR_RESOURCE;
		}
#endif
		EDDP_LL_LOCK_EXIT(pHandle->IfCtrlLock);

    }
    else
    {
    	if(NULL != pHandle)
    	{
    		Status = EDD_STS_ERR_SEQUENCE;
    	}
    	else
    	{
    		Status = EDD_STS_ERR_SERVICE;
    	}
    }

    EDDP_SWI_TRACE_00(pHandle->TraceIdx,LSA_TRACE_LEVEL_CHAT, "OUT:EDDP_LL_SHUTDOWN()");
    return Status;
}

/**
 * \brief           lower layer api function that closes the lower layer (interface) and frees allocated memory
 *
 * \param[in]       pLLManagement           lower layer handle
 *
 * \note            see EDDP_LLIF.DOCX for more informations
 *
 * \retval          EDD_STS_OK              good
 * \retval          EDD_STS_ERR_RESOURCE    any parameter (input) cause the error (see traces for details)
 */
LSA_RESULT EDDP_LOCAL_FCT_ATTR EDDP_LL_CLOSE(
        LSA_VOID_PTR_TYPE   pLLManagement)
{
	LSA_RESULT                  Status = EDD_STS_OK;
	EDDP_LL_HANDLE_PTR_TYPE   pHandle = (EDDP_LL_HANDLE_PTR_TYPE) pLLManagement;

    if( (NULL != pHandle) && (0U < (EDDP_LLIF_STATE_OPEN & pHandle->u16State)) )
    {
    	EDDP_LL_LOCK_ENTER(pHandle->IfCtrlLock);

    	EDDP_SWI_TRACE_00(pHandle->TraceIdx, LSA_TRACE_LEVEL_CHAT, "IN :EDDP_LL_CLOSE()");
    	if(FSP_SUCCESS == EDDP_LL_ETH_CLOSE(EDDP_LL_CAST_ETH(pHandle->pIfHandle)))
		{
			pHandle->u16State &= (~EDDP_LLIF_STATE_OPEN);
		}
		else
		{
			Status = EDD_STS_ERR_RESOURCE;
		}

    	EDDP_LL_LOCK_EXIT(pHandle->IfCtrlLock);

    	EDDP_SWI_TRACE_00(pHandle->TraceIdx,LSA_TRACE_LEVEL_CHAT, "OUT:EDDP_LL_CLOSE()");
    }
    else
    {
    	Status = EDD_STS_ERR_SERVICE;
    }
    return Status;
}
#endif

/**
 * \brief            function to trigge channel call back, indicates a PTCP frame send event to upper layer.
 *
	* \param[in]       p_timestamp: casted to EDD_RQB_NRT_SEND_TYPE, placed in queue for Rsync
 *									 port:        number of port, timestamp is for.
 *                   nanos:       actual timestamp (depature time) in nano seconds.
 *									 event:       EHER_SWITCH_PTP_CALLBACK_TS_VALID sends TS to Rsync, all other events are used to inicate that TS is unused.
 *
 * \retval           void
 */
LSA_VOID ethSendEvt(ether_switch_ptp_event_t event, uint8_t port, uint32_t nanos, ether_switch_timestamp_t * p_timestamp)
{	
	/* Rsync does not have a callback function */
	if(ETHER_SWITCH_PTP_CALLBACK_TS_VALID == event)
	{ //Rsync
		((struct timespec*)((EDD_RQB_NRT_SEND_TYPE*)p_timestamp)->hHDB)->tv_nsec = nanos;

		if(0 == OsQueSendFromISR(((EDD_RQB_NRT_SEND_TYPE*)p_timestamp)->hRQB, (PNIO_UINT32)p_timestamp))
		{
#ifdef RSyncDebug
			volatile int brk = 0;
#endif
		}
	}
}

/**
 * \brief           checks the target consumer of the RTC frame. 
 *
 * \param[in]       pConsumer                 address of pointer to consumer information
 * \param[in]       pRxHdr                    address of pointer to Receive Header
 * \param[in]       pRxDat                    address of pointer to Receive Data
 * \param[in]       RxDatLen                  Received data size
 * \param[in]       FirstPacket               First packet after AR establishment?
 *
 * \retval          PNIO_OK                   The consumer is the target.
 * \retval          PNIO_NOT_OK               The consumer is not the target.
 */
LSA_UINT32 EDDP_LL_ConsumerTargetCheck(EDDP_CRT_CONSUMER_PTR_TYPE pConsumer, LSA_UINT8* pRxHdr, LSA_UINT8* pRxDat, LSA_UINT32 RxDatLen, LSA_BOOL FirstPacket)
{
	LSA_UINT16 FrameID;
	LSA_UINT16 CycleCounterDiff;

	if (pConsumer->State != EDDP_CRT_STATE_HW_ACTIVE)
	{
		return PNIO_NOT_OK;
	}
	/** Check DataLen */
	if (pConsumer->Param.DataLen != (RxDatLen - (sizeof(FrameID) + sizeof(RTC_APDU))))
	{
		return PNIO_NOT_OK;
	}
	/** Check if the source MAC address of RTC frame matches the one of Consumer. */
	if ( 0 != memcmp( &pConsumer->Param.SrcMAC.MacAdr[0], &pRxHdr[6], 6 ) )
	{
		return PNIO_NOT_OK;
	}
	/** Check FrameID */
	FrameID = (LSA_UINT16)__REV16((LSA_UINT16) *((LSA_UINT16*)&pRxDat[0]));
	if (pConsumer->Param.FrameID != FrameID)
	{
		return PNIO_NOT_OK;
	}
	/** Check CycleCounter */
	if(LSA_FALSE == FirstPacket)
	{
		CycleCounterDiff = (LSA_UINT16)__REV16((LSA_UINT16) *((LSA_UINT16*)&pRxDat[RxDatLen - 4U/* CycleCounter Offset */]));
		CycleCounterDiff -= pConsumer->ConsAcwTypeInfo.pHwApduApi->CycleCounter;
		if ((CycleCounterDiff == 0) || (CycleCounterDiff > VAILD_CYCLECOUNTER_LIMIT))
		{
			return PNIO_NOT_OK;
		}
	}
	return PNIO_OK;
}

/**
 * \brief           checks the validity of the RTC frame. 
 *
 * \param[in]       pDDB                      Device-description block
 * \param[in]       pRxHdr                    address of pointer to Receive Header
 * \param[in]       pRxDat                    address of pointer to Receive Data
 * \param[in]       RxDatLen                  Received data size
 * \param[in]       FirstPacket               First packet after AR establishment?
 *
 * \retval          EDDP_CRT_CHAIN_IDX_END    Received consumer RTC frames are invalid.
 * \retval          Other Numbers             Received consumer RTC frames are valid.
 *                                            Valid pConsEntry entry number.
 */
LSA_UINT16 EDDP_LL_ConsumerFrameValidityCheck(EDDP_LOCAL_DDB_PTR_TYPE pDDB, LSA_UINT8* pRxHdr, LSA_UINT8* pRxDat, LSA_UINT32 RxDatLen, LSA_BOOL FirstPacket)
{
	EDDP_LL_HANDLE_PTR_TYPE    pHandle = (EDDP_LL_HANDLE_PTR_TYPE)EDDP_LL_MNG_HANDLE(1);
	EDDP_CRT_CONSUMER_PTR_TYPE pConsumer;

	/** Check target consumer */
	for (LSA_UINT16 ConsumerID = pDDB->CRT.Cons.ConsumerUsedList.BeginIdx; 
		ConsumerID != EDDP_CRT_CHAIN_IDX_END; 
		ConsumerID = pDDB->CRT.Cons.pConsEntry[ConsumerID].UsedFreeLink.Next)
	{
		pConsumer = &pDDB->CRT.Cons.pConsEntry[ConsumerID];

		if(PNIO_OK == EDDP_LL_ConsumerTargetCheck(pConsumer, pRxHdr, pRxDat, RxDatLen, FirstPacket))
		{
			LSA_UINT32 CSBValue = 0;
			if(PNIO_OK == EDDP_LL_ConsumerFrameApduCheck(pRxDat, RxDatLen, pConsumer, &CSBValue))
			{
				if(CSBValue) // DataStatus changed
				{
					EDDP_LL_SetCurrrentConsumerScoreboard(pDDB,ConsumerID,CSBValue);
					/** Generate the interrupt EDDP_INT_CSCOREBOARD_CHANGED_IFA */
					pHandle->dwEddpIrqMid |= (1UL << (EDDP_INT_CSCOREBOARD_CHANGED_IFA - EDDP_ISR_LOW_MAX_EVENTS));
				}
				return ConsumerID; // Matched Consumer
			}
		}
	}
	return EDDP_CRT_CHAIN_IDX_END; // No match found
}

/**
 * \brief           checks for DHT expiration.
 *
 * \param[in]       InterfaceID
 *
 * \retval          void
 */
LSA_VOID EDDP_LL_ConsumerDhtExpireCheck(LSA_UINT32 InterfaceID)
{
	EDDP_LL_HANDLE_PTR_TYPE    pHandle = (EDDP_LL_HANDLE_PTR_TYPE)EDDP_LL_MNG_HANDLE(InterfaceID);
	EDDP_LOCAL_DDB_PTR_TYPE    pDDB = (EDDP_LOCAL_DDB_PTR_TYPE) pHandle->hDDB;
	LSA_UINT16                 ConsumerID;
	ether_instance_t *ptEth = EDDP_LL_CAST_ETH(pHandle->pIfHandle);
	ether_switch_instance_ctrl_t *ptSwCtrl = (ether_switch_instance_ctrl_t*)ptEth->p_cfg->p_ether_switch_instance->p_ctrl;
	R_ETHSW_Type *ptEthSw = (R_ETHSW_Type*)(R_ETHSW_Type*) ptSwCtrl->p_reg_switch;
	LSA_UINT8 useTimer = ptEthSw->COMMAND_CONFIG_P3_b.TIMER_SEL;
	LSA_UINT32 localTdmaCounter;

	/** Get the head of consumer list. */
	ConsumerID = pDDB->CRT.Cons.ConsumerUsedList.BeginIdx;
	localTdmaCounter = g_Tdma_counter;
	while (ConsumerID != EDDP_CRT_CHAIN_IDX_END)
	{
		/** Check if the target consumer is subject to DHT expiration check */
		EDDP_CRT_CONSUMER_PTR_TYPE pConsumer = &pDDB->CRT.Cons.pConsEntry[ConsumerID];
		if(pConsumer->DHTExpireCheckEnable == LSA_TRUE && (localTdmaCounter % pConsumer->Param.CycleReductionRatio) == 0)
		{
			LSA_UINT32 dataHoldFactor;
			if( (FRAMEID_RTC3_START <= pConsumer->Param.FrameID) &&
				(FRAMEID_RTC3_END   >= pConsumer->Param.FrameID))
			{
				if((EDDP_LL_GetIRTActiveHostHwPortBits(pDDB) & EDDP_LL_GetIRTActiveRemoteHwPortBits(pDDB)) != 0)
				{
					dataHoldFactor = pConsumer->Param.DataHoldFactor;
				}
				else
				{
					dataHoldFactor = 256000000U / (g_SendClock * pConsumer->Param.CycleReductionRatio * 31250U);
					if(dataHoldFactor == 0)
					{
						dataHoldFactor = pConsumer->Param.DataHoldFactor;
					}
				}
			}
			else
			{
				if(pConsumer->FirstPacket)
				{
					dataHoldFactor = pConsumer->Param.DataHoldFactor;
				}
				else
				{
					dataHoldFactor = 256000000U / (g_SendClock * pConsumer->Param.CycleReductionRatio * 31250U);
					if(dataHoldFactor == 0)
					{
						dataHoldFactor = pConsumer->Param.DataHoldFactor;
					}
				}
			}
			if(pConsumer->DHTExpireCount < dataHoldFactor)
			{
				// Reload to 0 is done by receiving RTC frame(EDDP_LL_FilterFrame)
				OsIntDisable();
				pConsumer->DHTExpireCount++;
				OsIntEnable();
			}
			else
			{
				LSA_UINT32 ArIdx = EDDP_LL_GetArIdxfromConsumerId(ConsumerID);
				g_tRtcTxPktList[ArIdx].Status = EDD_STS_OK_PENDING; // Stop sending frames.

				// DHT expiration
				pConsumer->DHTExpireCheckEnable = LSA_FALSE;
				pConsumer->FirstPacket = LSA_FALSE;
				pConsumer->DHTExpireCount = 0;
				// Notification of DHT expiration
				LSA_UINT32 CSBValue = 0
				| PNIP_CPM_CSB_WORD1_DHT_EXPIRED_CHD_MASK	/// 0x00020000
				| PNIP_CPM_CSB_WORD1_FRM_RCVD_MASK			/// 0x00000100
				| PNIP_CPM_CSB_WORD1_RCVD_IN_DP2_MASK		/// 0x00000080
				| PNIP_CPM_CSB_WORD1_DS_PROBL_IND_MASK		/// 0x00000040
				| PNIP_CPM_CSB_WORD1_DS_RUNSTOP_MASK	   	/// 0x00000020
				| PNIP_CPM_CSB_WORD1_DS_STATE_MASK			/// 0x00000008
				| PNIP_CPM_CSB_WORD1_DHT_EXPIRED_MASK		/// 0x00000002
				;
				EDDP_LL_SetCurrrentConsumerScoreboard(pDDB,ConsumerID,CSBValue);

				/** Generate the interrupt EDDP_INT_CSCOREBOARD_CHANGED_IFA */
				PNIP_HAL_REG32_WRITE(PNIP_REG_IRQ2_MID_0, (1UL << (EDDP_INT_CSCOREBOARD_CHANGED_IFA - EDDP_ISR_LOW_MAX_EVENTS)));
				PNIP_HAL_REG32_WRITE(PNIP_REG_IRQ2EVENT_MID_0, (1UL << (EDDP_INT_CSCOREBOARD_CHANGED_IFA - EDDP_ISR_LOW_MAX_EVENTS)));
				eddp_interrupt(pDDB,EDDP_INT_SOURCE_GROUP);
			}
		}
		/** Update consumer index */
		ConsumerID = pConsumer->UsedFreeLink.Next;
	}
}

/**
 * \brief           set for current consmer scoreboard value.
 *
 * \param[in]       pDDB              Device-description block
 * \param[in]       ConsumerID        Consumer ID
 * \param[in]       ScoreboardValue   Scoreboard value
 *
 * \retval          void
 */
LSA_VOID EDDP_LL_SetCurrrentConsumerScoreboard(EDDP_LOCAL_DDB_PTR_TYPE pDDB,LSA_UINT16 ConsumerID,LSA_UINT32 ScoreboardValue)
{
	/** Get the index of current Scoreboard from PNIP */
	LSA_UINT32 CSBSelect = EDDP_HAL_REG32_READ(pDDB, pDDB->CRT.Cons.PNIPCSBControlReg) & PNIP_REG_CSB_CONTROL_IFA__MSK_CSB_SELECT;

	/** Select CSB depending on CSBSelect */
	PNIP_CPM_CSB_PTR_TYPE pCSB;
	if (CSBSelect == 0)
	{
		pCSB = pDDB->CRT.Cons.pHwBaseCSB0Api;
	}
	else
	{
		pCSB = pDDB->CRT.Cons.pHwBaseCSB1Api;
	}
	/** Set the CSB value to CSB  */
	EDDP_HAL_MEM32_WRITE(pDDB, &pCSB[ConsumerID], ScoreboardValue );
}


LSA_UINT32 EDDP_LL_ConsumerFrameApduCheck(LSA_UINT8* pRxBuffer, LSA_UINT32 RxLength, EDDP_CRT_CONSUMER_PTR_TYPE pConsumer, LSA_UINT32 *CSBValue)
{
    RTC_APDU_PTR pRxStat = LSA_NULL;
    LSA_UINT8 DataStatus = pConsumer->ConsAcwTypeInfo.pHwApduApi->DataStatus;
    LSA_UINT16 CycleCounter = pConsumer->ConsAcwTypeInfo.pHwApduApi->CycleCounter;
    PNIO_UINT32 Status = PNIO_OK;

    pRxStat = (RTC_APDU_PTR)&pRxBuffer[(RxLength - 4U)];


       if((pRxStat->data_status & (EDD_CSRT_DSTAT_BIT_DATA_VALID)) != (EDD_CSRT_DSTAT_BIT_DATA_VALID))
       {
          //invalide data state
          Status = PNIO_NOT_OK;
       }
       else if(pRxStat->transfer_status != 0)
       {
          //invalid transfer status
          Status = PNIO_NOT_OK;
       }
       else if(CycleCounter == pRxStat->cycle_counter)
       {
          //invalid cc
          Status = PNIO_NOT_OK;
       }
       else if((DataStatus & (EDD_CSRT_DSTAT_BIT_STATE|EDD_CSRT_DSTAT_BIT_STOP_RUN|EDD_CSRT_DSTAT_BIT_STATION_FAILURE|EDD_CSRT_DSTAT_BIT_REDUNDANCY))
               != (pRxStat->data_status &(EDD_CSRT_DSTAT_BIT_STATE|EDD_CSRT_DSTAT_BIT_STOP_RUN|EDD_CSRT_DSTAT_BIT_STATION_FAILURE)))
       {
           *CSBValue = 0
			| PNIP_CPM_CSB_WORD1_FRM_RCVD_MASK			/// 0x00000100
			| PNIP_CPM_CSB_WORD1_RCVD_IN_DP2_MASK		/// 0x00000080
			;

           if((pRxStat->data_status & EDD_CSRT_DSTAT_BIT_STATE) != (DataStatus & EDD_CSRT_DSTAT_BIT_STATE))
           {
              *CSBValue |= PNIP_CPM_CSB_WORD1_DS_STATE_CHD_MASK;
              if((pRxStat->data_status & EDD_CSRT_DSTAT_BIT_STATE))
              {
                *CSBValue |= PNIP_CPM_CSB_WORD1_DS_STATE_MASK; // primary
              }
           }

           if((pRxStat->data_status & EDD_CSRT_DSTAT_BIT_STOP_RUN) != (DataStatus & EDD_CSRT_DSTAT_BIT_STOP_RUN))
           {
              *CSBValue |= PNIP_CPM_CSB_WORD1_DS_RUNSTOP_CHD_MASK;
              if((pRxStat->data_status & EDD_CSRT_DSTAT_BIT_STOP_RUN))
              {
                *CSBValue |= PNIP_CPM_CSB_WORD1_DS_RUNSTOP_MASK; // run
              }
           }

           if((pRxStat->data_status & EDD_CSRT_DSTAT_BIT_STATION_FAILURE) != (DataStatus & EDD_CSRT_DSTAT_BIT_STATION_FAILURE))
           {
              *CSBValue |= PNIP_CPM_CSB_WORD1_DS_PROBL_IND_CHD_MASK;
              if((pRxStat->data_status & EDD_CSRT_DSTAT_BIT_STATION_FAILURE))
              {
                *CSBValue |= PNIP_CPM_CSB_WORD1_DS_PROBL_IND_MASK; // ok
              }
           }

           if((pRxStat->data_status & EDD_CSRT_DSTAT_BIT_REDUNDANCY) != (DataStatus & EDD_CSRT_DSTAT_BIT_REDUNDANCY))
           {
              *CSBValue |= PNIP_CPM_CSB_WORD1_DS_SYS_RED_CHD_MASK;
              if((pRxStat->data_status & EDD_CSRT_DSTAT_BIT_REDUNDANCY))
              {
                *CSBValue |= PNIP_CPM_CSB_WORD1_DS_SYS_RED_MASK; // redundancy exist
              }
           }

       }

    return Status;
}

#if IOD_INCLUDE_STORM_FILTER_DCP_SW
LSA_VOID EDDP_LL_SetStationNameInfo(LSA_UINT8 *pName,LSA_INT NameLen)
{
	if(pName == NULL)
	{
		g_StationNameLen = 0;
		g_StationNamePrt = NULL;
	}
	else
	{
		g_StationNamePrt = pName;
		g_StationNameLen = NameLen;
	}
}
#endif /* IOD_INCLUDE_STORM_FILTER_DCP_SW */

#if IOD_INCLUDE_STORM_FILTER_ETHER_SW
/**
 * \brief           InitInitialization of software filters.
 *
 * \param[in]       void
 *
 * \retval          void
 */
LSA_VOID EDDP_LL_EhterTypeFilterInit(LSA_VOID)
{
	if(LSA_TRUE == g_EtherTypeFilterInit)
	{
		return;
	}
	// Clear EtherType Filer Table
	OsMemSet(&g_EtherTypeFiletr,  0, sizeof(g_EtherTypeFiletr));
	OsAllocTimer(&g_EtherTypeFilterTimerID, LSA_TIMER_TYPE_CYCLIC , LSA_TIME_BASE_1MS , (PNIO_VOID*)EDDP_LL_EtherTypeFilterTimerCoallback);
	OsStartTimer(g_EtherTypeFilterTimerID,NULL,EDDP_LL_ETHER_TYPE_FILTER_TIMER);
	g_EtherTypeFilterInit = LSA_TRUE;
}

/**
 * \brief           Adding software filers for EtherType
 *
 * \param[in]       EtherType EtherType in the range of 0x0800 to 0xFFFF
 * \param[in]       Period Effective filter period in milliseconds(0 < n)
 * \param[in]       FrameLimit Upper limit to filter incoming frames
 *
 * \retval          EDD_STS_OK Successful addition of filter
 * \retval          EDD_STS_ERR_RESOURCE Filter table is full
 * \retval          EDD_STS_ERR_PARAM EtherType is less than 0x800
 */
EDD_RSP EDDP_LL_AddEtherTypeFilter(
    LSA_UINT16 EtherType,
    LSA_UINT16 Period,
    LSA_UINT32 FrameLimit)
{
	LSA_UINT16 FilterCount;
	EDDP_ETHER_TYPE_FILTER_PTR_TYPE pFilter;
	EDDP_ETHER_TYPE_FILTER_PTR_TYPE pEntryFilter;

	if(htons(ETH_TYPE_IPV4) > EtherType)
	{
		return EDD_STS_ERR_PARAM;
	}
	if(0 == Period)
	{
		return EDD_STS_ERR_PARAM;
	}
	pEntryFilter = NULL;
	for(FilterCount = 0, pFilter = &g_EtherTypeFiletr[0]; FilterCount < EDDP_LL_ETHER_TYPE_FILTER_MAX; FilterCount++,pFilter++)
	{
		if(0 == pFilter->EtherType && NULL == pEntryFilter)
		{
			pEntryFilter = pFilter;
		}
		else if(EtherType == pFilter->EtherType)
		{
			pEntryFilter = pFilter;
			break;
		}
	}
	if(NULL == pEntryFilter)
	{
		return EDD_STS_ERR_RESOURCE;
	}
	pEntryFilter->EtherType		= 0;	// Clear once in case it is referenced during filter update
	pEntryFilter->FilterPeriod	= Period;
	pEntryFilter->FrameLimit	= FrameLimit;
	pEntryFilter->FrameCounter	= 0;
	pEntryFilter->EtherType		= EtherType;

	return EDD_STS_OK;
}

/**
 * \brief           Delete  sofrware filters for EtherType
 *
 * \param[in]       EtherType EtherType in the range of 0x0800 to 0xFFFF
 *
 * \retval          EDD_STS_OK Successfully deleted filter
 * \retval          EDD_STS_ERR_RESOURCE No applicable filter settings for EtherType
 * \retval          EDD_STS_ERR_PARAM EtherType is less than 0x800
 */
EDD_RSP  EDDP_LL_DelEtherTypeFilter(LSA_UINT16 EtherType)
{
	LSA_UINT16 FilterCount;
	EDDP_ETHER_TYPE_FILTER_PTR_TYPE pFilter;
	if(htons(ETH_TYPE_IPV4) > EtherType)
	{
		return EDD_STS_ERR_PARAM;
	}
	for(FilterCount = 0, pFilter = &g_EtherTypeFiletr[0]; FilterCount < EDDP_LL_ETHER_TYPE_FILTER_MAX; FilterCount++,pFilter++)
	{
		if(EtherType == pFilter->EtherType)
		{
			pFilter->EtherType		= 0;
			pFilter->FilterPeriod	= 0;
			pFilter->FrameLimit		= 0;
			pFilter->FrameCounter	= 0;
			return EDD_STS_OK;
		}
	}

	return EDD_STS_ERR_RESOURCE;
}

/**
 * \brief           Determine if filtering is required
 *
 * \param[in]       EtherType EtherType in the range of 0x0000 to 0xFFFF
 *
 * \retval          EDD_STS_OK
 *                      Can accept frames of the specified EtherType.
 *                      The filter for the specified EtherType does not exist.
 *                      EtherType is less than 0x800.
 * \retval          EDD_STS_ERR_RESOURCE Can not accept frames of tghe specified EtherType
 */
EDD_RSP EDDP_LL_EtherTypeFilter(LSA_UINT16 EtherType)
{
	LSA_UINT16 FilterCount;
	EDDP_ETHER_TYPE_FILTER_PTR_TYPE pFilter;
	if(htons(ETH_TYPE_IPV4) > EtherType)
	{
		return EDD_STS_OK;
	}
	for(FilterCount = 0, pFilter = &g_EtherTypeFiletr[0]; FilterCount < EDDP_LL_ETHER_TYPE_FILTER_MAX; FilterCount++,pFilter++)
	{
		if(EtherType == pFilter->EtherType)
		{
			if(pFilter->FrameLimit > pFilter->FrameCounter)
			{
				pFilter->FrameCounter++;
				return EDD_STS_OK;
			}
			else
			{
				return EDD_STS_ERR_RESOURCE;
			}
		}
	}

	return EDD_STS_OK;
}

/**
 * \brief           Periodic reloading of counters for filtering
 *
 * \param[in]       timer_id Own timer id
 * \param[in]       user_id Not Use parameter
 *
 * \retval          void
 */
LSA_VOID EDDP_LL_EtherTypeFilterTimerCoallback(LSA_UINT16 timer_id,LSA_UINT32 user_id)
{
	LSA_UINT16 FilterCount;
	EDDP_ETHER_TYPE_FILTER_PTR_TYPE pFilter;
	LSA_UNUSED_ARG(timer_id);
	LSA_UNUSED_ARG(user_id);
	g_EtherTypeFilterCounter++;
	for(FilterCount = 0, pFilter = &g_EtherTypeFiletr[0]; FilterCount < EDDP_LL_ETHER_TYPE_FILTER_MAX; FilterCount++,pFilter++)
	{
		if(htons(ETH_TYPE_IPV4) <= pFilter->EtherType)
		{
			if((g_EtherTypeFilterCounter % pFilter->FilterPeriod) == 0)
			{
				pFilter->FrameCounter = 0;
			}
		}
	}
}
#endif /* IOD_INCLUDE_STORM_FILTER_ETHER_SW */

uint8_t early_green_port[16] = {0};     // Mask for the port, that needs to be switched to GREEN mode first. Not set when 0, otherwise either 01 or 02

void start_profinet(EDDP_LOCAL_DDB_PTR_TYPE pDDB)
{
    g_bProfinetSetup = 1U;

    // ETHSW_InitDescriptors();

    ETHSW_InitSwitchTimer();
    // Setup the scheduler
    setup_profinet(pDDB);

    return;
}

#define RTC3_RANGE 0x0FFF0100
// Delay it takes the hardware logic to trigger the pattern matcher delay function
#define PATTERN_TRIGGER_DLY_DLY 70
/*+  Description: This function is called after set up of     +*/
/*+               the IRT-Table and is also called                          +*/
/*+               every time GSY provides a new cable delay. This function  +*/
/*+               sets the register .                   +*/
/*+                                                                         +*/
/*+               If "RelativeTime-Mode"                                    +*/
/*+                     - calculate new IRT_DelayTime and set it            +*/
/*+                       to REG_IRT_DELAYTIME_Px                           +*/

void EDDP_Set_IRT_DelayTime(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	uint32_t                  PortID)
{
    R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
	  extern struct gPTPd gPTPd[NUM_PORTS];
    uint32_t          IRT_DelayTime;                // Total bridge delay time
    uint32_t          PRM_MaxLineRxDelay;
    __attribute__((unused))uint32_t          PRM_MaxBridgeDelay;
    uint32_t          CableRxDelay, LineRxDelay, RemoteTxDelay;
    uint32_t          HwPortID;

    IRT_DelayTime       = 0;
    HwPortID            = EDDP_SWI_get_HwPortID(pDDB, PortID);
    CableRxDelay         = pDDB->SWI.LinkStatus[PortID].CableDelayInNsMeasured * (EDDP_TIME_TICKS_RESOLUTION_NS);
    PRM_MaxLineRxDelay  = pDDB->SWI.LinkStatus[PortID].PRM_MaxLineRxDelay;
    PRM_MaxBridgeDelay  = pDDB->SWI.PRM_MaxBridgeDelay;
    uint32_t port_index = PortID-1;

    RemoteTxDelay = gPTPd[port_index].remoteTXDelay;
    LineRxDelay = CableRxDelay + pDDB->SWI.LinkStatus[PortID].RealPortRxDelay + RemoteTxDelay;

    /* ------------------------------------------------------------------------------------ */
    /* IRT_DelayTime = MaxBridgeDelay + (MaxLineRxDelay ・LineRxDelay) - Compensation       */
    /* ------------------------------------------------------------------------------------ */

    /* ------------------------------------------------------------------------------------ */
    /* set the new DelayTime to IRT_DelayTime_Px register only if                           */
    /*  - there is a valid MaxLineRxDelay from PRM                                          */
    /*  - there is a valid LineRxDelay from GSY                                             */
    /* ------------------------------------------------------------------------------------ */
    // check (PRM_MaxLineRxDelay != 0x0) because it is possibly
    // that the PRM_MaxLineRxDelay is not present for port without RED phase

    if ((PRM_MaxLineRxDelay != 0x0) && (CableRxDelay != 0x0) && (RemoteTxDelay != 0x0))
    {
        if (LineRxDelay < PRM_MaxLineRxDelay)
        {
            IRT_DelayTime =  (PRM_MaxLineRxDelay - LineRxDelay) - EDDP_SWI_IRT_DELAYTIME_COMPENSATION + 250;
        }
    }
    else
    {
      IRT_DelayTime = 0;
    }

    if (HwPortID == 2)
    {
     // IRT_DelayTime = 20000;  // For testing
    }
    // The Bridge Delay of the MTIP switch is 3960 at a CT_DELAY value of 32, each increment adds 40ns ?  to the delay.
    uint32_t ct_delay_new = 32 + IRT_DelayTime/80;
    if (IRT_DelayTime > (2560 + PATTERN_TRIGGER_DLY_DLY))
    {
      ct_delay_new = 32;
      // Set up special pattern matcher. This will override the normal queue selection pattern matcher by having a higher priority
        p_reg_ethsw->MMCTL_DLY_QTRIGGER_CTRL = IRT_DelayTime - 2560 - PATTERN_TRIGGER_DLY_DLY;   // Set delay time subtracting handling delays
        uint32_t    action      = MODE_2BYTE_RANGE | MATCH_LT | MATCH_RED | VLAN_SKIP | LEARNING_DIS | SET_PRIO | SWAP_BYTES | IMC_TRIGGER_DLY;
        uint32_t    max_min     = RTC3_RANGE;   // Find Frames with Frame ID
        uint32_t    length_type = 0x8892;       //  Profinet
        uint8_t     priority    = 4;                // Extra queue for RTC3 frames from external
        uint8_t     offset      = 2;

        uint32_t    ptrn_cmp[8] = {         0,0,0,0,0,0,0,0};
        uint32_t    ptrn_msk[8] = {0x0000ffff,0,0,0,0,0,0,0};

        uint8_t     port_mask   = HwPortID;
        uint8_t     port_forward_mask = 0x03 & (~(HwPortID & 0x03));  // Trigger for opposite port
        uint16_t    and_mask    = 0;
        uint8_t     queuesel    = 0;
        uint8_t timer = 0;

        ethsw_setup_mqprio_pattern_add (port_mask, PATTERN_IRT_SET_QUEUE_DLAY, action, priority, port_forward_mask, &ptrn_cmp[0], &ptrn_msk[0],
                                            length_type, offset, and_mask,  max_min, timer , queuesel );
								p_reg_ethsw->MMCTL_QGATE  = 0x02000000 | port_forward_mask; // Queue4 Close
    }
    else
    {
        ethsw_disable_pattern_matcher(HwPortID, PATTERN_IRT_SET_QUEUE_DLAY_MASK);
    }

    if (HwPortID == 1)
    {
      p_reg_ethsw->CT_DELAY_P0 = ct_delay_new;
    }
    if (HwPortID == 2)
    {
      p_reg_ethsw->CT_DELAY_P1 = ct_delay_new;
    }
}


static struct taprio_sched_entries entries[256];       // Max. 16 Phases with 8 entries each
                                                      // For debug here, move 10 lines down when not in debug
void setup_profinet(EDDP_LOCAL_DDB_PTR_TYPE pDDB)
{
    R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
    EDDP_PRM_PDIRDATA_PTR_TYPE      pPDIRData;
    uint8_t     port_mask   = 0x03; // Enable for external ports, frames to and from the management port 3 are not handled
    __attribute__((unused))uint8_t     pattern_num = 0;    // Pattern number
    uint8_t     enable      = 1;
    uint8_t     num_entries;
    uint8_t     portmask;
    uint8_t     init_setup = 0;
    uint8_t     timer       = ETHSW_TIMER0;

    if (0U == g_bProfinetSetup)      // Needs to be 1 indicating that the timer was elready set up
    {
        return;
    }

    if (pDDB == 0)
    {
      pPDIRData = 0;
    }
    else
    {
      pPDIRData   = pDDB->PRM.pPDIRData_A;
    }
    if (p_reg_ethsw->TDMA_CONFIG_b.TDMA_ENA == 0)
    {
        init_setup = 1;
       //Axel ETHSW_DisableTimer(timer);      // Disable Timer-0
        ETHSW_DisableIrqTdmaAll();  // Disable all TDMA interuupt

        p_reg_ethsw->UCAST_DEFAULT_MASK0 = 0x03;   // Unicast default mask only for external ports. unicast frames that have no learned target are dropped to avoid being flooded by wrong RTC3 frames.
        p_reg_ethsw->MODE_CONFIG = 0x0f00;   // Generally Enable Cut Through for all ports, needed to use the CT table for frame preemption octet count
     p_reg_ethsw-> IALK_CONTROL = 0x0f << 16;
        //p_reg_ethsw->MMCTL_CTFL_P0_3_ENA = 0x00303;  // Enable Cut Through Frame length feature for quwuw 0 and 1
        p_reg_ethsw->MMCTL_CTFL_P0_3_ENA = 0x00f0f;  // Enable Cut Through Frame length feature for quwuw 0 and 1
        // Program Cut through delay to keep the delay constant (factor of MII clock duration)
        p_reg_ethsw->CT_DELAY_P0 = 32;
        p_reg_ethsw->CT_DELAY_P1 = 32;
        p_reg_ethsw->CT_DELAY_P2 = 32;
        p_reg_ethsw->PRIORITY_TYPE1 = 0x00038892;   // Match Profinet frames to default priority 1. RTC3 frames are then handled specially later
        p_reg_ethsw->PRIORITY_TYPE2 = 0x000388f7;   // Match IEEE1588 PTP frames to ppriority 1

        #if 0
        {
            p_reg_ethsw->VLAN_PRIORITY0 = 0x0; //000249000; // Remapp VLAN prio to internal queues 0 and 1,
            p_reg_ethsw->VLAN_PRIORITY1 = 0x0; // 000249000; //      (0 0 3 3 3 3 0 0) => but look like (0 0 4 4 4 4 0 0) ?
            p_reg_ethsw->VLAN_PRIORITY2 = 0x0; //00249000;
            p_reg_ethsw->VLAN_PRIORITY3 = 0x0; //00249000;

            p_reg_ethsw->PRIORITY_CFG0 = 0x09;    // Eable VLAN priority resolution and type based priority resolution ,
            p_reg_ethsw->PRIORITY_CFG1 = 0x09;
            p_reg_ethsw->PRIORITY_CFG2 = 0x09;
            p_reg_ethsw->PRIORITY_CFG3 = 0x08;    // No VLAN for management port
        }
        #endif
       setup_pattern_matcher(0x0);     // Setup the patternmatcher but with portmask 0 to not enable it yet (is done after sync is done).
    }      // reset == 1

    #define TDMA_MAX_BACKTOBACK  177
    #define TX 0x01
    #define RX 0x02
    #define FSO_TYPE 0x04
    LSA_UINT32 cycle_time = pDDB->SWI.CycleInfo.CycleBaseFactor * 31250;
    uint8_t NumPhases = 1;     // Currently for Port[0] as we assume that all ports have the same phases
    uint8_t reset = 1;
    uint16_t index = 0;
    struct tcv_vector_t {
      uint32_t time;     // time, when this vector is triggered
      uint8_t port;      // number for respective port
      uint8_t status;     // New action/status for this activity
    };
    struct tcv_vector_t tcv_vector[6];
    if ( (pPDIRData == 0) || !(EDDP_PRMPDIRDataIsPDIRDataPresent(pDDB, LSA_TRUE)) )    // Flash is empty or invalid    {
    {
      portmask = 0x03;
	    // RED2 phase, trigger transmission of green frames
	    entries[index].gate_mask     = 0x03;        // enable Queue 3 for RTC3 frames from other devices
	    entries[index].port_mask     = portmask;    // Port Mask with one-hot encoding
	    entries[index].hold_req      = 0;           // One bit enable Hold request for frame preemption ,
	    entries[index].trigger_mode  = 0;           // One bit enable trigger mode ,
	    entries[index].gate_mode     = 1;           // One bit enable gate mode instead of trigger mode , only on newer devices,
	    entries[index].red_phase     = 0;           // Indicate Red phase for Profinet IRT
	    entries[index].in_ct_enable  = 0;           // One bit enable cut through ,
	    entries[index].out_ct_enable = 0;           // One bit enable cut through ,
	    entries[index].interval      = 0;           // offset of schedule entry related to start of cycle
	    entries[index].inc_0         = 1;           // Counter o
	    entries[index].inc_1         = 0;           // Counter 1
	    entries[index].gpio          = 0x12;        // Enable GPIO 1 to trigger transmission of green frames and GPIO4 for external PPS signal

		index++;

	     // green phase
	    entries[index].gate_mask     = 0x03;        // Gate Mask with one-hot encoding
	    entries[index].port_mask     = portmask;    // Port Mask with one-hot encoding
	    entries[index].hold_req      = 0;           // One bit enable Hold request for frame preemption ,
	    entries[index].trigger_mode  = 0;           // One bit enable trigger mode ,
	    entries[index].gate_mode     = 1;           // One bit enable gate mode instead of trigger mode , only on newer devices,
	    entries[index].red_phase     = 0;           // Indicate Red phase for Profinet IRT
	    entries[index].in_ct_enable  = 0;           // One bit enable cut through ,
	    entries[index].out_ct_enable = 0;           // One bit enable cut through ,
	    entries[index].interval      = 100000; // Offset of schedule entry in ns  entry related to start of cycle
	    entries[index].inc_0         = 0;           // Counter o
	    entries[index].inc_1         = 0;           // Counter 1
	    entries[index].gpio          = 0x00;        // Enable GPIO 1 to trigger transmission of green frames

		index++;
		// yellow phase
		entries[index].gate_mask     = 0x03;        // Gate Mask with one-hot encoding
		entries[index].port_mask     = portmask;    // Port Mask with one-hot encoding
		entries[index].hold_req      = 0;           // One bit enable Hold request for frame preemption ,
		entries[index].trigger_mode  = 0;           // One bit enable trigger mode ,
		entries[index].gate_mode     = 1;           // One bit enable gate mode instead of trigger mode , only on newer devices,
		entries[index].red_phase     = 0;           // Indicate Red phase for Profinet IRT
		entries[index].in_ct_enable  = 0;           // One bit enable cut through ,
		entries[index].out_ct_enable = 0;           // One bit enable cut through ,
		entries[index].interval      = cycle_time - 125000;   // Offset of schedule entry in ns  entry related to start of cycle
		entries[index].inc_0         = 0;           // Counter o
		entries[index].inc_1         = 0;           // Counter 1
		entries[index].gpio          = 0x04;        // Enable GPIO 2 to trigger transmission of yellow frames

		index++;

        // Entry is the same as above, trigger interrupt for transission of local RTC3 to GMAC
		entries[index].gate_mask     = 0x03;        // Enable Queues 2 and 3 for local RTC3 frames and others
		entries[index].port_mask     = portmask;    // Port Mask with one-hot encoding
		entries[index].hold_req      = 0;           // One bit enable Hold request for frame preemption ,
		entries[index].trigger_mode  = 0;           // One bit enable trigger mode ,
		entries[index].gate_mode     = 1;           // One bit enable gate mode instead of trigger mode , only on newer devices,
		entries[index].red_phase     = 0;           // Indicate Red phase for Profinet IRT
		entries[index].in_ct_enable  = 0;           // One bit enable cut through , must be set to make preemption work
		entries[index].out_ct_enable = 0;           // One bit enable cut through ,
		entries[index].interval      = cycle_time - GMAC_DELAY; // offset of schedule entry in ns
		entries[index].inc_0         = 0;           // Counter 0
		entries[index].inc_1         = 0;           // Counter 1
		entries[index].gpio          = 0;           // GPIO 0 enabled to trigger frame transmission interrupt
                index++;
    p_reg_ethsw->TDMA_CTR1 = 0x1 << 16;   // Set overflow for TDMA counter 1 to 1
#ifdef PrintfDebug
    printeth("TDMA INIT set");
#endif
  }
  else
  {
    // Calculate the times for the green periods for each port. These must be sorted so they are set up in the correct order. Invalid values (single port) are 0xffffffff
    __attribute__((unused))LSA_UINT32  HwPortID, phase_repeat;
    __attribute__((unused))uint32_t* fso_start_p;
    __attribute__((unused))uint32_t* green_start_tx_p;
    __attribute__((unused))uint32_t* green_start_rx_p;
    uint32_t yellow_start;
    __attribute__((unused))uint8_t enable_preemption;
    uint8_t num_tx_actions = 0;        //
    uint8_t num_rx_actions = 0;        //
    __attribute__((unused))uint8_t num_fso_actions = 0;        //
    __attribute__((unused))LSA_UINT32 red_start = 0;   //
    LSA_UINT32 green_start_rx[2] = {0x0ffffffff, 0x0ffffffff};   // Start time for green phase for RX on ports [index] as fetched from parameter settings
    LSA_UINT32 green_start_tx[2] = {0x0ffffffff, 0x0ffffffff};   // Start time for green phase for TX on ports [index] as fetched from parameter settings
    LSA_UINT32 fso[2] = {0, 0};     // If 0, no fso defined, fso for 2nd AR it available, index is for first and second frame
    LSA_UINT32 fso_port[2] = {0, 0};     // Portnumber for this Frame
    uint8_t noredtx[16];       // No red in this phase, coded as 01 for TX port 0, 0x02 for TX port 1 0x03 for both,
    uint8_t noredrx[16];       // No red in this phase, coded as 01 for RX port 0, 0x02 for RX port 1 0x03 for both,
    uint8_t singlephase = 0;     // Mask for eacuh port, if 1 this port has only a single phase with GreenStart at 0ns (NRT Port)

#if 0
    // For testing fill the config with some dummy data
    // Set up assignments (i. e. timing sets for a port and a config)
    pPDIRData->PDIRBeginEndData.BeginEndDataArray[0].NumberOfPhases = 16; 
    pPDIRData->PDIRBeginEndData.BeginEndDataArray[1].NumberOfPhases = 16; 
    pPDIRData->pIRFrameDataArray[1].ReductionRatio = 8; 
    pPDIRData->pIRFrameDataArray[1].FrameSendOffset = 43692;
          uint8_t port = 0; 
          uint32_t assi = 0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxRedOrangePeriodBegin = 0;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxOrangePeriodBegin = 74148;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxGreenPeriodBegin = 74148; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxRedOrangePeriodBegin = 0;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxOrangePeriodBegin = 72605;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxGreenPeriodBegin = 72605; 
          port = 0; 
          assi = 1; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxRedOrangePeriodBegin = 0;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxOrangePeriodBegin = 59727;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxGreenPeriodBegin = 59727; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxRedOrangePeriodBegin = 0;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxOrangePeriodBegin = 65725;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxGreenPeriodBegin = 65725; 
          port = 0; 
          assi = 2; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxRedOrangePeriodBegin = 0;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxOrangePeriodBegin = 50113;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxGreenPeriodBegin = 50113; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxRedOrangePeriodBegin = 0;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxOrangePeriodBegin = 65725;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxGreenPeriodBegin = 65725; 
          port = 0; 
          assi = 3; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxRedOrangePeriodBegin = 0;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxOrangePeriodBegin = 45306;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxGreenPeriodBegin = 45306; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxRedOrangePeriodBegin = 0;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxOrangePeriodBegin = 65725;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxGreenPeriodBegin = 65725; 
          port = 0; 
          assi = 4; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxRedOrangePeriodBegin = 0;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxOrangePeriodBegin = 0;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxGreenPeriodBegin = 0;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxRedOrangePeriodBegin = 0;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxOrangePeriodBegin = 0;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxGreenPeriodBegin = 0;
          // Set up phases (assign an assignment to each phase, this is separate for red, orange, green  )
          uint8_t phase = 0; 
          port = 0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0111; 
          phase = 1; 
          port = 0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0; 
          phase = 2; 
          port = 0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0; 
          phase = 3; 
          port = 0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0111; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0; 
          phase = 4; 
          port = 0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0111; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0111; 
          phase = 5; 
          port = 0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0333; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0; 
          phase = 6; 
          port = 0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0333; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0; 
          phase = 7; 
          port = 0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0222; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0; 
          phase = 8; 
          port = 0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0333; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0111; 
          phase = 9; 
          port = 0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0333; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0; 
          phase = 10; 
          port = 0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0333; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0; 
          phase = 11; 
          port = 0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0222; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0111; 
          phase = 12; 
          port = 0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0333; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0111; 
          phase = 13; 
          port = 0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0333; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0111; 
          phase = 14; 
          port = 0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0333; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0222; 
          phase = 15; 
          port = 0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0333; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0333; 
          phase = 0; 
          port = 1; 
          assi = 0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxRedOrangePeriodBegin = 0;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxOrangePeriodBegin = 78365;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxGreenPeriodBegin = 78365; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxRedOrangePeriodBegin = 0;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxOrangePeriodBegin = 68388;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxGreenPeriodBegin = 68388; 
          assi = 1; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxRedOrangePeriodBegin = 0;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxOrangePeriodBegin = 71485;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxGreenPeriodBegin = 71485; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxRedOrangePeriodBegin = 0;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxOrangePeriodBegin = 58774;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxGreenPeriodBegin = 58774; 
          assi = 2; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxRedOrangePeriodBegin = 0;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxOrangePeriodBegin = 71485;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxGreenPeriodBegin = 71485; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxRedOrangePeriodBegin = 0;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxOrangePeriodBegin = 49160;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxGreenPeriodBegin = 49160; 
          assi = 3; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxRedOrangePeriodBegin = 0;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxOrangePeriodBegin = 71485;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxGreenPeriodBegin = 71485; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxRedOrangePeriodBegin = 0;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxOrangePeriodBegin = 39546;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxGreenPeriodBegin = 39546; 
          assi = 4; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxRedOrangePeriodBegin = 0;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxOrangePeriodBegin = 0;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxGreenPeriodBegin = 0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxRedOrangePeriodBegin = 0;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxOrangePeriodBegin = 0;
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxGreenPeriodBegin = 0; 
          // Set up phases (assign an assignment to each phase, this is separate for red, orange, green  )
          phase = 0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0111; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0; 
          phase = 1; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0111; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0; 
          phase = 2; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0111; 
          phase = 3; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0111; 
          phase = 4; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0111; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0222; 
          phase = 5; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0333; 
          phase = 6; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0333; 
          phase = 7; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0222; 
          phase = 8; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0111; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0333; 
          phase = 9; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0111; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0333; 
          phase = 10; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0333; 
          phase = 11; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0111; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0333; 
          phase = 12; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0111; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0333; 
          phase = 13; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0111; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0333; 
          phase = 14; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0222; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0333; 
          phase = 15; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment = 0x0333; 
          pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment = 0x0333; 


#endif


    if (pPDIRData->PDIRBeginEndData.BeginEndDataArray[0].NumberOfPhases > pPDIRData->PDIRBeginEndData.BeginEndDataArray[1].NumberOfPhases)
    {
      NumPhases = pPDIRData->PDIRBeginEndData.BeginEndDataArray[0].NumberOfPhases;     //
    }
    else
    {
      NumPhases = pPDIRData->PDIRBeginEndData.BeginEndDataArray[1].NumberOfPhases;     //
    }

/*  Now set up the TDMA controller:
    There are up to EDDP_PRM_MAX_PHASES (16) Phases and up to EDDP_PRM_MAX_ASSIGNMENTS (5) assignments.
    Each Assignment contains timings for a phase, i. e. RedOrangePeriodBegion, OrangePeriodBegin, GreenPeriodBeggin each for TX and RX.
    Each phase is assigned one of the 5 assignments i. e. Phase j is assigned to TX Assignment n and Rx Assignement m.
    Thus there can be 16 phases, but only 5 different timings.
    All this is done for each port.
    The TDMA controller is set up in a way, that all 16 Phases are handled one after the other, after all 16 phases are handled the cycle starts from the start.
    The setup is handling as follows:
    1. Circle through the phases:
        Fetch all timing for the respective phase, i. e. start of green, start of yellow, FSO, (start of redorange ios always 0) for both ports.
        Sort the data in the correct time order. If multiple actions are required at the same time they are stored in different phases with the same time.
        set the data for TDMA controller.
    2. Configure the TDMA controller.

    INC_CTR1 is used to count the phases. It is reset to 0 at the start and then is counted up at the end of each phase.
    Open: When is the phase set to 0 / started ?
    Note: The number pf phases is always the same for both ports with the exception that one port is not used for IRT (i. e. RT, NRT, other) when there is only a single phase for this port.
*/



    for (uint8_t i=0; i < pPDIRData->PDIRBeginEndData.NumberOfPorts; i++ )   // circle through ports
    {
        HwPortID = EDDP_SWI_get_HwPortID(pDDB,i+1);
                if (   (pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].NumberOfAssignments == 1)   // i= portnumber
            && (pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[0].TxRedOrangePeriodBegin == 0x0)
            && (pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[0].TxOrangePeriodBegin == 0x0)
            && (pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[0].TxGreenPeriodBegin == 0x0)
            && (pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[0].RxRedOrangePeriodBegin == 0x0)
            && (pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[0].RxOrangePeriodBegin == 0x0)
            && (pPDIRData->PDIRBeginEndData.BeginEndDataArray[i].Assignments[0].RxGreenPeriodBegin == 0x0)
           )
        {
            // if this is a "green-only" port the IRT port status remains at EDD_IRT_NOT_SUPPORTED
            pDDB->SWI.LinkStatus[i+1].IRTPortStatus = EDD_IRT_NOT_SUPPORTED;
        }
        else
        {
            pDDB->SWI.LinkStatus[i+1].IRTPortStatus = EDD_IRT_PORT_INACTIVE;
        }

        pDDB->SWI.LinkStatus[i+1].RsvIntervalRed.Rx.EndLocalNs          = pPDIRData->LastLocalIRTFrameArray[i].RxEndLocalNs;
        pDDB->SWI.LinkStatus[i+1].RsvIntervalRed.Rx.LastLocalFrameLen   = pPDIRData->LastLocalIRTFrameArray[i].RxLastLocalFrameLen;
        pDDB->SWI.LinkStatus[i+1].RsvIntervalRed.Tx.EndLocalNs          = pPDIRData->LastLocalIRTFrameArray[i].TxEndLocalNs;
        pDDB->SWI.LinkStatus[i+1].RsvIntervalRed.Tx.LastLocalFrameLen   = pPDIRData->LastLocalIRTFrameArray[i].TxLastLocalFrameLen;
        // store new DelayValues (MaxPortTxDelay, MaxPortRxDelay, MaxLineRxDelay) to global DDB parameters
        pDDB->SWI.LinkStatus[i].PRM_MaxLineRxDelay = pPDIRData->PDIRGlobalData.DelayValues[i-1].MaxLineRxDelay;
     // Set up dthe bridge delay for both ports.
        pDDB->SWI.PRM_MaxBridgeDelay = pPDIRData->PDIRGlobalData.MaxBridgeDelay;

        EDDP_Set_IRT_DelayTime(pDDB,i+1);
    }
    //uint8_t index = 0;     // Counter for TCV of TDMA Controller
    __attribute__((unused))uint32_t lasttime = 0;    // Time of previous action

    // Fetch the Provider Frame data
    EDDP_PRM_PDIRFRAMEDATA_PTR_TYPE  pFrameDataArray[2];     // Pointer to list with RTC3 Frame descriptions, currently only one provider supported
    uint8_t PrvCnt = 0;
    uint8_t Frame_Used = pPDIRData->IRFrameDataArray_Count_Used;
    for (uint8_t FrmCnt = 0; FrmCnt < Frame_Used; FrmCnt++)
    {
      if (pPDIRData->pIRFrameDataArray[FrmCnt].SndRcvRole == EDDP_PRM_PDIRFRAMEDATA_ROLE_PROVIDER)
      {
        pFrameDataArray[PrvCnt] = &pPDIRData->pIRFrameDataArray[FrmCnt];
        PrvCnt++;
        if (PrvCnt > 1) break;
      }
    }
    // Set up the phase table
    uint32_t PhaseIndex = 0;     // A 1 at a certain position indicates a phase in which we send a RTC3 frame,
    uint8_t phase_inc = pFrameDataArray[0]->ReductionRatio;
    PhaseIndex |= (1 <<  (pFrameDataArray[0]->Phase-1));
    uint8_t phase_cnt = pFrameDataArray[0]->Phase - 1;
    while (phase_cnt < NumPhases)       // This will go over to the next Cycle on purpose to detect oif the next phase is fso-valid
    {
      phase_cnt += phase_inc;
      PhaseIndex |= (1 << phase_cnt);
    }
uint8_t testblub = 0;    // only debugging
    // circle through phases. currently we only support same amount of phases for all ports
    for (uint8_t phase=0; phase < NumPhases; phase++ )
    {
        uint8_t NextPhaseValid = 0;
        uint8_t nextphase; 
        early_green_port[phase] = 0; 

        if ((phase+1) == NumPhases) {
          nextphase = 0;
        }
        else
        {
          nextphase = phase + 1;
        }
        if ((1 << (phase+1)) & PhaseIndex)
        {
          NextPhaseValid = 1;               // The next phase sends a FSO frame, need to set the RedPhase interrupt.
        }
        // Find out if we are in a valid phase
        if ((1 << phase) & PhaseIndex)
        {
          fso[0] = pFrameDataArray[0]->FrameSendOffset - GATE_MII_TX_DELAY - SWITCH_TDMA_GATE_DELAY;
          fso_port[0] = ((pFrameDataArray[0]->TxPortGroupArray) >> 1)-1;   // TxPortGroupArray encoded as mask with bit 0 = local, Bit 1 = Port 0 and pit 1 = Port 1
        }
        else
        {
          fso[0] = 0;
        }
        noredtx[phase] = 0;
        noredtx[nextphase] = 0;
        noredrx[phase] = 0;
        noredrx[nextphase] = 0;

        for (uint8_t port=0; port < pPDIRData->PDIRBeginEndData.NumberOfPorts; port++ )   // circle through ports
        {
          green_start_rx[port] = 0;
          green_start_tx[port] = 0;
          if (!(singlephase & (0x01 << port)))
          {
              // fetch the times for this phase
            uint32_t TxPhaseAssignment = pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].TxPhaseAssignment;    // Index for timings
            uint32_t RxPhaseAssignment = pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[phase].RxPhaseAssignment;
            uint32_t TxPhaseAssignment_next = pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].PhaseAssignmentArray[nextphase].TxPhaseAssignment;
            // NxPhaseAssignment holds three 4-Bit numbers, red_index is at 0x0000f, orange_index is at 0x00f0 and green_index is at 0x0f00
            uint8_t assi = TxPhaseAssignment & 0x0f;    // Indicates which out of the max 5 possible assignments (i. e. timing sets) are used
            red_start = pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxRedOrangePeriodBegin;   // Always 0
            assi = (RxPhaseAssignment & 0x0f00) >> 8;
            green_start_rx[port] = pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].RxGreenPeriodBegin;
            if (green_start_rx[port] == 0)
            {
              noredrx[phase] |= 0x01 << port;
            }
            assi = (TxPhaseAssignment & 0x0f00) >> 8;
            uint8_t nextassi = (TxPhaseAssignment_next & 0x0f00) >> 8;
            green_start_tx[port] = pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[assi].TxGreenPeriodBegin;
            if (green_start_tx[port] > (GATE_MII_TX_DELAY + SWITCH_TDMA_GATE_DELAY_MIN)) 
            {
              green_start_tx[port] -= (GATE_MII_TX_DELAY + SWITCH_TDMA_GATE_DELAY_MIN);   // correct for delay between gate opening and transmission
                                                      // if time is 0 we ignore this (results in small inaccuracy)
            }
            else
            {
              noredtx[phase] |= 0x01 << port;    // Time for green start TX is 0, we have no red phase
              if ((pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].NumberOfPhases == 1) && (green_start_rx[port] )== 0)
              {
                singlephase |= 0x01 << port;     // This is a RT / NRT port  (not IRT so no special timing)
              }
            }
            noredtx[phase] |= singlephase;   // If singlephase was set in initial phase set bit for all phases
            noredrx[phase] |= singlephase;

            if (pPDIRData->PDIRBeginEndData.BeginEndDataArray[port].Assignments[nextassi].TxGreenPeriodBegin == 0)
            {
              noredtx[nextphase] |= 0x01 << port;    // Time for green start is 0, we have no red phase in the next phase

            }
            noredtx[nextphase] |= singlephase;
            yellow_start = cycle_time - 150000;    // 150us fix for yellow period
          }
        }   // ports


#define GREEN_TX_0 0x01
#define GREEN_TX_1 0x02
#define GREEN_RX_0 0x04
#define GREEN_RX_1 0x08
#define GREEN_FSO_0 0x010
#define GREEN_FSO_1 0x020
// INT_PRE_TIME The time in ns that the interrupt to switch the first of the two RX receive gates must be set in advance to compensate for the interrupt call time
#define INT_PRE_TIME 300

        uint32_t last_time_val = 1;
        uint8_t set_mark = 0;   // Reminder for active configurations, one bit for each setting to avoid configs to be set twice
        // Adjust time for the first of the two receive times if they are not equal. The earlier of the two needs to trigger an interrupt in advance and needs some pretime.
        if ((green_start_rx[0] != 0) && (green_start_rx[1] != 0))
        {
          if (green_start_rx[0] > green_start_rx[1])
          {
            green_start_rx[1] -= INT_PRE_TIME;
            green_start_rx[0] -= SWITCH_TDMA_RED_DELAY;
          }
          else if (green_start_rx[0] < green_start_rx[1])
          {
            green_start_rx[0] -= INT_PRE_TIME;
            green_start_rx[1] -= SWITCH_TDMA_RED_DELAY;
          }
        }

        for (uint8_t i=0; i<6; i++)     // Circle through possible times/TCVs and sort them
        {
          uint8_t set_mark_local = 0;
          tcv_vector[i].time = 0xffffffff;
          tcv_vector[i].status = 0;

          /* For tcv_vector[i] , search for the lowest next action by scanning all and store in tcv_vector
             If value is too close to previous value add TDMA_MAC_BACKTOBACK to previous timing thus shifting this
             actrion back a little bit. If two actions are at the same time two tcv_vectors with the same time are stored
             The entrie setting further down will hjandle this appropriately,
             In case the green_start is at 0 this is not handled here, i. e. there is no TCV vector for this case */

          if ((fso[0] < tcv_vector[i].time)  && (fso[0] >= last_time_val)  && ((set_mark & GREEN_FSO_0) == 0))
          {
            if ((i == 0) || ((i>0) && (fso[0] - tcv_vector[i-1].time) > TDMA_MAX_BACKTOBACK) )
            {
              tcv_vector[i].time = fso[0];    // This time triggers the fso
              tcv_vector[i].port = fso_port[0];
              tcv_vector[i].status = FSO_TYPE;
              set_mark_local = GREEN_FSO_0;
            }
            else
            {
              if (fso[0] == tcv_vector[i-1].time) // same time as previous but previous was not us
              {
                tcv_vector[i].time = fso[0];
              }
              else
              {
                tcv_vector[i].time = tcv_vector[i-1].time + TDMA_MAX_BACKTOBACK;   // We are too close behind the previous entry
              }
              tcv_vector[i].port = fso_port[0];
              tcv_vector[i].status = FSO_TYPE;
              set_mark_local = GREEN_FSO_0;
            }
          }
          if ((fso[1] < tcv_vector[i].time)  && (fso[1] >= last_time_val)  && ((set_mark & GREEN_FSO_1) == 0))
          {
            if ((i == 0) || ((i>0) && (fso[1] - tcv_vector[i-1].time) > TDMA_MAX_BACKTOBACK) )
            {
              tcv_vector[i].time = fso[1];
              tcv_vector[i].port = fso_port[1];
              tcv_vector[i].status = FSO_TYPE;
              set_mark_local = GREEN_FSO_1;
            }
            else
            {
              if (fso[1] == tcv_vector[i-1].time) // same time as previous but previous was not us
              {
                tcv_vector[i].time = fso[1];
              }
              else
              {
                tcv_vector[i].time = tcv_vector[i-1].time + TDMA_MAX_BACKTOBACK;   // We are too close behind the previous entry
              }
              tcv_vector[i].port = fso_port[1];
              tcv_vector[i].status = FSO_TYPE;
              set_mark_local = GREEN_FSO_1;
            }
          }
          if ((green_start_tx[0] < tcv_vector[i].time)  && (green_start_tx[0] >= last_time_val) && ((set_mark & GREEN_TX_0) == 0))
          {
            if ((i == 0) || ((i>0) && (green_start_tx[0] - tcv_vector[i-1].time) > TDMA_MAX_BACKTOBACK) )
            {
              tcv_vector[i].time = green_start_tx[0];
              tcv_vector[i].port = 0;
              tcv_vector[i].status = TX;
              set_mark_local = GREEN_TX_0;
            }
            else
            {
              if ((green_start_tx[0] == tcv_vector[i-1].time) && (tcv_vector[i-1].status != FSO_TYPE)) // same time as previous but previous was not us, and previous is not FSO as TDMA controller cannot handle FSO at same time
              {
                tcv_vector[i].time = green_start_tx[0];
              }
              else
              {
                tcv_vector[i].time = tcv_vector[i-1].time + TDMA_MAX_BACKTOBACK;   // We are too close behind the previous entry
              }
              tcv_vector[i].port = 0;
              tcv_vector[i].status = TX;
              set_mark_local = GREEN_TX_0;
            }
          }
          if ((green_start_tx[1] < tcv_vector[i].time)  && (green_start_tx[1] >= last_time_val) && ((set_mark & GREEN_TX_1) == 0) && (green_start_tx[1] < cycle_time) )
          {
            if ((i == 0) || ((i>0) && (green_start_tx[1] - tcv_vector[i-1].time) > TDMA_MAX_BACKTOBACK) )
            {
              tcv_vector[i].time = green_start_tx[1];
              tcv_vector[i].port = 1;
              tcv_vector[i].status = TX;
              set_mark_local = GREEN_TX_1;
            }
            else
            {
              if ((green_start_tx[1] == tcv_vector[i-1].time) && (tcv_vector[i-1].status != FSO_TYPE)) // same time as previous but previous was not us
              {
              tcv_vector[i].time = green_start_tx[1];
              }
              else
              {
                tcv_vector[i].time = tcv_vector[i-1].time + TDMA_MAX_BACKTOBACK;   // We are too close behind the previous entry
              }
              tcv_vector[i].port = 1;
              tcv_vector[i].status = TX;
              set_mark_local = GREEN_TX_1;
            }
          }
          if ((green_start_rx[0] < tcv_vector[i].time)  && (green_start_rx[0] >= last_time_val)  && ((set_mark & GREEN_RX_0) == 0))
          {
            if ((i == 0) || ((i>0) && (green_start_rx[0] - tcv_vector[i-1].time) > TDMA_MAX_BACKTOBACK) )
            {  
              tcv_vector[i].time = green_start_rx[0]; 
              tcv_vector[i].port = 0; 
              tcv_vector[i].status = RX; 
              set_mark_local = GREEN_RX_0; 
              if ((early_green_port[phase] == 0) && (green_start_rx[1] > 0)) { early_green_port[phase] = 0x01;  }   // This is the first of the two Green Starts 
            }
            else
            {
              if (green_start_rx[0] == tcv_vector[i-1].time) // same time as previous but previous was not us
              {
              tcv_vector[i].time = green_start_rx[0];
              }
              else
              {
                tcv_vector[i].time = tcv_vector[i-1].time + TDMA_MAX_BACKTOBACK;   // We are too close behind the previous entry
              }
              tcv_vector[i].port = 0; 
              tcv_vector[i].status = RX; 
              set_mark_local = GREEN_RX_0; 
              if ((early_green_port[phase] == 0) && (green_start_rx[1] > 0)) { early_green_port[phase] = 0x01;  }   // This is the first of the two Green Starts 
            }
          }
          if ((green_start_rx[1] < tcv_vector[i].time)  && (green_start_rx[1] >= last_time_val)  && ((set_mark & GREEN_RX_1) == 0))
          {
            if ((i == 0) || ((i>0) && (green_start_rx[1] - tcv_vector[i-1].time) > TDMA_MAX_BACKTOBACK) )
            {  
              tcv_vector[i].time = green_start_rx[1]; 
              tcv_vector[i].port = 1; 
              tcv_vector[i].status = RX; 
              if (((early_green_port[phase] == 0)||(set_mark_local & GREEN_RX_0)) && (green_start_rx[0] > 0)) { early_green_port[phase] = 0x02;  }   // This is the first of the two Green Starts 
              set_mark_local = GREEN_RX_1; 
            }
            else
            {
              if (green_start_rx[1] == tcv_vector[i-1].time) // same time as previous but previous was not us
              {
                tcv_vector[i].time = green_start_rx[1];
              }
              else
              {
                tcv_vector[i].time = tcv_vector[i-1].time + TDMA_MAX_BACKTOBACK;   // We are too close behind the previous entry
              }
              tcv_vector[i].port = 1; 
              tcv_vector[i].status = RX; 
              if (((early_green_port[phase] == 0)||(set_mark_local & GREEN_RX_0)) && (green_start_rx[0] > 0)) { early_green_port[phase] = 0x02;  }   // This is the first of the two Green Starts 
              set_mark_local = GREEN_RX_1; 
            }
          }
          if ((tcv_vector[i].status == TX) || (tcv_vector[i].status == FSO_TYPE))
          {
            num_tx_actions++;
          }
          else if (tcv_vector[i].status == RX)
          {
            num_rx_actions++;
          }
          last_time_val = tcv_vector[i].time;
          set_mark |= set_mark_local;
        }


        // Set up scheduler to define transmitting periods
    /* Default implementation:
        Time 0    Start of RED Period, Queue 3 is enabled for all RTC frames received on other port, NRT frames are discarded
        Time fso_start:   Transmission of first local RTC3 frame. Other rtc3 frames are blocked (should not happen), NRT frames are discarded, single trigger
        Time fso_start+1: Foreign rtc3 frames are passed , NRT frames are discarded, gate mode
        Time fso_start n:   Transmission of n-th local RTC3 frame. Other rtc3 frames are blocked (should not happen), NRT frames are discarded, single trigger
        Time fso_start+1: Foreign rtc3 frames are passed , NRT frames are discarded, gate mode
        repeat until all IRT frames are transmitted
        Time green start-GMAC_DELAY: Interrupt to prepare descriptors for NRT frames. Foreign Red frames are forwarded, local red frames or NRT frames are dropped.
        Time: GreenStart Port 0: Open gate for green period port 1
        Time: Green Start Port 1. Open gate for port 1
        Time: Yellow: Close all gates
        Time: cycletime-GMAC_DELAY+fso: Interrupt for red interrupt to start preparation of red frames. This might also trigger changes in TDMA setup
    */
        uint8_t gate_mask[2] = {0x0, 0x0};      // gat emask from previous entry, for each port 0 and 1
        port_mask   = 0x03;    // port_mask for scheduling, i. e. scheduling for port 0 and port 1
        enable      = 1;
        portmask    = port_mask;
        uint32_t phase_time = cycle_time * phase;    // Starttime of this phase

        // Prepare settings for REDPHASE at time 0. Setting of REDPHASE is 62ns late, but thats an error we can live with. settings for gates have already neem done and are repeated.
        if (noredrx[phase] == 0x03)
        {
          entries[index].red_phase     = 0;           // Indicate Red phase for Profinet IRT
        }
        else
        {
          entries[index].red_phase     =  1;           // Indicate Red phase for Profinet IRT
        }
        if ((noredtx[phase] & 0x03) != 0x03)    // One of the two ports has a red phase in the next cycle
        {
          entries[index].gate_mask     = 0x08;        // enable Queue 3 for RTC3 frames from other devices
          entries[index].port_mask     = ~(noredtx[phase]) & 0x03;    // Port Mask fior the port with the RED phase
          if (entries[index].port_mask & 0x01)
          {
            gate_mask[0] = entries[index].gate_mask;
          }
          if (entries[index].port_mask & 0x02)
          {
            gate_mask[1] = entries[index].gate_mask;
          }
        }
        else    // No RED phase
        {
          entries[index].gate_mask     = 0x03;        // enable Queue 3 for RTC3 frames from other devices
          entries[index].red_phase     = 0;           // Indicate Red phase for Profinet IRT
          entries[index].port_mask     = 0x0;    // Port Mask fior the port with the RED phase
        }
        entries[index].hold_req      = 0;           // One bit enable Hold request for frame preemption ,
        entries[index].trigger_mode  = 0;           // One bit enable trigger mode ,
        entries[index].gate_mode     = 1;           // One bit enable gate mode instead of trigger mode , only on newer devices,
        entries[index].in_ct_enable  = 1;           // One bit enable cut through , must be set to make preemption work
        entries[index].out_ct_enable = 1;           // One bit enable cut through ,
        entries[index].interval      = phase_time;           // // open gate a bit before 0 to make sure the delay of the switch is taken into account
        lasttime = 0;
        entries[index].inc_0         = 1;           // Counter 0
        entries[index].inc_1         = 0;           // Counter 1
        entries[index].gpio          = 0x10;         // Interrupt for  timer tick
        index++;


        // Setup the max. 6 actions for the phase
         // first green action
      uint8_t int_green_rx_marker = 0;     // Remember that we have already set the pattern matcher interrupt
      if (early_green_port[phase] == 0) 
      {
        int_green_rx_marker = 1;   // Only sinngle port, we do not need an interrupt for the early green port
      }
      uint8_t int_green_tx_marker = 0;     // Remember that we have already set the pattern matcher interrupt
      __attribute__((unused))uint8_t int_fso_marker = 0x02;     // Remember that we have already set one FSO interrupt, set to the interrupt number for first int and to 0 for second FSO
      for (int i=0; i<(num_tx_actions + num_rx_actions); i++)
      {
        /* If this time is the same as the last time, i. e. two actions need to be done simultaniously) we count the index back and try to merge the current action with the previous TCV */
        if ((i>0) && (tcv_vector[i].time == entries[index-1].interval))      // Same timing as last one
        {
          index--;    // Point to the previous index, we will only modify this one
          if (tcv_vector[i].status == RX)    // This is an RX time. On first occurence (one port green, the other red) trigger interrupt for pattern matcher.
          {
            if (tcv_vector[i-1].status == TX)  // Previous was TX,
            {
               if (int_green_rx_marker == 0)   // Early RX port, set interrupt flag
               {
                 entries[index].gpio = 0x08;        // Enable GPIO 4 to change pattern matcher for early port
                 int_green_rx_marker = 1;
               }
               else    // Second RX time
               {
                 entries[index].port_mask |= 1 << (tcv_vector[i].port);
                 entries[index].red_phase     = 0;           // Keep Red phase for Profinet IRT
               }
            }
            else if (tcv_vector[i-1].status == RX)  // Previous was also RX, remove ionterrupt, not needed any more
            {
              entries[index].gpio = 0x0;      // No int for pattern adjust needed
            }
            else if (tcv_vector[i-1].status == FSO_TYPE)  // Previous was FSO
           {
               if (int_green_rx_marker == 0)   // Early RX port, set interrupt flag
               {
                 entries[index].gpio = 0x08;        // Enable GPIO 4 to change pattern matcher for early port
                 int_green_rx_marker = 1;
               }
               else
               {
                 entries[index].red_phase = 0x01;        // Enable GPIO 4 to change pattern matcher for early port
               }
            }
          }
          else if (tcv_vector[i].status == TX)
          {
            if (tcv_vector[i-1].status == TX)  // Previous was also TX, adjust Port MASK
            {
              entries[index].port_mask |= 1 << (tcv_vector[i].port);
            }
            else if (tcv_vector[i-1].status == RX)  // Previous was RX which should have no gate setting and port mask
            {
              entries[index].port_mask |= 1 << (tcv_vector[i].port);
              entries[index].gate_mask = 0x03;      // Set to green phase, enable green queues
            }
            else if (tcv_vector[i-1].status == FSO_TYPE)  // Previous was FSO
            {
              // Should not happen here, this is handled in the tcv_vector setting, which shifts the Non-FSO by 128 ns so they fit in two TCVs.
            }
          }
          else if (tcv_vector[i].status == FSO_TYPE)
          {
            if (tcv_vector[i-1].status == TX)  // Previous was TX (must be other port)
            {
              // Should not happen here, this is handled in the tcv_vector setting, which shifts the Non-FSO by 128 ns so they fit in two TCVs.
            }
            else if (tcv_vector[i-1].status == RX)  // Previous was
            {
              entries[index].port_mask     = 0x01 << tcv_vector[i].port;        // Port Mask with one-hot encoding
              entries[index].gate_mask     = 0x0c;        // open gate for this IRT frame
            }
            else if (tcv_vector[i-1].status == FSO_TYPE)  // Previous was FSO
            {
              // Two FSO settings, not supported yet
            }
          }

        }
        else    // no time doublette
        {

          if (tcv_vector[i].status == RX)    // This is an RX time. On first occurence (one port green, the other red) trigger interrupt for pattern matcher.
                                             // On second occurence we need to disable to RED phase completely
                                             // RX generally onl handles the pattern matcher, there is no handling on the queues
          {
            if (int_green_rx_marker == 0)   // Early RX port, port mask 0, no changes, only GPIO for interrupt
            {                               // Interrupt then changes port mask and pattern matcher
             // greenstart_time[i] -= 900;    // Reduce to compensate for interrupt call time
              entries[index].port_mask     = 0x00;
              entries[index].gate_mask     = gate_mask[tcv_vector[i].port];        // Gate is not changed for this port
              entries[index].gpio          = 0x08;        // Enable GPIO 4 to change pattern matcher for early port
              entries[index].trigger_mode  = 0;           // One bit enable trigger mode ,
              entries[index].gate_mode     = 1;           // One bit enable gate mode instead of trigger mode , only on newer devices,
              if (num_rx_actions == 1)
              {
                entries[index].red_phase     = 0;
              }
              else
              {
                entries[index].red_phase     = 1;           // Keep Red phase for Profinet IRT
              }
              int_green_rx_marker = 1;          // Marker for first occurence already done
            }
            else                            // Second occurence of green RX time
            {
              entries[index].port_mask     = 0x01 << tcv_vector[i].port;        // Port Mask only for both ports, both are green now
              entries[index].gate_mask     = gate_mask[tcv_vector[i].port];   // Gate is not changed for this port
              entries[index].gpio          = 0x00;        // No GPIO
              entries[index].red_phase     = 0;           // Red phase done for Profinet IRT
              entries[index].trigger_mode  = 0;           // One bit enable trigger mode ,
              entries[index].gate_mode     = 1;           // One bit enable gate mode instead of trigger mode , only on newer devices,
            }
          }
          else if (tcv_vector[i].status == TX)
          {
            if (int_green_tx_marker == 0)     // First TX green phase, open respective port
            {
              entries[index].port_mask     = 0x01 << tcv_vector[i].port;        // Port Mask with one-hot encoding
              entries[index].gate_mask     = 0x03;        // open gate for this NRT gate
              gate_mask[tcv_vector[i].port] = 0x03;          // Store gate value for next entry
              entries[index].gpio          = 0x00;        // No GPIO
              entries[index].red_phase     = entries[index-1].red_phase;           // Red phase same as before
              entries[index].trigger_mode  = 0;           // One bit enable trigger mode ,
              entries[index].gate_mode     = 1;           // One bit enable gate mode instead of trigger mode , only on newer devices,
            }
            else                    // Last TX green phase time, open gates for complete green
            {
              entries[index].port_mask     = 0x01 << tcv_vector[i].port;        // Port Mask with one-hot encoding
              entries[index].gate_mask     = 0x03;        // open gate for both NRT gates
              gate_mask[tcv_vector[i].port] = 0x03;          // Store gate value for next entry
              entries[index].gpio          = 0x00;        // No GPIO
              entries[index].red_phase     = entries[index-1].red_phase;           // Red phase not changed
              entries[index].trigger_mode  = 0;           // One bit enable trigger mode ,
              entries[index].gate_mode     = 1;           // One bit enable gate mode instead of trigger mode , only on newer devices,
            }
          }
          else if (tcv_vector[i].status == FSO_TYPE)
          {
              entries[index].port_mask     = 0x01 << tcv_vector[i].port;        // Port Mask with one-hot encoding
              entries[index].gate_mask     = 0x0c;          // open gate for this IRT frame
              entries[index].gpio          = 0x0;        // Interrupt for NRT frames for green phase
              entries[index].red_phase     = entries[index-1].red_phase;           // Red phase same as before
              entries[index].trigger_mode  = 0;           // One bit enable trigger mode ,
              entries[index].gate_mode     = 1;           // One bit enable gate mode instead of trigger mode , only on newer devices,
              gate_mask[tcv_vector[i].port] = 0x0c;          // Store gate value for next entry
          }
          else {
            break;
          }
          entries[index].hold_req      = 0;           // One bit enable Hold request for frame preemption ,
          entries[index].in_ct_enable  = 1;           // One bit enable cut through ,
          entries[index].out_ct_enable = 1;           // One bit enable cut through ,
          entries[index].interval      = phase_time + tcv_vector[i].time; // Offset of schedule entry in ns  entry related to start of cycle
          lasttime = entries[index].interval;
          entries[index].inc_0         = 0;           // Counter o
          entries[index].inc_1         = 0;           // Counter 1
        }
        index++;
      }

        // yellow phase
        entries[index].gate_mask     = 0x03;        // Enable NRT frames for all ports except management port
        entries[index].port_mask     = ~(noredtx[nextphase]) & 0x03; // Port Mask with one-hot encoding
        entries[index].hold_req      = 0;           // One bit enable Hold request for frame preemption ,
        entries[index].trigger_mode  = 0;           // One bit enable trigger mode ,
        entries[index].gate_mode     = 1;           // One bit enable gate mode instead of trigger mode , only on newer devices,
        entries[index].red_phase     = 0;           // Indicate Red phase for Profinet IRT
        entries[index].in_ct_enable  = 1;           // One bit enable cut through ,
        entries[index].out_ct_enable = 1;           // One bit enable cut through ,
     // entries[index].interval = yellow_start - GMAC_DELAY; // Offset of schedule entry in ns  entry related to start of cycle
        entries[index].interval      = phase_time + yellow_start - 200;   // Offset of schedule entry in ns  entry related to start of cycle
        lasttime = entries[index].interval;
        entries[index].inc_0         = 0;           // Counter o
        entries[index].inc_1         = 0;           // Counter 1
        entries[index].gpio          = 0x04;        // Enable GPIO 2 to trigger transmission of yellow frames
        index++;

         // yellow  phase
        entries[index].gate_mask     = 0x00;        // Gate Mask with one-hot encoding  Axel Test if there are frames on queues f0
        entries[index].port_mask     = (~(noredtx[nextphase])) & 0x03;  // Only for port with red phase
        entries[index].hold_req      = 0;            // One bit enable Hold request for frame preemption ,
        entries[index].trigger_mode  = 0;            // One bit enable trigger mode ,
        entries[index].gate_mode     = 1;            // One bit enable gate mode instead of trigger mode , only on newer devices,
        entries[index].red_phase     = 0;            // Indicate Red phase for Profinet IRT
        entries[index].in_ct_enable  = 1;            // One bit enable cut through ,
        entries[index].out_ct_enable = 1;            // One bit enable cut through ,
        entries[index].interval      = phase_time + yellow_start; // Offset of schedule entry in ns  entry related to start of cycle
        lasttime = entries[index].interval;
        entries[index].inc_0         = 0;            // Counter o
        entries[index].inc_1         = 1;            // Counter 1
        entries[index].gpio          = 0;            // Counter 1
        index++;

        if (NextPhaseValid == 1)      // Only trigger interrupt if we send a RTC3 frame in the next cycle/phase
        {
            // Entry is the same as above, trigger interrupt for transission of local RTC3 to GMAC
          entries[index].gate_mask     = 0x0;        // Enable Queues 2 and 3 for local RTC3 frames and others
          entries[index].port_mask     = 0x0;    // no change, only trigger interrupt
          entries[index].hold_req      = 0;           // One bit enable Hold request for frame preemption ,
          entries[index].trigger_mode  = 0;           // One bit enable trigger mode ,
          entries[index].gate_mode     = 1;           // One bit enable gate mode instead of trigger mode , only on newer devices,
          entries[index].red_phase     = 0;           // Indicate Red phase for Profinet IRT
          entries[index].in_ct_enable  = 1;           // One bit enable cut through , must be set to make preemption work
          entries[index].out_ct_enable = 1;           // One bit enable cut through ,
          entries[index].interval      = phase_time + cycle_time - GMAC_DELAY; // offset of schedule entry in ns
          lasttime = entries[index].interval;
          entries[index].inc_0         = 0;           // Counter 0
          entries[index].inc_1         = 0;           // Counter 1
          entries[index].gpio          = 0x01;           // GPIO 0 enabled to trigger frame transmission interrupt
          index++;
        }

        // Prepare settings for gates at time 0 of next phase
        if ((noredtx[nextphase] & 0x03) != 0x03)    // One of the two ports has a red phase in the next cycle
        {
          entries[index].gate_mask     = 0x08;        // enable Queue 3 for RTC3 frames from other devices
          entries[index].red_phase     = 0;           // Indicate Red phase for Profinet IRT (here 0 as its done at time 0 of next entry)
          entries[index].port_mask     = ~(noredtx[nextphase]) & 0x03;    // Port Mask fior the port with the RED phase
          if (entries[index].port_mask & 0x01)
          {
            gate_mask[0] = entries[index].gate_mask;
          }
          if (entries[index].port_mask & 0x02)
          {
            gate_mask[1] = entries[index].gate_mask;
          }
        }
        else   // No RED phase on any port
        {
          entries[index].gate_mask     = 0x03;        // enable Queue 0/1 for NTRT
          entries[index].red_phase     = 0;           // Indicate Red phase for Profinet IRT
          entries[index].port_mask     = 0x03;    //
          gate_mask[0] = entries[index].gate_mask;
          gate_mask[1] = entries[index].gate_mask;
        }
        entries[index].hold_req      = 0;           // One bit enable Hold request for frame preemption ,
        entries[index].trigger_mode  = 0;           // One bit enable trigger mode ,
        entries[index].gate_mode     = 1;           // One bit enable gate mode instead of trigger mode , only on newer devices,
        entries[index].in_ct_enable  = 1;           // One bit enable cut through , must be set to make preemption work
        entries[index].out_ct_enable = 1;           // One bit enable cut through ,
        entries[index].interval      = phase_time + cycle_time - GATE_MII_TX_DELAY;    // open gate a bit before 0 to make sure the delay of the switch is taken into account
        lasttime = 0;
        entries[index].inc_0         = 0;           // Counter 0
        entries[index].inc_1         = 0;           // Counter 1
        entries[index].gpio          = 0x02;         // Interrupt for Green Frame prep. and timer tick
        index++;

        if (singlephase > 0)      // One of the two ports is a NRT or RT port, open this for the green phase
        {
            //
          entries[index].gate_mask     = 0x03;        // Enable Queues 1 and 0 for all RT and NRT frames
          entries[index].port_mask     = singlephase;    //
          entries[index].hold_req      = 0;           // One bit enable Hold request for frame preemption ,
          entries[index].trigger_mode  = 0;           // One bit enable trigger mode ,
          entries[index].gate_mode     = 1;           // One bit enable gate mode instead of trigger mode , only on newer devices,
          entries[index].red_phase     = 0;           // No red phase in this case
          entries[index].in_ct_enable  = 1;           // One bit enable cut through , must be set to make preemption work
          entries[index].out_ct_enable = 1;           // One bit enable cut through ,
          entries[index].interval      = phase_time + cycle_time - GATE_MII_TX_DELAY + TDMA_MAX_BACKTOBACK; // open gate a bit before 0 to make sure the delay of the switch is taken into account and leave enough time to previous setting
          lasttime = entries[index].interval;
          entries[index].inc_0         = 0;           // Counter 0
          entries[index].inc_1         = 0;           // Counter 1
          entries[index].gpio          = 0x0;           //
          index++;
        }
        testblub++;     // only for debuggign
    } // phases

#ifdef PrintfDebug
      char text_string[20];
      sprintf(&text_string[0], "TDMA PRM set %d", index);
      printeth(text_string);
#endif
  }   // Flash not empty
    num_entries = index;
    if (init_setup == 1)
    {
        // enable the nterrupts for the scheduler
        // p_reg_ethsw->TDMA_GPIO = 0x55 << 16; // Set GPIO to strobe mode
        ether_switch_enable_tdma0_icu();        // Interrupt 0 connected to TDMA GPIO0. Triggers before the beginning of the RED2 phase to initiate transmission of the local red frames
        ether_switch_enable_tdma1_icu();        // Interrupt 1 connected to TDMA GPIO1. Triggers before the beginning of the GREEN phase to initiate transmission of the local green frames
        ether_switch_enable_tdma2_icu();        // Interrupt 2 connected to TDMA GPIO2. Triggers before the beginning of the YELLOW phase to initiate transmission of the remaining local green frames
        ether_switch_enable_tdma3_icu();        // Interrupt 3 connected to TDMA GPIO3. Triggers before the beginning of the GREEN phase to adjust the pattern matcher for the RED/GREEN Issue fix
    }

		// start time at next turnaround of clock
		uint32_t    base_time   = 0;

		//Axel ethsw_taprio_init (timer);  // Start Timer 0 (leaves Timer 0 as system timer)
		ethsw_taprio_qopt_set (portmask, enable, timer, base_time, cycle_time, NumPhases, num_entries, &entries[0], reset);   // set scheduler

#if 0
		//setup frame preemption
		if (enable_preemption == 1)
		{
			port_mask = 0x07;
			uint8_t queue_mask         = 0x03;  // Make queues 0 and 1 preemptable
			uint8_t command            = PREEMPT_ENA | PREEMPT_VERIFY_DIS;  // Disable negotiation
			uint8_t minfragsize64      = 0;     // Miimum fragmentation size 64
			uint8_t close_preempt_mask = 0x0;   // Preempt frame when gate closes

			ethsw_setup_frame_preemption (port_mask, queue_mask, close_preempt_mask, minfragsize64, command);
		}
#endif
}

LSA_VOID EDDP_LL_Set_IP_Address(uint8_t *ip_address)
{
#if IOD_INCLUDE_STORM_FILTER_ARP_HW
    setup_pattern_matcher_ARP(ip_address);
#endif /* IOD_INCLUDE_STORM_FILTER_ARP_HW */
}

#ifdef PrintfDebug
uint8_t printeth_buf[100] = {
//	0x20, 0x87, 0x57, 0x6d, 0x29, 0xd1,   //  Destination MAC address
	0x0, 0xe0, 0x4c, 0x68, 0x43, 0x31,      //  Destination MAC address
	0x74, 0x90, 0x50, 0x10, 0xe9, 0x07,
	0x08, 0x00,
	0x45, 0x00,
	0x00, 0x00,    // length
	0x00, 0x00,
	0x00, 0x00,
	0x80, 0x11,
	0x79, 0xfc,   // header checksum, disabled
	0xc0, 0xa8, 0x00, 0x1e,   // source address 128.168.0.30
	0x00, 0x00, 0x00, 0x00,
	0x02, 0x02, 0x02, 0x02,    // Source/destination port 514
	0x00, 0x1e,    // length 30
	0xf9, 0xb1,    // checksum
	0x00
}; // Payload follows

QueueHandle_t printeth_queue;
void printeth_tx(char * printdata);
void app_printeth_thread_proc(void);

void printethFromISR(char * printdata){

  xQueueSendToBackFromISR(printeth_queue, printdata, 0);
}

// This is called from the DUT to print the string
void printeth(char * printdata){

  xQueueSendToBack(printeth_queue, printdata, 0);
}

uint32_t idisc_p0 = 0;
uint32_t idisc_p1 = 0;
uint32_t odisc_p0 = 0;
uint32_t odisc_p1 = 0;

// This thread waits for a string to be transmitted and forwards it to the ethernet interface
void app_printeth_thread_proc (void)
{
    R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
    R_GMAC_Type *  p_reg_etherc    = (R_GMAC_Type *) R_GMAC_BASE;

    char printdata_rb[65];

    Bsp_GetMacAddr ((PNIO_UINT8*)&printeth_buf[6]);
    printeth_buf[29] = printeth_buf[10];    // Use variable MAC Address for source IP address, this is not according to spec

    while (1)
    {
      if (xQueueReceive(printeth_queue, &printdata_rb[0], 10) == pdPASS)       //portMAX_DELAY);
      {
        printeth_tx(&printdata_rb[0]);
      }
      else
      {
        // Check frame drops
        if ((odisc_p0 !=  p_reg_ethsw->ODISC0) || (odisc_p1 !=  p_reg_ethsw->ODISC1) ||
            (idisc_p0 !=  p_reg_ethsw->IDISC_ANY0) || (idisc_p1 !=  p_reg_ethsw->IDISC_ANY1))
        {
              char text_string[30];
              snprintf(&text_string[0], 29, "disc %d %d %d %d", p_reg_ethsw->IDISC_ANY0, p_reg_ethsw->ODISC0, p_reg_ethsw->IDISC_ANY1, p_reg_ethsw->ODISC1 ); 
              odisc_p0 =  p_reg_ethsw->ODISC0;
              odisc_p1 =  p_reg_ethsw->ODISC1;
              idisc_p0 =  p_reg_ethsw->IDISC_ANY0;
              idisc_p1 =  p_reg_ethsw->IDISC_ANY1;
              printeth_tx(&text_string[0]);
        }
      }
    }
}
uint32_t retvalcnt = 0;
ssize_t sendto(void *const buf, size_t len, int idx, struct timespec * timeStamp, struct gPTPd * gPTP);

// This transfers the string to the ethernet interface and has time to do it
void printeth_tx(char * printdata)
{
  const int TEXTPOS = 42;
  extern struct gPTPd gPTPd[NUM_PORTS];
	struct gPTPd * PTPd;
//	if (gPTPd[0].enabled != 1)
//	{
//		return;
//	}
	while (gPTPd[0].enabled != 1)
  {
    vTaskDelay(2);
  };

	int i;

	for (i = TEXTPOS; i < (TEXTPOS+20); i++)
	{
		if ((0 == *printdata) || ('\n' == *printdata))
		{
			break;
		}
		else
		{
			printeth_buf[i] = *printdata++;
		}
	}

	for (int y = i; y<(TEXTPOS+20); y++)
	{
		printeth_buf[y] = ' ';
	}
	PTPd = &gPTPd[0];
   uint32_t retval;
   retval = sendto( &printeth_buf[0], i, 0, NULL, PTPd);
  if (retval == 0) {
    retvalcnt++;
  }
}
#endif

/*****************************************************************************/
/* end of file eddp_llif.c                                                  */
/*****************************************************************************/
