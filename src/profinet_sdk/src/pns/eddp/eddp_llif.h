/***********************************************************************************************************************
 * Copyright [2021-2024] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics Corporation and/or its affiliates and may only
 * be used with products of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.
 * Renesas products are sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for
 * the selection and use of Renesas products and Renesas assumes no liability.  No license, express or implied, to any
 * intellectual property right is granted by Renesas.  This software is protected under all applicable laws, including
 * copyright laws. Renesas reserves the right to change or discontinue this software and/or this documentation.
 * THE SOFTWARE AND DOCUMENTATION IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND
 * TO THE FULLEST EXTENT PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY,
 * INCLUDING WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE
 * SOFTWARE OR DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR
 * DOCUMENTATION (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER,
 * INCLUDING, WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY
 * LOST PROFITS, OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

#ifndef __EDDP_LLIF_H__                  /* ----- reinclude-protection ----- */
#define __EDDP_LLIF_H__

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

#include <stdint.h>
#include "r_ether_phy_rzt2.h"
/*=============================== Defines ===================================*/	

/*---------------------------------------------------------------------------*/
/* PHY specific parameters      											 */
/*---------------------------------------------------------------------------*/
#define EDDP_LL_PHY_OUI1REGADDRESS		            (LSA_UINT32)0x0002  // PHY Identifier 1
#define EDDP_LL_PHY_OUI1COMPAREVALUE		        (LSA_UINT32)0x0033  // OUI
#define EDDP_LL_PHY_OUI2REGADDRESS		            (LSA_UINT32)0x0003  // PHY Identifier 2
#define EDDP_LL_PHY_OUI2COMPAREVALUE		        (LSA_UINT32)0x2002  // OUI
#define EDDP_LL_PHY_LINKSTATUSREGADDRESS	        (LSA_UINT32)0x0001  // Basic Status register
#define EDDP_LL_PHY_LINKSTATUSBITMASK              	(LSA_UINT32)0x0004  // Link Status
#define EDDP_LL_PHY_SPEEDREGADDRESS	            	(LSA_UINT32)0x001F  // PHY Special Control/Status
#define EDDP_LL_PHY_SPEEDBITMASK			        (LSA_UINT32)0x0004  // Speed Indication:  (0:100MBit, 1:10MBit)           [???1.01?? = 10Mbps full-duplex, ???1.10?? = 100BASE-TX full-duplex]
#define EDDP_LL_PHY_DUPLEXITYREGADDRESS	        	(LSA_UINT32)0x001F  // PHY Special Control/Status
#define EDDP_LL_PHY_DUPLEXITYBITMASK		        (LSA_UINT32)0x0010  // Speed Indication:  (0:half duplex, 1:full duplex)  [???1.01?? = 10Mbps full-duplex, ???1.10?? = 100BASE-TX full-duplex]

/*---------------------------------------------------------------------------*/
/* PHY register number                                                       */
/*---------------------------------------------------------------------------*/
#define EDDP_LL_PHY_REG_BCMR                       (LSA_UINT32)0   // BasicModeControlRegister --> controls PowerDown and AutoNegotiation
#define EDDP_LL_PHY_REG_NEG_ADVERTISEMENT          (LSA_UINT32)4   // Auto-Negotiation Advertisement Register --> AutoNeg Cap.
#define EDDP_LL_PHY_REG_CONTROL                    (LSA_UINT32)17  // Mode Control/Status Register --> MDI-X
#define EDDP_LL_PHY_REG_SPECIAL_MODES              (LSA_UINT32)18  // Special Modes register       --> FX-Mode
//#define EDDP_LL_PHY_REG_LED_DIRECT_CONTROL         (LSA_UINT32)19  // Note: internal chip uses I/O lines over SCRB.PHY_LED_CONTROL
#define EDDP_LL_PHY_REG_BER_CNT                    (LSA_UINT32)23  // used for BER counter
#define EDDP_LL_PHY_REG_SPECIAL_CTL                (LSA_UINT32)31  // used for JK optimization

/*---------------------------------------------------------------------------*/
/* SHIM-Versatz                                                              */
/*---------------------------------------------------------------------------*/
#define EDDP_LL_PHY_TX_SHIM              0  // = (10 + 12) / 2
#define EDDP_LL_PHY_RX_SHIM              0  // = (10 + 12) / 2
#define EDDP_LL_PHY_TX_SHIM_FPGA                   15  // = (15 + 15) / 2
#define EDDP_LL_PHY_RX_SHIM_FPGA                   22  // = (20 + 24) / 2

  // delay betweeen arrival of frame and timestamp measurement
  // 40ns rx_cyc and 5 ns ts_cycl
#define SWITCH_RX_TS_DELAY  (3*40+1*8 + 0)         //Use shortest delay, if it is longer it will be added by the cable delay measurement and correction   
// DO NOT UISE #define SWITCH_TX_TS_DELAY  (1*8 + 8)        //  Note: This is also declared in r_ether_switch_rzt2.c due to compile issues  
#define GATE_MII_TX_DELAY 1000    // Delay between Gate and MII interface for the TX frame. 
#define SWITCH_TDMA_GATE_DELAY  53    // Delay(Typical) between TDMA trigger and actual opening of gate
#define SWITCH_TDMA_GATE_DELAY_MIN 51 // Delay(Minimum) between TDMA trigger and actual opening of gate
#define SWITCH_TDMA_RED_DELAY  41      // Delay between TDMA trigger and actual setting of RED 
  
/*---------------------------------------------------------------------------*/
/* PHY delay values         											     */
/*---------------------------------------------------------------------------*/
#ifdef UPD60620_PHY
#define EDDP_LL_PHY_PHY_TX_DELAY                   (90)          //  +200 just for test Ethercat Mode set on board by strap option 
#define EDDP_LL_PHY_PHY_RX_DELAY                   (170)           // RX_DV_J2T is set by strap option
  
#else
  //#define EDDP_LL_PHY_PHY_TX_DELAY                   397       // - SWITCH_TX_TS_DELAY                // Just for test Sum of both is 1030 including PORT delay
#define EDDP_LL_PHY_PHY_TX_DELAY                   (397+60)       //   Just for test Sum of both is 1030 including PORT delay
#define EDDP_LL_PHY_PHY_RX_DELAY                   450
#endif
  
#define EDDP_SWI_PTCP_TX_DELAY 0
#define EDDP_SWI_PTCP_RX_DELAY 0

/*---------------------------------------------------------------------------*/
/* Real Port delay values         											 */
/*---------------------------------------------------------------------------*/
#define EDDP_LL_PHY_REAL_PORT_TX_DELAY           (EDDP_LL_PHY_PHY_TX_DELAY + EDDP_SWI_PTCP_TX_DELAY - EDDP_LL_PHY_TX_SHIM) // 5
#define EDDP_LL_PHY_REAL_PORT_RX_DELAY           (EDDP_LL_PHY_PHY_RX_DELAY + EDDP_SWI_PTCP_RX_DELAY + EDDP_LL_PHY_RX_SHIM) // 197

// POF-Mode port delay values: ERTEC200P internal NEC PHY new (MII)
#define EDDP_LL_PHY_REAL_PORT_TX_DELAY_POF       1
#define EDDP_LL_PHY_REAL_PORT_RX_DELAY_POF       99
// POF-Mode port delay values: ERTEC200P internal NEC PHY new (MII)
#define EDDP_LL_PHY_REAL_PORT_TX_DELAY_POF_2     5
#define EDDP_LL_PHY_REAL_PORT_RX_DELAY_POF_2     103
// FX-Mode port delay values: ERTEC200P internal NEC PHY new (MII)
#define EDDP_LL_PHY_REAL_PORT_TX_DELAY_FX        2
#define EDDP_LL_PHY_REAL_PORT_RX_DELAY_FX        100

/*---------------------------------------------------------------------------*/
/* Max Port delay values         											 */
/*---------------------------------------------------------------------------*/
#define EDDP_LL_PHY_MAX_PORT_TX_DELAY            6 
#define EDDP_LL_PHY_MAX_PORT_RX_DELAY            198 
#define EDDP_LL_PHY_MAX_PORT_TX_DELAY_FPGA                 1
#define EDDP_LL_PHY_MAX_PORT_RX_DELAY_FPGA                 210
// POF-Mode port delay values: ERTEC200P internal NEC PHY new (MII), EDD_FX_TRANSCEIVER_QFBR5978
#define EDDP_LL_PHY_MAX_PORT_TX_DELAY_POF        2
#define EDDP_LL_PHY_MAX_PORT_RX_DELAY_POF        100
// POF-Mode port delay values: ERTEC200P internal NEC PHY new (MII), EDD_FX_TRANSCEIVER_QFBR5978_2
#define EDDP_LL_PHY_MAX_PORT_TX_DELAY_POF_2      8
#define EDDP_LL_PHY_MAX_PORT_RX_DELAY_POF_2      107
// FX-Mode port delay values: ERTEC200P internal NEC PHY new (MII), EDD_FX_TRANSCEIVER_AFBR59E4APZ
#define EDDP_LL_PHY_MAX_PORT_TX_DELAY_FX         4
#define EDDP_LL_PHY_MAX_PORT_RX_DELAY_FX         102

/*---------------------------------------------------------------------------*/
/* Software Filter Configration												 */
/*---------------------------------------------------------------------------*/
#define EDDP_LL_SWFILTER_ETHTYPE_IPV4            0x0800 // IPV4
#define EDDP_LL_SWFILTER_PERIOD_IPV4             10     // Filter period(millisecond)
#define EDDP_LL_SWFILTER_LIMIT_IPV4              100    // Frame reception limit per EDDP_LL_SWFILTER_PERIOD_IPV4



#define ETH_PORT_1 1
#define ETH_PORT_2 2

#define VAILD_CYCLECOUNTER_LIMIT 0xF000

/*============================== Prototypes =================================*/
	void ether_switch_enable_tdma0_icu (void);
	void ether_switch_enable_tdma1_icu (void);
	void ether_switch_enable_tdma2_icu (void);
	void ether_switch_enable_tdma3_icu (void);

	void setup_pattern_matcher(uint8_t portmask);
	void ETHSW_DisableIrqTdmaAll(void);
	uint32_t ethsw_get_time_sec(uint8_t timer);
	void ETHSW_InitSwitchTimer(void);
	
	void ether_switch_enable_tx_timestamp_icu (uint8_t portmask);
	uint32_t ether_phy_read (ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr);
	void ethsw_disable_pattern_matcher(uint8_t portmask, uint32_t pattern_mask);
	void ethsw_enable_pattern_matcher(uint8_t portmask, uint32_t pattern_mask);

/*=============================================================================
 * function name:  EDDP_LL_PHY_Init
 *
 * function:       This function shall return all parameters of a specific PHY
 *                 needed for link change information. This includes PHY 
 *                 register addresses and bit masks for link status, speed mode
 *                 and duplexity.  
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_LL_PHY_Init(
	EDDP_HANDLE				    hDDB,
	EDDP_SYS_HANDLE			    hSysDev,
	LSA_UINT32                  Hardware,
    LSA_UINT32				    HwPortID,
    LSA_UINT8  	                IsWireless,
    LSA_UINT8  	                IsMIDX,
	EDDP_PHY_INIT_PTR_TYPE      pInitData);

/*=============================================================================
 * function name:  EDDP_LL_PHY_LED_BlinkBegin
 *
 * function:       EDDP calls this function within the service 
 *				   EDD_SRV_LED_BLINK() before the LED(s) start to blink.
 *				   Can be used e.g. to diable the link/activity LED function 
 *			       of a PHY if necessary.  
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_LL_PHY_LED_BlinkBegin(
	EDDP_HANDLE 	    hDDB,
	EDDP_SYS_HANDLE     hSysDev,	
	LSA_UINT32          HwPortID);

/*=============================================================================
 * function name:  EDDP_LL_PHY_LED_BlinkSetMode
 *
 * function:       EDDP calls this function repeatedly within the service 
 *				   EDD_SRV_LED_BLINK() in order to turn on and turn off 
 *				   the LED(s) alternately.
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_LL_PHY_LED_BlinkSetMode(
	EDDP_HANDLE 	    hDDB,
	EDDP_SYS_HANDLE     hSysDev,	
	LSA_UINT32          HwPortID,
	LSA_UINT32		    LEDMode);

/*=============================================================================
 * function name:  EDDP_LL_PHY_LED_BlinkEnd
 *
 * function:       EDDP calls this function within the service
 *				   EDD_SRV_LED_BLINK() after blinking of LED(s) has finished. 
 *				   Can be used e.g. to re-enable the link/activity 
 *				   LED function of a PHY if necessary.
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_LL_PHY_LED_BlinkEnd(
	EDDP_HANDLE 	    hDDB,
	EDDP_SYS_HANDLE     hSysDev,	
	LSA_UINT32          HwPortID);

/*=============================================================================
 * function name:  EDDP_LL_PHY_LoadDelayValues
 *
 * function:       Load delay parameters depending on MediaType and hardware
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_LL_PHY_LoadDelayValues(
	EDDP_HANDLE				        hDDB,
	LSA_UINT32						Hardware,
    EDDP_PHY_LINK_STATUS_PTR_TYPE   pLinkStatus);

    /*=============================================================================
 * function name:  EDDP_LL_PHY_GetLinkStatus
 *
 * function:       Examine passed parameters in order to determine 
 *				   link, speed and duplexity and returns MAUType, MediaType, 
 *                 AutonegCapAdvertised, TxDelay, RxDelay, IsWireless and 
 *                 IsMDIX.
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_LL_PHY_GetLinkStatus(
	EDDP_HANDLE				        hDDB,
	EDDP_SYS_HANDLE			        hSysDev,
	LSA_UINT32						Hardware,
    LSA_UINT32				        HwPortID,
    EDDP_PHY_LINK_STATUS_PTR_TYPE   pLinkStatus);

/*=============================================================================
 * function name:  EDDP_LL_PHY_SetPowerDown
 *
 * function:       This function turns the PHY into software power down mode 
 *                 ON or OFF. When software power mode is OFF there will be 
 *                 no link and the PHY does not receive or send any data but 
 *                 can still be controlled via SMI.  				   
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_LL_PHY_SetPowerDown(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID, 
    LSA_UINT8               PowerDown);

/*=============================================================================
 * function name:  EDDP_LL_PHY_CheckPowerDown
 *
 * function:       If the PHY is PowerDown, this function returns LSA_TRUE.  
 *                 Otherwise it returns LSA_FALSE                              				   
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_LL_PHY_CheckPowerDown(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID,
    LSA_UINT8*              pIsPowerDown);

/*=============================================================================
 * function name:  EDDP_LL_PHY_SetSpeedDuplexityOrAutoneg
 *
 * function:       This function sets speed mode and duplexity when the 
 *                 'Autoneg' parameter is set to LSA_FALSE. If 'Autoneg' is 
 *                 set to LSA_TRUE �Speed� and �Duplexity� parameters are 
 *                 irrelevant. In case the transceiver does not support 
 *                 auto-negotiation it has to set up a connection at least at 
 *                 100MBit full duplex.        
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_LL_PHY_SetSpeedDuplexityOrAutoneg(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID,    
    LSA_UINT8               *pLinkSpeedModeConfigured,
    LSA_UINT32              Speed,
    LSA_UINT32              Duplexity,
    LSA_UINT8               Autoneg);

/*=============================================================================
 * function name:  EDDP_LL_PHY_CheckMAUType
 *
 * function:       This function checks if the given MAUType is supported by 
 *                 the hardware and returns the related Speed and Duplexity.
 *===========================================================================*/
EDD_RSP EDDP_SYSTEM_OUT_FCT_ATTR EDDP_LL_PHY_CheckMAUType(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID,
    LSA_UINT16              MAUType,
    LSA_UINT32              *pSpeed,
    LSA_UINT32              *pDuplexity);

/*=============================================================================
 * function name:  EDDP_LL_PHY_Get_Media_Type
 *
 *  function:      This function gives media and POF type wich be returned   
 *                 by user defined functions and saves them internaly.       
 *===========================================================================*/
LSA_VOID EDDP_SYSTEM_OUT_FCT_ATTR EDDP_LL_PHY_Get_Media_Type(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID,
    LSA_UINT8              *pMediaType,
    LSA_UINT8              *pOpticalTransType,
    LSA_UINT8              *pFxTransceiverType);

#if defined EDDP_CFG_DUMP_RZT2_REGS_ON_LINKDOWN
/*=============================================================================
* function name:  EDDP_LL_PHY_DumpRegs
*
*===========================================================================*/
LSA_VOID EDDP_SYSTEM_OUT_FCT_ATTR EDDP_LL_PHY_DumpRegs(
    EDDP_HANDLE				    hDDB,
    LSA_UINT32				    HwPortID);
#endif //EDDP_CFG_DUMP_RZT2_REGS_ON_LINKDOWN


LSA_VOID EDDP_SYSTEM_OUT_FCT_ATTR EDDP_LL_PHY_Get_MAU_Type(
    EDDP_HANDLE             hDDB,
    EDDP_SYS_HANDLE         hSysDev,
    LSA_UINT32              HwPortID,
    LSA_UINT32              Speed,
    LSA_UINT32              Duplexity,
    LSA_UINT16             *pMauType);


extern EDD_RSP EDDP_LL_SET_LINK(LSA_UINT32 InterfaceID, LSA_UINT16 PortID, LSA_UINT8 LinkCfg);
extern LSA_UINT32 eddp_ll_PhyRegRead(LSA_UINT32 InterfaceID, LSA_UINT16 PortID, LSA_UINT32 Reg);

LSA_VOID EDDP_LL_ENTER(LSA_VOID);
LSA_VOID EDDP_LL_EXIT(LSA_VOID);

//LSA_VOID_PTR_TYPE  EDDP_LL_MNG_HANDLE(LSA_UINT16 InterfaceID);
//LSA_VOID_PTR_TYPE  EDDP_LL_GetIfHandle(LSA_UINT16 InterfaceID);

EDD_RSP EDDP_LL_CoreRequest(EDD_UPPER_RQB_PTR_TYPE pRQB, EDDP_LOCAL_HDB_PTR_TYPE pHDB);
EDD_RSP  EDDP_LL_SWIDeviceOpen(EDDP_LOCAL_DDB_PTR_TYPE pDDB, EDDP_UPPER_DPB_PTR_TYPE pDPB);
EDD_RSP  EDDP_LL_SWIDeviceSetup1(EDDP_LOCAL_DDB_PTR_TYPE pDDB, EDDP_UPPER_DSB_PTR_TYPE pDSB);
EDD_RSP  EDDP_LL_GET_LINK_STATUS(LSA_UINT32 InterfaceID, LSA_UINT16 PortID, LSA_UINT32 *pLink, LSA_UINT32 *pSpeed, LSA_UINT32 *pDuplexity);
LSA_VOID EDDP_LL_GetPortStatistics(EDDP_LOCAL_DDB_PTR_TYPE pDDB, EDDP_LOCAL_GET_STATISTICS_PTR_TYPE pStats, LSA_UINT32 HwPortID);
EDD_RSP  EDDP_LL_CmdRequest(EDDP_LOCAL_DDB_PTR_TYPE pDDB, EDDP_CMD_PARAM_PTR_TYPE pParam);
LSA_VOID_PTR_TYPE EDDP_LL_CmdRequestPrm(EDDP_LOCAL_DDB_PTR_TYPE pDDB);

LSA_RESULT EDDP_LL_TX_BUFF_AVAILABLE(LSA_UINT32 InterfaceID);
LSA_RESULT EDDP_LL_NRT_SEND( LSA_UINT32 InterfaceID, EDD_UPPER_MEM_PTR_TYPE  pBufferAddr, LSA_UINT32 Length, LSA_UINT32 PortID, LSA_VOID* hUpper);
LSA_VOID   EDDP_LL_NRT_SEND_DONE(EDDP_LOCAL_DDB_PTR_TYPE pDDB);
LSA_VOID   EDDP_LL_SEND_TRIGGER(LSA_UINT32 InterfaceID);
LSA_VOID   EDDP_LL_NRT_RECV_DONE(EDDP_LOCAL_DDB_PTR_TYPE pDDB);
LSA_VOID   EDDP_LL_LINK_CHANGE_DONE(EDDP_LOCAL_DDB_PTR_TYPE  pDDB);

/*=============================================================================
 * function name:  EDDP_LL_ConsumerFrameValidityCheck
 *
 * function:       This function checks the validity of the RTC frame. 
 *                 If valid, it returns ConsumerID.
 *===========================================================================*/
LSA_UINT16 EDDP_LL_ConsumerFrameValidityCheck(
    EDDP_LOCAL_DDB_PTR_TYPE , 
    LSA_UINT8* , 
    LSA_UINT8* , 
    LSA_UINT32 ,
    LSA_BOOL);

/*=============================================================================
 * function name:  EDDP_LL_ConsumerDhtExpireCheck
 *
 * function:       This function checks for DHT expiration.
 *                 If expired, notify scoreboard handler.
 *===========================================================================*/
LSA_VOID EDDP_LL_ConsumerDhtExpireCheck(
    LSA_UINT32);


/*=============================================================================
 * function name:  EDDP_LL_SetCurrrentConsumerScoreboard
 *
 * function:       This function set for current consmer scoreboard.
 *
 *===========================================================================*/
LSA_VOID EDDP_LL_SetCurrrentConsumerScoreboard(
    EDDP_LOCAL_DDB_PTR_TYPE ,
    LSA_UINT16 ,
    LSA_UINT32);

/*=============================================================================
 * function name:  EDDP_LL_ConsumerFrameApduCheck
 *
 * function:       This function checks for Transfer and DataStatus of RT-Frame.
 *                 If invalide, notify scoreboard handler.
 *===========================================================================*/
LSA_UINT32 EDDP_LL_ConsumerFrameApduCheck(LSA_UINT8* pRxBuffer, LSA_UINT32 RxLength, EDDP_CRT_CONSUMER_PTR_TYPE pConsumer, LSA_UINT32 *CSBValue);

/*=============================================================================
 * function name:  start_profinet
 *
 * function:       initialize for PROFINET
 *
 *===========================================================================*/
void start_profinet(EDDP_LOCAL_DDB_PTR_TYPE pDDB);

/*=============================================================================
 * function name:  setup_profinet
 *
 * function:       Configure scheduler and filter settings for PROFINET
 *
 *===========================================================================*/
void setup_profinet(EDDP_LOCAL_DDB_PTR_TYPE pDDB);

/*=============================================================================
 * function name:  EDDP_LL_EhterTypeFilter
 *
 * function:       InitInitialization of software filters
 *
 *===========================================================================*/
LSA_VOID EDDP_LL_EhterTypeFilterInit(LSA_VOID);

/*=============================================================================
 * function name:  EDDP_LL_AddEtherTypeFilter
 *
 * function:       Adding software filers for EtherType
 *
 *===========================================================================*/
EDD_RSP EDDP_LL_AddEtherTypeFilter(
    LSA_UINT16 EtherType,
    LSA_UINT16 Period,
    LSA_UINT32 FrameLimit);

/*=============================================================================
 * function name:  EDDP_LL_DelEtherTypeFilter
 *
 * function:       Delete  sofrware filters for EtherType
 *
 *===========================================================================*/
EDD_RSP  EDDP_LL_DelEtherTypeFilter(
    LSA_UINT16 EtherType);

/*=============================================================================
 * function name:  EDDP_LL_EtherTypeFilter
 *
 * function:       Determine if filtering is required
 *
 *===========================================================================*/
EDD_RSP EDDP_LL_EtherTypeFilter(
    LSA_UINT16 EtherType);

/*=============================================================================
 * function name:  EDDP_LL_EtherTypeFilterTimerCoallback
 *
 * function:       Periodic reloading of counters for filtering
 *
 *===========================================================================*/
LSA_VOID EDDP_LL_EtherTypeFilterTimerCoallback(
    LSA_UINT16 timer_id,
    LSA_UINT32 user_id);

#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif  /* __EDDP_LLIF_H__ */
