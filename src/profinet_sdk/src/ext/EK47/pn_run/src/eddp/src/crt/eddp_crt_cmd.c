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
/*  F i l e               &F: eddp_crt_cmd.c                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  CRT (Cycle Real Time) for EDDP                                           */
/*      - CMD Interface (CmdRequest, Call-Back-Function)                     */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who         What                                             */
#ifdef EDDP_MESSAGE
/*  16.03.09    Sergeev     Erstellung                                       */
/*  24.11.10    JS          changes to prevent compiler warnings             */
#endif
/*****************************************************************************/


/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/

#define LTRC_ACT_MODUL_ID   29
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_CRT_CMD */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"           /* eddp headerfiles */
#include "eddp_int.h"           /* internal header  */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTCmdRequestPpm                                       +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB                +*/
/*+                             LSA_UINT16                              CrtCommand          +*/
/*+                             LSA_UINT16                              SynAsyn             +*/
/*+                                                                                         +*/
/*+  Result                :    EDD_RSP                                                     +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  CrtCommand         : CRT-Command                                                       +*/
/*+  SynAsyn            : synchron or asynchron                                             +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Central collect function for calling of Command Interface                 +*/
/*+               for all PPM commands.                                                     +*/
/*+                                                                                         +*/
/*+               CRT commands are defined in EDDP_CRT_CMD_PARAM_TYPE.                      +*/
/*+                                                                                         +*/
/*+               Fill command parameter and call command interface                         +*/
/*+               If Response of CMD-IF not OK, CMD-IF has not accepted the command         +*/
/*+               -> FATAL                                                                  +*/
/*+                                                                                         +*/
/*+               If synchron, no Cbf-function available and command is finished            +*/
/*+               after calling of command interface.                                       +*/
/*+                                                                                         +*/
/*+               If asynchron, command is finished in the Cbf-function that                +*/
/*+               is called by command interface.                                           +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCmdRequestPpm(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  CrtCommand,
    LSA_UINT16                  SynAsyn,
    EDDP_CMD_PARAM_PTR_TYPE     pCmdParam)
{
    EDD_RSP                             ResponseCmd;
    
    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTCmdRequestPpm(CrtCommand=0x%X, SynAsyn=0x%X)", 
        CrtCommand, SynAsyn);

    /* command param element must not be NULL */
    if(LSA_HOST_PTR_ARE_EQUAL(pCmdParam, LSA_NULL))
    {
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdRequestPpm(): FATAL -> pCmdParam is invalid!");
        EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
    }

    switch (CrtCommand)
    {
        /* ---------------------------------------------------------------------------------------- */
        /* PPM_NOP                                                                                  */
        /* ---------------------------------------------------------------------------------------- */
        case EDDP_CRT_CMD_PPM_NOP:
        {
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdRequestPpm(): FATAL -> PPM_NOP not supported!");
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;

        /* ---------------------------------------------------------------------------------------- */
        /* PPM_ON_OFF                                                                               */
        /* ---------------------------------------------------------------------------------------- */
        case EDDP_CRT_CMD_PPM_ON:
        case EDDP_CRT_CMD_PPM_OFF:
        {
            /* -------------------------------------------------------- */
            /* copying of command parameter                             */
            /* -------------------------------------------------------- */
            pCmdParam->CmdOpcode = EDDP_CMD_PPM_ON_OFF;

            // synchron or asynchron ?
            if (SynAsyn == EDDP_CRT_CMD_SYNCHRON)
                pCmdParam->Cbf  = 0;
            else
                pCmdParam->Cbf  = EDDP_CRTCmdDoneCbfPpm;

            // PPM ON or OFF ?
            if (CrtCommand == EDDP_CRT_CMD_PPM_ON)
                pCmdParam->param_union.Crt.Ppm.OnOff.PpmOn = LSA_TRUE;
            else
                pCmdParam->param_union.Crt.Ppm.OnOff.PpmOn = LSA_FALSE;
        }
        break;

        /* ---------------------------------------------------------------------------------------- */

        /* PPM_ACW_IN_OUT                                                                           */
        /* PPM_PACK_ACW_IN_OUT                                                                      */
		/* PPM_PACK_ACW_DG_ON_ACW_IN                                                                */
		/* PPM_PACK_ACW_DG_OFF_ACW_OUT                                                              */
        /* ---------------------------------------------------------------------------------------- */
        case EDDP_CRT_CMD_PPM_ACW_IN:
        case EDDP_CRT_CMD_PPM_ACW_OUT:
        case EDDP_CRT_CMD_PPM_PACK_ACW_IN:
        case EDDP_CRT_CMD_PPM_PACK_ACW_OUT:
        case EDDP_CRT_CMD_PPM_PACK_ACW_DG_ON_ACW_IN:
		case EDDP_CRT_CMD_PPM_PACK_ACW_DG_OFF_ACW_OUT:
		{
            LSA_UINT16                              ProviderId;
            LSA_UINT32                              i;
            LSA_UINT16                              TBaseIndex;
            LSA_UINT16                              AcwIdx;
            LSA_UINT16                              AcwIdxPrev;
            LSA_UINT32                              ApiAdr_NextPointer_Prev_Temp[EDDP_CMD_MAX_ACW_CNT_PER_CMD];
            LSA_UINT32                              ApiAdr_Acw_New_Temp[EDDP_CMD_MAX_ACW_CNT_PER_CMD];
            LSA_UINT32                              ApiAdr_Acw_DG_Next_Temp[EDDP_CMD_MAX_ACW_CNT_PER_CMD];
            EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
            EDDP_CRT_PROVIDER_PTR_TYPE              pProvider;
            EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;
            EDD_COMMON_MEM_U8_PTR_TYPE              Ptr8_Temp = LSA_NULL;
            EDDP_CRT_PROVIDER_TBASE_PTR_TYPE        pTBase;

            pProvMngm       = &pDDB->CRT.Prov;
            ProviderId      = pCmdParam->param_union.Crt.Ppm.ACW_InOut.ProviderId;
            pCmdParam->param_union.Crt.Ppm.ACW_InOut.Acw_Count = 0;

            /* ---------------------------------------------------------------- */
            /* addresses (offsets) of ACWs in APICtrl-RAM of this provider      */
            /* addresses (offsets) of "ACW_X_Ptr"                               */
            /* addresses (offsets) of "ACW.ACW_nextoffset" in prev. ACW         */
            /* ---------------------------------------------------------------- */
            if (pCmdParam->param_union.Crt.Ppm.ACW_InOut.AcwId == EDDP_CRT_CHAIN_IDX_END)
            {
                /* ------------------------------------------------ */
                /* all ACWs of this provider are added/removed      */
                /* ------------------------------------------------ */
                EDDP_ASSERT_FALSE(ProviderId >= pDDB->CRT.CfgParam.ProviderMaxNumber);
                pProvider   = &pProvMngm->pProvEntry[ProviderId];
                i           = 0;

                if ((CrtCommand == EDDP_CRT_CMD_PPM_ACW_IN) || (CrtCommand == EDDP_CRT_CMD_PPM_PACK_ACW_IN))
                    AcwIdx = pProvider->AcwList.BeginIdx;   // ACW_in : from BeginIdx to EndIdx
                else
                    AcwIdx = pProvider->AcwList.EndIdx;     // ACW_out: from EndIdx to BeginIdx

                // we only have EDDP_MAX_ACW_CNT_PER_CMD commands; 2nd condition to check for array boundaries
                while ((AcwIdx != EDDP_CRT_CHAIN_IDX_END) && (i < EDDP_CMD_MAX_ACW_CNT_PER_CMD))
                {
                    EDDP_ASSERT_FALSE(AcwIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

                    pAcw = &pProvMngm->pAcwEntry[AcwIdx];

                    // only if ACW is available in APICtrl-RAM
                    if (pAcw->State & EDDP_CRT_PROV_ACW_STATE_IN_HW_MASK)
                    {
                        AcwIdxPrev  = pAcw->AcwPhaseLink.Prev;
                        TBaseIndex  = EDDP_CRTGetTBaseIndex ( pDDB, 
                                                              pAcw->AddParam.CycleReductionRatio, 
                                                              pAcw->AddParam.CyclePhase);
                        pTBase      = &pDDB->CRT.Prov.pTBaseEntry[TBaseIndex];

                        // as ACW will also be activiated/passivated when added/removed, also update TBaseElement, ACW_Treductionselect and AcwPhaseList
                        if (CrtCommand == EDDP_CRT_CMD_PPM_ACW_IN)
                        {
                            /* -------------------------------------------- */
                            /* ACW_in from provider activate (all ACWs)     */
                            /* -------------------------------------------- */
                            // if this is first ACW in Trog, init TBaseElement
                            EDDP_CRTProvAcwIsFirstAcwAddedToTBase(pDDB, TBaseIndex);
                            // add this ACW to ACW_Treductionselect
                            EDDP_CRTProvAcwAddToTreductionselect(pDDB, AcwIdx);
                            // set R-O-G-Bit if this is first ACW in AcwPhaseList
                            EDDP_CRTProvAcwROGBitAddHwApi(pDDB, AcwIdx);
                        }
                        else if (CrtCommand == EDDP_CRT_CMD_PPM_ACW_OUT)
                        {
                            /* ------------------------------------------------------ */
                            /* ACW_out from provider passivate or remove (all ACWs)   */
                            /* ------------------------------------------------------ */
                            // set this ACW as PASSIVE
                            EDDP_CRTProvAcwSetPassiveRTFrame(pDDB, ProviderId, AcwIdx);
                            // delete R-O-G-Bit if this is last ACW in AcwPhaseList
                            EDDP_CRTProvAcwROGBitRemoveHwApi(pDDB, AcwIdx);
                            // remove this ACW from ACW_Treductionselect
                            EDDP_CRTProvAcwRemoveFromTreductionselect(pDDB, AcwIdx);
                        }

                        if (AcwIdxPrev == EDDP_CRT_CHAIN_IDX_END)
                        {
                            /* -------------------------------------------------------- */
                            /* address of "ACW_X_Ptr"                                   */
                            /* -------------------------------------------------------- */
                            // get HOST address of ACW_X_Ptr
                            switch (pAcw->ROGPhase)
                            {
                                case EDDP_CRT_PROV_ACW_PHASE_G:
                                    Ptr8_Temp = (LSA_UINT8*) &pTBase->pTrogEntry->pHwTrogApi->ACW_G_Ptr;
                                break;

                                case EDDP_CRT_PROV_ACW_PHASE_O:
                                    Ptr8_Temp = (LSA_UINT8*) &pTBase->pTrogEntry->pHwTrogApi->ACW_O_Ptr;
                                break;

                                case EDDP_CRT_PROV_ACW_PHASE_R:
                                    Ptr8_Temp = (LSA_UINT8*) &pTBase->pTrogEntry->pHwTrogApi->ACW_R_Ptr;
                                break;

                                default:
                                {
                                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdRequestPpm(): FATAL -> ROGPhase unknown! ROGPhase=0x%X", 
                                        pAcw->ROGPhase);
                                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                                }
                                break;
                            }
                            
                            if (CrtCommand == EDDP_CRT_CMD_PPM_ACW_OUT)
                            {
                                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCmdRequestPpm(): remove ACW (from TROG: 0x%X), to-be-removed-entry=0x%X", 
                                                  pAcw->ROGPhase, pProvMngm->pAcwEntry[AcwIdx].pHwAcwApi);
                            }
                        }
                        else
                        {
                            /* -------------------------------------------------------- */
                            /* address of "ACW.ACW_nextoffset" in prev. ACW             */
                            /* -------------------------------------------------------- */
                            // get HOST address of ACW_Word1
                            Ptr8_Temp = (LSA_UINT8*) pProvMngm->pAcwEntry[AcwIdxPrev].pHwAcwApi;
                            
                            if (CrtCommand == EDDP_CRT_CMD_PPM_ACW_OUT)
                            {
                                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCmdRequestPpm(): remove ACW, previous-entry=0x%X, to-be-removed-entry=0x%X", 
                                                  pProvMngm->pAcwEntry[AcwIdxPrev].pHwAcwApi, pProvMngm->pAcwEntry[AcwIdx].pHwAcwApi);
                            }
                            
                            // set pointer to address of "ACW_nextoffset" in ACW_Word1
                            Ptr8_Temp += sizeof(LSA_UINT16);
                        }

                        // get shiftet offset of "Ptr8_Temp" in APICtrl-RAM
                        ApiAdr_NextPointer_Prev_Temp[i] = EDDP_CRTGetPnipAddrShift (
                                                                pDDB, 
                                                                (EDD_COMMON_MEM_PTR_TYPE) Ptr8_Temp, 
                                                                EDDP_CRT_SHIFT_VALUE_1);

                        /* -------------------------------------------------------- */
                        /* addresses (offsets) of new ACW in APICtrl-RAM            */
                        /* -------------------------------------------------------- */
                        ApiAdr_Acw_New_Temp[i] = EDDP_CRTGetPnipAddrShift (
                                                        pDDB, 
                                                        (EDD_COMMON_MEM_PTR_TYPE) pAcw->pHwAcwApi, 
                                                        EDDP_CRT_SHIFT_VALUE_3);

                        /* -------------------------------------------------------- */
                        /* addresses (offsets) of next ACW_DG in APICtrl-RAM        */
                        /* -------------------------------------------------------- */
                        ApiAdr_Acw_DG_Next_Temp[i] = 0;

                        /* -------------------------------------------------------- */
                        /* copy to CMD-Interface                                    */
                        /* -------------------------------------------------------- */
                        pCmdParam->param_union.Crt.Ppm.ACW_InOut.ApiAdr_NextPointer_Prev[i]     = ApiAdr_NextPointer_Prev_Temp[i];
                        pCmdParam->param_union.Crt.Ppm.ACW_InOut.ApiAdr_Acw_New[i]              = ApiAdr_Acw_New_Temp[i];
                        pCmdParam->param_union.Crt.Ppm.ACW_InOut.ApiAdr_Acw_DG_Next[i]          = ApiAdr_Acw_DG_Next_Temp[i];
                        
                        
                        i++;
                        
                        /* remove software trog chaining for this provider/ACW */
                        /* NOTE: this can be done already here before command confirmation because */ 
                        /*       we assume that a command always returns with success, otherwise   */
                        /*       we  throw a fatal                                                 */
                        /* NOTE: ACW is cleaned after command confirmation                         */
                        if (CrtCommand == EDDP_CRT_CMD_PPM_ACW_OUT)
                        {
                            LSA_UINT16                                  _TBaseIndex;
                            EDDP_CRT_PROVIDER_ACW_PTR_TYPE              _pAcw;
                            
                            _pAcw        = &pProvMngm->pAcwEntry[AcwIdx];
                            _TBaseIndex  = EDDP_CRTGetTBaseIndex ( pDDB,
                                                                  _pAcw->AddParam.CycleReductionRatio,
                                                                  _pAcw->AddParam.CyclePhase);
                            // remove ACW from TBase tree
                            EDDP_CRTProvAcwRemoveFromTBase (pDDB, AcwIdx, LSA_FALSE);
                            // if this is last ACW in Trog, delete Trog/TBase
                            EDDP_CRTProvAcwIsLastAcwRemovedFromTBase (pDDB, _TBaseIndex);
                        }
                    }
                    
                    if ((CrtCommand == EDDP_CRT_CMD_PPM_ACW_IN) || (CrtCommand == EDDP_CRT_CMD_PPM_PACK_ACW_IN))
                        AcwIdx = pAcw->AcwProvLink.Next;    // ACW_in : next AcwIdx in this provider
                    else
                        AcwIdx = pAcw->AcwProvLink.Prev;    // ACW_out: prev AcwIdx in this provider
                }

                // check after increment of i if array boundaries would have been touched
                if (i > EDDP_CMD_MAX_ACW_CNT_PER_CMD)
                {
                    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdRequestPpm(): FATAL -> Number of ACWs used for this command is invalid! number=%d >= max=%d",
                        i, EDDP_CMD_MAX_ACW_CNT_PER_CMD);
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }
                else
                {
                    pCmdParam->param_union.Crt.Ppm.ACW_InOut.Acw_Count = i;
                }
                
            }
            else
            {
                /* ------------------------------------------------ */
                /* only one ACW of this provider is added/removed   */
                /* ------------------------------------------------ */
                AcwIdx      = pCmdParam->param_union.Crt.Ppm.ACW_InOut.AcwId;
                EDDP_ASSERT_FALSE(AcwIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);
                pAcw        = &pProvMngm->pAcwEntry[AcwIdx];
                AcwIdxPrev  = pAcw->AcwPhaseLink.Prev;
                TBaseIndex  = EDDP_CRTGetTBaseIndex ( pDDB, 
                                                      pAcw->AddParam.CycleReductionRatio, 
                                                      pAcw->AddParam.CyclePhase);
                pTBase      = &pDDB->CRT.Prov.pTBaseEntry[TBaseIndex];

                // as ACW will also be activiated / passivated when added / removed, also update TBaseElement, ACW_Treductionselect and AcwPhaseList
                if ((CrtCommand == EDDP_CRT_CMD_PPM_ACW_IN) || (CrtCommand == EDDP_CRT_CMD_PPM_PACK_ACW_IN) || (CrtCommand == EDDP_CRT_CMD_PPM_PACK_ACW_DG_ON_ACW_IN))
                {
                    /* -------------------------------------------- */
                    /* ACW_in from provider activate (one ACW)      */
                    /* -------------------------------------------- */
                    // if this is first ACW in Trog, init TBaseElement
                    EDDP_CRTProvAcwIsFirstAcwAddedToTBase(pDDB, TBaseIndex);
                    // add this ACW to ACW_Treductionselect
                    EDDP_CRTProvAcwAddToTreductionselect(pDDB, AcwIdx);
                    // set R-O-G-Bit if this is first ACW in AcwPhaseList
                    EDDP_CRTProvAcwROGBitAddHwApi(pDDB, AcwIdx);
                }
                else if (CrtCommand == EDDP_CRT_CMD_PPM_ACW_OUT)
                {
                    /* -------------------------------------------- */
                    /* ACW_out from provider passivate (one ACW)    */
                    /* -------------------------------------------- */
                    // set this ACW as PASSIVE
                    EDDP_CRTProvAcwSetPassiveRTFrame(pDDB, ProviderId, AcwIdx);
                    // delete R-O-G-Bit if this is last ACW in AcwPhaseList
                    EDDP_CRTProvAcwROGBitRemoveHwApi(pDDB, AcwIdx);
                    // remove this ACW from ACW_Treductionselect
                    EDDP_CRTProvAcwRemoveFromTreductionselect(pDDB, AcwIdx);
                }

                if (AcwIdxPrev == EDDP_CRT_CHAIN_IDX_END)
                {
                    /* -------------------------------------------------------- */
                    /* address of "ACW_X_Ptr"                                   */
                    /* -------------------------------------------------------- */
                    // get HOST address of ACW_X_Ptr
                    switch (pAcw->ROGPhase)
                    {
                        case EDDP_CRT_PROV_ACW_PHASE_G:
                            Ptr8_Temp = (LSA_UINT8*) &pTBase->pTrogEntry->pHwTrogApi->ACW_G_Ptr;
                        break;

                        case EDDP_CRT_PROV_ACW_PHASE_O:
                            Ptr8_Temp = (LSA_UINT8*) &pTBase->pTrogEntry->pHwTrogApi->ACW_O_Ptr;
                        break;

                        case EDDP_CRT_PROV_ACW_PHASE_R:
                            Ptr8_Temp = (LSA_UINT8*) &pTBase->pTrogEntry->pHwTrogApi->ACW_R_Ptr;
                        break;

                        default:
                        {
                            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdRequestPpm(): FATAL -> ROGPhase unknown! ROGPhase=0x%X", 
                                pAcw->ROGPhase);
                            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                        }
                        break;
                    }
                    
                    if (CrtCommand == EDDP_CRT_CMD_PPM_ACW_OUT)
                    {
                        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCmdRequestPpm(): remove ACW (from TROG: 0x%X), to-be-removed-entry=0x%X", 
                                          pAcw->ROGPhase, pProvMngm->pAcwEntry[AcwIdx].pHwAcwApi);
                    }
                }
                else
                {
                    /* -------------------------------------------------------- */
                    /* address of "ACW.ACW_nextoffset" in prev. ACW             */
                    /* -------------------------------------------------------- */
                    // get HOST address of ACW_Word1
                    Ptr8_Temp = (LSA_UINT8*) pProvMngm->pAcwEntry[AcwIdxPrev].pHwAcwApi;
                    
                    if (CrtCommand == EDDP_CRT_CMD_PPM_ACW_OUT)
                    {
                        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCmdRequestPpm(): remove ACW, previous-entry=0x%X, to-be-removed-entry=0x%X", 
                                          pProvMngm->pAcwEntry[AcwIdxPrev].pHwAcwApi, pProvMngm->pAcwEntry[AcwIdx].pHwAcwApi);
                    }
                    
                    // set pointer to address of "ACW_nextoffset" in ACW_Word1
                    Ptr8_Temp += sizeof(LSA_UINT16);
                }

                // get shiftet offset of "Ptr8_Temp" in APICtrl-RAM
                ApiAdr_NextPointer_Prev_Temp[0] = EDDP_CRTGetPnipAddrShift (
                                                        pDDB, 
                                                        (EDD_COMMON_MEM_PTR_TYPE) Ptr8_Temp, 
                                                        EDDP_CRT_SHIFT_VALUE_1);

                /* -------------------------------------------------------- */
                /* addresses (offsets) of new ACW in APICtrl-RAM            */
                /* -------------------------------------------------------- */
                ApiAdr_Acw_New_Temp[0] = EDDP_CRTGetPnipAddrShift (
                                                pDDB, 
                                                (EDD_COMMON_MEM_PTR_TYPE) pAcw->pHwAcwApi, 
                                                EDDP_CRT_SHIFT_VALUE_3);

                /* -------------------------------------------------------- */
                /* copy to CMD-Interface                                    */
                /* -------------------------------------------------------- */
                pCmdParam->param_union.Crt.Ppm.ACW_InOut.ApiAdr_NextPointer_Prev[0]     = ApiAdr_NextPointer_Prev_Temp[0];
                pCmdParam->param_union.Crt.Ppm.ACW_InOut.ApiAdr_Acw_New[0]              = ApiAdr_Acw_New_Temp[0];
                pCmdParam->param_union.Crt.Ppm.ACW_InOut.ApiAdr_Acw_DG_Next[0]          = 0;
                pCmdParam->param_union.Crt.Ppm.ACW_InOut.Acw_Count                      = 1;
                
                
                /* remove software trog chaining for this provider/ACW */
                /* NOTE: this can be done already here before command confirmation because */ 
                /*       we assume that a command always returns with success, otherwise   */
                /*       we  throw a fatal                                                 */
                /* NOTE: ACW is cleaned after command confirmation                         */
                if ((CrtCommand == EDDP_CRT_CMD_PPM_ACW_OUT) || (CrtCommand == EDDP_CRT_CMD_PPM_PACK_ACW_OUT) || (CrtCommand == EDDP_CRT_CMD_PPM_PACK_ACW_DG_OFF_ACW_OUT))
                {
                    LSA_UINT16                                  _TBaseIndex;
                    EDDP_CRT_PROVIDER_ACW_PTR_TYPE              _pAcw;
                    
                    EDDP_ASSERT_FALSE(AcwIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);
                    _pAcw        = &pProvMngm->pAcwEntry[AcwIdx];
                    _TBaseIndex  = EDDP_CRTGetTBaseIndex ( pDDB,
                                                          _pAcw->AddParam.CycleReductionRatio,
                                                          _pAcw->AddParam.CyclePhase);
                    
                    if (CrtCommand == EDDP_CRT_CMD_PPM_PACK_ACW_OUT)
                    {
                        // set this pack ACW as PASSIVE (delete PortInfo of this ACW)
                        EDDP_CRTProvAcwSetPassivePackFrame (pDDB, AcwIdx, LSA_TRUE);
                    }
                    else if (CrtCommand == EDDP_CRT_CMD_PPM_PACK_ACW_DG_OFF_ACW_OUT)
                    {
                        // set this pack ACW as PASSIVE (do not delete AddParam of this ACW, as Controller can be reactivated)
                        EDDP_CRTProvAcwSetPassivePackFrame (pDDB, AcwIdx, LSA_FALSE);
                    }
                    
                    // remove ACW from TBase tree
                    EDDP_CRTProvAcwRemoveFromTBase (pDDB, AcwIdx, LSA_FALSE);
                    // if this is last ACW in Trog, delete Trog/TBase
                    EDDP_CRTProvAcwIsLastAcwRemovedFromTBase (pDDB, _TBaseIndex);
                }
            }

            if (pCmdParam->param_union.Crt.Ppm.ACW_InOut.Acw_Count == 0)
            {
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdRequestPpm(): FATAL -> ACW_InOut.Acw_Count=0!");
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            else
            {
                /* -------------------------------------------------------- */
                /* copying of command parameter                             */
                /* -------------------------------------------------------- */
                pCmdParam->CmdOpcode = EDDP_CMD_PPM_ACW_IN_OUT;

                // synchron or asynchron ?
                if (SynAsyn == EDDP_CRT_CMD_SYNCHRON)
                {
                    pCmdParam->Cbf = 0;
                }
                else
                {
                    // which CallBackFunction must used ?
                    switch (CrtCommand)
                    {
                        case EDDP_CRT_CMD_PPM_ACW_IN:
                        case EDDP_CRT_CMD_PPM_ACW_OUT:
                        {
                            pCmdParam->Cbf = EDDP_CRTCmdDoneCbfPpm;

                            // ACW IN or OUT ?
                            if (CrtCommand == EDDP_CRT_CMD_PPM_ACW_IN)
                                pCmdParam->param_union.Crt.Ppm.ACW_InOut.Acw_In = LSA_TRUE;
                            else
                                pCmdParam->param_union.Crt.Ppm.ACW_InOut.Acw_In = LSA_FALSE;
                        }
                        break;

                        case EDDP_CRT_CMD_PPM_PACK_ACW_IN:
                        case EDDP_CRT_CMD_PPM_PACK_ACW_OUT:
                        {
                            pCmdParam->Cbf = EDDP_CRTCmdDoneCbfPpmPackGroup;

                            // ACW IN or OUT ?
                            if (CrtCommand == EDDP_CRT_CMD_PPM_PACK_ACW_IN)
                                pCmdParam->param_union.Crt.Ppm.ACW_InOut.Acw_In = LSA_TRUE;
                            else
                                pCmdParam->param_union.Crt.Ppm.ACW_InOut.Acw_In = LSA_FALSE;
                        }
                        break;

                        case EDDP_CRT_CMD_PPM_PACK_ACW_DG_ON_ACW_IN:
						case EDDP_CRT_CMD_PPM_PACK_ACW_DG_OFF_ACW_OUT:
                        {
                            pCmdParam->Cbf = EDDP_CRTCmdDoneCbfPpmAcwDG;

                            // ACW IN or OUT ?
                            if (CrtCommand == EDDP_CRT_CMD_PPM_PACK_ACW_DG_ON_ACW_IN)
                                pCmdParam->param_union.Crt.Ppm.ACW_InOut.Acw_In = LSA_TRUE;
                            else
                                pCmdParam->param_union.Crt.Ppm.ACW_InOut.Acw_In = LSA_FALSE;
                        }
                        break;

                        default:
                        {
                            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdRequestPpm(): FATAL -> CrtCommand(0x%X) is not allowed!", 
                                CrtCommand);
                            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                        }
                        break;
                    }
                }

                pCmdParam->param_union.Crt.Ppm.ACW_InOut.Acw_DG = LSA_FALSE;
            }
        }
        break;

        /* ---------------------------------------------------------------------------------------- */
        /* PPM_PACK_ACW_DG_ON_OFF                                                                   */
        /* ---------------------------------------------------------------------------------------- */
        case EDDP_CRT_CMD_PPM_PACK_ACW_DG_ON:
        case EDDP_CRT_CMD_PPM_PACK_ACW_DG_OFF:
        {
            LSA_UINT32                              ApiAdr_Acw_Temp[1];
            EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
            EDDP_CRT_PROVIDER_ACW_DG_PTR_TYPE       pAcwDG;

            pProvMngm    = &pDDB->CRT.Prov;
            pCmdParam->param_union.Crt.Ppm.ACW_OnOff.Acw_Count = 0;
            // AcwDGId is stored in AcwId
            EDDP_ASSERT_FALSE(pCmdParam->param_union.Crt.Ppm.ACW_OnOff.AcwId >= pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber);
            pAcwDG          = &pProvMngm->pAcwDGEntry[pCmdParam->param_union.Crt.Ppm.ACW_OnOff.AcwId];

            // only if ACW_DG is available in APICtrl-RAM
            if (pAcwDG->State & EDDP_CRT_PROV_ACW_STATE_IN_HW_MASK)
            {
                ApiAdr_Acw_Temp[0] = EDDP_CRTGetPnipAddrShift (
                                        pDDB, 
                                        (EDD_COMMON_MEM_PTR_TYPE) pAcwDG->pHwAcwDGApi, 
                                        EDDP_CRT_SHIFT_VALUE_3);
                // copy to CMD-Interface
                pCmdParam->param_union.Crt.Ppm.ACW_OnOff.ApiAdr_Acw[0]  = ApiAdr_Acw_Temp[0];
                pCmdParam->param_union.Crt.Ppm.ACW_OnOff.Acw_Count      = 1;
            }

            if (pCmdParam->param_union.Crt.Ppm.ACW_OnOff.Acw_Count == 0)
            {
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdRequestPpm(): FATAL -> ACW_DG_OnOff.Acw_Count=0!");
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            else
            {
                /* ---------------------------------------------------------------- */
                /* copying of command parameter                                     */
                /* ---------------------------------------------------------------- */
                pCmdParam->CmdOpcode = EDDP_CMD_PPM_ACW_ON_OFF;

                // synchron or asynchron ?
                if (SynAsyn == EDDP_CRT_CMD_SYNCHRON)
                    pCmdParam->Cbf = 0;
                else
                    pCmdParam->Cbf = EDDP_CRTCmdDoneCbfPpmAcwDG;

                // ACW_DG_ON or OFF ?
                if (CrtCommand == EDDP_CRT_CMD_PPM_PACK_ACW_DG_ON)
                    pCmdParam->param_union.Crt.Ppm.ACW_OnOff.Acw_On = LSA_TRUE;
                else
                    pCmdParam->param_union.Crt.Ppm.ACW_OnOff.Acw_On = LSA_FALSE;

                pCmdParam->param_union.Crt.Ppm.ACW_OnOff.Acw_DG     = LSA_TRUE;
                pCmdParam->param_union.Crt.Ppm.ACW_OnOff.FirstSend  = LSA_FALSE;
            }
        }
        break;

        /* ---------------------------------------------------------------------------------------- */
        /* PPM_ACW_MOVE                                                                             */
        /* ---------------------------------------------------------------------------------------- */
        case EDDP_CRT_CMD_PPM_ACW_MOVE:
        {
            LSA_UINT16                              ProviderId;
            LSA_UINT16                              NewCyclePhase;
            LSA_UINT32                              NewCyclePhaseSequence;
            LSA_UINT32                              ApiAdr_NextPointer_New_Prev_Temp;
            LSA_UINT32                              ApiAdr_NextPointer_Old_Prev_Temp;
            LSA_UINT32                              ApiAdr_Acw_Temp;
            LSA_UINT16                              AcwIdPrev;
            LSA_UINT16                              TBaseIndex;
            EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
        //  EDDP_CRT_PROVIDER_DDB_MNGM_PTR_TYPE     pProvMngm;
            EDDP_CRT_PROVIDER_PTR_TYPE              pProvider;
            EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;
            EDD_COMMON_MEM_U8_PTR_TYPE              Ptr8_Temp = LSA_NULL;
            EDDP_CRT_PROVIDER_TBASE_PTR_TYPE        pTBase;

            pProvMngm               = &pDDB->CRT.Prov;
            pProvMngm               = &pDDB->CRT.Prov;
            ProviderId              = pCmdParam->param_union.Crt.Ppm.ACW_Move.ProviderId;
            EDDP_ASSERT_FALSE(ProviderId >= pDDB->CRT.CfgParam.ProviderMaxNumber);
            pProvider               = &pProvMngm->pProvEntry[ProviderId];
            EDDP_ASSERT_FALSE(pProvider->AcwList.BeginIdx == EDDP_CRT_CHAIN_IDX_END);
            pAcw                    = &pProvMngm->pAcwEntry[pProvider->AcwList.BeginIdx];
            NewCyclePhase           = pCmdParam->param_union.Crt.Ppm.ACW_Move.CyclePhase;
            NewCyclePhaseSequence   = pCmdParam->param_union.Crt.Ppm.ACW_Move.CyclePhaseSequence;

            /* ---------------------------------------------------------------- */
            /* address (offset) of ACW which must be removed                    */
            /* ---------------------------------------------------------------- */
            ApiAdr_Acw_Temp = EDDP_CRTGetPnipAddrShift (
                                    pDDB, 
                                    (EDD_COMMON_MEM_PTR_TYPE) pAcw->pHwAcwApi, 
                                    EDDP_CRT_SHIFT_VALUE_3);

            /* ---------------------------------------------------------------- */
            /* Old-AcwPhaseList     -> ApiAdr_NextPointer_Old_Prev_Temp         */
            /* ---------------------------------------------------------------- */
            AcwIdPrev   = pAcw->AcwPhaseLink.Prev;
            TBaseIndex  = EDDP_CRTGetTBaseIndex ( pDDB, 
                                                  pAcw->AddParam.CycleReductionRatio, 
                                                  pAcw->AddParam.CyclePhase);
            pTBase      = &pProvMngm->pTBaseEntry[TBaseIndex];

            if (AcwIdPrev == EDDP_CRT_CHAIN_IDX_END)
            {
                /* -------------------------------------------------------- */
                /* address of "ACW_X_Ptr"                                   */
                /* -------------------------------------------------------- */
                // get HOST address of ACW_X_Ptr
                switch (pAcw->ROGPhase)
                {
                    case EDDP_CRT_PROV_ACW_PHASE_G:
                        Ptr8_Temp = (LSA_UINT8*) &pTBase->pTrogEntry->pHwTrogApi->ACW_G_Ptr;
                    break;

                    case EDDP_CRT_PROV_ACW_PHASE_O:
                        Ptr8_Temp = (LSA_UINT8*) &pTBase->pTrogEntry->pHwTrogApi->ACW_O_Ptr;
                    break;

                    case EDDP_CRT_PROV_ACW_PHASE_R:
                    default:
                    {
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdRequestPpm(): FATAL -> ROGPhase unsupported! ROGPhase=0x%X", 
                            pAcw->ROGPhase);
                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                    }
                    break;
                }
            }
            else
            {
                /* -------------------------------------------------------- */
                /* address of "ACW.ACW_nextoffset" in prev. ACW             */
                /* -------------------------------------------------------- */
                // get HOST address of ACW_Word1
                Ptr8_Temp = (LSA_UINT8*) pProvMngm->pAcwEntry[AcwIdPrev].pHwAcwApi;
                // set pointer to address of "ACW_nextoffset" in ACW_Word1
                Ptr8_Temp += sizeof(LSA_UINT16);
            }

            // get shiftet offset of "Ptr8_Temp" in APICtrl-RAM
            ApiAdr_NextPointer_Old_Prev_Temp = EDDP_CRTGetPnipAddrShift (
                                                    pDDB, 
                                                    (EDD_COMMON_MEM_PTR_TYPE) Ptr8_Temp, 
                                                    EDDP_CRT_SHIFT_VALUE_1);

            /* ------------------------------------ */
            /* save old parameter                   */
            /* ------------------------------------ */
            pCmdParam->param_union.Crt.Ppm.ACW_Move.CyclePhase          = pAcw->AddParam.CyclePhase;
            pCmdParam->param_union.Crt.Ppm.ACW_Move.CyclePhaseSequence  = pAcw->AddParam.CyclePhaseSequence;

            /* ------------------------------------ */
            /* remove ACW from Old-AcwPhaseList     */
            /* ------------------------------------ */
            EDDP_CRTProvAcwRemoveFromTBase (pDDB, pAcw->AcwId, LSA_FALSE);

            /* ---------------------------------------------------------------- */
            /* add to New-AcwPhaseList     -> ApiAdr_NextPointer_New_Prev_Temp  */
            /* ---------------------------------------------------------------- */
            // change parameter in CRT
            pAcw->AddParam.CyclePhase           = NewCyclePhase;
            pAcw->AddParam.CyclePhaseSequence   = NewCyclePhaseSequence;
            // add this ACW to New-AcwPhaseList
            EDDP_CRTProvAcwAddToTBase (pDDB, pAcw->AcwId, LSA_FALSE);

            TBaseIndex  = EDDP_CRTGetTBaseIndex ( pDDB, 
                                                  pAcw->AddParam.CycleReductionRatio, 
                                                  pAcw->AddParam.CyclePhase);
            pTBase      = &pProvMngm->pTBaseEntry[TBaseIndex];

            // if this is first ACW in Trog, init TBase
            EDDP_CRTProvAcwIsFirstAcwAddedToTBase (pDDB, TBaseIndex);
            // set R-O-G-Bit if this is first ACW in AcwPhaseList
            EDDP_CRTProvAcwROGBitAddHwApi (pDDB, pAcw->AcwId);

            AcwIdPrev = pAcw->AcwPhaseLink.Prev;

            if (AcwIdPrev == EDDP_CRT_CHAIN_IDX_END)
            {
                /* -------------------------------------------------------- */
                /* address of "ACW_X_Ptr"                                   */
                /* -------------------------------------------------------- */
                // get HOST address of ACW_X_Ptr
                switch (pAcw->ROGPhase)
                {
                    case EDDP_CRT_PROV_ACW_PHASE_G:
                        Ptr8_Temp = (LSA_UINT8*) &pTBase->pTrogEntry->pHwTrogApi->ACW_G_Ptr;
                    break;

                    case EDDP_CRT_PROV_ACW_PHASE_O:
                        Ptr8_Temp = (LSA_UINT8*) &pTBase->pTrogEntry->pHwTrogApi->ACW_O_Ptr;
                    break;

                    case EDDP_CRT_PROV_ACW_PHASE_R:
                    default:
                    {
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdRequestPpm(): FATAL -> ROGPhase unsupported! ROGPhase=0x%X", 
                            pAcw->ROGPhase);
                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                    }
                    break;
                }
            }
            else
            {
                /* -------------------------------------------------------- */
                /* address of "ACW.ACW_nextoffset" in prev. ACW             */
                /* -------------------------------------------------------- */
                // get HOST address of ACW_Word1
                Ptr8_Temp = (LSA_UINT8*) pProvMngm->pAcwEntry[AcwIdPrev].pHwAcwApi;
                // set pointer to address of "ACW_nextoffset" in ACW_Word1
                Ptr8_Temp += sizeof(LSA_UINT16);
            }

            // get shiftet offset of "Ptr8_Temp" in APICtrl-RAM
            ApiAdr_NextPointer_New_Prev_Temp = EDDP_CRTGetPnipAddrShift (
                                                    pDDB, 
                                                    (EDD_COMMON_MEM_PTR_TYPE) Ptr8_Temp, 
                                                    EDDP_CRT_SHIFT_VALUE_1);

            /* -------------------------------------------------------- */
            /* copying of command parameter                             */
            /* -------------------------------------------------------- */
            pCmdParam->CmdOpcode = EDDP_CMD_PPM_ACW_MOVE;

            // synchron or asynchron ?
            if (SynAsyn == EDDP_CRT_CMD_SYNCHRON)
                pCmdParam->Cbf  = 0;
            else
                pCmdParam->Cbf  = EDDP_CRTCmdDoneCbfPpm;

            pCmdParam->param_union.Crt.Ppm.ACW_Move.ApiAdr_NextPointer_New_Prev     = ApiAdr_NextPointer_New_Prev_Temp;
            pCmdParam->param_union.Crt.Ppm.ACW_Move.ApiAdr_NextPointer_Old_Prev     = ApiAdr_NextPointer_Old_Prev_Temp;
            pCmdParam->param_union.Crt.Ppm.ACW_Move.ApiAdr_Acw                      = ApiAdr_Acw_Temp;
            
        }
        break;

        /* ---------------------------------------------------------------------------------------- */
        /* PPM_ACW_XCHANGE_BUFF                                                                     */
        /* ---------------------------------------------------------------------------------------- */
        case EDDP_CRT_CMD_PPM_ACW_XCHANGE_BUFF:
        {
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdRequestPpm(): FATAL -> PPM_ACW_XCHANGE_BUFF not supported!");
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;

        /* ---------------------------------------------------------------------------------------- */
        /* PPM_Starttime: command "Update_BASECYCLE"                                                */
        /* ---------------------------------------------------------------------------------------- */
        case EDDP_CRT_CMD_PPM_STARTTIME:
        {
            /* -------------------------------------------------------- */
            /* copying of command parameter                             */
            /* -------------------------------------------------------- */
            pCmdParam->CmdOpcode = EDDP_CMD_GSYNC_UPDATE_BASECYCL;

            // synchron or asynchron ?
            if (SynAsyn == EDDP_CRT_CMD_SYNCHRON)
                pCmdParam->Cbf  = 0;
            else
                pCmdParam->Cbf  = EDDP_CRTCmdDoneCbfPpm;

            switch (EDDP_HALGetPnipRevision_Ext(pDDB))
            {
                #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
                case EDDP_HAL_HW_PNIP_R1A1:
                {
                    pCmdParam->param_union.Gsync.UpdateBaseCycl.PPM_Starttime_Ns = EDDP_CRT_PPM_STARTTIME_ERTEC200P_PNIP_REV1;
                }
                break;

                case EDDP_HAL_HW_PNIP_R1F1:
                {
                    pCmdParam->param_union.Gsync.UpdateBaseCycl.PPM_Starttime_Ns = EDDP_CRT_PPM_STARTTIME_FPGA_PNIP_REV1;
                }
                break;
                #endif


                #ifdef EDDP_CFG_HW_PNIP_REV2_SUPPORT
                case EDDP_HAL_HW_PNIP_R2A1:
                {
                    pCmdParam->param_union.Gsync.UpdateBaseCycl.PPM_Starttime_Ns = EDDP_CRT_PPM_STARTTIME_ERTEC200P_PNIP_REV2;
                }
                break;

                case EDDP_HAL_HW_PNIP_R2F1:
                {
                    pCmdParam->param_union.Gsync.UpdateBaseCycl.PPM_Starttime_Ns = EDDP_CRT_PPM_STARTTIME_FPGA_PNIP_REV2;
                }
                break;
                #endif

                default:
                {
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdRequestPpm(): FATAL -> PnipRevision(%d) is unknown", 
                        EDDP_HALGetPnipRevision_Ext(pDDB));
                    EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                }
                break;
            }

            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCmdRequestPpm(): set PPM_Starttime to %d ns", 
                pCmdParam->param_union.Gsync.UpdateBaseCycl.PPM_Starttime_Ns);
        }
        break;

        /* ---------------------------------------------------------------------------------------- */
        /* unknown PPM command                                                                      */
        /* ---------------------------------------------------------------------------------------- */
        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdRequestPpm(): FATAL -> CrtCommand(0x%X) unknown", 
                CrtCommand);
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }

    /* -------------------------------------------------------- */
    /* call CMD-Interface                                       */
    /* -------------------------------------------------------- */
    ResponseCmd = EDDP_CmdRequest (pDDB, pCmdParam);

    // check Response of CMD-Interface
    if (SynAsyn == EDDP_CRT_CMD_SYNCHRON)
    {
        // synchron, Response must be EDD_STS_OK
        if (ResponseCmd != EDD_STS_OK)
        {
            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdRequestPpm(): FATAL -> ResponseCmd(0x%X) of EDDP_CmdRequest() is not OK! SynAsyn=%d",
                ResponseCmd, SynAsyn);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
    }
    else
    {
        // asynchron, Response must be EDD_STS_OK_PENDING
        if (ResponseCmd != EDD_STS_OK_PENDING)
        {
            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdRequestPpm(): FATAL -> ResponseCmd(0x%X) of EDDP_CmdRequest() is not OK_PENDING! SynAsyn=%d",
                ResponseCmd, SynAsyn);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
    }

    return (ResponseCmd);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTCmdRequestCpm                                       +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB                +*/
/*+                             LSA_UINT16                              CrtCommand          +*/
/*+                             LSA_UINT16                              AcwId               +*/
/*+                             EDDP_CMD_CBF_PTR_TYPE                   pCbPtr              +*/
/*+                                                                                         +*/
/*+  Result                :    EDD_RSP                                                     +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  CrtCommand         : CRT-Command                                                       +*/
/*+  AcwId              : AcwId (Consumer or Container)                                     +*/
/*+  pCbPtr             : synchron (LSA_NULL) or asynchron (valid callback pointer)         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Central collect function for calling of Command Interface                 +*/
/*+               for all CPM commands.                                                     +*/
/*+                                                                                         +*/
/*+               CRT commands are defined in EDDP_CRT_CMD_PARAM_TYPE.                      +*/
/*+                                                                                         +*/
/*+               Fill command parameter and call command interface                         +*/
/*+               If Response of CMD-IF not OK, CMD-IF has not accepted the command         +*/
/*+               -> FATAL                                                                  +*/
/*+                                                                                         +*/
/*+               If synchron, no Cbf-function available and command is finished            +*/
/*+               after calling of command interface.                                       +*/
/*+                                                                                         +*/
/*+               If asynchron, command is finished in the Cbf-function that                +*/
/*+               is called by command interface.                                           +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP  EDDP_LOCAL_FCT_ATTR EDDP_CRTCmdRequestCpm(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  CrtCommand,
    LSA_UINT16                  AcwId,
    EDDP_CMD_CBF_PTR_TYPE       pCbPtr)
{
    EDD_RSP                         ResponseCmd;
    EDDP_CMD_PARAM_PTR_TYPE         pCmdParam;

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTCmdRequestCpm(CrtCommand=0x%X, pCbPtr=0x%X)", 
        CrtCommand, pCbPtr);

    pCmdParam   = LSA_NULL;
    ResponseCmd = EDD_STS_ERR_PARAM;

    switch (CrtCommand)
    {
        /* ---------------------------------------------------------------------------------------- */
        /* CPM_NOP                                                                                  */
        /* ---------------------------------------------------------------------------------------- */
        case EDDP_CRT_CMD_CPM_NOP:
        {
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdRequestCpm(): FATAL -> CPM_NOP not supported!"); 
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;

        /* ---------------------------------------------------------------------------------------- */
        /* CPM_ON_OFF																				*/
        /* ---------------------------------------------------------------------------------------- */
        case EDDP_CRT_CMD_CPM_ON:
        case EDDP_CRT_CMD_CPM_OFF:
        {
            /* -------------------------------------------------------- */
            /* copying of command parameter                             */
            /* -------------------------------------------------------- */
            pCmdParam = EDDP_CRTCmdGetInternalCmdParamElement(pDDB);
            pCmdParam->CmdOpcode = EDDP_CMD_CPM_ON_OFF;

            // CPM ON or OFF ?
            if (CrtCommand == EDDP_CRT_CMD_CPM_ON)
                pCmdParam->param_union.Crt.Cpm.OnOff.CpmOn  = LSA_TRUE;
            else
                pCmdParam->param_union.Crt.Cpm.OnOff.CpmOn  = LSA_FALSE;
        }
        break;

        /* ---------------------------------------------------------------------------------------- */
        /* CPM_ACW_ON_OFF                                                                           */
        /* ---------------------------------------------------------------------------------------- */
        case EDDP_CRT_CMD_CPM_ACW_ON:
        case EDDP_CRT_CMD_CPM_ACW_OFF:
        {
            EDDP_CRT_CONSUMER_MNGM_PTR_TYPE     pConsDDB;
            LSA_BOOL                            Acw_DG;
            LSA_UINT16                          DgIdx;
            LSA_UINT32                          ApiAdr_Acw_Temp;
            EDD_COMMON_MEM_PTR_TYPE             ApiVirtualAdr;
            EDDP_CRT_CONS_ACW_INFO_PTR_PTR_TYPE pCommonAcwPtr;

            pCmdParam = EDDP_CRTCmdGetInternalCmdParamElement(pDDB);
            pConsDDB = &pDDB->CRT.Cons;
            pCommonAcwPtr = pDDB->CRT.Cons.pAcwPtrArray;  // pointer array holds to different structs

            Acw_DG = LSA_FALSE;
            if ( AcwId < pDDB->CRT.CfgParam.ConsumerMaxNumber )
            {   // ***** normal ACW or ACW_DG ?
                if (pConsDDB->pConsEntry[AcwId].ConsumerFlags & EDDP_CONS_FLAG_IS_DFP_ITEM)
                {
                    Acw_DG = LSA_TRUE;  
                }
            } // else: container acw

            if (Acw_DG)
            {
                // we need here the location of the ACW_DG (idx mapping to pConsDgEntry[])
                DgIdx = pConsDDB->pConsEntry[AcwId].DgIdx;
                if (DgIdx == EDDP_CRT_CHAIN_IDX_END)
                {
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdRequestCpm(): FATAL -> ConsumerId=%d holds invalid DgIdx !",
                        AcwId);
                    EDDP_FATAL(EDDP_FATAL_ERR_SERVICE);
                }
                ApiVirtualAdr =  (EDD_COMMON_MEM_PTR_TYPE) pConsDDB->pConsDgEntry[DgIdx].pHwAcwDgApi;
            }
            else
            {   // ***** we need here the location of the ACW (non DFP or container ACW)
                ApiVirtualAdr =  (EDD_COMMON_MEM_PTR_TYPE) pCommonAcwPtr[AcwId]->pHwAcwApi;
            }
            /* -------------------------------------------------------- */
            /* address (offset) of ACW in APICtrl-RAM                   */
            /* -------------------------------------------------------- */
            ApiAdr_Acw_Temp = EDDP_CRTGetPnipAddrShift (
                                    pDDB, 
                                    ApiVirtualAdr,
                                    EDDP_CRT_SHIFT_VALUE_3);

            /* -------------------------------------------------------- */
            /* copying of command parameter                             */
            /* -------------------------------------------------------- */
            pCmdParam->CmdOpcode = EDDP_CMD_CPM_ACW_ON_OFF;

            // ACW ON or OFF ?
            if (CrtCommand == EDDP_CRT_CMD_CPM_ACW_ON)
            {    pCmdParam->param_union.Crt.Cpm.ACW_OnOff.Acw_On    = LSA_TRUE;    }
            else
            {    pCmdParam->param_union.Crt.Cpm.ACW_OnOff.Acw_On    = LSA_FALSE;   }
            pCmdParam->param_union.Crt.Cpm.ACW_OnOff.Acw_DG         = Acw_DG;
            //pCmdParam->param_union.Cpm.ACW_OnOff.Set_To_Unknown   <-- set externally !!!
            pCmdParam->param_union.Crt.Cpm.ACW_OnOff.ApiAdr_Acw     = ApiAdr_Acw_Temp;
            pCmdParam->param_union.Crt.Cpm.ACW_OnOff.AcwID = AcwId;
        }
        break;

        /* ---------------------------------------------------------------------------------------- */
        /* CPM_ACW_IN_OUT                                                                           */
        /* ---------------------------------------------------------------------------------------- */
        case EDDP_CRT_CMD_CPM_ACW_IN:
        case EDDP_CRT_CMD_CPM_ACW_OUT:
        {
            EDDP_CRT_CONS_ACW_INFO_PTR_PTR_TYPE pCommonAcwPtr;
            EDDP_CRT_CONSUMER_RBASE_PTR_TYPE    pRBase;
            EDD_COMMON_MEM_PTR_TYPE             TempPtr;
            LSA_UINT16                          RBaseIndex, ConsumerIdPrev;
            LSA_UINT32                          ApiAdr_NextPointer_Prev_Temp;
            LSA_UINT32                          ApiAdr_Acw_New_Temp;

            TempPtr								= 0;

            pCmdParam = EDDP_CRTCmdGetInternalCmdParamElement(pDDB);
            pCommonAcwPtr = pDDB->CRT.Cons.pAcwPtrArray;  // pointer array holds to different structs
            if ( AcwId < pDDB->CRT.CfgParam.ConsumerMaxNumber )
            {   // ***** Note: no support for ACW_DG's here because we use the container ACW for In/Out !!!
                EDDP_ASSERT_FALSE(pDDB->CRT.Cons.pConsEntry[AcwId].ConsumerFlags & EDDP_CONS_FLAG_IS_DFP_ITEM);
            }
            // else container ACW

            ConsumerIdPrev  = pCommonAcwPtr[AcwId]->RBaseLink.Prev;
            RBaseIndex      = pDDB->CRT.Cons.RBaseHandler.FctGetRBaseIndex(pDDB, pCommonAcwPtr[AcwId]->FrameID);
            pRBase          = &pDDB->CRT.Cons.pRBaseEntry[RBaseIndex];
            if (ConsumerIdPrev == EDDP_CRT_CHAIN_IDX_END)
            {   // ***** address of "ACW_List_Ptr"
                TempPtr = (EDD_COMMON_MEM_PTR_TYPE) &pRBase->pHwRBaseApi->ACW_List_Ptr;
            }
            else
            {   // ***** address of "ACW.ACW_nextoffset" in prev. ACW (add +2 here for offset of ACW_nextoffset)  
                TempPtr = (EDD_COMMON_MEM_PTR_TYPE)(((EDD_COMMON_MEM_U8_PTR_TYPE)(pCommonAcwPtr[ConsumerIdPrev]->pHwAcwApi)) + sizeof(LSA_UINT16));
            }

            // get shiftet offset of "Ptr8_Temp" in APICtrl-RAM
            ApiAdr_NextPointer_Prev_Temp = EDDP_CRTGetPnipAddrShift (pDDB, TempPtr, EDDP_CRT_SHIFT_VALUE_1);

            /* -------------------------------------------------------- */
            /* address (offset) of new ACW in APICtrl-RAM               */
            /* -------------------------------------------------------- */
            ApiAdr_Acw_New_Temp = EDDP_CRTGetPnipAddrShift (pDDB, (EDD_COMMON_MEM_PTR_TYPE) pCommonAcwPtr[AcwId]->pHwAcwApi, EDDP_CRT_SHIFT_VALUE_3);


            /* -------------------------------------------------------- */
            /* copying of command parameter                             */
            /* -------------------------------------------------------- */
            pCmdParam->CmdOpcode = EDDP_CMD_CPM_ACW_IN_OUT;

            // ACW IN or OUT ?
            if (CrtCommand == EDDP_CRT_CMD_CPM_ACW_IN)
                pCmdParam->param_union.Crt.Cpm.ACW_InOut.Word0.Acw_In   = LSA_TRUE;
            else
                pCmdParam->param_union.Crt.Cpm.ACW_InOut.Word0.Acw_In   = LSA_FALSE;

            pCmdParam->param_union.Crt.Cpm.ACW_InOut.Word0.Acw_DG                   = LSA_FALSE;
            pCmdParam->param_union.Crt.Cpm.ACW_InOut.Word0.ApiAdr_NextPointer_Prev  = ApiAdr_NextPointer_Prev_Temp;
            pCmdParam->param_union.Crt.Cpm.ACW_InOut.Word1.ApiAdr_Acw_New           = ApiAdr_Acw_New_Temp;
            pCmdParam->param_union.Crt.Cpm.ACW_InOut.AcwID                          = AcwId;
        }
        break;

        /* ---------------------------------------------------------------------------------------- */
        /* CPM_ACW_SET_UNKNOWN                                                                      */
        /* ---------------------------------------------------------------------------------------- */
        case EDDP_CRT_CMD_CPM_ACW_SET_UNKNOWN:
        {
            EDDP_CRT_CONSUMER_MNGM_PTR_TYPE     pConsDDB;
            LSA_BOOL                            Acw_DG;
            LSA_UINT16                          DgIdx;
            LSA_UINT32                          ApiAdr_Acw_Temp;
            EDD_COMMON_MEM_PTR_TYPE             ApiVirtualAdr;

            // ***** Note: ACW-container not allowed here !!!
            EDDP_ASSERT_FALSE(AcwId >= pDDB->CRT.CfgParam.ConsumerMaxNumber);

            pConsDDB = &pDDB->CRT.Cons;

            // ***** normal ACW or ACW_DG ?
            if (pConsDDB->pConsEntry[AcwId].ConsumerFlags & EDDP_CONS_FLAG_IS_DFP_ITEM)
            {   // ***** we need here the location of the ACW_DG (idx mapping to pConsDgEntry[])
                DgIdx = pConsDDB->pConsEntry[AcwId].DgIdx;
                if (DgIdx == EDDP_CRT_CHAIN_IDX_END)
                {
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdRequestCpm(): FATAL -> ConsumerId=%d holds invalid DgIdx !",
                        AcwId);
                    EDDP_FATAL(EDDP_FATAL_ERR_SERVICE);
                }
                ApiVirtualAdr = (EDD_COMMON_MEM_PTR_TYPE) pConsDDB->pConsDgEntry[DgIdx].pHwAcwDgApi;
                Acw_DG        = LSA_TRUE;
            }
            else
            {   // ***** we need here the location of the ACW (non DFP or container ACW)
                ApiVirtualAdr = (EDD_COMMON_MEM_PTR_TYPE) pConsDDB->pConsEntry[AcwId].ConsAcwTypeInfo.pHwAcwApi;
                Acw_DG        = LSA_FALSE;
            }
            /* -------------------------------------------------------- */
            /* address (offset) of ACW in APICtrl-RAM                   */
            /* -------------------------------------------------------- */
            ApiAdr_Acw_Temp = EDDP_CRTGetPnipAddrShift (
                                    pDDB, 
                                    ApiVirtualAdr,
                                    EDDP_CRT_SHIFT_VALUE_3);

            pCmdParam = EDDP_CRTCmdGetInternalCmdParamElement(pDDB);

            pCmdParam->CmdOpcode = EDDP_CMD_CPM_ACW_SET_UNKNOWN;
            pCmdParam->param_union.Crt.Cpm.SetUnknown.ConsumerId = AcwId;
            pCmdParam->param_union.Crt.Cpm.SetUnknown.Acw_DG     = Acw_DG;
            pCmdParam->param_union.Crt.Cpm.SetUnknown.ApiAdr_Acw = ApiAdr_Acw_Temp;
        }
        break;

        /* ---------------------------------------------------------------------------------------- */
        /* CPM_ACW_XCHANGE_BUFF                                                                     */
        /* ---------------------------------------------------------------------------------------- */
        case EDDP_CRT_CMD_CPM_ACW_XCHANGE_BUFF:
        {
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdRequestCpm(): FATAL -> CPM_ACW_XCHANGE_BUFF not supported!");
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;

        /* ---------------------------------------------------------------------------------------- */
        /* unknown CPM command                                                                      */
        /* ---------------------------------------------------------------------------------------- */
        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdRequestCpm(): FATAL -> CrtCommand(0x%X) unknown", 
                CrtCommand);
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }

    if (pCmdParam != LSA_NULL)
    {
        // ***** synchron or asynchron ?
        pCmdParam->Cbf  = pCbPtr;

        /* -------------------------------------------------------- */
        /* call CMD-Interface                                       */
        /* -------------------------------------------------------- */
        ResponseCmd = EDDP_CmdRequest (pDDB, pCmdParam);

        // check Response of CMD-Interface
        if (pCbPtr == 0)
        {
            // synchron, Response must be EDD_STS_OK
            if (ResponseCmd != EDD_STS_OK)
            {
                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdRequestCpm(): FATAL -> ResponseCmd(0x%X) of EDDP_CmdRequest() is not OK! pCbPtr=%p",
                    ResponseCmd, pCbPtr);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
        }
        else
        {
            // asynchron, Response must be EDD_STS_OK_PENDING
            if (ResponseCmd != EDD_STS_OK_PENDING)
            {
                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdRequestCpm(): FATAL -> ResponseCmd(0x%X) of EDDP_CmdRequest() is not OK_PENDING! pCbPtr=%p",
                    ResponseCmd, pCbPtr);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
        }
    }

    return ResponseCmd;
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTCmdDoneCbfPpm                                       +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB			                +*/
/*+						   :	EDDP_CMD_PARAM_PTR_TYPE       pCmdParam                     +*/
/*+						   :	EDD_RSP                       ResponseCmd                   +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                   (in)              +*/
/*+  pCmdParam      : Pointer to parameter structure                      (in)              +*/
/*+  ResponseCmd    : Response code for the confirmed command             (in)              +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description:  This function is called by command interface if command has finished     +*/
/*+                which was invoked asynchron by CRT-Provider.                             +*/
/*+                                                                                         +*/
/*+                Every command must be executed successfully. If no FATAL.                +*/
/*+                                                                                         +*/
/*+                The corrensponding RQB is sent back to the user.                         +*/
/*+                                                                                         +*/
/*+                Call RQB-Handler for other RQBs in Queue.                                +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTCmdDoneCbfPpm(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_CMD_PARAM_PTR_TYPE     pCmdParam,
    EDD_RSP                     ResponseCmd)
{
    EDD_RSP                             Response;
    LSA_BOOL                            isRequestFinish;
    EDDP_LOCAL_HDB_PTR_TYPE             pHDB;
    EDDP_GLB_CRT_RQB_HANDLER_PTR_TYPE   pRQBHandler;
    EDD_UPPER_RQB_PTR_TYPE              pRQB = LSA_NULL;
    EDD_SERVICE                         Service = 0;

    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTCmdDoneCbfPpm(pCmdParam=0x%X, ResponseCmd=0x%X): CmdOpcode=0x%X",
        pCmdParam, ResponseCmd, pCmdParam->CmdOpcode);

    Response            = EDD_STS_OK;
    isRequestFinish     = LSA_FALSE;
    pHDB                = LSA_NULL;
    pRQBHandler         = &pDDB->GlbCrtRQBHandler;

    if (ResponseCmd != EDD_STS_OK)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfPpm(): FATAL -> ResponseCmd(0x%X) not Ok!",
            ResponseCmd);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        // from which command is this Cbf ?
        switch (pCmdParam->CmdOpcode)
        {
            /* ---------------------------------------------------------------------------------------- */
            /* PPM_NOP                                                                                  */
            /* ---------------------------------------------------------------------------------------- */
            case EDDP_CMD_PPM_NOP:
            {
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfPpm(): FATAL -> PPM_NOP asynchronous not supported!");
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }
            break;

            /* ---------------------------------------------------------------------------------------- */
            /* PPM_ON_OFF                                                                               */
            /* ---------------------------------------------------------------------------------------- */
            case EDDP_CMD_PPM_ON_OFF:
            {
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfPpm(): FATAL -> PPM_ON_OFF asynchronous not supported!");
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }
            break;

            /* ---------------------------------------------------------------------------------------- */
            /* PPM_ACW_IN_OUT                                                                           */
            /* ---------------------------------------------------------------------------------------- */
            case EDDP_CMD_PPM_ACW_IN_OUT:
            {
                LSA_UINT16                                  ProviderId;
                LSA_UINT16                                  AcwIdx;
                EDDP_CRT_PROVIDER_MNGM_PTR_TYPE             pProvMngm;
                EDDP_CRT_PROVIDER_PTR_TYPE                  pProvider;
                EDDP_CRT_PROVIDER_ACW_PTR_TYPE              pAcw;

                pProvMngm    = &pDDB->CRT.Prov;
                ProviderId      = pCmdParam->param_union.Crt.Ppm.ACW_InOut.ProviderId;
                EDDP_ASSERT_FALSE(ProviderId >= pDDB->CRT.CfgParam.ProviderMaxNumber);
                pProvider       = &pProvMngm->pProvEntry[ProviderId];

                /* remove first RQB */
                pRQB = EDDP_GlbRQBQueueRemoveFromBegin(&pRQBHandler->RqbInProgressQueue);

                if (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL))
                {
                    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfPpm(): FATAL -> no RQB in work!");
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }

                Service = EDD_RQB_GET_SERVICE(pRQB);

                EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCmdDoneCbfPpm(): from Service=0x%X, ProviderId=%d, ACW_InOut=0x%X",
                    Service, ProviderId, pCmdParam->param_union.Crt.Ppm.ACW_InOut.Acw_In);

                // from which CRT service is this Cbf ?
                switch (Service)
                {
                    /* ------------------------------------------------ */
                    /* PROVIDER_CONTROL                                 */
                    /* ------------------------------------------------ */
                    case EDD_SRV_SRT_PROVIDER_CONTROL:
                    {
                        EDD_UPPER_CSRT_PROVIDER_CONTROL_PTR_TYPE pRqbParam = (EDD_UPPER_CSRT_PROVIDER_CONTROL_PTR_TYPE) pRQB->pParam;

                        /* sanity check */
                        if(ProviderId != pRqbParam->ProviderID)
                        {
                            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfPpm(): FATAL -> ProviderID from command does not match ProviderID from RQB!");
                                EDDP_FATAL(EDDP_FATAL_ERR_SERVICE);
                        }
                        
                        if (pCmdParam->param_union.Crt.Ppm.ACW_InOut.Acw_In)
                        {
                            /* -------------------------------- */
                            /* ACW_in from provider activate    */

                            /* -------------------------------- */
                            if (pCmdParam->param_union.Crt.Ppm.ACW_OnOff.AcwId == EDDP_CRT_CHAIN_IDX_END)
                            {
                                /* -------------------------------------------- */
                                /* ACW_in from provider activate (all ACWs)     */
                                /* -------------------------------------------- */
                                AcwIdx = pProvMngm->pProvEntry[ProviderId].AcwList.BeginIdx;
                                while (AcwIdx != EDDP_CRT_CHAIN_IDX_END)
                                {
                                    EDDP_ASSERT_FALSE(AcwIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);
                                    // set this ACW as ACTIVE
                                    EDDP_CRTProvAcwSetActiveRTFrame (pDDB, ProviderId, AcwIdx);
                                    // next AcwIdx in this provider
                                    AcwIdx = pProvMngm->pAcwEntry[AcwIdx].AcwProvLink.Next;
                                }
                            }
                            else
                            {
                                /* -------------------------------------------- */
                                /* ACW_in from provider activate (one ACW)      */
                                /* -------------------------------------------- */
                                AcwIdx = pCmdParam->param_union.Crt.Ppm.ACW_OnOff.AcwId;
                                EDDP_ASSERT_FALSE(AcwIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);
                                // set this ACW as ACTIVE
                                EDDP_CRTProvAcwSetActiveRTFrame (pDDB, ProviderId, AcwIdx);
                            }

                            // request is finished
                            isRequestFinish = LSA_TRUE;
                        }
                        else
                        {
                            /* -------------------------------- */
                            /* ACW_out from provider passivate  */
                            /* -------------------------------- */
                            if (pCmdParam->param_union.Crt.Ppm.ACW_InOut.AcwId == EDDP_CRT_CHAIN_IDX_END)
                            {
                                /* -------------------------------------------- */
                                /* ACW_out from provider passivate (all ACWs)   */
                                /* -------------------------------------------- */
                                // all ACWs of this provider were removed from HW
                                AcwIdx = pProvider->AcwList.BeginIdx;
                                while (AcwIdx != EDDP_CRT_CHAIN_IDX_END)
                                {
                                    EDDP_ASSERT_FALSE(AcwIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

                                    pAcw        = &pProvMngm->pAcwEntry[AcwIdx];
                                    
                                    /* ACW in API-CTRL-RAM is to be cleared now */
                                    /* NOTE: software trog chaining is already cleared before the command is executed */
                                    EDDP_CRTProvAcwDeleteHwApi (pDDB, AcwIdx);
                                    
                                    // next AcwIdx in this provider
                                    AcwIdx = pAcw->AcwProvLink.Next;
                                }
                            }
                            else
                            {
                                /* -------------------------------------------- */
                                /* ACW_out from provider passivate (one ACW)    */
                                /* -------------------------------------------- */
                                AcwIdx      = pCmdParam->param_union.Crt.Ppm.ACW_InOut.AcwId;
                                EDDP_ASSERT_FALSE(AcwIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);
                                pAcw        = &pProvMngm->pAcwEntry[AcwIdx];
                                
                                /* ACW in API-CTRL-RAM is to be cleared now */
                                /* NOTE: software trog chaining is already cleared before the command is executed */
                                EDDP_CRTProvAcwDeleteHwApi (pDDB, AcwIdx);
                            }

                            // request is finished
                            isRequestFinish = LSA_TRUE;
                        }
                    }
                    break;

                    /* ------------------------------------------------ */
                    /* PROVIDER_REMOVE                                  */
                    /* ------------------------------------------------ */
                    case EDD_SRV_SRT_PROVIDER_REMOVE:
                    {
                        EDD_UPPER_CSRT_PROVIDER_REMOVE_PTR_TYPE pRqbParam = (EDD_UPPER_CSRT_PROVIDER_REMOVE_PTR_TYPE) pRQB->pParam;

                        /* sanity check */
                        if(ProviderId != pRqbParam->ProviderID)
                        {
                            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfPpm(): FATAL -> ProviderID from command does not match ProviderID from RQB!");
                                EDDP_FATAL(EDDP_FATAL_ERR_SERVICE);
                        }
                        
                        if (pCmdParam->param_union.Crt.Ppm.ACW_InOut.Acw_In)
                        {
                            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfPpm(): FATAL -> with PROVIDER_REMOVE is Acw_In not allowed");
                            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                        }
                        else
                        {
                            /* -------------------------------- */
                            /* ACW_out from provider remove     */
                            /* -------------------------------- */
                            if (pCmdParam->param_union.Crt.Ppm.ACW_InOut.AcwId == EDDP_CRT_CHAIN_IDX_END)
                            {
                                /* ------------------------------------------------ */
                                /* ACW_out from provider remove (always all ACWs)   */
                                /* ------------------------------------------------ */
                                AcwIdx = pProvider->AcwList.BeginIdx;
                                while (AcwIdx != EDDP_CRT_CHAIN_IDX_END)
                                {
                                    EDDP_ASSERT_FALSE(AcwIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

                                    pAcw        = &pProvMngm->pAcwEntry[AcwIdx];

                                    /* ACW in API-CTRL-RAM is to be cleared now */
                                    /* NOTE: software trog chaining is already cleared before the command is executed */
                                    EDDP_CRTProvAcwDeleteHwApi (pDDB, AcwIdx);

                                    // next AcwIdx in this provider
                                    AcwIdx = pAcw->AcwProvLink.Next;
                                }

                                // remove provider from CRT management
                                EDDP_CRTProviderRemoveFromCRTMngm (pDDB, ProviderId);

                                // request is finished
                                isRequestFinish = LSA_TRUE;
                            }
                            else
                            {
                                /* -------------------------------------------- */
                                /* ACW_out from provider remove (one ACW)       */
                                /* -------------------------------------------- */
                                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfPpm(): FATAL -> ACW_out from provider remove only for one ACW does not supported!");
                                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                            }
                        }
                    }
                    break;

                    /* ------------------------------------------------ */
                    /* Unsupported Request in this case of Cbf          */
                    /* ------------------------------------------------ */
                    default:
                    {
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfPpm(): FATAL -> Unsupported Service(0x%X) in this case of Cbf!",
                            Service);
                        EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                    }
                    break;
                }
            }
            break;

            /* ---------------------------------------------------------------------------------------- */
            /* PPM_ACW_MOVE                                                                             */
            /* ---------------------------------------------------------------------------------------- */
            case EDDP_CMD_PPM_ACW_MOVE:
            {
                LSA_UINT16                              ProviderId;
                LSA_UINT16                              OldCyclePhase;
                LSA_UINT16                              TBaseIndex;
                EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
                EDDP_CRT_PROVIDER_PTR_TYPE              pProvider;
                EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;

                pProvMngm    = &pDDB->CRT.Prov;
                ProviderId      = pCmdParam->param_union.Crt.Ppm.ACW_Move.ProviderId;
                EDDP_ASSERT_FALSE(ProviderId >= pDDB->CRT.CfgParam.ProviderMaxNumber);
                pProvider       = &pProvMngm->pProvEntry[ProviderId];
                EDDP_ASSERT_FALSE(pProvider->AcwList.BeginIdx == EDDP_CRT_CHAIN_IDX_END);
                pAcw            = &pProvMngm->pAcwEntry[pProvider->AcwList.BeginIdx];
                OldCyclePhase   = pCmdParam->param_union.Crt.Ppm.ACW_Move.CyclePhase;

                /* remove first RQB */
                pRQB = EDDP_GlbRQBQueueRemoveFromBegin(&pRQBHandler->RqbInProgressQueue);

                if (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL))
                {
                    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfPpm(): FATAL -> no RQB in work!");
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }

                Service = EDD_RQB_GET_SERVICE(pRQB);

                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCmdDoneCbfPpm(): from Service=0x%X, ProviderId=%d",
                    Service, ProviderId);

                // from which CRT service is this Cbf ?
                switch (Service)
                {
                    /* ------------------------------------------------ */
                    /* PROVIDER_CHANGE_PHASE                            */
                    /* ------------------------------------------------ */
                    case EDD_SRV_SRT_PROVIDER_CHANGE_PHASE:
                    {
                        EDD_UPPER_CSRT_PROVIDER_CHANGE_PHASE_PTR_TYPE pRqbParam = (EDD_UPPER_CSRT_PROVIDER_CHANGE_PHASE_PTR_TYPE) pRQB->pParam;

                        /* sanity check */
                        if(ProviderId != pRqbParam->ProviderID)
                        {
                            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfPpm(): FATAL -> ProviderID from command does not match ProviderID from RQB!");
                                EDDP_FATAL(EDDP_FATAL_ERR_SERVICE);
                        }
                        
                        /* ------------------------ */
                        /* old AcwPhaseList         */
                        /* ------------------------ */
                        TBaseIndex  = EDDP_CRTGetTBaseIndex ( pDDB,
                                                              pAcw->AddParam.CycleReductionRatio,
                                                              OldCyclePhase);
                        // if this is last ACW in Trog, delete Trog/TBase
                        EDDP_CRTProvAcwIsLastAcwRemovedFromTBase (pDDB, TBaseIndex);

                        // request is finished
                        isRequestFinish = LSA_TRUE;
                    }
                    break;

                    /* ------------------------------------------------ */
                    /* Unsupported Request in this case of Cbf          */
                    /* ------------------------------------------------ */
                    default:
                    {
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfPpm(): FATAL -> Unsupported Service(0x%X) in this case of Cbf!",
                            Service);
                        EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                    }
                    break;
                }
            }
            break;

            /* ---------------------------------------------------------------------------------------- */
            /* PPM_ACW_XCHANGE_BUFF                                                                     */
            /* ---------------------------------------------------------------------------------------- */
            case EDDP_CMD_PPM_ACW_XCHANGE_BUFF:
            {
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfPpm(): FATAL -> PPM_ACW_XCHANGE_BUFF asynchronous not supported!");
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }
            break;

            /* ---------------------------------------------------------------------------------------- */
            /* PPM_Starttime: command "Update_BASECYCLE"                                                */
            /* ---------------------------------------------------------------------------------------- */
            case EDDP_CMD_GSYNC_UPDATE_BASECYCL:
            {
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfPpm(): FATAL -> PPM_Starttime asynchronous not supported!");
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }
            break;

            /* ---------------------------------------------------------------------------------------- */
            /* unknown command                                                                          */
            /* ---------------------------------------------------------------------------------------- */
            default:
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfPpm(): FATAL -> unknown command! CmdOpcode=0x%X",
                    pCmdParam->CmdOpcode);
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }
            break;
        }

        /* -------------------------------------------------------- */
        /* is request finished ?                                    */
        /* -------------------------------------------------------- */
        if (isRequestFinish)
        {
            // get Handle for this ExecuteRQB
            Response = EDDP_GlbHandleGetHDB (EDD_RQB_GET_HANDLE_LOWER(pRQB), &pHDB);

            if (Response != EDD_STS_OK)
            {
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "EDDP_CRTCmdDoneCbfPpm(): Invalid handle lower in RQB!");
                EDD_RQB_SET_RESPONSE(pRQB, EDD_STS_ERR_PARAM);
                EDDP_RQB_ERROR(pRQB);
            }

            // finish this request
            EDDP_GlbRequestFinish (pHDB, pRQB, EDD_STS_OK);

            // trigger the finish events of this Service
            EDDP_GlbTriggerFinishEventAfterRQBConfirmation (pDDB, pHDB, Service);
        }
        else
        {
            /* request is pending, add RQB to end of "in progress" queue */
            EDDP_GlbRQBQueueAddToEnd(&pRQBHandler->RqbInProgressQueue, pRQB);
        }
        
        /* NOTE: EDDP_GlbCrtRQBHandler is called within EDDP_CMD_ISR_ReqDone_IFA */
    }
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTCmdDoneCbfPpmPackGroup                              +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB			                +*/
/*+						   :	EDDP_CMD_PARAM_PTR_TYPE       pCmdParam                     +*/
/*+						   :	EDD_RSP                       ResponseCmd                   +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                   (in)              +*/
/*+  pCmdParam      : Pointer to parameter structure                      (in)              +*/
/*+  ResponseCmd    : Response code for the confirmed command             (in)              +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description:  This function is called by command interface if command has finished     +*/
/*+                which was invoked asynchron by CRT-Provider.                             +*/
/*+                                                                                         +*/
/*+                Every command must be executed successfully. If no FATAL.                +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTCmdDoneCbfPpmPackGroup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_CMD_PARAM_PTR_TYPE     pCmdParam,
    EDD_RSP                     ResponseCmd)
{
    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTCmdDoneCbfPpmPackGroup(pCmdParam=0x%X, ResponseCmd=0x%X): CmdOpcode=0x%X",
        pCmdParam, ResponseCmd, pCmdParam->CmdOpcode);

    if (ResponseCmd != EDD_STS_OK)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfPpmPackGroup(): FATAL -> ResponseCmd(0x%X) not Ok!",
            ResponseCmd);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        // from which command is this Cbf ?
        switch (pCmdParam->CmdOpcode)
        {
            /* ---------------------------------------------------------------------------------------- */
            /* PPM_ACW_IN_OUT                                                                           */
            /* ---------------------------------------------------------------------------------------- */
            case EDDP_CMD_PPM_ACW_IN_OUT:
            {
                LSA_UINT16                                  AcwIdx;

                AcwIdx = pCmdParam->param_union.Crt.Ppm.ACW_InOut.AcwId;
                EDDP_ASSERT_FALSE(AcwIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

                if (pCmdParam->param_union.Crt.Ppm.ACW_InOut.Acw_In)
                {
                    /* ------------ */
                    /* ACW_in       */
                    /* ------------ */
                    // set this pack ACW as ACTIVE
                    EDDP_CRTProvAcwSetActivePackFrame (pDDB, AcwIdx);
                    // go back to state machine
                    EDDP_CRTProviderPackGroupActivateAllStateMachineCbf (pDDB, EDD_STS_OK);
                }
                else
                {
                    /* ------------ */
                    /* ACW_out      */
                    /* ------------ */
                    // delete ACW and its ACW_DGs in APICtrl-RAM
                    EDDP_CRTProvAcwAcwDGDeleteHwApi(pDDB, AcwIdx);
                    // go back to state machine
                    EDDP_CRTProviderPackGroupPassivateAllStateMachineCbf (pDDB, EDD_STS_OK);
                }
            }
            break;

            /* ---------------------------------------------------------------------------------------- */
            /* unknown command                                                                          */
            /* ---------------------------------------------------------------------------------------- */
            default:
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfPpmPackGroup(): FATAL -> unknown command! CmdOpcode=0x%X",
                    pCmdParam->CmdOpcode);
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }
            break;
        }
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTCmdDoneCbfCpm                                       +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB			                +*/
/*+						   :	EDDP_CMD_PARAM_PTR_TYPE       pCmdParam                     +*/
/*+						   :	EDD_RSP                       ResponseCmd                   +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                   (in)              +*/
/*+  pCmdParam      : Pointer to parameter structure                      (in)              +*/
/*+  ResponseCmd    : Response code for the confirmed command             (in)              +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description:  This function is called by command interface if command has finished     +*/
/*+                which was invoked asynchron by CRT-Consumer.                             +*/
/*+                                                                                         +*/
/*+                Every command must be executed successfully. If no FATAL.                +*/
/*+                                                                                         +*/
/*+                The corrensponding RQB is sent back to the user.                         +*/
/*+                                                                                         +*/
/*+                Call RQB-Handler for other RQBs in Queue.                                +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTCmdDoneCbfCpm(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_CMD_PARAM_PTR_TYPE     pCmdParam,
    EDD_RSP                     ResponseCmd)
{
    EDD_RSP                             Response;
    LSA_BOOL                            isRequestFinish;
    EDDP_LOCAL_HDB_PTR_TYPE             pHDB;
    EDDP_GLB_CRT_RQB_HANDLER_PTR_TYPE   pRQBHandler;
    EDD_UPPER_RQB_PTR_TYPE              pRQB;
    EDD_SERVICE                         Service;

    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTCmdDoneCbfCpm(pCmdParam=0x%X, ResponseCmd=0x%X): CmdOpcode=0x%X",
        pCmdParam, ResponseCmd, pCmdParam->CmdOpcode);

    Response            = EDD_STS_OK;
    isRequestFinish     = LSA_FALSE;
    pHDB                = LSA_NULL;
    pRQBHandler         = &pDDB->GlbCrtRQBHandler;

    if (ResponseCmd != EDD_STS_OK)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfCpm(): FATAL -> ResponseCmd(0x%X) not Ok!",
            ResponseCmd);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        /* remove first RQB */
        pRQB = EDDP_GlbRQBQueueRemoveFromBegin(&pRQBHandler->RqbInProgressQueue);

        if (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL))
        {
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfCpm(): FATAL -> no RQB in work!");
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }

        Service = EDD_RQB_GET_SERVICE(pRQB);

        // from which command is this Cbf ?
        switch (pCmdParam->CmdOpcode)
        {
            /* ---------------------------------------------------------------------------------------- */
            /* CPM_NOP                                                                                  */
            /* ---------------------------------------------------------------------------------------- */
            case EDDP_CMD_CPM_NOP:
            {   // ***** always used synchron
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfCpm(): FATAL -> CPM_NOP asynchronous not supported!");
                    EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }
            break;

            /* ---------------------------------------------------------------------------------------- */
            /* CPM_ON_OFF                                                                               */
            /* ---------------------------------------------------------------------------------------- */
            case EDDP_CMD_CPM_ON_OFF:
            {   // ***** always used synchron
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfCpm(): FATAL -> CPM_ON_OFF asynchronous not supported!");
                    EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }
            break;

            /* ---------------------------------------------------------------------------------------- */
            /* CPM_ACW_ON_OFF                                                                           */
            /* ---------------------------------------------------------------------------------------- */
            case EDDP_CMD_CPM_ACW_ON_OFF:
            {   // ***** ACW_ON: CONSUMER_CONTROL, ACW_OFF: CONSUMER_CONTROL or CONSUMER_REMOVE
                LSA_UINT16                      ConsumerId;
                EDDP_CRT_CONSUMER_PTR_TYPE      pConsumer;

                ConsumerId  = pCmdParam->param_union.Crt.Cpm.ACW_OnOff.AcwID;
                pConsumer   = &pDDB->CRT.Cons.pConsEntry[ConsumerId];

                EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCmdDoneCbfCpm(): from Service=0x%X, ConsumerId=%d, ACW_OnOff=0x%X",
                    Service, ConsumerId, pCmdParam->param_union.Crt.Cpm.ACW_OnOff.Acw_On);

                // from which CRT service is this Cbf ?
                switch (Service)
                {
                    /* ------------------------------------------------ */
                    /* CONSUMER_CONTROL                                 */
                    /* ------------------------------------------------ */
                    case EDD_SRV_SRT_CONSUMER_CONTROL:
                    {
                        /* sanity check */
                        if(ConsumerId != ((EDD_UPPER_CSRT_CONSUMER_CONTROL_PTR_TYPE) pRQB->pParam)->ConsumerID)
                        {
                            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfCpm(): FATAL -> ConsumerID from command does not match ConsumerID from RQB!");
                                EDDP_FATAL(EDDP_FATAL_ERR_SERVICE);
                        }
                        
                        if (pCmdParam->param_union.Crt.Cpm.ACW_OnOff.Acw_On)
                        {   // ***** ACW_ON: set internal state to active
                            pConsumer->State = EDDP_CRT_STATE_HW_ACTIVE;
                            /* ------------------------------------------------ */
                            /* is provoder autostop feature  ?                  */
                            /* ------------------------------------------------ */
                            if (pConsumer->ConsumerFlags & EDDP_CONS_FLAG_PENDING_PROV_AUTOSTOP_ON)
                            {   // ***** consumer activated with ACW_ON cmd --> fire now the indication event for the PPM SW-module
                                EDDP_CRTConsumerEventForProvAutoStop (pDDB, pConsumer, EDD_CONS_CONTROL_MODE_PROV_AUTOSTOP_ENABLE, LSA_TRUE);
                            }
                        }
                        else
                        {   // ***** ACW_OFF: set internal state to inactive and fire INACTIVE event to the PPM-Mgnt
                            pConsumer->State = EDDP_CRT_STATE_HW_PASSIVE;

                            // is SRD-Consumer ?
                            if (pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_SRD_ITEM)
                            {   // ***** patching DataStatus to BACKUP within APICtrl-RAM
                                EDDP_CRTConsForceBackupDataStatusHwApi (pDDB, pConsumer);
                            }

                            // is DFP-Consumer ?
                            if (pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_DFP_ITEM)
                            {
                                // disconnect this consumer from DG
                                EDDP_CRTConsumerDisconnectFromConsDgEntry (pDDB, pConsumer);
                            }

                            // trigger MRT diagnosis only if this is MRPD consumer and IsPrmMRPDWatchDogActive is ON
                            if ((EDDP_IS_IRT_REDUNDANT_FRAMEID(pConsumer->Param.FrameID)) && (pConsumer->MrtDiag.IsPrmMRPDWatchDogActive))
                            {
                                // this consumer must be connected to PRM -> PDIRData.PDIRFrameData[x]
                                EDDP_ASSERT_FALSE(pConsumer->MrtDiag.PrmPDIRFrameDataIdx == EDDP_CRT_CHAIN_IDX_END);
                                // consumer is passivated, so trigger MRT diagnosis for this case:
                                // if a MRT diagnosis was fired as Appear, it must be fired again as DisAppear
                                EDDP_PRMDiagTriggerMRTDiagSingle (pDDB, pConsumer->MrtDiag.PrmPDIRFrameDataIdx, LSA_FALSE, LSA_FALSE, LSA_FALSE);
                            }
                        }

                        // request is finished
                        isRequestFinish = LSA_TRUE;
                    }
                    break;

                    /* ------------------------------------------------ */
                    /* CONSUMER_REMOVE                                  */
                    /* ------------------------------------------------ */
                    case EDD_SRV_SRT_CONSUMER_REMOVE:
                    {
                        /* sanity check */
                        if(ConsumerId != ((EDD_UPPER_CSRT_CONSUMER_REMOVE_PTR_TYPE) pRQB->pParam)->ConsumerID)
                        {
                            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfCpm(): FATAL -> ConsumerID from command does not match ConsumerID from RQB!");
                                EDDP_FATAL(EDDP_FATAL_ERR_SERVICE);
                        }
                        
                        // active consumer is removed, it is deactivated first
                        if (pCmdParam->param_union.Crt.Cpm.ACW_OnOff.Acw_On)
                        {
                            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfCpm(): FATAL -> with CONSUMER_REMOVE is Acw_On not allowed");
                            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                        }
                        else
                        {   // **** ACW_OFF: set passiv state
                            pConsumer->State = EDDP_CRT_STATE_HW_PASSIVE;   // indication to PPM at end of ACW_OUT
                                EDDP_ASSERT(pDDB->CRT.Cons.ConsumerActCnt);
                                pDDB->CRT.Cons.ConsumerActCnt--;

                            // trigger MRT diagnosis only if this is MRPD consumer and IsPrmMRPDWatchDogActive is ON
                            if ((EDDP_IS_IRT_REDUNDANT_FRAMEID(pConsumer->Param.FrameID)) && (pConsumer->MrtDiag.IsPrmMRPDWatchDogActive))
                            {
                                // this consumer must be connected to PRM -> PDIRData.PDIRFrameData[x]
                                EDDP_ASSERT_FALSE(pConsumer->MrtDiag.PrmPDIRFrameDataIdx == EDDP_CRT_CHAIN_IDX_END);
                                // consumer is passivated, so trigger MRT diagnosis for this case:
                                // if a MRT diagnosis was fired as Appear, it must be fired again as DisAppear
                                EDDP_PRMDiagTriggerMRTDiagSingle (pDDB, pConsumer->MrtDiag.PrmPDIRFrameDataIdx, LSA_FALSE, LSA_FALSE, LSA_FALSE);
                            }
                        }

                        // is DFP-Consumer ?
                        if (pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_DFP_ITEM)
                        {
                            // disconnect this consumer from DG
                            EDDP_CRTConsumerDisconnectFromConsDgEntry (pDDB, pConsumer);
                            // remove this consumer from SW list only, because state is SW_USED !!!
                            EDDP_CRTConsumerRemoveFromCRTMngm (pDDB, pConsumer->ConsumerId, LSA_FALSE);
                            // request is finished for DFP-Consumer
                            isRequestFinish = LSA_TRUE;
                        }
                        else
                        {
                            // now we can remove this non DFP consumer
                            // ***** call CmdRequest ASYNCHRON
                            // if command is finished, the Cbf EDDP_CRTCmdDoneCbfCpm() will be called
                            EDDP_CRTCmdRequestCpm (pDDB, EDDP_CRT_CMD_CPM_ACW_OUT, ConsumerId, EDDP_CRTCmdDoneCbfCpm);
                        }
                    }
                    break;

                    /* ------------------------------------------------ */
                    /* Unsupported Request in this case of Cbf          */
                    /* ------------------------------------------------ */
                    default:
                    {
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfCpm(): FATAL -> Unsupported Service(0x%X) in this case of Cbf!",
                            Service);
                        EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                    }
                    break;
                }
            }
            break;

            /* ---------------------------------------------------------------------------------------- */
            /* CPM_ACW_IN_OUT                                                                           */
            /* ---------------------------------------------------------------------------------------- */
            case EDDP_CMD_CPM_ACW_IN_OUT:
            {   // ***** ACW_IN: CONSUMER_CONTROL, ACW_OUT: CONSUMER_REMOVE
                EDDP_CRT_CONSUMER_PTR_TYPE  pConsumer;
                LSA_UINT16                  ConsumerId;

                ConsumerId  = pCmdParam->param_union.Crt.Cpm.ACW_InOut.AcwID;

                EDDP_ASSERT_FALSE(ConsumerId >= pDDB->CRT.CfgParam.ConsumerMaxNumber);

                pConsumer   = &pDDB->CRT.Cons.pConsEntry[ConsumerId];

                EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCmdDoneCbfCpm(): from Service=0x%X, ConsumerId=%d, ACW_InOut=0x%X",
                    Service, ConsumerId, pCmdParam->param_union.Crt.Cpm.ACW_InOut.Word0.Acw_In);

                // from which CRT service is this Cbf ?
                switch (Service)
                {
                    /* ------------------------------------------------ */
                    /* CONSUMER_CONTROL                                 */
                    /* ------------------------------------------------ */
                    case EDD_SRV_SRT_CONSUMER_CONTROL:
                    {
                        /* sanity check */
                        if(ConsumerId != ((EDD_UPPER_CSRT_CONSUMER_CONTROL_PTR_TYPE) pRQB->pParam)->ConsumerID)
                        {
                            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfCpm(): FATAL -> ConsumerID from command does not match ConsumerID from RQB!");
                                EDDP_FATAL(EDDP_FATAL_ERR_SERVICE);
                        }
                        
                        pConsumer->State = EDDP_CRT_STATE_HW_PASSIVE;
                        // Note: call EDDP_CRTConsumerDoControlReq again for sequence: ACW_IN --> ACW_ON (+ SetToUnknown)
                        if ( EDDP_CRTConsumerDoControlReq(pDDB, pRQB) != EDD_STS_OK_PENDING )
                        {
                            // request is finished
                            isRequestFinish = LSA_TRUE;
                        }
                    }
                    break;

                    /* ------------------------------------------------ */
                    /* CONSUMER_REMOVE                                  */
                    /* ------------------------------------------------ */
                    case EDD_SRV_SRT_CONSUMER_REMOVE:
                    {
                        /* sanity check */
                        if(ConsumerId != ((EDD_UPPER_CSRT_CONSUMER_REMOVE_PTR_TYPE) pRQB->pParam)->ConsumerID)
                        {
                            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfCpm(): FATAL -> ConsumerID from command does not match ConsumerID from RQB!");
                                EDDP_FATAL(EDDP_FATAL_ERR_SERVICE);
                        }
                        
                        // ***** remove consumer from RBaseEntry and SW list
                        EDDP_CRTConsumerRemoveFromCRTMngm (pDDB, ConsumerId, LSA_TRUE);
                        // request is finished
                        isRequestFinish = LSA_TRUE;
                    }
                    break;

                    /* ------------------------------------------------ */
                    /* Unsupported Request in this case of Cbf          */
                    /* ------------------------------------------------ */
                    default:
                    {
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfCpm(): FATAL -> Unsupported Service(0x%X) in this case of Cbf!",
                            Service);
                        EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                    }
                    break;
                }
            }
            break;

            /* ---------------------------------------------------------------------------------------- */
            /* CPM_ACW_SET_UNKNOWN                                                                      */
            /* ---------------------------------------------------------------------------------------- */
            case EDDP_CMD_CPM_ACW_SET_UNKNOWN:
            {
                if ( Service == EDD_SRV_SRT_CONSUMER_CONTROL )
                {
                    /* sanity check */
                    if(pCmdParam->param_union.Crt.Cpm.SetUnknown.ConsumerId != ((EDD_UPPER_CSRT_CONSUMER_CONTROL_PTR_TYPE) pRQB->pParam)->ConsumerID)
                    {
                        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfCpm(): FATAL -> ConsumerID from command does not match ConsumerID from RQB!");
                            EDDP_FATAL(EDDP_FATAL_ERR_SERVICE);
                    }
                    
                    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCmdDoneCbfCpm(): Ind: SET_UNKNOWN");
                    // request is finished
                    isRequestFinish = LSA_TRUE;
                }
                else
                {
                    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfCpm(): FATAL -> SET_UNKNOWN and no EDD_SRV_SRT_CONSUMER_CONTROL!!!");
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }
            }
            break;

            /* ---------------------------------------------------------------------------------------- */
            /* CPM_ACW_XCHANGE_BUFF                                                                     */
            /* ---------------------------------------------------------------------------------------- */
            case EDDP_CMD_CPM_ACW_XCHANGE_BUFF:
            {
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfCpm(): FATAL -> CPM_ACW_XCHANGE_BUFF asynchronous not supported!");
                    EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }
            break;

            /* ---------------------------------------------------------------------------------------- */
            /* unknown command                                                                          */
            /* ---------------------------------------------------------------------------------------- */
            default:
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfCpm(): FATAL -> unknown command! CmdOpcode=0x%X",
                    pCmdParam->CmdOpcode);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }

        /* -------------------------------------------------------- */
        /* is request finished ?                                    */
        /* -------------------------------------------------------- */
        if (isRequestFinish)
        {
            // get Handle for this ExecuteRQB
            Response = EDDP_GlbHandleGetHDB (EDD_RQB_GET_HANDLE_LOWER(pRQB), &pHDB);

            if (Response != EDD_STS_OK)
            {
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_UNEXP, "EDDP_CRTCmdDoneCbfCpm(): Invalid handle lower in RQB!");
                EDD_RQB_SET_RESPONSE(pRQB, EDD_STS_ERR_PARAM);
                EDDP_RQB_ERROR(pRQB);
            }

            // finish this request
            EDDP_GlbRequestFinish (pHDB, pRQB, EDD_STS_OK);

            // trigger the finish events of this Service
            EDDP_GlbTriggerFinishEventAfterRQBConfirmation (pDDB, pHDB, Service);
        }
        else
        {
            /* request is pending, add RQB to end of "in progress" queue */
            EDDP_GlbRQBQueueAddToEnd(&pRQBHandler->RqbInProgressQueue, pRQB);
        }

        /* NOTE: EDDP_GlbCrtRQBHandler is called within EDDP_CMD_ISR_ReqDone_IFA */
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTCmdDoneCbfPpmAcwDG                                  +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB			                +*/
/*+						   :	EDDP_CMD_PARAM_PTR_TYPE       pCmdParam                     +*/
/*+						   :	EDD_RSP                       ResponseCmd                   +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                   (in)              +*/
/*+  pCmdParam      : Pointer to parameter structure                      (in)              +*/
/*+  ResponseCmd    : Response code for the confirmed command             (in)              +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description:  This function is called by command interface if command has finished     +*/
/*+                which was invoked asynchron by CRT-Provider.                             +*/
/*+                                                                                         +*/
/*+                Every command must be executed successfully. If no FATAL.                +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTCmdDoneCbfPpmAcwDG(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_CMD_PARAM_PTR_TYPE     pCmdParam,
    EDD_RSP                     ResponseCmd)
{
    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTCmdDoneCbfPpmAcwDG(pCmdParam=0x%X, ResponseCmd=0x%X): CmdOpcode=0x%X",
        pCmdParam, ResponseCmd, pCmdParam->CmdOpcode);

    if (ResponseCmd != EDD_STS_OK)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfPpmAcwDG(): FATAL -> ResponseCmd(0x%X) not Ok!",
            ResponseCmd);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        // from which command is this Cbf ?
        switch (pCmdParam->CmdOpcode)
        {
			/* ---------------------------------------------------------------------------------------- */
            /* PPM_ACW_ON_OFF                                                                           */
            /* ---------------------------------------------------------------------------------------- */
            case EDDP_CMD_PPM_ACW_ON_OFF:
            {
                LSA_UINT16      AcwDGIdx;
                
                AcwDGIdx = pCmdParam->param_union.Crt.Ppm.ACW_OnOff.AcwId; // ACW_DG
                EDDP_ASSERT_FALSE(AcwDGIdx >= pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber);
                
                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCmdDoneCbfPpmAcwDG(): ACW_OnOff=0x%X, AcwDGIdx=%d", 
                    pCmdParam->param_union.Crt.Ppm.ACW_OnOff.Acw_On, AcwDGIdx);

                // ON or OFF ?
                if (pCmdParam->param_union.Crt.Ppm.ACW_OnOff.Acw_On)
                {
                    /* -------- */
                    /* ON       */
                    /* -------- */
                    // make sure this is ACW_DG
                    if (pCmdParam->param_union.Crt.Ppm.ACW_OnOff.Acw_DG)
                    {
                        // set this ACW_DG as ACTIVE
                        EDDP_CRTProvAcwDGSetActive (pDDB, AcwDGIdx);
                    }
                    else
                    {
                        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfPpmAcwDG(): FATAL -> this function may only be called for ACW_DGs!");
                        EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                    }
                    
                    // go back to state machine
                    EDDP_CRTProviderAcwDGActivateStateMachineCbf (pDDB, pCmdParam, EDD_STS_OK);
                }
                else
                {
                    /* -------- */
                    /* OFF      */
                    /* -------- */
                    // make sure this is ACW_DG
                    if (pCmdParam->param_union.Crt.Ppm.ACW_OnOff.Acw_DG)
                    {
                        // nothing to do - ACW_DG was already passivated before call of CmdRequest
                    }
                    else
                    {
                        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfPpmAcwDG(): FATAL -> this function may only be called for ACW_DGs!");
                        EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                    }
                    // go back to state machine
                    EDDP_CRTProviderAcwDGPassivateStateMachineCbf (pDDB, pCmdParam, EDD_STS_OK);
                }
            }
		    break;

            /* ---------------------------------------------------------------------------------------- */
            /* PPM_ACW_IN_OUT                                                                           */
            /* ---------------------------------------------------------------------------------------- */
            case EDDP_CMD_PPM_ACW_IN_OUT:
            {
                LSA_UINT16      AcwIdx;

                AcwIdx = pCmdParam->param_union.Crt.Ppm.ACW_InOut.AcwId;
                EDDP_ASSERT_FALSE(AcwIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);
                
                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCmdDoneCbfPpmAcwDG(): ACW_InOut=0x%X, AcwIdx=%d",
                    pCmdParam->param_union.Crt.Ppm.ACW_InOut.Acw_In, AcwIdx);

                if (pCmdParam->param_union.Crt.Ppm.ACW_InOut.Acw_In)
                {
                    /* ------------ */
                    /* ACW_in       */
                    /* ------------ */
                    // set this pack ACW as ACTIVE
                    EDDP_CRTProvAcwSetActivePackFrame (pDDB, AcwIdx);
                    // go back to state machine
                    EDDP_CRTProviderAcwDGActivateStateMachineCbf (pDDB, pCmdParam, EDD_STS_OK);
                }
				else
				{
					/* ------------ */
					/* ACW_out      */
					/* ------------ */
                    // delete ACW and its ACW_DGs in APICtrl-RAM
                    EDDP_CRTProvAcwAcwDGDeleteHwApi(pDDB, AcwIdx);
                    // go back to state machine
                    EDDP_CRTProviderAcwDGPassivateStateMachineCbf (pDDB, pCmdParam, EDD_STS_OK);
				}
            }
            break;

            /* ---------------------------------------------------------------------------------------- */
            /* unknown command                                                                          */
            /* ---------------------------------------------------------------------------------------- */
            default:
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfPpmAcwDG(): FATAL -> unknown command! CmdOpcode=0x%X",
                    pCmdParam->CmdOpcode);
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }
            break;
        }
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTCmdRequestApplTimer                                 +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB                +*/
/*+                             LSA_UINT16                              CrtCommand          +*/
/*+                             LSA_UINT16                              SynAsyn             +*/
/*+                                                                                         +*/
/*+  Result                :    EDD_RSP                                                     +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  CrtCommand         : CRT-Command                                                       +*/
/*+  SynAsyn            : synchron or asynchron                                             +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Central collect function for calling of Command Interface                 +*/
/*+               for ApplTimer commands of CRT.                                            +*/
/*+                                                                                         +*/
/*+               CRT commands are defined in EDDP_CRT_CMD_PARAM_TYPE.                      +*/
/*+                                                                                         +*/
/*+               Fill command parameter and call command interface                         +*/
/*+               If Response of CMD-IF not OK, CMD-IF has not accepted the command         +*/
/*+               -> FATAL                                                                  +*/
/*+                                                                                         +*/
/*+               If synchron, no Cbf-function available and command is finished            +*/
/*+               after calling of command interface.                                       +*/
/*+                                                                                         +*/
/*+               If asynchron, command is finished in the Cbf-function that                +*/
/*+               is called by command interface.                                           +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCmdRequestApplTimer(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  CrtCommand,
    LSA_UINT16                  SynAsyn)
{
    EDD_RSP                     ResponseCmd;
    EDDP_CMD_PARAM_PTR_TYPE     pCmdParam;

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTCmdRequestApplTimer(CrtCommand=0x%X, SynAsyn=0x%X)",
        CrtCommand, SynAsyn);

    pCmdParam = LSA_NULL;

    switch (CrtCommand)
    {
        /* ---------------------------------------------------------------------------------------- */
        /* Connect_SYNC_A/B                                                                         */
        /* ---------------------------------------------------------------------------------------- */
        case EDDP_CRT_CMD_APPL_TIMER_CONNECT_SYNC:
        {
            pCmdParam = &pDDB->CRT.CmdParam.Connect_Sync_A_B;

            /* -------------------------------------------------------- */
            /* copying of command parameter                             */
            /* -------------------------------------------------------- */
            EDDP_ASSERT_NO_NULL_FCT_PTR(pCmdParam->param_union.Gsync.ConnectSyncA.SmCbf);
            pCmdParam->CmdOpcode = EDDP_CMD_GSYNC_CONNECT_SYNC_A;
            pCmdParam->param_union.Gsync.ConnectSyncA.ApplTimerNr = PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_SELECT_APPL_TIMER_2;

            // synchron or asynchron ?
            if (SynAsyn == EDDP_CRT_CMD_SYNCHRON)
                pCmdParam->Cbf  = 0;
            else
                pCmdParam->Cbf  = EDDP_CRTCmdDoneCbfApplTimer;
        }
        break;

        /* ---------------------------------------------------------------------------------------- */
        /* Update_APPLCYCLE                                                                         */
        /* ---------------------------------------------------------------------------------------- */
        case EDDP_CRT_CMD_APPL_TIMER_UPDATE_APPLCYCLE:
        {
            pCmdParam = &pDDB->CRT.CmdParam.Update_ApplCycle;

            EDDP_ASSERT_NO_NULL_FCT_PTR(pCmdParam->param_union.Gsync.UpdateAppCycl.SmCbf);

            /* -------------------------------------------------------- */
            /* copying of command parameter                             */
            /* -------------------------------------------------------- */
            pCmdParam->CmdOpcode = EDDP_CMD_GSYNC_UPDATE_APPCYCL;

            // which ApplTimer(1,2,3) ?
            pCmdParam->param_union.Gsync.UpdateAppCycl.ApplTimerNr = PNIP_REG_SYNCCOMMAND_IF_CONTROL__VAL_SELECT_APPL_TIMER_2;

            // activate or deactivate the ApplTimer ?
            if (pCmdParam->param_union.Gsync.UpdateAppCycl.isApplTimerOn)
            {
                EDDP_ASSERT_FALSE( !(EDDP_CRT_IS_APPL_TIMER_REDUCTION_RATIO_IN_VALID_RANGE(pDDB->CRT.CfgParam.ApplTimerReductionRatio)) );
                pCmdParam->param_union.Gsync.UpdateAppCycl.ApplicationLengthNs =
                    ((pDDB->SWI.CycleInfo.CycleBaseFactor * EDDP_CRT_CYCLE_LENGTH_BASE_NS) * (pDDB->CRT.CfgParam.ApplTimerReductionRatio));
            }
            else
            {
                pCmdParam->param_union.Gsync.UpdateAppCycl.ApplicationLengthNs = 0;
            }

            // synchron or asynchron ?
            if (SynAsyn == EDDP_CRT_CMD_SYNCHRON)
                pCmdParam->Cbf  = 0;
            else
                pCmdParam->Cbf  = EDDP_CRTCmdDoneCbfApplTimer;
        }
        break;

        /* ---------------------------------------------------------------------------------------- */
        /* unknown ApplTimer command                                                                */
        /* ---------------------------------------------------------------------------------------- */
        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdRequestApplTimer(): FATAL -> CrtCommand(0x%X) unknown",
                CrtCommand);
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }

    /* -------------------------------------------------------- */
    /* call CMD-Interface                                       */
    /* -------------------------------------------------------- */
    ResponseCmd = EDDP_CmdRequest (pDDB, pCmdParam);

    // check Response of CMD-Interface
    if (SynAsyn == EDDP_CRT_CMD_SYNCHRON)
    {
        // synchron, Response must be EDD_STS_OK
        if (ResponseCmd != EDD_STS_OK)
        {
            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdRequestApplTimer(): FATAL -> ResponseCmd(0x%X) of EDDP_CmdRequest() is not OK! SynAsyn=%d",
                ResponseCmd, SynAsyn);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
    }
    else
    {
        // asynchron, Response must be EDD_STS_OK_PENDING
        if (ResponseCmd != EDD_STS_OK_PENDING)
        {
            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdRequestApplTimer(): FATAL -> ResponseCmd(0x%X) of EDDP_CmdRequest() is not OK_PENDING! SynAsyn=%d",
                ResponseCmd, SynAsyn);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
    }

    return (ResponseCmd);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTCmdDoneCbfApplTimer                                 +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB			                +*/
/*+						   :	EDDP_CMD_PARAM_PTR_TYPE       pCmdParam                     +*/
/*+						   :	EDD_RSP                       ResponseCmd                   +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                   (in)              +*/
/*+  pCmdParam      : Pointer to parameter structure                      (in)              +*/
/*+  ResponseCmd    : Response code for the confirmed command             (in)              +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description:  This function is called by command interface if command has finished     +*/
/*+                which was invoked asynchron by ApplTimer command.                        +*/
/*+                                                                                         +*/
/*+                Every command must be executed successfully. If no FATAL.                +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTCmdDoneCbfApplTimer(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_CMD_PARAM_PTR_TYPE     pCmdParam,
    EDD_RSP                     ResponseCmd)
{
    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTCmdDoneCbfApplTimer(pCmdParam=0x%X, ResponseCmd=0x%X): CmdOpcode=0x%X",
        pCmdParam, ResponseCmd, pCmdParam->CmdOpcode);

    if (ResponseCmd != EDD_STS_OK)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfApplTimer(): FATAL -> ResponseCmd(0x%X) not Ok!",
            ResponseCmd);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        // from which command is this Cbf ?
        switch (pCmdParam->CmdOpcode)
        {
            /* ---------------------------------------------------------------------------------------- */
            /* Connect_SYNC_A                                                                           */
            /* ---------------------------------------------------------------------------------------- */
          case EDDP_CMD_GSYNC_CONNECT_SYNC_A:
            {
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCmdDoneCbfApplTimer(): Connect_SYNC_A");

                // go back to caller
                pCmdParam->param_union.Gsync.ConnectSyncA.SmCbf(pDDB, EDD_STS_OK);
                pCmdParam->param_union.Gsync.ConnectSyncA.SmCbf = 0;
            }
        break;

            /* ---------------------------------------------------------------------------------------- */
            /* Update_APPLCYCLE                                                                         */
            /* ---------------------------------------------------------------------------------------- */
          case EDDP_CMD_GSYNC_UPDATE_APPCYCL:
            {
                EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCmdDoneCbfApplTimer(): Update_APPLCYCLE - isApplTimerOn(0x%X), ApplLengthNs(%d), ApplTimerReductionRatio(%d)",
                    pCmdParam->param_union.Gsync.UpdateAppCycl.isApplTimerOn,
                    pCmdParam->param_union.Gsync.UpdateAppCycl.ApplicationLengthNs,
                    pDDB->CRT.CfgParam.ApplTimerReductionRatio);

                // go back to caller
                pCmdParam->param_union.Gsync.UpdateAppCycl.SmCbf(pDDB, EDD_STS_OK);
                pCmdParam->param_union.Gsync.UpdateAppCycl.SmCbf = 0;
            }
        break;

            /* ---------------------------------------------------------------------------------------- */
            /* unknown command                                                                          */
            /* ---------------------------------------------------------------------------------------- */
            default:
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCmdDoneCbfApplTimer(): FATAL -> unknown command! CmdOpcode=0x%X",
                    pCmdParam->CmdOpcode);
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }
            break;
        }
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTCmdGetInternalCmdParamElement                       +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB                          +*/
/*+                                                                                         +*/
/*+  Result                :    EDDP_CMD_PARAM_PTR_TYPE                                     +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                   (in)              +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description:  This function returns the next free internal CRT command parameter       +*/
/*+                element.                                                                 +*/
/*+                                                                                         +*/
/*+                When no more free elements are available or the element is still in use  +*/
/*+                fatal is called.                                                         +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDDP_CMD_PARAM_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_CRTCmdGetInternalCmdParamElement(
    EDDP_LOCAL_DDB_PTR_TYPE           pDDB)
{
    EDDP_CRT_INTERNAL_CMD_PARAM_PTR_TYPE pCmdParam = &pDDB->CRT.CmdParam.CrtInternalCmdParam;

    /* command paramter element available? */
    if(pDDB->CMD.CrtCmdDepthAlloc <= pCmdParam->UsageCnt)
    {
        EDDP_CMD_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CRTCmdGetInternalCmdParamElement(): FATAL -> all internal command parameter elements are already in use (UsageCnt=%d)!",
                          pCmdParam->UsageCnt);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    /* check if next free element is at the end of the queue */
    if ( pCmdParam->pNextFreeCmdParam == (pCmdParam->pInternalCmdParam + (pCmdParam->CmdParamCnt - 1 )) )
    {
        /* set reference to next free element */
        pCmdParam->pNextFreeCmdParam = pCmdParam->pInternalCmdParam;
    }
    else
    {
        /* set reference to next free element */
        pCmdParam->pNextFreeCmdParam++;
    }

    /* internal CRT command parameter element must not be in use */
    if(pCmdParam->pNextFreeCmdParam->param_union.Crt.InUse)
    {
        EDDP_CMD_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CRTCmdGetInternalCmdParamElement(): FATAL -> internal command parameter element is already in use (freeElement=0x%X)!",
                          pCmdParam->pNextFreeCmdParam);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        /* command parameter element now in use */
        pCmdParam->pNextFreeCmdParam->param_union.Crt.InUse = LSA_TRUE;
        pCmdParam->pNextFreeCmdParam->param_union.Crt.PpmAcwDgSmIdx = 0xFFFF;
    }

    /* set in use count */
    pCmdParam->UsageCnt++;

    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCmdGetInternalCmdParamElement(): firstElement=0x%X, freeElement=0x%X, UsageCnt=%d",
                      pCmdParam->pInternalCmdParam, pCmdParam->pNextFreeCmdParam, pCmdParam->UsageCnt);

    return pCmdParam->pNextFreeCmdParam;
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTCmdReleaseInternalCmdParamElement                   +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB                          +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                   (in)              +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description:  This function releases a command parameter element that is currently     +*/
/*+                in use.                                                                  +*/
/*+                                                                                         +*/
/*+                When no command parameter element is currently in use or the element     +*/
/*+                that is to be released is not in use a fatal is called.                  +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTCmdReleaseInternalCmdParamElement(
    EDDP_LOCAL_DDB_PTR_TYPE           pDDB)
{
    EDDP_CRT_INTERNAL_CMD_PARAM_PTR_TYPE pCmdParam = &pDDB->CRT.CmdParam.CrtInternalCmdParam;

    /* command parameter elements in use? */
    if(0 < pCmdParam->UsageCnt)
    {
        pCmdParam->UsageCnt--;
    }
    else
    {
        EDDP_CMD_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CRTCmdReleaseInternalCmdParamElement(): FATAL -> no command elements in use!");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    /* command parameter element that is to be released must be in use */
    if(pCmdParam->pNextToBeReleasedCmdParam->param_union.Crt.InUse)
    {
        pCmdParam->pNextToBeReleasedCmdParam->param_union.Crt.InUse = LSA_FALSE;
    }
    else
    {
        EDDP_CMD_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CRTCmdReleaseInternalCmdParamElement(): FATAL -> command parameter element that is to be released is NOT in use (ToBeReleasedElement=0x%X)!",
                          pCmdParam->pNextToBeReleasedCmdParam);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCmdReleaseInternalCmdParamElement(): ToBeReleasedElement=0x%X, UsageCnt=%d",
                      pCmdParam->pNextToBeReleasedCmdParam, pCmdParam->UsageCnt);

    /* check if command parameter block is at the end of the queue */
    if ( pCmdParam->pNextToBeReleasedCmdParam == (pCmdParam->pInternalCmdParam + (pCmdParam->CmdParamCnt - 1 )) )
    {   // set pCurrentConfPtr to next confirmed element
        pCmdParam->pNextToBeReleasedCmdParam = pCmdParam->pInternalCmdParam;
    }
    else
    {   /* set reference to next to be released element */
        pCmdParam->pNextToBeReleasedCmdParam++;
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTCmdTriggerCrtQueue                                  +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB                          +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                   (in)              +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description:  This function triggers the CRT command queue. On getting this trigger    +*/
/*+                the HW starts to process the CRT command queue with all its enqueued     +*/
/*+                commands.                                                                +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTCmdTriggerCrtQueue(
    EDDP_LOCAL_DDB_PTR_TYPE           pDDB)
{
    EDDP_CMD_QUEUE_INFO_PTR_TYPE pCurrent_Queue_Info = &pDDB->CMD.Queue[EDDP_CMD_INDEX_HOST_ASYNC_CRT];
    
    /* trigger CRT queue only when no command sequence is currently being processed and and new CRT commands are present */
    if( (0 != pCurrent_Queue_Info->CmdRunningCount) &&
        (0 == pCurrent_Queue_Info->CmdRunSeqCount) )
    {
        /* sanity: check again CRT queue count */
        EDDP_ASSERT(pDDB->CRT.CmdParam.CrtInternalCmdParam.UsageCnt == pCurrent_Queue_Info->Queue_pParam.Count);
        EDDP_ASSERT(pDDB->CRT.CmdParam.CrtInternalCmdParam.UsageCnt == pCurrent_Queue_Info->CmdRunningCount);

        /* latch sequence count */
        pCurrent_Queue_Info->CmdRunSeqCount = pCurrent_Queue_Info->CmdRunningCount;
        
        EDDP_CMD_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCmdTriggerCrtQueue(): Trigger CRT command queue: sequence-count=%d!", pCurrent_Queue_Info->CmdRunSeqCount);

        /* trigger queue processing */
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_TRIGGERREQQUEUE, (1 << EDDP_CMD_INDEX_HOST_ASYNC_CRT));
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTCmdIsSequenceInWork                                 +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE       pDDB                          +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_BOOL                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                   (in)              +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description:  This function checks whether a CPM/PPM command sequence is currently     +*/
/*+                processed.                                                               +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTCmdIsSequenceInWork(
    EDDP_LOCAL_DDB_PTR_TYPE           pDDB)
{
    EDDP_CMD_QUEUE_INFO_PTR_TYPE pCurrent_Queue_Info = &pDDB->CMD.Queue[EDDP_CMD_INDEX_HOST_ASYNC_CRT];
    
    return (0 != pCurrent_Queue_Info->CmdRunSeqCount);
}

