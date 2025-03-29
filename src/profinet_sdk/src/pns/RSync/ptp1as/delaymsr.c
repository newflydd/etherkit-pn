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

#include "ecos_ertec_os_utils.h"
#include "pnorsyncif.h"
#include "gptpcmn.h"
#include "delaymsr.h"
#include "PTCP.h"
#include "r_ether_switch_rzt2_ext.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#ifdef PrintfDebug
	/* For syslog frames */
	#include "debug.h"
#endif

/* for abs() */
#include "stdlib.h"

/* Queue for delay response (special use case) */
void * dlRsQueue[NUM_PORTS] = {NULL};

/* simple moving average (SMA) calculation */
__attribute__ ((always_inline)) inline static void cableDelayAverage (struct gPTPd * gPTPd, int32_t tmpDelay)
{
#define SMA_WINDOW_SIZE 5
#define TRASHHOLD       1600
#define MAX_DRIFT       50
	
	 // Can be caused by deviating clocks. 
	if (tmpDelay <= 0) 
	{
		tmpDelay = 1; 
	}
	
	/* catch deviations caused by first sync attempts (i.e. Hard clock set) */
	if(tmpDelay < TRASHHOLD)       
	{
		
		/* smoothing factor alpha (how fast/slow do we follow a changing lineDelay trend); smaler value = faster */
		const int32_t alpha = 5;
		static int32_t window[NUM_PORTS] = {0};
		int32_t average = gPTPd->cableDelay;
		
		/* if evarage is 0 restart mean calculation (after cable un-plug) */
		window[gPTPd->port] = average == 0 ? 0 : window[gPTPd->port];
		
		if(window[gPTPd->port] < SMA_WINDOW_SIZE)
		{
			++window[gPTPd->port];
			average = average + ((tmpDelay - average) / window[gPTPd->port]);
		}
		else
		{
			average = average + ((tmpDelay - average) / alpha);
		}
		
		gPTPd->cableDelay = average;
		gPTPd->pChUsr->pChSys->Port[gPTPd->port].SyncLineDelay = average;
		gPTPd->pChUsr->pChSys->Port[gPTPd->port].CableDelay    = average;
		
		setLineDelay(gPTPd);
		
#ifdef PrintfDebug
		if (abs(tmpDelay - average) > 20)
		{
			char text_string[20];
			sprintf(&text_string[0], "%d CDelay %d",gPTPd->port , average);
			printeth(text_string);
		}
#endif
	}
}


/* Corrects frequency deviation in ResDeley 
 * See RCFpeer of "Line delay measurement and peeer rate compensation" in PROFINET specification
 */
#define RCFPEER_SIGNIFICANT_DIGITS 10000000
#define RCFPEER_MAX                10002000  // (RCFPEER_SIGNIFICANT_DIGITS + 200ppm
#define RCFPEER_MIN                 9998000  // (RCFPEER_SIGNIFICANT_DIGITS - 200ppm
#define CLK_PERIOD_NS                     8  // EtherSwitch Timer Clock 125MHz
void rcfPeerCalc(struct gPTPd* gPTPd,struct timespec *resDelay)
{
	LSA_UINT64 rcfPeerx1M; 
	if(gPTPd->DelayDriftAdjustDone)
	{
		if(gPTPd->delayRepeat > 1)
		{
			LSA_UINT32 calcT1 = (LSA_UINT32)gPTPd->ts[TX_DELAY_REQ].tv_nsec - gPTPd->LocalPortTimeT1_del;
			LSA_UINT32 calcT2 = (LSA_UINT32)gPTPd->ts[REMOTE_PORT_TIME_T2].tv_nsec - gPTPd->RemotePortTimeT2_del; 
			if(calcT1 != calcT2)
			{
				rcfPeerx1M = ((LSA_UINT64)calcT2 * RCFPEER_SIGNIFICANT_DIGITS) / (LSA_UINT64)calcT1;
				if(RCFPEER_MAX < rcfPeerx1M || RCFPEER_MIN > rcfPeerx1M)
				{
					rcfPeerx1M = RCFPEER_SIGNIFICANT_DIGITS;
				}
				gPTPd->rcfPeerx1M = rcfPeerx1M; 
			}
		}
		else
		{
			rcfPeerx1M = gPTPd->rcfPeerx1M; // Use stored value for first measurement of the 5-Batch as delay is too big
		}
        LSA_UINT32 resDelaypeer = (LSA_UINT32)(((LSA_UINT64)resDelay->tv_nsec * RCFPEER_SIGNIFICANT_DIGITS) / rcfPeerx1M);
        // The timer for delay measurement is in 8ns(125MHz) steps. Therefore, it is rounded up by the resolution of the timer.
		// resDelaypeer = (resDelaypeer + CLK_PERIOD_NS) / 10 * 10;
#if PrintfDebug
        char text_string[100];
        sprintf(&text_string[0], "%d %d Rcf=%lld Dp=%lu",gPTPd->delayRepeat, gPTPd->port, rcfPeerx1M-10000000, resDelaypeer); 
        printeth(text_string);
  //			sprintf(&text_string[0], "ResDpeer=%lu",resDelaypeer);
  //			printeth(text_string);
#endif
		resDelay->tv_nsec = resDelaypeer;
	}
	else
	{
		gPTPd->DelayDriftAdjustDone = 1;
	}
	gPTPd->RemotePortTimeT2_del = gPTPd->ts[REMOTE_PORT_TIME_T2].tv_nsec;
	gPTPd->LocalPortTimeT1_del = gPTPd->ts[TX_DELAY_REQ].tv_nsec;
}

void initDM(struct gPTPd* gPTPd)
{
	gPTPd->dm.state = DM_STATE_INIT;
	gPTPd->dm.delayReqInterval = GPTP_PDELAY_REQ_INTERVAL;
	gPTPd->dm.delayReqTimeOut  = GPTP_PDELAY_REQ_TIMEOUT;
}

void delayResponseTimout(PNIO_UINT16 timer_id, struct gPTPd* gPTPd)
{
#if PrintfDebug
  printeth("DelRespTimeout"); 
#endif
	invalidateCableDelay(gPTPd);
}
/* Delay SM aka. DM (DelayManager) */
void dmHandleEvent(struct gPTPd* gPTPd, int evtId)
{
		extern PNIO_UINT32 tsWait;
		struct timespec diff[3];

		switch(gPTPd->dm.state)
		{
				case DM_STATE_INIT:
								switch (evtId) {
												case GPTP_EVT_STATE_ENTRY:
																gPTPd->delayRepeat = 0;
																break;
												case GPTP_EVT_DM_ENABLE:
																dmHandleStateChange(gPTPd, DM_STATE_IDLE);
																break;
												case GPTP_EVT_STATE_EXIT:
																break;
												default:
																break;
								}
								break;

				case DM_STATE_IDLE:
								switch (evtId) 
								{
										case GPTP_EVT_STATE_ENTRY:
														break;
														
										case GPTP_EVT_DM_PDELAY_REQ_TO:
													gPTPd->delayRepeat = 0;
													gptp_startTimer(gPTPd, GPTP_TIMER_DELAYREQ_RPT, gPTPd->dm.delayReqInterval, GPTP_EVT_DM_PDELAY_REQ_RPT);
													break;
														
										case GPTP_EVT_DM_PDELAY_REQ_RPT:
														gptp_resetTimer(gPTPd, GPTP_TIMER_DELAYREQ_RPT);
														memset(&gPTPd->ts[TX_DELAY_REQ], 0, sizeof(struct timespec) * 4);
														
														OsResetTimer(gPTPd->respTimeout);
														OsStartTimer(gPTPd->respTimeout, (PNIO_UINT32)gPTPd, DEL_REQ_TO );
														
														if(R_OK == sendDelayReq(gPTPd))
														{
															getTxTS(gPTPd, &gPTPd->ts[TX_DELAY_REQ], 0);
														}
														else
														{
															OsResetTimer(gPTPd->respTimeout);
														}
														
														++gPTPd->delayRepeat;
															
													  if(GPTP_PDELAY_REPEAT == gPTPd->delayRepeat)
														{
															gptp_stopTimer(gPTPd, GPTP_TIMER_DELAYREQ_RPT);
															gptp_startTimer(gPTPd, GPTP_TIMER_DELAYREQ_TO, gPTPd->dm.delayReqTimeOut, GPTP_EVT_DM_PDELAY_REQ_TO);
														}
														
														dmHandleStateChange(gPTPd, DM_STATE_DELAY_RESP_WAIT);
														break;
														
										case GPTP_EVT_DM_PDELAY_RESP:
										case GPTP_EVT_DM_PDELAY_RESP_FLWUP:	
														/* got delay repose to fast, place in queue for later handling */
														xQueueReset(dlRsQueue[gPTPd->port]);
														OsQueSend  (dlRsQueue[gPTPd->port], (PNIO_UINT32)gPTPd->rxBuf, 0U);
		
														/* Spirta hack - TS takes too much time or is missing */
														OsResetTimer(gPTPd->respTimeout);
														break;
														
										case GPTP_EVT_DM_PDELAY_REQ:
											
														getRxTS(gPTPd, &gPTPd->ts[RX_DELAY_REQ]);
														gPTPd->dm.rxSeqNo = ((RSYNC_PTCP*)gPTPd->rxBuf)->PtcpData.DelayReq.PtcpHdr.SequenceID.Word;

														if (R_OK == sendDelayResp(gPTPd))
														{											
															if (0 == getTxTS(gPTPd, &gPTPd->ts[TX_DELAY_RES], 1))
															{
																break;
															}
															
															sendDelayRespFlwUp(gPTPd);
														}
														break;
														
										case GPTP_EVT_STATE_EXIT:
														break;
														
										default:
														break;
								}
								break;

				case DM_STATE_DELAY_RESP_WAIT:
			
								/* special Spirta case */
								if(GPTP_EVT_STATE_ENTRY == evtId)
								{
									RSYNC_PTCP* valid = LSA_NULL;
									/* Queue content is only used as flag (should be the same as gPTPd->rxBuf) */
									if((valid = (RSYNC_PTCP*) OsQueRecv(dlRsQueue[gPTPd->port], 0L)))
									{
										/* Safty meassure check. */
										if((valid == (RSYNC_PTCP*)gPTPd->rxBuf) && htons(((RSYNC_PTCP *)(gPTPd->rxBuf))->PduHdr.FrameId.Word) == GSY_FRAMEID_DELAY_RSP )
										{
											 /* Transfer the frame from the low-priority sending task back to the high-priority receiving task's queue to elevate its priority.*/
                       OsQueSendToFront(rxTsQueue[gPTPd->port], (PNIO_UINT32)gPTPd->RSyncReceiveAddr, 0U);
                       break;   
										}
									}
								}

								switch (evtId) 
								{
										case GPTP_EVT_STATE_ENTRY:
														//gptp_startTimer(gPTPd, GPTP_TIMER_DELAYREQ_TO, gPTPd->dm.delayReqTimeOut, GPTP_EVT_DM_PDELAY_REQ_TO);
														break;

										case GPTP_EVT_DM_PDELAY_REQ:
														gPTPd->dm.rxSeqNo = ((RSYNC_PTCP*)gPTPd->rxBuf)->PtcpData.DelayReq.PtcpHdr.SequenceID.Word;                             
														getRxTS(gPTPd, &gPTPd->ts[RX_DELAY_REQ]);
														
														if (R_OK == sendDelayResp(gPTPd))
														{
															if (0 == getTxTS(gPTPd, &gPTPd->ts[TX_DELAY_RES], 1)) 
															{
																break;
															}
															
															sendDelayRespFlwUp(gPTPd);
														}
														break;

										case GPTP_EVT_DM_PDELAY_REQ_RPT:
														gptp_resetTimer(gPTPd, GPTP_TIMER_DELAYREQ_RPT);
														memset(&gPTPd->ts[TX_DELAY_REQ], 0, sizeof(struct timespec) * 4);
														
														OsResetTimer(gPTPd->respTimeout);
														OsStartTimer(gPTPd->respTimeout, (PNIO_UINT32)gPTPd, DEL_REQ_TO );
														
														if(R_OK == sendDelayReq(gPTPd))
														{
															getTxTS(gPTPd, &gPTPd->ts[TX_DELAY_REQ], 0);
														}
														else
														{
															OsResetTimer(gPTPd->respTimeout);
														}
														
														++gPTPd->delayRepeat;
														
														if(GPTP_PDELAY_REPEAT == gPTPd->delayRepeat)
														{
															gptp_stopTimer(gPTPd, GPTP_TIMER_DELAYREQ_RPT);
															gptp_startTimer(gPTPd, GPTP_TIMER_DELAYREQ_TO, gPTPd->dm.delayReqTimeOut, GPTP_EVT_DM_PDELAY_REQ_TO);
														}
														
														break;
														
										case GPTP_EVT_DM_PDELAY_REQ_TO:
														gPTPd->delayRepeat = 0;
														gptp_startTimer(gPTPd, GPTP_TIMER_DELAYREQ_RPT, gPTPd->dm.delayReqInterval, GPTP_EVT_DM_PDELAY_REQ_RPT);
														break;

										case GPTP_EVT_DM_PDELAY_RESP:
														/* Got pdelayresponse, no follow up expected.*/
											
														OsResetTimer(gPTPd->respTimeout);

														gPTPd->ts[REMOTE_PORT_TIME_T2].tv_nsec = SWAP32(((RSYNC_PTCP*)gPTPd->rxBuf)->PtcpData.DelayRsp.PortTime.T2TimeStamp.Dword);
														gPTPd->remoteRXDelay = SWAP32(((RSYNC_PTCP*)gPTPd->rxBuf)->PtcpData.DelayRsp.PortParam.T2PortRxDelay.Dword);                              
														gPTPd->remoteTXDelay = SWAP32(((RSYNC_PTCP*)gPTPd->rxBuf)->PtcpData.DelayRsp.PortParam.T3PortTxDelay.Dword);
														getRxTS(gPTPd, &gPTPd->ts[RX_DELAY_RES]);
														gPTPd->ts[REMOTE_PORT_TIME_T3].tv_nsec = SWAP32(((RSYNC_PTCP*)gPTPd->rxBuf)->PtcpData.DelayFu.PtcpHdr.DelayNS.Dword) + gPTPd->ts[REMOTE_PORT_TIME_T2].tv_nsec;

														diff[LOCAL_REQDELAY].tv_nsec = (uint32_t)gPTPd->ts[RX_DELAY_RES].tv_nsec - (uint32_t)gPTPd->ts[TX_DELAY_REQ].tv_nsec;                // Timestampvalues between DelayReq frame sent and DelayResp Returned
														diff[REMOTE_RESDELAY].tv_nsec = (uint32_t)gPTPd->ts[REMOTE_PORT_TIME_T3].tv_nsec - (uint32_t)gPTPd->ts[REMOTE_PORT_TIME_T2].tv_nsec; // Residencetime at Partner

														rcfPeerCalc(gPTPd,&diff[REMOTE_RESDELAY]);
														if(diff[REMOTE_RESDELAY].tv_nsec > diff[LOCAL_REQDELAY].tv_nsec) 
														{
															// Something is wrong, Residence time at partner is larger than time between TX and RX on our side (even incl. PHYs) -> ERROR
#if PrintfDebug
                              printeth("NegDel Pdelresp");
#endif
															invalidateCableDelay(gPTPd);
														} 
														else 
														{
															diff[LOCAL_CABLEDELAY].tv_nsec = (uint32_t)diff[LOCAL_REQDELAY].tv_nsec - (uint32_t)diff[REMOTE_RESDELAY].tv_nsec;
															gPTPd->portDelay  = gPTPd->remoteRXDelay + gPTPd->remoteTXDelay + gPTPd->RealPortTxDelay + gPTPd->RealPortRxDelay;  
															
															int32_t tmpDelay  = ((int32_t) diff[LOCAL_CABLEDELAY].tv_nsec - (int32_t) gPTPd->portDelay) / 2;  // 

															cableDelayAverage(gPTPd, tmpDelay);
														}
 														dmHandleStateChange(gPTPd, DM_STATE_IDLE);
														break;

										case GPTP_EVT_DM_PDELAY_FURESP:
														/* Got pdelayresponse, waiting for follow up */
											
														gPTPd->ts[REMOTE_PORT_TIME_T2].tv_nsec = SWAP32(((RSYNC_PTCP*)gPTPd->rxBuf)->PtcpData.DelayRsp.PortTime.T2TimeStamp.Dword);
														gPTPd->remoteRXDelay = SWAP32(((RSYNC_PTCP*)gPTPd->rxBuf)->PtcpData.DelayRsp.PortParam.T2PortRxDelay.Dword);
														gPTPd->remoteTXDelay = SWAP32(((RSYNC_PTCP*)gPTPd->rxBuf)->PtcpData.DelayRsp.PortParam.T3PortTxDelay.Dword);
														getRxTS(gPTPd, &gPTPd->ts[RX_DELAY_RES]);

														dmHandleStateChange(gPTPd, DM_STATE_DELAY_RESP_FLWUP_WAIT);
														break;

										case GPTP_EVT_DM_PDELAY_RESP_FLWUP:
														/* Spirta hack */
														OsResetTimer(gPTPd->respTimeout);
														break;
														
										case GPTP_EVT_STATE_EXIT:
														break;
										default:
														break;
						}
						break;
																
						case DM_STATE_DELAY_RESP_FLWUP_WAIT:
							
										switch (evtId) 
										{
										case GPTP_EVT_STATE_ENTRY:
														break;
														
										case GPTP_EVT_DM_PDELAY_REQ_TO:
														gPTPd->delayRepeat = 0;
														gptp_startTimer(gPTPd, GPTP_TIMER_DELAYREQ_RPT, gPTPd->dm.delayReqInterval, GPTP_EVT_DM_PDELAY_REQ_RPT);
														break;

										case GPTP_EVT_DM_PDELAY_REQ:
														gPTPd->dm.rxSeqNo = ((RSYNC_PTCP*)gPTPd->rxBuf)->PtcpData.DelayReq.PtcpHdr.SequenceID.Word;                            
														getRxTS(gPTPd, &gPTPd->ts[RX_DELAY_REQ]);
														
														if (R_OK == sendDelayResp(gPTPd))
														{
															if (0 == getTxTS(gPTPd, &gPTPd->ts[TX_DELAY_RES], 1)) 
															{
																break;
															}
															
															sendDelayRespFlwUp(gPTPd);
														}
														
														break;

										case GPTP_EVT_DM_PDELAY_RESP_FLWUP:

														OsResetTimer(gPTPd->respTimeout);
														
														gPTPd->ts[REMOTE_PORT_TIME_T3].tv_nsec = SWAP32(((RSYNC_PTCP*)gPTPd->rxBuf)->PtcpData.DelayFu.PtcpHdr.DelayNS.Dword) + gPTPd->ts[REMOTE_PORT_TIME_T2].tv_nsec;

														diff[LOCAL_REQDELAY].tv_nsec = (uint32_t)gPTPd->ts[RX_DELAY_RES].tv_nsec - (uint32_t)gPTPd->ts[TX_DELAY_REQ].tv_nsec;
														diff[REMOTE_RESDELAY].tv_nsec = (uint32_t)gPTPd->ts[REMOTE_PORT_TIME_T3].tv_nsec - (uint32_t)gPTPd->ts[REMOTE_PORT_TIME_T2].tv_nsec;

														rcfPeerCalc(gPTPd,&diff[REMOTE_RESDELAY]);
														if(diff[REMOTE_RESDELAY].tv_nsec > diff[LOCAL_REQDELAY].tv_nsec) 
														{
															// got negative delay (ERROR)
#if PrintfDebug
                              printeth("NegDelay FLUP"); 
#endif
															invalidateCableDelay(gPTPd);
														}
														else
														{ 
															diff[LOCAL_CABLEDELAY].tv_nsec = (uint32_t)diff[LOCAL_REQDELAY].tv_nsec - (uint32_t)diff[REMOTE_RESDELAY].tv_nsec;
															gPTPd->portDelay  = gPTPd->remoteRXDelay + gPTPd->remoteTXDelay + gPTPd->RealPortTxDelay + gPTPd->RealPortRxDelay;
															
															int32_t tmpDelay  = ((int32_t) diff[LOCAL_CABLEDELAY].tv_nsec - (int32_t)gPTPd->portDelay) / 2;
#if PrintfDebug
															if ((tmpDelay > 300) || (tmpDelay < 2) )
															{
																char text_string[25];
																snprintf(&text_string[0], 24, "%d CD %d", gPTPd->port,  tmpDelay ); 
																printeth(text_string); 
															}
#endif

															cableDelayAverage(gPTPd, tmpDelay);                             
														}

														dmHandleStateChange(gPTPd, DM_STATE_IDLE);
														break;
														
										case GPTP_EVT_DM_PDELAY_REQ_RPT:
														gptp_resetTimer(gPTPd, GPTP_TIMER_DELAYREQ_RPT);
														memset(&gPTPd->ts[TX_DELAY_REQ], 0, sizeof(struct timespec) * 4);
														
														OsResetTimer(gPTPd->respTimeout);
														OsStartTimer(gPTPd->respTimeout, (PNIO_UINT32)gPTPd, DEL_REQ_TO );
														
														if(R_OK == sendDelayReq(gPTPd))
														{
															getTxTS(gPTPd, &gPTPd->ts[TX_DELAY_REQ], 0);
														}
														else
														{
															OsResetTimer(gPTPd->respTimeout);
														}
														
														++gPTPd->delayRepeat;
														
														if(GPTP_PDELAY_REPEAT == gPTPd->delayRepeat)
														{
															gptp_stopTimer(gPTPd, GPTP_TIMER_DELAYREQ_RPT);
															gptp_startTimer(gPTPd, GPTP_TIMER_DELAYREQ_TO, gPTPd->dm.delayReqTimeOut, GPTP_EVT_DM_PDELAY_REQ_TO);
														}
														
														break;
														
										case GPTP_EVT_DM_PDELAY_RESP:
														/* Spirta hack - TS takes too much time or is missing */
														OsResetTimer(gPTPd->respTimeout);
														break;
														
										case GPTP_EVT_STATE_EXIT:
														break;
											
										default:
														break;
										}
										break;

				default:
								break;
		}
}

void dmHandleStateChange(struct gPTPd* gPTPd, int toState)
{
	dmHandleEvent(gPTPd, GPTP_EVT_STATE_EXIT);
	gPTPd->dm.state = toState;
	dmHandleEvent(gPTPd, GPTP_EVT_STATE_ENTRY);
}

#ifdef GPTP_PROTO
uint32_t sendDelayReq(struct gPTPd* gPTPd)
{
	int txLen = sizeof(struct ethhdr);
	struct gPTPHdr * gh = (struct gPTPHdr *)&gPTPd->txBufDeRq[sizeof(struct ethhdr)];

	/* Fill gPTP header */
	gh->h.f.seqNo      = htons(gPTPd->dm.txSeqNo);
	gh->h.f.b1.msgType = (GPTP_TRANSPORT_L2 | GPTP_MSG_TYPE_PDELAY_REQ);
	gh->h.f.flags      = htons(GPTP_FLAGS_NONE);

	gh->h.f.ctrl      = GPTP_CONTROL_DELAY_ANNOUNCE;
	gh->h.f.logMsgInt = GPTP_PDELAY_REQ_INTERVAL_LOG;

	/* Add gPTP header size */
	txLen += sizeof(struct gPTPHdr);

	/* PTP body */
	txLen += (GPTP_TS_LEN + GPTP_PORT_IDEN_LEN);

	/* Insert length */
	gh->h.f.msgLen = htons(txLen - sizeof(struct ethhdr));

	if (0 == sendto(gPTPd->txBufDeRq, txLen, 0, GPTP_MSG_TYPE_PDELAY_REQ, &gPTPd->ts[TX_DELAY_REQ], gPTPd))
	{
		++gPTPd->sendError;
		return R_FAILED;
	}
	else
	{
		++gPTPd->dm.txSeqNo;
	}
	
	return R_OK;
}

uint32_t sendDelayResp(struct gPTPd* gPTPd)
{
	int txLen = sizeof(struct ethhdr);
	struct gPTPHdr * gh = (struct gPTPHdr *)&gPTPd->txBufDeRp[sizeof(struct ethhdr)];

	/* Fill gPTP header */
	gh->h.f.seqNo      = htons(gPTPd->dm.rxSeqNo);
	gh->h.f.b1.msgType = (GPTP_TRANSPORT_L2 | GPTP_MSG_TYPE_PDELAY_RESP);
	gh->h.f.flags      = htons(GPTP_FLAGS_TWO_STEP);

	gh->h.f.ctrl      = GPTP_CONTROL_DELAY_ANNOUNCE;
	gh->h.f.logMsgInt = GPTP_LOG_MSG_INT_MAX;

	/* Add gPTP header size */
	txLen += sizeof(struct gPTPHdr);

	/* PTP body */
	gptp_copyTSToBuf(&gPTPd->ts[RX_DELAY_REQ], &gPTPd->txBufDeRp[txLen]);
	txLen += GPTP_TS_LEN;
	memcpy(&gPTPd->txBufDeRp[txLen], &gPTPd->dm.reqPortIden[0], GPTP_PORT_IDEN_LEN);
	txLen += GPTP_PORT_IDEN_LEN;

	/* Insert length */
	gh->h.f.msgLen = htons(txLen - sizeof(struct ethhdr));

	if (0 == sendto(gPTPd->txBufDeRp, txLen, 0, GPTP_MSG_TYPE_PDELAY_RESP, &gPTPd->ts[TX_DELAY_RES], gPTPd))
	{
		++gPTPd->sendError;
		return R_FAILED;
	}

	return R_OK;
}

void sendDelayRespFlwUp(struct gPTPd* gPTPd)
{
	int txLen = sizeof(struct ethhdr);
	struct gPTPHdr * gh = (struct gPTPHdr *)&gPTPd->txBufReFu[sizeof(struct ethhdr)];

	/* Fill gPTP header */
	gh->h.f.seqNo      = htons(gPTPd->dm.rxSeqNo);
	gh->h.f.b1.msgType = (GPTP_TRANSPORT_L2 | GPTP_MSG_TYPE_PDELAY_RESP_FLWUP);
	gh->h.f.flags      = htons(GPTP_FLAGS_NONE);

	gh->h.f.ctrl      = GPTP_CONTROL_DELAY_ANNOUNCE;
	gh->h.f.logMsgInt = GPTP_LOG_MSG_INT_MAX;

	/* Add gPTP header size */
	txLen += sizeof(struct gPTPHdr);

	/* PTP body */
	gptp_copyTSToBuf(&gPTPd->ts[TX_DELAY_RES], &gPTPd->txBufReFu[txLen]);
	txLen += GPTP_TS_LEN;
	memcpy(&gPTPd->txBufReFu[txLen], &gPTPd->dm.reqPortIden[0], GPTP_PORT_IDEN_LEN);
	
	/* Insert length */
	txLen += GPTP_PORT_IDEN_LEN;
	gh->h.f.msgLen = htons(txLen - sizeof(struct ethhdr));

	if (0 == sendto(gPTPd->txBufReFu, txLen, 0, GPTP_MSG_TYPE_PDELAY_RESP_FLWUP, NULL, gPTPd))
	{
		++gPTPd->sendError;
		return R_FAILED;
	}
	else
	{
		++gPTPd->dm.rxSeqNo;
		return R_OK;
	}
}

#else

void prepareDelayReq(struct gPTPd* gPTPd)
{
	RSYNC_PTCP *     pPdu = (RSYNC_PTCP*)&gPTPd->txBufDeRq;
	MAC_ADR_TYPE	DstAddr = {PTCP_MULTICAST_ADDR_DELAY};

	memset(pPdu, 0, DELAY_REQ_PDU_SIZE);
	
	Bsp_GetPortMacAddr((PNIO_UINT8*)&pPdu->PduHdr.SrcMacAdr, gPTPd->port + 1);
	pPdu->PduHdr.DstMacAdr = DstAddr;
//#ifdef GSY_USE_VLAN
//	{
//		GSY_VLAN_TAG_TYPE	VlanTag = {GSY_VLAN_TAG};
//		pPdu->PduHdr.VlanTag = VlanTag;
//	}
//#endif
	pPdu->PduHdr.EthType.Word = htons(ETHERTYPE);
	pPdu->PduHdr.FrameId.Word = FRAMEID_DELAY_REQ;

	Bsp_GetPortMacAddr((PNIO_UINT8*)&pPdu->PtcpData.DelayReq.DelayParam.RequestSourceAddress, gPTPd->port + 1);
	pPdu->PtcpData.DelayReq.DelayParam.TypeLen.Word = htons(TLV_DELAYPARAM_TYPELEN);
	pPdu->PtcpData.DelayReq.End.TypeLen.Word = htons(TLV_END_TYPELEN);
}

uint32_t sendDelayReq(struct gPTPd* gPTPd)
{
	RSYNC_PTCP * pPdu = (RSYNC_PTCP*)&gPTPd->txBufDeRq;

	pPdu->PtcpData.DelayReq.PtcpHdr.SequenceID.Word = htons(gPTPd->dm.txSeqNo);
	
	if (0 == sendto(gPTPd->txBufDeRq, DELAY_REQ_PDU_SIZE, 0, &gPTPd->ts[TX_DELAY_REQ], gPTPd))
	{
		++gPTPd->sendError;
		return R_FAILED;
	}
	else
	{
		++gPTPd->dm.txSeqNo;
		return R_OK;
	}
}

void prepareDelayResp(struct gPTPd* gPTPd)
{
	RSYNC_PTCP *     pPdu = (RSYNC_PTCP*)&gPTPd->txBufDeRp;
	MAC_ADR_TYPE	DstAddr = {PTCP_MULTICAST_ADDR_DELAY};
	
	memset(pPdu, 0, DELAY_RSP_PDU_SIZE);
  Bsp_GetPortMacAddr((PNIO_UINT8*)&pPdu->PduHdr.SrcMacAdr, gPTPd->port + 1);

	pPdu->PduHdr.DstMacAdr = DstAddr;
	
//#ifdef GSY_USE_VLAN
//	{
//		GSY_VLAN_TAG_TYPE	VlanTag = {GSY_VLAN_TAG};
//		pPdu->PduHdr.VlanTag = VlanTag;
//	}
//#endif
	
	/* Fill gPTP header */
	pPdu->PduHdr.EthType.Word = htons(ETHERTYPE);
	pPdu->PduHdr.FrameId.Word = FRAMEID_DELAY_FURSP;

	/* Send local port delays (static phy delays)
	*/
	pPdu->PtcpData.DelayRsp.PortParam.T2PortRxDelay.Dword = SWAP32(gPTPd->RealPortRxDelay);
	pPdu->PtcpData.DelayRsp.PortParam.T3PortTxDelay.Dword = SWAP32(gPTPd->RealPortTxDelay);
	pPdu->PtcpData.DelayRsp.PortParam.TypeLen.Word        = htons(TLV_PORTPARAM_TYPELEN);

	pPdu->PtcpData.DelayRsp.DelayParam.TypeLen.Word = htons(TLV_DELAYPARAM_TYPELEN);
	pPdu->PtcpData.DelayRsp.PortTime.TypeLen.Word   = htons(TLV_PORTTIME_TYPELEN);
	pPdu->PtcpData.DelayRsp.End.TypeLen.Word        = htons(TLV_END_TYPELEN);
	pPdu->PtcpData.DelayRsp.PtcpHdr.Delay10ns.Dword = 0;
}

uint32_t sendDelayResp(struct gPTPd* gPTPd)
{
	RSYNC_PTCP * pPdu = (RSYNC_PTCP*)&gPTPd->txBufDeRp;

	/* Copy Sequence-ID from orginating request-frame into response-frame. */
	pPdu->PtcpData.DelayRsp.PtcpHdr.SequenceID.Word = gPTPd->dm.rxSeqNo;
	pPdu->PtcpData.DelayRsp.DelayParam = ((RSYNC_PTCP*)gPTPd->rxBuf)->PtcpData.DelayReq.DelayParam;
	
	pPdu->PtcpData.DelayRsp.PortTime.T2TimeStamp.Dword = SWAP32((LSA_UINT32)gPTPd->ts[RX_DELAY_REQ].tv_nsec);
	
	if (0 == sendto(gPTPd->txBufDeRp, DELAY_RSP_PDU_SIZE, 1, &gPTPd->ts[TX_DELAY_RES], gPTPd))
	{
		++gPTPd->sendError;
		return R_FAILED;
	}
	
	return R_OK;
}

void prepareDelayRespFlwUp (struct gPTPd* gPTPd)
{
	RSYNC_PTCP *     pPdu = (RSYNC_PTCP*)&gPTPd->txBufReFu;	
	MAC_ADR_TYPE	DstAddr = {PTCP_MULTICAST_ADDR_DELAY};

	memset(pPdu, 0, DELAY_FU_PDU_SIZE);
	
	Bsp_GetPortMacAddr((PNIO_UINT8*)&pPdu->PduHdr.SrcMacAdr, gPTPd->port + 1);

	pPdu->PduHdr.DstMacAdr = DstAddr;
//#ifdef GSY_USE_VLAN
//	{
//		GSY_VLAN_TAG_TYPE	VlanTag = {GSY_VLAN_TAG};
//		pPdu->PduHdr.VlanTag = VlanTag;
//	}
//#endif
	pPdu->PduHdr.EthType.Word = htons(ETHERTYPE);
	pPdu->PduHdr.FrameId.Word = FRAMEID_DELAY_FU;
	
	pPdu->PtcpData.DelayFu.DelayParam.TypeLen.Word = htons(TLV_DELAYPARAM_TYPELEN);
	pPdu->PtcpData.DelayFu.End.TypeLen.Word        = htons(TLV_END_TYPELEN);
}

void sendDelayRespFlwUp(struct gPTPd* gPTPd)
{
	RSYNC_PTCP *     pPdu = (RSYNC_PTCP*)&gPTPd->txBufReFu;	
	uint32_t        delay = 0;

	pPdu->PtcpData.DelayRsp.DelayParam = ((RSYNC_PTCP*)gPTPd->rxBuf)->PtcpData.DelayReq.DelayParam; 

	/* calculate CableDelay */
	delay = (uint32_t)gPTPd->ts[TX_DELAY_RES].tv_nsec - (uint32_t)gPTPd->ts[RX_DELAY_REQ].tv_nsec;
		
	pPdu->PtcpData.DelayFu.PtcpHdr.DelayNS.Dword   = SWAP32(delay);
	pPdu->PtcpData.DelayFu.PtcpHdr.SequenceID.Word = gPTPd->dm.rxSeqNo;
	
	if (0 == sendto(gPTPd->txBufReFu, DELAY_FU_PDU_SIZE, 4, NULL, gPTPd))
	{
		++gPTPd->sendError;
	}
}

void setLineDelay(struct gPTPd * gPTPd)
{
	gsy_LowerDelaySet(gPTPd->pChUsr->pChSys, gPTPd->port + 1);
	
	gsy_DiagUserInd(GSY_DIAG_SOURCE_DELAY, gPTPd->pChUsr->pChSys, LSA_NULL, 
									0       /* LSA_UINT32: MasterSeconds */, 
									0       /* LSA_UINT32: MasterNanoseconds */, 
									0       /* LSA_INT32: Offset */, 
									0       /* LSA_INT32: AdjustInterval */, 
									0       /* LSA_INT32: UserValue1 */, 
									0 	    /* LSA_INT32: UserValue2 */, 
									0       /* LSA_INT32: SetTimeHigh */, 
									0       /* LSA_INT32: SetTimeLow */, 
									gPTPd->port + 1  /* LSA_INT16: PortId */);
}

#endif
