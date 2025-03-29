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
/*  F i l e               &F: eddp_phy.c                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:  System adaption for EDDP PHY interface           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  11.11.08    AB    initial version                                        */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID   111
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_PHY */

#include "eddp_inc.h"
#include "eddp_int.h"

#ifdef EDDP_CFG_USE_DEFAULT_PHY_ADAPTION

#ifdef EDDP_CFG_PHY_NSC_SUPPORT
    #include "eddp_phy_nsc.h"
#endif

#ifdef EDDP_CFG_PHY_NEC_SUPPORT
    #include "eddp_phy_nec.h"
#endif

#ifdef EDDP_CFG_PHY_BCM5461S_SUPPORT
    #include "eddp_phy_bcm5461s.h"
#endif

#ifdef EDDP_CFG_PHY_TI_SUPPORT
    #include "eddp_phy_ti.h"
#endif

#ifdef EDDP_CFG_PHY_RZT2_SUPPORT
	#include "rzt2_eddp_phy.h"
#endif

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)

LSA_UINT8   g_EDDP_PHY_MDI_X_MAPPING[EDDP_CFG_MAX_DEVICES][EDDP_MAX_PORT_CNT];
LSA_UINT8   g_EDDP_PHY_ISWIRELESS[EDDP_CFG_MAX_DEVICES][EDDP_MAX_PORT_CNT];
LSA_UINT16  g_EDDP_PHY_MAUTYPE[EDDP_CFG_MAX_DEVICES][EDDP_MAX_PORT_CNT];
LSA_UINT8   g_EDDP_PHY_MEDIATYPE[EDDP_CFG_MAX_DEVICES][EDDP_MAX_PORT_CNT];	//array port-index = HWPortIdx
LSA_UINT8   g_EDDP_PHY_OPTICALTRANSTYPE[EDDP_CFG_MAX_DEVICES][EDDP_MAX_PORT_CNT];
LSA_UINT8   g_EDDP_PHY_FXTRANSCEIVERTYPE[EDDP_CFG_MAX_DEVICES][EDDP_MAX_PORT_CNT];
LSA_UINT8   g_EDDP_PHY_LINK_SPEED_MODE_CONFIGURED[EDDP_CFG_MAX_DEVICES][EDDP_MAX_PORT_CNT];


/* ------------------------------------------------------------------------ */
/* Supported "OUI unequal values"                                           */
/* ------------------------------------------------------------------------ */
#define EDDP_PHY_OUI_UNEQUAL_VALUE_ERTEC200P                    0x00000000
#define EDDP_PHY_OUI_UNEQUAL_VALUE_ERTEC200P_FPGA               0xFFFFFFFF





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_PHY_GetPhyAddress                      +*/
/*+  Input/Output          :    EDDP_HANDLE			hDDB                    +*/
/*+                        :    EDDP_SYS_HANDLE     hSysDev                 +*/
/*+                        :    LSA_UINT32			HWPortID                +*/
/*+                        :    LSA_UINT32*         pPhyAddress             +*/
/*+                        :    LSA_UINT32*         pOUI1RegAddress         +*/
/*+                        :    LSA_UINT32*         pOUI2RegAddress         +*/
/*+                        :    LSA_UINT32*         pOUIUnequalValue        +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+   hDDB              : EDDP Handle                                  (in) +*/
/*+   hSysDev           : Handle of system adaption                    (in) +*/
/*+   HWPortID          : Hardware port ID of the related PHY          (in) +*/
/*+   pPhyAddress       : PHY (SMI) address belonging to this HWPortID (out)+*/
/*+   pOUI1RegAddress   : PHY reg addr containing low bits of OUI      (out)+*/
/*+   pOUI2RegAddress   : PHY reg addr containing high bits of OUI     (out)+*/
/*+   pOUIUnequalValue  : Compare value for OUI                        (out)+*/
/*+                                                                         +*/
/*+  Results            : EDD_STS_OK                                        +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function returns the PHY address (used for the SMI   +*/
/*+               interface) of the passed hardware port ID. The start-up   +*/
/*+               polling mechanism of the PNIP then continuously reads the +*/
/*+               two PHY registers specified by pOUI1RegAddress and        +*/
/*+               pOUI2RegAddress until the read value is no longer equal to+*/
/*+               pOUIUnequalValue. Standard values are:                    +*/
/*+               pOUI1RegAddress : 2                                       +*/
/*+               pOUI2RegAddress : 3                                       +*/
/*+               pOUIUnequalValue: 0xFFFFFFFF                              +*/
/*+                                                                         +*/
/*+       Note:   EDDP expects this function to return with success. Any    +*/
/*+               other return value than �EDD_STS_OK� will be interpreted  +*/
/*+               as a fatal error.                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_GetPhyAddress(
    EDDP_HANDLE			hDDB,
    EDDP_SYS_HANDLE     hSysDev,
    LSA_UINT32			HWPortID,
    LSA_UINT32          PHYAddressOffset,
    LSA_UINT32*         pPhyAddress,
    LSA_UINT32*         pOUI1RegAddress,
    LSA_UINT32*         pOUI2RegAddress,
    LSA_UINT32*         pOUIUnequalValue)
{
    EDD_RSP                     Response;
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB;

    LSA_UNUSED_ARG(hSysDev);
    LSA_UNUSED_ARG(hDDB);

    Response    = EDD_STS_OK;
    pDDB        = (EDDP_LOCAL_DDB_PTR_TYPE) hDDB;

/*
    switch (OUIValue)
    {
        #ifdef EDDP_CFG_PHY_NSC_SUPPORT
        case ((EDDP_PHY_NSC_OUI2COMPAREVALUE << 16) | EDDP_PHY_NSC_OUI1COMPAREVALUE):
        {
            // HWPortID are:                1,2,3,4
            // NSC PHY addresses are:       0,1,2,3
            *pPhyAddress = HWPortID-1;
        }
        break;
        #endif

        #ifdef EDDP_CFG_PHY_NEC_SUPPORT
        case ((EDDP_PHY_NEC_OUI2COMPAREVALUE << 16) | EDDP_PHY_NEC_OUI1COMPAREVALUE):
        {
            // HWPortID are:                1,2,3,4
            // NEC PHY addresses are:       1,0,3,2 --> COB module without adapter PCB (ports crossed)
            // *pPhyAddress = (HWPortID-1)^1; 
            // NEC PHY addresses are:       0,1,2,3 --> COB module with adapter PCB (ports like NSC --> ok)
            *pPhyAddress = HWPortID-1;
        }
        break;
        #endif

        default:
        {
            // ***** default address to detect the OUI from a PHY
            // unknown address mapping, try 0,1,2,3
            *pPhyAddress = HWPortID-1;
        }
        break;
    }
*/

    *pPhyAddress = (HWPortID - 1) + PHYAddressOffset;

    // This register addresses are defined by IEEE and should be always 2 and 3
    *pOUI1RegAddress = 2;
    *pOUI2RegAddress = 3;

    switch (pDDB->Hardware)
    {
        case EDDP_HW_ERTEC200P:
        {
            // Usually the SMI interface reads 0x0000 when accessing a PHY register when the PHY is not ready
            *pOUIUnequalValue = EDDP_PHY_OUI_UNEQUAL_VALUE_ERTEC200P;
        }
        break;

        case EDDP_HW_ERTEC200P_FPGA:
        case EDDP_HW_HERA_FPGA:    
        {
            // Usually the SMI interface reads 0xFFFF when accessing a PHY register when the PHY is not ready
            *pOUIUnequalValue = EDDP_PHY_OUI_UNEQUAL_VALUE_ERTEC200P_FPGA;
        }
        break;

        default:
        {
            Response = EDD_STS_ERR_PARAM;
            EDDP_DBG_SET_DETAIL_ERR(pDDB, Response);
            EDDP_SWI_TRACE_01(pDDB->TraceIdx, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PHY_GetPhyAddress(): ERROR -> HardwareType(0x%X) not supported!", 
                pDDB->Hardware);
        }
        break;
    }

    return (Response);
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_PHY_GetFunctionPointers                +*/
/*+  Input/Output          :    EDDP_HANDLE			    hDDB                +*/
/*+                        :    EDDP_SYS_HANDLE		    hSysDev             +*/
/*+                        :    LSA_UINT32			    HWPortID            +*/
/*+                        :    LSA_UINT32              OUIValue            +*/
/*+                        :    EDDP_PHY_FCT_PTR_TYPE   pFunctionPointers   +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+   hDDB               : EDDP Handle                                 (in) +*/
/*+   hSysDev            : Handle of system adaption                   (in) +*/
/*+   HWPortID           : Hardware port ID of the related PHY         (in) +*/
/*+   OUIValue           : Read OUI reg value from start-up mechanism  (in) +*/
/*+   pFunctionPointers  : Pointer to struct of PHY function pointers  (out)+*/
/*+                                                                         +*/
/*+  Results            : EDD_STS_OK        (PHY is supported)              +*/
/*+                     : EDD_STS_ERR_PARAM (PHY is NOT supported)          +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This is a synchron outbound function which returns a      +*/
/*+               structure of function pointers that are provided by system+*/
/*+               adaption in order to control the PHYs. System adaption can+*/
/*+               select the right function by evaluating the OUIValue      +*/
/*+               parameter (vendor register value).                        +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_GetFunctionPointers(
    EDDP_HANDLE			    hDDB,
    EDDP_SYS_HANDLE		    hSysDev,
    LSA_UINT32			    HWPortID,
    LSA_UINT32              OUIValue,
    EDDP_PHY_FCT_PTR_TYPE   pFunctionPointers,
    EDDP_PHY_TYPE      *    pPHYType)
{

    EDDP_LOCAL_DDB_PTR_TYPE     pDDB;

    LSA_UNUSED_ARG(hSysDev);

    pDDB        = (EDDP_LOCAL_DDB_PTR_TYPE) hDDB;

    switch (OUIValue)
    {
        #ifdef EDDP_CFG_PHY_NSC_SUPPORT
        case ((EDDP_PHY_NSC_OUI2COMPAREVALUE << 16) | EDDP_PHY_NSC_OUI1COMPAREVALUE):
        {
            *pPHYType                                               = EDDP_PHY_TYPE_NSC;
            pDDB->SWI.PortParams[HWPortID].IsGBitPhy                = EDDP_PHY_PORT_IS_GBIT_FALSE;
            pFunctionPointers->EDDP_PHY_Init                        = EDDP_PHY_NSC_Init;
            pFunctionPointers->EDDP_PHY_SetPowerDown                = EDDP_PHY_NSC_SetPowerDown;
            pFunctionPointers->EDDP_PHY_CheckPowerDown              = EDDP_PHY_NSC_CheckPowerDown;
            pFunctionPointers->EDDP_PHY_LED_BlinkBegin              = EDDP_PHY_NSC_LED_BlinkBegin;
            pFunctionPointers->EDDP_PHY_LED_BlinkEnd                = EDDP_PHY_NSC_LED_BlinkEnd;
            pFunctionPointers->EDDP_PHY_LED_BlinkSetMode            = EDDP_PHY_NSC_LED_BlinkSetMode;
            pFunctionPointers->EDDP_PHY_LoadDelayValues             = EDDP_PHY_NSC_LoadDelayValues;
            pFunctionPointers->EDDP_PHY_GetLinkStatus               = EDDP_PHY_NSC_GetLinkStatus;
            pFunctionPointers->EDDP_PHY_SetSpeedDuplexityOrAutoneg  = EDDP_PHY_NSC_SetSpeedDuplexityOrAutoneg;
            pFunctionPointers->EDDP_PHY_CheckMAUType                = EDDP_PHY_NSC_CheckMAUType;
            pFunctionPointers->EDDP_PHY_Get_Media_Type              = EDDP_PHY_NSC_Get_Media_Type;
            pFunctionPointers->EDDP_PHY_Get_MAU_Type                = EDDP_PHY_NSC_GET_MAU_TYPE;
        }
        break;
        #endif

        #ifdef EDDP_CFG_PHY_NEC_SUPPORT
        case ((EDDP_PHY_NEC_OUI2COMPAREVALUE << 16) | EDDP_PHY_NEC_OUI1COMPAREVALUE):
        {
            if( EDDP_HAL_HW_HERA == EDDP_HALGetChipVersion(pDDB) ) 
              
            {
                EDDP_SWI_TRACE_00(pDDB->TraceIdx, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_SWI_FinishPHYSetup(): FATAL -> prohibit NEC PHY detected for HERA IFB!");
                EDDP_DBG_SET_DETAIL_ERR(pDDB, EDDP_FATAL_ERR_NULL_PTR);        
                EDDP_FATAL(EDDP_FATAL_ERR_NULL_PTR);
                return EDD_STS_ERR_PARAM;
            }
            *pPHYType                                               = EDDP_PHY_TYPE_NEC;
            pDDB->SWI.PortParams[HWPortID].IsGBitPhy                = EDDP_PHY_PORT_IS_GBIT_FALSE;
            pFunctionPointers->EDDP_PHY_Init                        = EDDP_PHY_NEC_Init;
            pFunctionPointers->EDDP_PHY_SetPowerDown                = EDDP_PHY_NEC_SetPowerDown;
            pFunctionPointers->EDDP_PHY_CheckPowerDown              = EDDP_PHY_NEC_CheckPowerDown;
            pFunctionPointers->EDDP_PHY_LED_BlinkBegin              = EDDP_PHY_NEC_LED_BlinkBegin;
            pFunctionPointers->EDDP_PHY_LED_BlinkEnd                = EDDP_PHY_NEC_LED_BlinkEnd;
            pFunctionPointers->EDDP_PHY_LED_BlinkSetMode            = EDDP_PHY_NEC_LED_BlinkSetMode;
            pFunctionPointers->EDDP_PHY_LoadDelayValues             = EDDP_PHY_NEC_LoadDelayValues;
            pFunctionPointers->EDDP_PHY_GetLinkStatus               = EDDP_PHY_NEC_GetLinkStatus;
            pFunctionPointers->EDDP_PHY_SetSpeedDuplexityOrAutoneg  = EDDP_PHY_NEC_SetSpeedDuplexityOrAutoneg;
            pFunctionPointers->EDDP_PHY_CheckMAUType                = EDDP_PHY_NEC_CheckMAUType;
            pFunctionPointers->EDDP_PHY_Get_Media_Type              = EDDP_PHY_NEC_Get_Media_Type;
            pFunctionPointers->EDDP_PHY_Get_MAU_Type                = EDDP_PHY_NEC_GET_MAU_TYPE;
        }
        break;
        #endif
        
        #ifdef EDDP_CFG_PHY_BCM5461S_SUPPORT
        case ((EDDP_PHY_BCM5461S_OUI2COMPAREVALUE << 16) | EDDP_PHY_BCM5461S_OUI1COMPAREVALUE):
        {
            *pPHYType                                               = EDDP_PHY_TYPE_BCM5461S;
            pDDB->SWI.PortParams[HWPortID].IsGBitPhy                = EDDP_PHY_PORT_IS_GBIT_TRUE;
            pFunctionPointers->EDDP_PHY_Init                        = EDDP_PHY_BCM5461S_Init;
            pFunctionPointers->EDDP_PHY_SetPowerDown                = EDDP_PHY_BCM5461S_SetPowerDown;
            pFunctionPointers->EDDP_PHY_CheckPowerDown              = EDDP_PHY_BCM5461S_CheckPowerDown;
            pFunctionPointers->EDDP_PHY_LED_BlinkBegin              = EDDP_PHY_BCM5461S_LED_BlinkBegin;
            pFunctionPointers->EDDP_PHY_LED_BlinkEnd                = EDDP_PHY_BCM5461S_LED_BlinkEnd;
            pFunctionPointers->EDDP_PHY_LED_BlinkSetMode            = EDDP_PHY_BCM5461S_LED_BlinkSetMode;
            pFunctionPointers->EDDP_PHY_LoadDelayValues             = EDDP_PHY_BCM5461S_LoadDelayValues;
            pFunctionPointers->EDDP_PHY_GetLinkStatus               = EDDP_PHY_BCM5461S_GetLinkStatus;
            pFunctionPointers->EDDP_PHY_SetSpeedDuplexityOrAutoneg  = EDDP_PHY_BCM5461S_SetSpeedDuplexityOrAutoneg;
            pFunctionPointers->EDDP_PHY_CheckMAUType                = EDDP_PHY_BCM5461S_CheckMAUType;
            pFunctionPointers->EDDP_PHY_Get_Media_Type              = EDDP_PHY_BCM5461S_Get_Media_Type;
            pFunctionPointers->EDDP_PHY_Get_MAU_Type                = EDDP_PHY_BCM5461S_GET_MAU_TYPE;
        }
        break;
        #endif

        #ifdef EDDP_CFG_PHY_TI_SUPPORT
        case ((EDDP_PHY_TI_OUI2COMPAREVALUE << 16) | EDDP_PHY_TI_OUI1COMPAREVALUE):
        {
            *pPHYType                                               = EDDP_PHY_TYPE_TI;
            pDDB->SWI.PortParams[HWPortID].IsGBitPhy                = EDDP_PHY_PORT_IS_GBIT_FALSE;
            pFunctionPointers->EDDP_PHY_Init                        = EDDP_PHY_TI_Init;
            pFunctionPointers->EDDP_PHY_SetPowerDown                = EDDP_PHY_TI_SetPowerDown;
            pFunctionPointers->EDDP_PHY_CheckPowerDown              = EDDP_PHY_TI_CheckPowerDown;
            pFunctionPointers->EDDP_PHY_LED_BlinkBegin              = EDDP_PHY_TI_LED_BlinkBegin;
            pFunctionPointers->EDDP_PHY_LED_BlinkEnd                = EDDP_PHY_TI_LED_BlinkEnd;
            pFunctionPointers->EDDP_PHY_LED_BlinkSetMode            = EDDP_PHY_TI_LED_BlinkSetMode;
            pFunctionPointers->EDDP_PHY_LoadDelayValues             = EDDP_PHY_TI_LoadDelayValues;
            pFunctionPointers->EDDP_PHY_GetLinkStatus               = EDDP_PHY_TI_GetLinkStatus;
            pFunctionPointers->EDDP_PHY_SetSpeedDuplexityOrAutoneg  = EDDP_PHY_TI_SetSpeedDuplexityOrAutoneg;
            pFunctionPointers->EDDP_PHY_CheckMAUType                = EDDP_PHY_TI_CheckMAUType;
            pFunctionPointers->EDDP_PHY_Get_Media_Type              = EDDP_PHY_TI_Get_Media_Type;
            pFunctionPointers->EDDP_PHY_Get_MAU_Type                = EDDP_PHY_TI_GET_MAU_TYPE;
        }
        break;
        #endif
#ifdef EDDP_CFG_PHY_RZT2_SUPPORT
        case ((RZT2_PHY_OUI2COMPAREVALUE << 16) | RZT2_PHY_OUI1COMPAREVALUE):
			*pPHYType                                               = EDDP_PHY_TYPE_RZT2;
			pDDB->SWI.PortParams[HWPortID].IsGBitPhy                = EDDP_PHY_PORT_IS_GBIT_FALSE;
			pFunctionPointers->EDDP_PHY_Init                        = RZT2_PHY_Init;
			pFunctionPointers->EDDP_PHY_SetPowerDown                = RZT2_PHY_SetPowerDown;
			pFunctionPointers->EDDP_PHY_CheckPowerDown              = RZT2_PHY_CheckPowerDown;
			pFunctionPointers->EDDP_PHY_LED_BlinkBegin              = RZT2_PHY_LED_BlinkBegin;
			pFunctionPointers->EDDP_PHY_LED_BlinkEnd                = RZT2_PHY_LED_BlinkEnd;
			pFunctionPointers->EDDP_PHY_LED_BlinkSetMode            = RZT2_PHY_LED_BlinkSetMode;
			pFunctionPointers->EDDP_PHY_LoadDelayValues             = RZT2_PHY_LoadDelayValues;
			pFunctionPointers->EDDP_PHY_GetLinkStatus               = RZT2_PHY_GetLinkStatus;
			pFunctionPointers->EDDP_PHY_SetSpeedDuplexityOrAutoneg  = RZT2_PHY_SetSpeedDuplexityOrAutoneg;
			pFunctionPointers->EDDP_PHY_CheckMAUType                = RZT2_PHY_CheckMAUType;
			pFunctionPointers->EDDP_PHY_Get_Media_Type              = RZT2_PHY_Get_Media_Type;
			pFunctionPointers->EDDP_PHY_Get_MAU_Type                = RZT2_PHY_GET_MAU_TYPE;
        	break;
#endif
        default:
        {
            LSA_UNUSED_ARG(pFunctionPointers);
            EDDP_SWI_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT, "EDDP_PHY_GetFunctionPointers(HwPortID=%d, OUIValue=0x%X): ERROR -> This PHY is not supported!", 
                HWPortID, OUIValue);
            return (EDD_STS_ERR_PARAM);
        }
        break;
    }

    return (EDD_STS_OK);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname               :    EDDP_PHY_MapAutonegSpeedDuplexityToLinkSpeedModeConfigured             +*/
/*+  Input/Output               :    LSA_BOOL            Autoneg                                            +*/
/*+                             :    LSA_UINT32          Speed                                              +*/
/*+                             :    LSA_UINT32          Duplexity                                          +*/
/*+                             :    LSA_UINT8           pLinkSpeedModeConfigured                           +*/
/*+                                                                                                         +*/
/*+  Result                     :    EDD_RSP                                                                +*/
/*+---------------------------------------------------------------------------------------------------------+*/
/*+  Input/Output:                                                                                          +*/
/*+                                                                                                         +*/
/*+   Autoneg                   :    If Autoneg equal LSA_FALSE means autonegotiation is not supported (in) +*/
/*+   Speed                     :    data transfer rate of the link varies from 10 MBit to 10 GBit     (in) +*/
/*+   Duplexity                 :    data transfer direction either fullduplex or halfduplex           (in) +*/
/*+   pLinkSpeedModeConfigured  :    the result of the mapping                                        (out) +*/

/*+                                                                                                         +*/
/*+  Results                    : EDD_STS_OK        (valid input parameter)                                 +*/
/*+                             : EDD_STS_ERR_PARAM (not valid input parameter)                             +*/
/*+---------------------------------------------------------------------------------------------------------+*/
/*+  Description: This is a synchron help function which maps the input parameters--------------------------+*/
/*+                  Autoneg, Speed and Duplexity to a to suitable Link Speed Mode Configured               +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_MapAutonegSpeedDuplexityToLinkSpeedModeConfigured(
    const LSA_UINT8                 Autoneg,
    const LSA_UINT32                Speed,
    const LSA_UINT32                Duplexity,
    LSA_UINT8                       *pLinkSpeedModeConfigured)
{
    EDD_RSP ReturnCode=LSA_RSP_OK;
    if(EDD_AUTONEG_ON == Autoneg)
    {
        *pLinkSpeedModeConfigured=EDD_LINK_AUTONEG;
    }
    else
    {
        if(EDD_LINK_MODE_FULL==Duplexity)
        {
            switch(Speed)
            {
                case EDD_LINK_SPEED_10:
                {
                    *pLinkSpeedModeConfigured=EDD_LINK_10MBIT_FULL;
                    break;
                }
                case EDD_LINK_SPEED_100:
                {
                    *pLinkSpeedModeConfigured=EDD_LINK_100MBIT_FULL;
                    break;
                }
                case EDD_LINK_SPEED_1000 :
                {
                    *pLinkSpeedModeConfigured=EDD_LINK_1GBIT_FULL;
                    break;
                }
                case EDD_LINK_SPEED_10000 :
                {
                    *pLinkSpeedModeConfigured=EDD_LINK_10GBIT_FULL;
                    break;
                }
                default:
                {
                    ReturnCode=LSA_RSP_ERR_PARAM;
                }
            }
        }
        else if(EDD_LINK_MODE_HALF==Duplexity)
        {
            switch(Speed)
            {
                case EDD_LINK_SPEED_10:
                {
                    *pLinkSpeedModeConfigured=EDD_LINK_10MBIT_HALF;
                    break;
                }
                case EDD_LINK_SPEED_100:
                {
                    *pLinkSpeedModeConfigured=EDD_LINK_100MBIT_HALF;
                    break;
                }
                case EDD_LINK_SPEED_1000 :
                {
                    *pLinkSpeedModeConfigured=EDD_LINK_1GBIT_HALF;
                    break;
                }
                case EDD_LINK_SPEED_10000 :
                {
                    *pLinkSpeedModeConfigured=EDD_LINK_10GBIT_HALF;
                    break;
                }
                default:
                {
                    ReturnCode=LSA_RSP_ERR_PARAM;
                }
            }
        }
        else
        {
            ReturnCode=LSA_RSP_ERR_PARAM;
        }
    }
    return ReturnCode;
}


#endif //EDDP_CFG_USE_DEFAULT_PHY_ADAPTION

/*****************************************************************************/
/*  end of file sysp_phy.c                                                   */
/*****************************************************************************/
