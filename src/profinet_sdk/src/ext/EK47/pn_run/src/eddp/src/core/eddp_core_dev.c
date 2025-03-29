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
/*  F i l e               &F: eddp_core_dev.c                           :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  EDDP Device and handle-management functions      */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  20.08.08    JS    initial version.                                       */
/*  22.12.08    LRG   K32                                                    */
/*  02.10.09    JS    added EDD_HW_TYPE_FEATURE_SYNC_MASTER_SUPPORT          */
/*  19.08.10    JS    added EDD_HW_TYPE_FEATURE_PREAMBLE_SHORTENING_SUPPORT  */
/*                    added EDD_HW_TYPE_FEATURE_FRAGMENTATION_SUPPORT        */
/*  24.11.10    JS    changes to prevent compiler warnings                   */
/*  17.01.11    JS    added MaxInterfaceCntOfAllEDD and MaxPortCntOfAllEDD   */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID  2
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_CORE_DEV */

/*===========================================================================*/
/*                                 includes                                  */
/*===========================================================================*/

#include "eddp_inc.h"            /* eddp headerfiles */
#include "eddp_int.h"            /* internal header  */

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)

/*===========================================================================*/
/*                                global Variables                           */
/*===========================================================================*/


/*===========================================================================*/
/*                                  Types                                    */
/*===========================================================================*/

/* --------------------------------------------------------------------------*/
/* DEVICE_OPEN Fct-table                                                     */
/* --------------------------------------------------------------------------*/

typedef struct _EDDP_DDB_CORE_DEV_OPEN_FCT_TBL_TYPE
{
    EDD_RSP                 LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, DeviceOpenFct)
    (EDDP_LOCAL_DDB_PTR_TYPE   pDDB,
     EDDP_UPPER_DPB_PTR_TYPE   pDPB);
    LSA_UINT32              OpenMsk;

} EDDP_DDB_CORE_DEV_OPEN_FCT_TBL_TYPE;

/* --------------------------------------------------------------------------*/
/* DEVICE_CLOSE Fct-table                                                    */
/* --------------------------------------------------------------------------*/

typedef struct _EDDP_DDB_CORE_DEV_CLOSE_FCT_TBL_TYPE
{
    EDD_RSP                 LSA_FCT_PTR(EDDP_LOCAL_FCT_ATTR, DeviceCloseFct)
    (EDDP_LOCAL_DDB_PTR_TYPE   pDDB);
    LSA_UINT32              OpenMsk;

} EDDP_DDB_CORE_DEV_CLOSE_FCT_TBL_TYPE;

/*===========================================================================*/
/*                                Constants                                  */
/*===========================================================================*/

/* --------------------------------------------------------------------------*/
/* DEVICE_OPEN Fct-table                                                     */
/* --------------------------------------------------------------------------*/
/* This functions will be called within EDDP_SRV_DEV_OPEN request by core    */
/* --------------------------------------------------------------------------*/

static EDDP_DDB_CORE_DEV_OPEN_FCT_TBL_TYPE  EDDP_CoreDeviceOpenFctTable[] =
{
    { EDDP_CoreDeviceOpen	, EDDP_DDB_CMP_CORE_OPEN	},
    { EDDP_CMDDeviceOpen	, EDDP_DDB_CMP_CMD_OPEN		},
    { EDDP_SWIDeviceOpen	, EDDP_DDB_CMP_SWI_OPEN		},
    { EDDP_GENDeviceOpen	, EDDP_DDB_CMP_GEN_OPEN		},
    { EDDP_NRTDeviceOpen	, EDDP_DDB_CMP_NRT_OPEN		},
    { EDDP_CRTDeviceOpen	, EDDP_DDB_CMP_CRT_OPEN		},
    { EDDP_PRMDeviceOpen	, EDDP_DDB_CMP_PRM_OPEN		},
    { EDDP_K32DeviceOpen	, EDDP_DDB_CMP_K32_OPEN		},
    { LSA_NULL				, 0							}  /* end of list */
};

/* --------------------------------------------------------------------------*/
/* DEVICE_CLOSE Fct-table                                                    */
/* --------------------------------------------------------------------------*/
/* This functions will be called within EDDP_SRV_DEV_CLOSE request by core or*/
/* within EDDP_SRV_DEV_OPEN if open fails (only the components successfully  */
/* opened will be closed)                                                    */
/* --------------------------------------------------------------------------*/

/* Note: The defines EDDP_DDB_CMP_xxx_OPEN are intentionally OPEN not CLOSE! */
/*       We want to test which component is OPEN and shall be closed!        */

static EDDP_DDB_CORE_DEV_CLOSE_FCT_TBL_TYPE  EDDP_CoreDeviceCloseFctTable[] =
{
    { EDDP_PRMDeviceClose	, EDDP_DDB_CMP_PRM_OPEN		},
    { EDDP_CRTDeviceClose	, EDDP_DDB_CMP_CRT_OPEN		},
    { EDDP_NRTDeviceClose	, EDDP_DDB_CMP_NRT_OPEN		},
    { EDDP_GENDeviceClose	, EDDP_DDB_CMP_GEN_OPEN		},
    { EDDP_SWIDeviceClose	, EDDP_DDB_CMP_SWI_OPEN		},
    { EDDP_K32DeviceClose	, EDDP_DDB_CMP_K32_OPEN		},
    { EDDP_CMDDeviceClose	, EDDP_DDB_CMP_CMD_OPEN		},
    { EDDP_CoreDeviceClose  , EDDP_DDB_CMP_CORE_OPEN	},
    { LSA_NULL				, 0							} /* end of list */
};


/*===========================================================================*/
/*                                 functions                                 */
/*===========================================================================*/

/*===========================================================================*/
/*                                 Macros                                    */
/*===========================================================================*/

/*===========================================================================*/
/*=====  handle-functions                                                ====*/
/*===========================================================================*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreValidateFeatures                   +*/
/*+  Input/Output          :    EDDP_UPPER_DPB_PTR_TYPE     pDPB            +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB         Pointer to DPB                                            +*/
/*+  pDDB         Pointer to DDB                                            +*/
/*+                                                                         +*/
/*+  Result:      EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Validate FeatureSupport - copy to DDB if success          +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_CoreValidateFeatures(
    EDDP_UPPER_DPB_PTR_TYPE     pDPB,
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDDP_DPB_FEATURE_TYPE FeatSupp  = pDPB->FeatureSupport; 

    if(   (EDD_FEATURE_DISABLE != FeatSupp.bShortPreambleSupported)
       && (EDD_FEATURE_ENABLE != FeatSupp.bShortPreambleSupported) )
    {
        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "EDDP_CoreValidateFeatures(bShortPreambleSupported out of range: 0x%X): EDD_STS_ERR_PARAM",
            FeatSupp.bShortPreambleSupported);
        return EDD_STS_ERR_PARAM;
    }
    else if(   (EDD_FEATURE_DISABLE !=  FeatSupp.bMRPDSupported)
            && (EDD_FEATURE_ENABLE !=  FeatSupp.bMRPDSupported) )
    {
        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "EDDP_CoreValidateFeatures(bMRPDSupported out of range: 0x%X): EDD_STS_ERR_PARAM",
            FeatSupp.bMRPDSupported);
        return EDD_STS_ERR_PARAM;
    }
    else if((EDD_DPB_FEATURE_FRAGTYPE_SUPPORTED_NO != FeatSupp.FragmentationtypeSupported)
        && (EDD_DPB_FEATURE_FRAGTYPE_SUPPORTED_DYNAMIC != FeatSupp.FragmentationtypeSupported))
    {        
        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "EDDP_CoreValidateFeatures(FragmentationtypeSupported out of range: 0x%X): EDD_STS_ERR_PARAM",
            FeatSupp.FragmentationtypeSupported);
        return EDD_STS_ERR_PARAM;
    }
    else if(!(FeatSupp.IRTForwardingModeSupported & (EDD_DPB_FEATURE_IRTFWDMODE_SUPPORTED_ABSOLUTE | EDD_DPB_FEATURE_IRTFWDMODE_SUPPORTED_RELATIVE)))
    {        
        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "EDDP_CoreValidateFeatures(IRTForwardingModeSupported out of range: 0x%X): EDD_STS_ERR_PARAM",
            FeatSupp.IRTForwardingModeSupported);
        return EDD_STS_ERR_PARAM;
    }
    else if((0 != FeatSupp.MaxDFPFrames)
        && (EDDP_CRT_MAX_PACK_FRAMES_ERTEC200P != FeatSupp.MaxDFPFrames))
    {
        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "EDDP_CoreValidateFeatures(MaxDFPFrames out of range: 0x%X): EDD_STS_ERR_PARAM",
            FeatSupp.MaxDFPFrames);
        return EDD_STS_ERR_PARAM;
    }
    else if(EDD_FEATURE_DISABLE != FeatSupp.ApplicationExist)
    {
        /* HSYNC Role APPL SUPP is not supported by the EDDP (atm) */
        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "EDDP_CoreValidateFeatures(ApplicationExist but not supported)(%d): EDD_STS_ERR_PARAM", 
            FeatSupp.ApplicationExist);
        return EDD_STS_ERR_PARAM;
    }
    else if(   (EDD_FEATURE_DISABLE != FeatSupp.AdditionalForwardingRulesSupported)
            && (EDD_FEATURE_ENABLE != FeatSupp.AdditionalForwardingRulesSupported) )
    {
        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "EDDP_CoreValidateFeatures illegal value for AdditionalForwardingRulesSupported(%d): EDD_STS_ERR_PARAM", 
            FeatSupp.AdditionalForwardingRulesSupported);
        return EDD_STS_ERR_PARAM;
    }
    else if(EDD_FEATURE_ENABLE == FeatSupp.AdditionalForwardingRulesSupported
        && !pDPB->SWI.MRPSupportedRole)
    {
        /* mrp has to be supported in case of HSYNC */
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "EDDP_CoreValidateFeatures(HSYNC without MRP): EDD_STS_ERR_PARAM");
        return EDD_STS_ERR_PARAM;            
    }
    else if(   (   (EDD_FEATURE_DISABLE != FeatSupp.MRPInterconnFwdRulesSupported)
                && (EDD_FEATURE_ENABLE != FeatSupp.MRPInterconnFwdRulesSupported))
            ||  (EDD_FEATURE_DISABLE != FeatSupp.MRPInterconnOriginatorSupported) )
    {
        EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "EDDP_CoreValidateFeatures illegal value(s) for MRPInterconnFwdRulesSupported(0x%X)/MRPInterconnOriginatorSupported(0x%X): EDD_STS_ERR_PARAM", 
            FeatSupp.MRPInterconnFwdRulesSupported, FeatSupp.MRPInterconnOriginatorSupported);
        return EDD_STS_ERR_PARAM;
    }
    else if((EDD_FEATURE_ENABLE == FeatSupp.MRPInterconnFwdRulesSupported)
        && (   (0 == pDPB->SWI.MaxMRPInterconn_Instances) 
            || (pDDB->SWI.PortCnt < 2) ) )
    {
        EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "MaxMRPInterconn_Instances=%d, PortMapCnt=%d but MRPInterconnFwdRulesSupported is ENABLEd: EDD_STS_ERR_PARAM",
            pDPB->SWI.MaxMRPInterconn_Instances, pDDB->SWI.PortCnt);
        return EDD_STS_ERR_PARAM;            
    }
    else if(   (EDD_FEATURE_DISABLE != FeatSupp.EIPExtensions)
            && (EDD_FEATURE_ENABLE  != FeatSupp.EIPExtensions)
           )
           
    {
        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "EDDP_CoreValidateFeatures(FeatSupp.EIPExtensions out of range: 0x%X): EDD_STS_ERR_PARAM",
                FeatSupp.EIPExtensions);
        return EDD_STS_ERR_PARAM;
    } 
    else if (   (EDD_FEATURE_DISABLE != FeatSupp.DLRExtensions)
             && (EDD_FEATURE_ENABLE != FeatSupp.DLRExtensions)
        )

    {
        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "EDDP_CoreValidateFeatures(FeatSupp.DLRExtensions out of range: 0x%X): EDD_STS_ERR_PARAM",
            FeatSupp.DLRExtensions);
        return EDD_STS_ERR_PARAM;
    }
    else
    {
        pDDB->FeatureSupport = FeatSupp;
        return EDD_STS_OK;
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreDevHandleUsesDDB                   +*/
/*+  Input/Output               EDDP_LOCAL_DDB_PTR_TYPE     pDDB            +*/
/*+  Result                :    LSA_BOOL                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB         Pointer to DDB                                            +*/
/*+                                                                         +*/
/*+  Result:      EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_RESOURCE                                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Searches in Handle-management for handle which uses the   +*/
/*+               specified DDB.                                            +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_BOOL EDDP_LOCAL_FCT_ATTR EDDP_CoreDevHandleUsesDDB(
    EDDP_LOCAL_DDB_PTR_TYPE pDDB)
{
    LSA_UINT32               Index;
    EDDP_LOCAL_HDB_PTR_TYPE  pHDB;
    LSA_BOOL                 Found;

    pHDB    = &pEDDPInfo->HDB[0];
    Found   = LSA_FALSE;
    Index   = 0;

    while ((Index < EDDP_CFG_MAX_CHANNELS) && (!Found))
    {
        if ((pHDB[Index].InUse) && (LSA_HOST_PTR_ARE_EQUAL(pHDB[Index].pDDB,pDDB)))
        {
            Found = LSA_TRUE;
        }

        Index++;
    }

    return (Found);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreDevHandleNew                       +*/
/*+  Input/Output          :    LSA_HANDLE_TYPE EDDP_LOCAL_MEM_ATTR* pHandle+*/
/*+                             EDDP_LOCAL_HDB_PTR_TYPE            * ppHDB  +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pHandle      Pointer to address for LSA_HANDLE which will be returned. +*/
/*+  ppHDB        Pointer to address for handle-management which will be    +*/
/*+               returned.                                                 +*/
/*+                                                                         +*/
/*+  Result:      EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_RESOURCE                                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Creates new EDD channel Handle                            +*/
/*+               On success an LSA_HANDLE and a pointer to the HDB will be +*/
/*+               returned.                                                 +*/
/*+               InUse and Handle is setup, the rest is initialized with 0 +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreDevHandleNew(
    LSA_HANDLE_TYPE          EDDP_LOCAL_MEM_ATTR         *pHandle,
    EDDP_LOCAL_HDB_PTR_TYPE  EDDP_LOCAL_MEM_ATTR         *ppHDB)
{
    LSA_UINT32               Index;
    EDD_RSP                  Response;
    EDDP_LOCAL_HDB_PTR_TYPE  pHDB = &pEDDPInfo->HDB[0];

    EDDP_CORE_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CoreDevHandleNew(pHandle=0x%X, ppHDB=0x%X)",
                       pHandle, ppHDB);

    if (pEDDPInfo->HDBUsedCnt >= EDDP_CFG_MAX_CHANNELS)
    {
        EDDP_CORE_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CoreDevHandleNew(): ERROR -> No more free Handles!");
        Response = EDD_STS_ERR_RESOURCE;
    }
    else
    {
        Response = EDD_STS_OK;
        Index    = 0;

        while ((Index < EDDP_CFG_MAX_CHANNELS) && (pHDB[Index].InUse))
        {
            Index++;
        }

        if (Index >= EDDP_CFG_MAX_CHANNELS)
        {
            EDDP_CORE_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CoreDevHandleNew(): ERROR -> No more free Handles!");
            Response = EDD_STS_ERR_RESOURCE;
        }
        else
        {
            pEDDPInfo->HDBUsedCnt++;

            EDDP_MEMSET_COMMON((EDD_COMMON_MEM_PTR_TYPE)&pHDB[Index],0,sizeof(EDDP_HDB_TYPE));

            pHDB[Index].InUse   = LSA_TRUE;
            pHDB[Index].Handle  = (LSA_HANDLE_TYPE) Index;

            *pHandle = (LSA_HANDLE_TYPE) Index;
            *ppHDB   = &pHDB[Index];
        }
    }

    EDDP_CORE_TRACE_03(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "H:--] OUT: EDDP_CoreDevHandleNew(Handle=0x%X, *ppHDB=0x%X): Response=0x%X",
                       *pHandle, *ppHDB, Response);

    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreDevHandleGetHDB                    +*/
/*+  Input/Output          :    LSA_HANDLE_TYPE                    Handle   +*/
/*+                             EDD_LOCAL_HDB_PTR_TYPE             * ppHDB  +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  Handle       Channel Handle                                            +*/
/*+  ppHDB        Pointer to address for handle-management which will be    +*/
/*+               returned.                                                 +*/
/*+                                                                         +*/
/*+  Result:      EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Gets pointer to handle-managment entry (HDB) for handle.  +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreDevHandleGetHDB(
    LSA_HANDLE_TYPE                             Handle,
    EDDP_LOCAL_HDB_PTR_TYPE EDDP_LOCAL_MEM_ATTR *ppHDB)
{
    EDD_RSP     Response;

    Response = EDD_STS_OK;

    /* Handle is index in table */
    if (Handle >= EDDP_CFG_MAX_CHANNELS)
    {
        Response = EDD_STS_ERR_PARAM;
    }
    else
    {
        *ppHDB = &pEDDPInfo->HDB[Handle];
    }

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreDevHandleRel                       +*/
/*+  Input/Output          :    LSA_HANDLE_TYPE                    Handle   +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  Handle       Channel Handle                                            +*/
/*+                                                                         +*/
/*+  Result:      EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_SEQUENCE                                      +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Frees channel handle.                                     +*/
/*+                                                                         +*/
/*+               If the handle is not in use, EDD_STS_ERR_SEQUENCE is      +*/
/*+               returned.                                                 +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreDevHandleRel(
    LSA_HANDLE_TYPE     Handle)
{
    EDDP_LOCAL_HDB_PTR_TYPE  pHDB;
    EDD_RSP                  Response;

    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:%2X] IN: EDDP_CoreDevHandleRel()", Handle);

    Response = EDD_STS_OK;

    if (Handle >= EDDP_CFG_MAX_CHANNELS)
    {
        EDDP_CORE_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_UNEXP, "[H:%2X] OUT: EDDP_CoreDevHandleRel(): Invalid Handle(0x%X)",
                           Handle, Handle);
        Response = EDD_STS_ERR_PARAM;
    }
    else
    {
        pHDB = &pEDDPInfo->HDB[Handle];

        if (!pHDB->InUse)
        {
            EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_UNEXP, "[H:%2X] OUT: EDDP_CoreDevHandleRel(): Handle not in use! Cannot free!",
                               Handle);
            Response = EDD_STS_ERR_SEQUENCE;
        }
        else
        {
            EDDP_ASSERT(pEDDPInfo->HDBUsedCnt);
            pEDDPInfo->HDBUsedCnt--;
            pHDB->InUse = LSA_FALSE;
        }
    }

    EDDP_CORE_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:%2X] OUT: EDDP_CoreDevHandleRel(): Response=0x%X",
                       Handle, Response);

    return (Response);
}

/*===========================================================================*/


/*===========================================================================*/
/*=====  DDB & DDBInfo functions                                         ====*/
/*===========================================================================*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreDevPutToMgm                        +*/
/*+  Input/Output          :    EDD_LOCAL_DDB_PTR_TYPE    pDDB              +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB         : Poiinter to DDB to put into EDD management              +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Put pDDB to device management. Fatal if no room!          +*/
/*+               Inits DevId within DDB.                                   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_VOID EDDP_LOCAL_FCT_ATTR  EDDP_CoreDevPutToMgm(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT32      i;
    LSA_BOOL        Found = LSA_FALSE;

    i       = 0;
    Found   = LSA_FALSE;

    /*--------------------------------------------------------*/
    /* Find a free entry within DeviceManagement              */
    /*--------------------------------------------------------*/
    while ((i< EDDP_CFG_MAX_DEVICES) && (!Found))
    {
        if ( LSA_HOST_PTR_ARE_EQUAL(pEDDPInfo->pDDB[i], LSA_NULL) )
        {
            pEDDPInfo->pDDB[i]  = pDDB;
            pDDB->DevId         = i;        /* Index is used for DevId */
            Found               = LSA_TRUE;

            /* for faster access to DevID put it into DDB too..*/
            pDDB->DevId = i;
        }
        i++;
    }

    pEDDPInfo->DDBUsedCnt++;

    if (!Found)
    {
        EDDP_CORE_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CoreDevPutToMgm(): FATAL -> All DeviceEntry used up!");
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreDevRemoveFromMgm                   +*/
/*+  Input/Output          :    EDD_LOCAL_DDB_PTR_TYPE    pDDB              +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB         : Poiinter to DDB to remove from EDD management           +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Remove pDDB from device management.                       +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_VOID EDDP_LOCAL_FCT_ATTR  EDDP_CoreDevRemoveFromMgm(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    LSA_UINT32      i;

    i = pDDB->DevId;

    if (i < EDDP_CFG_MAX_DEVICES)
    {
        pEDDPInfo->pDDB[i] = LSA_NULL;
        EDDP_ASSERT(pEDDPInfo->DDBUsedCnt);
        pEDDPInfo->DDBUsedCnt--;
    }
    else
    {
        EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL,"[H:--] EDDP_CoreDevRemoveFromMgm(): FATAL -> Invalid DevID=0x%X!", i);
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreDevCloseDDB                        +*/
/*+  Input                 :    EDDP_LOCAL_DDB_PTR_TYPE    pDDB             +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB         : Pointer to DDB to be closed                             +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Closes an DDB. The DDB is unqueued and the memory is freed+*/
/*+                                                                         +*/
/*+               EDD_STS_ERR_PARAM    is returned, if hDDB is invalid.     +*/
/*+                                                                         +*/
/*+               A fatal error is signaled, if no DDB is present.          +*/
/*+                                                                         +*/
/*+               Note: This function does not check for open handles for   +*/
/*+                     this device!!                                       +*/
/*+                     Does not check for open components!                 +*/
/*+                                                                         +*/
/*+               Important: No internal RQB must be in use !!! not checked.+*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static LSA_VOID EDDP_LOCAL_FCT_ATTR  EDDP_CoreDevCloseDDB(
    EDDP_LOCAL_DDB_PTR_TYPE    pDDB)
{
    EDDP_ASSERT_NO_NULL_PTR(pDDB);

    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CoreDevCloseDDB(pDDB=0x%X)", pDDB);

    EDDP_GlbFreeLocalMem(pDDB);

    EDDP_CORE_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CoreDevCloseDDB()");
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreDevCreateDDB                       +*/
/*+  Input/Output          :    EDD_LOCAL_DDB_PTR_TYPE  * ppDDB             +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  ppDDB        : Address for pointer to DDB which will be returned       +*/
/*+                                                                         +*/
/*+  Result:      EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_RESOURCE                                      +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Allocates basic DDB structure and initializes it. The new +*/
/*+               DDB is put into the global DDB-info structure and a       +*/
/*+               pointer to the new DDB is returned.                       +*/
/*+                                                                         +*/
/*+               EDD_STS_ERR_RESOURCE is returned, if alloc fails or no    +*/
/*+               more devices supported.                                   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static EDD_RSP  EDDP_LOCAL_FCT_ATTR  EDDP_CoreDevCreateDDB(
    EDDP_LOCAL_DDB_PTR_TYPE EDDP_LOCAL_MEM_ATTR * ppDDB)
{
    EDD_RSP                         Response;
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB = LSA_NULL;

    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CoreDevCreateDDB(ppDDB=0x%X)",
                       ppDDB);
    
    Response = EDD_STS_OK;
    *ppDDB   = LSA_NULL;

    /* -------------------------------------------------------------------- */
    /* alloc for DDB-Type                                                   */
    /* -------------------------------------------------------------------- */
    EDDP_CORE_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CoreDevCreateDDB(): Allocating DDB memory");
    pDDB = (EDDP_LOCAL_DDB_PTR_TYPE)EDDP_GlbAllocLocalMem (sizeof(EDDP_DDB_TYPE));

    if (LSA_HOST_PTR_ARE_EQUAL(pDDB, LSA_NULL))
    {
        Response = EDD_STS_ERR_RESOURCE;
        EDDP_CORE_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CoreDevCreateDDB(): ERROR -> Allocating DDB memory failed!");
    }
    else
    {
        EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW, "[H:--] EDDP_CoreDevCreateDDB(): Allocating DDB memory successful. pDDB: 0x%X",
                           pDDB);
        EDDP_MEMSET_COMMON(pDDB,0,sizeof(EDDP_DDB_TYPE));
        /* Note: CmpOpen,CmpSetup etc. init with 0 here */

        /* Init DDB */
        pDDB->I2C.I2CInitResult = EDD_STS_OK;
        pDDB->hDDB              = (EDDP_HANDLE) pDDB;
        pDDB->DevIsSetup        = LSA_FALSE;
        pDDB->TraceIdx          = EDDP_UNDEF_TRACE_IDX;
        *ppDDB                  = pDDB;
    }

    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CoreDevCreateDDB(): Response=0x%X",
                       Response);

    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreDevGetDDB                          +*/
/*+  Input/Output          :    EDD_HANDLE                hDDB              +*/
/*+                             EDDP_LOCAL_DDB_PTR_TYPE  * ppDDB            +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  hDDB         : Valid DDB-Handle                                        +*/
/*+  ppDDB        : Address for pointer to DDB which will be returned       +*/
/*+                                                                         +*/
/*+  Result:      EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Gets the DDB associated with the handle.                  +*/
/*+                                                                         +*/
/*+               EDD_STS_ERR_PARAM    is returned, if no DDB was found.    +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP  EDDP_LOCAL_FCT_ATTR  EDDP_CoreDevGetDDB(
    EDDP_HANDLE                                  hDDB,
    EDDP_LOCAL_DDB_PTR_TYPE EDDP_LOCAL_MEM_ATTR * ppDDB)
{
    EDD_RSP                     Response = EDD_STS_OK;
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB;

    pDDB = (EDDP_LOCAL_DDB_PTR_TYPE) hDDB;

    /* check if hDDB was a valid one..*/
    if ( (LSA_HOST_PTR_ARE_EQUAL(pDDB, LSA_NULL)) || (pDDB->hDDB != hDDB) )
    {
        *ppDDB      = LSA_NULL;
        Response    = EDD_STS_ERR_PARAM;
    }
    else
    {
        *ppDDB      = pDDB;
    }

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreDeviceOpenCheckDPB                 +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             EDDP_UPPER_DPB_PTR_TYPE        pDPB         +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  pDPB       : Pointer to DeviceParameterBlock                     (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function checks the DPB parameters                   +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreDeviceOpenCheckDPB(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB,
    EDDP_UPPER_DPB_PTR_TYPE     pDPB)
{
    EDD_RSP     Response = EDD_STS_OK;

    /* Parametercheck of DPB */
    if (LSA_HOST_PTR_ARE_EQUAL(pDPB->pPnipBase, LSA_NULL))
    {
        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CoreDeviceOpenCheckDPB(): ERROR -> pDPB->pPnipBase is NULL!");
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
    }

    if (Response == EDD_STS_OK)
    {
        if ((pDPB->EDDPLocation != EDDP_LOCATION_EXT_HOST) && (pDPB->EDDPLocation != EDDP_LOCATION_LOCAL))
        {
            EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CoreDeviceOpenCheckDPB(): ERROR -> pDPB->EDDPLocation invalid (0x%X)!",
                               pDPB->EDDPLocation);
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
        }
    }

    if (Response == EDD_STS_OK)
    {
        if (   (pDPB->Hardware != EDDP_HW_ERTEC200P) && (pDPB->Hardware != EDDP_HW_ERTEC200P_FPGA) 
            && (pDPB->Hardware != EDDP_HW_HERA_FPGA) && (pDPB->Hardware != EDDP_HW_HERA))
            
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
            EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CoreDeviceOpenCheckDPB(): ERROR -> pDPB->Hardware(0x%X) invalid!",
                               pDPB->Hardware);
        }
    }

    if (Response == EDD_STS_OK)
    {
        if ((pDPB->HardwareIF != EDDP_HW_IF_A) && (pDPB->HardwareIF != EDDP_HW_IF_B))
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
            EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CoreDeviceOpenCheckDPB(): ERROR -> pDPB->HardwareIF(0x%X) invalid!",
                               pDPB->HardwareIF);
        }
    }    

    if (Response == EDD_STS_OK)
    {
        if (pDPB->MaxInterfaceCntOfAllEDD > EDD_CFG_MAX_INTERFACE_CNT)
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
            EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CoreDeviceOpenCheckDPB(): ERROR -> pDPB->MaxInterfaceCntOfAllEDD(%d) > MaxInterfaceCnt(%d)!",
                               pDPB->MaxInterfaceCntOfAllEDD, EDD_CFG_MAX_INTERFACE_CNT);
        }
    }

    if (Response == EDD_STS_OK)
    {
        if (pDPB->MaxPortCntOfAllEDD > EDD_CFG_MAX_PORT_CNT_ALL_INTERFACE)
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
            EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CoreDeviceOpenCheckDPB(): ERROR -> pDPB->MaxPortCntOfAllEDD(%d) >  MaxPortCnt(%d)!",
                               pDPB->MaxPortCntOfAllEDD, EDD_CFG_MAX_PORT_CNT_ALL_INTERFACE);
        }
    }

    if(    (   (EDD_FEATURE_DISABLE != pDPB->SWI.Multicast_Bridge_IEEE802RSTP_Forward)
            && (EDD_FEATURE_ENABLE != pDPB->SWI.Multicast_Bridge_IEEE802RSTP_Forward) )
        || (   (EDD_SYS_FILL_INACTIVE != pDPB->NRT.FeedInLoadLimitationActive)
            && (EDD_SYS_FILL_ACTIVE != pDPB->NRT.FeedInLoadLimitationActive) )
        || (   (EDD_SYS_IO_CONFIGURED_OFF != pDPB->NRT.IO_Configured)
            && (EDD_SYS_IO_CONFIGURED_ON != pDPB->NRT.IO_Configured) )
        || (   (EDD_SYS_UDP_WHITELIST_FILTER_OFF != pDPB->NRT.RxFilterUDP_Broadcast)
            && (EDD_SYS_UDP_WHITELIST_FILTER_ON != pDPB->NRT.RxFilterUDP_Broadcast) )
        || (   (EDD_SYS_UDP_WHITELIST_FILTER_OFF != pDPB->NRT.RxFilterUDP_Unicast)
            && (EDD_SYS_UDP_WHITELIST_FILTER_ON != pDPB->NRT.RxFilterUDP_Unicast) )
        || (   (EDD_FEATURE_DISABLE != pDPB->NRT.RxDropLogging)
            && (EDD_FEATURE_ENABLE != pDPB->NRT.RxDropLogging) )
        )
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
        EDDP_CORE_TRACE_06(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CoreDeviceOpenCheckDPB(): ERROR -> Multicast_Bridge_IEEE802RSTP_Forward(%d), FeedInLoadLimitationActive(%d), IO_Configured(%d), RxFilterUDP_Unicast(%d), RxFilterUDP_Broadcast(%d), RxDropLogging(%d)!",
            pDPB->SWI.Multicast_Bridge_IEEE802RSTP_Forward, pDPB->NRT.FeedInLoadLimitationActive, pDPB->NRT.IO_Configured, pDPB->NRT.RxFilterUDP_Broadcast, pDPB->NRT.RxFilterUDP_Unicast, pDPB->NRT.RxDropLogging);
    }
    
    if (Response == EDD_STS_OK)
    {
        /* HERA only: check TxLocalApplicationFeedExtNs and RxLocalApplicationDelayExtNs */
        if ( (EDDP_HW_HERA == pDPB->Hardware) || (EDDP_HW_HERA_FPGA == pDPB->Hardware) )
        {
            // HERADevMark TP: Pr�fung erweitern
        }
    }

    if (Response == EDD_STS_OK)
    {
        if (   (EDD_FEATURE_ENABLE == pDPB->FeatureSupport.DLRExtensions)
            && (   (EDD_FEATURE_ENABLE != pDPB->FeatureSupport.EIPExtensions)
                || (0 != pDPB->SWI.MaxMRP_Instances)
                || ((pDPB->Hardware != EDDP_HW_ERTEC200P) && (pDPB->Hardware != EDDP_HW_ERTEC200P_FPGA)))
            )
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_CoreDeviceOpenCheckDPB(): ERROR -> pDPB->FeatureSupport.DLRExtensions(0x%X) only allowed for EIPExtensions(0x%X) ENABLED, MRP OFF, ERTEC200P!",
                pDPB->FeatureSupport.DLRExtensions, pDPB->FeatureSupport.EIPExtensions);
        }
    }

    return (Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreDeviceOpen                         +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE        pDDB         +*/
/*+                             EDDP_UPPER_DPB_PTR_TYPE        pDPB         +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+  pDPB       : Pointer to DeviceParameterBlock                     (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_ERR_RESOURCES                                     +*/
/*+               EDD_STS_ERR_PARAM                                         +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDD_SRV_DEV_OPEN                                          +*/
/*+               Initializes all Core management structures within DDB     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreDeviceOpen(
    EDDP_LOCAL_DDB_PTR_TYPE            pDDB,
    EDDP_UPPER_DPB_PTR_TYPE            pDPB)
{
    EDD_RSP     Response = EDD_STS_OK;

    EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] IN: EDDP_CoreDeviceOpen(pDDB=0x%X, pDPB=0x%X)", pDDB, pDPB);

    /* Parametercheck of DPB */
    Response = EDDP_CoreDeviceOpenCheckDPB (pDDB, pDPB);

    if (Response == EDD_STS_OK)
    {
        /* check UserPortID-to-HwPortID-Mapping and set up DDB with this mapping (including port count) */
        Response = EDDP_SWI_portmap_check (pDDB, pDPB);
    }

    if (Response == EDD_STS_OK)
    {
        /* save several parameters from DPB within DDB */
        pDDB->pPnipBase                 = pDPB->pPnipBase;
        pDDB->EDDPLocation              = pDPB->EDDPLocation;
        pDDB->Hardware                  = pDPB->Hardware;
        pDDB->K32.pK32TCMBase           = pDPB->pK32TCMBase;
        pDDB->K32.K32ATCMBlock.pMemBase = pDPB->pK32ATCMBase;
        pDDB->K32.K32BTCMBlock.pMemBase = pDPB->pK32BTCMBase;
        pDDB->K32.K32DDR3Block.pMemBase = pDPB->pK32DDR3Base;
        pDDB->MaxInterfaceCntOfAllEDD   = pDPB->MaxInterfaceCntOfAllEDD;
        pDDB->MaxPortCntOfAllEDD        = pDPB->MaxPortCntOfAllEDD;
        
        /* save hardware type within DDB */
        if ( (pDDB->Hardware == EDDP_HW_ERTEC200P) || (pDDB->Hardware == EDDP_HW_ERTEC200P_FPGA))
        {
            // we map all ERTEC 200 versions to only one version for the user
            pDDB->HardwareType  = EDD_HW_TYPE_USED_ERTEC_200P;

            pDDB->HardwareType  = EDD_HW_TYPE_USED_RZT2;

        }
        else if( (pDPB->Hardware == EDDP_HW_HERA)  || (pDPB->Hardware == EDDP_HW_HERA_FPGA) )
        {
            pDDB->HardwareType  = EDD_HW_TYPE_USED_HERA;
        }        
        else
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
            EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CoreDeviceOpen(): ERROR -> Cannot map Hardware to HardwareType!");
        }
                
        if (Response == EDD_STS_OK)
        {
            /* validate supported features and copy them into DDB if validation successful */
            Response = EDDP_CoreValidateFeatures(pDPB, pDDB);
        }

        if (Response == EDD_STS_OK)
        {
            /* set up supported hardware features */
            pDDB->HardwareType |= EDD_HW_TYPE_FEATURE_TIMESTAMP_SUPPORT                     |
                                  EDD_HW_TYPE_FEATURE_DELAY_SUPPORT                         |
                                  EDD_HW_TYPE_FEATURE_PARTIAL_DATA_CLASS_12_SUPPORT         |
                                  EDD_HW_TYPE_FEATURE_CT_SUPPORT                            |
                                  EDD_HW_TYPE_FEATURE_PARTIAL_DATA_CLASS_3_SUPPORT          |
                                  EDD_HW_TYPE_FEATURE_PARTIAL_DATA_CLASS_1_UDP_SUPPORT      |
                                  EDD_HW_TYPE_FEATURE_AUTOPADDING_CLASS_1_UDP_SUPPORT       |
                                  EDD_HW_TYPE_FEATURE_AUTOPADDING_CLASS_12_SUPPORT          |
                                  EDD_HW_TYPE_FEATURE_AUTOPADDING_CLASS_3_SUPPORT           |
                                  EDD_HW_TYPE_FEATURE_APDUSTATUS_SEPARATE                   |
#ifdef K32_MASTER_IN_FW
                                  EDD_HW_TYPE_FEATURE_SYNC_MASTER_HW_SUPPORT                |
#else
                                  EDD_HW_TYPE_FEATURE_SYNC_MASTER_NO_SUPPORT                |
#endif
#if 0 // pDDB->HardwareType  = EDD_HW_TYPE_USED_RZT2;
                                  EDD_HW_TYPE_FEATURE_SYNC_SLAVE_HW_SUPPORT                 |

                                  EDD_HW_TYPE_FEATURE_SYNC_DELAY_HW_SUPPORT                 |
#else
								  EDD_HW_TYPE_FEATURE_SYNC_SLAVE_SW_SUPPORT                 |

								  EDD_HW_TYPE_FEATURE_SYNC_DELAY_SW_SUPPORT                 |
#endif
                                  EDD_HW_TYPE_FEATURE_TIME_SLAVE_SUPPORT                    |
                                  EDD_HW_TYPE_FEATURE_TIME_MASTER_NO_SUPPORT;

            if(EDD_PORT_LONG_PREAMBLE != pDDB->FeatureSupport.bShortPreambleSupported)
            {
                pDDB->HardwareType |= EDD_HW_TYPE_FEATURE_PREAMBLE_SHORTENING_SUPPORT;
            }
            else
            {
                pDDB->HardwareType |= EDD_HW_TYPE_FEATURE_NO_PREAMBLE_SHORTENING_SUPPORT;
            }

            if(EDD_DPB_FEATURE_FRAGTYPE_SUPPORTED_NO != pDDB->FeatureSupport.FragmentationtypeSupported)
            {
                pDDB->HardwareType |= EDD_HW_TYPE_FEATURE_FRAGMENTATION_SUPPORT;
            }
            else
            {
                pDDB->HardwareType |= EDD_HW_TYPE_FEATURE_NO_FRAGMENTATION_SUPPORT;
            }           

            Response = EDDP_HALDeviceOpen (pDDB, pDPB);
        }

        if (Response == EDD_STS_OK)
        {
            switch (EDDP_HALGetPnipRevision(pDDB))
            {
                #ifdef EDDP_CFG_HW_PNIP_REV1_SUPPORT
                case EDDP_HAL_HW_PNIP_REV1:
                {
                    pDDB->K32.tcmSize   = 128*1024;
                    pDDB->K32.tileSize  = 16*1024; 
                }
                break;
                #endif

                #ifdef EDDP_CFG_HW_PNIP_REV2_SUPPORT
                case EDDP_HAL_HW_PNIP_REV2:
                {
                    pDDB->K32.tcmSize   = 256*1024;
                    pDDB->K32.tileSize  = 16*1024;
                }
                break;
                #endif
                
                default:
                {
                    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CoreDeviceOpen(): FATAL -> PnipRevision(%d) is unknown", 
                        EDDP_HALGetPnipRevision(pDDB));
                    EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
                }
                break;
            }

        }

        if (Response == EDD_STS_OK)
        {
            /* Init interrupt management */
            Response = EDDP_CoreISRDeviceOpen (pDDB, pDPB);
        }
    }

    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] OUT: EDDP_CoreDeviceOpen(): Response=0x%X", Response);

    return (Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreDeviceClose                        +*/
/*+  Input/Output          :    EDDP_LOCAL_DDB_PTR_TYPE           pDDB      +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pDDB       : Pointer to DeviceDescriptionBlock                   (in)  +*/
/*+                                                                         +*/
/*+  Results    : EDD_STS_OK                                                +*/
/*+               EDD_STS_xxx                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function is called from CORE within service          +*/
/*+               EDDP_SRV_DEV_CLOSE or EDDP_SRV_DEV_OPEN (only if          +*/
/*+               previous OPEN shall be aborted)                           +*/
/*+               Close device for Core                                     +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_LOCAL_FCT_ATTR EDDP_CoreDeviceClose(
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB)
{
    EDD_RSP     Response = EDD_STS_OK;

    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] IN: EDDP_CoreDeviceClose(pDDB=0x%X)", pDDB);
    Response = EDDP_CoreISRDeviceClose(pDDB);
    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_CHAT,"[H:--] OUT: EDDP_CoreDeviceClose(): Response=0x%X", Response);

    return(Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreDevOpenRequest                     +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE     pRQB             +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         Opcode:     EDD_OPC_SYSTEM                  +*/
/*+     LSA_HANDLE_TYPE         Handle:                                     +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user (not used)           +*/
/*+     EDD_SERVICE             Service:    EDDP_SRV_DEV_OPEN               +*/
/*+     LSA_RESPONSE_TYPE       Status:     Return status                   +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:     Depend on kind of request.      +*/
/*+                                                                         +*/
/*+  pParam points to EDDP_RQB_DEV_OPEN_TYPE                                +*/
/*+                                                                         +*/
/*+     hDDB     : Returned Device-Handle                                   +*/
/*+     pDDB     : Pointer to device-specific initialization parameters     +*/
/*+                (input - variables !)                                    +*/
/*+     Cbf      : Call-back-function. Called when Device-open is finished. +*/
/*+                LSA_NULL if not used.                                    +*/
/*+                                                                         +*/
/*+  RQB-return values via callback-function:                               +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         opcode:     EDD_OPC_SYSTEM                  +*/
/*+     LSA_HANDLE_TYPE         Handle:                                     +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user                      +*/
/*+     EDD_SERVICE             Service:                                    +*/
/*+     LSA_RESPONSE_TYPE       Status:     EDD_STS_OK                      +*/
/*+                                         EDD_STS_ERR_PARAM               +*/
/*+                                         EDD_STS_ERR_SEQUENCE            +*/
/*+                                         EDD_STS_ERR_RESOURCE            +*/
/*+                                         ...                             +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Request to open a device. This request is the first request*/
/*+               needed for initialization of the device. It returns an    +*/
/*+               device-handle which have to be used in further calls.     +*/
/*+                                                                         +*/
/*+               The request is finished by calling the specified callback +*/
/*+               function. If NULL no callback-function is called and      +*/
/*+               the status is set on function return.                     +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID  EDDP_LOCAL_FCT_ATTR EDDP_CoreDevOpenRequest(
    EDD_UPPER_RQB_PTR_TYPE      pRQB)
{
    EDD_RSP                         Response;
    EDD_RSP                         Response2;
    EDDP_UPPER_DEV_OPEN_PTR_TYPE    pRqbParam;
    EDDP_UPPER_DPB_PTR_TYPE         pDPB;   /* Pointer to device parameter*/
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB;
    LSA_UINT32                      i;
    LSA_BOOL                        Ready;
    EDDP_DBG_DETAIL_ERR_TYPE        ErrDetail = {0,0,0,0};

    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CoreDevOpenRequest(pRQB=0x%X)", pRQB);

    Response    = EDD_STS_OK;
    pDDB        = LSA_NULL;  /* avoids compiler warning */

    if (LSA_HOST_PTR_ARE_EQUAL(pRQB->pParam, LSA_NULL))
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_CORE_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CoreDevOpenRequest(): ERROR -> RQB->pParam is NULL!");
        EDD_RQB_SET_RESPONSE(pRQB, EDD_STS_ERR_PARAM);
        EDDP_RQB_ERROR(pRQB);
    }
    else
    {
        pRqbParam   = (EDDP_UPPER_DEV_OPEN_PTR_TYPE) pRQB->pParam;
        pDPB        = pRqbParam->pDPB;      /* pointer to hardwarespezific parameters */

        if (LSA_HOST_PTR_ARE_EQUAL(pDPB, LSA_NULL))
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR_LOCAL(&ErrDetail,Response);
            EDDP_CORE_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CoreDevOpenRequest(): ERROR -> RQB->pParam->pDPB is NULL!");
        }
        
        if (Response == EDD_STS_OK)
        {
            /* not all devices used ? */
            if (pEDDPInfo->DDBUsedCnt < EDDP_CFG_MAX_DEVICES)
            {
                /*---------------------------------------------------------------*/
                /* allocates and inits base DDB structure                        */
                /*---------------------------------------------------------------*/
                Response = EDDP_CoreDevCreateDDB (&pDDB);

                if (Response == EDD_STS_OK)
                {
                    /* -----------------------------------------------------------*/
                    /* RQB return values: DDB handle                              */
                    /* -----------------------------------------------------------*/
                    pRqbParam->hDDB = pDDB->hDDB;

                    if (Response == EDD_STS_OK)
                    {
                        /* Init global and CORE variables */
                        pDDB->hSysDev  = pDPB->hSysDev;

                        /* Init gSharedRAM pointer for system redundancy API */
                        /* Note that this Shared Memory is only used if the System Redundancy API is configured */
#ifdef EDDP_CFG_SYSRED_API_SUPPORT
                        if (LSA_HOST_PTR_ARE_EQUAL(pDPB->GSharedRAM_BaseAddr,LSA_NULL)) // shall be set to a valid memory area
                        {
                            Response = EDD_STS_ERR_PARAM;
                            EDDP_DBG_SET_DETAIL_ERR_LOCAL(&ErrDetail,Response);
                            EDDP_CORE_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CoreDevOpenRequest(): ERROR -> pDPB->GSharedRAM_BaseAddr is NULL!");
                        }
                        pDDB->pGSharedRAM = (EDDP_GSHAREDMEM_TYPE *) pDPB->GSharedRAM_BaseAddr;
#else
                        if (!LSA_HOST_PTR_ARE_EQUAL(pDPB->GSharedRAM_BaseAddr,LSA_NULL)) // shall be LSA_NULL if System Redundancy API is not supported
                        {
                            Response = EDD_STS_ERR_PARAM;
                            EDDP_DBG_SET_DETAIL_ERR_LOCAL(&ErrDetail,Response);
                            EDDP_CORE_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CoreDevOpenRequest(): ERROR -> pDPB->GSharedRAM_BaseAddr shall be NULL if System Redundancy API is not configured!");
                        }
                        pDDB->pGSharedRAM = LSA_NULL;
#endif

                        /* Init UserCmdSharedMem pointer for User Cmd API */
                        /* Note that this Shared Memory is only used if the User Cmd API is configured */
#ifdef EDDP_CFG_USR_CMD_API_SUPPORT
                        /* Note that this Shared Memory is only needed for the PTCP Time Sync Indication and can be omitted for Application Timer*/
                        pDDB->pUserCmdSharedMem = (EDDP_USERCMDSHAREDMEM_TYPE *) pDPB->UserCmdSharedMem_BaseAddr;
                        /* Initialize Circular List on the Shared Memory*/
                        if (!LSA_HOST_PTR_ARE_EQUAL(pDDB->pUserCmdSharedMem,LSA_NULL))
                        {
                            volatile EDDP_CIRC_LIST_DESCR_TYPE * pDescr           = &pDDB->pUserCmdSharedMem->CircListDescr;
                            volatile EDDP_TIMESTAMP_INFO_TYPE  * pTimeStampInfo   = &pDDB->pUserCmdSharedMem->TimestampInfoBackup;
                            pDescr->SrcWriteIndex = 0;  //Src==Dst: List is empty
                            pDescr->DstReadIndex  = 0;
                            pDescr->ListSize      = EDDP_CFG_USR_CMD_SHAREDMEM_MAXENTRIES;
                            pDescr->ListOffset    = sizeof(pDDB->pUserCmdSharedMem->CircListDescr) 
                                                  + sizeof(pDDB->pUserCmdSharedMem->TimestampInfoBackup);
                            pDescr->ElementSize   = sizeof(EDDP_TIMESTAMP_INFO_PAIR_TYPE);
                            EDDP_MEMSET_COMMON((EDD_COMMON_MEM_PTR_TYPE)pTimeStampInfo,0,sizeof(EDDP_TIMESTAMP_INFO_TYPE));
                            pTimeStampInfo->TimesStamp.Status = EDDP_PNIO_STATUS_NOT_SYNC;
                            pTimeStampInfo->SyncState         = EDDP_PTCP_SYNC_STATE_NO_MESSAGE_RECEIVED;
                        }
#else
                        pDDB->pUserCmdSharedMem = LSA_NULL;
#endif

                        pDDB->Core.Setup.State      = EDDP_DDB_STATE_SETUP_IDLE;
                        pDDB->Core.Shutdown.State   = EDDP_DDB_STATE_SHUTDOWN_IDLE;
                        pDDB->Core.HandleCnt        = 0;
                        pDDB->InterfaceID           = pDPB->InterfaceID;
                        pDDB->TraceIdx              = pDPB->TraceIdx;
                    }

                    /*------------------------------------------------------------*/
                    /* Open all from fct-table.                                   */
                    /*------------------------------------------------------------*/
                    Ready   = LSA_FALSE;
                    i       = 0;

                    while ((Response == EDD_STS_OK) && (!Ready))
                    {
                        if (! (0 == EDDP_CoreDeviceOpenFctTable[i].DeviceOpenFct))
                        {
                            EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW,"[H:--] EDDP_CoreDevOpenRequest[I:%d]. Calling DeviceOpenFct", i);

                            /* Call Subcomponente DeviceOpenFct*/
                            Response = EDDP_CoreDeviceOpenFctTable[i].DeviceOpenFct(pDDB,pDPB);

                            if (Response != EDD_STS_OK)
                            {
                                EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CoreDevOpenRequest[I:%d] ERROR -> Calling DeviceOpenFct failed! (0x%X)", i, Response);

                                /* Copy ErrDetail from DDB to ErrDetail for later use */
                                EDDP_DBG_COPY_DETAIL_ERR_DDB_LOCAL(pDDB,&ErrDetail);
                            }
                            else
                            {
                                pDDB->Core.CmpOpen |= EDDP_CoreDeviceOpenFctTable[i].OpenMsk;
                                EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW,"[H:--] EDDP_CoreDevOpenRequest[I:%d]. Calling DeviceOpenFct successful", i);
                            }
                        }
                        else
                        {
                            Ready = LSA_TRUE;
                        }

                        i++;
                    }

					/*------------------------------------------------------------*/
					/*  Set PHY MUX for HERA                                      */
					/*------------------------------------------------------------*/
					if (Response == EDD_STS_OK)
					{
						
						if (EDDP_HAL_HW_HERA == EDDP_HALGetChipVersion(pDDB))
						{
							Response = EDDP_SWI_Init_Portmapping_HERA(pDDB);

							if (EDD_STS_OK != Response)
							{
								EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_SWIDeviceSetup1(): ERROR -> EDDP_SWI_Init_Portmapping_HERA Response is: 0x%X",
									Response);
							}
						}
					}


                    /*------------------------------------------------------------*/
                    /* If OK put DDB to management. If not Close all              */
                    /*------------------------------------------------------------*/
                    if (Response == EDD_STS_OK)
                    {
#ifdef LSAS_CFG_USE_POF
                        // ***** Initialization of the I2C module
                        pDDB->I2C.I2CInitResult = EDDP_I2C_Init(pDDB);
#endif
                        EDDP_CoreDevPutToMgm(pDDB);
                        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH,"[H:--] EDDP_CoreDevOpenRequest(): successful: pDDB=0x%X", pDDB);
                    }
                    else
                    {
                        /*--------------------------------------------------------*/
                        /* If an error occured we close the DDB and all opened    */
                        /* components.                                            */
                        /*--------------------------------------------------------*/
                        EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_HIGH,"[H:--] EDDP_CoreDevOpenRequest(): failed. Closing opend subcomponents");

                        i     = 0;
                        Ready = LSA_FALSE;

                        while (! Ready)
                        {
                            /* LSA_NULL if table end.. */
                            if (! (0 == EDDP_CoreDeviceCloseFctTable[i].DeviceCloseFct))
                            {
                                if (pDDB->Core.CmpOpen & EDDP_CoreDeviceCloseFctTable[i].OpenMsk)
                                {
                                    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW,"[H:--] EDDP_CoreDevOpenRequest[I:%d]. Calling DeviceCloseFct", i);

                                    /* Call Subcomponente DeviceCloseFct*/
                                    Response2 = EDDP_CoreDeviceCloseFctTable[i].DeviceCloseFct(pDDB);

                                    if (Response2 != EDD_STS_OK) /* here no error expected!*/
                                    {
                                        EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_CoreDevOpenRequest[I:%d]. FATAL -> Error on DeviceCloseFct: 0x%X", i, Response2);
                                        EDDP_FATAL(EDDP_FATAL_ERR_DEV_OPEN);
                                    }
                                    else
                                    {
                                        pDDB->Core.CmpOpen &= ~ EDDP_CoreDeviceCloseFctTable[i].OpenMsk;
                                        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW,"[H:--] EDDP_CoreDevOpenRequest[I:%d]. Calling DeviceCloseFct successful", i);
                                    }
                                }
                                else
                                {
                                    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW,"[H:--] EDDP_CoreDevOpenRequest[I:%d]. Already Closed. Skipping.", i);
                                }
                            }
                            else
                            {
                                Ready = LSA_TRUE;
                            }

                            i++;
                        }

                        /* we always close the DDB */
                        EDDP_CoreDevCloseDDB(pDDB);
                    }
                }
                else
                {
                    EDDP_DBG_SET_DETAIL_ERR_LOCAL(&ErrDetail,Response);
                    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CoreDevOpenRequest(): ERROR -> Response=0x%X",
                                       Response);
                }
            }
            else
            {
                Response = EDD_STS_ERR_RESOURCE;
                EDDP_DBG_SET_DETAIL_ERR_LOCAL(&ErrDetail,Response);
                EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CoreDevOpenRequest(): ERROR -> Cannot create DDB (0x%X)!",
                                   Response);
            }
        }

        /* finished the request. Call CBF */
        EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE,"[H:--] <-- Request: EDDP_SRV_DEV_OPEN finished. Response=0x%X",
                             Response);
        EDD_RQB_SET_RESPONSE(pRQB, Response);
        EDDP_GlbCallCbfLow (pRqbParam->Cbf, pRQB ,ErrDetail.ModuleId, ErrDetail.LineNr);
    }

    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CoreDevOpenRequest(): Response=0x%X", Response);
}



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreDevCloseRequest                    +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE     pRQB             +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         Opcode:     EDD_OPC_SYSTEM                  +*/
/*+     LSA_HANDLE_TYPE         Handle:                                     +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user (not used)           +*/
/*+     EDD_SERVICE             Service:    EDDP_SRV_DEV_CLOSE              +*/
/*+     LSA_RESPONSE_TYPE       Status:     Return status                   +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:     Depend on kind of request.      +*/
/*+                                                                         +*/
/*+  pParam points to EDDP_RQB_DEV_OPEN_TYPE                                +*/
/*+                                                                         +*/
/*+     hDDB     : valid Device-Handle                                      +*/
/*+     Cbf      : Call-back-function. Called when Device-close is finished.+*/
/*+                LSA_NULL if not used.                                    +*/
/*+                                                                         +*/
/*+  RQB-return values via callback-function:                               +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         opcode:     EDD_OPC_SYSTEM                  +*/
/*+     LSA_HANDLE_TYPE         Handle:                                     +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user                      +*/
/*+     EDD_SERVICE             Service:                                    +*/
/*+     LSA_RESPONSE_TYPE       Status:     EDD_STS_OK                      +*/
/*+                                         EDD_STS_ERR_PARAM               +*/
/*+                                         EDD_STS_ERR_SEQUENCE            +*/
/*+                                         EDD_STS_ERR_RESOURCE            +*/
/*+                                         ...                             +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Request to close device. This request finishes all        +*/
/*+               device-handling by terminating all pending requests and   +*/
/*+               shut down the device.                                     +*/
/*+                                                                         +*/
/*+               If handles still open, we return EDD_STS_ERR_SEQUENCE.    +*/
/*+                                                                         +*/
/*+               When closing the device, the systemadaption have to make  +*/
/*+               sure, that no more interrupts come in for this device.    +*/
/*+               (i.e. with this DDB)                                      +*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+        Note:  If a subcomponent signals an error on close. we continue  +*/
/*+               but this is a fatal situation. we can not reopen the      +*/
/*+               device on error!                                          +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID  EDDP_LOCAL_FCT_ATTR EDDP_CoreDevCloseRequest(
    EDD_UPPER_RQB_PTR_TYPE      pRQB)
{
    EDD_RSP                         Response;
    EDDP_UPPER_DEV_CLOSE_PTR_TYPE   pRqbParam;
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB;
    LSA_UINT32                      i;
    LSA_BOOL                        Ready;
    EDDP_DBG_DETAIL_ERR_TYPE        ErrDetail = {0,0,0,0};

    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CoreDevCloseRequest(pRQB=0x%X)",
                       pRQB);

    Response = EDD_STS_OK;

    if (LSA_HOST_PTR_ARE_EQUAL(pRQB->pParam, LSA_NULL))
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_CORE_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CoreDevCloseRequest(): ERROR -> RQB->pParam is NULL!");
        EDD_RQB_SET_RESPONSE(pRQB, EDD_STS_ERR_PARAM);
        EDDP_RQB_ERROR(pRQB);
    }
    else
    {
        pRqbParam = (EDDP_UPPER_DEV_CLOSE_PTR_TYPE) pRQB->pParam;

        /* get pDDB from hDDB with validation */
        Response = EDDP_CoreDevGetDDB (pRqbParam->hDDB, &pDDB);

        if (Response != EDD_STS_OK)
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR_LOCAL(&ErrDetail,Response);
            EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CoreDevCloseRequest(): ERROR -> Invalid hDDB=0x%X!",
                               pRqbParam->hDDB);
        }
        else
        {
            /* check if close is allowed */
            if (pDDB->DevIsSetup)  /* Device still setup?    */
            {
                Response = EDD_STS_ERR_SEQUENCE;  /* Shutdown device first!   */
                EDDP_DBG_SET_DETAIL_ERR_LOCAL(&ErrDetail,Response);
                EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CoreDevCloseRequest(): ERROR -> Device still Setup (hDDB=0x%X)!", pRqbParam->hDDB);
            }
            else
            {
                i       = 0;
                Ready   = LSA_FALSE;

                /* -------------------------------------------------------*/
                /* close all opened components from fct-table             */
                /* -------------------------------------------------------*/
                while (! Ready)
                {
                    /* LSA_NULL if table end.. */
                    if (! (0 == EDDP_CoreDeviceCloseFctTable[i].DeviceCloseFct))
                    {
                        if (pDDB->Core.CmpOpen & EDDP_CoreDeviceCloseFctTable[i].OpenMsk)
                        {
                            EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW,"[H:--] EDDP_CoreDevCloseRequest[I:%d]. Calling DeviceCloseFct", i);

                            /* Call Subcomponente DeviceCloseFct*/
                            Response = EDDP_CoreDeviceCloseFctTable[i].DeviceCloseFct(pDDB);

                            if (Response != EDD_STS_OK) /* here no error expected!*/
                            {
                                EDDP_CORE_TRACE_02(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"[H:--] EDDP_CoreDevCloseRequest[I:%d]. Calling DeviceCloseFct failed: Response=0x%X",
                                                   i, Response);
                                /* Copy ErrDetail from DDB to ErrDetail for later use */
                                EDDP_DBG_COPY_DETAIL_ERR_DDB_LOCAL(pDDB,&ErrDetail);

                                Ready = LSA_TRUE;
                            }
                            else
                            {
                                pDDB->Core.CmpOpen &= ~ EDDP_CoreDeviceCloseFctTable[i].OpenMsk;
                                EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE_LOW,"[H:--] EDDP_CoreDevCloseRequest[I:%d]. Calling DeviceCloseFct successful", i);
                            }
                        }
                        else
                        {
                            EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_LOW,"[H:--] EDDP_CoreDevCloseRequest[I:%d]. Already Closed. Skipping.", i);
                        }
                    }
                    else
                    {
                        Ready = LSA_TRUE;
                    }

                    i++;
                }

                /* Close the DDB                                                     */
                /* On error we cant recover! This should be handled as fatal by user */
                if (Response == EDD_STS_OK)
                {
                    EDDP_CoreDevRemoveFromMgm(pDDB);     /* Remove DDB from EDD-management */
                    EDDP_CoreDevCloseDDB(pDDB); /* Close the DDB and free memory  */
                }
                else
                {
                    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CoreDevCloseRequest(): ERROR -> Response=0x%X",
                                       Response);
                }
            }
        }

        /* finished the request. Call CBF */
        EDDP_SYSTEM_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE,"[H:--] <-- Request: EDDP_SRV_DEV_CLOSE finished. Response=0x%X",
                             Response);
        EDD_RQB_SET_RESPONSE(pRQB, Response);
        EDDP_GlbCallCbfLow(pRqbParam->Cbf,pRQB,ErrDetail.ModuleId,ErrDetail.LineNr);
    }

    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CoreDevCloseRequest(): Response=0x%X", Response);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreDevSetupRequest                    +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE     pRQB             +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         Opcode:     EDD_OPC_SYSTEM                  +*/
/*+     LSA_HANDLE_TYPE         Handle:                                     +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user (not used)           +*/
/*+     EDD_SERVICE             Service:    EDDP_SRV_DEV_SETUP              +*/
/*+     LSA_RESPONSE_TYPE       Status:     Return status                   +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:     Depend on kind of request.      +*/
/*+                                                                         +*/
/*+  pParam points to EDDP_RQB_DEV_SETUP_TYPE                               +*/
/*+                                                                         +*/
/*+     hDDB     : valid Device-Handle                                      +*/
/*+     pDSB     : Pointer to device-specific initialization parameters     +*/
/*+                (input - variables !)                                    +*/
/*+     Cbf      : Call-back-function. Called when Device-open is finished. +*/
/*+                                                                         +*/
/*+  RQB-return values via callback-function:                               +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         opcode:     EDD_OPC_SYSTEM                  +*/
/*+     LSA_HANDLE_TYPE         Handle:                                     +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user                      +*/
/*+     EDD_SERVICE             Service:                                    +*/
/*+     LSA_RESPONSE_TYPE       Status:     EDD_STS_OK                      +*/
/*+                                         EDD_STS_ERR_PARAM               +*/
/*+                                         EDD_STS_ERR_SEQUENCE            +*/
/*+                                         EDD_STS_ERR_RESOURCE            +*/
/*+                                         ....                            +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Request to setup a device. This request has to be called  +*/
/*+               after a open_device-call.                                 +*/
/*+               This call setup the global device hardware.               +*/
/*+               because this may cause interrupts, interrupts             +*/
/*+               have to be enabled before calling.                        +*/
/*+                                                                         +*/
/*+               This request maybe asynchronous!                          +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID  EDDP_LOCAL_FCT_ATTR EDDP_CoreDevSetupRequest(
    EDD_UPPER_RQB_PTR_TYPE      pRQB)
{
    EDD_RSP                         Response;
    EDDP_UPPER_DEV_SETUP_PTR_TYPE   pRqbParam;
    EDDP_UPPER_DSB_PTR_TYPE         pDSB = LSA_NULL;
    EDDP_LOCAL_DDB_PTR_TYPE         pDDB;

    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] IN: EDDP_CoreDevSetupRequest(pRQB=0x%X)",
                       pRQB);

    Response = EDD_STS_OK;

    if (LSA_HOST_PTR_ARE_EQUAL(pRQB->pParam, LSA_NULL))
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_CORE_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CoreDevSetupRequest(): ERROR -> RQB->pParam is NULL!");
        EDD_RQB_SET_RESPONSE(pRQB, EDD_STS_ERR_PARAM);
        EDDP_RQB_ERROR(pRQB);
    }
    else
    {
        pRqbParam = (EDDP_UPPER_DEV_SETUP_PTR_TYPE) pRQB->pParam;

        /* get pDDB from hDDB with validation */
        Response = EDDP_CoreDevGetDDB (pRqbParam->hDDB, &pDDB);

        if (Response != EDD_STS_OK)
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CoreDevSetupRequest(): ERROR -> Invalid hDDB=0x%X!",
                               pRqbParam->hDDB);
        }
        else
        {
		    pDSB = pRqbParam->pDSB;      /* pointer to hardware specific parameters */

            if (0 == pRqbParam->Cbf)
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
                EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CoreDevSetupRequest(): ERROR -> CBF must not be 0!");
            }
            else
            {
                /* check if device is not already setup */
                if (pDDB->DevIsSetup)
                {
                    Response = EDD_STS_ERR_SEQUENCE;  /* Shutdown device first!   */
                    EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
                    EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CoreDevSetupRequest(): ERROR -> Device already Setup (hDDB=0x%X)!",
                                       pRqbParam->hDDB);
                }
                else
                {
                    /* we only allow one device Setup running. no queueing! */
                    if (pDDB->Core.Setup.State != EDDP_DDB_STATE_SETUP_IDLE)
                    {
                        Response = EDD_STS_ERR_SEQUENCE;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB,Response);
                        EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,"[H:--] EDDP_CoreDevSetupRequest(): ERROR -> DeviceSetup already running! (hDDB=0x%X)!",
                                           pRqbParam->hDDB);
                    }
                }
            }
        }

        if (Response == EDD_STS_OK)
        {
            /*--------------------------------------------------------*/
            /* Start Statemachine for DeviceSetup                     */
            /*--------------------------------------------------------*/
            pDDB->Core.Setup.State  = EDDP_DDB_STATE_SETUP_START;
            pDDB->Core.Setup.pRQB   = pRQB;
            pDDB->Core.Setup.pDSB   = pDSB;

            EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"[H:--] EDDP_CoreDevSetupRequest(): Starting Statemachine for DEV_SETUP");

            EDDP_CoreDevSetupStateMachine (pDDB, EDD_STS_OK);
        }
        else
        {
            /* finished the request. Call CBF */
            EDDP_SYSTEM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,"[H:--] <-- Request: EDDP_SRV_DEV_SETUP finished. Response=0x%X",
                                 Response);
            EDD_RQB_SET_RESPONSE(pRQB, Response);
            EDDP_GlbCallCbf(pRqbParam->Cbf,pRQB,pDDB); /* pDDB maybe NULL here */
        }
    }

    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "[H:--] OUT: EDDP_CoreDevSetupRequest(): Response=0x%X", Response);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+                                                                         +*/
/*+  Functionname          :    EDDP_CoreDevShutdownRequest                 +*/
/*+  Input/Output          :    EDD_UPPER_RQB_PTR_TYPE     pRQB             +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+  pRQB       : Pointer to Upper-RQB with Requestparameters               +*/
/*+                                                                         +*/
/*+  RQB-parameters:                                                        +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         Opcode:     EDD_OPC_SYSTEM                  +*/
/*+     LSA_HANDLE_TYPE         Handle:                                     +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user (not used)           +*/
/*+     EDD_SERVICE             Service:    EDDP_SRV_DEV_SHUTDOWN           +*/
/*+     LSA_RESPONSE_TYPE       Status:     Return status                   +*/
/*+     EDD_UPPER_MEM_PTR_TYPE  pParam:     Depend on kind of request.      +*/
/*+                                                                         +*/
/*+  pParam points to EDDP_RQB_DEV_SETUP_TYPE                               +*/
/*+                                                                         +*/
/*+     hDDB     : valid Device-Handle                                      +*/
/*+     Cbf      : Call-back-function. Called when Device-open is finished. +*/
/*+                                                                         +*/
/*+  RQB-return values via callback-function:                               +*/
/*+     RQB-header:                                                         +*/
/*+     LSA_OPCODE_TYPE         opcode:     EDD_OPC_SYSTEM                  +*/
/*+     LSA_HANDLE_TYPE         Handle:                                     +*/
/*+     LSA_USER_ID_TYPE        UserId:     ID of user                      +*/
/*+     EDD_SERVICE             Service:                                    +*/
/*+     LSA_RESPONSE_TYPE       Status:     EDD_STS_OK                      +*/
/*+                                         EDD_STS_ERR_PARAM               +*/
/*+                                         EDD_STS_ERR_SEQUENCE            +*/
/*+                                         EDD_STS_ERR_RESOURCE            +*/
/*+                                         ....                            +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Request to shutdown a device. This request has to be      +*/
/*+               called from a shutdown device-call.                       +*/
/*+               This call setup shutdown the global device hardware.      +*/
/*+                                                                         +*/
/*+               This request maybe asynchronous!                          +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LSA_VOID  EDDP_LOCAL_FCT_ATTR EDDP_CoreDevShutdownRequest(
    EDD_UPPER_RQB_PTR_TYPE      pRQB)
{
    EDD_RSP Response;
    EDDP_UPPER_DEV_SHUTDOWN_PTR_TYPE pRqbParam;
    EDDP_LOCAL_DDB_PTR_TYPE pDDB;

    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT,
            "[H:--] IN: EDDP_CoreDevShutdownRequest(pRQB=0x%X)", pRQB);

    Response = EDD_STS_OK;
    if (LSA_HOST_PTR_ARE_EQUAL(pRQB->pParam, LSA_NULL))
    {
        Response = EDD_STS_ERR_PARAM;
        EDDP_CORE_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR,
                "[H:--] EDDP_CoreDevShutdownRequest(): ERROR -> RQB->pParam is NULL!");
        EDD_RQB_SET_RESPONSE(pRQB, EDD_STS_ERR_PARAM);
        EDDP_RQB_ERROR(pRQB);
    }
    else
    {
        pRqbParam = (EDDP_UPPER_DEV_SHUTDOWN_PTR_TYPE) pRQB->pParam;

        /* get pDDB from hDDB with validation */
        Response = EDDP_CoreDevGetDDB(pRqbParam->hDDB, &pDDB);

        if (Response != EDD_STS_OK)
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR,
                    "[H:--] EDDP_CoreDevShutdownRequest(): ERROR -> Invalid hDDB=0x%X!",
                    pRqbParam->hDDB);
        }
        else
        {
            if (0 == pRqbParam->Cbf)
            {
                Response = EDD_STS_ERR_PARAM;
                EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR,
                        "[H:--] EDDP_CoreDevShutdownRequest(): ERROR -> CBF must not be 0!");
            }
            else
            {
                /* we only allow one device Shutdown running. no queueing! */
                if (pDDB->Core.Shutdown.State != EDDP_DDB_STATE_SETUP_IDLE)
                {
                    Response = EDD_STS_ERR_SEQUENCE;
                    EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                    EDDP_CORE_TRACE_01(pDDB->TraceIdx,
                            LSA_TRACE_LEVEL_ERROR,
                            "[H:--] EDDP_CoreDevShutdownRequest(): ERROR -> DeviceShutdown already running! (hDDB=0x%X)!",
                            pRqbParam->hDDB);
                }
                else
                {
                    /* check if device is setup (not allowed)*/
                    if (!pDDB->DevIsSetup)
                    {
                        Response = EDD_STS_ERR_SEQUENCE;
                        EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
                        EDDP_CORE_TRACE_01(pDDB->TraceIdx,
                                LSA_TRACE_LEVEL_ERROR,
                                "[H:--] EDDP_CoreDevShutdownRequest(): ERROR -> Device still setup! (hDDB=0x%X)!",
                                pRqbParam->hDDB);
                    }
                }
            }
        }
#ifdef LSAS_CFG_USE_POF
        // ***** DeInitialization of the I2C module
        if (Response == EDD_STS_OK)
        {
            Response = EDDP_I2C_DeInit(pDDB);
        }
#endif
        if (Response == EDD_STS_OK)
        {
            /* only if device is setup */
            if (pDDB->DevIsSetup)
            {
                /*--------------------------------------------------------*/
                /* Start Statemachine for DeviceShutdown                  */
                /*--------------------------------------------------------*/
                pDDB->Core.Shutdown.State = EDDP_DDB_STATE_SHUTDOWN_START;
                pDDB->Core.Shutdown.pRQB = pRQB;

                EDDP_CORE_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,
                        "[H:--] EDDP_CoreDevShutdownRequest(): Starting Statemachine for DEV_SHUTDOWN");

                EDDP_CoreDevShutdownStateMachine(pDDB, EDD_STS_OK);
            }
            else /* device not setup. of IF is still setup.just finish */
            {
                EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,
                        "[H:--] EDDP_CoreDevShutdownRequest(). Device not setup. Just finish. (hDDB=0x%X)!",
                        pRqbParam->hDDB);
                /* finished the request. Call CBF */
                EDDP_CORE_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,
                        "[H:--] <-- Request: EDDP_SRV_DEV_SHUTDOWN finished. Response: 0x%X",
                        Response);
                EDD_RQB_SET_RESPONSE(pRQB, Response);
                EDDP_GlbCallCbf(pRqbParam->Cbf, pRQB, LSA_NULL);
            }
        }
        else
        {
            /* finished the request. Call CBF */
            EDDP_SYSTEM_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_NOTE,
                    "[H:--] <-- Request: EDDP_SRV_DEV_SHUTDOWN finished. Response=0x%X",
                    Response);
            EDD_RQB_SET_RESPONSE(pRQB, Response);
            EDDP_GlbCallCbf(pRqbParam->Cbf, pRQB, pDDB); /* pDDB maybe NULL here */
        }
    }
    EDDP_CORE_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT,
            "[H:--] OUT: EDDP_CoreDevShutdownRequest(): Response=0x%X",
            Response);
}

/*****************************************************************************/
/*  end of file eddp_core_dev.c                                              */
/*****************************************************************************/
