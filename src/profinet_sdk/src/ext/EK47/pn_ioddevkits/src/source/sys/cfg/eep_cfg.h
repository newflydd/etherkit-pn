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
/*  F i l e               &F: eep_cfg.h                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  This file contains the configuration of the EEPROM driver.               */
/*                                                                           */
/*****************************************************************************/

/* **************************************************************************
*
*  H i s t o r y :
*
*  date     person  comment
*  -------- ------  -------
*  27.10.05 MaSi    File new - everything used to be in e\e_dat.h
*  14.12.06 MaSi    Conditionally test INIT string for identical versions
*  01.06.07 MaSi    - INT_SPERRE divided into INT_SPERRE_RD and INT_SPERRE_WR
*                   - Test: Read without interrupt lock
*  09.11.07 MaSi    IOL supplements
*  27.01.09 JaGa    all_cfg.h is included
*
*
* ****************************************************************************/

#ifndef E_CFG_H
  #define E_CFG_H

/* Cross-component settings. */


#ifdef TMS470_IAR
  #include "a_out.h"  /* For function a_time_wait_us (x) from the CPU driver. */
#endif


/* EEPROM-Typ
------------------------------------------------------------------------- */
// #define EEPROM_24C01
// #define EEPROM_24C02
// #define EEPROM_24C04
// #define EEPROM_24C08
//#define EEPROM_24C16
//#define EEPROM_24C32
// #define EEPROM_24C64
// #define EEPROM_24C128
// #define EEPROM_24C256
#define EEPROM_24C512


/* Proz-Typ
------------------------------------------------------------------------- */
// #define MSP430_IAR
// #define TMS470_IAR
//#define ERTEC_GHS
// #define NEC850_IAR
#define ATMEL_SAM9X25


/* Attributes
------------------------------------------------------------------------- */

#define P_DAT_DIA_ATTR
#define E_DAT_ALLG_ATTR


/* Constants
------------------------------------------------------------------------- */

typedef Unsigned8  UBYTE;
typedef Unsigned16 UWORD;
typedef Unsigned32 UDWORD;
typedef Unsigned32 ULONG;

#define EEP_LITTLE_ENDIAN


#define OK      TRUE

#define MAX_LEN_I_A_SZL     64                  // Max. Length that occurs at I & A-SZL's


/* internal test functions -> only during development! */
// #define EEPROM_SONDER_TEST

/* individual bits
------------------------------------------------------------------------- */
#define BIT0  0x01
#define BIT1  0x02
#define BIT2  0x04
#define BIT3  0x08
#define BIT4  0x10
#define BIT5  0x20
#define BIT6  0x40
#define BIT7  0x80

/* global configuration settings
------------------------------------------------------------------------- */
#define E_WATCHDOG   /*HAMA_SERVE_HWWD()  The hardware watchdog can be retriggered here */

//#define EEPROM_TEST   /* Checks plausibility of the transfer parameters */
#define INT_SPERRE_RD  0 /* read - 0: none, 1: local (short), 2: global (long) lock */
#define INT_SPERRE_WR  0 /* write - 0: none, 1: local (short), 2: global (long) blocking */

//#define EEPROM_ERROR_REMANENT /* defined: enter error code in EEPROM */

//#define INT_MESSAGE /* Report interrupt lock (TRACE ticks) */


/* Address PINs wired to the EEPROM */
#define EEPROM_HW_ADR (0x01 << 1)

/* Number of calibration values (4 bytes each): 1..64 */
#define MAX_KALI_VALUES  0


/* Make startup more robust, read init string multiple times?
   defined:   Init string is read several times, if it is never correct, 
              the EEPROM is deleted and reinitialized.
   undefined: init string is read once, if it is incorrect, 
              the EEPROM is deleted and reinitialized.
------------------------------------------------------------------------- */
#define MULTIPLE_INIT_STRING_READ


/* Should the FW version be included in the INIT string check?
   defined:   new FW version deletes the entire EEPROM, 
              also the calibration values are gone
   undefined: new FW version does not delete the EEPROM, 
              calibration values are retained
------------------------------------------------------------------------- */
// #define INIT_CHECK_FW_VERSION


/* Should calibration values from the EEPROM also be removed in flash?
   Goes with a command over the calibration interface. 
   A block of up to 32 related values can be copied from EEPROM 
   to Flash and back. Each value is a double word. 
   Works only with the MSP430!
------------------------------------------------------------------------- */
//#define EEPROM_2_FLASH_ENABLE

#ifdef EEPROM_2_FLASH_ENABLE

  #define FIRST_KALI_VAL_4_FLASH 0
  #define ANZ_KALI_VAL_4_FLASH   38

#endif


/* Should the functions EEPROM _..... Block_linear () be able to 
   access the entire EEPROM area? ---> define 
   Or are the accesses only allowed according to the area divided 
   in Pages for I&M and Kali? ------> comment out
------------------------------------------------------------------------- */
//#define ALLOW_COMPLETE_LINEAR_ACCESS


/* What ends up in the EEPROM?
------------------------------------------------------------------------- */

/* Pages for I&M data
------------------------------------------------------------------------- */
#define INHALTS_PAGE     0
#define DS231_BUF0_PAGE  1
#define DS232_BUF0_PAGE  2
#define DS233_BUF0_PAGE  3
#define DS234_BUF0_PAGE  4

/* ### 09.11.2007 MaSi: new for IOL master - beginning ### */

#define TL17_BUF0_PAGE   5
#define TL18_BUF0_PAGE   6
#define TL19_BUF0_PAGE   7
#define TL20_BUF0_PAGE   8

/* ### 09.11.2007 MaSi: new for IOL master - end   ### */

/* Total number of pages for I&M
------------------------------------------------------------------------- */
#define I_M_PAGES        9


/* Pages for calibration data
------------------------------------------------------------------------- */
#if (MAX_KALI_VALUES > 16)
  #define KALIBRIER_PAGE_0  10
  #define KALIBRIER_PAGE_1  11
  #define KALIBRIER_PAGE_2  12
  #define KALIBRIER_PAGE_3  13
#else
  #define KALIBRIER_PAGE    100
#endif

/* Total number of pages for calibration
------------------------------------------------------------------------- */
#if (MAX_KALI_VALUES > 16)
  #define KALIBRIER_PAGES    4
#else
  #define KALIBRIER_PAGES    1
#endif


/* PIN controls for the I2C interface
------------------------------------------------------------------------- */

#ifdef ATMEL_SAM9X25


typedef struct
{
  Unsigned16    szl_id;
  Unsigned16    index;
  Unsigned16    len;
  Unsigned16    anz_bloecke;

} IUA_KOPF_INFO_STRUC;

#define SET_I2C_DATA_OUT()   HAL_ARM_AT91_GPIO_CFG_DIRECTION (AT91_GPIO_PA30,  AT91_PIN_OUT)  /* TWD0 */
#define SET_I2C_DATA_IN()    HAL_ARM_AT91_GPIO_CFG_DIRECTION (AT91_GPIO_PA30,  AT91_PIN_IN)   /* TWD0 */
#define SET_I2C_DATA()       HAL_ARM_AT91_GPIO_PUT (AT91_GPIO_PA30,  1)
#define RES_I2C_DATA()       HAL_ARM_AT91_GPIO_PUT (AT91_GPIO_PA30,  0)
#define GET_I2C_DATA()       HAL_ARM_AT91_GPIO_GET_func()

#define SET_I2C_CLK_OUT()    HAL_ARM_AT91_GPIO_CFG_DIRECTION (AT91_GPIO_PA31,  AT91_PIN_OUT)  /* TWCKO */
#define SET_I2C_CLK()        HAL_ARM_AT91_GPIO_PUT (AT91_GPIO_PA31,  1)
#define RES_I2C_CLK()        HAL_ARM_AT91_GPIO_PUT (AT91_GPIO_PA31,  0)

#define SET_WC_OUT()         /* hardwired          */
#define SET_WC()             /* Writing prohibited */
#define RES_WC()             /* Writing allowed    */


#define WAIT_WR_HI()         asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");

#define WAIT_WR_LO()         asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
		                     asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");

#define WAIT_RD_HI()

#define WAIT_RD_LO()         asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");


#define WAIT_CTRL()          asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");\
							 asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");

#elif defined (MSP430_IAR)

    #define SET_I2C_DATA_OUT()   P3DIR |=  BIT4
    #define SET_I2C_DATA_IN()    P3DIR &= ~BIT4
    #define SET_I2C_DATA()       P3OUT |=  BIT4
    #define RES_I2C_DATA()       P3OUT &= ~BIT4
    #define GET_I2C_DATA()      (P3IN  &   BIT4)

    #define SET_I2C_CLK_OUT()    P3DIR |=  BIT5
    #define SET_I2C_CLK()        P3OUT |=  BIT5
    #define RES_I2C_CLK()        P3OUT &= ~BIT5

    #define SET_WC_OUT()         P2DIR |=  BIT7
    #define SET_WC()             P2OUT |=  BIT7 /* Writing prohibited */
    #define RES_WC()             P2OUT &= ~BIT7 /* Writing allowed    */


    #define WAIT_WR_HI()         __no_operation()
    #define WAIT_WR_LO()         __no_operation()
    #define WAIT_RD_HI()
  	#define WAIT_RD_LO()         __no_operation();__no_operation();\
                                 __no_operation();__no_operation()

    #define WAIT_CTRL()          __no_operation();__no_operation();\
                                 __no_operation()

#elif defined(TMS470_IAR)

    #define SET_I2C_DATA_OUT()   GIODIRA  |=  BIT1
    #define SET_I2C_DATA_IN()    GIODIRA  &= ~BIT1
    #define SET_I2C_DATA()       GIODSETA = BIT1
    #define RES_I2C_DATA()       GIODCLRA = BIT1
    #define GET_I2C_DATA()      (GIODINA  &   BIT1)

    #define SET_I2C_CLK_OUT()    GIODIRA  |=  BIT2
    #define SET_I2C_CLK()        GIODSETA = BIT2
    #define RES_I2C_CLK()        GIODCLRA = BIT2

    #define SET_WC_OUT()         GIODIRA  |=  BIT3
    #define SET_WC()             GIODSETA = BIT3 /* Writing prohibited */
    #define RES_WC()             GIODCLRA = BIT3 /* Writing allowed    */

  #define WAIT_WR_HI()         a_time_wait_us(5)
  #define WAIT_WR_LO()         a_time_wait_us(5)
  #define WAIT_RD_HI()         a_time_wait_us(5)
  #define WAIT_RD_LO()         a_time_wait_us(5)
  #define WAIT_CTRL()          a_time_wait_us(3)

#elif defined(ERTEC_GHS)


    #define SET_I2C_DATA_OUT()   /*HAMA_GPIO_P1_SDA_N_OUTPUT()*/
    #define SET_I2C_DATA_IN()    HAMA_GPIO_P1_SDA_N_INPUT()
    #define SET_I2C_DATA()       HAMA_GPIO_P1_SDA_N_INPUT()
    #define RES_I2C_DATA()       HAMA_GPIO_P1_SDA_N_OUTPUT()
    #define GET_I2C_DATA()       HAMA_GPIO_P1_SDA_N_READ()

    #define SET_I2C_CLK_OUT()
    #define SET_I2C_CLK()        HAMA_GPIO_P1_SCL_N_INPUT()
    #define RES_I2C_CLK()        HAMA_GPIO_P1_SCL_N_OUTPUT()

    #define SET_WC_OUT()
    #define SET_WC()             /* Writing prohibited */
    #define RES_WC()             /* Writing allowed    */

  #define WAIT_WR_HI()         I2C_NOP_HIGH()
  #define WAIT_WR_LO()         I2C_NOP_LOW()
  #define WAIT_RD_HI()         I2C_NOP_HIGH()
  #define WAIT_RD_LO()         I2C_NOP_LOW()
  #define WAIT_CTRL()          I2C_NOP_START()

#elif defined(NEC850_IAR)

  // PM9 after RESET = 0xFFFF  ---  0 = output / 1 = input

#if 1 /* Use bit commands */

    #define SET_I2C_DATA_OUT()   PM9H_bit.no2 = 0
    #define SET_I2C_DATA_IN()    PM9H_bit.no2 = 1
    #define SET_I2C_DATA()       P9H_bit.no2  = 1
    #define RES_I2C_DATA()       P9H_bit.no2  = 0
    #define GET_I2C_DATA()      (P9H_bit.no2)

    #define SET_I2C_CLK_OUT()    PM9H_bit.no4 = 0
    #define SET_I2C_CLK()        P9H_bit.no4  = 1
    #define RES_I2C_CLK()        P9H_bit.no4  = 0

    #define SET_WC_OUT()         PM9H_bit.no5 = 0
    #define SET_WC()             P9H_bit.no5  = 1
    #define RES_WC()             P9H_bit.no5  = 0

#else

    #define SET_I2C_DATA_OUT()   PM9 &= ~BIT10
    #define SET_I2C_DATA_IN()    PM9 |=  BIT10
    #define SET_I2C_DATA()       P9  |=  BIT10
    #define RES_I2C_DATA()       P9  &= ~BIT10
    #define GET_I2C_DATA()      (P9  &   BIT10)

    #define SET_I2C_CLK_OUT()    PM9 &= ~BIT12
    #define SET_I2C_CLK()        P9  |=  BIT12
    #define RES_I2C_CLK()        P9  &= ~BIT12

    #define SET_WC_OUT()         PM9 &= ~BIT13
    #define SET_WC()             P9  |=  BIT13
    #define RES_WC()             P9  &= ~BIT13

#endif

                               /* Adjust the number of NOPs! */
    #define WAIT_WR_HI()         __no_operation()
    #define WAIT_WR_LO()         __no_operation()
    #define WAIT_RD_HI()
  #define WAIT_RD_LO()         __no_operation();__no_operation();\
                               __no_operation();__no_operation()

    #define WAIT_CTRL()          __no_operation();__no_operation();\
                               __no_operation()

#else
  #error "Unbekannte CPU"
#endif


/* Storage attributes
------------------------------------------------------------------------- */


/* Prototypes
------------------------------------------------------------------------- */


#define EEP_REQ_WR_DONE()                                                 \
{                                                                         \
    /* on an occasion of read execution  */                               \
    eep_perform_services();                                               \
}

#define EEP_TRIGGER_PFS()                                                 \
{                                                                         \
    /* apma_eep_set_flag (); EEP request interface not used  */          \
}



/* fatal error handling */

extern LSA_VOID   PnpbLsasFatalError (LSA_FATAL_ERROR_TYPE* pLsaErr);

#define EEP_FATAL_ERROR(_ERROR_DETAIL_PTR)                                   \
{                                                                            \
    /*apma_spec_fatal_error( APMA_PDB_FW_MODULE_ID_EEP, _ERROR_DETAIL_PTR);*/\
	    PnpbLsasFatalError (_ERROR_DETAIL_PTR); \
}

#endif // Involvement

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
