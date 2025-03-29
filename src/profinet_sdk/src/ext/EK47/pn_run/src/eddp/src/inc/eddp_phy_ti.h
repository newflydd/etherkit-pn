#ifndef EDDP_PHY_TI_H                   /* ----- reinclude-protection ----- */
#define EDDP_PHY_TI_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

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
/*  F i l e               &F: eddp_phy_ti.h                                  */
/*                                                                           */
/*  V e r s i o n         &V: BC_PNRUN_P07.02.00.00_00.02.00.20         :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-09-21                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n:                                                   */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  H i s t o r y :                                                          */
/*  ________________________________________________________________________ */
/*                                                                           */
/*  Date        Who   What                                                   */
#ifdef EDDP_MESSAGE
/*  15.02.16    AG    initial version.                                       */
#endif
/*****************************************************************************/

/*=============================== Defines ===================================*/	

/*---------------------------------------------------------------------------*/
/* PHY specific parameters      											 */
/*---------------------------------------------------------------------------*/
#define EDDP_PHY_TI_OUI1REGADDRESS                  (LSA_UINT32)0x0002  // PHY Identifier Register 1
#define EDDP_PHY_TI_OUI1COMPAREVALUE                (LSA_UINT32)0x2000  // OUI
#define EDDP_PHY_TI_OUI2REGADDRESS		            (LSA_UINT32)0x0003  // PHY Identifier Register 2
#define EDDP_PHY_TI_OUI2COMPAREVALUE		        (LSA_UINT32)0xA212  // OUI
#define EDDP_PHY_TI_LINKSTATUSREGADDRESS	        (LSA_UINT32)0x0001  // BasicModeStatus Register
#define EDDP_PHY_TI_LINKSTATUSBITMASK               (LSA_UINT32)0x0004  // Link Status (0:no Link, 1:valid Link)
#define EDDP_PHY_TI_AUTONEGCOMPLETEBITMASK          (LSA_UINT32)0x0020  // Autoneg Complete Status (0:autoneg not complete, 1:autoneg complete)
#define EDDP_PHY_TI_LINKVALIDBITMASK                (LSA_UINT32)0x0020  // AutoNeg Complete (0:not, 1:complete)
#define EDDP_PHY_TI_SPEEDREGADDRESS	                (LSA_UINT32)0x0010  // PHY Status Register
#define EDDP_PHY_TI_SPEEDBITMASK                    (LSA_UINT32)0x0002  // Speed Status (0:100MBit, 1:10MBit)
#define EDDP_PHY_TI_SPEEDREGLINKSTATUSBITMASK       (LSA_UINT32)0x0001  // LinkStatus in speed register (0:no Link, 1:valid Link)
#define EDDP_PHY_TI_DUPLEXITYREGADDRESS	            (LSA_UINT32)0x0010  // PHY Status Register
#define EDDP_PHY_TI_DUPLEXITYBITMASK                (LSA_UINT32)0x0004  // Duplex Status (0:half duplex, 1:full duplex)
#define EDDP_PHY_TI_DUPLEXITYREGLINKSTATUSBITMASK   (LSA_UINT32)0x0001  // LinkStatus in duplexity register (0:no Link, 1:valid Link)


/*---------------------------------------------------------------------------*/
/* PHY register number                                                       */
/*---------------------------------------------------------------------------*/
#define EDDP_PHY_TI_REG_BCMR                       (LSA_UINT32)0   // BasicModeControlRegister --> controls PowerDown and AutoNegotiation
#define EDDP_PHY_TI_REG_NEG_ADVERTISEMENT          (LSA_UINT32)4   // Auto-Negotiation Advertisement Register --> AutoNeg Cap.
#define EDDP_PHY_TI_REG_SW_STRAP_CONTROL1          (LSA_UINT32)9   // Software Strap Control register 1 (SWSCR1)
#define EDDP_PHY_TI_REG_SW_STRAP_CONTROL3          (LSA_UINT32)11  // Software Strap Control register 3 (SWSCR1)
#define EDDP_PHY_TI_REG_CONTROL                    (LSA_UINT32)25  // Mode Control/Status Register --> MDI-X
#define EDDP_PHY_TI_REG_SPECIAL_MODES              (LSA_UINT32)18  // Special Modes register       --> FX-Mode
#define EDDP_PHY_TI_REG_LED_DIRECT_CONTROL         (LSA_UINT32)24  // LED DirectControl Register --> Led control
#define EDDP_PHY_TI_REG_BER_CNT                    (LSA_UINT32)23  // used for BER counter
#define EDDP_PHY_TI_REG_SPECIAL_CTL                (LSA_UINT32)31  // used for JK optimization

/*==========================================================================*/
/*====== SW Strap Control Register 1, 9d, 9h, TI ===========================*/
/*==========================================================================*/
#define EDDP_PHY_TI_SW_STRAP_CONTROL1_MASK_Fast_RXDV_Detection          (LSA_UINT32)0x0002
#define EDDP_PHY_TI_SW_STRAP_CONTROL1_MASK_RMII_Enhanced                (LSA_UINT32)0x0200
#define EDDP_PHY_TI_SW_STRAP_CONTROL1_MASK_LED_Cfg                      (LSA_UINT32)0x0400
#define EDDP_PHY_TI_SW_STRAP_CONTROL1_MASK_AN_0                         (LSA_UINT32)0x0800
#define EDDP_PHY_TI_SW_STRAP_CONTROL1_MASK_AN_1                         (LSA_UINT32)0x1000
#define EDDP_PHY_TI_SW_STRAP_CONTROL1_MASK_Auto_Negotiation_Enable      (LSA_UINT32)0x2000
#define EDDP_PHY_TI_SW_STRAP_CONTROL1_MASK_Auto_MDIX_Enable             (LSA_UINT32)0x4000
#define EDDP_PHY_TI_SW_STRAP_CONTROL1_MASK_SW_Strap_Config_Done         (LSA_UINT32)0x8000

/*==========================================================================*/
/*====== SW Strap Control Register 3, 11d, 11h, TI =========================*/
/*==========================================================================*/
#define EDDP_PHY_TI_SW_STRAP_CONTROL3_MASK_MDI_MDIX_Swap                (LSA_UINT32)0x0020

/*==========================================================================*/
/*====== SW Strap Control Register 1, 9d, 9h, TI ===========================*/
/*==========================================================================*/
#define EDDP_PHY_TI_PHY_CONTROL_BIT__Auto_MDIX_Enable                   (LSA_UINT32)0x8000
    
/*---------------------------------------------------------------------------*/
/* PHY delay values                                                          */
/*---------------------------------------------------------------------------*/
#define EDDP_PHY_TI_PHY_TX_DELAY                   50 
#define EDDP_PHY_TI_PHY_RX_DELAY                   180

/*---------------------------------------------------------------------------*/
/* SHIM-Versatz                                                              */
/*---------------------------------------------------------------------------*/
#define EDDP_PHY_TI_TX_SHIM_FPGA                   22  // = (20 + 24) / 2
#define EDDP_PHY_TI_RX_SHIM_FPGA                   22  // = (20 + 24) / 2
        
/*---------------------------------------------------------------------------*/
/* Real Port delay values                                                    */
/*---------------------------------------------------------------------------*/
#define EDDP_PHY_TI_REAL_PORT_TX_DELAY_HERA_FPGA        28
#define EDDP_PHY_TI_REAL_PORT_RX_DELAY_HERA_FPGA        202

/*---------------------------------------------------------------------------*/
/* Max Port delay values                                                     */
/*---------------------------------------------------------------------------*/
#define EDDP_PHY_TI_MAX_PORT_TX_DELAY_HERA_FPGA         30
#define EDDP_PHY_TI_MAX_PORT_RX_DELAY_HERA_FPGA         204

/*============================== Prototypes =================================*/

/*=============================================================================
 * function name:  EDDP_PHY_TI_Init
 *
 * function:       This function shall return all parameters of a specific PHY
 *                 needed for link change information. This includes PHY 
 *                 register addresses and bit masks for link status, speed mode
 *                 and duplexity.  
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_TI_Init(
    EDDP_HANDLE                 hDDB,
    EDDP_SYS_HANDLE             hSysDev,
    LSA_UINT32                  Hardware,
    LSA_UINT32                  HwPortID,
    LSA_UINT8                   IsWireless,
    LSA_UINT8                   IsMIDX,
    EDDP_PHY_INIT_PTR_TYPE      pInitData);

/*=============================================================================
 * function name:  EDDP_PHY_TI_LED_BlinkBegin
 *
 * function:       EDDP calls this function within the service 
 *				   EDD_SRV_LED_BLINK() before the LED(s) start to blink.
 *				   Can be used e.g. to diable the link/activity LED function 
 *			       of a PHY if necessary.  
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_TI_LED_BlinkBegin(
    EDDP_HANDLE         hDDB,
    EDDP_SYS_HANDLE     hSysDev,
    LSA_UINT32          HwPortID);

/*=============================================================================
 * function name:  EDDP_PHY_TI_LED_BlinkSetMode
 *
 * function:       EDDP calls this function repeatedly within the service 
 *				   EDD_SRV_LED_BLINK() in order to turn on and turn off 
 *				   the LED(s) alternately.
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_TI_LED_BlinkSetMode(
	EDDP_HANDLE 	    hDDB,
	EDDP_SYS_HANDLE     hSysDev,	
	LSA_UINT32          HwPortID,
	LSA_UINT32		    LEDMode);

/*=============================================================================
 * function name:  EDDP_PHY_TI_LED_BlinkEnd
 *
 * function:       EDDP calls this function within the service
 *				   EDD_SRV_LED_BLINK() after blinking of LED(s) has finished. 
 *				   Can be used e.g. to re-enable the link/activity 
 *				   LED function of a PHY if necessary.
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_TI_LED_BlinkEnd(
	EDDP_HANDLE 	    hDDB,
	EDDP_SYS_HANDLE     hSysDev,
	LSA_UINT32          HwPortID);

/*=============================================================================
 * function name:  EDDP_PHY_TI_LoadDelayValues
 *
 * function:       Load delay parameters depending on MediaType and hardware
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_TI_LoadDelayValues(
    EDDP_HANDLE                     hDDB,
    LSA_UINT32                      Hardware,
    EDDP_PHY_LINK_STATUS_PTR_TYPE   pLinkStatus);

/*=============================================================================
 * function name:  EDDP_PHY_TI_GetLinkStatus
 *
 * function:       Examine passed parameters in order to determine 
 *				   link, speed and duplexity and returns MAUType, MediaType, 
 *                 AutonegCapAdvertised, TxDelay, RxDelay, IsWireless and 
 *                 IsMDIX.
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_TI_GetLinkStatus(
	EDDP_HANDLE				        hDDB,
	EDDP_SYS_HANDLE			        hSysDev,
	LSA_UINT32						Hardware,
    LSA_UINT32				        HwPortID,
    EDDP_PHY_LINK_STATUS_PTR_TYPE   pLinkStatus);

/*=============================================================================
 * function name:  EDDP_PHY_TI_SetPowerDown
 *
 * function:       This function turns the PHY into software power down mode 
 *                 ON or OFF. When software power mode is OFF there will be 
 *                 no link and the PHY does not receive or send any data but 
 *                 can still be controlled via SMI.  				   
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_TI_SetPowerDown(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID, 
    LSA_UINT8               PowerDown);

/*=============================================================================
 * function name:  EDDP_PHY_TI_CheckPowerDown
 *
 * function:       If the PHY is PowerDown, this function returns LSA_TRUE.  
 *                 Otherwise it returns LSA_FALSE
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_TI_CheckPowerDown(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID,
    LSA_UINT8*              pIsPowerDown);

/*=============================================================================
 * function name:  EDDP_PHY_TI_SetSpeedDuplexityOrAutoneg
 *
 * function:       This function sets speed mode and duplexity when the 
 *                 'Autoneg' parameter is set to LSA_FALSE. If 'Autoneg' is 
 *                 set to LSA_TRUE “Speed” and “Duplexity” parameters are 
 *                 irrelevant. In case the transceiver does not support 
 *                 auto-negotiation it has to set up a connection at least at 
 *                 100MBit full duplex.        
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_TI_SetSpeedDuplexityOrAutoneg(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID,
    LSA_UINT8               *pLinkSpeedModeConfigured,
    LSA_UINT32              Speed,
    LSA_UINT32              Duplexity,
    LSA_UINT8               Autoneg);

/*=============================================================================
 * function name:  EDDP_PHY_TI_CheckMAUType
 *
 * function:       This function checks if the given MAUType is supported by 
 *                 the hardware and returns the related Speed and Duplexity.
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_TI_CheckMAUType(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID,
    LSA_UINT16              MAUType,
    LSA_UINT32              *pSpeed,
    LSA_UINT32              *pDuplexity);

/*=============================================================================
 * function name:  EDDP_PHY_TI_Get_Media_Type
 *
 *  function:      This function gives media and POF type wich be returned   
 *                 by user defined functions and saves them internaly.       
 *===========================================================================*/
LSA_VOID EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_TI_Get_Media_Type(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID,
    LSA_UINT8              *pMediaType,
    LSA_UINT8              *pOpticalTransType,
    LSA_UINT8              *pFxTransceiverType);

/*****************************************************************************/
/*  end of file EDDP_PHY_TI.H                                               */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_PHY_TI_H */
