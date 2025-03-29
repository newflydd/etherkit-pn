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
/*  C o m p o n e n t     &C: DCP (Discovery & Configuration Protocol)  :C&  */
/*                                                                           */
/*  F i l e               &F: dcp_usr.c                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  User-Interface.                                                          */
/*                                                                           */
/*****************************************************************************/
#ifdef DCP_MESSAGE /**********************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Version                   Who  What                          */
/*  2002-07-17  P00.00.00.00_00.01.00.00  lrg  file creation                 */
/*  2002-10-31  P01.00.00.00_00.01.01.01  lrg  1.Increment:Identify Response */
/*  2003-02-03  P01.00.00.00_00.03.00.00  lrg  3.Increment:                  */
/*              Call dcp_ChClean(), RbuPtr->ParaPtr->x -> RbuPtr->Params.x   */
/*              New: Set xxTimer.InUse when allocated                        */
/*              Call dcp_ServerReportSet() with RbuPtr                       */
/*  2003-03-20  P01.00.00.00_00.03.00.01  lrg  RELEASE_PATH_INFO() vor Upper */
/*              Callback aber nach freigeben der lower RBs                   */
/*  2003-04-17  P01.00.00.00_00.04.00.00  lrg  4.Increment: CD7 -> DCP       */
/*  2003-05-19  P01.00.00.00_00.05.00.00  lrg  5.Increment: Identify Timer   */
/*  2003-07-17  P01.00.00.00_00.07.01.00  lrg  7.Increment: Client           */
/*              LSA-Header 2.1.0.0_0.2.1.1 eingebaut                         */
/*  2003-07-30  P01.00.00.00_00.08.00.00  lrg  8.Increment: Client channel   */
/*  2003-08-25  P01.00.00.00_00.09.00.00  lrg  9.Increment: Fehlerbehebung:  */
/*              dcp_ClientSet() ChPtr->Identify.Timer.Time = ... entfernt,   */
/*              wird schon in dcp_ClientOpen() gemacht.                      */
/*  2003-08-29  P01.00.00.00_00.09.00.01  Compilerwarnings wg. RestartPtr    */
/*              und Initialisierung TimerId entfernt.                        */
/*  2003-09-29  P01.00.00.00_00.11.00.00  lrg 11.Increment: Mehrere IDENTIFY */
/*              gleichzeitig mit Timerindex im Filter "Sequence Number".     */
/*  2003-10-21  P01.00.00.00_00.11.00.01  lrg  Fehlerbehebung                */
/*              dcp_ClientConf() TimerPtr->Running = LSA_TRUE setzen         */
/*  2003-10-23  P02.00.00.00_00.01.00.00  lrg  1.Increment Version 2: 		 */
/*              Neue DCP-Header mit Xid und Length						     */
/*  2003-10-28  P02.00.00.00_00.01.01.00  lrg  Fehlerbehebung 281003lrg001   */
/*              "Multi-Identify" geht nur bei synchronem Callback!           */
/*  2003-12-08  P02.00.00.00_00.01.02.00  lrg  Neue TLV-Datensatzstruktur    */
/*  2003-12-15  P02.00.00.00_00.01.03.00  dsc  New version of timeout hand-  */
/*              ling introduced: DCP_DO_TIMEOUT_EXTERN                       */
/*  2003-12-23  P02.00.00.00_00.01.04.00  lrg  Kleinbuchstaben senden        */
/*              bei Identify NameOfStation-Filter.                           */
/*  2004-01-27  P02.00.00.00_00.02.00.00  lrg  dcp_ClientInd()               */
/*              Lower Recv-RQB bei IDENTIFY wieder in den lower Layer        */
/*  2004-02-09  P02.00.00.00_00.02.00.01  lrg  dcp_Client...()               */
/*              NEU: Native RQB-Interface mit Retry und Request-Queuing      */
/*  2004-02-13  P02.00.00.00_00.02.00.02  lrg sizeof(dataset)->dataset_SIZE  */
/*  2004-02-23  P02.00.00.00_00.02.00.03  lrg  dcp_CheckRsp()                */
/*              Frame-ID ueberpr�fen und DataOffset beruecksichtigen         */
/*  2004-03-02  P02.00.00.00_00.02.00.04  lrg  Transaktions-ID bei TxRetry   */
/*              nicht erhoehen. Xid des Request in Txs merken und bei der    */
/*              Indication ueberpr�fen.                                      */
/*  2004-03-03  P02.00.00.00_00.02.00.05  lrg  #ifdef DCP_REQ_LOCAL_DATA     */
/*              Request -> dcp_Request alternativ im Datenseg. statt Stack   */
/*  2004-03-05  P02.00.00.00_00.02.02.02  lrg  dcp_ClientInd()               */
/*              Lower RQB bei invalid Resp. immer wieder in den lower Layer  */
/*  2004-03-16  P02.00.00.00_00.02.02.03  lrg  Benutzung der QueueCancel aus */
/*              dcp_ClientInd() und dcp_ClientCancel() entfernt, da EDD nun  */
/*              RQB-selektiven CANCEL unterst�tzt.                           */
/*  2004-03-26  P02.00.00.00_00.02.02.04  lrg 050326lrg001                   */
/*              dcp_CheckRsp() Den OFFSET der DCP-PDU wg. VlanTag beachten   */
/*  2004-03-29  P02.00.00.00_00.02.02.05  lrg dcp_ReqHdrSet() Set Xid and    */
/*              Length: pdu.rsp->pdu.req, DCP_SWAPW() -> DCP_SWAP16()        */
/*  2004-04-16  P02.00.00.00_00.02.05.03  lrg 020404lrg001                   */
/*              Cancel client receives always at timeout                     */
/*  2004-04-20  P02.00.00.00_00.02.05.04  lrg dcp_CheckRsp(): Pruefung des   */
/*              Empfangsframes auf max. L�nge vor Kopie in lokale Struktur   */
/*  2004-04-22  P02.00.00.00_00.02.05.05  lrg 220404lrg001: Gecancelten RQB  */
/*              nur in die Client Receive Queue h�ngen, wenn diese noch      */
/*              nicht "voll" ist.                                            */
/*  2004-04-20  P02.00.00.00_00.03.00.00  lrg  In dcp_request() nach         */
/*              dcp_ServerRequest() verzweigen                               */
/*  2004-06-24  P03.00.00.00_00.01.00.00  lrg  Version 3: dcp_ChClose()      */
/*              Beim Serverkanal cancelen                                    */
/*  2004-12-17  P03.02.00.00_00.02.02.00  lrg  171204lrg001: New parameter   */
/*              RequestCount for dcp_ClientInd(). Lower Recv-RQB wieder in   */
/*              den lower Layer, wenn RequestCount < als der Fuellstand der  */
/*              Server-IndicationQueue.                                      */
/*  2004-12-22  P03.03.00.00_00.02.03.00  lrg  Copyright-Text                */
/*  2005-01-13  P03.03.00.00_00.03.02.00  lrg  dcp_ClientIdentify()          */
/*              LSA_INT RecvCnt -> LSA_UINT16 RecvCnt                        */
/*              dcp_ClientInd(): Client Recv-RQB-Zaehler dekrementieren      */
/*  2005-02-16  P03.04.00.00_00.04.00.00  lrg  LSA-Trace eingebaut           */
/*  2005-03-01  P03.04.00.00_00.04.00.01  lrg  dcp_ClientOpen()/RespVal,     */
/*              dcp_CheckSet()/DataLen, dcp_CheckRsp()/RetVal,RecvAgain entf.*/
/*  2005-04-15  P03.05.00.00_00.04.00.02  150405lrg001: Timer-RQB immer an   */
/*              dcp_TimerRequest() weitergeben.                              */
/*  2006-03-21  P03.10.00.00_00.01.01.02  210306lrg001: Bei Get/Set Response */
/*              vor der Sendeconfirmation verkraften                         */
/*  2006-06-30  P03.12.00.00_00.01.01.02  lrg  LSA_GLOB 2.1.0.0_0.7.2.1      */
/*  2006-07-28  P03.12.00.00_00.25.02.02  280706lrg001: Lower Memoryblock    */
/*              fuer EDD-Identify-Filter in dcp_ChOpen anfordern             */
/*  2007-03-07  P04.00.00.00_00.01.01.02  070307lrg001: NotifyId             */
/*              Neuer Dienst: HELLO                                          */
/*  2007-04-18  P03.12.00.00_00.40.02.02  180407lrg001: Vor DCP_RSP_ERR_     */
/*              RESOURCE: Pruefen ob genug Receive-RQBs im lower Layer sind  */
/*  2007-05-11  P04.00.00.00_00.10.01.02  110507lrg001: ArtsRQ AP00460836    */
/*  2007-05-30  P04.00.00.00_00.10.02.02  300507lrg001: Hello-Delay          */
/*  2007-06-19  P04.00.00.00_00.20.01.02  190607lrg001: Tolerance            */
/*  2007-11-14  P04.00.00.00_00.50.01.02 141107lrg002: \ am Zeilenende entf. */
/*  2007-12-05  P04.00.00.00_00.60.01.02 051207lrg001: ArtsRQ AP00552010     */
/*              Beim CloseChannel auf Sendeconfirmations warten.             */
/*  2008-06-19  P04.00.00.00_00.80.02.02 190608lrg001: ArtsRQ AP00655477     */
/*  2008-06-20  P04.00.00.00_00.80.02.03 lrg: DCP_MESSAGE                    */
/*  2008-12-04  P04.01.00.00_00.02.01.01 lrg: Package Version (PNIO)         */
/*  2010-08-24  P05.00.00.00_00.01.01.02 240810lrg001: AP00999022 UPPER_DONE */
/*  2011-01-18  P05.00.00.00_00.03.12.02 lrg: AP01017495 Index-Trace         */
/*  2011-06-29  S05.01.00.00_00.03.03.02 lrg: AP01138397 dcp_TimerTab...     */
/*  2011-08-04  T05.00.00.00_00.05.44.02 040811lrg: cast                     */
/*  2011-08-25  S05.01.00.00_00.04.03.02 AP00696784: Comments in english     */
/*  2012-01-11  T05.01.00.00_00.05.13.01 AP01278788: Identify client timeout */
/*              AP01297962: remove DCP_CLIENT_MODE and DCP_SERVER_MODE       */
/*  2012-02-09  T05.01.00.00_00.05.25.01 AP01313139: dcp_Request added       */
/*  2013-03-04  P05.02.00.00_00.04.17.02 AP01503994: free upper 2 bit of XID */
/*              DCP_MAX_CHANNELS = 63, lower RQB Makros dcp_cfg.h->dcp_low.h */
/*  2013-06-17  P05.03.00.00_00.01.10.01 AP01387412 DCP Timer Optimierung    */
/*  2013-07-12  P05.03.00.00_00.01.10.01 AP01561748:DCP: Verhalten bzgl.     */
/*              TransactionEnd an Norm anpassen                              */
/*  2013-07-18  P05.03.00.00_00.01.10.01 AP01567955 DCP: Warnings bei        */
/*              Generierung des PnDriver entfernen                           */
/*  2013-09-11  P05.03.00.00_00.02.13.01 Request 740218 - Es werden im       */
/*              AllocOnRequest-Mode pro Client-Identify zu viele Recv-RQBs   */
/*              allokiert                                                    */
/*  2013-10-07  P06.00.00.00_00.01.14.01 214376 [DCP] WP07:                  */
/*              Fast Shutdown needs a fast DCP-request-cancel                */
/*  2014-04-10  P05.03.00.00_00.03.03.01 1006228: Fatal in dcp_IdentifyInd...*/
/*              Check Timer CallId in dcp_ClientInd()                        */
/*  2014-06-18  P05.03.00.00_00.03.09.01 1061321: Hellotimer nach Delay mit  */
/*              Hellointerval starten                                        */
/*  2014-08-03  P05.03.00.00_00.03.15.01 1094328 DCP missing header check in */
/*              dcp_CheckRsp(): RspLen vor Addition der Headerl�nge pr�fen   */
/*  2016-01-13  LINT-Messages fixed                                          */
/*  2016-02-08  P06.00 1533871: cancel if Txs.Response is DCP_RSP_OK_CANCEL  */
/*  2016-06-10  P05.04 - P07.00 1737982: Use same Timer with old Xid         */
/*                                       for SET-/GET retry                  */
/*                                                                           */
/*****************************************************************************/
#endif /* DCP_MESSAGE */

/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/
#define LTRC_ACT_MODUL_ID	2		/* DCP_MODULE_ID_USR */
#define DCP_MODULE_ID 		LTRC_ACT_MODUL_ID

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/
#include "dcp_inc.h"
#include "dcp_int.h"
DCP_FILE_SYSTEM_EXTENSION(DCP_MODULE_ID)

/*****************************************************************************/
/* Local data                                                                */
/* ...                                                                       */
/*****************************************************************************/
#ifdef DCP_REQ_LOCAL_DATA
/* Structures for a copy of a DCP Response and building of a Client Request:
 * --> else located on stack
*/
DCP_ETH_FRAME		dcp_ClientFrame;
DCP_REQ_DATA		dcp_Request;
#endif

/*****************************************************************************/
/* Internal access function: dcp_RequestDone()                               */
/* Confirmation/indication of an upper DCP Request                           */
/*****************************************************************************/
LSA_VOID  DCP_LOCAL_FCT_ATTR  dcp_RequestDone(
DCP_CHA_PTR				ChPtr,
DCP_UPPER_RQB_PTR_TYPE  RbuPtr,
LSA_UINT16				RspVal)
{
	DCP_UPPER_CALLBACK_FCT_PTR_TYPE	CbfPtr;
	LSA_SYS_PTR_TYPE	SysPtr;
	LSA_HANDLE_TYPE		DcpHandle = 0;
	LSA_UINT32			TraceIdx = 0;

	CbfPtr = 0;

	if (!LSA_HOST_PTR_ARE_EQUAL(ChPtr, LSA_NULL))
	{
		DcpHandle = ChPtr->Handle;
		TraceIdx = ChPtr->TraceIdx;
	}

	DCP_UPPER_TRACE_03(TraceIdx, LSA_TRACE_LEVEL_NOTE, ">>> dcp_RequestDone() RbuPtr=%X ChPtr=%X DCP-Handle=%d",
			RbuPtr, ChPtr, DcpHandle);

	if(!LSA_HOST_PTR_ARE_EQUAL(ChPtr, LSA_NULL))
	{
		if (LSA_HOST_PTR_ARE_EQUAL(RbuPtr, LSA_NULL))
		{
			/* Get first Requst block from Upper Queue
			*/
			RbuPtr = ChPtr->QueueUpper.FirstPtr;
		}
		if (LSA_HOST_PTR_ARE_EQUAL(RbuPtr, LSA_NULL))
		{
			/* No Request block to confirm ?!?
			*/
			dcp_ErrorInternal(DCP_MODULE_ID, __LINE__, DCP_ERR_NO_RBU, LSA_NULL, 0);
		}
		else
		{
			/* Remove RQB from Queue and set handle
			*/
			dcp_DequeUpper(RbuPtr);
			DCP_UPPER_RQB_HANDLE_SET(RbuPtr, ChPtr->HandleUpper);
			if (DCP_UPPER_RQB_OPCODE_GET(RbuPtr) == DCP_OPC_OPEN_CHANNEL)
			{
				/* OPEN CHANNEL: return own Handle and set
				 *               Channel State to OPEN if Response is OK
				*/
				DCP_UPPER_RQB_HANDLE_LOWER_SET(RbuPtr, ChPtr->Handle);
				if (RspVal == DCP_RSP_OK)
					ChPtr->State = DCP_CHA_STATE_OPEN;

			}

			/* Use Upper callback function from channel data
			*/
			CbfPtr = ChPtr->CbfPtr;
			SysPtr = ChPtr->SysPtr;

			/* Set Response code and pass RQB to callback function of upper Layer
			*/

			DCP_UPPER_RQB_RESPONSE_SET(RbuPtr, RspVal);
			DCP_UPPER_RQB_ERR_COMPID_SET(RbuPtr, LSA_COMP_ID_UNUSED);
			DCP_UPPER_TRACE_04(TraceIdx, LSA_TRACE_LEVEL_NOTE, "< < dcp_RequestDone(Opcode=%d) RbuPtr=%X Response=0x%x HandleUpper=%d",
					DCP_UPPER_RQB_OPCODE_GET(RbuPtr), RbuPtr, DCP_UPPER_RQB_RESPONSE_GET(RbuPtr), DCP_UPPER_RQB_HANDLE_GET(RbuPtr));
			/* 190608lrg001:
			*/
			if (DCP_RSP_OK_ACTIVE == RspVal)
			{
				dcp_ErrorInternal(DCP_MODULE_ID, __LINE__, DCP_ERR_FATAL, LSA_NULL, 0);
			}
			/* 240810lrg001: use upper callback macro
			*/
			DCP_REQUEST_UPPER_DONE(CbfPtr, RbuPtr, SysPtr);
//			CbfPtr(RbuPtr);
		}
	}
	else
	{
		dcp_ErrorInternal(DCP_MODULE_ID, __LINE__, DCP_ERR_CH_PTR, LSA_NULL, 0);
	}

	DCP_UPPER_TRACE_02(TraceIdx, LSA_TRACE_LEVEL_NOTE, "<<< dcp_RequestDone() RbuPtr=%X CbfPtr=%X",
			RbuPtr, CbfPtr);
}

/*****************************************************************************/
/* Internal function: dcp_CallbackRbu()                                      */
/* Confirmation of an upper OPEN CHANNEL                                     */
/*****************************************************************************/
LSA_VOID  DCP_LOCAL_FCT_ATTR  dcp_CallbackRbu(
LSA_SYS_PTR_TYPE		SysPtr,
DCP_UPPER_RQB_PTR_TYPE	RbuPtr,
LSA_UINT16				RespVal)
{
	DCP_UPPER_CALLBACK_FCT_PTR_TYPE	CbfPtr;

	DCP_UPPER_TRACE_02(0, LSA_TRACE_LEVEL_NOTE,">>> dcp_CallbackRbu() RbuPtr=%X RespVal=0x%x",
			RbuPtr, RespVal);

	CbfPtr = 0;

	if (LSA_HOST_PTR_ARE_EQUAL(RbuPtr, LSA_NULL))
	{
		/* No request block for callback ?!?
		*/
		dcp_ErrorInternal(DCP_MODULE_ID, __LINE__, DCP_ERR_NO_RBU, LSA_NULL, 0);
	}
	else
	{
		if (DCP_UPPER_RQB_OPCODE_GET(RbuPtr) == DCP_OPC_OPEN_CHANNEL)
		{
			/* Get Upper callback function and upper Handle from RQB
			*/
			DCP_UPPER_RQB_HANDLE_SET(RbuPtr, DCP_UPPER_RQB_HANDLE_UPPER_GET(RbuPtr));
			CbfPtr = DCP_UPPER_RQB_CBF_GET(RbuPtr);
		}
		else
		{
			dcp_ErrorUser(DCP_MODULE_ID, __LINE__, DCP_ERR_BAD_HANDLE, RbuPtr, sizeof(DCP_RQB_TYPE));
		}
	}

	if ((!(0 == CbfPtr))
	&&	(!LSA_HOST_PTR_ARE_EQUAL(RbuPtr, LSA_NULL)))    //lint !e774 Boolean within 'RbuPtr!=0' always evaluates to True, because dcp_ErrorInternal() is an exit, HM 13.01.2016
	{
		/* Set Response code and pass RQB to callback function of upper Layer
		*/
		DCP_UPPER_RQB_RESPONSE_SET(RbuPtr, RespVal);
		DCP_UPPER_RQB_ERR_COMPID_SET(RbuPtr, LSA_COMP_ID_UNUSED);

		DCP_UPPER_TRACE_04(0, LSA_TRACE_LEVEL_NOTE,"< < dcp_CallbackRbu(Opcode=%d) RbuPtr=%X Response=0x%x HandleUpper=%d",
				DCP_UPPER_RQB_OPCODE_GET(RbuPtr), RbuPtr, DCP_UPPER_RQB_RESPONSE_GET(RbuPtr), DCP_UPPER_RQB_HANDLE_GET(RbuPtr));

		DCP_REQUEST_UPPER_DONE(CbfPtr, RbuPtr, SysPtr);
	}

	DCP_UPPER_TRACE_02(0, LSA_TRACE_LEVEL_NOTE,"<<< dcp_CallbackRbu() RbuPtr=%X CbfPtr=%X",
			RbuPtr, CbfPtr);
}

/*****************************************************************************/
/* Internal function: dcp_ChOpen()                                           */
/* Open channel                                                              */
/*****************************************************************************/
static LSA_UINT16  DCP_LOCAL_FCT_ATTR  dcp_ChOpen(
DCP_UPPER_RQB_PTR_TYPE  RbuPtr)
{
	DCP_DETAIL_PTR_TYPE		DetailPtr;
	LSA_SYS_PTR_TYPE		SysPtr;
	DCP_LOWER_RQB_PTR_TYPE	RblPu;
	DCP_CHA_PTR  			ChPtr = LSA_NULL;
	LSA_SYS_PATH_TYPE		SysPath = DCP_UPPER_RQB_SYSPATH_GET(RbuPtr);
	LSA_INT					Idx = 0;
	LSA_UINT16   	RespVal = DCP_RSP_ERR_RESOURCE;
	LSA_UINT16   	RetVal = LSA_RET_OK;

	DCP_FUNCTION_TRACE_01(0, LSA_TRACE_LEVEL_NOTE, "  > dcp_ChOpen() RbuPtr=%X",
			RbuPtr);

	/* Get path info
	*/
	DCP_GET_PATH_INFO(&RetVal, &SysPtr, &DetailPtr, SysPath);
	if (RetVal == LSA_RET_OK)
	{
        /* Check CDB params */
        DCP_IS_VALID_PTR(DetailPtr);
        if (   (   (DetailPtr->ClientCh != DCP_CDB_SERVER_CHANNEL)
                && (DetailPtr->ClientCh != DCP_CDB_CLIENT_CHANNEL) )
            || (   (DetailPtr->AllocOnRequest != DCP_CDB_ALLOC_LOWER_RQBS_AT_OPEN_CHANNEL)
                && (DetailPtr->AllocOnRequest != DCP_CDB_ALLOC_LOWER_RQBS_AT_REQUEST) )
            || (   (DetailPtr->HelloEnable != DCP_CDB_HELLO_DISABLE)
                && (DetailPtr->HelloEnable != DCP_CDB_HELLO_ENABLE) )
            )
        {
            DCP_ERROR_TRACE_03(0, LSA_TRACE_LEVEL_ERROR,"*** dcp_ChOpen() Illegal value(s) for ClientCh(%u), AllocOnRequest(%u), HelloEnable(%u)",
                DetailPtr->ClientCh, DetailPtr->AllocOnRequest, DetailPtr->HelloEnable);
            RespVal = DCP_RSP_ERR_PARAM;
            dcp_CallbackRbu(SysPtr, RbuPtr, RespVal);
        }
        else
        {
            /* 240810lrg001: create channel structure after getting path info
		    */
		    ChPtr = dcp_ChAlloc(RbuPtr, SysPtr, DetailPtr);
		    if (LSA_HOST_PTR_ARE_EQUAL(ChPtr, LSA_NULL))
		    {
			    DCP_ERROR_TRACE_00(0, LSA_TRACE_LEVEL_ERROR,"*** dcp_ChOpen() Cannot allocate channel");
			    dcp_CallbackRbu(SysPtr, RbuPtr, RespVal);
		    }
            else
            {
                /* Put upper RQB to Channel queue
                */
                ChPtr = dcp_EnqueUpper(RbuPtr);
                DCP_IS_VALID_PTR(ChPtr);
                if (ChPtr->DetailPtr->ClientCh == DCP_CDB_CLIENT_CHANNEL)
                {
                    /* Client channel
                    */
                    RespVal = dcp_ClientOpen(ChPtr);
                }
                else
                {
                    /* Server channel
                    */
                    RespVal = DCP_RSP_OK;

                    if (DCP_DEV_ETH == ChPtr->DetailPtr->DevId)
                    {
                        /* Allocate Timer for Server Identify response and client hold time
                        */
                        DCP_TIMER_PTR_TYPE	TimerPtr;

                        for (Idx = 0; Idx <= DCP_NUM_SERVER_IDENTIFY_RESOURCES; Idx++)
                        {
                            TimerPtr = dcp_TimerAlloc(ChPtr, LSA_TIMER_TYPE_ONE_SHOT, LSA_TIME_BASE_100MS);
                            if (LSA_HOST_PTR_ARE_EQUAL(TimerPtr, LSA_NULL))
                            {
                                RespVal = DCP_RSP_ERR_RESOURCE;
                                break;
                            }
                            else if (Idx < DCP_NUM_SERVER_IDENTIFY_RESOURCES)
                            {
                                /* Identify timer
                                */
                                TimerPtr->Single = LSA_FALSE;
                                dcp_TimerEnque(&ChPtr->TimerQueue, TimerPtr);
                            }
                            else
                            {
                                /* Client hold  timer
                                */
                                TimerPtr->Single = LSA_TRUE;
                                ChPtr->TimerPtr = TimerPtr;
                            }
                        }
                    }
                }
            }
			if (DCP_RSP_OK == RespVal)
			{
				/* Exists a channel with
				 * the same LowerId?
				*/
                DCP_IS_VALID_PTR(ChPtr);
				RespVal = dcp_ChJoin(ChPtr);
				if (RespVal == DCP_RSP_OK_ACTIVE)
				{
					/* No: open new Lower channel
					*/
					RespVal = dcp_OpenLowerStart(ChPtr);
				}
			    else if (RespVal == DCP_RSP_OK)
				{
					/* Yes: only allocate Lower Request blocks
					*/
					RblPu.VoidPtr = LSA_NULL;
					RespVal = dcp_OpenLowerFin(ChPtr, RblPu);
				}
			}
			if ((RespVal != DCP_RSP_OK) && (RespVal != DCP_RSP_OK_ACTIVE))
			{
				/* On error: free Path info and channel
				 * 240810lrg001: Callback before RELEASE_PATH_INFO() in dcp_ChFree()
				*/
                DCP_IS_VALID_PTR(ChPtr);
                dcp_DequeUpper(RbuPtr);
				dcp_ChClean(ChPtr);
				dcp_RequestDone(ChPtr, RbuPtr, RespVal);
				dcp_ChFree(ChPtr);
			}
		}
	}
	else
	{
		/* 240810lrg001: FATAL on error from GET_PATH_INFO()
		*/
		RespVal = DCP_RSP_ERR_SYS_PATH;
		DCP_ERROR_TRACE_01(0, LSA_TRACE_LEVEL_ERROR,"*** dcp_ChOpen() RetVal=0x%x from DCP_GET_PATH_INFO",
				RetVal);
		dcp_ErrorInternal(DCP_MODULE_ID, __LINE__, DCP_ERR_CONFIG, LSA_NULL, 0);
	}

	DCP_FUNCTION_TRACE_03(0, LSA_TRACE_LEVEL_NOTE, "  < dcp_ChOpen() RbuPtr=%X ChPtr=%X RespVal=0x%x",
			RbuPtr, ChPtr, RespVal);
	return(RespVal);
}

/*****************************************************************************/
/* Internal function: dcp_ChClose()                                          */
/* Close channel                                                             */
/*****************************************************************************/
static LSA_UINT16  DCP_LOCAL_FCT_ATTR  dcp_ChClose(
DCP_CHA_PTR		ChPtr)
{
	DCP_LOWER_RQB_PTR_TYPE	RblPu;
	LSA_UINT16 RespVal = DCP_RSP_ERR_SEQUENCE;

	DCP_FUNCTION_TRACE_04(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  > dcp_ChClose() ChPtr=%X State=0x%x Handle=%d ChLowPtr=%X",
			ChPtr, ChPtr->State, ChPtr->Handle, ChPtr->ChLowPtr);

	RblPu.VoidPtr = LSA_NULL;

	/* If the channel is open and no Requests are currently in process...
	*/
	if ((ChPtr->State == DCP_CHA_STATE_OPEN)
	&& (ChPtr->QueueUpper.Count == 1))
	{
		ChPtr->State = DCP_CHA_STATE_CLOSING;
		if (1 < ChPtr->ChLowPtr->ChCount)
		{
			/* There is another upper channel connectet
			 * to the lower channel:
			*/
			RespVal = DCP_RSP_OK;
			if ((ChPtr->DetailPtr->ClientCh == DCP_CDB_SERVER_CHANNEL)
			||  (ChPtr->DetailPtr->DevId == DCP_DEV_FSU))
			{
				/* Server channel: first cancel
				 * lower Recv RQBs
				*/
				RespVal = dcp_CancelRecv(ChPtr, 0, 0);
			}
			else
			{
				if (ChPtr->SendReqCount != 0)		//051207lrg001
				{
					ChPtr->WaitSendCnf = LSA_TRUE;
					RespVal = DCP_RSP_OK_ACTIVE;
				}
				else
				{
					/* Free this upper client channel
					 * and Callback
					*/
					dcp_CloseLowerFin(ChPtr, RblPu);
				}
			}
		}
		else
		{
			/* Start closing lower channel
			*/
			RespVal = dcp_CancelLower(ChPtr, LSA_TRUE, LSA_NULL, RblPu);
		}
	}

	DCP_FUNCTION_TRACE_02(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  < dcp_ChClose() ChPtr=%X RespVal=0x%x",
			ChPtr, RespVal);
	return(RespVal);
}

/*****************************************************************************/
/* External LSA access function: dcp_open_channel()                          */
/*****************************************************************************/
LSA_VOID  DCP_UPPER_IN_FCT_ATTR  dcp_open_channel(
DCP_UPPER_RQB_PTR_TYPE  RbuPtr)
{
	LSA_UINT16  RespVal = DCP_RSP_OK_ACTIVE;

	DCP_ENTER();
	DCP_UPPER_TRACE_02(0, LSA_TRACE_LEVEL_NOTE, ">>> dcp_open_channel() RbuPtr=%X HandleUpper=%d",
			RbuPtr, DCP_UPPER_RQB_HANDLE_UPPER_GET(RbuPtr));

	/* Mark Request block as active
	*/
	DCP_UPPER_RQB_RESPONSE_SET(RbuPtr, DCP_RSP_OK_ACTIVE);
	switch(DCP_UPPER_RQB_OPCODE_GET(RbuPtr))
	{
	case DCP_OPC_OPEN_CHANNEL:
		RespVal = dcp_ChOpen(RbuPtr);
		break;

	default:
		DCP_UPPER_RQB_RESPONSE_SET(RbuPtr, DCP_RSP_ERR_SYNTAX);
		dcp_ErrorUser(DCP_MODULE_ID, __LINE__, DCP_ERR_BAD_OPCODE, RbuPtr, sizeof(DCP_RQB_TYPE));
	}

	DCP_UPPER_TRACE_02(0, LSA_TRACE_LEVEL_NOTE, "<<< dcp_open_channel() RbuPtr=%X RespVal=0x%x",
			RbuPtr, RespVal);
	DCP_EXIT();
	return;
}

/*****************************************************************************/
/* External LSA access function: dcp_close_channel()                         */
/*****************************************************************************/
LSA_VOID  DCP_UPPER_IN_FCT_ATTR  dcp_close_channel(
DCP_UPPER_RQB_PTR_TYPE  RbuPtr)
{
	DCP_CHA_PTR ChPtr;
	LSA_UINT16	RespVal = DCP_RSP_OK_ACTIVE;
	LSA_UINT32	TraceIdx = 0;

	DCP_ENTER();
	DCP_UPPER_TRACE_02(0, LSA_TRACE_LEVEL_NOTE, ">>> dcp_close_channel() RbuPtr=%X Handle=%d",
			RbuPtr, DCP_UPPER_RQB_HANDLE_GET(RbuPtr));

	/* Put RQB into channel queue
	*/
	ChPtr = dcp_EnqueUpper(RbuPtr);
	if(!LSA_HOST_PTR_ARE_EQUAL(ChPtr, LSA_NULL))
	{
		TraceIdx = ChPtr->TraceIdx;
		DCP_FUNCTION_TRACE_03(TraceIdx, LSA_TRACE_LEVEL_NOTE, "  > dcp_close_channel() RbuPtr=%X ChPtr=%X Handle=%d",
				RbuPtr, ChPtr, DCP_UPPER_RQB_HANDLE_GET(RbuPtr));

		/* Mark Request block as active
		 */
		DCP_UPPER_RQB_RESPONSE_SET(RbuPtr, DCP_RSP_OK_ACTIVE);
		switch(DCP_UPPER_RQB_OPCODE_GET(RbuPtr))
		{
		case DCP_OPC_CLOSE_CHANNEL:
			RespVal = dcp_ChClose(ChPtr);
			break;

		default:
			RespVal = DCP_RSP_ERR_SYNTAX;
		}

		/* On error the request block is returned here
		 * else at lower close channel confirmation
		*/
		if ((RespVal != DCP_RSP_OK_ACTIVE)
		&&  (RespVal != DCP_RSP_OK))
		{
			dcp_RequestDone(ChPtr, RbuPtr, RespVal);
		}
	}
	else
	{
		RespVal = DCP_RSP_ERR_REF;
		DCP_UPPER_RQB_RESPONSE_SET(RbuPtr, RespVal);
		dcp_ErrorUser(DCP_MODULE_ID, __LINE__, DCP_ERR_BAD_HANDLE, RbuPtr, sizeof(DCP_RQB_TYPE));
	}

	DCP_UPPER_TRACE_03(TraceIdx, LSA_TRACE_LEVEL_NOTE, "<<< dcp_close_channel() RbuPtr=%X ChPtr=%X RespVal=0x%x",
			RbuPtr, ChPtr, RespVal);
	DCP_EXIT();
	return;
}

/*****************************************************************************/
/* External LSA access function: dcp_request()                               */
/*****************************************************************************/
LSA_VOID  DCP_UPPER_IN_FCT_ATTR  dcp_request(
DCP_UPPER_RQB_PTR_TYPE  RbuPtr)
{
	DCP_CHA_PTR	ChPtr;
	LSA_UINT16	RespVal = DCP_RSP_OK_ACTIVE;
	LSA_UINT32	TraceIdx = 0;

	DCP_ENTER();
	DCP_UPPER_TRACE_03(0, LSA_TRACE_LEVEL_NOTE, ">>> dcp_request() RbuPtr=%X Opcode=%d Handle=%d",
			RbuPtr, DCP_UPPER_RQB_OPCODE_GET(RbuPtr), DCP_UPPER_RQB_HANDLE_GET(RbuPtr));

#ifdef DCP_DO_TIMEOUT_EXTERN
	if (DCP_OPC_TIMER == DCP_UPPER_RQB_OPCODE_GET(RbuPtr))
	{
		/* 150405lrg001: pass Timer RQB also after close_channel
		 */
		dcp_TimeoutRequest(RbuPtr);
		DCP_UPPER_TRACE_02(0, LSA_TRACE_LEVEL_NOTE, "<<< dcp_request() TimerRbu=%x RespVal=0x%x",
				RbuPtr, RespVal);
		DCP_EXIT();
		return;
	}
#endif

	/* Check Handle and queue request block
	 */
	ChPtr = dcp_EnqueUpper(RbuPtr);
	if(!LSA_HOST_PTR_ARE_EQUAL(ChPtr, LSA_NULL))
	{
		TraceIdx = ChPtr->TraceIdx;
		DCP_FUNCTION_TRACE_03(TraceIdx, LSA_TRACE_LEVEL_NOTE, "  > dcp_request() RbuPtr=%X ChPtr=%X Handle=%d",
				RbuPtr, ChPtr, DCP_UPPER_RQB_HANDLE_GET(RbuPtr));

		if (ChPtr->State != DCP_CHA_STATE_OPEN)
		{
			RespVal = DCP_RSP_ERR_SEQUENCE;
		}
		else if (ChPtr->DetailPtr->DevId != DCP_DEV_ETH)
		{
			RespVal = DCP_RSP_ERR_REF;
		}
		else
		{
			/* Mark request block as active and pass it
			 * dependent on channel type to Server or Client
			*/
			DCP_UPPER_RQB_RESPONSE_SET(RbuPtr, DCP_RSP_OK_ACTIVE);

			RespVal = DCP_RSP_ERR_SYNTAX;
			if (ChPtr->DetailPtr->ClientCh == DCP_CDB_CLIENT_CHANNEL)
			{
				/* Clientkanal
				 */
				RespVal = dcp_ClientRequest(ChPtr, RbuPtr);
			}
			else
			{
				/* Serverkanal
				*/
				RespVal = dcp_ServerRequest(ChPtr, RbuPtr);
			}
		}

		/* The Request block is confirmed now
		 * if the request is already finished.
		*/
		if(RespVal != DCP_RSP_OK_ACTIVE)
		{
			dcp_RequestDone(ChPtr, RbuPtr, RespVal);
		}
	}
	else
	{
		/* Invalid Handle
		*/
		RespVal = DCP_RSP_ERR_REF;
		DCP_UPPER_RQB_RESPONSE_SET(RbuPtr, RespVal);
		dcp_ErrorUser(DCP_MODULE_ID, __LINE__, DCP_ERR_BAD_HANDLE, RbuPtr, sizeof(DCP_RQB_TYPE));
	}

	DCP_UPPER_TRACE_03(TraceIdx, LSA_TRACE_LEVEL_NOTE, "<<< dcp_request() RbuPtr=%X ChPtr=%X RespVal=0x%x",
			RbuPtr, ChPtr, RespVal);
	DCP_EXIT();
	return;
}

/*****************************************************************************/
/* Internal function: dcp_ClientOpen()                                       */
/* Allocate Resources for all Client services                                */
/*****************************************************************************/
LSA_UINT16 	DCP_LOCAL_FCT_ATTR dcp_ClientOpen(
DCP_CHA_PTR				ChPtr)
{
	LSA_INT		Idx;
	DCP_TIMER_PTR_TYPE	TimerPtr;
	LSA_UINT16			RespVal = LSA_RSP_OK;

	DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  > dcp_ClientOpen() ChPtr=%X ClientResources=%d AllocOnRequest=%d",
			ChPtr, ChPtr->DetailPtr->ClientResources, ChPtr->DetailPtr->AllocOnRequest);

	for(Idx = 0; Idx < ChPtr->DetailPtr->ClientResources; Idx++)
	{
		/* Allocate Timer for Identify/Set/Get Requests
		*/
		TimerPtr = dcp_TimerAlloc(ChPtr, LSA_TIMER_TYPE_ONE_SHOT, LSA_TIME_BASE_100MS);
		if (LSA_HOST_PTR_ARE_EQUAL(TimerPtr, LSA_NULL))
		{
			RespVal = LSA_RSP_ERR_RESOURCE;
			break;
		}
		else
		{
			/* Common client timer
			*/
			TimerPtr->Single = LSA_FALSE;
			dcp_TimerEnque(&ChPtr->TimerQueue, TimerPtr);
		}
	}

	if ((RespVal == LSA_RSP_OK) && (ChPtr->DetailPtr->HelloEnable == DCP_CDB_HELLO_ENABLE))
	{
#ifdef DCP_CLIENT_HELLO_ENABLE
		/* Allocate Timer for Hello Requests
		*/
		TimerPtr = dcp_TimerAlloc(ChPtr, LSA_TIMER_TYPE_ONE_SHOT, LSA_TIME_BASE_10MS);
		if (LSA_HOST_PTR_ARE_EQUAL(TimerPtr, LSA_NULL))
		{
			RespVal = LSA_RSP_ERR_RESOURCE;
		}
		else
		{
			/* Hello timer
			*/
			Idx++;
			TimerPtr->Single = LSA_TRUE;
			ChPtr->TimerPtr = TimerPtr;
		}
#else
		RespVal = DCP_RSP_ERR_CONFIG;
		DCP_ERROR_TRACE_00(ChPtr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** dcp_ClientOpen() Details.HelloEnable set but DCP_CLIENT_HELLO_ENABLE not defined");
#endif
	}

	DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  < dcp_ClientOpen() ChPtr=%X RespVal=0x%x: timer allocated=%u",
			ChPtr, RespVal, Idx);
	return(RespVal);
}

/*****************************************************************************/
/* Internal function: dcp_ClientRqb()                                        */
/* Allocate RQB resources for all Client services                            */
/*****************************************************************************/
LSA_UINT16 	DCP_LOCAL_FCT_ATTR dcp_ClientRqb(
DCP_CHA_PTR				ChPtr)
{
	LSA_UINT16	RespVal = DCP_RSP_OK;
	LSA_INT		SendAllocCount = 0;
	LSA_INT		i, RecvAllocCount = 0;

	DCP_FUNCTION_TRACE_02(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  > dcp_ClientRqb() ChPtr=%X ClientResources=%u",
			ChPtr, ChPtr->DetailPtr->ClientResources);

	if  (ChPtr->DetailPtr->AllocOnRequest == DCP_CDB_ALLOC_LOWER_RQBS_AT_OPEN_CHANNEL)
	{
		/* On OPEN_CHANNEL: allocate send/recv RQBs for all possibly
		 * concurrent GET-/SET- and IDENTIFY Requests
		 * AP01509782: use DCP_NOSERVICE (->QueueLower) for common client send resources (not HELLO)
		*/
		while (SendAllocCount < ChPtr->DetailPtr->ClientResources)
		{
			/* Allocate Lower Send RQB
			*/
			RespVal = dcp_AllocLower(ChPtr, 0, DCP_NOSERVICE, LSA_TRUE);
			if (DCP_RSP_OK == RespVal)
			{
				SendAllocCount++;
			}
			else
			{
				break;
			}
		}
		if (RespVal != DCP_RSP_ERR_RESOURCE)
		{
			/* Allocate Lower recv RQBs
			*/
			RecvAllocCount = 2 * ChPtr->DetailPtr->ClientResources;
			for (i = 0; i < RecvAllocCount; i++)
			{
				RespVal = dcp_AllocLower(ChPtr, 0, DCP_NOSERVICE, LSA_FALSE);
				if (DCP_RSP_OK == RespVal)
				{
					ChPtr->QueueRecvCnt++;
				}
				else
				{
					break;
				}
			}
		}
	}
	if ((DCP_RSP_OK == RespVal)
	&&  (ChPtr->DetailPtr->HelloEnable == DCP_CDB_HELLO_ENABLE))
	{
		/* Let the HELLO service allocate its RQBs
		*/
		RespVal = dcp_ClientHello(ChPtr, LSA_NULL, 0);
	}

	DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  < dcp_ClientRqb() ChPtr=%X RetVal=0x%x RecvAllocCount=%u",
			ChPtr, RespVal, RecvAllocCount);
	return(RespVal);
}

/*****************************************************************************/
/* Internal function: dcp_ReqHdrSet()                                        */
/* Fill DCP request frame header                                             */
/*****************************************************************************/
static LSA_INT 	DCP_LOCAL_FCT_ATTR dcp_ReqHdrSet(
DCP_CHA_PTR 			ChPtr,
DCP_LOWER_RQB_PTR_TYPE	RblPu,
LSA_UINT8				Service,
const DCP_USR_BYTE_PTR	DstAdrPtr,
DCP_REQ_DATA		*	DataPtr,
LSA_INT					DataLen,
LSA_UINT8				Tidx,
LSA_UINT16				ChXid,
LSA_UINT16				Faltung)
{
	DCP_ETH_SEND_PTR		EthFramePtr;
	DCP_ETH_TXBYTE_PTR_TYPE	DstPtr;
	LSA_UINT8			*	SrcPtr;
	LSA_UINT32				Xid = 0;
	LSA_INT					SendLen = 0;

	DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  > dcp_ReqHdrSet() ChPtr=%X RbEthPtr=%X Service=%d",
			ChPtr, RblPu.RbEthPtr, Service);

	if (ChPtr->DetailPtr->DevId == DCP_DEV_ETH)
	{
		/* Transfer channel handle as high Byte of high Word in XID and
		 * transfer Timer index as low Byte of high Word in XID
		 * AP01503994: max. DCP_MAX_CHANNELS = 63 to free uppermost 2 bits of XID (upper channel handle)
		*/
		Xid = DCP_CLIENT_XID_PNIO;
		Xid <<= 6;      //lint !e845 The left argument to operator '<<' is certain to be 0, 12.01.2016 HM, see define of DCP_CLIENT_XID_PNIO
		Xid |= (LSA_UINT8)ChPtr->Handle;
		Xid <<= 8;
		Xid |= Tidx;
		Xid <<= 16;
		Xid |= (LSA_UINT32)ChXid;

		EthFramePtr = (DCP_ETH_SEND_PTR)(void*)DCP_ETH_RQB_SEND_PTR_GET(RblPu.RbEthPtr);

		/* Ethernetheader
		*/
		DCP_ETH_FRAME_MACADR_SET(&EthFramePtr->Hdr.SrcMacAdr[0], &ChPtr->ChLowPtr->LocalMacAdr[0]);
		if (!LSA_HOST_PTR_ARE_EQUAL(DstAdrPtr, LSA_NULL))
		{
			DCP_ETH_FRAME_MACADR_SET(&EthFramePtr->Hdr.DstMacAdr[0], DstAdrPtr);
		}
		else if (DCP_HELLO == Service)
		{
			DCP_MAC_ADR HelloAddr = DCP_HELLO_MC_ADDR;
			DCP_ETH_FRAME_MACADR_SET(&EthFramePtr->Hdr.DstMacAdr[0], HelloAddr);
		}
		else
		{
			DCP_MAC_ADR IdentifyAddr = DCP_IDENTIFY_MC_ADDR;
			DCP_ETH_FRAME_MACADR_SET(&EthFramePtr->Hdr.DstMacAdr[0], IdentifyAddr);
		}

		/* SRT Header
		*/
		EthFramePtr->Hdr.VlanTag[0] = DCP_VLANTAG_0;
		EthFramePtr->Hdr.VlanTag[1] = DCP_VLANTAG_1;
		EthFramePtr->Hdr.VlanTag[2] = DCP_VLANTAG_2;
		EthFramePtr->Hdr.VlanTag[3] = DCP_VLANTAG_3;
		EthFramePtr->Hdr.EthType[0] = DCP_ETHTYPE_0;
		EthFramePtr->Hdr.EthType[1] = DCP_ETHTYPE_1;
		EthFramePtr->Hdr.FrameId[0] = DCP_FRAMEID_0;

		if (DCP_IDENTIFY == Service)
		{
			EthFramePtr->Hdr.FrameId[1] = DCP_FRAMEID_MRQ;
		}
		else if (DCP_HELLO == Service)
		{
			EthFramePtr->Hdr.FrameId[1] = DCP_FRAMEID_HLO;
		}
		else
		{
			EthFramePtr->Hdr.FrameId[1] = DCP_FRAMEID_URR;
		}


		/* DCP PDU Header
		*/
		EthFramePtr->Pdu.Req.Hdr.ServiceId = Service;
		EthFramePtr->Pdu.Req.Hdr.ServiceType = DCP_ROLE_REQ;

		/* Response Delay = Faltungsfaktor in Identify frame
		*/
		EthFramePtr->Pdu.Req.Hdr.RspDelayHigh = DCP_HIBYTE(Faltung);
		EthFramePtr->Pdu.Req.Hdr.RspDelayLow =  DCP_LOBYTE(Faltung);

		/* Transaction id and data length following the PDU Header
		*/
		//DCP_SWAPD(&Xid, &EthFramePtr->Pdu.Req.Hdr.XidHigh);   //This line produces Lint msg 415: Likely access of out-of-bounds pointer
		EthFramePtr->Pdu.Req.Hdr.XidHigh = DCP_SWAP16(DCP_HIWORD(Xid));
		EthFramePtr->Pdu.Req.Hdr.XidLow  = DCP_SWAP16(DCP_LOWORD(Xid));
		EthFramePtr->Pdu.Req.Hdr.Length  = DCP_SWAP16(DataLen);

		if (DataLen != 0)
		{
			/* DCP PDU Data
			 */
			DstPtr = &EthFramePtr->Pdu.Req.Data.Byte[0];
			SrcPtr = &DataPtr->Byte[0];
			for (SendLen = 0; SendLen < DataLen; SendLen++)
				DstPtr[SendLen] =  SrcPtr[SendLen];
		}

		SendLen += DCP_ETHDR_SIZE + DCP_HEADER_SIZE;
	}
	else
	{
		/* currently only Ethernet is supported
		 */
		DCP_ERROR_TRACE_02(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "*** dcp_ReqHdrSet() ChPtr=%X DCP_ERR_DEVICE=0x%x",
				ChPtr, DCP_ERR_DEVICE);
	}

	DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  < dcp_ReqHdrSet() ChPtr=%X Xid=0x%x SendLen=%d",
			ChPtr, Xid, SendLen);
	return(SendLen);
}

/*****************************************************************************/
/* Internal function: dcp_ClientIdentify()                                   */
/* Execute the client service IDENTIFY                                       */
/*****************************************************************************/
LSA_UINT16 	DCP_LOCAL_FCT_ATTR dcp_ClientIdentify(
DCP_CHA_PTR				ChPtr,
DCP_UPPER_RQB_PTR_TYPE	RbuPtr,
LSA_INT					Restart)
{
#ifndef DCP_REQ_LOCAL_DATA
	DCP_REQ_DATA			dcp_Request;
#endif
	LSA_USER_ID_TYPE		UserId;
	LSA_INT					DataLen;
	DCP_LOWER_RQB_PTR_TYPE	RblPu;
	DCP_DATASET_IDENT	*	FilterPtr;
	LSA_UINT32				Timeout;		// AP01278788: was UINT16
	LSA_UINT32				RecvCnt = 2;	// 740218 was: 2 * ChPtr->DetailPtr->ClientResources;
	LSA_UINT16				RespVal = DCP_RSP_OK_ACTIVE;
	LSA_UINT16				Faltung;
	LSA_INT					NodeCnt = 0;
	LSA_INT					i = Restart;	/* Dummy usage of Restart */
	DCP_TIMER_PTR_TYPE		TimerPtr = LSA_NULL;


	DCP_FUNCTION_TRACE_04(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  > dcp_ClientIdentify() ChPtr=%X RbuPtr=%X RequestId=0x%08x Restart=%d",
			ChPtr, RbuPtr, RbuPtr->args.Identify.RequestId, Restart);

	/* Calculate time (counter) depending on Faltungsfaktor and Tolerance
	 * with max. two seconds addition at all and reserve a Timer
	 * which will be started at positive send confirmation of the request
	*/
	Faltung = RbuPtr->args.Identify.Faltung;
	if (Faltung == 0)
	{
		Faltung = DCP_IDENTIFY_FALTUNG_DEFAULT;
	}

	/* 070711lrg: Timeout like IEC61158-6 V2.3
	Timeout = (0xffff % Faltung)*10;
	*/
	/* AP01278788: Identify client timeout
	*/
	if (Faltung == 1)
	{
		Timeout = 0;
	}
	else
	{
		Timeout = Faltung * DCP_MULTICAST_DELAY_BASE;	// Milliseconds
	}

	if ((0xffff - Timeout) < RbuPtr->args.Identify.Tolerance)
	{
		/* 190607lrg001: Tolerance check
		*/
		RbuPtr->args.Identify.NodeCnt = 0;
		RespVal = DCP_RSP_ERR_PARAM;
		DCP_ERROR_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_ERROR, "*** dcp_ClientIdentify() Timeout=%u + Tolerance=%u > 0xffff: Invalid Faltung=%u or Tolerance",
				Timeout, RbuPtr->args.Identify.Tolerance, Faltung);
	}
	else
	{
		if (0 == RbuPtr->args.Identify.Tolerance)
		{
			Timeout += 1000;								/* + 1 Second   */
		}
		else
		{
			Timeout += RbuPtr->args.Identify.Tolerance;		/* + Tolerance   */
		}

		/* AP01278788: IEC61158-6 V2.3: Round up to next second
		 * and limit timeout to a maximum of 65 seconds
		*/
		if ((Faltung > 1)
		&&  ((Timeout % 1000) != 0))
		{
			Timeout = ((Timeout / 1000) * 1000) + 1000;
		}
		if (Timeout > 65000)
		{
			Timeout = 65000;
		}

		/* Calculate Timer Ticks
		*/
		if (0 == (Timeout % DCP_MULTICAST_RSP_TIMER_BASE))
		{
			Timeout = Timeout / DCP_MULTICAST_RSP_TIMER_BASE;
		}
		else
		{
			/* Round up to next tick
			 */
			Timeout = (Timeout / DCP_MULTICAST_RSP_TIMER_BASE) + 1;
		}

		RblPu.VoidPtr = LSA_NULL;
		TimerPtr = dcp_TimerGet(ChPtr, DCP_TIMER_IDENTREQ, (LSA_UINT16)Timeout, &ChPtr->Identify.TimerQueue, RblPu, RbuPtr);
		if (LSA_HOST_PTR_ARE_EQUAL(TimerPtr, LSA_NULL))
		{
			/* All Timers are busy: Retry
			*/
			RespVal = DCP_RSP_OK_MORE;
		}
		else
		{
			/* Store NodeCnt from RQB locally and preset it
			 * with 0 as return value in the upper RQB
			*/
			NodeCnt = RbuPtr->args.Identify.NodeCnt;
			RbuPtr->args.Identify.NodeCnt = 0;
		}
	}

	if (   (RespVal == DCP_RSP_OK_ACTIVE)
	    && (ChPtr->DetailPtr->AllocOnRequest == DCP_CDB_ALLOC_LOWER_RQBS_AT_OPEN_CHANNEL))
	{
		if ((ChPtr->ChLowPtr->QueueRecv.Count < RecvCnt)
		||  (0 == ChPtr->QueueLower.Count))
		{
			/* 180407lrg001: check for enough Receive RQBs in lower Layer
			*/
			if (!LSA_HOST_PTR_ARE_EQUAL(ChPtr->ChLowPtr->ChServerPtr, LSA_NULL))
			{
				RecvCnt += ChPtr->ChLowPtr->ChServerPtr->QueueGet.Count;
				RecvCnt += ChPtr->ChLowPtr->ChServerPtr->QueueSet.Count;
				RecvCnt += ChPtr->ChLowPtr->ChServerPtr->QueueIdentify.Count;
			}
			RecvCnt -= ChPtr->ChLowPtr->QueueRecv.Count;
			if ((0 != ChPtr->QueueLower.Count)
			&&  (ChPtr->ChLowPtr->RecvRblAct >= (LSA_INT)RecvCnt))
			{
				DCP_IS_VALID_PTR(TimerPtr);
				/* 180407lrg001: only provide additional count of RQBs from queue
				*/
				DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  - dcp_ClientIdentify() reducing RecvCnt: Tid=%u RecvRblAct=%u LowerQueueRecv.Count=%u",
						TimerPtr->Id, ChPtr->ChLowPtr->RecvRblAct, ChPtr->ChLowPtr->QueueRecv.Count);
				RecvCnt = ChPtr->ChLowPtr->QueueRecv.Count;
			}
			else
			{
				/* Free reserved Timer
				*/
				RespVal = DCP_RSP_OK_MORE;
				dcp_TimerPut(ChPtr, TimerPtr, &ChPtr->Identify.TimerQueue);
				DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_WARN, "*** dcp_ClientIdentify() number of lower RQBs expected=%u: QueueLower.Count=%u QueueRecv.Count=%u",
						RecvCnt, ChPtr->QueueLower.Count, ChPtr->ChLowPtr->QueueRecv.Count);
			}
		}
	}

	if (RespVal == DCP_RSP_OK_ACTIVE)
	{
		DCP_IS_VALID_PTR(TimerPtr);
		if (ChPtr->DetailPtr->AllocOnRequest == DCP_CDB_ALLOC_LOWER_RQBS_AT_REQUEST)
		{
			/* Allocate Lower Receive RQBs for one Request:
			 * 740218 was: 2 * ChPtr->DetailPtr->ClientResources;
			*/
			LSA_UINT32 RecvMax = 2;

			for (RecvCnt = 0; RecvCnt < RecvMax; RecvCnt++)
			{
				RespVal = dcp_AllocLower(ChPtr, TimerPtr->Id, DCP_IDENTIFY, LSA_FALSE);
				if (DCP_RSP_OK != RespVal)
				{
					break;
				}
			}
			/* Allocate Lower Send RQB
			*/
			if (RespVal != DCP_RSP_ERR_RESOURCE)
			{
				RespVal = dcp_AllocLower(ChPtr, TimerPtr->Id, DCP_IDENTIFY, LSA_TRUE);
			}
		}
		else
		{
			/* Move one send RQB from common to service queue and set the UID to DCP_IDENTIFY
			*/
			RblPu = ChPtr->QueueLower.FirstPu;
			dcp_DequeLower(&ChPtr->QueueLower, RblPu, ChPtr);
			UserId.uvar32= DCP_ETH_RQB_USERID_UVAR32_GET(RblPu.RbEthPtr);
			UserId.uvar8_array[DCP_CLIENT_UID] = DCP_IDENTIFY;
			UserId.uvar8_array[DCP_INDEX_UID] = TimerPtr->Id;
			DCP_ETH_RQB_USERID_UVAR32_SET(RblPu.RbEthPtr, UserId.uvar32);
			dcp_EnqueLower(&ChPtr->Identify.QueueSend, RblPu, ChPtr);
		}

		/* If all resources are present...
		*/
		if (RespVal != DCP_RSP_ERR_RESOURCE)
		{
			ChPtr->Identify.Active = LSA_TRUE;
			RespVal = DCP_RSP_OK_ACTIVE;

			/* Put Lower Receive RQBs from Queue to Lower Layer
			*/
			while (ChPtr->Identify.Active && (RecvCnt != 0))
			{
				RecvCnt--;
				if (ChPtr->ChLowPtr->QueueRecv.Count != 0)
				{
					RblPu = ChPtr->ChLowPtr->QueueRecv.FirstPu;
					dcp_DequeLower(&ChPtr->ChLowPtr->QueueRecv, RblPu, ChPtr);
					dcp_DataRecv(ChPtr, RblPu, TimerPtr->Id, DCP_IDENTIFY);
				}
				else
				{
					dcp_ErrorInternal(DCP_MODULE_ID, __LINE__, DCP_ERR_NO_RBU, LSA_NULL, RecvCnt);
				}
			}

			if (ChPtr->Identify.Active)
			{
				/* Copy Node Count from RQB to Client channel data
				 * and store upper Request block
				 * in the Transaction structure
				*/
				TimerPtr->Tract.Txs.Response = DCP_RSP_OK_ACTIVE;
				TimerPtr->Tract.Txs.Count = NodeCnt;
				TimerPtr->Tract.Txs.Idx = 0;
				TimerPtr->Tract.Txs.RbuPtr = RbuPtr;
				TimerPtr->Tract.Txs.Xid = ++ChPtr->Identify.Xid;

				/* Remove Lower Send RQB from Queue
				*/
				RblPu = ChPtr->Identify.QueueSend.FirstPu;
				dcp_DequeLower(&ChPtr->Identify.QueueSend, RblPu, ChPtr);

				/* Build Identify Request PDU
				*/
				DataLen = 0;

				if (RbuPtr->args.Identify.FilterCnt != 0)
				{
					/* Calculate length of TLV Filter chain
					*/
					for (i = 0; i< RbuPtr->args.Identify.FilterCnt; i++)
					{
						FilterPtr = (DCP_DATASET_IDENT *)(void*)(RbuPtr->args.Identify.FilterPtr + DataLen);
						DataLen += DCP_DS_IDENT_HDR_SIZE + DCP_SWAP16(FilterPtr->Len);
						if ((DataLen % 2) != 0)
						{
							DataLen++;
						}
					}
					if (DataLen <= DCP_MAX_REQ_DATA_SIZE)
					{
						/* Copy Request TLV chain to local buffer
						*/
						DCP_COPY_UPPER_TO_LOCAL_MEM(RbuPtr->args.Identify.FilterPtr,
								&dcp_Request.Identify.Filter.Opt,
								(LSA_UINT16)DataLen, ChPtr->SysPtr);
					}
					else
					{
						RespVal = DCP_RSP_ERR_PARAM;
					}
				}
				else
				{
					FilterPtr = &dcp_Request.Identify.Filter;

					/* Send NO Filter in PDU
					*/
					FilterPtr->Opt = DCP_DBID_NOFILTER;
					FilterPtr->Id = DCP_DSID_NOFILTER;
					FilterPtr->Len = 0;
					DataLen += DCP_DS_IDENT_HDR_SIZE;
				}

				if (DCP_RSP_OK_ACTIVE == RespVal)
				{
					/* Copy local Request Structure to Send PDU
					 * and fill the Header
					*/
					DataLen = dcp_ReqHdrSet(ChPtr, RblPu, DCP_IDENTIFY,
								RbuPtr->args.Identify.AddrPtr,
								&dcp_Request, DataLen,
								TimerPtr->Id,
								TimerPtr->Tract.Txs.Xid,
								Faltung);

					/* Send IDENTIFY Request
					*/
					dcp_DataSend(ChPtr, RblPu, DataLen);
				}
			}
		}

		if (DCP_RSP_OK_ACTIVE != RespVal)
		{
			if (ChPtr->DetailPtr->AllocOnRequest == DCP_CDB_ALLOC_LOWER_RQBS_AT_REQUEST)
			{
				/* Without asynchronous allocation:
				 * Free lower RQBs in AllocOnRequest mode
				*/
				while ((RecvCnt != 0)
				&&     (ChPtr->ChLowPtr->QueueRecv.Count != 0))
				{
					RblPu = ChPtr->ChLowPtr->QueueRecv.FirstPu;
					dcp_DequeLower(&ChPtr->ChLowPtr->QueueRecv, RblPu, ChPtr);
					dcp_FreeLowerRb(ChPtr,RblPu);
					RecvCnt--;
				}
				if (ChPtr->Identify.QueueSend.Count != 0)
				{
					RblPu = ChPtr->Identify.QueueSend.FirstPu;
					dcp_DequeLower(&ChPtr->Identify.QueueSend, RblPu, ChPtr);
					dcp_FreeLowerRb(ChPtr,RblPu);
				}
			}
			/* Free reserved Timer
			*/
			if (!LSA_HOST_PTR_ARE_EQUAL(TimerPtr, LSA_NULL))    //lint !e774 Boolean within 'if' always evaluates to True, keep it for better readability, HM 13.01.2016
			{
				dcp_TimerPut(ChPtr, TimerPtr, &ChPtr->Identify.TimerQueue);
			}
		}
	}

	DCP_FUNCTION_TRACE_02(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  < dcp_ClientIdentify() ChPtr=%X RespVal=0x%x",
			ChPtr, RespVal);

	return(RespVal);
}

/*****************************************************************************/
/* Internal function: dcp_ClientSet()                                        */
/* Execute the client service SET                                            */
/*****************************************************************************/
LSA_UINT16 	DCP_LOCAL_FCT_ATTR dcp_ClientSet(
DCP_CHA_PTR				ChPtr,
DCP_UPPER_RQB_PTR_TYPE	RbuPtr,
LSA_INT					TxRetry)
{
#ifndef DCP_REQ_LOCAL_DATA
	DCP_REQ_DATA			dcp_Request;
#endif
	LSA_USER_ID_TYPE		UserId;
	DCP_LOWER_RQB_PTR_TYPE	RblPu;
	LSA_INT					DataLen;
	LSA_INT					i;
	LSA_UINT8				*BytePtr;
	DCP_DATASET				*DsPtr;
	LSA_UINT16				DsLen;
	LSA_UINT16				RespVal = DCP_RSP_OK_ACTIVE;
	LSA_UINT32				RecvCnt = 1;		// 180407lrg001
	DCP_TIMER_PTR_TYPE		TimerPtr = LSA_NULL;

	DCP_FUNCTION_TRACE_04(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  > dcp_ClientSet() ChPtr=%X RbuPtr=%X RequestId=0x%08x TxRetry=%d",
			ChPtr, RbuPtr, RbuPtr->args.Set.RequestId, TxRetry);

	RblPu.VoidPtr = LSA_NULL;
	if (0 == TxRetry)
	{
		/* Reserve Timer: will be started in dcp_ClientConf()
		*/
		TimerPtr = dcp_TimerGet(ChPtr, DCP_TIMER_SETREQ, DCP_CLIENT_UC_TIMEOUT_TICKS, &ChPtr->Set.TimerQueue, RblPu, RbuPtr);
		if (LSA_HOST_PTR_ARE_EQUAL(TimerPtr, LSA_NULL))
		{
			/* All Timers busy: Retry in dcp_ClientDone()
			 */
			RespVal = DCP_RSP_OK_MORE;
		}
		else
		{
			TimerPtr->Tract.Txs.Xid = ++ChPtr->Set.Xid;
			TimerPtr->Tract.Txs.Count = TxRetry;
			TimerPtr->Tract.Txs.RbuPtr = RbuPtr;
			TimerPtr->Tract.Txs.Response = DCP_RSP_OK_ACTIVE;
		}
	}
	else if (!LSA_HOST_PTR_ARE_EQUAL(ChPtr->Set.RetryTimerPtr, LSA_NULL))
	{
		/* 1737982: Use same Timer with old Xid for retry: will be started in dcp_ClientConf()
		*/
		TimerPtr = ChPtr->Set.RetryTimerPtr;
		TimerPtr->CallId = DCP_TIMER_SETREQ;
		TimerPtr->Time = DCP_CLIENT_UC_TIMEOUT_TICKS;
		TimerPtr->RbPu = RblPu;
		TimerPtr->RbPtr = RbuPtr;
		TimerPtr->Tract.Txs.Count = TxRetry;
		TimerPtr->Tract.Txs.RbuPtr = RbuPtr;
		TimerPtr->Tract.Txs.Response = DCP_RSP_OK_ACTIVE;
		ChPtr->Set.RetryTimerPtr = LSA_NULL;
		DCP_FUNCTION_TRACE_06(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  - dcp_ClientSet() ChPtr=%X RbuPtr=%X TxRetry=%d: using TimerPtr=%X Tid=%u Xid=0x%x",
			ChPtr, RbuPtr, TxRetry, TimerPtr, TimerPtr->Id, TimerPtr->Tract.Txs.Xid);
	}
	else
	{
		RespVal = DCP_RSP_ERR_RESOURCE;
		DCP_ERROR_TRACE_01(ChPtr->TraceIdx, LSA_TRACE_LEVEL_FATAL, "*** dcp_ClientSet() ChPtr=%X: Retry timer not found", ChPtr);
		dcp_ErrorInternal(DCP_MODULE_ID, __LINE__, DCP_ERR_TIMER, ChPtr, 0);
	}

	if (   (RespVal == DCP_RSP_OK_ACTIVE)
	    && (ChPtr->DetailPtr->AllocOnRequest == DCP_CDB_ALLOC_LOWER_RQBS_AT_OPEN_CHANNEL))
	{
		if ((0 == ChPtr->ChLowPtr->QueueRecv.Count)
		||  (0 == ChPtr->QueueLower.Count))
		{
			DCP_IS_VALID_PTR(TimerPtr);
			/* 180407lrg001: check for enough Receive RQBs in lower Layer
			*/
			if (!LSA_HOST_PTR_ARE_EQUAL(ChPtr->ChLowPtr->ChServerPtr, LSA_NULL))
			{
				RecvCnt += ChPtr->ChLowPtr->ChServerPtr->QueueGet.Count;
				RecvCnt += ChPtr->ChLowPtr->ChServerPtr->QueueSet.Count;
				RecvCnt += ChPtr->ChLowPtr->ChServerPtr->QueueIdentify.Count;
			}
			if ((0 != ChPtr->QueueLower.Count)
			&&  (ChPtr->ChLowPtr->RecvRblAct >= (LSA_INT)RecvCnt))
			{
				/* 180407lrg001: do not provide a lower Receive RQB
				*/
				DCP_FUNCTION_TRACE_04(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  - dcp_ClientSet() reducing RecvCnt: Tid=%u RecvRblAct=%u LowerQueueRecv.Count=%u SetQueueRecv.Count=%u",
						TimerPtr->Id, ChPtr->ChLowPtr->RecvRblAct, ChPtr->ChLowPtr->QueueRecv.Count, ChPtr->QueueLower.Count);
				RecvCnt = 0;
			}
			else
			{
				/* Free reserved Timer
				*/
				RespVal = DCP_RSP_OK_MORE;
				dcp_TimerPut(ChPtr, TimerPtr, &ChPtr->Set.TimerQueue);
				DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_WARN, "*** dcp_ClientSet() number of lower RQBs expected=%u: QueueLower.Count=%u QueueRecv.Count=%u",
						RecvCnt, ChPtr->QueueLower.Count, ChPtr->ChLowPtr->QueueRecv.Count);
			}
		}
	}

	if (RespVal == DCP_RSP_OK_ACTIVE)
	{
		if (ChPtr->DetailPtr->AllocOnRequest == DCP_CDB_ALLOC_LOWER_RQBS_AT_REQUEST)
		{
			/* allocate lower recv RQB
			 */
			RespVal = dcp_AllocLower(ChPtr, 0, DCP_SET, LSA_FALSE);
			if (RespVal != DCP_RSP_ERR_RESOURCE)
			{
				/* Allocate lower send RQB
				*/
				RespVal = dcp_AllocLower(ChPtr, 0, DCP_SET, LSA_TRUE);
			}
		}
		else
		{
			DCP_IS_VALID_PTR(TimerPtr);
			/* Move one send RQB from common to service queue and set the UID to DCP_SET
			*/
			RblPu = ChPtr->QueueLower.FirstPu;
			dcp_DequeLower(&ChPtr->QueueLower, RblPu, ChPtr);
			UserId.uvar32= DCP_ETH_RQB_USERID_UVAR32_GET(RblPu.RbEthPtr);
			UserId.uvar8_array[DCP_CLIENT_UID] = DCP_SET;
			UserId.uvar8_array[DCP_INDEX_UID] = TimerPtr->Id;
			DCP_ETH_RQB_USERID_UVAR32_SET(RblPu.RbEthPtr, UserId.uvar32);
			dcp_EnqueLower(&ChPtr->Set.QueueSend, RblPu, ChPtr);
		}

		/* If all resources are present...
		 */
		if (RespVal != DCP_RSP_ERR_RESOURCE)
		{
			RespVal = DCP_RSP_OK_ACTIVE;

			/* Build SET Request PDU
			*/
			DsLen = 0;
			DataLen = 0;
			DsPtr = &dcp_Request.Set.Dataset;
			BytePtr = &dcp_Request.Set.Dataset.Hdr.Opt;
#ifdef DCP_SET_TRANSACTION
			if (RbuPtr->args.Set.TrStart)
			{
				/* Insert Start Tranaction TLV
				 */
				DsPtr->Hdr.Opt = DCP_DBID_CTRL;
				DsPtr->Hdr.Id =  DCP_DSID_START;
				DsPtr->Hdr.Len = DCP_SWAP16(2);
				DsPtr->Status = 0;
				DataLen += DCP_DS_HDR_SIZE+2;
				BytePtr += DataLen;
			}
#endif
			/* Calculate length of TLV chain
			 */
			for (i = 0; i< RbuPtr->args.Set.OptCnt; i++)
			{
				DsPtr = (DCP_DATASET *)(void*)(RbuPtr->args.Set.ReqPtr + DsLen);
				DsLen += DCP_DS_HDR_SIZE + DCP_SWAP16(DsPtr->Hdr.Len);
				if ((DsLen % 2) != 0)
				{
					DsLen++;
				}
			}
			if ((DataLen + DsLen) <= DCP_MAX_REQ_DATA_SIZE)
			{
				if (DsLen != 0)
				{
					/* Copy Request TLV chain to local buffer
					*/
					DCP_COPY_UPPER_TO_LOCAL_MEM(RbuPtr->args.Set.ReqPtr, BytePtr,
							DsLen, ChPtr->SysPtr);
					DataLen += DsLen;
				}
#ifdef DCP_SET_TRANSACTION
				if (RbuPtr->args.Set.TrEnd)
				{
					if ((DataLen + DCP_DS_HDR_SIZE+2) <= DCP_MAX_REQ_DATA_SIZE)
					{
						/* Insert End Tranaction TLV
						 */
						BytePtr += DsLen;
						DsPtr = (DCP_DATASET *)BytePtr;
						DsPtr->Hdr.Opt = DCP_DBID_CTRL;
						DsPtr->Hdr.Id =  DCP_DSID_END;
						DsPtr->Hdr.Len = DCP_SWAP16(2);
						DsPtr->Status = 0;
						DataLen += DCP_DS_HDR_SIZE+2;
					}
					else
					{
						DataLen = 0;
					}
				}
#endif
			}
			else
			{
				DataLen = 0;
			}

			if (DataLen != 0)
			{
				ChPtr->Set.Active = LSA_TRUE;
				if (0 != RecvCnt)			// 180407lrg001
				{
    				DCP_IS_VALID_PTR(TimerPtr);
					/* Provide Lower Receive RQB from Queue to Lower Layer
					*/
					RblPu = ChPtr->ChLowPtr->QueueRecv.FirstPu;
					dcp_DequeLower(&ChPtr->ChLowPtr->QueueRecv, RblPu, ChPtr);
					dcp_DataRecv(ChPtr, RblPu, TimerPtr->Id, DCP_SET);
				}

				/* If the Recv RQB is not indicated with error
				*/
				if (ChPtr->Set.Active)
				{
    				DCP_IS_VALID_PTR(TimerPtr);
					/* Remove Lower Send RQB from Queue
					*/
					RblPu = ChPtr->Set.QueueSend.FirstPu;
					dcp_DequeLower(&ChPtr->Set.QueueSend, RblPu, ChPtr);

					/* Copy local Request Structure to Send PDU	and fill the Header
					 */
					DataLen = dcp_ReqHdrSet(ChPtr, RblPu, DCP_SET, RbuPtr->args.Set.AddrPtr,
							&dcp_Request, DataLen, TimerPtr->Id,
							TimerPtr->Tract.Txs.Xid, DCP_RESERVED);
					/* Send SET Request
					*/
					dcp_DataSend(ChPtr, RblPu, DataLen);
					RespVal = DCP_RSP_OK_ACTIVE;
				}
			}
			else
			{
				RespVal = DCP_RSP_ERR_PARAM;
			}
		}

		if (RespVal != DCP_RSP_OK_ACTIVE)
		{
			/* Without asynchronous allocation:
			 * Free lower RQBs in AllocOnRequest mode
			*/
			if (ChPtr->DetailPtr->AllocOnRequest == DCP_CDB_ALLOC_LOWER_RQBS_AT_REQUEST)
			{
				if (ChPtr->ChLowPtr->QueueRecv.Count != 0)
				{
					RblPu = ChPtr->ChLowPtr->QueueRecv.FirstPu;
					dcp_DequeLower(&ChPtr->ChLowPtr->QueueRecv, RblPu, ChPtr);
					dcp_FreeLowerRb(ChPtr,RblPu);
				}
				if (ChPtr->Set.QueueSend.Count != 0)
				{
					RblPu = ChPtr->Set.QueueSend.FirstPu;
					dcp_DequeLower(&ChPtr->Set.QueueSend, RblPu, ChPtr);
					dcp_FreeLowerRb(ChPtr,RblPu);
				}
			}
			/* Free reserved Timer
			*/
			if (!LSA_HOST_PTR_ARE_EQUAL(TimerPtr, LSA_NULL))
			{
				dcp_TimerPut(ChPtr, TimerPtr, &ChPtr->Set.TimerQueue);
			}
		}
	}

	DCP_FUNCTION_TRACE_02(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  < dcp_ClientSet() ChPtr=%X RespVal=0x%x",
			ChPtr, RespVal);
	return(RespVal);
}

/*****************************************************************************/
/* Internal function: dcp_ClientGet()                                        */
/* Execute the client service GET                                            */
/*****************************************************************************/
LSA_UINT16 	DCP_LOCAL_FCT_ATTR dcp_ClientGet(
DCP_CHA_PTR				ChPtr,
DCP_UPPER_RQB_PTR_TYPE	RbuPtr,
LSA_INT					TxRetry)
{
#ifndef DCP_REQ_LOCAL_DATA
	DCP_REQ_DATA					dcp_Request;
#endif
	LSA_USER_ID_TYPE				UserId;
	DCP_LOWER_RQB_PTR_TYPE			RblPu;
	DCP_TYPE DCP_UPPER_MEM_ATTR		*TypePtr;
	LSA_INT							DataLen;
	LSA_INT							i;
	LSA_UINT16						RespVal = DCP_RSP_OK_ACTIVE;
	LSA_UINT32						RecvCnt = 1;		// 180407lrg001
	DCP_TIMER_PTR_TYPE				TimerPtr = LSA_NULL;

	DCP_FUNCTION_TRACE_04(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  > dcp_ClientGet() ChPtr=%X RbuPtr=%X RequestId=0x%08x TxRetry=%d",
			ChPtr, RbuPtr, RbuPtr->args.Get.RequestId, TxRetry);

	RblPu.VoidPtr = LSA_NULL;
	if (0 == TxRetry)
	{
		/* Reserve Timer: will be started in dcp_ClientConf()
		*/
		TimerPtr = dcp_TimerGet(ChPtr, DCP_TIMER_GETREQ, DCP_CLIENT_UC_TIMEOUT_TICKS, &ChPtr->Get.TimerQueue, RblPu, RbuPtr);
		if (LSA_HOST_PTR_ARE_EQUAL(TimerPtr, LSA_NULL))
		{
			/* All Timers busy: Retry in dcp_ClientDone()
			 */
			RespVal = DCP_RSP_OK_MORE;
		}
		else
		{
			TimerPtr->Tract.Txs.Xid = ++ChPtr->Get.Xid;
			TimerPtr->Tract.Txs.Count = TxRetry;
			TimerPtr->Tract.Txs.RbuPtr = RbuPtr;
			TimerPtr->Tract.Txs.Response = DCP_RSP_OK_ACTIVE;
		}
	}
	else if (!LSA_HOST_PTR_ARE_EQUAL(ChPtr->Get.RetryTimerPtr, LSA_NULL))
	{
		/* 1737982: Use same Timer with old Xid for retry: will be started in dcp_ClientConf()
		*/
		TimerPtr = ChPtr->Get.RetryTimerPtr;
		TimerPtr->CallId = DCP_TIMER_GETREQ;
		TimerPtr->Time = DCP_CLIENT_UC_TIMEOUT_TICKS;
		TimerPtr->RbPu = RblPu;
		TimerPtr->RbPtr = RbuPtr;
		TimerPtr->Tract.Txs.Count = TxRetry;
		TimerPtr->Tract.Txs.RbuPtr = RbuPtr;
		TimerPtr->Tract.Txs.Response = DCP_RSP_OK_ACTIVE;
		ChPtr->Set.RetryTimerPtr = LSA_NULL;
		DCP_FUNCTION_TRACE_06(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  - dcp_ClientGet() ChPtr=%X RbuPtr=%X TxRetry=%d: using TimerPtr=%X Tid=%u Xid=0x%x",
			ChPtr, RbuPtr, TxRetry, TimerPtr, TimerPtr->Id, TimerPtr->Tract.Txs.Xid);
	}
	else
	{
		RespVal = DCP_RSP_ERR_RESOURCE;
		DCP_ERROR_TRACE_01(ChPtr->TraceIdx, LSA_TRACE_LEVEL_FATAL, "*** dcp_ClientGet() ChPtr=%X: Retry timer not found", ChPtr);
		dcp_ErrorInternal(DCP_MODULE_ID, __LINE__, DCP_ERR_TIMER, ChPtr, 0);
	}

	if (   (RespVal == DCP_RSP_OK_ACTIVE)
	    && (ChPtr->DetailPtr->AllocOnRequest == DCP_CDB_ALLOC_LOWER_RQBS_AT_OPEN_CHANNEL))
	{
		if ((0 == ChPtr->ChLowPtr->QueueRecv.Count)
		||  (0 == ChPtr->QueueLower.Count))
		{
			DCP_IS_VALID_PTR(TimerPtr);
			/* 180407lrg001: check for enough Receive RQBs in lower Layer
			*/
			if (!LSA_HOST_PTR_ARE_EQUAL(ChPtr->ChLowPtr->ChServerPtr, LSA_NULL))
			{
				RecvCnt += ChPtr->ChLowPtr->ChServerPtr->QueueGet.Count;
				RecvCnt += ChPtr->ChLowPtr->ChServerPtr->QueueSet.Count;
				RecvCnt += ChPtr->ChLowPtr->ChServerPtr->QueueIdentify.Count;
			}
			if ((0 != ChPtr->QueueLower.Count)
			&&  (ChPtr->ChLowPtr->RecvRblAct >= (LSA_INT)RecvCnt))
			{
				/* 180407lrg001: do not provide a lower Receive RQB
				*/
				DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  - dcp_ClientGet() reducing RecvCnt: Tid=%u RecvRblAct=%u QueueRecv.Count=%u",
						TimerPtr->Id, ChPtr->ChLowPtr->RecvRblAct, ChPtr->ChLowPtr->QueueRecv.Count);
				RecvCnt = 0;
			}
			else
			{
				/* Free reserved Timer
				*/
				RespVal = DCP_RSP_OK_MORE;
				dcp_TimerPut(ChPtr, TimerPtr, &ChPtr->Get.TimerQueue);
				DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_WARN, "*** dcp_ClientGet() number of lower RQBs expected=%u: QueueLower.Count=%u QueueRecv.Count=%u",
						RecvCnt, ChPtr->QueueLower.Count, ChPtr->ChLowPtr->QueueRecv.Count);
			}
		}
	}

	if (RespVal == DCP_RSP_OK_ACTIVE)
	{
		if (ChPtr->DetailPtr->AllocOnRequest == DCP_CDB_ALLOC_LOWER_RQBS_AT_REQUEST)
		{
			/* Allocate lower recv RQB
			*/
			RespVal = dcp_AllocLower(ChPtr, 0, DCP_GET, LSA_FALSE);
			if (RespVal != DCP_RSP_ERR_RESOURCE)
			{
				/* Allocate lower send RQB
				*/
				RespVal = dcp_AllocLower(ChPtr, 0, DCP_GET, LSA_TRUE);
			}
		}
		else
		{
			DCP_IS_VALID_PTR(TimerPtr);
			/* Move one send RQB from common to service queue and set the UID to DCP_GET
			*/
			RblPu = ChPtr->QueueLower.FirstPu;
			dcp_DequeLower(&ChPtr->QueueLower, RblPu, ChPtr);
			UserId.uvar32= DCP_ETH_RQB_USERID_UVAR32_GET(RblPu.RbEthPtr);
			UserId.uvar8_array[DCP_CLIENT_UID] = DCP_GET;
			UserId.uvar8_array[DCP_INDEX_UID] = TimerPtr->Id;
			DCP_ETH_RQB_USERID_UVAR32_SET(RblPu.RbEthPtr, UserId.uvar32);
			dcp_EnqueLower(&ChPtr->Get.QueueSend, RblPu, ChPtr);
		}

		/* If all resources are present...
		*/
		if (RespVal != DCP_RSP_ERR_RESOURCE)
		{
			ChPtr->Get.Active = LSA_TRUE;
			RespVal = DCP_RSP_OK_ACTIVE;

			if (0 != RecvCnt)			// 180407lrg001
			{
				DCP_IS_VALID_PTR(TimerPtr);
				/* ...provide Lower Receive RQB from Queue to Lower Layer
				*/
				RblPu = ChPtr->ChLowPtr->QueueRecv.FirstPu;
				dcp_DequeLower(&ChPtr->ChLowPtr->QueueRecv, RblPu, ChPtr);
				dcp_DataRecv(ChPtr, RblPu, TimerPtr->Id, DCP_GET);
			}

			/* If the Recv RQB is not indicated with error
			*/
			if (ChPtr->Get.Active)
			{
				DCP_IS_VALID_PTR(TimerPtr);
				/* Remove Lower Send RQB from Queue
				*/
				RblPu = ChPtr->Get.QueueSend.FirstPu;
				dcp_DequeLower(&ChPtr->Get.QueueSend, RblPu, ChPtr);

				/* Build GET Request PDU
				*/
				DataLen = 0;
				TypePtr = (DCP_TYPE *)(void*)RbuPtr->args.Get.ReqPtr;
				for (i = 0; i < RbuPtr->args.Get.OptCnt; i++)
				{
					/* Copy Selector(Types) list of user to
					 * the locale Get Request Structure
					 */
					dcp_Request.Get.Type[i] = *TypePtr;
					TypePtr++;
					DataLen += DCP_TYPE_SIZE;
				}

				/* Copy local Request Structure to Send PDU	and fill the Header
				*/
				DataLen = dcp_ReqHdrSet(ChPtr, RblPu, DCP_GET, RbuPtr->args.Get.AddrPtr,
							&dcp_Request, DataLen, TimerPtr->Id,
							TimerPtr->Tract.Txs.Xid, DCP_RESERVED);

				/* Send GET Request
				*/
				dcp_DataSend(ChPtr, RblPu, DataLen);
				RespVal = DCP_RSP_OK_ACTIVE;
			}
		}

		if (RespVal != DCP_RSP_OK_ACTIVE)
		{
			/* Without asynchronous allocation:
			 * Free lower RQBs in AllocOnRequest mode
			*/
			if (ChPtr->DetailPtr->AllocOnRequest == DCP_CDB_ALLOC_LOWER_RQBS_AT_REQUEST)
			{
				if (ChPtr->ChLowPtr->QueueRecv.Count != 0)
				{
					RblPu = ChPtr->ChLowPtr->QueueRecv.FirstPu;
					dcp_DequeLower(&ChPtr->ChLowPtr->QueueRecv, RblPu, ChPtr);
					dcp_FreeLowerRb(ChPtr,RblPu);
				}
				if (ChPtr->Get.QueueSend.Count != 0)
				{
					RblPu = ChPtr->Get.QueueSend.FirstPu;
					dcp_DequeLower(&ChPtr->Get.QueueSend, RblPu, ChPtr);
					dcp_FreeLowerRb(ChPtr,RblPu);
				}
			}
			/* Free reserved Timer
			*/
			if (!LSA_HOST_PTR_ARE_EQUAL(TimerPtr, LSA_NULL))
			{
				dcp_TimerPut(ChPtr, TimerPtr, &ChPtr->Get.TimerQueue);
			}
		}
	}

	DCP_FUNCTION_TRACE_02(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  < dcp_ClientGet() ChPtr=%X RespVal=0x%x",
			ChPtr, RespVal);

	return(RespVal);
}

/*****************************************************************************/
/* Internal function: dcp_ClientHello()                                      */
/* Execute the client service HELLO                                          */
/* or if RbuPtr == LSA_NULL: only allocate lower RQBs                        */
/*****************************************************************************/
LSA_UINT16 	DCP_LOCAL_FCT_ATTR dcp_ClientHello(
DCP_CHA_PTR				ChPtr,
DCP_UPPER_RQB_PTR_TYPE	RbuPtr,
LSA_INT					TxRetry)
{
#ifndef DCP_REQ_LOCAL_DATA
	DCP_REQ_DATA			dcp_Request;
#endif
	DCP_LOWER_RQB_PTR_TYPE	RblPu;
	LSA_UINT32				u;
	LSA_INT					i;
	LSA_UINT8				*BytePtr;
	DCP_DATASET				*DsPtr;
	LSA_UINT16				DsLen;
	LSA_UINT16				RetVal;
	LSA_UINT16				RespVal = DCP_RSP_OK_ACTIVE;
	LSA_INT					DataLen = 0;
	DCP_TIMER_PTR_TYPE		TimerPtr = LSA_NULL;

	DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  > dcp_ClientHello() ChPtr=%X RbuPtr=%X TxRetry=%d",
			ChPtr, RbuPtr, TxRetry);

	if (LSA_HOST_PTR_ARE_EQUAL(RbuPtr, LSA_NULL) && (0 == TxRetry))
	{
		/* At OPEN_CHANNEL: allocate all lower Send Request blocks
		*/
		for (u = ChPtr->Hello.QueueSend.Count; u < DCP_NUM_CLIENT_HELLO_RESOURCES; u++)
		{
			RespVal = dcp_AllocLower(ChPtr, 0, DCP_HELLO, LSA_TRUE);
			if (DCP_RSP_OK != RespVal)
				break;
		}
	}
	else if ((0 == TxRetry) && (!LSA_HOST_PTR_ARE_EQUAL(RbuPtr, LSA_NULL)) && (0 == RbuPtr->args.Hello.SendCnt))
	{
		/* Stop active HELLO.req
		*/
		RespVal = DCP_RSP_OK;
		if (ChPtr->Hello.Active)
		{
			if (!LSA_HOST_PTR_ARE_EQUAL(ChPtr->TimerPtr, LSA_NULL))
			{
				/* Timer unused
				*/
				TimerPtr = ChPtr->TimerPtr;
			}
			else if (!LSA_HOST_PTR_ARE_EQUAL(ChPtr->Hello.TimerQueue.FirstPtr, LSA_NULL))
			{
				/* Hello will be stopped on send confirmation
				*/
				TimerPtr = ChPtr->Hello.TimerQueue.FirstPtr;
				TimerPtr->Tract.Txs.Count = 0;
				TimerPtr = LSA_NULL;
			}
			else
			{
				/* Stop running Timer: Hello timer id is always TimerCount
				*/
				dcp_TimerStop(ChPtr, LSA_NULL, ChPtr->TimerCount, LSA_NULL);
				TimerPtr = ChPtr->TimerPtr;
			}
			
			if (!LSA_HOST_PTR_ARE_EQUAL(TimerPtr, LSA_NULL))
			{
				TimerPtr->Tract.Txs.Count = 0;
				if (!LSA_HOST_PTR_ARE_EQUAL(TimerPtr->RbPu.VoidPtr, LSA_NULL))
				{
				/* queue back lower RQB
				*/
					RblPu = TimerPtr->RbPu;
					TimerPtr->RbPu.VoidPtr = LSA_NULL;
					dcp_EnqueLower(&ChPtr->Hello.QueueSend, RblPu, ChPtr);
				}
			}
			
			ChPtr->Hello.Active = LSA_FALSE;
			if (ChPtr->Hello.QueueUpper.Count > 1)
			{
				/* If the Start RQB still is in the queue:
				 * confirm it prior to the Stop RQB
				*/
				RespVal = DCP_RSP_OK_ACTIVE;
			}
		}
	}
	else if ((0 == TxRetry) && ChPtr->Hello.Active)
	{
		/* Only 1 Start request allowed
		*/
		RespVal = DCP_RSP_ERR_SEQUENCE;
		DCP_ERROR_TRACE_02(ChPtr->TraceIdx, LSA_TRACE_LEVEL_ERROR, "*** dcp_ClientHello() ChPtr=%X RbuPtr=%X: Hello already running",
						ChPtr, RbuPtr);
	}
	else if ((0 == TxRetry) && (0 == ChPtr->Hello.QueueSend.Count))
	{
		RespVal = DCP_RSP_ERR_RESOURCE;
		DCP_ERROR_TRACE_02(ChPtr->TraceIdx, LSA_TRACE_LEVEL_ERROR, "*** dcp_ClientHello() ChPtr=%X RbuPtr=%X: No lower send RQB",
						ChPtr, RbuPtr);
	}
	else
	{
		RblPu.VoidPtr = LSA_NULL;
		if (0 == TxRetry)
		{
			DCP_IS_VALID_PTR(RbuPtr);
			/* Reserve Timer on User request:
			 * will be started below if Delay != 0 else in dcp_ClientConf()
			*/
			TimerPtr = dcp_TimerGet(ChPtr, DCP_TIMER_HELLOREQ, (LSA_UINT16)RbuPtr->args.Hello.Time, &ChPtr->Hello.TimerQueue, RblPu, RbuPtr);
			if (LSA_HOST_PTR_ARE_EQUAL(TimerPtr, LSA_NULL))
			{
				/* Timer already reserved
				*/
				RespVal = DCP_RSP_ERR_SEQUENCE;
				DCP_ERROR_TRACE_02(ChPtr->TraceIdx, LSA_TRACE_LEVEL_ERROR, "*** dcp_ClientHello() ChPtr=%X RbuPtr=%X: Timer already reserved",
								ChPtr, RbuPtr);
				dcp_ErrorInternal(DCP_MODULE_ID, __LINE__, DCP_ERR_TIMER, ChPtr, 0);
			}
			else
			{
				RespVal = DCP_RSP_OK_ACTIVE;

				/* Start Request: build HELLO Request PDU
				*/
				DsLen = 0;
				DataLen = 0;
				DsPtr = &dcp_Request.Hello.Dataset;
				BytePtr = &dcp_Request.Hello.Dataset.Hdr.Opt;

				/* Calculate length of TLV chain
				*/
				for (i = 0; i< RbuPtr->args.Hello.OptCnt; i++)
				{
					DsPtr = (DCP_DATASET *)(void*)(RbuPtr->args.Hello.ReqPtr + DsLen);
					DsLen += DCP_DS_HDR_SIZE + DCP_SWAP16(DsPtr->Hdr.Len);
					if ((DsLen % 2) != 0)
					{
						DsLen++;
					}
				}
				if ((DataLen + DsLen) <= DCP_MAX_REQ_DATA_SIZE)
				{
					if (DsLen != 0)
					{
						/* Copy Request TLV chain to local buffer
						*/
						DCP_COPY_UPPER_TO_LOCAL_MEM(RbuPtr->args.Hello.ReqPtr, BytePtr,
													DsLen, ChPtr->SysPtr);
						DataLen += DsLen;
					}
				}
				else
				{
					DataLen = 0;
					RespVal = DCP_RSP_ERR_PARAM;
					DCP_ERROR_TRACE_02(ChPtr->TraceIdx, LSA_TRACE_LEVEL_ERROR, "*** dcp_ClientHello() ChPtr=%X RbuPtr=%X invalid TLV chain",
							ChPtr, RbuPtr);
				}

				if (DataLen != 0)
				{
					TimerPtr->Tract.Txs.Xid = ++ChPtr->Hello.Xid;
					TimerPtr->Tract.Txs.Count = RbuPtr->args.Hello.SendCnt;
					ChPtr->Hello.Active = LSA_TRUE;
					ChPtr->Hello.RetryCount = 0;

					/* Get Lower Send RQB und upper RQB from Queues
					*/
					RblPu = ChPtr->Hello.QueueSend.FirstPu;
					dcp_DequeLower(&ChPtr->Hello.QueueSend, RblPu, ChPtr);
					dcp_GetUpperQ(&ChPtr->Hello.QueueUpper, RbuPtr);

					/* Copy local Request Structure to Send PDU	and fill the Header
					*/
					DataLen = dcp_ReqHdrSet(ChPtr, RblPu, DCP_HELLO, RbuPtr->args.Hello.AddrPtr, &dcp_Request,
								DataLen, TimerPtr->Id, TimerPtr->Tract.Txs.Xid, DCP_RESERVED);

					/* Put data length to timer for repetition
					*/
					TimerPtr->SendLen = DataLen;
				}
			}
		}
		else
		{
			/* Repetition: HELLO Request PDU already is in lower RQB
			*/
			TimerPtr = ChPtr->TimerPtr;
			if (!LSA_HOST_PTR_ARE_EQUAL(TimerPtr, LSA_NULL))
			{
				ChPtr->TimerPtr = LSA_NULL;
				dcp_TimerEnque(&ChPtr->Hello.TimerQueue, TimerPtr);
				if (LSA_HOST_PTR_ARE_EQUAL(TimerPtr->RbPu.VoidPtr, LSA_NULL)
				||  (0 == TimerPtr->SendLen))
				{
					RespVal = DCP_RSP_ERR_RESOURCE;
					DCP_ERROR_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_ERROR, "*** dcp_ClientHello() ChPtr=%X TxRetry=%d: No timer RbPu or invalid SendLen=%u",
									ChPtr, TxRetry, TimerPtr->SendLen);
					dcp_ErrorInternal(DCP_MODULE_ID, __LINE__, DCP_ERR_TIMER, ChPtr, 0);
				}
				else
				{
					DataLen = TimerPtr->SendLen;
					RblPu = TimerPtr->RbPu;
					TimerPtr->RbPu.VoidPtr = LSA_NULL;
					RespVal = DCP_RSP_OK_ACTIVE;
				}
			}
			else
			{
				/* Timer not in place
				*/
				RespVal = DCP_RSP_ERR_RESOURCE;
				DCP_ERROR_TRACE_02(ChPtr->TraceIdx, LSA_TRACE_LEVEL_ERROR, "*** dcp_ClientHello() ChPtr=%X TimerId=%u: Timer not in place",
						ChPtr, ChPtr->TimerCount);
				dcp_ErrorInternal(DCP_MODULE_ID, __LINE__, DCP_ERR_TIMER, ChPtr, ChPtr->TimerCount);

			}
		}
		if (RespVal == DCP_RSP_OK_ACTIVE)
		{
			if (!LSA_HOST_PTR_ARE_EQUAL(RbuPtr, LSA_NULL)
			&&  (RbuPtr->args.Hello.Delay != 0))
			{
				/* 300507lrg001: if the first request has to be send with delay:
				 * put lower RQB and delay time to Timer and start Timer now
				*/
				RetVal = dcp_TimerStart(ChPtr, TimerPtr, 0, &ChPtr->Hello.TimerQueue, (LSA_UINT16)RbuPtr->args.Hello.Delay, RblPu);
				if (RetVal != LSA_RET_OK)
				{
					DCP_ERROR_TRACE_04(ChPtr->TraceIdx, LSA_TRACE_LEVEL_ERROR, "*** dcp_ClientHello() ChPtr=%X TimerPtr=%X Id=%d: Hello Timer start RetVal=0x%x",
									ChPtr, TimerPtr, TimerPtr->Id, RetVal);
					dcp_ErrorInternal(DCP_MODULE_ID, __LINE__, DCP_ERR_TIMER, ChPtr, TimerPtr->Id);
				}
			}
			else
			{
				/* Send HELLO Request
				*/
				TimerPtr->Tract.Txs.Response = DCP_RSP_OK_ACTIVE;
				dcp_DataSend(ChPtr, RblPu, DataLen);
			}
		}
		else if (!LSA_HOST_PTR_ARE_EQUAL(TimerPtr, LSA_NULL))   //lint !e774 Boolean within 'if' always evaluates to True, keep it for better readability, HM 13.01.2016
		{
			/* Free reserved Timer
			*/
			dcp_TimerPut(ChPtr, TimerPtr, &ChPtr->Hello.TimerQueue);
		}
	}

	DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  < dcp_ClientHello() ChPtr=%X DataLen=%u RespVal=0x%x",
			ChPtr, DataLen, RespVal);

	return(RespVal);
}

/*****************************************************************************/
/* Internal function: dcp_ClientCancelTimer()                                */
/* Cancel timer for Identify/Get/Set requests                                */
/*****************************************************************************/
static LSA_VOID 	DCP_LOCAL_FCT_ATTR dcp_ClientCancelTimer(
DCP_CHA_PTR				ChPtr,
DCP_UPPER_RQB_PTR_TYPE	RbPtr,
LSA_INT					Idx,
LSA_UINT8				Uid)
{
	DCP_LOWER_RQB_PTR_TYPE	RbPu;
	DCP_CLIENT_DATA		*  	DataPtr = LSA_NULL;
	DCP_TIMER_PTR_TYPE 		TimerPtr = LSA_NULL;

	DCP_FUNCTION_TRACE_04(0, LSA_TRACE_LEVEL_NOTE, "  > dcp_ClientCancelTimer() ChPtr=%X RbPtr=%X Idx=%u Uid=%u",
			ChPtr, RbPtr, Idx, Uid);

	if (0 == Idx) 
	{
		/* first step: find number of requests to cancel
		*/
		ChPtr->Cancel.Count++;
	}
	else
	{
		/* second step: find timer
		*/
		TimerPtr = dcp_TimerTabFindRqb(ChPtr, RbPtr);
		if (!LSA_HOST_PTR_ARE_EQUAL(TimerPtr, LSA_NULL))
		{
			/* Stop Timer and call Client-Timeout
			*/
			dcp_TimerStop(ChPtr, TimerPtr, TimerPtr->Id, LSA_NULL);
			RbPu = TimerPtr->RbPu;
			TimerPtr->RbPu.VoidPtr = LSA_NULL;
			TimerPtr->Tract.Txs.Response = DCP_RSP_OK_CANCEL;
			dcp_ClientTimeout(ChPtr, RbPu, TimerPtr->RbPtr, TimerPtr, Uid);
		}
		else
		{
			DataPtr = dcp_ClientData(ChPtr, Uid);
			if (!LSA_HOST_PTR_ARE_EQUAL(DataPtr, LSA_NULL))
			{
				TimerPtr = dcp_TimerFindRqb(ChPtr, RbPtr, &DataPtr->TimerQueue);
				if (!LSA_HOST_PTR_ARE_EQUAL(TimerPtr, LSA_NULL))
				{
					/* Timer not yet running:
					 * Set Response to cancel at lower send confirmation
					*/
					TimerPtr->Tract.Txs.Response = DCP_RSP_OK_CANCEL;
				}
				else
				{
					dcp_ErrorInternal(DCP_MODULE_ID, __LINE__, DCP_ERR_TIMER, RbPtr, Uid);
				}
			}
			else
			{
				dcp_ErrorInternal(DCP_MODULE_ID, __LINE__, DCP_ERR_UID, LSA_NULL, Uid);
			}
		}
	}
	DCP_FUNCTION_TRACE_04(0, LSA_TRACE_LEVEL_NOTE, "  < dcp_ClientCancelTimer() ChPtr=%X Idx=%u TimerPtr=%X DataPtr=%X",
			ChPtr, Idx, TimerPtr, DataPtr);
}
	
/*****************************************************************************/
/* Internal function: dcp_ClientCancelReq()                                  */
/* Cancel Identify/Get/Set requests                                          */
/*****************************************************************************/
static LSA_UINT16 	DCP_LOCAL_FCT_ATTR dcp_ClientCancelReq(
DCP_CHA_PTR				ChPtr,
DCP_UPPER_RQB_PTR_TYPE	RbuPtr)
{
	DCP_UPPER_RQB_PTR_TYPE	RbPtr, NextPtr;
	LSA_OPCODE_TYPE			Opcode;
	LSA_INT					i;
	LSA_UINT16				RespVal = DCP_RSP_OK;
	LSA_UINT32				RequestId = RbuPtr->args.CancelReq.RequestId;

	DCP_FUNCTION_TRACE_05(0, LSA_TRACE_LEVEL_NOTE, "  > dcp_ClientCancelReq() ChPtr=%X RbuPtr=%X RequestId=0x%08x Retry.Fist=%X Retry.Count=%u",
			ChPtr, RbuPtr, RequestId, ChPtr->QueueRetry.FirstPtr, ChPtr->QueueRetry.Count);
	
	/* Cancel retry queue
	*/
	RbPtr = ChPtr->QueueRetry.FirstPtr;
	while (!LSA_HOST_PTR_ARE_EQUAL(RbPtr, LSA_NULL))
	{
		NextPtr = DCP_UPPER_RQB_NEXT_GET(RbPtr);
		Opcode = DCP_UPPER_RQB_OPCODE_GET(RbPtr);
		if (((DCP_OPC_IDENTIFY_REQ == Opcode) && (RbPtr->args.Identify.RequestId == RequestId))
		||  ((DCP_OPC_GET_REQ == Opcode) && (RbPtr->args.Get.RequestId == RequestId))
		||  ((DCP_OPC_SET_REQ == Opcode) && (RbPtr->args.Set.RequestId == RequestId)))
		{
			/* Remove upper RQB from queue and confirm this request
			*/
			dcp_GetUpperQ(&ChPtr->QueueRetry, RbPtr);
			dcp_RequestDone(ChPtr, RbPtr, DCP_RSP_OK_CANCEL);
		}
		RbPtr = NextPtr;
	}
	ChPtr->Cancel.Count = 0;

	/* Cancel service queues:
	 * i = 0: count RQBs i = 1: cancel
	*/
	for (i = 0; i < 2; i++)
	{
		/* Search IDENTIFY queue
		*/
		RbPtr = ChPtr->Identify.QueueUpper.FirstPtr;
		while (!LSA_HOST_PTR_ARE_EQUAL(RbPtr, LSA_NULL))
		{
			NextPtr = DCP_UPPER_RQB_NEXT_GET(RbPtr);
			if (RbPtr->args.Identify.RequestId == RequestId)
			{
				dcp_ClientCancelTimer(ChPtr, RbPtr, i, DCP_IDENTIFY);
			}
			RbPtr = NextPtr;
		}
		/* Search SET queue
		*/
		RbPtr = ChPtr->Set.QueueUpper.FirstPtr;
		while (!LSA_HOST_PTR_ARE_EQUAL(RbPtr, LSA_NULL))
		{
			NextPtr = DCP_UPPER_RQB_NEXT_GET(RbPtr);
			if (RbPtr->args.Set.RequestId == RequestId)
			{
				dcp_ClientCancelTimer(ChPtr, RbPtr, i, DCP_SET);
			}
			RbPtr = NextPtr;
		}
		/* Search GET queue
		*/
		RbPtr = ChPtr->Get.QueueUpper.FirstPtr;
		while (!LSA_HOST_PTR_ARE_EQUAL(RbPtr, LSA_NULL))
		{
			NextPtr = DCP_UPPER_RQB_NEXT_GET(RbPtr);
			if (RbPtr->args.Get.RequestId == RequestId)
			{
				dcp_ClientCancelTimer(ChPtr, RbPtr, i, DCP_GET);
			}
			RbPtr = NextPtr;
		}
	}
	if (0 != ChPtr->Cancel.Count)
	{
		RespVal = DCP_RSP_OK_ACTIVE;
	}
	DCP_FUNCTION_TRACE_04(0, LSA_TRACE_LEVEL_NOTE, "  < dcp_ClientCancelReq() ChPtr=%X RbuPtr=%X Cancel.Count=%u RespVal=0x%x",
			ChPtr, RbuPtr, ChPtr->Cancel.Count, RespVal);
	
	return(RespVal);
}
/*****************************************************************************/
/* Internal function: dcp_CheckIdentify()                                    */
/* Check Identify Parameters of upper Request block                          */
/*****************************************************************************/
static LSA_UINT16 	DCP_LOCAL_FCT_ATTR dcp_CheckIdentify(
DCP_CHA_PTR				ChPtr,
DCP_UPPER_RQB_PTR_TYPE	RbuPtr)
{
	LSA_UINT16		RespVal = DCP_RSP_OK_ACTIVE;

	DCP_FUNCTION_TRACE_03(0, LSA_TRACE_LEVEL_NOTE, "  > dcp_CheckIdentify() RbuPtr=%X NodePtr=%X NodeCnt%d",
			RbuPtr, RbuPtr->args.Identify.NodePtr, RbuPtr->args.Identify.NodeCnt);

	if (0 == ChPtr->DetailPtr->ClientResources)
	{
		RespVal = DCP_RSP_ERR_CONFIG;
		DCP_ERROR_TRACE_02(ChPtr->TraceIdx, LSA_TRACE_LEVEL_ERROR, "*** dcp_CheckIdentify() ChPtr=%X RbuPtr=%X: ClientResources=0 in channel details",
						ChPtr, RbuPtr);
	}
	else
	{
		if ((ChPtr->Cancel.Queue.Count != 0)
		&&  (ChPtr->Cancel.Queue.FirstPtr->args.CancelReq.RequestId == RbuPtr->args.Identify.RequestId))
		{
			/* Cancel in progress
			*/
			RespVal = DCP_RSP_ERR_SEQUENCE;
		}
		else
		{
			if (   (RbuPtr->args.Identify.Faltung > DCP_IDENTIFY_FALTUNG_MAX)
                || (   (RbuPtr->args.Identify.Notify != DCP_IDENTIFY_DO_NOT_NOTIFY_USER_ON_NEW_ENTRY)
                    && (RbuPtr->args.Identify.Notify != DCP_IDENTIFY_NOTIFY_USER_ON_NEW_ENTRY) ) 
               )
			{
				/* AP01278788: Faltung check
				*/
				RespVal = DCP_RSP_ERR_PARAM;
				DCP_ERROR_TRACE_03(0, LSA_TRACE_LEVEL_ERROR, "*** dcp_CheckIdentify() Faltung=%u > DCP_FALTUNG_MAX=%u, or possible illegal value for Notify(%u)",
						RbuPtr->args.Identify.Faltung, DCP_IDENTIFY_FALTUNG_MAX, RbuPtr->args.Identify.Notify);
			}
			else
			{
				/* Pointer and Length of return list
				*/
				if (   (RbuPtr->args.Identify.Notify == DCP_IDENTIFY_DO_NOT_NOTIFY_USER_ON_NEW_ENTRY)
                    && (   (RbuPtr->args.Identify.NodeCnt == 0)
				        ||  LSA_HOST_PTR_ARE_EQUAL(RbuPtr->args.Identify.NodePtr, LSA_NULL)))
				{
					RespVal = DCP_RSP_ERR_PARAM;
				}
		
				if ((DCP_RSP_OK_ACTIVE == RespVal)
				&&  LSA_HOST_PTR_ARE_EQUAL(RbuPtr->args.Identify.FilterPtr, LSA_NULL)
				&&  (RbuPtr->args.Identify.FilterCnt != 0))
				{
					RespVal = DCP_RSP_ERR_PARAM;
				}
		
				if ((DCP_RSP_OK_ACTIVE == RespVal)
				&&  LSA_HOST_PTR_ARE_EQUAL(RbuPtr->args.Identify.NodePtr, LSA_NULL)
				&&  (RbuPtr->args.Identify.NodeCnt != 0))
				{
					RespVal = DCP_RSP_ERR_PARAM;
				}
			}
		}
	}

	/* On error: reset Node count
	*/
	if (DCP_RSP_OK_ACTIVE != RespVal)
	{
		RbuPtr->args.Identify.NodeCnt = 0;
	}

	DCP_FUNCTION_TRACE_03(0, LSA_TRACE_LEVEL_NOTE, "  < dcp_CheckIdentify() RbuPtr=%X FilterPtr=%X RespVal=0x%x",
			RbuPtr, RbuPtr->args.Identify.FilterPtr, RespVal);
	return(RespVal);
}

/*****************************************************************************/
/* Internal function: dcp_CheckHello()                                       */
/* Check Hello Parameters of upper Request block                             */
/*****************************************************************************/
static LSA_UINT16 	DCP_LOCAL_FCT_ATTR dcp_CheckHello(
DCP_CHA_PTR				ChPtr,
DCP_UPPER_RQB_PTR_TYPE	RbuPtr)
{
	LSA_UINT16		RespVal = DCP_RSP_ERR_PARAM;

	DCP_FUNCTION_TRACE_03(0, LSA_TRACE_LEVEL_NOTE, "  > dcp_CheckHello() RbuPtr=%X AddrPtr=%X ReqPtr=%X",
			RbuPtr, RbuPtr->args.Hello.AddrPtr, RbuPtr->args.Hello.ReqPtr);

	if (ChPtr->DetailPtr->HelloEnable == DCP_CDB_HELLO_DISABLE)
	{
		RespVal = DCP_RSP_ERR_CONFIG;
		DCP_ERROR_TRACE_02(ChPtr->TraceIdx, LSA_TRACE_LEVEL_ERROR, "*** dcp_CheckHello() ChPtr=%X RbuPtr=%X: HelloEnable=FALSE in channel details",
						ChPtr, RbuPtr);
	}
	else
	{
		/* Pointer to buffer may be NULL only at HELLO stop (SendCnt == 0)
		*/
		if (!LSA_HOST_PTR_ARE_EQUAL(RbuPtr->args.Hello.ReqPtr, LSA_NULL)
		||  (RbuPtr->args.Hello.SendCnt == 0))
		{
			/* Count of TLVs may only be 0 at HELLO stop (SendCnt == 0)
			*/
			if ((RbuPtr->args.Hello.OptCnt != 0) || (RbuPtr->args.Hello.SendCnt == 0))
			{
				/* Time has to be valid if not HELLO stop
				 * and no repetition occurs
				*/
				if ((RbuPtr->args.Hello.SendCnt == 0) || (RbuPtr->args.Hello.SendCnt == 1)
				||  ((RbuPtr->args.Hello.Time > 0) && (RbuPtr->args.Hello.Time < 65536)))
				{
					/* 300507lrg001: Delay has to be valid if not HELLO stop
					*/
					if ((RbuPtr->args.Hello.SendCnt == 0)
					||  ((RbuPtr->args.Hello.Delay >= 0) && (RbuPtr->args.Hello.Delay < 65536)))
					{
						RespVal = DCP_RSP_OK_ACTIVE;
					}
				}
			}
		}
	}

	DCP_FUNCTION_TRACE_03(0, LSA_TRACE_LEVEL_NOTE, "  < dcp_CheckHello() OptCnt=%d SendCnt=%d RespVal=0x%x",
			RbuPtr->args.Hello.OptCnt, RbuPtr->args.Hello.SendCnt, RespVal);
	return(RespVal);
}

/*****************************************************************************/
/* Internal function: dcp_CheckSet()                                         */
/* Check Set Parameters of upper Request block                               */
/*****************************************************************************/
static LSA_UINT16 	DCP_LOCAL_FCT_ATTR dcp_CheckSet(
DCP_CHA_PTR				ChPtr,
DCP_UPPER_RQB_PTR_TYPE	RbuPtr)
{
	LSA_UINT16		RespVal = DCP_RSP_ERR_PARAM;

	DCP_FUNCTION_TRACE_03(0, LSA_TRACE_LEVEL_NOTE, "  > dcp_CheckSet() RbuPtr=%X AddrPtr=%X OptCnt=%d",
			RbuPtr, LSA_NULL, 0);

	if (0 == ChPtr->DetailPtr->ClientResources)
	{
		RespVal = DCP_RSP_ERR_CONFIG;
		DCP_ERROR_TRACE_02(ChPtr->TraceIdx, LSA_TRACE_LEVEL_ERROR, "*** dcp_CheckSet() ChPtr=%X RbuPtr=%X: ClientResources=0 in channel details",
						ChPtr, RbuPtr);
	}
	else
	{
		if ((ChPtr->Cancel.Queue.Count != 0)
		&&  (ChPtr->Cancel.Queue.FirstPtr->args.CancelReq.RequestId == RbuPtr->args.Set.RequestId))
		{
			/* Cancel in progress
			*/
			RespVal = DCP_RSP_ERR_SEQUENCE;
		}
		else
		{
			/* Check pointers to destination addresses and buffers
			*/
			if (!LSA_HOST_PTR_ARE_EQUAL(RbuPtr->args.Set.AddrPtr, LSA_NULL)
			&&  (!LSA_HOST_PTR_ARE_EQUAL(RbuPtr->args.Set.ReqPtr, LSA_NULL) || (RbuPtr->args.Set.OptCnt == 0))
			&&  (!LSA_HOST_PTR_ARE_EQUAL(RbuPtr->args.Set.RspPtr, LSA_NULL) || (RbuPtr->args.Set.OptCnt == 0)))
			{
				if (RbuPtr->args.Set.OptCnt != 0)
				{
					RespVal = DCP_RSP_OK_ACTIVE;
				}
#ifdef DCP_SET_TRANSACTION
				else if ((RbuPtr->args.Set.TrStart) || (RbuPtr->args.Set.TrEnd))
				{
					RespVal = DCP_RSP_OK_ACTIVE;
				}
#endif
			}
		}
	}

	DCP_FUNCTION_TRACE_03(0, LSA_TRACE_LEVEL_NOTE, "  < dcp_CheckSet() ReqPtr=%X RspPtr=%X RespVal=0x%x",
			RbuPtr->args.Set.ReqPtr, RbuPtr->args.Set.RspPtr, RespVal);

	return(RespVal);
}

/*****************************************************************************/
/* Internal function: dcp_CheckGet()                                         */
/* Check Get Parameters of upper Request block                               */
/*****************************************************************************/
static LSA_UINT16 	DCP_LOCAL_FCT_ATTR dcp_CheckGet(
DCP_CHA_PTR				ChPtr,
DCP_UPPER_RQB_PTR_TYPE	RbuPtr)
{
	LSA_UINT16		RespVal = DCP_RSP_ERR_PARAM;

	DCP_FUNCTION_TRACE_03(0, LSA_TRACE_LEVEL_NOTE, "  > dcp_CheckGet() RbuPtr=%X AddrPtr=%X OptCnt=%d",
			RbuPtr, RbuPtr->args.Get.AddrPtr, RbuPtr->args.Get.OptCnt);

	if (0 == ChPtr->DetailPtr->ClientResources)
	{
		RespVal = DCP_RSP_ERR_CONFIG;
		DCP_ERROR_TRACE_02(ChPtr->TraceIdx, LSA_TRACE_LEVEL_ERROR, "*** dcp_CheckGet() ChPtr=%X RbuPtr=%X: ClientResources=0 in channel details",
						ChPtr, RbuPtr);
	}
	else
	{
		if ((ChPtr->Cancel.Queue.Count != 0)
		&&  (ChPtr->Cancel.Queue.FirstPtr->args.CancelReq.RequestId == RbuPtr->args.Get.RequestId))
		{
			/* Cancel in progress
			*/
			RespVal = DCP_RSP_ERR_SEQUENCE;
		}
		else
		{
			/* Check pointers to destination addresses and buffers
			*/
			if (!LSA_HOST_PTR_ARE_EQUAL(RbuPtr->args.Get.AddrPtr, LSA_NULL)
			&&  !LSA_HOST_PTR_ARE_EQUAL(RbuPtr->args.Get.ReqPtr, LSA_NULL)
			&&  !LSA_HOST_PTR_ARE_EQUAL(RbuPtr->args.Get.RspPtr, LSA_NULL))
			{
				/* Length of Types List
				*/
				if ((RbuPtr->args.Get.OptCnt != 0)
				&&  ((RbuPtr->args.Get.OptCnt * DCP_TYPE_SIZE) <= DCP_MAX_REQ_DATA_SIZE))
				{
					RespVal = DCP_RSP_OK_ACTIVE;
				}
			}
		}
	}

	DCP_FUNCTION_TRACE_03(0, LSA_TRACE_LEVEL_NOTE, "  < dcp_CheckGet() ReqPtr=%X RspPtr=%X RespVal=0x%x",
			RbuPtr->args.Get.ReqPtr, RbuPtr->args.Get.RspPtr, RespVal);

	return(RespVal);
}

/*****************************************************************************/
/* Internal function: dcp_CheckNotify()                                      */
/* Check Notify Parameters of upper Request block                            */
/*****************************************************************************/
static LSA_UINT16 	DCP_LOCAL_FCT_ATTR dcp_CheckNotify(
DCP_CHA_PTR				ChPtr,
DCP_UPPER_RQB_PTR_TYPE	RbuPtr)
{
	LSA_UINT16		RespVal = DCP_RSP_ERR_PARAM;

	DCP_FUNCTION_TRACE_02(0, LSA_TRACE_LEVEL_NOTE, "  > dcp_CheckNotify() RbuPtr =%x NodePtr=%X",
			RbuPtr, RbuPtr->args.Notify.NodePtr);

	if (0 == ChPtr->DetailPtr->ClientResources)
	{
		RespVal = DCP_RSP_ERR_CONFIG;
		DCP_ERROR_TRACE_02(ChPtr->TraceIdx, LSA_TRACE_LEVEL_ERROR, "*** dcp_CheckNotify() ChPtr=%X RbuPtr=%X: ClientResources=0 in channel details",
						ChPtr, RbuPtr);
	}
	else
	{
		/* Check pointer to buffer
		*/
		if (!LSA_HOST_PTR_ARE_EQUAL(RbuPtr->args.Notify.NodePtr, LSA_NULL))
		{
			RespVal = DCP_RSP_OK_ACTIVE;
		}
	}

	DCP_FUNCTION_TRACE_02(0, LSA_TRACE_LEVEL_NOTE, "  < dcp_CheckNotify() RbuPtr=%X RespVal=0x%x",
			RbuPtr, RespVal);

	return(RespVal);
}

/*****************************************************************************/
/* Internal function: dcp_CheckBlocks()                                      */
/* Check DCP Response PDU for complete blocks                                */
/* returns length of block chain if ok, 0 otherwise                          */
/*****************************************************************************/
static LSA_INT 	DCP_LOCAL_FCT_ATTR dcp_CheckBlocks(
	DCP_CHA_PTR				ChPtr,
	LSA_UINT8			Uid,
	DCP_RSP				 *	RspPtr,
	LSA_INT					RspLen)
{
	LSA_UINT8		*	BytePtr;
	LSA_INT				ActLen = 0;
	LSA_INT				BlockLen = RspLen - DCP_HEADER_SIZE;

	DCP_FUNCTION_TRACE_02(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  > dcp_CheckBlocks() ChPtr=%X RspPtr=%X", ChPtr, RspPtr);

	switch (Uid)
	{
	case DCP_IDENTIFY:
		BytePtr = &RspPtr->Data.Identify.IdentDs.Hdr.Opt;
		break;
	case DCP_SET:
		BytePtr = &RspPtr->Data.Set.Result.Hdr.Opt;
		break;
	case DCP_GET:
		BytePtr = &RspPtr->Data.Get.Dataset.Hdr.Opt;
		break;
	default:
		DCP_ERROR_TRACE_02(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "*** dcp_CheckBlocks() ChPtr=%X invalid ServiceId=%d", ChPtr, Uid);
		return (0);
	}

	while (ActLen < BlockLen)
	{
		DCP_DATASET *DsPtr = (DCP_DATASET *)BytePtr;     //lint !e826 Suspicious pointer-to-pointer conversion, on purpose, HM 13.01.2016
		LSA_UINT16  DsLen = DCP_SWAP16(DsPtr->Hdr.Len) + DCP_DS_HDR_SIZE;
		if ((DsLen % 2) != 0)
		{
			DsLen++;
		}
		if ((ActLen + DsLen) > BlockLen)	/* truncated TLV */
		{
			DCP_ERROR_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_WARN, "*** dcp_CheckBlocks() RspLen=%u ActLen+DsLen=%u RestLen=%u: truncated TLV", RspLen, ActLen + DsLen, BlockLen);
			return (0); /* 2787889: dropping an inconsistent frame */
		}
		ActLen += DsLen;
		BytePtr += DsLen;
	}

	DCP_FUNCTION_TRACE_02(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  < dcp_CheckBlocks() ChPtr=%X RetLen=%d", ChPtr, RspLen);

	return (RspLen);
}

/*****************************************************************************/
/* Internal function: dcp_CheckRsp()                                         */
/* Check DCP Response PDU                                                    */
/*****************************************************************************/
LSA_INT 	DCP_LOCAL_FCT_ATTR dcp_CheckRsp(
DCP_CHA_PTR				ChPtr,
DCP_LOWER_RQB_PTR_TYPE	RblPu,
DCP_ETH_FRAME		*	FramePtr,
DCP_RSP				**	RspPtrPtr,
LSA_UINT8			**	AdrPtrPtr)
{
	DCP_LOWER_RXMEM_PTR_TYPE RxMemPtr;
	LSA_UINT8		*	BytePtr;
	DCP_ETH_RECV_PTR	EthFramePtr;
	DCP_RSP		 	*	RspPtr;			/* 050326lrg001 */
	LSA_UINT16			DataLength;
	LSA_UINT16			DataOffset;
	LSA_UINT8 			FrameId0;
	LSA_UINT8 			FrameId1;
	LSA_UINT16			RspLen = 0;
	LSA_INT				RetLen = 0;

	DCP_FUNCTION_TRACE_02(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  > dcp_CheckRsp() ChPtr=%X RbEthPtr=%X",
			ChPtr, RblPu.VoidPtr);

	if (ChPtr->DetailPtr->DevId == DCP_DEV_ETH)
	{
		/* Get data pointer and length from lower RQB
		*/
		EthFramePtr = (DCP_ETH_RECV_PTR)(void*)DCP_ETH_RQB_RECV_PTR_GET(RblPu.RbEthPtr);
		DataLength = DCP_ETH_RQB_RECV_DATA_LEN_GET(RblPu.RbEthPtr);
		DataOffset = DCP_ETH_RQB_RECV_DATA_OFFSET_GET(RblPu.RbEthPtr);
		BytePtr = (LSA_UINT8*)EthFramePtr;
		BytePtr += DataOffset;
		FrameId0 = *BytePtr++;
		FrameId1 = *BytePtr++;
		RspPtr = (DCP_RSP*)(void*)BytePtr;

		/* Check data length and frame id
		*/
		if ((DataLength > DCP_HEADER_SIZE)
		&&  ((DataOffset + DataLength) <= sizeof(DCP_ETH_FRAME))
		&&  (DCP_FRAMEID_0 == FrameId0)
		&&  ((DCP_FRAMEID_MRS == FrameId1) || (DCP_FRAMEID_URR == FrameId1)))
		{
			/* Get length of DCP Response PDU including Header
			*/
			RspLen = DCP_SWAP16(RspPtr->Hdr.Length);
			if (RspLen <= DCP_MAX_RSP_DATA_SIZE)		// 1094328: check RspLen
			{
				RspLen += DCP_HEADER_SIZE;

				/* If the PDU is OK...
				*/
				if ((RspLen <= sizeof(DCP_REQ))
				&&  (RspLen <= DataLength)
				&&  (DCP_ROLE_RSP == (RspPtr->Hdr.ServiceType & DCP_ROLE_MASK)))
				{
					/* Copy Ethernet frame to local Structure
					*/
					RxMemPtr.RxEthPtr = EthFramePtr;
					DCP_COPY_RXETH_TO_LOCAL_MEM(RxMemPtr.RxEthPtr, FramePtr,
							(LSA_UINT16)(DataOffset + DataLength), ChPtr->SysPtr);

					/* Return DataOffset and FrameID
					*/
					BytePtr = (LSA_UINT8*)FramePtr;
					BytePtr += DataOffset + 2;
					*RspPtrPtr = (DCP_RSP*)BytePtr;     //lint !e826 Suspicious pointer-to-pointer conversion, on purpose, HM 13.01.2016
					*AdrPtrPtr = FramePtr->Hdr.SrcMacAdr;
					RetLen = RspLen;
				}
			}
		}
	}

	DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  < dcp_CheckRsp() ChPtr=%X *RspPtrPtr=%X RetLen=%d",
			ChPtr, *RspPtrPtr, RetLen);

	return(RetLen);
}

/*****************************************************************************/
/* Internal function: dcp_ClientRequest()                                    */
/* Switch function for all client services                                   */
/*****************************************************************************/
LSA_UINT16 	DCP_LOCAL_FCT_ATTR dcp_ClientRequest(
DCP_CHA_PTR				ChPtr,
DCP_UPPER_RQB_PTR_TYPE	RbuPtr)
{
	DCP_CLIENT_DATA	*  	DataPtr = LSA_NULL;
	LSA_UINT16		 	RespVal = DCP_RSP_OK_ACTIVE;
	LSA_OPCODE_TYPE		Opcode = DCP_UPPER_RQB_OPCODE_GET(RbuPtr);

	DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  > dcp_ClientRequest() ChPtr=%X RbuPtr=%X Opcode=%d",
			ChPtr, RbuPtr, Opcode);

	/* Move RQB from Upper Channel Queue to service Queue
	 * and start service if Parameters are OK
	*/
	dcp_DequeUpper(RbuPtr);

	switch(Opcode)
	{
	case DCP_OPC_HELLO_REQ:
		RespVal = dcp_CheckHello(ChPtr, RbuPtr);
		if (RespVal == DCP_RSP_OK_ACTIVE)
		{
			DataPtr = &ChPtr->Hello;
			dcp_PutUpperQ(&DataPtr->QueueUpper, RbuPtr);
			RespVal = dcp_ClientHello(ChPtr, RbuPtr, 0);
		}
		break;

	case DCP_OPC_IDENTIFY_REQ:
		RespVal = dcp_CheckIdentify(ChPtr, RbuPtr);
		if (RespVal == DCP_RSP_OK_ACTIVE)
		{
			DataPtr = &ChPtr->Identify;
			dcp_PutUpperQ(&DataPtr->QueueUpper, RbuPtr);
			RespVal = dcp_ClientIdentify(ChPtr, RbuPtr, 0);
		}
		break;

	case DCP_OPC_NOTIFY:
		RespVal = dcp_CheckNotify(ChPtr, RbuPtr);
		if (RespVal == DCP_RSP_OK_ACTIVE)
		{
			/* Only queue Notify RQB for Identify service
			*/
			DataPtr = &ChPtr->Identify;
			dcp_PutUpperQ(&DataPtr->QueueNotify, RbuPtr);
		}
		break;

	case DCP_OPC_SET_REQ:
		RespVal = dcp_CheckSet(ChPtr, RbuPtr);
		if (RespVal == DCP_RSP_OK_ACTIVE)
		{
			DataPtr = &ChPtr->Set;
			dcp_PutUpperQ(&DataPtr->QueueUpper, RbuPtr);
			RespVal = dcp_ClientSet(ChPtr, RbuPtr, 0);
		}
		break;

	case DCP_OPC_GET_REQ:
		RespVal = dcp_CheckGet(ChPtr, RbuPtr);
		if (RespVal == DCP_RSP_OK_ACTIVE)
		{
			DataPtr = &ChPtr->Get;
			dcp_PutUpperQ(&DataPtr->QueueUpper, RbuPtr);
			RespVal = dcp_ClientGet(ChPtr, RbuPtr, 0);
		}
		break;

	case DCP_OPC_CANCEL_REQ:
		dcp_PutUpperQ(&ChPtr->Cancel.Queue, RbuPtr);
		if (ChPtr->Cancel.Queue.Count == 1)
		{
			/* Only the first cancel request will be started here. A queued successor 
			 * will be started at the confirmation of the predecessor in dcp_ClientDone().
			*/
			RespVal = dcp_ClientCancelReq(ChPtr, RbuPtr);
			if (RespVal != DCP_RSP_OK_ACTIVE)
			{
				/* Remove cancel RQB from queue on error or synchronous confirmation
				*/
				dcp_GetUpperQ(&ChPtr->Cancel.Queue, RbuPtr);
			}
		}
		break;

	default:
		RespVal = DCP_RSP_ERR_SYNTAX;
	}

	/* Remove upper RQB from service queue on error
	*/
	if ((RespVal != DCP_RSP_OK_ACTIVE)
	&&  (!LSA_HOST_PTR_ARE_EQUAL(DataPtr, LSA_NULL)))
	{
		dcp_GetUpperQ(&DataPtr->QueueUpper, RbuPtr);
		if (RespVal == DCP_RSP_OK_MORE)
		{
			/* Put RQB to RetryQueue to execute
			 * it at the next ClientDone() of this service
			*/
			RespVal = DCP_RSP_OK_ACTIVE;
			dcp_PutUpperQ(&ChPtr->QueueRetry, RbuPtr);
		}
	}

	DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  < dcp_ClientRequest() ChPtr=%X DataPtr=%X RespVal=0x%x",
			ChPtr, DataPtr, RespVal);

	return(RespVal);
}

/*****************************************************************************/
/* Internal access function: dcp_ClientData()                                */
/* Get pointer to Client Data structure in channel data                      */
/*****************************************************************************/
DCP_CLIENT_DATA	* DCP_LOCAL_FCT_ATTR dcp_ClientData(
DCP_CHA_PTR 			ChPtr,
LSA_UINT8				Uid)
{
	DCP_CLIENT_DATA	*  	DataPtr = LSA_NULL;

	DCP_FUNCTION_TRACE_02(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  > dcp_ClientData() ChPtr=%X Uid=%d",
			ChPtr, Uid);

	switch (Uid)
	{
	case DCP_IDENTIFY:
		DataPtr = &ChPtr->Identify;
		break;
	case DCP_SET:
		DataPtr = &ChPtr->Set;
		break;
	case DCP_GET:
		DataPtr = &ChPtr->Get;
		break;
	default:
		/* Invalid ID ?!?
		040305lrg001: dcp_ErrorInternal(DCP_MODULE_ID, __LINE__, DCP_ERR_UID, LSA_NULL, Uid);
		*/
		break;
	}

	DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  < dcp_ClientData() ChPtr=%X DataPtr=%X Uid=%d",
			ChPtr, DataPtr, Uid);
	return (DataPtr);
}

/*****************************************************************************/
/* Internal function: dcp_ClientDone()                                       */
/* Finish Client service with upper RQB confirmation                         */
/*****************************************************************************/
LSA_VOID 	DCP_LOCAL_FCT_ATTR dcp_ClientDone(
DCP_CHA_PTR 			ChPtr,
DCP_UPPER_RQB_PTR_TYPE	RbuPtr,
LSA_UINT16				Response,
LSA_UINT8				Uid,
LSA_INT					RetryCount)
{
	LSA_UINT16 LSA_FCT_PTR(DCP_LOCAL_FCT_ATTR, RestartPtr)
	(DCP_CHA_PTR ChPtr, DCP_UPPER_RQB_PTR_TYPE RbuPtr, LSA_INT TxRetry);
	DCP_LOWER_RQB_PTR_TYPE	RblPu;
	LSA_UINT16				RespVal;
	LSA_OPCODE_TYPE			Opcode;
	DCP_CLIENT_DATA	*		DataPtr = LSA_NULL;
	LSA_BOOL				TxRetry = LSA_FALSE;

	DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  > dcp_ClientDone() ChPtr=%X RbuPtr=%X Response=0x%x",
			ChPtr, RbuPtr, Response);

	switch (Uid)
	{
	case DCP_IDENTIFY:
		RestartPtr = dcp_ClientIdentify;
		DataPtr = &ChPtr->Identify;
		break;
	case DCP_SET:
		RestartPtr = dcp_ClientSet;
		DataPtr = &ChPtr->Set;
		break;
	case DCP_GET:
		RestartPtr = dcp_ClientGet;
		DataPtr = &ChPtr->Get;
		break;
	case DCP_HELLO:
		RestartPtr = dcp_ClientHello;
		DataPtr = &ChPtr->Hello;
		break;
	default:
		RestartPtr = 0;
		/* Invalid ID ?!?
		*/
		dcp_ErrorInternal(DCP_MODULE_ID, __LINE__, DCP_ERR_UID, LSA_NULL, Uid);
	}

	if (!LSA_HOST_PTR_ARE_EQUAL(DataPtr, LSA_NULL))     //lint !e774 Boolean within 'if' always evaluates to True, keep it for better readability, HM 13.01.2016
	{
		DataPtr->Active = LSA_FALSE;

		if (ChPtr->DetailPtr->AllocOnRequest == DCP_CDB_ALLOC_LOWER_RQBS_AT_REQUEST)
		{
			/* tbd: Free all RQBs of send queue
			*/
			while (DataPtr->QueueSend.Count != 0)
			{
				RblPu = DataPtr->QueueSend.FirstPu;
				dcp_DequeLower(&DataPtr->QueueSend, RblPu, ChPtr);
				dcp_FreeLowerRb(ChPtr, RblPu);
			}
		}
		else
		{
			/* Move all RQBs from service send queue
			 * to common send queue
			*/
			while (DataPtr->QueueSend.Count != 0)
			{
				RblPu = DataPtr->QueueSend.FirstPu;
				dcp_DequeLower(&DataPtr->QueueSend, RblPu, ChPtr);
				dcp_EnqueLower(&ChPtr->QueueLower, RblPu, ChPtr);
			}
		}

		if (RetryCount > 0)
		{
			if (RetryCount <= DCP_CLIENT_RETRY_COUNT)
			{
				/* Execute Get/Set Retry
				*/
				TxRetry = LSA_TRUE;
			}
			else
			{
				/* 1737982: Free saved timer
				*/
				dcp_TimerPut(ChPtr, DataPtr->RetryTimerPtr, &DataPtr->TimerQueue);
			}
		}

		if ((!TxRetry) && (DataPtr->QueueUpper.Count != 0))
		{
			/* Remove Upper RQB from Queue and
			 * confirm it with te right Response code
			*/
			if (LSA_HOST_PTR_ARE_EQUAL(RbuPtr, LSA_NULL))
			{
				RbuPtr = DataPtr->QueueUpper.FirstPtr;
			}
			dcp_GetUpperQ(&DataPtr->QueueUpper, RbuPtr);
			if (Response == DCP_RSP_OK)
			{
				Response = DCP_UPPER_RQB_RESPONSE_GET(RbuPtr);
			}
			if ((DCP_IDENTIFY == Uid) && (Response == DCP_RSP_OK_ACTIVE))
			{
				/* Not too many responses on identify request
				*/
				Response = DCP_RSP_OK;
			}
			/* Finish client service Request
			*/
			dcp_RequestDone(ChPtr, RbuPtr, Response);
		}
		if ((DCP_RSP_OK_CANCEL == Response) && (0 != ChPtr->Cancel.Count))
		{
			/* Decrement current cancel counter
			*/
			ChPtr->Cancel.Count--;
			if ((0 == ChPtr->Cancel.Count) && (0 != ChPtr->Cancel.Queue.Count))
			{
				/* All currently canceled requests are confirmed:
				 * remove from queue and confirm current cancel request
				*/
				RbuPtr = ChPtr->Cancel.Queue.FirstPtr;
				dcp_GetUpperQ(&ChPtr->Cancel.Queue, RbuPtr);
				dcp_RequestDone(ChPtr, RbuPtr, DCP_RSP_OK);
				RespVal = DCP_RSP_OK;
				while ((RespVal != DCP_RSP_OK_ACTIVE) && (0 != ChPtr->Cancel.Queue.Count))
				{
					/* Start queued cancel request
					*/
					RbuPtr = ChPtr->Cancel.Queue.FirstPtr;
					RespVal = dcp_ClientCancelReq(ChPtr, RbuPtr);
					if (RespVal != DCP_RSP_OK_ACTIVE)
					{
						/* Remove cancel RQB from queue and confirm it
						 * on error or synchronous confirmation
						*/
						dcp_GetUpperQ(&ChPtr->Cancel.Queue, RbuPtr);
						dcp_RequestDone(ChPtr, RbuPtr, RespVal);
					}
				}
			}
		}

		/* Start a new Client service if there is a send retry
		 * or the next upper RQB is waiting in the RetryQueue
		*/
		if (!(0 == RestartPtr))
		{
			if (!TxRetry && (ChPtr->QueueRetry.Count != 0))
			{
				/* Move RQB from RetryQueue to UpperQueue
				*/
				RetryCount = 0;
				RbuPtr = ChPtr->QueueRetry.FirstPtr;
				dcp_GetUpperQ(&ChPtr->QueueRetry, RbuPtr);
				Opcode = DCP_UPPER_RQB_OPCODE_GET(RbuPtr);
				switch(Opcode)
				{
				case DCP_OPC_IDENTIFY_REQ:
					RestartPtr = dcp_ClientIdentify;
					DataPtr = &ChPtr->Identify;
					break;

				case DCP_OPC_SET_REQ:
					RestartPtr = dcp_ClientSet;
					DataPtr = &ChPtr->Set;
					break;

				case DCP_OPC_GET_REQ:
					RestartPtr = dcp_ClientGet;
					DataPtr = &ChPtr->Get;
					break;

				default:
					DCP_ERROR_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_WARN, "*** dcp_ClientDone() ChPtr=%X: invalid opcode=%u for retry in RbuPtr=%X",
							ChPtr, Opcode, RbuPtr);
					dcp_RequestDone(ChPtr, RbuPtr, DCP_RSP_ERR_RESOURCE);
					RestartPtr = 0;
					RbuPtr = LSA_NULL;
				}
				if (!(0 == RestartPtr))
				{
					dcp_PutUpperQ(&DataPtr->QueueUpper, RbuPtr);
				}
			}
			else if (!TxRetry)
			{
				RbuPtr = LSA_NULL;
			}

			if ( (!LSA_HOST_PTR_ARE_EQUAL(RbuPtr, LSA_NULL)) && (!(0 == RestartPtr)) )
			{
				/* Restart service
				*/
				RespVal = RestartPtr(ChPtr, RbuPtr, RetryCount);
				if (RespVal != DCP_RSP_OK_ACTIVE)
				{
					/* On error: RQB back to RetryQueue or confirm
					*/
					dcp_GetUpperQ(&DataPtr->QueueUpper, RbuPtr);
					if (RespVal == DCP_RSP_OK_MORE)
					{
						/* Retry at next ClientDone()
						*/
						dcp_PutUpperQ(&ChPtr->QueueRetry, RbuPtr);
					}
					else
					{
						dcp_RequestDone(ChPtr, RbuPtr, RespVal);
					}
				}
			}
		}
	}
	DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  < dcp_ClientDone() ChPtr=%X RetryCount=%d Uid=%d",
			ChPtr, RetryCount, Uid);
}

/*****************************************************************************/
/* Internal access function: dcp_ClientConf()                                */
/* Process lower Send confirmation                                           */
/*****************************************************************************/
LSA_VOID 	DCP_LOCAL_FCT_ATTR dcp_ClientConf(
DCP_CHA_PTR 			ChPtr,
DCP_LOWER_RQB_PTR_TYPE	RblPu,
LSA_UINT16				Response,
LSA_UINT32				Xid,
LSA_UINT8				Uid)
{
	LSA_UINT16				RetVal  = DCP_RSP_OK;
	LSA_UINT16				RespVal = DCP_RSP_OK;
	LSA_UINT8				TimerId = DCP_LOBYTE(DCP_HIWORD((Xid & DCP_CLIENT_XID_MASK)));
	DCP_TIMER_PTR_TYPE		TimerPtr = LSA_NULL;
	DCP_CLIENT_DATA			*DataPtr = LSA_NULL;

	DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  > dcp_ClientConf() ChPtr=%X RbEthPtr=%X Response=0x%x",
			ChPtr, RblPu.VoidPtr, Response);

	switch (Uid)
	{
	case DCP_IDENTIFY:
		DataPtr = &ChPtr->Identify;
		break;
	case DCP_SET:
		DataPtr = &ChPtr->Set;
		break;
	case DCP_GET:
		DataPtr = &ChPtr->Get;
		break;
	case DCP_HELLO:
		DataPtr = &ChPtr->Hello;
		break;
	default:
		/* Invalid ID ?!?
		 * AP01567955 DCP: Warnings bei Generierung des PnDriver entfernen
		*/
		dcp_FreeLowerRb(ChPtr, RblPu);
		dcp_ErrorInternal(DCP_MODULE_ID, __LINE__, DCP_ERR_UID, LSA_NULL, Uid);
	}

	if (ChPtr->WaitSendCnf && (ChPtr->SendReqCount == 0))	//051207lrg001
	{
		/* TFS 2778975: put the timer out of the service queue */
		TimerPtr = dcp_TimerFind(ChPtr, TimerId, &DataPtr->TimerQueue);
		if (!LSA_HOST_PTR_ARE_EQUAL(TimerPtr, LSA_NULL))
		{
			dcp_TimerPut(ChPtr, TimerPtr, &DataPtr->TimerQueue);
		}

		/* All confirmations present: proceed closing channel
		*/
		dcp_FreeLowerRb(ChPtr, RblPu);
		RblPu.VoidPtr = LSA_NULL;
		dcp_CloseLowerFin(ChPtr, RblPu);
	}
	else if (!LSA_HOST_PTR_ARE_EQUAL(DataPtr, LSA_NULL))    //lint !e774 Boolean within 'if' always evaluates to True, keep it for better readability, HM 13.01.2016
	{
		/* Find reseved timer in service queue
		*/
		TimerPtr = dcp_TimerFind(ChPtr, TimerId, &DataPtr->TimerQueue);
		if (LSA_HOST_PTR_ARE_EQUAL(TimerPtr, LSA_NULL))
		{
			DCP_ERROR_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_ERROR, "*** dcp_ClientConf() ChPtr=%X Xid=0x%08x Uid=%u: Timer not found",
					ChPtr, Xid, Uid);
			dcp_ErrorInternal(DCP_MODULE_ID, __LINE__, DCP_ERR_TIMER, ChPtr, 0);
		}
		else if (DCP_HELLO == Uid)
		{
			if (DCP_RSP_OK != Response)
			{
				DCP_ERROR_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_ERROR, "*** dcp_ClientConf() ChPtr=%X Retry=%u Response=0x%x: Hello send failed",
						ChPtr, DataPtr->RetryCount, Response);
			}

			/* Hello.Request sent: decrement send count
			*/
			if (TimerPtr->Tract.Txs.Count > 0)
			{
				TimerPtr->Tract.Txs.Count--;
			}

			TimerPtr->Tract.Txs.Response = DCP_RSP_OK;
			if (TimerPtr->Tract.Txs.Count != 0)
			{
				/* On repetition: put lower RQB to Timer and start timer
				*/
				TimerPtr->RbPu = RblPu;
				if (!LSA_HOST_PTR_ARE_EQUAL(TimerPtr->RbPtr, LSA_NULL)
				&&  (0 != TimerPtr->RbPtr->args.Hello.Delay))
				{
					/* 1061321: load hello interval into timer
					*/
					RetVal = dcp_TimerStart(ChPtr, TimerPtr, 0, &DataPtr->TimerQueue, (LSA_UINT16)TimerPtr->RbPtr->args.Hello.Time, RblPu);
				}
				else
				{
					RetVal = dcp_TimerStart(ChPtr, TimerPtr, 0, &DataPtr->TimerQueue, 0, RblPu);
				}
				if (RetVal != LSA_RET_OK)
				{
					DCP_ERROR_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_ERROR, "*** dcp_ClientConf() ChPtr=%X Xid=0x%08x: Hello Timer start RetVal=0x%x",
							ChPtr, Xid, RetVal);
					dcp_ErrorInternal(DCP_MODULE_ID, __LINE__, DCP_ERR_TIMER, ChPtr, TimerPtr->Id);
				}
			}
			else
			{
				/* HELLO finished: put lower Send RQB to Queue
				*/
				ChPtr->Hello.Active = LSA_FALSE;
				dcp_EnqueLower(&DataPtr->QueueSend, RblPu, ChPtr);

			}
			if (!LSA_HOST_PTR_ARE_EQUAL(TimerPtr->RbPtr, LSA_NULL))
			{
				/* Confirm upper HELLO request on first lower send confirmation
				*/
				dcp_RequestDone(ChPtr, TimerPtr->RbPtr, Response);
				TimerPtr->RbPtr = LSA_NULL;
			}
			if (DataPtr->QueueUpper.Count > 0)
			{
				/* Confirm HELLO Stop RQB
				*/
				DCP_UPPER_RQB_PTR_TYPE RbuPtr = DataPtr->QueueUpper.FirstPtr;
				dcp_GetUpperQ(&DataPtr->QueueUpper, RbuPtr);
				dcp_RequestDone(ChPtr, RbuPtr, DCP_RSP_OK);
				RespVal = DCP_RSP_OK_ACTIVE;
			}
			if (!ChPtr->Hello.Active)
			{
				/* HELLO finished: free reserved Timer
				*/
				dcp_TimerPut(ChPtr, TimerPtr, &DataPtr->TimerQueue);
			}
		}
		else if ((DCP_RSP_OK == Response)
			  &&  (DCP_RSP_OK_ACTIVE == TimerPtr->Tract.Txs.Response))
		{
			/* Request sent successfully
			 * 210306lrg001: and Get-/Set Response not received yet
			*/
			TimerPtr->Tract.Txs.Response = DCP_RSP_OK;

			/* Put Send Request block for Cancel to Timer and start timer
			 * 020404lrg001: not if AllocOnRequest
			*/
			RetVal = dcp_TimerStart(ChPtr, TimerPtr, 0, &DataPtr->TimerQueue, 0, RblPu);
			if (RespVal != DCP_RSP_OK)      //lint !e774 Boolean within 'if' always evaluates to False, keep it for better readability, HM 13.01.2016
			{
				DCP_ERROR_TRACE_04(ChPtr->TraceIdx, LSA_TRACE_LEVEL_ERROR, "*** dcp_ClientConf() ChPtr=%X Xid=0x%08x Uid=%u: Timer start RespVal=0x%x",
						ChPtr, Xid, Uid, RespVal);
				dcp_ErrorInternal(DCP_MODULE_ID, __LINE__, DCP_ERR_TIMER, ChPtr, TimerPtr->Id);
			}
		}
		else
		{
			RetVal = DCP_RSP_ERR_LOWER_LAYER;
			if (((DCP_RSP_OK != Response) && (DCP_RSP_OK_CANCEL != Response))
			||  (DCP_RSP_ERR_LOWER_LAYER == TimerPtr->Tract.Txs.Response)
			||  (DCP_RSP_OK_CANCEL == TimerPtr->Tract.Txs.Response))
			{
				/* Request send failed or already a RECV indicated with error:
				 * 300404lrg001: always cancel Receive(s)
				 * 101013lrg001: DCP_RSP_OK_CANCEL is set in dcp_ClientCancelTimer()
				*/
				if (DCP_RSP_OK_CANCEL == TimerPtr->Tract.Txs.Response)
				{
					RetVal = DCP_RSP_OK_CANCEL;
				}
				else
				{
					TimerPtr->Tract.Txs.Response = DCP_RSP_ERR_LOWER_LAYER;
				}
				RespVal = dcp_CancelLower(ChPtr, LSA_FALSE, TimerPtr, RblPu);
			}
			if (RespVal != DCP_RSP_OK_ACTIVE)
			{
				/* On Cancel error or 210306lrg001: Free reserved Timer and free or queue lower RQB
				*/
				dcp_TimerPut(ChPtr, TimerPtr, &DataPtr->TimerQueue);
				if (ChPtr->DetailPtr->AllocOnRequest == DCP_CDB_ALLOC_LOWER_RQBS_AT_REQUEST)
					dcp_FreeLowerRb(ChPtr, RblPu);
				else
				{
					dcp_EnqueLower(&DataPtr->QueueSend, RblPu, ChPtr);
				}

				if ((DCP_RSP_OK != Response) && (DCP_RSP_OK_CANCEL != Response))
				{
					/* Finish Client service now with error if Cancel error
					 * else at Cancel Confirmation
					*/
					dcp_ClientDone(ChPtr, TimerPtr->RbPtr, RetVal, Uid, 0);
				}
			}
		}
	}
	DCP_FUNCTION_TRACE_05(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  < dcp_ClientConf() ChPtr=%X Xid=0x%08x Uid=0x%x RespVal=0x%x RetVal=0x%x",
			ChPtr, Xid, Uid, RespVal, RetVal);
}

/*****************************************************************************/
/* Internal function: dcp_IdentifyInd()                                      */
/* Process IDENTIFY-Response                                                 */
/*****************************************************************************/
static LSA_VOID 	DCP_LOCAL_FCT_ATTR dcp_IdentifyInd(
DCP_CHA_PTR 			ChPtr,
const LSA_UINT8		*	AdrPtr,
DCP_RSP				*	RspPtr,
LSA_INT					RspLen,
DCP_TIMER_PTR_TYPE		TimerPtr)
{
	DCP_DATASET		*		DsPtr;
	LSA_UINT16				DsLen;
	LSA_UINT8		*		BytePtr;
	LSA_INT					TlvCnt = 0;
	LSA_INT					ActLen = 0;
	DCP_UPPER_RQB_PTR_TYPE	RbuPtr = LSA_NULL;
	DCP_USR_NODE_PTR		NodePtr = LSA_NULL;
	LSA_UINT16				RestLen = (LSA_UINT16)(RspLen - DCP_HEADER_SIZE);	//040811lrg: cast

	DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  > dcp_IdentifyInd() ChPtr=%X TimerId=%u RspLen=%d",
			ChPtr, TimerPtr->Id, RspLen);

	if (ChPtr->Identify.QueueUpper.Count != 0)
	{
		/* If at least 1 upper RQB is waiting for callback:
		 * get Timer index from Transaktion ID of the Response,
		 * to find the appropriate upper Requestblock.
		*/
		RbuPtr = TimerPtr->Tract.Txs.RbuPtr;
		if (!LSA_HOST_PTR_ARE_EQUAL(RbuPtr, LSA_NULL))
		{
			/* Calculate Node TlvCnt
			*/
			BytePtr = &RspPtr->Data.Identify.IdentDs.Hdr.Opt;

			while (ActLen < RestLen)
			{
				TlvCnt++;
				DsPtr = (DCP_DATASET *)BytePtr;     //lint !e826 Suspicious pointer-to-pointer conversion, on purpose, HM 13.01.2016
				DsLen =  DCP_SWAP16(DsPtr->Hdr.Len) + DCP_DS_HDR_SIZE;
				if ((DsLen %2) != 0)
				{
					DsLen ++;
				}
				ActLen += DsLen;
				BytePtr += DsLen;
			}
			BytePtr = &RspPtr->Data.Identify.IdentDs.Hdr.Opt;

			if (!LSA_HOST_PTR_ARE_EQUAL(RbuPtr->args.Identify.NodePtr, LSA_NULL))
			{
				NodePtr = RbuPtr->args.Identify.NodePtr + RbuPtr->args.Identify.NodeCnt;
				if (RbuPtr->args.Identify.NodeCnt < TimerPtr->Tract.Txs.Count)
				{
					/* Space available in user buffer
					*/
					RbuPtr->args.Identify.NodeCnt++;
					TimerPtr->Tract.Txs.Idx++;
				}
				else
				{
					/* Buffer full: signal overflow in Response code
					*/
					DCP_UPPER_RQB_RESPONSE_SET(RbuPtr, DCP_RSP_OK_MORE);
					NodePtr = LSA_NULL;
				}
			}

			/* If the user has provided a buffer
			 * and therein is space...
			*/
			if (!LSA_HOST_PTR_ARE_EQUAL(NodePtr, LSA_NULL))
			{
				/* Put source address to Identify Node structure
				*/
				DCP_DS_SET_ADDR(NodePtr->NodeAddr, AdrPtr, DCP_MAC_ADR_SIZE);

				/* Copy TLVs from Response-PDU to user buffer
				 * and set count in the Node Structure
				*/
				DCP_COPY_LOCAL_TO_UPPER_MEM(BytePtr, NodePtr->TlvChain,
						RestLen, ChPtr->SysPtr);
				NodePtr->TlvCnt = TlvCnt;
			}
			/* If desired and present: indicate Notify RQB
			*/
			if ((RbuPtr->args.Identify.Notify == DCP_IDENTIFY_NOTIFY_USER_ON_NEW_ENTRY)
			&&  (ChPtr->Identify.QueueNotify.Count != 0))
			{
				LSA_UINT32 NotifyId = RbuPtr->args.Identify.NotifyId;
				RbuPtr = ChPtr->Identify.QueueNotify.FirstPtr;
				dcp_GetUpperQ(&ChPtr->Identify.QueueNotify, RbuPtr);
				NodePtr = RbuPtr->args.Notify.NodePtr;

				/* Return count of waiting Notify RQBs
				 * 070307lrg001: return NotifyId from Identify RQB
				*/
				RbuPtr->args.Notify.NotifyCnt = (LSA_INT)(ChPtr->Identify.QueueNotify.Count);
				RbuPtr->args.Notify.NotifyId = NotifyId;

				/* Put source address to Notify Node structure
				*/
				DCP_DS_SET_ADDR(NodePtr->NodeAddr, AdrPtr, DCP_MAC_ADR_SIZE);

				/* Copy TLVs from Response-PDU to user buffer
				 * and set count in the Node Structure
				*/
				DCP_COPY_LOCAL_TO_UPPER_MEM(BytePtr, NodePtr->TlvChain,
						RestLen, ChPtr->SysPtr);
				NodePtr->TlvCnt = TlvCnt;

				/* Indicate Notify RQB
				*/
				dcp_RequestDone(ChPtr, RbuPtr, DCP_RSP_OK);
			}
		}
	}
	DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  < dcp_IdentifyInd() RbuPtr=%X NodePtr=%X TlvCnt=%d",
			RbuPtr, NodePtr, TlvCnt);
}

/*****************************************************************************/
/* Internal function: dcp_SetInd()                                           */
/* Process SET-Response                                                      */
/*****************************************************************************/
static LSA_VOID 	DCP_LOCAL_FCT_ATTR dcp_SetInd(
DCP_CHA_PTR 			ChPtr,
const LSA_UINT8		*	AdrPtr,
DCP_RSP				*	RspPtr,
LSA_INT					RspLen,
DCP_TIMER_PTR_TYPE		TimerPtr)
{
	DCP_UPPER_RQB_PTR_TYPE	RbuPtr;
	LSA_UINT16				DsLen;
	DCP_DATASET		*		DsPtr;
	LSA_UINT8		*		BytePtr;
	LSA_UINT16				Response = DCP_RSP_OK;
	LSA_INT					ActLen = 0;
	LSA_INT					CopyLen = 0;
	LSA_INT					RestLen = RspLen - DCP_HEADER_SIZE;

	DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  > dcp_SetInd() ChPtr=%X TimerId=%u RspLen=%d",
			ChPtr, TimerPtr->Id, RspLen);

	/* 210306lrg001: if send confirmation has already arrived...
	*/
	if (TimerPtr->Tract.Txs.Response == DCP_RSP_OK)
	{
		/* Stop Response Timeout
		*/
		dcp_TimerStop(ChPtr, TimerPtr, 0, LSA_NULL);
	}
	TimerPtr->Tract.Txs.Response = DCP_RSP_OK_MORE;
	RbuPtr = TimerPtr->Tract.Txs.RbuPtr;
	TimerPtr->Tract.Txs.RbuPtr = LSA_NULL;
	TimerPtr->RbPtr = LSA_NULL;

	if (!LSA_HOST_PTR_ARE_EQUAL(TimerPtr->RbPu.VoidPtr, LSA_NULL))
	{
		/* Free Send-/Cancel RQB or put back to Send queue (020404lrg001)
		*/
		if (ChPtr->DetailPtr->AllocOnRequest == DCP_CDB_ALLOC_LOWER_RQBS_AT_OPEN_CHANNEL)
		{
			dcp_EnqueLower(&ChPtr->Set.QueueSend, TimerPtr->RbPu, ChPtr);
		}
		else
		{
			dcp_FreeLowerRb(ChPtr, TimerPtr->RbPu);
		}
		TimerPtr->RbPu.VoidPtr = LSA_NULL;
	}

	if (!LSA_HOST_PTR_ARE_EQUAL(RbuPtr, LSA_NULL))
	{
		/* Check source address of Response PDU
		*/
		if (!DCP_CMP_MAC_ADDR(AdrPtr, RbuPtr->args.Set.AddrPtr))
		{
			Response = DCP_RSP_ERR_PROTOCOL;
		}
		if (DCP_RSP_OK == Response)
		{
			/* Service not supported by the remote station
			*/
			if (DCP_RESULT_NO_SERVICE == (RspPtr->Hdr.ServiceType & DCP_RESULT_NO_SERVICE))
			{
				Response = DCP_RSP_ERR_NO_SERVICE;
			}
			else
			{
				/* Check minimum length
				*/
				if (RspLen < (DCP_HEADER_SIZE + DCP_DS_RESULT_MIN_SIZE))
				{
					Response = DCP_RSP_ERR_PROTOCOL;
				}
				else
				{
					RbuPtr->args.Set.OptCnt = 0;
					BytePtr = &RspPtr->Data.Set.Result.Hdr.Opt;

					while ((DCP_RSP_OK == Response) && (ActLen < RestLen))
					{
#ifdef DCP_SET_TRANSACTION
						LSA_UINT16				ReqType;
#endif
						DsPtr = (DCP_DATASET *)BytePtr;     //lint !e826 Suspicious pointer-to-pointer conversion, on purpose, HM 13.01.2016
						DsLen =  DCP_SWAP16(DsPtr->Hdr.Len) + DCP_DS_HDR_SIZE;
						if ((DsLen %2) != 0)
						{
							DsLen ++;
						}
#ifdef DCP_SET_TRANSACTION
						ReqType = DCP_SWAP16(DsPtr->Status);
						if (DCP_TYPE_CTRL_START == ReqType)
						{
							if (RbuPtr->args.Set.TrStart)
							{
								/* Check for server understood Transaktion start
								 * and if not: set return value to LSA_FALSE
								*/
								if (DsPtr->Data.Result.Code != DCP_RESULT_OK)
									RbuPtr->args.Set.TrStart = LSA_FALSE;
							}
							else
							{
								Response = DCP_RSP_ERR_PROTOCOL;
							}
						}
						else if (DCP_TYPE_CTRL_END == ReqType)
						{
							if (RbuPtr->args.Set.TrEnd)
							{
								/* Check for server understood Transaktion end
								 * and if not: set return value to LSA_FALSE
								*/
								if (DsPtr->Data.Result.Code != DCP_RESULT_OK)
									RbuPtr->args.Set.TrEnd = LSA_FALSE;
							}
							else
							{
								Response = DCP_RSP_ERR_PROTOCOL;
							}
						}
						else 
#endif
						if (!LSA_HOST_PTR_ARE_EQUAL(RbuPtr->args.Set.RspPtr, LSA_NULL))
						{
							if ((ActLen + DsLen) > RestLen)	// 1094328: truncated TLV: copy only til frame end
							{
								DsLen = (LSA_UINT16)(RestLen - ActLen);
							}
							/* Copy Response TLV to user buffer
							 * and increment OptCnt
							*/
							DCP_COPY_LOCAL_TO_UPPER_MEM(BytePtr, RbuPtr->args.Set.RspPtr + CopyLen,
									DsLen, ChPtr->SysPtr);
							CopyLen += DsLen;
							RbuPtr->args.Set.OptCnt++;
						}
						else
						{
							Response = DCP_RSP_ERR_PROTOCOL;
						}
						ActLen += DsLen;
						BytePtr += DsLen;
					}
				}
			}
		}

		/* Finish Client service
		*/
		DCP_UPPER_RQB_RESPONSE_SET(RbuPtr, Response);
		dcp_ClientDone(ChPtr, RbuPtr, Response, DCP_SET, 0);
	}

	DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  < dcp_SetInd() ChPtr=%X RbuPtr=%X Response=0x%x",
			ChPtr, RbuPtr, Response);
}

/*****************************************************************************/
/* Internal function: dcp_GetInd()                                           */
/* Process GET-Response                                                      */
/*****************************************************************************/
static LSA_VOID 	DCP_LOCAL_FCT_ATTR dcp_GetInd(
DCP_CHA_PTR 			ChPtr,
const LSA_UINT8		*	AdrPtr,
DCP_RSP				*	RspPtr,
LSA_INT					RspLen,
DCP_TIMER_PTR_TYPE		TimerPtr)
{

	DCP_UPPER_RQB_PTR_TYPE	RbuPtr;
	LSA_UINT16				DsLen;
	DCP_DATASET		*		DsPtr;
	LSA_UINT8		*		BytePtr;
	LSA_INT					ActLen = 0;
	LSA_INT					RestLen = RspLen - DCP_HEADER_SIZE;
	LSA_UINT16				Response = DCP_RSP_OK;

	DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  > dcp_GetInd() ChPtr=%X Timerid=%u RspLen=%d",
			ChPtr, TimerPtr->Id, RspLen);

	/* 210306lrg001: if send confirmation has already arrived...
	*/
	if (TimerPtr->Tract.Txs.Response == DCP_RSP_OK)
	{
		/* Stop Response Timeout
		*/
		dcp_TimerStop(ChPtr, TimerPtr, 0, LSA_NULL);
	}
	TimerPtr->Tract.Txs.Response = DCP_RSP_OK_MORE;
	RbuPtr = TimerPtr->Tract.Txs.RbuPtr;
	TimerPtr->Tract.Txs.RbuPtr = LSA_NULL;
	TimerPtr->RbPtr = LSA_NULL;

	if (!LSA_HOST_PTR_ARE_EQUAL(TimerPtr->RbPu.VoidPtr, LSA_NULL))
	{
		/* Free Send-/Cancel RQB or put back to Send queue (020404lrg001)
		*/
		if (ChPtr->DetailPtr->AllocOnRequest == DCP_CDB_ALLOC_LOWER_RQBS_AT_OPEN_CHANNEL)
		{
			dcp_EnqueLower(&ChPtr->Get.QueueSend, TimerPtr->RbPu, ChPtr);
		}
		else
		{
			dcp_FreeLowerRb(ChPtr, TimerPtr->RbPu);
		}
		TimerPtr->RbPu.VoidPtr = LSA_NULL;
	}

	if (!LSA_HOST_PTR_ARE_EQUAL(RbuPtr, LSA_NULL))
	{
		/* Check source address of Response PDU
		*/
		if (!DCP_CMP_MAC_ADDR(AdrPtr, RbuPtr->args.Get.AddrPtr))
		{
			Response = DCP_RSP_ERR_PROTOCOL;
		}
		if (DCP_RSP_OK == Response)
		{
			/* Service not supported by the remote station
			*/
			if (DCP_RESULT_NO_SERVICE == (RspPtr->Hdr.ServiceType & DCP_RESULT_NO_SERVICE))
			{
				Response = DCP_RSP_ERR_NO_SERVICE;
			}
			else
			{
				/* Check length of Response PDU
				 */
				if ((RestLen < DCP_DS_HDR_SIZE)
				||  (DCP_MAX_RSP_DATA_SIZE < RestLen))
				{
					Response = DCP_RSP_ERR_PROTOCOL;
				}
				else
				{
					/* Calculate OptCnt to Return
					*/
					RbuPtr->args.Get.OptCnt = 0;
					BytePtr = &RspPtr->Data.Get.Dataset.Hdr.Opt;

					while (ActLen < RestLen)
					{
						RbuPtr->args.Get.OptCnt++;
						DsPtr = (DCP_DATASET *)BytePtr;      //lint !e826 Suspicious pointer-to-pointer conversion, on purpose, HM 13.01.2016
						DsLen =  DCP_SWAP16(DsPtr->Hdr.Len) + DCP_DS_HDR_SIZE;
						if ((DsLen %2) != 0)
						{
							DsLen ++;
						}
						ActLen += DsLen;
						BytePtr += DsLen;
					}
					/* Copy Response TLV to user buffer
					*/
					DCP_COPY_LOCAL_TO_UPPER_MEM(&RspPtr->Data.Get.Dataset.Hdr.Opt,
												RbuPtr->args.Get.RspPtr,
												(LSA_UINT16)RestLen, ChPtr->SysPtr);
				}
			}
		}

		/* Finish Client service
		*/
		DCP_UPPER_RQB_RESPONSE_SET(RbuPtr, Response);
		dcp_ClientDone(ChPtr, RbuPtr, Response, DCP_GET, 0);
	}

	DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  < dcp_GetInd() ChPtr=%X RbuPtr=%X Response=0x%x",
			ChPtr, RbuPtr, Response);
}

/*****************************************************************************/
/* Internal access function: dcp_ClientInd()                                 */
/* Process lower Receive Indication (== Server Response)                     */
/*****************************************************************************/
LSA_VOID 	DCP_LOCAL_FCT_ATTR dcp_ClientInd(
DCP_CHA_PTR 			ChPtr,
DCP_LOWER_RQB_PTR_TYPE  RblPu,
LSA_UINT16				Response,
LSA_UINT8				Uid,
LSA_UINT32				Xidx,
LSA_UINT32				RequestCount)
{
#ifndef DCP_REQ_LOCAL_DATA
	DCP_ETH_FRAME			dcp_ClientFrame;
#endif
	LSA_UINT8		*		AdrPtr;
	DCP_RSP			*		RspPtr;
	LSA_INT					RspLen;
	LSA_UINT32				Xid = Xidx & DCP_CLIENT_XID_MASK;
	LSA_UINT8				TimerId = DCP_LOBYTE(DCP_HIWORD(Xid));
	LSA_UINT16				Xidw = 	DCP_LOWORD(Xid);
	LSA_UINT16				RespVal  = DCP_RSP_OK;
	DCP_CLIENT_DATA	*		DataPtr  = LSA_NULL;
	LSA_UINT8				ServiceId = DCP_NOSERVICE;
	LSA_BOOL				RspInvalid = LSA_FALSE;
	DCP_TIMER_PTR_TYPE		TimerPtr = LSA_NULL;
	LSA_UINT8				CallId = 0;					// 1006228: check timer CallId

	DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  > dcp_ClientInd() ChPtr=%X RbEthPtr=%X Response=0x%x",
						ChPtr, RblPu.VoidPtr, Response);

	/* 130105lrg001: decrement Client Recv-RQB count
	*/
	ChPtr->ChLowPtr->RecvRblClient--;

	if (DCP_RSP_OK_CANCEL != Response)
	{
		switch (Uid)
		{
		case DCP_IDENTIFY:
			CallId = DCP_TIMER_IDENTREQ;
			DataPtr = &ChPtr->Identify;
			break;
		case DCP_SET:
			CallId = DCP_TIMER_SETREQ;
			DataPtr = &ChPtr->Set;
			break;
		case DCP_GET:
			CallId = DCP_TIMER_GETREQ;
			DataPtr = &ChPtr->Get;
			break;
		default:
			RspInvalid = LSA_TRUE;
			break;
		}
		
		if (!LSA_HOST_PTR_ARE_EQUAL(DataPtr, LSA_NULL))
		{
			if (DCP_RSP_OK != Response)
			{
				/* Lower Error in Response code: Xid == TimerId
				*/
				TimerId = (LSA_UINT8)Xid;
			}
			/* Find timer in service queue
			*/
			TimerPtr = dcp_TimerFind(ChPtr, TimerId, &DataPtr->TimerQueue);
			if (!LSA_HOST_PTR_ARE_EQUAL(TimerPtr, LSA_NULL))
			{
				if (DCP_RSP_OK_CANCEL == TimerPtr->Tract.Txs.Response)
				{
					/* 1533871: Recv should have been cancelled
					*/ 
					Response = DCP_RSP_OK_CANCEL;
				}
			}
			else
			{
				DCP_ERROR_TRACE_04(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "*** dcp_ClientInd() ChPtr=%X Xid=0x%08x Uid=%u: TimerId=%u not found",
						ChPtr, Xidx, Uid, TimerId);
			}
		}
		else
		{
			DCP_ERROR_TRACE_02(ChPtr->TraceIdx, LSA_TRACE_LEVEL_WARN, "*** dcp_ClientInd() ChPtr=%X invalid ServiceId=%d",
					ChPtr, Uid);
		}
	}
	if (DCP_RSP_OK_CANCEL == Response)
	{
		/* Lower Requestblock has been canceled because of Timeout
		 * 171204lrg001: if the EDD has fewer RQBs in the Queue
		 * than the DCP Server Indications: reprovide RQB
		 * 130105lrg001: undo Client Recv-RQB count dekrement
		 * 110507lrg001: Sum of service queues instead of QueueInd.Count
		 * 141107lrg002:
		*/
		if (!LSA_HOST_PTR_ARE_EQUAL(ChPtr->ChLowPtr->ChServerPtr, LSA_NULL)
		&&  (RequestCount < (ChPtr->ChLowPtr->ChServerPtr->QueueSet.Count
							+ChPtr->ChLowPtr->ChServerPtr->QueueGet.Count
							+ChPtr->ChLowPtr->ChServerPtr->QueueIdentify.Count)))
		{
			ChPtr->ChLowPtr->RecvRblClient++;
			dcp_DataRecv(ChPtr, RblPu, 0, 0);
		}
		else
		{
			/* Free or queue RQB
			*/
			if (ChPtr->DetailPtr->AllocOnRequest == DCP_CDB_ALLOC_LOWER_RQBS_AT_REQUEST)
			{
				dcp_FreeLowerRb(ChPtr, RblPu);
			}
			else
			{
				/* queue RQB in the common lower receive Queue
				*/
				dcp_EnqueLower(&ChPtr->ChLowPtr->QueueRecv, RblPu, ChPtr);
			}
		}
	}
	else
	{
		if (DCP_RSP_OK == Response)
		{
			RspInvalid = LSA_TRUE;
			if (!LSA_HOST_PTR_ARE_EQUAL(TimerPtr, LSA_NULL))
			{
				/* If an upper RQB is waiting and Transaktion ID is valid...
				 * ...1006228: and CallId matches
				*/
				if (!LSA_HOST_PTR_ARE_EQUAL(TimerPtr->Tract.Txs.RbuPtr, LSA_NULL)
				&&  (CallId == TimerPtr->CallId)
				&&  (Xidw == TimerPtr->Tract.Txs.Xid))
				{
					/* Check and copy DCP Response PDU
					*/
					RspLen = dcp_CheckRsp(ChPtr, RblPu, &dcp_ClientFrame, &RspPtr, &AdrPtr);
					if (0 < RspLen)
					{
						RspLen = dcp_CheckBlocks(ChPtr, Uid, RspPtr, RspLen); /* 2787889, 2787898: check for complete frames */
					}
					if (0 < RspLen)
					{
						/* Start service indication
						*/
						RspInvalid = LSA_FALSE;
						ServiceId = Uid;
						if (   (ChPtr->DetailPtr->AllocOnRequest == DCP_CDB_ALLOC_LOWER_RQBS_AT_OPEN_CHANNEL)
						    && ((Uid == DCP_GET) || (Uid == DCP_SET)))
						{
							/* 260404lrg001: on GET und SET: queue RQB
							*/
							dcp_EnqueLower(&ChPtr->ChLowPtr->QueueRecv, RblPu, ChPtr);
							RblPu.VoidPtr = LSA_NULL;
						}
						switch (Uid)
						{
						case DCP_IDENTIFY:
							dcp_IdentifyInd(ChPtr, AdrPtr, RspPtr, RspLen, TimerPtr);
							break;
						case DCP_SET:
							dcp_SetInd(ChPtr, AdrPtr, RspPtr, RspLen, TimerPtr);
							break;
						case DCP_GET:
							dcp_GetInd(ChPtr, AdrPtr, RspPtr, RspLen, TimerPtr);
							break;
						default:
							/* Trace and ignore unknown service Response
							*/
							RspInvalid = LSA_TRUE;
							DCP_ERROR_TRACE_02(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "*** dcp_ClientInd() ChPtr=%X invalid ServiceId=%d",
									ChPtr, Uid);
						}
					}
				}
			}
		}
		else
		{
			/* Lower Error in Response code: Xid == TimerId
			*/
			if (!LSA_HOST_PTR_ARE_EQUAL(DataPtr, LSA_NULL))
			{
				if (!LSA_HOST_PTR_ARE_EQUAL(TimerPtr, LSA_NULL))
				{
					/* if send confirmation has already arrived...
					*/
					TimerPtr->Tract.Txs.Response = DCP_RSP_ERR_LOWER_LAYER;
					if (!LSA_HOST_PTR_ARE_EQUAL(TimerPtr->RbPu.VoidPtr, LSA_NULL))
					{
						/* ...stop Timer and cancel Receive request(s)
						*/
						dcp_TimerStop(ChPtr, TimerPtr, 0, &DataPtr->TimerQueue);
						RespVal = dcp_CancelLower(ChPtr, LSA_FALSE, TimerPtr, TimerPtr->RbPu);
						if (RespVal != DCP_RSP_OK_ACTIVE)
						{
							/* Free or queue Timer Cancel RQB
							*/
							if (ChPtr->DetailPtr->AllocOnRequest == DCP_CDB_ALLOC_LOWER_RQBS_AT_REQUEST)
							{
								dcp_FreeLowerRb(ChPtr, TimerPtr->RbPu);
							}
							else
							{
								dcp_EnqueLower(&DataPtr->QueueSend, TimerPtr->RbPu, ChPtr);
							}
							TimerPtr->RbPu.VoidPtr = LSA_NULL;
							dcp_TimerPut(ChPtr, TimerPtr, &DataPtr->TimerQueue);
	
							/* Finish Client service with error
							*/
							if (!LSA_HOST_PTR_ARE_EQUAL(TimerPtr->RbPtr, LSA_NULL))
							{
								dcp_ClientDone(ChPtr, TimerPtr->RbPtr, DCP_RSP_ERR_LOWER_LAYER, Uid, 0);
							}
						}
						else
						{
							TimerPtr->RbPu.VoidPtr = LSA_NULL;
						}
					}
				}
			}
			else
			{
				dcp_FreeLowerRb(ChPtr, RblPu);
				RblPu.VoidPtr = LSA_NULL;
			}
		}

		if (!LSA_HOST_PTR_ARE_EQUAL(RblPu.VoidPtr, LSA_NULL))
		{
			/* Reprovide Lower Recv Requestblock on invalid Response or IDENTIFY
			 * else free or queue it
			 */
			if (RspInvalid || (DCP_IDENTIFY == ServiceId))
			{
				if (Uid == 0)	// 120913lrg740218
				{
					ChPtr->ChLowPtr->RecvRblClient++;
				}
				dcp_DataRecv(ChPtr, RblPu, TimerId, Uid);
			}
			else if (   (ChPtr->DetailPtr->AllocOnRequest  == DCP_CDB_ALLOC_LOWER_RQBS_AT_REQUEST)
				     || LSA_HOST_PTR_ARE_EQUAL(DataPtr, LSA_NULL))
			{
				dcp_FreeLowerRb(ChPtr, RblPu);
			}
			else
			{
				dcp_EnqueLower(&ChPtr->ChLowPtr->QueueRecv, RblPu, ChPtr);
			}
		}
	}

	DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  < dcp_ClientInd() ChPtr=%X Xidx=0x%08x RequestCount=%d",
						ChPtr, Xidx, RequestCount);
}

/*****************************************************************************/
/* Internal access function: dcp_ClientCancel()                              */
/* Process Client Cancel Confirmation                                        */
/*****************************************************************************/
LSA_VOID 	DCP_LOCAL_FCT_ATTR dcp_ClientCancel(
DCP_CHA_PTR 			ChPtr,
DCP_LOWER_RQB_PTR_TYPE	RblPu,
LSA_UINT16				Response,
LSA_UINT8				Uid,
LSA_UINT8				TimerId)
{
	LSA_INT					RetryCount;
	DCP_CLIENT_DATA		*	DataPtr;
	DCP_UPPER_RQB_PTR_TYPE	RbuPtr = LSA_NULL;
	DCP_TIMER_PTR_TYPE		TimerPtr = LSA_NULL;

	DCP_IS_VALID_PTR(ChPtr);
	DCP_FUNCTION_TRACE_05(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  > dcp_ClientCancel() ChPtr=%X RbEthPtr=%X Response=0x%x Uid=%u TimerId=%u",
			ChPtr, RblPu.VoidPtr, Response, Uid, TimerId);

	DataPtr = dcp_ClientData(ChPtr, Uid);
	if (!LSA_HOST_PTR_ARE_EQUAL(DataPtr, LSA_NULL))
	{
		/* Find reseved timer in service queue
		*/
		TimerPtr = dcp_TimerFind(ChPtr, TimerId, &DataPtr->TimerQueue);
		if (LSA_HOST_PTR_ARE_EQUAL(TimerPtr, LSA_NULL))
		{
			DCP_ERROR_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_ERROR, "*** dcp_ClientCancel() ChPtr=%X Uid=%u: TimerId=%u not found",
					ChPtr, Uid, TimerId);
			dcp_ErrorInternal(DCP_MODULE_ID, __LINE__, DCP_ERR_TIMER, ChPtr, 0);
		}
		else
		{
			/* put send buffer into RQB parameter block again
			 * and set service to SEND for free RQB.
			*/
			DCP_ETH_RQB_SEND_DATA_SET(RblPu.RbEthPtr, TimerPtr->Tract.Txs.TxPu.TxEthPtr, DCP_ETH_MEM_MAX_LEN);
			DCP_ETH_RQB_SERVICE_SET(RblPu.RbEthPtr, DCP_ETH_SEND);

			/* Free or queue Cancel Requestblock in Sendqueue
			*/
			if (ChPtr->DetailPtr->AllocOnRequest == DCP_CDB_ALLOC_LOWER_RQBS_AT_OPEN_CHANNEL)
			{
				dcp_EnqueLower(&DataPtr->QueueSend, RblPu, ChPtr);
			}
			else
			{
				dcp_FreeLowerRb(ChPtr, RblPu);
			}

			RbuPtr = TimerPtr->Tract.Txs.RbuPtr;
			Response = TimerPtr->Tract.Txs.Response;
			if ((DCP_IDENTIFY == Uid) || (DCP_RSP_OK_CANCEL == Response)
			||  LSA_HOST_PTR_ARE_EQUAL(RbuPtr, LSA_NULL))
			{
				/* Free Transaction structure in timer block
				*/
				dcp_TimerPut(ChPtr, TimerPtr, &DataPtr->TimerQueue);
				RetryCount = 0;
			}
			else
			{
				/* 1737982: Save timer for retry
				*/
				DataPtr->RetryTimerPtr = TimerPtr;
				RetryCount = TimerPtr->Tract.Txs.Count;
				RetryCount++;
				if (Response ==	DCP_RSP_OK)
					Response = DCP_RSP_ERR_TIMEOUT;
			}
	
			/* 190608lrg001: else indicate the first RQB of upper Queue
			*/
			if (!LSA_HOST_PTR_ARE_EQUAL(RbuPtr, LSA_NULL))
			{
				/* Finish Client service
				*/
				dcp_ClientDone(ChPtr, RbuPtr, Response, Uid, RetryCount);
			}
			else
			{
				DCP_ERROR_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_WARN, "*** dcp_ClientCancel() ChPtr=%X Uid=%u TimerId=%u: no upper RQB",
								ChPtr, Uid, TimerPtr->Id);
			}
		}
	}
	else
	{
		DCP_ERROR_TRACE_02(ChPtr->TraceIdx, LSA_TRACE_LEVEL_WARN, "*** dcp_ClientCancel() ChPtr=%X invalid Uid=%d",
						ChPtr, Uid);
		dcp_FreeLowerRb(ChPtr, RblPu);
	}

	DCP_FUNCTION_TRACE_02(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  < dcp_ClientCancel() ChPtr=%X RbuPtr=%X",
						ChPtr, RbuPtr);
}


/*****************************************************************************/
/* Internal access function: dcp_ClientTimeout()                             */
/* Process Client Request Timeout                                            */
/*****************************************************************************/
LSA_VOID 	DCP_LOCAL_FCT_ATTR dcp_ClientTimeout(
DCP_CHA_PTR				ChPtr,
DCP_LOWER_RQB_PTR_TYPE	RblPu,
DCP_UPPER_RQB_PTR_TYPE	RbuPtr,
DCP_TIMER_PTR_TYPE		TimerPtr,
LSA_UINT8				Uid)
{
	LSA_UINT16			RespVal = DCP_RSP_ERR_TIMEOUT;
	DCP_CLIENT_DATA	*	DataPtr  = LSA_NULL;
	LSA_INT				RetryCount = 0;
	LSA_BOOL 			Found = LSA_FALSE;

	DCP_FUNCTION_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  > dcp_ClientTimeout() ChPtr=%X RbEthPtr=%X RbuPtr=%X",
						ChPtr, RblPu.VoidPtr, RbuPtr);

	if (ChPtr->State == DCP_CHA_STATE_OPEN)
	{
		if (DCP_HELLO == Uid)
		{
			/* Rerun Hello Request if not stopped
			 * else requeue lower RQB
			*/
			RespVal = DCP_RSP_OK;
			TimerPtr->Tract.Txs.RbuPtr = LSA_NULL;
			if (0 !=TimerPtr->Tract.Txs.Count)
			{
				ChPtr->Hello.RetryCount++;
				TimerPtr->RbPu = RblPu;
				RespVal = dcp_ClientHello(ChPtr, LSA_NULL, (LSA_INT)ChPtr->Hello.RetryCount);
			}
			else
			{
				dcp_EnqueLower(&ChPtr->Hello.QueueSend, RblPu, ChPtr);
			}
		}
		else
		{
			DataPtr = dcp_ClientData(ChPtr, Uid);
			if (LSA_HOST_PTR_ARE_EQUAL(DataPtr, LSA_NULL))
			{
				DCP_ERROR_TRACE_02(ChPtr->TraceIdx, LSA_TRACE_LEVEL_ERROR, "*** dcp_ClientTimeout() ChPtr=%X: invalid Uid=%u",
					ChPtr, Uid);
				dcp_ErrorInternal(DCP_MODULE_ID, __LINE__, DCP_ERR_TIMER, ChPtr, Uid);
			}
			else if (!LSA_HOST_PTR_ARE_EQUAL(RblPu.VoidPtr, LSA_NULL))
			{
				/* If timer has a Requestblock: use it for Cancel Request
				 * and queue back timer to service queue
				*/
				Found = dcp_TimerDeque(&ChPtr->TimerQueue , TimerPtr);
				dcp_TimerEnque(&DataPtr->TimerQueue, TimerPtr);
				RespVal = dcp_CancelLower(ChPtr, LSA_FALSE, TimerPtr, RblPu);
				if (RespVal != DCP_RSP_OK_ACTIVE)
				{
					/* On Cancel error: free Lower RQB and timer
					*/
					dcp_FreeLowerRb(ChPtr, RblPu);
					RblPu.VoidPtr = LSA_NULL;
					//1737982 dcp_TimerPut(ChPtr, TimerPtr, &DataPtr->TimerQueue);
					RespVal = DCP_RSP_ERR_LOWER_LAYER;
				}
			}
			if (LSA_HOST_PTR_ARE_EQUAL(RblPu.VoidPtr, LSA_NULL))
			{
				/* If no cancel has been done or on cancel error:
				 * retry Client service Set/Get now or finish service
				 * with Timeout error (Identify with OK).
				 * Free Transaction structure now if no Cancel Response is expected.
				*/
				TimerPtr->Tract.Txs.RbuPtr = LSA_NULL;
				if (DCP_RSP_OK_CANCEL == TimerPtr->Tract.Txs.Response)
				{
					/* Request is canceled: no retry, free timer
					*/
					dcp_TimerPut(ChPtr, TimerPtr, &DataPtr->TimerQueue);
					RespVal = DCP_RSP_OK_CANCEL;
					RetryCount = 0;
				}
				else if (DCP_IDENTIFY == Uid)
				{
					dcp_TimerPut(ChPtr, TimerPtr, &DataPtr->TimerQueue);
					RespVal = DCP_RSP_OK;
				}
				else if ((DCP_GET == Uid) || (DCP_SET == Uid))
				{
					/* 1737982: Save timer for retry
					*/
					DataPtr->RetryTimerPtr = TimerPtr;
					RetryCount = TimerPtr->Tract.Txs.Count;
					RetryCount++;
				}
				else
				{
					dcp_TimerPut(ChPtr, TimerPtr, &DataPtr->TimerQueue);
					RespVal = DCP_RSP_ERR_PROTOCOL;
					DCP_ERROR_TRACE_03(ChPtr->TraceIdx, LSA_TRACE_LEVEL_ERROR, "*** dcp_ClientTimeout() ChPtr=%X TimerPtr=%X: invalid Uid=%d",
							ChPtr, TimerPtr, Uid);
				}
				dcp_ClientDone(ChPtr, RbuPtr, RespVal, Uid, RetryCount);
			}
		}
	}
	else if (!LSA_HOST_PTR_ARE_EQUAL(RblPu.VoidPtr, LSA_NULL))
		dcp_FreeLowerRb(ChPtr, RblPu);


	DCP_FUNCTION_TRACE_05(ChPtr->TraceIdx, LSA_TRACE_LEVEL_NOTE, "  < dcp_ClientTimeout() ChPtr=%X TimerId=%d Uid=%d RespVal=0x%x Found=%X",
						ChPtr, TimerPtr->Id, Uid, RespVal, Found);
}

/*****************************************************************************/
/*  end of file DCP_USR.C                                                    */
/*****************************************************************************/
