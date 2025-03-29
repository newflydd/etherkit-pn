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
/*  C o m p o n e n t     &C: PnIODDevkits                              :C&  */
/*                                                                           */
/*  F i l e               &F: bspadapt.h                                :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Interface between EDD system adaption and the                            */
/*  OS-specific board support package                                        */
/*                                                                           */
/*****************************************************************************/
#ifndef _BSPADAPT_H
#define _BSPADAPT_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif


#include "iod_cfg2.h"

#if (PNIOD_PLATFORM & (PNIOD_PLATFORM_EB200P | PNIOD_PLATFORM_RZT2_EDDP) )
    #include "evma_inc.h"
#endif


PNIO_UINT32 BspAdapt_Open  (PNIO_UINT32 TxBufCnt,
                            PNIO_UINT32 RxBufCnt,
                            PNIO_UINT32 RxBufSize,
                            PNIO_UINT32 RxBufOffs);


LSA_BOOL Bsp_SetIpSuite (LSA_UINT32 interface_id, LSA_UINT32 ip_addr, LSA_UINT32 net_mask, LSA_UINT32 gateway );
// *** get device mac address and port mac addresses ****
PNIO_UINT32 Bsp_GetMacAddr		 (PNIO_UINT8* pDevMacAddr);                          // get device mac address
PNIO_UINT32 Bsp_GetPortMacAddr   (PNIO_UINT8* pPortMacAddr, PNIO_UINT32 PortNum);    // get port mac address
PNIO_UINT32 Bsp_Init  (void);

void        Bsp_PofInit     (void);

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200)
void 		Bsp_PofSclDir 	(PNIO_UINT8 portId, PNIO_UINT8 setInput);
void 		Bsp_PofSdaDir 	(PNIO_UINT8 portId, PNIO_UINT8 setInput);
PNIO_UINT8 	Bsp_PofSdaRead 	(PNIO_UINT8 portId);
#endif

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EBx00)

	// *** connect ertec switch interrupt ****
	typedef  void    (*PNIO_CBF_ERTEC_SWI_INT_H)       (PNIO_UINT32 Dummy);
    typedef  void    (*PNIO_CBF_ERTEC_SWI_INT_L)       (PNIO_UINT32 Dummy);
    typedef  void    (*PNIO_CBF_ERTEC_TIM1_INT)        (PNIO_UINT32 Dummy);
    typedef  void    (*PNIO_CBF_ERTEC_PARITY_ERR_INT)  (PNIO_UINT32 Dummy);
    typedef  void    (*PNIO_CBF_ERTEC_ACCESS_ERR_INT)  (PNIO_UINT32 Dummy);

    PNIO_UINT32 Bsp_ErtecSwiIntConnect  (PNIO_CBF_ERTEC_SWI_INT_H   ErtecSwiIntH,
                                         PNIO_CBF_ERTEC_SWI_INT_L   ErtecSwiIntL);
    PNIO_UINT32 Bsp_ErtecErrIntConnect  (PNIO_CBF_ERTEC_PARITY_ERR_INT   pErtecParityErrInt,
    									 PNIO_CBF_ERTEC_ACCESS_ERR_INT   pErtecAccessErrInt);
    PNIO_UINT32 Bsp_ErtecTim1IntConnect (PNIO_CBF_ERTEC_TIM1_INT    pErtecTim1Int);
    PNIO_UINT32 Bsp_ErtecTim1Start      (PNIO_UINT32                ErtecTim1PeriodUs);

    PNIO_VOID   Bsp_Timer1Int           (PNIO_UINT32                int_source);

    void Bsp_EVMA_register_ISR(PNIO_UINT32 vector, void (*isr)(void));
    void Bsp_EVMA_unregister_ISR(PNIO_UINT32 vector);
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_RZT2_EDDP)

	// *** connect ertec switch interrupt ****
	typedef  void    (*PNIO_CBF_ERTEC_SWI_INT_H)       (PNIO_UINT32 Dummy);
    typedef  void    (*PNIO_CBF_ERTEC_SWI_INT_L)       (PNIO_UINT32 Dummy);
    typedef  void    (*PNIO_CBF_ERTEC_TIM1_INT)        (PNIO_UINT32 Dummy);
    typedef  void    (*PNIO_CBF_ERTEC_PARITY_ERR_INT)  (PNIO_UINT32 Dummy);
    typedef  void    (*PNIO_CBF_ERTEC_ACCESS_ERR_INT)  (PNIO_UINT32 Dummy);

    PNIO_UINT32 Bsp_SwiIntConnect  (PNIO_VOID*   pfSwiIntH, PNIO_VOID*   pfSwiIntL);
    PNIO_UINT32 Bsp_ErrIntConnect  (PNIO_VOID*   pfParityErrInt, PNIO_VOID*   pfAccessErrInt);
    PNIO_UINT32 Bsp_Tim1IntConnect (PNIO_VOID*   pfTim1Int);
    PNIO_UINT32 Bsp_Tim1Start      (PNIO_UINT32  Tim1PeriodUs);

    PNIO_VOID   Bsp_Timer1Int           (PNIO_UINT32                int_source);

    void Bsp_EVMA_register_ISR(PNIO_UINT32 vector, void (*isr)(void));
    void Bsp_EVMA_unregister_ISR(PNIO_UINT32 vector);

    void Bsp_EVMA_enable_ISR(PNIO_UINT32 vector);
    void Bsp_EVMA_disable_ISR(PNIO_UINT32 vector);
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_DKSW)

    #ifndef LSA_RET_OK
     /*=====  function return-values  =====*/
     #define LSA_RET_OK                      LSA_OK /* without errors */
     #define LSA_RET_OK_TIMER_RESTARTED      0x0a   /* timer has been restarted */
     #define LSA_RET_OK_TIMER_NOT_RUNNING    0x0b   /* timer was not running; */
                                                    /* timer is stopped       */
     #define LSA_RET_ERR_RESSOURCE           0x84   /* no resources */
     #define LSA_RET_ERR_PARAM               0x86   /* parameter is wrong */
     #define LSA_RET_ERR_SEQUENCE            0x87   /* wrong sequence */
     #define LSA_RET_ERR_SYS_PATH            0x91   /* wrong system-path */
     #define LSA_RET_ERR_NO_TIMER            0xa0   /* no timer has been allocated*/
     #define LSA_RET_ERR_TIMER_IS_RUNNING    0xa1   /* because timer is running */
    #endif



    PNIO_UINT32 Bsp_MC_Enable  (void* pMACAddr);
    PNIO_UINT32 Bsp_MC_Disable (void* pMACAddr);
    PNIO_UINT32 Bsp_ReinitTransmitUnit(void);

    PNIO_UINT32 Bsp_SendData   (void*       phDDB,
    							void*       pTxBuf,
                                PNIO_UINT32 Datlen,
                                PNIO_UINT32 PortID);
	PNIO_UINT32 Bsp_RcvData    ( PNIO_UINT32    PortId,
                                 void*          pRxBuf,
                                 PNIO_UINT32*   pRxSize);
	void BspSigSegFaultInit ( void );
    // *------------------------------------------
    // *     PDEV (Physical Device)
    // *------------------------------------------
    // *** Link State values ***
    #define PDEV_LINK_STATE_UP       1
    #define PDEV_LINK_STATE_DOWN     2

    // *** PHY Power values ***
    #define PDEV_PHY_POWER_UNCHANGED 0
    #define PDEV_PHY_POWER_ON        1
    #define PDEV_PHY_POWER_OFF       2

    // *** Link Mode values ***
    #define PDEV_LINK_AUTONEG        0
    #define PDEV_LINK_100MBIT_HALF   1
    #define PDEV_LINK_100MBIT_FULL   2
    #define PDEV_LINK_10MBIT_HALF    3
    #define PDEV_LINK_10MBIT_FULL    4
    #define PDEV_LINK_1GBIT_HALF     5
    #define PDEV_LINK_1GBIT_FULL     6

    // *** Phy Type values (if you don't know, set to UNKNOWN) ***
    #define PDEV_PHY_TYPE_UNKNOWN      0   /* unknown PhyType                     */
    #define PDEV_PHY_TYPE_BASE5        1   /* thick coax MAU                      */
    #define PDEV_PHY_TYPE_BASE2        2   /* thin coax MAU                       */
    #define PDEV_PHY_TYPE_BASET        3   /* UTP MAU                             */
    #define PDEV_PHY_TYPE_BASEFP       4   /* passive fiber MAU                   */
    #define PDEV_PHY_TYPE_BASEFB       5   /* sync fiber MAU                      */
    #define PDEV_PHY_TYPE_BASEFL       6   /* async fiber MAU                     */
    #define PDEV_PHY_TYPE_BROAD36      7   /* broadband DTE MAU                   */
    #define PDEV_PHY_TYPE_BASET4       8   /* 4 pair categ. 3 UTP                 */
    #define PDEV_PHY_TYPE_BASETX       9   /* 2 pair categ. 5 UTP                 */
    #define PDEV_PHY_TYPE_BASEFX      10   /* X fiber over PMT                    */
    #define PDEV_PHY_TYPE_BASET2      11   /* 2 pair categ. 3 UTP                 */
    #define PDEV_PHY_TYPE_BASEX       12   /* PCS/PMA                             */
    #define PDEV_PHY_TYPE_BASELX      13   /* Fiber over long-wavelength laser    */
    #define PDEV_PHY_TYPE_BASESX      14   /* Fiber over short-wavelength laserr  */
    #define PDEV_PHY_TYPE_BASECX      15   /* Copper over 150-Ohm balanced cable  */
    #define PDEV_PHY_TYPE_AUI         16   /* no internal MAU,view from AUI       */
    #define PDEV_PHY_TYPE_BASEPX      17   /* Fiber over POF                      */
    #define PDEV_PHY_TYPE_BASELX10    18   /* Two fiber 10km                      */


	#define	PDEV_NO_DATA				0
	#define PDEV_FILTERED_DATA			1
	#define PDEV_VALID_DATA				2

    PNIO_UINT32 Bsp_GetPDevLinkState  (PNIO_UINT32  Port, PNIO_UINT8* pLinkState);
    PNIO_UINT32 Bsp_SetPDevLinkMode   (PNIO_UINT32 Port,
                                       PNIO_UINT8  LinkMode,
                                       PNIO_UINT8  PhyPower);
    PNIO_UINT32 Bsp_GetPDevLinkMode   (PNIO_UINT32  Port,
                                       PNIO_UINT8*  pLinkMode,
                                       PNIO_UINT8*  pPhyType);

    // *--------------------------------------------------------
    // * SWITCH
    // * only for switch adapted to standard ethernet controller
    // *--------------------------------------------------------
    #define BSP_PORT_STATE_BLOCKING     EDD_PORT_STATE_BLOCKING
    #define BSP_PORT_STATE_FORWARD      EDD_PORT_STATE_FORWARDING

    #if ((IOD_CFG_PDEV_NUMOF_PORTS > 1) && (PNIOD_PLATFORM &  PNIOD_PLATFORM_DKSW))

        // **** switch features ****/
        PNIO_UINT32 Bsp_SetPortState        (PNIO_UINT32 PortNum, PNIO_UINT16 PortState);
        PNIO_UINT32 Bsp_GetPortMode         (PNIO_UINT32 Port);
        PNIO_UINT32 Bsp_ChangePort      ( LSA_UINT16 PortID, LSA_UINT8 IsPulled);
        PNIO_UINT32 Bsp_ClearFDB            (void);     // clear all entries in filter data base

	    #define BSP_BRIDGE_MODE_SF	0	// store & forward mode
	    #define BSP_BRIDGE_MODE_CT	1	// cut through mode
	    extern PNIO_UINT32 Bsp_SetBridgeMode  (PNIO_UINT32 Mode);

        extern PNIO_UINT16  Bsp_SwitchMcForwControl(
                            PNIO_UINT32   PortNum,
                            PNIO_UINT16   Mode,
                            PNIO_UINT16   MACAddrPrio,
                            PNIO_UINT16   MACAddrGroup,
                            PNIO_UINT32   MACAddrLow);


	    #define BSP_LED_MODE_SPEED      0x0 /* display 10/100 Mbit - speed status */
	    #define BSP_LED_MODE_TX         0x1 /* transmit status                    */
	    #define BSP_LED_MODE_RX         0x2 /* receive  status                    */
	    #define BSP_LED_MODE_COLLISION  0x3 /* collision occured                  */
	    #define BSP_LED_MODE_LINK       0x4 /* link up/down status     (up = on)  */
	    #define BSP_LED_MODE_DUPLEX     0x5 /* full/half-duplex status (full=on)  */
	    #define BSP_LED_MODE_RX_TX      0x7 /* transmit or receive status         */
	    #define BSP_LED_MODE_ON         0x8 /* turn led on                        */
	    #define BSP_LED_MODE_OFF        0x9 /* turn led off                       */
	    #define BSP_LED_MODE_BLINK      0xA /* led blinks                         */
	    extern PNIO_UINT32 Bsp_GetLedMode (
							    PNIO_UINT32 PortID,
							    PNIO_UINT32 LEDNo,
							    PNIO_UINT32 *pLEDMode);
	    extern PNIO_UINT32 Bsp_SetLedMode (
							    PNIO_UINT32 PortID,
							    PNIO_UINT32 LEDNo,
							    PNIO_UINT32 LEDMode);
    #endif // switch on standard ethernet platform

#endif

// **** LED handling on the evaluation board ***
typedef enum
{ // LED
	PNIO_LED_RUN = 1,
	PNIO_LED_MAINT,
	PNIO_LED_ERROR,
	PNIO_LED_SYNC,
	PNIO_LED_LINKP1,
	PNIO_LED_LINKP2,
	PNIO_LED_LINKP3,
	PNIO_LED_LINKP4,
	PNIO_LED_ACTIVP1,
	PNIO_LED_ACTIVP2,
	PNIO_LED_ACTIVP3,
	PNIO_LED_ACTIVP4,
	PNIO_LED_FO1,
	PNIO_LED_FO2,
	PNIO_LED_POWER,
	PNIO_LED_DIAG,
	PNIO_LED_PENERGY,
	PNIO_LED_BLINK,
	PNIO_LED_USER_00,
	PNIO_LED_USER_01,
	PNIO_LED_USER_02,
	PNIO_LED_USER_03,
	PNIO_LED_USER_04,
	PNIO_LED_USER_05,
	PNIO_LED_USER_06,
	PNIO_LED_USER_07,
	PNIO_LED_USER_08,
	PNIO_LED_USER_09,
	PNIO_LED_USER_10,
	PNIO_LED_USER_11,
	PNIO_LED_USER_12,
	PNIO_LED_USER_13,
	PNIO_LED_USER_14,
	PNIO_LED_USER_15,
	PNIO_LED_NUMOF_ENTRIES
#ifndef PNIO_LEDTYPE_USE_ENUM_TYPE
} PNIO_LEDTYPE_ENUM;
#define PNIO_LEDTYPE	PNIO_UINT32
#else
} PNIO_LEDTYPE;
#endif

#define BSP_LED_CYCL_CTRL	    PNIO_LED_USER_00
#define BSP_LED_FAULT		    PNIO_LED_USER_01
#define BSP_LED_WARNING		    PNIO_LED_USER_02
#define BSP_LED_MOD_RUN	    	PNIO_LED_USER_03
#define BSP_LED_MOD_ERR	    	PNIO_LED_USER_04
#define BSP_LED_BUS_RUN	    	PNIO_LED_USER_05
#define BSP_LED_BUS_ERR	    	PNIO_LED_USER_06
#define BSP_LED_1	    		PNIO_LED_USER_07
#define BSP_LED_2	    		PNIO_LED_USER_08
#define BSP_LED_3	    		PNIO_LED_USER_09
#define BSP_LED_BLINK_START	    PNIO_LED_USER_10
#define BSP_LED_BLINK_STOP	    PNIO_LED_USER_11
#define BSP_LED_GROUP		    PNIO_LED_USER_12

PNIO_VOID   BspLed_Open           ( void );
PNIO_VOID   BspLed_StartLedBlink  ( PNIO_UINT32 frequency );
PNIO_VOID   BspLed_StopLedBlink   ( void );
PNIO_UINT32 Bsp_EbSetLed          ( PNIO_LEDTYPE Led, PNIO_UINT32 Val );
PNIO_UINT32 RZT2_PNS_GetLedStat (int Led);

typedef enum
{
    GPIO_DIR_IN        =   00,
    GPIO_DIR_OUT       =   01
} PNIO_GPIO_DIR;

PNIO_UINT32 Bsp_SetGpioMode         ( PNIO_UINT32    Gpio,
                                      PNIO_UINT32    Function,
                                      PNIO_GPIO_DIR  InOut );

PNIO_VOID   Bsp_ClearGPIOout_0_to_31 ( PNIO_UINT32 OutMsk );
PNIO_VOID   Bsp_SetGPIOout_0_to_31   ( PNIO_UINT32 OutMsk );


PNIO_UINT32 Bsp_ReadGPIOin_0_to_31  ( void );

typedef enum BSP_WD_GRANITY
{
	BSP_WD_100MS,
	BSP_WD_10MS,
	BSP_WD_1MS,
	BSP_WD_100US
}BSP_WD_GRANITY;

void 		Bsp_hw_watchdog_init	( PNIO_UINT32 wd_time, BSP_WD_GRANITY wd_granity );
void 		Bsp_hw_watchdog_deinit	( void );
void 		Bsp_hw_watchdog_start	( void );
void 		Bsp_hw_watchdog_stop	( void );
void 		Bsp_hw_watchdog_trigger	( void );

PNIO_VOID   Bsp_GPIO_set_int_for_acyc(PNIO_UINT32 int_vector, PNIO_VOID* gpio_isr, PNIO_VOID* gpio_dsr);
PNIO_VOID   Bsp_GPIO_set_int_for_acyc_confirm(PNIO_UINT32 int_vector, PNIO_VOID* gpio_isr, PNIO_VOID* gpio_dsr);
PNIO_VOID   Bsp_GPIO_isr(PNIO_UINT32 int_vector);



#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif

#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
