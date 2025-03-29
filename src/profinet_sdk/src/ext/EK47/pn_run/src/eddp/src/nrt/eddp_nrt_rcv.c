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
/*  F i l e               &F: eddp_nrt_rcv.c                            :F&  */
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

#define LTRC_ACT_MODUL_ID  66
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
/*+  Functionname          :    _EDDP_NRTInitSingleRcvRingWithBuffers       +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_UINT16                  RingIdx         +*/
/*+                             LSA_UINT16                  Count           +*/
/*+                             LSA_UINT16                  RxBuf32Size     +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  RingIdx    : index of RX-Ring (0..31)                                  +*/
/*+  Count      : nr of items in RX-Ring                                    +*/
/*+  RxBuf32Size: buffersize in byte that is to be alloced and the given    +*/
/*+               size must be valid with modulo 32bit                      +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_ALIGN                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: utility function for _EDDP_NRTInitRcvRings()              +*/
/*+               --> alloc RX-memory for RcvRing and memory for RX-buffers +*/
/*+         NOTE: expect that interrupts are disabled for this ring !!!     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_NRTInitSingleRcvRingWithBuffers(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  RingIdx,
    LSA_UINT16                  Count,
    LSA_UINT16                  RxBuf32Size)
{   
    EDDP_LOCAL_DDB_NRT_PTR_TYPE         pNRTIface;
    LSA_UINT16                          Idx;
    LSA_UINT32                          AsicAddr, AllocSize, TmpCfg;
    EDDP_NRT_RX_RING_TYPE_PTR           pNrtRxRingDescr;
    PNIP_LOCAL_DMACW_RCV_ITEM_PTR_TYPE  pNrtRxRingItems, pNrtRxPrevItem, pNrtRxCurrItem;
    EDD_RSP                             Response = EDD_STS_OK;

    // EDDP_ASSERT_NO_NULL_PTR(pDDB);

    if (Count && RxBuf32Size)
    {
        pNRTIface = &pDDB->NRT;                                                                             // pointer to NRT-mgm for faster access

        if (RingIdx < EDDP_NRT_RX_DMACW_CNT)
        {
            // ***** alloc memory for ring items
            // pointer to RX-Ring-mgm for faster access
            pNrtRxRingDescr = &pNRTIface->RxHwDescrRingMgm[RingIdx];
            // extra bytes for 32bit alignment shifting
            AllocSize = (sizeof(PNIP_DMACW_RCV_ITEM_TYPE)*(LSA_UINT32)(Count))+EDDP_HAL_32BIT_ALIGNMENT;
            // real memory address
            pNrtRxRingDescr->pAllocAddr = EDDP_GlbAllocUpperDevMem(pDDB, AllocSize);

            if (LSA_HOST_PTR_ARE_EQUAL(pNrtRxRingDescr->pAllocAddr, LSA_NULL))
            {
                Response = EDD_STS_ERR_RESOURCE;
            }
            else
            {
                LSA_UINT16 const SizeOfRxBufferArrayStruct = sizeof(EDDP_NRT_RX_FRAMEBUFFER_ARRAY) + (Count-1)*sizeof(EDD_UPPER_MEM_PTR_TYPE);

                // ***** memory for ring items exist --> connect all pointers (HW view) into a ring
                // ***** trim to 32bit alignment in Asic view and adapt address in host view to asic view                
                pNrtRxRingItems = (PNIP_LOCAL_DMACW_RCV_ITEM_PTR_TYPE) EDDP_NRTUpperAdaptAlignment(pDDB, pNrtRxRingDescr->pAllocAddr);

                // ***** write mgmt for the DMACW ring
                pNrtRxRingDescr->pRxRing        = pNrtRxRingItems;              // head of dmacw ring
                pNrtRxRingDescr->ItemCount      = Count;                        // nr of items in ring
                pNrtRxRingDescr->DoneIdx        = 0;                            // index position in DMACW ring for processed requests
                pNrtRxRingDescr->ExpBuf32Size   = RxBuf32Size;                  // expected buffer size for max. frame data
                pNrtRxPrevItem                  = &pNrtRxRingItems[Count-1];    // build linkage queue from previous to next item !!!
                // ***** write HW entries into the DMACW items of the DMACW ring

                //Alloc array for storing RX frame buffers
                pNrtRxRingDescr->pFrameBufferArray = (EDDP_NRT_RX_FRAMEBUFFER_ARRAY_PTR)EDDP_GlbAllocLocalMem(SizeOfRxBufferArrayStruct);
                if (LSA_HOST_PTR_ARE_EQUAL(pNrtRxRingDescr->pFrameBufferArray , LSA_NULL))
                {
                    Response = EDD_STS_ERR_RESOURCE;
                }
                else
                {
                    pNrtRxRingDescr->pFrameBufferArray->NrOfElements = Count;
                    pNrtRxRingDescr->pFrameBufferArray->SizeOfStruct = SizeOfRxBufferArrayStruct;

                    for ( Idx=0; Idx < Count; Idx++ )
                    {
                        // real memory address for all RX-buffers in the ring
                        pNrtRxRingDescr->pFrameBufferArray->FrameBufferUpperAddr[Idx] = EDDP_GlbAllocUpperRxMem (pDDB, RxBuf32Size);
                        if (LSA_HOST_PTR_ARE_EQUAL(pNrtRxRingDescr->pFrameBufferArray->FrameBufferUpperAddr[Idx], LSA_NULL))
                        {
                            // ***** error: release previous alloced buffer below
                            // set count of valid alloced RX-buffers
                            pNrtRxRingDescr->ItemCount = Idx;
                            Response = EDD_STS_ERR_RESOURCE;
                            break;
                        }

                        // get DMACW item
                        pNrtRxCurrItem = &pNrtRxRingItems[Idx];

                        // ***** HW entries
                        EDDP_DEV_MEM_ADDR_UPPER_TO_PNIP(pDDB->hSysDev, pNrtRxCurrItem, &AsicAddr);
                        EDDP_HAL_MEM32_WRITE(pDDB, (EDD_COMMON_MEM_U32_PTR_TYPE)&pNrtRxPrevItem->NextDMACWPtr,     AsicAddr); // link on prev. item !!!
                        EDDP_HAL_MEM32_WRITE(pDDB, &pNrtRxCurrItem->TimeStamp, 0);                                // zero
                        EDDP_RX_MEM_ADDR_UPPER_TO_PNIP(pDDB->hSysDev, pNrtRxRingDescr->pFrameBufferArray->FrameBufferUpperAddr[Idx], &AsicAddr);
                        EDDP_HAL_MEM32_WRITE(pDDB, (EDD_COMMON_MEM_U32_PTR_TYPE)&pNrtRxCurrItem->FrameBufferPtr,   AsicAddr); // bind RX-buffer
    #if (EDDP_NRT_CHECK_FDC_FIELD == 1)
                        // ***** set FDC to a defined value --> HW must update this field with received frame !!!!
                        EDDP_HAL_MEM32_WRITE(pDDB, &pNrtRxCurrItem->RcvInf2,   PNIP_DMACW_RCV_ITEM_INF2_DEF_FDC(EDDP_FDC_INVALID));     // default value for cfg
    #else
                        EDDP_HAL_MEM32_WRITE(pDDB, &pNrtRxCurrItem->RcvInf2,   PNIP_DMACW_RCV_ITEM_INF2_DEF);     // default value for cfg
    #endif
                        // max_frm_size given in 32bit units !!!
                        TmpCfg = PNIP_DMACW_RCV_ITEM_INF1_DEF_HW( (((RxBuf32Size > EDDP_RX_FRAME_BUFFER_LENGTH)?EDDP_RX_FRAME_BUFFER_LENGTH:(RxBuf32Size+3))>>2) );
                        EDDP_HAL_MEM32_WRITE(pDDB, &pNrtRxCurrItem->RcvInf1,  TmpCfg);                            // default cfg with frame buffer
                        pNrtRxPrevItem = pNrtRxCurrItem;
                    }
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
/*+  Functionname          :    _EDDP_NRTDeInitSingleRcvRingAndBuffers      +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_UINT16                  RingIdx         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  RingIdx    : index of RX-Ring (0..31)                                  +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_ALIGN                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: utility function for _EDDP_NRTInitRcvRings()              +*/
/*+               --> release RcvRing-memory and RX-buffers                 +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_NRTDeInitSingleRcvRingAndBuffers(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  RingIdx)
{   
    EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE  pNrtApiCtrlIfaceMgm;
    EDDP_LOCAL_DDB_NRT_PTR_TYPE                 pNRTIface;
    PNIP_LOCAL_DMACW_RCV_DESCR_PTR_TYPE         pRcvDescr;
    PNIP_LOCAL_DMACW_RCV_ITEM_PTR_TYPE          pNrtRxCurrItem, pNrtRxRingItems;
    EDDP_NRT_RX_RING_TYPE_PTR                   pRxRing;
    LSA_UINT16                                  Idx, Count;
    EDD_RSP                                     Response = EDD_STS_OK;

    // EDDP_ASSERT_NO_NULL_PTR(pDDB);

    pNrtApiCtrlIfaceMgm = (EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE)pDDB->NRT.pNrtApiCtrlIfaceMgm;
    pNRTIface           = &pDDB->NRT;
    pRcvDescr           = &pNrtApiCtrlIfaceMgm->ApiCtrlRamRcvDmacw[RingIdx];
    pRxRing             = &pNRTIface->RxHwDescrRingMgm[RingIdx];

    // ***** release TX-Mgm-memeory if pAllocAddr set
    if (pRxRing->pAllocAddr)
    {
        Count           = pRxRing->ItemCount;
        pNrtRxRingItems = pRxRing->pRxRing;

        for ( Idx=0; Idx < Count; Idx++ )
        {
            // get DMACW item
            pNrtRxCurrItem = &pNrtRxRingItems[Idx];

            if (LSA_HOST_PTR_ARE_EQUAL(pNrtRxCurrItem, LSA_NULL))
            {
                Response = EDD_STS_ERR_RESOURCE;
                break;
            }

            if ((LSA_UINT32)pRxRing->pFrameBufferArray->FrameBufferUpperAddr[Idx] != EDDP_PNIP_NULL_PTR)
            {
                if (EDDP_GlbFreeUpperRxMem(pDDB, pRxRing->pFrameBufferArray->FrameBufferUpperAddr[Idx]) != LSA_RET_OK)
                {
                    if (Response == EDD_STS_OK)
                    {
                        Response = EDD_STS_ERR_RESOURCE;
                    }
                }
            }
        }

        //Free array of RX buffers
        if (EDDP_GlbFreeLocalMem(pRxRing->pFrameBufferArray) != LSA_RET_OK)
        {
            if (Response == EDD_STS_OK)
            {
                Response = EDD_STS_ERR_RESOURCE;
            }
        }

        if (EDDP_GlbFreeUpperDevMem(pDDB, pRxRing->pAllocAddr) == LSA_RET_OK)
        {
            // ***** set RX-Descr "unused"
            EDDP_HAL_MEM32_WRITE(pDDB, &pRcvDescr->RcvDropCntTsErrIntVal, PNIP_DMACW_RCV_DESCR_CFG_DEF_SW);   // DMAWCDropCounter=0, no IRQs
            EDDP_HAL_MEM32_WRITE(pDDB, &pRcvDescr->CurrentDmacwHwPtr, EDDP_PNIP_NULL_PTR);                    // empty ring

            pRxRing->pAllocAddr     = LSA_NULL;
            pRxRing->pRxRing        = LSA_NULL;
            pRxRing->ItemCount      = 0;
            pRxRing->DoneIdx        = 0;
            pRxRing->ExpBuf32Size   = 0;
        }
        else
        {
            if (Response == EDD_STS_OK)
            {
                Response = EDD_STS_ERR_RESOURCE;
            }
        }
    }

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_NRTInitRcvRingsEmpty                  +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_UINT32                  RingMask        +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  RingMask   : selector for RX-Ring (bits 0..31)                         +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_ALIGN                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: utility function for _EDDP_NRTInitInterface()             +*/
/*+               --> set empty HW RcvRings                                 +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_NRTInitRcvRingsEmpty(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  RingMask)
{   
    LSA_UINT32                                  Idx;
    EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE  pNrtApiCtrlIfaceMgm;
    EDDP_NRT_RX_RING_TYPE_PTR                        pNrtRxRingDescr;
    PNIP_LOCAL_DMACW_RCV_DESCR_PTR_TYPE         pRcvPtr;

    // EDDP_ASSERT_NO_NULL_PTR(pDDB);
    pNrtApiCtrlIfaceMgm = (EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE)pDDB->NRT.pNrtApiCtrlIfaceMgm;

    for ( Idx=0; Idx < EDDP_NRT_RX_DMACW_CNT; Idx++ )
    {
        if ( (1<<Idx) & RingMask )
        {
            //***** set RX-Descr "unused",  (special) nullptr in ASIC !!!
            pRcvPtr = &pNrtApiCtrlIfaceMgm->ApiCtrlRamRcvDmacw[Idx];
            if (   (pDDB->FeatureSupport.DLRExtensions == EDD_FEATURE_ENABLE)
                && (Idx == EDDP_ARD_INDEX_MRP))
            {
                EDDP_HAL_MEM32_WRITE(pDDB, &pRcvPtr->RcvDropCntTsErrIntVal, PNIP_DMACW_RCV_DESCR_CFG_DEF_SW&~PNIP_DMACW_RCV_DESCR_TS_ON); // switch off TagStripping for the MRP/DLR ring
            }
            else
            {
                EDDP_HAL_MEM32_WRITE(pDDB, &pRcvPtr->RcvDropCntTsErrIntVal, PNIP_DMACW_RCV_DESCR_CFG_DEF_SW); // DMAWCDropCounter=0, no IRQs
            }
            EDDP_HAL_MEM32_WRITE(pDDB, &pRcvPtr->CurrentDmacwHwPtr, EDDP_PNIP_NULL_PTR);                      // empty ring

            // ***** set SW mgm info for this RX-ring to "unused"
            pNrtRxRingDescr                 = &pDDB->NRT.RxHwDescrRingMgm[Idx];
            pNrtRxRingDescr->pAllocAddr     = LSA_NULL;
            pNrtRxRingDescr->pRxRing        = LSA_NULL;
            pNrtRxRingDescr->ItemCount      = 0;
            pNrtRxRingDescr->DoneIdx        = 0;
            pNrtRxRingDescr->ExpBuf32Size   = 0;
        }
    }

    return (EDD_STS_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_NRTInitRcvRings                       +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_IPB_NRT_PTR_TYPE       pIPBNrt         +*/
/*+                             LSA_UINT32                  RingMask        +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pIPBNrt    : Pointer to parameter structure                            +*/
/*+  RingMask   : selector for RX-Ring (bits 0..31)                         +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_ALIGN                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: utility function for _EDDP_NRTInitInterface()             +*/
/*+               --> alloc memory for RcvRings and its RX-buffers          +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_NRTInitRcvRings(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_DPB_NRT_PTR_TYPE       pIPBNrt,
    LSA_UINT32                  RingMask)
{   
    LSA_UINT16  Idx, Count, Buf32Size;
    EDD_RSP     Response = EDD_STS_OK;

    // EDDP_ASSERT_NO_NULL_PTR(pDDB);
    pDDB->NRT.UsedRxRingMask = 0;

    // ***** step through all valid RX-rings and init ring with items if enabled by config
    for ( Idx=0; Idx < EDDP_NRT_RX_DMACW_CNT; Idx++)
    {
        if ( (1<<Idx) & RingMask )
        {
            Count = 0;

            switch (Idx)
            {
                case EDDP_ARD_INDEX_DROP:       if (EDD_FEATURE_ENABLE == pIPBNrt->RxDropLogging )   { Count = 32;  }
                    break;
                case EDDP_ARD_INDEX_OTHER:      Count = pIPBNrt->RxItemsInOtherRing;
                    break;
                case EDDP_ARD_INDEX_IP_MISC:    Count = pIPBNrt->RxItemsInIpMiscRing;
                    break;
                case EDDP_ARD_INDEX_IP_TCP:     Count = pIPBNrt->RxItemsInTcpRing;
                    break;
                case EDDP_ARD_INDEX_IP_UDP:     Count = pIPBNrt->RxItemsInUdpRing;
                    break;
                case EDDP_ARD_INDEX_ALARM:      Count = pIPBNrt->RxItemsInAlarmRing;
                    break;
                case EDDP_ARD_INDEX_DCP:        Count = pIPBNrt->RxItemsInDcpRing;
                    break;
                case EDDP_ARD_INDEX_PTCP_ANN:   Count = pIPBNrt->RxItemsInPtcpRing;
                    break;
                case EDDP_ARD_INDEX_LLDP:       Count = pIPBNrt->RxItemsInLldpRing;
                    break;
                case EDDP_ARD_INDEX_MRP:        Count = pIPBNrt->RxItemsInMrpRing;
                    break;
                case EDDP_ARD_INDEX_MISC:       Count = pIPBNrt->RxItemsInMiscRing;
                    break;
                case EDDP_ARD_INDEX_K32_SYNC_CLOCK:
                case EDDP_ARD_INDEX_K32_SYNC_TIME:
                case EDDP_ARD_INDEX_K32_SYNC_DELAY:
                    Count = 8;
                    break;
                default:
                    EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] _EDDP_NRTInitRcvRings(): ERROR -> unexpected RingIdx: %d", 
                        Idx);
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    break;
            }

            Buf32Size = (LSA_UINT16)EDD_FRAME_BUFFER_LENGTH; // buffer size for allocation

            if (Response == EDD_STS_OK)
            {
                Response = _EDDP_NRTInitSingleRcvRingWithBuffers (pDDB, Idx, Count, Buf32Size);
            }

            if (Response != EDD_STS_OK)
            {
                break; // stop init on error
            }

            if (Count)
            {
                pDDB->NRT.UsedRxRingMask |= (1<<Idx);
            }
        }
    }

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_NRTDeInitRcvRings                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_UINT32                  RingMask        +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  RingMask   : selector for RX-Ring (bits 0..31)                         +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_ALIGN                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: utility function for _EDDP_NRTDeInitInterface()           +*/
/*+               --> release memory for SndRings and its RX-buffers        +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_NRTDeInitRcvRings(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  RingMask)
{   
    LSA_UINT16 Idx;
    EDD_RSP    Response = EDD_STS_OK;

    // EDDP_ASSERT_NO_NULL_PTR(pDDB);

    // ***** step through all RX-rings and release memory + RX-buffers
    for ( Idx=0; Idx < EDDP_NRT_RX_DMACW_CNT; Idx++ )
    {
        // ***** catch first error, but try to release all (remaining) rings
        if (RingMask & (1<<Idx))
        {
            if (Response == EDD_STS_OK)
            {
                Response = _EDDP_NRTDeInitSingleRcvRingAndBuffers (pDDB, Idx);
            }
            else
            {
                _EDDP_NRTDeInitSingleRcvRingAndBuffers (pDDB, Idx);
            }
        }
    }

    pDDB->NRT.UsedRxRingMask = 0;

    return (Response);  
}




/*===========================================================================*/
/*                        public  functions                                  */
/*===========================================================================*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRTRecv                                +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDD_LOCAL_HDB_PTR_TYPE      pHDB            +*/
/*+  Result                :    LSA_RESULT                                  +*/
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
/*+               EDD_STS_ERR_CHANNEL_USE                                   +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: utility fct for EDDP_NRTRequest(RQB==EDD_SRV_NRT_RECV)    +*/
/*+               Checks for Parameters and if ok queues request in rx-queue+*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTRecv(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    LSA_UINT32                      AsicAddr, PortIdx;
    EDD_UPPER_NRT_RECV_PTR_TYPE     pRQBRcv = (EDD_UPPER_NRT_RECV_PTR_TYPE) pRQB->pParam;

    // pHDB->pNRT   checked by function caller !
    // pRQB->pParam checked by function caller !

    //initialize IOCount (RQ 1839107, Task 1839257)
    pRQBRcv->IOCount = 0;

    if (LSA_HOST_PTR_ARE_EQUAL(pRQBRcv->pBuffer, LSA_NULL)) 
    {
        EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_NRTRecv(pRQB=0x%X): ERROR -> pRxBuffer is NULL!", pRQB);
        EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
	    return EDD_STS_ERR_PARAM; 
    }

    if (pHDB->pNRT->UsrFrameFilter)
    {
        EDDP_DEV_MEM_ADDR_UPPER_TO_PNIP(pDDB->hSysDev, pRQBRcv->pBuffer, &AsicAddr);
        // ***** check 32bit alignment
        if (AsicAddr & EDDP_HAL_32BIT_ALIGNMENT)
        {
            EDDP_NRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:%2X] EDDP_NRTRecv(): ERROR -> HostRxBuf=0x%X, HwRxBuf=0x%X not 32Bit aligned!", 
                pHDB->Handle, pRQBRcv->pBuffer, AsicAddr);
            EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_ALIGN);
	        return EDD_STS_ERR_ALIGN; 
        }

        for ( PortIdx=0; PortIdx < EDD_CFG_MAX_PORT_CNT; PortIdx++ )
        {   // Note: Because the EDDP always forwards Announce frames in hardware, LSA_FALSE is returned for all Ports.
            pRQBRcv->doSwFwd[PortIdx] = EDD_NRT_RECV_DO_NOT_FWD_BY_UPPER_LAYER;
            // @kochn: set to always forward
            pRQBRcv->doSwFwd[PortIdx] = EDD_NRT_RECV_DO_FWD_BY_UPPER_LAYER;
        }

        EDDP_GlbRQBQueueAddToEnd (&pHDB->pNRT->RxUsrQueue, pRQB);

        EDDP_NRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%8X] EDDP_NRTRecv(pRQB=0x%X) is queued: new RxUsrQueue.Count=%d", 
            pHDB->pNRT->EDD_Handle, pRQB, pHDB->pNRT->RxUsrQueue.Count);

        return (EDD_STS_OK);
    }

    EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_NRTRecv(pRQB=0x%X): ERROR -> TX only enabled!", pRQB);

    return (EDD_STS_ERR_PARAM);
}
