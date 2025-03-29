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
/*  F i l e               &F: ertec_x00.h                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
/* ertecx00.h - ertecx00 startet kit board header */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,07aug06,mk   user timestamp.
01a,26apr05,jft  modifed from ertec400.

Wind River Services - Osterfeldstrasse 84 - D-85737 Ismaning, Germany
*/

/*
This file contains I/O address and related constants for the ERTEC200 BSP.
*/

#ifndef INCertecx00h
#define INCertecx00h

#ifdef __cplusplus
extern "C" {
#endif


#define IVEC_TO_INUM(intVec)    ((int) (intVec))

/* usability marcos */
#define ERTECX00_BIT_SET(reg, data)     ( (reg) |= (data) )
#define ERTECX00_BIT_CLR(reg, data)     ( (reg) &= ~(data) )

/* to make GNU compiler checking happy */
#ifdef _WRS_GNU_VAR_MACROS
    #define _WRS_ARGUNUSED  __attribute__ ((__unused__))
#else
    #define _WRS_ARGUNUSED
#endif

/* POF GPIO Ports */
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200P)
  // fix03.2
    #define GPIO_POF2_SDA	            6
    #define GPIO_POF2_SCL	            7
    #define GPIO_POF1_SDA	            4
    #define GPIO_POF1_SCL	            5
#elif (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200_400)
    #define GPIO_POF2_SDA	            2
    #define GPIO_POF2_SCL	            3
    #define GPIO_POF1_SDA	            13
    #define GPIO_POF1_SCL	            15
#else     // not supported, only for simulation
    #define GPIO_POF2_SDA	            0 
    #define GPIO_POF2_SCL	            0
    #define GPIO_POF1_SDA	            0
    #define GPIO_POF1_SCL	            0
#endif

/* Flash definitions */
#define FLASH_NV_ADDR               FLASH_ADRS
#define FLASH_NV_SIZE               FLASH_SIZE     /* writeable Flash available */

/* EMIF settings (see manual) */
/* note:
 * EMIF settings normally done in ertec bootcode!
 * this values can be changed in header.tcl.
 */
#define ERTECX00_ASYNC_BANK0_CFG    0x04622311
#define ERTECX00_ASYNC_BANK1_CFG    0x04622311
#define ERTECX00_ASYNC_BANK2_CFG    0x44622312
#define ERTECX00_ASYNC_BANK3_CFG    0x04622312

/* part of the memorymap */
#define   IRTE_SWITCH_START         ERTEC_SWITCH_START  /* IRTE SWITCH Base Addr */


#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB200)
    #define   GPIO_POLARITY_SELECT  ERTEC_GPIO_START + 0x00000014 /* Config GPIO IRQ POLARITY */
    #define   GPIO2_IOCTRL_REG      ERTEC_GPIO_START + 0x00000020 /* General Purpose 2 iocontrole */
    #define   GPIO2_OUT_REG         ERTEC_GPIO_START + 0x00000024 /* General Purpose 2 out */
    #define   GPIO2_IN_REG          ERTEC_GPIO_START + 0x00000028 /* General Purpose 2 in */
#endif

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_EB400)
    #define    EB400_SER_PHY_AUX_MODE_ADR                           29
    #define    EB400_SER_PHY_AUXILIARY_MODE_BIT__LinkLEDDisable     3
#endif


/* macros */
#define   READ_REG_ULONG(adr)         (*(volatile unsigned long  * const) (adr))
#define   WRITE_REG_ULONG(adr,value)  (*(volatile unsigned long  * const) (adr) = value)
#define   READ_REG_USHORT(adr)        (*(volatile unsigned short * const) (adr))
#define   WRITE_REG_USHORT(adr,value) (*(volatile unsigned short * const) (adr) = value)
#define   READ_REG_UCHAR(adr)         (*(volatile unsigned char  * const) (adr))
#define   WRITE_REG_UCHAR(adr,value)  (*(volatile unsigned char  * const) (adr) = value)

#define   SWAP_ULONG(x)  ((x) = ((ULONG)(x)<<24) | (((ULONG)(x) & 0x0000FF00)<<8) | \
                              (((ULONG)(x) & 0x00FF0000)>>8) | ((ULONG)(x)>>24))


#ifndef _ASMLANGUAGE
    typedef enum
    {
        ERTECX00_GPIO_OUT = 0,
        ERTECX00_GPIO_IN
    } ERTECX00_GPIO_DIRECTION;
    
    typedef enum
    {
        ERTECX00_GPIO_FUNC_0 = 0,
        ERTECX00_GPIO_FUNC_1,
        ERTECX00_GPIO_FUNC_2,
        ERTECX00_GPIO_FUNC_3
    } ERTECX00_GPIO_FUNCTION;
    
    #if (PNIOD_PLATFORM & PNIOD_PLATFORM_EBx00)
        typedef enum
        {
            ERTEC200_GPIO_NORMAL_POLARITY = 0,
            ERTEC200_GPIO_INVERTED_POLARITY
        } ERTEC200_GPIO_POLARITY;
    #endif
#endif /* _ASMLANGUAGE */


/* function declarations */
extern ERTECX00_GPIO_DIRECTION      ertecx00GpioDirectionGet    (PNIO_UINT32 port);
extern PNIO_BOOL                    ertecx00GpioDirectionSet    (PNIO_UINT32 port, ERTECX00_GPIO_DIRECTION direction);
extern ERTECX00_GPIO_FUNCTION       ertecx00GpioFunctionGet     (PNIO_UINT32 port);
extern PNIO_BOOL                    ertecx00GpioFunctionSet     (PNIO_UINT32 port, ERTECX00_GPIO_FUNCTION function);
extern PNIO_BOOL                    ertecx00GpioIn              (PNIO_UINT32 port);
extern PNIO_UINT32                  ertecx00GpioOut             (PNIO_UINT32 port, PNIO_BOOL output);
extern PNIO_VOID                    sysGpioInit                 (PNIO_VOID);

#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_EB200)
/**@note am2219 - implement ertec200GpioPolaritySet? */
//extern STATUS ertec200GpioPolaritySet (UINT32 port, ERTEC200_GPIO_POLARITY polarity);
#endif


#ifdef __cplusplus
}
#endif

#endif  /* ertec_x00.h */

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
