/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
/*  This program is protected by German copyright law and international      */
/*  treaties. The use of this software including but not limited to its      */
/*  Source Code is subject to restrictions as agreed in the license          */
/*  agreement between you and Siemens.                                       */
/*  Copying or distribution is not allowed unless expressly permitted        */
/*  according to your license agreement with Siemens.                        */
/*****************************************************************************/
/*                                                                           */
/*  P r o j e c t         &P: PROFINET IO Runtime Software              :P&  */
/*                                                                           */
/*  P a c k a g e         &W: PROFINET IO Runtime Software              :W&  */
/*                                                                           */
/*  C o m p o n e n t     &C: GSY (Generic Sync Module)                 :C&  */
/*                                                                           */
/*  F i l e               &F: gsy_low.c                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  Lower Interface (to EDD)                                                 */
/*                                                                           */
/*****************************************************************************/
#ifdef GSY_MESSAGE /**********************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Version                  Who  What                           */
/*  2004-11-29  P01.00.00.00_00.01.00.00 lrg  file creation                  */
/*  2005-04-04  P01.00.00.00_00.01.00.01 lrg  Umstellung auf TLVs            */
/*  2005-05-03  P03.05.00.00_00.03.00.01 lrg  gsy_UserDelayInd() bei der     */
/*              lower DelaySet-Confirmation aufrufen.                        */
/*  2005-06-02  P03.06.00.00_00.03.00.01 lrg  Prototype for PT2              */
/*  2005-06-15  P03.06.00.00_00.03.02.01 lrg  OpenChannel trotz RxTxNanos=0  */
/*  2005-07-26  P03.07.00.00_00.04.01.01 lrg  gsy_OpenLowerError() setzt     */
/*              Multicastadressen im EDD wieder zurueck.                     */
/*  2005-09-14  P03.07.00.00_00.08.01.01 js   Initialisierung einiger        */
/*                                            Variablen um Compiler Warning  */
/*                                            zu verhindern.                 */
/*  2005-10-27  P03.07.00.00_00.09.01.01 lrg  gsy_PortStatus() Delaymessung  */
/*                                            wird nur im FULL_DUPLEX-Mode   */
/*                                            gestartet. Indication an fwd.  */
/*  2005-11-18  P03.08.00.00_00.01.02.01 lrg  181105lrg001                   */
/*  2006-02-15  P03.09.00.00_00.02.01.02 lrg  gsy_FwdRcvIndication() nur im  */
/*              State GSY_CHA_STATE_OPEN aufrufen.                           */
/*  2006-03-29  P03.10.00.00_00.02.01.02 lrg PortFwd[] von gsy_detail nach   */
/*              GSY_CH_SYS verschoben                                        */
/*  2006-05-12  P03.11.00.00_00.03.01.02 lrg #ifndef GSY_TEST_MODE_FULL      */
/*  2006-05-15  P03.11.00.00_00.03.01.03 lrg GSY_LOWER_RQB_PORT_MACADDR_GET()*/
/*              gsy_OpenLowerDelay(): 2 RQBs fuer die EDD-Diagnosewerte      */
/*  2006-06-28  P03.12.00.00_00.02.01.02 lrg GSY_FILE_SYSTEM_EXTENSION()     */
/*  2006-07-27  P03.12.00.00_00.25.02.02 lrg GSY_RT_CLASS3_SUPPORT_ON        */
/*  2006-08-06  P03.12.00.00_00.25.04.01 js  Avoid compiler warning with gcc */
/*  2006-09-26  P03.12.00.00_00.25.04.02 lrg Neu: GSY_LOWER_RTC2_PARAMS_GET  */
/*  2007-01-15  P03.12.00.00_00.40.06.02 lrg 150107lrg002: Arts-RQ 402778    */
/*              Neuen State nur bei Full Duplex Mode setzen, damit die       */
/*              Delaymessung nicht nach LinkUp beim naechsten Commit startet */
/*  2007-01-18  P03.12.00.00_00.40.07.02 180107lrg001: Test ohne Delay-FU    */
/*  2007-07-09  P04.00.00.00_00.30.01.00 090707lrg001: gsy_SendSyncConfirm() */
/*              _SEND_SYNC, _RECV_SYNC, _CANCEL_SYNC... 170707lrg001         */
/*  2007-08-09  P04.00.00.00_00.30.03.00 080907js001:  falscher sync nicht   */
/*              mit fatal abbrechen.                                         */
/*  2007-08-27  P04.00.00.00_00.30.03.02 270807lrg001:  GSY_LINK_UP_DISABLED */
/*              f�r Delaymessung ignorieren                                  */
/*  2007-09-13  P04.00.00.00_00.30.05.02 130907lrg001:  Keine Ueberpr�fung   */
/*              des Frameheaders mehr bei RECV_SYNC und SEND_SYNC            */
/*  2007-10-15  P04.00.00.00_00.30.05.02 151007lrg002: Traceausgabe TopoOk   */
/*  2008-01-11  P04.00.00.00_00.60.02.02 lrg: Trace +++ LNK NOTE_HIGH->NOTE  */
/*  2008-06-20  P04.00.00.00_00.80.05.02 lrg: GSY_MESSAGE                    */
/*  2008-06-24  P04.01.00.00_00.01.01.02 240608lrg001: ArtsRQ AP00536908     */
/*              DelaySet zwischen Senderequest und Confirmation verz�gern    */
/*  2008-06-24  P04.01.00.00_00.01.02.02 110708lrg001: Arts-RQ AP00664879    */
/*              ExtLinkInd: Speed check: Delaymeasurement only at 100MBit    */
/*  2008-07-24  P04.01.00.00_00.01.02.03 240708lrg001: ArtsRQ AP00676859     */
/*              Check ClockTopoOk at output port on forwarding.              */
/*  2008-07-29  P04.01.00.00_00.01.03.02 290708lrg001: GSY_LOWER_CANCEL_SYNC */
/*              ueberspringen wenn der EDD keine Zeitstempel beherrscht      */
/*  2008-08-25  P04.01.00.00_00.01.03.03 250808lrg001: ArtsRQ AP00691597     */
/*              Stop delay measurement at GSY_LINK_UP_DISABLED               */
/*              CheckIn 2008-10-17 after gen P04.01.00.00_00.02.02.01        */
/*  2008-11-28  P04.01.00.00_00.02.02.03 KRISC32-Unterstuetzung              */
/*  2008-12-04  P04.01.00.00_00.02.03.01 lrg: Package Version (PNIO)         */
/*  2008-12-10  P04.01.00.00_00.02.03.02 101208lrg001: Announce mit          */
/*              gsy_DataSend() versenden                                     */
/*  2009-04-08  P04.01.00.00_00.03.03.02 lrg: K32_RQB_TYPE_DIAG_REQ          */
/*  2009-04-23  P04.01.00.00_00.03.04.02 lrg: EDD_SRV_PTCP_xxx               */
/*              240409lrg001: PortRx/Tx-Delays nur noch aus ExtLinkInd holen */
/*  2009-05-06  P04.02.00.00_00.01.01.01 060509lrg001: LSA_UNUSED_ARG()      */
/*  2009-05-14  P04.02.00.00_00.01.01.02 140509lrg001: gsy_DiagIndK32()      */
/*  2009-07-01  P04.02.00.00_00.01.03.02 lrg: gsy_DiagIndK32()               */
/*              pChUsr = LSA_NULL bei Rate fuer SyncId 1                     */
/*  2009-08-20  P04.02.00.00_00.01.04.02 200809lrg001: gsy_DiagIndK32()      */
/*              Indicate slave Takeover to master                            */
/*  2009-09-29  P04.02.00.00_00.01.04.03 290909lrg001: Retry setting Drift   */
/*  2009-10-08  P04.02.00.00_00.01.04.04 lrg: GSY_HW_TYPE_SLAVE_MSK...       */
/*  2009-11-13  P05.00.00.00_00.01.01.02 131109lrg001: StateSync aus Slave-  */
/*              State bei GSY_DIAG_SOURCE_DRIFT in gsy_DiagIndK32()          */
/*  2009-12-11  P05.00.00.00_00.01.02.02  111209lrg001: ArtsRQ AP00909835    */
/*              Free lower KRISC32 diagnostic RQB if channel is closing      */
/*              141209lrg001: Diagnose: NO Subdomain if K32 slave is stopped */
/*  2010-03-03  P05.00.00.00_00.01.03.02 lrg: Announce channel               */
/*  2010-03-31  P05.00.00.00_00.01.04.02 310310lrg001: gsy_DiagIndK32()      */
/*              Indicate slave Timeout to master                             */
/*  2010-04-15  P05.00.00.00_00.01.04.03 150410lrg001:                       */
/*              GSY_SLAVE_STATE_SYNC_OUT ist sync                            */
/*              GSY_LOWER_RQB_SEND_PRIO_SET(), GSY_SEND_PRIO_ANNO            */
/*  2010-04-28  P05.00.00.00_00.01.04.04 280410lrg002:                       */
/*              reprovide the receive Rbl with invalid FrameId               */
/*  2010-06-23  P05.00.00.00_00.01.05.02 230610lrg001: gsy_DiagIndK32()      */
/*              TAKEOVER trace with LSA_TRACE_LEVEL_WARN                     */
/*              German comments translated to english                        */
/*  2010-07-20  P05.00.00.00_00.01.06.03 lrg: AP00999110 GSY_TRACE_SR removed*/
/*  2010-08-25  P05.00.00.00_00.01.07.02 250810lrg001: AP00697597            */
/*              German comments translated to english                        */
/*  2010-12-22  P05.00.00.00_00.03.12.02 lrg: AP01017485 Index-Trace         */
/*  2011-07-25  P05.00.00.00_00.05.38.02 lrg: GSY_PROC_ removed from Trace   */
/*  2011-09-27  P05.00.00.00_00.05.56.02 AP01191740 Trace parameter count    */
/*  2012-06-11  P05.00.00.00_00.05.97.02 AP01366052 LSA trace %012u -> %010u */
/*  2012-11-07  P05.02.00.00_00.04.xx.xx AP01447671 GSY_MAC_ADR_TYPE moved   */
/*              from gsy_cfg.h/txt to gsy_pck.h/txt                          */
/*  2012-11-07  P05.03.00.00_00.02.22.01 955647,956018 [GSY] GSY_START_TIMER */
/*              f�hrt zu ACP FATAL                                           */
/*  2014-11-05  P06.00.00.00_00.01.49.01 1126470 [GSY] Announce-MAC-Adressen */
/*              nur bei Masterprojektierung aktivieren                       */
/*  2015-05-11  V06.00, Inc05, Feature 1227823, Task 1311090                 */
/*              only one user channel for clock or time sync                 */
/*  2015-10-02  V06.00, Inc10, RQ 1460015: check GSY_SYNC_ID_SUPPORTED in    */
/*              gsy_DiagIndK32()                                             */
/*                                                                           */
/*****************************************************************************/
#endif /* GSY_MESSAGE */

/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/
#define LTRC_ACT_MODUL_ID 	3		/* GSY_MODULE_ID_LOW */
#define GSY_MODULE_ID 		LTRC_ACT_MODUL_ID 

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/
#include "gsy_inc.h"
#include "gsy_int.h"
GSY_FILE_SYSTEM_EXTENSION(GSY_MODULE_ID)

#include "r_ether_rzt2.h"
#include "gptpcmn.h"

/*****************************************************************************/
/* Internal access function: gsy_DequeLower()		                         */
/* Remove lower Request block from Queue                                     */
/*****************************************************************************/
LSA_VOID    GSY_LOCAL_FCT_ATTR gsy_DequeLower(
GSY_LOWER_QUEUE		  *	pQue,
GSY_LOWER_RQB_PTR_TYPE 	pRbl)
{
	GSY_LOWER_RQB_PTR_TYPE	pPrev;
	GSY_LOWER_RQB_PTR_TYPE  pNext;

	GSY_FUNCTION_TRACE_03(GSY_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT,"  > gsy_DequeLower() pRbu=%X pQue=%X Count=%d",
					pRbl, pQue, pQue->Count);

	/* Find Request block in the Queue...
	*/
	pNext = pQue->pFirst;
	while (!LSA_HOST_PTR_ARE_EQUAL(pNext, pRbl)
	   &&  !LSA_HOST_PTR_ARE_EQUAL(pNext, LSA_NULL))
	{
		pNext = GSY_LOWER_RQB_NEXT_GET(pNext);
	}
	/* ...and if found: remove it
	*/
	if (!LSA_HOST_PTR_ARE_EQUAL(pNext, LSA_NULL))
	{
		pNext = GSY_LOWER_RQB_NEXT_GET(pRbl);
		pPrev = GSY_LOWER_RQB_PREV_GET(pRbl);
		
		/* If a predecessor is present... 
		*/
		if (!LSA_HOST_PTR_ARE_EQUAL(pPrev, LSA_NULL))
		{
			/* ...set its successor to next element
			*/
			GSY_LOWER_RQB_NEXT_SET(pPrev, pNext);
		}
		else
		{
			/* The successor becomes first element of the queue
			*/
			pQue->pFirst = pNext;
		}
		/* If a successor is present... 
		*/
		if (!LSA_HOST_PTR_ARE_EQUAL(pNext, LSA_NULL))
		{
			/* ...set its predecessor to previous element
			*/
			GSY_LOWER_RQB_PREV_SET(pNext, pPrev);
		}
		else
		{
			/* The predecessor becomes last element of the queue
			*/
			pQue->pLast = pPrev;
		}
		/* Delete chain pointers of removed RQB
		*/
		GSY_LOWER_RQB_PREV_SET(pRbl, LSA_NULL);
		GSY_LOWER_RQB_NEXT_SET(pRbl, LSA_NULL);
		
		/* Queue now contains one less element
		*/
		pQue->Count--;
	}
}

/*****************************************************************************/
/* External access function: gsy_request_lower_done()                        */
/* A lower requestblock comes back from lower layer on sync channel.         */
/*****************************************************************************/
LSA_VOID GSY_LOWER_IN_FCT_ATTR gsy_request_lower_done(
GSY_LOWER_RQB_PTR_TYPE  pRbl)
{
	GSY_CH_SYS_PTR			pChSys;
	LSA_OPCODE_TYPE			Opcode;
	GSY_LOWER_SERVICE_TYPE  Service;
	LSA_UINT16				LowerRsp;
	LSA_HANDLE_TYPE			Handle;
	
  GSY_ENTER();

 	/* Read Opcode, Service, UserID and Response from lower RB
	*/
	Opcode   = GSY_LOWER_RQB_OPCODE_GET(pRbl);
	Service  = GSY_LOWER_RQB_SERVICE_GET(pRbl);
	LowerRsp = GSY_LOWER_RQB_RESPONSE_GET(pRbl);
	Handle   = GSY_LOWER_RQB_HANDLE_GET(pRbl);

	pChSys = gsy_ChSysGet(Handle);

	if (!LSA_HOST_PTR_ARE_EQUAL(pChSys, LSA_NULL))
	{
		switch (Opcode)
		{
		case GSY_LOWER_OPC_OPEN_CHANNEL:
			gsy_OpenLowerGet(pChSys, pRbl);
			break;

		case GSY_LOWER_OPC_CLOSE_CHANNEL:

			/* Finish Close Channel
			*/
			break;

		case GSY_LOWER_OPC_REQUEST:

			/* Decode Service
			*/
			switch (Service)
			{

			/* ----------------------------------------------------------------
			 * Enable/disable receiving of a multicast address on this channel
			*/
			case GSY_LOWER_MULTICAST:
				if ((pChSys->State == GSY_CHA_STATE_CLOSING)
				||  (pChSys->State == GSY_CHA_STATE_ERROR))
					gsy_CloseLower(pChSys, pRbl, LSA_FALSE);
				else if (!LSA_HOST_PTR_ARE_EQUAL(pChSys->pChAnno, LSA_NULL)
				&&  ((GSY_CHA_STATE_OPEN_FWD == pChSys->pChAnno->State)
				 || (GSY_CHA_STATE_CLOSE_FWD == pChSys->pChAnno->State)))
				{
					gsy_FreeLowerRb(pChSys, pRbl, LSA_FALSE);
					(LSA_VOID)gsy_FwdMCConfirmation(pChSys, LowerRsp);
				}
				else
				{
					gsy_OpenLowerDelay(pChSys, pRbl);
				}
				break;

			/* ----------------------------------------------------------------
			*/
			case GSY_LOWER_SYNC_SET:
			case GSY_LOWER_TIME_SET:
				break;

			case GSY_LOWER_PARAMS_GET:
				gsy_OpenLowerSet(pChSys, pRbl);
				break;

			case GSY_LOWER_PORT_PARAMS_GET:
				gsy_OpenLowerPort(pChSys, pRbl);
				break;

			case GSY_LOWER_DELAY_SET:
				gsy_EnqueLower(&pChSys->QueueLowerCtrl, pRbl);
				break;

			case GSY_LOWER_RATE_SET:
				break;

			case GSY_LOWER_LINK_STATUS_EXT:
				gsy_PortStatus(pChSys, pRbl);
				break;
			}
			break;
			
			/* End of: case GSY_LOWER_OPC_REQUEST
			*/
		}
	}
	else
	{
		/* No Channel
		*/
		//ERROR
	}

    GSY_EXIT();
}

/*****************************************************************************/
/* Internal access function: gsy_ChAnnoGet()	                             */
/* Get channel pointer to allocated announce channel structure               */
/*****************************************************************************/
GSY_CH_ANNO_PTR  GSY_LOCAL_FCT_ATTR  gsy_ChAnnoGet(LSA_HANDLE_TYPE  Handle)
{
	LSA_HANDLE_TYPE	ChId    = 0;
	GSY_CH_ANNO_PTR	pChAnno = LSA_NULL;
	
	/* Check Handle
	*/
	if (Handle > GSY_MAX_SYS_CHANNELS)
	{
		ChId = (Handle - GSY_MAX_SYS_CHANNELS) - 1;
		if(ChId < GSY_MAX_SYS_CHANNELS)
		{
			/* Build pointer to channel structure
			*/
			pChAnno = (GSY_CH_ANNO_PTR)(gsy_data.pChAnnoList + ChId);
			
			/* Is not valid if channel is not allocated          
			*/ 
			if (pChAnno->State == GSY_CHA_STATE_FREE)
			{
				pChAnno = LSA_NULL;
			}
		}
	}
	
	return(pChAnno);
}


/*****************************************************************************/
/* External access function: gsy_request_lower_anno_done()                   */
/* A lower requestblock comes back from lower layer on announce channel.     */
/*****************************************************************************/
LSA_VOID GSY_LOWER_IN_FCT_ATTR gsy_request_lower_anno_done(GSY_LOWER_RQB_PTR_TYPE  pRbl)
{
	GSY_CH_SYS_PTR			pChSys = LSA_NULL;
	GSY_CH_ANNO_PTR			pChAnno;
	LSA_OPCODE_TYPE			Opcode;
	LSA_HANDLE_TYPE			Handle;

  GSY_ENTER();

 	/* Get Opcode, Service, UserID und Response from lower RB
	*/
	Opcode   = GSY_LOWER_RQB_OPCODE_GET(pRbl);
	Handle   = GSY_LOWER_RQB_HANDLE_GET(pRbl);

	pChAnno = gsy_ChAnnoGet(Handle);
	
	if (!LSA_HOST_PTR_ARE_EQUAL(pChAnno, LSA_NULL))
	{
		pChSys = pChAnno->pChSys;
		
		if (LSA_HOST_PTR_ARE_EQUAL(pChSys, LSA_NULL))
		{
			//ERROR
		}
		else
		{
			switch (Opcode)
			{
			case GSY_LOWER_OPC_OPEN_CHANNEL:
				(LSA_VOID)gsy_OpenLowerFwd(pChSys, pRbl);
				break;				

			default:
				break;
			}
		}
	}
	else
	{
		/* Kein Channel
		*/
		//ERROR
	}

	GSY_EXIT();
}

/*****************************************************************************/
/* Internal function: gsy_PortStatus()                                       */
/* PortLinkStatusIndication RQB is back from lower layer.                    */
/*****************************************************************************/
LSA_VOID GSY_LOCAL_FCT_ATTR gsy_PortStatus(
GSY_CH_SYS_PTR 			pChSys,
GSY_LOWER_RQB_PTR_TYPE	pRbl)
{
	LSA_UINT32 CableDelay = GSY_LOWER_RQB_LINK_STATUS_CABLEDELAY_GET(pRbl);
	LSA_UINT32 LineDelay  = GSY_LOWER_RQB_LINK_STATUS_LINEDELAY_GET(pRbl) * pChSys->RxTxNanos;
	LSA_UINT16 Response   = GSY_LOWER_RQB_RESPONSE_GET(pRbl);
	LSA_UINT16 PortId     = GSY_LOWER_RQB_LINK_STATUS_PORT_GET(pRbl);
	LSA_UINT8	 Status     = GSY_LOWER_RQB_LINK_STATUS_STATUS_GET(pRbl);
	LSA_UINT8	 Mode       = GSY_LOWER_RQB_LINK_STATUS_MODE_GET(pRbl);
	LSA_UINT8	 Speed      = GSY_LOWER_RQB_LINK_STATUS_SPEED_GET(pRbl);		// 110708lrg001
	LSA_BOOL	 TopoOk     = GSY_LOWER_RQB_LINK_STATUS_TOPO_OK_GET(pRbl);
	
	LSA_UINT16	    Idx = PortId - 1;
	LSA_UINT8	PortState = pChSys->Port[Idx].State;

	if ((pChSys->State == GSY_CHA_STATE_CLOSING)
	||  (pChSys->State == GSY_CHA_STATE_ERROR)
	||  (Response == GSY_LOWER_RSP_OK_CANCEL))
	{
		/* Free LinkStatusIndication RQB
		*/
		gsy_FreeLowerRb(pChSys, pRbl, LSA_FALSE);
		pRbl = LSA_NULL;
	}
	else if (Response == GSY_LOWER_RSP_OK)
	{
		{
			/* Update line delay (in ticks) and cable delay measured.
			*/
			//TODO:-OS- remove CableDelay = LineDelay = 30;
			if ((CableDelay != pChSys->Port[Idx].CableDelay)
			||  (LineDelay  != pChSys->Port[Idx].SyncLineDelay))
			{				
				pChSys->Port[Idx].SumCount++; 
				pChSys->Port[Idx].CableDelay    = CableDelay;
				pChSys->Port[Idx].SyncLineDelay = LineDelay;
				
				if ((pChSys->Port[Idx].MaxDelay == 0) || (pChSys->Port[Idx].MaxDelay < CableDelay))
					pChSys->Port[Idx].MaxDelay = CableDelay;
				
				if ((pChSys->Port[Idx].MinDelay == 0) || (pChSys->Port[Idx].MinDelay > CableDelay))
					pChSys->Port[Idx].MinDelay = CableDelay;
				
				gsy_DelaySet(pChSys, PortId);
			}
		}

		switch (Status)
		{
		case GSY_LINK_DOWN:

			if ((GSY_PORT_STATE_UP     == PortState) 
			||  (GSY_PORT_STATE_CLOSED == PortState))
				{
				/* Stop Delay Measurement
				*/
				GSY_SYNC_TRACE_01(pChSys->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH,"--- gsy_PortStatus(%u) link state change ->DOWN", PortId);
			}
			pChSys->Port[Idx].State = GSY_PORT_STATE_DOWN;
			break;

		case GSY_LINK_UP:
		case GSY_LINK_UP_CLOSED:
									/* 270807lrg001: MRP switches port to DISABLED instead of CLOSED and
									 * GSY gets the response EDD_STS_ERR_TX on sending
		case GSY_LINK_UP_DISABLED:
									*/
			/* Update port delays
			*/
			pChSys->Port[Idx].RxDelay = GSY_LOWER_RQB_LINK_STATUS_RXDELAY_GET(pRbl);
			pChSys->Port[Idx].TxDelay = GSY_LOWER_RQB_LINK_STATUS_TXDELAY_GET(pRbl);
			
			if (pChSys->Port[Idx].ClockSyncOk != TopoOk)
			{
				/* 151007lrg002: Trace
				*/
				GSY_SYNC_TRACE_02(pChSys->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH,"--- gsy_PortStatus(%u) topo state change ->[%d]", PortId, TopoOk);
				pChSys->Port[Idx].ClockSyncOk = TopoOk;
			}

			if (

#ifndef GSY_TEST_MODE_FULL
				(GSY_LINK_MODE_FULL == Mode) &&
				(EDD_LINK_SPEED_100 == Speed) &&  
#endif                                  
				((GSY_PORT_STATE_DOWN == PortState) 
			 ||  (GSY_PORT_STATE_DISABLED == PortState)
			 ||  (GSY_PORT_STATE_INIT == PortState)))   
			{
				GSY_SYNC_TRACE_02(pChSys->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH,"--- gsy_PortStatus(%u) link state change ->[%u]", PortId, Status);

				/* Start Delay-Measurement on LINK_UP_xxx after DOWN or INIT state
				 * 150107lrg002: Set new State only on Mode = Full Duplex,
				 * so that the Delay-Measurement starts not on the next Commit
				 * 110708lrg001: Check Speed: Delay-Measurement only at 100MHz
				 * 250808lrg001: Restart Delay-Measurement after DISABLED
				*/

				if (GSY_LINK_UP_CLOSED == Status)
					pChSys->Port[Idx].State = GSY_PORT_STATE_CLOSED;
				else
					pChSys->Port[Idx].State = GSY_PORT_STATE_UP;    
			}
			break;

		case GSY_LINK_UP_DISABLED:

			if ((GSY_PORT_STATE_UP == PortState) 
			||  (GSY_PORT_STATE_CLOSED == PortState))
			{

				/* 250808lrg001: Stop Delay-Measurement on GSY_LINK_UP_DISABLED
				*/
				//-OS-gsy_DelayReqStop(pChSys, PortId, GSY_LOWER_DELAY_ACTIVE);
			}
			pChSys->Port[Idx].State = GSY_PORT_STATE_DISABLED;
			break;

		default:
			break;

		}
	}
	else
	{
		/* ExtLinkIndication with error
		*/
		//ERROR
	}

	if(!LSA_HOST_PTR_ARE_EQUAL(pRbl, LSA_NULL))
	{
		/* Return RQB to lower Layer
		*/
		GSY_LOWER_RQB_HANDLE_SET      (pRbl, 0); /* sanity */
		GSY_LOWER_RQB_LOWER_HANDLE_SET(pRbl, pChSys->HandleLower);
		GSY_LOWER_RQB_ERR_COMPID_SET  (pRbl, LSA_COMP_ID_UNUSED);
		GSY_LOWER_RQB_OPCODE_SET      (pRbl, GSY_LOWER_OPC_REQUEST);
		GSY_LOWER_RQB_SERVICE_SET     (pRbl, GSY_LOWER_LINK_STATUS_EXT);

		/* Send request
		*/
		GSY_REQUEST_LOWER(pRbl, pChSys->pSys);
	}

	LSA_UNUSED_ARG(Speed);
}

/*****************************************************************************/
/* Internal access function: gsy_EnqueLower()	                             */
/* Store lower request block in queue                                        */
/*****************************************************************************/
LSA_VOID GSY_LOCAL_FCT_ATTR gsy_EnqueLower(
GSY_LOWER_QUEUE		  *	pQue,
GSY_LOWER_RQB_PTR_TYPE	pRbl)
{
	GSY_LOWER_RQB_PTR_TYPE  pLast;

	GSY_FUNCTION_TRACE_03(GSY_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT,"  > gsy_EnqueLower() pRbu=%X pQue=%X Count=%d",
					pRbl, pQue, pQue->Count);

	/* Append RQB to the lower Queue
	*/
	pLast = pQue->pLast;

	/* Mark new element as Queue end
	*/
	GSY_LOWER_RQB_NEXT_SET(pRbl, LSA_NULL);
	GSY_LOWER_RQB_PREV_SET(pRbl, pLast);
	pQue->pLast = pRbl;
	
	/* If the queue was empty... 
	*/
	if (LSA_HOST_PTR_ARE_EQUAL(pLast, LSA_NULL))
	{
		/* ...the last element is also th first one
		*/
		pQue->pFirst = pRbl;
	}
	else
	{
		/* Connect the new last to the old last element
		*/
		GSY_LOWER_RQB_NEXT_SET(pLast, pRbl);
	}
	/* Queue now contains one more element
	*/
	pQue->Count++;

	GSY_FUNCTION_TRACE_03(GSY_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT,"  < gsy_EnqueLower() pRbu=%X pQue=%X Count=%d",
					pRbl, pQue, pQue->Count);
}

/*****************************************************************************/
/* Internal access function: gsy_OpenLowerStart()                            */
/* Starts lower open channel by allocating a lower request block.            */
/*****************************************************************************/
LSA_UINT16 GSY_LOCAL_FCT_ATTR gsy_OpenLowerStart(
GSY_CH_SYS_PTR  			pChSys,
LSA_BOOL					ChAnno)
{
	LSA_USER_ID_TYPE        UserId;
	GSY_LOWER_RQB_PTR_TYPE	pRbl;
	GSY_LOWER_QUEUE			*pQueue;
	LSA_SYS_PTR_TYPE		pSys;
	LSA_UINT16 				RbuRespVal = GSY_RSP_OK_ACTIVE;

	GSY_FUNCTION_TRACE_02(pChSys->TraceIdx, LSA_TRACE_LEVEL_CHAT,"  > gsy_OpenLowerStart() pChSys=%X ChAnno=%u", 
					pChSys, ChAnno);

	pRbl = LSA_NULL;
	UserId.uvar32 = 0;
	
	if (ChAnno)
	{
		UserId.uvar8_array[GSY_HANDLE_UID] = pChSys->pChAnno->Handle;
		pSys = pChSys->pChAnno->pSys;
		pQueue = &pChSys->pChAnno->QueueLower; 
	}
	else
	{
		UserId.uvar8_array[GSY_HANDLE_UID] = pChSys->Handle;
		pSys = pChSys->pSys;
		pQueue = &pChSys->QueueLowerSend; 
	}

	/* Request lower RQB
	*/
	GSY_ALLOC_LOWER_RQB(&pRbl, UserId, sizeof(GSY_LOWER_RQB_TYPE), pSys);

	/* If the lower RB has been delivered synchronously...
	*/
	if(!LSA_HOST_PTR_ARE_EQUAL(pRbl, LSA_NULL))
	{
		/* ...put it into the channels queue and request lower parameter block
		*/
		GSY_LOWER_RQB_USERID_UVAR32_SET(pRbl, UserId.uvar32);
		gsy_EnqueLower(pQueue, pRbl);
		RbuRespVal = gsy_OpenLowerMem(pChSys, ChAnno);
	}
	else
	{
		/* Next step: get RQB asynchronously by gsy_lower_rqb()
		 * is Currently not supported!
		*/
		RbuRespVal = GSY_RSP_ERR_RESOURCE;
	}
	
	return(RbuRespVal);
}

/*****************************************************************************/
/* Internal function: gsy_OpenLowerMem()                                     */
/* Allocate lower memory for parameter block.                                */
/*****************************************************************************/

LSA_UINT16 GSY_LOCAL_FCT_ATTR gsy_OpenLowerMem(
GSY_CH_SYS_PTR  			pChSys,
LSA_BOOL					ChAnno)
{
	LSA_USER_ID_TYPE        UserId;
	GSY_LOWER_RQB_PTR_TYPE	pRbl;
	GSY_LOWER_MEM_PTR_TYPE	pMem;
	GSY_LOWER_QUEUE			*pQueue;
	LSA_SYS_PTR_TYPE		pSys;
	LSA_UINT16 				RbuRespVal = GSY_RSP_OK_ACTIVE;

	GSY_FUNCTION_TRACE_02(pChSys->TraceIdx, LSA_TRACE_LEVEL_CHAT,"  > gsy_OpenLowerMem() pChSys=%X ChAnno=%u", 
					pChSys, ChAnno);

	UserId.uvar32 = 0;
	if (ChAnno)
	{
		UserId.uvar8_array[GSY_HANDLE_UID] = pChSys->pChAnno->Handle;
		pSys = pChSys->pChAnno->pSys;
		pQueue = &pChSys->pChAnno->QueueLower; 
	}
	else
	{
		UserId.uvar8_array[GSY_HANDLE_UID] = pChSys->Handle;
		pSys = pChSys->pSys;
		pQueue = &pChSys->QueueLowerSend; 
	}
	pMem = LSA_NULL;

	/* Request lower memory
	*/
	GSY_ALLOC_LOWER_MEM(&pMem, UserId, sizeof(GSY_LOWER_PARAM_TYPE), pSys);

	/* ...put pointer to the memory or NULL into queued RQB 
	*/
	pRbl = pQueue->pFirst;
	GSY_LOWER_RQB_PPARAM_SET(pRbl, pMem);

	/* If the lower PB has been delivered synchronously...
	*/
	if(!LSA_HOST_PTR_ARE_EQUAL(pMem, LSA_NULL))
	{
		/* ...open lower channel
		*/
		gsy_OpenLowerCh(pChSys, ChAnno);
	}
	else
	{
		/* Next step: get RQB asynchronously by gsy_lower_mem()
		 * is Currently not supported!
		*/
		gsy_DequeLower(pQueue, pRbl);
		GSY_FREE_LOWER_RQB(&RbuRespVal, pRbl, pSys);
		RbuRespVal = GSY_RSP_ERR_RESOURCE;
	}
	
	return(RbuRespVal);
}

/*****************************************************************************/
/* Internal function: gsy_OpenLowerCh()                                      */
/* Open lower channel.                                                       */
/*****************************************************************************/

LSA_VOID GSY_LOCAL_FCT_ATTR gsy_OpenLowerCh(GSY_CH_SYS_PTR pChSys, LSA_BOOL ChAnno)
{
	GSY_LOWER_RQB_PTR_TYPE	pRbl;
	GSY_LOWER_QUEUE			   *pQueue;
	LSA_SYS_PTR_TYPE		    pSys;

	if (ChAnno)
	{
		pSys = pChSys->pChAnno->pSys;
		pQueue = &pChSys->pChAnno->QueueLower; 
	}
	else
	{
		pSys = pChSys->pSys;
		pQueue = &pChSys->QueueLowerSend; 
	}
	
	pRbl = pQueue->pFirst;
	
	/* Fill lower requst block for open_channel()
	*/
	GSY_LOWER_RQB_OPCODE_SET(pRbl, GSY_LOWER_OPC_OPEN_CHANNEL);
	GSY_LOWER_RQB_SERVICE_SET(pRbl, 0);
	GSY_LOWER_RQB_ERR_COMPID_SET(pRbl, LSA_COMP_ID_UNUSED);

	if (ChAnno)
	{
		GSY_LOWER_RQB_HANDLE_SET(pRbl, pChSys->pChAnno->Handle);
		GSY_LOWER_RQB_HANDLE_UPPER_SET(pRbl, pChSys->pChAnno->Handle);
		GSY_LOWER_RQB_SYS_PATH_SET(pRbl, pChSys->pChAnno->SysPath);
		GSY_LOWER_RQB_CBF_PTR_SET(pRbl, gsy_request_lower_anno_done);
	}
	else
	{
		GSY_LOWER_RQB_HANDLE_SET(pRbl, pChSys->Handle);
		GSY_LOWER_RQB_HANDLE_UPPER_SET(pRbl, pChSys->Handle);
		GSY_LOWER_RQB_SYS_PATH_SET(pRbl, pChSys->SysPath);
		GSY_LOWER_RQB_CBF_PTR_SET(pRbl, gsy_request_lower_done);
	}

	/* Get requst block from queue and send it to lower layer
	*/
	gsy_DequeLower(pQueue, pRbl);
	GSY_OPEN_CHANNEL_LOWER(pRbl, pSys);
}

/*****************************************************************************/
/* Internal function: gsy_OpenLowerGet()                                     */
/* Read parameters (MAC Addresse, number of ports) of the lower channel      */
/*****************************************************************************/

LSA_VOID GSY_LOCAL_FCT_ATTR gsy_OpenLowerGet(
GSY_CH_SYS_PTR 			pChSys,
GSY_LOWER_RQB_PTR_TYPE	pRbl)
{
	if (GSY_LOWER_RQB_RESPONSE_GET(pRbl) == GSY_LOWER_RSP_OK)
	{
		/* Store lower channel handle in channel data structure
		*/
		pChSys->HandleLower = GSY_LOWER_RQB_HANDLE_LOWER_GET(pRbl);

		/* Fill Lower Requstblock for REQUEST, Service: GetParams
		*/
		GSY_LOWER_RQB_OPCODE_SET(pRbl, GSY_LOWER_OPC_REQUEST);
		 GSY_LOWER_RQB_HANDLE_SET(pRbl, 0); /* sanity */
		GSY_LOWER_RQB_LOWER_HANDLE_SET(pRbl, pChSys->HandleLower);
		GSY_LOWER_RQB_SERVICE_SET(pRbl, GSY_LOWER_PARAMS_GET);
		GSY_LOWER_RQB_ERR_COMPID_SET(pRbl, LSA_COMP_ID_UNUSED);

		/* Send Lower Request
		*/
		GSY_REQUEST_LOWER(pRbl, pChSys->pSys);

		/* On Callback: gsy_request_lower_done() and gsy_LowerSet()
		*/
	}
	else
	{
		/* Error opening Lower Layer Channel
		*/
		//ERROR
	}

	GSY_FUNCTION_TRACE_02(pChSys->TraceIdx, LSA_TRACE_LEVEL_CHAT,"  < gsy_OpenLowerGet() pChSys=%X pRbl=%X", 
					pChSys, pRbl);
}

/*****************************************************************************/
/* Internal function: gsy_OpenLowerSet()                                     */
/* Activate multicast address for Delay Frames.                              */
/*****************************************************************************/

LSA_VOID GSY_LOCAL_FCT_ATTR gsy_OpenLowerSet(
																						 GSY_CH_SYS_PTR 			pChSys,
																						 GSY_LOWER_RQB_PTR_TYPE	pRbl)
{
	if (GSY_LOWER_RQB_RESPONSE_GET(pRbl) == GSY_LOWER_RSP_OK)
	{
		/* Store parameters of the channel
		*/
		pChSys->PortCount = GSY_LOWER_RQB_PARAM_PORTS_GET(pRbl);
		GSY_LOWER_RQB_PARAM_MACADDR_GET(pChSys->MACAddr, pRbl);			//041112lrg001
		pChSys->RxTxNanos = GSY_LOWER_RQB_PARAM_NANOSECONDS_GET(pRbl);
		pChSys->HardwareType = GSY_LOWER_RQB_PARAM_HWTYPE_GET(pRbl);
		pChSys->TraceIdx = GSY_LOWER_RQB_PARAM_TRACEIDX_GET(pRbl);
		
		if ((pChSys->PortCount > GSY_CH_MAX_PORTS)
#if 0 //
				||  ((GSY_HW_TYPE_MASTER_HW == (pChSys->HardwareType & GSY_HW_TYPE_MASTER_MSK))
						 &&  (GSY_HW_TYPE_SLAVE_HW != (pChSys->HardwareType & GSY_HW_TYPE_SLAVE_MSK)))
#endif
					)
			
		{
			/* Too many ports or wrong HW sync type
			*/
			//ERROR
		}
		else if (1/*-OS- GSY_HW_TYPE_DELAY_HW == (pChSys->HardwareType & GSY_HW_TYPE_DELAY_MSK)*/)
		{
			/* Do not enable delay multicast address on system channels
			* with delay measurement in hardware (KRISC32)
			*/
			(LSA_VOID)gsy_OpenLowerDelay(pChSys, pRbl);
		}
	}
	else
	{
		/* Error reading parameters of lower channel 
		*/
		gsy_OpenLowerError(pChSys, pRbl, GSY_RSP_ERR_LOWER_LAYER);
	}
}

/*****************************************************************************/
/* Internal function: gsy_OpenLowerDelay()                                   */
/* Allocate delay timer for every port and get port parameter.               */
/*****************************************************************************/
LSA_UINT16 GSY_LOCAL_FCT_ATTR gsy_OpenLowerDelay(
																								 GSY_CH_SYS_PTR  		pChSys,
																								 GSY_LOWER_RQB_PTR_TYPE	pRbl)
{
	GSY_LOWER_RQB_PTR_TYPE	pRb;
	LSA_INT					  NumRqb;
	LSA_UINT16 				PortId;
	LSA_UINT16 				RetVal;
	LSA_UINT16 				RbuRespVal = GSY_RSP_OK;
	
	if (GSY_LOWER_RQB_RESPONSE_GET(pRbl) != GSY_LOWER_RSP_OK)
	{
		/* Error enableing delay multicast address
		*/
		RbuRespVal = GSY_RSP_ERR_LOWER_LAYER;
	}
	else
	{
		/* State is open lower
		*/
		pChSys->State = GSY_CHA_STATE_OPEN_LOW;
	}
	
	if (  (RbuRespVal == GSY_RSP_OK)
		&&  (pChSys->RxTxNanos != 0))
	{
		/* To get PHY- and MAC Delays of all ports...
		*/
		for (PortId = 0; PortId < pChSys->PortCount; PortId++)
		{
			/* ...allocate new RQB with memory for parameters 
			* and put it into lower Queue
			*/
			//-OS- alloc needed dont remove
			RetVal = gsy_AllocEthParam(pChSys, &pRb, LSA_FALSE);
			
			if (LSA_RET_OK == RetVal)
			{
				gsy_EnqueLower(&pChSys->QueueLowerSend, pRb);
			}
			else
			{
				RbuRespVal = GSY_RSP_ERR_RESOURCE;
				break;
			}
			
			/* Allocate RQBs with memory for parameters for DelaySet requests
			* and put it into lower Queue
			*/
			for (NumRqb = 0; NumRqb < GSY_NUM_RQB_DELAY_SET; NumRqb++)
			{
				//-OS- alloc needed dont remove
				RetVal = gsy_AllocEthParam(pChSys, &pRb, LSA_FALSE);
				
				if (LSA_RET_OK == RetVal)
				{ 
					gsy_EnqueLower(&pChSys->QueueLowerCtrl, pRb);
				}
				else
					/* break inner loop
					*/
					break;
			}
			
			if (LSA_RET_OK != RetVal)
			{
				/* break outer loop
				*/
				RbuRespVal = GSY_RSP_ERR_RESOURCE;
				break;
			}
		}
		
		if (RbuRespVal == GSY_RSP_OK)
		{
			/* Allocate RQBs with memory for parameters for Rate(Drift)Set requests
			* for ClockSync and put it into lower Queue
			*/
			for (NumRqb = 0; NumRqb < GSY_NUM_RQB_RATE_SET; NumRqb++)
			{
				//-OS- alloc needed dont remove.
				RetVal = gsy_AllocEthParam(pChSys, &pRb, LSA_FALSE);
				
				if (LSA_RET_OK == RetVal)
					gsy_EnqueLower(&pChSys->QueueLowerCtrl, pRb);
				else
					break;
			}
			
			if (RbuRespVal == GSY_RSP_OK)
			{
				/* Request PHY- and MAC Delays of all ports
				*/
				for (PortId = 1; PortId <= pChSys->PortCount; PortId++)
				{
					/* Dequeue Lower RQB 
					* and fill for REQUEST, Service: GetPortParams
					*/
					pRb = pChSys->QueueLowerSend.pFirst;
					gsy_DequeLower(&pChSys->QueueLowerSend, pRb);
					
					GSY_LOWER_RQB_OPCODE_SET(pRb, GSY_LOWER_OPC_REQUEST);
					GSY_LOWER_RQB_HANDLE_SET(pRbl, 0); /* sanity */
					GSY_LOWER_RQB_LOWER_HANDLE_SET(pRb, pChSys->HandleLower);
					GSY_LOWER_RQB_SERVICE_SET(pRb, GSY_LOWER_PORT_PARAMS_GET);
					GSY_LOWER_RQB_PORT_DELAY_ID_SET(pRb, PortId);
					GSY_LOWER_RQB_ERR_COMPID_SET(pRb, LSA_COMP_ID_UNUSED);
					
					/* Send Request
					*/
					GSY_REQUEST_LOWER(pRb, pChSys->pSys);
					
					/* On callback: gsy_request_lower_done() and gsy_OpenLowerFwd()
					*/
				}
			}
		}
	}
	
	if (RbuRespVal == GSY_RSP_OK)
	{
		/* Store Lower Open Channel RQB for close channel
		*/
		pChSys->pOpenRbl = pRbl;
		
		if (pChSys->RxTxNanos == 0)
		{
			/* If the lower layer supports no timestamps:
			* finish OPEN_CHANNEL now with OK without allocating
			* delay timers and Receive RQBs. PortInit remains = 0. 
			*/
			gsy_CallbackSys(pChSys, LSA_NULL, GSY_RSP_OK);
		}
	}
	else
	{
		gsy_OpenLowerError(pChSys, pRbl, RbuRespVal);
	}
	
	return(RbuRespVal);
}

/*****************************************************************************/
/* Internal function: gsy_OpenLowerPort()                                    */
/* Store Port delays, allocate Receive-RQBs and send them to lower layer,    */
/* allocate Send RQBs and queue them.                                        */
/* Send one Link-Status-Indication-Request to lower layer.                   */
/*****************************************************************************/
LSA_UINT16 GSY_LOCAL_FCT_ATTR gsy_OpenLowerPort(
GSY_CH_SYS_PTR  		pChSys,
GSY_LOWER_RQB_PTR_TYPE	pRbl)
{
	LSA_UINT16 	PortId;
	LSA_UINT16 	RbuRespVal = pChSys->Resp;

	pChSys->PortInit++;

	if ((RbuRespVal == GSY_RSP_OK)
	&& (GSY_LOWER_RQB_RESPONSE_GET(pRbl) != GSY_LOWER_RSP_OK))
	{
		/* Error reading port parameter
		*/
		RbuRespVal = GSY_RSP_ERR_LOWER_LAYER;
	}

	if (RbuRespVal == GSY_RSP_OK)
	{
		/* Store Port-MAC-Address
		 * 240409lrg001: Get PortRx/Tx-Delays only from ExtLinkInd
		*/
		PortId = GSY_LOWER_RQB_PORT_DELAY_ID_GET(pRbl);
		GSY_LOWER_RQB_PORT_MACADDR_GET(pChSys->Port[PortId-1].MACAddr, pRbl);		//041112lrg001

		if (RbuRespVal == GSY_RSP_OK)
		{
			/* Fill Linkstatus-Indication-Request for this port
			 * 170707lrg001: ClockSyncOk from SyncTopoOk for SyncId 0 from ext.LinkInd
			*/
			GSY_LOWER_RQB_HANDLE_SET(pRbl, 0); /* sanity */
			GSY_LOWER_RQB_LOWER_HANDLE_SET(pRbl, pChSys->HandleLower);
			GSY_LOWER_RQB_ERR_COMPID_SET(pRbl, LSA_COMP_ID_UNUSED);
			GSY_LOWER_RQB_OPCODE_SET(pRbl, GSY_LOWER_OPC_REQUEST);
			GSY_LOWER_RQB_SERVICE_SET(pRbl, GSY_LOWER_LINK_STATUS_EXT);
			GSY_LOWER_RQB_LINK_STATUS_PORT_SET(pRbl, PortId);
			GSY_LOWER_RQB_LINK_STATUS_TOPO_OK_SET(pRbl, pChSys->Port[PortId-1].ClockSyncOk);
			GSY_LOWER_RQB_LINK_STATUS_ALL_SET_UNKNOWN(pRbl);

			/* Send Request
			 * 110210lrg002: stor RQB untisl Forwarding is initialised
			GSY_REQUEST_LOWER(pRbl, pChSys->pSys);
			*/
			pChSys->Port[PortId-1].pExtLinkRbl = pRbl;
		}
	}

	if (RbuRespVal != GSY_RSP_OK)
	{
		if (!LSA_HOST_PTR_ARE_EQUAL(pRbl, LSA_NULL))
		{
			gsy_FreeLowerRb(pChSys, pRbl, LSA_FALSE);
		}
		/* Store error until all PortParamsGet-Requests are back
		*/
		pChSys->Resp = RbuRespVal;
	}
	
	if (pChSys->PortInit == pChSys->PortCount)
	{
		/* If all ports are initialised: finish OPEN_CHANNEL
		*/
		gsy_OpenLowerEnd(pChSys, RbuRespVal);
	}

	return(RbuRespVal);
}

/*****************************************************************************/
/* Internal function: gsy_OpenLowerEnd()                                     */
/* If gsy_open_channel() has run til gsy_OpenLowerPort(), it finishes here.  */
/*****************************************************************************/
LSA_VOID GSY_LOCAL_FCT_ATTR gsy_OpenLowerEnd(
																						 GSY_CH_SYS_PTR	pChSys,
																						 LSA_UINT16		RbuRespVal)
{
	GSY_FUNCTION_TRACE_02(pChSys->TraceIdx, LSA_TRACE_LEVEL_CHAT,"  > gsy_OpenLowerEnd() pChSys=%X RbuRespVal=0x%x", 
												pChSys, RbuRespVal);
	
	if (RbuRespVal == GSY_RSP_OK)
	{
		/* Finish upper OpenSysChannel with response OK when
		* all PortParamsGet-Requests are back from lower layer
		*/
		gsy_CallbackSys(pChSys, LSA_NULL, RbuRespVal);
	}
	else
	{
		/* On error: start close channel by CANCEL of the Recv-RQBs
		*/
		//ERROR
	}
}

/*****************************************************************************/
/* Internal function: gsy_OpenLowerError()      		                     */
/* Undo lower channel open.                                                  */
/*****************************************************************************/
LSA_VOID GSY_LOCAL_FCT_ATTR gsy_OpenLowerError(
																							 GSY_CH_SYS_PTR  		    pChSys,
																							 GSY_LOWER_RQB_PTR_TYPE	pRbl,
																							 LSA_UINT16 				    RbuRespVal)
{
	LSA_UINT8				DelayMcAddr[] = GSY_MULTICAST_ADDR_DELAY;

	if (LSA_HOST_PTR_ARE_EQUAL(pRbl, LSA_NULL))
	{
		/* Use stored Open-RQB
		*/
		pRbl = pChSys->pOpenRbl;
		pChSys->pOpenRbl = LSA_NULL;
	}

	if (pChSys->State == GSY_CHA_STATE_OPEN_LOW)
	{
		/* Disable Delay-Multicastaddresse and close lower Channel
		*/
		pChSys->Resp  = RbuRespVal;
		pChSys->State = GSY_CHA_STATE_ERROR;
		
		if (GSY_HW_TYPE_DELAY_HW == (pChSys->HardwareType & GSY_HW_TYPE_DELAY_MSK))
		{
			/* Do not disable delay multicast address on system channels
			 * with delay measurement in hardware (KRISC32)
			*/
			//-OS-gsy_CloseLower(pChSys, pRbl, LSA_FALSE);
		}
		else
		{
			(LSA_VOID)gsy_MucAddrSet(pChSys,	pRbl, DelayMcAddr, LSA_FALSE, LSA_FALSE);
		}
	}
	else
	{
		pChSys->State = GSY_CHA_STATE_CLOSED;
		pChSys->Resp = RbuRespVal;
	}
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname  :    gsy_FwdMcControl                                    +*/
/*+  Input/Output  :    GSY_CH_SYS_PTR          pChSys                      +*/
/*+                     GSY_LOWER_RQB_PTR_TYPE  pRbl                        +*/
/*+                     LSA_BOOL                Enable                      +*/
/*+                     LSA_UINT16             *pRespone                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pChSys                 : Pointer to Systemchannel (to EDD)             +*/
/*+  pRbl                   : Pointer to EDD MC-RQB. LSA_NULL if none       +*/
/*+  Enable                 : LSA_TRUE: Enable MC                           +*/
/*+                           LSA_FALSE DisableMC                           +*/
/*+  pResponse              : Response                                      +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+  Result        :    LSA_FALSE: Ready, no MC-Request send to EDD         +*/
/*+                     LSA_TRUE : Tried to send MC-Request, check Response +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Sends MC enable/disable request to EDD                    +*/
/*+                                                                         +*/
/*+               Note: uses  pChSys->Fwd.MCSetIdx. Preset!                 +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_BOOL gsy_FwdMcControl(
																 GSY_CH_SYS_PTR                    pChSys,
																 GSY_LOWER_RQB_PTR_TYPE            pRbl,
																 LSA_BOOL                          Enable,
																 LSA_UINT16                      * pResponse)
{
	LSA_BOOL Ready;
	LSA_BOOL ChAnno = LSA_FALSE;
	
	GSY_MAC_ADR_TYPE    *pMac;
	GSY_MAC_ADR_TYPE    AddrSync = {GSY_MULTICAST_ADDR_SYNC};
	GSY_MAC_ADR_TYPE    AddrSyncFu = {GSY_MULTICAST_ADDR_FUSYNC};
	GSY_MAC_ADR_TYPE    AddrFu = {GSY_MULTICAST_ADDR_SYNC_FU};
	
	Ready      = LSA_TRUE;
	*pResponse = GSY_RSP_OK;
	
	if (pChSys->Fwd.MCSetIdx < GSY_FWD_MAC_ADDR_CNT)
	{
		{
			if ( pChSys->Fwd.MCEnabled[pChSys->Fwd.MCSetIdx].SyncEnabled != Enable )
			{
				pChSys->Fwd.MCSetSync = GSY_FWD_SYNC_SET;
				pMac = &AddrSync;
			}
			else if ( pChSys->Fwd.MCEnabled[pChSys->Fwd.MCSetIdx].SyncFuEnabled != Enable )
			{
				pChSys->Fwd.MCSetSync = GSY_FWD_SYNCFU_SET;
				pMac = &AddrSyncFu;
			}
			else
			{
				pChSys->Fwd.MCSetSync = GSY_FWD_FU_SET;
				pMac = &AddrFu;
			}
		}
		pMac->MacAdr[5] |= (LSA_UINT8)pChSys->Fwd.MCSetIdx;
		*pResponse = gsy_MucAddrSet(pChSys,pRbl, pMac->MacAdr, Enable, ChAnno);
	}
	else /* nothing to open (most likely a wrong config, but we continue)..*/
	{
		Ready = LSA_FALSE;
	}
	
	return(Ready);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname  :    gsy_FwdMCConfirmation                               +*/
/*+  Input/Output  :    GSY_CH_SYS_PTR          pChSys                      +*/
/*+                     GSY_LOWER_RQB_PTR_TYPE  pRbl                        +*/
/*+  Result        :    LSA_UINT16                                          +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pChSys                 : Pointer to Systemchannel (to EDD)             +*/
/*+  pRbl                   : Pointer to EDD MC-RQB                         +*/
/*+                                                                         +*/
/*+  Result     : GSY_RSP_OK                                                +*/
/*+               GSY_RSP_ERR_PARAM                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Indicates EDD-Multicast Confirmation                      +*/
/*+                                                                         +*/
/*+               - May issus another MC Request                            +*/
/*+               - May finish Channel init / exit                          +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 GSY_LOCAL_FCT_ATTR gsy_FwdMCConfirmation(
																										GSY_CH_SYS_PTR          pChSys,
																										LSA_UINT16              Response)
{
    LSA_BOOL   Enable;
    LSA_VOID   LSA_FCT_PTR(GSY_LOCAL_FCT_ATTR,Cbf)(GSY_CH_SYS_PTR  pChSys, LSA_UINT16 Response);

    Enable = LSA_TRUE;

    switch (pChSys->Fwd.Status)
    {
        case GSY_FWD_CHANNEL_CLOSING:
            Enable = LSA_FALSE;
            /* fall through */
            //lint -fallthrough

        case GSY_FWD_CHANNEL_OPENING:

            switch ( Response )
            {
                case GSY_LOWER_RSP_OK:

                    /* 280607lrg001: MAC address for sync frame without FollowUp
                     * 240907lrg001: MAC address for announce frame
                    */
                    if (GSY_FWD_SYNC_SET == pChSys->Fwd.MCSetSync)
                    {
                        pChSys->Fwd.MCEnabled[pChSys->Fwd.MCSetIdx].SyncEnabled = Enable;
                    }

                    else if (GSY_FWD_SYNCFU_SET == pChSys->Fwd.MCSetSync)
                    {
                        pChSys->Fwd.MCEnabled[pChSys->Fwd.MCSetIdx].SyncFuEnabled = Enable;
                    }
                    else
                    {
                        pChSys->Fwd.MCEnabled[pChSys->Fwd.MCSetIdx].FuEnabled = Enable;
                    }

                    {

                        if (0 == pChSys->Fwd.Cbf)
                        {
													//ERROR
                        }

                        if (Enable )
                        {
			
                            pChSys->Fwd.Status = GSY_FWD_CHANNEL_OPEN;

                            Cbf = pChSys->Fwd.Cbf;
                            pChSys->Fwd.Cbf = 0;

                            Cbf(pChSys,GSY_RSP_OK); /* call CBF, we finished */

                        }
                    }
                    break;

                default:
										//ERROR
                    break;
            }
            break;

        default:
            break;
    } /* switch */

    return(Response);
}

LSA_UINT16 GSY_LOCAL_FCT_ATTR gsy_FwdChannelInit(
																								 GSY_CH_SYS_PTR          pChSys,
																								 LSA_UINT16              PortCnt,
																								 GSY_FWD_INIT_PTR_TYPE   pPortInit,
																								 GSY_CBF_TYPE			Cbf)		
{
	GSY_FWD_FCB_PTR_TYPE  pFCB;
	GSY_FWD_PORT_PTR_TYPE pPort;
	GSY_FWD_INIT_PTR_TYPE pPortInitHelp;
	
	LSA_UINT16            Response;
	LSA_UINT32            FCBCnt;
	LSA_UINT32            FCBSize;
	LSA_UINT32            PortMgmSize;
	LSA_UINT32            i,y,k;
	LSA_BOOL              Ready;
	
	Response = GSY_RSP_OK;
	
	if ( ( PortCnt == 0 )                                ||
		   ( LSA_HOST_PTR_ARE_EQUAL(pChSys, LSA_NULL)    ) ||
			 ( (0 == Cbf)   )                                ||
		   ( LSA_HOST_PTR_ARE_EQUAL(pPortInit, LSA_NULL) ) ||
			 ( pChSys->RxTxNanos == 0 ))  /* timestamps needed ! */
	{
		Response = GSY_RSP_ERR_PARAM;
	}
	else
	{
		GSY_MEMSET_LOCAL(&pChSys->Fwd,0,sizeof(pChSys->Fwd));
		
		pChSys->Fwd.Status = GSY_FWD_CHANNEL_CLOSED;
		
		/* ----------------------------------------------------------------------*/
		/* Set Delay correction depending on hardware.                           */
		/* ----------------------------------------------------------------------*/
		
		switch (pChSys->HardwareType & GSY_HW_TYPE_MSK)
		{
		case GSY_HW_TYPE_STANDARD:
		case GSY_HW_TYPE_I210:
		case GSY_HW_TYPE_RZT2:
			pChSys->Fwd.DelayCorrectionMode = GSY_FWD_DELAY_CORRECTION_NO_HW_SUPPORT;
			break;
			
		case GSY_HW_TYPE_ERTEC_400:
			pChSys->Fwd.DelayCorrectionMode = GSY_FWD_DELAY_CORRECTION_ERTEC400;
			break;
			
		case GSY_HW_TYPE_ERTEC_200:
			
#ifndef GSY_CFG_FWD_ERTEC_200_DRIFT_CORRECTION
			pChSys->Fwd.DelayCorrectionMode = GSY_FWD_DELAY_CORRECTION_FULL_HW_SUPPORT;
#else
			pChSys->Fwd.DelayCorrectionMode = GSY_FWD_DELAY_CORRECTION_ERTEC200_DC;
#endif
			break;
			
		case GSY_HW_TYPE_SOC:
		case GSY_HW_TYPE_ERTEC_200P:
		case GSY_HW_TYPE_HERA:    
			pChSys->Fwd.DelayCorrectionMode = GSY_FWD_DELAY_CORRECTION_FULL_HW_SUPPORT;
			break;
			
		default:
			Response = GSY_RSP_ERR_PARAM;
			break;
		}
		
		/* ----------------------------------------------------------------------*/
		/* Init MC-Enabled infos                                                 */
		/* ----------------------------------------------------------------------*/
		
		for (i=0; i<GSY_FWD_MAC_ADDR_CNT; i++)
		{
			pChSys->Fwd.MCEnabled[i].SyncEnabled   = LSA_FALSE;
			pChSys->Fwd.MCEnabled[i].FuEnabled     = LSA_FALSE;
			pChSys->Fwd.MCEnabled[i].AnnoEnabled   = LSA_FALSE;
			pChSys->Fwd.MCEnabled[i].SyncFuEnabled = LSA_FALSE;
			pChSys->Fwd.MCEnabled[i].FwdUserMode   = GSY_FWD_MODE_DISABLE;
		}
		
		/* ----------------------------------------------------------------------*/
		/* Allocate PORT-Management for this channel                             */
		/* ----------------------------------------------------------------------*/
		
		PortMgmSize = PortCnt * sizeof(GSY_FWD_PORT_TYPE);
		
		pChSys->Fwd.PortCnt = PortCnt;
		
		if ( PortMgmSize <= 0xFFFF )
		{
			GSY_ALLOC_LOCAL_MEM((GSY_LOCAL_MEM_PTR_TYPE*)&pPort,(LSA_UINT16) PortMgmSize);
			
			if ( LSA_HOST_PTR_ARE_EQUAL(pPort, LSA_NULL) )
			{
				Response = GSY_RSP_ERR_RESOURCE;
				//GSY_ERROR_TRACE_00(pChSys->TraceIdx, LSA_TRACE_LEVEL_ERROR, "*** gsy_FwdChannelInit() Allocating memory failed!");
			}
			else
			{
				GSY_MEMSET_LOCAL(pPort,0,PortMgmSize);
				
				pChSys->Fwd.pPortMgm      = pPort;
				pChSys->Fwd.MaxLocalDelay = 0xFFFFFFFFUL / pChSys->RxTxNanos;
				
				pPortInitHelp = pPortInit;
				
				for (i=0; i<PortCnt; i++) /* all ports */
				{
#ifdef GSY_FWD_DBG_LINE_DELAY_SET_ON_INIT
					pPort->Status      = GSY_FWD_PORT_STS_LINE_DELAY_SET;
					pPort->LineDelayNS = i * 5 + 3000 + 3;
#else
					pPort->Status     = GSY_FWD_PORT_STS_LINE_DELAY_NOT_SET;
#endif
					pPort->ThisPortID  = (LSA_UINT16) (i+1);
					pPort->SendEnabled = LSA_TRUE; /* default */
					pPort->pChSys      = pChSys;
					
					pPort->CallCbfAfterSend = LSA_FALSE;
					
					/* 240708lrg001: No forwarding til ExtLinkInd with TopoOk=TRUE
					*/
					pPort->ClockTopoOk = LSA_FALSE;
					
					/* Init forwarding options */
					
					k = 1;
					
					for ( y=0; y<GSY_FWD_MAC_ADDR_CNT; y++) /* all macs/protocols */
					{
						pPort->FwdPortMode[y] = ( pPortInitHelp->FwdMode & k) ? GSY_FWD_MODE_ENABLE:GSY_FWD_MODE_DISABLE;
						k <<= 1;
					}
					
					pPortInitHelp++;
					pPort++;
				}
			}
		}
		else
		{
			Response = GSY_RSP_ERR_RESOURCE;
			//GSY_ERROR_TRACE_00(pChSys->TraceIdx, LSA_TRACE_LEVEL_ERROR, "*** gsy_FwdChannelInit() Portmanagement exceeds 64k!");
		}
		
		/* ----------------------------------------------------------------------*/
		/* Allocate Transmit buffers per Port                                    */
		/* ----------------------------------------------------------------------*/
		
		if ( Response == GSY_RSP_OK )
		{
			y = PortCnt;
			pPort = pChSys->Fwd.pPortMgm;
			
			while ( y && ( Response == GSY_RSP_OK ))
			{
				Response = gsy_AllocEthSend(pChSys,&pPort->TxRQBQueue, LSA_NULL, LSA_FALSE);
				
				if (GSY_RSP_OK == Response)
				{
					Response = gsy_AllocEthSend(pChSys,&pPort->AnnoRQBQueue, LSA_NULL, LSA_TRUE);
				}
				
				y--;
				pPort++;
			}
		}
		
		/* ----------------------------------------------------------------------*/
		/* Allocate FCBs for this channel                                        */
		/* ----------------------------------------------------------------------*/
		
		if ( Response == GSY_RSP_OK )
		{
			/* calculate total FCBs needed */
			/* we allocate one FCB per Port, Protocol and Master */
			FCBCnt = gsy_data.Fwd.ProtCnt * GSY_CFG_FWD_MAX_MASTERS_PER_PROTOCOL * PortCnt;
			
			pChSys->Fwd.FCBCnt  = FCBCnt;
			
			FCBSize = sizeof(GSY_FWD_FCB_TYPE) * FCBCnt;
			
			if ( FCBSize <= 0xFFFF )
			{
				GSY_ALLOC_LOCAL_MEM((GSY_LOCAL_MEM_PTR_TYPE*)&pFCB,(LSA_UINT16) FCBSize);
				
				if ( LSA_HOST_PTR_ARE_EQUAL(pFCB, LSA_NULL) )
				{
					Response = GSY_RSP_ERR_RESOURCE;
				}
				else
				{
					GSY_MEMSET_LOCAL(pFCB,0,FCBSize);
					
					pChSys->Fwd.pFCBAlloc = pFCB; /* Store for later release */
					
					while ( FCBCnt ) /* all FCBs */
					{
						pFCB->pChSys = pChSys;
						pFCB->Status = GSY_FWD_FCB_STS_FREE;
						
						GSY_PUT_BLOCK_TOP_USEQUEUE(pChSys->Fwd.FCBFreeQueue.pBottom,
																			 pChSys->Fwd.FCBFreeQueue.pTop,
																			 pFCB);
						pFCB++;
						FCBCnt--;
					}
				}
			}
			else
			{
				Response = GSY_RSP_ERR_RESOURCE;
			}
		}
		
		/* ----------------------------------------------------------------------*/
		/* Start opening multicast MACs within EDD                               */
		/* ----------------------------------------------------------------------*/
		
		if ( Response == GSY_RSP_OK )
		{
			pChSys->Fwd.MCSetIdx = 0;
			
			if ( gsy_FwdMcControl(pChSys,LSA_NULL,LSA_TRUE, &Response) )
			{
				if ( Response == GSY_RSP_OK )
				{
					pChSys->Fwd.Cbf    = Cbf;
					pChSys->Fwd.Status = GSY_FWD_CHANNEL_OPENING;
					Response = GSY_RSP_OK_ACTIVE;
				}
				else
				{
					/* MC-Enable failed */
					//ERROR
				}
			}
			else /* nothing to open (most likely a wrong config, but we continue)..*/
			{
				pChSys->Fwd.Status = GSY_FWD_CHANNEL_OPEN;
			}
		}
		
		if (( Response != GSY_RSP_OK ) &&
				( Response != GSY_RSP_OK_ACTIVE ))
		{
			//ERROR
		}
		else
		{
			i = 0;
			Ready = LSA_FALSE;
			
			/* find a free entry to store this pChSys
			*/
			while ((i<GSY_MAX_SYS_CHANNELS) && (! Ready))
			{
				if ( LSA_HOST_PTR_ARE_EQUAL(gsy_data.Fwd.EDDSysChannel[i], LSA_NULL) )
				{
					gsy_data.Fwd.EDDSysChannel[i] = pChSys;
					gsy_data.Fwd.EDDSysChannelCnt++;
					Ready = LSA_TRUE;
				}
				
				i++;
			}
		}
	}
	
	return(Response);
}

/*****************************************************************************/
/* Internal function: gsy_OpenLowerFwd()                                     */
/* Allocate Receive-RQBs for announce frames and initialize forwarding       */
/*****************************************************************************/
LSA_UINT16 GSY_LOCAL_FCT_ATTR gsy_OpenLowerFwd(
																							 GSY_CH_SYS_PTR  		    pChSys,
																							 GSY_LOWER_RQB_PTR_TYPE	pRbl)
{
	LSA_UINT16 	RbuRespVal;

	RbuRespVal = GSY_LOWER_RQB_RESPONSE_GET(pRbl);
	if (RbuRespVal != GSY_LOWER_RSP_OK)
	{
		//ERROR
	}
	else
	{
		/* Save lower handle in channel data
		*/
		pChSys->pChAnno->HandleLower = GSY_LOWER_RQB_HANDLE_LOWER_GET(pRbl);

		/* Store lower OpenChannel RQB for close channel
		*/
		pChSys->pChAnno->pOpenRbl = pRbl;
		RbuRespVal = GSY_RSP_OK;

		if (pChSys->RxTxNanos != 0)
		{
			/* Initialize channel forwarding 
			*/
			pChSys->pChAnno->State = GSY_CHA_STATE_OPEN_FWD;
			RbuRespVal = gsy_FwdChannelInit(pChSys, pChSys->PortCount, pChSys->PortFwd, gsy_OpenLowerAnnoEnd);
		}
	}

	return(RbuRespVal);
}

/*****************************************************************************/
/* Internal function: gsy_OpenLowerAnnoEnd()                                 */
/* If gsy_open_channel() has run til gsy_OpenLowerFwd(), it finishes here.   */
/*****************************************************************************/
LSA_VOID GSY_LOCAL_FCT_ATTR gsy_OpenLowerAnnoEnd(
																								 GSY_CH_SYS_PTR	pChSys,
																								 LSA_UINT16		  RbuRespVal)
{
	LSA_INT Idx;

	if (RbuRespVal == GSY_RSP_OK)
	{
		/* Finsh upper open channel without error
		*/
		gsy_CallbackAnno(pChSys, LSA_NULL, RbuRespVal);

		/* Send ExtLinkIndication RQBs for all ports on system channel
		*/
		for (Idx = 0; Idx < pChSys->PortCount; Idx++)
		{
			if (!LSA_HOST_PTR_ARE_EQUAL(pChSys->Port[Idx].pExtLinkRbl, LSA_NULL))
			{
				GSY_REQUEST_LOWER(pChSys->Port[Idx].pExtLinkRbl, pChSys->pSys);
				pChSys->Port[Idx].pExtLinkRbl = LSA_NULL;
			}
		}
	}
}

/*****************************************************************************/
/* Internal access function: gsy_CloseLower()                                */
/* Close Lower Channel.                                                      */
/*****************************************************************************/
LSA_VOID GSY_LOCAL_FCT_ATTR gsy_CloseLower(
																					 GSY_CH_SYS_PTR  		    pChSys,
																					 GSY_LOWER_RQB_PTR_TYPE	pRbl,
																					 LSA_BOOL				        ChAnno)
{
	/* Fill CLOSE request
	*/
	GSY_LOWER_RQB_HANDLE_SET(pRbl, 0); /* sanity */
	GSY_LOWER_RQB_OPCODE_SET(pRbl, GSY_LOWER_OPC_CLOSE_CHANNEL);
	GSY_LOWER_RQB_SERVICE_SET(pRbl, 0);
	GSY_LOWER_RQB_ERR_COMPID_SET(pRbl, LSA_COMP_ID_UNUSED);

	/* Send CLOSE request
	*/
	if (ChAnno)
	{
		GSY_LOWER_RQB_LOWER_HANDLE_SET(pRbl, pChSys->pChAnno->HandleLower);
		GSY_CLOSE_CHANNEL_LOWER(pRbl, pChSys->pChAnno->pSys);
	}
	else
	{
		GSY_LOWER_RQB_LOWER_HANDLE_SET(pRbl, pChSys->HandleLower);
		GSY_CLOSE_CHANNEL_LOWER(pRbl, pChSys->pSys);
	}
}

/*****************************************************************************/
/* Internal function: gsy_CloseLowerFin()                                    */
/* Lower Channel is closed: clean up and confirm user request.               */
/*****************************************************************************/
LSA_VOID GSY_LOCAL_FCT_ATTR gsy_CloseLowerFin(
																							GSY_CH_SYS_PTR  		    pChSys,
																							GSY_LOWER_RQB_PTR_TYPE	pRbl)
{
	LSA_UINT16 RetVal   = LSA_RET_OK;
	LSA_UINT16 RespVal  = GSY_RSP_OK;
	LSA_BOOL   ChRemove = LSA_TRUE;

	if (!LSA_HOST_PTR_ARE_EQUAL(pRbl, LSA_NULL))
	{
		if (GSY_RSP_OK != pChSys->Resp)
		{
			/* Error on gsy_open_channel()
			*/
			RespVal = pChSys->Resp;
		}
		else
		{
			RespVal = GSY_LOWER_RQB_RESPONSE_GET(pRbl);
			
			if (GSY_LOWER_RSP_OK != RespVal)
			{
				RespVal  = GSY_RSP_ERR_LOWER_LAYER;
				ChRemove = LSA_FALSE;
			}
		}
	}

	if (ChRemove)
	{
		/* Free lower RQB and clean up channel structure
		*/
		if (!LSA_HOST_PTR_ARE_EQUAL(pRbl, LSA_NULL))
		{
			gsy_FreeLowerRb(pChSys, pRbl, LSA_FALSE);
		}
		
		RetVal = gsy_ChSysClean(pChSys);
		
		if (LSA_RET_OK != RetVal)
		{
			pChSys->State = GSY_CHA_STATE_CLOSED;
			ChRemove = LSA_FALSE;
			pChSys->Resp = RespVal;
		}
	}
	else
	{
		/* Store lower RQB for close_channel() and reset channel state
		*/ 
		pChSys->pOpenRbl = pRbl;
	}

	/* If all resources are free now: confirm actual upper RQB to the user
	*/
	if (LSA_RET_OK == RetVal)
	{
		gsy_CallbackSys(pChSys, LSA_NULL, RespVal);
	}

	if (ChRemove)
	{
		/* Free channel structure
		*/
		gsy_ChSysFree(pChSys);
	}
}

/*****************************************************************************/
/* Internal function: gsy_CloseAnnoFin()                                     */
/* Lower Announce Channel is closed: clean up and confirm user request.      */
/*****************************************************************************/
LSA_VOID GSY_LOCAL_FCT_ATTR gsy_CloseAnnoFin(
GSY_CH_SYS_PTR  		pChSys,
GSY_LOWER_RQB_PTR_TYPE	pRbl)
{
	LSA_UINT16 RespVal = GSY_RSP_OK;
	LSA_BOOL   ChRemove = LSA_TRUE;

	if (!LSA_HOST_PTR_ARE_EQUAL(pRbl, LSA_NULL))
	{
		if (GSY_RSP_OK != pChSys->pChAnno->Resp)
		{
			/* Error at gsy_open_channel()
			*/
			RespVal = pChSys->pChAnno->Resp;
		}
		else
		{
			RespVal = GSY_LOWER_RQB_RESPONSE_GET(pRbl);
			
			if (GSY_LOWER_RSP_OK != RespVal)
			{
				RespVal = GSY_RSP_ERR_LOWER_LAYER;
				ChRemove = LSA_FALSE;
			}
		}
	}

	if (ChRemove)
	{
		/* Free lower RQB and channel ressources
		*/
		if (!LSA_HOST_PTR_ARE_EQUAL(pRbl, LSA_NULL))
		{
			gsy_FreeLowerRb(pChSys, pRbl, LSA_TRUE);
		}
		
		gsy_ChAnnoClean(pChSys->pChAnno);
	}
	else
	{
		/* Store lower RQB for close_channel()
		*/ 
		pChSys->pChAnno->pOpenRbl = pRbl;
	}

	/* Confirm actual upper RQB to user
	*/
	gsy_CallbackAnno(pChSys, LSA_NULL, RespVal);

	if (ChRemove)
	{
		/* Free channel structure
		*/
		gsy_ChAnnoFree(pChSys->pChAnno);
	}
}

/*****************************************************************************/
/* Internal access function: gsy_MucAddrSet()                                */
/* Enabled or disable a GSY Multicast address                                */
/*****************************************************************************/
LSA_UINT16 GSY_LOCAL_FCT_ATTR gsy_MucAddrSet(
																						 GSY_CH_SYS_PTR   		   pChSys,
																						 GSY_LOWER_RQB_PTR_TYPE  pRbl,
																						 LSA_UINT8       const * pMucAddr,
																						 LSA_BOOL				         Enable,
																						 LSA_BOOL				         ChAnno)
{
	LSA_UINT16 RetVal = LSA_RET_OK;

	/* Allocate lower request block if the parameter pointer is NULL
	*/
	if (LSA_HOST_PTR_ARE_EQUAL(pRbl, LSA_NULL))
		RetVal = gsy_AllocEthParam(pChSys, &pRbl, ChAnno);

	if (LSA_RET_OK == RetVal)
	{
		GSY_LOWER_RQB_HANDLE_SET(pRbl, 0); /* sanity */
		GSY_LOWER_RQB_OPCODE_SET(pRbl, GSY_LOWER_OPC_REQUEST);
		GSY_LOWER_RQB_SERVICE_SET(pRbl, GSY_LOWER_MULTICAST);
		GSY_LOWER_RQB_ERR_COMPID_SET(pRbl, LSA_COMP_ID_UNUSED);
		
		if (Enable)
		{
			GSY_LOWER_RQB_MULTICAST_ENABLE(pRbl, pMucAddr);
		}
		else
		{
			GSY_LOWER_RQB_MULTICAST_DISABLE(pRbl, pMucAddr);
		}

		/* Send Lower Request
		*/
		if (ChAnno)
		{
			GSY_LOWER_RQB_LOWER_HANDLE_SET(pRbl, pChSys->pChAnno->HandleLower);
			GSY_REQUEST_LOWER(pRbl, pChSys->pChAnno->pSys);
		}
		else
		{
			GSY_LOWER_RQB_LOWER_HANDLE_SET(pRbl, pChSys->HandleLower);
			GSY_REQUEST_LOWER(pRbl, pChSys->pSys);
		}
	}
	
	return(RetVal);
}

#ifndef GSY_CFG_SYNC_ONLY_IN_KRISC32
/*****************************************************************************/
/* Internal access function: gsy_LowerDelaySet()                             */
/* Send the LineDelay for one port to the lower layer.                       */
/*****************************************************************************/
LSA_VOID GSY_LOCAL_FCT_ATTR gsy_LowerDelaySet(GSY_CH_SYS_PTR pChSys, LSA_UINT16 PortId)
{
	GSY_LOWER_RQB_PTR_TYPE  pRbl;
	LSA_UINT32				LowerDelay;
	LSA_UINT16				Response = GSY_RSP_OK;

	/* Calculate delay ticks for lower Layer with the with
	 * the resolution from GET_PARAMS and round up
	*/
	LowerDelay = pChSys->Port[PortId-1].SyncLineDelay / pChSys->RxTxNanos;

	/* Use RQB of Lower Ctrl Queue
	*/
	pRbl = pChSys->QueueLowerCtrl.pFirst;
	
	if (LSA_HOST_PTR_ARE_EQUAL(pRbl, LSA_NULL))
		Response = GSY_RSP_ERR_RESOURCE;

	if ((GSY_RSP_OK == Response) && (LowerDelay != pChSys->Port[PortId-1].LowerDelayTicks))
	{
		GSY_IS_VALID_PTR(pRbl);

		/* If there is one RQB in queue and the lower Delay has changed
		*/
		pChSys->Port[PortId-1].LowerDelayTicks = LowerDelay;
		gsy_DequeLower(&pChSys->QueueLowerCtrl, pRbl);
		GSY_LOWER_RQB_OPCODE_SET(pRbl, GSY_LOWER_OPC_REQUEST);
		GSY_LOWER_RQB_HANDLE_SET(pRbl, 0); /* sanity */
		GSY_LOWER_RQB_LOWER_HANDLE_SET(pRbl, pChSys->HandleLower);
		GSY_LOWER_RQB_SERVICE_SET(pRbl, GSY_LOWER_DELAY_SET);
		GSY_LOWER_RQB_ERR_COMPID_SET(pRbl, LSA_COMP_ID_UNUSED);
		GSY_LOWER_RQB_SET_LINE_DELAY_PORT_SET(pRbl, PortId);
		GSY_LOWER_RQB_SET_LINE_DELAY_DELAY_SET(pRbl, LowerDelay);
		GSY_LOWER_RQB_SET_LINE_DELAY_CABLE_SET(pRbl, pChSys->Port[PortId-1].CableDelay);

		/* Send Lower Request
		*/
		GSY_REQUEST_LOWER(pRbl, pChSys->pSys);
	}
}
#endif //GSY_CFG_SYNC_ONLY_IN_KRISC32

/*****************************************************************************/
/* Internal function: gsy_AllocEthParam()      		                         */
/* Allocate lower Requstblock with Parameterblock                            */
/*****************************************************************************/
LSA_UINT16 GSY_LOCAL_FCT_ATTR gsy_AllocEthParam(
																								GSY_CH_SYS_PTR  		    pChSys,
																								GSY_LOWER_RQB_PTR_TYPE	*ppRbl,
																								LSA_BOOL				        ChAnno)
{
	GSY_LOWER_RQB_PTR_TYPE		pRbl;
	GSY_LOWER_MEM_PTR_TYPE		pMem;
	LSA_USER_ID_TYPE        	UserId;
	LSA_SYS_PTR_TYPE			pSys;
	LSA_UINT16 					RetVal = LSA_RET_OK;

	pMem = LSA_NULL;

	UserId.uvar32 = 0;
	if (ChAnno)
	{
		UserId.uvar8_array[GSY_HANDLE_UID] = pChSys->pChAnno->Handle;
		pSys = pChSys->pChAnno->pSys;
	}
	else
	{
		UserId.uvar8_array[GSY_HANDLE_UID] = pChSys->Handle;
		pSys = pChSys->pSys;
	}

	/* Request lower RQB
	*/
	GSY_ALLOC_LOWER_RQB(&pRbl, UserId, sizeof(GSY_LOWER_RQB_TYPE), pSys);

	/* If the lower RB has been delivered synchronously...
	*/
	if(!LSA_HOST_PTR_ARE_EQUAL(pRbl, LSA_NULL))
	{
		/* ...allocate memory for parameter block
		*/
		GSY_LOWER_RQB_USERID_UVAR32_SET(pRbl, UserId.uvar32);
		GSY_LOWER_RQB_PPARAM_SET(pRbl, LSA_NULL);
		GSY_ALLOC_LOWER_MEM(&pMem, UserId, sizeof(GSY_LOWER_PARAM_TYPE), pSys);

		/* If the lower PB has been delivered synchronously...
		*/
		if(!LSA_HOST_PTR_ARE_EQUAL(pMem, LSA_NULL))
		{
			/* ...set param pointer of the RB to the address of PB
			 * and return pointer to RB
			*/
			GSY_LOWER_RQB_PPARAM_SET(pRbl, pMem);
			*ppRbl = pRbl;
		}
		else
		{
			/* Next step: get RQB asynchronously by gsy_lower_mem()
			 * is currently not supported!
			*/
			GSY_FREE_LOWER_RQB(&RetVal, pRbl, pSys);
			RetVal = LSA_RET_ERR_RESOURCE;
		}
	}
	else
	{
		/* Next step: get RQB asynchronously by gsy_lower_rqb()
		 * is currently not supported!
		*/
		RetVal = LSA_RET_ERR_RESOURCE;
	}

	return(RetVal);
}

/*****************************************************************************/
/* Internal function: gsy_AllocEthSend()      		                         */
/* Allocate Lower Send-Requstblock with Parameterblock and Data block        */
/* and put it into the given queue.                                          */
/*****************************************************************************/
LSA_UINT16 GSY_LOCAL_FCT_ATTR gsy_AllocEthSend(
																							 GSY_CH_SYS_PTR  		pChSys,
																							 GSY_LOWER_QUEUE		*	pQueue,
																							 LSA_VOID *				pUser,
																							 LSA_BOOL				ChAnno)
{
	LSA_USER_ID_TYPE        UserId;
	LSA_SYS_PTR_TYPE			    pSys;
	GSY_LOWER_RQB_PTR_TYPE		 pRbl = LSA_NULL;
	GSY_LOWER_MEM_PTR_TYPE		 pMem = LSA_NULL;
	GSY_LOWER_TXMEM_PTR_TYPE pMemTx = LSA_NULL;
	LSA_UINT16 					     RetVal = LSA_RET_OK;
	LSA_UINT16 					 RbuRespVal = GSY_RSP_OK;

	UserId.uvar32 = 0;
	if (ChAnno)
	{
		UserId.uvar8_array[GSY_HANDLE_UID] = pChSys->pChAnno->Handle;
		pSys = pChSys->pChAnno->pSys;
	}
	else
	{
		UserId.uvar8_array[GSY_HANDLE_UID] = pChSys->Handle;
		pSys = pChSys->pSys;
	}

	/* Request lower RQB
	*/
	GSY_ALLOC_LOWER_RQB(&pRbl, UserId, sizeof(GSY_LOWER_RQB_TYPE), pSys);

	/* If the lower RB has been delivered synchronously...
	*/
	if(!LSA_HOST_PTR_ARE_EQUAL(pRbl, LSA_NULL))
	{
		/* ...put it into the channels queue and request lower parameter block
		*/
		GSY_LOWER_RQB_USERID_UVAR32_SET(pRbl, UserId.uvar32);
		GSY_LOWER_RQB_PPARAM_SET(pRbl, LSA_NULL);
		gsy_EnqueLower(pQueue, pRbl);
		GSY_ALLOC_LOWER_MEM(&pMem, UserId, sizeof(GSY_LOWER_PARAM_TYPE), pSys);

		/* If the lower PB has been delivered synchronously...
		*/
		if(!LSA_HOST_PTR_ARE_EQUAL(pMem, LSA_NULL))
		{
			/* ...set param pointer of the RB to the address of PB
			 * and request memory for send data	block
			*/
			GSY_LOWER_RQB_PPARAM_SET(pRbl, pMem);
			GSY_LOWER_RQB_SEND_DATA_SET(pRbl, LSA_NULL, 0);
			GSY_ALLOC_LOWER_TXMEM(&pMemTx, UserId, GSY_LOWER_MEM_MAX_LEN, pSys);

			/* If the lower DB has been delivered synchronously...
			*/
			if(!LSA_HOST_PTR_ARE_EQUAL(pMemTx, LSA_NULL))
			{
				/* ...set data pointer of the RB to the address of DB
				 * and set service code to SEND
				*/
				GSY_LOWER_RQB_SEND_DATA_SET(pRbl, pMemTx, GSY_LOWER_MEM_MAX_LEN);
				GSY_LOWER_RQB_SERVICE_SET(pRbl, GSY_LOWER_SEND);
				GSY_LOWER_RQB_USER_PTR_SET(pRbl, pUser);
			}
			else
			{
				/* Next step: get RQB asynchronously by gsy_lower_tx_mem()
				 * is currently not supported!
				*/
				gsy_DequeLower(pQueue, pRbl);
				pMem = GSY_LOWER_RQB_PPARAM_GET(pRbl);
				GSY_FREE_LOWER_MEM(&RetVal, pMem, pSys);
				GSY_FREE_LOWER_RQB(&RetVal, pRbl, pSys);
				RbuRespVal = GSY_RSP_ERR_RESOURCE;
			}
		}
		else
		{
			/* Next step: get RQB asynchronously by gsy_lower_mem()
			 * is currently not supported!
			*/
			gsy_DequeLower(pQueue, pRbl);
			GSY_FREE_LOWER_RQB(&RetVal, pRbl, pSys);
			RbuRespVal = GSY_RSP_ERR_RESOURCE;
		}
	}
	else
	{
		/* Next step: get RQB asynchronously by gsy_lower_rqb()
		 * is currently not supported!
		*/
		RbuRespVal = GSY_RSP_ERR_RESOURCE;
	}

	GSY_FUNCTION_TRACE_03(pChSys->TraceIdx, LSA_TRACE_LEVEL_CHAT,"  < gsy_AllocEthSend() pRbl=%X pMemTx=%X RetVal=0x%x", 
					pRbl, pMemTx, RbuRespVal);

	LSA_UNUSED_ARG(RetVal);     // Return value is not evaluated

	return(RbuRespVal);
}

/*****************************************************************************/
/* Internal function: gsy_AllocEthRecv()      		                         */
/* Allocate Lower Recv-Requstblock with Parameterblock and Data block        */
/* and put it into the given queue or send it to the lower layer.            */
/*****************************************************************************/
LSA_UINT16 GSY_LOCAL_FCT_ATTR gsy_AllocEthRecv(
																							 GSY_CH_SYS_PTR  		pChSys,
																							 GSY_LOWER_RQB_PTR_TYPE 	pRbr,
																							 GSY_LOWER_QUEUE		*	pQueue,
																							 LSA_HANDLE_TYPE			UserHandle,
																							 LSA_UINT8				TypeId,
																							 LSA_BOOL				ChAnno)
{
	LSA_USER_ID_TYPE        UserId;
	LSA_SYS_PTR_TYPE		      pSys;
	GSY_LOWER_RXMEM_PTR_TYPE pMemRx = LSA_NULL;
	GSY_LOWER_MEM_PTR_TYPE		 pMem = LSA_NULL;
	GSY_LOWER_RQB_PTR_TYPE		 pRbl = LSA_NULL;
	LSA_UINT16 					     RetVal = LSA_RET_OK;
	LSA_UINT16 					 RbuRespVal = GSY_RSP_OK;

	UserId.uvar32 = 0;
	UserId.uvar8_array[GSY_HANDLE_UID] = UserHandle;
	UserId.uvar8_array[GSY_TYPE_UID] = TypeId;
	if (ChAnno)
	{
		pSys = pChSys->pChAnno->pSys;
	}
	else
	{
		pSys = pChSys->pSys;
	}

	if(LSA_HOST_PTR_ARE_EQUAL(pRbr, LSA_NULL))
	{
		/* Allocate Lower Requestblock
		*/
		GSY_ALLOC_LOWER_RQB(&pRbl, UserId, sizeof(GSY_LOWER_RQB_TYPE), pSys);
	}
	else
	{
		pRbl = pRbr;
		pMem = GSY_LOWER_RQB_PPARAM_GET(pRbl);
	}

	/* If the lower RB has been delivered synchronously...
	*/
	if(!LSA_HOST_PTR_ARE_EQUAL(pRbl, LSA_NULL))
	{
		if(LSA_HOST_PTR_ARE_EQUAL(pMem, LSA_NULL))
		{
			/* Allocate Parameterblock and set pointer of the RQB
			*/
			GSY_LOWER_RQB_USERID_UVAR32_SET(pRbl, UserId.uvar32);
			GSY_ALLOC_LOWER_MEM(&pMem, UserId, sizeof(GSY_LOWER_PARAM_TYPE), pSys);
			GSY_LOWER_RQB_PPARAM_SET(pRbl, pMem);
		}

		if(!LSA_HOST_PTR_ARE_EQUAL(pMem, LSA_NULL))
		{
			/* Allocate Receive data block
			 * 101208lrg: receive Announce with GSY_LOWER_RECV
			*/
			if (GSY_SYNC_TYPE_ID == TypeId)
			{
				GSY_LOWER_RQB_RECV_SYNC_DATA_SET(pRbl, LSA_NULL, 0);
			}
			else
			{
				GSY_LOWER_RQB_RECV_DATA_SET(pRbl, LSA_NULL, 0);
			}
			
			GSY_ALLOC_LOWER_RXMEM(&pMemRx, UserId, GSY_LOWER_MEM_MAX_LEN, pSys);

			/* Wenn der lower DB synchron geliefert wurde...
			*/
			if(!LSA_HOST_PTR_ARE_EQUAL(pMemRx, LSA_NULL))
			{
				/* Fill RQB and set buffer pointers
				*/
				if (ChAnno)
				{
					GSY_LOWER_RQB_LOWER_HANDLE_SET(pRbl, pChSys->pChAnno->HandleLower);
				}
				else
				{
					GSY_LOWER_RQB_LOWER_HANDLE_SET(pRbl, pChSys->HandleLower);
				}
				GSY_LOWER_RQB_HANDLE_SET(pRbl, 0); /* sanity */
				GSY_LOWER_RQB_OPCODE_SET(pRbl, GSY_LOWER_OPC_REQUEST);
				GSY_LOWER_RQB_ERR_COMPID_SET(pRbl, LSA_COMP_ID_UNUSED);
				if (GSY_SYNC_TYPE_ID == TypeId)
				{
					GSY_LOWER_RQB_SERVICE_SET(pRbl, GSY_LOWER_RECV_SYNC);
					GSY_LOWER_RQB_RECV_SYNC_PTR_SET(pRbl, pMemRx);
					GSY_LOWER_RQB_RECV_SYNC_LEN_SET(pRbl, GSY_LOWER_MEM_MAX_LEN);
					GSY_LOWER_RQB_RECV_SYNC_ID_SET(pRbl, UserId.uvar32);
				}
				else
				{
					GSY_LOWER_RQB_SERVICE_SET(pRbl, GSY_LOWER_RECV);
					GSY_LOWER_RQB_RECV_PTR_SET(pRbl, pMemRx);
					GSY_LOWER_RQB_RECV_LEN_SET(pRbl, GSY_LOWER_MEM_MAX_LEN);
					GSY_LOWER_RQB_RECV_ID_SET(pRbl, UserId.uvar32);
				}

				if (!LSA_HOST_PTR_ARE_EQUAL(pQueue, LSA_NULL))
				{
					/* Put it into given queue
					*/
					gsy_EnqueLower(pQueue, pRbl);
				}
				else
				{
					/* Send Lower Receive Request
					*/
					GSY_REQUEST_LOWER(pRbl, pSys);
				}
			}
			else
			{
#ifdef GSY_MESSAGE
				/* Next step: get RQB asynchronously by gsy_lower_rx_mem()
				 * @note: Do not free the already allocated memory
				 * is currently not supported!
				*/
#endif /* GSY_MESSAGE */
				if (LSA_HOST_PTR_ARE_EQUAL(pRbr, LSA_NULL))
				{
					pMem = GSY_LOWER_RQB_PPARAM_GET(pRbl);
					GSY_FREE_LOWER_MEM(&RetVal, pMem, pSys);
					GSY_FREE_LOWER_RQB(&RetVal, pRbl, pSys);
				}
				
				RbuRespVal = GSY_RSP_ERR_RESOURCE;
			}
		}
		else
		{
			if(LSA_HOST_PTR_ARE_EQUAL(pRbr, LSA_NULL))
			{
				GSY_FREE_LOWER_RQB(&RetVal, pRbl, pSys);
			}
			
			RbuRespVal = GSY_RSP_ERR_RESOURCE;
		}
	}
	else
	{
		/* Next step: get RQB asynchronously by gsy_lower_rqb()
		 * is currently not supported!
		*/
		RbuRespVal = GSY_RSP_ERR_RESOURCE;
	}

	// Return value is not evaluated
	LSA_UNUSED_ARG(RetVal);

	return(RbuRespVal);
}

/*****************************************************************************/
/* Internal access function: gsy_FreeLowerRb()                               */
/* Free Requestblock, Parameterblock and Data buffer.                        */
/*****************************************************************************/
LSA_VOID GSY_LOCAL_FCT_ATTR gsy_FreeLowerRb(
																						GSY_CH_SYS_PTR  		    pChSys,
																						GSY_LOWER_RQB_PTR_TYPE  pRbl,
																						LSA_BOOL				        ChAnno)
{
	LSA_SYS_PTR_TYPE			    pSys;
	GSY_LOWER_MEM_PTR_TYPE		pMem;
	GSY_LOWER_TXMEM_PTR_TYPE	pMemTx = LSA_NULL;
	GSY_LOWER_RXMEM_PTR_TYPE	pMemRx = LSA_NULL;
	LSA_UINT16 					      RetVal = LSA_RET_OK;

	if (ChAnno)
	{
		pSys = pChSys->pChAnno->pSys;
	}
	else
	{
		pSys = pChSys->pSys;
	}

	pMem = GSY_LOWER_RQB_PPARAM_GET(pRbl);
	
	if (!LSA_HOST_PTR_ARE_EQUAL(pMem, LSA_NULL))
	{
		if ((GSY_LOWER_RECV      == GSY_LOWER_RQB_SERVICE_GET(pRbl))
		||  (GSY_LOWER_RECV_SYNC == GSY_LOWER_RQB_SERVICE_GET(pRbl)))
		{
			/* Free Receive Memory
			*/
			if (GSY_LOWER_RECV == GSY_LOWER_RQB_SERVICE_GET(pRbl))
			{
				pMemRx = GSY_LOWER_RQB_RECV_PTR_GET(pRbl);
			}
			else
			{
				pMemRx = GSY_LOWER_RQB_RECV_SYNC_PTR_GET(pRbl);
			}
			
			if (!LSA_HOST_PTR_ARE_EQUAL(pMemRx, LSA_NULL))
			{
				GSY_FREE_LOWER_RXMEM(&RetVal, pMemRx, pSys);
			}
		}
		else if ((GSY_LOWER_SEND      == GSY_LOWER_RQB_SERVICE_GET(pRbl))
			 ||    (GSY_LOWER_SEND_TS   == GSY_LOWER_RQB_SERVICE_GET(pRbl))
			 ||    (GSY_LOWER_SEND_SYNC == GSY_LOWER_RQB_SERVICE_GET(pRbl)))
		{
			/* Free Transmit Memory
			*/
			if (GSY_LOWER_SEND_SYNC == GSY_LOWER_RQB_SERVICE_GET(pRbl))
			{
				pMemTx = GSY_LOWER_RQB_SEND_SYNC_PTR_GET(pRbl);
			}
			else
			{
				pMemTx = GSY_LOWER_RQB_SEND_PTR_GET(pRbl);
			}
			if (!LSA_HOST_PTR_ARE_EQUAL(pMemTx, LSA_NULL))
			{
				GSY_FREE_LOWER_TXMEM(&RetVal, pMemTx, pSys);
			}
		}
		/* Free Parameter Memory
		*/
		GSY_FREE_LOWER_MEM(&RetVal, pMem, pSys);
	}

	GSY_FREE_LOWER_RQB(&RetVal, pRbl, pSys);
}

/*****************************************************************************/
/* External LSA access function: gsy_lower_rqb()                             */
/* Get Lower Requestblock asynchronously.                                    */
/*****************************************************************************/
LSA_VOID  GSY_SYSTEM_IN_FCT_ATTR  gsy_lower_rqb(
																								LSA_USER_ID_TYPE         UserId,
																								LSA_UINT16               Len,
																								GSY_LOWER_RQB_PTR_TYPE   pRbl)
{
	LSA_UINT16 RetVal     = LSA_RET_OK;
	GSY_CH_SYS_PTR pChSys = gsy_ChSysGet(UserId.uvar8_array[GSY_HANDLE_UID]);

	/* Currently only synchronous allocation is supported!
	*/
	if(!LSA_HOST_PTR_ARE_EQUAL(pChSys, LSA_NULL))
		GSY_FREE_LOWER_RQB(&RetVal, pRbl, pChSys->pSys);

	gsy_ErrorLower(GSY_MODULE_ID, __LINE__, GSY_ERR_ASYNC_PTR, pChSys, Len);

	// Return value is not evaluated
	LSA_UNUSED_ARG(RetVal);
}

/*****************************************************************************/
/* External LSA access function: gsy_lower_mem()                             */
/* Get lower datablock asynchronously.                                       */
/*****************************************************************************/
LSA_VOID  GSY_SYSTEM_IN_FCT_ATTR  gsy_lower_mem(
																								LSA_USER_ID_TYPE         UserId,
																								LSA_UINT16               Len,
																								GSY_LOWER_MEM_PTR_TYPE   DataPtr)
{
	LSA_UINT16 RetVal = LSA_RET_OK;
	GSY_CH_SYS_PTR pChSys = gsy_ChSysGet(UserId.uvar8_array[GSY_HANDLE_UID]);
	
	/* Currently only synchronous allocation is supported!
	*/
	if(!LSA_HOST_PTR_ARE_EQUAL(pChSys, LSA_NULL))
	{
		GSY_FREE_LOWER_MEM(&RetVal, DataPtr, pChSys->pSys);
	}
	
	gsy_ErrorLower(GSY_MODULE_ID, __LINE__, GSY_ERR_ASYNC_PTR, pChSys, Len);
	
	// Return value is not evaluated
	LSA_UNUSED_ARG(RetVal);
}

/*****************************************************************************/
/* External LSA access function: gsy_lower_tx_mem()                          */
/* Get lower send datablock asynchronously.                                  */
/*****************************************************************************/
LSA_VOID  GSY_SYSTEM_IN_FCT_ATTR  gsy_lower_tx_mem(
																									 LSA_USER_ID_TYPE         UserId,
																									 LSA_UINT16               Len,
																									 GSY_LOWER_TXMEM_PTR_TYPE DataPtr)
{
	LSA_UINT16 RetVal = LSA_RET_OK;
	GSY_CH_SYS_PTR pChSys = gsy_ChSysGet(UserId.uvar8_array[GSY_HANDLE_UID]);
	
	/* Currently only synchronous allocation is supported!
	*/
	if(!LSA_HOST_PTR_ARE_EQUAL(pChSys, LSA_NULL))
	{
		GSY_FREE_LOWER_TXMEM(&RetVal, DataPtr, pChSys->pSys);
	}
}

/*****************************************************************************/
/* External LSA access function: gsy_lower_rx_mem()                          */
/* Get lower receive datablock asynchronously.                               */
/*****************************************************************************/
LSA_VOID  GSY_SYSTEM_IN_FCT_ATTR  gsy_lower_rx_mem(
																									 LSA_USER_ID_TYPE         UserId,
																									 LSA_UINT16               Len,
																									 GSY_LOWER_RXMEM_PTR_TYPE DataPtr)
{
	LSA_UINT16 RetVal     = LSA_RET_OK;
	GSY_CH_SYS_PTR pChSys = gsy_ChSysGet(UserId.uvar8_array[GSY_HANDLE_UID]);
	
	/* Currently only synchronous allocation is supported!
	*/
	if(!LSA_HOST_PTR_ARE_EQUAL(pChSys, LSA_NULL))
	{
		GSY_FREE_LOWER_RXMEM(&RetVal, DataPtr, pChSys->pSys);
	}
}

/*-----------------------------------------------------------------------------
* EOF gsy_low.c
*/
