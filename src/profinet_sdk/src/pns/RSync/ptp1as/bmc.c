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
#include "sync.h"
#include "PTCP.h"
#include "bmc.h"

void initBMC(struct gPTPd* gPTPd)
{
  gPTPd->bmc.state = BMC_STATE_INIT;

#ifdef GPTP_PROTO
  gPTPd->bmc.announceInterval = GPTP_ANNOUNCE_INTERVAL;
  gPTPd->bmc.announceTimeout  = GPTP_ANNOUNCE_TIMEOUT;
#else
	gPTPd->bmc.announceInterval = gPTPd->pChUsr->Master.SendInterval * PTCP_ANNOUNCE_FACTOR;
	gPTPd->bmc.announceTimeout  = PTCP_ANNO_TIMEOUT;
#endif
	
}

void bmcHandleEvent(struct gPTPd* gPTPd, int evtId)
{
  switch(gPTPd->bmc.state) 
	{

				case BMC_STATE_INIT:
								switch (evtId) {
												case GPTP_EVT_STATE_ENTRY:
																break;
												case GPTP_EVT_BMC_ENABLE:
#ifdef GPTP_PROTO
																bmcHandleStateChange(gPTPd, BMC_STATE_GRAND_MASTER);
																updatePrioVectors(gPTPd);
#else
																if(    (MASTER_STATE_PRIMARY == gPTPd->pChUsr->Master.State) 
																		|| (SLAVE_STATE_SYNC     == gPTPd->pChUsr->Slave.State)
																	 )
																{
																	bmcHandleStateChange(gPTPd, BMC_STATE_GRAND_MASTER);
																	updatePrioVectors(gPTPd);
																}
																else
																{
																	bmcSetAllPortsToSlave();
																}
#endif
																break;
												case GPTP_EVT_STATE_EXIT:
																break;
												default:
									break;
					}
					break;

					case BMC_STATE_GRAND_MASTER:
									switch (evtId) {
													case GPTP_EVT_STATE_ENTRY:
#ifdef GPTP_PROTO
																	gptp_startTimer(gPTPd, GPTP_TIMER_ANNOUNCE_RPT, gPTPd->bmc.announceInterval, GPTP_EVT_BMC_ANNOUNCE_RPT);
																	csSetState(gPTPd, TRUE);
#else
																	if(    (MASTER_STATE_PRIMARY == gPTPd->pChUsr->Master.State) 
																		  || (SLAVE_STATE_SYNC     == gPTPd->pChUsr->Slave.State)
																		 )
																	{
																		gptp_startTimer(gPTPd, GPTP_TIMER_ANNOUNCE_RPT, gPTPd->bmc.announceInterval, GPTP_EVT_BMC_ANNOUNCE_RPT);
																		csSetState(gPTPd, TRUE);
																	}
#endif
																	break;
																	
													case GPTP_EVT_BMC_ANNOUNCE_MSG:
																	if(updateAnnounceInfo(gPTPd) == TRUE)
																	{
																		bmcHandleStateChange(gPTPd, BMC_STATE_SLAVE);
																		
																		/* Master found on this port */
																		gPTPd->isMasterPort = 1;
																		bmcSetAllPortsToSlave();
																	}
																	break;
																	
													case GPTP_EVT_BMC_ANNOUNCE_RPT:
#ifdef GPTP_PROTO
																	sendAnnounce(gPTPd);
#else
																	if(    (MASTER_STATE_PRIMARY == gPTPd->pChUsr->Master.State) 
																		  || (SLAVE_STATE_SYNC     == gPTPd->pChUsr->Slave.State)
																		 )
																	{
																		sendAnnounce(gPTPd);
																	}
#endif
																	break;
													case GPTP_EVT_STATE_EXIT:
																	gptp_stopTimer(gPTPd, GPTP_TIMER_ANNOUNCE_RPT);
																	break;
													default:
																	break;
									}
									break;

					case BMC_STATE_SLAVE:
									switch (evtId) {
													case GPTP_EVT_STATE_ENTRY:
#ifdef GPTP_PROTO
																	/* fall back to master after selected master fails to send annouce msg's periodically */
																	gptp_startTimer(gPTPd, GPTP_TIMER_ANNOUNCE_TO, gPTPd->bmc.announceTimeout, GPTP_EVT_BMC_ANNOUNCE_TO);
#else
																	if(    (MASTER_STATE_PRIMARY == gPTPd->pChUsr->Master.State) 
																		  || (SLAVE_STATE_SYNC     == gPTPd->pChUsr->Slave.State)
																		 )
																	{
																		gptp_startTimer(gPTPd, GPTP_TIMER_ANNOUNCE_TO, gPTPd->bmc.announceTimeout, GPTP_EVT_BMC_ANNOUNCE_TO);
																	}
#endif
																	csSetState(gPTPd, FALSE);
																	break;
																	
													case GPTP_EVT_BMC_ANNOUNCE_MSG:
																	if(updateAnnounceInfo(gPTPd) == TRUE)
																		gptp_resetTimer(gPTPd, GPTP_TIMER_ANNOUNCE_TO);
																	break;
																	
													case GPTP_EVT_BMC_ANNOUNCE_TO:
																	bmcHandleStateChange(gPTPd, BMC_STATE_GRAND_MASTER);
																	break;
																	
													case GPTP_EVT_STATE_EXIT:
																	gptp_stopTimer(gPTPd, GPTP_TIMER_ANNOUNCE_TO);
																	gPTPd->isMasterPort = 0;
																	break;
																	
													default:
														break;
									}
									break;
						
					default:
						break;
	}
}

void bmcHandleStateChange(struct gPTPd* gPTPd, int toState)
{
	bmcHandleEvent(gPTPd, GPTP_EVT_STATE_EXIT);
	gPTPd->bmc.state = toState;
	bmcHandleEvent(gPTPd, GPTP_EVT_STATE_ENTRY);
}

void bmcSetAllPortsToSlave(void)
{
	extern struct gPTPd gPTPd[NUM_PORTS];
	
	for(uint16_t port = 0; port < NUM_PORTS; ++port)
	{
		if (!gPTPd[port].isMasterPort)
		{
			bmcHandleStateChange(&gPTPd[port], BMC_STATE_SLAVE);
		}
	}
}

#ifdef GPTP_PROTO
void sendAnnounce(struct gPTPd* gPTPd)
{
	int    txLen = sizeof(struct ethhdr);
	struct gPTPHdr       *gh = (struct gPTPHdr *)   &gPTPd->txBufAnno[sizeof(struct ethhdr)];
	struct gPTPPrioVec *prio = (struct gPTPPrioVec*)&gPTPd->txBufAnno[GPTP_BODY_OFFSET];
	struct gPTPtlv *tlv;
	
	/* Fill gPTP header */
	gh->h.f.seqNo      = htons(gPTPd->bmc.annoSeqNo);
	gh->h.f.b1.msgType = (GPTP_TRANSPORT_L2 | GPTP_MSG_TYPE_ANNOUNCE);
	gh->h.f.flags      = htons(GPTP_FLAGS_PTP_TIMESCALE); 
	
	gh->h.f.ctrl      = GPTP_CONTROL_DELAY_ANNOUNCE;
	gh->h.f.logMsgInt = GPTP_ANNOUNCE_INTERVAL_LOG;
	
	/* Add gPTP header size */
	txLen += sizeof(struct gPTPHdr);
	
	/* PTP body */
	prio->currUTCOff = gPTPd->bmc.portPrio.currUTCOff;
	prio->prio1      = gPTPd->bmc.portPrio.prio1;
	prio->clockQual.clockClass    = gPTPd->bmc.portPrio.clockQual.clockClass;
	prio->clockQual.clockAccuracy = gPTPd->bmc.portPrio.clockQual.clockAccuracy;
	prio->clockQual.offsetScaledLogVariance = htons(gPTPd->bmc.portPrio.clockQual.offsetScaledLogVariance);
	prio->prio2 = gPTPd->bmc.portPrio.prio2;
	
	memcpy(&prio->iden[0], &gPTPd->bmc.portPrio.iden[0], GPTP_CLOCK_IDEN_LEN);
	
	prio->stepsRem = gPTPd->bmc.portPrio.stepsRem;
	prio->clockSrc = gPTPd->bmc.portPrio.clockSrc;
	txLen += sizeof(struct gPTPPrioVec);
	
	/* Path trace TLV */
	tlv = (struct gPTPtlv *)&gPTPd->txBufAnno[txLen];
	tlv->type = htons(GPTP_TLV_TYPE_PATH_TRACE);
	tlv->len  = htons(GPTP_CLOCK_IDEN_LEN);
	txLen += sizeof(struct gPTPtlv);
	
	memcpy(&gPTPd->txBufAnno[txLen], &gh->h.f.srcPortIden, GPTP_CLOCK_IDEN_LEN);
	
	/* Insert length */
	txLen += GPTP_CLOCK_IDEN_LEN;
	gh->h.f.msgLen = htons(txLen - sizeof(struct ethhdr));
	
	if (sendto(gPTPd->txBufAnno, txLen, 0, GPTP_MSG_TYPE_ANNOUNCE, NULL, gPTPd) == 0)
		;
	else
		gPTPd->bmc.annoSeqNo++;
}

#else //PTCP

void sendAnnounce(struct gPTPd* gPTPd)
{
	RSYNC_PTCP *     pPdu = (RSYNC_PTCP*)&gPTPd->txBufAnno;	
	MAC_ADR_TYPE	DstAddr = {PTCP_MULTICAST_ADDR_ANNOUNCE};
	LSA_UINT16		FrameId = FRAMEID_ANNOUNCE | gPTPd->pChUsr->SyncId;
	
	memset(pPdu, 0, ANNOUNCE_PDU_SIZE);

	DstAddr.MacAdr[5] |= gPTPd->pChUsr->SyncId;
	pPdu->PduHdr.DstMacAdr = DstAddr;
	
	Bsp_GetMacAddr((void*)&gPTPd->pChUsr->pChSys->MACAddr);
	memcpy(&pPdu->PduHdr.SrcMacAdr, &gPTPd->pChUsr->pChSys->MACAddr, sizeof(MAC_ADR_TYPE));
	
//	{
//		GSY_VLAN_TAG_TYPE	VlanTag = {GSY_VLAN_TAG};
//		pPdu->PduHdr.VlanTag = VlanTag;
//	}
	
	pPdu->PduHdr.EthType.Word = htons(GSY_ETHETYPE);
	pPdu->PduHdr.FrameId.Word = FrameId;	

	memcpy(&pPdu->PtcpData.Announce.Subdomain.SubdomainUUID, &gPTPd->pChUsr->SyncData.SubdomainID, sizeof(PTCP_UUID));
	memcpy(&pPdu->PtcpData.Announce.Subdomain.MasterSourceAddress, &gPTPd->pChUsr->pChSys->MACAddr, sizeof(MAC_ADR_TYPE));

	pPdu->PtcpData.Announce.Subdomain.TypeLen.Word    = htons(TLV_SUBDOMAIN_TYPELEN);
	
	pPdu->PtcpData.Announce.Master.MasterPriority1    = gPTPd->pChUsr->Master.LocalMaster.Priority1;
	pPdu->PtcpData.Announce.Master.MasterPriority2    = gPTPd->pChUsr->Master.LocalMaster.Priority2;
	pPdu->PtcpData.Announce.Master.ClockClass         = gPTPd->pChUsr->Master.LocalMaster.Class;
	pPdu->PtcpData.Announce.Master.ClockAccuracy      = gPTPd->pChUsr->Master.LocalMaster.Accuracy;
	pPdu->PtcpData.Announce.Master.ClockVariance.Word = htons(gPTPd->pChUsr->Master.LocalMaster.Variance);
	pPdu->PtcpData.Announce.Master.TypeLen.Word       = htons(TLV_MASTER_TYPELEN);

	pPdu->PtcpData.Announce.End.TypeLen.Word = htons(TLV_END_TYPELEN);
	
	if (0 == sendto(gPTPd->txBufAnno, ANNOUNCE_PDU_SIZE, 3, NULL, gPTPd))
	{
		++gPTPd->sendError;
	}
	else
	{
		++gPTPd->bmc.annoSeqNo;
	}
}
#endif
/*

  Relevant attributes in 802.1AS

  a) Priority1
  b) Class (formerly called stratum): Attribute defining a clock’s TAI traceability
  c) Time Source (formerly called clockIdentifier): Attribute defining the accuracy of a clock
  d) Variance (PTP Variance): An attribute defining the precision of a clock
  e) Whether the clock is a Boundary or an Ordinary clock
  f) Priority2: This is a user configurable designation that provides finer
     grained ordering among otherwise equivalent clocks
  g) Port Identity (formerly called UUID)

  The boundary clock participates in best master selection and invokes the BMCA, while the 
  peer-to-peer transparent clock does not participate in best master selection (11.1.3)

*/
bool updateAnnounceInfo(struct gPTPd* gPTPd)  // We are not a TC, see e) above
{
  bool gmFound = FALSE;

	struct gPTPPrioVec *gnPrio = (struct gPTPPrioVec *)&gPTPd->rxBuf[GPTP_BODY_OFFSET];

	if(gnPrio->prio1 < gPTPd->bmc.portPrio.prio1)
		gmFound = TRUE;
	else if(gnPrio->clockQual.clockClass < gPTPd->bmc.portPrio.clockQual.clockClass)
		gmFound = TRUE;
	else if(gnPrio->clockQual.clockAccuracy < gPTPd->bmc.portPrio.clockQual.clockAccuracy)
		gmFound = TRUE;
	else if(htons(gnPrio->clockQual.offsetScaledLogVariance) < gPTPd->bmc.portPrio.clockQual.offsetScaledLogVariance)
		gmFound = TRUE;
	else if(gnPrio->prio2 < gPTPd->bmc.portPrio.prio2)
		gmFound = TRUE;
	else 
	{
		for(int i = 0; ((i < GPTP_PORT_IDEN_LEN) && (gmFound == FALSE)); i++) 
		{
			if(gnPrio->iden[i] < gPTPd->bmc.portPrio.iden[i])
			{
				gmFound = TRUE;
			}
			else if(gnPrio->iden[i] > gPTPd->bmc.portPrio.iden[i])
			{
				break;
			}
			else
			{
				continue;
			}
		}	
	}

	if(gmFound == TRUE) 
	{
		memcpy(&gPTPd->bmc.gmPrio, gnPrio, sizeof(struct gPTPPrioVec));
	} 
	else 
	{
		memcpy(&gPTPd->bmc.gmPrio, &gPTPd->bmc.portPrio, sizeof(struct gPTPPrioVec));
	}
	
	return gmFound;
}
	
void updatePrioVectors(struct gPTPd* gPTPd)
{
  struct gPTPHdr *gh = (struct gPTPHdr *)&gPTPd->txBufAnno[sizeof(struct ethhdr)];

  gPTPd->bmc.portPrio.currUTCOff = TAIUTC;
  gPTPd->bmc.portPrio.prio1      = GPTP_DEFAULT_CLOCK_PRIO1;
	
  gPTPd->bmc.portPrio.clockQual.clockClass              = GPTP_DEFAULT_CLOCK_CLASS;
  gPTPd->bmc.portPrio.clockQual.clockAccuracy           = GPTP_DEFAULT_CLOCK_ACCURACY;
  gPTPd->bmc.portPrio.clockQual.offsetScaledLogVariance = GPTP_DEFAULT_OFFSET_VARIANCE;
	
  gPTPd->bmc.portPrio.prio2 = GPTP_DEFAULT_CLOCK_PRIO2;
  memcpy(&gPTPd->bmc.portPrio.iden[0], &gh->h.f.srcPortIden[0], GPTP_PORT_IDEN_LEN);
  gPTPd->bmc.portPrio.stepsRem = GPTP_DEFAULT_STEPS_REMOVED;
  gPTPd->bmc.portPrio.clockSrc = GPTP_CLOCK_TYPE_INT_OSC;

  memcpy(&gPTPd->bmc.gmPrio, &gPTPd->bmc.portPrio, sizeof(struct gPTPPrioVec));
}


