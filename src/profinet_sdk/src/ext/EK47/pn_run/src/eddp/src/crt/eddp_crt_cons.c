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
/*  F i l e               &F: eddp_crt_cons.c                           :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  CRT (Cycle Real Time) for EDDP  -> Consumer                              */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who         What                                             */
#ifdef EDDP_MESSAGE
/*  22.10.08    Sergeev     Erstellung                                       */
#endif
/*****************************************************************************/


/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/

#define LTRC_ACT_MODUL_ID   22
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_CRT_CONS */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"           /* eddp headerfiles */
#include "eddp_int.h"           /* internal header  */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    _EDDP_CRTCountActiveXxxConsumer             +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE    pDDB             +*/
/*+                             LSA_UINT8                 ConsumerFlagMask  +*/
/*+                                                                         +*/
/*+  Result                :    LSA_UINT32                                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB             : Pointer to DeviceDescriptionBlock                   +*/
/*+  ConsumerFlagMask : 0 or specific property, see: EDDP_CONS_FLAG_IS_XXX  +*/
/*+                                                                         +*/
/*+  Results    : Internal utility function. Counting all active Consumer   +*/
/*+               with given property for given pDDB                        +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT32 EDDP_LOCAL_FCT_ATTR _EDDP_CRTCountActiveXxxConsumer(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB, 
    LSA_UINT8                   ConsumerFlagMask)
{
    LSA_UINT32                      Idx, End, ActiveCnt;
    LSA_UINT16                      CurrIdx, MaxCnt;
    EDDP_CRT_CONSUMER_PTR_TYPE      pCons;

    ActiveCnt   = 0;
    End         = pDDB->CRT.Cons.ConsumerUsedCnt;
    MaxCnt      = pDDB->CRT.CfgParam.ConsumerMaxNumber;
    pCons       = pDDB->CRT.Cons.pConsEntry;
    CurrIdx     = pDDB->CRT.Cons.ConsumerUsedList.BeginIdx;

    for ( Idx=0; Idx < End; Idx++ )
    {
        if (CurrIdx > MaxCnt)
        {   // ***** EDDP_CRT_CHAIN_IDX_END or failure
            break;
        }

        if (pCons[CurrIdx].State == EDDP_CRT_STATE_HW_ACTIVE)
        {   // ***** state marked for active ACW
            if (ConsumerFlagMask)
            {   // ***** counting all active consumer with given property
                if (pCons[CurrIdx].ConsumerFlags & ConsumerFlagMask)
                {
                    ActiveCnt++;
                }
            }
            else
            {   // ***** counting still all active consumer
                ActiveCnt++;
            }
        }

        CurrIdx = pCons[CurrIdx].UsedFreeLink.Next;
    }

    return (ActiveCnt);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTConsumerSetCtRwb                    +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB        +*/
/*+                             EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer   +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pConsumer  : pointer to Consumer (parameters)                          +*/
/*+                                                                         +*/
/*+  Results    : -                                                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerSetCtRwb(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer)
{
    EDDP_ASSERT_NO_NULL_PTR(pConsumer);

    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] EDDP_CRTConsumerSetCtRwb(): ConsumerID=%d, FrameID=0x%X, CT-OutDataStart=0x%X", 
        pConsumer->ConsumerId, pConsumer->Param.FrameID, pConsumer->Param.CutThrough_OutDataStart);

    /* PNIP_REG_CPM_OUTDATASTART was replaced by PNIP_R2_REG_CPM_OUTDATASTART, there is no R3 type of this define*/
    EDDP_HAL_REG32_WRITE(pDDB, PNIP_R2_REG_CPM_OUTDATASTART, pConsumer->Param.CutThrough_OutDataStart);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTConsumerClrCtRwb                    +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             LSA_UINT16                  ConsumerID      +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  ConsumerID : Consumer to be removed                                    +*/
/*+                                                                         +*/
/*+  Results    : -                                                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerClrCtRwb(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ConsumerID)
{
    EDDP_ASSERT_FALSE(ConsumerID >= pDDB->CRT.CfgParam.ConsumerMaxNumber);

    LSA_UNUSED_ARG(ConsumerID);

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] EDDP_CRTConsumerClrCtRwb(ConsumerID=%d): FrameID=0x%X", 
        ConsumerID, pDDB->CRT.Cons.pConsEntry[ConsumerID].Param.FrameID);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTCountActiveDP2Consumer              +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                                                                         +*/
/*+  Result                :    LSA_UINT32                                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+                                                                         +*/
/*+  Results    : Counting all active RTC3-Consumer-ACWs for given pDDB     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_CRTCountActiveDP2Consumer(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    // ***** count only RTC3 consumer
    return (_EDDP_CRTCountActiveXxxConsumer(pDDB, EDDP_CONS_FLAG_IS_DP2ACW));
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTCountAllActiveConsumer              +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB        +*/
/*+                                                                         +*/
/*+  Result                :    LSA_UINT32                                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+                                                                         +*/
/*+  Results    : Counting all active Consumer-ACWs for given pDDB          +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_CRTCountAllActiveConsumer(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    // ***** count all active consumer
    return (_EDDP_CRTCountActiveXxxConsumer(pDDB, 0));
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTConsumerAdd                         +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE          pRQB        +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE         pDDB        +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE         pHDB        +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                         +*/
/*+                                                                         +*/
/*+  EDD_RQB_CSRT_CONSUMER_ADD_TYPE                                         +*/
/*+                                                                         +*/
/*+  FrameID                 : FrameID for Provider.                        +*/
/*+  SrcMAC                  : Source MAC address (only XRT)                +*/
/*+  SrcIP                   : Source IP (only xRTOverUDP)                  +*/
/*+  DataLen                 : DataLen of I/O-Data in frame.                +*/
/*+  CycleReductionRatio     : spezifies CycleTime based on CycleBaseFactor +*/
/*+                            (1,2,4,8,16,32,64,128..)                     +*/
/*+  CyclePhase              : spezifies Phase within cycle.                +*/
/*+  WatchdogFactor          : spezifies watchog time                       +*/
/*+  DataHoldFactor          : spezifies datahold time                      +*/
/*+  UserID                  : UserID for Event-indications.                +*/
/*+  ConsumerID              : returned ConsumerID.                         +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_SEQUENCE                                      +*/
/*+               EDD_STS_ERR_NO_LOCAL_IP                                   +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Request to add a new consumer (RT and RTOverUDP)          +*/
/*+                                                                         +*/
/*+               Check consumer parameter (FrameID, DataLen, ...)          +*/
/*+                                                                         +*/
/*+               Check if free consumer present. If not we                 +*/
/*+               return EDD_STS_ERR_RESOURCE.                              +*/
/*+                                                                         +*/
/*+               Add this new consumer to CRT management and chain the     +*/
/*+               consumer to "used-chain-list"                             +*/
/*+                                                                         +*/
/*+               Call command interface                                    +*/
/*+                                                                         +*/
/*+               If command is finished the function                       +*/
/*+               EDDP_CRTCmdDoneCbfCpm() is called                         +*/
/*+               Set consumer State to INACTIVE                            +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerAdd(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP                                     Response;
    EDD_UPPER_CSRT_CONSUMER_ADD_PTR_TYPE        pRqbParam;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pHDB);
#endif

    pRqbParam = (EDD_UPPER_CSRT_CONSUMER_ADD_PTR_TYPE) pRQB->pParam;

    EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_CRTConsumerAdd(pRQB=0x%X, FrameID=0x%X): ConsumerUsedCnt=%d", 
        pHDB->Handle, pRQB, pRqbParam->FrameID, pDDB->CRT.Cons.ConsumerUsedCnt);

    // ***** check consumer parameter (FrameID, DataLen, ...)
    Response = EDDP_CRTCheckConsumerAddParam (pDDB, pRqbParam);

    if (Response == EDD_STS_OK)
    {   // ***** add consumer to CRT management (into SW only, HW initialization for the ACW later!!!)
        Response = EDDP_CRTConsumerAddToCRTMngm (pDDB, pRqbParam);
    }

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] OUT: EDDP_CRTConsumerAdd(): ConsumerUsedCnt=%d, Response=0x%X", 
        pDDB->CRT.Cons.ConsumerUsedCnt, Response);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTConsumerRemove                      +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE          pRQB        +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE         pDDB        +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE         pHDB        +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                         +*/
/*+                                                                         +*/
/*+  EDD_RQB_CSRT_CONSUMER_REMOVE_TYPE                                      +*/
/*+                                                                         +*/
/*+  ConsumerID : Valid ConsumerID                                          +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_SEQUENCE                                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Request to remove a consumer.                             +*/
/*+                                                                         +*/
/*+               A consumer has to be in use                               +*/
/*+               If no EDD_STS_ERR_PARAM                                   +*/
/*+                                                                         +*/
/*+               Consumer may be active on remove.                         +*/
/*+                                                                         +*/
/*+               Call command interface                                    +*/
/*+                                                                         +*/
/*+               If command is finished the function                       +*/
/*+               EDDP_CRTCmdDoneCbfCpm() is called                         +*/
/*+               Remove consumer from CRT management                       +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerRemove(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP                                     Response;
    EDD_UPPER_CSRT_CONSUMER_REMOVE_PTR_TYPE     pRqbParam;
    EDDP_CRT_CONSUMER_PTR_TYPE                  pConsumer;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pHDB);
#endif

    pRqbParam = (EDD_UPPER_CSRT_CONSUMER_REMOVE_PTR_TYPE) pRQB->pParam;

    EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_CRTConsumerRemove(pRQB=0x%X, ConsumerID=%d): ConsumerUsedCnt=%d",
        pHDB->Handle, pRQB, pRqbParam->ConsumerID, pDDB->CRT.Cons.ConsumerUsedCnt);

    // ***** check provider remove parameter (ProviderID, State, ...)
    Response = EDDP_CRTCheckConsumerRemoveParam (pDDB, pRqbParam);

    if (Response == EDD_STS_OK)
    {   // ***** remember ConsumerId for CMD-Request
        EDDP_ASSERT_FALSE(pRqbParam->ConsumerID >= pDDB->CRT.CfgParam.ConsumerMaxNumber);
        pConsumer = &pDDB->CRT.Cons.pConsEntry[pRqbParam->ConsumerID];

        EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_CRTConsumerRemove(): FrameID=0x%X, Consumer-State=0x%X, DgIdx=%d", 
            pHDB->Handle, pConsumer->Param.FrameID, pConsumer->State, pConsumer->DgIdx);

        if (pConsumer->State == EDDP_CRT_STATE_HW_ACTIVE)
        {
            /* ------------------------------------ */
            /* this consumer is active              */
            /* deactivate first and then remove     */
            /* ------------------------------------ */
            // ***** call CmdRequest ASYNCHRON
            // ***** set Response for RQBHandler    --> EDD_STS_OK_PENDING
            // if command is finished, the Cbf EDDP_CRTCmdDoneCbfCpm() will be called
            Response = EDDP_CRTCmdRequestCpm (pDDB, EDDP_CRT_CMD_CPM_ACW_OFF, pRqbParam->ConsumerID, EDDP_CRTCmdDoneCbfCpm);
        }
        else
        {
            /* ------------------------------------ */
            /* this consumer is alreasy passive     */
            /* ------------------------------------ */
            if (pConsumer->State == EDDP_CRT_STATE_HW_PASSIVE)
            {
                if (pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_DFP_ITEM)
                {
                    // can be occur after USER sequence:  ADD --> ACTIVATE --> DEACTIVATE --> REMOVE
                    EDDP_CRTConsumerDisconnectFromConsDgEntry (pDDB, pConsumer);
                    // remove from SW list only, because state is SW_USED !!!
                    EDDP_CRTConsumerRemoveFromCRTMngm (pDDB, pConsumer->ConsumerId, LSA_FALSE);
                }
                else
                {
                    /* ------------------------------------ */
                    /* remove now                           */
                    /* ------------------------------------ */
                    // ***** call CmdRequest ASYNCHRON
                    // ***** set Response for RQBHandler    --> EDD_STS_OK_PENDING
                    // if command is finished, the Cbf EDDP_CRTCmdDoneCbfCpm() will be called
                    Response = EDDP_CRTCmdRequestCpm (pDDB, EDDP_CRT_CMD_CPM_ACW_OUT, pRqbParam->ConsumerID, EDDP_CRTCmdDoneCbfCpm);
                }
            }
            else
            {
                // remove from SW list only, because state=SW_USED is not in RBaseList !!!
                EDDP_CRTConsumerRemoveFromCRTMngm (pDDB, pRqbParam->ConsumerID, LSA_FALSE);
            }
        }

    }

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTConsumerRemove(): ConsumerUsedCnt=%d, Response=0x%X", 
        pDDB->CRT.Cons.ConsumerUsedCnt, Response);

    return (Response);
}




/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTConsumerControl                     +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE          pRQB        +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE         pDDB        +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE         pHDB        +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                         +*/
/*+                                                                         +*/
/*+  EDD_RQB_CSRT_CONSUMER_CONTROL_TYPE                                     +*/
/*+                                                                         +*/
/*+  ConsumerID : Valid ConsumerID                                          +*/
/*+  Mode       : ACTIVATE       : activate consumer                        +*/
/*+               PASSIVATE      : deaktivate consumer                      +*/
/*+               SET_TO_UNKNOWN : set the actual consumer state to UNKNOWN +*/
/*+               SET_MAC        : evalute parameter "SrcMAC"               +*/
/*+               SET_IP         : evalute parameter "SrcIP"                +*/
/*+               PROV_AUTOSTOP_ENABLE                                      +*/
/*+               PROV_AUTOSTOP_DISABLE                                     +*/
/*+                 enables or disables the autostop feature of             +*/
/*+                 all providers associated with this consumer             +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Request to control a consumer.                            +*/
/*+                                                                         +*/
/*+               Check consumer is InUse. If no set EDD_STS_ERR_PARAM      +*/
/*+                                                                         +*/
/*+               We activate or passivate a consumer                       +*/
/*+                                                                         +*/
/*+               Call command interface                                    +*/
/*+                                                                         +*/
/*+               If command is finished the function                       +*/
/*+               EDDP_CRTCmdDoneCbfCpm() is called                         +*/
/*+               Set consumer State to ACTIVE                              +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerControl(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP                                     Response;
    EDDP_CRT_CONSUMER_PTR_TYPE                  pConsumer;
    EDD_UPPER_CSRT_CONSUMER_CONTROL_PTR_TYPE    pRqbParam;
    LSA_UINT16                                  DgIdx, FrameID;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pHDB);
#endif

    pRqbParam = (EDD_UPPER_CSRT_CONSUMER_CONTROL_PTR_TYPE) pRQB->pParam;

    if (pRqbParam->ConsumerID < pDDB->CRT.CfgParam.ConsumerMaxNumber)
    {   // ***** ok
        FrameID = pDDB->CRT.Cons.pConsEntry[pRqbParam->ConsumerID].Param.FrameID;
    }
    else
    {   // ***** error
        FrameID = 0xFFFF;
    }

    EDDP_CRT_TRACE_08(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_CRTConsumerControl(pRQB=0x%X, ConsumerID=%d, Mode=0x%X, FrameID=0x%X, CRNumber=%d, SFPosition=%d, SFOffset=%d)", 
        pHDB->Handle, pRQB, pRqbParam->ConsumerID, pRqbParam->Mode, FrameID, pRqbParam->IOParams.CRNumber, pRqbParam->IOParams.SFPosition, pRqbParam->IOParams.SFOffset);

    Response                                  = EDD_STS_OK;
    DgIdx                                     = EDDP_CRT_CHAIN_IDX_END;
    pDDB->CRT.Cons.RequestFinishConsumerID    = EDDP_CRT_CHAIN_IDX_END;
    pDDB->CRT.Cons.ConsumerControlMode        = pRqbParam->Mode;

    // clear DebugInfo block of this Control RQB
    EDDP_MEMSET_COMMON(&(pRqbParam->DebugInfo[0]), 0, EDD_DEBUG_INFO_SIZE);

    // check consumer control parameter (ConsumerID, ...)
    // get valid 'DgIdx' only if DFP-Consumer is activated
    Response = EDDP_CRTCheckConsumerControlParam (pDDB, pRqbParam, &DgIdx);

    if (Response == EDD_STS_OK)
    {
        EDDP_ASSERT_FALSE(pRqbParam->ConsumerID >= pDDB->CRT.CfgParam.ConsumerMaxNumber);
        pConsumer = &pDDB->CRT.Cons.pConsEntry[pRqbParam->ConsumerID];

        EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"[H:--] EDDP_CRTConsumerControl(Mode=0x%X): ConsumerID=%d, FrameID=0x%X, ConsumerFlags=0x%X", 
            pRqbParam->Mode, pConsumer->ConsumerId, pConsumer->Param.FrameID, pConsumer->ConsumerFlags);
        
        if ((pConsumer->State == EDDP_CRT_STATE_SW_USED) && (pRqbParam->Mode & EDD_CONS_CONTROL_MODE_ACTIVATE))
        {
            /* -------------------------------- */
            /* only for first ACTIVATE          */
            /* -------------------------------- */
            // update at first all undefined parameters with first ACTIVATE request
            EDDP_CRTConsumerFirstActivateControlReq (pDDB, pRqbParam);

            if ((pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_DFP_ITEM))
            {
                // for DFP-Consumer run EDDP_CRTConsumerDoControlReq() to activate the DG only !!!
                // first connect this consumer to DG (change 'State' to EDDP_CRT_STATE_HW_PASSIVE)
                EDDP_CRTConsumerConnectToConsDgEntry (pDDB, pConsumer, DgIdx);
                // and activate this DG
                Response = EDDP_CRTConsumerDoControlReq (pDDB, pRQB);
            }
            else
            {
                // run now ACW_IN for not DFP-Consumer and later EDDP_CRTConsumerDoControlReq() from CBF
                Response = EDDP_CRTConsumerDoAsynAcwInCmd (pDDB, pRqbParam->ConsumerID, EDDP_CRTCmdDoneCbfCpm); // returns EDD_STS_OK_PENDING
            }

            // Count activated consumer
            if (EDD_STS_OK_PENDING == Response)
            {
                pDDB->CRT.Cons.ConsumerActCnt++;
            }
        }
        else
        {
            /* -------------------------------- */
            /* other Modes                      */
            /* -------------------------------- */
            // only for DFP-Consumer: passive DG shall be activated
            if (   (pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_DFP_ITEM)
                && ((pConsumer->State == EDDP_CRT_STATE_HW_PASSIVE) && (pRqbParam->Mode & EDD_CONS_CONTROL_MODE_ACTIVATE))
               )
            {
                // connect this consumer to DG (change 'State' to EDDP_CRT_STATE_HW_PASSIVE)
                EDDP_CRTConsumerConnectToConsDgEntry (pDDB, pConsumer, DgIdx);
            }

            // Do Control for all Consumers
            Response = EDDP_CRTConsumerDoControlReq (pDDB, pRQB);

            // Count activated consumer
            if (EDD_STS_OK_PENDING == Response)
            {
                if  (  (pRqbParam->Mode & EDD_CONS_CONTROL_MODE_ACTIVATE) 
                    && (EDDP_CRT_STATE_HW_ACTIVE != pConsumer->State)  )
                {
                    pDDB->CRT.Cons.ConsumerActCnt++;
                }
                if  (  (pRqbParam->Mode & EDD_CONS_CONTROL_MODE_PASSIVATE) 
                    && (EDDP_CRT_STATE_HW_PASSIVE != pConsumer->State)  )
                {
                    EDDP_ASSERT(pDDB->CRT.Cons.ConsumerActCnt);
                    pDDB->CRT.Cons.ConsumerActCnt--;
                }
            }
        }
    }

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTConsumerControl(): Response=0x%X", Response);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTConsumerAddToCRTMngm                                +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB                +*/
/*+                             EDD_UPPER_CSRT_CONSUMER_ADD_PTR_TYPE    pRqbParam           +*/
/*+                                                                                         +*/
/*+  Result                :    EDD_RSP                                                     +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                     +*/
/*+  pRqbParam      : pointer to RQB parameter from user (UPPER-RQB)                        +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Add new consumer to CRT management                                        +*/
/*+               the entry is treated as free if its Status == EDDP_CRT_STATE_FREE         +*/
/*+               The Consumer-Status will be changed                                       +*/
/*                to EDDP_CRT_STATE_SW_USED within this function.                           +*/
/*+               If consumer list is full -> FATAL                                         +*/
/*+                                                                                         +*/
/*+               Init ACW of this consumer in APICtrl-RAM                                  +*/
/*+                                                                                         +*/
/*+               Add consumer to RBase-ConsList                                            +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerAddToCRTMngm(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDD_UPPER_CSRT_CONSUMER_ADD_PTR_TYPE    pRqbParam)
{
    EDD_RSP                                 Response;
    LSA_UINT32                              iDstAdr;
    LSA_UINT16                              NewConsumerID;
    EDDP_CRT_CONSUMER_PTR_TYPE              pConsumer;

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTConsumerAddToCRTMngm(pRqbParam=0x%X)", pRqbParam);

    Response        = EDD_STS_OK;
    NewConsumerID   = EDDP_CRTConsumerGetFreeConsumer (pDDB);

    if (NewConsumerID == EDDP_CRT_CHAIN_IDX_END)
    {
        // this should have been stated in EDDP_CRTCheckConsumerAddParam()
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTConsumerAddToCRTMngm(): FATAL -> No more free consumer! ConsumerUsedCnt=%d", 
            pDDB->CRT.Cons.ConsumerUsedCnt);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        EDDP_CRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTConsumerAddToCRTMngm(): NewConsumerID=%d, FrameID=0x%X, DataLen=%d, ConsProperties=0x%X, CRNumber=%d", 
            NewConsumerID, pRqbParam->FrameID, pRqbParam->DataLen, pRqbParam->Properties, pRqbParam->IOParams.CRNumber);

        /* ------------------------------------------------------------------------ */
		/* add new consumer to CRT.Cons.pConsEntry[NewConsumerID]                   */
		/* ------------------------------------------------------------------------ */
        pConsumer = &pDDB->CRT.Cons.pConsEntry[NewConsumerID];

        // ***** copy parameter to management structure
        pRqbParam->ConsumerID   = NewConsumerID;    // new ConsumerID for upper user
        pConsumer->UpperUserId  = pRqbParam->UserID;
        pConsumer->State        = EDDP_CRT_STATE_SW_USED;

        if (pRqbParam->Properties & EDD_CSRT_CONS_PROP_PDU_FORMAT_DFP)
        {   // ***** DFP runs in DP2 plane (RED)
            pConsumer->ConsumerFlags |= (EDDP_CONS_FLAG_IS_DFP_ITEM | EDDP_CONS_FLAG_IS_DP2ACW);

            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTConsumerAddToCRTMngm(): DFP-Consumer: FrameID=0x%X, SFPosition=%d", 
                pRqbParam->FrameID, pRqbParam->IOParams.SFPosition);
        }
        else
        {
            if (pRqbParam->Properties & EDD_CSRT_CONS_PROP_RTC3_MODE_LEGACY)
            {   // ***** we have only one (consumer) ACW for 2 types of provider (GREEN --> RED)
                pConsumer->ConsumerFlags |= (EDDP_CONS_FLAG_IS_2CR_MODE | EDDP_CONS_FLAG_IS_DP2ACW);
            }
            else
            {   // ***** we have an 1:1 relation to the provider (GREEN or RED only)
                if (EDDP_IS_IRT_FRAMEID(pRqbParam->FrameID))
                {   // ***** FrameID is RTC3 --> RED
                    pConsumer->ConsumerFlags |= (EDDP_CONS_FLAG_IS_DP2ACW);
                }
            }
        }

        if (pRqbParam->Properties & EDD_CSRT_CONS_PROP_SYSRED)
        {   // ***** Note: check function allows valid property combination only !!!
            pConsumer->ConsumerFlags |= (EDDP_CONS_FLAG_IS_SRD_ITEM);
        }

        pConsumer->Param.FrameID                    = pRqbParam->FrameID;
        for ( iDstAdr=0; iDstAdr < EDD_MAC_ADDR_SIZE; iDstAdr++ )
        {
            pConsumer->Param.SrcMAC.MacAdr[iDstAdr] = 0;
        }
        for ( iDstAdr=0; iDstAdr < EDD_IP_ADDR_SIZE; iDstAdr++ )
        {
            pConsumer->Param.SrcIP.IPAdr[iDstAdr] = 0;
        }
        pConsumer->ConsAcwTypeInfo.FrameID          = pRqbParam->FrameID;   // put also FrameID into common PtrArray entry
        pConsumer->Param.DataLen                    = pRqbParam->DataLen;
        pConsumer->Param.Partial_DataOffset         = pRqbParam->Partial_DataOffset;
        pConsumer->Param.Partial_DataLen            = pRqbParam->Partial_DataLen;

        pConsumer->Param.CycleReductionRatio        = pRqbParam->CycleReductionRatio;   // set real HW value before activating the ACW !!!
        pConsumer->Param.CyclePhase                 = pRqbParam->CyclePhase;
        pConsumer->Param.DataHoldFactor             = pRqbParam->DataHoldFactor;        // set real HW value before activating the ACW !!!
        pConsumer->Param.MediaRedFactor             = EDDP_CRT_CONSUMER_MRT_OFF;        // set real value on Activate
        // ***** NOTE: we can't set MediaRedFactor/HwDataHoldFactor/HwCycleReductionRatio here because CycleReductionRatio can be undefined !!!
        //             --> set later before creating the ACW --> EDDP_CRTConsumerDoAsynAcwInCmd()
        pConsumer->Param.UserID                     = pRqbParam->UserID;
        pConsumer->Param.DataOffset                 = pRqbParam->IOParams.DataOffset;

        // APDUStatusOffset for upper user (offset to begin of the PNIP-RAM)
        if (pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_DFP_ITEM)
        {
            // DFP-Consumer: is UNDEFINED
            pRqbParam->IOParams.APDUStatusOffset    = EDD_DATAOFFSET_UNDEFINED;
        }
        else
        {
            // Not-DFP-Consumer: APDU-Status in frame
            pRqbParam->IOParams.APDUStatusOffset    = EDDP_HALGetPnipOffsetToStart (pDDB, (EDD_COMMON_MEM_PTR_TYPE)(&pDDB->CRT.Cons.pHwBaseApduApi[NewConsumerID]));
        }

        pConsumer->Param.ForwardMode                = pRqbParam->IOParams.ForwardMode;
        pConsumer->Param.CutThrough_OutDataStart    = pRqbParam->IOParams.CutThrough_OutDataStart;
        pConsumer->Param.CRNumber                   = pRqbParam->IOParams.CRNumber;
        pConsumer->Param.ClearOnMISS                = pRqbParam->IOParams.ClearOnMISS;        
        pConsumer->Param.AppGroup                   = pRqbParam->IOParams.AppGroup;
        pConsumer->Param.SFPosition                 = pRqbParam->IOParams.SFPosition;
        pConsumer->Param.SFOffset                   = pRqbParam->IOParams.SFOffset;

        if (EDD_CSRT_CONS_PROP_RTCLASS_UDP == (pRqbParam->Properties & EDD_CSRT_CONS_PROP_RTCLASS_MASK))
        {
           pConsumer->Param.RTClass = EDDP_CRT_RTCLASS_UDP;
        }
        else
        {
           pConsumer->Param.RTClass = EDDP_CRT_RTCLASS_RT;
        }

        pConsumer->Param.Properties                 = pRqbParam->Properties;

        // ***** assign pTSBApi and pCSBApi address for this new consumer (interface specific !!!)
        pConsumer->pHwTSBApi    = &pDDB->CRT.Cons.pHwBaseTSBApi[NewConsumerID];
        pConsumer->pHwCSB0Api   = &pDDB->CRT.Cons.pHwBaseCSB0Api[NewConsumerID];
        pConsumer->pHwCSB1Api   = &pDDB->CRT.Cons.pHwBaseCSB1Api[NewConsumerID];

		/* ------------------------------------------------------------------------ */
		/* is this Consumer with CutThrough-Mode ?                                  */
		/* ------------------------------------------------------------------------ */
        if (pConsumer->Param.ForwardMode == EDD_FORWARDMODE_CT)
        {   // ***** save ConsumerID as CutThrough-Consumer
            pDDB->CRT.Cons.CT_ConsumerID = NewConsumerID;
        }
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTConsumerRemoveFromCRTMngm                           +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                             LSA_UINT16                  ConsumerID;                     +*/
/*+                             LSA_BOOL                    RemoveFromeRBaseList            +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB                 : Pointer to DeviceDescriptionBlock                               +*/
/*+  ConsumerID           : Consumer to be removed                                          +*/
/*+  RemoveFromeRBaseList : if TRUE call EDDP_CRTConsumerRemoveAcwFromRBaseSWList()         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Remove one consumer from CRT management                                   +*/
/*+                                                                                         +*/
/*+               Consumer must be used in CRT, if no FATAL                                 +*/
/*+                                                                                         +*/
/*+               The Consumer-Status will be changed                                       +*/
/*                to EDDP_CRT_STATE_FREE within this function.                              +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerRemoveFromCRTMngm(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ConsumerID,
    LSA_BOOL                    RemoveFromeRBaseList)
{
    EDDP_CRT_CONSUMER_PTR_TYPE              pConsumer;

    EDDP_ASSERT_FALSE(ConsumerID >= pDDB->CRT.CfgParam.ConsumerMaxNumber);

    pConsumer    = &pDDB->CRT.Cons.pConsEntry[ConsumerID];

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CRTConsumerRemoveFromCRTMngm(ConsumerID=%d): ConsumerUsedCnt=%d",
        ConsumerID, pDDB->CRT.Cons.ConsumerUsedCnt);

    EDDP_CRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTConsumerRemoveFromCRTMngm(ConsumerID=%d): FrameID=0x%X, ConsumerFlags=0x%X, SFPosition=%d, DgIdx=%d", 
        ConsumerID, pConsumer->Param.FrameID, pConsumer->ConsumerFlags, pConsumer->Param.SFPosition, pConsumer->DgIdx);

    if (pDDB->CRT.Cons.ConsumerUsedCnt == 0)
    {
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTConsumerRemoveFromCRTMngm(): FATAL -> ConsumerUsedCnt=0!");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    if (RemoveFromeRBaseList)
    {   // ***** remove consumer from RBaseEntry
        pDDB->CRT.Cons.RBaseHandler.FctRemoveAcwFromRBase(pDDB, ConsumerID);
        pConsumer->State = EDDP_CRT_STATE_SW_USED;
    }

    if (pConsumer->State != EDDP_CRT_STATE_SW_USED)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTConsumerRemoveFromCRTMngm(): FATAL -> Cons-State(0x%X) != USED!", 
            pConsumer->State);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {   // ***** reset/clear ACW of this consumer in APICtrl-RAM
        EDDP_CRTConsDeInitConsAcwHwApi (pDDB, ConsumerID);

        // ***** remove consumer from UsedList
        EDDP_CRTConsumerReleaseConsumer (pDDB, ConsumerID);

        // ***** was this Consumer with CutThrough-Mode ?
        if (ConsumerID == pDDB->CRT.Cons.CT_ConsumerID)
        {   // ***** no more CutThrough-Mode in CRT
            EDDP_CRTConsumerClrCtRwb (pDDB, ConsumerID);
            pDDB->CRT.Cons.CT_ConsumerID = EDDP_CRT_CHAIN_IDX_END;
        }
    }

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTConsumerRemoveFromCRTMngm(): ConsumerUsedCnt=%d",
        pDDB->CRT.Cons.ConsumerUsedCnt);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTConsumerGetFreeConsumer                             +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_UINT16                                                  +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Add Consumer to UsedList by Consumer-Add                                  +*/
/*+                 -> get first free index from ConsFreeList and put it to ConsUsedList    +*/
/*+                 -> the NewConsumerID will be returned in return value                   +*/
/*+                                                                                         +*/
/*+                 -> if (NewConsumerID == EDDP_CRT_CHAIN_IDX_END)                         +*/
/*+                     - no more free consumer                                             +*/
/*+                     - set error by caller of this function                              +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerGetFreeConsumer(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT16                              NewConsumerID;
    EDDP_CRT_CONSUMER_PTR_TYPE              pConsumer;

    NewConsumerID   =  pDDB->CRT.Cons.ConsumerFreeList.BeginIdx;   // first index in the consumer free list

    if (NewConsumerID == EDDP_CRT_CHAIN_IDX_END)
    {
        // ***** no more free consumer
        // ***** set error by caller of this function
    }
    else
    {
        pConsumer = &pDDB->CRT.Cons.pConsEntry[NewConsumerID];

        /* ------------------------------------------------------------ */
	    /* Remove this new Consumer from free-chain list                */
	    /* ------------------------------------------------------------ */
        if (pConsumer->UsedFreeLink.Next == EDDP_CRT_CHAIN_IDX_END)
        {   // ***** this consumer is the last free consumer
            pDDB->CRT.Cons.ConsumerFreeList.BeginIdx  = EDDP_CRT_CHAIN_IDX_END;
            pDDB->CRT.Cons.ConsumerFreeList.EndIdx    = EDDP_CRT_CHAIN_IDX_END;
        }
        else
        {   // ***** this consumer is not the last free consumer
            pDDB->CRT.Cons.ConsumerFreeList.BeginIdx                                  = pConsumer->UsedFreeLink.Next;
            pDDB->CRT.Cons.pConsEntry[pConsumer->UsedFreeLink.Next].UsedFreeLink.Prev = EDDP_CRT_CHAIN_IDX_END;
        }

        /* ------------------------------------------------------------ */
        /* Put this new Consumer to end used-chain list                 */
        /* ------------------------------------------------------------ */
        if (pDDB->CRT.Cons.ConsumerUsedList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
        {   // ***** this consumer is the first consumer in the used list
            pDDB->CRT.Cons.ConsumerUsedList.BeginIdx = NewConsumerID;
            pDDB->CRT.Cons.ConsumerUsedList.EndIdx   = NewConsumerID;
            pConsumer->UsedFreeLink.Prev             = EDDP_CRT_CHAIN_IDX_END;
            pConsumer->UsedFreeLink.Next             = EDDP_CRT_CHAIN_IDX_END;
        }
        else
        {   // ***** this consumer is not the first consumer in the used list
            pConsumer->UsedFreeLink.Prev                                                        = pDDB->CRT.Cons.ConsumerUsedList.EndIdx;
            pConsumer->UsedFreeLink.Next                                                        = EDDP_CRT_CHAIN_IDX_END;
            pDDB->CRT.Cons.pConsEntry[pDDB->CRT.Cons.ConsumerUsedList.EndIdx].UsedFreeLink.Next = NewConsumerID;
            pDDB->CRT.Cons.ConsumerUsedList.EndIdx                                              = NewConsumerID;
        }

        pConsumer->ConsumerFlags = 0;
        pDDB->CRT.Cons.ConsumerUsedCnt++;
    }

    return (NewConsumerID);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTConsumerReleaseConsumer                             +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                             LSA_UINT16                  ConsumerID                      +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ConsumerID         : Consumer to be removed                                            +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: remove Consumer from UsedList by Consumer-Remove                          +*/
/*+                 -> remove this consumer from ConsUsedList and put it to ConsFreeList    +*/
/*+                 -> no check ConsumerID, it must be valid !                              +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerReleaseConsumer(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ConsumerID)
{
    LSA_UINT16                              Prev;
    LSA_UINT16                              Next;
    EDDP_CRT_CONSUMER_PTR_TYPE              pConsumer;

    EDDP_ASSERT_FALSE(ConsumerID >= pDDB->CRT.CfgParam.ConsumerMaxNumber);

    pConsumer   = &pDDB->CRT.Cons.pConsEntry[ConsumerID];
    Prev        = pConsumer->UsedFreeLink.Prev;
    Next        = pConsumer->UsedFreeLink.Next;

    /* ------------------------------------------------------------ */
    /* Remove this Consumer from used-chain list                    */
    /* ------------------------------------------------------------ */
    if ((Prev == EDDP_CRT_CHAIN_IDX_END) || (Next == EDDP_CRT_CHAIN_IDX_END))
    {
        if (Prev == EDDP_CRT_CHAIN_IDX_END)
        {   // ***** this consumer is first consumer in used list
            if (Next == EDDP_CRT_CHAIN_IDX_END)
            {   // ***** first and only one consumer in used list
                pDDB->CRT.Cons.ConsumerUsedList.BeginIdx    = EDDP_CRT_CHAIN_IDX_END;
                pDDB->CRT.Cons.ConsumerUsedList.EndIdx      = EDDP_CRT_CHAIN_IDX_END;
            }
            else
            {   // ***** first consumer in used list but not only one
                pDDB->CRT.Cons.ConsumerUsedList.BeginIdx            = Next;
                pDDB->CRT.Cons.pConsEntry[Next].UsedFreeLink.Prev   = EDDP_CRT_CHAIN_IDX_END;
            }
        }
        else
        {   // ***** last consumer in used list but not only one
            pDDB->CRT.Cons.pConsEntry[Prev].UsedFreeLink.Next   = EDDP_CRT_CHAIN_IDX_END;
            pDDB->CRT.Cons.ConsumerUsedList.EndIdx              = Prev;
        }
    }
    else
    {   // ***** this consumer is in the middle of the used list
        pDDB->CRT.Cons.pConsEntry[Prev].UsedFreeLink.Next   = Next;
        pDDB->CRT.Cons.pConsEntry[Next].UsedFreeLink.Prev   = Prev;
    }

    /* ------------------------------------------------------------ */
    /* Put this Consumer to beginn of free-chain list               */
    /* ------------------------------------------------------------ */
    if (pDDB->CRT.Cons.ConsumerFreeList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
    {   // ***** no consumer in free list
        pConsumer->UsedFreeLink.Prev              = EDDP_CRT_CHAIN_IDX_END;
        pConsumer->UsedFreeLink.Next              = EDDP_CRT_CHAIN_IDX_END;
        pDDB->CRT.Cons.ConsumerFreeList.BeginIdx  = ConsumerID;
        pDDB->CRT.Cons.ConsumerFreeList.EndIdx    = ConsumerID;
    }
    else
    {   // ***** one ore more consumer in free list
        pConsumer->UsedFreeLink.Prev                                                          = EDDP_CRT_CHAIN_IDX_END;
        pConsumer->UsedFreeLink.Next                                                          = pDDB->CRT.Cons.ConsumerFreeList.BeginIdx;
        pDDB->CRT.Cons.pConsEntry[pDDB->CRT.Cons.ConsumerFreeList.BeginIdx].UsedFreeLink.Prev = ConsumerID;
        pDDB->CRT.Cons.ConsumerFreeList.BeginIdx                                              = ConsumerID;
    }

    EDDP_ASSERT(pDDB->CRT.Cons.ConsumerUsedCnt);
    pDDB->CRT.Cons.ConsumerUsedCnt--;

    // ***** this consumer is no more in use
    pConsumer->State            = EDDP_CRT_STATE_FREE;
    pConsumer->ConsumerFlags    = 0;
    pConsumer->DgIdx            = EDDP_CRT_CHAIN_IDX_END;
    pConsumer->DgGrpIdx         = EDDP_CRT_CHAIN_IDX_END;
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTConsumerGetFreeDgItem                               +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_UINT16                                                  +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Add DG Consumer to UsedDgList by by internal StateMachine (PRM)           +*/
/*+                 -> get first free index from DgFreeList and put it to UsedDgList        +*/
/*+                 -> the NewDgID will be returned in return value                         +*/
/*+                                                                                         +*/
/*+                 -> if (NewDgID == EDDP_CRT_CHAIN_IDX_END)                               +*/
/*+                     - no more free DG consumer                                          +*/
/*+                     - set error by caller of this function                              +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerGetFreeDgItem(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT16                              NewDgID;
    EDDP_CRT_CONSUMER_DG_PTR_TYPE           pDgConsumer;

    NewDgID      =  pDDB->CRT.Cons.ConsumerDgFreeList.BeginIdx;   // first index in the consumer DG free list

    if (NewDgID == EDDP_CRT_CHAIN_IDX_END)
    {
        // ***** no more free DG consumer
        // ***** set error by caller of this function
    }
    else
    {
        pDgConsumer = &pDDB->CRT.Cons.pConsDgEntry[NewDgID];

        /* ------------------------------------------------------------ */
	    /* Remove this new DG Consumer from free-chain list             */
	    /* ------------------------------------------------------------ */
        if (pDgConsumer->UsedFreeDgLink.Next == EDDP_CRT_CHAIN_IDX_END)
        {   // ***** this DG consumer is the last free DG consumer
            pDDB->CRT.Cons.ConsumerDgFreeList.BeginIdx  = EDDP_CRT_CHAIN_IDX_END;
            pDDB->CRT.Cons.ConsumerDgFreeList.EndIdx    = EDDP_CRT_CHAIN_IDX_END;
        }
        else
        {   // ***** this DG consumer is not the last free DG consumer
            pDDB->CRT.Cons.ConsumerDgFreeList.BeginIdx                                        = pDgConsumer->UsedFreeDgLink.Next;
            pDDB->CRT.Cons.pConsDgEntry[pDgConsumer->UsedFreeDgLink.Next].UsedFreeDgLink.Prev = EDDP_CRT_CHAIN_IDX_END;
        }

        /* ------------------------------------------------------------ */
        /* Put this new DG Consumer to end used-chain list              */
        /* ------------------------------------------------------------ */
        if (pDDB->CRT.Cons.ConsumerDgUsedList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
        {   // ***** this consumer is the first consumer in the used list
            pDDB->CRT.Cons.ConsumerDgUsedList.BeginIdx = NewDgID;
            pDDB->CRT.Cons.ConsumerDgUsedList.EndIdx   = NewDgID;
            pDgConsumer->UsedFreeDgLink.Prev           = EDDP_CRT_CHAIN_IDX_END;
            pDgConsumer->UsedFreeDgLink.Next           = EDDP_CRT_CHAIN_IDX_END;
        }
        else
        {   // ***** this DG consumer is not the first DG consumer in the used list
            pDgConsumer->UsedFreeDgLink.Prev                                                          = pDDB->CRT.Cons.ConsumerDgUsedList.EndIdx;
            pDgConsumer->UsedFreeDgLink.Next                                                          = EDDP_CRT_CHAIN_IDX_END;
            pDDB->CRT.Cons.pConsDgEntry[pDDB->CRT.Cons.ConsumerDgUsedList.EndIdx].UsedFreeDgLink.Next = NewDgID;
            pDDB->CRT.Cons.ConsumerDgUsedList.EndIdx                                                  = NewDgID;
        }

        pDDB->CRT.Cons.ConsumerDgUsedCnt++;
    }

    return (NewDgID);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTConsumerAppendDgItem                                +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB                +*/
/*+                             EDDP_CRT_CONSUMER_CONTAINER_PTR_TYPE    pContainer          +*/
/*+                             LSA_UINT16                              DgID                +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  pContainer         : Container that holds Dg-items                                     +*/
/*+  DgID               : index of Dg-item                                                  +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: put given DG-item into Container (usedlist) and increment its DgCount     +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerAppendDgItem(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_CRT_CONSUMER_CONTAINER_PTR_TYPE    pContainer,
    LSA_UINT16                              DgID)
{
    EDDP_CRT_CONSUMER_DG_PTR_TYPE           pDgConsumer, pDgPrev;

    EDDP_ASSERT_NO_NULL_PTR(pContainer);
    EDDP_ASSERT_FALSE(DgID >= pDDB->CRT.CfgParam.ConsumerDGMaxNumber);

    pDgConsumer             = &pDDB->CRT.Cons.pConsDgEntry[DgID];
    pDgConsumer->DgGrpIdx   = pContainer->DgGrpIdx;     // set reference to container

    if (pContainer->ConsDgList.BeginIdx == EDDP_CRT_CHAIN_IDX_END )
    {   // ***** it is the first item
        pContainer->ConsDgList.BeginIdx = DgID; // first item in list
        pDgConsumer->DgLink.Prev        = EDDP_CRT_CHAIN_IDX_END;
        pDgConsumer->DgLink.Next        = EDDP_CRT_CHAIN_IDX_END;
    }
    else
    {   // ***** append after last item
        pDgPrev = &pDDB->CRT.Cons.pConsDgEntry[pContainer->ConsDgList.EndIdx];
        pDgPrev->DgLink.Next        = DgID;
        pDgConsumer->DgLink.Prev    = pDgPrev->DgIdx;
        pDgConsumer->DgLink.Next    = EDDP_CRT_CHAIN_IDX_END;
    }

    pContainer->ConsDgList.EndIdx = DgID;       // new item is always at the end
    pContainer->DgCount++;                      // count added item
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTConsumerRemoveFirstDgItem                           +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB                +*/
/*+                             EDDP_CRT_CONSUMER_CONTAINER_PTR_TYPE    pContainer          +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_UINT16                                                  +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  pContainer         : Container that holds Dg-items                                     +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: remove DG-item from Container and put it into freelist                    +*/
/*+                 -> remove DG from beginning of Container list                           +*/
/*+                 -> put into freelist                                                    +*/
/*+                 -> decr. DgCount of Container                                           +*/
/*+                 -> returns index of removed DG-item or EDDP_CRT_CHAIN_IDX_END           +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerRemoveFirstDgItem(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDDP_CRT_CONSUMER_CONTAINER_PTR_TYPE    pContainer)
{
    LSA_UINT16                              FirstDgIdx;
    EDDP_CRT_CONSUMER_DG_PTR_TYPE           pDgConsumer, pDgNext;

    EDDP_ASSERT_NO_NULL_PTR(pContainer);

    FirstDgIdx = pContainer->ConsDgList.BeginIdx;

    if (FirstDgIdx != EDDP_CRT_CHAIN_IDX_END)
    {   // ***** DGs in list
        pDgConsumer  = &pDDB->CRT.Cons.pConsDgEntry[FirstDgIdx];

        if (pContainer->ConsDgList.EndIdx == FirstDgIdx)
        {   // ***** remove the last item from the list --> set list empty
            pContainer->ConsDgList.BeginIdx = EDDP_CRT_CHAIN_IDX_END;
            pContainer->ConsDgList.EndIdx   = EDDP_CRT_CHAIN_IDX_END;
        }
        else
        {   // ***** remove first item and update linkage
            pContainer->ConsDgList.BeginIdx = pDgConsumer->DgLink.Next; 
            pDgNext                         = &pDDB->CRT.Cons.pConsDgEntry[pDgConsumer->DgLink.Next];
            pDgNext->DgLink.Prev            = EDDP_CRT_CHAIN_IDX_END;
        }

        // ***** release container reference and unbind from Dglist
        pDgConsumer->DgGrpIdx       = EDDP_CRT_CHAIN_IDX_END;
        pDgConsumer->DgLink.Prev    = EDDP_CRT_CHAIN_IDX_END;
        pDgConsumer->DgLink.Next    = EDDP_CRT_CHAIN_IDX_END;

        /* ------------------------------------------------------------ */
        /* Put this DG Consumer to end of free-chain list               */
        /* ------------------------------------------------------------ */
        if (pDDB->CRT.Cons.ConsumerDgFreeList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
        {   // ***** no DG consumer in free list
            pDgConsumer->UsedFreeDgLink.Prev             = EDDP_CRT_CHAIN_IDX_END;
            pDgConsumer->UsedFreeDgLink.Next             = EDDP_CRT_CHAIN_IDX_END;
            pDDB->CRT.Cons.ConsumerDgFreeList.BeginIdx   = FirstDgIdx;
            pDDB->CRT.Cons.ConsumerDgFreeList.EndIdx     = FirstDgIdx;
        }
        else
        {   // ***** one ore more consumer in free list
            pDgConsumer->UsedFreeDgLink.Prev                                                          = pDDB->CRT.Cons.ConsumerDgFreeList.EndIdx;
            pDgConsumer->UsedFreeDgLink.Next                                                          = EDDP_CRT_CHAIN_IDX_END;
            pDDB->CRT.Cons.pConsDgEntry[pDDB->CRT.Cons.ConsumerDgFreeList.EndIdx].UsedFreeDgLink.Next = FirstDgIdx;
            pDDB->CRT.Cons.ConsumerDgFreeList.EndIdx                                                  = FirstDgIdx;
        }

        EDDP_ASSERT(pDDB->CRT.Cons.ConsumerDgUsedCnt && pContainer->DgCount);
        pDDB->CRT.Cons.ConsumerDgUsedCnt--;
        pContainer->DgCount--;      // count removed item
    }

    return (FirstDgIdx);
}





#if defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTConsumerAddAcwToRBaseSWList                         +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                             LSA_UINT16                  AcwID                           +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                     +*/
/*+  AcwID          : Consumer/Container in RBaseList which get New_ACW_nextoffset          +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Add Consumer into ConsRBaseList by Consumer-Add depending on FrameID      +*/
/*+                                                                                         +*/
/*+               ConsRBaseList   Cons1 -> Cons2 -> ... -> ConsN                            +*/
/*+                                                                                         +*/
/*+               ConsRBaseList is sorted by FrameID                                        +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerAddAcwToRBaseSWList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID)
{
    LSA_UINT16                              RBaseIndex;
    LSA_UINT16                              NewFrameIDUser;
    LSA_UINT16                              AcwMaxNumber;
    LSA_UINT16                              ConsUsedIdx;
    LSA_UINT16                              ConsUsedIdxPrev;        // for add or remove between Prev and Next in ConsList
    LSA_UINT16                              ConsUsedIdxNext;
    EDDP_CRT_CONSUMER_RBASE_PTR_TYPE        pRBase;
    EDDP_CRT_CONS_ACW_INFO_PTR_PTR_TYPE     pCommonAcwPtr;

    AcwMaxNumber  = pDDB->CRT.CfgParam.ConsumerMaxNumber;
    AcwMaxNumber += pDDB->CRT.CfgParam.ConsumerContainerMaxNumber;
    EDDP_ASSERT_FALSE(AcwID >= AcwMaxNumber);

    pCommonAcwPtr    = &pDDB->CRT.Cons.pAcwPtrArray[0];
    NewFrameIDUser   = pCommonAcwPtr[AcwID]->FrameID;
    RBaseIndex       = pDDB->CRT.Cons.RBaseHandler.FctGetRBaseIndex(pDDB, NewFrameIDUser);
    pRBase           = &pDDB->CRT.Cons.pRBaseEntry[RBaseIndex];

    if (pRBase->ConsRBaseList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
    {
        // ***** this consumer is the first consumer in the ConsRBaseList
        pRBase->ConsRBaseList.BeginIdx       = AcwID;
        pRBase->ConsRBaseList.EndIdx         = AcwID;
        pCommonAcwPtr[AcwID]->RBaseLink.Prev = EDDP_CRT_CHAIN_IDX_END;
        pCommonAcwPtr[AcwID]->RBaseLink.Next = EDDP_CRT_CHAIN_IDX_END;
        // ACW_nextoffset of new ACW contains already the end identifier 0xFFFF
    }
    else
    {
        // ***** this consumer is not the first consumer in the ConsRBaseList
        // ***** chain this consumer to position "NewFrameIDUser" in ConsRBaseList
        ConsUsedIdx = pRBase->ConsRBaseList.BeginIdx;

        // ***** go through ConsRBaseList from BeginIdx to EndIdx
        while ( (NewFrameIDUser >= pCommonAcwPtr[ConsUsedIdx]->FrameID) && (ConsUsedIdx != pRBase->ConsRBaseList.EndIdx) )
        {   // ***** compare next ConsUsedIdx
            EDDP_ASSERT_FALSE( ( (ConsUsedIdx >= pDDB->CRT.CfgParam.ConsumerMaxNumber) && (!(pCommonAcwPtr[ConsUsedIdx]->IsAcwContainer)) ) );
            ConsUsedIdx = pCommonAcwPtr[ConsUsedIdx]->RBaseLink.Next;
        }

        // init ConsUsedIdxPrev and ConsUsedIdxNext
        if ((ConsUsedIdx == pRBase->ConsRBaseList.BeginIdx) || (ConsUsedIdx == pRBase->ConsRBaseList.EndIdx))
        {
            if (NewFrameIDUser < pCommonAcwPtr[ConsUsedIdx]->FrameID)
            {   // ***** chain before ConsUsedIdx
                ConsUsedIdxPrev = pCommonAcwPtr[ConsUsedIdx]->RBaseLink.Prev;
                ConsUsedIdxNext = ConsUsedIdx;
            }
            else
            {   // ***** chain behind ConsUsedIdx
                ConsUsedIdxPrev = ConsUsedIdx;
                ConsUsedIdxNext = pCommonAcwPtr[ConsUsedIdx]->RBaseLink.Next;
            }
        }
        else
        {
            if (NewFrameIDUser == pCommonAcwPtr[ConsUsedIdx]->FrameID)
            {   // ***** chain behind ConsUsedIdx
                ConsUsedIdxPrev = ConsUsedIdx;
                ConsUsedIdxNext = pCommonAcwPtr[ConsUsedIdx]->RBaseLink.Next;
            }
            else
            {   // ***** chain before ConsUsedIdx
                ConsUsedIdxPrev = pCommonAcwPtr[ConsUsedIdx]->RBaseLink.Prev;
                ConsUsedIdxNext = ConsUsedIdx;
            }
        }

        // ***** chain between ConsUsedIdxPrev and ConsUsedIdxNext
        if ((ConsUsedIdxPrev == EDDP_CRT_CHAIN_IDX_END) || (ConsUsedIdxNext == EDDP_CRT_CHAIN_IDX_END))
        {
            // ***** chain to begin or end in ConsRBaseList
            if (ConsUsedIdxPrev == EDDP_CRT_CHAIN_IDX_END)
            {
                if (ConsUsedIdxNext == EDDP_CRT_CHAIN_IDX_END)
                {
                    // ***** this case is for the first consumer in the ConsRBaseList - it was added above
                }
                else
                {
                    // ***** chain to begin in ConsRBaseList
                    pCommonAcwPtr[AcwID]->RBaseLink.Prev                             = EDDP_CRT_CHAIN_IDX_END;
                    pCommonAcwPtr[AcwID]->RBaseLink.Next                             = pRBase->ConsRBaseList.BeginIdx;
                    pCommonAcwPtr[pRBase->ConsRBaseList.BeginIdx]->RBaseLink.Prev    = AcwID;
                    pRBase->ConsRBaseList.BeginIdx                                   = AcwID;

                    // ***** update ACW_nextoffset of new ACW
                    pDDB->CRT.Cons.AcwHandler.FctUpdateAcwNextOffsetAcwHwApi (pDDB, AcwID);
                }
            }
            else
            {
                // ***** chain to end in ConsRBaseList
                pCommonAcwPtr[AcwID]->RBaseLink.Prev                         = pRBase->ConsRBaseList.EndIdx;
                pCommonAcwPtr[AcwID]->RBaseLink.Next                         = EDDP_CRT_CHAIN_IDX_END;
                pCommonAcwPtr[pRBase->ConsRBaseList.EndIdx]->RBaseLink.Next  = AcwID;
                pRBase->ConsRBaseList.EndIdx                                 = AcwID;

                // ***** ACW_nextoffset of new ACW contains already the end identifier 0xFFFF
            }
        }
        else
        {
            // ***** chain to middle in ConsRBaseList
            pCommonAcwPtr[AcwID]->RBaseLink.Prev             = ConsUsedIdxPrev;
            pCommonAcwPtr[AcwID]->RBaseLink.Next             = ConsUsedIdxNext;
            pCommonAcwPtr[ConsUsedIdxPrev]->RBaseLink.Next   = AcwID;
            pCommonAcwPtr[ConsUsedIdxNext]->RBaseLink.Prev   = AcwID;

            // ***** update ACW_nextoffset of new ACW
            pDDB->CRT.Cons.AcwHandler.FctUpdateAcwNextOffsetAcwHwApi (pDDB, AcwID);
        }
    }

    pRBase->AcwCnt++;

    if (pCommonAcwPtr[AcwID]->IsAcwContainer)
    {
        pRBase->ContainerCnt++;
    }
}
#endif





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTConsumerAddAcwToRBaseSWList_Rev3                    +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                             LSA_UINT16                  AcwID                           +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                     +*/
/*+  AcwID          : Consumer/Container in RBaseList which get New_ACW_nextoffset          +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Add Consumer into ConsRBaseList by Consumer-Add depending on FrameID      +*/
/*+                                                                                         +*/
/*+               ConsRBaseList   Cons1 -> Cons2 -> ... -> ConsN                            +*/
/*+                                                                                         +*/
/*+               NOTE: Consumer always added to end                                        +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerAddAcwToRBaseSWList_Rev3(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID)
{
    LSA_UINT16                              RBaseIndex;
    LSA_UINT16                              NewFrameIDUser;
    LSA_UINT16                              AcwMaxNumber;
    EDDP_CRT_CONSUMER_RBASE_PTR_TYPE        pRBase;
    EDDP_CRT_CONS_ACW_INFO_PTR_PTR_TYPE     pCommonAcwPtr;

    /* sanity check for valid ACW ID */
    AcwMaxNumber  = pDDB->CRT.CfgParam.ConsumerMaxNumber;
    AcwMaxNumber += pDDB->CRT.CfgParam.ConsumerContainerMaxNumber;
    EDDP_ASSERT_FALSE(AcwID >= AcwMaxNumber);

    pCommonAcwPtr    = &pDDB->CRT.Cons.pAcwPtrArray[0];
    NewFrameIDUser   = pCommonAcwPtr[AcwID]->FrameID;
    RBaseIndex       = pDDB->CRT.Cons.RBaseHandler.FctGetRBaseIndex(pDDB, NewFrameIDUser);
    pRBase           = &pDDB->CRT.Cons.pRBaseEntry[RBaseIndex];

    // ***** this consumer is the first consumer in ConsRBaseList
    if (pRBase->ConsRBaseList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
    {
        pRBase->ConsRBaseList.BeginIdx       = AcwID;
        pRBase->ConsRBaseList.EndIdx         = AcwID;
        pCommonAcwPtr[AcwID]->RBaseLink.Prev = EDDP_CRT_CHAIN_IDX_END;
        pCommonAcwPtr[AcwID]->RBaseLink.Next = EDDP_CRT_CHAIN_IDX_END;
    }
    // ***** this consumer is the new last one in ConsRBaseList
    else
    {
        pCommonAcwPtr[AcwID]->RBaseLink.Prev                         = pRBase->ConsRBaseList.EndIdx;
        pCommonAcwPtr[AcwID]->RBaseLink.Next                         = EDDP_CRT_CHAIN_IDX_END;
        pCommonAcwPtr[pRBase->ConsRBaseList.EndIdx]->RBaseLink.Next  = AcwID;
        pRBase->ConsRBaseList.EndIdx                                 = AcwID; /* IMPORTANT: this must be the last operation */
    }

    pRBase->AcwCnt++;

    if (pCommonAcwPtr[AcwID]->IsAcwContainer)
    {
        pRBase->ContainerCnt++;
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTConsumerRemoveAcwFromRBaseSWList                    +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                             LSA_UINT16                  AcwID                           +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                     +*/
/*+  AcwID          : Consumer/Container in RBaseList which get New_ACW_nextoffset          +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Remove Consumer from ConsRBaseList by Consumer-Remove                     +*/
/*+                 -> no check ConsumerID, it must be valid !                              +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerRemoveAcwFromRBaseSWList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID)
{
    LSA_UINT16                              RBaseIndex;
    LSA_UINT16                              ConsUsedIdxPrev;
    LSA_UINT16                              ConsUsedIdxNext;
    LSA_UINT16                              AcwMaxNumber;
    LSA_UINT16                              FrameID;
    EDDP_CRT_CONSUMER_RBASE_PTR_TYPE        pRBase;
    EDDP_CRT_CONS_ACW_INFO_PTR_PTR_TYPE     pCommonAcwPtr;

    AcwMaxNumber  = pDDB->CRT.CfgParam.ConsumerMaxNumber;
    AcwMaxNumber += pDDB->CRT.CfgParam.ConsumerContainerMaxNumber;
    EDDP_ASSERT_FALSE(AcwID >= AcwMaxNumber);

    pCommonAcwPtr   = &pDDB->CRT.Cons.pAcwPtrArray[0];
    FrameID         = pCommonAcwPtr[AcwID]->FrameID;
    RBaseIndex      = pDDB->CRT.Cons.RBaseHandler.FctGetRBaseIndex(pDDB, FrameID);
    pRBase          = &pDDB->CRT.Cons.pRBaseEntry[RBaseIndex];

    if (pRBase->AcwCnt == 0)
    {
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTConsumerRemoveAcwFromRBaseSWList(): FATAL -> No Consumer (ConsRBaseList) on this RBase-Entry!");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {   // ***** remove consumer from ConsRBaseList on this RBaseEntry
        ConsUsedIdxPrev = pCommonAcwPtr[AcwID]->RBaseLink.Prev;
        ConsUsedIdxNext = pCommonAcwPtr[AcwID]->RBaseLink.Next;

        if ((ConsUsedIdxPrev == EDDP_CRT_CHAIN_IDX_END) || (ConsUsedIdxNext == EDDP_CRT_CHAIN_IDX_END))
        {
            if (ConsUsedIdxPrev == EDDP_CRT_CHAIN_IDX_END)
            {   // ***** this consumer is first consumer in ConsRBaseList
                if (ConsUsedIdxNext == EDDP_CRT_CHAIN_IDX_END)
                {   // ***** first and only one consumer in ConsRBaseList
                    pRBase->ConsRBaseList.BeginIdx   = EDDP_CRT_CHAIN_IDX_END;
                    pRBase->ConsRBaseList.EndIdx     = EDDP_CRT_CHAIN_IDX_END;
                }
                else
                {   // ***** first consumer in ConsRBaseList but not only one
                    pRBase->ConsRBaseList.BeginIdx                   = ConsUsedIdxNext;
                    //pDDB->CRT.Cons.pConsEntry[ConsUsedIdxNext].ConsAcwTypeInfo.RBaseLink.Prev = EDDP_CRT_CHAIN_IDX_END;
                    pCommonAcwPtr[ConsUsedIdxNext]->RBaseLink.Prev   = EDDP_CRT_CHAIN_IDX_END;
                }
            }
            else
            {   // ***** last consumer in ConsRBaseList but not only one
                pCommonAcwPtr[ConsUsedIdxPrev]->RBaseLink.Next   = EDDP_CRT_CHAIN_IDX_END;
                pRBase->ConsRBaseList.EndIdx                     = ConsUsedIdxPrev;
            }
        }
        else
        {   // ***** this consumer is in the middle of ConsRBaseList
            pCommonAcwPtr[ConsUsedIdxPrev]->RBaseLink.Next  = ConsUsedIdxNext;
            pCommonAcwPtr[ConsUsedIdxNext]->RBaseLink.Prev  = ConsUsedIdxPrev;
        }

        pRBase->AcwCnt--;

        if (pCommonAcwPtr[AcwID]->IsAcwContainer)
        {
            EDDP_ASSERT( pRBase->ContainerCnt);
            pRBase->ContainerCnt--;
        }
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTGetConsumerState                               +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB                    +*/
/*+                      LSA_UINT16                     ConsumerID              +*/
/*+                                                                             +*/
/*+  Result         :    LSA_UINT16                                             +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  ConsumerID     : selects the Consumer                                      +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function reads the State from the Consumer.              +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT32 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetConsumerState(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT16                  ConsumerID)
{
    EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer;

    EDDP_ASSERT_FALSE(ConsumerID >= pDDB->CRT.CfgParam.ConsumerMaxNumber);
    pConsumer = &pDDB->CRT.Cons.pConsEntry[ConsumerID];

    return (pConsumer->State);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTGetShiftedTSBOffset                            +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB                    +*/
/*+                      LSA_UINT16                     ConsumerID              +*/
/*+                                                                             +*/
/*+  Result         :    LSA_UINT16                                             +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  ConsumerID     : selects the Consumer                                      +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function reads the shifted TimeScoreBoard offset from    +*/
/*+               the Consumer. This value can be used to build CRT-CMD.        +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTGetShiftedTSBOffset(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT16                  ConsumerID)
{
    EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer;

    EDDP_ASSERT_FALSE(ConsumerID >= pDDB->CRT.CfgParam.ConsumerMaxNumber);
    pConsumer = &pDDB->CRT.Cons.pConsEntry[ConsumerID];

    /* NOTE: offset returned is 16 bit wide, this is only ok because TSB entries are 64 bit aligned!!!! */
    return ( (LSA_UINT16)EDDP_CRTGetPnipAddrShift (pDDB, (EDD_COMMON_MEM_PTR_TYPE) pConsumer->pHwTSBApi, EDDP_CRT_SHIFT_VALUE_3) );
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTConsIsProvAutoStopEnabled                      +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB                    +*/
/*+                      LSA_UINT16                     ConsumerID              +*/
/*+                                                                             +*/
/*+  Result         :    LSA_BOOL                                               +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  ConsumerID     : selects the Consumer                                      +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function reads "ProvAutoStopEnabled" from the Consumer.  +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTConsIsProvAutoStopEnabled(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT16                  ConsumerID)
{
    EDDP_CRT_CONSUMER_PTR_TYPE  pConsumer;

    EDDP_ASSERT_FALSE(ConsumerID >= pDDB->CRT.CfgParam.ConsumerMaxNumber);

    pConsumer = &pDDB->CRT.Cons.pConsEntry[ConsumerID];

    if (pConsumer->ConsumerFlags & EDDP_CONS_FLAG_PROV_AUTOSTOP_ENABLED)
    {
        return (LSA_TRUE);
    }
    return (LSA_FALSE);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTConsExistFrameID                               +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB                    +*/
/*+                      LSA_UINT16                     FrameID                 +*/
/*+                      LSA_BOOL                       IgnoreDfp               +*/
/*+                                                                             +*/
/*+  Result         :    LSA_BOOL                                               +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  FrameID        : FrameID for searching in current existing consumers       +*/
/*+  IgnoreDfp      : TRUE=ignore DFP consumer, FALSE=check all consumer        +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function scan existing Consumers for given FrameID.      +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTConsExistFrameID(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT16                  FrameID,
    LSA_BOOL                    IgnoreDfp)
{
    EDDP_CRT_CONSUMER_PTR_TYPE  pCons;
    LSA_UINT16                  CurrIdx;
    LSA_BOOL                    IsDfp;

    if (pDDB->CRT.Cons.ConsumerUsedCnt)
    {
        // step throgh consumer list
        pCons   = pDDB->CRT.Cons.pConsEntry;
        CurrIdx = pDDB->CRT.Cons.ConsumerUsedList.BeginIdx;

        while (CurrIdx!=EDDP_CRT_CHAIN_IDX_END)
        {
            EDDP_ASSERT_FALSE(CurrIdx >= pDDB->CRT.CfgParam.ConsumerMaxNumber);

            if (pCons[CurrIdx].State != EDDP_CRT_STATE_FREE)
            {
                // consumer is used
                IsDfp = ( (pCons[CurrIdx].ConsumerFlags & EDDP_CONS_FLAG_IS_DFP_ITEM) ? LSA_TRUE : LSA_FALSE );

                if ((IgnoreDfp) && (IsDfp))
                {
                    // ignore this DFP consumer if filtered by funtion parameter
                }
                else
                {
                    if (pCons[CurrIdx].Param.FrameID == FrameID)
                    {
                        // found the FrameID in an used non DFP consumer
                        return (LSA_TRUE);
                    }
                }
            }
            CurrIdx = pCons[CurrIdx].UsedFreeLink.Next;
        }
    }

    return (LSA_FALSE);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTConsumerFirstActivateControlReq                +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE                    pDDB        +*/
/*+                      EDD_UPPER_CSRT_CONSUMER_CONTROL_PTR_TYPE   pRqbParam   +*/
/*+                                                                             +*/
/*+  Result         :    LSA_VOID                                               +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  pRqbParam      : specific part of the RQB from EDDP_CRTConsumerControl()   +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function updates UNDEF value on first ACTIVATE.          +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerFirstActivateControlReq(
    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB,
    EDD_UPPER_CSRT_CONSUMER_CONTROL_PTR_TYPE    pRqbParam)
{
    EDDP_CRT_CONSUMER_PTR_TYPE          pConsumer;
    EDDP_CRT_CONSUMER_PARAM_PTR_TYPE    pParamConsumer;
    LSA_UINT16                          ConsumerID = pRqbParam->ConsumerID;

    EDDP_ASSERT_FALSE(ConsumerID >= pDDB->CRT.CfgParam.ConsumerMaxNumber);

    pConsumer       = &pDDB->CRT.Cons.pConsEntry[ConsumerID];
    pParamConsumer  = &pConsumer->Param;

    // - Partial_DataLen
    // - Partial_DataOffset
    if (!pParamConsumer->Partial_DataLen)
    {   // ***** no partial access --> set full data lenght
        pParamConsumer->Partial_DataOffset  = 0;
        pParamConsumer->Partial_DataLen     = pParamConsumer->DataLen;
    }

    // CycleReductionRatio used in EDDP for TSBRR-Reload within the TimerScoreBoard !!!
    if (pConsumer->Param.CycleReductionRatio == EDD_CYCLE_REDUCTION_RATIO_UNDEFINED)
    {
        pConsumer->Param.CycleReductionRatio = pRqbParam->CycleReductionRatio;
    }

    // - CyclePhase          (not used in EDDP !!!)
    // - DataOffset
    if (pParamConsumer->DataOffset == EDD_DATAOFFSET_UNDEFINED)
    {   // ***** update if not set in CONSUMER_ADD
        pParamConsumer->DataOffset = pRqbParam->IOParams.DataOffset;
    }
    // - ForwardMode + CutThrough_OutDataStart
    if (pParamConsumer->ForwardMode == EDD_FORWARDMODE_UNDEFINED)
    {   // ***** update if not set in CONSUMER_ADD
        pParamConsumer->ForwardMode             = pRqbParam->IOParams.ForwardMode;
        pParamConsumer->CutThrough_OutDataStart = pRqbParam->IOParams.CutThrough_OutDataStart;
		/* ------------------------------------------------------------------------ */
		/* is this Consumer with CutThrough-Mode ?                                  */
		/* ------------------------------------------------------------------------ */
        if (pConsumer->Param.ForwardMode == EDD_FORWARDMODE_CT)
        {   // ***** save ConsumerID as CutThrough-Consumer
            if ( pDDB->CRT.Cons.CT_ConsumerID == EDDP_CRT_CHAIN_IDX_END )
            {
                pDDB->CRT.Cons.CT_ConsumerID = ConsumerID;
            }
        }
    }

    // - CRNumber
    if (pParamConsumer->CRNumber == EDD_CRNUMBER_UNDEFINED)
    {   // ***** update if not set in CONSUMER_ADD
        pParamConsumer->CRNumber = pRqbParam->IOParams.CRNumber;
    }

    // - AppGroup
    if (pParamConsumer->AppGroup == EDD_APPGROUP_UNDEFINED)
    {   // ***** update if not set in CONSUMER_ADD
        pParamConsumer->AppGroup = pRqbParam->IOParams.AppGroup;
    }

    // ***** WatchdogFactor == DataHoldFactor --> we can use one of them
    if (pParamConsumer->DataHoldFactor > EDDP_CRT_CONSUMER_DHT_MAX_SW)
    {   // ***** DHT: 254/255 --> DHT/2+1
        // ***** CRR: 1..128  --> CRR*2-1
        pParamConsumer->HwDataHoldFactor       = (LSA_UINT8)((pParamConsumer->DataHoldFactor>>1)+1);
        pParamConsumer->HwCycleReductionRatio  = ((pParamConsumer->CycleReductionRatio<<1)-1);
    }
    else
    {   // ***** DHT: 3..253 --> 4..254
        // ***** CRR: 1..128 --> 0..127
        pParamConsumer->HwDataHoldFactor       = (LSA_UINT8)(pParamConsumer->DataHoldFactor+1);
        pParamConsumer->HwCycleReductionRatio  = pParamConsumer->CycleReductionRatio-1;
    }

    // activate MediaRedFactor if MRPD
    if (EDDP_IS_IRT_REDUNDANT_FRAMEID(pConsumer->Param.FrameID))
    {
        // check if FrameID(x+1) exists in PDIRData_A
        if (EDDP_PRMPDIRDataIsRedundantFrameIDCoupleAvailable(pDDB, pConsumer->Param.FrameID, LSA_TRUE))
        {
            // MRPD: set MediaRedFactor < EDDP_CRT_CONSUMER_MRT_OFF
            switch (EDDP_HALGetPnipRevision(pDDB))
            {
                #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
                case EDDP_HAL_HW_PNIP_REV1:
                {
                    // MediaRedFactor = 3 x DHT or MAX
                    pConsumer->Param.MediaRedFactor = (3 * pParamConsumer->DataHoldFactor) + 1;

                    if (pConsumer->Param.MediaRedFactor > EDDP_CRT_CONSUMER_MRT_MAX_SW)
                    {
                        // patch to MAX
                        pConsumer->Param.MediaRedFactor = EDDP_CRT_CONSUMER_MRT_MAX_SW;
                    }
                }
                break;
                #endif

                #if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
                case EDDP_HAL_HW_PNIP_REV2:
                case EDDP_HAL_HW_PNIP_REV3:
                {
                    // MediaRedFactor is constant
                    pConsumer->Param.MediaRedFactor = EDDP_CRT_CONSUMER_MRT_PNIP_REV2;
                }
                break;
                #endif
                        
                default:
                {
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTConsumerFirstActivateControlReq(): FATAL -> Version(%d) of PNIP is unknown", 
                        EDDP_HALGetPnipRevision(pDDB));
                    EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                }
                break;
            }
        }
        else
        {
            // special multicast only
            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTConsumerFirstActivateControlReq(ConsumerID=%d, FrameID=0x%X): no FrameID(x+1) in PDIRData!", 
                ConsumerID, pConsumer->Param.FrameID);
        }
    }

    EDDP_CRT_TRACE_09(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTConsumerFirstActivateControlReq() Info: CycleBaseFactor=%d, ConsumerID=%d, FrameID=0x%X, CRNr=%d, RR=%d, DHT=%d, HW_RR=%d, HW_DHT=%d, MRT=%d", 
        pDDB->SWI.CycleInfo.CycleBaseFactor, 
        ConsumerID, 
        pConsumer->Param.FrameID, 
        pConsumer->Param.CRNumber, 
        pConsumer->Param.CycleReductionRatio, 
        pConsumer->Param.DataHoldFactor, 
        pParamConsumer->HwCycleReductionRatio, 
        pParamConsumer->HwDataHoldFactor, 
        pConsumer->Param.MediaRedFactor);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTConsumerDoAsynAcwInCmd                         +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB                    +*/
/*+                      LSA_UINT16                     AcwID                   +*/
/*+                      EDDP_CMD_CBF_PTR_TYPE          pCbPtr                  +*/
/*+                                                                             +*/
/*+  Result         :    EDD_RSP                                                +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  AcwID          : selects the Consumer (or Container ACW)                   +*/
/*+  pCbPtr         : valid callback pointer (no check !!!!)                    +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function executes an asynchronous cmd ACW_IN.            +*/
/*+               For ACW container the DgOffset will be set here!              +*/
/*+                                                                             +*/
/*+               This function initializes the ACW and insert it into          +*/
/*+               RBaseSWList.                                                  +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerDoAsynAcwInCmd(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID,
    EDDP_CMD_CBF_PTR_TYPE       pCbPtr)
{
    if (AcwID < pDDB->CRT.CfgParam.ConsumerMaxNumber)
    {   // ***** init this ACW in HW (AcwID --> ConsumerID)
        pDDB->CRT.Cons.AcwHandler.FctInitAcwHwApi (pDDB, AcwID);
    }
    else
    {   // ***** init this container ACW in HW (AcwID - MaxConsumer = ContainerIdx)
        pDDB->CRT.Cons.AcwHandler.FctInitContainerAcwHwApi (pDDB, AcwID);
    }

    /* ------------------------------------------------------------------------ */
    /* add new consumer to CRT.Cons.pRBaseEntry[RBaseIndex]                     */
    /* ------------------------------------------------------------------------ */
    pDDB->CRT.Cons.RBaseHandler.FctAddAcwToRBase(pDDB, AcwID);

    // ***** the given Cbf will be called when command is finished
    return (EDDP_CRTCmdRequestCpm(pDDB, EDDP_CRT_CMD_CPM_ACW_IN, AcwID, pCbPtr));
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTConsumerDoAsynAcwOutCmd                        +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE                   pDDB         +*/
/*+                      LSA_UINT16                                AcwID        +*/
/*+                      EDDP_CMD_CBF_PTR_TYPE                     pCbPtr       +*/
/*+                                                                             +*/
/*+  Result         :    EDD_RSP                                                +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  AcwID          : selects the Consumer (or Container ACW)                   +*/
/*+  pCbPtr         : valid callback pointer (no check !!!!)                    +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function executes an asynchronous cmd ACW_OUT.           +*/
/*+                                                                             +*/
/*+               Use this function initializes the asynchronous command only.  +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerDoAsynAcwOutCmd(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID,
    EDDP_CMD_CBF_PTR_TYPE       pCbPtr)
{
    return (EDDP_CRTCmdRequestCpm(pDDB, EDDP_CRT_CMD_CPM_ACW_OUT, AcwID, pCbPtr));
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTConsumerDoAsynAcwOnOffCmd                      +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB                    +*/
/*+                      LSA_UINT16                     AcwID                   +*/
/*+                      LSA_BOOL                       SetOn                   +*/
/*+                      EDDP_CMD_CBF_PTR_TYPE          pCbPtr                  +*/
/*+                                                                             +*/
/*+  Result         :    EDD_RSP                                                +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  AcwID          : selects the Consumer (or Container ACW)                   +*/
/*+  SetOn          : LSA_FALSE=OFF, LSA_TRUE=ON                                +*/
/*+  pCbPtr         : valid callback pointer (no check !!!!)                    +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function executes an asynchronous cmd ACW_ON/OFF.        +*/
/*+                                                                             +*/
/*+               Use this function initializes the asynchronous command only.  +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerDoAsynAcwOnOffCmd(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID,
    LSA_BOOL                    SetOn,
    EDDP_CMD_CBF_PTR_TYPE       pCbPtr)
{
    if (SetOn)
    {
        return (EDDP_CRTCmdRequestCpm(pDDB, EDDP_CRT_CMD_CPM_ACW_ON, AcwID, pCbPtr));
    }

    return (EDDP_CRTCmdRequestCpm(pDDB, EDDP_CRT_CMD_CPM_ACW_OFF, AcwID, pCbPtr));
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTConsumerGetConsDgEntry                         +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB                    +*/
/*+                      EDDP_CRT_CONSUMER_PTR_TYPE     pConsumer               +*/
/*+                      LSA_UINT8                      SFPosition              +*/
/*+                                                                             +*/
/*+  Result         :    LSA_UINT16 (index or EDDP_CRT_CHAIN_IDX_END on error)  +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  pConsumer      : pointer to the selected Consumer                          +*/
/*+  SFPosition     : search criteria (also: FrameID)                           +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function looks for right consumer DG (PosNr).            +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerGetConsDgEntry(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer,
    LSA_UINT8                       SFPosition)
{
    EDDP_CRT_CONSUMER_CONTAINER_PTR_TYPE    pConsContainerEntry;
    EDDP_CRT_CONSUMER_DG_PTR_TYPE           pConsDgEntry;
    LSA_UINT16                              Idx, End, FrameID;

    EDDP_ASSERT_NO_NULL_PTR(pConsumer);

    FrameID         = pConsumer->Param.FrameID;
    End             = pDDB->CRT.CfgParam.ConsumerDGMaxNumber;
    pConsDgEntry    = pDDB->CRT.Cons.pConsDgEntry;

    for ( Idx=0; Idx < End; Idx++ )
    {
        if (pConsDgEntry[Idx].DgGrpIdx == EDDP_CRT_CHAIN_IDX_END) 
            continue;   // not assigned to a container
        if (pConsDgEntry[Idx].pSubFrameData == LSA_NULL) 
            continue;   // not configured by PRM
        pConsContainerEntry = &pDDB->CRT.Cons.pConsContainerEntry[pConsDgEntry[Idx].DgGrpIdx];
        if (pConsContainerEntry->ConsAcwTypeInfo.FrameID != FrameID)
            continue;   // missmatch FrameID
        if (pConsDgEntry[Idx].pSubFrameData->Position == SFPosition)
        {   // ***** matching --> return Idx of pConsDgEntry[?]
            return Idx;
        }
    }

    return (EDDP_CRT_CHAIN_IDX_END);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTConsumerConnectToConsDgEntry                   +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB                    +*/
/*+                      EDDP_CRT_CONSUMER_PTR_TYPE     pConsumer               +*/
/*+                      LSA_UINT16                     DgIdx                   +*/
/*+                                                                             +*/
/*+  Result         :    LSA_VOID                                               +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  pConsumer      : Pointer to the selected Consumer                          +*/
/*+  DgIdx          : Index of DgEntry that must be added                       +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function looks for right consumer DG (PosNr) and builds  +*/
/*+               the connection (index mapping)                                +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerConnectToConsDgEntry(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer,
    LSA_UINT16                      DgIdx)
{
    EDDP_CRT_CONSUMER_DG_PTR_TYPE   pConsDgEntry;

    EDDP_ASSERT_NO_NULL_PTR(pConsumer);
    EDDP_ASSERT_FALSE(DgIdx >= pDDB->CRT.CfgParam.ConsumerDGMaxNumber);

    // do connection betwenn consumer and DG
    pConsDgEntry                        = &pDDB->CRT.Cons.pConsDgEntry[DgIdx];
    pConsDgEntry->ConnectedConsumerId   = pConsumer->ConsumerId;    // set link to SW consumer entry
    pConsumer->DgIdx                    = DgIdx;                    // bind the DG to the SW consumer
    pConsumer->DgGrpIdx                 = pConsDgEntry->DgGrpIdx;   // store container ACW idx
    pConsumer->State                    = EDDP_CRT_STATE_HW_PASSIVE;

    pDDB->CRT.Cons.AcwHandler.FctFinalInitAcwDgHwApi (pDDB, pConsumer, pConsDgEntry);    // update TSB reference and some other AcwDg-Entries
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTConsumerDisconnectFromConsDgEntry              +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB                    +*/
/*+                      EDDP_CRT_CONSUMER_PTR_TYPE     pConsumer               +*/
/*+                                                                             +*/
/*+  Result         :    LSA_VOID                                               +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  pConsumer      : pointer to the selected Consumer                          +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function releases the connection betwenn Consumer and DG.+*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerDisconnectFromConsDgEntry(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer)
{
    EDDP_CRT_CONSUMER_DG_PTR_TYPE   pConsDgEntry;

    EDDP_ASSERT_NO_NULL_PTR(pConsumer);
    // check the given consumer is DFP
    EDDP_ASSERT_FALSE( !(pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_DFP_ITEM) );
    // check this consumer is still connected to DG
    EDDP_ASSERT_FALSE(pConsumer->DgIdx >= pDDB->CRT.CfgParam.ConsumerDGMaxNumber);

    // this consumer and DG have no more connection
    pConsDgEntry                        = &pDDB->CRT.Cons.pConsDgEntry[pConsumer->DgIdx];
    pConsDgEntry->ConnectedConsumerId   = EDDP_CRT_CHAIN_IDX_END;
    pConsumer->DgIdx                    = EDDP_CRT_CHAIN_IDX_END;
    pConsumer->DgGrpIdx                 = EDDP_CRT_CHAIN_IDX_END;

    // this consumer is still used in SW
    pConsumer->State = EDDP_CRT_STATE_SW_USED;
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTConsumerEventForProvAutoStop                   +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                   +*/
/*+                      EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer              +*/
/*+                      LSA_UINT16                      Mode                   +*/
/*+                      LSA_BOOL                        bPendingProvAutoStopOn +*/
/*+                                                                             +*/
/*+  Result         :    LSA_VOID                                               +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  pConsumer      : pointer to the selected Consumer                          +*/
/*+  Mode           : Mode parameter from the RQB --> EDDP_CRTConsumerControl() +*/
/*+  bPendingProvAutoStopOn: control flag for this funtion how the event        +*/
/*+                          indication is to be generated (now, delayed, no)   +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function handles the generation of the event indiction.  +*/
/*+                                                                             +*/
/*+               Attention: Setting of the flag "PendingProvAutoStopOn" must   +*/
/*+               be synchronous with calling the cmd ACW_ON in                 +*/
/*+               EDDP_CRTConsumerDoControlReq()                                +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerEventForProvAutoStop(
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB,
    EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer,
    LSA_UINT16                      Mode,
    LSA_BOOL                        bPendingProvAutoStopOn)
{
    LSA_BOOL        bFireEvent = LSA_FALSE;

    EDDP_ASSERT_NO_NULL_PTR(pConsumer);

    if (bPendingProvAutoStopOn)
    {   // ***** fire the indication now after ACW_ON cmd is finished --> AUTOSTOP_ENABLE
        if (Mode & EDD_CONS_CONTROL_MODE_PROV_AUTOSTOP_ENABLE)
        {
            pConsumer->ConsumerFlags |= (EDDP_CONS_FLAG_PROV_AUTOSTOP_ENABLED);     // enable autostop
            pConsumer->ConsumerFlags &= ~(EDDP_CONS_FLAG_PENDING_PROV_AUTOSTOP_ON); // pending indication will be fired now --> clr flag
            bFireEvent = LSA_TRUE;
        }
    }
    else
    {   
        if (Mode & EDD_CONS_CONTROL_MODE_PROV_AUTOSTOP_DISABLE)
        {   // ***** call before ACW_OFF cmd --> AUTOSTOP_DISABLE
            pConsumer->ConsumerFlags &= ~(EDDP_CONS_FLAG_PROV_AUTOSTOP_ENABLED);    // disable autostop
            bFireEvent = LSA_TRUE;
        }
        else
        {
            if (Mode & EDD_CONS_CONTROL_MODE_PROV_AUTOSTOP_ENABLE)
            {   // ***** test when the indication must be generated
                if (Mode & EDD_CONS_CONTROL_MODE_ACTIVATE)
                {   // ***** check if ACW_ON will be executed now
                    switch(pConsumer->State)
                    {
                        case EDDP_CRT_STATE_HW_PASSIVE:
                            // ***** set flag for delayed indication after ACW_ON cmd
                            pConsumer->ConsumerFlags |= (EDDP_CONS_FLAG_PENDING_PROV_AUTOSTOP_ON);
                            break;
                        case EDDP_CRT_STATE_HW_ACTIVE:
                            // ***** call direct because the ACW is still active and no ACW_ON will be executed
                            pConsumer->ConsumerFlags |= (EDDP_CONS_FLAG_PROV_AUTOSTOP_ENABLED);     // enable autostop
                            bFireEvent = LSA_TRUE;
                            break;
                        default:
                            break;
                    
                    }
                }
                else
                {   // ***** fire the idication if ACW_ON will not be called and the ACW is used in SW or SW+HW
                    if (pConsumer->State != EDDP_CRT_STATE_FREE)
                    {   // ***** consumer exist --> fire event now
                        pConsumer->ConsumerFlags |= (EDDP_CONS_FLAG_PROV_AUTOSTOP_ENABLED);         // enable autostop
                        bFireEvent = LSA_TRUE;
                    }
                }
            }
        }
    }

    if (bFireEvent)
    {
        if (pConsumer->ConsumerFlags & EDDP_CONS_FLAG_PROV_AUTOSTOP_ENABLED)
        {
            EDDP_CRTProviderAutoStopControl(pDDB, pConsumer->ConsumerId, LSA_TRUE);
        }
        else
        {
            EDDP_CRTProviderAutoStopControl(pDDB, pConsumer->ConsumerId, LSA_FALSE);
        }
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTConsumerDoControlReq                           +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB                    +*/
/*+                      EDD_UPPER_RQB_PTR_TYPE         pRQB                    +*/
/*+                                                                             +*/
/*+  Result         :    EDD_RSP                                                +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  pRQB           : points to the RQB from the EDDP_CRTConsumerControl()      +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function executes the CONTROL request given with pRQB.   +*/
/*+                                                                             +*/
/*+               The function can be used for delayed request.                 +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTConsumerDoControlReq(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_UPPER_RQB_PTR_TYPE      pRQB)
{
    EDD_RSP                                     Response;
    LSA_UINT64                                  HW_Word64;
    LSA_UINT32                                  AuxTime, PreMul;
    LSA_UINT32                                  HwCycleReductionRatio;
    EDDP_CRT_CONSUMER_PTR_TYPE                  pConsumer;
    EDD_UPPER_CSRT_CONSUMER_CONTROL_PTR_TYPE    pRqbParam;

    Response    = EDD_STS_OK;
    pRqbParam   = (EDD_UPPER_CSRT_CONSUMER_CONTROL_PTR_TYPE) pRQB->pParam;
    EDDP_ASSERT_FALSE(pRqbParam->ConsumerID >= pDDB->CRT.CfgParam.ConsumerMaxNumber);
    pConsumer   = &pDDB->CRT.Cons.pConsEntry[pRqbParam->ConsumerID];

    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTConsumerDoControlReq(ConsumerID=%d): Consumer-State=0x%X, Mode=0x%X", 
        pRqbParam->ConsumerID, pConsumer->State, pRqbParam->Mode);

    /* ------------------------------------------------ */
    /* is provoder autostop feature  ?                  */
    /* ------------------------------------------------ */
    // ***** Note: The function sets the flag "PendingProvAutoStopOn" internally
    //             If the flag will be set to one, then must the cmd ACW_ON be called here bellow
    EDDP_CRTConsumerEventForProvAutoStop (pDDB, pConsumer, pRqbParam->Mode, LSA_FALSE);

    /* ------------------------------------------------ */
    /* update SrcMAC, SrcIP only if current consumer    */
    /* state is passive and (Mode=ACTIVATE)             */
    /* ------------------------------------------------ */
    if ((pConsumer->State != EDDP_CRT_STATE_HW_ACTIVE) && (pRqbParam->Mode & EDD_CONS_CONTROL_MODE_ACTIVATE))
    {   
        if (pConsumer->Param.RTClass == EDDP_CRT_RTCLASS_UDP)
        {
            pConsumer->Param.SrcIP = pRqbParam->SrcIP;
        }
        if (pConsumer->Param.RTClass == EDDP_CRT_RTCLASS_RT)
        {
            pConsumer->Param.SrcMAC = pRqbParam->SrcMAC;
        }
        // ***** note: RAM in ApiCtrl is reserved always if state > EDDP_CRT_STATE_FREE
        pDDB->CRT.Cons.AcwHandler.FctUpdateSrcAddrCsaAcwHwApi (pDDB, pRqbParam->ConsumerID);
    }

    /* ------------------------------------------------ */
    /* is Passivate requested ?                         */
    /* ------------------------------------------------ */
    if (Response == EDD_STS_OK)
    {
        if (pRqbParam->Mode & EDD_CONS_CONTROL_MODE_PASSIVATE)
        {
            pConsumer->ConsumerFlags &= ~EDDP_CONS_FLAG_AGAIN_RCVD;  // clear AGAIN information
            // is SRD-Consumer ?
            if (pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_SRD_ITEM)
            {   // ***** patching DataStatus to BACKUP within APICtrl-RAM
                EDDP_CRTConsForceBackupDataStatusHwApi (pDDB, pConsumer);
            }

            if (pConsumer->State == EDDP_CRT_STATE_HW_ACTIVE)
            {
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTConsumerDoControlReq(): MODE_PASSIVATE");

                /* ---------------------------- */
                /* deactivate command           */
                /* ---------------------------- */
                // ***** call CmdRequest ASYNCHRON --> expect result when the Cbf EDDP_CRTCmdDoneCbfCpm() will be called
                // returns EDD_STS_OK_PENDING
                Response = EDDP_CRTCmdRequestCpm (pDDB, EDDP_CRT_CMD_CPM_ACW_OFF, pRqbParam->ConsumerID, EDDP_CRTCmdDoneCbfCpm);
            }
        }
    }

    /* ------------------------------------------------ */
    /* is Activate/SetToUnknown requested ?             */
    /* ------------------------------------------------ */
    if (Response == EDD_STS_OK)
    {
        if ((pRqbParam->Mode & EDD_CONS_CONTROL_MODE_ACTIVATE) && (pConsumer->State == EDDP_CRT_STATE_HW_PASSIVE))
        {   // ***** activate the Consumer-ACW 
            /* ---------------------------- */
            /* activate command             */
            /* ---------------------------- */

            // ***** Setup the Scoreboardentries within PNIP and variables within Consumer manamgement
            EDDP_CRTScoreboardSetupEntry (pDDB, pConsumer);  

            // Initialization of APDU-TransferStatus for system redundancy (only for Not-DFP-Consumer APDU-Status in frame)
            if (!(pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_DFP_ITEM))
            {
                EDDP_HAL_MEM8_WRITE(pDDB, &pConsumer->ConsAcwTypeInfo.pHwApduApi->TransferStatus, 0x00);
            }

            if (pRqbParam->Mode & EDD_CONS_CONTROL_MODE_SET_TO_UNKNOWN)
            {   // ***** EDDP_CRTScoreboardSetupEntry() clears pConsumer->ScoreBoard.WaitSetToUnknown !!!
                // without ScoreBoardChange, clr indicated AGAIN, MISS & MISS_NOT_STOPPED
                EDDP_CRTSetConsumerToUnknown (pDDB, pConsumer, LSA_FALSE);
            }

            // ***** run after initialization of TSB --> EDDP_CRTScoreboardSetupEntry() !!!
            if (pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_2CR_MODE)
            {   // ***** on activating the AUX consumer set the GREEN detection time and later the RED detection time
                // ***** clear the 'old' Dp2Only-Bit if set by a last received red frame
                //       It is necessary for CR2 consumer to enable green frames again !!!
                pDDB->CRT.Cons.AcwHandler.FctSetupLegacyStartupAcwHwApi(pDDB, pConsumer);
                
                HW_Word64 = EDDP_HAL_MEM64_READ(pDDB, &pConsumer->pHwTSBApi->TSB_Word2);
                // ***** calculate the detection time greather 256ms
                HwCycleReductionRatio = pConsumer->Param.HwCycleReductionRatio;
                // ***** CycleBaseFactor: e.g: 128    --> 128 * 31.25us = 4ms
                PreMul  = pDDB->SWI.CycleInfo.CycleBaseFactor * (pConsumer->Param.HwDataHoldFactor-1);
                AuxTime = HwCycleReductionRatio * PreMul;   // = SendClock * CycleReductionRatio * DHF
                // ***** Note: we need a time greather 256ms
                //             --> 8192 = 256000us/31.25us --> to avoid calculation with float !!!
                if (AuxTime < (LSA_UINT32)EDDP_CRT_REDUCTION_8192)
                {
                    HwCycleReductionRatio++;    // eleminate RR-1
                    do
                    {
                        HwCycleReductionRatio = (HwCycleReductionRatio<<1);
                        AuxTime = HwCycleReductionRatio * PreMul;   // = SendClock * CycleReductionRatio * DHF
                    }
                    while (AuxTime < (LSA_UINT32)EDDP_CRT_REDUCTION_8192);
                    HwCycleReductionRatio--;    // make from SW --> HW (RR-1)
                }
                // 125 >> 2 = 31,25 us  --> to avoid calculation with float !!!
                // AuxTime = (CycleBaseFactor*DataHoldFactor) * CycleReductionRatio * 31,25us
                // eg: 320.000 = (  128      *  20          ) *      4              * 125>>2
                AuxTime = (PreMul * (HwCycleReductionRatio+1) * 125)>>2;
                EDDP_SET_BIT_VALUE64(HW_Word64, HwCycleReductionRatio, PNIP_CPM_TSB_WORD2_TSBRR_RELOAD_MASK , PNIP_CPM_TSB_WORD2_TSBRR_RELOAD_SHIFT);
                EDDP_HAL_MEM64_WRITE(pDDB, &pConsumer->pHwTSBApi->TSB_Word2, HW_Word64);
                EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTConsumerDoControlReq(GREEN, 2CR) ConsID: %d, HwCycleReductionRatio: %d, Time:%d us)", 
                    pConsumer->ConsumerId, HwCycleReductionRatio, AuxTime);
            }

            if (pConsumer->ConsumerFlags & EDDP_CONS_FLAG_MISS_RCVD)
            {
                pConsumer->ConsumerFlags &= ~EDDP_CONS_FLAG_AGAIN_RCVD;  // clear AGAIN information
            }
            pConsumer->ConsumerFlags &= ~EDDP_CONS_FLAG_MISS_RCVD;  // clear MISS information for: MISS and MISS_NOT_STOPPED event generated by SW

            // ***** call CmdRequest ASYNCHRON --> expect result later when the Cbf EDDP_CRTCmdDoneCbfCpm() will be called
            // returns EDD_STS_OK_PENDING
            Response = EDDP_CRTCmdRequestCpm (pDDB, EDDP_CRT_CMD_CPM_ACW_ON, pRqbParam->ConsumerID, EDDP_CRTCmdDoneCbfCpm);
        }
        else
        {   // ***** SET_TO_UNKNOWN command if ACW exist within HW
            if ( (pRqbParam->Mode & EDD_CONS_CONTROL_MODE_SET_TO_UNKNOWN) )
            {   // ***** if ACW off --> create Miss-Event, if ACW on --> sent command to HW
                EDDP_CRTSetConsumerToUnknown (pDDB, pConsumer, LSA_TRUE);   // with ScoreBoardChange, create Miss-Event by SW if ACW off

                if (pConsumer->State == EDDP_CRT_STATE_HW_ACTIVE)
                {   // ***** send command SetToUnknown if ACW still active (on)
                    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTConsumerDoControlReq(): MODE_SET_TO_UNKNOWN as command" );

                    /* ---------------------------- */
                    /* SetUnknown command           */
                    /* ---------------------------- */
                    // ***** call CmdRequest ASYNCHRON --> expect result later when the Cbf EDDP_CRTCmdDoneCbfCpm() will be called
                    // returns EDD_STS_OK_PENDING
                    Response = EDDP_CRTCmdRequestCpm (pDDB, EDDP_CRT_CMD_CPM_ACW_SET_UNKNOWN, pRqbParam->ConsumerID, EDDP_CRTCmdDoneCbfCpm);
                }
            }
        }
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTTriggerConsumerControlFinish                   +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB                    +*/
/*+                                                                             +*/
/*+  Result         :    LSA_VOID                                               +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function contains some activities for finish of          +*/
/*+               ConsumerControl after ConsumerControl confirmation.           +*/
/*+                 -> after EDDP_GlbRequestFinish() of ConsumerControl-RQB !!! +*/
/*+                                                                             +*/
/*+               If "RequestFinishConsumerID" is valid, so some activities of  +*/
/*+               this consumer must be done here.                              +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTTriggerConsumerControlFinish(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CRTTriggerConsumerControlFinish(): RequestFinishConsumerID=%d", 
        pDDB->CRT.Cons.RequestFinishConsumerID);

    // are some activities requested ?
    if (pDDB->CRT.Cons.RequestFinishConsumerID != EDDP_CRT_CHAIN_IDX_END)
    {
        // delete this ConsumerID flag
        pDDB->CRT.Cons.RequestFinishConsumerID = EDDP_CRT_CHAIN_IDX_END;

        if (pDDB->CRT.Cons.ConsumerControlMode & EDD_CONS_CONTROL_MODE_SET_TO_UNKNOWN)
        {
            EDDP_CRTConsumerCheckIndication (pDDB);
        }
    }

    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTTriggerConsumerControlFinish()");
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTConsMrtTriggerAppearDiagAllActiveMRPDConsumer  +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                       +*/
/*+                      LSA_BOOL                    isPrmCommit                +*/
/*+                                                                             +*/
/*+  Result         :    LSA_VOID                                               +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  isPrmCommit    : LSA_TRUE  : the call is from PrmCommit                    +*/
/*+                 : LSA_FALSE : the call is not from PrmCommit                +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: Trigger MRT diagnosis (Appear) for all active MRPD consumers  +*/
/*+               that have:                                                    +*/
/*+                 - MRT diagnosis is ON                                       +*/
/*+                 - MRT event is expired                                      +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTConsMrtTriggerAppearDiagAllActiveMRPDConsumer(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_BOOL                    isPrmCommit)
{
    LSA_UINT32                      Idx, End;
    LSA_UINT16                      CurrIdx, MaxCnt;
    EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer;

    End     = pDDB->CRT.Cons.ConsumerUsedCnt;
    MaxCnt  = pDDB->CRT.CfgParam.ConsumerMaxNumber;
    CurrIdx = pDDB->CRT.Cons.ConsumerUsedList.BeginIdx;

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTConsMrtTriggerAppearDiagAllActiveMRPDConsumer(isPrmCommit=0x%X): ConsumerUsedCnt=%d", 
        isPrmCommit, pDDB->CRT.Cons.ConsumerUsedCnt);

    for ( Idx=0; Idx < End; Idx++ )
    {
        if (CurrIdx > MaxCnt)
        {   // ***** EDDP_CRT_CHAIN_IDX_END or failure
            break;
        }

        pConsumer = &pDDB->CRT.Cons.pConsEntry[CurrIdx];

        // is this consumer active ?
        if (pConsumer->State == EDDP_CRT_STATE_HW_ACTIVE)
        {
            // is this RTC3 consumer with MRT diagnosis ON ?
            if ((pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_DP2ACW) && (pConsumer->MrtDiag.IsPrmMRPDWatchDogActive))
            {
                // this consumer must have redundant FrameID
                EDDP_ASSERT_FALSE(!(EDDP_IS_IRT_REDUNDANT_FRAMEID(pConsumer->Param.FrameID)));
                // this consumer must be connected to PRM -> PDIRData.PDIRFrameData[x]
                EDDP_ASSERT_FALSE(pConsumer->MrtDiag.PrmPDIRFrameDataIdx == EDDP_CRT_CHAIN_IDX_END);

                // trigger MRT diagnosis (Appear) for this consumer, if current MRT state is expired (Appear)
                if (pConsumer->MrtDiag.LastMrtExpired)
                {
                    EDDP_PRMDiagTriggerMRTDiagSingle (pDDB, pConsumer->MrtDiag.PrmPDIRFrameDataIdx, LSA_TRUE, LSA_FALSE, isPrmCommit);
                }
            }
        }

        CurrIdx = pConsumer->UsedFreeLink.Next;
    }
}
