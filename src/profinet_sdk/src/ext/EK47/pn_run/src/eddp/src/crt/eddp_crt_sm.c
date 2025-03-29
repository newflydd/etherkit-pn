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
/*  F i l e               &F: eddp_crt_sm.c                             :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  CRT (Cycle Real Time) for EDDP  -> State Machines                        */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who         What                                             */
#ifdef EDDP_MESSAGE
/*  17.11.10    Sergeev     Erstellung                                       */
#endif
/*****************************************************************************/


/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/

#define LTRC_ACT_MODUL_ID   36
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_CRT_SM */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"           /* eddp headerfiles */
#include "eddp_int.h"           /* internal header  */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :  EDDP_CRTGetProvAcwDGActivateStateMachine      +*/
/*                                                                          +*/
/*+  Input/Output          :  EDDP_LOCAL_DDB_PTR_TYPE     pDDB              +*/
/*+                           LSA_UINT16                  ProviderId        +*/
/*+                           EDDP_CRT_CBF_PTR_TYPE       Cbf               +*/
/*+  Input/Output          :  EDD_UPPER_RQB_PTR_TYPE      pRQB              +*/
/*+                                                                         +*/
/*+  Result                :  EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_SM_PTR_TYPE +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  ProviderId : ProviderID for provider that is to be activated           +*/
/*+  Cbf        : Callback function for finishing the request               +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+  Results    : Reference to DG activation state machine                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Get activation state machine for activating a DG.         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_SM_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_CRTGetProvAcwDGActivateStateMachine(
    EDDP_LOCAL_DDB_PTR_TYPE       pDDB,
    LSA_UINT16                    ProviderId,
    EDDP_CRT_PROV_DG_CBF_PTR_TYPE Cbf,
    EDD_UPPER_RQB_PTR_TYPE        pRQB)
{
    LSA_UINT16 Index;
    LSA_BOOL   Found = LSA_FALSE;

    for(Index = 0 ; Index < EDDP_CRT_PROVIDER_ACW_DG_STATE_MACHINE_DEPTH ; Index++)
    {
        if(!pDDB->CRT.Prov.AcwDGActivateSm[Index].InUse)
        {
            pDDB->CRT.Prov.AcwDGActivateSm[Index].InUse      = LSA_TRUE;
            pDDB->CRT.Prov.AcwDGActivateSm[Index].Cbf        = Cbf;
            pDDB->CRT.Prov.AcwDGActivateSm[Index].ProviderId = ProviderId;
            pDDB->CRT.Prov.AcwDGActivateSm[Index].State      = EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_START;
            pDDB->CRT.Prov.AcwDGActivateSm[Index].pRQB       = pRQB;
            pDDB->CRT.Prov.AcwDGActivateSm[Index].CmdCnt     = 0;
            pDDB->CRT.Prov.AcwDGActivateSm[Index].Index      = Index;
            Found = LSA_TRUE;
            break;
        }
    }

    if(!Found)
    {
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTGetProvAcwDGActivateStateMachine(): FATAL -> no free state machine found!");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTGetProvAcwDGActivateStateMachine(): found -> index=%d!", Index);
    }

    return (&pDDB->CRT.Prov.AcwDGActivateSm[Index]);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :  EDDP_CRTGetProvAcwDGPassivateStateMachine     +*/
/*                                                                          +*/
/*+  Input/Output          :  EDDP_LOCAL_DDB_PTR_TYPE     pDDB              +*/
/*+                           LSA_UINT16                  ProviderId        +*/
/*+                           EDDP_CRT_CBF_PTR_TYPE       Cbf               +*/
/*+  Input/Output          :  EDD_UPPER_RQB_PTR_TYPE      pRQB              +*/
/*+                                                                         +*/
/*+  Result                :  EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_SM_PTR_TYPE+*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  ProviderId : ProviderID for provider that is to be activated           +*/
/*+  Cbf        : Callback function for finishing the request               +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+  Results    : Reference to DG passivation state machine                 +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Get passivation state machine for passivating a DG.       +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_SM_PTR_TYPE EDDP_LOCAL_FCT_ATTR EDDP_CRTGetProvAcwDGPassivateStateMachine(
    EDDP_LOCAL_DDB_PTR_TYPE       pDDB,
    LSA_UINT16                    ProviderId,
    EDDP_CRT_PROV_DG_CBF_PTR_TYPE Cbf,
    EDD_UPPER_RQB_PTR_TYPE        pRQB)
{
    LSA_UINT16 Index;
    LSA_BOOL   Found = LSA_FALSE;

    for(Index = 0 ; Index < EDDP_CRT_PROVIDER_ACW_DG_STATE_MACHINE_DEPTH ; Index++)
    {
        if(!pDDB->CRT.Prov.AcwDGPassivateSm[Index].InUse)
        {
            pDDB->CRT.Prov.AcwDGPassivateSm[Index].InUse      = LSA_TRUE;
            pDDB->CRT.Prov.AcwDGPassivateSm[Index].Cbf        = Cbf;
            pDDB->CRT.Prov.AcwDGPassivateSm[Index].ProviderId = ProviderId;
            pDDB->CRT.Prov.AcwDGPassivateSm[Index].State      = EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_START;
            pDDB->CRT.Prov.AcwDGPassivateSm[Index].pRQB       = pRQB;
            pDDB->CRT.Prov.AcwDGPassivateSm[Index].CmdCnt     = 0;
            pDDB->CRT.Prov.AcwDGPassivateSm[Index].Index      = Index;
            Found = LSA_TRUE;
            break;
        }
    }

    if(!Found)
    {
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTGetProvAcwDGPassivateStateMachine(): FATAL -> no free state machine found!");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTGetProvAcwDGPassivateStateMachine(): found -> index=%d!", Index);
    }

    return (&pDDB->CRT.Prov.AcwDGPassivateSm[Index]);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTReleaseProvAcwDGActivateStateMachine+*/
/*                                                                          +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE      pRQB            +*/
/*+                             LSA_UINT16                  Index           +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  Index      : Index of state machine that is to be freed                +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Release activation state machine.                         +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTReleaseProvAcwDGActivateStateMachine(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  Index)
{
    if(!pDDB->CRT.Prov.AcwDGActivateSm[Index].InUse)
    {
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTReleaseProvAcwDGActivateStateMachine(): FATAL -> state machine not in use!");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    
    pDDB->CRT.Prov.AcwDGActivateSm[Index].InUse      = LSA_FALSE;
    pDDB->CRT.Prov.AcwDGActivateSm[Index].Cbf        = LSA_NULL;
    pDDB->CRT.Prov.AcwDGActivateSm[Index].ProviderId = EDDP_CRT_CHAIN_IDX_END;
    pDDB->CRT.Prov.AcwDGActivateSm[Index].State      = EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_IDLE;
    pDDB->CRT.Prov.AcwDGActivateSm[Index].pRQB       = LSA_NULL;
    pDDB->CRT.Prov.AcwDGActivateSm[Index].CmdCnt     = 0;
    pDDB->CRT.Prov.AcwDGActivateSm[Index].Index      = 0;
    
    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTReleaseProvAcwDGActivateStateMachine(): free'd -> index=%d!", Index);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :   EDDP_CRTReleaseProvAcwDGPassivateStateMachine+*/
/*                                                                          +*/
/*+  Input/Output          :   EDD_UPPER_RQB_PTR_TYPE      pRQB             +*/
/*+                            LSA_UINT16                  Index            +*/
/*+                                                                         +*/
/*+  Result                :   LSA_VOID                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+  Index      : Index of state machine that is to be freed                +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: Release passivation state machine.                        +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTReleaseProvAcwDGPassivateStateMachine(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  Index)
{
    if(!pDDB->CRT.Prov.AcwDGPassivateSm[Index].InUse)
    {
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTReleaseProvAcwDGPassivateStateMachineIndex(): FATAL -> state machine not in use!");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    
    pDDB->CRT.Prov.AcwDGPassivateSm[Index].InUse      = LSA_FALSE;
    pDDB->CRT.Prov.AcwDGPassivateSm[Index].Cbf        = LSA_NULL;
    pDDB->CRT.Prov.AcwDGPassivateSm[Index].ProviderId = EDDP_CRT_CHAIN_IDX_END;
    pDDB->CRT.Prov.AcwDGPassivateSm[Index].State      = EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_IDLE;
    pDDB->CRT.Prov.AcwDGPassivateSm[Index].pRQB       = LSA_NULL;
    pDDB->CRT.Prov.AcwDGPassivateSm[Index].CmdCnt     = 0;
    pDDB->CRT.Prov.AcwDGPassivateSm[Index].Index      = 0;
    
    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTReleaseProvAcwDGPassivateStateMachineIndex(): free'd -> index=%d!", Index);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProviderPackGroupActivateAllStateMachine        +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             EDD_RSP                         ResponseSm              +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ResponseSm     : Response of last action                                           +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: State machine of Provider-PackGroupActivate-All.                      +*/
/*+                                                                                     +*/
/*+               If no provider pack group in CRT is present, nothing to do.           +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderPackGroupActivateAllStateMachine(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_RSP                     ResponseSm)
{
    EDD_RSP                                                     Response;
    LSA_UINT16                                                  NextPackGrId;
    LSA_UINT16                                                  ActPackGrId;
    EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_TYPE        ActState;
    EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_TYPE        NextState;
    LSA_UINT16                                                  AcwIdx;
    LSA_UINT16                                                  SubframeBlockIdx;
    LSA_BOOL                                                    Ready;
    EDDP_CRT_CBF_PTR_TYPE                                       CbfTemp;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE                             pProvMngm;
    EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_SM_PTR_TYPE       pStateMachine;

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CRTProviderPackGroupActivateAllStateMachine(ResponseSm=0x%X)", ResponseSm);

    Response        = EDD_STS_OK;
    pProvMngm       = &pDDB->CRT.Prov;
    pStateMachine   = &pDDB->CRT.Prov.PackGroupActivateAllSm;
    ActState        = pStateMachine->State;     // actual state of state machine
    NextState       = ActState;
    ActPackGrId     = pStateMachine->ActPackGrId;
    NextPackGrId    = ActPackGrId;
    Ready           = LSA_FALSE;
    CbfTemp         = 0;

    if (ResponseSm != EDD_STS_OK)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderPackGroupActivateAllStateMachine(): FATAL -> INCONSISTENCE! ResponseSm(0x%X) is not Ok",
            ResponseSm);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    /* ------------------------------------------------------------------------ */
    /* finish handling of actual state before go to next state                  */
    /* ------------------------------------------------------------------------ */
    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProviderPackGroupActivateAllStateMachine(): ActState(%d)",
        ActState);

    switch (ActState)
    {
        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_START:
        /* ------------------------------------------------------------------------ */
        {
            EDDP_ASSERT_NO_NULL_FCT_PTR(pStateMachine->Cbf);
            pStateMachine->Response = EDD_STS_OK;

            // get first pack group from PackGroupUsedList
            NextPackGrId = pProvMngm->PackGroupUsedList.BeginIdx;

            if (NextPackGrId == EDDP_CRT_CHAIN_IDX_END)
            {
                // no pack groups in PackGroupUsedList
                // finish the state machine
                NextState = EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_FINISH;
            }
            else
            {
                // store this PackGrId in state machine
                pStateMachine->ActPackGrId = NextPackGrId;
                NextState = EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_NEXT_PACK_GROUP;
            }
        }
        break;

        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_NEXT_PACK_GROUP:
        /* ------------------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            EDDP_ASSERT_FALSE(ActPackGrId >= pDDB->CRT.CfgParam.ProviderPackGroupMaxNumber);

            // get next pack group from PackGroupUsedList
            NextPackGrId = pProvMngm->pProvPackGroupEntry[ActPackGrId].UsedFreeLink.Next;

            if (NextPackGrId == EDDP_CRT_CHAIN_IDX_END)
            {
                // no more pack groups in PackGroupUsedList
                // finish the state machine
                NextState = EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_FINISH;
            }
            else
            {
                // store this PackGrId in state machine
                pStateMachine->ActPackGrId = NextPackGrId;
                NextState = EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_NEXT_PACK_GROUP;
            }
        }
        break;

        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_ACW_IN:
        /* ------------------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            EDDP_ASSERT_FALSE(ActPackGrId >= pDDB->CRT.CfgParam.ProviderPackGroupMaxNumber);

            // get next pack group from PackGroupUsedList
            NextPackGrId = pProvMngm->pProvPackGroupEntry[ActPackGrId].UsedFreeLink.Next;

            if (NextPackGrId == EDDP_CRT_CHAIN_IDX_END)
            {
                // no more pack groups in PackGroupUsedList
                // finish the state machine
                NextState = EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_FINISH;
            }
            else
            {
                // store this PackGrId in state machine
                pStateMachine->ActPackGrId = NextPackGrId;
                NextState = EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_NEXT_PACK_GROUP;
            }
        }
        break;

        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_FINISH:
        default:
        /* ------------------------------------------------------------------------ */
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderPackGroupActivateAllStateMachine(): FATAL -> Unexpected ActState(%d)",
                ActState);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }

    /* ------------------------------------------------------------------------ */
    /* forward the state machine to next state                                  */
    /* ------------------------------------------------------------------------ */
    while (!Ready)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProviderPackGroupActivateAllStateMachine(): loop -> NextState(%d)",
            NextState);

        pStateMachine->State = NextState;

        switch (NextState)
        {
            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_NEXT_PACK_GROUP:
            /* ------------------------------------------------------------------------ */
            {
                EDDP_ASSERT_FALSE(NextPackGrId >= pDDB->CRT.CfgParam.ProviderPackGroupMaxNumber);
                
                // get SubframeBlockIdx of this pack group
                AcwIdx = pProvMngm->pProvPackGroupEntry[NextPackGrId].AcwId;
                SubframeBlockIdx = pProvMngm->pAcwEntry[AcwIdx].DfpParam.PrmSubframeBlockIdx;
                EDDP_ASSERT_FALSE(SubframeBlockIdx == EDDP_CRT_CHAIN_IDX_END);

                // which DFPmode has this pack group ?
                if (pDDB->PRM.pPDIRSubframeData_A->SubframeBlockArray[SubframeBlockIdx].SFIOCRProperties.DFPmode == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_DFPMODE_END_NODE)
                {
                    // this is Controller/MultiDevice
                    // the send ACW will be activated on Provider-Activate, if at least one ACW_DG is active

                    // get next pack group from PackGroupUsedList
                    NextPackGrId = pProvMngm->pProvPackGroupEntry[NextPackGrId].UsedFreeLink.Next;

                    if (NextPackGrId == EDDP_CRT_CHAIN_IDX_END)
                    {
                        // no more pack groups in PackGroupUsedList
                        // finish the state machine
                        NextState = EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_FINISH;
                    }
                    else
                    {
                        // store this PackGrId in state machine
                        pStateMachine->ActPackGrId = NextPackGrId;
                        NextState = EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_NEXT_PACK_GROUP;
                    }
                }
                else
                {
                    // this is Device - activate the send ACW
                    // set Active-Bit in ACW before CmdRequest is called (maybe not necessary for Device) 
                    EDDP_CRTProvAcwSetActiveBitHwApi(pDDB, AcwIdx);
                    
                    NextState = EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_ACW_IN;
                }
            }
            break;

            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_ACW_IN:
            /* ------------------------------------------------------------------------ */
            {
                EDDP_CMD_PARAM_PTR_TYPE pNewCmdParam = EDDP_CRTCmdGetInternalCmdParamElement(pDDB);

                EDDP_ASSERT_FALSE(NextPackGrId >= pDDB->CRT.CfgParam.ProviderPackGroupMaxNumber);
                EDDP_ASSERT_FALSE(pProvMngm->pProvPackGroupEntry[NextPackGrId].AcwId == EDDP_CRT_CHAIN_IDX_END);

                // remember data for CMD-Interface
                pNewCmdParam->param_union.Crt.Ppm.ACW_InOut.AcwId = pProvMngm->pProvPackGroupEntry[NextPackGrId].AcwId;
                pNewCmdParam->param_union.Crt.Ppm.ACW_InOut.ProviderId = EDDP_CRT_CHAIN_IDX_END;
                // call CmdRequest
                // if command is finished, the Cbf EDDP_CRTCmdDoneCbfPpmPackGroup() will be called
                EDDP_CRTCmdRequestPpm (pDDB, EDDP_CRT_CMD_PPM_PACK_ACW_IN, EDDP_CRT_CMD_ASYNCHRON, pNewCmdParam);

                Ready = LSA_TRUE;
            }
            break;

            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_FINISH:
            /* ------------------------------------------------------------------------ */
            {
                // finish the state machine
                Ready                   = LSA_TRUE;
                pStateMachine->State    = EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_IDLE;

                // go back to caller
                EDDP_ASSERT_NO_NULL_FCT_PTR(pStateMachine->Cbf);
                CbfTemp             = pStateMachine->Cbf;
                pStateMachine->Cbf  = 0;
                CbfTemp(pDDB, EDD_STS_OK);
                // Important: no action after calling of Cbf because the next RQB could be taken from queue within Cbf and executed asynchronous !!!
            }
            break;

            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_START:
            default:
            /* ------------------------------------------------------------------------ */
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderPackGroupActivateAllStateMachine(): FATAL -> Unexpected NextState(%d)",
                    NextState);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTProviderPackGroupActivateAllStateMachine(): Response=0x%X", Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProviderPackGroupActivateAllStateMachineCbf     +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             EDD_RSP                         Response                +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  Response       : Response of last action                                           +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Callbackfunction of Provider-Pack-Group-Activate state machine.       +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderPackGroupActivateAllStateMachineCbf(
  EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
  EDD_RSP                     Response)
{
    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProviderPackGroupActivateAllStateMachineCbf(): Response(0x%X)",
        Response);

    EDDP_CRTProviderPackGroupActivateAllStateMachine (pDDB, Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTActivatePackStateMachine                        +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             EDD_RSP                         ResponseSm              +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ResponseSm     : Response of last action                                           +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: State machine "CRT-Activate-Pack".                                    +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTActivatePackStateMachine(
  EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
  EDD_RSP                     ResponseSm)
{
    EDD_RSP                                     Response;
    EDDP_CRT_PACK_ACTIVATE_STATE_TYPE           ActState;
    EDDP_CRT_PACK_ACTIVATE_STATE_TYPE           NextState;
    LSA_BOOL                                    Ready;
    EDDP_CRT_CBF_PTR_TYPE                       CbfTemp;
    EDDP_CRT_PACK_ACTIVATE_SM_PTR_TYPE          pStateMachine;

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CRTActivatePackStateMachine(ResponseSm=0x%X)", ResponseSm);

    Response        = EDD_STS_OK;
    pStateMachine   = &pDDB->CRT.PackActivateSm;
    ActState        = pStateMachine->State;     // actual state of state machine
    NextState       = ActState;
    Ready           = LSA_FALSE;
    CbfTemp         = 0;

    if (ResponseSm != EDD_STS_OK)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTActivatePackStateMachine(): FATAL -> INCONSISTENCE! ResponseSm=0x%X",
            ResponseSm);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    /* ------------------------------------------------------------------------ */
    /* finish handling of actual state before go to next state                  */
    /* ------------------------------------------------------------------------ */
    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTActivatePackStateMachine(): ActState(%d)",
        ActState);

    switch (ActState)
    {
        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PACK_ACTIVATE_STATE_START:
        /* ------------------------------------------------------------------------ */
        {
            EDDP_ASSERT_NO_NULL_FCT_PTR(pStateMachine->Cbf);
            pStateMachine->Response = EDD_STS_OK;
            NextState               = EDDP_CRT_PACK_ACTIVATE_STATE_SND_ACW_ON;
        }
        break;

        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PACK_ACTIVATE_STATE_SND_ACW_ON:
        /* ------------------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            NextState               = EDDP_CRT_PACK_ACTIVATE_STATE_RCV_ACW_ON;
        }
        break;

        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PACK_ACTIVATE_STATE_RCV_ACW_ON:
        /* ------------------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            NextState               = EDDP_CRT_PACK_ACTIVATE_STATE_INBOUND_PACK_ON;
        }
        break;

        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PACK_ACTIVATE_STATE_INBOUND_PACK_ON:
        /* ------------------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            NextState               = EDDP_CRT_PACK_ACTIVATE_STATE_OUTBOUND_PACK_ON;
        }
        break;

        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PACK_ACTIVATE_STATE_OUTBOUND_PACK_ON:
        /* ------------------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            NextState               = EDDP_CRT_PACK_ACTIVATE_STATE_FINISH;
        }
        break;

        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PACK_ACTIVATE_STATE_FINISH:
        default:
        /* ------------------------------------------------------------------------ */
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTActivatePackStateMachine(): FATAL -> Unexpected ActState(%d)",
                ActState);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }

    /* ------------------------------------------------------------------------ */
    /* forward the state machine to next state                                  */
    /* ------------------------------------------------------------------------ */
    while (!Ready)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTActivatePackStateMachine(): loop -> NextState(%d)",
            NextState);

        pStateMachine->State = NextState;

        switch (NextState)
        {
            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PACK_ACTIVATE_STATE_SND_ACW_ON:
            /* ------------------------------------------------------------------------ */
            {
                // activate provider pack groups
                Response = EDDP_CRTProvDfpPackGroupActivateFromPDIRSubframeData (pDDB, LSA_TRUE);

                switch (Response)
                {
                    case EDD_STS_OK_PENDING:
                        Ready = LSA_TRUE;
                    break;

                    case EDD_STS_OK:
                        NextState = EDDP_CRT_PACK_ACTIVATE_STATE_RCV_ACW_ON;
                    break;

                    default:
                    {
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTActivatePackStateMachine(): FATAL -> Unexpected Response(0x%X) in state SND_ACW_ON",
                            Response);
                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                    }
                    break;
                }
            }
            break;

            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PACK_ACTIVATE_STATE_RCV_ACW_ON:
            /* ------------------------------------------------------------------------ */
            {
                // activate consumer pack groups
                Response = EDDP_CRTRcvAcwDGStateMachine (pDDB, EDDP_CRT_SET_RCV_ACWDG_ON, EDDP_CRTActivatePackStateMachineCbf);

                switch (Response)
                {
                    case EDD_STS_OK_PENDING:
                        Ready = LSA_TRUE;
                    break;

                    case EDD_STS_OK:
                        NextState = EDDP_CRT_PACK_ACTIVATE_STATE_INBOUND_PACK_ON;
                    break;

                    default:
                    {
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTActivatePackStateMachine(): FATAL -> Unexpected Response(0x%X) in state RCV_ACW_ON",
                            Response);
                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                    }
                    break;
                }
            }
            break;

            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PACK_ACTIVATE_STATE_INBOUND_PACK_ON:
            /* ------------------------------------------------------------------------ */
            {
                // activate InBound pack
                Response = EDDP_CRTInboundStateMachine (pDDB, EDDP_CRT_SET_INBOUND_ON, EDDP_CRTActivatePackStateMachineCbf);

                switch (Response)
                {
                    case EDD_STS_OK_PENDING:
                        Ready = LSA_TRUE;
                    break;

                    case EDD_STS_OK:
                        NextState = EDDP_CRT_PACK_ACTIVATE_STATE_OUTBOUND_PACK_ON;
                    break;

                    default:
                    {
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTActivatePackStateMachine(): FATAL -> Unexpected Response(0x%X) in state INBOUND_PACK_ON",
                            Response);
                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                    }
                    break;
                }
            }
            break;

            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PACK_ACTIVATE_STATE_OUTBOUND_PACK_ON:
            /* ------------------------------------------------------------------------ */
            {
                // activate OutBound pack
                Response = EDDP_CRTOutboundStateMachine (pDDB, EDDP_CRT_SET_OUTBOUND_ON, EDDP_CRTActivatePackStateMachineCbf);

                switch (Response)
                {
                    case EDD_STS_OK_PENDING:
                        Ready = LSA_TRUE;
                    break;

                    case EDD_STS_OK:
                        NextState = EDDP_CRT_PACK_ACTIVATE_STATE_FINISH;
                    break;

                    default:
                    {
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTActivatePackStateMachine(): FATAL -> Unexpected Response(0x%X) in state OUTBOUND_PACK_ON",
                            Response);
                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                    }
                    break;
                }
            }
            break;

            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PACK_ACTIVATE_STATE_FINISH:
            /* ------------------------------------------------------------------------ */
            {
                // DFP is active in HW
                EDDP_SET_BIT_VALUE16( (pDDB->SWI.IRTState),
                                      EDDP_SWI_IRTSTATE_BIT_ON,
                                      EDDP_SWI_IRTSTATE_IRT_DFP_MASK,
                                      EDDP_SWI_IRTSTATE_IRT_DFP_SHIFT)

                // finish the state machine
                Ready                   = LSA_TRUE;
                pStateMachine->State    = EDDP_CRT_PACK_ACTIVATE_STATE_IDLE;

                // go back to caller
                EDDP_ASSERT_NO_NULL_FCT_PTR(pStateMachine->Cbf);
                CbfTemp             = pStateMachine->Cbf;
                pStateMachine->Cbf  = 0;
                CbfTemp(pDDB, EDD_STS_OK);
                // Important: no action after calling of Cbf because the next RQB could be taken from queue within Cbf and executed asynchronous !!!
            }
            break;

            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PACK_ACTIVATE_STATE_START:
            default:
            /* ------------------------------------------------------------------------ */
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTActivatePackStateMachine(): FATAL -> Unexpected NextState(%d)",
                    NextState);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTActivatePackStateMachine(): Response=0x%X", Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTActivatePackStateMachineCbf                     +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             EDD_RSP                         Response                +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  Response       : Response of last action                                           +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Callbackfunction of CRT-Activate-Pack state machine.                  +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTActivatePackStateMachineCbf(
  EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
  EDD_RSP                     Response)
{
    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTActivatePackStateMachineCbf(): Response(0x%X)", Response);

    EDDP_CRTActivatePackStateMachine (pDDB, Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTPassivatePackStateMachine                       +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             EDD_RSP                         ResponseSm              +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ResponseSm     : Response of last action                                           +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: State machine "CRT-Passivate-Pack".                                   +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTPassivatePackStateMachine(
  EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
  EDD_RSP                     ResponseSm)
{
    EDD_RSP                                     Response;
    EDDP_CRT_PACK_PASSIVATE_STATE_TYPE          ActState;
    EDDP_CRT_PACK_PASSIVATE_STATE_TYPE          NextState;
    LSA_BOOL                                    Ready;
    EDDP_CRT_CBF_PTR_TYPE                       CbfTemp;
    EDDP_CRT_PACK_PASSIVATE_SM_PTR_TYPE         pStateMachine;

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CRTPassivatePackStateMachine(ResponseSm=0x%X)", ResponseSm);

    Response        = EDD_STS_OK;
    pStateMachine   = &pDDB->CRT.PackPassivateSm;
    ActState        = pStateMachine->State;     // actual state of state machine
    NextState       = ActState;
    Ready           = LSA_FALSE;
    CbfTemp         = 0;

    if (ResponseSm != EDD_STS_OK)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTPassivatePackStateMachine(): FATAL -> INCONSISTENCE! ResponseSm=0x%X",
            ResponseSm);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    /* ------------------------------------------------------------------------ */
    /* finish handling of actual state before go to next state                  */
    /* ------------------------------------------------------------------------ */
    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTPassivatePackStateMachine(): ActState(%d)", ActState);

    switch (ActState)
    {
        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PACK_PASSIVATE_STATE_START:
        /* ------------------------------------------------------------------------ */
        {
            EDDP_ASSERT_NO_NULL_FCT_PTR(pStateMachine->Cbf);
            pStateMachine->Response = EDD_STS_OK;
            NextState               = EDDP_CRT_PACK_PASSIVATE_STATE_WAITING_BREAK;
        }
        break;

        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PACK_PASSIVATE_STATE_WAITING_BREAK:
        /* ------------------------------------------------------------------------ */
        {
            #ifdef EDDP_MESSAGE
            #ifdef EDDP_PRAGMA_MESSAGE
            #pragma EDDP_PRAGMA_MESSAGE("Workaround CRT.WrkarPackGrpDisable for PNIP-Revision(1) - AP01372025")
            #endif /* EDDP_PRAGMA_MESSAGE */
            #endif /* EDDP_MESSAGE */
            #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
            if (EDDP_HAL_HW_PNIP_REV1 == EDDP_HALGetPnipRevision(pDDB))
            {
                pStateMachine->Response = EDD_STS_OK;
                NextState               = EDDP_CRT_PACK_PASSIVATE_STATE_INBOUND_PACK_OFF;
            }
            else
            {
                // this state is synchron
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTPassivatePackStateMachine(): FATAL -> The State(%d) is synchron!",
                    ActState);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            #else
            // this state is synchron
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTPassivatePackStateMachine(): FATAL -> The State(%d) is synchron!",
                ActState);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            #endif
        }
        break;

        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PACK_PASSIVATE_STATE_INBOUND_PACK_OFF:
        /* ------------------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            NextState               = EDDP_CRT_PACK_PASSIVATE_STATE_OUTBOUND_PACK_OFF;
        }
        break;

        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PACK_PASSIVATE_STATE_OUTBOUND_PACK_OFF:
        /* ------------------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            NextState               = EDDP_CRT_PACK_PASSIVATE_STATE_SND_ACW_OFF;
        }
        break;

        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PACK_PASSIVATE_STATE_SND_ACW_OFF:
        /* ------------------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            NextState               = EDDP_CRT_PACK_PASSIVATE_STATE_RCV_ACW_OFF;
        }
        break;

        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PACK_PASSIVATE_STATE_RCV_ACW_OFF:
        /* ------------------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            NextState               = EDDP_CRT_PACK_PASSIVATE_STATE_FINISH;
        }
        break;

        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PACK_PASSIVATE_STATE_FINISH:
        default:
        /* ------------------------------------------------------------------------ */
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTPassivatePackStateMachine(): FATAL -> Unexpected ActState(%d)",
                ActState);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }

    /* ------------------------------------------------------------------------ */
    /* forward the state machine to next state                                  */
    /* ------------------------------------------------------------------------ */
    while (!Ready)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTPassivatePackStateMachine(): loop -> NextState(%d)",
            NextState);

        pStateMachine->State = NextState;

        switch (NextState)
        {
            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PACK_PASSIVATE_STATE_WAITING_BREAK:
            /* ------------------------------------------------------------------------ */
            {
                #ifdef EDDP_MESSAGE
                #ifdef EDDP_PRAGMA_MESSAGE
                #pragma EDDP_PRAGMA_MESSAGE("Workaround CRT.WrkarPackGrpDisable for PNIP-Revision(1) - AP01372025")
                #endif /* EDDP_PRAGMA_MESSAGE */
                #endif /* EDDP_MESSAGE */
                #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
                if (EDDP_HAL_HW_PNIP_REV1 == EDDP_HALGetPnipRevision(pDDB))
                {
                    LSA_UINT8                               InboundActiveMask;
                    LSA_UINT32                              i, RegPack_X, RegValueTemp;
                    LSA_BOOL                                isOutboundActive;
                    EDDP_CRT_INBOUND_MNGM_PTR_TYPE      pInboundMgmt;

                    // InBound
                    pInboundMgmt        = &pDDB->CRT.Inbound;
                    InboundActiveMask   = pInboundMgmt->InboundActiveMask;

                    // OutBound (if no InboundActiveMask)
                    isOutboundActive = LSA_FALSE;
                    if (InboundActiveMask == 0)
                    {
                        RegPack_X = PNIP_REG_OUTBOUNDPACK_0;
                        // loop through all outbound groups
                        for ( i=0; i < EDDP_CRT_MAX_PACK_FRAMES_ERTEC200P; i++ )
                        {
                            RegValueTemp = EDDP_HAL_REG32_READ(pDDB, RegPack_X);
                            if (RegValueTemp & PNIP_REG_OUTBOUNDPACK_0__MSK_OUTBOUND_VALID)
                            {
                                isOutboundActive = LSA_TRUE;            // at least one outbound group is present
                                i = EDDP_CRT_MAX_PACK_FRAMES_ERTEC200P; // exit FOR loop
                            }
                            RegPack_X += 4;
                        }
                    }

                    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] EDDP_CRTPassivatePackStateMachine(): workaround PackGrp-Disable - InboundActiveMask=0x%X, isOutboundActive=0x%X",
                        InboundActiveMask, isOutboundActive);

                    // are some inbound/outbound groups active ?
                    if ((InboundActiveMask != 0) || (isOutboundActive))
                    {
                        // active inbound/outbound groups are present - this state is with waiting
                        if (InboundActiveMask != 0)
                        {
                            // for all active inbound groups set invalid data for workaround PackGrp-Disable
                            RegPack_X = PNIP_REG_INBOUNDPACK_0;
                            for ( i=0; i < EDDP_CRT_MAX_PACK_FRAMES_ERTEC200P; i++ )
                            {
                                if (InboundActiveMask & (1<<i))
                                {
                                    // disable interrupt "ISR_InBound_Late_ERR" of this PackGrp
                                    EDDP_CRTLateErrorSetupLateErrMask (pDDB, (LSA_UINT8)i, LSA_TRUE);
                                    // set invalid "InBoundPack_x.TxPort = 4 (0x3)"
                                    RegValueTemp = EDDP_HAL_REG32_READ(pDDB, RegPack_X);
                                    EDDP_SET_BIT_VALUE32 (RegValueTemp, 0x3, PNIP_REG_INBOUNDPACK_0__MSK_INBOUND_TXPORT, PNIP_REG_INBOUNDPACK_0_SHFT_INBOUND_TXPORT);
                                    EDDP_HAL_REG32_WRITE(pDDB, RegPack_X, RegValueTemp);
                                    // set invalid "ACW.DRM/Destination"
                                    EDDP_ASSERT_FALSE(pInboundMgmt->InboundGrps[i].WrkarPackGrpDisable.AcwId == EDDP_CRT_CHAIN_IDX_END);
                                    EDDP_CRTProvAcwSetupDrmAndDestination (pDDB, pInboundMgmt->InboundGrps[i].WrkarPackGrpDisable.AcwId, LSA_FALSE);
                                    // if redundant FrameID(x+1) is present
                                    if (pInboundMgmt->InboundGrps[i].WrkarPackGrpDisable.AcwIdRed1 != EDDP_CRT_CHAIN_IDX_END)
                                    {
                                        EDDP_CRTProvAcwSetupDrmAndDestination (pDDB, pInboundMgmt->InboundGrps[i].WrkarPackGrpDisable.AcwIdRed1, LSA_FALSE);
                                    }
                                }
                                RegPack_X += 4;
                            }
                        }

                        // start the timer of this timer user
                        EDDP_CRTWrkarPackGrpDisableTimerStart (pDDB, EDDP_CRT_WRKAR_PACKGRP_DISABLE_TIMER_USER_IDX_CRT_PASSIVE_PACK_SM);

                        Response    = EDD_STS_OK_PENDING;
                        Ready       = LSA_TRUE;
                    }
                    else
                    {
                        // no active inbound/outbound groups are present - this state is without waiting
                        NextState = EDDP_CRT_PACK_PASSIVATE_STATE_INBOUND_PACK_OFF;
                    }
                }
                else
                {
                    NextState = EDDP_CRT_PACK_PASSIVATE_STATE_INBOUND_PACK_OFF;
                }
                #else
                NextState = EDDP_CRT_PACK_PASSIVATE_STATE_INBOUND_PACK_OFF;
                #endif
            }
            break;

            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PACK_PASSIVATE_STATE_INBOUND_PACK_OFF:
            /* ------------------------------------------------------------------------ */
            {
                // passivate InBound pack
                Response = EDDP_CRTInboundStateMachine (pDDB, EDDP_CRT_SET_INBOUND_OFF, EDDP_CRTPassivatePackStateMachineCbf);

                switch (Response)
                {
                    case EDD_STS_OK_PENDING:
                        Ready = LSA_TRUE;
                    break;

                    case EDD_STS_OK:
                        NextState = EDDP_CRT_PACK_PASSIVATE_STATE_OUTBOUND_PACK_OFF;
                    break;

                    default:
                    {
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTPassivatePackStateMachine(): FATAL -> Unexpected Response(0x%X) in state INBOUND_PACK_OFF",
                            Response);
                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                    }
                    break;
                }
            }
            break;

            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PACK_PASSIVATE_STATE_OUTBOUND_PACK_OFF:
            /* ------------------------------------------------------------------------ */
            {
                // passivate OutBound pack
                Response = EDDP_CRTOutboundStateMachine (pDDB, EDDP_CRT_SET_OUTBOUND_OFF, EDDP_CRTPassivatePackStateMachineCbf);

                switch (Response)
                {
                    case EDD_STS_OK_PENDING:
                        Ready = LSA_TRUE;
                    break;

                    case EDD_STS_OK:
                        NextState = EDDP_CRT_PACK_PASSIVATE_STATE_SND_ACW_OFF;
                    break;

                    default:
                    {
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTPassivatePackStateMachine(): FATAL -> Unexpected Response(0x%X) in state OUTBOUND_PACK_OFF",
                            Response);
                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                    }
                    break;
                }
            }
            break;

            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PACK_PASSIVATE_STATE_SND_ACW_OFF:
            /* ------------------------------------------------------------------------ */
            {
                // passivate provider pack groups
                Response = EDDP_CRTProvDfpPackGroupPassivate (pDDB);

                switch (Response)
                {
                    case EDD_STS_OK_PENDING:
                        Ready = LSA_TRUE;
                    break;

                    case EDD_STS_OK:
                        NextState = EDDP_CRT_PACK_PASSIVATE_STATE_RCV_ACW_OFF;
                    break;

                    default:
                    {
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTPassivatePackStateMachine(): FATAL -> Unexpected Response(0x%X) in state SND_ACW_OFF",
                            Response);
                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                    }
                    break;
                }
            }
            break;

            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PACK_PASSIVATE_STATE_RCV_ACW_OFF:
            /* ------------------------------------------------------------------------ */
            {
                // passivate consumer pack groups
                Response = EDDP_CRTRcvAcwDGStateMachine (pDDB, EDDP_CRT_SET_RCV_ACWDG_OFF, EDDP_CRTPassivatePackStateMachineCbf);

                switch (Response)
                {
                    case EDD_STS_OK_PENDING:
                        Ready = LSA_TRUE;
                    break;

                    case EDD_STS_OK:
                        NextState = EDDP_CRT_PACK_PASSIVATE_STATE_FINISH;
                    break;

                    default:
                    {
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTPassivatePackStateMachine(): FATAL -> Unexpected Response(0x%X) in state RCV_ACW_OFF",
                            Response);
                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                    }
                    break;
                }
            }
            break;

            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PACK_PASSIVATE_STATE_FINISH:
            /* ------------------------------------------------------------------------ */
            {
                // DFP is not active in HW
                EDDP_SET_BIT_VALUE16( (pDDB->SWI.IRTState),
                                      EDDP_SWI_IRTSTATE_BIT_OFF,
                                      EDDP_SWI_IRTSTATE_IRT_DFP_MASK,
                                      EDDP_SWI_IRTSTATE_IRT_DFP_SHIFT)

                // finish the state machine
                Ready                   = LSA_TRUE;
                pStateMachine->State    = EDDP_CRT_PACK_PASSIVATE_STATE_IDLE;

                // go back to caller
                EDDP_ASSERT_NO_NULL_FCT_PTR(pStateMachine->Cbf);
                CbfTemp             = pStateMachine->Cbf;
                pStateMachine->Cbf  = 0;
                CbfTemp(pDDB, EDD_STS_OK);
                // Important: no action after calling of Cbf because the next RQB could be taken from queue within Cbf and executed asynchronous !!!
            }
            break;

            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PACK_PASSIVATE_STATE_START:
            default:
            /* ------------------------------------------------------------------------ */
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTPassivatePackStateMachine(): FATAL -> Unexpected NextState(%d)",
                    NextState);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTPassivatePackStateMachine(): Response=0x%X", Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTPassivatePackStateMachineCbf                    +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             EDD_RSP                         Response                +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  Response       : Response of last action                                           +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Callbackfunction of CRT-Passivate-Pack state machine.                 +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTPassivatePackStateMachineCbf(
  EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
  EDD_RSP                     Response)
{
    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTPassivatePackStateMachineCbf(): Response(0x%X)", Response);

    EDDP_CRTPassivatePackStateMachine (pDDB, Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProviderPackGroupPassivateAllStateMachine       +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             EDD_RSP                         ResponseSm              +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ResponseSm     : Response of last action                                           +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: State machine of Provider-PackGroupPassivate-All.                     +*/
/*+                                                                                     +*/
/*+               If no provider pack group in CRT is present, nothing to do.           +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderPackGroupPassivateAllStateMachine(
  EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
  EDD_RSP                     ResponseSm)
{
    EDD_RSP                                                     Response;
    LSA_UINT16                                                  NextPackGrId;
    LSA_UINT16                                                  ActPackGrId;
    EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_TYPE       ActState;
    EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_TYPE       NextState;
    LSA_UINT16                                                  AcwIdx;
    LSA_BOOL                                                    Ready;
    EDDP_CRT_CBF_PTR_TYPE                                       CbfTemp;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE                             pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE                              pAcw;
    EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_SM_PTR_TYPE      pStateMachine;

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CRTProviderPackGroupPassivateAllStateMachine(ResponseSm=0x%X)", ResponseSm);

    Response        = EDD_STS_OK;
    pProvMngm       = &pDDB->CRT.Prov;
    pStateMachine   = &pDDB->CRT.Prov.PackGroupPassivateAllSm;
    ActState        = pStateMachine->State;     // actual state of state machine
    NextState       = ActState;
    ActPackGrId     = pStateMachine->ActPackGrId;
    NextPackGrId    = ActPackGrId;
    Ready           = LSA_FALSE;
    CbfTemp         = 0;

    if (ResponseSm != EDD_STS_OK)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderPackGroupPassivateAllStateMachine(): FATAL -> INCONSISTENCE! ResponseSm(0x%X) is not Ok",
            ResponseSm);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    /* ------------------------------------------------------------------------ */
    /* finish handling of actual state before go to next state                  */
    /* ------------------------------------------------------------------------ */
    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProviderPackGroupPassivateAllStateMachine(): ActState(%d)",
        ActState);

    switch (ActState)
    {
        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_START:
        /* ------------------------------------------------------------------------ */
        {
            EDDP_ASSERT_NO_NULL_FCT_PTR(pStateMachine->Cbf);
            pStateMachine->Response = EDD_STS_OK;

            // get first pack group from PackGroupUsedList
            NextPackGrId = pProvMngm->PackGroupUsedList.BeginIdx;

            if (NextPackGrId == EDDP_CRT_CHAIN_IDX_END)
            {
                // no pack groups in PackGroupUsedList
                // finish the state machine
                NextState = EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_FINISH;
            }
            else
            {
                EDDP_ASSERT_FALSE(NextPackGrId >= pDDB->CRT.CfgParam.ProviderPackGroupMaxNumber);
                EDDP_ASSERT_FALSE(pProvMngm->pProvPackGroupEntry[NextPackGrId].AcwId == EDDP_CRT_CHAIN_IDX_END);
                
                // get ACW of this pack group
                AcwIdx = pProvMngm->pProvPackGroupEntry[NextPackGrId].AcwId;
                pAcw = &pProvMngm->pAcwEntry[AcwIdx];
                
                // check if this ACW is still active
                if (pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK)
                {
                    // this is Device - ACWs have to be chained out now
                    // store this PackGrId in state machine
                    pStateMachine->ActPackGrId = NextPackGrId;
                    NextState = EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_NEXT_PACK_GROUP;
                }
                else
                {
                    // this is Controller/MultiDevice - ACWs have already been chained out
                    while (NextPackGrId != EDDP_CRT_CHAIN_IDX_END)
                    {
                        // save the NextPackGrId of ActPackGrId, because ActPackGrId will be removed from PackGroupUsedList now
                        ActPackGrId = NextPackGrId;
                        NextPackGrId = pProvMngm->pProvPackGroupEntry[ActPackGrId].UsedFreeLink.Next;
                        // remove this pack group from CRT management
                        EDDP_CRTProvDfpPackGroupRemoveFromCRTMngm(pDDB, ActPackGrId);
                    }

                    // finish the state machine
                    NextState = EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_FINISH;
                }
            }
        }
        break;

        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_NEXT_PACK_GROUP:
        /* ------------------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            EDDP_ASSERT_FALSE(ActPackGrId >= pDDB->CRT.CfgParam.ProviderPackGroupMaxNumber);

            // get next pack group from PackGroupUsedList
            NextPackGrId = pProvMngm->pProvPackGroupEntry[ActPackGrId].UsedFreeLink.Next;

            if (NextPackGrId == EDDP_CRT_CHAIN_IDX_END)
            {
                // no more pack groups in PackGroupUsedList
                // finish the state machine
                NextState = EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_FINISH;
            }
            else
            {
                // store this PackGrId in state machine
                pStateMachine->ActPackGrId = NextPackGrId;
                NextState = EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_NEXT_PACK_GROUP;
            }
        }
        break;

        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_ACW_OUT:
        /* ------------------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            EDDP_ASSERT_FALSE(ActPackGrId >= pDDB->CRT.CfgParam.ProviderPackGroupMaxNumber);
            // save the NextPackGrId of ActPackGrId, because ActPackGrId will be removed from PackGroupUsedList now
            NextPackGrId = pProvMngm->pProvPackGroupEntry[ActPackGrId].UsedFreeLink.Next;

            // remove this pack group from CRT management
            EDDP_CRTProvDfpPackGroupRemoveFromCRTMngm (pDDB, ActPackGrId);

            if (NextPackGrId == EDDP_CRT_CHAIN_IDX_END)
            {
                // no more pack groups in PackGroupUsedList
                // finish the state machine
                NextState = EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_FINISH;
            }
            else
            {
                // store this PackGrId in state machine
                pStateMachine->ActPackGrId = NextPackGrId;
                NextState = EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_NEXT_PACK_GROUP;
            }
        }
        break;

        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_FINISH:
        default:
        /* ------------------------------------------------------------------------ */
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderPackGroupPassivateAllStateMachine(): FATAL -> Unexpected ActState(%d)",
                ActState);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }

    /* ------------------------------------------------------------------------ */
    /* forward the state machine to next state                                  */
    /* ------------------------------------------------------------------------ */
    while (!Ready)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProviderPackGroupPassivateAllStateMachine(): loop -> NextState(%d)",
            NextState);

        pStateMachine->State = NextState;

        switch (NextState)
        {
            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_NEXT_PACK_GROUP:
            /* ------------------------------------------------------------------------ */
            {
                EDDP_ASSERT_FALSE(NextPackGrId >= pDDB->CRT.CfgParam.ProviderPackGroupMaxNumber);
                   
                NextState = EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_ACW_OUT;
            }
            break;

            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_ACW_OUT:
            /* ------------------------------------------------------------------------ */
            {
                EDDP_CMD_PARAM_PTR_TYPE pNewCmdParam = EDDP_CRTCmdGetInternalCmdParamElement(pDDB);

                EDDP_ASSERT_FALSE(NextPackGrId >= pDDB->CRT.CfgParam.ProviderPackGroupMaxNumber);
                EDDP_ASSERT_FALSE(pProvMngm->pProvPackGroupEntry[NextPackGrId].AcwId == EDDP_CRT_CHAIN_IDX_END);

				// delete R-O-G-Bit if this is last ACW in AcwPhaseList
                EDDP_CRTProvAcwROGBitRemoveHwApi (pDDB, pProvMngm->pProvPackGroupEntry[NextPackGrId].AcwId);
                // remove this pack ACW from ACW_Treductionselect
                EDDP_CRTProvAcwRemoveFromTreductionselect (pDDB, pProvMngm->pProvPackGroupEntry[NextPackGrId].AcwId);

                // remember data for CMD-Interface
                pNewCmdParam->param_union.Crt.Ppm.ACW_InOut.AcwId = pProvMngm->pProvPackGroupEntry[NextPackGrId].AcwId;
                pNewCmdParam->param_union.Crt.Ppm.ACW_InOut.ProviderId = EDDP_CRT_CHAIN_IDX_END;
                // call CmdRequest
                // if command is finished, the Cbf EDDP_CRTCmdDoneCbfPpmPackGroup() will be called
                EDDP_CRTCmdRequestPpm (pDDB, EDDP_CRT_CMD_PPM_PACK_ACW_OUT, EDDP_CRT_CMD_ASYNCHRON, pNewCmdParam);

                Ready = LSA_TRUE;
            }
            break;

            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_FINISH:
            /* ------------------------------------------------------------------------ */
            {
                // finish the state machine
                Ready                   = LSA_TRUE;
                pStateMachine->State    = EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_IDLE;

                // go back to caller
                EDDP_ASSERT_NO_NULL_FCT_PTR(pStateMachine->Cbf);
                CbfTemp             = pStateMachine->Cbf;
                pStateMachine->Cbf  = 0;
                CbfTemp(pDDB, EDD_STS_OK);
                // Important: no action after calling of Cbf because the next RQB could be taken from queue within Cbf and executed asynchronous !!!
            }
            break;

            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_START:
            default:
            /* ------------------------------------------------------------------------ */
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderPackGroupPassivateAllStateMachine(): FATAL -> Unexpected NextState(%d)",
                    NextState);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTProviderPackGroupPassivateAllStateMachine(): Response=0x%X", Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProviderPackGroupPassivateAllStateMachineCbf    +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             EDD_RSP                         Response                +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  Response       : Response of last action                                           +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Callbackfunction of Provider-Pack-Group-Passivate state machine.      +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderPackGroupPassivateAllStateMachineCbf(
  EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
  EDD_RSP                     Response)
{
    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProviderPackGroupPassivateAllStateMachineCbf(): Response(0x%X)",
        Response);

    EDDP_CRTProviderPackGroupPassivateAllStateMachine (pDDB, Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname : EDDP_CRTFollowUpCmdAcwDGActivateCheckRqbInProgress      +*/
/*                                                                          +*/
/*+  Input/Output : EDDP_LOCAL_DDB_PTR_TYPE                       pDDB      +*/
/*+                 EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_SM_PTR_TYPE pStateMachine+*/
/*+                                                                         +*/
/*+  Result       : LSA_VOID                                                +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                      +*/
/*+  pStateMachine : reference to activation state machine                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: If there is a follow up command the current RQB has to    +*/
/*+               be queued to the end of the "in progress" queue.          +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTFollowUpCmdAcwDGActivateCheckRqbInProgress(
    EDDP_LOCAL_DDB_PTR_TYPE                       pDDB,
    EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_SM_PTR_TYPE pStateMachine)
{
    /* follow up command necessary? */ 
    if(1 < pStateMachine->CmdCnt)
    {
        EDD_UPPER_RQB_PTR_TYPE            pRQB;
        EDDP_GLB_CRT_RQB_HANDLER_PTR_TYPE pRQBHandler;
        
        pRQBHandler = &pDDB->GlbCrtRQBHandler;
        
        /* remove first RQB */
        pRQB = EDDP_GlbRQBQueueRemoveFromBegin(&pRQBHandler->RqbInProgressQueue);
        
        if (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL))
        {
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTFollowUpCmdAcwDGActivateCheckRqbInProgress(): FATAL -> no RQB in work!");
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        
        if (!LSA_HOST_PTR_ARE_EQUAL(pRQB, pStateMachine->pRQB))
        {
            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTFollowUpCmdAcwDGActivateCheckRqbInProgress(): FATAL -> RQB does not match (pRQB=0x%X, pRQBStateMachine=0x%X)!", 
                              pRQB, pStateMachine->pRQB);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTFollowUpCmdAcwDGActivateCheckRqbInProgress(): follow up command necessary (pRQB=0x%X)", pRQB);
        
        /* request is still pending, add RQB to end of "in progress" queue */
        EDDP_GlbRQBQueueAddToEnd(&pRQBHandler->RqbInProgressQueue, pRQB);
    }
}






/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProviderAcwDGActivateStateMachine               +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             EDD_RSP                         ResponseSm              +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ResponseSm     : Response of last action                                           +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: State machine of Provider-ACW_DG-Activate.                            +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderAcwDGActivateStateMachine(
  EDDP_LOCAL_DDB_PTR_TYPE                       pDDB,
  EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_SM_PTR_TYPE pStateMachine,
  EDD_RSP                                       ResponseSm)
{
    EDD_RSP                                             Response;
    EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_TYPE        ActState;
    EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_TYPE        NextState;
    LSA_UINT16                                          AcwDGIdx;
    LSA_BOOL                                            Ready;
    EDDP_CRT_PROV_DG_CBF_PTR_TYPE                       CbfTemp;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE                     pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE                      pAcw;
    EDDP_CRT_PROVIDER_ACW_DG_PTR_TYPE                   pAcwDG;
    EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_PTR_TYPE    pSubframeBlock;

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CRTProviderAcwDGActivateStateMachine(ResponseSm=0x%X)", ResponseSm);
        
    Response        = EDD_STS_OK;
    pProvMngm       = &pDDB->CRT.Prov;
    ActState        = pStateMachine->State;     // actual state of state machine
    NextState       = ActState;
    Ready           = LSA_FALSE;
    CbfTemp         = 0;

    if (ResponseSm != EDD_STS_OK)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderAcwDGActivateStateMachine(ResponseSm=0x%X): FATAL -> INCONSISTENCE!",
            ResponseSm);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    /* ------------------------------------------------------------------------ */
    /* finish handling of actual state before go to next state                  */
    /* ------------------------------------------------------------------------ */
    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProviderAcwDGActivateStateMachine(): ActState(%d)", ActState);

    switch (ActState)
    {
        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_START:
        /* ------------------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            EDDP_ASSERT_NO_NULL_FCT_PTR(pStateMachine->Cbf);
            EDDP_ASSERT_FALSE(pStateMachine->ProviderId == EDDP_CRT_CHAIN_IDX_END);
            NextState = EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_ACW_DG_ON;
        }
        break;

        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_ACW_DG_ON:
        /* ------------------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            EDDP_ASSERT_FALSE(pStateMachine->ProviderId == EDDP_CRT_CHAIN_IDX_END);
            AcwDGIdx = pProvMngm->pProvEntry[pStateMachine->ProviderId].DfpParam.AcwDGId;
            EDDP_ASSERT_FALSE(AcwDGIdx == EDDP_CRT_CHAIN_IDX_END);
            pAcwDG = &pProvMngm->pAcwDGEntry[AcwDGIdx];
            EDDP_ASSERT_FALSE(pAcwDG->AcwId == EDDP_CRT_CHAIN_IDX_END);
            pAcw = &pProvMngm->pAcwEntry[pAcwDG->AcwId];
            EDDP_ASSERT_FALSE(pAcw->DfpParam.PrmSubframeBlockIdx == EDDP_CRT_CHAIN_IDX_END);
            // active record buffer (A)
            pSubframeBlock = &pDDB->PRM.pPDIRSubframeData_A->SubframeBlockArray[pAcw->DfpParam.PrmSubframeBlockIdx];

            // which DFPmode has this pack group ?
            if (pSubframeBlock->SFIOCRProperties.DFPmode == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_DFPMODE_END_NODE)
            {
                // this is Controller/MultiDevice (ACW_in only if at least one ACW_DG is already active)
                // only if this ACW is not yet active
                if (!(pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK) && (pAcw->DfpParam.AcwDGActCnt == 1))
                {
                    NextState = EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_ACW_IN;
                }
                else
                {
                    // ACW is already active - finish the state machine
                    NextState = EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_FINISH;
                }
            }
            else
            {
                // this is Device (ACW is already active from PrmCommit)
                // finish the state machine
                NextState = EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_FINISH;
            }
        }
        break;

        /* ------------------------------------------------------------------------ */
	    case EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_ACW_IN:
        /* ------------------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            EDDP_ASSERT_FALSE(pStateMachine->ProviderId == EDDP_CRT_CHAIN_IDX_END);

            // if this pack group is redundant, the ACW_DG has 2 ACWs - activate the other ACWRed1 of this ACW_DG
            if (EDDP_IS_IRT_REDUNDANT_FRAMEID(pProvMngm->pProvEntry[pStateMachine->ProviderId].AddParam.FrameID))
            {
                AcwDGIdx = pProvMngm->pProvEntry[pStateMachine->ProviderId].DfpParam.AcwDGId;
                EDDP_ASSERT_FALSE(AcwDGIdx == EDDP_CRT_CHAIN_IDX_END);
                pAcwDG = &pProvMngm->pAcwDGEntry[AcwDGIdx];
                EDDP_ASSERT_FALSE(pAcwDG->AcwIdRed1 == EDDP_CRT_CHAIN_IDX_END);
                pAcw = &pProvMngm->pAcwEntry[pAcwDG->AcwIdRed1];

                // only if this ACWRed1 is not yet active
                if (!(pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK))
                {
                    NextState = EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_ACWRED1_IN;
                }
                else
                {
                    // ACW is already active - finish the state machine
                    NextState = EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_FINISH;
                }
            }
            else
            {
                // pack group is not redundant - finish the state machine
                NextState = EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_FINISH;
            }
        }
        break;

        /* ------------------------------------------------------------------------ */
	    case EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_ACWRED1_IN:
        /* ------------------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            EDDP_ASSERT_FALSE(pStateMachine->ProviderId == EDDP_CRT_CHAIN_IDX_END);
            NextState = EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_FINISH;
        }
        break;

        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_FINISH:
        default:
        /* ------------------------------------------------------------------------ */
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderAcwDGActivateStateMachine(): FATAL -> Unexpected ActState(%d)",
                ActState);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }

    /* ------------------------------------------------------------------------ */
    /* forward the state machine to next state                                  */
    /* ------------------------------------------------------------------------ */
    while (!Ready)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProviderAcwDGActivateStateMachine(): loop -> NextState(%d)",
            NextState);

        pStateMachine->State = NextState;

        switch (NextState)
        {
            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_ACW_DG_ON:
            /* ------------------------------------------------------------------------ */
            {
                EDDP_CMD_PARAM_PTR_TYPE pNewCmdParam = EDDP_CRTCmdGetInternalCmdParamElement(pDDB);

                EDDP_ASSERT_FALSE(pStateMachine->ProviderId == EDDP_CRT_CHAIN_IDX_END);
                AcwDGIdx = pProvMngm->pProvEntry[pStateMachine->ProviderId].DfpParam.AcwDGId;
                EDDP_ASSERT_FALSE(AcwDGIdx == EDDP_CRT_CHAIN_IDX_END);

                // remember data for CMD-Interface
                pNewCmdParam->param_union.Crt.PpmAcwDgSmIdx = pStateMachine->Index;
                pNewCmdParam->param_union.Crt.Ppm.ACW_OnOff.AcwId = AcwDGIdx;
                pNewCmdParam->param_union.Crt.Ppm.ACW_OnOff.ProviderId = pStateMachine->ProviderId;
                // call CmdRequest
                // if command is finished, the Cbf EDDP_CRTCmdDoneCbfPpmAcwDG() will be called
                EDDP_CRTCmdRequestPpm (pDDB, EDDP_CRT_CMD_PPM_PACK_ACW_DG_ON, EDDP_CRT_CMD_ASYNCHRON, pNewCmdParam);

                /* follow up command necessary? if so, queue RQB to end of "in progress" queue */
                pStateMachine->CmdCnt++;
                EDDP_CRTFollowUpCmdAcwDGActivateCheckRqbInProgress(pDDB, pStateMachine);
                
                Ready = LSA_TRUE;
            }
            break;

            /* ------------------------------------------------------------------------ */
	        case EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_ACW_IN:
            /* ------------------------------------------------------------------------ */
            {
                EDDP_CMD_PARAM_PTR_TYPE pNewCmdParam = EDDP_CRTCmdGetInternalCmdParamElement(pDDB);

                EDDP_ASSERT_FALSE(pStateMachine->ProviderId == EDDP_CRT_CHAIN_IDX_END);
                AcwDGIdx = pProvMngm->pProvEntry[pStateMachine->ProviderId].DfpParam.AcwDGId;
                EDDP_ASSERT_FALSE(AcwDGIdx == EDDP_CRT_CHAIN_IDX_END);

                pAcwDG = &pProvMngm->pAcwDGEntry[AcwDGIdx];
                EDDP_ASSERT_FALSE(pAcwDG->AcwId == EDDP_CRT_CHAIN_IDX_END);

                // remember data for CMD-Interface
                pNewCmdParam->param_union.Crt.PpmAcwDgSmIdx = pStateMachine->Index;
                pNewCmdParam->param_union.Crt.Ppm.ACW_InOut.AcwId = pAcwDG->AcwId;
                pNewCmdParam->param_union.Crt.Ppm.ACW_InOut.ProviderId = pStateMachine->ProviderId;
                // call CmdRequest
                // if command is finished, the Cbf EDDP_CRTCmdDoneCbfPpmAcwDG() will be called
                EDDP_CRTCmdRequestPpm (pDDB, EDDP_CRT_CMD_PPM_PACK_ACW_DG_ON_ACW_IN, EDDP_CRT_CMD_ASYNCHRON, pNewCmdParam);

                /* follow up command necessary? if so, queue RQB to end of "in progress" queue */
                pStateMachine->CmdCnt++;
                EDDP_CRTFollowUpCmdAcwDGActivateCheckRqbInProgress(pDDB, pStateMachine);
                
                Ready = LSA_TRUE;
            }
            break;

            /* ------------------------------------------------------------------------ */
	        case EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_ACWRED1_IN:
            /* ------------------------------------------------------------------------ */
            {
                EDDP_CMD_PARAM_PTR_TYPE pNewCmdParam = EDDP_CRTCmdGetInternalCmdParamElement(pDDB);

                EDDP_ASSERT_FALSE(pStateMachine->ProviderId == EDDP_CRT_CHAIN_IDX_END);
                AcwDGIdx = pProvMngm->pProvEntry[pStateMachine->ProviderId].DfpParam.AcwDGId;
                EDDP_ASSERT_FALSE(AcwDGIdx == EDDP_CRT_CHAIN_IDX_END);

                pAcwDG = &pProvMngm->pAcwDGEntry[AcwDGIdx];
                EDDP_ASSERT_FALSE(pAcwDG->AcwIdRed1 == EDDP_CRT_CHAIN_IDX_END);

                // remember data for CMD-Interface
                pNewCmdParam->param_union.Crt.PpmAcwDgSmIdx = pStateMachine->Index;
                pNewCmdParam->param_union.Crt.Ppm.ACW_InOut.AcwId = pAcwDG->AcwIdRed1;
                pNewCmdParam->param_union.Crt.Ppm.ACW_InOut.ProviderId = pStateMachine->ProviderId;
                // call CmdRequest
                // if command is finished, the Cbf EDDP_CRTCmdDoneCbfPpmAcwDG() will be called
                EDDP_CRTCmdRequestPpm (pDDB, EDDP_CRT_CMD_PPM_PACK_ACW_DG_ON_ACW_IN, EDDP_CRT_CMD_ASYNCHRON, pNewCmdParam);

                /* follow up command necessary? if so, queue RQB to end of "in progress" queue */
                pStateMachine->CmdCnt++;
                EDDP_CRTFollowUpCmdAcwDGActivateCheckRqbInProgress(pDDB, pStateMachine);
                
                Ready = LSA_TRUE;
            }
            break;

            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_FINISH:
            /* ------------------------------------------------------------------------ */
            {
                LSA_UINT16 ProviderId = pStateMachine->ProviderId;
                
                // finish the state machine
                Ready = LSA_TRUE;

                // go back to caller
                EDDP_ASSERT_NO_NULL_FCT_PTR(pStateMachine->Cbf);
                CbfTemp = pStateMachine->Cbf;

                /* release state machine */
                EDDP_CRTReleaseProvAcwDGActivateStateMachine(pDDB, pStateMachine->Index);
                
                CbfTemp(pDDB, ProviderId, EDD_STS_OK);
                // Important: no action after calling of Cbf because the next RQB could be taken from queue within Cbf and executed asynchronous !!!
            }
            break;

            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_START:
            default:
            /* ------------------------------------------------------------------------ */
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderAcwDGActivateStateMachine(): FATAL -> Unexpected NextState(%d)",
                    NextState);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }
    
    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTProviderAcwDGActivateStateMachine(): Response=0x%X", Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProviderAcwDGActivateStateMachineCbf            +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             EDD_RSP                         Response                +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  Response       : Response of last action                                           +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Callbackfunction of Provider-ACW_DG-Activate state machine.           +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderAcwDGActivateStateMachineCbf(
  EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
  EDDP_CMD_PARAM_PTR_TYPE     pCmdParam,
  EDD_RSP                     Response)
{
    LSA_UINT16 Index = pCmdParam->param_union.Crt.PpmAcwDgSmIdx;
    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProviderAcwDGActivateStateMachineCbf(): Response(0x%X)", Response);

    /* sanity check for Index */
    EDDP_ASSERT(Index < EDDP_CRT_PROVIDER_ACW_DG_STATE_MACHINE_DEPTH);
    
    EDDP_CRTProviderAcwDGActivateStateMachine (pDDB, &pDDB->CRT.Prov.AcwDGActivateSm[Index], Response);
}






/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname : EDDP_CRTFollowUpCmdAcwDGPassivateCheckRqbInProgress     +*/
/*                                                                          +*/
/*+  Input/Output : EDDP_LOCAL_DDB_PTR_TYPE                       pDDB      +*/
/*+                 EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_SM_PTR_TYPE pStateMachine+*/
/*+                                                                         +*/
/*+  Result       : LSA_VOID                                                +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB          : Pointer to DeviceDescriptionBlock                      +*/
/*+  pStateMachine : reference to passivatin state machine                  +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: If there is a follow up command the current RQB has to    +*/
/*+               be queued to the end of the "in progress" queue.          +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTFollowUpCmdAcwDGPassivateCheckRqbInProgress(
    EDDP_LOCAL_DDB_PTR_TYPE                       pDDB,
    EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_SM_PTR_TYPE pStateMachine)
{
    /* follow up command necessary? */ 
    if(1 < pStateMachine->CmdCnt)
    {
        EDD_UPPER_RQB_PTR_TYPE            pRQB;
        EDDP_GLB_CRT_RQB_HANDLER_PTR_TYPE pRQBHandler;
        
        pRQBHandler = &pDDB->GlbCrtRQBHandler;
        
        /* remove first RQB */
        pRQB = EDDP_GlbRQBQueueRemoveFromBegin(&pRQBHandler->RqbInProgressQueue);
        
        if (LSA_HOST_PTR_ARE_EQUAL(pRQB, LSA_NULL))
        {
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTFollowUpCmdAcwDGPassivateCheckRqbInProgress(): FATAL -> no RQB in work!");
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        
        if (!LSA_HOST_PTR_ARE_EQUAL(pRQB, pStateMachine->pRQB))
        {
            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTFollowUpCmdAcwDGPassivateCheckRqbInProgress(): FATAL -> RQB does not match (pRQB=0x%X, pRQBStateMachine=0x%X)!", 
                              pRQB, pStateMachine->pRQB);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTFollowUpCmdAcwDGPassivateCheckRqbInProgress(): follow up command necessary (pRQB=0x%X)", pRQB);
        
        /* request is still pending, add RQB to end of "in progress" queue */
        EDDP_GlbRQBQueueAddToEnd(&pRQBHandler->RqbInProgressQueue, pRQB);
    }
}






/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProviderAcwDGPassivateStateMachine              +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             EDD_RSP                         ResponseSm              +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ResponseSm     : Response of last action                                           +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: State machine of Provider-ACW_DG-Passivate.                           +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderAcwDGPassivateStateMachine(
  EDDP_LOCAL_DDB_PTR_TYPE                        pDDB,
  EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_SM_PTR_TYPE pStateMachine,
  EDD_RSP                                        ResponseSm)
{
    EDD_RSP                                             Response;
    EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_TYPE       ActState;
    EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_TYPE       NextState;
    LSA_UINT16                                          AcwDGIdx;
    LSA_BOOL                                            Ready;
    EDDP_CRT_PROV_DG_CBF_PTR_TYPE                       CbfTemp;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE                     pProvMngm;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE                      pAcw;
    EDDP_CRT_PROVIDER_ACW_DG_PTR_TYPE                   pAcwDG;
    EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_PTR_TYPE    pSubframeBlock;

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CRTProviderAcwDGPassivateStateMachine(ResponseSm=0x%X)", ResponseSm);
    
    Response        = EDD_STS_OK;
    pProvMngm       = &pDDB->CRT.Prov;
    ActState        = pStateMachine->State;     // actual state of state machine
    NextState       = ActState;
    Ready           = LSA_FALSE;
    CbfTemp         = 0;

    if (ResponseSm != EDD_STS_OK)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderAcwDGPassivateStateMachine(ResponseSm=0x%X): FATAL -> INCONSISTENCE!",
            ResponseSm);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    /* ------------------------------------------------------------------------ */
    /* finish handling of actual state before go to next state                  */
    /* ------------------------------------------------------------------------ */
    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProviderAcwDGPassivateStateMachine(): ActState(%d)", ActState);

    switch (ActState)
    {
        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_START:
        /* ------------------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            EDDP_ASSERT_NO_NULL_FCT_PTR(pStateMachine->Cbf);
            EDDP_ASSERT_FALSE(pStateMachine->ProviderId == EDDP_CRT_CHAIN_IDX_END);
            AcwDGIdx = pProvMngm->pProvEntry[pStateMachine->ProviderId].DfpParam.AcwDGId;
            EDDP_ASSERT_FALSE(AcwDGIdx == EDDP_CRT_CHAIN_IDX_END);
            pAcwDG  = &pProvMngm->pAcwDGEntry[AcwDGIdx];
            EDDP_ASSERT_FALSE(pAcwDG->AcwId == EDDP_CRT_CHAIN_IDX_END);
            pAcw    = &pProvMngm->pAcwEntry[pAcwDG->AcwId];
            EDDP_ASSERT_FALSE(pAcw->DfpParam.PrmSubframeBlockIdx == EDDP_CRT_CHAIN_IDX_END);
            // active record buffer (A)
            pSubframeBlock = &pDDB->PRM.pPDIRSubframeData_A->SubframeBlockArray[pAcw->DfpParam.PrmSubframeBlockIdx];

            // which DFPmode has this pack group ?
            if (pSubframeBlock->SFIOCRProperties.DFPmode == EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_DFPMODE_END_NODE)
            {
                // this is Controller/MultiDevice - ACW_out only if last active ACW_DG of this ACW should be passivated
                // ACW_out only if this ACW is currently active
                if ((pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK) && (pAcw->DfpParam.AcwDGActCnt == 1))
                {
                    // note: ACW_DG will be passivated afterwards
                    NextState = EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_ACW_OUT;
                }
                else
                {
                    // only passivate ACW_DG
                    EDDP_CRTProvAcwDGSetPassive(pDDB, AcwDGIdx);
                    NextState = EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_ACW_DG_OFF;
                }
            }
            else
            {
                // this is Device - only passivate ACW_DG
                // passivate ACW_DG
                EDDP_CRTProvAcwDGSetPassive(pDDB, AcwDGIdx);
                NextState = EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_ACW_DG_OFF;
            }
        }
        break;

        /* ------------------------------------------------------------------------ */
	    case EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_ACW_OUT:
        /* ------------------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            EDDP_ASSERT_FALSE(pStateMachine->ProviderId == EDDP_CRT_CHAIN_IDX_END);
            // if this pack group is redundant, the ACW_DG has 2 ACWs - passivate the other ACWRed1 of this ACW_DG 
            AcwDGIdx = pProvMngm->pProvEntry[pStateMachine->ProviderId].DfpParam.AcwDGId;
            EDDP_ASSERT_FALSE(AcwDGIdx == EDDP_CRT_CHAIN_IDX_END);
            pAcwDG = &pProvMngm->pAcwDGEntry[AcwDGIdx];
            
            // if this pack group is redundant, the ACW_DG has 2 ACWs - chain out the other ACWRed1 of this ACW_DG 
            if (EDDP_IS_IRT_REDUNDANT_FRAMEID(pProvMngm->pProvEntry[pStateMachine->ProviderId].AddParam.FrameID))
            {
                EDDP_ASSERT_FALSE(pAcwDG->AcwIdRed1 == EDDP_CRT_CHAIN_IDX_END);
                pAcw = &pProvMngm->pAcwEntry[pAcwDG->AcwIdRed1];

                // ACW_out only if this ACWRed1 is currently active
                if (pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK)
                {
                    NextState = EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_ACWRED1_OUT;
                }
                else
                {
                    // ACWRed1 is already passive - passivate last ACW_DG (only if it is active)
                    if (pAcwDG->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK)
                    {
                        // note: the deactivation of ACW_DG in HW is normally done via CMD-Interface (EDDP_CRT_CMD_PPM_PACK_ACW_DG_OFF).
                        //       this is not possible here because the corresponding pack ACW has already been chained out.
                        //       therefore the Active-Bit is deleted manually by call of the following function.
                        EDDP_CRTProvAcwDGDeleteActiveBitHwApi (pDDB, AcwDGIdx);
                        // passivate ACW_DG
                        EDDP_CRTProvAcwDGSetPassive (pDDB, AcwDGIdx);
                    }
                    
                    // finish the state machine
                    NextState = EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_FINISH;
                }
            }
            else
            {
                // pack group is not redundant - passivate last ACW_DG (only if it is active)
                if (pAcwDG->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK)
                {
                    // note: the deactivation of ACW_DG in HW is normally done via CMD-Interface (EDDP_CRT_CMD_PPM_PACK_ACW_DG_OFF).
                    //       this is not possible here because the corresponding pack ACW has already been chained out.
                    //       therefore the Active-Bit is deleted manually by call of the following function.
                    EDDP_CRTProvAcwDGDeleteActiveBitHwApi (pDDB, AcwDGIdx);
                    // passivate ACW_DG
                    EDDP_CRTProvAcwDGSetPassive (pDDB, AcwDGIdx);
                }
                
                // finish the state machine
                NextState = EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_FINISH;
            }
        }
        break;

        /* ------------------------------------------------------------------------ */
	    case EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_ACWRED1_OUT:
        /* ------------------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            EDDP_ASSERT_FALSE(pStateMachine->ProviderId == EDDP_CRT_CHAIN_IDX_END);
            AcwDGIdx = pProvMngm->pProvEntry[pStateMachine->ProviderId].DfpParam.AcwDGId;
            EDDP_ASSERT_FALSE(AcwDGIdx == EDDP_CRT_CHAIN_IDX_END);
            pAcwDG = &pProvMngm->pAcwDGEntry[AcwDGIdx];
            
            // passivate last ACW_DG (only if it is active)
            if (pAcwDG->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK)
            {
                // note: the deactivation of ACW_DG in HW is normally done via CMD-Interface (EDDP_CRT_CMD_PPM_PACK_ACW_DG_OFF).
                //       this is not possible here because the corresponding pack ACW has already been chained out.
                //       therefore the Active-Bit is deleted manually by call of the following function.
                EDDP_CRTProvAcwDGDeleteActiveBitHwApi (pDDB, AcwDGIdx);
                // passivate ACW_DG
                EDDP_CRTProvAcwDGSetPassive (pDDB, AcwDGIdx);
            }

            // finish the state machine
            NextState = EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_FINISH;
        }
        break;

        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_ACW_DG_OFF:
        /* ------------------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            EDDP_ASSERT_FALSE(pStateMachine->ProviderId == EDDP_CRT_CHAIN_IDX_END);
            NextState = EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_FINISH;
        }
        break;

        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_FINISH:
        default:
        /* ------------------------------------------------------------------------ */
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderAcwDGPassivateStateMachine(): FATAL -> Unexpected ActState(%d)",
                ActState);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }

    /* ------------------------------------------------------------------------ */
    /* forward the state machine to next state                                  */
    /* ------------------------------------------------------------------------ */
    while (!Ready)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProviderAcwDGPassivateStateMachine(): loop -> NextState(%d)",
            NextState);

        pStateMachine->State = NextState;

        switch (NextState)
        {
            /* ------------------------------------------------------------------------ */
	        case EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_ACW_OUT:
            /* ------------------------------------------------------------------------ */
            {
                EDDP_CMD_PARAM_PTR_TYPE pNewCmdParam = EDDP_CRTCmdGetInternalCmdParamElement(pDDB);

                EDDP_ASSERT_FALSE(pStateMachine->ProviderId == EDDP_CRT_CHAIN_IDX_END);
                AcwDGIdx = pProvMngm->pProvEntry[pStateMachine->ProviderId].DfpParam.AcwDGId;
                EDDP_ASSERT_FALSE(AcwDGIdx == EDDP_CRT_CHAIN_IDX_END);
                pAcwDG = &pProvMngm->pAcwDGEntry[AcwDGIdx];
                EDDP_ASSERT_FALSE(pAcwDG->AcwId == EDDP_CRT_CHAIN_IDX_END);

                // delete R-O-G-Bit if this is last ACW in AcwPhaseList
                EDDP_CRTProvAcwROGBitRemoveHwApi(pDDB, pAcwDG->AcwId);
                // remove this pack ACW from ACW_Treductionselect
                EDDP_CRTProvAcwRemoveFromTreductionselect(pDDB, pAcwDG->AcwId);
                
                // remember data for CMD-Interface
                pNewCmdParam->param_union.Crt.PpmAcwDgSmIdx = pStateMachine->Index;
                pNewCmdParam->param_union.Crt.Ppm.ACW_InOut.AcwId = pAcwDG->AcwId;
                pNewCmdParam->param_union.Crt.Ppm.ACW_InOut.ProviderId = EDDP_CRT_CHAIN_IDX_END;
                // call CmdRequest
                // if command is finished, the Cbf EDDP_CRTCmdDoneCbfPpmAcwDG() will be called
                EDDP_CRTCmdRequestPpm (pDDB, EDDP_CRT_CMD_PPM_PACK_ACW_DG_OFF_ACW_OUT, EDDP_CRT_CMD_ASYNCHRON, pNewCmdParam);

                /* follow up command necessary? if so, queue RQB to end of "in progress" queue */
                pStateMachine->CmdCnt++;
                EDDP_CRTFollowUpCmdAcwDGPassivateCheckRqbInProgress(pDDB, pStateMachine);
                
                Ready = LSA_TRUE;
            }
            break;

            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_ACWRED1_OUT:
            /* ------------------------------------------------------------------------ */
            {
                EDDP_CMD_PARAM_PTR_TYPE pNewCmdParam = EDDP_CRTCmdGetInternalCmdParamElement(pDDB);

                EDDP_ASSERT_FALSE(pStateMachine->ProviderId == EDDP_CRT_CHAIN_IDX_END);
                AcwDGIdx = pProvMngm->pProvEntry[pStateMachine->ProviderId].DfpParam.AcwDGId;
                EDDP_ASSERT_FALSE(AcwDGIdx == EDDP_CRT_CHAIN_IDX_END);
                pAcwDG = &pProvMngm->pAcwDGEntry[AcwDGIdx];
                EDDP_ASSERT_FALSE(pAcwDG->AcwIdRed1 == EDDP_CRT_CHAIN_IDX_END);

                // delete R-O-G-Bit if this is last ACW in AcwPhaseList
                EDDP_CRTProvAcwROGBitRemoveHwApi(pDDB, pAcwDG->AcwIdRed1);
                // remove this pack ACW from ACW_Treductionselect
                EDDP_CRTProvAcwRemoveFromTreductionselect(pDDB, pAcwDG->AcwIdRed1);

                // remember data for CMD-Interface
                pNewCmdParam->param_union.Crt.PpmAcwDgSmIdx = pStateMachine->Index;
                pNewCmdParam->param_union.Crt.Ppm.ACW_InOut.AcwId = pAcwDG->AcwIdRed1;
                pNewCmdParam->param_union.Crt.Ppm.ACW_InOut.ProviderId = EDDP_CRT_CHAIN_IDX_END;
                // call CmdRequest
                // if command is finished, the Cbf EDDP_CRTCmdDoneCbfPpmAcwDG() will be called
                EDDP_CRTCmdRequestPpm (pDDB, EDDP_CRT_CMD_PPM_PACK_ACW_DG_OFF_ACW_OUT, EDDP_CRT_CMD_ASYNCHRON, pNewCmdParam);

                /* follow up command necessary? if so, queue RQB to end of "in progress" queue */
                pStateMachine->CmdCnt++;
                EDDP_CRTFollowUpCmdAcwDGPassivateCheckRqbInProgress(pDDB, pStateMachine);
                
                Ready = LSA_TRUE;
            }
            break;

            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_ACW_DG_OFF:
            /* ------------------------------------------------------------------------ */
            {
                EDDP_CMD_PARAM_PTR_TYPE pNewCmdParam = EDDP_CRTCmdGetInternalCmdParamElement(pDDB);

                EDDP_ASSERT_FALSE(pStateMachine->ProviderId == EDDP_CRT_CHAIN_IDX_END);
                AcwDGIdx = pProvMngm->pProvEntry[pStateMachine->ProviderId].DfpParam.AcwDGId;
                EDDP_ASSERT_FALSE(AcwDGIdx == EDDP_CRT_CHAIN_IDX_END);

                // remember data for CMD-Interface
                pNewCmdParam->param_union.Crt.PpmAcwDgSmIdx = pStateMachine->Index;
                pNewCmdParam->param_union.Crt.Ppm.ACW_OnOff.AcwId = AcwDGIdx;
                pNewCmdParam->param_union.Crt.Ppm.ACW_OnOff.ProviderId = pStateMachine->ProviderId;
                // call CmdRequest
                // if command is finished, the Cbf EDDP_CRTCmdDoneCbfPpmAcwDG() will be called
                EDDP_CRTCmdRequestPpm (pDDB, EDDP_CRT_CMD_PPM_PACK_ACW_DG_OFF, EDDP_CRT_CMD_ASYNCHRON, pNewCmdParam);

                /* follow up command necessary? if so, queue RQB to end of "in progress" queue */
                pStateMachine->CmdCnt++;
                EDDP_CRTFollowUpCmdAcwDGPassivateCheckRqbInProgress(pDDB, pStateMachine);
                
                Ready = LSA_TRUE;
            }
            break;

            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_FINISH:
            /* ------------------------------------------------------------------------ */
            {
                LSA_UINT16 ProviderId = pStateMachine->ProviderId;
                
                // finish the state machine
                Ready = LSA_TRUE;

                // go back to caller
                EDDP_ASSERT_NO_NULL_FCT_PTR(pStateMachine->Cbf);
                CbfTemp = pStateMachine->Cbf;

                /* release state machine */
                EDDP_CRTReleaseProvAcwDGPassivateStateMachine(pDDB, pStateMachine->Index);
                
                CbfTemp(pDDB, ProviderId, EDD_STS_OK);
                // Important: no action after calling of Cbf because the next RQB could be taken from queue within Cbf and executed asynchronous !!!
            }
            break;

            /* ------------------------------------------------------------------------ */
            case EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_START:
            default:
            /* ------------------------------------------------------------------------ */
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTProviderAcwDGPassivateStateMachine(): FATAL -> Unexpected NextState(%d)",
                    NextState);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTProviderAcwDGPassivateStateMachine(): Response=0x%X", Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTProviderAcwDGPassivateStateMachineCbf           +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             EDD_RSP                         Response                +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  Response       : Response of last action                                           +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Callbackfunction of Provider-ACW_DG-Passivate state machine.          +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTProviderAcwDGPassivateStateMachineCbf(
  EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
  EDDP_CMD_PARAM_PTR_TYPE     pCmdParam,
  EDD_RSP                     Response)
{
    LSA_UINT16 Index = pCmdParam->param_union.Crt.PpmAcwDgSmIdx;
    
    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTProviderAcwDGPassivateStateMachineCbf(): Response(0x%X)", Response);

    /* sanity check for Index */
    EDDP_ASSERT(Index < EDDP_CRT_PROVIDER_ACW_DG_STATE_MACHINE_DEPTH);
    
    EDDP_CRTProviderAcwDGPassivateStateMachine (pDDB, &pDDB->CRT.Prov.AcwDGPassivateSm[Index], Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTApplTimerIfSetupStateMachine                    +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             EDD_RSP                         ResponseSm              +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ResponseSm     : Response of last action                                           +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: State machine of ApplTimerIfSetup.                                    +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTApplTimerIfSetupStateMachine(
  EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
  EDD_RSP                     ResponseSm)
{
    EDD_RSP                                Response;
    EDDP_CRT_APPL_TIMER_STATE_TYPE         ActState;
    EDDP_CRT_APPL_TIMER_STATE_TYPE         NextState;
    LSA_BOOL                               Ready;
    EDDP_CRT_CBF_PTR_TYPE                  CbfTemp;
    EDDP_CRT_APPL_TIMER_SM_PTR_TYPE        pStateMachine;

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CRTApplTimerIfSetupStateMachine(ResponseSm=0x%X)", ResponseSm);

    Response        = EDD_STS_OK;
    pStateMachine   = &pDDB->CRT.ApplTimerIfSetupSm;
    ActState        = pStateMachine->State;     // actual state of state machine
    NextState       = ActState;
    Ready           = LSA_FALSE;
    CbfTemp         = 0;

    if (ResponseSm != EDD_STS_OK)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTApplTimerIfSetupStateMachine(ResponseSm=0x%X): FATAL -> INCONSISTENCE!",
            ResponseSm);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    /* ------------------------------------------------------------------------ */
    /* finish handling of actual state before go to next state                  */
    /* ------------------------------------------------------------------------ */
    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTApplTimerIfSetupStateMachine(): ActState(%d)", ActState);

    switch (ActState)
    {
        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_APPL_TIMER_STATE_START:
        /* ------------------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            EDDP_ASSERT_NO_NULL_FCT_PTR(pStateMachine->Cbf);

            // signal start of SendClockChange to application (without actually changing the SendClock on startup)
            EDDP_SIGNAL_SENDCLOCK_CHANGE (pDDB->hSysDev, pDDB->SWI.CycleInfo.CycleBaseFactor, EDDP_SENDCLOCK_CHANGE_MODE_STARTING);

            NextState = EDDP_CRT_APPL_TIMER_STATE_SETUP_TRANSFER_END_ON;
        }
        break;

        /* ------------------------------------------------------------ */
        case EDDP_CRT_APPL_TIMER_STATE_SETUP_TRANSFER_END_ON:
        /* ------------------------------------------------------------ */
        {
            // this state is synchron
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTApplTimerIfSetupStateMachine(): FATAL -> The State(%d) is synchron!",
                ActState);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;

        /* ------------------------------------------------------------ */
        case EDDP_CRT_APPL_TIMER_STATE_UPDATE_APPL_LENGTH_ON:
        /* ------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            NextState = EDDP_CRT_APPL_TIMER_STATE_CONNECT_SYNC;
        }
        break;

        /* ------------------------------------------------------------ */
        case EDDP_CRT_APPL_TIMER_STATE_CONNECT_SYNC:
        /* ------------------------------------------------------------ */
        {
            pStateMachine->Response = EDD_STS_OK;
            NextState = EDDP_CRT_APPL_TIMER_STATE_FINISH;
        }
        break;

        /* ------------------------------------------------------------------------ */
        case EDDP_CRT_APPL_TIMER_STATE_FINISH:
        default:
        /* ------------------------------------------------------------------------ */
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTApplTimerIfSetupStateMachine(): FATAL -> Unexpected ActState(%d)",
                ActState);
            EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
        }
        break;
    }

    /* ------------------------------------------------------------------------ */
    /* forward the state machine to next state                                  */
    /* ------------------------------------------------------------------------ */
    while (!Ready)
    {
        EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTApplTimerIfSetupStateMachine(): loop -> NextState(%d)",
            NextState);

        pStateMachine->State = NextState;

        switch (NextState)
        {
            /* ------------------------------------------------------------ */
            case EDDP_CRT_APPL_TIMER_STATE_SETUP_TRANSFER_END_ON:
            /* ------------------------------------------------------------ */
            {
                EDDP_PRMSetupTransferEnd (pDDB, EDDP_PRM_TRANSFER_END_CASE_NO_PARAMETRIZATION);

                NextState = EDDP_CRT_APPL_TIMER_STATE_UPDATE_APPL_LENGTH_ON;
            }
            break;

            /* ------------------------------------------------------------ */
            case EDDP_CRT_APPL_TIMER_STATE_UPDATE_APPL_LENGTH_ON:
            /* ------------------------------------------------------------ */
            {
                // call CmdRequest for "Update_APPLCYCLE"
                pDDB->CRT.CmdParam.Update_ApplCycle.param_union.Gsync.UpdateAppCycl.isApplTimerOn   = LSA_TRUE;
                pDDB->CRT.CmdParam.Update_ApplCycle.param_union.Gsync.UpdateAppCycl.SmCbf           = EDDP_CRTApplTimerIfSetupStateMachineCbf;
                Response = EDDP_CRTCmdRequestApplTimer (pDDB, EDDP_CRT_CMD_APPL_TIMER_UPDATE_APPLCYCLE, EDDP_CRT_CMD_ASYNCHRON);

                switch (Response)
                {
                    case EDD_STS_OK_PENDING:
                        Ready = LSA_TRUE;
                    break;

                    case EDD_STS_OK:
                        NextState = EDDP_CRT_APPL_TIMER_STATE_CONNECT_SYNC;
                    break;

                    default:
                    {
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTApplTimerIfSetupStateMachine(): FATAL -> Unexpected Response(0x%X) in state UPDATE_APPL_LENGTH_ON",
                            Response);
                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                    }
                    break;
                }
            }
            break;

            /* ------------------------------------------------------------ */
            case EDDP_CRT_APPL_TIMER_STATE_CONNECT_SYNC:
            /* ------------------------------------------------------------ */
            {
                // call CmdRequest for "Connect_SYNC_A/B"
                pDDB->CRT.CmdParam.Connect_Sync_A_B.param_union.Gsync.ConnectSyncA.SmCbf = EDDP_CRTApplTimerIfSetupStateMachineCbf;

                Response = EDDP_CRTCmdRequestApplTimer (pDDB, EDDP_CRT_CMD_APPL_TIMER_CONNECT_SYNC, EDDP_CRT_CMD_ASYNCHRON);

                switch (Response)
                {
                    case EDD_STS_OK_PENDING:
                        Ready = LSA_TRUE;
                    break;

                    case EDD_STS_OK:
                        NextState = EDDP_CRT_APPL_TIMER_STATE_FINISH;
                    break;

                    default:
                    {
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTApplTimerIfSetupStateMachine(): FATAL -> Unexpected Response(0x%X) in state CONNECT_SYNC",
                            Response);
                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                    }
                    break;
                }
            }
            break;

            /* ------------------------------------------------------------ */
            case EDDP_CRT_APPL_TIMER_STATE_FINISH:
            /* ------------------------------------------------------------ */
            {
                // signal finish of SendClockChange to application (without actually changing the SendClock on startup)
                EDDP_SIGNAL_SENDCLOCK_CHANGE (pDDB->hSysDev, pDDB->SWI.CycleInfo.CycleBaseFactor, EDDP_SENDCLOCK_CHANGE_MODE_FINISHED);

                // finish the state machine
                Ready                   = LSA_TRUE;
                pStateMachine->State    = EDDP_CRT_APPL_TIMER_STATE_IDLE;

                // go back to caller
                EDDP_ASSERT_NO_NULL_FCT_PTR(pStateMachine->Cbf);
                CbfTemp             = pStateMachine->Cbf;
                pStateMachine->Cbf  = 0;
                CbfTemp(pDDB, EDD_STS_OK);
                // Important: no action after calling of Cbf because the next RQB could be taken from queue within Cbf and executed asynchronous !!!
            }
            break;

            /* ------------------------------------------------------------ */
            case EDDP_CRT_APPL_TIMER_STATE_START:
            default:
            /* ------------------------------------------------------------ */
            {
                EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTApplTimerIfSetupStateMachine(): FATAL -> Unexpected NextState(%d)",
                    NextState);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CRTApplTimerIfSetupStateMachine(): Response=0x%X", Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTApplTimerIfSetupStateMachineCbf                 +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             EDD_RSP                         Response                +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_VOID                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  Response       : Response of last action                                           +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Callbackfunction of ApplTimerIfSetup state machine.                   +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID EDDP_LOCAL_FCT_ATTR EDDP_CRTApplTimerIfSetupStateMachineCbf(
  EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
  EDD_RSP                     Response)
{
    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTApplTimerIfSetupStateMachineCbf(): Response(0x%X)", Response);

    EDDP_CRTApplTimerIfSetupStateMachine (pDDB, Response);
}
