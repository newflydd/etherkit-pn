

///*===========================================================================*/
///*                                 module-id                                 */
///*===========================================================================*/
#define LTRC_ACT_MODUL_ID 	2		/* GSY_MODULE_ID_USR */
#define GSY_MODULE_ID 		LTRC_ACT_MODUL_ID 

///*===========================================================================*/
///*                                 includes                                  */
///*===========================================================================*/
#include "gsy_inc.h"
#include "gsy_int.h"
#include "gptpcmn.h"
GSY_FILE_SYSTEM_EXTENSION(GSY_MODULE_ID)

///*****************************************************************************/
///* Local Data                                                                */
///* ...                                                                       */
///*****************************************************************************/


/*****************************************************************************/
/* Internal access function: gsy_DelaySet()                                  */
/* Send port delay values to FWD, EDD and User                               */
/*****************************************************************************/
LSA_VOID GSY_LOCAL_FCT_ATTR gsy_DelaySet(
																				 GSY_CH_SYS_PTR  pChSys,
																				 LSA_UINT16		PortId)
{
	pChSys->Port[PortId-1].DelaySetPending = LSA_FALSE;
	
	if (GSY_HW_TYPE_DELAY_SW == (pChSys->HardwareType & GSY_HW_TYPE_DELAY_MSK))
	{
		/* Set LineDelay in lower layer
		*/
		gsy_LowerDelaySet(pChSys, PortId);
	}
	
	gsy_DiagUserInd(GSY_DIAG_SOURCE_DELAY, pChSys, LSA_NULL, 
									0       /* LSA_UINT32: MasterSeconds */, 
									0       /* LSA_UINT32: MasterNanoseconds */, 
									0       /* LSA_INT32: Offset */, 
									0       /* LSA_INT32: AdjustInterval */, 
									0       /* LSA_INT32: UserValue1 */, 
									0 	    /* LSA_INT32: UserValue2 */, 
									0       /* LSA_INT32: SetTimeHigh */, 
									0       /* LSA_INT32: SetTimeLow */, 
									PortId  /* LSA_INT16: PortId */);
	
}

/*****************************************************************************/
/* External access function: gsy_init()                                      */
/* Initialise data struktures                                                */
/*****************************************************************************/
LSA_UINT16  GSY_SYSTEM_IN_FCT_ATTR  gsy_init(LSA_VOID)
{
	return LSA_RET_OK;
}

/*****************************************************************************/
/* Internal function: gsy_CallbackAnno()                                     */
/*****************************************************************************/
LSA_VOID  GSY_LOCAL_FCT_ATTR  gsy_CallbackAnno(
																							 GSY_CH_SYS_PTR			pChSys,
																							 GSY_UPPER_RQB_PTR_TYPE  pRbu,
																							 LSA_UINT16				RespVal)
{
	GSY_UPPER_CALLBACK_FCT_PTR_TYPE	pCbf = 0;
	
	if(!LSA_HOST_PTR_ARE_EQUAL(pChSys, LSA_NULL))
	{
		if (LSA_HOST_PTR_ARE_EQUAL(pRbu, LSA_NULL))
		{
			/* Take first requst block from upper queue
			*/
			pRbu = pChSys->pChAnno->QueueUpper.pFirst;
		}
		if (LSA_HOST_PTR_ARE_EQUAL(pRbu, LSA_NULL))
		{
			/* No upper RQB ?!?
			*/
			//ERROR
			
		}
		else
		{
			/* Remove RQB from queue and set handle
			*/
			gsy_DequeUpper(&pChSys->pChAnno->QueueUpper, pRbu);
			GSY_UPPER_RQB_HANDLE_SET(pRbu, pChSys->pChAnno->HandleUpper);
			if (GSY_UPPER_RQB_OPCODE_GET(pRbu) == GSY_OPC_OPEN_CHANNEL)
			{
				/* OPEN CHANNEL: return own handle and set channel state to OPEN if OK 
           -OS- HANDLE_LOWER_SET is mendatory for all channels.
				*/
				GSY_UPPER_RQB_HANDLE_LOWER_SET(pRbu, pChSys->pChAnno->Handle);
				
				if (RespVal == GSY_RSP_OK)
					pChSys->pChAnno->State = GSY_CHA_STATE_OPEN;
				
			}
			
			/* Use upper callback function from channel data
			*/
			pCbf = pChSys->pChAnno->pCbf;
		}
	}
	else 
	{
		/* No channel! Error at OPEN CHANNEL?
		*/
	}
	
	if ((!(0 == pCbf)) &&	(!LSA_HOST_PTR_ARE_EQUAL(pRbu, LSA_NULL)))
	{
		/* Set response code and call callback function of upper layer
		*/
		GSY_UPPER_RQB_RESPONSE_SET  (pRbu, RespVal);
		GSY_UPPER_RQB_ERR_COMPID_SET(pRbu, LSA_COMP_ID_UNUSED);
		
		GSY_REQUEST_UPPER_DONE(pCbf, pRbu, pChSys->pChAnno->pSys);
	}
}

/*****************************************************************************/
/* Internal function: gsy_PrmChangePort()                                    */
/*****************************************************************************/
//TODO:-OS- function needed dont remove
LSA_UINT16 GSY_LOCAL_FCT_ATTR gsy_PrmChangePort(
																								GSY_CH_USR_PTR			pChUsr,
																								GSY_UPPER_RQB_PTR_TYPE  pRbu)
{
	LSA_UINT8   ModuleState = GSY_UPPER_RQB_PRMCHANGEPORT_MODULESTATE_GET(pRbu);
	LSA_UINT16  RespVal     = GSY_RSP_OK;
	
#if (GSY_CFG_TRACE_MODE == 0)
	LSA_UNUSED_ARG(pChUsr);
#endif
	
	switch (ModuleState)
	{
	case GSY_PRM_PORTMODULE_PLUG_PREPARE:
		GSY_UPPER_RQB_PRMCHANGEPORT_PORTPARAMSNOTAPPLICABLE_SET(pRbu, GSY_PRM_PORT_PARAMS_APPLICABLE);
		break;
	case GSY_PRM_PORTMODULE_PLUG_COMMIT:
		GSY_FUNCTION_TRACE_02(pChUsr->TraceIdx, LSA_TRACE_LEVEL_NOTE,"    gsy_PrmChangePort() PortID=%d ModuleState=GSY_PRM_PORTMODULE_PLUG_COMMIT ApplyDefaultPortparams=%d", 
													PortID, GSY_UPPER_RQB_PRMCHANGEPORT_APPLYDEFAULTPARAMS_GET(pRbu));
		break;
	case GSY_PRM_PORTMODULE_PULLED:
		GSY_FUNCTION_TRACE_01(pChUsr->TraceIdx, LSA_TRACE_LEVEL_NOTE,"    gsy_PrmChangePort() PortID=%d ModuleState=GSY_PRM_PORTMODULE_PULLED", PortID);
		break;
	default:
		GSY_FUNCTION_TRACE_02(pChUsr->TraceIdx, LSA_TRACE_LEVEL_NOTE,"    gsy_PrmChangePort() PortID=%d ModuleState=%d UNKNOWN", PortID, ModuleState);
		break;
	}

	return(RespVal);
}

/*****************************************************************************/
/* Internal function: gsy_DiagUserGet()                                      */
/* Write actual diagnosis event to user RQB buffer.                          */
/*****************************************************************************/
LSA_UINT16 GSY_LOCAL_FCT_ATTR gsy_DiagUserGet(GSY_CH_SYS_PTR pChSys, GSY_UPPER_RQB_PTR_TYPE  pRbu)
{
	GSY_DRIFT_PTR		pDrift = LSA_NULL;
	LSA_UINT16	 		Trigger = GSY_UPPER_RQB_DIAGIND_TRIGGER_GET(pRbu);
	LSA_UINT16	 		RespVal = GSY_RSP_ERR_PARAM;
	LSA_UINT8			  SyncId = GSY_UPPER_RQB_DIAGIND_SYNCID_GET(pRbu);
	
	GSY_FUNCTION_TRACE_03(pChSys->TraceIdx, LSA_TRACE_LEVEL_CHAT,"  > gsy_DiagUserGet() pChSys=%X pRbu=%X SyncId=0x%02x", pChSys, pRbu, SyncId);
	
	if (0 == (((LSA_UINT32)(SyncId + 1)) & GSY_SYNC_ID_SUPPORTED))
	{
		GSY_ERROR_TRACE_01(pChSys->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_DiagUserGet() Invalid SyncId=0x%x", SyncId);
	}
	else if ((GSY_TRIGGER_NOW != Trigger) && (GSY_TRIGGER_ALL   != Trigger) 
					 &&  (GSY_TRIGGER_SYNC != Trigger) && (GSY_TRIGGER_CHECK != Trigger))
	{
		GSY_ERROR_TRACE_01(pChSys->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_DiagUserGet() Invalid Trigger=0x%x", Trigger);
	}
	else
	{
		RespVal = GSY_RSP_OK_ACTIVE;
		pDrift = &pChSys->Drift[SyncId][0];
		
		if (GSY_TRIGGER_CHECK == Trigger)
		{
			/* Copy input parameters to compare them
			* 161007lrg003: AP00526517 RateValid and PrimaryMaster
			*/
			pDrift->Diag.Ind = GSY_UPPER_RQB_DIAGIND_PARAM_GET(pRbu);
		}
		
		if ((GSY_TRIGGER_NOW == Trigger)
				||  ((pDrift->Diag.Ind.DriftInterval != pDrift->Diag.DriftInterval) && (GSY_TRIGGER_ALL == Trigger))
				||   !GSY_MACADDR_CMP(pDrift->Diag.Ind.MasterMacAddr, pDrift->Diag.MasterMacAddr)
				||   ((GSY_DIAG_SUBDOMAIN_CONFIGURED == pDrift->Diag.Ind.Subdomain?LSA_TRUE:LSA_FALSE) != pDrift->Diag.Subdomain)
				||   ((GSY_DIAG_RATE_VALID           == pDrift->Diag.Ind.RateValid?LSA_TRUE:LSA_FALSE) != pDrift->Diag.RateValid)
				||   (pDrift->Diag.Ind.RcvSyncPrio != pDrift->Diag.RcvSyncPrio)
				||   (pDrift->Diag.Ind.LocalPrio   != pDrift->Diag.LocalPrio)
				||   (pDrift->Diag.Ind.PLLState    != pDrift->Diag.PLLState)
				||   ((GSY_DIAG_SYNCID_SYNCHRONIZED == pDrift->Diag.Ind.StateSync?LSA_TRUE:LSA_FALSE) != pDrift->Diag.StateSync))
		{
			GSY_UPPER_RQB_DIAGIND_SYNCID_SET   (pRbu, SyncId);
			GSY_UPPER_RQB_DIAGIND_MASTERMAC_SET(pRbu, pDrift->Diag.MasterMacAddr);
			GSY_UPPER_RQB_DIAGIND_SUBDOMAIN_SET(pRbu, pDrift->Diag.Subdomain);
			GSY_UPPER_RQB_DIAGIND_STATESYNC_SET(pRbu, pDrift->Diag.StateSync);
			GSY_UPPER_RQB_DIAGIND_RATEVALID_SET(pRbu, pDrift->Diag.RateValid);
			GSY_UPPER_RQB_DIAGIND_RCVPRIO_SET  (pRbu, pDrift->Diag.RcvSyncPrio);
			GSY_UPPER_RQB_DIAGIND_LOCPRIO_SET  (pRbu, pDrift->Diag.LocalPrio);
			GSY_UPPER_RQB_DIAGIND_PLLSTATE_SET (pRbu, pDrift->Diag.PLLState);
			GSY_UPPER_RQB_DIAGIND_DRIFT_SET    (pRbu, pDrift->Diag.DriftInterval);
			GSY_UPPER_RQB_DIAGIND_USER1_SET    (pRbu, pDrift->Diag.UserValue1);
			GSY_UPPER_RQB_DIAGIND_USER2_SET    (pRbu, pDrift->Diag.UserValue2);
			
			pDrift->Diag.Ind = GSY_UPPER_RQB_DIAGIND_PARAM_GET(pRbu);
			RespVal = GSY_RSP_OK;
			
			GSY_SYNC_TRACE_10(pChSys->TraceIdx, LSA_TRACE_LEVEL_NOTE,"--- gsy_DiagUserGet(%02x) MAC=%02x-%02x-%02x Subdomain=%u StateSync=%u RateValid=%u RcvSyncPrio=0x%02x LocalPrio=0x%02x PLLState=%u",
												SyncId, pDrift->Diag.MasterMacAddr.MacAdr[3], pDrift->Diag.MasterMacAddr.MacAdr[4], pDrift->Diag.MasterMacAddr.MacAdr[5], pDrift->Diag.Subdomain, 
												pDrift->Diag.StateSync, pDrift->Diag.RateValid, pDrift->Diag.RcvSyncPrio, pDrift->Diag.LocalPrio, pDrift->Diag.PLLState);
		}
	}
	
	GSY_FUNCTION_TRACE_03(pChSys->TraceIdx, LSA_TRACE_LEVEL_CHAT,"  < gsy_DiagUserGet() pRbu=%X Trigger=%u RespVal=0x%x", pRbu, Trigger, RespVal);
	
	return(RespVal);
}


/*****************************************************************************/
/* Internal access function: gsy_PutUpperQ()	                             */
/* Store upper Request block in Queue                                        */
/*****************************************************************************/
LSA_VOID GSY_LOCAL_FCT_ATTR gsy_PutUpperQ(
																					GSY_UPPER_QUEUE		  *	pQue,
																					GSY_UPPER_RQB_PTR_TYPE	pRbu)
{
	GSY_UPPER_RQB_PTR_TYPE  pLast;
	
	GSY_FUNCTION_TRACE_03(GSY_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT,"  > gsy_PutUpperQ() pRbu=%X pQue=%X Count=%d",
												pRbu, pQue, pQue->Count);
	
	/* Append RQB to the upper Queue
	*/
	pLast = pQue->pLast;
	
	/* Mark new element as Queue end
	*/
	GSY_UPPER_RQB_NEXT_SET(pRbu, LSA_NULL);
	GSY_UPPER_RQB_PREV_SET(pRbu, pLast);
	pQue->pLast = pRbu;
	
	/* If the queue was empty... 
	*/
	if (LSA_HOST_PTR_ARE_EQUAL(pLast, LSA_NULL))
	{
		/* ...the last element is also th first one
		*/
		pQue->pFirst = pRbu;
	}
	else
	{
		/* Connect the new last to the old last element
		*/
		GSY_UPPER_RQB_NEXT_SET(pLast, pRbu);
	}
	/* Queue now contains one more element
	*/
	pQue->Count++;
	GSY_FUNCTION_TRACE_03(GSY_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT,"  < gsy_PutUpperQ() pRbu=%X pQue=%X Count=%d",
												pRbu, pQue, pQue->Count);
}

/*****************************************************************************/
/* Internal function: gsy_CallbackUsr()                                      */
/*****************************************************************************/
LSA_VOID  GSY_LOCAL_FCT_ATTR  gsy_CallbackUsr(
																							GSY_CH_USR_PTR			pChUsr,
																							GSY_UPPER_RQB_PTR_TYPE  pRbu,
																							LSA_UINT16				RespVal)
{
	LSA_UINT32	TraceIdx = 0;
	GSY_UPPER_CALLBACK_FCT_PTR_TYPE	pCbf = 0;
	
	if(!LSA_HOST_PTR_ARE_EQUAL(pChUsr, LSA_NULL))
	{
		TraceIdx = pChUsr->TraceIdx;
		GSY_UPPER_TRACE_03(TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH/*lrglevelnote*/, ">>> gsy_CallbackUsr() pChUsr=%X pRbu=%X RespVal=0x%x",
											 pChUsr, pRbu, RespVal);
		
		if (LSA_HOST_PTR_ARE_EQUAL(pRbu, LSA_NULL))
		{
			/* Take first RQB from Upper Queue
			*/
			pRbu = pChUsr->QueueUpper.pFirst;
		}
		if (LSA_HOST_PTR_ARE_EQUAL(pRbu, LSA_NULL))
		{
			/* No request block for callback ?!?
			*/
			//-OS- replace by somthing usefull			gsy_ErrorInternal(GSY_MODULE_ID, __LINE__, GSY_ERR_NO_RBU, LSA_NULL, 0);
		}
		else
		{
			/* Remove RQB from Queue and set handle
			*/
			gsy_DequeUpper(&pChUsr->QueueUpper, pRbu);
			GSY_UPPER_RQB_HANDLE_SET(pRbu, pChUsr->HandleUpper);
			if (GSY_UPPER_RQB_OPCODE_GET(pRbu) == GSY_OPC_OPEN_CHANNEL)
			{
				/* OPEN CHANNEL: return own Handle and set Channel
				*               State to OPEN if response is OK 
				*/
				GSY_UPPER_RQB_HANDLE_LOWER_SET(pRbu, pChUsr->Handle);
				if (RespVal == GSY_RSP_OK)
					pChUsr->State = GSY_CHA_STATE_OPEN;
				
			}
			
			/* Use Upper callback function stored in channel structure
			*/
			pCbf = pChUsr->pCbf;
		}
	}
	else 
	{
		/* No channel! Error at OPEN CHANNEL?
		*/
		GSY_UPPER_TRACE_02(GSY_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, ">>> gsy_CallbackUsr() pChUsr=NULL pRbu=%X RespVal=0x%x",
											 pRbu, RespVal);
		//-OS- replace by somthing usefull		gsy_ErrorInternal(GSY_MODULE_ID, __LINE__, GSY_ERR_FATAL, LSA_NULL, 0);
	}
	
	if ((!(0 == pCbf)                           )            /* function is called too for system services */
			&&	(!LSA_HOST_PTR_ARE_EQUAL(pRbu, LSA_NULL)))      //lint !e774 Boolean within 'if' always evaluates to True, it depends on the global lint rule -function( exit, gsy_ErrorInternal), HM 20.05.2016
	{
		/* Set Responsecode and pass RQB to Callbackfunktion of upper Layer
		*/
		GSY_UPPER_RQB_RESPONSE_SET(pRbu, RespVal);
		GSY_UPPER_RQB_ERR_COMPID_SET(pRbu, LSA_COMP_ID_UNUSED);
		
		GSY_REQUEST_UPPER_DONE(pCbf, pRbu, pChUsr->pSys);
	}
	else 
	{
		GSY_ERROR_TRACE_02(TraceIdx, LSA_TRACE_LEVEL_FATAL,"*** gsy_CallbackUsr() pCbf=%X pRbu=%X", pCbf, pRbu);
		//-OS- replace by somthing usefull		gsy_ErrorInternal(GSY_MODULE_ID, __LINE__, GSY_ERR_FATAL, LSA_NULL, 0);
	}
}

/*****************************************************************************/
/* Internal access function: gsy_ChSysGet()                                  */
/* Get channel pointer to allocated system channel structure                 */
/*****************************************************************************/
GSY_CH_SYS_PTR GSY_LOCAL_FCT_ATTR  gsy_ChSysGet(
																								LSA_HANDLE_TYPE  Handle)
{
	LSA_HANDLE_TYPE	ChId = 0;
	GSY_CH_SYS_PTR	pChSys = LSA_NULL;
	LSA_UINT32		TraceIdx = 0;
	
	GSY_FUNCTION_TRACE_01(GSY_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT,"  > gsy_ChSysGet() Handle=%d",
												Handle);
	
	/* Check Handle
	*/
	if (Handle > 0)
	{
		ChId = Handle - 1;
		if(ChId < GSY_MAX_SYS_CHANNELS)
		{
			/* Build pointer to system channel structure
			*/
			pChSys = (GSY_CH_SYS_PTR)(gsy_data.pChSysList + ChId);
			
			/* Is not valid if channel is not allocated          
			*/ 
			if (pChSys->State == GSY_CHA_STATE_FREE)
			{
				pChSys = LSA_NULL;
			}
			else
			{
				TraceIdx = pChSys->TraceIdx;
			}
		}
	}
	GSY_FUNCTION_TRACE_02(TraceIdx, LSA_TRACE_LEVEL_CHAT,"  < gsy_ChSysGet() pChSys=%X ChId=%d", pChSys, ChId);
	return(pChSys);
}

/*****************************************************************************/
/* Internal function: gsy_CallbackSys()                                      */
/*****************************************************************************/
LSA_VOID  GSY_LOCAL_FCT_ATTR  gsy_CallbackSys(
																							GSY_CH_SYS_PTR			pChSys,
																							GSY_UPPER_RQB_PTR_TYPE  pRbu,
																							LSA_UINT16				RespVal)
{
	LSA_UINT32	TraceIdx = 0;
	GSY_UPPER_CALLBACK_FCT_PTR_TYPE	pCbf = 0;
	
	if(!LSA_HOST_PTR_ARE_EQUAL(pChSys, LSA_NULL))
	{
		TraceIdx = pChSys->TraceIdx;
		GSY_UPPER_TRACE_03(TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH/*lrglevelnote*/,">>> gsy_CallbackSys() pChSys=%X pRbu=%X RespVal=0x%x",
											 pChSys, pRbu, RespVal);
		
		if (LSA_HOST_PTR_ARE_EQUAL(pRbu, LSA_NULL))
		{
			/* Take first RQB from Upper Queue
			*/
			pRbu = pChSys->QueueUpper.pFirst;
		}
		if (LSA_HOST_PTR_ARE_EQUAL(pRbu, LSA_NULL))
		{
			/* No request block for callback ?!?
			*/
			GSY_ERROR_TRACE_02(TraceIdx, LSA_TRACE_LEVEL_FATAL,"*** gsy_CallbackSys() pChSys=%X pRbu=NULL RespVal=0x%x", pChSys, RespVal);
			//-OS-			gsy_ErrorInternal(GSY_MODULE_ID, __LINE__, GSY_ERR_NO_RBU, LSA_NULL, 0);
		}
		else
		{
			/* Remove RQB from Queue and set handle
			*/
			gsy_DequeUpper(&pChSys->QueueUpper, pRbu);
			GSY_UPPER_RQB_HANDLE_SET(pRbu, pChSys->HandleUpper);
			if (GSY_UPPER_RQB_OPCODE_GET(pRbu) == GSY_OPC_OPEN_CHANNEL)
			{
				/* OPEN CHANNEL: return own Handle and set Channel
				*               State to OPEN if response is OK 
				*/
				GSY_UPPER_RQB_HANDLE_LOWER_SET(pRbu, pChSys->Handle);
				if (RespVal == GSY_RSP_OK)
					pChSys->State = GSY_CHA_STATE_OPEN;
				
			}
			
			/* Use Upper callback function stored in channel structure
			*/
			pCbf = pChSys->pCbf;
		}
	}
	else 
	{
		/* No channel! Error at OPEN CHANNEL?
		*/
		GSY_UPPER_TRACE_02(GSY_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL,">>> gsy_CallbackSys() pChSys=NULL pRbu=%X RespVal=0x%x",
											 pRbu, RespVal);
		//-OS-		gsy_ErrorInternal(GSY_MODULE_ID, __LINE__, GSY_ERR_FATAL, LSA_NULL, 0);
	}
	
	if ((!(0 == pCbf)         )            /* function is called too for system services */
			&&	(!LSA_HOST_PTR_ARE_EQUAL(pRbu, LSA_NULL)))       //lint !e774 Boolean within 'if' always evaluates to True, it depends on the global lint rule -function( exit, gsy_ErrorInternal), HM 20.05.2016
	{
		/* Set Responsecode and pass RQB to Callbackfunktion of upper Layer
		*/
		GSY_UPPER_RQB_RESPONSE_SET(pRbu, RespVal);
		GSY_UPPER_RQB_ERR_COMPID_SET(pRbu, LSA_COMP_ID_UNUSED);
		
		GSY_UPPER_TRACE_04(TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH/*lrglevelnote*/,"< < gsy_CallbackSys(Opcode=%d) pRbu=%X Response=0x%x HandleUpper=%d", 
											 GSY_UPPER_RQB_OPCODE_GET(pRbu), pRbu, GSY_UPPER_RQB_RESPONSE_GET(pRbu), GSY_UPPER_RQB_HANDLE_GET(pRbu));
		//		pCbf(pRbu);
		GSY_REQUEST_UPPER_DONE(pCbf, pRbu, pChSys->pSys);
	}
	else 
	{
		GSY_ERROR_TRACE_02(TraceIdx, LSA_TRACE_LEVEL_FATAL,"*** gsy_CallbackSys() pCbf=%X pRbu=%X", pCbf, pRbu);
		//-OS-		gsy_ErrorInternal(GSY_MODULE_ID, __LINE__, GSY_ERR_FATAL, LSA_NULL, 0);
	}
	
	GSY_UPPER_TRACE_02(TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH/*lrglevelnote*/,"<<< gsy_CallbackSys() pRbu=%X pCbf=%X",
										 pRbu, pCbf);
}

/*****************************************************************************/
/* Internal access function: gsy_DequeUpper()		                         */
/* Find and dequeue upper RQB                                                */
/*****************************************************************************/
LSA_VOID    GSY_LOCAL_FCT_ATTR gsy_DequeUpper(
																							GSY_UPPER_QUEUE		  *	pQue,
																							GSY_UPPER_RQB_PTR_TYPE	pRbu)
{
	GSY_FUNCTION_TRACE_03(GSY_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT,"  > gsy_DequeUpper() pRbu=%X pQue=%X Count=%d",
												pRbu, pQue, pQue->Count);
	
	/* Find Request block in the Queue...
	*/
	if (gsy_FindUpper(pQue, pRbu))
	{
		/* ...and if found: remove it
		*/
		gsy_GetUpperQ(pQue, pRbu);
	}
	
	GSY_FUNCTION_TRACE_03(GSY_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT,"  < gsy_DequeUpper() pRbu=%X pQue=%X Count=%d",
												pRbu, pQue, pQue->Count);
}

/*****************************************************************************/
/* Internal access function: gsy_FindUpper()		                         */
/* Find upper Request block in a Queue                                       */
/*****************************************************************************/
LSA_BOOL    GSY_LOCAL_FCT_ATTR gsy_FindUpper(
																						 GSY_UPPER_QUEUE		  *	pQue,
																						 GSY_UPPER_RQB_PTR_TYPE	pRbu)
{
	GSY_UPPER_RQB_PTR_TYPE  pNext;
	LSA_BOOL				RbFound = LSA_TRUE;
	
	GSY_FUNCTION_TRACE_03(GSY_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT,"  > gsy_FindUpper() pRbu=%X pQue=%X Count=%d",
												pRbu, pQue, pQue->Count);
	
	pNext = pQue->pFirst;
	while (!LSA_HOST_PTR_ARE_EQUAL(pNext, pRbu)
				 &&  !LSA_HOST_PTR_ARE_EQUAL(pNext, LSA_NULL))
	{
		pNext = GSY_UPPER_RQB_NEXT_GET(pNext);
	}
	
	if (LSA_HOST_PTR_ARE_EQUAL(pNext, LSA_NULL))
	{
		RbFound = LSA_FALSE;
	}
	
	GSY_FUNCTION_TRACE_03(GSY_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT,"  < gsy_FindUpper() pRbu=%X pQue=%X RbFound=%d",
												pRbu, pQue, RbFound);
	return(RbFound);
}

/*****************************************************************************/
/* Internal access function: gsy_GetUpperQ()	                             */
/* Remove upper Request block from Queue                                     */
/*****************************************************************************/
LSA_VOID GSY_LOCAL_FCT_ATTR gsy_GetUpperQ(
																					GSY_UPPER_QUEUE		  *	pQue,
																					GSY_UPPER_RQB_PTR_TYPE	pRbu)
{
	GSY_UPPER_RQB_PTR_TYPE  pPrev;
	GSY_UPPER_RQB_PTR_TYPE  pNext;
	
	GSY_FUNCTION_TRACE_03(GSY_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT,"  > gsy_GetUpperQ() pRbu=%X pQue=%X Count=%d",
												pRbu, pQue, pQue->Count);
	
	pNext = GSY_UPPER_RQB_NEXT_GET(pRbu);
	pPrev = GSY_UPPER_RQB_PREV_GET(pRbu);
	
	/* If a predecessor is present... 
	*/
	if (!LSA_HOST_PTR_ARE_EQUAL(pPrev, LSA_NULL))
	{
		/* ...set its successor to next element
		*/
		GSY_UPPER_RQB_NEXT_SET(pPrev, pNext);
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
		GSY_UPPER_RQB_PREV_SET(pNext, pPrev);
	}
	else
	{
		/* The predecessor becomes last element of the queue
		*/
		pQue->pLast = pPrev;
	}
	/* Delete chain pointers of removed RQB
	*/
	GSY_UPPER_RQB_PREV_SET(pRbu, LSA_NULL);
	GSY_UPPER_RQB_NEXT_SET(pRbu, LSA_NULL);
	
	/* Queue now contains one less element
	*/
	pQue->Count--;
	
	GSY_FUNCTION_TRACE_03(GSY_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT,"  < gsy_GetUpperQ() pRbu=%X pQue=%X Count=%d",
												pRbu, pQue, pQue->Count);
}

/*****************************************************************************/
/* External LSA access function: gsy_close_channel()                         */
/*****************************************************************************/
LSA_VOID  GSY_UPPER_IN_FCT_ATTR  gsy_close_channel(
																									 GSY_UPPER_RQB_PTR_TYPE  pRbu)
{		
	// We will never close a Channel, since PNO stack cannot be teared down.
	// and in case of error we are unoperational (no need to free memory for that)
  
	return;
}

/*****************************************************************************/
/* Internal access function: gsy_ChSysFind()                                 */
/* Find NicId in system channel structures                                   */
/*****************************************************************************/
GSY_CH_SYS_PTR  GSY_LOCAL_FCT_ATTR  gsy_ChSysFind(
																									LSA_UINT16 NicId)
{
	LSA_INT			Idx;
	GSY_CH_SYS_PTR 	pChSys = LSA_NULL;
	
	for (Idx = 0; Idx < GSY_MAX_SYS_CHANNELS; Idx++)
	{
		pChSys = (GSY_CH_SYS_PTR)(gsy_data.pChSysList + Idx);
		if ((pChSys->State != GSY_CHA_STATE_FREE)
				&&  (pChSys->pCDB->NicId == NicId))
		{
			/* Channel found
			*/
			break;
		}
	}
	
	if (Idx == GSY_MAX_SYS_CHANNELS)
	{
		pChSys = LSA_NULL;
	}
	
	return(pChSys);
}

/*****************************************************************************/
/* Internal function: gsy_CallbackRbu()                                      */
/*****************************************************************************/
LSA_VOID  GSY_LOCAL_FCT_ATTR  gsy_CallbackRbu(
																							LSA_SYS_PTR_TYPE		pSys,
																							GSY_UPPER_RQB_PTR_TYPE  pRbu,
																							LSA_UINT16				RespVal)
{
	GSY_UPPER_CALLBACK_FCT_PTR_TYPE	pCbf = 0;
	
	if (LSA_HOST_PTR_ARE_EQUAL(pRbu, LSA_NULL))
	{
		/* No request block for callback ?!?
		*/
		//ERROR
	}
	else
	{
		if (GSY_UPPER_RQB_OPCODE_GET(pRbu) == GSY_OPC_OPEN_CHANNEL)
		{
			/* Get Upper callback function and upper Handle from RQB 
			*/
			GSY_UPPER_RQB_HANDLE_SET(pRbu, GSY_UPPER_RQB_HANDLE_UPPER_GET(pRbu));
			pCbf = GSY_UPPER_RQB_CBF_GET(pRbu);
		}
		else
		{
			//ERROR
		}
	}
	
	if ((!(0 == pCbf)                                  )            /* function is called too for system services */
			&&	(!LSA_HOST_PTR_ARE_EQUAL(pRbu, LSA_NULL       )))      //lint !e774 Boolean within 'if' always evaluates to True, it depends on the global lint rule -function( exit, gsy_ErrorInternal), HM 20.05.2016
	{
		/* Set Responsecode and pass RQB to Callbackfunktion of upper Layer
		*/
		GSY_UPPER_RQB_RESPONSE_SET(pRbu, RespVal);
		GSY_UPPER_RQB_ERR_COMPID_SET(pRbu, LSA_COMP_ID_UNUSED);
		
		GSY_REQUEST_UPPER_DONE(pCbf, pRbu, pSys);
	}
	else 
	{
		//ERROR
	}
}

/*****************************************************************************/
/* External LSA access function: gsy_open_channel()                          */
/*****************************************************************************/
LSA_VOID  GSY_UPPER_IN_FCT_ATTR  gsy_open_channel(
																									GSY_UPPER_RQB_PTR_TYPE  pRbu)
{
	LSA_UINT16  RespVal = GSY_RSP_OK_ACTIVE;
	LSA_OPCODE_TYPE Opcode;                                   \
		
		GSY_ENTER();
		GSY_UPPER_TRACE_01(0, LSA_TRACE_LEVEL_NOTE_HIGH,">>> gsy_open_channel() pRbu=%X",
											 pRbu);
		
		if(!LSA_HOST_PTR_ARE_EQUAL(pRbu, LSA_NULL))
		{
			/* Mark RQB active
			*/
			GSY_UPPER_RQB_RESPONSE_SET(pRbu, GSY_RSP_OK_ACTIVE);
			Opcode = GSY_UPPER_RQB_OPCODE_GET(pRbu);
			switch(Opcode)
			{
			case GSY_OPC_OPEN_CHANNEL:
				if(0 != GSY_UPPER_RQB_CBF_GET(pRbu))
				{
					RespVal = gsy_ChOpen(pRbu);
				}
				else
				{
					RespVal = GSY_RSP_ERR_PARAM;
					GSY_ERROR_TRACE_01(0, LSA_TRACE_LEVEL_ERROR,"*** gsy_open_channel() pRbu=%X: gsy_request_upper_done_ptr=0", pRbu);
					GSY_UPPER_RQB_RESPONSE_SET(pRbu, RespVal);
					//TODO: -OS-				GSY_RQB_ERROR(pRbu);
				}
				break;
				
			default:
				RespVal = GSY_RSP_ERR_SYNTAX;
				GSY_UPPER_RQB_RESPONSE_SET(pRbu, RespVal);
				//-OS-			GSY_RQB_ERROR(pRbu);
			}
		}
		else
		{
			//ERROR
		}
		
		GSY_EXIT();
		return;
}

/*****************************************************************************/
/* Internal access function: gsy_ChSysAlloc()                               */
/* Systemchannel-Struktur allokieren                                         */
/*****************************************************************************/
GSY_CH_SYS_PTR  GSY_LOCAL_FCT_ATTR  gsy_ChSysAlloc(
																									 GSY_UPPER_RQB_PTR_TYPE  pRbu,
																									 LSA_SYS_PTR_TYPE		pSys,
																									 GSY_DETAIL_PTR_TYPE		pCDB)
{
	LSA_INT				iii, ii, i, Idx;
	GSY_CH_SYS_PTR 		pChSys = LSA_NULL;
	
	for (Idx = 0; Idx < GSY_MAX_SYS_CHANNELS; Idx++)
	{
		pChSys = (GSY_CH_SYS_PTR)(gsy_data.pChSysList + Idx);
		if (pChSys->State == GSY_CHA_STATE_FREE)
		{
			/* Found free channel: allocate and initialise
			*/
			pChSys->State = GSY_CHA_STATE_OPENING;
			pChSys->pSys = pSys;
			pChSys->pCDB = pCDB;
			pChSys->Resp = GSY_RSP_OK;
			pChSys->PortInit = 0;
			pChSys->PortCount = 2;
			pChSys->RxTxNanos = 0;
			pChSys->TraceIdx = 0;
			pChSys->HardwareType = GSY_HW_TYPE_STANDARD;
			pChSys->SysPath = GSY_UPPER_RQB_SYSPATH_GET(pRbu);
			pChSys->HandleUpper = GSY_UPPER_RQB_HANDLE_UPPER_GET(pRbu);
			pChSys->pCbf = GSY_UPPER_RQB_CBF_GET(pRbu);
			pChSys->PortDelayReqActive = 0;
			pChSys->PortDelayRspActive = 0;
			pChSys->DriftTimer.InUse = LSA_FALSE;
			pChSys->DriftTimer.Running = LSA_FALSE;
			pChSys->DriftTimer.pChSys = pChSys;
			
			for (i = 0; i < GSY_CH_MAX_PORTS ; i++)
			{
				/* 290306lrg001: allow forwarding for all configured SyncIds on all ports
				*/
				pChSys->PortFwd[i].FwdMode = GSY_SYNC_ID_SUPPORTED;
				pChSys->PortInp[i].FwdMode = GSY_SYNC_ID_SUPPORTED;
				
				pChSys->Port[i].State = GSY_PORT_STATE_INIT;
				pChSys->Port[i].SyncLineDelay = 0;
				pChSys->Port[i].CableDelay = 0;
				pChSys->Port[i].MinDelay = 0;
				pChSys->Port[i].MaxDelay = 0;
				pChSys->Port[i].SumCount = 0;
				pChSys->Port[i].TxDelay = 0;
				pChSys->Port[i].RxDelay = 0;
				pChSys->Port[i].ClockSyncOk = LSA_FALSE; 				// 170707lrg001
				pChSys->Port[i].DelayReqActive = LSA_FALSE;
				pChSys->Port[i].DelayRspActive = LSA_FALSE;				// 291008lrg001
				
#ifndef GSY_CFG_SYNC_ONLY_IN_KRISC32
				
				pChSys->Port[i].SumDelay = 0;
				pChSys->Port[i].SumCountLast = 0;
				pChSys->Port[i].DelayEvent = GSY_SYNC_EVENT_DELAY_OK;
				pChSys->Port[i].ActDelay.State = GSY_DELAY_STATE_INI;
				pChSys->Port[i].ActDelay.pDrift = LSA_NULL;
				pChSys->Port[i].LastError = LSA_RSP_OK; 
				pChSys->Port[i].ErrorCount = 0; 
				pChSys->Port[i].DelayReqSeqId = 0;
				pChSys->Port[i].DelayTimer.InUse = LSA_FALSE;
				pChSys->Port[i].DelayTimer.Running = LSA_FALSE;
				pChSys->Port[i].DelayTimer.Port = (LSA_UINT16)(i + 1);	// 060509lrg001
				pChSys->Port[i].DelayTimer.pChSys = pChSys;
				
#endif //GSY_CFG_SYNC_ONLY_IN_KRISC32
				
				pChSys->Port[i].PrmDomainWritten = LSA_FALSE;
				pChSys->Port[i].PrmDomainState = GSY_PRM_PORT_STATE_INI;
				pChSys->Port[i].PrmPeerWritten = LSA_FALSE;
				pChSys->Port[i].PrmPeerState = GSY_PRM_PORT_STATE_INI;
				pChSys->Port[i].DelaySetPending = LSA_FALSE;			//240608lrg001
				pChSys->Port[i].SendCnfPending = 0;						//240608lrg001
				pChSys->Port[i].pExtLinkRbl = LSA_NULL;
			}
			
			pChSys->pChUsr = LSA_NULL;
			
			for (i = 0; i < GSY_MAX_PROTOCOLS ; i++)
			{
				for (ii = 0; ii < GSY_CFG_FWD_MAX_MASTERS_PER_PROTOCOL ; ii++)
				{
					GSY_MEMSET_LOCAL(&pChSys->Drift[i][ii], 0, sizeof(GSY_DRIFT_TYPE));
					pChSys->Drift[i][ii].Diag.SyncId = (LSA_UINT8)i;				//230310lrg001; 040811lrg: cast
					pChSys->Drift[i][ii].Diag.PLLState = GSY_PLL_STATE_SYNC_OFF;
					pChSys->Drift[i][ii].SyncId = GSY_SYNCID_NONE;
					pChSys->Drift[i][ii].SyncIdRate = (LSA_UINT8)i;		// 1684006
					pChSys->Drift[i][ii].State = GSY_DRIFT_STATE_UNKNOWN;
					pChSys->Drift[i][ii].PendingSet = LSA_FALSE;		// 1684006
					pChSys->Drift[i][ii].DiagCount = 0;
					pChSys->Drift[i][ii].IntervalSetValid = LSA_FALSE;	/* 310707js001 */
					//					pChSys->Drift[i][ii].PrimaryMaster = LSA_FALSE;		/* 161007lrg003 170408lrg002 */
					pChSys->Drift[i][ii].RcvSyncPrio = 0;				/* 170408lrg002 */
					pChSys->Drift[i][ii].LocalPrio = 0;					/* 170408lrg002 */
					
					for (iii = 0; iii < GSY_DRIFT_MAX_COUNT ; iii++)
					{
						pChSys->Drift[i][ii].Old[iii].Used = LSA_TRUE;	//180108lrg001
					}
				}
			}
			pChSys->Fwd.Status = GSY_FWD_CHANNEL_CLOSED;
			
			/* Write own channel to RQB
			*/
			GSY_UPPER_RQB_HANDLE_SET(pRbu, pChSys->Handle);
			break;
		}
		else
			pChSys = LSA_NULL;
	}
	
	GSY_FUNCTION_TRACE_03(GSY_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT,"  < gsy_ChSysAlloc() pRbu=%X pChSys=%X Idx=%d",
												pRbu, pChSys, Idx);
	return(pChSys);
}

/*****************************************************************************/
/* Internal access function: gsy_ChAnnoAlloc()                               */
/* Allocate announce channel structure                                       */
/*****************************************************************************/
GSY_CH_ANNO_PTR	GSY_LOCAL_FCT_ATTR	gsy_ChAnnoAlloc(
																										GSY_CH_SYS_PTR 			pChSys,
																										GSY_UPPER_RQB_PTR_TYPE  pRbu,
																										LSA_SYS_PTR_TYPE		pSys,
																										GSY_DETAIL_PTR_TYPE		pCDB)
{
	LSA_INT				Idx;
	GSY_CH_ANNO_PTR 	pChAnno = LSA_NULL;
	
	for (Idx = 0; Idx < GSY_MAX_SYS_CHANNELS; Idx++)
	{
		pChAnno = (GSY_CH_ANNO_PTR)(gsy_data.pChAnnoList + Idx);
		if (pChAnno->State == GSY_CHA_STATE_FREE)
		{
			/* Found free channel: allocate and initialise
			*/
			pChAnno->State = GSY_CHA_STATE_OPENING;
			pChAnno->pSys = pSys;
			pChAnno->pCDB = pCDB;
			pChAnno->Resp = GSY_RSP_OK;
			pChAnno->SysPath = GSY_UPPER_RQB_SYSPATH_GET(pRbu);
			pChAnno->HandleUpper = GSY_UPPER_RQB_HANDLE_UPPER_GET(pRbu);
			pChAnno->pCbf = GSY_UPPER_RQB_CBF_GET(pRbu);
			pChAnno->pChSys = pChSys;
			pChAnno->TraceIdx = pChSys->TraceIdx;
			pChSys->pChAnno = pChAnno;
			
			/* Write own channel to RQB
			*/
			GSY_UPPER_RQB_HANDLE_SET(pRbu, pChAnno->Handle);
			break;
		}
		else
			pChAnno = LSA_NULL;
	}
	
	return(pChAnno);
}

/*****************************************************************************/
/* Internal access function: gsy_ChUsrAlloc()                                */
/* Allocate user channel structure                                           */
/*****************************************************************************/
GSY_CH_USR_PTR  GSY_LOCAL_FCT_ATTR  gsy_ChUsrAlloc(
																									 GSY_CH_SYS_PTR 			pChSys,
																									 GSY_UPPER_RQB_PTR_TYPE  pRbu,
																									 LSA_SYS_PTR_TYPE		pSys,
																									 GSY_DETAIL_PTR_TYPE		pCDB)
{
	LSA_INT			Idx;
	GSY_CH_USR_PTR 	pChUsr = LSA_NULL;
	
	GSY_FUNCTION_TRACE_03(pChSys->TraceIdx, LSA_TRACE_LEVEL_CHAT,"  > gsy_ChUsrAlloc() pChSys=%X pRbu=%X pCDB=%X",
												pChSys, pRbu, pCDB);
	
	for (Idx = 0; Idx < GSY_MAX_USR_CHANNELS; Idx++)
	{
		pChUsr = (GSY_CH_USR_PTR)(gsy_data.pChUsrList + Idx);
		if (pChUsr->State == GSY_CHA_STATE_FREE)
		{
			/* Found free channel: allocate and initialise
			*/
			pChUsr->State = GSY_CHA_STATE_OPENING;
			pChUsr->pSys = pSys;
			pChUsr->pCDB = pCDB;
			pChUsr->SysPath = GSY_UPPER_RQB_SYSPATH_GET(pRbu);
			pChUsr->HandleUpper = GSY_UPPER_RQB_HANDLE_UPPER_GET(pRbu);
			pChUsr->pCbf = GSY_UPPER_RQB_CBF_GET(pRbu);
			pChUsr->pChSys = pChSys;
			pChUsr->TraceIdx = pChSys->TraceIdx;
			pChUsr->qRTC2Rbl.pFirst = LSA_NULL;
			pChUsr->qRTC2Rbl.pLast = LSA_NULL;
			pChUsr->qRTC2Rbl.Count = 0;
			pChUsr->RTC2RblCount = 0;
			pChUsr->pSyncSetRbl	= LSA_NULL;
			pChUsr->PrmState = GSY_PRM_STATE_NONE;
			pChUsr->PrmSubdomainIDChanged = LSA_FALSE; /* 110806js001 */
			pChUsr->PrmRTC2 = LSA_FALSE;
			pChUsr->RTC2Sync = LSA_FALSE;
			pChUsr->RTC2Active = LSA_FALSE;
			pChUsr->RTC2Ctrl = LSA_FALSE;
			pChUsr->SyncId = GSY_SYNCID_NONE;
			pChUsr->SyncRole = GSY_SYNC_ROLE_LOCAL;
			pChUsr->DiagCount = 0;
			pChUsr->DiagIdxWr = 0;
			pChUsr->DiagNo = 0;
			pChUsr->MasterControlState = GSY_MASTER_CONTROL_STATE_NONE;
			pChUsr->InfoSync = LSA_TRUE;
			pChUsr->Event.SyncEvent = GSY_SYNC_EVENT_NONE;
			pChUsr->Event.OldEvent = GSY_SYNC_EVENT_NONE;
			pChUsr->Event.Prm.ChannelProperties = GSY_PRM_CHANNEL_PROPERTIES;
			pChUsr->Event.Prm.ChannelErrorType = GSY_PRM_CHANNEL_ERROR_TYPE_SYNC;
			pChUsr->Event.Prm.ExtChannelAddValue = GSY_PRM_CHANNEL_ADD_NONE;
			pChUsr->Event.OldPrm.ExtChannelErrorType = GSY_PRM_EVENT_NONE;
			pChUsr->Event.PrmFlags = GSY_PRM_IND_FLAGS_NONE;
			pChUsr->Event.NewPrm = LSA_FALSE;
			pChUsr->Event.PortId = 0;
			pChUsr->Slave.SyncTimer.InUse = LSA_FALSE;
			pChUsr->Slave.SyncTimer.Running = LSA_FALSE;
			pChUsr->Slave.SyncTimer.pChUsr = pChUsr;
			pChUsr->Slave.SyncTimer.pChSys = pChSys;
			pChUsr->Slave.State = GSY_SLAVE_STATE_OFF;
			pChUsr->Slave.Act.Timestamp = 0;
			pChUsr->Slave.Act.IndTimestamp = 0;
			pChUsr->Slave.Act.DelaySeconds = 0;
			pChUsr->Slave.Act.DelayNanos = 0;
			pChUsr->PLLData.State = GSY_PLL_STATE_SYNC_OFF;
			pChUsr->PLLData.CycleLength = 0;
			pChUsr->PLLData.ReductionRatio = 0;
			pChUsr->PLLData.PLLHWDelay = 0;
			pChUsr->PLLData.SetWaitCount = 0;
			pChUsr->PLLData.ActWaitCount = 0;
			pChUsr->SyncPLLActive = LSA_FALSE;
			pChUsr->Master.SeqId = 0;
			pChUsr->Master.State = GSY_MASTER_STATE_OFF;
			pChUsr->Master.SendTimer.InUse = LSA_FALSE;
			pChUsr->Master.SendTimer.Running = LSA_FALSE;
			pChUsr->Master.SendTimer.pChUsr = pChUsr;
			pChUsr->Master.SendTimer.pChSys = pChSys;
			GSY_MEMSET_LOCAL(&pChUsr->SyncData, 0, sizeof(GSY_SYNC_DATA_RECORD));
			
			/* Write own channel to RQB
			*/
			GSY_UPPER_RQB_HANDLE_SET(pRbu, pChUsr->Handle);
			break;
		}
		else
			pChUsr = LSA_NULL;
	}
	
	return(pChUsr);
}

/*****************************************************************************/
/* Internal function: gsy_ChOpen()                                           */
/*****************************************************************************/
LSA_UINT16  GSY_LOCAL_FCT_ATTR  gsy_ChOpen(
																					 GSY_UPPER_RQB_PTR_TYPE  pRbu)
{
	LSA_SYS_PATH_TYPE		SysPath;
	GSY_DETAIL_PTR_TYPE		pCDB;
	LSA_SYS_PTR_TYPE		pSys;
	GSY_CH_SYS_PTR 	        pChSys = LSA_NULL;
	GSY_CH_USR_PTR 	        pChUsr = LSA_NULL;
	GSY_CH_ANNO_PTR	        pChAnno = LSA_NULL;
	LSA_UINT16   	        RespVal = GSY_RSP_ERR_RESOURCE;
	LSA_UINT16   	        RetVal  = LSA_RET_OK;
	
	SysPath = GSY_UPPER_RQB_SYSPATH_GET(pRbu);
	GSY_GET_PATH_INFO(&RetVal, &pSys, &pCDB, SysPath);
	
	if (RetVal == LSA_RET_OK)
	{
		if (pCDB->PathType == GSY_PATH_TYPE_SYNC)
		{
			memset(gsy_data.ChSysList,0,sizeof(GSY_CH_SYS));
			memset(gsy_data.ChAnnoList,0,sizeof(GSY_CH_ANNO));
			memset(gsy_data.ChUsrList,0,sizeof(GSY_CH_USR));
			
			gsy_data.pChSysList  = gsy_data.ChSysList;
			gsy_data.pChAnnoList = gsy_data.ChAnnoList;
			gsy_data.pChUsrList  = gsy_data.ChUsrList;
			gsy_data.Fwd.ProtCnt = 2;
			
			gsy_data.ChSysList[0].State   = GSY_CHA_STATE_FREE;
			gsy_data.pChAnnoList[0].State = GSY_CHA_STATE_FREE;
			gsy_data.pChUsrList[0].State  = GSY_CHA_STATE_FREE;
			
			gsy_data.ChSysList[0].Handle   = 1;
			gsy_data.pChAnnoList[0].Handle = 2;
			gsy_data.pChUsrList[0].Handle  = 3;
			/* System channel: already exists another one with the same NicId?
			*/
			pChSys = gsy_ChSysFind(pCDB->NicId);
			if (!LSA_HOST_PTR_ARE_EQUAL(pChSys, LSA_NULL))
			{
				GSY_ERROR_TRACE_02(pChSys->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_ChOpen() System channel pChSys=%X already exists with pCDB->NicId=%u",
													 pChSys, pCDB->NicId);
				pChSys = LSA_NULL;
				RespVal = GSY_RSP_ERR_CONFIG;
			}
			else
			{
				/* Allocate strukture for system channel
				*/
				pChSys = gsy_ChSysAlloc(pRbu, pSys, pCDB);
				if(!LSA_HOST_PTR_ARE_EQUAL(pChSys, LSA_NULL))
				{
					/* Queue RQB and open lower channel
					*/
					gsy_PutUpperQ(&pChSys->QueueUpper, pRbu);
					RespVal = gsy_OpenLowerStart(pChSys, LSA_FALSE);
					if (RespVal != GSY_RSP_OK_ACTIVE)
						gsy_GetUpperQ(&pChSys->QueueUpper, pRbu);
				}
			}
			
			/* On error: free Pathinfo and channel
			*/
			if ((RespVal != GSY_RSP_OK) && (RespVal != GSY_RSP_OK_ACTIVE))
			{
				if (!LSA_HOST_PTR_ARE_EQUAL(pChSys, LSA_NULL))
				{
					//RetVal = gsy_ChSysClean(pChSys);
					if (LSA_RET_OK != RetVal)
					{
						//ERROR
					}
				}
				/* 210710lrg001: Callback before RELEASE_PATH_INFO()
				*/ 
				gsy_CallbackRbu(pSys, pRbu, RespVal);
				if (!LSA_HOST_PTR_ARE_EQUAL(pChSys, LSA_NULL))
				{
					//gsy_ChSysFree(pChSys);
				}
			}
		}
		else if (pCDB->PathType == GSY_PATH_TYPE_ANNO)
		{
			/* Announce channel: Exists a system channel with this NicId?
			*/
			pChSys = gsy_ChSysFind(pCDB->NicId);
			if (LSA_HOST_PTR_ARE_EQUAL(pChSys, LSA_NULL))
			{
				RespVal = GSY_RSP_ERR_SEQUENCE;
			}
			else if (GSY_CHA_STATE_OPEN != pChSys->State)
			{
				RespVal = GSY_RSP_ERR_SEQUENCE;
			}
			else if (!LSA_HOST_PTR_ARE_EQUAL(pChSys->pChAnno, LSA_NULL))
			{
				RespVal = GSY_RSP_ERR_SEQUENCE;
			}
			else
			{
				/* Allocate strukture for announce channel
				*/
				pChAnno = gsy_ChAnnoAlloc(pChSys, pRbu, pSys, pCDB);
				if(!LSA_HOST_PTR_ARE_EQUAL(pChAnno, LSA_NULL))
				{
					/* Queue RQB and open lower channel
					*/
					gsy_PutUpperQ(&pChAnno->QueueUpper, pRbu);
					RespVal = gsy_OpenLowerStart(pChSys, LSA_TRUE);
					if (RespVal != GSY_RSP_OK_ACTIVE)
						gsy_GetUpperQ(&pChAnno->QueueUpper, pRbu);
				}
			}
			
			/* On error: free path info and channel structure
			*/
			if ((RespVal != GSY_RSP_OK) && (RespVal != GSY_RSP_OK_ACTIVE))
			{
				if (!LSA_HOST_PTR_ARE_EQUAL(pChAnno, LSA_NULL))
				{
					//gsy_ChAnnoClean(pChAnno);
				}
				/* 210710lrg001: Callback before RELEASE_PATH_INFO()
				*/ 
				gsy_CallbackRbu(pSys, pRbu, RespVal);
				if (!LSA_HOST_PTR_ARE_EQUAL(pChAnno, LSA_NULL))
				{
					//gsy_ChAnnoFree(pChAnno);
				}
			}
		}
		else if (pCDB->PathType == GSY_PATH_TYPE_USR)
		{
			/* User channel: exists a system channel with this NicId?
			*/
			pChSys = gsy_ChSysFind(pCDB->NicId);
			if (LSA_HOST_PTR_ARE_EQUAL(pChSys, LSA_NULL))
			{
				//				GSY_ERROR_TRACE_01(GSY_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR,"*** gsy_ChOpen() No system channel found with pCDB->NicId=%u",
				//								pCDB->NicId);
				RespVal = GSY_RSP_ERR_SEQUENCE;
			}
			else if (LSA_HOST_PTR_ARE_EQUAL(pChSys->pChAnno, LSA_NULL))
			{
				//				GSY_ERROR_TRACE_01(pChSys->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_ChOpen() No announce channel found with pCDB->NicId=%u",
				//								pCDB->NicId);
				RespVal = GSY_RSP_ERR_SEQUENCE;
			}
			else if (!LSA_HOST_PTR_ARE_EQUAL(pChSys->pChUsr, LSA_NULL))
			{
				//				GSY_ERROR_TRACE_01(pChSys->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_ChOpen() User channel for pChUsr=%X already open",
				//								pChSys->pChUsr);
				RespVal = GSY_RSP_ERR_CONFIG;
			}
			else
			{
				/* Allocate strukture for user channel
				*/
				pChUsr = gsy_ChUsrAlloc(pChSys, pRbu, pSys, pCDB);
				if(!LSA_HOST_PTR_ARE_EQUAL(pChUsr, LSA_NULL))
				{
					/* Connect system channel with this user channel
					*/
					pChSys->pChUsr = pChUsr;
					pChSys->UserCount++;
					
					/* Prepare User channel for PrmWrite without PrmPrepare
					*/
					RespVal = gsy_PrmPrepare(pChUsr, LSA_NULL);
				}
			}
			if (RespVal != GSY_RSP_OK)
			{
				/* 210710lrg001: free allocated user channel structure on error
				*/ 
				if (!LSA_HOST_PTR_ARE_EQUAL(pChUsr, LSA_NULL))
				{
					//gsy_ChUsrClean(pChUsr);
				}
				/* 210710lrg001: Callback before RELEASE_PATH_INFO()
				*/ 
				gsy_CallbackRbu(pSys, pRbu, RespVal);
				if (!LSA_HOST_PTR_ARE_EQUAL(pChUsr, LSA_NULL))
				{
					//gsy_ChUsrFree(pChUsr);
				}
			}
			else
			{
				gsy_CallbackUsr(pChUsr, pRbu, RespVal);
			}
		}
		else
		{			
			RespVal = GSY_RSP_ERR_CONFIG;
			
			/* 210710lrg001: Callback and RELEASE_PATH_INFO()
			*/ 
			gsy_CallbackRbu(pSys, pRbu, RespVal);
			GSY_RELEASE_PATH_INFO(&RetVal, pSys, pCDB);
		}
	}
	else
	{
		//ERROR
	}
	
	return(RespVal);
}

/*****************************************************************************/
/* Internal access function: gsy_ChUsrGet()	                                 */
/* Get channel pointer to allocated user channel structure                   */
/*****************************************************************************/
GSY_CH_USR_PTR  GSY_LOCAL_FCT_ATTR  gsy_ChUsrGet(LSA_HANDLE_TYPE  Handle)
{
	LSA_HANDLE_TYPE	ChId  = 0;
	GSY_CH_USR_PTR	pChUsr = LSA_NULL;
	
	/* Check Handle
	*/
	if (Handle > 2*GSY_MAX_SYS_CHANNELS)
	{
		ChId = (Handle - 2*GSY_MAX_SYS_CHANNELS) - 1;
		
		if(ChId < GSY_MAX_USR_CHANNELS)
		{
			/* Build pointer to channel structure
			*/
			pChUsr = (GSY_CH_USR_PTR)(gsy_data.pChUsrList + ChId);
			
			/* Is not valid if channel is not allocated          
			*/ 
			if (pChUsr->State == GSY_CHA_STATE_FREE)
			{
				pChUsr = LSA_NULL;
			}
		}
	}
	
	return(pChUsr);
}

/*****************************************************************************/
/* External LSA access function: gsy_request()                               */
/*****************************************************************************/
LSA_VOID  GSY_UPPER_IN_FCT_ATTR  gsy_request(
																						 GSY_UPPER_RQB_PTR_TYPE  pRbu)
{            
	LSA_OPCODE_TYPE	Opcode;
	
	LSA_UINT16		RespVal = GSY_RSP_OK_ACTIVE;
	LSA_UINT32   	TraceIdx = 0;
	
	GSY_CH_SYS_PTR					pChSys  = LSA_NULL;
	GSY_CH_USR_PTR					pChUsr  = LSA_NULL;
	
	GSY_ENTER();
	Opcode = GSY_UPPER_RQB_OPCODE_GET(pRbu);
	
	GSY_UPPER_TRACE_02(GSY_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW,">>> gsy_request() pRbu=%X Opcode=%d",
										 pRbu, Opcode);
	
	{
		/* Set RQB to active
		*/
		GSY_UPPER_RQB_RESPONSE_SET(pRbu, GSY_RSP_OK_ACTIVE);
		
		/* Check Handle and get pointer to user channel structure
		*/
		pChUsr = gsy_ChUsrGet(GSY_UPPER_RQB_HANDLE_GET(pRbu));
		
		if(!LSA_HOST_PTR_ARE_EQUAL(pChUsr, LSA_NULL))
		{
			/* Request on user channel
			*/
			if (pChUsr->State != GSY_CHA_STATE_OPEN)
			{
				RespVal = GSY_RSP_ERR_SEQUENCE;
			}
			else if ((pChUsr->pChSys->RxTxNanos == 0)
							&&  (Opcode != GSY_OPC_PRM_PREPARE)
							&&  (Opcode != GSY_OPC_PRM_WRITE)
							&&  (Opcode != GSY_OPC_PRM_READ)
							&&  (Opcode != GSY_OPC_PRM_END)
							&&  (Opcode != GSY_OPC_PRM_COMMIT)
							&&  (Opcode != GSY_OPC_PRM_CHANGE_PORT)
							&&  (Opcode != GSY_OPC_PRM_INDICATION_PROVIDE)
							&&  (Opcode != GSY_OPC_DIAG_INDICATION_PROVIDE))
			{
				/* Only the above services are allowed on EDD without timestamps
				*/
				RespVal = GSY_RSP_ERR_PROTOCOL;
				GSY_ERROR_TRACE_02(TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_request() pChUsr=%X Opcode=%u not supported w/o timestamps", pChUsr, Opcode);
			}
			else
			{
				switch(Opcode)
				{
					/* Read Delay
					*/
				case GSY_OPC_GET_DELAY:
					//-OS- TODO: check if called from outside			        	RespVal = gsy_UserDelayGet(pChUsr->pChSys, pRbu);
					{
						volatile int brk = 0;
					}
					break;
					
					/* Prepare record loading 
					*/
				case GSY_OPC_PRM_PREPARE:
					RespVal = gsy_PrmPrepare(pChUsr, pRbu);
					break;
					
					/* Load record
					*/
				case GSY_OPC_PRM_WRITE:
					RespVal = gsy_PrmWrite(pChUsr, pRbu);
					break;
					
					/* Module pull/plug 
					*/
				case GSY_OPC_PRM_CHANGE_PORT:
					RespVal = gsy_PrmChangePort(pChUsr, pRbu);
					break;
					
					/* Finish loading records
					*/
				case GSY_OPC_PRM_END:
					RespVal = gsy_PrmEnd(pChUsr, pRbu);
					break;
					
					/* Activate loaded records = (Re)start of Master/Slave
					*/
				case GSY_OPC_PRM_COMMIT:
					RespVal = gsy_PrmCommit(pChUsr, pRbu);
					break;
					
					/* Read active Record or Diagnosis record
					*/
				case GSY_OPC_PRM_READ:
					RespVal = GSY_RSP_ERR_PRM_INDEX;
					if (pChUsr->pChSys->RxTxNanos != 0)
						RespVal = gsy_PrmRead(pChUsr, pRbu);
					break;
					
					/* PRM-Event Indication provide
					* 
					*/
				case GSY_OPC_PRM_INDICATION_PROVIDE:
					
					RespVal = gsy_PrmUserInd(pChUsr, pRbu);
					
					if (RespVal == GSY_RSP_OK_ACTIVE)
					{
						gsy_PutUpperQ(&pChUsr->QueuePrm, pRbu);
					}
					/* 230807lrg001: the acual RQB must not get ahead the RQBs waiting  
					*               for the lower confimation in the QueueEvent
					*/
					else if (pChUsr->QueueEvent.Count != 0)
					{
						gsy_PutUpperQ(&pChUsr->QueueEvent, pRbu);
						RespVal = GSY_RSP_OK_ACTIVE;
					}
					break;
					
					/* Diagnose-Event Indication provide
					* 
					*/
				case GSY_OPC_DIAG_INDICATION_PROVIDE:
					
					RespVal = gsy_DiagUserGet(pChUsr->pChSys, pRbu);
					
					if (GSY_RSP_OK_ACTIVE == RespVal)
						gsy_PutUpperQ(&pChUsr->pChSys->QueueDiag[GSY_UPPER_RQB_DIAGIND_SYNCID_GET(pRbu)], pRbu);
					else
						GSY_UPPER_RQB_DIAGIND_INDCNT_SET(pRbu, pChUsr->pChSys->QueueDiag[pChUsr->SyncId].Count);
					
					break;
					
					/* Data receive: check parameters (length has to be >= size of the FrameId)
					*/
				case GSY_OPC_RECV_DATA:
					RespVal = GSY_RSP_OK_ACTIVE;
					if (LSA_HOST_PTR_ARE_EQUAL(GSY_UPPER_RQB_RECV_DATA_PTR_GET(pRbu), LSA_NULL)
							||  (GSY_UPPER_RQB_RECV_DATA_LEN_GET(pRbu) < 2))
						RespVal = GSY_RSP_ERR_PARAM;
					else
						gsy_PutUpperQ(&pChUsr->QueueRcv, pRbu);
					break;
					
					/* Requests the start or stop of the PTCP time master
					*/
				case GSY_OPC_MASTER_CONTROL:
					RespVal = GSY_RSP_OK;//GSY_RSP_OK_CANCEL;//-OS- As device we are never master (check if GSY_RSP_OK_CANCEL is OK as response) gsy_MasterControl(pChUsr, pRbu);
					break;
					
				default:
					RespVal = GSY_RSP_ERR_SYNTAX;
				}
				
			}
			
			/* Callback the RQB of a finished request
			*/
			if(RespVal != GSY_RSP_OK_ACTIVE)
				gsy_CallbackUsr(pChUsr, pRbu, RespVal);
		}
		else
		{
			/* Check Handle and get system channel pointer
			*/
			pChSys = gsy_ChSysGet(GSY_UPPER_RQB_HANDLE_GET(pRbu));
			
			if(!LSA_HOST_PTR_ARE_EQUAL(pChSys, LSA_NULL))
			{
				/* Request on system channel
				*/
				TraceIdx = pChSys->TraceIdx;
				
				gsy_PutUpperQ(&pChSys->QueueUpper, pRbu);
				if (pChSys->State != GSY_CHA_STATE_OPEN)
				{
					RespVal = GSY_RSP_ERR_SEQUENCE;
				}
				else if (pChSys->RxTxNanos == 0)
				{
					/* EDD without timestamps
					*/
					RespVal = GSY_RSP_ERR_PROTOCOL;
				}
				else 
				{
					switch(Opcode)
					{
						/* Read Delay
						*/
					case GSY_OPC_GET_DELAY:
						//TODO: -OS- RespVal = gsy_UserDelayGet(pChSys, pRbu);
						{
							volatile int brk = 0;
						}
						break;
						
					default:
						RespVal = GSY_RSP_ERR_SYNTAX;
					}
				}
				/* Callback the RQB of a finished request
				*/
				if(RespVal != GSY_RSP_OK_ACTIVE)
					gsy_CallbackSys(pChSys, pRbu, RespVal);
			}
			else
			{
				/* Invalid Handle
				*/
				RespVal = GSY_RSP_ERR_REF;
				GSY_UPPER_RQB_RESPONSE_SET(pRbu, RespVal);
				//-OS- check if seen from outsede NEEDED??? GSY_RQB_ERROR(pRbu);
			}
		}
	}

		GSY_EXIT();
		return;
}

/*****************************************************************************/
/* Internal function: gsy_DiagUserInd()                                      */
/* Store diagnosis event and indicate it to the user, if one RQB is queued.  */
/*****************************************************************************/
LSA_VOID GSY_LOCAL_FCT_ATTR gsy_DiagUserInd(
																						LSA_INT					CallId,
																						GSY_CH_SYS_PTR			pChSys,
																						GSY_DRIFT_PTR			pDrift,
																						LSA_UINT32				MasterSeconds,
																						LSA_UINT32				MasterNanoseconds,
																						LSA_INT32				Offset,
																						LSA_INT32				AdjustInterval,
																						LSA_INT32				UserValue1,
																						LSA_INT32				UserValue2,
																						LSA_UINT32				SetTimeHigh,
																						LSA_UINT32				SetTimeLow,
																						LSA_UINT16				PortId)
{
	GSY_UPPER_RQB_PTR_TYPE  pRbu = LSA_NULL;
	
	LSA_UINT16				   RespVal;
	GSY_DIAG_DATA_TYPE * pDiag;
	GSY_DIAG_DATA_TYPE	 Diag;			// AP01368288 Compiler Warnings
	
	LSA_BOOL	NewValue = LSA_FALSE;
	LSA_UINT8	SyncId   = GSY_SYNCID_NONE;
	LSA_UINT8	ComClass = 0;
	LSA_UINT8	PLLState = GSY_PLL_STATE_SYNC_OFF;
	
	
	/* Rate is not calculated on delay measurement "only":
	* use local diagnosis structure
	*/
	if (LSA_HOST_PTR_ARE_EQUAL(pDrift, LSA_NULL))
	{
		GSY_MEMSET_LOCAL(&Diag, 0x00, sizeof(Diag));	// AP01368288 Compiler Warnings
		pDiag = &Diag;
	}
	else
	{
		pDiag  = &pDrift->Diag;
		SyncId = pDrift->SyncId;
		
		/* 191006lrg002: always write MAC address to diagnosis buffer
		* 030507lrg002: get MAC address from slave structure if synchronous
		*               because more than one prim. master may send at the same time 
		* 091007lrg001: from drift structure at primary Master
		*/
		if ((GSY_SYNCID_NONE != SyncId)
				&&  !LSA_HOST_PTR_ARE_EQUAL(pChSys->pChUsr, LSA_NULL))
		{
			if ((GSY_SLAVE_STATE_SYNC == pChSys->pChUsr->Slave.State)
					&&  (GSY_MASTER_STATE_PRIMARY != pChSys->pChUsr->Master.State))
				pDiag->MasterMacAddr = pChSys->pChUsr->Slave.MasterAddr;
			else
				pDiag->MasterMacAddr = pDrift->MasterAddr;
			
			if (GSY_MASTER_STATE_OFF != pChSys->pChUsr->Master.State)
			{
				/* 170408lrg002: at the Master LocalPrio is the projected  Priority1 with ActiveBit 
				*/
				pDrift->LocalPrio = pChSys->pChUsr->Master.LocalMaster.Priority1;
				
				if (GSY_MASTER_STATE_PRIMARY == pChSys->pChUsr->Master.State)
				{
					/* 170408lrg002: at the active Master is RcvSyncPrio 0
					*/
					pDrift->RcvSyncPrio = 0;
				}
			}
			else
			{
				/* 170408lrg002: LocalPrio is 0	at the slave
				* RcvSyncPrio is extracted from SyncFrame in gsy_TLVGetInfo()
				*/
				pDrift->LocalPrio = 0;
			}
			PLLState = pChSys->pChUsr->PLLData.State;
		}
		else
		{
			/* 170408lrg002: After Stop of Master/Slave
			*/
			pDrift->RcvSyncPrio = 0;
			pDrift->LocalPrio = 0;
			if (!LSA_HOST_PTR_ARE_EQUAL(pChSys->pChUsr, LSA_NULL))
			{
				PLLState = pChSys->pChUsr->PLLData.State;
			}
		}
		
		/* 161007lrg003: AP00526517 RateValid and PrimaryMaster
		*/
		if (pDiag->RateValid != pDrift->IntervalSetValid)
		{
			NewValue = LSA_TRUE;
			pDiag->RateValid = pDrift->IntervalSetValid;
		}
		//		if (pDiag->PrimaryMaster != pDrift->PrimaryMaster)	170408lrg002
		if (pDiag->LocalPrio != pDrift->LocalPrio)
		{
			NewValue = LSA_TRUE;
			pDiag->LocalPrio = pDrift->LocalPrio;
		}
		if (pDiag->RcvSyncPrio != pDrift->RcvSyncPrio)
		{
			NewValue = LSA_TRUE;
			pDiag->RcvSyncPrio = pDrift->RcvSyncPrio;
		}
	}
	
	/*  Store diagnosis event values if changed
	*/
	if (!LSA_HOST_PTR_ARE_EQUAL(pDrift, LSA_NULL)
			&&  (GSY_DRIFT_STATE_UNKNOWN != pDrift->State))
	{
		if (pDiag->DriftInterval != pDrift->AveInterval)
		{
			NewValue = LSA_TRUE;
			pDiag->DriftInterval = pDrift->AveInterval;
		}
	}
	else
	{
		if (pDiag->DriftInterval != 0)
		{
			NewValue = LSA_TRUE;
			pDiag->DriftInterval = 0;
		}
	}
	
	if (pDiag->PLLState != PLLState)
	{
		NewValue = LSA_TRUE;
		pDiag->PLLState = PLLState;
	}
	if (((Offset != 0) || (GSY_DIAG_SOURCE_OFFSET == CallId))
			&&  (pDiag->Offset != Offset))
	{
		//		NewValue = LSA_TRUE;
		pDiag->Offset = Offset;
	}
	if (((AdjustInterval != 0) || (GSY_DIAG_SOURCE_ADJUST == CallId))
			&&  (pDiag->AdjustInterval != AdjustInterval))
	{
		//		NewValue = LSA_TRUE;
		pDiag->AdjustInterval = AdjustInterval;
	}
	if ((UserValue1 != 0)
			&&  (pDiag->UserValue1 != UserValue1))
	{
		//		NewValue = LSA_TRUE;
		pDiag->UserValue1 = UserValue1;
	}
	if ((UserValue2 != 0)
			&&  (pDiag->UserValue2 != UserValue2))
	{
		//		NewValue = LSA_TRUE;
		pDiag->UserValue2 = UserValue2;
	}
	if (((MasterSeconds != 0) || (GSY_DIAG_SOURCE_OFFSET == CallId))
			&&  (pDiag->MasterSeconds != MasterSeconds))
	{
		pDiag->MasterSeconds = MasterSeconds;
	}
	if (((MasterNanoseconds != 0) || (GSY_DIAG_SOURCE_OFFSET == CallId))
			&&  (pDiag->MasterNanoseconds != MasterNanoseconds))
	{
		pDiag->MasterNanoseconds = MasterNanoseconds;
	}
	
	if (!LSA_HOST_PTR_ARE_EQUAL(pDrift, LSA_NULL) 
			&&  (NewValue
					 ||  (GSY_DIAG_SOURCE_SYNC == CallId)
						 ||  (GSY_DIAG_SOURCE_SUBDOMAIN == CallId)
							 ||  (GSY_DIAG_SOURCE_MASTER == CallId)))
	{
		if (GSY_SYNCID_NONE == SyncId)
		{
			/* Not yet received any SyncFrame
			*/
			SyncId = pDiag->SyncId;
			if ((GSY_SYNCID_NONE == SyncId)
					&&  !LSA_HOST_PTR_ARE_EQUAL(pChSys->pChUsr, LSA_NULL))	//1365226
			{
				SyncId = pChSys->pChUsr->SyncId;
			}
		}
		
		/* Callback user if a RQB is queued
		* and a diagnosis value has changed:
		*/
		if (GSY_SYNCID_NONE != SyncId)
		{
			pRbu = pChSys->QueueDiag[SyncId].pFirst;
			if (!LSA_HOST_PTR_ARE_EQUAL(pRbu, LSA_NULL))
			{
				RespVal = gsy_DiagUserGet(pChSys, pRbu);
				if (GSY_RSP_OK_ACTIVE != RespVal)
				{
					gsy_GetUpperQ(&pChSys->QueueDiag[SyncId], pRbu);
					GSY_UPPER_RQB_DIAGIND_INDCNT_SET(pRbu, pChSys->QueueDiag[SyncId].Count);
					gsy_CallbackUsr(pChSys->pChUsr, pRbu, RespVal);
				}
			}
		}
		else
		{
			GSY_ERROR_TRACE_03(pChSys->TraceIdx, LSA_TRACE_LEVEL_UNEXP,"*** gsy_DiagUserInd() pChSys=%X pDrift=%X CallId=%u: No SyncId found",
												 pChSys, pDrift, CallId);
		}
	}
	
	/*	Always update diagnosis record !
	if (NewValue || (GSY_DIAG_SOURCE_DELAY == CallId) || (GSY_DIAG_SOURCE_SET == CallId)) 
	061206lrg001 Do not write "new" diagnosis sources into DiagRecord */
	if ((GSY_DIAG_SOURCE_SYNC != CallId)
			&&  (GSY_DIAG_SOURCE_SUBDOMAIN != CallId)
				&&  (GSY_DIAG_SOURCE_MASTER != CallId))
	{
		/* 130606lrg002: not only if any diagnosis value has changed 
		*               or Delay/ExtPllOffset is written:
		* Write Diag structure++ into diagnosis record(s)
		*/
		/* Diagnosis record of the user channel[SyncId] 
		*/
		if ((GSY_SYNCID_TIME != SyncId)							//Task 1311090: 0xB001 not longer supported
				&&  !LSA_HOST_PTR_ARE_EQUAL(pChSys->pChUsr, LSA_NULL))
		{
			gsy_PrmDiagWrite(pChSys->pChUsr, pDiag, SetTimeHigh, SetTimeLow, PortId, GSY_SYNCID_CLOCK, ComClass, CallId);
		}
	}
	GSY_FUNCTION_TRACE_03(pChSys->TraceIdx, LSA_TRACE_LEVEL_CHAT,"  < gsy_DiagUserInd() NewValue=%d MasterSeconds=%u MasterNanos=%u",
												NewValue, MasterSeconds, MasterNanoseconds);
}

/*****************************************************************************/
/* Internal function: gsy_PrmEventSet()                                      */
/* Store actual Sync Event in the PRM Indication structure                   */
/*****************************************************************************/
LSA_UINT16 GSY_LOCAL_FCT_ATTR gsy_PrmEventSet(
GSY_CH_USR_PTR			pChUsr,
LSA_UINT16				SyncEvent)
{
	LSA_UINT16	PrmIndCnt = 0;

	GSY_FUNCTION_TRACE_03(pChUsr->TraceIdx, LSA_TRACE_LEVEL_CHAT,"  > gsy_PrmEventSet() pChUsr=%X Event=%d PrmFlags=0x%02x",
					pChUsr, SyncEvent, pChUsr->Event.PrmFlags);

	/* Update PRM Indication Structure
	*/

	switch (SyncEvent)
	{
	case GSY_SYNC_EVENT_DELAY_ERROR:
	case GSY_SYNC_EVENT_DELAY_OK:
	case GSY_SYNC_EVENT_WRONG_PTCP_SUBDOMAIN_ID:
			//ERROR
		break;

	case GSY_SYNC_EVENT_NO_MESSAGE_RECEIVED:
		
		pChUsr->Event.Prm.ExtChannelErrorType = GSY_PRM_EVENT_NO_MESSAGE_RECEIVED;
		pChUsr->Event.Prm.ChannelProperties   = GSY_PRM_CHANNEL_PROPERTIES|GSY_PRM_EVENT_APPEARS;
		pChUsr->Event.PrmFlags |= GSY_PRM_IND_FLAGS_TIMEOUT;
		pChUsr->Event.NewOldPrm = LSA_FALSE;
		
		if (!(pChUsr->Event.IndFlags & GSY_PRM_IND_FLAGS_TIMEOUT))
		{
			pChUsr->Event.NewPrm = LSA_TRUE;
			PrmIndCnt = 1;
			if (pChUsr->Event.IndFlags & GSY_PRM_IND_FLAGS_NOTSYNC)
			{
				/* 260207lrg001: GSY_PRM_EVENT_JITTER_OUT_OF_BOUNDARY before Timeout:
				 * now indicate "NotSync disappearing" before "Timeout appearing"
				*/
				pChUsr->Event.OldPrm = pChUsr->Event.Prm;
				pChUsr->Event.OldPrm.ExtChannelErrorType = GSY_PRM_EVENT_JITTER_OUT_OF_BOUNDARY;
				pChUsr->Event.OldPrm.ChannelProperties = GSY_PRM_CHANNEL_PROPERTIES|GSY_PRM_EVENT_DISAPPEARS;
				pChUsr->Event.NewOldPrm = LSA_TRUE;
				PrmIndCnt = 2;
				pChUsr->Event.PrmFlags &= ~GSY_PRM_IND_FLAGS_NOTSYNC;
			}
		}
		break;

	case GSY_SYNC_EVENT_JITTER_OUT_OF_BOUNDARY:
		pChUsr->Event.NewOldPrm = LSA_FALSE;
		if (pChUsr->Event.PrmFlags & GSY_PRM_IND_FLAGS_TIMEOUT)
		{
			/* Getting synchronous after Timeout or Start:
			*/
			if (pChUsr->Event.PrmFlags & GSY_PRM_IND_FLAGS_NOTSYNC)
			{
				if (pChUsr->Event.IndFlags & GSY_PRM_IND_FLAGS_TIMEOUT)
				{
					/* GSY_PRM_EVENT_JITTER_OUT_OF_BOUNDARY before Timeout:
					 * only indicate "Timeout disappearing" if it has been indicated as appearing before
					*/
					pChUsr->Event.Prm.ExtChannelErrorType = GSY_PRM_EVENT_NO_MESSAGE_RECEIVED;
					pChUsr->Event.Prm.ChannelProperties = GSY_PRM_CHANNEL_PROPERTIES|GSY_PRM_EVENT_DISAPPEARS;
					pChUsr->Event.NewPrm = LSA_TRUE;
					PrmIndCnt = 1;
				}
			}
			else
			{
				/* No GSY_PRM_EVENT_JITTER_OUT_OF_BOUNDARY before Timeout:
				 * max. indicate "Timeout disappearing" and "NotSync appearing" and set Flag
				*/
				if ((pChUsr->Event.IndFlags & GSY_PRM_IND_FLAGS_TIMEOUT)
				&&  (!(pChUsr->Event.IndFlags & GSY_PRM_IND_FLAGS_NOTSYNC)))
				{
					pChUsr->Event.OldPrm = pChUsr->Event.Prm;
					pChUsr->Event.OldPrm.ExtChannelErrorType = GSY_PRM_EVENT_NO_MESSAGE_RECEIVED;
					pChUsr->Event.OldPrm.ChannelProperties = GSY_PRM_CHANNEL_PROPERTIES|GSY_PRM_EVENT_DISAPPEARS;
					pChUsr->Event.Prm.ExtChannelErrorType = GSY_PRM_EVENT_JITTER_OUT_OF_BOUNDARY;
					pChUsr->Event.Prm.ChannelProperties = GSY_PRM_CHANNEL_PROPERTIES|GSY_PRM_EVENT_APPEARS;
					pChUsr->Event.NewOldPrm = LSA_TRUE;
					pChUsr->Event.NewPrm = LSA_TRUE;
					PrmIndCnt = 2;
				}
				else if (pChUsr->Event.IndFlags & GSY_PRM_IND_FLAGS_TIMEOUT)
				{
					pChUsr->Event.Prm.ExtChannelErrorType = GSY_PRM_EVENT_NO_MESSAGE_RECEIVED;
					pChUsr->Event.Prm.ChannelProperties = GSY_PRM_CHANNEL_PROPERTIES|GSY_PRM_EVENT_DISAPPEARS;
					pChUsr->Event.NewPrm = LSA_TRUE;
					PrmIndCnt = 1;
				}
				else if (!(pChUsr->Event.IndFlags & GSY_PRM_IND_FLAGS_NOTSYNC))
				{
					pChUsr->Event.Prm.ExtChannelErrorType = GSY_PRM_EVENT_JITTER_OUT_OF_BOUNDARY;
					pChUsr->Event.Prm.ChannelProperties = GSY_PRM_CHANNEL_PROPERTIES|GSY_PRM_EVENT_APPEARS;
					pChUsr->Event.NewPrm = LSA_TRUE;
					PrmIndCnt = 1;
				}
			}
			/* Reset Timeout PrmIndFlag
			*/
			pChUsr->Event.PrmFlags &= ~GSY_PRM_IND_FLAGS_TIMEOUT;
		}
		else if (!(pChUsr->Event.IndFlags & GSY_PRM_IND_FLAGS_NOTSYNC))
		{
			/* synchronisation lost and not indicated yet:
			*/
			pChUsr->Event.Prm.ExtChannelErrorType = GSY_PRM_EVENT_JITTER_OUT_OF_BOUNDARY;
			pChUsr->Event.Prm.ChannelProperties = GSY_PRM_CHANNEL_PROPERTIES|GSY_PRM_EVENT_APPEARS;
			pChUsr->Event.NewPrm = LSA_TRUE;
			PrmIndCnt = 1;
		}
		pChUsr->Event.PrmFlags |= GSY_PRM_IND_FLAGS_NOTSYNC;
		break;

	case GSY_SYNC_EVENT_SYNC_OK:

		/* 150306lrg001: is indicated in 3.11 as "old event disappearing" and all PrmFlags are reset
		*/
		pChUsr->Event.NewOldPrm = LSA_FALSE;
		if (pChUsr->Event.IndFlags & GSY_PRM_IND_FLAGS_NOTSYNC)
		{
			/* NotSync has been indicated...
			*/
			pChUsr->Event.Prm.ExtChannelErrorType = GSY_PRM_EVENT_JITTER_OUT_OF_BOUNDARY;
			pChUsr->Event.Prm.ChannelProperties = GSY_PRM_CHANNEL_PROPERTIES|GSY_PRM_EVENT_DISAPPEARS;
			pChUsr->Event.NewPrm = LSA_TRUE;
			PrmIndCnt = 1;
			if (pChUsr->Event.IndFlags & GSY_PRM_IND_FLAGS_TIMEOUT)
			{
				/* Timeout has been indicated also...
				*/
				pChUsr->Event.OldPrm = pChUsr->Event.Prm;
				pChUsr->Event.OldPrm.ExtChannelErrorType = GSY_PRM_EVENT_NO_MESSAGE_RECEIVED;
				pChUsr->Event.NewOldPrm = LSA_TRUE;
				PrmIndCnt++;	
			}
		}
		else if (pChUsr->Event.IndFlags & GSY_PRM_IND_FLAGS_TIMEOUT)								
		{
			/* Only Timeout has been indicated...
			*/
			pChUsr->Event.Prm.ExtChannelErrorType = GSY_PRM_EVENT_NO_MESSAGE_RECEIVED;
			pChUsr->Event.Prm.ChannelProperties = GSY_PRM_CHANNEL_PROPERTIES|GSY_PRM_EVENT_DISAPPEARS;
			pChUsr->Event.NewPrm = LSA_TRUE;
			PrmIndCnt = 1;
		}
		else if ((pChUsr->Event.IndFlags == GSY_PRM_IND_FLAGS_NONE)
		     &&	 (pChUsr->Event.PrmFlags == GSY_PRM_IND_FLAGS_NONE))
		{
			/* nothing has been indicated... (Handshake to CM [Im Westen nichts neues])
			*/
			pChUsr->Event.Prm.ExtChannelErrorType = GSY_PRM_EVENT_NONE;
			pChUsr->Event.NewPrm = LSA_TRUE;
			PrmIndCnt = 1;
		}
		pChUsr->Event.PrmFlags = GSY_PRM_IND_FLAGS_NONE;
		break;

	case GSY_SYNC_EVENT_NONE:

		/* 070706lrg001: nothing to indicate at FirstIndication
		*/
		pChUsr->Event.NewOldPrm = LSA_FALSE;
		pChUsr->Event.Prm.ExtChannelErrorType = GSY_PRM_EVENT_NONE;
		pChUsr->Event.NewPrm = LSA_TRUE;
		PrmIndCnt = 1;
		pChUsr->Event.PrmFlags = GSY_PRM_IND_FLAGS_NONE;
		break;

	default:
		GSY_ERROR_TRACE_03(pChUsr->TraceIdx, LSA_TRACE_LEVEL_UNEXP,"*** gsy_PrmEventSet() pChUsr=%X SyncId=%u: Unkwown event=%u",
						pChUsr, pChUsr->SyncId, SyncEvent);
	}

	return(PrmIndCnt);
}

/*****************************************************************************/
/* Internal function: gsy_PrmDiagWrite()                                     */
/* Write diagnosis block to diagnosis record                                 */
/*****************************************************************************/
LSA_VOID GSY_LOCAL_FCT_ATTR gsy_PrmDiagWrite(
																						 GSY_CH_USR_PTR		pChUsr,
																						 GSY_DIAG_DATA_TYPE	*pDiag,
																						 LSA_UINT32			SetTimeHigh,
																						 LSA_UINT32			SetTimeLow,
																						 LSA_UINT16			PortId,
																						 LSA_UINT8			SyncId,
																						 LSA_UINT8			ComClass,
																						 LSA_INT				CallId)
{
	LSA_UINT32	SyncDelay;
	LSA_UINT32	RecvDelay = pChUsr->Slave.Act.IndTimestamp - pChUsr->Slave.Act.Timestamp;
	LSA_UINT32	TimeStamp = 0;
	LSA_INT32	ExtPllOffset = GSY_GET_EXT_PLL_OFFSET(SyncId, pChUsr->pSys);
	GSY_DIAG_DATA_BLOCK	*pDiagBlock = &pChUsr->DiagRecord.DiagBlock[pChUsr->DiagIdxWr];
	
	GSY_FUNCTION_TRACE_03(pChUsr->TraceIdx, LSA_TRACE_LEVEL_CHAT,"  > gsy_PrmDiagWrite() pChUsr=%X SyncId=0x%02x CallId=%d",
												pChUsr, SyncId, CallId);
	
	if (GSY_HW_TYPE_SLAVE_SW == (pChUsr->pChSys->HardwareType & GSY_HW_TYPE_SLAVE_MSK))
		TimeStamp = GSY_GET_TIMESTAMP(pChUsr->pChSys->pSys);
	
	if (PortId != 0)
		pDiagBlock->LineDelay.Dword = GSY_SWAP32(pChUsr->pChSys->Port[PortId-1].SyncLineDelay);
	else
		pDiagBlock->LineDelay.Dword = 0;
	
	GSY_DIAG_TRACE_04(pChUsr->TraceIdx, LSA_TRACE_LEVEL_NOTE,"  > gsy_PrmDiagWrite(%x-%u) DiagIdxWr=%02u DiagNo=%05u",
										pChUsr, CallId, pChUsr->DiagIdxWr, pChUsr->DiagNo);
	
	if (RecvDelay > GSY_NANOS_PER_DWORD / pChUsr->pChSys->RxTxNanos)
		RecvDelay = 0xffffffff;
	else
		RecvDelay *= pChUsr->pChSys->RxTxNanos;
	
	if ((pChUsr->Slave.Act.DelaySeconds > GSY_SECONDS_PER_DWORD)
			||  ((pChUsr->Slave.Act.DelaySeconds == GSY_SECONDS_PER_DWORD)
					 &&  (pChUsr->Slave.Act.DelayNanos >= GSY_MAX_NANOS)))
		SyncDelay = 0xffffffff;
	else
	{
		SyncDelay = pChUsr->Slave.Act.DelaySeconds * GSY_NANOS_PER_SECOND;
		SyncDelay += pChUsr->Slave.Act.DelayNanos;
	}
	
	GSY_DIAG_TRACE_04(pChUsr->TraceIdx, LSA_TRACE_LEVEL_NOTE,"  - gsy_PrmDiagWrite() TimeStamp=%u SyncDelay=%u RecvDelay=%u ExtPllOffset=%d",
										TimeStamp, SyncDelay, RecvDelay, ExtPllOffset);
	
	pDiagBlock->ClpTermPHigh.Dword      = GSY_SWAP32((LSA_UINT32)((LSA_UINT64)(pChUsr->Slave.AdjustTermP) >> 32));
	pDiagBlock->ClpTermPLow.Dword       = GSY_SWAP32((LSA_UINT32)pChUsr->Slave.AdjustTermP);		
	pDiagBlock->ClpTermIHigh.Dword      = GSY_SWAP32((LSA_UINT32)((LSA_UINT64)(pChUsr->Slave.AdjustTermI) >> 32));
	pDiagBlock->ClpTermILow.Dword       = GSY_SWAP32((LSA_UINT32)pChUsr->Slave.AdjustTermI);		
	pDiagBlock->MasterSeconds.Dword     = GSY_SWAP32(pDiag->MasterSeconds);		
	pDiagBlock->MasterNanoseconds.Dword = GSY_SWAP32(pDiag->MasterNanoseconds);
	pDiagBlock->ExtPllOffset.Dword      = GSY_SWAP32((LSA_UINT32)ExtPllOffset);
	pDiagBlock->LocalOffset.Dword       = GSY_SWAP32((LSA_UINT32)(pDiag->Offset));
	pDiagBlock->AdjustInterval.Dword    = GSY_SWAP32((LSA_UINT32)(pDiag->AdjustInterval));
	pDiagBlock->DriftInterval.Dword     = GSY_SWAP32((LSA_UINT32)(pDiag->DriftInterval));
	pDiagBlock->SyncDelay.Dword         = GSY_SWAP32(SyncDelay);			
	pDiagBlock->RecvDelay.Dword         = GSY_SWAP32(RecvDelay);			
	pDiagBlock->PortId.Word             = GSY_SWAP16(PortId);				
	pDiagBlock->MasterMacAddr           = pDiag->MasterMacAddr;		
	pDiagBlock->SyncId                  = SyncId;				
	pDiagBlock->ComClass                = ComClass;	  		
	pDiagBlock->SlaveState              = pChUsr->Slave.State;	  		
	pDiagBlock->DiagSource              = (LSA_UINT8)CallId;  		
	pDiagBlock->BlockNo.Word            = GSY_SWAP16(pChUsr->DiagNo);			
	pDiagBlock->LocalTimeHigh.Dword     = GSY_SWAP32(SetTimeHigh);			
	pDiagBlock->LocalTimeLow.Dword      = GSY_SWAP32(SetTimeLow);			
	pDiagBlock->TimeStamp.Dword         = GSY_SWAP32(TimeStamp);
	
	if (GSY_SLAVE_STATE_OFF != pChUsr->Slave.State)
		pDiagBlock->SeqId.Word = GSY_SWAP16(pChUsr->Slave.Act.SeqId);
	else			
		pDiagBlock->SeqId.Word = GSY_SWAP16(pChUsr->Master.SeqId);
	
	/* Increment diagnosis block counter and index
	* Old entries above counter GSY_DIAG_BLOCK_COUNT will be overwritten
	*/
	pChUsr->DiagNo++;
	if (pChUsr->DiagCount < GSY_DIAG_BLOCK_COUNT)
		pChUsr->DiagCount++;
	
	if (++pChUsr->DiagIdxWr	>= GSY_DIAG_BLOCK_COUNT)
		pChUsr->DiagIdxWr = 0;
	
	GSY_DIAG_TRACE_04(pChUsr->TraceIdx, LSA_TRACE_LEVEL_NOTE,"--- gsy_PrmDiagWrite(%x:%u) DiagIdxWr=%02u DiagNo=%05u",
										pChUsr, SyncId, pChUsr->DiagIdxWr, pChUsr->DiagNo);
	
	GSY_FUNCTION_TRACE_03(pChUsr->TraceIdx, LSA_TRACE_LEVEL_CHAT,"  < gsy_PrmDiagWrite() pDiag=%X Port=%d ComClass=%d",
												pDiag, PortId, ComClass);
}

/*****************************************************************************/
/* Internal function: gsy_PrmUserInd()                                       */
/* Copy PRM event(s) to upper RQB                                            */
/*****************************************************************************/
LSA_UINT16 GSY_LOCAL_FCT_ATTR gsy_PrmUserInd(
																						 GSY_CH_USR_PTR			pChUsr,
																						 GSY_UPPER_RQB_PTR_TYPE  pRbu)
{
	LSA_UINT16	RespVal = GSY_RSP_OK_ACTIVE;
	GSY_UPPER_PRM_IND_PTR_TYPE	pDiag;
	
	GSY_FUNCTION_TRACE_03(pChUsr->TraceIdx, LSA_TRACE_LEVEL_CHAT,"  > gsy_PrmUserInd() pChUsr=%X Event=%d OldEvent=%d",
												pChUsr, pChUsr->Event.SyncEvent, pChUsr->Event.OldEvent);
	
	GSY_PRM_TRACE_07(pChUsr->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH,"--- gsy_PrmUserInd(%02x) Port=%u ExtChaErrType/Prop=0x%04x/0x%04x, NewOldPrm=%u NewPrm=%u QueueEventCount=%u",
									 pChUsr->SyncId, pChUsr->Event.PortId, pChUsr->Event.Prm.ExtChannelErrorType, pChUsr->Event.Prm.ChannelProperties, pChUsr->Event.NewOldPrm, pChUsr->Event.NewPrm, pChUsr->QueueEvent.Count);
	
	if (pChUsr->Event.NewPrm)
	{
		/* Fill PRM Indication RQB
		*/
		GSY_UPPER_RQB_PRMIND_PORTID_SET(pRbu, pChUsr->Event.PortId);
		if (GSY_PRM_EVENT_NONE == pChUsr->Event.Prm.ExtChannelErrorType)
		{
			/* First Event is SYNC_OK = no error -> empty Indication
			*/
			GSY_UPPER_RQB_PRMIND_ACTCNT_SET(pRbu, 0);
		}
		else
		{
			GSY_UPPER_RQB_PRMIND_ACTCNT_SET(pRbu, 1);
			pDiag = GSY_UPPER_RQB_PRMIND_PTR_GET(pRbu);
			if (pChUsr->Event.NewOldPrm)
			{
				/* More than one event: first copy old one
				*/
				GSY_UPPER_RQB_PRMIND_ACTCNT_SET(pRbu, 2);
				GSY_COPY_LOCAL_TO_UPPER_MEM(&pChUsr->Event.OldPrm, pDiag,
																		(LSA_UINT16)sizeof(GSY_PRM_IND_DATA_TYPE), pChUsr->pSys);
				pDiag++;
				pChUsr->Event.NewOldPrm = LSA_FALSE;
				
				/* 260207lrg001: Store indicated old PRM Event
				*/
				if (GSY_PRM_EVENT_NO_MESSAGE_RECEIVED == pChUsr->Event.OldPrm.ExtChannelErrorType)
				{
					if (GSY_PRM_EVENT_APPEARS & pChUsr->Event.OldPrm.ChannelProperties)
						pChUsr->Event.IndFlags |= GSY_PRM_IND_FLAGS_TIMEOUT;
					else
						pChUsr->Event.IndFlags &= ~GSY_PRM_IND_FLAGS_TIMEOUT;
				}
				else
				{
					if (GSY_PRM_EVENT_APPEARS & pChUsr->Event.OldPrm.ChannelProperties)
						pChUsr->Event.IndFlags |= GSY_PRM_IND_FLAGS_NOTSYNC;
					else
						pChUsr->Event.IndFlags &= ~GSY_PRM_IND_FLAGS_NOTSYNC;
				}
				GSY_PRM_TRACE_03(pChUsr->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH,"--- gsy_PrmUserInd(%02x) OldExtChaErrType/Prop=0x%04x/0x%04x",
												 pChUsr->SyncId, pChUsr->Event.OldPrm.ExtChannelErrorType, pChUsr->Event.OldPrm.ChannelProperties);
			}
			GSY_COPY_LOCAL_TO_UPPER_MEM(&pChUsr->Event.Prm, pDiag,
																	(LSA_UINT16)sizeof(GSY_PRM_IND_DATA_TYPE), pChUsr->pSys);
			
			/* Store indicated PRM Event
			*/
			if (GSY_PRM_EVENT_NO_MESSAGE_RECEIVED == pChUsr->Event.Prm.ExtChannelErrorType)
			{
				if (GSY_PRM_EVENT_APPEARS & pChUsr->Event.Prm.ChannelProperties)
					pChUsr->Event.IndFlags |= GSY_PRM_IND_FLAGS_TIMEOUT;
				else
					pChUsr->Event.IndFlags &= ~GSY_PRM_IND_FLAGS_TIMEOUT;
			}
			else
			{
				if (GSY_PRM_EVENT_APPEARS & pChUsr->Event.Prm.ChannelProperties)
					pChUsr->Event.IndFlags |= GSY_PRM_IND_FLAGS_NOTSYNC;
				else
					pChUsr->Event.IndFlags &= ~GSY_PRM_IND_FLAGS_NOTSYNC;
			}
		}
		RespVal = GSY_RSP_OK;
		pChUsr->Event.NewPrm = LSA_FALSE;
	}
	
	GSY_FUNCTION_TRACE_03(pChUsr->TraceIdx, LSA_TRACE_LEVEL_CHAT,"  < gsy_PrmUserInd() pRbu=%X EventSeconds=%u EventNanos=%u",
												pRbu, pChUsr->Event.Prm.Seconds, pChUsr->Event.Prm.Nanoseconds);
	return(RespVal);
}

/*****************************************************************************/
/* Internal function: gsy_PrmRead()                                          */
/* Read record                                                               */
/*****************************************************************************/
LSA_UINT16 GSY_LOCAL_FCT_ATTR gsy_PrmRead(
																					GSY_CH_USR_PTR			pChUsr,
																					GSY_UPPER_RQB_PTR_TYPE  pRbu)
{
	GSY_UPPER_SYNC_DATA_PTR	pSyncData;
	GSY_UPPER_BYTE_PTR_TYPE	pPortData;
	GSY_UPPER_BYTE_PTR_TYPE	pDiagUpper;
	GSY_DIAG_DATA_BLOCK		*pDiagRd;
	LSA_INT		IdxRd;
	LSA_UINT32	MaxCnt, ActCnt;
	LSA_UINT16	BlockLen;
	LSA_UINT16	PortIdx, PortId = GSY_UPPER_RQB_PRMRECORD_PORT_GET(pRbu);
	LSA_UINT16	RespVal = GSY_RSP_OK;
	LSA_UINT32	RecordIndex = GSY_UPPER_RQB_PRMRECORD_IDX_GET(pRbu);
	LSA_UINT32	RecordLen = GSY_UPPER_RQB_PRMRECORD_LEN_GET(pRbu);
	
	GSY_FUNCTION_TRACE_03(pChUsr->TraceIdx, LSA_TRACE_LEVEL_CHAT,"  > gsy_PrmRead() pChUsr=%X RecordIndex=0x%x SyncId=0x%02x",
												pChUsr, RecordIndex, pChUsr->SyncId);
	
	GSY_UPPER_RQB_PRMRECORD_LEN_SET(pRbu, 0);
	
	if (((GSY_PORT_DATA_IDX == RecordIndex) && (0 == PortId))
			||  ((GSY_PORT_DATA_IDX != RecordIndex) && (0 != PortId)))
	{
		GSY_ERROR_TRACE_01(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmRead() Invalid record PortId=%u",
											 PortId);
		RespVal = GSY_RSP_ERR_PRM_PORTID;
	}
	else
	{
		if (LSA_HOST_PTR_ARE_EQUAL(GSY_UPPER_RQB_PRMRECORD_PTR_GET(pRbu), LSA_NULL))
		{
			GSY_ERROR_TRACE_00(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmRead() NULL data pointer");
			RespVal = GSY_RSP_ERR_PARAM;
		}
		/* If record index is valid...
		*/
		else if (GSY_DIAG_DATA_IDX == RecordIndex)
		{
			/* Read diagnosis record
			*/
			RespVal = GSY_RSP_ERR_PRM_DATA;
			if (GSY_DIAG_BLOCK_LEN <= RecordLen)
			{
				/* Calculate count of entries to read
				*/
				MaxCnt = RecordLen / GSY_DIAG_BLOCK_LEN;
				if (MaxCnt > pChUsr->DiagCount)
					MaxCnt = pChUsr->DiagCount;
				
				/* Find read index:
				* always read the newest [MaxCnt] entries
				*/
				IdxRd = (LSA_INT32)(pChUsr->DiagIdxWr - MaxCnt);
				if (IdxRd < 0)
					IdxRd += GSY_DIAG_BLOCK_COUNT;		/* WrapAround */
				
				GSY_DIAG_TRACE_04(pChUsr->TraceIdx, LSA_TRACE_LEVEL_NOTE,"  - gsy_PrmRead(%x:%u) IdxRd=%02u MaxCnt=%02u",
													pChUsr, pChUsr->SyncId, IdxRd, MaxCnt);
				
				pDiagUpper = GSY_UPPER_RQB_PRMRECORD_PTR_GET(pRbu);
				for (ActCnt = 0; ActCnt < MaxCnt; ActCnt++)
				{
					/* Copy one diagnosis block into user buffer
					*/
					pDiagRd = &pChUsr->DiagRecord.DiagBlock[IdxRd];
					GSY_COPY_LOCAL_TO_UPPER_MEM(pDiagRd, pDiagUpper, GSY_DIAG_BLOCK_LEN, pChUsr->pSys);
					pDiagUpper += GSY_DIAG_BLOCK_LEN;
					if (++IdxRd >= GSY_DIAG_BLOCK_COUNT)
						IdxRd = 0;						/* WrapAround */
				}
				GSY_UPPER_RQB_PRMRECORD_LEN_SET(pRbu, MaxCnt * GSY_DIAG_BLOCK_LEN);
				RespVal = GSY_RSP_OK;
			}
		}
		else if ((GSY_SYNC_DATA_IDX == RecordIndex) || (GSY_SYNC_TIME_IDX == RecordIndex))	//Task 1311090
		{
			/* Record index is valid for clock or time sync record: check active SyncId
			*/
			if ((   ((GSY_SYNCID_CLOCK == pChUsr->SyncId) && (GSY_SYNC_DATA_IDX == RecordIndex)) 
					 || ((GSY_SYNCID_TIME == pChUsr->SyncId)  && (GSY_SYNC_TIME_IDX == RecordIndex)))
					)
			{
				/* If a Sync record for Master or Slave is active...
				*/
				RespVal = GSY_RSP_ERR_PRM_DATA;
				BlockLen = GSY_SWAP16(pChUsr->SyncData.BlockLength.Word) + GSY_BLOCK_TYPE_LEN_SIZE;
				
				if (BlockLen <= RecordLen)
				{
					/* Copy PDSyncData record into user buffer
					*/
					pSyncData = (GSY_UPPER_SYNC_DATA_PTR)(void*)GSY_UPPER_RQB_PRMRECORD_PTR_GET(pRbu);
					GSY_COPY_LOCAL_TO_UPPER_MEM(&pChUsr->SyncData, pSyncData, BlockLen, pChUsr->pSys);
					GSY_UPPER_RQB_PRMRECORD_LEN_SET(pRbu, BlockLen);
					RespVal = GSY_RSP_OK;
				}
				else
					GSY_ERROR_TRACE_02(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmRead() Invalid PDSyncData record length=%u BlockLen=%u",
														 RecordLen, BlockLen);
			}
		}
		else if ((GSY_SYNCID_CLOCK == pChUsr->SyncId) && (GSY_SYNC_PLL_IDX == RecordIndex)) 
		{
			/* Record index is valid for PDSyncPLL record: Parameter check
			*/
			if (pChUsr->SyncPLLActive)
			{
				/* If a PLL record for Master is active...
				*/
				RespVal = GSY_RSP_ERR_PRM_DATA;
				BlockLen = GSY_SWAP16(pChUsr->SyncPLL.BlockLength.Word) + GSY_BLOCK_TYPE_LEN_SIZE;
				if (BlockLen <= RecordLen)
				{
					/* Copy PDSyncPLL record into user buffer
					*/
					GSY_UPPER_SYNC_PLL_PTR pSyncPLL = (GSY_UPPER_SYNC_PLL_PTR)(void*)GSY_UPPER_RQB_PRMRECORD_PTR_GET(pRbu);
					GSY_COPY_LOCAL_TO_UPPER_MEM(&pChUsr->SyncPLL, pSyncPLL, BlockLen, pChUsr->pSys);
					GSY_UPPER_RQB_PRMRECORD_LEN_SET(pRbu, BlockLen);
					RespVal = GSY_RSP_OK;
				}
				else
					GSY_ERROR_TRACE_02(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmRead() Invalid PDSyncPLL record length=%u BlockLen=%u",
														 RecordLen, BlockLen);
			}
		}
		else if ((GSY_PORT_DATA_IDX == RecordIndex) && (0 != PortId))
		{
			/* Read PDPortDataAdjust record
			*/
			PortIdx = PortId - 1;
			if ((GSY_PRM_PORT_STATE_CFG == pChUsr->pChSys->Port[PortIdx].PrmDomainState)
					||  (GSY_PRM_PORT_STATE_CFG == pChUsr->pChSys->Port[PortIdx].PrmPeerState))
			{
				BlockLen = GSY_SWAP16(pChUsr->pChSys->Port[PortIdx].PrmPortDataAdjust.BlockLength.Word) + GSY_BLOCK_TYPE_LEN_SIZE;
				if (BlockLen <= RecordLen)
				{
					/* Copy PortDataAdjust header into user buffer
					*/
					GSY_UPPER_RQB_PRMRECORD_LEN_SET(pRbu, BlockLen);
					pPortData = (GSY_UPPER_BYTE_PTR_TYPE)GSY_UPPER_RQB_PRMRECORD_PTR_GET(pRbu);
					GSY_COPY_LOCAL_TO_UPPER_MEM(&pChUsr->pChSys->Port[PortIdx].PrmPortDataAdjust, pPortData, GSY_PORT_DATA_HDR_LEN, pChUsr->pSys);
					pPortData += GSY_PORT_DATA_HDR_LEN;
					if (GSY_PRM_PORT_STATE_CFG == pChUsr->pChSys->Port[PortIdx].PrmDomainState)
					{
						/* Copy DomainBoundary Subblock
						*/
						BlockLen = GSY_SWAP16(pChUsr->pChSys->Port[PortIdx].DomainBoundary.BlockLength.Word) + GSY_BLOCK_TYPE_LEN_SIZE;
						GSY_COPY_LOCAL_TO_UPPER_MEM(&pChUsr->pChSys->Port[PortIdx].DomainBoundary, pPortData, BlockLen, pChUsr->pSys);
						pPortData += BlockLen;
					}
					if (GSY_PRM_PORT_STATE_CFG == pChUsr->pChSys->Port[PortIdx].PrmPeerState)
					{
						/* Copy PeerToPeerBoundary Subblock
						*/
						BlockLen = GSY_SWAP16(pChUsr->pChSys->Port[PortIdx].PeerBoundary.BlockLength.Word) + GSY_BLOCK_TYPE_LEN_SIZE;
						GSY_COPY_LOCAL_TO_UPPER_MEM(&pChUsr->pChSys->Port[PortIdx].PeerBoundary, pPortData, BlockLen, pChUsr->pSys);
					}
					RespVal = GSY_RSP_OK;
				}
				else
				{
					GSY_ERROR_TRACE_02(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmRead() Invalid PDPortDataAdjust record length: %d BlockLen=%u",
														 RecordLen, BlockLen);
					RespVal = GSY_RSP_ERR_PRM_DATA;
				}
			}
		}
		else
		{
			GSY_ERROR_TRACE_01(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmRead() Invalid record index: 0x%x",
												 RecordIndex);
			RespVal = GSY_RSP_ERR_PRM_INDEX;
		}
	}
	
	GSY_PRM_TRACE_06(pChUsr->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH,"--- gsy_PrmRead(%02x) Port=%u RecordIndex=0x%08x, RecordLen=%u/%u RespVal=0x%04x",
									 pChUsr->SyncId, PortId, RecordIndex, RecordLen, GSY_UPPER_RQB_PRMRECORD_LEN_GET(pRbu), RespVal);
	
	GSY_FUNCTION_TRACE_03(pChUsr->TraceIdx, LSA_TRACE_LEVEL_CHAT,"  < gsy_PrmRead() pChUsr=%X RecordLen=%u RespVal=0x%x",
												pChUsr, RecordLen, RespVal);
	return(RespVal);
}

/*****************************************************************************/
/* Internal function: gsy_PrmPrepare()                                       */
/* Load default values for synchronisation in temporary (B) records          */
/*****************************************************************************/
LSA_UINT16 GSY_LOCAL_FCT_ATTR gsy_PrmPrepare(
																						 GSY_CH_USR_PTR			pChUsr,
																						 GSY_UPPER_RQB_PTR_TYPE  pRbu)
{
	LSA_UINT16	PortId, RespVal = GSY_RSP_OK;
	
	GSY_FUNCTION_TRACE_03(pChUsr->TraceIdx, LSA_TRACE_LEVEL_CHAT,"  > gsy_PrmPrepare() pChUsr=%X pRbu=%X SyncId=0x%02x",
												pChUsr, pRbu, pChUsr->SyncId);
	
	pChUsr->PrmState = GSY_PRM_STATE_PREPARE;
	
	/* Load default record (all 0 => SyncStop) into PrmData
	*/
	GSY_MEMSET_LOCAL(&pChUsr->PrmData, 0, sizeof(GSY_SYNC_DATA_RECORD));
	pChUsr->PrmData.BlockLength.Word = GSY_SWAP16(GSY_SYNC_DATA_BLOCKLEN);
	pChUsr->PrmData.BlockType.Word = GSY_SWAP16(GSY_SYNC_DATA_BLOCKTYPE);
	pChUsr->PrmData.SyncProperties.Word = GSY_SWAP16((GSY_SYNCID_NONE << 8) | GSY_SYNC_ROLE_LOCAL);
	pChUsr->PrmRecordIndexWritten = 0;
	pChUsr->PrmClockAndTimeWritten = LSA_FALSE;

	
	for (PortId = 0; PortId < pChUsr->pChSys->PortCount; PortId++)
	{
		pChUsr->pChSys->Port[PortId].PrmDomainWritten = LSA_FALSE;
		pChUsr->pChSys->Port[PortId].PrmPeerWritten = LSA_FALSE;
		
		/* 161106js001	Flag setting default values if no record is loaded
		*/
		pChUsr->pChSys->Port[PortId].PrmDomainCfg = LSA_TRUE;
	}
	
	/* 051206lrg001: Store default SendClockFactor
	*/
	pChUsr->PrmSendClock.Word = GSY_SWAP16(GSY_SEND_CLOCK_DEFAULT);
	
	/* Load default record (all 0 => no PLL sync) into PrmPLL
	*/
	GSY_MEMSET_LOCAL(&pChUsr->PrmPLL, 0, sizeof(GSY_SYNC_PLL_RECORD));
	pChUsr->PrmPLL.BlockLength.Word = GSY_SWAP16(GSY_SYNC_PLL_BLOCKLENGTH);
	pChUsr->PrmPLL.BlockType.Word = GSY_SWAP16(GSY_SYNC_PLL_BLOCKTYPE);
	pChUsr->PrmPLLWritten = LSA_FALSE;
	
	GSY_PRM_TRACE_02(pChUsr->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH,"--- gsy_PrmPrepare(%02x) RespVal=0x%x", pChUsr->SyncId, RespVal);
	
	GSY_FUNCTION_TRACE_01(pChUsr->TraceIdx, LSA_TRACE_LEVEL_CHAT,"  < gsy_PrmPrepare() pChUsr=%X",
												pChUsr);
	
	LSA_UNUSED_ARG(pRbu);
	
	return(RespVal);
}

/*****************************************************************************/
/* Internal function: gsy_PrmWritePortData()                                 */
/* Load DomainBoundaries for Forwarding configuration and PeerToPeer-        */
/* Boundaries for Delay measurement configuration in temporary (B) Record    */
/*****************************************************************************/
LSA_UINT16 GSY_LOCAL_FCT_ATTR gsy_PrmWritePortData(
																									 GSY_CH_USR_PTR			pChUsr,
																									 GSY_UPPER_RQB_PTR_TYPE  pRbu)
{
	GSY_UPPER_SUBBLOCK_PTR	pSubBlock;
	LSA_UINT8 				*pByte;
	GSY_CH_PORT				*pPort;
	GSY_UPPER_PORT_DATA_PTR pPortData = (GSY_UPPER_PORT_DATA_PTR)(void*)GSY_UPPER_RQB_PRMRECORD_PTR_GET(pRbu);
	LSA_UINT16				PortId = GSY_UPPER_RQB_PRMRECORD_PORT_GET(pRbu);
	LSA_UINT32	Offset0, Offset1, ErrOffset = GSY_PRM_ERR_OFFSET_DEFAULT;
	LSA_UINT32	RecordLen, RecordIdx;
	LSA_UINT32	DomainBoundary, DomainBoundaryIngress = 0;
	LSA_UINT16	BlockType, BlockLen, ActLen, Properties, Padding1, Padding2, Slot, Subslot, CheckLen;
	LSA_UINT8	VersionLow;
	LSA_UINT16	RespVal = GSY_RSP_ERR_PRM_PORTID;
	LSA_BOOL	HeaderWritten = LSA_FALSE;
	
	GSY_FUNCTION_TRACE_03(pChUsr->TraceIdx, LSA_TRACE_LEVEL_CHAT,"  > gsy_PrmWritePortData() pChUsr=%X Port=%d SyncId=0x%02x",
												pChUsr, PortId, pChUsr->SyncId);
	
	/* Check port ID
	*/
	if ((0 == PortId)
			|| (GSY_CH_MAX_PORTS < PortId))		
	{
		GSY_ERROR_TRACE_01(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmWritePortData() Invalid port ID=%u",
											 PortId);
	}
	else
	{
		/* Check record length
		*/
		pPort = &pChUsr->pChSys->Port[PortId-1];
		RespVal = GSY_RSP_ERR_PRM_DATA;
		RecordLen = GSY_UPPER_RQB_PRMRECORD_LEN_GET(pRbu);
		
		if ((RecordLen < GSY_BLOCK_TYPE_LEN_SIZE)
				||	(RecordLen != (LSA_UINT32)(GSY_BLOCK_TYPE_LEN_SIZE + GSY_SWAP16(pPortData->Header.BlockLength.Word))))
		{
			GSY_ERROR_TRACE_01(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmWritePortData() Invalid record length=%u",
												 RecordLen);
		}
		else
		{
			RespVal = GSY_RSP_ERR_PRM_BLOCK;
			
			/* Check block type and length
			*/
			BlockType = GSY_SWAP16(pPortData->Header.BlockType.Word);
			BlockLen = GSY_SWAP16(pPortData->Header.BlockLength.Word);
			if (BlockType != GSY_PORT_DATA_BLOCKTYPE)
			{
				ErrOffset = GSY_OFFSET_GET(pPortData, &pPortData->Header.BlockType);
				GSY_ERROR_TRACE_02(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmWritePortData() Invalid BlockType=0x%04x ErrOffset=%u",
													 BlockType, ErrOffset);
			}
			else if ((GSY_BLOCK_TYPE_LEN_SIZE + BlockLen) < GSY_PORT_DATA_HDR_LEN)
			{
				ErrOffset = GSY_OFFSET_GET(pPortData, &pPortData->Header.BlockLength);
				GSY_ERROR_TRACE_02(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmWritePortData() Invalid BlockLength=%u ErrOffset=%u",
													 BlockLen, ErrOffset);
			}
			else
			{
				/* Check block slot and subslot
				*/
				Slot = GSY_UPPER_RQB_PRMRECORD_SLOT_GET(pRbu);
				Subslot = GSY_UPPER_RQB_PRMRECORD_SUBSLOT_GET(pRbu);
				Padding1 = GSY_SWAP16(pPortData->Header.Padding1.Word);
				
				if (Padding1 != 0)
				{
					ErrOffset = GSY_OFFSET_GET(pPortData, &pPortData->Header.Padding1);
					GSY_ERROR_TRACE_02(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmWritePortData() Invalid padding=%u ErrOffset=%u",
														 Padding1, ErrOffset);
				}
				else if (Slot != GSY_SWAP16(pPortData->Header.SlotNumber.Word))
				{
					ErrOffset = GSY_OFFSET_GET(pPortData, &pPortData->Header.SlotNumber);
					GSY_ERROR_TRACE_02(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmWritePortData() Invalid RQB-slot=%u ErrOffset=%u",
														 Slot, ErrOffset);
				}
				else if (Subslot != GSY_SWAP16(pPortData->Header.SubslotNumber.Word))
				{
					ErrOffset = GSY_OFFSET_GET(pPortData, &pPortData->Header.SubslotNumber);
					GSY_ERROR_TRACE_02(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmWritePortData() Invalid RQB-subslot=%u ErrOffset=%u",
														 Subslot, ErrOffset);
				}
				else
				{
					/* 161106js001: if no DomainBoundary record present 
					* -> use default values. No error
					* An prior written record becomes invalid
					*/
					RespVal = GSY_RSP_OK;
					pPort->PrmDomainWritten = LSA_FALSE;
					pPort->PrmPeerWritten = LSA_FALSE;
					
					/* Find AdjustDomainBoundary or PeerToPeerBoundary Subblock
					*/
					pByte = (LSA_UINT8 *)&pPortData->SubBlock;
					RecordIdx = GSY_PORT_DATA_HDR_LEN;
					
					/* 161106js001: access BlockType only if Type/Len/Version of SubBlock is inside record
					*/
					while ((RespVal == GSY_RSP_OK) 
								 && 	   ((RecordIdx + GSY_BLOCK_HEADER_SIZE) < RecordLen))
					{
						pSubBlock = (GSY_UPPER_SUBBLOCK_PTR)pByte;
						BlockType = GSY_SWAP16(pSubBlock->BlockType.Word);
						BlockLen = GSY_BLOCK_TYPE_LEN_SIZE + GSY_SWAP16(pSubBlock->BlockLength.Word);
						VersionLow = pSubBlock->VersionLow;
						
						GSY_PRM_TRACE_03(pChUsr->TraceIdx, LSA_TRACE_LEVEL_CHAT,"--- gsy_PrmWritePortData() Subblock=0x%04x Length=%u VersionLow=%u",
														 BlockType, BlockLen, VersionLow);
#ifdef GSY_MESSAGE
						/* 161106js001: TBD: check record consitency. Length has to match
						*              TBD: DomainBoundary must not be located at arbitrary position
						*                   and only one subblock is allowed in record
						*/
#endif /* GSY_MESSAGE */
						
						if (GSY_DOMAIN_BOUNDARY_BLOCKTYPE == BlockType) 
						{
							/* Check rest length and block content of DomainBoundary subblock
							*/
							if (pChUsr->pChSys->RxTxNanos == 0)
							{
								/* 170107lrg001: no DomainBoundary Subblock is allowed for EDD without timestamps
								*/
								CheckLen = 0;
								RespVal = GSY_RSP_ERR_PRM_BLOCK;
								ErrOffset = GSY_OFFSET_GET(pPortData, &pSubBlock->BlockType);
								GSY_ERROR_TRACE_01(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmWritePortData() No HW support for subblock DomainBoundary ErrOffset=%u",
																	 ErrOffset);
							}
							else if (VersionLow > 1)
							{
								CheckLen = 0;
								RespVal = GSY_RSP_ERR_PRM_BLOCK;
								ErrOffset = GSY_OFFSET_GET(pPortData, &pSubBlock->VersionLow);
								GSY_ERROR_TRACE_02(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmWritePortData() DomainBoundary: Invalid VersionLow=%u ErrOffset=%u",
																	 VersionLow, ErrOffset);
							}
							else if (VersionLow == 1)
							{
								CheckLen = GSY_SUBBLOCK_HEADER_SIZE + sizeof(GSY_DOMAIN_BOUNDARY1);
							}
							else
							{
								CheckLen = GSY_SUBBLOCK_HEADER_SIZE + sizeof(GSY_DOMAIN_BOUNDARY);
							}
							if ((CheckLen != 0)
									&&  (BlockLen <= (RecordLen - RecordIdx)) && (BlockLen == CheckLen))	/* 161106js001: length must match ! */
							{
								Padding1 = GSY_SWAP16(pSubBlock->Padding1.Word);
								if (VersionLow == 1)
								{
									Padding2 = GSY_SWAP16(pSubBlock->Boundary.Domain1.Padding2.Word);
									Properties = GSY_SWAP16(pSubBlock->Boundary.Domain1.Properties.Word);
									Offset0 = GSY_OFFSET_GET(pPortData, &pSubBlock->Boundary.Domain1.Padding2);
									Offset1 = GSY_OFFSET_GET(pPortData, &pSubBlock->Boundary.Domain1.Properties);
								}
								else
								{
									Padding2 = GSY_SWAP16(pSubBlock->Boundary.Domain.Padding2.Word);
									Properties = GSY_SWAP16(pSubBlock->Boundary.Domain.Properties.Word);
									Offset0 = GSY_OFFSET_GET(pPortData, &pSubBlock->Boundary.Domain.Padding2);
									Offset1 = GSY_OFFSET_GET(pPortData, &pSubBlock->Boundary.Domain.Properties);
								}
								if (0 != Padding1)
								{
									RespVal = GSY_RSP_ERR_PRM_BLOCK;
									ErrOffset = GSY_OFFSET_GET(pPortData, &pSubBlock->Padding1);
									GSY_ERROR_TRACE_02(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmWritePortData() DomainBoundary: Invalid Padding1=%u ErrOffset=%u",
																		 Padding1, ErrOffset);
								}
								else if (0 != Padding2)
								{
									RespVal = GSY_RSP_ERR_PRM_BLOCK;
									ErrOffset = Offset0;
									GSY_ERROR_TRACE_02(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmWritePortData() DomainBoundary: Invalid Padding2=%u ErrOffset=%u",
																		 Padding2, ErrOffset);
								}
								else if (0 != Properties)
								{
									RespVal = GSY_RSP_ERR_PRM_BLOCK;
									ErrOffset = Offset1;
									GSY_ERROR_TRACE_02(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmWritePortData() DomainBoundary: Invalid Properties=0x%04x ErrOffset=%u",
																		 Properties, ErrOffset);
								}
								/* 300310lrg001: check Boundaries for supported SyncIds
								*/
								if (VersionLow == 1)
								{
									DomainBoundaryIngress = GSY_SWAP32(pPort->PrmDomainBoundary.Boundary.Domain1.DomainBoundaryIngress.Dword);
									DomainBoundary = GSY_SWAP32(pPort->PrmDomainBoundary.Boundary.Domain1.DomainBoundaryEgress.Dword);
									Offset0 = GSY_OFFSET_GET(pPortData, &pSubBlock->Boundary.Domain1.DomainBoundaryEgress);
								}
								else
								{
									DomainBoundary = GSY_SWAP32(pPort->PrmDomainBoundary.Boundary.Domain.DomainBoundary.Dword);
									Offset0 = GSY_OFFSET_GET(pPortData, &pSubBlock->Boundary.Domain.DomainBoundary);
								}
								if (DomainBoundary > GSY_SYNC_ID_SUPPORTED)
								{
									RespVal = GSY_RSP_ERR_PRM_BLOCK;
									ErrOffset = Offset0;
									GSY_ERROR_TRACE_03(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmWritePortData() DomainBoundary=0x%08x > GSY_SYNC_ID_SUPPORTED=0x%08x ErrOffset=%u",
																		 DomainBoundary, GSY_SYNC_ID_SUPPORTED, ErrOffset);
								}
								else if ((VersionLow == 1) && (DomainBoundaryIngress > GSY_SYNC_ID_SUPPORTED))
								{
									RespVal = GSY_RSP_ERR_PRM_BLOCK;
									ErrOffset = GSY_OFFSET_GET(pPortData, &pSubBlock->Boundary.Domain1.DomainBoundaryIngress);
									GSY_ERROR_TRACE_03(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmWritePortData() DomainBoundaryIngress=0x%08x  > GSY_SYNC_ID_SUPPORTED=0x%08x ErrOffset=%u",
																		 DomainBoundaryIngress, GSY_SYNC_ID_SUPPORTED, ErrOffset);
								}
								else
								{
									/* Copy PortDataAdjust header and DomainBoundary Subblock into Port data and update PrmState
									*/
									if (!HeaderWritten)
									{
										pPort->PrmPortDataAdjust = pPortData->Header;
										ActLen = GSY_PORT_DATA_HDR_LEN - GSY_BLOCK_TYPE_LEN_SIZE;
										HeaderWritten = LSA_TRUE;
									}
									else
									{
										ActLen = GSY_SWAP16(pPort->PrmPortDataAdjust.BlockLength.Word);
									}
									pPort->PrmPortDataAdjust.BlockLength.Word = GSY_SWAP16(ActLen + BlockLen);
									pPort->PrmDomainBoundary = *pSubBlock;
									pPort->PrmDomainWritten = LSA_TRUE;
									pChUsr->PrmState = GSY_PRM_STATE_WRITE;
									RespVal = GSY_RSP_OK;
								}
							}
							else if (CheckLen != 0)
							{
								/* 161106js001: Error Trace
								*/
								RespVal = GSY_RSP_ERR_PRM_BLOCK;
								ErrOffset = GSY_OFFSET_GET(pPortData, &pSubBlock->BlockLength);
							}
						}
						else if (GSY_PEER_BOUNDARY_BLOCKTYPE == BlockType) 
						{
							/* Check rest length and block content of PeerToPeerBoundary subblock
							*/
							if ((BlockLen > (RecordLen - RecordIdx))
									||  (BlockLen != (GSY_SUBBLOCK_HEADER_SIZE + sizeof(GSY_PEER_BOUNDARY))))
							{
								/* 161106js001: Error Trace
								*/
								RespVal = GSY_RSP_ERR_PRM_BLOCK;
								ErrOffset = GSY_OFFSET_GET(pPortData, &pSubBlock->BlockLength);
							}
							else
							{
								Padding1 = GSY_SWAP16(pSubBlock->Padding1.Word);
								Padding2 = GSY_SWAP16(pSubBlock->Boundary.Peer.Padding2.Word);
								Properties = GSY_SWAP16(pSubBlock->Boundary.Peer.Properties.Word);
								
								if (0 != Padding1)
								{
									RespVal = GSY_RSP_ERR_PRM_BLOCK;
									ErrOffset = GSY_OFFSET_GET(pPortData, &pSubBlock->Padding1);
								}
								else if (0 != Padding2)
								{
									RespVal = GSY_RSP_ERR_PRM_BLOCK;
									ErrOffset = GSY_OFFSET_GET(pPortData, &pSubBlock->Boundary.Peer.Padding2);
								}
								else if (0 != Properties)
								{
									RespVal = GSY_RSP_ERR_PRM_BLOCK;
									ErrOffset = GSY_OFFSET_GET(pPortData, &pSubBlock->Boundary.Peer.Properties);
								}
								/* 190608lrg001: Reject record on EDDS only if PTCP boundary (Bit 1) is set
								*/
								else if ((pChUsr->pChSys->RxTxNanos == 0)
												 &&  ((GSY_SWAP32(pSubBlock->Boundary.Peer.PeerBoundary.Dword) & 2) == 2))
								{
									/* No PeerToPeerBoundary Subblock with PTCP boundary is allowed for EDD without timestamp (EDDS)
									*/
									RespVal = GSY_RSP_ERR_PRM_BLOCK;
									ErrOffset = GSY_OFFSET_GET(pPortData, &pSubBlock->Boundary.Peer.PeerBoundary);
								}
								else
								{
									/* Copy PortDataAdjust header and PeerBoundary subblock into Port data and update PrmState
									*/
									if (!HeaderWritten)
									{
										pPort->PrmPortDataAdjust = pPortData->Header;
										ActLen = GSY_PORT_DATA_HDR_LEN - GSY_BLOCK_TYPE_LEN_SIZE;
										HeaderWritten = LSA_TRUE;
									}
									else
									{
										ActLen = GSY_SWAP16(pPort->PrmPortDataAdjust.BlockLength.Word);
									}
									pPort->PrmPortDataAdjust.BlockLength.Word = GSY_SWAP16(ActLen + BlockLen);
									pPort->PrmPeerBoundary = *pSubBlock;
									pPort->PrmPeerWritten = LSA_TRUE;
									pChUsr->PrmState = GSY_PRM_STATE_WRITE;
									RespVal = GSY_RSP_OK;
								}
							}
						}
						
						/* Next Subblock...
						*/
						RecordIdx += BlockLen;
						pByte += BlockLen;
						
					}	/* while */
					
					if (RespVal == GSY_RSP_OK)
					{
						pChUsr->PrmState = GSY_PRM_STATE_WRITE;
					}
				}
			}
		}
	}
	
	if (GSY_RSP_ERR_PRM_BLOCK == RespVal)
	{
		GSY_UPPER_RQB_PRMWRITE_ERROFFSET_SET(pRbu, ErrOffset);
	}
	
	return(RespVal);
}

/*****************************************************************************/
/* Internal function: gsy_PrmWriteSendClock()                                */
/* Store SendClockFactor for consistency check at gsy_PrmEnd()               */
/*****************************************************************************/
static LSA_UINT16 GSY_LOCAL_FCT_ATTR gsy_PrmWriteSendClock(
																													 GSY_CH_USR_PTR			pChUsr,
																													 GSY_UPPER_RQB_PTR_TYPE  pRbu)
{
	GSY_UPPER_SEND_CLOCK_PTR pRecordData = (GSY_UPPER_SEND_CLOCK_PTR)(void*)GSY_UPPER_RQB_PRMRECORD_PTR_GET(pRbu);
	LSA_UINT32	ErrOffset = GSY_PRM_ERR_OFFSET_DEFAULT;
	LSA_UINT32	RecordLen;
	LSA_UINT16	BlockType, BlockLen;
	LSA_UINT16	PortId = GSY_UPPER_RQB_PRMRECORD_PORT_GET(pRbu);
	LSA_UINT16	RespVal = GSY_RSP_ERR_PRM_PORTID;
	
	if (0 != PortId)
	{
		//ERROR
	}
	else
	{
		RespVal = GSY_RSP_ERR_PRM_DATA;
		RecordLen = GSY_UPPER_RQB_PRMRECORD_LEN_GET(pRbu);
		
		if ((RecordLen < GSY_BLOCK_TYPE_LEN_SIZE)
				||	(RecordLen != (LSA_UINT32)(GSY_BLOCK_TYPE_LEN_SIZE + GSY_SWAP16(pRecordData->BlockLength.Word))))
		{
			//ERROR
		}
		else
		{
			RespVal = GSY_RSP_ERR_PRM_BLOCK;
			BlockType = GSY_SWAP16(pRecordData->BlockType.Word);
			BlockLen = GSY_BLOCK_TYPE_LEN_SIZE + GSY_SWAP16(pRecordData->BlockLength.Word);
			
			if (BlockType != GSY_SEND_CLOCK_BLOCKTYPE)
			{
				ErrOffset = GSY_OFFSET_GET(pRecordData, &pRecordData->BlockType);
			}
			else if (BlockLen < sizeof(GSY_SEND_CLOCK_RECORD))
			{
				ErrOffset = GSY_OFFSET_GET(pRecordData, &pRecordData->BlockLength);
			}
			else
			{
				/* 051206lrg001: store SendClockFactor and set PrmState
				*/
				pChUsr->PrmSendClock = pRecordData->SendClockFactor;
				pChUsr->PrmState = GSY_PRM_STATE_WRITE;
				RespVal = GSY_RSP_OK;
			}
		}
	}
	
	if (GSY_RSP_ERR_PRM_BLOCK == RespVal)
	{
		GSY_UPPER_RQB_PRMWRITE_ERROFFSET_SET(pRbu, ErrOffset);
	}
	
	GSY_FUNCTION_TRACE_03(pChUsr->TraceIdx, LSA_TRACE_LEVEL_CHAT,"  < gsy_PrmWriteSendClock() pRbu=%X SendClock=%u RespVal=0x%x",
												pRbu, GSY_SWAP16(pChUsr->PrmSendClock.Word), RespVal);
	return(RespVal);
}

/*****************************************************************************/
/* Internal function: gsy_PrmWriteSyncPLL()                                  */
/* Store SyncPLL record for consistency check at gsy_PrmEnd()                */
/*****************************************************************************/
static LSA_UINT16 GSY_LOCAL_FCT_ATTR gsy_PrmWriteSyncPLL(
																												 GSY_CH_USR_PTR			pChUsr,
																												 GSY_UPPER_RQB_PTR_TYPE  pRbu)
{
	GSY_UPPER_SYNC_PLL_PTR	pRecordData = (GSY_UPPER_SYNC_PLL_PTR)(void*)GSY_UPPER_RQB_PRMRECORD_PTR_GET(pRbu);
	LSA_UINT16	PortId    = GSY_UPPER_RQB_PRMRECORD_PORT_GET(pRbu);
	LSA_UINT32	ErrOffset = GSY_PRM_ERR_OFFSET_DEFAULT;
	LSA_UINT16	RespVal   = GSY_RSP_ERR_PRM_PORTID;
	
	LSA_UINT32	RecordLen, PLLWindow;
	LSA_UINT16	BlockType, BlockLen, ReductionRatio, PLLHWDelay, ControlInterval, SetWaitCount, Reserved;
	
	LSA_UNUSED_ARG(SetWaitCount)
	
	if (0 != PortId)
	{
		//ERROR
	}
	else
	{
		RespVal = GSY_RSP_ERR_PRM_DATA;
		RecordLen = GSY_UPPER_RQB_PRMRECORD_LEN_GET(pRbu);
		
		if ((RecordLen < GSY_BLOCK_TYPE_LEN_SIZE)
				||	(RecordLen != (LSA_UINT32)(GSY_BLOCK_TYPE_LEN_SIZE + GSY_SWAP16(pRecordData->BlockLength.Word))))
		{
			//ERROR
		}
		else
		{
			/* check record block
			*/
			RespVal   = GSY_RSP_ERR_PRM_BLOCK;
			BlockType = GSY_SWAP16(pRecordData->BlockType.Word);
			BlockLen  = GSY_SWAP16(pRecordData->BlockLength.Word);
			
			if (BlockType != GSY_SYNC_PLL_BLOCKTYPE)
			{
				ErrOffset = GSY_OFFSET_GET(pRecordData, &pRecordData->BlockType);
			}
			else if (BlockLen != GSY_SYNC_PLL_BLOCKLENGTH)
			{
				ErrOffset = GSY_OFFSET_GET(pRecordData, &pRecordData->BlockLength);
			}
			else if (pRecordData->VersionHigh != GSY_SYNC_PLL_VERSIONHIGH)
			{
				ErrOffset = GSY_OFFSET_GET(pRecordData, &pRecordData->VersionHigh);
			}
			else if (pRecordData->VersionLow != GSY_SYNC_PLL_VERSIONLOW)
			{
				ErrOffset = GSY_OFFSET_GET(pRecordData, &pRecordData->VersionLow);
			}
			else
			{
				/* check record data
				*/
				ReductionRatio  = GSY_SWAP16(pRecordData->ReductionRatio.Word);
				PLLHWDelay      = GSY_SWAP16(pRecordData->PLLHWDelay.Word);
				ControlInterval = GSY_SWAP16(pRecordData->ControlInterval.Word);
				PLLWindow       = GSY_SWAP32(pRecordData->PLLWindow.Dword);
				SetWaitCount    = GSY_SWAP16(pRecordData->SetWaitCount.Word);
				Reserved        = GSY_SWAP16(pRecordData->Reserved.Word);
				
				if ((ReductionRatio != 1) && (ReductionRatio != 2) && (ReductionRatio != 4) && (ReductionRatio != 8) && (ReductionRatio != 16))
				{
					ErrOffset = GSY_OFFSET_GET(pRecordData, &pRecordData->ReductionRatio);
				}
				else if ((PLLHWDelay < GSY_SYNC_PLL_DELAYMIN) || (PLLHWDelay > GSY_SYNC_PLL_DELAYMAX))
				{
					ErrOffset = GSY_OFFSET_GET(pRecordData, &pRecordData->PLLHWDelay);
				}
				else if (ControlInterval == 0)
				{
					ErrOffset = GSY_OFFSET_GET(pRecordData, &pRecordData->ControlInterval);
				}
				else if ((PLLWindow < 1) || (PLLWindow > GSY_SYNC_PLLWINDOW_MAX))
				{
					ErrOffset = GSY_OFFSET_GET(pRecordData, &pRecordData->PLLWindow);
				}
				else if (Reserved != 0)
				{
					ErrOffset = GSY_OFFSET_GET(pRecordData, &pRecordData->Reserved);
				}
				else
				{
					/* store record data and set PrmState
					*/
					pChUsr->PrmPLL = *pRecordData;
					pChUsr->PrmPLLWritten = LSA_TRUE;
					pChUsr->PrmState = GSY_PRM_STATE_WRITE;
					RespVal = GSY_RSP_OK;
				}
			}
		}
	}
	if (GSY_RSP_ERR_PRM_BLOCK == RespVal)
	{
		GSY_UPPER_RQB_PRMWRITE_ERROFFSET_SET(pRbu, ErrOffset);
	}
	
	return(RespVal);
}

/*****************************************************************************/
/* Internal function: gsy_PrmValidName()                                     */
/* Check Subdomain name in PDSyncDAta Record like DCP station Name (CM)      */
/*****************************************************************************/
static LSA_BOOL  GSY_LOCAL_FCT_ATTR gsy_is_valid_port_name (
																														LSA_UINT16 length,
																														const GSY_UPPER_BYTE_PTR_TYPE name) 
{
	/*
	* port-names have the following forms (see the definition of station-name below)
	*
	* "port-xyz" with x, y, z = 0...9
	* "port-xyz-abcde" with x, y, z, a, b, c, d, e = 0...9
	*/
	
	if(
		  length >= 8
			&& (name[0] == 'p' || name[0] == 'P') /* note: case insensitive, see caller */
			&& (name[1] == 'o' || name[1] == 'O')
			&& (name[2] == 'r' || name[2] == 'R')
			&& (name[3] == 't' || name[3] == 'T')
			&& name[4] == '-'
			&& (name[5] >= '0' && name[5] <= '9')
			&& (name[6] >= '0' && name[6] <= '9')
			&& (name[7] >= '0' && name[7] <= '9')
		) 
	{											 
		 if( length == 8 ) {
			 return LSA_TRUE; /* matches with "port-xyz" with x, y, z = 0...9 */
		 }
													 
		 if(
				length == 14
				&& name[8] == '-'
				&& (name[ 9] >= '0' && name[ 9] <= '9')
				&& (name[10] >= '0' && name[10] <= '9')
				&& (name[11] >= '0' && name[11] <= '9')
				&& (name[12] >= '0' && name[12] <= '9')
				&& (name[13] >= '0' && name[13] <= '9')
			 ) {
						return LSA_TRUE; /* matches with "port-xyz-abcde" with x, y, z, a, b, c, d, e = 0...9 */
				 }
	 }
	
	return LSA_FALSE; /* not a port-name */
}

/*****************************************************************************/
LSA_BOOL  GSY_LOCAL_FCT_ATTR gsy_PrmValidName (
																							 GSY_CH_USR_PTR			pChUsr,
																							 LSA_UINT16 				length,
																							 GSY_UPPER_BYTE_PTR_TYPE name) 
{
	LSA_UINT16 idx;
	LSA_UINT16 lbl;
	LSA_UINT16 nr_of_labels;
	LSA_BOOL looks_like_ipaddr;
	
	/* Ascii check
	*/
	if (!('-' == 0x2d && '.' == 0x2e && '0' == 0x30 && 'a' == 0x61 && 'A' == 0x41 && '0' < '9' && 'a' < 'z' && 'A' < 'Z'))  //lint !e506 !e774 Constant Boolean within 'if' always evaluates to False, define-based behaviour, HM 20.05.2016
	{
		GSY_ERROR_TRACE_00(pChUsr->TraceIdx, LSA_TRACE_LEVEL_FATAL,"*** gsy_PrmValidName() not ASCII");
		gsy_ErrorInternal(GSY_MODULE_ID, __LINE__, GSY_ERR_FATAL, LSA_NULL, 0);
	}
	
	/*
	* a station-name has the following syntax(see AP00216830):
	* 1 or more labels, separated by [.]
	* total length is 1-240
	* label-length is 1-63
	* labels consist of [a-z0-9-]
	* labels do not start with [-]
	* labels do not end with [-]
	* the first (leftmost) label does not match with "port-xyz" with x, y, z = 0...9
	* the first (leftmost) label does not match with "port-xyz-abcde" with x, y, z, a, b, c, d, e = 0...9
	* station-names do not have the form n.n.n.n, n = 0...999
	* labels do only start with 'xn-' if RFC 3490 is applied
	*/
	
	if(!(length >= 1 && length <= 240)) 
	{
		return LSA_FALSE; /* station-name is too short or too long */
	}
	
	if(LSA_HOST_PTR_ARE_EQUAL(name, LSA_NULL)) {
		GSY_ERROR_TRACE_00(pChUsr->TraceIdx, LSA_TRACE_LEVEL_FATAL,"*** gsy_PrmValidName() name=LSA_NULL");
		//		gsy_ErrorInternal(GSY_MODULE_ID, __LINE__, GSY_ERR_FATAL, LSA_NULL, 0);
		return LSA_FALSE;       //lint !e527 Unreachable code, if gsy_ErrorInternal() is an exit (but this must not be), HM 10.06.2016
	}
	
	/***/
	
	nr_of_labels      = 0;
	looks_like_ipaddr = LSA_TRUE;
	lbl               = 0; /* start of label */
	
	for( idx = 0; idx <= length; ++idx ) {
		
		LSA_UINT8 ch;
		
		if( idx < length ) 
		{
			ch = name[idx];
		}
		else 
		{
			ch = '.'; /* fake a dot at the end */
		}
		
		/***/
		
		if( ch == '.' ) 
		{	
			LSA_UINT16 len = idx - lbl;
			
			if( len < 1 || len > 63 ) {
				return LSA_FALSE; /* the label is too short or too long */
			}
			
			if (idx == 0)
			{
				//ERROR
			}
			
			if( name[idx - 1] == '-' ) 
			{
				return LSA_FALSE; /* the label ends with '-' */
			}
			
			if( len > 3 ) 
			{
				/* note: port-names are longer than 3 chars */
				
				looks_like_ipaddr = LSA_FALSE; /* the label has more than 3 characters */
				
				if(nr_of_labels == 0) {
					
					if( gsy_is_valid_port_name(len, &name[lbl]) ) {
						return LSA_FALSE; /* the first (leftmost) label matches a port-name */
					}
				}
			}
			
			nr_of_labels += 1;
			lbl           = idx + 1; /* start of the next label */
			
			continue;
		}
		
		if( ch >= '0' && ch <= '9' ) 
		{
			continue;
		}
		
		looks_like_ipaddr = LSA_FALSE; /* the label contains not only digits */
		
		if( ch >= 'a' && ch <= 'z' ) 
		{
			continue;
		}
		
		if( ch >= 'A' && ch <= 'Z' ) 
		{
			/* for backwards-compatibility: allow upper-case here, convert in cm_station_name_copy() */
			continue;
		}
		
		if( idx == lbl ) 
		{
			return LSA_FALSE; /* the label does not start with a letter or digit */
		}
		
		if( ch == '-' ) 
		{
			continue;
		}
		
		return LSA_FALSE; /* the label contains an illegal char */
	}
	
	if( looks_like_ipaddr && nr_of_labels == 4 ) 
	{
		return LSA_FALSE; /* station-name looks like an ip-addr */
	}
	
	LSA_UNUSED_ARG(pChUsr);
	return LSA_TRUE; /* ok */
}

/*****************************************************************************/
/* Internal function: gsy_PrmWriteSyncData()                                 */
/* Write PDSyncData record to temporary Record (B)                           */
/*****************************************************************************/
static LSA_UINT16 GSY_LOCAL_FCT_ATTR gsy_PrmWriteSyncData(GSY_CH_USR_PTR pChUsr, GSY_UPPER_RQB_PTR_TYPE  pRbu)
{
	LSA_UINT32	ErrOffset = GSY_PRM_ERR_OFFSET_DEFAULT;
	LSA_UINT32	HWType, HWMaster, SendInterval, PLLWindow, ResIntervalBegin, ResIntervalEnd, RecordLen, RecordIndex;
	LSA_UINT16	BlockType, Padding, SendClockFactor, SyncTimeout, SyncTakeover, PortId, SyncProperties, MasterStartupTime;
	LSA_UINT8	  SyncRole, SyncId, MasterPrio, MasterLevel, MasterPrio1, MasterPrio2;
	LSA_UINT16	SyncDataLen = 0, CopyLen = 0, RespVal = GSY_RSP_ERR_SEQUENCE;
	GSY_UPPER_SYNC_DATA_PTR pSyncData = (GSY_UPPER_SYNC_DATA_PTR)(void*)GSY_UPPER_RQB_PRMRECORD_PTR_GET(pRbu);
	
	/* PortId check
	*/
	PortId = GSY_UPPER_RQB_PRMRECORD_PORT_GET(pRbu);
	RecordIndex = GSY_UPPER_RQB_PRMRECORD_IDX_GET(pRbu);
	
	if (0 != PortId)
	{
		RespVal = GSY_RSP_ERR_PRM_PORTID;
	}
	else
	{
		/* RecordLen check
		*/
		RespVal   = GSY_RSP_ERR_PRM_DATA;
		RecordLen = GSY_UPPER_RQB_PRMRECORD_LEN_GET(pRbu);
		
		if ((RecordLen < GSY_BLOCK_TYPE_LEN_SIZE)
				||	(RecordLen != (LSA_UINT32)(GSY_BLOCK_TYPE_LEN_SIZE + GSY_SWAP16(pSyncData->BlockLength.Word))))
		{
			//ERROR
		}
		else
		{
			/* SyncData check part 1
			*/
			RespVal     = GSY_RSP_ERR_PRM_BLOCK;
			BlockType   = GSY_SWAP16(pSyncData->BlockType.Word);
			SyncDataLen = GSY_SWAP16(pSyncData->BlockLength.Word) + GSY_BLOCK_TYPE_LEN_SIZE;
			Padding     = GSY_SWAP16(pSyncData->Padding1.Word);
			
			if (   ((GSY_SYNC_DATA_IDX == RecordIndex) && (GSY_SYNC_DATA_BLOCKTYPE != BlockType))
					|| ((GSY_SYNC_TIME_IDX == RecordIndex) && (GSY_SYNC_TIME_BLOCKTYPE != BlockType)))
			{
				ErrOffset = GSY_OFFSET_GET(pSyncData, &pSyncData->BlockType);
			}
			else if ((SyncDataLen < (GSY_SYNC_DATA_LEN + GSY_SYNC_DATA_NAMELEN_MIN))
					 ||  (SyncDataLen > sizeof(GSY_SYNC_DATA_RECORD))
					 ||  (SyncDataLen > RecordLen))
			{
				ErrOffset = GSY_OFFSET_GET(pSyncData, &pSyncData->BlockLength);
			}
			else if (1 != pSyncData->VersionHigh)
			{
				ErrOffset = GSY_OFFSET_GET(pSyncData, &pSyncData->VersionHigh);
			}
			else if (2 != pSyncData->VersionLow)
			{
				ErrOffset = GSY_OFFSET_GET(pSyncData, &pSyncData->VersionLow);
			}
			else if (0 != Padding)
			{
				ErrOffset = GSY_OFFSET_GET(pSyncData, &pSyncData->Padding1);
			}
			/* 290107lrg001: PDSyncData with SubdomainName since V1.1
			* 170707lrg002: only V1.2 supported for RTSync
			*/
			else if ((GSY_SYNC_DATA_NAMELEN_MIN > pSyncData->LengthSubdomainName)
					 ||  (GSY_SYNC_DATA_NAMELEN_MAX < pSyncData->LengthSubdomainName)
					 ||  (SyncDataLen < (GSY_SYNC_DATA_LEN + pSyncData->LengthSubdomainName)))
			{
				ErrOffset = GSY_OFFSET_GET(pSyncData, &pSyncData->LengthSubdomainName);
			}
			else if (!gsy_PrmValidName(pChUsr, pSyncData->LengthSubdomainName, pSyncData->SubdomainName))
			{
				ErrOffset = GSY_OFFSET_GET(pSyncData, pSyncData->SubdomainName);
			}
			else
			{
				CopyLen = SyncDataLen;
			}
		}
	}
	
	if (CopyLen != 0)
	{
		/* SyncData check part 2
		*/
		ResIntervalBegin = GSY_SWAP32(pSyncData->ResIntervalBegin.Dword);
		ResIntervalEnd   = GSY_SWAP32(pSyncData->ResIntervalEnd.Dword);
		PLLWindow        = GSY_SWAP32(pSyncData->PLLWindow.Dword);
		SendInterval     = GSY_SWAP32(pSyncData->SyncSendFactor.Dword);
		SendClockFactor  = GSY_SWAP16(pSyncData->SendClockFactor.Word);
		SyncTimeout      = GSY_SWAP16(pSyncData->TimeoutFactor.Word);
		
		/*
		PTCP Takeover 1Timeout Factor
		
		This attribute contains a timeout value for detection the loss of sync messages from the
		current sync master. The sync slave shall try to find a new sync master after the expiration
		of this time. The PTCP takeover timeout value shall be in the range from 32 ms to 16 352
		ms for Sync ID = CLOCK and in the range from 32 ms to 2 759 400 000 ms for Sync ID =
		TIME.
		Attribute type: Unsigned32
		Time Base: Sync Send Factor * 31,25 s
		Allowed values: 1 to 511
		Default value: 0x00000003 for Sync Master, 0x00000002 for Sync Slave
		*/
		
		SyncTakeover      = GSY_SWAP16(pSyncData->TakeoverFactor.Word);
		MasterStartupTime = GSY_SWAP16(pSyncData->MasterStartupTime.Word);	// 290807lrg001
		SyncProperties    = GSY_SWAP16(pSyncData->SyncProperties.Word);
		SyncRole    = (LSA_UINT8)(SyncProperties & 0x03);
		SyncId      = ((LSA_UINT8)(SyncProperties >> 8)) & GSY_SYNCID_MASK;
		HWMaster    = pChUsr->pChSys->HardwareType & GSY_HW_TYPE_MASTER_MSK;	// 240409lrg002: HW master or SyncSend supportet?
		HWType      = pChUsr->pChSys->HardwareType & GSY_HW_TYPE_MSK;			    // TSyncOverPTCP
		MasterPrio1 = pSyncData->MasterPriority1;
		MasterLevel = GSY_SYNC_PRIO1_LEVEL(MasterPrio1);					// 170408lrg001
		MasterPrio  = GSY_SYNC_PRIO1_PRIO(MasterPrio1);						// 170408lrg001
		MasterPrio2 = pSyncData->MasterPriority2;
		
		if (0 != ResIntervalBegin)
		{
			ErrOffset = GSY_OFFSET_GET(pSyncData, &pSyncData->ResIntervalBegin);
		}
		else if ((GSY_SYNC_TIME_IDX == RecordIndex) && (0 != ResIntervalEnd))
		{
			ErrOffset = GSY_OFFSET_GET(pSyncData, &pSyncData->ResIntervalEnd);
		}
		else if (GSY_SYNC_PLLWINDOW_MAX < PLLWindow)
		{
			ErrOffset = GSY_OFFSET_GET(pSyncData, &pSyncData->PLLWindow);
		}
		else if ((GSY_SYNC_SENDFACTOR_MIN > SendInterval) || (GSY_SYNC_SENDFACTOR_MAX < SendInterval))
		{
			ErrOffset = GSY_OFFSET_GET(pSyncData, &pSyncData->SyncSendFactor);
		}
		else if ((GSY_SYNC_TIME_IDX == RecordIndex) && (0 != SendClockFactor))
		{
			ErrOffset = GSY_OFFSET_GET(pSyncData, &pSyncData->SendClockFactor);
		}
#if GSY_SYNC_TIMEOUT_MIN != 0
		else if ((GSY_SYNC_TIMEOUT_MAX < SyncTimeout) && (GSY_SYNC_TIMEOUT_MIN > SyncTimeout))
#else
		else if (GSY_SYNC_TIMEOUT_MAX < SyncTimeout)
#endif
		{
			ErrOffset = GSY_OFFSET_GET(pSyncData, &pSyncData->TimeoutFactor);
		}
#if GSY_SYNC_TAKEOVER_MIN != 0
		else if (((GSY_SYNC_TAKEOVER_MAX < SyncTakeover) && (GSY_SYNC_TAKEOVER_MIN > SyncTakeover))
#else
		else if ((GSY_SYNC_TAKEOVER_MAX < SyncTakeover)
#endif
						 || ((GSY_SYNC_TIME_IDX == RecordIndex) && (0 != SyncTakeover)))
		{
			ErrOffset = GSY_OFFSET_GET(pSyncData, &pSyncData->TakeoverFactor);
		}
#if GSY_SYNC_STARTUP_MIN != 0
		else if (((GSY_SYNC_STARTUP_MAX < MasterStartupTime) && (GSY_SYNC_STARTUP_MIN > MasterStartupTime))
#else
		else if ((GSY_SYNC_STARTUP_MAX < MasterStartupTime)
#endif
						 || ((GSY_SYNC_TIME_IDX == RecordIndex) && (0 != MasterStartupTime)))
		{
			ErrOffset = GSY_OFFSET_GET(pSyncData, &pSyncData->MasterStartupTime);
		}
		else if (((GSY_SYNC_TIME_IDX == RecordIndex) && (GSY_SYNCID_TIME      != SyncId))
			 ||    ((GSY_SYNC_DATA_IDX == RecordIndex) && (GSY_SYNCID_CLOCK     != SyncId))
			 ||    ((GSY_SYNC_ROLE_MASTER != SyncRole) && (GSY_SYNC_ROLE_SLAVE  != SyncRole))
			 ||    ((GSY_SYNC_ROLE_MASTER == SyncRole) && (GSY_HW_TYPE_MASTER_NO == HWMaster))
			 ||    ((GSY_SYNC_TIME_IDX == RecordIndex) && (GSY_SYNC_ROLE_MASTER  == SyncRole) && (GSY_HW_TYPE_SOC != HWType))
			 ||    ((GSY_SYNC_TIME_IDX == RecordIndex) && (GSY_SYNC_ROLE_SLAVE   == SyncRole) && (GSY_HW_TYPE_ERTEC_200P != HWType) && (GSY_HW_TYPE_HERA != HWType)))
		{
			ErrOffset = GSY_OFFSET_GET(pSyncData, &pSyncData->SyncProperties);
		}
		else if (((GSY_SYNC_ROLE_SLAVE  == SyncRole) && (GSY_SYNC_PRIO1_SLAVE != MasterPrio1))	/* 170408lrg001: check */
				 ||  ((GSY_SYNC_ROLE_MASTER == SyncRole)
				 &&  ((GSY_SYNC_PRIO1_LEVEL_MAX < MasterLevel)			/* 170408lrg001 */
				 ||  ((GSY_SYNC_TIME_IDX == RecordIndex)    && (GSY_SYNC_PRIO1_PRIMARY   != MasterPrio))
				 || ((GSY_SYNC_PRIO1_PRIMARY != MasterPrio) && (GSY_SYNC_PRIO1_SECONDARY != MasterPrio)))))
		{
			ErrOffset = GSY_OFFSET_GET(pSyncData, &pSyncData->MasterPriority1);
		}
		else if ((GSY_SYNC_ROLE_MASTER == SyncRole) && (GSY_SYNC_PRIO2_DEFAULT != MasterPrio2))		/* 260706lrg001 */
		{
			ErrOffset = GSY_OFFSET_GET(pSyncData, &pSyncData->MasterPriority2);
		}
		else
		{
			/* Copy SyncData Record into channel data and update PrmState
			*/
			GSY_COPY_UPPER_TO_LOCAL_MEM(pSyncData, &pChUsr->PrmData, CopyLen, pChUsr->pSys);
			pChUsr->PrmState = GSY_PRM_STATE_WRITE;
			
			if ((0 != pChUsr->PrmRecordIndexWritten) && (RecordIndex != pChUsr->PrmRecordIndexWritten))
			{
				pChUsr->PrmClockAndTimeWritten = LSA_TRUE;
			}
			pChUsr->PrmRecordIndexWritten = RecordIndex;
			RespVal = GSY_RSP_OK;
		}
	}
							 
 if (GSY_RSP_ERR_PRM_BLOCK == RespVal)
 {
	 GSY_UPPER_RQB_PRMWRITE_ERROFFSET_SET(pRbu, ErrOffset);
 }
 
	return(RespVal);
}
							 
/*****************************************************************************/
/* Internal function: gsy_PrmWrite()                                         */
/* Write user record to temporary Record (B)                                 */
/*****************************************************************************/
LSA_UINT16 GSY_LOCAL_FCT_ATTR gsy_PrmWrite(
																					GSY_CH_USR_PTR			pChUsr,
																					GSY_UPPER_RQB_PTR_TYPE  pRbu)
{
 LSA_UINT32	RecordIndex = 0;
 LSA_UINT16	RespVal = GSY_RSP_ERR_SEQUENCE;
 GSY_UPPER_SYNC_DATA_PTR pRecordData = (GSY_UPPER_SYNC_DATA_PTR)(void*)GSY_UPPER_RQB_PRMRECORD_PTR_GET(pRbu);
 
 GSY_FUNCTION_TRACE_03(pChUsr->TraceIdx, LSA_TRACE_LEVEL_CHAT,"  > gsy_PrmWrite() pChUsr=%X pRbu=%X SyncId=0x%02x",
											 pChUsr, pRbu, pChUsr->SyncId);
 
 if (LSA_HOST_PTR_ARE_EQUAL(pRecordData, LSA_NULL))
 {
	 RespVal = GSY_RSP_ERR_PARAM;
	 GSY_ERROR_TRACE_01(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmWrite() Invalid record data pointer=%X",
											pRecordData);
 }
 else if ((GSY_PRM_STATE_PREPARE == pChUsr->PrmState)
			||  (GSY_PRM_STATE_WRITE   == pChUsr->PrmState)
			||  (GSY_PRM_STATE_ERROR   == pChUsr->PrmState))
 {
	 /* If there already was a PrmPrepare or a PrmWrite...
	 */
	 pChUsr->PrmState = GSY_PRM_STATE_ERROR;
	 RespVal = GSY_RSP_ERR_PRM_INDEX;
	 RecordIndex = GSY_UPPER_RQB_PRMRECORD_IDX_GET(pRbu);
	 
	 if (GSY_SEND_CLOCK_IDX == RecordIndex)
	 {
		 /* 051206lrg001: load SendClockFactor record
		 * 131206lrg001: also on EDDS
		 */
		 RespVal = gsy_PrmWriteSendClock(pChUsr, pRbu);
	 }
	 else
	 {
		 if (GSY_PORT_DATA_IDX == RecordIndex)
		 {
			 /* load PDPortDataAdjust record
			 * 170107lrg001: also on EDDS
			 */
			 RespVal = gsy_PrmWritePortData(pChUsr, pRbu);
		 }
		 else if (pChUsr->pChSys->RxTxNanos != 0)
		 {
			 /* NOT on EDDS
			 */
			 if (GSY_SYNC_PLL_IDX == RecordIndex)
			 {
				 /* 230910lrg001: load PDSyncPLL record
				 */
				 RespVal = gsy_PrmWriteSyncPLL(pChUsr, pRbu);
			 }
			 else if (((GSY_SYNC_DATA_IDX == RecordIndex) && (1 == (GSY_SYNC_ID_SUPPORTED & 1)))    //lint !e506 Constant Boolean, define-based behaviour, HM 20.05.2016
						||  ((GSY_SYNC_TIME_IDX == RecordIndex) && (2 == (GSY_SYNC_ID_SUPPORTED & 2))))   //lint !e506 !e774 Constant Boolean within 'if' always evaluates to True, define-based behaviour, HM 20.05.2016
			 {
				 /* Check and write sync data/time record
				 */
				 RespVal = gsy_PrmWriteSyncData(pChUsr, pRbu);
			 }
			 else
			 {
				 GSY_ERROR_TRACE_01(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmWrite() Invalid record index=0x%x",
														RecordIndex);
			 }
		 }
		 else
		 {
			 GSY_ERROR_TRACE_01(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmWrite() No HW support for record index=0x%x",
													RecordIndex);
		 }
	 }
 }
 else
 {
	 GSY_ERROR_TRACE_01(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmWrite() Invalid PRM state=%u",
											pChUsr->PrmState);
 }
 
 if ((GSY_RSP_OK != RespVal) && (GSY_RSP_ERR_PRM_BLOCK != RespVal))
 {
	 /* On GSY_RSP_ERR_PRM_BLOCK the error offset is already set
	 */
	 GSY_UPPER_RQB_PRMWRITE_ERROFFSET_SET(pRbu, GSY_PRM_ERR_OFFSET_DEFAULT);
 }
 
 GSY_FUNCTION_TRACE_03(pChUsr->TraceIdx, LSA_TRACE_LEVEL_CHAT,"  < gsy_PrmWrite() pChUsr=%X RecordIndex=0x%x RespVal=0x%x",
											 pChUsr, RecordIndex, RespVal);
 return(RespVal);
}

/*****************************************************************************/
/* Internal function: gsy_PrmEnd()                                           */
/* Check consistency of values in temporary records ueberpruefen             */
/*****************************************************************************/
LSA_UINT16 GSY_LOCAL_FCT_ATTR gsy_PrmEnd(
																				GSY_CH_USR_PTR			pChUsr,
																				GSY_UPPER_RQB_PTR_TYPE  pRbu)
{
 LSA_UINT16	SyncProperties;
 LSA_UINT8	SyncRole, SyncId, MasterPrio1;
 LSA_UINT16	RespVal = GSY_RSP_ERR_SEQUENCE;
 LSA_UINT32	ResIntervalEnd = GSY_SWAP32(pChUsr->PrmData.ResIntervalEnd.Dword);
 LSA_UINT32	SendClockNanos  = GSY_SWAP16(pChUsr->PrmSendClock.Word) * GSY_SYNC_CLOCK_BASE;	// 220107lrg002: was SWAP32
 LSA_UINT32	ErrOffset = GSY_PRM_ERR_OFFSET_DEFAULT;
 LSA_UINT32	ErrIndex = GSY_PRM_ERR_INDEX_DEFAULT;
 LSA_UINT8	PortparamsNotApplicable[EDD_CFG_MAX_PORT_CNT];
 
 GSY_FUNCTION_TRACE_03(pChUsr->TraceIdx, LSA_TRACE_LEVEL_CHAT,"  > gsy_PrmEnd() pChUsr=%X pRbu=%X SyncId=0x%02x",
											 pChUsr, pRbu, pChUsr->SyncId);
 
 /* If there already was a PrmPrepare or a PrmWrite...
 */
 if ((GSY_PRM_STATE_PREPARE == pChUsr->PrmState)
 ||  (GSY_PRM_STATE_WRITE == pChUsr->PrmState))
 {
	 /* 051206lrg001: check SendClockFactor if SyncRecord has been written
	 */
	 pChUsr->PrmState = GSY_PRM_STATE_ERROR;
	 RespVal = GSY_RSP_ERR_PRM_CONSISTENCY;
	 
	 if ((pChUsr->PrmData.SendClockFactor.Word != 0)
			 &&  (pChUsr->PrmData.SendClockFactor.Word != pChUsr->PrmSendClock.Word))
	 {
		 ErrOffset = GSY_OFFSET_GET(&pChUsr->PrmData, &pChUsr->PrmData.SendClockFactor);
		 GSY_ERROR_TRACE_03(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmEnd() SendClockFactor=%u does not match PDSyncData:%u ErrOffset=%u",
												GSY_SWAP16(pChUsr->PrmSendClock.Word), GSY_SWAP16(pChUsr->PrmData.SendClockFactor.Word), ErrOffset);
	 }
	 else if (ResIntervalEnd > SendClockNanos)
	 {
		 /* 131206lrg002: check ReservedIntervalEnd with SendClockFactor
		 */
		 ErrOffset = GSY_OFFSET_GET(&pChUsr->PrmData, &pChUsr->PrmData.ResIntervalEnd);
		 GSY_ERROR_TRACE_03(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmEnd() Invalid ResIntervalEnd=%u SendClockNanos=%u ErrOffset=%u",
												ResIntervalEnd, SendClockNanos, ErrOffset);
	 }
	 else if (pChUsr->PrmClockAndTimeWritten)
	 {
		 GSY_ERROR_TRACE_01(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmEnd() RecIdx=0x%x: Forbidden to overwrite the previous RecordIndex ",
												pChUsr->PrmRecordIndexWritten);
	 }
	 else
	 {
		 pChUsr->PrmRTC2 = LSA_FALSE;
		 SyncProperties = GSY_SWAP16(pChUsr->PrmData.SyncProperties.Word);
		 SyncRole = (LSA_UINT8)(SyncProperties & 0x03);
		 SyncId = ((LSA_UINT8)(SyncProperties >> 8)) & GSY_SYNCID_MASK;
		 MasterPrio1 = GSY_SYNC_PRIO1_PRIO(pChUsr->PrmData.MasterPriority1); 
		 
		 /* Check PDSyncPLL record
		 */
		 if (pChUsr->PrmPLLWritten
		&&  (SyncRole != GSY_SYNC_ROLE_LOCAL)										/* PDSync record written */
		&&  ((SyncId != GSY_SYNCID_CLOCK) || (SyncRole != GSY_SYNC_ROLE_MASTER)))	//RQ 1533977 //131210lrg001
		 {
			 pChUsr->PrmPLLWritten = LSA_FALSE;
			 ErrOffset = GSY_OFFSET_GET(&pChUsr->PrmData, &pChUsr->PrmData.SyncProperties);
			 GSY_ERROR_TRACE_03(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmEnd() Invalid SyncRole=%u or SyncId=%u for PDSyncPLL record ErrOffset=%u",
													SyncRole, SyncId, ErrOffset);
		 }
		 else if (pChUsr->PrmPLLWritten && (SyncRole == GSY_SYNC_ROLE_MASTER)
							&& (MasterPrio1 != GSY_SYNC_PRIO1_PRIMARY))
		 {
			 pChUsr->PrmPLLWritten = LSA_FALSE;
			 ErrOffset = GSY_OFFSET_GET(&pChUsr->PrmData, &pChUsr->PrmData.MasterPriority1);
			 GSY_ERROR_TRACE_02(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmEnd() Invalid MasterPrio1=%u for PDSyncPLL record ErrOffset=%u",
													MasterPrio1, ErrOffset);
		 }
		 else 
		 {
			 //if the SyncId was changed
			 pChUsr->PrmSyncIdChanged = ((SyncId != pChUsr->SyncId) ? LSA_TRUE : LSA_FALSE);
			 
			 RespVal = GSY_RSP_OK;
		 }
	 }
 }
 else
 {
	 GSY_ERROR_TRACE_01(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmEnd() Invalid PRM state=0x%x",
											pChUsr->PrmState);
 }
 
 if (GSY_RSP_OK != RespVal)
 {
	 if (GSY_RSP_ERR_PRM_CONSISTENCY == RespVal)
	 {
		 if (0 != pChUsr->PrmRecordIndexWritten)
		 {
			 ErrIndex = pChUsr->PrmRecordIndexWritten;
		 }
		 GSY_UPPER_RQB_PRMEND_ERROFFSET_SET(pRbu, ErrOffset);
		 GSY_UPPER_RQB_PRMEND_ERRINDEX_SET(pRbu, ErrIndex);
	 }
	 else
	 {
		 GSY_UPPER_RQB_PRMEND_ERROFFSET_SET(pRbu, GSY_PRM_ERR_OFFSET_DEFAULT);
		 GSY_UPPER_RQB_PRMEND_ERRINDEX_SET(pRbu, GSY_PRM_ERR_INDEX_DEFAULT);
	 }
	 GSY_UPPER_RQB_PRMEND_ERRPORTID_SET(pRbu, GSY_PRM_ERR_PORT_ID_DEFAULT);
	 GSY_UPPER_RQB_PRMEND_ERRFAULT_SET(pRbu, GSY_PRM_ERR_FAULT_DEFAULT);
 }
 else
 {
	 GSY_MEMSET_LOCAL(PortparamsNotApplicable, GSY_PRM_PORT_PARAMS_APPLICABLE, sizeof(PortparamsNotApplicable));
	 GSY_UPPER_RQB_PRMEND_PORTPARAMSNOTAPPLICABLE_SET(pRbu, PortparamsNotApplicable, pChUsr->pChSys->PortCount);
	 
	 pChUsr->PrmState = GSY_PRM_STATE_END;
	 GSY_PRM_TRACE_06(pChUsr->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH,"--- gsy_PrmEnd(%02x) PrmRTC2=%u ResEnd=%u SendClockNanos=%u PrmPLLWritten=%u PrmRecordIndexWritten=0x%x",
										pChUsr->SyncId, pChUsr->PrmRTC2, ResIntervalEnd, SendClockNanos, pChUsr->PrmPLLWritten, pChUsr->PrmRecordIndexWritten);
 }
 
 GSY_FUNCTION_TRACE_03(pChUsr->TraceIdx, LSA_TRACE_LEVEL_CHAT,"  < gsy_PrmEnd() pChUsr=%X PrmRTC2=%d RespVal=0x%x",
											 pChUsr, pChUsr->PrmRTC2, RespVal);
 
 LSA_UNUSED_ARG(pRbu);
 
 return(RespVal);
}

/*****************************************************************************/
/* Internal function: gsy_PrmCommit()                                        */
/* Put values of the temporary records (B) into active data (A)              */
/*****************************************************************************/
LSA_UINT16 GSY_LOCAL_FCT_ATTR gsy_PrmCommit(
																					 GSY_CH_USR_PTR			pChUsr,
																					 GSY_UPPER_RQB_PTR_TYPE  pRbu)
{
 LSA_UINT16	RespVal = GSY_RSP_ERR_SEQUENCE;
 LSA_UINT8	ApplyDefaultPortparams[EDD_CFG_MAX_PORT_CNT];
 LSA_UNUSED_ARG(ApplyDefaultPortparams)

 /* If prior was a PrmEnd...
 */
 if (GSY_PRM_STATE_END == pChUsr->PrmState)
 {
	 RespVal = GSY_RSP_OK;
	 
	 /* 170707lrg002: not longer setting of bandwidth and -(de)activation
	 */
	 
	 /* 110806js001: restart synchronisation on subdomain UUID change
	 */
	 
	 GSY_UPPER_RQB_PRMCOMMIT_APPLYDEFAULTPORTPARAMS_GET(pRbu, ApplyDefaultPortparams, EDD_CFG_MAX_PORT_CNT);
	 
	 if (GSY_SUBDOMAIN_CMP(pChUsr->PrmData.SubdomainID, pChUsr->SyncData.SubdomainID))
	 {
		 pChUsr->PrmSubdomainIDChanged = LSA_FALSE;
		 pChUsr->PrmSubdomainInd = LSA_FALSE;
	 }
	 else
	 {
		 pChUsr->PrmSubdomainIDChanged = LSA_TRUE;
		 pChUsr->PrmSubdomainInd = LSA_TRUE;
	 }
	 
	 /* Check PDSyncPLL and Data record for relevant changes
	 */
	 if ((pChUsr->SyncPLLActive || pChUsr->PrmPLLWritten)
			 &&  ((pChUsr->SyncPLL.ReductionRatio.Word != pChUsr->PrmPLL.ReductionRatio.Word)
				||  (pChUsr->SyncPLL.PLLHWDelay.Word != pChUsr->PrmPLL.PLLHWDelay.Word)
				||  (pChUsr->SyncPLL.ControlInterval.Word != pChUsr->PrmPLL.ControlInterval.Word)
	//		 ||  (pChUsr->SyncData.PLLWindow.Dword != pChUsr->PrmData.PLLWindow.Dword)
	//		 ||  (pChUsr->SyncData.SyncSendFactor.Dword != pChUsr->PrmData.SyncSendFactor.Dword)
	//		 ||  (pChUsr->SyncData.SendClockFactor.Word != pChUsr->PrmData.SendClockFactor.Word)))
				||  (pChUsr->SendClockFactor.Word != pChUsr->PrmSendClock.Word)))
	 {
		 pChUsr->SyncPLLChanged = LSA_TRUE;
	 }
	 else
	 {
		 pChUsr->SyncPLLChanged = LSA_FALSE;
	 }
	 /* Copy SyncData/PLL Record and update PrmState
	 */
	 pChUsr->SendClockFactor = pChUsr->PrmSendClock;
	 pChUsr->SyncPLL = pChUsr->PrmPLL;
	 pChUsr->SyncData = pChUsr->PrmData;
	 pChUsr->PrmState = GSY_PRM_STATE_COMMIT;
	 
	 GSY_PRM_TRACE_04(pChUsr->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH,"--- gsy_PrmCommit(%02x) SubdomainIDChanged=%u SyncPLLChanged=%u RespVal=0x%x",
										pChUsr->SyncId, pChUsr->PrmSubdomainIDChanged, pChUsr->SyncPLLChanged, RespVal);
	 
	 gsy_PrmCommitNext(pChUsr, pRbu);
	 RespVal = GSY_RSP_OK_ACTIVE;
 }
 else
 {
	 GSY_ERROR_TRACE_01(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmCommit() Invalid state=%u",
											pChUsr->PrmState);
 }
 
 GSY_FUNCTION_TRACE_02(pChUsr->TraceIdx, LSA_TRACE_LEVEL_CHAT,"  < gsy_PrmCommit() pChUsr=%X RespVal=0x%x",
											 pChUsr, RespVal);
 return(RespVal);
}

/*****************************************************************************/
/* Internal function: gsy_PrmCommitNext()                                    */
/* COMMIT state machine                                                      */
/*****************************************************************************/
LSA_VOID GSY_LOCAL_FCT_ATTR gsy_PrmCommitNext(
																						 GSY_CH_USR_PTR			pChUsr,
																						 GSY_UPPER_RQB_PTR_TYPE  pRbu)
{
 GSY_SYNC_DATA_RECORD 	*pSyncData;
 LSA_UINT32	PLLWindow, SlaveWindow, SyncTimeout, SyncTakeover, TimeStamp = 0, SendInterval;
 LSA_UINT16	Timeout, Takeover, SyncInTime, SyncInterval, PLLInterval, Epoch = 0; 
 LSA_UINT16	MasterStartupTime, SyncProperties;
 LSA_UINT16	SetWaitCount, ReductionRatio, PLLHWDelay, SendClockFactor;		//230910lrg001
 LSA_UINT8	SyncRole, SyncId, MasterPrio1, MasterPrio2;
 LSA_UINT16	RetVal = LSA_RET_OK;
 LSA_UINT16	RespVal = GSY_RSP_ERR_SEQUENCE;
 LSA_BOOL	SendIndication = LSA_FALSE;
 
 GSY_FUNCTION_TRACE_03(pChUsr->TraceIdx, LSA_TRACE_LEVEL_CHAT,"  > gsy_PrmCommitNext() pChUsr=%X pRbu=%X SyncId=0x%02x",
											 pChUsr, pRbu, pChUsr->SyncId);
 
 GSY_PRM_TRACE_02(pChUsr->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH,"--- gsy_PrmCommitNext(%02x) PRM state=%u",
									pChUsr->SyncId, pChUsr->PrmState);
 
 /* If prior was a PrmCommit...
 */
 if ((GSY_PRM_STATE_COMMIT == pChUsr->PrmState)
		 ||  (GSY_PRM_STATE_STOP == pChUsr->PrmState))
 {
	 /* Get Sync parameters from active SyncDataRecord
	 */
	 RespVal = GSY_RSP_OK;
	 pSyncData = &pChUsr->SyncData;
	 SlaveWindow = GSY_SWAP32(pSyncData->PLLWindow.Dword);
	 SendInterval = GSY_SWAP32(pSyncData->SyncSendFactor.Dword);
	 Timeout = GSY_SWAP16(pSyncData->TimeoutFactor.Word);
	 Takeover = GSY_SWAP16(pSyncData->TakeoverFactor.Word);
	 MasterStartupTime = GSY_SWAP16(pSyncData->MasterStartupTime.Word);	//290807lrg001
	 SyncProperties = GSY_SWAP16(pSyncData->SyncProperties.Word);
	 SyncRole = (LSA_UINT8)(SyncProperties & 0x03);
	 SyncId = ((LSA_UINT8)(SyncProperties >> 8)) & GSY_SYNCID_MASK;
	 MasterPrio1 = pSyncData->MasterPriority1;
	 MasterPrio2 = pSyncData->MasterPriority2;
	 
	 /* Get PLL parameters from active SyncPLL record
	 */
	 SendClockFactor = GSY_SWAP16(pChUsr->SendClockFactor.Word);
	 ReductionRatio  = GSY_SWAP16(pChUsr->SyncPLL.ReductionRatio.Word);
	 PLLHWDelay      = GSY_SWAP16(pChUsr->SyncPLL.PLLHWDelay.Word);
	 PLLInterval     = GSY_SWAP16(pChUsr->SyncPLL.ControlInterval.Word);
	 PLLWindow       = GSY_SWAP32(pChUsr->SyncPLL.PLLWindow.Dword);
	 SetWaitCount    = GSY_SWAP16(pChUsr->SyncPLL.SetWaitCount.Word);
	 
	 /* 210807lrg001: Timeout is counter instead muliple of 3125ns
	 *               Sync/SendInterval are muliple of 10ms (30...39 => 30)
	 */
	 SyncTimeout = Timeout;
	 SyncTakeover = Takeover;
	 SendInterval = (SendInterval / GSY_CLOCK_BASE_PER_MILLISECOND);
	 
	 if (GSY_SYNC_ROLE_LOCAL != SyncRole)
	 {
		 if (SendInterval < 10)
		 {
			 SendInterval = 10;
		 }
		 else if ((SendInterval % 10) != 0)
		 {
			 SendInterval = (SendInterval / 10) * 10;
		 }
	 }
	 
	 SyncInterval = (LSA_UINT16)SendInterval;
	 SyncInTime   = (LSA_UINT16)(SyncInterval * Timeout);
	 
	 if  ((pChUsr->SyncRole != SyncRole)
	 ||   (pChUsr->SyncId != SyncId)
	 ||   (pChUsr->PrmSubdomainIDChanged)				// 110806js001
	 ||   (pChUsr->SyncPLLChanged)						// 230910lrg001
	 ||   (pChUsr->Slave.SyncInterval != SyncInterval)
	 ||   (pChUsr->Master.SendInterval != (LSA_UINT16)SendInterval)
	 ||	 ((pChUsr->Master.Priority1 != MasterPrio1) && (GSY_SYNC_ROLE_MASTER == SyncRole)))	// 200308lrg001: Stop Master for Update Prio1
	 {
		 /* Stop/start Master, PLLsync or Slave depending of new SyncRole
		 */
		 if ((GSY_PRM_STATE_STOP   != pChUsr->PrmState)
		 && ((GSY_MASTER_STATE_OFF != pChUsr->Master.State)
		 ||  (GSY_SLAVE_STATE_OFF  != pChUsr->Slave.State)))
		 {
			 /* Stop Slave or Master
			 */
			 pChUsr->PrmState = GSY_PRM_STATE_STOP;
			 pChUsr->MasterControlState = GSY_MASTER_CONTROL_STATE_NONE;
			 //TODO:-OS-					RespVal = gsy_SyncStop(pChUsr, pRbu);
			 
			 if ((GSY_RSP_OK != RespVal) && (GSY_RSP_OK_ACTIVE != RespVal))
			 {
				 //ERROR
			 }
		 }
		 else if (pChUsr->SyncPLLChanged
							&&  (GSY_PLL_STATE_SYNC_OFF != pChUsr->PLLData.State))
		 {
			 /* Stop PLL synchronisation
			 */			 
			 if (!LSA_HOST_PTR_ARE_EQUAL(pRbu, LSA_NULL))
			 {
				 gsy_PutUpperQ(&pChUsr->QueueUpper, pRbu);
			 }
			 
			 RespVal = GSY_RSP_OK_ACTIVE;
			 pChUsr->PLLData.State = GSY_PLL_STATE_SYNC_STOP;
			 pChUsr->PrmState = GSY_PRM_STATE_STOP;
			 
			 if (GSY_HW_TYPE_SLAVE_HW == (pChUsr->pChSys->HardwareType & GSY_HW_TYPE_SLAVE_MSK))
			 {
				 /* Stop PLL synchronisation in KRISC32
				 */
			 }
			 /* else all will be done by gsy_MasterPLLTimer()
			 */
		 }
		 
		 if (GSY_RSP_OK == RespVal) 
		 {
			 if (pChUsr->SyncPLLChanged)
			 {				 
				 pChUsr->PLLData.ReductionRatio = ReductionRatio;
				 pChUsr->PLLData.PLLHWDelay = PLLHWDelay;
				 pChUsr->PLLData.PLLInterval = PLLInterval;
				 pChUsr->PLLData.PLLWindow = PLLWindow;
				 pChUsr->PLLData.SetWaitCount = SetWaitCount;
				 
				 if (pChUsr->PrmPLLWritten)
				 {
					 /* Start PLL synchronisation
					 */
					 pChUsr->PLLData.CycleLength = (LSA_UINT32)SendClockFactor * GSY_SYNC_CLOCK_BASE;
					 pChUsr->SyncPLLActive = LSA_TRUE;
					 
					 if (GSY_HW_TYPE_SLAVE_HW == (pChUsr->pChSys->HardwareType & GSY_HW_TYPE_SLAVE_MSK))
					 {
						 /* Start PLL synchronisation in KRISC32
						 */
						 //startPTP(pChUsr); 
					 }
					 else
					 {
						 /* Start PLL synchronisation in GSY
						 */
						 //-OS-gsy_MasterPLLStart(pChUsr);
						 //TODO:-OS- temprary stert here, for debug.							
						 //startPTP(pChUsr); 
					 }
				 }
				 else
				 {
					 /* Not longer PLL synchronisation
					 */
					 pChUsr->PLLData.CycleLength = 0;
					 pChUsr->SyncPLLActive = LSA_FALSE;
				 }
				 pChUsr->SyncPLLChanged = LSA_FALSE;
			 }
			 
			 if (GSY_SYNC_ROLE_LOCAL != SyncRole)
			 {
				 /* Now there is no running service and a new master or slave should be started: 
				 * Reset statistik data on (re)start of master/slave
				 * 100209lrg001: do not get time on EDDP-SyncChannel
				 */
				 GSY_MEMSET_LOCAL(&pChUsr->Statistics, 0, sizeof(GSY_SYNC_STATISTIC_TYPE));
				 
				 if (GSY_HW_TYPE_SLAVE_SW == (pChUsr->pChSys->HardwareType & GSY_HW_TYPE_SLAVE_MSK))
				 {
					 RetVal = GSY_SYNC_GET(&Epoch, &pChUsr->Statistics.StatisticAge.Seconds, 
																 &pChUsr->Statistics.StatisticAge.Nanoseconds, 
																 &TimeStamp, SyncId, pChUsr->pSys);
				 }
				 if (LSA_RET_OK != RetVal)
				 {
					 pChUsr->PrmState = GSY_PRM_STATE_ERROR;
					 RespVal = GSY_RSP_ERR_PARAM;
					 GSY_ERROR_TRACE_02(pChUsr->TraceIdx, LSA_TRACE_LEVEL_ERROR,"*** gsy_PrmCommitNext() SyncId=%u: Error=0x%x from GSY_SYNC_GET()",
															SyncId, RetVal);
					 //						gsy_ErrorUser(GSY_MODULE_ID, __LINE__, GSY_ERR_CONFIG, pRbu, sizeof(pRbu));
				 }
				 else
				 {
					 /* Reset Indication flags/event and set Sync parameters
					 */
					 pChUsr->Event.OldEvent = GSY_SYNC_EVENT_NONE;
					 pChUsr->Event.IndFlags = GSY_PRM_IND_FLAGS_NONE;
					 
					 if (GSY_SYNCID_CLOCK == SyncId)
					 {
						 pChUsr->Event.Prm.ChannelErrorType = GSY_PRM_CHANNEL_ERROR_TYPE_SYNC;
					 }
					 else
					 {
						 pChUsr->Event.Prm.ChannelErrorType = GSY_PRM_CHANNEL_ERROR_TYPE_TIME;
					 }
					 
					 pChUsr->SyncId = SyncId;
					 pChUsr->Slave.WindowSize   = SlaveWindow;
					 pChUsr->Slave.SyncInTime   = SyncInTime;
					 pChUsr->Slave.SyncTimeout  = SyncTimeout;
					 pChUsr->Slave.SyncTakeover = SyncTakeover;
					 pChUsr->Master.Priority1   = MasterPrio1;
					 pChUsr->Master.Priority2   = MasterPrio2;
					 pChUsr->Master.StartupTime = MasterStartupTime;
					 
					 /* Start Master or Slave
					 */
					 pChUsr->SyncRole = SyncRole;
					 pChUsr->Master.SendInterval = (LSA_UINT16)SendInterval;
					 pChUsr->Slave.SyncInterval = SyncInterval;
					 pChUsr->PrmSubdomainIDChanged = LSA_FALSE;  /* 110806js001 */
					 
					 pChUsr->PrmState = GSY_PRM_STATE_START;
					 if (GSY_SYNC_ROLE_MASTER == SyncRole)
					 {
						 if  ( ((pChUsr->pChSys->HardwareType & GSY_HW_TYPE_MSK) == GSY_HW_TYPE_SOC)
									&& (pChUsr->SyncId == GSY_SYNCID_TIME) )
						 {
							 pChUsr->MasterControlState = GSY_MASTER_CONTROL_STATE_STARTABLE;
							 SendIndication = LSA_TRUE;
						 }
						 else
						 {
							 //TODO:-OS-									RespVal = gsy_MasterStart(pChUsr);
						 }
					 }
					 else if (GSY_SYNC_ROLE_SLAVE == SyncRole)
					 {
						 startPTP(pChUsr);
						 RespVal = GSY_RSP_OK;
						 //RespVal = gsy_SlaveStart(pChUsr);
					 }
					 
					 if (GSY_RSP_OK != RespVal)
					 {
						 //ERROR
					 }
				 }
			 }
			 else
			 {
				 /* No new service after stop: 
				 * Reset Indication flags/event and set Sync parameters
				 */
				 SendIndication = LSA_TRUE;
				 pChUsr->PrmState = GSY_PRM_STATE_STOP;
				 pChUsr->Event.SyncEvent = GSY_SYNC_EVENT_NONE;		// 291106lrg001
				 pChUsr->Event.OldEvent = GSY_SYNC_EVENT_NONE;
				 pChUsr->Event.IndFlags = GSY_PRM_IND_FLAGS_NONE;
				 pChUsr->Event.PrmFlags = GSY_PRM_IND_FLAGS_NONE;
				 pChUsr->Slave.WindowSize = SlaveWindow;
				 pChUsr->Slave.SyncInterval = SyncInterval;
				 pChUsr->Slave.SyncInTime = SyncInTime;
				 pChUsr->Slave.SyncTimeout = SyncTimeout;
				 pChUsr->Slave.SyncTakeover = SyncTakeover;
				 pChUsr->Master.Priority1 = MasterPrio1;
				 pChUsr->Master.Priority2 = MasterPrio2;
				 pChUsr->Master.SendInterval = (LSA_UINT16)SendInterval;
				 pChUsr->Master.StartupTime = MasterStartupTime;
				 pChUsr->SyncId = GSY_SYNCID_NONE;
				 pChUsr->SyncRole = SyncRole;
			 }
		 }
	 }
	 else
	 {
		 if (pChUsr->PrmPLLWritten)
		 {
			 /* Update PLL synchronisation parameters
			 */
			 pChUsr->PLLData.ReductionRatio = ReductionRatio;
			 pChUsr->PLLData.PLLHWDelay     = PLLHWDelay;
			 pChUsr->PLLData.PLLInterval    = PLLInterval;
			 pChUsr->PLLData.PLLWindow      = PLLWindow;
			 pChUsr->PLLData.SetWaitCount   = SetWaitCount;
			 pChUsr->PLLData.CycleLength    = (LSA_UINT32)SendClockFactor * GSY_SYNC_CLOCK_BASE;	 
		 }
		 if (GSY_SYNC_ROLE_LOCAL != SyncRole)
		 {			 
			 pChUsr->PrmState = GSY_PRM_STATE_NONE;
			 pChUsr->Event.OldEvent = GSY_SYNC_EVENT_NONE;
			 pChUsr->Event.IndFlags = GSY_PRM_IND_FLAGS_NONE;
			 pChUsr->Event.PrmFlags = GSY_PRM_IND_FLAGS_NONE;
			 pChUsr->SyncId = SyncId;
			 pChUsr->Slave.WindowSize   = SlaveWindow;
			 pChUsr->Slave.SyncInTime   = SyncInTime;
			 pChUsr->Slave.SyncTimeout  = SyncTimeout;
			 pChUsr->Slave.SyncTakeover = SyncTakeover;
			 pChUsr->Master.Priority2   = MasterPrio2;
			 pChUsr->Master.StartupTime = MasterStartupTime;
			 
			 if (GSY_SYNC_ROLE_MASTER == SyncRole)
			 {
				 /* 200308lrg001: Update	running Master
				 */
				 if (pChUsr->Master.Priority1 != MasterPrio1)
				 {
					 pChUsr->Master.Priority1 = MasterPrio1;
					 MasterPrio1 = pChUsr->Master.LocalMaster.Priority1 & GSY_SYNC_PRIO1_ACTIVE;
					 MasterPrio1 |= pChUsr->Master.Priority1;
					 pChUsr->Master.LocalMaster.Priority1 = MasterPrio1;
				 }
				 
				 pChUsr->Master.LocalMaster.Priority2 = pChUsr->Master.Priority2;
				 
				 if (GSY_SYNC_TAKEOVER_DISABLED != pChUsr->Slave.SyncTakeover)
					 pChUsr->Master.TimeoutCount = pChUsr->Slave.SyncTakeover;
				 else
					 pChUsr->Master.TimeoutCount = pChUsr->Slave.SyncTimeout;
			 }
		 }
		 else
		 {
			 /* No old and no new service: 
			 * 070706lrg001: initiate empty PRM Indication
			 */
			 pChUsr->Event.SyncEvent = GSY_SYNC_EVENT_NONE;
		 }
		 
		 SendIndication = LSA_TRUE;
	 }
	 
	 if (GSY_RSP_OK_ACTIVE != RespVal)
	 {
		 /*  Change Forwarding configuration from PDPortDataAdjust record
		 */
		 pChUsr->pChSys->pCommitRbu = pRbu;
		 pChUsr->pChSys->PrmFwdInd  = SendIndication;
		 gsy_PrmCommitEnd(pChUsr->pChSys, GSY_RSP_OK);
	 }
 }
}
							 
/*****************************************************************************/
/* Internal access function: gsy_PrmCommitEnd()                              */
/* Finish COMMIT state machine                                               */
/*****************************************************************************/
LSA_VOID GSY_LOCAL_FCT_ATTR gsy_PrmCommitEnd(
																						GSY_CH_SYS_PTR			pChSys,
																						LSA_UINT16				RespVal)
{
 LSA_UINT16 			PortIdx;
 LSA_UINT32 			BitBoundary, SyncIdMaskIn, SyncIdMaskOut;
 GSY_DRIFT_PTR		pDriftClock = LSA_NULL;
 GSY_DRIFT_PTR		pDriftTime  = LSA_NULL;
 GSY_MAC_ADR_TYPE	NoMacAddr = {GSY_NO_MASTER_ADDR};
 LSA_BOOL			PortDataAdjustWritten = LSA_TRUE;

 
 if (GSY_RSP_OK != RespVal)
 {
	 //ERROR
 }
 /* 301106lrg001: Do not configure Forwarding an EDD without timestamps
 */
 else if (pChSys->RxTxNanos != 0)
 {
	 for (PortIdx = 0; PortIdx < pChSys->PortCount; PortIdx++)
	 {
		 
		 /* 161106js001: has port to be configured?
		 */
		 if (pChSys->Port[PortIdx].PrmDomainCfg)
		 {
			 /* 161106js001: take values from record or default values
			 */
			 if (pChSys->Port[PortIdx].PrmDomainWritten)
			 {
				 /* 161106js001: load PRM record as actual record and set state to CFG
				 *              DomainBoundary contains the PRM record on GSY_PRM_PORT_STATE_CFG
				 */
				 
				 pChSys->Port[PortIdx].PortDataAdjust = pChSys->Port[PortIdx].PrmPortDataAdjust; 	
				 pChSys->Port[PortIdx].DomainBoundary = pChSys->Port[PortIdx].PrmDomainBoundary; 	
				 pChSys->Port[PortIdx].PrmDomainState = GSY_PRM_PORT_STATE_CFG;
				 pChSys->Port[PortIdx].PrmDomainWritten = LSA_FALSE;
				 PortDataAdjustWritten = LSA_TRUE;
				 
				 /* Build Forwarding Mode by negation of PrmDomainBoundary
				 */
				 
				 if (pChSys->Port[PortIdx].DomainBoundary.VersionLow == 1)
				 {
					 SyncIdMaskIn = ~GSY_SWAP32(pChSys->Port[PortIdx].PrmDomainBoundary.Boundary.Domain1.DomainBoundaryIngress.Dword);
					 SyncIdMaskOut = ~GSY_SWAP32(pChSys->Port[PortIdx].PrmDomainBoundary.Boundary.Domain1.DomainBoundaryEgress.Dword);
				 }
				 else
				 {
					 SyncIdMaskIn = ~GSY_SWAP32(pChSys->Port[PortIdx].PrmDomainBoundary.Boundary.Domain.DomainBoundary.Dword);
					 SyncIdMaskOut = SyncIdMaskIn;
				 }
			 }
			 else
			 {
				 /* 2049154: No actual Record present
				 * Default: All is ON
				 */
				 pChSys->Port[PortIdx].PrmDomainState = GSY_PRM_PORT_STATE_INI;
				 SyncIdMaskIn = 0;	// AP01382171: gsy_usr.c(4426): warning C4245: '=' : conversion from 'int' to 'compiler_switch_type_uint32', signed/unsigned mismatch
				 SyncIdMaskIn = ~SyncIdMaskIn;
				 SyncIdMaskOut = SyncIdMaskIn;
			 }
			 /*	161106js001: configure port forwarding now
			 * -> because of asynchronous return of gsy_FwdPortForwardCtrl()
			 */
			 pChSys->Port[PortIdx].PrmDomainCfg = LSA_FALSE;
			 
			 pChSys->PortFwd[PortIdx].FwdMode = SyncIdMaskOut;
			 pChSys->PortInp[PortIdx].FwdMode = SyncIdMaskIn;
			 //TODO:-OS-				RespVal = gsy_FwdPortForwardCtrl(pChSys, (LSA_UINT16)(PortIdx+1), SyncIdMaskIn, SyncIdMaskOut, gsy_PrmCommitEnd);
			 if (GSY_RSP_OK != RespVal)
				 break;
		 }
	 }
	 
	 if ((GSY_RSP_OK != RespVal) && (GSY_RSP_OK_ACTIVE != RespVal))
	 {
		 //ERROR
	 }
 }
 
 if (GSY_RSP_OK == RespVal) 
 {
	 /*  Callback Upper Commit-RQB and start delay measurement at all ports
	 */
	 gsy_CallbackUsr(pChSys->pChUsr, pChSys->pCommitRbu, GSY_RSP_OK);

	 if(GSY_SYNC_ROLE_SLAVE != pChSys->pChUsr->SyncRole)
	 {
	 	 startPTP(pChSys->pChUsr);
	 }
	 
	 /* 301106lrg001: Do not start delay measurement an EDD without timestamps
	 * 190608lrg001: but store record on EDDS
	 if (pChSys->RxTxNanos != 0)
	 */
	 {
		 for (PortIdx = 0; PortIdx < pChSys->PortCount; PortIdx++)
		 {
			 /* 2049154: Default: no DelayBoundary is set
			 */
			 pChSys->Port[PortIdx].DelayBoundary = LSA_FALSE;
			 pChSys->Port[PortIdx].PrmPeerState = GSY_PRM_PORT_STATE_INI;
			 
			 if (pChSys->Port[PortIdx].PrmPeerWritten)
			 {
				 /* Copy Subblock into buffer (A) and update DelayBoundary
				 */
				 pChSys->Port[PortIdx].PrmPeerState = GSY_PRM_PORT_STATE_CFG;
				 pChSys->Port[PortIdx].PeerBoundary = pChSys->Port[PortIdx].PrmPeerBoundary;
				 BitBoundary = GSY_SWAP32(pChSys->Port[PortIdx].PeerBoundary.Boundary.Peer.PeerBoundary.Dword);
				 
				 if (BitBoundary & 2)
				 {
					 /* Bit1 = 1 stopps the delay measurement
					 */
					 pChSys->Port[PortIdx].DelayBoundary = LSA_TRUE;
				 }
				 if (!PortDataAdjustWritten)
					 pChSys->Port[PortIdx].PortDataAdjust = pChSys->Port[PortIdx].PrmPortDataAdjust; 	
			 }
		 }
	 }
	 
	 if (pChSys->PrmFwdInd)
	 {
		 /* Current service remains running:
		 * repeat last Indication as  acknowledgment after Callback
		 * 110806js001: bandwidth possibly has to be activated -> LSA_TRUE
		 *
		 */
		 gsy_SyncUserInd(pChSys->pChUsr, LSA_TRUE, LSA_TRUE);
	 }
	 
	 /* Inform CM that we are ready to sync */
	 pChSys->pChUsr->Event.SyncEvent = GSY_SYNC_EVENT_SYNC_OK; 
	 gsy_SyncUserInd(pChSys->pChUsr, LSA_FALSE, LSA_TRUE);
	 
	 if (    (pChSys->pChUsr->PrmSyncIdChanged) 
			 || ((pChSys->pChUsr->PrmSubdomainInd)
			 && ((GSY_HW_TYPE_SLAVE_HW != (pChSys->HardwareType & GSY_HW_TYPE_SLAVE_MSK))) )  )
	 {
		 
		 if (1 == (GSY_SYNC_ID_SUPPORTED & 1))
		 {	// SyncId 0 is supported
			 pDriftClock = pChSys->Drift[GSY_SYNCID_CLOCK];
			 gsy_PrmCommitEndDiag(pChSys, pDriftClock, GSY_SYNCID_CLOCK);
		 }
		 
		 if (2 == (GSY_SYNC_ID_SUPPORTED & 2))
		 {	// SyncId 1 is supported
			 pDriftTime = pChSys->Drift[GSY_SYNCID_TIME];
			 gsy_PrmCommitEndDiag(pChSys, pDriftTime, GSY_SYNCID_TIME);
		 }
		 
		 switch (pChSys->pChUsr->SyncId)
		 {
		 case GSY_SYNCID_CLOCK:
			 
			 if (GSY_SYNCID_CLOCK != pDriftClock->Diag.SyncId)
			 {//SynId has changed
				 pDriftClock->Diag.SyncId = GSY_SYNCID_CLOCK;
				 
				 if (!LSA_HOST_PTR_ARE_EQUAL(pDriftTime, LSA_NULL))
				 {
					 pDriftTime->Diag.SyncId = GSY_SYNCID_NONE;
					 pDriftTime->Diag.MasterMacAddr = NoMacAddr;
				 }
			 }
			 
			 gsy_DiagUserInd(GSY_DIAG_SOURCE_SUBDOMAIN, pChSys, pDriftClock, 
											 0      /* LSA_UINT32: MasterSeconds */, 
											 0      /* LSA_UINT32: MasterNanoseconds */, 
											 0      /* LSA_INT32: Offset */, 
											 0      /* LSA_INT32: AdjustInterval */, 
											 0      /* LSA_INT32: UserValue1 */, 
											 0      /* LSA_INT32: UserValue2 */, 
											 0      /* LSA_INT32: SetTimeHigh */, 
											 0      /* LSA_INT32: SetTimeLow */, 
											 0	    /* LSA_INT16: PortId */);
			 break;
			 
		 case GSY_SYNCID_TIME:
			 
			 if (GSY_SYNCID_TIME != pDriftTime->Diag.SyncId)
			 {//SynId has changed
				 pDriftTime->Diag.SyncId = GSY_SYNCID_TIME;
				 
				 if (!LSA_HOST_PTR_ARE_EQUAL(pDriftClock, LSA_NULL))
				 {
					 pDriftClock->Diag.SyncId = GSY_SYNCID_NONE;
					 pDriftClock->Diag.MasterMacAddr = NoMacAddr;
				 }
			 }
			 
			 gsy_DiagUserInd(GSY_DIAG_SOURCE_SUBDOMAIN, pChSys, pDriftTime, 
											 0      /* LSA_UINT32: MasterSeconds */, 
											 0      /* LSA_UINT32: MasterNanoseconds */, 
											 0      /* LSA_INT32: Offset */, 
											 0      /* LSA_INT32: AdjustInterval */, 
											 0      /* LSA_INT32: UserValue1 */, 
											 0      /* LSA_INT32: UserValue2 */, 
											 0      /* LSA_INT32: SetTimeHigh */, 
											 0      /* LSA_INT32: SetTimeLow */, 
											 0	    /* LSA_INT16: PortId */);
			 break;
			 
		 default:
			 
			 if (!LSA_HOST_PTR_ARE_EQUAL(pDriftClock, LSA_NULL))      //lint !e774 Boolean within 'if' always evaluates to True, it depends on the define GSY_SYNC_ID_SUPPORTED, HM 20.05.2016
			 {
				 pDriftClock->Diag.SyncId = GSY_SYNCID_NONE;
				 pDriftClock->Diag.MasterMacAddr = NoMacAddr;
			 }
			 
			 if (!LSA_HOST_PTR_ARE_EQUAL(pDriftTime, LSA_NULL))      //lint !e774 Boolean within 'if' always evaluates to True, it depends on the define GSY_SYNC_ID_SUPPORTED, HM 20.05.2016
			 {
				 pDriftTime->Diag.SyncId = GSY_SYNCID_NONE;
				 pDriftTime->Diag.MasterMacAddr = NoMacAddr;
			 }
			 
			 break;
		 }
		 
		 pChSys->pChUsr->PrmSubdomainInd  = LSA_FALSE;
		 pChSys->pChUsr->PrmSyncIdChanged = LSA_FALSE;
	 }
	 
	 RespVal = gsy_PrmPrepare(pChSys->pChUsr, LSA_NULL);
 }
}
							 
/*****************************************************************************/
/* Internal access function: gsy_PrmCommitEndDiag()                          */
/* Send subdomain diagnosis to user                                          */
/*****************************************************************************/
LSA_VOID GSY_LOCAL_FCT_ATTR gsy_PrmCommitEndDiag(
GSY_CH_SYS_PTR			pChSys,
GSY_DRIFT_PTR			pDrift,
LSA_UINT8				DriftId)
{	
	pDrift->Diag.Subdomain = LSA_FALSE;
	/* 221106lrg001: update Diagnosis data and send Indication
	* 061009lrg001: not on CLOCK or TIME sync if in harware
	*/
	if (!GSY_SUBDOMAIN_IS_NULL(pChSys->pChUsr->SyncData.SubdomainID))
	{
		if (((GSY_SYNCID_CLOCK == pChSys->pChUsr->SyncId) || (GSY_SYNCID_TIME == pChSys->pChUsr->SyncId))
				&&  (DriftId == pChSys->pChUsr->SyncId))
		{
			pDrift->Diag.Subdomain  = LSA_TRUE;
		}
	}
	/* 150107lrg001: enable drift measurement to new master on subdomain change
	* 070108lrg003: first set Diag.Subdomain because of diagnosis in gsy_DriftSet() 
	* 131210lrg001: no drift reset if PLL sync is running
	* 051015lrg001: no drift reset if slave runs in hardware
	*/
	if (((GSY_SYNCID_CLOCK == pDrift->Diag.SyncId) || (GSY_SYNCID_TIME == pDrift->Diag.SyncId))
			&&  (GSY_HW_TYPE_SLAVE_HW != (pChSys->HardwareType & GSY_HW_TYPE_SLAVE_MSK))
				&& !pChSys->pChUsr->SyncPLLActive)
	{
//-OS--OS--OS--OS--OS--OS--OS-		gsy_DriftReset(pChSys, pDrift, pChSys->pChUsr->SyncId);
	}
	
	pDrift->Diag.StateSync = LSA_FALSE;
	if ((GSY_SLAVE_STATE_SYNC == pChSys->pChUsr->Slave.State)
			||  (GSY_MASTER_STATE_PRIMARY == pChSys->pChUsr->Master.State))
	{
		if ((GSY_SYNCID_CLOCK == pChSys->pChUsr->SyncId) || (GSY_SYNCID_TIME == pChSys->pChUsr->SyncId))
		{
			pDrift->Diag.StateSync = LSA_TRUE;
		}
	}
	
	//In case of changing the SyncId, 
	//it is neccessary to clean out the previous diagnosis BEFORE Diag.SyncId is set to GSY_SYNCID_NONE
	gsy_DiagUserInd(GSY_DIAG_SOURCE_SUBDOMAIN, pChSys, pDrift, 
									0      /* LSA_UINT32: MasterSeconds */, 
									0      /* LSA_UINT32: MasterNanoseconds */, 
									0      /* LSA_INT32: Offset */, 
									0      /* LSA_INT32: AdjustInterval */, 
									0      /* LSA_INT32: UserValue1 */, 
									0      /* LSA_INT32: UserValue2 */, 
									0      /* LSA_INT32: SetTimeHigh */, 
									0      /* LSA_INT32: SetTimeLow */, 
									0	   /* LSA_INT16: PortId */);
	
}

///*****************************************************************************/
///*  end of file GSY_USR.C                                                    */
///*****************************************************************************/
							 