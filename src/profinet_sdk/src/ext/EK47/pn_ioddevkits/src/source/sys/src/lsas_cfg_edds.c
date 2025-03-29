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
/*  C o m p o n e n t     &C: PnIODDevkits                              :C&  */
/*                                                                           */
/*  F i l e               &F: lsas_cfg_edds.c                           :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/


/**
 * @file    lsas_cfg_edds.c
 * @brief   LSAS interface wrapper for EDDS lower level
 * @author  Tobias Weber
 * @version
 * @date    20.08.2013
 *
 * [BSP] <=> lsas_cfg_edds.c <=> [EDDS]
 *
 */

/*****************************************************************************/
#define LSAS_MODULE_ID  8

//#define INTERFACE_EXT_ATTR

#include "compiler.h"
#if( (PNIOD_PLATFORM & PNIOD_PLATFORM_DKSW) && !defined(EDDS_LLIF_EXT))
	#include "os.h"
	#include "os_taskprio.h"

    #include "lsas_inc.h"
    #include "lsas_int.h"
	#include "lsas_dat.h"

	#if PNIO_BIG_ENDIAN
		#undef _LITTLE_ENDIAN  /* insert here because "ertec200.h" takes use of it */
		#define  _BIG_ENDIAN
	#else
		#define _LITTLE_ENDIAN  /* insert here because "ertec200.h" takes use of it */
		#undef  _BIG_ENDIAN
	#endif

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname       :    lsas_edds_alloc_timer                          +*/
/*+  Input/Output       :    LSA_UINT16  EDDS_LOCAL_MEM_ATTR *  ret_val_ptr  +*/
/*+                          LSA_UINT16  EDDS_LOCAL_MEM_ATTR *  timer_id_ptr +*/
/*+                          LSA_UINT16                        timer_type   +*/
/*+                          LSA_UINT16                        time_base    +*/
/*+  Result             :    LSA_VOID                                       +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  timer_base         : timer base value                                  +*/
/*+                       LSA_TIME_BASE_1MS                                 +*/
/*+                       LSA_TIME_BASE_10MS                                +*/
/*+                       LSA_TIME_BASE_100MS                               +*/
/*+                       LSA_TIME_BASE_1S                                  +*/
/*+                       LSA_TIME_BASE_10S                                 +*/
/*+                       LSA_TIME_BASE_100S                                +*/
/*+  timer_id_ptr       : Pointer to timer id (returnvalue)                 +*/
/*+  timer_type         : type of timer                                     +*/
/*+                       LSA_TIMER_TYPE_ONE_SHOT                           +*/
/*+                       LSA_TIMER_TYPE_CYCLIC                             +*/
/*+  ret_val_ptr        : returncode: LSA_RET_OK            timer has been  +*/
/*+                                                         allocated         +*/
/*+                                   LSA_RET_ERR_NO_TIMER  no timer has    +*/
/*+                                                         been allocated  +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Allocate a timer                                          +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID  lsas_edds_alloc_timer( LSA_UINT16 * ret_val_ptr, LSA_UINT16 * timer_id_ptr, LSA_UINT16 timer_type, LSA_UINT16 time_base)
{
   *ret_val_ptr = (LSA_UINT16) OsAllocTimer (timer_id_ptr, timer_type, time_base, edds_timeout);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname       :    lsas_edds_start_timer                          +*/
/*+  Input/Output       :    LSA_UINT16  EDDS_LOCAL_MEM_ATTR *  ret_val_ptr  +*/
/*+                          LSA_UINT16                        timer_id     +*/
/*+                          LSA_USER_ID_TYPE                  user_id      +*/
/*+                          LSA_UINT16                        time         +*/
/*+  Result             :    LSA_VOID                                       +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  timer_id           : ID of timer                                       +*/
/*+  user_id            : ID of edd                                         +*/
/*+  time               : running time                                      +*/
/*+  ret_val_ptr        : returncode                                        +*/
/*+                             LSA_RET_OK                  timer has been    +*/
/*+                                                         started            +*/
/*+                             LSA_RET_OK_TIMER_RESTARTED  timer has been    +*/
/*+                                                         restarted        +*/
/*+                             LSA_RET_ERR_PARAM           timer hasnï¿½t    +*/
/*+                                                         been started    +*/
/*+                                                         because of wrong+*/
/*+                                                         timer-id        +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Starts a timer                                            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID  lsas_edds_start_timer( LSA_UINT16 *  ret_val_ptr, LSA_UINT16 timer_id, LSA_USER_ID_TYPE user_id, LSA_UINT16 time )
{
    *ret_val_ptr = (LSA_UINT16) OsStartTimer (timer_id,  (LSA_UINT32) user_id.uvar32, time);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname       :    lsas_edds_stop_timer                           +*/
/*+  Input/Output       :    LSA_UINT16  EDDS_LOCAL_MEM_ATTR *  ret_val_ptr  +*/
/*+                          LSA_UINT16                        timer_id     +*/
/*+  Result             :    LSA_VOID                                       +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  timer_id           : ID of timer                                       +*/
/*+  ret_val_ptr        : returncode                                        +*/
/*+                             LSA_RET_OK                    timer has been+*/
/*+                                                           stopped        +*/
/*+                             LSA_RET_OK_TIMER_NOT_RUNNING  timer was not    +*/
/*+                                                           running; timer+*/
/*+                                                           is stopped    +*/
/*+                             LSA_RET_ERR_PARAM             timer hasnï¿½t  +*/
/*+                                                           stopped       +*/
/*+                                                           because       +*/
/*+                                                           of wrong      +*/
/*+                                                           timer-id      +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Stops  a timer                                            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID  lsas_edds_stop_timer( LSA_UINT16 *  ret_val_ptr, LSA_UINT16 timer_id)
{
    *ret_val_ptr = (LSA_UINT16) OsStopTimer (timer_id);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname       :    lsas_edds_free_timer                           +*/
/*+  Input/Output       :    LSA_UINT16   *  ret_val_ptr  					+*/
/*+                          LSA_UINT16                        timer_id     +*/
/*+  Result             :    LSA_VOID                                       +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  timer_id           : ID of timer                                       +*/
/*+  ret_val_ptr        : returncode                                        +*/
/*+                           LSA_RET_OK                    timer has been  +*/
/*+                                                         deallocated        +*/
/*+                           LSA_RET_ERR_TIMER_IS_RUNNING  because timer is+*/
/*+                                                         running timer has*/
/*+                                                         not been        +*/
/*+                                                         deallocated        +*/
/*+                           LSA_RET_ERR_PARAM             no deallocation    +*/
/*+                                                         because of wrong+*/
/*+                                                         timer-id        +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: free   a timer                                            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID  lsas_edds_free_timer( LSA_UINT16 * ret_val_ptr, LSA_UINT16 timer_id )
{
    *ret_val_ptr = (LSA_UINT16) OsFreeTimer (timer_id);
}

void  lsas_edd_scheduler_task (void)
{
  void* pVoid = LSA_NULL;
  PNIO_UINT32 start, end;
  PNIO_UINT32 lastStart;
  LSA_BOOL kindOfCall;

  OsWaitOnEnable();

  while (1)
  {
    OsReadMessageBlocked (&pVoid, lsas_data.edds.tskid_eddScheduler);

    kindOfCall = (LSA_BOOL) pVoid;

    if ( lsas_data.edd_hxdb_set )
    	edds_scheduler ( (EDDS_HANDLE const ) lsas_com_get_hddb(), kindOfCall );
/*
    if ( ( start-lastStart ) > 4000 )
    	PNIO_printf ("%d : edds shed start: %d us, end: %d us, duration: %d us\n", (start-lastStart) , start, end, ( end-start ) );
*/
    lastStart = start;
  }
}

PNIO_VOID   lsas_edds_invokeScheduler( PNIO_VOID )
{
	// intermediate call of edds_scheduler
	OsSendMessage  (lsas_data.edds.tskid_eddScheduler, (void *) 1, OS_MBX_PRIO_NORM);
}

PNIO_VOID   edds_timeout( PNIO_UINT16 timer_id, PNIO_UINT32  user_id)
{
	// regular cyclic call of edds_scheduler
	OsSendMessage  (lsas_data.edds.tskid_eddScheduler, (void *) 0, OS_MBX_PRIO_NORM);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    lsas_edds_ll_open                           +*/
/*+  Input/Output          :    EDDS_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+                             EDDS_UPPER_DPB_PTR_TYPE          pDPB       +*/
/*+  Result                :    LSA_RESULT                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to Device Description Block (Device-management)   +*/
/*+  pDPB       : Pointer to Device Parameter Block                         +*/
/*+                                                                         +*/
/*+  Result     :           EDD_STS_OK                                      +*/
/*+                         EDD_STS_ERR_PARAM                               +*/
/*+                         EDDS_STS_ERR_RESOURCES                          +*/
/*+                         EDD_STS_ERR_ALIGN                               +*/
/*+                         others.                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Initialize LL Ethernetcontroller using DPB parameters     +*/
/*+                                                                         +*/
/*+               See DPB documentation.                                    +*/
/*+                                                                         +*/
/*+               global:                                                   +*/
/*+                                                                         +*/
/*+               pDPB->RxBufferCnt     : number of rx-buffers/descriptors  +*/
/*+               pDPB->TxBufferCnt     : number of tx-buffers/descriptors  +*/
/*+               pDPB->RxBufferSize    : rx-buffers size                   +*/
/*+               pDPB->RxBufferOffset  : rx-buffer offset                  +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+               pDDB->pGlob->LLHandle contains LLIF device structure!     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_open (LSA_VOID_PTR_TYPE 		pLLManagement,
						 	  EDDS_HANDLE				hDDB,
						 	  EDDS_UPPER_DPB_PTR_TYPE   pDPB,
							  LSA_UINT32				TraceIdx,
							  EDDS_LOCAL_EDDS_LL_CAPS_PTR_TYPE pCaps)
{
    PNIO_UINT32 Status;
    PNIO_UINT16 result;
	LSA_USER_ID_TYPE user_id;

	LSA_UNUSED_ARG (pLLManagement);
	LSA_UNUSED_ARG (hDDB);

	pCaps->HWFeatures = 0;
	//pCaps->SWFeatures = EDDS_LL_CAPS_SWF_SRT | EDDS_LL_CAPS_SWF_SEND_ASYNCHRONOUS;
	pCaps->PortCnt    = LSAS_CFG_PORT_CNT;
	pCaps->MaxFrameSize = 1536;

	Status = ENIC_Open (pDPB->TxBufferCnt, pDPB->RxBufferCnt, EDD_CFG_RX_FRAME_BUFFER_LENGTH/*pDPB->RxBufferSize*/, 0/*pDPB->RxBufferOffset*/);


    Status = OsCreateThread ((void  (*)(void))lsas_edd_scheduler_task, (PNIO_UINT8*) "Task of edds scheduler", TASK_PRIO_EDD, &lsas_data.edds.tskid_eddScheduler);
    PNIO_APP_ASSERT(Status == PNIO_OK, "ERROR: OsCreateThread\n");
    Status = OsCreateMsgQueue (lsas_data.edds.tskid_eddScheduler);
    PNIO_APP_ASSERT(Status == PNIO_OK, "ERROR: OsCreateThread\n");
    Status = OsStartThread (lsas_data.edds.tskid_eddScheduler);
    PNIO_APP_ASSERT(Status == PNIO_OK, "ERROR: OsCreateThread\n");

    lsas_edds_alloc_timer (&result, &lsas_data.edds.timer_id, LSA_TIMER_TYPE_CYCLIC, LSA_TIME_BASE_1MS);

	if (result != LSA_RET_OK)
	{
	    EDDS_LL_FUNCTION_TRACE_01(LSA_TRACE_LEVEL_NOTE_LOW, "OUT: EDDS_LL_ALLOC_TIMER(), Status: 0x%X", result);
	}

	user_id.uvar32 = 0;

	lsas_edds_start_timer (&result, lsas_data.edds.timer_id, user_id, 1);

	if (result != LSA_RET_OK)
	{
	    EDDS_LL_FUNCTION_TRACE_01(LSA_TRACE_LEVEL_NOTE_LOW, "OUT: EDDS_LL_START_TIMER(), Status: 0x%X", result);
	}

	return ( (Status == PNIO_OK) ? EDD_STS_OK : EDD_STS_ERR_SERVICE );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    lsas_edds_ll_recurringTask                  +*/
/*+  Input/Output          :    LSA_VOID_PTR_TYPE          pLLManagement    +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID   lsas_edds_ll_recurringTask ( LSA_VOID_PTR_TYPE       pLLManagement)
{
	LSA_UNUSED_ARG (pLLManagement);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    lsas_edds_ll_setup                              +*/
/*+  Input/Output          :    EDDS_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+                             EDDS_UPPER_DSB_PTR_TYPE          pDSB       +*/
/*+  Result                :    LSA_RESULT                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to Device Description Block (Device-management)   +*/
/*+  pDSB       : Pointer to Device Setup Block                             +*/
/*+                                                                         +*/
/*+  Result     :           EDD_STS_OK                                      +*/
/*+                         EDD_STS_ERR_PARAM                               +*/
/*+                         EDDS_STS_ERR_RESOURCES                          +*/
/*+                         EDD_STS_ERR_TIMEOUT                             +*/
/*+                         EDD_STS_ERR_HW                                  +*/
/*+                         others.                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Setup LL Ethernetcontroller and start it.                 +*/
/*+                                                                         +*/
/*+               pDDB->pGlob->LLHandle contains LLIF device structure!     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_setup (EDDS_LOCAL_DDB_PTR_TYPE pDDB,
							   EDDS_UPPER_DSB_PTR_TYPE pDSB,
							   EDDS_LOCAL_LL_HW_PARAM_PTR_TYPE pStartupLinkParam )
{
	LSA_UINT32 i;
    LSA_RESULT Status;
    LSA_UNUSED_ARG(pDDB);
    LSA_UNUSED_ARG(pDSB);

	Status = (LSA_RESULT) ENIC_Setup();			// activate the SRT Interface

	Bsp_GetMacAddr ((PNIO_UINT8*)&pStartupLinkParam->MACAddress);

	for ( i = 0; i < pStartupLinkParam->PortCnt ; i++ )
	{
		Bsp_GetPortMacAddr((PNIO_UINT8*)&pStartupLinkParam->MACAddressPort[i], i+1 );
		pStartupLinkParam->IsWireless[i] = LSA_FALSE;
		pStartupLinkParam->LinkSpeedMode[i] = EDD_LINK_AUTONEG;
		pStartupLinkParam->PHYPower[i] = EDDS_PHY_POWER_ON;
	}

    EDDS_LL_FUNCTION_TRACE_01(LSA_TRACE_LEVEL_NOTE_LOW, "OUT: EDDS_LL_SETUP(), Status: 0x%X", Status);

	return ( (Status == PNIO_OK) ? EDD_STS_OK : EDD_STS_ERR_SERVICE );
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    lsas_edds_ll_shutdown                           +*/
/*+  Input/Output          :    EDDS_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+  Result                :    LSA_RESULT                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to Device Description Block (Device-management)   +*/
/*+                                                                         +*/
/*+  Result     :           EDD_STS_OK                                      +*/
/*+                         EDD_STS_ERR_TIMEOUT                             +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Shutdown LL Ethernetcontroller. Stops it                  +*/
/*+                                                                         +*/
/*+               pDDB->pGlob->LLHandle contains LLIF device structure!     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_shutdown (EDDS_LOCAL_DDB_PTR_TYPE pDDB)
{
	LSA_RESULT  Status;
    LSA_UNUSED_ARG (pDDB);

    Status = (LSA_RESULT) ENIC_Shutdown();

    EDDS_LL_FUNCTION_TRACE_01(LSA_TRACE_LEVEL_NOTE_LOW, "OUT: EDDS_LL_Shutdown(), Status: 0x%X", Status);

	return ( (Status == PNIO_OK) ? EDD_STS_OK : EDD_STS_ERR_SERVICE );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    lsas_edds_ll_close                          +*/
/*+  Input/Output          :    EDDS_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+  Result                :    LSA_RESULT                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to Device Description Block (Device-management)   +*/
/*+                                                                         +*/
/*+  Result     :           EDD_STS_OK                                      +*/
/*+                         EDD_STS_ERR_TIMEOUT                             +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Closes LL Ethernetcontroller. Stops it                    +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_close (EDDS_LOCAL_DDB_PTR_TYPE pDDB)
{
    PNIO_UINT32 Status;
	LSA_UNUSED_ARG (pDDB);
    Status = (LSA_RESULT) ENIC_Close ();

	return ( (Status == PNIO_OK) ? EDD_STS_OK : EDD_STS_ERR_SERVICE );
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDS2_LL_SEND                               +*/
/*+  Input/Output          :    EDDS_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+                             EDD_UPPER_MEM_PTR pBufferAddr               +*/
/*+                             PNIO_UINT32        Length                   +*/
/*+  Result                :    LSA_RESULT                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       :  Pointer to Device Description Block (Device-management)  +*/
/*+  pTxBuf     :  Pointer to buffer with Frame-netto-data to send          +*/
/*+                This address will be used and put to the controller!     +*/
/*+                (The data is not copied!) So the buffer must not be      +*/
/*+                accessed until it is released!                           +*/
/*+  Datlen     :  Number of Bytes pBuffer points to  (max. 1514/1518 Byte) +*/
/*+                                                                         +*/
/*+  Result     :           EDD_STS_OK                                      +*/
/*+                         EDD_STS_ERR_PARAM                               +*/
/*+                         EDD_STS_ERR_RESOURCES                           +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Start sending a Frame. The frame (pBufferAddr) is given   +*/
/*+               to  the low-level function for further handling. Length   +*/
/*+               is the length of the frame. The frame has to include      +*/
/*+               mac-addresses, type/len, and data. FCS is added be low-   +*/
/*+               level function (normally be controller)                   +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+               pDDB->pGlob->LLHandle contains LLIF device structure!     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_send (EDDS_LOCAL_DDB_PTR_TYPE  pDDB,
							  EDD_UPPER_MEM_PTR_TYPE   pTxBuf,
							  PNIO_UINT32              Datlen,
							  PNIO_UINT32              PortID)
{
        LSA_RESULT Status;
        Status = (LSA_RESULT) ENIC_Send (lsas_com_get_hddb(), pTxBuf,  Datlen, PortID);
	    return (Status);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    lsas_edds_ll_send_sts                       +*/
/*+  Input/Output          :    EDDS_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+  Result                :    LSA_RESULT                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to Device Description Block (Device-management)   +*/
/*+                                                                         +*/
/*+  Result     :           EDD_STS_OK           : frame successul transm.  +*/
/*+                         EDD_STS_OK_NO_DATA   : no frame transmitted     +*/
/*+                         ... transmit errors                             +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Checks for finished transmissions. If a transmission was  +*/
/*+               finished, EDD_STS_OK is returned. If no (more) finished   +*/
/*+               EDD_STS_OK_NO_DATA is returned.                           +*/
/*+                                                                         +*/
/*+               pDDB->pGlob->LLHandle contains LLIF device structure!     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_send_sts(EDDS_LOCAL_DDB_PTR_TYPE  pDDB)
{
	LSA_RESULT  Status;

	LSA_UNUSED_ARG (pDDB);

	Status = (LSA_RESULT) ENIC_GetSendStatus ();
	return(Status);
}


//#ifdef EDDS_CFG_SEND_BLOCKED
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    lsas_edds_ll_send_trigger                   +*/
/*+  Input/Output          :    EDDS_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+  Result                :    --                                          +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       :  Pointer to Device Description Block (Device-management)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Starts transmission of frames applied by EDD_LL_SEND().   +*/
/*+               Only called if EDD_CFG_SEND_BLOCKED is defined (edd_cfg.h)+*/
/*+                                                                         +*/
/*+               pDDB->pGlob->LLHandle contains LLIF device structure!     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID lsas_edds_ll_send_trigger (EDDS_LOCAL_DDB_PTR_TYPE  pDDB)
{
	LSA_UNUSED_ARG (pDDB);
    //LSA_TRACE_00  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "EDDS_LL_SEND_TRIGGER not supported\n");
}
//#endif


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    lsas_edds_ll_reinit_tx                      +*/
/*+  Input/Output          :    EDDS_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+  Result                :    LSA_RESULT                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to Device Description Block (Device-management)   +*/
/*+                                                                         +*/
/*+  Result     :           EDD_STS_OK                                      +*/
/*+                         EDD_STS_ERR_TIMEOUT  :                          +*/
/*+                         EDDS_STS_ERR_RESOURCES                          +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Reinits Transmit-unit of ethernetcontroller. Called after +*/
/*+               a transmit-timeout occured                                +*/
/*+                                                                         +*/
/*+               pDDB->pGlob->LLHandle contains LLIF device structure!     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_reinit_tx (EDDS_LOCAL_DDB_PTR_TYPE  pDDB)
{
	LSA_RESULT  Status;

	LSA_UNUSED_ARG (pDDB);

	Status = (LSA_RESULT) ENIC_ReinitTransmitUnit();
	return(Status);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname  :    lsas_edds_ll_recv                                   +*/
/*+  Input/Output  :    EDDS_LOCAL_DDB_PTR_TYPE               pDDB          +*/
/*+                     EDDS_UPPER_MEM_PTR EDDS_LOCAL_MEM_ATTR *pBufferAddr +*/
/*+                     EDDS_LOCAL_MEM_U32_PTR_TYPE           pLength       +*/
/*+                     EDDS_LOCAL_MEM_U32_PTR_TYPE           pPortID       +*/
/*+  Result        :    LSA_RESULT                                          +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       :  Pointer to Device Description Block (Device-management)  +*/
/*+  pBufferAddr:  Pointer to pointer to buffer with Frame-netto-data       +*/
/*+                received if EDD_STS_OK.                                  +*/
/*+  pLength    :  Pointer to address with length of frame received.        +*/
/*+  pPortID    :  Pointer to address with PortID of frame received.        +*/
/*+                                                                         +*/
/*+  Result     :  EDD_STS_OK                                               +*/
/*+                EDD_STS_OK_NO_DATA                                       +*/
/*+                EDD_STS_ERR_RX                                           +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This functions checks if a frame was received. If not it  +*/
/*+               returns EDD_STS_OK_NO_DATA. If a frame was received, the  +*/
/*+               functions returns pBufferAddr and pLength and sets        +*/
/*+               EDD_STS_OK or EDD_STS_ERR_RX. The buffer returned must    +*/
/*+               not be accessed anymore by the LLIF! The EDDS calls       +*/
/*+               EDDS_LL_RECV_PROVIDE() to provide a new receive buffer    +*/
/*+               to the LL. EDD_STS_ERR_RX is set, if the frame has a      +*/
/*+               error (e.g. wrong CRC). The EDDS discards this frame      +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+               pDDB->pGlob->LLHandle contains LLIF device structure!     +*/
/*+                                                                         +*/
/*+ Use of this function within EDDS:                                       +*/
/*+                                                                         +*/
/*+                    Receive Interrupt                                    +*/
/*+                           |                                             +*/
/*+                           |                                             +*/
/*+       +------------------>+                                             +*/
/*+       |                   |                                             +*/
/*+       |                EDDS_LL_RECV()                                   +*/
/*+       |           +-------+------------+                                +*/
/*+       |           |                    |                                +*/
/*+       |     Frame present            No Frame                           +*/
/*+       |     (good or bad)              |                                +*/
/*+       |           |                    |                                +*/
/*+       |   handle buffer if good        |                                +*/
/*+       |           |                    |                                +*/
/*+       |    EDDS_LL_RECV_PROVIDE()      |                                +*/
/*+       |           |                    |                                +*/
/*+       +-----------+                    |                                +*/
/*+                                       End (wait for receive interrupt)  +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_recv (EDDS_LOCAL_DDB_PTR_TYPE                      pDDB,
							  EDD_UPPER_MEM_PTR_TYPE  EDDS_LOCAL_MEM_ATTR *pBufferAddr,
							  EDDS_LOCAL_MEM_U32_PTR_TYPE                  pLength,
							  EDDS_LOCAL_MEM_U32_PTR_TYPE                  pPortID)
{
    LSA_UNUSED_ARG(pDDB);
    return ( (LSA_RESULT) ENIC_Recv(pBufferAddr, pLength, pPortID) );
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname  :    lsas_edds_ll_recv_provide                           +*/
/*+  Input/Output  :    EDDS_LOCAL_DDB_PTR_TYPE               pDDB          +*/
/*+                     EDD_UPPER_MEM_PTR_TYPE                pBufferAddr   +*/
/*+  Result        :    LSA_RESULT                                          +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       :  Pointer to Device Description Block (Device-management)  +*/
/*+  pBufferAddr:  Pointer to ...                                           +*/
/*+                                                                         +*/
/*+  Result     :  EDD_STS_OK                                               +*/
/*+                PNIO_NOT_OK                                              +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description:                                                           +*/
/*+                                                                         +*/
/*+  This function provides a new data block from EDD to ENIC,              +*/
/*+  to copy received ethernet telegrams.                                   +*/
/*+  The Buffer address ist stored in a buffer list EnicDat.pRxBuf[].       +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_recv_provide (EDDS_LOCAL_DDB_PTR_TYPE  	  pDDB,
									  EDD_UPPER_MEM_PTR_TYPE      pBufferAddr)
{
	LSA_UNUSED_ARG(pDDB);
	return ( (LSA_RESULT) ENIC_RecvProvide(pBufferAddr) );
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :  lsas_edds_ll_get_link_state_setup             +*/
/*+  Input/Output          :  EDDS_LOCAL_DDB_PTR_TYPE          pDDB         +*/
/*+                           LSA_UINT32                       PortID       +*/
/*+                           EDD_LOCAL_MEM_U8_PTR_TYPE        pLinkStat    +*/
/*+                           EDD_LOCAL_MEM_U8_PTR_TYPE        pPHYPower    +*/
/*+                           LSA_BOOL EDDS_LOCAL_MEM_ATTR *   pIsWirele    +*/
/*+  Result                :  LSA_RESULT                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to Device Description Block (Device-management)   +*/
/*+  PortID     : Port to set Link for                                      +*/
/*+  pLinkStat  : LinkStatus setup                                          +*/
/*+               EDD_LINK_AUTONEG                                          +*/
/*+               EDD_LINK_100MBIT_HALF                                     +*/
/*+               EDD_LINK_100MBIT_FULL                                     +*/
/*+               EDD_LINK_10MBIT_HALF                                      +*/
/*+               EDD_LINK_10MBIT_FULL                                      +*/
/*+  pPHYPower  : PHY Power setup                                           +*/
/*+               EDD_PHY_POWER_ON                                          +*/
/*+               EDD_PHY_POWER_OFF                                         +*/
/*+  pIsWireless: Is this port a wireless port?                             +*/
/*+               LSA_FALSE  : no                                           +*/
/*+               LSA_TRUE   : yes                                          +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_SEQUENCE                                      +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Reads the actual Setup of LinkStat and PHYPower           +*/
/*+               (Note: This are the Setup-values!)                        +*/
/*+                                                                         +*/
/*+               pDDB->pGlob->LLHandle contains LLIF device structure!     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_get_link_state_setup (EDDS_LOCAL_DDB_PTR_TYPE          pDDB,
											  LSA_UINT32                       PortID,
											  EDDS_LOCAL_MEM_U8_PTR_TYPE       pLinkStat,
											  EDDS_LOCAL_MEM_U8_PTR_TYPE       pPHYPower,
											  LSA_BOOL EDDS_LOCAL_MEM_ATTR *   pIsWireless)
{
	LSA_RESULT  Status = EDD_STS_OK;
    *pIsWireless = LSA_FALSE;

	if (1) //( pDDB->pGlob->HWIsSetup )
	{
		if (  PortID != 0 )
		{
			*pLinkStat = EDD_LINK_AUTONEG;
			*pPHYPower = EDDS_PHY_POWER_ON;
		}
		else
		{
			Status = EDD_STS_ERR_PARAM;
		}
	}
	else
	{
		Status = EDD_STS_ERR_SEQUENCE;
	}
	return(Status);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname       :    lsas_edds_ll_get_link_state                    +*/
/*+  Input/Output       :    EDDS_LOCAL_DDB_PTR_TYPE             pDDB       +*/
/*+                          LSA_UINT32                          PortID     +*/
/*+                          EDD_UPPER_GET_LINK_STATUS_PTR_TYPE  pLinkStat  +*/
/*+                          EDD_LOCAL_MEM_U32_PTR_TYPE          pPhyType   +*/
/*+                          EDD_LOCAL_MEM_U32_PTR_TYPE          pPortStatus+*/
/*+                          EDD_LOCAL_MEM_U32_PTR_TYPE          pLinkSpe.. +*/
/*+                                                                         +*/
/*+  Result             :    LSA_RESULT                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to Device Description Block (Device-management)   +*/
/*+  PortID     : Port to get link status from                              +*/
/*+  pLinkStat  : Pointer to LinkStatus to be filled                        +*/
/*+  pPhyType   : Pointer to address for actual PhyType (see edd_usr.h)     +*/
/*+  pPortStatus: Pointer to address for actual PortStatus (see edd_usr.h)  +*/
/*+  pAutonegCapAdvertised: Pointer to  AutonegCapAdvertised                +*/
/*+  pLinkSpeedModeConfigured: Pointer to LinkSpeedModeConfigured           +*/
/*+                                                                         +*/
/*+  Result     :           EDD_STS_OK                                      +*/
/*+                           :                                             +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Gets LinkStatus from Low-Level-functions                  +*/
/*+                                                                         +*/
/*+               pDDB->pGlob->LLHandle contains LLIF device structure!     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_get_link_state (EDDS_LOCAL_DDB_PTR_TYPE             pDDB,
                                        LSA_UINT32                          PortID,
                                        EDD_UPPER_GET_LINK_STATUS_PTR_TYPE  pLinkStat,
                                        EDDS_LOCAL_MEM_U16_PTR_TYPE         pMAUType,
                                        EDDS_LOCAL_MEM_U8_PTR_TYPE          pMediaType,
                                        EDDS_LOCAL_MEM_U32_PTR_TYPE         pPortStatus,
                                        EDDS_LOCAL_MEM_U32_PTR_TYPE         pAutonegCapAdvertised,
                                        EDDS_LOCAL_MEM_U8_PTR_TYPE          pLinkSpeedModeConfigured )
{
	LSA_RESULT  Status = EDD_STS_OK;
    PNIO_UINT8  LinkState = 0;
    PNIO_UINT8  LinkMode = 0;
    PNIO_UINT8  PhyType;

	LSA_UNUSED_ARG (pDDB);

    if (PortID > IOD_CFG_PDEV_NUMOF_PORTS)     // PortID = 1....EDD_CFG_PORT_CNT
    {
        LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "PortID=%d not valid\n\n", PortID);
    }

	Bsp_GetPDevLinkState (PortID, &LinkState);
	pLinkStat->Status   = LinkState;

	Bsp_GetPDevLinkMode(PortID, &LinkMode, (PNIO_UINT8*)&PhyType);

    *pMAUType    = EDD_MAUTYPE_UNKNOWN;
    *pMediaType  = EDD_MEDIATYPE_UNKNOWN;

    switch (LinkMode)
    {
        case PDEV_LINK_AUTONEG:
             pLinkStat->Mode     = EDD_LINK_UNKNOWN;
             pLinkStat->Speed    = EDD_LINK_UNKNOWN;
             *pLinkSpeedModeConfigured  = EDD_LINK_AUTONEG;
             break;
        case PDEV_LINK_1GBIT_HALF:
             pLinkStat->Mode     = EDD_LINK_MODE_HALF;
             pLinkStat->Speed    = EDD_LINK_SPEED_1000;
             *pLinkSpeedModeConfigured  = EDD_LINK_1GBIT_HALF;
             break;
        case PDEV_LINK_1GBIT_FULL:
             pLinkStat->Mode     = EDD_LINK_MODE_FULL;
             pLinkStat->Speed    = EDD_LINK_SPEED_1000;
             *pLinkSpeedModeConfigured  = EDD_LINK_1GBIT_FULL;
             break;
        case PDEV_LINK_100MBIT_HALF:
             pLinkStat->Mode     = EDD_LINK_MODE_HALF;
             pLinkStat->Speed    = EDD_LINK_SPEED_100;
             *pLinkSpeedModeConfigured  = EDD_LINK_100MBIT_HALF;
             break;
        case PDEV_LINK_100MBIT_FULL:
             pLinkStat->Mode     = EDD_LINK_MODE_FULL;
             pLinkStat->Speed    = EDD_LINK_SPEED_100;
             *pLinkSpeedModeConfigured  = EDD_LINK_100MBIT_FULL;
             *pMAUType    		 = LLDP_TLV_MAU_TYPE_100BASETXFD;
             *pMediaType  		 = EDD_MEDIATYPE_COPPER_CABLE;
             break;
        case PDEV_LINK_10MBIT_HALF:
             pLinkStat->Mode     = EDD_LINK_MODE_HALF;
             pLinkStat->Speed    = EDD_LINK_SPEED_10;
             *pLinkSpeedModeConfigured  = EDD_LINK_10MBIT_HALF;
             break;
        case PDEV_LINK_10MBIT_FULL:
             pLinkStat->Mode     = EDD_LINK_MODE_FULL;
             pLinkStat->Speed    = EDD_LINK_SPEED_10;
             *pLinkSpeedModeConfigured  = EDD_LINK_10MBIT_FULL;
             break;
        default:
             LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "invalid link mode=%d\n", LinkMode);
             break;
    }

    *pPortStatus = EDD_PORT_PRESENT;

    *pAutonegCapAdvertised     = EDD_AUTONEG_CAP_100BASETXFD; // || EDD_AUTONEG_CAP_100BASETX


    #if (PNIOD_PLATFORM & PNIOD_PLATFORM_WIN)
        PNIO_printf ("EDD_LL_GET_LINK_STATE (PortID %d)\n", PortID);
    #endif

    return(Status);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    lsas_edds_ll_check_mautype                  +*/
/*+  Input/Output          :    EDDS_LOCAL_DDB_PTR_TYPE             pDDB    +*/
/*+                             LSA_UINT32                          PortID  +*/
/*+                             LSA_UINT16                          MAUType +*/
/*+                             LSA_UINT8                           PHYPower+*/
/*+                             EDDS_LOCAL_MEM_U8_PTR_TYPE         pLinkStat+*/
/*+  Result                :    LSA_RESULT                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to Device Description Block (Device-management)   +*/
/*+  PortID     : Port to check LinkStat for (1..EDDS_MAX_PORT_CNT)         +*/
/*+  MAUType    : MAUType to be checked                                     +*/
/*+               EDD_MAUTYPE_UNKNOWN (=0)                                  +*/
/*+               else IEEE MAUType (1..x)                                  +*/
/*+  PHYPower   : Power of PHY to be checked                                +*/
/*+               EDDS_PHY_POWER_ON                                         +*/
/*+               EDDS_PHY_POWER_OFF                                        +*/
/*+               EDDS_PHY_POWER_UNCHANGED(do not check)                    +*/
/*+  pLinkStat  : LinkState  setup                                          +*/
/*+               EDD_LINK_100MBIT_HALF                                     +*/
/*+               EDD_LINK_100MBIT_FULL                                     +*/
/*+               EDD_LINK_10MBIT_HALF                                      +*/
/*+               EDD_LINK_10MBIT_FULL                                      +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Check if the MAUType/PHYPower can be set for Port         +*/
/*+                                                                         +*/
/*+               If a MAUType is present (<> EDD_MAUTYPE_UNKNOWN) check if +*/
/*+               it is supported and return pLinkStat with Speed/Mode for  +*/
/*+               MAUType.                                                  +*/
/*+                                                                         +*/
/*+               If PHYPower is <> EDDS_PHY_POWER_UNCHANGED check if PHY   +*/
/*+               power is supported.                                       +*/
/*+                                                                         +*/
/*+               pDDB->pGlob->LLHandle contains LLIF device structure!     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_check_mautype (EDDS_LOCAL_DDB_PTR_TYPE         pDDB,
									   LSA_UINT32                      PortID,
									   LSA_UINT16                      MAUType,
									   LSA_UINT8                       PHYPower,
									   EDDS_LOCAL_MEM_U8_PTR_TYPE      pLinkStat)
{
	LSA_RESULT Status = EDD_STS_OK;

	EDDS_LL_FUNCTION_TRACE_04(LSA_TRACE_LEVEL_NOTE_LOW,
							  "IN : EDDS_LL_CHECK_MAUTYPE(pDDB: 0x%X, PortID: %d, MAUType: %d, Power: 0x%X)",
							  pDDB, PortID, MAUType, PHYPower);
	LSA_UNUSED_ARG(pDDB);
	LSA_UNUSED_ARG(MAUType);
	LSA_UNUSED_ARG(PHYPower);
	LSA_UNUSED_ARG(PortID);
	LSA_UNUSED_ARG(pLinkStat);

	EDDS_LL_FUNCTION_TRACE_01(LSA_TRACE_LEVEL_NOTE_LOW,	"OUT: EDDS_LL_CHECK_MAUTYPE(), Status: 0x%X", Status);
	return(Status);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    lsas_edds_ll_get_stats                      +*/
/*+  Input/Output          :    EDDS_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+                             EDD_UPPER_GET_STATS_PTR_TYPE        pStats  +*/
/*+  Result                :    LSA_RESULT                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to Device Description Block (Device-management)   +*/
/*+  pStats     : Pointer to Statistics Structure to be filled              +*/
/*+                                                                         +*/
/*+  Result     :           EDD_STS_OK                                      +*/
/*+                           :                                             +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Gets statistics from Low-Level-functions                  +*/
/*+                                                                         +*/
/*+               pDDB->pGlob->LLHandle contains LLIF device structure!     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT  lsas_edds_ll_get_stats (EDDS_LOCAL_DDB_PTR_TYPE         pDDB,
			 					    LSA_UINT32                      PortID,
									EDDS_LOCAL_STATISTICS_PTR_TYPE  pStats)
{
    LSA_RESULT Status;
    BSP_STATISTIC_DATA  BspStat;

    Status = ENIC_GetStats (PortID, (PNIO_VOID*)&BspStat);
    pStats->InOctets        = BspStat.RxBytesTotal;
    pStats->InUcastPkts     = BspStat.RxFramesUcast;
    pStats->InNUcastPkts    = BspStat.RxFramesNUcast;
    pStats->OutOctets       = BspStat.TxBytesTotal;
    pStats->OutUcastPkts    = BspStat.TxFramesUcast;
    pStats->OutNUcastPkts   = BspStat.TxFramesNUcast;
    return (Status);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :  lsas_edds_ll_get_caps                         +*/
/*+  Input/Output          :  LSA_VOID_PTR_TYPE             pLLManagement   +*/
/*+                           EDDS_LOCAL_EDDS_LL_CAPS_PTR_TYPE pCaps [out]  +*/
/*+  Result                :  LSA_RESULT                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pLLManagement: Pointer to LLIF specific management structure           +*/
/*+  PortID     : PortID to read statistics for                             +*/
/*+               0: local interface                                        +*/
/*+               1..x: Port                                                +*/
/*+                                                                         +*/
/*+               Note: if only one port present, local interface is equal  +*/
/*+                     to the port statistic                               +*/
/*+                                                                         +*/
/*+  pStats     : Pointer to Statistics Structure to be filled              +*/
/*+                                                                         +*/
/*+  Result     :           EDD_STS_OK                                      +*/
/*+                           :                                             +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Gets statistics from Low-Level-functions                   */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_get_caps (LSA_VOID_PTR_TYPE 			    pLLManagement,
			   				   	  EDDS_LOCAL_EDDS_LL_CAPS_PTR_TYPE	pCaps)
{
	LSA_RESULT Status = EDD_STS_OK;
	LSA_UNUSED_ARG(pLLManagement);
	//PACKET32_LL_HANDLE_TYPE* handle = (PACKET32_LL_HANDLE_TYPE*)pLLManagement;

	//EDDS_LL_TRACE_00(handle->TraceIdx,LSA_TRACE_LEVEL_CHAT,
	//		"IN :PACKET32_LL_GET_CAPS()");

	//LSA_UNUSED_ARG(handle);
	pCaps->HWFeatures = 0;
	//pCaps->SWFeatures = EDDS_LL_CAPS_SWF_SRT | EDDS_LL_CAPS_SWF_SEND_ASYNCHRONOUS;
	pCaps->PortCnt    = LSAS_CFG_PORT_CNT;
	pCaps->MaxFrameSize = 1536;

	//EDDS_LL_TRACE_01(handle->TraceIdx,LSA_TRACE_LEVEL_CHAT,
	//		"OUT:PACKET32_LL_GET_CAPS(), Status: 0x%X",
	//		Status);
	return (Status);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :  lsas_edds_ll_get_mac_addr                     +*/
/*+  Input/Output          :  EDDS_LOCAL_DDB_PTR_TYPE           pDDB        +*/
/*+                           EDD_MAC_ADR_TYPE EDDS_LOCAL_MEM_ATTR *pMACAddr+*/
/*+  Result                :  LSA_RESULT                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to Device Description Block (Device-management)   +*/
/*+  PortID     : 0    : MAC Address from Station (Interface)               +*/
/*+               1..EDD_MAX_PORT_CNT : Port MAC Address                    +*/
/*+  pMCAddr    : Pointer to 6 Byte MAC Address                             +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Gets local MAC-address of Ethernetcontroller              +*/
/*+                                                                         +*/
/*+               pDDB->pGlob->LLHandle contains LLIF device structure!     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_get_mac_addr (EDDS_LOCAL_DDB_PTR_TYPE              pDDB,
									  LSA_UINT32                           PortID,
									  EDD_MAC_ADR_TYPE EDD_UPPER_MEM_ATTR *pMACAddr)
{
	LSA_UNUSED_ARG (pDDB);
	return ( (LSA_RESULT) ((PortID == 0) ?
			   Bsp_GetMacAddr     ((PNIO_UINT8*)pMACAddr) : 			// read device mac address
			   Bsp_GetPortMacAddr ((PNIO_UINT8*)pMACAddr, PortID) ));	// read port mac address
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    lsas_edds_ll_mc_enable                      +*/
/*+  Input/Output          :    EDDS_LOCAL_DDB_PTR_TYPE              pDDB   +*/
/*+                             EDD_MAC_ADR_TYPE EDD_UPPER_MEM_ATTR *pMCAddr+*/
/*+  Result                :    LSA_RESULT                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to Device Description Block (Device-management)   +*/
/*+  pMCAddr    : Pointer to 6 Byte Multicastaddress to Enable              +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_RESOURCE      : no more free MC entrys        +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Enables Multicastaddress                                  +*/
/*+                                                                         +*/
/*+               pDDB->pGlob->LLHandle contains LLIF device structure!     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_mc_enable ( EDDS_LOCAL_DDB_PTR_TYPE              pDDB,
								    EDD_MAC_ADR_TYPE EDD_UPPER_MEM_ATTR *pMCAddr)
{
	LSA_UNUSED_ARG (pDDB);
	return ( (LSA_RESULT) ENIC_MC_Enable (&pMCAddr->MacAdr[0]));	// enable the multicast address
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    lsas_edds_ll_mc_disable                     +*/
/*+  Input/Output          :    EDDS_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+                             LSA_BOOL                        DisableAll  +*/
/*+                             EDD_MAC_ADR_TYPE EDD_UPPER_MEM_ATTR *pMCAddr+*/
/*+  Result                :    LSA_RESULT                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to Device Description Block (Device-management)   +*/
/*+  DisableAll  :LSA_TRUE: frees all Multicast addresses. pMCAddr          +*/
/*+                         not used.                                       +*/
/*+               LSA_FALSE:free pMACAddr only                              +*/
/*+  pMCAddr    : Pointer to 6 Byte Multicastaddress to Disable if          +*/
/*+               DisableAll = LSA_FALSE                                    +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Disables Multicastaddress                                 +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_mc_disable (EDDS_LOCAL_DDB_PTR_TYPE              pDDB,
									LSA_BOOL                             DisableAll,
									EDD_MAC_ADR_TYPE EDD_UPPER_MEM_ATTR *pMCAddr)
{
	LSA_UNUSED_ARG (pDDB);
	// ptr=0 -> disable all
	return ( (LSA_RESULT) ENIC_MC_Disable ((DisableAll) ? NULL : (void*)pMCAddr));	// enable the multicast address
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    lsas_edds_ll_set_link_state                 +*/
/*+  Input/Output          :    EDDS_LOCAL_DDB_PTR_TYPE              pDDB   +*/
/*+                             LSA_UINT32                          PortID  +*/
/*+                             LSA_UINT8                           LinkStat+*/
/*+                             LSA_UINT8                           PHYPower+*/
/*+  Result                :    LSA_RESULT                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to Device Description Block (Device-management)   +*/
/*+  PortID     : Port to set Link for                                      +*/
/*+  LinkStat   : LinkStatus to be setup                                    +*/
/*+               EDD_LINK_AUTONEG                                          +*/
/*+               EDD_LINK_100MBIT_HALF                                     +*/
/*+               EDD_LINK_100MBIT_FULL                                     +*/
/*+               EDD_LINK_10MBIT_HALF                                      +*/
/*+               EDD_LINK_10MBIT_FULL                                      +*/
/*+               EDD_LINK_UNCHANGED     (do not change LinkStat)           +*/
/*+  PHYPower   : Power of PHY                                              +*/
/*+               EDD_PHY_POWER_ON                                          +*/
/*+               EDD_PHY_POWER_OFF                                         +*/
/*+               EDD_PHY_POWER_UNCHANGED(do not change power)              +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_SEQUENCE                                      +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Set Link to given values (only if hardware is setup)      +*/
/*+                                                                         +*/
/*+               pDDB->pGlob->LLHandle contains LLIF device structure!     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_set_link_state (EDDS_LOCAL_DDB_PTR_TYPE pDDB,
										LSA_UINT32              PortID,
										LSA_UINT8               LinkStat,
										LSA_UINT8               EddPhyPower)
{
    LSA_RESULT  Status = EDD_STS_OK;
    PNIO_UINT8 PhyPower = PDEV_PHY_POWER_UNCHANGED;
	LSA_UNUSED_ARG (pDDB);

    switch (EddPhyPower)
    {
        case EDDS_PHY_POWER_ON:        PhyPower = PDEV_PHY_POWER_ON;         break;
        case EDDS_PHY_POWER_OFF:       PhyPower = PDEV_PHY_POWER_OFF;        break;
        case EDDS_PHY_POWER_UNCHANGED: PhyPower = PDEV_PHY_POWER_UNCHANGED;  break;
        default :  return(EDD_STS_ERR_PARAM);                               break;
    }

    #if (PNIOD_PLATFORM & PNIOD_PLATFORM_WIN)
        PNIO_printf ("EDD_LL_SET_LINK_STATE (PortID %d, LinkStat %d, PHYPower %d\n",
                      PortID, LinkStat, PhyPower);
    #endif

    if (PortID > IOD_CFG_PDEV_NUMOF_PORTS)     // PortID = 1....IOD_CFG_PDEV_NUMOF_PORTS
    {
	    return(EDD_STS_ERR_PARAM);  // not supported
    }

	if (1) //( pDDB->pGlob->HWIsSetup )
    {
	    switch (LinkStat){
		    case EDD_LINK_AUTONEG:
			    Bsp_SetPDevLinkMode (PortID, PDEV_LINK_AUTONEG, PhyPower);
			    break;
		    case EDD_LINK_100MBIT_HALF:
			    Bsp_SetPDevLinkMode (PortID, PDEV_LINK_100MBIT_HALF, PhyPower);
			    break;
		    case EDD_LINK_100MBIT_FULL:
			    Bsp_SetPDevLinkMode (PortID, PDEV_LINK_100MBIT_FULL, PhyPower);
			    break;
		    case EDD_LINK_10MBIT_HALF:
			    Bsp_SetPDevLinkMode (PortID, PDEV_LINK_10MBIT_HALF, PhyPower);
			    break;
		    case EDD_LINK_10MBIT_FULL:
			    Bsp_SetPDevLinkMode (PortID, PDEV_LINK_10MBIT_FULL, PhyPower);
			    break;
		    case EDD_LINK_UNCHANGED:
			    break;
		    default:
			    Status = EDD_STS_ERR_PARAM;
                LSA_TRACE_01  (TRACE_SUBSYS_IOD_SYSADAPT, LSA_TRACE_LEVEL_ERROR, "invalid linkstat=%d\n", LinkStat);
			    break;
	    }
    }
    else
    {
        Status = EDD_STS_ERR_SEQUENCE;
    }
    return(Status);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    lsas_edds_ll_led_backup_mode                +*/
/*+  Input/Output          :    LSA_VOID_PTR_TYPE             pLLManagement +*/
/*+                             LSA_UINT32                      PortID      +*/
/*+  Result                :    LSA_RESULT                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pLLManagement: Pointer to LLIF specific management structure           +*/
/*+  PortID     : Port to set LED mode for                                  +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_TIMEOUT                                       +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Backup LED-Mode for PHY LEDs                              +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+               pLLManagement contains LLIF device structure!             +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_led_backup_mode (LSA_VOID_PTR_TYPE pLLManagement, LSA_UINT32 PortID)
{
    LSA_RESULT Status;

	LSA_UNUSED_ARG(pLLManagement);
	LSA_UNUSED_ARG(PortID);

	Status = EDD_STS_OK;

	LSAS_EDD_PHY_LED_BLINK_BEGIN (PortID);  // start BLINK
	return (Status);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    lsas_edds_ll_led_restore_mode               +*/
/*+  Input/Output          :    LSA_VOID_PTR_TYPE             pLLManagement +*/
/*+                             LSA_UINT32                      PortID      +*/
/*+  Result                :    LSA_RESULT                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pLLManagement: Pointer to LLIF specific management structure           +*/
/*+  PortID     : Port to set LED mode for                                  +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_TIMEOUT                                       +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: RESTORE LED-Mode for PHY LEDs                             +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+               pLLManagement contains LLIF device structure!             +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_led_restore_mode (LSA_VOID_PTR_TYPE pLLManagement, LSA_UINT32 PortID)
{
    LSA_RESULT Status;

	LSA_UNUSED_ARG(pLLManagement);
	LSA_UNUSED_ARG(PortID);

	Status = EDD_STS_OK;

	LSAS_EDD_PHY_LED_BLINK_END (PortID);
	return (Status);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    lsas_edds_ll_led_set_mode                   +*/
/*+  Input/Output          :    EDDS_LOCAL_DDB_PTR_TYPE          pDDB       +*/
/*+                             LSA_UINT32                      PortID      +*/
/*+                             LSA_UINT16                      LEDNr       +*/
/*+                             LSA_UINT16                      LEDMode     +*/
/*+  Result                :    LSA_RESULT                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to Device Description Block (Device-management)   +*/
/*+  PortID     : Port to set LED mode for                                  +*/
/*+  LEDNr      : Number of LED to set Mode for (0..2)                      +*/
/*+  LEDMode    : Mode of LED to set                                        +*/
/*+                                                                         +*/
/*+               EDD_LED_MODE_SPEED                                        +*/
/*+               EDD_LED_MODE_TX                                           +*/
/*+               EDD_LED_MODE_RX                                           +*/
/*+               EDD_LED_MODE_COLLISION                                    +*/
/*+               EDD_LED_MODE_LINK                                         +*/
/*+               EDD_LED_MODE_DUPLEX                                       +*/
/*+               EDD_LED_MODE_RX_TX                                        +*/
/*+               EDD_LED_MODE_ON                                           +*/
/*+               EDD_LED_MODE_OFF                                          +*/
/*+               EDD_LED_MODE_BLINK                                        +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_TIMEOUT                                       +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Sets LED-Mode for PHY LEDs (here:DUMMY, LED is set by BSP +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_led_set_mode (EDDS_LOCAL_DDB_PTR_TYPE	pDDB,
									  LSA_UINT32                PortID,
									  LSA_BOOL                  On)
{
	LSA_UINT16 LEDNo = (LSA_UINT16) PortID;
    PNIO_UINT32 Status;
	LSA_UNUSED_ARG (pDDB);

	Status = Bsp_EbSetLed (PNIO_LED_BLINK, On);
    if (Status == PNIO_OK)
        return (EDD_STS_OK);
    else
        return (EDD_STS_ERR_TIMEOUT);
}

#ifdef LLIF_CFG_SWITCH_SUPPORT
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname   :    lsas_edds_ll_switch_set_port_state                 +*/
/*+  Input/Output   :    EDDS_LOCAL_DDB_PTR_TYPE                pDDB        +*/
/*+                      EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE  pPortStates +*/
/*+  Result         :    LSA_RESULT                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to Device Description Block (Device-management)+*/
/*+  pPortStates   : Pointer to array with Port ids/modes                   +*/
/*+                                                                         +*/
/*+  possible PortStates:                                                   +*/
/*+                                                                         +*/
/*+         EDD_PORT_STATE_DISABLE                                          +*/
/*+         EDD_PORT_STATE_BLOCKING                                         +*/
/*+         EDD_PORT_STATE_LISTENING                                        +*/
/*+         EDD_PORT_STATE_LEARNING                                         +*/
/*+         EDD_PORT_STATE_FORWARDING                                       +*/
/*+         EDD_PORT_STATE_UNCHANGED                                        +*/
/*+                                                                         +*/
/*+  Details to parameters see edds-service EDD_SRV_SWITCH_SET_PORT_STATE   +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_NOT_SUPPORTED                                 +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Controls port mode for switching                          +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+               pDDB->pGlob->LLHandle contains LLIF device structure!     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_switch_set_port_state (EDDS_LOCAL_DDB_PTR_TYPE                 pDDB,
											   EDD_UPPER_SWI_SET_PORT_STATE_PTR_TYPE   pPortStates)
{
    #if (IOD_CFG_PDEV_NUMOF_PORTS >= 2)
        PNIO_UINT32 i;
        LSA_UNUSED_ARG (pDDB);

        for (i = 0; i < IOD_CFG_PDEV_NUMOF_PORTS; i++)
        {
            switch (pPortStates->PortIDState[i])
            {
                case    EDD_PORT_STATE_BLOCKING:
                case    EDD_PORT_STATE_FORWARDING:
                        Bsp_SetPortState (i+1, pPortStates->PortIDState[i]);
                        break;
                case    EDD_PORT_STATE_UNCHANGED:   // do nothing for this port
                        break;

                default:
                        EDDS_LL_FUNCTION_TRACE_02(LSA_TRACE_LEVEL_ERROR,
                        		"EDDS_LL_SWITCH_SET_PORT_STATE (P%d, State=%d not supported",
                                    i+1, pPortStates->PortIDState[i]);

                        return (EDD_STS_ERR_NOT_SUPPORTED);
                        break;
            }
          }
    #else
        LSA_UNUSED_ARG (pDDB);
        LSA_UNUSED_ARG (pPortStates);
    #endif

    return (LSA_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname   :    lsas_edds_ll_switch_multicast_fwd_ctrl             +*/
/*+  Input/Output   :    EDDS_LOCAL_DDB_PTR_TYPE         pDDB               +*/
/*+                      EDD_UPPER_PORTID_MODE_PTR_TYPE  pPortIDModeArray   +*/
/*+                      LSA_UINT16                      PortIDModeCnt      +*/
/*+                      LSA_UINT16                      MACAddrPrio        +*/
/*+                      LSA_UINT16                      MACAddrGroup       +*/
/*+                      LSA_UINT16                      MACAddrLow         +*/
/*+  Result         :    LSA_RESULT                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to Device Description Block (Device-management)   +*/
/*+  pPortIDModeArray   :   Pointer to array with Port ids/modes            +*/
/*+  PortIDModeCnt      :   Number of arrayelements within pPortIDModeArr.  +*/
/*+  MACAddrPrio        :   MAC Address priority                            +*/
/*+  MACAddrGroup       :   MAC Address group                               +*/
/*+  MACAddrLow         :   MAC Address low bits. Depends on Group          +*/
/*+                                                                         +*/
/*+  Details to parameters see edds-service EDD_SRV_MULTICAST_FWD_CTRL      +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_RESOURCE                                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Controls forwarding of multicastadresses.                 +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+               pDDB->pGlob->LLHandle contains LLIF device structure!     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_switch_multicast_fwd_ctrl ( EDDS_LOCAL_DDB_PTR_TYPE        pDDB,
													EDD_UPPER_PORTID_MODE_PTR_TYPE pPortIDModeArray,
													LSA_UINT16                     PortIDModeCnt,
													LSA_UINT16                     MACAddrPrio,
													LSA_UINT16                     MACAddrGroup,
													LSA_UINT32                     MACAddrLow)
{
    LSA_RESULT  Ret = LSA_OK;

    #if (IOD_CFG_PDEV_NUMOF_PORTS >= 2)
        PNIO_UINT32 i;
        LSA_UNUSED_ARG (pDDB);

        for (i = 0; i < PortIDModeCnt; i++)
        {
            if (Ret == LSA_OK)
            {
                Ret = Bsp_SwitchMcForwControl  (
                            i+1,                         // port number 1..IOD_CFG_PDEV_NUMOF_PORTS
                            (pPortIDModeArray+i)->Mode,  // port mode
                            MACAddrPrio,
                            MACAddrGroup,
                            MACAddrLow);
            }
        }
    #else
        LSA_UNUSED_ARG (pDDB);
        LSA_UNUSED_ARG (pPortIDModeArray);
        LSA_UNUSED_ARG (PortIDModeCnt);
        LSA_UNUSED_ARG (MACAddrPrio);
        LSA_UNUSED_ARG (MACAddrGroup);
        LSA_UNUSED_ARG (MACAddrLow);
    #endif

    return (Ret);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname   :    lsas_edds_ll_switch_flush_filtering_db             +*/
/*+  Input/Output   :    EDDS_LOCAL_DDB_PTR_TYPE                pDDB        +*/
/*+  Result         :    LSA_RESULT                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to Device Description Block (Device-management)+*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_NOT_SUPPORTED                                 +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Flushs filtering data base and tx-queues of forwarding    +*/
/*+                                                                         +*/
/*+               pDDB->pGlob->LLHandle contains LLIF device structure!     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_switch_flush_filtering_db (EDDS_LOCAL_DDB_PTR_TYPE pDDB)
{
    PNIO_UINT32 Status = LSA_OK;
    LSA_UNUSED_ARG (pDDB);

    #if (IOD_CFG_PDEV_NUMOF_PORTS >= 2)
        Status = Bsp_ClearFDB();
        if (Status != PNIO_OK)
            return (EDD_STS_ERR_NOT_SUPPORTED);
    #endif
    return (PNIO_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname   :    lsas_edds_ll_switch_set_port_redundant             +*/
/*+  Input/Output   :    EDDS_LOCAL_DDB_PTR_TYPE                    pDDB    +*/
/*+                      EDDS_UPPER_SWI_SET_PORT_REDUNDANT_PTR_TYPE pPortRed+*/
/*+  Result         :    LSA_RESULT                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to Device Description Block (Device-management)+*/
/*+  pPortRed      : Pointer to array of all ports.                         +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_NOT_SUPPORTED                                 +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Set redundancy bit (RT-Bit) of port. Only two port can    +*/
/*+               have the RT-Bit at once.                                  +*/
/*+                                                                         +*/
/*+               pDDB->pGlob->LLHandle contains LLIF device structure!     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_switch_set_port_redundant (EDDS_LOCAL_DDB_PTR_TYPE                     pDDB,
												   EDDS_UPPER_SWI_SET_PORT_REDUNDANT_PTR_TYPE  pPortRed)
{
    LSA_UNUSED_ARG (pDDB);
    LSA_UNUSED_ARG (pPortRed);

    #if ((IOD_CFG_PDEV_NUMOF_PORTS >= 2) && (PNIOD_PLATFORM & PNIOD_PLATFORM_WIN))
	{
		int i;
		for (i=0; i < IOD_CFG_PDEV_NUMOF_PORTS; i++)
			PNIO_printf ( "EDDS_LL_SWITCH_SET_PORT_REDUNDANT(Port=%d, Red=%d) \n",
						  i+1, pPortRed->PortID[i]);
	}
    #endif
    return (LSA_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname   :    lsas_edds_ll_switch_set_bridge_mode                +*/
/*+  Input/Output   :    EDDS_LOCAL_DDB_PTR_TYPE                    pDDB    +*/
/*+                      LSA_UINT32                                 Mode    +*/
/*+  Result         :    LSA_RESULT                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to Device Description Block (Device-management)+*/
/*+  Mode          : Pointer to array of all ports.                         +*/
/*+                   EDDS_BRIDGE_MODE_SF     store & forward               +*/
/*+                   EDDS_BRIDGE_MODE_CT     cut through                   +*/
/*+                                                                         +*/
/*+  Details to parameters see edds-service EDDS_SRV_SWITCH_SET_BRIDGE_MODE +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_NOT_SUPPORTED                                 +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Set bridge mode. SF (Store and forward) or CT (CutThrough)+*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+               pDDB->pGlob->LLHandle contains LLIF device structure!     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_switch_set_bridge_mode (EDDS_LOCAL_DDB_PTR_TYPE             pDDB,
												LSA_UINT32                          Mode)
{
    #if (IOD_CFG_PDEV_NUMOF_PORTS >= 2)
        PNIO_UINT32 Status;
        LSA_UNUSED_ARG (pDDB);
        Status = Bsp_SetBridgeMode (Mode);
        if (Status == PNIO_OK)
            return (EDD_STS_OK);
        else
            return (EDD_STS_ERR_NOT_SUPPORTED);
    #else
        LSA_UNUSED_ARG (pDDB);
        LSA_UNUSED_ARG (Mode);
        return (PNIO_OK);
	#endif
}

#ifdef LLIF_CFG_SWITCH_DROP_FRAME_SUPPORT
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname   :    lsas_edds_ll_switch_drop_cnt                       +*/
/*+  Input/Output   :    EDDS_LOCAL_DDB_PTR_TYPE                pDDB        +*/
/*+                      EDD_UPPER_MEM_U32_PTR_TYPE             pDropCnt    +*/
/*+  Result         :    LSA_RESULT                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to Device Description Block (Device-management)+*/
/*+  pDropCnt      : Pointer to ...                                         +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: ...                                                       +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_RESULT  lsas_edds_ll_switch_drop_cnt (LSA_VOID_PTR_TYPE          pDDB,
										  EDD_UPPER_MEM_U32_PTR_TYPE pDropCnt)
{
    LSA_UNUSED_ARG (pDDB);
    LSA_UNUSED_ARG (pDropCnt);
}
#endif /* LLIF_CFG_SWITCH_DROP_FRAME_SUPPORT */

#endif /* LLIF_CFG_SWITCH_SUPPORT */


#ifdef LLIF_CFG_USE_LL_ARP_FILTER
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname   :    lsas_edds_ll_set_arp_filter                        +*/
/*+  Input/Output   :    LSA_VOID_PTR_TYPE             pLLManagement        +*/
/*+                      EDDS_ARP_FRAME_FILTER_PTR     pArpFilter           +*/
/*+  Result         :    LSA_RESULT                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pLLManagement   : Pointer to ...                                       +*/
/*+  pArpFilter      : Pointer to ...                                       +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: ...                                                       +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_set_arp_filter (LSA_VOID_PTR_TYPE 		  pLLManagement,
										EDDS_ARP_FRAME_FILTER_PTR pArpFilter)
{
	LSA_UNUSED_ARG (pLLManagement);
	LSA_UNUSED_ARG (pArpFilter);
}
#endif /* LLIF_CFG_USE_LL_ARP_FILTER */




//###############################################################################################
//###############################################################################################
//###############################################################################################


// EDDS_LL_CHECK_LINK_STATE
// EDDS_LL_GET_PORT_CNT

#if 0
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    lsas_edds_ll_check_link_state                    +*/
/*+  Input/Output          :    EDDS_LOCAL_DDB_PTR_TYPE              pDDB   +*/
/*+                             LSA_UINT32                          PortID  +*/
/*+                             LSA_UINT8                           LinkStat+*/
/*+                             LSA_UINT8                           PHYPower+*/
/*+  Result                :    LSA_RESULT                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to Device Description Block (Device-management)   +*/
/*+  PortID     : Port to check LinkStat for (1..EDD_MAX_PORT_CNT)          +*/
/*+  LinkStat   : LinkStatus to be setup                                    +*/
/*+               EDD_LINK_AUTONEG                                          +*/
/*+               EDD_LINK_100MBIT_HALF                                     +*/
/*+               EDD_LINK_100MBIT_FULL                                     +*/
/*+               EDD_LINK_10MBIT_HALF                                      +*/
/*+               EDD_LINK_10MBIT_FULL                                      +*/
/*+               EDD_LINK_UNCHANGED     (do not check)                     +*/
/*+  PHYPower   : Power of PHY                                              +*/
/*+               EDD_PHY_POWER_ON                                          +*/
/*+               EDD_PHY_POWER_OFF                                         +*/
/*+               EDD_PHY_POWER_UNCHANGED(do not check)                     +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Check if the LinkStat/PHYPower can be set for Port        +*/
/*+                                                                         +*/
/*+               pDDB->pGlob->LLHandle contains LLIF device structure!     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_RESULT lsas_edds_ll_check_link_state(
	EDDS_LOCAL_DDB_PTR_TYPE          pDDB,
	LSA_UINT32                      PortID,
	LSA_UINT8                       LinkStat,
	LSA_UINT8                       PHYPower)
{
	LSA_RESULT Status;
	Status = EDD_STS_OK;

	if (  PortID == 0 )
	{
		Status = EDD_STS_ERR_PARAM;
	}

	LSA_UNUSED_ARG(pDDB);
	LSA_UNUSED_ARG(PHYPower);
	LSA_UNUSED_ARG(LinkStat);

	return(Status);
}

#ifdef LLIF_CFG_PORT_CNT_DYNAMIC
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    lsas_edds_ll_get_port_cnt                        +*/
/*+  Input/Output          :    EDDS_LOCAL_DDB_PTR_TYPE         pDDB        +*/
/*+                             EDD_UPPER_MEM_U32_PTR_TYPE     pPortCnt     +*/
/*+  Result                :    LSA_RESULT                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to Device Description Block (Device-management)   +*/
/*+  pPortCnt   : Pointer to address for PortCnt                            +*/
/*+               PortCnt has to be 1..EDD_MAX_PORT_CNT !                   +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Gets PortCnt (1..EDD_MAX_PORT_CNT) actual in use.         +*/
/*+               This function may be used if the PortCnt is determined    +*/
/*+               at runtime and differs from EDD_MAX_PORT_CNT.             +*/
/*+                                                                         +*/
/*+               Called by EDD core after EDD_LL_SETUP within edd_setup    +*/
/*+                                                                         +*/
/*+               Note: The PortCnt can not be changed while running!       +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID lsas_edds_ll_get_port_cnt(
	EDDS_LOCAL_DDB_PTR_TYPE               pDDB,
	EDD_LOCAL_MEM_U32_PTR_TYPE           pPortCnt)
{
	LSA_UNUSED_ARG(pDDB);
	*pPortCnt = EDD_MAX_PORT_CNT;
}
#endif



#endif



#endif /* ((PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P) || (PNIOD_PLATFORM & PNIOD_PLATFORM_DKSW)) */


/* end of file *************************************************************/

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
