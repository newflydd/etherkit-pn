#ifndef EDDP_PHY_BCM5461S_H                   /* ----- reinclude-protection ----- */
#define EDDP_PHY_BCM5461S_H

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
/*  F i l e               &F: eddp_phy_bcm5461s.h                       :F&  */
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

/*=============================== Defines ===================================*/	
    
/*---------------------------------------------------------------------------*/
/* PHY register number                                                       */
/*---------------------------------------------------------------------------*/
#define EDDP_PHY_BCM5461S_REG_MII_CONTROL                           (LSA_UINT32)0x0000   // MII control register --> controls PowerDown, AutoNegotiation, Speed and Mode
#define EDDP_PHY_BCM5461S_REG_MII_CONTROL_RESERVED_MASK             (LSA_UINT32)0x003F
#define EDDP_PHY_BCM5461S_REG_MII_CONTROL_RESTART_AUTONEG           (LSA_UINT32)0x0200
#define EDDP_PHY_BCM5461S_REG_MII_CONTROL_POWER_DOWN                (LSA_UINT32)0x0800
#define EDDP_PHY_BCM5461S_REG_MII_CONTROL_AUTONEG_ENABLE            (LSA_UINT32)0x1000
#define EDDP_PHY_BCM5461S_REG_MII_CONTROL_100MBIT_ENABLE            (LSA_UINT32)0x2000  // 100 mbit: LSB (bit 13) = 1 && MSB (bit 6) = 0
#define EDDP_PHY_BCM5461S_REG_MII_CONTROL_FD_ENABLE                 (LSA_UINT32)0x0100
#define EDDP_PHY_BCM5461S_REG_MII_CONTROL_ISOLATE_MODE              (LSA_UINT32)0x0400
#define EDDP_PHY_BCM5461S_REG_MII_CONTROL_INTERNAL_LOOPBACK         (LSA_UINT32)0x4000
    
#define EDDP_PHY_BCM5461S_REG_MII_STATUS                            (LSA_UINT32)0x0001   // MII status register
#define EDDP_PHY_BCM5461S_REG_MII_STATUS_LINK_STATUS                (LSA_UINT32)0x0004   // bit 2: 1 = link up
#define EDDP_PHY_BCM5461S_REG_MII_STATUS_AUTONEG_COMPLETE           (LSA_UINT32)0x0020   // bit 5: 1 = autoneg is complete
    
#define EDDP_PHY_BCM5461S_REG_AUTONEG_ADVERTISEMENT                     (LSA_UINT32)0x0004   // Auto-Negotiation Advertisement Register --> AutoNeg Cap.
#define EDDP_PHY_BCM5461S_REG_AUTONEG_ADVERTISEMENT_10MBIT_MASK         (LSA_UINT32)0x0060
#define EDDP_PHY_BCM5461S_REG_AUTONEG_ADVERTISEMENT_100MBIT_MASK        (LSA_UINT32)0x0180
#define EDDP_PHY_BCM5461S_REG_AUTONEG_ADVERTISEMENT_PAUSE_MASK          (LSA_UINT32)0x0C00
#define EDDP_PHY_BCM5461S_REG_AUTONEG_ADVERTISEMENT_ASYMMETRIC_PAUSE    (LSA_UINT32)0x0800
#define EDDP_PHY_BCM5461S_REG_AUTONEG_ADVERTISEMENT_PAUSE_CAPABLE       (LSA_UINT32)0x0400
#define EDDP_PHY_BCM5461S_REG_AUTONEG_ADVERTISEMENT_RESERVED_MASK       (LSA_UINT32)0x5000
    
#define EDDP_PHY_BCM5461S_REG_GBIT_CONTROL                          (LSA_UINT32)0x0009   // 1000BASE-T control register (Gigabit support)
#define EDDP_PHY_BCM5461S_REG_GBIT_CONTROL_RESERVED_MASK            (LSA_UINT32)0x00FF    
    
#define EDDP_PHY_BCM5461S_REG_PHY_EXTENDED_CONTROL                      (LSA_UINT32)0x0010
#define EDDP_PHY_BCM5461S_REG_PHY_EXTENDED_CONTROL_RESERVED_MASK        (LSA_UINT32)0x0006
#define EDDP_PHY_BCM5461S_REG_PHY_EXTENDED_CONTROL_FORCE_LED_TRAFFIC    (LSA_UINT32)0x0020
#define EDDP_PHY_BCM5461S_REG_PHY_EXTENDED_CONTROL_FORCE_LED_ON         (LSA_UINT32)0x0010
#define EDDP_PHY_BCM5461S_REG_PHY_EXTENDED_CONTROL_FORCE_LED_OFF        (LSA_UINT32)0x0008
    
#define EDDP_PHY_BCM5461S_REG_PHY_EXTENDED_STATUS                   (LSA_UINT32)0x0011

#define EDDP_PHY_BCM5461S_REG_POWER_MII_CTRL                        (LSA_UINT32)0x0018
#define EDDP_PHY_BCM5461S_REG_POWER_MII_CTRL_READ_MASK              (LSA_UINT32)0x2007
#define EDDP_PHY_BCM5461S_REG_POWER_MII_CTRL_WRITE_MASK             (LSA_UINT32)0x0700
#define EDDP_PHY_BCM5461S_REG_POWER_MII_CTRL_RESERVED_MASK          (LSA_UINT32)0xF898
#define EDDP_PHY_BCM5461S_REG_POWER_MII_CTRL_SHADOW                 (LSA_UINT32)0x0002
#define EDDP_PHY_BCM5461S_REG_POWER_MII_CTRL_SUPER_ISOLATED         (LSA_UINT32)0x0020
#define EDDP_PHY_BCM5461S_REG_POWER_MII_CTRL_SET_SUPER_ISOLATED     (LSA_UINT32)0x0722
#define EDDP_PHY_BCM5461S_REG_POWER_MII_CTRL_RESET_SUPER_ISOLATED   (LSA_UINT32)0x0702
    
#define EDDP_PHY_BCM5461S_REG_MISC_CTRL                             (LSA_UINT32)0x0018
#define EDDP_PHY_BCM5461S_REG_MISC_CTRL_READ_MASK                   (LSA_UINT32)0x7007
#define EDDP_PHY_BCM5461S_REG_MISC_CTRL_WRITE_MASK                  (LSA_UINT32)0x8000
#define EDDP_PHY_BCM5461S_REG_MISC_CTRL_RESERVED_MASK               (LSA_UINT32)0x0410
#define EDDP_PHY_BCM5461S_REG_MISC_CTRL_SHADOW                      (LSA_UINT32)0x0007
#define EDDP_PHY_BCM5461S_REG_MISC_CTRL_SHADOW_MASK                 (LSA_UINT32)0x0007
#define EDDP_PHY_BCM5461S_REG_MISC_CTRL_RGMII_DELAY                 (LSA_UINT32)0x0100

    
#define EDDP_PHY_BCM5461S_REG_AUX_STATUS_SUMMARY                    (LSA_UINT32)0x0019
#define EDDP_PHY_BCM5461S_REG_AUX_STATUS_SUMMARY_RESERVED_MASK      (LSA_UINT32)0x0000
#define EDDP_PHY_BCM5461S_REG_AUX_STATUS_SUMMARY_AUTONEG_HCD        (LSA_UINT32)0x0700
#define EDDP_PHY_BCM5461S_REG_AUX_STATUS_SUMMARY_100MBIT_FD         (LSA_UINT32)0x0500
#define EDDP_PHY_BCM5461S_REG_AUX_STATUS_SUMMARY_100MBIT_HD         (LSA_UINT32)0x0300
#define EDDP_PHY_BCM5461S_REG_AUX_STATUS_SUMMARY_10MBIT_FD          (LSA_UINT32)0x0200
#define EDDP_PHY_BCM5461S_REG_AUX_STATUS_SUMMARY_10MBIT_HD          (LSA_UINT32)0x0100
#define EDDP_PHY_BCM5461S_REG_AUX_STATUS_SUMMARY_1000MBIT_FD        (LSA_UINT32)0x0700
#define EDDP_PHY_BCM5461S_REG_AUX_STATUS_SUMMARY_1000MBIT_HD        (LSA_UINT32)0x0600
    
#define EDDP_PHY_BCM5461S_REG_1C_ACCESS                             (LSA_UINT32)0x001C
#define EDDP_PHY_BCM5461S_REG_1C_ACCESS_READ                        (LSA_UINT32)0x0000
#define EDDP_PHY_BCM5461S_REG_1C_ACCESS_WRITE                       (LSA_UINT32)0x8000
    
#define EDDP_PHY_BCM5461S_REG_1C_CLOCK_ALIGN_SHADOW                 (LSA_UINT32)0x0C00
#define EDDP_PHY_BCM5461S_REG_1C_CLOCK_ALIGN_RESERVED_MASK          (LSA_UINT32)0x01FF
#define EDDP_PHY_BCM5461S_REG_1C_CLOCK_ALIGN_CLOCK_DELAY_EN         (LSA_UINT32)0x0200

#define EDDP_PHY_BCM5461S_REG_1C_LED_STATUS_SHADOW                  (LSA_UINT32)0x2000
#define EDDP_PHY_BCM5461S_REG_1C_LED_STATUS_RESERVED_MASK           (LSA_UINT32)0x0220
#define EDDP_PHY_BCM5461S_REG_1C_LED_STATUS_SLAVE                   (LSA_UINT32)0x0100
#define EDDP_PHY_BCM5461S_REG_1C_LED_STATUS_FDX                     (LSA_UINT32)0x0080
#define EDDP_PHY_BCM5461S_REG_1C_LED_STATUS_INTR                    (LSA_UINT32)0x0040
#define EDDP_PHY_BCM5461S_REG_1C_LED_STATUS_LINKSPD                 (LSA_UINT32)0x0018
#define EDDP_PHY_BCM5461S_REG_1C_LED_STATUS_LINKSPD2                (LSA_UINT32)0x0010
#define EDDP_PHY_BCM5461S_REG_1C_LED_STATUS_LINKSPD1                (LSA_UINT32)0x0008
#define EDDP_PHY_BCM5461S_REG_1C_LED_STATUS_TRANSMIT                (LSA_UINT32)0x0004
#define EDDP_PHY_BCM5461S_REG_1C_LED_STATUS_RECEIVE                 (LSA_UINT32)0x0002
#define EDDP_PHY_BCM5461S_REG_1C_LED_STATUS_QUALITY                 (LSA_UINT32)0x0001
    
#define EDDP_PHY_BCM5461S_REG_1C_LED_SELECTOR_1_SHADOW              (LSA_UINT32)0x3400
#define EDDP_PHY_BCM5461S_REG_1C_LED_SELECTOR_1_RESERVED_MASK       (LSA_UINT32)0x0300  
#define EDDP_PHY_BCM5461S_REG_1C_LED_SELECTOR_1_LED1_ON             (LSA_UINT32)0x000F
#define EDDP_PHY_BCM5461S_REG_1C_LED_SELECTOR_1_LED1_OFF            (LSA_UINT32)0x000E
#define EDDP_PHY_BCM5461S_REG_1C_LED_SELECTOR_1_LED1_LINKSPD1       (LSA_UINT32)0x0000
#define EDDP_PHY_BCM5461S_REG_1C_LED_SELECTOR_1_LED2_ON             (LSA_UINT32)0x00F0
#define EDDP_PHY_BCM5461S_REG_1C_LED_SELECTOR_1_LED2_OFF            (LSA_UINT32)0x00E0
#define EDDP_PHY_BCM5461S_REG_1C_LED_SELECTOR_1_LED2_LINKSPD2       (LSA_UINT32)0x0010
    
#define EDDP_PHY_BCM5461S_REG_1C_LED_SELECTOR_2_SHADOW              (LSA_UINT32)0x3800
#define EDDP_PHY_BCM5461S_REG_1C_LED_SELECTOR_2_RESERVED_MASK       (LSA_UINT32)0x0300    
#define EDDP_PHY_BCM5461S_REG_1C_LED_SELECTOR_2_LED3_ON             (LSA_UINT32)0x000F
#define EDDP_PHY_BCM5461S_REG_1C_LED_SELECTOR_2_LED3_OFF            (LSA_UINT32)0x000E
#define EDDP_PHY_BCM5461S_REG_1C_LED_SELECTOR_2_LED3_ACTIVITYLED    (LSA_UINT32)0x0003
#define EDDP_PHY_BCM5461S_REG_1C_LED_SELECTOR_2_LED4_ON             (LSA_UINT32)0x00F0
#define EDDP_PHY_BCM5461S_REG_1C_LED_SELECTOR_2_LED4_OFF            (LSA_UINT32)0x00E0
#define EDDP_PHY_BCM5461S_REG_1C_LED_SELECTOR_2_LED4_INTR           (LSA_UINT32)0x0060
   
#define EDDP_PHY_BCM5461S_REG_1C_ACCESS_AUTODETECT_SHADOW           (LSA_UINT32)0x7800
#define EDDP_PHY_BCM5461S_REG_1C_ACCESS_AUTODETECT_RESERVED_MASK    (LSA_UINT32)0x0218
#define EDDP_PHY_BCM5461S_REG_1C_ACCESS_AUTODETECT_COPPER           (LSA_UINT32)0x0000
#define EDDP_PHY_BCM5461S_REG_1C_ACCESS_AUTODETECT_FIBER            (LSA_UINT32)0x0000
    
#define EDDP_PHY_BCM5461S_REG_1C_ACCESS_MODE_CONTROL_SHADOW         (LSA_UINT32)0x7C00
#define EDDP_PHY_BCM5461S_REG_1C_ACCESS_MODE_CONTROL_COPPER         (LSA_UINT32)0x0008
#define EDDP_PHY_BCM5461S_REG_1C_ACCESS_MODE_CONTROL_FIBER          (LSA_UINT32)0x000B
    
#define EDDP_PHY_BCM5461S_REG_1C_ACCESS_LED_SELECTOR_1              (LSA_UINT32)0x3400
#define EDDP_PHY_BCM5461S_REG_1C_ACCESS_LED_SELECTOR_2              (LSA_UINT32)0x3800

#define EDDP_PHY_BCM5461S_REG_TEST_REGISTER_1                       (LSA_UINT32)0x001E
#define EDDP_PHY_BCM5461S_REG_TEST_REGISTER_1_RESERVED_MASK         (LSA_UINT32)0x7F7F
#define EDDP_PHY_BCM5461S_REG_TEST_REGISTER_1_SWAP_MDI              (LSA_UINT32)0x0080

/*---------------------------------------------------------------------------*/
/* PHY specific parameters                                                   */
/*---------------------------------------------------------------------------*/
#define EDDP_PHY_BCM5461S_OUI1REGADDRESS                (LSA_UINT32)0x0002  // PHY Identifier 1
#define EDDP_PHY_BCM5461S_OUI1COMPAREVALUE              (LSA_UINT32)0x0020  // OUI
#define EDDP_PHY_BCM5461S_OUI2REGADDRESS                (LSA_UINT32)0x0003  // PHY Identifier 2
#define EDDP_PHY_BCM5461S_OUI2COMPAREVALUE              (LSA_UINT32)0x60C1  // OUI
#define EDDP_PHY_BCM5461S_LINKSTATUSREGADDRESS          EDDP_PHY_BCM5461S_REG_MII_STATUS  // MII Status register
#define EDDP_PHY_BCM5461S_LINKSTATUSBITMASK             EDDP_PHY_BCM5461S_REG_MII_STATUS_LINK_STATUS  // Link Status
#define EDDP_PHY_BCM5461S_AUTONEGCOMPLETEBITMASK        EDDP_PHY_BCM5461S_REG_MII_STATUS_AUTONEG_COMPLETE  // Autoneg complete
#define EDDP_PHY_BCM5461S_SPEEDREGADDRESS               EDDP_PHY_BCM5461S_REG_AUX_STATUS_SUMMARY  // Auxiliary Status Summary
#define EDDP_PHY_BCM5461S_SPEEDBITMASK                  EDDP_PHY_BCM5461S_REG_AUX_STATUS_SUMMARY_AUTONEG_HCD  // Speed Indication: HCD
#define EDDP_PHY_BCM5461S_DUPLEXITYREGADDRESS           EDDP_PHY_BCM5461S_REG_AUX_STATUS_SUMMARY  // Auxiliary Status Summary
#define EDDP_PHY_BCM5461S_DUPLEXITYBITMASK              EDDP_PHY_BCM5461S_REG_AUX_STATUS_SUMMARY_AUTONEG_HCD  // Duplexity Indication: HCD
    
/*---------------------------------------------------------------------------*/
/* PHY delay values         											     */
/*---------------------------------------------------------------------------*/
#define EDDP_PHY_BCM5461S_PHY_TX_DELAY                   70 
#define EDDP_PHY_BCM5461S_PHY_RX_DELAY                   240

/*---------------------------------------------------------------------------*/
/* SHIM-Versatz                                                              */
/*---------------------------------------------------------------------------*/
#define EDDP_PHY_BCM5461S_TX_SHIM_FPGA                   22  // = (20 + 24) / 2
#define EDDP_PHY_BCM5461S_RX_SHIM_FPGA                   22  // = (20 + 24) / 2
    
/*---------------------------------------------------------------------------*/
/* Real Port delay values         											 */
/*---------------------------------------------------------------------------*/
#define EDDP_PHY_BCM5461S_REAL_PORT_TX_DELAY_HERA_FPGA        48
#define EDDP_PHY_BCM5461S_REAL_PORT_RX_DELAY_HERA_FPGA        262

/*---------------------------------------------------------------------------*/
/* Max Port delay values         											 */
/*---------------------------------------------------------------------------*/
#define EDDP_PHY_BCM5461S_MAX_PORT_TX_DELAY_HERA_FPGA         60
#define EDDP_PHY_BCM5461S_MAX_PORT_RX_DELAY_HERA_FPGA         264

/*============================== Prototypes =================================*/

/*=============================================================================
 * function name:  EDDP_PHY_BCM5461S_Init
 *
 * function:       This function shall return all parameters of a specific PHY
 *                 needed for link change information. This includes PHY 
 *                 register addresses and bit masks for link status, speed mode
 *                 and duplexity.  
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_BCM5461S_Init(
	EDDP_HANDLE				    hDDB,
	EDDP_SYS_HANDLE			    hSysDev,
	LSA_UINT32                  Hardware,
    LSA_UINT32				    HwPortID,
    LSA_UINT8  	                IsWireless,
    LSA_UINT8  	                IsMIDX,
	EDDP_PHY_INIT_PTR_TYPE      pInitData);

/*=============================================================================
 * function name:  EDDP_PHY_BCM5461S_LED_BlinkBegin
 *
 * function:       EDDP calls this function within the service 
 *				   EDD_SRV_LED_BLINK() before the LED(s) start to blink.
 *				   Can be used e.g. to diable the link/activity LED function 
 *			       of a PHY if necessary.  
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_BCM5461S_LED_BlinkBegin(
	EDDP_HANDLE 	    hDDB,
	EDDP_SYS_HANDLE     hSysDev,	
	LSA_UINT32          HwPortID);

/*=============================================================================
 * function name:  EDDP_PHY_BCM5461S_LED_BlinkSetMode
 *
 * function:       EDDP calls this function repeatedly within the service 
 *				   EDD_SRV_LED_BLINK() in order to turn on and turn off 
 *				   the LED(s) alternately.
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_BCM5461S_LED_BlinkSetMode(
	EDDP_HANDLE 	    hDDB,
	EDDP_SYS_HANDLE     hSysDev,	
	LSA_UINT32          HwPortID,
	LSA_UINT32		    LEDMode);

/*=============================================================================
 * function name:  EDDP_PHY_BCM5461S_LED_BlinkEnd
 *
 * function:       EDDP calls this function within the service
 *				   EDD_SRV_LED_BLINK() after blinking of LED(s) has finished. 
 *				   Can be used e.g. to re-enable the link/activity 
 *				   LED function of a PHY if necessary.
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_BCM5461S_LED_BlinkEnd(
	EDDP_HANDLE 	    hDDB,
	EDDP_SYS_HANDLE     hSysDev,	
	LSA_UINT32          HwPortID);

/*=============================================================================
 * function name:  EDDP_PHY_BCM5461S_LoadDelayValues
 *
 * function:       Load delay parameters depending on MediaType and hardware
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_BCM5461S_LoadDelayValues(
    EDDP_HANDLE                     hDDB,
    LSA_UINT32                      Hardware,
    EDDP_PHY_LINK_STATUS_PTR_TYPE   pLinkStatus);

/*=============================================================================
 * function name:  EDDP_PHY_BCM5461S_GetLinkStatus
 *
 * function:       Examine passed parameters in order to determine 
 *				   link, speed and duplexity and returns MAUType, MediaType, 
 *                 AutonegCapAdvertised, TxDelay, RxDelay, IsWireless and 
 *                 IsMDIX.
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_BCM5461S_GetLinkStatus(
	EDDP_HANDLE				        hDDB,
	EDDP_SYS_HANDLE			        hSysDev,
	LSA_UINT32						Hardware,
    LSA_UINT32				        HwPortID,
    EDDP_PHY_LINK_STATUS_PTR_TYPE   pLinkStatus);

/*=============================================================================
 * function name:  EDDP_PHY_BCM5461S_SetPowerDown
 *
 * function:       This function turns the PHY into software power down mode 
 *                 ON or OFF. When software power mode is OFF there will be 
 *                 no link and the PHY does not receive or send any data but 
 *                 can still be controlled via SMI.  				   
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_BCM5461S_SetPowerDown(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID, 
    LSA_UINT8               PowerDown);

/*=============================================================================
 * function name:  EDDP_PHY_BCM5461S_CheckPowerDown
 *
 * function:       If the PHY is PowerDown, this function returns LSA_TRUE.  
 *                 Otherwise it returns LSA_FALSE                                              
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_BCM5461S_CheckPowerDown(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID,
    LSA_UINT8*              pIsPowerDown);

/*=============================================================================
 * function name:  EDDP_PHY_BCM5461S_SetSpeedDuplexityOrAutoneg
 *
 * function:       This function sets speed mode and duplexity when the 
 *                 'Autoneg' parameter is set to LSA_FALSE. If 'Autoneg' is 
 *                 set to LSA_TRUE “Speed” and “Duplexity” parameters are 
 *                 irrelevant. In case the transceiver does not support 
 *                 auto-negotiation it has to set up a connection at least at 
 *                 100MBit full duplex.        
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_BCM5461S_SetSpeedDuplexityOrAutoneg(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID,    
    LSA_UINT8               *pLinkSpeedModeConfigured,
    LSA_UINT32              Speed,
    LSA_UINT32              Duplexity,
    LSA_UINT8               Autoneg);

/*=============================================================================
 * function name:  EDDP_PHY_BCM5461S_CheckMAUType
 *
 * function:       This function checks if the given MAUType is supported by 
 *                 the hardware and returns the related Speed and Duplexity.
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_BCM5461S_CheckMAUType(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID,
    LSA_UINT16              MAUType,
    LSA_UINT32              *pSpeed,
    LSA_UINT32              *pDuplexity);

/*=============================================================================
 * function name:  EDDP_PHY_BCM5461S_Get_Media_Type
 *
 *  function:      This function gives media and POF type wich be returned   
 *                 by user defined functions and saves them internaly.       
 *===========================================================================*/
LSA_VOID EDDP_SYSTEM_OUT_FCT_ATTR EDDP_PHY_BCM5461S_Get_Media_Type(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID,
    LSA_UINT8              *pMediaType,
    LSA_UINT8              *pOpticalTransType,
    LSA_UINT8              *FxTransceiverType);

/*****************************************************************************/
/*  end of file EDDP_PHY_BCM5461S.H                                               */
/*****************************************************************************/

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

/*---------------------------------------------------------------------------*/
#endif  /* of EDDP_PHY_BCM5461S_H */
