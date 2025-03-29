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
/*  F i l e               &F: eddp_crt_check.c                          :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*  CRT (Cycle Real Time) for EDDP  -> check functions                       */
/*      - checking of parameter, data                                        */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who         What                                             */
#ifdef EDDP_MESSAGE
/*  17.11.08    Sergeev     Erstellung                                       */
#endif
/*****************************************************************************/



/*===========================================================================*/
/*                                 module-id                                 */
/*===========================================================================*/

#define LTRC_ACT_MODUL_ID   28
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_CRT_CHECK */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"           /* eddp headerfiles */
#include "eddp_int.h"           /* internal header  */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTCheckProviderAddParam                           +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB            +*/
/*+                             EDD_UPPER_CSRT_PROVIDER_ADD_PTR_TYPE    pRqbParam       +*/
/*                              LSA_UINT8                               ProviderAcwPhase+*/
/*+                                                                                     +*/
/*+  Result                :    EDD_RSP                                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                     (in)    +*/
/*+  pRqbParam          : Pointer to RQB parameter from user (UPPER-RQB)        (in)    +*/
/*+  ProviderAcwPhase   : Phase - Red, Orange, Green                            (in)    +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks the Provider-Add parameter.                      +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckProviderAddParam(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
	EDD_UPPER_CSRT_PROVIDER_ADD_PTR_TYPE    pRqbParam,
	LSA_UINT8                               ProviderAcwPhase)
{
    EDD_RSP                                 Response;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCheckProviderAddParam(pRqbParam=0x%X, ProviderAcwPhase=0x%X)", 
        pRqbParam, ProviderAcwPhase);

    Response     = EDD_STS_OK;
    pProvMngm    = &pDDB->CRT.Prov;

    /* ---------------------------------------------------- */
    /* parameter for all provider                           */
    /* ---------------------------------------------------- */
    if (pDDB->CRT.CfgParam.ProviderMaxNumber == 0)
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> ProviderMaxNumber=0!");
    }
    else
    {
        if (pProvMngm->ProviderFreeList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
        {
            Response = EDD_STS_ERR_RESOURCE;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> No more free Provider! ProviderUsedCnt=%d, AcwUsedCnt=%d", 
                pProvMngm->ProviderUsedCnt, pProvMngm->AcwUsedCnt);
        }
        else
        {
            if (   (!(pRqbParam->Properties & EDD_CSRT_PROV_PROP_PDU_FORMAT_DFP)) 
                && (pProvMngm->AcwFreeList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
               )
            {
                Response = EDD_STS_ERR_RESOURCE;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> No more free ACW! ProviderUsedCnt=%d, AcwUsedCnt=%d", 
                    pProvMngm->ProviderUsedCnt, pProvMngm->AcwUsedCnt);
            }
            else
            {
                if (ProviderAcwPhase == EDDP_CRT_PROV_ACW_PHASE_INVALID)
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> ProviderAcwPhase(0x%X) is invalid!", 
                        ProviderAcwPhase);
                }
                else
                {
                    if (pRqbParam->GroupID >= pDDB->CRT.CfgParam.ProviderGroupMaxNumber)
                    {
                        Response = EDD_STS_ERR_PARAM;
                        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> GroupID(%d) is out of range! GroupID-Max=%d", 
                            pRqbParam->GroupID, ((pDDB->CRT.CfgParam.ProviderGroupMaxNumber)-1));
                    }
                    else
                    {
                        if (   (pRqbParam->IOParams.CRNumber != EDD_CRNUMBER_UNDEFINED)
                            && (pRqbParam->IOParams.CRNumber > EDDP_CRT_MAX_CRNUMBER_PERIF_ERTEC200P)
                           )
                        {
                            Response = EDD_STS_ERR_PARAM;
                            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> CRNumber(%d) is out of range! CRNumberMax=%d", 
                                pRqbParam->IOParams.CRNumber, EDDP_CRT_MAX_CRNUMBER_PERIF_ERTEC200P);
                        }
                        else
                        {
                            if (   (pRqbParam->IOParams.AppGroup != EDD_APPGROUP_UNDEFINED)
                                && (pRqbParam->IOParams.AppGroup > EDDP_CRT_PROV_MAX_APPGROUP)
                               )
                            {
                                Response = EDD_STS_ERR_PARAM;
                                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> AppGroup(%d) is out of range! AppGroupMax=%d", 
                                    pRqbParam->IOParams.AppGroup, EDDP_CRT_PROV_MAX_APPGROUP);
                            }
                            else
                            {
                                if (pRqbParam->IOParams.DataOffset == EDD_DATAOFFSET_INVALID)
                                {
                                    Response = EDD_STS_ERR_PARAM;
                                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> DataOffset=INVALID");
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
#if defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)	
    if (   (Response == EDD_STS_OK)
        && (EDDP_HAL_HW_ERTEC200P == EDDP_HALGetChipVersion(pDDB))
       )
    {
        /* ---------------------------------------------------- */
        /* check IOParams.CRNumber                              */
        /* ---------------------------------------------------- */
        if (pRqbParam->IOParams.CRNumber != EDD_CRNUMBER_UNDEFINED)
        {
            if (!EDDP_CRT_IS_CRNUMBER_IN_PERIF_RANGE(pRqbParam->IOParams.CRNumber))
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> CRNumber(%d) is not a valid Perif-Nr",
                    pRqbParam->IOParams.CRNumber);
            }
            else
            {
                /* ---------------------------------------------------- */
                /* check IOParams.DataOffset                            */
                /* ---------------------------------------------------- */
                if (pRqbParam->IOParams.DataOffset != EDD_DATAOFFSET_UNDEFINED)
                {
                    // check 8 byte alignment (3 bits = 0)                 */
                    if (!(EDDP_GlbIsAligned(pDDB, pRqbParam->IOParams.DataOffset, EDDP_GLB_ALIGNMENT_8_BYTE)))
                    {
                        Response = EDD_STS_ERR_PARAM;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> DataOffset(0x%X) for PERIF is not 8 byte aligned!",
                            pRqbParam->IOParams.DataOffset);
                    }
                }
            }
        }
    }
#endif
    
    /* ---------------------------------------------------- */
    /* parameter dependent of RT/RToverUDP provider         */
    /*  -> not for DFP                                      */
    /*      - DataLenDFP will be checked below              */
    /*      - ReductionRatioDFP is from PRM-PDIRFrameData   */
    /* ---------------------------------------------------- */
    if ((Response == EDD_STS_OK) && (!(pRqbParam->Properties & EDD_CSRT_PROV_PROP_PDU_FORMAT_DFP)))
    {
        switch (pRqbParam->Properties & EDD_CSRT_PROV_PROP_RTCLASS_MASK)
        {
            case EDD_CSRT_PROV_PROP_RTCLASS_1:
            case EDD_CSRT_PROV_PROP_RTCLASS_2:
            case EDD_CSRT_PROV_PROP_RTCLASS_3:
            {
                // don't check the DataLenMin because of "Autopading"
                if (pRqbParam->DataLen > EDD_CSRT_DATALEN_MAX)
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> DataLen(%d) > DataLenMax(%d)", 
                        pRqbParam->DataLen, EDD_CSRT_DATALEN_MAX);
                }
                else
                {
                    // check range of CycleReductionRatio only if it is already valid
                    Response = EDDP_CRTCheckRangeOfCycleReductionRatio (pDDB, EDDP_CRT_RTCLASS_RT, pRqbParam->Properties, pRqbParam->CycleReductionRatio, LSA_TRUE);
                }
            }
            break;

            case EDD_CSRT_PROV_PROP_RTCLASS_UDP:
            {
                #ifdef EDDP_CRT_SUPPORT_RT_OVER_UDP_ON
                // don't check the DataLenMin because of "Autopading"
                if (pRqbParam->DataLen > EDD_CSRT_UDP_DATALEN_MAX)
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> DataLenUDP(%d) > DataLenUDPMax(%d)", 
                        pRqbParam->DataLen, EDD_CSRT_UDP_DATALEN_MAX);
                }
                else
                {
                    // check range of CycleReductionRatio only if it is already valid
                    Response = EDDP_CRTCheckRangeOfCycleReductionRatio (pDDB, EDDP_CRT_RTCLASS_UDP, pRqbParam->Properties, pRqbParam->CycleReductionRatio, LSA_TRUE);
                }
                #else
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> RToverUDP is not supported! Properties=0x%X", 
                    pRqbParam->Properties);
                #endif
            }
            break;

            default:
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> Unknown RTC-Properties(0x%X)", 
                    (pRqbParam->Properties & EDD_CSRT_PROV_PROP_RTCLASS_MASK));
            }
            break;
        }
    }

    /* ---------------------------------------------------- */
    /* parameter dependent of phase R/O/G                   */
    /* ---------------------------------------------------- */
    if (Response == EDD_STS_OK)
    {
        switch (ProviderAcwPhase)
        {
            case EDDP_CRT_PROV_ACW_PHASE_G:
            case EDDP_CRT_PROV_ACW_PHASE_O:
            {
                if (   (pRqbParam->CycleReductionRatio != EDD_CYCLE_REDUCTION_RATIO_UNDEFINED)
                    && (pRqbParam->CyclePhase != EDD_CYCLE_PHASE_UNDEFINED)
                    && (pRqbParam->CyclePhase > pRqbParam->CycleReductionRatio)
                   )
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> Ph(%d) > RR(%d)!", 
                        pRqbParam->CyclePhase, pRqbParam->CycleReductionRatio);
                }
            }
            break;

            case EDDP_CRT_PROV_ACW_PHASE_R:
            {
                if (pRqbParam->FrameID == EDD_FRAME_ID_UNDEFINED)
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_00(pDDB->TraceIdx,  LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> FrameID(UNDEFINED) is not permitted for RTC3-Provider!");
                }

                // check whether the RTC3-FrameID is already used by other provider
                // only for not DFP-Provider
                if (Response == EDD_STS_OK)
                {
                    if (!(pRqbParam->Properties & EDD_CSRT_PROV_PROP_PDU_FORMAT_DFP))
                    {
                        if (EDDP_CRTCheckProviderExistFrameID(pDDB, pRqbParam->FrameID))
                        {
                            Response = EDD_STS_ERR_PARAM;
                            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> new RTC3-FrameID(0x%X) is already used by other Provider!", 
                                pRqbParam->FrameID);
                        }
                    }
                }

                if (Response == EDD_STS_OK)
                {
                    // check CyclePhase only for LEGACY-Provider (for OneProviderMode CyclePhase is from PDIRData)
                    if (   (pRqbParam->Properties & EDD_CSRT_PROV_PROP_RTC3_MODE_LEGACY)
                        && (pRqbParam->CycleReductionRatio != EDD_CYCLE_REDUCTION_RATIO_UNDEFINED)
                        && (pRqbParam->CyclePhase != EDD_CYCLE_PHASE_UNDEFINED)
                        && (pRqbParam->CyclePhase > pRqbParam->CycleReductionRatio)
                       )
                    {
                        Response = EDD_STS_ERR_PARAM;
                        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> Ph(%d) > RR(%d)!", 
                            pRqbParam->CyclePhase, pRqbParam->CycleReductionRatio);
                    }
                }
            }
	        break;

            default:
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCheckProviderAddParam(): FATAL -> ROGPhase unknown! ROGPhase=0x%X", 
                    ProviderAcwPhase);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }

    /* ---------------------------------------------------- */
    /* parameter for RTC3-LEGACY-Provider                   */
    /* ---------------------------------------------------- */
    if ((Response == EDD_STS_OK) && (pRqbParam->Properties & EDD_CSRT_PROV_PROP_RTC3_MODE_LEGACY))
    {
        if (ProviderAcwPhase != EDDP_CRT_PROV_ACW_PHASE_R)
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> PROP_RTC3_MODE_LEGACY is only with RTC3 allowed! ProviderAcwPhase=0x%X", 
                ProviderAcwPhase);
        }
        else
        {
            // are there enough ACWs available for this new provider ?
            if ((pDDB->CRT.CfgParam.ProviderAcwMaxNumber - pProvMngm->AcwUsedCnt) < EDDP_CRT_PROV_ADD_NUMBER_OF_ACW_RTC3_LEGACY)
            {
                Response = EDD_STS_ERR_RESOURCE;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> No more free ACW! RTC3-LEGACY-Provider needs %d ACWs: ProviderUsedCnt=%d, AcwUsedCnt=%d", 
                    EDDP_CRT_PROV_ADD_NUMBER_OF_ACW_RTC3_LEGACY, pProvMngm->ProviderUsedCnt, pProvMngm->AcwUsedCnt);
            }
        }
    }

    /* ---------------------------------------------------- */
    /* parameter for redundant provider                     */
    /* ---------------------------------------------------- */
    if ((Response == EDD_STS_OK) && (EDDP_IS_IRT_REDUNDANT_FRAMEID(pRqbParam->FrameID)))
    {
        if (ProviderAcwPhase != EDDP_CRT_PROV_ACW_PHASE_R)
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> redundant FrameID(0x%X) is only with RTC3 allowed! ProviderAcwPhase=0x%X", 
                pRqbParam->FrameID, ProviderAcwPhase);
        }
        else
        {
            if (pRqbParam->Properties & EDD_CSRT_PROV_PROP_RTC3_MODE_LEGACY)
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> redundant FrameID(0x%X) and PROP_RTC3_MODE_LEGACY!", 
                    pRqbParam->FrameID);
            }
            else
            {
                if (pRqbParam->FrameID & 0x1)
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> redundant FrameID(0x%X) is not even number!", 
                        pRqbParam->FrameID);
                }
                else
                {
                    // are there enough ACWs available for this new provider ?
                    if ((pDDB->CRT.CfgParam.ProviderAcwMaxNumber - pProvMngm->AcwUsedCnt) < EDDP_CRT_PROV_ADD_NUMBER_OF_ACW_RTC3_MRPD)
                    {
                        Response = EDD_STS_ERR_RESOURCE;
                        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                        EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> No more free ACW! Redundant Provider needs %d ACWs: ProviderUsedCnt=%d, AcwUsedCnt=%d", 
                            EDDP_CRT_PROV_ADD_NUMBER_OF_ACW_RTC3_MRPD, pProvMngm->ProviderUsedCnt, pProvMngm->AcwUsedCnt);
                    }
                }
            }
        }
    }

    /* ---------------------------------------------------- */
    /* parameter for DFP-Provider                           */
    /* ---------------------------------------------------- */
    if ((Response == EDD_STS_OK) && (pRqbParam->Properties & EDD_CSRT_PROV_PROP_PDU_FORMAT_DFP))
    {
        if (ProviderAcwPhase != EDDP_CRT_PROV_ACW_PHASE_R)
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> DFP-Provider is only with RTC3 allowed! ProviderAcwPhase=0x%X", 
                ProviderAcwPhase);
        }
        else
        {
            if (pRqbParam->Properties & EDD_CSRT_PROV_PROP_RTC3_MODE_LEGACY)
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> DFP-Provider and PROP_RTC3_MODE_LEGACY!");
            }
            else
            {
                if (   (pRqbParam->IOParams.SFPosition != EDD_SFPOSITION_UNDEFINED) 
                    && (!(EDDP_CRT_IS_DFP_SFPOSITION_IN_VALID_RANGE(pRqbParam->IOParams.SFPosition)))
                   )
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> DFP-SFPosition(%d) is out of range! SFPositionMin(%d), SFPositionMax(%d)", 
                        pRqbParam->IOParams.SFPosition, EDDP_CRT_MIN_DFP_SFPOSITION, EDDP_CRT_MAX_DFP_SFPOSITION);
                }
                else
                {
                    if (pRqbParam->IOParams.SFOffset == EDD_SFOFFSET_INVALID)
                    {
                        Response = EDD_STS_ERR_PARAM;
                        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> SFOffset=INVALID");
                    }
                    else
                    {
                        // don't check the DataLenMin because of "Autopading"
                        if (pRqbParam->DataLen > EDD_CSRT_DFP_DATALEN_MAX)
                        {
                            Response = EDD_STS_ERR_PARAM;
                            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> DataLen(%d) > DataLenMaxDFP(%d)", 
                                pRqbParam->DataLen, EDD_CSRT_DFP_DATALEN_MAX);
                        }
                    }
                }
            }
        }
    }

    /* ---------------------------------------------------- */
    /* parameter for provider with a MultiCast-CR           */
    /* ---------------------------------------------------- */
    if ((Response == EDD_STS_OK) && (pRqbParam->Properties & EDD_CSRT_PROV_PROP_MODE_MC_CR))
    {
        if (pRqbParam->Properties & EDD_CSRT_PROV_PROP_RTC3_MODE_LEGACY)
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> MultiCast-Provider and PROP_RTC3_MODE_LEGACY!");
        }
        else
        {
            if (pRqbParam->Properties & EDD_CSRT_PROV_PROP_PDU_FORMAT_DFP)
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> MultiCast-Provider and PROP_PDU_FORMAT_DFP!");
            }
        }
    }

    /* ---------------------------------------------------- */
    /* parameter for System-Redundancy-Provider             */
    /* ---------------------------------------------------- */
    if ((Response == EDD_STS_OK) && (pRqbParam->Properties & EDD_CSRT_PROV_PROP_SYSRED))
    {
        if (ProviderAcwPhase == EDDP_CRT_PROV_ACW_PHASE_R)
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> System-Redundancy-Provider is not allowed with RTC3! ProviderAcwPhase=0x%X", 
                ProviderAcwPhase);
        }
        else
        {
            if (pRqbParam->Properties & EDD_CSRT_PROV_PROP_RTC3_MODE_LEGACY)
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> System-Redundancy-Provider and PROP_RTC3_MODE_LEGACY!");
            }
            else
            {
                if (pRqbParam->Properties & EDD_CSRT_PROV_PROP_PDU_FORMAT_DFP)
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> System-Redundancy-Provider and PROP_PDU_FORMAT_DFP!");
                }
            }
        }
    }

    /* ---------------------------------------------------- */
    /* Check "CRNumber"                                     */
    /* ---------------------------------------------------- */
#if defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)
    // in case of ERTEC200+ the CR Number should be unique
    if( (pDDB->Hardware == EDDP_HW_ERTEC200P) || (pDDB->Hardware == EDDP_HW_ERTEC200P_FPGA) )
    {
        if (Response == EDD_STS_OK)
        {
            if (pRqbParam->IOParams.CRNumber != EDD_CRNUMBER_UNDEFINED)
            {
                LSA_UINT16      ProvIdx;
        
                ProvIdx = pDDB->CRT.Prov.ProviderUsedList.BeginIdx;
                // loop through all used providers
                while (ProvIdx != EDDP_CRT_CHAIN_IDX_END)
                {
                    EDDP_ASSERT_FALSE(ProvIdx >= pDDB->CRT.CfgParam.ProviderMaxNumber);
        
                    if (pProvMngm->pProvEntry[ProvIdx].AddParam.CRNumber != EDD_CRNUMBER_UNDEFINED) 
                    {
                        if (pProvMngm->pProvEntry[ProvIdx].AddParam.CRNumber == pRqbParam->IOParams.CRNumber)
                        {
                            Response = EDD_STS_ERR_PARAM;
                            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> CRNumber(%d) already in use!", 
                                                  pRqbParam->IOParams.CRNumber);
                            break;
                        }
                    }
                    ProvIdx = pProvMngm->pProvEntry[ProvIdx].UsedFreeLink.Next;
                }
            }
        }
    }
#endif

    if (Response == EDD_STS_OK)
	{
#if defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)	
        if(pDDB->Hardware == EDDP_HW_ERTEC200P || pDDB->Hardware == EDDP_HW_ERTEC200P_FPGA)
        {
            /* ---------------------------------------------------- */
            /* Check "pIOBaseAdrPerif"                              */
            /* ---------------------------------------------------- */
            if (LSA_HOST_PTR_ARE_EQUAL(pDDB->CRT.pIOBaseAdrPerif, LSA_NULL))
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderAddParam(): ERROR -> pIOBaseAdrPerif=NULL!");
            }
        }
#endif
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTCheckProviderRemoveParam                        +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB            +*/
/*+                             EDD_UPPER_CSRT_PROVIDER_REMOVE_PTR_TYPE pRqbParam       +*/
/*+                                                                                     +*/
/*+  Result                :    EDD_RSP                                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         (in)    +*/
/*+  pRqbParam      : pointer to RQB parameter from user (UPPER-RQB)            (in)    +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks the Provider-Remove parameter.                   +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckProviderRemoveParam(
    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB,
	EDD_UPPER_CSRT_PROVIDER_REMOVE_PTR_TYPE     pRqbParam)
{
    EDD_RSP             Response;

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCheckProviderRemoveParam(pRqbParam=0x%X, ProviderID=%d)", 
        pRqbParam, pRqbParam->ProviderID);

    Response = EDD_STS_OK;

    if (pRqbParam->ProviderID >= pDDB->CRT.CfgParam.ProviderMaxNumber)
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderRemoveParam(): ERROR -> ProviderID(%d) is out of range! ProviderID-Max=%d", 
            pRqbParam->ProviderID, (pDDB->CRT.CfgParam.ProviderMaxNumber-1));
    }
    else
    {
        if (pDDB->CRT.Prov.pProvEntry[pRqbParam->ProviderID].State == EDDP_CRT_STATE_FREE)
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderRemoveParam(): ERROR -> Provider is not used!");
        }
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTCheckProviderControlParam                       +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB        +*/
/*+                             EDD_UPPER_CSRT_PROVIDER_CONTROL_PTR_TYPE    pRqbParam   +*/
/*+                                                                                     +*/
/*+  Result                :    EDD_RSP                                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         (in)    +*/
/*+  pRqbParam      : pointer to RQB parameter from user (UPPER-RQB)            (in)    +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks the Provider-Control parameter.                  +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckProviderControlParam(
    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB,
	EDD_UPPER_CSRT_PROVIDER_CONTROL_PTR_TYPE    pRqbParam)
{
    EDD_RSP                                     Response;
    LSA_UINT16                                  AcwIdx;
    LSA_UINT16                                  DataLenPDIRData;
    LSA_UINT16                                  CycleReductionRatioTemp;
    LSA_UINT16                                  CycleReductionRatioPDIRData, CyclePhasePDIRData;
    LSA_UINT16                                  DataLenMin;
    LSA_UINT8                                   SFPositionTemp;
    LSA_UINT32                                  SFOffsetTemp;
    LSA_UINT8                                   ProviderAcwPhase;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE             pProvMngm;
//  EDDP_CRT_PROVIDER_DDB_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE                  pProvider;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE              pAcw;
    EDDP_CRT_PROVIDER_ACW_DG_PTR_TYPE           pAcwDG;

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCheckProviderControlParam(pRqbParam=0x%X, ProviderID=%d)", 
        pRqbParam, pRqbParam->ProviderID);

    Response                    = EDD_STS_OK;
    pProvMngm                   = &pDDB->CRT.Prov;
    pProvider                   = LSA_NULL; // init to prevent compiler warning - it will be set below
    DataLenPDIRData             = 0;
    DataLenMin                  = 0;
    CycleReductionRatioPDIRData = EDD_CYCLE_REDUCTION_RATIO_UNDEFINED;
    CyclePhasePDIRData          = EDD_CYCLE_PHASE_UNDEFINED;

    /* ---------------------------------------------------- */
    /* general parameter                                    */
    /* ---------------------------------------------------- */
    if (pRqbParam->ProviderID >= pDDB->CRT.CfgParam.ProviderMaxNumber)
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> ProviderID(%d) is out of range! ProviderID-Max=%d", 
            pRqbParam->ProviderID, (pDDB->CRT.CfgParam.ProviderMaxNumber-1));
    }
    else
    {
        pProvider = &pProvMngm->pProvEntry[pRqbParam->ProviderID];

        if (pProvMngm->ProviderUsedList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
	        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> There is no used provider!");
        }
        else
        {
            if (pProvider->State == EDDP_CRT_STATE_FREE)
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
	            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> Provider is not used!");
            }
            else
            {
                if (pRqbParam->IOParams.DataOffset == EDD_DATAOFFSET_INVALID)
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> DataOffset=INVALID");
                }
                else
                {
                    // check DataOffset only if it is not yet valid in provider and provider is activated !
                    if (   (pProvider->AddParam.DataOffset == EDD_DATAOFFSET_UNDEFINED)
                        && ((pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE) || (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE_AUX))
                        && (pRqbParam->IOParams.DataOffset == EDD_DATAOFFSET_UNDEFINED)
                       )
                    {
                        Response = EDD_STS_ERR_PARAM;
                        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> Provider is activated but DataOffset is UNDEFINED!");
                    }
                    else
                    {
                        LSA_BOOL CRNumberValid = LSA_FALSE;
                        #if defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)	
                        if (EDDP_HAL_HW_ERTEC200P == EDDP_HALGetChipVersion(pDDB))
                        {
                            CRNumberValid = (pRqbParam->IOParams.CRNumber > EDDP_CRT_MAX_CRNUMBER_PERIF_ERTEC200P) ? LSA_FALSE : LSA_TRUE;
                        }
                        #endif

                        // check CRNumber only if it is not yet valid in provider and provider is activated !
                        if (   (pProvider->AddParam.CRNumber == EDD_CRNUMBER_UNDEFINED)
                            && ((pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE) || (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE_AUX))
                            && ((pRqbParam->IOParams.CRNumber == EDD_CRNUMBER_UNDEFINED) || !CRNumberValid)
                           )
                        {
                            Response = EDD_STS_ERR_PARAM;
                            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
	                        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> Provider is activated but CRNumber(%d) is UNDEFINED or out of range! CRNumberMax=%d", 
                                pRqbParam->IOParams.CRNumber, EDDP_CRT_MAX_CRNUMBER_PERIF_ERTEC200P);
                        }
                        else
                        {
                            if ((pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE) && (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_PASSIVATE))
                            {
                                Response = EDD_STS_ERR_PARAM;
                                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> MODE_ACTIVATE and MODE_PASSIVATE!");
                            }
                            else
                            {
                                if (   (pProvider->AuxAcwId == EDDP_CRT_CHAIN_IDX_END)
                                    && ((pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE_AUX) || (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_PASSIVATE_AUX))
                                   )
                                {
                                    Response = EDD_STS_ERR_PARAM;
                                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> MODE_ACTIVATE_AUX or MODE_PASSIVATE_AUX but this provider has no AUX-ACW! ROGPhase=0x%X", 
                                        pProvider->ROGPhase);
                                }
                                else
                                {
                                    if (   (pRqbParam->IOParams.AppGroup != EDD_APPGROUP_UNDEFINED)
                                        && (pRqbParam->IOParams.AppGroup > EDDP_CRT_PROV_MAX_APPGROUP)
                                       )
                                    {
                                        Response = EDD_STS_ERR_PARAM;
                                        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> AppGroup(%d) is out of range! AppGroupMax=%d", 
                                            pRqbParam->IOParams.AppGroup, EDDP_CRT_PROV_MAX_APPGROUP);
                                    }
                                    else
                                    {
                                        // check AppGroup only if it is not yet valid in provider and provider is activated !
                                        if (   (pProvider->AddParam.AppGroup == EDD_APPGROUP_UNDEFINED)
                                            && ((pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE) || (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE_AUX))
                                            && (pRqbParam->IOParams.AppGroup == EDD_APPGROUP_UNDEFINED)
                                           )
                                        {
                                            Response = EDD_STS_ERR_PARAM;
                                            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> Provider is activated but AppGroup(%d) is UNDEFINED!", 
                                                pRqbParam->IOParams.AppGroup);
                                        }
                                        else
                                        {
                                            // Mode(SET_UNDEFINED_PARAMS) is only alone permitted
                                            if (   (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_SET_UNDEFINED_PARAMS)
                                                && (pRqbParam->Mode & ~EDD_PROV_CONTROL_MODE_SET_UNDEFINED_PARAMS)
                                               )
                                            {
                                                Response = EDD_STS_ERR_PARAM;
                                                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> MODE_SET_UNDEFINED_PARAMS is only alone permitted! Mode=0x%X", 
                                                    pRqbParam->Mode);
                                            }
                                            else
                                            {
                                                // Mode(MODE_STATUS) is only for BIT_STATION_FAILURE permitted
                                                if (    (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_STATUS)
                                                    &&  ((pRqbParam->Mask & ~EDD_CSRT_DSTAT_LOCAL_MASK_ALL) || (pRqbParam->Status & ~EDD_CSRT_DSTAT_LOCAL_MASK_ALL))
                                                   )
                                                {
                                                    Response = EDD_STS_ERR_PARAM;
                                                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                                    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> MODE_STATUS - Mask(0x%X) or Status(0x%X) contain forbidden bits! Mode=0x%X", 
                                                        pRqbParam->Mask, pRqbParam->Status, pRqbParam->Mode);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

#if defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)	
    if (   (Response == EDD_STS_OK)
        && (EDDP_HAL_HW_ERTEC200P == EDDP_HALGetChipVersion(pDDB))
       )
    {
        /* ----------------------------------------------------------- */
        /* check IOParams.CRNumber (only if not yet valid in provider) */
        /* ----------------------------------------------------------- */
        if (pProvider->AddParam.CRNumber == EDD_CRNUMBER_UNDEFINED)
        {
            // check only if Mode is SET_UNDEFINED_PARAMS or ACTIVATE
            if (   (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_SET_UNDEFINED_PARAMS)
                || ((pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE) || (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE_AUX))
               )
            {
                if (!EDDP_CRT_IS_CRNUMBER_IN_PERIF_RANGE(pRqbParam->IOParams.CRNumber))
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> CRNumber(%d) is not valid Perif-Nr",
                        pRqbParam->IOParams.CRNumber);
                }
            }
        }
        
        /* ------------------------------------------------------------- */
        /* check IOParams.DataOffset (only if not yet valid in provider) */
        /* ------------------------------------------------------------- */
        if (   (Response == EDD_STS_OK)
            && (pProvider->AddParam.DataOffset == EDD_DATAOFFSET_UNDEFINED)
           )
        {
            if (   (EDDP_CRT_IS_CRNUMBER_IN_PERIF_RANGE(pRqbParam->IOParams.CRNumber))
                && (pRqbParam->IOParams.DataOffset != EDD_DATAOFFSET_UNDEFINED)
               )
            {
                // check 8 byte alignment (3 bits = 0)
                if (!(EDDP_GlbIsAligned(pDDB, pRqbParam->IOParams.DataOffset, EDDP_GLB_ALIGNMENT_8_BYTE))) // 3 bits = 0
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> DataOffset(0x%X) for PERIF is not 8 byte aligned!",
                        pRqbParam->IOParams.DataOffset);
                }
            }
        }
    }
#endif

    /* ---------------------------------------------------- */
    /* CyclePhase parameter check only if                   */
    /*  - Mode = SET_UNDEFINED_PARAMS                       */
    /*  - Mode = ACTIVATE                                   */
    /*  - provider (ACW) is not yet active                  */
    /* ---------------------------------------------------- */
    if (   (Response == EDD_STS_OK) 
        && (   (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_SET_UNDEFINED_PARAMS)
            || ((pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE_AUX) || (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE))
           )
       )
    {
        // CyclePhase parameter for all ACWs of this provider
        AcwIdx = pProvider->AcwList.BeginIdx;
        while (AcwIdx != EDDP_CRT_CHAIN_IDX_END)
        {
            EDDP_ASSERT_FALSE(AcwIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

            pAcw = &pProvMngm->pAcwEntry[AcwIdx];

            // only if this ACW is not yet active
            if (!(pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK))
            {
                /* -------------------------------------------------------- */
                /* CycleReductionRatio                                      */
                /* -------------------------------------------------------- */
                if (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_SET_UNDEFINED_PARAMS)
                {
                    // Mode = SET_UNDEFINED_PARAMS
                    // no special checks
                }
                else
                {
                    // Mode = ACTIVATE
                    if (   (pAcw->AddParam.CycleReductionRatio == EDD_CYCLE_REDUCTION_RATIO_UNDEFINED)
                        && (pRqbParam->CycleReductionRatio == EDD_CYCLE_REDUCTION_RATIO_UNDEFINED)
                       )
                    {
                        Response = EDD_STS_ERR_PARAM;
                        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> RR=UNDEFINED in ACW and in RQB");
                    }
                }

                // check range of CycleReductionRatio only if it is valid in RQB and not yet valid in ACW
                if (   (pRqbParam->CycleReductionRatio != EDD_CYCLE_REDUCTION_RATIO_UNDEFINED)
                    && (pAcw->AddParam.CycleReductionRatio == EDD_CYCLE_REDUCTION_RATIO_UNDEFINED)
                   )
                {
                    Response = EDDP_CRTCheckRangeOfCycleReductionRatio (pDDB, pProvider->RTClass, pProvider->AddParam.Properties, pRqbParam->CycleReductionRatio, LSA_FALSE);
                }

                /* -------------------------------------------------------- */
                /* CyclePhase                                               */
                /* check CyclePhase only for RTC1/2/3-LEGACY-Provider       */
                /* for OneProviderMode CyclePhase is from PDIRData          */
                /* -------------------------------------------------------- */
                if (Response == EDD_STS_OK)
                {
                    if (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_SET_UNDEFINED_PARAMS)
                    {
                        // Mode = SET_UNDEFINED_PARAMS
                        // no special checks
                    }
                    else
                    {
                        // Mode = ACTIVATE
                        if (   (pProvider->ROGPhase == EDDP_CRT_PROV_ACW_PHASE_G)
                            || (pProvider->ROGPhase == EDDP_CRT_PROV_ACW_PHASE_O)
                            || ((pProvider->ROGPhase == EDDP_CRT_PROV_ACW_PHASE_R) && (pProvider->AuxAcwId != EDDP_CRT_CHAIN_IDX_END))
                           )
                        {
                            if (   (pAcw->AddParam.CyclePhase == EDD_CYCLE_PHASE_UNDEFINED)
                                && (pRqbParam->CyclePhase == EDD_CYCLE_PHASE_UNDEFINED)
                               )
                            {
                                Response = EDD_STS_ERR_PARAM;
                                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> Ph=UNDEFINED in ACW and in RQB");
                            }
                        }
                    }
                }

                /* -------------------------------------------------------- */
                /* CyclePhaseSequence                                       */
                /* -------------------------------------------------------- */
                if (Response == EDD_STS_OK)
                {
                    if (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_SET_UNDEFINED_PARAMS)
                    {
                        // Mode = SET_UNDEFINED_PARAMS
                        // no special checks
                    }
                    else
                    {
                        // Mode = ACTIVATE
                        if (   (pAcw->AddParam.CyclePhaseSequence == EDD_CYCLE_PHASE_SEQUENCE_UNDEFINED)
                            && (pRqbParam->CyclePhaseSequence == EDD_CYCLE_PHASE_SEQUENCE_UNDEFINED)
                           )
                        {
                            Response = EDD_STS_ERR_PARAM;
                            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> PhSeq=UNDEFINED in ACW and in RQB");
                        }
                    }
                }

                /* -------------------------------------------------------- */
                /* check (CyclePhase > CycleReductionRatio)                 */
                /* only for RTC1/2/3-LEGACY-Provider                        */
                /* -------------------------------------------------------- */
                if (Response == EDD_STS_OK)
                {
                    if (   (pProvider->ROGPhase == EDDP_CRT_PROV_ACW_PHASE_G)
                        || (pProvider->ROGPhase == EDDP_CRT_PROV_ACW_PHASE_O)
                        || ((pProvider->ROGPhase == EDDP_CRT_PROV_ACW_PHASE_R) && (pProvider->AuxAcwId != EDDP_CRT_CHAIN_IDX_END))
                       )
                    {
                        // check only if one of these parameters is still UNDEFINED in ACW
                        if (   (pAcw->AddParam.CyclePhase == EDD_CYCLE_PHASE_UNDEFINED)
                            || (pAcw->AddParam.CycleReductionRatio == EDD_CYCLE_REDUCTION_RATIO_UNDEFINED)
                           )
                        {
                            // check only if both parameters are valid in RQB
                            if (   (pRqbParam->CyclePhase != EDD_CYCLE_PHASE_UNDEFINED)
                                && (pRqbParam->CycleReductionRatio != EDD_CYCLE_REDUCTION_RATIO_UNDEFINED)
                               )
                            {
                                if (pRqbParam->CyclePhase > pRqbParam->CycleReductionRatio)
                                {
                                    Response = EDD_STS_ERR_PARAM;
                                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> Ph(%d) > RR(%d) in RQB", 
                                        pRqbParam->CyclePhase, pRqbParam->CycleReductionRatio);
                                }
                            }

                            // check CyclePhase(ACW) against CycleReductionRatio(RQB)
                            // check if only CyclePhase is valid in ACW and CycleReductionRatio is UNDEFINED in ACW
                            if (   (Response == EDD_STS_OK)
                                && ((pAcw->AddParam.CyclePhase != EDD_CYCLE_PHASE_UNDEFINED) && (pAcw->AddParam.CycleReductionRatio == EDD_CYCLE_REDUCTION_RATIO_UNDEFINED))
                               )
                            {
                                // check only if CycleReductionRatio is valid in RQB
                                if (pRqbParam->CycleReductionRatio != EDD_CYCLE_REDUCTION_RATIO_UNDEFINED)
                                {
                                    // check CyclePhase(ACW) against CycleReductionRatio(RQB)
                                    if (pAcw->AddParam.CyclePhase > pRqbParam->CycleReductionRatio)
                                    {
                                        Response = EDD_STS_ERR_PARAM;
                                        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> Ph(%d) in ACW > RR(%d) in RQB", 
                                            pAcw->AddParam.CyclePhase, pRqbParam->CycleReductionRatio);
                                    }
                                }
                            }

                            // check CyclePhase(RQB) against CycleReductionRatio(ACW)
                            // check if only CyclePhase is UNDEFINED in ACW and CycleReductionRatio is valid in ACW
                            if (   (Response == EDD_STS_OK)
                                && ((pAcw->AddParam.CyclePhase == EDD_CYCLE_PHASE_UNDEFINED) && (pAcw->AddParam.CycleReductionRatio != EDD_CYCLE_REDUCTION_RATIO_UNDEFINED))
                               )
                            {
                                // check only if CyclePhase is valid in RQB
                                if (pRqbParam->CyclePhase != EDD_CYCLE_PHASE_UNDEFINED)
                                {
                                    // check CyclePhase(RQB) against CycleReductionRatio(ACW)
                                    if (pRqbParam->CyclePhase > pAcw->AddParam.CycleReductionRatio)
                                    {
                                        Response = EDD_STS_ERR_PARAM;
                                        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> Ph(%d) in RQB > RR(%d) in ACW", 
                                            pAcw->AddParam.CyclePhase, pRqbParam->CycleReductionRatio);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if (Response == EDD_STS_OK)
                AcwIdx = pProvMngm->pAcwEntry[AcwIdx].AcwProvLink.Next;      // next AcwIdx in this provider
            else
                AcwIdx = EDDP_CRT_CHAIN_IDX_END;                                // error - exit while loop
        }
    }

    /* ---------------------------------------------------- */
    /* Provider-AutoStop parameter                          */
    /* ---------------------------------------------------- */
    if ((Response == EDD_STS_OK) && (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_AUTOSTOP))
    {
        if (pDDB->CRT.CfgParam.ConsumerMaxNumber == 0)
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> MODE_AUTOSTOP but ConsumerMaxNumber=0!");
        }
        else
        {
            if (pRqbParam->AutoStopConsumerID != EDD_CONSUMER_ID_REMOVE_AS)
            {
                // this consumer must be available in CRT
                if (pRqbParam->AutoStopConsumerID >= pDDB->CRT.CfgParam.ConsumerMaxNumber)
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> MODE_AUTOSTOP but AutoStopConsumerID(%d) is out of range! ConsumerID-Max=%d", 
                        pRqbParam->AutoStopConsumerID, (pDDB->CRT.CfgParam.ConsumerMaxNumber-1));
                }
                else
                {
                    if (EDDP_CRTGetConsumerState(pDDB, pRqbParam->AutoStopConsumerID) == EDDP_CRT_STATE_FREE)
                    {
                        Response = EDD_STS_ERR_PARAM;
                        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
	                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> MODE_AUTOSTOP but AutoStopConsumerID(%d) is not used!", 
                            pRqbParam->AutoStopConsumerID);
                    }
                    else
                    {
                        if (   (pProvider->AutoStopConsumerID != EDD_CONSUMER_ID_REMOVE_AS)
                            && (pProvider->AutoStopConsumerID != pRqbParam->AutoStopConsumerID)
                           )
                        {
                            Response = EDD_STS_ERR_PARAM;
                            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
	                        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> MODE_AUTOSTOP - This provider has already a reference to AutoStopConsumerID(%d)! RQB-AutoStopConsumerID=%d", 
                                pProvider->AutoStopConsumerID, pRqbParam->AutoStopConsumerID);
                        }
                    }
                }
            }
        }
    }

    /* ---------------------------------------------------- */
    /* parameter for RTC3-LEGACY-Provider                   */
    /* ---------------------------------------------------- */
    if ((Response == EDD_STS_OK) && (pProvider->AuxAcwId != EDDP_CRT_CHAIN_IDX_END))
    {
        if ((pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE_AUX) && (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_PASSIVATE_AUX))
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> MODE_ACTIVATE_AUX and MODE_PASSIVATE_AUX!");
        }
        else
        {
            if ((pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE) && (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_PASSIVATE_AUX))
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> MODE_ACTIVATE and MODE_PASSIVATE_AUX!");
            }
            else
            {
                if ((pRqbParam->Mode & EDD_PROV_CONTROL_MODE_PASSIVATE) && (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE_AUX))
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> MODE_PASSIVATE and MODE_ACTIVATE_AUX!");
                }
                else
                {
                    pAcw = &pProvMngm->pAcwEntry[pProvider->AuxAcwId];   // AUX-ACW

                    if ((pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE) && (!(pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK)))
                    {
                        Response = EDD_STS_ERR_SEQUENCE;
                        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> MODE_ACTIVATE but the AUX-ACW is not yet active!");
                    }
                }
            }
        }

        /* -------------------------------- */
        /* DstMAC address only for AUX-ACW  */
        /* -------------------------------- */
        if (Response == EDD_STS_OK)
        {
            EDDP_ASSERT_FALSE(pProvider->AuxAcwId == EDDP_CRT_CHAIN_IDX_END);
            pAcw = &pProvMngm->pAcwEntry[pProvider->AuxAcwId];

            // check following parameters only if AUX-ACW is activated (MODE_ACTIVATE_AUX) - AUX-ACW is activated always first
            // and only if this ACW is not yet active
            if (   (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE_AUX)
                && (!(pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK))
               )
            {
                Response = EDDP_CRTProvAcwCheckAndSetDstMACFromProviderControlRQB (pDDB, pProvider->ProviderId, pRqbParam);
            }
        }
    }

    /* ---------------------------------------------------- */
    /* parameter dependent of phase R/O/G                   */
    /* ---------------------------------------------------- */
    if (Response == EDD_STS_OK)
    {
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
                    /* ------------------------ */
                    /* FrameID                  */
                    /* ------------------------ */
                    if ((pProvider->AddParam.FrameID == EDD_FRAME_ID_UNDEFINED) && (pRqbParam->FrameID == EDD_FRAME_ID_UNDEFINED))
                    {
                        Response = EDD_STS_ERR_PARAM;
                        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> FrameID=UNDEFINED in Provider and in RQB!");
                    }
                    else
                    {
                        // check further only if RQB.FrameID is valid and is different to Provider.FrameID
                        if ((pRqbParam->FrameID != EDD_FRAME_ID_UNDEFINED) && (pRqbParam->FrameID != pProvider->AddParam.FrameID))
                        {
                            // new FrameID is given - check the FrameID matches with Properties
                            ProviderAcwPhase = EDDP_CRTGetProviderAcwPhase (pDDB, pProvider->AddParam.Properties, pRqbParam->FrameID);

                            if (   (ProviderAcwPhase == EDDP_CRT_PROV_ACW_PHASE_INVALID)
                                || (ProviderAcwPhase == EDDP_CRT_PROV_ACW_PHASE_R)
                               )
                            {
                                Response = EDD_STS_ERR_PARAM;
                                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> FrameID(0x%X) does not match with ProvProperties(0x%X)! ProviderAcwPhase(0x%X)", 
                                    pRqbParam->FrameID, pProvider->AddParam.Properties, ProviderAcwPhase);
                            }
                            else
                            {
                                // store FrameID in Provider
                                pProvider->AddParam.FrameID = pRqbParam->FrameID;
                                // store FrameID in ACWs of this provider
                                AcwIdx = pProvider->AcwList.BeginIdx;
                                while (AcwIdx != EDDP_CRT_CHAIN_IDX_END)
                                {
                                    EDDP_ASSERT_FALSE(AcwIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);
                                    pAcw = &pProvMngm->pAcwEntry[AcwIdx];

                                    pAcw->AddParam.FrameID = pRqbParam->FrameID;

                                    // next AcwIdx in this provider
                                    AcwIdx = pProvMngm->pAcwEntry[AcwIdx].AcwProvLink.Next;
                                }
                            }
                        }
                    }

                    /* ------------------------ */
                    /* DstMAC address           */
                    /* ------------------------ */
                    if (Response == EDD_STS_OK)
                    {
                        // set the DstMAC from RQB to ACW
                        Response = EDDP_CRTProvAcwCheckAndSetDstMACFromProviderControlRQB (pDDB, pProvider->ProviderId, pRqbParam);
                    }

                    /* ------------------------ */
                    /* DstIP address            */
                    /* ------------------------ */
                    if ((Response == EDD_STS_OK) && (pProvider->RTClass == EDDP_CRT_RTCLASS_UDP))
                    {
                        // set the DstIP from RQB to provider
                        pProvider->AddParam.DstIP = pRqbParam->DstIP;
                    }
                }
            }
            break;

            case EDDP_CRT_PROV_ACW_PHASE_R:
            {
                if (pProvider->ProviderFlags & EDDP_CRT_PROV_FLAGS_IS_DFP)
                {
                    /* -------------------------------------------- */
                    /* DFP-Provider                                 */
                    /* activate/passivate only one ACW_DG           */
                    /* -------------------------------------------- */
                    // check following parameters only if (Mode=SET_UNDEFINED_PARAMS) or (Mode=ACTIVATE)
                    if (   (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_SET_UNDEFINED_PARAMS) 
                        || (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE)
                       )
                    {
                        /* -------------------------------------------- */
                        /* SFPosition                                   */
                        /* -------------------------------------------- */
                        // check only if it is not yet valid in provider
                        if ((Response == EDD_STS_OK) && (pProvider->DfpParam.SFPosition == EDD_SFPOSITION_UNDEFINED))
                        {
                            if (pRqbParam->IOParams.SFPosition == EDD_SFPOSITION_UNDEFINED)
                            {
                                Response = EDD_STS_ERR_PARAM;
                                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> for DFP-Provider is SFPosition=UNDEFINED!");
                            }
                            else
                            {
                                if (!(EDDP_CRT_IS_DFP_SFPOSITION_IN_VALID_RANGE(pRqbParam->IOParams.SFPosition)))
                                {
                                    Response = EDD_STS_ERR_PARAM;
                                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> for DFP-Provider is SFPosition(%d) out of range! SFPositionMin(%d), SFPositionMax(%d)", 
                                        pRqbParam->IOParams.SFPosition, EDDP_CRT_MIN_DFP_SFPOSITION, EDDP_CRT_MAX_DFP_SFPOSITION);
                                }
                            }
                        }

                        /* -------------------------------------------- */
                        /* SFOffset                                     */
                        /* -------------------------------------------- */
                        // check only if it is not yet valid in provider
                        if ((Response == EDD_STS_OK) && (pProvider->DfpParam.SFOffset == EDD_SFOFFSET_UNDEFINED))
                        {
                            if (pRqbParam->IOParams.SFOffset == EDD_SFOFFSET_UNDEFINED)
                            {
                                Response = EDD_STS_ERR_PARAM;
                                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> for DFP-Provider is SFOffset=UNDEFINED!");
                            }
                        }
                    }

                    // check following parameters only if ACW_DG is activated (MODE_ACTIVATE)
                    // and this DFP-Provider is not yet connected to a pack group (is still passive)
                    if (   (Response == EDD_STS_OK)
                        && (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE) 
                        && (pProvider->DfpParam.PackGrId == EDDP_CRT_CHAIN_IDX_END)
                       )
                    {
                        /* -------------------------------------------- */
                        /* DFP (PDIRSubframeData and PDIRData)          */
                        /* must be present for activate                 */
                        /* -------------------------------------------- */
                        if ( !(EDDP_PRMPDIRSubframeDataIsDfpPresent(pDDB, LSA_TRUE)) )
                        {
                            Response = EDD_STS_ERR_SEQUENCE;
                            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> activate DFP-Provider but DFP is not present in PRM!");
                        }

                        /* -------------------------------------------- */
                        /* is Provider.FrameID in PDIRData present?     */
                        /* -------------------------------------------- */
                        if (Response == EDD_STS_OK)
                        {
                            if ( !(EDDP_PRMPDIRDataIsFrameIDPresent(pDDB, pProvider->AddParam.FrameID, LSA_TRUE, &CycleReductionRatioPDIRData, &CyclePhasePDIRData, LSA_TRUE)) )
                            {
                                Response = EDD_STS_ERR_PARAM;
                                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> The DFP-Provider.FrameID(0x%X) is not found at all or not found with the expected role in PDIRData-Record!", 
                                    pProvider->AddParam.FrameID);
                            }
                        }

                        /* -------------------------------------------- */
                        /* PackGroup                                    */
                        /* -------------------------------------------- */
                        if (Response == EDD_STS_OK)
                        {
                            if (pProvMngm->PackGroupUsedCnt == 0)
                            {
                                Response = EDD_STS_ERR_SEQUENCE;
                                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> activate DFP-Provider but there is no pack groups in CRT management!");
                            }
                            else
                            {
                                // search the AcwDG with given "SFPosition" and connect this provider to pack group
                                if (pProvider->DfpParam.SFPosition == EDD_SFPOSITION_UNDEFINED)
                                    SFPositionTemp = pRqbParam->IOParams.SFPosition;
                                else
                                    SFPositionTemp = pProvider->DfpParam.SFPosition;

                                // only relevant for HERA: copy the DataOffset for check before connection to a pack group
                                // (it will be needed in the function EDDP_CRTProvDfpConnectToPackGroup() below)
                                if (pProvider->DfpParamCheck.DataOffset == EDD_DATAOFFSET_UNDEFINED)
                                    pProvider->DfpParamCheck.DataOffset = pRqbParam->IOParams.DataOffset;

                                // connect this provider to pack group
                                Response = EDDP_CRTProvDfpConnectToPackGroup (pDDB, pProvider->ProviderId, SFPositionTemp);

                                if (Response != EDD_STS_OK)
                                {
                                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> activate DFP-Provider FrameID(0x%X) but SFPosition(%d) is not found within PRM!", 
                                        pProvider->AddParam.FrameID, SFPositionTemp);
                                    // set SFPosition to UNDEFINED
                                    pProvider->DfpParam.SFPosition = EDD_SFPOSITION_UNDEFINED;
                                }
                            }
                        }

                        // check following parameters only if this provider is connected to pack group
                        if ((Response == EDD_STS_OK) && (pProvider->DfpParam.PackGrId != EDDP_CRT_CHAIN_IDX_END))
                        {
                            /* ---------------------------------------------------- */
                            /* SFOffset against PDIRSubframeData.SFOffset           */
                            /* ---------------------------------------------------- */
                            EDDP_ASSERT_FALSE(pProvider->DfpParam.AcwDGId == EDDP_CRT_CHAIN_IDX_END);
                            pAcwDG = &pProvMngm->pAcwDGEntry[pProvider->DfpParam.AcwDGId];

                            if (pProvider->DfpParam.SFOffset == EDD_SFOFFSET_UNDEFINED)
                                SFOffsetTemp = pRqbParam->IOParams.SFOffset;
                            else
                                SFOffsetTemp = pProvider->DfpParam.SFOffset;

                            // compare Provider-SFOffset with PRM-SFOffset
                            if (SFOffsetTemp != pAcwDG->AddParam.DG_Offset)
                            {
                                Response = EDD_STS_ERR_PARAM;
                                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                EDDP_CRT_TRACE_04(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> activate DFP-Provider FrameID(0x%X) but SFOffset(%d) != PRM-SFOffset(%d), SFPosition=%d",
                                    pProvider->AddParam.FrameID, SFOffsetTemp, pAcwDG->AddParam.DG_Offset, pProvider->DfpParam.SFPosition);
                                // set SFOffset to UNDEFINED
                                pProvider->DfpParam.SFOffset = EDD_SFOFFSET_UNDEFINED;
                            }

                            /* ---------------------------------------------------- */
                            /* DataLen against PDIRSubframeData.DataLen             */
                            /* ---------------------------------------------------- */
                            if (Response == EDD_STS_OK)
                            {
                                // provider DataLen must be identical with DataLen in PDIRData
                                if (pProvider->AddParam.DataLen != pAcwDG->AddParam.DG_Len)
                                {
                                    Response = EDD_STS_ERR_PARAM;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> DFP-Provider-ADD-DataLen(%d) does not match with PDIRData-DataLength(%d)!",
                                        pProvider->AddParam.DataLen, pAcwDG->AddParam.DG_Len);
                                }
                            }

                            /* --------------------------------------------------------- */
                            /* CycleReductionRatio against PDIRData.CycleReductionRatio  */
                            /* --------------------------------------------------------- */
                            if (Response == EDD_STS_OK)
                            {
                                if (pProvider->DfpParamCheck.CycleReductionRatio == EDD_CYCLE_REDUCTION_RATIO_UNDEFINED)
                                    CycleReductionRatioTemp = pRqbParam->CycleReductionRatio;
                                else
                                    CycleReductionRatioTemp = pProvider->DfpParamCheck.CycleReductionRatio;

                                // provider CycleReductionRatio must be identical with CycleReductionRatio in PDIRData
                                if (CycleReductionRatioTemp != CycleReductionRatioPDIRData)
                                {
                                    Response = EDD_STS_ERR_PARAM;
                                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                                    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> DFP-Provider-ADD/CONTROL-CycleReductionRatio(%d) does not match with PDIRData-CycleReductionRatio(%d)",
                                        CycleReductionRatioTemp, CycleReductionRatioPDIRData);
                                }
                            }
                                               
                            if (Response != EDD_STS_OK)
                            {
                                // disconnect the DFP-provider from pack group
                                EDDP_CRTProvDfpDisconnectFromPackGroup (pDDB, pProvider->ProviderId);
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
                    // first ACW of this provider is always red ACW
                    EDDP_ASSERT_FALSE(pAcw->ROGPhase != EDDP_CRT_PROV_ACW_PHASE_R);

                    // check following parameters only if main ACW is activated (MODE_ACTIVATE)
                    // and only if this ACW is not yet active
                    if (   (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE)
                        && (!(pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK))
                       )
                    {
                        /* -------------------------------------------- */
                        /* PDIRData must be present for activate        */
                        /* -------------------------------------------- */
                        if ( !(EDDP_PRMPDIRDataIsPDIRDataPresent(pDDB, LSA_TRUE)) )
                        {
                            Response = EDD_STS_ERR_SEQUENCE;
                            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> activate RTC3 provider but PDIRData is not present!");
                        }

                        /* -------------------------------------------- */
                        /* is Provider.FrameID in PDIRData present?     */
                        /* -------------------------------------------- */
                        if (Response == EDD_STS_OK)
                        {
                            if ( !(EDDP_PRMPDIRDataIsFrameIDPresent(pDDB, pProvider->AddParam.FrameID, LSA_TRUE, &CycleReductionRatioPDIRData, &CyclePhasePDIRData, LSA_TRUE)) )
                            {
                                Response = EDD_STS_ERR_PARAM;
                                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> The Provider.FrameID(0x%X) is not found at all or not found with the expected role in PDIRData-Record!", 
                                    pProvider->AddParam.FrameID);
                            }
                        }

                        /* -------------------------------------------- */
                        /* redundant provider                           */
                        /* -------------------------------------------- */
                        if ((Response == EDD_STS_OK) && (pProvider->ProviderFlags & EDDP_CRT_PROV_FLAGS_IS_REDUNDANT))
                        {
                            // check the FrameID(x+1) is available in PDIRData_A
                            if ( !(EDDP_PRMPDIRDataIsRedundantFrameIDCoupleAvailable(pDDB, pProvider->AddParam.FrameID, LSA_TRUE)) )
                            {
                                Response = EDD_STS_ERR_PARAM;
                                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> This provider has redundant FrameID(0x%X) but no FrameID(x+1) is available in PDIRData!", 
                                    pProvider->AddParam.FrameID);
                            }
                        }

                        /* -------------------------------------------- */
                        /* port parameter                               */
                        /* -------------------------------------------- */
                        if (Response == EDD_STS_OK)
                        {
                            // check and get port parameter
                            Response = EDDP_CRTCheckAndGetProviderPortParam (pDDB, pRqbParam);

                            if (Response == EDD_STS_OK)
                            {
                                // check DataLen in provider and in PDIRData
                                DataLenPDIRData = EDDP_PRMPDIRDataGetDataLen (pDDB, pProvider->AddParam.FrameID, LSA_TRUE);

                                switch (pProvider->RTClass)
                                {
                                    case EDDP_CRT_RTCLASS_RT:
                                        DataLenMin = EDD_CSRT_DATALEN_MIN;
                                    break;

                                    case EDDP_CRT_RTCLASS_UDP:
                                        DataLenMin = EDD_CSRT_UDP_DATALEN_MIN;
                                    break;

                                    default:
                                    {
                                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCheckProviderControlParam(): FATAL -> Provider-RTClass unknown! RTClass=0x%X", 
                                            pProvider->RTClass);
                                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                                    }
                                    break;
                                }

                                if (pProvider->AddParam.DataLen <= DataLenMin)
                                {
                                    // DataLen in PDIRData must be DATALEN_MIN
                                    if (DataLenPDIRData != DataLenMin)
                                    {
                                        Response = EDD_STS_ERR_PARAM;
                                    }
                                }
                                else
                                {
                                    // provider DataLen must be identical with DataLen in PDIRData
                                    if (pProvider->AddParam.DataLen != DataLenPDIRData)
                                    {
                                        Response = EDD_STS_ERR_PARAM;
                                    }
                                }

                                if (Response != EDD_STS_OK)
                                {
                                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> Provider-ADD-DataLen(%d) does not match with PDIRData-DataLength(%d)!", 
                                        pProvider->AddParam.DataLen, DataLenPDIRData);
                                }
                            }
                        }

                        /* -------------------------------------------- */
                        /* DstMAC address                               */
                        /* FastForwardingMulticastMACAdd in PDIRData    */
                        /* set DstMAC always after setup of PortInfo    */
                        /* because of FrameID in FFW-MAC                */
                        /* -------------------------------------------- */
                        if (Response == EDD_STS_OK)
                        {
                            LSA_UINT32      FastForwardingMulticastMACAdd = 0;

                            EDDP_ASSERT_FALSE(pProvider->AcwList.BeginIdx == EDDP_CRT_CHAIN_IDX_END);
                            pAcw = &pProvMngm->pAcwEntry[pProvider->AcwList.BeginIdx];

                            if (pProvider->AuxAcwId == EDDP_CRT_CHAIN_IDX_END)
                            {
                                // 1-CR start up (advanced)
                                if (pProvider->ProviderFlags & EDDP_CRT_PROV_FLAGS_IS_MCCR)
                                {
                                    // MultiCast-Provider
                                    if ( EDDP_PRMPDIRDataIsFrameDataPropertiesPresent(pDDB, LSA_TRUE) )
                                    {
                                        // "FrameDataProperties" is present in PDIRData
                                        // get parameter PDIRData.FrameDataProperties.FastForwardingMulticastMACAdd
                                        FastForwardingMulticastMACAdd = 
                                            EDDP_PRMPDIRDataGetFrameDataProperties (pDDB, EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_SHIFT, LSA_TRUE);
                                        // if Legacy, so use RTC3-MultiCast-DstMAC from MC-Table
                                        if (FastForwardingMulticastMACAdd == EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_LEGACY)
                                        {
                                            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW,"[H:--] EDDP_CRTCheckProviderControlParam(): MultiCast-Provider with 1-CR start up (advanced) - FastForwardingMulticastMACAdd(0x%X) is legacy in PDIRData.FrameDataProperties - so use RTC3-MC-MAC!", 
                                                FastForwardingMulticastMACAdd);
                                            FastForwardingMulticastMACAdd = EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_RTC3;
                                        }
                                    }
                                    else
                                    {
                                        // "FrameDataProperties" is not present in PDIRData
                                        // use RTC3-MultiCast-DstMAC from MC-Table
                                        FastForwardingMulticastMACAdd = EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_RTC3;
                                    }
                                }
                                else
                                {
                                    // UniCast-Provider
                                    // "FrameDataProperties" must be present in PDIRData
                                    if ( EDDP_PRMPDIRDataIsFrameDataPropertiesPresent(pDDB, LSA_TRUE) )
                                    {
                                        // get parameter PDIRData.FrameDataProperties.FastForwardingMulticastMACAdd
                                        FastForwardingMulticastMACAdd = 
                                            EDDP_PRMPDIRDataGetFrameDataProperties (pDDB, EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_SHIFT, LSA_TRUE);
                                        // Legacy is not permitted
                                        if (FastForwardingMulticastMACAdd == EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_LEGACY)
                                        {
                                            Response = EDD_STS_ERR_PARAM;
                                            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> UniCast-Provider with 1-CR start up (advanced) but FastForwardingMulticastMACAdd(0x%X) is legacy in PDIRData.FrameDataProperties!", 
                                                FastForwardingMulticastMACAdd);
                                        }
                                    }
                                    else
                                    {
                                        Response = EDD_STS_ERR_PARAM;
                                        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> UniCast-Provider with 1-CR start up (advanced) but PDIRData.FrameDataProperties is not present!");
                                    }
                                }
                            }
                            else
                            {
                                // 2-CR start up (legacy)
                                if (pProvider->ProviderFlags & EDDP_CRT_PROV_FLAGS_IS_MCCR)
                                {
                                    Response = EDD_STS_ERR_PARAM;
                                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> MultiCast-Provider and RTC3-Legacy!");
                                }
                                else
                                {
                                    // UniCast-Provider
                                    if ( EDDP_PRMPDIRDataIsFrameDataPropertiesPresent(pDDB, LSA_TRUE) )
                                    {
                                        // "FrameDataProperties" is present in PDIRData
                                        // get parameter PDIRData.FrameDataProperties.FastForwardingMulticastMACAdd
                                        FastForwardingMulticastMACAdd = 
                                            EDDP_PRMPDIRDataGetFrameDataProperties (pDDB, EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_SHIFT, LSA_TRUE);
                                        // only Legacy is permitted
                                        if (FastForwardingMulticastMACAdd != EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_LEGACY)
                                        {
                                            Response = EDD_STS_ERR_PARAM;
                                            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> UniCast-Provider with 2-CR start up (legacy) but FastForwardingMulticastMACAdd(0x%X) is not legacy in PDIRData.FrameDataProperties!", 
                                                FastForwardingMulticastMACAdd);
                                        }
                                    }
                                    else
                                    {
                                        // "FrameDataProperties" is not present in PDIRData
                                        FastForwardingMulticastMACAdd = EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FASTFORWARDINGMULTICASTMACADD_LEGACY;
                                    }

                                    if (Response == EDD_STS_OK)
                                    {
                                        // set the DstMAC from RQB to ACW
                                        Response = EDDP_CRTProvAcwCheckAndSetDstMACFromProviderControlRQB (pDDB, pProvider->ProviderId, pRqbParam);
                                    }
                                }
                            }

                            // now set the DstMAC dependent of FastForwardingMulticastMACAdd
                            if (Response == EDD_STS_OK)
                            {
                                // set DstMAC from PDIRData to all ACWs of this provider (but not to AUX-ACW)
                                AcwIdx = pProvider->AcwList.BeginIdx;
                                // loop through all ACWs in this provider from BeginIdx to EndIdx
                                while (AcwIdx != EDDP_CRT_CHAIN_IDX_END)
                                {
                                    EDDP_ASSERT_FALSE(AcwIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);
                                    // not for AUX-ACW
                                    if (AcwIdx != pProvider->AuxAcwId)
                                    {
                                        // set DstMAC to ACW
                                        EDDP_CRTProvAcwSetDstMACFromPDIRData (pDDB, AcwIdx, (LSA_UINT16)FastForwardingMulticastMACAdd);
                                    }
                                    // next AcwIdx in this provider
                                    AcwIdx = pProvMngm->pAcwEntry[AcwIdx].AcwProvLink.Next;
                                }
                            }
                        }
                    }
                }
            }
	        break;

            default:
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCheckProviderControlParam(): FATAL -> ROGPhase unknown! ROGPhase=0x%X", 
                    pProvider->ROGPhase);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }
    }

    /* ---------------------------------------------------- */
    /* is SENDCLOCK_CHANGE current in processing ?          */
    /* ---------------------------------------------------- */
    if (Response == EDD_STS_OK)
    {
        if ((pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE) || (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE_AUX))
        {
            if (EDDP_CMD_Is_Command_Running(pDDB, EDDP_CMD_GSYNC_UPDATE_COMCYCL))
            {
                Response = EDD_STS_ERR_SEQUENCE;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> Service EDD_SRV_SENDCLOCK_CHANGE is current in processing on Activate");
            }
        }
    }

	/* ---------------------------------------------------- */
	/* Check "CRNumber"                                     */
	/* ---------------------------------------------------- */
#if defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)
    // in case of ERTEC200+ the CR Number should be unique
    if( (pDDB->Hardware == EDDP_HW_ERTEC200P) || (pDDB->Hardware == EDDP_HW_ERTEC200P_FPGA) )
    {
        if (Response == EDD_STS_OK)
        {
            if (   (pProvider->AddParam.CRNumber == EDD_CRNUMBER_UNDEFINED)
                && (   (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_SET_UNDEFINED_PARAMS) 
                                || (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE)
                    || (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE_AUX)
                            )
               )
            {
                if (pRqbParam->IOParams.CRNumber != EDD_CRNUMBER_UNDEFINED)
                {
                    LSA_UINT16      ProvIdx;
        
                    ProvIdx = pDDB->CRT.Prov.ProviderUsedList.BeginIdx;
                    // loop through all used providers
                    while (ProvIdx != EDDP_CRT_CHAIN_IDX_END)
                    {
                        EDDP_ASSERT_FALSE(ProvIdx >= pDDB->CRT.CfgParam.ProviderMaxNumber);
        
                        if (pProvMngm->pProvEntry[ProvIdx].AddParam.CRNumber != EDD_CRNUMBER_UNDEFINED) 
                        {
                            if (pProvMngm->pProvEntry[ProvIdx].AddParam.CRNumber == pRqbParam->IOParams.CRNumber)
                            {
                                Response = EDD_STS_ERR_PARAM;
                                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> CRNumber(%d) already in use!", 
                                                         pRqbParam->IOParams.CRNumber);
                                break;
                            }
                        }
                        ProvIdx = pProvMngm->pProvEntry[ProvIdx].UsedFreeLink.Next;
                    }
                }
            }
        }
    }
#endif
	
    if (Response == EDD_STS_OK)
	{
        if (  (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE) || (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE_AUX)  )
        {
         
#if defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)
            /* ---------------------------------------------------------- */
            /* Check "pIOBaseAdrPerif"                                    */
            /* ---------------------------------------------------------- */
            if((pDDB->Hardware == EDDP_HW_ERTEC200P) || (pDDB->Hardware == EDDP_HW_ERTEC200P_FPGA))
            {
                if(LSA_HOST_PTR_ARE_EQUAL(pDDB->CRT.pIOBaseAdrPerif, LSA_NULL))
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> pIOBaseAdrPerif=NULL on Activate!");
                }
            }
#endif
        }
    }

	/* ---------------------------------------------------- */
    /* PN-IP Rev1: "Running in a Squeeze" is not allowed    */
    /*  - RTC1/2/RToverUDP provider is activated            */
    /*  - Fragmentation is ON in PDIRData                   */
	/* ---------------------------------------------------- */
    #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
    if (   (Response == EDD_STS_OK)
        && (EDDP_HAL_HW_PNIP_REV1 == EDDP_HALGetPnipRevision(pDDB)) 
       )
    {
        // for RTC1/2/RToverUDP provider and if PDIRData is present
        if ((pProvider->ROGPhase != EDDP_CRT_PROV_ACW_PHASE_R) && (EDDP_PRMPDIRDataIsPDIRDataPresent(pDDB, LSA_TRUE)))
        {
            EDDP_ASSERT_FALSE(pProvider->AcwList.BeginIdx == EDDP_CRT_CHAIN_IDX_END);
            pAcw = &pProvMngm->pAcwEntry[pProvider->AcwList.BeginIdx];

            // check following parameters only if main ACW is activated (MODE_ACTIVATE)
            // and only if this ACW is not yet active
            if (   (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE)
                && (!(pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK))
               )
            {
                LSA_UINT32      FragmentationMode = EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_OFF;

                // is "FrameDataProperties" present in PDIRData ?
                if (EDDP_PRMPDIRDataIsFrameDataPropertiesPresent(pDDB, LSA_TRUE))
                {
                    // get parameter PDIRData.FrameDataProperties.FragmentationMode
                    FragmentationMode = EDDP_PRMPDIRDataGetFrameDataProperties (pDDB, EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_SHIFT, LSA_TRUE);
                }

                // is Fragmentation ON in PDIRData ?
                if (FragmentationMode != EDDP_PRM_PDIRDATA_FRAMEDATAPROPERTIES_FRAGMENTATIONMODE_OFF)
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> with PN-IP Rev1 the <Running in a Squeeze> is not allowed: RTC1/2/RToverUDP provider is activated and Fragmentation=ON in PDIRData!");
                }
            }
        }
    }
    #endif
    
    /* ---------------------------------------------------- */
    /* check IO_Configured and FeedInLoadLimitation         */
    /* ---------------------------------------------------- */
    if (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE)
    {
        if (EDD_STS_OK == Response)
        {
            Response = EDDP_NRTFillIsIOAllowed(pDDB);
            if(EDD_STS_ERR_NOT_ALLOWED == Response)
            {
                Response = EDD_STS_ERR_SERVICE;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, 
                            "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> MODE_ACTIVATE not allowed because of IO_Configured=FALSE");
            }
        }
    
        if (EDD_STS_OK == Response)
        {
            Response = EDDP_NRTFillIsFeedInLoadLimitationActive(pDDB);
            if(EDD_STS_ERR_NOT_ALLOWED == Response)
            {
                Response = EDD_STS_ERR_SERVICE;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,
                            "[H:--] EDDP_CRTCheckProviderControlParam(): ERROR -> MODE_ACTIVATE not allowed because of FeedInLoadLimitation=FALSE");
            }
        }
    }

    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTCheckConsumerAddParam                           +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB            +*/
/*+                             EDD_UPPER_CSRT_CONSUMER_ADD_PTR_TYPE    pRqbParam       +*/
/*+                                                                                     +*/
/*+  Result                :    EDD_RSP                                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         (in)    +*/
/*+  pRqbParam      : pointer to RQB parameter from user (UPPER-RQB)            (in)    +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks the Consumer-Add parameter.                      +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckConsumerAddParam(
    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB,
	EDD_UPPER_CSRT_CONSUMER_ADD_PTR_TYPE    pRqbParam)
{
    LSA_UINT16          DataLenMin, DataLenMax, /*CycleReductionRatioMin, CycleReductionRatioMax,*/ RtClassProp;
    EDD_RSP             Response;
    LSA_UINT8           RtClass, BaseRtClass;

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCheckConsumerAddParam(pRqbParam=0x%X)", pRqbParam);

    if (pDDB->CRT.CfgParam.ConsumerMaxNumber == 0)
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> ConsumerMaxNumber=0!");
        return (Response);
    }

    if (pDDB->CRT.Cons.ConsumerFreeList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
    {
        Response = EDD_STS_ERR_RESOURCE;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> No more free consumer! ConsumerUsedCnt=%d", 
            pDDB->CRT.Cons.ConsumerUsedCnt);
        return (Response);
    }
    
    /* All consumers within TSB must be processed within a cycle. For small send clocks */
    /* less than 250us the maximum amount of consumer must be restricted because otherwise */
    /* the hardware can not process all consumers. */

    Response = EDD_STS_OK;
    /* ---------------------------------------------------- */
    /* check IOParams                                       */
    /* ---------------------------------------------------- */
    if( (EDDP_HAL_HW_HERA == EDDP_HALGetChipVersion(pDDB)) &&
        (0 != pRqbParam->IOParams.AppGroup)
      )
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,
                    "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> With chip version HERA only AppGroup of 0 is allowed! AppGroup(%d)",
                    pRqbParam->IOParams.AppGroup);
        return (Response);
    }

    if (   (EDD_CONS_CLEAR_ON_MISS_DISABLE != pRqbParam->IOParams.ClearOnMISS)
        && (EDD_CONS_CLEAR_ON_MISS_ENABLE != pRqbParam->IOParams.ClearOnMISS) )
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,
            "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR ->Illegal value for ClearOnMiss(%d)",
            pRqbParam->IOParams.ClearOnMISS);
        return (Response);
    }

    /* ---------------------------------------------------- */
    /* initialization of check values                       */
    /* ---------------------------------------------------- */
    RtClassProp = (pRqbParam->Properties & EDD_CSRT_CONS_PROP_RTCLASS_MASK);

    if (pRqbParam->Properties & EDD_CSRT_CONS_PROP_SYSRED)
    {   // ***** expect SRD in combination with RTC1+2 only
        switch (RtClassProp)
        {
            case EDD_CSRT_CONS_PROP_RTCLASS_1:
            case EDD_CSRT_CONS_PROP_RTCLASS_2:
                break;
            default:
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> System-Redundancy-Consumer is not allowed with RTC3! (Properties=0x%X)", 
                    pRqbParam->Properties);
                return (Response);
                break;
        }
    }

    if ( pRqbParam->Properties & EDD_CSRT_CONS_PROP_PDU_FORMAT_DFP )
    {   // ***** expect DFP in combination with RTC3 only
        if ( (RtClassProp != EDD_CSRT_CONS_PROP_RTCLASS_3) || (pRqbParam->Properties & EDD_CSRT_CONS_PROP_RTC3_MODE_LEGACY) )
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> DFP expects non legacy with PROP_RTCLASS_3 (Properties=0x%X)", 
                pRqbParam->Properties);
            return (Response);
        }
        // ***** Note: pRqbParam->IOParams.SFPosition must be valid on ACTIVATE
        if ( pRqbParam->IOParams.SFPosition != EDD_SFPOSITION_UNDEFINED )
        {
            if ( !EDDP_CRT_IS_DFP_SFPOSITION_IN_VALID_RANGE(pRqbParam->IOParams.SFPosition) )
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> SFPosition(%d) is out of range and is not to UNDEFINED set", 
                    pRqbParam->IOParams.SFPosition);
                return (Response);
            }
        }
    }

    switch (RtClassProp)
    {
        case EDD_CSRT_CONS_PROP_RTCLASS_1:
        case EDD_CSRT_CONS_PROP_RTCLASS_2:
        case EDD_CSRT_CONS_PROP_RTCLASS_3:
        {                 
            if (pRqbParam->Properties & EDD_CSRT_CONS_PROP_PDU_FORMAT_DFP)
            {
                DataLenMin = EDD_CSRT_DFP_DATALEN_MIN;
                DataLenMax = EDD_CSRT_DFP_DATALEN_MAX;
            }
            else
            {
                DataLenMin = EDD_CSRT_DATALEN_MIN;
                DataLenMax = EDD_CSRT_DATALEN_MAX;
            }
            //CycleReductionRatioMin = EDD_CSRT_CYCLE_REDUCTION_RATIO_MIN;
            //CycleReductionRatioMax = EDD_CSRT_CYCLE_REDUCTION_RATIO_MAX;
            BaseRtClass = EDDP_CRT_RTCLASS_RT;
        }
        break;

        case EDD_CSRT_CONS_PROP_RTCLASS_UDP:
        {
            #ifdef EDDP_CRT_SUPPORT_RT_OVER_UDP_ON
            //
            #else
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> RToverUDP is not supported! Properties=0x%X", 
                pRqbParam->Properties);
            #endif

            RtClassProp = EDD_CSRT_CONS_PROP_RTCLASS_1; // RtOverUdp is in range of RTC1: 0xC000 – 0xFBFF
            DataLenMin  = EDD_CSRT_UDP_DATALEN_MIN;
            DataLenMax  = EDD_CSRT_UDP_DATALEN_MAX;
            //CycleReductionRatioMin = EDD_CSRT_UDP_CYCLE_REDUCTION_RATIO_MIN;
            //CycleReductionRatioMax = EDD_CSRT_UDP_CYCLE_REDUCTION_RATIO_MAX;
            BaseRtClass = EDDP_CRT_RTCLASS_UDP;
        }
        break;

        default:
        {   // ***** no RTclass flag set
            RtClassProp = 0;
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> Unknown RTC-Properties: RTC-Properties=0x%X", 
                (pRqbParam->Properties & EDD_CSRT_CONS_PROP_RTCLASS_MASK));
            DataLenMin = DataLenMax = 0; //CycleReductionRatioMin = CycleReductionRatioMax = 0;
            BaseRtClass = EDDP_CRT_RTCLASS_UNKNOWN;
        }
        break;
    }

    // FrameID
    // Properties
    if (RtClassProp)    // one of RTC1 .. RTC3
    {
        if (pRqbParam->Properties & EDD_CSRT_CONS_PROP_RTC3_MODE_LEGACY)
        {   // ***** 2CR only with RTC3 !!!!
            if (RtClassProp != EDD_CSRT_CONS_PROP_RTCLASS_3)
            {
                // 2-CR-mode only allowed for RTC3
                Response = EDD_STS_ERR_PARAM;
            }
        }

        RtClass = EDDP_CRTGetRTClass (pDDB, pRqbParam->FrameID);

        switch (RtClassProp)
        {
            case EDD_CSRT_CONS_PROP_RTCLASS_1:
            {
                if (RtClass != EDDP_CRT_FRAMEID_RTCLASS_1)
                {
                    Response = EDD_STS_ERR_PARAM;
                }
                else
                {
                    if (pRqbParam->FrameID >= (EDDP_CRT_FRAMEID_RT_CLASS1_START + pDDB->CRT.CfgParam.ConsumerMaxNumber))
                    {
                        Response = EDD_STS_ERR_PARAM;
                        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> FrameID(0x%X) > FrameIDStart+ConsumerMaxNumber(0x%X)!", 
                            pRqbParam->FrameID, (EDDP_CRT_FRAMEID_RT_CLASS1_START + pDDB->CRT.CfgParam.ConsumerMaxNumber)-1);
                    }
                }
            }
            break;

            case EDD_CSRT_CONS_PROP_RTCLASS_2:
            {
                if (RtClass != EDDP_CRT_FRAMEID_RTCLASS_2)
                {
                    Response = EDD_STS_ERR_PARAM;
                }
                else
                {
                    if (pRqbParam->FrameID >= (EDDP_CRT_FRAMEID_RT_CLASS2_START + pDDB->CRT.CfgParam.ConsumerMaxNumber))
                    {
                        Response = EDD_STS_ERR_PARAM;
                        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> FrameID(0x%X) > FrameIDStart+ConsumerMaxNumber(0x%X)!", 
                            pRqbParam->FrameID, (EDDP_CRT_FRAMEID_RT_CLASS2_START + pDDB->CRT.CfgParam.ConsumerMaxNumber)-1);
                    }
                }
            }
            break;

            case EDD_CSRT_CONS_PROP_RTCLASS_3:
            {
                if (RtClass != EDDP_CRT_FRAMEID_RTCLASS_3)
                {
                    Response = EDD_STS_ERR_PARAM;
                }
                else
                {
                    // checks for redundant FrameID
                    if (EDDP_IS_IRT_REDUNDANT_FRAMEID(pRqbParam->FrameID))
                    {   
                        if (pRqbParam->FrameID & 0x1)
                        {
                            Response = EDD_STS_ERR_PARAM;
                            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> redundant FrameID(0x%X) is not even number!", 
                                pRqbParam->FrameID);
                        }
                        else
                        {
                            // redundant FrameID --> only allowed in 1-CR-mode
                            if (pRqbParam->Properties & EDD_CSRT_CONS_PROP_RTC3_MODE_LEGACY)
                            {   // ***** not allowed in 2CR mode (Legacy)
                                Response = EDD_STS_ERR_PARAM;
                                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> redundant FrameID(0x%X) with 2CR mode!", 
                                    pRqbParam->FrameID);
                            }
                            else
                            {
                                // redundant FrameID --> not allowed with EDD_FORWARDMODE_CT
                                if (pRqbParam->IOParams.ForwardMode == EDD_FORWARDMODE_CT)
                                {
                                    Response = EDD_STS_ERR_PARAM;
                                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> redundant FrameID(0x%X) with cut-through mode!", 
                                        pRqbParam->FrameID);
                                }
                            }
                        }
                    }
                }
            }
            break;

            default:
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_WARN, "[H:--] EDDP_CRTCheckConsumerAddParam(): FrameID(0x%X) not in predefined range", 
                    pRqbParam->FrameID);
            }
            break;
        }

        if (Response == EDD_STS_ERR_PARAM)
        {
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> FrameID(0x%X) missmatches given properties: 0x%X", 
                pRqbParam->FrameID, pRqbParam->Properties);
        }
        else
        {
            if (RtClassProp == EDD_CSRT_CONS_PROP_RTCLASS_3)
            {   // ***** is'nt allowed to add a RTC3 (non DFP) FrameID twice !!!
                if (!(pRqbParam->Properties & EDD_CSRT_CONS_PROP_PDU_FORMAT_DFP))
                {
                    if ( EDDP_CRTConsExistFrameID(pDDB, pRqbParam->FrameID, LSA_TRUE) )
                    {
                        Response = EDD_STS_ERR_PARAM;
                        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> FrameID(0x%X) still used", 
                            pRqbParam->FrameID);
                    }
                }
            }
        }
    }

    if (Response != EDD_STS_OK)
    {
        return (Response);
    }

    /* ---------------------------------------------------- */
    /* parameter DataLen dependent of RT/RToverUDP consumer */
    /* ---------------------------------------------------- */
    if ((pRqbParam->DataLen < DataLenMin) || (pRqbParam->DataLen > DataLenMax))
    {   // ***** out of range
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> DataLen(%d) is out of range: DataLenMin=%d, DataLenMax=%d", 
            pRqbParam->DataLen, DataLenMin, DataLenMax);
        return (Response);
    }

    // parameter (CycleReductionRatio) CyclePhase   (not used in EDDP !!!)
    /*
    if ( (pRqbParam->CyclePhase != EDD_CYCLE_PHASE_UNDEFINED) &&
         ((pRqbParam->CycleReductionRatio != EDD_CYCLE_REDUCTION_RATIO_UNDEFINED))
       )
    {
        if (pRqbParam->CyclePhase > pRqbParam->CycleReductionRatio)
        {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> Ph(%d) > RR(%d)", 
                    pRqbParam->CyclePhase, pRqbParam->CycleReductionRatio);
                return (Response);
        }
    }
    */
    // parameter CycleReductionRatio used in EDDP for TSBRR-Reload within the TimerScoreBoard !!!
    Response = EDDP_CRTCheckRangeOfCycleReductionRatio (pDDB, BaseRtClass, pRqbParam->Properties, pRqbParam->CycleReductionRatio, LSA_TRUE);
    if (Response != EDD_STS_OK)
    {
        return (Response);
    }

	// parameter DataHoldFactor
	if ( pRqbParam->Properties & EDD_CSRT_CONS_PROP_PDU_FORMAT_DFP )
	{
		if (   (pRqbParam->DataHoldFactor < EDD_SRT_DFP_CONSUMER_DATAHOLD_FACTOR_MIN)
			|| (pRqbParam->DataHoldFactor > EDD_SRT_DFP_CONSUMER_DATAHOLD_FACTOR_MAX)
		)
		{
			Response = EDD_STS_ERR_PARAM;
			EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
			EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> DataHoldFactor(%d) is out of range (DFP!)! Min=%d, Max=%d", 
				pRqbParam->DataHoldFactor, EDD_SRT_DFP_CONSUMER_DATAHOLD_FACTOR_MIN, EDD_SRT_DFP_CONSUMER_DATAHOLD_FACTOR_MAX);
			return (Response);
		}
	}
	else
	{
		if (   (pRqbParam->DataHoldFactor < EDD_SRT_CONSUMER_DATAHOLD_FACTOR_MIN)
			|| (pRqbParam->DataHoldFactor > EDD_SRT_CONSUMER_DATAHOLD_FACTOR_MAX)
		)
		{
			Response = EDD_STS_ERR_PARAM;
			EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
			EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> DataHoldFactor(%d) is out of range! Min=%d, Max=%d", 
				pRqbParam->DataHoldFactor, EDD_SRT_CONSUMER_DATAHOLD_FACTOR_MIN, EDD_SRT_CONSUMER_DATAHOLD_FACTOR_MAX);
			return (Response);
		}
	}

    // parameter UserID
    // parameter ConsumerID


    // parameters in IOParams
    // parameter IOParams.BufferProperties
    // parameter IOParams.CRNumber
    // parameter IOParams.AppGroup
    // parameter IOParams.ForwardMode + CutThrough_OutDataStart
    if (pRqbParam->IOParams.ForwardMode == EDD_FORWARDMODE_CT)
    {
#if 1
        // cut-through mode is not allowed any longer (see TFS-RQ 1193172)
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> CutThrough-Mode is not allowed any longer");
        return (Response);
#else   
        // if cut-through mode shall be used again, reinsert this code
        if (pDDB->CRT.Cons.CT_ConsumerID != EDDP_CRT_CHAIN_IDX_END)
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB->pDDB, Response);
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> one cunsumer with CutThrough-Mode is already added -> CT_ConsumerID=%d", 
                pDDB->CRT.Cons.CT_ConsumerID);
            return (Response);
        }
        if ( (pRqbParam->IOParams.CRNumber != EDD_CRNUMBER_NOT_USED) && (pRqbParam->IOParams.CRNumber != EDD_CRNUMBER_UNDEFINED) )
        {   // ***** with EDD_FORWARDMODE_CT only EDD_CRNUMBER_NOT_USED (SDRAM) is allowed for activating 
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> CRNumber(%d) in CT mode expected for NOT_USED or UNDEF", 
                pRqbParam->IOParams.CRNumber);
            return (Response);
        }
        if (RtClassProp != EDD_CSRT_CONS_PROP_RTCLASS_3)
        {   // ***** not RTC3 consumer !!!
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> CT is only with RTC3 allowed but this is RTC%d", 
                RtClassProp);
            return (Response);
        }
        // parameter IOParams.CutThrough_OutDataStart only if CT mode enabled
        if (pRqbParam->IOParams.CutThrough_OutDataStart > EDDP_CRT_MAX_CUTTHROUGH_OUTDATASTART)
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> CT_OutDataStart(0x%X) is out of range [0..0x%X]", 
                pRqbParam->IOParams.CutThrough_OutDataStart, EDDP_CRT_MAX_CUTTHROUGH_OUTDATASTART);
            return (Response);
        }
#endif
    }
    else
    {
#if defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)
        if((pDDB->Hardware == EDDP_HW_ERTEC200P) || (pDDB->Hardware == EDDP_HW_ERTEC200P_FPGA))
        {
            if ( !(EDDP_CRT_IS_CRNUMBER_IN_PERIF_RANGE(pRqbParam->IOParams.CRNumber)) && (pRqbParam->IOParams.CRNumber!= EDD_CRNUMBER_UNDEFINED) )
            {   
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> CRNumber(%d) is not a valid Perif-Nr %d)", 
                    pRqbParam->IOParams.CRNumber,pRqbParam->IOParams.ForwardMode);
            }
        }
#endif
    }

    // parameter IOParams.DataOffset
    if (pRqbParam->IOParams.DataOffset == EDD_DATAOFFSET_INVALID)
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> DataOffset=INVALID");
        return (Response);
    }

#if defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)
    if( EDDP_HAL_HW_ERTEC200P == EDDP_HALGetChipVersion(pDDB) )
    {
        // check 8 byte alignment of IOParams.DataOffset
        if (   (EDDP_CRT_IS_CRNUMBER_IN_PERIF_RANGE(pRqbParam->IOParams.CRNumber))
            && (pRqbParam->IOParams.DataOffset != EDD_DATAOFFSET_UNDEFINED)
           )
        {
            // this consumer uses PERIF: check 8 byte alignment of DataOffset (3 bits = 0)
            if ( !(EDDP_GlbIsAligned(pDDB, pRqbParam->IOParams.DataOffset, EDDP_GLB_ALIGNMENT_8_BYTE)) )
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> DataOffset(0x%X) for PERIF is not 8 byte aligned!", 
                    pRqbParam->IOParams.DataOffset);
                return (Response);
            }
        }
    }
#endif
    // parameter Partial_DataLen
    // parameter Partial_DataOffset
    if (pRqbParam->Partial_DataLen > 0)
    {   // ***** Partial_DataOffset range: 0..(DataLen-1)
        if (pRqbParam->Partial_DataOffset >= pRqbParam->DataLen)
        {   // ***** out of range
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> Partial_DataOffset(%d) is out of range: 0..%d", 
                pRqbParam->Partial_DataOffset , (pRqbParam->DataLen-1));
            return (Response);
        }
        else
        {   // ***** Partial_DataLen range: 1..(DataLen-Partial_DataOffset)
            if (pRqbParam->Partial_DataLen > (pRqbParam->DataLen - pRqbParam->Partial_DataOffset))
            {   // ***** out of range
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> Partial_DataLen(%d) is out of range: 1..%d", 
                    pRqbParam->Partial_DataLen, (pRqbParam->DataLen - pRqbParam->Partial_DataOffset));
                return (Response);
            }
        }
    } // ***** else: partial access not used

	/* ---------------------------------------------------- */
	/* Check "CRNumber"                                     */
	/* ---------------------------------------------------- */
#if defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)
    //In case of ERTEC200+ the CR Number should be unique
    if( (pDDB->Hardware == EDDP_HW_ERTEC200P) || (pDDB->Hardware == EDDP_HW_ERTEC200P_FPGA) )
    {
        if (Response == EDD_STS_OK)
        {
            if ( pRqbParam->IOParams.CRNumber != EDD_CRNUMBER_UNDEFINED )
            {
                LSA_UINT16 ConsIdx;
                
                ConsIdx = pDDB->CRT.Cons.ConsumerUsedList.BeginIdx;
                // loop through all used consumers
                while (ConsIdx != EDDP_CRT_CHAIN_IDX_END)
                {
                    EDDP_ASSERT_FALSE(ConsIdx >= pDDB->CRT.CfgParam.ConsumerMaxNumber);
                    
                        if(pDDB->CRT.Cons.pConsEntry[ConsIdx].Param.CRNumber != EDD_CRNUMBER_UNDEFINED) 
                        {
                            if(pDDB->CRT.Cons.pConsEntry[ConsIdx].Param.CRNumber == pRqbParam->IOParams.CRNumber)
                            {
                                Response = EDD_STS_ERR_PARAM;
                                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> CRNumber (%d) already in use!", pRqbParam->IOParams.CRNumber);
                                break;
                            }
                        }
                    ConsIdx = pDDB->CRT.Cons.pConsEntry[ConsIdx].UsedFreeLink.Next;
                }
            }
        }
    }
#endif

	/* ---------------------------------------------------- */
	/* Check "pIOBaseAdrPerif" and "pIOBaseAdrExtRam"       */
	/* ---------------------------------------------------- */
    if (Response == EDD_STS_OK)
    {        
#if defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)
        if((pDDB->Hardware == EDDP_HW_ERTEC200P) || (pDDB->Hardware == EDDP_HW_ERTEC200P_FPGA))
        {
            if (pRqbParam->IOParams.ForwardMode == EDD_FORWARDMODE_CT)
            {
                 // CutThrough-Consumer: uses extarnel RAM
                if (LSA_HOST_PTR_ARE_EQUAL(pDDB->CRT.pIOBaseAdrExtRam, LSA_NULL))
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> pIOBaseAdrExtRam=NULL for CT-Consumer!");
                    return (Response);
                 }
             }
             else
             {
                 // not CutThrough-Consumer: uses PERIF
                 if (LSA_HOST_PTR_ARE_EQUAL(pDDB->CRT.pIOBaseAdrPerif, LSA_NULL))
                 {
                     Response = EDD_STS_ERR_PARAM;
                     EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                     EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerAddParam(): ERROR -> pIOBaseAdrPerif=NULL!");
                     return (Response);
                  }
              }
        }
#endif
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTCheckCloseChannel                               +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                        :    EDDP_LOCAL_HDB_PTR_TYPE         pHDB                    +*/
/*+                                                                                     +*/
/*+  Result                :    EDD_RSP                                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                                     +*/
/*+  pHDB       : Pointer to HandleDescriptionBlock                                     +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks data necessary for CloseChannel.                 +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckCloseChannel(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_LOCAL_HDB_PTR_TYPE     pHDB)
{
    EDD_RSP         Response;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(pHDB);
#endif

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:%2X] EDDP_CRTCheckCloseChannel()", 
        pHDB->Handle);

    Response = EDD_STS_OK;

    // are there some providers or consumers in CRT (DDB) management ?
    if ( EDDP_CRTCheckIsProviderConsumerPresent(pDDB) )
    {
        // user must remove all provider and consumer before CloseChannel
        Response = EDD_STS_ERR_SEQUENCE;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_CRTCheckCloseChannel(): ERROR -> Provider or Consumer available in CRT! Response=0x%X", 
            pHDB->Handle, Response);
    }
    else
    {
        // check if there are some RQBs in processing of this CRT channel
        if (pHDB->pCRT->RunningRQBCount > 0)
        {
            // user must wait until all RQBs are finished
            Response = EDD_STS_ERR_SEQUENCE;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:%2X] EDDP_CRTCheckCloseChannel(): ERROR -> CRT-RQB is still in processing in GlbCrtRQBHandler: ProviderUsedCnt=%d, ConsumerUsedCnt=%d, CRT-RunningRQBCount=%d, RqbQueue.Count=%d, Response=0x%X", 
                pHDB->Handle, pDDB->CRT.Prov.ProviderUsedCnt, pDDB->CRT.Cons.ConsumerUsedCnt, pHDB->pCRT->RunningRQBCount, pDDB->GlbCrtRQBHandler.RqbQueue.Count, Response);
        }
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTCheckProviderSetStateParam                      +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB        +*/
/*+                             EDD_UPPER_CSRT_PROVIDER_SET_STATE_PTR_TYPE  pRqbParam   +*/
/*+                             EDD_SERVICE                                 Service     +*/
/*+                                                                                     +*/
/*+  Result                :    EDD_RSP                                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DviceDescriptionBlock                      (in)        +*/
/*+  pRqbParam      : pointer to RQB parameter from user (UPPER-RQB)        (in)        +*/
/*+  Service        : EDD-Service                                           (in)        +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks the ProviderSetState parameter.                  +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckProviderSetStateParam(
    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB,
	EDD_UPPER_CSRT_PROVIDER_SET_STATE_PTR_TYPE  pRqbParam,
	EDD_SERVICE                                 Service)
{
    EDD_RSP         Response;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(Service);
#endif

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCheckProviderSetStateParam(pRqbParam=0x%X, Service=0x%X)", 
        pRqbParam, Service);

    Response = EDD_STS_OK;

    if (pRqbParam->GroupID >= pDDB->CRT.CfgParam.ProviderGroupMaxNumber)
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderSetStateParam(): ERROR -> GroupID(%d) is out of range! GroupID-Max=%d", 
            pRqbParam->GroupID, ((pDDB->CRT.CfgParam.ProviderGroupMaxNumber)-1));
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTCheckProviderChangePhaseParam                   +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                         pDDB    +*/
/*+                             EDD_UPPER_CSRT_PROVIDER_CHANGE_PHASE_PTR_TYPE   pRqbParam+*/
/*+                                                                                     +*/
/*+  Result                :    EDD_RSP                                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                     (in)        +*/
/*+  pRqbParam      : pointer to RQB parameter from user (UPPER-RQB)        (in)        +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks the ProviderChangePhase parameter.               +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckProviderChangePhaseParam(
    EDDP_LOCAL_DDB_PTR_TYPE                         pDDB,
	EDD_UPPER_CSRT_PROVIDER_CHANGE_PHASE_PTR_TYPE   pRqbParam)
{
    EDD_RSP                                 Response;
    LSA_UINT16                              TBaseIndex;
    LSA_UINT32                              AcwPhaseCnt_ROG;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE              pProvider;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;
    EDDP_CRT_PROVIDER_TBASE_PTR_TYPE        pTBase;

    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCheckProviderChangePhaseParam(pRqbParam=0x%X)", 
        pRqbParam);

    Response    = EDD_STS_OK;
    pProvMngm   = &pDDB->CRT.Prov;

    /* ---------------------------------------------------- */
    /* general parameter                                    */
    /* ---------------------------------------------------- */
    if (pRqbParam->ProviderID >= pDDB->CRT.CfgParam.ProviderMaxNumber)
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderChangePhaseParam(): ERROR -> ProviderID(%d) is out of range! ProviderID-Max=%d", 
            pRqbParam->ProviderID, (pDDB->CRT.CfgParam.ProviderMaxNumber-1));
    }
    else
    {
        if (pProvMngm->ProviderUsedList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
	        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderChangePhaseParam(): ERROR -> There is no used provider!");
        }
        else
        {
            if (pProvMngm->pProvEntry[pRqbParam->ProviderID].State == EDDP_CRT_STATE_FREE)
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
	            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderChangePhaseParam(): ERROR -> Provider is not used!");
            }
            else
            {
                if (   (pRqbParam->CyclePhase == EDD_CYCLE_PHASE_UNDEFINED)
                    || (pRqbParam->CyclePhaseSequence == EDD_CYCLE_PHASE_SEQUENCE_UNDEFINED)
                   )
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderChangePhaseParam(): ERROR -> Ph(%d) or PhSeq(0x%X) is UNDEFINED", 
                        pRqbParam->CyclePhase, pRqbParam->CyclePhaseSequence);
                }
                else
                {
                    if (pProvMngm->pProvEntry[pRqbParam->ProviderID].ROGPhase == EDDP_CRT_PROV_ACW_PHASE_R)
                    {
                        Response = EDD_STS_ERR_PARAM;
                        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderChangePhaseParam(): ERROR -> for RTClass3 provider not supported");
                    }
                    else
                    {
                        if (pProvMngm->pProvEntry[pRqbParam->ProviderID].AcwCnt > 1)
                        {
                            Response = EDD_STS_ERR_PARAM;
                            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderChangePhaseParam(): ERROR -> this provider has more ACWs -> AcwCnt(%d)", 
                                pProvMngm->pProvEntry[pRqbParam->ProviderID].AcwCnt);
                        }
                    }
                }
            }
        }
    }

    /* ---------------------------------------------------- */
    /* checks for CycleReductionRatio, CyclePhase           */
    /* ---------------------------------------------------- */
    if (Response == EDD_STS_OK) 
    {
        pProvider   = &pProvMngm->pProvEntry[pRqbParam->ProviderID];
        EDDP_ASSERT_FALSE(pProvider->AcwList.BeginIdx == EDDP_CRT_CHAIN_IDX_END);
        pAcw        = &pProvMngm->pAcwEntry[pProvider->AcwList.BeginIdx];

        if (   (pAcw->AddParam.CycleReductionRatio != EDD_CYCLE_REDUCTION_RATIO_UNDEFINED)
            && (pRqbParam->CyclePhase > pAcw->AddParam.CycleReductionRatio)
           )
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckProviderChangePhaseParam(): ERROR -> Ph(%d) in RQB > RR(%d) in ACW", 
                pRqbParam->CyclePhase, pAcw->AddParam.CycleReductionRatio);
        }
    }

    /* ---------------------------------------------------- */
    /* resource check of Trog entry                         */
    /* check only if this ACW is already chained in         */
    /* AcwPhaseList and is available in HW                  */
    /* ---------------------------------------------------- */
    if (Response == EDD_STS_OK) 
    {
        pProvider   = &pProvMngm->pProvEntry[pRqbParam->ProviderID];
        EDDP_ASSERT_FALSE(pProvider->AcwList.BeginIdx == EDDP_CRT_CHAIN_IDX_END);
        pAcw        = &pProvMngm->pAcwEntry[pProvider->AcwList.BeginIdx];

        if ((pAcw->State & EDDP_CRT_PROV_ACW_STATE_IN_PHASELIST_MASK) && (pAcw->State & EDDP_CRT_PROV_ACW_STATE_IN_HW_MASK))
        {
            // check if new (additional) Trog is necessary and if yes: is one Trog available ?
            // new TBase
            TBaseIndex  = EDDP_CRTGetTBaseIndex ( pDDB, 
                                                  pAcw->AddParam.CycleReductionRatio, 
                                                  pRqbParam->CyclePhase);
            pTBase      = &pDDB->CRT.Prov.pTBaseEntry[TBaseIndex];

            if (LSA_HOST_PTR_ARE_EQUAL(pTBase->pTrogEntry, LSA_NULL))
            {
                // there is no Trog on new in TBase, to which the ACW will be moved

                // old TBase
                TBaseIndex  = EDDP_CRTGetTBaseIndex ( pDDB, 
                                                      pAcw->AddParam.CycleReductionRatio, 
                                                      pAcw->AddParam.CyclePhase);
                pTBase      = &pDDB->CRT.Prov.pTBaseEntry[TBaseIndex];

                // get sum of all ACWs in old Trog
                AcwPhaseCnt_ROG = pTBase->pTrogEntry->TrogAcwList.AcwPhaseCnt_R
                                + pTBase->pTrogEntry->TrogAcwList.AcwPhaseCnt_O
                                + pTBase->pTrogEntry->TrogAcwList.AcwPhaseCnt_G;

                if (AcwPhaseCnt_ROG > 1)
                {
                    // there are more ACWs an old Trog
                    // new and additional Trog will be necessary for new TBase
                    if (pProvMngm->TrogUsedCnt >= pDDB->CRT.CfgParam.ProviderTrogMaxNumber)
                    {
                        // FATAL because one additional TrogEntry was planned extra for ACW_move command
                        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCheckProviderChangePhaseParam(): FATAL -> additional TrogEntry is necessary but no free Trog is available! TrogUsedCnt=%d, ProviderTrogMaxNumber=%d", 
                            pProvMngm->TrogUsedCnt, pDDB->CRT.CfgParam.ProviderTrogMaxNumber);
                        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                    }
                }
            }
        }
    }

    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTCheckConsumerControlParam                       +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB        +*/
/*+                             EDD_UPPER_CSRT_CONSUMER_CONTROL_PTR_TYPE    pRqbParam   +*/
/*+                             LSA_UINT16                                  *pDgIdx     +*/
/*+                                                                                     +*/
/*+  Result                :    EDD_RSP                                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         (in)    +*/
/*+  pRqbParam      : pointer to RQB parameter from user (UPPER-RQB)            (in)    +*/
/*+  pDgIdx         : returns DG_Idx if DFP-Consumer (reduce searching)         (out)   +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks the Consumer-Control parameter.                  +*/
/*+  NOTE:        pDgIdx must be valid for DFP-Consumer, because no PTR-check !!!       +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckConsumerControlParam(
    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB,
    EDD_UPPER_CSRT_CONSUMER_CONTROL_PTR_TYPE    pRqbParam,
    LSA_UINT16                                  *pDgIdx)
{
    EDD_RSP                                 Response;
    EDDP_CRT_CONSUMER_PTR_TYPE              pConsumer;
#if defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)
    LSA_UINT16                              CRNumber = EDD_CRNUMBER_UNDEFINED;
#endif
    LSA_UINT16                              CycleReductionRatio, CyclePhase;
    LSA_UINT8                               ForwardMode;
    LSA_UINT8                               AppGroup;
    LSA_UINT8                               SFPosition;
    LSA_UINT32                              SFOffset, DataOffset;
    LSA_UINT16                              DataLenPDIRData, CycleReductionRatioPDIRData, CyclePhasePDIRData;
    LSA_BOOL                                ConsActivating;
    EDDP_PRM_PDIRSUBFRAMEDATA_SUBFRAMEBLOCK_SUBFRAMEDATA_PTR_TYPE  pSubFrameData = LSA_NULL;

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCheckConsumerControlParam(pRqbParam=0x%X, ConsumerID=%d)", 
        pRqbParam, pRqbParam->ConsumerID);

    Response                    = EDD_STS_OK;
    pConsumer                   = &pDDB->CRT.Cons.pConsEntry[pRqbParam->ConsumerID];
    CycleReductionRatioPDIRData = EDD_CYCLE_REDUCTION_RATIO_UNDEFINED;
    CyclePhasePDIRData          = EDD_CYCLE_PHASE_UNDEFINED;
	DataOffset                  = EDD_DATAOFFSET_UNDEFINED;

    if (pRqbParam->Mode & EDD_CONS_CONTROL_MODE_ACTIVATE)
    {   ConsActivating = LSA_TRUE;  }
    else
    {   ConsActivating = LSA_FALSE; }

    if (pRqbParam->ConsumerID >= pDDB->CRT.CfgParam.ConsumerMaxNumber)
    {   // ***** invalid Index (ConsumerID)
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> ConsumerID(%d) is out of range! ConsumerID-Max=%d", 
            pRqbParam->ConsumerID, (pDDB->CRT.CfgParam.ConsumerMaxNumber-1));
        return (Response);
    }

    if (pDDB->CRT.Cons.ConsumerUsedList.BeginIdx == EDDP_CRT_CHAIN_IDX_END)
    {   // ***** no Consumer in list !!!!
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> There is no used consumer!");
        return (Response);
    }

    // ***** check state of selected Consumer
    if (pConsumer->State == EDDP_CRT_STATE_FREE)
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> Consumer is not used!");
        return (Response);
    }

    // ***** check AppGroup of selected Consumer
    if( (EDDP_HAL_HW_HERA == EDDP_HALGetChipVersion(pDDB)) &&
        (0 != pRqbParam->IOParams.AppGroup)
      )
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,
                    "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> With chip version HERA only AppGroup of 0 is allowed! AppGroup(%d)",
                    pRqbParam->IOParams.AppGroup);
        return (Response);
    }
    if (ConsActivating)
    {
        // ***** if FrameID is in IRT range: consumer DataLen must be identical with DataLen in PDIRData
        if (EDDP_IS_IRT_FRAMEID(pConsumer->Param.FrameID))
        {
            if (pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_DFP_ITEM)
            {
                /* -------------------------------------------- */
                /* DFP (PDIRSubframeData and PDIRData)          */
                /* must be present for activate                 */
                /* -------------------------------------------- */
                if ( !(EDDP_PRMPDIRSubframeDataIsDfpPresent(pDDB, LSA_TRUE)) )    // PDIRSubframeData_A
                {
                    Response = EDD_STS_ERR_SEQUENCE;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> activate DFP-Consumer but DFP is not present in PRM!");
                    return (Response);
                }
                
                // ***** pConsumer->Param.DataLen for DFP will be checked some line later

                /* -------------------------------------------- */
                /* is Consumer.FrameID in PDIRData present?     */
                /* -------------------------------------------- */
                if ( !(EDDP_PRMPDIRDataIsFrameIDPresent(pDDB, pConsumer->Param.FrameID, LSA_FALSE, &CycleReductionRatioPDIRData, &CyclePhasePDIRData, LSA_TRUE)) )
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> The DFP-Consumer.FrameID(0x%X) is not found at all or not found with the expected role in PDIRData-Record!", 
                        pConsumer->Param.FrameID);
                    return (Response);
                }

                /* -------------------------------------------- */
                /* port parameter                               */
                /* -------------------------------------------- */

                // check CycleReductionRatio in consumer and in PDIRData
                if (pConsumer->Param.CycleReductionRatio == EDD_CYCLE_REDUCTION_RATIO_UNDEFINED)
                    CycleReductionRatio = pRqbParam->CycleReductionRatio;
                else
                    CycleReductionRatio = pConsumer->Param.CycleReductionRatio;

                if (CycleReductionRatio != CycleReductionRatioPDIRData)
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> CycleReductionRatio(%d) of DFP-Consumer missmatchs PDIRData.CycleReductionRatio(%d)", 
                        CycleReductionRatio, CycleReductionRatioPDIRData);
                    return (Response);
                }
            }
            else
            {
                /* -------------------------------------------- */
                /* PDIRData must be present for activate        */
                /* -------------------------------------------- */
                if ( !(EDDP_PRMPDIRDataIsPDIRDataPresent(pDDB, LSA_TRUE)) )    // PDIRData_A
                {
                    Response = EDD_STS_ERR_SEQUENCE;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> activate RTC3 consumer but PDIRData is not present!");
                    return (Response);
                }

                /* -------------------------------------------- */
                /* is Consumer.FrameID in PDIRData present?     */
                /* -------------------------------------------- */
                if ( !(EDDP_PRMPDIRDataIsFrameIDPresent(pDDB, pConsumer->Param.FrameID, LSA_FALSE, &CycleReductionRatioPDIRData, &CyclePhasePDIRData, LSA_TRUE)) )
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> The Consumer.FrameID(0x%X) is not found at all or not found with the expected role in PDIRData-Record!", 
                        pConsumer->Param.FrameID);
                    return (Response);
                }
                
                /* -------------------------------------------- */
                /* port parameter                               */
                /* -------------------------------------------- */

                // check DataLen in consumer and in PDIRData (LSA_TRUE: search in active record buffer)
                DataLenPDIRData = EDDP_PRMPDIRDataGetDataLen (pDDB, pConsumer->Param.FrameID, LSA_TRUE);

                if (pConsumer->Param.DataLen != DataLenPDIRData)
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> DataLen(%d) missmatchs PDIRData.DataLength(%d)", 
                        pConsumer->Param.DataLen, DataLenPDIRData);
                    return (Response);
                }

                // check CycleReductionRatio in consumer and in PDIRData
                if (pConsumer->Param.CycleReductionRatio == EDD_CYCLE_REDUCTION_RATIO_UNDEFINED)
                    CycleReductionRatio = pRqbParam->CycleReductionRatio;
                else
                    CycleReductionRatio = pConsumer->Param.CycleReductionRatio;

                if (CycleReductionRatio != CycleReductionRatioPDIRData)
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> CycleReductionRatio(%d) missmatchs PDIRData.CycleReductionRatio(%d)", 
                        CycleReductionRatio, CycleReductionRatioPDIRData);
                    return (Response);
                }

                // check CyclePhase in consumer and in PDIRData (for LEGACY-consumer)
                if (pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_2CR_MODE)
                {
                    if (pConsumer->Param.CyclePhase == EDD_CYCLE_PHASE_UNDEFINED)
                        CyclePhase = pRqbParam->CyclePhase;
                    else
                        CyclePhase = pConsumer->Param.CyclePhase;

                    if (CyclePhase != CyclePhasePDIRData)
                    {
                        Response = EDD_STS_ERR_PARAM;
                        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> CyclePhase(%d) missmatchs PDIRData.CyclePhase(%d)", 
                            CyclePhase, CyclePhasePDIRData);
                        return (Response);
                    }
                }
            }
        }
    }

    // ConsumerID
    // Mode
    // Follow combinations of multiple bits in Mode are not permitted:
    // ==============================================================)
    // MODE_ACTIVATE and MODE_PASSIVATE together not permitted --> ERR_PARAM	   
    // MODE_PROV_AUTOSTOP_ENABLE and MODE_PROV_AUTOSTOP_DISABLE together not permitted --> ERR_PARAM	

    // ***** MODE_ACTIVATE and MODE_PASSIVATE together not permitted --> ERR_PARAM
    if ((pRqbParam->Mode & (EDD_CONS_CONTROL_MODE_PASSIVATE | EDD_CONS_CONTROL_MODE_ACTIVATE)) == 
         (EDD_CONS_CONTROL_MODE_PASSIVATE | EDD_CONS_CONTROL_MODE_ACTIVATE))
    {   // ***** ACTIVATE + PASSIVATE invalid combination !!!
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> MODE_ACTIVATE and MODE_PASSIVATE!");
        return (Response);
    }
    // ***** MODE_PASSIVATE and MODE_SET_TO_UNKNOWN together not permitted --> ERR_PARAM
    if ((pRqbParam->Mode & (EDD_CONS_CONTROL_MODE_PASSIVATE | EDD_CONS_CONTROL_MODE_SET_TO_UNKNOWN)) == 
         (EDD_CONS_CONTROL_MODE_PASSIVATE | EDD_CONS_CONTROL_MODE_SET_TO_UNKNOWN))
    {   // ***** SET_TO_UNKNOWN + PASSIVATE invalid combination !!!
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> MODE_SET_TO_UNKNOWN and MODE_PASSIVATE!");
        return (Response);
    }

    // check SrcMAC, SrcIP only if current consumer state is passive and (Mode=ACTIVATE)
    if ((pConsumer->State != EDDP_CRT_STATE_HW_ACTIVE) && (ConsActivating))
    {
        if (pConsumer->Param.RTClass == EDDP_CRT_RTCLASS_RT)
        {
            // RTClass1/2/3 consumer
            // only UniCast SrcMAC is allowed
            if (EDDP_IS_MC_MAC(pRqbParam->SrcMAC))
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_07(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(ConsumerID=%d): ERROR -> SrcMAC(%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X) is not a UniCast address!", 
                    pRqbParam->ConsumerID, 
                    pRqbParam->SrcMAC.MacAdr[0], 
                    pRqbParam->SrcMAC.MacAdr[1], 
                    pRqbParam->SrcMAC.MacAdr[2], 
                    pRqbParam->SrcMAC.MacAdr[3], 
                    pRqbParam->SrcMAC.MacAdr[4], 
                    pRqbParam->SrcMAC.MacAdr[5]);
                return (Response);
            }
        }
        else
        {
            // RToverUDP consumer
            // IP(FF:FF:FF:FF) is not allowed
            if (EDDP_IS_BC_IP(pRqbParam->SrcIP))
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_05(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(ConsumerID=%d): ERROR -> SrcIP(%d:%d:%d:%d) is invalid!", 
                    pRqbParam->ConsumerID, 
                    pRqbParam->SrcIP.IPAdr[0], 
                    pRqbParam->SrcIP.IPAdr[1], 
                    pRqbParam->SrcIP.IPAdr[2], 
                    pRqbParam->SrcIP.IPAdr[3]);
                return (Response);
            }
        }
    }

    // ***** MODE_PROV_AUTOSTOP_ENABLE and MODE_PROV_AUTOSTOP_DISABLE together not permitted --> ERR_PARAM
    if ((pRqbParam->Mode & (EDD_CONS_CONTROL_MODE_PROV_AUTOSTOP_ENABLE | EDD_CONS_CONTROL_MODE_PROV_AUTOSTOP_DISABLE) ) ==
         (EDD_CONS_CONTROL_MODE_PROV_AUTOSTOP_ENABLE | EDD_CONS_CONTROL_MODE_PROV_AUTOSTOP_DISABLE) )
    {   // ***** MODE_AUTOSTOP_ENABLE + MODE_PROV_AUTOSTOP_DISABLE invalid combination !!!
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> MODE_PROV_AUTOSTOP_ENABLE and MODE_PROV_AUTOSTOP_DISABLE!");
        return (Response);
    }
    else
    {
        if (pRqbParam->Mode & EDD_CONS_CONTROL_MODE_PROV_AUTOSTOP_ENABLE)
        {   // ***** expect provider with our ConsumerID to enable ProviderAutoStop !
            if ( !(EDDP_CRTCheckProvAutoStopReference(pDDB, pRqbParam->ConsumerID)) )
            {   // ***** PPM found no provider for this ConsumerID --> print a warning into the trace !!!
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCheckConsumerControlParam(): MODE_PROV_AUTOSTOP_ENABLE but there is no AutoStop-Provider for this ConsumerID(%d)!", 
                    pRqbParam->ConsumerID);
            }
        }
    }

    // ***** UNDEF not allowed if activating the consumer !!!
    if (ConsActivating)
    {
        // CycleReductionRatio used in EDDP for TSBRR-Reload within the TimerScoreBoard !!!
        if (pConsumer->Param.CycleReductionRatio == EDD_CYCLE_REDUCTION_RATIO_UNDEFINED)
        {   CycleReductionRatio = pRqbParam->CycleReductionRatio;           }
        else
        {   CycleReductionRatio = pConsumer->Param.CycleReductionRatio;     }

        if (pConsumer->Param.RTClass == EDDP_CRT_RTCLASS_UDP)
        {
            Response = EDDP_CRTCheckRangeOfCycleReductionRatio (pDDB, EDDP_CRT_RTCLASS_UDP, pConsumer->Param.Properties, CycleReductionRatio, LSA_FALSE);    // UNDEF not allowed
        }
        else
        {
            Response = EDDP_CRTCheckRangeOfCycleReductionRatio (pDDB, EDDP_CRT_RTCLASS_RT, pConsumer->Param.Properties, CycleReductionRatio, LSA_FALSE);     // UNDEF not allowed
        }

        if (Response != EDD_STS_OK)
        {
            return (Response);
        }
    }

    // CyclePhase (not used in EDDP !!!)

    if (ConsActivating)
    {
        // IOParams.BufferProperties
        // IOParams.CRNumber
        
#if defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)
    //In case of ERTEC200+ the CR Number should be unique
    if( EDDP_HAL_HW_ERTEC200P == EDDP_HALGetChipVersion(pDDB) )
    {        
        if ( pConsumer->Param.CRNumber == EDD_CRNUMBER_UNDEFINED )
        { CRNumber = pRqbParam->IOParams.CRNumber; }
        else
        { CRNumber = pConsumer->Param.CRNumber;    }

        if (CRNumber == EDD_CRNUMBER_UNDEFINED)
        {   // ***** CRNumber not set
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> CRNumber(%d) is undefined!", 
                CRNumber);
            return (Response);
        }
    }
#endif // defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)
    
        // ***** Test Range later depending on ForwardMode

        // IOParams.AppGroup
        if ( pConsumer->Param.AppGroup == EDD_APPGROUP_UNDEFINED )
        { AppGroup = pRqbParam->IOParams.AppGroup; }
        else
        { AppGroup = pConsumer->Param.AppGroup;    }

        if ((AppGroup == EDD_APPGROUP_UNDEFINED) || (AppGroup > EDDP_CRT_CONS_MAX_APPGROUP))
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> AppGroup(%d) is out of range: 0..%d!",
                AppGroup, EDDP_CRT_CONS_MAX_APPGROUP);
            return (Response);
        }

        // IOParams.SFPosition
        if (pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_DFP_ITEM)
        {
            if (pConsumer->Param.SFPosition == EDD_SFPOSITION_UNDEFINED)
            {  SFPosition = pRqbParam->IOParams.SFPosition; }
            else
            {  SFPosition = pConsumer->Param.SFPosition;    }

            if ((SFPosition == EDD_SFPOSITION_UNDEFINED) || !EDDP_CRT_IS_DFP_SFPOSITION_IN_VALID_RANGE(SFPosition))
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> SFPosition(%d) is out of range or UNDEFINED", 
                    SFPosition);
                return (Response);
            }

            *pDgIdx = EDDP_CRTConsumerGetConsDgEntry (pDDB, pConsumer, SFPosition);

            if (*pDgIdx == EDDP_CRT_CHAIN_IDX_END)
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> no matching of SFPosition(%d) within PRM", 
                    SFPosition);
                return (Response);
            }

            pSubFrameData = pDDB->CRT.Cons.pConsDgEntry[*pDgIdx].pSubFrameData;

            if (pSubFrameData == LSA_NULL)
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> no pSubFrameData for SFPosition(%d)/DgIdx:%d", 
                    SFPosition, *pDgIdx);
                return (Response);
            }

            if (pSubFrameData->DataLength != pConsumer->Param.DataLen)
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> SFPosition(%d) with SFDataLength:%d, Consumer with DataLength:%d !!!", 
                    SFPosition, pSubFrameData->DataLength, pConsumer->Param.DataLen);
                return (Response);
            }
        }

        // IOParams.ForwardMode + CutThrough_OutDataStart
        if (pConsumer->Param.ForwardMode == EDD_FORWARDMODE_UNDEFINED)
        {  ForwardMode = pRqbParam->IOParams.ForwardMode; }
        else
        {  ForwardMode = pConsumer->Param.ForwardMode;    }
      
       if (ForwardMode == EDD_FORWARDMODE_UNDEFINED)
       {   // ***** ForwardMode not set
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> ForwardMode not set!");
            return (Response);
        }
        if (ForwardMode == EDD_FORWARDMODE_CT) 
        {
#if 1
            // cut-through mode is not allowed any longer (see TFS-RQ 1193172)
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> CutThrough-Mode is not allowed any longer");
            return (Response);
#else
            // if cut-through mode shall be used again, reinsert this code
            if ((pDDB->CRT.Cons.CT_ConsumerID == pRqbParam->ConsumerID) || (pDDB->CRT.Cons.CT_ConsumerID == EDDP_CRT_CHAIN_IDX_END))
            {
                // expect no CT is set or this Consumer is the CT itself
            }
            else
            {   
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> one cunsumer with CutThrough-Mode is already added -> CT_ConsumerID=%d", 
                    pDDB->CRT.Cons.CT_ConsumerID);
                return (Response);
            }

            if (CRNumber != EDD_CRNUMBER_NOT_USED)
            {   // ***** only allowed with SDRAM (EDD_CRNUMBER_NOT_USED)
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> CRNumber(%d) in CT mode expected with value(%d)", 
                    CRNumber, EDD_CRNUMBER_NOT_USED);
                return (Response);
            }

            if (!(pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_DP2ACW))
            {   // ***** not RTC3 consumer !!!
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> CT is only with RTC3 allowed. ConsumerFlags(0x%X)", 
                    pConsumer->ConsumerFlags);
                return (Response);
            }

            if (EDDP_IS_IRT_REDUNDANT_FRAMEID(pConsumer->Param.FrameID))
            {   // ***** not allowed with redundant FrameID
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> CT is not allowed with redundant FrameID (0x%X)", 
                    pConsumer->Param.FrameID);
                return (Response);
            }

            // IOParams.CutThrough_OutDataStart
            if (pRqbParam->IOParams.CutThrough_OutDataStart > EDDP_CRT_MAX_CUTTHROUGH_OUTDATASTART)
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> CT_OutDataStart(0x%X) is out of range [0..0x%X]", 
                    pRqbParam->IOParams.CutThrough_OutDataStart, EDDP_CRT_MAX_CUTTHROUGH_OUTDATASTART);
                return (Response);
            }
#endif
        }
        else
        {   // ***** EDD_FORWARDMODE_SF
#if defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)			
            if((pDDB->Hardware == EDDP_HW_ERTEC200P) || (pDDB->Hardware == EDDP_HW_ERTEC200P_FPGA))
            {
                if (!EDDP_CRT_IS_CRNUMBER_IN_PERIF_RANGE(CRNumber))
                {   
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> illegal CRNumber(%d) for Perif ",
                        pRqbParam->IOParams.CRNumber);
                    return (Response);
                }
            }
#endif
        }

        // parameter IOParams.SFOffset
        if (pConsumer->ConsumerFlags & EDDP_CONS_FLAG_IS_DFP_ITEM)
        {
            // activate this part later when SFOffset can be verified
            if (pConsumer->Param.SFOffset == EDD_SFOFFSET_UNDEFINED)
            {
                SFOffset = pRqbParam->IOParams.SFOffset;
            }
            else
            {
                SFOffset = pConsumer->Param.SFOffset;
            }

            if (SFOffset == EDD_SFOFFSET_UNDEFINED)
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> SFOffset=INVALID");
                return (Response);
            }

            EDDP_ASSERT_NO_NULL_PTR(pSubFrameData);
            if (pSubFrameData->Offset != SFOffset)
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> SFOffset:%d != expected Offs:%d",
                    SFOffset, pSubFrameData->Offset);
                return (Response);
            }
        }
        
#if defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)
        if( EDDP_HAL_HW_ERTEC200P == EDDP_HALGetChipVersion(pDDB) )
        {
            // parameter IOParams.DataOffset
            if (pConsumer->Param.DataOffset == EDD_DATAOFFSET_UNDEFINED)
            {
                DataOffset = pRqbParam->IOParams.DataOffset;
            }
            else
            {
                DataOffset = pConsumer->Param.DataOffset;
            }
        }
#endif             
        if (  (DataOffset == EDD_DATAOFFSET_INVALID)
            ||(DataOffset == EDD_DATAOFFSET_UNDEFINED)
           )
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> DataOffset=0x%X",
                              DataOffset);
            return (Response);
        }

#if defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)
        if( EDDP_HAL_HW_ERTEC200P == EDDP_HALGetChipVersion(pDDB) )  
        {
        // check 8 byte alignment of IOParams.DataOffset
            if (   (EDDP_CRT_IS_CRNUMBER_IN_PERIF_RANGE(CRNumber))
                && (DataOffset != EDD_DATAOFFSET_UNDEFINED)
               )
            {
                // this consumer uses PERIF: check 8 byte alignment of DataOffset (3 bits = 0)
                if ( !(EDDP_GlbIsAligned(pDDB, DataOffset, EDDP_GLB_ALIGNMENT_8_BYTE)) )
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> DataOffset(0x%X) for PERIF is not 8 byte aligned!", 
                        DataOffset);
                    return (Response);
                }
            }
        }
#endif

        // check "pIOBaseAdrPerif" and "pIOBaseAdrExtRam" on Activate
        if (ForwardMode == EDD_FORWARDMODE_CT)
        {
            // CutThrough-Consumer: uses extarnel RAM
            if (LSA_HOST_PTR_ARE_EQUAL(pDDB->CRT.pIOBaseAdrExtRam, LSA_NULL))
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> pIOBaseAdrExtRam=NULL for CT-Consumer on Activate!");
                return (Response);
            }
        }
        else
        {
#if defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)
            if((pDDB->Hardware == EDDP_HW_ERTEC200P) || (pDDB->Hardware == EDDP_HW_ERTEC200P_FPGA))
            {
                // not CutThrough-Consumer: uses PERIF
                if (LSA_HOST_PTR_ARE_EQUAL(pDDB->CRT.pIOBaseAdrPerif, LSA_NULL))
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                    EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> pIOBaseAdrPerif=NULL on Activate!");
                    return (Response);
                 }
            }
#endif       
        }
    }

    /* ---------------------------------------------------- */
    /* is SENDCLOCK_CHANGE current in processing ?          */
    /* ---------------------------------------------------- */
    if ((Response == EDD_STS_OK) && (ConsActivating))
    {
        if (EDDP_CMD_Is_Command_Running(pDDB, EDDP_CMD_GSYNC_UPDATE_COMCYCL))
        {
            Response = EDD_STS_ERR_SEQUENCE;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> Service EDD_SRV_SENDCLOCK_CHANGE is current in processing on Activate");
        }
    }

	/* ---------------------------------------------------- */
	/* Check "CRNumber" */
	/* ---------------------------------------------------- */
#if defined(EDDP_CFG_HW_ERTEC200P_SUPPORT)
    //In case of ERTEC200+ the CR Number should be unique
    if( (pDDB->Hardware == EDDP_HW_ERTEC200P) || (pDDB->Hardware == EDDP_HW_ERTEC200P_FPGA) )
    {
        if (Response == EDD_STS_OK)
        {
            if (    ( pConsumer->Param.CRNumber == EDD_CRNUMBER_UNDEFINED ) 
                 && ( pRqbParam->Mode & EDD_CONS_CONTROL_MODE_ACTIVATE )
               )
            {
                if ( pRqbParam->IOParams.CRNumber != EDD_CRNUMBER_UNDEFINED )
                {
                    LSA_UINT16 ConsIdx;
            
                    ConsIdx = pDDB->CRT.Cons.ConsumerUsedList.BeginIdx;
                    // loop through all used consumers
                    while (ConsIdx != EDDP_CRT_CHAIN_IDX_END)
                    {
                        EDDP_ASSERT_FALSE(ConsIdx >= pDDB->CRT.CfgParam.ConsumerMaxNumber);

                        if (pDDB->CRT.Cons.pConsEntry[ConsIdx].Param.CRNumber != EDD_CRNUMBER_UNDEFINED) 
                        {
                            if(pDDB->CRT.Cons.pConsEntry[ConsIdx].Param.CRNumber == pRqbParam->IOParams.CRNumber)
                            {
                                Response = EDD_STS_ERR_PARAM;
                                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> CRNumber (%d) already in use!", 
                                                   pRqbParam->IOParams.CRNumber);
                                break;
                            }
                        }
                        ConsIdx = pDDB->CRT.Cons.pConsEntry[ConsIdx].UsedFreeLink.Next;
                    }
                }
            }
        }
    }
#endif

    /* ---------------------------------------------------- */
    /* check IO_Configured and FeedInLoadLimitation         */
    /* ---------------------------------------------------- */
    if (pRqbParam->Mode & EDD_CONS_CONTROL_MODE_ACTIVATE)
    {
        if (EDD_STS_OK == Response)
        {
            Response = EDDP_NRTFillIsIOAllowed(pDDB);
            if(EDD_STS_ERR_NOT_ALLOWED == Response)
            {
                Response = EDD_STS_ERR_SERVICE;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,
                            "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> MODE_ACTIVATE not allowed because of IO_Configured=FALSE");
            }
        }

        if (EDD_STS_OK == Response)
        {
            Response = EDDP_NRTFillIsFeedInLoadLimitationActive(pDDB);
            if(EDD_STS_ERR_NOT_ALLOWED == Response)
            {
                Response = EDD_STS_ERR_SERVICE;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,
                            "[H:--] EDDP_CRTCheckConsumerControlParam(): ERROR -> MODE_ACTIVATE not allowed because of FeedInLoadLimitation=FALSE");
            }
        }
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTCheckConsumerRemoveParam                        +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB            +*/
/*+                             EDD_UPPER_CSRT_CONSUMER_REMOVE_PTR_TYPE pRqbParam       +*/
/*+                                                                                     +*/
/*+  Result                :    EDD_RSP                                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         (in)    +*/
/*+  pRqbParam      : pointer to RQB parameter from user (UPPER-RQB)            (in)    +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks the Consumer-Remove parameter.                   +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckConsumerRemoveParam(
    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB,
	EDD_UPPER_CSRT_CONSUMER_REMOVE_PTR_TYPE     pRqbParam)
{
    EDD_RSP             Response;

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCheckConsumerRemoveParam(pRqbParam=0x%X, ConsumerID=%d)", 
        pRqbParam, pRqbParam->ConsumerID);

    Response = EDD_STS_OK;

    if (pRqbParam->ConsumerID >= pDDB->CRT.CfgParam.ConsumerMaxNumber)
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerRemoveParam(): ERROR -> ConsumerID(%d) is out of range, ConsumerID-Max=%d", 
            pRqbParam->ConsumerID, (pDDB->CRT.CfgParam.ConsumerMaxNumber-1));
    }
    else
    {
        if (pDDB->CRT.Cons.pConsEntry[pRqbParam->ConsumerID].State == EDDP_CRT_STATE_FREE)
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerRemoveParam(): ERROR -> Consumer is not used!");
        }
        else
        {
            if (EDDP_CRTCheckProvAutoStopReference(pDDB, pRqbParam->ConsumerID))
            {
                // PPM found connected provider for this ConsumerID !!!
                Response = EDD_STS_ERR_SEQUENCE;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckConsumerRemoveParam(): ERROR -> found connected AutoStop-Provider!");
            }
        }
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTCheckGetAPDUStatusParam                         +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                 pDDB            +*/
/*+                             EDD_UPPER_CSRT_GET_APDU_STATUS_PTR_TYPE pRqbParam       +*/
/*+                             EDD_SERVICE                             Service         +*/
/*+                                                                                     +*/
/*+  Result                :    EDD_RSP                                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         (in)    +*/
/*+  pRqbParam      : pointer to RQB parameter from user (UPPER-RQB)            (in)    +*/
/*+  Service        : EDD-Service                                               (in)    +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks the GetAPDUStatus parameter.                     +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckGetAPDUStatusParam(
    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB,
	EDD_UPPER_CSRT_GET_APDU_STATUS_PTR_TYPE     pRqbParam,
	EDD_SERVICE                                 Service)
{
    EDD_RSP             Response;

#if (EDDP_CFG_TRACE_MODE == 0)
    LSA_UNUSED_ARG(Service);
#endif

    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CRTCheckGetAPDUStatusParam(pRqbParam=0x%X, Service=0x%X)", 
        pRqbParam, Service);

    Response = EDD_STS_OK;

    switch (pRqbParam->Type)
    {
        /* -------------------- */
        /* Provider             */
        /* -------------------- */
        case EDD_CSRT_TYPE_PROVIDER:
        {
            if (pRqbParam->ID >= pDDB->CRT.CfgParam.ProviderMaxNumber)
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckGetAPDUStatusParam(): ERROR -> ProviderID(%d) is out of range, ProviderID-Max=%d", 
                    pRqbParam->ID, (pDDB->CRT.CfgParam.ProviderMaxNumber-1));
            }
            else
            {
                if (pDDB->CRT.Prov.pProvEntry[pRqbParam->ID].State == EDDP_CRT_STATE_FREE)
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
	                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckGetAPDUStatusParam(): ERROR -> Provider is not used!");
                }
            }
        }
        break;

        /* -------------------- */
        /* Consumer             */
        /* -------------------- */
        case EDD_CSRT_TYPE_CONSUMER:
        {
            if (pRqbParam->ID >= pDDB->CRT.CfgParam.ConsumerMaxNumber)
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckGetAPDUStatusParam(): ERROR -> ConsumerID(%d) is out of range, ConsumerID-Max=%d", 
                    pRqbParam->ID, (pDDB->CRT.CfgParam.ConsumerMaxNumber-1));
            }
            else
            {
                if (pDDB->CRT.Cons.pConsEntry[pRqbParam->ID].State == EDDP_CRT_STATE_FREE)
                {
                    Response = EDD_STS_ERR_PARAM;
                    EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
	                EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckGetAPDUStatusParam(): ERROR -> Consumer is not used!");
                }
            }
        }
        break;

        /* -------------------- */
        /* Unknown Type         */
        /* -------------------- */
        default:
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckGetAPDUStatusParam(): ERROR -> Type(0x%X) invalid", 
                pRqbParam->Type);
        }
        break;
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTCheckProvAutoStopReference                      +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                        +*/
/*+                             LSA_UINT16                  ConsumerID                  +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_BOOL                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  ConsumerID     : Index in consumer array                                           +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks the AutoStop reference to provider(s)            +*/
/*+               of given consumer.                                                    +*/
/*+               ConsumerID must be valid - no check.                                  +*/
/*+                                                                                     +*/
/*+               Return LSA_TRUE  : one or more provider with reference to this consumer+*/
/*+                      LSA_FALSE : no provider with references to this consumer       +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckProvAutoStopReference(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT16                  ConsumerID)
{
    LSA_BOOL        State;

    EDDP_ASSERT_FALSE(ConsumerID >= pDDB->CRT.CfgParam.ConsumerMaxNumber);

    if (pDDB->CRT.Prov.pASGroupEntry[ConsumerID].ProviderCnt > 0)
        State = LSA_TRUE;
    else
        State = LSA_FALSE;

    return (State);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTCheckAndGetProviderPortParam                    +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB        +*/
/*+                             EDD_UPPER_CSRT_PROVIDER_CONTROL_PTR_TYPE    pRqbParam   +*/
/*+                                                                                     +*/
/*+  Result                :    EDD_RSP                                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                                     +*/
/*+  pRqbParam  : Pointer to RQB parameter from user (UPPER-RQB)                        +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks and gets port parameter of RTClass3 provider.    +*/
/*+               If this provider has not yet port information, get it from PRM.       +*/
/*+               and copy all PortInfo to ACWs.                                        +*/
/*+                                                                                     +*/
/*+               ProviderID is in given pRqbParam.                                     +*/
/*+                                                                                     +*/
/*+               Check whether this provider has reserved enough ACWs. If more         +*/
/*+               valid FrameSendOffset(s) in PortInfoArray than ACWs from PROVIDER_ADD +*/
/*+               so get new ACWs from AcwUsedList.                                     +*/
/*+                 -> If no more free ACW in AcwUsedList, ERR_RESOURCE!                +*/
/*+                                                                                     +*/
/*+               Call this function only if:                                           +*/
/*+                 - this provider is not DFP-Provider                                 +*/
/*+                 - this provider is RED (only RTClass3 provider have PortInfo)       +*/
/*+                 - first ACW of this provider is RED                                 +*/
/*+                 - this provider is activated here (change from PASSIVE to ACTIVE)   +*/
/*+               if no, FATAL!                                                         +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckAndGetProviderPortParam(
    EDDP_LOCAL_DDB_PTR_TYPE                     pDDB,
    EDD_UPPER_CSRT_PROVIDER_CONTROL_PTR_TYPE    pRqbParam)
{
    EDD_RSP                                 Response;
    LSA_UINT16                              ProviderID, NewAcwID, AcwIdx;
    LSA_UINT32                              i;
    LSA_UINT32                              AcwCntPortInfo;
    LSA_UINT16                              CycleReductionRatioTemp;
    LSA_UINT16                              CyclePhaseTemp;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;
    EDDP_CRT_PROVIDER_PTR_TYPE              pProvider;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcw;
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcwNew;    // if this provider needs new ACW
    EDDP_CRT_PROVIDER_ACW_PTR_TYPE          pAcwFirst;  // first ACW in AcwProvLink

    Response        = EDD_STS_OK;
    pProvMngm    = &pDDB->CRT.Prov;
    ProviderID      = pRqbParam->ProviderID;
    EDDP_ASSERT_FALSE(ProviderID >= pDDB->CRT.CfgParam.ProviderMaxNumber);
    pProvider       = &pProvMngm->pProvEntry[ProviderID];

    // is this DFP-Provider ?
    if (pProvider->ProviderFlags & EDDP_CRT_PROV_FLAGS_IS_DFP)
    {
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCheckAndGetProviderPortParam(ProviderID=%d): FATAL -> This function is not allowed for DFP-Provider! ProviderFlags(0x%X)", 
            ProviderID, pProvider->ProviderFlags);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    // is this RED Provider ?
    if (pProvider->ROGPhase != EDDP_CRT_PROV_ACW_PHASE_R)
    {
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCheckAndGetProviderPortParam(ProviderID=%d): FATAL -> ROGPhase(0x%X) of this provider is not red!", 
            ProviderID, pProvider->ROGPhase);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    EDDP_ASSERT_FALSE(pProvider->AcwList.BeginIdx == EDDP_CRT_CHAIN_IDX_END);
    pAcw = &pProvMngm->pAcwEntry[pProvider->AcwList.BeginIdx];

    // first ACW of this provider must always be red ACW
    if (pAcw->ROGPhase != EDDP_CRT_PROV_ACW_PHASE_R)
    {
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCheckAndGetProviderPortParam(ProviderID=%d): FATAL -> RTC3 provider but first ACW-Phase is not red! ROGPhase=0x%X", 
            ProviderID, pAcw->ROGPhase);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    // only if this provider is activated and first ACW is not yet active
    if (   (pRqbParam->Mode & EDD_PROV_CONTROL_MODE_ACTIVATE)
        && (!(pAcw->State & EDDP_CRT_PROV_ACW_STATE_ACTIVE_MASK))
       )
    {
        // ok
    }
    else
    {
        EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCheckAndGetProviderPortParam(ProviderID=%d): FATAL -> This function is only allowed if this provider is activated and first ACW is not yet active! Mode=0x%X, FirstAcw-State=0x%X", 
            ProviderID, pRqbParam->Mode, pAcw->State);
        EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
    }

    // has this first ACW valid PortInfo ?
    if (pAcw->AddParam.FrameSendOffset == EDDP_CRT_FRAME_SEND_OFFSET_UNDEFINED)
    {
        /* -------------------------------------------- */
        /* get PortInfo from PRM                        */
        /* -------------------------------------------- */
        if (pProvider->ProviderFlags & EDDP_CRT_PROV_FLAGS_IS_REDUNDANT)
        {
            Response = EDDP_PRMPDIRDataGetRTC3PortInfoTx (pDDB, 
                                pProvider->AddParam.FrameID, 
                                LSA_TRUE, 
                                LSA_TRUE, 
                                &pProvider->AddParam.PortInfo);
        }
        else
        {
            Response = EDDP_PRMPDIRDataGetRTC3PortInfoTx (pDDB, 
                                pProvider->AddParam.FrameID, 
                                LSA_FALSE, 
                                LSA_TRUE, 
                                &pProvider->AddParam.PortInfo);
        }

        if (Response != EDD_STS_OK)
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckAndGetProviderPortParam(): ERROR -> no PortInfo from PRM!");
        }
        else
        {
            // the following checks are not for RTC3-LEGACY-Provider
            if (pProvider->AuxAcwId == EDDP_CRT_CHAIN_IDX_END)
            {
                // check whether this provider has reserved enough ACWs
                // every valid FrameSendOffset needs one ACW
                // loop through all valid entries in PortInfoArray
                AcwCntPortInfo = 0;
                for ( i=0; i < EDDP_MAX_PORT_CNT; i++ )
                {
                    if (pProvider->AddParam.PortInfo.PortInfoArray[i].FrameSendOffset != EDDP_CRT_FRAME_SEND_OFFSET_UNDEFINED)
                        AcwCntPortInfo++;

                    if (pProvider->AddParam.PortInfo.PortInfoArray[i].MCFrameSendOffset != EDDP_CRT_FRAME_SEND_OFFSET_UNDEFINED)
                        AcwCntPortInfo++;
                }

                if (pProvider->AcwCnt > AcwCntPortInfo)
                {
                    EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCheckAndGetProviderPortParam(): FATAL -> RTC3 provider has allocated more ACWs than PortInfo! AcwCnt=%d, AcwCntPortInfo=%d", 
                        pProvider->AcwCnt, AcwCntPortInfo);
                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                }
                else
                {
                    if (AcwCntPortInfo > pProvider->AcwCnt)
                    {
                        // this provider needs more ACWs
                        AcwCntPortInfo = (AcwCntPortInfo - pProvider->AcwCnt);

                        if (AcwCntPortInfo > (pDDB->CRT.CfgParam.ProviderAcwMaxNumber - pProvMngm->AcwUsedCnt))
                        {
                            Response = EDD_STS_ERR_RESOURCE;
                            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                            EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckAndGetProviderPortParam(): ERROR -> No more free ACW! This provider needs additional %d ACW! ProviderUsedCnt=%d, AcwUsedCnt=%d", 
                                        AcwCntPortInfo, pProvMngm->ProviderUsedCnt, pProvMngm->AcwUsedCnt);
                        }
                        else
                        {
                            for ( i=0; i < AcwCntPortInfo; i++ )
                            {
                                NewAcwID = EDDP_CRTProvAcwAddToUsedList (pDDB);     // get NewAcwID

                                if (NewAcwID == EDDP_CRT_CHAIN_IDX_END)
                                {
                                    // this should have been stated above in resource check of "AcwCntPortInfo"
                                    EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCheckAndGetProviderPortParam(): FATAL -> No more free ACW! This provider needs additional %d ACW! ProviderUsedCnt=%d, AcwUsedCnt=%d", 
                                        AcwCntPortInfo, pProvMngm->ProviderUsedCnt, pProvMngm->AcwUsedCnt);
                                    EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
                                }
                                else
                                {
                                    pAcwNew     = &pProvMngm->pAcwEntry[NewAcwID];
                                    EDDP_ASSERT_FALSE(pProvider->AcwList.BeginIdx == EDDP_CRT_CHAIN_IDX_END);
                                    pAcwFirst   = &pProvMngm->pAcwEntry[pProvider->AcwList.BeginIdx];

                                    // add this new ACW to provider
                                    EDDP_CRTProvAcwAddToProvider (pDDB, pProvider->ProviderId, NewAcwID);
                                    // copy ACW specific parameter from prev. ACW of this provider
                                    pAcwNew->ROGPhase   = EDDP_CRT_PROV_ACW_PHASE_R;
                                    pAcwNew->AddParam   = pAcwFirst->AddParam;

                                    EDDP_CRT_TRACE_07(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTCheckAndGetProviderPortParam(): This provider needs one more ACW: NewAcwID=%d, FrameID=0x%X, RR=%d, Ph=%d, PhSeq=0x%X, FSO=0x%X, AuxAcwId=%d", 
                                        NewAcwID, pAcwNew->AddParam.FrameID, pAcwNew->AddParam.CycleReductionRatio, pAcwNew->AddParam.CyclePhase, pAcwNew->AddParam.CyclePhaseSequence, pAcwNew->AddParam.FrameSendOffset, pProvider->AuxAcwId);
                                }
                            }
                        }
                    }
                }
            }
        }

        /* -------------------------------------------- */
        /* now copy all PortInfo to ACWs                */
        /* -------------------------------------------- */
        if (Response == EDD_STS_OK)
        {
            AcwIdx = pProvider->AcwList.BeginIdx;

            // loop through all valid entries in PortInfoArray
            // every valid FrameSendOffset needs one ACW
            for ( i=0; i < EDDP_MAX_PORT_CNT; i++ )
            {
                /* ------------------------ */
                /* Standard Provider        */
                /* ------------------------ */
                if (pProvider->AddParam.PortInfo.PortInfoArray[i].FrameSendOffset != EDDP_CRT_FRAME_SEND_OFFSET_UNDEFINED)
                {
                    EDDP_ASSERT_FALSE(AcwIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

                    pAcw = &pProvMngm->pAcwEntry[AcwIdx];

                    // check ProviderADD.CycleReductionRatio against PDIRData.CycleReductionRatio
                    if (pAcw->AddParam.CycleReductionRatio == EDD_CYCLE_REDUCTION_RATIO_UNDEFINED)
                        CycleReductionRatioTemp = pRqbParam->CycleReductionRatio;
                    else
                        CycleReductionRatioTemp = pAcw->AddParam.CycleReductionRatio;

                    if (CycleReductionRatioTemp != pProvider->AddParam.PortInfo.PortInfoArray[i].ReductionRatio)
                    {
                        Response = EDD_STS_ERR_PARAM;
                        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckAndGetProviderPortParam(): ERROR -> ProviderADD.CycleReductionRatio(%d) != PDIRData.CycleReductionRatio(%d)", 
                            CycleReductionRatioTemp, pProvider->AddParam.PortInfo.PortInfoArray[i].ReductionRatio);
                        // exit FOR loop
                        break;
                    }

                    // check ProviderADD.CyclePhase against PDIRData.CyclePhase (for LEGACY-Provider)
                    if ((Response == EDD_STS_OK) && (pProvider->AuxAcwId != EDDP_CRT_CHAIN_IDX_END))
                    {
                        if (pAcw->AddParam.CyclePhase == EDD_CYCLE_PHASE_UNDEFINED)
                            CyclePhaseTemp = pRqbParam->CyclePhase;
                        else
                            CyclePhaseTemp = pAcw->AddParam.CyclePhase;

                        if (CyclePhaseTemp != pProvider->AddParam.PortInfo.PortInfoArray[i].Phase)
                        {
                            Response = EDD_STS_ERR_PARAM;
                            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckAndGetProviderPortParam(): ERROR -> ProviderADD.CyclePhase(%d) != PDIRData.CyclePhase(%d)", 
                                CyclePhaseTemp, pProvider->AddParam.PortInfo.PortInfoArray[i].Phase);
                            // exit FOR loop
                            break;
                        }
                    }

                    // copy PortInfo to ACW
                    if (Response == EDD_STS_OK)
                    {
                        pAcw->AddParam.FrameID          = pProvider->AddParam.PortInfo.PortInfoArray[i].FrameID;
                        pAcw->AddParam.TxPortID         = (LSA_UINT16) (i + 1); // user PortID = 1..4
                        pAcw->AddParam.FrameSendOffset  = pProvider->AddParam.PortInfo.PortInfoArray[i].FrameSendOffset;
                        // ACW has new FrameSendOffset, so chain this ACW within its provider
                        EDDP_CRTProvAcwChainSendOrderToProvider (pDDB, ProviderID, AcwIdx);

                        // CyclePhase from PDIRData only for OneProviderMode (not for LEGACY-Provider)
                        if (pProvider->AuxAcwId == EDDP_CRT_CHAIN_IDX_END)
                        {
                            pAcw->AddParam.CyclePhase = pProvider->AddParam.PortInfo.PortInfoArray[i].Phase;
                        }

                        EDDP_CRT_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTCheckAndGetProviderPortParam(): AcwId=%d, FrameID=0x%X, TxPortID=%d, FSO=0x%X, Ph=%d, AuxAcwId=%d", 
                                pAcw->AcwId, pAcw->AddParam.FrameID, pAcw->AddParam.TxPortID, pAcw->AddParam.FrameSendOffset, pAcw->AddParam.CyclePhase, pProvider->AuxAcwId);
                    }

                    // next AcwIdx in this provider
                    AcwIdx = pProvMngm->pAcwEntry[AcwIdx].AcwProvLink.Next;
                }

                /* ------------------------ */
                /* Multi-Cast Provider      */
                /* ------------------------ */
                if (   (Response == EDD_STS_OK)
                    && (pProvider->AddParam.PortInfo.PortInfoArray[i].MCFrameSendOffset != EDDP_CRT_FRAME_SEND_OFFSET_UNDEFINED)
                   )
                {
                    EDDP_ASSERT_FALSE(AcwIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

                    pAcw = &pProvMngm->pAcwEntry[AcwIdx];

                    // check ProviderADD.CycleReductionRatio against PDIRData.CycleReductionRatio
                    if (pAcw->AddParam.CycleReductionRatio == EDD_CYCLE_REDUCTION_RATIO_UNDEFINED)
                        CycleReductionRatioTemp = pRqbParam->CycleReductionRatio;
                    else
                        CycleReductionRatioTemp = pAcw->AddParam.CycleReductionRatio;

                    if (CycleReductionRatioTemp != pProvider->AddParam.PortInfo.PortInfoArray[i].MCReductionRatio)
                    {
                        Response = EDD_STS_ERR_PARAM;
                        EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckAndGetProviderPortParam(): ERROR -> ProviderADD.CycleReductionRatio(%d) != PDIRData.MCReductionRatio(%d)", 
                            CycleReductionRatioTemp, pProvider->AddParam.PortInfo.PortInfoArray[i].MCReductionRatio);
                        // exit FOR loop
                        break;
                    }

                    // check ProviderADD.CyclePhase against PDIRData.CyclePhase (for LEGACY-Provider)
                    if ((Response == EDD_STS_OK) && (pProvider->AuxAcwId != EDDP_CRT_CHAIN_IDX_END))
                    {
                        if (pAcw->AddParam.CyclePhase == EDD_CYCLE_PHASE_UNDEFINED)
                            CyclePhaseTemp = pRqbParam->CyclePhase;
                        else
                            CyclePhaseTemp = pAcw->AddParam.CyclePhase;

                        if (CyclePhaseTemp != pProvider->AddParam.PortInfo.PortInfoArray[i].MCPhase)
                        {
                            Response = EDD_STS_ERR_PARAM;
                            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                            EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckAndGetProviderPortParam(): ERROR -> ProviderADD.CyclePhase(%d) != PDIRData.MCPhase(%d)", 
                                CyclePhaseTemp, pProvider->AddParam.PortInfo.PortInfoArray[i].MCPhase);
                            // exit FOR loop
                            break;
                        }
                    }

                    // copy PortInfo to ACW
                    if (Response == EDD_STS_OK)
                    {
                        pAcw->AddParam.FrameID          = pProvider->AddParam.PortInfo.PortInfoArray[i].MCFrameID;
                        pAcw->AddParam.TxPortID         = (LSA_UINT16) (i + 1); // user PortID = 1..4
                        pAcw->AddParam.FrameSendOffset  = pProvider->AddParam.PortInfo.PortInfoArray[i].MCFrameSendOffset;
                        // ACW has new FrameSendOffset, so chain this ACW within its provider
                        EDDP_CRTProvAcwChainSendOrderToProvider (pDDB, ProviderID, AcwIdx);

                        // CyclePhase from PDIRData only for OneProviderMode (not for LEGACY-Provider)
                        if (pProvider->AuxAcwId == EDDP_CRT_CHAIN_IDX_END)
                        {
                            pAcw->AddParam.CyclePhase = pProvider->AddParam.PortInfo.PortInfoArray[i].MCPhase;
                        }

                        EDDP_CRT_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE, "[H:--] EDDP_CRTCheckAndGetProviderPortParam(): Multi-Cast -> AcwId=%d, FrameID=0x%X, TxPortID=%d, FSO=0x%X, Ph=%d, AuxAcwId=%d", 
                            pAcw->AcwId, pAcw->AddParam.FrameID, pAcw->AddParam.TxPortID, pAcw->AddParam.FrameSendOffset, pAcw->AddParam.CyclePhase, pProvider->AuxAcwId);
                    }

                    // next AcwIdx in this provider
                    AcwIdx = pProvMngm->pAcwEntry[AcwIdx].AcwProvLink.Next;
                }
            }
        }
    }

    if (Response != EDD_STS_OK)
    {
        // in error case delete PortInfo of all ACWs in this provider - this cannot be used any more at the next ACTIVATE
        AcwIdx = pProvider->AcwList.BeginIdx;
        // loop through all ACWs in this provider from BeginIdx to EndIdx
        while (AcwIdx != EDDP_CRT_CHAIN_IDX_END)
	    {
            EDDP_ASSERT_FALSE(AcwIdx >= pDDB->CRT.CfgParam.ProviderAcwMaxNumber);

            pAcw = &pProvMngm->pAcwEntry[AcwIdx];
            pAcw->AddParam.FrameSendOffset = EDDP_CRT_FRAME_SEND_OFFSET_UNDEFINED;
            // next AcwIdx in this provider
            AcwIdx = pProvMngm->pAcwEntry[AcwIdx].AcwProvLink.Next;
        }
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTCheckIsActiveRTC3ProviderConsumerPresent        +*/
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
/*+  Description: This function checks whether CRT contains ACTIVE                      +*/
/*+               RTC3 provider or consumer of given interface.                         +*/
/*+                                                                                     +*/
/*+               Return LSA_TRUE  : CRT contains ACTIVE RTC3 provider or consumer      +*/
/*+                      LSA_FALSE : no ACTIVE RTC3 provider or consumer in CRT         +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckIsActiveRTC3ProviderConsumerPresent(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_BOOL                    State;
    LSA_UINT32                  ConsumerActCntRTC3;
    EDDP_DDB_CRT_PTR_TYPE       pCRTMngm;

    pCRTMngm           = &pDDB->CRT;
    State              = LSA_FALSE;
    ConsumerActCntRTC3 = EDDP_CRTCountActiveDP2Consumer(pDDB);

    if ((pCRTMngm->Prov.AcwActCntProvCntrlRTC3 > 0) || (ConsumerActCntRTC3 > 0))
    {
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] EDDP_CRTCheckIsActiveRTC3ProviderConsumerPresent(): ProviderActCntRTC3DDB=%d, ConsumerActCntRTC3DDB=%d", 
            pCRTMngm->Prov.AcwActCntProvCntrlRTC3, ConsumerActCntRTC3);

        State = LSA_TRUE;
    }

    return (State);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTCheckIsActiveRTC12ProviderPresent               +*/
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
/*+  Description: This function checks whether CRT contains ACTIVE                      +*/
/*+               RTC1/2 provider of given interface.                                   +*/
/*+                                                                                     +*/
/*+               Return LSA_TRUE  : CRT contains ACTIVE RTC1/2 provider                +*/
/*+                      LSA_FALSE : no ACTIVE RTC1/2 provider in CRT                   +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckIsActiveRTC12ProviderPresent(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_BOOL                    State;
    EDDP_DDB_CRT_PTR_TYPE       pCRTMngm;

    pCRTMngm = &pDDB->CRT;
    State    = LSA_FALSE;

    if (pCRTMngm->Prov.AcwActCntProvCntrlRTC12 > 0)
    {
        EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] EDDP_CRTCheckIsActiveRTC12ProviderPresent(): AcwActCntProvCntrlRTC12=%d", 
            pCRTMngm->Prov.AcwActCntProvCntrlRTC12);

        State = LSA_TRUE;
    }

    return (State);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTCheckIsActiveProviderConsumerPresent            +*/
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
/*+  Description: This function checks whether CRT contains ACTIVE                      +*/
/*+               provider or consumer of given interface.                              +*/
/*+                                                                                     +*/
/*+               Return LSA_TRUE  : CRT contains ACTIVE provider or consumer           +*/
/*+                      LSA_FALSE : no ACTIVE provider or consumer in CRT              +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckIsActiveProviderConsumerPresent(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_BOOL                    State;
    LSA_UINT32                  ConsumerActCnt;
    EDDP_DDB_CRT_PTR_TYPE       pCRTMngm;

    pCRTMngm       = &pDDB->CRT;
    State          = LSA_FALSE;
    ConsumerActCnt = EDDP_CRTCountAllActiveConsumer (pDDB);

    if ((pCRTMngm->Prov.AcwActCntProvCntrl > 0) || (ConsumerActCnt > 0))
    {
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] EDDP_CRTCheckIsActiveProviderConsumerPresent(): ProviderActCntDDB=%d, ConsumerActCntDDB=%d", 
            pCRTMngm->Prov.AcwActCntProvCntrl, ConsumerActCnt);

        State = LSA_TRUE;
    }

    return (State);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTCheckRangeOfCycleReductionRatio                 +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                    +*/
/*+                             LSA_UINT8                       RTClass                 +*/
/*+                             LSA_UINT16                      Properties              +*/
/*+                             LSA_UINT16                      CycleReductionRatio     +*/
/*+                             LSA_BOOL                        UndefAllowed            +*/
/*+                                                                                     +*/
/*+  Result                :    EDD_RSP                                                 +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB                   : Pointer to DeviceDescriptionBlock                 (in)    +*/
/*+  RTClass                : RT or RToverUDP                                   (in)    +*/
/*+  Properties             : RTClass1, RTClass2, RTClass3...                   (in)    +*/
/*+  CycleReductionRatio    : CycleReductionRatio for check                     (in)    +*/
/*+  UndefAllowed           : Set LSA_TRUE is undefined value is allowed        (in)    +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This function checks the valid range of CycleReductionRatio for       +*/
/*+               RTClass   : EDDP_CRT_RTCLASS_RT                                       +*/
/*+                         : EDDP_CRT_RTCLASS_UDP                                      +*/
/*+                                                                                     +*/
/*+               Response = EDD_STS_ERR_PARAM, if                                      +*/
/*+                 - CycleReductionRatio is UNDEFINED and UndefAllowed=LSA_FALSE       +*/
/*+                 - CycleReductionRatio is out of range                               +*/
/*+                 - CycleReductionRatio is not a value of a binary tree               +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckRangeOfCycleReductionRatio(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    LSA_UINT8                   RTClass,
    LSA_UINT16                  Properties,
    LSA_UINT16                  CycleReductionRatio,
    LSA_BOOL                    UndefAllowed)
{
    EDD_RSP     Response;
    LSA_UINT16  CycleReductionRatioMin = 0, CycleReductionRatioMax = 0;

    Response = EDD_STS_OK;

    if (CycleReductionRatio == EDD_CYCLE_REDUCTION_RATIO_UNDEFINED)
    {
        if (!(UndefAllowed))
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckRangeOfCycleReductionRatio(): ERROR -> RR=UNDEFINED");
        }
        // else EDDP_CRT_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CRTCheckRangeOfCycleReductionRatio(): Info -> RR=UNDEFINED");
    }
    else
    {
        switch (RTClass)
        {
            case EDDP_CRT_RTCLASS_RT:
            {
                if ((Properties & EDD_CSRT_PROV_PROP_RTCLASS_MASK) != EDD_CSRT_PROV_PROP_RTCLASS_3) // also works for consumer as defines have identical values
                {
                    CycleReductionRatioMin = EDD_CSRT_CYCLE_REDUCTION_RATIO_MIN;
                    CycleReductionRatioMax = EDD_CSRT_CYCLE_REDUCTION_RATIO_MAX;
                }
                else
                {
                    CycleReductionRatioMin = EDD_CSRT_CYCLE_REDUCTION_RATIO_RTC3_MIN;
                    CycleReductionRatioMax = EDD_CSRT_CYCLE_REDUCTION_RATIO_RTC3_MAX;
                }
            }
            break;

            case EDDP_CRT_RTCLASS_UDP:
            {
                CycleReductionRatioMin = EDD_CSRT_UDP_CYCLE_REDUCTION_RATIO_MIN;
                CycleReductionRatioMax = EDD_CSRT_UDP_CYCLE_REDUCTION_RATIO_MAX;
            }
            break;

            default:
            {
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCheckRangeOfCycleReductionRatios(): FATAL -> RTClass unknown! RTClass=0x%X", 
                    RTClass);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
            break;
        }

        if (   (CycleReductionRatio < CycleReductionRatioMin) 
            || (CycleReductionRatio > CycleReductionRatioMax)
           )
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
            EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckRangeOfCycleReductionRatio(): ERROR -> RR(%d) out of range, CycleReductionRatioMin=%d, CycleReductionRatioMax=%d", 
                CycleReductionRatio, CycleReductionRatioMin, CycleReductionRatioMax);
        }
        else
        {
            if ( !(EDDP_CRTCheckIsCycleReductionRatioPowerOfTwo(pDDB, CycleReductionRatio)) )
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR (pDDB, Response);
                EDDP_CRT_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CRTCheckRangeOfCycleReductionRatio(): ERROR -> RR(%d) is not a power of two", 
                    CycleReductionRatio);
            }
        }
    }
    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTCheckIsProviderConsumerPresent                  +*/
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
/*+  Description: This function checks whether CRT contains provider or consumer        +*/
/*+               on this device (pDDB).                                             +*/
/*+                                                                                     +*/
/*+               Return LSA_TRUE  : CRT contains provider or consumer                  +*/
/*+                      LSA_FALSE : no provider or consumer in CRT                     +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckIsProviderConsumerPresent(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_BOOL                    State;
    EDDP_DDB_CRT_PTR_TYPE       pCRTMngm;

    pCRTMngm = &pDDB->CRT;
    State    = LSA_FALSE;

    if ((pCRTMngm->Prov.ProviderUsedCnt > 0) || (pCRTMngm->Cons.ConsumerUsedCnt > 0))
    {
        EDDP_CRT_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] EDDP_CRTCheckIsProviderConsumerPresent(): ProviderUsedCnt=%d, ConsumerUsedCnt=%d", 
            pCRTMngm->Prov.ProviderUsedCnt, pCRTMngm->Cons.ConsumerUsedCnt);

        State = LSA_TRUE;
    }

    return (State);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                                     +*/
/*+  Functionname          :    EDDP_CRTCheckIsBCAddress                                +*/
/*+                                                                                     +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB                        +*/
/*+                             EDD_MAC_ADR_TYPE            *pMac                       +*/
/*+                                                                                     +*/
/*+  Result                :    LSA_BOOL                                                +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Input/Output:                                                                      +*/
/*+                                                                                     +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                                 +*/
/*+  *pMac          : Pointer to MAC address                                            +*/
/*+                                                                                     +*/
/*+-------------------------------------------------------------------------------------+*/
/*+                                                                                     +*/
/*+  Description: This utility function checks the given MAC address for type broadcast.+*/
/*+                                                                                     +*/
/*+               Return LSA_TRUE  : is a BC address                                    +*/
/*+                      LSA_FALSE : is no BC address                                   +*/
/*+                                                                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckIsBCAddress(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDD_MAC_ADR_TYPE            *pMac)
{
    LSA_UINT8       nTemp;

    LSA_UNUSED_ARG(pDDB);

    nTemp = pMac->MacAdr[0];

    if (nTemp == EDDP_CRT_INVALID_VALUE_8)
    {
        nTemp &= pMac->MacAdr[1];
        nTemp &= pMac->MacAdr[2];
        nTemp &= pMac->MacAdr[3];
        nTemp &= pMac->MacAdr[4];
        nTemp &= pMac->MacAdr[5];
        if (nTemp == EDDP_CRT_INVALID_VALUE_8)
        {   // ***** is a BC address
            return (LSA_TRUE);
        }
    }

    return (LSA_FALSE);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTCheckProviderExistFrameID                      +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                   +*/
/*+                      LSA_UINT16                      FrameID                +*/
/*+                                                                             +*/
/*+  Result         :    LSA_BOOL                                               +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB           : Pointer to DeviceDescriptionBlock                         +*/
/*+  FrameID        : FrameID for searching in current existing consumers       +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function checks existing Providers for given FrameID.    +*/
/*+                                                                             +*/
/*+                 LSA_TRUE    : some providers exist with this FrameID        +*/
/*+                 LSA_FALSE   : no provider with this FrameID                 +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckProviderExistFrameID(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT16                  FrameID)
{
    LSA_BOOL                                State;
    LSA_UINT16                              ProvIdx;
    EDDP_CRT_PROVIDER_MNGM_PTR_TYPE         pProvMngm;

    State           = LSA_FALSE;
    pProvMngm    = &pDDB->CRT.Prov;

    ProvIdx = pProvMngm->ProviderUsedList.BeginIdx;
    // loop through all used providers
    while (ProvIdx != EDDP_CRT_CHAIN_IDX_END)
    {
        EDDP_ASSERT_FALSE(ProvIdx >= pDDB->CRT.CfgParam.ProviderMaxNumber);

        if (FrameID == pProvMngm->pProvEntry[ProvIdx].AddParam.FrameID)
        {
            State   = LSA_TRUE;
            ProvIdx = EDDP_CRT_CHAIN_IDX_END;   // exit while loop
        }
        else
        {
            // next ProvIdx in ProviderUsedList
            ProvIdx = pProvMngm->pProvEntry[ProvIdx].UsedFreeLink.Next;
        }
    }

    return (State);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTCheckIsCycleBaseFactorEven                     +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                   +*/
/*+                      LSA_UINT16                      CycleBaseFactor        +*/
/*+                                                                             +*/
/*+  Result         :    LSA_BOOL                                               +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                     +*/
/*+  CycleBaseFactor    : CycleBaseFactor                                       +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function checks the given CycleBaseFactor is even and    +*/
/*+               belongs to the range 1,2,4,8,16,32,64,128.                    +*/
/*+                                                                             +*/
/*+                 LSA_TRUE    : CycleBaseFactor is even and is in valid range +*/
/*+                 LSA_FALSE   : CycleBaseFactor is not even                   +*/
/*+                                                                             +*/
/*+               If the CycleBaseFactor is out of range 1..128 -> FATAL!       +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckIsCycleBaseFactorEven(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT16                  CycleBaseFactor)
{
    LSA_BOOL        State;

    LSA_UNUSED_ARG(pDDB);

    State = LSA_FALSE;

    switch (CycleBaseFactor)
    {
        case EDDP_CRT_CYCLE_BASE_FACTOR_1:
        case EDDP_CRT_CYCLE_BASE_FACTOR_2:
        case EDDP_CRT_CYCLE_BASE_FACTOR_4:
        case EDDP_CRT_CYCLE_BASE_FACTOR_8:
        case EDDP_CRT_CYCLE_BASE_FACTOR_16:
        case EDDP_CRT_CYCLE_BASE_FACTOR_32:
        case EDDP_CRT_CYCLE_BASE_FACTOR_64:
        case EDDP_CRT_CYCLE_BASE_FACTOR_128:
        {
            State = LSA_TRUE;
        }
        break;

        default:
        {
            if ((CycleBaseFactor < EDDP_CRT_CYCLE_BASE_FACTOR_MIN) || (CycleBaseFactor > EDDP_CRT_CYCLE_BASE_FACTOR_MAX))
            {
                EDDP_CRT_TRACE_03(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CRTCheckIsCycleBaseFactorEven(): FATAL -> CycleBaseFactor(%d) is out of range (%d..%d)!", 
                    CycleBaseFactor, EDDP_CRT_CYCLE_BASE_FACTOR_MIN, EDDP_CRT_CYCLE_BASE_FACTOR_MAX);
                EDDP_FATAL(EDDP_FATAL_ERR_PARAM);
            }
        }
        break;
    }

    return (State);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                             +*/
/*+  Functionname   :    EDDP_CRTCheckIsCycleReductionRatioPowerOfTwo           +*/
/*+                                                                             +*/
/*+  Input/Output   :    EDDP_LOCAL_DDB_PTR_TYPE         pDDB                   +*/
/*+                      LSA_UINT16                      CycleReductionRatio    +*/
/*+                                                                             +*/
/*+  Result         :    LSA_BOOL                                               +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Input/Output:                                                              +*/
/*+                                                                             +*/
/*+  pDDB               : Pointer to DeviceDescriptionBlock                     +*/
/*+  CycleBaseFactor    : CycleReductionRatio for check                         +*/
/*+                                                                             +*/
/*+-----------------------------------------------------------------------------+*/
/*+                                                                             +*/
/*+  Description: This function checks if the given CycleReductionRatio is a    +*/
/*+               power of two (1,2,4,8,...)                                    +*/
/*+                                                                             +*/
/*+                 LSA_TRUE    : CycleReductionRatio is a power of two         +*/
/*+                 LSA_FALSE   : CycleReductionRatio is not a power of two     +*/
/*+                                                                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CRTCheckIsCycleReductionRatioPowerOfTwo(
	EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
	LSA_UINT16                  CycleReductionRatio)
{
    LSA_BOOL        State;
    LSA_UINT8       NrOfOnes;
    LSA_UINT16      CRRtmp;

    LSA_UNUSED_ARG(pDDB);

    State = LSA_FALSE;
    NrOfOnes = 0;
    CRRtmp = CycleReductionRatio;
    
    while (CRRtmp)
    {
        if (CRRtmp & 1)
        {
            NrOfOnes++;
        }
        CRRtmp = (CRRtmp>>1);
    }
    
    if (NrOfOnes == 1)
    {
        State = LSA_TRUE;
    }

    return (State);
}


