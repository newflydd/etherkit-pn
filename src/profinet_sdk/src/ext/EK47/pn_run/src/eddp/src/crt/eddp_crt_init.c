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
/*  F i l e               &F: eddp_crt_init.c                           :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  CRT (Cycle Real Time) for EDDP  -> initialization                        */
/*      - start up                                                           */
/*      - shut down                                                          */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who         What                                             */
#ifdef EDDP_MESSAGE
/*  07.11.08    Sergeev     Erstellung                                       */
/*  24.11.10    JS          changes to prevent compiler warnings             */
#endif
/*****************************************************************************/


/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/

#define LTRC_ACT_MODUL_ID   25
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_CRT_INIT */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"           /* eddp headerfiles */
#include "eddp_int.h"           /* internal header  */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)

#ifdef EDDP_CFG_LOCAL_MEM_STATIC
    EDDP_LOCAL_MEM_ATTR     EDDP_HDB_CRT_TYPE       g_EDDP_HDB_CRT[EDDP_CFG_MAX_CHANNELS];
#endif





/*---------------------------------------------------------------------------*/
/* FrameID area configuration. Used on ACW index lookup for a specific       */
/* FrameID.                                                                  */
/* Note: Currently default configuration is used!                            */
/*---------------------------------------------------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname       :   _EDDP_CRTAssignApiCtrlBaseAddress                               +*/
/*+                                                                                         +*/
/*+  Input/Output       :   EDDP_LOCAL_DDB_PTR_TYPE         pDDB                            +*/
/*+                     :   LSA_UINT32                      RAMSubType                      +*/
/*+                     :   LSA_UINT32                      ExpectedSize                    +*/
/*+                                                                                         +*/
/*+  Result             :   EDD_COMMON_MEM_PTR_TYPE                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  pDDB         : Pointer to Device Description Block                                     +*/
/*+  RAMSubType   : selects the specific ApiCtrl SubRam                                     +*/
/*+  ExpectedSize : expected SubBlockSize for comparing                                     +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Internal utility function for EDDP_CRTInitConsumerDevOpen()               +*/
/*+                 -> read specified address from HAL module                               +*/
/*+                 -> test returned address                                                +*/
/*+                 -> test given size with returned HAL size                               +*/
/*+                                                                                         +*/
/*+               Note: goes into FATAL on error or returns the requested base address.     +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static EDD_COMMON_MEM_PTR_TYPE EDDP_LOCAL_FCT_ATTR _EDDP_CRTAssignApiCtrlBaseAddress(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB, 
    LSA_UINT32                  RAMSubType, 
    LSA_UINT32                  ExpectedSize)
{
    EDD_COMMON_MEM_PTR_TYPE     pReturnPtr;
    LSA_UINT32                  APICtrlRAM_Size;

    pReturnPtr = EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, RAMSubType, &APICtrlRAM_Size);

    if (LSA_HOST_PTR_ARE_EQUAL(pReturnPtr, LSA_NULL))
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] _EDDP_CRTAssignApiCtrlBaseAddress(): FATAL -> LSA_NULL = EDDP_HALGetRAMSubAddress(RAMTYPE_API_CTRL, RAMSubType=%d)", 
            RAMSubType);
        EDDP_FATAL(EDDP_FATAL_ERR_GET_APICTRLRAM);
    }
    else
    {
        // check "APICtrlRAM_Size" with given ExpectedSize
        if (APICtrlRAM_Size < ExpectedSize)
        {
            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] _EDDP_CRTAssignApiCtrlBaseAddress(): FATAL -> APICtrlRAM_Size(0x%X) != ExpectedSize(0x%X)", 
                APICtrlRAM_Size, ExpectedSize);
            EDDP_FATAL(EDDP_FATAL_ERR_GET_APICTRLRAM);
        }
    }

    return (pReturnPtr);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname       :   EDDP_CRTInitProviderDevOpen                                     +*/
/*+                                                                                         +*/
/*+  Input/Output       :   EDDP_LOCAL_DDB_PTR_TYPE         pDDB                            +*/
/*+                     :   EDDP_UPPER_DPB_PTR_TYPE         pDPB                            +*/
/*+                                                                                         +*/
/*+  Result             :   EDD_RSP                                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  pDDB       : Pointer to Device Description Block                                       +*/
/*+  pDPB       : Pointer to Device Parameter Block                                         +*/
/*+                                                                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: This function creates a provider list at time of DEV_OPEN                 +*/
/*+                 -> init struct EDDP_CRT_PROVIDER_DDB_MNGM_TYPE                          +*/
/*+                 -> init struct EDDP_CRT_PROVIDER_MNGM_TYPE                              +*/
/*+                 -> EDDP-CORE has inited the DDB already with 0                          +*/
/*+                 -> write only values != 0                                               +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTInitProviderDevOpen(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_UPPER_DPB_PTR_TYPE     pDPB)
{
    EDD_RSP                                 Response;
//  LSA_UINT32                              i;
    LSA_UINT32                              j;
    LSA_UINT16                              MaxProv, MaxProvAcw, MaxProvAcwDG, MaxProvTrog, MaxProvDataStatus, MaxProvGroup, MaxProvPackGroup;
    LSA_UINT32                              RamSubApiCtrl_TBase;
    LSA_UINT32                              RamSubApiCtrl_Trog;
    LSA_UINT32                              RamSubApiCtrl_DS;
    LSA_UINT32                              APICtrlRAM_Size;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
//  EDDP_CRT_PROVIDER_DDB_MNGM_PTR_TYPE     pProvMngm;
    LSA_UINT16                              HwType;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pDPB);
#endif

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTInitProviderDevOpen(pDPB=0x%X)", pDPB);

    Response            = EDD_STS_OK;
    pProvMngm          = &pDDB->CRT.Prov;
    MaxProv             = pDDB->CRT.CfgParam.ProviderMaxNumber;
    MaxProvAcw          = pDDB->CRT.CfgParam.ProviderAcwMaxNumber;
    MaxProvAcwDG        = pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber;
    MaxProvPackGroup    = pDDB->CRT.CfgParam.ProviderPackGroupMaxNumber;
    RamSubApiCtrl_TBase = EDDP_CRT_INVALID_VALUE_32;    // init to prevent compiler warning
    RamSubApiCtrl_Trog  = EDDP_CRT_INVALID_VALUE_32;
    RamSubApiCtrl_DS    = EDDP_CRT_INVALID_VALUE_32;

    HwType = EDDP_HALGetChipVersion(pDDB);
    
    if (MaxProv > 0)
    {
        /* -------------------------------------------------------------------------------- */
        /* DDB specific data                                                                */
        /* -------------------------------------------------------------------------------- */
        /* -------------------------------------------------------------------------------- */
        /* init CRT.Prov.pProvEntry[x]                                                      */
        /* init CRT.Prov.pAcwEntry[x]                                                       */
        /* init CRT.Prov.pAcwDGEntry[x]                                                     */
        /* init CRT.Prov.pProvPackGroupEntry[x]                                             */
        /* -------------------------------------------------------------------------------- */
        pProvMngm->ProviderFreeList.EndIdx   = MaxProv - 1;
        pProvMngm->AcwFreeList.EndIdx        = MaxProvAcw - 1;
        pProvMngm->AcwDGFreeList.EndIdx      = MaxProvAcwDG - 1;
        pProvMngm->PackGroupFreeList.EndIdx  = MaxProvPackGroup - 1;

        /* -------------------------------------------------------------------------------- */
        /* init CRT.Prov.pProvEntry[x]                                                      */
        /* -------------------------------------------------------------------------------- */
        EDDP_MEMSET_COMMON ( pProvMngm->pProvEntry,  0, (MaxProv * sizeof(EDDP_CRT_PROVIDER_TYPE)) );

        for ( j=0; j < MaxProv; j++ )
        {
            if (j == 0)
                pProvMngm->pProvEntry[j].UsedFreeLink.Prev   = EDDP_CRT_CHAIN_IDX_END;
            else
                pProvMngm->pProvEntry[j].UsedFreeLink.Prev   = (LSA_UINT16) j - 1;
            pProvMngm->pProvEntry[j].UsedFreeLink.Next       = (LSA_UINT16) j + 1;
            pProvMngm->pProvEntry[j].GroupLink.Prev          = EDDP_CRT_CHAIN_IDX_END;   // not yet used
            pProvMngm->pProvEntry[j].GroupLink.Next          = EDDP_CRT_CHAIN_IDX_END;   // not yet used
            pProvMngm->pProvEntry[j].AcwList.BeginIdx        = EDDP_CRT_CHAIN_IDX_END;   // not yet used
            pProvMngm->pProvEntry[j].AcwList.EndIdx          = EDDP_CRT_CHAIN_IDX_END;   // not yet used
            pProvMngm->pProvEntry[j].ProviderId              = (LSA_UINT16) j;
            pProvMngm->pProvEntry[j].State                   = EDDP_CRT_STATE_FREE;
        }
        // last chain element
        pProvMngm->pProvEntry[MaxProv-1].UsedFreeLink.Next   = EDDP_CRT_CHAIN_IDX_END;

        /* -------------------------------------------------------------------------------- */
        /* init CRT.Prov.pAcwEntry[x]                                                       */
        /* -------------------------------------------------------------------------------- */
        EDDP_MEMSET_COMMON ( pProvMngm->pAcwEntry,   0, (MaxProvAcw * sizeof(EDDP_CRT_PROVIDER_ACW_TYPE)) );

        for ( j=0; j < MaxProvAcw; j++ )
        {
            if (j == 0)
                pProvMngm->pAcwEntry[j].UsedFreeLink.Prev    = EDDP_CRT_CHAIN_IDX_END;
            else
                pProvMngm->pAcwEntry[j].UsedFreeLink.Prev    = (LSA_UINT16) j - 1;
            pProvMngm->pAcwEntry[j].UsedFreeLink.Next        = (LSA_UINT16) j + 1;
            pProvMngm->pAcwEntry[j].AcwProvLink.Prev         = EDDP_CRT_CHAIN_IDX_END;   // not yet used
            pProvMngm->pAcwEntry[j].AcwProvLink.Next         = EDDP_CRT_CHAIN_IDX_END;   // not yet used
            pProvMngm->pAcwEntry[j].AcwPhaseLink.Prev        = EDDP_CRT_CHAIN_IDX_END;   // not yet used
            pProvMngm->pAcwEntry[j].AcwPhaseLink.Next        = EDDP_CRT_CHAIN_IDX_END;   // not yet used
            pProvMngm->pAcwEntry[j].AcwDGList.BeginIdx       = EDDP_CRT_CHAIN_IDX_END;   // not yet used
            pProvMngm->pAcwEntry[j].AcwDGList.EndIdx         = EDDP_CRT_CHAIN_IDX_END;   // not yet used
            pProvMngm->pAcwEntry[j].AcwId                    = (LSA_UINT16) j;
            /* DfpParam */
            pProvMngm->pAcwEntry[j].DfpParam.PrmSubframeBlockIdx = EDDP_CRT_CHAIN_IDX_END;
            pProvMngm->pAcwEntry[j].DfpParam.PackGrId            = EDDP_CRT_CHAIN_IDX_END;
        }
        // last chain element
        pProvMngm->pAcwEntry[MaxProvAcw-1].UsedFreeLink.Next = EDDP_CRT_CHAIN_IDX_END;

        /* -------------------------------------------------------------------------------- */
        /* init CRT.Prov.pAcwDGEntry[x]                                                     */
        /* -------------------------------------------------------------------------------- */
        EDDP_MEMSET_COMMON ( pProvMngm->pAcwDGEntry, 0, (MaxProvAcwDG * sizeof(EDDP_CRT_PROVIDER_ACW_DG_TYPE)) );

        for ( j=0; j < MaxProvAcwDG; j++ )
        {
            if (j == 0)
                pProvMngm->pAcwDGEntry[j].UsedFreeLink.Prev  = EDDP_CRT_CHAIN_IDX_END;
            else
                pProvMngm->pAcwDGEntry[j].UsedFreeLink.Prev  = (LSA_UINT16) j - 1;
            pProvMngm->pAcwDGEntry[j].UsedFreeLink.Next      = (LSA_UINT16) j + 1;
            pProvMngm->pAcwDGEntry[j].AcwDGAcwLink.Prev      = EDDP_CRT_CHAIN_IDX_END;   // not yet used
            pProvMngm->pAcwDGEntry[j].AcwDGAcwLink.Next      = EDDP_CRT_CHAIN_IDX_END;   // not yet used
            pProvMngm->pAcwDGEntry[j].AcwDGAcwLinkRed1.Prev  = EDDP_CRT_CHAIN_IDX_END;   // not yet used
            pProvMngm->pAcwDGEntry[j].AcwDGAcwLinkRed1.Next  = EDDP_CRT_CHAIN_IDX_END;   // not yet used
            pProvMngm->pAcwDGEntry[j].AcwDGId                = (LSA_UINT16) j;
            pProvMngm->pAcwDGEntry[j].AcwId                  = EDDP_CRT_CHAIN_IDX_END;   // not yet used
            pProvMngm->pAcwDGEntry[j].AcwIdRed1              = EDDP_CRT_CHAIN_IDX_END;   // not yet used
            /* ------------------------------------------------------------------------------------ */
        }
        // last chain element
        pProvMngm->pAcwDGEntry[MaxProvAcwDG-1].UsedFreeLink.Next = EDDP_CRT_CHAIN_IDX_END;

        /* -------------------------------------------------------------------------------- */
        /* init CRT.Prov.pProvPackGroupEntry[x]                                             */
        /* -------------------------------------------------------------------------------- */
        EDDP_MEMSET_COMMON ( pProvMngm->pProvPackGroupEntry, 0, (MaxProvPackGroup * sizeof(EDDP_CRT_PROVIDER_PACK_GROUP_TYPE)) );

        for ( j=0; j < MaxProvPackGroup; j++ )
        {
            if (j == 0)
                pProvMngm->pProvPackGroupEntry[j].UsedFreeLink.Prev  = EDDP_CRT_CHAIN_IDX_END;
            else
                pProvMngm->pProvPackGroupEntry[j].UsedFreeLink.Prev  = (LSA_UINT16) j - 1;
            pProvMngm->pProvPackGroupEntry[j].UsedFreeLink.Next      = (LSA_UINT16) j + 1;
            pProvMngm->pProvPackGroupEntry[j].PackGrId               = (LSA_UINT16) j;
            pProvMngm->pProvPackGroupEntry[j].AcwId                  = EDDP_CRT_CHAIN_IDX_END;   // not yet used
        }
        // last chain element
        pProvMngm->pProvPackGroupEntry[MaxProvPackGroup-1].UsedFreeLink.Next = EDDP_CRT_CHAIN_IDX_END;

        /* -------------------------------------------------------------------------------- */
        /* init APICtrl-RAM addresses                                                       */
        /* -------------------------------------------------------------------------------- */
        /* ---------------------------------------------------------------- */
        /* base address of ACW array in APICtrl-RAM                         */
        /* ---------------------------------------------------------------- */
        #if defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)
        if(EDDP_HAL_HW_ERTEC200P == HwType)
        {
            /* ----------------------------------------------------------------- */
            /* virtualization of ACW handler functions for different revisions   */
            /* ----------------------------------------------------------------- */
            pProvMngm->AcwHandler.FctInitHwApiRTFrame                 = EDDP_CRTProvAcwInitHwApiRTFrame;
            pProvMngm->AcwHandler.FctInitHwApiPackFrame               = EDDP_CRTProvAcwInitHwApiPackFrame;
            pProvMngm->AcwHandler.FctUpdateAcwNextOffsetAcwHwApi      = EDDP_CRTProvAcwUpdateAcwNextOffsetAcwHwApi;
            pProvMngm->AcwHandler.FctWriteSBOffset                    = EDDP_CRTProvAcwWriteSBOffset;
            pProvMngm->AcwHandler.FctUpdateAcwDGNextOffsetAcwHwApi    = EDDP_CRTProvAcwDGUpdateAcwDGNextOffsetAcwHwApi;
			pProvMngm->AcwHandler.FctDGInitHwApiFromPrmCommit         = EDDP_CRTProvAcwDGInitHwApiFromPrmCommit;
			pProvMngm->AcwHandler.FctDGInitHwApiFromProvider          = EDDP_CRTProvAcwDGInitHwApiFromProvider;

            /* ----------------------------------------------------------------- */
            /* base address of ACW array in APICtrl-RAM                          */
            /* ----------------------------------------------------------------- */
            pProvMngm->pHwBaseAcwApi =
                (PNIP_ACW_SND_PTR_TYPE)EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_API_CTRL, EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_PROV_ACW, &APICtrlRAM_Size);

            if (LSA_HOST_PTR_ARE_EQUAL(pProvMngm->pHwBaseAcwApi, LSA_NULL))
            {
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTInitProviderDevOpen(): FATAL -> LSA_NULL = EDDP_HALGetRAMSubAddress() for PROV_ACW");
                EDDP_FATAL(EDDP_FATAL_ERR_GET_APICTRLRAM);
            }
            else
            {
                // check "APICtrlRAM_Size" of ACW container
                if (APICtrlRAM_Size != ((pDDB->CRT.CfgHwAllocParamApi.MaxHWPpmAcw) * sizeof(EDDP_HAL_APICTRL_CRT_PROV_ACW)))
                {
                    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTInitProviderDevOpen(): FATAL -> APICtrlRAM_Size(0x%X) not identical with sizeof(PROV_ACW)=0x%X",
                        APICtrlRAM_Size, ((pDDB->CRT.CfgHwAllocParamApi.MaxHWPpmAcw) * sizeof(EDDP_HAL_APICTRL_CRT_PROV_ACW)));
                    EDDP_FATAL(EDDP_FATAL_ERR_GET_APICTRLRAM);
                }
            }
        }
        #endif
            
        /* ---------------------------------------------------------------- */
        /* base address of ACW_DG array in APICtrl-RAM                      */
        /* ---------------------------------------------------------------- */
        pProvMngm->pHwBaseAcwDGApi = 
            (PNIP_ACWDG_SND_PTR_TYPE) EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_PROV_ACW_DG, &APICtrlRAM_Size);

        if (LSA_HOST_PTR_ARE_EQUAL(pProvMngm->pHwBaseAcwDGApi, LSA_NULL))
        {
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTInitProviderDevOpen(): FATAL -> LSA_NULL = EDDP_HALGetRAMSubAddress() for PROV_ACW_DG");
            EDDP_FATAL(EDDP_FATAL_ERR_GET_APICTRLRAM);
        }
        else
        {
            // check "APICtrlRAM_Size" of ACW_DG container
            if ( APICtrlRAM_Size != ((pDDB->CRT.CfgHwAllocParamApi.MaxHWPpmAcwDG) * sizeof(EDDP_HAL_APICTRL_CRT_PROV_ACW_DG)) )
            {
                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTInitProviderDevOpen(): FATAL -> APICtrlRAM_Size(0x%X) not identical with sizeof(PROV_ACW_DG)=0x%X", 
                    APICtrlRAM_Size, ((pDDB->CRT.CfgHwAllocParamApi.MaxHWPpmAcwDG) * sizeof(EDDP_HAL_APICTRL_CRT_PROV_ACW_DG)));
                EDDP_FATAL(EDDP_FATAL_ERR_GET_APICTRLRAM);
            }
        }
        
        /* ---------------------------------------------------------------- */
        /* base address of HW send list Mx and Nx in APICtrl-RAM            */
        /* ---------------------------------------------------------------- */

        /* -------------------------------------------------------------------------------- */
        /* DDB specific data                                                                */
        /* -------------------------------------------------------------------------------- */
        pProvMngm          = &pDDB->CRT.Prov;
        MaxProvGroup        = pDDB->CRT.CfgParam.ProviderGroupMaxNumber;
        MaxProvTrog         = pDDB->CRT.CfgParam.ProviderTrogMaxNumber;
        MaxProvDataStatus   = pDDB->CRT.CfgParam.ProviderDataStatusMaxNumber;

        // Provider
        pProvMngm->ProviderUsedList.BeginIdx         = EDDP_CRT_CHAIN_IDX_END;   // not yet used
        pProvMngm->ProviderUsedList.EndIdx           = EDDP_CRT_CHAIN_IDX_END;   // not yet used
        // Acw
        pProvMngm->AcwUsedList.BeginIdx              = EDDP_CRT_CHAIN_IDX_END;   // not yet used
        pProvMngm->AcwUsedList.EndIdx                = EDDP_CRT_CHAIN_IDX_END;   // not yet used
        // AcwDG
        pProvMngm->AcwDGUsedList.BeginIdx            = EDDP_CRT_CHAIN_IDX_END;   // not yet used
        pProvMngm->AcwDGUsedList.EndIdx              = EDDP_CRT_CHAIN_IDX_END;   // not yet used
        // PackGroup
        pProvMngm->PackGroupUsedList.BeginIdx        = EDDP_CRT_CHAIN_IDX_END;   // not yet used
        pProvMngm->PackGroupUsedList.EndIdx          = EDDP_CRT_CHAIN_IDX_END;   // not yet used
        // Trog
        pProvMngm->TrogFreeList.EndIdx               = MaxProvTrog - 1;
        pProvMngm->TrogUsedList.BeginIdx             = EDDP_CRT_CHAIN_IDX_END;   // not yet used
        pProvMngm->TrogUsedList.EndIdx               = EDDP_CRT_CHAIN_IDX_END;   // not yet used
        // DataStatus
        pProvMngm->DSFreeList.EndIdx                 = MaxProvDataStatus - 1;
        pProvMngm->DSUsedList.BeginIdx               = EDDP_CRT_CHAIN_IDX_END;   // not yet used
        pProvMngm->DSUsedList.EndIdx                 = EDDP_CRT_CHAIN_IDX_END;   // not yet used
        // PackGroup

        /* -------------------------------------------------------------------------------- */
        /* init CRT.Prov.pTrogEntry[x]                                                      */
        /* -------------------------------------------------------------------------------- */
        EDDP_MEMSET_COMMON ( pProvMngm->pTrogEntry, 0, (MaxProvTrog * sizeof(EDDP_CRT_PROVIDER_TROG_TYPE)) );

        for ( j=0; j < MaxProvTrog; j++ )
        {
            if (j == 0)
                pProvMngm->pTrogEntry[j].UsedFreeLink.Prev               = EDDP_CRT_CHAIN_IDX_END;
            else
                pProvMngm->pTrogEntry[j].UsedFreeLink.Prev               = (LSA_UINT16) j - 1;
            pProvMngm->pTrogEntry[j].UsedFreeLink.Next                   = (LSA_UINT16) j + 1;
            pProvMngm->pTrogEntry[j].TrogId                              = (LSA_UINT16) j;
            pProvMngm->pTrogEntry[j].TrogAcwList.AcwPhaseList_R.BeginIdx = EDDP_CRT_CHAIN_IDX_END;   // not yet used
            pProvMngm->pTrogEntry[j].TrogAcwList.AcwPhaseList_R.EndIdx   = EDDP_CRT_CHAIN_IDX_END;   // not yet used
            pProvMngm->pTrogEntry[j].TrogAcwList.AcwPhaseList_O.BeginIdx = EDDP_CRT_CHAIN_IDX_END;   // not yet used
            pProvMngm->pTrogEntry[j].TrogAcwList.AcwPhaseList_O.EndIdx   = EDDP_CRT_CHAIN_IDX_END;   // not yet used
            pProvMngm->pTrogEntry[j].TrogAcwList.AcwPhaseList_G.BeginIdx = EDDP_CRT_CHAIN_IDX_END;   // not yet used
            pProvMngm->pTrogEntry[j].TrogAcwList.AcwPhaseList_G.EndIdx   = EDDP_CRT_CHAIN_IDX_END;   // not yet used
        }
        // last chain element
        pProvMngm->pTrogEntry[MaxProvTrog-1].UsedFreeLink.Next           = EDDP_CRT_CHAIN_IDX_END;

        /* -------------------------------------------------------------------------------- */
        /* init CRT.Prov.pProvGroupEntry[x]                                                 */
        /* -------------------------------------------------------------------------------- */
        EDDP_MEMSET_COMMON ( pProvMngm->pProvGroupEntry, 0, (MaxProvGroup * sizeof(EDDP_CRT_PROVIDER_GROUP_TYPE)) );

        for ( j=0; j < MaxProvGroup; j++ )
        {
            pProvMngm->pProvGroupEntry[j].GroupList.BeginIdx = EDDP_CRT_CHAIN_IDX_END;   // not yet used
            pProvMngm->pProvGroupEntry[j].GroupList.EndIdx   = EDDP_CRT_CHAIN_IDX_END;   // not yet used
            pProvMngm->pProvGroupEntry[j].GroupDataStatus    = EDD_CSRT_DSTAT_GLOBAL_INIT_VALUE;
        }

        /* -------------------------------------------------------------------------------- */
        /* init CRT.Prov.pTBaseEntry[x]                                                     */
        /* -------------------------------------------------------------------------------- */
        EDDP_MEMSET_COMMON ( pProvMngm->pTBaseEntry, 0, (EDDP_CRT_MAX_COUNT_TBASE * sizeof(EDDP_CRT_PROVIDER_TBASE_TYPE)) );

        /* -------------------------------------------------------------------------------- */
        /* init CRT.Prov.pDSEntry[x]                                                        */
        /* -------------------------------------------------------------------------------- */
        EDDP_MEMSET_COMMON ( pProvMngm->pDSEntry, 0, (MaxProvDataStatus * sizeof(EDDP_CRT_PROVIDER_DS_TYPE)) );

        for ( j=0; j < MaxProvDataStatus; j++ )
        {
            if (j == 0)
                pProvMngm->pDSEntry[j].UsedFreeLink.Prev = EDDP_CRT_CHAIN_IDX_END;
            else
                pProvMngm->pDSEntry[j].UsedFreeLink.Prev = (LSA_UINT16) j - 1;
            pProvMngm->pDSEntry[j].UsedFreeLink.Next     = (LSA_UINT16) j + 1;
        }
        // last chain element
        pProvMngm->pDSEntry[MaxProvDataStatus-1].UsedFreeLink.Next = EDDP_CRT_CHAIN_IDX_END;

        /* -------------------------------------------------------------------------------- */
        /* init APICtrl-RAM addresses                                                       */
        /* -------------------------------------------------------------------------------- */
        // IFA
        RamSubApiCtrl_TBase = EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_TBASE_LIST;
        RamSubApiCtrl_Trog  = EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_TROG_LIST;
        RamSubApiCtrl_DS    = EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_DS_LIST;

        /* ---------------------------------------------------------------- */
        /* base address of TBase array in APICtrl-RAM                       */
        /* ---------------------------------------------------------------- */
        pProvMngm->pHwBaseTBaseApi = 
            (PNIP_ACW_SND_TBASE_PTR_TYPE) EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, RamSubApiCtrl_TBase, &APICtrlRAM_Size);

        if (LSA_HOST_PTR_ARE_EQUAL(pProvMngm->pHwBaseTBaseApi, LSA_NULL))
        {
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTInitProviderDevOpen(IF[]): FATAL -> LSA_NULL = EDDP_HALGetRAMSubAddress() for TBASE_LIST");
            EDDP_FATAL(EDDP_FATAL_ERR_GET_APICTRLRAM);
        }
        else
        {
            // check "APICtrlRAM_Size" of TBase container
            if (RamSubApiCtrl_TBase == EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_TBASE_LIST)
            {
                // IFA
                if ( APICtrlRAM_Size != (sizeof(EDDP_HAL_APICTRL_CRT_TBASE_ARRAY)) )
                {
                    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTInitProviderDevOpen(IF[]): FATAL -> APICtrlRAM_Size(0x%X) not identical with sizeof(TBASE_LIST)=0x%X", 
                        APICtrlRAM_Size, sizeof(EDDP_HAL_APICTRL_CRT_TBASE_ARRAY));
                    EDDP_FATAL(EDDP_FATAL_ERR_GET_APICTRLRAM);
                }
            }
        }

        /* ---------------------------------------------------------------- */
        /* base address of Trog array in APICtrl-RAM                        */
        /* ---------------------------------------------------------------- */
        pProvMngm->pHwBaseTrogApi = 
            (PNIP_ACW_SND_TROG_PTR_TYPE) EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, RamSubApiCtrl_Trog, &APICtrlRAM_Size);

        if (LSA_HOST_PTR_ARE_EQUAL(pProvMngm->pHwBaseTrogApi, LSA_NULL))
        {
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTInitProviderDevOpen(IF[]): FATAL -> LSA_NULL = EDDP_HALGetRAMSubAddress() for TROG_LIST");
            EDDP_FATAL(EDDP_FATAL_ERR_GET_APICTRLRAM);
        }
        else
        {
            // check "APICtrlRAM_Size" of TROG container
            if (RamSubApiCtrl_Trog == EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_TROG_LIST)
            {
                // IFA
                if ( APICtrlRAM_Size != ((pDDB->CRT.CfgHwAllocParamApi.MaxHWPpmTrog) * sizeof(EDDP_HAL_APICTRL_CRT_TROG_ARRAY)) )
                {
                    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTInitProviderDevOpen(IF[]): FATAL -> APICtrlRAM_Size(0x%X) not identical with sizeof(TROG_LIST)=0x%X", 
                        APICtrlRAM_Size, ((pDDB->CRT.CfgHwAllocParamApi.MaxHWPpmTrog) * sizeof(EDDP_HAL_APICTRL_CRT_TROG_ARRAY)));
                    EDDP_FATAL(EDDP_FATAL_ERR_GET_APICTRLRAM);
                }
            }
        }

        /* ---------------------------------------------------------------- */
        /* base address of DataStatus array in APICtrl-RAM                  */
        /* ---------------------------------------------------------------- */
        // switch to current PNIP revision
        switch (EDDP_HALGetPnipRevision(pDDB))
        {
            #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
            case EDDP_HAL_HW_PNIP_REV1:
            {
                pProvMngm->pHwBaseDSApiPnipR1 = 
                    (PNIP_R1_ACW_SND_DS_PTR_TYPE) EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, RamSubApiCtrl_DS, &APICtrlRAM_Size);

                if (LSA_HOST_PTR_ARE_EQUAL(pProvMngm->pHwBaseDSApiPnipR1, LSA_NULL))
                {
                    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTInitProviderDevOpen(IF[]): FATAL -> LSA_NULL = EDDP_HALGetRAMSubAddress() for DS_LIST");
                    EDDP_FATAL(EDDP_FATAL_ERR_GET_APICTRLRAM);
                }
                else
                {
                    // check "APICtrlRAM_Size" of DataStatus container
                    if (RamSubApiCtrl_DS == EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_DS_LIST)
                    {
                        // IFA
                        if ( APICtrlRAM_Size != ((pDDB->CRT.CfgHwAllocParamApi.MaxHWPpmDataStatus) * sizeof(EDDP_HAL_PNIP_R1_APICTRL_CRT_DS_ARRAY)) )
                        {
                            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTInitProviderDevOpen(IF[]): FATAL -> APICtrlRAM_Size(0x%X) not identical with sizeof(DS_LIST)=0x%X", 
                                APICtrlRAM_Size, ((pDDB->CRT.CfgHwAllocParamApi.MaxHWPpmDataStatus) * sizeof(EDDP_HAL_PNIP_R1_APICTRL_CRT_DS_ARRAY)));
                            EDDP_FATAL(EDDP_FATAL_ERR_GET_APICTRLRAM);
                        }
                    }
                }
            }
            break;
            #endif

            #if defined(EDDP_CFG_HW_PNIP_REV2_SUPPORT)
            case EDDP_HAL_HW_PNIP_REV2:
            case EDDP_HAL_HW_PNIP_REV3:
            {
                pProvMngm->pHwBaseDSApiPnipR2 = 
                    (PNIP_R2_ACW_SND_DS_PTR_TYPE) EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_API_CTRL, RamSubApiCtrl_DS, &APICtrlRAM_Size);

                if (LSA_HOST_PTR_ARE_EQUAL(pProvMngm->pHwBaseDSApiPnipR2, LSA_NULL))
                {
                    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTInitProviderDevOpen(IF[]): FATAL -> LSA_NULL = EDDP_HALGetRAMSubAddress() for DS_LIST");
                    EDDP_FATAL(EDDP_FATAL_ERR_GET_APICTRLRAM);
                }
                else
                {
                    // check "APICtrlRAM_Size" of DataStatus container
                    if (RamSubApiCtrl_DS == EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_DS_LIST)
                    {
                        // IFA
                        if ( APICtrlRAM_Size != ((pDDB->CRT.CfgHwAllocParamApi.MaxHWPpmDataStatus) * sizeof(EDDP_HAL_PNIP_R2_APICTRL_CRT_DS_ARRAY)) )
                        {
                            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTInitProviderDevOpen(IF[]): FATAL -> APICtrlRAM_Size(0x%X) not identical with sizeof(DS_LIST)=0x%X", 
                                APICtrlRAM_Size, ((pDDB->CRT.CfgHwAllocParamApi.MaxHWPpmDataStatus) * sizeof(EDDP_HAL_PNIP_R2_APICTRL_CRT_DS_ARRAY)));
                            EDDP_FATAL(EDDP_FATAL_ERR_GET_APICTRLRAM);
                        }
                    }
                }
            }
            break;
            #endif
            
            default:
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTInitProviderDevOpen(): FATAL -> Version(%d) of PNIP is unknown", 
                    EDDP_HALGetPnipRevision(pDDB));
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }
            break;
        }
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname       :   EDDP_CRTInitProviderDevSetup                                    +*/
/*+                                                                                         +*/
/*+  Input/Output       :   EDDP_LOCAL_DDB_PTR_TYPE         pDDB                            +*/
/*+                     :   EDDP_UPPER_DSB_PTR_TYPE         pDSB                            +*/
/*+                                                                                         +*/
/*+  Result             :   EDD_RSP                                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  pDDB       : Pointer to Device Description Block                                       +*/
/*+  pDPB       : Pointer to Device Setup Block                                             +*/
/*+                                                                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: This function is called at time of DEV_SETUP                              +*/
/*+                 -> write defaults into HW RAM                                           +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTInitProviderDevSetup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_UPPER_DSB_PTR_TYPE     pDSB)
{
    EDD_RSP                                 Response;
    //LSA_UINT32                              i;
    LSA_UINT16                              MaxProvTrog;
    LSA_UINT32                              RamSubApiCtrl_Trog, APICtrlRAM_Size_Trog;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pDSB);
#endif

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTInitProviderDevSetup(pDSB=0x%X)", pDSB);

    Response                = EDD_STS_OK;
    RamSubApiCtrl_Trog      = EDDP_CRT_INVALID_VALUE_32;    // init to prevent compiler warning
    APICtrlRAM_Size_Trog    = EDDP_CRT_INVALID_VALUE_32;

    if (pDDB->CRT.CfgParam.ProviderMaxNumber > 0)
    {
        MaxProvTrog = pDDB->CRT.CfgParam.ProviderTrogMaxNumber;

        if (MaxProvTrog > 0)
        {
            /* -------------------------------------------------------- */
            /* init container                                           */
            /* -------------------------------------------------------- */
            RamSubApiCtrl_Trog      = EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_TROG_LIST;
            APICtrlRAM_Size_Trog    = (MaxProvTrog * sizeof(EDDP_HAL_APICTRL_CRT_TROG_ARRAY));
                
            /* -------------------------------------------------------- */
            /* write init value to Trog container in APICtrl-RAM        */
            /* -------------------------------------------------------- */
            EDDP_HALMemset ( pDDB, 
                                EDDP_HAL_RAMTYPE_API_CTRL, 
                                RamSubApiCtrl_Trog, 
                                0, 
                                APICtrlRAM_Size_Trog, 
                                EDDP_CRT_INVALID_VALUE_8);     // 0xFF necessary for command "ACW_move"
        }
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname       :   EDDP_CRTInitProviderASGroupDevOpen                              +*/
/*+                                                                                         +*/
/*+  Input/Output       :   EDDP_LOCAL_DDB_PTR_TYPE         pDDB                            +*/
/*+                                                                                         +*/
/*+  Result             :   LSA_VOID                                                        +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+                                                                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: This function is called by EDDP_CRTInitConsumerDevOpen() and inits        +*/
/*+               the AutoStop provider array pASGroupEntry[x]                              +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_VOID EDDP_LOCAL_FCT_ATTR _EDDP_CRTInitProviderASGroupDevOpen(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT32                              i;
    LSA_UINT16                              MaxCons;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;

    pProvMngm    = &pDDB->CRT.Prov;
    MaxCons         = pDDB->CRT.CfgParam.ConsumerMaxNumber;

    if (LSA_HOST_PTR_ARE_EQUAL(pProvMngm->pASGroupEntry, LSA_NULL))
    {
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTInitProviderASGroupDevOpen(): FATAL -> pASGroupEntry=NULL");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    else
    {
        EDDP_MEMSET_COMMON ( pProvMngm->pASGroupEntry, 0, (MaxCons * sizeof(EDDP_CRT_PROVIDER_ASGROUP_TYPE)) );

        for ( i=0; i < MaxCons; i++ )
        {
            pProvMngm->pASGroupEntry[i].ASGroupList.BeginIdx = EDDP_CRT_CHAIN_IDX_END;
            pProvMngm->pASGroupEntry[i].ASGroupList.EndIdx   = EDDP_CRT_CHAIN_IDX_END;
        }
    }
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname       :   EDDP_CRTInitConsumerDevOpen                                     +*/
/*+                                                                                         +*/
/*+  Input/Output       :   EDDP_LOCAL_DDB_PTR_TYPE         pDDB                            +*/
/*+                     :   EDDP_UPPER_DPB_PTR_TYPE         pDPB                            +*/
/*+                                                                                         +*/
/*+  Result             :   EDD_RSP                                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  pDDB       : Pointer to Device Description Block                                       +*/
/*+  pDPB       : Pointer to Device Parameter Block                                         +*/
/*+                                                                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: This function creates a consumer list at time of DEV_OPEN                 +*/
/*+                 -> init struct EDDP_CRT_CONSUMER_MNGM_TYPE                              +*/
/*+                 -> EDDP-CORE has inited the DDB already with 0                          +*/
/*+                 -> write only values != 0                                               +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTInitConsumerDevOpen(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_UPPER_DPB_PTR_TYPE     pDPB)
{
    EDD_RSP                                 Response;
    LSA_UINT16                              ItemIdx, MaxCons, MaxAllAcw, MaxDgCons, MaxContainerCons;
    LSA_UINT32                              RamSubApiCtrl_ConsTsb,  RamSubApiCtrl_ConsTsbLen;
    LSA_UINT32                              RamSubApiCtrl_ConsCsb0, RamSubApiCtrl_ConsCsbLen;
    LSA_UINT32                              RamSubApiCtrl_ConsCsb1;
    LSA_UINT32                              ExpectedSize;
    EDDP_CRT_CONS_ACW_INFO_PTR_TYPE         pCommonAcwPtr;
    LSA_BOOL                                IsContainer;
    EDDP_CRT_CONSUMER_PTR_TYPE              pConsumer;
    EDDP_CRT_CONSUMER_DG_PTR_TYPE           pConsDgEntry;
    EDDP_CRT_CONSUMER_CONTAINER_PTR_TYPE    pConsContainerEntry;
    LSA_UINT16                              HwType, RBaseEntryCnt;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pDPB);
#endif

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTInitConsumerDevOpen(pDPB=0x%X)", pDPB);

    Response                    = EDD_STS_OK;
    MaxCons                     = pDDB->CRT.CfgParam.ConsumerMaxNumber;
    MaxContainerCons            = pDDB->CRT.CfgParam.ConsumerContainerMaxNumber;
    MaxDgCons                   = pDDB->CRT.CfgParam.ConsumerDGMaxNumber;
    MaxAllAcw                   = MaxCons + MaxContainerCons;
    RamSubApiCtrl_ConsTsb       = EDDP_CRT_INVALID_VALUE_32;
    RamSubApiCtrl_ConsTsbLen    = EDDP_CRT_INVALID_VALUE_32;
    RamSubApiCtrl_ConsCsb0      = EDDP_CRT_INVALID_VALUE_32;
    RamSubApiCtrl_ConsCsb1      = EDDP_CRT_INVALID_VALUE_32;
    RamSubApiCtrl_ConsCsbLen    = EDDP_CRT_INVALID_VALUE_32;
    RBaseEntryCnt               = pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmRBaseEntries;
	ExpectedSize				= 0;

    pDDB->CRT.Cons.ConsumerUsedCnt    = 0;
    pDDB->CRT.Cons.ConsumerActCnt     = 0;
    pDDB->CRT.Cons.ConsumerDgUsedCnt  = 0;
    
    HwType = EDDP_HALGetChipVersion(pDDB);
    
    /* hardware type must either ERTEC200P or HERA, assured on HAL setup */
    EDDP_ASSERT((EDDP_HAL_HW_ERTEC200P == HwType)||(EDDP_HAL_HW_HERA == HwType));

    if (MaxCons > 0)
    {
        /* -------------------------------------------------------------------------------- */
        /* DDB specific data                                                                */
        /* -------------------------------------------------------------------------------- */
        
        /* NOTE: with HERA maximum consumer count can differ from maximum DG count */
        if(EDDP_HAL_HW_ERTEC200P == HwType)
        {
            EDDP_ASSERT_FALSE(MaxCons != MaxDgCons);
        }

        // ***** pConsEntry
        EDDP_MEMSET_COMMON ( pDDB->CRT.Cons.pConsEntry, 0, (MaxCons * sizeof(EDDP_CRT_CONSUMER_TYPE)) );
        // ***** Note: linkage within pDDB->CRT.Cons.pConsEntry[x].UsedFreeLink
        pDDB->CRT.Cons.ConsumerFreeList.BeginIdx   = 0;
        pDDB->CRT.Cons.ConsumerFreeList.EndIdx     = MaxCons - 1;
        for ( ItemIdx=0; ItemIdx < MaxCons; ItemIdx++ )
        {
            pConsumer = &pDDB->CRT.Cons.pConsEntry[ItemIdx];
            // pConsumer->ConsAcwTypeInfo will be set later
            pConsumer->UsedFreeLink.Prev = ItemIdx - 1;
            pConsumer->UsedFreeLink.Next = ItemIdx + 1;

            pConsumer->ConsumerId       = ItemIdx;
            pConsumer->State            = EDDP_CRT_STATE_FREE;
            pConsumer->ConsumerFlags    = 0;
            pConsumer->DgIdx            = EDDP_CRT_CHAIN_IDX_END;
            pConsumer->DgGrpIdx         = EDDP_CRT_CHAIN_IDX_END;
        }
        pDDB->CRT.Cons.pConsEntry[0].UsedFreeLink.Prev         = EDDP_CRT_CHAIN_IDX_END;    // first chain element
        pDDB->CRT.Cons.pConsEntry[MaxCons-1].UsedFreeLink.Next = EDDP_CRT_CHAIN_IDX_END;    // last chain element

        // ***** pConsDgEntry
        EDDP_MEMSET_COMMON ( pDDB->CRT.Cons.pConsDgEntry, 0, (MaxDgCons * sizeof(EDDP_CRT_CONSUMER_DG_TYPE)) );
        // ***** Note: linkage within pDDB->CRT.Cons.pConsDgEntry[x].UsedFreeDgLink
        pDDB->CRT.Cons.ConsumerDgFreeList.BeginIdx   = 0;
        pDDB->CRT.Cons.ConsumerDgFreeList.EndIdx     = MaxDgCons - 1;
        for ( ItemIdx=0; ItemIdx < MaxDgCons; ItemIdx++ )
        {
            pConsDgEntry = &pDDB->CRT.Cons.pConsDgEntry[ItemIdx];
            pConsDgEntry->DgIdx               = ItemIdx;  // own IDX
            pConsDgEntry->DgGrpIdx            = EDDP_CRT_CHAIN_IDX_END;

            pConsDgEntry->UsedFreeDgLink.Prev = ItemIdx - 1;
            pConsDgEntry->UsedFreeDgLink.Next = ItemIdx + 1;

            pConsDgEntry->DgLink.Prev         = EDDP_CRT_CHAIN_IDX_END;
            pConsDgEntry->DgLink.Next         = EDDP_CRT_CHAIN_IDX_END;
            pConsDgEntry->ConnectedConsumerId = EDDP_CRT_CHAIN_IDX_END;
            pConsDgEntry->pSubFrameData       = LSA_NULL;
        }
        pDDB->CRT.Cons.pConsDgEntry[0].UsedFreeDgLink.Prev           = EDDP_CRT_CHAIN_IDX_END;    // first chain element
        pDDB->CRT.Cons.pConsDgEntry[MaxDgCons-1].UsedFreeDgLink.Next = EDDP_CRT_CHAIN_IDX_END;    // last chain element

        // ***** pConsContainerEntry
        EDDP_MEMSET_COMMON ( pDDB->CRT.Cons.pConsContainerEntry, 0, (MaxContainerCons * sizeof(EDDP_CRT_CONSUMER_CONTAINER_TYPE)) );
        for ( ItemIdx=0; ItemIdx < MaxContainerCons; ItemIdx++ )
        {
            pConsContainerEntry = &pDDB->CRT.Cons.pConsContainerEntry[ItemIdx];
            // pConsContainerEntry->ConsAcwTypeInfo will be set later
            pConsContainerEntry->DgGrpIdx             = ItemIdx;  // own IDX
            pConsContainerEntry->ConsDgList.BeginIdx  = EDDP_CRT_CHAIN_IDX_END;
            pConsContainerEntry->ConsDgList.EndIdx    = EDDP_CRT_CHAIN_IDX_END;
            pConsContainerEntry->DgCount              = 0;
            pConsContainerEntry->pPDIRFrameData       = LSA_NULL;
        }

        // ***** pRBasePtrArray: we bring two different structures together into pRBasePtrArray
        //                       to have a common access way for linkage into RBase
        EDDP_MEMSET_COMMON ( pDDB->CRT.Cons.pAcwPtrArray, 0, (MaxAllAcw * sizeof(EDDP_CRT_CONS_ACW_INFO_PTR_TYPE)) );
        for ( ItemIdx=0; ItemIdx < MaxAllAcw; ItemIdx++ )
        {
            if ( ItemIdx < MaxCons )
            {   // ***** normal ACW
                pCommonAcwPtr = &(pDDB->CRT.Cons.pConsEntry[ItemIdx].ConsAcwTypeInfo);
                IsContainer = LSA_FALSE;
            }
            else
            {   // ***** container ACW
                pCommonAcwPtr = &(pDDB->CRT.Cons.pConsContainerEntry[ItemIdx-MaxCons].ConsAcwTypeInfo);
                IsContainer = LSA_TRUE;
            }
            pDDB->CRT.Cons.pAcwPtrArray[ItemIdx] = pCommonAcwPtr;

            pCommonAcwPtr->AcwIdx         = ItemIdx;   // own IDX
            pCommonAcwPtr->RBaseLink.Prev = EDDP_CRT_CHAIN_IDX_END;
            pCommonAcwPtr->RBaseLink.Next = EDDP_CRT_CHAIN_IDX_END;
            pCommonAcwPtr->IsAcwContainer = IsContainer;
            pCommonAcwPtr->FrameID        = 0;
        }
                
        /* ---------------------------------------------------------------- */
        /* base address of APDU array in APICtrl-RAM                        */
        /* ---------------------------------------------------------------- */
        ExpectedSize = (pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmAllAcw * sizeof(EDDP_HAL_APICTRL_CRT_CONS_APDU));
        pDDB->CRT.Cons.pHwBaseApduApi = (PNIP_ADPU_RCV_PTR_TYPE) _EDDP_CRTAssignApiCtrlBaseAddress (pDDB, EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_CONS_APDU, ExpectedSize);
                
        /* ---------------------------------------------------------------- */
        /* base address of DG_Status array in APICtrl-RAM                   */
        /* ---------------------------------------------------------------- */
        ExpectedSize = (pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmAcwDG * sizeof(EDDP_HAL_APICTRL_CRT_CONS_DG_STATUS));
        pDDB->CRT.Cons.pHwBaseDgStatusApi = (PNIP_DG_STATUS_RCV_PTR_TYPE) _EDDP_CRTAssignApiCtrlBaseAddress (pDDB, EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_CONS_DG_STATUS, ExpectedSize);
                
                
        /* ---------------------------------------------------------------- */
        /* handle revision depending stuff for ERTEC200P                    */
        /* ---------------------------------------------------------------- */
        #if defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)
        if(EDDP_HAL_HW_ERTEC200P == HwType)
        {                
            /* ----------------------------------------------------------------- */
            /* virtualization of RBase handler functions for different revisions */
            /* ----------------------------------------------------------------- */
            pDDB->CRT.Cons.RBaseHandler.FctAddAcwToRBase      = EDDP_CRTConsumerAddAcwToRBaseSWList;
            pDDB->CRT.Cons.RBaseHandler.FctRemoveAcwFromRBase = EDDP_CRTConsumerRemoveAcwFromRBaseSWList;
            pDDB->CRT.Cons.RBaseHandler.FctGetRBaseIndex      = EDDP_CRTGetRBaseIndex;
            
            /* ----------------------------------------------------------------- */
            /* virtualization of ACW handler functions for different revisions   */
            /* ----------------------------------------------------------------- */
            pDDB->CRT.Cons.AcwHandler.FctBasicInitAcwDgHwApi         = EDDP_CRTConsBasicInitConsAcwDgHwApi;
            pDDB->CRT.Cons.AcwHandler.FctFinalInitAcwDgHwApi         = EDDP_CRTConsFinalInitConsAcwDgHwApi;
            pDDB->CRT.Cons.AcwHandler.FctInitAcwHwApi                = EDDP_CRTConsInitConsAcwHwApi;
            pDDB->CRT.Cons.AcwHandler.FctInitContainerAcwHwApi       = EDDP_CRTConsInitContainerAcwHwApi;
            pDDB->CRT.Cons.AcwHandler.FctSetupLegacyStartupAcwHwApi  = EDDP_CRTConsSetupLegacyStartupAcwHwApi;
            pDDB->CRT.Cons.AcwHandler.FctUpdateAcwNextOffsetAcwHwApi = EDDP_CRTConsUpdateAcwNextOffsetAcwHwApi;
            pDDB->CRT.Cons.AcwHandler.FctUpdateSrcAddrCsaAcwHwApi    = EDDP_CRTConsUpdateSrcAddrCsaInConsAcwHwApi;
            
            /* ---------------------------------------------------------------- */
            /* base address of ACW array in APICtrl-RAM                         */
            /* ---------------------------------------------------------------- */
            ExpectedSize = (pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmAllAcw * sizeof(EDDP_HAL_APICTRL_CRT_CONS_ACW));
            pDDB->CRT.Cons.pHwBaseAcwApi = _EDDP_CRTAssignApiCtrlBaseAddress (pDDB, EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_CONS_ACW, ExpectedSize);            
            
            /* ---------------------------------------------------------------- */
            /* base address of DG array in APICtrl-RAM                          */
            /* ---------------------------------------------------------------- */
            ExpectedSize = (pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmAcwDG * sizeof(EDDP_HAL_APICTRL_CRT_CONS_ACW_DG));
            pDDB->CRT.Cons.pHwBaseAcwDgApi = _EDDP_CRTAssignApiCtrlBaseAddress (pDDB, EDDP_HAL_RAMSUB_APICTRL_CRT_DDB_CONS_ACW_DG, ExpectedSize);             
            
            /* ---------------------------------------------------------------- */
            /* base address of ACW index table in API-RT-Ctrl-RAM               */
            /* NOTE: for init see EDDP_CRTInitConsumerDevSetup                  */
            /* ---------------------------------------------------------------- */
            ExpectedSize = (sizeof(EDDP_HAL_APICTRL_CRT_RBASE_ARRAY));
            pDDB->CRT.Cons.pHwBaseRBaseApi = (PNIP_ACW_RCV_RBASE_PTR_TYPE) _EDDP_CRTAssignApiCtrlBaseAddress (pDDB, EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_RBASE_LIST, ExpectedSize);
            
            /* ---------------------------------------------------------------- */
            /* assign ACW and APDU respectively ACW-DG and DG-Status to         */
            /* consumer, consumer-container and DG-consumer                     */
            /* ---------------------------------------------------------------- */ 
            
            // ***** assign pAcwApi and pApduApi address to every consumer
            for ( ItemIdx=0; ItemIdx < MaxCons; ItemIdx++ )
            {
                pDDB->CRT.Cons.pConsEntry[ItemIdx].ConsAcwTypeInfo.pHwAcwApi     = &((PNIP_ACW_RCV_PTR_TYPE)(pDDB->CRT.Cons.pHwBaseAcwApi))[ItemIdx];
                pDDB->CRT.Cons.pConsEntry[ItemIdx].ConsAcwTypeInfo.pHwApduApi    = &pDDB->CRT.Cons.pHwBaseApduApi[ItemIdx];
                // Note: pHwBaseTSBApi, pHwBaseCSB0Api and pHwBaseCSB1Api must be assign interface specific  
            }
            // ***** assign pAcwApi and pApduApi address to every container ACW
            for ( ItemIdx=0; ItemIdx < MaxContainerCons; ItemIdx++ )
            {
                pDDB->CRT.Cons.pConsContainerEntry[ItemIdx].ConsAcwTypeInfo.pHwAcwApi    = &((PNIP_ACW_RCV_PTR_TYPE)(pDDB->CRT.Cons.pHwBaseAcwApi))[MaxCons+ItemIdx];
                pDDB->CRT.Cons.pConsContainerEntry[ItemIdx].ConsAcwTypeInfo.pHwApduApi   = &pDDB->CRT.Cons.pHwBaseApduApi[MaxCons+ItemIdx];
            }
            // ***** assign pHwAcwDgApi and pHwDgStatusApi address to every ACW-DG
            for ( ItemIdx=0; ItemIdx < MaxDgCons; ItemIdx++ )
            {
                pDDB->CRT.Cons.pConsDgEntry[ItemIdx].pHwAcwDgApi     = &((PNIP_ACW_DG_RCV_PTR_TYPE)(pDDB->CRT.Cons.pHwBaseAcwDgApi))[ItemIdx];
                pDDB->CRT.Cons.pConsDgEntry[ItemIdx].pHwDgStatusApi  = &pDDB->CRT.Cons.pHwBaseDgStatusApi[ItemIdx];
                pDDB->CRT.Cons.pConsDgEntry[ItemIdx].pSubFrameData   = LSA_NULL;
            }
        }
        #endif
        
        /* ---------------------------------------------------------------- */
        /* initialize RBase entries                                         */
        /* ---------------------------------------------------------------- */  
        EDDP_MEMSET_COMMON ( pDDB->CRT.Cons.pRBaseEntry, 0, (RBaseEntryCnt * sizeof(EDDP_CRT_CONSUMER_RBASE_TYPE)) );

        for ( ItemIdx=0; ItemIdx < RBaseEntryCnt; ItemIdx++ )
        {
            pDDB->CRT.Cons.pRBaseEntry[ItemIdx].ConsRBaseList.BeginIdx = EDDP_CRT_CHAIN_IDX_END;   // consumer not yet used
            pDDB->CRT.Cons.pRBaseEntry[ItemIdx].ConsRBaseList.EndIdx   = EDDP_CRT_CHAIN_IDX_END;   // consumer not yet used
            
            /* for init, see EDDP_CRTInitConsumerDevSetup */
            pDDB->CRT.Cons.pRBaseEntry[ItemIdx].pHwRBaseApi = &pDDB->CRT.Cons.pHwBaseRBaseApi[ItemIdx];
        }
                
        pDDB->CRT.Cons.RequestFinishConsumerID = EDDP_CRT_CHAIN_IDX_END;

        /* -------------------------------------------------------------------------------- */
        /* DDB specific data                                                                */
        /* -------------------------------------------------------------------------------- */

        /* -------------------------------------------------------------------------------- */
        /* init CRT.Cons.pConsEntry[x]                                                      */
        /* -------------------------------------------------------------------------------- */
        pDDB->CRT.Cons.ConsumerUsedList.BeginIdx    = EDDP_CRT_CHAIN_IDX_END;   // consumer not yet used
        pDDB->CRT.Cons.ConsumerUsedList.EndIdx      = EDDP_CRT_CHAIN_IDX_END;   // consumer not yet used
        pDDB->CRT.Cons.ConsumerUsedCnt              = 0;

        /* -------------------------------------------------------------------------------- */
        /* init CRT.Cons.pConsDgEntry[x]                                                    */
        /* -------------------------------------------------------------------------------- */
        pDDB->CRT.Cons.ConsumerDgUsedList.BeginIdx    = EDDP_CRT_CHAIN_IDX_END;   // consumer not yet used
        pDDB->CRT.Cons.ConsumerDgUsedList.EndIdx      = EDDP_CRT_CHAIN_IDX_END;   // consumer not yet used
        pDDB->CRT.Cons.ConsumerDgUsedCnt              = 0;

        /* -------------------------------------------------------------------------------- */
        /* init CRT.Prov.pASGroupEntry[x]                                                   */
        /* -------------------------------------------------------------------------------- */
        _EDDP_CRTInitProviderASGroupDevOpen (pDDB);

        /* -------------------------------------------------------------------------------- */
        /* Init IF specific parameters. e.g APICtrl-RAM addresses                           */
        /* -------------------------------------------------------------------------------- */
        // IFA
        RamSubApiCtrl_ConsTsb       = EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_CONS_TSB;
        RamSubApiCtrl_ConsTsbLen    = (pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmAcw * sizeof(EDDP_HAL_APICTRL_CRT_CONS_TSB));
        RamSubApiCtrl_ConsCsb0      = EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_CONS_CSB0;
        RamSubApiCtrl_ConsCsb1      = EDDP_HAL_RAMSUB_APICTRL_CRT_IFA_CONS_CSB1;
        RamSubApiCtrl_ConsCsbLen    = (pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmAcw * sizeof(EDDP_HAL_APICTRL_CRT_CONS_CSB));

        pDDB->CRT.Cons.PNIPCSBControlReg = PNIP_REG_CSB_CONTROL_IFA;

        /* ---------------------------------------------------------------- */
        /* base address of TSB array in APICtrl-RAM                         */
        /* ---------------------------------------------------------------- */
        pDDB->CRT.Cons.pHwBaseTSBApi = (PNIP_CPM_TSB_PTR_TYPE) _EDDP_CRTAssignApiCtrlBaseAddress (pDDB, RamSubApiCtrl_ConsTsb, RamSubApiCtrl_ConsTsbLen);

        /* ---------------------------------------------------------------- */
        /* base address of CSB0/1 array in APICtrl-RAM                      */
        /* ---------------------------------------------------------------- */
        pDDB->CRT.Cons.pHwBaseCSB0Api = (PNIP_CPM_CSB_PTR_TYPE) _EDDP_CRTAssignApiCtrlBaseAddress (pDDB, RamSubApiCtrl_ConsCsb0, RamSubApiCtrl_ConsCsbLen);
        pDDB->CRT.Cons.pHwBaseCSB1Api = (PNIP_CPM_CSB_PTR_TYPE) _EDDP_CRTAssignApiCtrlBaseAddress (pDDB, RamSubApiCtrl_ConsCsb1, RamSubApiCtrl_ConsCsbLen);

        pDDB->CRT.Cons.SBEventPending = LSA_FALSE;

        /* ---------------------------------------------------------------- */
        /* CT_ConsumerID                                                    */
        /* ---------------------------------------------------------------- */
        pDDB->CRT.Cons.CT_ConsumerID = EDDP_CRT_CHAIN_IDX_END;
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname       :   EDDP_CRTInitConsumerDevSetup                                    +*/
/*+                                                                                         +*/
/*+  Input/Output       :   EDDP_LOCAL_DDB_PTR_TYPE         pDDB                            +*/
/*+                     :   EDDP_UPPER_DSB_PTR_TYPE         pDSB                            +*/
/*+                                                                                         +*/
/*+  Result             :   EDD_RSP                                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  pDDB       : Pointer to Device Description Block                                       +*/
/*+  pDPB       : Pointer to Device Setup Block                                             +*/
/*+                                                                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: This function is called at time of DEV_SETUP                              +*/
/*+                 -> write defaults into HW RAM                                           +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTInitConsumerDevSetup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_UPPER_DSB_PTR_TYPE     pDSB)
{
    EDD_RSP                                 Response;
    LSA_UINT32                              j;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pDSB);
#endif

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTInitConsumerDevSetup(pDSB=0x%X)", pDSB);

    Response = EDD_STS_OK;
    
    if (pDDB->CRT.CfgParam.ConsumerMaxNumber > 0)
    {   
        // ***** set HW pointer in RBASE list to invalid / unused
        for ( j=0; j < pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmRBaseEntries; j++ )
        {
            EDDP_HAL_MEM16_WRITE(pDDB, &pDDB->CRT.Cons.pRBaseEntry[j].pHwRBaseApi->ACW_List_Ptr, pDDB->CRT.CfgParam.ConsumerRBaseInvalidIndicator);
        }
    }
        
    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTAllocLocalMemDDB                                    +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                                                                                         +*/
/*+  Result                :    EDD_RSP                                                     +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Alloc local memory of CRT management in DDB.                              +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTAllocLocalMemDDB(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP                                 Response;
    LSA_UINT16                              MaxProv, MaxProvAcw, MaxProvAcwDG, MaxProvTrog, MaxProvDataStatus, MaxProvGroup, MaxProvPackGroup;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    LSA_UINT16                              MaxCons, MaxConsDG, MaxConsContainer;
    LSA_UINT16                              HwType;

    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTAllocLocalMemDDB()");

    Response            = EDD_STS_OK;
    MaxProv             = pDDB->CRT.CfgParam.ProviderMaxNumber;
    MaxProvAcw          = pDDB->CRT.CfgParam.ProviderAcwMaxNumber;
    MaxProvAcwDG        = pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber;
    MaxProvPackGroup    = pDDB->CRT.CfgParam.ProviderPackGroupMaxNumber;
    MaxCons             = pDDB->CRT.CfgParam.ConsumerMaxNumber;
    MaxConsDG           = pDDB->CRT.CfgParam.ConsumerDGMaxNumber;
    MaxConsContainer    = pDDB->CRT.CfgParam.ConsumerContainerMaxNumber;
    
    HwType = EDDP_HALGetChipVersion(pDDB);
    EDDP_ASSERT( (EDDP_HAL_HW_HERA == HwType) | (EDDP_HAL_HW_ERTEC200P == HwType) );

    /* -------------------------------------------------------------------------------- */
    /* DDB specific AllocLocalMem - Provider                                            */
    /* -------------------------------------------------------------------------------- */
    if (MaxProv > 0)
    {
        pProvMngm = &pDDB->CRT.Prov;

        pProvMngm->pProvEntry            = (EDDP_CRT_PROVIDER_PTR_TYPE)            EDDP_GlbAllocLocalMem ((MaxProv * sizeof(EDDP_CRT_PROVIDER_TYPE)));
        pProvMngm->pAcwEntry             = (EDDP_CRT_PROVIDER_ACW_PTR_TYPE)        EDDP_GlbAllocLocalMem ((MaxProvAcw * sizeof(EDDP_CRT_PROVIDER_ACW_TYPE)));
        pProvMngm->pAcwDGEntry           = (EDDP_CRT_PROVIDER_ACW_DG_PTR_TYPE)     EDDP_GlbAllocLocalMem ((MaxProvAcwDG * sizeof(EDDP_CRT_PROVIDER_ACW_DG_TYPE)));
        pProvMngm->pProvPackGroupEntry   = (EDDP_CRT_PROVIDER_PACK_GROUP_PTR_TYPE) EDDP_GlbAllocLocalMem ((MaxProvPackGroup * sizeof(EDDP_CRT_PROVIDER_PACK_GROUP_TYPE)));

        // check Alloc successful ?
        if (   (LSA_HOST_PTR_ARE_EQUAL(pProvMngm->pProvEntry, LSA_NULL))
            || (LSA_HOST_PTR_ARE_EQUAL(pProvMngm->pAcwEntry, LSA_NULL))
            || (LSA_HOST_PTR_ARE_EQUAL(pProvMngm->pAcwDGEntry, LSA_NULL))
            || (LSA_HOST_PTR_ARE_EQUAL(pProvMngm->pProvPackGroupEntry, LSA_NULL))
           )
        {
            Response = EDD_STS_ERR_RESOURCE;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTAllocLocalMemDDB(): ERROR -> AllocLocalMem of DDB.CRT.Prov is not Ok: pProvEntry=0x%X, pAcwEntry=0x%X, pAcwDGEntry=0x%X, pProvPackGroupEntry=0x%X, Response=0x%X", 
                pProvMngm->pProvEntry, pProvMngm->pAcwEntry, pProvMngm->pAcwDGEntry, pProvMngm->pProvPackGroupEntry, Response);
        }
    }

    /* -------------------------------------------------------- */
    /* DDB specific AllocLocalMem - Consumer                    */
    /* -------------------------------------------------------- */
    if (Response == EDD_STS_OK)
    {
        if (MaxCons > 0)
        {
            pDDB->CRT.Cons.pAcwPtrArray        = (EDDP_CRT_CONS_ACW_INFO_PTR_TYPE*)     EDDP_GlbAllocLocalMem (((MaxCons+MaxConsContainer) * sizeof(EDDP_CRT_CONS_ACW_INFO_PTR_TYPE)));
            pDDB->CRT.Cons.pConsEntry          = (EDDP_CRT_CONSUMER_PTR_TYPE)           EDDP_GlbAllocLocalMem ((MaxCons * sizeof(EDDP_CRT_CONSUMER_TYPE)));
            pDDB->CRT.Cons.pConsContainerEntry = (EDDP_CRT_CONSUMER_CONTAINER_PTR_TYPE) EDDP_GlbAllocLocalMem ((MaxConsContainer * sizeof(EDDP_CRT_CONSUMER_CONTAINER_TYPE)));
            pDDB->CRT.Cons.pConsDgEntry        = (EDDP_CRT_CONSUMER_DG_PTR_TYPE)        EDDP_GlbAllocLocalMem ((MaxConsDG * sizeof(EDDP_CRT_CONSUMER_DG_TYPE)));

            // check Alloc successful ?
            if ( LSA_HOST_PTR_ARE_EQUAL(pDDB->CRT.Cons.pAcwPtrArray, LSA_NULL)
                 || LSA_HOST_PTR_ARE_EQUAL(pDDB->CRT.Cons.pConsEntry, LSA_NULL)
                 || LSA_HOST_PTR_ARE_EQUAL(pDDB->CRT.Cons.pConsContainerEntry, LSA_NULL)
                 || LSA_HOST_PTR_ARE_EQUAL(pDDB->CRT.Cons.pConsDgEntry, LSA_NULL)
               )
            {
                Response = EDD_STS_ERR_RESOURCE;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTAllocLocalMemDDB(): ERROR -> AllocLocalMem of DDB.CRT.Cons is not Ok: pAcwPtrArray=0x%X, pConsEntry=0x%X, pConsContainerEntry=0x%X, pConsDgEntry=0x%X, Response=0x%X", 
                    pDDB->CRT.Cons.pAcwPtrArray, pDDB->CRT.Cons.pConsEntry, pDDB->CRT.Cons.pConsContainerEntry, pDDB->CRT.Cons.pConsDgEntry, Response);
            }
        }
    }

    /* -------------------------------------------------------------------------------- */
    /* DDB specific AllocLocalMem                                                       */
    /* -------------------------------------------------------------------------------- */
    if (Response == EDD_STS_OK)
    {
        /* -------------------------------------------------------- */
        /* Provider                                                 */
        /* -------------------------------------------------------- */
        if (MaxProv > 0)
        {
            pProvMngm           = &pDDB->CRT.Prov;
            MaxProvTrog         = pDDB->CRT.CfgParam.ProviderTrogMaxNumber;
            MaxProvDataStatus   = pDDB->CRT.CfgParam.ProviderDataStatusMaxNumber;
            MaxProvGroup        = pDDB->CRT.CfgParam.ProviderGroupMaxNumber;

            pProvMngm->pTrogEntry        = (EDDP_CRT_PROVIDER_TROG_PTR_TYPE)  EDDP_GlbAllocLocalMem ((MaxProvTrog * sizeof(EDDP_CRT_PROVIDER_TROG_TYPE)));
            pProvMngm->pProvGroupEntry   = (EDDP_CRT_PROVIDER_GROUP_PTR_TYPE) EDDP_GlbAllocLocalMem ((MaxProvGroup * sizeof(EDDP_CRT_PROVIDER_GROUP_TYPE)));
            pProvMngm->pTBaseEntry       = (EDDP_CRT_PROVIDER_TBASE_PTR_TYPE) EDDP_GlbAllocLocalMem ((EDDP_CRT_MAX_COUNT_TBASE * sizeof(EDDP_CRT_PROVIDER_TBASE_TYPE)));
            pProvMngm->pDSEntry          = (EDDP_CRT_PROVIDER_DS_PTR_TYPE)    EDDP_GlbAllocLocalMem ((MaxProvDataStatus * sizeof(EDDP_CRT_PROVIDER_DS_TYPE)));

            // check Alloc successful ?
            if (   (LSA_HOST_PTR_ARE_EQUAL(pProvMngm->pTrogEntry, LSA_NULL))
                || (LSA_HOST_PTR_ARE_EQUAL(pProvMngm->pProvGroupEntry, LSA_NULL))
                || (LSA_HOST_PTR_ARE_EQUAL(pProvMngm->pTBaseEntry, LSA_NULL))
                || (LSA_HOST_PTR_ARE_EQUAL(pProvMngm->pDSEntry, LSA_NULL))
                )
            {
                Response = EDD_STS_ERR_RESOURCE;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTAllocLocalMemDDB(IF[]): ERROR -> AllocLocalMem of DDB.CRT.Prov is not Ok: pTrogEntry=0x%X, pProvGroupEntry=0x%X, pTBaseEntry=0x%X, pDSEntry=0x%X, Response=0x%X", 
                    pProvMngm->pTrogEntry, pProvMngm->pProvGroupEntry, pProvMngm->pTBaseEntry, pProvMngm->pDSEntry, Response);
            }
        }

        /* -------------------------------------------------------- */
        /* Consumer                                                 */
        /* -------------------------------------------------------- */
        if (Response == EDD_STS_OK)
        {
            if (MaxCons > 0)
            {
                LSA_UINT32 RBaseEntries = pDDB->CRT.CfgHwAllocParamApi.MaxHWCpmRBaseEntries;
                pProvMngm = &pDDB->CRT.Prov;

                pDDB->CRT.Cons.pRBaseEntry = (EDDP_CRT_CONSUMER_RBASE_PTR_TYPE) EDDP_GlbAllocLocalMem ((RBaseEntries) * sizeof(EDDP_CRT_CONSUMER_RBASE_TYPE));
                pProvMngm->pASGroupEntry = (EDDP_CRT_PROVIDER_ASGROUP_PTR_TYPE) EDDP_GlbAllocLocalMem ((MaxCons * sizeof(EDDP_CRT_PROVIDER_ASGROUP_TYPE)));

                // check Alloc successful ?
                if (   (LSA_HOST_PTR_ARE_EQUAL(pDDB->CRT.Cons.pRBaseEntry, LSA_NULL))
                    || (LSA_HOST_PTR_ARE_EQUAL(pProvMngm->pASGroupEntry, LSA_NULL))
                    )
                {
                    Response = EDD_STS_ERR_RESOURCE;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTAllocLocalMemDDB(): ERROR -> AllocLocalMem of CRT.Cons is not Ok: pRBaseEntry=0x%X, pASGroupEntry=0x%X, Response=0x%X", 
                        pDDB->CRT.Cons.pRBaseEntry, pProvMngm->pASGroupEntry, Response);
                }
            }
        }
    }
    
    /* -------------------------------------------------------- */
    /* CPM and PPM internal command parameter elements          */
    /* -------------------------------------------------------- */
    if (Response == EDD_STS_OK)
    {
        EDDP_CRT_INTERNAL_CMD_PARAM_PTR_TYPE pCrtInternalCmdParam = &pDDB->CRT.CmdParam.CrtInternalCmdParam;
                        
        /* alloc internal command parameter elements */
        pCrtInternalCmdParam->pInternalCmdParam = (EDDP_CMD_PARAM_PTR_TYPE) EDDP_GlbAllocLocalMem ((pDDB->CMD.CrtCmdDepthAlloc) * sizeof(EDDP_CMD_PARAM_TYPE));
        
        if ( LSA_HOST_PTR_ARE_EQUAL(pCrtInternalCmdParam->pInternalCmdParam, LSA_NULL) )
        {
            Response = EDD_STS_ERR_RESOURCE;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTAllocLocalMemDDB(): ERROR -> AllocLocalMem of internal command parameter is not Ok: pCPMInternalCmdParam=0x%X, Response=0x%X", 
                              pCrtInternalCmdParam->pInternalCmdParam, Response);
        }
        else
        {
            EDDP_MEMSET_COMMON ( pCrtInternalCmdParam->pInternalCmdParam,  0, (pDDB->CMD.CrtCmdDepthAlloc) * sizeof(EDDP_CMD_PARAM_TYPE) );
            
            /* NOTE: set next free element to last element, with  */
            pCrtInternalCmdParam->pNextFreeCmdParam         = pCrtInternalCmdParam->pInternalCmdParam + (pCrtInternalCmdParam->CmdParamCnt - 1 );
            pCrtInternalCmdParam->pNextToBeReleasedCmdParam = pCrtInternalCmdParam->pInternalCmdParam;
            pCrtInternalCmdParam->CmdParamCnt               = pDDB->CMD.CrtCmdDepthAlloc;
            pCrtInternalCmdParam->UsageCnt                  = 0;
        }
    } 

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTAllocLocalMemHDB                                    +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE             pHDB                    +*/
/*+                                                                                         +*/
/*+  Result                :    EDD_RSP                                                     +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                                         +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Alloc local memory of CRT management in HDB.                              +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTAllocLocalMemHDB(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP                         Response;
    EDDP_LOCAL_HDB_CRT_PTR_TYPE     pHDB_CRT_PoolItem;

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:%2X] EDDP_CRTAllocLocalMemHDB()", pHDB->Handle);

    Response = EDD_STS_OK;

    #ifdef EDDP_CFG_LOCAL_MEM_STATIC
    LSA_UNUSED_ARG(pDDB);
    pHDB_CRT_PoolItem = &g_EDDP_HDB_CRT[pHDB->Handle];  // use static RAM
    #else
    pHDB_CRT_PoolItem = (EDDP_LOCAL_HDB_CRT_PTR_TYPE) EDDP_GlbAllocLocalMem (sizeof(EDDP_HDB_CRT_TYPE));    // alloc Mgmt-data in Heap

    // check Alloc successful ?
    if (LSA_HOST_PTR_ARE_EQUAL(pHDB_CRT_PoolItem, LSA_NULL))
    {
        Response = EDD_STS_ERR_RESOURCE;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_CRTAllocLocalMemHDB(): ERROR -> AllocLocalMem of pHDB->pCRT(0x%X) is not Ok! Response=0x%X", 
            pHDB->Handle, pHDB_CRT_PoolItem, Response);
    }
    #endif

    pHDB->pCRT = pHDB_CRT_PoolItem;

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTFreeLocalMemDDB                                     +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                                                                                         +*/
/*+  Result                :    EDD_RSP                                                     +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Free local memory of CRT management in DDB.                               +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTFreeLocalMemDDB(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP                                 Response;
    LSA_UINT16                              FreeStatus;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;

    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTFreeLocalMemDDB()");

    Response    = EDD_STS_OK;
    FreeStatus  = 0;

    // to prevent compiler warning
    LSA_UNUSED_ARG(FreeStatus);

    /* -------------------------------------------------------------------------------- */
    /* DDB specific FreeLocalMem - Provider                                             */
    /* -------------------------------------------------------------------------------- */
    pProvMngm = &pDDB->CRT.Prov;
    /* -------------------------------------------------------- */
    if (LSA_HOST_PTR_ARE_EQUAL(pProvMngm->pProvEntry, LSA_NULL))
    {
        // the pointer is already deleted !
    }
    else
    {
        FreeStatus = EDDP_GlbFreeLocalMem (pProvMngm->pProvEntry);
        pProvMngm->pProvEntry = LSA_NULL;
    }
    /* -------------------------------------------------------- */
    if (LSA_HOST_PTR_ARE_EQUAL(pProvMngm->pAcwEntry, LSA_NULL))
    {
        // the pointer is already deleted !
    }
    else
    {
        FreeStatus = EDDP_GlbFreeLocalMem (pProvMngm->pAcwEntry);
        pProvMngm->pAcwEntry = LSA_NULL;
    }
    /* -------------------------------------------------------- */
    if (LSA_HOST_PTR_ARE_EQUAL(pProvMngm->pAcwDGEntry, LSA_NULL))
    {
        // the pointer is already deleted !
    }
    else
    {
        FreeStatus = EDDP_GlbFreeLocalMem (pProvMngm->pAcwDGEntry);
        pProvMngm->pAcwDGEntry = LSA_NULL;
    }
    /* -------------------------------------------------------- */
    if (LSA_HOST_PTR_ARE_EQUAL(pProvMngm->pProvPackGroupEntry, LSA_NULL))
    {
        // the pointer is already deleted !
    }
    else
    {
        FreeStatus = EDDP_GlbFreeLocalMem (pProvMngm->pProvPackGroupEntry);
        pProvMngm->pProvPackGroupEntry = LSA_NULL;
    }
    /* -------------------------------------------------------- */
    // Api-Pointer im HAL only delete (without FreeLocalMem)
    pProvMngm->pHwBaseAcwApi     = LSA_NULL;
    pProvMngm->pHwBaseAcwDGApi   = LSA_NULL;
    /* -------------------------------------------------------- */

    /* -------------------------------------------------------------------------------- */
    /* DDB specific FreeLocalMem - Consumer                                             */
    /* -------------------------------------------------------------------------------- */
    /* -------------------------------------------------------- */
    if (LSA_HOST_PTR_ARE_EQUAL(pDDB->CRT.Cons.pAcwPtrArray, LSA_NULL))
    {
        // the pointer is already deleted !
    }
    else
    {
        FreeStatus = EDDP_GlbFreeLocalMem (pDDB->CRT.Cons.pAcwPtrArray);
        pDDB->CRT.Cons.pAcwPtrArray = LSA_NULL;
    }
    /* -------------------------------------------------------- */
    if (LSA_HOST_PTR_ARE_EQUAL(pDDB->CRT.Cons.pConsEntry, LSA_NULL))
    {
        // the pointer is already deleted !
    }
    else
    {
        FreeStatus = EDDP_GlbFreeLocalMem (pDDB->CRT.Cons.pConsEntry);
        pDDB->CRT.Cons.pConsEntry = LSA_NULL;
    }
    if (LSA_HOST_PTR_ARE_EQUAL(pDDB->CRT.Cons.pConsContainerEntry, LSA_NULL))
    {
        // the pointer is already deleted !
    }
    else
    {
        FreeStatus = EDDP_GlbFreeLocalMem (pDDB->CRT.Cons.pConsContainerEntry);
        pDDB->CRT.Cons.pConsContainerEntry = LSA_NULL;
    }
    if (LSA_HOST_PTR_ARE_EQUAL(pDDB->CRT.Cons.pConsDgEntry, LSA_NULL))
    {
        // the pointer is already deleted !
    }
    else
    {
        FreeStatus = EDDP_GlbFreeLocalMem (pDDB->CRT.Cons.pConsDgEntry);
        pDDB->CRT.Cons.pConsDgEntry = LSA_NULL;
    }
    // Api-Pointer im HAL only delete (without FreeLocalMem)
    pDDB->CRT.Cons.pHwBaseAcwApi      = LSA_NULL;
    pDDB->CRT.Cons.pHwBaseApduApi     = LSA_NULL;
    pDDB->CRT.Cons.pHwBaseAcwDgApi    = LSA_NULL;
    pDDB->CRT.Cons.pHwBaseDgStatusApi = LSA_NULL;

    /* -------------------------------------------------------------------------------- */
    /* DDB specific FreeLocalMem                                                        */
    /* -------------------------------------------------------------------------------- */

    /* -------------------------------------------------------- */
    /* Provider                                                 */
    /* -------------------------------------------------------- */
    pProvMngm = &pDDB->CRT.Prov;
    /* -------------------------------------------------------- */
    if (LSA_HOST_PTR_ARE_EQUAL(pProvMngm->pTrogEntry, LSA_NULL))
    {
        // the pointer is already deleted !
    }
    else
    {
        FreeStatus = EDDP_GlbFreeLocalMem (pProvMngm->pTrogEntry);
        pProvMngm->pTrogEntry = LSA_NULL;
    }
    /* -------------------------------------------------------- */
    if (LSA_HOST_PTR_ARE_EQUAL(pProvMngm->pProvGroupEntry, LSA_NULL))
    {
        // the pointer is already deleted !
    }
    else
    {
        FreeStatus = EDDP_GlbFreeLocalMem (pProvMngm->pProvGroupEntry);
        pProvMngm->pProvGroupEntry = LSA_NULL;
    }
    /* -------------------------------------------------------- */
    if (LSA_HOST_PTR_ARE_EQUAL(pProvMngm->pASGroupEntry, LSA_NULL))
    {
        // the pointer is already deleted !
    }
    else
    {
        FreeStatus = EDDP_GlbFreeLocalMem (pProvMngm->pASGroupEntry);
        pProvMngm->pASGroupEntry = LSA_NULL;
    }
    /* -------------------------------------------------------- */
    if (LSA_HOST_PTR_ARE_EQUAL(pProvMngm->pTBaseEntry, LSA_NULL))
    {
        // the pointer is already deleted !
    }
    else
    {
        FreeStatus = EDDP_GlbFreeLocalMem (pProvMngm->pTBaseEntry);
        pProvMngm->pTBaseEntry = LSA_NULL;
    }
    /* -------------------------------------------------------- */
    if (LSA_HOST_PTR_ARE_EQUAL(pProvMngm->pDSEntry, LSA_NULL))
    {
        // the pointer is already deleted !
    }
    else
    {
        FreeStatus = EDDP_GlbFreeLocalMem (pProvMngm->pDSEntry);
        pProvMngm->pDSEntry = LSA_NULL;
    }
    /* -------------------------------------------------------- */
    // Api-Pointer im HAL only delete (without FreeLocalMem)
    pProvMngm->pHwBaseTBaseApi           = LSA_NULL;
    pProvMngm->pHwBaseTrogApi            = LSA_NULL;
    #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
    pProvMngm->pHwBaseDSApiPnipR1        = LSA_NULL;
    #endif
    #ifdef EDDP_CFG_HW_PNIP_REV2_SUPPORT
    pProvMngm->pHwBaseDSApiPnipR2        = LSA_NULL;
    #endif
    /* -------------------------------------------------------- */

    /* -------------------------------------------------------- */
    /* Consumer                                                 */
    /* -------------------------------------------------------- */
    //pConsMngm = &pDDB->CRT.Cons;
    /* -------------------------------------------------------- */
    if (LSA_HOST_PTR_ARE_EQUAL(pDDB->CRT.Cons.pRBaseEntry, LSA_NULL))
    {
        // the pointer is already deleted !
    }
    else
    {
        FreeStatus = EDDP_GlbFreeLocalMem (pDDB->CRT.Cons.pRBaseEntry);
        pDDB->CRT.Cons.pRBaseEntry = LSA_NULL;
    }
    /* -------------------------------------------------------- */
    // Api-Pointer im HAL only delete (without FreeLocalMem)
    pDDB->CRT.Cons.pHwBaseRBaseApi    = LSA_NULL;
    /* -------------------------------------------------------- */

    /* -------------------------------------------------------- */
    /* Consumer SB                                              */
    /* -------------------------------------------------------- */
    pDDB->CRT.Cons.pHwBaseTSBApi      = LSA_NULL;
    pDDB->CRT.Cons.pHwBaseCSB0Api     = LSA_NULL;
    pDDB->CRT.Cons.pHwBaseCSB1Api     = LSA_NULL;

    /* -------------------------------------------------------- */
    /* CPM and PPM internal command parameter                   */
    /* -------------------------------------------------------- */
    if (LSA_HOST_PTR_ARE_EQUAL(pDDB->CRT.CmdParam.CrtInternalCmdParam.pInternalCmdParam, LSA_NULL))
    {
        // the pointer is already deleted !
    }
    else
    {
        EDDP_CRT_INTERNAL_CMD_PARAM_PTR_TYPE pCrtInternalCmdParam = &pDDB->CRT.CmdParam.CrtInternalCmdParam;
        
        FreeStatus = EDDP_GlbFreeLocalMem (pCrtInternalCmdParam->pInternalCmdParam);
        pCrtInternalCmdParam->pInternalCmdParam         = LSA_NULL;
        pCrtInternalCmdParam->pNextFreeCmdParam         = LSA_NULL;
        pCrtInternalCmdParam->pNextToBeReleasedCmdParam = LSA_NULL;
        pCrtInternalCmdParam->CmdParamCnt               = 0;
        pCrtInternalCmdParam->UsageCnt                  = 0;
    }
    
    return (Response);
}




/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTFreeLocalMemHDB                                     +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE             pHDB                    +*/
/*+                                                                                         +*/
/*+  Result                :    EDD_RSP                                                     +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                                         +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                                         +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Free local memory of CRT management in HDB.                               +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTFreeLocalMemHDB(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP         Response;
    LSA_UINT16      FreeStatus;

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:%2X] EDDP_CRTFreeLocalMemHDB()", pHDB->Handle);

    Response    = EDD_STS_OK;
    FreeStatus  = 0;

    // to prevent compiler warning
    LSA_UNUSED_ARG(pDDB);
    LSA_UNUSED_ARG(FreeStatus);

    #ifdef EDDP_CFG_LOCAL_MEM_STATIC
    FreeStatus = 0; // for suppressing of warning
    #else
    if (LSA_HOST_PTR_ARE_EQUAL(pHDB->pCRT, LSA_NULL))
    {
        // the pointer is already deleted !
    }
    else
    {
        FreeStatus = EDDP_GlbFreeLocalMem (pHDB->pCRT);
    }
    #endif

    pHDB->pCRT = LSA_NULL;

    return (Response);
}




/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTInitPnipRegDevSetup                             +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB            +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE                 pDSB            +*/
/*+                                                                                     +*/
/*+  Result                :    EDD_RSP                                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                                     +*/
/*+  pDSB       : Pointer to DeviceSetupBlock                                           +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: Init PNIP-Register for CRT.                                           +*/
/*+               It is assumed that alle PNIP-Memory is init with 0 at startup, so     +*/
/*+               we dont need to clear PNIP-memory!                                    +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTInitPnipRegDevSetup(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DSB_PTR_TYPE     pDSB)
{
    EDD_RSP                                 Response;
    LSA_UINT32                              RegValueTemp;
    LSA_UINT32                              PnipAdr;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pDSB);
#endif

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTInitPnipRegDevSetup(pDSB=0x%X)", pDSB);

    Response = EDD_STS_OK;

    /* ---------------------------------------------------------------- */
    /* DDB specific PNIP-Register                                       */
    /* ---------------------------------------------------------------- */
    // init PNIP_REG_LOCAL_SPORT_ETYPE_0
    EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_LOCAL_SPORT_ETYPE_0, (EDDP_CRT_ETHER_TYPE << PNIP_REG_LOCAL_SPORT_ETYPE_0_SHFT_LOCAL_SPORT_ETYPE));

    // init PNIP_REG_AUTOSTOP_BASETIME
    EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_AUTOSTOP_BASETIME, EDDP_CRT_REG_AUTOSTOP_BASETIME_INIT);

    // init PNIP_REG_LOCAL_IP_HEADER.IP_TTL
    RegValueTemp = EDDP_HAL_REG32_READ (pDDB, PNIP_REG_LOCAL_IP_HEADER);
    EDDP_SET_BIT_VALUE32 (RegValueTemp, 
                          EDDP_CRT_IP_TTL_VALUE, 
                          PNIP_REG_LOCAL_IP_HEADER__MSK_IP_TTL, 
                          PNIP_REG_LOCAL_IP_HEADER_SHFT_IP_TTL)
    EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_LOCAL_IP_HEADER, RegValueTemp);

    /* Init PNIP_F0_REG_PPM_FSO_LEADTIME...                                      */
    /* ...and setup the API-Types used for CPM receiption without API-HW-Filter. */
    /* We dont use the HW-Filter with CPM RT-Frames! We queue all RT             */
    /* Frames within Queue14/15 to local API.                                    */
    /* Queue 14 is used for Store&Forward, Queue 15 for CutThrough.              */
    RegValueTemp = EDDP_CRTGetFSOLeadtimeOrFetchtimeDelta(pDDB);
    switch (EDDP_HALGetPnipRevision(pDDB))
    {
        #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
        case EDDP_HAL_HW_PNIP_REV1:
        {
            EDDP_HAL_REG32_WRITE (pDDB, PNIP_R2F0_REG_PPM_FSO_LEADTIME, RegValueTemp);
            
            RegValueTemp = (PNIP_R2_REG_QUEUETYPE_SELECT__MSK_APITYPE_VALID14 | EDDP_CRT_API_TYPE_RT_SF << PNIP_R2_REG_QUEUETYPE_SELECT_SHFT_APITYPE_QUEUE14 | 
                            PNIP_R2_REG_QUEUETYPE_SELECT__MSK_APITYPE_VALID15 | EDDP_CRT_API_TYPE_RT_CT << PNIP_R2_REG_QUEUETYPE_SELECT_SHFT_APITYPE_QUEUE15);
            EDDP_HAL_REG32_WRITE (pDDB, PNIP_R2_REG_QUEUETYPE_SELECT, RegValueTemp);
        }
        break;
        #endif

        #ifdef EDDP_CFG_HW_PNIP_REV2_SUPPORT
        case EDDP_HAL_HW_PNIP_REV2:
        {
            EDDP_HAL_REG32_WRITE (pDDB, PNIP_R2F0_REG_PPM_FSO_LEADTIME, RegValueTemp);

            RegValueTemp = (PNIP_R2_REG_QUEUETYPE_SELECT__MSK_APITYPE_VALID14 | EDDP_CRT_API_TYPE_RT_SF << PNIP_R2_REG_QUEUETYPE_SELECT_SHFT_APITYPE_QUEUE14 | 
                            PNIP_R2_REG_QUEUETYPE_SELECT__MSK_APITYPE_VALID15 | EDDP_CRT_API_TYPE_RT_CT << PNIP_R2_REG_QUEUETYPE_SELECT_SHFT_APITYPE_QUEUE15 | 
                            PNIP_R2_REG_QUEUETYPE_SELECT__MSK_Q15_SELECT);
            EDDP_HAL_REG32_WRITE (pDDB, PNIP_R2_REG_QUEUETYPE_SELECT, RegValueTemp);
        }
        break;
        #endif
        
        
        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTInitPnipRegDevSetup(): FATAL -> Version(%d) of PNIP is unknown", 
                EDDP_HALGetPnipRevision(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    }

    /* ---------------------------------------------------------------- */
    /* DDB specific PNIP-Register                                       */
    /* ---------------------------------------------------------------- */
    pProvMngm = &pDDB->CRT.Prov;

    /* ---------------------------------------------------- */
    /* PPM register                                         */
    /* ---------------------------------------------------- */
    // ***** PPM is off if ProviderMaxNumber=0 --> do not write PPM-registers to avoid sideffects
    if (pDDB->CRT.CfgParam.ProviderMaxNumber > 0)
    {
        // init PNIP_REG_ACW_TBASEPTR_IFA
        RegValueTemp = EDDP_HAL_H2P_ADDR(pDDB, pProvMngm->pHwBaseTBaseApi);
        EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_ACW_TBASEPTR_IFA, (RegValueTemp << PNIP_REG_ACW_TBASEPTR_IFA_SHFT_ACW_TBASEPTR));

        // init PNIP_REG_ACW_TROGPTR_IFA
        RegValueTemp = EDDP_HAL_H2P_ADDR(pDDB, pProvMngm->pHwBaseTrogApi);
        EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_ACW_TROGPTR_IFA, (RegValueTemp << PNIP_REG_ACW_TROGPTR_IFA_SHFT_ACW_TROGPTR));

        // init PNIP_REG_APPLDATASTATUS_IFA_0-7
        EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_APPLDATASTATUS_IFA_0, (EDD_CSRT_DSTAT_GLOBAL_INIT_VALUE    << PNIP_REG_APPLDATASTATUS_IFA_0_SHFT_APPLDATASTATUS));
        EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_APPLDATASTATUS_IFA_1, (EDD_CSRT_DSTAT_GLOBAL_INIT_VALUE    << PNIP_REG_APPLDATASTATUS_IFA_1_SHFT_APPLDATASTATUS));
        EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_APPLDATASTATUS_IFA_2, (EDD_CSRT_DSTAT_GLOBAL_INIT_VALUE    << PNIP_REG_APPLDATASTATUS_IFA_2_SHFT_APPLDATASTATUS));
        EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_APPLDATASTATUS_IFA_3, (EDD_CSRT_DSTAT_GLOBAL_INIT_VALUE    << PNIP_REG_APPLDATASTATUS_IFA_3_SHFT_APPLDATASTATUS));
        EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_APPLDATASTATUS_IFA_4, (EDD_CSRT_DSTAT_GLOBAL_INIT_VALUE    << PNIP_REG_APPLDATASTATUS_IFA_4_SHFT_APPLDATASTATUS));
        EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_APPLDATASTATUS_IFA_5, (EDD_CSRT_DSTAT_GLOBAL_INIT_VALUE    << PNIP_REG_APPLDATASTATUS_IFA_5_SHFT_APPLDATASTATUS));
        EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_APPLDATASTATUS_IFA_6, (EDD_CSRT_DSTAT_GLOBAL_INIT_VALUE    << PNIP_REG_APPLDATASTATUS_IFA_6_SHFT_APPLDATASTATUS));
        EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_APPLDATASTATUS_IFA_7, (EDDP_CRT_REG_APPLDATASTATUS_7_INIT  << PNIP_REG_APPLDATASTATUS_IFA_7_SHFT_APPLDATASTATUS));

        // init PNIP_REG_DATASTATUS_MASK_IFA
        EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_DATASTATUS_MASK_IFA, (EDD_CSRT_DSTAT_WIN_MASK << PNIP_REG_DATASTATUS_MASK_IFA_SHFT_DATASTATUS_MASK));

        // init PNIP_REG_ACW_T_PO1K_IFA     : 1, 2, 4, 8, 16
        EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_ACW_T_PO1K_IFA,  (EDDP_CRT_ACW_T_PO1   << PNIP_REG_ACW_T_PO1K_IFA_SHFT_PHASE_OFFSET));
        EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_ACW_T_PO2K_IFA,  (EDDP_CRT_ACW_T_PO2   << PNIP_REG_ACW_T_PO2K_IFA_SHFT_PHASE_OFFSET));
        EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_ACW_T_PO4K_IFA,  (EDDP_CRT_ACW_T_PO4   << PNIP_REG_ACW_T_PO4K_IFA_SHFT_PHASE_OFFSET));
        EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_ACW_T_PO8K_IFA,  (EDDP_CRT_ACW_T_PO8   << PNIP_REG_ACW_T_PO8K_IFA_SHFT_PHASE_OFFSET));
        EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_ACW_T_PO16K_IFA, (EDDP_CRT_ACW_T_PO16  << PNIP_REG_ACW_T_PO16K_IFA_SHFT_PHASE_OFFSET));

        // init PNIP_REG_DG_DATASTATUS_IFA_0-7
        EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_DG_DATASTATUS_IFA_0, (EDDP_CRT_REG_DG_DATASTATUS_0_6_INIT  << PNIP_REG_DG_DATASTATUS_IFA_0_SHFT_DG_DATASTATUS));
        EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_DG_DATASTATUS_IFA_1, (EDDP_CRT_REG_DG_DATASTATUS_0_6_INIT  << PNIP_REG_DG_DATASTATUS_IFA_1_SHFT_DG_DATASTATUS));
        EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_DG_DATASTATUS_IFA_2, (EDDP_CRT_REG_DG_DATASTATUS_0_6_INIT  << PNIP_REG_DG_DATASTATUS_IFA_2_SHFT_DG_DATASTATUS));
        EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_DG_DATASTATUS_IFA_3, (EDDP_CRT_REG_DG_DATASTATUS_0_6_INIT  << PNIP_REG_DG_DATASTATUS_IFA_3_SHFT_DG_DATASTATUS));
        EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_DG_DATASTATUS_IFA_4, (EDDP_CRT_REG_DG_DATASTATUS_0_6_INIT  << PNIP_REG_DG_DATASTATUS_IFA_4_SHFT_DG_DATASTATUS));
        EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_DG_DATASTATUS_IFA_5, (EDDP_CRT_REG_DG_DATASTATUS_0_6_INIT  << PNIP_REG_DG_DATASTATUS_IFA_5_SHFT_DG_DATASTATUS));
        EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_DG_DATASTATUS_IFA_6, (EDDP_CRT_REG_DG_DATASTATUS_0_6_INIT  << PNIP_REG_DG_DATASTATUS_IFA_6_SHFT_DG_DATASTATUS));
        EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_DG_DATASTATUS_IFA_7, (EDDP_CRT_REG_DG_DATASTATUS_7_INIT    << PNIP_REG_DG_DATASTATUS_IFA_7_SHFT_DG_DATASTATUS));

        // init PNIP_REG_DG_DATASTATUS_MASK_IFA
        EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_DG_DATASTATUS_MASK_IFA, (EDDP_CRT_REG_DG_DATASTATUS_MASK_INIT << PNIP_REG_DG_DATASTATUS_MASK_IFA_SHFT_DG_DATASTATUS_MASK));

        /* -------------------------------------------------------- */
        /* the following registers are dependent of PNIP revision   */
        /*  - PNIP_REG_DATASTATUS_BASE_IFA                          */
        /*  - PNIP_REG_PPM_HOL_NOT_EMPTY_IFA                        */
        /*  - PNIP_R2_REG_PPM_RESSOURCE_LACK_IFA                    */
        /* -------------------------------------------------------- */
        switch (EDDP_HALGetPnipRevision(pDDB))
        {
            #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
            case EDDP_HAL_HW_PNIP_REV1:
            {
                // init PNIP_REG_DATASTATUS_BASE_IFA
                RegValueTemp = EDDP_HAL_H2P_ADDR(pDDB, pProvMngm->pHwBaseDSApiPnipR1);
                EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_DATASTATUS_BASE_IFA, (RegValueTemp << PNIP_REG_DATASTATUS_BASE_IFA_SHFT_DS_BASEPTR));

                // PNIP_REG_PPM_HOL_NOT_EMPTY_IFA: the check of "HOL_Not_Empty" is already active by PNIP init for Resource-Type(5): RTC1/2

                // there is no PNIP_R2_REG_PPM_RESSOURCE_LACK_IFA in revision 1
            }
            break;
            #endif

            #ifdef EDDP_CFG_HW_PNIP_REV2_SUPPORT
            case EDDP_HAL_HW_PNIP_REV2:
            {
                // init PNIP_REG_DATASTATUS_BASE_IFA
                RegValueTemp = EDDP_HAL_H2P_ADDR(pDDB, pProvMngm->pHwBaseDSApiPnipR2);
                EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_DATASTATUS_BASE_IFA, (RegValueTemp << PNIP_REG_DATASTATUS_BASE_IFA_SHFT_DS_BASEPTR));

                // init PNIP_REG_PPM_HOL_NOT_EMPTY_IFA: deactivate the check of "HOL_Not_Empty" for Resource-Type(5): RTC1/2
                EDDP_HAL_REG32_WRITE (pDDB, PNIP_REG_PPM_HOL_NOT_EMPTY_IFA, (0x1 << PNIP_REG_PPM_HOL_NOT_EMPTY_IFA_SHFT_MASK_RESSOURCETYPE_5));

                // init PNIP_R2_REG_PPM_RESSOURCE_LACK_IFA: all Resource-Types(15..0)=1
                /* PNIP_R2_REG_PPM_RESSOURCE_LACK_IFA was updated by PNIP_R3_REG_PPM_RESSOURCE_LACK_IFA*/
                EDDP_HAL_REG32_WRITE (pDDB, PNIP_R3_REG_PPM_RESSOURCE_LACK_IFA, EDDP_CRT_REG_PPM_RESSOURCE_LACK_INIT);
            }
            break;
            #endif
            
            default:
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTInitPnipRegDevSetup(): FATAL -> Version(%d) of PNIP is unknown", 
                    EDDP_HALGetPnipRevision(pDDB));
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }
            break;
        }
    }

    /* ---------------------------------------------------- */
    /* CPM register                                         */
    /* ---------------------------------------------------- */
    // ***** CPM is off if ConsumerMaxNumber=0 --> do not write CPM-registers to avoid sideffects
    if (pDDB->CRT.CfgParam.ConsumerMaxNumber > 0)
    {        
        // init PNIP_REG_TSB_IFA
        PnipAdr = ( EDDP_HAL_H2P_ADDR (pDDB, pDDB->CRT.Cons.pHwBaseTSBApi) & PNIP_REG_TSB_IFA__MSK_TIMER_SCOREBOARD_BASEPTR);
        PnipAdr |= (pDDB->CRT.CfgParam.ConsumerMaxNumber << PNIP_REG_TSB_IFA_SHFT_TSB_ENTRIES);
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_TSB_IFA, PnipAdr);

        // init PNIP_REG_CSB_0_IFA
        RegValueTemp = EDDP_HAL_H2P_ADDR(pDDB, pDDB->CRT.Cons.pHwBaseCSB0Api);
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_CSB_0_IFA, (RegValueTemp << PNIP_REG_CSB_0_IFA_SHFT_CONSUMER_SCOREBOARD_BASEPTR));

        // init PNIP_REG_CSB_1_IFA
        RegValueTemp = EDDP_HAL_H2P_ADDR(pDDB, pDDB->CRT.Cons.pHwBaseCSB1Api);
        EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_CSB_1_IFA, (RegValueTemp << PNIP_REG_CSB_1_IFA_SHFT_CONSUMER_SCOREBOARD_BASEPTR));

        /* Note. We assume TSB and CSB memory is cleared at startup */
        /*       So the TSB-Entrys are invalid!                     */

        // do revision dependent stuff
        switch (EDDP_HALGetPnipRevision(pDDB))
        {
            #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
            case EDDP_HAL_HW_PNIP_REV1:
            {
                // there is no PNIP_R2_REG_MRT_RR_RELOADVALUE_IFA in revision 1
                
                // init PNIP_REG_ACW_RBASEPTR_IFA
                RegValueTemp = EDDP_HAL_H2P_ADDR(pDDB, pDDB->CRT.Cons.pHwBaseRBaseApi);
                EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_ACW_RBASEPTR_IFA, (RegValueTemp << PNIP_REG_ACW_RBASEPTR_IFA_SHFT_ACW_RBASEPTR));

                // init PNIP_REG_ACW_RINDEXSIZE_IFA
                EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_ACW_RINDEXSIZE_IFA, (EDDP_CRT_MAX_RINDEX_SIZE << PNIP_REG_ACW_RINDEXSIZE_IFA_SHFT_ACW_RINDEXSIZE));
            }
            break;
            #endif

            #ifdef EDDP_CFG_HW_PNIP_REV2_SUPPORT
            case EDDP_HAL_HW_PNIP_REV2:
            {
                RegValueTemp = EDDP_CRTGetConsMrtRrReloadValue (pDDB);

                /* PNIP_R2_REG_MRT_RR_RELOADVALUE_IFA was updated by PNIP_R3_REG_MRT_RR_RELOADVALUE_IFA*/
                EDDP_HAL_REG32_WRITE(pDDB, PNIP_R3_REG_MRT_RR_RELOADVALUE_IFA, RegValueTemp);
                
                // init PNIP_REG_ACW_RBASEPTR_IFA
                RegValueTemp = EDDP_HAL_H2P_ADDR(pDDB, pDDB->CRT.Cons.pHwBaseRBaseApi);
                EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_ACW_RBASEPTR_IFA, (RegValueTemp << PNIP_REG_ACW_RBASEPTR_IFA_SHFT_ACW_RBASEPTR));

                // init PNIP_REG_ACW_RINDEXSIZE_IFA
                EDDP_HAL_REG32_WRITE(pDDB, PNIP_REG_ACW_RINDEXSIZE_IFA, (EDDP_CRT_MAX_RINDEX_SIZE << PNIP_REG_ACW_RINDEXSIZE_IFA_SHFT_ACW_RINDEXSIZE));
            }
            break;
            #endif
            
            
            default:
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTInitPnipRegDevSetup(): FATAL -> Version(%d)of PNIP is unknown", 
                    EDDP_HALGetPnipRevision(pDDB));
                EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
            }
            break;
        }
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTInitCfgParam                        +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB        +*/
/*+                        :    EDDP_UPPER_DPB_PTR_TYPE         pDPB        +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pDPB       : Pointer to DeviceParameterBlock (init-parameter from sys) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function inits configuration parameter of CRT.       +*/
/*+               Check parameter of system adaption, if valid copy these   +*/
/*+               parameters to CRT.                                        +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTInitCfgParam(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	EDDP_UPPER_DPB_PTR_TYPE     pDPB)
{
    EDD_RSP         Response;
    LSA_UINT16      CrtMaxProvider; // max. number of providers (depending on chip version)
    LSA_UINT16      CrtMaxConsumer; // max. number of consumers (depending on chip version)
    LSA_UINT16      CrtConsumerContainerMaxNumber; // max. number of consumer DFT containers (depending on chip version)
    LSA_UINT16      CrtMaxConsumerDG; // max. number of consumer DG's (depending on chip version)
    LSA_UINT16      CrtConsumerRBaseInvalidIndicator; // RBase invalid indicator, means RBase entry is not in use
    LSA_UINT16      CrtProviderPackGroupMaxNumber; // max. number of provider pack frames (depending on chip version)
    LSA_UINT16      CrtProviderAcwDGMaxNumber; // max. number of provider DG's (depending on chip verions)
    LSA_UINT16      CrtProviderAcwMaxNumber; // max. number of provider ACW's (depending on chip version)
    LSA_UINT16      CrtProviderTrogMaxNumber; // max. number of provider TROG's (depending on chip version)
    LSA_UINT16      CrtProviderDataStatusMaxNumber; // max. number of provider DataStatus elements (depending on chip version)

    Response       = EDD_STS_OK;
    CrtMaxProvider = 0;
    CrtMaxConsumer = 0;
    CrtConsumerContainerMaxNumber = 0;
    CrtMaxConsumerDG = 0;
    CrtProviderPackGroupMaxNumber = 0;
    CrtProviderAcwDGMaxNumber = 0;
    CrtProviderAcwMaxNumber = 0;
    CrtProviderTrogMaxNumber = 0;
    CrtProviderDataStatusMaxNumber = 0;
    CrtConsumerRBaseInvalidIndicator = 0;

    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTInitCfgParam(ProviderMaxNumberUser=%d, ConsumerMaxNumberUser=%d): Hardware=0x%X", 
        pDPB->CRT.ProviderMaxNumber, pDPB->CRT.ConsumerMaxNumber, pDDB->Hardware);


    /* ---------------------------------------------------------------- */
    /* The combination (IO_Configured = LSA_TRUE) and                   */
    /* (ConsumerCnt=0 and ProviderCnt=0) is not allowed and             */
    /* will return ERR_PARAM!                                           */
    /* ---------------------------------------------------------------- */
    if (0 == pDPB->CRT.ProviderMaxNumber &&
        0 == pDPB->CRT.ConsumerMaxNumber &&
        (EDD_SYS_IO_CONFIGURED_ON == pDPB->NRT.IO_Configured) )
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTInitCfgParam(): ERROR -> No Cons/Prov defined, but IO_Configured = LSA_TRUE");
    }

    /* ---------------------------------------------------------------- */
    /* DDB specific parameter for CRT                                   */
    /* ---------------------------------------------------------------- */
    switch(EDDP_HALGetChipVersion(pDDB))
    {
#ifdef EDDP_CFG_HW_ERTEC200P_SUPPORT
        case EDDP_HAL_HW_ERTEC200P:
        {
            LSA_UINT16 MaxMcProv;      // max. multicast providers
            
            CrtMaxConsumer                = EDDP_CRT_MAX_CONSUMER_ERTEC200P;
            CrtConsumerContainerMaxNumber = EDDP_CRT_MAX_CONSUMER_ACW_CONTAINER_ERTEC200P;
            CrtMaxConsumerDG              = pDPB->CRT.ConsumerMaxNumber;
            CrtConsumerRBaseInvalidIndicator = EDDP_CRT_INVALID_VALUE_16;
         
            CrtMaxProvider                 = EDDP_CRT_MAX_PROVIDER_ERTEC200P;
            CrtProviderPackGroupMaxNumber  = EDDP_CRT_MAX_PROVIDER_PACK_GROUP_ERTEC200P;
            CrtProviderAcwDGMaxNumber      = pDPB->CRT.ProviderMaxNumber;
            MaxMcProv                      = pDPB->CRT.ProviderMaxNumber / 2;
            CrtProviderAcwMaxNumber        = ( (4 * MaxMcProv) + (2 * (pDPB->CRT.ProviderMaxNumber - MaxMcProv)) );
            CrtProviderTrogMaxNumber       = ( (2 * MaxMcProv) + (2 * (pDPB->CRT.ProviderMaxNumber - MaxMcProv)) + 1);
            CrtProviderDataStatusMaxNumber = (pDPB->CRT.ProviderMaxNumber + CrtProviderAcwDGMaxNumber);
        }
        break;
#endif

        default:
        {
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTInitCfgParam(): FATAL -> Version(%d) of chip is unknown", 
                EDDP_HALGetChipVersion(pDDB));
            EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
        }
        break;
    } 

    if (pDPB->CRT.ProviderMaxNumber > CrtMaxProvider)
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTInitCfgParam(): ERROR -> ProviderMaxNumberUser(%d) is out of range! MaxProv=%d", 
            pDPB->CRT.ProviderMaxNumber, CrtMaxProvider);
    }
    else
    {
        pDDB->CRT.CfgParam.ProviderMaxNumber = pDPB->CRT.ProviderMaxNumber;

        if (pDPB->CRT.ProviderMaxNumber > 0)
        {            
            pDDB->CRT.CfgParam.ProviderAcwMaxNumber         = CrtProviderAcwMaxNumber;
            pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber       = CrtProviderAcwDGMaxNumber;
            pDDB->CRT.CfgParam.ProviderPackGroupMaxNumber   = CrtProviderPackGroupMaxNumber;
            pDDB->CRT.CfgParam.ProviderTrogMaxNumber        = CrtProviderTrogMaxNumber;
            pDDB->CRT.CfgParam.ProviderDataStatusMaxNumber  = CrtProviderDataStatusMaxNumber;
            
            /* ---------------------------------------- */
            /* ProviderGroupMaxNumber                   */
            /* ---------------------------------------- */
            //  user maximum                      eddp maximum
            if (EDD_CFG_CSRT_MAX_PROVIDER_GROUP > EDDP_CRT_MAX_PROVIDER_GROUP)
            {
                pDDB->CRT.CfgParam.ProviderGroupMaxNumber = EDDP_CRT_MAX_PROVIDER_GROUP;
            }
            else
            {
                pDDB->CRT.CfgParam.ProviderGroupMaxNumber = EDD_CFG_CSRT_MAX_PROVIDER_GROUP;
            }
        }
    }

    if (pDPB->CRT.ConsumerMaxNumber > CrtMaxConsumer)
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTInitCfgParam(): ERROR -> ConsumerMaxNumberUser(%d) is out of range! MaxCons=%d", 
            pDPB->CRT.ConsumerMaxNumber, CrtMaxConsumer);
    }
    else
    {
        pDDB->CRT.CfgParam.ConsumerMaxNumber    = pDPB->CRT.ConsumerMaxNumber;      // Nr of Consumer
        pDDB->CRT.CfgParam.ConsumerDGMaxNumber  = CrtMaxConsumerDG;                 // Nr of DGs
        pDDB->CRT.CfgParam.ConsumerRBaseInvalidIndicator = CrtConsumerRBaseInvalidIndicator; // identifier 

        if (pDPB->CRT.ConsumerMaxNumber > 0)
        {
            pDDB->CRT.CfgParam.ConsumerContainerMaxNumber = CrtConsumerContainerMaxNumber;
        }
    }

    /* ---------------------------------------------------------------- */
    /* DDB specific parameter for CRT                                   */
    /* ---------------------------------------------------------------- */
    if (Response == EDD_STS_OK)
    {
        /* ---------------------------------------- */
        /* ApplTimerMode                            */
        /* ---------------------------------------- */
        switch (pDPB->CRT.ApplTimerMode)
        {
            case EDDP_APPL_TIMER_CFG_MODE_NONE:
                // nothing to do
            break;

            case EDDP_APPL_TIMER_CFG_MODE_TRANSFER_END:
            {
                if ( !(EDDP_CRT_IS_APPL_TIMER_REDUCTION_RATIO_IN_VALID_RANGE(pDPB->CRT.ApplTimerReductionRatio)) )
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTInitCfgParam(IF[]): ERROR -> ApplTimerMode(%d) but ApplTimerReductionRatio(%d) is not in valid range!", 
                        pDPB->CRT.ApplTimerMode, pDPB->CRT.ApplTimerReductionRatio);
                }
                else
                {
                    if ( !(EDDP_CRT_IS_TRANSFEREND_CORRECTION_VALUE_IN_VALID_RANGE(pDPB->CRT.TransferEndCorrectionValue, pDPB->CRT.isTransferEndCorrectionPos)) )
                    {
                        Response = EDD_STS_ERR_PARAM;
                        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTInitCfgParam(IF[]): ERROR -> ApplTimerMode(%d) but TransferEndCorrectionValue(%d) is not in valid range!", 
                            pDPB->CRT.ApplTimerMode, pDPB->CRT.TransferEndCorrectionValue);
                    }
                }
            }
            break;

            default:
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTInitCfgParam(IF[]): ERROR -> ApplTimerMode(%d) is unknown!", 
                    pDPB->CRT.ApplTimerMode);
            }
            break;
        }
        // copy ApplTimerMode parameters
        if (Response == EDD_STS_OK)
        {
            pDDB->CRT.CfgParam.ApplTimerMode                = pDPB->CRT.ApplTimerMode;
            pDDB->CRT.CfgParam.ApplTimerReductionRatio      = pDPB->CRT.ApplTimerReductionRatio;
            pDDB->CRT.CfgParam.TransferEndCorrectionValue   = pDPB->CRT.TransferEndCorrectionValue;
            pDDB->CRT.CfgParam.isTransferEndCorrectionPos   = (EDDP_DPB_TRANSFERENDCORRECTIONVALUE_IS_POS == pDPB->CRT.isTransferEndCorrectionPos)?LSA_TRUE:LSA_FALSE;
        }

        /* ---------------------------------------- */
        /* Follow parameters only if                */
        /* (Provider > 0) or (Consumer > 0)         */
        /* ---------------------------------------- */
        if (   (Response == EDD_STS_OK)
            && ((pDPB->CRT.ProviderMaxNumber > 0) || (pDPB->CRT.ConsumerMaxNumber > 0))
            )
        {
            switch (EDDP_HALGetChipVersion(pDDB))
            {
#ifdef EDDP_CFG_HW_ERTEC200P_SUPPORT
                case EDDP_HAL_HW_ERTEC200P:
                {
                    /* ------------------------------------------------------------------------- */
                    /* pIOBaseAdrPerif : is checked here                                         */
                    /* pIOBaseAdrExtRam: will be checked with CutThrough-Consumer for ERTEC200P  */
                    /*                    for HERA the check is done in this function            */
                    /* ------------------------------------------------------------------------- */
                    if (LSA_HOST_PTR_ARE_EQUAL(pDPB->CRT.pIOBaseAdrPerif, LSA_NULL))
                    {
                        Response = EDD_STS_ERR_PARAM;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTInitCfgParam(IF[]): ERROR -> pIOBaseAdrPerif=NULL, Response=0x%X",
                            Response);
                    }
                    else
                    {
                        pDDB->CRT.pIOBaseAdrPerif  = pDPB->CRT.pIOBaseAdrPerif;
                        /*the pIOBaseAdrExtRam can not be checked because it's value depends on the value if the CR Number,
                        will be check at the latest by the activation of the consumers */
                        pDDB->CRT.pIOBaseAdrExtRam = pDPB->CRT.pIOBaseAdrExtRam;
                    }
                }
                break;
#endif
                default:
                {
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTInitCfgParam(): FATAL -> Version(%d) of chip is unknown", 
                        EDDP_HALGetChipVersion(pDDB));
                    EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                }
                break;
            }       
        }
        
        /* ---------------------------------------- */
        /* trace parameter                          */
        /* ---------------------------------------- */
        if (Response == EDD_STS_OK)
        {            
            // trace consumer parameters
            EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTInitCfgParam(): ConsumerMaxNumber=%d, ConsumerAcwMaxNumber=%d, ConsumerAcwDGMaxNumber=%d", 
                pDDB->CRT.CfgParam.ConsumerMaxNumber,                      
                (pDDB->CRT.CfgParam.ConsumerMaxNumber + pDDB->CRT.CfgParam.ConsumerContainerMaxNumber), 
                pDDB->CRT.CfgParam.ConsumerDGMaxNumber);
         
            // trace provider parameters
            EDDP_CRT_TRACE_07(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTInitCfgParam(): ProviderMaxNumber=%d, ProviderAcwMaxNumber=%d, ProviderAcwDGMaxNumber=%d, ProviderPackGroupMaxNumber=%d, ProviderGroupMaxNumber=%d, ProviderTrogMaxNumber=%d, ProviderDataStatusMaxNumber=%d", 
                pDDB->CRT.CfgParam.ProviderMaxNumber,
                pDDB->CRT.CfgParam.ProviderAcwMaxNumber, 
                pDDB->CRT.CfgParam.ProviderAcwDGMaxNumber,
                pDDB->CRT.CfgParam.ProviderPackGroupMaxNumber,
                pDDB->CRT.CfgParam.ProviderGroupMaxNumber, 
                pDDB->CRT.CfgParam.ProviderTrogMaxNumber, 
                pDDB->CRT.CfgParam.ProviderDataStatusMaxNumber);
            
            // trace CRT parameters
            EDDP_CRT_TRACE_08(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTInitCfgParam(): pIOBaseAdrPerif=0x%X, pIOBaseAdrExtRam=0x%X, pCPMBaseAddrFastMem=0x%X, pCPMBaseAddrSlowMem=0x%X, CPMBaseAddrFastMemPNIP=0x%X, CPMBaseAddrSlowMemPNIP=0x%X, CPM_NILOffsetFastMem=0x%X, CPM_NILOffsetSlowMem=0x%X",
                pDDB->CRT.pIOBaseAdrPerif,
                pDDB->CRT.pIOBaseAdrExtRam,
                pDDB->CRT.pCPMBaseAddrFastMem,
                pDDB->CRT.pCPMBaseAddrSlowMem,
                pDDB->CRT.CPMBaseAddrFastMemPNIP,
                pDDB->CRT.CPMBaseAddrSlowMemPNIP,
                pDDB->CRT.CPM_NILOffsetFastMem,
                pDDB->CRT.CPM_NILOffsetSlowMem);
            
            EDDP_CRT_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTInitCfgParam(): pPPMBaseAddrFastMem=0x%X, pPPMBaseAddrSlowMem=0x%X, PPMBaseAddrFastMemPNIP=0x%X, PPMBaseAddrSlowMemPNIP=0x%X, PPM_Lifetime64FastMem=0x%X, PPM_Lifetime64SlowMem=0x%X",
                pDDB->CRT.pPPMBaseAddrFastMem,
                pDDB->CRT.pPPMBaseAddrSlowMem,
                pDDB->CRT.PPMBaseAddrFastMemPNIP,
                pDDB->CRT.PPMBaseAddrSlowMemPNIP,
                pDDB->CRT.PPM_Lifetime64FastMem,
                pDDB->CRT.PPM_Lifetime64SlowMem);
            
            // trace CRT-ApplTimerMode parameters
            EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTInitCfgParam(): ApplTimerMode=%d, ApplTimerReductionRatio=%d, TransferEndCorrectionValue=%d, isTransferEndCorrectionPos=%d", 
                pDDB->CRT.CfgParam.ApplTimerMode, 
                pDDB->CRT.CfgParam.ApplTimerReductionRatio, 
                pDDB->CRT.CfgParam.TransferEndCorrectionValue, 
                pDDB->CRT.CfgParam.isTransferEndCorrectionPos);
        }
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CRTInitDevOpen                         +*/
/*+                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB        +*/
/*+                        :    EDDP_UPPER_DPB_PTR_TYPE         pDPB        +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                         +*/
/*+  pDPB       : Pointer to DeviceParameterBlock (init-parameter from sys) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Description: This function inits configuration parameter of CRT.       +*/
/*+               Check parameter of system adaption, if valid copy these   +*/
/*+               parameters to CRT.                                        +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTInitDevOpen(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DPB_PTR_TYPE     pDPB)
{
    EDD_COMMON_MEM_U8_PTR_TYPE cbl_address;
    LSA_UINT8* target_param_address;
    LSA_UINT16  index;
    EDDP_CMD_PARAM_PTR_TYPE pInternalCmdParam;
    LSA_UINT32 Size = 0;
    
    LSA_UNUSED_ARG(pDPB);
 
    /* --------------------------------------------------------------------------- */
    /* setup external command argument reference for CPM and PPM within ApiCtrlRam */
    /* --------------------------------------------------------------------------- */
    
    pInternalCmdParam    = pDDB->CRT.CmdParam.CrtInternalCmdParam.pInternalCmdParam;
    target_param_address = (LSA_UINT8*)EDDP_HALGetRAMSubAddress(pDDB, EDDP_HAL_RAMTYPE_API_CTRL, EDDP_HAL_RAMSUB_APICTRL_COMMON_CMD_ARGS, &Size);
    
    if( Size < (pDDB->CMD.CrtCmdDepthAlloc * sizeof(EDDP_HAL_API_RT_CTRL_COMMON_CMD_ARG_TYPE)) )
    {
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTInitDevOpen(): ERROR -> size of RAM too small, can not assign external command parameter within ApiCtrl-Ram!");
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }
    
    for(index = 0 ; index < pDDB->CMD.CrtCmdDepthAlloc ; index++)
    {
        pInternalCmdParam[index].param_union.Crt.pExternalCmdParam_ApiCtrlRam = (LSA_UINT32*)target_param_address;
        target_param_address += sizeof(EDDP_HAL_API_RT_CTRL_COMMON_CMD_ARG_TYPE);
    }
    
    /* ------------------------------------------------------------------ */
    /* setup command block reference for CPM and PPM within Cmd-Ram       */
    /* ------------------------------------------------------------------ */    
    
    cbl_address = (EDD_COMMON_MEM_U8_PTR_TYPE)EDDP_HALGetRAMSubAddress (pDDB, EDDP_HAL_RAMTYPE_CMD, EDDP_HAL_RAMSUB_CMD_CRT_COMMAND_HOST_ASYNC, 0);
    
    for(index = 0 ; index < pDDB->CMD.CrtCmdDepthAlloc ; index++)
    {
        pInternalCmdParam[index].param_union.Crt.pCBL = (EDDP_CMD_COMMAND_TYPE*)cbl_address;
        cbl_address += sizeof(EDDP_CMD_COMMAND_BLOCKS_CRT_HOST_ASYNC_TYPE);
    }
    
    return (EDD_STS_OK);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTAllocTimer                                          +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                                                                                         +*/
/*+  Result                :    EDD_RSP                                                     +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Alloc timer of CRT.                                                       +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTAllocTimer(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP             Response;
    //LSA_UINT32          i;

    Response = EDD_STS_OK;

    /* ---------------------------------------------------------------- */
    /* DDB specific timer                                               */
    /* ---------------------------------------------------------------- */
    // Inbound-LateErrLSATimer
    Response = EDDP_CoreTimerAlloc (pDDB, 
                                    LSA_TIMER_TYPE_CYCLIC, 
                                    LSA_TIME_BASE_100MS, 
                                    //LSA_NULL, 
                                    EDDP_TIMER_INBOUND_LATE_ERR, 
                                    LSA_NULL, 
                                    &pDDB->CRT.Inbound.LateErrLSATimer);

    /* ---------------------------------------------------------------- */
    /* DDB specific timer                                               */
    /* ---------------------------------------------------------------- */
    if (Response == EDD_STS_OK)
    {
        if (Response == EDD_STS_OK)
        {
            // PpmHolNotEmptyTimer
            Response = EDDP_CoreTimerAlloc (pDDB, 
                                            LSA_TIMER_TYPE_ONE_SHOT, 
                                            LSA_TIME_BASE_1S, 
                                            EDDP_TIMER_PPM_HOL_NOT_EMPTY, 
                                            LSA_NULL, 
                                            &pDDB->CRT.Prov.PpmHolNotEmptyTimer);

            if (Response == EDD_STS_OK)
            {
                // PpmNotFinishedTimer
                Response = EDDP_CoreTimerAlloc (pDDB, 
                                                LSA_TIMER_TYPE_ONE_SHOT, 
                                                LSA_TIME_BASE_1S, 
                                                EDDP_TIMER_PPM_NOT_FINISHED, 
                                                LSA_NULL, 
                                                &pDDB->CRT.Prov.PpmNotFinishedTimer);

                #ifdef EDDP_MESSAGE
                #ifdef EDDP_PRAGMA_MESSAGE
                #pragma EDDP_PRAGMA_MESSAGE("Workaround CRT.WrkarPackGrpDisable for PNIP-Revision(1) - AP01372025")
                #endif /* EDDP_PRAGMA_MESSAGE */
                #endif /* EDDP_MESSAGE */
                #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
                if (Response == EDD_STS_OK)
                {
                    // WrkarPackGrpDisable-LSATimer
                    if ( EDDP_HAL_HW_PNIP_REV1 == EDDP_HALGetPnipRevision(pDDB) )
                    {
                        Response = EDDP_CoreTimerAlloc (pDDB, 
                                                        LSA_TIMER_TYPE_CYCLIC, 
                                                        LSA_TIME_BASE_100MS, 
                                                        EDDP_TIMER_WRKAR_PACKGRP_DISABLE, 
                                                        LSA_NULL, 
                                                        &pDDB->CRT.WrkarPackGrpDisable.LSATimer);
                    }
                }
                #endif
            }
        }
    }

    if (Response != EDD_STS_OK)
    {
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTAllocTimer(): ERROR -> CoreTimerAlloc is not OK! Response=0x%X", 
            Response);
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTFreeTimer                                           +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                                                                                         +*/
/*+  Result                :    EDD_RSP                                                     +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: FreeAlloc timer of CRT.                                                   +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTFreeTimer(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP             Response;
    //LSA_UINT32          i;

    Response = EDD_STS_OK;

    /* ---------------------------------------------------------------- */
    /* DDB specific timer                                               */
    /* ---------------------------------------------------------------- */
    Response = EDDP_CoreTimerFree (pDDB->CRT.Inbound.LateErrLSATimer);

    /* ---------------------------------------------------------------- */
    /* DDB specific timer                                               */
    /* ---------------------------------------------------------------- */
    if (Response == EDD_STS_OK)
    {
        Response = EDDP_CoreTimerFree (pDDB->CRT.Prov.PpmHolNotEmptyTimer);

        if (Response == EDD_STS_OK)
        {
            Response = EDDP_CoreTimerFree (pDDB->CRT.Prov.PpmNotFinishedTimer);

            #ifdef EDDP_MESSAGE
            #ifdef EDDP_PRAGMA_MESSAGE
            #pragma EDDP_PRAGMA_MESSAGE("Workaround CRT.WrkarPackGrpDisable for PNIP-Revision(1) - AP01372025")
            #endif /* EDDP_PRAGMA_MESSAGE */
            #endif /* EDDP_MESSAGE */
            #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
            if (Response == EDD_STS_OK)
            {
                if ( EDDP_HAL_HW_PNIP_REV1 == EDDP_HALGetPnipRevision(pDDB) )
                {
                    Response = EDDP_CoreTimerFree (pDDB->CRT.WrkarPackGrpDisable.LSATimer);
                }
            }
            #endif
        }
    }

    if (Response != EDD_STS_OK)
    {
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTFreeTimer(): ERROR -> CoreTimerFree is not OK! Response=0x%X", 
            Response);
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                         +*/
/*+  Functionname          :    EDDP_CRTInitStateMachine                                    +*/
/*+                                                                                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE             pDDB                    +*/
/*+                                                                                         +*/
/*+  Result                :    EDD_RSP                                                     +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Input/Output:                                                                          +*/
/*+                                                                                         +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                                 +*/
/*+                                                                                         +*/
/*+-----------------------------------------------------------------------------------------+*/
/*+                                                                                         +*/
/*+  Description: Init CRT state machines.                                                  +*/
/*+                                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTInitStateMachine(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP             Response;
    LSA_UINT16 idx;
    
    Response = EDD_STS_OK;

    // common
    pDDB->CRT.PackActivateSm.State                  = EDDP_CRT_PACK_ACTIVATE_STATE_IDLE;
    pDDB->CRT.PackActivateSm.Cbf                    = 0;
    pDDB->CRT.PackPassivateSm.State                 = EDDP_CRT_PACK_PASSIVATE_STATE_IDLE;
    pDDB->CRT.PackPassivateSm.Cbf                   = 0;

    // provider
    pDDB->CRT.Prov.PackGroupActivateAllSm.State     = EDDP_CRT_PROVIDER_PACK_GROUP_ACTIVATE_ALL_STATE_IDLE;
    pDDB->CRT.Prov.PackGroupActivateAllSm.Cbf       = 0;
    pDDB->CRT.Prov.PackGroupPassivateAllSm.State    = EDDP_CRT_PROVIDER_PACK_GROUP_PASSIVATE_ALL_STATE_IDLE;
    pDDB->CRT.Prov.PackGroupPassivateAllSm.Cbf      = 0;
    
    for(idx = 0 ; idx < EDDP_CRT_PROVIDER_ACW_DG_STATE_MACHINE_DEPTH ; idx++)
    {
        pDDB->CRT.Prov.AcwDGActivateSm[idx].State       = EDDP_CRT_PROVIDER_ACW_DG_ACTIVATE_STATE_IDLE;
        pDDB->CRT.Prov.AcwDGActivateSm[idx].ProviderId  = EDDP_CRT_CHAIN_IDX_END;
        pDDB->CRT.Prov.AcwDGActivateSm[idx].Cbf         = 0;
        pDDB->CRT.Prov.AcwDGActivateSm[idx].InUse       = LSA_FALSE;
        pDDB->CRT.Prov.AcwDGActivateSm[idx].CmdCnt      = 0;
        pDDB->CRT.Prov.AcwDGActivateSm[idx].Index       = 0;
        pDDB->CRT.Prov.AcwDGActivateSm[idx].pRQB        = LSA_NULL;
        
        pDDB->CRT.Prov.AcwDGPassivateSm[idx].State      = EDDP_CRT_PROVIDER_ACW_DG_PASSIVATE_STATE_IDLE;
        pDDB->CRT.Prov.AcwDGPassivateSm[idx].ProviderId = EDDP_CRT_CHAIN_IDX_END;
        pDDB->CRT.Prov.AcwDGPassivateSm[idx].Cbf        = 0;
        pDDB->CRT.Prov.AcwDGPassivateSm[idx].InUse      = LSA_FALSE;
        pDDB->CRT.Prov.AcwDGPassivateSm[idx].CmdCnt     = 0;
        pDDB->CRT.Prov.AcwDGPassivateSm[idx].Index      = 0;
        pDDB->CRT.Prov.AcwDGPassivateSm[idx].pRQB       = LSA_NULL;
    }

    // consumer
    pDDB->CRT.Cons.CrtPackSm.Cbf                    = 0;

    // ApplTimerIfSetup
    pDDB->CRT.ApplTimerIfSetupSm.State              = EDDP_CRT_APPL_TIMER_STATE_IDLE;
    pDDB->CRT.ApplTimerIfSetupSm.Cbf                = 0;

    return (Response);
}
