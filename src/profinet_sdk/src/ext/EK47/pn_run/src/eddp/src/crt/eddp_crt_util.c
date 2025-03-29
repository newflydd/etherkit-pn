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
/*  F i l e               &F: eddp_crt_util.c                           :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  CRT (Cycle Real Time) for EDDP  -> Utilities                             */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who         What                                             */
#ifdef EDDP_MESSAGE
/*  11.11.08    Sergeev     Erstellung                                       */
#endif
/*****************************************************************************/


/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/

#define LTRC_ACT_MODUL_ID   26
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_CRT_UTIL */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"           /* eddp headerfiles */
#include "eddp_int.h"           /* internal header  */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTIndProvideFinishAll                             +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                        :    EDD_RSP                         ResponseFinish          +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ResponseFinish : Response for EDDP_GlbRequestFinish()                              +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: In this function all RQBs of CRT indication queue are finished.       +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTIndProvideFinishAll(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_RSP                     ResponseFinish)
{
    EDD_RSP                         Response;
    LSA_BOOL                        Ready;
    EDDP_CRT_INDPROVIDE_PTR_TYPE    pIndication;
    EDD_UPPER_RQB_PTR_TYPE          pRQB;
    EDDP_LOCAL_HDB_PTR_TYPE         pHDB;

    Response        = EDD_STS_OK;
    Ready           = LSA_FALSE;
    pIndication     = &pDDB->CRT.IndProvide;
    pRQB            = LSA_NULL;
    pHDB            = LSA_NULL;

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTIndProvideFinishAll(ResponseFinish=0x%X): RqbQueue.Count=%d", 
        ResponseFinish, pIndication->RqbQueue.Count);

    /* ---------------------------------------- */
    /* so long RQBs are available in queue      */
    /* ---------------------------------------- */
    while (!Ready)
    {
        // get first RQB from indication queue
        pRQB = EDDP_GlbRQBQueueRemoveFromBegin (&(pIndication->RqbQueue));

        if ( !(LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL)) )
        {
            // get Handle for this RQB
            Response = EDDP_GlbHandleGetHDB (EDD_RQB_GET_HANDLE_LOWER(pRQB), &pHDB);

            if (Response != EDD_STS_OK)
            {
	            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "EDDP_CRTIndProvideFinishAll(): Invalid handle lower in RQB!");
	            EDD_RQB_SET_RESPONSE(pRQB, EDD_STS_ERR_PARAM);
	            EDDP_RQB_ERROR(pRQB);
            }
            else
            {
                // and finish this request
                EDDP_GlbRequestFinish (pHDB, pRQB, ResponseFinish);
            }
        }
        else
        {
            // no (more) RQBs in queue
            Ready = LSA_TRUE;
        }
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTIndProvideFinishSingle                          +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                        +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_BOOL                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function indicates one single indication RQB.                    +*/
/*+                                                                                     +*/
/*+               Check if indication queue contains resource.                          +*/
/*+               If yes, check if an indications resource contains entries             +*/
/*+               and if yes indicate it to user.                                       +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTIndProvideFinishSingle(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP                                 Response;
    LSA_BOOL                                State;
    EDDP_DDB_CRT_PTR_TYPE                   pCRTMngm;
    EDD_UPPER_RQB_PTR_TYPE                  pIndRQB;
    EDD_UPPER_CSRT_IND_PROVIDE_PTR_TYPE     pIndParam;
    EDDP_LOCAL_HDB_PTR_TYPE                 pHDB;

    Response        = EDD_STS_OK;
    State           = LSA_FALSE;
    pCRTMngm        = &pDDB->CRT;
    pIndRQB         = pCRTMngm->IndProvide.RqbQueue.pFirst;  // first RQB in indication queue
    pHDB            = LSA_NULL;

    if (LSA_HOST_PTR_ARE_EQUAL(pIndRQB, LSA_NULL))
    {
        // no indication resource present
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] EDDP_CRTIndProvideFinishSingle(): No indication resource present in CRT!");
    }
    else
    {
        pIndParam = (EDD_UPPER_CSRT_IND_PROVIDE_PTR_TYPE) pIndRQB->pParam;

        if (LSA_HOST_PTR_ARE_EQUAL(pIndParam, LSA_NULL))
        {
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTIndProvideFinishSingle(): FATAL -> pIndRQB->pParam=NULL!");
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        else
        {
            EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTIndProvideFinishSingle(RqbQueue.Count=%d): The indication Rqb(0x%X) has %d entry", 
                pCRTMngm->IndProvide.RqbQueue.Count, pIndRQB, pIndParam->Count);

            // does this indication resource contain entries ?
            if (pIndParam->Count > 0)
            {
                State = LSA_TRUE;

                // remove this indication RQB from queue
                pIndRQB = EDDP_GlbRQBQueueRemoveFromBegin (&(pCRTMngm->IndProvide.RqbQueue));
                // get Handle for this indication RQB
                Response = EDDP_GlbHandleGetHDB (EDD_RQB_GET_HANDLE_LOWER(pIndRQB), &pHDB);

                if (Response != EDD_STS_OK)
                {
                    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "EDDP_CRTIndProvideFinishSingle: Invalid handle lower in RQB!");
                    EDD_RQB_SET_RESPONSE(pIndRQB, EDD_STS_ERR_PARAM);
                    EDDP_RQB_ERROR(pIndRQB);
                }
                else
                {
                    // fill DebugInfo of this indication RQB
                    EDDP_CRTFillDebugInfoMissIndication (pDDB, pIndParam);
                    // finish this indication RQB
                    EDDP_GlbRequestFinish (pHDB, pIndRQB, EDD_STS_OK);
                }
            }
        }
    }

    return (State);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTFillIndication                                  +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                        +*/
/*+                             LSA_UINT32                  UserID                      +*/
/*+                             LSA_UINT32                  Event                       +*/
/*+                             LSA_UINT32                  EventStatus                 +*/
/*+                             LSA_UINT16                  CycleCounter                +*/
/*+                             LSA_UINT32                  DebugInfo                   +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_BOOL                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  UserID         : UserID from add consumer/provider                                 +*/
/*+  Event          : Indication Event                                                  +*/
/*+  EventStatus    : Current Status for all Events (not for EDD_CSRT_PROV_EVENT_DROP)  +*/
/*+  CycleCounter   : Cycle Counter                                                     +*/
/*+  DebugInfo      : 4 bytes DebugInfo (not for EDD_CSRT_PROV_EVENT_DROP)              +*/
/*+                                                                                     +*/
/*+  EDD_RQB_CSRT_IND_PROVIDE_TYPE                                                      +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function fills first indication RQB (pRQB->pParam) of            +*/
/*+               indication queue with given parameters.                               +*/
/*+                                                                                     +*/
/*+               The function may be called multiple to fill more events in one RQB.   +*/
/*+                                                                                     +*/
/*+               If resource is full, finish this indication RQB here.                 +*/
/*+                                                                                     +*/
/*+               Return LSA_TRUE  : indication resource is present in CRT              +*/
/*+                      LSA_FALSE : indication resource is not present in CRT          +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTFillIndication(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT32                  UserID,
    LSA_UINT32                  Event,
    LSA_UINT32                  EventStatus,
    LSA_UINT16                  CycleCounter,
    LSA_UINT32                  DebugInfo)
{
    EDD_RSP                                 Response;
    LSA_BOOL                                State;
    LSA_UINT32                              MaskValue32;
    EDDP_DDB_CRT_PTR_TYPE                   pCRTMngm;
    EDD_UPPER_RQB_PTR_TYPE                  pIndRQB;
    EDD_UPPER_CSRT_IND_PROVIDE_PTR_TYPE     pIndParam;
    EDDP_LOCAL_HDB_PTR_TYPE                 pHDB;

    Response        = EDD_STS_OK;
    State           = LSA_TRUE;
    MaskValue32     = EDDP_CRT_INVALID_VALUE_32;
    pCRTMngm        = &pDDB->CRT;
    pIndRQB         = pCRTMngm->IndProvide.RqbQueue.pFirst;  // first RQB in indication queue
    pHDB            = LSA_NULL;

    EDDP_CRT_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTFillIndication(UserID=%d, Event=0x%X, EventStatus=0x%X, CycleCounter=%d, DebugInfo=0x%X): RqbQueue.Count=%d", 
        UserID, Event, EventStatus, CycleCounter, DebugInfo, pCRTMngm->IndProvide.RqbQueue.Count);

    if (LSA_HOST_PTR_ARE_EQUAL(pIndRQB, LSA_NULL))
    {
        // no indication resource in CRT
        State = LSA_FALSE;
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] EDDP_CRTFillIndication(): No indication resource in CRT!");
    }
    else
    {
        pIndParam = (EDD_UPPER_CSRT_IND_PROVIDE_PTR_TYPE) pIndRQB->pParam;

        if (LSA_HOST_PTR_ARE_EQUAL(pIndParam, LSA_NULL))
        {
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTFillIndication(): FATAL -> pIndRQB->pParam=NULL!");
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        else
        {
            // fill event parameter in indication resource
            pIndParam->Data[pIndParam->Count].UserID        = UserID;
            pIndParam->Data[pIndParam->Count].Event         = Event;
            pIndParam->Data[pIndParam->Count].EventStatus   = EventStatus;
            pIndParam->Data[pIndParam->Count].CycleCnt      = CycleCounter;
            pIndParam->Data[pIndParam->Count].DebugInfo[0]  = (LSA_UINT8) EDDP_GET_BIT_VALUE(DebugInfo, MaskValue32, 0);
            pIndParam->Data[pIndParam->Count].DebugInfo[1]  = (LSA_UINT8) EDDP_GET_BIT_VALUE(DebugInfo, MaskValue32, 8);
            pIndParam->Data[pIndParam->Count].DebugInfo[2]  = (LSA_UINT8) EDDP_GET_BIT_VALUE(DebugInfo, MaskValue32, 16);
            pIndParam->Data[pIndParam->Count].DebugInfo[3]  = (LSA_UINT8) EDDP_GET_BIT_VALUE(DebugInfo, MaskValue32, 24);
            pIndParam->Count++;

            if (pIndParam->Count >= EDD_CFG_CSRT_MAX_INDICATIONS)
            {
                // resource is full - indicate this RQB
                // remove this indication RQB from queue
                pIndRQB = EDDP_GlbRQBQueueRemoveFromBegin (&(pCRTMngm->IndProvide.RqbQueue));
                // get Handle for this indication RQB
                Response = EDDP_GlbHandleGetHDB (EDD_RQB_GET_HANDLE_LOWER(pIndRQB), &pHDB);

                if (Response != EDD_STS_OK)
                {
                    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "EDDP_CRTFillIndication(): Invalid handle lower in RQB!");
                    EDD_RQB_SET_RESPONSE(pIndRQB, EDD_STS_ERR_PARAM);
                    EDDP_RQB_ERROR(pIndRQB);
                }
                else
                {
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTFillIndication(): Rest-RqbQueue.Count=%d", 
                        pCRTMngm->IndProvide.RqbQueue.Count);

                    // fill DebugInfo of this indication RQB
                    EDDP_CRTFillDebugInfoMissIndication (pDDB, pIndParam);
                    // finish this indication RQB
                    EDDP_GlbRequestFinish (pHDB, pIndRQB, EDD_STS_OK);
                }
            }
        }
    }

    return (State);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTDeleteHwApiDDB                                  +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB       : Pointer to Device Description Block                                   +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function deletes all CRT container in APICtrl-RAM                +*/
/*+               of the device                                                         +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTDeleteHwApiDDB(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT16          MaxProvAcw;
    LSA_UINT16          MaxProvAcwDG;
    LSA_UINT32          RamSubApiCtrl_ProvAcw,      APICtrlRAM_Size_ProvAcw;        // EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_PROV_ACW
    LSA_UINT32          RamSubApiCtrl_ProvAcwDG,    APICtrlRAM_Size_ProvAcwDG;      // EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_PROV_ACW_DG
    LSA_UINT32          APICtrlRAM_FillSize;

    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTDeleteHwApiDDB()");

    MaxProvAcw      = pDDB->CRT.CfgParam.ProviderAcwMaxNumber;
    MaxProvAcwDG    = pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber;

    /* ------------------------------------------------ */
    /* init container                                   */
    /* ------------------------------------------------ */
    RamSubApiCtrl_ProvAcw       = EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_PROV_ACW;
    APICtrlRAM_Size_ProvAcw     = (MaxProvAcw * sizeof(EDDP_HAL_APICTRL_CRT_PROV_ACW));
    RamSubApiCtrl_ProvAcwDG     = EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_PROV_ACW_DG;
    APICtrlRAM_Size_ProvAcwDG   = (MaxProvAcwDG * sizeof(EDDP_HAL_APICTRL_CRT_PROV_ACW_DG));

    /* ------------------------------------------------ */
    /* Provider container in APICtrl-RAM                */
    /* ------------------------------------------------ */
    EDDP_HALMemset (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, RamSubApiCtrl_ProvAcw,   0, APICtrlRAM_Size_ProvAcw,   0);
    EDDP_HALMemset (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, RamSubApiCtrl_ProvAcwDG, 0, APICtrlRAM_Size_ProvAcwDG, 0);

    /* ------------------------------------------------ */
    /* Consumer/container-ACW + ACW-DGs in APICtrl-RAM  */
    /* ------------------------------------------------ */
    APICtrlRAM_FillSize = (pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmAllAcw * sizeof(EDDP_HAL_APICTRL_CRT_CONS_ACW));
    EDDP_HALMemset (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_CONS_ACW,        0, APICtrlRAM_FillSize,   0);

    APICtrlRAM_FillSize = (pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmAllAcw * sizeof(EDDP_HAL_APICTRL_CRT_CONS_APDU));
    EDDP_HALMemset (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_CONS_APDU,       0, APICtrlRAM_FillSize,   0);

    APICtrlRAM_FillSize = (pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmAcwDG * sizeof(EDDP_HAL_APICTRL_CRT_CONS_ACW_DG));
    EDDP_HALMemset (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_CONS_ACW_DG,     0, APICtrlRAM_FillSize,   0);

    APICtrlRAM_FillSize = (pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmAcwDG * sizeof(EDDP_HAL_APICTRL_CRT_CONS_DG_STATUS));
    EDDP_HALMemset (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_CONS_DG_STATUS,  0, APICtrlRAM_FillSize,   0);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTFillDebugInfoError                  +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             LSA_UINT32                  ModuleId        +*/
/*+                             LSA_UINT32                  LineNr          +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to RQB                                            +*/
/*+  ModuleId   : ModuleId used with error                                  +*/
/*+  LineNr     : LineNr   used with error                                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Fill error DebugInfo in CRT services                      +*/
/*+                - PROVIDER_CONTROL                                       +*/
/*+                - CONSUMER_CONTROL                                       +*/
/*+               with aditional information like the ModuleId and LineNr.  +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#ifdef EDDP_CFG_DETAIL_ERROR_INFO
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTFillDebugInfoError(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    LSA_UINT32                  ModuleId,
    LSA_UINT32                  LineNr)
{
    EDD_RSP                         Response;
    EDD_SERVICE                     Service;
    LSA_UINT16                      MaskValue16;
    LSA_UINT32                      MaskValue32;
    EDD_UPPER_MEM_U8_PTR_TYPE       pDebugInfo;

    Response        = EDD_RQB_GET_RESPONSE(pRQB);
    Service         = EDD_RQB_GET_SERVICE(pRQB);
    MaskValue16     = EDDP_CRT_INVALID_VALUE_16;
    MaskValue32     = EDDP_CRT_INVALID_VALUE_32;

    switch (Service)
    {
	    case EDD_SRV_SRT_PROVIDER_CONTROL:
	    case EDD_SRV_SRT_CONSUMER_CONTROL:
        {
            /* ---------------------------------------- */
            /* Set pointer to begin of DebugInfo        */
            /* ---------------------------------------- */
            if (Service == EDD_SRV_SRT_PROVIDER_CONTROL)
            {
                pDebugInfo = (EDD_UPPER_MEM_U8_PTR_TYPE) &(((EDD_UPPER_CSRT_PROVIDER_CONTROL_PTR_TYPE)pRQB->pParam)->DebugInfo[0]);
            }
            else
            {
                pDebugInfo = (EDD_UPPER_MEM_U8_PTR_TYPE) &(((EDD_UPPER_CSRT_CONSUMER_CONTROL_PTR_TYPE)pRQB->pParam)->DebugInfo[0]);
            }

            /* ---------------------------------------- */
            /* Fill DebugInfo with error                */
            /* ---------------------------------------- */
            // Byte(0)      : Ident
            pDebugInfo[0] = EDD_DEBUGINFO_CONTROL_IDENT_EDDP;
            // Byte(2-3)    : Response
            pDebugInfo[2] = (LSA_UINT8) EDDP_GET_BIT_VALUE(Response, MaskValue16, 8);
            pDebugInfo[3] = (LSA_UINT8) EDDP_GET_BIT_VALUE(Response, MaskValue16, 0);
            // Byte(4-5)    : ModuleId
            pDebugInfo[4] = (LSA_UINT8) EDDP_GET_BIT_VALUE(ModuleId, MaskValue32, 8);
            pDebugInfo[5] = (LSA_UINT8) EDDP_GET_BIT_VALUE(ModuleId, MaskValue32, 0);
            // Byte(6-7)    : LineNr
            pDebugInfo[6] = (LSA_UINT8) EDDP_GET_BIT_VALUE(LineNr, MaskValue32, 8);
            pDebugInfo[7] = (LSA_UINT8) EDDP_GET_BIT_VALUE(LineNr, MaskValue32, 0);

            /* ---------------------------------------- */
            /* Trace error DebugInfo (16 byte)          */
            /* ---------------------------------------- */
            EDDP_CRT_TRACE_05(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTFillDebugInfoError(pRQB=0x%X, ModuleId=%d, LineNr=%d): Service=0x%X, Response=0x%X", 
                pRQB, ModuleId, LineNr, Service, Response);
            // do not delete the empty spaces in this trace text
            EDDP_CRT_TRACE_07(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--]    ErrorDebugInfo = 0x%X 0x00 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00", 
                pDebugInfo[0], pDebugInfo[2], pDebugInfo[3], pDebugInfo[4], pDebugInfo[5], pDebugInfo[6], pDebugInfo[7]);
        }
		break;

        default:
            // nothing to do
		break;
    }
}
#endif





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTFillDebugInfoMissIndication         +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB+*/
/*+                             EDD_UPPER_CSRT_IND_PROVIDE_PTR_TYPE     pIndParam+*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pIndParam  : Pointer to parameter of indication RQB (RQB->pParam)      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Fill DebugInfo of given parameter in indication RQB       +*/
/*+               with aditional information like SyncOk, RTC2/3-PortStatus.+*/
/*+                 -> IndParam.DebugInfo[0..11]                            +*/
/*+                                                                         +*/
/*+               Fill only if at least one MISS is present.                +*/
/*+                 -> Look for MISS indication in all entries of           +*/
/*+                    pIndParam->Data[Count].Event                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTFillDebugInfoMissIndication(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
    EDD_UPPER_CSRT_IND_PROVIDE_PTR_TYPE     pIndParam)
{
    LSA_UINT32          i, iPortID, iDebugInfo;
    LSA_UINT8           TempValue8;
    LSA_BOOL            isFillDebugInfo;

    iDebugInfo      = 0;
    TempValue8      = 0;
    isFillDebugInfo = LSA_FALSE;

    /* ------------------------------------------------ */
    /* look for MISS indication in all entries          */
    /* ------------------------------------------------ */
    for ( i=0; i < pIndParam->Count; i++ )
    {
        if (pIndParam->Data[i].Event & EDD_CSRT_CONS_EVENT_MISS)
        {
            isFillDebugInfo = LSA_TRUE;
            i = pIndParam->Count;   // exit FOR loop
        }
    }

    /* ------------------------------------------------ */
    /* fill IndParam.DebugInfo[0..11]                   */
    /* ------------------------------------------------ */
    if (isFillDebugInfo)
    {
        /* ------------------------------------------------------------------------------------------------ */
        /* Byte(0)      : Ident                                                                             */
        /* ------------------------------------------------------------------------------------------------ */
        pIndParam->DebugInfo[iDebugInfo] = EDD_DEBUGINFO_IND_PROVIDE_IDENT_EDDP;

        /* ------------------------------------------------------------------------------------------------ */
        /* Byte(1)      : Global                                                                            */
        /* ------------------------------------------------------------------------------------------------ */
        iDebugInfo++;
        //  - Bit(0)    : Global.Valid
        EDDP_SET_BIT_VALUE8( pIndParam->DebugInfo[iDebugInfo], 
                             0x1, 
                             EDDP_CRT_IND_PROVIDE_DEBUGINFO_GLOBAL_VALID_MASK, 
                             EDDP_CRT_IND_PROVIDE_DEBUGINFO_GLOBAL_VALID_SHIFT);
        //  - Bit(1)    : Global.SyncOk
        EDDP_SET_BIT_VALUE8( pIndParam->DebugInfo[iDebugInfo], 
                             pDDB->SWI.SyncOk, 
                             EDDP_CRT_IND_PROVIDE_DEBUGINFO_GLOBAL_SYNCOK_MASK, 
                             EDDP_CRT_IND_PROVIDE_DEBUGINFO_GLOBAL_SYNCOK_SHIFT);

        for ( iPortID=1; iPortID <= pDDB->SWI.PortCnt; iPortID++ )
        {
            /* ------------------------------------------------------------------------------------------------ */
            /* Byte(2)      : PortStatus1                                                                       */
            /* ------------------------------------------------------------------------------------------------ */
            iDebugInfo++;
            //  - Bit(0..1) : RTC3PortStatus
            switch (pDDB->SWI.LinkStatus[iPortID].IRTPortStatus)
            {
                case EDD_IRT_NOT_SUPPORTED:
                    TempValue8 = EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC3PST_NOT_SUPPORTED;
                break;

                case EDD_IRT_PORT_INACTIVE:
                    TempValue8 = EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC3PST_INACTIVE;
                break;

                case EDD_IRT_PORT_ACTIVE_TX_UP:
                    TempValue8 = EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC3PST_TX;
                break;

                case EDD_IRT_PORT_ACTIVE_TX_RX:
                    TempValue8 = EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC3PST_RX_TX;
                break;

                default:
                {
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTFillDebugInfoMissIndication(): FATAL -> IRTPortStatus(0x%X) is unknown for Port1!", 
                        pDDB->SWI.LinkStatus[iPortID].IRTPortStatus);
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }
                break;
            }
            EDDP_SET_BIT_VALUE8( pIndParam->DebugInfo[iDebugInfo], 
                                 TempValue8, 
                                 EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC3PST_MASK, 
                                 EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC3PST_SHIFT);

            //  - Bit(2..3) : RTC2PortStatus
            switch (pDDB->SWI.LinkStatus[iPortID].RTClass2_PortStatus)
            {
                case EDD_RTCLASS2_NOT_SUPPORTED:
                    TempValue8 = EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC2PST_NOT_SUPPORTED;
                break;

                case EDD_RTCLASS2_INACTIVE:
                    TempValue8 = EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC2PST_INACTIVE;
                break;

                case EDD_RTCLASS2_ACTIVE:
                    TempValue8 = EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC2PST_ACTIVE;
                break;

                default:
                {
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTFillDebugInfoMissIndication(): FATAL -> RTClass2_PortStatus(0x%X) is unknown for Port1!", 
                        pDDB->SWI.LinkStatus[iPortID].RTClass2_PortStatus);
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }
                break;
            }
            EDDP_SET_BIT_VALUE8( pIndParam->DebugInfo[iDebugInfo], 
                                 TempValue8, 
                                 EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC2PST_MASK, 
                                 EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC2PST_SHIFT);

            //  - Bit(4..5) : RtClass3_RemoteState
            switch (pDDB->SWI.RTC3_PortState[iPortID].RemoteState)
            {
                case EDD_SET_REMOTE_PORT_STATE_RT_CLASS3_OFF:
                    TempValue8 = EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC3REMOTESTATE_OFF;
                break;

                case EDD_SET_REMOTE_PORT_STATE_RT_CLASS3_TX:
                    TempValue8 = EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC3REMOTESTATE_TX;
                break;

                case EDD_SET_REMOTE_PORT_STATE_RT_CLASS3_RXTX:
                    TempValue8 = EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC3REMOTESTATE_RX_TX;
                break;

                default:
                {
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTFillDebugInfoMissIndication(): FATAL -> RemoteState(0x%X) is unknown for Port1!", 
                        pDDB->SWI.RTC3_PortState[iPortID].RemoteState);
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }
                break;
            }
            EDDP_SET_BIT_VALUE8( pIndParam->DebugInfo[iDebugInfo], 
                                 TempValue8, 
                                 EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC3REMOTESTATE_MASK, 
                                 EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_RTC3REMOTESTATE_SHIFT);

            //  - Bit(6)    : Preamble
            if (pDDB->SWI.RTC3_PortState[iPortID].Preamble == EDDP_SWI_PREAMBLE_OK)
                TempValue8 = 1;
            else
                TempValue8 = 0;
            EDDP_SET_BIT_VALUE8( pIndParam->DebugInfo[iDebugInfo], 
                                 TempValue8, 
                                 EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_PREAMBLE_MASK, 
                                 EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS1_PREAMBLE_SHIFT);

            /* ------------------------------------------------------------------------------------------------ */
            /* Byte(3)      : PortStatus2                                                                       */
            /* ------------------------------------------------------------------------------------------------ */
            iDebugInfo++;
            //  - Bit(0)    : RtClass2_TopoState
            if (pDDB->SWI.RtClass2_TopoState[iPortID] == EDD_SET_REMOTE_PORT_STATE_TOPO_OK)
                TempValue8 = 1;
            else
                TempValue8 = 0;
            EDDP_SET_BIT_VALUE8( pIndParam->DebugInfo[iDebugInfo], 
                                 TempValue8, 
                                 EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS2_RTC2TOPOST_MASK, 
                                 EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS2_RTC2TOPOST_SHIFT);

            //  - Bit(1)    : RtClass3_TopoState
            if (pDDB->SWI.RTC3_PortState[iPortID].TopoState == EDD_SET_REMOTE_PORT_STATE_TOPO_OK)
                TempValue8 = 1;
            else
                TempValue8 = 0;
            EDDP_SET_BIT_VALUE8( pIndParam->DebugInfo[iDebugInfo], 
                                 TempValue8, 
                                 EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS2_RTC3TOPOST_MASK, 
                                 EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS2_RTC3TOPOST_SHIFT);

            //  - Bit(2)    : RtClass3_PDEVTopoState
            if (pDDB->SWI.RTC3_PortState[iPortID].PDEVTopoState == EDD_SET_REMOTE_PORT_STATE_TOPO_OK)
                TempValue8 = 1;
            else
                TempValue8 = 0;
            EDDP_SET_BIT_VALUE8( pIndParam->DebugInfo[iDebugInfo], 
                                 TempValue8, 
                                 EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS2_RTC3PDEVTOPOST_MASK, 
                                 EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS2_RTC3PDEVTOPOST_SHIFT);

            //  - Bit(3)    : Neighbour_TopoState
            if (pDDB->SWI.LinkStatus[iPortID].Neighbour_TopoState == EDD_SET_REMOTE_PORT_STATE_TOPO_OK)
                TempValue8 = 1;
            else
                TempValue8 = 0;
            EDDP_SET_BIT_VALUE8( pIndParam->DebugInfo[iDebugInfo], 
                                 TempValue8, 
                                 EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS2_NBTOPOST_MASK, 
                                 EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS2_NBTOPOST_SHIFT);

            //  - Bit(4..7) : PortState
            EDDP_SET_BIT_VALUE8( pIndParam->DebugInfo[iDebugInfo], 
                                 pDDB->SWI.LinkStatus[iPortID].PortState, 
                                 EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS2_PORTSTATE_MASK, 
                                 EDDP_CRT_IND_PROVIDE_DEBUGINFO_PORTSTATUS2_PORTSTATE_SHIFT);

            /* ------------------------------------------------------------------------------------------------ */
            /* Byte(5)      : next port                                                                         */
            /* ------------------------------------------------------------------------------------------------ */
        }

        /* ---------------------------------------- */
        /* Trace DebugInfo (12 byte)                */
        /*  ==> trace only 2 ports                  */
        /* ---------------------------------------- */
        EDDP_CRT_TRACE_07(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTFillDebugInfoMissIndication(): DebugInfo = 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x00 0x00 0x00 0x00 0x00", 
            pIndParam->DebugInfo[0], 
            pIndParam->DebugInfo[1], 
            pIndParam->DebugInfo[2], 
            pIndParam->DebugInfo[3], 
            pIndParam->DebugInfo[4], 
            pIndParam->DebugInfo[5], 
            pIndParam->DebugInfo[6]);
    }
}
