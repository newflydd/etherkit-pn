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
/*  F i l e               &F: eddp_phy_nec.c                            :F&  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:													 */
/*                                                                           */
/*  This file contains sample implementations for PHY-specific functions	 */
/*  that have to be provided by system adaption.							 */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  15.12.08    AB    initial version                                        */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID   115
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_LL_PHY */

#include "eddp_inc.h"
#include "eddp_int.h"
#include "eddp_phy.h"

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)

#ifdef EDDP_CFG_PHY_RZT2_SUPPORT

#include "eddp_llif.h"
#include "bspadapt.h"


//#define EDDP_DISABLE_10MBIT_SUPPORT


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
    __attribute__((unused))EDDP_LOCAL_DDB_PTR_TYPE     pDDB;

    LSA_UNUSED_ARG(hSysDev);
    LSA_UNUSED_ARG(hDDB);

    Response    = EDD_STS_OK;
    pDDB        = (EDDP_LOCAL_DDB_PTR_TYPE) hDDB;

    if(LSA_NULL != pPhyAddress)
    {
    	*pPhyAddress = (HWPortID - 1) + PHYAddressOffset;
    }
    // This register addresses are defined by IEEE and should be always 2 and 3
    if(LSA_NULL != pOUI1RegAddress)
    {
    	*pOUI1RegAddress = 2;
    }
    if(LSA_NULL != pOUI2RegAddress)
    {
    	*pOUI2RegAddress = 3;
    }
    if(LSA_NULL != pOUIUnequalValue)
    {
    	*pOUIUnequalValue = EDDP_PHY_OUI_UNEQUAL_VALUE_ERTEC200P;
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
	EDD_RSP Status = EDD_STS_ERR_PARAM;
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB = (EDDP_LOCAL_DDB_PTR_TYPE) hDDB;;
    LSA_UNUSED_ARG(hSysDev);

    if( (0U < HWPortID) && (HWPortID <= EDDP_MAX_PORT_CNT) )
    {
		if(LSA_NULL != LSA_NULL)
		{
			*pPHYType = EDDP_PHY_TYPE_RZT2;
		}

		if(LSA_NULL != pDDB)
		{
			pDDB->SWI.PortParams[HWPortID].IsGBitPhy                = EDDP_PHY_PORT_IS_GBIT_FALSE;
		}

		if(LSA_NULL != pFunctionPointers)
		{
			pFunctionPointers->EDDP_PHY_Init                        = EDDP_LL_PHY_Init;
			pFunctionPointers->EDDP_PHY_SetPowerDown                = EDDP_LL_PHY_SetPowerDown;
			pFunctionPointers->EDDP_PHY_CheckPowerDown              = EDDP_LL_PHY_CheckPowerDown;
			pFunctionPointers->EDDP_PHY_LED_BlinkBegin              = EDDP_LL_PHY_LED_BlinkBegin;
			pFunctionPointers->EDDP_PHY_LED_BlinkEnd                = EDDP_LL_PHY_LED_BlinkEnd;
			pFunctionPointers->EDDP_PHY_LED_BlinkSetMode            = EDDP_LL_PHY_LED_BlinkSetMode;
			pFunctionPointers->EDDP_PHY_LoadDelayValues             = EDDP_LL_PHY_LoadDelayValues;
			pFunctionPointers->EDDP_PHY_GetLinkStatus               = EDDP_LL_PHY_GetLinkStatus;
			pFunctionPointers->EDDP_PHY_SetSpeedDuplexityOrAutoneg  = EDDP_LL_PHY_SetSpeedDuplexityOrAutoneg;
			pFunctionPointers->EDDP_PHY_CheckMAUType                = EDDP_LL_PHY_CheckMAUType;
			pFunctionPointers->EDDP_PHY_Get_Media_Type              = EDDP_LL_PHY_Get_Media_Type;
			pFunctionPointers->EDDP_PHY_Get_MAU_Type                = EDDP_LL_PHY_Get_MAU_Type;
		}

		Status = EDD_STS_OK;
    }

    return (Status);
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
                }break;
            }
        }
        else
        {
            ReturnCode=LSA_RSP_ERR_PARAM;
        }
    }
    return ReturnCode;
}



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_LL_PHY_Init                           +*/
/*+  Input/Output          :    EDDP_HANDLE				    hDDB            +*/
/*+                        :    EDDP_SYS_HANDLE			    hSysDev         +*/
/*+                        :    LSA_UINT32                  Hardware        +*/
/*+                        :    LSA_UINT32				    HwPortID        +*/
/*+                        :    LSA_BOOL				    IsWireless      +*/
/*+                        :    LSA_BOOL				    IsMIDX          +*/
/*+                        :    EDDP_PHY_INIT_PTR_TYPE      pInitData       +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+   hDDB       : EDDP Handle                                         (in) +*/
/*+   hSysDev    : Handle of system adaption                           (in) +*/
/*+   Hardware   : internal HW identifier                              (in) +*/
/*+   HwPortID   : Hardware port ID of the related PHY                 (in) +*/
/*+   IsWireless : comes from Setup block                              (in) +*/
/*+   IsMIDX     : comes from Setup block                              (in) +*/
/*+   pInitData  : Pointer to struct for PHY link status parameters    (out)+*/
/*+                                                                         +*/
/*+  Results     : EDD_STS_OK                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function shall return all parameters of a specific   +*/
/*+               PHY needed for link change information. This includes PHY +*/
/*+               register addresses and bit masks for link status, speed   +*/
/*+               mode and duplexity. System adaption may also access the   +*/
/*+               PHY in order to configure special parameters.             +*/
/*+                                                                         +*/
/*+       Note:   EDDP expects this function to return with success. Any    +*/
/*+               other return value than �EDD_STS_OK� will be interpreted  +*/
/*+               as a fatal error.                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_LL_PHY_Init(
	EDDP_HANDLE				    hDDB,
	EDDP_SYS_HANDLE			    hSysDev,
	LSA_UINT32                  Hardware,
    LSA_UINT32				    HwPortID,
    LSA_UINT8  	                IsWireless,
    LSA_UINT8  	                IsMIDX,
	EDDP_PHY_INIT_PTR_TYPE      pInitData)
{
  __attribute__((unused))LSA_UINT32      RegVal;
	LSA_UINT32      DevId;
	__attribute__((unused))LSA_BOOL        Fx2TxChange;

    Fx2TxChange = LSA_FALSE;
	DevId       = ((EDDP_LOCAL_DDB_PTR_TYPE)hDDB)->DevId;
	EDDP_ASSERT_FALSE(DevId >= EDDP_CFG_MAX_DEVICES);
 

    g_EDDP_PHY_ISWIRELESS[DevId][HwPortID-1] = IsWireless;

    EDDP_LL_PHY_Get_Media_Type(hDDB, hSysDev, HwPortID, &g_EDDP_PHY_MEDIATYPE[DevId][HwPortID-1], &g_EDDP_PHY_OPTICALTRANSTYPE[DevId][HwPortID-1], &g_EDDP_PHY_FXTRANSCEIVERTYPE[DevId][HwPortID - 1]);


    // Fx/Tx-mode setting (this should match the SCRB configuration)
    //      Reg18.10 = 0    : Tx-Mode
    //      Reg18.10 = 1    : Fx-Mode
	// Exception: FPGA Board. With FPGA Board the mode can only be set for both ports
	// Note: With Fx-mode no MDIX shall be set and setting is ignored!
    // read current value from register

    //EDDP_LL_PHY: eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_LL_PHY_REG_SPECIAL_MODES, &RegVal);

    if (g_EDDP_PHY_MEDIATYPE[DevId][HwPortID-1] == EDD_MEDIATYPE_FIBER_OPTIC_CABLE)
	{
        /* ------------------------------------ */
        /* MediaType = Fx-Mode (POF)            */
        /* ------------------------------------ */
            EDDP_SWI_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "EDDP_LL_PHY_Init(HwPortID=%d): FATAL -> OpticalTransType (%d) illegal value!",
                HwPortID, g_EDDP_PHY_OPTICALTRANSTYPE[DevId][HwPortID-1]);
            EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }
    else
    {
        /* ------------------------------------ */
        /* MediaType = Tx-Mode (Copper)         */
        /* ------------------------------------ */

        g_EDDP_PHY_MDI_X_MAPPING[DevId][HwPortID-1] = IsMIDX;
        // a global structure to transfer the linkSpeedModeConfigured between the phy service and the prm service.
        // for copper Phys it is initiliazed with autoneg
        g_EDDP_PHY_LINK_SPEED_MODE_CONFIGURED[DevId][HwPortID-1]= EDD_LINK_AUTONEG ;
    }

#if 1 //EDDP_LL_PHY:

#else
    // write new value to register
    eddp_ll_PhyRegisterWrite (hDDB, HwPortID, EDDP_LL_PHY_REG_SPECIAL_MODES, RegVal);

    // ***** JK optimization enable
    eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_LL_PHY_REG_SPECIAL_CTL, &RegVal);
    RegVal |= 0x0002;   // Reg31.1: '1' � rx_dv rises on "J" delimiter falls on "T" delimiter
    eddp_ll_PhyRegisterWrite (hDDB, HwPortID, EDDP_LL_PHY_REG_SPECIAL_CTL, RegVal);

    // ***** set values for BER-Counter
    eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_LL_PHY_REG_BER_CNT, &RegVal);
    RegVal &= 0x807F;       // clear bits 7:14 --> not really neccessary because remainings bits are read only
    if (g_EDDP_PHY_MEDIATYPE[DevId][HwPortID - 1] == EDD_MEDIATYPE_FIBER_OPTIC_CABLE)
    {
        //switch off BER monitor (see TFS2285430)
        RegVal |= (0x0 << 14);    // '0'      � set BER_CNT_LNK_DIS
        RegVal |= (0x0 << 11);    // '000'    
        RegVal |= (0x0 << 7);     // '0000'   � BER_Window 0 -> disabled
    }
    else
    {
        //switch on BER monitor for CO ports only
        RegVal |= (0x1<<14);    // '1'      � set BER_CNT_LNK_EN
        RegVal |= (0x3<<11);    // '011'    � BER_Cnt_Trig > 4 (trigger level for LinkUp/Down)
        RegVal |= (0x5<<7);     // '0101'   � BER_Window 1,6ms (lenght of BER counter)
    }
    eddp_ll_PhyRegisterWrite (hDDB, HwPortID, EDDP_LL_PHY_REG_BER_CNT, RegVal);

    #ifdef EDDP_DISABLE_10MBIT_SUPPORT
	// ***** Configure PHY for 100 MBit only
    eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_LL_PHY_REG_NEG_ADVERTISEMENT, &RegVal);
    /*
    Register 4: Auto-Negotiation Advertisement
    15  RW  NP          Next Page
    14  RO  RESERVED    
    13  RW  RF          Remote Fault
    12  RW  RESERVED    
    11  RW  PAUSE[1]    Pause Operation
    10  RW  PAUSE[0]            
    9   RO  T4          100BASE-T4 Support
    8   RW  TX_FD       100BASE-TX Full Duplex Support
    7   RW  TX          100BASE-TX Support
    6   RW  10_FD       10BASE-T Full Duplex Support
    5   RW  10          10BASE-T Support
    4:0 RW  SELECTOR    Protocol Selection Bits

    Note: Bits[8:5] set by PHY MODE
    */
    RegVal |= ( (1<<7) | (1<<8) );      // enable 100 MBit support
    RegVal &= ~( (1<<5) | (1<<6) );     // disable 10 MBit support

    eddp_ll_PhyRegisterWrite (hDDB, HwPortID, EDDP_LL_PHY_REG_NEG_ADVERTISEMENT, RegVal);
    #endif
#endif

    LSA_UNUSED_ARG(hSysDev);
    LSA_UNUSED_ARG(Hardware);

	return EDD_STS_OK;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_LL_PHY_LoadDelayValues                +*/
/*+  Input/Output          :    EDDP_HANDLE			            hDDB        +*/
/*+                        :    LSA_UINT32						Hardware    +*/
/*+                        :    EDDP_PHY_LINK_STATUS_PTR_TYPE   pLinkStatus +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+   hDDB                  : EDDP Handle                              (in) +*/
/*+   Hardware              : internal HW identifier                   (in) +*/
/*+   HwPortID              : Hardware port ID of the related PHY      (in) +*/
/*+                                                                         +*/
/*+   Elements of pLinkStatus:                                              +*/
/*+    MediaType            : Current Media Type                       (in) +*/
/*+    OpticalTransType     : Current Trans Type                       (in) +*/
/*+    RealPortTxDelay      : Average tx port delay of used hardware   (out)+*/
/*+    RealPortRxDelay      : Average rx port delay of used hardware   (out)+*/
/*+    MaxPortTxDelay       : Maximum tx port delay of used hardware   (out)+*/
/*+    MaxPortRxDelay       : Maximum rx port delay of used hardware   (out)+*/
/*+                                                                         +*/
/*+  Results                : EDD_STS_OK                                    +*/
/*+                           EDD_STS_ERR_PARAM                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_LL_PHY_LoadDelayValues(
	EDDP_HANDLE				        hDDB,
	LSA_UINT32						Hardware,
    EDDP_PHY_LINK_STATUS_PTR_TYPE   pLinkStatus)
{
	EDD_RSP Status = EDD_STS_ERR_PARAM;

    LSA_UNUSED_ARG(hDDB);

    if(LSA_NULL != pLinkStatus)
    {
		if (pLinkStatus->MediaType == EDD_MEDIATYPE_FIBER_OPTIC_CABLE)
		{
			EDDP_SWI_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "EDDP_LL_PHY_LoadDelayValues: FATAL -> FxTransceiverType (0x%X) illegal value!",
			pLinkStatus->FxTransceiverType);
			EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
		}
		else
		{
			// Tx-Mode
			pLinkStatus->RealPortTxDelay    = EDDP_LL_PHY_REAL_PORT_TX_DELAY;
			pLinkStatus->RealPortRxDelay    = EDDP_LL_PHY_REAL_PORT_RX_DELAY;
			pLinkStatus->MaxPortTxDelay     = EDDP_LL_PHY_MAX_PORT_TX_DELAY;
			pLinkStatus->MaxPortRxDelay     = EDDP_LL_PHY_MAX_PORT_RX_DELAY;
		}

		Status = EDD_STS_OK;
    }

    return Status;
}



/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname               :    EDDP_LL_PHY_SetSpeedDuplexityOrAutoneg     +*/
/*+  Input/Output               :    EDDP_HANDLE        hDDB                     +*/
/*+                             :    EDDP_SYS_HANDLE    hSysDev                  +*/
/*+                             :    LSA_UINT32         HwPortID                 +*/
/*+                             :    LSA_UINT32        *pLinkSpeedModeConfigured +*/
/*+                             :    LSA_UINT32         Speed                    +*/
/*+                             :    LSA_UINT32         Duplexity                +*/
/*+                             :    LSA_UINT8          Autoneg                  +*/
/*+                                                                              +*/
/*+  Result                     :    EDD_RSP                                     +*/
/*+------------------------------------------------------------------------------+*/
/*+  Input/Output:                                                               +*/
/*+                                                                              +*/
/*+   hDDB                      : EDDP Handle                          (in)      +*/
/*+   hSysDev                   : Handle of system adaption            (in)      +*/
/*+   HwPortID                  : Hardware port ID of the related PHY  (in)      +*/
/*+   *pLinkSpeedModeConfigured : Link speed and link mode configured in the phy,+*/
/*+                               it usually corresponds to the input parameters +*/
/*+                               below Speed, Duplexity and Autoneg, just in    +*/
/*+                               case of POF media, no auto-negotiation is      +*/
/*+                               supported,therefore pLinkSpeedModeConfigured   +*/
/*+                               refers to no autoneg modes                     +*/
/*+   Speed                     : Requested speed                      (in)      +*/
/*+   Duplexity                 : Requested duplexity                  (in)      +*/
/*+   Autoneg                   : Autoneg ON or OFF                    (in)      +*/
/*+                                                                              +*/
/*+  Results     : EDD_STS_OK                                                    +*/
/*+-------------------------------------------------------------------------     +*/
/*+  Description: This function sets speed mode and duplexity when the           +*/
/*+               Autoneg parameter is set to LSA_FALSE. If Autoneg is 			 +*/
/*+               set to LSA_TRUE Speed and Duplexity parameters are         	 +*/
/*+               irrelevant. In case the transceiver does not support           +*/
/*+               auto-negotiation it has to set up a connection at least at     +*/
/*+               100MBit full duplex and has to return for                  	 +*/
/*+               AutonegCapAdvertised (see EDDP_PHY_GetLinkStatus).           	 +*/
/*+                                                                              +*/
/*+       Note:   EDDP expects this function to return with success. Any         +*/
/*+               other return value than �EDD_STS_OK� will be interpreted       +*/
/*+               as a fatal error.                                              +*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_LL_PHY_SetSpeedDuplexityOrAutoneg(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID,
    LSA_UINT8               *pLinkSpeedModeConfigured,
    LSA_UINT32              Speed,
    LSA_UINT32              Duplexity,
    LSA_UINT8               Autoneg)
{
    __attribute__((unused))LSA_UINT32      RegVal;
    LSA_UINT32      DevId;
    EDD_RSP         ReturnCode=LSA_RSP_ERR_PARAM;
    LSA_UINT8       LinkSpeedModeConfigured = 0;
    LSA_UINT8       PhyDisabled = 0;
	DevId = ((EDDP_LOCAL_DDB_PTR_TYPE)hDDB)->DevId;
	EDDP_ASSERT_FALSE(DevId >= EDDP_CFG_MAX_DEVICES);
  	EDDP_LOCAL_DDB_PTR_TYPE     pDDB;
   EDDP_CoreDevGetDDB (hDDB, &pDDB);
#if 0
    eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_LL_PHY_REG_BCMR, &RegVal);
#endif

	// ***** configure for FIBER OPTIC always 100MB FD if Autoneg set
    if (g_EDDP_PHY_MEDIATYPE[DevId][HwPortID-1] == EDD_MEDIATYPE_FIBER_OPTIC_CABLE)
    {
        if (EDD_AUTONEG_ON == Autoneg)
        {
            // ***** patching input parameter for right setup
            Autoneg     = EDD_AUTONEG_OFF;
            Speed       = EDD_LINK_SPEED_100;
            Duplexity   = EDD_LINK_MODE_FULL;
        }
    }

#if 1
    RegVal = eddp_ll_PhyRegRead (0x01, HwPortID, EDDP_LL_PHY_REG_BCMR);
    if (pDDB->PRM.FirstCommitExecuted) 
    {
        if (EDD_AUTONEG_ON == Autoneg)
        {
            eddp_ll_PhyRegWrite (0x01, HwPortID, EDDP_LL_PHY_REG_BCMR, 0x1000 | RegVal/*enable Auto-Neg*/);
            //eddp_ll_PhyRegisterWrite (hDDB, HwPortID, EDDP_LL_PHY_REG_BCMR, (0x1000/*enable Auto-Neg*/));
            //eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_LL_PHY_REG_CONTROL, &RegVal);
            // ***** result depends on auto-neg settings
#ifdef UPD60620_PHY      
           LSA_UINT32 RegVal1 = eddp_ll_PhyRegRead (0x01, HwPortID, EDDP_LL_PHY_REG_CONTROL);
	          eddp_ll_PhyRegWrite (0x01, HwPortID, EDDP_LL_PHY_REG_CONTROL, RegVal1 | 0x0080 /*Auto Detect MDI/MDIX mode*/);
#endif
          /* restart AutoNegotiation
            eddp_ll_PhyRegisterRead(hDDB, HwPortID, EDDP_LL_PHY_REG_BCMR, &RegVal);
            eddp_ll_PhyRegisterWrite(hDDB, HwPortID, EDDP_LL_PHY_REG_BCMR, (RegVal | 0x200));// restart AutoNeg
        */
        }
        else
        {
            if (Speed == EDD_LINK_SPEED_100)
            {
                RegVal |=  0x2000/*100Mbit*/;
            }
            else if (Speed == EDD_LINK_SPEED_10)
            {
                RegVal &= ~0x2000/*10Mbit*/;
            }
            else
            {
                return EDD_STS_ERR_PARAM;
            }

            if (Duplexity == EDD_LINK_MODE_FULL)
            {
                RegVal |=  0x0100/*Full Duplex*/;
            }
            else if (Duplexity == EDD_LINK_MODE_HALF)
            {
               RegVal &= ~0x0100/*Half Duplex*/;
            }
            else
            {
                return EDD_STS_ERR_PARAM;
            }
            eddp_ll_PhyRegWrite (0x01, HwPortID, EDDP_LL_PHY_REG_BCMR, (RegVal & ~0x1000/*disable Auto-Neg*/));
            //eddp_ll_PhyRegisterWrite (hDDB, HwPortID, EDDP_LL_PHY_REG_BCMR, (RegVal & ~0x1000/*disable Auto-Neg*/));

          //  eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_LL_PHY_REG_CONTROL, &RegVal);
          //  RegVal &= ~0x0080; /*Manual MDI/MDIX mode*/
#if 0
        // Remove MDI control. This is done automatically in the PHY open function
            RegVal = eddp_ll_PhyRegRead (0x01, HwPortID, EDDP_LL_PHY_REG_CONTROL);
            RegVal &= ~0x0080; /*Manual MDI/MDIX mode*/
            if (g_EDDP_PHY_MDI_X_MAPPING[DevId][HwPortID-1])
            {
                // ***** force this port to operate in MDI-X mode
	           // eddp_ll_PhyRegisterWrite (hDDB, HwPortID, EDDP_LL_PHY_REG_CONTROL, (RegVal | 0x0040/*MDIX mode*/));
          	    eddp_ll_PhyRegWrite (0x01, HwPortID, EDDP_LL_PHY_REG_CONTROL, (RegVal | 0x0040/*MDIX mode*/));
            }
            else
            {
                // ***** force this port to operate in MDI mode
          	    eddp_ll_PhyRegWrite (0x01, HwPortID, EDDP_LL_PHY_REG_CONTROL, (RegVal & ~0x0040/*MDI mode*/));
	            //eddp_ll_PhyRegisterWrite (hDDB, HwPortID, EDDP_LL_PHY_REG_CONTROL, (RegVal & ~0x0040/*MDI mode*/));
            }
#endif        
        }
    }
#endif

    ReturnCode=EDDP_PHY_MapAutonegSpeedDuplexityToLinkSpeedModeConfigured(Autoneg,Speed,Duplexity,&LinkSpeedModeConfigured);
    EDDP_ASSERT(LSA_RSP_OK==ReturnCode);
    *pLinkSpeedModeConfigured = LinkSpeedModeConfigured,
    g_EDDP_PHY_LINK_SPEED_MODE_CONFIGURED[DevId][HwPortID-1]=LinkSpeedModeConfigured;
    LSA_UNUSED_ARG(hSysDev);

    ReturnCode = EDDP_LL_SET_LINK(((EDDP_LOCAL_DDB_PTR_TYPE)hDDB)->InterfaceID, HwPortID, LinkSpeedModeConfigured);

    return ReturnCode;
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_LL_PHY_CheckMAUType                   +*/
/*+  Input/Output          :    EDDP_HANDLE             hDDB                +*/
/*+                        :    EDDP_SYS_HANDLE         hSysDev             +*/
/*+                        :    LSA_UINT32              HwPortID            +*/
/*+                        :    LSA_UINT16              MAUType             +*/
/*+                        :    LSA_UINT32              *pSpeed             +*/
/*+                        :    LSA_UINT32              *pDuplexity         +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+   hDDB       : EDDP Handle                                         (in) +*/
/*+   hSysDev    : Handle of system adaption                           (in) +*/
/*+   HwPortID   : Hardware port ID of the related PHY                 (in) +*/
/*+   MAUType    : MAU Type to be checked                              (in) +*/
/*+   pSpeed     : Speed extracted from MAU Type                       (out)+*/
/*+   pDuplexity : Duplexity extracted from MAU Type                   (out)+*/
/*+                                                                         +*/
/*+  Results     : EDD_STS_OK        (MAUType is supported by hardware)     +*/
/*+              : EDD_STS_ERR_PARAM (MAUType is NOT supported by hardware) +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function checks if the given MAUType is supported by +*/
/*+               the hardware and returns the related Speed and Duplexity. +*/
/*+                                                                         +*/
/*+       Note:   EDDP expects this function to return with success. Any    +*/
/*+               other return value than �EDD_STS_OK� will be interpreted  +*/
/*+               as a fatal error.                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_LL_PHY_CheckMAUType(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID,
    LSA_UINT16              MAUType,
    LSA_UINT32              *pSpeed,
    LSA_UINT32              *pDuplexity)
{
    EDD_RSP     Result = EDD_STS_ERR_PARAM;
    LSA_UINT32      DevId;

	*pSpeed     = EDD_LINK_UNKNOWN;
	*pDuplexity = EDD_LINK_UNKNOWN;

	/* because we cant use hSysDev we use EDDP internal variables */
	DevId = ((EDDP_LOCAL_DDB_PTR_TYPE)hDDB)->DevId;
	EDDP_ASSERT_FALSE(DevId >= EDDP_CFG_MAX_DEVICES);

	LSA_UNUSED_ARG(hSysDev);

	// ***** FIBER_OPTIC_CABLE
	if ( g_EDDP_PHY_MEDIATYPE[DevId][HwPortID-1] == EDD_MEDIATYPE_FIBER_OPTIC_CABLE )
	{
		if( MAUType == EDDP_PHY_MAUTYPE_100BASEFXHD )         // 17/11h
		{   // ***** FIBER_OPTIC_CABLE
			*pSpeed = EDD_LINK_SPEED_100;
			*pDuplexity  = EDD_LINK_MODE_HALF;
			Result = EDD_STS_OK;
		}
		else if( MAUType == EDDP_PRM_PDPORT_DATA_MAUTYPE_100BASEPXFD )    // 54/36h
		{   // ***** FIBER_OPTIC_CABLE (POF)
			*pSpeed = EDD_LINK_SPEED_100;
			*pDuplexity  = EDD_LINK_MODE_FULL;
			Result = EDD_STS_OK;
		}
		else if (MAUType == EDDP_PHY_MAUTYPE_100BASEFXFD)
		{
			// ***** GOF
			*pSpeed     = EDD_LINK_SPEED_100;
			*pDuplexity = EDD_LINK_MODE_FULL;
			Result = EDD_STS_OK;
		}
	}
	else // ***** COPPER_CABLE & RADIO_COMMUNICATION
	{
		if( MAUType == EDDP_PHY_MAUTYPE_10BASETXHD    )         // 10/0Ah
		{   // ***** COPPER_CABLE & RADIO_COMMUNICATION
			*pSpeed = EDD_LINK_SPEED_10;
			*pDuplexity  = EDD_LINK_MODE_HALF;
			Result = EDD_STS_OK;
		}
		else if( MAUType == EDDP_PHY_MAUTYPE_10BASETXFD )     // 11/0Bh
		{   // ***** COPPER_CABLE & RADIO_COMMUNICATION
			*pSpeed = EDD_LINK_SPEED_10;
			*pDuplexity  = EDD_LINK_MODE_FULL;
			Result = EDD_STS_OK;
		}
		else if( MAUType == EDDP_PHY_MAUTYPE_100BASETXHD )    // 15/0Fh
		{   // ***** COPPER_CABLE & RADIO_COMMUNICATION
			*pSpeed = EDD_LINK_SPEED_100;
			*pDuplexity  = EDD_LINK_MODE_HALF;
			Result = EDD_STS_OK;
		}
		else if( MAUType == EDDP_PHY_MAUTYPE_100BASETXFD )    // 16/10h
		{   // ***** COPPER_CABLE & RADIO_COMMUNICATION
			*pSpeed = EDD_LINK_SPEED_100;
			*pDuplexity  = EDD_LINK_MODE_FULL;
			Result = EDD_STS_OK;
		}
	}

	return Result;
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_LL_PHY_Get_Media_Type                 +*/
/*+  Input/Output          :    EDDP_HANDLE             hDDB                +*/
/*+                        :    EDDP_SYS_HANDLE         hSysDev             +*/
/*+                        :    LSA_UINT32              HwPortID            +*/
/*+                        :    LSA_UINT16              MAUType             +*/
/*+                        :    LSA_UINT32              *pMediaType         +*/
/*+                        :    LSA_UINT32              *pOpticalTransType  +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+   hDDB       : EDDP Handle                                         (in) +*/
/*+   hSysDev    : Handle of system adaption                           (in) +*/
/*+   HwPortID   : Hardware port ID of the related PHY                 (in) +*/
/*+   MAUType    : MAU Type to be checked                              (in) +*/
/*+   pMediaType : Media type returned by user defined functions       (out)+*/
/*+   pOpticalTransType : Trans type returned by user defined functions(out)+*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function gives media and POF type wich be returned   +*/
/*+               by user defined functions and saves them internaly.       +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_SYSTEM_OUT_FCT_ATTR EDDP_LL_PHY_Get_Media_Type  (EDDP_HANDLE            hDDB,
                                                                EDDP_SYS_HANDLE        hSysDev,
                                                                LSA_UINT32             HwPortID,
                                                                LSA_UINT8              *pMediaType,
                                                                LSA_UINT8              *pOpticalTransType,
                                                                LSA_UINT8		       *pFxTransceiverType)
{
	LSA_UINT32 DevId = ((EDDP_LOCAL_DDB_PTR_TYPE)hDDB)->DevId;
	EDDP_ASSERT_FALSE(DevId >= EDDP_CFG_MAX_DEVICES);


    switch(HwPortID)
    {
    case 1U:
    case 2U:
    case 3U:
    	if(LSA_NULL != pMediaType)
    	{
    		*pMediaType = EDD_MEDIATYPE_COPPER_CABLE;
    	}
    	if(LSA_NULL != pOpticalTransType)
    	{
    		*pOpticalTransType  = LSA_FALSE;
    	}
    	if(LSA_NULL != pFxTransceiverType)
    	{
    		*pFxTransceiverType = EDD_FX_TRANSCEIVER_UNKNOWN;
    	}
    	break;
    default:
    	break;
    }

    if (   (EDD_MEDIATYPE_FIBER_OPTIC_CABLE == *pMediaType)
        && (EDD_PORT_OPTICALTYPE_ISPOF   != *pOpticalTransType)
        && (EDD_PORT_OPTICALTYPE_ISOTHER != *pOpticalTransType))
    {
        EDDP_SWI_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "EDDP_LL_PHY_Get_Media_Type(HwPortID=%d): FATAL -> OpticalTransType (%d) illegal value!",
            HwPortID, *pOpticalTransType);
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }

    g_EDDP_PHY_MEDIATYPE        [DevId][HwPortID - 1] = *pMediaType;
    g_EDDP_PHY_OPTICALTRANSTYPE [DevId][HwPortID - 1] = *pOpticalTransType;
    g_EDDP_PHY_FXTRANSCEIVERTYPE[DevId][HwPortID - 1] = *pFxTransceiverType;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_LL_PHY_Get_MAU_Type                   +*/
/*+  Input/Output          :    EDDP_HANDLE             hDDB                +*/
/*+                        :    EDDP_SYS_HANDLE         hSysDev             +*/
/*+                        :    LSA_UINT32              HwPortID            +*/
/*+                        :    LSA_UINT32              Speed               +*/
/*+                        :    LSA_UINT23              Duplexity           +*/
/*+                        :    LSA_UINT16              *pMauType           +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+   hDDB       : EDDP Handle                                         (in) +*/
/*+   hSysDev    : Handle of system adaption                           (in) +*/
/*+   HwPortID   : Hardware port ID of the related PHY (1..x)          (in) +*/
/*+   Speed      : Actual Speed of the Link                            (in) +*/
/*+                EDD_LINK_SPEED_10                                        +*/
/*+                EDD_LINK_SPEED_100                                       +*/
/*+   Duplexity  : Actual duplexity of link                            (in) +*/
/*+                EDD_LINK_MODE_HALF                                       +*/
/*+                EDD_LINK_MODE_FULL                                       +*/
/*+   pMauType   : Pointer to address for returned MAUType of port.    (out)+*/
/*+                for COPPER_CABLE & RADIO_COMMUNICATION:                  +*/
/*+                10BASETXHD                                               +*/
/*+                10BASETXFD                                               +*/
/*+                100BASETXHD                                              +*/
/*+                100BASETXFD                                              +*/
/*+                for FIBER_OPTIC_CABLE:                                   +*/
/*+                100BASEFXHD                                              +*/
/*+                100BASEFXFD                                              +*/
/*+                                                                         +*/
/*+  Results     : LSA_VOID or EDDP_FATAL()                                 +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: Get MAUType of a port for internal NEC PHY                +*/
/*+                                                                         +*/
/*+       Note:   EDDP expects this function to execute with success.       +*/
/*+               Otherwise a fatal error must be generated.                +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_SYSTEM_OUT_FCT_ATTR EDDP_LL_PHY_Get_MAU_Type(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID,
    LSA_UINT32              Speed,
    LSA_UINT32              Duplexity,
    LSA_UINT16             *pMauType)
{
	LSA_UINT32      DevId;

	if(LSA_NULL != pMauType)
	{
		*pMauType = EDD_MAUTYPE_UNKNOWN;

		/* because we cant use hSysDev we use EDDP internal variables */
		DevId = ((EDDP_LOCAL_DDB_PTR_TYPE)hDDB)->DevId;
		EDDP_ASSERT_FALSE(DevId >= EDDP_CFG_MAX_DEVICES);

		LSA_UNUSED_ARG(hSysDev);

		if (g_EDDP_PHY_MEDIATYPE[DevId][HwPortID-1] == EDD_MEDIATYPE_FIBER_OPTIC_CABLE)
		{
			/* ---------------------------------------- */
			/* FIBER_OPTIC_CABLE                        */
			/* ---------------------------------------- */
			if (Speed == EDD_LINK_SPEED_100)
			{
				if (Duplexity == EDD_LINK_MODE_HALF)
				{
					*pMauType = EDDP_PHY_MAUTYPE_100BASEFXHD;
				}
				else if (LSAS_PORT_IS_POF(HwPortID))
				{
					/* according to RQ 215001, we have to set 0x36 as POF-MAU type */
					*pMauType = EDDP_PHY_MAUTYPE_100BASEPXFD;
				}
				else
				{
					*pMauType = EDDP_PHY_MAUTYPE_100BASEFXFD;
				}
				return;
			}
		}
		else
		{
			/* ---------------------------------------- */
			/* COPPER_CABLE & RADIO_COMMUNICATION       */
			/* ---------------------------------------- */
			switch(Speed)
			{
			case EDD_LINK_SPEED_10:
				if (Duplexity == EDD_LINK_MODE_HALF)
				{
					*pMauType = EDDP_PHY_MAUTYPE_10BASETXHD;
				}
				else
				{
					*pMauType = EDDP_PHY_MAUTYPE_10BASETXFD;
				}
				break;
			case EDD_LINK_SPEED_100:
				if (Duplexity == EDD_LINK_MODE_HALF)
				{
					*pMauType = EDDP_PHY_MAUTYPE_100BASETXHD;
				}
				else
				{
					*pMauType = EDDP_PHY_MAUTYPE_100BASETXFD;
				}
				break;
			case EDD_LINK_SPEED_1000:
			default:
				break;
			}
		}
	}

  //  EDDP_FATAL(EDDP_FATAL_ERR_NOT_SUPPORTED);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_LL_PHY_LED_BlinkBegin                 +*/
/*+  Input/Output          :    EDDP_HANDLE         hDDB                    +*/
/*+                        :    EDDP_SYS_HANDLE     hSysDev                 +*/
/*+                        :    LSA_UINT32          HwPortID                +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+   hDDB       : EDDP Handle                                         (in) +*/
/*+   hSysDev    : Handle of system adaption                           (in) +*/
/*+   HwPortID   : Hardware port ID of the related PHY                 (in) +*/
/*+                                                                         +*/
/*+  Results     : EDD_STS_OK                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: EDDP calls this function within the service               +*/
/*+               EDD_SRV_LED_BLINK() before the LED(s) start to blink. Can +*/
/*+               be used e.g. to diable the link/activity LED function of  +*/
/*+               a PHY if necessary.                                       +*/
/*+                                                                         +*/
/*+       Note:   EDDP expects this function to return with success. Any    +*/
/*+               other return value than �EDD_STS_OK� will be interpreted  +*/
/*+               as a fatal error.                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_LL_PHY_LED_BlinkBegin(
	EDDP_HANDLE 	    hDDB,
	EDDP_SYS_HANDLE     hSysDev,
	LSA_UINT32          HwPortID)
{
    //LSA_UINT32        RegVal;

    LSA_UNUSED_ARG(hDDB);
    LSA_UNUSED_ARG(hSysDev);
    LSA_UNUSED_ARG(HwPortID);

    EDDP_SWI_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT," +++ EDDP_PHY_LED_BlinkBegin +++ ");

    LSAS_EDD_PHY_LED_BLINK_BEGIN(HwPortID);
    /*
    RegVal = ApbPer.Scrb.PHY_LED_CONTROL
    switch (HwPortID)
    {
        // ***** EN_P1_XLINK_STATUS + EN_P1_XACTIVITY --> controled by SW
        case 1:
            RegVal |= 0x03;
            ApbPer.Scrb.PHY_LED_CONTROL = RegVal;
            break;
        // ***** EN_P2_XLINK_STATUS + EN_P2_XACTIVITY --> controled by SW
        case 2:
            RegVal |= 0x0C;
            ApbPer.Scrb.PHY_LED_CONTROL = RegVal;
            break;
        default:
            break;
    }
    */

    return (EDD_STS_OK);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_LL_PHY_LED_BlinkSetMode               +*/
/*+  Input/Output          :    EDDP_HANDLE         hDDB                    +*/
/*+                        :    EDDP_SYS_HANDLE     hSysDev                 +*/
/*+                        :    LSA_UINT32          HwPortID                +*/
/*+                        :    LSA_UINT32		    LEDMode                 +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+   hDDB       : EDDP Handle                                         (in) +*/
/*+   hSysDev    : Handle of system adaption                           (in) +*/
/*+   HwPortID   : Hardware port ID of the related PHY                 (in) +*/
/*+   LEDMode    : Turns the LED(s) ON or OFF                          (in) +*/
/*+                                                                         +*/
/*+  Results     : EDD_STS_OK                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: EDDP calls this function repeatedly within the service    +*/
/*+               EDD_SRV_LED_BLINK() in order to turn on and turn off the  +*/
/*+               LED(s) alternately.                                       +*/
/*+                                                                         +*/
/*+       Note:   EDDP expects this function to return with success. Any    +*/
/*+               other return value than �EDD_STS_OK� will be interpreted  +*/
/*+               as a fatal error.                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_LL_PHY_LED_BlinkSetMode(
	EDDP_HANDLE 	    hDDB,
	EDDP_SYS_HANDLE     hSysDev,
	LSA_UINT32          HwPortID,
	LSA_UINT32		    LEDMode)
{
    LSA_UNUSED_ARG(hDDB);
    LSA_UNUSED_ARG(hSysDev);

    switch(LEDMode)
    {
        case EDDP_LED_MODE_OFF:
            //LedVal = 0;
            if (HwPortID == 1)
            {
                Bsp_EbSetLed(PNIO_LED_BLINK, 0);
            }
            EDDP_SWI_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT," +++ EDDP_PHY_LED_BlinkSetMode | LED OFF | +++ ");
        break;

        case EDDP_LED_MODE_ON:
            //LedVal = 1;
            if (HwPortID == 1)
            {
                Bsp_EbSetLed(PNIO_LED_BLINK, 1);
            }
            EDDP_SWI_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT," +++ EDDP_PHY_LED_BlinkSetMode | LED ON  | +++ ");
        break;

        default:
            EDDP_SWI_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR," +++ ERROR -> Wrong LED Mode! +++ ");
        break;
    }

    LSAS_EDD_PHY_LED_SET_MODE(HwPortID, LEDMode);

    /*
    RegVal = ApbPer.Scrb.PHY_LED_CONTROL

    switch (HwPortID)
    {
        // ***** P1_XLINK_STATUS_SW + EN_P1_XACTIVITY_SW
        case 1:
            RegVal &= 0xFFFFFCFF;
            if (LedVal)
            {
                RegVal |= 0x0300;
            }
            break;
        // ***** EN_P2_XLINK_STATUS_SW + EN_P2_XACTIVITY_SW
        case 2:
            RegVal &= 0xFFFFF3FF;
            if (LedVal)
            {
                RegVal |= 0x0C00;
            }
            break;
        default:
            LedVal = 2;
            break;
    }
    if (LedVal < 2)
    {
        ApbPer.Scrb.PHY_LED_CONTROL = RegVal;
    }
    */

	return (EDD_STS_OK);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_LL_PHY_LED_BlinkEnd                   +*/
/*+  Input/Output          :    EDDP_HANDLE         hDDB                    +*/
/*+                        :    EDDP_SYS_HANDLE     hSysDev                 +*/
/*+                        :    LSA_UINT32          HwPortID                +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+   hDDB       : EDDP Handle                                         (in) +*/
/*+   hSysDev    : Handle of system adaption                           (in) +*/
/*+   HwPortID   : Hardware port ID of the related PHY                 (in) +*/
/*+                                                                         +*/
/*+  Results     : EDD_STS_OK                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: EDDP calls this function within the service               +*/
/*+               EDD_SRV_LED_BLINK() after blinking of LED(s) has finished.+*/
/*+               Can be used e.g. to re-enable the link/activity LED       +*/
/*+               function of a PHY if necessary.                           +*/
/*+                                                                         +*/
/*+       Note:   EDDP expects this function to return with success. Any    +*/
/*+               other return value than �EDD_STS_OK� will be interpreted  +*/
/*+               as a fatal error.                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_LL_PHY_LED_BlinkEnd(
	EDDP_HANDLE 	    hDDB,
	EDDP_SYS_HANDLE     hSysDev,
	LSA_UINT32          HwPortID)
{
    //LSA_UINT32     RegVal;
    LSA_UNUSED_ARG(hDDB);
    LSA_UNUSED_ARG(hSysDev);
    LSA_UNUSED_ARG(HwPortID);

    EDDP_SWI_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_CHAT," +++ EDDP_PHY_LED_BlinkEnd +++ ");

    LSAS_EDD_PHY_LED_BLINK_END(HwPortID);

    /*
    RegVal = ApbPer.Scrb.PHY_LED_CONTROL
    switch (HwPortID)
    {
        // ***** EN_P1_XLINK_STATUS + EN_P1_XACTIVITY --> controled by PHY
        case 1:
            RegVal &= 0xFFFFFFFC;
            ApbPer.Scrb.PHY_LED_CONTROL = RegVal;
            break;
        // ***** EN_P2_XLINK_STATUS + EN_P2_XACTIVITY --> controled by PHY
        case 2:
            RegVal &= 0xFFFFFFF3;
            ApbPer.Scrb.PHY_LED_CONTROL = RegVal;
            break;
        default:
            break;
    }
    */

	return (EDD_STS_OK);
}



#endif //

/*****************************************************************************/
/*  end of file eddp_phy_nec.c                                               */
/*****************************************************************************/
