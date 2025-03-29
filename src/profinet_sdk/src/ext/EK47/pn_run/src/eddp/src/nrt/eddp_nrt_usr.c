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
/*  F i l e               &F: eddp_nrt_usr.c                            :F&  */
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
/*  11.11.09    lrg   111109lrg001: Set RequestCnt on EDD_STS_OK_CANCEL.     */
/*  24.11.10    JS    added casts to prevent compiler warning                */
#endif
/*****************************************************************************/



/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/

#define LTRC_ACT_MODUL_ID  64
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
/*+  Functionname          :    _EDDP_NRTDCPFilterCheckParam                +*/
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
/*+  Result                :    EDD_STS_OK                                  +*/
/*+                             EDD_STS_ERR_PARAM                           +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Checks parameter of EDD_SRV_NRT_SET_DCP_FILTER request.   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_NRTDCPFilterCheckParam(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDDP_LOCAL_DDB_NRT_PTR_TYPE                 pNrtDDB;
    EDD_UPPER_NRT_SET_DCP_FILTER_PTR_TYPE       pNRTDcpSetFilter;
    EDD_RSP                                     Response;



    Response            = EDD_STS_OK;
    pNRTDcpSetFilter    = (EDD_UPPER_NRT_SET_DCP_FILTER_PTR_TYPE) pRQB->pParam;     // tested in EDDP_NRTRequest()
    pNrtDDB             = pHDB->pNRT->pNrtDDB; // ***** pHDB->pNRT tested by caller

    /* check pNrtDDB -> FATAL */
    EDDP_ASSERT_NO_NULL_PTR(pNrtDDB);

    /* check pNRTDcpSetFilter->Mode -> ERR_PARAM */
    if( (EDD_DCP_FILTER_OFF != pNRTDcpSetFilter->Mode) &&
                (EDD_DCP_FILTER_ON != pNRTDcpSetFilter->Mode))
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] _EDDP_NRTDCPFilterCheckParam(): ERROR -> unknown RQB-Mode(0x%X)!",
            pHDB->Handle, pNRTDcpSetFilter->Mode);
    }

    if(EDD_STS_OK == Response)
    {
        /* for Mode == ON */
        if(EDD_DCP_FILTER_ON == pNRTDcpSetFilter->Mode)
        {
            /* check FilterTLVCnt -> ERR_PARAM */
            if ((pNRTDcpSetFilter->FilterTLVCnt == 0) || (pNRTDcpSetFilter->FilterTLVCnt > pNrtDDB->DcpFilterCnt))
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_NRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] _EDDP_NRTDCPFilterCheckParam(): ERROR -> RQB-FilterTLVCnt(%d) is out of range (1..%d) !",
                    pHDB->Handle, pNRTDcpSetFilter->FilterTLVCnt, pNrtDDB->DcpFilterCnt);
            }

            if(EDD_STS_OK == Response)
            {
                LSA_UINT8 index;
                /* for each FilterTLV */
                for(index = 0; index < pNRTDcpSetFilter->FilterTLVCnt; index++)
                {
                    LSA_UINT16 DcpFilterLen;
                    LSA_UINT16 *pDcpFilterData;

                    /* check pTLV not NULL */
                    if(LSA_HOST_PTR_ARE_EQUAL(LSA_NULL,pNRTDcpSetFilter->FilterTLV[index].pTLV))
                    {
                        Response = EDD_STS_ERR_PARAM;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] _EDDP_NRTDCPFilterCheckParam(): ERROR -> no filter data ! Filter-Idx=%d",
                            pHDB->Handle, index);
                        /* to end for loop */
                        break;
                    }

                    /* check TLVLen */
                    pDcpFilterData = (LSA_UINT16*)pNRTDcpSetFilter->FilterTLV[index].pTLV;

                    #ifdef EDDP_CFG_BIG_ENDIAN
                    DcpFilterLen = *(pDcpFilterData+1);
                    #else
                    DcpFilterLen = EDDP_SWAP16(*(pDcpFilterData+1));
                    #endif

                    if (   (DcpFilterLen != (pNRTDcpSetFilter->FilterTLV[index].TLVLen - 4)) // length unequal to the TLVLen value
                        || (DcpFilterLen > (EDD_DCP_MAX_SET_DCP_FILTER_TLV_LEN - 4))         // length too long
                        || (DcpFilterLen == 0)                                               // length too short
                       ) // check for length without TypeHi, TypeLo, and LenHi, LenLo
                    {
                        Response = EDD_STS_ERR_PARAM;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_NRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] _EDDP_NRTDCPFilterCheckParam(): ERROR -> DcpLen(%d) is unequal 'TLVLen-4'(%d) or not in range of the allowed size (1...%d)! Filter-Idx=%d",
                            pHDB->Handle, DcpFilterLen, (pNRTDcpSetFilter->FilterTLV[index].TLVLen - 4), (EDD_DCP_MAX_SET_DCP_FILTER_TLV_LEN-4), index);
                        /* to end for loop */
                        break;
                    }

                }
            }
        }
    }

    return (Response);
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_NRTFilterGetStringOffs                +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB        +*/
/*+                             EDDP_LOCAL_DDB_NRT_PTR_TYPE     pNrtDDB     +*/
/*+                             LSA_UINT8                       nIdx        +*/
/*+  Result                :    LSA_UINT32                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pNrtDDB    : Pointer to global NRT mgm                                 +*/
/*+  nIdx       : string index defined by the filter code program           +*/
/*+                                                                         +*/
/*+  Result     : 0 for error, else calculated offset                       +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: utility function for EDDP_NRT???Filter()                  +*/
/*+               --> calculate offset for StringPtr used within            +*/
/*+               StringDescriptor (SCMP opcode)                            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_UINT32 _EDDP_NRTFilterGetStringOffs(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDDP_LOCAL_DDB_NRT_PTR_TYPE     pNrtDDB,
    LSA_UINT8                       nIdx)
{
    EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE      pNrtApiCtrlIfaceMgm;
    LSA_UINT32                                      nRet = 0;

    LSA_UNUSED_ARG(pDDB);

    pNrtApiCtrlIfaceMgm = (EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE) pNrtDDB->pNrtApiCtrlIfaceMgm;

    switch (EDDP_HALGetPnipRevision(pDDB))
    {
#if defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT) ||defined (EDDP_CFG_HW_PNIP_REV2_SUPPORT)
        case EDDP_HAL_HW_PNIP_REV1:
        case EDDP_HAL_HW_PNIP_REV2:
            switch (nIdx)
            {
                // ***** DCP Interface
                case EDDP_SDT_IDX_DCP_01TYPE:       // ; SDT idx 0  0000    --> Idx 0
                case EDDP_SDT_IDX_DCP_01TLV:        // ; SDT idx 1  0001    --> Idx 0
                // ***** DCP Port 0..3
                case EDDP_SDT_IDX_DCP_02TYPE:       // ; SDT idx 2  0010    --> Idx 1
                case EDDP_SDT_IDX_DCP_02TLV:        // ; SDT idx 3  0011    --> Idx 1
                case EDDP_SDT_IDX_DCP_03TYPE:       // ; SDT idx 4  0100    --> Idx 2
                case EDDP_SDT_IDX_DCP_03TLV:        // ; SDT idx 5  0101    --> Idx 2
                case EDDP_SDT_IDX_DCP_04TYPE:       // ; SDT idx 6  0110    --> Idx 3
                case EDDP_SDT_IDX_DCP_04TLV:        // ; SDT idx 7  0111    --> Idx 3
                case EDDP_SDT_IDX_DCP_05TYPE:       // ; SDT idx 8  1000    --> Idx 4
                case EDDP_SDT_IDX_DCP_05TLV:        // ; SDT idx 9  1001    --> Idx 4
                {
                    if (pNrtDDB->pNrtApiCtrlIfaceDcpFilter != LSA_NULL)
                    {
                        nIdx = (nIdx >> 1);     // convert to Index 0..4

                        if (nIdx < pNrtDDB->DcpFilterCnt)
                        {
                            nRet = (LSA_UINT32)pNrtDDB->pNrtApiCtrlIfaceDcpFilter;
                            nRet += nIdx * sizeof(EDDP_HAL_APICTRL_NRT_IF_DCP_FILTER_TYPE);
                            nRet -= (LSA_UINT32)&pNrtApiCtrlIfaceMgm->ApiCtrlRamSdt[0];
                        }
                    }
                }
                break;

                // ***** ARP Interface
                case EDDP_SDT_IDX_ARP_IP:
                {
                    nRet = ( (LSA_UINT32)&pNrtApiCtrlIfaceMgm->ApiCtrlRamArpIf[0] - (LSA_UINT32)&pNrtApiCtrlIfaceMgm->ApiCtrlRamSdt[0] );
                }
                break;

                // ***** DCP-HELLO 0..31
                default:
                {
                    if ((nIdx >= EDDP_SDT_IDX_DCP_HELLO_MIN) && (nIdx <= EDDP_SDT_IDX_DCP_HELLO_MAX))
                    {
                        if (pNrtDDB->pNrtApiCtrlIfaceDcpHelloFilter != LSA_NULL)
                        {
                            nIdx -= EDDP_SDT_IDX_DCP_HELLO_MIN;     // convert to Index 0..EDDP_SDT_IDX_DCP_HELLO_MAX

                            if (nIdx < (2*pNrtDDB->DcpHelloFilterCnt))
                            {
                                nRet = (LSA_UINT32)pNrtDDB->pNrtApiCtrlIfaceDcpHelloFilter;
                                nRet += (nIdx/2) * sizeof(EDDP_HAL_APICTRL_NRT_IF_DCPHELLO_FILTER_TYPE);
                                nRet -= (LSA_UINT32)&pNrtApiCtrlIfaceMgm->ApiCtrlRamSdt[0];
                                nRet += ((nIdx&1)*(EDDP_FILTER_DCPHELLO_STR_OFFS));   // if string index --> go behind length part and 2 byte padding --> need 32bit alignment !!!!
                            }
                        }
                    }
                }
                break;
            }
            break;
#endif
        default:
            EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] _EDDP_NRTFilterGetStringOffs(): FATAL -> Version(%d) of PNIP is unknown",
                EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            break;
    }

    if (nRet & 0x3)
    {
        EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--]: _EDDP_NRTFilterGetStringOffs(nIdx=%d): FATAL -> no 32bit Addr(0x%X) !!!",
            nIdx, nRet);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    return (nRet);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_NRTCancelTxSingleQueue                +*/
/*+  Input/Output          :    EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
/*+                             EDDP_LOCAL_HDB_NRT_PTR_TYPE pNRTHDB         +*/
/*+                             EDDP_NRT_TX_MGM_TYPE_PTR    pTxRqbQueue     +*/
/*+                             LSA_HANDLE_TYPE             Handle          +*/
/*+  Result                :    LSA_UINT32                                  +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                         +*/
/*+  pNRTHDB    : Pointer to specific NRT HDB                               +*/
/*+  pTxRqbQueue: Pointer to TX-RQB-Queue                                   +*/
/*+  Handle     : Filter for RQBs used by this Handle                       +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: utility function for _EDDP_NRTCancelTx()                  +*/
/*+               --> skip RQBs selected by Handle from TX-RQB-Queue        +*/
/*+                   and return the RQBs to the user                       +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_NRTCancelTxSingleQueue(
    EDDP_LOCAL_HDB_PTR_TYPE         pHDB,
    EDDP_LOCAL_HDB_NRT_PTR_TYPE     pNRTHDB,
    EDDP_NRT_TX_MGM_TYPE_PTR        pTxRqbQueue) //,
    //LSA_HANDLE_TYPE                 Handle)
{
    LSA_UINT32                  TxQueueCnt, Pos;
    EDD_UPPER_RQB_PTR_TYPE      pCurrRQB, pRQB;

    pCurrRQB    = pTxRqbQueue->TxReqQueue.pFirst;
    TxQueueCnt  = pTxRqbQueue->TxReqQueue.Count;

    for ( Pos=0; (Pos<TxQueueCnt) && (pCurrRQB!=LSA_NULL); Pos++ )
    {
        if (LSA_HOST_PTR_ARE_EQUAL(EDD_RQB_GET_HANDLE_LOWER(pCurrRQB), pHDB))
        {
            // ***** RQB matches
            pRQB        = pCurrRQB;
            pCurrRQB    = EDD_RQB_GET_NEXT_RQB_PTR(pRQB);   // keep next pointer in list

            // because list changed now
            EDDP_GlbRQBQueueRemoveFromQueue (&pTxRqbQueue->TxReqQueue, pRQB);
            // confirmation to user
            EDDP_GlbRequestFinish (pHDB, pRQB, EDD_STS_OK_CANCEL);
            EDDP_ASSERT(pNRTHDB->TxQueueReqCnt);
            pNRTHDB->TxQueueReqCnt--;
        }
        else
        {
            // ***** go on with next RQB
            pCurrRQB = EDD_RQB_GET_NEXT_RQB_PTR(pCurrRQB);
        }
    }

    return (EDD_STS_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_NRTCancelTx                           +*/
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
/*+-------------------------------------------------------------------------+*/
/*+  Description: utility function for EDDP_NRTCancel()                     +*/
/*+               --> skip RQBs selected by Handle from all TX-RQB-Queues   +*/
/*+                   and return the RQBs to the user                       +*/
/*+                   store the USER-RQB if cancel can not be finished now  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_NRTCancelTx(
    EDD_UPPER_RQB_PTR_TYPE          pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE         pHDB)
{
    LSA_UINT16                      TxQueueIdx;
    EDDP_LOCAL_HDB_NRT_PTR_TYPE     pNRTHDB = pHDB->pNRT;
    EDDP_LOCAL_DDB_NRT_PTR_TYPE     pNRTDDB = &pDDB->NRT;

    if (LSA_HOST_PTR_ARE_EQUAL(pNRTHDB, LSA_NULL))
    {
        return (EDD_STS_ERR_PARAM);
    }

    // ***** skip all RQBs for this channel (in the TX queues)
    if (pNRTHDB->TxQueueReqCnt)
    {
        for ( TxQueueIdx=0; TxQueueIdx < EDDP_NRT_TX_GRP_CNT; TxQueueIdx++ )
        {
            _EDDP_NRTCancelTxSingleQueue (pHDB, pNRTHDB, &pNRTDDB->TxQueue[TxQueueIdx]); //, pNRTHDB->Handle);
        }
    }

    // ***** test if RQBs are still active in the ASIC --> wait for completion of these RQBs in the ISR
    if (pNRTHDB->TxInProgCnt)
    {
        // ***** store information that 'cancel' is active in background
        pNRTHDB->CancelInProgress   = LSA_TRUE;
        pNRTHDB->pUserCancelRQB     = pRQB;
    }

    return (EDD_STS_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_NRTCancelRx                           +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB        +*/
/*+                             EDD_LOCAL_HDB_PTR_TYPE          pHDB        +*/
/*+                             EDD_UPPER_NRT_CANCEL_PTR_TYPE   pRQBCancel  +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                         +*/
/*+  pRQBCancel : Pointer to CancelRequest                                  +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: utility function for EDDP_NRTCancel()                     +*/
/*+               --> skip all RQBs selected by the Channel/Handle (pHDB)   +*/
/*+                   from RX-RQB-Queues and return the RQBs to the user    +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_NRTCancelRx(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE         pHDB,
    EDD_UPPER_NRT_CANCEL_PTR_TYPE   pRQBCancel)
{
    LSA_UINT32                      Pos, Count;
    EDD_UPPER_RQB_PTR_TYPE          pRQB;
    EDD_UPPER_NRT_RECV_PTR_TYPE     pRQBRcv;
    EDDP_LOCAL_HDB_NRT_PTR_TYPE     pNRTHDB = pHDB->pNRT;

    if (LSA_HOST_PTR_ARE_EQUAL(pNRTHDB, LSA_NULL))
    {
        return (EDD_STS_ERR_PARAM);
    }

    Count = pNRTHDB->RxUsrQueue.Count;

    for ( Pos=0; Pos < Count; Pos++ )
    {
        pRQB                = EDDP_GlbRQBQueueRemoveFromBegin (&pNRTHDB->RxUsrQueue);
        pRQBRcv             = (EDD_UPPER_NRT_RECV_PTR_TYPE) pRQB->pParam;
        pRQBRcv->RequestCnt = pNRTHDB->RxUsrQueue.Count;

        EDDP_GlbRequestFinish (pHDB, pRQB, EDD_STS_OK_CANCEL);
    }

    LSA_UNUSED_ARG(pRQBCancel); // handle unused parameter
    LSA_UNUSED_ARG(pDDB);       // handle unused parameter

    return (EDD_STS_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_NRTCancelRxById                       +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB        +*/
/*+                             EDD_LOCAL_HDB_PTR_TYPE          pHDB        +*/
/*+                             EDD_UPPER_NRT_CANCEL_PTR_TYPE   pRQBCancel  +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                         +*/
/*+  pRQBCancel : Pointer to CancelRequest                                  +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: utility function for EDDP_NRTCancel()                     +*/
/*+               --> skip all RQBs from Queue for selected Channel/Handle  +*/
/*+                   (pHDB) that matchs the ID and return RQBs to the user +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR _EDDP_NRTCancelRxById(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE         pHDB,
    EDD_UPPER_NRT_CANCEL_PTR_TYPE   pRQBCancel)
{
    LSA_UINT32                      Pos, Count;
    EDD_UPPER_RQB_PTR_TYPE          pRQB, pCurrRQB;
    EDD_UPPER_NRT_RECV_PTR_TYPE     pRQBRcv;
    EDDP_LOCAL_HDB_NRT_PTR_TYPE     pNRTHDB = pHDB->pNRT;

    if (LSA_HOST_PTR_ARE_EQUAL(pNRTHDB, LSA_NULL))
    {
        return (EDD_STS_ERR_PARAM);
    }

    Count       = pNRTHDB->RxUsrQueue.Count;
    pCurrRQB    = pNRTHDB->RxUsrQueue.pFirst;

    // ***** step through queue and search for RQBs with given RequestID
    for ( Pos=0; Pos<Count; Pos++ )
    {
        pRQBRcv = (EDD_UPPER_NRT_RECV_PTR_TYPE) pCurrRQB->pParam;

        if (pRQBRcv->RequestID == pRQBCancel->RequestID)
        {
            // ***** RQB matches
            pRQB        = pCurrRQB;
            pCurrRQB    = EDD_RQB_GET_NEXT_RQB_PTR(pRQB);   // keep next pointer in list

            // because list changed now
            EDDP_GlbRQBQueueRemoveFromQueue (&pNRTHDB->RxUsrQueue, pRQB);
            pRQBRcv->RequestCnt = pNRTHDB->RxUsrQueue.Count;
            // confirmation to user
            EDDP_GlbRequestFinish (pHDB, pRQB, EDD_STS_OK_CANCEL);
        }
        else
        {   // ***** go on with next RQB
            pCurrRQB = EDD_RQB_GET_NEXT_RQB_PTR(pCurrRQB);
        }
    }

    LSA_UNUSED_ARG(pDDB); // handle unused parameter

    return (EDD_STS_OK);
}


/*===========================================================================*/
/*                        public  functions                                  */
/*===========================================================================*/



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRTCancel                              +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDD_LOCAL_HDB_PTR_TYPE      pHDB            +*/
/*+                             EDD_UPPER_NRT_CANCEL_PTR_TYPE  pRQBCancel   +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                         +*/
/*+  pRQBCancel : Pointer to CancelRequest                                  +*/
/*+                                                                         +*/
/*+  Result     : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_ALIGN                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Cancels NRT-Requests                                      +*/
/*+               Finishs all rx/tx-requests depending on selected group.   +*/
/*+               Selected cancel group is in pRQBCancel->Mode              +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTCancel(
    EDD_UPPER_RQB_PTR_TYPE          pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE         pHDB,
    EDD_UPPER_NRT_CANCEL_PTR_TYPE   pRQBCancel)
{
    // pHDB->pNRT                   checked by function caller !
    // pRQB->pParam                 checked by function caller !
    // pHDB->pNRT->CancelInProgress checked by function caller !
    EDD_RSP     Response = EDD_STS_ERR_SEQUENCE;

    if (LSA_HOST_PTR_ARE_EQUAL(pHDB->pNRT->pUserCancelRQB, LSA_NULL))
    {
        switch (pRQBCancel->Mode)
        {
            //-------------------------------------------------------------
            case EDD_NRT_CANCEL_MODE_ALL:
            //-------------------------------------------------------------
            {
                Response = _EDDP_NRTCancelRx (pDDB, pHDB, pRQBCancel);

                if (Response == EDD_STS_OK)
                {
                    Response = _EDDP_NRTCancelTx (pRQB, pDDB, pHDB);
                }
            }
            break;

            //-------------------------------------------------------------
            case EDD_NRT_CANCEL_MODE_RX_ALL:
            //-------------------------------------------------------------
            {
                Response = _EDDP_NRTCancelRx (pDDB, pHDB, pRQBCancel);
            }
            break;

            //-------------------------------------------------------------
            case EDD_NRT_CANCEL_MODE_TX_ALL:
            //-------------------------------------------------------------
            {
                Response = _EDDP_NRTCancelTx (pRQB, pDDB, pHDB);
            }
            break;

            //-------------------------------------------------------------
            case EDD_NRT_CANCEL_MODE_RX_BY_ID:
            //-------------------------------------------------------------
            {
                Response = _EDDP_NRTCancelRxById (pDDB, pHDB, pRQBCancel);
            }
            break;

            //-------------------------------------------------------------
            default:
            //-------------------------------------------------------------
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_NRTCancel(pRQB=0x%X): ERROR -> unknown cancel mode(%d)!",
                    pRQB, pRQBCancel->Mode);
            }
            break;
        }

        // ***** test if RQB confirmation must be done internally
        if (Response == EDD_STS_OK)
        {
            // ***** Request is queued or ready for confirmation
            if (LSA_HOST_PTR_ARE_EQUAL(pHDB->pNRT->pUserCancelRQB, LSA_NULL))
            {
                // ***** Request is ready for confirmation
                EDDP_GlbRequestFinish (pHDB, pRQB, EDD_STS_OK);
            }
        }
    }

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRTDCPFilter                           +*/
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
/*+  Result                :    EDD_STS_OK                                  +*/
/*+                             EDD_STS_ERR_PARAM                           +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Handles EDD_SRV_NRT_SET_DCP_FILTER request.               +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTDCPFilter(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDDP_LOCAL_DDB_NRT_PTR_TYPE                 pNrtDDB;
    EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE  pNrtApiCtrlIfaceMgm;
    EDD_UPPER_NRT_SET_DCP_FILTER_PTR_TYPE       pNRTDcpSetFilter;
    LSA_UINT8                                   Idx, LogicalTlvCnt;
    LSA_UINT16                                  *pDcpData, DcpLen;
    EDD_RSP                                     Response;

    // ***** pHDB->pNRT tested by caller

    Response            = EDD_STS_OK;
    pNRTDcpSetFilter    = (EDD_UPPER_NRT_SET_DCP_FILTER_PTR_TYPE) pRQB->pParam;     // tested in EDDP_NRTRequest()
    pNrtDDB             = pHDB->pNRT->pNrtDDB;

    EDDP_NRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_NRTDCPFilter(Mode=%d, FilterTLVCnt=%d)",
        pHDB->Handle, pNRTDcpSetFilter->Mode, pNRTDcpSetFilter->FilterTLVCnt);

    pNrtApiCtrlIfaceMgm = (EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE)pNrtDDB->pNrtApiCtrlIfaceMgm;
    EDDP_ASSERT_NO_NULL_PTR(pNrtApiCtrlIfaceMgm);
    EDDP_ASSERT_NO_NULL_PTR(pNrtDDB->pNrtApiCtrlIfaceDcpFilter);

    Response = _EDDP_NRTDCPFilterCheckParam(pRQB,pDDB,pHDB);

    if(EDD_STS_OK == Response)
    {
        LogicalTlvCnt = pNrtDDB->DcpFilterCnt;      // nr of logical entries (IFace + PortCount)

        switch (pNRTDcpSetFilter->Mode)
        {
            /* ---------------------------- */
            /* Mode = Filter OFF            */
            /* ---------------------------- */
            case EDD_DCP_FILTER_OFF:
            {
                switch (EDDP_HALGetPnipRevision(pDDB))
                {
#if defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT) ||defined (EDDP_CFG_HW_PNIP_REV2_SUPPORT)
                        case EDDP_HAL_HW_PNIP_REV1:
                        case EDDP_HAL_HW_PNIP_REV2:
                        {
                            // ***** update of strings not necessary, because string compare will be disabled
                            // ***** one TLV RQB entry is represented with two HW entries ( 0:Type/Len, 1:Full string)
                            //       --> disable both descriptor entries
                            for ( Idx=0; Idx < (LogicalTlvCnt<<1); Idx++ )
                            {
                                // ***** Note: EDDP_HWCWriteSdtEntry() returns EDD_STS_OK always
                                EDDP_HWCWriteSdtEntry (pDDB, (LSA_UINT32)&pNrtApiCtrlIfaceMgm->ApiCtrlRamSdt[0], Idx+EDDP_SDT_IDX_DCP_MIN, PNIP_FILTER_STRING_DESCR_DEF_OFF);
                            }
                        }
                        break;
#endif
                        default:
                        {
                            EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_NRTDCPFilter(): FATAL -> Version(%d) of PNIP is unknown",
                                EDDP_HALGetPnipRevision(pDDB));
                            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                        }
                        break;
                        
                }

                EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_NRTDCPFilter(): set FilterTLVCnt(%d) OFF",
                    pHDB->Handle, pNrtDDB->FilterTLVCnt);

                pNrtDDB->FilterTLVCnt       = 0;                        // no TLV used by NRT
                pNrtDDB->FilterTLVHandle    = EDDP_CFG_MAX_CHANNELS;    // set handle invalid
            }
            break;

            /* ---------------------------- */
            /* Mode = Filter ON             */
            /* ---------------------------- */
            case EDD_DCP_FILTER_ON:
            {
                switch (EDDP_HALGetPnipRevision(pDDB))
                {
#if defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT) ||defined (EDDP_CFG_HW_PNIP_REV2_SUPPORT)
                        case EDDP_HAL_HW_PNIP_REV1:
                        case EDDP_HAL_HW_PNIP_REV2:
                        {
                            LSA_UINT32 StringCfgValue;
                            
                            // ***** enable entries til pNRTDcpSetFilter->FilterTLVCnt and disable remaining entries
                            // ***** one TLV RQB entry is represented with two HW entries (0:Type/Len, 1:Full string)
                            for ( Idx=0; Idx < (LogicalTlvCnt<<1); Idx+= 2)
                            {
                                if ( Idx < (pNRTDcpSetFilter->FilterTLVCnt << 1) )
                                {
                                    // ***** enable string pair if user data exist
                                    StringCfgValue  = 0;
                                    pDcpData        = (LSA_UINT16*) pNRTDcpSetFilter->FilterTLV[Idx>>1].pTLV;
            
                                    // ***** dcp data pointer is valid
                                    PNIP_FILTER_STRING_DESCR_SET_EN(StringCfgValue, (LSA_UINT32)PNIP_FILTER_STRING_DESCR_EN_CMP);
            
                                    #ifdef EDDP_CFG_BIG_ENDIAN
                                    DcpLen = *(pDcpData+1);
                                    #else
                                    DcpLen = EDDP_SWAP16(*(pDcpData+1));
                                    #endif
            
                                    DcpLen += 4;    // add [TypeHi, TypeLo, LenHi, LenLo]
            
                                    // ***** update short (TYPE) and complete DCP string
                                    PNIP_FILTER_STRING_DESCR_SET_OFFS(StringCfgValue, _EDDP_NRTFilterGetStringOffs(pHDB->pDDB, pNrtDDB, Idx+EDDP_SDT_IDX_DCP_MIN));
            
                                    if (PNIP_FILTER_STRING_DESCR_GET_OFFS(StringCfgValue))
                                    {
                                        // ***** valid result from _EDDP_NRTFilterGetStringOffs()
                                        // ***** enable full part at first --> TYPE, LEN, STRING
                                        PNIP_FILTER_STRING_DESCR_SET_LEN(StringCfgValue,  DcpLen);
                                        // ***** Note: EDDP_HWCWriteSdtEntry() returns EDD_STS_OK always
                                        EDDP_HWCUpdateSdtString( pDDB,
                                                                 (LSA_UINT32)&pNrtApiCtrlIfaceMgm->ApiCtrlRamSdt[0],
                                                                 (Idx+1) + EDDP_SDT_IDX_DCP_MIN,
                                                                 StringCfgValue,
                                                                 ((LSA_UINT32)pNrtDDB->pNrtApiCtrlIfaceDcpFilter) + ((Idx>>1)*EDDP_FILTER_DCP_SIZE),
                                                                 (LSA_UINT8*)pDcpData,
                                                                 DcpLen);
                                        // ***** enable short part now --> TYPE  ==> first 2 bytes of DCP data shared with complete DCP string
                                        PNIP_FILTER_STRING_DESCR_SET_LEN(StringCfgValue,  2);
                                        // ***** Note: EDDP_HWCWriteSdtEntry() returns EDD_STS_OK always
                                        EDDP_HWCWriteSdtEntry (pDDB, (LSA_UINT32)&pNrtApiCtrlIfaceMgm->ApiCtrlRamSdt[0], Idx+EDDP_SDT_IDX_DCP_MIN, StringCfgValue);
                                    }
                                }
                                else
                                {
                                    // ***** Note: EDDP_HWCWriteSdtEntry() returns EDD_STS_OK always
                                    // ***** disable short entry (TYPE only)
                                    EDDP_HWCWriteSdtEntry (pDDB, (LSA_UINT32)&pNrtApiCtrlIfaceMgm->ApiCtrlRamSdt[0], Idx+EDDP_SDT_IDX_DCP_MIN,  PNIP_FILTER_STRING_DESCR_DEF_OFF);
                                    // ***** disable complete DCP string
                                    EDDP_HWCWriteSdtEntry (pDDB, (LSA_UINT32)&pNrtApiCtrlIfaceMgm->ApiCtrlRamSdt[0], (Idx+1)+EDDP_SDT_IDX_DCP_MIN,  PNIP_FILTER_STRING_DESCR_DEF_OFF);
                                }
                            } // end of for ( Idx=0; ... )
                        }
                        break;
#endif
                        default:
                        {
                            EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_NRTDCPFilter(): FATAL -> Version(%d) of PNIP is unknown",
                                EDDP_HALGetPnipRevision(pDDB));
                            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                        }
                        break;
                }

                if (Response == EDD_STS_OK)
                {
                    pNrtDDB->FilterTLVCnt       = pNRTDcpSetFilter->FilterTLVCnt;   // new NRT count
                    pNrtDDB->FilterTLVHandle    = pHDB->Handle;                     // store this handle

                    EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_NRTDCPFilter(): set FilterTLVCnt(%d) ON",
                        pHDB->Handle, pNrtDDB->FilterTLVCnt);
                }
            }
            break;

            /* ---------------------------- */
            /* unknown Mode                 */
            /* ---------------------------- */
            default:
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_NRTDCPFilter(): ERROR -> unknown RQB-Mode(%d)!",
                    pHDB->Handle, pNRTDcpSetFilter->Mode);
            }
            break;
        }
    }

    EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_NRTDCPFilter(): Response=0x%X", Response);

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRTDCPHelloFilter                      +*/
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
/*+  Result                :    EDD_STS_OK                                  +*/
/*+                             EDD_STS_ERR_PARAM                           +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Handles EDD_SRV_NRT_SET_DCP_HELLO_FILTER request.         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTDCPHelloFilter(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDDP_LOCAL_DDB_NRT_PTR_TYPE                 pNrtDDB;
    EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE  pNrtApiCtrlIfaceMgm;
    EDD_UPPER_NRT_SET_DCP_HELLO_FILTER_PTR_TYPE pNRTDcpHelloSetFilter;
    EDDP_NRT_DCP_HELLO_FILTER_MGM_PTR           pDcpHelloFilter;
    LSA_UINT32                                  LenCfgValue, StringCfgValue, i;
    LSA_BOOL                                    bFound;
    LSA_UINT8                                   Idx;
    LSA_UINT16                                  LenVal;
    LSA_UINT8                                   LenValAr[EDDP_FILTER_DCPHELLO_LEN_SIZE];
    EDD_RSP                                     Response;

    // ***** pHDB->pNRT tested by caller

    // Note: The DcpHello filter will be set by the OHA (higher LSA layer) but its channel
    //       is not enabled for DcpHello!

    Response                = EDD_STS_OK;
    pNrtDDB                 = pHDB->pNRT->pNrtDDB;
    pNRTDcpHelloSetFilter   = (EDD_UPPER_NRT_SET_DCP_HELLO_FILTER_PTR_TYPE) pRQB->pParam;   // tested in EDDP_NRTRequest()

    EDDP_NRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_NRTDCPHelloFilter(Mode=%d, UserID=%d, NameOfStationLen=%d)",
        pHDB->Handle, pNRTDcpHelloSetFilter->Mode, pNRTDcpHelloSetFilter->UserID, pNRTDcpHelloSetFilter->NameOfStationLen);

    if (pNrtDDB)
    {
        pNrtApiCtrlIfaceMgm = (EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE)pNrtDDB->pNrtApiCtrlIfaceMgm;
        EDDP_ASSERT_NO_NULL_PTR(pNrtApiCtrlIfaceMgm);

        if (  pNrtDDB->pNrtApiCtrlIfaceDcpHelloFilter == LSA_NULL 
           && EDDP_HAL_HW_PNIP_REV3 != EDDP_HALGetPnipRevision(pDDB)
           )
        {
            EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_RESOURCE);
            return (EDD_STS_ERR_RESOURCE);
        }

        pDcpHelloFilter = &pNrtDDB->pDCPHelloFilter[0];

        switch (pNRTDcpHelloSetFilter->Mode)
        {
            /* ---------------------------- */
            /* Mode = all Filter OFF        */
            /* ---------------------------- */
            // ***** update of strings not necessary, because string compare will be disabled
            case EDD_DCP_HELLO_FILTER_OFF_ALL:
            {
                EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_NRTDCPHelloFilter(): Turn of all filters", pHDB->Handle);

                switch (EDDP_HALGetPnipRevision(pDDB))
                {
#if defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT) ||defined (EDDP_CFG_HW_PNIP_REV2_SUPPORT)
                        case EDDP_HAL_HW_PNIP_REV1:
                        case EDDP_HAL_HW_PNIP_REV2:
                        {
                            for ( Idx=0; Idx < pNrtDDB->DcpHelloFilterCnt; Idx++)
                            {
                                // ***** disable all DCP-HELLO: len & string fields available in HAL-Layout
                                EDDP_HWCWriteSdtEntry (pDDB, (LSA_UINT32)&pNrtApiCtrlIfaceMgm->ApiCtrlRamSdt[0], 2*Idx+EDDP_SDT_IDX_DCP_HELLO_MIN,   PNIP_FILTER_STRING_DESCR_DEF_OFF);
                                EDDP_HWCWriteSdtEntry (pDDB, (LSA_UINT32)&pNrtApiCtrlIfaceMgm->ApiCtrlRamSdt[0], 2*Idx+EDDP_SDT_IDX_DCP_HELLO_MIN+1, PNIP_FILTER_STRING_DESCR_DEF_OFF);

                                if (Idx < pNrtDDB->DcpHelloFilterCnt)
                                {
                                    pDcpHelloFilter[Idx].InUse = LSA_FALSE;
                                }
                            }
                        }
                        break;
#endif
                        default:
                        {
                            EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_NRTDCPHelloFilter(): FATAL -> Version(%d) of PNIP is unknown",
                                EDDP_HALGetPnipRevision(pDDB));
                            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                        }
                        break;
                        
                }
            }
            break;

            /* ---------------------------- */
            /* Mode = one Filter OFF        */
            /* ---------------------------- */
            // ***** update of string not necessary, because string compare will be disabled
            case EDD_DCP_HELLO_FILTER_OFF:
            {
                EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_NRTDCPHelloFilter(): Turn of all filters with UserID(%d)",
                    pHDB->Handle, pNRTDcpHelloSetFilter->UserID);

                bFound = LSA_FALSE;

                switch(EDDP_HALGetPnipRevision(pDDB))
                {
#if defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT) ||defined (EDDP_CFG_HW_PNIP_REV2_SUPPORT)
                    case EDDP_HAL_HW_PNIP_REV1:
                    case EDDP_HAL_HW_PNIP_REV2:
                    {
                        for ( Idx=0; Idx < pNrtDDB->DcpHelloFilterCnt; Idx++ )
                        {
                            // ***** search over all DCP-HELLO strings available in HAL-Layout
                            if (   (pDcpHelloFilter[Idx].InUse)
                                && (pNRTDcpHelloSetFilter->UserID == pDcpHelloFilter[Idx].UserID)
                                && (pDcpHelloFilter[Idx].Handle == pHDB->Handle)
                               )
                            {
                                pDcpHelloFilter[Idx].InUse  = LSA_FALSE;
                                bFound                      = LSA_TRUE;

                                EDDP_HWCWriteSdtEntry (pDDB, (LSA_UINT32)&pNrtApiCtrlIfaceMgm->ApiCtrlRamSdt[0], 2*Idx+EDDP_SDT_IDX_DCP_HELLO_MIN,   PNIP_FILTER_STRING_DESCR_DEF_OFF);
                                EDDP_HWCWriteSdtEntry (pDDB, (LSA_UINT32)&pNrtApiCtrlIfaceMgm->ApiCtrlRamSdt[0], 2*Idx+EDDP_SDT_IDX_DCP_HELLO_MIN+1, PNIP_FILTER_STRING_DESCR_DEF_OFF);
                            }
                        }
                    }
                    break;
#endif
                    default:
                    {
                        EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_NRTDCPHelloFilter(): FATAL -> Version(%d) of PNIP is unknown",
                            EDDP_HALGetPnipRevision(pDDB));
                        EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                    }
                    break;
                }

                if (!bFound)
                {
                    // ***** no Filter found
                    Response = EDD_STS_ERR_RESOURCE;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_NRTDCPHelloFilter(): ERROR -> No filter found with UserID(%d)",
                        pHDB->Handle, pNRTDcpHelloSetFilter->UserID);
                }
            }
            break;

            /* ---------------------------- */
            /* Mode = Filter ON             */
            /* ---------------------------- */
            // ***** update of string is necessary, because string compare will be enabled
            case EDD_DCP_HELLO_FILTER_ON:
            {
                EDDP_NRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_NRTDCPHelloFilter(): Turn on filters with UserID(%d), NameOfStationLen(%d)",
                    pHDB->Handle, pNRTDcpHelloSetFilter->UserID, pNRTDcpHelloSetFilter->NameOfStationLen);

                bFound = LSA_FALSE;
                
                if(LSA_HOST_PTR_ARE_EQUAL(LSA_NULL, pNRTDcpHelloSetFilter->pNameOfStation))
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_NRTDCPHelloFilter(): ERROR -> pNameOfStation is NULL",
                        pHDB->Handle);
                    break;
                }

                switch(EDDP_HALGetPnipRevision(pDDB))
                {
#if defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT) ||defined (EDDP_CFG_HW_PNIP_REV2_SUPPORT)
                    case EDDP_HAL_HW_PNIP_REV1:
                    case EDDP_HAL_HW_PNIP_REV2:
                    {
                        LSA_UINT32 ApiCtrlAddr;
                        
                        for ( Idx=0; Idx < pNrtDDB->DcpHelloFilterCnt; Idx++ )
                        {
                            // ***** search over all DCP-HELLO strings available in HAL-Layout
                            if (!(pDcpHelloFilter[Idx].InUse))
                            {
                                StringCfgValue = LenCfgValue = 0;
        
                                if ( (pNRTDcpHelloSetFilter->NameOfStationLen) && (pNRTDcpHelloSetFilter->NameOfStationLen <= EDDP_FILTER_DCPHELLO_STR_SIZE) )
                                {
                                    // ***** string and length valid
                                    PNIP_FILTER_STRING_DESCR_SET_EN(LenCfgValue,    PNIP_FILTER_STRING_DESCR_EN_CMP);
                                    PNIP_FILTER_STRING_DESCR_SET_LEN(LenCfgValue,    (EDDP_FILTER_DCPHELLO_LEN_SIZE));
                                    PNIP_FILTER_STRING_DESCR_SET_OFFS(LenCfgValue,   _EDDP_NRTFilterGetStringOffs(pHDB->pDDB, pNrtDDB,  (2*Idx+EDDP_SDT_IDX_DCP_HELLO_MIN)));
        
                                    PNIP_FILTER_STRING_DESCR_SET_EN(StringCfgValue,    PNIP_FILTER_STRING_DESCR_EN_CMP);
                                    PNIP_FILTER_STRING_DESCR_SET_LEN(StringCfgValue,    pNRTDcpHelloSetFilter->NameOfStationLen);
                                    PNIP_FILTER_STRING_DESCR_SET_OFFS(StringCfgValue,   _EDDP_NRTFilterGetStringOffs(pHDB->pDDB, pNrtDDB,  (2*Idx+EDDP_SDT_IDX_DCP_HELLO_MIN+1)));
        
                                    if (PNIP_FILTER_STRING_DESCR_GET_OFFS(StringCfgValue))
                                    {
                                        // ***** valid result from _EDDP_NRTFilterGetStringOffs()
                                        ApiCtrlAddr = (LSA_UINT32)pNrtDDB->pNrtApiCtrlIfaceDcpHelloFilter + Idx*EDDP_FILTER_DCPHELLO_FULL_SIZE;
                                        EDDP_HWCUpdateSdtString( pDDB,   (LSA_UINT32)&pNrtApiCtrlIfaceMgm->ApiCtrlRamSdt[0], (2*Idx + EDDP_SDT_IDX_DCP_HELLO_MIN + 1), StringCfgValue,
                                                                (ApiCtrlAddr + EDDP_FILTER_DCPHELLO_STR_OFFS), (LSA_UINT8*)&pNRTDcpHelloSetFilter->pNameOfStation[0], (LSA_UINT16)pNRTDcpHelloSetFilter->NameOfStationLen);
        
                                        // add 2 byte to be conform with protocol
                                        LenVal      = (LSA_UINT16)(pNRTDcpHelloSetFilter->NameOfStationLen+EDDP_FILTER_DCPHELLO_LEN_SIZE);
                                        LenValAr[0] = (LSA_UINT8)(LenVal >> 8);
                                        LenValAr[1] = (LSA_UINT8)(LenVal & 0xFF);
        
                                        EDDP_HWCUpdateSdtString( pDDB,   (LSA_UINT32)&pNrtApiCtrlIfaceMgm->ApiCtrlRamSdt[0], (2*Idx + EDDP_SDT_IDX_DCP_HELLO_MIN), LenCfgValue,
                                                                ApiCtrlAddr, &LenValAr[0], EDDP_FILTER_DCPHELLO_LEN_SIZE);
        
                                        // ***** copy values from RQB into NRT array
                                        pDcpHelloFilter[Idx].InUse              = LSA_TRUE;
                                        pDcpHelloFilter[Idx].Handle             = pHDB->Handle;
                                        pDcpHelloFilter[Idx].UserID             = pNRTDcpHelloSetFilter->UserID;
                                        pDcpHelloFilter[Idx].StationNameLen     = pNRTDcpHelloSetFilter->NameOfStationLen;
        
                                        bFound  = LSA_TRUE;
                                        i       = (LSA_UINT8) pNRTDcpHelloSetFilter->NameOfStationLen;   // max 240
        
                                        while (i)
                                        {
                                            // ***** copy station name
                                            i--;
                                            pDcpHelloFilter[Idx].StationName[i] = pNRTDcpHelloSetFilter->pNameOfStation[i];
                                        }
                                        break;
                                    }
                                }
                                else
                                {
                                    bFound  = LSA_TRUE;
                                    Response = EDD_STS_ERR_PARAM;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_NRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_NRTDCPHelloFilter(): ERROR -> NameOfStationLen(%d) is out of range (1..%d)",
                                        pHDB->Handle, pNRTDcpHelloSetFilter->NameOfStationLen, EDDP_FILTER_DCPHELLO_STR_SIZE);
                                    break;
                                }
                            }
                        }
                    }
                    break;
#endif
                    default:
                    {
                        EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_NRTDCPHelloFilter(): FATAL -> Version(%d) of PNIP is unknown",
                            EDDP_HALGetPnipRevision(pDDB));
                        EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                    }
                    break;
                }

                if (!bFound)
                {
                    // ***** no free Filter found
                    Response = EDD_STS_ERR_RESOURCE;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_NRTDCPHelloFilter(): ERROR -> No free filter found for UserID(%d)",
                        pHDB->Handle, pNRTDcpHelloSetFilter->UserID);
                }
            }
            break;
            
            default:
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_NRTDCPHelloFilter(): ERROR -> Invalid Mode(%d)",
                    pHDB->Handle, pNRTDcpHelloSetFilter->Mode);
            }
            break;
        }
    }
    else
    {
        EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:%2X]: EDDP_NRTDCPHelloFilter(): FATAL -> pHDB->pNRT->pNrtDDB is NULL !!!",
            pHDB->Handle);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_NRTDCPHelloFilter(): Response=0x%X", Response);

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_NRTARPFilter                           +*/
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
/*+  Result                :    EDD_STS_OK                                  +*/
/*+                             EDD_STS_ERR_PARAM                           +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Handles EDD_SRV_NRT_SET_ARP_FILTER request.               +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_NRTARPFilter(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDDP_LOCAL_DDB_NRT_PTR_TYPE                 pNrtDDB;
    EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE  pNrtApiCtrlIfaceMgm;
    EDD_UPPER_NRT_SET_ARP_FILTER_PTR_TYPE       pNRTARPSetFilter;
    LSA_UINT32                                  StringCfgValue;

    // ***** pHDB->pNRT tested by caller

    // Note: The ARP filter will be set by the OHA (higher LSA layer) but its channel
    //       is not enabled for ARP!

    pNRTARPSetFilter = (EDD_UPPER_NRT_SET_ARP_FILTER_PTR_TYPE) pRQB->pParam;    // tested in EDDP_NRTRequest()

    EDDP_NRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_NRTARPFilter(Mode=%d, UserIndex=%d)",
        pHDB->Handle, pNRTARPSetFilter->Mode, pNRTARPSetFilter->UserIndex);

    pNrtDDB = pHDB->pNRT->pNrtDDB;

    if (pNrtDDB)
    {
        pNrtApiCtrlIfaceMgm = (EDDP_HAL_LOCAL_APICTRL_NRT_IF_MGM_PTR_TYPE)pNrtDDB->pNrtApiCtrlIfaceMgm;
        EDDP_ASSERT_NO_NULL_PTR(pNrtApiCtrlIfaceMgm);
        
        switch(pNRTARPSetFilter->Mode)
        {
            case EDD_ARP_FILTER_OFF:
                switch (EDDP_HALGetPnipRevision(pDDB))
                {
#if defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT) ||defined (EDDP_CFG_HW_PNIP_REV2_SUPPORT)
                    case EDDP_HAL_HW_PNIP_REV1:
                    case EDDP_HAL_HW_PNIP_REV2:
                        // ***** update of string not necessary, because string compare will be set to always matching (with lenght 0)
                        return ( EDDP_HWCWriteSdtEntry(pDDB, (LSA_UINT32)&pNrtApiCtrlIfaceMgm->ApiCtrlRamSdt[0], EDDP_SDT_IDX_ARP_IP,  PNIP_FILTER_STRING_DESCR_DEF_ON) );
#endif
                    default:
                    {
                        EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_NRTARPFilter(): FATAL -> Version(%d) of PNIP is unknown",
                            EDDP_HALGetPnipRevision(pDDB));
                        EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                    }
                    break;
                }
            case EDD_ARP_FILTER_ON:
                EDDP_NRT_TRACE_01(pDDB->TraceIdx,LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_NRTARPFilter: Turn ON ARP filters",
                    pHDB->Handle);
                if (pNRTARPSetFilter->UserIndex >= EDDP_MAX_ARP_FILTER ) 
                {
                    EDDP_NRT_TRACE_02(pDDB->TraceIdx,LSA_TRACE_LEVEL_ERROR,"[H:%2X] EDDP_NRTARPFilter: Invalid UserIndex (%d)",
                        pHDB->Handle,
                        pNRTARPSetFilter->UserIndex);
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
                    return EDD_STS_ERR_PARAM;
                }
                else
                {
                    // ***** update of string is necessary, because string compare will be enabled
                    StringCfgValue = 0;
                    PNIP_FILTER_STRING_DESCR_SET_EN(StringCfgValue,  PNIP_FILTER_STRING_DESCR_EN_CMP);
                    PNIP_FILTER_STRING_DESCR_SET_LEN(StringCfgValue,    EDD_IP_ADDR_SIZE);
                    switch (EDDP_HALGetPnipRevision(pDDB))
                    {
#if defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT) ||defined (EDDP_CFG_HW_PNIP_REV2_SUPPORT)
                        case EDDP_HAL_HW_PNIP_REV1:
                        case EDDP_HAL_HW_PNIP_REV2:
                            PNIP_FILTER_STRING_DESCR_SET_OFFS(StringCfgValue,   _EDDP_NRTFilterGetStringOffs(pHDB->pDDB, pNrtDDB,  EDDP_SDT_IDX_ARP_IP));

                            if( !(PNIP_FILTER_STRING_DESCR_GET_OFFS(StringCfgValue)) )
                            {
                                // ***** error from _EDDP_NRTFilterGetStringOffs()
                                return (EDD_STS_ERR_PARAM);
                            }
                            return EDDP_HWCUpdateSdtString( pDDB,
                                (LSA_UINT32)&pNrtApiCtrlIfaceMgm->ApiCtrlRamSdt[0],
                                EDDP_SDT_IDX_ARP_IP,
                                StringCfgValue,
                                (LSA_UINT32)&pNrtApiCtrlIfaceMgm->ApiCtrlRamArpIf[0],
                                (LSA_UINT8*)&pNRTARPSetFilter->IPAddr,
                                EDD_IP_ADDR_SIZE);
#endif
                        default:
                            EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_NRTARPFilter(): FATAL -> Version(%d) of PNIP is unknown",
                                EDDP_HALGetPnipRevision(pDDB));
                            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                            break;
                    }
                }
            default:
                EDDP_NRT_TRACE_00(pDDB->TraceIdx,LSA_TRACE_LEVEL_ERROR,"EDDP_NRTARPFilter: Invalid parameter");
                return EDD_STS_ERR_PARAM;
        }

//        if (pNRTARPSetFilter->Mode == EDD_ARP_FILTER_OFF)
//        {
//            switch (EDDP_HALGetPnipRevision(pDDB))
//            {
//#if defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT) ||defined (EDDP_CFG_HW_PNIP_REV2_SUPPORT)
//                case EDDP_HAL_HW_PNIP_REV1:
//                case EDDP_HAL_HW_PNIP_REV2:
//                    // ***** update of string not necessary, because string compare will be set to always matching (with lenght 0)
//                    return ( EDDP_HWCWriteSdtEntry(pDDB, (LSA_UINT32)&pNrtApiCtrlIfaceMgm->ApiCtrlRamSdt[0], EDDP_SDT_IDX_ARP_IP,  PNIP_FILTER_STRING_DESCR_DEF_ON) );
//                    break;
//#endif
//                default:
//                {
//                    EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_NRTARPFilter(): FATAL -> Version(%d) of PNIP is unknown",
//                        EDDP_HALGetPnipRevision(pDDB));
//                    EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
//                }
//                break;
//            }
//        }
//        else if (pNRTARPSetFilter->FilterCnt != 1)
//        {
//            EDDP_NRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_NRTARPFilter(): ERROR -> FilterCnt(%d) is not 1", 
//                pHDB->Handle, pNRTARPSetFilter->FilterCnt);
//            EDDP_DBG_SET_DETAIL_ERR(pDDB, EDD_STS_ERR_PARAM);
//            return (EDD_STS_ERR_PARAM);
//        }
//        else
//        {
//            // ***** update of string is necessary, because string compare will be enabled
//            StringCfgValue = 0;
//            PNIP_FILTER_STRING_DESCR_SET_EN(StringCfgValue,  (LSA_UINT32) PNIP_FILTER_STRING_DESCR_EN_CMP);
//            PNIP_FILTER_STRING_DESCR_SET_LEN(StringCfgValue,    EDD_IP_ADDR_SIZE);
//            switch (EDDP_HALGetPnipRevision(pDDB))
//            {
//#if defined(EDDP_CFG_HW_PNIP_REV1_SUPPORT) ||defined (EDDP_CFG_HW_PNIP_REV2_SUPPORT)
//                    case EDDP_HAL_HW_PNIP_REV1:
//                    case EDDP_HAL_HW_PNIP_REV2:
//                        PNIP_FILTER_STRING_DESCR_SET_OFFS(StringCfgValue,   _EDDP_NRTFilterGetStringOffs(pHDB->pDDB, pNrtDDB,  EDDP_SDT_IDX_ARP_IP));
//
//                        if( !(PNIP_FILTER_STRING_DESCR_GET_OFFS(StringCfgValue)) )
//                        {
//                            // ***** error from _EDDP_NRTFilterGetStringOffs()
//                            return (EDD_STS_ERR_PARAM);
//                        }
//                        return EDDP_HWCUpdateSdtString( pDDB,
//                            (LSA_UINT32)&pNrtApiCtrlIfaceMgm->ApiCtrlRamSdt[0],
//                            EDDP_SDT_IDX_ARP_IP,
//                            StringCfgValue,
//                            (LSA_UINT32)&pNrtApiCtrlIfaceMgm->ApiCtrlRamArpIf[0],
//                            (LSA_UINT8*)&pNRTARPSetFilter->IPAddr[0],
//                            EDD_IP_ADDR_SIZE);
//                        break;
//#endif
//                    default:
//                    {
//                        EDDP_NRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_NRTARPFilter(): FATAL -> Version(%d) of PNIP is unknown",
//                            EDDP_HALGetPnipRevision(pDDB));
//                        EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
//                    }
//                    break;
//            }
//        }
    }

    return (EDD_STS_ERR_PARAM);
}
