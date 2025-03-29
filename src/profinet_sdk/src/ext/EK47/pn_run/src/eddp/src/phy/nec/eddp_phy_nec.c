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
#define EDDP_MODULE_ID      LTRC_ACT_MODUL_ID /* EDDP_MODULE_ID_EDDP_PHY_NEC */

#include "eddp_inc.h"
#include "eddp_int.h"
#include "eddp_phy.h"

EDDP_FILE_SYSTEM_EXTENSION(EDDP_MODULE_ID)

#if 1 //def EDDP_CFG_PHY_NEC_SUPPORT

#include "eddp_phy_nec.h"

#define EDDP_DISABLE_10MBIT_SUPPORT

// With FPGA Version reset Port if it is configured for Fx-Mode via SCRB but shall be
// used as T-Mode(CU). Because with FPGA the Fx/Tx-Mode can only be configured for both 
// ports, both ports will be configured for Fx if one port is Fx. We have to 
// change to CU and force a SW-Reset so the CU port work properly
#define EDDP_PHY_NEC_FPGA_FX2TX_PORT_RESET





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_PHY_NEC_Init                           +*/
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

EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NEC_Init(
	EDDP_HANDLE				    hDDB,
	EDDP_SYS_HANDLE			    hSysDev,
	LSA_UINT32                  Hardware,
    LSA_UINT32				    HwPortID,
    LSA_UINT8  	                IsWireless,
    LSA_UINT8  	                IsMIDX,
	EDDP_PHY_INIT_PTR_TYPE      pInitData)
{
    LSA_UINT32      RegVal;
	LSA_UINT32      DevId;
	LSA_BOOL        Fx2TxChange;

    Fx2TxChange = LSA_FALSE;
	DevId       = ((EDDP_LOCAL_DDB_PTR_TYPE)hDDB)->DevId;
	EDDP_ASSERT_FALSE(DevId >= EDDP_CFG_MAX_DEVICES);
 
    pInitData->LinkStatusRegAddress = EDDP_PHY_NEC_LINKSTATUSREGADDRESS;
	pInitData->LinkStatusBitMask	= EDDP_PHY_NEC_LINKSTATUSBITMASK;
    pInitData->SpeedRegAddress	    = EDDP_PHY_NEC_SPEEDREGADDRESS; 
	pInitData->SpeedBitMask			= EDDP_PHY_NEC_SPEEDBITMASK; 
	pInitData->DuplexityRegAddress	= EDDP_PHY_NEC_DUPLEXITYREGADDRESS; 
    pInitData->DuplexityBitMask		= EDDP_PHY_NEC_DUPLEXITYBITMASK; 

    g_EDDP_PHY_ISWIRELESS[DevId][HwPortID-1] = IsWireless;

    EDDP_PHY_NEC_GET_MEDIA_TYPE(hDDB, hSysDev, HwPortID, &g_EDDP_PHY_MEDIATYPE[DevId][HwPortID-1], &g_EDDP_PHY_OPTICALTRANSTYPE[DevId][HwPortID-1], &g_EDDP_PHY_FXTRANSCEIVERTYPE[DevId][HwPortID - 1]);

    // Fx/Tx-mode setting (this should match the SCRB configuration)
    //      Reg18.10 = 0    : Tx-Mode
    //      Reg18.10 = 1    : Fx-Mode
	// Exception: FPGA Board. With FPGA Board the mode can only be set for both ports
	// Note: With Fx-mode no MDIX shall be set and setting is ignored!
    // read current value from register
    eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_PHY_NEC_REG_SPECIAL_MODES, &RegVal);

    if (g_EDDP_PHY_MEDIATYPE[DevId][HwPortID-1] == EDD_MEDIATYPE_FIBER_OPTIC_CABLE)
	{
        /* ------------------------------------ */
        /* MediaType = Fx-Mode (POF)            */
        /* ------------------------------------ */
        // check whether Reg18.10 (Fx-Mode) is set
        if (!(RegVal & 0x0400))
		{
            // no Fx-Mode on startup from SCRB ?
			EDDP_SWI_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PHY_NEC_Init(HwPortID=%d): FATAL -> MediaType is Fx-Mode (POF) but no Fx-Mode is set in SCRB(0x%X)!", 
                HwPortID, RegVal);
            EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
		}
        if (   (EDD_PORT_OPTICALTYPE_ISPOF   != g_EDDP_PHY_OPTICALTRANSTYPE[DevId][HwPortID-1])
            && (EDD_PORT_OPTICALTYPE_ISOTHER != g_EDDP_PHY_OPTICALTRANSTYPE[DevId][HwPortID-1]))
        {
            EDDP_SWI_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "EDDP_PHY_NEC_Init(HwPortID=%d): FATAL -> OpticalTransType (%d) illegal value!",
                HwPortID, g_EDDP_PHY_OPTICALTRANSTYPE[DevId][HwPortID-1]);
            EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
        }

        if (IsMIDX)
		{
			EDDP_SWI_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE_HIGH, "[H:--] EDDP_PHY_NEC_Init(HwPortID=%d): isMDIX = LSA_TRUE not allowed with Fx-Mode (POF)! Will be ignored!", 
                HwPortID);
		}

        g_EDDP_PHY_MDI_X_MAPPING[DevId][HwPortID-1] = LSA_FALSE;
        // a global structure to transfer the linkSpeedModeConfigured between the phy service and the prm service.
        // for POF Phys it is initiliazed with 100 Mbit full dupplex
        g_EDDP_PHY_LINK_SPEED_MODE_CONFIGURED[DevId][HwPortID-1]= EDD_LINK_100MBIT_FULL ;

    }
    else
    {
        /* ------------------------------------ */
        /* MediaType = Tx-Mode (Copper)         */
        /* ------------------------------------ */
        // check whether Reg18.10 (Tx-Mode) is set
		if (RegVal & 0x0400)
		{
            // no Tx-Mode on startup from SCRB. Set Tx-Mode by EDDP only for FPGA.
            Fx2TxChange = LSA_TRUE;

            #ifdef EDDP_PHY_NEC_FPGA_FX2TX_PORT_RESET
            // with FPGA changing from Fx to Tx is allowed but needs a reset
            if ( (Hardware != EDDP_HW_ERTEC200P_FPGA) || (Hardware != EDDP_HW_HERA_FPGA) )
            #endif
            {
                EDDP_SWI_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PHY_NEC_Init(HwPortID=%d): FATAL -> MediaType is Tx-Mode but no Tx-Mode is set in SCRB(0x%X)!", 
                    HwPortID, RegVal);
                EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
            }
            RegVal &= ~0x0400;  // clear Reg18.10: Tx-Mode
         }

        g_EDDP_PHY_MDI_X_MAPPING[DevId][HwPortID-1] = IsMIDX;
        // a global structure to transfer the linkSpeedModeConfigured between the phy service and the prm service.
        // for copper Phys it is initiliazed with autoneg
        g_EDDP_PHY_LINK_SPEED_MODE_CONFIGURED[DevId][HwPortID-1]= EDD_LINK_AUTONEG ;
    }
    // write new value to register
    eddp_ll_PhyRegisterWrite (hDDB, HwPortID, EDDP_PHY_NEC_REG_SPECIAL_MODES, RegVal);

    #ifdef EDDP_PHY_NEC_FPGA_FX2TX_PORT_RESET
	// reset PHY if this is the FPGA version and the Port shall change from Fx to Tx.
	// This is only neccessary if the Port was configured for Fx within SCRB but will
	// be used as Tx port now. PHY seems to need a reset if Fx is changed to TX to work properly.
    if (( Hardware == EDDP_HW_ERTEC200P_FPGA || Hardware == EDDP_HW_HERA_FPGA ) && (Fx2TxChange))
    {
		LSA_UINT32      i = 0;

        eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_PHY_NEC_REG_BCMR, &RegVal);
		RegVal |= 0x8000;
		eddp_ll_PhyRegisterWrite (hDDB, HwPortID, EDDP_PHY_NEC_REG_BCMR, RegVal);

        do 
		{
			eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_PHY_NEC_REG_BCMR, &RegVal);
			EDDP_HALWaitNsTicks ((EDDP_LOCAL_DDB_PTR_TYPE)hDDB, EDDP_HAL_10US_TO);
			i++;
		}
		while ((RegVal & 0x8000) && (i < 1000));

        if (i >= 1000)
		{
            EDDP_SWI_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "[H:--] EDDP_PHY_NEC_Init(HwPortID=%d): FATAL -> PHY-Reset failed!", 
                HwPortID);
            EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
		}
	}
    #endif

    // ***** JK optimization enable
    eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_PHY_NEC_REG_SPECIAL_CTL, &RegVal);
    RegVal |= 0x0002;   // Reg31.1: '1' � rx_dv rises on "J" delimiter falls on "T" delimiter
    eddp_ll_PhyRegisterWrite (hDDB, HwPortID, EDDP_PHY_NEC_REG_SPECIAL_CTL, RegVal);

    // ***** set values for BER-Counter
    eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_PHY_NEC_REG_BER_CNT, &RegVal);
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
    eddp_ll_PhyRegisterWrite (hDDB, HwPortID, EDDP_PHY_NEC_REG_BER_CNT, RegVal);

    #ifdef EDDP_DISABLE_10MBIT_SUPPORT
	// ***** Configure PHY for 100 MBit only
    eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_PHY_NEC_REG_NEG_ADVERTISEMENT, &RegVal);
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

    eddp_ll_PhyRegisterWrite (hDDB, HwPortID, EDDP_PHY_NEC_REG_NEG_ADVERTISEMENT, RegVal);
    #endif

    LSA_UNUSED_ARG(hSysDev);
    LSA_UNUSED_ARG(Hardware);

	return EDD_STS_OK;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_PHY_NEC_LoadDelayValues                +*/
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
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NEC_LoadDelayValues(
	EDDP_HANDLE				        hDDB,
	LSA_UINT32						Hardware,
    EDDP_PHY_LINK_STATUS_PTR_TYPE   pLinkStatus)
{
    LSA_UNUSED_ARG(hDDB);

    switch (Hardware)
    {
        /* ------------ */
        /* ASIC         */
        /* ------------ */
        case EDDP_HW_ERTEC200P:
        {
            if (pLinkStatus->MediaType == EDD_MEDIATYPE_FIBER_OPTIC_CABLE)
            {
                if (pLinkStatus->FxTransceiverType == EDD_FX_TRANSCEIVER_QFBR5978)
                {
                    // POF-Mode
                    pLinkStatus->RealPortTxDelay    = EDDP_PHY_NEC_REAL_PORT_TX_DELAY_ERTEC200P_POF;
                    pLinkStatus->RealPortRxDelay    = EDDP_PHY_NEC_REAL_PORT_RX_DELAY_ERTEC200P_POF;
                    pLinkStatus->MaxPortTxDelay     = EDDP_PHY_NEC_MAX_PORT_TX_DELAY_ERTEC200P_POF;
                    pLinkStatus->MaxPortRxDelay     = EDDP_PHY_NEC_MAX_PORT_RX_DELAY_ERTEC200P_POF;
                }
                else if (pLinkStatus->FxTransceiverType == EDD_FX_TRANSCEIVER_QFBR5978_2)
                {
                    // FX-Mode
                    pLinkStatus->RealPortTxDelay    = EDDP_PHY_NEC_REAL_PORT_TX_DELAY_ERTEC200P_POF_2;
                    pLinkStatus->RealPortRxDelay    = EDDP_PHY_NEC_REAL_PORT_RX_DELAY_ERTEC200P_POF_2;
                    pLinkStatus->MaxPortTxDelay     = EDDP_PHY_NEC_MAX_PORT_TX_DELAY_ERTEC200P_POF_2;
                    pLinkStatus->MaxPortRxDelay     = EDDP_PHY_NEC_MAX_PORT_RX_DELAY_ERTEC200P_POF_2;
                }
                else if (pLinkStatus->FxTransceiverType == EDD_FX_TRANSCEIVER_AFBR59E4APZ)
                {
                    // FX-Mode
                    pLinkStatus->RealPortTxDelay = EDDP_PHY_NEC_REAL_PORT_TX_DELAY_ERTEC200P_FX;
                    pLinkStatus->RealPortRxDelay = EDDP_PHY_NEC_REAL_PORT_RX_DELAY_ERTEC200P_FX;
                    pLinkStatus->MaxPortTxDelay = EDDP_PHY_NEC_MAX_PORT_TX_DELAY_ERTEC200P_FX;
                    pLinkStatus->MaxPortRxDelay = EDDP_PHY_NEC_MAX_PORT_RX_DELAY_ERTEC200P_FX;
                }
                else
                {
                    EDDP_SWI_TRACE_01(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "EDDP_PHY_NEC_LoadDelayValues: FATAL -> FxTransceiverType (0x%X) illegal value!",
                        pLinkStatus->FxTransceiverType);
                    EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
                }
            }
            else
            {
                // Tx-Mode
			    pLinkStatus->RealPortTxDelay    = EDDP_PHY_NEC_REAL_PORT_TX_DELAY_ERTEC200P;
			    pLinkStatus->RealPortRxDelay    = EDDP_PHY_NEC_REAL_PORT_RX_DELAY_ERTEC200P;
                pLinkStatus->MaxPortTxDelay     = EDDP_PHY_NEC_MAX_PORT_TX_DELAY_ERTEC200P;
                pLinkStatus->MaxPortRxDelay     = EDDP_PHY_NEC_MAX_PORT_RX_DELAY_ERTEC200P;
            }
        }
		break;

        /* ------------ */
        /* FPGA         */
        /* ------------ */
        case EDDP_HW_ERTEC200P_FPGA:
        case EDDP_HW_HERA_FPGA:    
        {
            if (pLinkStatus->MediaType == EDD_MEDIATYPE_FIBER_OPTIC_CABLE)
            {
                // Fx-Mode
                EDDP_SWI_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PHY_NEC_LoadDelayValues(): ERROR -> NEC-PHY not supported with ERTEC200P-FPGA in Fx-Mode!");
			    return EDD_STS_ERR_PARAM;
            }
            else
            {
                // Tx-Mode
			    pLinkStatus->RealPortTxDelay    = EDDP_PHY_NEC_REAL_PORT_TX_DELAY_FPGA;
			    pLinkStatus->RealPortRxDelay    = EDDP_PHY_NEC_REAL_PORT_RX_DELAY_FPGA;
                pLinkStatus->MaxPortTxDelay     = EDDP_PHY_NEC_MAX_PORT_TX_DELAY_FPGA;
                pLinkStatus->MaxPortRxDelay     = EDDP_PHY_NEC_MAX_PORT_RX_DELAY_FPGA;
            }
        }
		break;

        default:
        {
            EDDP_SWI_TRACE_00(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_ERROR, "[H:--] EDDP_PHY_NEC_LoadDelayValues(): ERROR -> HardwareType not supported!");
			return EDD_STS_ERR_PARAM;
        }
		break;
    }

    return EDD_STS_OK;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_PHY_NEC_GetLinkStatus                  +*/
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
/*+  Results                : EDD_STS_OK                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Description: This function determines the current link status, speed   +*/
/*+               and duplexity from the register values passed when the    +*/
/*+               function is called. It then stores this information in    +*/
/*+               Link, Speed and Duplexity. Additionally, the current      +*/
/*+               MAUType, MediaType, AutonegCapAdvertised, TxDelay, RxDelay+*/
/*+               IsWireless and IsMDIX are returned.                       +*/
/*+               If the transceiver does not support auto-negotiation it   +*/
/*+               has to return �0� for �AutonegCapAdvertised�.             +*/
/*+                                                                         +*/
/*+       Note:   EDDP expects this function to return with success. Any    +*/
/*+               other return value than �EDD_STS_OK� will be interpreted  +*/
/*+               as a fatal error.                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NEC_GetLinkStatus(
	EDDP_HANDLE				        hDDB,
	EDDP_SYS_HANDLE			        hSysDev,
	LSA_UINT32						Hardware,
    LSA_UINT32				        HwPortID,
    EDDP_PHY_LINK_STATUS_PTR_TYPE   pLinkStatus)
{
    LSA_UINT32		RegVal;
	LSA_UINT32      DevId;
    EDD_RSP         Response;

	DevId = ((EDDP_LOCAL_DDB_PTR_TYPE)hDDB)->DevId;
	EDDP_ASSERT_FALSE(DevId >= EDDP_CFG_MAX_DEVICES);

	// ***** information if transceiver is TX or FX
	pLinkStatus->MediaType          = g_EDDP_PHY_MEDIATYPE        [DevId][HwPortID-1];
	pLinkStatus->OpticalTransType   = g_EDDP_PHY_OPTICALTRANSTYPE [DevId][HwPortID-1];
    pLinkStatus->FxTransceiverType  = g_EDDP_PHY_FXTRANSCEIVERTYPE[DevId][HwPortID-1];

    EDDP_SWI_TRACE_04(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_NOTE,"[H:--] EDDP_PHY_NEC_GetLinkStatus(HwPortID=%d): RegLink=%Xh, RegSpeed=%Xh, RegDuplexity=%X", 
        HwPortID, pLinkStatus->Link, pLinkStatus->Speed, pLinkStatus->Duplexity);

    // ***** Examine passed parameters in order to determine link, speed and duplexity.
    pLinkStatus->AutonegCapAdvertised   = EDD_AUTONEG_CAP_NONE;
    pLinkStatus->Link                   = (pLinkStatus->Link & EDDP_PHY_NEC_LINKSTATUSBITMASK) ? EDD_LINK_UP : EDD_LINK_DOWN;

    //Attention: TFS2141324: Due to polling times, LinkStatus can still be UP (previous pollcycle) while real LinkStatus is DOWN already and speed/duplexity undefined.
    //           ==> 10MBit or HD have to be allowed!  
    if (pLinkStatus->Link == EDD_LINK_UP)
    {
        pLinkStatus->Speed     = ( pLinkStatus->Speed & EDDP_PHY_NEC_SPEEDBITMASK )         ? EDD_LINK_SPEED_10  : EDD_LINK_SPEED_100;
	    pLinkStatus->Duplexity = ( pLinkStatus->Duplexity & EDDP_PHY_NEC_DUPLEXITYBITMASK ) ? EDD_LINK_MODE_FULL : EDD_LINK_MODE_HALF;
        // ***** Determine current MAUType
        EDDP_PHY_NEC_GET_MAU_TYPE(hDDB, hSysDev, HwPortID, pLinkStatus->Speed, pLinkStatus->Duplexity, &pLinkStatus->MAUType);
    }
    else
    {
        #if defined EDDP_CFG_DUMP_NEC_REGS_ON_LINKDOWN
            EDDP_SWI_TRACE_04(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_WARN,"[H:--] EDDP_PHY_NEC_GetLinkStatus(HwPortID=%d, !!!DOWN!!!): RegLink=%Xh, RegSpeed=%Xh, RegDuplexity=%X", 
                HwPortID, pLinkStatus->Link, pLinkStatus->Speed, pLinkStatus->Duplexity);
            EDDP_PHY_NEC_DumpRegs(hDDB, HwPortID);
        #endif //EDDP_CFG_DUMP_NEC_REGS_ON_LINKDOWN

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
        // Advertisement 
        eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_PHY_NEC_REG_NEG_ADVERTISEMENT, &RegVal);

        if (RegVal & 0x0020)    { pLinkStatus->AutonegCapAdvertised |= EDD_AUTONEG_CAP_10BASET    ; }   // 10Mbps able
        if (RegVal & 0x0040)    { pLinkStatus->AutonegCapAdvertised |= EDD_AUTONEG_CAP_10BASETFD  ; }   // 10Mbps with full duplex
        if (RegVal & 0x0080)    { pLinkStatus->AutonegCapAdvertised |= EDD_AUTONEG_CAP_100BASETX  ; }   // 100BASE-TX able
        if (RegVal & 0x0100)    { pLinkStatus->AutonegCapAdvertised |= EDD_AUTONEG_CAP_100BASETXFD; }   // 100BASE-TX able with full duplex
        if (RegVal & 0x0200)    { pLinkStatus->AutonegCapAdvertised |= EDD_AUTONEG_CAP_100BASET4  ; }   // 100BASE-T4 able

        // ***** Pause Operation
        switch (RegVal & 0x0C00)
        {
            // ***** 11: Both Symmetric PAUSE and Asymmetric PAUSE toward local device
            case 0x0C00:
                pLinkStatus->AutonegCapAdvertised |= EDD_AUTONEG_CAP_FDXBPAUSE;
                break;
            // ***** 10: Symmetric PAUSE
            case 0x0800:
                pLinkStatus->AutonegCapAdvertised |= EDD_AUTONEG_CAP_FDXSPAUSE;
                break;
            // ***** 01: Asymmetric PAUSE toward link Partner
            case 0x0400:
                pLinkStatus->AutonegCapAdvertised |= EDD_AUTONEG_CAP_FDXAPAUSE;
                break;
            // ***** 00: No PAUSE
            default:
                break;
        }
    }

    // ***** set Port delay values
    Response = EDDP_PHY_NEC_LoadDelayValues(hDDB, Hardware, pLinkStatus);
    if (EDD_STS_OK != Response)
    {
        //Traceentries are made in EDDP_PHY_NEC_LoadDelayValues
        return (Response);
    }

    // ***** set IsWireles
    pLinkStatus->IsWireless = g_EDDP_PHY_ISWIRELESS[DevId][HwPortID-1];
    // ***** set IsMDIX
    pLinkStatus->IsMDIX     = g_EDDP_PHY_MDI_X_MAPPING[DevId][HwPortID-1];  

    pLinkStatus->LinkSpeedModeConfigured     = g_EDDP_PHY_LINK_SPEED_MODE_CONFIGURED[DevId][HwPortID-1]; 

    return EDD_STS_OK;
}




/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_PHY_NEC_SetPowerDown                   +*/
/*+  Input/Output          :    EDDP_HANDLE             hDDB                +*/
/*+                        :    EDDP_SYS_HANDLE         hSysDev             +*/
/*+                        :    LSA_UINT32              HwPortID            +*/
/*+                        :    LSA_BOOL                PowerDown           +*/
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
/*+               other return value than �EDD_STS_OK� will be interpreted  +*/
/*+               as a fatal error.                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NEC_SetPowerDown(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID, 
    LSA_UINT8               PowerDown)
{
    LSA_UINT32      RegVal;

    LSA_UNUSED_ARG(hSysDev);

    eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_PHY_NEC_REG_BCMR, &RegVal);

    if (EDDP_PHY_POWERDOWN == PowerDown)
    {
        eddp_ll_PhyRegisterWrite (hDDB,HwPortID, EDDP_PHY_NEC_REG_BCMR, (RegVal | 0x0800/*power down*/)); 
    }
    else
    {
        eddp_ll_PhyRegisterWrite (hDDB,HwPortID, EDDP_PHY_NEC_REG_BCMR, (RegVal & ~(0x0800/*normal mode*/|0x4000/*release Loopback*/))); 
    }
    
    return EDD_STS_OK;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_PHY_NEC_CheckPowerDown                 +*/
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

EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NEC_CheckPowerDown(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID,
    LSA_UINT8*              pIsPowerDown)
{
    LSA_UINT32      RegVal;

    LSA_UNUSED_ARG(hSysDev);

    eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_PHY_NEC_REG_BCMR, &RegVal);

    if(RegVal & 0x0800/*power down*/)
    {
        *pIsPowerDown = EDDP_PHY_POWERDOWN;
    }
    else
    {
        *pIsPowerDown = EDDP_PHY_POWERUP;
    }

    return EDD_STS_OK;
}



/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname               :    EDDP_PHY_NEC_SetSpeedDuplexityOrAutoneg     +*/
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
/*+               �Autoneg� parameter is set to LSA_FALSE. If �Autoneg� is       +*/
/*+               set to LSA_TRUE �Speed� and �Duplexity� parameters are         +*/
/*+               irrelevant. In case the transceiver does not support           +*/
/*+               auto-negotiation it has to set up a connection at least at     +*/
/*+               100MBit full duplex and has to return �0� for                  +*/
/*+               �AutonegCapAdvertised� (see EDDP_PHY_GetLinkStatus).           +*/
/*+                                                                              +*/
/*+       Note:   EDDP expects this function to return with success. Any         +*/
/*+               other return value than �EDD_STS_OK� will be interpreted       +*/
/*+               as a fatal error.                                              +*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NEC_SetSpeedDuplexityOrAutoneg(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID,
    LSA_UINT8               *pLinkSpeedModeConfigured,
    LSA_UINT32              Speed,
    LSA_UINT32              Duplexity,
    LSA_UINT8               Autoneg)
{
    LSA_UINT32      RegVal;
    LSA_UINT32      DevId;
    EDD_RSP         ReturnCode=LSA_RSP_ERR_PARAM;

	DevId = ((EDDP_LOCAL_DDB_PTR_TYPE)hDDB)->DevId;
	EDDP_ASSERT_FALSE(DevId >= EDDP_CFG_MAX_DEVICES);

    eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_PHY_NEC_REG_BCMR, &RegVal);

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

    if (EDD_AUTONEG_ON == Autoneg)
    {
        eddp_ll_PhyRegisterWrite (hDDB, HwPortID, EDDP_PHY_NEC_REG_BCMR, (RegVal | 0x1000/*enable Auto-Neg*/));
        eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_PHY_NEC_REG_CONTROL, &RegVal);
        // ***** result depends on auto-neg settings
	    eddp_ll_PhyRegisterWrite (hDDB, HwPortID, EDDP_PHY_NEC_REG_CONTROL, RegVal | 0x0080 /*Auto Detect MDI/MDIX mode*/);
        /* restart AutoNegotiation
        eddp_ll_PhyRegisterRead(hDDB, HwPortID, EDDP_PHY_NEC_REG_BCMR, &RegVal);
        eddp_ll_PhyRegisterWrite(hDDB, HwPortID, EDDP_PHY_NEC_REG_BCMR, (RegVal | 0x200));// restart AutoNeg
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
        
        eddp_ll_PhyRegisterWrite (hDDB, HwPortID, EDDP_PHY_NEC_REG_BCMR, (RegVal & ~0x1000/*disable Auto-Neg*/));

        eddp_ll_PhyRegisterRead (hDDB, HwPortID, EDDP_PHY_NEC_REG_CONTROL, &RegVal);
        RegVal &= ~0x0080; /*Manual MDI/MDIX mode*/

        if (g_EDDP_PHY_MDI_X_MAPPING[DevId][HwPortID-1])
        {
            // ***** force this port to operate in MDI-X mode
	        eddp_ll_PhyRegisterWrite (hDDB, HwPortID, EDDP_PHY_NEC_REG_CONTROL, (RegVal | 0x0040/*MDIX mode*/));
        }
        else
        {
            // ***** force this port to operate in MDI mode
	        eddp_ll_PhyRegisterWrite (hDDB, HwPortID, EDDP_PHY_NEC_REG_CONTROL, (RegVal & ~0x0040/*MDI mode*/));
        }

    }
    ReturnCode=EDDP_PHY_MapAutonegSpeedDuplexityToLinkSpeedModeConfigured(Autoneg,Speed,Duplexity,pLinkSpeedModeConfigured);
    EDDP_ASSERT(LSA_RSP_OK==ReturnCode);
    g_EDDP_PHY_LINK_SPEED_MODE_CONFIGURED[DevId][HwPortID-1]=*pLinkSpeedModeConfigured;
    LSA_UNUSED_ARG(hSysDev);

    return EDD_STS_OK;
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_PHY_NEC_CheckMAUType                   +*/
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

EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NEC_CheckMAUType(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID,
    LSA_UINT16              MAUType,
    LSA_UINT32              *pSpeed,
    LSA_UINT32              *pDuplexity)
{
    EDD_RSP     Result;

    Result = EDDP_PHY_NEC_CHECK_MAU_TYPE(hDDB, hSysDev, HwPortID, MAUType, pSpeed, pDuplexity);

    return Result;
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_PHY_NEC_Get_Media_Type                 +*/
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
LSA_VOID EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NEC_Get_Media_Type  (EDDP_HANDLE            hDDB,
                                                                EDDP_SYS_HANDLE        hSysDev,
                                                                LSA_UINT32             HwPortID,
                                                                LSA_UINT8              *pMediaType,
                                                                LSA_UINT8              *pOpticalTransType,
                                                                LSA_UINT8		       *pFxTransceiverType)
{
	LSA_UINT32 DevId = ((EDDP_LOCAL_DDB_PTR_TYPE)hDDB)->DevId;
	EDDP_ASSERT_FALSE(DevId >= EDDP_CFG_MAX_DEVICES);

    EDDP_PHY_NEC_GET_MEDIA_TYPE(hDDB, hSysDev, HwPortID, pMediaType, pOpticalTransType, pFxTransceiverType);

    if (   (EDD_MEDIATYPE_FIBER_OPTIC_CABLE == *pMediaType)
        && (EDD_PORT_OPTICALTYPE_ISPOF   != *pOpticalTransType)
        && (EDD_PORT_OPTICALTYPE_ISOTHER != *pOpticalTransType))
    {
        EDDP_SWI_TRACE_02(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_FATAL, "EDDP_PHY_NEC_Get_Media_Type(HwPortID=%d): FATAL -> OpticalTransType (%d) illegal value!",
            HwPortID, *pOpticalTransType);
        EDDP_FATAL(EDDP_FATAL_ERR_INCONSISTENCE);
    }

    g_EDDP_PHY_MEDIATYPE        [DevId][HwPortID - 1] = *pMediaType;
    g_EDDP_PHY_OPTICALTRANSTYPE [DevId][HwPortID - 1] = *pOpticalTransType;
    g_EDDP_PHY_FXTRANSCEIVERTYPE[DevId][HwPortID - 1] = *pFxTransceiverType;
}

#if defined EDDP_CFG_DUMP_NEC_REGS_ON_LINKDOWN
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+  Functionname          :    EDDP_PHY_NEC_DumpRegs                       +*/
/*+  Input/Output          :    EDDP_HANDLE             hDDB                +*/
/*+                        :    LSA_UINT32              HwPortID            +*/
/*+                                                                         +*/
/*+  Result                :    LSA_VOID                                    +*/
/*+-------------------------------------------------------------------------+*/
/*+  Input/Output:                                                          +*/
/*+                                                                         +*/
/*+   hDDB       : EDDP Handle                                         (in) +*/
/*+   HwPortID   : Hardware port ID of the related PHY                 (in) +*/
/*+                                                                         +*/
/*+-------------------------------------------------------------------------+*/
/*+                                                                         +*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LSA_VOID EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_NEC_DumpRegs(
    EDDP_HANDLE				    hDDB,
    LSA_UINT32				    HwPortID)
{
    LSA_UINT32      i;
    LSA_UINT32      PhyRegs[32];
    EDDP_LOCAL_DDB_PTR_TYPE     pDDB;
    PNIP_STATISTIC_MEM_STRUCT_PTR_TYPE      pStatistic_Mem;
    LSA_UINT32      ErrCode_DgFCSErr_Px;
    LSA_UINT32      ErrCode_DisableErr_Px;
    LSA_UINT32      ErrCode_FCSErr_Px;
    LSA_UINT32      ErrCode_InPackPosLengthErr_Px;
    LSA_UINT32      ErrCode_InPackShortErr_Px;
    LSA_UINT32      ErrCode_LongErr_Px;
    LSA_UINT32      ErrCode_MACBufferErr_Px;
    LSA_UINT32      ErrCode_MACSAErr_Px;
    LSA_UINT32      ErrCode_NoResourceErr_Px;
    LSA_UINT32      ErrCode_PackParErr_Px;
    LSA_UINT32      ErrCode_PTCPErr_Px;
    LSA_UINT32      ErrCode_RxErr_Px;
    LSA_UINT32      ErrCode_ShortErr_Px;
    
    EDDP_CoreDevGetDDB (hDDB, &pDDB);
    pStatistic_Mem = (PNIP_STATISTIC_MEM_STRUCT_PTR_TYPE)EDDP_HALGetRAMBaseAddress(pDDB, EDDP_HAL_RAMTYPE_STATISTIC, LSA_NULL);
    EDDP_ASSERT_NO_NULL_PTR(pStatistic_Mem);

    for (i=0; i<=31; i++)
    {
        switch (i)
        {
            case 2:
            case 3:
            case 16:
            case 20:
            case 21:
            case 22:
                break;

            default:
            {
                eddp_ll_PhyRegisterRead (hDDB, HwPortID, i, &PhyRegs[i]);
                break;
            }
        }
    }

    EDDP_SWI_TRACE_08(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_WARN, "[H:--] EDDP_PHY_NEC_DUMP(HwPortID=%d): 0:0x%x, 1:0x%X, 4:0x%X, 5:0x%X, 6:0x%X, 7:0x%X, 8:0x%X",
        HwPortID, PhyRegs[0], PhyRegs[1], PhyRegs[4], PhyRegs[5], PhyRegs[6], PhyRegs[7], PhyRegs[8] );
    EDDP_SWI_TRACE_08(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_WARN, "[H:--] EDDP_PHY_NEC_DUMP 9:0x%x, 10:0x%X, 11:0x%X, 12:0x%X, 13:0x%X, 14:0x%X, 15:0x%X, 17:0x%X",
        PhyRegs[9], PhyRegs[10], PhyRegs[11], PhyRegs[12], PhyRegs[13], PhyRegs[14], PhyRegs[15], PhyRegs[17] );
    EDDP_SWI_TRACE_08(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_WARN, "[H:--] EDDP_PHY_NEC_DUMP 18:0x%x, 19:0x%X, 23:0x%X, 24:0x%X, 25:0x%X, 26:0x%X, 27:0x%X, 28:0x%X",
        PhyRegs[18], PhyRegs[19], PhyRegs[23], PhyRegs[24], PhyRegs[25], PhyRegs[26], PhyRegs[27], PhyRegs[28] );
    EDDP_SWI_TRACE_03(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_WARN, "[H:--] EDDP_PHY_NEC_DUMP 29:0x%x, 30:0x%X, 31:0x%X",
        PhyRegs[29], PhyRegs[30], PhyRegs[31]);

    ErrCode_DgFCSErr_Px         = EDDP_HAL_MEM32_READ(pDDB, &pStatistic_Mem->PortStatistic[HwPortID-1].ErrCode_DgFCSErr_Px);
    ErrCode_DisableErr_Px       = EDDP_HAL_MEM32_READ(pDDB, &pStatistic_Mem->PortStatistic[HwPortID-1].ErrCode_DisableErr_Px);
    ErrCode_FCSErr_Px           = EDDP_HAL_MEM32_READ(pDDB, &pStatistic_Mem->PortStatistic[HwPortID-1].ErrCode_FCSErr_Px);
    ErrCode_InPackPosLengthErr_Px= EDDP_HAL_MEM32_READ(pDDB, &pStatistic_Mem->PortStatistic[HwPortID-1].ErrCode_InPackPosLengthErr_Px);
    ErrCode_InPackShortErr_Px   = EDDP_HAL_MEM32_READ(pDDB, &pStatistic_Mem->PortStatistic[HwPortID-1].ErrCode_InPackShortErr_Px);
    ErrCode_LongErr_Px          = EDDP_HAL_MEM32_READ(pDDB, &pStatistic_Mem->PortStatistic[HwPortID-1].ErrCode_LongErr_Px);
    ErrCode_MACBufferErr_Px     = EDDP_HAL_MEM32_READ(pDDB, &pStatistic_Mem->PortStatistic[HwPortID-1].ErrCode_MACBufferErr_Px);
    ErrCode_MACSAErr_Px         = EDDP_HAL_MEM32_READ(pDDB, &pStatistic_Mem->PortStatistic[HwPortID-1].ErrCode_MACSAErr_Px);
    ErrCode_NoResourceErr_Px    = EDDP_HAL_MEM32_READ(pDDB, &pStatistic_Mem->PortStatistic[HwPortID-1].ErrCode_NoResourceErr_Px);
    ErrCode_PackParErr_Px       = EDDP_HAL_MEM32_READ(pDDB, &pStatistic_Mem->PortStatistic[HwPortID-1].ErrCode_PackParErr_Px);
    ErrCode_PTCPErr_Px          = EDDP_HAL_MEM32_READ(pDDB, &pStatistic_Mem->PortStatistic[HwPortID-1].ErrCode_PTCPErr_Px);
    ErrCode_RxErr_Px            = EDDP_HAL_MEM32_READ(pDDB, &pStatistic_Mem->PortStatistic[HwPortID-1].ErrCode_RxErr_Px);
    ErrCode_ShortErr_Px         = EDDP_HAL_MEM32_READ(pDDB, &pStatistic_Mem->PortStatistic[HwPortID-1].ErrCode_ShortErr_Px);
    
    EDDP_SWI_TRACE_07(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_WARN, "[H:--] EDDP_PHY_NEC_DUMP ErrCode_DgFCSErr_Px:0x%x, ErrCode_DisableErr_Px:0x%X, ErrCode_FCSErr_Px:0x%X, ErrCode_InPackPosLengthErr_Px:0x%X, ErrCode_InPackShortErr_Px:0x%X, ErrCode_LongErr_Px:0x%X, ErrCode_MACBufferErr_Px:0x%X",
        ErrCode_DgFCSErr_Px, ErrCode_DisableErr_Px, ErrCode_FCSErr_Px, ErrCode_InPackPosLengthErr_Px, ErrCode_InPackShortErr_Px, ErrCode_LongErr_Px, ErrCode_MACBufferErr_Px );
    EDDP_SWI_TRACE_06(EDDP_UNDEF_TRACE_IDX, LSA_TRACE_LEVEL_WARN, "[H:--] EDDP_PHY_NEC_DUMP ErrCode_MACSAErr_Px:0x%x, ErrCode_NoResourceErr_Px:0x%X, ErrCode_PackParErr_Px:0x%X, ErrCode_PTCPErr_Px:0x%X, ErrCode_RxErr_Px:0x%X, ErrCode_ShortErr_Px:0x%X",
        ErrCode_MACSAErr_Px, ErrCode_NoResourceErr_Px, ErrCode_PackParErr_Px, ErrCode_PTCPErr_Px, ErrCode_RxErr_Px, ErrCode_ShortErr_Px );
}
#endif //EDDP_CFG_DUMP_NEC_REGS_ON_LINKDOWN

#endif // EDDP_CFG_PHY_NEC_SUPPORT

/*****************************************************************************/
/*  end of file eddp_phy_nec.c                                               */
/*****************************************************************************/
