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
/*  F i l e               &F: eddp_phy_nsc.c                            :F&  */
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
/*  27.09.11    JS    moved EDDP_PHY_NSC_REG_PCS_CFG to eddp_phy_nsc.h       */
/*                    Configure SD_Option to 0 (AP01232277)                  */
#endif
/*****************************************************************************/

#define LTRC_ACT_MODUL_ID   113
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_PHY_NSC */

#include "eddp_inc.h"
#include "eddp_int.h"
#include "eddp_phy.h"

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)

#ifdef EDDP_CFG_PHY_NSC_SUPPORT

#include "eddp_phy_nsc.h"

// #define EDDP_DISABLE_10MBIT_SUPPORT





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_PHY_NSC_Init                           +*/
/*+  Input/Output          :    EDDP_HANDLE				hDDB                +*/
/*+                        :    EDDP_SYS_HANDLE			hSysDev             +*/
/*+                        :    LSA_UINT32              Hardware            +*/
/*+                        :    LSA_UINT32				HwPortID            +*/
/*+                        :    LSA_UINT8				IsWireless          +*/
/*+                        :    LSA_BOOL				IsMIDX              +*/
/*+                        :    EDDP_PHY_INIT_PTR_TYPE	pInitData           +*/
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
/*+               other return value than “EDD_STS_OK” will be interpreted  +*/
/*+               as a fatal error.                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NSC_Init(
	EDDP_HANDLE				hDDB,
	EDDP_SYS_HANDLE			hSysDev,
	LSA_UINT32              Hardware,
    LSA_UINT32				HwPortID,
    LSA_UINT8 	            IsWireless,
    LSA_UINT8  	            IsMIDX,
	EDDP_PHY_INIT_PTR_TYPE  pInitData)
{
    LSA_UINT32      RegVal;
	LSA_UINT32      DevId;

	DevId = ((EDDP_LOCAL_DDB_PTR_TYPE)hDDB)->DevId;
	EDDP_ASSERT_FALSE(DevId >= EDDP_CFG_MAX_DEVICES);

    pInitData->LinkStatusRegAddress	= EDDP_PHY_NSC_LINKSTATUSREGADDRESS;
	pInitData->LinkStatusBitMask	= EDDP_PHY_NSC_LINKSTATUSBITMASK|EDDP_PHY_NSC_LINKVALIDBITMASK;
    pInitData->SpeedRegAddress	    = EDDP_PHY_NSC_SPEEDREGADDRESS; 
	pInitData->SpeedBitMask			= EDDP_PHY_NSC_SPEEDBITMASK; 
	pInitData->DuplexityRegAddress	= EDDP_PHY_NSC_DUPLEXITYREGADDRESS; 
    pInitData->DuplexityBitMask		= EDDP_PHY_NSC_DUPLEXITYBITMASK; 

    g_EDDP_PHY_ISWIRELESS[DevId][HwPortID-1] = IsWireless;

    EDDP_PHY_NSC_GET_MEDIA_TYPE(hDDB, hSysDev, HwPortID, &g_EDDP_PHY_MEDIATYPE[DevId][HwPortID-1], &g_EDDP_PHY_OPTICALTRANSTYPE[DevId][HwPortID-1], &g_EDDP_PHY_FXTRANSCEIVERTYPE[DevId][HwPortID - 1]);

    // ***** FX-mode setting 
	// Note: With Fx-mode no MDIX shall be set and setting is ignored! 
	if (g_EDDP_PHY_MEDIATYPE[DevId][HwPortID-1] == EDD_MEDIATYPE_FIBER_OPTIC_CABLE)
	{
        if (   (EDD_PORT_OPTICALTYPE_ISPOF   != g_EDDP_PHY_OPTICALTRANSTYPE[DevId][HwPortID-1])
            && (EDD_PORT_OPTICALTYPE_ISOTHER != g_EDDP_PHY_OPTICALTRANSTYPE[DevId][HwPortID-1]))
        {
            EDDP_SWI_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "EDDP_PHY_NSC_Init(HwPortID=%d): FATAL -> OpticalTransType (%d) illegal value!",
                HwPortID, g_EDDP_PHY_OPTICALTRANSTYPE[DevId][HwPortID-1]);
            EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
        }

        if (IsMIDX)
		{
			EDDP_SWI_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_HIGH,"[H:--] EDDP_PHY_NSC_Init(HwPortID=%d): isMDIX = LSA_TRUE not allowed with Fx-Mode (POF)! Will be ignored!", 
                HwPortID); 
        }
        g_EDDP_PHY_MDI_X_MAPPING[DevId][HwPortID-1] = LSA_FALSE;
        // a global structure to transfer the linkSpeedModeConfigured between the phy service and the prm service.
        // for copper Phys it is initiliazed with autoneg
        g_EDDP_PHY_LINK_SPEED_MODE_CONFIGURED[DevId][HwPortID-1]= EDD_LINK_100MBIT_FULL ;
    }
    else
    {
        g_EDDP_PHY_MDI_X_MAPPING[DevId][HwPortID-1] = IsMIDX;
        // a global structure to transfer the linkSpeedModeConfigured between the phy service and the prm service.
        // for copper Phys it is initiliazed with autoneg
        g_EDDP_PHY_LINK_SPEED_MODE_CONFIGURED[DevId][HwPortID-1]= EDD_LINK_AUTONEG ;
    }

    #ifdef EDDP_DISABLE_10MBIT_SUPPORT
	// ***** Configure PHY for 100 MBit only
    eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_PHY_NSC_REG_NEG_ADVERTISEMENT, &RegVal);
    /*
    Negotiation Advertisement Register (ANAR), address 04h
    15  RW  NP          Next Page Indication
    14  RW  RESERVED
    13  RW  RF          Remote Fault
    12  RW  RESERVED    
    11  RW  ASM_DIR     Asymmetric PAUSE Support for Full Duplex Links
    10  RW  PAUSE       PAUSE Support for Full Duplex Links:
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

    eddp_ll_PhyRegisterWrite (hDDB, HwPortID, EDDP_PHY_NSC_REG_NEG_ADVERTISEMENT, RegVal);
    #endif

	/* configure SD_Option to 0 so a single line break force a link-down */
	/* AP01232277                                                        */

    eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_PHY_NSC_REG_PCS_CFG, &RegVal);
    RegVal &= ~0x100; // SD_Option, Bit 8 set to 0 (default is 1) 
    eddp_ll_PhyRegisterWrite (hDDB, HwPortID, EDDP_PHY_NSC_REG_PCS_CFG, RegVal);

    LSA_UNUSED_ARG(hSysDev);
    LSA_UNUSED_ARG(Hardware);

	return EDD_STS_OK;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_PHY_NSC_LoadDelayValues                +*/
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
/*+    OpticalTransType     : Is POF/SFP/OTHER                         (in) +*/
/*+    RealPortTxDelay      : Average tx port delay of used hardware   (out)+*/
/*+    RealPortRxDelay      : Average rx port delay of used hardware   (out)+*/
/*+    MaxPortTxDelay       : Maximum tx port delay of used hardware   (out)+*/
/*+    MaxPortRxDelay       : Maximum rx port delay of used hardware   (out)+*/
/*+                                                                         +*/
/*+  Results                : EDD_STS_OK                                    +*/
/*+                           EDD_STS_ERR_PARAM                             +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NSC_LoadDelayValues(
	EDDP_HANDLE				        hDDB,
	LSA_UINT32						Hardware,
    EDDP_PHY_LINK_STATUS_PTR_TYPE   pLinkStatus)
{
    LSA_UNUSED_ARG(hDDB);

	switch (Hardware)
    {
        case EDDP_HW_ERTEC200P:
        {
            if (pLinkStatus->MediaType == EDD_MEDIATYPE_FIBER_OPTIC_CABLE)
            {
                EDDP_SWI_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PHY_NSC_GetLinkStatus(): ERROR -> NSC-PHY not supported with ERTEC200P in FX-Mode!");
			    return EDD_STS_ERR_PARAM;
            }
            else
            {
                // Tx-Mode
			    pLinkStatus->RealPortTxDelay    = EDDP_PHY_NSC_REAL_PORT_TX_DELAY_ERTEC200P;
			    pLinkStatus->RealPortRxDelay    = EDDP_PHY_NSC_REAL_PORT_RX_DELAY_ERTEC200P;
                pLinkStatus->MaxPortTxDelay     = EDDP_PHY_NSC_MAX_PORT_TX_DELAY_ERTEC200P;
                pLinkStatus->MaxPortRxDelay     = EDDP_PHY_NSC_MAX_PORT_RX_DELAY_ERTEC200P;
            }
        }
        break;

        case EDDP_HW_ERTEC200P_FPGA:
        case EDDP_HW_HERA_FPGA:    
        {
            if (pLinkStatus->MediaType == EDD_MEDIATYPE_FIBER_OPTIC_CABLE)
            {
                // Fx-Mode
			    pLinkStatus->RealPortTxDelay    = EDDP_PHY_NSC_REAL_PORT_TX_DELAY_FPGA_FX;
			    pLinkStatus->RealPortRxDelay    = EDDP_PHY_NSC_REAL_PORT_RX_DELAY_FPGA_FX;
                pLinkStatus->MaxPortTxDelay     = EDDP_PHY_NSC_MAX_PORT_TX_DELAY_FPGA_FX;
                pLinkStatus->MaxPortRxDelay     = EDDP_PHY_NSC_MAX_PORT_RX_DELAY_FPGA_FX;
            }
            else
            {
                // Tx-Mode
			    pLinkStatus->RealPortTxDelay    = EDDP_PHY_NSC_REAL_PORT_TX_DELAY_FPGA;
			    pLinkStatus->RealPortRxDelay    = EDDP_PHY_NSC_REAL_PORT_RX_DELAY_FPGA;
                pLinkStatus->MaxPortTxDelay     = EDDP_PHY_NSC_MAX_PORT_TX_DELAY_FPGA;
                pLinkStatus->MaxPortRxDelay     = EDDP_PHY_NSC_MAX_PORT_RX_DELAY_FPGA;
            }
        }
        break;

        default:
        {
            EDDP_SWI_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PHY_NSC_GetLinkStatus(): ERROR -> HardwareType not supported!");
			return EDD_STS_ERR_PARAM;
        }
        break;
    }

    return EDD_STS_OK;
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_PHY_NSC_GetLinkStatus                  +*/
/*+  Input/Output          :    EDDP_HANDLE			            hDDB        +*/
/*+                        :    EDDP_SYS_HANDLE		            hSysDev     +*/
/*+                        :    LSA_UINT32						Hardware    +*/
/*+                        :    LSA_UINT32			            HwPortID    +*/
/*+                        :    EDDP_PHY_LINK_STATUS_PTR_TYPE   pLinkStatus +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+   hDDB                  : EDDP Handle                              (in) +*/
/*+   hSysDev               : Handle of system adaption                (in) +*/
/*+   Hardware              : internal HW identifier                   (in) +*/
/*+   HwPortID              : Hardware port ID of the related PHY      (in) +*/
/*+                                                                         +*/
/*+   Elements of pLinkStatus:                                              +*/
/*+    Link                 : IN: PHY register containing link information  +*/
/*+                           OUT: Current link status                      +*/
/*+    Speed                : IN: PHY register containing speed information +*/
/*+                           OUT: Current speed status                     +*/
/*+    Duplexity            : IN: PHY reg containing duplexity information  +*/
/*+                           OUT: Current duplexity status                 +*/
/*+    MAUType              : Current MAU Type                         (out)+*/
/*+    MediaType            : Current Media Type                       (out)+*/
/*+    AutonegCapAdvertised : Current advertised capabilities @auto-neg(out)+*/
/*+    PHY_TxDelay          : Transmit PHY delay in nano seconds (ns)  (out)+*/
/*+    PHY_RxDelay          : Receive PHY delay in nano seconds (ns)   (out)+*/
/*+    IsWireless           : Is this port a wireless port?            (out)+*/
/*+    IsMDIX               : Is this a "Crossed Port"?                (out)+*/
/*+                                                                         +*/
/*+                                                                         +*/
/*+  Results                : EDD_STS_OK                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function determines the current link status, speed   +*/
/*+               and duplexity from the register values passed when the    +*/
/*+               function is called. It then stores this information in    +*/
/*+               Link, Speed and Duplexity. Additionally, the current      +*/
/*+               MAUType, MediaType, AutonegCapAdvertised, TxDelay, RxDelay+*/
/*+               IsWireless and IsMDIX are returned.                       +*/
/*+               If the transceiver does not support auto-negotiation it   +*/
/*+               has to return “0” for “AutonegCapAdvertised”.             +*/
/*+                                                                         +*/
/*+       Note:   EDDP expects this function to return with success. Any    +*/
/*+               other return value than “EDD_STS_OK” will be interpreted  +*/
/*+               as a fatal error.                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NSC_GetLinkStatus(
	EDDP_HANDLE				        hDDB,
	EDDP_SYS_HANDLE			        hSysDev,
	LSA_UINT32						Hardware,
    LSA_UINT32				        HwPortID,
    EDDP_PHY_LINK_STATUS_PTR_TYPE   pLinkStatus)
{
    LSA_UINT32		RegVal;
    LSA_BOOL		bBitsValid;
	LSA_UINT32      DevId;
    EDD_RSP         Response;

	DevId = ((EDDP_LOCAL_DDB_PTR_TYPE)hDDB)->DevId;
	EDDP_ASSERT_FALSE(DevId >= EDDP_CFG_MAX_DEVICES);

	// ***** information if transceiver is TX or FX
	pLinkStatus->MediaType          = g_EDDP_PHY_MEDIATYPE        [DevId][HwPortID-1];
	pLinkStatus->OpticalTransType   = g_EDDP_PHY_OPTICALTRANSTYPE [DevId][HwPortID-1];
    pLinkStatus->FxTransceiverType  = g_EDDP_PHY_FXTRANSCEIVERTYPE[DevId][HwPortID-1];

	// ***** Examine passed parameters in order to determine link, speed and duplexity.
    pLinkStatus->AutonegCapAdvertised = 0;

    // ***** if AutoNeg is not active the Duplexity and Speed Bits are valid !!!
    bBitsValid = LSA_TRUE;

    // ***** read CFG if AutoNeg is active and AutoNeg is completed
    eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_PHY_NSC_REG_BCMR, &RegVal);

    if (RegVal & (0x1000/*enable Auto-Neg*/))
    {
        if (!(pLinkStatus->Link & (0x0020/*Auto-Neg complete*/)))
        {
            bBitsValid = LSA_FALSE;
        }
    }

    EDDP_SWI_TRACE_06(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE,"[H:--] EDDP_PHY_NSC_GetLinkStatus(HwPortID=%d) RegLink=%Xh, RegSpeed=%Xh, RegDuplexity=%X, REG_0=%Xh, S/D-Valid=%d", 
        HwPortID, pLinkStatus->Link, pLinkStatus->Speed, pLinkStatus->Duplexity, RegVal, bBitsValid);

    // ***** Examine passed parameters in order to determine link, speed and duplexity.
    if (bBitsValid)
    {
        pLinkStatus->Link   = (pLinkStatus->Link & EDDP_PHY_NSC_LINKSTATUSBITMASK) ? EDD_LINK_UP : EDD_LINK_DOWN;
    }
    else
    {
        pLinkStatus->Link   = EDD_LINK_DOWN;
    }

    if (pLinkStatus->Link == EDD_LINK_UP)
    {
        //Attention: TFS2141324: Due to polling times, LinkStatus can still be UP (previous pollcycle) while real LinkStatus is DOWN already and speed/duplexity undefined.
        //           Check mirrored bits in speed and duplexity register
        pLinkStatus->Link = (pLinkStatus->Speed & EDDP_PHY_NSC_SPEEDREGLINKSTATUSBITMASK) ? ((pLinkStatus->Duplexity & EDDP_PHY_NSC_DUPLEXITYREGLINKSTATUSBITMASK) ? EDD_LINK_UP : EDD_LINK_DOWN) : EDD_LINK_DOWN;
    }

    if (pLinkStatus->Link == EDD_LINK_UP)
    {
        pLinkStatus->Speed      = ( pLinkStatus->Speed & EDDP_PHY_NSC_SPEEDBITMASK )         ? EDD_LINK_SPEED_10  : EDD_LINK_SPEED_100;
        pLinkStatus->Duplexity  = ( pLinkStatus->Duplexity & EDDP_PHY_NSC_DUPLEXITYBITMASK ) ? EDD_LINK_MODE_FULL : EDD_LINK_MODE_HALF;
        // ***** Determine current MAUType
        EDDP_PHY_NSC_GET_MAU_TYPE(hDDB,hSysDev,HwPortID,pLinkStatus->Speed,pLinkStatus->Duplexity,&pLinkStatus->MAUType);
    }
    else
    {
        pLinkStatus->Speed      = EDD_LINK_UNKNOWN;
        pLinkStatus->Duplexity  = EDD_LINK_UNKNOWN;
        pLinkStatus->MAUType    = EDD_MAUTYPE_UNKNOWN;
    }

    if (pLinkStatus->MediaType == EDD_MEDIATYPE_FIBER_OPTIC_CABLE)
    {  
        // ***** FIBER_OPTIC. fixed. 
        pLinkStatus->AutonegCapAdvertised = EDD_AUTONEG_CAP_NONE;
    }
    else 
    {
        // ***** COPPER + RADIO
        eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_PHY_NSC_REG_NEG_ADVERTISEMENT, &RegVal);

        pLinkStatus->AutonegCapAdvertised = 0;

        if (RegVal & 0x0020)    { pLinkStatus->AutonegCapAdvertised |= EDD_AUTONEG_CAP_10BASET    ; }
        if (RegVal & 0x0040)    { pLinkStatus->AutonegCapAdvertised |= EDD_AUTONEG_CAP_10BASETFD  ; }
        if (RegVal & 0x0080)    { pLinkStatus->AutonegCapAdvertised |= EDD_AUTONEG_CAP_100BASETX  ; }
        if (RegVal & 0x0100)    { pLinkStatus->AutonegCapAdvertised |= EDD_AUTONEG_CAP_100BASETXFD; }
        if (RegVal & 0x0200)    { pLinkStatus->AutonegCapAdvertised |= EDD_AUTONEG_CAP_100BASET4  ; }
        if (RegVal & 0x0400)    { pLinkStatus->AutonegCapAdvertised |= EDD_AUTONEG_CAP_FDXPAUSE   ; }
        if (RegVal & 0x0800)    { pLinkStatus->AutonegCapAdvertised |= EDD_AUTONEG_CAP_FDXAPAUSE  ; }
    }

    /* set Port delay values */
    Response = EDDP_PHY_NSC_LoadDelayValues(hDDB, Hardware, pLinkStatus);

	if (EDD_STS_OK != Response)
    {
        //Traceentries are made in EDDP_PHY_NSC_LoadDelayValues
        return Response;
    }

    /* set IsWireles */
    pLinkStatus->IsWireless = g_EDDP_PHY_ISWIRELESS[DevId][HwPortID-1];
    /* set IsMDIX */
    pLinkStatus->IsMDIX     = g_EDDP_PHY_MDI_X_MAPPING[DevId][HwPortID-1];  

    pLinkStatus->LinkSpeedModeConfigured     = g_EDDP_PHY_LINK_SPEED_MODE_CONFIGURED[DevId][HwPortID-1]; 

    return EDD_STS_OK;
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_PHY_NSC_SetPowerDown                   +*/
/*+  Input/Output          :    EDDP_HANDLE         hDDB                    +*/
/*+                        :    EDDP_SYS_HANDLE     hSysDev                 +*/
/*+                        :    LSA_UINT32          HwPortID                +*/
/*+                        :    LSA_BOOL            PowerDown               +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+   hDDB       : EDDP Handle                                         (in) +*/
/*+   hSysDev    : Handle of system adaption                           (in) +*/
/*+   HwPortID   : Hardware port ID of the related PHY                 (in) +*/
/*+   PowerDown  : LSA_TRUE turns the PHY off / LSA_FALSE := on        (in) +*/
/*+                                                                         +*/
/*+  Results     : EDD_STS_OK                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function turns the PHY into software power down mode +*/
/*+               ON or OFF. When software power mode is OFF there will be  +*/
/*+               no link and the PHY does not receive or send any data but +*/
/*+               can still be controlled via SMI.                          +*/
/*+                                                                         +*/
/*+       Note:   EDDP expects this function to return with success. Any    +*/
/*+               other return value than “EDD_STS_OK” will be interpreted  +*/
/*+               as a fatal error.                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NSC_SetPowerDown(
    EDDP_HANDLE         hDDB,
    EDDP_SYS_HANDLE     hSysDev,
    LSA_UINT32          HwPortID, 
    LSA_UINT8           PowerDown)
{
    LSA_UINT32		RegVal;

    LSA_UNUSED_ARG(hSysDev);

    eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_PHY_NSC_REG_BCMR, &RegVal);

    if (EDDP_PHY_POWERDOWN == PowerDown)
    {
        eddp_ll_PhyRegisterWrite (hDDB,HwPortID, EDDP_PHY_NSC_REG_BCMR, (RegVal | 0x0800/*power down*/)); 
    }
    else
    {
        eddp_ll_PhyRegisterWrite (hDDB,HwPortID, EDDP_PHY_NSC_REG_BCMR, (RegVal & ~0x0800/*normal mode*/)); 
    }
    
    return EDD_STS_OK;
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_PHY_NSC_CheckPowerDown                 +*/
/*+  Input/Output          :    EDDP_HANDLE             hDDB                +*/
/*+                        :    EDDP_SYS_HANDLE         hSysDev             +*/
/*+                        :    LSA_UINT32              HwPortID            +*/
/*+                        :    LSA_BOOL                pIsPowerDown        +*/
/*+                                                                         +*/
/*+  Result                :    EDD_RSP                                     +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+   hDDB       : EDDP Handle                                         (in) +*/
/*+   hSysDev    : Handle of system adaption                           (in) +*/
/*+   HwPortID   : Hardware port ID of the related PHY                 (in) +*/
/*+   pIsPowerDown: PowerDown state of the related PHY (LSA_FALSE if   (out)+*/
/*+                 not PowerUp)                                            +*/
/*+                                                                         +*/
/*+  Results     : EDD_STS_OK if PHY supports this check                    +*/
/*+                EDD_STS_ERR_NOT_SUPPORTED if PHY does not support        +*/
/*+                this check                                               +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function checks if the PHY is in powerdown state.    +*/
/*+               Needed for service EDD_SRV_PRM_CHANGE_PORT.               +*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NSC_CheckPowerDown(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID,
    LSA_UINT8*              pIsPowerDown)
{
    LSA_UINT32      RegVal;

    LSA_UNUSED_ARG(hSysDev);

    eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_PHY_NSC_REG_BCMR, &RegVal);

    if(RegVal & 0x0800/*power down*/)
    {
        *pIsPowerDown = EDDP_PHY_POWERDOWN;
    }
    else
    {
        *pIsPowerDown = EDDP_PHY_POWERUP;
    }

    return EDD_STS_ERR_NOT_SUPPORTED;
}



/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname               :    EDDP_PHY_NSC_SetSpeedDuplexityOrAutoneg     +*/
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
/*+               “Autoneg” parameter is set to LSA_FALSE. If “Autoneg” is       +*/
/*+               set to LSA_TRUE “Speed” and “Duplexity” parameters are         +*/
/*+               irrelevant. In case the transceiver does not support           +*/
/*+               auto-negotiation it has to set up a connection at least at     +*/
/*+               100MBit full duplex and has to return “0” for                  +*/
/*+               “AutonegCapAdvertised” (see EDDP_PHY_GetLinkStatus).           +*/
/*+                                                                              +*/
/*+       Note:   EDDP expects this function to return with success. Any         +*/
/*+               other return value than “EDD_STS_OK” will be interpreted       +*/
/*+               as a fatal error.                                              +*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NSC_SetSpeedDuplexityOrAutoneg(
    EDDP_HANDLE         hDDB,
    EDDP_SYS_HANDLE     hSysDev,
    LSA_UINT32          HwPortID,
    LSA_UINT8           *pLinkSpeedModeConfigured,
    LSA_UINT32          Speed,
    LSA_UINT32          Duplexity,
    LSA_UINT8           Autoneg)
{
    LSA_UINT32      RegVal;
    LSA_UINT32      DevId;
    EDD_RSP         ReturnCode=LSA_RSP_ERR_PARAM;

	DevId = ((EDDP_LOCAL_DDB_PTR_TYPE)hDDB)->DevId;
	EDDP_ASSERT_FALSE(DevId >= EDDP_CFG_MAX_DEVICES);

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

    eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_PHY_NSC_REG_BCMR, &RegVal);

    if (EDD_AUTONEG_ON == Autoneg)
    {
        eddp_ll_PhyRegisterWrite (hDDB, HwPortID, EDDP_PHY_NSC_REG_BCMR, (RegVal | 0x1000/*enable Auto-Neg*/));

        eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_PHY_NSC_REG_CONTROL, &RegVal);
        // disable force MDIX (now result depends on auto-neg settings)
	    eddp_ll_PhyRegisterWrite (hDDB, HwPortID, EDDP_PHY_NSC_REG_CONTROL, (RegVal & ~0x4000/*disable force MDIX*/));
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
        
        eddp_ll_PhyRegisterWrite (hDDB, HwPortID, EDDP_PHY_NSC_REG_BCMR, (RegVal & ~0x1000/*disable Auto-Neg*/));

        eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_PHY_NSC_REG_CONTROL, &RegVal);

        if (g_EDDP_PHY_MDI_X_MAPPING[DevId][HwPortID -1] )
        {
            // ***** force this port to operate in MDI-X mode
	        eddp_ll_PhyRegisterWrite (hDDB, HwPortID, EDDP_PHY_NSC_REG_CONTROL, (RegVal | 0x4000/*force MDI-X*/));
        }
        else
        {
            // force this port to operate in MDI mode
	        eddp_ll_PhyRegisterWrite (hDDB, HwPortID, EDDP_PHY_NSC_REG_CONTROL, RegVal & ~0x4000 /*normal operation*/);
        }
    }
    ReturnCode=EDDP_PHY_MapAutonegSpeedDuplexityToLinkSpeedModeConfigured(Autoneg,Speed,Duplexity,pLinkSpeedModeConfigured);
    EDDP_ASSERT(LSA_RSP_OK==ReturnCode);
    g_EDDP_PHY_LINK_SPEED_MODE_CONFIGURED[DevId][HwPortID-1]=*pLinkSpeedModeConfigured;
    LSA_UNUSED_ARG(hSysDev);

    return EDD_STS_OK;
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_PHY_NSC_CheckMAUType                   +*/
/*+  Input/Output          :    EDDP_HANDLE         hDDB                    +*/
/*+                        :    EDDP_SYS_HANDLE     hSysDev                 +*/
/*+                        :    LSA_UINT32          HwPortID                +*/
/*+                        :    LSA_UINT16          MAUType                 +*/
/*+                        :    LSA_UINT32          *pSpeed                 +*/
/*+                        :    LSA_UINT32          *pDuplexity             +*/
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
/*+               other return value than “EDD_STS_OK” will be interpreted  +*/
/*+               as a fatal error.                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NSC_CheckMAUType(
    EDDP_HANDLE         hDDB,
    EDDP_SYS_HANDLE     hSysDev,
    LSA_UINT32          HwPortID,
    LSA_UINT16          MAUType,
    LSA_UINT32          *pSpeed,
    LSA_UINT32          *pDuplexity)
{
    EDD_RSP     Result;

    Result = EDDP_PHY_NSC_CHECK_MAU_TYPE(hDDB, hSysDev, HwPortID, MAUType, pSpeed, pDuplexity);

    return Result;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_PHY_NSC_Get_Media_Type                 +*/
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
LSA_VOID EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NSC_Get_Media_Type  (EDDP_HANDLE            hDDB,
                                                                EDDP_SYS_HANDLE        hSysDev,
                                                                LSA_UINT32             HwPortID,
                                                                LSA_UINT8              *pMediaType,
                                                                LSA_UINT8              *pOpticalTransType,
                                                                LSA_UINT8              *pFxTransceiverType)
{
	LSA_UINT32 DevId = ((EDDP_LOCAL_DDB_PTR_TYPE)hDDB)->DevId;
	EDDP_ASSERT_FALSE(DevId >= EDDP_CFG_MAX_DEVICES);

    EDDP_PHY_NSC_GET_MEDIA_TYPE(hDDB, hSysDev, HwPortID, pMediaType, pOpticalTransType, pFxTransceiverType);

    if (   (EDD_MEDIATYPE_FIBER_OPTIC_CABLE == *pMediaType)
        && (EDD_PORT_OPTICALTYPE_ISPOF   != *pOpticalTransType)
        && (EDD_PORT_OPTICALTYPE_ISOTHER != *pOpticalTransType))
    {
        EDDP_SWI_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "EDDP_PHY_NSC_Get_Media_Type(HwPortID=%d): FATAL -> OpticalTransType (%d) illegal value!",
            HwPortID, *pOpticalTransType);
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }

    g_EDDP_PHY_MEDIATYPE        [DevId][HwPortID  -1] = *pMediaType;
    g_EDDP_PHY_OPTICALTRANSTYPE [DevId][HwPortID - 1] = *pOpticalTransType;
    g_EDDP_PHY_FXTRANSCEIVERTYPE[DevId][HwPortID - 1] = *pFxTransceiverType;
}

#endif // EDDP_CFG_PHY_NSC_SUPPORT

/*****************************************************************************/
/*  end of file eddp_phy_nsc.c                                               */
/*****************************************************************************/
