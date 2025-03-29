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
/*  F i l e               &F: eddp_nrt_snd.c                            :F&  */
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
#endif
/*****************************************************************************/




/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/

#define LTRC_ACT_MODUL_ID  65
#define EDDP_MODULE_ID     LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_NRT */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"               /* eddp headerfiles */
#include "eddp_int.h"               /* internal header  */
#include "eddp_hwc.h"               /* hardwarefilter code  */
#include "eddp_nrt_inc.h"


EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)

//#define EDDP_NRT_TX_DUMP_SUPPORTED


/*===========================================================================*/
/*                        private functions                                  */
/*===========================================================================*/

#ifdef EDDP_NRT_TX_DUMP_SUPPORTED
LSA_VOID EDDP_LOCAL_FCT_ATTR _EDDP_NRTDumpFrame(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_UPPER_MEM_U8_PTR_TYPE   pBuffer,
    LSA_UINT16                  Length
    )
{
    EDD_UPPER_MEM_U8_PTR_TYPE   pBuf = pBuffer;
    LSA_UINT16                  Len;

    LSA_UNUSED_ARG(pDDB);
    while(Length)
    {
        if ( Length > 8 )
        {
            Len = 8;
        }
        else
        {
            Len = Length;
        }
		EDDP_NRT_TRACE_BYTE_ARRAY(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "_NRT: ", pBuf, Len);
        pBuf   += Len;
        Length -= Len;
    }
}
#endif


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_NRTInitSingleSndRing                  +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_UINT16                  RingIdx         +*/
/*+                             LSA_UINT16                  Count           +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  RingIdx    : index of TX-Ring (0..15)                                  +*/
/*+  Count      : nr of items in TX-Ring                                    +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_ALIGN                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: utility function for _EDDP_NRTInitSndRings()              +*/
/*+               --> init selected TX-Ring: alloc item memory and connect  +*/
/*+                   items into a ring                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_NRTInitSingleSndRing(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  RingIdx,
    LSA_UINT16                  Count
    )
{
    EDDP_LOCAL_DDB_NRT_PTR_TYPE         pNRTIface;
    LSA_UINT32                          Idx, AsicAddr, AllocSize;
    EDDP_NRT_TX_RING_TYPE_PTR           pNrtTxRingDescr;
    PNIP_LOCAL_DMACW_SND_ITEM_PTR_TYPE  pNrtTxRingItems, pNrtTxPrevItem, pNrtTxCurrItem;
    EDD_RSP                             Response = EDD_STS_OK;

    // EDDP_ASSERT_NO_NULL_PTR(pDDB);

    if (Count)
    {
        pNRTIface = &pDDB->NRT;

        if (RingIdx < EDDP_NRT_TX_DMACW_CNT)
        {
            // **** alloc memory for ring items
            pNrtTxRingDescr             = &pNRTIface->TxHwDescrRingMgm[RingIdx];            // pointer to mgmt
            AllocSize                   = (sizeof(PNIP_DMACW_SND_ITEM_TYPE)*(LSA_UINT32)(Count))+EDDP_HAL_32BIT_ALIGNMENT;
            pNrtTxRingDescr->pAllocAddr = EDDP_GlbAllocUpperDevMem(pDDB, AllocSize);  // store real (unaligned) memory address


            if (LSA_HOST_PTR_ARE_EQUAL(pNrtTxRingDescr->pAllocAddr, LSA_NULL))
            {
                Response = EDD_STS_ERR_RESOURCE;
            }
            else
            {
                // ***** memory for ring items exist --> connect all pointers (HW view) into a ring
                // ***** trim to 32bit alignment in Asic view and adapt address in host view to asic view
                pNrtTxRingItems = (PNIP_LOCAL_DMACW_SND_ITEM_PTR_TYPE) EDDP_NRTUpperAdaptAlignment(pDDB, pNrtTxRingDescr->pAllocAddr);

                // ***** write mgmt for the DMACW ring
                pNrtTxRingDescr->pTxRing    = pNrtTxRingItems;              // head of dmacw ring
                pNrtTxRingDescr->ItemCount  = Count;                        // nr of items in ring
                pNrtTxRingDescr->FreeCount  = Count;                        // at beginning all items free for TX
                pNrtTxRingDescr->InsertIdx  = 0;                            // set position for insert transmit request
                pNrtTxRingDescr->DoneIdx    = 0;                            // set position for processed requests
                pNrtTxPrevItem              = &pNrtTxRingItems[Count-1];    // build linkage queue from previous to next item !!!

                // ***** write HW entries into the DMACW items of the DMACW ring
                for ( Idx=0; Idx < Count; Idx++ )
                {
                    // get DMACW item
                    pNrtTxCurrItem = &pNrtTxRingItems[Idx];

                    EDDP_HAL_MEM32_WRITE(pDDB, &pNrtTxCurrItem->SndCfg1, PNIP_DMACW_SND_ITEM_CFG1_DEF_SW);        // default cfg: owner is SW
                    EDDP_DEV_MEM_ADDR_UPPER_TO_PNIP(pDDB->hSysDev, pNrtTxCurrItem, &AsicAddr);                           // map address into Asic view
                    EDDP_HAL_MEM32_WRITE(pDDB, (EDD_COMMON_MEM_U32_PTR_TYPE)&pNrtTxPrevItem->NextDMACWPtr,     AsicAddr);    // link on prev. item !!!
                    EDDP_HAL_MEM32_WRITE(pDDB, &pNrtTxCurrItem->TimeStamp, 0);                                    // zero
                    EDDP_HAL_MEM32_WRITE(pDDB, (EDD_COMMON_MEM_U32_PTR_TYPE)&pNrtTxCurrItem->FrameBufferPtr,   EDDP_PNIP_NULL_PTR);
                    EDDP_HAL_MEM32_WRITE(pDDB, &pNrtTxCurrItem->SndCfg2,   PNIP_DMACW_SND_ITEM_CFG2_DEF);         // default cfg

                    // ***** special SW entries to find mgmt data for the processed TX-frame in ISR
                    pNrtTxCurrItem->pTxRqb  = LSA_NULL;         // not used
                    pNrtTxCurrItem->pNrtHDB = LSA_NULL;         // not used
                    pNrtTxPrevItem          = pNrtTxCurrItem;   // store pointer for linkage
                }
            }
        }
        else
        {
            Response = EDD_STS_ERR_PARAM;
        }
    }

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_NRTDeInitSingleSndRing                +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_UINT16                  RingIdx         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  RingIdx    : index of TX-Ring (0..15)                                  +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_ALIGN                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: utility function for _EDDP_NRTInitSndRings()              +*/
/*+               --> deinit selected TX-Ring: release item memory          +*/
/*+         Note: check internal members "pAllocAddr" to see that this      +*/
/*+               memeory must be released                                  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_NRTDeInitSingleSndRing(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  RingIdx
    )
{
    EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE  pNrtApiCtrlIfaceMgm;
    EDDP_LOCAL_DDB_NRT_PTR_TYPE                 pNRTIface;
    PNIP_LOCAL_DMACW_SND_DESCR_PTR_TYPE         pSndDescr;
    EDDP_NRT_TX_RING_TYPE_PTR                   pTxRing;
    EDD_RSP                                     Response = EDD_STS_OK;

    // EDDP_ASSERT_NO_NULL_PTR(pDDB);

    pNrtApiCtrlIfaceMgm = (EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE) pDDB->NRT.pNrtApiCtrlIfaceMgm;
    pNRTIface           = &pDDB->NRT;
    pSndDescr           = &pNrtApiCtrlIfaceMgm->ApiCtrlRamSndDmacw[RingIdx];
    pTxRing             = &pNRTIface->TxHwDescrRingMgm[RingIdx];

    // ***** release TX-Mgm-memeory if pAllocAddr set
    if (pTxRing->pAllocAddr)
    {
        if ( EDDP_GlbFreeUpperDevMem(pDDB, pTxRing->pAllocAddr) == LSA_RET_OK )
        {
            // ***** set TX-Descr "unused"
            EDDP_HAL_MEM32_WRITE(pDDB, &pSndDescr->SndWdtErrIntVal, PNIP_DMACW_SND_DESCR_CFG_OFF);    // Watchdog=off, no IRQs
            EDDP_HAL_MEM32_WRITE(pDDB, &pSndDescr->CurrentDmacwHwPtr, EDDP_PNIP_NULL_PTR);            // empty ring

            pTxRing->pAllocAddr = LSA_NULL;
            pTxRing->pTxRing    = LSA_NULL;
            pTxRing->ItemCount  = 0;
            pTxRing->FreeCount  = 0;
            pTxRing->InsertIdx  = 0;
            pTxRing->DoneIdx    = 0;
        }
        else
        {
            Response = EDD_STS_ERR_RESOURCE;
        }
    }

    return (Response);
}



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_NRTInitSndRingsEmpty                  +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_UINT16                  RingMask        +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  RingMask   : selector for TX-Ring (bits 0..15)                         +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_ALIGN                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: utility function for EDDP_NRTDeviceOpen()                 +*/
/*+               --> set empty HW SndRings: all Pointers in                +*/
/*+               SndDescrTable[16] to EDDP_PNIP_NULL_PTR                   +*/
/*+         Note: set internal members "pAllocAddr" to NULL to see later    +*/
/*+               which memeory must be released                            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_NRTInitSndRingsEmpty(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  RingMask)
{   
    EDDP_LOCAL_DDB_NRT_PTR_TYPE                 pNRTIface;
    LSA_UINT32                                  Idx;
    EDDP_NRT_TX_RING_TYPE_PTR                        pNrtTxRingDescr;
    EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE  pNrtApiCtrlIfaceMgm;
    PNIP_LOCAL_DMACW_SND_DESCR_PTR_TYPE         pSndDescr;

    // EDDP_ASSERT_NO_NULL_PTR(pDDB);

    pNrtApiCtrlIfaceMgm = (EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE) pDDB->NRT.pNrtApiCtrlIfaceMgm;
    pNRTIface           = &pDDB->NRT;

    // ***** step through all valid TX-rings and init rings empty
    for ( Idx=0; Idx < EDDP_NRT_TX_DMACW_CNT; Idx++ )
    {   
        if ( (1<<Idx) & RingMask )
        {
            //***** set TX-Descr "unused",  (special) nullptr in ASIC !!!
            pSndDescr = &pNrtApiCtrlIfaceMgm->ApiCtrlRamSndDmacw[Idx];

            EDDP_HAL_MEM32_WRITE(pDDB, &pSndDescr->SndWdtErrIntVal, PNIP_DMACW_SND_DESCR_CFG_OFF);    // Watchdog=off, no IRQs
            EDDP_HAL_MEM32_WRITE(pDDB, &pSndDescr->CurrentDmacwHwPtr, EDDP_PNIP_NULL_PTR);            // empty ring

            // ***** set SW mgm info for this TX-ring to "unused"
            pNrtTxRingDescr                 = &pNRTIface->TxHwDescrRingMgm[Idx];
            pNrtTxRingDescr->pAllocAddr     = LSA_NULL;
            pNrtTxRingDescr->pTxRing        = LSA_NULL;
            pNrtTxRingDescr->ItemCount      = 0;
            pNrtTxRingDescr->FreeCount      = 0;
            pNrtTxRingDescr->InsertIdx      = 0;
            pNrtTxRingDescr->DoneIdx        = 0;
        }
    }

    return (EDD_STS_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_NRTDeInitSndRings                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_UINT16                  RingMask        +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  RingMask   : selector for TX-Ring (bits 0..15)                         +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_ALIGN                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: utility function for EDDP_NRTDeviceClose()                +*/
/*+               --> release memory for SndRings                           +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_NRTDeInitSndRings(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  RingMask)
{   
    LSA_UINT16      Idx;
    EDD_RSP         Response = EDD_STS_OK;

    // EDDP_ASSERT_NO_NULL_PTR(pDDB);

    // ***** step through all TX-rings and release memory
    for ( Idx=0; Idx < EDDP_NRT_TX_DMACW_CNT; Idx++ )
    {
        // ***** catch first error, but try to release all (remaining) rings
        if ( (1<<Idx) & RingMask )
        {
            if (Response == EDD_STS_OK)
            {
                Response = _EDDP_NRTDeInitSingleSndRing (pDDB, Idx);
            }
            else
            {
                _EDDP_NRTDeInitSingleSndRing (pDDB, Idx);
            }
        }
    }

    pDDB->NRT.UsedTxRingMask = 0;

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_NRTInitSndRings                       +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_IPB_NRT_PTR_TYPE       pIPBNrt         +*/
/*+                             LSA_UINT16                  RingMask        +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pIPBNrt    : Pointer to parameter structure                            +*/
/*+  RingMask   : selector for TX-Ring (bits 0..15)                         +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_ALIGN                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: utility function for EDDP_NRTDeviceOpen()                 +*/
/*+               --> alloc memory for SndRings                             +*/
/*+         Note: expect that internal members "pAllocAddr" were set to     +*/
/*+               NULL (before calling this function) to detect which       +*/
/*+               memeory must be release on error/in future                +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_NRTInitSndRings(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_DPB_NRT_PTR_TYPE       pIPBNrt,
    LSA_UINT32                  RingMask)
{   
    LSA_UINT16      Idx, Count;
    EDD_RSP         Response = EDD_STS_OK;

    // EDDP_ASSERT_NO_NULL_PTR(pDDB);

    pDDB->NRT.UsedTxRingMask = 0;

    // ***** step through all valid RX-rings and init ring with items if enabled by config
    for ( Idx=0; Idx < EDDP_NRT_TX_DMACW_CNT; Idx++ )
    {
        if ( (1<<Idx) & RingMask )
        {
            Count = 0;
            switch(Idx)
            {
                case EDDP_NRT_TX_GRP_DEFAULT:       Count = pIPBNrt->TxItemsInDefaultRing;
                    break;
                case EDDP_NRT_TX_GRP_TCP:           Count = pIPBNrt->TxItemsInTcpRing;
                    break;
                case EDDP_NRT_TX_GRP_UDP:           Count = pIPBNrt->TxItemsInUdpRing;
                    break;
                case EDDP_NRT_TX_GRP_IP_MISC:       Count = pIPBNrt->TxItemsInIpMiscRing;
                    break;
                case EDDP_NRT_TX_GRP_DCP:           Count = pIPBNrt->TxItemsInDcpRing;
                    break;
                case EDDP_NRT_TX_GRP_ALARM:         Count = pIPBNrt->TxItemsInAlarmRing;
                    break;
                case EDDP_NRT_TX_GRP_MRP:           Count = pIPBNrt->TxItemsInMrpRing;
                    break;
                case EDDP_NRT_TX_GRP_LLDP:          Count = pIPBNrt->TxItemsInLldpRing;
                    break;
                case EDDP_NRT_TX_GRP_SYNC:          Count = pIPBNrt->TxItemsInPtcpRing;
                    break;
                default:
                    EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] _EDDP_NRTInitSndRings(): ERROR -> unexpected RingIdx: %d", 
                        Idx);
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    break;
            }

            if (Response == EDD_STS_OK)
            {
                Response = _EDDP_NRTInitSingleSndRing (pDDB, Idx, Count);
            }

            if (Response != EDD_STS_OK)
            {
                break;  // stop init on error
            }

            if (Count)
            {
                pDDB->NRT.UsedTxRingMask |= (1<<Idx);
            }
        }
    }

    return (Response);
}






/*===========================================================================*/
/*                        public  functions                                  */
/*===========================================================================*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_NRTSendFrame                          +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_UINT16                  QueueIdx        +*/
/*+                             LSA_UINT16                  SendMaxItems    +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB         : Pointer to DeviceDescriptionBlock                       +*/
/*+  QueueIdx     : selector for TX-Queue                                   +*/
/*+  SendMaxItems : how much RQB are allowed for transmitting               +*/
/*+                                                                         +*/
/*+  Result       : EDD_STS_OK                                              +*/
/*+                 EDD_STS_ERR_PARAM                                       +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: utility function for EDDNRTSend() and ISR_Acyc_Snd_IFx    +*/
/*+               --> check if DMACW-space left and send TX-fame by using   +*/
/*+               RQB from selected TX-queue and write data into HW/ASIC    +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP _EDDP_NRTSendFrame(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  QueueIdx,
    LSA_UINT16                  SendMaxItems)
{   
    EDDP_LOCAL_DDB_NRT_PTR_TYPE             pNRTIface;
    EDDP_RQB_QUEUE_PTR_TYPE                 pTxRqbQueue;
    PNIP_LOCAL_DMACW_SND_ITEM_PTR_TYPE      pTxRing;
    EDD_UPPER_RQB_PTR_TYPE                  pRQB;
    EDD_UPPER_NRT_SEND_PTR_TYPE             pRQBSnd;
    EDDP_NRT_TX_RING_TYPE_PTR               pTxHwDescrRingMgm;
    PNIP_DMACW_SND_ITEM_TYPE                DmacwCfg;
    LSA_UINT32                              AsicAddr, TriggerRegister, TriggerMask, CurrTrigger;
    EDDP_LOCAL_HDB_PTR_TYPE                 pHDB;
    LSA_UINT16                              PortID;

    EDD_RSP Status = EDD_STS_OK;

    if( (LSA_NULL != pDDB) && ((1 << QueueIdx) & EDDP_NRT_TX_DMACW_MASK) )
    {
        // ***** index valid for own used TX-Rings
        pNRTIface   = &pDDB->NRT;
        pTxRqbQueue = &pNRTIface->TxQueue[QueueIdx].TxReqQueue;     // pointer for faster access

        // ***** EDDP_IF_A
        TriggerRegister = PNIP_REG_SNDTRIGGER_IFA;
        TriggerMask     = (PNIP_REG_SNDTRIGGER_IFA__MSK_SNDTRIGER_SD_0<<QueueIdx);

        CurrTrigger         = EDDP_HAL_REG32_READ(pDDB, TriggerRegister);
        pTxHwDescrRingMgm   = &pNRTIface->TxHwDescrRingMgm[QueueIdx];   // pointer for faster access

		while((pTxRqbQueue->Count) && (pTxHwDescrRingMgm->FreeCount) && (0U < SendMaxItems) )
        {
			// ***** something in queue for transmission

			// get TX-RQB
            pRQB = EDDP_GlbRQBQueueRemoveFromBegin(pTxRqbQueue);

			if( (EDD_STS_OK == EDDP_LL_TX_BUFF_AVAILABLE(pDDB->InterfaceID)) &&
				  (LSA_NULL != pRQB) && (LSA_NULL != pRQB->pParam) &&
				(EDD_STS_OK == EDDP_GlbHandleGetHDB (EDD_RQB_GET_HANDLE_LOWER(pRQB), &pHDB) ) &&
				(LSA_NULL != pHDB) && (0U < pHDB->pNRT->TxQueueReqCnt ) )
			{
				LSA_UINT16 BuffIdx = 0;
				pHDB->pNRT->TxQueueReqCnt--;        // item removed from queue and
				pHDB->pNRT->TxInProgCnt++;          // will be put into the ASIC
				pTxHwDescrRingMgm->FreeCount--;     // we need now one DMACW item for transmit

				// pointer to item for faster access
				pTxRing = &pTxHwDescrRingMgm->pTxRing[pTxHwDescrRingMgm->InsertIdx];

				// ***** set to next DMACW item and rewind to start in DMACW ring on index overflow
				if ( (++pTxHwDescrRingMgm->InsertIdx) >= pTxHwDescrRingMgm->ItemCount )
				{
					pTxHwDescrRingMgm->InsertIdx = 0;
				}

				//The macro gets the RQB and can modify the framebuffer content referenced by pPuffer within the RQB
				EDDP_NRT_SEND_HOOK(pDDB->hSysDev, pRQB);

				// pointer to TX data
				pRQBSnd = (EDD_UPPER_NRT_SEND_PTR_TYPE) pRQB->pParam;
				pRQBSnd->TxRingIdx = QueueIdx;
				pRQBSnd->hRQB = pRQB;
				pRQBSnd->hHDB = pHDB;
			
				Status = EDDP_LL_NRT_SEND(pDDB->InterfaceID, &pRQBSnd->pBuffer[BuffIdx], pRQBSnd->Length, pRQBSnd->PortID, (LSA_VOID*)pRQBSnd);

				pRQB = (EDD_UPPER_RQB_PTR_TYPE) pRQBSnd->hRQB;
				pHDB = (EDDP_LOCAL_HDB_PTR_TYPE)pRQBSnd->hHDB;
			
				if(EDD_STS_OK != Status)
				{
					EDDP_GlbRequestFinish (pHDB, pRQB, EDD_STS_ERR_TX);
				}
				else
				{
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
					pTxHwDescrRingMgm = &pNRTIface->TxHwDescrRingMgm[pRQBSnd->TxRingIdx];

					EDDP_GlbRequestFinish (pHDB, pRQB, EDD_STS_OK);

					pHDB->pNRT->TxInProgCnt--;
					pTxHwDescrRingMgm->FreeCount++;
				}
			}
			else
			{
				if(LSA_NULL != pRQB)
				{ // you come here on link down during send.
					Status = EDD_STS_ERR_TX;
					
					if(EDD_STS_OK == EDDP_GlbHandleGetHDB (EDD_RQB_GET_HANDLE_LOWER(pRQB), &pHDB) )
					{						
						// Send frame-send failure to PNO-Stack as send confirmation.
						EDDP_GlbRequestFinish (pHDB, pRQB, EDD_STS_ERR_TX);
					}
					else
					{
						// FATAL - ERROR, PNO stack will misbehave if we come here.
					}
				}
				else
				{
					Status = EDD_STS_OK_NO_DATA;
				}
			}

			SendMaxItems--;

        } // end while()

        EDDP_LL_SEND_TRIGGER(pDDB->InterfaceID);

        return (EDD_STS_OK);
    }
	else
	{
		Status = EDD_STS_ERR_PARAM;
	}

    return (EDD_STS_ERR_PARAM);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDNRTSend                                  +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDD_LOCAL_HDB_PTR_TYPE      pHDB            +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                         +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_ALIGN                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: utility fct for EDDP_NRTRequest(RQB==EDD_SRV_NRT_SEND)    +*/
/*+               Checks for Parameters and if ok queues request in tx-queue+*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTSend(
	EDD_UPPER_RQB_PTR_TYPE      pRQB,
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{   
    LSA_UINT32                      MinLength;
    LSA_UINT32                      AsicAddr;
    EDDP_LOCAL_DDB_NRT_PTR_TYPE     pNRTIface;
    EDDP_RQB_QUEUE_PTR_TYPE         pTxRqbQueue;
    EDD_UPPER_NRT_SEND_PTR_TYPE     pRQBSnd = (EDD_UPPER_NRT_SEND_PTR_TYPE)pRQB->pParam;
    LSA_UINT16                      Idx;

    // pHDB->pNRT   checked by function caller !
    // pRQB->pParam checked by function caller !
    
    if (LSA_HOST_PTR_ARE_EQUAL(pRQBSnd->pBuffer, LSA_NULL)) 
    {
        EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_NRTSend(pRQB=0x%X): ERROR -> pTxBuffer is NULL!", pRQB);
	    EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
        return EDD_STS_ERR_PARAM; 
    }

    EDDP_DEV_MEM_ADDR_UPPER_TO_PNIP(pDDB->hSysDev, pRQBSnd->pBuffer, &AsicAddr);

    // ***** check 32bit alignment
    if (AsicAddr & EDDP_HAL_32BIT_ALIGNMENT)
    {
        EDDP_NRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_NRTSend(): ERROR -> HostTxBuf=0x%X, HwTxBuf=0x%X not 32Bit aligned!", 
            pHDB->Handle, pRQBSnd->pBuffer, AsicAddr);
        EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_ALIGN);
        return (EDD_STS_ERR_ALIGN);
    }

    {
        MinLength = EDDP_NRT_TX_MIN_SND_LEN_REV12;
    }
    
    if (pRQBSnd->Length < MinLength)
    {
        EDDP_NRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_NRTSend(pRQB=0x%X): ERROR -> TxBuffer too short(%d), minimum length(%d)", 
            pRQB, pRQBSnd->Length, MinLength);
        EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
        return (EDD_STS_ERR_PARAM);
    }

    if (pRQBSnd->Length > EDD_NRT_MAX_SND_LEN) 
    {
        EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_NRTSend(pRQB=0x%X): ERROR -> TxBuffer too long(%d)", 
            pRQB, pRQBSnd->Length);
	    EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
        return (EDD_STS_ERR_PARAM);
    }

    if (pRQBSnd->PortID != EDD_PORT_ID_AUTO)
    {
        if (EDDP_SWI_get_HwPortID(pDDB, pRQBSnd->PortID) == 0)
        {
            EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_NRTSend(pRQB=0x%X): ERROR -> invalid PortID(%d)", 
                pRQB, pRQBSnd->PortID);
	        EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
            return (EDD_STS_ERR_PARAM);
        }
    }
    
    if (   (pRQBSnd->TxFrmGroup == EDD_NRT_TX_GRP_DLR)
        && (pDDB->FeatureSupport.DLRExtensions != EDD_FEATURE_ENABLE)) 
    {
        EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_NRTSend(pRQB=0x%X): ERROR -> DLR frame requested to be sent but DLRExtensions not ENABLED", 
            pRQB);
        EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_NOT_SUPPORTED);
        return (EDD_STS_ERR_NOT_SUPPORTED);
    }
    
    if (   (pRQBSnd->TxFrmGroup == EDD_NRT_TX_GRP_MRP)
        && (pDDB->FeatureSupport.DLRExtensions == EDD_FEATURE_ENABLE)) 
    {
        EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_NRTSend(pRQB=0x%X): ERROR -> Sending of MRP frames not supported with DLRExtensions ENABLED", 
            pRQB);
        EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_NOT_SUPPORTED);
        return (EDD_STS_ERR_NOT_SUPPORTED);
    }

    pNRTIface = pHDB->pNRT->pNrtDDB;

    // ***** find right RQB-Queue
    switch (pRQBSnd->TxFrmGroup)
    {
        case EDD_NRT_TX_GRP_DEFAULT:        /* Queue: DEFAULT=0             */
            Idx = EDDP_NRT_TX_GRP_DEFAULT;
            break;
        case EDD_NRT_TX_GRP_IP_ICMP:        /* Queue: IP_MISC=3             */
        case EDD_NRT_TX_GRP_IP_IGMP:
            Idx = EDDP_NRT_TX_GRP_IP_MISC;
            break;
        case EDD_NRT_TX_GRP_IP_TCP:         /* Queue: TCP=1                 */
            Idx = EDDP_NRT_TX_GRP_TCP;
            break;
        case EDD_NRT_TX_GRP_IP_UDP:         /* Queue: UDP=2                 */
            Idx = EDDP_NRT_TX_GRP_UDP;
            break;
        case EDD_NRT_TX_GRP_ARP:            /* Queue: IP_MISC=3             */
            Idx = EDDP_NRT_TX_GRP_IP_MISC;
            break;
        case EDD_NRT_TX_GRP_DCP:            /* Queue: DCP=4                  */
            Idx = EDDP_NRT_TX_GRP_DCP;
            break;
        case EDD_NRT_TX_GRP_ASRT:           /* Queue: ALARM=5                */
            Idx = EDDP_NRT_TX_GRP_ALARM;
            break;
        case EDD_NRT_TX_GRP_MRP:            /* Queue: MRP=6                  */
            Idx = EDDP_NRT_TX_GRP_MRP;
            break;
        case EDD_NRT_TX_GRP_LEN_TSYNC:      /* Queue: DEFAULT=0             */
            Idx = EDDP_NRT_TX_GRP_DEFAULT;
            break;
        case EDD_NRT_TX_GRP_LEN_STDBY:      /* Queue: MRP=6                  */
        case EDD_NRT_TX_GRP_LEN_HSR:
            Idx = EDDP_NRT_TX_GRP_MRP;
            break;
        case EDD_NRT_TX_GRP_LLDP:           /* Queue: LLDP=7                 */
            Idx = EDDP_NRT_TX_GRP_LLDP;
            break;
        case EDD_NRT_TX_GRP_PTCP:           /* Queue: SYNC=8                 */
            Idx = EDDP_NRT_TX_GRP_SYNC;
            break;
        case EDD_NRT_TX_GRP_DLR:            /* Queue: MRP=6                  */
            Idx = EDDP_NRT_TX_GRP_MRP;
            break;
        default:
            Idx = EDDP_NRT_TX_GRP_CNT+1;    /* invalid                       */
            EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_NRTSend(pRQB=0x%X): ERROR -> invalid TxFrmGroup: %d", 
                pRQB, pRQBSnd->TxFrmGroup);
            EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
            break;
    }

    if (Idx <= EDDP_NRT_TX_DMACW_MAX) 
    {
        if (pNRTIface->UsedTxRingMask & (1 <<Idx))
        {
            pTxRqbQueue = &pNRTIface->TxQueue[Idx].TxReqQueue;

         //   EDDP_LL_ENTER(); // Lock EDDP-LLIF

            EDDP_GlbRQBQueueAddToEnd(pTxRqbQueue, pRQB);
            
            EDDP_NRT_TRACE_07(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:%8X] EDDP_NRTSend(pRQB=0x%X, TxQueue[%d], PortID=%d, Len=%d, pBuf=0x%X) added, new RqbQueue.Count=%d", 
                pHDB->pNRT->EDD_Handle, pRQB, Idx, pRQBSnd->PortID, pRQBSnd->Length, pRQBSnd->pBuffer, pTxRqbQueue->Count);

            pHDB->pNRT->TxQueueReqCnt++;

            _EDDP_NRTSendFrame(pDDB, Idx, 1);   // check if DMACW-space left and send first RQB from selected TX-queue

       //     EDDP_LL_EXIT(); // Unlock EDDP-LLIF

            return (EDD_STS_OK);
        }
        else
        {
            EDDP_NRT_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H:%8X] EDDP_NRTSend(pRQB=0x%X, TxQueue[%d], PortID=%d, Len=%d, pBuf=0x%X) not accepted, no items configured", 
                pHDB->pNRT->EDD_Handle, pRQB, Idx, pRQBSnd->PortID, pRQBSnd->Length, pRQBSnd->pBuffer);
            return (EDD_STS_ERR_RESOURCE);
        }
    }

    return (EDD_STS_ERR_PARAM);
}
