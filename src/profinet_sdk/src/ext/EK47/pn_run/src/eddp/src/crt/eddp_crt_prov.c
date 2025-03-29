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
/*  F i l e               &F: eddp_crt_prov.c                           :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  CRT (Cycle Real Time) for EDDP  -> Provider                              */
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

#define LTRC_ACT_MODUL_ID   23
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_CRT_PROV */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"           /* eddp headerfiles */
#include "eddp_int.h"           /* internal header  */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTProviderAdd                         +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
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
/*+  EDD_RQB_CSRT_PROVIDER_ADD_TYPE                                         +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_OK_PENDING                                        +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_RESOURCE                                      +*/
/*+               EDD_STS_ERR_SERVICE                                       +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Request to add a new provider (RTClass1/2/3UDP).          +*/
/*+                                                                         +*/
/*+               Check provider parameter (FrameID, DataLen, ...).         +*/
/*+                                                                         +*/
/*+               Check if free Provider is present.                        +*/
/*+               If not we return EDD_STS_ERR_RESOURCE.                    +*/
/*+                                                                         +*/
/*+               Add this new provider to CRT management.                  +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderAdd(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP                                 Response;
    LSA_UINT8                               ProviderAcwPhase;
    EDD_UPPER_CSRT_PROVIDER_ADD_PTR_TYPE    pRqbParam;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pHDB);
#endif

    Response    = EDD_STS_OK;
    pRqbParam   = (EDD_UPPER_CSRT_PROVIDER_ADD_PTR_TYPE) pRQB->pParam;

    EDDP_CRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_CRTProviderAdd(pRQB=0x%X, ProvProperties=0x%X, FrameID=0x%X): ProviderUsedCnt=%d",
        pHDB->Handle, pRQB, pRqbParam->Properties, pRqbParam->FrameID, pDDB->CRT.Prov.ProviderUsedCnt);

    ProviderAcwPhase = EDDP_CRTGetProviderAcwPhase (pDDB, pRqbParam->Properties, pRqbParam->FrameID);

    // check provider add parameter (FrameID, DataLen, ...)
    Response = EDDP_CRTCheckProviderAddParam (pDDB, pRqbParam, ProviderAcwPhase);

    if (Response == EDD_STS_OK)
    {
        // add provider to CRT management
        Response = EDDP_CRTProviderAddToCRTMngm (pDDB, pRqbParam, ProviderAcwPhase);
    }

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTProviderAdd(): ProviderUsedCnt=%d, Response=0x%X", 
        pDDB->CRT.Prov.ProviderUsedCnt, Response);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTProviderRemove                      +*/
/*                                                                          +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
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
/*+  EDD_RQB_CSRT_PROVIDER_REMOVE_TYPE                                      +*/
/*+                                                                         +*/
/*+  ProviderID : Valid ProviderID                                          +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_OK_PENDING                                        +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_RESOURCE                                      +*/
/*+               EDD_STS_ERR_SERVICE                                       +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Request to remove a provider.                             +*/
/*+                                                                         +*/
/*+               A provider must be in use. If no EDD_STS_ERR_PARAM.       +*/
/*+                                                                         +*/
/*+               Provider may be active on remove.                         +*/
/*+                 ACTIVATE:                                               +*/
/*+                     - deactivate ACW with ACW_off command               +*/
/*+                     - in Cbf of ACW_off chain ACW from AcwPhaseList     +*/
/*+                       in APICtrl-RAM with ACW_out command               +*/
/*+                     - in Cbf of ACW_out                                 +*/
/*+                         -> chain ACW from SW (RemoveFromTBase)          +*/
/*+                         -> delete ACW in APICtrl-RAM                    +*/
/*+                         -> remove provider from CRT management          +*/
/*+                         -> send CONTROL-RQB back to user                +*/
/*+                 not ACTIVATE:                                           +*/
/*+                     - only remove provider from CRT management          +*/
/*+                     - send CONTROL-RQB back to user                     +*/
/*+                                                                         +*/
/*+               Call command interface.                                   +*/
/*+               If command is finished the function                       +*/
/*+               EDDP_CRTCmdDoneCbfPpm() is called                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderRemove(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP                                     Response;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE             pProvMngm;
//  EDDP_CRT_PROVIDER_DDB_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE                  pProvider;
    EDD_UPPER_CSRT_PROVIDER_REMOVE_PTR_TYPE     pRqbParam;

    Response     = EDD_STS_OK;
    pProvMngm    = &pDDB->CRT.Prov;
    pProvMngm    = &pDDB->CRT.Prov;
    pRqbParam    = (EDD_UPPER_CSRT_PROVIDER_REMOVE_PTR_TYPE) pRQB->pParam;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pHDB);
#endif

    EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_CRTProviderRemove(pRQB=0x%X, ProviderID=%d): ProviderUsedCnt=%d", 
        pHDB->Handle, pRQB, pRqbParam->ProviderID, pProvMngm->ProviderUsedCnt);

    // check provider remove parameter (ProviderID, State, ...)
    Response = EDDP_CRTCheckProviderRemoveParam (pDDB, pRqbParam);

    if (Response == EDD_STS_OK)
    {
        EDDP_ASSERT_FALSE(pRqbParam->ProviderID >= pDDB->CRT.CfgParam.ProviderMaxNumber);
        pProvider = &pProvMngm->pProvEntry[pRqbParam->ProviderID];

        if (pProvider->ProviderFlags & EDDP_CRT_PROV_FLAGS_IS_DFP)
        {
            /* -------------------------------------------- */
            /* DFP-Provider                                 */
            /* -------------------------------------------- */
            EDDP_CRT_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_CRTProviderRemove(): DFP-Provider: FrameID=0x%X, ProviderFlags=0x%X, PackGrId=%d, AcwDGId=%d, SFPosition=%d", 
                pHDB->Handle, pProvider->AddParam.FrameID, pProvider->ProviderFlags, pProvider->DfpParam.PackGrId, pProvider->DfpParam.AcwDGId, pProvider->DfpParam.SFPosition);

            if (pProvider->DfpParam.PackGrId == EDDP_CRT_CHAIN_IDX_END)
            {
                /* ---------------------------------------------------------------- */
                /* This DFP-Provider has no connected pack group.                   */
                /* Only remove provider from CRT management.                        */
                /* ---------------------------------------------------------------- */
                EDDP_CRTProviderRemoveFromCRTMngm (pDDB, pProvider->ProviderId);
            }
            else
            {
                /* ---------------------------------------------------------------- */
                /* This DFP-Provider has still connected pack group                 */
                /* so the ACW_DG of this provider must be passivated first.         */
                /* ---------------------------------------------------------------- */
                
                EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_SM_PTR_TYPE pStateMachine = EDDP_CRTGetProvAcwDGPassivateStateMachine(pDDB, pProvider->ProviderId, EDDP_CRTProvDfpAcwDGRemoveDoneCbf, pRQB);
                
                // start the Provider-ACW_DG-Passivate state machine for this ACW_DG                
                EDDP_CRTProviderAcwDGPassivateStateMachine (pDDB, pStateMachine, EDD_STS_OK);
                
                Response = EDD_STS_OK_PENDING;
            }
        }
        else
        {
            /* -------------------------------------------- */
            /* not DFP-Provider                             */
            /* -------------------------------------------- */
            EDDP_CRT_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_CRTProviderRemove(): FrameID=0x%X, ProviderFlags=0x%X, Provider-State=0x%X, AcwCnt=%d, AcwActCnt=%d", 
                pHDB->Handle, pProvider->AddParam.FrameID, pProvider->ProviderFlags, pProvider->State, pProvider->AcwCnt, pProvider->AcwActCnt);

            if (pProvider->AcwActCnt == 0)
            {
                /* ---------------------------------------------------------------- */
                /* This provider has no active ACWs                                 */
                /* so all ACWs of this provider are already removed from HW.        */
                /* Only remove provider from CRT management.                        */
                /* ---------------------------------------------------------------- */
                EDDP_CRTProviderRemoveFromCRTMngm (pDDB, pProvider->ProviderId);
            }
            else
            {
                EDDP_CMD_PARAM_PTR_TYPE pNewCmdParam = EDDP_CRTCmdGetInternalCmdParamElement(pDDB);
                /* ---------------------------------------------------------------- */
                /* This provider has active ACWs,                                   */
                /* but it is allowed to call the ACW_out command with               */
                /* set Active Bit.                                                  */
                /* ---------------------------------------------------------------- */
                pNewCmdParam->param_union.Crt.Ppm.ACW_InOut.ProviderId  = pProvider->ProviderId;
                pNewCmdParam->param_union.Crt.Ppm.ACW_InOut.AcwId       = EDDP_CRT_CHAIN_IDX_END;
                // call CmdRequest
                // if command is finished, the Cbf EDDP_CRTCmdDoneCbfPpm() will be called
                EDDP_CRTCmdRequestPpm (pDDB, EDDP_CRT_CMD_PPM_ACW_OUT, EDDP_CRT_CMD_ASYNCHRON, pNewCmdParam);
                // set Response for RQBHandler
                Response = EDD_STS_OK_PENDING;
            }
        }
    }

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTProviderRemove(): ProviderUsedCnt=%d, Response=0x%X", 
        pProvMngm->ProviderUsedCnt, Response);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTProviderControl                     +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
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
/*+  EDD_RQB_CSRT_PROVIDER_CONTROL_TYPE                                     +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_OK_PENDING                                        +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_RESOURCE                                      +*/
/*+               EDD_STS_ERR_SERVICE                                       +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Request to control a provider.                            +*/
/*+                                                                         +*/
/*+               Check provider is InUse. If no set EDD_STS_ERR_PARAM.     +*/
/*+                                                                         +*/
/*+               If change DataStatus is requested, update                 +*/
/*+               LocalDataStatus of this provider.                         +*/
/*+               If provider state is changed from ACTIVE to PASSIVE       +*/
/*+               the DataStatus will be changed after ACW_off command.     +*/
/*+                                                                         +*/
/*+               If AutoStop ADD/REMOVE is requested, evaluate             +*/
/*+               the AutoStopConsumerID in provider.                       +*/
/*+                 ADD    : Add this provider to AutoStop-GroupList.       +*/
/*+                          Check whether the AutoStop is already          +*/
/*+                          enabled by consumer. If yes, activate the      +*/
/*+                          ACW.Scoreboard_offset.                         +*/
/*+                 REMOVE : Remove this provider from AutoStop-GroupList   +*/
/*+                          and delete the ACW.Scoreboard_offset.          +*/
/*+                          If provider state is changed from ACTIVE       +*/
/*+                          to PASSIVE the ACW.Scoreboard_offset           +*/
/*+                          will be deleted after ACW_off command.         +*/
/*+               AutoStop will be enabled/disabled by consumer.            +*/
/*+                                                                         +*/
/*+               We ACTIVATE or PASSIVATE a provider.                      +*/
/*+                 MODE_ACTIVATE:                                          +*/
/*+                     - If RTClass3 provider, get PortInfo from PRM.      +*/
/*+                     - chain ACW in SW (AddToTBase -> AcwPhaseList)      +*/
/*+                     - init ACW of this provider in APICtrl-RAM          +*/
/*+                     - chain ACW in HW (ACW.ACW_nextoffset)              +*/
/*+                     - ACW_in command                                    +*/
/*+                     - in Cbf of ACW_in activate with ACW_on command     +*/
/*+                     - in Cbf of ACW_on send CONTROL-RQB back to user    +*/
/*+                 MODE_PASSIVATE:                                         +*/
/*+                     - deactivate ACW with ACW_off command               +*/
/*+                     - in Cbf of ACW_off chain ACW from AcwPhaseList     +*/
/*+                       in HW with ACW_out command                        +*/
/*+                     - in Cbf of ACW_out                                 +*/
/*+                         -> chain ACW from SW (RemoveFromTBase)          +*/
/*+                         -> delete ACW in APICtrl-RAM                    +*/
/*+                         -> send CONTROL-RQB back to user                +*/
/*+                 MODE_ACTIVATE_AUX                                       +*/
/*+                     - if this provider has AUX-ACW only this AUX-ACW    +*/
/*+                       will be added and activated                       +*/
/*+                 MODE_PASSIVATE_AUX                                      +*/
/*+                     - if this provider has AUX-ACW only this AUX-ACW    +*/
/*+                       will be passivate and removed from HW             +*/
/*+                                                                         +*/
/*+               Call command interface.                                   +*/
/*+               If command is finished the function                       +*/
/*+               EDDP_CRTCmdDoneCbfPpm() is called                         +*/
/*+               Set provider State to ACTIVE/INACTIVE                     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderControl(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP                                     Response;
    LSA_BOOL                                    ActCmdExecute;      // ACTIVATE is executed
    LSA_BOOL                                    PassCmdExecute;     // PASSIVATE is executed
    LSA_UINT16                                  AcwIdx, AcwId;
    LSA_UINT16                                  ACW_SB_offset;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE             pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE                  pProvider;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE              pAcw;
    EDDP_CRT_PROVIDER_ACW_DG_PTR_TYPE           pAcwDG;
    EDD_UPPER_CSRT_PROVIDER_CONTROL_PTR_TYPE    pRqbParam;
    EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_PTR_TYPE    pSubframeBlock;

    Response        = EDD_STS_OK;
    pProvMngm       = &pDDB->CRT.Prov;
    ActCmdExecute   = LSA_FALSE;
    PassCmdExecute  = LSA_FALSE;
    pRqbParam       = (EDD_UPPER_CSRT_PROVIDER_CONTROL_PTR_TYPE) pRQB->pParam;
    AcwId           = 0;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pHDB);
#endif

    EDDP_CRT_TRACE_08(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_CRTProviderControl(pRQB=0x%X, ProviderID=%d, Mode=0x%X, FrameID=0x%X, CRNumber=%d, SFPosition=%d, SFOffset=%d)",
        pHDB->Handle, pRQB, pRqbParam->ProviderID, pRqbParam->Mode, pRqbParam->FrameID, pRqbParam->IOParams.CRNumber, pRqbParam->IOParams.SFPosition, pRqbParam->IOParams.SFOffset);

    // clear DebugInfo block of this Control RQB
    EDDP_MEMSET_COMMON(pRqbParam->DebugInfo, 0, EDD_DEBUG_INFO_SIZE);

    // check provider control parameter (ProviderID, ...)
    Response = EDDP_CRTCheckProviderControlParam (pDDB, pRqbParam);

    if (Response == EDD_STS_OK)
    {
        EDDP_ASSERT_FALSE(pRqbParam->ProviderID >= pDDB->CRT.CfgParam.ProviderMaxNumber);
        pProvider = &pProvMngm->pProvEntry[pRqbParam->ProviderID];

        EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"[H:--] EDDP_CRTProviderControl(Mode=0x%X): ProviderID=%d, FrameID=0x%X, ProviderFlags=0x%X", 
            pRqbParam->Mode, pProvider->ProviderId, pProvider->AddParam.FrameID, pProvider->ProviderFlags);

        /* ---------------------------------------------------------------------------- */
        /* copy parameter for Activate only if Mode is SET_UNDEFINED_PARAMS or ACTIVATE */
        /* always before init ACW or ACW_DG!                                            */
        /* ---------------------------------------------------------------------------- */
        if (   (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_SET_UNDEFINED_PARAMS) 
            || ((pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE) || (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE_AUX)) 
           )
        {
            EDDP_CRTProviderIOParamUpdateForActivate (pDDB, pProvider->ProviderId, pRqbParam);
        }

        if (pProvider->ProviderFlags & EDDP_CRT_PROV_FLAGS_IS_DFP)
        {
            /* -------------------------------------------- */
            /* DFP-Provider                                 */
            /* -------------------------------------------- */
            EDDP_CRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"[H:--] EDDP_CRTProviderControl(): DFP-Provider: PackGrId=%d, AcwDGId=%d, SFPosition=%d, Current-AutoStopConsumerID=%d, RQB-AutoStopConsumerID=%d", 
                pProvider->DfpParam.PackGrId, pProvider->DfpParam.AcwDGId, pProvider->DfpParam.SFPosition, pProvider->AutoStopConsumerID, pRqbParam->AutoStopConsumerID);

            /* ---------------------------------------------------------------------------- */
            /* is Activate requested ?                                                      */
            /* ---------------------------------------------------------------------------- */
            if (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE)
            {
                EDDP_ASSERT_FALSE(pProvider->DfpParam.AcwDGId == EDDP_CRT_CHAIN_IDX_END);
                pAcwDG = &pProvMngm->pAcwDGEntry[pProvider->DfpParam.AcwDGId];

                // only if this ACW_DG is not yet active
                if (!(pAcwDG->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK))
                {                    
                    EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_SM_PTR_TYPE pStateMachine;
					pAcw = &pProvMngm->pAcwEntry[pAcwDG->AcwId];
                    EDDP_ASSERT_FALSE(pAcw->DfpParam.PrmSubframeBlockIdx == EDDP_CRT_CHAIN_IDX_END);
                    // active record buffer (A)
                    pSubframeBlock = &pDDB->PRM.pPDIRSubframeData_A->SubframeBlockArray[pAcw->DfpParam.PrmSubframeBlockIdx];
                    
                    // do the following steps only for Controller (for Device, this was already done at PrmCommit)
                    if (pSubframeBlock->SFIOCRProperties.DFPmode == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_DFPMODE_END_NODE)
                    {
                        // only if corresponding ACW is not yet active (i.e. this is first ACW_DG to be activated)
                        if (!(pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK))
                        {
                            EDDP_CRTProvDfpAcwAcwDGInitAndChainHwApi(pDDB, pAcwDG->AcwId, LSA_FALSE);

                            // if redundant FrameID(x+1) is present, also handle ACWRed1
                            if (EDDP_IS_IRT_REDUNDANT_FRAMEID(pProvMngm->pProvEntry[pProvider->ProviderId].AddParam.FrameID))
                            {
                                EDDP_CRTProvDfpAcwAcwDGInitAndChainHwApi(pDDB, pAcwDG->AcwIdRed1, LSA_TRUE);
                            }
                        }
                    }   

                    // complete data in ACW_DG
					pProvMngm->AcwHandler.FctDGInitHwApiFromProvider (pDDB, pProvider->ProviderId, pAcwDG->AcwDGId);

                    // check whether the AutoStop is already added
                    if (pProvider->AutoStopConsumerID != EDD_CONSUMER_ID_REMOVE_AS)
                    {
                        // for this provider is the AutoStop coupling already added
                        // check whether the AutoStop is already activated by consumer
                        if (EDDP_CRTConsIsProvAutoStopEnabled(pDDB, pProvider->AutoStopConsumerID))
                        {
                            // activate the ACW.Scoreboard_offset
                            ACW_SB_offset = EDDP_CRTGetShiftedTSBOffset (pDDB, pProvider->AutoStopConsumerID);
                            // write the "Scoreboard_offset" to all ACWs of this provider
                            pDDB->CRT.Prov.AcwHandler.FctWriteSBOffset (pDDB, pProvider->ProviderId, ACW_SB_offset);
                        }
                    }

                    /* get sm */
                    pStateMachine = EDDP_CRTGetProvAcwDGActivateStateMachine(pDDB, pProvider->ProviderId, EDDP_CRTProvDfpAcwDGControlDoneCbf, pRQB);
                    
                    // start the Provider-ACW_DG-Activate state machine for this ACW_DG
                    EDDP_CRTProviderAcwDGActivateStateMachine (pDDB, pStateMachine, EDD_STS_OK);

                    Response        = EDD_STS_OK_PENDING;
                    ActCmdExecute   = LSA_TRUE;
                }
                else
                {
                    // not executing the ACW_DG_on command - ACW_DG is already active
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProviderControl(): DFP-Provider: not executing the ACW_DG_on command - AcwDGId(%d) is already active", 
                        pAcwDG->AcwDGId);
                }
            }

            /* ---------------------------------------------------------------------------- */
            /* is Passivate requested ?                                                     */
            /* ---------------------------------------------------------------------------- */
            if (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_PASSIVATE)
            {
                // only if this provider is connected to a pack group
                if (pProvider->DfpParam.PackGrId == EDDP_CRT_CHAIN_IDX_END)
                {
                    // nothing to do
                    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"[H:--] EDDP_CRTProviderControl(): DFP-Provider: nothing to do - provider is not connected to a pack group");
                }
                else
                {
                    EDDP_ASSERT_FALSE(pProvider->DfpParam.AcwDGId == EDDP_CRT_CHAIN_IDX_END);
                    pAcwDG = &pProvMngm->pAcwDGEntry[pProvider->DfpParam.AcwDGId];

                    // only if this ACW_DG is still active
                    if (pAcwDG->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK)
                    {
                        EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_SM_PTR_TYPE pStateMachine = EDDP_CRTGetProvAcwDGPassivateStateMachine(pDDB, pProvider->ProviderId, EDDP_CRTProvDfpAcwDGControlDoneCbf, pRQB);
                        
                        // start the Provider-ACW_DG-Activate state machine for this ACW_DG
                        EDDP_CRTProviderAcwDGPassivateStateMachine (pDDB, pStateMachine, EDD_STS_OK);
                        
                        Response        = EDD_STS_OK_PENDING;
                        PassCmdExecute  = LSA_TRUE;
                    }
                    else
                    {
                        // not executing the ACW_DG_off command - ACW_DG is already passive
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProviderControl(): DFP-Provider: not executing the ACW_DG_off command - AcwDGId(%d) is already passive", 
                            pAcwDG->AcwDGId);
                    }
                }
            }
        }
        else
        {
            /* -------------------------------------------- */
            /* not DFP-Provider                             */
            /* -------------------------------------------- */
            EDDP_ASSERT_FALSE(pProvider->AcwList.BeginIdx == EDDP_CRT_CHAIN_IDX_END);
            pAcw = &pProvMngm->pAcwEntry[pProvider->AcwList.BeginIdx];

            EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"[H:--] EDDP_CRTProviderControl(): AcwCnt=%d, AcwActCnt=%d, AuxAcwId=%d, FirstAcw-State=0x%X", 
                pProvider->AcwCnt, pProvider->AcwActCnt, pProvider->AuxAcwId, pAcw->State);
            EDDP_CRT_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"[H:--] EDDP_CRTProviderControl(): Current-AutoStopConsumerID=%d, RQB-AutoStopConsumerID=%d, RR=%d, Ph=%d, PhSeq=0x%X, FSO=0x%X", 
                pProvider->AutoStopConsumerID,      pRqbParam->AutoStopConsumerID, 
                pAcw->AddParam.CycleReductionRatio, pAcw->AddParam.CyclePhase, 
                pAcw->AddParam.CyclePhaseSequence,  pAcw->AddParam.FrameSendOffset);

            /* ---------------------------------------------------------------------------- */
            /* is Activate requested ?                                                      */
            /* ---------------------------------------------------------------------------- */
            if ((pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE) || (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE_AUX))
            {
                if ((pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE) && (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE_AUX))
                {
                    /* ------------------------------------------------ */
                    /* MODE_ACTIVATE and MODE_ACTIVATE_AUX              */
                    /* all ACWs are activated                           */
                    /*  -> only if these ACWs are not yet active        */
                    /* ------------------------------------------------ */
                    /* init all ACWs of this provider in APICtrl-RAM    */
                    /* add all ACWs of this provider to TBase tree      */
                    /* ------------------------------------------------ */
                    AcwIdx = pProvider->AcwList.BeginIdx;
                    while (AcwIdx != EDDP_CRT_CHAIN_IDX_END)
                    {
                        EDDP_ASSERT_FALSE(AcwIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);
                        pAcw = &pProvMngm->pAcwEntry[AcwIdx];
                        // only if this ACW is not yet active
                        if (!(pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK))
                        {
                            // init ACW in APICtrl-RAM
                            pDDB->CRT.Prov.AcwHandler.FctInitHwApiRTFrame (pDDB, pProvider->ProviderId, pAcw->AcwId);
                            // chain this ACW in AcwPhaseList
                            EDDP_CRTProvAcwAddToTBase (pDDB, pAcw->AcwId, LSA_FALSE);
                            // execute ACW_in command
                            ActCmdExecute = LSA_TRUE;
                        }
                        // next AcwIdx in this provider
                        AcwIdx = pProvMngm->pAcwEntry[AcwIdx].AcwProvLink.Next;
                    }

                    /* ------------------------------------------------ */
                    /* chain all ACWs of this provider in HW            */
                    /*  -> update ACW.ACW_nextoffset                    */
                    /*  -> only after all ACWs are available in HW      */
                    /* ------------------------------------------------ */
                    if (ActCmdExecute)
                    {
                        AcwIdx = pProvider->AcwList.BeginIdx;
                        while (AcwIdx != EDDP_CRT_CHAIN_IDX_END)
                        {
                            EDDP_ASSERT_FALSE(AcwIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);
                            pAcw = &pProvMngm->pAcwEntry[AcwIdx];
                            // only if this ACW is not yet active
                            if (!(pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK))
                            {
                                // chain ACW in HW (update ACW.ACW_nextoffset)
                                pDDB->CRT.Prov.AcwHandler.FctUpdateAcwNextOffsetAcwHwApi (pDDB, pAcw->AcwId);
                            }
                            // next AcwIdx in this provider
                            AcwIdx = pProvMngm->pAcwEntry[AcwIdx].AcwProvLink.Next;
                        }
                        // AcwId for CMD-Interface
                        AcwId = EDDP_CRT_CHAIN_IDX_END;
                    }
                }
                else
                {
                    /* ------------------------------------------------ */
                    /* MODE_ACTIVATE or MODE_ACTIVATE_AUX               */
                    /* ------------------------------------------------ */
                    if (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE)
                    {
                        /* ------------------------------------------------ */
                        /* MODE_ACTIVATE                                    */
                        /* all ACWs unless AUX are activated                */
                        /*  -> only if these ACWs are not yet active        */
                        /* ------------------------------------------------ */
                        /* init ACWs of this provider in APICtrl-RAM        */
                        /* add ACWs of this provider to TBase tree          */
                        /* ------------------------------------------------ */
                        AcwIdx = pProvider->AcwList.BeginIdx;
                        while (AcwIdx != EDDP_CRT_CHAIN_IDX_END)
                        {
                            EDDP_ASSERT_FALSE(AcwIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);
                            pAcw = &pProvMngm->pAcwEntry[AcwIdx];
                            // not for AUX-ACW and only if this ACW is not yet active
                            if ((pAcw->AcwId != pProvider->AuxAcwId) && (!(pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK)))
                            {
                                // init ACW in APICtrl-RAM
                                pDDB->CRT.Prov.AcwHandler.FctInitHwApiRTFrame (pDDB, pProvider->ProviderId, pAcw->AcwId);
                                // chain this ACW in AcwPhaseList
                                EDDP_CRTProvAcwAddToTBase (pDDB, pAcw->AcwId, LSA_FALSE);
                                // execute ACW_in command
                                ActCmdExecute = LSA_TRUE;
                            }
                            // next AcwIdx in this provider
                            AcwIdx = pProvMngm->pAcwEntry[AcwIdx].AcwProvLink.Next;
                        }

                        /* ------------------------------------------------ */
                        /* chain ACWs of this provider in HW                */
                        /*  -> update ACW.ACW_nextoffset                    */
                        /*  -> only after all ACWs are available in HW      */
                        /* ------------------------------------------------ */
                        if (ActCmdExecute)
                        {
                            AcwIdx = pProvider->AcwList.BeginIdx;
                            while (AcwIdx != EDDP_CRT_CHAIN_IDX_END)
                            {
                                EDDP_ASSERT_FALSE(AcwIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);
                                pAcw = &pProvMngm->pAcwEntry[AcwIdx];
                                // not for AUX-ACW and only if this ACW is not yet active
                                if ((pAcw->AcwId != pProvider->AuxAcwId) && (!(pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK)))
                                {
                                    // chain ACW in HW (update ACW.ACW_nextoffset)
                                    pDDB->CRT.Prov.AcwHandler.FctUpdateAcwNextOffsetAcwHwApi (pDDB, pAcw->AcwId);
                                }
                                // next AcwIdx in this provider
                                AcwIdx = pProvMngm->pAcwEntry[AcwIdx].AcwProvLink.Next;
                            }
                            // AcwId for CMD-Interface
                            if (pProvider->AuxAcwId == EDDP_CRT_CHAIN_IDX_END)
                                // all ACWs
                                AcwId = EDDP_CRT_CHAIN_IDX_END;
                            else
                                // for RTC3-LEGACY-Provider only the first ACW (this is the main ACW)
                                AcwId = pProvider->AcwList.BeginIdx;
                        }
                    }
                    else
                    {
                        /* ------------------------------------------------ */
                        /* MODE_ACTIVATE_AUX                                */
                        /* only AUX-ACW is activated                        */
                        /*  -> only if this ACW is not yet active           */
                        /* ------------------------------------------------ */
                        // check of (AuxAcwId != EDDP_CRT_CHAIN_IDX_END) is already done in EDDP_CRTCheckProviderControlParam()
                        pAcw = &pProvMngm->pAcwEntry[pProvider->AuxAcwId];
                        // only if this ACW is not yet active
                        if (!(pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK))
                        {
                            // init ACW in APICtrl-RAM
                            pDDB->CRT.Prov.AcwHandler.FctInitHwApiRTFrame (pDDB, pProvider->ProviderId, pAcw->AcwId);
                            // chain this ACW in AcwPhaseList
                            EDDP_CRTProvAcwAddToTBase (pDDB, pAcw->AcwId, LSA_FALSE);
                            // chain this ACW in HW (update ACW.ACW_nextoffset)
                            pDDB->CRT.Prov.AcwHandler.FctUpdateAcwNextOffsetAcwHwApi(pDDB, pAcw->AcwId);
                            // AcwId for CMD-Interface
                            AcwId = pAcw->AcwId;
                            // execute ACW_in command
                            ActCmdExecute = LSA_TRUE;
                        }
                    }
                }

                /* -------------------------------- */
                /* execute the ACW_in command ?     */
                /* -------------------------------- */
                if (ActCmdExecute)
                {
                    EDDP_CMD_PARAM_PTR_TYPE pNewCmdParam = EDDP_CRTCmdGetInternalCmdParamElement(pDDB);
                    
                    /* ------------------------------------------------ */
                    /* check whether the AutoStop is already activated  */
                    /* ------------------------------------------------ */
                    if (pProvider->AutoStopConsumerID != EDD_CONSUMER_ID_REMOVE_AS)
                    {
                        // for this provider is the AutoStop coupling already added
                        // check whether the AutoStop is already activated by consumer
                        if (EDDP_CRTConsIsProvAutoStopEnabled(pDDB, pProvider->AutoStopConsumerID))
                        {
                            // activate the ACW.Scoreboard_offset
                            ACW_SB_offset = EDDP_CRTGetShiftedTSBOffset (pDDB, pProvider->AutoStopConsumerID);
                            // write the "Scoreboard_offset" to all ACWs of this provider
                            pDDB->CRT.Prov.AcwHandler.FctWriteSBOffset (pDDB, pProvider->ProviderId, ACW_SB_offset);
                        }
                    }

                    /* ------------------------------------------------ */
                    /* ACW_in command                                   */
                    /* ------------------------------------------------ */
                    // remember data for CMD-Interface
                    pNewCmdParam->param_union.Crt.Ppm.ACW_InOut.ProviderId = pProvider->ProviderId;
                    pNewCmdParam->param_union.Crt.Ppm.ACW_InOut.AcwId      = AcwId;
                    // call CmdRequest
                    // if command is finished, the Cbf EDDP_CRTCmdDoneCbfPpm() will be called
                    EDDP_CRTCmdRequestPpm (pDDB, EDDP_CRT_CMD_PPM_ACW_IN, EDDP_CRT_CMD_ASYNCHRON, pNewCmdParam);
                    // set Response for RQBHandler
                    Response = EDD_STS_OK_PENDING;
                }
                else
                {
                    // not executing the ACW_in command - provider (ACW) is already active
                    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProviderControl(): not executing the ACW_in command - provider (ACW) is already active");
                }
            }

            /* ---------------------------------------------------------------------------- */
            /* is Passivate requested ?                                                     */
            /* ---------------------------------------------------------------------------- */
            if ((pRqbParam->Mode & EDD_PROV_CONTROL_MODE_PASSIVATE) || (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_PASSIVATE_AUX))
            {
                if (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_PASSIVATE)
                {
                    /* ------------------------------------------------ */
                    /* MODE_PASSIVATE passivates all ACWs               */
                    /*  -> only if this provider has active ACWs        */
                    /* ------------------------------------------------ */
                    if (pProvider->AcwActCnt > 0)
                    {
                        // AcwId for CMD-Interface
                        AcwId = EDDP_CRT_CHAIN_IDX_END;
                        // execute ACW_off command
                        PassCmdExecute = LSA_TRUE;
                    }
                }
                else
                {
                    if (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_PASSIVATE_AUX)
                    {
                        /* ------------------------------------------------ */
                        /* MODE_PASSIVATE_AUX                               */
                        /* only AUX-ACW is passivated                       */
                        /*  -> only if this ACW is not yet passive          */
                        /* ------------------------------------------------ */
                        // check of (AuxAcwId != EDDP_CRT_CHAIN_IDX_END) is already done in EDDP_CRTCheckProviderControlParam()
                        pAcw = &pProvMngm->pAcwEntry[pProvider->AuxAcwId];
                        // passivate only if this ACW is current active
                        if (pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK)
                        {
                            // AcwId for CMD-Interface
                            AcwId = pAcw->AcwId;
                            // execute ACW_off command
                            PassCmdExecute = LSA_TRUE;
                        }
                    }
                }

                /* -------------------------------- */
                /* execute the ACW_out command ?    */
                /* -------------------------------- */
                if (PassCmdExecute)
                {
                    EDDP_CMD_PARAM_PTR_TYPE pNewCmdParam = EDDP_CRTCmdGetInternalCmdParamElement(pDDB);
                                        
                    // remember data for CMD-Interface
                    pNewCmdParam->param_union.Crt.Ppm.ACW_InOut.ProviderId = pProvider->ProviderId;
                    pNewCmdParam->param_union.Crt.Ppm.ACW_InOut.AcwId      = AcwId;
                    // call CmdRequest
                    // if command is finished, the Cbf EDDP_CRTCmdDoneCbfPpm() will be called
                    EDDP_CRTCmdRequestPpm (pDDB, EDDP_CRT_CMD_PPM_ACW_OUT, EDDP_CRT_CMD_ASYNCHRON, pNewCmdParam);
                    // set Response for RQBHandler
                    Response = EDD_STS_OK_PENDING;
                }
                else
                {
                    // not executing the ACW_out command - provider (ACW) is already passive
                    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProviderControl(): not executing the ACW_out command - provider (ACW) is already passive");
                }
            }
        }

        /* ---------------------------------------------------------------------------- */
        /* is change DataStatus requested ?                                             */
        /* ---------------------------------------------------------------------------- */
        if (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_STATUS)
        {
            // check whether DataStatus can be changed here
            if (PassCmdExecute)
            {
                // provider is passivated in this request
                // in this case the DataStatus will be changed after ACW_off command
            }
            else
            {
                // change provider DataStatus
                EDDP_CRTProviderChangeDataStatus (pDDB, pProvider->ProviderId, pRqbParam->Status, pRqbParam->Mask);
            }
        }

        /* ---------------------------------------------------------------------------- */
        /* is AutoStop ADD/REMOVE requested ?                                           */
        /* ---------------------------------------------------------------------------- */
        if (   (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_AUTOSTOP)
            && (pRqbParam->AutoStopConsumerID != pProvider->AutoStopConsumerID)
           )
        {
            if (pRqbParam->AutoStopConsumerID == EDD_CONSUMER_ID_REMOVE_AS)
            {
                // remove this provider from AutoStop
                EDDP_CRTProviderRemoveFromASGroupList (pDDB, pProvider->ProviderId);
                // check whether the ACW.Scoreboard_offset can be deleted here
                if (PassCmdExecute)
                {
                    // provider is passivated in this request
                    // in this case the ACW.Scoreboard_offset will be deleted after ACW_off command
                }
                else
                {
                    // delete the ACW.Scoreboard_offset in all ACWs of this provider
                    // note: distinction between PNIP_REV2 and PNIP_REV3 will be done via mask value in the function
                    pDDB->CRT.Prov.AcwHandler.FctWriteSBOffset (pDDB, pProvider->ProviderId, PNIP_R2_ACW_SND_WORD3_SCOREBOARD_OFFSET_OFF_VALUE);
                }
            }
            else
            {
                // add this provider to AutoStop
                EDDP_CRTProviderAddToASGroupList (pDDB, pProvider->ProviderId, pRqbParam->AutoStopConsumerID);
                // check whether the AutoStop is already activated by consumer
                if (EDDP_CRTConsIsProvAutoStopEnabled(pDDB, pRqbParam->AutoStopConsumerID))
                {
                    // activate the ACW.Scoreboard_offset
                    ACW_SB_offset = EDDP_CRTGetShiftedTSBOffset (pDDB, pRqbParam->AutoStopConsumerID);
                    // write the "Scoreboard_offset" to all ACWs of this provider
                    pDDB->CRT.Prov.AcwHandler.FctWriteSBOffset (pDDB, pProvider->ProviderId, ACW_SB_offset);
                }
            }
        }
    }

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTProviderControl(): Response=0x%X", Response);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProviderAddToCRTMngm                                +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB                +*/
/*+                             EDD_UPPER_CSRT_PROVIDER_ADD_PTR_TYPE    pRqbParam           +*/
/*+                             LSA_UINT8                               ProviderAcwPhase    +*/
/*+                                                                                         +*/
/*+  Result                :    EDD_RSP                                                     +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  pRqbParam          : pointer to RQB parameter from user (UPPER-RQB)                    +*/
/*+  ProviderAcwPhase   : Phase - Red, Orange, Green                                        +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Add new provider to CRT management.                                       +*/
/*+               The entry is treated as free if its Status == EDDP_CRT_STATE_FREE         +*/
/*+               The Provider-Status will be changed to EDDP_CRT_STATE_SW_USED             +*/
/*+               in this function.                                                         +*/
/*+               If ACW list is not full but provider list is full -> FATAL                +*/
/*+                                                                                         +*/
/*+               - Add new provider to ProvUsedList                                        +*/
/*+               - Copy Provider-Add-Parameter                                             +*/
/*+               - Add new DataStatus to DataStatusUsedList                                +*/
/*+               - Add new ACW(s) to AcwUsedList and to provider                           +*/
/*+               - Add provider to pProvGroupEntry                                         +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderAddToCRTMngm(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
	EDD_UPPER_CSRT_PROVIDER_ADD_PTR_TYPE    pRqbParam,
	LSA_UINT8                               ProviderAcwPhase)
{
    EDD_RSP                                 Response;
    LSA_UINT32                              iAcw, iDstAdr;
    LSA_UINT16                              NewProviderID, NewAcwID, NewDataStatusId;
    LSA_UINT32                              AcwCnt;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
//  EDDP_CRT_PROVIDER_DDB_MNGM_PTR_TYPE     pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE              pProvider;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;
    EDDP_CRT_PROVIDER_DS_PTR_TYPE           pDataStatus;

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProviderAddToCRTMngm(pRqbParam=0x%X, ProviderAcwPhase=0x%X)", 
        pRqbParam, ProviderAcwPhase);

    Response     = EDD_STS_OK;
    pProvMngm    = &pDDB->CRT.Prov;
    pProvMngm    = &pDDB->CRT.Prov;
    AcwCnt       = EDDP_CRT_PROV_ADD_NUMBER_OF_ACW_DEFAULT;

    /* ------------------------------------------------------------------------ */
    /* add new provider to CRT.Prov.pProvEntry[NewProviderID]                   */
    /* ------------------------------------------------------------------------ */
    NewProviderID = EDDP_CRTProviderAddToUsedList (pDDB);   // get NewProviderID

    if (NewProviderID == EDDP_CRT_CHAIN_IDX_END)
    {
        // this should have been stated in EDDP_CRTCheckProviderAddParam()
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderAddToCRTMngm(): FATAL -> No more free provider! ProviderUsedCnt=%d", 
            pProvMngm->ProviderUsedCnt);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        EDDP_CRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProviderAddToCRTMngm(): NewProviderID=%d, FrameID=0x%X, DataLen=%d, ProvProperties=0x%X, CRNumber=%d", 
            NewProviderID, pRqbParam->FrameID, pRqbParam->DataLen, pRqbParam->Properties, pRqbParam->IOParams.CRNumber);

        pProvider = &pProvMngm->pProvEntry[NewProviderID];

        /* ------------------------------------------------------------------------ */
        /* copy parameter to management structure                                   */
        /* ------------------------------------------------------------------------ */
        pRqbParam->ProviderID                       = NewProviderID;    // new ProviderID for upper user
        pProvider->State                            = EDDP_CRT_STATE_SW_USED;
        pProvider->ProviderFlags                    = 0;

        pProvider->ROGPhase                         = ProviderAcwPhase;
        if (EDD_CSRT_PROV_PROP_RTCLASS_UDP == (pRqbParam->Properties & EDD_CSRT_PROV_PROP_RTCLASS_MASK))
            pProvider->RTClass                      = EDDP_CRT_RTCLASS_UDP;
        else
            pProvider->RTClass                      = EDDP_CRT_RTCLASS_RT;
        pProvider->AutoStopConsumerID               = EDD_CONSUMER_ID_REMOVE_AS;
        pProvider->AuxAcwId                         = EDDP_CRT_CHAIN_IDX_END;
        /* AddParam */
        pProvider->AddParam.FrameID                 = pRqbParam->FrameID;
        pProvider->AddParam.Properties              = pRqbParam->Properties;
        for ( iDstAdr=0; iDstAdr < EDD_IP_ADDR_SIZE; iDstAdr++ )
        {
            pProvider->AddParam.DstIP.IPAdr[iDstAdr] = 0;
        }
        pProvider->AddParam.DataLen                 = pRqbParam->DataLen;
        pProvider->AddParam.BufferProperties        = pRqbParam->IOParams.BufferProperties;
        pProvider->AddParam.CRNumber                = pRqbParam->IOParams.CRNumber;
        pProvider->AddParam.AppGroup                = pRqbParam->IOParams.AppGroup;
        pProvider->AddParam.DataOffset              = pRqbParam->IOParams.DataOffset;
        /* DfpParam */
        pProvider->DfpParam.PackGrId                = EDDP_CRT_CHAIN_IDX_END;
        pProvider->DfpParam.SFPosition              = pRqbParam->IOParams.SFPosition;
        pProvider->DfpParam.SFOffset                = pRqbParam->IOParams.SFOffset;
        pProvider->DfpParam.AcwDGId                 = EDDP_CRT_CHAIN_IDX_END;
        /* DfpParamCheck */
        pProvider->DfpParamCheck.CycleReductionRatio = pRqbParam->CycleReductionRatio;
        pProvider->DfpParamCheck.CyclePhase          = pRqbParam->CyclePhase;
        pProvider->DfpParamCheck.DataOffset          = pRqbParam->IOParams.DataOffset; // only relevant for HERA

        /* ------------------------------------------------------------------------ */
        /* get and init new DataStatus entry                                        */
        /* ------------------------------------------------------------------------ */
        NewDataStatusId = EDDP_CRTDataStatusAddToUsedList (pDDB);

		if (NewDataStatusId == EDDP_CRT_CHAIN_IDX_END)
        {
            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderAddToCRTMngm(): FATAL -> No more DataStatus entries! ProviderUsedCnt=%d, AcwUsedCnt=%d", 
                pProvMngm->ProviderUsedCnt, pProvMngm->AcwUsedCnt);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        else
        {
            // if no DFP    : this is the DataStatus in APDU-Status of frame
            // if DFP       : this is the DataStatus in DataGramm
            pDataStatus = &pProvMngm->pDSEntry[NewDataStatusId];

            // switch to current PNIP revision
            switch (EDDP_HALGetPnipRevision(pDDB))
            {
                #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
                case EDDP_HAL_HW_PNIP_REV1:
                {
                    pDataStatus->LocalDataStatus    = EDD_CSRT_DSTAT_LOCAL_INIT_VALUE;
                    pDataStatus->DS_Selector        = (LSA_UINT8) pRqbParam->GroupID;
                    pDataStatus->pHwDSApiPnipR1     = &pProvMngm->pHwBaseDSApiPnipR1[NewDataStatusId];
                    pProvider->DataStatusId         = NewDataStatusId;

                    // set LocalDataStatus for System-Redundancy-Provider
                    if (pRqbParam->Properties & EDD_CSRT_PROV_PROP_SYSRED)
                    {
                        // delete Bit(State): set it to BACKUP
                        EDDP_SET_BIT_VALUE16(pDataStatus->LocalDataStatus, 0, EDD_CSRT_DSTAT_BIT_STATE, 0)
                    }
                    // This is the first time the value is written, so EDDP_ENTER_IO() and EDDP_EXIT_IO() are not necassary here to lock the memory
                    // write the values to APICtrl-RAM
                    EDDP_HAL_MEM8_WRITE (pDDB, &pDataStatus->pHwDSApiPnipR1->DataStatus,  pDataStatus->LocalDataStatus);
                    EDDP_HAL_MEM8_WRITE (pDDB, &pDataStatus->pHwDSApiPnipR1->DS_Selector, pDataStatus->DS_Selector);

                    // DataStatusOffset for upper user (offset to begin of the PNIP-RAM)
                    pRqbParam->IOParams.DataStatusOffset = EDDP_HALGetPnipOffsetToStart (pDDB, (EDD_COMMON_MEM_PTR_TYPE)(&pDataStatus->pHwDSApiPnipR1->DataStatus));
                }
                break;
                #endif

                #if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
                case EDDP_HAL_HW_PNIP_REV2:
                case EDDP_HAL_HW_PNIP_REV3:
                {
                    pDataStatus->LocalDataStatus    = EDD_CSRT_DSTAT_LOCAL_INIT_VALUE;
                    pDataStatus->DS_Selector        = (LSA_UINT8) pRqbParam->GroupID;
                    pDataStatus->pHwDSApiPnipR2     = &pProvMngm->pHwBaseDSApiPnipR2[NewDataStatusId];
                    pProvider->DataStatusId         = NewDataStatusId;

                    // set LocalDataStatus for System-Redundancy-Provider
                    if (pRqbParam->Properties & EDD_CSRT_PROV_PROP_SYSRED)
                    {
                        // delete Bit(State): set it to BACKUP
                        EDDP_SET_BIT_VALUE16(pDataStatus->LocalDataStatus, 0, EDD_CSRT_DSTAT_BIT_STATE, 0)
                        if( EDDP_HALGetPnipRevision(pDDB) == EDDP_HAL_HW_PNIP_REV3 )
                        {
                          // write the values to APICtrl-RAM
                          EDDP_HAL_MEM8_WRITE (pDDB, &pDataStatus->pHwDSApiPnipR2->DataStatus_Saved, pDataStatus->LocalDataStatus);
                        }
    
                    }
                    // This is the first time the value is written, so EDDP_ENTER_IO() and EDDP_EXIT_IO() are not necassary here to lock the memory
                    // write the values to APICtrl-RAM
                    EDDP_HAL_MEM8_WRITE (pDDB, &pDataStatus->pHwDSApiPnipR2->DataStatus,  pDataStatus->LocalDataStatus);
                    EDDP_HAL_MEM8_WRITE (pDDB, &pDataStatus->pHwDSApiPnipR2->DS_Selector, pDataStatus->DS_Selector);

                    // DataStatusOffset for upper user (offset to begin of the PNIP-RAM)
                    pRqbParam->IOParams.DataStatusOffset = EDDP_HALGetPnipOffsetToStart (pDDB, (EDD_COMMON_MEM_PTR_TYPE)(&pDataStatus->pHwDSApiPnipR2->DataStatus));
                }
                break;
                #endif

                default:
                {
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderAddToCRTMngm(): FATAL -> Version(%d) of PNIP is unknown", 
                        EDDP_HALGetPnipRevision(pDDB));
                    EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                }
                break;
            }

        }

        /* ------------------------------------------------------------------------ */
        /* additional qualities of all provider                                     */
        /* ------------------------------------------------------------------------ */
        // is this provider used with a MultiCast-CR ?
        if (pRqbParam->Properties & EDD_CSRT_PROV_PROP_MODE_MC_CR)
        {
            pProvider->ProviderFlags |= EDDP_CRT_PROV_FLAGS_IS_MCCR;
        }

        // is this System-Redundancy-Provider ?
        if (pRqbParam->Properties & EDD_CSRT_PROV_PROP_SYSRED)
        {
            pProvider->ProviderFlags |= EDDP_CRT_PROV_FLAGS_IS_SYSRED;
        }

        /* ------------------------------------------------------------------------ */
        /* additional qualities of RTC3 provider                                    */
        /* ------------------------------------------------------------------------ */
        if (ProviderAcwPhase == EDDP_CRT_PROV_ACW_PHASE_R)
        {
            if (pRqbParam->Properties & EDD_CSRT_PROV_PROP_RTC3_MODE_LEGACY)
            {
                AcwCnt = EDDP_CRT_PROV_ADD_NUMBER_OF_ACW_RTC3_LEGACY;
            }
            else
            {
                // is this Redundant FrameID ?
                if (EDDP_IS_IRT_REDUNDANT_FRAMEID(pRqbParam->FrameID))
                {
                    pProvider->ProviderFlags |= EDDP_CRT_PROV_FLAGS_IS_REDUNDANT;
                    AcwCnt = EDDP_CRT_PROV_ADD_NUMBER_OF_ACW_RTC3_MRPD;
                }

                // is this DFP-Provider ?
                // check it always at the end because the AcwCnt is set to 0
                if (pRqbParam->Properties & EDD_CSRT_PROV_PROP_PDU_FORMAT_DFP)
                {
                    // DFP provider will be connected to a pack group in PackGroupUsedList on Activate
                    pProvider->ProviderFlags |= EDDP_CRT_PROV_FLAGS_IS_DFP;
                    AcwCnt = EDDP_CRT_PROV_ADD_NUMBER_OF_ACW_RTC3_DFP;

                    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProviderAddToCRTMngm(): DFP-Provider: FrameID=0x%X, SFPosition=%d", 
                        pProvider->AddParam.FrameID, pProvider->DfpParam.SFPosition);
                }
            }
        }

		/* ------------------------------------------------------------------------------------------------ */
		/*	Check if the number of ACWs used for this command is less then EDDP_MAX_ACW_CNT_PER_CMD         */
		/*	Note that the arrays used to set ACWs for this command are limited to EDDP_MAX_ACW_CNT_PER_CMD  */
		/* ------------------------------------------------------------------------------------------------ */
		if (AcwCnt >= EDDP_CMD_MAX_ACW_CNT_PER_CMD)
		{
			EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderAddToCRTMngm(): FATAL -> AcwCnt(%d) > AcwMaxNr(%d) per command!", 
                AcwCnt, EDDP_CMD_MAX_ACW_CNT_PER_CMD);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
		}

		/* ------------------------------------------------------------------------ */
        /* add new ACW to AcwUsedList                                               */
        /* add new ACW to provider                                                  */
        /* copy ACW specific parameter                                              */
        /* ------------------------------------------------------------------------ */
        for ( iAcw=0; iAcw < AcwCnt; iAcw++ )
        {
            NewAcwID = EDDP_CRTProvAcwAddToUsedList (pDDB);     // get NewAcwID

            if (NewAcwID == EDDP_CRT_CHAIN_IDX_END)
            {
                // this should have been stated in EDDP_CRTCheckProviderAddParam()
                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderAddToCRTMngm(): FATAL -> No more free ACW! ProviderUsedCnt=%d, AcwUsedCnt=%d", 
                    pProvMngm->ProviderUsedCnt, pProvMngm->AcwUsedCnt);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            else
            {
                pAcw = &pProvMngm->pAcwEntry[NewAcwID];

                /* ---------------------------------------- */
                /* add ACW to provider                      */
                /* ---------------------------------------- */
                EDDP_CRTProvAcwAddToProvider (pDDB, NewProviderID, NewAcwID);

                /* ---------------------------------------- */
                /* copy ACW specific parameter              */
                /* ---------------------------------------- */
                pAcw->ROGPhase                      = ProviderAcwPhase;
                pAcw->DataStatusId                  = EDDP_CRT_CHAIN_IDX_END;   // only for pack frame
                /* AddParam */
                pAcw->AddParam.FrameID              = pRqbParam->FrameID;
                for ( iDstAdr=0; iDstAdr < EDD_MAC_ADDR_SIZE; iDstAdr++ )
                {
                    pAcw->AddParam.DstMAC.MacAdr[iDstAdr] = 0;
                }
                pAcw->AddParam.CycleReductionRatio  = pRqbParam->CycleReductionRatio;
                pAcw->AddParam.CyclePhase           = pRqbParam->CyclePhase;
                pAcw->AddParam.CyclePhaseSequence   = pRqbParam->CyclePhaseSequence;
                pAcw->AddParam.TxPortID             = EDD_PORT_ID_AUTO;
                pAcw->AddParam.FrameSendOffset      = EDDP_CRT_FRAME_SEND_OFFSET_UNDEFINED;
                /* DfpParam */
                pAcw->DfpParam.PrmSubframeBlockIdx  = EDDP_CRT_CHAIN_IDX_END;
                pAcw->DfpParam.PackGrId             = EDDP_CRT_CHAIN_IDX_END;
                pAcw->DfpParam.ACW_DG_order         = EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SFIOCRPROPERTIES_DFPREDUNDANTPATHLAYOUT_HIGH;

                /* ---------------------------------------- */
                /* if more ACWs for RTClass3 provider       */
                /* ---------------------------------------- */
                if ((ProviderAcwPhase == EDDP_CRT_PROV_ACW_PHASE_R) && (iAcw > 0))
                {
                    // is this RTC3-LEGACY-Provider and is this last ACW of this provider ?
                    if ((pRqbParam->Properties & EDD_CSRT_PROV_PROP_RTC3_MODE_LEGACY) && (iAcw == (AcwCnt-1)))
                    {
                        // this ACW is AUX-ACW
                        pProvider->AuxAcwId                 = NewAcwID;
                        pAcw->ROGPhase                      = EDDP_CRT_PROV_ACW_PHASE_G;
                        pAcw->AddParam.CyclePhaseSequence   = 0;
                        // CycleReductionRatio (128) is only for even CycleBaseFactor
                        if ( EDDP_CRTCheckIsCycleBaseFactorEven(pDDB, pDDB->SWI.CycleInfo.CycleBaseFactor) )
                            pAcw->AddParam.CycleReductionRatio = EDDP_CRT_REDUCTION_128;
                    }
                }

                EDDP_CRT_TRACE_07(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProviderAddToCRTMngm(): NewAcwID=%d, FrameID=0x%X, RR=%d, Ph=%d, PhSeq=0x%X, FSO=0x%X, AuxAcwId=%d", 
                    NewAcwID,                           pAcw->AddParam.FrameID, 
                    pAcw->AddParam.CycleReductionRatio, pAcw->AddParam.CyclePhase, 
                    pAcw->AddParam.CyclePhaseSequence,  pAcw->AddParam.FrameSendOffset, 
                    pProvider->AuxAcwId);
            }
        }

        /* ------------------------------------------------------------------------ */
        /* add new provider to CRT.Prov.pProvGroupEntry[GroupId].GroupList          */
        /* ------------------------------------------------------------------------ */
        EDDP_CRTProviderAddToGroupList (pDDB, NewProviderID);
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProviderRemoveFromCRTMngm                           +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             LSA_UINT16                          ProviderID              +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ProviderID         : Provider to be removed                                            +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Remove one provider from CRT management.                                  +*/
/*+                                                                                         +*/
/*+               If no provider in CRT management on this interface -> FATAL!              +*/
/*+                                                                                         +*/
/*+               These cases are not allowed for removing:                                 +*/
/*+                 STATE_FREE      : provider is not used in CRT                           +*/
/*+                 AcwActCnt > 0   : provider has active ACWs in HW                        +*/
/*+                 DFP-Provider    : is still connected to a pack group                    +*/
/*+               -> FATAL!                                                                 +*/
/*+                                                                                         +*/
/*+               Remove provider from AutoStop                                             +*/
/*+               Remove provider from GroupList                                            +*/
/*+               Delete DataStatus of this provider in APICtrl-RAM                         +*/
/*+               Remove DataStatus from UsedList                                           +*/
/*+               Remove all ACWs from this provider                                        +*/
/*+               Remove these ACWs from AcwUsedList                                        +*/
/*+               Remove provider from ProvUsedList                                         +*/
/*+                                                                                         +*/
/*+               The Provider-State will be changed to EDDP_CRT_STATE_FREE                 +*/
/*                in this function.                                                         +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderRemoveFromCRTMngm(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID)
{
    LSA_UINT16                              AcwIdx;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
//  EDDP_CRT_PROVIDER_DDB_MNGM_PTR_TYPE     pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE              pProvider;

    EDDP_ASSERT_FALSE(ProviderID >= pDDB->CRT.CfgParam.ProviderMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pProvMngm    = &pDDB->CRT.Prov;
    pProvider    = &pProvMngm->pProvEntry[ProviderID];

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CRTProviderRemoveFromCRTMngm(ProviderID=%d): ProviderUsedCnt=%d", 
        ProviderID, pProvMngm->ProviderUsedCnt);

    EDDP_CRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProviderRemoveFromCRTMngm(ProviderID=%d): ProviderUsedCnt=%d, Current-AutoStopConsumerID=%d, AcwCnt=%d, PackGrId=%d", 
        ProviderID, pProvMngm->ProviderUsedCnt, pProvider->AutoStopConsumerID, pProvider->AcwCnt, pProvider->DfpParam.PackGrId);

    // are there some providers in CRT management on this interface ?
    if (pProvMngm->ProviderUsedCnt == 0)
    {
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderRemoveFromCRTMngm(): FATAL -> ProviderUsedCnt=0!");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        if ((pProvider->State == EDDP_CRT_STATE_FREE) || (pProvider->AcwActCnt > 0))
        {
            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderRemoveFromCRTMngm(): FATAL -> Provider-State(0x%X) or AcwActCnt(%d) is not allowed for removing!", 
                pProvider->State, pProvider->AcwActCnt);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        else
        {
            if ((pProvider->ProviderFlags & EDDP_CRT_PROV_FLAGS_IS_DFP) && (pProvider->DfpParam.PackGrId != EDDP_CRT_CHAIN_IDX_END))
            {
                // DFP-Provider must be already disconnected from a pack group
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderRemoveFromCRTMngm(): FATAL -> DFP-Provider is still connected to a pack group! PackGrId=%d", 
                    pProvider->DfpParam.PackGrId);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            else
            {
                // is the AutoStop coupling available yet ?
                if (pProvider->AutoStopConsumerID != EDD_CONSUMER_ID_REMOVE_AS)
                {
                    // remove this provider from AutoStop-GroupList
                    EDDP_CRTProviderRemoveFromASGroupList (pDDB, ProviderID);
                }

                // remove provider from GroupList
                EDDP_CRTProviderRemoveFromGroupList (pDDB, ProviderID);
                // delete DataStatus in APICtrl-RAM
                EDDP_CRTProviderDeleteDataStatusHwApi (pDDB, pProvider->DataStatusId);
                // remove DataStatus from UsedList
                EDDP_CRTDataStatusRemoveFromUsedList (pDDB, pProvider->DataStatusId);

                // first AcwIdx in this provider
                AcwIdx = pProvider->AcwList.BeginIdx;
                // loop through all ACWs in this provider from BeginIdx to EndIdx
                while (AcwIdx != EDDP_CRT_CHAIN_IDX_END)
		        {
                    EDDP_ASSERT_FALSE(AcwIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);
                    // remove ACW from provider
                    EDDP_CRTProvAcwRemoveFromProvider (pDDB, ProviderID, AcwIdx);
                    // remove ACW from UsedList
                    EDDP_CRTProvAcwRemoveFromUsedList (pDDB, AcwIdx);
                    // next AcwIdx in this provider
                    AcwIdx = pProvMngm->pAcwEntry[AcwIdx].AcwProvLink.Next;
                }

                // remove provider from UsedList
                EDDP_CRTProviderRemoveFromUsedList (pDDB, ProviderID);
                // this provider is no more in use
                pProvider->State = EDDP_CRT_STATE_FREE;
            }
        }
    }

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTProviderRemoveFromCRTMngm(): ProviderUsedCnt=%d", 
        pProvMngm->ProviderUsedCnt);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProviderAddToUsedList                               +*/
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
/*+  Description: Add new Provider to UsedList by Provider-Add                              +*/
/*+                 -> get first free index from ProvFreeList and put it to ProvUsedList    +*/
/*+                 -> the NewProviderID will be returned in return value                   +*/
/*+                                                                                         +*/
/*+                 -> if (NewProviderID == EDDP_CRT_CHAIN_IDX_END)                         +*/
/*+                     - no more free provider                                             +*/
/*+                     - set error by caller of this function                              +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderAddToUsedList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT16                              NewProviderID;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
//  EDDP_CRT_PROVIDER_DDB_MNGM_PTR_TYPE     pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE              pProvider;

    pProvMngm       = &pDDB->CRT.Prov;
    pProvMngm       = &pDDB->CRT.Prov;
    NewProviderID   = pProvMngm->ProviderFreeList.BeginIdx;      // first index in provider free list

    if (NewProviderID == EDDP_CRT_CHAIN_IDX_END)
    {
        // no more free provider
        // set error by caller of this function
    }
    else
    {
        pProvider = &pProvMngm->pProvEntry[NewProviderID];

        /* ------------------------------------------------------------ */
	    /* Remove this new Provider from free-chain list                */
	    /* ------------------------------------------------------------ */
        if (pProvider->UsedFreeLink.Next == EDDP_CRT_CHAIN_IDX_END)
        {
            // this provider is the last free provider
            pProvMngm->ProviderFreeList.BeginIdx = EDDP_CRT_CHAIN_IDX_END;
            pProvMngm->ProviderFreeList.EndIdx   = EDDP_CRT_CHAIN_IDX_END;
        }
        else
        {
            // this provider is not the last free provider
            pProvMngm->ProviderFreeList.BeginIdx                                     = pProvider->UsedFreeLink.Next;
            pProvMngm->pProvEntry[pProvider->UsedFreeLink.Next].UsedFreeLink.Prev    = EDDP_CRT_CHAIN_IDX_END;
        }

        /* ------------------------------------------------------------ */
        /* Put this new Provider to end of used-chain list              */
        /* ------------------------------------------------------------ */
        if (pProvMngm->ProviderUsedList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
        {
            // this provider is the first provider in the used list
            pProvMngm->ProviderUsedList.BeginIdx = NewProviderID;
            pProvMngm->ProviderUsedList.EndIdx   = NewProviderID;
            pProvider->UsedFreeLink.Prev         = EDDP_CRT_CHAIN_IDX_END;
            pProvider->UsedFreeLink.Next         = EDDP_CRT_CHAIN_IDX_END;
        }
        else
        {
            // this provider is not the first provider in the used list
            pProvider->UsedFreeLink.Prev                                                        = pProvMngm->ProviderUsedList.EndIdx;
            pProvider->UsedFreeLink.Next                                                        = EDDP_CRT_CHAIN_IDX_END;
            pProvMngm->pProvEntry[pProvMngm->ProviderUsedList.EndIdx].UsedFreeLink.Next   = NewProviderID;
            pProvMngm->ProviderUsedList.EndIdx                                               = NewProviderID;
        }

        pProvMngm->ProviderUsedCnt++;
    }

    return (NewProviderID);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProviderRemoveFromUsedList                          +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             LSA_UINT16                          ProviderID              +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ProviderID         : Provider to be removed                                            +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Remove Provider from UsedList by Provider-Remove                          +*/
/*+                 -> remove this provider from ProvUsedList and put it to ProvFreeList    +*/
/*+                 -> no check ProviderID, it must be valid !                              +*/
/*+                                                                                         +*/
/*+               If no provider in CRT management on this interface -> FATAL!              +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderRemoveFromUsedList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID)
{
    LSA_UINT16                              Prev;
    LSA_UINT16                              Next;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
//  EDDP_CRT_PROVIDER_DDB_MNGM_PTR_TYPE     pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE              pProvider;

    EDDP_ASSERT_FALSE(ProviderID >= pDDB->CRT.CfgParam.ProviderMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pProvMngm    = &pDDB->CRT.Prov;
    pProvider    = &pProvMngm->pProvEntry[ProviderID];
    Prev         = pProvider->UsedFreeLink.Prev;
    Next         = pProvider->UsedFreeLink.Next;

    // are there some providers in CRT management on this interface ?
    if (pProvMngm->ProviderUsedCnt == 0)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderRemoveFromUsedList(ProviderID=%d): FATAL -> ProviderUsedCnt=0!", 
            ProviderID);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        /* ------------------------------------------------------------ */
        /* Remove this Provider from used-chain list                    */
        /* ------------------------------------------------------------ */
        if ((Prev == EDDP_CRT_CHAIN_IDX_END) || (Next == EDDP_CRT_CHAIN_IDX_END))
        {
            if (Prev == EDDP_CRT_CHAIN_IDX_END)
            {
                // this provider is first provider in used list
                if (Next == EDDP_CRT_CHAIN_IDX_END)
                {
                    // first and only one provider in used list
                    pProvMngm->ProviderUsedList.BeginIdx = EDDP_CRT_CHAIN_IDX_END;
                    pProvMngm->ProviderUsedList.EndIdx   = EDDP_CRT_CHAIN_IDX_END;
                }
                else
                {
                    // first provider in used list but not only one
                    pProvMngm->ProviderUsedList.BeginIdx             = Next;
                    pProvMngm->pProvEntry[Next].UsedFreeLink.Prev    = EDDP_CRT_CHAIN_IDX_END;
                }
            }
            else
            {
                // last provider in used list but not only one
                pProvMngm->pProvEntry[Prev].UsedFreeLink.Next    = EDDP_CRT_CHAIN_IDX_END;
                pProvMngm->ProviderUsedList.EndIdx               = Prev;
            }
        }
        else
        {
            // this provider is in the middle of the used list
            pProvMngm->pProvEntry[Prev].UsedFreeLink.Next    = Next;
            pProvMngm->pProvEntry[Next].UsedFreeLink.Prev    = Prev;
        }

        /* ------------------------------------------------------------ */
        /* Put this Provider to begin of free-chain list                */
        /* ------------------------------------------------------------ */
        if (pProvMngm->ProviderFreeList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
        {
            // no provider in free list
            pProvider->UsedFreeLink.Prev         = EDDP_CRT_CHAIN_IDX_END;
            pProvider->UsedFreeLink.Next         = EDDP_CRT_CHAIN_IDX_END;
            pProvMngm->ProviderFreeList.BeginIdx = ProviderID;
            pProvMngm->ProviderFreeList.EndIdx   = ProviderID;
        }
        else
        {
            // one ore more provider in free list
            pProvider->UsedFreeLink.Prev                                                        = EDDP_CRT_CHAIN_IDX_END;
            pProvider->UsedFreeLink.Next                                                        = pProvMngm->ProviderFreeList.BeginIdx;
            pProvMngm->pProvEntry[pProvMngm->ProviderFreeList.BeginIdx].UsedFreeLink.Prev = ProviderID;
            pProvMngm->ProviderFreeList.BeginIdx                                             = ProviderID;
        }

//        EDDP_ASSERT_FALSE(pProvMngm->ProviderUsedCntDDB == 0);
//        pProvMngm->ProviderUsedCntDDB--;

        EDDP_ASSERT_FALSE(pProvMngm->ProviderUsedCnt == 0);
        pProvMngm->ProviderUsedCnt--;
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTTrogEntryAddToUsedList                              +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_UINT16                                                  +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB   : Pointer to DeviceDescriptionBlock                                             +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Add TrogEntry to TrogUsedList if this TrogEntry is added                  +*/
/*+               to TBase array                                                            +*/
/*+                 -> get first free index from TrogFreeList and put it to TrogUsedList    +*/
/*+                 -> the NewTrogUsedIdx will be returned in return value                  +*/
/*+                                                                                         +*/
/*+                 -> if (NewTrogUsedIdx == EDDP_CRT_CHAIN_IDX_END)                        +*/
/*+                     - no more free TrogEntry                                            +*/
/*+                     - set error by caller of this function                              +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTTrogEntryAddToUsedList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT16                              NewTrogUsedIdx;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_TROG_PTR_TYPE         pTrog;

    pProvMngm       = &pDDB->CRT.Prov;
    NewTrogUsedIdx  = pProvMngm->TrogFreeList.BeginIdx;      // first index in the Trog free list

    if (NewTrogUsedIdx == EDDP_CRT_CHAIN_IDX_END)
    {
        // no more free TrogEntry
        // set error by caller of this function
    }
    else
    {
        pTrog = &pProvMngm->pTrogEntry[NewTrogUsedIdx];

        /* ------------------------------------------------------------ */
		/* Remove this new Trog from free-chain list                    */
		/* ------------------------------------------------------------ */
        if (pTrog->UsedFreeLink.Next == EDDP_CRT_CHAIN_IDX_END)
        {
            // this Trog is the last free Trog
            pProvMngm->TrogFreeList.BeginIdx = EDDP_CRT_CHAIN_IDX_END;
            pProvMngm->TrogFreeList.EndIdx   = EDDP_CRT_CHAIN_IDX_END;
        }
        else
        {
            // this Trog is not the last free Trog
            pProvMngm->TrogFreeList.BeginIdx                                     = pTrog->UsedFreeLink.Next;
            pProvMngm->pTrogEntry[pTrog->UsedFreeLink.Next].UsedFreeLink.Prev    = EDDP_CRT_CHAIN_IDX_END;
        }

        /* ------------------------------------------------------------ */
        /* Put this new Trog to end of used-chain list                  */
        /* ------------------------------------------------------------ */
        if (pProvMngm->TrogUsedList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
        {
            // this Trog is the first Trog in the used list
            pProvMngm->TrogUsedList.BeginIdx = NewTrogUsedIdx;
            pProvMngm->TrogUsedList.EndIdx   = NewTrogUsedIdx;
            pTrog->UsedFreeLink.Prev         = EDDP_CRT_CHAIN_IDX_END;
            pTrog->UsedFreeLink.Next         = EDDP_CRT_CHAIN_IDX_END;
        }
        else
        {
            // this Trog is not the first Trog in the used list
            pTrog->UsedFreeLink.Prev                                                  = pProvMngm->TrogUsedList.EndIdx;
            pTrog->UsedFreeLink.Next                                                  = EDDP_CRT_CHAIN_IDX_END;
            pProvMngm->pTrogEntry[pProvMngm->TrogUsedList.EndIdx].UsedFreeLink.Next   = NewTrogUsedIdx;
            pProvMngm->TrogUsedList.EndIdx                                            = NewTrogUsedIdx;
        }

        pProvMngm->TrogUsedCnt++;
    }

    return (NewTrogUsedIdx);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTTrogEntryRemoveFromUsedList                         +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             LSA_UINT16                          TrogUsedId              +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB                   : Pointer to DeviceDescriptionBlock                             +*/
/*+  TrogUsedId             : Index of Trog that is removed from TrogUsedList               +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Remove TrogEntry from TrogUsedList if this TrogEntry is removed           +*/
/*+               from TBase array                                                          +*/
/*+                 -> remove TrogUsedId from TrogUsedList and put it to TrogFreeList       +*/
/*+                 -> no check TrogUsedId, it must be valid !                              +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTTrogEntryRemoveFromUsedList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  TrogUsedId)
{
    LSA_UINT16                              Prev;
    LSA_UINT16                              Next;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_TROG_PTR_TYPE         pTrog;

    EDDP_ASSERT_FALSE(TrogUsedId >= pDDB->CRT.CfgParam.ProviderTrogMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pTrog        = &pProvMngm->pTrogEntry[TrogUsedId];
    Prev         = pTrog->UsedFreeLink.Prev;
    Next         = pTrog->UsedFreeLink.Next;

    /* ------------------------------------------------------------ */
    /* Remove this Trog from used-chain list                        */
    /* ------------------------------------------------------------ */
    if ((Prev == EDDP_CRT_CHAIN_IDX_END) || (Next == EDDP_CRT_CHAIN_IDX_END))
    {
        if (Prev == EDDP_CRT_CHAIN_IDX_END)
        {
            // this Trog is first Trog in used list
            if (Next == EDDP_CRT_CHAIN_IDX_END)
            {
                // first and only one Trog in used list
                pProvMngm->TrogUsedList.BeginIdx = EDDP_CRT_CHAIN_IDX_END;
                pProvMngm->TrogUsedList.EndIdx   = EDDP_CRT_CHAIN_IDX_END;
            }
            else
            {
                // first Trog in used list but not only one
                pProvMngm->TrogUsedList.BeginIdx                 = Next;
                pProvMngm->pTrogEntry[Next].UsedFreeLink.Prev    = Prev;
            }
        }
        else
        {
            // last Trog in used list but not only one
            pProvMngm->pTrogEntry[Prev].UsedFreeLink.Next    = Next;
            pProvMngm->TrogUsedList.EndIdx                   = Prev;
        }
    }
    else
    {
        // this Trog is in the middle of the used list
        pProvMngm->pTrogEntry[Prev].UsedFreeLink.Next    = Next;
        pProvMngm->pTrogEntry[Next].UsedFreeLink.Prev    = Prev;
    }

    /* ------------------------------------------------------------ */
    /* Put this Trog to begin of free-chain list                    */
    /* ------------------------------------------------------------ */
    if (pProvMngm->TrogFreeList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
    {
        // no Trog in free list
        pTrog->UsedFreeLink.Prev         = EDDP_CRT_CHAIN_IDX_END;
        pTrog->UsedFreeLink.Next         = EDDP_CRT_CHAIN_IDX_END;
        pProvMngm->TrogFreeList.BeginIdx = TrogUsedId;
        pProvMngm->TrogFreeList.EndIdx   = TrogUsedId;
    }
    else
    {
        // one ore more Trog in free list
        pTrog->UsedFreeLink.Prev                                                  = EDDP_CRT_CHAIN_IDX_END;
        pTrog->UsedFreeLink.Next                                                  = pProvMngm->TrogFreeList.BeginIdx;
        pProvMngm->pTrogEntry[pProvMngm->TrogFreeList.BeginIdx].UsedFreeLink.Prev = TrogUsedId;
        pProvMngm->TrogFreeList.BeginIdx                                          = TrogUsedId;
    }

    EDDP_ASSERT_FALSE(pProvMngm->TrogUsedCnt == 0);
    pProvMngm->TrogUsedCnt--;
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTDataStatusAddToUsedList                             +*/
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
/*+  Description: Add new DataStatus to UsedList by Provider-Add                            +*/
/*+                 -> get first free index from DataStatus-FreeList and put it to          +*/
/*+                    DataStatus-UsedList                                                  +*/
/*+                 -> the NewDataStatusId will be returned in return value                 +*/
/*+                                                                                         +*/
/*+                 -> if (NewDataStatusId == EDDP_CRT_CHAIN_IDX_END)                       +*/
/*+                     - no more free DataStatus entries                                   +*/
/*+                     - set error by caller of this function                              +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_UINT16 EDDP_LOCAL_FCT_ATTR EDDP_CRTDataStatusAddToUsedList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT16                              NewDataStatusId;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_DS_PTR_TYPE           pDataStatus;

    pProvMngm       = &pDDB->CRT.Prov;
    NewDataStatusId = pProvMngm->DSFreeList.BeginIdx;    // first index in the DataStatus free list

    if (NewDataStatusId == EDDP_CRT_CHAIN_IDX_END)
    {
        // no more free DataStatus entries
        // set error by caller of this function
    }
    else
    {
        pDataStatus = &pProvMngm->pDSEntry[NewDataStatusId];

        /* ------------------------------------------------------------ */
		/* Remove this new DataStatus from free-chain list              */
		/* ------------------------------------------------------------ */
        if (pDataStatus->UsedFreeLink.Next == EDDP_CRT_CHAIN_IDX_END)
        {
            // this DataStatus is the last free DataStatus
            pProvMngm->DSFreeList.BeginIdx   = EDDP_CRT_CHAIN_IDX_END;
            pProvMngm->DSFreeList.EndIdx     = EDDP_CRT_CHAIN_IDX_END;
        }
        else
        {
            // this DataStatus is not the last free DataStatus
            pProvMngm->DSFreeList.BeginIdx                                           = pDataStatus->UsedFreeLink.Next;
            pProvMngm->pDSEntry[pDataStatus->UsedFreeLink.Next].UsedFreeLink.Prev    = EDDP_CRT_CHAIN_IDX_END;
        }

        /* ------------------------------------------------------------ */
        /* Put this new DataStatus to end of used-chain list            */
        /* ------------------------------------------------------------ */
        if (pProvMngm->DSUsedList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
        {
            // this DataStatus is the first DataStatus in the used list
            pProvMngm->DSUsedList.BeginIdx   = NewDataStatusId;
            pProvMngm->DSUsedList.EndIdx     = NewDataStatusId;
            pDataStatus->UsedFreeLink.Prev   = EDDP_CRT_CHAIN_IDX_END;
            pDataStatus->UsedFreeLink.Next   = EDDP_CRT_CHAIN_IDX_END;
        }
        else
        {
            // this DataStatus is not the first DataStatus in the used list
            pDataStatus->UsedFreeLink.Prev                                              = pProvMngm->DSUsedList.EndIdx;
            pDataStatus->UsedFreeLink.Next                                              = EDDP_CRT_CHAIN_IDX_END;
            pProvMngm->pDSEntry[pProvMngm->DSUsedList.EndIdx].UsedFreeLink.Next   = NewDataStatusId;
            pProvMngm->DSUsedList.EndIdx                                             = NewDataStatusId;
        }
    }

    return (NewDataStatusId);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTDataStatusRemoveFromUsedList                        +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             LSA_UINT16                          DataStatusUsedId        +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB                   : Pointer to DeviceDescriptionBlock                             +*/
/*+  DataStatusUsedId       : Index of DataStatus that is removed from DataStatus-UsedList  +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Remove DataStatus from DataStatus-UsedList                                +*/
/*+                 -> remove DataStatusUsedId from DataStatus-UsedList and put it to       +*/
/*+                    DataStatus-FreeList                                                  +*/
/*+                 -> no check DataStatusUsedId, it must be valid !                        +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTDataStatusRemoveFromUsedList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  DataStatusUsedId)
{
    LSA_UINT16                              Prev;
    LSA_UINT16                              Next;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_DS_PTR_TYPE           pDataStatus;

    EDDP_ASSERT_FALSE(DataStatusUsedId >= pDDB->CRT.CfgParam.ProviderDataStatusMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pDataStatus  = &pDDB->CRT.Prov.pDSEntry[DataStatusUsedId];
    Prev         = pDataStatus->UsedFreeLink.Prev;
    Next         = pDataStatus->UsedFreeLink.Next;

    /* ------------------------------------------------------------ */
    /* Remove this DataStatus from used-chain list                  */
    /* ------------------------------------------------------------ */
    if ((Prev == EDDP_CRT_CHAIN_IDX_END) || (Next == EDDP_CRT_CHAIN_IDX_END))
    {
        if (Prev == EDDP_CRT_CHAIN_IDX_END)
        {
            // this DataStatus is first DataStatus in used list
            if (Next == EDDP_CRT_CHAIN_IDX_END)
            {
                // first and only one DataStatus in used list
                pProvMngm->DSUsedList.BeginIdx   = EDDP_CRT_CHAIN_IDX_END;
                pProvMngm->DSUsedList.EndIdx     = EDDP_CRT_CHAIN_IDX_END;
            }
            else
            {
                // first DataStatus in used list but not only one
                pProvMngm->DSUsedList.BeginIdx               = Next;
                pProvMngm->pDSEntry[Next].UsedFreeLink.Prev  = Prev;
            }
        }
        else
        {
            // last DataStatus in used list but not only one
            pProvMngm->pDSEntry[Prev].UsedFreeLink.Next  = Next;
            pProvMngm->DSUsedList.EndIdx                 = Prev;
        }
    }
    else
    {
        // this DataStatus is in the middle of the used list
        pProvMngm->pDSEntry[Prev].UsedFreeLink.Next  = Next;
        pProvMngm->pDSEntry[Next].UsedFreeLink.Prev  = Prev;
    }

    /* ------------------------------------------------------------ */
    /* Put this DataStatus to begin of free-chain list              */
    /* ------------------------------------------------------------ */
    if (pProvMngm->DSFreeList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
    {
        // no DataStatus in free list
        pDataStatus->UsedFreeLink.Prev   = EDDP_CRT_CHAIN_IDX_END;
        pDataStatus->UsedFreeLink.Next   = EDDP_CRT_CHAIN_IDX_END;
        pProvMngm->DSFreeList.BeginIdx   = DataStatusUsedId;
        pProvMngm->DSFreeList.EndIdx     = DataStatusUsedId;
    }
    else
    {
        // one ore more DataStatus in free list
        pDataStatus->UsedFreeLink.Prev                                              = EDDP_CRT_CHAIN_IDX_END;
        pDataStatus->UsedFreeLink.Next                                              = pProvMngm->DSFreeList.BeginIdx;
        pProvMngm->pDSEntry[pProvMngm->DSFreeList.BeginIdx].UsedFreeLink.Prev = DataStatusUsedId;
        pProvMngm->DSFreeList.BeginIdx                                           = DataStatusUsedId;
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTProviderSetState                    +*/
/*                                                                          +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
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
/*+  EDD_RQB_CSRT_PROVIDER_SET_STATE_TYPE                                   +*/
/*+                                                                         +*/
/*+  GroupID    : Providergroup to set status for                           +*/
/*+  Status     : Status with bit values                                    +*/
/*+  Mask       : Mask for Status (spezifies bits to change)                +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Request to set gobal DataStatus in provider group.        +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderSetState(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP                                         Response;
    EDD_SERVICE                                     Service;
    EDD_UPPER_CSRT_PROVIDER_SET_STATE_PTR_TYPE      pRqbParam;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pHDB);
#endif

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_CRTProviderSetState(pRQB=0x%X)",
        pHDB->Handle, pRQB);

    Response    = EDD_STS_OK;
    Service     = EDD_RQB_GET_SERVICE(pRQB);
    pRqbParam   = (EDD_UPPER_CSRT_PROVIDER_SET_STATE_PTR_TYPE) pRQB->pParam;

    // check ProviderSetState parameter
    Response = EDDP_CRTCheckProviderSetStateParam (pDDB, pRqbParam, Service);

    if (Response == EDD_STS_OK)
    {
        // change DataStatus of this provider group
        EDDP_CRTProviderChangeGroupDataStatus (pDDB, pRqbParam->GroupID, pRqbParam->Status, pRqbParam->Mask);
    }

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTProviderSetState(): Response=0x%X", Response);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProviderChangeDataStatus                            +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             LSA_UINT16                          ProviderID              +*/
/*+                             LSA_UINT8                           LocalDataStatus         +*/
/*+                             LSA_UINT8                           LocalMask               +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ProviderID         : ProviderID                                                        +*/
/*+  LocalDataStatus    : New Bit-value of LocalDataStatus                                  +*/
/*+  LocalMask          : mask which bit to set of LocalDataStatus                          +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Change DataStatus of single provider by PROVIDER_CONTROL.                 +*/
/*+                                                                                         +*/
/*+               No check ProviderID, it must be valid and InUse!                          +*/
/*+                                                                                         +*/
/*+               Update LocalDataStatus in DataStatus array of this provider.              +*/
/*+                                                                                         +*/
/*+               This bits in "LocalDataStatus" can be changed:                            +*/
/*+                 Bit(5)  : StationProblemIndicator (OK/Failure)                          +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderChangeDataStatus(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID,
    LSA_UINT8                   LocalDataStatus,
    LSA_UINT8                   LocalMask)
{
    LSA_UINT8                               OldLocalDataStatus;
    LSA_UINT8                               NewLocalDataStatus;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
//  EDDP_CRT_PROVIDER_DDB_MNGM_PTR_TYPE     pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE              pProvider;
    EDDP_CRT_PROVIDER_DS_PTR_TYPE           pDataStatus;

    EDDP_ASSERT_FALSE(ProviderID >= pDDB->CRT.CfgParam.ProviderMaxNumber);

    pProvMngm        = &pDDB->CRT.Prov;
    pProvMngm        = &pDDB->CRT.Prov;
    pProvider           = &pProvMngm->pProvEntry[ProviderID];
    EDDP_ASSERT_FALSE(pProvider->DataStatusId >= pDDB->CRT.CfgParam.ProviderDataStatusMaxNumber);
    pDataStatus         = &pProvMngm->pDSEntry[pProvider->DataStatusId];
    OldLocalDataStatus  = 0;

    #ifdef EDDP_CFG_SYSRED_API_SUPPORT
    // with the System Redundancy API applied, the access to the Datastatus becomes an access to a critical section. 
    /* ---------------------------- */
    /* Enter critical section       */
    /* ---------------------------- */
    EDDP_ENTER_IO();
    #endif

    // read current DataStatus from APICtrl-RAM
    switch (EDDP_HALGetPnipRevision(pDDB))
    {
        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
        case EDDP_HAL_HW_PNIP_REV1:
            OldLocalDataStatus = EDDP_HAL_MEM8_READ(pDDB, &(pDataStatus->pHwDSApiPnipR1->DataStatus)); // get value from hw --> READ
        break;
        #endif

        #if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
        case EDDP_HAL_HW_PNIP_REV2:
        case EDDP_HAL_HW_PNIP_REV3:
            OldLocalDataStatus = EDDP_HAL_MEM8_READ(pDDB, &(pDataStatus->pHwDSApiPnipR2->DataStatus)); // get value from hw --> READ
        break;
        #endif
        
        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderChangeDataStatus(): FATAL -> Version(%d) of PNIP is unknown",
                              EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
    }

    LocalDataStatus = (LocalDataStatus & EDD_CSRT_DSTAT_LOCAL_MASK_ALL);    // allowed DataStatus-Bits for ProviderDataStatus
    LocalMask       = (LocalMask       & EDD_CSRT_DSTAT_LOCAL_MASK_ALL);    // allowed Mask-Bits for ProviderDataStatus

    // calculate new DataStatus
    NewLocalDataStatus = EDDP_CRTGetNewDataStatus (pDDB, OldLocalDataStatus, LocalMask, LocalDataStatus);

    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProviderChangeDataStatus(ProviderID=%d): OldLocalDataStatus=0x%X, NewLocalDataStatus=0x%X", 
        ProviderID, OldLocalDataStatus, NewLocalDataStatus);

    if (OldLocalDataStatus != NewLocalDataStatus)
    {
        // save new DataStatus in CRT
        pDataStatus->LocalDataStatus = NewLocalDataStatus;

        // save new DataStatus in APICtrl-RAM
        // switch to current PNIP revision
        switch (EDDP_HALGetPnipRevision(pDDB))
        {
            #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
            case EDDP_HAL_HW_PNIP_REV1:
            {
                EDDP_HAL_MEM8_WRITE (pDDB, &pDataStatus->pHwDSApiPnipR1->DataStatus, NewLocalDataStatus);
            }
            break;
            #endif

            #if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
            case EDDP_HAL_HW_PNIP_REV2:
            case EDDP_HAL_HW_PNIP_REV3:
            {
                EDDP_HAL_MEM8_WRITE (pDDB, &pDataStatus->pHwDSApiPnipR2->DataStatus, NewLocalDataStatus);
            }
            break;
            #endif
            
            default:
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderChangeDataStatus(): FATAL -> Version(%d) of PNIP is unknown", 
                    EDDP_HALGetPnipRevision(pDDB));
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }
            break;
        }
    }

    #ifdef EDDP_CFG_SYSRED_API_SUPPORT
    /* ---------------------------- */
    /* Exit critical section        */
    /* ---------------------------- */
    EDDP_EXIT_IO();
    #endif
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProviderChangeGroupDataStatus                       +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             LSA_UINT16                          GroupId                 +*/
/*+                             LSA_UINT8                           GroupDataStatus         +*/
/*+                             LSA_UINT8                           GroupMask               +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  GroupId            : GroupId                                                           +*/
/*+  GroupDataStatus    : New Bit-value of GroupDataStatus                                  +*/
/*+  GroupMask          : mask which bit to set GroupDataStatus                             +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Change DataStatus of provider group by PROVIDER_SET_STATE.                +*/
/*+                                                                                         +*/
/*+               No check GroupId, it must be valid!                                       +*/
/*+                                                                                         +*/
/*+               Update GroupDataStatus in ProvGroup.                                      +*/
/*+                                                                                         +*/
/*+               Update GroupDataStatus in PNIP register "PNIP_REG_APPLDATASTATUS_IFA_x"   +*/
/*+               (x = GroupId)                                                             +*/
/*+               This bits in "GroupDataStatus" can be changed:                            +*/
/*+                 Bit 4   : ProviderState (STOP/RUN)                                      +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderChangeGroupDataStatus(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  GroupId,
    LSA_UINT8                   GroupDataStatus,
    LSA_UINT8                   GroupMask)
{
    LSA_UINT8                               OldGroupDataStatus;
    LSA_UINT8                               NewGroupDataStatus;
    EDDP_CRT_PROVIDER_GROUP_PTR_TYPE        pProvGroup;

    EDDP_ASSERT_FALSE(GroupId >= pDDB->CRT.CfgParam.ProviderGroupMaxNumber);

    pProvGroup          = &pDDB->CRT.Prov.pProvGroupEntry[GroupId];
    OldGroupDataStatus  = pProvGroup->GroupDataStatus;
    GroupDataStatus     = (GroupDataStatus & EDD_CSRT_DSTAT_GLOBAL_MASK_ALL);   // allowed DataStatus-Bits for GroupDataStatus
    GroupMask           = (GroupMask       & EDD_CSRT_DSTAT_GLOBAL_MASK_ALL);   // allowed Mask-Bits for GroupDataStatus

    // calculate new DataStatus
    NewGroupDataStatus = EDDP_CRTGetNewDataStatus (pDDB, OldGroupDataStatus, GroupMask, GroupDataStatus);

    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProviderChangeGroupDataStatus(GroupId=%d): OldGroupDataStatus=0x%X, NewGroupDataStatus=0x%X", 
        GroupId, OldGroupDataStatus, NewGroupDataStatus);

    if (OldGroupDataStatus != NewGroupDataStatus)
    {
        // save new GroupDataStatus in CRT
        pProvGroup->GroupDataStatus = NewGroupDataStatus;

        // save new GroupDataStatus in PNIP-Register
        switch (GroupId)
        {
            case EDDP_CRT_GROUP_ID_0:
            {
                EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_APPLDATASTATUS_IFA_0, (LSA_UINT32) NewGroupDataStatus);
                EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_DG_DATASTATUS_IFA_0, (LSA_UINT32) NewGroupDataStatus);
            }
            break;

            case EDDP_CRT_GROUP_ID_1:
            {
                EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_APPLDATASTATUS_IFA_1, (LSA_UINT32) NewGroupDataStatus);
                EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_DG_DATASTATUS_IFA_1, (LSA_UINT32) NewGroupDataStatus);
            }
            break;

            case EDDP_CRT_GROUP_ID_2:
            {
                EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_APPLDATASTATUS_IFA_2, (LSA_UINT32) NewGroupDataStatus);
                EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_DG_DATASTATUS_IFA_2, (LSA_UINT32) NewGroupDataStatus);
            }
            break;

            case EDDP_CRT_GROUP_ID_3:
            {
                EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_APPLDATASTATUS_IFA_3, (LSA_UINT32) NewGroupDataStatus);
                EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_DG_DATASTATUS_IFA_3, (LSA_UINT32) NewGroupDataStatus);
            }
            break;

            case EDDP_CRT_GROUP_ID_4:
            {
                EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_APPLDATASTATUS_IFA_4, (LSA_UINT32) NewGroupDataStatus);
                EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_DG_DATASTATUS_IFA_4, (LSA_UINT32) NewGroupDataStatus);
            }
            break;

            case EDDP_CRT_GROUP_ID_5:
            {
                EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_APPLDATASTATUS_IFA_5, (LSA_UINT32) NewGroupDataStatus);
                EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_DG_DATASTATUS_IFA_5, (LSA_UINT32) NewGroupDataStatus);
            }
            break;

            case EDDP_CRT_GROUP_ID_6:
            {
                EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_APPLDATASTATUS_IFA_6, (LSA_UINT32) NewGroupDataStatus);
                EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_DG_DATASTATUS_IFA_6, (LSA_UINT32) NewGroupDataStatus);
            }
            break;

            case EDDP_CRT_GROUP_ID_7:
            {
                EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_APPLDATASTATUS_IFA_7, (LSA_UINT32) NewGroupDataStatus);
                EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_DG_DATASTATUS_IFA_7, (LSA_UINT32) NewGroupDataStatus);
            }
            break;

            default:
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderChangeGroupDataStatus(): FATAL -> GroupId invalid! GroupId=%d", 
                    GroupId);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTProviderChangePhase                 +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE     pHDB            +*/
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
/*+  EDD_RQB_CSRT_PROVIDER_CHANGE_PHASE_TYPE                                +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_ERR_RESOURCE                                      +*/
/*+               EDD_STS_ERR_LIMIT_REACHED                                 +*/
/*+               EDD_STS_ERR_PHASE_MASKED                                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Request to change phase of a provider (ACW).              +*/
/*+               Only RTC1/2/UDP provider can change the phase.            +*/
/*+               If RTC3 provider -> EDD_STS_ERR_PARAM                     +*/
/*+               If provider has more ACWs -> EDD_STS_ERR_PARAM            +*/
/*+               If new Trog is necessary but no Trog is available         +*/
/*+                 -> EDD_STS_ERR_RESOURCE                                 +*/
/*+                                                                         +*/
/*+               not executing the request if:                             +*/
/*+                 - new parameter have no changes                         +*/
/*+                 - ACW shall be moved inside the same phase and this ACW +*/
/*+                   is the only one ACW in the phase (only change the     +*/
/*+                   parameter in CRT)                                     +*/
/*+                 - ACW is not available in APICtrl-RAM                   +*/
/*+                   (only change the parameter in CRT)                    +*/
/*+                 - ACW is not chained in AcwPhaseList                    +*/
/*+               In these cases change only the parameter in CRT.          +*/
/*+                                                                         +*/
/*+               This steps will be executed before the command:           +*/
/*+               - Save new Rqb-Parameter and call command interface       +*/
/*+                 command "ACW_Move"                                      +*/
/*+               - Remove ACW from Old-AcwPhaseList                        +*/
/*+               - If this is the last ACW in old Trog, remove Trog from   +*/
/*+                 TrogUsedList                                            +*/
/*+               - Save old parameter in ACW_Move struct                   +*/
/*+               - Change parameter in CRT                                 +*/
/*+               - Add this ACW to New-AcwPhaseList                        +*/
/*+               - If this is the first ADD in this Trog, init             +*/
/*+                 TBaseElement in APICtrl-RAM                             +*/
/*+               - put R-O-G-Bit if this is first ACW in AcwPhaseList      +*/
/*+                                                                         +*/
/*+               This steps will be executed afte the command in Cbf:      +*/
/*+               - If it was the last ACW in old Trog                      +*/
/*+                     -> delete old Trog in APICtrl-RAM                   +*/
/*+                     -> delete old TBaseElement in APICtrl-RAM           +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderChangePhase(
    EDD_UPPER_RQB_PTR_TYPE      pRQB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP                                             Response;
    LSA_BOOL                                            CmdExecute;
    LSA_UINT16                                          TBaseIndex;
    LSA_UINT32                                          AcwPhaseCnt;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE                     pProvMngm;
//  EDDP_CRT_PROVIDER_DDB_MNGM_PTR_TYPE                 pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE                          pProvider;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE                      pAcw;
    EDDP_CRT_PROVIDER_TBASE_PTR_TYPE                    pTBase;
    EDD_UPPER_CSRT_PROVIDER_CHANGE_PHASE_PTR_TYPE       pRqbParam;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pHDB);
#endif

    Response     = EDD_STS_OK;
    CmdExecute   = LSA_TRUE;
    pProvMngm    = &pDDB->CRT.Prov;
    pProvMngm    = &pDDB->CRT.Prov;
    AcwPhaseCnt  = 0;
    pRqbParam    = (EDD_UPPER_CSRT_PROVIDER_CHANGE_PHASE_PTR_TYPE) pRQB->pParam;

    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_CRTProviderChangePhase(pRQB=0x%X, ProviderID=%d)",
        pHDB->Handle, pRQB, pRqbParam->ProviderID);

    // check ProviderChangePhase parameter
    Response = EDDP_CRTCheckProviderChangePhaseParam (pDDB, pRqbParam);

    if (Response == EDD_STS_OK)
    {
        EDDP_ASSERT_FALSE(pRqbParam->ProviderID >= pDDB->CRT.CfgParam.ProviderMaxNumber);
        pProvider   = &pProvMngm->pProvEntry[pRqbParam->ProviderID];
        EDDP_ASSERT_FALSE(pProvider->AcwList.BeginIdx == EDDP_CRT_CHAIN_IDX_END);
        pAcw        = &pProvMngm->pAcwEntry[pProvider->AcwList.BeginIdx];

        EDDP_CRT_TRACE_08(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:%2X] EDDP_CRTProviderChangePhase(): FrameID=0x%X, ProviderFlags=0x%X, Current-Ph=%d, Current-PhSeq=0x%X, New-Ph=%d, New-PhSeq=0x%X, ACW-State=0x%X",
            pHDB->Handle, pProvider->AddParam.FrameID, pProvider->ProviderFlags, 
            pAcw->AddParam.CyclePhase, pAcw->AddParam.CyclePhaseSequence, pRqbParam->CyclePhase, pRqbParam->CyclePhaseSequence, 
            pAcw->State);

        /* -------------------------------------------------------------------- */
        /* check whether new parameters have changes                            */
        /* -------------------------------------------------------------------- */
        if (   (pRqbParam->CyclePhase         == pAcw->AddParam.CyclePhase)
            && (pRqbParam->CyclePhaseSequence == pAcw->AddParam.CyclePhaseSequence)
           )
        {
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProviderChangePhase(): not executing the command - RqbParam CyclePhase and CyclePhaseSequence have no changes");
            // not executing the command
            CmdExecute = LSA_FALSE;
        }

        /* -------------------------------------------------------------------- */
        /* is this ACW already added to AcwPhaseList and is available in HW ?   */
        /* -------------------------------------------------------------------- */
        if (CmdExecute)
        {
            if ((pAcw->State & EDDP_CRT_PROV_ACW_STATE_IN_PHASELIST_MASK) && (pAcw->State & EDDP_CRT_PROV_ACW_STATE_IN_HW_MASK))
            {
                /* ---------------------------------------------------------------- */
                /* check whether the ACW will be moved inside the same phase        */
                /* only with new CyclePhaseSequence                                 */
                /* and this ACW is the only one ACW in this phase                   */
                /* ---------------------------------------------------------------- */
                if (pRqbParam->CyclePhase == pAcw->AddParam.CyclePhase)
                {
                    TBaseIndex  = EDDP_CRTGetTBaseIndex ( pDDB, 
                                                          pAcw->AddParam.CycleReductionRatio, 
                                                          pAcw->AddParam.CyclePhase);
                    pTBase      = &pProvMngm->pTBaseEntry[TBaseIndex];

                    switch (pAcw->ROGPhase)
                    {
                        case EDDP_CRT_PROV_ACW_PHASE_G:
                            AcwPhaseCnt = pTBase->pTrogEntry->TrogAcwList.AcwPhaseCnt_G;
                        break;

                        case EDDP_CRT_PROV_ACW_PHASE_O:
                            AcwPhaseCnt = pTBase->pTrogEntry->TrogAcwList.AcwPhaseCnt_O;
                        break;

                        case EDDP_CRT_PROV_ACW_PHASE_R:
                        default:
                        {
                            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderChangePhase(): FATAL -> ROGPhase unsupported! ROGPhase=0x%X", 
                                pAcw->ROGPhase);
                            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                        }
                        break;
                    }

                    if (AcwPhaseCnt == 1)
                    {
                        // this ACW is the only one ACW in this phase
                        // only change the parameter "CyclePhaseSequence"
                        pAcw->AddParam.CyclePhaseSequence = pRqbParam->CyclePhaseSequence;

                        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProviderChangePhase(): not executing the command - ACW shall be moved inside the same phase and this ACW is the only one ACW in the phase, only change the parameter CyclePhaseSequence in CRT");
                        // not executing the command
                        CmdExecute = LSA_FALSE;
                    }
                }
            }
            else
            {
                // this ACW is not chained in AcwPhaseList and is not available in HW
                // only change the parameter in CRT
                pAcw->AddParam.CyclePhase           = pRqbParam->CyclePhase;
                pAcw->AddParam.CyclePhaseSequence   = pRqbParam->CyclePhaseSequence;

                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProviderChangePhase(): not executing the command - ACW is not chained in AcwPhaseList and is not available in HW, only change parameter in CRT");
                // not executing the command
                CmdExecute = LSA_FALSE;
            }
        }

        /* -------------------------------- */
        /* execute the command ?            */
        /* -------------------------------- */
        if (CmdExecute)
        {
            EDDP_CMD_PARAM_PTR_TYPE pNewCmdParam = EDDP_CRTCmdGetInternalCmdParamElement(pDDB);
            
            // remember parameter for CMD-Request
            pNewCmdParam->param_union.Crt.Ppm.ACW_Move.ProviderId             = pProvider->ProviderId;
            pNewCmdParam->param_union.Crt.Ppm.ACW_Move.CyclePhase             = pRqbParam->CyclePhase;
            pNewCmdParam->param_union.Crt.Ppm.ACW_Move.CyclePhaseSequence     = pRqbParam->CyclePhaseSequence;
            // call CmdRequest
            // if command is finished, the Cbf EDDP_CRTCmdDoneCbfPpm() will be called
            EDDP_CRTCmdRequestPpm (pDDB, EDDP_CRT_CMD_PPM_ACW_MOVE, EDDP_CRT_CMD_ASYNCHRON, pNewCmdParam);
            // set Response for RQBHandler
            Response = EDD_STS_OK_PENDING;
        }
    }

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTProviderChangePhase(): Response=0x%X", Response);

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProviderInitTBaseElementHwApi                       +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                        +*/
/*+                             LSA_UINT16                      TBaseIndex                  +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                     +*/
/*+  TBaseIndex     : Index in TBase-Array                                                  +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: This function inits the TBaseElement in APICtrl-RAM.                      +*/
/*+                                                                                         +*/
/*+               If this TBase does not contain Trog or the TBaseElement is already        +*/
/*+               initialized, so nothing to do.                                            +*/
/*+                                                                                         +*/
/*+               TBaseElement:                                                             +*/
/*+                 - Bit(15)   : Red_enable                                                +*/
/*+                 - Bit(14)   : Orange_enable                                             +*/
/*+                 - Bit(13)   : Green_enable                                              +*/
/*+                 - Bit(12)   : DG_CC_enable                                              +*/
/*+                 - Bit(11:0) : ROG_offset                                                +*/
/*+                                                                                         +*/
/*+               Init ROG_offset                                                           +*/
/*+                 - Offset to address in Reg. "PNIP_REG_ACW_TROGPTR_IFA/IFB"              +*/
/*+                 - 64Bit alined last 3 bits are cut off                                  +*/
/*+                                                                                         +*/
/*+               Activate DG_CC_enable Bit.                                                +*/
/*+                                                                                         +*/
/*+               Write new TBaseElement to APICtrl-RAM.                                    +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderInitTBaseElementHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  TBaseIndex)
{
    LSA_UINT16                          ROG_offset;
    LSA_UINT16                          TBaseElementTemp;
    LSA_UINT32                          ApiAdrTemp1;
    LSA_UINT32                          ApiAdrTemp2;
    EDDP_CRT_PROVIDER_TBASE_PTR_TYPE    pTBase;

    EDDP_ASSERT_FALSE(TBaseIndex >= EDDP_CRT_MAX_COUNT_TBASE);

    TBaseElementTemp    = 0;
    ROG_offset          = 0;
    pTBase              = &pDDB->CRT.Prov.pTBaseEntry[TBaseIndex];

    // only if Trog is available on this TBase and TBaseElement not initialized yet
    if ( !(LSA_HOST_PTR_ARE_EQUAL(pTBase->pTrogEntry, LSA_NULL)) && (!pTBase->TBaseElementInHw) )
    {
        /* -------------------------------------------- */
        /* init ROG_offset                              */
        /* -------------------------------------------- */
        ApiAdrTemp1 = EDDP_HAL_H2P_ADDR (pDDB, pDDB->CRT.Prov.pHwBaseTrogApi);
        ApiAdrTemp2 = EDDP_HAL_H2P_ADDR (pDDB, pTBase->pTrogEntry->pHwTrogApi);
        EDDP_ASSERT_FALSE(ApiAdrTemp1 > ApiAdrTemp2);
        ROG_offset = (LSA_UINT16) (ApiAdrTemp2 - ApiAdrTemp1);

        /* -------------------------------------------- */
        /* check alignment of ROG_offset                */
        /* -------------------------------------------- */
        if ( !(EDDP_GlbIsAligned(pDDB, ROG_offset, EDDP_GLB_ALIGNMENT_8_BYTE)) )
        {
            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderInitTBaseElementHwApi(TBaseIndex=%d): FATAL -> ROG_offset(0x%X) is not 64Bit aligned!", 
                TBaseIndex, ROG_offset);
            EDDP_FATAL(EDDP_FATAL_ERR_ALIGNMENT);
        }
        else
        {
            // it is 64Bit aligned -> cutt off the first 3 bit
            EDDP_SHIFT_VALUE_RIGHT (ROG_offset, EDDP_CRT_SHIFT_VALUE_3)
        }

        /* -------------------------------------------- */
        /* write ROG_offset to TBaseElementTemp         */
        /* -------------------------------------------- */
        EDDP_SET_BIT_VALUE16( TBaseElementTemp, 
                              ROG_offset, 
                              PNIP_ACW_SND_TBASE_ROG_OFFSET_MASK, 
                              PNIP_ACW_SND_TBASE_ROG_OFFSET_SHIFT)

        /* -------------------------------------------- */
        /* activate DG_CC_enable in TBaseElementTemp    */
        /* -------------------------------------------- */
        EDDP_SET_BIT_VALUE16( TBaseElementTemp, 
                              PNIP_ACW_SND_TBASE_BIT_ON, 
                              PNIP_ACW_SND_TBASE_DG_CC_ENABLE_MASK, 
                              PNIP_ACW_SND_TBASE_DG_CC_ENABLE_SHIFT)

        /* -------------------------------------------- */
        /* write TBaseElement to APICtrl-RAM            */
        /* -------------------------------------------- */
        EDDP_HAL_MEM16_WRITE (pDDB, &pTBase->pHwTBaseApi->TBaseElement, TBaseElementTemp);

        EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProviderInitTBaseElementHwApi(TBaseIndex=%d): pHwBaseTBaseApi=0x%X, pHwTBaseApi=0x%X, TBaseElement=0x%X", 
            TBaseIndex, pDDB->CRT.Prov.pHwBaseTBaseApi, pTBase->pHwTBaseApi, TBaseElementTemp);

        pTBase->TBaseElementInHw    = LSA_TRUE;
        pTBase->TBaseElement        = TBaseElementTemp;
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProviderDeleteTBaseElementHwApi                 +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      TBaseIndex              +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  TBaseIndex     : Index in TBase-Array                                              +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function deletes one TBaseElement in APICtrl-RAM.                +*/
/*+               Call this function only after last ACW is removed from Trog!          +*/
/*+                                                                                     +*/
/*+               Calculate the parameter for function EDDP_HALMemset()                 +*/
/*+               and call this function.                                               +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderDeleteTBaseElementHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  TBaseIndex)
{
    EDD_RSP                             Response;
    LSA_UINT32                          RamSubApiCtrl;
    LSA_UINT32                          ApiAdrTemp1;
    LSA_UINT32                          ApiAdrTemp2;
    LSA_UINT32                          IntOffs;
    EDDP_CRT_PROVIDER_TBASE_PTR_TYPE    pTBase;

    EDDP_ASSERT_FALSE(TBaseIndex >= EDDP_CRT_MAX_COUNT_TBASE);

    Response        = EDD_STS_OK;
    pTBase          = &pDDB->CRT.Prov.pTBaseEntry[TBaseIndex];
    RamSubApiCtrl   = EDDP_CRT_INVALID_VALUE_32;    // init to prevent compiler warning

    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProviderDeleteTBaseElementHwApi(TBaseIndex=%d): pHwBaseTBaseApi=0x%X, pHwTBaseApi=0x%X", 
        TBaseIndex, pDDB->CRT.Prov.pHwBaseTBaseApi, pTBase->pHwTBaseApi);

    if (pTBase->TBaseElementInHw)
    {
        RamSubApiCtrl = EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_TBASE_LIST;     // IFA

        ApiAdrTemp1 = (LSA_UINT32) pDDB->CRT.Prov.pHwBaseTBaseApi;
        ApiAdrTemp2 = (LSA_UINT32) pTBase->pHwTBaseApi;
        EDDP_ASSERT_FALSE(ApiAdrTemp1 > ApiAdrTemp2);
        IntOffs = ApiAdrTemp2 - ApiAdrTemp1;

        Response = EDDP_HALMemset ( pDDB, 
                                    EDDP_HAL_RAMTYPE_API_CTRL, 
                                    RamSubApiCtrl, 
                                    IntOffs, 
                                    (sizeof(PNIP_ACW_SND_TBASE_TYPE)), 
                                    0);

        if (Response != EDD_STS_OK)
        {
            EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderDeleteTBaseElementHwApi(): FATAL -> Response(0x%X) of EDDP_HALMemset() is not OK!, IntOffs=0x%X, Length=0x%X",
                Response, IntOffs, (sizeof(PNIP_ACW_SND_TBASE_TYPE)));
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        else
        {
            // TBaseElement is no more available in HW
            pTBase->TBaseElementInHw    = LSA_FALSE;
            pTBase->TBaseElement        = 0;
        }
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProviderDeleteTrogHwApi                         +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      TrogId                  +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                                     +*/
/*+  TrogId     : Index in Trog-Array                                                   +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function deletes one TROG in APICtrl-RAM.                        +*/
/*+               Call this function only after last ACW is removed from this Trog!     +*/
/*+                                                                                     +*/
/*+               Calculate the parameter for function EDDP_HALMemset()                 +*/
/*+               and call this function.                                               +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderDeleteTrogHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  TrogId)
{
    EDD_RSP                             Response;
    LSA_UINT32                          RamSubApiCtrl;
    LSA_UINT32                          ApiAdrTemp1;
    LSA_UINT32                          ApiAdrTemp2;
    LSA_UINT32                          IntOffs;
    EDDP_CRT_PROVIDER_TROG_PTR_TYPE     pTrog;

    EDDP_ASSERT_FALSE(TrogId >= pDDB->CRT.CfgParam.ProviderTrogMaxNumber);

    Response        = EDD_STS_OK;
    pTrog           = &pDDB->CRT.Prov.pTrogEntry[TrogId];
    RamSubApiCtrl   = EDDP_CRT_INVALID_VALUE_32;    // init to prevent compiler warning

    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProviderDeleteTrogHwApi(TrogId=%d): pHwBaseTrogApi=0x%X, pHwTrogApi=0x%X", 
        TrogId, pDDB->CRT.Prov.pHwBaseTrogApi, pTrog->pHwTrogApi);

    RamSubApiCtrl = EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_TROG_LIST;  // IFA

    ApiAdrTemp1 = (LSA_UINT32) pDDB->CRT.Prov.pHwBaseTrogApi;
    ApiAdrTemp2 = (LSA_UINT32) pTrog->pHwTrogApi;
    EDDP_ASSERT_FALSE(ApiAdrTemp1 > ApiAdrTemp2);
    IntOffs = ApiAdrTemp2 - ApiAdrTemp1;

    Response = EDDP_HALMemset ( pDDB, 
                                EDDP_HAL_RAMTYPE_API_CTRL, 
                                RamSubApiCtrl, 
                                IntOffs, 
                                (sizeof(PNIP_ACW_SND_TROG_TYPE)), 
                                EDDP_CRT_INVALID_VALUE_8);      // 0xFF necessary for command "ACW_move"

    if (Response != EDD_STS_OK)
    {
        EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderDeleteTrogHwApi(): FATAL -> Response(0x%X) of EDDP_HALMemset() is not OK!, IntOffs=0x%X, Length=0x%X",
            Response, IntOffs, (sizeof(PNIP_ACW_SND_TROG_TYPE)));
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProviderAddToGroupList                              +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             LSA_UINT16                          ProviderID              +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ProviderID         : Index in provider array                                           +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Add Provider to GroupList by Provider-Add.                                +*/
/*+               The ACW with DataStatusId must be already added to this provider!         +*/
/*+                                                                                         +*/
/*+               Put this new provider to end of GroupList.                                +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderAddToGroupList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID)
{
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE              pProvider;
    EDDP_CRT_PROVIDER_GROUP_PTR_TYPE        pProvGroup;
    EDDP_CRT_PROVIDER_DS_PTR_TYPE           pDataStatus;

    EDDP_ASSERT_FALSE(ProviderID >= pDDB->CRT.CfgParam.ProviderMaxNumber);

    pProvMngm       = &pDDB->CRT.Prov;
    pProvider       = &pProvMngm->pProvEntry[ProviderID];
    EDDP_ASSERT_FALSE(pProvider->DataStatusId >= pDDB->CRT.CfgParam.ProviderDataStatusMaxNumber);
    pDataStatus     = &pDDB->CRT.Prov.pDSEntry[pProvider->DataStatusId];
    EDDP_ASSERT_FALSE(pDataStatus->DS_Selector >= pDDB->CRT.CfgParam.ProviderGroupMaxNumber);
    pProvGroup      = &pDDB->CRT.Prov.pProvGroupEntry[pDataStatus->DS_Selector];

    /* ------------------------------------------------------------ */
    /* Put this new provider to end of GroupList                    */
    /* ------------------------------------------------------------ */
    if (pProvGroup->GroupList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
    {
        // this provider is the first provider in GroupList
        pProvGroup->GroupList.BeginIdx  = ProviderID;
        pProvGroup->GroupList.EndIdx    = ProviderID;
        pProvider->GroupLink.Prev       = EDDP_CRT_CHAIN_IDX_END;
        pProvider->GroupLink.Next       = EDDP_CRT_CHAIN_IDX_END;
    }
    else
    {
        // this provider is not the first provider in GroupList
        pProvider->GroupLink.Prev                                               = pProvGroup->GroupList.EndIdx;
        pProvider->GroupLink.Next                                               = EDDP_CRT_CHAIN_IDX_END;
        pProvMngm->pProvEntry[pProvGroup->GroupList.EndIdx].GroupLink.Next   = ProviderID;
        pProvGroup->GroupList.EndIdx                                            = ProviderID;
    }

    pProvGroup->ProviderCnt++;
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProviderRemoveFromGroupList                         +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             LSA_UINT16                          ProviderID              +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ProviderID         : Index in provider array                                           +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Remove Provider from GroupList by Provider-Remove.                        +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderRemoveFromGroupList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID)
{
    LSA_UINT16                              Prev;
    LSA_UINT16                              Next;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE              pProvider;
    EDDP_CRT_PROVIDER_GROUP_PTR_TYPE        pProvGroup;
    EDDP_CRT_PROVIDER_DS_PTR_TYPE           pDataStatus;

    EDDP_ASSERT_FALSE(ProviderID >= pDDB->CRT.CfgParam.ProviderMaxNumber);

    pProvMngm       = &pDDB->CRT.Prov;
    pProvider       = &pProvMngm->pProvEntry[ProviderID];
    EDDP_ASSERT_FALSE(pProvider->DataStatusId >= pDDB->CRT.CfgParam.ProviderDataStatusMaxNumber);
    pDataStatus     = &pDDB->CRT.Prov.pDSEntry[pProvider->DataStatusId];
    EDDP_ASSERT_FALSE(pDataStatus->DS_Selector >= pDDB->CRT.CfgParam.ProviderGroupMaxNumber);
    pProvGroup      = &pDDB->CRT.Prov.pProvGroupEntry[pDataStatus->DS_Selector];
    Prev            = pProvider->GroupLink.Prev;
    Next            = pProvider->GroupLink.Next;

    /* ------------------------------------------------------------ */
    /* Remove this provider from GroupList                          */
    /* ------------------------------------------------------------ */
    if ((Prev == EDDP_CRT_CHAIN_IDX_END) || (Next == EDDP_CRT_CHAIN_IDX_END))
    {
        if (Prev == EDDP_CRT_CHAIN_IDX_END)
        {
            // this provider is first provider in GroupList
            if (Next == EDDP_CRT_CHAIN_IDX_END)
            {
                // first and only one provider in GroupList
                pProvGroup->GroupList.BeginIdx  = EDDP_CRT_CHAIN_IDX_END;
                pProvGroup->GroupList.EndIdx    = EDDP_CRT_CHAIN_IDX_END;
            }
            else
            {
                // first provider in GroupList but not only one
                pProvGroup->GroupList.BeginIdx                  = Next;
                pProvMngm->pProvEntry[Next].GroupLink.Prev   = EDDP_CRT_CHAIN_IDX_END;
            }
        }
        else
        {
            // last provider in GroupList but not only one
            pProvMngm->pProvEntry[Prev].GroupLink.Next   = EDDP_CRT_CHAIN_IDX_END;
            pProvGroup->GroupList.EndIdx                    = Prev;
        }
    }
    else
    {
        // this provider is in the middle of the GroupList
        pProvMngm->pProvEntry[Prev].GroupLink.Next   = Next;
        pProvMngm->pProvEntry[Next].GroupLink.Prev   = Prev;
    }

    EDDP_ASSERT_FALSE(pProvGroup->ProviderCnt == 0);
    pProvGroup->ProviderCnt--;
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProviderAddToASGroupList                            +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             LSA_UINT16                          ProviderID              +*/
/*+                             LSA_UINT16                          AutoStopConsumerID      +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ProviderID         : Index in provider array                                           +*/
/*+  AutoStopConsumerID : AutoStop-ConsumerID which will be used for Provider-AutoStop      +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Add Provider to AutoStop-GroupList.                                       +*/
/*+               Put this provider to end of AutoStop-GroupList.                           +*/
/*+                                                                                         +*/
/*+               If this provider has already the reference to AutoStop consumer -> FATAL. +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderAddToASGroupList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID,
    LSA_UINT16                  AutoStopConsumerID)
{
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE              pProvider;
    EDDP_CRT_PROVIDER_ASGROUP_PTR_TYPE      pProvASGroup;

    EDDP_ASSERT_FALSE(ProviderID >= pDDB->CRT.CfgParam.ProviderMaxNumber);
    EDDP_ASSERT_FALSE(AutoStopConsumerID >= pDDB->CRT.CfgParam.ConsumerMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pProvider    = &pProvMngm->pProvEntry[ProviderID];

    if (pProvider->AutoStopConsumerID != EDD_CONSUMER_ID_REMOVE_AS)
    {
        EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderAddToASGroupList(ProviderID=%d, AutoStopConsumerID=%d): FATAL -> This provider has already the reference to AutoStop consumer! Provider-AutoStopConsumerID=%d", 
            ProviderID, AutoStopConsumerID, pProvider->AutoStopConsumerID);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        pProvASGroup = &pDDB->CRT.Prov.pASGroupEntry[AutoStopConsumerID];

        /* ------------------------------------------------------------ */
        /* Put this provider to end of AutoStop-GroupList               */
        /* ------------------------------------------------------------ */
        if (pProvASGroup->ASGroupList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
        {
            // this provider is the first provider in AutoStop-GroupList
            pProvASGroup->ASGroupList.BeginIdx  = ProviderID;
            pProvASGroup->ASGroupList.EndIdx    = ProviderID;
            pProvider->ASGroupLink.Prev         = EDDP_CRT_CHAIN_IDX_END;
            pProvider->ASGroupLink.Next         = EDDP_CRT_CHAIN_IDX_END;
        }
        else
        {
            // this provider is not the first provider in AutoStop-GroupList
            pProvider->ASGroupLink.Prev                                                 = pProvASGroup->ASGroupList.EndIdx;
            pProvider->ASGroupLink.Next                                                 = EDDP_CRT_CHAIN_IDX_END;
            pProvMngm->pProvEntry[pProvASGroup->ASGroupList.EndIdx].ASGroupLink.Next = ProviderID;
            pProvASGroup->ASGroupList.EndIdx                                            = ProviderID;
        }

        // store new AutoStopConsumerID in provider
        pProvider->AutoStopConsumerID = AutoStopConsumerID;

        pProvASGroup->ProviderCnt++;
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProviderRemoveFromASGroupList                       +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             LSA_UINT16                          ProviderID              +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ProviderID         : Index in provider array                                           +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Remove Provider from AutoStop-GroupList.                                  +*/
/*+                                                                                         +*/
/*+               If this provider has no reference to AutoStop consumer -> FATAL.          +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderRemoveFromASGroupList(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ProviderID)
{
    LSA_UINT16                              Prev;
    LSA_UINT16                              Next;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE              pProvider;
    EDDP_CRT_PROVIDER_ASGROUP_PTR_TYPE      pProvASGroup;

    EDDP_ASSERT_FALSE(ProviderID >= pDDB->CRT.CfgParam.ProviderMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pProvider    = &pProvMngm->pProvEntry[ProviderID];
    Prev         = pProvider->ASGroupLink.Prev;
    Next         = pProvider->ASGroupLink.Next;

    if (pProvider->AutoStopConsumerID == EDD_CONSUMER_ID_REMOVE_AS)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderRemoveFromASGroupList(ProviderID=%d): FATAL -> This provider has no reference to AutoStop consumer!", 
            ProviderID);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        pProvASGroup = &pDDB->CRT.Prov.pASGroupEntry[pProvider->AutoStopConsumerID];

        /* ------------------------------------------------------------ */
        /* Remove this provider from AutoStop-GroupList                 */
        /* ------------------------------------------------------------ */
        if ((Prev == EDDP_CRT_CHAIN_IDX_END) || (Next == EDDP_CRT_CHAIN_IDX_END))
        {
            if (Prev == EDDP_CRT_CHAIN_IDX_END)
            {
                // this provider is first provider in AutoStop-GroupList
                if (Next == EDDP_CRT_CHAIN_IDX_END)
                {
                    // first and only one provider in AutoStop-GroupList
                    pProvASGroup->ASGroupList.BeginIdx  = EDDP_CRT_CHAIN_IDX_END;
                    pProvASGroup->ASGroupList.EndIdx    = EDDP_CRT_CHAIN_IDX_END;
                }
                else
                {
                    // first provider in AutoStop-GroupList but not only one
                    pProvASGroup->ASGroupList.BeginIdx           = Next;
                    pProvMngm->pProvEntry[Next].ASGroupLink.Prev = EDDP_CRT_CHAIN_IDX_END;
                }
            }
            else
            {
                // last provider in AutoStop-GroupList but not only one
                pProvMngm->pProvEntry[Prev].ASGroupLink.Next = EDDP_CRT_CHAIN_IDX_END;
                pProvASGroup->ASGroupList.EndIdx             = Prev;
            }
        }
        else
        {
            // this provider is in the middle of the AutoStop-GroupList
            pProvMngm->pProvEntry[Prev].ASGroupLink.Next = Next;
            pProvMngm->pProvEntry[Next].ASGroupLink.Prev = Prev;
        }

        // delete the AutoStopConsumerID in provider
        pProvider->AutoStopConsumerID = EDD_CONSUMER_ID_REMOVE_AS;

        EDDP_ASSERT_FALSE(pProvASGroup->ProviderCnt == 0);
        pProvASGroup->ProviderCnt--;
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProviderAutoStopControl                             +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             LSA_UINT16                          ConsumerID              +*/
/*+                             LSA_BOOL                            IsEnable                +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ConsumerID         : Index in consumer array                                           +*/
/*+  IsEnable           : LSA_TRUE  - Provider-AutoStop enable                              +*/
/*+                       LSA_FALSE - Provider-AutoStop disable                             +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: This function is called by consumer if Provider-AutoStop                  +*/
/*+               shall be switched enable or disable.                                      +*/
/*+               Consumer must be used in CRT. If no -> FATAL!                             +*/
/*+                                                                                         +*/
/*+                 - Enable    : activate the "Scoreboard_offset" in the send ACWs         +*/
/*+                 - Disable   : deactivate the "Scoreboard_offset" in the send ACWs       +*/
/*+                                                                                         +*/
/*+               Note: It is also possible that no providers coupling to this consumer     +*/
/*+               exists yet.                                                               +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderAutoStopControl(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ConsumerID,
    LSA_BOOL                    IsEnable)
{
    LSA_UINT16                              ProvIdx;
    LSA_UINT16                              ACW_SB_offset;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_ASGROUP_PTR_TYPE      pProvASGroup;

    EDDP_ASSERT_FALSE(ConsumerID >= pDDB->CRT.CfgParam.ConsumerMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pProvASGroup = &pDDB->CRT.Prov.pASGroupEntry[ConsumerID];

    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTProviderAutoStopControl(ConsumerID=%d, IsEnable=0x%X): ASGroup-ProviderCnt=%d", 
        ConsumerID, IsEnable, pProvASGroup->ProviderCnt);

    if (EDDP_CRTGetConsumerState(pDDB, ConsumerID) == EDDP_CRT_STATE_FREE)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderAutoStopControl(ConsumerID=%d): FATAL -> This Consumer is not used!", 
            ConsumerID);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        /* ------------------------ */
        /* Enable or Disable ?      */
        /* ------------------------ */
        if (IsEnable)
        {
            // activate the "Scoreboard_offset"
            ACW_SB_offset = EDDP_CRTGetShiftedTSBOffset (pDDB, ConsumerID);
        }
        else
        {
            // deactivate the "Scoreboard_offset"
            ACW_SB_offset = PNIP_R2_ACW_SND_WORD3_SCOREBOARD_OFFSET_OFF_VALUE;
        }

        /* ---------------------------------------------------------------- */
        /* write new "ACW.Scoreboard_offset"                                */
        /* - loop through all providers of AutoStop-GroupList               */
        /* - if no providers coupling exist, nothing to do                  */
        /* ---------------------------------------------------------------- */
        ProvIdx = pProvASGroup->ASGroupList.BeginIdx;
        while (ProvIdx != EDDP_CRT_CHAIN_IDX_END)
        {
            EDDP_ASSERT_FALSE(ProvIdx >= pDDB->CRT.CfgParam.ProviderMaxNumber);
            // write the "Scoreboard_offset" to all ACWs of this provider
            pDDB->CRT.Prov.AcwHandler.FctWriteSBOffset (pDDB, ProvIdx, ACW_SB_offset);
            // next ProvIdx in this AutoStop-GroupList
            ProvIdx = pProvMngm->pProvEntry[ProvIdx].ASGroupLink.Next;
        }
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProviderDeleteDataStatusHwApi                   +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT16                      DataStatusId            +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  DataStatusId   : Index in DataStatus array                                         +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function deletes one DataStatus in APICtrl-RAM.                  +*/
/*+               Call this function only if provider is removed from CRT management!   +*/
/*+                                                                                     +*/
/*+               Calculate the parameter for function EDDP_HALMemset()                 +*/
/*+               and call this function.                                               +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderDeleteDataStatusHwApi(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  DataStatusId)
{
    EDD_RSP                             Response;
    LSA_UINT32                          RamSubApiCtrl;
    LSA_UINT32                          ApiAdrTemp1;
    LSA_UINT32                          ApiAdrTemp2;
    LSA_UINT32                          IntOffs;
    EDDP_CRT_PROVIDER_DS_PTR_TYPE       pDataStatus;

    EDDP_ASSERT_FALSE(DataStatusId >= pDDB->CRT.CfgParam.ProviderDataStatusMaxNumber);

    Response        = EDD_STS_OK;
    pDataStatus     = &pDDB->CRT.Prov.pDSEntry[DataStatusId];
    RamSubApiCtrl   = EDDP_CRT_INVALID_VALUE_32;    // init to prevent compiler warning

    RamSubApiCtrl = EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_DS_LIST;    // IFA

    // switch to current PNIP revision
    switch (EDDP_HALGetPnipRevision(pDDB))
    {
        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
        case EDDP_HAL_HW_PNIP_REV1:
        {
            EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProviderDeleteDataStatusHwApi(DataStatusId=%d): pHwBaseDSApiPnipR1=0x%X, pHwDSApiPnipR1=0x%X", 
                DataStatusId, pDDB->CRT.Prov.pHwBaseDSApiPnipR1, pDataStatus->pHwDSApiPnipR1);

            ApiAdrTemp1 = (LSA_UINT32) pDDB->CRT.Prov.pHwBaseDSApiPnipR1;
            ApiAdrTemp2 = (LSA_UINT32) pDataStatus->pHwDSApiPnipR1;
            EDDP_ASSERT_FALSE(ApiAdrTemp1 > ApiAdrTemp2);
            IntOffs = ApiAdrTemp2 - ApiAdrTemp1;

            #ifdef EDDP_CFG_SYSRED_API_SUPPORT
            EDDP_ENTER_IO(); // Lock the area
            #endif
            Response = EDDP_HALMemset ( pDDB, 
                                        EDDP_HAL_RAMTYPE_API_CTRL, 
                                        RamSubApiCtrl, 
                                        IntOffs, 
                                        (sizeof(PNIP_R1_ACW_SND_DS_TYPE)), 
                                        0);
            #ifdef EDDP_CFG_SYSRED_API_SUPPORT
            EDDP_EXIT_IO(); // release memory lock
            #endif

            if (Response != EDD_STS_OK)
            {
                EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderDeleteDataStatusHwApi(): FATAL -> Response(0x%X) of EDDP_HALMemset() is not OK!, IntOffs=0x%X, Length=0x%X",
                    Response, IntOffs, (sizeof(PNIP_R1_ACW_SND_DS_TYPE)));
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
        }
        break;
        #endif

        #if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
        case EDDP_HAL_HW_PNIP_REV2:
        case EDDP_HAL_HW_PNIP_REV3:
        {
            EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProviderDeleteDataStatusHwApi(DataStatusId=%d): pHwBaseDSApiPnipR2=0x%X, pHwDSApiPnipR2=0x%X", 
                DataStatusId, pDDB->CRT.Prov.pHwBaseDSApiPnipR2, pDataStatus->pHwDSApiPnipR2);

            ApiAdrTemp1 = (LSA_UINT32) pDDB->CRT.Prov.pHwBaseDSApiPnipR2;
            ApiAdrTemp2 = (LSA_UINT32) pDataStatus->pHwDSApiPnipR2;
            EDDP_ASSERT_FALSE(ApiAdrTemp1 > ApiAdrTemp2);
            IntOffs = ApiAdrTemp2 - ApiAdrTemp1;

            #ifdef EDDP_CFG_SYSRED_API_SUPPORT
            EDDP_ENTER_IO(); // Lock the area
            #endif
            Response = EDDP_HALMemset ( pDDB, 
                                        EDDP_HAL_RAMTYPE_API_CTRL, 
                                        RamSubApiCtrl, 
                                        IntOffs, 
                                        (sizeof(PNIP_R2_ACW_SND_DS_TYPE)), 
                                        0);
            #ifdef EDDP_CFG_SYSRED_API_SUPPORT
            EDDP_EXIT_IO(); // release memory lock
            #endif

            if (Response != EDD_STS_OK)
            {
                EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderDeleteDataStatusHwApi(): FATAL -> Response(0x%X) of EDDP_HALMemset() is not OK!, IntOffs=0x%X, Length=0x%X",
                    Response, IntOffs, (sizeof(PNIP_R2_ACW_SND_DS_TYPE)));
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
        }
        break;
        #endif

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderDeleteDataStatusHwApi(): FATAL -> Version(%d) of PNIP is unknown", 
                EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTProviderIOParamUpdateForActivate                    +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB            +*/
/*+                             LSA_UINT16                                  ProviderID      +*/
/*+                             EDD_UPPER_CSRT_PROVIDER_CONTROL_PTR_TYPE    pRqbParam       +*/
/*+                                                                                         +*/
/*+  Result                :    LSA_VOID                                                    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ProviderID         : ProviderID in CRT                                                 +*/
/*+  pRqbParam          : pointer to RQB parameter from user (UPPER-RQB)            (in)    +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: This function copies all parameters to Provider/ACW to activate the ACW.  +*/
/*+                                                                                         +*/
/*+               IMPORTANT: The data of parameters are only copied                         +*/
/*+               to provider if these data are still UNDEFINED, because these parameters   +*/
/*+               can only be changed before first ACTIVATE and only one times!             +*/
/*+                                                                                         +*/
/*+               This function does not check whether the parameters are valid.            +*/
/*+                                                                                         +*/
/*+               If provider state is FREE -> FATAL!                                       +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderIOParamUpdateForActivate(
    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB,
    LSA_UINT16                                  ProviderID,
    EDD_UPPER_CSRT_PROVIDER_CONTROL_PTR_TYPE    pRqbParam)
{
    LSA_UINT16                              AcwIdx;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE              pProvider;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;

    EDDP_ASSERT_FALSE(ProviderID >= pDDB->CRT.CfgParam.ProviderMaxNumber);

    pProvMngm    = &pDDB->CRT.Prov;
    pProvider    = &pProvMngm->pProvEntry[ProviderID];

    if (pProvider->State == EDDP_CRT_STATE_FREE)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderIOParamUpdateForActivate(ProviderID=%d): FATAL -> Provider-State=FREE!", 
            ProviderID);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        /* -------------------------------- */
        /* CyclePhase parameter             */
        /* for all ACWs of this provider    */
        /* -------------------------------- */
        AcwIdx = pProvider->AcwList.BeginIdx;
        while (AcwIdx != EDDP_CRT_CHAIN_IDX_END)
        {
            EDDP_ASSERT_FALSE(AcwIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

            pAcw = &pProvMngm->pAcwEntry[AcwIdx];

            // CycleReductionRatio
            if (pAcw->AddParam.CycleReductionRatio == EDD_CYCLE_REDUCTION_RATIO_UNDEFINED)
                pAcw->AddParam.CycleReductionRatio = pRqbParam->CycleReductionRatio;

            // CyclePhase
            if (pAcw->AddParam.CyclePhase == EDD_CYCLE_PHASE_UNDEFINED)
                pAcw->AddParam.CyclePhase = pRqbParam->CyclePhase;

            // CyclePhaseSequence
            if (pAcw->AddParam.CyclePhaseSequence == EDD_CYCLE_PHASE_SEQUENCE_UNDEFINED)
                pAcw->AddParam.CyclePhaseSequence = pRqbParam->CyclePhaseSequence;

            // next AcwIdx in this provider
            AcwIdx = pProvMngm->pAcwEntry[AcwIdx].AcwProvLink.Next;
        }

        /* -------------------------------- */
        /* IOParams                         */
        /* -------------------------------- */
        // BufferProperties
		if (EDD_PROV_BUFFER_PROP_HERA_MEM_TYPE_UNDEFINED == (pProvider->AddParam.BufferProperties & EDD_PROV_BUFFER_PROP_HERA_MEM_TYPE_MASK))
		{
			pProvider->AddParam.BufferProperties = pRqbParam->IOParams.BufferProperties;
		}

        // CRNumber
        if (pProvider->AddParam.CRNumber == EDD_CRNUMBER_UNDEFINED)
        {
            pProvider->AddParam.CRNumber = pRqbParam->IOParams.CRNumber;

            #if defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)  
            if(pDDB->Hardware == EDDP_HW_ERTEC200P || pDDB->Hardware == EDDP_HW_ERTEC200P_FPGA)
            {
                if (pProvider->AddParam.CRNumber == EDD_CRNUMBER_NOT_USED)
                {
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderIOParamUpdateForActivate(): FATAL -> CRNumber(%d) for extarnel RAM is not supported!", 
                            pProvider->AddParam.CRNumber);
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }
            }
            #endif
        }

        // AppGroup
        if (pProvider->AddParam.AppGroup == EDD_APPGROUP_UNDEFINED)
            pProvider->AddParam.AppGroup = pRqbParam->IOParams.AppGroup;

        // DataOffset
        if (pProvider->AddParam.DataOffset == EDD_DATAOFFSET_UNDEFINED)
            pProvider->AddParam.DataOffset = pRqbParam->IOParams.DataOffset;

        /* ------------------------------------------------ */
        /* following parameters only for DFP-Provider       */
        /* ------------------------------------------------ */
        if (pProvider->ProviderFlags & EDDP_CRT_PROV_FLAGS_IS_DFP)
        {
            // SFPosition
            if (pProvider->DfpParam.SFPosition == EDD_SFPOSITION_UNDEFINED)
                pProvider->DfpParam.SFPosition = pRqbParam->IOParams.SFPosition;

            // SFOffset
            if (pProvider->DfpParam.SFOffset == EDD_SFOFFSET_UNDEFINED)
                pProvider->DfpParam.SFOffset = pRqbParam->IOParams.SFOffset;
        }
    }
}
