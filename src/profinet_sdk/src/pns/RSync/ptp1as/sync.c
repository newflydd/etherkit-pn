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
#include "pnorsyncif.h"
#include "delaymsr.h"
#include "sync.h"
#include "r_ether_switch_rzt2_ext.h"
#include "ecos_ertec_os_utils.h"
#include "PTCP.h"
#include "eddp_rzt2.h"
/* For syslog frames */
#include "debug.h"
/* for abs() */
#include "stdlib.h"

#ifdef PrintfDebug
  #include "printf.h"
#endif

static struct timespec syncDelay[4];
void* syncFollowUpQueue;

#ifdef RSyncDebug
long long syncDelaySec;
long      syncDelayNs;
#endif

// Initial max deviation of clock before restart initiated
static uint32_t  window_size  = 2000;       // 2* 1000 as we have 60ns nbetween update intervals
static uint8_t   unsync_cnt   = 0;
static uint8_t   adjust_cycle = 1;
static int       diffsign     = 1;    // If negative we are behind

void initCS(struct gPTPd* gPTPd)
{
	gPTPd->cs.state = CS_STATE_INIT;

	reloadCSTimeout(gPTPd);
}

void reloadCSTimeout(struct gPTPd* gPTPd)
{
	uint32_t SyncInterval;
	uint32_t SyncTimeoutFactor;
	uint32_t TakeoverTimeoutFactor;

	// Get SyncInterval
	SyncInterval = gPTPd->pChUsr->Slave.SyncInterval != 0 ? gPTPd->pChUsr->Slave.SyncInterval : GPTP_SYNC_INTERVAL;

	// Get SyncTimeout Factor
	SyncTimeoutFactor = gPTPd->pChUsr->Slave.SyncTimeout != GSY_SYNC_TIMEOUT_DISABLED ? gPTPd->pChUsr->Slave.SyncTimeout : GPTP_SYNC_TIMEOUT_FACTOR;

	// Get SyncTakeoverTimeout Factor
	TakeoverTimeoutFactor = gPTPd->pChUsr->Slave.SyncTakeover != GSY_SYNC_TAKEOVER_DISABLED ? gPTPd->pChUsr->Slave.SyncTakeover : GPTP_TAKEOVER_TIMEOUT_FACTOR;

	// Sync timeout setup
	gPTPd->cs.syncInterval			= SyncInterval;
	gPTPd->cs.syncTimeout 			= SyncTimeoutFactor * SyncInterval;
	gPTPd->cs.syncTakeoverTimeout	= TakeoverTimeoutFactor * SyncInterval;
}

void csSetState(struct gPTPd* gPTPd, bool gmMaster)
{
	if((gmMaster == TRUE) && (gPTPd->cs.state != CS_STATE_GRAND_MASTER))
	{
		csHandleStateChange(gPTPd, CS_STATE_GRAND_MASTER);
	}
	else if((gmMaster == FALSE) && (gPTPd->cs.state != CS_STATE_SLAVE))
	{
		csHandleStateChange(gPTPd, CS_STATE_SLAVE);
	}
}

uint32_t count_clock_restart = 0; 
uint32_t diff_last = 0; 

void csHandleEvent(struct gPTPd* gPTPd, int evtId)
{
	extern PNIO_UINT32 tsWait;
	struct timespec sync[10];
	struct timespec duration;

	/* for degugging only */
	int diffsign_del = 1;
	LSA_UNUSED_ARG( diffsign_del );

	switch(gPTPd->cs.state) 
	{
		case CS_STATE_INIT:
						switch (evtId)
						{
								case GPTP_EVT_STATE_ENTRY:
										break;
								case GPTP_EVT_CS_ENABLE:
										break;
								case GPTP_EVT_STATE_EXIT:
										break;
								default:
										break;
							}
							break;

		case CS_STATE_GRAND_MASTER:
						switch (evtId) 
						{
										case GPTP_EVT_STATE_ENTRY:
														gptp_startTimer(gPTPd, GPTP_TIMER_SYNC_RPT, gPTPd->cs.syncInterval, GPTP_EVT_CS_SYNC_RPT);
														break;
										case GPTP_EVT_CS_SYNC_RPT:
#ifdef GPTP_PROTO
														if(R_OK == sendSync(gPTPd))
														{
															getTxTS(gPTPd, &gPTPd->ts[6], 2);
															sendSyncFlwup(gPTPd);
														}
#else
														if(MASTER_STATE_PRIMARY == gPTPd->pChUsr->Master.State)
														{
															if(R_OK == sendSync(gPTPd))
															{
																getTxTS(gPTPd, &gPTPd->ts[6], 2);
																sendSyncFlwup(gPTPd);
															}
														}
#endif
														break;
										case GPTP_EVT_STATE_EXIT:
														gptp_stopTimer(gPTPd, GPTP_TIMER_SYNC_RPT);
														break;
										default:
														break;
						}
						break;

		case CS_STATE_SLAVE:
									switch (evtId) {
										case GPTP_EVT_STATE_ENTRY:
													gptp_startTimer(gPTPd, GPTP_TIMER_SYNC_TO, gPTPd->cs.syncTimeout, GPTP_EVT_CS_SYNC_TO);
													break;

									  case GPTP_EVT_CS_SYNCFU_MSG:
										case GPTP_EVT_CS_SYNC_MSG:
														getRxTS(gPTPd, &gPTPd->ts[RX_SYNC_FROM_MASTER]);
														
                            /* Delay Time in Sync Frame */
														sync[MASTER_DELAY].tv_sec   = 0; 
														sync[MASTER_DELAY].tv_nsec  = SWAP32(((RSYNC_PTCP*)gPTPd->rxBuf)->PtcpData.Sync.PtcpHdr.Delay10ns.Dword) * 10;
														sync[MASTER_DELAY].tv_nsec += ((RSYNC_PTCP*)gPTPd->rxBuf)->PtcpData.Sync.PtcpHdr.Delay1ns;
														sync[MASTER_DELAY].tv_nsec += SWAP32(((RSYNC_PTCP*)gPTPd->rxBuf)->PtcpData.Sync.PtcpHdr.DelayNS.Dword);
                            
														gPTPd->ts[SYN_DELAY_SUM].tv_sec  = 0; 
						                gPTPd->ts[SYN_DELAY_SUM].tv_nsec = sync[MASTER_DELAY].tv_nsec;

														if (GPTP_EVT_CS_SYNCFU_MSG == evtId)
														{ // Two-Step
															gPTPd->twoStep = 1;
															memcpy(gPTPd->txBufSync, gPTPd->rxBuf, SYNC_PDU_SIZE);
															forwardSync(gPTPd, gPTPd->txBufSync, 1UL);
															break;
														}
														
														gPTPd->twoStep = 0;

										case GPTP_EVT_CS_SYNC_FLWUP_MSG:
														{
															// Received timestamp into ts[RX_SYNC_FU_FROM_MASTER].
#ifdef GPTP_PROTO
															gptp_copyTSFromBuf(&gPTPd->ts[RX_SYNC_FU_FROM_MASTER], &gPTPd->rxBuf[(GPTP_BODY_OFFSET)]);
#else
															if(GPTP_EVT_CS_SYNC_FLWUP_MSG == evtId)
															{
																// Two-Step: Calculate Time of transmissionm (Master Time in Sync Frame)
																gPTPd->ts[RX_SYNC_FU_FROM_MASTER].tv_sec  = SWAP32(((RSYNC_PTCP*)gPTPd->rxBuf)->PtcpData.SyncFu.Time.Seconds.Dword);
																gPTPd->ts[RX_SYNC_FU_FROM_MASTER].tv_nsec = SWAP32(((RSYNC_PTCP*)gPTPd->rxBuf)->PtcpData.SyncFu.Time.NanoSeconds.Dword);
															}
															else
															{
																// One-Step: Calculate Time of transmissionm (Master Time in Sync Frame)
																gPTPd->ts[RX_SYNC_FU_FROM_MASTER].tv_sec  = SWAP32(((RSYNC_PTCP*)gPTPd->rxBuf)->PtcpData.Sync.TLV.Time.Seconds.Dword);
																gPTPd->ts[RX_SYNC_FU_FROM_MASTER].tv_nsec = SWAP32(((RSYNC_PTCP*)gPTPd->rxBuf)->PtcpData.Sync.TLV.Time.NanoSeconds.Dword);
															}
															
															/* preliminary fix for MTIP wraparound bug */
															if(gPTPd->ts[RX_SYNC_FU_FROM_MASTER].tv_nsec >= 999000000UL)
															{
																if(GPTP_EVT_CS_SYNC_FLWUP_MSG == evtId)
																{
																	/* two step, send followUp */
																	memcpy(gPTPd->txBufSyFu, gPTPd->rxBuf, SYNC_FU_PDU_SIZE);
																	forwardSync(gPTPd, gPTPd->txBufSyFu, 0L);
																}
																else
																{
																	/* one step, convert to sync and followUp */
																	forwardSync(gPTPd, gPTPd->rxBuf, 1L);
																}
															 
															 /* reset sync timeout(TO) timer */
															 gptp_resetTimer(gPTPd, GPTP_TIMER_SYNC_TO);
															 break;
															}
															
															if(GPTP_EVT_CS_SYNC_FLWUP_MSG == evtId)
															{
																/* Delay Time in Follow Up Frame */
																sync[MASTER_DELAY].tv_sec   = gPTPd->ts[SYN_DELAY_SUM].tv_sec;
																sync[MASTER_DELAY].tv_nsec  = gPTPd->ts[SYN_DELAY_SUM].tv_nsec;
																sync[MASTER_DELAY].tv_nsec += SWAP32(((RSYNC_PTCP*)gPTPd->rxBuf)->PtcpData.SyncFu.PtcpHdr.Delay10ns.Dword) * 10;
																sync[MASTER_DELAY].tv_nsec += ((RSYNC_PTCP*)gPTPd->rxBuf)->PtcpData.SyncFu.PtcpHdr.Delay1ns;
																sync[MASTER_DELAY].tv_nsec += SWAP32(((RSYNC_PTCP*)gPTPd->rxBuf)->PtcpData.SyncFu.PtcpHdr.DelayNS.Dword);
															}
															
#endif
															uint32_t phydelays = gPTPd->RealPortRxDelay + gPTPd->pChUsr->pChSys->Port[gPTPd->port].CableDelay + gPTPd->remoteTXDelay;
															
															// Actual target time (sollankunftszeit)  for arrival of sync into sync[0].
															sync[MASTER_DELAY].tv_nsec += (phydelays);
															
															// Real Time at which the sync frame arrived at RX port based on master time.  
															gptp_timespec_sum(&gPTPd->ts[RX_SYNC_FU_FROM_MASTER],&sync[MASTER_DELAY],&sync[TX_MASTER_SENT_TIME]);
															
															// Own time -> ts[LOCALTIME].
															clock_gettime(ETHSW_TIMER0, &gPTPd->ts[LOCALTIME]);  
															
															// Create local copy of RX Timestamp which includes the seconds value															
															if (gPTPd->ts[RX_SYNC_FROM_MASTER].tv_nsec < gPTPd->ts[LOCALTIME].tv_nsec) // RX timestamp should be smaller than local time
															{
																sync[TS_RX_LOCAL_TIME].tv_sec = gPTPd->ts[LOCALTIME].tv_sec;
															}
															else
															{ // Overflow
																sync[TS_RX_LOCAL_TIME].tv_sec = gPTPd->ts[LOCALTIME].tv_sec - 1;
															}
															
															sync[TS_RX_LOCAL_TIME].tv_nsec = gPTPd->ts[RX_SYNC_FROM_MASTER].tv_nsec;   // RX Timestamp at port															
															// Clock deviation still needs to add phy delay and cable delay
															diffsign = gptp_timespec_absdiff(&sync[TX_MASTER_SENT_TIME],&sync[TS_RX_LOCAL_TIME],&sync[OFFSET_TO_MASTER]);
															
															// deviation between clocks more than allowed window.
															if(adjust_cycle && (count_clock_restart == 0) && (((sync[OFFSET_TO_MASTER].tv_sec > 0) || (sync[OFFSET_TO_MASTER].tv_nsec > window_size))))
															{
																count_clock_restart = 10;       
																// Time for the frame from RX TS to here 
																OsIntDisable();
																gptp_timespec_diff(&sync[TS_RX_LOCAL_TIME],&gPTPd->ts[LOCALTIME],&sync[DIFF_RX_TS_CURRENT_TIME]);
																// arrivalttime@master + residencetime -> sync[3] = Current time based on master clock.
																gptp_timespec_sum(&sync[DIFF_RX_TS_CURRENT_TIME],&sync[TX_MASTER_SENT_TIME],&sync[TARGET_ACTUAL_TIME]);
																
																// set clock hard to time in sync frame plus residence time.
																// *** Empirically evaluated for execution of setslave ***
																sync[TARGET_ACTUAL_TIME].tv_nsec += 5000;
																
																if (sync[TARGET_ACTUAL_TIME].tv_nsec > 1000000000)
																{
																	sync[TARGET_ACTUAL_TIME].tv_nsec -= 1000000000;
																	sync[TARGET_ACTUAL_TIME].tv_sec  += 1;
																}

																if (sync[TARGET_ACTUAL_TIME].tv_nsec > 999999900)  // Correct for Hardware Bug where writing a value close to ATIME_EVT_PERIOD can cause the time to miss a second 
                                  
																{
																	sync[TARGET_ACTUAL_TIME].tv_nsec = 999999900;  // We are a bit off, will be corrected in next cycle
																}
																
																R_ETHSW_Type  * p_reg_ethsw = (R_ETHSW_Type *)R_ETHSW_BASE;
																p_reg_ethsw->TDMA_CONFIG_b.TDMA_ENA = 0 ;
																clock_settime(ETHSW_TIMER0, &sync[TARGET_ACTUAL_TIME]);

																OsIntEnable();
																if (gPTPd->SyncState == GSY_DIAG_SYNCID_SYNCHRONIZED)
																{
																	syncChanged(gPTPd, GSY_DIAG_SYNCID_NOT_SYNCHRONIZED);
#ifdef PrintfDebug
																	printeth("Sync_Lost - Windowsize");
#endif
																}
																// Restart the TDMA controller after setting timer . Here we do not take the phases into account to restart 
																// quickly, thus the update_tdma is set to handle that during the green interrupt
																uint32_t curr_time = sync[TARGET_ACTUAL_TIME].tv_nsec; 
																uint32_t interval_start = curr_time - (curr_time % (p_reg_ethsw->TDMA_CYCLE));    // elspsed time since last cycle start 
																interval_start += (p_reg_ethsw->TDMA_CYCLE);  // Set to next interval start
																
																if (interval_start > 1000000000) 
																{
																	interval_start -= 1000000000; 
																}
																
																// Used to handle a new cycle time or TDMA setting in the red phasejust before the new cycle starts
																extern tdma_update_data_t tdma_update_data; 
																
																p_reg_ethsw->TDMA_START = interval_start;        // Starttime at next next interval start + adju
																p_reg_ethsw->TDMA_CONFIG_b.TDMA_ENA = 1 ;
																tdma_update_data.update_tdma = 1;

																gPTPd->max_sync_diff_adj = 50000;
																gPTPd->max_sync_drift_adj = 20000;
#ifdef PrintfDebug
																char text_string[20];
																sprintf(&text_string[0], "Set Clock %ld", sync[OFFSET_TO_MASTER].tv_nsec);
																printeth(text_string);
#endif
																adjust_cycle = 0;
															}
															else
															{ // finetune.
																if(count_clock_restart > 0) 
																{
																	count_clock_restart--;
																}
											
																// Only adjust every second cycle to let the clock settle.
																if (adjust_cycle)
																{
                                  // Time between sync messages (Drift). 
                                  gptp_timespec_diff(&syncDelay[TX_MASTER_SENT_TIME], &sync[TX_MASTER_SENT_TIME], &duration );
																	// Correct Drift.
																	if (syncDelay[OFFSET_TO_MASTER].tv_nsec)  
																	{
																		// Clockdrift in sync[CLOCKDRIFT]
																		sync[CLOCKDRIFT].tv_nsec = diffsign * sync[OFFSET_TO_MASTER].tv_nsec - syncDelay[OFFSET_TO_MASTER].tv_nsec + syncDelay[DIFFCORRVAL].tv_nsec;
																		int8_t diffsign_drift = 1;
#if PrintfDebug
																		if ((sync[CLOCKDRIFT].tv_nsec > 25) || (sync[OFFSET_TO_MASTER].tv_nsec > 25))
                                    {
                                      char text_string[20];
                                      sprintf(&text_string[0], "Diff %ld %ld", sync[CLOCKDRIFT].tv_nsec, diffsign * sync[OFFSET_TO_MASTER].tv_nsec); 
                                      printeth(text_string);
                                    }
#endif
																		
																		if (sync[CLOCKDRIFT].tv_nsec < 0) 
																		{
																			diffsign_drift = -1; 
																			sync[CLOCKDRIFT].tv_nsec = abs(sync[CLOCKDRIFT].tv_nsec);
																		}
																		
																		if ((sync[CLOCKDRIFT].tv_nsec > gPTPd->max_sync_drift_adj) && (count_clock_restart < 8))  //
																		{
																			sync[CLOCKDRIFT].tv_nsec = gPTPd->max_sync_drift_adj;
																		}
																		else if (sync[CLOCKDRIFT].tv_nsec < 100)
																		{
																			sync[CLOCKDRIFT].tv_nsec = sync[CLOCKDRIFT].tv_nsec / 4; 
																		}
																		
                                    if (sync[CLOCKDRIFT].tv_nsec > 20000)   // Seems like the master clock is stopped or something is severely wrong, reset drift correction  
                                    {
                                      clear_drift_reg(0); 
                                    }
                                    else
                                    {
																		 clock_adjdrift((-1)*diffsign_drift, sync[CLOCKDRIFT].tv_nsec, duration.tv_nsec, 0);
																		}
																		
                                    gPTPd->act_sync_drift_adj = sync[CLOCKDRIFT].tv_nsec;   // absolute value
																		
																		if ((sync[CLOCKDRIFT].tv_nsec < 20) && (sync[OFFSET_TO_MASTER].tv_nsec < 100) && (gPTPd->SyncState == GSY_DIAG_SYNCID_NOT_SYNCHRONIZED)) 
																		{
																			syncChanged(gPTPd, GSY_DIAG_SYNCID_SYNCHRONIZED);
																			gPTPd->max_sync_drift_adj = 70;  // Empirically evaluated to pass SPIRTA syncslave03 test (combination of resistance to peaks and following of drifts)
																			gPTPd->max_sync_diff_adj = 300;
#ifdef PrintfDebug
																			//app_printf("Synced\n");
																			printeth("Synced");
#endif
																			unsync_cnt = 0; 
																		}
																		
																		//reset to 1000, spirta test
																		if (((sync[CLOCKDRIFT].tv_nsec > 400) || (sync[OFFSET_TO_MASTER].tv_nsec > 100)) && (unsync_cnt++ > 2) && (gPTPd->SyncState == GSY_DIAG_SYNCID_SYNCHRONIZED) ) 
																		{
																			syncChanged(gPTPd, GSY_DIAG_SYNCID_NOT_SYNCHRONIZED);
#ifdef PrintfDebug
																		//	app_printf("Sync lost\n");
																			printeth("Sync Lost");
#endif
																		}
																		else 
																		{
																			unsync_cnt = 0; 
																		}
																	}

																	
																	adjust_cycle = 0;
																	diffsign_del = diffsign;
#ifdef PrintfDebug
																	syncDelayNs  = sync[OFFSET_TO_MASTER].tv_nsec;
																	syncDelaySec = (s64)sync[OFFSET_TO_MASTER].tv_sec;
#endif
																} // adjust_cycle
																else
																{
																	adjust_cycle = 1;
																}
																
																int32_t diffcorrval = 0;
																
																if ((sync[OFFSET_TO_MASTER].tv_nsec > gPTPd->max_sync_diff_adj) && (count_clock_restart < 9))
																{
																	clock_adjtime(-1*diffsign, (uint32_t) gPTPd->max_sync_diff_adj, 15000000);
																	diffcorrval = diffsign * gPTPd->max_sync_diff_adj; 
																}
																else
																{
																	clock_adjtime(-1*diffsign, (uint32_t) sync[OFFSET_TO_MASTER].tv_nsec , 2000000);
																	diffcorrval = diffsign * sync[OFFSET_TO_MASTER].tv_nsec; 
																}
																/* Correction not finished before next cycle */
																/* Store last offset value for drift correction */
																syncDelay[OFFSET_TO_MASTER].tv_sec  = sync[OFFSET_TO_MASTER].tv_sec;
																syncDelay[OFFSET_TO_MASTER].tv_nsec = diffsign * sync[OFFSET_TO_MASTER].tv_nsec;
																diff_last = diffsign * sync[OFFSET_TO_MASTER].tv_nsec;
																syncDelay[DIFFCORRVAL].tv_nsec = diffcorrval; 
																/* Store last sync time for drift correction to calculate interval*/
																syncDelay[TX_MASTER_SENT_TIME].tv_sec  = sync[TX_MASTER_SENT_TIME].tv_sec;
																syncDelay[TX_MASTER_SENT_TIME].tv_nsec = sync[TX_MASTER_SENT_TIME].tv_nsec;
																syncDelay[TARGET_ACTUAL_TIME].tv_sec  = gPTPd->ts[LOCALTIME].tv_sec;
																syncDelay[TARGET_ACTUAL_TIME].tv_nsec = gPTPd->ts[LOCALTIME].tv_nsec;
                                  
#ifdef PrintfDebug
																// app_printf("Diffna %d %d %d\n", diffsign, sync[OFFSET_TO_MASTER].tv_nsec, syncDelay[OFFSET_TO_MASTER].tv_nsec);
#endif
															} //finetune
															
															if(GPTP_EVT_CS_SYNC_FLWUP_MSG == evtId)
															{	/* two step, send followUp */
																memcpy(gPTPd->txBufSyFu, gPTPd->rxBuf, SYNC_FU_PDU_SIZE);
																forwardSync(gPTPd, gPTPd->txBufSyFu, 0L);
															}
															else
															{
																/* one step, convert to sync and followUp */
																forwardSync(gPTPd, gPTPd->rxBuf, 1L);
															}
														}
														
														gptp_resetTimer(gPTPd, GPTP_TIMER_SYNC_TO);
														
													break;

										case GPTP_EVT_CS_SYNC_TO:
													gptp_stopTimer(gPTPd, GPTP_TIMER_SYNC_TO);
													gptp_clearMasterMAC(gPTPd);
											
													/* check if sync frames are still ariving, otherwise (T)ime(O)ut */
													if(gPTPd->SyncState == GSY_DIAG_SYNCID_SYNCHRONIZED)
													{
#ifdef PrintfDebug
														printeth("Timeout Sync\n"); 
#endif
														syncChanged(gPTPd, GSY_DIAG_SYNCID_NOT_SYNCHRONIZED);
													}
													break;
													
										case GPTP_EVT_CS_SYNC_TAKEOVER:
													gptp_stopTimer(gPTPd, GPTP_TIMER_SYNC_TAKEOVER);
													gptp_clearMasterMAC(gPTPd);
													setLLDPMasterMAC(GSY_DIAG_SOURCE_SUBDOMAIN,gPTPd, 1);
													break;
													
										case GPTP_EVT_STATE_EXIT:
													gptp_stopTimer(gPTPd, GPTP_TIMER_SYNC_TO);
													break;

										default:
													break;
						}
						break;

		default:
						break;
	}
}

void csHandleStateChange(struct gPTPd* gPTPd, int toState)
{
	csHandleEvent(gPTPd, GPTP_EVT_STATE_EXIT);
	gPTPd->cs.state = toState;
	csHandleEvent(gPTPd, GPTP_EVT_STATE_ENTRY);
}

#ifdef GPTP_PROTO
uint23_t sendSync(struct gPTPd* gPTPd)
{
	int paddingSize = 2;
	int txLen = sizeof(struct ethhdr);
	struct gPTPHdr *gh = (struct gPTPHdr *)&gPTPd->txBufSync[sizeof(struct ethhdr)];
	
	/* Fill gPTP header */
	gh->h.f.seqNo      = htons(gPTPd->cs.syncSeqNo);
	gh->h.f.b1.msgType = (GPTP_TRANSPORT_L2 | GPTP_MSG_TYPE_SYNC);
	gh->h.f.flags      = htons(GPTP_FLAGS_TWO_STEP);
	
	gh->h.f.ctrl      = GPTP_CONTROL_SYNC;
	gh->h.f.logMsgInt = GPTP_SYNC_INTERVAL_LOG;
	
	/* Add gPTP header size */
	txLen += sizeof(struct gPTPHdr);
	
	/* PTP body */
	txLen += GPTP_TS_LEN;
	
	/* Insert length */
	gh->h.f.msgLen = htons(txLen - sizeof(struct ethhdr));
	
	if (0 == sendto(gPTPd->txBufSync, txLen + paddingSize + 0x0e, 0, GPTP_MSG_TYPE_SYNC, &gPTPd->ts[6], gPTPd))
	{
		++gPTPd->errorSend;
		return R_FAILED;
	}

	return R_OK;
}

void sendSyncFlwup(struct gPTPd* gPTPd)
{
	int txLen = sizeof(struct ethhdr);
	struct gPTPHdr *gh = (struct gPTPHdr *)&gPTPd->txBufSyFu[sizeof(struct ethhdr)];
	struct gPTPtlv *tlv;
	struct gPTPOrgExt *orgExt;
	
	/* Fill gPTP header */
	gh->h.f.seqNo      = htons(gPTPd->cs.syncSeqNo);
	gh->h.f.b1.msgType = (GPTP_TRANSPORT_L2 | GPTP_MSG_TYPE_SYNC_FLWUP);
	gh->h.f.flags      = htons(GPTP_FLAGS_NONE);
	/* gPTP message type */
	gh->h.f.ctrl       = GPTP_CONTROL_SYNC_FLWUP;
	gh->h.f.logMsgInt  = GPTP_SYNC_INTERVAL_LOG; 
	
	/* Add gPTP header size */
	txLen += sizeof(struct gPTPHdr);
	
	/* PTP body */
	gptp_copyTSToBuf(&gPTPd->ts[6], &gPTPd->txBufSyFu[txLen]);
	txLen += GPTP_TS_LEN;
	
	/* Organization TLV */
	tlv       = (struct gPTPtlv *)&gPTPd->txBufSyFu[txLen];
	tlv->type = htons(GPTP_TLV_TYPE_ORG_EXT);
	tlv->len  = htons(sizeof(struct gPTPOrgExt));

	txLen += sizeof(struct gPTPtlv);
	orgExt = (struct gPTPOrgExt *)&gPTPd->txBufSyFu[txLen];

	orgExt->orgType[0] = 0x00;
	orgExt->orgType[1] = 0x80;
	orgExt->orgType[2] = 0xC2;

	orgExt->orgSubType[0] = 0x00;
	orgExt->orgSubType[1] = 0x00;
	orgExt->orgSubType[2] = 0x01;

	/* Insert length */
	txLen += sizeof(struct gPTPOrgExt);
	gh->h.f.msgLen = htons(txLen - sizeof(struct ethhdr));

	if (sendto(gPTPd->txBufSyFu, txLen, 0, GPTP_MSG_TYPE_SYNC_FLWUP, NULL, gPTPd) == 0)
		;
	else
		++gPTPd->cs.syncSeqNo;
}

#else
// Generate referenz time for transmission of next sync frame. Consists of current time in 
// seconds, nanoseconds at last cycle begin and timestamp since last cycle begin
void syncGet(LSA_UINT32 * pSeconds, LSA_UINT32 * pNanoSeconds, LSA_UINT32 * pTimeStamp)
{
	uint8_t  timer = 0;
	uint32_t ns    = ethsw_get_time(timer);
	*pSeconds      = ethsw_get_time_sec(timer);

	*pTimeStamp   = ns % 4000000;        // pTimeStamp is time since beginning of last cycle
	*pNanoSeconds = ns - (*pTimeStamp);  // AG   must be updated to real cycle time Time when last ccle started 
}

uint32_t sendSync(struct gPTPd* gPTPd)
{
	RSYNC_PTCP * pPdu = (RSYNC_PTCP*)&gPTPd->txBufSync[0];

	MAC_ADR_TYPE	DstAddr   = {PTCP_MULTICAST_ADDR_SYNC};
	MAC_ADR_TYPE	DstAddrFu = {PTCP_MULTICAST_ADDR_FUSYNC};
	LSA_UINT16	  FrameId;

	memset(pPdu, 0, SYNC_PDU_SIZE);

//#ifdef GSY_USE_VLAN
//	{
//		GSY_VLAN_TAG_TYPE	VlanTag = {GSY_VLAN_TAG};
//		pPdu->PduHdr.VlanTag = VlanTag;
//	}
//#endif	

	/* Read actual PTCP time with timestamp from the system
	*/
	syncGet(&gPTPd->pChUsr->Master.Seconds, &gPTPd->pChUsr->Master.Nanos, &gPTPd->pChUsr->Master.Timestamp);

	if (gPTPd->pChUsr->SyncId == GSY_SYNCID_CLOCK)
	{
		pPdu->PduHdr.DstMacAdr = DstAddr;
		FrameId = FRAMEID_SYNC | gPTPd->pChUsr->SyncId;
	}
	else
	{ /* Unsupported on RSK device */
		pPdu->PduHdr.DstMacAdr = DstAddrFu;
		FrameId = FRAMEID_FUSYNC | gPTPd->pChUsr->SyncId;
	}

	pPdu->PduHdr.FrameId.Word = FrameId;
	pPdu->PduHdr.DstMacAdr.MacAdr[5] |= gPTPd->pChUsr->SyncId;

	Bsp_GetPortMacAddr((PNIO_UINT8*)&pPdu->PduHdr.SrcMacAdr, gPTPd->port + 1);
	Bsp_GetMacAddr((PNIO_UINT8*)&gPTPd->pChUsr->pChSys->MACAddr);

	pPdu->PduHdr.EthType.Word = htons(ETHERTYPE);

	memcpy((void*)&pPdu->PtcpData.Sync.TLV.Subdomain.SubdomainUUID, (void*)&gPTPd->pChUsr->SyncData.SubdomainID, sizeof(PTCP_UUID));
	memcpy((void*)&pPdu->PtcpData.Sync.TLV.Subdomain.MasterSourceAddress, (void*)&gPTPd->pChUsr->pChSys->MACAddr, sizeof(MAC_ADR_TYPE));

	pPdu->PtcpData.Sync.TLV.Subdomain.TypeLen.Word = htons(TLV_SUBDOMAIN_TYPELEN);
	pPdu->PtcpData.Sync.TLV.Time.TypeLen.Word      = htons(TLV_TIME_TYPELEN);

	pPdu->PtcpData.Sync.TLV.TimeExt.CurrentUTCOffset.Word = htons(gPTPd->pChUsr->Master.CurrentUTCOffset);
	pPdu->PtcpData.Sync.TLV.TimeExt.TypeLen.Word          = htons(TLV_TIMEEXT_TYPELEN);
	pPdu->PtcpData.Sync.TLV.TimeExt.Flags.Word = 0;
	pPdu->PtcpData.Sync.TLV.TimeExt.Reserved1  = 0;
	pPdu->PtcpData.Sync.TLV.TimeExt.Reserved2  = 0;

	pPdu->PtcpData.Sync.TLV.Time.EpochNumber.Word  = htons(gPTPd->pChUsr->Master.EpochNumber);
	pPdu->PtcpData.Sync.TLV.Time.Seconds.Dword     = SWAP32(gPTPd->pChUsr->Master.Seconds);
	pPdu->PtcpData.Sync.TLV.Time.NanoSeconds.Dword = SWAP32(gPTPd->pChUsr->Master.Nanos);

	pPdu->PtcpData.Sync.TLV.Master.MasterPriority1    = gPTPd->pChUsr->Master.LocalMaster.Priority1;
	pPdu->PtcpData.Sync.TLV.Master.MasterPriority2    = gPTPd->pChUsr->Master.LocalMaster.Priority2;
	pPdu->PtcpData.Sync.TLV.Master.ClockClass         = gPTPd->pChUsr->Master.LocalMaster.Class;
	pPdu->PtcpData.Sync.TLV.Master.ClockAccuracy      = gPTPd->pChUsr->Master.LocalMaster.Accuracy;
	pPdu->PtcpData.Sync.TLV.Master.ClockVariance.Word = htons(gPTPd->pChUsr->Master.LocalMaster.Variance);
	pPdu->PtcpData.Sync.TLV.Master.TypeLen.Word       = htons(TLV_MASTER_TYPELEN);

	pPdu->PtcpData.Sync.PtcpHdr.SequenceID.Word = htons(gPTPd->pChUsr->Master.SeqId);
	pPdu->PtcpData.Sync.PtcpHdr.ReceiveTS.Dword = SWAP32(gPTPd->pChUsr->Master.Timestamp);
	pPdu->PtcpData.Sync.PtcpHdr.Delay10ns.Dword = 0;
	pPdu->PtcpData.Sync.PtcpHdr.Delay1ns        = 0;
	pPdu->PtcpData.Sync.PtcpHdr.DelayNS.Dword   = 0;

	pPdu->PtcpData.Sync.TLV.End.TypeLen.Word = htons(TLV_END_TYPELEN);

	if (0 == sendto(gPTPd->txBufSync, SYNC_PDU_SIZE, 2, &gPTPd->ts[6], gPTPd))
	{
		++gPTPd->sendError;
		return R_FAILED;
	}
	else
	{
		++gPTPd->pChUsr->Master.SeqId;
	}

	return R_OK;
}

void prepareSyncFlwup(struct gPTPd* gPTPd)
{
	RSYNC_PTCP *        pPdu = (RSYNC_PTCP*)&gPTPd->txBufSyFu;
	GSY_MAC_ADR_TYPE DstAddr = {PTCP_MULTICAST_ADDR_SYNC_FU};
	LSA_UINT16			 FrameId = GSY_FRAMEID_SYNC_FU | gPTPd->pChUsr->SyncId;

	memset(pPdu, 0, SYNC_FU_PDU_SIZE);

	DstAddr.MacAdr[5] |=  gPTPd->pChUsr->SyncId;
	memcpy((void*)&pPdu->PduHdr.DstMacAdr,&DstAddr, sizeof(GSY_MAC_ADR_TYPE));
	Bsp_GetPortMacAddr((PNIO_UINT8*)&pPdu->PduHdr.SrcMacAdr, gPTPd->port + 1);

//#ifdef USE_VLAN
//	{
//		VLAN_TAG_TYPE	VlanTag = {VLAN_TAG};
//		pPdu->PduHdr.VlanTag  = VlanTag;
//	}
//#endif
	pPdu->PduHdr.EthType.Word = htons(ETHERTYPE);
	pPdu->PduHdr.FrameId.Word = htons(FrameId);

	memcpy((void*)&pPdu->PtcpData.SyncFu.Subdomain.SubdomainUUID, &gPTPd->pChUsr->SyncData.SubdomainID, sizeof(PTCP_UUID));

	pPdu->PtcpData.SyncFu.Subdomain.TypeLen.Word = htons(TLV_SUBDOMAIN_TYPELEN);

	pPdu->PtcpData.SyncFu.Time.TypeLen.Word = htons(TLV_TIME_TYPELEN);
	pPdu->PtcpData.SyncFu.End.TypeLen.Word  = htons(TLV_END_TYPELEN);
}

void sendSyncFlwup(struct gPTPd* gPTPd)
{
	RSYNC_PTCP *        pPdu = (RSYNC_PTCP*)&gPTPd->txBufSyFu;
	GSY_MAC_ADR_TYPE DstAddr = {PTCP_MULTICAST_ADDR_SYNC_FU};
	LSA_UINT16			 FrameId = GSY_FRAMEID_SYNC_FU | gPTPd->pChUsr->SyncId;

	DstAddr.MacAdr[5] |= gPTPd->pChUsr->SyncId;
	memcpy((void*)&pPdu->PduHdr.DstMacAdr,&DstAddr, sizeof(GSY_MAC_ADR_TYPE));

	pPdu->PduHdr.FrameId.Word = htons(FrameId);

	if (0 == sendto(gPTPd->txBufSyFu, SYNC_FU_PDU_SIZE, 5, NULL, gPTPd))
	{
		++gPTPd->sendError;
	}
	else
	{
		++gPTPd->cs.syncSeqNo;
	}
}

/* extracted as inline function for readability */
__attribute__ ((always_inline)) inline void syncFwdPrepare(struct gPTPd* PTPd, uint8_t * frameBuffer, uint32_t port)
{
	extern struct gPTPd gPTPd[NUM_PORTS];

	MAC_ADR_TYPE    AddrSyncFu = {PTCP_MULTICAST_ADDR_FUSYNC};
	uint16_t        masterPort = PTPd->port;

	RSYNC_PTCP * pPdu = (RSYNC_PTCP*)frameBuffer;
	
	/* Add receiving port Line-Delay to SyncFU, one-step and two-step */
	pPdu->PtcpData.Sync.PtcpHdr.DelayNS.Dword = SWAP32(SWAP32(pPdu->PtcpData.Sync.PtcpHdr.DelayNS.Dword) + gPTPd[masterPort].remoteTXDelay + gPTPd[masterPort].RealPortRxDelay + gPTPd[masterPort].cableDelay);
  
	if(0 == gPTPd[masterPort].twoStep)
	{
		/* store sequence-ID for follow-Up */
		uint16_t seqID   = ((RSYNC_PTCP*)&gPTPd[masterPort].rxBuf[0])->PtcpData.Sync.PtcpHdr.SequenceID.Word;
		
		/* take current master time */
		uint16_t epoche  = ((RSYNC_PTCP*)&gPTPd[masterPort].rxBuf[0])->PtcpData.Sync.TLV.Time.EpochNumber.Word;
		uint32_t seconds = ((RSYNC_PTCP*)&gPTPd[masterPort].rxBuf[0])->PtcpData.Sync.TLV.Time.Seconds.Dword;
		uint32_t nannos  = ((RSYNC_PTCP*)&gPTPd[masterPort].rxBuf[0])->PtcpData.Sync.TLV.Time.NanoSeconds.Dword;
		
		/* ***************************************************************************** */
		/*                            prepare sync fwd                                   */
		/* ***************************************************************************** */
		
		/* set destination MAc */
		AddrSyncFu.MacAdr[5] |= gPTPd[gPTPd->port].syncID;
		pPdu->PduHdr.DstMacAdr = AddrSyncFu;
		
		/* set new Frame-ID */
		pPdu->PduHdr.FrameId.Word = (FRAMEID_FUSYNC | (LSA_UINT16)gPTPd[masterPort].syncID);
		
		/* ***************************************************************************** */
		/*                            prepare follow-up                                  */
		/* ***************************************************************************** */
		
		pPdu = (RSYNC_PTCP*)&gPTPd[port].txBufSyFu;
		
		/* Take Seq. ID from initial Sync frame */
		pPdu->PtcpData.SyncFu.PtcpHdr.SequenceID.Word = seqID;
		
		/* set master time */
		pPdu->PtcpData.SyncFu.Time.EpochNumber.Word  = epoche;
		pPdu->PtcpData.SyncFu.Time.Seconds.Dword     = seconds;
		pPdu->PtcpData.SyncFu.Time.NanoSeconds.Dword = nannos;
	}
}

void forwardSync(struct gPTPd* gPTPdRef, uint8_t * frameBuffer, uint32_t isSync)
{
	extern PNIO_UINT32 tsWait;
	extern EDDP_LL_HANDLE_PTR_TYPE pHandle;
	extern struct gPTPd gPTPd[NUM_PORTS];

	uint16_t masterPort = gPTPdRef->port;
	
	for(uint32_t port = 0; port < NUM_PORTS; ++port)
	{
		/* forward on all other ports if forwarding port has delay set */
		if((masterPort != port) && (PORT_ENABLED == gPTPd[port].enabled) && (0 < gPTPd[port].cableDelay))
		{
			/* Replace Src. MAC Address with our MAC Address */
			RSYNC_PTCP * pPdu = (RSYNC_PTCP*)frameBuffer;    	
			Bsp_GetPortMacAddr((PNIO_UINT8*)&pPdu->PduHdr.SrcMacAdr, port+1);

			if((0UL < (pHandle->dwLinkStatusAll & (1 << port))))
			{ // Forward Sync.
				/* send syncFU for oneStep and twoStep */
				if(1 == isSync)
				{ 
					syncFwdPrepare(gPTPdRef, frameBuffer, port);
					
					if(0 == sendto(frameBuffer, SYNC_PDU_SIZE, 5, &gPTPd[port].ts[TX_SYNC_FW], &gPTPd[port]))
					{
						++gPTPd[port].sendError;
					}
					else
					{ //Send syncFU.
						if(0 == OsQueSend(syncFollowUpQueue, (PNIO_UINT32)&gPTPd[port], 0U))
						{
#ifdef RSyncDebug
							volatile int brk = 0;
#endif
						}
					}
				}
				else
				{ // Followup, two-step
					if(0 == OsQueSend(syncFollowUpQueue, (PNIO_UINT32)&gPTPd[port] | 1, 0U))
					{
#ifdef RSyncDebug
						volatile int brk = 0;
#endif
					}
				}
			}
		}
	}
}

/* inform PNO stack about changed sync status */
void syncChanged(struct gPTPd* gPTPd, LSA_UINT32 state)
{
	EDDP_LOCAL_HDB_PTR_TYPE pHDB;
	uint16_t Status = EDDP_CoreDevHandleGetHDB(0, &pHDB);
	EDD_RQB_TYPE rqb;
	EDD_RQB_SET_SYNC_STATE_TYPE param;
	
	gPTPd->SyncState = state;
	param.SyncState  = state;	
	rqb.pParam       = &param;
	
	if(EDD_STS_OK == Status)
	{
		EDDP_GEN_RequestSetSyncState(&rqb,pHDB->pDDB,pHDB);
		
		if(GSY_DIAG_SYNCID_SYNCHRONIZED == state)
		{
			setLLDPMasterMAC(GSY_DIAG_SOURCE_SYNC,gPTPd, 0);
			setLLDPMasterMAC(GSY_DIAG_SOURCE_SYNC,gPTPd, 1);
			reloadCSTimeout(gPTPd);
			gptp_startTimer(gPTPd, GPTP_TIMER_SYNC_TO, gPTPd->cs.syncTimeout, GPTP_EVT_CS_SYNC_TO);
		}
		else
		{
			extern uint16_t gSyncPort;
			gSyncPort = 0xffff;

			setLLDPMasterMAC(GSY_DIAG_SOURCE_SYNC,gPTPd, 1);
			setLLDPMasterMAC(GSY_DIAG_SOURCE_SYNC,gPTPd, 0);
			gptp_stopTimer(gPTPd, GPTP_TIMER_SYNC_TO);
		}
	}
}

/*****************************************************************************/
/* Internal function: gsy_SyncUserInd()                                      */
/* Store time of the actual Sync Event and signal the last or actual event   */
/* to the user if a upper RQB is in queue.                                   */
/*****************************************************************************/
LSA_VOID GSY_LOCAL_FCT_ATTR gsy_SyncUserInd(
																						GSY_CH_USR_PTR	pChUsr,
																						LSA_BOOL				bSetTime,
																						LSA_BOOL				bSetEvent)
{
	LSA_UINT32	TimeStamp = 0;
	LSA_UINT16	PrmIndCnt = 0;
	LSA_UINT16	SyncEvent = pChUsr->Event.SyncEvent;
	LSA_UINT16	Response  = GSY_RSP_OK;
	
	GSY_UPPER_RQB_PTR_TYPE pRbu = LSA_NULL;
	
	if (bSetTime)
	{
		pChUsr->Event.Prm.Seconds     = 0;
		pChUsr->Event.Prm.Nanoseconds = 0;
		
		/* EventTime = now: read actual time with stamp from system
		*/
		syncGet(&pChUsr->Event.Prm.Seconds, &pChUsr->Event.Prm.Nanoseconds, &TimeStamp);
	}
	
	if (bSetEvent)
	{
		/* Update PRM Indication structure
		*/
		PrmIndCnt = gsy_PrmEventSet(pChUsr, SyncEvent);
		
		/* Interface-Event
		*/
		pChUsr->Event.PortId = 0;	
	}
	
	// arbitrarily often (PrmCommit w/o PrmWrite)
	if ((GSY_MASTER_STATE_STOP != pChUsr->Master.State)
			&&  (GSY_SLAVE_STATE_STOP  != pChUsr->Slave.State)
			&&  ((GSY_SYNC_EVENT_NONE == pChUsr->Event.SyncEvent)
			|| (SyncEvent != pChUsr->Event.OldEvent) 
			|| (pChUsr->Event.PortId != pChUsr->Event.OldPort))
			)
	{
		/* If an upper RQB is in the Indication-Queue
		*  and Master/Slave is not stopping:
		*  indicate event or error to user
		*/
		if ((pChUsr->QueuePrm.Count != 0) && (0 != PrmIndCnt))
		{
			/* Get PRM Indication RQB from Queue and fill in the values
			*/
			pRbu = pChUsr->QueuePrm.pFirst;
			gsy_DequeUpper(&pChUsr->QueuePrm, pRbu);
			(LSA_VOID)gsy_PrmUserInd(pChUsr, pRbu);
		}
		else
		{
			//ERROR
		}
		
		pChUsr->Event.OldEvent = SyncEvent;
		pChUsr->Event.OldPort  = pChUsr->Event.PortId;
		
		/* If no lower Request is pending (120207lrg001: or just running): send upper Indication
		*/
		if (!LSA_HOST_PTR_ARE_EQUAL(pRbu, LSA_NULL))
		{
			/* 120207lrg001: PRM-Indication RQB waits for lower Confirmation
			*/
			if (pChUsr->QueueEvent.Count != 0)
			{
				gsy_PutUpperQ(&pChUsr->QueueEvent, pRbu);
			}
			else
			{
				gsy_CallbackUsr(pChUsr, pRbu, Response);
			}
		}
	}
}
#endif
