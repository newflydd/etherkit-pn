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
/*  F i l e               &F: eddp_crt_prov_acw.c                       :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  CRT (Cycle Real Time) for EDDP  -> Provider ACWs                         */
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
/*  24.11.10    JS          changes to prevent compiler warnings             */
#endif
/*****************************************************************************/


/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/

#define LTRC_ACT_MODUL_ID   32
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_CRT_PROV_ACW */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"           /* eddp headerfiles */
#include "eddp_int.h"           /* internal header  */


EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvAcwAddToTBase                                   +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             LSA_UINT16                          AcwID                   +*/
/*+                             LSA_BOOL                            isDFP                   +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                     +*/
/*+  AcwID          : Index in ACW array                                                    +*/
/*+  isDFP          : is this DFP-ACW ?                                                     +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Add one ACW to TBase tree.                                                +*/
/*+                                                                                         +*/
/*+               If this ACW is already added to AcwPhaseList, nothing to do.              +*/
/*+                                                                                         +*/
/*+               If no Trog available on the TBase[Red + Phase], take a new Trog from      +*/
/*+               TrogFreeList and put new ACW to this Trog as first ACW in AcwPhaseList.   +*/
/*+                                                                                         +*/
/*+               If one Trog available on the TBase[Red + Phase], chain new ACW            +*/
/*+               to the AcwPhaseList of this Trog.                                         +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwAddToTBase(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID,
    LSA_BOOL                    isDFP)
{
    LSA_UINT16                              TBaseIndex;
    LSA_UINT16                              TrogUsedIdx;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
//  EDDP_CRT_PROVIDER_DDB_MNGM_PTR_TYPE     pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;
    EDDP_CRT_PROVIDER_TBASE_PTR_TYPE        pTBase;

    EDDP_ASSERT_FALSE(AcwID >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pProvMngm    = &pDDB->CRT.Prov;
    pAcw            = &pProvMngm->pAcwEntry[AcwID];

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProvAcwAddToTBase(AcwID=%d): Acw-State=0x%X", 
        AcwID, pAcw->State);

    // only if this ACW is not yet added to AcwPhaseList
    if (!(pAcw->State & EDDP_CRT_PROV_ACW_STATE_IN_PHASELIST_MASK))
    {
        TBaseIndex  = EDDP_CRTGetTBaseIndex ( pDDB, 
                                              pAcw->AddParam.CycleReductionRatio, 
                                              pAcw->AddParam.CyclePhase);
        pTBase      = &pProvMngm->pTBaseEntry[TBaseIndex];

        if (LSA_HOST_PTR_ARE_EQUAL(pTBase->pTrogEntry, LSA_NULL))
        {
            /* ------------------------------------------------------------------------ */
            /* there is no Trog on this position in TBase array                         */
            /* ------------------------------------------------------------------------ */
            // take one Trog from TrogFreeList and get TrogUsedIdx
            TrogUsedIdx = EDDP_CRTTrogEntryAddToUsedList (pDDB);

            if (TrogUsedIdx == EDDP_CRT_CHAIN_IDX_END)
            {
                // this should have been stated in CheckXxxParam
                EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwAddToTBase(AcwID=%d): FATAL -> No more free Trog-Entry: ProviderUsedCnt=%d, AcwUsedCnt=%d", 
                    AcwID, pProvMngm->ProviderUsedCnt, pProvMngm->AcwUsedCnt);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            else
            {
                pTBase->TBaseElementInHw    = LSA_FALSE;
                pTBase->TBaseElement        = 0;
                pTBase->ReductionNr         = EDDP_CRTGetReductionNrFromReductionRatio (pDDB, pAcw->AddParam.CycleReductionRatio);

                // init pointer to new Trog
                pTBase->pTrogEntry = &pProvMngm->pTrogEntry[TrogUsedIdx];
                // init pointer to new Trog in APICtrl-RAM
                pTBase->pTrogEntry->pHwTrogApi = &pProvMngm->pHwBaseTrogApi[TrogUsedIdx];
                // init pointer to new TBaseElement in APICtrl-RAM
                pTBase->pHwTBaseApi = &pProvMngm->pHwBaseTBaseApi[TBaseIndex];

                EDDP_CRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW,"[H:--] EDDP_CRTProvAcwAddToTBase(): TBaseIndex=%d, pTBase->ReductionNr=%d, pTBase->pTrogEntry=0x0, NewTrogId=%d, pHwBaseTBaseApi=0x%X, pHwTBaseApi=0x%X", 
                    TBaseIndex, pTBase->ReductionNr, TrogUsedIdx, pProvMngm->pHwBaseTBaseApi, pTBase->pHwTBaseApi);
            }
        }
        else
        {
            /* ---------------------------------------------------------------------------- */
            /* there is one Trog on this position in TBase array                            */
            /* chain this new ACW to AcwPhaseList of this pTrogEntry[TrogId]                */
            /* ---------------------------------------------------------------------------- */
            // get TrogUsedIdx
            TrogUsedIdx = pTBase->pTrogEntry->TrogId;

            EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProvAcwAddToTBase(): TBaseIndex=%d, pTBase->pTrogEntry=0x%X, TrogId=%d", 
                TBaseIndex, pTBase->pTrogEntry, TrogUsedIdx);
        }

        /* ---------------------------------------------------------------------------- */
        /* update AcwPhaseList:     AcwPhaseList -> ACW1 -> ACW2 ...                    */
        /*  - add new ACW to AcwPhaseList                                               */
        /* ---------------------------------------------------------------------------- */
        switch (pAcw->ROGPhase)
        {
            case EDDP_CRT_PROV_ACW_PHASE_G:
            case EDDP_CRT_PROV_ACW_PHASE_O:
            {
                EDDP_CRTProvAcwPhaseListUpdate_O_G (pDDB, EDDP_CRT_ADD, TrogUsedIdx, AcwID);
            }
            break;

            case EDDP_CRT_PROV_ACW_PHASE_R:
            {
                EDDP_CRTProvAcwPhaseListUpdate_R (pDDB, EDDP_CRT_ADD, TrogUsedIdx, AcwID, isDFP);
            }
            break;

            default:
            {
                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwAddToTBase(AcwID=%d): FATAL -> ROGPhase unknown! ROGPhase=0x%X", 
                    AcwID, pAcw->ROGPhase);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvAcwRemoveFromTBase                              +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             LSA_UINT16                          AcwID                   +*/
/*+                             LSA_BOOL                            isDFP                   +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                     +*/
/*+  AcwID          : Index in ACW array                                                    +*/
/*+  isDFP          : is this DFP-ACW ?                                                     +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Remove one ACW from TBase tree.                                           +*/
/*+               If this ACW is already removed from AcwPhaseList, nothing to do.          +*/
/*+               One Trog must be available on this TBase. If no, nothing to do.           +*/
/*+                                                                                         +*/
/*+               No check AcwID, it must be valid !                                        +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwRemoveFromTBase(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID,
    LSA_BOOL                    isDFP)
{
    LSA_UINT16                              TBaseIndex;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
//  EDDP_CRT_PROVIDER_DDB_MNGM_PTR_TYPE     pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;
    EDDP_CRT_PROVIDER_TBASE_PTR_TYPE        pTBase;

    EDDP_ASSERT_FALSE(AcwID >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pProvMngm    = &pDDB->CRT.Prov;
    pAcw            = &pProvMngm->pAcwEntry[AcwID];

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProvAcwRemoveFromTBase(AcwID=%d): Acw-State=0x%X", 
        AcwID, pAcw->State);

    // only if this ACW is not yet removed from AcwPhaseList
    if (pAcw->State & EDDP_CRT_PROV_ACW_STATE_IN_PHASELIST_MASK)
    {
        TBaseIndex  = EDDP_CRTGetTBaseIndex ( pDDB, 
                                              pAcw->AddParam.CycleReductionRatio, 
                                              pAcw->AddParam.CyclePhase);
        pTBase      = &pProvMngm->pTBaseEntry[TBaseIndex];

        // only if Trog is available on this TBase
        if ( !(LSA_HOST_PTR_ARE_EQUAL(pTBase->pTrogEntry, LSA_NULL)) )
        {
            EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProvAcwRemoveFromTBase(): TBaseIndex=%d, TrogId=%d, pHwBaseTBaseApi=0x%X, pHwTBaseApi=0x%X", 
                TBaseIndex, pTBase->pTrogEntry->TrogId, pProvMngm->pHwBaseTBaseApi, pTBase->pHwTBaseApi);

            // update AcwPhaseList:  AcwPhaseList -> ACW1 -> ACW2 ...
            switch (pAcw->ROGPhase)
            {
                case EDDP_CRT_PROV_ACW_PHASE_G:
                case EDDP_CRT_PROV_ACW_PHASE_O:
                {
                    EDDP_CRTProvAcwPhaseListUpdate_O_G (pDDB, EDDP_CRT_REMOVE, pTBase->pTrogEntry->TrogId, AcwID);
                }
                break;

                case EDDP_CRT_PROV_ACW_PHASE_R:
                {
                    EDDP_CRTProvAcwPhaseListUpdate_R (pDDB, EDDP_CRT_REMOVE, pTBase->pTrogEntry->TrogId, AcwID, isDFP);
                }
                break;

                default:
                {
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwRemoveFromTBase(): FATAL -> ROGPhase unknown! ROGPhase=0x%X", 
                        pAcw->ROGPhase);
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }
                break;
            }
        }
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvAcwIsFirstAcwAddedToTBase                       +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             LSA_UINT16                          TBaseIndex              +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                     +*/
/*+  TBaseIndex     : Index in TBase array                                                  +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: This function checks whether first ACW is added to TBase.                 +*/
/*+               If yes:                                                                   +*/
/*+                 - init TBaseElement in APICtrl-RAM                                      +*/
/*+                                                                                         +*/
/*+               Call this function only if this ACW is already added to AcwPhaseList      +*/
/*+               (already added to TBase) because of "AcwPhaseCnt_ROG"!                    +*/
/*+                                                                                         +*/
/*+               One Trog must be available on this TBase. If no, nothing to do.           +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwIsFirstAcwAddedToTBase(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  TBaseIndex)
{
    LSA_UINT32                              AcwPhaseCnt_ROG;
    EDDP_CRT_PROVIDER_TBASE_PTR_TYPE        pTBase;

    EDDP_ASSERT_FALSE(TBaseIndex >= EDDP_CRT_MAX_COUNT_TBASE);
    pTBase = &pDDB->CRT.Prov.pTBaseEntry[TBaseIndex];

    // only if Trog is available on this TBase
    if ( !(LSA_HOST_PTR_ARE_EQUAL(pTBase->pTrogEntry, LSA_NULL)) )
    {
        // get sum of all ACWs in this Trog
        AcwPhaseCnt_ROG = pTBase->pTrogEntry->TrogAcwList.AcwPhaseCnt_R
                        + pTBase->pTrogEntry->TrogAcwList.AcwPhaseCnt_O
                        + pTBase->pTrogEntry->TrogAcwList.AcwPhaseCnt_G;

        // does this Trog contain ACWs ?
        if (AcwPhaseCnt_ROG > 0)
        {
            // init TBaseElement in APICtrl-RAM
            EDDP_CRTProviderInitTBaseElementHwApi (pDDB, TBaseIndex);
        }
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvAcwIsLastAcwRemovedFromTBase                    +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             LSA_UINT16                          TBaseIndex              +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                     +*/
/*+  TBaseIndex     : Index in TBase array                                                  +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: This function checks whether last ACW is removed from TBase.              +*/
/*+               Does this TBase contain Trog with ACWs?                                   +*/
/*+               If no more ACWs in Trog:                                                  +*/
/*+                 - remove Trog from TrogUsedList                                         +*/
/*+                 - delete Trog in APICtrl-RAM                                            +*/
/*+                 - delete TBaseElement in APICtrl-RAM                                    +*/
/*+                 - delete parameter of TBase entry                                       +*/
/*+                                                                                         +*/
/*+               Call this function only if this ACW is already removed from AcwPhaseList  +*/
/*+               (already removed from TBase) because of "AcwPhaseCnt_ROG"!                +*/
/*+                                                                                         +*/
/*+               One Trog must be available on this TBase. If no, nothing to do.           +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwIsLastAcwRemovedFromTBase(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  TBaseIndex)
{
    LSA_UINT32                              AcwPhaseCnt_ROG;
    EDDP_CRT_PROVIDER_TBASE_PTR_TYPE        pTBase;

    EDDP_ASSERT_FALSE(TBaseIndex >= EDDP_CRT_MAX_COUNT_TBASE);
    pTBase = &pDDB->CRT.Prov.pTBaseEntry[TBaseIndex];

    // only if Trog is available on this TBase
    if ( !(LSA_HOST_PTR_ARE_EQUAL(pTBase->pTrogEntry, LSA_NULL)) )
    {
        // get sum of all ACWs in this Trog
        AcwPhaseCnt_ROG = pTBase->pTrogEntry->TrogAcwList.AcwPhaseCnt_R
                        + pTBase->pTrogEntry->TrogAcwList.AcwPhaseCnt_O
                        + pTBase->pTrogEntry->TrogAcwList.AcwPhaseCnt_G;

        // does this Trog contain no more ACWs ?
        if (AcwPhaseCnt_ROG == 0)
        {
            // remove Trog from TrogUsedList
            EDDP_CRTTrogEntryRemoveFromUsedList (pDDB, pTBase->pTrogEntry->TrogId);
            // delete Trog in APICtrl-RAM
            EDDP_CRTProviderDeleteTrogHwApi (pDDB, pTBase->pTrogEntry->TrogId);
            // delete TBaseElement in APICtrl-RAM
            EDDP_CRTProviderDeleteTBaseElementHwApi (pDDB, TBaseIndex);
            // delete parameter of TBase entry
            pTBase->ReductionNr = 0;
            pTBase->pTrogEntry  = LSA_NULL;
            pTBase->pHwTBaseApi = LSA_NULL;
        }
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvAcwAddToUsedList                                +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
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
/*+  Description: Add new ACW to UsedList by Provider-Add                                   +*/
/*+                 -> get first free index from AcwFreeList and put it to AcwUsedList      +*/
/*+                 -> the NewAcwID will be returned in return value                        +*/
/*+                                                                                         +*/
/*+                 -> if (NewAcwID == EDDP_CRT_CHAIN_IDX_END)                              +*/
/*+                     - no more free ACW                                                  +*/
/*+                     - set error by caller of this function                              +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwAddToUsedList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT16                              NewAcwID;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
//  EDDP_CRT_PROVIDER_DDB_MNGM_PTR_TYPE     pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;

    pProvMngm    = &pDDB->CRT.Prov;
    pProvMngm    = &pDDB->CRT.Prov;
    NewAcwID        = pProvMngm->AcwFreeList.BeginIdx;   // first index in ACW free list

    if (NewAcwID == EDDP_CRT_CHAIN_IDX_END)
    {
        // no more free ACW
        // set error by caller of this function
    }
    else
    {
        pAcw = &pProvMngm->pAcwEntry[NewAcwID];

        /* ------------------------------------------------------------ */
	    /* Remove this new ACW from free-chain list                     */
	    /* ------------------------------------------------------------ */
        if (pAcw->UsedFreeLink.Next == EDDP_CRT_CHAIN_IDX_END)
        {
            // this ACW is the last free ACW
            pProvMngm->AcwFreeList.BeginIdx  = EDDP_CRT_CHAIN_IDX_END;
            pProvMngm->AcwFreeList.EndIdx    = EDDP_CRT_CHAIN_IDX_END;
        }
        else
        {
            // this ACW is not the last free ACW
            pProvMngm->AcwFreeList.BeginIdx                                  = pAcw->UsedFreeLink.Next;
            pProvMngm->pAcwEntry[pAcw->UsedFreeLink.Next].UsedFreeLink.Prev  = EDDP_CRT_CHAIN_IDX_END;
        }

        /* ------------------------------------------------------------ */
        /* Put this new ACW to end used-chain list                      */
        /* ------------------------------------------------------------ */
        if (pProvMngm->AcwUsedList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
        {
            // this ACW is the first ACW in the used list
            pProvMngm->AcwUsedList.BeginIdx  = NewAcwID;
            pProvMngm->AcwUsedList.EndIdx    = NewAcwID;
            pAcw->UsedFreeLink.Prev             = EDDP_CRT_CHAIN_IDX_END;
            pAcw->UsedFreeLink.Next             = EDDP_CRT_CHAIN_IDX_END;
        }
        else
        {
            // this ACW is not the first ACW in the used list
            pAcw->UsedFreeLink.Prev                                                     = pProvMngm->AcwUsedList.EndIdx;
            pAcw->UsedFreeLink.Next                                                     = EDDP_CRT_CHAIN_IDX_END;
            pProvMngm->pAcwEntry[pProvMngm->AcwUsedList.EndIdx].UsedFreeLink.Next = NewAcwID;
            pProvMngm->AcwUsedList.EndIdx                                            = NewAcwID;
        }

        EDDP_SET_BIT_VALUE16( (pAcw->State), 
                              EDDP_CRT_PROV_ACW_STATE_BIT_ON, 
                              EDDP_CRT_PROV_ACW_STATE_IN_SW_MASK, 
                              EDDP_CRT_PROV_ACW_STATE_IN_SW_SHIFT)

        pProvMngm->AcwUsedCnt++;
    }

    return (NewAcwID);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvAcwAddToProvider                                +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                        +*/
/*+                             LSA_UINT16                      ProviderID                  +*/
/*+                             LSA_UINT16                      AcwID                       +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ProviderID         : Index in provider array                                           +*/
/*+  AcwID              : Index in ACW array                                                +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Add new ACW to provider.                                                  +*/
/*+                                                                                         +*/
/*+               Chain this ACW in AcwProvLink of this provider.                           +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwAddToProvider(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID,
    LSA_UINT16                  AcwID)
{
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE              pProvider;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;

    EDDP_ASSERT_FALSE(ProviderID >= pDDB->CRT.CfgParam.ProviderMaxNumber);
    EDDP_ASSERT_FALSE(AcwID >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pProvider       = &pProvMngm->pProvEntry[ProviderID];
    pAcw            = &pProvMngm->pAcwEntry[AcwID];

    /* ------------------------------------------------------------ */
    /* Put this new ACW to end of AcwList                           */
    /* ------------------------------------------------------------ */
    if (pProvider->AcwList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
    {
        // this ACW is the first ACW in AcwList
        pProvider->AcwList.BeginIdx = AcwID;
        pProvider->AcwList.EndIdx   = AcwID;
        pAcw->AcwProvLink.Prev      = EDDP_CRT_CHAIN_IDX_END;
        pAcw->AcwProvLink.Next      = EDDP_CRT_CHAIN_IDX_END;
    }
    else
    {
        // this ACW is not the first ACW in AcwList
        pAcw->AcwProvLink.Prev                                              = pProvider->AcwList.EndIdx;
        pAcw->AcwProvLink.Next                                              = EDDP_CRT_CHAIN_IDX_END;
        pProvMngm->pAcwEntry[pProvider->AcwList.EndIdx].AcwProvLink.Next = AcwID;
        pProvider->AcwList.EndIdx                                           = AcwID;
    }

    pProvider->AcwCnt++;
}





#if defined (EDDP_CFG_HW_ERTEC200P_SUPPORT)
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProvAcwInitHwApiRTFrame                         +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      ProviderID              +*/
/*+                             LSA_UINT16                      AcwID                   +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  ProviderID         : Index in provider array                                       +*/
/*+  AcwID              : Index in ACW array                                            +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Init the send ACW for RT or RToverUDP in APICtrl-RAM.                 +*/
/*+               Use ACW.Word1..5                                                      +*/
/*+                                                                                     +*/
/*+               Call this function only after coping of all provider parameter!       +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
extern LSA_VOID EDDP_LL_CRTProvAcwInitHwApiRTFrame(
	    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	    LSA_UINT16                  ProviderID,
	    LSA_UINT16                  AcwID);
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwInitHwApiRTFrame(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID,
    LSA_UINT16                  AcwID)
{
    LSA_UINT64                              ACW_Word64;
    LSA_UINT32                              ValueTemp;
    EDD_UPPER_MEM_U8_PTR_TYPE               User_Addr;
    LSA_UINT32                              Pnip_Addr;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE              pProvider;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;

    EDDP_ASSERT_FALSE(ProviderID >= pDDB->CRT.CfgParam.ProviderMaxNumber);
    EDDP_ASSERT_FALSE(AcwID >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pProvider       = &pProvMngm->pProvEntry[ProviderID];
    pAcw            = &pProvMngm->pAcwEntry[AcwID];
    pAcw->pHwAcwApi = &((PNIP_ACW_SND_PTR_TYPE)(pProvMngm->pHwBaseAcwApi))[AcwID];

    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProvAcwInitHwApiRTFrame(ProviderID=%d, AcwID=%d): Acw-State=0x%X", 
        ProviderID, AcwID, pAcw->State);

    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    /* ACW.Word1                                                                                       */
    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    ACW_Word64 = 0;
    /* ----------------------------------------------------------------------------------------------- */
    // FrameID to be sent
    EDDP_ASSERT_FALSE(pAcw->AddParam.FrameID == EDD_FRAME_ID_UNDEFINED);
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          pAcw->AddParam.FrameID, 
                          PNIP_ACW_RCV_WORD1_FRAMEID_MASK, 
                          PNIP_ACW_SND_WORD1_FRAMEID_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // ACW_nextoffset: Offset of next ACW in APICtrl-RAM (init ACW_nextoffset to begin as last ACW)
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_R2_ACW_SND_WORD1_ACW_NEXTOFFSET_INIT_VALUE,
                          PNIP_R2_ACW_SND_WORD1_ACW_NEXTOFFSET_MASK,
                          PNIP_ACW_SND_WORD1_ACW_NEXTOFFSET_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // ACW_DG_offset: Offset of first ACW_DG in APICtrl-RAM which belongs to this ACW (init ACW_DG_offset to begin as last ACW)
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_R2_ACW_SND_WORD1_ACW_DG_OFFSET_INIT_VALUE,
                          PNIP_R2_ACW_SND_WORD1_ACW_DG_OFFSET_MASK,
                          PNIP_ACW_SND_WORD1_ACW_DG_OFFSET_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // Opcode for ACW Send
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD1_OPCODE_ACW_SND_VALUE, 
                          PNIP_ACW_SND_WORD1_OPCODE_MASK, 
                          PNIP_ACW_SND_WORD1_OPCODE_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // Active Bit: as it is allowed to call the command "ACW_in_out" (PROVIDER_CONTROL) with activated ACW, set Active Bit here
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD1_ACTIVE_INIT_VALUE,
                          PNIP_ACW_SND_WORD1_ACTIVE_MASK, 
                          PNIP_ACW_SND_WORD1_ACTIVE_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // FirstSend Bit - after first sending the bit will be reseted by HW
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD1_FIRSTSEND_INIT_VALUE, 
                          PNIP_ACW_SND_WORD1_FIRSTSEND_MASK, 
                          PNIP_ACW_SND_WORD1_FIRSTSEND_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // DP_Raw_Mode  : DataPlane specification for IRTtop
    // DP2_Type     : DP1/DP2 (DP2 = send only if Red is active)
    switch (pAcw->ROGPhase)
    {
        case EDDP_CRT_PROV_ACW_PHASE_G:
        case EDDP_CRT_PROV_ACW_PHASE_O:
        {
            if (pAcw->AcwId == pProvider->AuxAcwId)
            {
                // AUX-ACW with DataPlane specification because this FrameID is in IRT-Table available and
                // all FrameIDs in IRT-Table are DP2 automatically!
                // The AUX-ACW must be no DP2
                EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                      PNIP_ACW_SND_WORD1_DP_RAW_MODE_ON_VALUE, 
                                      PNIP_ACW_SND_WORD1_DP_RAW_MODE_MASK, 
                                      PNIP_ACW_SND_WORD1_DP_RAW_MODE_SHIFT)
            }
            else
            {
                // all other ACWs without DataPlane specification
                EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                      PNIP_ACW_SND_WORD1_DP_RAW_MODE_OFF_VALUE, 
                                      PNIP_ACW_SND_WORD1_DP_RAW_MODE_MASK, 
                                      PNIP_ACW_SND_WORD1_DP_RAW_MODE_SHIFT)
            }
            // DP2_Type = DP1
            EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                  PNIP_ACW_SND_WORD1_DP2_TYPE_DP1_VALUE, 
                                  PNIP_ACW_SND_WORD1_DP2_TYPE_MASK, 
                                  PNIP_ACW_SND_WORD1_DP2_TYPE_SHIFT)
        }
        break;

        case EDDP_CRT_PROV_ACW_PHASE_R:
        {
            // with DataPlane specification
            EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                  PNIP_ACW_SND_WORD1_DP_RAW_MODE_ON_VALUE, 
                                  PNIP_ACW_SND_WORD1_DP_RAW_MODE_MASK, 
                                  PNIP_ACW_SND_WORD1_DP_RAW_MODE_SHIFT)
            // DP2_Type = DP2
            EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                  PNIP_ACW_SND_WORD1_DP2_TYPE_DP2_VALUE, 
                                  PNIP_ACW_SND_WORD1_DP2_TYPE_MASK, 
                                  PNIP_ACW_SND_WORD1_DP2_TYPE_SHIFT)
        }
        break;

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiRTFrame(): FATAL -> ROGPhase unknown! ROGPhase=0x%X", 
                pAcw->ROGPhase);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }
    /* ----------------------------------------------------------------------------------------------- */
    // EtherType: Index 0-3 to select one of Register "Local_SPort_Etype_0-3"
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD1_ETHERTYPE_SELECT_INDEX0_VALUE, 
                          PNIP_ACW_SND_WORD1_ETHERTYPE_SELECT_MASK, 
                          PNIP_ACW_SND_WORD1_ETHERTYPE_SELECT_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // InBound_Pack: Bit for info the pack frame is (not) a InBound-Pack-Frame
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD1_INBOUND_PACK_OFF_VALUE, 
                          PNIP_ACW_SND_WORD1_INBOUND_PACK_MASK, 
                          PNIP_ACW_SND_WORD1_INBOUND_PACK_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // RT_class: RT-Frame or RToverUDP-Frame
    ValueTemp = 0;
    switch (pProvider->RTClass)
    {
        case EDDP_CRT_RTCLASS_RT:
            ValueTemp = PNIP_ACW_SND_WORD1_RT_CLASS_RT_VALUE;
        break;

        case EDDP_CRT_RTCLASS_UDP:
            ValueTemp = PNIP_ACW_SND_WORD1_RT_CLASS_UDP_VALUE;
        break;

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiRTFrame(): FATAL -> Provider-RTClass unknown! RTClass=0x%X", 
                pProvider->RTClass);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          ValueTemp, 
                          PNIP_ACW_SND_WORD1_RT_CLASS_MASK, 
                          PNIP_ACW_SND_WORD1_RT_CLASS_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // ACW_DG_order: ascending or descending order for sending of ACW_DGs
    switch (pAcw->DfpParam.ACW_DG_order)
    {
        case EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_HIGH:
        {
            EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                  PNIP_ACW_SND_WORD1_ACW_DG_ORDER_HIGH_VALUE, 
                                  PNIP_ACW_SND_WORD1_ACW_DG_ORDER_MASK, 
                                  PNIP_ACW_SND_WORD1_ACW_DG_ORDER_SHIFT)
        }
        break;

        case EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_DOWN:
        {
            EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                  PNIP_ACW_SND_WORD1_ACW_DG_ORDER_DOWN_VALUE, 
                                  PNIP_ACW_SND_WORD1_ACW_DG_ORDER_MASK, 
                                  PNIP_ACW_SND_WORD1_ACW_DG_ORDER_SHIFT)
        }
        break;

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiRTFrame(): FATAL -> ACW_DG_order(0x%X) is unknown!", 
                pAcw->DfpParam.ACW_DG_order);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }
    /* ----------------------------------------------------------------------------------------------- */
    // DRM: Destination Raw Mode for port
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD1_DRM_OFF_VALUE, 
                          PNIP_ACW_SND_WORD1_DRM_MASK, 
                          PNIP_ACW_SND_WORD1_DRM_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // Destination: port specification (PortID)
    ValueTemp = 0;
    switch (pAcw->ROGPhase)
    {
        case EDDP_CRT_PROV_ACW_PHASE_G:
        case EDDP_CRT_PROV_ACW_PHASE_O:
        {
            EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                  PNIP_ACW_SND_WORD1_DESTINATION_OFF_VALUE, 
                                  PNIP_ACW_SND_WORD1_DESTINATION_MASK, 
                                  PNIP_ACW_SND_WORD1_DESTINATION_SHIFT)
        }
        break;

        case EDDP_CRT_PROV_ACW_PHASE_R:
        {
            if (pAcw->AddParam.TxPortID == EDD_PORT_ID_AUTO)
            {
                EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                      PNIP_ACW_SND_WORD1_DESTINATION_OFF_VALUE, 
                                      PNIP_ACW_SND_WORD1_DESTINATION_MASK, 
                                      PNIP_ACW_SND_WORD1_DESTINATION_SHIFT)
            }
            else
            {
                // get Hw-PortID from User-PortID
                switch (EDDP_SWI_get_HwPortID(pDDB, pAcw->AddParam.TxPortID))
                {
                    case 1:     ValueTemp = PNIP_ACW_SND_WORD1_DESTINATION_P1_VALUE;    break;
                    case 2:     ValueTemp = PNIP_ACW_SND_WORD1_DESTINATION_P2_VALUE;    break;
                    case 3:     ValueTemp = PNIP_ACW_SND_WORD1_DESTINATION_P3_VALUE;    break;
                    case 4:     ValueTemp = PNIP_ACW_SND_WORD1_DESTINATION_P4_VALUE;    break;
                    default:
                    {
                        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiRTFrame(): FATAL -> Invalid Hw-PortID from SWI! Acw-PortID=%d, Hw-PortID=%d", 
                            pAcw->AddParam.TxPortID, EDDP_SWI_get_HwPortID (pDDB, pAcw->AddParam.TxPortID));
                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                    }
                    break;
                }
                EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                      ValueTemp, 
                                      PNIP_ACW_SND_WORD1_DESTINATION_MASK, 
                                      PNIP_ACW_SND_WORD1_DESTINATION_SHIFT)
            }
        }
        break;

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiRTFrame(): FATAL -> ROGPhase unknown! ROGPhase=0x%X", 
                pAcw->ROGPhase);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }
    /* ----------------------------------------------------------------------------------------------- */

    // write HOST value to APICtrl-RAM
    EDDP_HAL_MEM64_WRITE (pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->ACW_Word1, ACW_Word64);

    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    /* ACW.Word2                                                                                       */
    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    ACW_Word64 = 0;
    /* ----------------------------------------------------------------------------------------------- */
    // DstMAC-Address to be sent    -> Byte 5
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          pAcw->AddParam.DstMAC.MacAdr[5], 
                          PNIP_ACW_SND_WORD2_MAC_DA_0_MASK, 
                          PNIP_ACW_SND_WORD2_MAC_DA_0_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // DstMAC-Address to be sent    -> Byte 4
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          pAcw->AddParam.DstMAC.MacAdr[4], 
                          PNIP_ACW_SND_WORD2_MAC_DA_1_MASK, 
                          PNIP_ACW_SND_WORD2_MAC_DA_1_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // DstMAC-Address to be sent    -> Byte 3
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          pAcw->AddParam.DstMAC.MacAdr[3], 
                          PNIP_ACW_SND_WORD2_MAC_DA_2_MASK, 
                          PNIP_ACW_SND_WORD2_MAC_DA_2_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // DstMAC-Address to be sent    -> Byte 2
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          pAcw->AddParam.DstMAC.MacAdr[2], 
                          PNIP_ACW_SND_WORD2_MAC_DA_3_MASK, 
                          PNIP_ACW_SND_WORD2_MAC_DA_3_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // DstMAC-Address to be sent    -> Byte 1
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          pAcw->AddParam.DstMAC.MacAdr[1], 
                          PNIP_ACW_SND_WORD2_MAC_DA_4_MASK, 
                          PNIP_ACW_SND_WORD2_MAC_DA_4_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // DstMAC-Address to be sent    -> Byte 0
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          pAcw->AddParam.DstMAC.MacAdr[0], 
                          PNIP_ACW_SND_WORD2_MAC_DA_5_MASK, 
                          PNIP_ACW_SND_WORD2_MAC_DA_5_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // Q_Select: info which queue is used by frame
    ValueTemp = 0;
    switch (pAcw->ROGPhase)
    {
        case EDDP_CRT_PROV_ACW_PHASE_G:
        case EDDP_CRT_PROV_ACW_PHASE_O:
            ValueTemp = PNIP_ACW_SND_WORD2_Q_SELECT_PHASE_VALUE;    // Green, Orange: Phase-Queue-14
        break;

        case EDDP_CRT_PROV_ACW_PHASE_R:
            ValueTemp = PNIP_ACW_SND_WORD2_Q_SELECT_TIME_VALUE;     // Red: Time-Queue-15
        break;

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiRTFrame(): FATAL -> ROGPhase unknown! ROGPhase=0x%X", 
                pAcw->ROGPhase);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          ValueTemp, 
                          PNIP_ACW_SND_WORD2_Q_SELECT_MASK, 
                          PNIP_ACW_SND_WORD2_Q_SELECT_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // PhasenInfo: which phases are used in QCW
    ValueTemp = 0;
    switch (pAcw->ROGPhase)
    {
        case EDDP_CRT_PROV_ACW_PHASE_G:
        {
            // Green phase and TransferType
            ValueTemp = ( PNIP_ACW_SND_WORD2_PHASENINFO_GREEN_ON_VALUE | 
                          PNIP_ACW_SND_WORD2_PHASENINFO_TRANSFERTYPE_OFF_VALUE);
        }
        break;

        case EDDP_CRT_PROV_ACW_PHASE_O:
        {
            // Orange, Green phases and TransferType
            ValueTemp = ( PNIP_ACW_SND_WORD2_PHASENINFO_ORANGE_ON_VALUE | 
                          PNIP_ACW_SND_WORD2_PHASENINFO_GREEN_ON_VALUE | 
                          PNIP_ACW_SND_WORD2_PHASENINFO_TRANSFERTYPE_ON_VALUE);
        }
        break;

        case EDDP_CRT_PROV_ACW_PHASE_R:
        {
            // Red, Orange, Green phases and TransferType
            ValueTemp = ( PNIP_ACW_SND_WORD2_PHASENINFO_RED_ON_VALUE | 
                          PNIP_ACW_SND_WORD2_PHASENINFO_ORANGE_ON_VALUE | 
                          PNIP_ACW_SND_WORD2_PHASENINFO_GREEN_ON_VALUE | 
                          PNIP_ACW_SND_WORD2_PHASENINFO_TRANSFERTYPE_ON_VALUE);
        }
        break;

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiRTFrame(): FATAL -> ROGPhase unknown! ROGPhase=0x%X", 
                pAcw->ROGPhase);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          ValueTemp, 
                          PNIP_ACW_SND_WORD2_PHASENINFO_MASK, 
                          PNIP_ACW_SND_WORD2_PHASENINFO_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // Tx_TimeMode: for sending in Time Queue-15 (Minumum/ExactTime)
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD2_TX_TIMEMODE_EXACT, 
                          PNIP_ACW_SND_WORD2_TX_TIMEMODE_MASK, 
                          PNIP_ACW_SND_WORD2_TX_TIMEMODE_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // DA_Select: select the source of DstMAC-Address
    ValueTemp = 0;
    if (EDDP_IS_ZERO_MAC(pAcw->AddParam.DstMAC))
    {
        // Bit for info: get DstMAC-Address from Register "Router_MAC_DA"
        ValueTemp = PNIP_ACW_SND_WORD2_DA_SELECT_REG_VALUE;
    }
    else
    {
        // Bit for info: get DstMAC-Address from ACW.MAC_DA
        ValueTemp = PNIP_ACW_SND_WORD2_DA_SELECT_ACW_VALUE;
    }
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          ValueTemp, 
                          PNIP_ACW_SND_WORD2_DA_SELECT_MASK, 
                          PNIP_ACW_SND_WORD2_DA_SELECT_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // VLAN_Tag: Frame with or without VLAN_Tag
    switch (pAcw->ROGPhase)
    {
        case EDDP_CRT_PROV_ACW_PHASE_G:
        case EDDP_CRT_PROV_ACW_PHASE_O:
        {
            // Frame with VLAN_Tag
            EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                  PNIP_ACW_SND_WORD2_VLAN_TAG_ON_VALUE, 
                                  PNIP_ACW_SND_WORD2_VLAN_TAG_MASK, 
                                  PNIP_ACW_SND_WORD2_VLAN_TAG_SHIFT)
        }
        break;

        case EDDP_CRT_PROV_ACW_PHASE_R:
        {
            // Frame without VLAN_Tag
            EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                  PNIP_ACW_SND_WORD2_VLAN_TAG_OFF_VALUE, 
                                  PNIP_ACW_SND_WORD2_VLAN_TAG_MASK, 
                                  PNIP_ACW_SND_WORD2_VLAN_TAG_SHIFT)
        }
        break;

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiRTFrame(): FATAL -> ROGPhase unknown! ROGPhase=0x%X", 
                pAcw->ROGPhase);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }
    /* ----------------------------------------------------------------------------------------------- */
    // VLAN_Prio: VLAN priority
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD2_VLAN_PRIO_INIT_VALUE, 
                          PNIP_ACW_SND_WORD2_VLAN_PRIO_MASK, 
                          PNIP_ACW_SND_WORD2_VLAN_PRIO_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // UseType_Raw_Mode: Bit for info how get the "UseType"
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD2_USETYPE_RAW_MODE_FRAME_CLASS_VALUE, 
                          PNIP_ACW_SND_WORD2_USETYPE_RAW_MODE_MASK, 
                          PNIP_ACW_SND_WORD2_USETYPE_RAW_MODE_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // UseType specification
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD2_USETYPE_STD_VALUE, 
                          PNIP_ACW_SND_WORD2_USETYPE_MASK, 
                          PNIP_ACW_SND_WORD2_USETYPE_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */

    // write HOST value to APICtrl-RAM
    EDDP_HAL_MEM64_WRITE (pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->ACW_Word2, ACW_Word64);

    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    /* ACW.Word3                                                                                       */
    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    ACW_Word64 = 0;
    /* ----------------------------------------------------------------------------------------------- */
    // Scoreboard_offset: Pointer to Score Board of this Consumer (AutoStopConsumerID)
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_R2_ACW_SND_WORD3_SCOREBOARD_OFFSET_OFF_VALUE,
                          PNIP_R2_ACW_SND_WORD3_SCOREBOARD_OFFSET_MASK,
                          PNIP_ACW_SND_WORD3_SCOREBOARD_OFFSET_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // DataStatus_offset: index to DataStatus struct in APICtrl-RAM
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          pProvider->DataStatusId, 
                          PNIP_ACW_SND_WORD3_DATASTATUS_OFFSET_MASK, 
                          PNIP_ACW_SND_WORD3_DATASTATUS_OFFSET_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // ACW_Wait and CT_Schwelle set always together for PERIF and Extern RAM 
    #ifdef EDDP_CRT_SUPPORT_ACW_WAIT_ON
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD3_ACW_WAIT_ON_VALUE, 
                          PNIP_ACW_SND_WORD3_ACW_WAIT_MASK, 
                          PNIP_ACW_SND_WORD3_ACW_WAIT_SHIFT)

    switch (pDDB->Hardware)
    {
        case EDDP_HW_ERTEC200P:
        {
            EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                  PNIP_ACW_SND_WORD3_CT_SCHWELLE_0_VALUE, 
                                  PNIP_ACW_SND_WORD3_CT_SCHWELLE_MASK, 
                                  PNIP_ACW_SND_WORD3_CT_SCHWELLE_SHIFT)
        }
        break;
        case EDDP_HW_ERTEC200P_FPGA: 
        {
            // CT_Schwelle dependent of time/phase controlled ACW
            switch (pAcw->ROGPhase)
            {
                case EDDP_CRT_PROV_ACW_PHASE_G:
                case EDDP_CRT_PROV_ACW_PHASE_O:
                {
                    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                          PNIP_ACW_SND_WORD3_CT_SCHWELLE_1_VALUE, 
                                          PNIP_ACW_SND_WORD3_CT_SCHWELLE_MASK, 
                                          PNIP_ACW_SND_WORD3_CT_SCHWELLE_SHIFT)
                }
                break;

                case EDDP_CRT_PROV_ACW_PHASE_R:
                {
                    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                          PNIP_ACW_SND_WORD3_CT_SCHWELLE_0_VALUE, 
                                          PNIP_ACW_SND_WORD3_CT_SCHWELLE_MASK, 
                                          PNIP_ACW_SND_WORD3_CT_SCHWELLE_SHIFT)
                }
                break;

                default:
                {
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiRTFrame(): FATAL -> ROGPhase unknown! ROGPhase=0x%X", 
                        pAcw->ROGPhase);
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }
                break;
            }
        }
        break;

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiRTFrame(): FATAL -> Hardware(0x%X) is not supported!", 
                pDDB->Hardware);
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }
    #else
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD3_ACW_WAIT_OFF_VALUE, 
                          PNIP_ACW_SND_WORD3_ACW_WAIT_MASK, 
                          PNIP_ACW_SND_WORD3_ACW_WAIT_SHIFT)
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD3_CT_SCHWELLE_1_VALUE, 
                          PNIP_ACW_SND_WORD3_CT_SCHWELLE_MASK, 
                          PNIP_ACW_SND_WORD3_CT_SCHWELLE_SHIFT)
    #endif
    
    /* ----------------------------------------------------------------------------------------------- */
    // Tx-Time: sending time (Time Queue-15)
    ValueTemp = 0;
    switch (pAcw->ROGPhase)
    {
        case EDDP_CRT_PROV_ACW_PHASE_G:
        case EDDP_CRT_PROV_ACW_PHASE_O:
            ValueTemp = PNIP_ACW_SND_WORD3_TX_TIME_OFF_VALUE;   // Green, Orange
        break;

        case EDDP_CRT_PROV_ACW_PHASE_R:
            ValueTemp = ( (pAcw->AddParam.FrameSendOffset) / (EDDP_UNIT_RESOLUTION_8) );    // Red
        break;

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiRTFrame(): FATAL -> ROGPhase unknown! ROGPhase=0x%X", 
                pAcw->ROGPhase);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          ValueTemp, 
                          PNIP_ACW_SND_WORD3_TX_TIME_MASK, 
                          PNIP_ACW_SND_WORD3_TX_TIME_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // reserved and DS_Handling (dependent of PN-IP revision)
    switch (EDDP_HALGetPnipRevision(pDDB))
    {
        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
        case EDDP_HAL_HW_PNIP_REV1:
        {
            // reserved
            EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                  PNIP_ACW_SND_WORD3_RESERVED_VALUE, 
                                  PNIP_R1_ACW_SND_WORD3_RESERVED2_MASK_PNIP, 
                                  PNIP_R1_ACW_SND_WORD3_RESERVED2_SHIFT_PNIP)
        }
        break;
        #endif

        #ifdef EDDP_CFG_HW_PNIP_REV2_SUPPORT
        case EDDP_HAL_HW_PNIP_REV2:
        {
            // reserved
            EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                  PNIP_ACW_SND_WORD3_RESERVED_VALUE, 
                                  PNIP_R2_ACW_SND_WORD3_RESERVED2_MASK_PNIP, 
                                  PNIP_R2_ACW_SND_WORD3_RESERVED2_SHIFT_PNIP)
            // DS_Handling: is this System-Redundancy-Provider?
            if (pProvider->ProviderFlags & EDDP_CRT_PROV_FLAGS_IS_SYSRED)
            {
                // SRD provider
                EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                      PNIP_R2_ACW_SND_WORD3_DS_HANDLING_SAVE_VALUE,
                                      PNIP_R2_ACW_SND_WORD3_DS_HANDLING_MASK_PNIP, 
                                      PNIP_R2_ACW_SND_WORD3_DS_HANDLING_SHIFT_PNIP)
            }
            else
            {
                // not SRD provider
                EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                      PNIP_R2_ACW_SND_WORD3_DS_HANDLING_NOT_SAVE_VALUE, 
                                      PNIP_R2_ACW_SND_WORD3_DS_HANDLING_MASK_PNIP,
                                      PNIP_R2_ACW_SND_WORD3_DS_HANDLING_SHIFT_PNIP)
            }
        }
        break;
        #endif

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiRTFrame(): FATAL -> Version(%d) of PNIP is unknown", 
                EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }
    /* ----------------------------------------------------------------------------------------------- */
    // Ressource_Raw_Mode: get "ResourceType" by ACW.ResourceType
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD3_RESSOURCE_RAW_MODE_ACW_VALUE, 
                          PNIP_ACW_SND_WORD3_RESSOURCE_RAW_MODE_MASK, 
                          PNIP_ACW_SND_WORD3_RESSOURCE_RAW_MODE_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // ResourceType
    ValueTemp = 0;
    /* ---------------------------------------- */
    /* ResourceType(RT_L)   : RTC2, RTC1        */
    /* ResourceType(RT_H1)  : RTC3 Port1        */
    /* ResourceType(RT_H2)  : RTC3 Port2        */
    /* ---------------------------------------- */
    switch (pAcw->ROGPhase)
    {
        case EDDP_CRT_PROV_ACW_PHASE_G:
        case EDDP_CRT_PROV_ACW_PHASE_O:
        {
            ValueTemp = PNIP_ACW_SND_WORD3_RESSOURCE_TYPE_RT_L_VALUE;
        }
        break;

        case EDDP_CRT_PROV_ACW_PHASE_R:
        {
            // distinction after HW because of different number of ports
            switch (pDDB->Hardware)
            {
                case EDDP_HW_ERTEC200P:
                case EDDP_HW_ERTEC200P_FPGA:
                {
                    // get Hw-PortID from User-PortID
                    switch (EDDP_SWI_get_HwPortID(pDDB, pAcw->AddParam.TxPortID))
                    {
                        case 1:     ValueTemp = PNIP_ACW_SND_WORD3_RESSOURCE_TYPE_RT_H1_VALUE;  break;
                        case 2:     ValueTemp = PNIP_ACW_SND_WORD3_RESSOURCE_TYPE_RT_H2_VALUE;  break;
                        case 3:     ValueTemp = PNIP_ACW_SND_WORD3_RESSOURCE_TYPE_RT_H3_VALUE;  break;
                        case 4:     ValueTemp = PNIP_ACW_SND_WORD3_RESSOURCE_TYPE_RT_H4_VALUE;  break;

                        default:
                        {
                            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiRTFrame(): FATAL -> Invalid Hw-PortID from SWI! Acw-PortID=%d, Hw-PortID=%d", 
                                pAcw->AddParam.TxPortID, EDDP_SWI_get_HwPortID (pDDB, pAcw->AddParam.TxPortID));
                            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                        }
                        break;
                    }
                }
                break;

                default:
                {
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiRTFrame(): FATAL -> Hardware(0x%X) is not supported!", 
                        pDDB->Hardware);
                    EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                }
                break;
            }
        }
        break;

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiRTFrame(): FATAL -> ROGPhase unknown! ROGPhase=0x%X", 
                pAcw->ROGPhase);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          ValueTemp, 
                          PNIP_R2_ACW_SND_WORD3_RESSOURCE_TYPE_MASK, 
                          PNIP_R2_ACW_SND_WORD3_RESSOURCE_TYPE_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */

    // write HOST value to APICtrl-RAM
    EDDP_HAL_MEM64_WRITE (pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->ACW_Word3, ACW_Word64);

    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    /* ACW.Word4                                                                                       */
    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    ACW_Word64 = 0;
    /* ----------------------------------------------------------------------------------------------- */
    // Data_Ptr
    // to prevent compiler warning
    User_Addr   = 0;
    Pnip_Addr   = EDDP_CRT_INVALID_VALUE_32;

    if (pProvider->AddParam.DataOffset == EDD_DATAOFFSET_UNDEFINED)
    {
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiRTFrame(): FATAL -> DataOffset=UNDEFINED!");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        if (pProvider->AddParam.CRNumber == EDD_CRNUMBER_NOT_USED)
        {
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiRTFrame(): FATAL -> extarnel RAM is not supported!");
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        else
        {
            // use PERIF
            EDDP_ASSERT_NO_NULL_PTR(pDDB->CRT.pIOBaseAdrPerif);
            User_Addr = ((pDDB->CRT.pIOBaseAdrPerif) + (pProvider->AddParam.DataOffset));
            // convert upper memory pointer to 32-Bit physical pnip address
            EDDP_IO_PERIF_ADDR_UPPER_TO_PNIP (pDDB->hSysDev, User_Addr, &Pnip_Addr);
            // the Pnip_Addr must be 8 byte aligned - check it
            switch (pDDB->Hardware)
            {
                case EDDP_HW_ERTEC200P:
                {
                    // nothing to do
                }
                break;
                
                case EDDP_HW_ERTEC200P_FPGA:
                {
                    if ( !(EDDP_GlbIsAligned(pDDB, Pnip_Addr, EDDP_GLB_ALIGNMENT_8_BYTE)) )
                    {
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiRTFrame(): FATAL -> Pnip_Addr(0x%X) is not 8 byte aligned!", 
                            Pnip_Addr);
                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                    }
                }
                break;

                default:
                {
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiRTFrame(): FATAL -> Hardware(0x%X) is not supported!", 
                        pDDB->Hardware);
                    EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                }
                break;
            }
        }

        EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProvAcwInitHwApiRTFrame(): CRNumber=%d, DataOffset=0x%X, User_Addr=0x%X, Pnip_Addr=0x%X", 
            pProvider->AddParam.CRNumber, pProvider->AddParam.DataOffset, User_Addr, Pnip_Addr);
    }
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          Pnip_Addr, 
                          PNIP_R2_ACW_SND_WORD4_DATA_PTR_MASK, 
                          PNIP_R2_ACW_SND_WORD4_DATA_PTR_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // DataStart_offset: tbd. for IRT+
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD4_DATASTART_OFFSET_VALUE, 
                          PNIP_R2_ACW_SND_WORD4_DATASTART_OFFSET_MASK,
                          PNIP_R2_ACW_SND_WORD4_DATASTART_OFFSET_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // DataLength: data length of only IO-Data between FrameID and APDU-Status
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          pProvider->AddParam.DataLen, 
                          PNIP_ACW_SND_WORD4_DATALENGTH_DATASTART_OFFSET2_MASK, 
                          PNIP_ACW_SND_WORD4_DATALENGTH_DATASTART_OFFSET2_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // reserved
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD4_RESERVED_VALUE, 
                          PNIP_R2_ACW_SND_WORD4_RESERVED1_MASK, 
                          PNIP_R2_ACW_SND_WORD4_RESERVED1_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // Scatter_select (only in PN-IP Rev1)
    switch (EDDP_HALGetPnipRevision(pDDB))
    {
        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
        case EDDP_HAL_HW_PNIP_REV1:
        {
            EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                  PNIP_ACW_SND_WORD4_SCATTER_SELECT_OFF_VALUE, 
                                  PNIP_R2_ACW_SND_WORD4_SCATTER_SELECT_MASK, 
                                  PNIP_R2_ACW_SND_WORD4_SCATTER_SELECT_SHIFT)
        }
        break;
        #endif

        #ifdef EDDP_CFG_HW_PNIP_REV2_SUPPORT
        case EDDP_HAL_HW_PNIP_REV2:
        {
            // Scatter_select is not present in PN-IP Rev2
        }
        break;
        #endif

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiRTFrame(): FATAL -> Version(%d) of PNIP is unknown", 
                EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }
    /* ----------------------------------------------------------------------------------------------- */
    // CR_Number
    if ((pProvider->AddParam.CRNumber == EDD_CRNUMBER_UNDEFINED) || (pProvider->AddParam.CRNumber > EDDP_CRT_MAX_CRNUMBER_PERIF_ERTEC200P))
    {
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiRTFrame(): FATAL -> CRNumber(%d) is UNDEFINED or out of range! CRNumberMax=%d", 
            pProvider->AddParam.CRNumber, EDDP_CRT_MAX_CRNUMBER_PERIF_ERTEC200P);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        EDDP_SET_BIT_VALUE64( ACW_Word64, 
                              pProvider->AddParam.CRNumber, 
                              PNIP_R2_ACW_SND_WORD4_CR_NUMBER_MASK, 
                              PNIP_R2_ACW_SND_WORD4_CR_NUMBER_SHIFT)
    }
    /* ----------------------------------------------------------------------------------------------- */
    // Buffer_Mode (only used in PN-IP Rev1)
    switch (EDDP_HALGetPnipRevision(pDDB))
    {
        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
        case EDDP_HAL_HW_PNIP_REV1:
        {
            EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                  PNIP_ACW_SND_WORD4_BUFFER_MODE_NORMAL_VALUE, 
                                  PNIP_R2_ACW_SND_WORD4_BUFFER_MODE_MASK, 
                                  PNIP_R2_ACW_SND_WORD4_BUFFER_MODE_SHIFT)
        }
        break;
        #endif

        #ifdef EDDP_CFG_HW_PNIP_REV2_SUPPORT
        case EDDP_HAL_HW_PNIP_REV2:
        {
            // Buffer_Mode is not present in PN-IP Rev2
        }
        break;
        #endif

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiRTFrame(): FATAL -> Version(%d) of PNIP is unknown", 
                EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }
    /* ----------------------------------------------------------------------------------------------- */
    // App_Group
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          pProvider->AddParam.AppGroup, 
                          PNIP_R2_ACW_SND_WORD4_APP_GROUP_MASK, 
                          PNIP_R2_ACW_SND_WORD4_APP_GROUP_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */

    // write HOST value to APICtrl-RAM
    EDDP_HAL_MEM64_WRITE (pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->ACW_Word4, ACW_Word64);

    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    /* ACW.Word5: only for RToverUDP (ACW.RT_class = UDP)                                              */
    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    ACW_Word64 = 0;

    if (pProvider->RTClass == EDDP_CRT_RTCLASS_UDP)
    {
        /* ----------------------------------------------------------------------------------------------- */
        // reserved
        EDDP_SET_BIT_VALUE64( ACW_Word64, 
                              PNIP_ACW_SND_WORD5_RESERVED_VALUE, 
                              PNIP_ACW_SND_WORD5_RESERVED_MASK, 
                              PNIP_ACW_SND_WORD5_RESERVED_SHIFT)
        /* ----------------------------------------------------------------------------------------------- */
        // IP_SA_select: select Source IP Address
        ValueTemp = 0;

        // IFA
        ValueTemp = PNIP_ACW_SND_WORD5_IP_SA_SELECT_INDEX0_VALUE_VALUE;
        
        EDDP_SET_BIT_VALUE64( ACW_Word64, 
                              ValueTemp, 
                              PNIP_ACW_SND_WORD5_IP_SA_SELECT_MASK, 
                              PNIP_ACW_SND_WORD5_IP_SA_SELECT_SHIFT)
        /* ----------------------------------------------------------------------------------------------- */
        // UDP_SP_select: get UDP-SrcPort aus Register "Local_SPort_Etype_x"
        EDDP_SET_BIT_VALUE64( ACW_Word64, 
                              PNIP_ACW_SND_WORD5_UDP_SP_SELECT_INDEX0_VALUE, 
                              PNIP_ACW_SND_WORD5_UDP_SP_SELECT_MASK, 
                              PNIP_ACW_SND_WORD5_UDP_SP_SELECT_SHIFT)
        /* ----------------------------------------------------------------------------------------------- */
        // IP_DA: Destination IP Address   -> Byte 3
        EDDP_SET_BIT_VALUE64( ACW_Word64, 
                              pProvider->AddParam.DstIP.IPAdr[3], 
                              PNIP_ACW_SND_WORD5_IP_DA_0_MASK, 
                              PNIP_ACW_SND_WORD5_IP_DA_0_SHIFT)
        /* ----------------------------------------------------------------------------------------------- */
        // IP_DA: Destination IP Address   -> Byte 2
        EDDP_SET_BIT_VALUE64( ACW_Word64, 
                              pProvider->AddParam.DstIP.IPAdr[2], 
                              PNIP_ACW_SND_WORD5_IP_DA_1_MASK, 
                              PNIP_ACW_SND_WORD5_IP_DA_1_SHIFT)
        /* ----------------------------------------------------------------------------------------------- */
        // IP_DA: Destination IP Address   -> Byte 1
        EDDP_SET_BIT_VALUE64( ACW_Word64, 
                              pProvider->AddParam.DstIP.IPAdr[1], 
                              PNIP_ACW_SND_WORD5_IP_DA_2_MASK, 
                              PNIP_ACW_SND_WORD5_IP_DA_2_SHIFT)
        /* ----------------------------------------------------------------------------------------------- */
        // IP_DA: Destination IP Address   -> Byte 0
        EDDP_SET_BIT_VALUE64( ACW_Word64, 
                              pProvider->AddParam.DstIP.IPAdr[0], 
                              PNIP_ACW_SND_WORD5_IP_DA_3_MASK, 
                              PNIP_ACW_SND_WORD5_IP_DA_3_SHIFT)
        /* ----------------------------------------------------------------------------------------------- */
        // UDP_DP: UDP Destination-Port
        EDDP_SET_BIT_VALUE64( ACW_Word64, 
                              PNIP_ACW_SND_WORD5_UDP_DP_VALUE, 
                              PNIP_ACW_SND_WORD5_UDP_DP_MASK, 
                              PNIP_ACW_SND_WORD5_UDP_DP_SHIFT)
        /* ----------------------------------------------------------------------------------------------- */
    }

    // write HOST value to APICtrl-RAM
    EDDP_HAL_MEM64_WRITE (pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->ACW_Word5, ACW_Word64);

    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    /* ACW.PpmSwData                                                                                   */
    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    EDDP_HAL_MEM16_WRITE (pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->PpmSwData.ProviderId, ProviderID);

    /* ----------------------------------------------------------------------------------------------- */
    /* ACW in HW available                                                                             */
    /* ----------------------------------------------------------------------------------------------- */
    EDDP_SET_BIT_VALUE16( (pAcw->State), 
                          EDDP_CRT_PROV_ACW_STATE_BIT_ON, 
                          EDDP_CRT_PROV_ACW_STATE_IN_HW_MASK, 
                          EDDP_CRT_PROV_ACW_STATE_IN_HW_SHIFT)

    EDDP_LL_CRTProvAcwInitHwApiRTFrame(pDDB, ProviderID, AcwID);
}
#endif


#if defined (EDDP_CFG_HW_ERTEC200P_SUPPORT)
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProvAcwInitHwApiPackFrame                       +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      AcwID                   +*/
/*+                             LSA_BOOL                        InActiveRecord_A        +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  AcwID              : Index in ACW array                                            +*/
/*+  InActiveRecord_A   : LSA_TRUE  - search in active record buffer (A)                +*/
/*+                       LSA_FALSE - search in new record buffer (B)                   +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Init the send ACW for pack frame in APICtrl-RAM.                      +*/
/*+               Use ACW.Word1..3                                                      +*/
/*+                                                                                     +*/
/*+               This pack ACW must be already connected to PDIRSubframeData!          +*/
/*+                                                                                     +*/
/*+               Only for RED ACWs!                                                    +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwInitHwApiPackFrame(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID,
    LSA_BOOL                    InActiveRecord_A)
{
    LSA_UINT64                                          ACW_Word64;
    LSA_UINT32                                          ValueTemp;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE                     pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE                      pAcw;
    EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_PTR_TYPE    pSubframeBlock;

    EDDP_ASSERT_FALSE(AcwID >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pAcw            = &pProvMngm->pAcwEntry[AcwID];
    pAcw->pHwAcwApi = &((PNIP_ACW_SND_PTR_TYPE)(pProvMngm->pHwBaseAcwApi))[AcwID];

    EDDP_ASSERT_FALSE(pAcw->ROGPhase != EDDP_CRT_PROV_ACW_PHASE_R);

    // is DFP (PDIRSubframeData and PDIRData) present ?
    if ( !(EDDP_PRMPDIRSubframeDataIsDfpPresent(pDDB, InActiveRecord_A)) )
    {
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiPackFrame(AcwID=%d, InActiveRecord_A=0x%X): FATAL -> The record PDIRSubframeData or PDIRData is not present!", 
            AcwID, InActiveRecord_A);
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }

    // init pointer to PDIRSubframeData
    EDDP_ASSERT_FALSE(pAcw->DfpParam.PrmSubframeBlockIdx == EDDP_CRT_CHAIN_IDX_END);
    if (InActiveRecord_A)
    {
        pSubframeBlock = &pDDB->PRM.pPDIRSubframeData_A->SubframeBlockArray[pAcw->DfpParam.PrmSubframeBlockIdx];
    }
    else
    {
        pSubframeBlock = &pDDB->PRM.pPDIRSubframeData_B->SubframeBlockArray[pAcw->DfpParam.PrmSubframeBlockIdx];
    }

    EDDP_CRT_TRACE_07(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProvAcwInitHwApiPackFrame(AcwID=%d, InActiveRecord_A=0x%X): AcwDGCnt=%d, Acw-State=0x%X, PrmSubframeBlockIdx=%d, DFPmode=%d, HwConfig=0x%X", 
        AcwID, 
        InActiveRecord_A, 
        pAcw->DfpParam.AcwDGCnt, 
        pAcw->State, 
        pAcw->DfpParam.PrmSubframeBlockIdx, 
        pSubframeBlock->SFIOCRProperties.DFPmode, 
        pSubframeBlock->HwConfig);

    if (pAcw->ROGPhase != EDDP_CRT_PROV_ACW_PHASE_R)
    {
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiPackFrame(AcwID=%d): FATAL -> The given ACW is not red! ROGPhase=0x%X", 
            AcwID, pAcw->ROGPhase);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    /* ACW.Word1                                                                                       */
    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    ACW_Word64 = 0;
    /* ----------------------------------------------------------------------------------------------- */
    // FrameID to be sent
    EDDP_ASSERT_FALSE(pAcw->AddParam.FrameID == EDD_FRAME_ID_UNDEFINED);
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          pAcw->AddParam.FrameID, 
                          PNIP_ACW_RCV_WORD1_FRAMEID_MASK, 
                          PNIP_ACW_SND_WORD1_FRAMEID_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // ACW_nextoffset: Offset of next ACW in APICtrl-RAM (init ACW_nextoffset to begin as last ACW)
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_R2_ACW_SND_WORD1_ACW_NEXTOFFSET_INIT_VALUE,
                          PNIP_R2_ACW_SND_WORD1_ACW_NEXTOFFSET_MASK,
                          PNIP_ACW_SND_WORD1_ACW_NEXTOFFSET_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // ACW_DG_offset: Offset of first ACW_DG in APICtrl-RAM which belongs to this ACW (init ACW_DG_offset to begin as last ACW)
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_R2_ACW_SND_WORD1_ACW_DG_OFFSET_INIT_VALUE,
                          PNIP_R2_ACW_SND_WORD1_ACW_DG_OFFSET_MASK,
                          PNIP_ACW_SND_WORD1_ACW_DG_OFFSET_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // Opcode for ACW Send
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD1_OPCODE_ACW_SND_VALUE, 
                          PNIP_ACW_SND_WORD1_OPCODE_MASK, 
                          PNIP_ACW_SND_WORD1_OPCODE_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // Active Bit (Activate/Deactivate this ACW - will be set by command "ACW_on_off" (PROVIDER_CONTROL)
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD1_ACTIVE_INIT_VALUE,
                          PNIP_ACW_SND_WORD1_ACTIVE_MASK, 
                          PNIP_ACW_SND_WORD1_ACTIVE_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // FirstSend Bit - after first sending the bit will be reseted by HW
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD1_FIRSTSEND_INIT_VALUE, 
                          PNIP_ACW_SND_WORD1_FIRSTSEND_MASK, 
                          PNIP_ACW_SND_WORD1_FIRSTSEND_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // DP_Raw_Mode  : DataPlane specification for IRTtop
    // DP2_Type     : DP1/DP2 (DP2 = send only if Red is active)
    // with DataPlane specification
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD1_DP_RAW_MODE_ON_VALUE, 
                          PNIP_ACW_SND_WORD1_DP_RAW_MODE_MASK, 
                          PNIP_ACW_SND_WORD1_DP_RAW_MODE_SHIFT)
    // DP2_Type = DP2
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD1_DP2_TYPE_DP2_VALUE, 
                          PNIP_ACW_SND_WORD1_DP2_TYPE_MASK, 
                          PNIP_ACW_SND_WORD1_DP2_TYPE_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // EtherType: Index 0-3 to select one of Register "Local_SPort_Etype_0-3"
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD1_ETHERTYPE_SELECT_INDEX0_VALUE, 
                          PNIP_ACW_SND_WORD1_ETHERTYPE_SELECT_MASK, 
                          PNIP_ACW_SND_WORD1_ETHERTYPE_SELECT_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // InBound_Pack: Bit for info the pack frame is (not) a InBound-Pack-Frame
    // if redundant FrameID, it is valid for FrameID(x) and FrameID(x+1)
    if (pSubframeBlock->HwConfig & EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_HWCONFIG_SND_ACW_INBOUND_PACK_MASK)
    {
        EDDP_SET_BIT_VALUE64( ACW_Word64, 
                              PNIP_ACW_SND_WORD1_INBOUND_PACK_ON_VALUE, 
                              PNIP_ACW_SND_WORD1_INBOUND_PACK_MASK, 
                              PNIP_ACW_SND_WORD1_INBOUND_PACK_SHIFT)
    }
    else
    {
        EDDP_SET_BIT_VALUE64( ACW_Word64, 
                              PNIP_ACW_SND_WORD1_INBOUND_PACK_OFF_VALUE, 
                              PNIP_ACW_SND_WORD1_INBOUND_PACK_MASK, 
                              PNIP_ACW_SND_WORD1_INBOUND_PACK_SHIFT)
    }
    /* ----------------------------------------------------------------------------------------------- */
    // RT_class: RT-Frame or RToverUDP-Frame
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD1_RT_CLASS_RT_VALUE, 
                          PNIP_ACW_SND_WORD1_RT_CLASS_MASK, 
                          PNIP_ACW_SND_WORD1_RT_CLASS_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // ACW_DG_order: ascending or descending order for sending of ACW_DGs
    switch (pAcw->DfpParam.ACW_DG_order)
    {
        case EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_HIGH:
        {
            EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                  PNIP_ACW_SND_WORD1_ACW_DG_ORDER_HIGH_VALUE, 
                                  PNIP_ACW_SND_WORD1_ACW_DG_ORDER_MASK, 
                                  PNIP_ACW_SND_WORD1_ACW_DG_ORDER_SHIFT)
        }
        break;

        case EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_DOWN:
        {
            EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                  PNIP_ACW_SND_WORD1_ACW_DG_ORDER_DOWN_VALUE, 
                                  PNIP_ACW_SND_WORD1_ACW_DG_ORDER_MASK, 
                                  PNIP_ACW_SND_WORD1_ACW_DG_ORDER_SHIFT)
        }
        break;

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiPackFrame(): FATAL -> ACW_DG_order(0x%X) is unknown!", 
                pAcw->DfpParam.ACW_DG_order);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }
    /* ----------------------------------------------------------------------------------------------- */
    // DRM: Destination Raw Mode for port
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD1_DRM_OFF_VALUE, 
                          PNIP_ACW_SND_WORD1_DRM_MASK, 
                          PNIP_ACW_SND_WORD1_DRM_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // Destination: port specification (PortID)
    ValueTemp = 0;
    if (pAcw->AddParam.TxPortID == EDD_PORT_ID_AUTO)
    {
        EDDP_SET_BIT_VALUE64( ACW_Word64, 
                              PNIP_ACW_SND_WORD1_DESTINATION_OFF_VALUE, 
                              PNIP_ACW_SND_WORD1_DESTINATION_MASK, 
                              PNIP_ACW_SND_WORD1_DESTINATION_SHIFT)
    }
    else
    {
        // get Hw-PortID from User-PortID
        switch (EDDP_SWI_get_HwPortID(pDDB, pAcw->AddParam.TxPortID))
        {
            case 1:     ValueTemp = PNIP_ACW_SND_WORD1_DESTINATION_P1_VALUE;    break;
            case 2:     ValueTemp = PNIP_ACW_SND_WORD1_DESTINATION_P2_VALUE;    break;
            case 3:     ValueTemp = PNIP_ACW_SND_WORD1_DESTINATION_P3_VALUE;    break;
            case 4:     ValueTemp = PNIP_ACW_SND_WORD1_DESTINATION_P4_VALUE;    break;
            default:
            {
                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiPackFrame(): FATAL -> Invalid Hw-PortID from SWI! Acw-PortID=%d, Hw-PortID=%d", 
                    pAcw->AddParam.TxPortID, EDDP_SWI_get_HwPortID (pDDB, pAcw->AddParam.TxPortID));
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
        EDDP_SET_BIT_VALUE64( ACW_Word64, 
                              ValueTemp, 
                              PNIP_ACW_SND_WORD1_DESTINATION_MASK, 
                              PNIP_ACW_SND_WORD1_DESTINATION_SHIFT)
    }
    /* ----------------------------------------------------------------------------------------------- */

    // write HOST value to APICtrl-RAM
    EDDP_HAL_MEM64_WRITE (pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->ACW_Word1, ACW_Word64);

    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    /* ACW.Word2                                                                                       */
    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    ACW_Word64 = 0;
    /* ----------------------------------------------------------------------------------------------- */
    // DstMAC-Address to be sent    -> Byte 5
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          pAcw->AddParam.DstMAC.MacAdr[5], 
                          PNIP_ACW_SND_WORD2_MAC_DA_0_MASK, 
                          PNIP_ACW_SND_WORD2_MAC_DA_0_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // DstMAC-Address to be sent    -> Byte 4
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          pAcw->AddParam.DstMAC.MacAdr[4], 
                          PNIP_ACW_SND_WORD2_MAC_DA_1_MASK, 
                          PNIP_ACW_SND_WORD2_MAC_DA_1_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // DstMAC-Address to be sent    -> Byte 3
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          pAcw->AddParam.DstMAC.MacAdr[3], 
                          PNIP_ACW_SND_WORD2_MAC_DA_2_MASK, 
                          PNIP_ACW_SND_WORD2_MAC_DA_2_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // DstMAC-Address to be sent    -> Byte 2
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          pAcw->AddParam.DstMAC.MacAdr[2], 
                          PNIP_ACW_SND_WORD2_MAC_DA_3_MASK, 
                          PNIP_ACW_SND_WORD2_MAC_DA_3_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // DstMAC-Address to be sent    -> Byte 1
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          pAcw->AddParam.DstMAC.MacAdr[1], 
                          PNIP_ACW_SND_WORD2_MAC_DA_4_MASK, 
                          PNIP_ACW_SND_WORD2_MAC_DA_4_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // DstMAC-Address to be sent    -> Byte 0
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          pAcw->AddParam.DstMAC.MacAdr[0], 
                          PNIP_ACW_SND_WORD2_MAC_DA_5_MASK, 
                          PNIP_ACW_SND_WORD2_MAC_DA_5_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // Q_Select: info which queue is used by frame
    ValueTemp = 0;
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD2_Q_SELECT_TIME_VALUE, 
                          PNIP_ACW_SND_WORD2_Q_SELECT_MASK, 
                          PNIP_ACW_SND_WORD2_Q_SELECT_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // PhasenInfo: which phases are used in QCW
    ValueTemp = 0;
    // Red, Orange, Green phases and TransferType
    ValueTemp = ( PNIP_ACW_SND_WORD2_PHASENINFO_RED_ON_VALUE | 
                  PNIP_ACW_SND_WORD2_PHASENINFO_ORANGE_ON_VALUE | 
                  PNIP_ACW_SND_WORD2_PHASENINFO_GREEN_ON_VALUE | 
                  PNIP_ACW_SND_WORD2_PHASENINFO_TRANSFERTYPE_ON_VALUE);
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          ValueTemp, 
                          PNIP_ACW_SND_WORD2_PHASENINFO_MASK, 
                          PNIP_ACW_SND_WORD2_PHASENINFO_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // Tx_TimeMode: for sending in Time Queue-15 (Minumum/ExactTime)
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD2_TX_TIMEMODE_EXACT, 
                          PNIP_ACW_SND_WORD2_TX_TIMEMODE_MASK, 
                          PNIP_ACW_SND_WORD2_TX_TIMEMODE_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // DA_Select: select the source of DstMAC-Address
    ValueTemp = 0;
    if (   (pAcw->AddParam.DstMAC.MacAdr[0] == 0)
        && (pAcw->AddParam.DstMAC.MacAdr[1] == 0)
        && (pAcw->AddParam.DstMAC.MacAdr[2] == 0)
        && (pAcw->AddParam.DstMAC.MacAdr[3] == 0)
        && (pAcw->AddParam.DstMAC.MacAdr[4] == 0)
        && (pAcw->AddParam.DstMAC.MacAdr[5] == 0)
       )
    {
        // Bit for info: get DstMAC-Address from Register "Router_MAC_DA"
        ValueTemp = PNIP_ACW_SND_WORD2_DA_SELECT_REG_VALUE;
    }
    else
    {
        // Bit for info: get DstMAC-Address from ACW.MAC_DA
        ValueTemp = PNIP_ACW_SND_WORD2_DA_SELECT_ACW_VALUE;
    }
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          ValueTemp, 
                          PNIP_ACW_SND_WORD2_DA_SELECT_MASK, 
                          PNIP_ACW_SND_WORD2_DA_SELECT_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // VLAN_Tag: Frame with or without VLAN_Tag
    // Frame without VLAN_Tag
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD2_VLAN_TAG_OFF_VALUE, 
                          PNIP_ACW_SND_WORD2_VLAN_TAG_MASK, 
                          PNIP_ACW_SND_WORD2_VLAN_TAG_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // VLAN_Prio: VLAN priority
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD2_VLAN_PRIO_INIT_VALUE, 
                          PNIP_ACW_SND_WORD2_VLAN_PRIO_MASK, 
                          PNIP_ACW_SND_WORD2_VLAN_PRIO_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // UseType_Raw_Mode: Bit for info how get the "UseType"
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD2_USETYPE_RAW_MODE_FRAME_CLASS_VALUE, 
                          PNIP_ACW_SND_WORD2_USETYPE_RAW_MODE_MASK, 
                          PNIP_ACW_SND_WORD2_USETYPE_RAW_MODE_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // UseType specification
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD2_USETYPE_STD_VALUE, 
                          PNIP_ACW_SND_WORD2_USETYPE_MASK, 
                          PNIP_ACW_SND_WORD2_USETYPE_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */

    // write HOST value to APICtrl-RAM
    EDDP_HAL_MEM64_WRITE (pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->ACW_Word2, ACW_Word64);

    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    /* ACW.Word3                                                                                       */
    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    ACW_Word64 = 0;
    /* ----------------------------------------------------------------------------------------------- */
    // Scoreboard_offset: Pointer to Score Board of this Consumer (AutoStopConsumerID)
    // is always OFF for pack frame, because it is saved in ACW_DG
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_R2_ACW_SND_WORD3_SCOREBOARD_OFFSET_OFF_VALUE, 
                          PNIP_R2_ACW_SND_WORD3_SCOREBOARD_OFFSET_MASK,
                          PNIP_ACW_SND_WORD3_SCOREBOARD_OFFSET_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // DataStatus_offset: index to DataStatus struct in APICtrl-RAM
    // here invalid, because it is saved in ACW_DG
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          pAcw->DataStatusId, 
                          PNIP_ACW_SND_WORD3_DATASTATUS_OFFSET_MASK, 
                          PNIP_ACW_SND_WORD3_DATASTATUS_OFFSET_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // ACW_Wait and CT_Schwelle set always together for SDRAM/PERIF
    // PERIF
    #ifdef EDDP_CRT_SUPPORT_ACW_WAIT_ON
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD3_ACW_WAIT_ON_VALUE, 
                          PNIP_ACW_SND_WORD3_ACW_WAIT_MASK, 
                          PNIP_ACW_SND_WORD3_ACW_WAIT_SHIFT)
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD3_CT_SCHWELLE_0_VALUE, 
                          PNIP_ACW_SND_WORD3_CT_SCHWELLE_MASK, 
                          PNIP_ACW_SND_WORD3_CT_SCHWELLE_SHIFT)
    #else
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD3_ACW_WAIT_OFF_VALUE, 
                          PNIP_ACW_SND_WORD3_ACW_WAIT_MASK, 
                          PNIP_ACW_SND_WORD3_ACW_WAIT_SHIFT)
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD3_CT_SCHWELLE_1_VALUE, 
                          PNIP_ACW_SND_WORD3_CT_SCHWELLE_MASK, 
                          PNIP_ACW_SND_WORD3_CT_SCHWELLE_SHIFT)
    #endif
    /* ----------------------------------------------------------------------------------------------- */
    // Tx-Time: sending time (Time Queue-15)
    ValueTemp = ( (pAcw->AddParam.FrameSendOffset) / (EDDP_UNIT_RESOLUTION_8) );    // Red
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          ValueTemp, 
                          PNIP_ACW_SND_WORD3_TX_TIME_MASK, 
                          PNIP_ACW_SND_WORD3_TX_TIME_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // reserved and DS_Handling (dependent of PN-IP revision)
    switch (EDDP_HALGetPnipRevision(pDDB))
    {
        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
        case EDDP_HAL_HW_PNIP_REV1:
        {
            // reserved
            EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                  PNIP_ACW_SND_WORD3_RESERVED_VALUE, 
                                  PNIP_R1_ACW_SND_WORD3_RESERVED2_MASK_PNIP, 
                                  PNIP_R1_ACW_SND_WORD3_RESERVED2_SHIFT_PNIP)
        }
        break;
        #endif

        
        #ifdef EDDP_CFG_HW_PNIP_REV2_SUPPORT
        case EDDP_HAL_HW_PNIP_REV2:
        {
            // reserved
            EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                  PNIP_ACW_SND_WORD3_RESERVED_VALUE, 
                                  PNIP_R2_ACW_SND_WORD3_RESERVED2_MASK_PNIP, 
                                  PNIP_R2_ACW_SND_WORD3_RESERVED2_SHIFT_PNIP)
            // DS_Handling: is only for System-Redundancy-Provider, for pack frames is always OFF
            EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                  PNIP_R2_ACW_SND_WORD3_DS_HANDLING_NOT_SAVE_VALUE, 
                                  PNIP_R2_ACW_SND_WORD3_DS_HANDLING_MASK_PNIP,
                                  PNIP_R2_ACW_SND_WORD3_DS_HANDLING_SHIFT_PNIP)
        }
        break;
        #endif

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiPackFrame(): FATAL -> Version(%d) of PNIP is unknown", 
                EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }
    /* ----------------------------------------------------------------------------------------------- */
    // Ressource_Raw_Mode: get "ResourceType" by ACW.ResourceType
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACW_SND_WORD3_RESSOURCE_RAW_MODE_ACW_VALUE, 
                          PNIP_ACW_SND_WORD3_RESSOURCE_RAW_MODE_MASK, 
                          PNIP_ACW_SND_WORD3_RESSOURCE_RAW_MODE_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // ResourceType
    ValueTemp = 0;
    /* ---------------------------------------- */
    /* ResourceType(RT_H1) for RTC3 Port1       */
    /* ResourceType(RT_H2) for RTC3 Port2       */
    /* ---------------------------------------- */
    // distinction after HW because of different number of ports
    switch (pDDB->Hardware)
    {
        case EDDP_HW_ERTEC200P:
        case EDDP_HW_ERTEC200P_FPGA:
        {
            // get Hw-PortID from User-PortID
            switch (EDDP_SWI_get_HwPortID(pDDB, pAcw->AddParam.TxPortID))
            {
                case 1:     ValueTemp = PNIP_ACW_SND_WORD3_RESSOURCE_TYPE_RT_H1_VALUE;  break;
                case 2:     ValueTemp = PNIP_ACW_SND_WORD3_RESSOURCE_TYPE_RT_H2_VALUE;  break;
                case 3:     ValueTemp = PNIP_ACW_SND_WORD3_RESSOURCE_TYPE_RT_H3_VALUE;  break;
                case 4:     ValueTemp = PNIP_ACW_SND_WORD3_RESSOURCE_TYPE_RT_H4_VALUE;  break;

                default:
                {
                    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiPackFrame(): FATAL -> Invalid Hw-PortID from SWI! Acw-PortID=%d, Hw-PortID=%d", 
                        pAcw->AddParam.TxPortID, EDDP_SWI_get_HwPortID (pDDB, pAcw->AddParam.TxPortID));
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }
                break;
            }
        }
        break;

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwInitHwApiPackFrame(): FATAL -> Hardware(0x%X) is not supported!", 
                pDDB->Hardware);
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          ValueTemp, 
                          PNIP_R2_ACW_SND_WORD3_RESSOURCE_TYPE_MASK, 
                          PNIP_R2_ACW_SND_WORD3_RESSOURCE_TYPE_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */

    // write HOST value to APICtrl-RAM
    EDDP_HAL_MEM64_WRITE (pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->ACW_Word3, ACW_Word64);

    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    /* ACW.Word4                                                                                       */
    /* ACW.Word5                                                                                       */
    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    ACW_Word64 = 0;
    /* ----------------------------------------------------------------------------------------------- */
    // write HOST value to APICtrl-RAM
    EDDP_HAL_MEM64_WRITE (pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->ACW_Word4, ACW_Word64);
    EDDP_HAL_MEM64_WRITE (pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->ACW_Word5, ACW_Word64);

    /* ----------------------------------------------------------------------------------------------- */
    /* ACW in HW available                                                                             */
    /* ----------------------------------------------------------------------------------------------- */
    EDDP_SET_BIT_VALUE16( (pAcw->State), 
                          EDDP_CRT_PROV_ACW_STATE_BIT_ON, 
                          EDDP_CRT_PROV_ACW_STATE_IN_HW_MASK, 
                          EDDP_CRT_PROV_ACW_STATE_IN_HW_SHIFT)
}
#endif

#if defined (EDDP_CFG_HW_ERTEC200P_SUPPORT)
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProvAcwDGInitHwApiFromProvider                  +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      ProviderID              +*/
/*+                             LSA_UINT16                      AcwDGID                 +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  ProviderID         : Index in provider array                                       +*/
/*+  AcwDGID            : Index in ACW_DG array                                         +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Init ACW_DG in APICtrl-RAM by call from provider.                     +*/
/*+                                                                                     +*/
/*+               Call this function only after coping of all provider and ACW_DG       +*/
/*+               parameter!                                                            +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDGInitHwApiFromProvider(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID,
    LSA_UINT16                  AcwDGID)
{
    LSA_UINT64                              ACW_Word64;
    EDD_UPPER_MEM_U8_PTR_TYPE               User_Addr;
    LSA_UINT32                              Pnip_Addr;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE              pProvider;
    EDDP_CRT_PROVIDER_ACW_DG_PTR_TYPE       pAcwDG;

    pProvMngm        = &pDDB->CRT.Prov;
    pProvider           = &pProvMngm->pProvEntry[ProviderID];
    pAcwDG              = &pProvMngm->pAcwDGEntry[AcwDGID];
    pAcwDG->pHwAcwDGApi = &pProvMngm->pHwBaseAcwDGApi[AcwDGID];

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProvAcwDGInitHwApiFromProvider(ProviderID=%d, AcwDGID=%d)", 
        ProviderID, AcwDGID);

    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    /* ACW_DG.Word1                                                                                    */
    /* is already set by call from PrmCommit, first read Word1 from APICtrl-RAM                        */
    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    ACW_Word64 = EDDP_HAL_MEM64_READ (pDDB, &pAcwDG->pHwAcwDGApi->ACW_Word1);
    /* ----------------------------------------------------------------------------------------------- */
    // DataStatus_offset: index to DataStatus struct in APICtrl-RAM
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          pProvider->DataStatusId, 
                          PNIP_ACWDG_SND_WORD1_DATASTATUS_OFFSET_MASK, 
                          PNIP_ACWDG_SND_WORD1_DATASTATUS_OFFSET_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */

    // write HOST value to APICtrl-RAM
    EDDP_HAL_MEM64_WRITE (pDDB, &pAcwDG->pHwAcwDGApi->ACW_Word1, ACW_Word64);

    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    /* ACW_DG.Word2                                                                                    */
    /* is already set by call from PrmCommit, first read Word2 from APICtrl-RAM                        */
    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    ACW_Word64 = EDDP_HAL_MEM64_READ (pDDB, &pAcwDG->pHwAcwDGApi->ACW_Word2);
    /* ----------------------------------------------------------------------------------------------- */
    // Scoreboard_offset: Pointer to Score Board of this Consumer (AutoStopConsumerID)
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_R2_ACWDG_SND_WORD2_SCOREBOARD_OFFSET_OFF_VALUE, 
                          PNIP_R2_ACWDG_SND_WORD2_SCOREBOARD_OFFSET_MASK, 
                          PNIP_ACWDG_SND_WORD2_SCOREBOARD_OFFSET_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // Data_Ptr
    // to prevent compiler warning
    User_Addr   = 0;
    Pnip_Addr   = EDDP_CRT_INVALID_VALUE_32;

    if (pProvider->AddParam.DataOffset == EDD_DATAOFFSET_UNDEFINED)
    {
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwDGInitHwApiFromProvider(): FATAL -> DataOffset=UNDEFINED!");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        if (pProvider->AddParam.CRNumber == EDD_CRNUMBER_NOT_USED)
        {
            // use extarnel RAM
            {        
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwDGInitHwApiFromProvider(): FATAL -> extarnel RAM is not supported!");
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
        }
        else
        {
            // use PERIF
            EDDP_ASSERT_NO_NULL_PTR(pDDB->CRT.pIOBaseAdrPerif);
            User_Addr = ((pDDB->CRT.pIOBaseAdrPerif) + (pProvider->AddParam.DataOffset));
            // convert upper memory pointer to 32-Bit physical pnip address
            EDDP_IO_PERIF_ADDR_UPPER_TO_PNIP (pDDB->hSysDev, User_Addr, &Pnip_Addr);
            // the Pnip_Addr must be 8 byte aligned - check it
            switch (pDDB->Hardware)
            {
                case EDDP_HW_ERTEC200P:
                {
                    // nothing to do
                }
                break;
                                
                case EDDP_HW_ERTEC200P_FPGA:
                case EDDP_HW_HERA_FPGA:    
                {
                    if ( !(EDDP_GlbIsAligned(pDDB, Pnip_Addr, EDDP_GLB_ALIGNMENT_8_BYTE)) )
                    {
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwDGInitHwApiFromProvider(): FATAL -> Pnip_Addr(0x%X) is not 8 byte aligned!", 
                            Pnip_Addr);
                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                    }
                }
                break;

                default:
                {
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwDGInitHwApiFromProvider(): FATAL -> Hardware(0x%X) is not supported!", 
                        pDDB->Hardware);
                    EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                }
                break;
            }
        }

        EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProvAcwDGInitHwApiFromProvider(): CRNumber=%d, DataOffset=0x%X, User_Addr=0x%X, Pnip_Addr=0x%X", 
            pProvider->AddParam.CRNumber, pProvider->AddParam.DataOffset, User_Addr, Pnip_Addr);
    }
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          Pnip_Addr, 
                          PNIP_R2_ACWDG_SND_WORD2_DATA_PTR_MASK, 
                          PNIP_R2_ACWDG_SND_WORD2_DATA_PTR_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // DataLength
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          pProvider->AddParam.DataLen, 
                          PNIP_R2_ACWDG_SND_WORD2_DATALENGTH_MASK, 
                          PNIP_R2_ACWDG_SND_WORD2_DATALENGTH_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // CR_Number
    if ((pProvider->AddParam.CRNumber == EDD_CRNUMBER_UNDEFINED) || (pProvider->AddParam.CRNumber > EDDP_CRT_MAX_CRNUMBER_PERIF_ERTEC200P))
    {
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwDGInitHwApiFromProvider(): FATAL -> CRNumber(%d) is UNDEFINED or out of range! CRNumberMax=%d", 
            pProvider->AddParam.CRNumber, EDDP_CRT_MAX_CRNUMBER_PERIF_ERTEC200P);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        EDDP_SET_BIT_VALUE64( ACW_Word64, 
                              pProvider->AddParam.CRNumber, 
                              PNIP_R2_ACWDG_SND_WORD2_CR_NUMBER_MASK,
                              PNIP_R2_ACWDG_SND_WORD2_CR_NUMBER_SHIFT)
    }
    /* ----------------------------------------------------------------------------------------------- */
    // Buffer_Mode (only used in PN-IP Rev1)
    switch (EDDP_HALGetPnipRevision(pDDB))
    {
        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
        case EDDP_HAL_HW_PNIP_REV1:
        {
            EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                  PNIP_ACWDG_SND_WORD2_BUFFER_MODE_NORMAL_VALUE, 
                                  PNIP_R2_ACWDG_SND_WORD2_BUFFER_MODE_MASK,
                                  PNIP_R2_ACWDG_SND_WORD2_BUFFER_MODE_SHIFT)
        }
        break;
        #endif

        #if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
        case EDDP_HAL_HW_PNIP_REV2:
        {
            // Buffer_Mode is not present in PN-IP Rev2
        }
        break;
        #endif
        
        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwDGInitHwApiFromProvider(): FATAL -> Version(%d) of PNIP is unknown", 
                EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }
    /* ----------------------------------------------------------------------------------------------- */
    // App_Group
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          pProvider->AddParam.AppGroup, 
                          PNIP_R2_ACWDG_SND_WORD2_APP_GROUP_MASK,
                          PNIP_R2_ACWDG_SND_WORD2_APP_GROUP_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */

    // write HOST value to APICtrl-RAM
    EDDP_HAL_MEM64_WRITE (pDDB, &pAcwDG->pHwAcwDGApi->ACW_Word2, ACW_Word64);
}
#endif

#if defined (EDDP_CFG_HW_ERTEC200P_SUPPORT)
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProvAcwDGInitHwApiFromPrmCommit                 +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      AcwDGID                 +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                             +*/
/*+  AcwDGID            : Index in ACW_DG array                                         +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Init ACW_DG in APICtrl-RAM by call from PrmCommit.                    +*/
/*+                                                                                     +*/
/*+               Some parameters are not yet known here, so set it to default.         +*/
/*+               These parameters will be updated by call from Provider-Activate in    +*/
/*+               EDDP_CRTProvAcwDGInitHwApiFromProvider().                             +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDGInitHwApiFromPrmCommit(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwDGID)
{
    LSA_UINT64                              ACW_Word64;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_ACW_DG_PTR_TYPE       pAcwDG;

    pProvMngm        = &pDDB->CRT.Prov;
    pAcwDG              = &pProvMngm->pAcwDGEntry[AcwDGID];
    pAcwDG->pHwAcwDGApi = &pProvMngm->pHwBaseAcwDGApi[AcwDGID];

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProvAcwDGInitHwApiFromPrmCommit(AcwDGID=%d)", AcwDGID);

    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    /* ACW_DG.Word1                                                                                    */
    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    ACW_Word64 = 0;
    /* ----------------------------------------------------------------------------------------------- */
    // DG_PosT
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          pAcwDG->AddParam.DG_PosT, 
                          PNIP_ACWDG_SND_WORD1_DG_POST_MASK, 
                          PNIP_ACWDG_SND_WORD1_DG_POST_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // DG_Len
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          pAcwDG->AddParam.DG_Len, 
                          PNIP_ACWDG_SND_WORD1_DG_LEN_MASK, 
                          PNIP_ACWDG_SND_WORD1_DG_LEN_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // ACW_DG_nextoffset0
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_R2_ACWDG_SND_WORD1_ACW_DG_NEXTOFFSET_INIT_VALUE,
                          PNIP_R2_ACWDG_SND_WORD1_ACW_DG_NEXTOFFSET0_MASK,
                          PNIP_ACWDG_SND_WORD1_ACW_DG_NEXTOFFSET0_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // ACW_DG_nextoffset1
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_R2_ACWDG_SND_WORD1_ACW_DG_NEXTOFFSET_INIT_VALUE,
                          PNIP_R2_ACWDG_SND_WORD1_ACW_DG_NEXTOFFSET1_MASK,
                          PNIP_ACWDG_SND_WORD1_ACW_DG_NEXTOFFSET1_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // Opcode
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACWDG_SND_WORD1_OPCODE_ACWDG_SND_VALUE, 
                          PNIP_ACWDG_SND_WORD1_OPCODE_MASK, 
                          PNIP_ACWDG_SND_WORD1_OPCODE_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // Active
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACWDG_SND_WORD1_ACTIVE_INIT_VALUE, 
                          PNIP_ACWDG_SND_WORD1_ACTIVE_MASK, 
                          PNIP_ACWDG_SND_WORD1_ACTIVE_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // FirstSend
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACWDG_SND_WORD1_FIRSTSEND_INIT_VALUE, 
                          PNIP_ACWDG_SND_WORD1_FIRSTSEND_MASK, 
                          PNIP_ACWDG_SND_WORD1_FIRSTSEND_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */
    // Scatter_select (only in PN-IP Rev1)
    switch (EDDP_HALGetPnipRevision(pDDB))
    {
        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
        case EDDP_HAL_HW_PNIP_REV1:
        {
            EDDP_SET_BIT_VALUE64( ACW_Word64, 
                                  PNIP_R1_ACWDG_SND_WORD1_SCATTER_SELECT_OFF_VALUE, 
                                  PNIP_R1_ACWDG_SND_WORD1_SCATTER_SELECT_MASK, 
                                  PNIP_R1_ACWDG_SND_WORD1_SCATTER_SELECT_SHIFT)
        }
        break;
        #endif

        #if (defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT))
        case EDDP_HAL_HW_PNIP_REV2:
        {
            // Scatter_select is not present in PN-IP Rev2
        }
        break;
        #endif

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwDGInitHwApiFromPrmCommit(): FATAL -> Version(%d) of PNIP is unknown", 
                EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }
    /* ----------------------------------------------------------------------------------------------- */
    // DataStatus_offset: index to DataStatus struct in APICtrl-RAM
    // here invalid, because it will be set from provider
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_ACWDG_SND_WORD1_DATASTATUS_OFFSET_INIT_VALUE, 
                          PNIP_ACWDG_SND_WORD1_DATASTATUS_OFFSET_MASK, 
                          PNIP_ACWDG_SND_WORD1_DATASTATUS_OFFSET_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */

    // write HOST value to APICtrl-RAM
    EDDP_HAL_MEM64_WRITE (pDDB, &pAcwDG->pHwAcwDGApi->ACW_Word1, ACW_Word64);

    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    /* ACW_DG.Word2                                                                                    */
    /* ----------------------------------------------------------------------------------------------- */
    /* ----------------------------------------------------------------------------------------------- */
    ACW_Word64 = 0;
    /* ----------------------------------------------------------------------------------------------- */
    // Scoreboard_offset: Pointer to Score Board of this Consumer (AutoStopConsumerID)
    EDDP_SET_BIT_VALUE64( ACW_Word64, 
                          PNIP_R2_ACWDG_SND_WORD2_SCOREBOARD_OFFSET_OFF_VALUE,
                          PNIP_R2_ACWDG_SND_WORD2_SCOREBOARD_OFFSET_MASK,
                          PNIP_ACWDG_SND_WORD2_SCOREBOARD_OFFSET_SHIFT)
    /* ----------------------------------------------------------------------------------------------- */

    // write HOST value to APICtrl-RAM
    EDDP_HAL_MEM64_WRITE (pDDB, &pAcwDG->pHwAcwDGApi->ACW_Word2, ACW_Word64);

    /* ----------------------------------------------------------------------------------------------- */
    /* ACW_DG in HW available                                                                          */
    /* ----------------------------------------------------------------------------------------------- */
    EDDP_SET_BIT_VALUE16( (pAcwDG->State), 
                          EDDP_CRT_PROV_ACW_STATE_BIT_ON, 
                          EDDP_CRT_PROV_ACW_STATE_IN_HW_MASK, 
                          EDDP_CRT_PROV_ACW_STATE_IN_HW_SHIFT)
}
#endif

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvAcwPhaseListUpdate_O_G                          +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             LSA_UINT16                          AddRemove               +*/
/*+                             LSA_UINT16                          TrogID                  +*/
/*+                             LSA_UINT16                          AcwID                   +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  AddRemove          : Add or Remove                                                     +*/
/*+  TrogID             : Index in Trog array                                               +*/
/*+  AcwID              : Index in ACW array                                                +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Update AcwPhaseList (Orange/Green) by Provider-Add or Provider-Remove.    +*/
/*+                                                                                         +*/
/*+               ACW is sorted by CyclePhaseSequence.                                      +*/
/*+                                                                                         +*/
/*+               Chain this ACW to or from AcwPhaseList_O_G:   ACW(1) -> ACW(2) ... ACW(n) +*/
/*+                                                                                         +*/
/*+               ACW must be already added to provider!                                    +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwPhaseListUpdate_O_G(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AddRemove,
    LSA_UINT16                  TrogID,
    LSA_UINT16                  AcwID)
{
    LSA_UINT32                              NewCyclePhaseSequenceUser;
    LSA_UINT16                              PhaseUsedIdx;
    LSA_UINT16                              PhaseUsedIdxPrev;   // for add or remove between Prev and Next in send list
    LSA_UINT16                              PhaseUsedIdxNext;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
//  EDDP_CRT_PROVIDER_DDB_MNGM_PTR_TYPE     pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;
    EDDP_CRT_PROVIDER_TROG_PTR_TYPE         pTrog;
    EDDP_CRT_CHAIN_BASE_TYPE                AcwPhaseList;   // AcwPhaseList_O, AcwPhaseList_G
    LSA_UINT32                              AcwPhaseCnt;    // AcwPhaseCnt_O, AcwPhaseCnt_G
    LSA_UINT32                              AcwPhaseActCnt; // AcwPhaseActCnt_O, AcwPhaseActCnt_G

    EDDP_ASSERT_FALSE(TrogID >= pDDB->CRT.CfgParam.ProviderTrogMaxNumber);
    EDDP_ASSERT_FALSE(AcwID >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

    pProvMngm            = &pDDB->CRT.Prov;
    pProvMngm            = &pDDB->CRT.Prov;
    pAcw                    = &pProvMngm->pAcwEntry[AcwID];
    pTrog                   = &pProvMngm->pTrogEntry[TrogID];
    // to prevent compiler warning
    AcwPhaseList.BeginIdx   = EDDP_CRT_CHAIN_IDX_END;
    AcwPhaseList.EndIdx     = EDDP_CRT_CHAIN_IDX_END;
    AcwPhaseCnt             = 0;
    AcwPhaseActCnt          = 0;

    switch (pAcw->ROGPhase)
    {
        case EDDP_CRT_PROV_ACW_PHASE_G:
        {
            AcwPhaseList    = pTrog->TrogAcwList.AcwPhaseList_G;
            AcwPhaseCnt     = pTrog->TrogAcwList.AcwPhaseCnt_G;
            AcwPhaseActCnt  = pTrog->TrogAcwList.AcwPhaseActCnt_G;
        }
        break;

        case EDDP_CRT_PROV_ACW_PHASE_O:
        {
            AcwPhaseList    = pTrog->TrogAcwList.AcwPhaseList_O;
            AcwPhaseCnt     = pTrog->TrogAcwList.AcwPhaseCnt_O;
            AcwPhaseActCnt  = pTrog->TrogAcwList.AcwPhaseActCnt_O;
        }
        break;

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwPhaseListUpdate_O_G(): FATAL -> ROGPhase unknown/unsupported in this function! ROGPhase=0x%X", 
                pAcw->ROGPhase);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }

    if (AddRemove == EDDP_CRT_ADD)
    {
        /* ---------------------------------------------------------------------------- */
        /* ACW is added                                                                 */
        /* ---------------------------------------------------------------------------- */
        if (!(pAcw->State & EDDP_CRT_PROV_ACW_STATE_IN_PHASELIST_MASK))
        {
            NewCyclePhaseSequenceUser = pAcw->AddParam.CyclePhaseSequence;

            if (AcwPhaseList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
            {
                // this ACW is the first ACW in the Phase list
                AcwPhaseList.BeginIdx       = AcwID;
                AcwPhaseList.EndIdx         = AcwID;
                pAcw->AcwPhaseLink.Prev     = EDDP_CRT_CHAIN_IDX_END;
                pAcw->AcwPhaseLink.Next     = EDDP_CRT_CHAIN_IDX_END;
            }
            else
            {
                // this ACW is not the first ACW in the Phase list
                if (NewCyclePhaseSequenceUser == EDD_CYCLE_PHASE_SEQUENCE_LAST)
                {
                    // chain this ACW to end in Phase list
                    pAcw->AcwPhaseLink.Prev                                         = AcwPhaseList.EndIdx;
                    pAcw->AcwPhaseLink.Next                                         = EDDP_CRT_CHAIN_IDX_END;
                    pProvMngm->pAcwEntry[AcwPhaseList.EndIdx].AcwPhaseLink.Next  = AcwID;
                    AcwPhaseList.EndIdx                                             = AcwID;
                }
                else
                {
                    // chain this ACW to position "NewCyclePhaseSequenceUser" in Phase list
                    if (pProvMngm->pAcwEntry[AcwPhaseList.BeginIdx].AddParam.CyclePhaseSequence == EDD_CYCLE_PHASE_SEQUENCE_LAST)
                    {
                        // all ACW in Phase list have (CyclePhaseSequence = LAST)
                        // chain to begin in Phase list
                        pAcw->AcwPhaseLink.Prev                                             = EDDP_CRT_CHAIN_IDX_END;
                        pAcw->AcwPhaseLink.Next                                             = AcwPhaseList.BeginIdx;
                        pProvMngm->pAcwEntry[AcwPhaseList.BeginIdx].AcwPhaseLink.Prev    = AcwID;
                        AcwPhaseList.BeginIdx                                               = AcwID;
                    }
                    else
                    {
                        // there is one or more ACW in Phase list that have (CyclePhaseSequence != 0)
                        PhaseUsedIdx = AcwPhaseList.BeginIdx;

                        // go through Phase list from BeginIdx to EndIdx
                        while (   (NewCyclePhaseSequenceUser >= pProvMngm->pAcwEntry[PhaseUsedIdx].AddParam.CyclePhaseSequence)
                               && (pProvMngm->pAcwEntry[PhaseUsedIdx].AddParam.CyclePhaseSequence != EDD_CYCLE_PHASE_SEQUENCE_LAST)
                               && (PhaseUsedIdx != AcwPhaseList.EndIdx)
                              )
                        {
                            EDDP_ASSERT_FALSE(PhaseUsedIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);
                            // compare next PhaseUsedIdx
                            PhaseUsedIdx = pProvMngm->pAcwEntry[PhaseUsedIdx].AcwPhaseLink.Next;
                        }

                        // init PhaseUsedIdxPrev and PhaseUsedIdxNext
                        if ((PhaseUsedIdx == AcwPhaseList.BeginIdx) || (PhaseUsedIdx == AcwPhaseList.EndIdx))
                        {
                            if (   (NewCyclePhaseSequenceUser < pProvMngm->pAcwEntry[PhaseUsedIdx].AddParam.CyclePhaseSequence)
                                || (pProvMngm->pAcwEntry[PhaseUsedIdx].AddParam.CyclePhaseSequence == EDD_CYCLE_PHASE_SEQUENCE_LAST)
                               )
                            {
                                // chain before PhaseUsedIdx
                                PhaseUsedIdxPrev = pProvMngm->pAcwEntry[PhaseUsedIdx].AcwPhaseLink.Prev;
                                PhaseUsedIdxNext = PhaseUsedIdx;
                            }
                            else
                            {
                                // chain behind PhaseUsedIdx
                                PhaseUsedIdxPrev = PhaseUsedIdx;
                                PhaseUsedIdxNext = pProvMngm->pAcwEntry[PhaseUsedIdx].AcwPhaseLink.Next;
                            }
                        }
                        else
                        {
                            if (NewCyclePhaseSequenceUser == pProvMngm->pAcwEntry[PhaseUsedIdx].AddParam.CyclePhaseSequence)
                            {
                                // chain behind PhaseUsedIdx
                                PhaseUsedIdxPrev = PhaseUsedIdx;
                                PhaseUsedIdxNext = pProvMngm->pAcwEntry[PhaseUsedIdx].AcwPhaseLink.Next;
                            }
                            else
                            {
                                // chain before PhaseUsedIdx
                                PhaseUsedIdxPrev = pProvMngm->pAcwEntry[PhaseUsedIdx].AcwPhaseLink.Prev;
                                PhaseUsedIdxNext = PhaseUsedIdx;
                            }
                        }

                        // chain between PhaseUsedIdxPrev and PhaseUsedIdxNext
                        if ((PhaseUsedIdxPrev == EDDP_CRT_CHAIN_IDX_END) || (PhaseUsedIdxNext == EDDP_CRT_CHAIN_IDX_END))
                        {
                            // chain to begin or end in Phase list
                            if (PhaseUsedIdxPrev == EDDP_CRT_CHAIN_IDX_END)
                            {
                                if (PhaseUsedIdxNext == EDDP_CRT_CHAIN_IDX_END)
                                {
                                    // this case is for the first ACW in the Phase list - it was added above
                                }
                                else
                                {
                                    // chain to begin in Phase list
                                    pAcw->AcwPhaseLink.Prev                                             = EDDP_CRT_CHAIN_IDX_END;
                                    pAcw->AcwPhaseLink.Next                                             = AcwPhaseList.BeginIdx;
                                    pProvMngm->pAcwEntry[AcwPhaseList.BeginIdx].AcwPhaseLink.Prev    = AcwID;
                                    AcwPhaseList.BeginIdx                                               = AcwID;
                                }
                            }
                            else
                            {
                                // chain to end in Phase list
                                pAcw->AcwPhaseLink.Prev                                         = AcwPhaseList.EndIdx;
                                pAcw->AcwPhaseLink.Next                                         = EDDP_CRT_CHAIN_IDX_END;
                                pProvMngm->pAcwEntry[AcwPhaseList.EndIdx].AcwPhaseLink.Next  = AcwID;
                                AcwPhaseList.EndIdx                                             = AcwID;
                            }
                        }
                        else
                        {
                            // chain to middle in Phase list
                            pAcw->AcwPhaseLink.Prev                                     = PhaseUsedIdxPrev;
                            pAcw->AcwPhaseLink.Next                                     = PhaseUsedIdxNext;
                            pProvMngm->pAcwEntry[PhaseUsedIdxPrev].AcwPhaseLink.Next = AcwID;
                            pProvMngm->pAcwEntry[PhaseUsedIdxNext].AcwPhaseLink.Prev = AcwID;
                        }
                    }
                }
            }

            EDDP_SET_BIT_VALUE16( (pAcw->State), 
                                  EDDP_CRT_PROV_ACW_STATE_BIT_ON, 
                                  EDDP_CRT_PROV_ACW_STATE_IN_PHASELIST_MASK, 
                                  EDDP_CRT_PROV_ACW_STATE_IN_PHASELIST_SHIFT)

            AcwPhaseCnt++;

            // is this ACW active ?
            if (pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK)
            {
                AcwPhaseActCnt++;
            }
        }
    }

    else
    {
        /* ---------------------------------------------------------------------------- */
        /* ACW is removed                                                               */
        /* ---------------------------------------------------------------------------- */
        if (pAcw->State & EDDP_CRT_PROV_ACW_STATE_IN_PHASELIST_MASK)
        {
            // this ACW is between Prev and Next
            PhaseUsedIdxPrev    = pAcw->AcwPhaseLink.Prev;
            PhaseUsedIdxNext    = pAcw->AcwPhaseLink.Next;

            if ((PhaseUsedIdxPrev == EDDP_CRT_CHAIN_IDX_END) || (PhaseUsedIdxNext == EDDP_CRT_CHAIN_IDX_END))
            {
                if (PhaseUsedIdxPrev == EDDP_CRT_CHAIN_IDX_END)
                {
                    // this ACW is first ACW in Phase list
                    if (PhaseUsedIdxNext == EDDP_CRT_CHAIN_IDX_END)
                    {
                        // first and only one ACW in Phase list
                        AcwPhaseList.BeginIdx   = EDDP_CRT_CHAIN_IDX_END;
                        AcwPhaseList.EndIdx     = EDDP_CRT_CHAIN_IDX_END;
                    }
                    else
                    {
                        // first ACW in Phase list but not only one
                        AcwPhaseList.BeginIdx                                       = PhaseUsedIdxNext;
                        pProvMngm->pAcwEntry[PhaseUsedIdxNext].AcwPhaseLink.Prev = EDDP_CRT_CHAIN_IDX_END;
                    }
                }
                else
                {
                    // last ACW in Phase list but not only one
                    pProvMngm->pAcwEntry[PhaseUsedIdxPrev].AcwPhaseLink.Next = EDDP_CRT_CHAIN_IDX_END;
                    AcwPhaseList.EndIdx                                         = PhaseUsedIdxPrev;
                }
            }
            else
            {
                // this ACW is in the middle of Phase list
                pProvMngm->pAcwEntry[PhaseUsedIdxPrev].AcwPhaseLink.Next = PhaseUsedIdxNext;
                pProvMngm->pAcwEntry[PhaseUsedIdxNext].AcwPhaseLink.Prev = PhaseUsedIdxPrev;
            }

            EDDP_SET_BIT_VALUE16( (pAcw->State), 
                                  EDDP_CRT_PROV_ACW_STATE_BIT_OFF, 
                                  EDDP_CRT_PROV_ACW_STATE_IN_PHASELIST_MASK, 
                                  EDDP_CRT_PROV_ACW_STATE_IN_PHASELIST_SHIFT)

            EDDP_ASSERT_FALSE(AcwPhaseCnt == 0);
            AcwPhaseCnt--;

            // is this ACW active ?
            if (pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK)
            {
                EDDP_ASSERT_FALSE(AcwPhaseActCnt == 0);
                AcwPhaseActCnt--;
            }
        }
    }

    // write values back
    switch (pAcw->ROGPhase)
    {
        case EDDP_CRT_PROV_ACW_PHASE_G:
        {
            pTrog->TrogAcwList.AcwPhaseList_G   = AcwPhaseList;
            pTrog->TrogAcwList.AcwPhaseCnt_G    = AcwPhaseCnt;
            pTrog->TrogAcwList.AcwPhaseActCnt_G = AcwPhaseActCnt;
        }
        break;

        case EDDP_CRT_PROV_ACW_PHASE_O:
        {
            pTrog->TrogAcwList.AcwPhaseList_O   = AcwPhaseList;
            pTrog->TrogAcwList.AcwPhaseCnt_O    = AcwPhaseCnt;
            pTrog->TrogAcwList.AcwPhaseActCnt_O = AcwPhaseActCnt;
        }
        break;

        case EDDP_CRT_PROV_ACW_PHASE_R:
        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwPhaseListUpdate_O_G(): FATAL -> ROGPhase unknown/unsupported in this function! ROGPhase=0x%X", 
                pAcw->ROGPhase);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvAcwPhaseListUpdate_R                            +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             LSA_UINT16                          AddRemove               +*/
/*+                             LSA_UINT16                          TrogID                  +*/
/*+                             LSA_UINT16                          AcwID                   +*/
/*+                             LSA_BOOL                            isDFP                   +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  AddRemove          : Add or Remove                                                     +*/
/*+  TrogID             : Index in Trog array                                               +*/
/*+  AcwID              : Index in ACW array                                                +*/
/*+  isDFP              : is this DFP-ACW ?                                                 +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Update AcwPhaseList (Red) by Provider-Add or Provider-Remove.             +*/
/*+                                                                                         +*/
/*+               ACW is sorted by FrameSendOffset.                                         +*/
/*+                 - FrameSendOffset must be valid. If no -> FATAL!                        +*/
/*+                                                                                         +*/
/*+               Chain this ACW to or from AcwPhaseList_R:     ACW(1) -> ACW(2) ... ACW(n) +*/
/*+                                                                                         +*/
/*+               ACW must be already added to provider!                                    +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwPhaseListUpdate_R(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AddRemove,
    LSA_UINT16                  TrogID,
    LSA_UINT16                  AcwID,
    LSA_BOOL                    isDFP)
{
    LSA_UINT32                              NewFrameSendOffsetUser;
    LSA_UINT16                              PhaseUsedIdx;
    LSA_UINT16                              PhaseUsedIdxPrev;   // for add or remove between Prev and Next in send list
    LSA_UINT16                              PhaseUsedIdxNext;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
//  EDDP_CRT_PROVIDER_DDB_MNGM_PTR_TYPE     pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;
    EDDP_CRT_PROVIDER_TROG_PTR_TYPE         pTrog;
    EDDP_CRT_CHAIN_BASE_TYPE                AcwPhaseList;   // AcwPhaseList_R
    LSA_UINT32                              AcwPhaseCnt;    // AcwPhaseCnt_R
    LSA_UINT32                              AcwPhaseActCnt; // AcwPhaseActCnt_R
    LSA_UINT32                              AcwDfpPhaseCnt; // AcwDfpPhaseCnt_R

    EDDP_ASSERT_FALSE(TrogID >= pDDB->CRT.CfgParam.ProviderTrogMaxNumber);
    EDDP_ASSERT_FALSE(AcwID >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

    pProvMngm            = &pDDB->CRT.Prov;
    pProvMngm            = &pDDB->CRT.Prov;
    pAcw                    = &pProvMngm->pAcwEntry[AcwID];
    pTrog                   = &pProvMngm->pTrogEntry[TrogID];
    // to prevent compiler warning
    AcwPhaseList.BeginIdx   = EDDP_CRT_CHAIN_IDX_END;
    AcwPhaseList.EndIdx     = EDDP_CRT_CHAIN_IDX_END;
    AcwPhaseCnt             = 0;
    AcwPhaseActCnt          = 0;
    AcwDfpPhaseCnt          = 0;

    switch (pAcw->ROGPhase)
    {
        case EDDP_CRT_PROV_ACW_PHASE_R:
        {
            AcwPhaseList    = pTrog->TrogAcwList.AcwPhaseList_R;
            AcwPhaseCnt     = pTrog->TrogAcwList.AcwPhaseCnt_R;
            AcwPhaseActCnt  = pTrog->TrogAcwList.AcwPhaseActCnt_R;
            AcwDfpPhaseCnt  = pTrog->TrogAcwList.AcwDfpPhaseCnt_R;
        }
        break;

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwPhaseListUpdate_R(): FATAL -> ROGPhase unknown/unsupported in this function! ROGPhase=0x%X", 
                pAcw->ROGPhase);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }

    if (AddRemove == EDDP_CRT_ADD)
    {
        /* ---------------------------------------------------------------------------- */
        /* ACW is added                                                                 */
        /* ---------------------------------------------------------------------------- */
        if (!(pAcw->State & EDDP_CRT_PROV_ACW_STATE_IN_PHASELIST_MASK))
        {
            if (pAcw->AddParam.FrameSendOffset == EDDP_CRT_FRAME_SEND_OFFSET_UNDEFINED)
            {
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwPhaseListUpdate_R(): FATAL -> FrameSendOffset = UNDEFINED");
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            NewFrameSendOffsetUser = pAcw->AddParam.FrameSendOffset;

            if (AcwPhaseList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
            {
                // this ACW is the first ACW in the Phase list
                AcwPhaseList.BeginIdx       = AcwID;
                AcwPhaseList.EndIdx         = AcwID;
                pAcw->AcwPhaseLink.Prev     = EDDP_CRT_CHAIN_IDX_END;
                pAcw->AcwPhaseLink.Next     = EDDP_CRT_CHAIN_IDX_END;
            }
            else
            {
                // check the first/last ACWs in the Phase list have valid FrameSendOffset
                EDDP_ASSERT_FALSE(pProvMngm->pAcwEntry[AcwPhaseList.BeginIdx].AddParam.FrameSendOffset == EDDP_CRT_FRAME_SEND_OFFSET_UNDEFINED);
                EDDP_ASSERT_FALSE(pProvMngm->pAcwEntry[AcwPhaseList.EndIdx].AddParam.FrameSendOffset == EDDP_CRT_FRAME_SEND_OFFSET_UNDEFINED);

                // this ACW is not the first ACW in the Phase list
                if (NewFrameSendOffsetUser >= pProvMngm->pAcwEntry[AcwPhaseList.EndIdx].AddParam.FrameSendOffset)
                {
                    // chain this ACW to end in Phase list
                    pAcw->AcwPhaseLink.Prev                                         = AcwPhaseList.EndIdx;
                    pAcw->AcwPhaseLink.Next                                         = EDDP_CRT_CHAIN_IDX_END;
                    pProvMngm->pAcwEntry[AcwPhaseList.EndIdx].AcwPhaseLink.Next  = AcwID;
                    AcwPhaseList.EndIdx                                             = AcwID;
                }
                else
                {
                    if (NewFrameSendOffsetUser < pProvMngm->pAcwEntry[AcwPhaseList.BeginIdx].AddParam.FrameSendOffset)
                    {
                        // chain this ACW to begin in Phase list
                        pAcw->AcwPhaseLink.Prev                                             = EDDP_CRT_CHAIN_IDX_END;
                        pAcw->AcwPhaseLink.Next                                             = AcwPhaseList.BeginIdx;
                        pProvMngm->pAcwEntry[AcwPhaseList.BeginIdx].AcwPhaseLink.Prev    = AcwID;
                        AcwPhaseList.BeginIdx                                               = AcwID;
                    }
                    else
                    {
                        // chain to middle in Phase list
                        PhaseUsedIdx = AcwPhaseList.BeginIdx;

                        // go through Phase list from BeginIdx to EndIdx
                        while (   (NewFrameSendOffsetUser >= pProvMngm->pAcwEntry[PhaseUsedIdx].AddParam.FrameSendOffset)
                               && (PhaseUsedIdx != AcwPhaseList.EndIdx)
                              )
                        {
                            EDDP_ASSERT_FALSE(PhaseUsedIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);
                            // compare next PhaseUsedIdx
                            PhaseUsedIdx = pProvMngm->pAcwEntry[PhaseUsedIdx].AcwPhaseLink.Next;
                        }

                        // init PhaseUsedIdxPrev and PhaseUsedIdxNext
                        if ((PhaseUsedIdx == AcwPhaseList.BeginIdx) || (PhaseUsedIdx == AcwPhaseList.EndIdx))
                        {
                            if (NewFrameSendOffsetUser < pProvMngm->pAcwEntry[PhaseUsedIdx].AddParam.FrameSendOffset)
                            {
                                // chain before PhaseUsedIdx
                                PhaseUsedIdxPrev = pProvMngm->pAcwEntry[PhaseUsedIdx].AcwPhaseLink.Prev;
                                PhaseUsedIdxNext = PhaseUsedIdx;
                            }
                            else
                            {
                                // chain behind PhaseUsedIdx
                                PhaseUsedIdxPrev = PhaseUsedIdx;
                                PhaseUsedIdxNext = pProvMngm->pAcwEntry[PhaseUsedIdx].AcwPhaseLink.Next;
                            }
                        }
                        else
                        {
                            if (NewFrameSendOffsetUser == pProvMngm->pAcwEntry[PhaseUsedIdx].AddParam.FrameSendOffset)
                            {
                                // chain behind PhaseUsedIdx
                                PhaseUsedIdxPrev = PhaseUsedIdx;
                                PhaseUsedIdxNext = pProvMngm->pAcwEntry[PhaseUsedIdx].AcwPhaseLink.Next;
                            }
                            else
                            {
                                // chain before PhaseUsedIdx
                                PhaseUsedIdxPrev = pProvMngm->pAcwEntry[PhaseUsedIdx].AcwPhaseLink.Prev;
                                PhaseUsedIdxNext = PhaseUsedIdx;
                            }
                        }

                        // chain between PhaseUsedIdxPrev and PhaseUsedIdxNext
                        if ((PhaseUsedIdxPrev == EDDP_CRT_CHAIN_IDX_END) || (PhaseUsedIdxNext == EDDP_CRT_CHAIN_IDX_END))
                        {
                            // chain to begin or end in Phase list
                            if (PhaseUsedIdxPrev == EDDP_CRT_CHAIN_IDX_END)
                            {
                                if (PhaseUsedIdxNext == EDDP_CRT_CHAIN_IDX_END)
                                {
                                    // this case is for the first ACW in the Phase list - it was added above
                                }
                                else
                                {
                                    // chain to begin in Phase list
                                    pAcw->AcwPhaseLink.Prev                                             = EDDP_CRT_CHAIN_IDX_END;
                                    pAcw->AcwPhaseLink.Next                                             = AcwPhaseList.BeginIdx;
                                    pProvMngm->pAcwEntry[AcwPhaseList.BeginIdx].AcwPhaseLink.Prev    = AcwID;
                                    AcwPhaseList.BeginIdx                                               = AcwID;
                                }
                            }
                            else
                            {
                                // chain to end in Phase list
                                pAcw->AcwPhaseLink.Prev                                         = AcwPhaseList.EndIdx;
                                pAcw->AcwPhaseLink.Next                                         = EDDP_CRT_CHAIN_IDX_END;
                                pProvMngm->pAcwEntry[AcwPhaseList.EndIdx].AcwPhaseLink.Next  = AcwID;
                                AcwPhaseList.EndIdx                                             = AcwID;
                            }
                        }
                        else
                        {
                            // chain to middle in Phase list
                            pAcw->AcwPhaseLink.Prev                                     = PhaseUsedIdxPrev;
                            pAcw->AcwPhaseLink.Next                                     = PhaseUsedIdxNext;
                            pProvMngm->pAcwEntry[PhaseUsedIdxPrev].AcwPhaseLink.Next = AcwID;
                            pProvMngm->pAcwEntry[PhaseUsedIdxNext].AcwPhaseLink.Prev = AcwID;
                        }
                    }
                }
            }

            EDDP_SET_BIT_VALUE16( (pAcw->State), 
                                  EDDP_CRT_PROV_ACW_STATE_BIT_ON, 
                                  EDDP_CRT_PROV_ACW_STATE_IN_PHASELIST_MASK, 
                                  EDDP_CRT_PROV_ACW_STATE_IN_PHASELIST_SHIFT)

            AcwPhaseCnt++;

            if (isDFP)
            {
                AcwDfpPhaseCnt++;
            }

            // is this ACW active ?
            if (pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK)
            {
                AcwPhaseActCnt++;
            }
        }
    }

    else
    {
        /* ---------------------------------------------------------------------------- */
        /* ACW is removed                                                               */
        /* ---------------------------------------------------------------------------- */
        if (pAcw->State & EDDP_CRT_PROV_ACW_STATE_IN_PHASELIST_MASK)
        {
            // this ACW is between Prev and Next
            PhaseUsedIdxPrev    = pAcw->AcwPhaseLink.Prev;
            PhaseUsedIdxNext    = pAcw->AcwPhaseLink.Next;

            if ((PhaseUsedIdxPrev == EDDP_CRT_CHAIN_IDX_END) || (PhaseUsedIdxNext == EDDP_CRT_CHAIN_IDX_END))
            {
                if (PhaseUsedIdxPrev == EDDP_CRT_CHAIN_IDX_END)
                {
                    // this ACW is first ACW in Phase list
                    if (PhaseUsedIdxNext == EDDP_CRT_CHAIN_IDX_END)
                    {
                        // first and only one ACW in Phase list
                        AcwPhaseList.BeginIdx   = EDDP_CRT_CHAIN_IDX_END;
                        AcwPhaseList.EndIdx     = EDDP_CRT_CHAIN_IDX_END;
                    }
                    else
                    {
                        // first ACW in Phase list but not only one
                        AcwPhaseList.BeginIdx                                       = PhaseUsedIdxNext;
                        pProvMngm->pAcwEntry[PhaseUsedIdxNext].AcwPhaseLink.Prev = EDDP_CRT_CHAIN_IDX_END;
                    }
                }
                else
                {
                    // last ACW in Phase list but not only one
                    pProvMngm->pAcwEntry[PhaseUsedIdxPrev].AcwPhaseLink.Next = EDDP_CRT_CHAIN_IDX_END;
                    AcwPhaseList.EndIdx                                         = PhaseUsedIdxPrev;
                }
            }
            else
            {
                // this ACW is in the middle of Phase list
                pProvMngm->pAcwEntry[PhaseUsedIdxPrev].AcwPhaseLink.Next = PhaseUsedIdxNext;
                pProvMngm->pAcwEntry[PhaseUsedIdxNext].AcwPhaseLink.Prev = PhaseUsedIdxPrev;
            }

            EDDP_SET_BIT_VALUE16( (pAcw->State), 
                                  EDDP_CRT_PROV_ACW_STATE_BIT_OFF, 
                                  EDDP_CRT_PROV_ACW_STATE_IN_PHASELIST_MASK, 
                                  EDDP_CRT_PROV_ACW_STATE_IN_PHASELIST_SHIFT)

            EDDP_ASSERT_FALSE(AcwPhaseCnt == 0);
            AcwPhaseCnt--;

            if (isDFP)
            {
                EDDP_ASSERT_FALSE(AcwDfpPhaseCnt == 0);
                AcwDfpPhaseCnt--;
            }

            // is this ACW active ?
            if (pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK)
            {
                EDDP_ASSERT_FALSE(AcwPhaseActCnt == 0);
                AcwPhaseActCnt--;
            }
        }
    }

    // write values back
    switch (pAcw->ROGPhase)
    {
        case EDDP_CRT_PROV_ACW_PHASE_R:
        {
            pTrog->TrogAcwList.AcwPhaseList_R       = AcwPhaseList;
            pTrog->TrogAcwList.AcwPhaseCnt_R        = AcwPhaseCnt;
            pTrog->TrogAcwList.AcwPhaseActCnt_R     = AcwPhaseActCnt;
            pTrog->TrogAcwList.AcwDfpPhaseCnt_R     = AcwDfpPhaseCnt;
        }
        break;

        case EDDP_CRT_PROV_ACW_PHASE_G:
        case EDDP_CRT_PROV_ACW_PHASE_O:
        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwPhaseListUpdate_R(): FATAL -> ROGPhase unknown/unsupported in this function! ROGPhase=0x%X", 
                pAcw->ROGPhase);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }
}





#if defined (EDDP_CFG_HW_ERTEC200P_SUPPORT)
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvAcwUpdateAcwNextOffsetAcwHwApi                  +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                        +*/
/*+                             LSA_UINT16                      AcwID                       +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                     +*/
/*+  AcwID          : ACW in phase list which get New_ACW_nextoffset                        +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Update ACW.ACW_nextoffset in APICtrl-RAM.                                 +*/
/*+                                                                                         +*/
/*+               This ACW must be already added to AcwPhaseList and available in HW.       +*/
/*+               If no, nothing to do.                                                     +*/
/*+               If yes, IMPORTANT:                                                        +*/
/*+                 the next ACW must be already added to AcwPhaseList and available in HW  +*/
/*+                 because the "ACW_nextoffset" is the address of next ACW in APICtrl-RAM. +*/
/*+                 If no -> FATAL!                                                         +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwUpdateAcwNextOffsetAcwHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID)
{
    LSA_UINT32                              New_ACW_nextoffset;     // New_ACW_nextoffset = Offset of next ACW in APICtrl-RAM
    LSA_UINT64                              ACW_Word_Temp;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcwNext;

    EDDP_ASSERT_FALSE(AcwID >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

    New_ACW_nextoffset  = PNIP_R2_ACW_SND_WORD1_ACW_NEXTOFFSET_INIT_VALUE;
    pProvMngm        = &pDDB->CRT.Prov;
    pAcw                = &pProvMngm->pAcwEntry[AcwID];

    // only if this ACW is already added to AcwPhaseList and is available in HW
    if ((pAcw->State & EDDP_CRT_PROV_ACW_STATE_IN_PHASELIST_MASK) && (pAcw->State & EDDP_CRT_PROV_ACW_STATE_IN_HW_MASK))
    {
        if (pAcw->AcwPhaseLink.Next == EDDP_CRT_CHAIN_IDX_END)
        {
            // update last ACW in ACW list  -> PNIP_ACW_SND_WORD1_ACW_NEXTOFFSET_INIT_VALUE
        }
        else
        {
            pAcwNext = &pProvMngm->pAcwEntry[pAcw->AcwPhaseLink.Next];

            // next ACW must be already added to AcwPhaseList and available in HW
            if ((pAcwNext->State & EDDP_CRT_PROV_ACW_STATE_IN_PHASELIST_MASK) && (pAcwNext->State & EDDP_CRT_PROV_ACW_STATE_IN_HW_MASK))
            {
                // address (offset) of next ACW in APICtrl-RAM
                New_ACW_nextoffset  = EDDP_CRTGetPnipAddrShift (
                                            pDDB, 
                                            (EDD_COMMON_MEM_PTR_TYPE) pAcwNext->pHwAcwApi, 
                                            EDDP_CRT_SHIFT_VALUE_3);
            }
            else
            {
                EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwUpdateAcwNextOffsetAcwHwApi(AcwID=%d): FATAL -> This ACW has next ACW (NextAcwID=%d) in PhaseList but the next ACW ist not available in HW or is not chained in AcwPhaseList! NextAcw-State=0x%X", 
                    AcwID, pAcwNext->AcwId, pAcwNext->State);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
        }

        // get actual ACW_Word1 of this ACW
        ACW_Word_Temp = EDDP_HAL_MEM64_READ (pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->ACW_Word1);
        // update ACW_Word1 with New_ACW_nextoffset
        EDDP_SET_BIT_VALUE64( ACW_Word_Temp, 
                              New_ACW_nextoffset, 
                              PNIP_R2_ACW_SND_WORD1_ACW_NEXTOFFSET_MASK,
                              PNIP_ACW_SND_WORD1_ACW_NEXTOFFSET_SHIFT)
        // and write ACW_Word1 back to APICtrl-RAM
        EDDP_HAL_MEM64_WRITE (pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->ACW_Word1, ACW_Word_Temp);
    }
}
#endif


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvAcwRemoveFromProvider                           +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                        +*/
/*+                             LSA_UINT16                      ProviderID                  +*/
/*+                             LSA_UINT16                      AcwID                       +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ProviderID         : Index in provider array                                           +*/
/*+  AcwID              : Index in ACW array                                                +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Remove ACW from provider - from "AcwProvLink".                            +*/
/*+                                                                                         +*/
/*+               The given provider must have ACW. If no -> FATAL!                         +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwRemoveFromProvider(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID,
    LSA_UINT16                  AcwID)
{
    LSA_UINT16                              Prev;
    LSA_UINT16                              Next;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE              pProvider;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;

    EDDP_ASSERT_FALSE(ProviderID >= pDDB->CRT.CfgParam.ProviderMaxNumber);
    EDDP_ASSERT_FALSE(AcwID >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pProvider       = &pProvMngm->pProvEntry[ProviderID];
    pAcw            = &pProvMngm->pAcwEntry[AcwID];
    Prev            = pAcw->AcwProvLink.Prev;
    Next            = pAcw->AcwProvLink.Next;

    // are there some ACWs in this provider ?
    if (pProvider->AcwCnt == 0)
    {
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwRemoveFromProvider(ProviderID=%d, AcwID=%d): FATAL -> AcwCnt=0!", 
            ProviderID, AcwID);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        /* ------------------------------------------------------------ */
        /* Remove this ACW from Prov-AcwList                            */
        /* ------------------------------------------------------------ */
        if ((Prev == EDDP_CRT_CHAIN_IDX_END) || (Next == EDDP_CRT_CHAIN_IDX_END))
        {
            if (Prev == EDDP_CRT_CHAIN_IDX_END)
            {
                // this ACW is first ACW in Prov-AcwList
                if (Next == EDDP_CRT_CHAIN_IDX_END)
                {
                    // first and only one ACW in Prov-AcwList
                    pProvider->AcwList.BeginIdx = EDDP_CRT_CHAIN_IDX_END;
                    pProvider->AcwList.EndIdx   = EDDP_CRT_CHAIN_IDX_END;
                }
                else
                {
                    // first ACW in Prov-AcwList but not only one
                    pProvider->AcwList.BeginIdx                     = Next;
                    pProvMngm->pAcwEntry[Next].AcwProvLink.Prev  = EDDP_CRT_CHAIN_IDX_END;
                }
            }
            else
            {
                // last ACW in Prov-AcwList but not only one
                pProvMngm->pAcwEntry[Prev].AcwProvLink.Next  = EDDP_CRT_CHAIN_IDX_END;
                pProvider->AcwList.EndIdx                       = Prev;
            }
        }
        else
        {
            // this ACW is in the middle of the Prov-AcwList
            pProvMngm->pAcwEntry[Prev].AcwProvLink.Next  = Next;
            pProvMngm->pAcwEntry[Next].AcwProvLink.Prev  = Prev;
        }

        EDDP_ASSERT_FALSE(pProvider->AcwCnt == 0);
        pProvider->AcwCnt--;

        // is this AUX-ACW ?
        if (AcwID == pProvider->AuxAcwId)
        {
            // this provider has no more the AUX-ACW
            pProvider->AuxAcwId = EDDP_CRT_CHAIN_IDX_END;
        }
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvAcwRemoveFromUsedList                           +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             LSA_UINT16                          AcwID                   +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                                         +*/
/*+  AcwID      : ACW to be removed                                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Remove ACW from UsedList by Provider-Remove                               +*/
/*+                 -> remove this ACW from AcwUsedList and put it to AcwFreeList           +*/
/*+                 -> no check AcwID, it must be valid !                                   +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwRemoveFromUsedList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID)
{
    LSA_UINT16                              Prev;
    LSA_UINT16                              Next;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
//  EDDP_CRT_PROVIDER_DDB_MNGM_PTR_TYPE     pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;

    EDDP_ASSERT_FALSE(AcwID >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pProvMngm    = &pDDB->CRT.Prov;
    pAcw            = &pProvMngm->pAcwEntry[AcwID];
    Prev            = pAcw->UsedFreeLink.Prev;
    Next            = pAcw->UsedFreeLink.Next;

    // are there some ACWs in CRT management ?
    if (pProvMngm->AcwUsedCnt == 0)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwRemoveFromUsedList(AcwID=%d): FATAL -> AcwUsedCnt=0!", 
            AcwID);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        /* ------------------------------------------------------------ */
        /* Remove this ACW from used-chain list                         */
        /* ------------------------------------------------------------ */
        if ((Prev == EDDP_CRT_CHAIN_IDX_END) || (Next == EDDP_CRT_CHAIN_IDX_END))
        {
            if (Prev == EDDP_CRT_CHAIN_IDX_END)
            {
                // this ACW is first ACW in used list
                if (Next == EDDP_CRT_CHAIN_IDX_END)
                {
                    // first and only one ACW in used list
                    pProvMngm->AcwUsedList.BeginIdx  = EDDP_CRT_CHAIN_IDX_END;
                    pProvMngm->AcwUsedList.EndIdx    = EDDP_CRT_CHAIN_IDX_END;
                }
                else
                {
                    // first ACW in used list but not only one
                    pProvMngm->AcwUsedList.BeginIdx              = Next;
                    pProvMngm->pAcwEntry[Next].UsedFreeLink.Prev = EDDP_CRT_CHAIN_IDX_END;
                }
            }
            else
            {
                // last ACW in used list but not only one
                pProvMngm->pAcwEntry[Prev].UsedFreeLink.Next = EDDP_CRT_CHAIN_IDX_END;
                pProvMngm->AcwUsedList.EndIdx                = Prev;
            }
        }
        else
        {
            // this ACW is in the middle of the used list
            pProvMngm->pAcwEntry[Prev].UsedFreeLink.Next = Next;
            pProvMngm->pAcwEntry[Next].UsedFreeLink.Prev = Prev;
        }

        /* ------------------------------------------------------------ */
        /* Put this ACW to begin of free-chain list                     */
        /* ------------------------------------------------------------ */
        if (pProvMngm->AcwFreeList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
        {
            // no ACW in free list
            pAcw->UsedFreeLink.Prev             = EDDP_CRT_CHAIN_IDX_END;
            pAcw->UsedFreeLink.Next             = EDDP_CRT_CHAIN_IDX_END;
            pProvMngm->AcwFreeList.BeginIdx  = AcwID;
            pProvMngm->AcwFreeList.EndIdx    = AcwID;
        }
        else
        {
            // one ore more ACW in free list
            pAcw->UsedFreeLink.Prev                                                         = EDDP_CRT_CHAIN_IDX_END;
            pAcw->UsedFreeLink.Next                                                         = pProvMngm->AcwFreeList.BeginIdx;
            pProvMngm->pAcwEntry[pProvMngm->AcwFreeList.BeginIdx].UsedFreeLink.Prev   = AcwID;
            pProvMngm->AcwFreeList.BeginIdx                                              = AcwID;
        }

        EDDP_SET_BIT_VALUE16( (pAcw->State), 
                              EDDP_CRT_PROV_ACW_STATE_BIT_OFF, 
                              EDDP_CRT_PROV_ACW_STATE_IN_SW_MASK, 
                              EDDP_CRT_PROV_ACW_STATE_IN_SW_SHIFT)

        EDDP_ASSERT_FALSE(pProvMngm->AcwUsedCnt == 0);
        pProvMngm->AcwUsedCnt--;
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProvAcwDeleteHwApi                              +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      AcwID                   +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  AcwID          : Index in ACW array                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function deletes one ACW in APICtrl-RAM.                         +*/
/*+                                                                                     +*/
/*+               Calculate the parameter for function EDDP_HALMemset()                 +*/
/*+               and call this function.                                               +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDeleteHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID)
{
    EDD_RSP                             Response;
    LSA_UINT32                          ApiAdrTemp1;
    LSA_UINT32                          ApiAdrTemp2;
    LSA_UINT32                          IntOffs;
    LSA_UINT16                          HwAcwSize;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE      pAcw;

    EDDP_ASSERT_FALSE(AcwID >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

    Response    = EDD_STS_OK;
    HwAcwSize   = pDDB->CRT.CfgHwAllocParamApi.HWPpmAcwSize;
    pAcw        = &pDDB->CRT.Prov.pAcwEntry[AcwID];

    EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProvAcwDeleteHwApi(AcwID=%d): pHwBaseAcwApi=0x%X, pHwAcwApi=0x%X, Acw-State=0x%X", 
        AcwID, pDDB->CRT.Prov.pHwBaseAcwApi, pAcw->pHwAcwApi, pAcw->State);

    if (pAcw->State & EDDP_CRT_PROV_ACW_STATE_IN_HW_MASK)
    {
        ApiAdrTemp1 = (LSA_UINT32) pDDB->CRT.Prov.pHwBaseAcwApi;
        ApiAdrTemp2 = (LSA_UINT32) pAcw->pHwAcwApi;
        EDDP_ASSERT_FALSE(ApiAdrTemp1 > ApiAdrTemp2);
        IntOffs = ApiAdrTemp2 - ApiAdrTemp1;

        Response = EDDP_HALMemset ( pDDB, 
                                    EDDP_HAL_RAMTYPE_API_CTRL, 
                                    EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_PROV_ACW, 
                                    IntOffs, 
                                    HwAcwSize,
                                    0);

        if (Response != EDD_STS_OK)
        {
            EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwDeleteHwApi(AcwID=%d): FATAL -> Response(0x%X) of EDDP_HALMemset() is not OK!, IntOffs=0x%X, Length=0x%X",
                AcwID, Response, IntOffs, (sizeof(PNIP_ACW_SND_TYPE)));
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        else
        {
            pAcw->pHwAcwApi = LSA_NULL;
            // ACW is no more available in HW
            EDDP_SET_BIT_VALUE16( (pAcw->State), 
                                  EDDP_CRT_PROV_ACW_STATE_BIT_OFF, 
                                  EDDP_CRT_PROV_ACW_STATE_IN_HW_MASK, 
                                  EDDP_CRT_PROV_ACW_STATE_IN_HW_SHIFT)
        }
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProvAcwDGDeleteHwApi                            +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      AcwDGId                 +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  AcwDGId        : Index in ACW_DG array                                             +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function deletes one ACW_DG in APICtrl-RAM.                      +*/
/*+                                                                                     +*/
/*+               Calculate the parameter for function EDDP_HALMemset()                 +*/
/*+               and call this function.                                               +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDGDeleteHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwDGId)
{
    EDD_RSP                             Response;
    LSA_UINT32                          ApiAdrTemp1;
    LSA_UINT32                          ApiAdrTemp2;
    LSA_UINT32                          IntOffs;
    EDDP_CRT_PROVIDER_ACW_DG_PTR_TYPE   pAcwDG;

    Response    = EDD_STS_OK;
    pAcwDG      = &pDDB->CRT.Prov.pAcwDGEntry[AcwDGId];

    EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProvAcwDGDeleteHwApi(AcwDGId=%d): pHwBaseAcwDGApi=0x%X, pHwAcwDGApi=0x%X, AcwDG-State=0x%X", 
        AcwDGId, pDDB->CRT.Prov.pHwBaseAcwDGApi, pAcwDG->pHwAcwDGApi, pAcwDG->State);

    if (pAcwDG->State & EDDP_CRT_PROV_ACW_STATE_IN_HW_MASK)
    {
        ApiAdrTemp1 = (LSA_UINT32) pDDB->CRT.Prov.pHwBaseAcwDGApi;
        ApiAdrTemp2 = (LSA_UINT32) pAcwDG->pHwAcwDGApi;
        EDDP_ASSERT_FALSE(ApiAdrTemp1 > ApiAdrTemp2);
        IntOffs = ApiAdrTemp2 - ApiAdrTemp1;

        Response = EDDP_HALMemset ( pDDB, 
                                    EDDP_HAL_RAMTYPE_API_CTRL, 
                                    EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_PROV_ACW_DG, 
                                    IntOffs, 
                                    (sizeof(PNIP_ACWDG_SND_TYPE)), 
                                    0);

        if (Response != EDD_STS_OK)
        {
            EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwDGDeleteHwApi(AcwDGId=%d): FATAL -> Response(0x%X) of EDDP_HALMemset() is not OK!, IntOffs=0x%X, Length=0x%X",
                AcwDGId, Response, IntOffs, (sizeof(PNIP_ACWDG_SND_TYPE)));
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        else
        {
            pAcwDG->pHwAcwDGApi = LSA_NULL;
            // ACW_DG is no more available in HW
            EDDP_SET_BIT_VALUE16( (pAcwDG->State), 
                                  EDDP_CRT_PROV_ACW_STATE_BIT_OFF, 
                                  EDDP_CRT_PROV_ACW_STATE_IN_HW_MASK, 
                                  EDDP_CRT_PROV_ACW_STATE_IN_HW_SHIFT)
        }
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvAcwROGBitAddHwApi                               +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             LSA_UINT16                          AcwID                   +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                     +*/
/*+  AcwID          : Index in ACW array                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: This function adds the R-O-G-enable Bit to TBaseElement in APICtrl-RAM    +*/
/*+               of the TROG-Entry, if first ACW is added in AcwPhaseList.                 +*/
/*+                                                                                         +*/
/*+               Check put R-O-G-enable Bit is necessary                                   +*/
/*+                 -> this bit is not yet set                                              +*/
/*+                                                                                         +*/
/*+               If no TrogEntry is available on TBase or ACW is not in AcwPhaseList,      +*/
/*+               nothing to do.                                                            +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwROGBitAddHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID)
{
    LSA_UINT16                              TBaseIndex;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;
    EDDP_CRT_PROVIDER_TBASE_PTR_TYPE        pTBase;

    EDDP_ASSERT_FALSE(AcwID >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pAcw            = &pProvMngm->pAcwEntry[AcwID];
    TBaseIndex      = EDDP_CRTGetTBaseIndex ( pDDB, 
                                              pAcw->AddParam.CycleReductionRatio, 
                                              pAcw->AddParam.CyclePhase);
    pTBase          = &pDDB->CRT.Prov.pTBaseEntry[TBaseIndex];

    EDDP_CRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProvAcwROGBitAddHwApi(AcwID=%d): TBaseIndex=%d, pTBase->pTrogEntry=0x%X, ROGPhase=0x%X, Acw-State=0x%X", 
        AcwID, TBaseIndex, pTBase->pTrogEntry, pAcw->ROGPhase, pAcw->State);

    // only if Trog is available on this TBase and this ACW is already added to AcwPhaseList
    if ( !(LSA_HOST_PTR_ARE_EQUAL(pTBase->pTrogEntry, LSA_NULL)) && (pAcw->State & EDDP_CRT_PROV_ACW_STATE_IN_PHASELIST_MASK) )
    {
        // add R-O-G-enable Bit to TBaseElement
        switch (pAcw->ROGPhase)
        {
            case EDDP_CRT_PROV_ACW_PHASE_G:
            {
                if ( !(EDDP_GET_BIT_VALUE( (pTBase->TBaseElement), 
                                            PNIP_ACW_SND_TBASE_GREEN_ENABLE_MASK, 
                                            PNIP_ACW_SND_TBASE_GREEN_ENABLE_SHIFT))
                   )
                {
                    // first ADD in AcwPhaseList_G
                    // set Bit "Green_enable" in TBaseElement
                    EDDP_SET_BIT_VALUE16 ( pTBase->TBaseElement, 
                                           PNIP_ACW_SND_TBASE_BIT_ON,
                                           PNIP_ACW_SND_TBASE_GREEN_ENABLE_MASK,
                                           PNIP_ACW_SND_TBASE_GREEN_ENABLE_SHIFT)

                    // write new TBaseElement to APICtrl-RAM
                    EDDP_HAL_MEM16_WRITE (pDDB, &pTBase->pHwTBaseApi->TBaseElement, pTBase->TBaseElement);

                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProvAcwROGBitAddHwApi(): New-TBaseElement=0x%X", 
                        pTBase->TBaseElement);
                }
            }
            break;

            case EDDP_CRT_PROV_ACW_PHASE_O:
            {
                if ( !(EDDP_GET_BIT_VALUE( (pTBase->TBaseElement), 
                                            PNIP_ACW_SND_TBASE_ORANGE_ENABLE_MASK, 
                                            PNIP_ACW_SND_TBASE_ORANGE_ENABLE_SHIFT))
                   )
                {
                    // first ADD in AcwPhaseList_O
                    // set Bit "Orange_enable" in TBaseElement
                    EDDP_SET_BIT_VALUE16 ( pTBase->TBaseElement, 
                                           PNIP_ACW_SND_TBASE_BIT_ON,
                                           PNIP_ACW_SND_TBASE_ORANGE_ENABLE_MASK,
                                           PNIP_ACW_SND_TBASE_ORANGE_ENABLE_SHIFT)

                    // write new TBaseElement to APICtrl-RAM
                    EDDP_HAL_MEM16_WRITE (pDDB, &pTBase->pHwTBaseApi->TBaseElement, pTBase->TBaseElement);

                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProvAcwROGBitAddHwApi(): New-TBaseElement=0x%X", 
                        pTBase->TBaseElement);
                }
            }
            break;

            case EDDP_CRT_PROV_ACW_PHASE_R:
            {
                if ( !(EDDP_GET_BIT_VALUE( (pTBase->TBaseElement), 
                                            PNIP_ACW_SND_TBASE_RED_ENABLE_MASK, 
                                            PNIP_ACW_SND_TBASE_RED_ENABLE_SHIFT))
                   )
                {
                    // first ADD in AcwPhaseList_R
                    // set Bit "Red_enable" in TBaseElement
                    EDDP_SET_BIT_VALUE16 ( pTBase->TBaseElement, 
                                           PNIP_ACW_SND_TBASE_BIT_ON,
                                           PNIP_ACW_SND_TBASE_RED_ENABLE_MASK,
                                           PNIP_ACW_SND_TBASE_RED_ENABLE_SHIFT)

                    // write new TBaseElement to APICtrl-RAM
                    EDDP_HAL_MEM16_WRITE (pDDB, &pTBase->pHwTBaseApi->TBaseElement, pTBase->TBaseElement);

                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProvAcwROGBitAddHwApi(): New-TBaseElement=0x%X", 
                        pTBase->TBaseElement);
                }
            }
            break;

            default:
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwROGBitAddHwApi(): FATAL -> ROGPhase unknown! ROGPhase=0x%X", 
                    pAcw->ROGPhase);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvAcwROGBitRemoveHwApi                            +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             LSA_UINT16                          AcwID                   +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                     +*/
/*+  AcwID          : Index in ACW array                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: This function removes R-O-G-enable Bit from TBaseElement in APICtrl-RAM   +*/
/*+               of the TROG-Entry, if last ACW is removed from AcwPhaseList.              +*/
/*+                                                                                         +*/
/*+               Check delete R-O-G-enable Bit is necessary                                +*/
/*+                 -> (AcwPhaseActCnt_X == 0)                                              +*/
/*+                 -> call this function only if this ACW is already passivated in HW      +*/
/*+                                                                                         +*/
/*+               If no TrogEntry is available on TBase or ACW is not in AcwPhaseList,      +*/
/*+               nothing to do.                                                            +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwROGBitRemoveHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID)
{
    LSA_UINT16                              TBaseIndex;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;
    EDDP_CRT_PROVIDER_TBASE_PTR_TYPE        pTBase;

    EDDP_ASSERT_FALSE(AcwID >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pAcw            = &pProvMngm->pAcwEntry[AcwID];
    TBaseIndex      = EDDP_CRTGetTBaseIndex ( pDDB, 
                                              pAcw->AddParam.CycleReductionRatio, 
                                              pAcw->AddParam.CyclePhase);
    pTBase          = &pDDB->CRT.Prov.pTBaseEntry[TBaseIndex];

    EDDP_CRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProvAcwROGBitRemoveHwApi(AcwID=%d): TBaseIndex=%d, pTBase->pTrogEntry=0x%X, ROGPhase=0x%X, Acw-State=0x%X", 
        AcwID, TBaseIndex, pTBase->pTrogEntry, pAcw->ROGPhase, pAcw->State);

    // only if Trog is available on this TBase and this ACW is not yet removed from AcwPhaseList
    if ( !(LSA_HOST_PTR_ARE_EQUAL(pTBase->pTrogEntry, LSA_NULL)) && (pAcw->State & EDDP_CRT_PROV_ACW_STATE_IN_PHASELIST_MASK) )
    {
        // remove R-O-G-enable Bit from TBaseElement
        switch (pAcw->ROGPhase)
        {
            case EDDP_CRT_PROV_ACW_PHASE_G:
            {
                if (pTBase->pTrogEntry->TrogAcwList.AcwPhaseActCnt_G == 0)
                {
                    // last REMOVE in AcwPhaseList_G
                    // delete Bit "Green_enable" in TBaseElement
                    EDDP_SET_BIT_VALUE16 ( pTBase->TBaseElement, 
                                           PNIP_ACW_SND_TBASE_BIT_OFF,
                                           PNIP_ACW_SND_TBASE_GREEN_ENABLE_MASK,
                                           PNIP_ACW_SND_TBASE_GREEN_ENABLE_SHIFT)

                    // write new TBaseElement to APICtrl-RAM
                    EDDP_HAL_MEM16_WRITE (pDDB, &pTBase->pHwTBaseApi->TBaseElement, pTBase->TBaseElement);

                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProvAcwROGBitRemoveHwApi(): New-TBaseElement=0x%X", 
                        pTBase->TBaseElement);
                }
            }
            break;

            case EDDP_CRT_PROV_ACW_PHASE_O:
            {
                if (pTBase->pTrogEntry->TrogAcwList.AcwPhaseActCnt_O == 0)
                {
                    // last REMOVE in AcwPhaseList_O
                    // delete Bit "Orange_enable" in TBaseElement
                    EDDP_SET_BIT_VALUE16 ( pTBase->TBaseElement, 
                                           PNIP_ACW_SND_TBASE_BIT_OFF,
                                           PNIP_ACW_SND_TBASE_ORANGE_ENABLE_MASK,
                                           PNIP_ACW_SND_TBASE_ORANGE_ENABLE_SHIFT)

                    // write new TBaseElement to APICtrl-RAM
                    EDDP_HAL_MEM16_WRITE (pDDB, &pTBase->pHwTBaseApi->TBaseElement, pTBase->TBaseElement);

                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProvAcwROGBitRemoveHwApi(): New-TBaseElement=0x%X", 
                        pTBase->TBaseElement);
                }
            }
            break;

            case EDDP_CRT_PROV_ACW_PHASE_R:
            {
                if ((pTBase->pTrogEntry->TrogAcwList.AcwPhaseActCnt_R == 0) && (pTBase->pTrogEntry->TrogAcwList.AcwDfpPhaseCnt_R == 0))
                {
                    // last REMOVE in AcwPhaseList_R
                    // delete Bit "Red_enable" in TBaseElement
                    EDDP_SET_BIT_VALUE16 ( pTBase->TBaseElement, 
                                           PNIP_ACW_SND_TBASE_BIT_OFF,
                                           PNIP_ACW_SND_TBASE_RED_ENABLE_MASK,
                                           PNIP_ACW_SND_TBASE_RED_ENABLE_SHIFT)

                    // write new TBaseElement to APICtrl-RAM
                    EDDP_HAL_MEM16_WRITE (pDDB, &pTBase->pHwTBaseApi->TBaseElement, pTBase->TBaseElement);

                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProvAcwROGBitRemoveHwApi(): New-TBaseElement=0x%X", 
                        pTBase->TBaseElement);
                }
            }
            break;

            default:
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwROGBitRemoveHwApi(): FATAL -> ROGPhase unknown! ROGPhase=0x%X", 
                    pAcw->ROGPhase);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }
}





#if defined (EDDP_CFG_HW_ERTEC200P_SUPPORT)
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProvAcwWriteSBOffset                            +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      ProviderID              +*/
/*+                             LSA_UINT16                      ACW_SB_offset           +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ProviderID     : Index in provider array                                           +*/
/*+  ACW_SB_offset  : TimerSB address (offset) in APICtrl-RAM of consumer               +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function writes the "Scoreboard_offset" to all ACWs              +*/
/*+               of this provider.                                                     +*/
/*+               If this provider has AUX-ACW, the AUX-ACW is ignored.                 +*/
/*+               Provider must be used in CRT. If no -> FATAL!                         +*/
/*+                                                                                     +*/
/*+               If this provider has no ACW in APICtrl-RAM nothing to do.             +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwWriteSBOffset(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID,
    LSA_UINT16                  ACW_SB_offset)
{
    LSA_UINT16                              AcwIdx;
    LSA_UINT64                              ACW_Word_Temp;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE              pProvider;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;
    EDDP_CRT_PROVIDER_ACW_DG_PTR_TYPE       pAcwDG;

    pProvMngm    = &pDDB->CRT.Prov;
    pProvider       = &pProvMngm->pProvEntry[ProviderID];
    AcwIdx          = pProvider->AcwList.BeginIdx;

    EDDP_CRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProvAcwWriteSBOffset(ProviderID=%d, ACW_SB_offset=0x%X): ProviderFlags=0x%X, AcwCnt=%d, AuxAcwId=%d", 
        ProviderID, ACW_SB_offset, pProvider->ProviderFlags, pProvider->AcwCnt, pProvider->AuxAcwId);

    if (pProvider->State == EDDP_CRT_STATE_FREE)
    {
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwWriteSBOffset(ProviderID=%d, ACW_SB_offset=0x%X): FATAL -> Provider is not used!", 
            ProviderID, ACW_SB_offset);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        // loop through all ACWs in this provider
        while (AcwIdx != EDDP_CRT_CHAIN_IDX_END)
        {
            EDDP_ASSERT_FALSE(AcwIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

            pAcw = &pProvMngm->pAcwEntry[AcwIdx];

            if (pProvider->ProviderFlags & EDDP_CRT_PROV_FLAGS_IS_DFP)
            {
                // write the "Scoreboard_offset" only to ACW_DG of this provider
                EDDP_ASSERT_FALSE(pProvider->DfpParam.AcwDGId == EDDP_CRT_CHAIN_IDX_END);
                pAcwDG = &pProvMngm->pAcwDGEntry[pProvider->DfpParam.AcwDGId];

                // only if ACW_DG is available in HW
                if (pAcwDG->State & EDDP_CRT_PROV_ACW_STATE_IN_HW_MASK)
                {
                    // get actual ACW_Word2 of this ACW_DG
                    ACW_Word_Temp = EDDP_HAL_MEM64_READ (pDDB, &pAcwDG->pHwAcwDGApi->ACW_Word2);
                    // update ACW_Word2 with new Scoreboard_offset
                    EDDP_SET_BIT_VALUE64( ACW_Word_Temp, 
                                          ACW_SB_offset, 
                                          PNIP_R2_ACWDG_SND_WORD2_SCOREBOARD_OFFSET_MASK,
                                          PNIP_ACWDG_SND_WORD2_SCOREBOARD_OFFSET_SHIFT)
                    // and write ACW_Word2 back to APICtrl-RAM
                    EDDP_HAL_MEM64_WRITE (pDDB, &pAcwDG->pHwAcwDGApi->ACW_Word2, ACW_Word_Temp);
                }
            }
            else
            {
                // only if ACW available in APICtrl-RAM and not for AUX-ACW
                if ((pAcw->State & EDDP_CRT_PROV_ACW_STATE_IN_HW_MASK) && (pAcw->AcwId != pProvider->AuxAcwId))
                {
                    // get actual ACW_Word3 of this ACW
                    ACW_Word_Temp = EDDP_HAL_MEM64_READ (pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->ACW_Word3);
                    // update ACW_Word3 with new Scoreboard_offset
                    EDDP_SET_BIT_VALUE64( ACW_Word_Temp, 
                                          ACW_SB_offset, 
                                          PNIP_R2_ACW_SND_WORD3_SCOREBOARD_OFFSET_MASK,
                                          PNIP_ACW_SND_WORD3_SCOREBOARD_OFFSET_SHIFT)
                    // and write ACW_Word3 back to APICtrl-RAM
                    EDDP_HAL_MEM64_WRITE (pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->ACW_Word3, ACW_Word_Temp);
                }
            }

            // next AcwIdx in this provider
            AcwIdx = pProvMngm->pAcwEntry[AcwIdx].AcwProvLink.Next;
        }
    }
}
#endif

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProvAcwSetActiveRTFrame                         +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      ProviderID              +*/
/*+                             LSA_UINT16                      AcwID                   +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ProviderID     : Index in provider array                                           +*/
/*+  AcwID          : Index in ACW array                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function sets the ACW (for RT-Frame) as ACTIVE.                  +*/
/*+                 - Increase the AcwPhaseActCnt_X if this ACW is activated in HW.     +*/
/*+                 - Increase the AcwActCnt of this provider.                          +*/
/*+                 - Set bit EDDP_CRT_PROV_ACW_STATE_BIT_ON in ACW state.              +*/
/*+                 - Increase AcwActCnt                                             +*/
/*+                 - Increase QCWUsedCnt                                               +*/
/*+                                                                                     +*/
/*+               Call this function only after ACW_on command.                         +*/
/*+                                                                                     +*/
/*+               If no TrogEntry is available on TBase or ACW is already active,       +*/
/*+               nothing to do.                                                        +*/
/*+                                                                                     +*/
/*+               Not allowed for DFP-Provider -> FATAL!                                +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwSetActiveRTFrame(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID,
    LSA_UINT16                  AcwID)
{
    LSA_UINT16                              TBaseIndex;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
//  EDDP_CRT_PROVIDER_DDB_MNGM_PTR_TYPE     pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE              pProvider;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;
    EDDP_CRT_PROVIDER_TBASE_PTR_TYPE        pTBase;

    EDDP_ASSERT_FALSE(ProviderID >= pDDB->CRT.CfgParam.ProviderMaxNumber);
    EDDP_ASSERT_FALSE(AcwID >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pProvMngm    = &pDDB->CRT.Prov;
    pProvider       = &pProvMngm->pProvEntry[ProviderID];
    pAcw            = &pProvMngm->pAcwEntry[AcwID];
    TBaseIndex      = EDDP_CRTGetTBaseIndex ( pDDB, 
                                              pAcw->AddParam.CycleReductionRatio, 
                                              pAcw->AddParam.CyclePhase);
    pTBase          = &pProvMngm->pTBaseEntry[TBaseIndex];

    if (pProvider->ProviderFlags & EDDP_CRT_PROV_FLAGS_IS_DFP)
    {
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwSetActiveRTFrame(ProviderID=%d, AcwID=%d): FATAL -> not allowed for DFP-Provider!", 
            ProviderID, AcwID);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    // only if Trog is available on this TBase and this ACW is not yet active
    if ( !(LSA_HOST_PTR_ARE_EQUAL(pTBase->pTrogEntry, LSA_NULL)) && (!(pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK)) )
    {
        switch (pAcw->ROGPhase)
        {
            case EDDP_CRT_PROV_ACW_PHASE_G:
            {
                pTBase->pTrogEntry->TrogAcwList.AcwPhaseActCnt_G++;
                pProvMngm->AcwActCntProvCntrlRTC12++;
            }
            break;

            case EDDP_CRT_PROV_ACW_PHASE_O:
            {
                pTBase->pTrogEntry->TrogAcwList.AcwPhaseActCnt_O++;
                pProvMngm->AcwActCntProvCntrlRTC12++;
            }
            break;

            case EDDP_CRT_PROV_ACW_PHASE_R:
            {
                pTBase->pTrogEntry->TrogAcwList.AcwPhaseActCnt_R++;
                pProvMngm->AcwActCntProvCntrlRTC3++;
            }
            break;

            default:
            {
                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwSetActiveRTFrame(AcwID=%d): FATAL -> ROGPhase unknown! ROGPhase=0x%X", 
                    AcwID, pAcw->ROGPhase);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }

        EDDP_SET_BIT_VALUE16( (pAcw->State), 
                              EDDP_CRT_PROV_ACW_STATE_BIT_ON, 
                              EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK, 
                              EDDP_CRT_PROV_ACW_STATE_ACTIVE_SHIFT)

        pProvider->AcwActCnt++;
        pProvMngm->AcwActCnt++;
        pProvMngm->AcwActCntProvCntrl++;
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProvAcwSetPassiveRTFrame                        +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      ProviderID              +*/
/*+                             LSA_UINT16                      AcwID                   +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ProviderID     : Index in provider array                                           +*/
/*+  AcwID          : Index in ACW array                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function sets the ACW (for RT-Frame) as PASSIVE.                 +*/
/*+                 - Decrement the AcwPhaseActCnt_X if this ACW is activated in HW.    +*/
/*+                 - Decrement the AcwActCnt of this provider.                         +*/
/*+                 - Delete ACW data which must be found out newly at the next ACTIVATE.+*/
/*+                 - Delete bit EDDP_CRT_PROV_ACW_STATE_BIT_ON in ACW state.           +*/
/*+                 - Decrement AcwActCnt                                            +*/
/*+                 - Decrement QCWUsedCnt                                              +*/
/*+                                                                                     +*/
/*+               Call this function only after ACW_off command.                        +*/
/*+                                                                                     +*/
/*+               If no TrogEntry is available on TBase or ACW is already passive,      +*/
/*+               nothing to do.                                                        +*/
/*+                                                                                     +*/
/*+               Not allowed for DFP-Provider -> FATAL!                                +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwSetPassiveRTFrame(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID,
    LSA_UINT16                  AcwID)
{
    LSA_UINT16                              TBaseIndex;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
//  EDDP_CRT_PROVIDER_DDB_MNGM_PTR_TYPE     pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE              pProvider;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;
    EDDP_CRT_PROVIDER_TBASE_PTR_TYPE        pTBase;

    EDDP_ASSERT_FALSE(ProviderID >= pDDB->CRT.CfgParam.ProviderMaxNumber);
    EDDP_ASSERT_FALSE(AcwID >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pProvMngm    = &pDDB->CRT.Prov;
    pProvider       = &pProvMngm->pProvEntry[ProviderID];
    pAcw            = &pProvMngm->pAcwEntry[AcwID];
    TBaseIndex      = EDDP_CRTGetTBaseIndex ( pDDB, 
                                              pAcw->AddParam.CycleReductionRatio, 
                                              pAcw->AddParam.CyclePhase);
    pTBase          = &pProvMngm->pTBaseEntry[TBaseIndex];

    if (pProvider->ProviderFlags & EDDP_CRT_PROV_FLAGS_IS_DFP)
    {
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwSetPassiveRTFrame(ProviderID=%d, AcwID=%d): FATAL -> not allowed for DFP-Provider!", 
            ProviderID, AcwID);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    // only if Trog is available on this TBase and this ACW is not yet passive
    if ( !(LSA_HOST_PTR_ARE_EQUAL(pTBase->pTrogEntry, LSA_NULL)) && (pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK) )
    {
        switch (pAcw->ROGPhase)
        {
            case EDDP_CRT_PROV_ACW_PHASE_G:
            {
                EDDP_ASSERT_FALSE(pTBase->pTrogEntry->TrogAcwList.AcwPhaseActCnt_G == 0);
                pTBase->pTrogEntry->TrogAcwList.AcwPhaseActCnt_G--;

                EDDP_ASSERT_FALSE(pProvMngm->AcwActCntProvCntrlRTC12 == 0);
                pProvMngm->AcwActCntProvCntrlRTC12--;
            }
            break;

            case EDDP_CRT_PROV_ACW_PHASE_O:
            {
                EDDP_ASSERT_FALSE(pTBase->pTrogEntry->TrogAcwList.AcwPhaseActCnt_O == 0);
                pTBase->pTrogEntry->TrogAcwList.AcwPhaseActCnt_O--;

                EDDP_ASSERT_FALSE(pProvMngm->AcwActCntProvCntrlRTC12 == 0);
                pProvMngm->AcwActCntProvCntrlRTC12--;
            }
            break;

            case EDDP_CRT_PROV_ACW_PHASE_R:
            {
                EDDP_ASSERT_FALSE(pTBase->pTrogEntry->TrogAcwList.AcwPhaseActCnt_R == 0);
                pTBase->pTrogEntry->TrogAcwList.AcwPhaseActCnt_R--;

                EDDP_ASSERT_FALSE(pProvMngm->AcwActCntProvCntrlRTC3 == 0);
                pProvMngm->AcwActCntProvCntrlRTC3--;

                // delete PortInfo in ACW - this cannot be used any more at the next ACTIVATE
                pAcw->AddParam.FrameSendOffset = EDDP_CRT_FRAME_SEND_OFFSET_UNDEFINED;
            }
            break;

            default:
            {
                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwSetPassiveRTFrame(AcwID=%d): FATAL -> ROGPhase unknown! ROGPhase=0x%X", 
                    AcwID, pAcw->ROGPhase);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }

        EDDP_SET_BIT_VALUE16( (pAcw->State), 
                              EDDP_CRT_PROV_ACW_STATE_BIT_OFF, 
                              EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK, 
                              EDDP_CRT_PROV_ACW_STATE_ACTIVE_SHIFT)

        EDDP_ASSERT_FALSE(pProvider->AcwActCnt == 0);
        pProvider->AcwActCnt--;

        EDDP_ASSERT_FALSE(pProvMngm->AcwActCnt == 0);
        pProvMngm->AcwActCnt--;

        EDDP_ASSERT_FALSE(pProvMngm->AcwActCntProvCntrl == 0);
        pProvMngm->AcwActCntProvCntrl--;
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvAcwChainSendOrderToProvider                     +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                        +*/
/*+                             LSA_UINT16                      ProviderID                  +*/
/*+                             LSA_UINT16                      AcwID                       +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ProviderID         : Index in provider array                                           +*/
/*+  AcwID              : ACW which must be chained                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: This function chains the given ACW on its sending order                   +*/
/*+               within its provider.                                                      +*/
/*+               This function is necessary for ACW_in/out command, because                +*/
/*+               the sending order is important for ACW_in and ACW_out!                    +*/
/*+                                                                                         +*/
/*+               These cases are not allowed for this function:                            +*/
/*+                 - given ACW is not RED ACW (because sorting is on its "FrameSendOffset")+*/
/*+                                                                                         +*/
/*+               Nothing to do if:                                                         +*/
/*+                 - given ACW is not found in given provider                              +*/
/*+                 - given provider has AUX-ACW                                            +*/
/*+                 - given provider has only one ACW                                       +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwChainSendOrderToProvider(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID,
    LSA_UINT16                  AcwID)
{
    LSA_UINT16                              AcwIdx;
    LSA_UINT16                              ProvAcwUsedIdx;
    LSA_UINT16                              ProvAcwUsedIdxPrev;     // for add or remove between Prev and Next in ProvAcwList
    LSA_UINT16                              ProvAcwUsedIdxNext;
    LSA_UINT32                              NewFrameSendOffset;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE              pProvider;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;

    EDDP_ASSERT_FALSE(ProviderID >= pDDB->CRT.CfgParam.ProviderMaxNumber);
    EDDP_ASSERT_FALSE(AcwID >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pProvider       = &pProvMngm->pProvEntry[ProviderID];
    pAcw            = &pProvMngm->pAcwEntry[AcwID];

    if (pAcw->ROGPhase != EDDP_CRT_PROV_ACW_PHASE_R)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwChainSendOrderToProvider(): FATAL -> given ACW is not red! ROGPhase=0x%X", 
            pAcw->ROGPhase);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        // only if this provider has more than one ACW and this provider has no AUX-ACW
        if ((pProvider->AcwCnt > 1) && (pProvider->AuxAcwId == EDDP_CRT_CHAIN_IDX_END))
        {
            /* -------------------------------------------- */
            /* search the given ACW in its provider         */
            /* -------------------------------------------- */
            // first AcwIdx in this provider
            AcwIdx = pProvider->AcwList.BeginIdx;
            // loop through all ACWs in this provider from BeginIdx to given AcwID
            while (AcwIdx != EDDP_CRT_CHAIN_IDX_END)
	        {
                EDDP_ASSERT_FALSE(AcwIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

                if (AcwIdx == AcwID)
                {
                    // given ACW in its provider is found
                    // first remove this ACW from provider (it will be chained new)
                    EDDP_CRTProvAcwRemoveFromProvider (pDDB, ProviderID, AcwID);

                    // chain this ACW to provider with new FrameSendOffset
                    NewFrameSendOffset = pAcw->AddParam.FrameSendOffset;

                    if (pProvider->AcwList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
                    {
                        // this ACW is the first ACW in ProvAcwList
                        pProvider->AcwList.BeginIdx = AcwID;
                        pProvider->AcwList.EndIdx   = AcwID;
                        pAcw->AcwProvLink.Prev      = EDDP_CRT_CHAIN_IDX_END;
                        pAcw->AcwProvLink.Next      = EDDP_CRT_CHAIN_IDX_END;
                    }
                    else
                    {
                        // this ACW is not the first ACW in the ProvAcwList
                        if (NewFrameSendOffset >= pProvMngm->pAcwEntry[pProvider->AcwList.EndIdx].AddParam.FrameSendOffset)
                        {
                            // chain this ACW to end in ProvAcwList
                            pAcw->AcwProvLink.Prev                                              = pProvider->AcwList.EndIdx;
                            pAcw->AcwProvLink.Next                                              = EDDP_CRT_CHAIN_IDX_END;
                            pProvMngm->pAcwEntry[pProvider->AcwList.EndIdx].AcwProvLink.Next = AcwID;
                            pProvider->AcwList.EndIdx                                           = AcwID;
                        }
                        else
                        {
                            // chain to middle in ProvAcwList
                            ProvAcwUsedIdx = pProvider->AcwList.BeginIdx;

                            // go through ProvAcwList from BeginIdx to EndIdx
                            while (   (NewFrameSendOffset >= pProvMngm->pAcwEntry[ProvAcwUsedIdx].AddParam.FrameSendOffset)
                                   && (ProvAcwUsedIdx != pProvider->AcwList.EndIdx)
                                  )
                            {
                                EDDP_ASSERT_FALSE(ProvAcwUsedIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);
                                // compare next ProvAcwUsedIdx
                                ProvAcwUsedIdx = pProvMngm->pAcwEntry[ProvAcwUsedIdx].AcwPhaseLink.Next;
                            }

                            // init ProvAcwUsedIdxPrev and ProvAcwUsedIdxNext
                            if ((ProvAcwUsedIdx == pProvider->AcwList.BeginIdx) || (ProvAcwUsedIdx == pProvider->AcwList.EndIdx))
                            {
                                if (NewFrameSendOffset < pProvMngm->pAcwEntry[ProvAcwUsedIdx].AddParam.FrameSendOffset)
                                {
                                    // chain before ProvAcwUsedIdx
                                    ProvAcwUsedIdxPrev = pProvMngm->pAcwEntry[ProvAcwUsedIdx].AcwProvLink.Prev;
                                    ProvAcwUsedIdxNext = ProvAcwUsedIdx;
                                }
                                else
                                {
                                    // chain behind ProvAcwUsedIdx
                                    ProvAcwUsedIdxPrev = ProvAcwUsedIdx;
                                    ProvAcwUsedIdxNext = pProvMngm->pAcwEntry[ProvAcwUsedIdx].AcwProvLink.Next;
                                }
                            }
                            else
                            {
                                if (NewFrameSendOffset == pProvMngm->pAcwEntry[ProvAcwUsedIdx].AddParam.FrameSendOffset)
                                {
                                    // chain behind ProvAcwUsedIdx
                                    ProvAcwUsedIdxPrev = ProvAcwUsedIdx;
                                    ProvAcwUsedIdxNext = pProvMngm->pAcwEntry[ProvAcwUsedIdx].AcwProvLink.Next;
                                }
                                else
                                {
                                    // chain before ProvAcwUsedIdx
                                    ProvAcwUsedIdxPrev = pProvMngm->pAcwEntry[ProvAcwUsedIdx].AcwProvLink.Prev;
                                    ProvAcwUsedIdxNext = ProvAcwUsedIdx;
                                }
                            }

                            // chain between ProvAcwUsedIdxPrev and ProvAcwUsedIdxNext
                            if ((ProvAcwUsedIdxPrev == EDDP_CRT_CHAIN_IDX_END) || (ProvAcwUsedIdxNext == EDDP_CRT_CHAIN_IDX_END))
                            {
                                // chain to begin or end in ProvAcwList
                                if (ProvAcwUsedIdxPrev == EDDP_CRT_CHAIN_IDX_END)
                                {
                                    if (ProvAcwUsedIdxNext == EDDP_CRT_CHAIN_IDX_END)
                                    {
                                        // this case is for the first ACW in the ProvAcwList - it was added above
                                    }
                                    else
                                    {
                                        // chain to begin in ProvAcwList
                                        pAcw->AcwProvLink.Prev                                                  = EDDP_CRT_CHAIN_IDX_END;
                                        pAcw->AcwProvLink.Next                                                  = pProvider->AcwList.BeginIdx;
                                        pProvMngm->pAcwEntry[pProvider->AcwList.BeginIdx].AcwProvLink.Prev   = AcwID;
                                        pProvider->AcwList.BeginIdx                                             = AcwID;
                                    }
                                }
                                else
                                {
                                    // chain to end in ProvAcwList
                                    pAcw->AcwProvLink.Prev                                              = pProvider->AcwList.EndIdx;
                                    pAcw->AcwProvLink.Next                                              = EDDP_CRT_CHAIN_IDX_END;
                                    pProvMngm->pAcwEntry[pProvider->AcwList.EndIdx].AcwProvLink.Next = AcwID;
                                    pProvider->AcwList.EndIdx                                           = AcwID;
                                }
                            }
                            else
                            {
                                // chain to middle in ProvAcwList
                                pAcw->AcwProvLink.Prev                                          = ProvAcwUsedIdxPrev;
                                pAcw->AcwProvLink.Next                                          = ProvAcwUsedIdxNext;
                                pProvMngm->pAcwEntry[ProvAcwUsedIdxPrev].AcwProvLink.Next    = AcwID;
                                pProvMngm->pAcwEntry[ProvAcwUsedIdxNext].AcwProvLink.Prev    = AcwID;
                            }
                        }
                    }

                    pProvider->AcwCnt++;
                    // exit while loop
                    AcwIdx = EDDP_CRT_CHAIN_IDX_END;
                }
                else
                {
                    // next AcwIdx in this provider
                    AcwIdx = pProvMngm->pAcwEntry[AcwIdx].AcwProvLink.Next;
                }
            }
        }
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvAcwSetDstMACFromPDIRData                        +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                            +*/
/*+                             LSA_UINT16                  AcwID                           +*/
/*+                             LSA_UINT16                  FastForwardingMulticastMACAdd   +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB                           : Pointer to DeviceDescriptionBlock                     +*/
/*+  AcwID                          : AcwID                                                 +*/
/*+  FastForwardingMulticastMACAdd  : PDIRData.FrameDataProperties.FastForwardingMulticastMACAdd+*/
/*+                                     - EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_LEGACY+*/
/*+                                     - EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_RTC3+*/
/*+                                     - EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_FFW+*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: This function sets the DstMAC address to ACW dependent of                 +*/
/*+               PDIRData.FrameDataProperties.FastForwardingMulticastMACAdd.               +*/
/*+                                                                                         +*/
/*+               These cases are not allowed for this function:                            +*/
/*+                 - given ACW is not red                                                  +*/
/*+               -> FATAL!                                                                 +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwSetDstMACFromPDIRData(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID,
    LSA_UINT16                  FastForwardingMulticastMACAdd)
{
    LSA_UINT32                              i;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;

    EDDP_ASSERT_FALSE(AcwID >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pAcw            = &pProvMngm->pAcwEntry[AcwID];

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProvAcwSetDstMACFromPDIRData(AcwID=%d, FastForwardingMulticastMACAdd=0x%X)", 
        AcwID, FastForwardingMulticastMACAdd);

    if (pAcw->ROGPhase != EDDP_CRT_PROV_ACW_PHASE_R)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwSetDstMACFromPDIRData(): FATAL -> ROGPhase(0x%X) of this ACW is not red!", 
            pAcw->ROGPhase);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        // now set DstMAC address to this ACW
        switch (FastForwardingMulticastMACAdd)
        {
            case EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_LEGACY:
            {
                // nothing to do: use DstMAC from PROVIDER_ADD / CONTROL
            }
            break;

            case EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_RTC3:
            {
                // use RTC3-MultiCast-DstMAC from MC-Table
				EDD_MAC_ADR_TYPE    DstMacAdr = { EDDP_MC_MAC_ADR_RTC3 };

                for ( i=0; i < EDD_MAC_ADDR_SIZE; i++ )
                {
                    pAcw->AddParam.DstMAC.MacAdr[i] = DstMacAdr.MacAdr[i];
                }
            }
            break;

            case EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_FFW:
            {
                #ifdef EDDP_SWI_SUPPORT_FFW_ON
                // use FastForwarding-MultiCast-DstMAC
				EDD_MAC_ADR_TYPE    DstMacAdr = { EDDP_MC_MAC_ADR_FFW };
                LSA_UINT8           MacAdrByte;
                LSA_UINT16          MaskFrameID = EDDP_CRT_INVALID_VALUE_16;

                for ( i=0; i < EDD_MAC_ADDR_SIZE; i++ )
                {
                    pAcw->AddParam.DstMAC.MacAdr[i] = DstMacAdr.MacAdr[i];
                }

                // MacAdr[0]    : FrameID(11:8) + 0011
                MacAdrByte = (LSA_UINT8) EDDP_GET_BIT_VALUE(pAcw->AddParam.FrameID, MaskFrameID, 8);
                EDDP_SHIFT_VALUE_LEFT(MacAdrByte, 4);
                pAcw->AddParam.DstMAC.MacAdr[0] |= MacAdrByte;

                // MacAdr[1]    : FrameID(7:0)
                MacAdrByte = (LSA_UINT8) EDDP_GET_BIT_VALUE(pAcw->AddParam.FrameID, MaskFrameID, 0);
                pAcw->AddParam.DstMAC.MacAdr[1] = MacAdrByte;
                #else
                // use RTC3-MultiCast-DstMAC from MC-Table
                EDD_MAC_ADR_TYPE    DstMacAdr = EDDP_MC_MAC_ADR_RTC3;

                for ( i=0; i < EDD_MAC_ADDR_SIZE; i++ )
                {
                    pAcw->AddParam.DstMAC.MacAdr[i] = DstMacAdr.MacAdr[i];
                }

                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H:--] EDDP_CRTProvAcwSetDstMACFromPDIRData(): FastForwarding is not supported in PNIP - use RTC3 MacAdr!");
                #endif
            }
            break;

            default:
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwSetDstMACFromPDIRData(): FATAL -> The value of FastForwardingMulticastMACAdd(0x%X) is unknown!", 
                    FastForwardingMulticastMACAdd);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvAcwDGAddToUsedList                              +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
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
/*+  Description: Add new ACW_DG to UsedList by Provider-Add                                +*/
/*+                 -> get first free index from AcwDGFreeList and put it to AcwDGUsedList  +*/
/*+                 -> the NewAcwDGID will be returned in return value                      +*/
/*+                                                                                         +*/
/*+                 -> if (NewAcwDGID == EDDP_CRT_CHAIN_IDX_END)                            +*/
/*+                     - no more free ACW_DG                                               +*/
/*+                     - set error by caller of this function                              +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDGAddToUsedList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT16                              NewAcwDGID;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
//  EDDP_CRT_PROVIDER_DDB_MNGM_PTR_TYPE     pProvMngm;
    EDDP_CRT_PROVIDER_ACW_DG_PTR_TYPE       pAcwDG;

    pProvMngm    = &pDDB->CRT.Prov;
    pProvMngm    = &pDDB->CRT.Prov;
    NewAcwDGID   = pProvMngm->AcwDGFreeList.BeginIdx;   // first index in ACW_DG free list

    if (NewAcwDGID == EDDP_CRT_CHAIN_IDX_END)
    {
        // no more free ACW_DG
        // set error by caller of this function
    }
    else
    {
        pAcwDG = &pProvMngm->pAcwDGEntry[NewAcwDGID];

        /* ------------------------------------------------------------ */
	    /* Remove this new ACW_DG from free-chain list                  */
	    /* ------------------------------------------------------------ */
        if (pAcwDG->UsedFreeLink.Next == EDDP_CRT_CHAIN_IDX_END)
        {
            // this ACW_DG is the last free ACW_DG
            pProvMngm->AcwDGFreeList.BeginIdx    = EDDP_CRT_CHAIN_IDX_END;
            pProvMngm->AcwDGFreeList.EndIdx      = EDDP_CRT_CHAIN_IDX_END;
        }
        else
        {
            // this ACW_DG is not the last free ACW_DG
            pProvMngm->AcwDGFreeList.BeginIdx                                    = pAcwDG->UsedFreeLink.Next;
            pProvMngm->pAcwDGEntry[pAcwDG->UsedFreeLink.Next].UsedFreeLink.Prev  = EDDP_CRT_CHAIN_IDX_END;
        }

        /* ------------------------------------------------------------ */
        /* Put this new ACW_DG to end used-chain list                   */
        /* ------------------------------------------------------------ */
        if (pProvMngm->AcwDGUsedList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
        {
            // this ACW_DG is the first ACW_DG in the used list
            pProvMngm->AcwDGUsedList.BeginIdx    = NewAcwDGID;
            pProvMngm->AcwDGUsedList.EndIdx      = NewAcwDGID;
            pAcwDG->UsedFreeLink.Prev            = EDDP_CRT_CHAIN_IDX_END;
            pAcwDG->UsedFreeLink.Next            = EDDP_CRT_CHAIN_IDX_END;
        }
        else
        {
            // this ACW_DG is not the first ACW_DG in the used list
            pAcwDG->UsedFreeLink.Prev                                                 = pProvMngm->AcwDGUsedList.EndIdx;
            pAcwDG->UsedFreeLink.Next                                                 = EDDP_CRT_CHAIN_IDX_END;
            pProvMngm->pAcwDGEntry[pProvMngm->AcwDGUsedList.EndIdx].UsedFreeLink.Next = NewAcwDGID;
            pProvMngm->AcwDGUsedList.EndIdx                                           = NewAcwDGID;
        }

        EDDP_SET_BIT_VALUE16( (pAcwDG->State), 
                              EDDP_CRT_PROV_ACW_STATE_BIT_ON, 
                              EDDP_CRT_PROV_ACW_STATE_IN_SW_MASK, 
                              EDDP_CRT_PROV_ACW_STATE_IN_SW_SHIFT)

        pProvMngm->AcwDGUsedCnt++;
    }

    return (NewAcwDGID);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvAcwDGRemoveFromUsedList                         +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             LSA_UINT16                          AcwDGID                 +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                                         +*/
/*+  AcwDGID    : ACW_DG to be removed                                                      +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Remove ACW_DG from UsedList by Provider-Remove                            +*/
/*+                 -> remove this ACW_DG from AcwDGUsedList and put it to AcwDGFreeList    +*/
/*+                 -> no check AcwDGID, it must be valid !                                 +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDGRemoveFromUsedList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwDGID)
{
    LSA_UINT16                              Prev;
    LSA_UINT16                              Next;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
//  EDDP_CRT_PROVIDER_DDB_MNGM_PTR_TYPE     pProvMngm;
    EDDP_CRT_PROVIDER_ACW_DG_PTR_TYPE       pAcwDG;

    pProvMngm    = &pDDB->CRT.Prov;
    pProvMngm    = &pDDB->CRT.Prov;
    pAcwDG          = &pProvMngm->pAcwDGEntry[AcwDGID];
    Prev            = pAcwDG->UsedFreeLink.Prev;
    Next            = pAcwDG->UsedFreeLink.Next;

    // are there some ACW_DGs in CRT management ?
    if (pProvMngm->AcwDGUsedCnt == 0)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwDGRemoveFromUsedList(AcwDGID=%d): FATAL -> AcwDGUsedCnt=0!", 
            AcwDGID);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        /* ------------------------------------------------------------ */
        /* Remove this ACW_DG from used-chain list                      */
        /* ------------------------------------------------------------ */
        if ((Prev == EDDP_CRT_CHAIN_IDX_END) || (Next == EDDP_CRT_CHAIN_IDX_END))
        {
            if (Prev == EDDP_CRT_CHAIN_IDX_END)
            {
                // this ACW_DG is first ACW_DG in used list
                if (Next == EDDP_CRT_CHAIN_IDX_END)
                {
                    // first and only one ACW_DG in used list
                    pProvMngm->AcwDGUsedList.BeginIdx    = EDDP_CRT_CHAIN_IDX_END;
                    pProvMngm->AcwDGUsedList.EndIdx      = EDDP_CRT_CHAIN_IDX_END;
                }
                else
                {
                    // first ACW_DG in used list but not only one
                    pProvMngm->AcwDGUsedList.BeginIdx                = Next;
                    pProvMngm->pAcwDGEntry[Next].UsedFreeLink.Prev   = EDDP_CRT_CHAIN_IDX_END;
                }
            }
            else
            {
                // last ACW_DG in used list but not only one
                pProvMngm->pAcwDGEntry[Prev].UsedFreeLink.Next   = EDDP_CRT_CHAIN_IDX_END;
                pProvMngm->AcwDGUsedList.EndIdx                  = Prev;
            }
        }
        else
        {
            // this ACW_DG is in the middle of the used list
            pProvMngm->pAcwDGEntry[Prev].UsedFreeLink.Next   = Next;
            pProvMngm->pAcwDGEntry[Next].UsedFreeLink.Prev   = Prev;
        }

        /* ------------------------------------------------------------ */
        /* Put this ACW_DG to begin of free-chain list                  */
        /* ------------------------------------------------------------ */
        if (pProvMngm->AcwDGFreeList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
        {
            // no ACW_DG in free list
            pAcwDG->UsedFreeLink.Prev               = EDDP_CRT_CHAIN_IDX_END;
            pAcwDG->UsedFreeLink.Next               = EDDP_CRT_CHAIN_IDX_END;
            pProvMngm->AcwDGFreeList.BeginIdx    = AcwDGID;
            pProvMngm->AcwDGFreeList.EndIdx      = AcwDGID;
        }
        else
        {
            // one ore more ACW_DG in free list
            pAcwDG->UsedFreeLink.Prev                                                           = EDDP_CRT_CHAIN_IDX_END;
            pAcwDG->UsedFreeLink.Next                                                           = pProvMngm->AcwDGFreeList.BeginIdx;
            pProvMngm->pAcwDGEntry[pProvMngm->AcwDGFreeList.BeginIdx].UsedFreeLink.Prev   = AcwDGID;
            pProvMngm->AcwDGFreeList.BeginIdx                                                = AcwDGID;
        }

        EDDP_SET_BIT_VALUE16( (pAcwDG->State), 
                              EDDP_CRT_PROV_ACW_STATE_BIT_OFF, 
                              EDDP_CRT_PROV_ACW_STATE_IN_SW_MASK, 
                              EDDP_CRT_PROV_ACW_STATE_IN_SW_SHIFT)

        EDDP_ASSERT_FALSE(pProvMngm->AcwDGUsedCnt == 0);
        pProvMngm->AcwDGUsedCnt--;
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvAcwDGAddToAcw                                   +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                        +*/
/*+                             LSA_UINT16                      AcwID                       +*/
/*+                             LSA_UINT16                      AcwDGID                     +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  AcwID              : Index in ACW array                                                +*/
/*+  AcwDGID            : Index in ACW_DG array                                             +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Add new ACW_DG to ACW.                                                    +*/
/*+                                                                                         +*/
/*+               Chain this ACW_DG in AcwDGAcwLink.                                        +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDGAddToAcw(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID,
    LSA_UINT16                  AcwDGID)
{
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;
    EDDP_CRT_PROVIDER_ACW_DG_PTR_TYPE       pAcwDG;

    EDDP_ASSERT_FALSE(AcwID >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);
    EDDP_ASSERT_FALSE(AcwDGID >= pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pAcw            = &pProvMngm->pAcwEntry[AcwID];
    pAcwDG          = &pProvMngm->pAcwDGEntry[AcwDGID];

    /* ------------------------------------------------------------ */
    /* Put this new ACW_DG to end of AcwDGList                      */
    /* ------------------------------------------------------------ */
    if (pAcw->AcwDGList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
    {
        // this ACW_DG is the first ACW_DG in AcwDGList
        pAcw->AcwDGList.BeginIdx    = AcwDGID;
        pAcw->AcwDGList.EndIdx      = AcwDGID;
        pAcwDG->AcwDGAcwLink.Prev   = EDDP_CRT_CHAIN_IDX_END;
        pAcwDG->AcwDGAcwLink.Next   = EDDP_CRT_CHAIN_IDX_END;
    }
    else
    {
        // this ACW_DG is not the first ACW_DG in AcwDGList
        pAcwDG->AcwDGAcwLink.Prev                                           = pAcw->AcwDGList.EndIdx;
        pAcwDG->AcwDGAcwLink.Next                                           = EDDP_CRT_CHAIN_IDX_END;
        pProvMngm->pAcwDGEntry[pAcw->AcwDGList.EndIdx].AcwDGAcwLink.Next = AcwDGID;
        pAcw->AcwDGList.EndIdx                                              = AcwDGID;
    }

    pAcwDG->AcwId = AcwID;
    pAcw->DfpParam.AcwDGCnt++;
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvAcwDGRemoveFromAcw                              +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                        +*/
/*+                             LSA_UINT16                      AcwDGID                     +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  AcwDGID            : Index in ACW_DG array                                             +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Remove ACW_DG from ACW - from "AcwDGAcwLink".                             +*/
/*+                                                                                         +*/
/*+               The given ACW must have ACW_DG. If no -> FATAL!                           +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDGRemoveFromAcw(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwDGID)
{
    LSA_UINT16                              Prev;
    LSA_UINT16                              Next;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;
    EDDP_CRT_PROVIDER_ACW_DG_PTR_TYPE       pAcwDG;

    EDDP_ASSERT_FALSE(AcwDGID >= pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pAcwDG          = &pProvMngm->pAcwDGEntry[AcwDGID];
    EDDP_ASSERT_FALSE(pAcwDG->AcwId == EDDP_CRT_CHAIN_IDX_END);
    pAcw            = &pProvMngm->pAcwEntry[pAcwDG->AcwId];
    Prev            = pAcwDG->AcwDGAcwLink.Prev;
    Next            = pAcwDG->AcwDGAcwLink.Next;

    // are there some ACW_DGs in this ACW ?
    if (pAcw->DfpParam.AcwDGCnt == 0)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwDGRemoveFromAcw(AcwDGID=%d): FATAL -> AcwDGCnt=0!", 
            AcwDGID);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        /* ------------------------------------------------------------ */
        /* Remove this ACW_DG from ACW-AcwDGList                        */
        /* ------------------------------------------------------------ */
        if ((Prev == EDDP_CRT_CHAIN_IDX_END) || (Next == EDDP_CRT_CHAIN_IDX_END))
        {
            if (Prev == EDDP_CRT_CHAIN_IDX_END)
            {
                // this ACW_DG is first ACW_DG in ACW-AcwDGList
                if (Next == EDDP_CRT_CHAIN_IDX_END)
                {
                    // first and only one ACW_DG in ACW-AcwDGList
                    pAcw->AcwDGList.BeginIdx    = EDDP_CRT_CHAIN_IDX_END;
                    pAcw->AcwDGList.EndIdx      = EDDP_CRT_CHAIN_IDX_END;
                }
                else
                {
                    // first ACW_DG in ACW-AcwDGList but not only one
                    pAcw->AcwDGList.BeginIdx                            = Next;
                    pProvMngm->pAcwDGEntry[Next].AcwDGAcwLink.Prev   = EDDP_CRT_CHAIN_IDX_END;
                }
            }
            else
            {
                // last ACW_DG in ACW-AcwDGList but not only one
                pProvMngm->pAcwDGEntry[Prev].AcwDGAcwLink.Next   = EDDP_CRT_CHAIN_IDX_END;
                pAcw->AcwDGList.EndIdx                              = Prev;
            }
        }
        else
        {
            // this ACW_DG is in the middle of the ACW-AcwDGList
            pProvMngm->pAcwDGEntry[Prev].AcwDGAcwLink.Next   = Next;
            pProvMngm->pAcwDGEntry[Next].AcwDGAcwLink.Prev   = Prev;
        }

        pAcwDG->AcwId = EDDP_CRT_CHAIN_IDX_END;

        EDDP_ASSERT_FALSE(pAcw->DfpParam.AcwDGCnt == 0);
        pAcw->DfpParam.AcwDGCnt--;
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvAcwDGAddToAcwRed1                               +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                        +*/
/*+                             LSA_UINT16                      AcwIDRed                    +*/
/*+                             LSA_UINT16                      AcwIDRed1                   +*/
/*+                             LSA_UINT16                      AcwDGID                     +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  AcwIDRed           : redundant ACW with FrameID(x)                                     +*/
/*+  AcwIDRed1          : redundant ACW with FrameID(x+1)                                   +*/
/*+  AcwDGID            : Index in ACW_DG array                                             +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Add new ACW_DG to redundant AcwIDRed1 with FrameID(x+1).                  +*/
/*+                                                                                         +*/
/*+               Chain this ACW_DG in AcwDGAcwLinkRed1.                                    +*/
/*+                                                                                         +*/
/*+               Call this function only if the redundant ACW(x) is already added to       +*/
/*+               ACW_DGs.                                                                  +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDGAddToAcwRed1(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwIDRed,
    LSA_UINT16                  AcwIDRed1,
    LSA_UINT16                  AcwDGID)
{
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcwRed, pAcwRed1;
    EDDP_CRT_PROVIDER_ACW_DG_PTR_TYPE       pAcwDG;

    pProvMngm    = &pDDB->CRT.Prov;
    pAcwRed         = &pProvMngm->pAcwEntry[AcwIDRed];
    pAcwRed1        = &pProvMngm->pAcwEntry[AcwIDRed1];
    pAcwDG          = &pProvMngm->pAcwDGEntry[AcwDGID];

    if (pAcwRed->DfpParam.AcwDGCnt == 0)
    {
        EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwDGAddToAcwRed1(AcwIDRed=%d, AcwIDRed1=%d, AcwDGID=%d): FATAL -> AcwRed.AcwDGCnt=0!", 
            AcwIDRed, AcwIDRed1, AcwDGID);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    // which ACW_DG order has this ACW ?
    switch (pAcwRed1->DfpParam.ACW_DG_order)
    {
        case EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_HIGH:
        {
            // ACW(x+1) has the same order like ACW(x)
            pAcwRed1->AcwDGList.BeginIdx    = pAcwRed->AcwDGList.BeginIdx;
            pAcwRed1->AcwDGList.EndIdx      = pAcwRed->AcwDGList.EndIdx;
            pAcwDG->AcwDGAcwLinkRed1.Prev   = pAcwDG->AcwDGAcwLink.Prev;
            pAcwDG->AcwDGAcwLinkRed1.Next   = pAcwDG->AcwDGAcwLink.Next;
        }
        break;

        case EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_DOWN:
        {
            // ACW(x+1) has the reversed order like ACW(x)
            pAcwRed1->AcwDGList.BeginIdx    = pAcwRed->AcwDGList.EndIdx;
            pAcwRed1->AcwDGList.EndIdx      = pAcwRed->AcwDGList.BeginIdx;
            pAcwDG->AcwDGAcwLinkRed1.Prev   = pAcwDG->AcwDGAcwLink.Next;
            pAcwDG->AcwDGAcwLinkRed1.Next   = pAcwDG->AcwDGAcwLink.Prev;
        }
        break;

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwDGAddToAcwRed1(): FATAL -> ACW_DG_order(0x%X) is unknown!", 
                pAcwRed1->DfpParam.ACW_DG_order);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }

    pAcwDG->AcwIdRed1 = AcwIDRed1;
    pAcwRed1->DfpParam.AcwDGCnt++;
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvAcwDGRemoveFromAcwRed1                          +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                        +*/
/*+                             LSA_UINT16                      AcwIDRed1                   +*/
/*+                             LSA_UINT16                      AcwDGID                     +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  AcwIDRed1          : redundant ACW with FrameID(x+1), to which the given ACW_DG belongs+*/
/*+  AcwDGID            : Index in ACW_DG array                                             +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Remove ACW_DG from ACWRed1 - from "AcwDGAcwLinkRed1".                     +*/
/*+                                                                                         +*/
/*+               The given ACW must have ACW_DG. If no -> FATAL!                           +*/
/*+                                                                                         +*/
/*+               ACW_DGs are chained in AcwDGAcwLinkRed1.                                  +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDGRemoveFromAcwRed1(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwIDRed1,
    LSA_UINT16                  AcwDGID)
{
    LSA_UINT16                              Prev;
    LSA_UINT16                              Next;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcwRed1;
    EDDP_CRT_PROVIDER_ACW_DG_PTR_TYPE       pAcwDG;

    EDDP_ASSERT_FALSE(AcwIDRed1 >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);
    EDDP_ASSERT_FALSE(AcwDGID >= pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pAcwRed1        = &pProvMngm->pAcwEntry[AcwIDRed1];
    pAcwDG          = &pProvMngm->pAcwDGEntry[AcwDGID];
    Prev            = pAcwDG->AcwDGAcwLinkRed1.Prev;
    Next            = pAcwDG->AcwDGAcwLinkRed1.Next;

    if (pAcwRed1->DfpParam.AcwDGCnt == 0)
    {
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwDGRemoveFromAcwRed1(AcwIDRed1=%d, AcwDGID=%d): FATAL -> AcwRed1.AcwDGCnt=0!", 
            AcwIDRed1, AcwDGID);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        /* ------------------------------------------------------------ */
        /* Remove this ACW_DG from ACW-AcwDGList                        */
        /* ------------------------------------------------------------ */
        if ((Prev == EDDP_CRT_CHAIN_IDX_END) || (Next == EDDP_CRT_CHAIN_IDX_END))
        {
            if (Prev == EDDP_CRT_CHAIN_IDX_END)
            {
                // this ACW_DG is first ACW_DG in ACW-AcwDGList
                if (Next == EDDP_CRT_CHAIN_IDX_END)
                {
                    // first and only one ACW_DG in ACW-AcwDGList
                    pAcwRed1->AcwDGList.BeginIdx    = EDDP_CRT_CHAIN_IDX_END;
                    pAcwRed1->AcwDGList.EndIdx      = EDDP_CRT_CHAIN_IDX_END;
                }
                else
                {
                    // first ACW_DG in ACW-AcwDGList but not only one
                    pAcwRed1->AcwDGList.BeginIdx                            = Next;
                    pProvMngm->pAcwDGEntry[Next].AcwDGAcwLinkRed1.Prev   = EDDP_CRT_CHAIN_IDX_END;
                }
            }
            else
            {
                // last ACW_DG in ACW-AcwDGList but not only one
                pProvMngm->pAcwDGEntry[Prev].AcwDGAcwLinkRed1.Next   = EDDP_CRT_CHAIN_IDX_END;
                pAcwRed1->AcwDGList.EndIdx                              = Prev;
            }
        }
        else
        {
            // this ACW_DG is in the middle of the ACW-AcwDGList
            pProvMngm->pAcwDGEntry[Prev].AcwDGAcwLinkRed1.Next   = Next;
            pProvMngm->pAcwDGEntry[Next].AcwDGAcwLinkRed1.Prev   = Prev;
        }

        pAcwDG->AcwIdRed1 = EDDP_CRT_CHAIN_IDX_END;

        EDDP_ASSERT_FALSE(pAcwRed1->DfpParam.AcwDGCnt == 0);
        pAcwRed1->DfpParam.AcwDGCnt--;
    }
}





#if defined (EDDP_CFG_HW_ERTEC200P_SUPPORT)
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvAcwDGUpdateAcwDGNextOffsetAcwHwApi              +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                        +*/
/*+                             LSA_UINT16                      AcwID                       +*/
/*+                             LSA_UINT16                      AcwDGID                     +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                     +*/
/*+  AcwID          : ACW, to which the ACW_DG belongs                                      +*/
/*+  AcwDGID        : Index in ACW_DG array                                                 +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Update                                                                    +*/
/*+                 - ACW.ACW_DG_offset (if first ACW_DG)                                   +*/
/*+                 - ACW_DG.ACW_DG_nextoffset0                                             +*/
/*+                 - ACW_DG.ACW_DG_nextoffset1                                             +*/
/*+               in APICtrl-RAM.                                                           +*/
/*+                                                                                         +*/
/*+               Update always only the given ACW_DG and the prev. ACW_DG!                 +*/
/*+                                                                                         +*/
/*+               This ACW must be already added to AcwPhaseList and available in HW.       +*/
/*+               If no, nothing to do.                                                     +*/
/*+               If yes, IMPORTANT:                                                        +*/
/*+                 the next ACW_DG must be already added to AcwDGList and available in HW  +*/
/*+                 because the "ACW_DG_nextoffset0" is the address of next ACW_DG          +*/
/*+                 in APICtrl-RAM.                                                         +*/
/*+                 If no -> FATAL!                                                         +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDGUpdateAcwDGNextOffsetAcwHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID,
    LSA_UINT16                  AcwDGID)
{
    LSA_UINT32                              New_nextoffset;     // Offset of next ACW (ACW_DG) in APICtrl-RAM
    LSA_UINT64                              ACW_Word_Temp;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;
    EDDP_CRT_PROVIDER_ACW_DG_PTR_TYPE       pAcwDG;
    EDDP_CRT_PROVIDER_ACW_DG_PTR_TYPE       pAcwDGNext;

    EDDP_ASSERT_FALSE(AcwID >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);
    EDDP_ASSERT_FALSE(AcwDGID >= pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber);

    New_nextoffset  = PNIP_R2_ACW_SND_WORD1_ACW_NEXTOFFSET_INIT_VALUE; // init for ACW
    pProvMngm    = &pDDB->CRT.Prov;
    pAcw            = &pProvMngm->pAcwEntry[AcwID];
    pAcwDG          = &pProvMngm->pAcwDGEntry[AcwDGID];

    switch (pAcw->DfpParam.ACW_DG_order)
    {
        case EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_HIGH:
        {
            /* ------------------------------------------------ */
            /* ACW.ACW_DG_offset                                */
            /*            |                                     */
            /*           \|/                                    */
            /*        ACW_DG(1) -> ACW_DG(2) -> ACW_DG(3)       */
            /*                   AcwDGAcwLink                   */
            /* ------------------------------------------------ */
            if (   (pAcw->State   & EDDP_CRT_PROV_ACW_STATE_IN_PHASELIST_MASK)  /* ACW is added to AcwPhaseList */
                && (pAcw->State   & EDDP_CRT_PROV_ACW_STATE_IN_HW_MASK)         /* ACW is available in HW */
                && (pAcwDG->State & EDDP_CRT_PROV_ACW_STATE_IN_HW_MASK)         /* ACW_DG is available in HW */
               )
            {
                /* ------------------------------------------------ */
                /* update ACW.ACW_DG_offset                         */
                /* ------------------------------------------------ */
                // only if this ACW_DG is first in AcwDGList
                if (pAcwDG->AcwDGAcwLink.Prev == EDDP_CRT_CHAIN_IDX_END)
                {
                    // this ACW_DG is first in AcwDGList
                    // address (offset) of this ACW_DG in APICtrl-RAM
                    New_nextoffset  = EDDP_CRTGetPnipAddrShift (
                                            pDDB, 
                                            (EDD_COMMON_MEM_PTR_TYPE) pAcwDG->pHwAcwDGApi, 
                                            EDDP_CRT_SHIFT_VALUE_3);

                    // get actual ACW_Word1 of this ACW
                    ACW_Word_Temp = EDDP_HAL_MEM64_READ (pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->ACW_Word1);
                    // update ACW_Word1 with new ACW.ACW_DG_offset
                    EDDP_SET_BIT_VALUE64( ACW_Word_Temp, 
                                          New_nextoffset, 
                                          PNIP_R2_ACW_SND_WORD1_ACW_DG_OFFSET_MASK,
                                          PNIP_ACW_SND_WORD1_ACW_DG_OFFSET_SHIFT)
                    // and write ACW_Word1 back to APICtrl-RAM
                    EDDP_HAL_MEM64_WRITE (pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->ACW_Word1, ACW_Word_Temp);
                }

                /* ------------------------------------------------ */
                /* update ACW_DG.ACW_DG_nextoffset0                 */
                /* ------------------------------------------------ */
                // init again (for ACW_DG) if this has changed above
                New_nextoffset = PNIP_R2_ACWDG_SND_WORD1_ACW_DG_NEXTOFFSET_INIT_VALUE;

                // is this ACW_DG last in AcwDGList ? 
                if (pAcwDG->AcwDGAcwLink.Next == EDDP_CRT_CHAIN_IDX_END)
                {
                    // update last ACW_DG in AcwDGList  -> PNIP_R2_ACWDG_SND_WORD1_ACW_DG_NEXTOFFSET_INIT_VALUE
                }
                else
                {
                    pAcwDGNext = &pProvMngm->pAcwDGEntry[pAcwDG->AcwDGAcwLink.Next];

                    // next ACW_DG must be already available in HW
                    if (pAcwDGNext->State & EDDP_CRT_PROV_ACW_STATE_IN_HW_MASK)
                    {
                        // address (offset) of next ACW_DG in APICtrl-RAM
                        New_nextoffset  = EDDP_CRTGetPnipAddrShift (
                                                pDDB, 
                                                (EDD_COMMON_MEM_PTR_TYPE) pAcwDGNext->pHwAcwDGApi, 
                                                EDDP_CRT_SHIFT_VALUE_3);
                    }
                    else
                    {
                        EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwDGUpdateAcwDGNextOffsetAcwHwApi(AcwDGID=%d): FATAL -> This ACW_DG has next ACW_DG (NextAcwDGID=%d) in AcwDGList but the next ACW_DG ist not available in HW! NextAcwDG-State=0x%X", 
                            AcwDGID, pAcwDGNext->AcwDGId, pAcwDGNext->State);
                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                    }
                }

                // get actual ACW_Word1 of this ACW_DG
                ACW_Word_Temp = EDDP_HAL_MEM64_READ (pDDB, &pAcwDG->pHwAcwDGApi->ACW_Word1);
                // update ACW_Word1 with new ACW_DG.ACW_DG_nextoffset0
                EDDP_SET_BIT_VALUE64( ACW_Word_Temp, 
                                      New_nextoffset, 
                                      PNIP_R2_ACWDG_SND_WORD1_ACW_DG_NEXTOFFSET0_MASK,
                                      PNIP_ACWDG_SND_WORD1_ACW_DG_NEXTOFFSET0_SHIFT)
                // and write ACW_Word1 back to APICtrl-RAM
                EDDP_HAL_MEM64_WRITE (pDDB, &pAcwDG->pHwAcwDGApi->ACW_Word1, ACW_Word_Temp);
            }
        }
        break;

        case EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_DOWN:
        {
            /* ------------------------------------------------ */
            /* ACW.ACW_DG_offset -------------------            */
            /*                                      |           */
            /*                                     \|/          */
            /*        ACW_DG(1) <- ACW_DG(2) <- ACW_DG(3)       */
            /*                  AcwDGAcwLinkRed1                */
            /* ------------------------------------------------ */
            if (   (pAcw->State   & EDDP_CRT_PROV_ACW_STATE_IN_PHASELIST_MASK)  /* ACW is added to AcwPhaseList */
                && (pAcw->State   & EDDP_CRT_PROV_ACW_STATE_IN_HW_MASK)         /* ACW is available in HW */
                && (pAcwDG->State & EDDP_CRT_PROV_ACW_STATE_IN_HW_MASK)         /* ACW_DG is available in HW */
               )
            {
                /* ------------------------------------------------ */
                /* update ACW.ACW_DG_offset                         */
                /* ------------------------------------------------ */
                // only if this ACW_DG is first in AcwDGList
                if (pAcwDG->AcwDGAcwLinkRed1.Prev == EDDP_CRT_CHAIN_IDX_END)
                {
                    // this ACW_DG is first in AcwDGList
                    // address (offset) of this ACW_DG in APICtrl-RAM
                    New_nextoffset  = EDDP_CRTGetPnipAddrShift (
                                            pDDB, 
                                            (EDD_COMMON_MEM_PTR_TYPE) pAcwDG->pHwAcwDGApi, 
                                            EDDP_CRT_SHIFT_VALUE_3);

                    // get actual ACW_Word1 of this ACW
                    ACW_Word_Temp = EDDP_HAL_MEM64_READ (pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->ACW_Word1);
                    // update ACW_Word1 with new ACW.ACW_DG_offset
                    EDDP_SET_BIT_VALUE64( ACW_Word_Temp, 
                                          New_nextoffset, 
                                          PNIP_R2_ACW_SND_WORD1_ACW_DG_OFFSET_MASK,
                                          PNIP_ACW_SND_WORD1_ACW_DG_OFFSET_SHIFT)
                    // and write ACW_Word1 back to APICtrl-RAM
                    EDDP_HAL_MEM64_WRITE (pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->ACW_Word1, ACW_Word_Temp);
                }

                /* ------------------------------------------------ */
                /* update ACW_DG.ACW_DG_nextoffset1                 */
                /* ------------------------------------------------ */
                // init again (for ACW_DG) if this has changed above
                New_nextoffset = PNIP_R2_ACWDG_SND_WORD1_ACW_DG_NEXTOFFSET_INIT_VALUE;

                // is this ACW_DG last in AcwDGList ? 
                if (pAcwDG->AcwDGAcwLinkRed1.Next == EDDP_CRT_CHAIN_IDX_END)
                {
                    // update last ACW_DG in AcwDGList  -> PNIP_ACWDG_SND_WORD1_ACW_DG_NEXTOFFSET_INIT_VALUE
                }
                else
                {
                    pAcwDGNext = &pProvMngm->pAcwDGEntry[pAcwDG->AcwDGAcwLinkRed1.Next];

                    // prev. ACW_DG must be already available in HW
                    if (pAcwDGNext->State & EDDP_CRT_PROV_ACW_STATE_IN_HW_MASK)
                    {
                        // address (offset) of next ACW_DG in APICtrl-RAM
                        New_nextoffset  = EDDP_CRTGetPnipAddrShift (
                                                pDDB, 
                                                (EDD_COMMON_MEM_PTR_TYPE) pAcwDGNext->pHwAcwDGApi, 
                                                EDDP_CRT_SHIFT_VALUE_3);
                    }
                    else
                    {
                        EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwDGUpdateAcwDGNextOffsetAcwHwApi(AcwDGID=%d): FATAL -> This ACW_DG has next ACW_DG (NextAcwDGID=%d) in AcwDGList but the next ACW_DG ist not available in HW! NextAcwDG-State=0x%X", 
                            AcwDGID, pAcwDGNext->AcwDGId, pAcwDGNext->State);
                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                    }
                }

                // get actual ACW_Word1 of this ACW_DG
                ACW_Word_Temp = EDDP_HAL_MEM64_READ (pDDB, &pAcwDG->pHwAcwDGApi->ACW_Word1);
                // update ACW_Word1 with new ACW_DG.ACW_DG_nextoffset1
                EDDP_SET_BIT_VALUE64( ACW_Word_Temp, 
                                      New_nextoffset, 
                                      PNIP_R2_ACWDG_SND_WORD1_ACW_DG_NEXTOFFSET1_MASK,
                                      PNIP_ACWDG_SND_WORD1_ACW_DG_NEXTOFFSET1_SHIFT)
                // and write ACW_Word1 back to APICtrl-RAM
                EDDP_HAL_MEM64_WRITE (pDDB, &pAcwDG->pHwAcwDGApi->ACW_Word1, ACW_Word_Temp);
            }
        }
        break;

        default:
        {
            EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwDGUpdateAcwDGNextOffsetAcwHwApi(AcwDGID=%d): FATAL -> ACW_DG_order(0x%X) is unknown! AcwId=%d", 
                AcwDGID, pAcw->DfpParam.ACW_DG_order, pAcw->AcwId);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }
}
#endif


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProvAcwSetActivePackFrame                       +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      AcwID                   +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  AcwID          : Index in ACW array                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function sets the ACW (for Pack-Frame) as ACTIVE.                +*/
/*+                 - Increase the AcwPhaseActCnt_X if this ACW is activated in HW.     +*/
/*+                 - Set bit EDDP_CRT_PROV_ACW_STATE_BIT_ON in ACW state.              +*/
/*+                                                                                     +*/
/*+               Call this function only after ACW_on command.                         +*/
/*+                                                                                     +*/
/*+               If no TrogEntry is available on TBase or ACW is already active,       +*/
/*+               nothing to do.                                                        +*/
/*+                                                                                     +*/
/*+               Only for RED ACWs!                                                    +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwSetActivePackFrame(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID)
{
    LSA_UINT16                              TBaseIndex;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;
    EDDP_CRT_PROVIDER_TBASE_PTR_TYPE        pTBase;

    EDDP_ASSERT_FALSE(AcwID >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pAcw            = &pProvMngm->pAcwEntry[AcwID];
    TBaseIndex      = EDDP_CRTGetTBaseIndex ( pDDB, 
                                              pAcw->AddParam.CycleReductionRatio, 
                                              pAcw->AddParam.CyclePhase);
    pTBase          = &pProvMngm->pTBaseEntry[TBaseIndex];

    // only if Trog is available on this TBase and this ACW is not yet active
    if ( !(LSA_HOST_PTR_ARE_EQUAL(pTBase->pTrogEntry, LSA_NULL)) && (!(pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK)) )
    {
        switch (pAcw->ROGPhase)
        {
            case EDDP_CRT_PROV_ACW_PHASE_R:
                pTBase->pTrogEntry->TrogAcwList.AcwPhaseActCnt_R++;
            break;

            case EDDP_CRT_PROV_ACW_PHASE_G:
            case EDDP_CRT_PROV_ACW_PHASE_O:
            default:
            {
                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwSetActivePackFrame(AcwID=%d): FATAL -> ROGPhase(0x%X) is not allowed!", 
                    AcwID, pAcw->ROGPhase);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }

        EDDP_SET_BIT_VALUE16( (pAcw->State), 
                              EDDP_CRT_PROV_ACW_STATE_BIT_ON, 
                              EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK, 
                              EDDP_CRT_PROV_ACW_STATE_ACTIVE_SHIFT)

        pProvMngm->AcwActCnt++;
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProvAcwSetPassivePackFrame                      +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      AcwID                   +*/
/*+                             LSA_BOOL                        DeleteAddParam          +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  AcwID          : Index in ACW array                                                +*/
/*+  DeleteAddParam : delete AddParam(FrameSendOffset) of given ACW                     +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function sets the ACW (for Pack-Frame) as PASSIVE                +*/
/*+                 - Increase the AcwPhaseActCnt_X if this ACW is activated in HW.     +*/
/*+                 - Set bit EDDP_CRT_PROV_ACW_STATE_BIT_ON in ACW state.              +*/
/*+                                                                                     +*/
/*+               Call this function only after ACW_off command.                        +*/
/*+                                                                                     +*/
/*+               If no TrogEntry is available on TBase or ACW is already active,       +*/
/*+               nothing to do.                                                        +*/
/*+                                                                                     +*/
/*+               Only for RED ACWs!                                                    +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwSetPassivePackFrame(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID,
    LSA_BOOL                    DeleteAddParam)
{
    LSA_UINT16                              TBaseIndex;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;
    EDDP_CRT_PROVIDER_TBASE_PTR_TYPE        pTBase;

    EDDP_ASSERT_FALSE(AcwID >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

    pProvMngm       = &pDDB->CRT.Prov;
    pAcw            = &pProvMngm->pAcwEntry[AcwID];
    TBaseIndex      = EDDP_CRTGetTBaseIndex ( pDDB, 
                                              pAcw->AddParam.CycleReductionRatio, 
                                              pAcw->AddParam.CyclePhase);
    pTBase          = &pProvMngm->pTBaseEntry[TBaseIndex];

    // only if Trog is available on this TBase and this ACW is not yet passive
    if ( !(LSA_HOST_PTR_ARE_EQUAL(pTBase->pTrogEntry, LSA_NULL)) && (pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK) )
    {
        switch (pAcw->ROGPhase)
        {
            case EDDP_CRT_PROV_ACW_PHASE_R:
            {
                EDDP_ASSERT_FALSE(pTBase->pTrogEntry->TrogAcwList.AcwPhaseActCnt_R == 0);
                pTBase->pTrogEntry->TrogAcwList.AcwPhaseActCnt_R--;

                if (DeleteAddParam)
                {
                    // delete AddParam in ACW - this cannot be used any more at the next ACTIVATE
                    pAcw->AddParam.FrameSendOffset = EDDP_CRT_FRAME_SEND_OFFSET_UNDEFINED;
                }
            }
            break;

            case EDDP_CRT_PROV_ACW_PHASE_G:
            case EDDP_CRT_PROV_ACW_PHASE_O:
            default:
            {
                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwSetPassivePackFrame(AcwID=%d): FATAL -> ROGPhase(0x%X) is not allowed!", 
                    AcwID, pAcw->ROGPhase);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }

        EDDP_SET_BIT_VALUE16( (pAcw->State), 
                              EDDP_CRT_PROV_ACW_STATE_BIT_OFF, 
                              EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK, 
                              EDDP_CRT_PROV_ACW_STATE_ACTIVE_SHIFT)

        EDDP_ASSERT_FALSE(pProvMngm->AcwActCnt == 0);
        pProvMngm->AcwActCnt--;
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProvAcwDGSetActive                              +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      AcwDGID                 +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  AcwDGID        : Index in ACW_DG array                                             +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function sets the ACW_DG as ACTIVE.                              +*/
/*+                                                                                     +*/
/*+               Call this function only after ACW_DG_on command.                      +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDGSetActive(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwDGID)
{
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw, pAcwRed1;
    EDDP_CRT_PROVIDER_ACW_DG_PTR_TYPE       pAcwDG;

    EDDP_ASSERT_FALSE(AcwDGID >= pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pAcwDG          = &pProvMngm->pAcwDGEntry[AcwDGID];
    EDDP_ASSERT_FALSE(pAcwDG->AcwId == EDDP_CRT_CHAIN_IDX_END);
    pAcw            = &pProvMngm->pAcwEntry[pAcwDG->AcwId];

    // only if this ACW_DG is not yet active
    if ( !(pAcwDG->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK) )
    {
        EDDP_SET_BIT_VALUE16( (pAcwDG->State), 
                              EDDP_CRT_PROV_ACW_STATE_BIT_ON, 
                              EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK, 
                              EDDP_CRT_PROV_ACW_STATE_ACTIVE_SHIFT)

        pAcw->DfpParam.AcwDGActCnt++;

        if (pAcwDG->AcwIdRed1 != EDDP_CRT_CHAIN_IDX_END)
        {
            // this ACW_DG has ACWRed1
            pAcwRed1 = &pProvMngm->pAcwEntry[pAcwDG->AcwIdRed1];
            pAcwRed1->DfpParam.AcwDGActCnt++;
        }

        pProvMngm->AcwActCntProvCntrlRTC3++;
        pProvMngm->AcwActCntProvCntrl++;
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProvAcwDGSetPassive                             +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      AcwDGID                 +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  AcwDGID        : Index in ACW_DG array                                             +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function sets the ACW_DG as PASSIVE.                             +*/
/*+                                                                                     +*/
/*+               Call this function only after ACW_DG_off command.                     +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDGSetPassive(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwDGID)
{
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw, pAcwRed1;
    EDDP_CRT_PROVIDER_ACW_DG_PTR_TYPE       pAcwDG;

    EDDP_ASSERT_FALSE(AcwDGID >= pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber);

    pProvMngm       = &pDDB->CRT.Prov;
    pAcwDG          = &pProvMngm->pAcwDGEntry[AcwDGID];
    pAcw            = &pProvMngm->pAcwEntry[pAcwDG->AcwId];

    // only if this ACW_DG is not yet passive
    if (pAcwDG->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK)
    {
        EDDP_SET_BIT_VALUE16( (pAcwDG->State), 
                              EDDP_CRT_PROV_ACW_STATE_BIT_OFF, 
                              EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK, 
                              EDDP_CRT_PROV_ACW_STATE_ACTIVE_SHIFT)

        EDDP_ASSERT_FALSE(pAcw->DfpParam.AcwDGActCnt == 0);
        pAcw->DfpParam.AcwDGActCnt--;

        if (pAcwDG->AcwIdRed1 != EDDP_CRT_CHAIN_IDX_END)
        {
            // this ACW_DG has ACWRed1
            pAcwRed1 = &pProvMngm->pAcwEntry[pAcwDG->AcwIdRed1];
            EDDP_ASSERT_FALSE(pAcwRed1->DfpParam.AcwDGActCnt == 0);
            pAcwRed1->DfpParam.AcwDGActCnt--;
        }

        EDDP_ASSERT_FALSE(pProvMngm->AcwActCntProvCntrlRTC3 == 0);
        pProvMngm->AcwActCntProvCntrlRTC3--;

        EDDP_ASSERT_FALSE(pProvMngm->AcwActCntProvCntrl == 0);
        pProvMngm->AcwActCntProvCntrl--;
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProvAcwDGDeleteActiveBitHwApi                   +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      AcwDGID                 +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  AcwID          : Index in ACW array                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function deletes the Active-Bit of ACW_DG in APICtrl-RAM         +*/
/*+               (normally done by command EDDP_CRT_CMD_PPM_PACK_ACW_DG_OFF of         +*/
/*+               CMD-Interface).                                                       +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDGDeleteActiveBitHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwDGID)
{
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_ACW_DG_PTR_TYPE       pAcwDG;
    LSA_UINT64                              ACW_Word_Temp;

    pProvMngm = &pDDB->CRT.Prov;
    pAcwDG = &pProvMngm->pAcwDGEntry[AcwDGID];
    pAcwDG->pHwAcwDGApi = &pProvMngm->pHwBaseAcwDGApi[AcwDGID];
    
    // only if this ACW_DG is not yet passive
    if (pAcwDG->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK)
    {
        // read current ACW_DG.Word1 from APICtrl-RAM
        ACW_Word_Temp = EDDP_HAL_MEM64_READ(pDDB, &pAcwDG->pHwAcwDGApi->ACW_Word1);

        // delete Active-Bit
        EDDP_SET_BIT_VALUE64( ACW_Word_Temp,
                              PNIP_ACWDG_SND_WORD1_ACTIVE_OFF_VALUE,
                              PNIP_ACWDG_SND_WORD1_ACTIVE_MASK,
                              PNIP_ACWDG_SND_WORD1_ACTIVE_SHIFT)

        // write ACW_DG.Word1 back to APICtrl-RAM
        EDDP_HAL_MEM64_WRITE(pDDB, &pAcwDG->pHwAcwDGApi->ACW_Word1, ACW_Word_Temp);
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProvAcwSetActiveBitHwApi                        +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      AcwID                   +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  AcwID          : Index in ACW array                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function sets the Active-Bit of ACW in APICtrl-RAM               +*/
/*+               (only if it is not yet set).                                          +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwSetActiveBitHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID)
{
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;
    LSA_UINT64                              ACW_Word_Temp;

    pProvMngm = &pDDB->CRT.Prov;
    pAcw = &pProvMngm->pAcwEntry[AcwID];

    // read current ACW.Word1 from APICtrl-RAM
    ACW_Word_Temp = EDDP_HAL_MEM64_READ(pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->ACW_Word1);

    // only if Active-Bit is not yet set
    if (!(ACW_Word_Temp & PNIP_ACW_SND_WORD1_ACTIVE_MASK))
    {
        // set Active-Bit
        EDDP_SET_BIT_VALUE64( ACW_Word_Temp,
                              PNIP_ACW_SND_WORD1_ACTIVE_INIT_VALUE,
                              PNIP_ACW_SND_WORD1_ACTIVE_MASK,
                              PNIP_ACW_SND_WORD1_ACTIVE_SHIFT)

        // write ACW.Word1 back to APICtrl-RAM
        EDDP_HAL_MEM64_WRITE(pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->ACW_Word1, ACW_Word_Temp);
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProvAcwDeleteActiveBitHwApi                     +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      AcwID                   +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  AcwID          : Index in ACW array                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function deletes the Active-Bit of ACW in APICtrl-RAM            +*/
/*+               (only if it is set).                                                  +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwDeleteActiveBitHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID)
{
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;
    LSA_UINT64                              ACW_Word_Temp;

    pProvMngm = &pDDB->CRT.Prov;
    pAcw = &pProvMngm->pAcwEntry[AcwID];

    // read current ACW.Word1 from APICtrl-RAM
    ACW_Word_Temp = EDDP_HAL_MEM64_READ(pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->ACW_Word1);

    // only if Active-Bit is set
    if (ACW_Word_Temp & PNIP_ACW_SND_WORD1_ACTIVE_MASK)
    {
        // delete Active-Bit
        EDDP_SET_BIT_VALUE64( ACW_Word_Temp,
                              0x0,
                              PNIP_ACW_SND_WORD1_ACTIVE_MASK,
                              PNIP_ACW_SND_WORD1_ACTIVE_SHIFT)

        // write ACW.Word1 back to APICtrl-RAM
        EDDP_HAL_MEM64_WRITE(pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->ACW_Word1, ACW_Word_Temp);
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProvAcwCheckAndSetDstMACFromProviderControlRQB      +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB            +*/
/*+                             LSA_UINT16                                  ProviderID      +*/
/*+                             EDD_UPPER_CSRT_PROVIDER_CONTROL_PTR_TYPE    pRqbParam       +*/
/*+                                                                                         +*/
/*+  Result                :    EDD_RSP                                                     +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                                         +*/
/*+  ProviderID : ProviderID                                                                +*/
/*+  pRqbParam  : Pointer to RQB parameter from user (UPPER-RQB)                            +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: This function checks the DstMAC address in RQB and sets it to ACWs        +*/
/*+               of this provider.                                                         +*/
/*+                                                                                         +*/
/*+               This function must be called only if provider is activated.               +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwCheckAndSetDstMACFromProviderControlRQB(
    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB,
    LSA_UINT16                                  ProviderID,
    EDD_UPPER_CSRT_PROVIDER_CONTROL_PTR_TYPE    pRqbParam)
{
    EDD_RSP                                     Response;
    LSA_UINT16                                  AcwIdx;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE             pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE                  pProvider;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE              pAcw;

    EDDP_ASSERT_FALSE(ProviderID >= pDDB->CRT.CfgParam.ProviderMaxNumber);

    Response        = EDD_STS_OK;
    pProvMngm    = &pDDB->CRT.Prov;
    pProvider       = &pProvMngm->pProvEntry[ProviderID];

    switch (pProvider->ROGPhase)
    {
        case EDDP_CRT_PROV_ACW_PHASE_G:
        case EDDP_CRT_PROV_ACW_PHASE_O:
        {
            EDDP_ASSERT_FALSE(pProvider->AcwList.BeginIdx == EDDP_CRT_CHAIN_IDX_END);
            pAcw = &pProvMngm->pAcwEntry[pProvider->AcwList.BeginIdx];

            // check following parameters only if (Mode=ACTIVATE)
            // and only if this ACW is not yet active
            if (   (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE)
                && (!(pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK))
               )
            {
                if ((EDDP_IS_ZERO_MAC(pRqbParam->DstMAC)) && (EDDP_IS_ZERO_MAC(pAcw->AddParam.DstMAC)))
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTProvAcwCheckAndSetDstMACFromProviderControlRQB(ProviderID=%d): ERROR -> DstMAC(00:00:00:00:00:00) in Provider and in RQB!", 
                        ProviderID);
                }
                else
                {
                    // check further only if RQB.DstMAC is valid and is different to Provider.DstMAC
                    if ( !(EDDP_IS_ZERO_MAC(pRqbParam->DstMAC)) )
                    {
                        if (   (pRqbParam->DstMAC.MacAdr[0] != pAcw->AddParam.DstMAC.MacAdr[0])
                            || (pRqbParam->DstMAC.MacAdr[1] != pAcw->AddParam.DstMAC.MacAdr[1])
                            || (pRqbParam->DstMAC.MacAdr[2] != pAcw->AddParam.DstMAC.MacAdr[2])
                            || (pRqbParam->DstMAC.MacAdr[3] != pAcw->AddParam.DstMAC.MacAdr[3])
                            || (pRqbParam->DstMAC.MacAdr[4] != pAcw->AddParam.DstMAC.MacAdr[4])
                            || (pRqbParam->DstMAC.MacAdr[5] != pAcw->AddParam.DstMAC.MacAdr[5])
                           )
                        {
                            if (pProvider->ProviderFlags & EDDP_CRT_PROV_FLAGS_IS_MCCR)
                            {
                                // MultiCast-Provider - is the DstMAC a MultiCast address ?
                                if (!(EDDP_IS_MC_MAC(pRqbParam->DstMAC)))
                                {
                                    Response = EDD_STS_ERR_PARAM;
                                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                    EDDP_CRT_TRACE_07(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTProvAcwCheckAndSetDstMACFromProviderControlRQB(ProviderID=%d): ERROR -> MultiCast-Provider (RTC1/2) but the DstMAC(%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X) is not MultiCast address!", 
                                        ProviderID, 
                                        pRqbParam->DstMAC.MacAdr[0], 
                                        pRqbParam->DstMAC.MacAdr[1], 
                                        pRqbParam->DstMAC.MacAdr[2], 
                                        pRqbParam->DstMAC.MacAdr[3], 
                                        pRqbParam->DstMAC.MacAdr[4], 
                                        pRqbParam->DstMAC.MacAdr[5]);
                                }
                            }
                            else
                            {
                                // UniCast-Provider - is the DstMAC a UniCast address ?
                                if (EDDP_IS_MC_MAC(pRqbParam->DstMAC))
                                {
                                    Response = EDD_STS_ERR_PARAM;
                                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                    EDDP_CRT_TRACE_07(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTProvAcwCheckAndSetDstMACFromProviderControlRQB(ProviderID=%d): ERROR -> UniCast-Provider (RTC1/2) but the DstMAC(%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X) is not UniCast address!", 
                                        ProviderID, 
                                        pRqbParam->DstMAC.MacAdr[0], 
                                        pRqbParam->DstMAC.MacAdr[1], 
                                        pRqbParam->DstMAC.MacAdr[2], 
                                        pRqbParam->DstMAC.MacAdr[3], 
                                        pRqbParam->DstMAC.MacAdr[4], 
                                        pRqbParam->DstMAC.MacAdr[5]);
                                }
                            }

                            // new DstMAC address is present - store it in ACWs of this provider
                            if (Response == EDD_STS_OK)
                            {
                                AcwIdx = pProvider->AcwList.BeginIdx;
                                while (AcwIdx != EDDP_CRT_CHAIN_IDX_END)
                                {
                                    EDDP_ASSERT_FALSE(AcwIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);
                                    pAcw = &pProvMngm->pAcwEntry[AcwIdx];

                                    pAcw->AddParam.DstMAC = pRqbParam->DstMAC;

                                    // next AcwIdx in this provider
                                    AcwIdx = pProvMngm->pAcwEntry[AcwIdx].AcwProvLink.Next;
                                }
                            }
                        }
                    }
                }
            }
        }
        break;

        case EDDP_CRT_PROV_ACW_PHASE_R:
        {
            if (pProvider->AuxAcwId == EDDP_CRT_CHAIN_IDX_END)
            {
                // Provider: 1-CR start up (advanced)
                // the DstMAC address will be set from PRM-PDIRData
            }
            else
            {
                // Provider: 2-CR start up (legacy)

                /* -------------------------------- */
                /* DstMAC address for AUX-ACW       */
                /* -------------------------------- */
                EDDP_ASSERT_FALSE(pProvider->AuxAcwId == EDDP_CRT_CHAIN_IDX_END);
                pAcw = &pProvMngm->pAcwEntry[pProvider->AuxAcwId];

                // check following parameters only if AUX-ACW is activated (MODE_ACTIVATE_AUX) - AUX-ACW is activated always first
                // and only if this ACW is not yet active
                if (   (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE_AUX)
                    && (!(pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK))
                   )
                {
                    if ((EDDP_IS_ZERO_MAC(pRqbParam->DstMAC)) && (EDDP_IS_ZERO_MAC(pAcw->AddParam.DstMAC)))
                    {
                        Response = EDD_STS_ERR_PARAM;
                        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTProvAcwCheckAndSetDstMACFromProviderControlRQB(ProviderID=%d): ERROR -> DstMAC(00:00:00:00:00:00) in Provider and in RQB!", 
                            ProviderID);
                    }
                    else
                    {
                        // check further only if RQB.DstMAC is valid and is different to Provider.DstMAC
                        if ( !(EDDP_IS_ZERO_MAC(pRqbParam->DstMAC)) )
                        {
                            if (   (pRqbParam->DstMAC.MacAdr[0] != pAcw->AddParam.DstMAC.MacAdr[0])
                                || (pRqbParam->DstMAC.MacAdr[1] != pAcw->AddParam.DstMAC.MacAdr[1])
                                || (pRqbParam->DstMAC.MacAdr[2] != pAcw->AddParam.DstMAC.MacAdr[2])
                                || (pRqbParam->DstMAC.MacAdr[3] != pAcw->AddParam.DstMAC.MacAdr[3])
                                || (pRqbParam->DstMAC.MacAdr[4] != pAcw->AddParam.DstMAC.MacAdr[4])
                                || (pRqbParam->DstMAC.MacAdr[5] != pAcw->AddParam.DstMAC.MacAdr[5])
                               )
                            {
                                if (EDDP_IS_MC_MAC(pRqbParam->DstMAC))
                                {
                                    Response = EDD_STS_ERR_PARAM;
                                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                    EDDP_CRT_TRACE_07(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTProvAcwCheckAndSetDstMACFromProviderControlRQB(ProviderID=%d): ERROR -> UniCast-Provider (RTC3 legacy) but the DstMAC(%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X) is not UniCast address!", 
                                        ProviderID, 
                                        pRqbParam->DstMAC.MacAdr[0], 
                                        pRqbParam->DstMAC.MacAdr[1], 
                                        pRqbParam->DstMAC.MacAdr[2], 
                                        pRqbParam->DstMAC.MacAdr[3], 
                                        pRqbParam->DstMAC.MacAdr[4], 
                                        pRqbParam->DstMAC.MacAdr[5]);
                                }
                                else
                                {
                                    // everything is ok - store new DstMAC in AUX-ACW of this provider
                                    pAcw->AddParam.DstMAC = pRqbParam->DstMAC;
                                }
                            }
                        }
                    }
                }

                /* -------------------------------- */
                /* DstMAC address for RTC3-ACW      */
                /* -------------------------------- */
                if (Response == EDD_STS_OK)
                {
                    EDDP_ASSERT_FALSE(pProvider->AcwList.BeginIdx == EDDP_CRT_CHAIN_IDX_END);
                    pAcw = &pProvMngm->pAcwEntry[pProvider->AcwList.BeginIdx];

                    // only if RTC3-ACW is activated (MODE_ACTIVATE) and only if this ACW is not yet active
                    if (   (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE)
                        && (!(pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK))
                       )
                    {
                        // the DstMAC address is already valid in AUX-ACW (store it in RTC3-ACW)
                        pAcw->AddParam.DstMAC = pProvMngm->pAcwEntry[pProvider->AuxAcwId].AddParam.DstMAC;
                    }
                }
            }
        }
        break;

        default:
        {
            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwCheckAndSetDstMACFromProviderControlRQB(ProviderID=%d): FATAL -> ROGPhase unknown! ROGPhase=0x%X", 
                ProviderID, pProvider->ROGPhase);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }

    return (Response);
}




/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProvAcwAddToTreductionselect                    +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                        +*/
/*+                             LSA_UINT16                  AcwID                       +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  AcwID          : Index in ACW array                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function adds the given ACW to "ACW_Treductionselect".           +*/
/*+               If this ACW is first ACW in this reduction, the reduction is          +*/
/*+               activated in register "ACW_Treductionselect_IFA/B".                   +*/
/*+                                                                                     +*/
/*+               Do something only if TrogEntry is available on TBase and ACW is       +*/
/*+               not yet counted for ACW_Treductionselect.                             +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwAddToTreductionselect(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID)
{
    LSA_UINT16                              TBaseIndex;
    LSA_UINT32                              RegAddrTemp;
    LSA_UINT32                              ReductionNrMask, ReductionNrShift;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
//  EDDP_CRT_PROVIDER_DDB_MNGM_PTR_TYPE     pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;
    EDDP_CRT_PROVIDER_TBASE_PTR_TYPE        pTBase;

    EDDP_ASSERT_FALSE(AcwID >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pProvMngm    = &pDDB->CRT.Prov;
    pAcw            = &pProvMngm->pAcwEntry[AcwID];
    TBaseIndex      = EDDP_CRTGetTBaseIndex ( pDDB, 
                                              pAcw->AddParam.CycleReductionRatio, 
                                              pAcw->AddParam.CyclePhase);
    pTBase          = &pDDB->CRT.Prov.pTBaseEntry[TBaseIndex];
    RegAddrTemp     = PNIP_REG_ACW_TREDUCTIONSELECT_IFA;

    EDDP_CRT_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProvAcwAddToTreductionselect(AcwID=%d): current ACW_Treductionselect=0x%X, ReductionNr=%d, TBaseIndex=%d, pTBase->pTrogEntry=0x%X, Acw-State=0x%X", 
        AcwID, pProvMngm->ACW_Treductionselect, pTBase->ReductionNr, TBaseIndex, pTBase->pTrogEntry, pAcw->State);

    // only if Trog is available on this TBase and this ACW is not yet counted for ACW_Treductionselect
    if ( !(LSA_HOST_PTR_ARE_EQUAL(pTBase->pTrogEntry, LSA_NULL)) && (!(pAcw->State & EDDP_CRT_PROV_ACW_STATE_TREDUCTIONSELECT_MASK)) )
    {
        EDDP_ASSERT_FALSE(!(EDDP_CRT_IS_REDUCTIONNR_IN_VALID_RANGE(pTBase->ReductionNr)));

        // add this ACW to ACW_Treductionselect
        pProvMngm->Treductionselect[(pTBase->ReductionNr)-1].AcwCnt++;
        // and set this flag in Acw-State
        EDDP_SET_BIT_VALUE16( (pAcw->State), 
                              EDDP_CRT_PROV_ACW_STATE_BIT_ON, 
                              EDDP_CRT_PROV_ACW_STATE_TREDUCTIONSELECT_MASK, 
                              EDDP_CRT_PROV_ACW_STATE_TREDUCTIONSELECT_SHIFT)

        // activate reduction in register "ACW_Treductionselect_IFA/B" only if this is first ACW in reduction
        if (pProvMngm->Treductionselect[(pTBase->ReductionNr)-1].AcwCnt == 1)
        {
            ReductionNrShift    = pTBase->ReductionNr - 1;
            ReductionNrMask     = 1;
            EDDP_SHIFT_VALUE_LEFT(ReductionNrMask, ReductionNrShift);

            // change "ACW_Treductionselect" and write it to register
            EDDP_SET_BIT_VALUE32(pProvMngm->ACW_Treductionselect, 1, ReductionNrMask, ReductionNrShift);
            EDDP_HAL_REG32_WRITE(pDDB, RegAddrTemp, pProvMngm->ACW_Treductionselect);
        }

        EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProvAcwAddToTreductionselect(AcwID=%d): new ACW_Treductionselect=0x%X, AcwCnt=%d, Acw-State=0x%X", 
            AcwID, pProvMngm->ACW_Treductionselect, pProvMngm->Treductionselect[(pTBase->ReductionNr)-1].AcwCnt, pAcw->State);
    }
}






/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProvAcwRemoveFromTreductionselect               +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                        +*/
/*+                             LSA_UINT16                  AcwID                       +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  AcwID          : Index in ACW array                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function removes the given ACW from "ACW_Treductionselect".      +*/
/*+               If this ACW is last ACW in this reduction, the reduction is           +*/
/*+               deactivated in register "ACW_Treductionselect_IFA/B".                 +*/
/*+                                                                                     +*/
/*+               Do something only if TrogEntry is available on TBase and ACW is       +*/
/*+               already counted for ACW_Treductionselect.                             +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwRemoveFromTreductionselect(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID)
{
    LSA_UINT16                              TBaseIndex;
    LSA_UINT32                              RegAddrTemp;
    LSA_UINT32                              ReductionNrMask, ReductionNrShift;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;
    EDDP_CRT_PROVIDER_TBASE_PTR_TYPE        pTBase;

    EDDP_ASSERT_FALSE(AcwID >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pAcw            = &pProvMngm->pAcwEntry[AcwID];
    TBaseIndex      = EDDP_CRTGetTBaseIndex ( pDDB, 
                                              pAcw->AddParam.CycleReductionRatio, 
                                              pAcw->AddParam.CyclePhase);
    pTBase          = &pDDB->CRT.Prov.pTBaseEntry[TBaseIndex];
    RegAddrTemp     = PNIP_REG_ACW_TREDUCTIONSELECT_IFA;

    EDDP_CRT_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProvAcwRemoveFromTreductionselect(AcwID=%d): current ACW_Treductionselect=0x%X, ReductionNr=%d, TBaseIndex=%d, pTBase->pTrogEntry=0x%X, Acw-State=0x%X", 
        AcwID, pProvMngm->ACW_Treductionselect, pTBase->ReductionNr, TBaseIndex, pTBase->pTrogEntry, pAcw->State);

    // only if Trog is available on this TBase and this ACW is already counted for ACW_Treductionselect
    if ( !(LSA_HOST_PTR_ARE_EQUAL(pTBase->pTrogEntry, LSA_NULL)) && (pAcw->State & EDDP_CRT_PROV_ACW_STATE_TREDUCTIONSELECT_MASK) )
    {
        EDDP_ASSERT_FALSE(!(EDDP_CRT_IS_REDUCTIONNR_IN_VALID_RANGE(pTBase->ReductionNr)));

        // remove this ACW from ACW_Treductionselect
        EDDP_ASSERT_FALSE(pProvMngm->Treductionselect[(pTBase->ReductionNr)-1].AcwCnt == 0);
        pProvMngm->Treductionselect[(pTBase->ReductionNr)-1].AcwCnt--;
        // and delete this flag in Acw-State
        EDDP_SET_BIT_VALUE16( (pAcw->State), 
                              EDDP_CRT_PROV_ACW_STATE_BIT_OFF, 
                              EDDP_CRT_PROV_ACW_STATE_TREDUCTIONSELECT_MASK, 
                              EDDP_CRT_PROV_ACW_STATE_TREDUCTIONSELECT_SHIFT)

        // delete reduction in register "ACW_Treductionselect_IFA/B" only if this is last ACW in reduction
        if (pProvMngm->Treductionselect[(pTBase->ReductionNr)-1].AcwCnt == 0)
        {
            ReductionNrShift    = pTBase->ReductionNr - 1;
            ReductionNrMask     = 1;
            EDDP_SHIFT_VALUE_LEFT(ReductionNrMask, ReductionNrShift);

            // change "ACW_Treductionselect" and write it to register
            EDDP_SET_BIT_VALUE32(pProvMngm->ACW_Treductionselect, 0, ReductionNrMask, ReductionNrShift);
            EDDP_HAL_REG32_WRITE(pDDB, RegAddrTemp, pProvMngm->ACW_Treductionselect);
        }

        EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProvAcwRemoveFromTreductionselect(AcwID=%d): new ACW_Treductionselect=0x%X, AcwCnt=%d, Acw-State=0x%X", 
            AcwID, pProvMngm->ACW_Treductionselect, pProvMngm->Treductionselect[(pTBase->ReductionNr)-1].AcwCnt, pAcw->State);
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProvAcwSetupDrmAndDestination                   +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                        +*/
/*+                             LSA_UINT16                  AcwID                       +*/
/*+                             LSA_BOOL                    isValid                     +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  AcwID          : Index in ACW array                                                +*/
/*+  isValid        : LSA_TRUE  - DRM(0), Destination(TxPort)                           +*/
/*+                   LSA_FALSE - DRM(1), Destination(0)                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function writes the ACW bits:                                    +*/
/*+                 - DRM           : DestinationRawMode                                +*/
/*+                 - Destination   : TxPort                                            +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwSetupDrmAndDestination(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID,
    LSA_BOOL                    isValid)
{
    LSA_UINT64                          ACW_Word_Temp;
    LSA_UINT32                          Destination;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE      pAcw;

    EDDP_ASSERT_FALSE(EDDP_HAL_HW_PNIP_REV1 != EDDP_HALGetPnipRevision(pDDB));
    EDDP_ASSERT_FALSE(AcwID >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

    pAcw = &pDDB->CRT.Prov.pAcwEntry[AcwID];

    // only if ACW available in APICtrl-RAM
    if (pAcw->State & EDDP_CRT_PROV_ACW_STATE_IN_HW_MASK)
    {
        // get actual ACW_Word1 of this ACW
        // note: this also works for PNIP_REV3, even if PNIP_ACW_SND_PTR_TYPE is used, as ACW_Word1 has the same position within the structure
        ACW_Word_Temp   = EDDP_HAL_MEM64_READ (pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->ACW_Word1);
        Destination     = 0;

        if (isValid)
        {
            // set DRM(0), Destination(TxPort)
            EDDP_SET_BIT_VALUE64( ACW_Word_Temp, 
                                  PNIP_ACW_SND_WORD1_DRM_OFF_VALUE, 
                                  PNIP_ACW_SND_WORD1_DRM_MASK, 
                                  PNIP_ACW_SND_WORD1_DRM_SHIFT)
            
            // get Hw-PortID from User-PortID
            switch (EDDP_SWI_get_HwPortID(pDDB, pAcw->AddParam.TxPortID))
            {
                case 1: Destination = PNIP_ACW_SND_WORD1_DESTINATION_P1_VALUE;  break;
                case 2: Destination = PNIP_ACW_SND_WORD1_DESTINATION_P2_VALUE;  break;
                case 3: Destination = PNIP_ACW_SND_WORD1_DESTINATION_P3_VALUE;  break;
                case 4: Destination = PNIP_ACW_SND_WORD1_DESTINATION_P4_VALUE;  break;
                default:
                {
                    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProvAcwSetupDrmAndDestination(): FATAL -> Invalid Hw-PortID from SWI! Acw-PortID=%d, Hw-PortID=%d", 
                        pAcw->AddParam.TxPortID, EDDP_SWI_get_HwPortID (pDDB, pAcw->AddParam.TxPortID));
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }
                break;
            }
        }
        else
        {
            // set DRM(1), Destination(0)
            EDDP_SET_BIT_VALUE64( ACW_Word_Temp, 
                                  PNIP_ACW_SND_WORD1_DRM_ON_VALUE, 
                                  PNIP_ACW_SND_WORD1_DRM_MASK, 
                                  PNIP_ACW_SND_WORD1_DRM_SHIFT)
        }

        EDDP_SET_BIT_VALUE64( ACW_Word_Temp, 
                              Destination, 
                              PNIP_ACW_SND_WORD1_DESTINATION_MASK, 
                              PNIP_ACW_SND_WORD1_DESTINATION_SHIFT)

        // and write ACW_Word1 back to APICtrl-RAM
        // note: this also works for PNIP_REV3, even if PNIP_ACW_SND_PTR_TYPE is used, as ACW_Word1 has the same position within the structure
        EDDP_HAL_MEM64_WRITE (pDDB, &((PNIP_ACW_SND_PTR_TYPE)(pAcw->pHwAcwApi))->ACW_Word1, ACW_Word_Temp);
    }
}
#endif





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProvAcwAcwDGDeleteHwApi                         +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      AcwID                   +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  AcwID          : Index in ACW array                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function deletes one ACW and its ACW_DGs in APICtrl-RAM.         +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProvAcwAcwDGDeleteHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  AcwID)
{
    LSA_UINT16                                  AcwDGID;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE              pAcw;
    EDDP_CRT_PROVIDER_ACW_DG_PTR_TYPE           pAcwDG;
    EDDP_CRT_PROVIDER_PACK_GROUP_PTR_TYPE       pPackGroup;

    EDDP_ASSERT_FALSE(AcwID >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);
    pAcw = &pDDB->CRT.Prov.pAcwEntry[AcwID];
    EDDP_ASSERT_FALSE(pAcw->DfpParam.PackGrId == EDDP_CRT_CHAIN_IDX_END);
    pPackGroup = &pDDB->CRT.Prov.pProvPackGroupEntry[pAcw->DfpParam.PackGrId];

    // delete ACW in APICtrl-RAM
    EDDP_CRTProvAcwDeleteHwApi (pDDB, AcwID);

    // delete ACW_DGs in APICtrl-RAM
    // is this pack group with redundant FrameID?
    if (EDDP_IS_IRT_REDUNDANT_FRAMEID(pAcw->AddParam.FrameID))
    {
        /* -------------------------------- */
        /* redundant pack group             */
        /* -------------------------------- */
        if (pPackGroup->isRedundant1)
        {
            /* ---------------------------------------- */
            /* redundant FrameID(x+1)                   */
            /* ACW_DGs are chained in AcwDGAcwLinkRed1  */
            /* ---------------------------------------- */
            switch (pAcw->DfpParam.ACW_DG_order)
            {
            case EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_HIGH:
            {
                AcwDGID = pAcw->AcwDGList.BeginIdx;
                while (AcwDGID != EDDP_CRT_CHAIN_IDX_END)
                {
                    EDDP_ASSERT_FALSE(AcwDGID >= pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber);
                    pAcwDG = &pDDB->CRT.Prov.pAcwDGEntry[AcwDGID];

                    if ((pAcwDG->AcwId != EDDP_CRT_CHAIN_IDX_END) && (pAcwDG->AcwIdRed1 != EDDP_CRT_CHAIN_IDX_END))
                    {
                        // this ACW_DG is still used by both ACWs: by FrameID(x) and FrameID(x+1)
                        // do not delete this ACW_DG in APICtrl-RAM !!!
                        AcwDGID = EDDP_CRT_CHAIN_IDX_END;
                    }
                    else
                    {
                        // delete this ACW_DG in APICtrl-RAM
                        EDDP_CRTProvAcwDGDeleteHwApi (pDDB, AcwDGID);
                        // next AcwDGID in this ACW
                        AcwDGID = pDDB->CRT.Prov.pAcwDGEntry[AcwDGID].AcwDGAcwLinkRed1.Next;
                    }
                }
            }
            break;

            case EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_DOWN:
            {
                AcwDGID = pAcw->AcwDGList.EndIdx;
                while (AcwDGID != EDDP_CRT_CHAIN_IDX_END)
                {
                    EDDP_ASSERT_FALSE(AcwDGID >= pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber);
                    pAcwDG = &pDDB->CRT.Prov.pAcwDGEntry[AcwDGID];

                    if ((pAcwDG->AcwId != EDDP_CRT_CHAIN_IDX_END) && (pAcwDG->AcwIdRed1 != EDDP_CRT_CHAIN_IDX_END))
                    {
                        // this ACW_DG is still used by both ACWs: by FrameID(x) and FrameID(x+1)
                        // do not delete this ACW_DG in APICtrl-RAM !!!
                        AcwDGID = EDDP_CRT_CHAIN_IDX_END;
                    }
                    else
                    {
                        // delete this ACW_DG in APICtrl-RAM
                        EDDP_CRTProvAcwDGDeleteHwApi (pDDB, AcwDGID);
                        // next AcwDGID in this ACW
                        AcwDGID = pDDB->CRT.Prov.pAcwDGEntry[AcwDGID].AcwDGAcwLinkRed1.Prev;
                    }
                }
            }
            break;

            default:
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfPpmPackGroup(): FATAL -> ACW_DG_order(0x%X) is unknown!",
                    pAcw->DfpParam.ACW_DG_order);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
            }
        }
        else
        {
            /* ---------------------------------------- */
            /* redundant FrameID(x)                     */
            /* ACW_DGs are chained in AcwDGAcwLink      */
            /* ---------------------------------------- */
            AcwDGID = pAcw->AcwDGList.BeginIdx;
            while (AcwDGID != EDDP_CRT_CHAIN_IDX_END)
            {
                EDDP_ASSERT_FALSE(AcwDGID >= pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber);
                pAcwDG = &pDDB->CRT.Prov.pAcwDGEntry[AcwDGID];

                if ((pAcwDG->AcwId != EDDP_CRT_CHAIN_IDX_END) && (pAcwDG->AcwIdRed1 != EDDP_CRT_CHAIN_IDX_END))
                {
                    // this ACW_DG is still used by both ACWs: by FrameID(x) and FrameID(x+1)
                    // do not delete this ACW_DG in APICtrl-RAM !!!
                    AcwDGID = EDDP_CRT_CHAIN_IDX_END;
                }
                else
                {
                    // delete this ACW_DG in APICtrl-RAM
                    EDDP_CRTProvAcwDGDeleteHwApi (pDDB, AcwDGID);
                    // next AcwDGID in this ACW
                    AcwDGID = pDDB->CRT.Prov.pAcwDGEntry[AcwDGID].AcwDGAcwLink.Next;
                }
            }
        }
    }
    else
    {
        /* -------------------------------- */
        /* not redundant pack group         */
        /* -------------------------------- */
        // delete ACW_DGs in APICtrl-RAM
        AcwDGID = pAcw->AcwDGList.BeginIdx;
        while (AcwDGID != EDDP_CRT_CHAIN_IDX_END)
        {
            EDDP_ASSERT_FALSE(AcwDGID >= pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber);
            // delete this ACW_DG in APICtrl-RAM
            EDDP_CRTProvAcwDGDeleteHwApi (pDDB, AcwDGID);
            // next AcwDGID in this ACW
            AcwDGID = pDDB->CRT.Prov.pAcwDGEntry[AcwDGID].AcwDGAcwLink.Next;
        }
    }
}
