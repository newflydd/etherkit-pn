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
/*  C o m p o n e n t     &C: EDDP (EthernetDeviceDriver for PN-IP)     :C&  */
/*                                                                           */
/*  F i l e               &F: eddp_nrt_isr.c                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP Debugging support functions                 */
/*                          (system dependend)                               */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  24.09.08    SF    initial version.                                       */
/*  20.10.09    lrg   EDDP_NRT_TRACE_BYTE_ARRAY statt EDDP_DUMP_FRAME()      */
/*  24.11.10    JS    added casts to prevent compiler warning                */
#endif
/*****************************************************************************/



/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/

#define LTRC_ACT_MODUL_ID  67
#define EDDP_MODULE_ID     LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_NRT */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"               /* eddp headerfiles */
#include "eddp_int.h"               /* internal header  */
#include "eddp_hwc.h"               /* hardwarefilter code  */
#include "eddp_nrt_inc.h"


EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)




/*===========================================================================*/
/*                        private functions                                  */
/*===========================================================================*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_NRTCancelTxCheckAsynCompletion        +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDD_LOCAL_HDB_PTR_TYPE      pHDB            +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: utility function for asynchronous EDDP_NRTCancel()        +*/
/*+               --> test for completion of the cancel request and return  +*/
/*+                   the stored 'cancel'-RQB to the user                   +*/
/*+         NOTE: call this function only if cancel running in background ! +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR _EDDP_NRTCancelTxCheckAsynCompletion(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDDP_LOCAL_HDB_NRT_PTR_TYPE     pNrtHDB;

    EDDP_ASSERT(!LSA_HOST_PTR_ARE_EQUAL(pHDB, LSA_NULL));
    pNrtHDB = pHDB->pNRT;
    EDDP_ASSERT(!LSA_HOST_PTR_ARE_EQUAL(pNrtHDB, LSA_NULL));

    // ***** test that all pending RQBs in ASIC/HW are finished
    if ( !pNrtHDB->TxInProgCnt )
    {
        if (pNrtHDB->pUserCancelRQB != LSA_NULL)
        {
            // ***** finish the 'cancel' request and update internal state marker
            EDDP_GlbRequestFinish (pHDB, pNrtHDB->pUserCancelRQB, EDD_STS_OK);
            pNrtHDB->pUserCancelRQB     = LSA_NULL;
            pNrtHDB->CancelInProgress   = LSA_FALSE;
        }
    }

    LSA_UNUSED_ARG(pDDB);   // handle unused parameter

}



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRT_ISR_Acyc_Snd_IFA                   +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32                     EventNr      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  EventNr    : Event Number (Interrupt Number)                     (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE after a send ring done  +*/
/*+               interrupt occurred. It is executed within LOW context and +*/
/*+               informs the user about the processed TX frames (in a RQB) +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_NRT_ISR_Acyc_Snd_IFA(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr)
{
#if 1
	//-OS- EDDP_LL_NRT_SEND_DONE(pDDB);
#else

    LSA_UINT32                          Status_Reg;
    LSA_UINT16                          Loops, TxRingIdx;
    #if (EDDP_NRT_RXTX_COUNTER_SUPPORT == 1)
    LSA_UINT32                          Tmp;
    LSA_UINT8                           nPrintError;
    EDD_UPPER_NRT_SEND_PTR_TYPE         pRQBSnd;
    #else
    #ifdef EDDP_MESSAGE
    #ifdef EDDP_PRAGMA_MESSAGE
    #pragma EDDP_PRAGMA_MESSAGE("Warning: EDDP_NRT_ISR_Acyc_Rcv/Snd_IFA()  SW Rx/Tx-statistic disabled !!!")
    #endif /* EDDP_PRAGMA_MESSAGE */
    #endif /* EDDP_MESSAGE */
    #endif
    EDDP_LOCAL_DDB_NRT_PTR_TYPE         pNRTIface;
    EDDP_NRT_TX_RING_TYPE_PTR           pTxHwDescrRingMgm;
    EDD_UPPER_RQB_PTR_TYPE              pRQB;
    EDDP_LOCAL_HDB_NRT_PTR_TYPE         pNrtHDB;
    EDDP_LOCAL_HDB_PTR_TYPE             pHDB;
    PNIP_LOCAL_DMACW_SND_ITEM_PTR_TYPE  pTxRingItem;
    PNIP_DMACW_SND_ITEM_TYPE            SndItem;

    // EDDP_ASSERT_NO_NULL_PTR(pDDB);
    // EDDP_ASSERT_NO_NULL_PTR(pDDB->pIF);

    LSA_UNUSED_ARG(EventNr);    // handle unused parameter

    // read STATUSCHANGE to get reason(s) of this interrupt for own used rings only
    Status_Reg = ( (EDDP_HAL_REG32_READ(pDDB, PNIP_REG_SNDSTATUS_IFA)) & EDDP_NRT_TX_DMACW_MASK );

  //  if (Status_Reg)
    {
        EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_NRT_ISR_Acyc_Snd_IFA(EventNr=0x%X, SNDSTATUS_IFA=0x%X)", 
            EventNr, Status_Reg);

        // clear change indication bit(s) for own used rings
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_SNDSTATUS_IFA, Status_Reg);

        pNRTIface = &pDDB->NRT;
        //EDDP_ASSERT_NO_NULL_PTR(pNRTIface);

        // step through signaled rings
        for ( TxRingIdx=0; TxRingIdx < EDDP_NRT_TX_GRP_CNT; TxRingIdx++ )
        {
            // check if exist ring event
            if ((1<<TxRingIdx) & Status_Reg)
            {
                // at minimum one send request is finished
                pTxHwDescrRingMgm = &pNRTIface->TxHwDescrRingMgm[TxRingIdx];

                EDDP_NRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] ###### EDDP_NRT_ISR_Acyc_Snd_IFA(Tx-Ring=%d): ItemCount=%d, FreeCount=%d, DoneIdx=%d", 
                    TxRingIdx, pTxHwDescrRingMgm->ItemCount, pTxHwDescrRingMgm->FreeCount, pTxHwDescrRingMgm->DoneIdx);

                // EDDP_ASSERT_NO_NULL_PTR(pTxHwDescrRingMgm);
                if (pTxHwDescrRingMgm->DoneIdx < pTxHwDescrRingMgm->ItemCount)
                {
                    // process transmitted items
                    Loops = pTxHwDescrRingMgm->ItemCount - pTxHwDescrRingMgm->FreeCount;

                    if (Loops > pTxHwDescrRingMgm->ItemCount)
                    {
                        // program error SW/ASIC
                        Loops = 0;
                    }
                    nPrintError = 1;

                    while (Loops--)
                    {
                        pTxRingItem = &pTxHwDescrRingMgm->pTxRing[pTxHwDescrRingMgm->DoneIdx];
                        SndItem.SndCfg1 = EDDP_HAL_MEM32_READ(pDDB, (EDD_COMMON_MEM_U32_PTR_TYPE)&pTxRingItem->SndCfg1);
                        // NextDMACWPtr
                        // TimeStamp
                        // FrameBufferPtr
                        // SndCfg2
                        // [pTxRqb, pNrtHDB]
                        pRQB = (EDD_UPPER_RQB_PTR_TYPE)EDDP_HAL_MEM32_READ(pDDB, (EDD_COMMON_MEM_U32_PTR_TYPE)&pTxRingItem->pTxRqb);

                        if ( (SndItem.SndCfg1 & PNIP_DMACW_SND_ITEM_CFG1_OWN_MSK) == (LSA_UINT32)PNIP_DMACW_SND_ITEM_CFG1_OWN_SW )
                        {
                            // item processed by ASIC
                            pNrtHDB = (EDDP_LOCAL_HDB_NRT_PTR_TYPE)EDDP_HAL_MEM32_READ(pDDB, (EDD_COMMON_MEM_U32_PTR_TYPE)&pTxRingItem->pNrtHDB);

                            EDDP_GlbHandleGetHDB (pNrtHDB->EDD_Handle, &pHDB);
                            EDDP_ASSERT_NO_NULL_PTR(pHDB);

                            SndItem.FrameBufferPtr = EDDP_HAL_MEM32_READ(pDDB, (EDD_COMMON_MEM_U32_PTR_TYPE)&pTxRingItem->FrameBufferPtr);

                            EDDP_NRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] ###### EDDP_NRT_ISR_Acyc_Snd_IFA(): DmacwPtr=0x%8.8X, Buffer=0x%8.8X, Len=%d, Result=0x%X", 
                                pTxRingItem, SndItem.FrameBufferPtr,PNIP_DMACW_SND_ITEM_GET_FRMLEN(SndItem), PNIP_DMACW_SND_ITEM_GET_RESULT(SndItem)>>PNIP_DMACW_SND_ITEM_CFG1_RESULT_SHIFT);

                            if (PNIP_DMACW_SND_ITEM_GET_RESULT(SndItem) <= PNIP_DMACW_SND_ITEM_CFG1_RESULT_OK_WARN)
                            {
                                // we use always USE_TYPE standard --> no timestamp before
                                #if (EDDP_NRT_RXTX_COUNTER_SUPPORT == 1)
                                pRQBSnd = (EDD_UPPER_NRT_SEND_PTR_TYPE) pRQB->pParam;

                                if ( ((LSA_UINT8*)pRQBSnd->pBuffer)[0] & 1 )
                                {
                                    // counts BC/MC tx-packets
                                    pNRTIface->Stats.OutNUcastPkts ++;
                                    if ( ((LSA_UINT8*)pRQBSnd->pBuffer)[0] == 0xFF )
                                    {
                                        // counts BC rx-packets
                                        pNRTIface->Stats.OutBroadcastPkts++;
                                    }
                                    else
                                    {
                                        // counts MC rx-packets
                                        pNRTIface->Stats.OutMulticastPkts++;
                                    }
                                }
                                else
                                {
                                    // counts UC tx-packets
                                    pNRTIface->Stats.OutUcastPkts ++;
                                }
                                #endif

                                EDDP_GlbRequestFinish (pHDB, pRQB, EDD_STS_OK);
                            }
                            else
                            {
                                #if (EDDP_NRT_RXTX_COUNTER_SUPPORT == 1)
                                Tmp = PNIP_DMACW_SND_ITEM_GET_RESULT(SndItem);

                                if (Tmp == PNIP_DMACW_SND_ITEM_CFG1_RESULT_ERR_WDT)
                                {
                                    // count timeouts
                                    pNRTIface->Stats.OutDiscards++;

                                    if (PNIP_DMACW_SND_ITEM_IS_WDR_TX_QCW(SndItem))
                                    {
                                        Tmp = PNIP_DMACW_SND_ITEM_CFG1_WDR_BARRIER >> PNIP_DMACW_SND_ITEM_CFG1_WDR_SHIFT;
                                    }
                                    else
                                    {
                                        Tmp = PNIP_DMACW_SND_ITEM_CFG1_WDR_TX_QCW >> PNIP_DMACW_SND_ITEM_CFG1_WDR_SHIFT;
                                    }

                                    if (nPrintError)
                                    {
                                        //  print the first error within the TX ring only
                                        EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN,"[H:--] EDDP_NRT_ISR_Acyc_Snd_IFA(SndWatchdogExpired): WatchDogError=%d (0=Barrier/1=TxQCW) in Tx-Ring(%d)!", 
                                            Tmp, TxRingIdx);
                                        nPrintError--;
                                    }
                                }
                                else
                                {
                                    // other TX errors
                                    pNRTIface->Stats.OutErrors++;

                                    if (nPrintError) // print the first error within the TX ring only
                                    {    
                                        // Wrong Port will be traced with trace level LSA_TRACE_LEVEL_CHAT, the other send errors on the trace level LSA_TRACE_LEVEL_WARN
                                        if(PNIP_DMACW_SND_ITEM_CFG1_RESULT_ERR_PORT == Tmp) 
                                        {
                                            EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] EDDP_NRT_ISR_Acyc_Snd_IFA(SendError=%d): in Tx-Ring(%d) (2=Wrong Port)", 
                                                Tmp>>PNIP_DMACW_SND_ITEM_CFG1_RESULT_SHIFT, TxRingIdx);
                                        }
                                        else    // trace with LSA_TRACE_LEVEL_WARN, error code != wrong port
                                        {
                                            EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN,"[H:--] EDDP_NRT_ISR_Acyc_Snd_IFA(SendError=%d): in Tx-Ring(%d) (3=ResponseErr/5=LenghtErr/6=NotAligned/7=AHBReadErr)", 
                                                Tmp>>PNIP_DMACW_SND_ITEM_CFG1_RESULT_SHIFT, TxRingIdx);
                                        }
                                        nPrintError--;
                                    }
                                }
                                #endif

                                EDDP_GlbRequestFinish (pHDB, pRQB, EDD_STS_ERR_TX);
                            }

                            EDDP_ASSERT(pNrtHDB->TxInProgCnt);
                            pNrtHDB->TxInProgCnt--;

                            if ( (++pTxHwDescrRingMgm->DoneIdx) >= pTxHwDescrRingMgm->ItemCount )
                            {   
                                pTxHwDescrRingMgm->DoneIdx = 0;
                            }

                            pTxHwDescrRingMgm->FreeCount++;

                            // check if cancel running in background
                            if (pNrtHDB->CancelInProgress)
                            {
                                _EDDP_NRTCancelTxCheckAsynCompletion (pDDB, pHDB);
                            }
                        }
                        else
                        {
                            // item not processed by ASIC
                            EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_NRT_ISR_Acyc_Snd_IFA(DmacwPtr=0x%8.8X, pRQB=0x%X): missing OwnerBit=SW", 
                                pTxRingItem, pRQB);
                            break;
                        }
                    }
                }
                // else SW/ASIC error

                // check TX-Queue if queued RQBs exist
                Loops = (LSA_UINT16) pNRTIface->TxQueue[TxRingIdx].TxReqQueue.Count;

                if (Loops)
                {
                    _EDDP_NRTSendFrame (pDDB, TxRingIdx, Loops);
                }
            }
        } // end for loop

        EDDP_NRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_NRT_ISR_Acyc_Snd_IFA()");
    }
#endif
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_NRT_SearchUserChannelIdx              +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_NRT_PTR_TYPE pNRTIface       +*/
/*+                             LSA_UINT16                  HWFrameFilter   +*/
/*+  Result                :    LSA_UINT32                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pNRTIface      : Pointer to NRT-Mgm structure                          +*/
/*+  HWFrameFilter  : received FilterDecisionCode                           +*/
/*+                                                                         +*/
/*+  Result     : index of the userchannel-mgm on success                   +*/
/*+               or EDDP_NRT_MAX_FDC_CNT on failure                        +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: utility function returns the index of the userchannel-mgm +*/
/*+               if HWFrameFilter matches and chanel active                +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_UINT32 _EDDP_NRT_SearchUserChannelIdx(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDDP_LOCAL_DDB_NRT_PTR_TYPE     pNRTIface, 
    LSA_UINT16                      HWFrameFilter)
{
    LSA_UINT32      ChIdx;

    // ***** special develompent channel for dropped frames --> special bit set in HWFrameFilter
    if (HWFrameFilter & EDDP_FDC_DROP_MASK)
    {
        if (pNRTIface->FilterInfo[0].pNrtHDB != LSA_NULL)
        {
            // ***** channel is active
            return (0);
        }
        // ***** channel is not opened
        return (EDDP_NRT_MAX_FDC_CNT);
    }

    // ***** Test Filters for suitable FilterDecisionCode 
    for ( ChIdx = 1; ChIdx < EDDP_NRT_MAX_FDC_CNT; ChIdx++ )
    {
        if (pNRTIface->FilterInfo[ChIdx].HWFrameFilter == HWFrameFilter)
        {
            
            if (pNRTIface->FilterInfo[ChIdx].pNrtHDB != LSA_NULL)
            {
                // ***** channel is active
                return (ChIdx);
            }
            else if (   (HWFrameFilter == EDDP_FDC_MRP) 
                     && (EDD_FEATURE_ENABLE  == pDDB->FeatureSupport.DLRExtensions)
                     && (pNRTIface->FilterInfo[EDDP_NRT_FILTER_DLR_IDX].pNrtHDB != LSA_NULL))
            {
                return (EDDP_NRT_FILTER_DLR_IDX);
            }
            // ***** channel is not opened
            return (EDDP_NRT_MAX_FDC_CNT);
        }
    }

    // ***** return bad index for filter not matched
    return (ChIdx);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRT_ISR_Acyc_Rcv_IFA                   +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32                     EventNr      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  EventNr    : Event Number (Interrupt Number)                     (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE after a receive ring    +*/
/*+               triggered its interrupt. It is executed within LOW        +*/
/*+               context and try to inform the user about a processed      +*/
/*+               RX frames if a swab buffer is avialable from a RQB        +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_NRT_ISR_Acyc_Rcv_IFA(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr)
{
#if 1
	EDDP_LL_NRT_RECV_DONE(pDDB);
#else
    LSA_UINT32                                      i, Status_Reg, RxRingIdx, Tmp, RxLen, Loops, UsrChIdx, AsicAddr, DMACWDropCnt;
    LSA_BOOL                                        isFramePass;
    EDDP_LOCAL_DDB_NRT_PTR_TYPE                     pNRTIface;
    EDDP_DPB_NRT_PTR_TYPE                           pNrtIFCfg;
    EDDP_LOCAL_HDB_NRT_PTR_TYPE                     pNrtHDB;
    EDDP_LOCAL_HDB_PTR_TYPE                         pHDB;
    EDDP_NRT_RX_RING_TYPE_PTR                       pRxHwDescrRingMgm;
    PNIP_LOCAL_DMACW_RCV_ITEM_PTR_TYPE              pRxRingItem;
    PNIP_DMACW_RCV_ITEM_TYPE                        RcvItem;
    EDD_UPPER_MEM_PTR_TYPE                          RxBufAddr;
    EDDP_RQB_QUEUE_PTR_TYPE                         pUsrChRqbQueue;
    EDD_UPPER_RQB_PTR_TYPE                          pRQB;
    EDD_UPPER_NRT_RECV_PTR_TYPE                     pRQBRcv;
    LSA_UINT32                                      TraceNoRqbs, TraceNoUsrCh;
    PNIP_LOCAL_DMACW_RCV_DESCR_PTR_TYPE             pRcvPtr;
    EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE      pNrtApiCtrlIfaceMgm;
#if (EDDP_CFG_TRACE_MODE != 0)
    EDD_UPPER_MEM_U8_PTR_TYPE                       pMacAdrTrace;
#endif

    // EDDP_ASSERT_NO_NULL_PTR(pDDB);
    // EDDP_ASSERT_NO_NULL_PTR(pDDB->pIF);

    LSA_UNUSED_ARG(EventNr);    // handle unused parameter

    // read STATUSCHANGE to get reason(s) of this interrupt for own used rings only
    Status_Reg = ( (EDDP_HAL_REG32_READ(pDDB, PNIP_REG_RCVSTATUS_IFA)) & EDDP_ARD_NRT_USED_RINGS );

    if (Status_Reg)
    {
        EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_NRT_ISR_Acyc_Rcv_IFA(EventNr=0x%X, RCVSTATUS_IFA=0x%X)", 
            EventNr, Status_Reg);

        // clear change indication bit(s)
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_RCVSTATUS_IFA, Status_Reg);

        pNRTIface = &pDDB->NRT;
        EDDP_ASSERT_NO_NULL_PTR(pNRTIface);

        pNrtIFCfg = &pDDB->NRT.NrtIFCfg;
        EDDP_ASSERT_NO_NULL_PTR(pNrtIFCfg);

        pNrtApiCtrlIfaceMgm = (EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE) pNRTIface->pNrtApiCtrlIfaceMgm;
        EDDP_ASSERT_NO_NULL_PTR(pNrtApiCtrlIfaceMgm);

        // step through signaled rings
        for ( RxRingIdx=0; RxRingIdx < EDDP_NRT_RX_DMACW_CNT; RxRingIdx++ )
        {
            // check if exist ring event
            if ((1<<RxRingIdx) & Status_Reg)
            {
                pRxHwDescrRingMgm   = &pNRTIface->RxHwDescrRingMgm[RxRingIdx];
                Loops               = pRxHwDescrRingMgm->ItemCount;
                TraceNoRqbs         = TraceNoUsrCh = 0;

                while (Loops--)
                {
                    // RcvInf1, NextDMACWPtr, TimeStamp, FrameBufferUpperPtr, RcvInf2
                    pRxRingItem     = &pRxHwDescrRingMgm->pRxRing[pRxHwDescrRingMgm->DoneIdx];

                    RcvItem.RcvInf1 = EDDP_HAL_MEM32_READ(pDDB, (EDD_COMMON_MEM_U32_PTR_TYPE)&pRxRingItem->RcvInf1);

                    if (PNIP_DMACW_RCV_ITEM_GET_OWNER(RcvItem) == (LSA_UINT32) PNIP_DMACW_RCV_ITEM_INF1_OWN_SW)
                    {
                        // item processed by ASIC
                        RcvItem.NextDMACWPtr    = EDDP_HAL_MEM32_READ(pDDB, (EDD_COMMON_MEM_U32_PTR_TYPE)&pRxRingItem->NextDMACWPtr);
                        RcvItem.TimeStamp       = EDDP_HAL_MEM32_READ(pDDB, (EDD_COMMON_MEM_U32_PTR_TYPE)&pRxRingItem->TimeStamp);
                        RcvItem.FrameBufferPtr  = EDDP_HAL_MEM32_READ(pDDB, (EDD_COMMON_MEM_U32_PTR_TYPE)&pRxRingItem->FrameBufferPtr);
                        RcvItem.RcvInf2         = EDDP_HAL_MEM32_READ(pDDB, (EDD_COMMON_MEM_U32_PTR_TYPE)&pRxRingItem->RcvInf2);
                        RxLen                   = PNIP_DMACW_RCV_ITEM_GET_RXFRMLEN(RcvItem);
                        UsrChIdx                = _EDDP_NRT_SearchUserChannelIdx (pDDB, pNRTIface, (PNIP_DMACW_RCV_ITEM_GET_FDC(RcvItem)));
                        RxBufAddr               = pRxHwDescrRingMgm->pFrameBufferArray->FrameBufferUpperAddr[pRxHwDescrRingMgm->DoneIdx];
                        EDDP_RX_MEM_ADDR_UPPER_TO_PNIP(pDDB->hSysDev, RxBufAddr, &AsicAddr);
                        EDDP_ASSERT_FALSE(AsicAddr != RcvItem.FrameBufferPtr);
                        pRcvPtr                 = &pNrtApiCtrlIfaceMgm->ApiCtrlRamRcvDmacw[RxRingIdx];
                        DMACWDropCnt            = (EDDP_HAL_MEM32_READ(pDDB, &pRcvPtr->RcvDropCntTsErrIntVal) >> PNIP_DMACW_RCV_DESCR_DROPCNT_SHIFT);

                        EDDP_NRT_TRACE_07(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] ###### EDDP_NRT_ISR_Acyc_Rcv_IFA(Rx-Ring=%d): DmacwPtr=0x%8.8X, FDC=0x%4.4X, RxLen=%d, UsrChIdx=%d, Port=%d, DMACWDropCnt=%d", 
                            RxRingIdx, pRxRingItem, PNIP_DMACW_RCV_ITEM_GET_FDC(RcvItem), RxLen, UsrChIdx, (PNIP_DMACW_RCV_ITEM_GET_SOURCE(RcvItem)>>PNIP_DMACW_RCV_ITEM_INF1_SOURCE_SHIFT), DMACWDropCnt);

                        #if (EDDP_CFG_TRACE_MODE != 0)
                        /* -------------------------------------------- */
                        /* Trace DstMAC and SrcMAC of received frame    */
                        /* -------------------------------------------- */
                        // set pointer to DstMAC (first 6 bytes in frame)
                        pMacAdrTrace = (EDD_UPPER_MEM_U8_PTR_TYPE) RxBufAddr;
                        EDDP_NRT_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] ###### EDDP_NRT_ISR_Acyc_Rcv_IFA(DstMAC=%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X)", 
                            pMacAdrTrace[0], pMacAdrTrace[1], pMacAdrTrace[2], pMacAdrTrace[3], pMacAdrTrace[4], pMacAdrTrace[5]);
                        // set pointer to SrcMAC (next 6 bytes in frame)
                        pMacAdrTrace += 6;
                        EDDP_NRT_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] ###### EDDP_NRT_ISR_Acyc_Rcv_IFA(SrcMAC=%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X)", 
                            pMacAdrTrace[0], pMacAdrTrace[1], pMacAdrTrace[2], pMacAdrTrace[3], pMacAdrTrace[4], pMacAdrTrace[5]);
                        #endif

                        #if (EDDP_NRT_CHECK_FDC_FIELD == 1)
                        if ( PNIP_DMACW_RCV_ITEM_GET_FDC(RcvItem) == EDDP_FDC_INVALID )
                        {
                            // HW had not updated this field with received frame
                            EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_NRT_ISR_Acyc_Rcv_IFA(): FATAL -> bad FDC in Rx-Ring(%d)!", 
                                RxRingIdx);
                            EDDP_FATAL(EDDP_FATAL_ERR_UNEXP_INTERRUPT);
                        }
                        #endif

                        #if EDDP_NRT_RXTX_COUNTER_SUPPORT == 1
                        // simple SW counter
                        //#if PNIP_DMACW_RCV_ITEM_INF1_CFG_SYN_YES == 0
                        //#error "expect PNIP_DMACW_RCV_ITEM_INF1_CFG_SYN_YES as non zero defined"
                        //#endif

                        Tmp = (LSA_UINT32) RxBufAddr;

                        if (PNIP_DMACW_RCV_ITEM_GET_SYN(RcvItem))   // == PNIP_DMACW_RCV_ITEM_INF1_CFG_SYN_YES
                        {
                            // Sync-Frame (8 byte timestamp included)
                            Tmp += 8;
                        }

                        if ( ((LSA_UINT8*)Tmp)[0] & 1 )
                        {
                            // counts BC/MC rx-packets
                            pNRTIface->Stats.InNUcastPkts ++;
                            if (((LSA_UINT8*)Tmp)[0] == 0xFF)
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
                        #endif

                        // is UserChannel present for this FrameFilter ?
                        if (UsrChIdx < EDDP_NRT_MAX_FDC_CNT)
                        {
                            // check if received frame can be signalled to user channel
                            pNrtHDB         = pNRTIface->FilterInfo[UsrChIdx].pNrtHDB;
                            pUsrChRqbQueue  = &pNrtHDB->RxUsrQueue;
                            isFramePass     = LSA_TRUE; // frame PASS or DROP

                            /* -------------------------------------------------------- */
                            /* UDP Whitelist filtering                                  */
                            /* check the filtering of UDP Broadcast/Unicast frames      */
                            /* -------------------------------------------------------- */
                            switch (pNRTIface->FilterInfo[UsrChIdx].UsrFrameFilter)
                            {
                                case EDD_NRT_FRAME_IP_UDP:          /* IP-Header.Protocol = 17 (UDP)                                                    */
                                case EDD_NRT_FRAME_UDP_ASRT:        /* IP-Header.Protocol = 17 (UDP) and RToverUDP with UDP-Header.DstPort = 0x8892     */
                                case EDD_NRT_FRAME_UDP_DCP:         /* IP-Header.Protocol = 17 (UDP) and RToverUDP with UDP-Header.DstPort = 0x8892     */
                                case EDD_NRT_FRAME_UDP_DCP_HELLO:   /* IP-Header.Protocol = 17 (UDP) and RToverUDP with UDP-Header.DstPort = 0x8892     */
                                {
                                    LSA_UINT8                   IHL;                                    
                                    LSA_UINT16                  FragmentOffset;
                                    LSA_UINT16                  UDPDstPort;
                                    EDD_MAC_ADR_TYPE            DstMacAdr = {{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }}; // set to 0 to avoid a warning
                                    EDD_UPPER_MEM_U8_PTR_TYPE   pFrame;

                                    // set pointer to DstMAC of the received frame
                                    pFrame = (EDD_UPPER_MEM_U8_PTR_TYPE) RxBufAddr;
                                    // copy the bytes of DstMAC
                                    for ( i=0; i < EDD_MAC_ADDR_SIZE; i++ )
                                    {
                                        DstMacAdr.MacAdr[i] = pFrame[i];
                                    }

                                    // get the UDPDstPort from frame (no VLAN-Tag in frame)
                                    // UDPDstPort = DstMAC(6) + SrcMAC(6) + EtherType(2) + IP-Header[20 = IHL*4] + UDP-Header[SrcPort(2)+DstPort]
                                    IHL         = pFrame[14];                       // get Version/IHL from IP-Header
                                    IHL         = EDDP_GET_BIT_VALUE(IHL, 0x0F, 0); // get IHL
                                    // get Fragment Offset from IP header
                                    FragmentOffset = pFrame[20];                    // get fragment offset
                                    EDDP_SHIFT_VALUE_LEFT(FragmentOffset, 8);                                    
                                    FragmentOffset += pFrame[21]; 
                                    FragmentOffset &= 0x1FFF;                       // separate from flags

                                    if (0 == FragmentOffset)
                                    {
                                        pFrame      = &pFrame[(14 + (IHL * 4) + 2)];    // set pointer to UDPDstPort
                                        UDPDstPort  = EDDP_GET_U16(pFrame);             // and get UDPDstPort

                                        /* -------------------------------- */
                                        /* is BC and BC filter enable ?     */
                                        /* -------------------------------- */
                                        if ((EDDP_IS_BC_MAC(DstMacAdr)) && (EDD_SYS_UDP_WHITELIST_FILTER_ON == pNrtIFCfg->RxFilterUDP_Broadcast))
                                        {
                                            if ((pDDB->FeatureSupport.EIPExtensions == EDD_FEATURE_ENABLE)
                                                && (UDPDstPort == EDDP_NRT_UDP_DST_PORT_EIP)
                                                   )
                                            {
                                                // frame = PASS
                                            }
                                            else
                                            if (   (UDPDstPort != EDDP_NRT_UDP_DST_PORT_NTP_SNTP)
                                                && (UDPDstPort != EDDP_NRT_UDP_DST_PORT_DHCP_CLIENT)
                                               )
                                            {
                                                // frame = DROP
                                                isFramePass = LSA_FALSE;
                                                EDDP_NRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] ###### EDDP_NRT_ISR_Acyc_Rcv_IFA(Rx-Ring=%d, UsrChIdx=%d): This frame is dropped by EDDP - RxFilterUDP_Broadcast is enable for UDPDstPort=0x%X", 
                                                    RxRingIdx, UsrChIdx, UDPDstPort);
                                            }
                                        }

                                        /* -------------------------------- */
                                        /* is UC and UC filter enable ?     */
                                        /* -------------------------------- */
                                        if (   (isFramePass)
                                            && ((!(EDDP_IS_MC_MAC(DstMacAdr))) && (EDD_SYS_UDP_WHITELIST_FILTER_ON == pNrtIFCfg->RxFilterUDP_Unicast))
                                           )
                                        {
                                            if (   ((UDPDstPort >= EDDP_NRT_UDP_DST_PORT_SNMP_MIN) && (UDPDstPort <= EDDP_NRT_UDP_DST_PORT_SNMP_MAX))
                                                || (UDPDstPort == EDDP_NRT_UDP_DST_PORT_IANA_EPM)
#if (EDDP_NRT_UDP_DST_PORT_FREE_MAX==0xFFFF)
                                                || (UDPDstPort >= EDDP_NRT_UDP_DST_PORT_FREE_MIN)
#else
                                                || ((UDPDstPort >= EDDP_NRT_UDP_DST_PORT_FREE_MIN) && (UDPDstPort <= EDDP_NRT_UDP_DST_PORT_FREE_MAX))
#endif
                                                || (UDPDstPort == EDDP_NRT_UDP_DST_PORT_DHCP_CLIENT)
                                                || (UDPDstPort == EDDP_NRT_UDP_DST_PORT_NTP_SNTP)
                                               )
                                            {
                                                // frame = PASS
                                            }
                                            else if ((pDDB->FeatureSupport.EIPExtensions == EDD_FEATURE_ENABLE)
                                                    && ((UDPDstPort == EDDP_NRT_UDP_DST_PORT_EIP_DLR) 
                                                    ||  (UDPDstPort == EDDP_NRT_UDP_DST_PORT_EIP))
                                                    )
                                            {
                                                    // frame = PASS
                                            }

                                            else
                                            {
                                                // frame = DROP
                                                isFramePass = LSA_FALSE;
                                                EDDP_NRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] ###### EDDP_NRT_ISR_Acyc_Rcv_IFA(Rx-Ring=%d, UsrChIdx=%d): This frame is dropped by EDDP - RxFilterUDP_Unicast is enable for UDPDstPort=0x%X", 
                                                    RxRingIdx, UsrChIdx, UDPDstPort);
                                            }
                                        }
                                    }
                                }
                                break;

                                default:
                                    // no UDP frame
                                break;
                            }

                            /* -------------------------------------------------------- */
                            /* shall this frame be signalled to the user ?              */
                            /* -------------------------------------------------------- */
                            if (isFramePass)
                            {
                                if (pUsrChRqbQueue->Count)
                                {
                                    // RQB exist for callback
                                    pRQB = EDDP_GlbRQBQueueRemoveFromBegin (pUsrChRqbQueue);

                                    EDDP_GlbHandleGetHDB (pNrtHDB->EDD_Handle, &pHDB);
                                    EDDP_ASSERT_NO_NULL_PTR(pHDB);

                                    if (!LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL))
                                    {
                                        // swap RX-buffer and execute the callback
                                        pRQBRcv = (EDD_UPPER_NRT_RECV_PTR_TYPE) pRQB->pParam;

                                        if (!LSA_HOST_PTR_ARE_EQUAL(pRQBRcv, LSA_NULL))
                                        {
                                            EDDP_RX_MEM_ADDR_UPPER_TO_PNIP(pDDB->hSysDev, pRQBRcv->pBuffer, &PNIP_DMACW_RCV_ITEM_GET_RXBUF(RcvItem));        // write given Rx-buffer-pointer (from RQB) into RcvItem.FrameBufferPtr
                                            pRxHwDescrRingMgm->pFrameBufferArray->FrameBufferUpperAddr[pRxHwDescrRingMgm->DoneIdx] = pRQBRcv->pBuffer;
                                            EDDP_NRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] ###### EDDP_NRT_ISR_Acyc_Rcv_IFA(): Buffer(0x%8.8X) to Usr, Buffer(0x%8.8X) to ASIC - RxUsrQueue has %d item left", 
                                                RxBufAddr, pRQBRcv->pBuffer, pUsrChRqbQueue->Count);
                                            pRQBRcv->pBuffer = (EDD_UPPER_MEM_U8_PTR_TYPE) RxBufAddr;                                                                       // and write filled HW-buffer-pointer into RQB (pRQBRcv)
                                            EDDP_HAL_MEM32_WRITE(pDDB, (EDD_COMMON_MEM_U32_PTR_TYPE)&pRxRingItem->FrameBufferPtr, PNIP_DMACW_RCV_ITEM_GET_RXBUF(RcvItem));  // update new RX-buffer address in the DMACW (ASIC RX-entry)
#if(EDD_NRT_MIN_SND_LEN!=14)
    #error "need define with: (DST mac) 6 Byte + (SRC mac) 6 Byte + (LNG/Type) 2 Byte"
#endif
                                            //pRQBRcv->RequestID
                                            pRQBRcv->IOCount        = PNIP_DMACW_RCV_ITEM_GET_RXFRMLEN(RcvItem);
                                            pRQBRcv->UserDataLength = pRQBRcv->IOCount - EDD_NRT_MIN_SND_LEN;   // (DST mac) 6 Byte + (SRC mac) 6 Byte + (LNG/Type) 2 Byte
                                            pRQBRcv->UserDataOffset = EDD_NRT_MIN_SND_LEN;

                                            // map PortID from HW-view into EDD-view
                                            switch (PNIP_DMACW_RCV_ITEM_GET_SOURCE(RcvItem))
                                            {
                                                case PNIP_DMACW_RCV_ITEM_INF1_SOURCE_P1:    pRQBRcv->PortID = 1;    break;
                                                case PNIP_DMACW_RCV_ITEM_INF1_SOURCE_P2:    pRQBRcv->PortID = 2;    break;
                                                case PNIP_DMACW_RCV_ITEM_INF1_SOURCE_P3:    pRQBRcv->PortID = 3;    break;
                                                case PNIP_DMACW_RCV_ITEM_INF1_SOURCE_P4:    pRQBRcv->PortID = 4;    break;
                                                default:                                    pRQBRcv->PortID = 0;    break;
                                            }

                                            pRQBRcv->PortID         = (LSA_UINT16) EDDP_SWI_get_PortID_and_IF(pDDB, pRQBRcv->PortID, LSA_NULL);
                                            pRQBRcv->FrameFilter    = pNRTIface->FilterInfo[UsrChIdx].UsrFrameFilter;

										    /* Fill FrameFilterInfo with UserID to show the user the Number of the Filter used
											    Note: If more then 3 Filters are available, the Index (0,1,...31) in the pDCPHelloFilter table 
											    can be found in the coding of the EDDP_FDC_DCP_HELLOx define (coded in Bits 10...14, see HWFilter docu). 
										    */
                                            {
                                                LSA_UINT32 idx;
                                                
                                                pRQBRcv->FrameFilterInfo    = 0x0; 
                                                for(idx = 0; idx < pDDB->NRT.DcpHelloFilterCnt; idx++)
                                                {
                                                    if(pNRTIface->FilterInfo[UsrChIdx].HWFrameFilter == (EDDP_FDC_DCP_HELLO | (idx << 10)))
                                                    {
                                                        pRQBRcv->FrameFilterInfo    = pHDB->pDDB->NRT.pDCPHelloFilter[idx].UserID;
                                                        break;
                                                    }
                                                }
                                            }
                                            
//										    if(pNRTIface->FilterInfo[UsrChIdx].HWFrameFilter == EDDP_FDC_DCP_HELLO) 
//										    {
//											    pRQBRcv->FrameFilterInfo	= pHDB->pDDB->NRT.pDCPHelloFilter[0].UserID;
//										    }
//										    else if(pNRTIface->FilterInfo[UsrChIdx].HWFrameFilter == EDDP_FDC_DCP_HELLO2) 
//										    {
//											    pRQBRcv->FrameFilterInfo	= pHDB->pDDB->NRT.pDCPHelloFilter[1].UserID;
//										    }
//										    else if(pNRTIface->FilterInfo[UsrChIdx].HWFrameFilter == EDDP_FDC_DCP_HELLO3)
//										    {
//											    pRQBRcv->FrameFilterInfo	= pHDB->pDDB->NRT.pDCPHelloFilter[2].UserID;
//										    }
//										    else 
//										    {
//											    pRQBRcv->FrameFilterInfo	= 0x0; 
//										    }
										    if(RxRingIdx==0) // droped frames on RX-Ring should get undefined FrameFilterInfo (eg. 0). If not set to zero here, FrameFilterInfo would be the value 'UserID' from Hello-Filter1
										    {
											    pRQBRcv->FrameFilterInfo	= 0;
										    }

										    pRQBRcv->RequestCnt = pUsrChRqbQueue->Count;
                                            pRQBRcv->RxTime     = PNIP_DMACW_RCV_ITEM_GET_TS(RcvItem);

                                            EDDP_NRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] ###### EDDP_NRT_ISR_Acyc_Rcv_IFA(Rx-Ring=%d, UsrChIdx=%d): The received frame goes to user channel - RxRQB(RequestCnt=%d, RxTime=0x%X)", 
                                                RxRingIdx, UsrChIdx, pRQBRcv->RequestCnt, pRQBRcv->RxTime);

                                            //pRQBRcv->doSwFwd[] --> this Out-Parameter will be filled with LSA_FALSE in EDDP_NRTRecv()
                                            if (PNIP_DMACW_RCV_ITEM_GET_RESULT(RcvItem) <= PNIP_DMACW_RCV_ITEM_INF1_RESULT_OK_WARN)
                                            {
                                                // PNIP_DMACW_RCV_ITEM_INF1_RESULT_OK || PNIP_DMACW_RCV_ITEM_INF1_RESULT_OK_WARN
                                                EDDP_GlbRequestFinish (pHDB, pRQB, EDD_STS_OK);
                                            }
                                            else
                                            {   // PNIP_DMACW_RCV_ITEM_INF1_RESULT_ERR_BUFSHORT || PNIP_DMACW_RCV_ITEM_INF1_RESULT_ERR_BUFALIGN
                                                EDDP_GlbRequestFinish (pHDB, pRQB, EDD_STS_ERR_RX);
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    // no RQB for higher layer present
                                    if ( !(TraceNoRqbs & (1<<UsrChIdx)) )
                                    {   // avoid redundant trace
                                        TraceNoRqbs |= (1<<UsrChIdx);
                                        EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] ###### EDDP_NRT_ISR_Acyc_Rcv_IFA(Rx-Ring=%d): RxUsrQueue[UsrChIdx=%d] empty !!!!", 
                                            RxRingIdx, UsrChIdx);
                                    }
                                }
                            }
                        }
                        else
                        {
                            // give the rcv-item back to the ASIC
                            if ( !(TraceNoUsrCh & (1<<RxRingIdx)) )
                            {   // avoid redundant trace
                                TraceNoUsrCh |= (1<<RxRingIdx);
                                EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] ###### EDDP_NRT_ISR_Acyc_Rcv_IFA(): No UserChannel for Rx-Ring=%d", 
                                    RxRingIdx);
                            }
                        }

                        // put rcv-item back for receiving into ASIC
                        #if (EDDP_NRT_CHECK_FDC_FIELD == 1)
                        // set FDC to a defined value --> HW must update this field with received frame !!!!
                        PNIP_DMACW_RCV_ITEM_SET_FDC(RcvItem, EDDP_FDC_INVALID);
                        EDDP_HAL_MEM32_WRITE(pDDB, (EDD_COMMON_MEM_U32_PTR_TYPE)&pRxRingItem->RcvInf2, PNIP_DMACW_RCV_ITEM_GET_INF2(RcvItem));
                        #endif
                        PNIP_DMACW_RCV_ITEM_SET_OWNER_HW(RcvItem);
                        EDDP_HAL_MEM32_WRITE(pDDB, (EDD_COMMON_MEM_U32_PTR_TYPE)&pRxRingItem->RcvInf1, PNIP_DMACW_RCV_ITEM_GET_INF1(RcvItem));

                        if ((++pRxHwDescrRingMgm->DoneIdx) >= pRxHwDescrRingMgm->ItemCount)
                        {
                            pRxHwDescrRingMgm->DoneIdx = 0;
                        }
                    }
                    else
                    {
                        // item not processed by ASIC
                        break;
                    }
                }
            }
        }

        EDDP_NRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_NRT_ISR_Acyc_Rcv_IFA()");
    }
#endif
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRT_ISR_Acyc_Snd_ERR                   +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32                     EventNr      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  EventNr    : Event Number (Interrupt Number)                     (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE for specific send       +*/
/*+               errors. It is executed within LOW context.                +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_NRT_ISR_Acyc_Snd_ERR(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr)
{
    LSA_UINT32            SndErrStatus, SndErrEvent, SndEvent, Err;
#if (EDDP_CFG_TRACE_MODE ==  0)
    LSA_UNUSED_ARG(EventNr);
#endif

    SndErrStatus    = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_SNDERRSTATUS_IFA); 
    SndErrEvent     = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_SNDERREVENT_IFA); 
    SndEvent        = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_SNDEVENT_IFA); 
    EDDP_NRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_NRT_ISR_Acyc_Snd_ERR(EventNr=0x%X): ERROR -> RegSndErrStatusIFA=0x%X, RegSndErrEventIFA=0x%X, RegSndEventIFA=0x%X", 
        EventNr, SndErrStatus, SndErrEvent, SndEvent);

    /* There is a macro that checks PNIP_R2_REG_ACCESS_ERROR and PNIP_R3_REG_ACCESS_ERROR has the same value. */
    /* Checked in eddp_hal_pnip_val.h. We use the R2 version. */
    Err = EDDP_HAL_REG32_READ(pDDB, PNIP_R2_REG_ACCESS_ERROR); 
    EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_NRT_ISR_Acyc_Snd_ERR(EventNr=0x%X): FATAL -> ACCESS_ERROR=0x%X", 
        EventNr, Err);
    EDDP_FATAL(EDDP_FATAL_ERR_UNEXP_INTERRUPT);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRT_ISR_Acyc_Rcv_ERR                   +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32                     EventNr      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  EventNr    : Event Number (Interrupt Number)                     (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE for specific receive    +*/
/*+               errors. It is executed within LOW context.                +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_NRT_ISR_Acyc_Rcv_ERR(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr)
{
    LSA_UINT32            RcvErrStatus, RcvErrEvent, RcvEvent, Err;
#if (EDDP_CFG_TRACE_MODE ==  0)
    LSA_UNUSED_ARG(EventNr);
#endif

    RcvErrStatus    = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_RCVERRSTATUS_IFA); 
    RcvErrEvent     = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_RCVERREVENT_IFA); 
    RcvEvent        = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_RCVEVENT_IFA); 
    EDDP_NRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_NRT_ISR_Acyc_Rcv_ERR(EventNr=0x%X): ERROR -> RegRcvErrStatusIFA=0x%X, RegRcvErrEventIFA=0x%X, RegRcvEventIFA=0x%X", 
        EventNr, RcvErrStatus, RcvErrEvent, RcvEvent);

    /* There is a macro that checks PNIP_R2_REG_ACCESS_ERROR and PNIP_R3_REG_ACCESS_ERROR has the same value. */
    /* Checked in eddp_hal_pnip_val.h. We use the R2 version. */
    Err = EDDP_HAL_REG32_READ(pDDB, PNIP_R2_REG_ACCESS_ERROR); 
    EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_NRT_ISR_Acyc_Rcv_ERR(EventNr=0x%X): FATAL -> ACCESS_ERROR=0x%X", 
        EventNr, Err);
    EDDP_FATAL(EDDP_FATAL_ERR_UNEXP_INTERRUPT);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRT_ISR_APIFilter_ERR                  +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             LSA_UINT32                     EventNr      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  EventNr    : Event Number (Interrupt Number)                     (in)  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE for specific filtering  +*/
/*+               errors. It is executed within LOW context.                +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_NRT_ISR_APIFilter_ERR(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  EventNr)
{
    LSA_UINT32      ErrEvent, Err;

#if (EDDP_CFG_TRACE_MODE ==  0)
    LSA_UNUSED_ARG(EventNr);
#endif

    ErrEvent = EDDP_HAL_REG32_READ(pDDB, PNIP_REG_FILTERERRSTATUS_IFA); 
    EDDP_NRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_NRT_ISR_APIFilter_ERR(EventNr=0x%X): ERROR -> RegFilterErrStatusIFA=0x%X RegFilterErrEventIFA=0x%X", 
        EventNr, ErrEvent, (LSA_UINT32)EDDP_HAL_REG32_READ(pDDB, PNIP_REG_FILTERERREVENT_IFA));

    /* There is a macro that checks PNIP_R2_REG_ACCESS_ERROR and PNIP_R3_REG_ACCESS_ERROR has the same value. */
    /* Checked in eddp_hal_pnip_val.h. We use the R2 version. */
    Err = EDDP_HAL_REG32_READ(pDDB, PNIP_R2_REG_ACCESS_ERROR); 
    EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_NRT_ISR_APIFilter_ERR(EventNr=0x%X): FATAL -> ACCESS_ERROR=0x%X", 
        EventNr, Err);
    EDDP_FATAL(EDDP_FATAL_ERR_UNEXP_INTERRUPT);
}

/*===========================================================================*/
/*                        public  functions                                  */
/*===========================================================================*/
