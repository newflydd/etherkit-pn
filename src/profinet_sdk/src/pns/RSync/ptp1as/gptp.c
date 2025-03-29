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

#include "dep/datatypes_dep.h"
#include "ecos_ertec_os_utils.h"
#include "pnorsyncif.h"
#include "pnio_types.h"
#include "r_ether_rzt2.h"
#include "r_ether_api_rzt2.h"
#include "r_ether_switch_rzt2_ext.h"
#include "gptpcmn.h"
#include "delaymsr.h"
#include "bmc.h"
#include "sync.h"

//EDDP/PNO includes
#include "compiler.h"
#include "lsa_cfg.h"
#include "edd_cfg.h"
#include "lsa_types.h"
#include "os.h"
#include "lsas_inc.h"
#include "lsas_int.h"
#include "lsas_dat.h"
#include "eddp_inc.h"
#include "eddp_int.h"
#include "pnio_types.h"
#include "common_data.h"
#include "eddp_types.h"
#include "edd_usr.h"
#include "os.h"

// Legacy interfacing
#include "gsy_int.h"

//Low level PNO access
#include "eddp_core_glb.h"
#include "eddp_rzt2.h"

/* needed for vTaskDelay() */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

struct gPTPd gPTPd[NUM_PORTS];

/* inbount TS, TS taken from switch MGMT-Tag */
void * rxTsQueue[NUM_PORTS] = {NULL};

EDDP_LL_HANDLE_PTR_TYPE pHandle = NULL;

/* Create the task that processes PTP RX/TX events. */
static PNIO_UINT32  pThreadIdSend;
static PNIO_UINT32  pThreadIdSync;
static PNIO_UINT32  pThreadIdRdeceive[NUM_PORTS];

MAC_ADR_TYPE emptyMAC = {PTCP_EMPTY_ADDR};
static MAC_ADR_TYPE MasterMAC;
static PTCP_UUID subdomainUUID;
static uint8_t rsyncRunning = 0;

uint16_t gSyncPort = 0xffff;

void gptp_clearMasterMAC(struct gPTPd * gPTPd)
{
	memset(&gPTPd->masterMAC, 0, MAC_ADR_LENGTH);
	memset(&MasterMAC, 0, MAC_ADR_LENGTH);
	memset(&subdomainUUID, 0, UUID_LENGTH);
	gSyncPort = 0xFFFF;
}

/* inform CM about master MAC address */
void setLLDPMasterMAC(int16_t diagIndId, struct gPTPd* gPTPd, uint8_t hasSubDomain)
{
	if(GSY_SYNCID_NONE == gPTPd->pChUsr->SyncId)
	{
		return;
	}
	GSY_DRIFT_PTR pDrift = &gPTPd->pChUsr->pChSys->Drift[gPTPd->pChUsr->SyncId][0];

	pDrift->SyncId         = gPTPd->pChUsr->SyncId;
	
	/* Get subdomain from PRM dataset (PRM data are stored in: pChUsr->SyncData) */	
	pDrift->Diag.SyncId = gPTPd->pChUsr->SyncId;
	if(hasSubDomain)
	{
		pDrift->Diag.Subdomain = 1;
		pDrift->Diag.StateSync = gPTPd->SyncState;
		memcpy(&pDrift->MasterAddr, &gPTPd->masterMAC, MAC_ADR_LENGTH);
	}
	else
	{
		/* This is treated as error on CM side */
		pDrift->Diag.Subdomain = 0;
		pDrift->Diag.StateSync = GSY_DIAG_SYNCID_NOT_SYNCHRONIZED;
		memset(&pDrift->MasterAddr, 0, MAC_ADR_LENGTH);
	}

	gsy_DiagUserInd(diagIndId, gPTPd->pChUsr->pChSys, pDrift, 
			0      /* LSA_UINT32: MasterSeconds */, 
			0      /* LSA_UINT32: MasterNanoseconds */, 
			0      /* LSA_INT32: Offset */, 
			0      /* LSA_INT32: AdjustInterval */, 
			0      /* LSA_INT32: UserValue1 */, 
			0      /* LSA_INT32: UserValue2 */, 
			0      /* LSA_INT32: SetTimeHigh */, 
			0      /* LSA_INT32: SetTimeLow */, 
			0	     /* LSA_INT16: PortId */);
}

static void PTPSend(void)
{
	extern LSA_VOID_PTR_TYPE  EDDP_LL_MNG_HANDLE(LSA_UINT16 InterfaceID);
	extern void delayResponseTimout(PNIO_UINT16 timer_id, PNIO_UINT32 user_id);
	
	EDDP_LOCAL_HDB_PTR_TYPE pHDB;
	uint16_t Status = EDDP_CoreDevHandleGetHDB(0, &pHDB);
	
	if(EDD_STS_OK == Status)
	{
		pHandle = (EDDP_LL_HANDLE_PTR_TYPE)EDDP_LL_MNG_HANDLE(1);
		
		OsWaitOnEnable();
		
		for(uint32_t port = 0; port < NUM_PORTS; ++port)
		{
			OsAllocTimer(&gPTPd[port].respTimeout, LSA_TIMER_TYPE_ONE_SHOT , LSA_TIME_BASE_1MS, (PNIO_VOID*)delayResponseTimout);
			
			gPTPd[port].enabled = PORT_DISABLED;
			
			if (EDD_PORT_STATE_DISABLE != pHDB->pDDB->SWI.LinkStatus[port+1].PortState)
			{
				gPTPd[port].enabled = PORT_ENABLED;
				
				/* Initialize */
				gptp_init(port);
				
				/* Start operations, wait for phy-link */
				gptp_start();	
				
				/* 
					Real and Max delay values are send in: sendDelayResp() PTCP frame.
					port 0 has special meaning (in SWI), physical ports start at 1.
				*/
				gPTPd[port].RealPortTxDelay = pHDB->pDDB->SWI.LinkStatus[port+1].RealPortTxDelay;
				gPTPd[port].RealPortRxDelay = pHDB->pDDB->SWI.LinkStatus[port+1].RealPortRxDelay;
				gPTPd[port].MaxPortTxDelay  = pHDB->pDDB->SWI.LinkStatus[port+1].MaxPortTxDelay;
				gPTPd[port].MaxPortRxDelay  = pHDB->pDDB->SWI.LinkStatus[port+1].MaxPortRxDelay;
				
				/* Start the state machines */
				dmHandleEvent (&gPTPd[port], GPTP_EVT_DM_ENABLE );
				bmcHandleEvent(&gPTPd[port], GPTP_EVT_BMC_ENABLE);
				csHandleEvent (&gPTPd[port], GPTP_EVT_CS_ENABLE );
			}
		}
		
		// We need to have a proper syncId from PRM data,
		// before we send our first PrmIndication.
		while ((GSY_SYNCID_NONE == gPTPd[ANY_PORT].pChUsr->SyncId) && (GSY_SYNC_ROLE_SLAVE == gPTPd[ANY_PORT].pChUsr->SyncRole))
		{
			vTaskDelay (100);
		}
		
		/* prepare send frames () */
		for(uint32_t port = 0; port < NUM_PORTS; ++port)
		{
			/* Initialize tx */
			gptp_initTxBuf(&gPTPd[port]);
		}
		
		for(int port = 0; port < NUM_PORTS; ++ port)
		{
			gptp_startTimer(&gPTPd[port], GPTP_TIMER_DELAYREQ_RPT, gPTPd[port].dm.delayReqInterval, GPTP_EVT_DM_PDELAY_REQ_RPT);
		}
		
		while(1)
		{			
			for(uint32_t port = 0; port < NUM_PORTS; ++port)
			{					
				if(gPTPd[port].enabled)
				{
					eventLoop(&gPTPd[port]);
				}
				
				/* check if link status has changed */
			  if(GSY_PORT_STATE_DOWN == gPTPd[port].pChUsr->pChSys->Port[port].State)
				{
					gPTPd[port].portDown = 1;
					
					if(0 != gPTPd[port].cableDelay)
					{ // Update LLDP delay value.
						invalidateCableDelay(&gPTPd[port]);
						gptp_initTxBuf(&gPTPd[port]);

//TODO: -OS- This should be fixed (00:50:C2:98:B0:00 ====> 00:50:C2:98:BF:FF = TI2000 TECNOLOGIA INFORMATICA 2000 Mac Address)
            if ((gPTPd[port].masterMAC.MacAdr[0] > 0) && (gPTPd[port].masterMAC.MacAdr[5] > 0))
            {
              clear_drift_reg(1);  
              gPTPd[port].DelayDriftAdjustDone = 0;
            }
					}
				}

				vTaskDelay(1);
				}
			}
		}
	else
	{
		/* FATAL -> EDDP_CoreDevHandleGetHDB failed.*/
		while(1);
	}
}

static void syncFollowUpTask(void)
{
	extern void  *syncFollowUpQueue;
	extern struct gPTPd gPTPd[NUM_PORTS];
	
	struct gPTPd  *PTPd;
	uint16_t masterPort;
	
	OsWaitOnEnable();
	
	while(1)
	{
		PTPd = (struct gPTPd *) OsQueRecv(syncFollowUpQueue, 0xffffffffUL);
		
		if((uint32_t)PTPd & 1)
		{ // Send Follow-UP
			PTPd = (struct gPTPd *)((uint32_t)PTPd & 0xfffffffe);
			masterPort = (PTPd->port + 1) % 2;
			RSYNC_PTCP *pPdu = (RSYNC_PTCP*)gPTPd[masterPort].txBufSyFu;
						
			uint32_t delayFollowUP = (uint32_t)gPTPd[PTPd->port].ts[TX_SYNC_FW].tv_nsec - (uint32_t)gPTPd[masterPort].ts[RX_SYNC_FROM_MASTER].tv_nsec;
			
			if((uint32_t)gPTPd[PTPd->port].ts[TX_SYNC_FW].tv_nsec < (uint32_t)gPTPd[masterPort].ts[RX_SYNC_FROM_MASTER].tv_nsec)
			{
				delayFollowUP += 1000000000;
			}

			uint32_t origDelay = SWAP32(pPdu->PtcpData.SyncFu.PtcpHdr.DelayNS.Dword);
			
			/* set delay */
			pPdu->PtcpData.SyncFu.PtcpHdr.DelayNS.Dword = SWAP32(origDelay + delayFollowUP);
			
			if (0 == sendto(pPdu, SYNC_FU_PDU_SIZE, 5, NULL, &gPTPd[PTPd->port]))
			{
				++gPTPd->sendError;
			}
		}
		
		else

		/* calculate delay to naighbor side */
		if(getTxTS(PTPd, &PTPd->ts[TX_SYNC_FW], 5))
		{
			masterPort = (PTPd->port + 1) % 2;
			RSYNC_PTCP * pPdu = (RSYNC_PTCP*)&PTPd->txBufSyFu;
			
			if(0 == gPTPd[masterPort].twoStep)
			{
				uint32_t delay = (uint32_t)PTPd->ts[TX_SYNC_FW].tv_nsec - (uint32_t)gPTPd[masterPort].ts[RX_SYNC_FROM_MASTER].tv_nsec;
				
				if((uint32_t)PTPd->ts[TX_SYNC_FW].tv_nsec < (uint32_t)gPTPd[masterPort].ts[RX_SYNC_FROM_MASTER].tv_nsec)
				{
					delay += 1000000000;
				}
				
				/* measurement was done based on timer 0 but we need it based on timer 1 -> conversion needed. */
 				// Removed drift correction for forwarding sync frames. Needs to be done later, but current implementation is wrong as we are not using timer 1 and more)
				// delay = adjust_time_drift (delay);				

				/* set delay */
				pPdu->PtcpData.SyncFu.PtcpHdr.DelayNS.Dword = SWAP32(delay);
				
				if (0 == sendto(PTPd->txBufSyFu, SYNC_FU_PDU_SIZE, 5, NULL, PTPd))
				{
					++PTPd->sendError;
				}
			}
		}
	}
}

static uint8_t  frameBuffer[NUM_PORTS][EDD_CFG_RX_FRAME_BUFFER_LENGTH];
static uint16_t receivePort = 0xffff;

static void PTPReceive(void)
{
	uint16_t receiveInstance;
	EDD_RQB_NRT_RECV_TYPE* RSyncReceive;
	OsWaitOnEnable();
	
	++receivePort;
	receiveInstance = receivePort,
	
	rxTsQueue[receivePort] = OsCreateQue(4, sizeof(EDD_RQB_NRT_RECV_TYPE*));
	xQueueReset(rxTsQueue[receiveInstance]);
	
	while (1)
	{ /* handle the received frame-type */
		RSyncReceive  = (EDD_RQB_NRT_RECV_TYPE*) OsQueRecv(rxTsQueue[receiveInstance], 0xffffffffUL);
		uint16_t port = RSyncReceive->PortID - 1;
	
		if(PORT_ENABLED == gPTPd[port].enabled)
		{
			if((0UL < (pHandle->dwLinkStatusAll & (1 << port))))
			{
				// Copy MAC addresses
				memcpy(frameBuffer[port], RSyncReceive->pBuffer, 12);
				// Copy remainder after MGMTTAG, RequestCnt is frame size without MGMTTAG already
				memcpy(&frameBuffer[port][12], &RSyncReceive->pBuffer[12+8], RSyncReceive->RequestCnt-12);
				
				gPTPd[port].rxBuf = (uint8_t*)frameBuffer[port];
				gPTPd[port].RSyncReceive.RxTime = RSyncReceive->RxTime;
				gPTPd[port].RSyncReceiveAddr    = RSyncReceive;
				
				gptp_handleEvent(gptp_parseMsg(port), port);
			}
		}
	}
}

void eventLoop(struct gPTPd * gPTPd)
{
	u64 currTickTS = gptp_getCurrMilliSecTS();

	for(int i = 0; i < GPTP_NUM_TIMERS; i++)
	{
		if (gPTPd->timers[i].timeInterval > 0)
		{ 			
			/* When the requested time elapsed for this timer */
			if((u64)((u64)gPTPd->timers[i].lastTS + (u64)gPTPd->timers[i].timeInterval) < currTickTS)
			{
				/* if eth-Link is down then only process sync timeout event: GPTP_EVT_CS_SYNC_TO.
					 to disable CyclicIO, otherwise we get a DHT expire after re-plug on every  
					 class3 frame, and we are unable to go into sync-state again.
				*/
				if( (0UL < (pHandle->dwLinkStatusAll & (1 << gPTPd->port))) || 
					  (GPTP_EVT_CS_SYNC_TO == gPTPd->timers[i].timerEvt)      ||
            (GPTP_EVT_CS_SYNC_TAKEOVER == gPTPd->timers[i].timerEvt))
				{ 
					/* restart port SM after cable un-plug/re-plug */
					if(1 == gPTPd->portDown)
					{
						gPTPd->portDown = 0;
						gptp_init(gPTPd->port);
						
						/* Start the state machines */
						dmHandleEvent (gPTPd, GPTP_EVT_DM_ENABLE );
						bmcHandleEvent(gPTPd, GPTP_EVT_BMC_ENABLE);
						csHandleEvent (gPTPd, GPTP_EVT_CS_ENABLE );
					}
					
					/* Update and handle the timer event */
					gPTPd->timers[i].lastTS = currTickTS;
					gptp_handleEvent(gPTPd->timers[i].timerEvt, gPTPd->port);
				}				
			}
		}
	}
}

void gptp_init(uint16_t port)
{		
	/* port number of this port instance */
	gPTPd[port].port = port;
	/* only used if we are master, 
     as device we will never be master */
	gPTPd[port].isMasterPort  = 0;
	/* restart delay measurement averaging */
	gPTPd[port].cableDelay    = 0;
	gPTPd[port].DelayDriftAdjustDone    = 0;
	
	/* reset all frame sequence number */
	gPTPd[port].dm.rxSeqNo    = 0;
	gPTPd[port].dm.txSeqNo    = 0;
	gPTPd[port].bmc.annoSeqNo = 0;
	gPTPd[port].cs.syncSeqNo  = 0;
	
	/* Initialize modules (SM) */
	initDM (&gPTPd[port]);
	initBMC(&gPTPd[port]);
	initCS (&gPTPd[port]);
	
	/* Init the state machines */
	dmHandleEvent (&gPTPd[port], GPTP_EVT_STATE_ENTRY);
	bmcHandleEvent(&gPTPd[port], GPTP_EVT_STATE_ENTRY);
	
	clear_drift_reg(1);  
}

/*
	TSN differs from 1588 hardware (hw timestamping) in that TSN networks will 
	generate an error if synchronization falls out of expected bounds.

	And that TSN timing packets have priority scheduling.
*/
void gptp_start(void)
{
	extern ether_instance_ctrl_t ether_ctrl;
	extern LSA_VOID_PTR_TYPE  EDDP_LL_MNG_HANDLE(LSA_UINT16 InterfaceID);
	
	EDDP_LL_HANDLE_PTR_TYPE pHandle = (EDDP_LL_HANDLE_PTR_TYPE)EDDP_LL_MNG_HANDLE(1);
	
	// Wait until switch is configured by network manager
	while ((ether_ctrl.open != ETHER_OPEN) || (0UL == pHandle->dwLinkStatusAll)) 
	{
		vTaskDelay (100);
	}
}

int gptp_parseMsg(uint16_t port)
{
		int evt = GPTP_EVT_NONE;
		struct ethhdr*  eh = (struct ethhdr *) gPTPd[port].rxBuf;
		
#ifdef GPTP_PROTO
		struct gPTPHdr* gh = (struct gPTPHdr *)&gPTPd[port].rxBuf[sizeof(struct ethhdr)];

		if(eh->h_proto == htons((ETH_P_1588)))
		{
			  switch(gh->h.f.b1.msgType & 0x0f)
#else
		RSYNC_PTCP * gh = (RSYNC_PTCP*)&gPTPd[port].rxBuf[0];
		
		if(eh->h_proto == htons(ETHERTYPE)) 
		{
				uint16_t FrameId = htons(gh->PduHdr.FrameId.Word);

			  if (((GSY_FRAMEID_SYNC_MIN   <= FrameId) && (GSY_FRAMEID_SYNC_MAX   >= FrameId))
				 || ((GSY_FRAMEID_FUSYNC_MIN <= FrameId) && (GSY_FRAMEID_FUSYNC_MAX >= FrameId))
				 || ((GSY_FRAMEID_FU_MIN     <= FrameId) && (GSY_FRAMEID_FU_MAX     >= FrameId)))
				{
					if(GSY_SYNCID_NONE == gPTPd[ANY_PORT].pChUsr->SyncId)
					{
						return GPTP_EVT_NONE; // (no further sync processing)
					}
					
					gPTPd[port].syncID = FrameId & PTCP_SYNCID_MASK;
					MAC_ADR_TYPE tmpMasterMAC = ((RSYNC_PTCP*)gPTPd[port].rxBuf)->PtcpData.Sync.TLV.Subdomain.MasterSourceAddress;
					
					if(0 != memcmp((const void *)&MasterMAC, (const void *)&tmpMasterMAC, MAC_ADR_LENGTH))
					{
						if (GSY_MACADDR_IS_NULL(MasterMAC))
						{
							MasterMAC = tmpMasterMAC;
							gPTPd[port].masterMAC = tmpMasterMAC;

							/* SyncMaster TakeoverTieout Start */
							reloadCSTimeout(&gPTPd[port]);
							gptp_startTimer(&gPTPd[port], GPTP_TIMER_SYNC_TAKEOVER, gPTPd[port].cs.syncTakeoverTimeout , GPTP_EVT_CS_SYNC_TAKEOVER);
							setLLDPMasterMAC(GSY_DIAG_SOURCE_SUBDOMAIN,&gPTPd[port], 1);

							/* insert master-MAC into prepared buffer */
							for(uint32_t port = 0; port < NUM_PORTS; ++port)
							{
								memcpy(&((RSYNC_PTCP*)&gPTPd[port].txBufSyFu[0])->PtcpData.SyncFu.Subdomain.MasterSourceAddress, &tmpMasterMAC, MAC_ADR_LENGTH);
							}
						}
						else
						{
							return GPTP_EVT_NONE; // (no further sync processing)
						}
					}
					
					if (0 != memcmp(&((RSYNC_PTCP*)gPTPd[port].rxBuf)->PtcpData.Sync.TLV.Subdomain.SubdomainUUID, &subdomainUUID, UUID_LENGTH))
					{
						if (GSY_SUBDOMAIN_IS_NULL(subdomainUUID))
						{
							memcpy(&subdomainUUID, &((RSYNC_PTCP*)gPTPd[port].rxBuf)->PtcpData.SyncFu.Subdomain.SubdomainUUID, UUID_LENGTH);
						}
						else
						{
							return GPTP_EVT_NONE; // (no further sync processing);
						}
					}

					/* Reduce intervall to single value (indicator) */
					if((GSY_FRAMEID_SYNC_MIN <= FrameId) && (GSY_FRAMEID_SYNC_MAX >= FrameId))
					{ // One-Step
					  FrameId = GPTP_MSG_TYPE_SYNC;
					}
					
					else
						
					if (((GSY_FRAMEID_FUSYNC_MIN <= FrameId) && (GSY_FRAMEID_FUSYNC_MAX >= FrameId)))
					{ // Two-Step
						FrameId = GPTP_MSG_TYPE_SYNCFU;
					}

					else

					if (((GSY_FRAMEID_FU_MIN <= FrameId) && (GSY_FRAMEID_FU_MAX >= FrameId)))
					{ // Two-Step
						FrameId = GPTP_MSG_TYPE_SYNC_FLWUP;
					}
					
					uint16_t synSeqId = htons(((RSYNC_PTCP*)gPTPd[port].rxBuf)->PtcpData.Sync.PtcpHdr.SequenceID.Word);
					
					if(0xffff == gSyncPort)
					{
						gSyncPort = port;
						gPTPd[gSyncPort].cs.syncSeqNo = synSeqId - 1;
					}
					else
					{
						/* ingoring MRP for now */
						if(gSyncPort != port)
						{
							return GPTP_EVT_NONE; // (no further sync processing)
						}
					}
					/* SyncMaster TakeoverTieout restart */
					gptp_resetTimer(&gPTPd[gSyncPort], GPTP_TIMER_SYNC_TAKEOVER);
					
					/* check if sync sequence id is in expeceted range */
					if( (synSeqId > gPTPd[gSyncPort].cs.syncSeqNo) ||
							(gPTPd[gSyncPort].cs.syncSeqNo == 0xffff ) 
						)
					{
						gPTPd[gSyncPort].cs.syncSeqNo = synSeqId;
					}
					else
					{
						if(FrameId != GPTP_MSG_TYPE_SYNC_FLWUP)
						{
							return GPTP_EVT_NONE; // (no further sync processing)
						}
					}
				}
				else
				{
					FrameId = htons(FrameId); //TODO:-OS- not nice to do it twice, refine!
				}
				
 			  switch(FrameId)
#endif
				{
						case FRAMEID_DELAY_REQ:
						case GPTP_MSG_TYPE_PDELAY_REQ:
#ifdef GPTP_PROTO
										memcpy(&gPTPd[port].dm.reqPortIden[0], &gh->h.f.srcPortIden[0], GPTP_PORT_IDEN_LEN);
#endif
										evt = GPTP_EVT_DM_PDELAY_REQ;
										break;

						case FRAMEID_DELAY_RSP:
										evt = GPTP_EVT_DM_PDELAY_RESP;
										break;

						case FRAMEID_DELAY_FURSP:
						case GPTP_MSG_TYPE_PDELAY_RESP:
										evt = GPTP_EVT_DM_PDELAY_FURESP;
										break;

						case FRAMEID_DELAY_FU:  
						case GPTP_MSG_TYPE_PDELAY_RESP_FLWUP:
										evt = GPTP_EVT_DM_PDELAY_RESP_FLWUP;
										break;

						case GPTP_MSG_TYPE_ANNOUNCE:
										evt = GPTP_EVT_BMC_ANNOUNCE_MSG;
										break;

						case FRAMEID_SYNC:
						case GPTP_MSG_TYPE_SYNC:
										evt = GPTP_EVT_CS_SYNC_MSG;
										break;

						case GPTP_MSG_TYPE_SYNCFU:
										evt = GPTP_EVT_CS_SYNCFU_MSG;
										break;

						case GPTP_MSG_TYPE_SYNC_FLWUP:
										evt = GPTP_EVT_CS_SYNC_FLWUP_MSG;
										break;

						default:
										break;
				}
		};

		return evt;
}

void gptp_handleEvent(int evt, uint16_t port)
{		
	if (evt != GPTP_EVT_NONE) 
	{
			switch(evt & GPTP_EVT_DEST_MASK) 
			{
					case GPTP_EVT_DEST_DM:
									dmHandleEvent(&gPTPd[port], evt);
									break;
									
					case GPTP_EVT_DEST_BMC:
									bmcHandleEvent(&gPTPd[port], evt);
									break;
									
					case GPTP_EVT_DEST_CS:
									csHandleEvent(&gPTPd[port], evt);
									break;
									
					default:
									break;
			}
	}
}

void startPTP (GSY_CH_USR_PTR	pChUsr)
{
	extern void * syncFollowUpQueue;
	extern void * dlRsQueue[NUM_PORTS];
	
	if(0 == rsyncRunning)
	{
		rsyncRunning = 1;
		syncFollowUpQueue = OsCreateQue(10, sizeof(EDD_RQB_NRT_RECV_TYPE*));
		
		for(uint32_t port = 0; port < NUM_PORTS; ++port)
		{
			/* interface to PRM data */
			gPTPd[port].pChUsr = pChUsr;
			/* prevent port buffer overwrite */
			
			const uint8_t cMAX_FRAMETYPES = 6;
			
			for(int queue = 0; queue < cMAX_FRAMETYPES; ++queue)
			{
				gPTPd[port].txTsQueue[queue] = OsCreateQue(1, sizeof(EDD_RQB_NRT_SEND_TYPE*));
			}
			
			/* Queue for postponed handling of delay response frames */
			dlRsQueue[port] = OsCreateQue(1, sizeof(EDD_RQB_NRT_RECV_TYPE*));
		}
		
		uint32_t Status = OsCreateThread ((void  (*)(void))syncFollowUpTask, (PNIO_UINT8*)"syncFW", 29 /* prio */, &pThreadIdSync);
		
		if(PNIO_OK == Status)
		{
			Status = OsCreateMsgQueue (pThreadIdSync);
		}
		
		if(PNIO_OK == Status)
		{
			Status = OsStartThread  (pThreadIdSync);
		}
		
		for(int port = 0; port < NUM_PORTS; ++ port)
		{
			if(PNIO_OK == Status)
			{
				Status = OsCreateThread ((void  (*)(void))PTPReceive, (PNIO_UINT8*)"RSyncReceive", 30 /* prio */, &pThreadIdRdeceive[port]);
				
				if(PNIO_OK == Status)
				{
					Status = OsCreateMsgQueue (pThreadIdRdeceive[port]);
				}
				
				if(PNIO_OK == Status)
				{
					Status = OsStartThread (pThreadIdRdeceive[port]);
				}
			}
		}
		
		if(PNIO_OK == Status)
		{
			Status = OsCreateThread ((void  (*)(void))PTPSend, (PNIO_UINT8*)"RSyncSend", 26 /* prio */, &pThreadIdSend);
		}
		
		if(PNIO_OK == Status)
		{
			Status = OsCreateMsgQueue (pThreadIdSend);
		}
		
		if(PNIO_OK == Status)
		{
			Status = OsStartThread  (pThreadIdSend);
		}
	}
	else
	{
		if(GSY_SYNC_ROLE_SLAVE == pChUsr->SyncRole)
		{						
			/* prepare send frames () */
			for(uint32_t port = 0; port < NUM_PORTS; ++port)
			{
				/* Initialize tx */
				gptp_initTxBuf(&gPTPd[port]);
				/* new PRM data set- received go out of sync */
				gptp_clearMasterMAC(&gPTPd[port]);
				
				if(GSY_DIAG_SYNCID_SYNCHRONIZED == gPTPd[port].SyncState)
				{
					syncChanged(&gPTPd[port], GSY_DIAG_SYNCID_NOT_SYNCHRONIZED);
          #ifdef PrintfDebug
																	printeth("Sync_Lost - New PRM");
          #endif

				}
			}
		}
	}
}
